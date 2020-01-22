#include "stdafx.h"
#include "CheckDistancePost.h"
#include "CCheckDistancePost.h"
#include "Arguments.h"
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/directory_util.h>
#include "CFeatureClass.h"
#include "tracer/LinkTracer.h"
#include "StdOutput.h"
#include "CLogger.h"
#include <boost/tuple/tuple.hpp>
#include <ArcHelperEx/AheGeometryOp.h>
#include "ErrorType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
using namespace ATL;
using namespace err_type;

int _tmain( int argc, _TCHAR* argv[] )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	try
	{
		const arctl::coinitializer aCoInitializer;

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		try
		{
			// 標準出力管理クラス
			StdOutput stdOut;

			// コマンドライン引数処理
			sindy::CArguments args;
			if( !args.parse( argc, argv ) )
			{
				stdOut.writeErrMsg( _T( "コマンドライン引数に問題があります" ) );
				return 1;
			}

			// 標準出力ヘッダー出力
			CString stdOutHeader = stdOut.CreateLogHeader( args.m_strOpParam );
			cout << CT2A( stdOutHeader ) << endl;

			// 接続
			sindy::CCheckDistancePost cCheckDistancePost;

			if( !cCheckDistancePost.Connect( args.m_strDb.c_str() ) )
			{
				stdOut.writeErrMsg( _T( "ERROR:指定データベースに接続できません。" ) );
				return 0;
			}

			double dbBuff = _tcstod( args.m_strBuf.c_str(), NULL );

			// コストファイル読み込み
			cCheckDistancePost.ReadRoadCost( args.m_strRoadCost.c_str() );
			cCheckDistancePost.ReadLinkCost( args.m_strLinkCost.c_str() );

			// DistancePostPointフィーチャクラス取得
			if( !cCheckDistancePost.OpenDistancePostPoint() )
			{
				stdOut.writeErrMsg( _T( "ERROR:指定データベースに[DISTANCE_POST_POINT]はありません。" ) );
				return 1;
			}

			// ROAD_LINK/ROAD_NODEフィーチャクラス取得
			if( !cCheckDistancePost.OpenNetwork() )
			{
				stdOut.writeErrMsg( _T( "ERROR:指定データベースに[ROAD_LINK/ROAD_NODE]はありません。" ) );
				return 1;
			}

			// 作業開始時間出力
			CString startTime = stdOut.GetNow();
			stdOut.writeConsole( _T( "\nstart " ) + startTime + _T( "\n" ) );

			double dbPitch = _tcstod( args.m_strPitch.c_str(), nullptr );
			double dbRange = _tcstod( args.m_strRange.c_str(), nullptr );
			double dbSeparate = _tcstod( args.m_strSeparate.c_str(), nullptr );

			ISpatialReferenceFactory2Ptr ipSpRefFct( CLSID_SpatialReferenceEnvironment );
			IProjectedCoordinateSystemPtr ipPrjCoordinateSystem;
			ipSpRefFct->CreateProjectedCoordinateSystem( esriSRProjCS_TokyoJapan10, &ipPrjCoordinateSystem );

			// 距離標ポイントフィーチャクラス/空間参照取得
			IFeatureClassPtr distancePostFC = cCheckDistancePost.getDistancePost();
			ISpatialReferencePtr distancePostSpRef;
			( (IGeoDatasetPtr)distancePostFC )->get_SpatialReference( &distancePostSpRef );

			// 道路ノードフィーチャクラス/空間参照取得
			IFeatureClassPtr roadNodeFC = cCheckDistancePost.getRoadNode();
			ISpatialReferencePtr roadNodeSpRef;
			( (IGeoDatasetPtr)roadNodeFC )->get_SpatialReference( &roadNodeSpRef );

			// 道路リンクフィーチャクラス取得
			IFeatureClassPtr roadLinkFC = cCheckDistancePost.getRoadLink();
			long roadCodeIdx = -1;
			IFieldsPtr ipFields;
			if( SUCCEEDED( roadLinkFC->get_Fields( &ipFields ) ) ) {
				ipFields->FindField( CComBSTR( sindy::schema::road_link::kRoadCode ), &roadCodeIdx );
			}
			IGeoDatasetPtr ipGeoDataset = (IGeoDatasetPtr)roadLinkFC;
			ISpatialReferencePtr roadLinkSpRef;
			if( ipGeoDataset )
				ipGeoDataset->get_SpatialReference( &roadLinkSpRef );

			// 路線リストチェック
			std::map<long, long> mapErr;
			std::map<long, std::pair<std::list<double>, std::map<double, long>>> mapContainer;
			long nRet = cCheckDistancePost.CheckRoadList( args.m_strList.c_str(), dbPitch, mapContainer, mapErr );
			switch( nRet )
			{
			case no_problem:
				break;
			case less_point:
				// TODO:各for文を関数化
				for( const auto& item : mapErr )
				{
					if( item.second == 0 )	// 正常
						continue;

					boost::tuple<long, long, CString> roadInfo = cCheckDistancePost.GetRoadList( item.first );
					CString strTmp;
					strTmp.Format( _T( "ERROR:<%d><%d>の距離標ポイントが2未満です。" ), roadInfo.get<0>(), roadInfo.get<1>() );
					stdOut.writeErrMsg( strTmp );
				}
				return 1;
			case short_distance:
				for( const auto& item : mapErr )
				{
					if( item.second == 0 )	// 正常
						continue;

					boost::tuple<long, long, CString> roadInfo = cCheckDistancePost.GetRoadList( item.first );
					CString strTmp;
					strTmp.Format( _T( "ERROR:<%d><%d>の最大距離標値と最少距離標値の差が<%.1lf>以下です。" ), roadInfo.get<0>(), roadInfo.get<1>(), dbPitch );
					stdOut.writeErrMsg( strTmp );
				}
				return 1;
			default:
				for( const auto& item : mapErr )
				{
					if( item.second == 0 )	// 正常
						continue;

					CString strTmp;
					strTmp.Format( _T( "ERROR:路線リスト<%d 行目>が正しく指定されていません。" ), item.first );
					stdOut.writeErrMsg( strTmp );
				}
				return 1;
			}

			// 2条間チェック用 map<路線コード, map<方向コード, map<距離標記号, 最短経路形状>>>
			std::map<long, std::map<long, std::map<CString, CComPtr<IPolyline>>>> mapRoadGeoms;
			// 2条間チェック用 map<路線コード, map<方向コード, map<距離標記号, map<距離標値, pair<OID, ポイント形状>>>>>
			std::map<long, std::map<long, std::map<CString, std::map<double, std::pair<long, CComPtr<IPoint>>>>>> mapPts;

			// 路線リスト毎に処理
			for( const auto& cRoadList : mapContainer )
			{
				// 対象路線の情報取得(路線コード, 方向コード, 距離標記号)
				boost::tuple<long, long, CString> roadInfo = cCheckDistancePost.GetRoadList( cRoadList.first );

				// 指定された路線ごとの作業開始時間をコンソールに出力
				CString roadInfoMsg;
				roadInfoMsg.Format( _T( "%d\t%d\t%s" ), roadInfo.get<0>(), roadInfo.get<1>(), roadInfo.get<2>() );
				stdOut.writeRoadInfoMsg( roadInfoMsg );

				bool bNG = false;
				// 実行ログファイル管理クラスオブジェクト
				sindy::CSPLogger pLogger;
				pLogger.reset( new sindy::CLogger() );
				pLogger->initLog( args.m_strLogPath.c_str(), roadInfo.get<0>(), roadInfo.get<1>(), roadInfo.get<2>(), args.m_strOpParam, true );

				// 距離標ポイント検索
				cCheckDistancePost.searchDistancePoint( cRoadList.second.second );

				// 距離標ポイント群(距離標値, CSRowBase)
				std::map<double, sindy::CSPRowBase> mapRows = cCheckDistancePost.findDistancePoints();
				std::list<long> listNotExcludeRoads; // 除外してはいけない道路（距離標ポイントが乗っている道路）
				std::list<long> listExcludeMesh;
				std::list<long> listRouteIDs; // 追加した道路リンクのOID保持用
				std::list<std::pair<sindy::CSPRowBase, CString>> listLenErrs; // ポイント間距離のエラーメッセージ格納用リスト list(CSPRowBase, エラーメッセージ)
				bool allPointOnRoad = true;	// 全ての距離標ポイントが路線上に乗っているか
				IPolylinePtr allRoutePolyline( CLSID_Polyline ); // 全体形状格納用
				list<long> allRouteIDs;		// 最短経路となる全道路リンクのOID
				list<long> excludeOIDs;		// 形状格納済のOID格納用
				IPointPtr toPointOfStartRoad( CLSID_Point ); // 始点・次点が乗っている経路のTo点

				// 距離標ポイント毎のループ [bug12155]
				for( std::list<double>::const_iterator it = cRoadList.second.first.begin(); it != cRoadList.second.first.end(); ++it )
				{
					// 基点取得
					sindy::CSPRowBase pFirstRow = mapRows[ ( *it ) ];
					double basePointDistance = ( *it ); // 基点の距離標値

					// 基点が乗っている道路取得
					sindy::CFeatureClass::Container firstRoadContainer = cCheckDistancePost.searchRoads( pFirstRow->GetShapeCopy(), dbBuff );
					if( firstRoadContainer.empty() )
					{
						CString strTmp;
						strTmp.Format( _T( "ERROR:<%d>が道路リンク上に設定されていません。" ), pFirstRow->GetOID() );
						pLogger->AddLog( pFirstRow, 1000, strTmp );
						allPointOnRoad = false;
						continue;
					}

					// 除外してはいけない道路リストに格納
					for( const auto& cRoads : firstRoadContainer )
						listNotExcludeRoads.push_back( cRoads.first );

					// 次点取得
					auto nextIt = std::next( it );
					if( nextIt == cRoadList.second.first.end() ) break;
					sindy::CSPRowBase pSecondRow = mapRows[ ( *nextIt ) ];
					double nextPointDistance = ( *nextIt ); // 次点の距離標値
					// 次点が乗っている道路取得
					sindy::CFeatureClass::Container secondRoadContainer = cCheckDistancePost.searchRoads( pSecondRow->GetShapeCopy(), dbBuff );
					if( secondRoadContainer.empty() )
					{
						CString strTmp;
						strTmp.Format( _T( "ERROR:<%d>が道路リンク上に設定されていません。" ), pSecondRow->GetOID() );
						pLogger->AddLog( pSecondRow, 1000, strTmp );
						allPointOnRoad = false;
						it = nextIt;
						continue;
					}

					std::list<long> listSecondRoads; // 次点のOID格納用
					for( const auto& cRoads : secondRoadContainer )
						listSecondRoads.push_back( cRoads.first );

					// 除外してはいけない道路リストに次点が乗っている道路を格納
					for( const auto& cSecond : listSecondRoads )
						listNotExcludeRoads.push_back( cSecond );

					// 基点、次点が乗っている道路が対象路線コードを持っているか
					bool firstRoadOnHWY = cCheckDistancePost.checkOnHWY( firstRoadContainer, roadInfo.get<0>(), roadCodeIdx );
					bool secondRoadOnHWY = cCheckDistancePost.checkOnHWY( secondRoadContainer, roadInfo.get<0>(), roadCodeIdx );

					// 検索対象となる道路格納用コンテナ. まずは基点が乗っている道路を格納.
					sindy::CFeatureClass::Container cRoads = firstRoadContainer;

					// 次点が乗っている道路があるかどうか
					bool nextRoadFind = false;
					long nBase = 1;

					// 最短経路の候補なる道路リンクを取得
					while( !nextRoadFind )
					{
						sindy::CFeatureClass::Container cRoads1;
						std::list<long> listMesh;
						IGeometryPtr searchGeom = cCheckDistancePost.searchRoadGeoms( cRoads, nBase, listExcludeMesh, listMesh );

						if( firstRoadOnHWY && secondRoadOnHWY ) {
							// 対象路線コードの道路に、基点、次点の両方が乗っているときは、路線コードで検索
							cRoads1 = cCheckDistancePost.searchRoads( searchGeom, dbBuff, roadInfo.get<0>() );
						}
						else {
							// cRoadsTmp内の道路が存在するメッシュ内の検索対象道路を全て取得
							cRoads1 = cCheckDistancePost.searchRoads( searchGeom, dbBuff );
						}
						listExcludeMesh.merge( listMesh );
						cRoads.insert( cRoads1.begin(), cRoads1.end() );

						// 道路除外
						cCheckDistancePost.excludeRoad( cRoads, listNotExcludeRoads, listRouteIDs );

						// 次点が乗っている道路があるか確認
						for( const auto& cRoad : cRoads1 )
						{
							if( listSecondRoads.end() != std::find( listSecondRoads.begin(), listSecondRoads.end(), cRoad.first ) )
							{
								nextRoadFind = true;
								break;
							}
						}
						if( !nextRoadFind )
						{
							++nBase;
						}
					}

					// 経路探索開始
					sindy::LinkTracer cLinkTracer;
					cLinkTracer.toStartSelectLinkSection();
					cLinkTracer.init_cache( cCheckDistancePost.getRoadLink(), dbBuff );

					// 経路探索クラスに道路を渡す
					for( const auto& cRoad : cRoads )
						cLinkTracer.add_cache( cRoad.first, (_IRow*)( *cRoad.second.get() ) );
					cLinkTracer.cache();

					// 経路探索
					cLinkTracer.onNormalMode( pFirstRow->GetShapeCopy() );
					cLinkTracer.onNormalMode( pSecondRow->GetShapeCopy() );

					// 経路探索形状取得
					IPolylinePtr ipPolyline( CLSID_Polyline );
					ipPolyline = cLinkTracer.getRouteGeom();
					ipPolyline->putref_SpatialReference( distancePostSpRef );
					if( !ipPolyline )
					{
						pLogger->AddLog( pFirstRow, 1001, _T( "ERROR:経路計算に失敗しました。道路リンク上に設定されていない可能性があります。" ) );
						allPointOnRoad = false;
						break;
					}
					// クローン取得
					ipPolyline = cCheckDistancePost.Clone( ipPolyline );

					// 座標系変更 
					ipPolyline->Project( ipPrjCoordinateSystem );

					// 基点形状を取得
					IPointPtr ipFirstP( CLSID_Point );
					ipFirstP = pFirstRow->GetShapeCopy();
					ipFirstP->putref_SpatialReference( distancePostSpRef );
					ipFirstP->Project( ipPrjCoordinateSystem );	// 座標系を変更

					// 次点形状を取得
					IPointPtr ipSecondP( CLSID_Point );
					ipSecondP = pSecondRow->GetShapeCopy();
					ipSecondP->putref_SpatialReference( distancePostSpRef );
					ipSecondP->Project( ipPrjCoordinateSystem );	// 座標系を変更

					// 経路探索した道路が1本の時に、形状の向きが道路の向きになっている事があるので
					// 向きを距離標ポイントが置かれている方向にあわせる
					double checkPointLen = cCheckDistancePost.getPointLen( ipPolyline, ipFirstP, ipSecondP );

					// 次点とラインの始点までの距離が基点とラインの始点までの距離よりも近いなら反転
					if( checkPointLen < 0.0f ){
						ipPolyline->ReverseOrientation();
					}

					if( allPointOnRoad )
					{
						// 距離標の乗っている方向に向きをそろえるため、始点～次点が乗っている経路は先に格納しておく
						if( it == cRoadList.second.first.begin() ){
							allRoutePolyline = ipPolyline;
							allRoutePolyline->Project( ipPrjCoordinateSystem );
						}

						// 最短経路となるリンクのOIDを格納
						for( const auto& cRoute : cLinkTracer.getRouteIDs() ){
							if( it == cRoadList.second.first.begin() ){
								// 形状を格納したリンクのOIDは除外リストに追加
								excludeOIDs.push_back( cRoute );
							}
							else{
								// 重複しないようにOIDを格納する
								if( allRouteIDs.end() == std::find( allRouteIDs.begin(), allRouteIDs.end(), cRoute ) ){
									allRouteIDs.push_back( cRoute );
								}
							}
						}
					}

					// 基点までの距離
					double toBasePointLen = cCheckDistancePost.split_point_length( ipPolyline, ipFirstP );

					// 次点までの距離
					double toNextPointLen = cCheckDistancePost.split_point_length( ipPolyline, ipSecondP );

					// 距離標ポイント間の道路の距離を取得
					double pointLen = toNextPointLen - toBasePointLen;

					// 距離標値の差を取得
					double distanceDiff = nextPointDistance - basePointDistance;

					// 距離標値の差と距離標ポイント間の距離をチェック
					if( !cCheckDistancePost.checkRoadLen( pointLen, distanceDiff, dbPitch, dbRange ) )
					{
						CString strErr;
						strErr.Format( _T( "ERROR:次点との距離標ポイント間距離と処理ピッチの差異、または距離標値の差と処理ピッチの差異が%.1lf％以上あります。" ), dbRange );
						listLenErrs.push_back( std::make_pair( pFirstRow, strErr ) );
					}
				}

				// ポイント間距離に関するエラーメッセージを出力
				if( !listLenErrs.empty() ){
					for( const auto& cErr : listLenErrs )
						pLogger->AddLog( cErr.first, 1002, cErr.second );
				}

				if( !allPointOnRoad ){
					stdOut.writeConsole( _T( "正常終了" ) );
					continue;
				}

				// 形状格納済のリンクのOIDを削除する
				for(const auto& OID : excludeOIDs){
					if( allRouteIDs.end() != std::find( allRouteIDs.begin(), allRouteIDs.end(), OID ) ){
						allRouteIDs.remove( OID );
					}
				}
				// 全体の経路形状を取得
				sindy::CFeatureClass::Container allRoads = cCheckDistancePost.searchRoadsByOID( allRouteIDs );
				cCheckDistancePost.getRoutePolyline( allRoutePolyline, allRoads, allRouteIDs, ipPrjCoordinateSystem );
				allRoutePolyline->Project( ipPrjCoordinateSystem );	// 座標系を変更

				// 2条間距離チェック用mapを作成
				mapRoadGeoms[ roadInfo.get<0>() ][ roadInfo.get<1>() ][ roadInfo.get<2>() ] = allRoutePolyline;

				for( const auto& it : cRoadList.second.first )
				{
					sindy::CSPRowBase pRow = mapRows[ it ];

					IPointPtr ipPoint( CLSID_Point );
					ipPoint->putref_SpatialReference( distancePostSpRef );
					ipPoint = pRow->GetShapeCopy();
					ipPoint->Project( ipPrjCoordinateSystem );	// 座標系を変更

					mapPts[ roadInfo.get<0>() ][ roadInfo.get<1>() ][ roadInfo.get<2>() ][ it ] = std::pair<long, CComPtr<IPoint>>( pRow->GetOID(), (IPoint*)ipPoint );
				}

				// 2条間距離チェック
				for( std::map<long, std::map<long, std::map<CString, std::map<double, std::pair<long, CComPtr<IPoint>>>>>>::iterator it = mapPts.begin(); it != mapPts.end(); ++it )
				{
					// it->first	路線コード
					// it->second	map<方向コード,map<>>

					// 今見ている路線でない、または路線コードに２つの方向コードが紐づいていない
					if( it->first != roadInfo.get<0>() || it->second.size() != 2 )
						continue;

					std::map<long, std::map<CString, std::map<double, std::pair<long, CComPtr<IPoint>>>>>::iterator itFirst = it->second.begin();
					std::map<long, std::map<CString, std::map<double, std::pair<long, CComPtr<IPoint>>>>>::iterator itSecond = it->second.begin();
					++itSecond;

					// 距離標記号毎のループ
					for( const auto& pPts2 : itFirst->second )
					{
						sindy::CSPLogger pSeparateLogger;
						pSeparateLogger.reset( new sindy::CLogger() );
						pSeparateLogger->initLog( args.m_strLogPath.c_str(), it->first, itFirst->first, pPts2.first, args.m_strOpParam, false );

						// 同じ"距離標記号"があるか
						if( itSecond->second.end() == itSecond->second.find( pPts2.first ) )
							break;

						// pPts2.first	距離標記号
						// pPts2.second	map<距離標値,pair<OID, ポイント形状>>
						for( const auto& pPt : pPts2.second ) // 距離標値毎のループ
						{
							// pPt.first	距離標値
							// pPt.second	pair<OID,ポイント形状>
							if( itSecond->second[ pPts2.first ].end() == itSecond->second[ pPts2.first ].find( pPt.first ) )	// 同じ"距離標値"があるか
							{
								CString strTmp;
								strTmp.Format( _T( "ERROR:同一路線に同一距離標値<%.1lf>が存在しません。" ), pPt.first );
								pSeparateLogger->AddLog( pPt.second.first, pPt.first, pPts2.first, 1003, strTmp );
								continue;
							}

							// 反対路線の最近傍リンクへの距離を取得
							IPointPtr point1 = (IPoint*)pPt.second.second;
							IPointPtr point2 = (IPoint*)itSecond->second[ pPts2.first ].find( pPt.first )->second.second;

							double dbDist1 = 0.0f;
							IProximityOperatorPtr ipProxy1 = (IPolyline*)mapRoadGeoms[ it->first ][ itFirst->first ][ pPts2.first ];
							if( FAILED( ipProxy1->ReturnDistance( point2, &dbDist1 ) ) )
							{
								_ASSERT( FALSE );
								continue;
							}

							double dbDist2 = 0.0f;
							IProximityOperatorPtr ipProxy2 = (IPolyline*)mapRoadGeoms[ it->first ][ itSecond->first ][ pPts2.first ];
							if( FAILED( ipProxy2->ReturnDistance( point1, &dbDist2 ) ) )
							{
								_ASSERT( FALSE );
								continue;
							}

							// 距離標ポイント間の距離取得
							IPolylinePtr ipDppPoly( CLSID_Polyline );
							ipDppPoly->putref_SpatialReference( distancePostSpRef );
							ipDppPoly->Project( ipPrjCoordinateSystem );	// 座標系変更
							ipDppPoly->put_FromPoint( pPt.second.second );
							ipDppPoly->put_ToPoint( itSecond->second[ pPts2.first ].find( pPt.first )->second.second );

							double dbLen = 0.0f;
							ipDppPoly->get_Length( &dbLen );

							// 比較
							double dbPointDiff1 = ( abs( dbLen - dbDist1 ) / dbLen ) * 100;
							double dbPointDiff2 = ( abs( dbLen - dbDist2 ) / dbLen ) * 100;

							if( dbPointDiff1 >= dbSeparate || dbPointDiff2 >= dbSeparate )
							{
								CString strTmp;
								strTmp.Format( _T( "ERROR:同一路線の同一距離標ポイント間距離が、上下線の最近傍リンクと<%.1f>％以上離れている。" ), dbSeparate );
								pSeparateLogger->AddLog( pPt.second.first, pPt.first, pPts2.first, 1004, strTmp );
							}
						}
					}
				}
				stdOut.writeConsole( _T( "正常終了" ) );
			}
			// 全指定路線の作業終了時間出力
			CString endTime = stdOut.GetNow();
			stdOut.writeConsole( _T( "\nend " ) + endTime );
			stdOut.writeConsole( _T( "正常終了" ) );

			return 0;
		}
		catch( const _com_error& e )
		{
			throw std::runtime_error( uh::toStr( atl2::stringize( e ) ) );
		}
	}
	catch( const std::exception& e )
	{
		cerr << e.what() << endl;
		cerr << "異常終了" << endl;
		return 1;
	}
}