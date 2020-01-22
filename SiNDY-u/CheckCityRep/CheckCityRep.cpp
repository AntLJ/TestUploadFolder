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

// CheckCityRep.cpp: CheckCityRep クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckCityRep.h"
#include <WinLib/VersionInfo.h>
#include "GlobalFunc.h"

double	gXFactor	= 0.125 / 1280.0;
double	gYFactor	= 5.0 / 60.0 / 960.0;

// 使用法
void CheckCityRep::Usage()
{
	cerr << "[地番注記チェックツール使用法]\n"
		 << "(Usage)CheckCityRep.exe <各種オプション>\n"
		 << "■オプションについて\n"
		 << " ◇チェックモードオプション(何れか必須指定)\n"
		 << "   -a                        ... 全国チェック\n"
		 << "   -l <11桁住所コードリスト> ... リスト指定チェック\n" 
		 << "   -r <開始市区町村コード>-<終了市区町村コード> ... コード範囲指定チェック\n" 
		 << " ◇地番注記検索範囲指定オプション(10進度単位)\n"
		 << "   -s <検索範囲> ... 指定範囲の半径を持つ円内を検索(デフォルトは、0.002[約200m])\n"
		 << " ◇その他オプション\n"
		 << "   -o <ログファイル> ... ログファイル指定(必須)\n"
		 << "■環境変数について(どれも必須指定)\n"
		 << " DB_ADDRESS   ... 住所系SDE接続プロパティ\n"
		 << " DB_MAP       ... 地図系SDE接続プロパティ\n"
		 << " FC_CITY_REP_POINT ... 地番代表点フィーチャクラス名\n" 
		 << " FC_CITY_ADMIN     ... 都市地図行政界フィーチャクラス\n"
		 << " FC_GOU_POINT      ... 号ポイントフィーチャクラス\n"
		 << " FC_CITY_SITE      ... 都市地図背景フィーチャクラス" << endl;
}

// 初期化
bool CheckCityRep::Init( int argc, char* argv[] )
{
	if( argc < 3 )
	{
		cerr << "#Error 引数が足りません" << endl;
		return false;
	}

	for( int i = 1; i < argc && argv[i][0] == '-'; i++ )
	{
		switch(argv[i][1])
		{
		case 'a':
			m_eCheckMode = kCheckAll;
			break;
		case 'l':
			m_eCheckMode = kCheckByList;
			if( i+1 < argc )
				m_strAddrList = argv[++i];	// 住所コードリスト読み込み
			else
			{
				cerr << "#Error -lオプション時の引数が足りません" << endl;
				return false;
			}
			break;
		case 'r':
			m_eCheckMode = kCheckByRange;
			if( i+1 < argc )
			{
				CString strRange(argv[++i]);
				if( strRange.GetLength() != 11 || strRange[5] != '-' )
				{
					cerr << "#Error -rオプション時の引数が不正 : " << strRange << endl;
					return false;
				}
				m_strRangeMin = strRange.Mid(0, 5);
				m_strRangeMax = strRange.Mid(6, 5);
			}
			else
			{
				cerr << "#Error -rオプション時の引数が足りません" << endl;
				return false;
			}
			break;
		case 's':
			if( i+1 < argc )
			{
				m_dSearchRange = _tstof(argv[++i]);
				if( m_dSearchRange <= 0.0 )
				{
					cerr << "#Error 地番注記検索範囲は、0.0より大きい値を指定して下さい : " << argv[i] << endl;
					return false;
				}
			}
			else
			{
				cerr << "#Error -sオプション時の引数が足りません" << endl;
				return false;
			}
			break;
		case 'o':
			if( i+1 < argc )
			{
				m_ofs.open( static_cast<CT2CW>(argv[++i]) );
				if( !m_ofs )
				{
					cerr << "#Error ログファイルのオープンに失敗 : " << argv[i] << endl;
					return false;
				}
			}
			else
			{
				cerr << "#Error -oオプション時の引数が足りません" << endl;
				return false;
			}
			break;
		default:
			cerr << "#Error 不正なオプションです : " << argv[i] << endl;
			return false;
		}
	}

	if( !m_ofs.is_open() )
	{
		cerr << "#Error ログファイル(-o オプション)が指定されていません" << endl;
		return false;
	}

	if( m_eCheckMode == kCheckNone )
	{
		cerr << "#Errro チェックモードが指定されていない" << endl;
		return false;
	}

	// 環境変数からDB情報取得
#ifdef _DEBUG
	_tputenv( _T("DB_ADDRESS=SiNDYTEST/SiNDYTEST/CheckCityRep/5151/coral2") );
	_tputenv( _T("DB_MAP=SiNDYTEST/SiNDYTEST/CheckCityRep/5151/coral2") );
	_tputenv( _T("FC_CITY_REP_POINT=City_Rep_Point") );
	_tputenv( _T("FC_CITY_ADMIN=City_Admin") );
	_tputenv( _T("FC_GOU_POINT=Gou_Point") );
	_tputenv( _T("FC_CITY_SITE=City_Site") );
#endif

	CString strDBAddress, strDBMap;
	if( !strDBAddress.GetEnvironmentVariable(_T("DB_ADDRESS")) )
	{
		cerr << "#Error 環境変数 DB_ADDRESS が未設定" << endl;
		return false;
	}
	if( !strDBMap.GetEnvironmentVariable(_T("DB_MAP")) )
	{
		cerr << "#Error 環境変数 DB_MAP が未設定" << endl;
		return false;
	}

	if( NULL == (m_ipWorkspaceAddress = gf::GetWorkspace(strDBAddress)) )
	{
		cerr << "#Error 住所系SDEへの接続に失敗 : " << strDBAddress << endl;
		return false;
	}
	cout << "#住所系SDEへの接続 : " << strDBAddress << endl;

	if( NULL == (m_ipWorkspaceMap = gf::GetWorkspace(strDBMap)) )
	{
		cerr << "#Error 地図系SDEへの接続に失敗 : " << strDBMap << endl;
		return false;
	}
	cout << "#地図系SDEへの接続 : " << strDBMap << endl;

	// フィーチャクラス取得
	if( !gf::GetFCFromEnv(_T("FC_CITY_REP_POINT"), m_ipWorkspaceAddress, m_ipFC_CityRepPoint)
		|| !gf::GetFCFromEnv(_T("FC_CITY_ADMIN"), m_ipWorkspaceAddress, m_ipFC_CityAdmin) 
		|| !gf::GetFCFromEnv(_T("FC_GOU_POINT"), m_ipWorkspaceAddress, m_ipFC_GouPoint)
		|| !gf::GetFCFromEnv(_T("FC_CITY_SITE"), m_ipWorkspaceMap, m_ipFC_CitySite) )
		return false;

	// フィールドインデックス取得
	if( FAILED(m_ipFC_CityRepPoint->FindField(_bstr_t(city_rep_point::kAnno), &mFID_AnnoF))
		|| FAILED(m_ipFC_CityRepPoint->FindField(_bstr_t(city_rep_point::kPointClass), &mFID_PointClassC))
		|| FAILED(m_ipFC_CityRepPoint->FindField(_bstr_t(city_rep_point::kAddrCode), &mFID_AddrCode))
		|| FAILED(m_ipFC_CityRepPoint->FindField(_bstr_t(city_rep_point::kRepType), &mFID_RepTypeC))
		|| FAILED(m_ipFC_CityRepPoint->FindField(_bstr_t(city_rep_point::kChibanNo), &mFID_ChibanNo))
		|| FAILED(m_ipFC_GouPoint->FindField(_bstr_t(gou_point::kGouType), &mFID_GouType))
		|| FAILED(m_ipFC_CitySite->FindField(_bstr_t(city_site::kBgClass), &mFID_BgClassC)) )
	{
		cerr << "#Error フィールドインデックスの取得に失敗" << endl;
		return false;
	}

	// ログヘッダー出力
	CVersion cVer;
	CString strHeader;
	strHeader.Format( _T("#SINDYSTDLOG\n")
					  _T("#%s FILEVERSION %s PRODUCTVERSION %s\n")
					  _T("#DB_ADDRESS: %s\n")
					  _T("#DB_MAP: %s"),
					  cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion(), strDBAddress, strDBMap );
	m_ofs << strHeader << endl;

	return true;
}

// 実行
bool CheckCityRep::Execute()
{
	m_ofs << "#開始時刻: " << gf::GetCurrentTime() << endl;

	// 住所コードリスト作成
	if( !fnMakeACList() )
		return false;

	((IGeoDatasetPtr)m_ipFC_CityRepPoint)->get_SpatialReference(&m_ipSpRef);

	// 住所コード（11桁）のループ	
	for( AddrCodeList::iterator	it = mACList.begin(); it != mACList.end(); ++it )
	{
		// ８桁単位で空間キャッシュを作成していたが、ISpatialCache脱却のため、空間キャッシュ無しで実行するよう変更（Bug9843）
		cout << "#" << *it << " チェック中..." << endl;

		IGeometryPtr ipG_CityAdmin( fnUniteCityAdmin(*it) );
		if( ! ipG_CityAdmin )
			continue;

		IFeatureCursorPtr ipFCur_CityRepPoint( gf::SelectByShape(ipG_CityAdmin, m_ipFC_CityRepPoint, NULL, esriSpatialRelContains) );
		if( ! ipFCur_CityRepPoint )
			continue;

		// CityRepPoint のループ（ターゲット）
		IFeaturePtr	ipF_CityRepPoint;
		while(ipFCur_CityRepPoint->NextFeature(&ipF_CityRepPoint) == S_OK)
		{
			CComVariant	aVarTmp;
			
			// 地番代表点以外は無視
			ipF_CityRepPoint->get_Value(mFID_PointClassC, &aVarTmp);
			if(aVarTmp.lVal != 0) continue;

			// 地番注記フラグが立っているものは無視
			ipF_CityRepPoint->get_Value(mFID_AnnoF, &aVarTmp);
			if(aVarTmp.lVal == 1) continue;

			// 住所コード取得
			ipF_CityRepPoint->get_Value(mFID_AddrCode, &aVarTmp);
			CString strAddrCode = (aVarTmp.vt == VT_BSTR)? CString(aVarTmp.bstrVal) : NULL;
			aVarTmp.Clear();

			IGeometryPtr	ipG_CityRepPoint, ipG_Buffer;
			ipF_CityRepPoint->get_Shape(&ipG_CityRepPoint);
			((ITopologicalOperatorPtr)ipG_CityRepPoint)->Buffer(m_dSearchRange, &ipG_Buffer);

			// CityRepPoint のループ（周辺） … 地番注記フラグの立っているポイントがあればOK
			bool aAnnoExist = false;
			IFeatureCursorPtr ipFCur_CityRepPoint2( gf::SelectByShape(ipG_Buffer, m_ipFC_CityRepPoint, NULL, esriSpatialRelIntersects) );
			IFeaturePtr	ipF_CityRepPoint2;
			while( ipFCur_CityRepPoint2->NextFeature(&ipF_CityRepPoint2) == S_OK )
			{
				ipF_CityRepPoint2->get_Value(mFID_AnnoF, &aVarTmp);
				if(aVarTmp.lVal == 1) {
					aAnnoExist = true;
					break;
				}
			}

			// 周辺に地番注記ないとき
			if( !aAnnoExist )
			{
				// 代表点・地番止まりフラグが「エリア」の場合		→ エラー
				// 代表点・地番止まりフラグが「エリア」でない場合	→ 号ポイント（GOU）由来ならエラー
				ipF_CityRepPoint->get_Value(mFID_RepTypeC, &aVarTmp);
				if( aVarTmp.lVal > 1 )
				{
					long lGouCount = 0;
					IFeatureCursorPtr	ipFCur_GouPoint = gf::SelectByShape( ipG_CityRepPoint, m_ipFC_GouPoint, NULL, esriSpatialRelIntersects );
					if( ipFCur_GouPoint )
					{
						IFeaturePtr	ipF_GouPoint;
						while( ipFCur_GouPoint->NextFeature(&ipF_GouPoint) == S_OK )
						{
							ipF_GouPoint->get_Value(mFID_GouType, &aVarTmp);
							if( aVarTmp.lVal == 1 )
								++lGouCount;
						}
					}

					if( lGouCount == 0 )
						continue;
				}

				// 背景をチェック①（注記発生対象でない背景上の場合は問題なし）
				long aCSCount = 0;
				IFeatureCursorPtr	ipFCur_CitySite = gf::SelectByShape( ipG_CityRepPoint, m_ipFC_CitySite, &aCSCount, esriSpatialRelWithin );
				if(aCSCount < 1) continue;	// 本当はエラー（細街路上にあってはならない）

				IFeaturePtr	ipF_CitySite;
				if(ipFCur_CitySite->NextFeature(&ipF_CitySite) == S_OK)
				{
					ipF_CitySite->get_Value(mFID_BgClassC, &aVarTmp);
					switch(aVarTmp.lVal)
					{
					case city_site::bg_class::kBlock:              // 街区
					case city_site::bg_class::kForest:             // 山林
					case city_site::bg_class::kLawnOther:          // 芝生等その他緑地
					case city_site::bg_class::kPark:               // 植栽地
					case city_site::bg_class::kCenterDividerPark:  // 中央分離帯（植栽地）
					case city_site::bg_class::kHospital:           // 病院
					case city_site::bg_class::kSchool:             // 学校
					case city_site::bg_class::kPlant:              // 工場
					case city_site::bg_class::kOtherSite:          // 舗装地その他敷地
						break;
					default:
						continue;
					}
				}

				// 背景をチェック②（注記矩形が道路にかかっていたら問題なし）
				IEnvelopePtr	ipE_AnnoRect(CLSID_Envelope);
				ipF_CityRepPoint->get_Value(mFID_ChibanNo, &aVarTmp);
				CString strChiban;
				strChiban.Format( _T("%ld"), aVarTmp.lVal );
				double	aX = 0.0, aY = 0.0;
				((IPointPtr)ipG_CityRepPoint)->QueryCoords(&aX, &aY);
				ipE_AnnoRect->PutCoords(aX - gXFactor * strChiban.GetLength() / 2.0, aY - gYFactor / 2.0, aX + gXFactor * strChiban.GetLength() / 2.0, aY + gYFactor / 2.0);
				ipE_AnnoRect->putref_SpatialReference(m_ipSpRef);
				ipE_AnnoRect->SnapToSpatialReference();
				ipFCur_CitySite = gf::SelectByShape((IGeometryPtr)ipE_AnnoRect, m_ipFC_CitySite, &aCSCount, esriSpatialRelIntersects );
				if(aCSCount < 1) continue;	// 本当はエラー（細街路上にあってはならない）

				bool	aInvalidBG = false;
				while(ipFCur_CitySite->NextFeature(&ipF_CitySite) == S_OK)
				{
					using namespace city_site::bg_class;
					ipF_CitySite->get_Value(mFID_BgClassC, &aVarTmp);
					switch(aVarTmp.lVal) {
					case kInterCityTollExpressway:  // 都市間高速道（有料）
					case kUrbanTollExpressway:      // 都市高速（有料）
					case kTollRoad:                 // 有料道路
					case kNationalHighway:          // 国道
					case kPrincipalPrefecturalRoad: // 主要地方道
					case kPrefectureNormalRoad:     // 一般都道府県道
					case kOtherArterialRoad:        // その他幹線道路
					case kInterCityFreeExpressway:  // 都市間高速道（無料）
					case kUrbanFreeExpressway:      // 都市高速（無料）
					case kUnderRepairRoad:          // 工事中道路
					case kTollGateLane:             // 料金所レーン
						aInvalidBG	= true;
						break;
					}
				}
				if(aInvalidBG) continue;

				// ここまで来たらエラー出力
				long lOID = 0;
				ipF_CityRepPoint->get_OID( &lOID );

				CString strLog;
				strLog.Format( _T("0\t%s\t%ld\t\t\tERROR\t\t地番注記フラグが立っていない\t%s"),
							   gf::GetEnvValue(_T("FC_CITY_REP_POINT")), lOID, strAddrCode );
				m_ofs << strLog << endl;
			}
		}

	}

	m_ofs << "#終了時刻: " << gf::GetCurrentTime() << endl;

	return true;

}

// 行政界コードリスト作成
bool CheckCityRep::fnMakeACList()
{
	cout << "#住所コードリスト作成中...";

	CString strWhere, strSubField;
	strWhere.Format( _T("%s > '00000' and %s = 1"), city_admin::kCityCode, city_admin::kAreaClass );
	strSubField.Format( _T("distinct(%s || %s)"), city_admin::kCityCode, city_admin::kAddrCode );

	switch(m_eCheckMode)
	{
	case kCheckByRange:
		strWhere.AppendFormat( _T(" and %s between '%s' and '%s'"), city_admin::kCityCode, m_strRangeMin, m_strRangeMax );
	case kCheckAll:
		{
			IQueryDefPtr	ipQDef_CityAdmin;
			((IFeatureWorkspacePtr)m_ipWorkspaceAddress)->CreateQueryDef(&ipQDef_CityAdmin);
			ipQDef_CityAdmin->put_Tables(CComBSTR(gf::GetEnvValue(_T("FC_CITY_ADMIN"))));
			ipQDef_CityAdmin->put_SubFields(CComBSTR(strSubField));
			ipQDef_CityAdmin->put_WhereClause(CComBSTR(strWhere));

			_ICursorPtr ipCursor;
			ipQDef_CityAdmin->Evaluate(&ipCursor);
			_IRowPtr ipRow;
			while( ipCursor->NextRow(&ipRow) == S_OK )
			{
				CComVariant vaValue;
				ipRow->get_Value(0, &vaValue);
				CString strAddrCode = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
				mACList.insert(strAddrCode);
			}
		}
		break;
	case kCheckByList:
		{
			ifstream ifs( static_cast<CT2CW>(m_strAddrList) );
			if( !ifs )
			{
				cout << "市区町村コードリストの読み込みに失敗 : " << m_strAddrList << endl;
				return false;
			}

			while( !ifs.eof() )
			{
				string str;
				getline( ifs, str );
				if( !str.empty() )
					mACList.insert( str.c_str() );
			}
		}
		break;
	default:
		break;
	}

	cout << "完了" << endl;

	return true;
}

// 同一11桁コードの行政界をひとまとめにする
IGeometryPtr CheckCityRep::fnUniteCityAdmin( const CString& strAddrCode )
{
	IGeometryPtr ipG_Return(CLSID_Polygon);
	ipG_Return->putref_SpatialReference( m_ipSpRef );

	CString strWhere;
	strWhere.Format( _T("%s = '%s' and %s = '%s' and %s = 1"), 
					 city_admin::kCityCode, strAddrCode.Left(5), city_admin::kAddrCode, strAddrCode.Mid(5, 6), city_admin::kAreaClass );

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( _bstr_t(strWhere) );

	IFeatureCursorPtr	ipFeatureCursor;
	m_ipFC_CityAdmin->Search( ipQuery, VARIANT_FALSE, &ipFeatureCursor );
	if( ipFeatureCursor )
	{
		IFeaturePtr	ipFeature;
		while(ipFeatureCursor->NextFeature(&ipFeature) == S_OK)
		{
			IGeometryPtr	ipG_CityAdmin, ipG_Tmp;
			ipFeature->get_Shape(&ipG_CityAdmin);
			((ITopologicalOperatorPtr)ipG_Return)->Union(ipG_CityAdmin, &ipG_Tmp);
			ipG_Return = ipG_Tmp;
		}

		return ipG_Return;
	}
	else
		return NULL;
}

