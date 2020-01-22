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

#include "LogSys.h"
#include "common.h"
#include "BldNameImporter.h"
#include "SurveyPointManager.h"

#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/tstring.h>
#include <sindy/workspace.h>
#include <TDC/sindylib_core/Workspace.h>
#include <WinLib/CStringTokenizer.h>

namespace po = boost::program_options;

/**
 * @brief	���X�g�擾�֐�
 * @param	filePath [in] ���X�g�t�@�C���̃p�X
 * @param	list [out] �ǂݍ��񂾃��X�g
 * @return true ����
 * @return false ���s
 */
bool get_list(LPCTSTR filePath, std::vector<StrPair>* list)
{
	CString errMsg;
	// �t�@�C���I�[�v��
	std::ifstream ifs(filePath);
	if (! ifs.is_open()) {
		errMsg.Format(_T("%s�̓ǂݍ��݂Ɏ��s���܂���"), filePath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// �ǂݍ���
	std::string temp;
	while(std::getline(ifs, temp)) {
		CString strLine = CA2CT(temp.c_str());
		CStringTokenizer cTokenizer(strLine, _T("\t"), TOKEN_RET_EMPTY_ALL);
		long tokenCount = cTokenizer.CountTokens();
		if (tokenCount != 2) {
			errMsg.Format(_T("�u�������񃊃X�g�̃t�H�[�}�b�g���s���ł��B(�^�u��؂��2�t�B�[���h�ɂȂ��Ă��Ȃ�),%s"), filePath);
			return false;
		}
		StrPair strPair;
		strPair.e_orgStr = cTokenizer.GetNextToken();
		CString replaceStr = cTokenizer.GetNextToken();
		if (replaceStr.CompareNoCase(_T("(�폜)")) == 0 || 
			replaceStr.CompareNoCase(_T("�i�폜)")) == 0 ||
			replaceStr.CompareNoCase(_T("(�폜�j")) == 0 ||
			replaceStr.CompareNoCase(_T("�i�폜�j")) == 0 ||
			replaceStr.CompareNoCase(_T(" ")) == 0 )
			strPair.e_replaceStr = _T("");
		else
			strPair.e_replaceStr = replaceStr;
		list->push_back(strPair);
	}
	return true;
}

/**
 * @brief	���X�g�擾�֐�
 * @param	filePath [in] ���X�g�t�@�C���̃p�X
 * @param	list [out] �ǂݍ��񂾃��X�g
 * @return true ����
 * @return false ���s
 */
bool get_list(LPCTSTR filePath, std::vector<CString>* list)
{
	// �t�@�C���I�[�v��
	std::ifstream ifs(filePath);
	if (! ifs.is_open()) {
		CString errMsg;
		errMsg.Format(_T("%s�̓ǂݍ��݂Ɏ��s���܂���"), filePath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// �ǂݍ���
	std::string temp;
	while (std::getline(ifs, temp)) {
		CString record = CA2CT(temp.c_str());
		record.Trim();
		if (! record.IsEmpty())
			list->push_back(record);
	}
	return true;
}

/**
 * @brief  ���X�g�擾�֐�
 * @param  filePath [in] ���X�g�t�@�C���̃p�X
 * @param  list [out] �ǂݍ��񂾃��X�g
 * @return true ����
 * @return false ���s
 */
bool get_list(LPCTSTR filePath, std::vector<long>* list)
{
	// �t�@�C���I�[�v��
	std::ifstream ifs(filePath);
	if (! ifs.is_open()) {
		CString errMsg;
		errMsg.Format(_T("%s�̓ǂݍ��݂Ɏ��s���܂���"), filePath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// �ǂݍ���
	std::string temp;
	while (std::getline(ifs, temp)) {
		CString record = CA2CT(temp.c_str());
		record.Trim();
		if (! record.IsEmpty())
			list->push_back(_ttol(record));
	}
	return true;
}

/**
 * @brief	���X�g�擾�֐�
 * @param	filePath [in] ���X�g�t�@�C���̃p�X
 * @param	list [out] �ǂݍ��񂾃��X�g
 * @return true ����
 * @return false ���s
 */
bool get_list(LPCTSTR filePath, std::set<CString>* list)
{
	// �t�@�C���I�[�v��
	std::ifstream ifs(filePath);
	if (! ifs.is_open()) {
		CString errMsg;
		errMsg.Format(_T("%s�̓ǂݍ��݂Ɏ��s���܂���"), filePath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// �ǂݍ���
	std::string temp;
	while (std::getline(ifs, temp)) {
		CString record = CA2CT(temp.c_str());
		record.Trim();
		if (! record.IsEmpty())
			list->insert(record);
	}
	return true;
}

/**
 * @brief  ���X�g�擾�֐�
 * @param  filePath [in] ���X�g�t�@�C���̃p�X
 * @param  list [out] �ǂݍ��񂾃��X�g
 * @return true ����
 * @return false ���s
 */
bool get_list(LPCTSTR filePath, std::set<long>* list)
{
	// �t�@�C���I�[�v��
	std::ifstream ifs(filePath);
	if (! ifs.is_open()) {
		CString errMsg;
		errMsg.Format(_T("%s�̓ǂݍ��݂Ɏ��s���܂���"), filePath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// �ǂݍ���
	std::string temp;
	while (std::getline(ifs, temp)) {
		CString record = CA2CT(temp.c_str());
		record.Trim();
		if (! record.IsEmpty())
			list->insert(_ttol(record));
	}
	return true;
}

/**
 * @brief  ���X�g�擾�֐�
 * @param  filePath [in] ���X�g�t�@�C���̃p�X
 * @param  list [out] �ǂݍ��񂾃��X�g
 * @return true ����
 * @return false ���s
 */
bool get_list(LPCTSTR filePath, std::map<long, long>* list)
{
	CString errMsg;
	// �t�@�C���I�[�v��
	std::ifstream ifs(filePath);
	if (! ifs.is_open()) {
		errMsg.Format(_T("%s�̓ǂݍ��݂Ɏ��s���܂���"), filePath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// �ǂݍ���
	std::string temp;
	while(std::getline(ifs, temp)) {
		CString strLine = CA2CT(temp.c_str());
		CStringTokenizer cTokenizer(strLine, _T("\t"), TOKEN_RET_EMPTY_ALL);
		long tokenCount = cTokenizer.CountTokens();
		if (tokenCount != 2) {
			errMsg.Format(_T("�̗p�����L�q���X�g�̃t�H�[�}�b�g���s���ł��B(�^�u��؂��2�t�B�[���h�ɂȂ��Ă��Ȃ�),%s"), filePath);
			return false;
		}
		long priority, matchPer;
		priority = _ttol(cTokenizer.GetNextToken());
		matchPer = _ttol(cTokenizer.GetNextToken());
		list->insert(std::map<long, long>::value_type(priority, matchPer));
	}
	return true;
}

/**
 * @brief  �t�B�[���h�C���f�b�N�X�擾�֐�
 * @param  tableName [in] �e�[�u����
 * @param  ipTable [in] �e�[�u��
 * @param  indeces [out] �C���f�b�N�X���
 * @return true ����
 * @return false ���s
 */
bool get_field_indeces(LPCTSTR tableName, const ITablePtr& ipTable, std::map<CString, long>* indeces)
{
	CString errMsg;
	indeces->clear();
	// �t�B�[���h�Q�擾
	IFieldsPtr ipFields;
	if (FAILED(ipTable->get_Fields(&ipFields))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[���h�Q�擾�Ɏ��s���܂���"), tableName);
		return false;
	}
	// �t�B�[���h���擾
	long fieldCount;
	if (FAILED(ipFields->get_FieldCount(&fieldCount))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[���h���擾�Ɏ��s���܂���"), tableName);
		return false;
	}
	// �e�t�B�[���h���E�t�B�[���h�C���f�b�N�X�擾
	for (long i = 0; i < fieldCount; ++i) {
		// �t�B�[���h�擾
		IFieldPtr ipField;
		if (FAILED(ipFields->get_Field(i, &ipField))) {
			errMsg.Format(_T("�t�B�[���h�擾�Ɏ��s���܂����Bindex = %d"), i);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, tableName);
			return false;
		}
		// �t�B�[���h���擾
		CComBSTR fieldName;
		if (FAILED(ipField->get_Name(&fieldName))) {
			errMsg.Format(_T("�t�B�[���h���擾�Ɏ��s���܂����B index = %d"), i);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, tableName);
			return false;
		}
		// index�o�^
		indeces->insert(std::pair<CString, long>(CString(fieldName).MakeLower(), i));		
	}
	return true;
}

/**
 * @brief  �e�[�u���擾�`�F�b�N�֐�
 * @param  tableName [in] �e�[�u����
 * @param  ipTable [in] �e�[�u��
 * @return true �e�[�u���擾����
 * @return false �e�[�u���擾���s
 */
bool chk_table(LPCTSTR tableName, ITablePtr& ipTable)
{
	if (nullptr == ipTable){
		CString errMsg;
		errMsg.Format(_T("%s�̃I�[�v���Ɏ��s���܂���"), tableName);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	return true;
}

/**
 * @brief  ���R���T�C�������t���̕ҏW�I������
 * @param  workspace [in] ���[�N�X�y�[�X
 * @return HRESULT 
 */
HRESULT StopEditingOrReconcile(sindy::CWorkspace& workspace)
{
	CString errMsg;
	IWorkspaceEditPtr ipWork((IWorkspace*)workspace);
	if (! ipWork)
		return false;

	HRESULT hr = ipWork->StopEditing( VARIANT_TRUE );
	if(FDO_E_VERSION_REDEFINED != hr){
		if( S_OK == hr )
			LogSys::GetInstance().WriteProcLog(true, true, _T("StopEditing(saveEdits=true)....OK\n"));
		else
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("StopEditing(SaveEdits=true)......FAILED!!\n"));
		return hr;
		
	}

	// ���R���T�C�����K�v�ȏꍇ
	VARIANT_BOOL vbConflicts = VARIANT_FALSE;
	IVersionEdit4Ptr ipVerEdit( ipWork );
	hr = ipVerEdit->Reconcile4( 
		CComBSTR(workspace.GetNameString()->GetVersionName()), 
		VARIANT_TRUE,        // �r���I�Ƀ��R���T�C����������
		VARIANT_TRUE,        // �R���t���N�g���N������Abort����
		VARIANT_FALSE,       // TODO: ChildWins���ĂȂɁH�H
		VARIANT_TRUE,        // �J�������x���ł̃��R���T�C������������
		&vbConflicts );
	switch( hr )
	{
	case FDO_E_RECONCILE_VERSION_NOT_AVAILABLE:	errMsg = _T("Reconcile : [FDO_E_RECONCILE_VERSION_NOT_AVAILABLE]"); break;
	case FDO_E_VERSION_BEING_EDITED:			errMsg = _T("Reconcile : [FDO_E_VERSION_BEING_EDITED]"); break;
	case FDO_E_VERSION_NOT_FOUND:				errMsg = _T("Reconcile : [FDO_E_VERSION_NOT_FOUND]"); break;
	case FDO_E_NOT_EDITING:						errMsg = _T("Reconcile : [FDO_E_NOT_EDITING]"); break;
	case S_OK:
		// �R���t���N�g�����ꍇ
		if( vbConflicts )
		{
			errMsg = _T("ERROR : Reconcile : �R���t���N�g���N�������߁A�ҏW��j�����܂����B");
			hr = E_FAIL;
		}
		else {
			LogSys::GetInstance().WriteProcLog(true, true, _T("Reconciling...OK\n"));
			hr = StopEditingOrReconcile( workspace ); // ���R���T�C����͍ēx�ۑ������iStopEditing�j���K�v
		}
		break;
	default:
		errMsg = _T("ERROR : Reconcile : ���R���T�C���Ō����s���̃G���[[%ld]\n");
		break;
	}
	if ( S_OK != hr )
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
	return hr;
}

/**
 * @brief  �ҏW�I���֐�
 * @param  workspace [in] ���[�N�X�y�[�X
 * @param  save [in] �ۑ��̗L���@true:�ۑ����� false:�ۑ����Ȃ�
 * @retval true ����I��
 * @retval false �ُ�I��
 */
bool StopEdit(sindy::CWorkspace& workspace, bool save)
{
	if (save) {
		// �ҏW�Z�b�V�����I��
		if (sindy::errorcode::sindyErr_NoErr != workspace.StopEditOperation()) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�ҏW�Z�b�V�����I���Ɏ��s���܂���"));
			return false;
		}
		// ���R���T�C�������t���̕ҏW�I������
		if (FAILED(StopEditingOrReconcile(workspace))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�ҏW�I���Ɏ��s���܂���"));
			return false;
		}
	} else {
		workspace.AbortEditOperation();
		workspace.StopEditing(false);
	}
	return true;
}

/**
 * @brief  �ҏW�J�n�֐�
 * @param  workspace [in] ���[�N�X�y�[�X
 * @param  nonversioned [in] �o�[�W�����Ή��̗L�� true:�o�[�W�������Ή� false:�o�[�W�����Ή�
 * @retval true ����I��
 * @retval false �ُ�I��
 */
bool StartEdit(sindy::CWorkspace& workspace, bool nonversioned)
{
	// �ҏW�J�n
	IMultiuserWorkspaceEditPtr multiWork((IWorkspace*)workspace);
	if (multiWork) {
		if (FAILED(multiWork->StartMultiuserEditing( nonversioned ? esriMESMNonVersioned : esriMESMVersioned ))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�ҏW�J�n�Ɏ��s���܂���"));
			return false;
		}
	} else {
		if (sindy::errorcode::sindyErr_NoErr != workspace.StartEditing(false)) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�ҏW�J�n�Ɏ��s���܂���"));
			return false;
		}
	}
	// �ҏW�Z�b�V�����J�n
	if ( sindy::errorcode::sindyErr_NoErr != workspace.StartEditOperation()) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�ҏW�Z�b�V�����J�n�Ɏ��s���܂���"));
		StopEdit(workspace, false);
		return false;
	}
	LogSys::GetInstance().WriteProcLog(false, true, _T("StartEditing(UndoRedo=false)...OK\n"));
	return true;
}

/**
 * @brief  ���[�N�X�y�[�X�擾�֐�
 * @param  connectInfo [in] ���[�N�X�y�[�X�ւ̃p�X(DB��)
 * @param  workspace [out] ���[�N�X�y�[�X
 * @retval true ����
 * @retval false ���s
 */
bool get_workspace(LPCTSTR connectInfo, sindy::CWorkspace* workspace)
{
	CString errMsg;
	IWorkspaceName2Ptr ipWorkspaceName = sindy::create_workspace_name(connectInfo);
	if (! ipWorkspaceName) {
		errMsg.Format(_T("�ڑ������񂪖����ł� : %s"), connectInfo);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	IUnknownPtr ipUnknown;
	if (FAILED(((INamePtr)ipWorkspaceName)->Open(&ipUnknown))) {
		errMsg.Format(_T("%s�̐ڑ��Ɏ��s���܂���"), connectInfo);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	*workspace = IWorkspacePtr(ipUnknown);
	return true;
}

/**
 * @brief �Z��DB�ւ̐ڑ�
 * @param strAddrDB [in] �ڑ�������i<user>@<service> �`���j
 * @param con [out] �ڑ���DB
 * @retval true  ����
 * @retval false ���s
 */
bool openAddrDB(const CString& strAddrDB, ADODB::_ConnectionPtr& con)
{
	// �ڑ������񂩂烆�[�U�E�T�[�r�X�����擾
	int nPos = strAddrDB.Find( _T('@') );
	if( 0 >= nPos )
	{
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�Z��DB�ڑ������񂪖����ł� : ") + strAddrDB );
		return false;
	}
	CString strDBUser = strAddrDB.Left( nPos );
	CString strDBSrvc = strAddrDB.Mid( nPos + 1 );
	if( strDBUser.IsEmpty() || strDBSrvc.IsEmpty() )
	{
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�Z��DB�ڑ������񂪖����ł� : ") + strAddrDB );
		return false;
	}
	
	// �Z��DB�ɐڑ�
	CString strConnect;
	strConnect.Format( _T(" Provider=OraOLEDB.Oracle; Data Source=%s; User ID=%s; Password=%s; "), strDBSrvc, strDBUser, strDBUser );
	con->ConnectionString = _bstr_t( strConnect );
	try
	{
		if( FAILED( con->Open( "", "", "", ADODB::adConnectUnspecified ) ) )
		{
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�Z��DB�ɐڑ��ł��܂��� : ") + strAddrDB );
			return false;
		}
	}
	catch( const _com_error& e )
	{
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�Z��DB�ڑ����ɗ�O���� : ") + strAddrDB );
		throw e;
	}

	return true;
}

bool exec(const po::variables_map &vmContent, bool isUpdate, bool isSurvey)
{
	// �e��DB�ւ̐ڑ�
	sindy::CWorkspace  srcWorkspace, mapWorkspace, addrWorkspace, baseWorkspace, poiWorkspace, twnWorkspace, meshWorkspace;
	if (! get_workspace(vmContent[SRC_DB].as<uh::tstring>().c_str(), &srcWorkspace) ||			// �f��DB
		! get_workspace(vmContent[MAP_DB].as<uh::tstring>().c_str(), &mapWorkspace) ||			// MAPDB
		! get_workspace(vmContent[ADDR_DB].as<uh::tstring>().c_str(), &addrWorkspace) ||		// �s���EDB
		! get_workspace(vmContent[POI_DB].as<uh::tstring>().c_str(), &poiWorkspace) ||			// POIDB
		! get_workspace(vmContent[BASE_DB].as<uh::tstring>().c_str(), &baseWorkspace))			// �x�[�XDB
		return false;

	// TWNDB�A�Z��DB�͒����Ώۃ|�C���g�o�̓��[�h�̂ݐڑ�
	ADODB::_ConnectionPtr conAddrDB(__uuidof(ADODB::Connection));
	if (isSurvey) {
		if (! get_workspace(vmContent[MESH_DB].as<uh::tstring>().c_str(), &meshWorkspace))		// ���b�V��DB
			return false;
		if (! get_workspace(vmContent[TWN_DB].as<uh::tstring>().c_str(), &twnWorkspace))		// TWNDB
			return false;
		if (! openAddrDB(vmContent[AM_DB].as<uh::tstring>().c_str(), conAddrDB))
			return false;
	}
	
	// �e��e�[�u���̃I�[�v��(�����[�h����)
	IFeatureClassPtr ipBldSrcPt, ipBld, ipCityAdm, ipPoiPtOrg, ipBldNamePt;
	ITablePtr ipBldAddInfo;
	ipBldSrcPt = srcWorkspace.OpenTable(sindy::schema::sj::buildingname_src_point::kTableName);
	ipBldAddInfo = srcWorkspace.OpenTable(sindy::schema::sj::buildingname_src_addinfo::kTableName);
	ipBld = mapWorkspace.OpenTable(sindy::schema::building::kTableName);
	ipCityAdm = addrWorkspace.OpenTable(sindy::schema::city_admin::kTableName);
	ipPoiPtOrg = poiWorkspace.OpenTable(sindy::schema::sindyk::poi_point_org::kTableName);
	ipBldNamePt = baseWorkspace.OpenTable(sindy::schema::sj::buildingname_point::kTableName);
	if (! chk_table(sindy::schema::sj::buildingname_src_point::kTableName, (ITablePtr)ipBldSrcPt) ||
		! chk_table(sindy::schema::sj::buildingname_src_addinfo::kTableName, ipBldAddInfo) ||
		! chk_table(sindy::schema::building::kTableName, (ITablePtr)ipBld) ||
		! chk_table(sindy::schema::city_admin::kTableName, (ITablePtr)ipCityAdm) ||
		! chk_table(sindy::schema::sindyk::poi_point_org::kTableName, (ITablePtr)ipPoiPtOrg) ||
		! chk_table(sindy::schema::sj::buildingname_point::kTableName, (ITablePtr)ipBldNamePt))
		return false;
	// DB�X�V�p�e�[�u���̃I�[�v��
	ITablePtr ipRelBldName;
	if (isUpdate) {
		ipRelBldName = baseWorkspace.OpenTable(sindy::schema::sj::rel_buildingname::kTableName);
		if (! chk_table(sindy::schema::sj::rel_buildingname::kTableName, ipRelBldName))
			return false;
	}
	// �����Ώۃ|�C���g�o�͗p�̃I�[�v��
	IFeatureClassPtr ipTwnpage, ipCityMesh;
	if (isSurvey) {
		ipTwnpage = twnWorkspace.OpenTable(vmContent[TWN_TBL_NAME].as<uh::tstring>().c_str());
		ipCityMesh = meshWorkspace.OpenTable(sindy::schema::citymesh::kTableName);
		if (! chk_table(vmContent[TWN_TBL_NAME].as<uh::tstring>().c_str(), (ITablePtr)ipTwnpage))
			return false;
		if (! chk_table(sindy::schema::citymesh::kTableName, (ITablePtr)ipCityMesh))
			return false;
	}

	// �e��e�[�u���̃C���f�b�N�X�擾
	std::map<CString, long> bldSrcIndeces, bldAddInfoIndeces, bldIndeces, poiPtOrgIndeces, bldNamePtIndeces;
	if (! get_field_indeces(sindy::schema::sj::buildingname_src_point::kTableName, (ITablePtr)ipBldSrcPt, &bldSrcIndeces) ||
		! get_field_indeces(sindy::schema::sj::buildingname_src_addinfo::kTableName, ipBldAddInfo, &bldAddInfoIndeces) ||
		! get_field_indeces(sindy::schema::building::kTableName, (ITablePtr)ipBld, &bldIndeces) ||
		! get_field_indeces(sindy::schema::sindyk::poi_point_org::kTableName, (ITablePtr)ipPoiPtOrg, &poiPtOrgIndeces) ||
		! get_field_indeces(sindy::schema::sj::buildingname_point::kTableName, (ITablePtr)ipBldNamePt, &bldNamePtIndeces))
		return false;
	// DB�X�V�p�e�[�u���̃C���f�b�N�X�擾
	std::map<CString, long> relBldNameIndeces;
	if (isUpdate) {
		if (! get_field_indeces(sindy::schema::sj::rel_buildingname::kTableName, ipRelBldName, &relBldNameIndeces))
			return false;
	}
	// �����Ώۃ|�C���g�o�͗p�e�[�u���̃C���f�b�N�X�擾
	std::map<CString, long> twnpageIndeces, cityMeshIndexes, cityAdminIndexes;
	if (isSurvey) {
		if (! get_field_indeces(vmContent[TWN_DB].as<uh::tstring>().c_str(), (ITablePtr)ipTwnpage, &twnpageIndeces) ||
			! get_field_indeces(sindy::schema::citymesh::kTableName, (ITablePtr)ipCityMesh, &cityMeshIndexes) ||
			! get_field_indeces(sindy::schema::city_admin::kTableName, (ITablePtr)ipCityAdm, &cityAdminIndexes))
			return false;
	}

	// �e�탊�X�g�̓ǂݍ���
	std::set<CString> tgtAddrList;
	std::vector<CString> poiPriList;
	std::vector<CString> supPoiPriList;
	std::vector<long> srcPriListPrior;
	std::vector<long> srcPriListMatch;
	std::set<long> bldExcList;
	std::vector<StrPair> replaceList;
	std::map<long, long> condList;
	if (! get_list(vmContent[TGT_ADDR_LIST].as<uh::tstring>().c_str(), &tgtAddrList) ||			// �����Ώۍs���E���X�g
		! get_list(vmContent[SRC_PRI_LIST_PRIOR].as<uh::tstring>().c_str(), &srcPriListPrior) ||	// �f�ޗD��x��ʃ��X�g�i�D�旘�p�j
		! get_list(vmContent[SRC_PRI_LIST_MATCH].as<uh::tstring>().c_str(), &srcPriListMatch) ||	// �f�ޗD��x��ʃ��X�g�i�}�b�`���O�j
		! get_list(vmContent[POI_PRI_LIST].as<uh::tstring>().c_str(), &poiPriList) ||			// ���LPOI�D��x��ʃ��X�g
		! get_list(vmContent[SUP_POI_PRI_LIST].as<uh::tstring>().c_str(), &supPoiPriList) ||	// �⊮�p���LPOI�D��x��ʃ��X�g
		! get_list(vmContent[BLD_EXC_LIST].as<uh::tstring>().c_str(), &bldExcList) ||			// �����ΏۊO������ʃ��X�g
		! get_list(vmContent[REPLACE_LIST].as<uh::tstring>().c_str(), &replaceList) ||			// �u�������񃊃X�g
		! get_list(vmContent[COND_LIST].as<uh::tstring>().c_str(), &condList))					// �̗p�����L�q���X�g
		return false;

	// �ҏW�J�n
	// DB�X�V���[�h
	if (isUpdate) {
		if (! StartEdit(baseWorkspace, true))
			return false;
	}

	// �����Ώۃ|�C���g�o�̓��[�h
	SurveyPointManager surveyPtMgr( ipCityMesh, ipCityAdm, cityMeshIndexes, cityAdminIndexes, bldSrcIndeces, twnpageIndeces, vmContent[SOURCENAME].as<uh::tstring>().c_str(), vmContent[SOURCEDATE].as<uh::tstring>().c_str(), conAddrDB );
	if (isSurvey) {
		CString fileName = vmContent[SURVEY_LIST].as<uh::tstring>().c_str();
		if(! surveyPtMgr.open( fileName ) )
		{
			std::cerr << CT2A( fileName ) << " �̃I�[�v���Ɏ��s" << std::endl;
			return false;
		}
	}

	// �C���|�[�g�N���X��`
	BldNameImporter bldNameImporter(srcPriListPrior,
									srcPriListMatch,
								    poiPriList,
									supPoiPriList,
									replaceList,
									bldExcList,
									condList,
									bldSrcIndeces,
									bldAddInfoIndeces,
									bldIndeces,
									twnpageIndeces,
									poiPtOrgIndeces,
									bldNamePtIndeces,
									relBldNameIndeces,
									ipBldSrcPt,
									ipBldAddInfo,
									ipBld,
									ipCityAdm,
									ipTwnpage,
									ipPoiPtOrg,
									ipBldNamePt,
									ipRelBldName,
									isUpdate,
									isSurvey,
									surveyPtMgr);

	if(! bldNameImporter.Init() ) return false;

	// �s���E�R�[�h���ɏ��������{
	for (const auto& addrCode : tgtAddrList) {
		if (! bldNameImporter.Import(addrCode)) {
			LogSys::GetInstance().WriteProcLog(true, false, addrCode + _T("\tNG\n"));
			if (isUpdate) 
				StopEdit(baseWorkspace, false);
			return false;
		} else {
			LogSys::GetInstance().WriteProcLog(true, false, addrCode + _T("\tOK\n"));
		}
	}

	// �ҏW�I��
	// DB�X�V���[�h
	if (isUpdate) {
		if (! StopEdit(baseWorkspace, true))
			return false;
	}
	// �����Ώۃ|�C���g�o�̓��[�h
	if (isSurvey) {
		surveyPtMgr.close();
	}
	return true;
}
