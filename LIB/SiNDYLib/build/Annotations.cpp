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
 * @file Annotations.cpp
 * @brief <b>CAnnotationsクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @vertion $Id$
 */
#include "stdafx.h"
#include "Annotations.h"
#include "GlobalFunctions.h"
#include "util.h"
#include "TableTypeUtil.h"
#include "AheWKSFunctions.h"
#include "AnnoGlobals.h"
#include "GeometryRule.h"
#include <algorithm>
#include "ErrorObject.h"
#include "GlobalRules.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR ANNO_START_SEARCH       = _T("CAnnotations : start a search%s...");
	LPCTSTR EFFECTIVE_CACHE         = _T("(Cache is effective)");
	LPCTSTR NON_EFFECTIVE_CACHE     = _T("(Cache is not effective)");
	LPCTSTR SEARCH_TRUE_POINT	    = _T("True position[search %d Features : %.5lfsec]...");
	LPCTSTR SEARCH_DISPPOINT_STATUS = _T("Display position[Total time:%.5lfsec, search:%.5lfsec, Fetch loop:%.5lfsec, The first half of Fetch loop:%.5lfsec, SetDispLine:%.5lf] Search Table[%ds:%.5lfsec]...");
	LPCTSTR ADD_SELECTED_DISPPOINT  = _T("Adds a selected display position [%s:%d]\n");
	LPCTSTR FINISH_SEARCH_ANNO      = _T("Finished searching[Total:%.5lfsec cache:%.5lfsec]\n");
	LPCTSTR START_CACHE_CREATION    = _T("Start creating parameter cache...");
	LPCTSTR FINISH_CREATE_SEC       = _T("Finished creating:%.5lfsec\n");
	LPCTSTR REMOVE_OUT_OF_AREA      = _T("[%s:%d] It is removed because it is out of a search area\n");
	LPCTSTR START_SPATIAL_CREATION  = _T("Start creating spatial index...");
#else
	LPCTSTR ANNO_START_SEARCH       = _T("CAnnotations : 検索開始%s…");
	LPCTSTR EFFECTIVE_CACHE         = _T("（キャッシュあり）");
	LPCTSTR NON_EFFECTIVE_CACHE     = _T("（キャッシュなし）");
	LPCTSTR SEARCH_TRUE_POINT	    = _T("真位置[%d個検索：%.5lf秒]…");
	LPCTSTR SEARCH_DISPPOINT_STATUS = _T("表示位置[総時間：%.5lf秒 検索：%.5lf秒 フェッチループ：%.5lf秒 フェッチループ前半：%.5lf秒 SetDispLine：%.5lf] テーブル検索[%d回：%.5lf秒]…");
	LPCTSTR ADD_SELECTED_DISPPOINT  = _T("表示位置追加取得 [%s:%d]\n");
	LPCTSTR FINISH_SEARCH_ANNO      = _T("検索終了[Total：%.5lf秒 キャッシュ：%.5lf秒]\n");
	LPCTSTR START_CACHE_CREATION    = _T("パラメータキャッシュ作成開始…");
	LPCTSTR FINISH_CREATE_SEC       = _T("作成完了：%.5lf秒\n");
	LPCTSTR REMOVE_OUT_OF_AREA      = _T("[%s:%d] 検索エリア外のため除外します\n");
	LPCTSTR START_SPATIAL_CREATION  = _T("空間インデックス作成開始…");
#endif // SINDY_FOR_ENGLISH
} // namespace

namespace sindy {
using namespace errorcode;

/**
 * @brief 検索しすぎたオブジェクトをそぎ落とす際のスケール限定テーブルタイプ
 *
 * CAnnotations::_SelectByShape()で使用
 * フィーチャコピーツールでの使用を前提としているので、この変数を
 * 使用する際は参考にすること。
 */
sindyTableType::ECode g_emViewTableType = sindyTableType::unknown;

// メッシュコードで検索する</b>\n
sindyErrCode CAnnotations::SelectByMesh( long lMeshCode, mesh_type::ECode emMeshType, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/ )
{
	// mesh_type -> group
	sindyAnnotationGroup emGroup = (sindyAnnotationGroup)-1;
	switch( emMeshType )
	{
		case mesh_type::kCity:   emGroup = sindyAnnoGroup_city; break;
		case mesh_type::kBase:   emGroup = sindyAnnoGroup_base; break;
		case mesh_type::kMiddle: emGroup = sindyAnnoGroup_middle; break;
		case mesh_type::kTop:    emGroup = sindyAnnoGroup_top; break;
		default: break;
	}
	// メッシュコード -> エリア変換
	IEnvelopePtr ipEnv( AheMeshCode2Envelope( lMeshCode, emGroup, GetSpatialReference() ) );
	if( ! ipEnv )
		return sindyErr_ArgLimitOver;
	// 検索
	return Select( ipEnv, bRecycling, bCreateCache, pErrs );
}

// 形状で検索する
sindyErrCode CAnnotations::Select( IGeometry* ipGeom, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit /*= true*/ )
{
	return _SelectByShape( ipGeom, bRecycling, bCreateCache, pErrs, bInit );
}
sindyErrCode CAnnotations::_SelectByShape( IGeometry* ipGeom, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit /*= true*/, IFeatureCursor* ipDispCursor/* = NULL*/, sindyTableType::ECode* pDispTableType/* = NULL*/, bool bCreateIndex/* = true*/ )
{
	double dStart = GetTickCount();
	TRACEMESSAGE( ANNO_START_SEARCH, ( bRecycling ) ? EFFECTIVE_CACHE : NON_EFFECTIVE_CACHE );

	// 初期化
	if( bInit )
		clear();

	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	CGeometry cSearchGeom( ipGeom );
	LOGASSERTEERR_IF( p, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( ! ( bRecycling && ( ! bCreateCache ) ), sindyErr_ArgIsConsistency )
		{
			// 共有オブジェクト取得
			CSPTableNameString spAnnoNameString( GetNameString() );
			CSPFieldMap spAnnoFieldMap( GetFieldMap() );

			// ジオメトリがNULLでも検索できるが、ここではあえて
			// エラーとすることで予期しない全件検索を回避する
			LOGASSERTEERR_IF( cSearchGeom != NULL, sindyErr_ArgIsNull )
			{
				{
					IQueryFilterPtr ipFilter = AheInitSpatialFilter( NULL, ipGeom );
					IFeatureCursorPtr ipCursor;
					
					double dAnnoFetchStart = GetTickCount();	// 真位置取得開始時間（トレース用）

					// 真位置を検索
					std::map<long,long> mapAnnoID;	// OIDとリストのインデックスの対応表
					//std::list<long> listExtendAnnoIDs;	// 選択矩形内にない真位置のID保持用
					std::map<sindyTableType::ECode,std::list<long> > maplistID; // 足りないオブジェクトを判別するのに使用

					// 記号の大きさを考慮して、
					// 与えられた検索形状を最大記号/2分バッファリングして検索する
					IGeometryPtr ipSearchGeom( cSearchGeom.Buffer( (double)8.0*AheCalcDispLineType21PXLen( *scale_rbegin(), true ) ) );
//					if( g_bBufferedSearch )
//						ipSearchGeom = cSearchGeom.Buffer( (double)8.0*AheCalcDispLineType21PXLen( *scale_rbegin(), true ) );
//					else
//						ipSearchGeom = ipGeom;

					LOGASSERTEERR_IF( ipSearchGeom, sindyErr_GeometryFunctionFailed )
					{
						((ISpatialFilterPtr)ipFilter)->putref_Geometry( ipSearchGeom );
						ipCursor = Search( ipFilter, bRecycling );
					}

					LOGASSERTEERR_IF( ipCursor, sindyErr_SearchFailed )
					{
						long lCount = ( bInit ) ? 0 : size( m_emTableType );
						IFeature* ipFeature = NULL;
//						IEnvelopePtr ipAnnoRect( CLSID_Envelope ); // 真位置矩形比較で使用するのであらかじめ作成しておく
//						ipAnnoRect->putref_SpatialReference( GetSpatialReference() ); // 空間参照セット
						while( S_OK == ipCursor->NextFeature( &ipFeature ) )
						{
							if( ! ipFeature )
								break;

							// OIDとリストのインデックス対応表作成
							long lOID = 0;
							ipFeature->get_OID( &lOID );
							mapAnnoID[lOID] = lCount;
							// IDは後で足りないものを引くのに使うのでリストにも格納
							maplistID[GetTableType()].push_back( lOID );

							// バッファリングしないモードだと、真位置のみ
							// 選択されるような状況の場合に表示位置を選択
							// することができない！！とりあえず暫定的に
							// listExtendAnnoIDsに突っ込んであとで検索してもらう
//							if( ! g_bBufferedSearch )
//								listExtendAnnoIDs.push_back( lOID );

							CSPRelAnnotationParameter spAnno( new CRelAnnotationParameter( ipFeature, m_emTableType, false, spAnnoFieldMap, spAnnoNameString ) );

/*							// コンテナに追加する前に、検索エリアに本当に含まれるかどうか
							// を事前に検査し、いらないものははじく
							// これは、S1～S4全てで行う必要がある（bug 3041）
							bool bIsContain = false;
							IGeometryPtr ipAnnoPoint( (IPoint*)(spAnno->GetShape()->GetPoint(0)) );
							for( scale_iterator itScale = scale_begin(), itScaleEnd = scale_end(); itScale != itScaleEnd; ++itScale )
							{
								// 真位置がバッファリング前の検索エリアにあるかどうか
								if( ! cSearchGeom.Disjoint( ipAnnoPoint ) )
								{
									bIsContain = true;
									break;
								}
								// 記号表示真位置がバッファリング前の検索エリアにない場合、
								// 記号表示じゃないのならいらない
								if( ! spAnno->CanShowMark( *itScale ) )
									continue;
								// 真位置矩形がバッファリング前の検索エリアとかぶるかどうか
								WKSEnvelope env = {0};
								spAnno->CalcSymbolBounds( spAnno->CalcMarkLength( *itScale, true ), spAnno->CalcMarkLength( *itScale, false ), env );
								ipAnnoRect->PutWKSCoords( &env );
								ipAnnoRect->SnapToSpatialReference();
								if( ! cSearchGeom.Disjoint( (IGeometryPtr)ipAnnoRect ) )
									bIsContain = true;
							}

							// 検索エリア内に含まれないものは捨てる
							if( ! bIsContain )
							{
								TRACEMESSAGE(_T("真位置 OID[%d] 検索エリア外のため排除\n"), lOID );
								continue;
							}*/

							push_back( spAnno );
							
							ipFeature->Release();
							ipFeature = NULL;

							if( bRecycling )
								spAnno->CreateCache( true, spAnnoFieldMap, spAnnoNameString );
							else if( bCreateCache )
								spAnno->CreateCache( false, spAnnoFieldMap, spAnnoNameString );

							++lCount;
						}
						ipCursor = NULL;
						TRACEMESSAGE(SEARCH_TRUE_POINT, lCount, ((double)GetTickCount()-dAnnoFetchStart)/(double)1000.0);
					}

					// 表示位置を検索
					double dSearchDispTime = 0.0; // 表示位置検索時間（トレース用）
					double dFetchDispTime = 0.0;  // 表示位置フェッチループ時間（トレース用）
					double dFetchLoop1Time = 0.0; // 表示位置フェッチループ前半時間（トレース用）

					LPCTSTR lpcszOwnerName = GetOwnerName();	// 真位置のオーナー名

					for( scale_iterator itScale = scale_begin(), itScaleEnd = scale_end(); itScale != itScaleEnd; ++itScale )
					{
						// カーソルが指定されており、表示位置テーブルが限定されている場合はそれ以外を飛ばす
						if( ipDispCursor && pDispTableType && *itScale != *pDispTableType )
							continue;

						double dStartDispFetch = GetTickCount();	// 表示位置取得開始時間（トレース用）
						double dSearchTable = 0.0;					// 関連真位置検索総時間（トレース用）
						long lSearchTableCount =0;					// 関連真位置検索総回数（トレース用）

						// 表示位置レイヤを取得（オーナー名がある場合はオーナー名をつける）
						CString strDispClassName;
						if( 0 < lstrlen(lpcszOwnerName) )
							strDispClassName.Format(_T("%s.%s"), lpcszOwnerName, AheTableType2TableName( *itScale ) );
						else
							strDispClassName = AheTableType2TableName( *itScale );					

						CFeatureClass cDispClass( *itScale, OpenTable( strDispClassName ) );
						LOGASSERTEERR_IF( cDispClass != NULL, sindyErr_TableNotFound )
						{
							double dSearchDispStart = (double)GetTickCount();

							// WARNING 検索エリアを最大スケールフォント4つ分拡大する[bug 3041]暫定対策
							// だが、これでも旗揚げやコピー注記では拾えないものが出てくるはずなので、
							// ちゃんとやるのであれば真位置のOBJECTIDで検索する必要がある
							if( ! ipDispCursor )
							{
								IGeometryPtr ipSearchGeom( cSearchGeom.Buffer( (double)schema::disp_line::font_size::k24 * AheCalcDispLineType21PTLen( *itScale, true ) ) );
//								if( g_bBufferedSearch )
//									ipSearchGeom = cSearchGeom.Buffer( (double)schema::disp_line::font_size::k24 * AheCalcDispLineType21PTLen( *itScale, true ) * (double)4.0 );
//								else
//									ipSearchGeom = ipGeom;
								LOGASSERTEERR_IF( NULL != ipSearchGeom, sindyErr_GeometryFunctionFailed )
								{
									// 検索
									((ISpatialFilterPtr)ipFilter)->putref_Geometry( ipSearchGeom );
									ipCursor = cDispClass.Search( ipFilter, bRecycling );
									dSearchDispTime += (double)GetTickCount()-dSearchDispStart; // 表示位置総検索時間を計算
								}
							}
							else
								ipCursor = ipDispCursor;

							// フェッチ
							double dFetchDispStart = GetTickCount();	// 表示位置フェッチループ開始時間（トレース用）
							LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
							{
								// 共有オブジェクト取得
								CSPTableNameString spDispNameString( cDispClass.GetNameString() );
								CSPFieldMap spDispFields( cDispClass.GetFieldMap() );

								// 少しでも高速化するために、真位置IDのインデックス番号を取得しておく
								long lAnnoIDIndex = spDispFields->FindField( schema::disp_line::kAnnoID );	// インデックス番号
								VARIANT vaAnnoID;												// 関連ID保持用
								VariantInit( &vaAnnoID );

								SASSERT( lAnnoIDIndex >= 0 );	// インデックス番号は0以上

								IFeature* ipDisp = NULL;
								while( S_OK == ipCursor->NextFeature( &ipDisp ) )
								{
									double dFetchLoop1Start = GetTickCount();
									if( ipDisp == NULL ) break;

//									// 要らない表示位置を落とす

									ipDisp->get_Value( lAnnoIDIndex, &vaAnnoID );
									SASSERT( vaAnnoID.vt == VT_I4 );	// 値はあるはず

									long lOID = -1;
									ipDisp->get_OID( &lOID );
									// 後で足りないものを引くのに使うので記録しておく
									maplistID[*itScale].push_back( lOID );
#ifdef _DEBUG
									ATLTRACE(_T("表示位置[%s] OBJECTID[%ld]\n"), spDispNameString->GetTableName(), lOID );
#endif // ifdef _DEBUG

									CRelAnnotationParameter* pAnnoParam = NULL; // アクセスしやすくするため
									std::map<long,long>::const_iterator itID = mapAnnoID.find( vaAnnoID.lVal );
									if( itID != mapAnnoID.end() )
									{
										
										pAnnoParam = static_cast<CRelAnnotationParameter*>(&*find( m_emTableType )->second[itID->second]);// 見つけておく
										dFetchLoop1Time += GetTickCount()-dFetchLoop1Start;
										pAnnoParam->SetDispLine( *itScale, ipDisp, bRecycling, spDispFields, spDispNameString );
									}
									// 検索した真位置の中にない場合
									else {
										double dSearch = GetTickCount();

										// リストに追加
										IFeaturePtr ipFeature( GetFeature( vaAnnoID.lVal ) );
										if( ipFeature )
										{
											CSPRelAnnotationParameter spAnnoParam = CSPRelAnnotationParameter( new CRelAnnotationParameter( ipFeature, m_emTableType, false, spAnnoFieldMap, spAnnoNameString ) );
											pAnnoParam = static_cast<CRelAnnotationParameter*>(&*spAnnoParam);

											if( bRecycling )
												spAnnoParam->CreateCache( true, spAnnoFieldMap, spAnnoNameString );
											else if( bCreateCache )
												spAnnoParam->CreateCache( false, spAnnoFieldMap, spAnnoNameString );
											push_back( spAnnoParam );

											mapAnnoID[vaAnnoID.lVal] = size( m_emTableType ) - 1;
											// 後で足りないオブジェクトを引くのに使用するので確保
											maplistID[GetTableType()].push_back( vaAnnoID.lVal );

											spAnnoParam->SetDispLine( *itScale, ipDisp, bRecycling, spDispFields, spDispNameString ); // 関連付け
										}
										else {
											emErr = sindyErr_RelFeatureNotFound;	// 関連フィーチャが見つからない [bug 4000]
											if( pErrs )
											{
												CDispLine cDisp( ipDisp );
												pErrs->push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale() ) ) );
											}
											else
												ERRORLOG(emErr);											
										}

										// 後で関連表示ラインフィーチャをちゃんと検索するためにID保持
//										listExtendAnnoIDs.push_back( vaAnnoID.lVal );
										
										// トレースタイム用
										dSearchTable += GetTickCount()-dSearch;
										++lSearchTableCount;
									}

									ipDisp->Release();
									ipDisp = NULL;

									emErr = sindyErr_NoErr;	// 関連付けられなかったものは捨てるという仕様（ログには追加してある）なので、エラーコードはここでクリア
								}
							}
							dFetchDispTime += (double)GetTickCount()-dFetchDispStart; // フェッチループ時間を計算
						}
						TRACEMESSAGE(SEARCH_DISPPOINT_STATUS, ((double)GetTickCount()-dStartDispFetch)/(double)1000.0, dSearchDispTime/(double)1000.0, dFetchDispTime/(double)1000.0, dFetchLoop1Time/(double)1000.0, g_dTotalSetDispLineTime/(double)1000.0, lSearchTableCount, dSearchTable/(double)1000.0);
					}
					// この時点で、真位置から離れた場所に存在する表示位置が足りていないケースがありうる
					// コピー注記の場合など、どのぐらい離れているか検討もつかないので、現在選択できている真位置
					// のIDで表示位置IDを検索し、それから取得済みのものを引いたものを選択する
					//
					// 表示位置IDを取得
					std::list<CString> strIDs;
					IDs2Str( maplistID[GetTableType()], strIDs );
					std::map<sindyTableType::ECode, std::list<long> > maplistID_Exact; // 取得できていなければならないIDのリスト格納用
					for( std::list<CString>::const_iterator it = strIDs.begin(); it != strIDs.end(); ++it )
					{
						for( scale_iterator itScale = scale_begin(), itScaleEnd = scale_end(); itScale != itScaleEnd; ++itScale )
						{
							// 検索が指定されており、表示位置テーブルが限定されている場合はそれ以外を飛ばす
							if( ipDispCursor && pDispTableType && *itScale != *pDispTableType )
								continue;

							_ICursorPtr ipCursor;
							OpenSameOwnerTable( AheTableType2TableName( *itScale ) )->Search( AheInitQueryFilter( NULL, _T("OBJECTID"), _T("%s in (%s)"), schema::disp_line::kAnnoID, *it ), VARIANT_TRUE, &ipCursor );
							_IRowPtr ipRow;
							while( S_OK == ipCursor->NextRow( &ipRow ) )
							{
								long lOID = 0;
								ipRow->get_OID( &lOID );
								maplistID_Exact[*itScale].push_back( lOID );
							}
						}
					}
					// 検索できたIDリストから既に検索済みのリストを引いたものが足りないリスト
					std::map<sindyTableType::ECode,std::list<long> > maplistID_Query; // 拾ってこなければならないリスト
					for( std::map<sindyTableType::ECode,std::list<long> >::iterator it = maplistID_Exact.begin(); it != maplistID_Exact.end(); ++it )
					{
						std::list<long>& listID_Exact = it->second;
						std::list<long>& listID = maplistID[it->first];
						std::list<long>& listID_Query = maplistID_Query[it->first]; // 拾ってこなければならないID
						listID_Exact.sort();
						listID.sort();
						// listID_Exactに含まれて、listIDに含まれないリストをlistID_Queryに入れる
						std::set_difference( listID_Exact.begin(), listID_Exact.end(), listID.begin(), listID.end(), std::back_inserter(listID_Query) );
					}
					for( scale_iterator itScale = scale_begin(), itScaleEnd = scale_end(); itScale != itScaleEnd; ++itScale )
					{
						// 検索が指定されており、表示位置テーブルが限定されている場合はそれ以外を飛ばす
						if( ipDispCursor && pDispTableType && *itScale != *pDispTableType )
							continue;

						std::list<long>& listID = maplistID_Query[*itScale];
						if( ! listID.empty() )
						{
							CString strDispTableName( AheTableType2TableName(*itScale) );
							CFeatureClass cDispClass( *itScale, OpenSameOwnerTable( strDispTableName ) );
							
							// 共有オブジェクト取得
							CSPTableNameString spDispNameString( cDispClass.GetNameString() );
							CSPFieldMap spDispFields( cDispClass.GetFieldMap() );
							
							// 少しでも高速化するために、真位置IDのインデックス番号を取得しておく
							long lAnnoIDIndex = spDispFields->FindField( schema::disp_line::kAnnoID );	// インデックス番号
							VARIANT vaAnnoID;												// 関連ID保持用

							SASSERT( lAnnoIDIndex >= 0 );	// インデックス番号は0以上

							VariantInit( &vaAnnoID );

							_ICursorPtr ipCursor( cDispClass.GetRows( listID, bRecycling ) );
							if( ipCursor )
							{
								_IRowPtr ipDisp;
								while( S_OK == ipCursor->NextRow( &ipDisp ) )
								{
									ipDisp->get_Value( lAnnoIDIndex, &vaAnnoID );
									static_cast<CRelAnnotationParameter&>(**find(m_emTableType, vaAnnoID.lVal)).SetDispLine( *itScale, (IFeaturePtr)ipDisp, bRecycling, spDispFields, spDispNameString );
									TRACEMESSAGE(ADD_SELECTED_DISPPOINT, strDispTableName, AheGetOID( ipDisp ) );
								}
							}
						}
					}
/*
					// この時点で、「検索矩形内に存在しない真位置」に関連する表示位置フィーチャが不足している（都市地図以外）ので、再度表示位置を検索する
					if( !listExtendAnnoIDs.empty() )
					{
						for( scale_iterator itScale = scale_begin(); itScale != scale_end(); ++itScale )
						{
							// 検索が指定されており、表示位置テーブルが限定されている場合はそれ以外を飛ばす
							if( ipDispCursor && pDispTableType && *itScale != *pDispTableType )
								continue;

							// 表示位置レイヤを取得（オーナー名がある場合はオーナー名をつける）
							CString strDispClassName;
							if( 0 < lstrlen(lpcszOwnerName) )
								strDispClassName.Format(_T("%s.%s"), lpcszOwnerName, AheTableType2TableName( *itScale ) );
							else
								strDispClassName = AheTableType2TableName( *itScale );					
							CFeatureClass cDispClass( *itScale, OpenTable( strDispClassName ) );

							LOGASSERTEERR_IF( cDispClass != NULL, sindyErr_TableNotFound )
							{
								// 共有オブジェクト取得
								CSPTableNameString spDispNameString( cDispClass.GetNameString() );
								CSPFieldMap spDispFields( cDispClass.GetFieldMap() );

								// 少しでも高速化するために、真位置IDのインデックス番号を取得しておく
								long lAnnoIDIndex = spDispFields->FindField( schema::disp_line::kAnnoID );	// インデックス番号
								VARIANT vaAnnoID;												// 関連ID保持用

								SASSERT( lAnnoIDIndex >= 0 );	// インデックス番号は0以上

								VariantInit( &vaAnnoID );

								// WHERE句のINの中身を作成
								std::list<CString> listStrIDs;
								cDispClass.IDs2Str( listExtendAnnoIDs, listStrIDs );

								for( std::list<CString>::const_iterator itStrIDs = listStrIDs.begin(); itStrIDs != listStrIDs.end(); ++itStrIDs )
								{
									CString strWhere;
									strWhere.Format(_T("%s IN (%s)"), schema::disp_line::kAnnoID ,*itStrIDs );
									IFeatureCursorPtr ipCursor = cDispClass.Search( AheInitQueryFilter( NULL, NULL, strWhere ), bRecycling );

									LOGASSERTEERR_IF( NULL != ipCursor, sindyErr_SearchFailed )
									{
										IFeature* ipDisp = NULL;
										while( S_OK == ipCursor->NextFeature( &ipDisp ) )
										{
											if( ipDisp == NULL ) break;

											LOGASSERTEERR_IF( SUCCEEDED( ipDisp->get_Value( lAnnoIDIndex, &vaAnnoID ) ), sindyErr_AOGetValueFailed )
											{
												SASSERT( vaAnnoID.vt == VT_I4 );	// 値はあるはず

												std::map<long,long>::const_iterator itID = mapAnnoID.find( vaAnnoID.lVal );
												if( mapAnnoID.end() != itID )
												{
													// 既に登録されていない場合のみ登録
													CSPRow spDisp( find( ipDisp ) );
													if( ! spDisp.get() )
														static_cast<CRelAnnotationParameter&>(**find(m_emTableType, vaAnnoID.lVal)).SetDispLine( *itScale, ipDisp, bRecycling, spDispFields, spDispNameString );
												}
												else {
													emErr = sindyErr_RelFeatureNotFound;	// 関連フィーチャが見つからない [bug 4000]
													if( pErrs )
													{
														CDispLine cDisp( ipDisp );
														pErrs->push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale() ) ) );
													}
													else
														ERRORLOG(emErr);
												}
											}
											ipDisp->Release();
											ipDisp = NULL;
										}
									}
								}
							}
						}
					}
*/
				}
			}
		}
	}

/*#ifdef _DEBUG
	CString strTmp;
	for( CAnnotations::const_iterator it = begin(); it != end(); ++it )
	{
		strTmp.Format(_T("%s OID=%d"), __func__, it->GetDispLine(sindyDisp_city).GetOID() );
		AheTraceGeometry( (IGeometry*)*it->GetDispLine(sindyDisp_city).GetShape(), strTmp );
	}
#endif // _DEBUG //*/
	TRACEMESSAGE(FINISH_SEARCH_ANNO, ((double)GetTickCount()-dStart)/(double)1000.0, g_dTotalCacheTime/(double)1000.0);

	// パラメータキャッシュを作成
	double dStartCalcParameter = GetTickCount();
	TRACEMESSAGE(START_CACHE_CREATION);
	UpdateParameter();
	TRACEMESSAGE(FINISH_CREATE_SEC, ((double)GetTickCount()-dStartCalcParameter)/(double)1000.0 );

	// g_bBufferedSearchがfalseの時は余計なものを落とす
	if( ! g_bBufferedSearch )
	{
		// 全体の形状のUNIONを作成し、それと検索形状を比較する
		rows_iterator it = begin(GetTableType());
		while( it != end(GetTableType()) )
		{
			bool bIsContain = false;
			for( scale_iterator itScale = scale_begin(), itScaleEnd = scale_end(); itScale != itScaleEnd; ++itScale )
			{
				// ここの検索ルーチンは S4～S1までを相手にするため、例えばe系でS4で表示していても実際に相手にしているのは
				// S3,S2,S1も含まれる。そのため、表示上検索範囲外であっても拾われているように見えるものが出てくる（bug 9639）。
				// 表示を真とする場合は現在の表示スケールのみで判断すべきなのだが、ここまでその情報を引っ張ってくることが
				// できないので、グローバル変数で定義されていたらそのスケールに限定することにする。
				if( g_emViewTableType != sindyTableType::unknown && g_emViewTableType != *itScale )
					continue;
				IGeometryPtr ipGeom( (*it)->GetShapeCopy( true, false, false, false, AheTableType2AnnotationScale( *itScale ) ) );
				if( ! cSearchGeom.Disjoint( ipGeom ) )
				{
					bIsContain = true;
					break;
				}
			}
			if( ! bIsContain ) {
				TRACEMESSAGE(REMOVE_OUT_OF_AREA, AheTableType2TableName(GetTableType()), (*it)->GetOID() );
				it = erase( GetTableType(), it );
			}
			else
				++it;
		}
	}

	if( bCreateIndex ) // インデックス作成は任意に（bug ）
	{
		// 空間インデックスを作成する
		double dStartCreateIndex = GetTickCount();
		TRACEMESSAGE(START_SPATIAL_CREATION);
		WKSEnvelope cRect = {-1.0};
		cSearchGeom.GetEnvelope()->QueryCoords( &cRect.XMin, &cRect.YMin, &cRect.XMax, &cRect.YMax );

		// 空間インデックスのグリッド
		double dGrid = 0.0001;
		switch( GetTableType() )
		{
			case sindyTableType::city_annotation:	dGrid = 0.0005; break; 
			case sindyTableType::base_annotation:	dGrid = 0.002;  break; // city の4倍
			case sindyTableType::middle_annotation:	dGrid = 0.012;  break; // base の16倍
			case sindyTableType::top_annotation:	dGrid = 0.192;  break; // middle の16倍
			default: break;
		}
		CreateSpatialIndex( cRect, dGrid );
		TRACEMESSAGE(FINISH_CREATE_SEC, ((double)GetTickCount()-dStartCreateIndex)/(double)1000.0 );
	}

	return emErr;
}

// 形状で検索する
sindyErrCode CAnnotations::SelectByObject( _IRow* /*ipRow*/, bool/* bRecycling = false*/, bool/* bCreateCache = false*/, bool bInit /*= true*/ )
{
	// 初期化
	if( bInit == true )
		clear();

	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	ERR_RETURN( emErr );
}

// 渡されたオブジェクトセットに関連するものを検索する
sindyErrCode CAnnotations::Select( _ISet* ipRowSet, bool bCreateCache/* = false*/, CErrorObjects* /* pErrs = NULL*/, bool bInit /*= true*/, bool/* bCreateIndex = true*/ )
{
	// SiNDYLib対応の属性ダイアログでしか使用されていないもののはずなので、ひとまず
	// 無効化しておく。検索系関数の役割分担を明確化しながら実装しなおす必要あり by furukawa.
	return sindyErr_NoImplimentation;
#if 0

	// 初期化
	if( bInit == true )
		clear();

	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	// 引数チェック
	if( ! ipRowSet )
	{
		emErr = sindyErr_ArgIsNull;
		return emErr;
	}

	std::list<CRow> listDispLine; // 表示位置一次格納用
	std::map<long,long> mapAnnoID; // 真位置のOBJECTIDとコンテナ位置の対応表

	// 共有オブジェクト取得
	CSPTableNameString spAnnoNameString( GetNameString() );
	CSPFieldMap spAnnoFieldMap( GetFieldMap() );

	long lCount = 0;
	ipRowSet->get_Count( &lCount );
	ipRowSet->Reset();
	for( long i = 0; i < lCount; ++i )
	{
		// 処理の都合上真位置から登録したほうが良いので、表示位置は別に取っておく
		IUnknownPtr ipUnk;
		ipRowSet->Next( &ipUnk );
		CRow cRow( (_IRowPtr)ipUnk );
		sindyTableType::ECode emTableType = cRow.GetTableType();
		if( AheIsAnnoPointTableType( emTableType ) )
		{
			// 真位置をコンテナに追加
			// 既存のものがある場合を考慮する必要があるのだが、とりあえず置いておく
			CSPRelAnnotationParameter spAnnoParam( new CRelAnnotationParameter( (_IRow*)cRow, emTableType, false, spAnnoFieldMap, spAnnoNameString ) );
			push_back( spAnnoParam, true ); // 既に追加されている場合を考慮
			if( bCreateCache )
				spAnnoParam->CreateCache( false, spAnnoFieldMap, spAnnoNameString );

			// OIDとリストのインデックス対応表作成
			mapAnnoID[cRow.GetOID()] = m_listRelAnno.size() - 1;
		}
		else if( AheIsAnnoLineTableType( cRow.GetTableType() ) )
			listDispLine.push_back( cRow );
	}
	// 表示位置に関連する真位置を検索する
	for( std::list<CRow>::const_iterator it = listDispLine.begin(); it != listDispLine.end(); ++it )
	{
		long lAnnoID = it->GetLongValueByFieldName( schema::disp_line::kAnnoID, -1 );
		// 対応表に存在しなければ引いてくる
		std::map<long,long>::const_iterator itID = mapAnnoID.find( lAnnoID );
		if( mapAnnoID.end() == itID )
		{
			m_listRelAnno.push_back( CRelAnnotationParameter( GetTableType(), GetFeature(lAnnoID) ) );
			if( bCreateCache )
				m_listRelAnno.rbegin()->CreateCache( false, spAnnoFieldMap, spAnnoNameString );

			// OIDとリストのインデックス対応表作成
			mapAnnoID[lAnnoID] = m_listRelAnno.size() - 1;

			itID = mapAnnoID.find( lAnnoID );
		}
		// 表示位置追加
		// 既に登録されていない場合のみ登録 <- 渡されたものを優先する必要があるんじゃない？？（後で直して）
		if( ! m_listRelAnno[itID->second].exist( it->GetTableType(), it->GetOID() ) )
			m_listRelAnno[itID->second].SetDispLine( it->GetTableType(), (IFeaturePtr)(_IRow*)*it, false/*, &spDispFields, &spDispNameString*/ );
	}
	// 既に登録されている真位置に関連する表示位置が全て渡されているとは限らないため、
	// 対応表に登録したIDに関連する表示位置を検索する
	std::list<long> listAnnoID;
	for( std::map<long,long>::const_iterator it = mapAnnoID.begin(); it != mapAnnoID.end(); ++it ) // IDリスト作成
		listAnnoID.push_back( it->first );
	for( scale_iterator itScale = scale_begin(); itScale != scale_end(); ++itScale )
	{
		CFeatureClass cDispClass( *itScale, OpenSameOwnerTable( AheTableType2TableName( *itScale ) ) );
		// 共有オブジェクト取得
		CSPTableNameString spDispNameString( cDispClass.GetNameString() );
		CSPFieldMap spDispFields( cDispClass.GetFieldMap() );
		LOGASSERTEERR_IF( cDispClass != NULL, sindyErr_TableNotFound )
		{
			_ICursorPtr ipCursor = cDispClass.GetRows( listAnnoID, false, schema::disp_line::kAnnoID );
			if( ipCursor )
			{
				LOGASSERTEERR_IF( ipCursor, sindyErr_SearchFailed )
				{
					CRow cRow;
					while( S_OK == ipCursor->NextRow( cRow.Release() ) )
					{
						long lAnnoID = cRow.GetLongValueByFieldName( schema::disp_line::kAnnoID, -1 );
						if( ! m_listRelAnno[mapAnnoID[lAnnoID]].exist( cRow.GetTableType(), cRow.GetOID() ) )
							m_listRelAnno[mapAnnoID[lAnnoID]].SetDispLine( cRow.GetTableType(), (IFeaturePtr)(_IRow*)cRow, false, spDispFields, spDispNameString );
					}
				}
			}
		}
	}
	return emErr;
#endif // if 0
}

// 空間インデックスを作成する
void CAnnotations::CreateSpatialIndex( const WKSEnvelope& cIndexRect, double dGrid )
{
	int nDivideX = (int)((cIndexRect.XMax-cIndexRect.XMin)/dGrid);
	nDivideX = ( nDivideX > 0 ) ? nDivideX : 1;
	int nDivideY = (int)((cIndexRect.YMax-cIndexRect.YMin)/dGrid);
	nDivideY = ( nDivideY > 0 ) ? nDivideY : 1;

	CreateSpatialIndex( cIndexRect, nDivideX, nDivideY );
}

// 空間インデックスを作成する
void CAnnotations::CreateSpatialIndex( const WKSEnvelope& cIndexRect, int nDivideX, int nDivideY )
{
	// 空間インデックス矩形作成
	//
	// 渡された矩形は通常はSelectByShapeで与えられた矩形であり、
	// 表示文字列バウンディングボックスや真位置記号バウンディングボックス、
	// コピー注記などを考慮すると、当然渡された矩形に対して大きくなることが
	// ある。そのままでインデックスを作成すると格納できないものが生じるため、
	// 冗長ではあるがここで再度矩形をチェックした上でインデックスを作成する。
	WKSEnvelope cAnnoRect[4], cDispRect[4];
	int nCount = ( GetTableType() == sindyTableType::city_annotation ) ? 1 : 4;
	for( int i = 0; i < nCount; ++i ){ cAnnoRect[i] = cIndexRect; cDispRect[i] = cIndexRect; }
	
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator itAnno = it->second.begin(); itAnno != it->second.end(); ++itAnno )
		{
			// 真位置矩形
			const CRelAnnotationParameter& cAnnoParam = static_cast<CRelAnnotationParameter&>(**itAnno);
			int nCount = 0;
			for( CRelAnnotationParameter::annorect_const_iterator itAnnoRect = cAnnoParam.annorect_begin(); itAnnoRect != cAnnoParam.annorect_end(); ++itAnnoRect, ++nCount )
			{
				cAnnoRect[nCount].XMin = min( cAnnoRect[nCount].XMin, itAnnoRect->XMin );
				cAnnoRect[nCount].YMin = min( cAnnoRect[nCount].YMin, itAnnoRect->YMin );
				cAnnoRect[nCount].XMax = max( cAnnoRect[nCount].XMax, itAnnoRect->XMax );
				cAnnoRect[nCount].YMax = max( cAnnoRect[nCount].YMax, itAnnoRect->YMax );
			}

			// 表示位置矩形
			for( CRelAnnotationParameter::const_iterator it2 = cAnnoParam.begin(); it2 != cAnnoParam.end(); ++it2 )
			{
				for( CRelAnnotationParameter::const_rows_iterator itDisp = it2->second.begin(); itDisp != it2->second.end(); ++itDisp )
				{
					const CDispLine& cDisp = static_cast<CDispLine&>(**itDisp);
					WKSEnvelope* pDispRectTmp = NULL;
					switch( cDisp.GetTableType() )
					{
						case sindyTableType::city_disp_line:
						case sindyTableType::c_sc1disp_line:
						case sindyTableType::b_sc4disp_line: 
						case sindyTableType::m_sc4disp_line: 
						case sindyTableType::t_sc4disp_line: pDispRectTmp = &cDispRect[0]; break;
						case sindyTableType::b_sc3disp_line: 
						case sindyTableType::m_sc3disp_line: 
						case sindyTableType::t_sc3disp_line: pDispRectTmp = &cDispRect[1]; break;
						case sindyTableType::b_sc2disp_line: 
						case sindyTableType::m_sc2disp_line: 
						case sindyTableType::t_sc2disp_line: pDispRectTmp = &cDispRect[2]; break;
						case sindyTableType::b_sc1disp_line: 
						case sindyTableType::m_sc1disp_line: 
						case sindyTableType::t_sc1disp_line: pDispRectTmp = &cDispRect[3]; break;
						default: ERRORLOG(sindyErr_UnknownTableType); SASSERT(false); break;
					}
					if( pDispRectTmp )
					{
						pDispRectTmp->XMin = min( cDisp.m_cRect.XMin, pDispRectTmp->XMin );
						pDispRectTmp->YMin = min( cDisp.m_cRect.YMin, pDispRectTmp->YMin );
						pDispRectTmp->XMax = max( cDisp.m_cRect.XMax, pDispRectTmp->XMax );
						pDispRectTmp->YMax = max( cDisp.m_cRect.YMax, pDispRectTmp->YMax );
					}
				}
			}
		}
	}

	// 空間インデックス初期化
	for( int i = 0; i < nCount; ++i )
	{
		m_cAnnoSpatialIndex[i].Init( cAnnoRect[i], nDivideX, nDivideY );
		m_cDispSpatialIndex[i].Init( cDispRect[i], nDivideX, nDivideY );
	}

	// 空間インデックス作成
	long lAnnoCount = 0;	// リストの順番を識別IDとする
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator itAnno = it->second.begin(); itAnno != it->second.end(); ++itAnno, ++lAnnoCount )
		{
			const CRelAnnotationParameter& cAnnoParam = static_cast<CRelAnnotationParameter&>(**itAnno);
			// 注記真位置空間インデックス作成
			int nIndex = 0;
			for( CRelAnnotationParameter::annorect_const_iterator itAnnoRect = cAnnoParam.annorect_begin(); itAnnoRect != cAnnoParam.annorect_end(); ++itAnnoRect, ++nIndex )
				m_cAnnoSpatialIndex[nIndex].Add( *itAnnoRect, lAnnoCount );

			// 注記表示位置空間インデックス作成
			for( CRelAnnotationParameter::const_iterator it2 = (*itAnno)->begin(); it2 != (*itAnno)->end(); ++it2 )
			{
				for( CRelAnnotationParameter::const_rows_iterator itDisp = it2->second.begin(); itDisp != it2->second.end(); ++itDisp )
				{
					const CDispLine& cDisp = static_cast<CDispLine&>(**itDisp);
					int nIndex = -1;
					switch( cDisp.GetTableType() )
					{
						case sindyTableType::city_disp_line:
						case sindyTableType::c_sc1disp_line:
						case sindyTableType::b_sc4disp_line: 
						case sindyTableType::m_sc4disp_line: 
						case sindyTableType::t_sc4disp_line: nIndex = 0; break;
						case sindyTableType::b_sc3disp_line: 
						case sindyTableType::m_sc3disp_line: 
						case sindyTableType::t_sc3disp_line: nIndex = 1; break;
						case sindyTableType::b_sc2disp_line: 
						case sindyTableType::m_sc2disp_line: 
						case sindyTableType::t_sc2disp_line: nIndex = 2; break;
						case sindyTableType::b_sc1disp_line: 
						case sindyTableType::m_sc1disp_line: 
						case sindyTableType::t_sc1disp_line: nIndex = 3; break;
						default: ERRORLOG(sindyErr_UnknownTableType); SASSERT(false); break;
					}
					m_cDispSpatialIndex[nIndex].Add( cDisp.m_cRect, lAnnoCount );
				}
			}
		}
	}
/*#ifdef _DEBUG
	// インデックスの中身がどれぐらいあるか調べる
	for( int i = 0; i < nCount; ++i )
	{
		m_cAnnoSpatialIndex[i].Trace();
		m_cDispSpatialIndex[i].Trace();
	}
#endif // ifdef _DEBUG*/
}

sindyErrCode CAnnotations::Move( const CGeometry&/* cGeom*/, const std::pair<double,double>&/* pairMove*/, sindyeMoveMode/* emMode*/, CErrorObjects&/* cErrObjects*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
#if 0
	for( const_iterator it = begin(); it != end(); ++it )
	{
		// 真位置
		it->GetShape()->Move( (IGeometry*)cGeom, pairMove, emMode, cErrObjects );

		for( CRelAnnotation::const_iterator itDisp = it->begin(); itDisp != it->end(); ++itDisp )
		{
			// 表示位置
			itDisp->GetShape()->
			if( ! cGeom.Disjoint( (IGeometry*)(*itDisp->GetShape()) ) )
				listFeatures.push_back( dynamic_cast<CRow*>(const_cast<CDispLine*>(&*itDisp)) );
		}
	}
#endif // if 0
	return emErr;
}

// 与えられた形状との交差部分で分割処理を行う
/*errorcode::sindyErrCode CAnnotations::Split( const CGeometry& cGeom, const CSplitTarget& cSplitTarget, CErrorObjects& cErrObjects )
{
	sindyErrCode emErr = NoErr; // 返り値

	return emErr;
}*/

IGeometryPtr _WKSPoints2Polygon( long lPointCount, WKSPoint* pPoints, ISpatialReference* ipSpRef )
{
	sindyErrCode emErr = sindyErr_NoErr;
	IGeometryCollectionPtr ipRet; // 返り値

	LOGASSERTERR_IF( SUCCEEDED( ipRet.CreateInstance( CLSID_Polygon ) ), sindyErr_COMCreateInstanceFailed )
	{
		((IGeometryPtr)ipRet)->putref_SpatialReference( ipSpRef );

		for( long i = 0; i < lPointCount/5; ++i )
		{
			IPointCollectionPtr ipCol;
			LOGASSERTERR_IF( SUCCEEDED( ipCol.CreateInstance( CLSID_Ring ) ), sindyErr_COMCreateInstanceFailed )
			{
				((IGeometryPtr)ipCol)->putref_SpatialReference( ipSpRef );
				LOGASSERTERR_IF( SUCCEEDED( ipCol->SetWKSPoints( 5, &pPoints[i*5] ) ), sindyErr_GeometryFunctionFailed )
				{
					LOGASSERTEERR_IF( SUCCEEDED( ipRet->AddGeometry( (IGeometryPtr)ipCol, &vtMissing, &vtMissing ) ), sindyErr_GeometryFunctionFailed )
					{
					}
				}
			}
		}
		LOGASSERTEERR_IF( SUCCEEDED( ((IPolygonPtr)ipRet)->SimplifyPreserveFromTo() ), sindyErr_GeometryFunctionFailed )
		{
		}
	}
	return ipRet;
}

// 整合性チェックを行う
errorcode::sindyErrCode CAnnotations::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const
{
	sindyErrCode emErr = sindyErr_NoErr;
	long lAnnoCount = 0;						// 管理リストカウント

	// 記号と文字列ポリゴンの距離チェック時の閾値（ピクセル）
	// ＊注意＊
	// 本当は、↓のような方法ではなく、CRelAnnotationParameter::CheckLogicにcRuleを渡して、そこでちゃんとレイヤ名を特定すべき
	std::pair<double,double> pairThresholdPx = cRule.GetRelThresholdPx( schema::city_annotation::kTableName, schema::city_disp_line::kTableName );

	IRelationalOperatorPtr ipAnnoRectRelOpe;	// 記号矩形と関連しない注記文字列ポリゴンとの重複チェック用
	IEnvelopePtr ipAnnoRect;
	// 後で何回も使用するので先にインスタンスを作っておく
	LOGASSERT_IF( SUCCEEDED( ipAnnoRectRelOpe.CreateInstance( CLSID_Envelope ) ), sindyErr_COMCreateInstanceFailed )
	{
		LOGASSERT_IF( SUCCEEDED( ipAnnoRectRelOpe->QueryInterface( IID_IEnvelope, (void**)&ipAnnoRect ) ), sindyErr_COMQueryInterfaceFailed )
		{
			LOGASSERT_IF( SUCCEEDED( ipAnnoRect->putref_SpatialReference( GetSpatialReference() ) ), sindyErr_COMFunctionFailed );
		}
	}

	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator itRelAnno = it->second.begin(); itRelAnno != it->second.end(); ++itRelAnno, ++lAnnoCount )
		{
			const CRelAnnotationParameter& cRelAnno = static_cast<const CRelAnnotationParameter&>(**itRelAnno);
			bool bCanOverlap = cRelAnno.CanOverlap();		// 重複が許される種別かどうか

			// 真位置単体チェック・表示位置単体チェック・真位置＆表示位置複合チェック
			emErr = cRelAnno.CheckLogic( cErrObjects, true, true, pairThresholdPx.first, pairThresholdPx.second );

			// 注記記号重複チェック
			if( ! bCanOverlap )
			{
				// スケールごとにチェック
				CRelAnnotationParameter::annorect_const_iterator itAnnoRect = cRelAnno.annorect_begin();
				for( int nIndex = 0; nIndex < cRelAnno.GetDispLayerCount(); ++nIndex )
				{
					if( cRelAnno.CanShowMark( cRelAnno.GetDispLayer(nIndex) ) )
					{
						// 空間インデックスから該当するものを選択
						std::list<long> listIDs;
						m_cAnnoSpatialIndex[nIndex].Search( *itAnnoRect, listIDs );
						for( std::list<long>::const_iterator itID = listIDs.begin(); itID != listIDs.end(); ++itID )
						{
							if( lAnnoCount == *itID ) continue;	// 同じものは飛ばす

							const_rows_iterator itRelAnno2(it->second.begin() + *itID);

							if( 0 < (std::list<CRelAnnotationParameter>::difference_type)(itRelAnno2-itRelAnno) )	// itRelAnno よりも itRelAnno2 が後ろにある場合にのみチェック
							{
								const CRelAnnotationParameter& cRelAnno2 = static_cast<const CRelAnnotationParameter&>(**itRelAnno2);

								// 記号同士の重複チェック（重複不可及び記号表示のもののみ）
								if( ! cRelAnno2.CanOverlap() && cRelAnno2.CanShowMark( cRelAnno2.GetDispLayer(nIndex) ) )
								{
									CRelAnnotationParameter::annorect_const_iterator itAnnoRect2 = cRelAnno2.annorect_begin();
									std::advance( itAnnoRect2, nIndex );
									if( WKSIsCross( *itAnnoRect, *itAnnoRect2 ) )
										cErrObjects.push_back( CErrorObject( cRelAnno, CErrorInfo( sindyErr_AnnoMarkAndMarkIsOverlaped, sindyErrLevel_ERR, NULL, NULL, cRelAnno.GetDispScale(nIndex), _T("%s"), AheTableType2ScaleTypeString( cRelAnno2.GetDispLayer(nIndex) ) ) ) );	// 記号同士が重なっている
								}
							}
						}
						// 関連しない注記表示文字列ポリゴンとの重複チェック
						LOGASSERTE_IF( NULL != ipAnnoRect, sindyErr_AlgorithmFailed )
						{
							// 記号矩形を作成
							LOGASSERT_IF( SUCCEEDED( ipAnnoRect->PutCoords( itAnnoRect->XMin, itAnnoRect->YMin, itAnnoRect->XMax, itAnnoRect->YMax ) ), sindyErr_GeometryFunctionFailed );
						}

						listIDs.clear();
						m_cDispSpatialIndex[nIndex].Search( *itAnnoRect, listIDs );
						for( std::list<long>::const_iterator itID = listIDs.begin(); itID != listIDs.end(); ++itID )
						{
							if( lAnnoCount == *itID ) continue;	// 同じものは飛ばす

							const_rows_iterator itRelAnno2(it->second.begin() + *itID);
							const CRelAnnotationParameter& cRelAnno2 = static_cast<const CRelAnnotationParameter&>(**itRelAnno2);
	//						if( 0 < (std::list<CRelAnnotationParameter>::difference_type)(itRelAnno2-itRelAnno) )	// itRelAnno よりも itRelAnno2 が後ろにある場合にのみチェック <- 記号矩形と文字列矩形のテストなので、重複することがないため「後ろにある場合」のみではまずい（bug 3767）
							{
								if( ! cRelAnno2.CanOverlap() )
								{
									for( CRelAnnotationParameter::const_iterator it2 = cRelAnno2.begin(); it2 != cRelAnno2.end(); ++it2 )
									{
										for( CRelAnnotationParameter::const_rows_iterator itDisp = it2->second.begin(); itDisp != it2->second.end(); ++itDisp )
										{
											const CDispLine& cDisp = static_cast<const CDispLine&>(**itDisp);
											if( cRelAnno.GetDispLayer(nIndex) == cDisp.GetTableType() )	// 同じテーブルタイプ同士のみチェック
											{
												VARIANT_BOOL vbDisjoint = VARIANT_FALSE;
												LOGASSERT_IF( SUCCEEDED( ipAnnoRectRelOpe->Disjoint( cDisp.m_ipGeom, &vbDisjoint ) ), sindyErr_GeometryFunctionFailed )
												{
													if( ! vbDisjoint )
														cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( sindyErr_AnnoMarkAndDispBoundsIsOverlaped, sindyErrLevel_ERR, NULL, NULL, cDisp.GetMaxScale() ) ) );
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}

			// 表示位置チェック
			for( CRelAnnotationParameter::const_iterator it2 = cRelAnno.begin(); it2 != cRelAnno.end(); ++it2 )
			{
				for( CRelAnnotationParameter::const_rows_iterator itDisp = it2->second.begin(); itDisp != it2->second.end(); ++itDisp )
				{
					const CDispLine& cDisp = static_cast<const CDispLine&>(**itDisp);
					// 表示位置重複チェック
					if( ! bCanOverlap )
					{
						IRelationalOperatorPtr ipRelOpe( cDisp.m_ipGeom );
						VARIANT_BOOL vbDisjoint = VARIANT_FALSE;			// 重複チェック用

						// 空間インデックスから該当するものを選択
						std::list<long> listIDs;
						long lIndex = -1;	// 空間インデックスのインデックス番号
						switch( cDisp.GetTableType() )
						{
							case sindyTableType::city_disp_line:
							case sindyTableType::c_sc1disp_line:
							case sindyTableType::b_sc4disp_line:
							case sindyTableType::m_sc4disp_line:
							case sindyTableType::t_sc4disp_line: lIndex = 0; break;
							case sindyTableType::b_sc3disp_line:
							case sindyTableType::m_sc3disp_line:
							case sindyTableType::t_sc3disp_line: lIndex = 1; break;
							case sindyTableType::b_sc2disp_line:
							case sindyTableType::m_sc2disp_line:
							case sindyTableType::t_sc2disp_line: lIndex = 2; break;
							case sindyTableType::b_sc1disp_line:
							case sindyTableType::m_sc1disp_line:
							case sindyTableType::t_sc1disp_line: lIndex = 3; break;
							default: ERRORLOG(sindyErr_UnknownTableType); SASSERT(false); break;	// あるわけない
						}
						m_cDispSpatialIndex[lIndex].Search( cDisp.m_cRect, listIDs );
						for( std::list<long>::const_iterator itID = listIDs.begin(); itID != listIDs.end(); ++itID )
						{
							if( lAnnoCount == *itID ) continue;	// 同じものは飛ばす

							const_rows_iterator itRelAnno2(it->second.begin() + *itID);
							
							if( 0 < (std::list<CRelAnnotationParameter>::difference_type)(itRelAnno2-itRelAnno) )	// itRelAnno よりも itRelAnno2 が後ろにある場合にのみチェック
							{
								const CRelAnnotationParameter& cRelAnno2 = static_cast<const CRelAnnotationParameter&>(**itRelAnno2);
								if( ! cRelAnno2.CanOverlap() )
								{
									for( CRelAnnotationParameter::const_iterator it3 = (*itRelAnno2)->begin(); it3 != (*itRelAnno2)->end(); ++it3 )
									{
										for( CRelAnnotationParameter::const_rows_iterator itDisp2 = it3->second.begin(); itDisp2 != it3->second.end(); ++itDisp2 )
										{
											const CDispLine& cDisp2 = static_cast<const CDispLine&>(**itDisp2);
											if( cDisp.GetTableType() == cDisp2.GetTableType() )	// 同じテーブルタイプ同士のみチェック
											{
												LOGASSERT_IF( SUCCEEDED( ipRelOpe->Disjoint( cDisp2.m_ipGeom, &vbDisjoint ) ), sindyErr_GeometryFunctionFailed )
												{
													if( ! vbDisjoint )
													{
														// 線状注記の表示文字列バウンディングボックスは構造上大きめになるが、そのためにエラーが大量に出てしまう。そこで、
														// 表示タイプが線状注記のもののみ警告とし、その場合に個々の文字列矩形と比較して本当に重複しているかどうかをチェックする
														if( ( cDisp.m_emLineType & sindyAnnoLine_Linear ) || ( cDisp2.m_emLineType & sindyAnnoLine_Linear ) )
														{
															// 比較元を作成
															IRelationalOperatorPtr ipRelOpe2 = ( cDisp.m_emLineType & sindyAnnoLine_Linear  ) ? _WKSPoints2Polygon( cDisp.m_nStringRectsSize  * 5, cDisp.m_pStringRects,  GetSpatialReference() ) : ipRelOpe;
															// 比較先を作成
															IGeometryPtr ipGeomTmp           = ( cDisp2.m_emLineType & sindyAnnoLine_Linear ) ? _WKSPoints2Polygon( cDisp2.m_nStringRectsSize * 5, cDisp2.m_pStringRects, GetSpatialReference() ) : cDisp2.m_ipGeom;

															// 比較
															LOGASSERT_IF( SUCCEEDED( ipRelOpe2->Disjoint( ipGeomTmp, &vbDisjoint ) ), sindyErr_GeometryFunctionFailed )
															{
																if( ! vbDisjoint )
																	cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( sindyErr_AnnoDispBoundsIsOverlaped, sindyErrLevel_ERR, NULL, NULL, cDisp.GetMaxScale() ) ) );
																else
																	cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( sindyErr_AnnoDispBoundsIsOverlaped, sindyErrLevel_WARN, NULL, NULL, cDisp.GetMaxScale() ) ) );
															}
														}
														else
															cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( sindyErr_AnnoDispBoundsIsOverlaped, sindyErrLevel_ERR, NULL, NULL, cDisp.GetMaxScale() ) ) );
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}

			// 真位置＆表示位置チェック
			if( sindyErr_NoErr != ( emErr = cRelAnno.CheckPosition() ) )
				cErrObjects.push_back( CErrorObject( cRelAnno, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, NULL, cRelAnno.GetDispScale(0) ) ) );
		}
	}
	return cErrObjects.GetDefaultErrCode();
}

} // sindy
