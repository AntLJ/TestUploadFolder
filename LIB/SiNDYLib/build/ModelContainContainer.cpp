/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#include "stdafx.h"
#include "ModelContainContainer.h"
#include "GlobalFunctions.h"
#include "RuleModel.h"
#include "Feature.h"
#include "TableTypeUtil.h"

using namespace sindy;
using namespace sindy::errorcode;

CModelContainContainer::CModelContainContainer(void)
{
}

CModelContainContainer::~CModelContainContainer(void)
{
}

// 形状で検索する
sindyErrCode CModelContainContainer::Select( IGeometry* ipGeom, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects *pErrs/* = NULL*/, bool bInit /*= true*/  )
{
	// ジオメトリがNULLでも検索できるが、ここではあえて
	// エラーとすることで予期しない全件検索を回避する
	RETURN_IF( ! ipGeom, sindyErr_ArgIsNull )

	// ベースフィーチャを検索する
	sindyErrCode emErr = CFeatureContainer::Select( ipGeom, bRecycling, bCreateCache, pErrs, bInit );
	if( emErr != sindyErr_NoErr ) // エラーログはCFeatureContainerで出るのでいらない
		return emErr;

	// 形状作成(含む側）
	IGeometryCollectionPtr ipGeomBag( CLSID_GeometryBag );

	((IGeometryPtr)ipGeomBag)->putref_SpatialReference( GetSpatialReference() );
	ipGeomBag->AddGeometry( ipGeom );

	for ( CContainer::const_rows_iterator itContain = CContainer::begin(GetTableType()); itContain != CContainer::end(GetTableType()); ++itContain )
		ipGeomBag->AddGeometry( (*itContain)->GetShapeCopy() );

	// 内包されるフィーチャを検索する
	for( CRuleRelationshipContain::const_value_iterator it = g_cRuleRelationshipContain.begin(GetTableName()), itEnd = g_cRuleRelationshipContain.end(GetTableName()); it != itEnd; ++it )
	{
		IGeometryCollectionPtr ipGeomContainBag( CLSID_GeometryBag );
		sindyTableType::ECode emContainType = AheTableName2TableType( it->first );
		CFeatureClass cClass( OpenSameOwnerTable( emContainType ) );
		((IGeometryPtr)ipGeomContainBag)->putref_SpatialReference( cClass.GetSpatialReference() );

		IFeatureCursorPtr ipCursor( cClass.Search( AheInitSpatialFilter( NULL, (IGeometryPtr)ipGeomBag ), bRecycling ) );
		LOGASSERTEERR_IF( ipCursor, sindyErr_SearchFailed )
		{
			CSPFieldMap spFieldMap = cClass.GetFieldMap();
			CSPTableNameString spNameString = cClass.GetNameString();

			IFeaturePtr ipFeature;
			while( S_OK == ipCursor->NextFeature( &ipFeature ) )
			{
				if( ipFeature == NULL ) break;

				CSPRow cFeat( new CFeature( ipFeature, emContainType, false, spFieldMap, spNameString ) );

				// 追加検索モードの場合、同じフィーチャであったら登録はしない
				bool bFind = false;

				// 既に関係づけられている？
				for ( const_rows_iterator itContain = CContainer::begin(GetTableType()); itContain != CContainer::end(GetTableType()); ++itContain ) {
					if ( (*itContain)->find( *cFeat.get() ) ) {
						bFind = true;
						break;
					}
				}

				if ( !bFind ) {
					// 関係づけられていないけど格納済み？
					if ( find( *cFeat.get() ) )
						bFind = true;
				}

				if( bInit || ! bFind )
				{
					// リサイクルする場合はキャッシュを作る
					if( bRecycling || bCreateCache )
						cFeat->CreateCache( bRecycling ? true : false );
				}

				if ( !bFind ) {
					// 関連付け
					for ( rows_iterator itContain = CContainer::begin(GetTableType()); itContain != CContainer::end(GetTableType()); ++itContain ) {
						if ( (*itContain)->GetShape()->Contains( cFeat->GetShapeCopy() ) || (*itContain)->GetShape()->Equals( cFeat->GetShapeCopy() ) ) {
							(*itContain)->push_back( cFeat );

							// 形状作成(含まれる側）
							ipGeomContainBag->AddGeometry( cFeat->GetShapeCopy() );
						}
					}
				}
			}

			long nCount = 0;

			ipGeomContainBag->get_GeometryCount( &nCount );

			if ( nCount > 0 ) {
				// IGeometryCollectionに格納されている形状を一つにまとめる [Arc10.3.1移行対応]
				// (2017/3/29)都市建物ポリゴン、都市建物階数ポリゴンで検証を実施。
				// PolylineもUnion可能であるが、現在のモデルではこの関数を利用して紐付けを行うPolylineのレイヤがないため未検証。
				CGeometry unionGeom;
				for (long l = 0; l < nCount; ++l)
				{
					IGeometryPtr geom;
					ipGeomContainBag->get_Geometry(l, &geom);
					emErr = unionGeom.Union(geom);
						if (emErr != sindyErr_NoErr)
							return emErr;
				}
				// 含む側の追加検索
				sindyErrCode emErr = CModelContainContainer::Select( unionGeom, bRecycling, bCreateCache, pErrs, false );
				if( emErr != sindyErr_NoErr ) // エラーログはCFeatureContainerで出るのでいらない
					return emErr;
			}

			ipCursor = NULL;
		}
	}

	return emErr;
}

// 与えられた形状との交差部分で分割処理を行う
errorcode::sindyErrCode CModelContainContainer::Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects )
{
	errorcode::sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( pSource, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

		if( pFeature )
		{
			_Split( pSource, cGeom, cErrObjects );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// 与えられた形状との交差部分で分割処理を行う
errorcode::sindyErrCode CModelContainContainer::_Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects )
{
	// TODO: CRoadNetwork::_Split、CFeatures::Splitと似たようなコードあり。統一すべき！！
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( pSource, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

		if( pFeature )
		{
			if( GetTableType() == pSource->GetTableType() )
			{
				// 含まれる側をSplit
				for( CRuleRelationshipContain::const_value_iterator itRule = g_cRuleRelationshipContain.begin(GetTableName()),
					itRuleEnd = g_cRuleRelationshipContain.end(GetTableName());
					itRule != itRuleEnd; ++itRule )
				{
					CContainer cContains;
					sindyTableType::ECode emContainType = AheTableName2TableType( itRuleEnd->first );

					for( const_rows_iterator itCon = pSource->begin(emContainType),
						itConEnd = pSource->end(emContainType);
						itCon != itConEnd; ++itCon )
						cContains.push_back( (*itCon) );

					for ( CContainer::rows_iterator itContain = cContains.begin(emContainType), itContainEnd = cContains.end(emContainType); itContain != itContainEnd; ++itContain )
						_Split( (*itContain), cGeom, cErrObjects );
				}

				CGeometry cSplittedGeoms, cSplittedPoints;

				// Split
				if ( sindyErr_NoErr == pFeature->Split( cGeom, cSplittedGeoms, cSplittedPoints, cErrObjects ) ) {
					if ( (IGeometry*)cSplittedGeoms ) {
						if ( cSplittedGeoms.GetPartCount() > 0 ) {
							// 分割形状を割り当てる
							for ( CGeometry::const_part_iterator itPart = cSplittedGeoms.part_begin(), itPartEnd = cSplittedGeoms.part_end(); itPart != itPartEnd; ++itPart ) {
								CGeometry cNewGeom = (IGeometryPtr)AheCreateClone( (*itPart) );

								if ( (IGeometry*)cNewGeom ) {
									CSPRow pCon;
									WKSPoint* point_wks = new WKSPoint[cNewGeom.GetPointCount()];

									((IPointCollectionPtr)(IGeometry*)cNewGeom)->QueryWKSPoints( 0, cNewGeom.GetPointCount(), point_wks );

									if ( itPart == cSplittedGeoms.part_begin() ) {
										// 現在の中身と入れ替える
										((IPointCollectionPtr)(IGeometry*)*pFeature->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );
										pCon = pSource;
									} else {
										// フィーチャ作成
										pCon.reset( new CFeature( NULL, pSource->GetTableType(), true, pSource->GetFieldMap(), pSource->GetNameString() ) );

										CTable( pSource->GetTable() ).CreateRow( *pCon );

										// 属性をコピー
										pSource->CopyTo( *pCon, true, false, false, false );

										// 形状をセット
										((IPointCollectionPtr)(IGeometry*)*pCon->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );

										// 含む側を格納
										push_back( pCon );
									}
									delete [] point_wks;
									point_wks = NULL;
								}
							}

							// 含まれる側関連付け
							for( CRuleRelationshipContain::const_value_iterator itRule = g_cRuleRelationshipContain.begin(GetTableName()),
								itRuleEnd = g_cRuleRelationshipContain.end(GetTableName());
								itRule != itRuleEnd; ++itRule )
							{
								CContainer cContains;
								sindyTableType::ECode emContainType = AheTableName2TableType( itRule->first );

								// 変更されたものを取得
								for ( const_rows_iterator itContain = begin(GetTableType()); itContain != end(GetTableType()); ++itContain ) {
									for( const_rows_iterator itCon = (*itContain)->begin(emContainType),
										itConEnd = (*itContain)->end(emContainType);
										itCon != itConEnd; ++itCon ) {
											schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

											if ( (*itCon)->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kDeleted )
												cContains.push_back( (*itCon) );
									}
								}

								// 関連付け
								for ( CContainer::const_rows_iterator itCon = cContains.begin(AheTableName2TableType(itRule->first)), itConEnd = cContains.end(AheTableName2TableType(itRule->first)); itCon != itConEnd; ++itCon ) {
									for ( rows_iterator itContain = CContainer::begin(GetTableType()); itContain != CContainer::end(GetTableType()); ++itContain ) {
										if ( (*itContain)->GetShape()->Contains( (*itCon)->GetShapeCopy() ) || (*itContain)->GetShape()->Equals( (*itCon)->GetShapeCopy() ) ) {
											// 格納
											(*itContain)->push_back( (*itCon) );
										} else
											// 含む側が変わったので除外
											(*itContain)->erase( (*itCon) );
									}
								}
							}
						}
					}
				}
			}
			else {
				// TODO: 上のコードにも似た箇所があり。なんとか統一すること！！
				CGeometry cSplittedGeoms, cSplittedPoints;

				// Split
				if ( sindyErr_NoErr == pFeature->Split( cGeom, cSplittedGeoms, cSplittedPoints, cErrObjects ) ) {
					if ( (IGeometry*)cSplittedGeoms ) {
						if ( cSplittedGeoms.GetPartCount() > 0 ) {
							CSPRow pContain;

							for( rows_iterator it = begin(GetTableType()),
								itEnd = end(GetTableType());
								it != itEnd; ++it )
							{
								// 所属する含む側を取得
								for ( const_rows_iterator itContain = begin(GetTableType()); itContain != end(GetTableType()); ++itContain ) {
									for( CRuleRelationshipContain::const_value_iterator itRule = g_cRuleRelationshipContain.begin(GetTableName()),
										itRuleEnd = g_cRuleRelationshipContain.end(GetTableName());
										itRule != itRuleEnd; ++itRule )
									{
										sindyTableType::ECode emContainType = AheTableName2TableType( itRule->first );

										for( const_rows_iterator itCon = (*itContain)->begin(emContainType),
											itConEnd = (*itContain)->end(emContainType);
											itCon != itConEnd; ++itCon )
										{
											if ( pSource == (*itCon) ) {
												pContain = *itContain;
												break;
											}
										}
									}

									if ( pContain )
										break;
								}

								if ( pContain )
									break;
							}

							if ( pContain ) {
								// 含む側に作成フィーチャを格納
								CTable cTable( pSource->GetTable() );

								// 分割形状を割り当てる
								for ( CGeometry::const_part_iterator itPart = cSplittedGeoms.part_begin(), itPartEnd = cSplittedGeoms.part_end(); itPart != itPartEnd; ++itPart ) {
									CGeometry cNewGeom = (IGeometryPtr)AheCreateClone( (*itPart) );

									if ( (IGeometry*)cNewGeom ) {
										WKSPoint* point_wks = new WKSPoint[cNewGeom.GetPointCount()];

										((IPointCollectionPtr)(IGeometry*)cNewGeom)->QueryWKSPoints( 0, cNewGeom.GetPointCount(), point_wks );

										if ( itPart == cSplittedGeoms.part_begin() ) {
											// 現在の中身と入れ替える
											((IPointCollectionPtr)(IGeometry*)*pFeature->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );
										} else {
											// フィーチャ作成
											CSPRow cNew;

											cNew.reset( new CFeature( NULL, pSource->GetTableType(), true, pSource->GetFieldMap(), pSource->GetNameString() ) );

											cTable.CreateRow( *cNew.get() );

											// 属性をコピー
											pSource->CopyTo( *cNew.get(), true, false, false, false );

											// 形状をセット
											((IPointCollectionPtr)(IGeometry*)*cNew->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );

											// 含む側に追加
											pContain->push_back( cNew );
										}
										delete [] point_wks;
										point_wks = NULL;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CModelContainContainer::Merge( CSPRow pSource, CSPRow pDest, CErrorObjects& cErrObjects )
{
	errorcode::sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( pSource && pDest, sindyErr_ArgIsNull )
	{
		if( *pSource->GetNameString() == *pDest->GetNameString() )	// 同じタイプ？
		{
			CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

			if ( pFeature ) {
				// [Bug 5798]含む側のマージの時は、含まれる側の所属を変更する。
				// 包含関係ではないマージの時は、含まれる側の所属を変更するのか、含まれる側もマージするのか、
				// 後々選択できなきゃいけない気がするけど、使ってるのDeleteToolだけだから所属を変更するほうにしとく
				// 含まれる側のマージ時は、含む側はなにもしない
				if( GetTableType() == pSource->GetTableType() )	// 含む側？
				{
					// 削除される含む側に所属する含まれる側を変更
					for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
						itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
						itContain != itContainEnd; ++itContain )
					{
						sindyTableType::ECode emContainType = AheTableName2TableType( itContain->first );
						CContainer cContainContainer;	// 一時用

						// TODO : CContainer::push_back()/CContainer::erase()が、iteratorを返してくれないから
						//        一時用に格納してから追加/削除してる。
						// 一時用に格納
						for( rows_iterator itCon = pDest->begin(emContainType),
							itConEnd = pDest->end(emContainType);
							itCon != itConEnd; ++itCon )
						{
							cContainContainer.push_back( (*itCon) );
						}

						// 所属を変更
						for( rows_iterator itCon = cContainContainer.begin(emContainType),
							itConEnd = cContainContainer.end(emContainType);
							itCon != itConEnd; ++itCon )
						{
							pSource->push_back( (*itCon) );
							pDest->erase( (*itCon) );	// 削除されないように除外
						}
					}

					// 形状のマージ
					if ( sindyErr_NoErr == pFeature->Merge( *pDest->GetShape(), cErrObjects ) ) {
						// マージ先の削除
						pDest->Delete();
					}
				}
				else
				{
					// 形状のマージ
					if ( sindyErr_NoErr == pFeature->Merge( *pDest->GetShape(), cErrObjects ) ) {
						// マージ先の削除
						pDest->Delete();
					}
				}
			}
		} else {
			cErrObjects.push_back( CErrorObject( (*pSource) ) );
			cErrObjects.push_back( CErrorObject( (*pDest) ) );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CModelContainContainer::Erase( CSPRow pRow, const CGeometry& cGeom, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pRow.get() );

		if ( pFeature ) {
			CGeometry cIntersect = pFeature->GetShape()->IntersectPoints( cGeom );

			if( (IGeometry*)cIntersect && ! cIntersect.IsEmpty() )
			{ 
				if( GetTableType() == pRow->GetTableType() )
				{
					// 関連オブジェクトの削除
					if ( sindyErr_NoErr != pFeature->GetShape()->Erase( cIntersect ) )
						cErrObjects.push_back( (*pRow) );

					for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
						itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
						itContain != itContainEnd; ++itContain )
					{
						sindyTableType::ECode emContainType = AheTableName2TableType( itContain->first );

						for( rows_iterator itCon = pRow->begin(emContainType),
							itConEnd = pRow->end(emContainType);
							itCon != itConEnd; ++itCon )
						{
							if ( sindyErr_NoErr != Erase( *itCon, cIntersect, cErrObjects ) )
								break;
						}
					}
				}
				else {
					// 削除
					if ( sindyErr_NoErr != pFeature->GetShape()->Erase( cIntersect ) )
						cErrObjects.push_back( (*pRow) );

					// 含む側の削除
					for( rows_iterator it = begin(GetTableType()),
						itEnd = end(GetTableType());
						it != itEnd; ++it )
					{
						bool bHasCon = false;

						// 内包関係のもの
						for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
							itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
							itContain != itContainEnd; ++itContain )
						{
							sindyTableType::ECode emContainType = AheTableName2TableType( itContain->first );

							for( const_rows_iterator itCon = (*it)->begin(emContainType),
								itConEnd = (*it)->end(emContainType);
								itCon != itConEnd; ++itCon )
							{
								if ( pRow == (*itCon) ) {
									bHasCon = true;
									break;
								}
							}

							if ( bHasCon )
								break;
						}

						if ( bHasCon )
							// 削除
								Erase( *it, cIntersect, cErrObjects );
					}
				}
			}
		}
		else
			cErrObjects.push_back( CErrorObject( (*pRow) ) );
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CModelContainContainer::Delete( CSPRow pRow, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		CFeature* pFeature = dynamic_cast<CFeature*>( pRow.get() );

		if( pFeature )
		{
			if( GetTableType() == pRow->GetTableType() )
			{
				// 関連オブジェクトの削除はCRow::Delete()で
				// 行うようにした
				pRow->Delete(true);
			}
			else {
				// TODO: pRowが含まれる側の場合、これを削除することによって
				// 含まれるものが0になった場合は含む側も削除するのが現在の仕様
				// 本当は、包含関係が 1...n、0...nのどちらかが分からないとだめ
				// なのでそのようなルールを追加する必要がる。
				// で、削除した結果0になるかどうかは、CContainer::size()で
				// テーブルタイプの他に削除したものを含むかどうかのフラグを引数
				// に追加できるようにすればコード削減となるはずなので後で
				// 書くこと

				// 削除
				pRow->Delete();

				// 含む側の削除
				for( rows_iterator it = begin(GetTableType()),
					itEnd = end(GetTableType());
					it != itEnd; ++it )
				{
					bool bHasCon = false;

					// 内包関係のもの
					for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
						itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
						itContain != itContainEnd; ++itContain )
					{
						sindyTableType::ECode emContainType = AheTableName2TableType( itContain->first );

						for( const_rows_iterator itCon = (*it)->begin(emContainType),
							itConEnd = (*it)->end(emContainType);
							itCon != itConEnd; ++itCon )
						{
							if ( pRow == (*itCon) ) {
								bHasCon = true;
								break;
							}
						}

						if ( bHasCon )
							break;
					}

					if ( bHasCon )
						// 削除
							Delete( (*it), cErrObjects );
				}
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// 編集内容を保存する
errorcode::sindyErrCode CModelContainContainer::Store( CErrorObjects& cErrObjects )
{
	// モデルと内包関係モデル
	for( iterator it = begin(); it != end(); ++it )
	{
		for( rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			(*it2)->Store( cErrObjects );
	}

	return cErrObjects.GetDefaultErrCode();
}

// 整合性をチェックする
errorcode::sindyErrCode CModelContainContainer::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const
{
	// 重複チェック等
	CContainer::CheckLogic( cRule, cErrObjects );

	// TODO : 誰かが実装すること
	for ( const_rows_iterator it = CContainer::begin(GetTableType()); it != CContainer::end(GetTableType()); ++it ) {
		schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

		(*it)->Changed( &emUpdateType );
		if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
			(*it)->CheckLogic( cRule, cErrObjects );

			// 含まれる側の重複チェック等
			static_cast<CContainer>(*(*it)).CheckLogic( cRule, cErrObjects );

			// 複合チェック
			CGeometry cContainGeom;

			for( CRuleRelationshipContain::const_value_iterator itRule = g_cRuleRelationshipContain.begin(GetTableName()), itRuleEnd = g_cRuleRelationshipContain.end(GetTableName()); itRule != itRuleEnd; ++itRule )
			{
				sindyTableType::ECode emContainType = AheTableName2TableType( itRule->first );

				for ( const_rows_iterator itContain = (*it)->begin(emContainType); itContain != (*it)->end(emContainType); ++itContain ) {
					emUpdateType = schema::ipc_table::update_type::kDefault;

					(*itContain)->Changed( &emUpdateType );
					if ( emUpdateType != schema::ipc_table::update_type::kDeleted )
						cContainGeom.Union( (*itContain)->GetShapeCopy() );
				}
			}

			// 含まれる側がないのはおかしい
			if ( !(IGeometry*)cContainGeom )
				cErrObjects.push_back( CErrorObject( *it->get(), CErrorInfo( sindyErr_BadModelContainContain, sindyErrLevel_ERR ) ) );
			else {
				// Equalsだと、だめぽっていわれる事（点列の順番が違うとかで）が多いのでSymmetricDifferenceして、
				// 形状がなければOKであればNG。
				IGeometryPtr ipSymGeomBase = CGeometry::SymmetricDifference( (*it)->GetShapeCopy(), cContainGeom );	// 含む側に対して含まれる側をSymmetricDifference
				IGeometryPtr ipSymGeomContain = CGeometry::SymmetricDifference( cContainGeom, (*it)->GetShapeCopy() );	// 含まれる側に対して含む側をSymmetricDifference

				if ( ipSymGeomBase || ipSymGeomContain ) {
					VARIANT_BOOL vbIsEmptyBase = VARIANT_TRUE;

					if ( ipSymGeomBase )
						ipSymGeomBase->get_IsEmpty( &vbIsEmptyBase );

					VARIANT_BOOL vbIsEmptyContain = VARIANT_TRUE;

					if ( ipSymGeomContain )
						ipSymGeomContain->get_IsEmpty( &vbIsEmptyContain );

					// 含む側の形状と含まれる側の形状で差分があるのはおかしいのでエラー
					if ( ! vbIsEmptyBase || ! vbIsEmptyContain )
						cErrObjects.push_back( CErrorObject( *it->get(), CErrorInfo( sindyErr_BadModelContain, sindyErrLevel_ERR ) ) );
				}
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// コンフリクト回避のための既編集チェックを行う
sindyErrCode CModelContainContainer::CheckModify( CErrorObjects& cErrObjects ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			(*it2)->CheckModify( cErrObjects );
	}

	return cErrObjects.GetDefaultErrCode();
}
