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
#include <TableBase.h>
#include <ErrorObject2.h>
#include "tracer/LinkCostList.h"
#include <boost/tuple/tuple.hpp>
#include "CCheckDistancePost.h"
#include "ErrorType.h"
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
	bool CCheckDistancePost::Connect(LPCTSTR lpcszConnection)
	{
		m_cWorkspace = sindy::create_workspace( lpcszConnection );

		return (IWorkspace*)m_cWorkspace ? true : false;
	}

	// Distance_Post_Point取得
	bool CCheckDistancePost::OpenDistancePostPoint()
	{
		if ( m_cWorkspace )
		{
			m_cDistancePostClass = m_cWorkspace.OpenTable(sindy::schema::distance_post_point::kTableName);
			m_cDistancePostClass.CreateFieldMap();
		}

		return (IFeatureClassPtr)m_cDistancePostClass ? true : false;
	}

	// ROAD_LINK/ROAD_NODE取得
	bool CCheckDistancePost::OpenNetwork()
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
	bool CCheckDistancePost::OpenMesh()
	{
		if ( m_cWorkspace )
			m_cBaseMeshClass = m_cWorkspace.OpenTable(sindy::schema::basemesh::kTableName);

		return (IFeatureClassPtr)m_cBaseMeshClass ? true : false;
	}

	// 路線リスト確認
	long CCheckDistancePost::CheckRoadList(LPCTSTR lpcszFile, double dbPitch, std::map<long, std::pair<std::list<double>, std::map<double, long>>>& mapContainer, std::map<long,long>& mapErr)
	{
		using namespace sindy::schema::distance_post_point;
		using namespace err_type;

		std::ifstream aFin(lpcszFile);
		if (aFin)
		{
			long nErr = no_problem;
			std::string aRecord;
			long nLineCount = 0;
			while (std::getline(aFin, aRecord))
			{
				mapErr[nLineCount] = no_problem;

				vector<CString> vecAttr = uh::str_util::split( uh::toTStr(aRecord).c_str(), _T("\t") );

				if ( vecAttr.size() <= 1 || vecAttr.size() > 3 )
				{
					nErr = less_column;
					mapErr[nLineCount] = nErr;
					++nLineCount;
					continue;
				}

				if ( vecAttr.at(0).GetLength() <= 0 )
				{
					nErr = no_roadcode;
					mapErr[nLineCount] = nErr;
					++nLineCount;
					continue;
				}

				if ( vecAttr.at(1).GetLength() <= 0 )
				{
					nErr = no_directioncode;
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
				return no_line_roadlist;

			CSPFieldMap pSpFieldMap = m_cDistancePostClass.GetFieldMap();
			long nIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kDistance );
			for ( const auto& item : m_mapRoadList )
			{
				CString strWhere;
				strWhere.Format(_T("%s = %d and %s = %d and "), kRoadCode, item.second.get<0>(), kDirection, item.second.get<1>());

				// 距離標記号が指定されていなければNULLで、指定されていればその記号で検索 [bug12154]
				strWhere += (item.second.get<2>().IsEmpty()) ? 
					uh::str_util::format(_T("%s is null"), kDistanceMark) : uh::str_util::format(_T("%s = '%s'"), kDistanceMark, item.second.get<2>());

				CFeatureClass::Container cContainer = m_cDistancePostClass.searchNoContain( AheInitQueryFilter(nullptr, nullptr, strWhere) );
				if ( cContainer.size() < 2 )
				{
					nErr = less_point;
					mapErr[item.first] = nErr;
					continue;
				}

				// 距離標値の昇順ソート
				std::list<double> listDistances;
				std::map<double, long> mapOIDs;
				for ( const auto& cRow : cContainer )
				{
					CComVariant vaVal( cRow.second->GetValue(nIdx) );
					CString strTmp;
					strTmp.Format( _T("%.1lf"), vaVal.fltVal );

					listDistances.push_back(_tcstod(strTmp, nullptr));

					mapOIDs[vaVal.fltVal] = cRow.second->GetOID();
				}

				listDistances.sort();

				// ポイント間の最小最大距離標値がピッチ以下か確認
				if ( listDistances.back() - listDistances.front() <= dbPitch )
				{
					nErr = short_distance;
					mapErr[item.first] = nErr;
					continue;
				}

				mapContainer[item.first] = std::pair<std::list<double>, std::map<double, long>>(listDistances, mapOIDs);
			}

			return nErr;
		}

		return failed_load;
	}

	// 距離標ポイント検索
	void CCheckDistancePost::searchDistancePoint(const std::map<double, long>& mapValues)
	{
		std::list<CString> listIDs; // OID群

		for ( const auto& cVal : mapValues )
		{
			CString strTmp;
			strTmp.Format( _T("%d"), cVal.second );
			listIDs.push_back( strTmp );
		}

		std::list<CString> strIDs; // 検索用OID群の文字列
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
	std::map<double, CSPRowBase> CCheckDistancePost::findDistancePoints()
	{
		CFeatureClass::Container cContainer = m_cDistancePostClass.getContainer();
		CSPFieldMap pSpFieldMap =  m_cDistancePostClass.GetFieldMap();
		long nIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kDistance );
		std::map<double, CSPRowBase> mapRows;

		for ( const auto& it : cContainer )
		{
			CComVariant vaVal( it.second->GetValue(nIdx) );
			CString strTmp;
			strTmp.Format( _T("%.1f"), vaVal.fltVal );

			mapRows[_tcstod(strTmp, nullptr)] = it.second;
		}

		return mapRows;
	}

	// 距離標ポイント取得
	CSPRowBase CCheckDistancePost::findDistancePoint(double dbVal)
	{
		CFeatureClass::Container cContainer = m_cDistancePostClass.getContainer();
		CSPFieldMap pSpFieldMap =  m_cDistancePostClass.GetFieldMap();
		long nIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kDistance );
		CSPRowBase pRow;

		for ( const auto& it : cContainer )
		{
			CComVariant vaVal( it.second->GetValue(nIdx) );
			CString strTmp, strTmp2;
			strTmp.Format( _T("%.1lf"), vaVal.fltVal );
			strTmp2.Format( _T("%.1lf"), dbVal );

			if ( _tcstod(strTmp, nullptr) == _tcstod(strTmp2, nullptr) )
			{
				pRow = it.second;
				break;
			}
		}

		return pRow;
	}

	// 距離標ポイント群形状取得
	IGeometryPtr CCheckDistancePost::createDistancePointGeom()
	{
		IGeometryCollectionPtr ipGeoms( CLSID_GeometryBag );
		CFeatureClass::Container cContainer = m_cDistancePostClass.getContainer();

		for ( const auto& it : cContainer )
			ipGeoms->AddGeometry( it.second->GetShapeCopy() );

		return ipGeoms;
	}

	// 単位変換
	double CCheckDistancePost::convert(double dbValue, esriUnits esriSrc, esriUnits esriDest)
	{
		IUnitConverterPtr ipUnit( CLSID_UnitConverter );
		double dbRet = 0.0f;

		atl2::valid(ipUnit->ConvertUnits( dbValue, esriSrc, esriDest, &dbRet ));
		return dbRet;
	}

	// 道路検索
	CFeatureClass::Container CCheckDistancePost::searchRoads(const IGeometryPtr& ipGeom, double dbBuf, const long& roadCode /*= -1*/)
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

		// 路線コードが渡されている場合には路線コードも検索条件に含める [bug 12155]
		if (roadCode != -1)
		{
			CString strTmp;
			strTmp.Format(_T("%s = %d"), sindy::schema::road_link::kRoadCode, roadCode);
			if (strWhere.GetLength() > 0)
				strWhere += _T(" and ");
			strWhere += strTmp;
		}

		sindy::CGeometryBase cGeom(ipGeom);
		IGeometryPtr ipBufferGeom( cGeom.Buffer( convert(dbBuf, esriMeters, esriDecimalDegrees) ) );
		return m_cRoadLinkClass.searchNoContain( AheInitQueryFilter(AheInitSpatialFilter(nullptr, ipBufferGeom, m_cRoadLinkClass.getShapeFieldName()), nullptr, strWhere) );
	}

	// 道路検索用形状取得
	IGeometryPtr CCheckDistancePost::searchRoadGeoms(const CFeatureClass::Container& cRoads, long nBase, const std::list<long>& listExcludeMesh, std::list<long>& listMesh)
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
	void CCheckDistancePost::excludeRoad(CFeatureClass::Container& cRoads, const std::list<long>& listNotExcludeRoads, const std::list<long>& listRouteIDs)
	{
		// 端点ノードが接続している道路は除外
		CSPFieldMap pSpFieldMap =  m_cRoadLinkClass.GetFieldMap();
		long nFromIdx = pSpFieldMap->FindField( sindy::schema::road_link::kFromNodeID );
		long nToIdx = pSpFieldMap->FindField( sindy::schema::road_link::kToNodeID );
		std::list<CString> listNodeIDs;

		// map<OID, FromNodeID, ToNodeID>
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

	// 距離(km)取得
	double CCheckDistancePost::GetLength(const IPolylinePtr& ipPolyline)
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

	// コストファイル読み込み(道路種別)
	void CCheckDistancePost::ReadRoadCost(LPCTSTR lpcszFile)
	{
		std::ifstream aFin(lpcszFile);
		if (aFin)
		{
			std::string aRecord;
			long nLineCount = 0;
			while (std::getline(aFin, aRecord))
			{
				vector<CString> vecAttr = uh::str_util::split( uh::toTStr(aRecord).c_str(), _T("\t") );

				if ( vecAttr.size() != 2 )
					continue;

				if ( vecAttr.at(0).GetLength() <= 0 )
					continue;

				if ( vecAttr.at(1).GetLength() <= 0 )
					continue;

				LinkCostList::m_cRoadClassCostMap[_tcstol(vecAttr.at(0), 0, 10)] = _tcstol(vecAttr.at(1), 0, 10);

				++nLineCount;
			}
		}
	}

	// コストファイル読み込み(リンク種別)
	void CCheckDistancePost::ReadLinkCost(LPCTSTR lpcszFile)
	{
		std::ifstream aFin(lpcszFile);
		if (aFin)
		{
			std::string aRecord;
			long nLineCount = 0;
			while (std::getline(aFin, aRecord))
			{
				vector<CString> vecAttr = uh::str_util::split( uh::toTStr(aRecord).c_str(), _T("\t") );

				if ( vecAttr.size() != 2 )
					continue;

				if ( vecAttr.at(0).GetLength() <= 0 )
					continue;

				if ( vecAttr.at(1).GetLength() <= 0 )
					continue;

				LinkCostList::m_cLinkClassCostMap[_tcstol(vecAttr.at(0), 0, 10)] = _tcstol(vecAttr.at(1), 0, 10);

				++nLineCount;
			}
		}
	}

	// 距離標値間の距離と距離標ポイント間の距離をチェック [bug12155]
	bool CCheckDistancePost::checkRoadLen(const double& pointLen, const double& distanceDiff, const double& pitch, const double& range)
	{
		// 単位変換
		double convDistanceDiff = convert(distanceDiff, esriKilometers, esriMeters);
		double convPitch = convert(pitch, esriKilometers, esriMeters);

		// 比較
		double pointDiff = (abs(convPitch - convDistanceDiff) / convPitch) * 100;
		double roadDiff  = (abs(convPitch - pointLen) / convPitch) * 100;

		// 差異が許容値以上か
		if (pointDiff >= range || roadDiff >= range)
		{
			return false;
		}

		return true;
	}

	// COMオブジェクトのコピーを作成する
	IClonePtr CCheckDistancePost::Clone( const IUnknownPtr& ipUnk )
	{
		IClonePtr ipClone( ipUnk ), ipRet;

		if( ! ipClone )
			return nullptr;

		atl2::valid(ipClone->Clone( &ipRet ));

		return ipRet;
	}

	// 切り詰め
	CString CCheckDistancePost::convertStr(double dbVal, long nLen)
	{
		double dbTmp = dbVal * pow(10.0f, nLen);

		dbTmp = (double)(LONGLONG)(dbTmp + 0.9f);
    
		CString strTmp;
		strTmp.Format( _T("%.1lf"), dbTmp * pow(10.0f, -nLen) );

		return strTmp;
	}

	// 分割点までの距離取得 [bug12155]
	double CCheckDistancePost::split_point_length(const IPolylinePtr& ipPolyline, const IPointPtr& ipPoint)
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

	// 道路検索（OID指定）[bug12155]
	CFeatureClass::Container CCheckDistancePost::searchRoadsByOID(const std::list<long>& listRouteIDs)
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

	// 対象路線上の道路であるかチェック [bug12155]
	bool CCheckDistancePost::checkOnHWY(const CFeatureClass::Container& roadContainer, const long& roadCode, long roadCodeIdx)
	{
		for (const auto& road : roadContainer)
		{
			CComVariant roadCodeVal = road.second->GetValue(roadCodeIdx);
			if (roadCode == roadCodeVal.lVal)
			{
				return true;
			}
		}
		return false;
	}

	// 経路形状を取得する [bug12155]
	void CCheckDistancePost::getRoutePolyline( IPolylinePtr& routePolyline, const CFeatureClass::Container& roadContainer, const list<long>& routeIDs, const IProjectedCoordinateSystemPtr& ipPrjCoordinateSystem )
	{
		for( const auto& routeID : routeIDs )
		{
			// 全体形状のtoを取得
			IPointPtr toPoint( CLSID_Point );
			toPoint = AheGetToPoint( routePolyline );
			toPoint->Project( ipPrjCoordinateSystem ); // 座標系を変更

			// リンク形状取得
			IPolylinePtr linkPolyline( CLSID_Polyline );
			for( const auto& road : roadContainer ){
				if( road.first == routeID ){
					linkPolyline = road.second->GetShapeCopy();
					break;
				}
			}
			linkPolyline->Project( ipPrjCoordinateSystem ); // 座標系を変更

			// リンク形状のfrom点取得
			IPointPtr linkFromPoint( CLSID_Point );
			linkFromPoint = AheGetFromPoint( linkPolyline );
			linkFromPoint->Project( ipPrjCoordinateSystem ); // 座標系を変更

			// 形状の向きをroutePolylineに合わせる
			if( !AheIsEqual( toPoint, linkFromPoint ) )
			{
				linkPolyline->ReverseOrientation();
			}

			IPointCollectionPtr ipRoutePointColl( routePolyline );
			IPointCollectionPtr ipLinkPointColl( linkPolyline );

			ipLinkPointColl->RemovePoints( 0, 1 );
			ipRoutePointColl->AddPointCollection( ipLinkPointColl );
		}
	}

	// ポリラインに乗っているポイント間の距離を取得する [bug12155]
	double CCheckDistancePost::getPointLen( const IPolylinePtr& polyline, const IPointPtr& point1, const IPointPtr& point2 )
	{
		double toPointLen1 = split_point_length( polyline, point1 );
		double toPointLen2 = split_point_length( polyline, point2 );

		double pointLen = toPointLen2 - toPointLen1;
		return pointLen;
	}
}
