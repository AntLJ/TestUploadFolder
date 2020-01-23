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
#include "Import.h"
#include "ImportData.h"
#include <chrono>

using namespace sindy::schema;
using namespace Reflect;
namespace {

	// UPDATE_C�ɐݒ肳���l�̒�`
	const long NO_CHG = 0x00;
	const long SHAPE_CHG = 0x01;	///< �`��ɕύX���������i�r�b�g�t���O�j
	const long ATTR_CHG = 0x02;		///< �����ɕύX���������i�r�b�g�t���O�j
	const long BOTH_CHG = SHAPE_CHG | ATTR_CHG;	///< �`��Ƒ����ɕύX��������
	// �ȉ��Q��DiffImportPGDB�݂̂Ŏg�p����(SiNDY-e,c�ł͎g�p���Ȃ�)
	const long DELETE_CHG = 0x04;	///< �폜���ꂽ
	const long NEW_CHG = 0x08;		///< �V�K�ǉ����ꂽ

	LPCTSTR CHG_STATUS_NAME =_T("UPDATE_C");	///< �X�V�X�e�[�^�X�̃t�B�[���h��
	LPCTSTR ORG_OBJID_NAME = _T("ORG_OBJID");	///< �I���W�i���̃I�u�W�F�N�gID�t�B�[���h��
	LPCTSTR ORG_MODIFYDATE_NAME = _T("ORG_MODIFYDATE");	///< �I���W�i���̍ŏI�X�V���t�t�B�[���h��
	LPCTSTR GEOSPACE_ID_NAME = _T("GEOSPACE_ID"); //!< GEOSPACEID�t�B�[���h��
}

// �R���X�g���N�^
CImport::CImport(void)
: m_OrgIdIndex(-1)
, m_ChgStatusIndex(-1)
, m_OrgModifyDateIndex(-1)
, m_Command(NONE)
, m_tdcMode(false)
, m_geospaceMode(false)
, m_emptyDBMode(false)
{
	m_ExecInfo = _T("�����C���|�[�g");

	m_CommandString[DIFF_UPDATE] = _T("�����X�V");
	m_CommandString[ONLY_DELETE] = _T("�폜�̂�");
	m_CommandString[ONLY_ADD]    = _T("�ǉ��̂�");
	m_CommandString[DELETE_ADD]  = _T("�폜�{�ǉ�");
}

// �f�X�g���N�^
CImport::~CImport(void)
{
}


// �������֐�
bool CImport::init(const CArguments& args, const IWorkspacePtr& ipWorkspace,
				   const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport )
{
	// true�����Ԃ��ĂȂ����ACSDEBase��init()���I�[�o�[���C�h���Ă��邽��
	// bool�l��Ԃ��Ȃ��Ƃ����Ȃ����߂����Ȃ��Ă���B
	m_OldPath = args.m_oldPath.c_str();
	m_NewPath = args.m_newPath.c_str();
	m_testMode = args.m_testMode;
	m_tdcMode = args.m_tdcMode;
	m_geospaceMode = args.m_geospaceMode;
	m_layerList = args.m_layerList;
	m_logFile = args.m_logFile;
	m_emptyDBMode = args.m_emptyDBMode;
	m_ipWorkspaceEdit = ipWorkspace;
	m_ipSpRef = ipSpRef;
	m_ipFeatureClasses = ipFeatureClasses;
	m_sindyCImport = isSindyCImport;

	COleDateTime oleTime;
	oleTime.ParseDateTime( args.m_availDate.c_str() );
	m_availDate = oleTime;

	return true;
}

// ���b�V���P�ʂł̍����C���|�[�g�i���f�j�֐�
bool CImport::fnExecMesh(COMMAND cCommand)
{
	m_Command = cCommand;
	_ftprintf(stderr, _T("#%d,%s\n"), g_CurrentMesh, m_CommandString[m_Command]);
	// �ҏW�OPGDB�擾
	IWorkspacePtr ipOldWorkspace;
	if( m_Command == DIFF_UPDATE ) // U�R�}���h�ȊO�Ȃ�ҏW�O�͂���Ȃ�
	{
		ipOldWorkspace = fnOpenLocalDatabase(m_OldPath);
		if( !ipOldWorkspace )
		{
			return false;
		}
	}
	// �ҏW��PGDB�擾
	IWorkspacePtr ipNewWorkspace = fnOpenLocalDatabase(m_NewPath);
	if( !ipNewWorkspace )
	{
		return false;
	}
	// ���b�V���|���S���擾
	if( !fnGetMesh(false) )
	{
		return false;
	}
	
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i)
	{
		const auto start_time = chrono::system_clock::now();

		m_AppendID.clear();
		m_TargetID.clear();
		// �ҏW�OPGDB�̃t�B�[�`���N���X
		IFeatureClassPtr ipOldClass;
		if( m_Command == DIFF_UPDATE )
		{
			ipOldClass = fnOpenLocalFeatureClass(ipOldWorkspace, i);
			if( !ipOldClass )
			{
				return false;
			}
			ipOldClass->FindField( CComBSTR(ORG_OBJID_NAME), &m_OrgIdIndex );
			if( m_OrgIdIndex < 0 )
			{
				_ftprintf(stderr, _T("#ERROR,�t�B�[���h��������Ȃ��B,%s\n"), ORG_OBJID_NAME);
				return false;
			}
		}
		// �ҏW��PGDB�̃t�B�[�`���N���X
		IFeatureClassPtr ipNewClass = fnOpenLocalFeatureClass(ipNewWorkspace, i);
		if( !ipNewClass )
		{
			return false;
		}
		// UPDATE_C�t�B�[���h�C���f�b�N�X�擾
		ipNewClass->FindField( CComBSTR(CHG_STATUS_NAME), &m_ChgStatusIndex );
		if(m_ChgStatusIndex < 0)
		{
			_ftprintf(stderr, _T("#ERROR,�t�B�[���h��������Ȃ��B,%s\n"), CHG_STATUS_NAME);
			return false;
		}
		// ORG_MODIFYDATE�t�B�[���h�C���f�b�N�X�擾
		ipNewClass->FindField(CComBSTR(ORG_MODIFYDATE_NAME), &m_OrgModifyDateIndex);
		if(m_OrgModifyDateIndex < 0)
		{
			_ftprintf(stderr, _T("#ERROR,�t�B�[���h��������Ȃ��B,%s\n"), ORG_MODIFYDATE_NAME);
			return false;
		}

		for(int j = 0; gEnvTable[g_DataIndex].m_Fields[i][j].m_fieldName != NULL; ++j)
		{
			// �ҏW�O�t�B�[�`���̃t�B�[���h�C���f�b�N�X�擾
			if(m_Command == DIFF_UPDATE)
			{
				ipOldClass->FindField(CComBSTR(gEnvTable[g_DataIndex].m_Fields[i][j].m_fieldName), &gEnvTable[g_DataIndex].m_Fields[i][j].m_OldPgdbIndex);
				if( gEnvTable[g_DataIndex].m_Fields[i][j].m_OldPgdbIndex < 0 )
				{
					_ftprintf(stderr, _T("#ERROR,�t�B�[���h��������Ȃ��B,%s\n"), gEnvTable[g_DataIndex].m_Fields[i][j].m_fieldName);
					return false;
				}
			}
			// �ҏW��t�B�[�`���̃t�B�[���h�C���f�b�N�X�擾
			ipNewClass->FindField(CComBSTR(gEnvTable[g_DataIndex].m_Fields[i][j].m_fieldName), &gEnvTable[g_DataIndex].m_Fields[i][j].m_NewPgdbIndex);
			if( gEnvTable[g_DataIndex].m_Fields[i][j].m_NewPgdbIndex < 0 )
			{
				_ftprintf(stderr, _T("#ERROR,�t�B�[���h��������Ȃ��B,%s\n"), gEnvTable[g_DataIndex].m_Fields[i][j].m_fieldName);
				return false;
			}
		}

		// GEOSPACE�C���|�[�g����SiNDY������U�폜����
		if( m_geospaceMode )
		{
			// �����ւ��Ώۃ��C�����H
			CString targetTable = ((CString)gEnvTable[g_DataIndex].m_OrgTableNames[i]).MakeUpper();
			auto& itr = std::find( m_layerList.begin(), m_layerList.end(), targetTable );
			if( itr != m_layerList.end() )
			{
				_tprintf(_T("�����ւ��Ή����s���܂��B,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i]);
				_tprintf(_T("�폜���Ă��܂��B,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i]);
				// �ΏۂȂ�S�폜
				if( !fnSiNDYDelete( i ) )
				{
					_ftprintf(stderr, _T("#ERROR,�I�u�W�F�N�g�폜�Ɏ��s�B,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i]);
					return false;
				}
			}
			else
			{
				// �ΏۂłȂ���Βʏ퍷���C���|�[�g
				_tprintf(_T("�����ւ����X�g�ɂ���܂���B�ʏ�C���|�[�g�B,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i]);
				_tprintf(_T("��r�Ɣ��f�����Ă��܂��i�ύX�E�폜�j�B,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i]);
				switch( gEnvTable[g_DataIndex].m_Type )
				{
					case CHANGE_TYPE_NORMAL:
						fnCompare(ipOldClass, ipNewClass, i);	//��r�ƕύX�E�폜
						break;
					case CHANGE_TYPE_POINTS:
						fnComparePoint(ipOldClass, ipNewClass, i);	//���b�V�����E���ӎ����Ȃ��Ă�����r�ƕύX�E�폜
						break;
					default:
						break;
				}
			}
			// �ǂ���̏ꍇ���V�K���̃C���|�[�g�͕K�v
			_tprintf(_T("�V�K����ǉ����Ă܂��B,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i]);
			fnAddNew( ipNewClass, i ); // �V�K�ǉ�
		}
		else
		{
			if( m_Command == DIFF_UPDATE )
			{
				//�ʏ퍷��
				_tprintf( _T("��r�Ɣ��f�����Ă��܂��i�ύX�E�폜�j�B,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i] );
				switch( gEnvTable[g_DataIndex].m_Type )
				{
				case CHANGE_TYPE_NORMAL:
					fnCompare(ipOldClass, ipNewClass, i);	//��r�ƕύX�E�폜
					break;
				case CHANGE_TYPE_POINTS:
					fnComparePoint(ipOldClass, ipNewClass, i);	//���b�V�����E���ӎ����Ȃ��Ă�����r�ƕύX�E�폜
					break;
				case CHANGE_TYPE_POI:
					fnComparePoi(ipOldClass, ipNewClass, i); // ��r�ƕύX �폜�͂��Ȃ�
					break;
				default:
					break;
				}
			}
			else if( m_Command == ONLY_DELETE || m_Command == DELETE_ADD )
			{
				//���ꃂ�[�h�i�폜����j�̎�
				_tprintf( _T("��������؂����Ă��܂��B,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i] );
				fnClipData(i);
			}

			// POI�̐V�K�ǉ��͂Ȃ�
			if( m_Command != ONLY_DELETE && gEnvTable[g_DataIndex].m_Type != CHANGE_TYPE_POI )
			{
				_tprintf( _T("�V�K����ǉ����Ă܂��B,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i] );
				fnAddNew(ipNewClass, i);	//�V�K�ǉ�
			}
		}
		//�O�̂��ߖ����I�ɔj��
		ipNewClass = nullptr;
		ipOldClass = nullptr;

		// ���C�����Ƃ̏�������
		const auto timeSpan = chrono::system_clock::now() - start_time;
		cout << CStringA(gEnvTable[g_DataIndex].m_OrgTableNames[i]) << ":"
			<< chrono::duration_cast<chrono::milliseconds>(timeSpan).count() << "[ms]" << endl;
	}
	// 1���b�V�����������I��邲�ƂɃN���A
	m_crossMeshOldOid.clear();
	m_crossMeshNewOid.clear();
	m_onlyNewOid.clear();
	return true;
}

// PGDB�I�[�v���֐�
IWorkspacePtr CImport::fnOpenLocalDatabase(LPCTSTR cBasePath)
{
	// �w��p�X���`.mdb, �`.gdb�Ȃ�A����𒼐ڊJ��
	CString aExt = CString( cBasePath ).Right(4);
	if( 0 == aExt.CompareNoCase(_T(".mdb")) || 0 == aExt.CompareNoCase(_T(".gdb")) )
	{
		return fnOpenWorkspaceFromFile(cBasePath);
	}

	// �w��p�X���f�B���N�g�����w���Ă���Ȃ�A���̉��Ƀ��b�V�����Ƃ�PGDB������Ɖ��肵�ĊJ��
	CString aDatabase;
	aDatabase.Format(_T("%s\\%04d\\%d.mdb"), cBasePath, g_CurrentMesh/10000, g_CurrentMesh); 
	return fnOpenWorkspaceFromFile(aDatabase);
}

// PGDB���̎w��C���f�b�N�X�̃t�B�[�`���N���X�I�[�v���֐�
IFeatureClassPtr CImport::fnOpenLocalFeatureClass(IWorkspacePtr ipWorkspace, long cIndex)
{
	IFeatureClassPtr ipFeatureClass;
	if(IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass(
		CComBSTR(gEnvTable[g_DataIndex].m_OrgTableNames[cIndex]), &ipFeatureClass) != S_OK){
		_ftprintf(stderr, _T("#ERROR,�t�B�[�`���[�N���X���J���Ȃ��B,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[cIndex]);
		return NULL;
	}
	return ipFeatureClass;
}

// ���b�V�����E���l�����Ă̔�r�ƕύX�E�폜
bool CImport::fnCompare(IFeatureClassPtr ipOldClass, IFeatureClassPtr ipNewClass, long cIndex)
{
	CString targetTable = gEnvTable[g_DataIndex].m_OrgTableNames[cIndex];

	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	ipFilter->putref_Geometry( m_ipMeshGeom );
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );
	CComBSTR shapeField;
	ipOldClass->get_ShapeFieldName( &shapeField );
	ipFilter->put_GeometryField( shapeField );

	IFeatureCursorPtr ipOldCursor;
	if( FAILED( ipOldClass->Search( ipFilter, VARIANT_FALSE, &ipOldCursor ) ) )
	{
		fnPrintComError();
		return false;
	}

	ipNewClass->get_ShapeFieldName( &shapeField );
	ipFilter->put_GeometryField( shapeField );
	IFeatureCursorPtr ipNewCursor;
	if( FAILED( ipNewClass->Search( ipFilter, VARIANT_FALSE, &ipNewCursor ) ) )
	{
		fnPrintComError();
		return false;
	}

	// ��PGDB�f�[�^��ORG_OBJID���Ƃɂ܂Ƃ߂�
	map<long, list<CAdapt<IFeaturePtr>>> mapOrgIDOldFeature;
	vector<long> oldOrgObjId;
	IFeaturePtr ipOldFeature;
	while( ipOldCursor->NextFeature( &ipOldFeature ) == S_OK && ipOldFeature )
	{
		long oid = -1;
		ipOldFeature->get_OID( &oid );

		CComVariant orgObjID;
		ipOldFeature->get_Value( m_OrgIdIndex, &orgObjID );
		if( orgObjID.lVal == 0 )
		{
			_ftprintf(
			stderr, _T("WARNING,��PGDB�t�B�[�`����ORG_OBJID��0,�t�B�[�`���N���X:%s,OBJECTID:%ld\n"),
			targetTable, oid);
		}

		CComVariant updateStatus;
		ipOldFeature->get_Value( m_ChgStatusIndex, &updateStatus );
		if( updateStatus.lVal != NO_CHG )
		{
			_ftprintf(
			stderr, _T("WARNING,��PGDB�t�B�[�`����UPDATE_C��0�łȂ�,�t�B�[�`���N���X:%s,OBJECTID:%ld,UPDATE_C:%ld\n"),
			targetTable, oid, updateStatus.lVal);
		}

		mapOrgIDOldFeature[orgObjID.lVal].push_back(ipOldFeature);
		oldOrgObjId.push_back( orgObjID.lVal );

		// ���X���b�V���܂����ł�����̂�ORG_OBJID��ێ�
		// Point�͕ʊ֐��ɍs���̂�Crosses�Ŕ���
		IGeometryPtr ipGeometry;
		ipOldFeature->get_Shape( &ipGeometry );
		VARIANT_BOOL cross = VARIANT_FALSE, disjoint = VARIANT_FALSE;
		((IRelationalOperatorPtr)ipGeometry)->Crosses( m_ipMeshGeom, &cross );
		if( cross )
		{
			m_crossMeshOldOid[targetTable].push_back( orgObjID.lVal );
		}
	}

	// �VPGDB�f�[�^��ORG_OBJID���Ƃɂ܂Ƃ߂�
	map<long, list<CAdapt<IFeaturePtr>>> mapOrgIDNewFeature;
	IFeaturePtr ipNewFeature;
	while( ipNewCursor->NextFeature( &ipNewFeature ) == S_OK && ipNewFeature )
	{
		CComVariant orgObjID;
		ipNewFeature->get_Value( m_OrgIdIndex, &orgObjID );
		if( orgObjID.lVal == 0 )
		{
			// ���̎��_��ORG_OBJID=0�̃I�u�W�F�N�g�́A�V�K���ǉ��̎��ɑΉ�����̂ł����ł͒e��
			continue;
		}

		mapOrgIDNewFeature[orgObjID.lVal].push_back(ipNewFeature);

		// ���b�V���܂����ł�����̂�ORG_OBJID��ێ�
		// Point�͕ʊ֐��ɍs���̂�Crosses�Ŕ���
		IGeometryPtr ipGeometry;
		ipNewFeature->get_Shape( &ipGeometry );
		VARIANT_BOOL cross = VARIANT_FALSE;
		((IRelationalOperatorPtr)ipGeometry)->Crosses( m_ipMeshGeom, &cross );
		if( cross )
		{
			m_crossMeshNewOid[targetTable].push_back( orgObjID.lVal );
		}

		// NEW���ɂ������Ȃ�ORG_OBJID��ێ�(���b�V�����E�͂ݏo���悤�ɂȂ����悤�ȃI�u�W�F�N�g)
		if( oldOrgObjId.end() == std::find( oldOrgObjId.begin(), oldOrgObjId.end(), orgObjID.lVal ) )
			m_onlyNewOid[targetTable].push_back( orgObjID.lVal );
	}

	// ORG_OBJID���L�[�ɂ��Ȃ���X�V�Ώۂ�T���X�V����
	for( const auto& oldTarget : mapOrgIDOldFeature )
	{
		const auto& newTarget = mapOrgIDNewFeature.find( oldTarget.first );
		if( newTarget == mapOrgIDNewFeature.end() )
		{
			// new���Ō�����Ȃ��Ȃ�폜���ꂽ�Ƃ�������
			for( const auto& oldFeature : oldTarget.second )
			{
				if( m_emptyDBMode )
				{
					// �����ς�ORG_OBJID�����ɂ����牽�����Ȃ��B
					const auto& update = std::find( m_updateOid[targetTable].begin(), m_updateOid[targetTable].end(), oldTarget.first );
					if( update != m_updateOid[targetTable].end() )
						continue;

					const auto& deleted = std::find( m_deleteOid[targetTable].begin(), m_deleteOid[targetTable].end(), oldTarget.first );
					if( deleted != m_deleteOid[targetTable].end() )
						continue;

					// UPDATE_C���폜�����ɂ��čX�V
					// �w�i���Ƃ悭���邪�A�Ώۃ��b�V���̊O�ɐڂ��Ă�p�^�[���͂�����continue�ɂȂ�B
					if( !fnInsertIntoEmptyDB( oldFeature, cIndex, DELETE_CHG ) )
						continue;

					m_deleteOid[targetTable].push_back( oldTarget.first );
					continue;
				}

				// SiNDY�ɂ��邩�H
				IFeaturePtr ipFeature;
				m_ipFeatureClasses[cIndex]->GetFeature( oldTarget.first, &ipFeature );
				if( !ipFeature )
				{
					// SiNDY�ł��폜����Ă���
					fnPrintLogStyle( warning, cIndex, oldTarget.first, _T("���ɍ폜����Ă���B") );
					continue;
				}

				// SiNDY�ɑ��݂��Ă���Ȃ�폜
				if( !fnSiNDYChange( cIndex, oldTarget.first, ipFeature, oldFeature, _T("�폜") ) )
					continue;

				if( !fnOverMesh( cIndex, oldTarget.first, ipFeature ) )
				{
					// �X�V�O�I�u�W�F�N�g�iSiNDY��Ń`�F�b�N�j�����b�V�����܂����ł��Ȃ�������
					// ���E��ł���������ڂ��Ă��邾���Ȃ�OK
					fnSiNDYDeleteCore(cIndex, ipFeature, oldTarget.first, _T("�폜"));
				}
				else
				{
					// �����������b�V���ɂ܂�����I�u�W�F�N�g��������؂�\��
					fnClipAndApend( ipNewClass, cIndex, oldTarget.first, NULL, NO_CHG );
				}
			}
			continue;
		}

		std::vector<CImportData> importDatas;
		for( const auto& newFeature : newTarget->second )
		{
			CImportData importData;

			// ORG_OBJID������Łu�`��̂ݕύX�v�Ɓu�����̂ݕύX�v�̗����������ꍇ�A�ǂ��炩��V�K�����ɂ��Ȃ��ƍX�V�ł��Ȃ�
			// �܂��A�u�����̂ݕύX�v���ǂ̑������ύX����Ă��邩������Ă݂Ȃ��Ƃ킩��Ȃ��̂ŁA��U�S���擾
			for( int i = 0; gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_fieldName != NULL; ++i )
			{
				CString fieldName = gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_fieldName;
				long index = gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_NewPgdbIndex;
				CComVariant val;
				newFeature->get_Value( index, &val );

				importData.m_mapFieldValue[fieldName] = val;
			}

			CComVariant status;
			newFeature->get_Value( m_ChgStatusIndex, &status );
			if( importDatas.empty() )
			{
				// ����
				importData.m_feature = newFeature;
				importData.m_updateStatus |= status.lVal;
				importDatas.push_back( importData );
				continue;
			}

			// ���������������m�Ō`����}�[�W���Ă����B�������Ⴄ���������A�V�K�����ɂ���B
			auto& itrDatas = find_if( importDatas.begin(), importDatas.end(),
				[&importData](CImportData& data){ return data.IsSameAttribute( importData ); } );

			if( itrDatas != importDatas.end() )
			{
				itrDatas->m_updateStatus |= status.lVal;
				if( !itrDatas->UnionGeometry( newFeature ) )
					fnPrintLogStyle( warning, cIndex, newTarget->first, _T("�C���|�[�g�O�}�[�W���s�B") );
			}
			else
			{
				importData.m_feature = newFeature;
				importData.m_feature->put_Value( m_OrgIdIndex, CComVariant(0) ); // ORG_OBJID=0�ɂ��邱�ƂŐV�K��������
				importData.m_updateStatus |= status.lVal;
				importDatas.push_back( importData );
			}
		}

		// �X�V
		for( const auto& data : importDatas )
		{
			IFeaturePtr ipNewFeature = data.m_feature;

			// ORG_OBJID=0 (�V�K����)�͉����D��
			CComVariant orgObjId;
			ipNewFeature->get_Value( m_OrgIdIndex, &orgObjId );
			if( orgObjId.lVal == 0 )
			{
				// �V�K����
				if( m_emptyDBMode )
				{
					fnInsertIntoEmptyDB( ipNewFeature, cIndex, NEW_CHG );
					continue;
				}

				// ��DB���ᖳ���Ƃ��p�̊֐��H�K�v�H
			}

			// UPDATE_C��0�Ȃ牽�����Ȃ�
			if( NO_CHG == data.m_updateStatus )
			{
				if( m_emptyDBMode )
				{
					m_updateOid[targetTable].push_back( orgObjId.lVal );
				}
				continue;
			}

			// ��̕��̏����ŕς���Ă��邩������Ȃ��̂ŁA�Z�b�g������
			ipNewFeature->put_Value( m_ChgStatusIndex, CComVariant(data.m_updateStatus) );

			// �ȉ��A�����̍X�V
			if( m_emptyDBMode )
			{
				// NEW�������b�V���܂����ł���Ȃ�܂��X�V
				const auto& crossNew = std::find( m_crossMeshNewOid[targetTable].begin(), m_crossMeshNewOid[targetTable].end(), orgObjId.lVal );
				if( crossNew == m_crossMeshNewOid[targetTable].end() )
				{
					// �폜�����ɂȂ��Ă���Ȃ�A�Њ���͐V�K����
					const auto& deleted = std::find( m_deleteOid[targetTable].begin(), m_deleteOid[targetTable].end(), orgObjId.lVal );
					if( deleted != m_deleteOid[targetTable].end() )
						fnInsertIntoEmptyDB( ipNewFeature, cIndex, NEW_CHG );
					else
						fnInsertIntoEmptyDB( ipNewFeature, cIndex );

					m_updateOid[targetTable].push_back( orgObjId.lVal );
					continue;
				}

				// NEW�������b�V�����܂����ł��Ȃ��Ƃ��͏����ς�ORG_OBJID���m�F(�ʃ��b�V���̏����ō폜�����ɂȂ��Ă��邩��)
				// �����ς݂��AOLD�������b�V�����܂����ł��Ȃ��Ȃ�V�K����
				const auto& update = std::find( m_updateOid[targetTable].begin(), m_updateOid[targetTable].end(), orgObjId.lVal );
				const auto& cross = std::find( m_crossMeshOldOid[targetTable].begin(), m_crossMeshOldOid[targetTable].end(), orgObjId.lVal );
				if( update != m_updateOid[targetTable].end() && cross == m_crossMeshOldOid[targetTable].end() )
				{
					fnInsertIntoEmptyDB( ipNewFeature, cIndex, NEW_CHG );
				}
				else
				{
					// �������̂��̂͒P�ɍX�V���邾��
					fnInsertIntoEmptyDB( ipNewFeature, cIndex );
				}
				m_updateOid[targetTable].push_back( orgObjId.lVal );
				continue;
			}

			if( !fnSiNDYChange( cIndex, oldTarget.first, NULL, ipNewFeature, _T("�ύX") ) )
			{
				// SiNDY��ō폜����Ă�����̂́A���̐�͖���
				// �����D��̎������̃I�u�W�F�N�g��
				continue;
			}

			if( !fnOverMesh( cIndex, oldTarget.first ) )
			{
				// �X�V�O�I�u�W�F�N�g�iSiNDY��Ń`�F�b�N�j�����b�V�����܂����ł��Ȃ�������
				// ���E��ł���������ڂ��Ă��邾���Ȃ�OK
				fnReflectToSiNDY( ipNewFeature, cIndex, oldTarget.first, data.m_updateStatus );
			}
			else
			{
				// �����������b�V���ɂ܂�����I�u�W�F�N�g��������؂�\��
				fnClipAndApend( ipNewClass, cIndex, oldTarget.first, ipNewFeature, data.m_updateStatus );
			}
		}
	}
	return true;
}

// ���b�V�����E���l�����Ȃ��Ă������̂̔�r�ƕύX�E�폜
// �i���L�ƃ|�C���g���Y�����邪�A���L���Ȃ��Ȃ����̂ŁA�����|�C���g�̂݁j
bool CImport::fnComparePoint(IFeatureClassPtr ipOldClass, IFeatureClassPtr ipNewClass, long cIndex)
{
	IFeatureCursorPtr ipCursor;
	if( FAILED( ipOldClass->Search( NULL, VARIANT_FALSE, &ipCursor ) ) )
	{
		fnPrintComError();
		return false;
	}
	IFeaturePtr ipOldFeature;
	while( ipCursor->NextFeature( &ipOldFeature ) == S_OK && ipOldFeature )
	{
		long aObjID;
		ipOldFeature->get_OID( &aObjID );
		CComVariant aOrgID;
		ipOldFeature->get_Value( m_OrgIdIndex, &aOrgID );
		IFeaturePtr ipNewFeature;
		ipNewClass->GetFeature( aObjID, &ipNewFeature );
		if( ipNewFeature )
		{
			//�ύX���`�F�b�N
			CComVariant aUpdate;
			ipNewFeature->get_Value( m_ChgStatusIndex, &aUpdate );
			if( aUpdate.vt != VT_NULL && aUpdate.lVal != NO_CHG )
			{
				if( m_emptyDBMode )
				{
					fnInsertIntoEmptyDB(ipNewFeature, cIndex);
					continue;
				}

				if( !fnSiNDYChange( cIndex, aOrgID.lVal, NULL, ipOldFeature, _T("�ύX") ) )
				{
					//SiNDY��ō폜����Ă��邩��A���̐�͖���
					//�����D�惂�[�h�̎���
					continue;
				}
				//�Ⴂ����������X�V
				fnReflectToSiNDY( ipNewFeature, cIndex, aOrgID.lVal, aUpdate.lVal );
			}
		}
		else
		{
			// PGDB�Ԃō폜����Ă���Ƃ�
			if( m_emptyDBMode )
			{
				fnInsertIntoEmptyDB( ipOldFeature, cIndex, DELETE_CHG );
				continue;
			}

			// �C���|�[�g��Ƀt�B�[�`��������΍폜
			IFeaturePtr ipFeature;
			m_ipFeatureClasses[cIndex]->GetFeature( aOrgID.lVal, &ipFeature );
			if( ipFeature )
			{
				if( fnSiNDYChange( cIndex, aOrgID.lVal, ipFeature, ipOldFeature, _T("�폜") ) )
				{
					fnSiNDYDeleteCore(cIndex, ipFeature, aOrgID.lVal, _T("�폜"));
				}
			}
			else
			{
				fnPrintLogStyle( warning, cIndex, aOrgID.lVal, _T("���ɍ폜����Ă���B") );
			}
		}
	}
	return true;
}

bool CImport::fnComparePoi( const IFeatureClassPtr& ipOldClass, const IFeatureClassPtr& ipNewClass, long cIndex )
{
	IFeatureCursorPtr ipCursor;
	if( FAILED( ipOldClass->Search( NULL, VARIANT_FALSE, &ipCursor ) ) )
	{
		fnPrintComError();
		return false;
	}

	// �C���|�[�g�����Ɏg�p����̂ō폜�R�[�h�̃C���f�b�N�X�擾
	long pgdbUpdDateIdx = -1, sdeDeleteIdx = -1, sdeUpdDateIdx = -1;
	long sdeWorkerIdx = -1, sdeWorkDateIdx = -1, sdeVerifierIdx = -1, sdeVerifyDateIdx = -1;
	m_ipFeatureClasses[cIndex]->FindField( CComBSTR(sindyk::poi_point::kDelete), &sdeDeleteIdx );
	ipOldClass->FindField( CComBSTR(sindyk::poi_point::kUPDAvailDate), &pgdbUpdDateIdx );
	m_ipFeatureClasses[cIndex]->FindField( CComBSTR(sindyk::poi_point::kUPDAvailDate), &sdeUpdDateIdx );
	m_ipFeatureClasses[cIndex]->FindField( CComBSTR(sindyk::poi_point::kWorker), &sdeWorkerIdx );
	m_ipFeatureClasses[cIndex]->FindField( CComBSTR(sindyk::poi_point::kWorkDate), &sdeWorkDateIdx );
	m_ipFeatureClasses[cIndex]->FindField( CComBSTR(sindyk::poi_point::kVerifier), &sdeVerifierIdx );
	m_ipFeatureClasses[cIndex]->FindField( CComBSTR(sindyk::poi_point::kVerifyDate), &sdeVerifyDateIdx );
	if( pgdbUpdDateIdx < 0 || sdeDeleteIdx < 0 || sdeUpdDateIdx < 0 ||
		sdeWorkerIdx < 0 || sdeWorkDateIdx < 0 || sdeVerifierIdx < 0 || sdeVerifyDateIdx < 0 )
	{
		fnPrintLogStyle( warning, cIndex, NULL, _T("�C���f�b�N�X�擾�Ɏ��s�B") );
		return false;
	}

	// ��r�J�n
	IFeaturePtr ipOldFeature;
	while( ipCursor->NextFeature( &ipOldFeature ) == S_OK && ipOldFeature )
	{
		// OLD_PGDB��OID
		long oldOid = 0;
		ipOldFeature->get_OID( &oldOid );
		// SDE��OID(ORG_OBJID)
		CComVariant orgOid;
		ipOldFeature->get_Value( m_OrgIdIndex, &orgOid );
		// NEW_PGDB����OLD_PGDB��OID�Ŏ擾
		IFeaturePtr ipNewFeature;
		ipNewClass->GetFeature( oldOid, &ipNewFeature );
		if( !ipNewFeature )
		{
			// �Ȃ��Ȃ�폜���ꂽ�Ƃ�������
			fnPrintLogStyle( error, cIndex, orgOid.lVal, _T("PGDB��ō폜����Ă���i�ύX�Ȃ��j�B") );
			continue;
		}

		//�ύX���`�F�b�N
		CComVariant aUpdate;
		ipNewFeature->get_Value( m_ChgStatusIndex, &aUpdate );
		if( aUpdate.vt == VT_NULL || aUpdate.lVal == NO_CHG )
			continue;

		if( m_emptyDBMode )
		{
			fnInsertIntoEmptyDB( ipNewFeature, cIndex );
			continue;
		}

		IFeaturePtr ipSdeFeature;
		m_ipFeatureClasses[cIndex]->GetFeature( orgOid.lVal, &ipSdeFeature );
		if( !ipSdeFeature )
		{
			fnPrintLogStyle( warning, cIndex, orgOid.lVal, _T("SDE���I�u�W�F�N�g�擾���s�B") );
			continue;
		}

		// OLD_PGDB��SDE�ňʒu���ς���Ă�H
		IGeometryPtr oldGeom, sdeGeom;
		ipOldFeature->get_Shape( &oldGeom );
		ipSdeFeature->get_Shape( &sdeGeom );
		oldGeom->putref_SpatialReference( m_ipSpRef );
		sdeGeom->putref_SpatialReference( m_ipSpRef );
		double oldX = 0, oldY =0, sdeX = 0, sdeY = 0;
		((IPointPtr)oldGeom)->QueryCoords( &oldX, &oldY );
		((IPointPtr)sdeGeom)->QueryCoords( &sdeX, &sdeY );
		if( oldX != sdeX || oldY != sdeY )
		{
			fnPrintLogStyle( warning, cIndex, orgOid.lVal, _T("����SDE��ňړ�����Ă���B") );
			continue;
		}

		// SDE�ō폜�ς݁H
		CComVariant deleteCode;
		ipSdeFeature->get_Value( sdeDeleteIdx, &deleteCode );
		if( deleteCode.lVal == sindyk::poi_point::delete_code::kDead )
		{
			fnPrintLogStyle( warning, cIndex, orgOid.lVal, _T("SDE��Łu�폜�v�ɂȂ��Ă���B") );
			continue;
		}

		// �����܂ł�����|�C���g�ړ��m��
		CString message;
		// �`��ύX����
		IGeometryPtr newGeom;
		ipNewFeature->get_ShapeCopy( &newGeom );
		newGeom->putref_SpatialReference( m_ipSpRef );
		ipSdeFeature->putref_Shape( newGeom );
		message = _T("�y�`��z");

		// �ҏW�ҁE�ҏW������
		SYSTEMTIME stNow = {};
		GetLocalTime( &stNow );
		DATE today;
		SystemTimeToVariantTime( &stNow, &today );

		// �����؂��H
		CComVariant sdeWorker, sdeWorkDate;
		CComVariant sdeVerifier, sdeVerifyDate;
		ipSdeFeature->get_Value( sdeWorkerIdx, &sdeWorker );
		ipSdeFeature->get_Value( sdeWorkDateIdx, &sdeWorkDate );
		ipSdeFeature->get_Value( sdeVerifierIdx, &sdeVerifier );
		ipSdeFeature->get_Value( sdeVerifyDateIdx, &sdeVerifyDate );
		if( !( ( sdeWorker.vt != VT_NULL && sdeWorkDate.vt != VT_NULL ) 
			&& ( sdeVerifier.vt == VT_NULL && sdeVerifyDate.vt == VT_NULL ) ) )
		{
			// �����؂łȂ���Ό��؎ҁE���؊������X�V
			message += _T("�yVERIFIER�z�yVERIFY_DATE�z");
			ipSdeFeature->put_Value( sdeVerifierIdx,CComVariant(PROG_NAME));
			ipSdeFeature->put_Value( sdeVerifyDateIdx, CComVariant(today) );
		}

		// �ǂ���ɂ���ҏW�ҁE�ҏW�������͍X�V
		message += _T("�yWORKER�z�yWORK_DATE�z");
		ipSdeFeature->put_Value( sdeWorkerIdx, CComVariant(PROG_NAME) );
		ipSdeFeature->put_Value( sdeWorkDateIdx, CComVariant(today) );

		// �L�����t�擾
		CComVariant oldUpdDate, sdeUpdDate;
		ipOldFeature->get_Value( pgdbUpdDateIdx, &oldUpdDate );
		ipSdeFeature->get_Value( sdeUpdDateIdx, &sdeUpdDate );
				
		// ��r�̂��߂�COleDateTime�ɂ������̂ň�USYSTEMTIME�Ŏ擾
		SYSTEMTIME sdeSysTime = {}, oldSysTime = {};
		VariantTimeToSystemTime( sdeUpdDate.dblVal, &sdeSysTime );
		VariantTimeToSystemTime( oldUpdDate.dblVal, &oldSysTime );
		COleDateTime sdeCTime( sdeSysTime ), oldCTime( oldSysTime );

		bool changeDate = true; // �L�����t��ύX���邩�ǂ���
		// SDE�I�u�W�F�N�g�̗L�����t�͎w�肳�ꂽ�L�����t����̓��t���H
		if( m_availDate <= sdeCTime )
		{
			fnPrintLogStyle( warning, cIndex, orgOid.lVal, _T("SDE���̗L�����t���w����t����̂��ߗL�����t�͕ύX���܂���i�|�C���g�͈ړ�����j�B") );
			changeDate = false;
		}

		// OLD_PGDB��SDE�ŗL�����t���ς���Ă�H
		if( changeDate && sdeCTime != oldCTime )
		{
			fnPrintLogStyle( warning, cIndex, orgOid.lVal, _T("SDE��ŗL�����t���ύX����Ă��邽�ߗL�����t�͕ύX���܂���i�|�C���g�͈ړ�����j�B") );
			changeDate = false;
		}

		// �L�����t�X�V�̕K�v���Ȃ���Ό`��ύX���Ď��̃t�B�[�`����
		if( !changeDate )
		{
			fnSiNDYPoiChange( ipSdeFeature, ipNewFeature, cIndex, orgOid.lVal, message );
			continue;
		}

		// �����܂ł�����L�����t���X�V CTime����DATE�ɕϊ�
		message += _T("�yUPD_AVAIL_DATE�z");
		SYSTEMTIME st;
		if( !m_availDate.GetAsSystemTime(st) )
		{
			_ASSERT( false );
			continue;
		}
		DATE availDate;
		SystemTimeToVariantTime( &st, &availDate );
		ipSdeFeature->put_Value( sdeUpdDateIdx, CComVariant(availDate) );
		fnSiNDYPoiChange( ipSdeFeature, ipNewFeature, cIndex, orgOid.lVal, message );
	}
	return true;
}

bool CImport::fnSiNDYPoiChange( IFeaturePtr& sindyFeature, const IFeaturePtr& pgdbFeature, long cIndex, long oid, CString& message )
{
	fnSetCommonAttr( sindyFeature, cIndex, pgdbFeature );
	message += _T("�ύX");
	return fnStoreFeature(sindyFeature, cIndex, oid, message);
}

// ���b�V���|���S�����폜�A���b�V�����E�͐؂����čX�V�i�폜�E�폜+�X�V�ł����g���Ȃ��j
bool CImport::fnClipData(long cIndex)
{
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR bstrName;
	m_ipFeatureClasses[cIndex]->get_ShapeFieldName( &bstrName );
	if(ipFilter->put_GeometryField(bstrName) != S_OK){
		return false;
	}
	if(ipFilter->putref_Geometry(m_ipMeshGeom) != S_OK){
		return false;
	}
	if(ipFilter->put_SpatialRel(esriSpatialRelContains) != S_OK){
		return false;
	}

	bool aResult = false;
	//���S����̂��̂��ɍ폜
	long aCount;
	m_ipFeatureClasses[cIndex]->FeatureCount(ipFilter, &aCount);
	_ftprintf(stderr, _T("#���S����ꂽ�I�u�W�F�N�g�� = %d\n"), aCount);
	if(aCount > 0){
		if(g_ShapeType[cIndex] == esriGeometryPoint){
			if(_tcscmp(gEnvTable[g_DataIndex].m_Suffix, _T("gou")) == 0){
				CString aWhere;
				aWhere.Format(_T("%s = 1"), GOUTYPE_NAME);
				ipFilter->put_WhereClause(CComBSTR(aWhere));
			}else{
				return false;
			}
		}
		if( !m_testMode ){
			_tprintf(_T("���S������폜���Ă��܂��B\n"));
			_ftprintf(stderr, _T("#���S������폜���Ă��܂��B\n"));
			HRESULT hr = ((ITablePtr)m_ipFeatureClasses[cIndex].m_T)->DeleteSearchedRows(ipFilter);
			if(SUCCEEDED(hr)){
				aResult = true;
				_tprintf(_T("���S������폜���܂����B(%d)\n"), hr);
				_ftprintf(stderr, _T("#���S������폜���܂����B(%d)\n"), hr);
			}else{
				fnPrintComError();
				_tprintf(_T("���S������폜���s�B(%d)\n"), hr);
				_ftprintf(stderr, _T("#���S������폜���s�B(%d)\n"), hr);
			}
	//		ipFilter->put_WhereClause(CComBSTR("")); //�|�C���g�p��Where�폜
		}
	}else{
		_tprintf(_T("���S����̃I�u�W�F�N�g������܂���B���E�̏��������܂�\n"));
		_ftprintf(stderr, _T("#���S����̃I�u�W�F�N�g������܂���B���E�̏��������܂�\n"));
	}
	//���S����̂��̂��ɍ폜�@�����܂�

	if(ipFilter->put_SpatialRel(esriSpatialRelIntersects) != S_OK){
		return false;
	}

	IFeatureCursorPtr ipFeatureCursor;
	m_ipFeatureClasses[cIndex]->FeatureCount(ipFilter, &aCount);
	_ftprintf(stderr, _T("#���S����폜��Ɍ������ꂽ�ΏۃI�u�W�F�N�g�� = %d\n"), aCount);

	if(m_ipFeatureClasses[cIndex]->Update(ipFilter, VARIANT_FALSE, &ipFeatureCursor) != S_OK){
		fnPrintComError();
		return false;
	}
	if(g_ShapeType[cIndex] == esriGeometryPolygon || g_ShapeType[cIndex] == esriGeometryPolyline){
		aResult |= fnCutObject(ipFeatureCursor, cIndex);
	}else if(g_ShapeType[cIndex] == esriGeometryPoint){
		aResult |= fnDeletePoint(ipFeatureCursor, cIndex);
	}else{
		_ftprintf(stderr, _T("#ERROR,�T�|�[�g���Ă��Ȃ��`���ł��B\n"));
		aResult = false;
	}
	return aResult;
}

// �V�K�ǉ������ꊇ�ǉ��֐�
bool CImport::fnAddNew(IFeatureClassPtr ipFeatureClass, long cIndex)
{
	using namespace sindy::schema::category;
	long orgObjIdIndex = -1;
	ipFeatureClass->FindField( CComBSTR(sindyc_table::kOrgOBJID), &orgObjIdIndex );

	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	ipFilter->putref_Geometry( m_ipMeshGeom );
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );
	CComBSTR shapeField;
	ipFeatureClass->get_ShapeFieldName( &shapeField );
	ipFilter->put_GeometryField( shapeField );

	IFeatureCursorPtr ipCursor;
	if(FAILED(ipFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor)))
	{
		fnPrintComError();
		return false;
	}

	IFeatureCursorPtr ipInsCursor;
	if( !m_testMode )
	{
		if(FAILED(m_ipFeatureClasses[cIndex]->Insert(VARIANT_TRUE, &ipInsCursor)))
		{
			fnPrintComError();
			return false;
		}
	}

	// �P���ɐV�K�ǉ��Ȃ��̂̏���
	IFeatureBufferPtr ipBuffer;
	m_ipFeatureClasses[cIndex]->CreateFeatureBuffer(&ipBuffer);
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK && ipFeature)
	{
#ifdef _DEBUG
		// TODO:�}���`�p�[�g����Insert�ł�������ۂ��B�v�����B(bug11114)
		long oid = 0;
		ipFeature->get_OID( &oid );
		if( oid == 143 )
			int stop = 0;
#endif

		if(m_Command == DIFF_UPDATE)
		{
			CComVariant orgObjId;
			ipFeature->get_Value( orgObjIdIndex, &orgObjId );
			if( !m_emptyDBMode )
			{
				if( orgObjId.vt != VT_NULL && orgObjId.lVal != 0 )
				{
					continue;
				}
			}
			else
			{
				// ��DB���[�h�͂�����ƃt���[���ς���Ă���̂ŁANEW����UPDATE_C�����Ȃ���X�V����B
				if( orgObjId.vt != VT_NULL && orgObjId.lVal != 0 )
				{
					CString table = g_LayerName[cIndex];
					CComVariant updateC;
					ipFeature->get_Value( g_UpdateCIndex[cIndex], &updateC );
					switch(updateC.lVal)
					{
						case SHAPE_CHG:
						case ATTR_CHG:
						case BOTH_CHG:
							{
								// �������̃��b�V������OLD�������Ȃ����Ƃ�����B(NEW�������b�V�������Č`��ύX�����Ƃ�)
								// ���̂Ƃ��͂����ōX�V���Ă��B
								const auto& onlyNew = std::find( m_onlyNewOid[table].begin(), m_onlyNewOid[table].end(), orgObjId.lVal );
								if( onlyNew == m_onlyNewOid[table].end() )
								{
									// OLD��������Ȃ�fnCompare�ōX�V����Ă���͂��Ȃ̂łƂ΂�
									continue;
								}
								break;
							}
						case NO_CHG:
						case DELETE_CHG:
							{
								// �ω��Ȃ��Ȃ�Ȃɂ����Ȃ�
								// �폜�Ȃ�fnCompare�őΉ��ς݂̂͂�
								continue;
								break;
							}
						case NEW_CHG:
							{
								// �V�K�͓��ɏ����Ȃ��������s��
								break;
							}
						default:
							break;
					}
				}
				// �؂�\���fnReflectShape�܂���
			}
		}

		if( !fnReflectShape( ipBuffer, ipFeature, cIndex, 0 ) )
			continue;

		CString aDummy;
		if( !fnReflectAttr(ipBuffer, ipFeature, cIndex, aDummy) )
			continue;

		if( !fnSetCommonAttr(ipBuffer, cIndex, ipFeature) )
			continue;

		if( m_sindyCImport )
		{
			if( FAILED(ipBuffer->put_Value( g_UpdateCIndex[cIndex], CComVariant(NEW_CHG) ) ) )
			{
				fnPrintComError();
				fnPrintLogStyle( error, cIndex, 0, _T("put_Value���s�B") );
				continue;
			}

			if( FAILED(ipBuffer->put_Value( g_OrgObjIdIndex[cIndex], CComVariant(0) ) ) )
			{
				fnPrintComError();
				fnPrintLogStyle( error, cIndex, 0, _T("put_Value���s�B") );
				continue;
			}
		}
		if(g_GouTypeIndex > -1 && cIndex == 0)
		{
			//������Ƃ���炵�����ǁACS�΍�
			//���^�C�v�̐ݒ�
			if( FAILED(ipBuffer->put_Value(g_GouTypeIndex, CComVariant(1))) )
			{
				fnPrintComError();
				fnPrintLogStyle( error, cIndex, 0, _T("put_Value���s�B") );
				continue;
			}
		}
		CComVariant aID = 0;
#ifndef _DEBUG
		CString msg = _T("�ǉ�");
#else
		CString msg;
		msg.Format(_T("%ld\t�ǉ�"), oid);
#endif
		fnInsertFeature(ipInsCursor, ipBuffer, aID, cIndex, msg);
	}

	// ���b�V�����E�̐؂�\��ŋ��E�ׂ��ő����Ⴂ�ɂȂ�A�V�K�����ɂȂ������̂̏���
	if( m_Command == DIFF_UPDATE )
	{
		for( auto& aInsertID : m_AppendID )
		{
			ipFeatureClass->GetFeature(aInsertID, &ipFeature);
			if( !ipFeature )
			{
				fnPrintLogStyle(error, cIndex, aInsertID, _T("�ǉ�����t�B�[�`����������Ȃ��B"));
				continue;
			}
			CComVariant aOrgID;
			ipFeature->get_Value(m_OrgIdIndex, &aOrgID);
			IFeaturePtr ipOrgFeature;
			m_ipFeatureClasses[cIndex]->GetFeature(aOrgID.lVal, &ipOrgFeature);
			IFeatureBufferPtr ipBuffer;
			m_ipFeatureClasses[cIndex]->CreateFeatureBuffer(&ipBuffer);
			if( !fnReflectShape( ipBuffer, ipFeature, cIndex, aOrgID.lVal ) )
				continue;

			CString aDummy;
			if( !fnReflectAttr(ipBuffer, ipFeature, cIndex, aDummy) )
				continue;

			if( !fnSetCommonAttr(ipBuffer, cIndex, ipOrgFeature) )
				continue;

			if( m_sindyCImport )
				ipBuffer->put_Value( g_UpdateCIndex[cIndex], CComVariant(NEW_CHG) );

			CComVariant aID = 0;
			CString msg = _T("�ǉ�(�V�K����)");
			fnInsertFeature(ipInsCursor, ipBuffer, aID, cIndex, msg);
		}
	}

	if( !m_testMode )
	{
		if( FAILED( ipInsCursor->Flush() ) )
		{
			fnPrintComError();
			return false;
		}
	}
	return true;
}

// Export���SiNDY��ɕύX��������ꂽ���A�X�V���t2��̔�r�Ŕ��f����֐�
// �֐����Ɩ߂�l���t���ۂ��C�����邪�A�X�V�ł��Ȃ��E�X�V���Ă͂����Ȃ�����false��Ԃ�
bool CImport::fnSiNDYChange(long cIndex, long cObjID, IFeaturePtr ipOrgFeature, IFeaturePtr ipPgdbFeature, LPCTSTR cChgType)
{
	//Export��ɕύX�����������`�F�b�N
	if( !ipOrgFeature )
	{
		m_ipFeatureClasses[cIndex]->GetFeature( cObjID, &ipOrgFeature );
		if( !ipOrgFeature )
		{
			long aObjID;
			ipPgdbFeature->get_OID( &aObjID );
			fnPrintLogStyle( error, cIndex, cObjID, _T("�ύX�Ώۂ�SiNDY��ō폜����Ă���B") );
			return false;
		}
	}

	CComVariant aSindyModDate;
	ipOrgFeature->get_Value( g_ModifyDateIndex[cIndex], &aSindyModDate );
	CComVariant aPgdbModDate;
	ipPgdbFeature->get_Value( m_OrgModifyDateIndex, &aPgdbModDate );
	if( aSindyModDate.date != aPgdbModDate.date )
	{
		if( m_tdcMode )
		{
			//�����D�惂�[�h��������A���ɍs������_��
			CString aMsg = CString(_T("�ύX�Ώۂ�SiNDY��ŕύX����Ă��邽�ߔ��f���܂���(")) + CString(cChgType) + _T(")");
			fnPrintLogStyle( warning, cIndex, cObjID, aMsg );
			return false;
		}
		else
		{
			//�ʏ�̓��b�Z�[�W����
			fnPrintLogStyle( warning, cIndex, cObjID, _T("�ύX�Ώۂ�SiNDY��ŕύX����Ă���B") );
		}
	}
	return true;
}

// ���b�V�����܂������I�u�W�F�N�g���𔻒肷��֐�
bool CImport::fnOverMesh(long cIndex, long cObjID, IFeaturePtr ipFeature)
{
	if( !ipFeature )
	{
		m_ipFeatureClasses[cIndex]->GetFeature(cObjID, &ipFeature);
		if( !ipFeature )
		{
			//fnSiNDYChange()�Œe�����炱���ɂ͗��Ȃ��͂�
			fnPrintLogStyle(critical, cIndex, cObjID, _T("�ύX�Ώۂ�SiNDY��ō폜����Ă���i�o�Ă͂����Ȃ����b�Z�[�W�j�B"));
			return false;
		}
	}

	IGeometryPtr ipGeom = fnGetClipShape(ipFeature, cIndex, cObjID);
	if( !ipGeom )
	{
		return false;
	}

	IPointCollectionPtr ipPoints(ipGeom);
	long aCount = 0;
	ipPoints->get_PointCount(&aCount);

	//�v�`�F�b�N�I
	if( ( g_ShapeType[cIndex] == esriGeometryPolygon && aCount > 3 )
		|| ( g_ShapeType[cIndex] == esriGeometryPolyline && aCount > 1 ) )
	{
		return true;
	}
	return false;
}

// ���b�V�����܂����I�u�W�F�N�g��؂�\�肵�čX�V����֐�
bool CImport::fnClipAndApend(IFeatureClassPtr ipNewClass, long cIndex, long cObjID, IFeaturePtr ipNewFeature, long cStatus)
{
	IFeaturePtr ipFeature;
	if( FAILED( m_ipFeatureClasses[cIndex]->GetFeature( cObjID, &ipFeature ) ) )
	{
		fnPrintComError();
		return false;
	}
	if( !ipFeature )
	{
		fnPrintLogStyle(error, cIndex, cObjID, _T("�t�B�[�`���[��������Ȃ��B"));
		return false;
	}

	//�؂�\�菈��
	IGeometryPtr ipGeom = fnGetRemakeShape(ipNewClass, ipFeature, cIndex, cObjID);
	if( !ipGeom )
	{
		fnPrintLogStyle(error, cIndex, cObjID, _T("���b�V�����E�؂�\�莸�s�B"));
		return false;
	}

	IGeometryCollectionPtr ipGeoCol = ipGeom;
	long aCount = 0;
	if( FAILED( ipGeoCol->get_GeometryCount( &aCount ) ) )
	{
		fnPrintComError();
		return false;
	}
	if( aCount > 1 )
	{
		esriGeometryType aShapeType;
		if( FAILED( m_ipFeatureClasses[cIndex]->get_ShapeType( &aShapeType ) ) )
		{
			fnPrintLogStyle( error, cIndex, cObjID, _T("ShapeType�擾���s�B") );
			return false;
		}
		switch(aShapeType)
		{
		case esriGeometryPolygon:
			{
				msi_get_parts_info aPolUtil;
				if( aPolUtil.Init( ipGeom, cObjID, (string)CT2A(g_LayerName[cIndex]).m_psz, 0, stderr ) )
				{
					aCount = aPolUtil.Get_Part_Num();
					if( aCount > 1 )
					{
						cStatus = NO_CHG;	//�����̕ύX�͔��f�����Ȃ�����
						fnPrintLogStyle( warning, cIndex, cObjID, _T("�č\���㕡���ɕ����ꂽ�B") );
					}
				}
				else
				{
					fnPrintLogStyle( error, cIndex, cObjID, _T("�č\����|���S���̕��͎��s�B") );
					return false;
				}
			}
			break;
		case esriGeometryPolyline:
			{
				cStatus = NO_CHG;	//�����̕ύX�͔��f�����Ȃ�����
				fnPrintLogStyle( warning, cIndex, cObjID, _T("�č\���㕡���ɕ����ꂽ(���C��)�B") );
			}
			break;
		default:
			{
				fnPrintLogStyle( critical, cIndex, cObjID, _T("�T�|�[�g�O�i��΃o�O�j�B") );
				return false;
			}
			break;
		}
	}
	else if(aCount < 1)
	{
		if( !ipNewFeature || cStatus & ATTR_CHG )
		{
			//�P���폜�ŉ�
			return fnSiNDYDeleteCore(cIndex, ipFeature, cObjID, _T("���b�V�����E�폜"));
		}
		else
		{
			fnPrintLogStyle( error, cIndex, cObjID, _T("�؂�\���̌`�󂪕s���B") );
			return false;
		}
	}

	ipFeature->putref_Shape(ipGeom);
	fnSetCommonAttr(ipFeature, cIndex, ipFeature);
	CString logMessage = _T("���b�V�����E�ύX");
	return fnStoreFeature(ipFeature, cIndex, cObjID, logMessage);
}

// ���b�V�����܂����ł��Ȃ����̂̒P���X�V�֐�
bool CImport::fnReflectToSiNDY(IFeaturePtr ipNewFeature, long cIndex, long cObjID, long cStatus)
{
	IFeaturePtr ipFeature;
	if( FAILED( m_ipFeatureClasses[cIndex]->GetFeature( cObjID, &ipFeature ) ) )
	{
		fnPrintComError();
		return false;
	}
	if( !ipFeature )
	{
		fnPrintLogStyle( critical, cIndex, cObjID, _T("�t�B�[�`���[��������Ȃ��B") );
		return false;
	}
		
	CString aMsg;
	if( ( cStatus & SHAPE_CHG ) )
	{
		//�`��ύX����
		if( !fnReflectShape( ipFeature, ipNewFeature, cIndex, cObjID ) )
			return false;
		aMsg = _T("�y�`��z");
	}
	if( ( cStatus & ATTR_CHG ) )
	{
		//�����ύX����
		if( !fnReflectAttr( ipFeature, ipNewFeature, cIndex, aMsg ) )
			return false;
	}

	if( !fnSetCommonAttr( ipFeature, cIndex, ipFeature ) )
		return false;

	aMsg += _T("�ύX");
	if( !fnStoreFeature( ipFeature, cIndex, cObjID, aMsg ) )
		return false;

	return true;
}

// ���b�V���|���S���ŃN���b�v���čX�V�܂��͍폜����֐�
bool CImport::fnCutObject(IFeatureCursorPtr ipCursor, long cIndex)
{
	IRelationalOperatorPtr ipRel = m_ipMeshGeom;
	IFeaturePtr ipFeature;
	bool aResult = false;
	HRESULT aHResult;
	int aCount = 0;

	while( ( aHResult = ipCursor->NextFeature(&ipFeature) ) == S_OK && ipFeature )
	{
		aCount++;
		long aObjID = 0;
		ipFeature->get_OID( &aObjID );
		IGeometryPtr ipGeometry;
		if( FAILED( ipFeature->get_Shape(&ipGeometry) ) ){
			fnPrintComError();
			return false;
		}
		VARIANT_BOOL aIsTouch = VARIANT_FALSE;
		if( FAILED( ipRel->Touches(ipGeometry, &aIsTouch) ) ){
			fnPrintLogStyle(error, cIndex, aObjID, _T("����s�\�B"));
			continue;
		}

		//�O���Őڂ���̂͑ΏۊO
		if( aIsTouch ){
			fnPrintLogStyle(info, cIndex, aObjID, _T("�ΏۊO�B"));
			aResult = true;
		}

		VARIANT_BOOL aIsCross = VARIANT_FALSE;
		if( g_ShapeType[cIndex] == esriGeometryPolygon ){
			ipRel->Overlaps(ipGeometry, &aIsCross);
		}else{
			ipRel->Crosses(ipGeometry, &aIsCross);
		}

		if( aIsCross ){
			IGeometryPtr ipGeom = fnGetClipShape(ipFeature, cIndex, aObjID, true);
			if( ipGeom ){
				ipFeature->putref_Shape(ipGeom);
				CString logMessage = _T("�ό`");
				aResult = fnStoreFeature( ipFeature, cIndex, aObjID, logMessage );
			}else{
				//���̂܂܍폜
				CString logMessage = _T("�폜");
				aResult = fnStoreFeature( ipFeature, cIndex, aObjID, logMessage );
			}
		}else{
			VARIANT_BOOL aIsContains = VARIANT_FALSE;
			if(FAILED(ipRel->Contains(ipGeometry, &aIsContains))){
				fnPrintComError();
				fnPrintLogStyle(error, cIndex, aObjID, _T("��܊֌W����s�\�B"));
				continue;
			}

			if(aIsContains){
				//���S����i��ɏ����Ă�͂������ǁA�O�̂��߁j
				//���̂܂܍폜
				CString logMessage = _T("�폜");
				aResult = fnStoreFeature( ipFeature, cIndex, aObjID, logMessage );
			}else{
				//���b�V�������S�Ɋ܂ށi�s���E�E�C���ł͂��肦��j
				IGeometryPtr ipGeom = fnGetClipShape(ipFeature, cIndex, aObjID, true);
				if( ipGeom ){
					ipFeature->putref_Shape(ipGeom);
					CString logMessage = _T("�ό`(�J�蔲��)");
					aResult = fnStoreFeature( ipFeature, cIndex, aObjID, logMessage );
				}else{
					//���̂܂܍폜
					aResult = fnSiNDYDeleteCore(cIndex, ipFeature, aObjID, _T("�폜(�v�m�F)"));
				}
			}
		}
	}
	_ftprintf(stderr, _T("#HRESULT = %d\n"), aHResult);
	_ftprintf(stderr, _T("#��������Count = %d\n"), aCount);
	if( FAILED(aHResult) ){
		fnPrintComError();
	}
	return aResult;
}

// �������ꂽ�I�u�W�F�N�g�𖳏����ō폜����֐��i��Ƀ|�C���g�p�j
bool CImport::fnDeletePoint(IFeatureCursorPtr ipCursor, long cIndex)
{
	IFeaturePtr ipFeature;
	HRESULT aHResult;
	int aCount = 0;
	while((aHResult = ipCursor->NextFeature(&ipFeature)) == S_OK && ipFeature){
		aCount++;
		//���̂܂܍폜
		long aObjID = 0;
		ipFeature->get_OID(&aObjID);
		fnSiNDYDeleteCore(cIndex, ipFeature, aObjID, _T("�폜"));
	}
	_ftprintf(stderr, _T("#HRESULT = %d\n"), aHResult);
	_ftprintf(stderr, _T("#��������Count = %d\n"), aCount);
	if(FAILED(aHResult)){
		fnPrintComError();
	}
	return true;
}

// ���f�Ώۑ����̃R�s�[�֐�
bool CImport::fnReflectAttr(IFeaturePtr ipTgtFeature, IFeaturePtr ipSrcFeature, long cIndex, CString& cField)
{
	for(int i = 0; gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_fieldName != NULL; i++){
		CComVariant aSrcVal, aTgtVal;
		ipSrcFeature->get_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_NewPgdbIndex, &aSrcVal);
		ipTgtFeature->get_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SindyIndex, &aTgtVal);
		if(aSrcVal != aTgtVal){
			ipTgtFeature->put_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SindyIndex, aSrcVal);
			cField += _T("�y");
			cField += gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_fieldName;
			cField += _T("�z");
		}
	}
	return true;
}

// ���b�V���|���S���ŃN���b�v�����`������߂�
// fnGetRemakeShape�Ǝ��Ă���悤�Ɍ����邪�A������́A���b�V���|���S���Ő؂������c��̃W�I���g�������߂�
IGeometryPtr CImport::fnGetClipShape(IFeaturePtr ipFeature, long cIndex, long cObjID, bool cForceMode)
{
	IGeometryPtr ipGeom;
	ipFeature->get_ShapeCopy(&ipGeom);

	if(!cForceMode){
		//�O�̃��b�V���̉e���Ń}���`�ł��؂��肪�K�v�ɂȂ邱�Ƃ����邩��A
		//���Ղɏȗ��ł��Ȃ��@2008/02/01
		if(g_ShapeType[cIndex] == esriGeometryPolygon){
			msi_get_parts_info aPolUtil;
			if(aPolUtil.Init(ipGeom, cObjID, (string)CT2A(g_LayerName[cIndex]).m_psz, 0, stderr)){
				if(aPolUtil.Get_Part_Num() > 1){
					//���������ǁA�}���`�p�[�g�Ȃ�ԈႢ�Ȃ����b�V�����E�̂͂�
					//�������Ȃ��A�؂�\��̓r���ł̂݃}���`�p�[�g�ɂȂ�O��
					return ipGeom;
				}
			}else{
				//�ǂ����悤���Ȃ�
				fnPrintLogStyle(error, cIndex, cObjID, _T("���莞�|���S���̕��͎��s�B"));
				return ipGeom;
			}
		}else{
			IGeometryCollectionPtr ipGeoCol = ipGeom;
			long aCount;
			if(FAILED(ipGeoCol->get_GeometryCount(&aCount))){
				fnPrintComError();
				return ipGeom;
			}
			if(aCount > 1){
				//���������ǁA�}���`�p�[�g�Ȃ�ԈႢ�Ȃ����b�V�����E�̂͂�
				//�������Ȃ��A�؂�\��̓r���ł̂݃}���`�p�[�g�ɂȂ�O��
				return ipGeom;
			}
		}
	}
	ITopologicalOperatorPtr ipTopo = ipGeom;
	IGeometryPtr ipTgtGeom;
	//���f�[�^�����b�V���Ő؂���
	if(FAILED(ipTopo->Difference(m_ipMeshGeom, &ipTgtGeom))){
		fnPrintComError();
		fnPrintLogStyle(error, cIndex, cObjID, _T("�؂��莸�s�B"));
		return ipGeom;
	}
	ipTgtGeom->putref_SpatialReference( m_ipSpRef );
	return ipTgtGeom;
}

// ���b�V�����E�̃I�u�W�F�N�g�ɑ΂��āA���Y���b�V���O�����Ɠ��Y���b�V�����̐؂�\����s��
// �}���`�p�[�g�ɂȂ邱�Ƃ�����A�I���W�i��ID���������̂�1�ɂ܂Ƃ߂悤�Ƃ��邪�A�������ύX�ɂȂ��Ă�����̂�����ΐV�K�ǉ������Ƃ���
IGeometryPtr CImport::fnGetRemakeShape(IFeatureClassPtr ipNewClass, IFeaturePtr ipFeature, long cIndex, long cObjID)
{
	IGeometryPtr ipGeom;
	ipFeature->get_ShapeCopy(&ipGeom);

	ITopologicalOperatorPtr ipTopo = ipGeom;
	IGeometryPtr ipTgtGeom;
	//���f�[�^�����b�V���Ő؂���
	if(FAILED(ipTopo->Difference(m_ipMeshGeom, &ipTgtGeom))){
		fnPrintComError();
		fnPrintLogStyle(error, cIndex, cObjID, _T("�؂��莸�s�B"));
		return NULL;
	}

	ipTgtGeom->putref_SpatialReference( m_ipSpRef );

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString aWhere;
	aWhere.Format(_T("%s = %d"), ORG_OBJID_NAME, cObjID);
	ipFilter->put_WhereClause(CComBSTR(aWhere));

	IFeatureCursorPtr ipCursor;
	if(FAILED(ipNewClass->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return NULL;
	}
	IFeaturePtr ipSrcFeature;
	while(ipCursor->NextFeature(&ipSrcFeature) == S_OK && ipSrcFeature){
		ipTopo = ipTgtGeom;
		CComVariant aUpdate;
		ipSrcFeature->get_Value(m_ChgStatusIndex, &aUpdate);
		if(aUpdate.lVal & ATTR_CHG){
			//�����ύX����������͐V�K����
			long aObjID = 0;
			ipSrcFeature->get_OID(&aObjID);
			m_AppendID.insert(aObjID);
			continue;
		}

		IGeometryPtr ipSrcGeom, ipTgtGeom2;
		ipSrcFeature->get_Shape(&ipSrcGeom);
		if( FAILED(ipTopo->Union(ipSrcGeom, &ipTgtGeom2)) )
		{
			fnPrintComError();
			fnPrintLogStyle(error, cIndex, cObjID, _T("�؂����̃}�[�W���s�B"));
			return NULL;
		}
		ipTgtGeom = ipTgtGeom2;
	}

	return ipTgtGeom;
}

// sindy�̃t�B�[�`�����폜���� (GS�Ή�)
bool CImport::fnSiNDYDelete( const int cIndex )
{
	// Building�̏ꍇ�͍폜�����GEOSPACE_ID���o���K�v������
	bool isBuilding = false;
	if( 0 == ((CString)gEnvTable[g_DataIndex].m_OrgTableNames[cIndex]).CompareNoCase( building::kTableName ) )
		isBuilding = true;

	// ���b�V����`�Ƃ�InterSects�擾
	IFeatureCursorPtr ipCursor;
	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	CComBSTR bstrFieldName;
	m_ipFeatureClasses[cIndex]->get_ShapeFieldName( &bstrFieldName );
	ipFilter->put_GeometryField( bstrFieldName );
	ipFilter->putref_Geometry( m_ipMeshGeom );
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );
	
	m_ipFeatureClasses[cIndex]->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	if( !ipCursor )
		return false;

	long geoIDIndex = -1;
	m_ipFeatureClasses[cIndex]->FindField( CComBSTR(GEOSPACE_ID_NAME), &geoIDIndex );

	// �폜
	IFeaturePtr ipFeature;
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature )
	{
		long oid = 0;
		ipFeature->get_OID( &oid );
		// ���b�V����`�̊O���ɂ͂ݏo���悤�Ȃ�͊O�������̌`����c��
		IGeometryPtr ipGeom, ipDiffGeom;
		ipFeature->get_ShapeCopy( &ipGeom );
		ITopologicalOperatorPtr ipTopo( ipGeom );
		if( FAILED(ipTopo->Difference( m_ipMeshGeom, &ipDiffGeom )) ) // ���b�V���̊O���擾
		{
			fnPrintComError();
			fnPrintLogStyle(error, cIndex, oid, _T("���b�V���`��ł̐؂��莸�s�B"));
			continue;
		}
		ipDiffGeom->putref_SpatialReference( m_ipSpRef );
		esriGeometryType geomType = esriGeometryNull;
		ipGeom->get_GeometryType( &geomType );
		switch( geomType )
		{
		case esriGeometryPolyline:
			{
				// �݂͂������̒���
				// ���C���̓��b�V�����E�Ő؂�Ă���C������
				double length = 0;
				((IPolylinePtr)ipDiffGeom)->get_Length( &length );
				if( length == 0 )
				{
					// �͂ݏo���ĂȂ��Ȃ�폜
					fnSiNDYDeleteCore( cIndex, ipFeature, oid, _T("�폜�B") );
				}
				else
				{
					// �͂ݏo���Ă�Ȃ�A�͂ݏo�����̌`�󂾂��c��
					fnSiNDYPutDifferenceGeom( cIndex, ipFeature, oid, ipDiffGeom );
				}
				break;
			}
		case esriGeometryPolygon:
			{
				// �݂͂������̖ʐ�
				double area = 0;
				((IAreaPtr)ipDiffGeom)->get_Area( &area );
				if( area == 0 )
				{
					CString msg = _T("�폜�B");
					if( isBuilding )
					{
						// �ƌ`�폜����ꍇ��GEOSPACEID���擾
						CComVariant geoID;
						ipFeature->get_Value( geoIDIndex, &geoID );
						CString strGeoID( geoID.bstrVal );
						if( strGeoID.IsEmpty() )
							strGeoID = _T("�Ȃ�");
						msg.AppendFormat( _T(" GEOSPACE_ID�F%s"), strGeoID );
					}
					// �͂ݏo���ĂȂ��Ȃ�폜
					fnSiNDYDeleteCore( cIndex, ipFeature, oid, msg );
				}
				else
				{
					// �͂ݏo���Ă�Ȃ�A�͂ݏo�����̌`�󂾂��c��
					fnSiNDYPutDifferenceGeom( cIndex, ipFeature, oid, ipDiffGeom );
				}
				break;
			}
		default:
			{
				// ����Ƃ�����|�C���g
				fnSiNDYDeleteCore( cIndex, ipFeature, oid, _T("�폜�B") );
				break;
			}
		}
	}

	return true;
}

bool CImport::fnSiNDYDeleteCore( const int cIndex, const IFeaturePtr& ipFeature, const long oid, const CString& message )
{
	if( m_testMode )
	{
		fnPrintLogStyle( info, cIndex, oid, _T("�폜�����B") );
		return true;
	}

	if( FAILED( ipFeature->Delete() ) )
	{
		fnPrintComError();
		fnPrintLogStyle( error, cIndex, oid, _T("�폜���s�B") );
		return false;
	}
	fnPrintLogStyle( info, cIndex, oid, message );
	return true;
}

bool CImport::fnSiNDYPutDifferenceGeom( const int cIndex, const IFeaturePtr& ipFeature, const long oid, const IGeometryPtr& ipDiffGeom )
{
	ipFeature->putref_Shape( ipDiffGeom );
	CString logMessage = _T("�`��ύX");
	return fnStoreFeature( ipFeature, cIndex, oid, logMessage );
}

template <typename T>
bool CImport::fnReflectShape( T& ipTgtFeature, const IFeaturePtr& ipSrcFeature, long cIndex, long cObjID )
{
	IGeometryPtr ipGeom;
	ipSrcFeature->get_ShapeCopy( &ipGeom );

	// GS���ƈȍ~�́ANULL�`��̃I�u�W�F�N�g������\��������
	// ���������̂͂����Ń��^�[�����Ă��܂�
	if( !ipGeom )
		return false;

	// �����Ώۃ��b�V������͂ݏo�����͐؂��Ă���
	IEnvelopePtr ipMeshEnvelope;
	m_ipMeshGeom->get_Envelope( &ipMeshEnvelope );
	if( FAILED(((ITopologicalOperatorPtr)ipGeom)->Clip( ipMeshEnvelope )) )
	{
		fnPrintComError();
		fnPrintLogStyle( error, cIndex, cObjID, _T("Clip���s�B") );
		return false;
	}

	// �����ň������̂̓��b�V����intersects�Ŏ���Ă���I�u�W�F�N�g�B
	// GS�V�t���[���ƌ��f�[�^�����b�V���͈͓��Ɏ��܂��ĂȂ��̂ŁA
	// Clip��������null�`��Ƃ������Ƃ����肤��B���������Ƃ���return false�B
	esriGeometryType type = esriGeometryNull;
	ipGeom->get_GeometryType( &type );
	switch( type )
	{
	case esriGeometryPolygon:
		{
			double area = 0;
			((IAreaPtr)ipGeom)->get_Area( &area );
			if( area == 0 )
			{
				return false;
			}
			break;
		}
	case esriGeometryPolyline:
		{
			double length = 0;
			((IPolylinePtr)ipGeom)->get_Length( &length );
			if( length == 0 )
			{
				return false;
			}
			break;
		}
	default:
		break;
	}

	if( FAILED( ipTgtFeature->putref_Shape( ipGeom ) ) )
	{
		fnPrintComError();
		fnPrintLogStyle( error, cIndex, cObjID, _T("�`��R�s�[���s�B") );
		return false;
	};
	return true;
}

bool CImport::fnStoreFeature( const IFeaturePtr& sindyFeature, long cIndex, long oid, CString &message )
{
	if( m_testMode )
	{
		message += _T("�����B");
		fnPrintLogStyle( info, cIndex, oid, message );
		return true;
	}

	if( FAILED( sindyFeature->Store() ) )
	{
		message += _T("���s�B");
		fnPrintComError();
		fnPrintLogStyle( error, cIndex, oid, message );
		return false;
	}
	message += _T("�B"); //�����b�Z�[�W�ƍ��킹�邽�߂̋�_
	fnPrintLogStyle( info, cIndex, oid, message );
	return true;
}

bool CImport::fnInsertFeature(IFeatureCursorPtr& ipInsCursor, const IFeatureBufferPtr& ipBuffer, CComVariant &aID, long cIndex, CString& message)
{
	if( m_testMode )
	{
		message += _T("�����B");
		fnPrintLogStyle(info, cIndex, aID.lVal, message);
		return true;
	}

	if( FAILED( ipInsCursor->InsertFeature( ipBuffer, &aID ) ) )
	{
		message += _T("���s�B");
		fnPrintLogStyle(error, cIndex, aID.lVal, message);
		return false;
	}

	message += _T("�B");

	fnPrintLogStyle(info, cIndex, aID.lVal, message);
	return true;
}

bool CImport::fnInsertIntoEmptyDB( const IFeaturePtr& ipNewFeature, long cIndex, long updateStatus /* = -1 */ )
{
	IFeatureCursorPtr ipInsertCursor;
	if( !m_testMode )
	{
		if( FAILED( m_ipFeatureClasses[cIndex]->Insert( VARIANT_TRUE, &ipInsertCursor ) ) )
		{
			fnPrintComError();
			return false;
		}
	}
	IFeatureBufferPtr ipFeature;
	m_ipFeatureClasses[cIndex]->CreateFeatureBuffer( &ipFeature );
	if( !fnReflectShape( ipFeature, ipNewFeature, cIndex, 0 ) )
		return false;

	CString dummy;
	if( !fnReflectAttr( ipFeature, ipNewFeature, cIndex, dummy ) )
		return false;

	if( !fnSetCommonAttr( ipFeature, cIndex, ipNewFeature ) )
		return false;

	// updateStatus == -1 �Ȃ� fnSetCommonAttr() �ŃZ�b�g���ꂽ�l�̂܂܂ŗǂ�
	if( m_sindyCImport && updateStatus != -1 )
	{
		if( FAILED(ipFeature->put_Value( g_UpdateCIndex[cIndex], CComVariant(updateStatus) ) ) )
		{
			fnPrintComError();
			fnPrintLogStyle( error, cIndex, 0, _T("put_Value���s�B") );
			return false;
		}

		// �V�K�����̂��̂�ORG_OBJID���K��0�ɂȂ�悤�ɂ���B
		if( updateStatus == NEW_CHG )
		{
			if( FAILED(ipFeature->put_Value( g_OrgObjIdIndex[cIndex], CComVariant(0) ) ) )
			{
				fnPrintComError();
				fnPrintLogStyle( error, cIndex, 0, _T("put_Value���s�B") );
				return false;
			}
		}
	}

	CComVariant id;
	CString msg = _T("�ǉ��i��DB�C���|�[�g�j");
	if( !fnInsertFeature( ipInsertCursor, ipFeature, id, cIndex, msg ) )
		return false;

	return true;
}

IWorkspacePtr CImport::fnOpenWorkspaceFromFile(LPCTSTR filePath)
{
	IWorkspacePtr ipWorkspace = sindy::create_workspace(filePath);
	if( !ipWorkspace )
	{
		_ftprintf( stderr, _T("#ERROR,%s ���J���܂���ł���\n"), filePath );
		return nullptr;
	}
	return ipWorkspace;
}
