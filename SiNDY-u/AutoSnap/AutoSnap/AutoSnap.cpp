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
#include "AutoSnap.h"

#include <algorithm>
#include <cctype>
#include <atl2/exception.h>
#include <sindy/workspace.h>

#include "resource.h"

//Constant string 
const wchar_t strNULL[]            = L"NULL";

using namespace std;

bool CAutoSnap::GetMiniLength(const wstring& str, double& value)
{
	wstring strTmp(str.size(), 0);
	transform(str.begin(), str.end(), strTmp.begin(), towupper);
	if(m_cfg.mini_list.find(strTmp) == m_cfg.mini_list.end())
	{
		wcerr << L"Layer:" << str << L" mini length information does not exist!" << endl;
		return false;
	}
	else
	{
		value = m_cfg.mini_list[strTmp];
		return true;
	}
}

bool CAutoSnap::ReadMeshList()
{
	wcout << L"Caching mesh list" << endl;
	CFileTable ft;
	if(!ft.Open(m_cfg.mesh_list))
	{
		wcerr << L"Open mesh list file:"<< m_cfg.mesh_list << L" failed!" << endl;
		return false;
	}
	CFileTable::Records rds;
	//Read all records
	if(!ft.Read(rds))
	{
		wcerr << L"Read mesh list file:"<< m_cfg.mesh_list << L" failed!" << endl;
		return false;
	}
	if(rds.empty())
	{
		wcerr << L"Mesh list file is empty:"<< m_cfg.mesh_list << L" failed!" << endl;
		return false;
	}

	if(m_cfg.type == AutoSnapConfig::T_MULTI_LOG)
	{
		for(const auto& rd : rds)
		{
			MeshInfo mi;
			mi.str = rd[0];
			m_meshList.push_back(mi);
		}
		return true;
	}

	IFeatureWorkspacePtr ipWorkspace;
	IFeatureClassPtr ipClass;

	//Not multi files, so open workspace once.
	if(m_cfg.type == AutoSnapConfig::T_NORMAL)
	{
		ipWorkspace = sindy::create_workspace(m_cfg.mesh.c_str());
		if(!ipWorkspace)
		{
			wcerr << L"Open mesh feature workspace error:" << m_cfg.mesh << endl;
			return false;
		}

		if(ipWorkspace->OpenFeatureClass(CComBSTR(m_cfg.mesh_name.c_str()), &ipClass) != S_OK || !ipClass)
		{
			wcerr << L"Open mesh feature class error:"<< m_cfg.mesh_name << L" failed!" << endl;
			return false;
		}
	}
	//Get all needed mesh objects
	for(const auto& rd : rds)
	{
		if(rd.empty())
			continue;

		if(m_cfg.type == AutoSnapConfig::T_MULTI_FGDB || m_cfg.type == AutoSnapConfig::T_MULTI_LOG)
		{
			std::wstring filename = GetFilenameByMesh(rd[0]);
			if(filename.empty())
			{
				wcerr << L"Invalid mesh code:" << rd[0] << endl;
				return false;
			}

			ipWorkspace = sindy::create_workspace(filename.c_str());
			if(!ipWorkspace)
			{
				wcerr << L"Open mesh feature workspace error:" << filename << endl;
				return false;
			}

			//Open mesh class	
			if(ipWorkspace->OpenFeatureClass(CComBSTR(m_cfg.mesh_name.c_str()), &ipClass) != S_OK || !ipClass)
			{
				wcerr << L"Open mesh feature class error:"<< m_cfg.mesh_name << L" failed!" << endl;
				return false;
			}
		}

		MeshInfo mi;
		mi.ipGeo = GetMeshObject(rd[9], ipClass);
		if(!mi.ipGeo)
		{
			wcerr << L"Get mesh shape object failed" << endl;
			return false;
		}
		mi.str = rd[0];
		m_meshList.push_back(mi);
		wcout << mi.str << endl;
	}

	wcout << L"Caching finished!" << endl;
	return true;
}

//Read snap layer information
bool CAutoSnap::ReadSnapLayerFile(const IFeatureWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatures)
{
	//Read all snap layer information
	CFileTable ft;
	if(!ft.Open(m_cfg.snap_layer))
	{
		wcerr << L"Open snap layer file:"<< m_cfg.snap_layer << L" failed!" << endl;
		return false;
	}
	CFileTable::Records rds;
	//Read all records
	if(!ft.Read(rds))
	{
		wcerr << L"Read snap layer file:"<< m_cfg.snap_layer << L" failed!" << endl;
		return false;
	}
	//Count of records >= 3 
	if(rds.size() < 3)
	{
		wcerr << L"Snap layer file:"<< m_cfg.snap_layer << L" line error!" << endl;
		return false;
	}

	//対象	CITY_RAILWAY	NULL
	m_strSnapName.resize(rds[1][1].size());
	transform(rds[1][1].begin(), rds[1][1].end(), m_strSnapName.begin(), towupper);
	//Open snap 
	atl2::valid(ipWorkspace->OpenFeatureClass(CComBSTR(m_strSnapName.c_str()), &ipFeatures));

	if(!ipFeatures)
	{
		wcerr << L"Open feature class:"<< m_strSnapName << L" failed!" << endl;
		return false;
	}
	if(ipFeatures->GetShapeType() != esriGeometryType::esriGeometryPolyline){
		wcerr << L"Snap feature class is not polyline:" << ipFeatures->GetShapeType() << endl;
		return false;
	}

	m_layers.clear();

	//種類	レイヤ名	WHERE句	共有編集対象1	共有編集対象2	共有編集対象3
	for(size_t i = 2; i < rds.size(); ++i)
	{
		CFileTable::Record& rd = rds[i];
		if(rd.size() > 2)
		{
			double value(0);
			if(!GetMiniLength(rd[1].c_str(), value))
				return false;

			CAutoSnap::SnapLayerInfo sli;
			//Open feature
			atl2::valid(ipWorkspace->OpenFeatureClass(CComBSTR(rd[1].c_str()), &sli.feature));
			
			if(!sli.feature)
			{
				wcerr << L"Open feature class:"<< rd[1] << L" failed!" << endl;
				return false;
			}

			//Get filter
			if(rd[2] != strNULL)
			{
				sli.filter = rd[2];
			}
			sli.length = value;
			//Get all extended features
			for(size_t i = 3; i < rd.size(); ++i)
			{
				if(!GetMiniLength(rd[i].c_str(), value))
					return false;
				CAutoSnap::LayerInfo li;
				atl2::valid(ipWorkspace->OpenFeatureClass(CComBSTR(rd[i].c_str()), &li.feature));
				//li.name = rd[i];
				li.length = value;
				sli.ext.push_back(li);
			}
			m_layers.push_back(sli);
		}
		else
		{
			wcerr << L"Snap layer record error: needed field is missing."<< endl;
			return false;
		}
	}

	return true;
}

//Open snap records file which is outputted by ShapeChecker tool
bool CAutoSnap::OpenSnapRecordsFile(const wstring& strName, CFileTable& ft)
{
	//Open snap records file
	if(!ft.Open(strName))
	{
		wcerr << L"Open snap file:"<< strName << L" failed!" << endl;
		return false;
	}
	//Skip the header
	CFileTable::Record rd;
	if(!ft.Read(rd))
	{
		wcerr << L"Header does not exist in layer file:" << m_cfg.snap_layer << L" !" << endl;
		return false;
	}

	return true;
}

bool CAutoSnap::InitLogFile(const wstring& strname)
{
	//Open log file
	if(!m_ftLog.Open(strname, false))
	{
		wcerr << L"Open log file:"<< strname << L" failed!" << endl;
		return false;
	}
	
	if(!m_ftLog.Write(L"# FREESTYLELOG")) return false;

	CFileTable::Record rd;
	rd.push_back(L"NAME");
	rd.push_back(L"OID");
	rd.push_back(L"LON");
	rd.push_back(L"LAT");
	rd.push_back(L"ERRMSG");
	return m_ftLog.Write(rd);
}

bool CAutoSnap::Log(const wchar_t* OID, const IPointPtr& ipPoint, const wchar_t* errMsg)
{
	CStringW str;
	str.Format(L"%s\t%s\t%.15lf\t%.15lf\t%s", m_strSnapName.c_str(), OID, ipPoint->GetX(), ipPoint->GetY(), errMsg);
	return m_ftLog.Write(str);
}

bool CAutoSnap::Log(const wchar_t* OID, const wchar_t* strLon, const wchar_t*  strLat, const wchar_t* errMsg)
{
	CStringW str;
	str.Format(L"%s\t%s\t%s\t%s\t%s", m_strSnapName.c_str(), OID, strLon, strLat, errMsg);
	return m_ftLog.Write(str);
}

bool CAutoSnap::Log(const wchar_t* OID, const IPointPtr& ipPoint, unsigned int nMsg)
{
	CStringW strMsg;
	strMsg.LoadStringW(nMsg);
	return Log(OID, ipPoint, strMsg);
}

bool CAutoSnap::Log(const wchar_t* OID, const wchar_t* strLon, const wchar_t*  strLat, unsigned int nMsg)
{
	CStringW strMsg;
	strMsg.LoadStringW(nMsg);
	return Log(OID, strLon, strLat, strMsg);
}

//Initialize environment
bool CAutoSnap::init()
{
	if(!ReadMeshList()) return false;

	if(m_cfg.type == AutoSnapConfig::T_MULTI_FGDB || m_cfg.type == AutoSnapConfig::T_NORMAL)
	{
		if(!InitLogFile(m_cfg.log)) return false;
	}
	//Get radius
	if(m_cfg.radius < 0)
	{
		wcerr << L"Radius must >= 0 ("<< m_cfg.radius << L")!" << endl;
		return false;
	}

	return true;
}

bool CAutoSnap::ExecuteSingle()
{
	//Open workspace
	IWorkspaceEditPtr ipEdit = sindy::create_workspace(m_cfg.lot.c_str());
	if(!ipEdit)
	{
		wcerr << L"Open workspace:" << m_cfg.lot << L" failed!"<<endl;
		return false;
	}

	if(ipEdit->StartEditing(VARIANT_FALSE) != S_OK)
	{
		wcerr << L"Edit workspace error" << endl;
		return false;
	}

	IFeatureClassPtr ipFeatures;
	//Open the snap feature class
	//Process Snap Layer information
	if(!ReadSnapLayerFile(ipEdit, ipFeatures))
	{
		if(ipEdit->StopEditing(VARIANT_FALSE) != S_OK)
		{
			wcerr << L"Stop workspace error" << endl;
		}
		return false;
	}

	//Maybe the log file is large, so process records one by one
	CFileTable ft;
	if(!OpenSnapRecordsFile(m_cfg.snap, ft))
	{
		return false;
	}
	CFileTable::Record rd;
	while(ft.Read(rd))
	{
		wstring strTmp(rd[1].size(), 0);
		transform(rd[1].begin(), rd[1].end(), strTmp.begin(), towupper);
		if(strTmp != m_strSnapName)
		{
			continue;
		}
		//Not in mesh list
		IPointPtr ipPoint(CLSID_Point);
		ipPoint->PutRefSpatialReference(m_meshList[0].ipGeo->GetSpatialReference());

		ipPoint->PutX(stod(rd[6]));
		ipPoint->PutY(stod(rd[7]));
		size_t nMesh;
		if(!IsInMesh(ipPoint, nMesh))
		{
			wcerr << L"Out of mesh list" << endl;
			Log(rd[2].c_str(), rd[6].c_str(), rd[7].c_str(), IDS_ERR_OUT_MESH);
			return true;
		}
		//Main function
		if(!Process(ipEdit, ipFeatures, rd[2], rd[6], rd[7], m_meshList[nMesh].ipGeo))
		{
			if(ipEdit->StopEditing(VARIANT_FALSE) != S_OK)
			{
				wcerr << L"Stop workspace error" << endl;
			}
			return false;
		}
	}

	if(!SaveEdit(ipEdit))
	{
		return false;
	}

	return true;
}

bool CAutoSnap::SaveEdit(const IWorkspaceEditPtr& ipEdit)
{
	if(ipEdit)
	{
		VARIANT_BOOL vb;
		if(ipEdit->HasEdits(&vb) == S_OK && vb)
		{
			if(ipEdit->StopEditing(VARIANT_TRUE) != S_OK)
			{
				wcerr << L"Save workspace error" << endl;
				return false;
			}
		}
		else
		{
			if(ipEdit->StopEditing(VARIANT_FALSE) != S_OK)
			{
				wcerr << L"Stop workspace error" << endl;
				return false;
			}
		}
	}

	return true;
}

//
// This function is not been used
//
bool CAutoSnap::ExecuteMultiFile()
{	
	//Optimization
	std::wstring strLast;	//Last FGDB file name
	IWorkspaceEditPtr ipEdit;
	IFeatureClassPtr ipFeatures;

	//Maybe the log file is large, so process records one by one
	CFileTable ft;
	if(!OpenSnapRecordsFile(m_cfg.snap, ft))
	{
		return false;
	}

	CFileTable::Record rd;
	while(ft.Read(rd))
	{
		//Checking whether there are valid Lon/Lat values.
		if(std::isalpha(rd[6][0]) || std::isalpha(rd[7][0]))
		{
			continue;
		}

		//Find the mesh via Lon/Lat from log file
		IPointPtr ipPoint(CLSID_Point);
		ipPoint->PutRefSpatialReference(m_meshList[0].ipGeo->GetSpatialReference());
		ipPoint->PutX(stod(rd[6]));
		ipPoint->PutY(stod(rd[7]));
		size_t nMesh;
		if(!IsInMesh(ipPoint, nMesh))
		{
			wcerr << L"Out of mesh list" << endl;
			Log(rd[2].c_str(), rd[6].c_str(), rd[7].c_str(), IDS_ERR_OUT_MESH);
			return true;
		}
		
		//Get mesh filename
		std::wstring filename = GetFilenameByMesh(m_meshList[nMesh].str);

		if(filename != strLast)
		{
			strLast = filename;

			if(!SaveEdit(ipEdit))
			{
				return false;
			}

			//Open workspace
			ipEdit = sindy::create_workspace(filename.c_str());
			if(!ipEdit)
			{
				wcerr << L"Open workspace:" << m_cfg.lot << L" failed!"<<endl;
				return false;
			}

			if(ipEdit->StartEditing(VARIANT_FALSE) != S_OK)
			{
				wcerr << L"Edit workspace error" << endl;
				return false;
			}

			//Open the snap feature class
			//Process Snap Layer information
			if(!ReadSnapLayerFile(ipEdit, ipFeatures))
			{
				if(ipEdit->StopEditing(VARIANT_FALSE) != S_OK)
				{
					wcerr << L"Stop workspace error" << endl;
				}
				return false;
			}
		}
		wstring strTmp(rd[1].size(), 0);
		transform(rd[1].begin(), rd[1].end(), strTmp.begin(), towupper);
		if(strTmp != m_strSnapName)
		{
			continue;
		}

		//Main function
		if(!Process(ipEdit, ipFeatures, rd[2], rd[6], rd[7], m_meshList[nMesh].ipGeo))
		{
			if(ipEdit->StopEditing(VARIANT_FALSE) != S_OK)
			{
				wcerr << L"Stop workspace error" << endl;
			}
			return false;
		}
	}

	//For last modification
	if(!SaveEdit(ipEdit))
	{
		return false;
	}

	return true;
}

bool CAutoSnap::ExecuteMultiLog()
{
	//Loop all mesh
	for(const auto& mesh : m_meshList)
	{
		wcout << mesh.str << endl;
		m_ftLog.Close();
		wstring logName = m_cfg.log +  GetLogNameByMesh(mesh.str);
		if(!InitLogFile(logName))
		{
			wcerr << "Init log file error " << logName << endl;
			return false;
		}
		//Read all log contents
		logName = m_cfg.snap + GetSnapLogNameByMesh(mesh.str);
		CFileTable ft;
		if(!OpenSnapRecordsFile(logName, ft))
		{
			wcerr << "Log of the mesh does not exist " << logName << endl;
			continue;
		}
		CFileTable::Records rds;
		//
		ft.Read(rds);
		//There isn't a valid record
		if(rds.empty())
		{
			continue;
		}

		std::wstring strFilename = GetFilenameByMesh(mesh.str);
		IWorkspaceEditPtr ipEdit = sindy::create_workspace(strFilename.c_str());
		if(!ipEdit)
		{
			wcerr << L"Open workspace:" << m_cfg.lot << L" failed!"<<endl;
			return false;
		}

		if(ipEdit->StartEditing(VARIANT_FALSE) != S_OK)
		{
			wcerr << L"Edit workspace error" << endl;
			return false;
		}

		//Get Mesh object
		IFeatureClassPtr ipMesh;
		if(IFeatureWorkspacePtr(ipEdit)->OpenFeatureClass(CComBSTR(m_cfg.mesh_name.c_str()), &ipMesh) != S_OK || !ipMesh)
		{
			wcerr << L"Open mesh feature class error:"<< m_cfg.mesh_name << L" failed!" << endl;
			return false;
		}
		IGeometryPtr ipMeshGeo = GetMeshObject(mesh.str, ipMesh);
		if(!ipMeshGeo)
		{
			wcerr << L"Get mesh shape date error:"<< m_cfg.mesh_name << L" failed!" << endl;
			return false;
		}

		//Open the snap feature class
		//Process Snap Layer information
		IFeatureClassPtr ipFeatures;
		if(!ReadSnapLayerFile(ipEdit, ipFeatures))
		{
			if(ipEdit->StopEditing(VARIANT_FALSE) != S_OK)
			{
				wcerr << L"Stop workspace error" << endl;
			}
			return false;
		}

		//Loop all records		
		for(const auto& rd : rds)
		{
			wstring strTmp(rd[1].size(), 0);
			transform(rd[1].begin(), rd[1].end(), strTmp.begin(), towupper);
			if(strTmp != m_strSnapName)
			{
				continue;
			}
			//Not in mesh list
			IPointPtr ipPoint(CLSID_Point);
			ipPoint->PutRefSpatialReference(ipMeshGeo->GetSpatialReference());

			ipPoint->PutX(stod(rd[6]));
			ipPoint->PutY(stod(rd[7]));

			if(!IsInMesh(ipPoint, ipMeshGeo))
			{
				wcerr << L"Out of mesh list" << endl;
				Log(rd[2].c_str(), rd[6].c_str(), rd[7].c_str(), IDS_ERR_OUT_MESH);
				continue;
			}
			//Main function
			if(!Process(ipEdit, ipFeatures, rd[2], rd[6], rd[7], ipMeshGeo))
			{
				if(ipEdit->StopEditing(VARIANT_FALSE) != S_OK)
				{
					wcerr << L"Stop workspace error" << endl;
				}
				return false;
			}
		}

		//For last modification
		if(!SaveEdit(ipEdit))
		{
			return false;
		}
	}

	return true;
}

//Run
bool CAutoSnap::execute()
{
	// Initialize
	if( !init() )
		return false;

	switch(m_cfg.type)
	{
	case AutoSnapConfig::T_NORMAL:
		return ExecuteSingle();
	case AutoSnapConfig::T_MULTI_FGDB:
		return ExecuteMultiFile();
	case AutoSnapConfig::T_MULTI_LOG:
		return ExecuteMultiLog();
	default:
		wcerr << L"Unknown type " << m_cfg.type << endl;
		return false;
	}
	
}

IFeaturePtr CAutoSnap::GetFeature(const IFeatureClassPtr& ipFeatures, const wstring& oid)
{
	//Field name of ObjectID
	CStringW strName = ipFeatures->GetOIDFieldName().GetBSTR();
	//Sql text
	CStringW strSql;

	strSql.Format(L"%s = %s", strName, oid.c_str());
	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	if(!ipQuery)
	{
		wcerr << L"Create QueryFilter failed!" << endl;
		return nullptr;
	}
	atl2::valid(ipQuery->put_WhereClause(CComBSTR(strSql)));

	IFeatureCursorPtr ipCursor;
	if(ipFeatures->Search(ipQuery, VARIANT_FALSE, &ipCursor) != S_OK)
	{
		wcerr << L"Search failed with "<< strSql << endl;
		return nullptr;
	}

	//There can only be one result for using the Object ID as the filter.
	IFeaturePtr ipFeature;
	if(ipCursor->NextFeature(&ipFeature) != S_OK)
	{
		wcerr << L"Snap object "<< oid << " does not exist!" << endl;
		return nullptr;
	}

	if(!ipFeature)
	{
		wcerr << L"NextFeature failed! " << endl;
		return nullptr;
	}
	
	return ipFeature;
}

IEnvelopePtr CAutoSnap::CreateEnvelope(const IPointPtr& ipPoint, double raduis)
{
	if(!ipPoint) return nullptr;

	IEnvelopePtr ipEnvelope(CLSID_Envelope);
	if(!ipEnvelope) return nullptr;

	ipEnvelope->PutRefSpatialReference(ipPoint->GetSpatialReference());
	ipEnvelope->PutXMin(ipPoint->GetX() - raduis);
	ipEnvelope->PutXMax(ipPoint->GetX() + raduis);
	ipEnvelope->PutYMin(ipPoint->GetY() - raduis);
	ipEnvelope->PutYMax(ipPoint->GetY() + raduis);

	return ipEnvelope;
}

//Point in mesh
bool CAutoSnap::IsInMesh(const IPointPtr& ipPoint, size_t& nIndex)
{
	IRelationalOperatorPtr ipRel(ipPoint);
	if(!ipRel) return false;
	for(size_t i = 0; i < m_meshList.size(); ++i)
	{
		if(ipRel->_Disjoint(m_meshList[i].ipGeo) == VARIANT_FALSE)
		{
			nIndex = i;
			return true;
		}
	}
	return false;
}

bool CAutoSnap::IsInMesh(const IPointPtr& ipPoint, const IGeometryPtr& ipGeo)
{
	IRelationalOperatorPtr ipRel(ipPoint);
	
	if(ipRel->_Disjoint(ipGeo) == VARIANT_FALSE)
	{
		return true;
	}
	return false;
}

//Point on the mesh border
bool CAutoSnap::IsOnMeshBorder(const IPointPtr& ipPoint, const IGeometryPtr& ipMesh)
{
	IRelationalOperatorPtr ipRel(ipPoint);
	if(ipRel && ipMesh)
	{
		return (ipRel->_Touches(ipMesh) == VARIANT_TRUE);
	}
	return false;
}

//Maybe there is no this case in the data when the value of range is small enough.
bool CAutoSnap::IsTooShort(const IPointCollectionPtr& ipPointCollection, const IEnvelopePtr& ipEnvelope)
{
	if(ipPointCollection->GetPointCount() != 2)
		return false;
	IPointPtr ipFrom = ipPointCollection->GetPoint(0);
	IPointPtr ipTo = ipPointCollection->GetPoint(1);

	if(!ipFrom || !ipTo)
		return false;

	IRelationalOperatorPtr ipRel(ipEnvelope);
	if(ipRel->_Disjoint(ipFrom) == VARIANT_TRUE)
		return false;
	if(ipRel->_Disjoint(ipTo) == VARIANT_TRUE)
		return false;
	return true;
}

bool CAutoSnap::Process(const IWorkspaceEditPtr& ipEdit, const IFeatureClassPtr& ipFeatures, const wstring& oid, const wstring& lon, const wstring& lat, const IGeometryPtr& ipMesh)
{
	//There are several steps to check the status of ArcObject.
	//It is complicated, So I do not put these codes into different functions.
	wcout << L"#" << oid << endl;

	IFeaturePtr ipFeature = GetFeature(ipFeatures, oid);
	//Feature does not exist , continue
	if(!ipFeature)
	{
		wcerr << oid.c_str() <<L" does not exist" << endl;
		Log(oid.c_str(), lon.c_str(), lat.c_str(), IDS_ERR_NO_OID);
		return true;
	}

	IGeometryPtr ipGeo = ipFeature->GetShapeCopy();
	IPointCollectionPtr ipPointCollection(ipGeo);
	//Geometry error
	if(!ipGeo || !ipPointCollection)
	{
		wcerr << L"GetShape failed! " << endl;	
		Log(oid.c_str(), lon.c_str(), lat.c_str(), IDS_ERR_NO_SHAPE);
		return true;
	}

	//Endpoint
	IPointPtr ipPoint;
	//Index number of end point
	long nIndex(0);
	//Extend line
	IPolylinePtr ipExt;	
	if(!GetPoint(ipPointCollection, lon, lat, nIndex, ipPoint, ipExt))
	{
		wcerr << L"Point not found " << oid << endl;
		//Endpoint not exists
		Log(oid.c_str(), lon.c_str(), lat.c_str(), IDS_ERR_NO_END_POINT);
		return true;
	}

	//Get the envelope
	IEnvelopePtr ipEnvelope = CreateEnvelope(ipPoint, m_cfg.radius);
	if(!ipEnvelope){
		wcerr << L"Create envelope failed" << endl;
		return false;
	}

	//The two endpoints of the line are all in the envelope,
	//So we can not judge the intersection point 
	//that which does it belong to.
	if(IsTooShort(ipPointCollection, ipEnvelope))
	{
		Log(oid.c_str(), lon.c_str(), lat.c_str(), IDS_ERR_MULTI_POINTS);
		return true;
	}

	ITopologicalOperatorPtr ipTopo(ipExt);
	if(!ipTopo)
	{
		wcerr << L"Create topological failed" << endl;
		Log(oid.c_str(), lon.c_str(), lat.c_str(), IDS_ERR_OUT_MESH);
		return false;
	}

	AllResults ar;
	int nVal = AllIntersect(ipTopo, ipEnvelope, ar);
	switch(nVal)
	{
	case LogicErr:
		wcerr << L"There are multiple intersection points" << endl;
		Log(oid.c_str(), lon.c_str(), lat.c_str(), IDS_ERR_MULTI_POINTS);
		return true;
	case SystemErr:
		wcerr << L"System error" << endl;
		return false;
	default:
		break;
	}

	//No intersection point exists
	if(!ar.ipPoint)
	{
		wcerr << L"There is no intersection point" << endl;
		Log(oid.c_str(), lon.c_str(), lat.c_str(), IDS_ERR_NO_POINT);
		return true;
	}

	//The intersection point is on the mesh border.
	if(IsOnMeshBorder(ar.ipPoint, ipMesh))
	{
		wcerr << L"Point is on mesh border" << endl;
		//On mesh border
		Log(oid.c_str(), ar.ipPoint, IDS_ERR_ON_BORDER);
		return true;
	}

	//For performance,
	//When the result of checking a geometry is OK, 
	//The intersection point will be inserted directly.
	//(In other words, the actions of checking and inserting are put together.)
	//And process the next geometry, and so on.
	//When the result of checking a geometry is NG,
	//Then all above inserted points of geometries will be aborted by calling AbortEditOperation,
	if(ipEdit->StartEditOperation() != S_OK)
	{
		wcerr << L"StartEditOperation Error" << endl;
		return false;
	}

	//Add the intersection point to snap feature
	nVal = InsertMainLayer(ar);
	switch(nVal)
	{
	case NoErr:
		if(!InsertExtLayer(ar, ipEnvelope))
		{
			//System error, so stop the program.
			ipEdit->AbortEditOperation();
			wcerr << L"System Error" << endl;
			return false;
		}
		break;
	case LogicErr:
		//Mini-Segment
		//Abort the current record and log an error
		//then process the next record
		//Result of checking a geometry is NG.
		ipEdit->AbortEditOperation();
		wcerr << L"Short segment" << endl;
		Log(oid.c_str(), ar.ipPoint, IDS_ERR_SHORT);
		return true;
	case SystemErr:
		//System error, stop program
		ipEdit->AbortEditOperation();
		wcerr << L"System Error" << endl;
		return false;
	default:
		break;
	}

	//Move the endpoint of the line to the intersection point
	if(!Equal(ipPoint, ar.ipPoint))
	{
		//Move and update the point
		if(ipPointCollection->UpdatePoint(nIndex, ar.ipPoint) != S_OK)
		{
			//System error
			ipEdit->AbortEditOperation();
			wcerr << L"Update point failed" << endl;
			return false;
		}
		//Storing failed
		//The use of ArcObject is complicated.
		ipFeature->PutRefShape(ipGeo);
		if(ipFeature->Store() != S_OK)
		{
			//System error
			ipEdit->AbortEditOperation();
			wcerr << L"Store failed" << endl;
			return false;
		}
		//Calculate the length
		double dOrg(0), dNew(0);
		if(GetLength(ipExt->GetFromPoint(), ipPoint, dOrg) &&
			GetLength(ipExt->GetFromPoint(), ar.ipPoint, dNew))
		{
			if(dOrg < dNew) //Longer: means that the line is extended.
				Log(oid.c_str(), ar.ipPoint, IDS_ADD_POINT_EXTENDED);
			else //Shorter:means that the line is cut.
				Log(oid.c_str(), ar.ipPoint, IDS_ADD_POINT_CUT);
		}
		else
		{
			//For can not get the length, 
			//So we don't know the result: longer or shorter
			Log(oid.c_str(), ar.ipPoint, IDS_ADD_POINT_UNKNOW);
		}
	}
	else
	{
		//Do not need to move the endpoint of line
		Log(oid.c_str(), ar.ipPoint, IDS_ADD_POINT);
	}

	if(ipEdit->StopEditOperation() != S_OK)
	{
		//System error
		wcerr << L"StopEditOperation Error" << endl;
		ipEdit->AbortEditOperation();
		return false;
	}
	return true;
}

bool CAutoSnap::IsShort(const IPointPtr& ipPointA, const IPointPtr& ipPointB, double length)
{
	double dLength(0);
	if(!GetLength(ipPointA, ipPointB, dLength))
		return false;
	return (dLength <= length);
}

bool CAutoSnap::GetLength(const IPointPtr& ipPointA, const IPointPtr& ipPointB, double& length)
{
	IProximityOperatorPtr ipProx(ipPointA);
	if(!ipProx) return false;
	length = ipProx->_ReturnDistance(ipPointB);	
	return true;
}

bool CAutoSnap::IsWithIn(const IPointPtr& ipPoint, const IPointPtr& ipFrom, const IPointPtr& ipTo)
{
	IPolylinePtr ipLine(CLSID_Polyline);
	if(!ipLine) return false;
	ipLine->PutRefSpatialReference(ipFrom->GetSpatialReference());
	ipLine->PutFromPoint(ipFrom);
	ipLine->PutToPoint(ipTo);
	IRelationalOperatorPtr ipRel(ipPoint);
	//Why the result is not as the excepting by _Within
	//if(ipRel && ipRel->_Within(ipLine)) return true;
	return (ipRel && ipRel->_Disjoint(ipLine) == VARIANT_FALSE);
}

int CAutoSnap::InsertPoint(const IFeaturePtr& ipFeature, const IPointPtr& ipPoint, double length, bool isBreak)
{
	bool isInserted = false;
	IPointCollectionPtr ipCollect(ipFeature->GetShapeCopy());
	if(!ipCollect) return SystemErr;
	
	for(long i =0; i < ipCollect->GetPointCount() -1 ; ++i)
	{
		IPointPtr ipFrom = ipCollect->GetPoint(i);
		IPointPtr ipTo = ipCollect->GetPoint(i + 1);
		if(IsWithIn(ipPoint, ipFrom, ipTo))
		{
			//Point exists, So not insert it.
			if(Equal(ipPoint, ipFrom) || Equal(ipPoint, ipTo))
			{
				wcout << L"Point exists in " << ipFeature->GetOID() << L" at " << i << endl;
				continue;
			}
			//Mini length problem
			if(IsShort(ipPoint,ipFrom, length) || IsShort(ipPoint, ipTo, length))
			{
				if(isBreak)
					return LogicErr;
				else
					continue;
			}
			//There is no mini-segment, insert the point directly.
			WKSPoint pointNew[1];
			pointNew[0].X = ipPoint->GetX();
			pointNew[0].Y = ipPoint->GetY();
			ipCollect->InsertWKSPoints(++i, 1, pointNew);
			isInserted = true;
			wcout << L"Insert point into " << ipFeature->GetOID() << L" at " << i <<endl;
		}
	}
	//When the point is inserted, Store it.
	if(isInserted)
	{
		ipFeature->PutRefShape(IGeometryPtr(ipCollect));
		if(ipFeature->Store() != S_OK)
		{
			wcerr << L"Store error" << endl;
			return SystemErr;
		}
	}
	return NoErr;
}

int CAutoSnap::InsertMainLayer(const AllResults& result)
{
	for(auto& lr : result.layers)
	{
		for(auto& sr : lr.result)
		{
			if(sr.isIntersect)
			{
				//The length between the intersection point and the original points is too short
				//According to the specification, We should report an error.
				int nVal = InsertPoint(sr.ipFeature, result.ipPoint, lr.length, true);
				//An error occurred!
				if(nVal != NoErr)
					return nVal;
			}
		}
	}
	return NoErr;
}

bool CAutoSnap::InsertExtLayer(const AllResults& result, const IEnvelopePtr& ipEnvelope)
{
	for(auto& layer : m_layers)
	{
		for(auto& extLayer : layer.ext)
		{
			LayerResults lr;
			wstring str;
			if(!GetSnapFeature(extLayer.feature, str, ipEnvelope, lr))
				return false;
			for(auto& res : lr.result)
			{
				//According to the specification,
				//We do not need to care the mini length problem of the ext layers.
				if(InsertPoint(res.ipFeature, result.ipPoint, extLayer.length, false) == SystemErr)
					return false;
			}
		}
	}
	return true;
}

int CAutoSnap::AllIntersect(const ITopologicalOperatorPtr& ipTopo, const IEnvelopePtr& ipEnvelope, AllResults& result)
{
	IPointPtr ipPoint;
	//Process all layers
	for(auto& layer : m_layers)
	{
		LayerResults lr;
		lr.length = layer.length;
		if(!GetSnapFeature(layer.feature, layer.filter, ipEnvelope, lr))
			return SystemErr;

		IPointPtr ipTmp;
		//Process one layer intersection
		int nVal = LayerIntersect(ipTopo, ipEnvelope, lr, ipTmp);
		if(nVal != NoErr)
			return nVal;;
		if(ipTmp)
		{
			//Multiple points (The Lon/Lat of these points are different)
			if(ipPoint && !Equal(ipPoint, ipTmp))
				return LogicErr;
			ipPoint = ipTmp;
		}

		result.layers.push_back(lr);
	}
	result.ipPoint = ipPoint;
	return NoErr;
}

int CAutoSnap::LayerIntersect(const ITopologicalOperatorPtr& ipTopo, const IEnvelopePtr& ipEnvelope, LayerResults& result, IPointPtr& ipPoint)
{
	IPointPtr ipCross;
	//Process all features
	for(auto& ls : result.result)
	{
		IPointPtr ipTmp;
		//Process one feature
		int nVal = Intersect(ipTopo, ipEnvelope, ls.ipFeature, ipTmp);
		if(nVal != NoErr)
		{
			return nVal;
		}
		if(ipTmp)
		{
			//Multiple points(The Lon/Lat of these points are different)
			if(ipCross && !Equal(ipTmp, ipCross))
				return LogicErr;
			ipCross = ipTmp;
			ls.isIntersect = true;
		}
	}
	ipPoint = ipCross;
	return NoErr;
}

int CAutoSnap::Intersect(const ITopologicalOperatorPtr& ipTopo, const IEnvelopePtr& ipEnvelope, const IFeaturePtr& ipFeature, IPointPtr& ipPoint)
{
	IGeometryPtr ipResult;
	IPointPtr ipCross;
	IRelationalOperatorPtr ipRel(ipEnvelope);
	if(!ipRel) return SystemErr; //System error

	//Get the intersection points
	HRESULT hr = ipTopo->Intersect(ipFeature->GetShape(), esriGeometryDimension::esriGeometry0Dimension, &ipResult);
	if(hr != S_OK || !ipResult)
		return NoErr; //Not intersect. no error

	IPointCollectionPtr ipPointConnection(ipResult);
	if(ipPointConnection)
	{
		for(long i=0; i< ipPointConnection->GetPointCount(); ++i)
		{
			//When the intersection point is not in the Envelope, Skip it!
			if(ipRel->_Disjoint(ipPointConnection->GetPoint(i)) == VARIANT_TRUE)
			{
				continue;
			}
			if(ipCross) //There already have been intersection point(s).
			{
				//Checking whether the Lon/Lat of these points are the same.
				if(!Equal(ipCross, ipPointConnection->GetPoint(i)))
				{
					//Not the same, Multiple points,
					return LogicErr;
				}
			}
			else
			{
				ipCross = ipPointConnection->GetPoint(i);
			}
		}
	}
	
	ipPoint = ipCross;
	return NoErr; //No error
}

bool CAutoSnap::GetSnapFeature(const IFeatureClassPtr& ipClass, wstring& strFilter, const IEnvelopePtr& ipEnv, LayerResults& result)
{
	//Get all snap objects by the range
	ISpatialFilterPtr ipSpatial(CLSID_SpatialFilter);
	if(!ipSpatial)
		return false;
	ipSpatial->putref_Geometry(ipEnv);
	ipSpatial->PutSpatialRel(esriSpatialRelEnum::esriSpatialRelIntersects);
	if(!strFilter.empty())
		ipSpatial->put_WhereClause(CComBSTR(strFilter.c_str()));

	IFeatureCursorPtr ipSnapCursor;
	if(ipClass->Search(ipSpatial, VARIANT_FALSE, &ipSnapCursor) != S_OK || !ipSnapCursor)
		return true;

	//Store all results
	IFeaturePtr ipSnapFeature;
	while(ipSnapCursor->NextFeature(&ipSnapFeature) == S_OK)
	{
		SnapResult sr;
		sr.ipFeature = ipSnapFeature;
		result.result.push_back(sr);
	}
	return true;
}

bool CAutoSnap::GetPoint(const IPointCollectionPtr& ipPointCollection, const wstring& lon, const wstring& lat, long& nIndex, IPointPtr& ipPoint, IPolylinePtr& ipExt)
{
	//Get endpoints
	IPointPtr ipFirst, ipLast;
	long nCount = ipPointCollection->GetPointCount();
	ipFirst = ipPointCollection->GetPoint(0);
	ipLast = ipPointCollection->GetPoint(nCount- 1);

	ISpatialReferencePtr ipRef = ipFirst->GetSpatialReference();
	//Extend line
	IPolylinePtr ipPolyline(CLSID_Polyline);
	ipPolyline->PutRefSpatialReference(ipRef);
	//For extend point calculation
	ILinePtr ipLine(CLSID_Line);
	ipLine->PutRefSpatialReference(ipRef);

	//Matches the from endpoint
	if(Equal(ipFirst, lon, lat))
	{
		ipLine->PutCoords(ipPointCollection->GetPoint(1), ipFirst);
		ipPolyline->PutFromPoint(ipPointCollection->GetPoint(1));
		ipPoint = ipFirst;
		nIndex = 0;
	}
	//Matches the to endpoint
	else if(Equal(ipLast, lon, lat))
	{
		ipLine->PutCoords(ipPointCollection->GetPoint(nCount - 2), ipLast);
		ipPolyline->PutFromPoint(ipPointCollection->GetPoint(nCount - 2));
		ipPoint = ipLast;
		nIndex = nCount- 1;
	}
	//Not matches
	else
	{
		//Not exists
		return false;
	}
	//Calculate the extend point by angle(line) and the length(m_raduis) of the line
	IConstructPointPtr ipCon(CLSID_Point);
	ipCon->ConstructAngleDistance(ipPoint, ipLine->GetAngle(), m_cfg.radius);
	//Put the extend point
	ipPolyline->PutToPoint(IPointPtr(ipCon));
	ipExt = ipPolyline;

	return true;
}

bool CAutoSnap::Equal(const IPointPtr& ipPoint, const wstring& lon, const wstring& lat)
{
	//Use the format of ShapeChecker: "%.15lf" as comparing string
	const CStringW str(L"%.15lf");
	CStringW strLon, strLat;
	strLon.Format(str, ipPoint->GetX());
	strLat.Format(str, ipPoint->GetY());
	return (strLon == lon.c_str() && strLat == lat.c_str());
}

bool CAutoSnap::Equal(const IPointPtr& ipPointA, const IPointPtr& ipPointB)
{
	//Tolerance???
	//What is the specification ?
	IRelationalOperatorPtr ipRel(ipPointA);
	//For debugging
	//std::wcerr << ipPointA->GetX() << L" " << ipPointA->GetY() << std::endl;
	//std::wcerr << ipPointB->GetX() << L" " << ipPointB->GetY() << std::endl;
	//
	if(ipRel)
	{
		return (ipRel->_Equals(ipPointB) == VARIANT_TRUE);
	}
	return false;
}

std::wstring CAutoSnap::GetFilenameByMesh(const std::wstring& str)
{
	// MeshCode(8 numbers) + ".mdb"
	std::wstring filename;
	//Check the number of numbers
	if(str.size() == 8)
	{
		std::wstring::const_iterator it = str.begin();
		filename = m_cfg.lot;
		filename.append(it, it + 4);
		filename += L'\\';
		filename += str;
		filename += L".mdb";
	}
	return filename;
}

std::wstring CAutoSnap::GetLogNameByMesh(const std::wstring& str)
{
	CStringW res;
	res.Format(m_cfg.log_format.c_str(), str.c_str());
	return res;
}

std::wstring CAutoSnap::GetSnapLogNameByMesh(const std::wstring& str)
{
	CStringW res;
	res.Format(m_cfg.snap_format.c_str(), str.c_str());
	return res;
}

IGeometryPtr CAutoSnap::GetMeshObject(const wstring& strMesh, const IFeatureClassPtr& ipClass)
{
	CStringW str;
	//Construct SQL
	str.Format(L"MESHCODE=%s", strMesh.c_str());
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	ipFilter->put_WhereClause(CComBSTR(str));
	IFeatureCursorPtr ipCursor;
	if(ipClass->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK)
	{
		wcerr << L"Search failed with "<< str << endl;
		return nullptr;
	}
	//Get the first object
	IFeaturePtr ipFeature;
	if(ipCursor->NextFeature(&ipFeature) != S_OK){
		wcerr << L"Mesh Code "<< strMesh.c_str() << "does not exist!" << endl;
		return nullptr;
	}

	if(!ipFeature){
		wcerr << L"NextFeature failed! " << endl;
		return nullptr;
	}
	//Return the geometry object
	return ipFeature->GetShapeCopy();
}
