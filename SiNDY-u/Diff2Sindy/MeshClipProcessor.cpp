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
#include "MeshClipProcessor.h"
#include <TDC/sindylib_core/ErrorCode.h>
#include <TDC/sindylib_core/meshutil.h>
#include <WinLib/VersionInfo.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheLayerFunctions.h>
#include <TDC/sindylib_base/TableContainer.h>
#include "sindy/schema.h"
#include "sindy/libschema.h"
#include "RowContainer.h"
#include "GeometryBase.h"
#include "ErrorObject2.h"
#include "Notifier.h"
#include "ClipUtility.h"

namespace sindy {
	extern bool g_bSetHistory;
}

using namespace sindy;

// �����Ώۂ̋�Ԍ����N�G���t�B���^���쐬
ISpatialFilterPtr createMeshSpatialFilter(IGeometryPtr pGeom)
{
	// �ԋp�l
	IGeometryPtr ipBoundary;
	if( (IEnvelopePtr)pGeom )
		ipBoundary = pGeom;
	else
		ITopologicalOperatorPtr( pGeom )->get_Boundary(&ipBoundary);
	return AheInitSpatialFilter( nullptr, ipBoundary );
}

template <>
MeshClipProcessor<uh::tstring>::MeshClipProcessor(const CWorkspace& workspace, const ClippingDefinitionManager& m_ClipDef)
	: Processor<uh::tstring>(),
	m_workspace(workspace),
	m_mgr(m_ClipDef)
{
	CVersion version;
	m_internalName = uh::toTStr( version.GetInternalName().GetString());		//!< �c�[������
}

// �������s
template <>
bool MeshClipProcessor<uh::tstring>::proc(const uh::tstring& src)
{
	uh::tstring mesh(src);

	// ���O
	{
		std::wstringstream ss;
		ss << _T("�N���b�s���O�J�n") <<  _T(" �Ώۃ��b�V��:") <<  mesh;
		notifier::versatile(ss.str(), std::wcout);
	}

	// �ҏW�J�n
	if( !m_workspace.StartOperation() )
		notifier::critical::versatile(_T("�ҏW�J�n�Ɏ��s"));

	// �}�[�W��`�t�@�C���ɋL�ڂ��ꂽ�e�[�u�����������[�v
	for(unsigned  i = 0 ; i < m_mgr.size(); ++i)
	{

		ClippingDefinition clippingDef = m_mgr.get(i);

		// �t�B�[�`���N���X���擾
		IFeatureClassPtr pClipTableClass = m_workspace.Workspace().OpenSameOwnersTable(clippingDef.c_str());
		if( !pClipTableClass )
			continue;

		// �R���e�i������
		CTableFinder finder;
		{
			CModel model;
			model.Add( AheGetFeatureClassName(pClipTableClass), sindy::schema::kObjectID, NULL, NULL );
			finder.InitCollection( (ITablePtr)pClipTableClass, model );
		}
		CTableContainer& con = *finder.FindTable(AheGetFeatureClassName(pClipTableClass));
		// ��Ԍ����t�@�C���^�[���擾
		IGeometryPtr meshPolygon = MESH2ENV( _ttol(mesh.c_str()), AheGetSpatialReference( pClipTableClass ) );


		if( !doClip( con, meshPolygon ) )
			return false;

		// �ۑ����{
		CErrorObjectsBase err;
		bool backup = g_bSetHistory;
		g_bSetHistory = m_mgr.isSetHistory(); // �ۑ����ɗ�����t�^���邩�ǂ���
		con.Store(err);
		g_bSetHistory = backup;
		if( sindy::errorcode::sindyErr_NoErr != err.GetDefaultErrCode() )
		{
			for( auto& e : err )
			{
				for( auto& info : *e )
				{
					const auto& errObj = *e->GetErrorObject();
					notifier::reportObjInfo( (LPCTSTR)errObj.get<0>(), errObj.get<1>(), notifier::error_level::error, uh::toTStr(info->GetErrorMessage() ), std::wcerr );
				}
			}
			return false;
		}
	}

	// �ҏW�I��
	if( !m_workspace.StopOperation() )
		notifier::critical::versatile(_T("�ҏW�I���Ɏ��s"));

	// ���O
	{
		std::wstringstream ss;
		ss << _T("�N���b�s���O�I��") <<  _T(" �Ώۃ��b�V��:") <<  mesh;
		notifier::versatile(ss.str(), std::wcout);
	}

	return true;
}

// ���[�v�����O�������\�b�h
bool MeshClipProcessor<uh::tstring>::preprocess()
{
	// �ڑ��m�F
	if( !m_workspace.Workspace() )
	{
		notifier::critical::versatile(_T("�o�͐��DB�ɐڑ����s"));
		return false;
	}

	// �ҏW�Ώۂ��o�[�W�����Ή�����Ή����ɉ����āA�ҏW���J�n����
	std::list<uh::tstring> listTarget;
	for( unsigned  i = 0 ; i < m_mgr.size(); ++i )
		listTarget.push_back( m_mgr.get(i) );
	
	return m_workspace.StartEditingByObjectType(listTarget); // �G���[���b�Z�[�W��CEditor�ŏo��
}

// �N���b�v���������{����
template <>
bool MeshClipProcessor<uh::tstring>::doClip( sindy::CTableContainer& con, IEnvelopePtr mesh )
{
	ISpatialFilterPtr pMeshFilter = createMeshSpatialFilter( mesh );
	// ���b�V���ɐڂ���t�B�[�`����S�Ď擾���A�L���b�V������
	con._Select( (IQueryFilterPtr)pMeshFilter, false );
	con.CreateCache();

	// �N���b�v�������s�𖢑R�ɖh�����߁A���b�V�����E�t�߂̍\���_��
	// ���b�V�����E�ɃX�i�b�v������
	// TODO: ��������Ȃ��ƃ��b�V�����E�t�߂̍\���_�Ɉ���������
	// ���Ƃ����邪�A��肪�����Ă��珈����ǉ����邱�Ƃɂ���B
	// ���Ȃ݂ɁA�X�i�b�v����֐����͎̂����ς�

	// �N���b�v����
	for( auto& row : con )
		Clip( con, *CAST_ROWC(row), mesh );

	return true;
}
