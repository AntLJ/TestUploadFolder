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
	// 処理DB接続
	bool CCreateDistancePost::Connect(LPCTSTR lpcszConnection)
	{
		m_cWorkspace = sindy::create_workspace( lpcszConnection );

		return (IWorkspace*)m_cWorkspace ? true : false;
	}

	// Distance_Post_Point取得
	bool CCreateDistancePost::OpenDistancePostPoint()
	{
		if ( m_cWorkspace )
		{
			m_cDistancePostClass = m_cWorkspace.OpenTable(sindy::schema::distance_post_point::kTableName);
			m_cDistancePostClass.CreateFieldMap();
		}

		return (IFeatureClassPtr)m_cDistancePostClass ? true : false;
	}

	// ROAD_LINK/ROAD_NODE取得
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

	// メッシュ取得
	bool CCreateDistancePost::OpenMesh()
	{
		if ( m_cWorkspace )
			m_cBaseMeshClass = m_cWorkspace.OpenTable(sindy::schema::basemesh::kTableName);

		return (IFeatureClassPtr)m_cBaseMeshClass ? true : false;
	}

	// 路線リスト確認
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

			// 行数確認
			if ( nLineCount <= 0 ) 
				return -2;

			CSPFieldMap pSpFieldMap = m_cDistancePostClass.GetFieldMap();
			long nIdx = pSpFieldMap->FindField( kDistance );
			for ( const auto& item : m_mapRoadList )
			{
				CString strWhere;
				strWhere.Format(_T("%s = %d and %s = %d and %s "), kRoadCode, item.second.get<0>(), kDirection, item.second.get<1>(), kDistanceMark);

				// 距離標記号が指定されていなければNULLで、指定されていればその記号で検索(bug12125)
				strWhere += (item.second.get<2>().IsEmpty()) ? _T("is null") : uh::str_util::format(_T("= '%s'"), item.second.get<2>());

				CFeatureClass::Container cContainer = m_cDistancePostClass.searchNoContain( AheInitQueryFilter(nullptr, nullptr, strWhere) );
				if ( cContainer.size() < 2 )
				{
					nErr = -6;
					mapErr[item.first] = nErr;
					continue;
				}

				// 距離標値の昇順ソート
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

				// ポイント間の最小最大距離標値がピッチ以下か確認
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

	// 距離標ポイント検索
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

	// 距離標ポイント群取得
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

	// 距離標ポイント取得
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

	// 距離標ポイント検索
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

	// 道路検索
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

	// 道路検索用形状取得
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

	// 道路除外処理
	void CCreateDistancePost::excludeRoad(CFeatureClass::Container& cRoads, const std::list<long>& listNotExcludeRoads, const std::list<long>& listRouteIDs)
	{
		// 端点ノードが接続している道路は除外
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

		// ノード検索
		std::list<CString> strIDs;
		CTableBase((IFeatureClassPtr)m_cRoadNodeClass).IDs2Str(listNodeIDs, strIDs);

		for ( const auto& strID : strIDs )
		{
			CString strWhere;
			strWhere.Format( _T("%s in (%s) and %s = %d"), m_cRoadNodeClass.getOIDFieldName(), strID, sindy::schema::road_node::kNodeClass, sindy::schema::road_node::node_class::kTerminal );
			m_cRoadNodeClass.search( AheInitQueryFilter(nullptr, nullptr, strWhere) );
		}

		// 端点ノードに接続している道路を除外
		std::list<long> listExcludeRoads;
		CFeatureClass::Container cContainer = m_cRoadNodeClass.getContainer();

		for ( const auto& cRoadNode : mapRoadNodes )
		{
			if ( cContainer.end() != cContainer.find( _tcstol(cRoadNode.second.first, 0, 10) ) ||
				 cContainer.end() != cContainer.find( _tcstol(cRoadNode.second.second, 0, 10) ) )
			{
				// 除外してはいけないリストになければ除外
				if ( listNotExcludeRoads.end() == std::find(listNotExcludeRoads.begin(), listNotExcludeRoads.end(), cRoadNode.first) &&
					 listExcludeRoads.end() == std::find(listExcludeRoads.begin(), listExcludeRoads.end(), cRoadNode.first) )
					// 除外
					listExcludeRoads.push_back( cRoadNode.first );
			}
		}

		// 経路探索済みの道路を除外する
		for ( const auto& cRoadNode : mapRoadNodes )
		{
			if ( listRouteIDs.end() != std::find(listRouteIDs.begin(), listRouteIDs.end(), cRoadNode.first) && 
				 listNotExcludeRoads.end() == std::find(listNotExcludeRoads.begin(), listNotExcludeRoads.end(), cRoadNode.first) &&
				 listExcludeRoads.end() == std::find(listExcludeRoads.begin(), listExcludeRoads.end(), cRoadNode.first) )
				// 除外
				listExcludeRoads.push_back( cRoadNode.first );
		}

		// 除外した道路に接続する道路が1本しかない時は、その道路も除外する
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
					// 除外
					listExcludeRoads.push_back( listConnRoads.front() );
			}
		}

		// コンテナから削除
		CFeatureClass::Container::iterator it = cRoads.begin();
		while (it != cRoads.end())
		{
			if ( listExcludeRoads.end() != std::find(listExcludeRoads.begin(), listExcludeRoads.end(), it->first) )
				cRoads.erase(it++);
			else
				++it;
		}
	}

	// 距離標ポイント形状作成
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

			// ピッチ単位で分割するかどうか
			if ( bPitch )
			{
				// 単位合わせした点と補正点時のみ距離を10等分ではなく、ピッチ単位で作成する
				dbPointVal = dbPitchTmp;
				dbVal = dbPitchTmp;
			}
			else
			{
				// ポイントの位置は距離を10等分した位置で作成
				dbPointVal = dbLenTmp / nSplitNum;

				// 距離標値は補正点間の距離標値を10等分した値を入れる
				dbVal = (dbEnd - dbFirst) / nSplitNum;
			}

			for ( long i = 1; i < nSplitNum; ++i )
			{
				CString strTmp( convertStr(dbFirst + dbVal * i, 0) );

				// ピッチ単位で分割時はdbEndを超えたら終わるようにする
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
			// 距離で分割していく
			for ( double dbVal = dbPitchTmp; dbVal < dbLineLen - (dbLenStart+dbLenEnd); dbVal += dbPitchTmp)
			{
				CString strTmp( convertStr(dbFirst + dbVal, 0) );
				mapPoints[_tcstod(strTmp, nullptr)] = split_distance(ipPolyline, dbLenStart + dbVal);
			}
		}
		else
		{
			// 指定された距離で1ポイントだけ作成
			CString strTmp( convertStr(dbEnd, 0) );
			mapPoints[_tcstod(strTmp, nullptr)] = split_distance(ipPolyline, dbFirst);
		}

		return true;
	}

	// 距離(km)取得
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

	// 分割点取得（距離）
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

	// 分割点取得（ポイント）	
	IPointPtr CCreateDistancePost::split_point(const IPolylinePtr& ipPolyline, const IPointPtr& ipPoint, double dbVal, double& dbLen)
	{
		VARIANT_BOOL vaSplitHappened = VARIANT_FALSE;
		long lNewPartIndex = -1, lNewSegmentIndex = -1;

		ipPolyline->SplitAtPoint( ipPoint, VARIANT_FALSE, VARIANT_FALSE, &vaSplitHappened, &lNewPartIndex, &lNewSegmentIndex );

		// 分割点までの距離取得
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

	// 編集開始
	void CCreateDistancePost::startEdit()
	{
		m_cWorkspace.StartEditing(false);
		m_cWorkspace.StartEditOperation();
	}

	// 編集終了
	void CCreateDistancePost::stopEdit()
	{
		m_cWorkspace.StopEditOperation();
		m_cWorkspace.StopEditing(true);
	}

	// 編集破棄
	void CCreateDistancePost::abortEdit()
	{
		m_cWorkspace.AbortEditOperation();
	}

	// 距離標ポイント作成
	CSPRowBase CCreateDistancePost::createDistancePoint(const IPointPtr& ipPoint, double dbVal, long nLineNo)
	{
		CSPFieldMap pSpFieldMap =  m_cDistancePostClass.GetFieldMap();
		long nIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kDistance );
		long nRIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kRoadCode );
		long nDIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kDirection );
		long nMIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kDistanceMark );
		CSPRowBase pRow = m_cDistancePostClass.Create();

		if ( ipPoint )
			ipPoint->Project(m_cDistancePostClass.getSpRef());	// 元の座標系に戻す

		pRow->SetShape( ipPoint );
		pRow->SetValue( nIdx, dbVal );
		pRow->SetValue( nRIdx, m_mapRoadList[nLineNo].get<0>() );
		pRow->SetValue( nDIdx, m_mapRoadList[nLineNo].get<1>() );
		// FGDBでこのフィールドにNULLを入れようとすると空文字が入るため、NULLを入れる場合はSetValueしない
		if (m_mapRoadList[nLineNo].get<2>() != "") {
			pRow->SetValue(nMIdx, m_mapRoadList[nLineNo].get<2>());
		}

		sindy::CErrorObjectsBase cErrs;
		pRow->Store(cErrs);

		return pRow;
	}

	// 単位変換
	double CCreateDistancePost::convert(double dbValue, esriUnits esriSrc, esriUnits esriDest)
	{
		IUnitConverterPtr ipUnit( CLSID_UnitConverter );
		double dbRet = 0.0f;

		atl2::valid(ipUnit->ConvertUnits( dbValue, esriSrc, esriDest, &dbRet ));
		return dbRet;
	}

	// コストファイル読み込み(道路種別)
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

				if ( vecAttr.size() != 2 )// 2列以外は読み込まない
					continue;

				if ( vecAttr.at(0).GetLength() <= 0 || vecAttr.at(1).GetLength() <= 0 )
					continue;

				LinkCostList::m_cRoadClassCostMap[_tcstol(vecAttr.at(0), 0, 10)] = _tcstol(vecAttr.at(1), 0, 10);

				++nLineCount;
			}
		}
	}

	// コストファイル読み込み(リンク種別)
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

				if ( vecAttr.size() != 2 )	// 2列以外は読み込まない
					continue;

				if ( vecAttr.at(0).GetLength() <= 0 || vecAttr.at(1).GetLength() <= 0 )
					continue;

				LinkCostList::m_cLinkClassCostMap[_tcstol(vecAttr.at(0), 0, 10)] = _tcstol(vecAttr.at(1), 0, 10);

				++nLineCount;
			}
		}
	}

	// フィールド番号取得
	long CCreateDistancePost::GetFieldIndex(LPCTSTR lpcszField)
	{
		CSPFieldMap pSpFieldMap = m_cDistancePostClass.GetFieldMap();
		return pSpFieldMap->FindField( lpcszField );
	}

	// COMオブジェクトのコピーを作成する
	IClonePtr CCreateDistancePost::Clone( const IUnknownPtr& ipUnk )
	{
		IClonePtr ipClone( ipUnk ), ipRet;

		if( ! ipClone )
			return nullptr;

		atl2::valid(ipClone->Clone( &ipRet ));

		return ipRet;
	}

	// 道路検索（OID指定）
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

	// 分割点までの距離取得
	double CCreateDistancePost::split_point_length(const IPolylinePtr& ipPolyline, const IPointPtr& ipPoint)
	{
		VARIANT_BOOL vaSplitHappened = VARIANT_FALSE;
		long lNewPartIndex = -1, lNewSegmentIndex = -1;

		ipPolyline->SplitAtPoint( ipPoint, VARIANT_FALSE, VARIANT_FALSE, &vaSplitHappened, &lNewPartIndex, &lNewSegmentIndex );

		// 分割点までの距離取得
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

	// 分割した2点間の距離を取得
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

	// 切り詰め
	CString CCreateDistancePost::convertStr(double dbVal, long nLen)
	{
		double dbTmp = dbVal * pow(10.0f, nLen);

		dbTmp = (double)(LONGLONG)(dbTmp + 0.9f);
    
		CString strTmp;
		strTmp.Format( _T("%.1lf"), dbTmp * pow(10.0f, -nLen) );

		return strTmp;
	}

	// 生成する距離標ポイントを取得する
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
		// 距離標値順に処理
		sindy::LinkTracer cLinkTracer;

		cLinkTracer.toStartSelectLinkSection();

		std::list<long> listNotExcludeRoads;	// 除外してはいけない道路（距離標ポイントが乗っている道路）
		std::list<long> listExcludeMesh;
		sindy::CFeatureClass::Container cRoads;
		std::list<long> listRouteIDs;
		IPointPtr ipFirstRoadFromPoint;
		double dbLenStart = 0.0f, dbLenEnd = 0.0f;

		for (std::list<double>::const_iterator it = existDistanceList.begin(); it != existDistanceList.end(); ++it)
		{
			double dbFirst = 0.0f, dbEnd = 0.0f, dbFirstTmp = 0.0f, dbEndTmp = 0.0f;
			bool bFirst = false, bEnd = false;

			// 始点
			sindy::CSPRowBase pFirstRow = mapRows.at(*it);

			if (existDistanceList.begin() == it)
			{
				bFirst = true;

				// 単位合わせ
				dbFirstTmp = (*it);

				// 1kmピッチ以上の時にのみ単位合わせを行う
				if (dbPitch >= 1.0f)
					dbFirst = ((*it) - fmod((*it), dbPitch)) + dbPitch;
				else
					dbFirst = (*it);
			}
			else
			{
				dbFirst = (*it);
			}

			// 次点
			++it;
			if (existDistanceList.end() == it)	// 終点は何もしない
			{
				break;
			}
			else
			{
				++it;
				// 単位合わせ
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

			// 始点取得
			std::list<long> listFirstRoads = searchDistancePostOnRoad(pFirstRow->GetShapeCopy(), dbBuff);

			if (listFirstRoads.empty())
			{
				errorMsg.Format(_T("ERROR:<%ld>が道路リンク上に設定されていません。"), pFirstRow->GetOID());
				return false;
			}

			if (listFirstRoads.size() > 2) {
				errorMsg.Format(_T("ERROR:<%ld>が3本以上の道路リンクに乗っています。"), pFirstRow->GetOID());
				return false;
			}

			for (const auto& cFrist : listFirstRoads)
				listNotExcludeRoads.push_back(cFrist);

			// 次点取得
			std::list<long> listSecondRoads = searchDistancePostOnRoad(pSecondRow->GetShapeCopy(), dbBuff);

			if (listSecondRoads.empty())
			{
				errorMsg.Format(_T("ERROR:<%ld>が道路リンク上に設定されていません。"), pSecondRow->GetOID());
				return false;
			}

			if (listSecondRoads.size() > 2) {
				errorMsg.Format(_T("ERROR:<%ld>が3本以上の道路リンクに乗っています。"), pSecondRow->GetOID());
				return false;
			}

			for (const auto& cSecond : listSecondRoads)
				listNotExcludeRoads.push_back(cSecond);

			// 道路検索
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

			// 道路除外
			excludeRoad(cRoads, listNotExcludeRoads, listRouteIDs);

			// 次点が乗っている道路があるか確認
			bool bFind = false;
			for (const auto& cRoad : cRoads)
			{
				if (listSecondRoads.end() != std::find(listSecondRoads.begin(), listSecondRoads.end(), cRoad.first))
				{
					bFind = true;
					break;
				}
			}

			// ない時は再検索
			if (!bFind)
			{
				++nBase;
				cRoadsTmp = cRoads;
				goto search_road;
			}

			// 経路探索開始
			cLinkTracer.toStartSelectLinkSection();
			cLinkTracer.init_cache(getRoadLink(), dbBuff);

			// 経路探索クラスに道路を渡す
			for (const auto& cRoad : cRoads)
				cLinkTracer.add_cache(cRoad.first, (_IRow*)(*cRoad.second.get()));
			cLinkTracer.cache();

			// 経路探索
			cLinkTracer.onNormalMode(pFirstRow->GetShapeCopy());
			cLinkTracer.onNormalMode(pSecondRow->GetShapeCopy());

			// 経路探索形状取得
			IPolylinePtr ipPolyline = cLinkTracer.getRouteGeom();
			if (!ipPolyline)
			{
				errorMsg.Format(_T("ERROR:経路計算に失敗しました。<%ld>または<%ld>が道路リンク上に設定されていない可能性があります。"), pFirstRow->GetOID(), pSecondRow->GetOID());
				return false;
			}

			for (const auto& cRoute : cLinkTracer.getRouteIDs())
			{
				if (listRouteIDs.end() == std::find(listRouteIDs.begin(), listRouteIDs.end(), cRoute))
					listRouteIDs.push_back(cRoute);
			}

			// クローン取得
			ipPolyline = Clone(ipPolyline);

			// 座標系を変更
			ipPolyline->Project(ipPrjCoordinateSystem);

			IPointPtr ipFirstP(pFirstRow->GetShapeCopy());
			IPointPtr ipSecondP(pSecondRow->GetShapeCopy());

			ipFirstP->Project(ipPrjCoordinateSystem);	// 座標系を変更
			ipSecondP->Project(ipPrjCoordinateSystem);	// 座標系を変更

			if (!ipFirstRoadFromPoint)
			{
				// 経路探索した道路数が1本の時に、形状の向きが道路の向きになっている事があるので
				// 向きを距離標ポイントが置かれている方向にあわせる
				double dbFLen = split_point_length(Clone(ipPolyline), ipFirstP);
				double dbSLen = split_point_length(Clone(ipPolyline), ipSecondP);

				// 次点とラインの始点までの距離が起点とラインの始点までの距離よりも近いなら反転
				if (dbFLen > dbSLen)
					ipPolyline->ReverseOrientation();

				ipFirstRoadFromPoint = AheGetFromPoint(ipPolyline);
				ipFirstRoadFromPoint->Project(ipPrjCoordinateSystem);	// 座標系を変更
			}

			IPointPtr ipToP;
			bool bF = true;
			sindy::CFeatureClass::Container cRoads2 = searchRoads(listRouteIDs);
			for (const auto& cRoute : listRouteIDs)
			{
				IPolylinePtr ipPoly(cRoads2[cRoute]->GetShapeCopy());
				ipPoly->Project(ipPrjCoordinateSystem);	// 座標系を変更

				IPointPtr ipF(AheGetFromPoint(ipPoly));
				ipF->Project(ipPrjCoordinateSystem);	// 座標系を変更

				if (bF)
				{
					ipPoly = Clone(ipPoly);
					ipPoly->Project(ipPrjCoordinateSystem);	// 座標系を変更

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
				ipToP->Project(ipPrjCoordinateSystem);	// 座標系を変更
			}
			ipAllPolyline->Project(ipPrjCoordinateSystem);	// 座標系を変更

															// 起点又は終点一つ前の補正点時は、単位合わせしたポイントを作成
			if (bFirst)
			{
				mapPoints[convert(dbFirst, esriKilometers, esriMeters)] = split_point(ipAllPolyline, ipFirstP, dbFirstTmp != 0.0f ? dbFirst - dbFirstTmp : dbFirst, dbLenStart);
			}

			if (bEnd)
			{
				ipAllPolyline->ReverseOrientation();
				mapPoints[convert(dbEnd, esriKilometers, esriMeters)] = split_point(ipAllPolyline, ipSecondP, dbEndTmp != 0.0f ? dbEndTmp - dbEnd : 0.0f, dbLenEnd);
				ipAllPolyline->ReverseOrientation();	// 元に戻す
			}

			// 距離標ポイント間の距離標値の差を取得 [bug 12017]
			double distanceDiff = dbEnd - dbFirst;

			bool pitchAdjust = false; // 単位合わせしたかどうか [bug 12017]

			// 指定ピッチが1km以上の時に、既存距離標ポイントの距離標値が指定ピッチで割り切れない場合、単位合わせを行う [bug 12017]
			if (!bFirst && distanceDiff >= dbPitch && dbPitch >= 1.0f && fmod(dbFirst, dbPitch) != 0)
			{
				// 単位合わせ
				dbFirstTmp = dbFirst;
				dbFirst = ((dbFirst)-fmod(dbFirst, dbPitch)) + dbPitch;
				pitchAdjust = true;

				// 単位合わせしたポイントを作成
				double mapPointskey = convert(dbFirst, esriKilometers, esriMeters);
				double splitVal = dbFirst - dbFirstTmp;
				mapPoints[mapPointskey] = split_point(ipAllPolyline, ipFirstP, splitVal, dbLenStart);
			}

			// 指定ポイント間の処理
			if (2 == existDistanceList.size())
			{
				// 起点/終点のみの場合
				createDistancePoint(ipAllPolyline, convert(dbFirst, esriKilometers, esriMeters), convert(dbEnd, esriKilometers, esriMeters), dbPitch, dbPitch, 0, mapPoints, dbLenStart, dbLenEnd, false);
			}
			else
			{
				// 指定ポイントが3点以上の場合(補正点または既存距離標ポイントあり)
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

				// 距離標ポイント間の道路の距離を取得
				double dbLen = split_length(ipAllPolyline, ipFirstP, ipSecondP);

				// 始点～指定ポイントまたは指定ポイント～終点の処理 [bug 12017]
				if (bFirst || bEnd) {
					// 指定ピッチで距離標ポイントを生成する
					createDistancePoint(ipAllPolyline, firstP, endP, dbPitch, dbLen, 0, mapPoints, dbLenStart, dbLenEnd, (bFirst || bEnd));
					continue;
				}

				// 既存距離標ポイントが補正点かどうか [bug 12017]
				if (distanceDiff == correctionPointLen) {
					// 補正点間を10分割する距離標ポイントを生成
					createDistancePoint(ipAllPolyline, firstP, endP, dbPitch, dbLen, 10, mapPoints, dbLenStart, dbLenEnd, (bFirst || bEnd));
				}
				else {
					// 単位合わせされている、またはポイント間が指定ピッチ以上ある場合
					if (pitchAdjust || distanceDiff >= dbPitch) {
						// 指定ピッチで距離標ポイントを生成する
						createDistancePoint(ipAllPolyline, firstP, endP, dbPitch, dbLen, 0, mapPoints, dbLenStart, dbLenEnd, true);
					}
					else continue; // ポイント間が指定ピッチより小さい既存距離標ポイント間には距離標ポイントを発生させない [bug 12017]
				}
			}
		}
		cLinkTracer.toStartSelectLinkSection();
		return true;
	}
}
