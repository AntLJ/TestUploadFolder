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

// CheckBaseRepExist.cpp: CheckBaseRepExist �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/tstring.h>
#include "CheckBaseRepExist.h"

using namespace std;
using namespace sindy::schema;

string	gMessage;

const char* gFieldStr = "\t�Z���R�[�h�Q";	// [Bug9463]�Œǉ�

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CheckBaseRepExist::CheckBaseRepExist():
	m_fIdGPGouNumber(-1), m_fIdCACityCode(-1), m_fIdCAAddrCode(-1),
	m_fIdCSAddrCode(-1), m_fIdBRAddrCode(-1),
	m_Argc(0), m_Argv(nullptr), m_fpStatList(nullptr)
{
	m_checkMode	= kNone;
}

CheckBaseRepExist::~CheckBaseRepExist()
{

}

bool CheckBaseRepExist::
fnGetEnvVar()
{
#ifdef _DEBUG
	//m_dbAddress = "FIX201511/FIX201511/SDE.DEFAULT/5151/plum";
	m_dbAddress = "\\\\win\\tdc\\ced-ref\\verification\\SiNDY-u\\map\\CheckBaseRepExist\\PGDB\\���ؗpFGDB.gdb";
	m_fcGouPoint = "GOU_POINT";
	m_fcCityAdmin = "CITY_ADMIN";
	m_fcBaseRep = "BASE_REP_POINT";
	m_fcCSAddrPoint = "CS_ADDR_POINT";
	return true;
#endif

	char	*aTmpStr;

	// TODO: ��ʂ�`�F�b�N���āA�ݒ肳��Ă��Ȃ����ϐ��͑S���܂Ƃ߂ďo�͂���悤�ɂ���
	aTmpStr	= getenv("DB_ADDRESS");
	if(!aTmpStr) {
		cout << "���ϐ� DB_ADDRESS ���ݒ肳��Ă��܂���" << endl;
		cerr << "���ϐ� DB_ADDRESS ���ݒ肳��Ă��܂���" << endl;
		return false;
	} else {
		m_dbAddress = aTmpStr;
	}

	aTmpStr	= getenv("FC_GOUPOINT");
	if(!aTmpStr) {
		cout << "���ϐ� FC_GOUPOINT ���ݒ肳��Ă��܂���" << endl;
		cerr << "���ϐ� FC_GOUPOINT ���ݒ肳��Ă��܂���" << endl;
		return false;
	} else {
		m_fcGouPoint = aTmpStr;
	}

	aTmpStr	= getenv("FC_CITYADMIN");
	if(!aTmpStr) {
		cout << "���ϐ� FC_CITYADMIN ���ݒ肳��Ă��܂���" << endl;
		cerr << "���ϐ� FC_CITYADMIN ���ݒ肳��Ă��܂���" << endl;
		return false;
	} else {
		m_fcCityAdmin = aTmpStr;
	}

	aTmpStr	= getenv("FC_BASEREP");
	if(!aTmpStr) {
		cout << "���ϐ� FC_BASEREP ���ݒ肳��Ă��܂���" << endl;
		cerr << "���ϐ� FC_BASEREP ���ݒ肳��Ă��܂���" << endl;
		return false;
	} else {
		m_fcBaseRep = aTmpStr;
	}

	aTmpStr	= getenv("FC_CSADDRPOINT");
	if(!aTmpStr) {
		cout << "���ϐ� FC_CSADDRPOINT ���ݒ肳��Ă��܂���" << endl;
		cerr << "���ϐ� FC_CSADDRPOINT ���ݒ肳��Ă��܂���" << endl;
		return false;
	} else {
		m_fcCSAddrPoint = aTmpStr;
	}

	return true;

}

bool CheckBaseRepExist::
fnAnalyzeArg()
{
	for(int i = 1; i < m_Argc && m_Argv[i][0] == '-'; i++) {

		switch(m_Argv[i][1]) {

		case 'c':
			switch(m_Argv[i][2]) {
			case 's':
				m_checkMode	= kCitySingle;
				i++;
				if(i < m_Argc) m_cityCode = m_Argv[i];
				else {
					cout << "����������܂���" << endl;
					cerr << "����������܂���" << endl;
					return false;
				}
				break;
			case 'l':
				m_checkMode	= kCityList;
				i++;
				if(i < m_Argc) m_fnCityList = m_Argv[i];
				else {
					cout << "����������܂���" << endl;
					cerr << "����������܂���" << endl;
					return false;
				}
				break;
			case 'a':
				m_checkMode	= kCityAll;
				break;
			default:
				cout << "�s���ȃI�v�V�����ł�: " << m_Argv[i] << endl;
				cerr << "�s���ȃI�v�V�����ł�: " << m_Argv[i] << endl;
				return false;
			}
			break;

		case 'o':
			i++;
			if(i < m_Argc) m_fnLogFile = m_Argv[i];
			else {
				cout << "����������܂���" << endl;
				cerr << "����������܂���" << endl;
				return false;
			}
			break;

		case 's':
			i++;
			if(i < m_Argc) m_fnStatList = m_Argv[i];
			else {
				cout << "����������܂���" << endl;
				cerr << "����������܂���" << endl;
				return false;
			}
			break;

		case '?':
			return false;
			break;

		default:
			cout << "�s���ȃI�v�V�����ł�: " << m_Argv[i] << endl;
			cerr << "�s���ȃI�v�V�����ł�: " << m_Argv[i] << endl;
			return false;

		}

	}

	if(m_checkMode == kNone) {
		cout << "�I�v�V���� -c* ���w�肳��Ă��܂���" << endl;
		cerr << "�I�v�V���� -c* ���w�肳��Ă��܂���" << endl;
		return false;
	}

	return true;

}

bool CheckBaseRepExist::
fnCreateCityList()
{

	ifstream	fin;

	switch(m_checkMode) {

	case kCitySingle:
		m_cityList.insert(m_cityCode);
		break;

	case kCityList:
		char		aBuf[100];
		fin.open(m_fnCityList.c_str());
		if(fin.fail()) {
			cout << "�t�@�C�� " << m_fnCityList << " ���I�[�v���ł��܂���" << endl;
			cerr << "�t�@�C�� " << m_fnCityList << " ���I�[�v���ł��܂���" << endl;
			return false;
		}
		while(!fin.eof()) {
			string	aCityCode;
			fin.getline(aBuf, 100);
			if(fin.fail()) {
				cout << "�t�@�C�� " << m_fnCityList << " �̓ǂݍ��݂ŃG���[���������܂���" << endl;
				cerr << "�t�@�C�� " << m_fnCityList << " �̓ǂݍ��݂ŃG���[���������܂���" << endl;
				return false;
			}
			aCityCode = aBuf;
			m_cityList.insert(aCityCode.substr(0, 5));
		}
		fin.close();
		break;

	case kCityAll:

		{
			_ICursorPtr	ipCur_CityAdmin;
			_IRowPtr		ipR_CityAdmin;
			ipCur_CityAdmin = SearchByWhereClause(
				uh::str_util::format(_T("%s > '00000'"), city_admin::kCityCode),
				m_fcCityAdmin.c_str(),
				uh::str_util::format(_T("distinct %s"), city_admin::kCityCode));
			while(ipCur_CityAdmin->NextRow(&ipR_CityAdmin) == S_OK) {
				string	aCityCode;
				CComVariant	aTmpVar;
				ipR_CityAdmin->get_Value(0, &aTmpVar);
				aCityCode = ExtractStr(aTmpVar);
				m_cityList.insert(aCityCode);
			}
		}
		break;

	default:
		return false;

	}

	return true;

}

// ���s�̏�����
bool CheckBaseRepExist::
Init( void )
{
	if(!fnGetEnvVar())	return false;
	if(!fnAnalyzeArg())	return false;

	if(m_fnLogFile.empty()) {
		cout << "���O�o�͐悪�w�肳��Ă��܂���" << endl;
		cerr << "���O�o�͐悪�w�肳��Ă��܂���" << endl;
		return false;
	}

	if(m_fnStatList.empty()) {
		cout << "�X�e�[�^�X���X�g�o�͐悪�w�肳��Ă��܂���" << endl;
		cerr << "�X�e�[�^�X���X�g�o�͐悪�w�肳��Ă��܂���" << endl;
		return false;
	}

	if(!Connect(m_dbAddress.c_str()))	return false;

	// �t�B�[�`���N���X�̎擾
	m_ipFcGouPoint		= GetFeatureClass(m_fcGouPoint.c_str());
	if(!m_ipFcGouPoint)	return false;
	m_ipFcCityAdmin		= GetFeatureClass(m_fcCityAdmin.c_str());
	if(!m_ipFcCityAdmin)	return false;
	m_ipFcBaseRep		= GetFeatureClass(m_fcBaseRep.c_str());
	if(!m_ipFcBaseRep)	return false;

	m_ipFcCSAddrPoint	= GetFeatureClass(m_fcCSAddrPoint.c_str());
	if(!m_ipFcCSAddrPoint)	return false;

	// �t�B�[���h�E�C���f�b�N�X�̎擾
	if(!GetFieldID(m_ipFcGouPoint,		gou_point::kGouNo,	&m_fIdGPGouNumber))		return false;
	if(!GetFieldID(m_ipFcCityAdmin,		city_admin::kCityCode,		&m_fIdCACityCode))		return false;
	if(!GetFieldID(m_ipFcCityAdmin,		city_admin::kAddrCode,		&m_fIdCAAddrCode))		return false;
	if(!GetFieldID(m_ipFcBaseRep,		base_rep_point::kAddrCode,		&m_fIdBRAddrCode))		return false;
	if(!GetFieldID(m_ipFcCSAddrPoint,	cs_addr_point::kAddrCode,		&m_fIdCSAddrCode))		return false;

	if(!m_jdbAccess.Connect()) {
		cout << m_jdbAccess.eErrMsg << endl;
		cerr << m_jdbAccess.eErrMsg << endl;
		return false;
	}

	// ���O�t�@�C���̃I�[�v��
	if(!m_errLog.Open(m_fnLogFile.c_str())) {
		cout << "���O�t�@�C�����J���܂���" << endl;
		cerr << "���O�t�@�C�����J���܂���" << endl;
		return false;
	}
	if(!m_errLog.SetProgramID(PROGRAM_ID)) {
		cout << "�v���O�����h�c��ݒ�ł��܂���" << endl;
		cerr << "�v���O�����h�c��ݒ�ł��܂���" << endl;
		return false;
	}
	if(!m_errLog.SetErrCodeTable(gECTable, ECT_MAX_RECORD)) {
		cout << "�G���[�R�[�h�e�[�u����ݒ�ł��܂���" << endl;
		cerr << "�G���[�R�[�h�e�[�u����ݒ�ł��܂���" << endl;
		return false;
	}

	// �X�e�[�^�X���X�g(�o��)�̃I�[�v��
	m_fpStatList = fopen(m_fnStatList.c_str(), "w");
	if(!m_fpStatList) {
		cout << "�X�e�[�^�X���X�g���J���܂���" << endl;
		cerr << "�X�e�[�^�X���X�g���J���܂���" << endl;
		return false;
	}

	// �c�[���������O�֏o��
	m_errLog.OutputComment("Base_Rep_Point ���݃`�F�b�N by H.Hyodo");

	// GeoDB�ڑ�����������O�֏o��
	gMessage = "DB_ADDRESS : " + m_dbAddress;
	m_errLog.OutputComment(gMessage.c_str());

	// �`�F�b�N�J�n���������O�֏o��
	m_errLog.OutputTime();
	m_errLog.Flush();

	return true;
}

// �厚�^���R�[�h���X�g�̍쐬
void CheckBaseRepExist::
MakeOazaAzaList( const string& crCityCode, AdminList& crAddrList )
{
	_ICursorPtr	ipCur_CityAdmin;
	ipCur_CityAdmin = SearchByWhereClause(
		uh::str_util::format(_T("%s = '%s' and %s <> '999999'"),
			city_admin::kCityCode, uh::toTStr(crCityCode).c_str(), city_admin::kAddrCode),
		m_fcCityAdmin.c_str(),
		uh::str_util::format(_T("distinct %s"), city_admin::kAddrCode));
	_IRowPtr		ipR_CityAdmin;
	while(ipCur_CityAdmin->NextRow(&ipR_CityAdmin) == S_OK) {
		adminfo	aAdmInfo;
		CComVariant	aAddr1Var;	// �Z���R�[�h�P
		ipR_CityAdmin->get_Value(0, &aAddr1Var);
		aAdmInfo.mCode6 = ExtractStr(aAddr1Var);
		crAddrList.insert(aAdmInfo);	// �Z���R�[�h1�̃��X�g
	}

	// [Bug9463]�Z���R�[�h�Q�����݂���ꍇ�i<>000000�j
	ipCur_CityAdmin = SearchByWhereClause(
		uh::str_util::format(_T("%s = '%s' and %s <> '000000'"),
			city_admin::kCityCode, uh::toTStr(crCityCode).c_str(), city_admin::kAddrCode2),
		m_fcCityAdmin.c_str(),
		uh::str_util::format(_T("distinct %s"), city_admin::kAddrCode2));
	while(ipCur_CityAdmin->NextRow(&ipR_CityAdmin) == S_OK) {
		adminfo	aAdmInfo2;
		CComVariant	aAddr2Var;	// �Z���R�[�h�Q
		ipR_CityAdmin->get_Value(0, &aAddr2Var);
		aAdmInfo2.mCode6 = ExtractStr(aAddr2Var);
		aAdmInfo2.mInfoType = adminfo::ADDR2;
		crAddrList.insert(aAdmInfo2);	// �Z���R�[�h2�̃��X�g
	}

	// �厚��\�R�[�h�̒ǉ�
	AdminList aOazaRepList;
	AdminList::iterator	aAddrItr;
	for(aAddrItr = crAddrList.begin(); aAddrItr != crAddrList.end(); ++aAddrItr) {
		if("100" < (*aAddrItr).mCode6.substr(3, 3) && (*aAddrItr).mCode6.substr(3, 3) <= "899") {	// [bug 8183](�����R�[�h)�Ή� 2010.08.27
			bool	aIsValid = false;
			adminfo	aOazaRepCode;
			aOazaRepCode.mInfoType = (*aAddrItr).mInfoType;
			aOazaRepCode.mCode6 = (*aAddrItr).mCode6.substr(0, 3) + "000";
			m_jdbAccess.IsValid((crCityCode + aOazaRepCode.mCode6).c_str(), &aIsValid);
			if(aIsValid) {
				aOazaRepList.insert(aOazaRepCode);
			}
		}
	}
	for(aAddrItr = aOazaRepList.begin(); aAddrItr != aOazaRepList.end(); aAddrItr++) {
		crAddrList.insert(*aAddrItr);
	}
}

// Base_Rep_Point �̃��X�g�쐬
void CheckBaseRepExist::
MakeBaseRepPointList( const string& cCityCode, AOList& cAOListBR )
{
	IFeatureCursorPtr	ipFCur_BaseRep;
	long	aBRCount = 0;
	ipFCur_BaseRep = SearchByWhereClause(
		uh::str_util::format(_T("%s like '%s%%'"), base_rep_point::kAddrCode, uh::toTStr(cCityCode).c_str()),
		m_ipFcBaseRep,
		&aBRCount);
	if(aBRCount > 0) {
		IFeaturePtr		ipF_BaseRep;
		while(ipFCur_BaseRep->NextFeature(&ipF_BaseRep) == S_OK) {
			AORec	aAORec;
			CComVariant	aTmpVar;
			ipF_BaseRep->get_Value(m_fIdBRAddrCode, &aTmpVar);
			aAORec.eAddrCode = ExtractStr(aTmpVar);
			ipF_BaseRep->get_OID(&aAORec.eOID);
			cAOListBR.insert(aAORec);
		}
	}
}

// 11���s���E�̓����Ɋ܂܂�鍆�̃J�E���g���擾
void CheckBaseRepExist::
GetContainedGouCount( const string& crCityCode,
						 const adminfo& crAdmInfo,
						 bool&	crOazaRepFlag,
						 long& crOID_CityAdmin,
						 long& crGPTotal )
{
	// ����11���|���S���̌���
	long	aCACount = 0;
	IFeatureCursorPtr	ipFCur_CityAdmin;
	// �Z���R�[�h�P�A�Q�̐؂�ւ�
	string	aAddrCodeField;
	if( crAdmInfo.mInfoType == adminfo::ADDR1 ) {
		aAddrCodeField	= city_admin::kAddrCode;
	}else {
		aAddrCodeField	= city_admin::kAddrCode2;
	}

	if(crAdmInfo.mCode6.substr(3, 3) == "000") {
		crOazaRepFlag = true;
		ipFCur_CityAdmin = SearchByWhereClause(
			uh::str_util::format(_T("%s = '%s' and %s like '%s%%'"),
				city_admin::kCityCode, uh::toTStr(crCityCode).c_str(),
				uh::toTStr(aAddrCodeField).c_str(),
				uh::toTStr(crAdmInfo.mCode6.substr(0, 3)).c_str()),
			m_ipFcCityAdmin,
			&aCACount);
	} else {
		ipFCur_CityAdmin = SearchByWhereClause(
			uh::str_util::format(_T("%s = '%s' and %s = '%s'"),
				city_admin::kCityCode, uh::toTStr(crCityCode).c_str(),
				uh::toTStr(aAddrCodeField).c_str(),
				uh::toTStr(crAdmInfo.mCode6).c_str()),
			m_ipFcCityAdmin,
			&aCACount);
	}

	// ���ؗp�f�[�^���ƁA�����擾�ł��Ȃ��������蓾��
	if(!ipFCur_CityAdmin)
		return;

	// ����11���|���S���̃��[�v
	IFeaturePtr	ipF_CityAdmin;
	while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK && ipF_CityAdmin) {

		ipF_CityAdmin->get_OID(&crOID_CityAdmin);

		IGeometryPtr	ipG_CityAdmin;
		ipF_CityAdmin->get_Shape(&ipG_CityAdmin);

		// �L���ȍ��|�C���g�̃J�E���g
		long	aGPCount = 0;
		IFeatureCursorPtr	ipFCur_GouPoint;
		ipFCur_GouPoint = SearchByShape(ipG_CityAdmin, esriSpatialRelContains, m_ipFcGouPoint, &aGPCount);
		if(aGPCount > 0) {
			IFeaturePtr	ipF_GouPoint;
			while(ipFCur_GouPoint->NextFeature(&ipF_GouPoint) == S_OK) {
				CComVariant	aGouVar;	// ���ԍ�
				ipF_GouPoint->get_Value(m_fIdGPGouNumber, &aGouVar);
				if(aGouVar.vt != VT_BSTR) --aGPCount;
			}
		}
		crGPTotal += aGPCount;
	}
}

// ���|�C���g�̃`�F�b�N
void CheckBaseRepExist::
CheckGoupoint( const string& crCityCode,
				 const adminfo& crAdmInfo,
				 bool crOazaRepFlag,
				 long crOID_CityAdmin,
				 long crGPTotal )
{
	// ���|�C���g�̃`�F�b�N
	gMessage = crCityCode + crAdmInfo.mCode6;
	string	aTmpMessage;
	long	aErrorCode	= 0;
	if(crGPTotal == 0) {
		// 11�����L���i�p�~�E�����łȂ��j��
		bool	aIsValid = false;
		m_jdbAccess.IsValid((crCityCode + crAdmInfo.mCode6).c_str(), &aIsValid);
		if(aIsValid) {

			if(crOazaRepFlag) {
				aErrorCode	= crAdmInfo.mInfoType==adminfo::ADDR1? ERR_NO_OAZA_REP : ERR_NO_OAZA_REP_ADDR2;
				aTmpMessage	= "��\�_�Ȃ��i�厚�j";
			} else {
				aErrorCode	= crAdmInfo.mInfoType==adminfo::ADDR1? ERR_NO_BASE_REP : ERR_NO_BASE_REP_ADDR2;
				aTmpMessage	= "��\�_�Ȃ�";
			}

		} else {
			aErrorCode	= crAdmInfo.mInfoType==adminfo::ADDR1? ERR_INVALID_CODE : ERR_INVALID_CODE_ADDR2;
			aTmpMessage	= "�p�~ or ����";
		}
	} else {
		aErrorCode	= crAdmInfo.mInfoType==adminfo::ADDR1? ERR_FROM_GOU_POINT : ERR_FROM_GOU_POINT_ADDR2;
		aTmpMessage	= "��\�_����i���|�C���g�j";
	}
	// ���b�Z�[�W�o��
	m_errLog.OutputErr(m_fcCityAdmin.c_str(), crOID_CityAdmin, aErrorCode, gMessage.c_str());
	fprintf(m_fpStatList, "%s%s\t%s%s\n", crCityCode.c_str(), crAdmInfo.mCode6.c_str(), aTmpMessage.c_str(), crAdmInfo.mInfoType==adminfo::ADDR1?"":gFieldStr);
}

// Base_Rep_Point �Ɋւ���`�F�b�N
void CheckBaseRepExist::
CheckBaseRepPoint( const AOList& crAOListBR )
{
	AOList::iterator aItr;
	for(aItr = crAOListBR.begin(); aItr != crAOListBR.end(); aItr++) {
		if(("100" < (*aItr).eAddrCode.substr(8, 3) && (*aItr).eAddrCode.substr(8, 3) <= "999") ) {	// [bug 8183](�����R�[�h)�Ή� 2010.08.27
			AOList::iterator aItrF = crAOListBR.find((*aItr).eAddrCode.substr(0, 8) + "000");
			if(aItrF == crAOListBR.end()) {
				gMessage = (*aItr).eAddrCode;
				m_errLog.OutputErr(m_fcBaseRep.c_str(), (*aItr).eOID, ERR_NO_OAZA_REP, gMessage.c_str());
			}
		}
	}
}

// CS_Addr_Point �Ɋւ���`�F�b�N
void CheckBaseRepExist::
CheckCSAddrPoint( const AOList& crAOListBR, const string& crCityCode )
{
	// CS_Addr_Point �Ɋւ���`�F�b�N
	long	aCSCount = 0;
	IFeatureCursorPtr	ipFCur_CSAddrPoint;
	ipFCur_CSAddrPoint = SearchByWhereClause(
		uh::str_util::format(_T("%s like '%s%%'"), cs_addr_point::kAddrCode, uh::toTStr(crCityCode).c_str()),
		m_ipFcCSAddrPoint,
		&aCSCount);
	if(aCSCount > 0) {
		IFeaturePtr	ipF_CSAddrPoint;
		while(ipFCur_CSAddrPoint->NextFeature(&ipF_CSAddrPoint) == S_OK) {
			string	aAddrCode;
			CComVariant	aAddrVar;
			ipF_CSAddrPoint->get_Value(m_fIdCSAddrCode, &aAddrVar);
			aAddrCode = ExtractStr(aAddrVar);
			AOList::iterator aBRItr = crAOListBR.find(aAddrCode.substr(0, 11));
			if(aBRItr == crAOListBR.end()) { // ���݂��Ȃ� �� �G���[
				long	aOID_CSAddrPoint = 0;
				ipF_CSAddrPoint->get_OID(&aOID_CSAddrPoint);
				gMessage = aAddrCode;
				m_errLog.OutputErr(m_fcCSAddrPoint.c_str(), aOID_CSAddrPoint, ERR_NO_BASE_REP, gMessage.c_str());
			}
		}
	}
}

void CheckBaseRepExist::
CheckAreacodeMaster(const string& cityCode, const AOList& aoList)
{
	list<string> addrCodes;
	if( !m_jdbAccess.GetAddrCodeByCityCode(cityCode, addrCodes) ) {
		// JDB_Access���ŃG���[���b�Z�[�W���o��̂ŁA�����ł͓��ɂȂɂ����Ȃ�
		return;
	}

	for(const auto& addrCode : addrCodes) {

		if( 0 == addrCode.substr(5,6).compare("000000") ) {
			// ��6����000000�������猟���ΏۊO
			continue;
		}

		// ������Ȃ�������G���[���O��
		const auto& findResult = find_if(aoList.begin(), aoList.end(),
			[addrCode](const AORec& rec){
				return (0 == addrCode.compare(rec.eAddrCode));
		});
		if( aoList.end() == findResult ) {
			m_errLog.OutputErr("AREACODE_MASTER", 0, ERR_NOT_MATCH_AREACODEMASTER, addrCode.c_str());
		}
	}
}

bool CheckBaseRepExist::
Execute(int cArgc, char **cArgv)
{
	m_Argc	= cArgc;
	m_Argv	= cArgv;

	// ������
	if( !Init () ) {
		return false;
	}

	// �s�撬���R�[�h���X�g�̍쐬
	if(!fnCreateCityList()) {
		return false;
	}

	// �s�撬�����[�v
	for(const auto& aCity : m_cityList) {

		// �厚�^���R�[�h���X�g�̍쐬
		AdminList	aAddrList;
		MakeOazaAzaList( aCity, aAddrList );

		// Base_Rep_Point �̃��X�g�쐬
		AOList	aAOListBR;
		MakeBaseRepPointList( aCity, aAOListBR );

		// �厚�^���R�[�h���X�g�̃��[�v
		for(const auto& aAddr : aAddrList) {

			cout << aCity << aAddr.mCode6 << ": �`�F�b�N��..." << endl;

			bool	aOazaRepFlag = false;

			// ���Y11�������� Base_Rep_Point �����݂��邩�H
			string aAddrCode = aCity + aAddr.mCode6;
			AOList::iterator aBRItr = aAOListBR.find(aAddrCode);
			if(aBRItr != aAOListBR.end()) {	// ���݂��Ă��� �� �`�F�b�N�I��
				fprintf(m_fpStatList, "%s%s\t��\�_����%s\n", aCity.c_str(), aAddr.mCode6.c_str(), aAddr.mInfoType==adminfo::ADDR1?"":gFieldStr);
				continue;
			}

			// ����11���|���S���̌���
			long	aGPTotal = 0;
			long	aOID_CityAdmin = 0;
			GetContainedGouCount( aCity, aAddr, aOazaRepFlag, aOID_CityAdmin, aGPTotal );

			// ���|�C���g�̃`�F�b�N
			CheckGoupoint( aCity, aAddr, aOazaRepFlag, aOID_CityAdmin, aGPTotal );

			// ���O�̃t���b�V��
			m_errLog.Flush();
			fflush(m_fpStatList);

		}

		// Base_Rep_Point �Ɋւ���`�F�b�N
		CheckBaseRepPoint( aAOListBR );

		// CS_Addr_Point �Ɋւ���`�F�b�N
		CheckCSAddrPoint( aAOListBR, aCity );

		// Areacode_Master �Ɋւ���`�F�b�N
		CheckAreacodeMaster( aCity, aAOListBR );

		m_errLog.Flush();
	}

	// �`�F�b�N�I�����������O�֏o��
	m_errLog.OutputTime();

	// ���O�t�@�C���̃N���[�Y
	m_errLog.Flush();
	m_errLog.Close();
	fclose(m_fpStatList);

	return true;

}
