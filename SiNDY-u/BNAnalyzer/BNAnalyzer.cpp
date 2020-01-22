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

// BNAnalyzer.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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
	// ���́FXY�t�^�σf�[�^�A�ƌ`�|���S���e�[�u��(SiNDY)�A�Z���R�[�h�A�ƌ`ID�e�[�u��(PostgreSQL)
	// �o�́F���͌��ʂ��e�L�X�g�t�@�C���ŏo��
	std::cerr << "�h�Ύ{�݃f�[�^�E�s���Y�f�[�^�̖��́E�������̓c�[��" << std::endl;
	std::cerr << "Usage : BuildingNamePointAnalyzer.exe [Option] <XY�t�^�f�[�^>" << std::endl;
	std::cerr << "Option : -I [�ݒ�t�@�C��] �� �w�肵�Ȃ��ꍇ�A�J�����g��BuildingNamePointAnalyzer.ini��ݒ肷��" << std::endl;
	std::cerr << "         -P [TKY2JGD.par�̎w��] �� ���̃I�v�V������t�����ꍇ�ɂ́A���͍��W�����E���n�n�ł���A���{���n�n�ɕϊ����ĕ]������" << std::endl;
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
	// DB�ڑ���̎擾
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
	// DB�ڑ�
	IWorkspacePtr ipWorkspace;
	IFeatureClassPtr ipFeatureClass;
	CComBSTR bstrShapeField;
	ISpatialReferencePtr ipSpatialReference;

	std::cerr << "�f�[�^�x�[�X�ɐڑ���...";
	{
		IWorkspaceFactory2Ptr ipWorkspaceFactory(CLSID_SdeWorkspaceFactory);
		if (FAILED(ipWorkspaceFactory->OpenFromString(CComBSTR(gSdeConnection), NULL, &ipWorkspace))) {
			std::cerr << CT2CA(gSdeConnection) << "�ɐڑ��ł��܂���" << std::endl;
			return false;
		}

		if (FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(_T("BUILDING")), &ipFeatureClass))) {
			std::cerr << "Building�e�[�u�����I�[�v���ł��܂���" << std::endl;
			return false;
		}

	
		ipFeatureClass->get_ShapeFieldName(&bstrShapeField);

	
		((IGeoDatasetPtr)ipFeatureClass)->get_SpatialReference(&ipSpatialReference);
	}

	std::string c = CT2CA(gPgConnection);
	ns::PgDatabase addrDatabase(c.c_str());

	if (! addrDatabase.connected()) {
		std::cerr << c.c_str() << "�ɐڑ��ł��܂���" << std::endl;
		return false;
	}
	std::cerr << std::endl;

	std::cerr << CT2CA(gXYText) << "�̓ǂݎ�蒆...";
	BNFile file(gXYText);
	if (! file.is_open()) {
		std::cerr << CT2CA(gXYText) << "��ǂݎ��܂���" << std::endl;
		return false;
	}
	std::cerr << std::endl;

	std::cout << "��������,�Z��,�Z���R�[�h,�ܓx,�o�x,�Z�����x,PP����,�ƌ`����,�ƌ`ID,����ƌ`�������Z��,����Z����������,����Z�����R�[�h��" << std::endl;
	CStringArray data;
	int cur = 0;
	while (file.read(data)) {
		cur++;
		std::cerr << CT2CA(gXYText) << "�̉�͒�..(" << cur << ")\r";
		if (cur <= SKIPLINE) continue;

		// �K�v�ȏ��̎擾
		double lon = _tcstod(data[BN::Longitude], 0);		// �o�x
		double lat = _tcstod(data[BN::Latitude], 0);		// �ܓx
		if (gJGDtoTokyo) {
			double x = lon, y = lat;
			gCrdcnv.JGD2000toTOKYO_RP(x, y, &lon, &lat, true);
		}
		CString bldname = data[BN::Name];			// ������
		CString addrname = data[BN::Addr];				// �Z����
		CString addrcode;									// �Z���R�[�h
		addrcode.Format(_T("%s%s%s%s%s%s"), data[BN::PrefCode], data[BN::CityCode], data[BN::OazaCode], data[BN::ChomeCode], data[BN::ChibanCode], data[BN::GouCode]);
		int addrprecision = _ttoi(data[BN::Precision]);		// �Z�����x

		// �s���|�C���g���� -- �n�Ԏ~�܂�t���O���Ȃ��A��������2�A3�̏ꍇ�̓s���|�C���g
		int ppjudge = 0;
		int a = _ttoi(data[BN::ChibanFlag]);
		int b = _ttoi(data[BN::Precision]);
		ppjudge = (b == 5 || a == 2 || a == 4) ? 1 : 0; // ���x5�܂��́A��\�_�E�n�Ԏ~�܂�t���O��2��4�̂��̂��s���|�C���g

		// �ƌ`���� -- 0 �F�ƌ`�O�A1:�ƌ`���A2�F�ƌ`����������(�ƌ`�̋��E��ɂ���ƍl������)
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
				std::cerr << "�ƌ`�`��̌����Ɏ��s���܂���" << std::endl;
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

		// �ƌ`ID -- �ŏ��Ɍ����������̂��o��
		int kakeiid = OIDs.size() ? OIDs[0] : -1;

		// ����ƌ`�������Z��
		int plural_points = 0;
		if (kakeiid != -1) {
			CString s;
			s.Format(_T("SELECT point_count FROM building_point WHERE building_id = %d;"), kakeiid);
			ns::RowsPtr rows = addrDatabase.execute(CT2CA(s));
			if (rows == NULL) {
				std::cerr << "�ƌ`ID�̌����Ɏ��s" << std::endl;
				return false;
			}
			ns::RowPtr row = rows->next();
			plural_points = (row != NULL && row->getInt(0) > 1) ? 1 : 0; // 0 : ����ƌ`�������Z���Ȃ�, 1 : ����ƌ`�������Z������
		} else {
			plural_points = -1;
		}

		// ����Z����������
		int plural_addr = 0;
		int r = file.get_same_code_count(addrcode);
		plural_addr = (r == 1) ? 0 : (r >= 2) ? 1 : -1;

		// ����Z�����R�[�h��
		int plural_rec = -1;
		{
			CString s;
			s.Format(_T("SELECT count FROM addr_point WHERE addrcode = \'%s\';"), addrcode);
			ns::RowsPtr rows = addrDatabase.execute(CT2CA(s));
			if (rows == NULL) {
				std::cerr << "�Z���R�[�h�����Ɏ��s" << std::endl;
				return false;
			}
			ns::RowPtr row = rows->next();
			plural_rec = (row != NULL) ? row->getInt(0) : -1;
		}

		// �o��
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
	case 0: std::cerr << "����ɏI�����܂���" << std::endl; break;
	case 1: std::cerr << "�ُ�I�����܂���" << std::endl; break;
	case 2: std::cerr << "��O���������܂���" << std::endl; break;
	default: break;
	}
	clock_t end = clock();
	std::cerr.setf(std::ios_base::fixed, std::ios_base::floatfield); std::cout.precision(3);
	std::cerr << (double)(end - start)/CLOCKS_PER_SEC << " �b������܂���" << std::endl;
	return retval;
}

