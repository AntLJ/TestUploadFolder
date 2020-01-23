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

#pragma once


class CCreateSJShapeObj
{
public:
	CCreateSJShapeObj(void);
	~CCreateSJShapeObj(void);
	// èâä˙âªä÷êî
	bool Init(int argc, _TCHAR **argv);
	// ÉÅÉCÉìä÷êî
	bool Run();
private:
	void ConnectServer( const CString& strConnectServer, bool bIsMesh );
	void CreateSJShape( unsigned long lMeshCode, IGeometryPtr ipTertiaryMeshGeom );
	IFeatureClassPtr CreateShapeFeatureClass( unsigned long lMeshCode, CString strFeatureClassName, ISpatialReferencePtr ipSpRef );
	std::map< long, long > GetCorrespondFieldIndex( CString strFeatureClassName, IFieldsPtr ipShapeFields, IFieldsPtr ipFields );

private:
	CString		m_strLogFileName;
	CString		m_strOutputDir;
	IFeatureClassPtr m_ipCityArea;
	IFeatureClassPtr m_ipTertiaryMesh;
	std::list< unsigned long > m_listMesh;
	std::list< CAdapt< IFeatureClassPtr > > m_listSJFeatureClass;
};
