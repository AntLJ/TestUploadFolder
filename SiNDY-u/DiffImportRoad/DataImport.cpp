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
#include <sindy/libschema.h>
#include <WinLib/libwinlib.h>
#include <libarchelper.h>
#define _USE_SINDYLIB_STATIC
#include <libsindylibcore.h>
#include <libsindylibbase.h>
#include <coord_converter.h>
#include <WinLib/arcobjects_import_highmethod.h>
#include <sindy/workspace.h>
#include <schema.h>
#include "global.h"
#include "DataImport.h"

#include "Workspace.h"
#include "VersionInfo.h"
#include <TDC/useful_headers/str_util.h>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace sindy;
using namespace sindy::schema;

const long g_lReconcileCount = 20;		//!< ���R���T�C����

bool CImport::run()
{
	// �C���|�[�g�p�̃e�[�u��������
	setTableType();

	if(!m_cInputInfo.strExceptList.IsEmpty())
	{
		if(!readExceptFieldList())
			return false;
	}

	// ��Ɨp�q�o�[�W�������쐬
	SYSTEMTIME stSystemTime;
	::GetLocalTime( &stSystemTime);
#ifdef _DEBUG
	m_strWorkVersionName.Format(_T("DiffImportRoad_%04d%02d%02d_%02d%02d%02d_for_post_debug"), 
#else
	m_strWorkVersionName.Format(_T("DiffImportRoad_%04d%02d%02d_%02d%02d%02d_for_post"), 
#endif
		stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
		stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
	// �c�[���X�V���쐬
	m_strProgModifyDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), 
		stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
		stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);

	// ���O�t�@�C�����ݒ�
	CString strLogFileName;
	strLogFileName.Format(_T("%s\\log_%04d%02d%02d_%02d%02d%02d.txt"), 
		m_cInputInfo.strLogDir,
		stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
		stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);

	// �G���[���O�t�@�C�����ݒ�
	CString strErrLogFileName;
	strErrLogFileName.Format(_T("%s\\err_%04d%02d%02d_%02d%02d%02d.txt"), 
		m_cInputInfo.strLogDir,
		stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
		stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);

	// ���O�t�@�C���쐬
	m_ofs.open(strLogFileName);
	if( m_ofs.fail() )
	{
		writeErrorLog( ERR, strLogFileName, _T("���O�t�@�C���I�[�v�����s"), _T(""), 0 );
		return false;
	}
	m_ofs << "# FREESTYLELOG" << endl;
	m_ofs << "FLAG\tCOMMENT{�������b�V��}\tCOMMENT{�ڑ���}\tLAYER\tOBJECTID\tCOMMENT{�C���|�[�g��}\tCOMMENT{��}\t" << endl;

	// �G���[���O�t�@�C���쐬
	m_ofsErr.open(strErrLogFileName);
	if( m_ofsErr.fail() )
	{
		writeErrorLog( ERR, strErrLogFileName, _T("�G���[���O�t�@�C���I�[�v�����s"), _T(""), 0 );
		return false;
	}

	// �c�[�����A�c�[���o�[�W�����擾
	CVersion cToolVersion;
	cout << "#" << CT2A(cToolVersion.GetOriginalFilename()) << "\tFILEVERSION : " << CT2A(cToolVersion.GetFileVersion()) << "\tPRODUCTVERSION : " << CT2A(cToolVersion.GetProductVersion()) << endl;
	m_strToolName = cToolVersion.GetOriginalFilename();

	// ���b�V���P�ʂŏ������s��
	long lCount = 0, lSize = m_cInputInfo.setMesh.size();
	for( auto& mesh : m_cInputInfo.setMesh )
	{
		// ������
		m_mapData2Update.clear();
		m_mapUpdate2Data.clear();
		m_mapPGDBOldID2SDENewID.clear();
		m_vecImportLog.clear();

		{
#ifdef _DEBUG
			CFuncTraceWrapper ft( g_timetrace, _T("CImport::run") );
#endif

			lCount++;
			cout << "�� " << lCount << " / " << lSize << " �� ( " << mesh << " ) \t" << CT2A(getNowTime()) << endl;
			cout << "�@�������f�́uP�v�A�����I���́uQ�v�A�����I���́uCtrl+C�v" << endl;

			m_lMeshCode = mesh;
			CString strBeforePath, strAfterPath;
			strBeforePath.Format( _T("%s\\%ld\\%ld.mdb"), m_cInputInfo.strBeforeDir, long(mesh/100), mesh );
			strAfterPath.Format( _T("%s\\%ld\\%ld.mdb"), m_cInputInfo.strAfterDir, long(mesh/100), mesh );

			IWorkspacePtr ipCurrentSDEWork = create_workspace( m_cInputInfo.strSDEConnect );
			IWorkspacePtr ipBeforeWork = create_workspace( strBeforePath );
			IWorkspacePtr ipAfterWork = create_workspace( strAfterPath );

			if( !ipBeforeWork || !ipAfterWork || !ipCurrentSDEWork )
			{
				// ���[�N�X�y�[�X��NULL���Ă���_���ł��傤�A�A
				writeErrorLog( ERR, _T(""), _T("���[�N�X�y�[�X�擾���s"), _T(""), 0 );
				return false;
			}
			// ��Ɨp�q�o�[�W�����쐬�{�ڑ�
			IWorkspacePtr ipSDEWork;
			if( !createWorkVersion( ipCurrentSDEWork, ipSDEWork ) )
			{
				// �G���[���b�Z�[�W�͊֐����ŏo�͍ς�
				writeErrorLog( ERR, _T(""), _T("��Ɨp�q�o�[�W�����ڑ����s"), _T(""), 0 );
				return false;
			}

			// SDE�̃f�[�^�擾
			CNWData cSDEData;
			if( !cSDEData.Init_SDE( m_mapTableMap, ipSDEWork, m_exceptFieldMap ) ) // sde�p�̓t�B�[�`���N���X�A�e�[�u���I�[�v���̂�
			{
				writeErrorLog( ERR, _T(""), _T("SDE�f�[�^�擾���s"), _T(""), 0 );
				return false;
			}

			// ��ԎQ�Ɛݒ�
			m_ipSpRef = cSDEData.GetSpatialReference();

			// ���b�V���`��擾
			IGeometryPtr ipMeshGeom;
			if( !getMeshGeom( mesh, ipMeshGeom ) )
			{
				writeErrorLog( ERR, _T(""), _T("���b�V���`��擾���s"), _T(""), 0 );
				return false;
			}

			// �V��PGDB�{SDE�̃f�[�^�擾
			CNWData cBeforeData, cAfterData;
			if( !cBeforeData.Init( m_mapTableMap, ipBeforeWork, mesh, ipMeshGeom, m_exceptFieldMap ) ||
				!cAfterData.Init( m_mapTableMap, ipAfterWork, mesh, ipMeshGeom, m_exceptFieldMap ) )
			{
				writeErrorLog( ERR, _T(""), _T("�ҏW�OPGDB�A�ҏW��PGDB"), _T(""), 0 );
				continue;
			}

			// �X�V���Z�b�g
			if( !setUpdateData( cBeforeData, cAfterData ) )
			{
				writeErrorLog( ERR, _T(""), _T("�X�V���擾���s"), _T(""), 0 );
				continue;
			}

			// SDE�̃f�[�^�Ɣ�r���A�R���t���N�g���N�����Ă��Ȃ����`�F�b�N
			if( !setConflictData( cBeforeData, cAfterData, cSDEData ) )
			{
				writeErrorLog( ERR, _T(""), _T("�R���t���N�g�`�F�b�N���s"), _T(""), 0 );
				continue;
			}

			// �����ł悤�₭�X�V����SDE�ɔ��f����
			if( !reflectData( cBeforeData, cAfterData, cSDEData ) )
			{
				writeErrorLog( ERR, _T(""), _T("�X�V��񔽉f���s"), _T(""), 0 );
				continue;
			}
			// �Ō�̍Ō�Ń��O�o��
			for( const auto& importlog : m_vecImportLog )
			{
				m_ofs << CT2A(importlog) << endl;
			}
		}

		g_timetrace.Trace();
		g_timetrace.Report();
		g_timetrace.Clear();

		// �r�����f���邩
		if( IsSuspension() )
			break;
	}

	return true;
}

bool CImport::IsSuspension()
{
	// �r�����f�Ȃ�
	if( _kbhit() )
	{
		int iKey = _getch();
		if( iKey == 'Q' )
		{
			// �I��
			cout << "...�����I���I�I" << endl;
			writeErrorLog( INFO, _T(""), _T("�����I��"), _T(""), 0 );
			return true;
		}
		else if( iKey == 'P' )
		{
			//���f
			cout << "...�������f�I�I" << endl;
			writeErrorLog( INFO, _T(""), _T("�������f"), _T(""), 0 );
			while( 1 )
			{
				//�啶���́uQ�v���́uS�v�ȊO�͎󂯕t���Ȃ��iCTRL+C�͌����Ă��܂��j
				cout << "�uS�v�L�[�ōĊJ���܂��B�uQ�v�L�[�ŏI�����܂��B" << endl;
				int iContinueKey = _getch();
				if( iContinueKey == 'Q' )
				{
					cout << "...�����I��" << endl;
					writeErrorLog( INFO, _T(""), _T("�����I��"), _T(""), 0 );
					return true;
				}
				else if( iContinueKey == 'S' )
				{
					cout << "...�����ĊJ" << endl;
					writeErrorLog( INFO, _T(""), _T("�����ĊJ"), _T(""), 0 );
					return false;
				}
			}
		}
	}
	return false;
}

void CImport::setTableType()
{
	// �C���|�[�g�p�̃e�[�u�����X�g
	m_mapTableMap[road_link::kTableName] = DATATYPE::FEATURECLASS;
	m_mapTableMap[road_node::kTableName] = DATATYPE::FEATURECLASS;
	m_mapTableMap[segment_attr::kTableName] = DATATYPE::FEATURECLASS;
	m_mapTableMap[grad::kTableName] = DATATYPE::FEATURECLASS;
	m_mapTableMap[base_site::kTableName] = DATATYPE::FEATURECLASS;
	m_mapTableMap[time_oneway::kTableName] = DATATYPE::REL_LINKTABLE;
	m_mapTableMap[time_nopassage::kTableName] = DATATYPE::REL_LINKTABLE;
	m_mapTableMap[simple_grad::kTableName] = DATATYPE::REL_LINKTABLE;
	m_mapTableMap[vehicle_reg::kTableName] = DATATYPE::REL_LINKTABLE;
	m_mapTableMap[vehicle_type::kTableName] = DATATYPE::REL_OTHERTABLE;
	m_mapTableMap[inf_byway::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_dirguide::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_guide::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_internavi::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_intersection::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_lane::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_route::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_turnreg::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_uturn::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_vics::kTableName] = DATATYPE::INF;
	m_mapTableMap[lq_byway::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_dirguide::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_guide::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_internavi::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_intersection::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_lane::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_route::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_turnreg::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_uturn::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_vics::kTableName] = DATATYPE::LQ;

	// �R���t���N�g���m���������鏇�Ԃ�ݒ�i���Ԃ͏d�v�j
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( road_link::kTableName, REL_LINK ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( road_node::kTableName, REL_NODE ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( segment_attr::kTableName, REL_AGOPS ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( grad::kTableName, REL_AGOPS ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( time_oneway::kTableName, REL_AGOPS ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( time_nopassage::kTableName, REL_AGOPS ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( simple_grad::kTableName, REL_AGOPS ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( vehicle_reg::kTableName, REL_AGOPS ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( vehicle_type::kTableName, REL_VEHICLETYPE ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_byway::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_dirguide::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_guide::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_internavi::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_intersection::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_lane::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_route::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_turnreg::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_uturn::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_vics::kTableName, REL_INF ) );

	// SDE�Ƀf�[�^���f���������鏇�Ԃ�ݒ�i���Ԃ͏d�v�j
	m_vecReflectTable.push_back( road_node::kTableName );
	m_vecReflectTable.push_back( road_link::kTableName );
	m_vecReflectTable.push_back( segment_attr::kTableName );
	m_vecReflectTable.push_back( grad::kTableName );
	m_vecReflectTable.push_back( time_oneway::kTableName );
	m_vecReflectTable.push_back( time_nopassage::kTableName );
	m_vecReflectTable.push_back( simple_grad::kTableName );
	m_vecReflectTable.push_back( vehicle_reg::kTableName );
	m_vecReflectTable.push_back( vehicle_type::kTableName );
	m_vecReflectTable.push_back( inf_byway::kTableName );
	m_vecReflectTable.push_back( inf_dirguide::kTableName );
	m_vecReflectTable.push_back( inf_guide::kTableName );
	m_vecReflectTable.push_back( inf_internavi::kTableName );
	m_vecReflectTable.push_back( inf_intersection::kTableName );
	m_vecReflectTable.push_back( inf_lane::kTableName );
	m_vecReflectTable.push_back( inf_route::kTableName );
	m_vecReflectTable.push_back( inf_turnreg::kTableName );
	m_vecReflectTable.push_back( inf_uturn::kTableName );
	m_vecReflectTable.push_back( inf_vics::kTableName );
}

bool CImport::getMeshGeom( long lMeshCode, IGeometryPtr& ipMeshGeom )
{
	// �w�胁�b�V���̃��b�V���`��擾
	double dMinX = 0.0, dMinY = 0.0;
	double dMaxX = 0.0, dMaxY = 0.0;
	g_cnv.MeshtoLL( lMeshCode, 0, 0, &dMinX, &dMinY );
	g_cnv.MeshtoLL( lMeshCode, 1000000, 1000000, &dMaxX, &dMaxY );

	IEnvelopePtr ipEnv(CLSID_Envelope);
	ipEnv->PutCoords( dMinX, dMinY, dMaxX, dMaxY );
	ipEnv->putref_SpatialReference( m_ipSpRef );
	ipMeshGeom = ipEnv;

	if( !ipMeshGeom )
		return false;
	else
		return true;
}

bool CImport::setUpdateData( const CNWData& cBeforeData, const CNWData& cAfterData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setUpdateData") );
#endif

	bool bRet = false;

	// �X�V�A�폜�f�[�^�擾
	for( const auto& tablename : m_mapTableMap )
	{
		map<CString,TableData>::const_iterator itBefore = cBeforeData.m_mapTableData.find( tablename.first );
		map<CString,TableData>::const_iterator itAfter = cAfterData.m_mapTableData.find( tablename.first );
		if( cBeforeData.m_mapTableData.end() == itBefore || cAfterData.m_mapTableData.end() == itAfter )
		{
			_ASSERT(0);
			bRet = false;
			break;
		}

		bool bSetData = true;
		switch( tablename.second )
		{
		case FEATURECLASS:
		case TABLE:
		case REL_LINKTABLE:
		case REL_OTHERTABLE:
		case INF:
			bSetData = setUpdateTableData( tablename.first, itBefore->second, itAfter->second, tablename.second );
			break;
		case LQ:
		default:
			break;
		}

		if( !bSetData )
		{
			_ASSERT(0);
			return false;
		}
	}

	return true;
}

bool CImport::setUpdateTableData( const CString& strTableName, const TableData& beforeTableData, const TableData& afterTableData, DATATYPE dataType )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setUpdateTableData") );
#endif
	set<CString> exceptFields = getExceptFieldSet(strTableName);

	// �ҏW�O���ҏW��Ō���
	for( const auto& beforeRow : beforeTableData.mapRow )
	{
		long lBeforeOID = beforeRow.first; // �ҏW�O�f�[�^�̃I�u�W�F�N�gID
		map<long,CAdapt<_IRowPtr>>::const_iterator itAfterRow = afterTableData.mapRow.find( lBeforeOID );
		if( afterTableData.mapRow.end() == itAfterRow )
		{
			// �ҏW�O�ɑ��݂��A�ҏW��ɑ��݂��Ȃ����͍̂폜�f�[�^
			m_mapData2Update[strTableName][lBeforeOID].updateType = UPDATETYPE::DEL;
			m_mapData2Update[strTableName][lBeforeOID].dataStatus = DATASTATUS::UPDATE;
			m_mapUpdate2Data[strTableName][UPDATETYPE::DEL].insert( lBeforeOID );
		}
		else
		{
			bool bUpdateAttr = false;
			bool bUpdateShape = false;
			// �������X�V����Ă��邩�`�F�b�N
			if(!IsEqualAttributeBySubstance(beforeTableData.cFieldMap, afterTableData.cFieldMap, beforeRow.second, itAfterRow->second, exceptFields, false ))
			{
				bUpdateAttr = true;
			}
			// �`��X�V����Ă��邩�`�F�b�N
			switch( dataType )
			{
			case INF:
				{
					// �����N��̏ꍇ��LQ���r
					map<long,set<LqData>>::const_iterator itBeforeInf = beforeTableData.mapInfData.find( lBeforeOID );
					map<long,set<LqData>>::const_iterator itAfterInf = afterTableData.mapInfData.find( lBeforeOID );
					if( beforeTableData.mapInfData.end() == itBeforeInf || afterTableData.mapInfData.end() == itAfterInf )
					{
						_ASSERT(0);
						writeErrorLog( ERR, __func__, _T("INF�ɕR�t��LQ�f�[�^�擾���s"), strTableName, lBeforeOID );
						return false;
					}
					if( itBeforeInf->second != itAfterInf->second )
						bUpdateShape = true;
					break;
				}
			case FEATURECLASS:
				{
					// �t�B�[�`���N���X�̏ꍇ
					long lShapeIndex = beforeTableData.cFieldMap.GetShapeIndex();
					if( lShapeIndex >= 0 )
					{
						// �`���v���Ă��邩�`�F�b�N
						IGeometryPtr ipBeforeGeom;
						IGeometryPtr ipAfterGeom;
						IFeaturePtr(beforeRow.second.m_T)->get_ShapeCopy( &ipBeforeGeom );
						IFeaturePtr(itAfterRow->second.m_T)->get_ShapeCopy( &ipAfterGeom );
						if( !ipBeforeGeom || !ipAfterGeom )
						{
							_ASSERT(0);
							writeErrorLog( ERR, __func__, _T("�`��擾"), strTableName, lBeforeOID );
							return false;
						}
						if( !IsSameGeom( ipBeforeGeom, ipAfterGeom ) )
							bUpdateShape = true;
					}
				}
				break;
			default:
				break;
			}

			// �X�V���̐ݒ�
			UPDATETYPE updateType = UPDATETYPE::SAME;
			DATASTATUS dataStatus = DATASTATUS::UPDATE;
			if( bUpdateAttr && bUpdateShape )        // �����{�`��X�V
				updateType = UPDATETYPE::SHAPE_ATTR;
			else if( !bUpdateAttr && bUpdateShape )  // �`��X�V
				updateType = UPDATETYPE::SHAPE;
			else if( bUpdateAttr && !bUpdateShape )  // �����X�V
				updateType = UPDATETYPE::ATTR;
			else                                     // �ύX�Ȃ�
				dataStatus = DATASTATUS::NONE;

			m_mapData2Update[strTableName][lBeforeOID].updateType = updateType;
			m_mapData2Update[strTableName][lBeforeOID].dataStatus = dataStatus;
			m_mapUpdate2Data[strTableName][updateType].insert( lBeforeOID );
		}

		// ���b�V���O�f�[�^�������ꍇ�̐ݒ�
		if( !beforeTableData.mapInMeshData.find( lBeforeOID )->second )
			m_mapData2Update[strTableName][lBeforeOID].dataStatus = DATASTATUS(m_mapData2Update[strTableName][lBeforeOID].dataStatus | DATASTATUS::OUTMESH);
	}

	// �ҏW�と�ҏW�O�Ō���
	for( const auto& afterRow : afterTableData.mapRow )
	{
		long lAfterOID = afterRow.first; // �ҏW��f�[�^�̃I�u�W�F�N�g
		map<long,CAdapt<_IRowPtr>>::const_iterator itBeforeRow = beforeTableData.mapRow.find( lAfterOID );
		if( beforeTableData.mapRow.end() == itBeforeRow )
		{
			// �ҏW��ɑ��݂��A�ҏW�O�ɑ��݂��Ȃ����̂͐V�K�f�[�^
			m_mapData2Update[strTableName][lAfterOID].updateType = UPDATETYPE::NEW;
			m_mapData2Update[strTableName][lAfterOID].dataStatus = DATASTATUS::UPDATE;
			m_mapUpdate2Data[strTableName][UPDATETYPE::NEW].insert( lAfterOID );

			// ���b�V���O�f�[�^�������ꍇ�̐ݒ�
			if( !afterTableData.mapInMeshData.find( lAfterOID )->second )
				m_mapData2Update[strTableName][lAfterOID].dataStatus = DATASTATUS(m_mapData2Update[strTableName][lAfterOID].dataStatus | DATASTATUS::OUTMESH);
		}
	}

	return true;
}

bool CImport::IsConflict( const CFieldMap& cSDEFieldMap, const CFieldMap& cPGDBFieldMap, const _IRowPtr& ipSDERow, const _IRowPtr& ipBeforeRow, const _IRowPtr& ipAfterRow, const std::set<CString>& exceptFieldSet )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::IsConflict") );
#endif

	// OPERATOR��MODIFYDATE��SDE��PGDB�ňقȂ��SDE���̃f�[�^�͍X�V���ꂽ���ƂɂȂ�
	// MODIFYPROGNAME��PROGMODIFYDATE���R���t���N�g�����Ƃ��Ēǉ�(bug10363)

	// �t�B�[���h�C���f�b�N�X�擾
	long lSDEOperatorIndex = cSDEFieldMap.FindField( ipc_table::kOperator );
	long lSDEModifyDateIndex = cSDEFieldMap.FindField( ipc_table::kModifyDate );
	long lSDEModifyProgNameIndex = cSDEFieldMap.FindField( ipc_table::kModifyProgName );	// bug10363
	long lSDEProgModifyDateIndex = cSDEFieldMap.FindField( ipc_table::kProgModifyDate );	// bug10363

	long lPGDBOperatorIndex = cPGDBFieldMap.FindField( category::sindyc_table::kOrgOperator );
	long lPGDBModifyDateIndex = cPGDBFieldMap.FindField( category::sindyc_table::kOrgModifyDate );
	long lPGDBModifyProgNameIndex = cPGDBFieldMap.FindField( ipc_table::kModifyProgName );	// bug10363
	long lPGDBProgModifyDateIndex = cPGDBFieldMap.FindField( ipc_table::kProgModifyDate );	// bug10363

	// �����Ŏ��s���邱�Ƃ͖����Ǝv�����ǁA�A
	if( lSDEOperatorIndex < 0  || lSDEModifyDateIndex < 0  || lSDEModifyProgNameIndex < 0  || lSDEProgModifyDateIndex < 0
	 || lPGDBOperatorIndex < 0 || lPGDBModifyDateIndex < 0 || lPGDBModifyProgNameIndex < 0 || lPGDBProgModifyDateIndex < 0)
	{
		// TODO:�`�F�b�N�������Ȃ�A�ق�Ƃ� return true �ł����́H
		_ASSERT(0);
		writeErrorLog( ERR, __func__, _T("�t�B�[���h�C���f�b�N�X��NG"), _T(""), 0 );
		return true;
	}

	CComVariant vaSDEOperator , vaSDEModifyDate , vaSDEModifyProgName , vaSDEProgModifyDate;
	CComVariant vaPGDBOperator, vaPGDBModifyDate, vaPGDBModifyProgName, vaPGDBProgModifyDate;

	ipSDERow->get_Value( lSDEOperatorIndex, &vaSDEOperator );
	ipSDERow->get_Value( lSDEModifyDateIndex, &vaSDEModifyDate );
	ipSDERow->get_Value( lSDEModifyProgNameIndex, &vaSDEModifyProgName );
	ipSDERow->get_Value( lSDEProgModifyDateIndex, &vaSDEProgModifyDate );

	ipBeforeRow->get_Value( lPGDBOperatorIndex, &vaPGDBOperator );
	ipBeforeRow->get_Value( lPGDBModifyDateIndex, &vaPGDBModifyDate );
	ipBeforeRow->get_Value( lPGDBModifyProgNameIndex, &vaPGDBModifyProgName );
	ipBeforeRow->get_Value( lPGDBProgModifyDateIndex, &vaPGDBProgModifyDate );

	// ��v���Ă���΃R���t���N�g���Ă��Ȃ�
	if( vaSDEOperator == vaPGDBOperator && vaSDEModifyDate == vaPGDBModifyDate
	 && vaSDEModifyProgName == vaPGDBModifyProgName && vaSDEProgModifyDate == vaPGDBProgModifyDate )
		return false;

	// ���O�t�B�[���h����łȂ��Ȃ�Abefore�̑�����SDE�̑���������
	if(!exceptFieldSet.empty())
	{
		if(!IsEqualAttributeBySubstance(cSDEFieldMap, cPGDBFieldMap, ipSDERow, ipBeforeRow, exceptFieldSet))
			return true;

		// �`�󎝂��������ꍇ�́A�`������ׂ�
		if(!(cSDEFieldMap.GetShapeIndex()<0))
		{
			IRelationalOperatorPtr ipRelOp(((IFeaturePtr)ipSDERow)->GetShape());
			IGeometryPtr ipPGDBGeom = ((IFeaturePtr)ipBeforeRow)->GetShape();
			VARIANT_BOOL vbEqual = ipRelOp->_Equals(ipPGDBGeom);
			if(vbEqual != VARIANT_TRUE)
				return true;
		}
		return false;
	}

	// �R���t���N�g���Ă���
	return true;
}

bool CImport::IsEqualAttributeBySubstance(
	const CFieldMap& srcFieldMap,
	const CFieldMap& dstFieldMap,
	const _IRowPtr& ipSrcRow,
	const _IRowPtr& ipDstRow,
	const std::set<CString>& exceptFieldSet,
	bool  sdeCheck /*= true*/ )
{
	int fieldNum = srcFieldMap.GetFieldCount();

	for(long i=0; i<fieldNum; ++i)
	{
		// �ҏW�s�̂��́A�`��͖�������
		if(!srcFieldMap.IsEditable(i) || srcFieldMap.IsShapeField(i))
			continue;

		CString fieldName = srcFieldMap.GetName(i);

		// PGDB���m�̔�r�ł́AOPERATOR��MODIFYDATE�͕ʂ̂Ƃ���Ō��Ă��邵�A
		// SDE��PGDB�̔�r�ł́A����͌��X�C���|�[�g�ΏۊO�Ȃ̂ŏ��O����OK
		if( fieldName.CompareNoCase(ipc_table::kModifyDate) == 0
		 || fieldName.CompareNoCase(ipc_table::kOperator) == 0)
			continue;

		// PGDB���m�łȂ��ASDE��PGDB�̔�r�̏ꍇ
		if(sdeCheck)
		{
			// PGDB��SDE�ō��v���Ȃ�����(�֘A�e�[�u����ID)�A���邢�͌��X�C���|�[�g����Ȃ������͖�������
			if(fieldName.CompareNoCase(road_link::kFromNodeID) == 0
			|| fieldName.CompareNoCase(road_link::kToNodeID) == 0
			|| fieldName.CompareNoCase(link_queue::kInfID) == 0
			|| fieldName.CompareNoCase(link_queue::kLinkID) == 0
			|| fieldName.CompareNoCase(ipc_table::kPurpose) == 0
			|| fieldName.CompareNoCase(ipc_table::kProgModifyDate) == 0
			|| fieldName.CompareNoCase(ipc_table::kModifyProgName) == 0)
				continue;
		}

		// �����C���|�[�g�̏��O�t�B�[���h�͖�������
		if(exceptFieldSet.find(fieldName) != exceptFieldSet.end())
		{
			continue;
		}
		// �Ή��̎��Ȃ������͖�������
		long dstField = dstFieldMap.FindField(fieldName);
		if(dstField<0)
		{
			continue;
		}

		// ��L�̓C���|�[�g�Ɋ֌W�Ȃ������Q�B����ȊO�ō��ق�����Γ����ł͂Ȃ�
		CComVariant vaSrcVal = ipSrcRow->GetValue(i);
		CComVariant vaDstVal = ipDstRow->GetValue(dstField);
		if(vaSrcVal != vaDstVal)
		{
			return false;
		}
	}
	return true;
}

bool CImport::setConflictData( const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setConflictData") );
#endif

	for( const auto& conflictTable : m_vecConflictTable )
	{
		if( !setConflictTableData( conflictTable.second, conflictTable.first, cBeforeData, cAfterData, cSDEData ) )
		{
			return false;
		}
	}

	// 2�����b�V���m�[�h�ɕR�t���X�V�\��̃f�[�^�͍X�V�s�ɂ���
	if( !setNGDataForEdgeNode( DATASTATUS::REL_EDGENODE, cBeforeData, cAfterData, cSDEData ) )
	{
		writeErrorLog( ERR, __func__, _T("2�����b�V�����E�ɕR�t���f�[�^�̃`�F�b�N���s"), _T(""), 0 );
		return false;
	}
	
	// �m�[�hID��0��FromTo�m�[�hID���������N�ɕR�t���X�V�\��̃f�[�^�͍X�V�s�ɂ���
	if( !setNGDataForIDZeroNodeLink( DATASTATUS::REL_ZERONODE, cBeforeData, cAfterData, cSDEData ) )
	{
		writeErrorLog( ERR, __func__, _T("FromTo�m�[�hID��0�̃����N�̃`�F�b�N���s"), _T(""), 0 );
		return false;
	}

	return true;
}

bool CImport::setConflictTableData( DATASTATUS dataStatus, const CString& strTableName, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setConflictTableData") );
#endif

	set<long> setNGLink; //!< �X�V�s�ɂ��������N
	set<long> setSearchedLink; //!< �T���ς݃����N

	// �폜�f�[�^�̃R���t���N�g��ԃZ�b�g
	if( !setConflictTableData2( strTableName, UPDATETYPE::DEL, cBeforeData, cAfterData, cSDEData, setSearchedLink, setNGLink ) )
	{
		writeErrorLog( ERR, __func__, _T("�폜�f�[�^�̃R���t���N�g�`�F�b�N���s"), strTableName, 0 );
		return false;
	}
	// �֘A���铹�H�m�[�h�A�����N�������A�X�΁A�ʍs�֎~�A����ʍs�A�ȈՌX�΁A�����N�񂪍X�V�\��ꍇ�͍X�V�s�ɂ���
	if( !setUpdateNGData( dataStatus, setNGLink, cBeforeData ) )
	{
		writeErrorLog( ERR, __func__, _T("�����A�`��X�V�f�[�^�Ɋ֘A����f�[�^�̍X�V�X�e�[�^�X�ύX���s"), strTableName, 0 );
		return false;
	}

	// NG�����N�N���A
	setNGLink.clear();

	// �`��X�V�A�����X�V�A�`��{�����X�V�f�[�^�̃R���t���N�g��ԃZ�b�g
	if( !setConflictTableData2( strTableName, UPDATETYPE::SHAPE     , cBeforeData, cAfterData, cSDEData, setSearchedLink, setNGLink ) ||
		!setConflictTableData2( strTableName, UPDATETYPE::ATTR      , cBeforeData, cAfterData, cSDEData, setSearchedLink, setNGLink ) ||
		!setConflictTableData2( strTableName, UPDATETYPE::SHAPE_ATTR, cBeforeData, cAfterData, cSDEData, setSearchedLink, setNGLink ))
	{
		writeErrorLog( ERR, __func__, _T("�����A�`��X�V�f�[�^�̃R���t���N�g�`�F�b�N���s"), strTableName, 0 );
		return false;
	}
	// �֘A���铹�H�m�[�h�A�����N�������A�X�΁A�ʍs�֎~�A����ʍs�A�ȈՌX�΁A�����N�񂪍X�V�\��ꍇ�͍X�V�s�ɂ���
	if( !setUpdateNGData( dataStatus, setNGLink, cAfterData ) )
	{
		writeErrorLog( ERR, __func__, _T("�����A�`��X�V�f�[�^�Ɋ֘A����f�[�^�̍X�V�X�e�[�^�X�ύX���s"), strTableName, 0 );
		return false;
	}

	// �V�K���́��̏����ōX�V�s�ΏۂɂȂ�Ȃ����OK

	return true;
}

bool CImport::setNGDataForEdgeNode( DATASTATUS dataStatus, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setNGDataForEdgeNode") );
#endif

	set<long> setBeforeNGLink, setAfterNGLink; //!< �X�V�s�ɂ��������N
	set<long> setBeforeSearchedLink, setAfterSearchedLink; //!< �T���ς݃����N

	for( const auto& edgenode : cBeforeData.m_setMeshNode )
	{
		// ���b�V���m�[�h�ɕR�t���X�V�Ώۃ����N�͍X�V�����Ȃ�
		if( !searchNoUpdateLink( dataStatus, edgenode, cBeforeData.m_mapLink2FromToNode, cBeforeData.m_mapNode2Link, setBeforeSearchedLink, setBeforeNGLink ) )
		{
			return false;
		}
	}
	for( const auto& edgenode : cAfterData.m_setMeshNode )
	{
		// ���b�V���m�[�h�ɕR�t���X�V�Ώۃ����N�͍X�V�����Ȃ�
		if( !searchNoUpdateLink( dataStatus, edgenode, cAfterData.m_mapLink2FromToNode, cAfterData.m_mapNode2Link, setAfterSearchedLink, setAfterNGLink ) )
		{
			return false;
		}
	}

	// �֘A���铹�H�m�[�h�A�����N�������A�X�΁A�ʍs�֎~�A����ʍs�A�ȈՌX�΁A�����N�񂪍X�V�\��ꍇ�͍X�V�s�ɂ���
	if( !setUpdateNGData( dataStatus, setBeforeNGLink, cBeforeData )
	 || !setUpdateNGData( dataStatus, setAfterNGLink, cAfterData ) )
	{
		// �G���[
		return false;
	}

	return true;
}

bool CImport::setNGDataForIDZeroNodeLink( DATASTATUS dataStatus, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setNGDataForIDZeroNodeLink") );
#endif

	set<long> setBeforeNGLink, setAfterNGLink; //!< �X�V�s�ɂ��������N
	set<long> setBeforeSearchedLink, setAfterSearchedLink; //!< �T���ς݃����N

	if( cBeforeData.m_mapNode2Link.end() != cBeforeData.m_mapNode2Link.find( 0 ) )
	{
		if( !searchNoUpdateLink( dataStatus, 0, cBeforeData.m_mapLink2FromToNode, cBeforeData.m_mapNode2Link, setBeforeSearchedLink, setBeforeNGLink ) )
		{
			return false;
		}
	}
	if( cAfterData.m_mapNode2Link.end() != cAfterData.m_mapNode2Link.find( 0 ) )
	{
		if( !searchNoUpdateLink( dataStatus, 0, cAfterData.m_mapLink2FromToNode, cAfterData.m_mapNode2Link, setAfterSearchedLink, setAfterNGLink ) )
		{
			return false;
		}
	}

	// �֘A���铹�H�m�[�h�A�����N�������A�X�΁A�ʍs�֎~�A����ʍs�A�ȈՌX�΁A�����N�񂪍X�V�\��ꍇ�͍X�V�s�ɂ���
	if( !setUpdateNGData( dataStatus, setBeforeNGLink, cBeforeData )
	 || !setUpdateNGData( dataStatus, setAfterNGLink, cAfterData ) )
	{
		// �G���[
		return false;
	}

	return true;
}
bool CImport::setUpdateNGData( DATASTATUS dataStatus, const set<long>& setNGLink, const CNWData& cPGDBData )
{
	// �����N�������A�X�΁A�ʍs�֎~�A����ʍs�A�ȈՌX�΁A�����N�񂪍X�V�\��ꍇ�͍X�V�s�ɂ���
	for( const auto& tableName : m_mapTableMap )
	{
		if( 0 == _tcsicmp( tableName.first, road_node::kTableName ) )
		{
			if( !setUpdateNGNode( dataStatus, setNGLink, cPGDBData ) )
			{
				return false;
			}
		}
		else if( IsAGOPSTable( tableName.first ) )
		{
			if( !setUpdateNGAGOPS( dataStatus, tableName.first, setNGLink, cPGDBData ) )
			{
				return false;
			}
		}
		else if( IsInfTable( tableName.first ) )
		{
			if( !setUpdateNGInf( dataStatus, tableName.first, setNGLink, cPGDBData ) )
			{
				return false;
			}
		}
	}
	// �Ԏ�^�C�v��NG�f�[�^�ݒ�͍Ō��(���/�ʋ֎Ԏ�ʋK�����I����Ă���)
	if(!setUpdateNGVehicleType(dataStatus, cPGDBData))
	{
		return false;
	}

	return true;
}

bool CImport::setConflictTableData2( const CString& strTableName, UPDATETYPE updateType, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData, set<long>& setSearchedLink, set<long>& setNGLink )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setConflictTableData2") );
#endif
	const CNWData& cPGDBData = (updateType == UPDATETYPE::DEL?cBeforeData:cAfterData);

	set<CString> exceptFields = getExceptFieldSet(strTableName);

	map<CString,map<long,UpdateData>>::iterator itData2Update = m_mapData2Update.find( strTableName );
	map<CString,map<UPDATETYPE,set<long>>>::iterator itUpdate2Data = m_mapUpdate2Data.find( strTableName );
	if( m_mapData2Update.end() == itData2Update || m_mapUpdate2Data.end() == itUpdate2Data )
		return true;

	CFieldMap cPGDBDataField = cPGDBData.GetFieldMap( strTableName );
	CFieldMap cSDEDataField = cSDEData.GetFieldMap( strTableName );
	ITablePtr ipSDEDataTable = cSDEData.GetTable( strTableName );
	map<UPDATETYPE,set<long>>::const_iterator itUpdateTypeData = itUpdate2Data->second.find( updateType );

	if( itUpdate2Data->second.end() != itUpdateTypeData )
	{
		for( const auto& updata : itUpdateTypeData->second )
		{
			long lOrgID = cPGDBData.GetOrgOIDFromPGDBOID( strTableName, updata );
			if( lOrgID < 0 )
			{
				// �G���[
				_ASSERT(0);
				return false;
			}
			bool bConflict = false;
			_IRowPtr ipSDERow;
			if( S_OK == ipSDEDataTable->GetRow( lOrgID, &ipSDERow ) && ipSDERow )
			{
				// �X�V�Ώۃf�[�^��SDE���ɑ��݂���ꍇ�̓R���t���N�g�`�F�b�N�����s
				_IRowPtr ipAfterRow  = cPGDBData.GetRow( strTableName, updata ); // PGDB�ɂ͂���͂��Ȃ񂾂��ǂˁ[
				_IRowPtr ipBeforeRow = cBeforeData.GetRow( strTableName, updata );
				if( !ipAfterRow || !ipBeforeRow )
				{
					// �G���[
					_ASSERT(0);
					return false;
				}
				// �R���t���N�g���Ă��邩�`�F�b�N
				
				if( IsConflict( cSDEDataField, cPGDBDataField, ipSDERow, ipBeforeRow, ipAfterRow, exceptFields ) )
				{
					bConflict = true;
				}
			}
			else
			{
				// SDE���ɑΏۃf�[�^�������ꍇ�͍폜�ς݂Ƃ������ƂŃR���t���N�g���Ă��邱�ƂɂȂ�
				bConflict = true;
			}

			// �R���t���N�g���Ă���ꍇ�͊֘A����X�V�\��f�[�^�S�Ă��X�V�s�ɂ���
			//   -SDE�ɑ��݂��邪SDE���ōX�V����Ă���ꍇ�i�R���t���N�g�j��
			//   -SDE�ɑ��݂��Ȃ��i���ɍ폜�ς݁��R���t���N�g�j�̏ꍇ�͍X�V�s�Ƃ���
			if( bConflict )
			{
				// �R���t���N�g���Ă���ꍇ�͎������g�ɃR���t���N�g�t���O�𗧂Ă�i�X�V�s�Ƃ���j
				itData2Update->second[updata].dataStatus = DATASTATUS(itData2Update->second[updata].dataStatus | DATASTATUS::CONFLICT);

				if( 0 == _tcsicmp( strTableName, road_link::kTableName ) )
				{
					// �����N�����̏���
					// ���̃����N�ɕR�t���X�V�Ώۃ����N���X�V�����Ȃ�
					map<long,pair<long,long>>::const_iterator itPGDBLink2FromToNode = cPGDBData.m_mapLink2FromToNode.find( updata );
					if( cPGDBData.m_mapLink2FromToNode.end() != itPGDBLink2FromToNode )
					{
						setSearchedLink.insert( updata );
						if( !searchNoUpdateLink( DATASTATUS::REL_LINK, itPGDBLink2FromToNode->second.first, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
							return false;
						if( !searchNoUpdateLink( DATASTATUS::REL_LINK, itPGDBLink2FromToNode->second.second, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
							return false;
					}
					else
					{
						_ASSERT(0);
						return false;
					}
				}
				else if( 0 == _tcsicmp( strTableName, road_node::kTableName ) )
				{
					// �m�[�h�����̏���
					// ���̃m�[�h�ɕR�t���X�V�Ώۃ����N���X�V�����Ȃ�
					if( !searchNoUpdateLink( DATASTATUS::REL_NODE, updata, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
						return false;
				}
				else if( IsAGOPSTable( strTableName ) )
				{
					// [�����N�������A�X�΁A����ʍs�A�ʍs�֎~�A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK��]�����̏���
					// ����[�����N�������A�X�΁A����ʍs�A�ʍs�֎~�A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK��]�ɕR�t���X�V�Ώۃ����N���X�V�����Ȃ�
					map<CString,TableData>::const_iterator itPGDBData = cPGDBData.m_mapTableData.find( strTableName );
					if( cPGDBData.m_mapTableData.end() == itPGDBData )
					{
						_ASSERT(0);
						return false;
					}
					map<long,long>::const_iterator itAGOPS2Link = itPGDBData->second.mapAGOPS2Link.find( updata );
					if( itPGDBData->second.mapAGOPS2Link.end() == itAGOPS2Link )
					{
						_ASSERT(0);
						return false;
					}
					// �X�V�\�胊���N�Ȃ�R�t�������N���X�ɒT��
					if( IsUpdateRow( road_link::kTableName, DATASTATUS::REL_AGOPS, itAGOPS2Link->second ) )
					{
						map<long,pair<long,long>>::const_iterator itPGDBLink2FromToNode = cPGDBData.m_mapLink2FromToNode.find( itAGOPS2Link->second );
						if( cPGDBData.m_mapLink2FromToNode.end() != itPGDBLink2FromToNode )
						{
							// �R�t���������N�͒T���ς݂�
							setSearchedLink.insert( itAGOPS2Link->second );
							if( !searchNoUpdateLink( DATASTATUS::REL_AGOPS, itPGDBLink2FromToNode->second.first, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
								return false;
							if( !searchNoUpdateLink( DATASTATUS::REL_AGOPS, itPGDBLink2FromToNode->second.second, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
								return false;
						}
						else
						{
							_ASSERT(0);
							return false;
						}
					}
				}
				else if( IsVehicleTypeTable( strTableName ) )
				{
					// [�����N�������A�X�΁A����ʍs�A�ʍs�֎~�A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK��]�����̏���
					// ����[�����N�������A�X�΁A����ʍs�A�ʍs�֎~�A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK��]�ɕR�t���X�V�Ώۃ����N���X�V�����Ȃ�
					map<CString,TableData>::const_iterator itPGDBData = cPGDBData.m_mapTableData.find( strTableName );
					if( cPGDBData.m_mapTableData.end() == itPGDBData )
					{
						_ASSERT(0);
						return false;
					}
					map<long,long>::const_iterator itType2VehicleReg = itPGDBData->second.mapType2VehicleReg.find( updata );
					if( itPGDBData->second.mapType2VehicleReg.end() == itType2VehicleReg )
					{
						_ASSERT(0);
						return false;
					}
					// �R�t�����/�ʋ֎Ԏ�ʋK���̏����X�V����
					IsUpdateRow( vehicle_reg::kTableName, DATASTATUS::REL_VEHICLETYPE, itType2VehicleReg->second );

					map<CString,TableData>::const_iterator itPGDBData2 = cPGDBData.m_mapTableData.find( vehicle_reg::kTableName );
					map<long,long>::const_iterator itVehicleReg2Link = itPGDBData2->second.mapAGOPS2Link.find( itType2VehicleReg->second );
					if( itPGDBData2->second.mapAGOPS2Link.end() == itVehicleReg2Link )
					{
						_ASSERT(0);
						return false;
					}
					// �X�V�\�胊���N�Ȃ�R�t�������N���X�ɒT��
					if( IsUpdateRow( road_link::kTableName, DATASTATUS::REL_VEHICLETYPE, itVehicleReg2Link->second ) )
					{
						map<long,pair<long,long>>::const_iterator itPGDBLink2FromToNode = cPGDBData.m_mapLink2FromToNode.find( itVehicleReg2Link->second );
						if( cPGDBData.m_mapLink2FromToNode.end() != itPGDBLink2FromToNode )
						{
							// �R�t���������N�͒T���ς݂�
							setSearchedLink.insert( itVehicleReg2Link->second );
							if( !searchNoUpdateLink( DATASTATUS::REL_VEHICLETYPE, itPGDBLink2FromToNode->second.first, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
								return false;
							if( !searchNoUpdateLink( DATASTATUS::REL_VEHICLETYPE, itPGDBLink2FromToNode->second.second, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
								return false;
						}
						else
						{
							_ASSERT(0);
							return false;
						}
					}
				}
				else if( IsInfTable( strTableName ) )
				{
					// �����N������̏���
					// �����N��ɕR�t���X�V�Ώۃ����N���X�V�����Ȃ�
					map<CString,TableData>::const_iterator itPGDBData = cPGDBData.m_mapTableData.find( strTableName );
					if( cPGDBData.m_mapTableData.end() == itPGDBData )
					{
						_ASSERT(0);
						return false;
					}
					map<long,set<long>>::const_iterator itInf2Link = itPGDBData->second.mapInf2Link.find( updata );
					if( itPGDBData->second.mapInf2Link.end() == itInf2Link )
					{
						_ASSERT(0);
						return false;
					}
					for( const auto& inflink : itInf2Link->second )
					{
						// �X�V�\�胊���N�Ȃ�R�t�������N���X�ɒT��
						if( IsUpdateRow( road_link::kTableName, DATASTATUS::REL_INF, inflink ) )
						{
							map<long,pair<long,long>>::const_iterator itPGDBLink2FromToNode = cPGDBData.m_mapLink2FromToNode.find( inflink );
							if( cPGDBData.m_mapLink2FromToNode.end() != itPGDBLink2FromToNode )
							{
								// �R�t���������N�͒T���ς݂�
								setSearchedLink.insert( inflink );
								if( !searchNoUpdateLink( DATASTATUS::REL_INF, itPGDBLink2FromToNode->second.first, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
									return false;
								if( !searchNoUpdateLink( DATASTATUS::REL_INF, itPGDBLink2FromToNode->second.second, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
									return false;
							}
							else
							{
								// ���b�V�����܂����������N�񂪑��݂��邽�߃X���[
								//_ASSERT(0);
								//return false;
							}
						}
					}
				}
			}
		}
	}
	return true;
}

bool CImport::IsUpdateRow( LPCTSTR tableName, DATASTATUS dataStatus, long lPGDBLinkID )
{
	map<CString,map<long,UpdateData>>::iterator itLinkData2Update = m_mapData2Update.find( tableName );
	if( m_mapData2Update.end() == itLinkData2Update )
		return false;

	map<long,UpdateData>::iterator itLinkUpdate = itLinkData2Update->second.find( lPGDBLinkID );
	if( itLinkData2Update->second.end() == itLinkUpdate )
		return false;

	if( itLinkUpdate->second.updateType == SAME )
		return false;

	// �X�V�\�胊���N���X�e�[�^�X�ύX�i�X�V�s�j�ɂ���
	itLinkUpdate->second.dataStatus = DATASTATUS( itLinkUpdate->second.dataStatus | dataStatus );

	return true;
}

bool CImport::setUpdateNGNode( DATASTATUS dataStatus, const set<long>& setNGLink, const CNWData& cPGDBData )
{
	for( const auto& nglink : setNGLink )
	{
		map<long,pair<long,long>>::const_iterator itNGLinkNode = cPGDBData.m_mapLink2FromToNode.find( nglink );
		if( cPGDBData.m_mapLink2FromToNode.end() == itNGLinkNode )
		{
			// �����ɂ͂��Ȃ��͂��B�B
			_ASSERT(0);
			return false;
		}
		// �����N�ɕR��FromTo�m�[�h���擾
		map<long,UpdateData>::iterator itUpdateNode1 = m_mapData2Update[road_node::kTableName].find( itNGLinkNode->second.first );
		map<long,UpdateData>::iterator itUpdateNode2 = m_mapData2Update[road_node::kTableName].find( itNGLinkNode->second.second );
		if( m_mapData2Update[road_node::kTableName].end() == itUpdateNode1 || m_mapData2Update[road_node::kTableName].end() == itUpdateNode2 )
		{
			// �����ɂ͂��Ȃ��͂��B�B
			_ASSERT(0);
			return false;
		}
		if( itUpdateNode1->second.updateType != SAME )
			itUpdateNode1->second.dataStatus = DATASTATUS( itUpdateNode1->second.dataStatus | dataStatus );
		if( itUpdateNode2->second.updateType != SAME )
			itUpdateNode2->second.dataStatus = DATASTATUS( itUpdateNode2->second.dataStatus | dataStatus );
	}
	return true;
}

bool CImport::setUpdateNGAGOPS( DATASTATUS dataStatus, const CString& strTableName, const set<long>& setNGLink, const CNWData& cPGDBData )
{
	map<CString,map<long,UpdateData>>::iterator itTableUpdateAGOPS = m_mapData2Update.find(strTableName);
	if( m_mapData2Update.end() == itTableUpdateAGOPS )
	{
		// [�����N�������A�X�΁A����ʍs�A�ʍs�֎~�A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK��]��������΃X���[
		return true;
	}
	for( const auto& nglink : setNGLink )
	{
		map<CString,TableData>::const_iterator itPGDBData = cPGDBData.m_mapTableData.find( strTableName );
		if( cPGDBData.m_mapTableData.end() == itPGDBData )
		{
			// �����ɂ͂��Ȃ��͂��B�B
			_ASSERT(0);
			return false;
		}
		map<long,set<long>>::const_iterator itAGOPS = itPGDBData->second.mapLink2AGOPS.find( nglink );
		if( itPGDBData->second.mapLink2AGOPS.end() == itAGOPS )
		{
			// NG�����N�ɕR�t��[�����N�������A�X�΁A����ʍs�A�ʍs�֎~�A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK��]���Ȃ���΃X���[
			continue;
		}

		// NG���H�����N�ɕR�t���X�V�\���[�����N�������A�X�΁A����ʍs�A�ʍs�֎~�A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK��]�͍X�V�s�Ƃ���
		for( const auto& ngagops : itAGOPS->second )
		{
			map<long,UpdateData>::iterator itUpdateAGOPS = itTableUpdateAGOPS->second.find( ngagops );
			if( itTableUpdateAGOPS->second.end() == itUpdateAGOPS )
			{
				// �����ɂ͂��Ȃ��͂��B�B
				_ASSERT(0);
				return false;
			}
			if( itUpdateAGOPS->second.updateType != SAME )
				itUpdateAGOPS->second.dataStatus = DATASTATUS( itUpdateAGOPS->second.dataStatus | dataStatus );
		}
	}
	return true;
}

bool CImport::setUpdateNGVehicleType( DATASTATUS dataStatus, const CNWData& cPGDBData )
{
	map<CString,map<long,UpdateData>>::iterator itTableUpdateVehicleType = m_mapData2Update.find(vehicle_type::kTableName);
	if( m_mapData2Update.end() == itTableUpdateVehicleType )
	{
		// �Ԏ�^�C�v��������΃X���[
		return true;
	}
	map<CString,map<long,UpdateData>>::iterator itTableUpdateVehicleReg = m_mapData2Update.find(vehicle_reg::kTableName);
	if( m_mapData2Update.end() == itTableUpdateVehicleReg )
	{
		// �����ɂ͂��Ȃ��͂��B�B
		_ASSERT(0);
		return false;
	}
	set<long> ngVehicleRegSet;
	for( const auto& element : itTableUpdateVehicleReg->second )
	{
		// �Ԏ�ʋK���Ɂu�X�V����v�u�X�V�Ȃ��v�ȊO�̃r�b�g�������Ă�����NG
		if( ~(NONE|UPDATE) & element.second.dataStatus )
		{
			ngVehicleRegSet.insert(element.first);
		}
	}
	for( const auto& ngVehicleReg : ngVehicleRegSet )
	{
		map<CString,TableData>::const_iterator itPGDBData = cPGDBData.m_mapTableData.find(vehicle_type::kTableName);
		if( cPGDBData.m_mapTableData.end() == itPGDBData )
		{
			// �����ɂ͂��Ȃ��͂��B�B
			_ASSERT(0);
			return false;
		}
		map<long,set<long>>::const_iterator itVehicleType = itPGDBData->second.mapVehicleReg2Type.find( ngVehicleReg );
		if( itPGDBData->second.mapVehicleReg2Type.end() == itVehicleType )
		{
			// NG�Ԏ�ʋK���ɕR�t���Ԏ�^�C�v���Ȃ���΃X���[
			continue;
		}

		// NG���H�����N�ɕR�t���X�V�\���[�����N�������A�X�΁A����ʍs�A�ʍs�֎~�A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK��]�͍X�V�s�Ƃ���
		for( const auto& ngVehicleType : itVehicleType->second )
		{
			map<long,UpdateData>::iterator itUpdateVehicleType = itTableUpdateVehicleType->second.find( ngVehicleType );
			if( itTableUpdateVehicleType->second.end() == itUpdateVehicleType )
			{
				// �����ɂ͂��Ȃ��͂��B�B
				_ASSERT(0);
				return false;
			}
			if( itUpdateVehicleType->second.updateType != SAME )
				itUpdateVehicleType->second.dataStatus = DATASTATUS( itUpdateVehicleType->second.dataStatus | dataStatus );
		}
	}
	return true;
}

bool CImport::setUpdateNGInf( DATASTATUS dataStatus, const CString& strTableName, const set<long>& setNGLink, const CNWData& cPGDBData )
{
	map<CString,map<long,UpdateData>>::iterator itTableUpdateInf = m_mapData2Update.find(strTableName);
	if( m_mapData2Update.end() == itTableUpdateInf )
	{
		// �����N�񂪖�����΃X���[
		return true;
	}
	for( const auto& nglink : setNGLink )
	{
		map<CString,TableData>::const_iterator itPGDBData = cPGDBData.m_mapTableData.find(strTableName);
		if( cPGDBData.m_mapTableData.end() == itPGDBData )
		{
			// �����ɂ͂��Ȃ��͂��B�B
			_ASSERT(0);
			return false;
		}
		map<long,set<long>>::const_iterator itInf = itPGDBData->second.mapLink2Inf.find( nglink );
		if( itPGDBData->second.mapLink2Inf.end() == itInf )
		{
			// NG�����N�ɕR�t���Ȃ������N��̏ꍇ�̓X���[
			continue;
		}

		// NG���H�����N�ɕR�t���X�V�\��̃����N��͍X�V�s�Ƃ���
		for( const auto& nginf : itInf->second )
		{
			map<long,UpdateData>::iterator itUpdateInf = itTableUpdateInf->second.find( nginf );
			if( itTableUpdateInf->second.end() == itUpdateInf )
			{
				// �����ɂ͂��Ȃ��͂��B�B
				_ASSERT(0);
				return false;
			}
			if( itUpdateInf->second.updateType != UPDATETYPE::SAME )
				itUpdateInf->second.dataStatus = DATASTATUS( itUpdateInf->second.dataStatus | dataStatus );
		}
	}
	return true;
}


bool CImport::searchNoUpdateLink( DATASTATUS dataStatus, long lSearchNodeID, const map<long,pair<long,long>>& mapPGDBLink2FromToNode, const map<long,set<long>>& mapPGDBNode2Link, set<long>& setSearchedLink, set<long>& setNGLink )
{
	map<long,set<long>>::const_iterator itNode2Link = mapPGDBNode2Link.find( lSearchNodeID );
	if( mapPGDBNode2Link.end() == itNode2Link )
	{
		// �G���[
		_ASSERT(0);
		return false;
	}

	for( const auto& link : itNode2Link->second )
	{
		// �T���ς݃����N�̓X���[
		if( setSearchedLink.end() != setSearchedLink.find( link ) )
			continue;

		map<long,UpdateData>::iterator itUpdateLink = m_mapData2Update[road_link::kTableName].find( link );
		if( m_mapData2Update[road_link::kTableName].end() == itUpdateLink )
		{
			// �����ɂ͂��Ȃ��͂��B�B
			_ASSERT(0);
			return false;
		}
		// �T���ς݃����N�ɒǉ�
		setSearchedLink.insert( link );

		// �X�V����Ă��Ȃ������N�̓X���[
		if( UPDATETYPE::SAME == itUpdateLink->second.updateType )
			continue;

		// �X�V����Ă��郊���N�͍X�V�s�Ƃ���
		itUpdateLink->second.dataStatus = DATASTATUS( itUpdateLink->second.dataStatus | dataStatus );
		setNGLink.insert( link );
		
		// ����T��
		map<long,pair<long,long>>::const_iterator itLink2FromToNode = mapPGDBLink2FromToNode.find( link );
		if( mapPGDBLink2FromToNode.end() == itLink2FromToNode )
		{
			// �G���[
			_ASSERT(0);
			return false;
		} 
		long lNextSearchNodeID = ( itLink2FromToNode->second.first == lSearchNodeID ? itLink2FromToNode->second.second : itLink2FromToNode->second.first );
		if( !searchNoUpdateLink( dataStatus, lNextSearchNodeID, mapPGDBLink2FromToNode, mapPGDBNode2Link, setSearchedLink, setNGLink ) )
			return false;
	}

	return true;
}

bool CImport::reflectData( const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::reflect_Data") );
#endif

	// �ҏW�ۑ����邩
	bool bSave = true;

	// �ҏW�J�n
	if( !StartEdit( cSDEData.GetWorkspace() ) )
	{
		return false;
	}

	// �X�V��񊄂蓖��
	// �m�[�h�������N�������N������or�����N��̏��Łi���s���Ԃ͏d�v�I�j
	for( const auto& reflectTable : m_vecReflectTable )
	{
		if( !reflectTableData( reflectTable, cBeforeData, cAfterData, cSDEData ) )
		{
			writeErrorLog( ERR, __func__, _T("���f���s"), _T(""), 0 );
			bSave = false;
			break;
		}
	}

	// �ҏW�I��
	if( m_cInputInfo.bTestMode ) // �e�X�g���[�h�̂Ƃ��͕ҏW�j��
		bSave = false;
	if( !StopEdit( bSave, cSDEData.GetWorkspace() ) )
	{
		return false;
	}

	return true;
}

bool CImport::reflectNewData( const CString& strTableName, long lPGDBOID, const CNWData& cSDEData, const CNWData& cAfterData, const TableData& sdeTableData, const TableData& afterTableData )
{
	// �V�K�쐬
	map<long,CAdapt<_IRowPtr>>::const_iterator itAfterRow = afterTableData.mapRow.find( lPGDBOID );
	if( afterTableData.mapRow.end() == itAfterRow )
	{
		// �Ȃ��͂��͂Ȃ��B�B
		_ASSERT(0);
		return false;
	}

	_IRowPtr ipSDERow;
	if( S_OK != sdeTableData.ipTable->CreateRow( &ipSDERow ) )
	{
		// ���s���Ȃ��Ǝv�����ǁB�B
		_ASSERT(0);
		return false;
	}

	// �V�K�쐬�����f�[�^�ɕҏW��f�[�^�̏����Ԃ�����
	if( !setRowData( ipSDERow, itAfterRow->second.m_T, sdeTableData.cFieldMap, afterTableData.cFieldMap ) )
	{
		_ASSERT(0);
		return false;
	}
	// �ҏW��f�[�^�ƐV�K�쐬�f�[�^��ID�}�b�v���m��
	long lOID = 0;
	if( S_OK != ipSDERow->get_OID( &lOID ) )
	{
		_ASSERT(0);
		return false;
	}
	m_mapPGDBOldID2SDENewID[strTableName][lPGDBOID] = lOID;

	// ���H�����N�̏ꍇ
	if( 0 == _tcsicmp( strTableName, road_link::kTableName ) )
	{
		// FromTo�m�[�hID�̕t���ւ�
		map<long,pair<long,long>>::const_iterator itAfterLink2FromToNode = cAfterData.m_mapLink2FromToNode.find( lPGDBOID );
		if( cAfterData.m_mapLink2FromToNode.end() == itAfterLink2FromToNode )
		{
			// �Ȃ��͂��͂Ȃ��B�B
			_ASSERT(0);
			return false;	
		}
		// FromTo�m�[�hID�̕t���ւ�����
		if( !replaceFromToNodeID( strTableName, cAfterData, ipSDERow, itAfterLink2FromToNode->second.first, sdeTableData.cFieldMap.FindField( road_link::kFromNodeID ) ) || 
			!replaceFromToNodeID( strTableName, cAfterData, ipSDERow, itAfterLink2FromToNode->second.second, sdeTableData.cFieldMap.FindField( road_link::kToNodeID ) ) )
		{
			_ASSERT(0);
			return false;
		}
	}
	// �����N�������A�X�΁A����ʍs�A�ʍs�֎~�A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK���̏ꍇ
	else if( IsAGOPSTable( strTableName ) )
	{
		// �Ή������N�̕t���ւ�
		map<long,long>::const_iterator itAGOPS2Link = afterTableData.mapAGOPS2Link.find( lPGDBOID );
		if( afterTableData.mapAGOPS2Link.end() == itAGOPS2Link )
		{
			// �Ȃ��͂��͂Ȃ��B�B
			_ASSERT(0);
			return false;
		}
		long lOrgLinkID = -1;
		if( !getOrgIDFromPgdbID( road_link::kTableName, cAfterData, itAGOPS2Link->second, lOrgLinkID ) )
		{
			_ASSERT(0);
			return false;
		}
		if( S_OK != ipSDERow->put_Value( sdeTableData.cFieldMap.FindField( category::link_relational_table::kLinkID ), CComVariant( lOrgLinkID ) ) )
		{
			_ASSERT(0);
			return false;
		}
	}
	else if( IsVehicleTypeTable( strTableName ) )
	{
		// �Ή��Ԏ�ʋK���̕t���ւ�
		map<long,long>::const_iterator itType2VehicleReg = afterTableData.mapType2VehicleReg.find( lPGDBOID );
		if( afterTableData.mapType2VehicleReg.end() == itType2VehicleReg )
		{
			// �Ȃ��͂��͂Ȃ��B�B
			_ASSERT(0);
			return false;
		}
		long lOrgLinkID = -1;
		if( !getOrgIDFromPgdbID( vehicle_reg::kTableName, cAfterData, itType2VehicleReg->second, lOrgLinkID ) )
		{
			_ASSERT(0);
			return false;
		}
		if( S_OK != ipSDERow->put_Value( sdeTableData.cFieldMap.FindField( vehicle_type::kRegID ), CComVariant( lOrgLinkID ) ) )
		{
			_ASSERT(0);
			return false;
		}
	}
	// �����N��̏ꍇ
	else if( IsInfTable( strTableName ) )
	{
		CString strLqTableName = _T("LQ_") + strTableName.Right( strTableName.GetLength() - 4 );
		map<CString,TableData>::const_iterator itSDELqTableData = cSDEData.m_mapTableData.find( strLqTableName );
		map<long,set<LqData>>::const_iterator itAfterLqData = afterTableData.mapInfData.find( lPGDBOID );
		ITablePtr ipSDELqTable = itSDELqTableData->second.ipTable;
		if( !createLqData( lOID, ipSDELqTable, itSDELqTableData->second.cFieldMap, cAfterData, itAfterLqData->second ) )
		{
			_ASSERT(0);
			return false;
		}
	}

	// �Ō�ɃX�g�A����
	if( S_OK != ipSDERow->Store() )
	{
		_ASSERT(0);
		return false;
	}
	// ���O�o��
	m_vecImportLog.push_back( getImportLog( SDE, strTableName, lOID, true, _T("�V�K�쐬") ) );

	return  true;
}
bool CImport::replaceFromToNodeID( const CString& strTableName, const CNWData& cAfterData, _IRowPtr& ipSDERow, long lPGDBNodeID, long lNodeIndex )
{
	bool bNew = false;
	if( m_mapPGDBOldID2SDENewID.end() != m_mapPGDBOldID2SDENewID.find( road_node::kTableName ) )
	{
		if( m_mapPGDBOldID2SDENewID[road_node::kTableName].end() != m_mapPGDBOldID2SDENewID[road_node::kTableName].find( lPGDBNodeID ) )
		{
			if( S_OK != ipSDERow->put_Value( lNodeIndex, CComVariant(m_mapPGDBOldID2SDENewID[road_node::kTableName][lPGDBNodeID]) ) )
			{
				_ASSERT(0);
				return false;
			}
			bNew = true;
		}
	}
	if( !bNew )
	{
		long lOrgNodeID = cAfterData.GetOrgOIDFromPGDBOID( road_node::kTableName, lPGDBNodeID );
		if( lOrgNodeID < 0 )
		{
			_ASSERT(0);
			return false;
		}
		if( S_OK != ipSDERow->put_Value( lNodeIndex, CComVariant( lOrgNodeID ) ) )
		{
			_ASSERT(0);
			return false;
		}
	}

	return true;
}

bool CImport::reflectDeleteData( const CString& strTableName, long lPGDBOID, const CNWData& cSDEData, const CNWData& cBeforeData, const TableData& sdeTableData, const TableData& beforeTableData )
{
	// �폜
	map<long,CAdapt<_IRowPtr>>::const_iterator itBeforeRow = beforeTableData.mapRow.find( lPGDBOID );
	if( beforeTableData.mapRow.end() == itBeforeRow )
	{
		// �Ȃ��͂��͂Ȃ��B�B
		_ASSERT(0);
		return false;
	}
	long lOrgOID = cBeforeData.GetOrgOIDFromPGDBOID( strTableName, itBeforeRow->first );
	if( lOrgOID < 0 )
	{
		// �Ȃ��͂��͂Ȃ��B�B
		_ASSERT(0);
		return false;
	}
	// SDE����폜�t�B�[�`�����擾
	_IRowPtr ipSDERow;
	if( S_OK != sdeTableData.ipTable->GetRow( lOrgOID, &ipSDERow ) || !ipSDERow )
	{
		// �Ȃ��͂��͂Ȃ��B�B
		_ASSERT(0);
		return false;
	}
	// �폜���čX�V
	if( S_OK != ipSDERow->Delete() ||
		S_OK != ipSDERow->Store() )
	{
		_ASSERT(0);
		return false;
	}
	// �����N��̏ꍇ
	if( IsInfTable( strTableName ) )
	{
		// INF�ɕR�t��LQ���폜����
		CString strLqTableName = _T("LQ_") + strTableName.Right( strTableName.GetLength() - 4 );
		map<CString,TableData>::const_iterator itSDELqTableData = cSDEData.m_mapTableData.find( strLqTableName );
		ITablePtr ipSDELqTable = itSDELqTableData->second.ipTable;
		if( !deleteLqData( lOrgOID, ipSDELqTable ) )
		{
			_ASSERT(0);
			return false;
		}
	}
	// ���O�o��
	m_vecImportLog.push_back( getImportLog( SDE, strTableName, lOrgOID, true, _T("�폜") ) );
	
	return true;
}

bool CImport::reflectUpdateData(
	const CString& strTableName,
	long lPGDBOID,
	UPDATETYPE updateType,
	const CNWData& cSDEData,
	const CNWData& cAfterData,
	const TableData& sdeTableData,
	const TableData& beforeTableData,
	const TableData& afterTableData )
{
	// �����A�`��A�`��+�����X�V
	map<long,CAdapt<_IRowPtr>>::const_iterator itAfterRow = afterTableData.mapRow.find( lPGDBOID );
	if( afterTableData.mapRow.end() == itAfterRow )
	{
		// �Ȃ��͂��͂Ȃ��B�B
		_ASSERT(0);
		return false;
	}
	// �C���|�[�g���O�t�B�[���h�̕ύX�L�����m�F����
	CString exceptFields;
	if(!afterTableData.exceptFieldIndexSet.empty())
	{
		map<long,CAdapt<_IRowPtr>>::const_iterator itBeforeRow = beforeTableData.mapRow.find( lPGDBOID );
		if( beforeTableData.mapRow.end() == itBeforeRow )
		{
			// �Ȃ��͂��͂Ȃ��B�B
			_ASSERT(0);
			return false;
		}
		exceptFields = getExceptFieldString(beforeTableData, afterTableData, itBeforeRow->second, itAfterRow->second, afterTableData.exceptFieldIndexSet);
	}

	long lOrgOID = cAfterData.GetOrgOIDFromPGDBOID( strTableName, itAfterRow->first );
	if( lOrgOID < 0 )
	{
		// �Ȃ��͂��͂Ȃ��B�B
		_ASSERT(0);
		return false;
	}
	// SDE����X�V�t�B�[�`�����擾
	_IRowPtr ipSDERow;
	if( S_OK != sdeTableData.ipTable->GetRow( lOrgOID, &ipSDERow ) || !ipSDERow )
	{
		// �Ȃ��͂��͂Ȃ��B�B
		_ASSERT(0);
		return false;
	}
	long lOID = 0;
	if( S_OK != ipSDERow->get_OID( &lOID ) )
	{
		_ASSERT(0);
		return false;
	}
	// �f�[�^�X�V
	set<CString> exceptFieldSet = getExceptFieldSet(strTableName);
	if( !setRowData( ipSDERow, itAfterRow->second.m_T, sdeTableData.cFieldMap, afterTableData.cFieldMap, exceptFieldSet ) )
	{
		_ASSERT(0);
		return false;
	}
	// ���H�����N�̏ꍇ
	if( 0 == _tcsicmp( strTableName, road_link::kTableName ) )
	{
		// FromTo�m�[�hID�̕t���ւ�
		map<long,pair<long,long>>::const_iterator itAfterLink2FromToNode = cAfterData.m_mapLink2FromToNode.find( lPGDBOID );
		if( cAfterData.m_mapLink2FromToNode.end() == itAfterLink2FromToNode )
		{
			// �Ȃ��͂��͂Ȃ��B�B
			_ASSERT(0);
			return false;
		}
		// FromTo�m�[�hID�̕t���ւ�
		if( !replaceFromToNodeID( strTableName, cAfterData, ipSDERow, itAfterLink2FromToNode->second.first, sdeTableData.cFieldMap.FindField( road_link::kFromNodeID ) ) || 
			!replaceFromToNodeID( strTableName, cAfterData, ipSDERow, itAfterLink2FromToNode->second.second, sdeTableData.cFieldMap.FindField( road_link::kToNodeID ) ) )
		{
			_ASSERT(0);
			return false;
		}
	}
	// �����N�������A�X�΁A����ʍs�A�ʍs�֎~�A�ȈՌX�΁A���/�ʋ֎Ԏ�ʋK���̏ꍇ
	else if( IsAGOPSTable( strTableName ) )
	{
		// �Ή������N�̕t���ւ�
		map<long,long>::const_iterator itAGOPS2Link = afterTableData.mapAGOPS2Link.find( lPGDBOID );
		if( afterTableData.mapAGOPS2Link.end() == itAGOPS2Link )
		{
			// �Ȃ��͂��͂Ȃ��B�B
			_ASSERT(0);
			return false;
		}
		long lOrgLinkID = -1;
		if( !getOrgIDFromPgdbID( road_link::kTableName, cAfterData, itAGOPS2Link->second, lOrgLinkID ) )
		{
			_ASSERT(0);
			return false;
		}
		if( S_OK != ipSDERow->put_Value( sdeTableData.cFieldMap.FindField( category::link_relational_table::kLinkID ), CComVariant( lOrgLinkID ) ) )
		{
			_ASSERT(0);
			return false;
		}
	}
	else if( IsVehicleTypeTable( strTableName ) )
	{
		// �Ή��Ԏ�ʋK���̕t���ւ�
		map<long,long>::const_iterator itType2VehicleReg = afterTableData.mapType2VehicleReg.find( lPGDBOID );
		if( afterTableData.mapType2VehicleReg.end() == itType2VehicleReg )
		{
			// �Ȃ��͂��͂Ȃ��B�B
			_ASSERT(0);
			return false;
		}
		long lOrgVehicleRegID = -1;
		if( !getOrgIDFromPgdbID( vehicle_reg::kTableName, cAfterData, itType2VehicleReg->second, lOrgVehicleRegID ) )
		{
			_ASSERT(0);
			return false;
		}
		if( S_OK != ipSDERow->put_Value( sdeTableData.cFieldMap.FindField( vehicle_type::kRegID ), CComVariant( lOrgVehicleRegID ) ) )
		{
			_ASSERT(0);
			return false;
		}
	}
	// �����N��̏ꍇ
	else if( IsInfTable( strTableName ) )
	{
		// �`��A�`��{�����X�V�̏ꍇ�ALQ�̕t���ւ����s��
		if( updateType == UPDATETYPE::SHAPE ||
			updateType == UPDATETYPE::SHAPE_ATTR )
		{
			CString strLqTableName = _T("LQ_") + strTableName.Right( strTableName.GetLength() - 4 );
			map<CString,TableData>::const_iterator itSDELqTableData = cSDEData.m_mapTableData.find( strLqTableName );
			map<long,set<LqData>>::const_iterator itAfterLqData = afterTableData.mapInfData.find( lPGDBOID );
			ITablePtr ipSDELqTable = itSDELqTableData->second.ipTable;
			// ��ULQ���폜���Ă���V�K�쐬����
			if( !deleteLqData( lOrgOID, ipSDELqTable ) ||
				!createLqData( lOID, ipSDELqTable, itSDELqTableData->second.cFieldMap, cAfterData, itAfterLqData->second ) )
			{
				_ASSERT(0);
				return false;
			}
		}
	}

	// �Ō�ɃX�g�A����
	if( S_OK != ipSDERow->Store() )
	{
		_ASSERT(0);
		return false;
	}
	CString strStatus;
	if( updateType == UPDATETYPE::SHAPE )
		strStatus = _T("�`��X�V");
	else if( updateType == UPDATETYPE::SHAPE_ATTR )
		strStatus = _T("�`��{�����X�V");
	else
		strStatus = _T("�����X�V");

	if(!exceptFields.IsEmpty())
		strStatus.Append(exceptFields);

	// ���O�o��
	m_vecImportLog.push_back( getImportLog( SDE, strTableName, lOrgOID, true, strStatus ) );

	return true;
}

CString CImport::getExceptFieldString(
	const TableData& beforeTableData,
	const TableData& afterTableData,
	const _IRowPtr& ipBeforeRow,
	const _IRowPtr& ipAfterRow,
	const std::set<long>& exceptFieldIndexSet
	)
{
	CString result;
	// ���O�t�B�[���h
	for(long afterIndex : afterTableData.exceptFieldIndexSet)
	{
		CString fieldName = afterTableData.cFieldMap.GetName(afterIndex);
		long beforeIndex = beforeTableData.cFieldMap.FindField(fieldName);
		CComVariant vaAfterVal  = ipAfterRow->GetValue(afterIndex);
		CComVariant vaBeforeVal = ipBeforeRow->GetValue(beforeIndex);
		if(vaAfterVal != vaBeforeVal)
		{
			result.AppendFormat(_T(",%s"), fieldName);
		}
	}
	if(!result.IsEmpty())
	{
		result.TrimLeft(_T(","));
		result = _T("(���O:")+result+_T(")");
	}
	return result;
}

std::set<CString> CImport::getExceptFieldSet(LPCTSTR fieldName)
{
	const auto& exceptElement = m_exceptFieldMap.find(CString(fieldName).MakeUpper());
	if(exceptElement != m_exceptFieldMap.end())
		return exceptElement->second;

	return set<CString>();
}

bool CImport::reflectTableData( const CString& strTableName, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::reflectTableData") );
#endif

	map<CString,map<long,UpdateData>>::const_iterator itData2Update = m_mapData2Update.find( strTableName );
	map<CString,TableData>::const_iterator itBeforeTableData = cBeforeData.m_mapTableData.find( strTableName );
	map<CString,TableData>::const_iterator itAfterTableData = cAfterData.m_mapTableData.find( strTableName );
	map<CString,TableData>::const_iterator itSDETableData = cSDEData.m_mapTableData.find( strTableName );
	if( m_mapData2Update.end() != itData2Update )
	{
		for( const auto& updatedata : itData2Update->second )
		{
			if( updatedata.second.dataStatus == DATASTATUS::UPDATE ) 
			{
				// �X�V�L�f�[�^
				if( updatedata.second.updateType == UPDATETYPE::NEW )
				{
					// �V�K�쐬
					if( !reflectNewData( strTableName, updatedata.first, cSDEData, cAfterData, itSDETableData->second, itAfterTableData->second ) )
					{
						writeErrorLog( ERR, __func__, _T("�V�K�f�[�^���f���s"), strTableName, updatedata.first );
						return false;
					}
				}
				else if( updatedata.second.updateType == UPDATETYPE::DEL )
				{
					// �폜
					if( !reflectDeleteData( strTableName, updatedata.first, cSDEData, cBeforeData, itSDETableData->second, itBeforeTableData->second ) )
					{
						writeErrorLog( ERR, __func__, _T("�폜�f�[�^���f���s"), strTableName, updatedata.first );
						return false;
					}
				}
				else if( updatedata.second.updateType == UPDATETYPE::SHAPE ||
						 updatedata.second.updateType == UPDATETYPE::ATTR ||
						 updatedata.second.updateType == UPDATETYPE::SHAPE_ATTR )
				{
					// �����A�`��A�`��+�����X�V
					if( !reflectUpdateData( strTableName, updatedata.first, updatedata.second.updateType, cSDEData, cAfterData, itSDETableData->second, itBeforeTableData->second, itAfterTableData->second ) )
					{
						writeErrorLog( ERR, __func__, _T("����,�`��f�[�^���f���s"), strTableName, updatedata.first );
						return false;
					}
				} 
			}
			else if( updatedata.second.dataStatus == DATASTATUS::NONE || updatedata.second.dataStatus == DATASTATUS::OUTMESH )
			{
				// �X�V���f�[�^�A�w�胁�b�V���O�����̂��̂̓X���[
			}
			else
			{
				CString strNGComment;
				// �X�V�L�f�[�^���������ǍX�V�����Ȃ�����
				if( updatedata.second.dataStatus & DATASTATUS::CONFLICT )
					strNGComment += _T("�R���t���N�g,");
				if( updatedata.second.dataStatus & DATASTATUS::REL_LINK )
					strNGComment += _T("�����N,");
				if( updatedata.second.dataStatus & DATASTATUS::REL_NODE )
					strNGComment += _T("�m�[�h,");
				if( updatedata.second.dataStatus & DATASTATUS::REL_AGOPS )
					strNGComment += _T("�����N������,");
				if( updatedata.second.dataStatus & DATASTATUS::REL_INF )
					strNGComment += _T("�����N��,");
				if( updatedata.second.dataStatus & DATASTATUS::REL_EDGENODE )
					strNGComment += _T("2�����b�V�����E,");
				if( updatedata.second.dataStatus & DATASTATUS::REL_ZERONODE )
					strNGComment += _T("�����NFromToID��0,");
				if( updatedata.second.dataStatus & DATASTATUS::REL_VEHICLETYPE )
					strNGComment += _T("�Ԏ�^�C�v,");
				if( updatedata.second.dataStatus & DATASTATUS::OUTMESH )
					strNGComment += _T("�w�胁�b�V���O,");
				strNGComment.TrimRight( _T(",") );

				DBTYPE dbType;
				if( updatedata.second.updateType == UPDATETYPE::DEL )
					dbType = BEFORE;
				else
					dbType = AFTER;

				m_vecImportLog.push_back( getImportLog( dbType, strTableName, updatedata.first, false, strNGComment ) );
			}
		}
	}
	return true;
}

bool CImport::createWorkVersion( IWorkspacePtr& ipWorkspace, IWorkspacePtr& ipSDEWork )
{
	IVersionPtr ipCurrentVersion( ipWorkspace );
	if( !ipCurrentVersion )
	{
		_ASSERT(0);
		return false;
	}
	// ����o�[�W���������������ꍇ�͂���𗘗p����
	IVersionPtr ipChildVersion;
	((IVersionedWorkspacePtr)ipCurrentVersion)->FindVersion( CComBSTR(m_strWorkVersionName), &ipChildVersion );
	if( !ipChildVersion )
	{
		HRESULT hr = ipCurrentVersion->CreateVersion( CComBSTR(m_strWorkVersionName), &ipChildVersion );
		if( ipChildVersion == NULL )
		{
			_ASSERT(0);
			return false;
		}
	}
	CString strWorkVersionName = sindy::workspace_address((IWorkspacePtr)ipChildVersion).c_str();

	ipSDEWork = create_workspace( strWorkVersionName );
	if( !ipSDEWork )
	{
		_ASSERT(0);
		return false;
	}

	return true;
}

bool CImport::setRowData( _IRowPtr& ipSDERow, const _IRowPtr& ipPGDBRow, const CFieldMap& cSDEField, const CFieldMap& cPGDBField, const std::set<CString>& exceptFieldSet /*= std::set<CString>()*/)
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setRowData") );
#endif
	for( long i = 0; i < cSDEField.GetFieldCount(); ++i )
	{
		if( !cSDEField.IsEditable( i ) )
			continue;

		CComVariant vaPGDBValue;
		CString strFieldName = cSDEField.GetName( i );
		// MODIFYDATE�̂Ƃ��͍X�V���Ȃ�(bug10009)
		if(strFieldName.CompareNoCase(ipc_table::kModifyDate) == 0)
			continue;
		
		// �C���|�[�g���O�t�B�[���h�͖���(bug10363)
		else if( exceptFieldSet.find(strFieldName) != exceptFieldSet.end() )
		{
			continue;
		}

		// OPERATOR�̂Ƃ��͈����Ŏw�肳��Ă���l������
		else if(strFieldName.CompareNoCase(ipc_table::kOperator) == 0)
		{
			if(m_cInputInfo.strOperator.IsEmpty())
				continue;
			vaPGDBValue = m_cInputInfo.strOperator;
		}

		// PURPOSE_C�̂Ƃ��͈����Ŏw�肳��Ă���l������
		else if(strFieldName.CompareNoCase(ipc_table::kPurpose) == 0)
		{
			if(m_cInputInfo.lPurpose == -1)
				continue;
			vaPGDBValue = m_cInputInfo.lPurpose;
		}
		// TODO:SDE�ɐV�����t�B�[���h���ǉ����ꂽ�肵���ۂɁA�Ή��̎��Ȃ��t�B�[���h���o�Ă��邽�߁A�x��������ʂɏo�͂����
		// �ŏ��ɁASDE��PGDB�̃t�B�[���h�̐��������m�F���A���ق̂���t�B�[���h���i�[���A���̃t�B�[���h�͏����悤�ɏC������ׂ�
		else if( S_OK != ipPGDBRow->get_Value( cPGDBField.FindField( cSDEField.GetName( i ) ), &vaPGDBValue ) )
		{
			_ASSERT(0);
			return false;
		}

		if( S_OK != ipSDERow->put_Value( i, vaPGDBValue ) )
		{
			_ASSERT(0);
			return false;
		}


		if( cSDEField.IsShapeField( i ) )
		{
			IGeometryPtr ipGeom;
			if( S_OK != IFeaturePtr(ipPGDBRow)->get_ShapeCopy( &ipGeom ) || !ipGeom )
			{
				_ASSERT(0);
				return false;
			}
			if( S_OK != IFeaturePtr(ipSDERow)->putref_Shape( ipGeom ) )
			{
				_ASSERT(0);
				return false;
			}
		}
	}
	// �c�[�����{�c�[���X�V����ǋL
	if( S_OK != ipSDERow->put_Value( cSDEField.FindField( ipc_table::kProgModifyDate ), CComVariant( m_strProgModifyDate ) ) ||
		S_OK != ipSDERow->put_Value( cSDEField.FindField( ipc_table::kModifyProgName ), CComVariant( m_strToolName ) ) )
	{
		_ASSERT(0);
		return false;
	}

	return true;
}

bool CImport::getOrgIDFromPgdbID( LPCTSTR tableName, const CNWData& cNWData, long lPgdbID, long& lOrgID )
{
	lOrgID = -1; // �ꉞ������

	// �ΏۃI�u�W�F�N�g���V�K�쐬�̏ꍇ�͕t���ւ�
	if( m_mapPGDBOldID2SDENewID.end() != m_mapPGDBOldID2SDENewID.find( tableName ) )
	{
		if( m_mapPGDBOldID2SDENewID[tableName].end() != m_mapPGDBOldID2SDENewID[tableName].find( lPgdbID ) )
		{
			lOrgID = m_mapPGDBOldID2SDENewID[tableName][lPgdbID];
			return true;
		}
	}
	// �V�K�I�u�W�F�N�g�łȂ��ꍇ�͊����I�u�W�F�N�g�ŕR�t
	lOrgID = cNWData.GetOrgOIDFromPGDBOID( tableName, lPgdbID );
	if( lOrgID < 0 )
	{
		_ASSERT(0);
		return false;
	}
	return true;
}

bool CImport::createLqData( long lSDEInfID, ITablePtr& ipSDELqTable, const CFieldMap& cSDELqField, const CNWData& cAfterData, const set<LqData>& setAfterLqData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::create_LqData") );
#endif

	// �ҏW��PGDB��LQ�f�[�^�Ń��[�v
	for( const auto& afterlq : setAfterLqData )
	{
		long lLinkID = -1;

		// �����NID�̕t���ւ�
		if( !getOrgIDFromPgdbID( road_link::kTableName, cAfterData, afterlq.lLinkID, lLinkID ) )
		{
			_ASSERT(0);
			return false;
		}

		_IRowPtr ipLqRow;
		if( S_OK != ipSDELqTable->CreateRow( &ipLqRow ) || !ipLqRow )
		{
			_ASSERT(0);
			return false;
		}
		if( S_OK != ipLqRow->put_Value( cSDELqField.FindField( link_queue::kMeshCode ), CComVariant(afterlq.lMeshCode) ) || 
			S_OK != ipLqRow->put_Value( cSDELqField.FindField( link_queue::kLinkID ), CComVariant(lLinkID) ) ||
			S_OK != ipLqRow->put_Value( cSDELqField.FindField( link_queue::kLinkDir ), CComVariant(afterlq.lLinkDir) ) ||
			S_OK != ipLqRow->put_Value( cSDELqField.FindField( link_queue::kSequence ), CComVariant(afterlq.lSeq) ) ||
			S_OK != ipLqRow->put_Value( cSDELqField.FindField( link_queue::kLastLink ), CComVariant(afterlq.lLast) ) ||
			S_OK != ipLqRow->put_Value( cSDELqField.FindField( link_queue::kInfID ), CComVariant(lSDEInfID) ) ||
			S_OK != ipLqRow->put_Value( cSDELqField.FindField( ipc_table::kProgModifyDate ), CComVariant(m_strProgModifyDate) ) ||
			S_OK != ipLqRow->put_Value( cSDELqField.FindField( ipc_table::kModifyProgName ), CComVariant(m_strToolName) ) )
		{
			_ASSERT(0);
			return false;
		}
		if( S_OK != ipLqRow->Store() )
		{
			_ASSERT(0);
			return false;
		}
	}

	return true;
}

bool CImport::deleteLqData( long lSDEInfID, ITablePtr& ipSDELqTable )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::delete_LqData") );
#endif

	// INF��ID�ɕR�t��LQ���擾
	CString strWhere;
	strWhere.Format( _T("%s=%ld"), link_queue::kInfID, lSDEInfID );
	IQueryFilterPtr ipQFil(CLSID_QueryFilter);
	ipQFil->put_WhereClause( CComBSTR(strWhere) );
	_ICursorPtr ipCursor;
	if( S_OK == ipSDELqTable->Search( ipQFil, VARIANT_FALSE, &ipCursor ) && ipCursor )
	{
		_IRowPtr ipRow;
		while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow )
		{
			// �q�b�g�������̂��폜
			if( S_OK != ipRow->Delete() ||
				S_OK != ipRow->Store() )
			{
				_ASSERT(0);
				return false;
			}
		}
	}
	else
	{
		_ASSERT(0);
		return false;
	}

	return true;
}

CString CImport::getImportLog( DBTYPE dbType, const CString& strTableName, long lOID, bool bOK, const CString& strStatus )
{
	CString strConnect;
	switch( dbType )
	{
		case BEFORE:
			strConnect = _T("�ҏW�OPGDB");
			break;
		case AFTER:
			strConnect = _T("�ҏW��PGDB");
			break;
		case SDE:
			strConnect = _T("SDE");
			break;
		default:
			break;
	}
	CString strRet, strOKNG;
	strOKNG = bOK ? _T("OK") : _T("NG");
	strRet.Format( _T("0\t%ld\t%s\t%s\t%ld\t%s\t%s\t"), m_lMeshCode, strConnect, strTableName, lOID, strOKNG, strStatus );

	return strRet;
}

void CImport::writeErrorLog( ERRTYPE errType, const CString& strFuncName, const CString& strComment, const CString& strTableName, long lOID, bool bOutTime/*=false*/)
{
	CString strErrType;
	switch( errType )
	{
	case 0:
		strErrType = _T("INFO");
		break;
	case 1:
		strErrType = _T("NOTICE");
		break;
	case 2:
		strErrType = _T("WARNING");
		break;
	case 3:
		strErrType = _T("ERROR");
		break;
	case 4:
		strErrType = _T("CRITICAL");
		break;
	default:
		break;
	}
	
	CString strTableOID;
	if( !strTableName.IsEmpty() )
	{
		if( lOID > 0 )
			strTableOID.Format( _T("%s:%ld"), strTableName, lOID );
		else
			strTableOID = strTableName;
	}

	CString strErrMessage;
	strErrMessage.Format( _T("#%s\tMESHCODE�F%ld\t%s\t%s\t%s\t"), strErrType, m_lMeshCode, strFuncName, strComment, strTableOID );
	
	if(bOutTime)
	{
		SYSTEMTIME aTime;
		GetLocalTime(&aTime);
		strErrMessage += uh::str_util::ToString(&aTime);
	}
	if( m_ofsErr.fail() )
		cerr << CT2A(strErrMessage) << endl;
	else
		m_ofsErr << CT2A(strErrMessage) << endl;
}

bool CImport::StartEdit( IWorkspacePtr& ipWorkspace )
{
	IWorkspaceEditPtr ipWorkspaceEdit(ipWorkspace);
	if( !ipWorkspaceEdit )
		return false;
	
	if( S_OK != ipWorkspaceEdit->StartEditing( VARIANT_TRUE ) )
	{
		writeErrorLog( ERR, _T(""), _T("StartEditing()���s"), _T(""), 0 );
		return false;
	}

	if( S_OK != ipWorkspaceEdit->StartEditOperation() )
	{
		writeErrorLog( ERR, _T(""), _T("StartEditOperation()���s"), _T(""), 0 );
		return false;
	}

	return true;
}

bool CImport::StopEdit( bool bSave, IWorkspacePtr& ipWorkspace )
{
	IWorkspaceEditPtr ipWorkspaceEdit( ipWorkspace );
	if( !ipWorkspaceEdit )
		return false;
	
	if( bSave )
	{
		if( S_OK != ipWorkspaceEdit->StopEditOperation() )
		{
			writeErrorLog( ERR, _T(""), _T("StopEditOperation()���s"), _T(""), 0 );
			if( S_OK == ipWorkspaceEdit->AbortEditOperation() )
				writeErrorLog( INFO, _T(""), _T("AbortEditOperation()����"), _T(""), 0 );
			else
				writeErrorLog( ERR,_T(""), _T("AbortEditOperation()���s"), _T(""), 0 );
			if( S_OK == ipWorkspaceEdit->StopEditing( VARIANT_FALSE ) )
				writeErrorLog( INFO, _T(""), _T("StopEditing()����"), _T(""), 0 );
			else
				writeErrorLog( ERR, _T(""), _T("StopEditing()���s"), _T(""), 0 );
			return false;
		}
	}
	else
	{
		if( S_OK == ipWorkspaceEdit->AbortEditOperation() )
			writeErrorLog( INFO, _T(""), _T("AbortEditOperation()����"), _T(""), 0 );
		else
			writeErrorLog( ERR, _T(""), _T("AbortEditOperation()���s"), _T(""), 0 );
	}

	if( bSave )
	{
		bool bReconcile = true;
		int lCount = 0;
		// �ҏW�I������
		while( !DoReconcile( ipWorkspaceEdit ) )
		{
			lCount++;
			if( lCount > g_lReconcileCount )
			{
				CString strReconcile;
				strReconcile.Format( _T("���R���T�C��%d�񎸔s"), g_lReconcileCount );
				writeErrorLog( ERR, __func__, strReconcile, _T(""), 0 );
				bReconcile = false;
				break;
			}
		}
		return bReconcile;
	}
	else
	{
		if( S_OK != ipWorkspaceEdit->StopEditing( VARIANT_FALSE ) )
		{
			writeErrorLog( ERR, _T(""), _T("StopEditing()���s"), _T(""), 0, true );
			return false;
		}
		else
			writeErrorLog( INFO, _T(""), _T("StopEditing()����"), _T(""), 0, true ); 
	}

	return true;
}

bool CImport::DoReconcile( IWorkspaceEditPtr& ipWorkspaceEdit )
{
	// StopEditing()�Ɏ��s�����烊�R���T�C��
	HRESULT hr = ipWorkspaceEdit->StopEditing( VARIANT_TRUE );
	bool bRet = false;
	switch( hr )
	{
		case FDO_E_VERSION_REDEFINED:
		{
			std::cout << "�����R���T�C����...\n";
			writeErrorLog( INFO, _T(""), _T("���R���T�C�����{"), _T(""), 0 );
			IVersionEditPtr ipVersionEdit( ipWorkspaceEdit );
			if( ipVersionEdit )
			{
				IVersionInfoPtr ipVersionInfo, ipParentVersionInfo;
				IVersionPtr ipVersion( ipVersionEdit );
				CComBSTR bstrVersionName, bstrParentVersionName;
				if( ipVersion )
				{
					if( SUCCEEDED( ipVersion->get_VersionInfo( &ipVersionInfo ) ) )
						ipVersionInfo->get_VersionName( &bstrVersionName );
				}
				if( !CString(bstrVersionName).IsEmpty() )
				{
					VARIANT_BOOL vbConflicts = VARIANT_FALSE;
					HRESULT hrReconcile = hr = ipVersionEdit->Reconcile( bstrParentVersionName, &vbConflicts );
					switch( hrReconcile )
					{
						case FDO_E_RECONCILE_VERSION_NOT_AVAILABLE:
							writeErrorLog( ERR, _T(""), _T("���R���T�C�����s�FFDO_E_RECONCILE_VERSION_NOT_AVAILABLE"), _T(""), 0 );
							break;
						case FDO_E_VERSION_BEING_EDITED:
							writeErrorLog( ERR, _T(""), _T("���R���T�C�����s�FFDO_E_VERSION_BEING_EDITED"), _T(""), 0 );
							break;
						case FDO_E_VERSION_NOT_FOUND:
							writeErrorLog( ERR, _T(""), _T("���R���T�C�����s�FFDO_E_VERSION_NOT_FOUND"), _T(""), 0 );
							break;
						case S_OK:
							// �R���t���N�g�����ꍇ�̓G���[�Ƃ����A���̂܂ܔ�����
							// �R���t���N�g���̏����͌Ăяo����ɔC����
							if( vbConflicts )
								writeErrorLog( WARNING, _T(""), _T("���R���T�C�������F�R���t���N�g����"), _T(""), 0 );
							else {
								// �w���v�ɂ́AReconcile���I��������StopEditing�������I�ɌĂ΂��悤�Ȃ��Ƃ�
								// �����Ă������̂ŁA�������Ȃ��Ă������̂��ȁH�H
								writeErrorLog( INFO, _T(""), _T("���R���T�C�������F�ҏW�I��"), _T(""), 0 );
							}
							bRet = true;
						break;
						default:
							writeErrorLog( ERR, _T(""), _T("���R���T�C�����Ɍ����s���̃G���[������"), _T(""), 0 );
							break;
					}
				}
				else
					writeErrorLog( ERR, _T(""), _T("IVersionEdit����o�[�W�������̎擾���s"), _T(""), 0 );
			}
			else
				writeErrorLog( ERR, _T(""), _T("IVersionEdit�̎擾���s"), _T(""), 0 );
		}
			break;
		case S_OK:
			writeErrorLog( INFO, _T(""), _T("�ҏW�ۑ�����"), _T(""), 0, true );
			bRet = true;
			break;
		default:
			writeErrorLog( ERR, _T(""), _T("�ҏW�ۑ����s"), _T(""), 0, true );
			break;
	}
	return bRet;
}

bool CImport::readExceptFieldList()
{
	using namespace uh::str_util;

	// TSV�t�@�C���̓ǂݍ���
	ifstream ifs(CT2A(m_cInputInfo.strExceptList));
	if(!ifs)
	{
		CString message;
		cerr << "���O�t�B�[���h�t�@�C���̓ǂݍ��݂Ɏ��s���܂���" << endl;
		cerr << "ERROR\texcept_fieldlist : " << CT2A(m_cInputInfo.strExceptList) << " �����������m�F���Ă�������" << endl;
		return false;
	}

	bool succeed_f = true;
	string line;
	int lineNum = 0;
	while(getline(ifs, line))
	{
		CString curLine(CA2T(line.c_str()));
		curLine = curLine.MakeUpper();

		std::vector<CString> cols = split(curLine, _T("\t"));
		if(cols.size() != 2)
		{
			cerr << "ERROR\texcept_fieldlist : ��̐����s���ł�\t" << line << endl;
			succeed_f = false;
			continue;
		}
		CString layerName = cols[0];
		if(m_exceptFieldMap.find(layerName) != m_exceptFieldMap.end())
		{
			cerr << "ERROR\texcept_fieldlist : ���C���̎w�肪�d�����Ă��܂�\t" << CT2A(layerName) << endl;
			succeed_f = false;
			continue;
		}
		std::vector<CString> cols2 = split(cols[1], _T(","));
		for(const auto& col : cols2)
		{
			if(!checkExceptField(col))
			{
				cerr << "ERROR\texcept_fieldlist : �C���|�[�g���O�t�B�[���h�ɂ͐ݒ�ł��Ȃ��l�ł�\t"
				     << CT2A(layerName) << "\t" << CT2A(col) << endl;
				succeed_f = false;
				continue;
			}
			m_exceptFieldMap[cols[0]].insert(col);
		}
	}
	return succeed_f;
}

bool CImport::checkExceptField(const CString& fieldName)
{
	using namespace sindy::schema;

	return ((fieldName.CompareNoCase(_T("OBJECTID")) != 0 )
	     && (fieldName.CompareNoCase(_T("SHAPE")) != 0 )
	     && (fieldName.CompareNoCase(ipc_table::kPurpose) != 0 )
	     && (fieldName.CompareNoCase(ipc_table::kProgModifyDate) != 0 )
	     && (fieldName.CompareNoCase(ipc_table::kModifyProgName) != 0 )
		 && (fieldName.CompareNoCase(ipc_table::kModifyDate) != 0 )
		 && (fieldName.CompareNoCase(ipc_table::kOperator) != 0 )
	     && (fieldName.Right(3).CompareNoCase(_T("_ID")) != 0 )
	     && (fieldName.Right(3).CompareNoCase(_T("_RF")) != 0 )
	     && (fieldName.Right(5).CompareNoCase(_T("_LQRF")) != 0 )
		 );
}
