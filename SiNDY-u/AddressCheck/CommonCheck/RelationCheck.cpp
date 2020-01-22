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
#include "..\GlobalFunc.h"
#include "RelationCheck.h"



// �����`�F�b�N
bool RelationCheck::checkOutArea( const IGeometryPtr& ipMeshGeometry, IGeometryCollectionPtr& ipGeometryCollection, const ISpatialReferencePtr& ipSpatialReference, const ErrorIdInfo&errorInfo, LayerCheckBase* layerCheck, const ErrorIdInfo& errorInfoNoPolygonInMesh, const CString& meshName, long meshId )const
{
	// �d���ȃG���[���b�Z�[�W���o�͂��Ȃ����߂̔��f
	if( layerCheck->m_errorIdSet.find( errorInfo ) != layerCheck->m_errorIdSet.end() )	return true;

	// ���b�V����ADDR_POLYGON/CITY_POLYGON���}�[�W����
	IPolygonPtr ipMergeGeometry = gf::constructUnion( ipGeometryCollection, ipSpatialReference );
	if( !ipMergeGeometry)
	{
		printProgress(getMsg( eFailConstructUnion ));
		return false;
	}
	if( gf::isTouches(ipMergeGeometry, ipMeshGeometry) && ( layerCheck->m_errorIdSet.find(errorInfoNoPolygonInMesh) == layerCheck->m_errorIdSet.end() ) )
	{
		layerCheck->m_errorLog->write( emCheck_ErrCode(errorInfoNoPolygonInMesh.errorCode), meshName, meshId, NULL, _T("%d"), errorInfoNoPolygonInMesh.oId );
		return true;
	}
	
	// Diff�����|���S���擾���� 
	IGeometryPtr ipDifferentGeometry = gf::difference( ipMeshGeometry, ipMergeGeometry );
	if( !ipDifferentGeometry )
	{
		printProgress( getMsg( eFailDifference ));
		return false;
	}

	// �����G���A�����݂���ꍇ�A�o��
	long geometryCount(0L);
	IGeometryCollectionPtr(ipDifferentGeometry)->get_GeometryCount(&geometryCount);
	for( long index = 0L; index < geometryCount;  ++index )
	{
		IGeometryPtr path;
		IGeometryCollectionPtr(ipDifferentGeometry)->get_Geometry( index, &path );
		if( !path ) continue;

		// �`�F�b�N�Ώۂ̓s�s�n�}���b�V���ɂ�����Z���|���S����S�ă}�[�W���A�s�s�n�}���b�V���}�s��Clip�����`�󂪓s�s�n�}���b�V���}�s�ƈقȂ�ꍇ�G���[
		layerCheck->m_errorLog->write( emCheck_ErrCode(errorInfo.errorCode), layerCheck->getName(), INVALID_VALUE, path, _T("%d"), errorInfo.oId );
	}

	return true;
}