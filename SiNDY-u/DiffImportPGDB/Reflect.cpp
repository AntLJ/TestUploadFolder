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
#include "Reflect.h"
#include "Import.h"
#include "MeshMerge.h"
#include "SeparatePolygon.h"
#include "DelSinglePoint.h"

using namespace sindy::schema;

namespace Reflect
{
	esriGeometryType g_ShapeType[fc_max_size] = {esriGeometryNull};
	CString g_LayerName[fc_max_size];
	long g_OperatorIndex[fc_max_size] = {-1};
	long g_UpdateIndex[fc_max_size] = {-1};
	long g_PurposeIndex[fc_max_size] = {-1};
	long g_ClaimIndex[fc_max_size] = {-1};
	long g_ProgNameIndex[fc_max_size] = {-1};
	long g_ModifyDateIndex[fc_max_size] = {-1};
	long g_ProgModifyDateIndex[fc_max_size] = {-1};
	long g_SourceIndex[fc_max_size] = {-1};
	long g_OrgOperatorIndex[fc_max_size];
	long g_OrgModifyIndex[fc_max_size];
	long g_OrgObjIdIndex[fc_max_size];
	long g_UpdateCIndex[fc_max_size];
	long g_CheckCIndex[fc_max_size];
	long g_MeshcodeIndex[fc_max_size];
	long g_GouTypeIndex = -1;
	long g_CurrentMesh = -1;
	bool g_IsContinue = false;
	CString g_Suffix = _T("");
	MESH_MAP g_MeshMap;
	MESH_POLY_MAP g_MeshPolyMap;
	long g_NowProccess = -1;
	int g_DataIndex = -1;
	CString g_Operator = _T("");
	
	CString gMeshPoly = _T("CityMesh");

	LPCTSTR GOUTYPE_NAME = _T("GOUTYPE");
	LPCTSTR PROG_NAME = _T("DiffImportPGDB");
}

namespace {
	
	// GS�Ή��Œǉ� "City_Area", "City_Bridge_Line", "City_Tunnel", "City_Road", "Building_Roof"
	// [2015.01] GS�Ή��Ŏ��̃��C����ǉ� "City_Divider" �s�v�ɂȂ����̂Ŏ��̃��C�����폜 "City_Bridge_Line"
	LPCTSTR gFrmTables[] = {
		city_site::kTableName, city_line::kTableName, city_area::kTableName,
		city_tunnel::kTableName, city_road::kTableName, city_divider::kTableName, NULL};
	LPCTSTR gHigTables[] = {
		building::kTableName, building_line::kTableName, building_step::kTableName,
		building_roof::kTableName, NULL};
	LPCTSTR gRalTables[] = {city_station::kTableName, city_railway::kTableName, NULL};
	LPCTSTR gAdmTables[] = {city_admin::kTableName, NULL};
	LPCTSTR gGouTables[] = {gou_point::kTableName, cs_addr_point::kTableName, NULL};
//	LPCTSTR gAnoTables[] = {_T("City_Annotation"), _T("City_Disp_Line"), NULL};
	LPCTSTR gRefTables[] = {_T("RefS_Point"), NULL};
	LPCTSTR gPoiTables[] = {_T("Export_Poi_Point"), NULL};

	//�T�[�o��̕ʂ̃t�B�[�`���N���X�Ƀ��[�h����ꍇ�̂��߂ɕ�����
	//���ϐ��ŕύX�\
	CString gTgtFrmTables[] = {
		city_site::kTableName, city_line::kTableName, city_area::kTableName,
		city_tunnel::kTableName, city_road::kTableName, city_divider::kTableName, NULL};
	CString gTgtHigTables[] = {
		building::kTableName, building_line::kTableName, building_step::kTableName,
		building_roof::kTableName, NULL};
	CString gTgtRalTables[] = {city_station::kTableName, city_railway::kTableName, NULL};
	CString gTgtAdmTables[] = {city_admin::kTableName, NULL};
	CString gTgtGouTables[] = {gou_point::kTableName, cs_addr_point::kTableName, NULL};
	CString gTgtAnoTables[] = {city_annotation::kTableName, city_disp_line::kTableName, NULL};
	CString gTgtRefTables[] = {_T("RefS_Point"), NULL};
	CString gTgtPoiTables[] = {sindyk::poi_point::kTableName, NULL};


	// ����
	FieldTable gHigPFields[] = {
		{building::kBldClass,  -1, -1, -1, true},
		{building::kCorridor,  -1, -1, -1, true},
		{building::kNoWall,    -1, -1, -1, true}, // GEOSPACE�V��
		{building::kGeospaceID, -1, -1, -1, true}, // GEOSPACE�V��
		{NULL, -1, -1, -1, false}
	};

	// �������C��
	FieldTable gHigAFields[] = {
		{building_line::kBldClass, -1, -1, -1, true},
		{NULL, -1, -1, -1}
	};

	// �����K��
	FieldTable gStpPFields[] = {
		{building_step::kFloors, -1, -1, -1, true},
		{NULL, -1, -1, false}
	};
	
	// GEOSPACE�V�� ��������
	FieldTable gFroPFields[] = {
		{building_roof::kRfClass, -1, -1, -1, true}, // ����{�ݎ��
		{NULL, -1, -1}
	};

	FieldTable *gHigFields[] = {
		gHigPFields,
		gHigAFields,
		gStpPFields,
		gFroPFields
	};

	// �w�i
	FieldTable gFrmPFields[] = {
		{city_site::kBgClass,  -1, -1, -1, true},
		{NULL, -1, -1, -1, false}
	};

	// �w�i���C��
	FieldTable gFrmAFields[] = {
		{city_line::kBgClass, -1, -1, -1, true},
		{NULL, -1, -1, -1, false}
	};
	
	// GEOSPACE�V�� �s�s�n�}�G���A
	FieldTable gArePFields[] = {
		{city_area::kAreaClass, -1, -1, -1, true}, // �G���A���
		{NULL, -1, -1}
	};
	
	// GEOSPACE�V�� �s�s�n�}�g���l��
	FieldTable gTunPFields[] = {
		{city_tunnel::kTunnelClass, -1, -1, -1, true}, // �g���l�����
		{NULL, -1, -1}
	};

	// GEOSPACE�V�� �s�s�n�}���H
	FieldTable gRodPFields[] = {
		{city_road::kRoadClass, -1, -1}, // ���H�|���S�����
		{NULL, -1, -1}
	};

	// GEOSPACE�V�� �s�s�n�}������������
	FieldTable gDivPFields[] = {
		{city_divider::kDividerClass, -1, -1, -1, true}, // �����������ю��
		{NULL, -1, -1}
	};

	FieldTable *gFrmFields[] = {
		gFrmPFields,
		gFrmAFields,
		gArePFields,
		gTunPFields,
		gRodPFields,
		gDivPFields
	};

	// �S���w
	FieldTable gRalPFields[] = {
		{city_station::kStationClass, -1, -1, -1, true},
		{city_station::kUnderGround, -1, -1, -1, true},
		{NULL, -1, -1}
	};

	// �S�����C��
	FieldTable gRalAFields[] = {
		{city_railway::kRailwayClass, -1, -1, -1, true},
		{city_railway::kUnderGround, -1, -1, -1, true},
		{NULL, -1, -1, -1, false}
	};

	FieldTable *gRalFields[] = {
		gRalPFields,
		gRalAFields
	};

	// �s���E
	FieldTable gAdmPFields[] = {
		{city_admin::kCityCode,  -1, -1, -1, true},
		{city_admin::kAddrCode, -1, -1, -1, true},
		{city_admin::kAddrCode2, -1, -1, -1, true},
		{city_admin::kGaikuFugo, -1, -1, -1, true},
		{city_admin::kExtGaikuFugo, -1, -1, -1, true},
		{city_admin::kAddrClass, -1, -1, -1, true},
		{city_admin::kAreaClass, -1, -1, -1, true},
		{city_admin::kManage, -1, -1, -1, false},
		{city_admin::kColorCode, -1, -1, -1, false},
		{city_admin::kConvType, -1, -1, -1, true},
		{NULL, -1, -1, -1, false}
	};

	FieldTable *gAdmFields[] = {
		gAdmPFields,
	};

	// �����
	FieldTable gGouPFields[] = {
		{gou_point::kGouNo,  -1, -1, -1, false},
		{gou_point::kExpGouNo, -1, -1, -1, false},
		{gou_point::kPriority, -1, -1, -1, false},
		{gou_point::kConfirm, -1, -1, -1, false},
		{NULL, -1, -1}
	};

	// CS���
	FieldTable gCsPFields[] = {
		{cs_addr_point::kAddrCode,  -1, -1, -1, false},
		{cs_addr_point::kSource,  -1, -1, -1, false},
		{NULL, -1, -1, false}
	};

	FieldTable *gGouFields[] = {
		gGouPFields,
		gCsPFields
	};

	// ���L���
	FieldTable gAnoPFields[] = {
		{city_annotation::kNameString1,  -1, -1, -1, false},
		{city_annotation::kNameString2, -1, -1, -1, false},
		{city_annotation::kStringNum1, -1, -1, -1, false},
		{city_annotation::kStringNum2, -1, -1, -1, false},
		{city_annotation::kAnnoCode, -1, -1, -1, false},
		{city_annotation::kDispType, -1, -1, -1, false},
		{NULL, -1, -1, false}
	};

	FieldTable gAnoAFields[] = {
		{city_disp_line::kFontSize, -1, -1, -1, false},
		{city_disp_line::kTargetStr, -1, -1, -1, false},
		{NULL, -1, -1, -1, false}
	};

	FieldTable *gAnoFields[] = {
		gAnoPFields,
		gAnoAFields
	};

	// �����Њg�[��Ɨp���
	FieldTable gRefPFields[] = {
		{_T("ADDRCODE"), -1, -1, -1, false},
		{_T("GOU_NO"),  -1, -1, -1, false},
		{NULL, -1, -1, false}
	};

	FieldTable *gRefFields[] = {
		gRefPFields,
	};
	
	// POI_POINT�C���|�[�g�Ώۂ͈ܓx�o�x�ƗL�����t�̂�
	// ���Ȃ݂� ContentsSeq, SerialID, Name �̓G�N�X�|�[�g�ΏۂɂȂ��Ă���
	FieldTable gPoiPFields[] = {
		{sindyk::poi_point::kUPDAvailDate, -1, -1, -1, false},
		{NULL, -1, -1, -1, false}
	};

	FieldTable *gPoiFields[] = {
		gPoiPFields
	};
}
namespace Reflect{
	const EnvTable gEnvTable[] = {
		{_T("hig"), gHigTables, gTgtHigTables, gHigFields, CHANGE_TYPE_NORMAL},
		{_T("frm"), gFrmTables, gTgtFrmTables, gFrmFields, CHANGE_TYPE_NORMAL},
		{_T("ral"), gRalTables, gTgtRalTables, gRalFields, CHANGE_TYPE_NORMAL},
		{_T("adm"), gAdmTables, gTgtAdmTables, gAdmFields, CHANGE_TYPE_NORMAL},
		{_T("ref"), gRefTables, gTgtRefTables, gRefFields, CHANGE_TYPE_POINTS},
		{_T("gou"), gGouTables, gTgtGouTables, gGouFields, CHANGE_TYPE_POINTS},
		{_T("poi"), gPoiTables, gTgtPoiTables, gPoiFields, CHANGE_TYPE_POI},
		{NULL, NULL, NULL, NULL, -1}
	};

} //namespace

using namespace Reflect;

// �R���X�g���N�^
CReflect::CReflect(void)
: m_sindyCImport(false)
{
}

// �f�X�g���N�^
CReflect::~CReflect(void)
{
	g_MeshPolyMap.clear();
}

// �������֐��i���f���Ă��A�����J�n�O�ɕK���Ă΂��j
bool CReflect::init(LPCTSTR cConnectStr, const CArguments& args)
{
	//�ڑ�
	m_ipWorkspace = CReflect::connectSDE( cConnectStr );
	if( !m_ipWorkspace )
		return false;

	if( 0 != g_Suffix.CompareNoCase( args.m_suffix.c_str() ) )
	{
		//�ȈՓI�ɏ��񂩂ǂ����̔���
		//�����͏���̂�
		CString oldPath = args.m_oldPath.c_str();
		CString newPath = args.m_newPath.c_str();
		if(_taccess(oldPath, 0x04) == 0)
		{
			m_OldPath = oldPath;
		}
		else
		{
			_ftprintf(stderr, _T("#ERROR �ҏW�O�f�[�^�̃p�X�����݂��Ȃ����A�N�Z�X�ł��܂���B,%s\n"), oldPath);
			_tprintf( _T("ERROR �ҏW�O�f�[�^�̃p�X�����݂��Ȃ����A�N�Z�X�ł��܂���B,%s\n"), oldPath);
			return false;
		}
		if(_taccess(newPath, 0x04) == 0)
		{
			m_NewPath = newPath;
		}
		else
		{
			_ftprintf(stderr, _T("#ERROR �ҏW��f�[�^�̃p�X�����݂��Ȃ����A�N�Z�X�ł��܂���B,%s\n"), newPath);
			_tprintf( _T("ERROR �ҏW��f�[�^�̃p�X�����݂��Ȃ����A�N�Z�X�ł��܂���B,%s\n"), newPath);
			return false;
		}
		if( 0 == m_OldPath.CompareNoCase( m_NewPath ) )
		{
			_ftprintf(stderr, _T("#ERROR �ҏW�O�̃f�[�^�p�X�ƕҏW��f�[�^�̃p�X�������ł��B,%s,%s\n"), oldPath, newPath);
			_tprintf(_T("ERROR �ҏW�O�̃f�[�^�p�X�ƕҏW��f�[�^�̃p�X�������ł��B,%s,%s\n"), oldPath, newPath);
			return false;
		}
		if(!fnFirstOnlyInit(args))
		{
			fnUnInit();
			return false;
		}
	}
	//2��ڈȍ~�i���f����̕��A�j�̏ꍇ�͊J������
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i)
	{
		CString table = gEnvTable[g_DataIndex].m_OrgTableNames[i];
		if( !m_ipFeatureClasses[i].m_T )
		{
			if(FAILED(((IFeatureWorkspacePtr)m_ipWorkspace)->OpenFeatureClass(CComBSTR(table), &m_ipFeatureClasses[i].m_T)))
			{
				_ftprintf(stderr, _T("#ERROR �t�B�[�`���[�N���X���J���܂���B,%s\n"), table);
				_tprintf(_T("ERROR �t�B�[�`���[�N���X���J���܂���B,%s\n"), table);
				return false;
			}
		}
	}
	//�ŏ������ł�����������Ȃ����ǁA�|�����疈�ڑ����Ɏ擾
	IGeoDatasetPtr ipDataset = m_ipFeatureClasses[0].m_T;
	if(FAILED(ipDataset->get_SpatialReference(&m_ipSpRef)))
	{
		_ftprintf(stderr, _T("#ERROR,SpatialReference�擾���s�B,%s\n"), gEnvTable[g_DataIndex].m_TgtTableNames[0]);
		return false;
	}
	return true;
}

// SDE�ڑ��֐�
IWorkspacePtr CReflect::connectSDE(LPCTSTR lpszConnectStr)
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

// ���ϐ��擾�֐�
void CReflect::fnGetEnv(void)
{
	//�e�t�B�[�`���N���X��
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i){
		CString aTmp = _T("FC_") + CString(gEnvTable[g_DataIndex].m_OrgTableNames[i]);
		LPCTSTR aEnv = _tgetenv(aTmp);
		if(aEnv){
			gEnvTable[g_DataIndex].m_TgtTableNames[i] = aEnv;
		}
		_ftprintf(stderr, _T("#�ύX��ݒ�F%s\n"), gEnvTable[g_DataIndex].m_TgtTableNames[i]);
	}

	LPCTSTR aEnv = _tgetenv(_T("FC_CITYMESH"));
	if(aEnv != NULL){
		gMeshPoly = aEnv;
	}
}

// �ŏ���1��̂ݎ��s����΂悢�������֐�
bool CReflect::fnFirstOnlyInit(const CArguments& args)
{
	CString suffix = args.m_suffix.c_str();
	//�L���Ȋg���q������
	for(int i = 0; gEnvTable[i].m_Suffix != NULL; ++i)
	{
		if( 0 == suffix.CompareNoCase( gEnvTable[i].m_Suffix ) )
		{
			g_DataIndex = i;
			break;
		}
	}
	if( g_DataIndex < 0 )
	{
		CString aSuffList = gEnvTable[0].m_Suffix;
		for(int i = 1; gEnvTable[i].m_Suffix != NULL; ++i)
		{
			aSuffList += _T(",");
			aSuffList += gEnvTable[i].m_Suffix;
		}
		_tprintf(_T("#ERROR,�g���q�ɂ� %s �̂����ꂩ���w�肵�ĉ������B\n"), static_cast<LPCTSTR>(aSuffList));
		_ftprintf(stderr, _T("#ERROR,�g���q�ɂ� %s �̂����ꂩ���w�肵�ĉ������B\n"), static_cast<LPCTSTR>(aSuffList));
		return false;
	}
#ifdef _DEBUG
	g_Operator = _T("for_debug");
#else
	LPCTSTR aOperator = _tgetenv(_T("OPERATOR"));
	if(aOperator != NULL){
		g_Operator = aOperator;
	}else{
		_ftprintf(stderr, _T("#���ϐ��uOPERATOR�v���Z�b�g����Ă��܂���B\n"));
		_tprintf(_T("#���ϐ��uOPERATOR�v���Z�b�g����Ă��܂���B\n"));
		return false;
	}
#endif

	g_Suffix = suffix;
	fnGetEnv();

	//���b�V�����X�g�ǂݍ���
	if(!fnMeshLoad(args)){
		return false;
	}

	IFeatureWorkspacePtr ipFeatureWorkspace(m_ipWorkspace);
	ATLASSERT( ipFeatureWorkspace != NULL );
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i)
	{
		if(FAILED(ipFeatureWorkspace->OpenFeatureClass(CComBSTR(gEnvTable[g_DataIndex].m_TgtTableNames[i]),	&m_ipFeatureClasses[i].m_T)))
		{
			_ftprintf(stderr, _T("#ERROR �t�B�[�`���[�N���X���J���܂���B,%s\n"), gEnvTable[g_DataIndex].m_TgtTableNames[i]);
			_tprintf(_T("ERROR �t�B�[�`���[�N���X���J���܂���B,%s\n"), gEnvTable[g_DataIndex].m_TgtTableNames[i]);
			return false;
		}
		//�������[�h�łȂ����̓o�[�W�����Ή��ł��邱�Ƃ��K�v
		if(!args.m_forceMode && !args.m_testMode)
		{
			//�o�[�W�����Ή����`�F�b�N
			IVersionedObjectPtr ipVersionedObject = m_ipFeatureClasses[i].m_T;
			if(ipVersionedObject)
			{
				VARIANT_BOOL aIsVersioned = VARIANT_FALSE;
				ipVersionedObject->get_IsRegisteredAsVersioned(&aIsVersioned);
				if(!aIsVersioned)
				{
					_tprintf(_T("ERROR,�o�[�W�����Ή��ł͂Ȃ����߁A�X�V�ł��܂���\n"));
					_ftprintf(stderr, _T("#ERROR,�o�[�W�����Ή��ł͂Ȃ����߁A�X�V�ł��܂���\n"));
					return false;
				}
			}
		}

		CComBSTR aLayerName;
		((IDatasetPtr)m_ipFeatureClasses[i].m_T)->get_Name(&aLayerName);
		g_LayerName[i] = aLayerName;
		for(int j = 0; gEnvTable[g_DataIndex].m_Fields[i][j].m_fieldName != NULL; ++j)
		{
			CString fieldName = gEnvTable[g_DataIndex].m_Fields[i][j].m_fieldName;
			m_ipFeatureClasses[i]->FindField(CComBSTR(fieldName), &gEnvTable[g_DataIndex].m_Fields[i][j].m_SindyIndex);
			if(gEnvTable[g_DataIndex].m_Fields[i][j].m_SindyIndex < 0)
			{
				_ftprintf(stderr, _T("#ERROR,�t�B�[���h��������܂���B,%s\n"), fieldName);
				return false;
			}
		}
		if(!fnGetCommonFieldIndex(i))
		{
			return false;
		}
		m_ipFeatureClasses[i]->get_ShapeType(&g_ShapeType[i]);	//���b�V�����E�̔���,�}�[�W����E���Ȃ����ŕK�v�i�|���S��or���C���j
	}
	// ���^�C�v�p���ꏈ��
	if( 0 == suffix.CompareNoCase( _T("gou") ) )
	{
		m_ipFeatureClasses[0]->FindField(CComBSTR(GOUTYPE_NAME), &g_GouTypeIndex);
		if(g_GouTypeIndex < 0)
		{
			_ftprintf(stderr, _T("#ERROR,�t�B�[���h��������܂���B,%s\n"), GOUTYPE_NAME);
			return false;
		}
	}
	else
	{
		g_GouTypeIndex = -1;
	}
	g_NowProccess = 0;
	return true;
}

// ���ʂŎg�p����t�B�[���h�C���f�b�N�X�擾�֐�
bool CReflect::fnGetCommonFieldIndex(int cIndex)
{
	using namespace sindy::schema::ipc_table;
	using namespace sindy::schema::category::sindyc_table;

	// SiNDY-c�p���ǂ����m�F����̂�1�����Ɏ���Ă݂�
	// TODO:���Ȃ�������G���[�o�邪SiNDY-c�p���ǂ����m�肷��܂ŃG���[�o���Ȃ��悤�ɂ�����
	if( fnFindFieldIndex(cIndex, kOrgOperator, g_OrgOperatorIndex[cIndex]) )
	{
		// ��ꂽ�Ȃ�FeatureClass��SiNDY-c�p�̂͂��Ȃ̂ŁA�ȉ���4������Ă���
		if( !fnFindFieldIndex(cIndex, kOrgModifyDate, g_OrgModifyIndex[cIndex]) )
			return false;

		if( !fnFindFieldIndex(cIndex, kOrgOBJID, g_OrgObjIdIndex[cIndex]) )
			return false;

		if( !fnFindFieldIndex(cIndex, kUpdateC, g_UpdateCIndex[cIndex]) )
			return false;

		if( !fnFindFieldIndex(cIndex, kCheckC, g_CheckCIndex[cIndex]) )
			return false;

		if( !fnFindFieldIndex(cIndex, _T("MESHCODE"), g_MeshcodeIndex[cIndex]) )
			return false;

		m_sindyCImport = true;
	}
	else
	{
		// ����2��SiNDY-c�p��FeatureClass�ɂ͂��Ȃ�
		if( !fnFindFieldIndex(cIndex, kModifyProgName, g_ProgNameIndex[cIndex]) )
			return false;

		if( !fnFindFieldIndex(cIndex, kProgModifyDate, g_ProgModifyDateIndex[cIndex]) )
			return false;

		m_sindyCImport = false;
	}

	// �ȉ���SiNDY�p�ESiNDY-c�p�ǂ���ɂ�����X�L�[�}
	if( !fnFindFieldIndex(cIndex, kOperator, g_OperatorIndex[cIndex]) )
		return false;

	if( !fnFindFieldIndex(cIndex, kUpdateType, g_UpdateIndex[cIndex]) )
		return false;

	if( !fnFindFieldIndex(cIndex, kPurpose, g_PurposeIndex[cIndex]) )
		return false;

	if( !fnFindFieldIndex(cIndex, kUserClaim, g_ClaimIndex[cIndex]) )
		return false;
	
	if( !fnFindFieldIndex(cIndex, kModifyDate, g_ModifyDateIndex[cIndex]) )
		return false;

	if( !fnFindFieldIndex(cIndex, kSource, g_SourceIndex[cIndex]) )
		return false;

	return true;
}

/*
	�ҏW�������f��ƃ��C���֐��B���́i�ȑO�͌ʂ������j�@�\���g�ݍ��܂�Ă���B
	�E������荞��
	�E�s�v�\���_�폜
	�E���b�V�����E�}�[�W
	�E�}���`�p�[�g�|���S������
	�E�s�v�\���_�폜
*/
bool CReflect::execReflect( const CArguments& args )
{
	//�����C���|�[�g
	if(g_NowProccess == 0){
		CImport aImport;
		aImport.init(args, m_ipWorkspace, m_ipSpRef, m_ipFeatureClasses, m_sindyCImport);
		if(!aImport.executeALL(300)){
			return false;
		}
	}

	if( args.m_onlyImport )
	{
		// TODO:�K�v�Ȃ�}���`�p�[�g�|���S������
		// UPDATE_C=4�̈����Ƃ��͗v����
		//CSeparatePolygon aSeparatePolygon;
		//if(aSeparatePolygon.init(args, m_ipWorkspace, m_ipSpRef, m_ipFeatureClasses, m_sindyCImport)){
		//	//�|���S�������鎞�������s
		//	if(!aSeparatePolygon.executeALL()){
		//		return false;
		//	}
		//}
		fnUnInit(); //�S���j��
		return g_IsContinue;
	}

	//�s�v�\���_�폜+���b�V�����E�}�[�W
	if(g_NowProccess == 1){
		CMeshMerge aMeshMerge;
		if(aMeshMerge.init(args, m_ipWorkspace, m_ipSpRef, m_ipFeatureClasses, m_sindyCImport)){
			//�|���S���A�|�����C�������鎞�������s
			if(!aMeshMerge.executeALL()){
				return false;
			}
		}
	}
	
	//�}���`�p�[�g�|���S������
	if(g_NowProccess == 2){
		CSeparatePolygon aSeparatePolygon;
		if(aSeparatePolygon.init(args, m_ipWorkspace, m_ipSpRef, m_ipFeatureClasses, m_sindyCImport)){
			//�|���S�������鎞�������s
			if(!aSeparatePolygon.executeALL()){
				return false;
			}
		}
	}
	//�s�v�\���_�폜���̂Q�i���E�s���������j
	if(g_NowProccess == 3){
		CDelSinglePoint aDelSinglePoint;
		if(aDelSinglePoint.init(args, m_ipWorkspace, m_ipSpRef, m_ipFeatureClasses, m_sindyCImport)){
			//�|���S�������鎞�������s
			if(!aDelSinglePoint.executeALL()){
				return false;
			}
		}
	}
	
	fnUnInit();	//�S���j��
	return g_IsContinue;
}

// �R�}���h�t�����b�V�����X�g�̓ǂݍ��݂ƃ��b�V���|���S���擾�֐�
bool CReflect::fnMeshLoad(const CArguments& args)
{
	CString meshList = args.m_meshPath.c_str();
	if(g_MeshMap.empty())
	{
		_tprintf( _T("���b�V�����X�g��͊J�n\n") );
		FILE *pFile;
		if((pFile = _tfopen(meshList, _T("rt"))) != NULL)
		{
			//���b�V���|���S��
			IFeatureClassPtr ipMesh;
			if(FAILED(((IFeatureWorkspacePtr)m_ipWorkspace)->OpenFeatureClass(CComBSTR(gMeshPoly), &ipMesh)))
			{
				_ftprintf(stderr, _T("#ERROR �t�B�[�`���[�N���X���J���܂���B,%s\n"), static_cast<LPCTSTR>(gMeshPoly));
				_tprintf(_T("ERROR �t�B�[�`���[�N���X���J���܂���B,%s\n"), static_cast<LPCTSTR>(gMeshPoly));
				fclose(pFile);
				return false;
			}
			char aBuf[16] = "";
			while(fgets(aBuf, 16, pFile) != NULL)
			{
				if(aBuf[0] == '#')
				{
					continue;
				}
				long aMesh = atol(aBuf);
				// ���b�V�����ƂɃ��O�t�@�C���쐬 (bug 10509)
				args.m_logFile.InitLogFile( aMesh );

				COMMAND aCommand = NONE;
				char *p = strchr(aBuf, ',');
				if(p != NULL)
				{
					p++;
					switch(*p)
					{
						case 'u':
						case 'U':
							//������荞�݁i�����z�肵�Ă����ʂ�j
							aCommand = DIFF_UPDATE;
							break;
						case 'd':
						case 'D':
							//�폜�̂�
							aCommand = ONLY_DELETE;
							break;
						case 'a':
						case 'A':
							//�ǉ��̂�
							aCommand = ONLY_ADD;
							break;
						case 'n':
						case 'N':
							//�V�K�ǉ��i�폜���܂ށj
							aCommand = DELETE_ADD;
							break;
						default:
							fprintf(stderr, "#ERROR,�s���ȃR�}���h[%c],�������܂���B%d\n", *p, aMesh);
							printf("ERROR,�s���ȃR�}���h[%c],�������܂���B%d\n", *p, aMesh);
							break;
					}
				}
				else
				{
					_ftprintf(stderr, _T("#ERROR,�R�}���h���Ȃ����ߏ������܂���B,%d\n"), aMesh);
				}
				if(aCommand == NONE)
				{
					continue;
				}
				//���b�V���|���S���L�[�v
				IQueryFilterPtr ipFilter(CLSID_QueryFilter);
				CString aWhere;
				aWhere.Format(_T("MESHCODE = %d"), aMesh);
				ipFilter->put_WhereClause(CComBSTR(aWhere));
				IFeatureCursorPtr ipCursor;
				if(FAILED(ipMesh->Search(ipFilter, VARIANT_FALSE, &ipCursor)))
				{
					_ftprintf(stderr, _T("#ERROR,���b�V���|���S���̌������s�B,%s\n"), static_cast<LPCTSTR>(aWhere));
					return false;
				}
				IFeaturePtr ipFeature;
				if(ipCursor->NextFeature(&ipFeature) != S_OK)
				{
					_ftprintf(stderr, _T("#ERROR,���b�V���|���S����������Ȃ��B,%d\n"), aMesh);
					return false;		
				}
				IGeometryPtr ipGeom;
				ipFeature->get_ShapeCopy(&ipGeom);
				if(ipGeom)
				{
					g_MeshPolyMap[aMesh] = ipGeom;
					g_MeshMap[aMesh] = aCommand;
				}
				else
				{
					_ftprintf(stderr, _T("#ERROR,���b�V���|���S�����擾�ł��Ȃ��B,%d\n"), aMesh);
				}
			}
			ipMesh = NULL;
			fclose(pFile);
		}
		else
		{
			_ftprintf(stderr, _T("#ERROR,���X�g�t�@�C�����J���܂���B,%s\n"), meshList);
			_tprintf(_T("ERROR,���X�g�t�@�C�����J���܂���B,%s\n"), meshList);
			return false;
		}
	}
	return true;
}

// �㏈���֐��i�S���j������SDE����ؒf����j
void CReflect::fnUnInit(void)
{
	m_ipSpRef = nullptr;
	for( auto& fc : m_ipFeatureClasses )
	{
		// m_ipFeatureClasses.fill(nullptr); ���Ă�肽������CAdapt���邩��H�ł��Ȃ�
		fc = nullptr;
	}
	m_ipWorkspace = nullptr;
}

bool CReflect::fnFindFieldIndex(int cIndex, const CString& fieldName, long& fieldIndex)
{
	if(m_ipFeatureClasses[cIndex]->FindField(CComBSTR(fieldName), &fieldIndex) != S_OK){
		_ftprintf(stderr, _T("#ERROR,�C���f�b�N�X�������s�B[%s]\n"), fieldName);
			return false;
	}
	if(fieldIndex < 0){
		_ftprintf(stderr, _T("#ERROR,�t�B�[���h��������Ȃ��B[%s]\n"), fieldName);
			return false;
	}
	return true;
}
