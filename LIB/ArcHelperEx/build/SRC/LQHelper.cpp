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
#include "TargetFeatures.h"
#include "LQHelper.h"
#include "WaitCursor.h"
#include "Mesh.h"
#include "AheGlobalsX.h"
#include "AheGeometryConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR ERROR_LQ_RESTRICTION_CHECK           = _T("リンク列規制チェックエラー"); 
	LPCTSTR CONFIRM_CONTINUE                     = _T("\n処理を続けますか？"); 
	LPCTSTR CHECK_LQ_RESTRICTION                 = _T("リンク列規制チェック"); 
	LPCTSTR UNCONDITIONAL_PASS                   = _T("条件なし・通過"); 
	LPCTSTR UNCONDITIONAL_BACKWARD               = _T("条件なし・逆送"); 
	LPCTSTR CONDITIONAL_PASS                     = _T("条件あり・通過"); 
	LPCTSTR CONDITIONAL_BACKWARD                 = _T("条件あり・逆送"); 
	LPCTSTR PARTIAL_INCLUSION_SAME_DIRECTION     = _T("部分包含・同方向"); 
	LPCTSTR PARTIAL_INCLUSION_OPPOSITE_DIRECTION = _T("部分包含・逆方向"); 
	LPCTSTR FULL_INCLUSION_SAME_DIRECTION        = _T("完全包含・同方向"); 
	LPCTSTR FULL_INCLUSION_OPPOSITE_DIRECTION    = _T("完全包含・逆方向"); 
	LPCTSTR PASS                                 = _T("通過"); 
#else
	LPCTSTR ERROR_LQ_RESTRICTION_CHECK           = _T("Link queue restriction check error"); 
	LPCTSTR CONFIRM_CONTINUE                     = _T("\nDo you want to continue?"); 
	LPCTSTR CHECK_LQ_RESTRICTION                 = _T("Link queue restriction check"); 
	LPCTSTR UNCONDITIONAL_PASS                   = _T("Unconditional : pass"); 
	LPCTSTR UNCONDITIONAL_BACKWARD               = _T("Unconditional : backward"); 
	LPCTSTR CONDITIONAL_PASS                     = _T("Conditional : pass"); 
	LPCTSTR CONDITIONAL_BACKWARD                 = _T("Conditional : backward"); 
	LPCTSTR PARTIAL_INCLUSION_SAME_DIRECTION     = _T("Partial inclusion : same direction"); 
	LPCTSTR PARTIAL_INCLUSION_OPPOSITE_DIRECTION = _T("Partial inclusion : opposite direction"); 
	LPCTSTR FULL_INCLUSION_SAME_DIRECTION        = _T("Full inclusion : same direction"); 
	LPCTSTR FULL_INCLUSION_OPPOSITE_DIRECTION    = _T("Full inclusion : opposite direction"); 
	LPCTSTR PASS                                 = _T("Pass"); 
#endif // SINDY_FOR_ENGLISH
}

#ifdef _DEBUG
CTimerPerformanceCounter::CTimerPerformanceCounter()
{
	perf_cnt = 0;
	start_time = 0;
	time_scale = 0;
}

// 初期化処理：パフォーマンスカウンタの単位を獲得
void CTimerPerformanceCounter::Init()
{
	perf_cnt = 0;
	start_time = 0;
	time_scale = 0;
	// perf_cntの測定
	if ( QueryPerformanceFrequency( (LARGE_INTEGER *)&perf_cnt ) != 0 )
		time_scale = 1.0f / perf_cnt;
}

// 現在のパフォーマンスカウンタ値を保存
void CTimerPerformanceCounter::Start()
{
	Init();
	QueryPerformanceCounter( (LARGE_INTEGER *)&start_time );
}

// 前回保存した値との差分を返す
DOUBLE CTimerPerformanceCounter::Stop()
{
	LONGLONG	finish_time = 0;

	QueryPerformanceCounter( (LARGE_INTEGER *)&finish_time );
	return (DOUBLE)((finish_time - start_time)*time_scale);
}
#endif // ifdef _DEBUG

// LQEditTool/LQSkecthTool
CLQHelper::CLQHelper( IApplicationPtr ipApp, IWorkspacePtr ipWorkspace )
{
	m_ipApp = ipApp;
	m_ipWorkspace = ipWorkspace;
	Initialization();

	if ( !m_hWnd )
		m_ipApp->get_hWnd( (OLE_HANDLE*)&m_hWnd );
}

// AttributeDlg
CLQHelper::CLQHelper( IApplicationPtr ipApp, IWorkspacePtr ipWorkspace, HWND hWnd/*=NULL*/, HWND hParent/*=NULL*/ )
{
	m_ipApp = ipApp;
	m_ipWorkspace = ipWorkspace;
	Initialization();
	m_hWnd = hWnd;
	m_hParent = hParent;

	if ( !m_hWnd )
		m_ipApp->get_hWnd( (OLE_HANDLE*)&m_hWnd );
}

// LQEditTool/LQSkecthTool
void CLQHelper::Init( IApplicationPtr ipApp, IWorkspacePtr ipWorkspace )
{
	m_ipApp = ipApp;
	m_ipWorkspace = ipWorkspace;
	Initialization();

	if ( !m_hWnd )
		m_ipApp->get_hWnd( (OLE_HANDLE*)&m_hWnd );
}

// AttributeDlg
void CLQHelper::Init2( IApplicationPtr ipApp, IWorkspacePtr ipWorkspace, HWND hWnd/*=NULL*/, HWND hParent/*=NULL*/ )
{
	m_ipApp = ipApp;
	m_ipWorkspace = ipWorkspace;
	Initialization();
	m_hWnd = hWnd;
	m_hParent = hParent;

	if ( !m_hWnd )
		m_ipApp->get_hWnd( (OLE_HANDLE*)&m_hWnd );
}

void CLQHelper::Initialization()
{
	m_hWnd = NULL;
	m_hParent = NULL;
	m_bIsNoMessage = FALSE;
	m_bIsMessageBox = TRUE;
	m_strErrorMessage.Empty();
	m_bInit = true;

	m_ipQueryFilter.CreateInstance( CLSID_QueryFilter );
}

IGeometryPtr CLQHelper::GetGeometry(const CTargetItems& cRoadItems, IPointPtr ipBaseFromPoint)
{
	CreateGeomInfo geominfo={FALSE,NULL,NULL,NULL};

	// 形状作成
	for ( CTargetItems::const_iterator itRoad = cRoadItems.begin(); itRoad != cRoadItems.end(); ++itRoad ) {
		IGeometryPtr ipGeom = AheGetClone( itRoad->GetShape() );

		if ( !ipGeom )
			continue;

		CreateGeometry( geominfo, ipGeom, ipBaseFromPoint );
	}

	return geominfo.ipPolyline;
}

IGeometryPtr CLQHelper::GetGeometry(LONG lInfoID, const std::list<LQITEM>& listLQ, std::map<LONG, IGeometryPtr> mapShape)
{
	if ( m_mapLQShape[lInfoID] )
		return m_mapLQShape[lInfoID];

	CreateGeomInfo geominfo={FALSE,NULL,NULL,NULL};

	// 形状作成
	for ( std::list<LQITEM>::const_iterator itLQ = listLQ.begin(); itLQ != listLQ.end(); ++itLQ ) {
		IGeometryPtr ipGeom = AheGetClone( mapShape[itLQ->LinkID] );

		if ( !ipGeom )
			continue;

		if ( itLQ->Direction == 2 )
			((IPolylinePtr)ipGeom)->ReverseOrientation();

		CreateGeometry( geominfo, ipGeom, NULL );
	}

	m_mapLQShape[lInfoID] = geominfo.ipPolyline;
	return geominfo.ipPolyline;
}

std::map< LONG, LONG > CLQHelper::GetInfoID(LONG lDrawMode, LONG LinkID, LPCTSTR lpszFieldName )
{
	std::map<LONG, LONG> mapInfoID;

	GetRoadLayer();

	// LQテーブル取得
	ITablePtr ipLQTable = GetTable( GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoLQTableName ) );

	if ( ipLQTable )	// [Bug 6551]リンク列のテーブルが存在しないことを許容するためNULLチェック強化
	{
		// Where句作成
		CString strWhere;

		strWhere.Format( _T("%s AND LINK_ID=%d"), GetContiguityMeshCode(), LinkID );

		// SubFields作成
		CString strFields = _T("INF_ID, ");

		strFields += lpszFieldName;

		// 検索
		m_ipQueryFilter->put_SubFields( CComBSTR(strFields) );
		m_ipQueryFilter->put_WhereClause( CComBSTR(strWhere) );

		_ICursorPtr ipCursor;
		ipLQTable->Search( m_ipQueryFilter, VARIANT_TRUE, &ipCursor );
		if ( !ipCursor )
			return mapInfoID;

		// フィールドインデックス取得
		IClassPtr ipClass = ipLQTable;
		IFieldsPtr ipFields;

		ipClass->get_Fields( &ipFields );

		// INF_ID
		long lIndex = 0;
		ipFields->FindField( CComBSTR(_T("INF_ID")), &lIndex );

		// lpszFieldName
		long lDIndex = 0;

		ipFields->FindField( CComBSTR(lpszFieldName), &lDIndex );

		_IRowPtr ipRow;

		while ( ipCursor->NextRow( &ipRow ) == S_OK ) {
			if ( !ipRow )
				break;

			// INF_ID
			CComVariant vaValue;

			ipRow->get_Value( lIndex, &vaValue );

			// lpszFieldName
			CComVariant vaValueD;

			ipRow->get_Value( lDIndex, &vaValueD );

			mapInfoID[vaValue.lVal] = vaValueD.lVal;
		}
	}

	return mapInfoID;
}

// LQ列ソート
void CLQHelper::LQItemsSort(std::list<LQITEM>& listLQ)
{
	std::list<LONG> listTemp;

	for ( std::list<LQITEM>::const_iterator itC = listLQ.begin(); itC != listLQ.end(); ++itC )
		listTemp.push_back( itC->Sequence );

	listTemp.sort();
	listTemp.unique();

	std::list<LQITEM> listSortLQRows;

	for ( std::list<LONG>::const_iterator itS = listTemp.begin(); itS != listTemp.end(); ++itS ) {
		for ( std::list<LQITEM>::const_iterator itNew = listLQ.begin(); itNew != listLQ.end(); ++itNew ) {
			if ( itNew->Sequence == (*itS) ) {
				listSortLQRows.push_back( (*itNew) );
				break;
			}
		}
	}

	listLQ.swap( listSortLQRows );
}

void CLQHelper::GetRoadShapes(LONG lDrawMode, LPCTSTR lpcszIDs)
{
	CString strWhere;

	strWhere.Format( _T("OBJECTID IN (%s)"), lpcszIDs );

	// 検索
	m_ipQueryFilter->put_SubFields( CComBSTR(_T("OBJECTID, SHAPE")) );
	m_ipQueryFilter->put_WhereClause( CComBSTR(strWhere) );

	_ICursorPtr ipCursor;

	m_ipRoadTable->Search( m_ipQueryFilter, VARIANT_TRUE, &ipCursor );
	if ( !ipCursor )
		return;

	IFeatureCursorPtr ipFeatureCursor = ipCursor;
	IFeaturePtr ipFeature;

	while ( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK ) {
		if ( !ipFeature )
			break;

		long OID = 0;

		ipFeature->get_OID( &OID );

		IGeometryPtr ipGeom;

		ipFeature->get_Shape( &ipGeom );

		m_mapInfos[lDrawMode].mapShape[OID] = AheGetClone( ipGeom );
	}
}

void CLQHelper::GetInfoLQ(LONG lDrawMode, LONG LinkID/*, map< LONG, list<LQITEM> >& mapInfo, map< LONG, IGeometryPtr >& mapShape*/)
{
	GetRoadLayer();

	if ( m_bInit ) {
		m_mapInfos[lDrawMode].mapInfo.clear();
		m_mapInfos[lDrawMode].mapShape.clear();
	}

	// LQテーブル取得
	ITablePtr ipLQTable = GetTable( GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoLQTableName ) );

	if ( ipLQTable )	// [Bug 6551]リンク列のテーブルが存在しないことを許容するためNULLチェック強化
	{
		// Where句作成
		CString strWhere;

		strWhere.Format( _T("%s AND LINK_ID=%d"), GetContiguityMeshCode(), LinkID );

		if ( ! m_bInit ) {
			if ( ! m_mapInfos[lDrawMode].mapInfo.empty() ) {
				strWhere += _T(" AND INF_ID NOT IN (");

				CString strIDs;
				for ( std::map< LONG, std::list<LQITEM> >::const_iterator it = m_mapInfos[lDrawMode].mapInfo.begin();
					it != m_mapInfos[lDrawMode].mapInfo.end(); ++it ) {

					if ( it != m_mapInfos[lDrawMode].mapInfo.begin() )
						strIDs += _T(",");

					strIDs.Format( _T("%d"), it->first );
				}

				strIDs += _T(")");
				strWhere += strIDs;
			}
		}

		// 検索
		m_ipQueryFilter->put_SubFields( CComBSTR(_T("INF_ID")) );
		m_ipQueryFilter->put_WhereClause( CComBSTR(strWhere) );

		_ICursorPtr ipCursor;

		ipLQTable->Search( m_ipQueryFilter, VARIANT_TRUE, &ipCursor );
		if ( !ipCursor )
			return;

		// フィールドインデックス取得
		IClassPtr ipClass = ipLQTable;
		IFieldsPtr ipFields;

		ipClass->get_Fields( &ipFields );

		// INF_ID
		long lIndex = 0;
		ipFields->FindField( CComBSTR(_T("INF_ID")), &lIndex );

		_IRowPtr ipRow;
		CString strIDs;

		// INF_ID群作成
		while ( ipCursor->NextRow( &ipRow ) == S_OK ) {
			if ( !ipRow )
				break;

			// INF_ID
			CComVariant vaValue;

			ipRow->get_Value( lIndex, &vaValue );

			// 
			if ( strIDs.IsEmpty() )
				strIDs.Format( _T("%d"), vaValue.lVal );
			else {
				CString strID;

				strID.Format( _T(",%d"), vaValue.lVal );
				strIDs += strID;
			}
		}

		if ( !strIDs.IsEmpty() ) {
			// Where句作成
			strWhere.Format( _T("%s AND INF_ID IN (%s)"), GetContiguityMeshCode(), strIDs );

			// 検索
			m_ipQueryFilter->put_SubFields( CComBSTR(_T("OBJECTID, INF_ID, LINK_ID, LINKDIR_C, SEQUENCE, LASTLINK_F, MESHCODE")) );
			m_ipQueryFilter->put_WhereClause( CComBSTR(strWhere) );

			ipLQTable->Search( m_ipQueryFilter, VARIANT_TRUE, &ipCursor );
			if ( !ipCursor )
				return;

			// LINK_ID
			long lLinkIDIndex = 0;

			ipFields->FindField( CComBSTR(LINK_ID_FIELD), &lLinkIDIndex );

			// LINKDIR_C
			long lLinkDirIndex = 0;

			ipFields->FindField( CComBSTR(LINKDIR_C_FIELD), &lLinkDirIndex );

			// SEQUENCE
			long lSequenceIndex = 0;

			ipFields->FindField( CComBSTR(SEQUENCE_FIELD), &lSequenceIndex );

			// LASTLINK_F
			long lLastFIndex = 0;

			ipFields->FindField( CComBSTR(LASTLINK_F_FIELD), &lLastFIndex );

			// MESHCODE
			long lMeshIndex = 0;

			ipFields->FindField( CComBSTR(MESHCODE_FIELD), &lMeshIndex );

			std::list<LQITEM> listLQ;
			std::list<LONG> listRoad;

			while ( ipCursor->NextRow( &ipRow ) == S_OK ) {
				if ( !ipRow )
					break;

				// OBJECTID
				long OID = 0;

				ipRow->get_OID( &OID );

				// INF_ID
				CComVariant vaValue;

				ipRow->get_Value( lIndex, &vaValue );

				// LINK_ID
				CComVariant vaValueLinkID;

				ipRow->get_Value( lLinkIDIndex, &vaValueLinkID );

				// LINKDIR_C
				CComVariant vaValueDir;

				ipRow->get_Value( lLinkDirIndex, &vaValueDir );

				// SEQUENCE
				CComVariant vaValueSeq;

				ipRow->get_Value( lSequenceIndex, &vaValueSeq );

				// LAST_LINKF
				CComVariant vaValueLastF;

				ipRow->get_Value( lLastFIndex, &vaValueLastF );

				// MESHCODE
				CComVariant vaValueMesh;

				ipRow->get_Value( lMeshIndex, &vaValueMesh );

				// 
				LQITEM LQItem;

				//LQItem.Object = ipRow;
				LQItem.OID = OID;
				LQItem.InfoID = vaValue.lVal;;
				LQItem.LinkID = vaValueLinkID.lVal;
				LQItem.Sequence = vaValueSeq.lVal;
				LQItem.Direction = vaValueDir.lVal;
				LQItem.LastF = vaValueLastF.lVal;
				LQItem.MeshCode = vaValueMesh.lVal;

				m_mapInfos[lDrawMode].mapInfo[vaValue.lVal].push_back( LQItem );
				listRoad.push_back( vaValueLinkID.lVal );
			}

			// LINK_IDソート
			listRoad.sort();
			listRoad.unique();

			// LQソート
			for ( std::map< LONG, std::list<LQITEM> >::iterator it = m_mapInfos[lDrawMode].mapInfo.begin(); it != m_mapInfos[lDrawMode].mapInfo.end(); ++it )
				LQItemsSort( it->second );

			long i = 0;

			strIDs.Empty();
			for ( std::list<LONG>::const_iterator itRoad = listRoad.begin(); itRoad != listRoad.end(); ++itRoad, ++i ) {
				if ( strIDs.IsEmpty() )
					strIDs.Format( _T("%d"), (*itRoad) );
				else {
					CString strID;

					strID.Format( _T(",%d"), (*itRoad) );
					strIDs += strID;

					if ( i >= 999 ) {
						i = 0;

						GetRoadShapes( lDrawMode, strIDs );
					}
				}
			}

			if ( !strIDs.IsEmpty() )
				GetRoadShapes( lDrawMode, strIDs );
		}
	}
}

// 規制チェック
BOOL CLQHelper::LQCheck( const CTargetItems& cRoadItems, IPointPtr ipBaseFromPoint, LONG& lRetErrorDrawMode, BOOL bIsCheckFrom, BOOL bEditEnd, BOOL bIsCheckAll/*=TRUE*/, LONG lOwnerOID/*=-1*/, std::vector<LONG>* vecVICSID/*=NULL*/, bool bInit/*=true*/ )
{
	GetRoadLayer();

	// 余計なモードを除外
	lRetErrorDrawMode = lRetErrorDrawMode & ~(_MULTISYMBOL_VERTEX | _MULTISYMBOL_ENDPOINT | _MULTISYMBOL_ONEWAY | _MULTISYMBOL_NOPASSAGE | _MULTISYMBOL_ANNOTATION | _MULTISYMBOL_S1ANNOPOINT | _MULTISYMBOL_S2ANNOPOINT | _MULTISYMBOL_S3ANNOPOINT | _MULTISYMBOL_S4ANNOPOINT);
	if ( lRetErrorDrawMode == 0 )
		return TRUE;

	CWaitCursor cur;

	// 初期化
	m_strErrorMessage.Empty();
	m_bIsNoMessage = TRUE;

	m_bInit = bInit;
	if ( m_bInit ) {
		m_mapInfos.clear();
		m_mapLQShape.clear();
		m_mapINF.clear();
		m_mapTables.clear();
	}
	m_mapMsg.clear();

	for ( LONG i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; ++i ) {
		long lDrawMode = GetLoadQueueDrawMode( (sindyeRoadQueueType)i );

		if ( lRetErrorDrawMode & lDrawMode ) {
			m_CheckItem.Name = GetLoadQueueInfo( (sindyeRoadQueueType)i, sindyeLoadQueueInfoAliasName );

			if ( bIsCheckAll || bEditEnd ) {
				CreateGeomInfo geominfo={FALSE,NULL,NULL,NULL};

				long	j = 0;
				for ( CTargetItems::const_iterator it = cRoadItems.begin(); it != cRoadItems.end(); ++it, ++j ) {
					IGeometryPtr ipGeom = AheGetClone( it->GetShape() );

					if ( !ipGeom )
						continue;

					CreateGeometry( geominfo, ipGeom, ipBaseFromPoint );

					m_CheckItem.Item = (*it);
					m_CheckItem.Mode = lDrawMode;
					m_CheckItem.Mode = bEditEnd;

					if ( !LQCheckByDrawMode( cRoadItems, (*it), geominfo.ipPolyline, ipBaseFromPoint, lDrawMode, bIsCheckFrom, j, j == (INT)cRoadItems.size() ? TRUE: FALSE, bEditEnd, lOwnerOID, vecVICSID ) ) {
						lRetErrorDrawMode = lRetErrorDrawMode & ~lDrawMode;
						if ( lRetErrorDrawMode == 0 ) {
							if ( m_bIsMessageBox && !m_strErrorMessage.IsEmpty() ) {
								AheMessageBox( m_strErrorMessage, ERROR_LQ_RESTRICTION_CHECK, MB_OK|MB_ICONEXCLAMATION, WARNLEVEL_NONE );
							}
							m_bIsNoMessage = FALSE;
							return FALSE;
						}
					}
				}
			} else {
				CTargetItems::const_iterator it = cRoadItems.end();

				if ( bIsCheckFrom )
					it = cRoadItems.begin();
				else
					it--;

				IGeometryPtr ipLQGeometry = GetGeometry( cRoadItems, ipBaseFromPoint );

				m_CheckItem.Item = (*it);
				m_CheckItem.Mode = lDrawMode;
				m_CheckItem.Mode = FALSE;

				if ( !LQCheckByDrawMode( cRoadItems, (*it), ipLQGeometry, ipBaseFromPoint, lDrawMode, bIsCheckFrom, cRoadItems.size(), TRUE, bEditEnd, lOwnerOID, vecVICSID ) ) {
					lRetErrorDrawMode = lRetErrorDrawMode & ~lDrawMode;
					if ( lRetErrorDrawMode == 0 ) {
						if ( m_bIsMessageBox && !m_strErrorMessage.IsEmpty() )
							AheMessageBox( m_strErrorMessage, ERROR_LQ_RESTRICTION_CHECK, MB_OK|MB_ICONEXCLAMATION, WARNLEVEL_NONE );

						m_bIsNoMessage = FALSE;
						return FALSE;
					}
				}
			}
		}
	}

	if ( m_bIsMessageBox && !m_mapMsg.empty() ) {
		CString	strMsgs;

		for ( std::map< LONG, CString>::const_iterator itMsg = m_mapMsg.begin(); itMsg != m_mapMsg.end(); ++itMsg )
			strMsgs += itMsg->second;

		if ( AheMessageBox( m_hWnd, strMsgs + CONFIRM_CONTINUE, CHECK_LQ_RESTRICTION, MB_ICONQUESTION | MB_OKCANCEL, 0 ) == IDCANCEL )
			return FALSE;
	}

	return TRUE;
}

// 規制チェック
BOOL CLQHelper::LQCheck( const CTargetItem& cRoadItem, const CComVariant& vaNewValue_Oneway/*=-1*/, const CComVariant& vaNewValue_NoPassage/*=-1*/, const CComVariant& vaNewValue_OutCourse/*=-1*/ )
{
	return LQCheck( cRoadItem.GetFeature(), vaNewValue_Oneway, vaNewValue_NoPassage, vaNewValue_OutCourse );
}

// 規制チェック
BOOL CLQHelper::LQCheck( IFeaturePtr ipFeature, const CComVariant& vaNewValue_Oneway/*=-1*/, const CComVariant& vaNewValue_NoPassage/*=-1*/, const CComVariant& vaNewValue_OutCourse/*=-1*/, bool excludeUTurn/* = false */ )
{
	m_strErrorMessage.Empty();
	m_bIsNoMessage = TRUE;

	if ( m_hParent )
		::EnableWindow( m_hParent, FALSE );

	m_CheckItem.Item = ipFeature;

	BOOL	bRet = TRUE;

	if ( vaNewValue_Oneway.lVal != -1 ) {
		if ( !LQCheckByOneWay( ipFeature, vaNewValue_Oneway ) )
			bRet = FALSE;
	}

	if ( vaNewValue_NoPassage.lVal != -1 ) {
		if ( !LQCheckByNoPassage( ipFeature, vaNewValue_NoPassage, excludeUTurn ) )
			bRet = FALSE;
	}

	if ( vaNewValue_OutCourse.lVal != -1 ) {
		if ( !LQCheckByOutCourse( ipFeature, vaNewValue_OutCourse ) )
			bRet = FALSE;
	}

	if ( !bRet ) {
		if ( m_bIsMessageBox && !m_strErrorMessage.IsEmpty() )
			AheMessageBox( m_hWnd, m_strErrorMessage, ERROR_LQ_RESTRICTION_CHECK, MB_OK|MB_ICONEXCLAMATION, WARNLEVEL_NONE );
	}

	if ( m_hParent )
		::EnableWindow( m_hParent, TRUE );

	return bRet;
}

//////////////////////////////////////////////////////////////////////
// 規制チェック関数
//////////////////////////////////////////////////////////////////////
// 規制チェック
BOOL CLQHelper::LQCheckByDrawMode( const CTargetItems& cRoadItems, const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, BOOL bIsCheckFrom, LONG lCount, BOOL bEnd/*=FALSE*/, BOOL bEditEnd/*=FALSE*/, LONG lOwnerOID/*=-1*/, std::vector<LONG>* vecVICSID/*=NULL*/ )
{
	CWaitCursor cur;

	//!< 経路外チェック
	switch( lDrawMode )
	{
		case _MULTISYMBOL_VICS:
		case _MULTISYMBOL_INTERNAVI:
			if ( !LQCheckByOutCourse( cRoadItem, lDrawMode ) )
				return FALSE;
			break;
		default: break;
	}

	// 交差点チェック
	switch( lDrawMode )
	{
		case _MULTISYMBOL_VICS:
		case _MULTISYMBOL_INTERNAVI:
			if ( !LQCheckByVICSNoChange( cRoadItems, cRoadItem, ipLQGeometry, ipBaseFromPoint, lDrawMode, vecVICSID ) )
				return FALSE;
			break;
		default: break;
	}

	// ２次メッシュノードを跨いでいるか？
	switch( lDrawMode )
	{
		case _MULTISYMBOL_DIRGUIDE:
		case _MULTISYMBOL_GUIDE:
		case _MULTISYMBOL_NOLRTURN:
		case _MULTISYMBOL_LANE:
		case _MULTISYMBOL_UTURN:
			if ( bEditEnd )
			{
				if ( !IsAdjoinDisconnect( cRoadItems, ipBaseFromPoint, lDrawMode ) )
					return FALSE;
			}
		default: break;
	}

	//!< 重複チェック(同一方向/同一形状)
	switch( lDrawMode )
	{
		case _MULTISYMBOL_ROUTE:
			if ( bEditEnd )
			{
				if ( !LQCheckBySameShapeDirection( cRoadItem, ipLQGeometry, ipBaseFromPoint, lDrawMode, lOwnerOID, bIsCheckFrom ) )
					return FALSE;
			}
			break;
		default: break;
	}

	// 規制チェック(一方通行)
	if ( !LQCheckByOneWay( cRoadItem, ipLQGeometry, ipBaseFromPoint, lDrawMode, lCount, bEditEnd, lOwnerOID ) )
		return FALSE;

	// 規制チェック(通行禁止)
	if ( !LQCheckByNoPassage( cRoadItem, lDrawMode, lCount, bEditEnd, lOwnerOID ) )
		return FALSE;

	// 規制チェック(リンク列)
	if ( !LQCheckByLQ( cRoadItem, ipLQGeometry, ipBaseFromPoint, lDrawMode, bIsCheckFrom, lCount, bEditEnd, lOwnerOID ) )
		return FALSE;

	return TRUE;
}

// 規制チェック(一方通行)
BOOL CLQHelper::LQCheckByOneWay( const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, LONG lCount, BOOL bEnd, LONG lOwnerOID )
{
	// 一方通行取得
	CComVariant vaValue = AheGetAttribute( cRoadItem.GetFeature(), ONEWAY_C_FIELD );

	if ( vaValue.lVal > 0  ) {
		// 空間参照取得
		ISpatialReferencePtr ipSpRef = AheGetSpatialReference( cRoadItem.GetFeature() );

		IGeometryPtr ipRoadGeom = AheGetClone( cRoadItem.GetShape() );

		// 選択した道路のFrom/Toポイント取得
		IPointPtr ipRoadFromPoint, ipRoadToPoint;

		((IPolylinePtr)ipRoadGeom)->get_FromPoint( &ipRoadFromPoint );
		AheSnapToSpatialReference( ipRoadFromPoint, ipSpRef );
		((IPolylinePtr)ipRoadGeom)->get_ToPoint( &ipRoadToPoint );
		AheSnapToSpatialReference( ipRoadToPoint, ipSpRef );

		// 通過/逆送判定
		BOOL bIsDuplicateType=FALSE;	// 逆送:FALSE, 通過:TRUE

		if ( !ipLQGeometry && ipBaseFromPoint ) {
			// 初回 or リンク列が1本時
			if ( vaValue.lVal == 1 || vaValue.lVal == 3 ) {
				if ( AheIsEqual( ipRoadFromPoint, ipBaseFromPoint, ipSpRef ) )
					bIsDuplicateType = TRUE;	// 通過
				else
					bIsDuplicateType = FALSE;	// 逆送
			} else {
				if ( AheIsEqual( ipRoadToPoint, ipBaseFromPoint, ipSpRef ) )
					bIsDuplicateType = TRUE;	// 通過
				else
					bIsDuplicateType = FALSE;	// 逆送
			}
		} else {
			// リンク列形状のFrom/Toポイント取得
			IPointPtr ipLQFromPoint, ipLQToPoint;

			((IPolylinePtr)ipLQGeometry)->get_FromPoint( &ipLQFromPoint );
			AheSnapToSpatialReference( ipLQFromPoint, ipSpRef );
			((IPolylinePtr)ipLQGeometry)->get_ToPoint( &ipLQToPoint );
			AheSnapToSpatialReference( ipLQToPoint, ipSpRef );

			// 比較
			if ( AheIsEqual( ipRoadFromPoint, ipLQFromPoint, ipSpRef ) || AheIsEqual( ipRoadToPoint, ipLQToPoint, ipSpRef ) )
				bIsDuplicateType = TRUE;	// 通過
			else
				bIsDuplicateType = FALSE;	// 逆送

			// 種別が逆方向の時は、通過を逆送に逆送を通過に。
			if ( vaValue.lVal == 2 || vaValue.lVal == 4 || vaValue.lVal == 5 )
				bIsDuplicateType = !bIsDuplicateType;
		}

		// 条件あり/なし判定
		BOOL bIsConditionType=FALSE;	// 条件なし:FALSE, 条件あり:TRUE

		if ( vaValue.lVal == 1 || vaValue.lVal == 2 )
			// 条件なし
			bIsConditionType = FALSE;
		else if ( vaValue.lVal == 3 || vaValue.lVal == 4 || vaValue.lVal == 5 )
			// 条件あり
			bIsConditionType = TRUE;

		// チェックタイプ決定
		LONG lCheckType=0;

		if ( !bIsConditionType && bIsDuplicateType )
			lCheckType = 0;	// 条件なし・通過
		else if ( !bIsConditionType && !bIsDuplicateType )
			lCheckType = 1;	// 条件なし・逆送
		else if ( bIsConditionType && bIsDuplicateType )
			lCheckType = 2; // 条件あり・通過
		else
			lCheckType = 3;	// 条件あり・逆送

		// INFOテーブル取得
		ITablePtr ipInfoTable = GetTable( GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoInfoTableName ) );

		// 動作操作取得
		sindyeCheckOperation	CheckOPType = sindyeCheckOperation_ONE;
		sindyeCheckAction		CheckACTType = sindyeCheckAction_OK;

		GetCheckOperationAction( sindyeCheckOneway, lCheckType, lDrawMode, ipInfoTable, lOwnerOID, &CheckOPType, &CheckACTType );

		if ( CheckOPType == sindyeCheckOperation_ONE || ( bEnd && CheckOPType == sindyeCheckOperation_END ) ) {
			CString strConMsg;

			switch ( lCheckType ) {
				case 0:
					strConMsg = UNCONDITIONAL_PASS; break;
				case 1:
					strConMsg = UNCONDITIONAL_BACKWARD; break;
				case 2:
					strConMsg = CONDITIONAL_PASS; break;
				case 3:
					strConMsg = CONDITIONAL_BACKWARD; break;
			}

			// 処理
			switch ( CheckACTType ) {
				case sindyeCheckAction_OK:
					break;
				case sindyeCheckAction_NG:
					return DispErrorMessage( sindyeCheckErrorCode_Oneway, lDrawMode, cRoadItem.GetOID(), FALSE, strConMsg );
				case sindyeCheckAction_MSG:
					if ( bEnd && CheckOPType == sindyeCheckOperation_ONE )
						return TRUE;

					return DispErrorMessage( sindyeCheckErrorCode_Oneway_Msg, lDrawMode, cRoadItem.GetOID(), FALSE, strConMsg );
			}
		}
	}

	return TRUE;
}

// 規制チェック(通行禁止)
BOOL CLQHelper::LQCheckByNoPassage( const CTargetItem& cRoadItem, LONG lDrawMode, LONG lCount, BOOL bEnd, LONG lOwnerOID )
{
	// 通行禁止あり/なし(条件あり/なし)チェック
	CComVariant vaValue = AheGetAttribute( cRoadItem.GetFeature(), NOPASSAGE_C_FIELD );

	if ( vaValue.lVal > 0 ) {
		// チェックタイプ決定
		LONG lCheckType=0;
		sindyeCheckType opCode = sindyeCheckNoPassage;

		if ( vaValue.lVal == 1 ) {
			// 条件なし通行禁止
			lCheckType = 0;
			opCode = sindyeCheckNoPassage;
		} else if ( vaValue.lVal == 2 ) {
			// 条件あり通行禁止
			lCheckType = 1;
			opCode = sindyeCheckNoPassage;
		} else if ( vaValue.lVal == 4 ) {
			// 条件あり通行禁止（関係者以外）
			lCheckType = 0;
			opCode = sindyeCheckNoPassage_AP;
		} else if ( vaValue.lVal == 5 ) {
			// 条件あり通行禁止（関係者以外）
			lCheckType = 1;
			opCode = sindyeCheckNoPassage_AP;
		} else if ( vaValue.lVal == 3 ) {
			// 通行不可
			lCheckType = 0;
			opCode = sindyeCheckNoPassage_NG;
		}

		// INFOテーブル取得
		ITablePtr ipInfoTable = GetTable( GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoInfoTableName ) );

		// 動作操作取得
		sindyeCheckOperation	CheckOPType = sindyeCheckOperation_ONE;
		sindyeCheckAction		CheckACTType = sindyeCheckAction_OK;

		GetCheckOperationAction( opCode, lCheckType, lDrawMode, ipInfoTable, lOwnerOID, &CheckOPType, &CheckACTType );

		if ( CheckOPType == sindyeCheckOperation_ONE || ( bEnd && CheckOPType == sindyeCheckOperation_END ) ) {
			// 処理
			switch ( CheckACTType ) {
				case sindyeCheckAction_OK:
					break;
				case sindyeCheckAction_NG:
					return DispErrorMessage( sindyeCheckErrorCode_NoPassage, lDrawMode, cRoadItem.GetOID(), FALSE, PASS );
				case sindyeCheckAction_MSG:
					if ( bEnd && CheckOPType == sindyeCheckOperation_ONE )
						return TRUE;

					return DispErrorMessage( sindyeCheckErrorCode_NoPassage_Msg, lDrawMode, cRoadItem.GetOID(), FALSE, PASS );
			}
		}
	}

	return TRUE;
}

// 規制チェック(リンク列)
BOOL CLQHelper::LQCheckByLQ( const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, BOOL bIsCheckFrom, LONG lCount, BOOL bEnd/*=FALSE*/, LONG lOwnerOID/*=-1*/)
{
	BOOL	bRet = TRUE;

	for ( LONG i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; ++i ) {
		long CheckDrawMode = GetLoadQueueDrawMode( (sindyeRoadQueueType)i );

		CComVariant vaValue = AheGetAttribute( cRoadItem.GetFeature(), GetLoadQueueInfo( CheckDrawMode, sindyeLoadQueueInfoLQRFRoadFieldName ) );
		if ( vaValue.lVal > 0 ) {
			IGeometryPtr ipBaseGeometry;

			// 移動/作成チェック形状
			if ( !ipLQGeometry && ipBaseFromPoint )
				ipBaseGeometry = AheGetClone( cRoadItem.GetShape() );
			else
				ipBaseGeometry = AheGetClone( ipLQGeometry );

			// 重なり判定
			GetInfoLQ( CheckDrawMode, cRoadItem.GetOID() );

			ITablePtr ipInfoTableOwner = GetTable( GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoInfoTableName ) );
			ITablePtr ipInfoTable = GetTable( GetLoadQueueInfo( CheckDrawMode, sindyeLoadQueueInfoInfoTableName ) );

			for ( std::map< LONG, std::list<LQITEM> >::const_iterator itInfo = m_mapInfos[CheckDrawMode].mapInfo.begin(); itInfo != m_mapInfos[CheckDrawMode].mapInfo.end(); ++itInfo ) {
				// [Bug 6518]VICSを両方向に作成すると落ちる
				// VICS系リンク列作成時に、既存のVICS系リンク列に接するように道路を選択、
				// 最初の選択でリンク列チェックが走って、m_mapInfosにドローモードとINF/LQが入る
				// 確定時にVICSを作らないでインターナビだけ作ろうとすると、
				// m_mapInfosには中途半端なVICSがいるのでLQCheckByLQに入って落ちてしまっていた。
				// 根本の原因は、今までINFとかLQをキャッシュしない作りにしていたのを
				// キャッシュできるようにしたため。キャッシュに対応しきれていなかった。。。
				if ( itInfo->first == lOwnerOID || itInfo->second.empty() )
					continue;

				IGeometryPtr ipUninoGeom = GetGeometry( itInfo->first, itInfo->second, m_mapInfos[CheckDrawMode].mapShape );

				if ( !LQCheckByLQ( ipInfoTableOwner, ipInfoTable, CheckDrawMode, itInfo->first, cRoadItem, ipBaseGeometry, ipUninoGeom, lDrawMode, bIsCheckFrom, lCount, bEnd, lOwnerOID ) )
					bRet = FALSE;
			}
		}
	}

	return bRet;
}

BOOL CLQHelper::LQCheckByLQ(ITablePtr ipInfoTableOwner, ITablePtr ipInfoTable, LONG CheckDrawMode, LONG lInfoID, const CTargetItem& cRoadItem, IGeometryPtr ipBaseGeometry, IGeometryPtr ipUninoGeom, LONG lDrawMode, BOOL bIsCheckFrom, LONG lCount, BOOL bEnd/*=FALSE*/, LONG lOwnerOID )
{
	// 空間参照取得
	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( cRoadItem.GetFeature() );

	// 包含チェック
	long nInclude = IsIncluedByPoints( ipBaseGeometry, ipUninoGeom, ipSpRef );

	if ( nInclude > 0 ) {
		// 同一方向チェック
		BOOL bIsEqualDirection = IsEqualDirectionByPoints( ipUninoGeom, ipBaseGeometry, bIsCheckFrom, ipSpRef );

		long lCheckType = 0;

		if ( bIsEqualDirection && nInclude == 1 )
			lCheckType = 0;	// 部分包含・同方向
		else if ( !bIsEqualDirection && nInclude == 1 )
			lCheckType = 1;	// 部分包含・逆方向
		else if ( bIsEqualDirection && nInclude == 2 )
			lCheckType = 2;	// 完全包含・同方向
		else
			lCheckType = 3;	// 完全包含・逆方向

		// 条件判定
		BOOL bConditionType = GetConditionType( ipInfoTableOwner, lOwnerOID, lDrawMode );

		// チェックタイプ決定
		sindyeCheckType opCode = sindyeDirguide;
		switch ( lDrawMode ) {
			case _MULTISYMBOL_DIRGUIDE:
				opCode = sindyeDirguide;
				break;
			case _MULTISYMBOL_GUIDE:
				opCode = sindyeGuide;
				break;
			case _MULTISYMBOL_VICS:
				opCode = sindyeVics;
				break;
			case _MULTISYMBOL_BYWAY:
				opCode = sindyeByway;
				break;
			case _MULTISYMBOL_NOLRTURN:
				if ( bConditionType )
					opCode = sindyeTurnreg_CN;
				else
					opCode = sindyeTurnreg;
				break;
			case _MULTISYMBOL_LANE:
				opCode = sindyeLane;
				break;
			case _MULTISYMBOL_ROUTE:
				if ( bConditionType )
					opCode = sindyeRoute_TR;
				else
					opCode = sindyeRoute;
				break;
			case _MULTISYMBOL_UTURN:
				opCode = sindyeUTurn;
				break;
			case _MULTISYMBOL_INTERNAVI:
				opCode = sindyeInternavi;
				break;
			case _MULTISYMBOL_IPCVICS:
				opCode = sindyeIPCVics;
				break;
			case _MULTISYMBOL_INTERSECTION:
				opCode = sindyeIntersection;
				break;
		}

		// 動作操作取得
		sindyeCheckOperation	CheckOPType = sindyeCheckOperation_ONE;
		sindyeCheckAction		CheckACTType = sindyeCheckAction_OK;

		GetCheckOperationAction( opCode, lCheckType, CheckDrawMode, ipInfoTable, lInfoID, &CheckOPType, &CheckACTType );

		if ( CheckOPType == sindyeCheckOperation_ONE || ( bEnd && CheckOPType == sindyeCheckOperation_END ) ) {
			CString strConMsg;

			switch ( lCheckType ) {
				case 0:
					strConMsg = PARTIAL_INCLUSION_SAME_DIRECTION; break;
				case 1:
					strConMsg = PARTIAL_INCLUSION_OPPOSITE_DIRECTION; break;
				case 2:
					strConMsg = FULL_INCLUSION_SAME_DIRECTION; break;
				case 3:
					strConMsg = FULL_INCLUSION_OPPOSITE_DIRECTION; break;
			}

			// 処理
			switch ( CheckACTType ) {
				case sindyeCheckAction_OK:
					break;
				case sindyeCheckAction_NG:
					return DispErrorMessage( sindyeCheckErrorCode_LQ, lDrawMode, CheckDrawMode, cRoadItem.GetOID(), lInfoID, FALSE, strConMsg );
				case sindyeCheckAction_MSG:
					if ( !DispErrorMessage( sindyeCheckErrorCode_LQ_Msg, lDrawMode, CheckDrawMode, cRoadItem.GetOID(), lInfoID, FALSE, strConMsg ) )
						return FALSE;
					break;
			}
		}
	}

	return TRUE;
}

// 重複チェック
BOOL CLQHelper::LQCheckBySameShapeDirection( const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, LONG lOwnerOID, BOOL bIsCheckFrom/*=FALSE*/ )
{
	// リンク列存在フラグ取得
	CComVariant vaValue = AheGetAttribute( cRoadItem.GetFeature(), GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoLQRFRoadFieldName ) );

	if ( vaValue.lVal > 0 ) {
		// 空間参照取得
		ISpatialReferencePtr ipSpRef = AheGetSpatialReference( cRoadItem.GetFeature() );

		// 移動/作成チェック形状
		IGeometryPtr ipBaseGeometry;

		if ( !ipLQGeometry && ipBaseFromPoint )
			ipBaseGeometry = AheGetClone( cRoadItem.GetShape() );
		else
			ipBaseGeometry = AheGetClone( ipLQGeometry );

		// リンク列取得
		GetInfoLQ( lDrawMode, cRoadItem.GetOID() );

		// 重なり判定
		for ( std::map< LONG, std::list<LQITEM> >::const_iterator itInfo = m_mapInfos[lDrawMode].mapInfo.begin(); itInfo != m_mapInfos[lDrawMode].mapInfo.end(); ++itInfo ) {
			// [Bug 6518]VICSを両方向に作成すると落ちる
			// VICS系リンク列作成時に、既存のVICS系リンク列に接するように道路を選択、
			// 最初の選択でリンク列チェックが走って、m_mapInfosにドローモードとINF/LQが入る
			// 確定時にVICSを作らないでインターナビだけ作ろうとすると、
			// m_mapInfosには中途半端なVICSがいるのでLQCheckByLQに入って落ちてしまっていた。
			// 根本の原因は、今までINFとかLQをキャッシュしない作りにしていたのを
			// キャッシュできるようにしたため。キャッシュに対応しきれていなかった。。。
			if ( itInfo->first == lOwnerOID || itInfo->second.empty() )
				continue;

			// 形状取得
			IGeometryPtr ipUninoGeom = GetGeometry( itInfo->first, itInfo->second, m_mapInfos[lDrawMode].mapShape );

			// 同一方向?
			if ( IsEqualDirectionByPoints( ipUninoGeom, ipBaseGeometry, bIsCheckFrom, ipSpRef ) ) {
				// 同一形状?
				if ( IsEqualByPoints( ipUninoGeom, ipBaseGeometry, ipSpRef, FALSE ) )
					return DispErrorMessage( sindyeCheckErrorCode_Set_ShapeDirection, lDrawMode );
			}
		}
	}

	return TRUE;
}

// 経路外チェック
BOOL CLQHelper::LQCheckByOutCourse( const CTargetItem& cRoadItem, LONG lDrawMode )
{
	// 道路種別取得
	CComVariant vaValue = AheGetAttribute( cRoadItem.GetFeature(), _T("NAVICLASS_C") );

	if ( vaValue.lVal == 0 ) {
		// 経路外
		if ( lDrawMode == _MULTISYMBOL_VICS )
			return DispErrorMessage(sindyeCheckErrorCode_OutCourse_Vics, lDrawMode);
		else 
			return DispErrorMessage(sindyeCheckErrorCode_OutCourse_Internavi, lDrawMode);
	}

	return TRUE;
}

// 規制チェック(一方通行)
BOOL CLQHelper::LQCheckByOneWay( IFeaturePtr ipFeature, const CComVariant& vaNewValue )
{
	if ( ipFeature && vaNewValue.lVal > 0 ) {
		// 条件あり/なし判定
		BOOL bIsConditionType=FALSE;	// 条件なし:FALSE, 条件あり:TRUE

		if ( vaNewValue.lVal == 1 || vaNewValue.lVal == 2 )
			// 条件なし
			bIsConditionType = FALSE;
		else if ( vaNewValue.lVal == 3 || vaNewValue.lVal == 4 || vaNewValue.lVal == 5 )
			// 条件あり
			bIsConditionType = TRUE;

		for ( LONG i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; ++i ) {
			long lDrawMode = GetLoadQueueDrawMode( (sindyeRoadQueueType)i );

			CComVariant vaValue = AheGetAttribute( ipFeature, GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoLQRFRoadFieldName ) );

			if ( vaValue.lVal == 1 ) {
				long	OID = 0;

				ipFeature->get_OID( &OID );

				ITablePtr ipInfoTable = GetTable( GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoInfoTableName ) );

				std::map<LONG,LONG> mapInfoID = GetInfoID( lDrawMode, OID, LINKDIR_C_FIELD );

				for ( std::map<LONG,LONG>::const_iterator it = mapInfoID.begin(); it != mapInfoID.end(); ++it ) {
					BOOL bIsDuplicateType = FALSE;

					// 通過判定
					if ( it->second == 1 && ( vaNewValue.lVal == 1 || vaNewValue.lVal == 3 ) )
						bIsDuplicateType = TRUE;
					else if ( it->second == 2 && ( vaNewValue.lVal == 2 || vaNewValue.lVal == 4 || vaNewValue.lVal == 5 ) )
						bIsDuplicateType = TRUE;

					// チェックタイプ決定
					LONG lCheckType = 0;

					if ( !bIsConditionType && bIsDuplicateType )
						lCheckType = 0;	// 条件なし・通過
					else if ( !bIsConditionType && !bIsDuplicateType )
						lCheckType = 1;	// 条件なし・逆送
					else if ( bIsConditionType && bIsDuplicateType )
						lCheckType = 2;	// 条件あり・通過
					else
						lCheckType = 3;	// 条件あり・逆送

					// 動作操作取得
					sindyeCheckOperation	CheckOPType = sindyeCheckOperation_ONE;
					sindyeCheckAction		CheckACTType = sindyeCheckAction_OK;

					GetCheckOperationAction( sindyeCheckOneway, lCheckType, lDrawMode, ipInfoTable, it->first, &CheckOPType, &CheckACTType );

					CString strConMsg;

					switch ( lCheckType ) {
						case 0:
							strConMsg = UNCONDITIONAL_PASS; break;
						case 1:
							strConMsg = UNCONDITIONAL_BACKWARD; break;
						case 2:
							strConMsg = CONDITIONAL_PASS; break;
						case 3:
							strConMsg = CONDITIONAL_BACKWARD; break;
					}

					BOOL TorF = TRUE;

					switch ( CheckACTType ) {
						case sindyeCheckAction_OK:
							break;
						case sindyeCheckAction_NG:
							TorF = DispErrorMessage( sindyeCheckErrorCode_Oneway, lDrawMode, OID, TRUE, strConMsg );
							break;
						case sindyeCheckAction_MSG:
							TorF = DispErrorMessage( sindyeCheckErrorCode_Oneway_Msg, lDrawMode, OID, TRUE, strConMsg );
							break;
					}

					if ( !TorF )
						return FALSE;
				}
			}
		}
	}

	return TRUE;
}

// 規制チェック(通行禁止)
BOOL CLQHelper::LQCheckByNoPassage( IFeaturePtr ipFeature, const CComVariant& vaNewValue, bool excludeUTurn/* = false*/  )
{
	if ( ipFeature && vaNewValue.lVal > 0 ) {
		for ( long i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; ++i ) {

			// Uターンは除外（bug 10617）
			if( excludeUTurn &&
				sindyeRoadQueueUTurn == (sindyeRoadQueueType)i )
				continue;

			long lDrawMode = GetLoadQueueDrawMode( (sindyeRoadQueueType)i );

			CComVariant vaValue = AheGetAttribute( ipFeature, GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoLQRFRoadFieldName ) );

			if ( vaValue.lVal == 1 ) {
				long	OID = 0;

				ipFeature->get_OID( &OID );

				ITablePtr ipInfoTable = GetTable( GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoInfoTableName ) );

				std::map<LONG,LONG> mapInfoID = GetInfoID( lDrawMode, OID, LINKDIR_C_FIELD );

				for ( std::map<LONG,LONG>::const_iterator it = mapInfoID.begin(); it != mapInfoID.end(); ++it ) {
					sindyeCheckType opCode = sindyeCheckNoPassage;
					long lCheckType = 0;

					if ( vaNewValue.lVal == 1 ) {
						// 条件なし通行禁止
						lCheckType = 0;
						opCode = sindyeCheckNoPassage;
					} else if ( vaNewValue.lVal == 2 ) {
						// 条件あり通行禁止
						lCheckType = 1;
						opCode = sindyeCheckNoPassage;
					} else if ( vaNewValue.lVal == 4 ) {
						// 条件あり通行禁止（関係者以外）
						lCheckType = 0;
						opCode = sindyeCheckNoPassage_AP;
					} else if ( vaNewValue.lVal == 5 ) {
						// 条件あり通行禁止（関係者以外）
						lCheckType = 1;
						opCode = sindyeCheckNoPassage_AP;
					} else if ( vaNewValue.lVal == 3 ) {
						// 通行不可
						lCheckType = 0;
						opCode = sindyeCheckNoPassage_NG;
					}

					// 動作操作取得
					sindyeCheckOperation	CheckOPType = sindyeCheckOperation_ONE;
					sindyeCheckAction		CheckACTType = sindyeCheckAction_OK;

					GetCheckOperationAction( opCode, lCheckType, lDrawMode, ipInfoTable, it->first, &CheckOPType, &CheckACTType );

					BOOL TorF = TRUE;

					switch ( CheckACTType ) {
						case sindyeCheckAction_OK:
							break;
						case sindyeCheckAction_NG:
							TorF = DispErrorMessage( sindyeCheckErrorCode_NoPassage, lDrawMode, OID, TRUE, PASS );
							break;
						case sindyeCheckAction_MSG:
							TorF = DispErrorMessage( sindyeCheckErrorCode_NoPassage_Msg, lDrawMode, OID, TRUE, PASS );
							break;
					}

					if ( !TorF )
						return FALSE;
				}
			}
		}
	}

	return TRUE;
}

// 経路外チェック
BOOL CLQHelper::LQCheckByOutCourse( IFeaturePtr ipFeature, const CComVariant& vaNewValue )
{
	if ( ipFeature ) {
		CComVariant vaValue, vaValueI;

		// VICS&インターナビの存在フラグ取得
		vaValue = AheGetAttribute( ipFeature, GetLoadQueueInfo( _MULTISYMBOL_VICS, sindyeLoadQueueInfoLQRFRoadFieldName ) );
		vaValueI = AheGetAttribute( ipFeature, GetLoadQueueInfo( _MULTISYMBOL_INTERNAVI, sindyeLoadQueueInfoLQRFRoadFieldName ) );

		LONG lOID=0;

		ipFeature->get_OID( &lOID );

		if ( vaValue.lVal == 1 && vaValueI.lVal == 0 )
			return DispErrorMessage( sindyeCheckErrorCode_OutCourse1, _MULTISYMBOL_VICS, lOID, TRUE );
		else if ( vaValue.lVal == 0 && vaValueI.lVal == 1 )
			return DispErrorMessage( sindyeCheckErrorCode_OutCourse2, _MULTISYMBOL_INTERNAVI, lOID, TRUE );
		else if ( vaValue.lVal == 1 && vaValueI.lVal == 1 )
			return DispErrorMessage( sindyeCheckErrorCode_OutCourse3, (_MULTISYMBOL_VICS|_MULTISYMBOL_INTERNAVI), lOID, TRUE );
	}

	return TRUE;
}

BOOL CLQHelper::IsAdjoinDisconnect(const CTargetItems& cRoadItems, IPointPtr ipBaseFromPoint, LONG lDrawMode)
{
	// 空間参照取得
	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( (*cRoadItems.begin()).GetFeature() );

	IGeometryPtr ipRoadGeom = AheGetClone( (*cRoadItems.begin()).GetShape() );

	// Fromポイント取得
	IPointPtr ipFromPoint;

	((IPolylinePtr)ipRoadGeom)->get_FromPoint( &ipFromPoint );
	AheSnapToSpatialReference( ipFromPoint, ipSpRef );

	// Fromポイント取得
	IPointPtr ipPoint;

	if ( AheIsEqual( ipBaseFromPoint, ipFromPoint, ipSpRef ) )
		((IPolylinePtr)ipRoadGeom)->get_ToPoint( &ipPoint );
	else
		((IPolylinePtr)ipRoadGeom)->get_FromPoint( &ipPoint );
	AheSnapToSpatialReference( ipPoint, ipSpRef );

	IDocumentPtr ipDoc;

	m_ipApp->get_Document( &ipDoc );

	IMxDocumentPtr ipMxDoc = ipDoc;
	IMapPtr ipMap;

	if ( ipMxDoc )
		ipMxDoc->get_FocusMap( &ipMap );

	// ノード取得
	IGeometryPtr ipSearchGeom = ipPoint;
	CTargetItems cItems;

	DOUBLE dBuffer = AheGetXYDomain1UnitLength( ipSpRef ) * 2.0f;

	ITopologicalOperatorPtr ipTopo( ipPoint );
	if ( ipTopo != NULL )
		ipTopo->Buffer( dBuffer, &ipSearchGeom );

	AheSnapToSpatialReference( ipSearchGeom, ipSpRef );

	IFeatureClassPtr ipNodeClass = GetTable( NODE_TABLE_NAME );

	AheSelectByShapeFromFeatureClass( ipSearchGeom, cItems, ipNodeClass );
	for ( CTargetItems::iterator it = cItems.begin(); it != cItems.end(); ++it ) {
		IGeometryPtr ipNodeGeom = AheGetClone( it->GetShape() );

		AheSnapToSpatialReference( ipNodeGeom, ipSpRef );

		if ( AheIsEqual( ipNodeGeom, ipPoint, ipSpRef ) && it->IsBorderNode() )
			return DispErrorMessage( sindyeCheckErrorCode_AdjoinDisconnect_Node, lDrawMode );
	}

	return TRUE;
}

BOOL CLQHelper::LQCheckByVICSNoChange( const CTargetItems& cRoadItems, const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, std::vector<LONG>* vecVICSID/*=NULL*/ )
{
	IPointPtr ipBaseToPoint;

	// Toポイント取得
	((IPolylinePtr)ipLQGeometry)->get_ToPoint( &ipBaseToPoint );

	// 交差点チェック
	if ( !IsVICSNoChange( cRoadItems, cRoadItem, ipBaseFromPoint, ipBaseToPoint, ipLQGeometry, lDrawMode, vecVICSID ) )
		return FALSE;

	return TRUE;
}

BOOL CLQHelper::IsVICSNoChange( const CTargetItems& cRoadItems, const CTargetItem& cRoadItem, IPointPtr ipBaseFromPoint, IPointPtr ipBaseToPoint, IGeometryPtr ipLQGeometry, LONG lDrawMode, std::vector<LONG>* pvecVICSID/*=NULL*/)
{
	CWaitCursor cur;

	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( cRoadItem.GetShape() );
	IGeometryPtr ipRoadGeom = AheGetClone( cRoadItem.GetShape() );

	// 道路のFrom/Toポイント取得
	IPointPtr ipFromPoint, ipToPoint;

	((IPolylinePtr)ipRoadGeom)->get_FromPoint( &ipFromPoint );
	AheSnapToSpatialReference( ipFromPoint, ipSpRef );
	((IPolylinePtr)ipRoadGeom)->get_ToPoint( &ipToPoint );
	AheSnapToSpatialReference( ipToPoint, ipSpRef );

	// From/Toポイントに接続する道路を検索
	IGeometryPtr ipSearchGeom = ipFromPoint;
	CTargetItems cItems;

	DOUBLE dBuffer = AheGetXYDomain1UnitLength( ipSpRef ) * 2.0f;

	ITopologicalOperatorPtr ipTopo( ipFromPoint );
	if ( ipTopo != NULL )
		ipTopo->Buffer( dBuffer, &ipSearchGeom );

	AheSnapToSpatialReference( ipSearchGeom, ipSpRef );
	AheSelectByShapeFromFeatureClass( ipSearchGeom, cItems, m_ipRoadTable );

	ipSearchGeom = ipToPoint;
	ipTopo = ipToPoint;
	if ( ipTopo != NULL )
		ipTopo->Buffer( dBuffer, &ipSearchGeom );

	AheSnapToSpatialReference( ipSearchGeom, ipSpRef );
	AheSelectByShapeFromFeatureClass( ipSearchGeom, cItems, m_ipRoadTable );

	// 選択道路除外
	cItems.remove( cRoadItem );

	//
	CTargetItems cRoads;

	for ( CTargetItems::const_iterator it = cItems.begin(); it != cItems.end(); ++it ) {
		// リンク列存在フラグ取得
		CComVariant vaValue = AheGetAttribute( (*it).GetFeature(), GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoLQRFRoadFieldName ) );

		if ( 1 == vaValue.lVal )
			cRoads.push_back(*it);
	}

	cRoads -= cRoadItems;

	//std::map< LONG, std::list<LQITEM> > mapDataVICS;

	// LQテーブル取得
	ITablePtr ipLQTable = GetTable( GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoLQTableName ) );

	if ( ipLQTable )	// [Bug 6551]リンク列のテーブルが存在しないことを許容するためNULLチェック強化
	{
		// 道路のOBJECTID群取得
		CString	strIDs;

		for ( CTargetItems::const_iterator it = cRoads.begin(); it != cRoads.end(); ++it ) {
			if ( strIDs.IsEmpty() )
				strIDs.Format( _T("%d"), (*it).GetOID() );
			else {
				CString	strID;

				strID.Format( _T(",%d"), (*it).GetOID() );
				strIDs += strID;
			}
		}

		// Where句作成
		CString strWhere;

		strWhere.Format( _T("%s AND LINK_ID IN (%s)"), GetContiguityMeshCode(), strIDs );

		// LQ検索
		m_ipQueryFilter->put_SubFields( CComBSTR(_T("OBJECTID,LINK_ID,LINKDIR_C,INF_ID,SEQUENCE,LASTLINK_F")) );
		m_ipQueryFilter->put_WhereClause( CComBSTR(strWhere) );

		_ICursorPtr ipCursor;

		ipLQTable->Search( m_ipQueryFilter, VARIANT_TRUE, &ipCursor );
		if ( ipCursor ) {
			// フィールドインデックス取得
			IClassPtr ipClass = ipLQTable;
			IFieldsPtr ipFields;

			ipClass->get_Fields( &ipFields );

			// INF_ID
			LONG lIndex=0;

			ipFields->FindField( CComBSTR(_T("INF_ID")), &lIndex );

			// LINKID
			LONG lLinkIDIndex=0;

			ipFields->FindField( CComBSTR(LINK_ID_FIELD), &lLinkIDIndex );

			// DIR_C
			LONG lLinkDirIndex=0;

			ipFields->FindField( CComBSTR(LINKDIR_C_FIELD), &lLinkDirIndex );

			// SEQUENCE
			LONG lSequenceIndex=0;

			ipFields->FindField( CComBSTR(SEQUENCE_FIELD), &lSequenceIndex );

			std::list<LONG>	listINF_IDs;
			std::list<LONG> listRoad;
			_IRowPtr ipRow;

			while ( ipCursor->NextRow( &ipRow ) == S_OK ) {
				if ( !ipRow )
					break;

				// OID
				long	OID=0;

				ipRow->get_OID( &OID );

				// INF_ID
				CComVariant vaValue;
				ipRow->get_Value( lIndex, &vaValue );

				// LINK_ID
				CComVariant vaValueLinkID;
				ipRow->get_Value( lLinkIDIndex, &vaValueLinkID );

				// DIR_C
				CComVariant vaValueDir;
				ipRow->get_Value( lLinkDirIndex, &vaValueDir );

				// SEQUENCE
				CComVariant vaValueSeq;
				ipRow->get_Value( lSequenceIndex, &vaValueSeq );

				// アイテムにセット
				LQITEM LQItem;

				//LQItem.Object = ipRow;
				LQItem.OID = OID;
				LQItem.InfoID = vaValue.lVal;;
				LQItem.LinkID = vaValueLinkID.lVal;
				LQItem.Sequence = vaValueSeq.lVal;
				LQItem.Direction = vaValueDir.lVal;

				// 格納済みアイテムに追加
				//mapDataVICS[LQItem.InfoID].push_back( LQItem );
				m_mapInfos[lDrawMode].mapInfo[LQItem.InfoID].push_back( LQItem );

				// INF_ID群
				listINF_IDs.push_back( LQItem.InfoID );

				// [Bug 6518]VICSを両方向に作成すると落ちる
				// 道路形状を保持していない？
				if ( m_mapInfos[lDrawMode].mapShape.find( LQItem.LinkID ) == m_mapInfos[lDrawMode].mapShape.end() )
					listRoad.push_back( LQItem.LinkID );
			}

			// LINK_IDソート
			listRoad.sort();
			listRoad.unique();

			// LQからINF検索
			strIDs.Empty();
			for ( std::list<LONG>::const_iterator itINF_ID = listINF_IDs.begin(); itINF_ID != listINF_IDs.end(); ++itINF_ID ) {
				// VICS_LINK_IDがないものは除外
				if ( m_mapINF[(*itINF_ID)] >= 1 )
					continue;

				if ( strIDs.IsEmpty() )
					strIDs.Format( _T("%d"), (*itINF_ID) );
				else {
					CString	strID;

					strID.Format( _T(",%d"), (*itINF_ID) );
					strIDs += strID;
				}
			}

			if ( !strIDs.IsEmpty() ) {
				// INFOテーブル取得
				ITablePtr ipInfoTable = GetTable( GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoInfoTableName ) );

				if ( ipInfoTable )	// [Bug 6551]リンク列のテーブルが存在しないことを許容するためNULLチェック強化
				{
					// Where句作成
					strWhere.Format( _T("OBJECTID IN (%s)"), strIDs );

					// INFO検索
					m_ipQueryFilter->put_SubFields( CComBSTR(_T("OBJECTID,VICS_LINK_ID")) );
					m_ipQueryFilter->put_WhereClause( CComBSTR(strWhere) );
					ipInfoTable->Search( m_ipQueryFilter, VARIANT_TRUE, &ipCursor );
					if ( !ipCursor )
						return TRUE;

					// フィールドインデックス取得
					ipClass = ipInfoTable;

					ipClass->get_Fields( &ipFields );

					// VICS_LINK_ID
					LONG lVicsLinkID=0;

					ipFields->FindField( CComBSTR(_T("VICS_LINK_ID")), &lVicsLinkID );

					while ( ipCursor->NextRow( &ipRow ) == S_OK ) {
						if ( !ipRow )
							break;

						// OID
						long	OID=0;

						ipRow->get_OID( &OID );

						// VICS_LINK_ID
						CComVariant vaValue;

						ipRow->get_Value( lVicsLinkID, &vaValue );

						// 格納
						m_mapINF[OID] = vaValue.lVal;
					}
				}
			}

			for ( std::map<LONG, LONG>::const_iterator itInf = m_mapINF.begin(); itInf != m_mapINF.end(); ++itInf ) {
				// 除外VICS_LINK_IDチェック
				if ( pvecVICSID ) {
					bool bIs = false;

					for ( std::vector<LONG>::const_iterator itID = pvecVICSID->begin(); itID != pvecVICSID->end(); ++itID ) {
						if ( (*itID) == itInf->first ) {
							bIs = true;
							break;
						}
					}

					if ( bIs ) 
						continue;
				}

				// リンク列ソート
				LQItemsSort(m_mapInfos[lDrawMode].mapInfo[itInf->first]);

				// [Bug 6518]VICSを両方向に作成すると落ちる
				// 保持していなかった道路形状を取得する。
				long i = 0;
				strIDs.Empty();
				for ( std::list<LONG>::const_iterator itRoad = listRoad.begin(); itRoad != listRoad.end(); ++itRoad, ++i ) {
					if ( strIDs.IsEmpty() )
						strIDs.Format( _T("%d"), (*itRoad) );
					else {
						CString strID;

						strID.Format( _T(",%d"), (*itRoad) );
						strIDs += strID;

						if ( i >= 999 ) {
							i = 0;

							GetRoadShapes( lDrawMode, strIDs );
						}
					}
				}

				if ( !strIDs.IsEmpty() )
					GetRoadShapes( lDrawMode, strIDs );

				// [バグ 6518] [e]VICSを両方向に作成すると落ちる
				// ここでチェックに必要なLQだけを抽出しておく
				std::list<LQITEM> listLQs;
				for ( std::list<LQITEM>::const_iterator itLQ = m_mapInfos[lDrawMode].mapInfo[itInf->first].begin(); itLQ != m_mapInfos[lDrawMode].mapInfo[itInf->first].end(); ++itLQ )
				{
					for ( CTargetItems::const_iterator it = cRoads.begin(); it != cRoads.end(); ++it ) {
						if ( itLQ->LinkID == (*it).GetOID() )
						{
							listLQs.push_back( *itLQ );
							break;
						}
					}
				}

				if ( listLQs.size() >= 1 ) {
					// チェック
					if ( !CheckVicsHitDir( cRoadItem, &cRoads, ipBaseFromPoint, ipBaseToPoint, ipLQGeometry, &listLQs ) )
						return DispErrorMessage( sindyeCheckErrorCode_NoChange_VICS, lDrawMode );
				}
			}
		}
	}

	return TRUE;
}



BOOL CLQHelper::CheckVicsHitDir( const CTargetItem& cRoadItem, CTargetItems* pcRoads, IPointPtr ipBaseFromPoint, IPointPtr ipBaseToPoint, IGeometryPtr ipLQGeometry, std::list<LQITEM>* lstLQ)
{
	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( cRoadItem.GetShape() );
	IRelationalOperatorPtr ipRel = GetOffset( ipLQGeometry, ipBaseFromPoint, lstLQ->size() == 1 );

	if ( !ipRel ) {
		ATLTRACE(_T("おかしい %d\n"), lstLQ->size());
		return TRUE;
	}

	if ( lstLQ->size() == 1 ) {
		std::list<LQITEM>::const_iterator itLQ = lstLQ->begin();

		// LQの形状取得
		IGeometryPtr ipGeometry;

		for ( CTargetItems::const_iterator it = pcRoads->begin(); it != pcRoads->end(); ++it ) {
			if ( itLQ->LinkID == (*it).GetOID() ) {
				ipGeometry = AheGetClone( (*it).GetShape() );
				if ( itLQ->Direction == 2 )
					((IPolylinePtr)ipGeometry)->ReverseOrientation();

				break;
			}
		}

		if ( ipGeometry ) {
			IPointPtr ipFromPoint;

			((IPolylinePtr)ipGeometry)->get_FromPoint( &ipFromPoint );

			IPointPtr ipToPoint;

			((IPolylinePtr)ipGeometry)->get_ToPoint( &ipToPoint );

			// 移動/作成途中のリンク列形状のFrom/Toに一致したら除外
			if ( (AheIsEqual( ipBaseFromPoint, ipFromPoint ) || AheIsEqual( ipBaseFromPoint, ipToPoint )) || (AheIsEqual( ipBaseToPoint, ipFromPoint ) || AheIsEqual( ipBaseToPoint, ipToPoint )) )
				return TRUE;

			// [bug 8441] [e][要望]リンク列移動ツールで延長できない場所がある
			// 比較対象となる2つのオブジェクトに対して、ライン方向、左側へオフセットし、
			// そのラインがDisjointであるかどうかで、VICSが作成できるか判断している(bug2025参照)
			// 道路リンクが長く、途中で立体交差している場合に、Disjointではなくなる
			_ISegmentPtr ipSeg;
			ISegmentCollectionPtr ipSegCol( ipGeometry );
			if( !AheIsDisjoint( ipLQGeometry, ipFromPoint, ipSpRef ) )
			{
				// 比較セグメントはFrom側
				ipSegCol->get_Segment( 0, &ipSeg );
			}
			else if( !AheIsDisjoint( ipLQGeometry, ipToPoint, ipSpRef ) )
			{
				// 比較セグメントはTo側
				ipSegCol->get_Segment( -1, &ipSeg );
			}
			else
				ATLTRACE(_T("おかしい 始点でも終点でもLQGeometryと接触していない\n"));

			if( ipSeg )
			{
				// [Bug 9615][e]VICS交差点が作成できてしまう 
				// Arc10.1からのバグかもしれない。
				// GetOffsetの中でipSegをIPolylineにクエリインターフェイスしているが、それはできない
				// そのおかげで、帰ってくるオフセットラインはNULLってはいないものの、形状がないラインだった
				IGeometryPtr ipSegmentGeom = ToPolyline( ipSeg );
				if( !AheIsDisjoint( ipRel, GetOffset( ipSegmentGeom, ipFromPoint, true ), ipSpRef ) )
					return FALSE;
			}
			else
				ATLTRACE(_T("おかしい セグメントが取得できなかった\n"));
		}
	}else if ( lstLQ->size() >= 2 ) {
		// [バグ 6518] [e]VICSを両方向に作成すると落ちる
		// 2つ以上のLQが入っているのでループでまわしてチェック
		std::list<LQITEM>::const_iterator itLQ = lstLQ->begin();

		for ( ; itLQ != lstLQ->end(); ++itLQ )
		{
			std::list<LQITEM>::const_iterator itLQ2 = itLQ;
			++itLQ2;
			if ( itLQ2 == lstLQ->end() )
				break;

			if ( itLQ->Sequence + 1 != itLQ2->Sequence && itLQ->Sequence - 1 != itLQ2->Sequence )
				return TRUE;

			// 選択したLQの形状取得
			IGeometryPtr ipGeometry, ipGeometry2;

			for ( CTargetItems::const_iterator it = pcRoads->begin(); it != pcRoads->end(); ++it ) {
				if ( itLQ->LinkID == (*it).GetOID() ) {
					ipGeometry = AheGetClone( (*it).GetShape() );
					if ( itLQ->Direction == 2 )
						((IPolylinePtr)ipGeometry)->ReverseOrientation();
				} else if ( itLQ2->LinkID == (*it).GetOID() ) {
					ipGeometry2 = AheGetClone( (*it).GetShape() );
					if ( itLQ2->Direction == 2 )
						((IPolylinePtr)ipGeometry2)->ReverseOrientation();
				}
			}

			if ( ipGeometry && ipGeometry2 ) {
				IPolylinePtr ipPolyline;
				IPointPtr ipStartPoint;

				if ( itLQ->Sequence < itLQ2->Sequence ) {
					((IPolylinePtr)ipGeometry)->get_FromPoint( &ipStartPoint );
					AheSnapToSpatialReference( ipStartPoint, ipSpRef );

					ipPolyline = MeargePolyline( ipGeometry, ipGeometry2, ipStartPoint );
				} else if ( itLQ->Sequence > itLQ2->Sequence ) {
					((IPolylinePtr)ipGeometry2)->get_FromPoint( &ipStartPoint );
					AheSnapToSpatialReference( ipStartPoint, ipSpRef );

					ipPolyline = MeargePolyline( ipGeometry2, ipGeometry, ipStartPoint );
				}

				if ( !ipPolyline )
					return TRUE;

				if ( ! AheIsDisjoint( ipRel, GetOffset( ipPolyline, ipStartPoint, true ), ipSpRef ) )
					return FALSE;
			}

			++itLQ;
		}
	} else {
		ATLTRACE(_T("おかしい %d\n"), lstLQ->size());
	}

	return TRUE;
}

IGeometryPtr CLQHelper::GetOffset(IGeometryPtr ipGeometry, IPointPtr ipBasePonit, bool bChk/*=false*/)
{
	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipGeometry );
	IConstructCurvePtr ipConstructCurve(CLSID_Polyline);

	((IGeometryPtr)ipConstructCurve)->putref_SpatialReference( ipSpRef ); // 空間参照付与

	// オフセット
	CComVariant vaOffsetHow( (LONG)esriConstructOffsetMitered );
	// [バグ 6518] [e]VICSを両方向に作成すると落ちる
	// オフセットに差をつけないと、接している角度によってうまくいかない
	ipConstructCurve->ConstructOffset( (IPolylinePtr)ipGeometry, -( AheGetXYDomain1UnitLength( ipSpRef ) * (bChk ? 10.0f : 8.0f) ), &vaOffsetHow );

	return ipConstructCurve;
}

//////////////////////////////////////////////////////////////////////
// エラー
//////////////////////////////////////////////////////////////////////
BOOL CLQHelper::DispErrorMessage(sindyeCheckErrorCode ErrorCode, LONG lDrawMode, BOOL bIsType/*=FALSE*/, LPCTSTR lpcszConMsg/*=NULL*/)
{
	// エラー取得
	sindyeCheckErrorType ErrorType = GetError( ErrorCode );
	CString strErrMsg;

	strErrMsg.Format( ErrorType.Message, GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoAliasName ), lpcszConMsg );

	if ( ErrorType.MsgType == 1 ) {
		if ( bIsType == FALSE ) {
			CString strMsg = m_mapMsg[lDrawMode];

			if ( !strMsg.IsEmpty() )
				strMsg += _T("\n");
			strMsg += strErrMsg;
			strMsg += _T("\n");

			m_mapMsg[lDrawMode] = strMsg;
		} else {
			if ( m_bIsMessageBox ) {
				if ( AheMessageBox( m_hWnd, strErrMsg + CONFIRM_CONTINUE, ERROR_LQ_RESTRICTION_CHECK, ErrorType.Type, ErrorType.Level ) == IDCANCEL )
					return FALSE;
			}
		}
	} else if ( ErrorType.MsgType == 2 ) {
		if ( !m_CheckItem.bEditEnd )
			m_strErrorMessage = m_strErrorMessage + strErrMsg + _T("\n");

		if ( m_bIsMessageBox && !m_bIsNoMessage )
			AheMessageBox( m_hWnd, strErrMsg, ERROR_LQ_RESTRICTION_CHECK, ErrorType.Type, ErrorType.Level );

		return FALSE;
	} else {
		if ( !m_CheckItem.bEditEnd )
			m_strErrorMessage = m_strErrorMessage + strErrMsg + _T("\n");

		if ( m_bIsMessageBox && !m_bIsNoMessage )
			AheMessageBox( m_hWnd, strErrMsg, ERROR_LQ_RESTRICTION_CHECK, ErrorType.Type, ErrorType.Level );

		return FALSE;
	}

	return TRUE;
}

BOOL CLQHelper::DispErrorMessage(sindyeCheckErrorCode ErrorCode, LONG lDrawMode, LONG CheckDrawMode, LONG OID, LONG InfoID, BOOL bIsType/*=FALSE*/, LPCTSTR lpcszConMsg/*=NULL*/)
{
	// エラー取得
	sindyeCheckErrorType ErrorType = GetError( ErrorCode );
	CString strErrMsg;

	strErrMsg.Format( ErrorType.Message, GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoAliasName ), GetLoadQueueInfo( CheckDrawMode, sindyeLoadQueueInfoAliasName ), OID, GetLoadQueueInfo( CheckDrawMode, sindyeLoadQueueInfoAliasName ), InfoID, lpcszConMsg );

	if ( ErrorType.MsgType == 1 ) {
		if ( bIsType == FALSE ) {
			CString strMsg = m_mapMsg[lDrawMode];

			if ( !strMsg.IsEmpty() )
				strMsg += _T("\n");
			strMsg += strErrMsg;
			strMsg += _T("\n");

			m_mapMsg[lDrawMode] = strMsg;
		} else {
			if ( m_bIsMessageBox ) {
				if ( AheMessageBox( m_hWnd, strErrMsg + CONFIRM_CONTINUE, ERROR_LQ_RESTRICTION_CHECK, ErrorType.Type, ErrorType.Level ) == IDCANCEL )
					return FALSE;
			}
		}
	} else if ( ErrorType.MsgType == 2 ) {
		if ( !m_CheckItem.bEditEnd )
			m_strErrorMessage = m_strErrorMessage + strErrMsg + _T("\n");

		if ( m_bIsMessageBox && !m_bIsNoMessage )
			AheMessageBox( m_hWnd, strErrMsg, ERROR_LQ_RESTRICTION_CHECK, ErrorType.Type, ErrorType.Level );
		return FALSE;
	} else {
		if ( !m_CheckItem.bEditEnd )
			m_strErrorMessage = m_strErrorMessage + strErrMsg + _T("\n");

		if ( m_bIsMessageBox && !m_bIsNoMessage )
			AheMessageBox( m_hWnd, strErrMsg, ERROR_LQ_RESTRICTION_CHECK, ErrorType.Type, ErrorType.Level );
		return FALSE;
	}

	return TRUE;
}

BOOL CLQHelper::DispErrorMessage(sindyeCheckErrorCode ErrorCode, LONG lDrawMode, LONG OID, BOOL bIsType/*=FALSE*/, LPCTSTR lpcszConMsg/*=NULL*/)
{
	// エラー取得
	sindyeCheckErrorType ErrorType = GetError( ErrorCode );
	CString strErrMsg;

	strErrMsg.Format( ErrorType.Message, GetLoadQueueInfo( lDrawMode, sindyeLoadQueueInfoAliasName ), OID, lpcszConMsg );

	if ( ErrorType.MsgType == 1 ) {
		if ( bIsType == FALSE ) {
			CString strMsg = m_mapMsg[lDrawMode];

			if ( !strMsg.IsEmpty() )
				strMsg += _T("\n");
			strMsg += strErrMsg;
			strMsg += _T("\n");

			m_mapMsg[lDrawMode] = strMsg;
		} else {
			if ( m_bIsMessageBox ) {
				if ( AheMessageBox( m_hWnd, strErrMsg + CONFIRM_CONTINUE, ERROR_LQ_RESTRICTION_CHECK, ErrorType.Type, ErrorType.Level ) == IDCANCEL )
					return FALSE;
			}
		}
	} else if ( ErrorType.MsgType == 2 ) {
		if ( !m_CheckItem.bEditEnd )
			m_strErrorMessage = m_strErrorMessage + strErrMsg + _T("\n");

		if ( m_bIsMessageBox && !m_bIsNoMessage )
			AheMessageBox( m_hWnd, strErrMsg, ERROR_LQ_RESTRICTION_CHECK, ErrorType.Type, ErrorType.Level );
		return FALSE;
	} else {
		if ( !m_CheckItem.bEditEnd )
			m_strErrorMessage = m_strErrorMessage + strErrMsg + _T("\n");

		if ( m_bIsMessageBox && !m_bIsNoMessage )
			AheMessageBox( m_hWnd, strErrMsg, ERROR_LQ_RESTRICTION_CHECK, ErrorType.Type, ErrorType.Level );
		return FALSE;
	}

	return TRUE;
}

BOOL CLQHelper::IsEqualByPoints( IGeometryPtr ipSourceGeom, IGeometryPtr ipDestGeom, ISpatialReferencePtr ipSpRef/*=NULL*/, BOOL bDir/*=FALSE*/ )
{
	IGeometryPtr ipSourceGeomCopy( AheGetClone( ipSourceGeom ) );
	IGeometryPtr ipDestGeomCopy( AheGetClone( ipDestGeom ) );

	if ( ipSpRef ) {
		AheSnapToSpatialReference( ipSourceGeomCopy, ipSpRef );
		AheSnapToSpatialReference( ipDestGeomCopy, ipSpRef );
	}

	// 空間参照が違う場合は判定がちゃんとできないので、
	// 無理やり細かいほうの空間参照をセットする（点列には影響ないはず）
	if ( !AheHasSameSpatialReference( ipSourceGeomCopy, ipDestGeomCopy ) ) {
		ISpatialReferencePtr ipMinimumSpRef = AheGetMinimumSpatialReference( AheGetSpatialReference( ipSourceGeomCopy ), AheGetSpatialReference( ipDestGeomCopy ) );
		ipSourceGeomCopy->putref_SpatialReference( ipMinimumSpRef );
		ipDestGeomCopy->putref_SpatialReference( ipMinimumSpRef );
	}

	LONG lSourceCount=0, lDestCount=0;

	((IPointCollectionPtr)ipSourceGeomCopy)->get_PointCount( &lSourceCount );
	((IPointCollectionPtr)ipDestGeomCopy)->get_PointCount( &lDestCount );

	if ( lSourceCount != lDestCount )
		return FALSE;

#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
	IPointCollectionPtr ipSourcePointColl( ipSourceGeomCopy ), ipDestPointColl( ipDestGeomCopy );
	BOOL TorF = TRUE;

	for ( LONG i = 0; i < lSourceCount; ++i ) {
		IPointPtr ipPointSrc, ipPointDest;
		ipSourcePointColl->get_Point( i, &ipPointSrc );
		ipDestPointColl->get_Point( i, &ipPointDest );

		if ( ! AheIsEqual( ipPointSrc, ipPointDest ) )
		{
			TorF = FALSE;
			break;
		}
	}

	if ( bDir ) {
		TorF = TRUE;
		for ( LONG i = lSourceCount - 1; i >= 0; --i ) {
			IPointPtr ipPointSrc, ipPointDest;
			ipSourcePointColl->get_Point( i, &ipPointSrc );
			ipDestPointColl->get_Point( i, &ipPointDest );

			if ( ! AheIsEqual( ipPointSrc, ipPointDest ) )
			{
				TorF = FALSE;
				break;
			}
		}
	}

#else
	WKSPoint* pSourcePoints = new WKSPoint[lSourceCount];
	WKSPoint* pDestPoints = new WKSPoint[lDestCount];

	((IPointCollectionPtr)ipSourceGeomCopy)->QueryWKSPoints( 0, lSourceCount, pSourcePoints );
	((IPointCollectionPtr)ipDestGeomCopy)->QueryWKSPoints( 0, lDestCount, pDestPoints );

	BOOL TorF = TRUE;

	for ( LONG i = 0; i < lSourceCount; ++i ) {
		if ( pSourcePoints[i].X != pDestPoints[i].X || pSourcePoints[i].Y != pDestPoints[i].Y ) {
			TorF = FALSE;
			break;
		}
	}

	if ( bDir ) {
		TorF = TRUE;
		for ( LONG i = lSourceCount - 1; i >= 0; --i ) {
			if ( pSourcePoints[i].X != pDestPoints[i].X || pSourcePoints[i].Y != pDestPoints[i].Y ) {
				TorF = FALSE;
				break;
			}
		}
	}

	delete [] pSourcePoints;
	delete [] pDestPoints;
#endif

	return TorF;
};

BOOL CLQHelper::IsEqualDirectionByPoints( IGeometryPtr ipSourceGeom, IGeometryPtr ipDestGeom, BOOL bIsCheckFrom/*=FALSE*/, ISpatialReferencePtr ipSpRef/*=NULL*/ )
{
	IGeometryPtr ipSourceGeomCopy( AheGetClone( ipSourceGeom ) );
	IGeometryPtr ipDestGeomCopy( AheGetClone( ipDestGeom ) );

	if ( ipSpRef ) {
		AheSnapToSpatialReference( ipSourceGeomCopy, ipSpRef );
		AheSnapToSpatialReference( ipDestGeomCopy, ipSpRef );
	}

	// 空間参照が違う場合は判定がちゃんとできないので、
	// 無理やり細かいほうの空間参照をセットする（点列には影響ないはず）
	if ( !AheHasSameSpatialReference( ipSourceGeomCopy, ipDestGeomCopy ) ) {
		ISpatialReferencePtr ipMinimumSpRef = AheGetMinimumSpatialReference( AheGetSpatialReference( ipSourceGeomCopy ), AheGetSpatialReference( ipDestGeomCopy ) );
		ipSourceGeomCopy->putref_SpatialReference( ipMinimumSpRef );
		ipDestGeomCopy->putref_SpatialReference( ipMinimumSpRef );
	}

	LONG lSourceCount=0, lDestCount=0;

	((IPointCollectionPtr)ipSourceGeomCopy)->get_PointCount( &lSourceCount );
	((IPointCollectionPtr)ipDestGeomCopy)->get_PointCount( &lDestCount );

#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
	IPointCollectionPtr ipSourcePointColl( ipSourceGeomCopy ), ipDestPointColl( ipDestGeomCopy );
	LONG lFirst=0, lSecond=0;

	if ( bIsCheckFrom ) {
		lFirst = 1;
		lSecond = 0;
	} else {
		lFirst = lDestCount - 1;
		lSecond = lDestCount - 2;
	}

	// 一致点Indexが２つないってことは包含されていないから、方向も違うでいいんじゃない。
	std::list<long> listHitIdxs;

	for ( long i = 0; i < lSourceCount; ++i )
	{
		IPointPtr ipPointSrc, ipPointDestFirst, ipPointDestSecond;
		ipSourcePointColl->get_Point( i, &ipPointSrc );
		ipDestPointColl->get_Point( lFirst, &ipPointDestFirst );
		ipDestPointColl->get_Point( lSecond, &ipPointDestSecond );

		if ( AheIsEqual( ipPointSrc, ipPointDestFirst ) || AheIsEqual( ipPointSrc, ipPointDestSecond ) )
			listHitIdxs.push_back( i );
	}

	BOOL bRet = FALSE;

	if ( ! listHitIdxs.empty() )
	{
		listHitIdxs.sort();

		if ( listHitIdxs.size() >= 3 )	// ループ時 or 重複してる
		{
			// 始終点Index以外の重複点Indexを除く
			for ( std::list<long>::iterator it = listHitIdxs.begin(); it != listHitIdxs.end(); )
			{
				bool bErase = false;

				if ( *it != 0 && *it != lSourceCount-1 )
				{
					IPointPtr ipPointSrc;
					ipSourcePointColl->get_Point( *it, &ipPointSrc );

					for ( std::list<long>::iterator it2 = it; it2 != listHitIdxs.end(); ++it2 )
					{
						if ( it != it2 )
						{
							IPointPtr ipPointSrc2;
							ipSourcePointColl->get_Point( *it2, &ipPointSrc2 );

							if ( AheIsEqual( ipPointSrc, ipPointSrc2 ) )
							{
								bErase = true;
								break;
							}
						}
					}
				}

				if ( bErase )
					it = listHitIdxs.erase( it );
				else
					++it;
			}

			// ループ？
			IPointPtr ipPointSrc, ipPointSrc2;
			ipSourcePointColl->get_Point( 0, &ipPointSrc );
			ipSourcePointColl->get_Point( lSourceCount-1, &ipPointSrc2 );

			if ( AheIsEqual( ipPointSrc, ipPointSrc2 ) )
			{
				if ( listHitIdxs.size() == 3 )
				{
					// 始終点Index以外のIndexを取得
					long lHit = -1;

					for ( std::list<long>::iterator it = listHitIdxs.begin(); it != listHitIdxs.end(); ++it )
					{
						if ( *it != 0 && *it != lSourceCount-1 )
						{
							lHit = *it;
							break;
						}
					}

					// 始終点Index以外のIndexが、始点Index･終点Indexどちらに近いかチェック
					std::list<long>::iterator it = listHitIdxs.begin();

					if ( lHit <= (lSourceCount-1) - lHit )
					{
						// 始点Indexに近い時は、終点Indexを除く
						it = listHitIdxs.end(); --it;
						listHitIdxs.erase( it );
					}
					else
					{
						// 終点Indexに近い時は、始点Indexを除く
						listHitIdxs.erase( it );
					}
				}
			}
		}

		if ( listHitIdxs.size() == 2 )
		{
			IPointPtr ipPointSrc, ipPointDestSecond;
			ipSourcePointColl->get_Point( *listHitIdxs.begin(), &ipPointSrc );
			ipDestPointColl->get_Point( lSecond, &ipPointDestSecond );

			if ( AheIsEqual( ipPointSrc, ipPointDestSecond ) )
				bRet = TRUE;
		}
	}
#else
	WKSPoint* pSourcePoints = new WKSPoint[lSourceCount];
	WKSPoint* pDestPoints = new WKSPoint[lDestCount];

	((IPointCollectionPtr)ipSourceGeomCopy)->QueryWKSPoints( 0, lSourceCount, pSourcePoints );
	((IPointCollectionPtr)ipDestGeomCopy)->QueryWKSPoints( 0, lDestCount, pDestPoints );

	LONG lFirst=0, lSecond=0;

	if ( bIsCheckFrom ) {
		lFirst = 1;
		lSecond = 0;
	} else {
		lFirst = lDestCount - 1;
		lSecond = lDestCount - 2;
	}

	// 一致点Indexが２つないってことは包含されていないから、方向も違うでいいんじゃない。
	std::list<long> listHitIdxs;

	for ( long i = 0; i < lSourceCount; ++i )
	{
		if( ( pSourcePoints[i].X == pDestPoints[lFirst].X && pSourcePoints[i].Y == pDestPoints[lFirst].Y ) ||
			( pSourcePoints[i].X == pDestPoints[lSecond].X && pSourcePoints[i].Y == pDestPoints[lSecond].Y ) )
			listHitIdxs.push_back( i );
	}

	BOOL bRet = FALSE;

	if ( ! listHitIdxs.empty() )
	{
		listHitIdxs.sort();

		if ( listHitIdxs.size() >= 3 )	// ループ時 or 重複してる
		{
			// 始終点Index以外の重複点Indexを除く
			for ( std::list<long>::iterator it = listHitIdxs.begin(); it != listHitIdxs.end(); )
			{
				bool bErase = false;

				if ( *it != 0 && *it != lSourceCount-1 )
				{
					for ( std::list<long>::iterator it2 = it; it2 != listHitIdxs.end(); ++it2 )
					{
						if ( it != it2 )
						{
							if ( pSourcePoints[*it].X == pSourcePoints[*it2].X && pSourcePoints[*it].Y == pSourcePoints[*it2].Y )
							{
								bErase = true;
								break;
							}
						}
					}
				}

				if ( bErase )
					it = listHitIdxs.erase( it );
				else
					++it;
			}

			// ループ？
			if(pSourcePoints[0].X == pSourcePoints[lSourceCount-1].X && pSourcePoints[0].Y == pSourcePoints[lSourceCount-1].Y)
			{
				if ( listHitIdxs.size() == 3 )
				{
					// 始終点Index以外のIndexを取得
					long lHit = -1;

					for ( std::list<long>::iterator it = listHitIdxs.begin(); it != listHitIdxs.end(); ++it )
					{
						if ( *it != 0 && *it != lSourceCount-1 )
						{
							lHit = *it;
							break;
						}
					}

					// 始終点Index以外のIndexが、始点Index･終点Indexどちらに近いかチェック
					std::list<long>::iterator it = listHitIdxs.begin();

					if ( lHit <= (lSourceCount-1) - lHit )
					{
						// 始点Indexに近い時は、終点Indexを除く
						it = listHitIdxs.end(); --it;
						listHitIdxs.erase( it );
					}
					else
					{
						// 終点Indexに近い時は、始点Indexを除く
						listHitIdxs.erase( it );
					}
				}
			}
		}

		if ( listHitIdxs.size() == 2 )
		{
			if ( pSourcePoints[*listHitIdxs.begin()].X == pDestPoints[lSecond].X && pSourcePoints[*listHitIdxs.begin()].Y == pDestPoints[lSecond].Y )
				bRet = TRUE;
		}
	}

	delete [] pSourcePoints;
	delete [] pDestPoints;
#endif

	return bRet;
}

long CLQHelper::IsIncluedByPoints( IGeometryPtr ipSourceGeom, IGeometryPtr ipDestGeom, ISpatialReferencePtr ipSpRef/*=NULL*/ )
{
	IGeometryPtr ipSourceGeomCopy( AheGetClone( ipSourceGeom ) );
	IGeometryPtr ipDestGeomCopy( AheGetClone( ipDestGeom ) );

	if ( ipSpRef ) {
		AheSnapToSpatialReference( ipSourceGeomCopy, ipSpRef );
		AheSnapToSpatialReference( ipDestGeomCopy, ipSpRef );
	}

	// 空間参照が違う場合は判定がちゃんとできないので、
	// 無理やり細かいほうの空間参照をセットする（点列には影響ないはず）
	if ( !AheHasSameSpatialReference( ipSourceGeomCopy, ipDestGeomCopy ) ) {
		ISpatialReferencePtr ipMinimumSpRef = AheGetMinimumSpatialReference( AheGetSpatialReference( ipSourceGeomCopy ), AheGetSpatialReference( ipDestGeomCopy ) );
		ipSourceGeomCopy->putref_SpatialReference( ipMinimumSpRef );
		ipDestGeomCopy->putref_SpatialReference( ipMinimumSpRef );
	}

	ITopologicalOperator2Ptr ipTopo = ipDestGeomCopy;
	IGeometryPtr ipIntersectGeom;

	if ( ipTopo )
		ipTopo->Intersect( ipSourceGeomCopy, esriGeometry1Dimension, &ipIntersectGeom );

	if ( !ipIntersectGeom )
		return 0;	// 包含なし

	long lIntersectCount = 0;

	((IPointCollectionPtr)ipIntersectGeom)->get_PointCount( &lIntersectCount );
	if ( lIntersectCount == 0 )
		return 0;	// 包含なし

	long lDestCount = 0;

	((IPointCollectionPtr)ipDestGeomCopy)->get_PointCount( &lDestCount );

	if ( lDestCount <= lIntersectCount )
	{
		// [Bug 6781] [e]右左折禁止の包含チェックが期待通り動作しない
		// 複数の形状が返ってきた時は完全包含ではないはず。。。
		IGeometryCollectionPtr ipGeomColl( ipIntersectGeom );
		long lGCount = -1;

		ipGeomColl->get_GeometryCount( &lGCount );

		if ( lGCount == 1 )
			return 2;	// 完全包含
	}

	return 1;	// 部分包含
}

std::list<LONG> CLQHelper::GetContiguityMeshCodes()
{
	std::list<LONG> listMesh;

	// 隣接8メッシュ取得
	IDocumentPtr ipDoc;

	m_ipApp->get_Document( &ipDoc );

	IMxDocumentPtr ipMxDoc = ipDoc;
	if ( ipMxDoc ) {
		IActiveViewPtr ipAct;

		ipMxDoc->get_ActiveView( &ipAct );
		if ( ipMxDoc ) {
			IEnvelopePtr ipEnv;

			ipAct->get_Extent( &ipEnv );

			IAreaPtr ipArea = ipEnv;

			if ( ipArea ) {
				IPointPtr ipCPoint;

				ipArea->get_Centroid( &ipCPoint );

				CMesh cMesh;

				LONG nMesh = cMesh.GetMeshCode( ipCPoint );

				listMesh.push_front( nMesh );
				listMesh.push_front( AheGETMESHCODE( nMesh, -1, 1 ) );
				listMesh.push_front( AheGETMESHCODE( nMesh, -1, 0 ) );
				listMesh.push_front( AheGETMESHCODE( nMesh, -1, -1 ) );
				listMesh.push_front( AheGETMESHCODE( nMesh, 0, 1 ) );
				listMesh.push_front( AheGETMESHCODE( nMesh, 0, -1 ) );
				listMesh.push_front( AheGETMESHCODE( nMesh, 1, 1 ) );
				listMesh.push_front( AheGETMESHCODE( nMesh, 1, 0 ) );
				listMesh.push_front( AheGETMESHCODE( nMesh, 1, -1 ) );

				listMesh.sort();
				listMesh.unique();
			}
		}
	}

	return listMesh;
}

CString CLQHelper::GetContiguityMeshCode()
{
	std::list<LONG> listMesh = GetContiguityMeshCodes();
	CString strMeshCodes;

	for ( std::list<LONG>::const_iterator itMesh = listMesh.begin(); itMesh != listMesh.end(); ++itMesh ) {
		if ( strMeshCodes.IsEmpty() ) {
			strMeshCodes.Format( _T("MESHCODE IN (%d"), (*itMesh) );
		} else {
			CString strMeshCode;

			strMeshCode.Format( _T(",%d"), (*itMesh) );
			strMeshCodes += strMeshCode;
		}
	}

	strMeshCodes += _T(")");
	return strMeshCodes;
}

IPolylinePtr CLQHelper::MeargePolyline(IPolylinePtr ipSourceGeom, IPolylinePtr ipDestGeom, IPointPtr ipStartPoint)
{
	if ( !ipSourceGeom )
		return ipDestGeom;

	if ( !ipDestGeom )
		return ipSourceGeom;

	// 空間参照取得
	ISpatialReferencePtr ipSpRef;

	ipSourceGeom->get_SpatialReference( &ipSpRef );

	// 現在のポイント数取得
	LONG lBasePointCount = 0;
	((IPointCollectionPtr)ipSourceGeom)->get_PointCount( &lBasePointCount );

	// 道路のポイント数取得
	LONG lAddPointCount = 0;
	((IPointCollectionPtr)ipDestGeom)->get_PointCount( &lAddPointCount );

	// 作成ポイント群
	WKSPoint* pPoints = new WKSPoint[lBasePointCount + lAddPointCount];

	// 現在のポイント取得
	((IPointCollectionPtr)ipSourceGeom)->QueryWKSPoints( 0, lBasePointCount, pPoints );

	// 道路のポイント取得
	((IPointCollectionPtr)ipDestGeom)->QueryWKSPoints( 1, lAddPointCount-1, &(pPoints[lBasePointCount]) );

	// 返すポリライン作成
	IPointCollectionPtr ipNewPolyline( CLSID_Polyline );

	((IGeometryPtr)ipNewPolyline)->putref_SpatialReference( ipSpRef ); // 空間参照付与
	ipNewPolyline->SetWKSPoints( lBasePointCount + lAddPointCount - 1, pPoints );

	delete [] pPoints;

	((IGeometryPtr)ipNewPolyline)->SnapToSpatialReference();
	((IPolylinePtr)ipNewPolyline)->SimplifyNetwork();

	// Fromポイント取得
	IPointPtr ipFromPoint;

	((IPolylinePtr)ipNewPolyline)->get_FromPoint( &ipFromPoint );
	AheSnapToSpatialReference( ipFromPoint, ipSpRef );

	// 作成前のFromポイントと比較
	if ( !AheIsEqual( ipStartPoint, ipFromPoint, ipSpRef ) )
		((IPolylinePtr)ipNewPolyline)->ReverseOrientation();

	return ipNewPolyline;
}

void CLQHelper::CreateGeometry( CreateGeomInfo& geominfo, IGeometryPtr ipGeom, IPointPtr ipBaseFromPoint)
{
	if ( !ipGeom )
		return;

	// 空間参照取得
	ISpatialReferencePtr ipSpRef;

	ipGeom->get_SpatialReference( &ipSpRef );

	// Fromポイント取得
	IPointPtr ipFromPoint;

	((IPolylinePtr)ipGeom)->get_FromPoint( &ipFromPoint );
	AheSnapToSpatialReference( ipFromPoint, ipSpRef );

	if ( !geominfo.bFirst ) {
		if ( ipBaseFromPoint ) {
			// ベースのFromポイントと比較
			if ( !AheIsEqual( ipFromPoint, ipBaseFromPoint, ipSpRef ) )
				((IPolylinePtr)ipGeom)->ReverseOrientation();
		}

		// 一番最初のFromポイント取得
		((IPolylinePtr)ipGeom)->get_FromPoint( &geominfo.ipStartPoint );
		AheSnapToSpatialReference( geominfo.ipStartPoint, ipSpRef );

		geominfo.bFirst = TRUE;
	} else {
		// 作成形状のToポイントと比較
		if ( !AheIsEqual( ipFromPoint, geominfo.ipEndToPoint, ipSpRef ) )
			((IPolylinePtr)ipGeom)->ReverseOrientation();
	}

	if ( !geominfo.ipPolyline )
		geominfo.ipPolyline = ipGeom;
	else
		geominfo.ipPolyline = MeargePolyline( geominfo.ipPolyline, ipGeom, geominfo.ipStartPoint );

	// 作成形状のToポイント取得
	if ( geominfo.ipPolyline ) {
		geominfo.ipPolyline->get_ToPoint( &geominfo.ipEndToPoint );
		AheSnapToSpatialReference( geominfo.ipEndToPoint, ipSpRef );
	}
}

void CLQHelper::GetRoadLayer()
{
	if ( m_ipApp ) {
		IDocumentPtr ipDoc;

		m_ipApp->get_Document( &ipDoc );

		IMxDocumentPtr ipMxDoc = ipDoc;

		if ( ipMxDoc ) {
			IMapPtr ipMap;

			ipMxDoc->get_FocusMap( &ipMap );

			m_ipRoadLayer = AheGetRoadLayer( ipMap, TRUE );
			m_ipRoadTable = AheGetFeatureClass( m_ipRoadLayer );
		}
	}
}

BOOL CLQHelper::GetConditionType( ITablePtr ipInfoTable, long InfID, long DrawMode )
{
	BOOL bRet = FALSE;

	if ( ipInfoTable && InfID > 0 ) {
		// 条件判定
		switch ( DrawMode ) {
			case _MULTISYMBOL_NOLRTURN:
				{
					_IRowPtr ipRow;

					ipInfoTable->GetRow( InfID, &ipRow );

					if ( ipRow ) {
						CComVariant vaValue1 = AheGetAttribute( ipRow, _T("DAYOFWEEK1_C") );
						CComVariant vaValue2 = AheGetAttribute( ipRow, _T("DAYOFWEEK2_C") );
						CComVariant vaValue3 = AheGetAttribute( ipRow, _T("DAYOFWEEK3_C") );
						CComVariant vaValue4 = AheGetAttribute( ipRow, _T("DAYOFWEEK4_C") );
						// 条件あり?
						if ( vaValue1.lVal != 0 || vaValue2.lVal != 0 || vaValue3.lVal != 0 || vaValue4.lVal != 0 )
							bRet = TRUE;
					}
				}
				break;
			case _MULTISYMBOL_ROUTE:
				{
					_IRowPtr ipRow;

					ipInfoTable->GetRow( InfID, &ipRow );

					if ( ipRow ) {
						CComVariant vaValue1 = AheGetAttribute( ipRow, _T("ROUTECLASS_C") );
						// 乗り継ぎ道?
						if ( vaValue1.lVal == 2 )
							bRet = TRUE;
					}
				}
				break;
		}
	}

	return bRet;
}

IPolylinePtr CLQHelper::Segment2Polyline(_ISegmentPtr ipSegment, ISpatialReferencePtr ipSpRef)
{
	IPolylinePtr ipNew;

	if ( ipSegment ) {
		ipNew.CreateInstance( CLSID_Polyline );
		ipNew->putref_SpatialReference( ipSpRef ); // 空間参照付与

		IPointPtr ipFromPoint;

		ipSegment->get_FromPoint( &ipFromPoint );

		IPointPtr ipToPoint;

		ipSegment->get_ToPoint( &ipToPoint );

		ipNew->put_FromPoint( ipFromPoint );
		ipNew->put_ToPoint( ipToPoint );
		ipNew->putref_SpatialReference( ipSpRef );
		ipNew->SnapToSpatialReference();
		ipNew->SimplifyNetwork();
	}

	return ipNew;
}

void CLQHelper::GetCheckOperationAction(sindyeCheckType checkcode, long lCheckType, long lDrawMode, ITablePtr ipInfoTable, long lOwnerOID, sindyeCheckOperation* pCheckOPType, sindyeCheckAction* pCheckACTType )
{
	// 条件判定
	BOOL bCheckConditionType = GetConditionType( ipInfoTable, lOwnerOID, lDrawMode );

	// 操作取得
	sindyeCheckConditionOperation CheckOperation = GetCheckOperation( lDrawMode, checkcode, lCheckType );

	// 動作取得
	sindyeCheckConditionAction CheckAction = GetCheckAction( lDrawMode, checkcode, lCheckType );

	// 右左折禁止（条件あり）と乗り継ぎ道はSecondaryを使用する。
	if ( bCheckConditionType ) {
		(*pCheckOPType) = CheckOperation.Secondary;
		(*pCheckACTType) = CheckAction.Secondary;
	} else {
		(*pCheckOPType) = CheckOperation.Primary;
		(*pCheckACTType) = CheckAction.Primary;
	}
}

ITablePtr CLQHelper::GetTable(LPCTSTR lpcszName)
{
	ITablePtr ipTable;

	std::map<CString, CAdapt<ITablePtr>>::const_iterator it = m_mapTables.find( lpcszName );
	if ( it == m_mapTables.end() ) {
		if ( lpcszName && m_ipRoadTable ) {
			ipTable = AheOpenSameOwnerTable( m_ipRoadTable, lpcszName );
			m_mapTables[lpcszName] = ipTable;
		}
	} else
		ipTable = it->second.m_T;

	return ipTable;
}
