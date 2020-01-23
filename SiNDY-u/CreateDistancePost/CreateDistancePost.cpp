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
#include "CreateDistancePost.h"
#include "CCreateDistancePost.h"
#include "Arguments.h"
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/directory_util.h>
#include "CFeatureClass.h"
#include "CStdOutput.h"
#include "CLogger.h"
#include <ArcHelperEx/AheGeometryOp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
using namespace ATL;

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	try
	{
		const arctl::coinitializer aCoInitializer;

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		try
		{
			// 標準出力管理クラスオブジェクト
			CStdOutput stdOut;

			// コマンドライン引数処理
			sindy::CArguments args;
			if(! args.parse(argc, argv))
			{
				stdOut.writeErrMsg(_T("コマンドライン引数に問題があります"));
				return 1;
			}

			// 標準出力ヘッダー出力
			CString stdOutHeader = stdOut.CreateLogHeader(args.m_strOpParam);
			cout << CT2A(stdOutHeader) << endl;

			// 接続
			sindy::CCreateDistancePost cCreateDistancePost;

			if ( ! cCreateDistancePost.Connect(args.m_strDb.c_str()) )
			{
				stdOut.writeErrMsg(_T("ERROR:指定データベースに接続できません。"));
				return 1;
			}

			// コストファイル読み込み
			cCreateDistancePost.ReadRoadCost(args.m_strRoadCost.c_str());
			cCreateDistancePost.ReadLinkCost(args.m_strLinkCost.c_str());

			// DistancePostPointフィーチャクラス取得
			if ( ! cCreateDistancePost.OpenDistancePostPoint() )
			{
				stdOut.writeErrMsg(_T("ERROR:指定データベースに[DISTANCE_POST_POINT]はありません。"));
				return 1;
			}

			// ROAD_LINK/ROAD_NODEフィーチャクラス取得
			if ( ! cCreateDistancePost.OpenNetwork() )
			{
				stdOut.writeErrMsg(_T("ERROR:指定データベースに[ROAD_LINK/ROAD_NODE]はありません。"));
				return 1;
			}

			// 作業開始時間出力
			CString startTime = stdOut.GetNow();
			stdOut.writeConsole(_T("\nstart ") + startTime + _T("\n"));

			double dbPitch = _tcstod(args.m_strPitch.c_str(), nullptr);	// ピッチ
			double dbBuff = _tcstod(args.m_strBuf.c_str(), nullptr);	// バッファ

			// 処理ピッチから補正点間の距離を取得
			double correctionPointLen = (dbPitch) * 10;

			// 路線リストチェック
			std::map<long,long> mapErr;
			std::map<long, std::pair<std::list<double>, std::map<double, long>>> mapContainer;
			long nRet = cCreateDistancePost.CheckRoadList(args.m_strList.c_str(), dbPitch, mapContainer, mapErr);
			if ( 0 != nRet )
			{
				if ( -6 == nRet )
				{
					for ( const auto& item : mapErr )
					{
						if ( item.second == 0 )	// 正常
							continue;

						boost::tuple<long,long,CString> pair = cCreateDistancePost.GetRoadList(item.first);
						CString strTmp;
						strTmp.Format(_T("ERROR:<%ld><%ld><%s>の距離標ポイントが2未満です。"), pair.get<0>(), pair.get<1>(), pair.get<2>() );
						stdOut.writeErrMsg(strTmp);
					}
				}
				else if ( -7 == nRet )
				{
					for ( const auto& item : mapErr )
					{
						if ( item.second == 0 )	// 正常
							continue;

						boost::tuple<long,long,CString> pair = cCreateDistancePost.GetRoadList(item.first);
						CString strTmp;
						strTmp.Format(_T("ERROR:<%ld><%ld><%s>の最大距離標値と最少距離標値の差が<%.1f>以下です。"), pair.get<0>(), pair.get<1>(), pair.get<2>(), dbPitch );
						stdOut.writeErrMsg(strTmp);
					}
				}
				else
				{
					for ( const auto& item : mapErr )
					{
						if ( item.second == 0 )	// 正常
							continue;

						CString strTmp;
						strTmp.Format(_T("ERROR:路線リスト<%ld 行目>が正しく指定されていません。"), item.first );
						stdOut.writeErrMsg(strTmp);
					}
				}

				return 1;
			}

			long nMIdx = cCreateDistancePost.GetFieldIndex( sindy::schema::distance_post_point::kDistanceMark );

			// 座標系変更
			ISpatialReferenceFactory2Ptr ipSpRefFct(CLSID_SpatialReferenceEnvironment);
			IProjectedCoordinateSystemPtr ipPrjCoordinateSystem;
			ipSpRefFct->CreateProjectedCoordinateSystem(esriSRProjCS_TokyoJapan10, &ipPrjCoordinateSystem);

			// 路線リスト毎に処理
			for ( const auto& cRoadList : mapContainer )
			{
				boost::tuple<long,long,CString> pair = cCreateDistancePost.GetRoadList(cRoadList.first);

				// 指定された路線ごとの作業開始時間をコンソールに出力
				CString roadInfoMsg;
				roadInfoMsg.Format(_T("%ld\t%ld\t%s"), pair.get<0>(), pair.get<1>(), pair.get<2>());
				stdOut.writeRoadInfoMsg(roadInfoMsg);

				// 実行ログファイル管理クラスオブジェクト
				sindy::CSPLogger pLogger;
				pLogger.reset( new sindy::CLogger() );
				pLogger->initLog(args.m_strLogPath.c_str(), pair, args.m_strOpParam);

				// 距離標ポイント検索
				cCreateDistancePost.searchDistancePoint(cRoadList.second.second);

				std::map<double, sindy::CSPRowBase> mapRows = cCreateDistancePost.findDistancePoints();

				// 路線形状格納用
				IPolylinePtr ipAllPolyline;
				
				// 生成する距離標ポイント格納用
				std::map<double, IPointPtr> mapPoints;

				// エラーメッセージ格納用
				CString errMsg;

				// 生成する距離標ポイントを取得
				if ( !cCreateDistancePost.getCreateDistancePost(cRoadList.second.first, mapRows, 
					ipPrjCoordinateSystem, ipAllPolyline, mapPoints, dbPitch, dbBuff, correctionPointLen, errMsg) )
				{
					stdOut.writeErrMsg(errMsg);
					continue;
				}

				if ( ipAllPolyline )
				{
					// 経路探索道路形状の距離取得
					double dbLen = 0.0f;
					ipAllPolyline->get_Length(&dbLen);

					if ( dbLen <= dbPitch )
					{
						CString strTmp;
						strTmp.Format(_T("ERROR:<%ld><%ld><%s>の処理対象道路リンク長距離<%.1lf>が<%.1lf>以下です。"), pair.get<0>(), pair.get<1>(), pair.get<2>(), dbLen, dbPitch );
						stdOut.writeErrMsg(strTmp);
						continue;
					}

					cCreateDistancePost.startEdit();

					// 同じ距離標値があれば、そのポイントは作成しない
					long nEditCnt = 0;
					bool bEdit = 0;
					for ( const auto& pPt : mapPoints )
					{
						CString strTmp( cCreateDistancePost.convertStr(cCreateDistancePost.convert( pPt.first, esriMeters, esriKilometers )) );
						// 距離標値で検索
						// 同じ距離標値のポイントは作成しない
						sindy::CSPRowBase pRow = mapRows[_tcstod(strTmp, nullptr)];

						if ( ! pRow )
						{
							// 作成
							pRow = cCreateDistancePost.createDistancePoint(pPt.second, _tcstod(strTmp, nullptr), cRoadList.first );

							CComVariant vaVal( pRow->GetValue(nMIdx) );

							pLogger->Log(pRow->GetOID(), CString(vaVal.bstrVal), _tcstod(strTmp, nullptr));

							++nEditCnt;
							bEdit = true;
						}

						if ( nEditCnt >= 1000 )
						{
							cCreateDistancePost.stopEdit();
							cCreateDistancePost.startEdit();
							nEditCnt = 0;
						}
					}

					if (bEdit)
						cCreateDistancePost.stopEdit();
					else
						cCreateDistancePost.abortEdit();

					stdOut.writeConsole(_T("正常終了"));
				}
			}
			// 全指定路線の作業終了時間出力
			CString endTime = stdOut.GetNow();
			stdOut.writeConsole(_T("\nend ") + endTime);
			stdOut.writeConsole(_T("正常終了"));
			return 0;
		}
		catch(const _com_error& e)
		{
			throw std::runtime_error(uh::toStr(atl2::stringize(e)));
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << "異常終了" << endl;
		return 1;
	}
}

