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
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/directory_util.h>
#include <sindymacroutil.h>
#include <GeometryBase.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <SiNDYLogFunctions.h>
#include <ArcHelperEx/define.h>
#include <ArcHelperEx/AheMeshUtil.h>
#include <ArcHelperEx/AheGlobals.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <ArcHelperEx/AheLayerFunctions.h>
#include "CCreateDistancePost.h"
#include <TableBase.h>
#include <ErrorObject2.h>
#include "tracer/LinkCostList.h"
#include <sindy/workspace.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
using namespace ATL;

namespace sindy
{
	// ����DB�ڑ�
	bool CCreateDistancePost::Connect(LPCTSTR lpcszConnection)
	{
		m_cWorkspace = sindy::create_workspace( lpcszConnection );

		return (IWorkspace*)m_cWorkspace ? true : false;
	}

	// Distance_Post_Point�擾
	bool CCreateDistancePost::OpenDistancePostPoint()
	{
		if ( m_cWorkspace )
		{
			m_cDistancePostClass = m_cWorkspace.OpenTable(sindy::schema::distance_post_point::kTableName);
			m_cDistancePostClass.CreateFieldMap();
		}

		return (IFeatureClassPtr)m_cDistancePostClass ? true : false;
	}

	// ROAD_LINK/ROAD_NODE�擾
	bool CCreateDistancePost::OpenNetwork()
	{
		if ( m_cWorkspace )
		{
			m_cRoadLinkClass = m_cWorkspace.OpenTable(sindy::schema::road_link::kTableName);
			m_cRoadLinkClass.CreateFieldMap();

			m_cRoadNodeClass = m_cWorkspace.OpenTable(sindy::schema::road_node::kTableName);
			m_cRoadNodeClass.CreateFieldMap();
		}

		return ((IFeatureClassPtr)m_cRoadLinkClass && (IFeatureClassPtr)m_cRoadNodeClass) ? true : false;
	}

	// ���b�V���擾
	bool CCreateDistancePost::OpenMesh()
	{
		if ( m_cWorkspace )
			m_cBaseMeshClass = m_cWorkspace.OpenTable(sindy::schema::basemesh::kTableName);

		return (IFeatureClassPtr)m_cBaseMeshClass ? true : false;
	}

	// �H�����X�g�m�F
	long CCreateDistancePost::CheckRoadList(LPCTSTR lpcszFile, double dbPitch, std::map<long, std::pair<std::list<double>, std::map<double, long>>>& mapContainer, std::map<long,long>& mapErr)
	{
		using namespace sindy::schema::distance_post_point;

		std::ifstream aFin(lpcszFile);
		if (aFin)
		{
			long nErr = 0;
			std::string aRecord;
			long nLineCount = 0;
			while (std::getline(aFin, aRecord))
			{
				mapErr[nLineCount] = 0;

				vector<CString> vecAttr = uh::str_util::split( uh::toTStr(aRecord).c_str(), _T("\t") );

				if ( vecAttr.size() <= 1 || vecAttr.size() > 3 )
				{
					nErr = -3;
					mapErr[nLineCount] = nErr;
					++nLineCount;
					continue;
				}

				if ( vecAttr.at(0).GetLength() <= 0 )
				{
					nErr = -4;
					mapErr[nLineCount] = nErr;
					++nLineCount;
					continue;
				}

				if ( vecAttr.at(1).GetLength() <= 0 )
				{
					nErr = -5;
					mapErr[nLineCount] = nErr;
					++nLineCount;
					continue;
				}

				if ( vecAttr.size() == 2 )
					m_mapRoadList[nLineCount] = boost::make_tuple<long,long,CString>(_tcstol(vecAttr.at(0), 0, 10), _tcstol(vecAttr.at(1), 0, 10), _T(""));
				else
					m_mapRoadList[nLineCount] = boost::make_tuple<long,long,CString>(_tcstol(vecAttr.at(0), 0, 10), _tcstol(vecAttr.at(1), 0, 10), vecAttr.at(2));

				++nLineCount;
			}

			// �s���m�F
			if ( nLineCount <= 0 ) 
				return -2;

			CSPFieldMap pSpFieldMap = m_cDistancePostClass.GetFieldMap();
			long nIdx = pSpFieldMap->FindField( kDistance );
			for ( const auto& item : m_mapRoadList )
			{
				CString strWhere;
				strWhere.Format(_T("%s = %d and %s = %d and %s "), kRoadCode, item.second.get<0>(), kDirection, item.second.get<1>(), kDistanceMark);

				// �����W�L�����w�肳��Ă��Ȃ����NULL�ŁA�w�肳��Ă���΂��̋L���Ō���(bug12125)
				strWhere += (item.second.get<2>().IsEmpty()) ? _T("is null") : uh::str_util::format(_T("= '%s'"), item.second.get<2>());

				CFeatureClass::Container cContainer = m_cDistancePostClass.searchNoContain( AheInitQueryFilter(nullptr, nullptr, strWhere) );
				if ( cContainer.size() < 2 )
				{
					nErr = -6;
					mapErr[item.first] = nErr;
					continue;
				}

				// �����W�l�̏����\�[�g
				std::list<double> listDistances;
				std::map<double, long> mapOIDs;
				for ( const auto& cRow : cContainer )
				{
					CComVariant vaVal( cRow.second->GetValue(nIdx) );
					CString strTmp( convertStr(vaVal.fltVal) );

					listDistances.push_back(_tcstod(strTmp, nullptr));

					mapOIDs[vaVal.fltVal] = cRow.second->GetOID();
				}

				listDistances.sort();

				// �|�C���g�Ԃ̍ŏ��ő勗���W�l���s�b�`�ȉ����m�F
				if ( listDistances.back() - listDistances.front() <= dbPitch )
				{
					nErr = -7;
					mapErr[item.first] = nErr;
					continue;
				}

				mapContainer[item.first] = std::pair<std::list<double>, std::map<double, long>>(listDistances, mapOIDs);
			}

			return nErr;
		}

		return -1;
	}

	// �����W�|�C���g����
	void CCreateDistancePost::searchDistancePoint(const std::map<double, long>& mapValues)
	{
		std::list<CString> listIDs;

		for ( const auto& cVal : mapValues )
		{
			CString strTmp;
			strTmp.Format( _T("%d"), cVal.second );
			listIDs.push_back( strTmp );
		}

		std::list<CString> strIDs;
		CTableBase((IFeatureClassPtr)m_cDistancePostClass).IDs2Str(listIDs, strIDs);

		m_cDistancePostClass.Clear();
		for ( const auto& strID : strIDs )
		{
			CString strWhere;
			strWhere.Format( _T("%s in (%s)"), m_cDistancePostClass.getOIDFieldName(), strID );
			m_cDistancePostClass.search( AheInitQueryFilter(nullptr, nullptr, strWhere) );
		}
	}

	// �����W�|�C���g�Q�擾
	std::map<double, CSPRowBase> CCreateDistancePost::findDistancePoints()
	{
		CFeatureClass::Container cContainer = m_cDistancePostClass.getContainer();
		CSPFieldMap pSpFieldMap =  m_cDistancePostClass.GetFieldMap();
		long nIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kDistance );
		std::map<double, CSPRowBase> mapRows;

		for ( const auto& it : cContainer )
		{
			CComVariant vaVal( it.second->GetValue(nIdx) );
			CString strTmp( convertStr(vaVal.fltVal) );

			mapRows[_tcstod(strTmp, nullptr)] = it.second;
		}

		return mapRows;
	}

	// �����W�|�C���g�擾
	CSPRowBase CCreateDistancePost::findDistancePoint(double dbVal)
	{
		CFeatureClass::Container cContainer = m_cDistancePostClass.getContainer();
		CSPFieldMap pSpFieldMap =  m_cDistancePostClass.GetFieldMap();
		long nIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kDistance );
		CSPRowBase pRow;

		for ( const auto& it : cContainer )
		{
			CComVariant vaVal( it.second->GetValue(nIdx) );
			CString strTmp( convertStr(vaVal.fltVal) );
			CString strTmp2( convertStr(dbVal) );

			if ( _tcstod(strTmp, nullptr) == _tcstod(strTmp2, nullptr) )
			{
				pRow = it.second;
				break;
			}
		}

		return pRow;
	}

	// �����W�|�C���g����
	std::list<long> CCreateDistancePost::searchDistancePostOnRoad(const IGeometryPtr& ipGeom, double dbBuf)
	{
		sindy::CGeometryBase cGeom(ipGeom);
		IGeometryPtr ipBufferGeom( cGeom.Buffer( convert(dbBuf, esriMeters, esriDecimalDegrees) ) );

		CString strRoadClass;
		for ( const auto& nCode : LinkCostList::m_cRoadClassCostMap )
		{
			CString strTmp;
			strTmp.Format( _T("%d,"), nCode );
			strRoadClass += strTmp;
		}

		CString strLinkClass;
		for ( const auto& nCode : LinkCostList::m_cLinkClassCostMap )
		{
			CString strTmp;
			strTmp.Format( _T("%d,"), nCode );
			strLinkClass += strTmp;
		}

		CString strWhere;
		if ( strRoadClass.GetLength() > 0 )
		{
			CString strTmp;
			strTmp.Format( _T("%s in (%s)"), sindy::schema::road_link::kRoadClass, strRoadClass.Left(strRoadClass.GetLength()-1) );
			strWhere = strTmp;
		}

		if ( strLinkClass.GetLength() > 0 )
		{
			CString strTmp;
			strTmp.Format( _T("%s in (%s)"), sindy::schema::road_link::kMainLinkClass, strLinkClass.Left(strLinkClass.GetLength()-1) );
			if ( strWhere.GetLength() > 0 )
				strWhere += _T(" and ");
			strWhere += strTmp;
		}

		CFeatureClass::Container cContainer = m_cRoadLinkClass.searchNoContain( AheInitQueryFilter(AheInitSpatialFilter(nullptr, ipBufferGeom, m_cRoadLinkClass.getShapeFieldName()), NULL, strWhere) );

		std::list<long> listIDs;
		for ( const auto& cRoads : cContainer )
			listIDs.push_back( cRoads.first );

		return listIDs;
	}

	// ���H����
	CFeatureClass::Container CCreateDistancePost::searchRoads(const IGeometryPtr& ipGeom, double dbBuf)
	{
		CString strRoadClass;
		for ( const auto& nCode : LinkCostList::m_cRoadClassCostMap )
		{
			CString strTmp;
			strTmp.Format( _T("%d,"), nCode );
			strRoadClass += strTmp;
		}

		CString strLinkClass;
		for ( const auto& nCode : LinkCostList::m_cLinkClassCostMap )
		{
			CString strTmp;
			strTmp.Format( _T("%d,"), nCode );
			strLinkClass += strTmp;
		}

		CString strWhere;
		if ( strRoadClass.GetLength() > 0 )
		{
			CString strTmp;
			strTmp.Format( _T("%s in (%s)"), sindy::schema::road_link::kRoadClass, strRoadClass.Left(strRoadClass.GetLength()-1) );
			strWhere = strTmp;
		}

		if ( strLinkClass.GetLength() > 0 )
		{
			CString strTmp;
			strTmp.Format( _T("%s in (%s)"), sindy::schema::road_link::kMainLinkClass, strLinkClass.Left(strLinkClass.GetLength()-1) );
			if ( strWhere.GetLength() > 0 )
				strWhere += _T(" and ");
			strWhere += strTmp;
		}

		sindy::CGeometryBase cGeom(ipGeom);
		IGeometryPtr ipBufferGeom( cGeom.Buffer( convert(dbBuf, esriMeters, esriDecimalDegrees) ) );
		return m_cRoadLinkClass.searchNoContain( AheInitQueryFilter(AheInitSpatialFilter(nullptr, ipBufferGeom, m_cRoadLinkClass.getShapeFieldName()), nullptr, strWhere) );
	}

	// ���H�����p�`��擾
	IGeometryPtr CCreateDistancePost::searchRoadGeoms(const CFeatureClass::Container& cRoads, long nBase, const std::list<long>& listExcludeMesh, std::list<long>& listMesh)
	{
		CMesh mesh;

		mesh.SetSpatialReference( m_cRoadLinkClass.getSpRef() );
		for ( const auto& cRoad : cRoads )
		{
			int nMeshCode = mesh.GetExistMeshCode(cRoad.second->GetShapeCopy());

			for ( long i = nBase * -1; i <= nBase; ++i )
			{
				for ( long j = nBase * -1; j <= nBase; ++j )
				{
					if ( listExcludeMesh.end() != std::find(listExcludeMesh.begin(), listExcludeMesh.end(), AheGETMESHCODE( nMeshCode, i, j )) )
						continue;

					mesh.AddMesh( AheGETMESHCODE( nMeshCode, i, j ) );
				}
			}
		}

		return mesh.GetMeshesPolygon();
	}

	// ���H���O����
	void CCreateDistancePost::excludeRoad(CFeatureClass::Container& cRoads, const std::list<long>& listNotExcludeRoads, const std::list<long>& listRouteIDs)
	{
		// �[�_�m�[�h���ڑ����Ă��铹�H�͏��O
		CSPFieldMap pSpFieldMap =  m_cRoadLinkClass.GetFieldMap();
		long nFromIdx = pSpFieldMap->FindField( sindy::schema::road_link::kFromNodeID );
		long nToIdx = pSpFieldMap->FindField( sindy::schema::road_link::kToNodeID );
		std::list<CString> listNodeIDs;

		std::map<long, std::pair<CString,CString>> mapRoadNodes;
		for ( const auto& cRoad : cRoads )
		{
			CString strFrom;
			CComVariant vaFromVal( cRoad.second->GetValue(nFromIdx) );

			strFrom.Format( _T("%d"), vaFromVal.lVal);
			listNodeIDs.push_back(strFrom);

			CString strTo;
			CComVariant vaToVal( cRoad.second->GetValue(nToIdx) );

			strTo.Format( _T("%d"), vaToVal.lVal);
			listNodeIDs.push_back(strTo);

			mapRoadNodes[cRoad.second->GetOID()] = std::pair<CString,CString>(strFrom, strTo);
		}

		// �m�[�h����
		std::list<CString> strIDs;
		CTableBase((IFeatureClassPtr)m_cRoadNodeClass).IDs2Str(listNodeIDs, strIDs);

		for ( const auto& strID : strIDs )
		{
			CString strWhere;
			strWhere.Format( _T("%s in (%s) and %s = %d"), m_cRoadNodeClass.getOIDFieldName(), strID, sindy::schema::road_node::kNodeClass, sindy::schema::road_node::node_class::kTerminal );
			m_cRoadNodeClass.search( AheInitQueryFilter(nullptr, nullptr, strWhere) );
		}

		// �[�_�m�[�h�ɐڑ����Ă��铹�H�����O
		std::list<long> listExcludeRoads;
		CFeatureClass::Container cContainer = m_cRoadNodeClass.getContainer();

		for ( const auto& cRoadNode : mapRoadNodes )
		{
			if ( cContainer.end() != cContainer.find( _tcstol(cRoadNode.second.first, 0, 10) ) ||
				 cContainer.end() != cContainer.find( _tcstol(cRoadNode.second.second, 0, 10) ) )
			{
				// ���O���Ă͂����Ȃ����X�g�ɂȂ���Ώ��O
				if ( listNotExcludeRoads.end() == std::find(listNotExcludeRoads.begin(), listNotExcludeRoads.end(), cRoadNode.first) &&
					 listExcludeRoads.end() == std::find(listExcludeRoads.begin(), listExcludeRoads.end(), cRoadNode.first) )
					// ���O
					listExcludeRoads.push_back( cRoadNode.first );
			}
		}

		// �o�H�T���ς݂̓��H�����O����
		for ( const auto& cRoadNode : mapRoadNodes )
		{
			if ( listRouteIDs.end() != std::find(listRouteIDs.begin(), listRouteIDs.end(), cRoadNode.first) && 
				 listNotExcludeRoads.end() == std::find(listNotExcludeRoads.begin(), listNotExcludeRoads.end(), cRoadNode.first) &&
				 listExcludeRoads.end() == std::find(listExcludeRoads.begin(), listExcludeRoads.end(), cRoadNode.first) )
				// ���O
				listExcludeRoads.push_back( cRoadNode.first );
		}

		// ���O�������H�ɐڑ����铹�H��1�{�����Ȃ����́A���̓��H�����O����
		for ( const auto& cExRoad : listExcludeRoads )
		{
#if _DEBUG
			CString strTmp;
			strTmp.Format(_T(" Exclude OID:%d"), cExRoad);
			cerr << CT2CA(strTmp) << endl;
#endif

			long nFromId = _tcstol(mapRoadNodes[cExRoad].first, 0, 10);
			long nToId = _tcstol(mapRoadNodes[cExRoad].second, 0, 10);

			std::list<long> listConnRoads;
			for ( const auto& cRoadNode : mapRoadNodes )
			{
				if ( listExcludeRoads.end() != std::find(listExcludeRoads.begin(), listExcludeRoads.end(), cRoadNode.first) )
					continue;

				if ( _tcstol(cRoadNode.second.first, 0, 10) == nFromId || _tcstol(cRoadNode.second.first, 0, 10) == nToId ||
					 _tcstol(cRoadNode.second.second, 0, 10) == nFromId || _tcstol(cRoadNode.second.second, 0, 10) == nToId )
				{
					listConnRoads.push_back(cRoadNode.first);
					if ( listConnRoads.size() >= 2 )
						break;
				}
			}

			if ( listConnRoads.size() == 1 )
			{
				if ( listNotExcludeRoads.end() == std::find(listNotExcludeRoads.begin(), listNotExcludeRoads.end(), listConnRoads.front()) &&
					 listExcludeRoads.end() == std::find(listExcludeRoads.begin(), listExcludeRoads.end(), listConnRoads.front()) )
					// ���O
					listExcludeRoads.push_back( listConnRoads.front() );
			}
		}

		// �R���e�i����폜
		CFeatureClass::Container::iterator it = cRoads.begin();
		while (it != cRoads.end())
		{
			if ( listExcludeRoads.end() != std::find(listExcludeRoads.begin(), listExcludeRoads.end(), it->first) )
				cRoads.erase(it++);
			else
				++it;
		}
	}

	// �����W�|�C���g�`��쐬
	bool CCreateDistancePost::createDistancePoint(const IPolylinePtr& ipPolyline, double dbFirst, double dbEnd, double dbPitch, double dbLen, long nSplitNum, std::map<double, IPointPtr>& mapPoints, double dbLenStart, double dbLenEnd, bool bPitch)
	{
		if ( ! ipPolyline )
			return false;

		double dbLineLen = GetLength(ipPolyline);
		double dbPitchTmp = convert(dbPitch, esriKilometers, esriMeters); 

		if ( nSplitNum > 0 )
		{
			CString strTmp( convertStr(dbLen, 0) );
			double dbLenTmp = _tcstod(strTmp, nullptr);
			double dbPointVal = 0, dbVal = 0;

			// �s�b�`�P�ʂŕ������邩�ǂ���
			if ( bPitch )
			{
				// �P�ʍ��킹�����_�ƕ␳�_���̂݋�����10�����ł͂Ȃ��A�s�b�`�P�ʂō쐬����
				dbPointVal = dbPitchTmp;
				dbVal = dbPitchTmp;
			}
			else
			{
				// �|�C���g�̈ʒu�͋�����10���������ʒu�ō쐬
				dbPointVal = dbLenTmp / nSplitNum;

				// �����W�l�͕␳�_�Ԃ̋����W�l��10���������l������
				dbVal = (dbEnd - dbFirst) / nSplitNum;
			}

			for ( long i = 1; i < nSplitNum; ++i )
			{
				CString strTmp( convertStr(dbFirst + dbVal * i, 0) );

				// �s�b�`�P�ʂŕ�������dbEnd�𒴂�����I���悤�ɂ���
				if ( bPitch )
				{
					if ( _tcstod(strTmp, nullptr) > dbEnd )
						break;
				}

				mapPoints[_tcstod(strTmp, nullptr)] = split_distance(ipPolyline, dbLenStart + (dbPointVal * i));
			}
		}
		else if ( nSplitNum == 0 )
		{
			// �����ŕ������Ă���
			for ( double dbVal = dbPitchTmp; dbVal < dbLineLen - (dbLenStart+dbLenEnd); dbVal += dbPitchTmp)
			{
				CString strTmp( convertStr(dbFirst + dbVal, 0) );
				mapPoints[_tcstod(strTmp, nullptr)] = split_distance(ipPolyline, dbLenStart + dbVal);
			}
		}
		else
		{
			// �w�肳�ꂽ������1�|�C���g�����쐬
			CString strTmp( convertStr(dbEnd, 0) );
			mapPoints[_tcstod(strTmp, nullptr)] = split_distance(ipPolyline, dbFirst);
		}

		return true;
	}

	// ����(km)�擾
	double CCreateDistancePost::GetLength(const IPolylinePtr& ipPolyline)
	{
		ISegmentCollectionPtr ipSegColl( ipPolyline );
		double dbLen = 0.0f;

		if ( ipSegColl )
		{
			long nCount = 0;

			ipSegColl->get_SegmentCount( &nCount );

			for ( long i = 0; i < nCount; ++i )
			{
				_ISegmentPtr ipSeg;

				ipSegColl->get_Segment( i, &ipSeg );

				double dbSegLen = 0.0f;
				ipSeg->get_Length( &dbSegLen );

				dbLen += dbSegLen;
			}
		}

		return dbLen;
	}

	// �����_�擾�i�����j
	IPointPtr CCreateDistancePost::split_distance(const IPolylinePtr& ipPolyline, double dbVal)
	{
		IGeometryPtr ipSplitPoint;

		if ( dbVal != 0.0f )
		{
			VARIANT_BOOL vaSplitHappened = VARIANT_FALSE;
			long lNewPartIndex = -1, lNewSegmentIndex = -1;

			ipPolyline->SplitAtDistance( dbVal, VARIANT_FALSE, VARIANT_FALSE, &vaSplitHappened, &lNewPartIndex, &lNewSegmentIndex );

			ISegmentCollectionPtr ipSegColl( ipPolyline );
			_ISegmentPtr ipSegment;
			long nSegCnt = 0;

			ipSegColl->get_SegmentCount( &nSegCnt );
			ipSegColl->get_Segment( lNewSegmentIndex, &ipSegment );

			if ( ipSegment )
			{
				IPointPtr ipFromPoint;
				ipSegment->get_FromPoint( &ipFromPoint );
				ipSplitPoint = ipFromPoint;
			}
		}
		else
		{
			IPointCollectionPtr ipPointColl( ipPolyline );

			if ( ipPointColl )
			{
				IPointPtr ipPoint;
				ipPointColl->get_Point( 0, &ipPoint );
				ipSplitPoint = ipPoint;
			}
		}

		return ipSplitPoint;
	}

	// �����_�擾�i�|�C���g�j	
	IPointPtr CCreateDistancePost::split_point(const IPolylinePtr& ipPolyline, const IPointPtr& ipPoint, double dbVal, double& dbLen)
	{
		VARIANT_BOOL vaSplitHappened = VARIANT_FALSE;
		long lNewPartIndex = -1, lNewSegmentIndex = -1;

		ipPolyline->SplitAtPoint( ipPoint, VARIANT_FALSE, VARIANT_FALSE, &vaSplitHappened, &lNewPartIndex, &lNewSegmentIndex );

		// �����_�܂ł̋����擾
		ISegmentCollectionPtr ipSegCol( ipPolyline );
		long nSegCnt = 0;

		dbLen = 0.0f;

		ipSegCol->get_SegmentCount( &nSegCnt );
		for( long i = 0; i < lNewSegmentIndex; ++i )
		{
			_ISegmentPtr ipSegment;

			ipSegCol->get_Segment( i, &ipSegment );
			if( ipSegment )
			{
				double dbSegLen = 0.0f;
				ipSegment->get_Length( &dbSegLen );

				dbLen += dbSegLen;
			}
		}
		dbLen += convert( dbVal, esriKilometers, esriMeters);

		return split_distance( ipPolyline, dbLen );
	}

	// �ҏW�J�n
	void CCreateDistancePost::startEdit()
	{
		m_cWorkspace.StartEditing(false);
		m_cWorkspace.StartEditOperation();
	}

	// �ҏW�I��
	void CCreateDistancePost::stopEdit()
	{
		m_cWorkspace.StopEditOperation();
		m_cWorkspace.StopEditing(true);
	}

	// �ҏW�j��
	void CCreateDistancePost::abortEdit()
	{
		m_cWorkspace.AbortEditOperation();
	}

	// �����W�|�C���g�쐬
	CSPRowBase CCreateDistancePost::createDistancePoint(const IPointPtr& ipPoint, double dbVal, long nLineNo)
	{
		CSPFieldMap pSpFieldMap =  m_cDistancePostClass.GetFieldMap();
		long nIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kDistance );
		long nRIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kRoadCode );
		long nDIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kDirection );
		long nMIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kDistanceMark );
		CSPRowBase pRow = m_cDistancePostClass.Create();

		if ( ipPoint )
			ipPoint->Project(m_cDistancePostClass.getSpRef());	// ���̍��W�n�ɖ߂�

		pRow->SetShape( ipPoint );
		pRow->SetValue( nIdx, dbVal );
		pRow->SetValue( nRIdx, m_mapRoadList[nLineNo].get<0>() );
		pRow->SetValue( nDIdx, m_mapRoadList[nLineNo].get<1>() );
		// FGDB�ł��̃t�B�[���h��NULL�����悤�Ƃ���Ƌ󕶎������邽�߁ANULL������ꍇ��SetValue���Ȃ�
		if (m_mapRoadList[nLineNo].get<2>() != "") {
			pRow->SetValue(nMIdx, m_mapRoadList[nLineNo].get<2>());
		}

		sindy::CErrorObjectsBase cErrs;
		pRow->Store(cErrs);

		return pRow;
	}

	// �P�ʕϊ�
	double CCreateDistancePost::convert(double dbValue, esriUnits esriSrc, esriUnits esriDest)
	{
		IUnitConverterPtr ipUnit( CLSID_UnitConverter );
		double dbRet = 0.0f;

		atl2::valid(ipUnit->ConvertUnits( dbValue, esriSrc, esriDest, &dbRet ));
		return dbRet;
	}

	// �R�X�g�t�@�C���ǂݍ���(���H���)
	void CCreateDistancePost::ReadRoadCost(LPCTSTR lpcszFile)
	{
		std::ifstream aFin(lpcszFile);
		if (aFin)
		{
			std::string aRecord;
			long nLineCount = 0;
			while (std::getline(aFin, aRecord))
			{
				vector<CString> vecAttr = uh::str_util::split( uh::toTStr(aRecord).c_str(), _T("\t") );

				if ( vecAttr.size() != 2 )// 2��ȊO�͓ǂݍ��܂Ȃ�
					continue;

				if ( vecAttr.at(0).GetLength() <= 0 || vecAttr.at(1).GetLength() <= 0 )
					continue;

				LinkCostList::m_cRoadClassCostMap[_tcstol(vecAttr.at(0), 0, 10)] = _tcstol(vecAttr.at(1), 0, 10);

				++nLineCount;
			}
		}
	}

	// �R�X�g�t�@�C���ǂݍ���(�����N���)
	void CCreateDistancePost::ReadLinkCost(LPCTSTR lpcszFile)
	{
		std::ifstream aFin(lpcszFile);
		if (aFin)
		{
			std::string aRecord;
			long nLineCount = 0;
			while (std::getline(aFin, aRecord))
			{
				vector<CString> vecAttr = uh::str_util::split( uh::toTStr(aRecord).c_str(), _T("\t") );

				if ( vecAttr.size() != 2 )	// 2��ȊO�͓ǂݍ��܂Ȃ�
					continue;

				if ( vecAttr.at(0).GetLength() <= 0 || vecAttr.at(1).GetLength() <= 0 )
					continue;

				LinkCostList::m_cLinkClassCostMap[_tcstol(vecAttr.at(0), 0, 10)] = _tcstol(vecAttr.at(1), 0, 10);

				++nLineCount;
			}
		}
	}

	// �t�B�[���h�ԍ��擾
	long CCreateDistancePost::GetFieldIndex(LPCTSTR lpcszField)
	{
		CSPFieldMap pSpFieldMap = m_cDistancePostClass.GetFieldMap();
		return pSpFieldMap->FindField( lpcszField );
	}

	// COM�I�u�W�F�N�g�̃R�s�[���쐬����
	IClonePtr CCreateDistancePost::Clone( const IUnknownPtr& ipUnk )
	{
		IClonePtr ipClone( ipUnk ), ipRet;

		if( ! ipClone )
			return nullptr;

		atl2::valid(ipClone->Clone( &ipRet ));

		return ipRet;
	}

	// ���H�����iOID�w��j
	CFeatureClass::Container CCreateDistancePost::searchRoads(const std::list<long>& listRouteIDs)
	{
		std::list<CString> listIDs;

		for ( const auto& cId : listRouteIDs )
		{
			CString strTmp;
			strTmp.Format( _T("%d"), cId );
			listIDs.push_back( strTmp );
		}

		std::list<CString> strIDs;
		CTableBase((IFeatureClassPtr)m_cRoadLinkClass).IDs2Str(listIDs, strIDs);

		m_cRoadLinkClass.Clear();
		for ( const auto& strID : strIDs )
		{
			CString strWhere;
			strWhere.Format( _T("%s in (%s)"), m_cRoadLinkClass.getOIDFieldName(), strID );
			m_cRoadLinkClass.search( AheInitQueryFilter(NULL, NULL, strWhere) );
		}

		return m_cRoadLinkClass.getContainer();
	}

	// �����_�܂ł̋����擾
	double CCreateDistancePost::split_point_length(const IPolylinePtr& ipPolyline, const IPointPtr& ipPoint)
	{
		VARIANT_BOOL vaSplitHappened = VARIANT_FALSE;
		long lNewPartIndex = -1, lNewSegmentIndex = -1;

		ipPolyline->SplitAtPoint( ipPoint, VARIANT_FALSE, VARIANT_FALSE, &vaSplitHappened, &lNewPartIndex, &lNewSegmentIndex );

		// �����_�܂ł̋����擾
		ISegmentCollectionPtr ipSegCol( ipPolyline );
		long nSegCnt = 0;
		double dbLen = 0.0f;

		ipSegCol->get_SegmentCount( &nSegCnt );
		for( long i = 0; i < lNewSegmentIndex; ++i )
		{
			_ISegmentPtr ipSegment;

			ipSegCol->get_Segment( i, &ipSegment );
			if( ipSegment )
			{
				double dbSegLen = 0.0f;
				ipSegment->get_Length( &dbSegLen );

				dbLen += dbSegLen;
			}
		}

		return dbLen;
	}

	// ��������2�_�Ԃ̋������擾
	double CCreateDistancePost::split_length(const IPolylinePtr& ipPolyline, const IPointPtr& ipPoint1, const IPointPtr& ipPoint2)
	{
		VARIANT_BOOL vaSplitHappened = VARIANT_FALSE;
		long lNewPartIndex1 = -1, lNewSegmentIndex1 = -1;
		long lNewPartIndex2 = -1, lNewSegmentIndex2 = -1;

		ipPolyline->SplitAtPoint( ipPoint1, VARIANT_FALSE, VARIANT_FALSE, &vaSplitHappened, &lNewPartIndex1, &lNewSegmentIndex1 );
		ipPolyline->SplitAtPoint( ipPoint2, VARIANT_FALSE, VARIANT_FALSE, &vaSplitHappened, &lNewPartIndex2, &lNewSegmentIndex2 );

		ISegmentCollectionPtr ipSegColl( ipPolyline );
		if ( !ipSegColl )
			return -1.0f;

		double dbLen = 0.0f;
		for ( long i = lNewSegmentIndex1; i < lNewSegmentIndex2; ++i )
		{
			_ISegmentPtr ipSegment;
			ipSegColl->get_Segment( i, &ipSegment );

			double dbSegLen = 0.0f;
			ipSegment->get_Length( &dbSegLen );

			dbLen += dbSegLen;
		}

		return dbLen;
	}

	// �؂�l��
	CString CCreateDistancePost::convertStr(double dbVal, long nLen)
	{
		double dbTmp = dbVal * pow(10.0f, nLen);

		dbTmp = (double)(LONGLONG)(dbTmp + 0.9f);
    
		CString strTmp;
		strTmp.Format( _T("%.1lf"), dbTmp * pow(10.0f, -nLen) );

		return strTmp;
	}

	// �������鋗���W�|�C���g���擾����
	bool CCreateDistancePost::getCreateDistancePost(
		const std::list<double>& existDistanceList,
		const std::map<double, sindy::CSPRowBase>& mapRows,
		const IProjectedCoordinateSystemPtr& ipPrjCoordinateSystem,
		IPolylinePtr& ipAllPolyline,
		std::map<double, IPointPtr>& mapPoints,
		double dbPitch,
		double dbBuff,
		double correctionPointLen,
		CString& errorMsg)
	{
		// �����W�l���ɏ���
		sindy::LinkTracer cLinkTracer;

		cLinkTracer.toStartSelectLinkSection();

		std::list<long> listNotExcludeRoads;	// ���O���Ă͂����Ȃ����H�i�����W�|�C���g������Ă��铹�H�j
		std::list<long> listExcludeMesh;
		sindy::CFeatureClass::Container cRoads;
		std::list<long> listRouteIDs;
		IPointPtr ipFirstRoadFromPoint;
		double dbLenStart = 0.0f, dbLenEnd = 0.0f;

		for (std::list<double>::const_iterator it = existDistanceList.begin(); it != existDistanceList.end(); ++it)
		{
			double dbFirst = 0.0f, dbEnd = 0.0f, dbFirstTmp = 0.0f, dbEndTmp = 0.0f;
			bool bFirst = false, bEnd = false;

			// �n�_
			sindy::CSPRowBase pFirstRow = mapRows.at(*it);

			if (existDistanceList.begin() == it)
			{
				bFirst = true;

				// �P�ʍ��킹
				dbFirstTmp = (*it);

				// 1km�s�b�`�ȏ�̎��ɂ̂ݒP�ʍ��킹���s��
				if (dbPitch >= 1.0f)
					dbFirst = ((*it) - fmod((*it), dbPitch)) + dbPitch;
				else
					dbFirst = (*it);
			}
			else
			{
				dbFirst = (*it);
			}

			// ���_
			++it;
			if (existDistanceList.end() == it)	// �I�_�͉������Ȃ�
			{
				break;
			}
			else
			{
				++it;
				// �P�ʍ��킹
				if (existDistanceList.end() == it)
				{
					bEnd = true;
					--it;
					if (dbPitch >= 1.0f)
					{
						dbEndTmp = (*it);
						dbEnd = (*it) - fmod((*it), dbPitch);
					}
					else
						dbEnd = (*it);
				}
				else
				{
					--it;
					dbEnd = (*it);
				}
			}

			sindy::CSPRowBase pSecondRow = mapRows.at(*it);
			--it;

			// �n�_�擾
			std::list<long> listFirstRoads = searchDistancePostOnRoad(pFirstRow->GetShapeCopy(), dbBuff);

			if (listFirstRoads.empty())
			{
				errorMsg.Format(_T("ERROR:<%ld>�����H�����N��ɐݒ肳��Ă��܂���B"), pFirstRow->GetOID());
				return false;
			}

			if (listFirstRoads.size() > 2) {
				errorMsg.Format(_T("ERROR:<%ld>��3�{�ȏ�̓��H�����N�ɏ���Ă��܂��B"), pFirstRow->GetOID());
				return false;
			}

			for (const auto& cFrist : listFirstRoads)
				listNotExcludeRoads.push_back(cFrist);

			// ���_�擾
			std::list<long> listSecondRoads = searchDistancePostOnRoad(pSecondRow->GetShapeCopy(), dbBuff);

			if (listSecondRoads.empty())
			{
				errorMsg.Format(_T("ERROR:<%ld>�����H�����N��ɐݒ肳��Ă��܂���B"), pSecondRow->GetOID());
				return false;
			}

			if (listSecondRoads.size() > 2) {
				errorMsg.Format(_T("ERROR:<%ld>��3�{�ȏ�̓��H�����N�ɏ���Ă��܂��B"), pSecondRow->GetOID());
				return false;
			}

			for (const auto& cSecond : listSecondRoads)
				listNotExcludeRoads.push_back(cSecond);

			// ���H����
			ISpatialReferencePtr ipSpRef;
			pFirstRow->GetShapeCopy()->get_SpatialReference(&ipSpRef);

			IGeometryPtr ipSearchGeom(CLSID_GeometryBag);
			ipSearchGeom->putref_SpatialReference(ipSpRef);

			IGeometryCollectionPtr ipGeomColl(ipSearchGeom);
			ipGeomColl->AddGeometry(pFirstRow->GetShapeCopy());

			sindy::CFeatureClass::Container cRoadsTmp = searchRoads(ipSearchGeom, dbBuff);

			long nBase = 1;
		search_road:

			std::list<long> listMesh;
			sindy::CFeatureClass::Container cRoads1 = searchRoads(searchRoadGeoms(cRoadsTmp, nBase, listExcludeMesh, listMesh), dbBuff);
			listExcludeMesh.merge(listMesh);
			cRoads.insert(cRoads1.begin(), cRoads1.end());

			// ���H���O
			excludeRoad(cRoads, listNotExcludeRoads, listRouteIDs);

			// ���_������Ă��铹�H�����邩�m�F
			bool bFind = false;
			for (const auto& cRoad : cRoads)
			{
				if (listSecondRoads.end() != std::find(listSecondRoads.begin(), listSecondRoads.end(), cRoad.first))
				{
					bFind = true;
					break;
				}
			}

			// �Ȃ����͍Č���
			if (!bFind)
			{
				++nBase;
				cRoadsTmp = cRoads;
				goto search_road;
			}

			// �o�H�T���J�n
			cLinkTracer.toStartSelectLinkSection();
			cLinkTracer.init_cache(getRoadLink(), dbBuff);

			// �o�H�T���N���X�ɓ��H��n��
			for (const auto& cRoad : cRoads)
				cLinkTracer.add_cache(cRoad.first, (_IRow*)(*cRoad.second.get()));
			cLinkTracer.cache();

			// �o�H�T��
			cLinkTracer.onNormalMode(pFirstRow->GetShapeCopy());
			cLinkTracer.onNormalMode(pSecondRow->GetShapeCopy());

			// �o�H�T���`��擾
			IPolylinePtr ipPolyline = cLinkTracer.getRouteGeom();
			if (!ipPolyline)
			{
				errorMsg.Format(_T("ERROR:�o�H�v�Z�Ɏ��s���܂����B<%ld>�܂���<%ld>�����H�����N��ɐݒ肳��Ă��Ȃ��\��������܂��B"), pFirstRow->GetOID(), pSecondRow->GetOID());
				return false;
			}

			for (const auto& cRoute : cLinkTracer.getRouteIDs())
			{
				if (listRouteIDs.end() == std::find(listRouteIDs.begin(), listRouteIDs.end(), cRoute))
					listRouteIDs.push_back(cRoute);
			}

			// �N���[���擾
			ipPolyline = Clone(ipPolyline);

			// ���W�n��ύX
			ipPolyline->Project(ipPrjCoordinateSystem);

			IPointPtr ipFirstP(pFirstRow->GetShapeCopy());
			IPointPtr ipSecondP(pSecondRow->GetShapeCopy());

			ipFirstP->Project(ipPrjCoordinateSystem);	// ���W�n��ύX
			ipSecondP->Project(ipPrjCoordinateSystem);	// ���W�n��ύX

			if (!ipFirstRoadFromPoint)
			{
				// �o�H�T���������H����1�{�̎��ɁA�`��̌��������H�̌����ɂȂ��Ă��鎖������̂�
				// �����������W�|�C���g���u����Ă�������ɂ��킹��
				double dbFLen = split_point_length(Clone(ipPolyline), ipFirstP);
				double dbSLen = split_point_length(Clone(ipPolyline), ipSecondP);

				// ���_�ƃ��C���̎n�_�܂ł̋������N�_�ƃ��C���̎n�_�܂ł̋��������߂��Ȃ甽�]
				if (dbFLen > dbSLen)
					ipPolyline->ReverseOrientation();

				ipFirstRoadFromPoint = AheGetFromPoint(ipPolyline);
				ipFirstRoadFromPoint->Project(ipPrjCoordinateSystem);	// ���W�n��ύX
			}

			IPointPtr ipToP;
			bool bF = true;
			sindy::CFeatureClass::Container cRoads2 = searchRoads(listRouteIDs);
			for (const auto& cRoute : listRouteIDs)
			{
				IPolylinePtr ipPoly(cRoads2[cRoute]->GetShapeCopy());
				ipPoly->Project(ipPrjCoordinateSystem);	// ���W�n��ύX

				IPointPtr ipF(AheGetFromPoint(ipPoly));
				ipF->Project(ipPrjCoordinateSystem);	// ���W�n��ύX

				if (bF)
				{
					ipPoly = Clone(ipPoly);
					ipPoly->Project(ipPrjCoordinateSystem);	// ���W�n��ύX

					if (!AheIsEqual(ipFirstRoadFromPoint, ipF))
						ipPoly->ReverseOrientation();

					ipAllPolyline = ipPoly;
					bF = false;
				}
				else
				{
					if (!AheIsEqual(ipToP, ipF))
						ipPoly->ReverseOrientation();

					IPointCollectionPtr ipPointColl(ipAllPolyline);
					IPointCollectionPtr ipPointColl2(ipPoly);

					ipPointColl2->RemovePoints(0, 1);
					ipPointColl->AddPointCollection(ipPointColl2);
				}

				ipToP = AheGetToPoint(ipAllPolyline);
				ipToP->Project(ipPrjCoordinateSystem);	// ���W�n��ύX
			}
			ipAllPolyline->Project(ipPrjCoordinateSystem);	// ���W�n��ύX

															// �N�_���͏I�_��O�̕␳�_���́A�P�ʍ��킹�����|�C���g���쐬
			if (bFirst)
			{
				mapPoints[convert(dbFirst, esriKilometers, esriMeters)] = split_point(ipAllPolyline, ipFirstP, dbFirstTmp != 0.0f ? dbFirst - dbFirstTmp : dbFirst, dbLenStart);
			}

			if (bEnd)
			{
				ipAllPolyline->ReverseOrientation();
				mapPoints[convert(dbEnd, esriKilometers, esriMeters)] = split_point(ipAllPolyline, ipSecondP, dbEndTmp != 0.0f ? dbEndTmp - dbEnd : 0.0f, dbLenEnd);
				ipAllPolyline->ReverseOrientation();	// ���ɖ߂�
			}

			// �����W�|�C���g�Ԃ̋����W�l�̍����擾 [bug 12017]
			double distanceDiff = dbEnd - dbFirst;

			bool pitchAdjust = false; // �P�ʍ��킹�������ǂ��� [bug 12017]

			// �w��s�b�`��1km�ȏ�̎��ɁA���������W�|�C���g�̋����W�l���w��s�b�`�Ŋ���؂�Ȃ��ꍇ�A�P�ʍ��킹���s�� [bug 12017]
			if (!bFirst && distanceDiff >= dbPitch && dbPitch >= 1.0f && fmod(dbFirst, dbPitch) != 0)
			{
				// �P�ʍ��킹
				dbFirstTmp = dbFirst;
				dbFirst = ((dbFirst)-fmod(dbFirst, dbPitch)) + dbPitch;
				pitchAdjust = true;

				// �P�ʍ��킹�����|�C���g���쐬
				double mapPointskey = convert(dbFirst, esriKilometers, esriMeters);
				double splitVal = dbFirst - dbFirstTmp;
				mapPoints[mapPointskey] = split_point(ipAllPolyline, ipFirstP, splitVal, dbLenStart);
			}

			// �w��|�C���g�Ԃ̏���
			if (2 == existDistanceList.size())
			{
				// �N�_/�I�_�݂̂̏ꍇ
				createDistancePoint(ipAllPolyline, convert(dbFirst, esriKilometers, esriMeters), convert(dbEnd, esriKilometers, esriMeters), dbPitch, dbPitch, 0, mapPoints, dbLenStart, dbLenEnd, false);
			}
			else
			{
				// �w��|�C���g��3�_�ȏ�̏ꍇ(�␳�_�܂��͊��������W�|�C���g����)
				if (bFirst) {
					ipFirstP = mapPoints[convert(dbFirst, esriKilometers, esriMeters)];
				}
				else if (pitchAdjust) {
					ipFirstP = mapPoints[convert(dbFirst, esriKilometers, esriMeters)];
				}
				else {
					split_point(ipAllPolyline, ipFirstP, 0.0f, dbLenStart);
				}

				if (bEnd)
					ipSecondP = mapPoints[convert(dbEnd, esriKilometers, esriMeters)];

				double firstP = convert(dbFirst, esriKilometers, esriMeters);
				double endP = convert(dbEnd, esriKilometers, esriMeters);

				// �����W�|�C���g�Ԃ̓��H�̋������擾
				double dbLen = split_length(ipAllPolyline, ipFirstP, ipSecondP);

				// �n�_�`�w��|�C���g�܂��͎w��|�C���g�`�I�_�̏��� [bug 12017]
				if (bFirst || bEnd) {
					// �w��s�b�`�ŋ����W�|�C���g�𐶐�����
					createDistancePoint(ipAllPolyline, firstP, endP, dbPitch, dbLen, 0, mapPoints, dbLenStart, dbLenEnd, (bFirst || bEnd));
					continue;
				}

				// ���������W�|�C���g���␳�_���ǂ��� [bug 12017]
				if (distanceDiff == correctionPointLen) {
					// �␳�_�Ԃ�10�������鋗���W�|�C���g�𐶐�
					createDistancePoint(ipAllPolyline, firstP, endP, dbPitch, dbLen, 10, mapPoints, dbLenStart, dbLenEnd, (bFirst || bEnd));
				}
				else {
					// �P�ʍ��킹����Ă���A�܂��̓|�C���g�Ԃ��w��s�b�`�ȏ゠��ꍇ
					if (pitchAdjust || distanceDiff >= dbPitch) {
						// �w��s�b�`�ŋ����W�|�C���g�𐶐�����
						createDistancePoint(ipAllPolyline, firstP, endP, dbPitch, dbLen, 0, mapPoints, dbLenStart, dbLenEnd, true);
					}
					else continue; // �|�C���g�Ԃ��w��s�b�`��菬�������������W�|�C���g�Ԃɂ͋����W�|�C���g�𔭐������Ȃ� [bug 12017]
				}
			}
		}
		cLinkTracer.toStartSelectLinkSection();
		return true;
	}
}
