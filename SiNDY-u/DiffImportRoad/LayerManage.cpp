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

#include "StdAfx.h"
#include <sindy/workspace.h>
#include "LayerManage.h"
#include "Reflect.h"
using namespace Reflect;
using namespace sindy::schema;

// �R���X�g���N�^
CLayerManage::CLayerManage(void)
: m_IsInitial(false)
, m_MeshLayer(_T("Reference.BaseMesh"))
, m_FcIndex(-1)
, m_ExtFcIndex(-1)
, m_OtherIndex(-1)
{
}

// �f�X�g���N�^
CLayerManage::~CLayerManage(void)
{
}

// �u����́v�g��Ȃ������̓R�����g�A�E�g
// �A���A�L���ɂ������͒������K�v�ɂȂ邩��
// �����̏��Ԃ��d�v�ɂȂ�̂ŁA���ς�炸�e�[�u���`��

///< �t�B�[�`���N���X�Ǘ��e�[�u��
EnvTable CLayerManage::m_FcEnvTable[] = {
	//EnvTable(_T("���ԏ�"), parking_shape::kTableName),
	//EnvTable(_T("�{�g���l�b�N"), _T("BOTTLENECK_RAILCROSSING")),
	EnvTable(_T("���H�����N"), road_link::kTableName),
	EnvTable(_T("���H�m�[�h"), road_node::kTableName),
#ifndef STOP_POINT
	EnvTable(_T("�����N������"), segment_attr::kTableName),
	EnvTable(_T("�X��"), grad::kTableName),
	EnvTable(_T("�w�i�|���S��"), base_site::kTableName),
	//�ҏW���邱�Ƃ͂Ȃ����炢��Ȃ��@EnvTable(_T("�w�i���C��"), base_line::kTableName),
#endif
	EnvTable(_T(""), _T(""))
};

///< �t�B�[�`���N���X(�ꎞ��~)�Ǘ��e�[�u��
EnvTable CLayerManage::m_ExtFcEnvTable[] = {
	EnvTable(_T("�ꎞ��~"), stop_point::kTableName),
	EnvTable(_T(""), _T(""))
};

///< �����N��p�e�[�u���Ǘ��e�[�u��
LqEnvTable CLayerManage::m_LqEnvTable[] = {
	LqEnvTable(_T("BYWAY"), inf_byway::kTableName, lq_byway::kTableName),
	LqEnvTable(_T("DIRGUIDE"), inf_dirguide::kTableName, lq_dirguide::kTableName),
	LqEnvTable(_T("GUIDE"), inf_guide::kTableName, lq_guide::kTableName),
	LqEnvTable(_T("INTERNAVI"), inf_internavi::kTableName, lq_internavi::kTableName),
	LqEnvTable(_T("INTERSECTION"), inf_intersection::kTableName, lq_intersection::kTableName),
	LqEnvTable(_T("LANE"), inf_lane::kTableName, lq_lane::kTableName),
	LqEnvTable(_T("ROUTE"), inf_route::kTableName, lq_route::kTableName),
	LqEnvTable(_T("TURNREG"), inf_turnreg::kTableName, lq_turnreg::kTableName),
	LqEnvTable(_T("UTURN"), inf_uturn::kTableName, lq_uturn::kTableName),
	LqEnvTable(_T("VICS"), inf_vics::kTableName, lq_vics::kTableName),
	LqEnvTable(_T(""), _T(""), _T(""))
};

///< ���̑��e�[�u���Ǘ��e�[�u��
EnvTable CLayerManage::m_OtherTable[] = {
	EnvTable(_T("�X��"), _T("SIMPLE_GRAD")),
	EnvTable(_T("���"), time_oneway::kTableName),
	EnvTable(_T("�ʍs�֎~"), time_nopassage::kTableName),
//	EnvTable(_T("�{�g���l�b�N"), _T("SEGMENT_BOTTLENECK")),
	EnvTable(_T(""), _T(""))
};

// ������
bool CLayerManage::init(const CString cConnectStr[2])
{
	if(!m_IsInitial){
		fnGetEnv();
		m_IsInitial = true;
	}
#ifdef STOP_POINT
	for(int i = 0; i < 2; ++i){
		m_ipWorkspace[i] = connectSDE( cConnectStr[i] );
		if(!m_ipWorkspace[i]){
			return false;
		}
	}
#else
		m_ipWorkspace[0] = connectSDE( cConnectStr[0] );
		if(!m_ipWorkspace[0]){
			return false;
		}
#endif

	return fnOpenSDELayers(m_IsInitial);
}

// SDE�ڑ��֐�
IWorkspacePtr CLayerManage::connectSDE(LPCTSTR lpszConnectStr)
{
	// �ڑ�
	_tprintf(_T("%s�ɐڑ���..."), lpszConnectStr);
	IWorkspacePtr ipWorkspace =  sindy::create_workspace(lpszConnectStr);
	if( NULL == ipWorkspace )
	{
		_tprintf(_T("[%s]�ɐڑ��ł��Ȃ������x��\n") , lpszConnectStr);
		_ftprintf(stderr, _T("[%s]�ɐڑ��ł��Ȃ������x��\n") , lpszConnectStr);
		return NULL;
	}
	_tprintf(_T("�ڑ����܂���\n"));
	_ftprintf(stderr, _T("#%s�ɐڑ�\n"), sindy::workspace_address(ipWorkspace).c_str());
	return ipWorkspace;
}

// SDE�̃e�[�u���E�t�B�[�`���N���X�i�����N��֘A�ȊO�̑S���j�̃I�[�v���Ɗe�폀��
bool CLayerManage::fnOpenSDELayers(bool cIsEdit)
{
	// ��{�t�B�[�`���N���X�̏���
	for(int i = 0; !m_FcEnvTable[i].m_OrgTable.IsEmpty(); i++){
#ifdef STOP_POINT
		//�ꎞ��~�ł́A�����N��m�[�h�̃o�[�W�����Ή��̏�Ԃ�����K�v���Ȃ�
		if(!fnOpenSDEFeatureClass(m_ipWorkspace[DB_ROAD], m_FcEnvTable[i], false)){
			return false;
		}
#else
		if(!fnOpenSDEFeatureClass(m_ipWorkspace[DB_ROAD], m_FcEnvTable[i], cIsEdit)){
			return false;
		}
#endif
	}

#ifdef STOP_POINT
	// �g���t�B�[�`���N���X�̏����i����ꎞ��~�̂݁j
	for(int i = 0; !m_ExtFcEnvTable[i].m_OrgTable.IsEmpty(); ++i){
		if(!fnOpenSDEFeatureClass(m_ipWorkspace[DB_STOP], m_ExtFcEnvTable[i], cIsEdit)){
			return false;
		}
	}
#else
	// �ꎞ��~�̑���ɁA�����N���e��e�[�u���̏���
	for(int i = 0; !m_LqEnvTable[i].m_OrgTables[0].IsEmpty(); i++){
		if(!fnOpenSDETables(m_ipWorkspace[DB_ROAD], m_LqEnvTable[i], cIsEdit)){
			return false;
		}
	}
	
	for(int i = 0; !m_OtherTable[i].m_OrgTable.IsEmpty(); i++){
		if(!fnOpenSDETable(m_ipWorkspace[DB_ROAD], m_OtherTable[i].m_SdeTable, m_OtherTable[i].m_SDEIndexMap, cIsEdit)){
			return false;
		}
	}
	// ���b�V���|���S���͂�����ƕʊǗ��i�t�B�[���h��񂪂���Ȃ����x�����ǁj
	if(!fnOpenMeshPoly()){
		return false;
	}

#endif
	////�ŏ������ł�����������Ȃ����ǁA�|�����疈�ڑ����Ɏ擾�i�m�[�h�͐�΂Ȃ��Ƃ����Ȃ��Ǝv������j
	if(m_TableInfo.find(road_node::kTableName) != m_TableInfo.end()){
		IGeoDatasetPtr ipDataset = m_TableInfo[road_node::kTableName].m_ipTable;
		if(FAILED(ipDataset->get_SpatialReference(&m_ipSpRef))){
			_ftprintf(stderr, _T("#ERROR,SpatialReference�擾���s,ROAD_NODE\n"));
			return false;
		}
		return true;
	}
	return false;
}

// ���ϐ�����ݒ�𔽉f
void CLayerManage::fnGetEnv(void)
{
	//�e�t�B�[�`���N���X��
	for(int i = 0; !m_FcEnvTable[i].m_OrgTable.IsEmpty(); ++i){
		CString aTmp = _T("FC_") + CString(m_FcEnvTable[i].m_OrgTable);
		LPCTSTR aEnv = _tgetenv(aTmp);
		if(aEnv){
			m_FcEnvTable[i].m_SdeTable = aEnv;
		}
		_ftprintf(stderr, _T("#�ύX��ݒ�F%s\n"), m_FcEnvTable[i].m_SdeTable);
	}
	for(int i = 0; !m_ExtFcEnvTable[i].m_OrgTable.IsEmpty(); ++i){
		CString aTmp = _T("FC_") + CString(m_ExtFcEnvTable[i].m_OrgTable);
		LPCTSTR aEnv = _tgetenv(aTmp);
		if(aEnv){
			m_ExtFcEnvTable[i].m_SdeTable = aEnv;
		}
		_ftprintf(stderr, _T("#�ύX��ݒ�F%s\n"), m_ExtFcEnvTable[i].m_SdeTable);
	}
	LPCTSTR aEnv = _tgetenv(_T("FC_BASEMESH"));
	if(aEnv != NULL){
		m_MeshLayer = aEnv;
	}
}

// SDE�̃t�B�[�`���N���X�̃I�[�v���Ɗe�폀���i1���j
bool CLayerManage::fnOpenSDEFeatureClass(const IFeatureWorkspacePtr& ipFeatureWorkspace, EnvTable& cEnv, bool cIsEdit)
{
	IFeatureClassPtr ipFeatureClass;
	if(FAILED(ipFeatureWorkspace->OpenFeatureClass(CComBSTR(cEnv.m_SdeTable), &ipFeatureClass))){
		_ftprintf(stderr, _T("#ERROR �t�B�[�`���[�N���X���J���܂���,%s\n"), cEnv.m_SdeTable);
		_tprintf(_T("ERROR �t�B�[�`���[�N���X���J���܂���,%s\n"), cEnv.m_SdeTable);
		return false;
	}
	//���f�ΏۂȂ�o�[�W�����Ή��ł��邱�Ƃ��K�v
	if(cIsEdit && !(g_Mode & NO_CHG_MODE)){
		//�o�[�W�����Ή����`�F�b�N
	    IVersionedObjectPtr ipVersionedObject =ipFeatureClass;
		if(ipVersionedObject){
			VARIANT_BOOL aIsVersioned;
			ipVersionedObject->get_IsRegisteredAsVersioned(&aIsVersioned);
			if(!aIsVersioned){
				_tprintf(_T("ERROR,�o�[�W�����Ή��ł͂Ȃ����߁A�X�V�ł��܂���\n"));
				_ftprintf(stderr, _T("#ERROR,�o�[�W�����Ή��ł͂Ȃ����߁A�X�V�ł��܂���\n"));
				return false;
			}
		}
	}
	fnRegistTable(ipFeatureClass, cEnv.m_OrgTable);	// ���ʂɂ̓^�O�Ƃ���m_OrgTable���g��
	return getFieldInfo(ipFeatureClass, cEnv.m_SDEIndexMap);
}

// SDE�̃����N��֘A�e�[�u���S���̃I�[�v���Ɗe�폀��
bool CLayerManage::fnOpenSDETables(const IFeatureWorkspacePtr& ipFeatureWorkspace, LqEnvTable& cEnv, bool cIsEdit)
{
	for(int i = 0; i < 2; ++i){
		if(!fnOpenSDETable(ipFeatureWorkspace, cEnv.m_SdeTables[i], cEnv.m_SDEIndexMap[i], cIsEdit)){
			return false;
		}
	}
	return true;
}

// SDE�̃����N��֘A�e�[�u���̃I�[�v���Ɗe�폀���i1���j
bool CLayerManage::fnOpenSDETable(const IFeatureWorkspacePtr& ipFeatureWorkspace, LPCTSTR cTableName, INDEX_MAP& cIndexMap, bool cIsEdit)
{
	ITablePtr ipTable;
	if(FAILED(ipFeatureWorkspace->OpenTable(CComBSTR(cTableName), &ipTable))){
		_ftprintf(stderr, _T("#ERROR �e�[�u�����J���܂���,%s\n"), cTableName);
		_tprintf(_T("ERROR �e�[�u�����J���܂���,%s\n"), cTableName);
		return false;
	}
	//���f�ΏۂȂ�o�[�W�����Ή��ł��邱�Ƃ��K�v
	if(cIsEdit && !(g_Mode & NO_CHG_MODE)){
		//�o�[�W�����Ή����`�F�b�N
	    IVersionedObjectPtr ipVersionedObject =ipTable;
		if(ipVersionedObject){
			VARIANT_BOOL aIsVersioned;
			ipVersionedObject->get_IsRegisteredAsVersioned(&aIsVersioned);
			if(!aIsVersioned){
				_tprintf(_T("ERROR,�o�[�W�����Ή��ł͂Ȃ����߁A�X�V�ł��܂���\n"));
				_ftprintf(stderr, _T("#ERROR,�o�[�W�����Ή��ł͂Ȃ����߁A�X�V�ł��܂���\n"));
				return false;
			}
		}
	}

	fnRegistTable(ipTable, cTableName);
	return getFieldInfo(ipTable,cIndexMap);
}
// �����R�s�[�Ώۂ̃t�B�[���h����SDE�̃t�B�[���h�C���f�b�N�X���擾
bool CLayerManage::getFieldInfo(const ITablePtr& ipTable, INDEX_MAP& cIndexMap)
{
	cIndexMap.clear();
	//�R�s�[�ɕK�v��SDE�̃t�B�[���h���ƃt�B�[���h�C���f�b�N�X���擾
	IFieldsPtr ipFields;
	ipTable->get_Fields(&ipFields);
	if(!ipFields){
		return false;
	}
	long aCount;
	ipFields->get_FieldCount(&aCount);
	for(int i = 0; i < aCount; ++i){
		IFieldPtr ipField;
		ipFields->get_Field(i, &ipField);
		esriFieldType aType;
		ipField->get_Type(&aType);
		if(aType == esriFieldTypeOID || aType == esriFieldTypeGeometry){
			//OID�ƌ`��͂���Ȃ��i�R�s�[�Ώۂɂ��Ȃ������ʈ����j
			continue;
		}
		CComBSTR abstrName;
		ipField->get_Name(&abstrName);
		CString aName = abstrName;
		//sindy::schema���S���啶���ł��邱�Ƃ�O��Ƃ��Ă���B���̑O�񂪕��ꂽ�琳��ɓ����Ȃ�
		aName.MakeUpper();	//��̌����̎��p�ɁA�O�̂��߁A�S���啶���ɂ���B
		if(aName.Find(_T("SHAPE.")) == 0 || aName == ipc_table::kSource){
			//SHAPE.LEN�ASHAPE.AREA�ASOURCE������
			continue;
		}
		CComBSTR aAlias;
		ipField->get_AliasName(&aAlias);

		cIndexMap[aName] = FieldInfo(i, CString(aAlias));
	}
	return true;
}

// �㏈���֐��i�S���j������SDE����ؒf����j
void CLayerManage::unInit(void)
{
	m_ipSpRef = NULL;
	for(TABLE_INFO::iterator aLayerInfo = m_TableInfo.begin(); aLayerInfo != m_TableInfo.end(); ++aLayerInfo){
		aLayerInfo->second.m_ipTable = NULL;
	}
	m_TableInfo.clear();
	m_ipWorkspace[DB_ROAD] = m_ipWorkspace[DB_STOP] = NULL;
}

// ���ʗp�e�[�u�����̂���Ή�����e�[�u�����擾
ITablePtr CLayerManage::findTable(LPCTSTR cLayerName) const
{
	//const�֐��ɂ��邽�߁A[]���Z�q���g��Ȃ��悤�ɂ��Ă݂�
	TABLE_INFO::const_iterator it = m_TableInfo.find(cLayerName);
	if(it != m_TableInfo.end()){
		return it->second.m_ipTable;
	}else{
		return NULL;
	}
}

// ���ʗp�e�[�u�����̂���Ή�������ۂ́i���[�U���t���Ƃ��́j�e�[�u�����̂��擾
LPCTSTR CLayerManage::findTableName(LPCTSTR cLayerName) const
{
	//const�֐��ɂ��邽�߁A[]���Z�q���g��Ȃ��悤�ɂ��Ă݂�
	TABLE_INFO::const_iterator it = m_TableInfo.find(cLayerName);
	if(it != m_TableInfo.end()){
		return it->second.m_LayerName;
	}else{
		return _T("�s���i�o�O�j");
	}
}

// �ҏW�i���f�j�Ώۂ̃��[�N�X�y�[�X���擾
IWorkspacePtr CLayerManage::getEditWorkspace(void) const
{
#ifdef STOP_POINT
	return m_ipWorkspace[DB_STOP];
#else
	return m_ipWorkspace[DB_ROAD];
#endif
}

// �e�[�u���̎�ޖ��ɍŏ��̏����Ώۂ̏����擾
EnvTable* CLayerManage::getFitstEnv(TABLE_TYPE cType)
{
	switch(cType){
		case FC_TABLE:
			m_FcIndex = 0;
			return &m_FcEnvTable[m_FcIndex];
			break;
		case EXT_FC_TABLE:
			m_ExtFcIndex = 0;
			return &m_ExtFcEnvTable[m_ExtFcIndex];
			break;
		case OTHER_TABLE:
			m_OtherIndex = 0;
			return &m_OtherTable[m_OtherIndex];
			break;
		default:
			break;
	}
	//�o�O���ĂȂ���΂����ɗ��Ȃ����ǂǂ����悤
	return NULL;
}

// �e�[�u���̎�ޖ��Ɏ��̏����Ώۂ̏����擾
EnvTable* CLayerManage::getNextEnv(TABLE_TYPE cType)
{
	switch(cType){
		case FC_TABLE:
			++m_FcIndex;
			return &m_FcEnvTable[m_FcIndex];
			break;
		case EXT_FC_TABLE:
			++m_ExtFcIndex;
			return &m_ExtFcEnvTable[m_ExtFcIndex];
			break;
		case OTHER_TABLE:
			++m_OtherIndex;
			return &m_OtherTable[m_OtherIndex];
			break;
		default:
			break;
	}
	//�o�O���ĂȂ���΂����ɗ��Ȃ����ǂǂ����悤
	return NULL;
}

// ���ʗp�e�[�u�����̂���Ή���������擾
EnvTable* CLayerManage::findEnvTable(LPCTSTR cLayerName) const
{
	for(int i = 0; !m_FcEnvTable[i].m_OrgTable.IsEmpty(); i++){
		if(m_FcEnvTable[i].m_OrgTable == cLayerName){
			return &m_FcEnvTable[i];
		}
	}
	return NULL;
}

// �����N��ōŏ��̏����Ώۂ̏����擾
LqEnvTable* CLayerManage::getFitstLQEnv(void)
{
	m_LqIndex = 0;
	return &m_LqEnvTable[m_LqIndex];
}

// �����N��Ŏ��̏����Ώۂ̏����擾
LqEnvTable* CLayerManage::getNextLQEnv(void)
{
	++m_LqIndex;
	return &m_LqEnvTable[m_LqIndex];
}

// ���b�V���|���S���I�[�v��
bool CLayerManage::fnOpenMeshPoly(void)
{
	IFeatureClassPtr ipFeatureClass;
	if(FAILED(((IFeatureWorkspacePtr)m_ipWorkspace[DB_ROAD])->OpenFeatureClass(CComBSTR(m_MeshLayer), &ipFeatureClass))){
		_ftprintf(stderr, _T("#ERROR ���b�V���t�B�[�`���[�N���X���J���܂���,%s\n"), m_MeshLayer);
		_tprintf(_T("ERROR ���b�V���t�B�[�`���[�N���X���J���܂���,%s\n"), m_MeshLayer);
		return false;
	}
	fnRegistTable(ipFeatureClass, basemesh::kTableName);
	return true;
}

// ���ʗp�e�[�u�����̂ƃe�[�u�����֘A�Â��ĊǗ��i�o�^�j
bool CLayerManage::fnRegistTable(const ITablePtr& ipTable, LPCTSTR cTableName)
{
	CComBSTR aLayerName;
	((IDatasetPtr)ipTable)->get_Name(&aLayerName);
	LayerInfo aLayerInfo;
	aLayerInfo.m_LayerName = aLayerName;
	aLayerInfo.m_ipTable = ipTable;
	m_TableInfo[cTableName] = aLayerInfo;
	return true;
}


