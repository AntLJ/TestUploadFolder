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

// Mesh.h: CMesh クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESH_H__9AF5DC9E_FCD9_40F9_A29D_3A23102FCF10__INCLUDED_)
#define AFX_MESH_H__9AF5DC9E_FCD9_40F9_A29D_3A23102FCF10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMesh  
{
public:
	CMesh();
	virtual ~CMesh();

	void SetSpatialReference( ISpatialReferencePtr ipSpRel );
	IEnvelopePtr GetMeshExtent( UINT iMeshCode );
	void AddMesh( UINT iMeshCode );
	void AddMesh( DOUBLE dx, DOUBLE dy );
	void AddMesh( IPointPtr ipPoint );
	void AddMesh( IGeometryPtr ipGeom );
	void DeleteMesh( UINT iMeshCode );
	void DeleteMesh( DOUBLE dx, DOUBLE dy );
	void DeleteMesh( IPointPtr ipPoint );
	BOOL Exist( UINT iMeshCode );
	BOOL Exist( DOUBLE dx, DOUBLE dy );
	UINT GetMeshCode( DOUBLE dx, DOUBLE dy );
	UINT GetMeshCode( IPointPtr ipPoint );
	UINT GetExistMeshCode( IGeometryPtr ipGeom );
	_ISetPtr SplitShapeByMesh( IGeometryPtr ipGeom );
	IGeometryPtr GetIntersectPointsByShape( IGeometryPtr ipGeom );
	BOOL IsTouches( IGeometryPtr ipGeom, std::list<UINT>& listMesh );
	IPolygonPtr EnvelopeToPolygon( IEnvelopePtr ipEnv );
	IGeometryCollectionPtr GetMeshesPolygon();
	UINT INC2NDMESH_X(UINT mesh);
	UINT INC2NDMESH_Y(UINT mesh);
	UINT DEC2NDMESH_X(UINT mesh);
	UINT DEC2NDMESH_Y(UINT mesh);
private:
private:
	std::map<UINT, IEnvelopePtr> m_mapMeshList;
	ISpatialReferencePtr	m_ipSpRel;	//!< 空間参照格納用
};

#endif // !defined(AFX_MESH_H__9AF5DC9E_FCD9_40F9_A29D_3A23102FCF10__INCLUDED_)
