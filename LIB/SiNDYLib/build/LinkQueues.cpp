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

// LinkQueue.cpp: CLinkQueues クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LinkQueues.h"
#include "LQDirGuide.h"
#include "LQGuide.h"
#include "LQVics.h"
#include "LQByway.h"
#include "LQTurnreg.h"
#include "LQLane.h"
#include "LQRoute.h"
#include "LQUTurn.h"
#include "LQInternavi.h"
#include "InfoDirGuide.h"
#include "InfoGuide.h"
#include "InfoVics.h"
#include "InfoByway.h"
#include "InfoTurnreg.h"
#include "InfoLane.h"
#include "InfoRoute.h"
#include "InfoUTurn.h"
#include "InfoInternavi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {

CLinkQueues::CLinkQueues()
{

}

CLinkQueues::~CLinkQueues()
{
	for ( std::map<long, CLinkQueue*>::iterator it = m_mapLQs.begin(); it != m_mapLQs.end(); it++ ) {
		if ( (*it).second ) {
			delete (*it).second;
		}
	}
}

errorcode::sindyErrCode CLinkQueues::SelectByRoadID(long lValue, link_queue::lq_kind::ECode eCode)
{
	USES_CONVERSION;

	/// eCode から種別を識別（複数可にする？）
	link_queue::CLQ* pcLQ = NULL;
	link_queue::CInfo* pcInf = NULL;

	switch ( eCode ) {
	case link_queue::lq_kind::Dirguide:
		pcLQ = NEW lq_dirguide::CLQDirGuide;
		pcInf = NEW inf_dirguide::CInfoDirGuide;
		break;
	case link_queue::lq_kind::Guide:
		pcLQ = NEW lq_guide::CLQGuide;
		pcInf = NEW inf_guide::CInfoGuide;
		break;
	case link_queue::lq_kind::Vics:
		pcLQ = NEW lq_vics::CLQVics;
		pcInf = NEW inf_vics::CInfoVics;
		break;
	case link_queue::lq_kind::Byway:
		pcLQ = NEW lq_byway::CLQByway;
		pcInf = NEW inf_byway::CInfoByway;
		break;
	case link_queue::lq_kind::Turnreg:
		pcLQ = NEW lq_turnreg::CLQTurnReg;
		pcInf = NEW inf_turnreg::CInfoTurnReg;
		break;
	case link_queue::lq_kind::Lane:
		pcLQ = NEW lq_lane::CLQLane;
		pcInf = NEW inf_lane::CInfoLane;
		break;
	case link_queue::lq_kind::Route:
		pcLQ = NEW lq_route::CLQRoute;
		pcInf = NEW inf_route::CInfoRoute;
		break;
	case link_queue::lq_kind::UTurn:
		pcLQ = NEW lq_uturn::CLQUTurn;
		pcInf = NEW inf_uturn::CInfoUTurn;
		break;
	case link_queue::lq_kind::Internavi:
		pcLQ = NEW lq_internavi::CLQInternavi;
		pcInf = NEW inf_internavi::CInfoInternavi;
		break;
	}

	/// 指定された道路リンクに作成された LQ を取得
	ITablePtr ipTable;
	((IRow*)pcLQ)->get_Table(&ipTable);

	/// クエリフィルタの作成
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);

	CString strWhereClause;
	strWhereClause.Format(_T("LINK_ID = %d"), lValue);
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	
	ICursorPtr ipCursor, ipCursor2;
	CString sTmp, sWhere;

	if ( S_OK == ipTable->Search(ipQueryFilter, VARIANT_FALSE, &ipCursor)) {
		if ( ipCursor ) {
			/// 検索結果を m_mapData に格納
			IRowPtr ipRow;
			long	lngInfID;
			CLQ*	pcLQTmp = NULL;

			while( S_OK == ipCursor->NextRow( (IRow**)pcLQTmp)) {
				if( pcLQTmp ) {
					lngInfID = pcLQTmp->GetInfId();

					sTmp.Format(_T("%d"), lngInfID);
					
					/// INF Table 検索用の WHERE 句を作成
					if ( sWhere.IsEmpty()) {
						sWhere = sTmp;
					} else {
						sWhere = sWhere + _T(",") + sTmp;
					}
				}
			}

			if ( FALSE == sWhere.IsEmpty()) {
				/// INF を検索				
				ITablePtr ipTable2;
				((IRow*)pcInf)->get_Table(&ipTable2);

				ipQueryFilter = NULL;
				ipQueryFilter.CreateInstance(CLSID_QueryFilter);
				ipQueryFilter->put_SubFields(_T("*"));
				ipQueryFilter->put_WhereClause( CComBSTR(sWhere) );

				if ( S_OK == ipTable2->Search(ipQueryFilter, VARIANT_FALSE, &ipCursor2)) {
					CInfo* pcInfTmp = NULL;

					if ( ipCursor2 ) {
						while ( S_OK == (ipCursor2->NextRow((IRow**)pcInfTmp))) {
							if ( pcInfTmp ) {
								/// 取得した情報を CLinkQueue に格納
								CLinkQueue* pcLinkQueue = NEW CLinkQueue;
								pcLinkQueue->m_cInfo = pcInfTmp;

								lngInfID = pcInfTmp->GetOID();

								/// ひとまず格納
								m_mapLQs.insert(std::pair<long, CLinkQueue*>(lngInfID, pcLinkQueue));

								sTmp.Format(_T("%d"), lngInfID);
								
								/// INF Table 検索用の WHERE 句を作成
								if ( sWhere.IsEmpty()) {
									sWhere = sTmp;
								} else {
									sWhere = sWhere + _T(",") + sTmp;
								}
							}
						}
					}				
				}

				if ( FALSE == sWhere.IsEmpty()) {
					/// 再び LQ を検索
					ipQueryFilter = NULL;
					ipQueryFilter.CreateInstance(CLSID_QueryFilter);
					ipQueryFilter->put_SubFields(_T("*"));
					ipQueryFilter->put_WhereClause( CComBSTR(sWhere) );

					/// LQ を検索
					if ( S_OK == ipTable->Search(ipQueryFilter, VARIANT_FALSE, &ipCursor)) {
						IRowPtr ipRow;

						if ( ipCursor ) {
							int i = 0, nInfID = 0;

							while ( S_OK == (ipCursor->NextRow((IRow**)pcLQTmp))) {
								if ( pcLQTmp ) {
									++i;
								
									nInfID = pcLQTmp->GetInfId();

									/// 以前格納したものを取りだす
									std::map<long, CLinkQueue*>::iterator it = m_mapLQs.find(nInfID);

									if ( it != m_mapLQs.end()) {
										(*it).second->m_mapLQ.insert(std::pair<int, link_queue::CLQ*>(i, pcLQTmp));
									} else {
										/// なんでないの？
										ATLASSERT(FALSE);
									}
								}
							}
						}				
					}
				}
			}
		}
	}

	delete pcLQ;
	delete pcInf;
	
	return errorcode::sindyErr_NoErr;
}


}
