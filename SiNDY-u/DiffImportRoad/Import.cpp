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
#include <ArcHelperEx/GlobalFunctions.h>
#include "Import.h"

using namespace Reflect;
using namespace sindy::schema;

namespace {

	const long SHAPE_CHG = 0x01; //!< �`��ɕύX���������i�r�b�g�t���O�j
	const long ATTR_CHG  = 0x02; //!< �����ɕύX���������i�r�b�g�t���O�j
	const long BOTH_CHG  = SHAPE_CHG | ATTR_CHG; //!< �`��Ƒ����ɕύX���������i�V�K���܂ށj

	LPCTSTR CHG_STATUS_NAME     = _T("UPDATE_C");       //!< �X�V�X�e�[�^�X�̃t�B�[���h��
	LPCTSTR ORG_OBJID_NAME      = _T("ORG_OBJID");      //!< �I���W�i���̃I�u�W�F�N�gID�t�B�[���h��
	LPCTSTR ORG_MODIFYDATE_NAME = _T("ORG_MODIFYDATE"); //!< �I���W�i���̍ŏI�X�V���t�t�B�[���h��
	LPCTSTR ORG_OPERATOR_NAME   = _T("ORG_OPERATOR");   //!< �I���W�i���̕ҏW�Җ�

//	const double BUF_SIZE = 0.000001;	///< ����Ƃ݂Ȃ����̃o�b�t�@�T�C�Y

	// ���̃��C����ID���w���t�B�[���h�Q
	LPCTSTR REL_ID_TABLE[] = {
		category::link_relational_table::kLinkID,
		stop_point::kNodeID,
		category::from_to_node_relational_table::kToNodeID,
		category::from_to_node_relational_table::kFromNodeID,
		NULL
	};
}

// �R���X�g���N�^
CImport::CImport(CLayerManage* cLayerManage)
{
	m_LayerManage = cLayerManage;
	m_ExecInfo = _T("�����C���|�[�g");
}

// �f�X�g���N�^
CImport::~CImport(void)
{
}


// �������֐�
bool CImport::init(LPCTSTR cOldPath, LPCTSTR cNewPath)
{
	m_OldPath = cOldPath;
	m_NewPath = cNewPath;
	return true;
}

// ���b�V���P�ʂł̍����C���|�[�g�i���f�j�֐�
bool CImport::fnExecMesh(void)
{
	m_IDSets.clear();
	m_IDMaps.clear();
	m_OldTableInfo.clear();


	_ftprintf(stderr, _T("#%d\n"), g_CurrentMesh);
	IWorkspacePtr ipOldWorkspace = fnOpenLocalDatabase(m_OldPath);
	if(ipOldWorkspace == NULL){
		return false;
	}
	IWorkspacePtr ipNewWorkspace = fnOpenLocalDatabase(m_NewPath);
	if(ipNewWorkspace == NULL){
		return false;
	}
#ifdef STOP_POINT
	//���H��ʃt�B�[�`���N���X(�ҏW�͂��Ȃ���ID�̊֌W�擾����)
	for(EnvTable *aEnv = m_LayerManage->getFitstEnv(CLayerManage::FC_TABLE); ! aEnv->m_OrgTable.IsEmpty(); aEnv = m_LayerManage->getNextEnv(CLayerManage::FC_TABLE)){
		if(!fnReflectGeneral(ipOldWorkspace, ipNewWorkspace,  *aEnv, true)){
			return false;
		}
	}
	//���̑��i����ꎞ��~�̂݁j
	for(EnvTable *aEnv = m_LayerManage->getFitstEnv(CLayerManage::EXT_FC_TABLE); ! aEnv->m_OrgTable.IsEmpty(); aEnv = m_LayerManage->getNextEnv(CLayerManage::EXT_FC_TABLE)){
		if(!fnReflectGeneral(ipOldWorkspace, ipNewWorkspace,  *aEnv, false)){
			return false;
		}
	}
#else
	if(!fnGetMesh()){
		return false;
	}
	//���H��ʃt�B�[�`���N���X�i�ҏW����j
	//��Ƀm�[�h��ID�̊֌W���擾���Ă���i�����N���ɏ�������������j
	EnvTable *aEnvNode = m_LayerManage->findEnvTable(road_node::kTableName);
	if(aEnvNode){
		if(!fnReflectGeneral(ipOldWorkspace, ipNewWorkspace,  *aEnvNode, true)){
			return false;
		}
	}
	EnvTable *aEnvLink = m_LayerManage->findEnvTable(road_link::kTableName);
	if(aEnvLink){
		if(!fnReflectGeneral(ipOldWorkspace, ipNewWorkspace,  *aEnvLink, true)){
			return false;
		}
	}
#ifdef _DEBUG
//#define LQTEST�@//�����N��݂̂̃e�X�g���ɂ͒�`����
#endif
	for(EnvTable *aEnv = m_LayerManage->getFitstEnv(CLayerManage::FC_TABLE); ! aEnv->m_OrgTable.IsEmpty(); aEnv = m_LayerManage->getNextEnv(CLayerManage::FC_TABLE)){
#ifdef LQTEST
		if(!fnReflectGeneral(ipOldWorkspace, ipNewWorkspace,  *aEnv, true)){
#else
		if(!fnReflectGeneral(ipOldWorkspace, ipNewWorkspace,  *aEnv, false)){

#endif			return false;
		}
	}

	//�����N��
	for(LqEnvTable *aEnv = m_LayerManage->getFitstLQEnv(); ! aEnv->m_OrgTables[0].IsEmpty(); aEnv = m_LayerManage->getNextLQEnv()){
		if(!fnReflectLQ(ipOldWorkspace, ipNewWorkspace,  *aEnv)){
			return false;
		}
		// �����N��֘A�̃t���O�̃P�A���K�v
		fnLinkFlagCare(*aEnv);
	}

	//�����N��ȊO�̊e��e�[�u����
	for(EnvTable *aEnv = m_LayerManage->getFitstEnv(CLayerManage::OTHER_TABLE); ! aEnv->m_OrgTable.IsEmpty(); aEnv = m_LayerManage->getNextEnv(CLayerManage::OTHER_TABLE)){
		if(!fnReflectGeneral(ipOldWorkspace, ipNewWorkspace,  *aEnv, false)){
			return false;
		}
	}

#endif
	return true;
}

// ���C���P�ʂŕύX�𔽉f����֐�
// �ucGetIDOnly�v���utrue�v�̎���ID�̑Ή����擾���邾���Ŕ��f�͂��Ȃ�
bool CImport::fnReflectGeneral(const IWorkspacePtr& ipOldWorkspace, const IWorkspacePtr& ipNewWorkspace, EnvTable& cEnv, bool cGetIDOnly)
{
	//��Ɨp�t�B�[�`���N���X�um_ipTable�v�ɃZ�b�g�i����T���̂��ʓ|�Ȃ̂Łj
	m_ipTable[0] =  m_LayerManage->findTable(cEnv.m_OrgTable);
	m_AppendID.clear();
	ITablePtr ipOldTable = fnOpenLocalTable(ipOldWorkspace, cEnv.m_OrgTable);
	if(!ipOldTable){
		return false;
	}

	ITablePtr ipNewTable = fnOpenLocalTable(ipNewWorkspace, cEnv.m_OrgTable);
	if(!ipNewTable){
		return false;
	}
	CComBSTR aLayerName;
	((IDatasetPtr)ipOldTable)->get_Name(&aLayerName);
	LayerInfo aLayerInfo;
	aLayerInfo.m_LayerName = aLayerName;
	aLayerInfo.m_ipTable = ipOldTable;
	m_OldTableInfo[cEnv.m_OrgTable] = aLayerInfo;

	//�t�B�[���h�C���f�b�N�X�̎擾
	CLayerManage::getFieldInfo(ipNewTable, cEnv.m_PGDBIndexMap);

	//�ʏ퍷��
	if(cGetIDOnly){
		_tprintf(_T("%s,ID�̑Ή����擾���Ă��܂�\n"), cEnv.m_OrgTable);
	}else{
		_tprintf(_T("%s,��r�Ɣ��f�����Ă��܂��i�ύX�E�폜�j\n"), cEnv.m_OrgTable);
	}
	if(!fnCompare(ipOldTable, ipNewTable, cEnv, cGetIDOnly)){	//��r�ƕύX�E�폜�i�|���S���Ƃ���ȊO�̏����͓����ŕ����Ă���j
		return false;
	}

	if(!cGetIDOnly){
		_tprintf(_T("%s,�V�K����ǉ����Ă܂�\n"), cEnv.m_OrgTable);
		fnAddNew(ipNewTable, cEnv);	//�V�K�ǉ�
	}

	//�O�̂��ߖ����I�ɔj��
	ipNewTable = NULL;
	return true;
}

// PGDB�I�[�v���֐�
IWorkspacePtr CImport::fnOpenLocalDatabase(LPCTSTR cBasePath) const
{
	CString aDatabase;
	aDatabase.Format(_T("%s\\%04d\\%d.mdb"), cBasePath,  g_CurrentMesh/100,  g_CurrentMesh); 

	// �p�[�\�i���W�IDB
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
	IWorkspacePtr ipWorkspace;

	// �I�[�v�����Ă݂�
	ipWorkspaceFactory->OpenFromFile( CComBSTR( aDatabase ), 0, &ipWorkspace );
	if( ipWorkspace == NULL )
	{
		_ftprintf( stderr, _T("#ERROR,%s ���J���܂���ł���\n"), aDatabase);
		return NULL;
	}
	return ipWorkspace;
}

// PGDB���̎w�背�C�����̃e�[�u���i�t�B�[�`���N���X�j�I�[�v���֐�
ITablePtr CImport::fnOpenLocalTable(const IWorkspacePtr& ipWorkspace, LPCTSTR cLayerName) const
{
	ITablePtr ipTable;
	if(IFeatureWorkspacePtr(ipWorkspace)->OpenTable(CComBSTR(cLayerName), &ipTable) != S_OK){
		_ftprintf(stderr, _T("#ERROR,�e�[�u�����J���Ȃ�,%s\n"), cLayerName);
		return NULL;
	}
	return ipTable;
}

// ��r�ƕύX�E�폜�֐�
bool CImport::fnCompare(const ITablePtr& ipOldTable, const ITablePtr& ipNewTable, EnvTable& cEnv, bool cGetIDOnly)
{
	//�|���S���̎���������������ς��邽�ߒ��ׂĂ���
	esriGeometryType aType = esriGeometryNull;
	IFeatureClassPtr ipFeatureClass = m_ipTable[0];
	if(ipFeatureClass){
		ipFeatureClass->get_ShapeType(&aType);
	}
	_ICursorPtr ipCursor;
#ifdef STOP_POINT
	//1���b�V�������o���Ă��Ȃ��̂ŁA�t�@�C�����S�����Ώ�
	if(FAILED(ipOldTable->Search(NULL, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return false;
	}
#else
	//�b��΍�ŗא�9���b�V���o���Ă���̂ŁA�����b�V��������ΏۂƂ���
	//�����ɂ͊O����ڂ���̂���������Ă��܂����A�^�p�ł͋��E���͂�����Ȃ����ƂɂȂ��Ă���̂ŁA
	//�����������������Ŏ��Q�͂Ȃ��͂�
	IQueryFilterPtr ipFilter;
	if(aType != esriGeometryNull){
		//�t�B�[�`���N���X�Ȃ��Ԍ���
		CComBSTR          bstrFieldName;
		ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
		ipFilter = AheInitSpatialFilter( (ISpatialFilterPtr)AheInitQueryFilter( NULL, bstrFieldName ), m_ipMeshPoly, bstrFieldName );
	}else{
		//���b�V���R�[�h�������Ă��玩���b�V���A����ȊO�͑S��
		if(cEnv.m_PGDBIndexMap.find(category::mesh_code_table::kMeshCode) != cEnv.m_PGDBIndexMap.end()){
			ipFilter = AheInitQueryFilter(NULL, NULL, _T("%s = %d"), category::mesh_code_table::kMeshCode, g_CurrentMesh);
		}
	}

	if(FAILED(ipOldTable->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return false;
	}
#endif
	ID_MAP aIDMap;	///< PGDB��SDE��OID�̊֌W�i��ŎQ�Ƃ��Ă���ID�̐U��ւ��Ŏg�p�j
	CHG_ID aChgID;	///< ���f�E�s��ID�i���v���O�������ŕύX�������́A���f���Ă͂����Ȃ����̂̔���Ŏg�p�j
	ID_SET_MAP::iterator it = m_IDSets.find(cEnv.m_OrgTable);
	if(it != m_IDSets.end()){
		aChgID = it->second;
	}
	_IRowPtr ipOldRow;
	while(ipCursor->NextRow(&ipOldRow) == S_OK && ipOldRow){
		long aObjID;
		ipOldRow->get_OID(&aObjID);
#ifdef FOR_BUG 
		m_ExistID.insert(aObjID);	//ORG_OBJID���p���������p
#endif

		CComVariant aOrgID;
		ipOldRow->get_Value(cEnv.m_PGDBIndexMap[ORG_OBJID_NAME].m_Index, &aOrgID);
		aIDMap[aObjID] = aOrgID.lVal;	//PGDB��ID�ɑΉ�����SDE��ID��ێ�
		if(cGetIDOnly){
			continue;
		}
		//�����ς݂̃I�u�W�F�N�g������ID�ŊǗ��i�r���Ń}���`�p�[�g�ɂȂ������̂��߂Ȃ̂ŁA�|���S���̎������Ӗ��Ȃ��͂��j
		if(aChgID.m_OK.find(aObjID) != aChgID.m_OK.end()){
			continue;
		}

		//���f�[�^�P�ʂŖ�������������i���b�V�����j
		_IRowPtr ipNewRow;
		ipNewTable->GetRow(aObjID, &ipNewRow);
		if(ipNewRow){
			//�ύX���`�F�b�N
			CComVariant aUpdate;
			ipNewRow->get_Value(cEnv.m_PGDBIndexMap[CHG_STATUS_NAME].m_Index, &aUpdate);
			if(aUpdate.vt != VT_NULL && aUpdate.lVal != 0){
				if(aChgID.m_NG.find(aObjID) != aChgID.m_NG.end())
				{
					//���̊֘A�I�u�W�F�N�g�̉e���ŕύX�s�ɂȂ��Ă�����̂̓��b�Z�[�W��\�����Ĕ��f���Ȃ�(���������N�̉e���Ńm�[�h���ύX�ł��Ȃ��ꍇ�����̂͂�)
					fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�֘A�I�u�W�F�N�g�ɏ����ΏۊO���܂܂�Ă���i�ύX�j"), road_link::kTableName, PGDB_LAYER);
					continue;
				}
				if(aType != esriGeometryPolygon && (fnMeshRelObject(ipNewRow) || fnMeshRelObject(ipOldRow))){
					//���b�V�����E�Ɋ֌W������̂̓��b�Z�[�W��\�����Ĕ��f���Ȃ�(�|���S���͐؂�\�肷�邩�狖��)
					fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("���b�V�����E���͔͈͊O�̃I�u�W�F�N�g�Ȃ̂Ŕ��f���Ȃ�"), NULL, PGDB_LAYER);
					aChgID.m_NG.insert(aObjID);
					fnProhibitRelIDAll(ipNewRow, cEnv.m_PGDBIndexMap);
					continue;
				}
				//�Ⴂ����������X�V
				if(!fnSiNDYChange(cEnv, aOrgID.lVal, (_IRowPtr)NULL, ipOldRow, _T("�ύX"), m_ipTable )){
					//SiNDY��ō폜����Ă�����̂́A���̐�͖���
					//�����D��̎������̃I�u�W�F�N�g��
					aChgID.m_NG.insert(aObjID);
					fnProhibitRelIDAll(ipNewRow, cEnv.m_PGDBIndexMap);
					continue;
				}
				if(aType != esriGeometryPolygon || !fnOverMesh(cEnv, aOrgID.lVal)){
					//�X�V�O�I�u�W�F�N�g�iSiNDY��Ń`�F�b�N�j�����b�V�����܂����ł��Ȃ�������
					//���E��ł���������ڂ��Ă��邾���Ȃ�OK
					//�|���S���łȂ����OK
					if(fnReflectToSiNDY(ipNewRow, cEnv, aOrgID.lVal, aUpdate.lVal)){
						aChgID.m_OK.insert(aObjID);
					}else{
						aChgID.m_NG.insert(aObjID);
						fnProhibitRelIDAll(ipNewRow, cEnv.m_PGDBIndexMap);
					}
				}else{
					//�����������b�V���ɂ܂�����I�u�W�F�N�g��������؂�\��
					if(fnClipAndApend(ipNewTable, cEnv, aOrgID.lVal, ipNewRow, aUpdate.lVal)){
						aChgID.m_OK.insert(aObjID);
					}else{
						aChgID.m_NG.insert(aObjID);
						fnProhibitRelIDAll(ipNewRow, cEnv.m_PGDBIndexMap);
					}
				}
			}
		}else{
			// �yBug9997�z�ʏ펞�͍폜�ł��Ȃ��悤�C��
#ifndef STOP_POINT
			// �ʏ펞�̍폜�́A�폜���s�����Ƃ���i�������A�`������̂݁j
			if( aType != esriGeometryNull ){
				aChgID.m_NG.insert(aObjID);
				fnProhibitRelIDAll(ipOldRow, cEnv.m_PGDBIndexMap);
				continue;
			}
#endif
			//�폜���ꂽ
			if(aChgID.m_NG.find(aObjID) != aChgID.m_NG.end())
			{
				//���̊֘A�I�u�W�F�N�g�̉e���ŕύX�s�ɂȂ��Ă�����̂̓��b�Z�[�W��\�����Ĕ��f���Ȃ�(���������N�̉e���Ńm�[�h���ύX�ł��Ȃ��ꍇ�����̂͂�)
				fnPrintLogStyre(2, cEnv.m_OrgTable, aOrgID.lVal, _T("�֘A�I�u�W�F�N�g�ɏ����ΏۊO���܂܂�Ă���i�폜�j"), road_link::kTableName, TGT_LAYER);
				continue;
			}
			if(aType != esriGeometryPolygon && fnMeshRelObject(ipOldRow)){
				//���b�V�����E�Ɋ֌W������̂̓��b�Z�[�W��\�����Ĕ��f���Ȃ�(�|���S���͐؂�\�肷�邩�狖�����A�ʏ�͂��肦�Ȃ�)
				fnPrintLogStyre(2, cEnv.m_OrgTable, aOrgID.lVal, _T("���b�V�����E���͔͈͊O�̃I�u�W�F�N�g�Ȃ̂ō폜���Ȃ�"), NULL, TGT_LAYER);
				aChgID.m_NG.insert(aObjID);
				fnProhibitRelIDAll(ipOldRow, cEnv.m_PGDBIndexMap);
				continue;
			}
			_IRowPtr ipRow;
			m_ipTable[0]->GetRow(aOrgID.lVal, &ipRow);
			if(ipRow){
				if(fnSiNDYChange(cEnv, aOrgID.lVal, ipRow, ipOldRow, _T("�폜"), m_ipTable )){
					if(aType != esriGeometryPolygon || !fnOverMesh(cEnv, aOrgID.lVal, ipRow)){
						//�X�V�O�I�u�W�F�N�g�iSiNDY��Ń`�F�b�N�j�����b�V�����܂����ł��Ȃ�������
						//���E��ł���������ڂ��Ă��邾���Ȃ�OK
						//�|���S���łȂ����OK
						if(!(g_Mode & NO_CHG_MODE)){
							if(FAILED(ipRow->Delete())){
								aChgID.m_NG.insert(aObjID);
								fnProhibitRelIDAll(ipOldRow, cEnv.m_PGDBIndexMap);
								fnPrintComError();
								fnPrintLogStyre(2, cEnv.m_OrgTable, aOrgID.lVal, _T("�폜���s"), NULL, TGT_LAYER);
							}else{
								aChgID.m_OK.insert(aObjID);
								fnPrintLogStyre(0, cEnv.m_OrgTable, aOrgID.lVal, _T("�폜"), NULL, FIX_LAYER);
							}
						}else{
							aChgID.m_OK.insert(aObjID);
							fnPrintLogStyre(0, cEnv.m_OrgTable, aOrgID.lVal, _T("�폜�����"), NULL, TGT_LAYER);
						}
					}else{
						//�����������b�V���ɂ܂�����I�u�W�F�N�g��������؂�\��
						if(fnClipAndApend(ipNewTable, cEnv, aOrgID.lVal, NULL, 0)){
							aChgID.m_OK.insert(aObjID);
						}else{
							aChgID.m_NG.insert(aObjID);
							fnProhibitRelIDAll(ipOldRow, cEnv.m_PGDBIndexMap);
						}
					}
				}else{
					//�폜���Ă͂����Ȃ�
					aChgID.m_NG.insert(aObjID);
					fnProhibitRelIDAll(ipOldRow, cEnv.m_PGDBIndexMap);
				}
			}else{
				fnPrintLogStyre(1, cEnv.m_OrgTable, aOrgID.lVal, _T("���ɍ폜����Ă���"), NULL, FIX_LAYER);
				aChgID.m_NG.insert(aObjID);
				fnProhibitRelIDAll(ipOldRow, cEnv.m_PGDBIndexMap);
			}
		}
	}

	//�t�@�C�����S����ID�֌W���擾����i�K�v�����肻���H�j
	if(FAILED(ipOldTable->Search(NULL, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return false;
	}
	while(ipCursor->NextRow(&ipOldRow) == S_OK && ipOldRow){
		long aObjID;
		ipOldRow->get_OID(&aObjID);
		CComVariant aOrgID;
		ipOldRow->get_Value(cEnv.m_PGDBIndexMap[ORG_OBJID_NAME].m_Index, &aOrgID);
		aIDMap[aObjID] = aOrgID.lVal;	//PGDB��ID�ɑΉ�����SDE��ID��ێ�
	}

	m_IDMaps[cEnv.m_OrgTable] = aIDMap;
	m_IDSets[cEnv.m_OrgTable] = aChgID;
	return true;
}

// �V�K�ǉ������ꊇ�ǉ��֐�
bool CImport::fnAddNew(const ITablePtr& ipTable, EnvTable& cEnv)
{
	//�|���S���̎���������������ς��邽�ߒ��ׂĂ���
	esriGeometryType aType = esriGeometryNull;
	IFeatureClassPtr ipFeatureClass = m_ipTable[0];
	if(ipFeatureClass){
		ipFeatureClass->get_ShapeType(&aType);
	}

	IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, NULL, _T("%s IS NULL OR %s = 0"), ORG_OBJID_NAME, ORG_OBJID_NAME);

	_ICursorPtr ipCursor;
	if(FAILED(ipTable->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return false;
	}

	IRowBufferPtr ipBuffer;
	_ICursorPtr ipInsCursor;
	if(!( g_Mode & NO_CHG_MODE)){
		if(FAILED(m_ipTable[0]->Insert(VARIANT_TRUE, &ipInsCursor))){
			fnPrintComError();
			return false;
		}
		m_ipTable[0]->CreateRowBuffer(&ipBuffer);
	}

	ID_MAP& aIDMap = m_IDMaps[cEnv.m_OrgTable];
	CHG_ID& aChgID = m_IDSets[cEnv.m_OrgTable];
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow){
		long aObjID;
		ipRow->get_OID(&aObjID);

		// �yBug9997�z�ʏ펞�͒ǉ��ł��Ȃ��悤�C��
#ifndef STOP_POINT
		// �ǉ����s�����Ƃ���i�������A�`������̂݁j
		if( aType != esriGeometryNull ){	
			aChgID.m_NG.insert(aObjID);
			fnProhibitRelIDAll(ipRow, cEnv.m_PGDBIndexMap);
			continue;
		}
#endif

		//�G���[���b�Z�[�W�Ƀ|�C���g���g������A���߂�
		IGeometryPtr ipGeom;
		IFeaturePtr ipFeature = ipRow;
		if(ipFeature){
			ipFeature->get_ShapeCopy(&ipGeom);
		}
#ifdef STOP_POINT
		//����ꎞ��~�ł̂ݎg�p�B�����m�[�h�E�����N�̑g����������ǉ����Ȃ�
		if(fnExistSamePoint(ipRow, cEnv)){
			continue;
		}
		//����ꎞ��~�ł̂ݎg�p�B�����m�[�h�E�����N�̑g����������ǉ����Ȃ� �����܂�
#else
		// �����N�������E�K���n���A����ƌ��Ȃ�����̂���������ǉ����Ȃ�
		if(fnExistSameObj(ipRow, cEnv)){
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("����Ƃ݂Ȃ�����̂����ɑ��݂��܂�"), NULL, PGDB_LAYER, ipGeom);
			continue;
		}
#endif		

		if(aChgID.m_NG.find(aObjID) != aChgID.m_NG.end())
		{
			//���̊֘A�I�u�W�F�N�g�̉e���ŕύX�s�ɂȂ��Ă�����̂̓��b�Z�[�W��\�����Ĕ��f���Ȃ�(���������N�̉e���Ńm�[�h���ύX�ł��Ȃ��ꍇ�����̂͂�)
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�֘A�I�u�W�F�N�g�ɏ����ΏۊO���܂܂�Ă���i�ǉ��j"), road_link::kTableName, PGDB_LAYER, ipGeom);
			continue;
		}

		if(fnMeshRelObject(ipRow)){
			//���b�V�����E�Ɋ֌W������̂̓��b�Z�[�W��\�����Ĕ��f���Ȃ�
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("���b�V�����E���͔͈͊O�̃I�u�W�F�N�g�Ȃ̂Œǉ����Ȃ�"), NULL, PGDB_LAYER, ipGeom);
			aChgID.m_NG.insert(aObjID);
			fnProhibitRelIDAll(ipRow, cEnv.m_PGDBIndexMap);
			continue;
		}
		//�֘A���郊���N��m�[�h�̍폜�E�`��ω�������
		CString aRelLayer;
		if(!fnRelObjsChange(ipRow, cEnv.m_PGDBIndexMap, cEnv.m_OrgTable, ipGeom, aRelLayer)){
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�֘A�I�u�W�F�N�g�ɏ����ΏۊO���܂܂�Ă���i�ǉ��j"), aRelLayer, PGDB_LAYER, ipGeom);
			aChgID.m_NG.insert(aObjID);
			fnProhibitRelIDAll(ipRow, cEnv.m_PGDBIndexMap);
			continue;
		}

		if(!( g_Mode & NO_CHG_MODE)){
			if(ipFeature){
				((IFeatureBufferPtr)ipBuffer)->putref_Shape(ipGeom);
			}
			CString aDummy;
			fnReflectAttr((_IRowPtr)ipBuffer, ipRow, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, aDummy);
			fnSetCommonAttr((_IRowPtr)ipBuffer, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, NULL);
			CComVariant aID = 0;
			if(SUCCEEDED(ipInsCursor->InsertRow(ipBuffer, &aID))){
				aIDMap[aObjID] = aID.lVal;
				aChgID.m_OK.insert(aObjID);
				fnPrintLogStyre(0, cEnv.m_OrgTable, aID.lVal, _T("�ǉ�"), NULL, TGT_LAYER, ipGeom);
			}else{
				aChgID.m_NG.insert(aObjID);
				fnProhibitRelIDAll(ipRow, cEnv.m_PGDBIndexMap);
				fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�ǉ����s"), NULL, PGDB_LAYER, ipGeom);
			}
		}else{
			//�ύX����p�Ƀ_�~�[ID���Z�b�g
			aIDMap[aObjID] = -1;
			aChgID.m_OK.insert(aObjID);
			fnPrintLogStyre(0, cEnv.m_OrgTable, aObjID, _T("�ǉ������"), NULL, PGDB_LAYER, ipGeom);
		}
	}

	if(!( g_Mode & NO_CHG_MODE)){
		if(FAILED(ipInsCursor->Flush())){
			fnPrintComError();
			return false;
		}
	}
	return true;
}

// Export���SiNDY��ɕύX��������ꂽ���A�X�V���t2��̔�r�Ŕ��f����֐��i���������p�j
// �֐����Ɩ߂�l���t���ۂ��C�����邪�A�X�V�ł��Ȃ��E�X�V���Ă͂����Ȃ�����false��Ԃ�
bool CImport::fnSiNDYChange(LqEnvTable& cEnv, long cTblType, long cObjID, _IRowPtr& ipOrgRow, const _IRowPtr& ipPgdbRow, LPCTSTR cChgType, const ITablePtr ipTable[2])
{
	return fnSiNDYChange(cEnv.m_PGDBIndexMap[cTblType], cEnv.m_SDEIndexMap[cTblType], cEnv.m_OrgTables[cTblType], cObjID, ipOrgRow, ipPgdbRow, cChgType, ipTable[cTblType] );
}

// Export���SiNDY��ɕύX��������ꂽ���A�X�V���t2��̔�r�Ŕ��f����֐��i�{�́j
bool CImport::fnSiNDYChange(INDEX_MAP& cPgdbIndexMap, INDEX_MAP& cSdeIndexMap, const CString& cTableName, long cObjID, _IRowPtr& ipOrgRow, const _IRowPtr& ipPgdbRow, LPCTSTR cChgType, const ITablePtr& ipTable )
{
	long aObjID = 0;
	ipPgdbRow->get_OID(&aObjID);
	//Export��ɕύX�����������`�F�b�N
	if(!ipOrgRow){
		_IRowPtr ipRow;
		ipTable->GetRow(cObjID, &ipRow);
		if(!ipRow){
			fnPrintLogStyre(2, cTableName, aObjID, _T("�ύX�Ώۂ�SiNDY��ō폜����Ă���"), NULL, PGDB_LAYER);
			return false;
		}
		ipOrgRow = ipRow;
	}
	IGeometryPtr ipGeom;
	IFeaturePtr ipFeature = ipOrgRow;
	if(ipFeature){
		//�t�B�[�`���Ȃ�`��擾�i�G���[���b�Z�[�W�p�j
		ipFeature->get_Shape(&ipGeom);
	}
	CComVariant aSindyModDate;
	ipOrgRow->get_Value(cSdeIndexMap[ipc_table::kModifyDate].m_Index, &aSindyModDate);
	CComVariant aPgdbModDate;
	ipPgdbRow->get_Value(cPgdbIndexMap[ORG_MODIFYDATE_NAME].m_Index, &aPgdbModDate);
	//PGDB�̍X�V���t��NULL����Ȃ���������t������r�ANULL��������S������r�i���LQ�΍�j
	if((aPgdbModDate.vt != VT_NULL && aSindyModDate.date != aPgdbModDate.date) || (aPgdbModDate.vt == VT_NULL && fnIsAttrChange(ipOrgRow, cSdeIndexMap, ipPgdbRow, cPgdbIndexMap))){
		if( g_Mode & TDC_MODE){
			//�����D�悾������A���ɍs������_��
			CString aMsg = CString(_T("�ύX�Ώۂ�SiNDY��ŕύX����Ă���(")) + CString(cChgType) + _T(")");
			fnPrintLogStyre(2, cTableName, aObjID, aMsg, NULL, PGDB_LAYER);
			return false;
		}else{
			//�ʏ�̓��b�Z�[�W����
			fnPrintLogStyre(2, cTableName, aObjID, _T("�ύX�Ώۂ�SiNDY��ŕύX����Ă���"), NULL, PGDB_LAYER);
		}
	}

	//�֘A���郊���N��m�[�h�̍폜�E�`��ω�������
	CString aRelLayer;
	if(!fnRelObjsChange(ipPgdbRow, cPgdbIndexMap, cTableName, ipGeom, aRelLayer)){
		fnPrintLogStyre(2, cTableName, aObjID, _T("�֘A�I�u�W�F�N�g�ɏ����ΏۊO���܂܂�Ă���i�ύX�j"), aRelLayer, PGDB_LAYER);
		return false;
	}

	// ���H�����N�̏ꍇ�A�֘A�����N�񂪍X�V�\������i�����N���X�V������A�����N�񂪍X�V�ł��Ȃ��P�[�X�̉���j
	if( cTableName == road_link::kTableName ){

		// PGDB�̃��[�N�X�y�[�X�擾
		IWorkspacePtr ipOldWorkspace = fnOpenLocalDatabase(m_OldPath);
		if(!ipOldWorkspace){
			return false;
		}
		IWorkspacePtr ipNewWorkspace = fnOpenLocalDatabase(m_NewPath);
		if(!ipNewWorkspace){
			return false;
		}

		// �S�Ẵ����N��Ŏ��{
		for(LqEnvTable *env = m_LayerManage->getFitstLQEnv(); ! env->m_OrgTables[0].IsEmpty(); env = m_LayerManage->getNextLQEnv()){

			ITablePtr ipSdeTable[2];
			ITablePtr ipOldTable[2];
			ITablePtr ipNewTable[2];

			// Inf/Lq�̃e�[�u����V�����ꂼ��Ŏ擾
			for(int i = 0; i < 2; ++i){
				ipSdeTable[i] = m_LayerManage->findTable(env->m_OrgTables[i]);
				ipOldTable[i] = fnOpenLocalTable(ipOldWorkspace, env->m_OrgTables[i]);
				if(!ipOldTable[i]){
					return false;
				}

				ipNewTable[i] = fnOpenLocalTable(ipNewWorkspace, env->m_OrgTables[i]);
				if(!ipNewTable[i]){
					return false;
				}

				//�t�B�[���h�C���f�b�N�X�̎擾
				CLayerManage::getFieldInfo(ipNewTable[i], env->m_PGDBIndexMap[i]);
			}

			// �����N��ID����A���g�̊֘AInf/Lq���X�V�\���m�F
			if( !fnCanRelInfLqUpdate( ipOldTable, ipNewTable, ipSdeTable, *env, aObjID )){
				fnPrintLogStyre(2, cTableName, aObjID, _T("�֘A�����N�񂪍X�V�s��"), NULL, PGDB_LAYER);
				return false;
			}
		}
	}

	return true;
}

// Export���SiNDY��ɕύX��������ꂽ���A�X�V���t2��̔�r�Ŕ��f����֐��i���������p�j
bool CImport::fnSiNDYChange(EnvTable& cEnv, long cObjID, _IRowPtr& ipOrgRow, const _IRowPtr& ipPgdbRow, LPCTSTR cChgType, const ITablePtr ipTable[2] )
{
	return fnSiNDYChange(cEnv.m_PGDBIndexMap, cEnv.m_SDEIndexMap, cEnv.m_OrgTable, cObjID, ipOrgRow, ipPgdbRow, cChgType, ipTable[0] );
}

// ���b�V�����܂������I�u�W�F�N�g���𔻒肷��֐��i�|���S���p�j
bool CImport::fnOverMesh(EnvTable& cEnv, long cObjID, IFeaturePtr ipFeature) const
{
	if(ipFeature == NULL){
		((IFeatureClassPtr)m_ipTable[0])->GetFeature(cObjID, &ipFeature);
		if(ipFeature == NULL){
			//fnSiNDYChange()�Œe�����炱���ɂ͗��Ȃ��͂�
			fnPrintLogStyre(3, cEnv.m_OrgTable, cObjID, _T("�ύX�Ώۂ�SiNDY��ō폜����Ă���i�o�Ă͂����Ȃ����b�Z�[�W�j"), NULL, FIX_LAYER);
			return false;
		}
	}
	IGeometryPtr ipGeom = fnGetClipShape(ipFeature, cEnv, cObjID);
	if(ipGeom == NULL){
		return false;
	}
	IPointCollectionPtr ipPoints(ipGeom);
	long aCount;
	ipPoints->get_PointCount(&aCount);

	if(aCount > 3){ //�|���S���Ƃ��Đ������邽�߂ɂ�4�_�ȏ�K�v
		return true;
	}
	return false;
}

// ���b�V�����܂����I�u�W�F�N�g��؂�\�肵�čX�V����֐�
bool CImport::fnClipAndApend(const IFeatureClassPtr& ipNewClass, EnvTable& cEnv, long cObjID, const IFeaturePtr& ipNewFeature, long cStatus)
{
	long aPgdbID;	///< PGDB��ID�i���O�p�j
	ipNewFeature->get_OID(&aPgdbID);

	//�����ɗ���̂̓|���S���̎�����
	IFeaturePtr ipFeature;
	if(FAILED(((IFeatureClassPtr)m_ipTable[0])->GetFeature(cObjID, &ipFeature))){
		fnPrintComError();
		return false;
	}
	if(!ipFeature){
		fnPrintLogStyre(3, cEnv.m_OrgTable, cObjID, _T("�t�B�[�`���[��������Ȃ�(�o�Ă͂����Ȃ�)"), NULL, TGT_LAYER);
		return false;
	}

	//�؂�\�菈��
	IGeometryPtr ipGeom = fnGetRemakeShape(ipNewClass, ipFeature, cEnv, cObjID);
	if(ipGeom == NULL){
		fnPrintLogStyre(2, cEnv.m_OrgTable, cObjID, _T("���b�V�����E�؂�\�莸�s"), NULL, TGT_LAYER);
		return false;
	}
	IGeometryCollectionPtr ipGeoCol = ipGeom;
	long aCount;
	if(FAILED(ipGeoCol->get_GeometryCount(&aCount))){
		fnPrintComError();
		return false;
	}
	if(aCount > 1){
		msi_get_parts_info aPolUtil;
		if(aPolUtil.Init(ipGeom, cObjID, (string)CT2A(m_LayerManage->findTableName(cEnv.m_OrgTable)).m_psz, 0, stderr)){
			aCount = aPolUtil.Get_Part_Num();
			if(aCount > 1){
				cStatus = 0;	//�����̕ύX�͔��f�����Ȃ�����
				fnPrintLogStyre(1, cEnv.m_OrgTable, cObjID, _T("�č\���㕡���ɕ����ꂽ"), NULL, TGT_LAYER);
			}
		}else{
			fnPrintLogStyre(2, cEnv.m_OrgTable, aPgdbID, _T("�č\����|���S���̕��͎��s"), NULL, PGDB_LAYER);
			return false;
		}
	}else if(aCount < 1){
		if(ipNewFeature == NULL || cStatus & ATTR_CHG){
			//�P���폜�ŉ�
			if(FAILED(ipFeature->Delete())){
				fnPrintComError();
				fnPrintLogStyre(2, cEnv.m_OrgTable, aPgdbID, _T("���b�V�����E�폜���s"), NULL, PGDB_LAYER);
				return false;
			}else{
				fnPrintLogStyre(0, cEnv.m_OrgTable, cObjID, _T("���b�V�����E�ō폜"), NULL, TGT_LAYER);
				return true;
			}
		}else{
			fnPrintLogStyre(2, cEnv.m_OrgTable, aPgdbID, _T("�؂�\���̌`�󂪕s��"), NULL, PGDB_LAYER);
			return false;
		}
	}
	ipFeature->putref_Shape(ipGeom);
	fnSetCommonAttr((_IRowPtr)ipFeature, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, ipNewFeature);
	if(!(g_Mode & NO_CHG_MODE)){
		if(FAILED(ipFeature->Store())){
			fnPrintComError();
			fnPrintLogStyre(2, cEnv.m_OrgTable, aPgdbID, _T("���b�V�����E�ύX���s"), NULL, PGDB_LAYER);
			return false;
		}
		fnPrintLogStyre(0, cEnv.m_OrgTable, cObjID, _T("���b�V�����E�ύX"), NULL, TGT_LAYER);
	}else{
		fnPrintLogStyre(0, cEnv.m_OrgTable, cObjID, _T("���b�V�����E�ύX�����"), NULL, TGT_LAYER);
	}
	return true;
}

// ���b�V�����܂����ł��Ȃ����̂̒P���X�V�֐�
bool CImport::fnReflectToSiNDY(const _IRowPtr& ipNewRow, EnvTable& cEnv, long cObjID, long cStatus)
{
	_IRowPtr ipRow;
	if(FAILED(m_ipTable[0]->GetRow(cObjID, &ipRow))){
		fnPrintComError();
		return false;
	}
	if(!ipRow){
		fnPrintLogStyre(3, cEnv.m_OrgTable, cObjID, _T("�t�B�[�`���[��������Ȃ�(�o�Ă͂����Ȃ�)"), NULL, TGT_LAYER);
		return false;
	}
	
	CString aMsg;

	if(cStatus & SHAPE_CHG){
		// �yBug9997�z�ʏ펞�͌`��ύX�ł��Ȃ��悤�C��
#ifdef STOP_POINT
		//�`��ύX����(�o�O���ĂȂ���΃t�B�[�`���̂͂����Ǝv������A�V�K�̏ꍇ�͂����ɗ���)
		IFeaturePtr ipFeature = ipNewRow;
		if(ipFeature){
			IGeometryPtr ipGeom;
			ipFeature->get_ShapeCopy(&ipGeom);
			((IFeaturePtr)ipRow)->putref_Shape(ipGeom);
			aMsg = _T("�y�`��z");
		}
#else
		// �����������f�������Ƃ��낾���ǁA�t���O�����������Ȃ����肷��̂ŁA������p��
		return false;
#endif
	}

	if(cStatus & ATTR_CHG){
		//�����ύX����
		fnReflectAttr(ipRow, ipNewRow, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, aMsg);
	}
	if(aMsg.IsEmpty()){
		aMsg = _T("�y�����n�̂݁z");
	}
	fnSetCommonAttr(ipRow, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, ipNewRow);
	if(!( g_Mode & NO_CHG_MODE)){
		if(FAILED(ipRow->Store())){
			fnPrintComError();
			long aObjID;
			ipNewRow->get_OID(&aObjID);
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�ύX���s"), NULL, PGDB_LAYER);
			return false;		
		}
		fnPrintLogStyre(0, cEnv.m_OrgTable, cObjID, _T("�ύX"), aMsg, TGT_LAYER);
	}else{
		fnPrintLogStyre(0, cEnv.m_OrgTable, cObjID, _T("�ύX�����"), aMsg, TGT_LAYER);
	}
	return true;
}

// INF,LQ�p�X�V�֐�
bool CImport::fnInfReflectToSiNDY(const _IRowPtr& ipNewRow, LqEnvTable& cEnv, long cObjID)
{
	long PgdbID;
	ipNewRow->get_OID(&PgdbID);
	//INF�X�V
	_IRowPtr ipRow;
	if(FAILED(m_ipTable[0]->GetRow(cObjID, &ipRow))){
		fnPrintComError();
		return false;
	}
	if(!ipRow){
		fnPrintLogStyre(3, cEnv.m_OrgTables[0], PgdbID, _T("���E��������Ȃ�"), NULL, PGDB_LAYER);
		return false;
	}
	
	CString aMsg;
	//�����ύX����
	fnReflectAttr(ipRow, ipNewRow, cEnv.m_SDEIndexMap[0], cEnv.m_PGDBIndexMap[0], aMsg);

	fnSetCommonAttr(ipRow, cEnv.m_SDEIndexMap[0], cEnv.m_PGDBIndexMap[0], ipNewRow);
	if(!(g_Mode & NO_CHG_MODE)){
		if(FAILED(ipRow->Store())){
			fnPrintComError();
			fnPrintLogStyre(2, cEnv.m_OrgTables[0], PgdbID, _T("�ύX���s"), NULL, PGDB_LAYER);
			return false;		
		}
		fnPrintLogStyre(0, cEnv.m_OrgTables[0], cObjID, _T("�ύX"), aMsg, TGT_LAYER);
	}else{
		fnPrintLogStyre(0, cEnv.m_OrgTables[0], cObjID,  _T("�ύX�����"), aMsg, TGT_LAYER);
	}
	return true;
}

// ���f�Ώۑ����̃R�s�[(PGDB->SDE)�֐�
bool CImport::fnReflectAttr(_IRowPtr& ipTgtRow, const _IRowPtr& ipSrcRow, INDEX_MAP& cSDEIndex, INDEX_MAP& cPGDBIndex, CString& cField)
{
	bool aIsChange = false;	//���ۂɕύX�������ۂ�
	//�ʑ���
	for(INDEX_MAP::const_iterator it = cSDEIndex.begin(); it != cSDEIndex.end(); ++it){
		//�ŏI�X�V���t�͔��f�ΏۊO�A�ŏI�X�V�҂͌�Ŏw��l�œ��ꂷ��̂ŏ��O�AINF_ID�͕ʓr�X�V����̂ŏ��O
		//��ƖړI�A�X�V���e�i�H�j���ΏۊO�i����v�]�j
		//�������r�́A�R���e�i�ɓ����ۂɑ啶���ɂ��Ă���̂ƁA�X�L�[�}���啶������̂͂��Ȃ̂Łu==�v�ł����͂�
		if(it->first == ipc_table::kOperator || it->first == ipc_table::kModifyDate || it->first == link_queue::kInfID
			|| it->first == ipc_table::kPurpose || it->first == ipc_table::kUpdateType){
			continue;
		}
		CComVariant aVal;
		ipSrcRow->get_Value(cPGDBIndex[it->first].m_Index, &aVal);
		if(it->first == category::link_relational_table::kLinkID){
			aVal.lVal = m_IDMaps[road_link::kTableName][aVal.lVal];
		}else if(it->first == category::from_to_node_relational_table::kToNodeID || it->first == category::from_to_node_relational_table::kFromNodeID || it->first == stop_point::kNodeID){
			aVal.lVal = m_IDMaps[road_node::kTableName][aVal.lVal];
		}
		CComVariant aTgtVal;
		ipTgtRow->get_Value(it->second.m_Index, &aTgtVal);
		if(aVal != aTgtVal){
			ipTgtRow->put_Value(it->second.m_Index, aVal);
			cField += _T("�y");
			cField += it->second.m_Alias;
			cField += _T("�z");
			aIsChange = true;
		}
	}
	return aIsChange;
}

// ���b�V���|���S���ŃN���b�v�����`������߂�֐�
// fnGetRemakeShape�Ǝ��Ă���悤�Ɍ����邪�A������́A���b�V���|���S���Ő؂������c��̃W�I���g�������߂�
IGeometryPtr CImport::fnGetClipShape(const IFeaturePtr& ipFeature, EnvTable& cEnv, long cObjID, bool cForceMode) const
{
	IGeometryPtr ipGeom;
	ipFeature->get_ShapeCopy(&ipGeom);

	if(!cForceMode){
		//�O�̃��b�V���̉e���Ń}���`�ł��؂��肪�K�v�ɂȂ邱�Ƃ����邩��A
		//���Ղɏȗ��ł��Ȃ��@2008/02/01
		msi_get_parts_info aPolUtil;
		if(aPolUtil.Init(ipGeom, cObjID, (string)CT2A(m_LayerManage->findTableName(cEnv.m_OrgTable)).m_psz, 0, stderr)){
			if(aPolUtil.Get_Part_Num() > 1){
				//���������ǁA�}���`�p�[�g�Ȃ�ԈႢ�Ȃ����b�V�����E�̂͂�
				//�������Ȃ��A�؂�\��̓r���ł̂݃}���`�p�[�g�ɂȂ�O��
				return ipGeom;
			}
		}else{
			//�ǂ����悤���Ȃ�
			fnPrintLogStyre(2, cEnv.m_OrgTable, cObjID, _T("���莞�|���S���̕��͎��s"), NULL, FIX_LAYER);
			return ipGeom;
		}
	}
	ITopologicalOperatorPtr ipTopo = ipGeom;
	IGeometryPtr ipTgtGeom;
	//���f�[�^�����b�V���Ő؂���
	if(FAILED(ipTopo->Difference(m_ipMeshPoly, &ipTgtGeom))){
		fnPrintComError();
		fnPrintLogStyre(2, cEnv.m_OrgTable, cObjID, _T("�؂��莸�s"), NULL, FIX_LAYER);
		return ipGeom;
	}
	ipTgtGeom->putref_SpatialReference( m_LayerManage->get_SpRef() );
	return ipTgtGeom;
}

// ���b�V�����E�̃I�u�W�F�N�g�ɑ΂��āA���Y���b�V���O�����Ɠ��Y���b�V�����̐؂�\����s���֐�
// �}���`�p�[�g�ɂȂ邱�Ƃ�����A�I���W�i��ID���������̂�1�ɂ܂Ƃ߂悤�Ƃ��邪�A�������ύX�ɂȂ��Ă�����̂�����ΐV�K�ǉ������Ƃ���
IGeometryPtr CImport::fnGetRemakeShape(const IFeatureClassPtr& ipNewClass, const IFeaturePtr& ipFeature, EnvTable& cEnv, long cObjID)
{
	IGeometryPtr ipGeom;
	ipFeature->get_ShapeCopy(&ipGeom);

	ITopologicalOperatorPtr ipTopo = ipGeom;
	//�Ώۃf�[�^�����b�V���Ő؂���i�א�9���b�V���o���Ă���̂ŁA�������؂�Ȃ��Ƃ����Ȃ��j
	IGeometryPtr ipTgtGeom;
	if(FAILED(ipTopo->Intersect(m_ipMeshPoly, esriGeometry2Dimension, &ipTgtGeom))){
		fnPrintComError();
		fnPrintLogStyre(2, cEnv.m_OrgTable, cObjID, _T("�؂��莸�s"), NULL, TGT_LAYER);
		return NULL;
	}
	ipTopo = ipTgtGeom;

	//���f�[�^�����b�V���Ő؂���
	if(FAILED(ipTopo->Difference(m_ipMeshPoly, &ipTgtGeom))){
		fnPrintComError();
		fnPrintLogStyre(2, cEnv.m_OrgTable, cObjID, _T("�؂��莸�s"), NULL, TGT_LAYER);
		return NULL;
	}
	ipTgtGeom->putref_SpatialReference( m_LayerManage->get_SpRef() );

	IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, NULL, _T("%s = %d"), ORG_OBJID_NAME, cObjID);

	IFeatureCursorPtr ipCursor;
	if(FAILED(ipNewClass->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return NULL;
	}
	bool aNeedWarning = false;
	IFeaturePtr ipSrcFeature;
	while(ipCursor->NextFeature(&ipSrcFeature) == S_OK && ipSrcFeature){
		ipTopo = ipTgtGeom;
		CComVariant aUpdate;
		ipSrcFeature->get_Value(cEnv.m_PGDBIndexMap[CHG_STATUS_NAME].m_Index, &aUpdate);
		if(aUpdate.lVal & ATTR_CHG){
			//�����ύX����������͐V�K�����i���H��Ƃł͂����ĂȂȂ�Ȃ����ǁj
			long aObjID;
			ipSrcFeature->get_OID(&aObjID);
			m_AppendID.insert(aObjID);
			continue;
		}

		IGeometryPtr ipSrcGeom, ipTgtGeom2;
		ipSrcFeature->get_Shape(&ipSrcGeom);
		ipTopo->Union(ipSrcGeom, &ipTgtGeom2);
		ipTgtGeom = ipTgtGeom2;
	}
	return ipTgtGeom;
}

// �����ʒu���͓��������NID,�m�[�hID�������̂����ɑ��݂��邩�`�F�b�N����֐�
bool CImport::fnExistSamePoint(const IFeaturePtr& ipFeature, EnvTable& cEnv)
{
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	{
		//�ʒu�I�ɓ����ɂ��邩�`�F�b�N
		CComBSTR          bstrFieldName;
		((IFeatureClassPtr)m_ipTable[0])->get_ShapeFieldName( &bstrFieldName );
		ISpatialFilterPtr ipFilter = AheInitSpatialFilter( (ISpatialFilterPtr)AheInitQueryFilter( NULL, bstrFieldName ), ipGeom, bstrFieldName );
		IFeatureCursorPtr ipCursor;
		if(FAILED(((IFeatureClassPtr)m_ipTable[0])->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
			fnPrintComError();
			return false;
		}
		IFeaturePtr ipTgtFeature;
		ipCursor->NextFeature(&ipTgtFeature);
		if(ipTgtFeature){
			long aObjID;
			ipTgtFeature->get_OID(&aObjID);
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("�����ʒu�Ƀ|�C���g������"), NULL, TGT_LAYER, ipGeom);
			return true;
		}
	}
	{
		//���������NID,�m�[�hID�������̂��`�F�b�N
		CComVariant aVal;
		ipFeature->get_Value(cEnv.m_PGDBIndexMap[category::link_relational_table::kLinkID].m_Index, &aVal);
		long aOrgLinkID = m_IDMaps[road_link::kTableName][aVal.lVal];

		ipFeature->get_Value(cEnv.m_PGDBIndexMap[stop_point::kNodeID].m_Index, &aVal);
		long aOrgNodeID = m_IDMaps[road_node::kTableName][aVal.lVal];

		IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, NULL, _T("LINK_ID = %ld AND NODE_ID = %ld"), aOrgLinkID, aOrgNodeID);

		IFeatureCursorPtr ipCursor;
		if(FAILED(((IFeatureClassPtr)m_ipTable[0])->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
			fnPrintComError();
			return false;
		}
		IFeaturePtr ipTgtFeature;
		ipCursor->NextFeature(&ipTgtFeature);
		if(ipTgtFeature){
			CString aMsg;
			aMsg.Format(_T("���������NID(%ld)�E�m�[�hID(%ld)�����|�C���g������"), aOrgLinkID, aOrgNodeID);
			long aObjID;
			ipTgtFeature->get_OID(&aObjID);
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, aMsg, NULL, TGT_LAYER, ipGeom);
			return true;
		}
	}
	return false;
}

// �����C����ID�������t�B�[���h�ɂ��āA���̎����Ă���I�u�W�F�N�g�̌`��̕ω��𒲂ׂ�֐�(�ꎞ��~)�B
// �����C����ID�������t�B�[���h�ɂ��āA���̎����Ă���I�u�W�F�N�g�̕ω��𒲂ׂ�֐�(�ꎞ��~�ȊO)�B
bool CImport::fnRelObjChange(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap, const CString& cTableName, const CString& cFieldName, const IGeometryPtr& ipGeom, CString& cRelLayer)
{
	INDEX_MAP::const_iterator it = cPgdbIndexMap.find(cFieldName);
	if(it == cPgdbIndexMap.end()){
		//�w��t�B�[���h�͎����Ă��Ȃ�����֌W�Ȃ�
		return true;
	}
	//���[�J���ł�ID���擾
	CComVariant aVal;
	ipRow->get_Value(it->second.m_Index, &aVal);
	
	if(cFieldName == category::link_relational_table::kLinkID){
		cRelLayer = road_link::kTableName;
	}else if(cFieldName == category::from_to_node_relational_table::kToNodeID || cFieldName == category::from_to_node_relational_table::kFromNodeID || cFieldName == stop_point::kNodeID){
		cRelLayer = road_node::kTableName;
	}
	if(cRelLayer.IsEmpty()){
		//�Ăԑ������������͂�������
		fnPrintLogStyre(3, cTableName, -1, _T("�o�O�̂͂��i�o�Ă͂����Ȃ��j"), NULL, TGT_LAYER);
		return false;
	}
	CHG_ID& aChgID = m_IDSets[cRelLayer];

#ifndef STOP_POINT
	//���̃v���O�������ł̍X�V�ɂ����̂Ȃ狖���Ȃ��Ɛ��������Ȃ�
	if(aChgID.m_OK.find(aVal.lVal) != aChgID.m_OK.end()){
		return true;
	}
	//���ɔ��f�s�̂��̂��w���Ă�����ANG
	if(aChgID.m_NG.find(aVal.lVal) != aChgID.m_NG.end()){
		return false;
	}
#endif
	long aPgdbID;
	ipRow->get_OID(&aPgdbID);
	//SDE��ł�ID�ɕϊ�
	long aOrgID = m_IDMaps[cRelLayer][aVal.lVal];

	IFeaturePtr ipSdeFeature;
	IFeatureClassPtr ipSdeClass =  m_LayerManage->findTable(cRelLayer);
	ipSdeClass->GetFeature(aOrgID, &ipSdeFeature);
	if(!ipSdeFeature){
		fnPrintLogStyre(2, cRelLayer, aPgdbID, _T("�Q�ƃm�[�h�E�����N��������Ȃ�"), NULL, PGDB_LAYER, ipGeom);
		return false;
	}

	IFeaturePtr ipPgdbFeature;
	IFeatureClassPtr ipPgdbClass = m_OldTableInfo[cRelLayer].m_ipTable;
	ipPgdbClass->GetFeature(aVal.lVal, &ipPgdbFeature);
	if(!ipPgdbFeature){
		fnPrintLogStyre(3, cTableName, aPgdbID, _T("�o�O�̂͂��i�o�Ă͂����Ȃ��j"), NULL, PGDB_LAYER);
		return false;
	}

	if(fnMeshRelObject(ipSdeFeature) || fnMeshRelObject(ipPgdbFeature)){
		//���b�V�����E�Ɋ֌W������̂̓��b�Z�[�W��\�����Ĕ��f���Ȃ�
		fnPrintLogStyre(2, cRelLayer, aPgdbID, _T("�֘A�I�u�W�F�N�g�����b�V�����E���͔͈͊O�̃I�u�W�F�N�g�Ȃ̂Ŕ��f���Ȃ�"), NULL, PGDB_LAYER);
		return false;
	}

#ifndef STOP_POINT
	//�֘A�����N�A�m�[�h�̎�ʂ��`�F�b�N
	EnvTable *pEnv = m_LayerManage->findEnvTable(cRelLayer);
	if(!pEnv){
		fnPrintLogStyre(3, cTableName, aPgdbID, _T("�o�O�̂͂��i�o�Ă͂����Ȃ��j"), NULL, PGDB_LAYER);
		return false;
	}
	CComVariant aSindyModDate;
	ipSdeFeature->get_Value(pEnv->m_SDEIndexMap[ipc_table::kModifyDate].m_Index, &aSindyModDate);
	CComVariant aPgdbModDate;
	ipPgdbFeature->get_Value(pEnv->m_PGDBIndexMap[ORG_MODIFYDATE_NAME].m_Index, &aPgdbModDate);
	//PGDB�̍X�V���t��NULL����Ȃ���������t������r�ANULL��������S������r�i���LQ�΍�j
	if((aPgdbModDate.vt != VT_NULL && aSindyModDate.date != aPgdbModDate.date) || (aPgdbModDate.vt == VT_NULL && fnIsAttrChange(ipSdeFeature, pEnv->m_SDEIndexMap, ipPgdbFeature, pEnv->m_PGDBIndexMap))){
		fnPrintLogStyre(2, cRelLayer, aPgdbID, _T("�Q�ƃm�[�h�E�����N�������ύX����Ă���"), NULL, PGDB_LAYER, ipGeom);
		return false;	
	}
#endif
	if(!fnShapeComp(ipSdeFeature, ipPgdbFeature)){
		fnPrintLogStyre(2, cRelLayer, aPgdbID, _T("�Q�ƃm�[�h�E�����N�`�󂪕ύX����Ă���"), NULL, PGDB_LAYER, ipGeom);
		return false;
	}
	return true;
}

// �����C����ID�������t�B�[���h�ɂ��āA���̎����Ă���I�u�W�F�N�g�̕ω��𒲂ׂ�֐�
bool CImport::fnRelObjsChange(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap, const CString& cTableName, const IGeometryPtr& ipGeom, CString& cRelLayer)
{
	for(int i = 0; REL_ID_TABLE[i] != NULL; ++i){
		if(!fnRelObjChange(ipRow, cPgdbIndexMap, cTableName, REL_ID_TABLE[i], ipGeom, cRelLayer)){
			return false;
		}
	}
	return true;
}

// �Q�̃t�B�[�`���ɂ��āA�`��̈�v�𒲂ׂ�֐�
bool CImport::fnShapeComp(const IFeaturePtr& ipFeature1, const IFeaturePtr& ipFeature2) const
{
	IGeometryPtr ipGeom1;
	ipFeature1->get_Shape(&ipGeom1);

	IGeometryPtr ipGeom2;
	ipFeature2->get_Shape(&ipGeom2);

	if(!ipGeom1 || !ipGeom2){
		fnPrintLogStyre(3, _T("���C����"), -1, _T("NULL Shape�i�o�Ă͂����Ȃ��͂��j"), NULL, PGDB_LAYER);
		return false;
	}

	VARIANT_BOOL aEqual = false;
	/* �Q�l 
	�_��̉�]�Ƃ��A���e�l�Ƃ��i�H�j���l�����āA�`��Ƃ��Ă͓����ƌ������Ƃɂ������ꍇ�͉��̒ʂ�
	�����ł́A�ҏW����Ă��Ȃ����Ƃ��m�F�������̂ŁA�����Ȕ�r��I��
	if(((IRelationalOperatorPtr)ipGeom1)->Equals(ipGeom2, &aEqual) == S_OK && !aEqual) {	// �Â���r
	*/
	//�uIsEqual�v�́A�`�󂪈Ⴄ���ɁuS_FALSE�v��Ԃ��悤�ŁA�u==S_OK�v�Ƃ���Ɛ��������ʂɂȂ�Ȃ�
	if(SUCCEEDED(((IClonePtr)ipGeom1)->IsEqual((IClonePtr)ipGeom2, &aEqual)) && !aEqual){	//�����Ȕ�r
		//�`�󂪈Ⴄ�Ȃ�i�ύX����������jfalse
		return false;
	}
	return true;
}

// INF�ELQ�̃y�A�P�ʂŕύX�𔽉f����֐�
bool CImport::fnReflectLQ(const IWorkspacePtr& ipOldWorkspace, const IWorkspacePtr& ipNewWorkspace, LqEnvTable& cEnv)
{
	ITablePtr ipOldTable[2];
	ITablePtr ipNewTable[2];
	for(int i = 0; i < 2; ++i){
		m_ipTable[i] = m_LayerManage->findTable(cEnv.m_OrgTables[i]);
		ipOldTable[i] = fnOpenLocalTable(ipOldWorkspace, cEnv.m_OrgTables[i]);
		if(!ipOldTable[i]){
			return false;
		}

		ipNewTable[i] = fnOpenLocalTable(ipNewWorkspace, cEnv.m_OrgTables[i]);
		if(!ipNewTable[i]){
			return false;
		}

		//�t�B�[���h�C���f�b�N�X�̎擾
		CLayerManage::getFieldInfo(ipNewTable[i], cEnv.m_PGDBIndexMap[i]);
	}
	//INF�P�ʂōX�V�\�������Ȃ���X�V�ƍ폜
	_tprintf(_T("%s,�X�V�ƍ폜�����Ă��܂�\n"), cEnv.m_OrgTables[0]);
	if(!fnInfModAndDel(ipOldTable, ipNewTable, cEnv)){
		return false;
	}
	//INF�P�ʂōX�V�\�������Ȃ���ǉ�
	return fnInfAdd(ipOldTable, ipNewTable, cEnv);
}

// �����N��̐V�K�ǉ��֐�
bool CImport::fnInfAdd(const ITablePtr ipOldTable[2], const ITablePtr ipNewTable[2], LqEnvTable& cEnv)
{
	IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, NULL, _T("%s IS NULL OR %s = 0"), ORG_OBJID_NAME, ORG_OBJID_NAME);

	_ICursorPtr ipCursor;
	if(FAILED(ipNewTable[0]->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return false;
	}

	IRowBufferPtr ipBuffer;
	_ICursorPtr ipInsCursor;
	if(!( g_Mode & NO_CHG_MODE)){
		if(FAILED(m_ipTable[0]->Insert(VARIANT_TRUE, &ipInsCursor))){
			fnPrintComError();
			return false;
		}
		m_ipTable[0]->CreateRowBuffer(&ipBuffer);
	}

	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow){
		long aObjID;
		ipRow->get_OID(&aObjID);
		//�֘A���郊���N��m�[�h�̍폜�E�`��ω�������
		CString aRelLayer;
		if(!fnRelObjsChange(ipRow, cEnv.m_PGDBIndexMap[0], cEnv.m_OrgTables[0], NULL, aRelLayer)){
			fnPrintLogStyre(2, cEnv.m_OrgTables[0], aObjID, _T("�֘A�I�u�W�F�N�g�ɏ����ΏۊO���܂܂�Ă���iINF�ǉ��j"), aRelLayer, PGDB_LAYER);
			continue;
		}

		ID_SET aAdd, aMod, aDel;
		if(fnCanRefrectLq(aObjID, ipOldTable[1], ipNewTable[1], m_ipTable, cEnv, aAdd, aMod, aDel)){
			if(fnExistSameLQ(ipNewTable[1], ipRow, cEnv, m_ipTable[1] )){
				//�����ƌ��Ȃ�����̂���������ǉ����Ȃ��i���\�肪���񂾏����ɂȂ�̂ŁA�ŏI�`�F�b�N�̈Ӗ��ł��̈ʒu�j
				fnPrintLogStyre(2, cEnv.m_OrgTables[0], aObjID, _T("����Ƃ݂Ȃ�����̂����ɑ��݂��܂�"), NULL, PGDB_LAYER);
				continue;
			}
			CComVariant aID = 0;
			if(!( g_Mode & NO_CHG_MODE)){
				CString aDummy;
				fnReflectAttr((_IRowPtr)ipBuffer, ipRow, cEnv.m_SDEIndexMap[0], cEnv.m_PGDBIndexMap[0], aDummy);
				fnSetCommonAttr((_IRowPtr)ipBuffer, cEnv.m_SDEIndexMap[0], cEnv.m_PGDBIndexMap[0], NULL);
				if(SUCCEEDED(ipInsCursor->InsertRow(ipBuffer, &aID))){
					fnPrintLogStyre(0, cEnv.m_OrgTables[0], aID.lVal, _T("�ǉ�"), NULL, TGT_LAYER);
				}else{
					fnPrintLogStyre(2, cEnv.m_OrgTables[0], aObjID, _T("�ǉ����s"), NULL, PGDB_LAYER);
					continue;	//�G���[�Ŗ߂������������̂�������Ȃ�
				}
			}else{
				fnPrintLogStyre(0, cEnv.m_OrgTables[0], aObjID, _T("�ǉ������"), NULL, PGDB_LAYER);
			}
			if(!fnAddLQs(aAdd, aID.lVal, ipNewTable[1], cEnv)){//�X�V���Ȃ����ł����O���o��悤�ɁA���̈ʒu
				return false;
			}
		}
	}
	if(!( g_Mode & NO_CHG_MODE)){
		if(FAILED(ipInsCursor->Flush())){
			fnPrintComError();
			return false;
		}
	}
	return true;
}

// �����N��̕ύX�ƍ폜�֐�
bool CImport::fnInfModAndDel(const ITablePtr ipOldTable[2], const ITablePtr ipNewTable[2], LqEnvTable& cEnv)
{
	_ICursorPtr ipCursor;
	if(FAILED(ipOldTable[0]->Search(NULL, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return false;
	}

	_IRowPtr ipOldRow;
	while(ipCursor->NextRow(&ipOldRow) == S_OK && ipOldRow){
		long aObjID;
		ipOldRow->get_OID(&aObjID);

		CComVariant aOrgID;
		ipOldRow->get_Value(cEnv.m_PGDBIndexMap[0][ORG_OBJID_NAME].m_Index, &aOrgID);
		_IRowPtr ipNewRow;
		ipNewTable[0]->GetRow(aObjID, &ipNewRow);
		if(ipNewRow){
			//�ύX���`�F�b�N
			CComVariant aUpdate;
			ipNewRow->get_Value(cEnv.m_PGDBIndexMap[0][CHG_STATUS_NAME].m_Index, &aUpdate);
			if(aUpdate.vt != VT_NULL && aUpdate.lVal != 0){
				//�Ⴂ����������X�V
				if(!fnSiNDYChange(cEnv, 0, aOrgID.lVal, (_IRowPtr)NULL, ipOldRow, _T("�ύX"), m_ipTable )){
					//SiNDY��ō폜����Ă�����̂́A���̐�͖���
					//�����D��̎������̃I�u�W�F�N�g��
					continue;
				}
				//Inf���͍̂X�V�\�����ALQ�����Ȃ��Ƃ킩��Ȃ�
				ID_SET aAdd, aMod, aDel;
				if(fnCanRefrectLq(aObjID, ipOldTable[1], ipNewTable[1], m_ipTable, cEnv, aAdd, aMod, aDel)){
					//fnPrintLogStyre(0, cEnv.m_TgtTables[0], aOrgID.lVal, _T("INF�X�V�H"));
					fnInfReflectToSiNDY(ipNewRow, cEnv, aOrgID.lVal);
					if(!fnDeleteLQs(aDel, cEnv.m_OrgTables[1])){
						return false;
					}
					if(!fnModLQs(aMod, ipNewTable[1], cEnv)){
						return false;
					}
					if(!fnAddLQs(aAdd, aOrgID.lVal, ipNewTable[1], cEnv)){
						return false;
					}
				}
			}
		}else{
			//�폜���ꂽ
			_IRowPtr ipRow;
			m_ipTable[0]->GetRow(aOrgID.lVal, &ipRow);
			if(ipRow){
				if(fnSiNDYChange(cEnv, 0, aOrgID.lVal, ipRow, ipOldRow, _T("�폜"), m_ipTable )){
					//Inf���͍̂폜�\�����ALQ�����Ȃ��Ƃ킩��Ȃ�
					ID_SET aAdd, aMod, aDel;
					if(fnCanRefrectLq(aObjID, ipOldTable[1], ipNewTable[1], m_ipTable, cEnv, aAdd, aMod, aDel)){
						//INF,LQ�S�폜
						if(!(g_Mode & NO_CHG_MODE)){
							if(FAILED(ipRow->Delete())){
								fnPrintComError();
								fnPrintLogStyre(2, cEnv.m_OrgTables[0], aOrgID.lVal, _T("�폜���s"), NULL, TGT_LAYER);
							}else{
								fnPrintLogStyre(0, cEnv.m_OrgTables[0], aOrgID.lVal, _T("�폜"), NULL, FIX_LAYER);
							}
						}else{
							fnPrintLogStyre(0, cEnv.m_OrgTables[0], aOrgID.lVal, _T("�폜�����"), NULL, PGDB_LAYER);
						}
						if(!fnDeleteLQs(aDel, cEnv.m_OrgTables[1])){
							return false;
						}
					}
				}
			}else{
				fnPrintLogStyre(1, cEnv.m_OrgTables[0], aOrgID.lVal, _T("���ɍ폜����Ă���"), NULL, FIX_LAYER);
			}
		}
	}
	return true;
}

// LQ�̕ύX��Ԃ����Ȃ���ύX�\���𒲂ׂ�֐�
bool CImport::fnCanRefrectLq(long cInfID, const ITablePtr& ipOldTable, const ITablePtr& ipNewTable, const ITablePtr ipSdeTable[2], LqEnvTable& cEnv, ID_SET& cAdd, ID_SET& cMod, ID_SET& cDel)
{
	//�w��INF_ID������LQ�̐V�����r���āA�ǉ��E�X�V�E�폜�𒊏o
	//�֘A�����N�̕ω��̗L�����`�F�b�N
	//LQ���폜�����P�[�X�ɂ��Ă��A�֘A�����N�̓`�F�b�N�Ώہi���H����G�Ɋm�F�������ʁj

	//�u�V�v�ɂ�����̂���`�F�b�N�i�폜�ȊO�j
	IQueryFilterPtr ipFilter = AheInitQueryFilter(NULL, NULL, _T("%s = %d"), link_queue::kInfID, cInfID);

	_ICursorPtr ipCursor;
	ipNewTable->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	if(!ipCursor){
		return false;
	}
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow){
		long aObjID;
		ipRow->get_OID(&aObjID);
		_IRowPtr ipOldRow;
		ipOldTable->GetRow(aObjID, &ipOldRow);
		if(ipOldRow){
			CComVariant aOrgID;
			ipOldRow->get_Value(cEnv.m_PGDBIndexMap[1][ORG_OBJID_NAME].m_Index, &aOrgID);
			//�ύX����ƕύX�Ȃ��i�ǉ��E�폜�ȊO�j
			if(!fnSiNDYChange(cEnv, 1, aOrgID.lVal, (_IRowPtr)NULL, ipOldRow, _T("LQ�ύX"), ipSdeTable )){
				return false;
			}
			if(fnIsAttrChange(ipRow, cEnv.m_PGDBIndexMap[1], ipOldRow, cEnv.m_PGDBIndexMap[1])){
				cMod.insert(aObjID);
			}
		}else{
			//�ǉ�
			//LQ���͔̂�r�ł��Ȃ����A�֘A���郊���N��m�[�h�̍폜�E�`��ω�������
			cAdd.insert(aObjID);
			CString aRelLayer;
			if(!fnRelObjsChange(ipRow, cEnv.m_PGDBIndexMap[1], cEnv.m_OrgTables[1], NULL, aRelLayer)){
				fnPrintLogStyre(2, cEnv.m_OrgTables[0], aObjID, _T("�֘A�I�u�W�F�N�g�ɏ����ΏۊO���܂܂�Ă���iLQ�ǉ��j"), aRelLayer, PGDB_LAYER);
				return false;
			}
		}
	}

	//�u���v�ɂ����Ȃ����̂��`�F�b�N
	ipOldTable->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	if(!ipCursor){
		return false;
	}
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow){
		long aObjID;
		ipRow->get_OID(&aObjID);
		_IRowPtr ipNewRow;
		ipNewTable->GetRow(aObjID, &ipNewRow);
		if(!ipNewRow){
			//�폜
			CComVariant aOrgID;
			ipRow->get_Value(cEnv.m_PGDBIndexMap[1][ORG_OBJID_NAME].m_Index, &aOrgID);
			cDel.insert(aOrgID.lVal);	//�폜������SDE���ID��ێ�
			if(!fnSiNDYChange(cEnv, 1, aOrgID.lVal, (_IRowPtr)NULL, ipRow, _T("LQ�ύX"), ipSdeTable )){
				return false;
			}
		}
	}
	return true;
}

// �����̕ύX�̗L���𒲂ׂ�֐�
// �P���Ȕ�r�ł͂Ȃ��A���Ȃ��ėǂ��i���Ă͂����Ȃ��j���������ȂǁA�����ɓs���̗ǂ���r������
// �t�B�[���h�����Ⴄ���́ASrc���ɏ��Ȃ������Z�b�g������������
bool CImport::fnIsAttrChange(const _IRowPtr& ipSrcRow, const INDEX_MAP& cSrcIndex, const _IRowPtr& ipTgtRow, const INDEX_MAP& cTgtIndex)
{
	//��r�t�B�[���h���������ƌ������Ƃ́APGDB���m�i�Ă�łȂ�����SDE���m�j�Ȃ̂ŁA�قڒP����r
	//��{��SDE��PGDB�̔�r
	bool aNormalComp = true;
	if(cSrcIndex.size() == cTgtIndex.size()){
		//PGDB���m�̎�
		aNormalComp = false;
	}
	for(INDEX_MAP::const_iterator aSrc = cSrcIndex.begin(); aSrc != cSrcIndex.end(); ++aSrc){
		if(aSrc->first == ipc_table::kModifyDate){
			//�ŏI�X�V���t�͕ʂɔ�r����̂ŁA��΂��������֗�
			continue;
		}

		if(aNormalComp && (aSrc->first.CompareNoCase(ipc_table::kOperator) == 0
			|| aSrc->first.CompareNoCase(link_queue::kInfID) == 0)){
			//�ŏI�X�V�҂�SDE��PGDB�ňႤ�悤�ɂ��Ă���̂ŁA��r�ΏۊO
			//INF_ID�͑Ή������Ă��Ȃ��̂ƁA���������ς�邱�Ƃ͂Ȃ��͂��Ȃ̂őΏۊO
			continue;
		}
		INDEX_MAP::const_iterator aTgt = cTgtIndex.find(aSrc->first);
		if(aTgt != cTgtIndex.end()){
			CComVariant aSrcVal;
			ipSrcRow->get_Value(aSrc->second.m_Index, &aSrcVal);
			CComVariant aTgtVal;
			ipTgtRow->get_Value(aTgt->second.m_Index, &aTgtVal);
			if(aNormalComp){
				//SDE���ID�ɕϊ����Ĕ�r
				if(aSrc->first == category::link_relational_table::kLinkID){
					aTgtVal.lVal = m_IDMaps[road_link::kTableName][aTgtVal.lVal];
				}else if(aSrc->first == category::from_to_node_relational_table::kToNodeID || aSrc->first == category::from_to_node_relational_table::kFromNodeID || aSrc->first == stop_point::kNodeID){
					aTgtVal.lVal = m_IDMaps[road_node::kTableName][aTgtVal.lVal];
				}
			}
			if(aSrcVal != aTgtVal){
				return true;
			}
		}
	}
	return false;
}

// �Ώ�ID��LQ���폜����֐�
// cDelIDs�ɂ�SDE��̂��̂�����
bool CImport::fnDeleteLQs(const ID_SET& cDelIDs, LPCTSTR cLayerName)
{
	foreach(const long aObjID, cDelIDs){
		_IRowPtr ipRow;
		m_ipTable[1]->GetRow(aObjID, &ipRow);
		if(ipRow){
			if(!(g_Mode & NO_CHG_MODE)){
				if(FAILED(ipRow->Delete())){
					fnPrintComError();
					fnPrintLogStyre(2, cLayerName, aObjID, _T("�폜���s"), NULL, TGT_LAYER);
					return false;
				}else{
					fnPrintLogStyre(0, cLayerName, aObjID, _T("�폜"), NULL, FIX_LAYER);
				}
			}else{
				fnPrintLogStyre(0, cLayerName, aObjID, _T("�폜�����"), NULL, TGT_LAYER);
			}
		}
	}
	return true;
}

// �Ώ�ID��LQ��ǉ�����֐�
// cAddIDs�ɂ�PGDB��̂��̂�����
bool CImport::fnAddLQs(const ID_SET& cAddIDs, long cInfID, const ITablePtr& ipNewTable, LqEnvTable& cEnv)
{
	_ICursorPtr ipInsCursor;
	IRowBufferPtr ipBuffer;
	if(!( g_Mode & NO_CHG_MODE)){
		if(FAILED(m_ipTable[1]->Insert(VARIANT_TRUE, &ipInsCursor))){
			fnPrintComError();
			return false;
		}
		m_ipTable[1]->CreateRowBuffer(&ipBuffer);
	}

	foreach(const long aObjID, cAddIDs){
		_IRowPtr ipRow;
		ipNewTable->GetRow(aObjID, &ipRow);
		if(!ipRow){
			fnPrintLogStyre(3, cEnv.m_OrgTables[1], aObjID, _T("���E��������Ȃ�(��΃o�O)"), NULL, PGDB_LAYER);
			return false;
		}

		if(!( g_Mode & NO_CHG_MODE)){
			CString aDummy;
			fnReflectAttr((_IRowPtr)ipBuffer, ipRow, cEnv.m_SDEIndexMap[1], cEnv.m_PGDBIndexMap[1], aDummy);
			fnSetCommonAttr((_IRowPtr)ipBuffer, cEnv.m_SDEIndexMap[1], cEnv.m_PGDBIndexMap[1], NULL);
			ipBuffer->put_Value(cEnv.m_SDEIndexMap[1][link_queue::kInfID].m_Index, CComVariant(cInfID));	//INF_ID�͕t���ւ�
			CComVariant aID = 0;
			if(SUCCEEDED(ipInsCursor->InsertRow(ipBuffer, &aID))){
				long aObjID;
				ipRow->get_OID(&aObjID);
				fnPrintLogStyre(0, cEnv.m_OrgTables[1], aID.lVal, _T("�ǉ�"), NULL, TGT_LAYER);
			}else{
				fnPrintLogStyre(2, cEnv.m_OrgTables[1], aObjID, _T("�ǉ����s"), NULL, PGDB_LAYER);
			}
		}else{
			fnPrintLogStyre(0, cEnv.m_OrgTables[1], aObjID, _T("�ǉ������"), NULL, PGDB_LAYER);
		}
	}
	return true;
}

// �Ώ�ID��LQ��ύX����֐�
// cModIDs�ɂ�PGDB��̂��̂�����
bool CImport::fnModLQs(const ID_SET& cModIDs, const ITablePtr& ipNewTable, LqEnvTable& cEnv)
{
	foreach(const long aObjID, cModIDs){
		_IRowPtr ipNewRow;
		ipNewTable->GetRow(aObjID, &ipNewRow);
		if(!ipNewRow){
			fnPrintLogStyre(3, cEnv.m_OrgTables[1], aObjID, _T("���E��������Ȃ�(��΃o�O)"), NULL, PGDB_LAYER);
			return false;
		}

		CComVariant aOrgID;
		ipNewRow->get_Value(cEnv.m_PGDBIndexMap[1][ORG_OBJID_NAME].m_Index, &aOrgID);
		_IRowPtr ipRow;
		if(FAILED(m_ipTable[1]->GetRow(aOrgID.lVal, &ipRow))){
			fnPrintComError();
			return false;
		}
		if(!ipRow){
			fnPrintLogStyre(3, cEnv.m_OrgTables[1], aObjID, _T("���E��������Ȃ�"), NULL, PGDB_LAYER);
			return false;
		}
	
		CString aMsg;
		//�����ύX����
		if(fnReflectAttr(ipRow, ipNewRow, cEnv.m_SDEIndexMap[1], cEnv.m_PGDBIndexMap[1], aMsg)){
			fnSetCommonAttr(ipRow, cEnv.m_SDEIndexMap[1], cEnv.m_PGDBIndexMap[1], ipNewRow);
			if(!(g_Mode & NO_CHG_MODE)){
				if(FAILED(ipRow->Store())){
					fnPrintComError();
					fnPrintLogStyre(2, cEnv.m_OrgTables[1], aObjID, _T("�ύX���s"), NULL, PGDB_LAYER);
					return false;		
				}
				fnPrintLogStyre(0, cEnv.m_OrgTables[1], aOrgID.lVal, _T("�ύX"), aMsg, TGT_LAYER);
			}else{
				fnPrintLogStyre(0, cEnv.m_OrgTables[1], aOrgID.lVal, _T("�ύX�����"), aMsg, TGT_LAYER);
			}
		}
	}
	return true;
}

// ���b�V�����E�Ɋւ�閔�̓��b�V���͈͊O���𔻒肷��֐�
bool CImport::fnMeshRelObject(const _IRowPtr& ipRow) const
{
	IFeaturePtr ipFeature = ipRow;
	if(ipFeature){
		IGeometryPtr ipGeom;
		ipFeature->get_Shape(&ipGeom);
#if 1
		IRelationalOperatorPtr ipRel(m_ipMeshBoundary);
		VARIANT_BOOL aDisjoint;
		//���b�V���g�Ɗւ���Ă��Ȃ�����
		if(SUCCEEDED(ipRel->Disjoint(ipGeom, &aDisjoint)) && aDisjoint){
			ipRel = m_ipMeshPoly;
			//���b�V���|���S���̒��ɔ[�܂��Ă��邱��
			VARIANT_BOOL aContain;
			if(SUCCEEDED(ipRel->Contains(ipGeom, &aContain)) && aContain){
				return false;
			}
		}
		return true;
#else
		//���b�V���|���S���Ɋ��S����Ă邱��
		IRelationalOperatorPtr ipRel(m_ipMeshPoly);
		VARIANT_BOOL aContain;
		if(SUCCEEDED(ipRel->Contains(ipGeom, &aContain)) && aContain){
			return false;
		}
		return true;
#endif
	}
	return false;
}

// �Q�Ƃ��Ă��鑼�̃��C����ύX�֎~���X�g�ɒǉ�����֐��i�����NID,�m�[�hID���S���j
bool CImport::fnProhibitRelIDAll(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap)
{
	for(int i = 0; REL_ID_TABLE[i] != NULL; ++i){
		fnProhibitRelID(ipRow, cPgdbIndexMap, REL_ID_TABLE[i]);
	}
	return true;
}

// �Q�Ƃ��Ă��鑼�̃��C����ύX�֎~���X�g�ɒǉ�����֐��i�t�B�[���h���w��j
bool CImport::fnProhibitRelID(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap, const CString& cFieldName)
{
	INDEX_MAP::const_iterator it = cPgdbIndexMap.find(cFieldName);
	if(it == cPgdbIndexMap.end()){
		//�w��t�B�[���h�͎����Ă��Ȃ�����֌W�Ȃ�
		return true;
	}
	//���[�J���ł�ID���擾
	CComVariant aVal;
	ipRow->get_Value(it->second.m_Index, &aVal);
	
	CString aRelLayer;
	long aOrgID = -1;
	if(cFieldName == category::link_relational_table::kLinkID){
		aRelLayer = road_link::kTableName;
	}else if(cFieldName == category::from_to_node_relational_table::kToNodeID || cFieldName == category::from_to_node_relational_table::kFromNodeID || cFieldName == stop_point::kNodeID){
		aRelLayer = road_node::kTableName;
	}
	if(aRelLayer.IsEmpty()){
		//�Ăԑ������������͂�������
		return false;
	}
	m_IDSets[aRelLayer].m_NG.insert(aVal.lVal);
	return true;
}

// �����N��Ɗ֘A�t���O�̐��������֐�
bool CImport::fnLinkFlagCare(LqEnvTable& cEnv)
{
	CString aFlagField = cEnv.m_Tag + _T("_LQRF");	///< �֘A�t���O�̃t�B�[���h���i����Ȃ����ł����̂��H�j

	//���H�����N�̏��擾
	CHG_ID& aChgID = m_IDSets[road_link::kTableName];	///< ���H�����N�̕ύX�E�s���
	IFeatureClassPtr ipClass = m_LayerManage->findTable(road_link::kTableName);	///< ���H�����N�t�B�[�`���N���X
	EnvTable *aLinkEnv = m_LayerManage->findEnvTable(road_link::kTableName);	///< ���H�����N�̏��e�[�u��
	long aIndex = aLinkEnv->m_SDEIndexMap[aFlagField].m_Index;	///< �֘A�t���O�̃t�B�[���h�C���f�b�N�X

	// �ύX�����A���悤�Ƃ������̂��Ώۂł����͂�
	fnLinkFlagCareByID(ipClass, aIndex, aChgID.m_OK);	// �����������ł�����������Ȃ�����
	fnLinkFlagCareByID(ipClass, aIndex, aChgID.m_NG);	// �O�̂��߂�������
	return true;
}

// ID��w��Ń����N��Ɗ֘A�t���O�̐��������֐�
bool CImport::fnLinkFlagCareByID(const IFeatureClassPtr& ipClass, long cFlagIndex, const ID_SET& cIDSet)
{
	foreach(const long aObjID, cIDSet){
		// SDE���ID�ɕϊ�
		long aSdeID = m_IDMaps[road_link::kTableName][aObjID];

		IFeaturePtr ipFeature;
		ipClass->GetFeature(aSdeID, &ipFeature);
		if(!ipFeature){
			//�폜���ꂽ���̂����邩��X�L�b�v
			continue;
		}
		CComVariant aVal;
		ipFeature->get_Value(cFlagIndex, &aVal);
		int aFlag = fnIsExistLQ(aSdeID);
		if(aVal.lVal != aFlag){
			if(!( g_Mode & NO_CHG_MODE)){
				ipFeature->put_Value(cFlagIndex, CComVariant(aFlag));
				ipFeature->Store();
				//_ftprintf(stderr, _T("INFO\tLINK�̃t���O�ύX\t%d\n"), aObjID);
				fnPrintLogStyre(0, road_link::kTableName, aSdeID, _T("LINK�̃t���O�ύX�i��n���j"), NULL, TGT_LAYER);
			}else{
				// �����ɗ��邱�Ƃ͂Ȃ��͂�
				fnPrintLogStyre(0, road_link::kTableName, aSdeID, _T("LINK�̃t���O�v�ύX�i��n���j"), NULL, TGT_LAYER);
			}
		}
	}
	return true;
}

// �����N��ID�����Ƀ����N��ŎQ�Ƃ���Ă��邩�𔻒肷��֐�
// �߂�l�����̂܂܊֘A�t���O�̒l�Ƃ��邽�߂ɐ��l��0/1��Ԃ�
int CImport::fnIsExistLQ(long cObjID) const
{
	//�����NID�Ō������āA�����ŐU�蕪��
	IQueryFilterPtr ipFilter = AheInitQueryFilter(NULL, NULL, _T("%s = %d"), category::link_relational_table::kLinkID, cObjID);

	//�t���O�Ɍ����͊֌W�Ȃ��i�L�������j
	long aCount;
	m_ipTable[1]->RowCount(ipFilter, &aCount);
	if(aCount > 0){
		return 1;
	}
	return 0;
}

// �����Ƃ݂Ȃ�����̂����ɑ��݂��邩�𒲂ׂ�֐�
bool CImport::fnExistSameObj(const _IRowPtr& ipRow, EnvTable& cEnv)
{
	//�e�[�u���E�t�B�[�`���N���X�ɂ���āA�u����v�Ƃ݂Ȃ��������Ⴄ
	//����������ƌn�����ĂĂ�肽���������A��t���ɂȂ��Ă��܂����̂ŁE�E
	if(cEnv.m_OrgTable != segment_attr::kTableName && cEnv.m_OrgTable != time_oneway::kTableName && cEnv.m_OrgTable != time_nopassage::kTableName){
		//�����N�������E��ʁE�ʍs�֎~ �ȊO�Ȃ�X���[
		return false;
	}

	//�����NID�������ŁA���̑����i���𓙂̋��ʑ������������A���L�̑����j����v������̂����邩
	CComVariant aLinkID;
	ipRow->get_Value(cEnv.m_PGDBIndexMap[category::link_relational_table::kLinkID].m_Index, &aLinkID);
	// SDE���ID�ɕϊ�
	aLinkID.lVal = m_IDMaps[road_link::kTableName][aLinkID.lVal];
	IQueryFilterPtr ipFilter = AheInitQueryFilter(NULL, NULL, _T("%s = %d"), category::link_relational_table::kLinkID, aLinkID.lVal);
	
	_ICursorPtr ipCursor;
	m_ipTable[0]->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	if(!ipCursor){
		// �����ɗ��Ă͂����Ȃ��͂������A���f�֎~�ɂ��邽�߂�true�i���݂���j��Ԃ�
		return true;
	}
	_IRowPtr ipTgtRow;
	while(ipCursor->NextRow(&ipTgtRow) == S_OK && ipTgtRow){
		if(cEnv.m_OrgTable == segment_attr::kTableName){
			//�����N�������Ȃ�A�`�����r�i������v�H�ڐG����ł��j
			if(fnShapePartComp(ipRow, ipTgtRow)){
				return true;
			}
		}else{
			if(fnSameAttr(ipRow, ipTgtRow, cEnv)){
				return true;
			}
		}
	}
	return false;
}

// �ŗL�����i���ʑ����ȊO�Łj�̍��ق𒲂ׂ�֐�
bool CImport::fnSameAttr(const _IRowPtr& ipPgdbRow, const _IRowPtr& ipSdeRow, EnvTable& cEnv)
{
	//���܂���������΁ufnIsAttrChange(LQ���ӎ����č쐬��������)�v�Ƌ��ʂł��ǂ�����
	for(INDEX_MAP::const_iterator aSrc = cEnv.m_SDEIndexMap.begin(); aSrc != cEnv.m_SDEIndexMap.end(); ++aSrc){
		if(aSrc->first.CompareNoCase(ipc_table::kModifyDate) == 0
			|| aSrc->first.CompareNoCase(ipc_table::kOperator) == 0
			|| aSrc->first.CompareNoCase(ipc_table::kProgModifyDate) == 0
			|| aSrc->first.CompareNoCase(ipc_table::kPurpose) == 0
			|| aSrc->first.CompareNoCase(ipc_table::kSource) == 0
			|| aSrc->first.CompareNoCase(ipc_table::kUpdateType) == 0
			|| aSrc->first.CompareNoCase(ipc_table::kModifyProgName) == 0){
			// ���ʑ����͑ΏۊO
			continue;
		}
		INDEX_MAP::const_iterator aTgt = cEnv.m_PGDBIndexMap.find(aSrc->first);
		if(aTgt != cEnv.m_PGDBIndexMap.end()){
			CComVariant aSrcVal;
			ipPgdbRow->get_Value(aSrc->second.m_Index, &aSrcVal);
			CComVariant aTgtVal;
			ipSdeRow->get_Value(aTgt->second.m_Index, &aTgtVal);
			//SDE���ID�ɕϊ����Ĕ�r
			if(aSrc->first == category::link_relational_table::kLinkID){
				aTgtVal.lVal = m_IDMaps[road_link::kTableName][aTgtVal.lVal];
			}else if(aSrc->first == category::from_to_node_relational_table::kToNodeID || aSrc->first == category::from_to_node_relational_table::kFromNodeID || aSrc->first == stop_point::kNodeID){
				aTgtVal.lVal = m_IDMaps[road_node::kTableName][aTgtVal.lVal];
			}
			if(aSrcVal != aTgtVal){
				return true;
			}
		}
	}
	return false;
}

bool CImport::fnShapePartComp(const IFeaturePtr& ipFeature1, const IFeaturePtr& ipFeature2) const
{
	IGeometryPtr ipGeom1;
	ipFeature1->get_Shape(&ipGeom1);

	IGeometryPtr ipGeom2;
	ipFeature2->get_Shape(&ipGeom2);

	if(!ipGeom1 || !ipGeom2){
		fnPrintLogStyre(3, _T("���C����"), -1, _T("NULL Shape�i�o�Ă͂����Ȃ��͂��j"), NULL, PGDB_LAYER);
		return false;
	}
#if 1
	// 2�_�ȏ㓯���_����������i�{���͘A�����Ă��邱�Ƃ��������������񂾂낤���ǁj
	IPointCollectionPtr ipPointCol1 = ipGeom1;
	long aPointCount1 = 0;
	ipPointCol1->get_PointCount( &aPointCount1 );
	std::vector<WKSPoint> pPt1( aPointCount1 );
	ipPointCol1->QueryWKSPoints( 0, aPointCount1, &pPt1.at(0) );

	IPointCollectionPtr ipPointCol2 = ipGeom2;
	long aPointCount2 = 0;
	ipPointCol2->get_PointCount( &aPointCount2 );
	std::vector<WKSPoint> pPt2( aPointCount2 );
	ipPointCol2->QueryWKSPoints( 0, aPointCount2, &pPt2.at(0) );
	int aMachCount = 0;
	for(int i = 0; i < aPointCount1; ++i){
		for(int j = 0; j < aPointCount2; ++j){
			if(pPt1[i].X == pPt2[j].X && pPt1[i].Y == pPt2[j].Y){
				++aMachCount;
			}
			if(aMachCount > 1){
				return true;
			}
		}
	}
#else
	// �ǂ��炩�ɓ����Ă��邱�Ƃ̔�������҂������A�_�����ۂ�
	VARIANT_BOOL aContain = VARIANT_FALSE;
	if(SUCCEEDED(((IRelationalOperatorPtr)ipGeom1)->Contains(ipGeom2, &aContain)) && aContain){
		return true;
	}else if(SUCCEEDED(((IRelationalOperatorPtr)ipGeom2)->Contains(ipGeom1, &aContain)) && aContain){
		return true;
	}
#endif
	return false;
}

bool CImport::fnExistSameLQ(const ITablePtr& ipLqTable, const _IRowPtr& ipNewRow, LqEnvTable &cEnv, const ITablePtr& ipSdeLqTable )
{
	// �����̃����N������擾
	long aObjID;
	ipNewRow->get_OID(&aObjID);
	LQ_MAP aLqMap;
	fnGetLqInfo(ipLqTable, aObjID, cEnv.m_PGDBIndexMap[1], true, aLqMap);

	// �n�_�������ɂȂ郊���N����
	long aStartLinkID = aLqMap[1];
	IQueryFilterPtr ipFilter = AheInitQueryFilter(NULL, NULL, _T("%s = %d AND %s = 1"), link_queue::kLinkID, aStartLinkID, link_queue::kSequence);
	_ICursorPtr ipCursor;
	ipSdeLqTable->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	if(!ipCursor){
		return false;
	}

	// �J�n�����N�����������N��́uLQ�v���񋓂����
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow){
		CComVariant aInfID;
		ipRow->get_Value(cEnv.m_SDEIndexMap[1][link_queue::kInfID].m_Index, &aInfID);

		LQ_MAP aSdeLqMap;
		fnGetLqInfo(ipSdeLqTable, aInfID.lVal, cEnv.m_SDEIndexMap[1], false, aSdeLqMap);
		// �i���ꂩ��ǉ����悤�Ƃ��Ă���j�����Ɠ����Z�b�g�i���тƃ����NID�j�̗L���𒲂ׂ�
		if(aLqMap == aSdeLqMap){
			return true;
		}
	}

	return false;
}

// INF��ID������LQ�����������NID�����ԂɂȂ�悤�擾����֐�
bool CImport::fnGetLqInfo(const ITablePtr& ipLqTable, long cInfID, INDEX_MAP& cLqIndexMap, bool cIsPGDB, LQ_MAP& cLqMap)
{
	IQueryFilterPtr ipFilter = AheInitQueryFilter(NULL, NULL, _T("%s = %d"), link_queue::kInfID, cInfID);

	_ICursorPtr ipCursor;
	ipLqTable->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	if(!ipCursor){
		return false;
	}
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow){
		CComVariant aSeq;
		ipRow->get_Value(cLqIndexMap[link_queue::kSequence].m_Index, &aSeq);
		CComVariant aLinkID;
		ipRow->get_Value(cLqIndexMap[link_queue::kLinkID].m_Index, &aLinkID);
		if(cIsPGDB){
			// SDE��̃����NID�ɕϊ����Ċi�[
			cLqMap[aSeq.lVal] = m_IDMaps[road_link::kTableName][aLinkID.lVal];
		}else{
			// SDE�����炻�̂܂܊i�[
			cLqMap[aSeq.lVal] = aLinkID.lVal;
		}
	}
	return true;
}

// �����N��ID����A���g�̊֘AInf/Lq���X�V�\���m�F
bool CImport::fnCanRelInfLqUpdate( const ITablePtr ipOldTable[2], const ITablePtr ipNewTable[2], const ITablePtr ipSdeTable[2], LqEnvTable& env, long linkPgdbOID )
{
	// �u�����N��ObjectID �� LQ_*�ɂ���ID������LQ���w��INF_ID �� ����INF_ID������INF_*��ObjectID�v �Ƃ����`�Ŋ֘AINF��ID���擾����

	ID_SET    infModOIDs, infAddOIDs; //!< �����N���g�̊֘AInf�i�[�p�i�ύX�E�ǉ��j

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// LQ_*�̎��g��LINK_ID��������
	strWhere.Format( _T("%s = %d"), category::link_relational_table::kLinkID, linkPgdbOID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );
		
	// ����
	_IRowPtr    ipOldLqRow,    ipNewLqRow;
	_ICursorPtr ipOldLqCursor, ipNewLqCursor;
	ipOldTable[1]->Search( ipFilter, VARIANT_FALSE, &ipOldLqCursor );
	ipNewTable[1]->Search( ipFilter, VARIANT_FALSE, &ipNewLqCursor );

	// �X�V�OPGDB��LQ����A�֘AInf��OID���X�g���쐬����i�ύXor�폜�Ώۊm�F�p�j
	while( S_OK == ipOldLqCursor->NextRow(&ipOldLqRow) && ipOldLqRow ){
		
		CComVariant vaValue;
		ipOldLqRow->get_Value( env.m_PGDBIndexMap[1][link_queue::kInfID].m_Index, &vaValue );

		// InfID��ύX���X�g�Ɋi�[
		infModOIDs.insert( vaValue.lVal );
	}

	// �X�V��PGDB��LQ����A�֘AInf��OID���X�g���쐬����i�ǉ��Ώۊm�F�p�j
	while( S_OK == ipNewLqCursor->NextRow(&ipNewLqRow) && ipNewLqRow ){
		
		CComVariant vaValue;
		ipNewLqRow->get_Value( env.m_PGDBIndexMap[1][link_queue::kInfID].m_Index, &vaValue );

		// �V�K�ǉ����̂݁A���X�g�Ɋi�[
		if( infModOIDs.end() == infModOIDs.find( vaValue.lVal ) ){
			infAddOIDs.insert( vaValue.lVal );
		}
	}

	// �쐬����Inf��ObjectID���X�g����Ainf���������A�X�V�\�����f�i�ύXor�폜�Ώۊm�F�p�j
	for( auto& infOID : infModOIDs ){
			
		_IRowPtr ipOldRow;
		ipOldTable[0]->GetRow( infOID, &ipOldRow );

		if( !ipOldRow ){
			continue;
		}

		CComVariant aOrgID;
		ipOldRow->get_Value(env.m_PGDBIndexMap[0][ORG_OBJID_NAME].m_Index, &aOrgID);

		// �ύX��PGDB����Y��Inf���擾
		_IRowPtr ipNewRow;
		ipNewTable[0]->GetRow(infOID, &ipNewRow);
		if(ipNewRow){
			//�ύX���`�F�b�N
			CComVariant aUpdate;
			ipNewRow->get_Value(env.m_PGDBIndexMap[0][CHG_STATUS_NAME].m_Index, &aUpdate);
			if(aUpdate.vt != VT_NULL && aUpdate.lVal != 0){
				//�Ⴂ������ꍇ
				if(!fnSiNDYChange(env, 0, aOrgID.lVal, (_IRowPtr)NULL, ipOldRow, _T("�ύX"), ipSdeTable )){
					//Inf���ύX�s�̏ꍇ�́A�Y��Link���ύX�s��
					return false;
				}
				//Inf���͍̂X�V�\�����ALQ�����Ȃ��Ƃ킩��Ȃ�
				ID_SET aAdd, aMod, aDel;
				if( !fnCanRefrectLq(infOID, ipOldTable[1], ipNewTable[1], ipSdeTable, env, aAdd, aMod, aDel )){
					// Lq���ύX�s�̏ꍇ�́A�Y��Link���ύX�s��
					return false;
				}
			}
		}
		else{
			//�폜���ꂽ
			_IRowPtr ipRow;
			ipSdeTable[0]->GetRow(aOrgID.lVal, &ipRow);
			if(ipRow){
				// �폜�Ώۂ�Inf�̏�Ԃ��m�F
				if(!fnSiNDYChange(env, 0, aOrgID.lVal, ipRow, ipOldRow, _T("�폜"), ipSdeTable )){
					// Inf���폜�s�̏ꍇ�́A�Y��Link���ύX�s��
					return false;
				}
				//Inf���͍̂폜�\�����ALQ�����Ȃ��Ƃ킩��Ȃ�
				ID_SET aAdd, aMod, aDel;
				if( !fnCanRefrectLq(infOID, ipOldTable[1], ipNewTable[1], ipSdeTable, env, aAdd, aMod, aDel )){
					// Lq���폜�s�̏ꍇ�́A�Y��Link���ύX�s��
					return false;
				}
			}
		}
	}

	// �쐬����Inf��ObjectID���X�g����Ainf���������A�X�V�\�����f�i�ǉ��Ώۊm�F�p�j
	for( auto& infOID : infAddOIDs ){
			
		_IRowPtr ipNewRow;
		ipNewTable[0]->GetRow( infOID, &ipNewRow );

		if( !ipNewRow ){
			continue;
		}

		CComVariant aOrgID;
		ipNewRow->get_Value(env.m_PGDBIndexMap[0][ORG_OBJID_NAME].m_Index, &aOrgID);
		
		// ORG_OBJID����ȊO�͏��O�i�V�K�I�u�W�F�N�g����Ȃ��j
		if( aOrgID.vt != VT_NULL && aOrgID.vt != VT_EMPTY && aOrgID.lVal != 0 ){
			continue;
		}

		//�֘A���郊���N�̍폜�E�`��ω�������
		CString aRelLayer;
		if(!fnRelObjsChange(ipNewRow, env.m_PGDBIndexMap[0], env.m_OrgTables[0], NULL, aRelLayer)){
			// ���g��Inf�̊֘A�����N���ύX�s�̏ꍇ�́A�Y��Link���ύX�s��
			return false;
		}

		ID_SET aAdd, aMod, aDel;
		if(!fnCanRefrectLq(infOID, ipOldTable[1], ipNewTable[1], ipSdeTable, env, aAdd, aMod, aDel)){
			// Lq���܂ߒǉ��ł��Ȃ��ꍇ�́A�Y��Link���ύX�s��
			return false;
		}
			
		if(fnExistSameLQ(ipNewTable[1], ipNewRow, env, ipSdeTable[1] )){
			//�����ƌ��Ȃ�����̂���������ǉ����Ȃ������ƂȂ邽�߁A�Y��Link���ύX�s��
			return false;
		}
	}

	return true;
}
