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

/**
 * @file Features.cpp
 * @brief <b>CFeaturesクラス実装ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "Features.h"

#include "util.h"
#include "Map.h"
#include "TableTypeUtil.h"
#include "RoadNetwork.h"
#include "Annotations.h"
#include "GlobalRules.h"
#include "Rule.h"
#include "FeatureContainer.h"
#include "ListString.h"
#include "Workspace.h"
#include "ModelContainContainer.h"
#include "AheNetwork.h"
#include "RuleModel.h"
#include "AnnoGlobals.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

// 検索するためのコンテナをセットする
sindyErrCode CFeatures::SetFeatureContainer( IWorkspace* ipWorkspace, LPCTSTR lpcszName, std::list<CFeatureContainer*>& listContainer )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( ipWorkspace && lpcszName, sindyErr_ArgIsNull )
	{
		CWorkspace cWork( ipWorkspace );

		// オーナー名とテーブル名に分割
		CString strTableName( lpcszName );
		CString strOwnerName;
		if( _tcschr( lpcszName, _T('.') ) )
		{
			int i=0;
			strOwnerName = strTableName.Tokenize( _T("."), i );
			strTableName = strTableName.Tokenize( _T("."), i );
		}

		// テーブルタイプを取得
		sindyTableType::ECode emTableType = AheTableName2TableType( strTableName );

		// テーブルタイプでコンテナをセット
		CFeatureContainer* pContainer = NULL;
		CString strBaseName;

		// ネットワーク系
		if( AheIsRelateWithNetwork( strTableName ) )
		{
			strBaseName = AheGetNetworkLinkFeatureClassName( strTableName ); // ネットワークリンクテーブル名
			pContainer = new CRoadNetwork();
		}
		// 注記系
		else if( AheIsAnnoPointTableType( emTableType ) ||
			AheIsAnnoLineTableType( emTableType ) )
		{
			strBaseName = AheTableType2TableName( AheTableType2AnnoPointTableType( emTableType ) );
			pContainer = new CAnnotations();
		}
		// 内包関連系
		else if( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() || g_cRuleRelationshipContain.find_value( strTableName ) != g_cRuleRelationshipContain.end() )
		{
			if ( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() )
				strBaseName = g_cRuleRelationshipContain.find( strTableName )->first; // 「含む」側のテーブル名
			else
				strBaseName = g_cRuleRelationshipContain.find_value( strTableName )->first;

			pContainer = new CModelContainContainer();
		}
		// その他
		else {
			strBaseName = strTableName;
			pContainer = new CFeatureContainer();
		}

		// 既に読み込まれているかどうかチェック
		bool bFind = false;
		for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		{
			if( typeid(*it) == typeid(pContainer) )
			{
				// オーナー名付きで渡されているときはオーナー名も比較対象にする
				if( 0 == strBaseName.CompareNoCase( (*it)->GetTableName() ) &&
					( strOwnerName.IsEmpty() || 0 == strOwnerName.CompareNoCase( (*it)->GetOwnerName() ) ) )
				{
					bFind = true;
					listContainer.push_back( *it ); // 見つけたものは返却用コンテナに入れておく
					delete pContainer; // TODO: shared_ptr管理にすればいらなくなる
					break;
				}
			}
		}

		// 読み込まれていないなら新規に読み込み・登録
		if( ! bFind )
		{
			// オーナー名.ネットワークリンクテーブル名にする
			CString strName( lpcszName );
			int nPeriodIndex = strName.Find( _T('.'), 0 );
			CString strLinkName = ( 0 < nPeriodIndex ) ? strName.Left( nPeriodIndex ) + _T(".") + strBaseName : strBaseName;

			// オープンできたら登録（bug 9779）
			ITablePtr ipTable = cWork.OpenTable( strLinkName );
			if( ipTable )
			{
				pContainer->SetObject( (IFeatureClassPtr)ipTable );
				m_listFeatureContainer.push_back( pContainer );
				listContainer.push_back( pContainer ); // 新規のものも返却用コンテナに入れておく
			}
		}
	}

	return emErr;
}

// 検索するためのコンテナをセットする
sindyErrCode CFeatures::SetFeatureContainer( IWorkspace* ipWorkspace, const CListString& listNames, std::list<CFeatureContainer*>& listContainer )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	for( CListString::const_iterator it = listNames.begin(), itEnd = listNames.end(); it != itEnd; ++it )
	{
		sindyErrCode emErrTmp = SetFeatureContainer( ipWorkspace, *it, listContainer );
		if( sindyErr_NoErr == emErr )
			emErr = emErrTmp;
	}
	// 重複している可能性があるのでユニークにする
	listContainer.sort();
	listContainer.unique();

	return emErr;
}
	
// フィーチャを選択する
sindyErrCode CFeatures::SelectByShape( IWorkspace* ipWorkspace, IGeometry* ipGeom, LPCTSTR lpcszName, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit/* = true*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( lpcszName, sindyErr_ArgIsNull )
	{
		CListString listNames;
		listNames.push_back( lpcszName );

		emErr = SelectByShape( ipWorkspace, ipGeom, listNames, bRecycling, bCreateCache, pErrs, bInit );
	}

	return emErr;
}

// フィーチャを選択する
sindyErrCode CFeatures::SelectByShape( IWorkspace* ipWorkspace, IGeometry* ipGeom, const CListString& listNames, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* cErrs/* = NULL*/, bool bInit/* = true*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	// 共有編集対象を追加する
	//// TODO: ワークスペースがSDEならオーナー名を必須とする
	//// そうじゃないとどのオーナーのテーブルを開いて良いのかわからない
	CListString listNamesTmp;
	for( CListString::const_iterator it = listNames.begin(); it != listNames.end(); ++it )
	{
		// オーナー名付きであることを考慮してテーブル名のみ取り出す
		CString strTableName, strOwnerName;
		int nPeriodIndex = it->Find( _T('.'), 0 );
		if( 0 < nPeriodIndex )
		{
			strOwnerName = it->Left( nPeriodIndex );
			strTableName = it->Right( it->GetLength() - nPeriodIndex - 1 );
		}
		else
			strTableName = *it;

		// 共有編集対象取得
		CListString listStr;
		g_cRule.GetSharedMovableLayer( strTableName, listStr );

		// 取得できたリストにオーナー名をつけてlistNamesTmpに格納
		for( CListString::const_iterator it = listStr.begin(); it != listStr.end(); ++it )
		{
			CString strNameTmp;
			if( 0 < nPeriodIndex )
				strNameTmp.Format(_T("%s.%s"), strOwnerName, *it );
			else
				strNameTmp = *it;

			listNamesTmp.push_back( strNameTmp );
		}
	}

	// 複数同一テーブル読み込み防止
	listNamesTmp.sort();
	listNamesTmp.unique();

	// コンテナ登録
	std::list<CFeatureContainer*> listContainer; // listNamesTmpにマッチしたコンテナリスト
	LOGASSERTE_IF( sindyErr_NoErr == ( emErr = SetFeatureContainer( ipWorkspace, listNamesTmp, listContainer ) ), emErr )
	{
		// 検索
		for( std::list<CFeatureContainer*>::iterator it = listContainer.begin(); it != listContainer.end(); ++it )
		{
			sindyErrCode emErrTmp = sindyErr_NoErr;
			LOGASSERTE_IF( sindyErr_NoErr == ( emErrTmp = (*it)->Select( ipGeom, bRecycling, bCreateCache, cErrs, bInit ) ), emErrTmp )
			{
			}
			if( sindyErr_NoErr == emErr ) 
				emErr = emErrTmp;	// エラーが起きた場合は最初のエラーコードが返る
		}
	}

// 毎回警告される… TODO: 警告される原因を直し、必ず元に戻すこと（新DeleteTool?）
//#ifdef _DEBUG
//	// キャッシュが作成されたかどうかチェック
//	if( bCreateCache )
//	{
//		if( ! empty() )
//		{
//			LOGASSERTEERR_IF( IsCached(), sindyErr_FeatureCacheNotFound );
//		}
//	}
//#endif // ifdef _DEBUG

	return emErr;
}

// フィーチャを選択する
sindyErrCode CFeatures::SelectByObject( IWorkspace* ipWorkspace, _IRow* ipRow, bool bRecycling/* = false*/, bool bCreateCache/* = false*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( ipWorkspace && ipRow, sindyErr_ArgIsNull )
	{
/*		// 共有移動対象を取得
		CListString listNames;*/
		CRow cRow( ipRow );
/*		g_cRule.GetSharedMovableLayer( cRow.GetTableName(), listNames );*/

		// 変更後の形状で検索
		IGeometryPtr ipGeom;
		IFeaturePtr ipFeature( ipRow );
		
		if( NULL != ipFeature )
			ipFeature->get_Shape( &ipGeom );

		LOGASSERTEERR_IF( NULL != ipGeom, sindyErr_GeometryNotFound )
		{
			LOGASSERTE_IF( sindyErr_NoErr == ( emErr = SelectByShape( ipWorkspace, ipGeom, cRow.GetNameString()->GetOwnerTableName(), bRecycling, bCreateCache ) ), emErr )
			{
				// 変更前の形状で検索
				IFeatureChangesPtr ipChanges( ipRow );
				if( NULL != ipChanges )
				{
					VARIANT_BOOL vbChanged = VARIANT_FALSE;
					ipChanges->get_ShapeChanged( &vbChanged );
					if( vbChanged )
					{
						IGeometryPtr ipOriginalGeom;
						ipChanges->get_OriginalShape( &ipOriginalGeom );
						if( NULL != ipOriginalGeom )
						{
							LOGASSERTE_IF( sindyErr_NoErr == ( emErr = SelectByShape( ipWorkspace, ipOriginalGeom, cRow.GetNameString()->GetOwnerTableName(), bRecycling, bCreateCache ) ), emErr );
						}
					}
				}
			}
		}
	}

	return emErr;
}

//与えられた形状に関連するものをコンテナに格納する
void CFeatures::QueryFeatures( IGeometry* ipGeom, CContainer& cContainer ) const
{
	CGeometry cGeom( ipGeom );
	for( const_iterator it = begin(); it != end(); ++it ) 
		(*it)->Query( sindyCompOpeNotEqual, cGeom, sindyeSpatialRelDisjoint, cContainer );
}

// 登録されているフィーチャがあれば、そのフィーチャのポインタを返します
CRow* CFeatures::FindObject( _IRow* ipRow )
{
	if( ipRow == NULL )
		return NULL;

	CRow* pRet = NULL;	// 返り値

	for( iterator it = begin(); it != end(); it++ )
	{
		pRet = (*it)->find( ipRow ).get();
		if( pRet ) break;
	}

	return pRet;
}

bool CFeatures::IsCached() const
{
	bool bIsCached = false;
	for( const_iterator it = begin(); it != end(); ++it )
	{
		bool bIsCachedTmp = (*it)->IsCached();
		if( ! bIsCached ) bIsCached = bIsCachedTmp;
	}

	return bIsCached;
}

// 与えられた形状の範囲で平行移動処理を行う
sindyErrCode CFeatures::Move( const CGeometry& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

#if 0
	// 検索形状がポイントで、移動モードがsindyeMove_Pointの場合は、
	// 一番近い構成点を移動する。
	// そのために、まずポイントから一番近い構成点を探す
	if( cGeom.GetGeometryType() == esriGeometryPoint && sindyeMove_Point == emMode )
	{
		IPointPtr ipNearestVertex;
		double dNearestDistance = -1;
		for( iterator it = begin(); it != end(); ++it )
		{
			IPointPtr ipVertex = (*it)->ReturnNearestVertex( (IPointPtr)(IGeometry*)cGeom ); <- これじゃ実際の検索形状（多分円になる）よりも外のものがくる可能性があるので、
			double dDistance = cGeom.ReturnDistance( ipVertex );                                やっぱり点での移動の場合は閾値やバッファリング後の形状がないと話にならない
			if( 0 > dNearestDistance || dNearestDistance > dDistance )                          一番現実的なのは、sindyeMove_Pointの時は検索形状の中心から近いものを移動する
			{                                                                                   ようにすることか？？
				ipNearestVertex = ipVertex;                                                     ただし、その場合もここの時点で点を確定する必要がある。
				dNearestDistance = dDistance;                                                   それとも、いっそのこと点も一緒に渡すか？？？
			}
		}
		if( ipNearestVertex != NULL )
			cGeom = ipNearestVertex;
	}
#endif // if 0

	for( iterator it = begin(); it != end(); ++it )
	{
		if( sindyErr_NoErr != ( emErr = (*it)->Move( cGeom, pairMove, emMode, cErrObjects ) ) )
			break;	// エラー
	}

	return emErr;
}

// 点->点の移動処理を行う
sindyErrCode CFeatures::MovePoint( IPoint* ipFrom, IPoint* ipTo, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( ipFrom && ipTo, sindyErr_ArgIsNull )
	{
		WKSPoint pFrom = {-1, -1}, pTo = {-1, -1};
		ipTo->QueryCoords( &pTo.X, &pTo.Y );
		ipFrom->QueryCoords( &pFrom.X, &pFrom.Y );

		emErr = Move( CGeometry( ipFrom ), std::pair<double,double>( pTo.X - pFrom.X, pTo.Y - pFrom.Y ), sindyeMove_Point, cErrObjects );
	}

	return emErr;
}

// 与えられた形状との交差部分で分割処理を行う
errorcode::sindyErrCode CFeatures::Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects )
{
	// TODO: ここのコードはCModelContainContainer::_Aplit、CRoadNetWork::_Splitと共通化すべき
	sindyErrCode emErr = sindyErr_NoErr; // 返り値

	LOGASSERTEERR_IF( pSource, sindyErr_ArgIsNull )
	{
		CString strTableName( CString_LastPeriodRight( pSource->GetTableName() ) );

		if( AheIsRelateWithNetwork( strTableName ) )
		{
			for ( const_iterator it = begin(); it != end(); ++it ) {
				if ( dynamic_cast<CRoadNetwork*>( *it ) ) {	// 道路ネットワーク？
					CRoadNetwork* pRoadNetwork = static_cast<CRoadNetwork*>( (*it) );

					if( 0 == lstrcmpi( pRoadNetwork->GetTableName(), AheGetNetworkLinkFeatureClassName( strTableName ) ) )
						pRoadNetwork->Split( pSource, cGeom, cErrObjects );
				}
			}
		}
		// 内包関連系
		else if( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() || g_cRuleRelationshipContain.find_value( strTableName ) != g_cRuleRelationshipContain.end() )
		{
			for ( const_iterator it = begin(); it != end(); ++it ) {
				if ( dynamic_cast<CModelContainContainer*>( *it ) ) {	// 包含モデル？
					CModelContainContainer* pModelContain = static_cast<CModelContainContainer*>( (*it) );

					CString strBaseName;

					if ( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() )
						strBaseName = g_cRuleRelationshipContain.find( strTableName )->first;
					else
						strBaseName = g_cRuleRelationshipContain.find_value( strTableName )->first;

					if( 0 == lstrcmpi( pModelContain->GetTableName(), strBaseName ) )
						pModelContain->Split( pSource, cGeom, cErrObjects );
				}
			}
		}
		else
		{
			CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

			if ( pFeature ) {
				if ( pFeature->GetShape() ) {
					esriGeometryType emType = pFeature->GetShape()->GetGeometryType();

					switch ( emType ) {
						case esriGeometryPolyline:
						case esriGeometryPolygon:
							{
								CGeometry cSplittedGeoms, cSplittedPoints;

								// Split
								if ( sindyErr_NoErr == pFeature->Split( cGeom, cSplittedGeoms, cSplittedPoints, cErrObjects ) ) {
									if ( (IGeometry*)cSplittedGeoms ) {
										if ( cSplittedGeoms.GetPartCount() ) {
											// 新しくできたオブジェクトをクラス群に格納するために所属するコンテナ取得
											CFeatureContainer* pContainer = NULL;

											for ( iterator it = begin(); it != end(); ++it ) {
												if ( (*it)->find( *pSource.get() ) ) {
													pContainer = (*it);
													break;
												}
											}

											if ( pContainer ) {
												CTable cTable( *pContainer );

												// 分割形状を割り当てる
												// TODO: CRoadNetWork::Splitでは始点を持つ形状がID引継ぎ対象となっているので、共通化する際にはその辺に気をつけること！！
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

															cNew.reset( new CFeature( NULL, pContainer->GetTableType(), true, pContainer->GetFieldMap(), pContainer->GetNameString() ) );

															cTable.CreateRow( *cNew.get() );

															// 属性をコピー
															pSource->CopyTo( *cNew.get(), true, false, false );

															// 形状をセット
															((IPointCollectionPtr)(IGeometry*)*cNew->GetShape())->SetWKSPoints( cNewGeom.GetPointCount(), point_wks );

															// コンテナに追加
															pContainer->push_back( cNew );
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
							break;
						default:
							_ASSERTE(false); break;
					}
				} else {
					cErrObjects.push_back( CErrorObject( (*pSource) ) );
					_ASSERTE(false);
				}
			} else {
				cErrObjects.push_back( CErrorObject( (*pSource) ) );
				_ASSERTE(false);
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CFeatures::Merge( CSPRow pSource, CSPRow pDest, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr; // 返り値

	LOGASSERTEERR_IF( pSource && pDest, sindyErr_ArgIsNull )
	{
		sindyTableType::ECode emSourceTableType = pSource->GetTableType();
		sindyTableType::ECode emDestTableType = pDest->GetTableType();

		if ( *pSource->GetNameString() == *pDest->GetNameString() && emSourceTableType == emDestTableType ) {	// 同じフィーチャクラス？
			CString strTableName( CString_LastPeriodRight( pSource->GetTableName() ) );

			if( AheIsRelateWithNetwork( strTableName ) )
			{
				for ( const_iterator it = begin(); it != end(); ++it ) {
					if ( dynamic_cast<CRoadNetwork*>( *it ) ) {	// 道路ネットワーク
						CRoadNetwork* pRoadNetwork = static_cast<CRoadNetwork*>( (*it) );

						if( 0 == lstrcmpi( pRoadNetwork->GetTableName(), AheGetNetworkLinkFeatureClassName( strTableName ) ) )
							pRoadNetwork->Merge( pSource, pDest, cErrObjects );
					}
				}
			}
			// 内包関連系
			else if( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() || g_cRuleRelationshipContain.find_value( strTableName ) != g_cRuleRelationshipContain.end() )
			{
				for ( const_iterator it = begin(); it != end(); ++it ) {
					if ( dynamic_cast<CModelContainContainer*>( *it ) ) {	// 道路ネットワーク
						CModelContainContainer* pModelContain = static_cast<CModelContainContainer*>( (*it) );

						CString strBaseName;

						if ( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() )
							strBaseName = g_cRuleRelationshipContain.find( strTableName )->first;
						else
							strBaseName = g_cRuleRelationshipContain.find_value( strTableName )->first;

						if( 0 == lstrcmpi( pModelContain->GetTableName(), strBaseName ) )
							pModelContain->Merge( pSource, pDest, cErrObjects );
					}
				}
			}
			else
			{
				CFeature* pFeature = dynamic_cast<CFeature*>( pSource.get() );

				if ( pFeature ) {
					if ( pFeature->GetShape() ) {
						esriGeometryType emType = pFeature->GetShape()->GetGeometryType();

						switch ( emType ) {
							case esriGeometryPolyline:
							case esriGeometryPolygon:
								{
									if( sindyErr_NoErr == pFeature->Merge( CGeometry(pDest->GetShapeCopy()), cErrObjects ) )
									{
										// マージ先を削除
										pDest->Delete();
									} else
										cErrObjects.push_back( CErrorObject( (*pSource) ) );
								}
								break;
							default:
								_ASSERTE(false); break;
						}
					} else {
						cErrObjects.push_back( CErrorObject( (*pSource) ) );
						_ASSERTE(false);
					}
				} else {
					cErrObjects.push_back( CErrorObject( (*pSource) ) );
					_ASSERTE(false);
				}
			}
		} else {
			cErrObjects.push_back( CErrorObject( (*pSource) ) );
			cErrObjects.push_back( CErrorObject( (*pDest) ) );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CFeatures::Erase( CSPRow pRow, const CGeometry& cGeom, CErrorObjects& cErrObjects, bool bEraseOnly/*=false*/ )
{
	sindyErrCode emErr = sindyErr_NoErr; // 返り値

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		CString strTableName( CString_LastPeriodRight( pRow->GetTableName() ) );

		if( AheIsRelateWithNetwork( strTableName ) )
		{
			for( const_iterator it = begin(); it != end(); ++it ) {
				if ( dynamic_cast<CRoadNetwork*>( *it ) ) {	// 道路ネットワーク
					CRoadNetwork* pRoadNetwork = static_cast<CRoadNetwork*>( (*it) );

					if( 0 == lstrcmpi( pRoadNetwork->GetTableName(), AheGetNetworkLinkFeatureClassName( strTableName ) ) )
						pRoadNetwork->Erase( pRow, cGeom, cErrObjects, bEraseOnly );
				}
			}
		}
		// 内包関連系
		else if( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() || g_cRuleRelationshipContain.find_value( strTableName ) != g_cRuleRelationshipContain.end() )
		{
			for( const_iterator it = begin(); it != end(); ++it ) {
				if ( dynamic_cast<CModelContainContainer*>( *it ) ) {	// 道路ネットワーク
					CModelContainContainer* pModelContain = static_cast<CModelContainContainer*>( (*it) );

					CString strBaseName;

					if ( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() )
						strBaseName = g_cRuleRelationshipContain.find( strTableName )->first;
					else
						strBaseName = g_cRuleRelationshipContain.find_value( strTableName )->first;

					if( 0 == lstrcmpi( pModelContain->GetTableName(), strBaseName ) )
						pModelContain->Erase( pRow, cGeom, cErrObjects );
				}
			}
		}
		else
		{
			CFeature* pFeature = dynamic_cast<CFeature*>( pRow.get() );

			if ( pFeature ) {
				if ( pFeature->GetShape() ) {
					esriGeometryType emType = pFeature->GetShape()->GetGeometryType();

					switch ( emType ) {
						case esriGeometryPoint:
						case esriGeometryPolygon:
						case esriGeometryPolyline:
							pFeature->Erase( cGeom, cErrObjects );
							break;
						default:
							_ASSERTE(false);
							break;
					}
				} else {
					cErrObjects.push_back( CErrorObject( (*pRow) ) );
					_ASSERTE(false);
				}
			} else {
				cErrObjects.push_back( CErrorObject( (*pRow) ) );
				_ASSERTE(false);
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CFeatures::Delete( CSPRow pRow, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr; // 返り値

	LOGASSERTEERR_IF( pRow, sindyErr_ArgIsNull )
	{
		CString strTableName( CString_LastPeriodRight( pRow->GetTableName() ) );

		if( AheIsRelateWithNetwork( strTableName ) )
		{
			for( const_iterator it = begin(); it != end(); ++it ) {
				if ( dynamic_cast<CRoadNetwork*>( *it ) ) {	// 道路ネットワーク
					CRoadNetwork* pRoadNetwork = static_cast<CRoadNetwork*>( (*it) );

					if( 0 == lstrcmpi( pRoadNetwork->GetTableName(), AheGetNetworkLinkFeatureClassName( strTableName ) ) )
						pRoadNetwork->Delete( pRow, cErrObjects );
				}
			}
		}
		// 内包関連系
		else if( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() || g_cRuleRelationshipContain.find_value( strTableName ) != g_cRuleRelationshipContain.end() )
		{
			for( const_iterator it = begin(); it != end(); ++it ) {
				if ( dynamic_cast<CModelContainContainer*>( *it ) ) {	// 道路ネットワーク
					CModelContainContainer* pModelContain = static_cast<CModelContainContainer*>( (*it) );

					CString strBaseName;

					if ( g_cRuleRelationshipContain.find( strTableName ) != g_cRuleRelationshipContain.end() )
						strBaseName = g_cRuleRelationshipContain.find( strTableName )->first;
					else
						strBaseName = g_cRuleRelationshipContain.find_value( strTableName )->first;

					if( 0 == lstrcmpi( pModelContain->GetTableName(), strBaseName ) )
						pModelContain->Delete( pRow, cErrObjects );
				}
			}
		}
		else
		{
			CFeature* pFeature = dynamic_cast<CFeature*>( pRow.get() );

			if ( pFeature ) {
				CErrorObject cErrObj( (*pFeature) ); // エラー情報格納用

				pFeature->Delete();
			} else {
				cErrObjects.push_back( CErrorObject( (*pRow) ) );
				_ASSERTE(false);
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// クローンを作成する
sindyErrCode CFeatures::Clone( IWorkspace* ipWorkspace/* = NULL*/ )
{
	sindyErrCode emErr = sindyErr_NoErr; // 戻り値

	// ipWorkspaceが指定されている場合は同名のテーブルをオープンする
	// SiNDY-bのルールとして、接続ユーザと同じオーナーのテーブルしか編集してはならない
	// という暗黙のルールがあるので、接続ユーザをオーナー名として検索する
	CWorkspace cWork( ipWorkspace );
	CString strOwnerName;
	if( cWork )
		strOwnerName = cWork.GetNameString()->GetUserName();

	for( iterator it = begin(); it != end(); ++it )
	{
		if( sindyErr_NoErr != ( emErr = (*it)->CreateCache() ) ) // キャッシュを作成する
			break;

		// 
		CTable cTable;
		if( cWork )
		{
			cTable = cWork.OpenTable( ( ! strOwnerName.IsEmpty() ) ? strOwnerName + _T(".") + CString((*it)->GetTableName()) : (*it)->GetTableName() );
			if( ! cTable )
			{
				emErr = sindyErr_AOGetFeatureClassFailed; // テーブルがない場合は継続させない
				break;
			}
			else {
				// テーブルを挿げ替える
				static_cast<CTable&>(**it).SetObject( (ITable*)cTable );
				cTable = static_cast<CTable&>(**it);
			}
		}
		for( CContainer::iterator it2 = (*it)->begin(); it2 != (*it)->end(); ++it2 )
		{
			for( CContainer::rows_iterator itRow = it2->second.begin(); itRow != it2->second.end(); ++itRow )
			{
				if( sindyErr_NoErr != ( emErr = (*itRow)->ReplaceNewRow( &cTable, true ) ) ) // 階層的に挿げ替え
					break;
			}
			if( sindyErr_NoErr != emErr )
				break;
		}
		if( sindyErr_NoErr != emErr )
			break;
	}
	return emErr;
}

// 編集内容を保存する
sindyErrCode CFeatures::Store( CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	// ModifyCheckで編集されたオブジェクトを対象にチェック
	LOGASSERTE_IF( sindyErr_NoErr == ( emErr = CheckModify( cErrObjects ) ), sindyErr_AlreadyModified ) // 本当のエラーコードはemErrに入るからここのエラーコードは適当
	{
		// キャッシュがあれば、キャッシュへの変更結果を反映しながらStore
		// 履歴はここで自動的に付与
		for( const_iterator it = begin(); it != end(); ++it )
		{
			if( sindyErr_NoErr != ( emErr = (*it)->Store( cErrObjects ) ) )
				break;	// エラーが起きたらbreak
		}
	}
	return emErr;
}

// 整合性をチェックする
sindyErrCode CFeatures::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const
{
	for( const_iterator it = begin(); it != end(); ++it ) {
		if ( (*it)->GetShapeType() == esriGeometryPolygon ) {
			// 重複チェック用に編集のあったオブジェクトの形状で追加検索してからチェック。
			IGeometryPtr ipEditArea = (*it)->GetEditArea();

			if ( ipEditArea )
				(*it)->Select( ipEditArea, false, true, NULL, false );
		}

		(*it)->CheckLogic( cRule, cErrObjects );

		// [Bug 5846]誘導リンクの端点は歩行者ノード上にあるかチェック
		if ( (*it)->GetTableType() == sindyTableType::guide_link )	// 誘導リンク？
			CheckLogic_GuideLink( (*it), cErrObjects );
	}


	return cErrObjects.GetDefaultErrCode();
}

sindyErrCode CFeatures::CheckModify( CErrorObjects& cErrObjects ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
		(*it)->CheckModify( cErrObjects );
	
	return cErrObjects.GetDefaultErrCode();
}

void CFeatures::clear()
{
	for( iterator it = begin(); it != end(); ++it )
		delete *it;
	m_listFeatureContainer.clear();
}

bool CFeatures::empty() const
{
	bool bRet = true;
	for( const_iterator it = begin(); it != end(); ++it )
	{
		if( ! (*it)->empty() )
		{
			bRet = false;
			break;
		}
	}

	return bRet;
}

// 現在格納されているフィーチャの存在矩形を返す
IEnvelopePtr CFeatures::GetEnvelope( bool bOriginal/* = true*/, bool bChanged/* = true*/, bool bFeatureChangedOriginal/* = true*/ ) const
{
	IEnvelopePtr ipRet;
	for( const_iterator it = begin(); it != end(); ++it )
	{
		IEnvelopePtr ipEnv = (*it)->GetEnvelope( bOriginal, bChanged, bFeatureChangedOriginal );
		if( ipRet != NULL )
		{
			if( ipEnv != NULL )
			{
				LOGASSERTE_IF( SUCCEEDED( ipRet->Union( ipEnv ) ), sindyErr_GeometryFunctionFailed )
				{
				}
			}
		}
		else
			ipRet = ipEnv;
	}
	return ipRet;
}

// 現在格納されているフィーチャの形状を返す
IGeometryPtr CFeatures::GetShape( bool bLatest/* = true*/, bool bOriginal/* = false*/, bool bFeatureChangedOriginal/* = false*/, long lParam/* = -1*/ ) const
{
	IGeometryPtr ipRet;

	IGeometryCollectionPtr ipPointsCol, ipPolylinesCol, ipPolygonsCol;
	for( const_iterator it = begin(); it != end(); ++it )
	{
		IGeometryPtr ipGeom = (*it)->GetShape( bLatest, bOriginal, bFeatureChangedOriginal, lParam );
		if( ipGeom != NULL )
		{
			esriGeometryType emType = esriGeometryAny;
			ipGeom->get_GeometryType( &emType );
			switch( emType )
			{
				case esriGeometryBag:
				{
					IEnumGeometryPtr ipEnumGeom( ipGeom );
					ipEnumGeom->Reset();
					IGeometryPtr ipGeomPart;
					while( S_OK == ipEnumGeom->Next( &ipGeomPart ) )
					{
						esriGeometryType emTypePart = esriGeometryAny;
						ipGeomPart->get_GeometryType( &emTypePart );
						switch( emTypePart )
						{
							case esriGeometryMultipoint:
							case esriGeometryPoint:
								if( ipPointsCol == NULL )
									ipPointsCol.CreateInstance( CLSID_GeometryBag );
								ipPointsCol->AddGeometry( ipGeomPart );
								break;
							case esriGeometryPolyline:
								if( ipPolylinesCol == NULL )
									ipPolylinesCol.CreateInstance( CLSID_GeometryBag );
								ipPolylinesCol->AddGeometry( ipGeomPart );
								break;
							case esriGeometryPolygon:
								if( ipPolygonsCol == NULL )
									ipPolygonsCol.CreateInstance( CLSID_GeometryBag );
								ipPolygonsCol->AddGeometry( ipGeomPart );
								break;
							default:
								_ASSERTE(false);
								break;
						}
					}
					break;
				}
				case esriGeometryMultipoint:
				case esriGeometryPoint:
					if( ipPointsCol == NULL )
						ipPointsCol.CreateInstance( CLSID_GeometryBag );
					ipPointsCol->AddGeometry( ipGeom );
					break;
				case esriGeometryPolyline:
					if( ipPolylinesCol == NULL )
						ipPolylinesCol.CreateInstance( CLSID_GeometryBag );
					ipPolylinesCol->AddGeometry( ipGeom );
					break;
				case esriGeometryPolygon:
					if( ipPolygonsCol == NULL )
						ipPolygonsCol.CreateInstance( CLSID_GeometryBag );
					ipPolygonsCol->AddGeometry( ipGeom );
					break;
				default:
					_ASSERTE(false);
					break;
			}
		}
	}
	// それぞれをUnionする
	ITopologicalOperatorPtr ipPointsTopo, ipPolylinesTopo, ipPolygonsTopo;
	if( ipPointsCol != NULL )
	{
		ipPointsTopo.CreateInstance( CLSID_Multipoint );
		ipPointsTopo->ConstructUnion( (IEnumGeometryPtr)ipPointsCol );
	}
	if( ipPolylinesCol != NULL )
	{
		ipPolylinesTopo.CreateInstance( CLSID_Polyline );
		ipPolylinesTopo->ConstructUnion( (IEnumGeometryPtr)ipPolylinesCol );
	}
	if( ipPolygonsCol != NULL )
	{
		ipPolygonsTopo.CreateInstance( CLSID_Polygon );
		ipPolygonsTopo->ConstructUnion( (IEnumGeometryPtr)ipPolygonsCol );
	}
	// どれか一つしかなければそれを返り値とする
	bool bPoints    = ( ipPointsTopo != NULL && ipPolylinesTopo == NULL && ipPolygonsTopo == NULL );
	bool bPolylines = ( ipPointsTopo == NULL && ipPolylinesTopo != NULL && ipPolygonsTopo == NULL );
	bool bPolygons  = ( ipPointsTopo == NULL && ipPolylinesTopo == NULL && ipPolygonsTopo != NULL );
	if( bPoints || bPolylines || bPolygons )
	{
		if( bPoints )
			ipRet = ipPointsTopo;
		else if( bPolylines )
			ipRet = ipPolylinesTopo;
		else if( bPolygons )
			ipRet = ipPolygonsTopo;
	}
	else if( ipPointsTopo != NULL || ipPolylinesTopo != NULL || ipPolygonsTopo != NULL )
	{
		IGeometryCollectionPtr ipGeomCol( CLSID_GeometryBag );
		if( ipPointsTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPointsTopo );
		if( ipPolylinesTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPolylinesTopo );
		if( ipPolygonsTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPolygonsTopo );
		ipRet = ipGeomCol;
	}
	return ipRet;
}

#ifdef _DEBUG
void CFeatures::Trace( bool bAttribute/* = true*/, bool bGeometry/* = true*/ ) const
{
	for( const_iterator it = begin(); it != end(); ++it ) (*it)->Trace( bAttribute, bGeometry );
}
#else
void CFeatures::Trace( bool/* bAttribute = true*/, bool/* bGeometry = true*/ ) const
{
}
#endif // ifdef _DEBUG

// 整合性をチェックする
sindyErrCode CFeatures::CheckLogic_GuideLink( CFeatureContainer* pContainer, CErrorObjects& cErrObjects ) const
{
	if ( pContainer ) {
		// [Bug 5846] [歩行者NW]誘導リンクの端点選択で短縮しないようにして欲しい
		// 誘導リンクの端点は歩行者ノード上になければいけないみたいなので、ここで整合性チェック
		CRoadNetwork* pRoadNetwork = NULL;

		// ネットワークモデルがあるかチェック
		for( const_iterator it = begin(); it != end(); ++it ) {
			if( AheIsRelateWithNetwork( (*it)->GetTableName() ) )
			{
				if ( AheTableName2TableType( AheGetNetworkNodeFeatureClassName( (*it)->GetTableName() ) ) == sindyTableType::walk_node ) {
					pRoadNetwork = static_cast<CRoadNetwork*>( (*it) );
					break;
				}
			}
		}

		for ( CContainer::const_rows_iterator itGuide = pContainer->begin(pContainer->GetTableType()), itGuideEnd = pContainer->end(pContainer->GetTableType()); 
			itGuide != itGuideEnd; ++itGuide ) {

			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			// 編集したもの and 削除されていないもののみチェック
			if ( (*itGuide)->Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kDeleted ) {
				// 編集前の形状と編集後の形状で、始終点に変更があるかチェック
				CGeometry cLastestFrom = (*itGuide)->GetShape()->GetPoint( 0 );
				CGeometry cLastestTo = (*itGuide)->GetShape()->GetPoint( (*itGuide)->GetShape()->GetPointCount() - 1 );

				// [Bug 6142] Guide_Linkを分断するようにセグメント削除すると落ちる
				// 編集された始終点のみノード上にあるかどうかをチェックするために、
				// 形状の差分を取って始終点が編集されたか確認をしていたので、
				// Splitした時に新しく作られたオブジェクトのチェックで、
				// 元の形状がないために落ちていた。
				// 編集された始終点のみではなく、始終点がノード上にあるかどうかをチェックするように修正。
				// Bug 6142 CGeometry cOrginalGeom = (*itGuide)->GetShapeCopy( false, true );
				// Bug 6142 CGeometry cOrginalFrom = cOrginalGeom.GetPoint( 0 );
				// Bug 6142 CGeometry cOrginalTo = cOrginalGeom.GetPoint( cOrginalGeom.GetPointCount() - 1 );

				// Bug 6142 bool bEditFrom = false, bEditTo = false;

				// Bug 6142 if ( ! cLastestFrom.Equals( cOrginalFrom ) )	// 始点が違う？
				// Bug 6142 	bEditFrom = true;

				// Bug 6142 if ( ! cLastestTo.Equals( cOrginalTo ) )	// 終点が違う？
				// Bug 6142 	bEditTo = true;

				// Bug 6142 if ( bEditFrom || bEditTo ) {	// 始終点に変更がある？
					bool bHasFrom = false, bHasTo = false;

					if ( pRoadNetwork ) {	// 歩行者NWがなければ警告扱い
						// 変更があった始終点に一致する歩行者ノードがあるかチェック
						sindyTableType::ECode emNetworkNodeTableType = AheTableName2TableType( AheGetNetworkNodeFeatureClassName( pRoadNetwork->GetTableName() ) ); // ノードテーブルタイプ

						for ( CContainer::const_rows_iterator itNode = pRoadNetwork->begin(emNetworkNodeTableType), itNodeEnd = pRoadNetwork->end(emNetworkNodeTableType); 
							itNode != itNodeEnd; ++itNode ) {

							emUpdateType = schema::ipc_table::update_type::kDefault;

							// 編集したもの or 削除されていないもののみチェック
							if ( (*itNode)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
								// Bug 6142 if ( bEditFrom ) {
									if ( cLastestFrom.Equals( (IGeometry*)*(*itNode)->GetShape() ) )
										bHasFrom = true;
								// Bug 6142 }

								// Bug 6142 if ( bEditTo ) {
									if ( cLastestTo.Equals( (IGeometry*)*(*itNode)->GetShape() ) )
										bHasTo = true;
								// Bug 6142 }
							}
						}
					}

					if ( (/* Bug 6142 bEditFrom &&*/ ! bHasFrom) || (/* Bug 6142 bEditTo &&*/ ! bHasTo) )	// 始終点に一致する歩行者ノードがない
						cErrObjects.push_back( CErrorObject( *itGuide->get(), CErrorInfo( sindyErr_RuleRelationshipContainGeometryNotOnWalkNode, sindyErrLevel_WARN ) ) );
				// Bug 6142 }
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

} // sindy
