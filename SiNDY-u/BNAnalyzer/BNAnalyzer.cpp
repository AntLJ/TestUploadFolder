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

// BNAnalyzer.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <time.h>
#include "BNFile.h"

static CString		gInifile = _T(".\\BNAnalyzer.ini");
static CString		gXYText = _T("");
static CString		gSdeConnection = _T("");
static CString		gPgConnection = _T("");

static crd_cnv		gCrdcnv;
static bool			gJGDtoTokyo = false;

bool usage()
{
	// 入力：XY付与済データ、家形ポリゴンテーブル(SiNDY)、住所コード、家形IDテーブル(PostgreSQL)
	// 出力：分析結果をテキストファイルで出力
	std::cerr << "防火施設データ・不動産データの名称・建物分析ツール" << std::endl;
	std::cerr << "Usage : BuildingNamePointAnalyzer.exe [Option] <XY付与データ>" << std::endl;
	std::cerr << "Option : -I [設定ファイル] → 指定しない場合、カレントのBuildingNamePointAnalyzer.iniを設定する" << std::endl;
	std::cerr << "         -P [TKY2JGD.parの指定] → このオプションを付けた場合には、入力座標が世界測地系であり、日本測地系に変換して評価する" << std::endl;
	return false;
}

bool arg_check(int argc, _TCHAR **argv)
{
	if (argc <= 1)
		return usage();
	while (--argc) {
		_TCHAR *s = *++argv;
		switch (*s) {
		case _T('-'):
			switch (*++s) {
			case _T('I'):
				gInifile = *++argv; --argc; break;
			case _T('P'):
				{
					CString param = *++argv; --argc;
					std::string p = CT2CA(param);
					gCrdcnv.JGD2000_RP_Init((char *)p.c_str());
					gJGDtoTokyo = true;
				}
				break;
			default:
				std::cerr << "Invalid option." << std::endl;
				return usage();
			}
			break;
		default:
			gXYText = s;
			break;
		}
	}
	// DB接続先の取得
	_TCHAR sde_server[256], sde_instance[256], sde_version[256], sde_user[256], sde_password[256];
	::GetPrivateProfileString(_T("SDE"), _T("SERVER"), _T("error"), sde_server, sizeof(sde_server), gInifile);
	::GetPrivateProfileString(_T("SDE"), _T("INSTANCE"), _T("error"), sde_instance, sizeof(sde_instance), gInifile);
	::GetPrivateProfileString(_T("SDE"), _T("VERSION"), _T("error"), sde_version, sizeof(sde_version), gInifile);
	::GetPrivateProfileString(_T("SDE"), _T("USER"), _T("error"), sde_user, sizeof(sde_user), gInifile);
	::GetPrivateProfileString(_T("SDE"), _T("PASSWORD"), _T("error"), sde_password, sizeof(sde_password), gInifile);

	gSdeConnection.Format(_T("server=%s;instance=%s;version=%s;user=%s;password=%s"), sde_server, sde_instance, sde_version, sde_user, sde_password);

	_TCHAR pg_host[256], pg_port[256], pg_database[256], pg_user[256], pg_password[256];
	::GetPrivateProfileString(_T("POSTGRESQL"), _T("HOST"), _T("error"), pg_host, sizeof(pg_host), gInifile);
	::GetPrivateProfileString(_T("POSTGRESQL"), _T("PORT"), _T("error"), pg_port, sizeof(pg_port), gInifile);
	::GetPrivateProfileString(_T("POSTGRESQL"), _T("DATABASE"), _T("error"), pg_database, sizeof(pg_database), gInifile);
	::GetPrivateProfileString(_T("POSTGRESQL"), _T("USER"), _T("error"), pg_user, sizeof(pg_user), gInifile);
	::GetPrivateProfileString(_T("POSTGRESQL"), _T("PASSWORD"), _T("error"), pg_password, sizeof(pg_password), gInifile);

	gPgConnection.Format(_T("host=%s port=%s dbname=%s user=%s password=%s"), pg_host, pg_port, pg_database, pg_user, pg_password);

	return true;
}

bool run()
{
	// DB接続
	IWorkspacePtr ipWorkspace;
	IFeatureClassPtr ipFeatureClass;
	CComBSTR bstrShapeField;
	ISpatialReferencePtr ipSpatialReference;

	std::cerr << "データベースに接続中...";
	{
		IWorkspaceFactory2Ptr ipWorkspaceFactory(CLSID_SdeWorkspaceFactory);
		if (FAILED(ipWorkspaceFactory->OpenFromString(CComBSTR(gSdeConnection), NULL, &ipWorkspace))) {
			std::cerr << CT2CA(gSdeConnection) << "に接続できません" << std::endl;
			return false;
		}

		if (FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(_T("BUILDING")), &ipFeatureClass))) {
			std::cerr << "Buildingテーブルをオープンできません" << std::endl;
			return false;
		}

	
		ipFeatureClass->get_ShapeFieldName(&bstrShapeField);

	
		((IGeoDatasetPtr)ipFeatureClass)->get_SpatialReference(&ipSpatialReference);
	}

	std::string c = CT2CA(gPgConnection);
	ns::PgDatabase addrDatabase(c.c_str());

	if (! addrDatabase.connected()) {
		std::cerr << c.c_str() << "に接続できません" << std::endl;
		return false;
	}
	std::cerr << std::endl;

	std::cerr << CT2CA(gXYText) << "の読み取り中...";
	BNFile file(gXYText);
	if (! file.is_open()) {
		std::cerr << CT2CA(gXYText) << "を読み取れません" << std::endl;
		return false;
	}
	std::cerr << std::endl;

	std::cout << "建物名称,住所,住所コード,緯度,経度,住所精度,PP判定,家形判定,家形ID,同一家形内複数住所,同一住所複数存在,同一住所レコード数" << std::endl;
	CStringArray data;
	int cur = 0;
	while (file.read(data)) {
		cur++;
		std::cerr << CT2CA(gXYText) << "の解析中..(" << cur << ")\r";
		if (cur <= SKIPLINE) continue;

		// 必要な情報の取得
		double lon = _tcstod(data[BN::Longitude], 0);		// 経度
		double lat = _tcstod(data[BN::Latitude], 0);		// 緯度
		if (gJGDtoTokyo) {
			double x = lon, y = lat;
			gCrdcnv.JGD2000toTOKYO_RP(x, y, &lon, &lat, true);
		}
		CString bldname = data[BN::Name];			// 建物名
		CString addrname = data[BN::Addr];				// 住所名
		CString addrcode;									// 住所コード
		addrcode.Format(_T("%s%s%s%s%s%s"), data[BN::PrefCode], data[BN::CityCode], data[BN::OazaCode], data[BN::ChomeCode], data[BN::ChibanCode], data[BN::GouCode]);
		int addrprecision = _ttoi(data[BN::Precision]);		// 住所精度

		// ピンポイント判定 -- 地番止まりフラグがない、もしくは2、3の場合はピンポイント
		int ppjudge = 0;
		int a = _ttoi(data[BN::ChibanFlag]);
		int b = _ttoi(data[BN::Precision]);
		ppjudge = (b == 5 || a == 2 || a == 4) ? 1 : 0; // 精度5または、代表点・地番止まりフラグが2か4のものがピンポイント

		// 家形判定 -- 0 ：家形外、1:家形内、2：家形内だが複数(家形の境界上にあると考えられる)
		int kakeijudge = 0;
		std::vector<long> OIDs;
		{
			ISpatialFilterPtr ipSpatialFilter(CLSID_SpatialFilter);
			ipSpatialFilter->put_GeometryField(bstrShapeField);
			ipSpatialFilter->put_SpatialRel(esriSpatialRelIntersects);
			IPointPtr ipPoint(CLSID_Point);
			ipPoint->put_X(lon);
			ipPoint->put_Y(lat);
			ipPoint->putref_SpatialReference(ipSpatialReference);
			ipSpatialFilter->putref_Geometry(ipPoint);
			IFeatureCursorPtr ipFeatureCursor;
			if (FAILED(ipFeatureClass->Search(ipSpatialFilter, VARIANT_FALSE, &ipFeatureCursor))) {
				std::cerr << "家形形状の検索に失敗しました" << std::endl;
				return false;
			}
			IFeaturePtr ipFeature;
			while (ipFeatureCursor->NextFeature(&ipFeature) == S_OK) {
				long OID;
				ipFeature->get_OID(&OID);
				OIDs.push_back(OID);
			}
			kakeijudge = (OIDs.size() == 1) ? 1 : (OIDs.size() > 1) ? 2 : 0;
		}

		// 家形ID -- 最初に見つかったものを出力
		int kakeiid = OIDs.size() ? OIDs[0] : -1;

		// 同一家形内複数住所
		int plural_points = 0;
		if (kakeiid != -1) {
			CString s;
			s.Format(_T("SELECT point_count FROM building_point WHERE building_id = %d;"), kakeiid);
			ns::RowsPtr rows = addrDatabase.execute(CT2CA(s));
			if (rows == NULL) {
				std::cerr << "家形IDの検索に失敗" << std::endl;
				return false;
			}
			ns::RowPtr row = rows->next();
			plural_points = (row != NULL && row->getInt(0) > 1) ? 1 : 0; // 0 : 同一家形内複数住所なし, 1 : 同一家形内複数住所あり
		} else {
			plural_points = -1;
		}

		// 同一住所複数存在
		int plural_addr = 0;
		int r = file.get_same_code_count(addrcode);
		plural_addr = (r == 1) ? 0 : (r >= 2) ? 1 : -1;

		// 同一住所レコード数
		int plural_rec = -1;
		{
			CString s;
			s.Format(_T("SELECT count FROM addr_point WHERE addrcode = \'%s\';"), addrcode);
			ns::RowsPtr rows = addrDatabase.execute(CT2CA(s));
			if (rows == NULL) {
				std::cerr << "住所コード検索に失敗" << std::endl;
				return false;
			}
			ns::RowPtr row = rows->next();
			plural_rec = (row != NULL) ? row->getInt(0) : -1;
		}

		// 出力
		std::cout << CT2CA(bldname);
		std::cout << ",";
		std::cout << CT2CA(addrname);
		std::cout << ",";
		std::cout << CT2CA(addrcode);
		std::cout << ",";
		std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield); std::cout.precision(10);
		std::cout << lat;
		std::cout << ",";
		std::cout << lon;
		std::cout << ",";
		std::cout.clear();
		std::cout << addrprecision;
		std::cout << ",";
		std::cout << ppjudge;
		std::cout << ",";
		std::cout << kakeijudge;
		std::cout << ",";
		std::cout << kakeiid;
		std::cout << ",";
		std::cout << plural_points;
		std::cout << ",";
		std::cout << plural_addr;
		std::cout << ",";
		std::cout << plural_rec;
		std::cout << std::endl;
	}
	std::cerr << std::endl;

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;
	clock_t start = clock();
	try {
		arctl::coinitializer initializer;
		{
			if (! arg_check(argc, argv)) {
				retval = -1;
			} else if (! run()) {
				retval = 1;
			} else {
				retval = 0;
			}
		}
	} catch (...) {
		retval = 2;
	}
	switch (retval) {
	case 0: std::cerr << "正常に終了しました" << std::endl; break;
	case 1: std::cerr << "異常終了しました" << std::endl; break;
	case 2: std::cerr << "例外が発生しました" << std::endl; break;
	default: break;
	}
	clock_t end = clock();
	std::cerr.setf(std::ios_base::fixed, std::ios_base::floatfield); std::cout.precision(3);
	std::cerr << (double)(end - start)/CLOCKS_PER_SEC << " 秒かかりました" << std::endl;
	return retval;
}

