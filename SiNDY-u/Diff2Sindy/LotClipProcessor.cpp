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
#include "LotClipProcessor.h"
#include <TDC/sindylib_core/meshutil.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include "ClipUtility.h"

using namespace sindy;

template<>
LotClipProcessor<uh::tstring>::LotClipProcessor(const CWorkspace& workspace, const ClippingDefinitionManager& m_ClipDef, const std::vector<uh::tstring>& meshes)
	: MeshClipProcessor<uh::tstring>( workspace, m_ClipDef ), m_vecMesh(meshes)
{
}

// ���b�g�|���S��������������
template<>
void LotClipProcessor<uh::tstring>::initLotPolygon( ISpatialReferencePtr spref )
{
	IGeometryCollectionPtr bag( CLSID_GeometryBag );
	((IGeometryPtr)bag)->putref_SpatialReference(spref);
	for( const auto& mesh : m_vecMesh )
		bag->AddGeometry( MESH2POLY( _ttol(mesh.c_str()), spref ) );

	ITopologicalOperatorPtr topo( CLSID_Polygon );
	((IGeometryPtr)topo)->putref_SpatialReference(spref);
	topo->ConstructUnion((IEnumGeometryPtr)bag);
	m_ipLotPolygon = topo;
}

/// �N���b�v���������{����
template<>
bool LotClipProcessor<uh::tstring>::doClip( CTableContainer& con, IEnvelopePtr mesh )
{
	if( !m_ipLotPolygon )
		initLotPolygon( AheGetSpatialReference((ITable*)con) );

	ISpatialReferencePtr spref;
	m_ipLotPolygon->get_SpatialReference(&spref);

	// ���b�g���E�ƃ��b�V���̋��L����ӂ��擾
	IGeometryPtr lotBoundary;
	ITopologicalOperatorPtr(m_ipLotPolygon)->get_Boundary( &lotBoundary );
	IGeometryPtr sharedLine;
	// TODO:�ڐG���肪���܂������Ȃ����Ƃ����蓾��̂ŁA���b�V����`�̃o�b�t�@�Ŕ��肷��悤�C��
	ITopologicalOperatorPtr(lotBoundary)->Intersect(mesh, esriGeometry1Dimension, &sharedLine);
	// �W�I���g������̏ꍇ�͔�΂�
	if( IsEmpty(sharedLine) )
		return true;

	// TODO:�����R������蓾��̂ŁA�|�C���g�⃉�C���Ō��������A
	//      �o�b�t�@�����O�����|���S���Ō�������悤�C��
	ISpatialFilterPtr pMeshFilter = AheInitSpatialFilter( nullptr, sharedLine );
	// ���L�ӂɐڂ���t�B�[�`����S�Ď擾���A�L���b�V������
	con._Select( (IQueryFilterPtr)pMeshFilter, false );
	con.CreateCache();

	// �N���b�v�������s�𖢑R�ɖh�����߁A���b�V�����E�t�߂̍\���_��
	// ���b�V�����E�ɃX�i�b�v������
	// TODO: ��������Ȃ��ƃ��b�V�����E�t�߂̍\���_�Ɉ���������
	// ���Ƃ����邪�A��肪�����Ă��珈����ǉ����邱�Ƃɂ���B
	// ���Ȃ݂ɁA�X�i�b�v����֐����͎̂����ς�

	// �N���b�v����
	for( auto& row : con )
		Clip( con, *CAST_ROWC(row), m_ipLotPolygon );

	return true;
}
