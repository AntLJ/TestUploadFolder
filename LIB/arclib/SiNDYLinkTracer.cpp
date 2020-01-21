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

// SiNDYLinkTracer.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <algorithm>
#include "SiNDYLinkTracer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;
using namespace sindy::schema::link_queue;
using namespace sindy::schema::road_link;

// LinkTracer::Turnreg
//////////////////////////////////////////////////////////////////////

LinkTracer::Turnreg::Turnreg()
{
}

LinkTracer::Turnreg::Turnreg(_IRow* ipInfTurnreg)
{
	setInfTurnreg(ipInfTurnreg);
}

void LinkTracer::Turnreg::clear()
{
	m_cLinkIdList.clear();
}

/**
 * @brief オブジェクトIDと、時間規制を設定する
 *
 * @retval true 成功
 * @retval false 引数がNULLだった
 */
bool LinkTracer::Turnreg::setInfTurnreg(_IRow* ipInfTurnreg)
{
	if(! ipInfTurnreg)
		return false;

	return true;
}

// LinkTracer::TurnregMap
//////////////////////////////////////////////////////////////////////

/**
 * @brief 右左折禁止を登録する
 *
 * @param ipInfTurnregCursor [in] 右左折禁止Infカーソル
 * @param ipLqTurnregTable [in] 右左折禁止Lqテーブル
 * @retval true 成功
 * @retval false 失敗
 */
bool LinkTracer::TurnregMap::setTurnreg(_ICursor* ipInfTurnregCursor, ITable* ipLqTurnregTable)
{
	clear();

	if(! (ipInfTurnregCursor && ipLqTurnregTable))
		return false;

	_IRowPtr ipRow;
	while(ipInfTurnregCursor->NextRow(&ipRow) == S_OK) {
		long nObjectId;
		ipRow->get_OID(&nObjectId);
		insert(value_type(nObjectId, Turnreg(ipRow)));
	}

	if(empty())
		return true;

	using namespace sindy::schema::lq_turnreg;

	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	ipQueryFilter->put_SubFields(CComBSTR(kInfID));
	ipQueryFilter->AddField(CComBSTR(kSequence));
	ipQueryFilter->AddField(CComBSTR(kLinkID));

	_ICursorPtr ipCursor;
	if(ipLqTurnregTable->Search(ipQueryFilter, VARIANT_TRUE, &ipCursor) != S_OK || ipCursor == 0)
		return false;

	IFieldsPtr ipFields;
	ipLqTurnregTable->get_Fields(&ipFields);
	if(ipFields == 0)
		return false;

	long nInfIndex;
	long nSequenceIndex;
	long nLinkIdIndex;

	ipFields->FindField(CComBSTR(kInfID), &nInfIndex);
	ipFields->FindField(CComBSTR(kSequence), &nSequenceIndex);
	ipFields->FindField(CComBSTR(kLinkID), &nLinkIdIndex);
	if(nInfIndex < 0 || nSequenceIndex < 0 || nLinkIdIndex < 0)
		return false;

	CComVariant va;

	while(ipCursor->NextRow(&ipRow) == S_OK) {
		ipRow->get_Value(nInfIndex, &va);
		va.ChangeType(VT_I4);

		iterator it = find(va.lVal);
		if(it == end())
			continue;
		Turnreg& rTurnreg = it->second;

		ipRow->get_Value(nSequenceIndex, &va);
		va.ChangeType(VT_I4);
		long nSequence = va.lVal;

		ipRow->get_Value(nLinkIdIndex, &va);
		va.ChangeType(VT_I4);
		long nLinkId = va.lVal;

		if(static_cast<long>(rTurnreg.m_cLinkIdList.size()) < nSequence) {
			rTurnreg.m_cLinkIdList.resize(nSequence);
		}
		rTurnreg.m_cLinkIdList[nSequence - 1] = nLinkId;
	}

	return true;
}


// LinkTracer::Route
//////////////////////////////////////////////////////////////////////

LinkTracer::Route::Route() :
m_nNodeId(0),
m_pLink(0),
m_dDepth(0.0),
m_pPrevRoute(0)
{
}

void LinkTracer::Route::clear()
{
	m_nNodeId = 0;
	m_pLink = 0;
	m_pPrevRoute = 0;
	m_dDepth = 0.0;
}

bool LinkTracer::Route::setRoute(const Route* pPrevRoute, long nNodeId, const Link* pLink)
{
	clear();

	// ノードとリンクが揃っていなければfalse
	if(! (nNodeId && pLink))
		return false;

	// 今まで通ってきた経路の最後のリンクを取得
	const Link* pPrevLink = pPrevRoute ? pPrevRoute->m_pLink : 0;

	// イメージ
	//    <前のリンク>       <対象ノード>       <次のリンク>
	// ------------------------->●------------------------->
	// 
	// 前のリンクから対象ノードへ行けなかったり、次のリンクが対象から入れなければfalse
	if(pPrevLink && ! (pPrevLink->isOutableTo(nNodeId) && pLink->isInableFrom(nNodeId)))
		return false;

	// 前のルートがあるならば
	if(pPrevRoute) {
		// 前のルートの右左禁を取得
		const std::map<const Turnreg*, long>& rPrevTurnregMap = pPrevRoute->m_cTurnregMap;
		// 各右左禁を巡回
		for(std::map<const Turnreg*, long>::const_iterator it = rPrevTurnregMap.begin(); it != rPrevTurnregMap.end(); ++it) {
			const Turnreg* pTurnreg = it->first;
			long nNextCount = it->second + 1;

			// 次のリンクがその右左禁と無関係なら何もしない
			Link::TurnregSet::const_iterator it2 = pLink->m_cTurnregSet.find(pTurnreg);
			if(it2 == pLink->m_cTurnregSet.end())
				continue;

			// 前のルートが保有する右左禁の次のリンクが当該リンクである場合
			if(pTurnreg->m_cLinkIdList[nNextCount] == pLink->m_nObjectId) {
				// 次のリンクが右左禁の最後のリンクでなければ右左禁にリンクを追加するだけ
				if(nNextCount + 1 < static_cast<int>(pTurnreg->m_cLinkIdList.size())) {
					m_cTurnregMap[pTurnreg] = nNextCount;
				}
				else
				{
					// 次のリンクが右左禁の最後のリンクだったらルート追加失敗
					return false;
				}
			}
		}
	}

	// 当該リンクが関わる右左禁を巡回する
	for(Link::TurnregSet::const_iterator it = pLink->m_cTurnregSet.begin(); it != pLink->m_cTurnregSet.end(); ++it) {
		// 右左禁がなかったりリンクIDのリストが空だったりしたらルート追加失敗(例外投げたりした方がよいかもしれない)
		const Turnreg* pTurnreg = *it;
		if(! pTurnreg || pTurnreg->m_cLinkIdList.empty())
			return false;

		// 右左禁のリンクが1本で構成されてたらダメ
		if(pTurnreg->m_cLinkIdList.size() == 1) 
			return false;

		// 右左禁のリンクの最初のリンクが次のリンクだったら
		if(pTurnreg->m_cLinkIdList[0] == pLink->m_nObjectId) {
			// 対象ノードが次のリンクのFromノードであれば、To側のリンク群を取得
			// ToノードであればFrom側の接続リンク群を取得
			// つまり、次のリンクの中間ノードじゃない側、次の次のリンク群を取得
			const Link::ConnectLinkSet& rConnectLinkSet =
				(nNodeId == pLink->m_nFromNodeId) ? pLink->m_cOutLinkSet2 : pLink->m_cOutLinkSet1;

			// 次の次のリンク群を巡回
			for(Link::ConnectLinkSet::const_iterator it2 = rConnectLinkSet.begin(); it2 != rConnectLinkSet.end(); ++it2) {
				// 次の次のリンクが、右左禁の2番目のリンクだった場合に右左禁mapに登録
				const Link* pConnectLink = *it2;
				if(pConnectLink->m_nObjectId == pTurnreg->m_cLinkIdList[1]) {
					m_cTurnregMap.insert(std::map<const Turnreg*, long>::value_type(pTurnreg, 0));
					break;
				}
			}
		}
	}

	// ルートのノードIDは対象ノード、リンクは次のリンク、前のルートを登録
	m_nNodeId = nNodeId;
	m_pLink = pLink;
	m_pPrevRoute = pPrevRoute;

	// 深さは前のルートにリンクのウェイトを加算する
	m_dDepth = (m_pPrevRoute ? m_pPrevRoute->m_dDepth : 0.0) + m_pLink->getWeight(m_pPrevRoute);

	return true;
}

/**
 * @brief 当ルートを構成するすべてのルートの一覧を取得する
 *
 * @note 自分自身も含まれる
 *
 * @param rRouteSet [out] ルート一覧
 */
void LinkTracer::Route::getRouteSet(std::set<const Route*>& rRouteSet) const
{
	for(const Route* pRoute = this; pRoute; pRoute = pRoute->m_pPrevRoute) {
		rRouteSet.insert(pRoute);
	}
}

/**
 * @brief 当ルートを構成する初源のルートを取得する
 *
 * @return 初源のルート。当ルート自身である場合もある。
 */
const LinkTracer::Route* LinkTracer::Route::getFirstRoute() const
{
	const Route* pRoute = 0;
	for(pRoute = this ; pRoute && pRoute->m_pPrevRoute; pRoute = m_pPrevRoute) {
	}

	return pRoute;
}

const LinkTracer::Route* LinkTracer::Route::searchRoute(long nNodeId) const
{
	for(const Route* pRoute = this; pRoute; pRoute = pRoute->m_pPrevRoute) {
		if(pRoute->m_nNodeId == nNodeId)
			return pRoute;
	}

	return 0;
}

void LinkTracer::Route::getLinkIdList(LinkIdList& rLinkIdList) const
{
	for(const Route* pRoute = this; pRoute; pRoute = pRoute->m_pPrevRoute) {
		rLinkIdList.push_back(pRoute->m_pLink->m_nObjectId);
	}

	if(! rLinkIdList.empty()) {
		std::reverse(rLinkIdList.begin(), rLinkIdList.end());
	}
}

void LinkTracer::Route::getNodeIdList(NodeIdList& rNodeIdList) const
{
	for(const Route* pRoute = this; pRoute; pRoute = pRoute->m_pPrevRoute) {
		if(pRoute == this) {
			rNodeIdList.push_back(pRoute->m_pLink->getOppositeNodeId(pRoute->m_nNodeId));
		}
		rNodeIdList.push_back(pRoute->m_nNodeId);
	}

	if(! rNodeIdList.empty()) {
		std::reverse(rNodeIdList.begin(), rNodeIdList.end());
	}
}

bool LinkTracer::Route::getNodeIdList(int nStartNodeId, int nEndNodeId, std::vector<long>& rNodeIdList) const
{
	rNodeIdList.clear();

	bool bEnd = false;
	bool bStart = false;

	std::vector<long> cNodeIdList;
	if(m_nNodeId && m_pLink) {
		long nNodeId = m_pLink->getOppositeNodeId(m_nNodeId);
		if(nNodeId == nEndNodeId) {
			bEnd = true;
			rNodeIdList.push_back(nNodeId);
			if(nNodeId == nStartNodeId) {
				bStart = true;
				rNodeIdList.push_back(nNodeId);
			}
		}
	}

	for(const Route* pRoute = this; pRoute && ! (bEnd && bStart); pRoute = pRoute->m_pPrevRoute) {
		if(bEnd) {
			rNodeIdList.push_back(pRoute->m_nNodeId);
			if(pRoute->m_nNodeId == nStartNodeId) {
				bStart = true;
			}
		}
		else {
			if(pRoute->m_nNodeId == nEndNodeId) {
				bEnd = true;
				rNodeIdList.push_back(pRoute->m_nNodeId);
				if(pRoute->m_nNodeId  == nStartNodeId) {
					bStart = true;
					rNodeIdList.push_back(pRoute->m_nNodeId);
				}
			}
		}
	}

	if(bEnd && bStart) {
		std::reverse(rNodeIdList.begin(), rNodeIdList.end());
		return true;
	}
	else {
		rNodeIdList.clear();
		return false;
	}
}

void LinkTracer::Route::getNodeIdLinkList(NodeIdLinkList& rNodeIdLinkList) const
{
	for(const Route* pRoute = this; pRoute; pRoute = pRoute->m_pPrevRoute) {
		rNodeIdLinkList.push_back(NodeIdLink(pRoute->m_nNodeId, pRoute->m_pLink));
	}

	if(! rNodeIdLinkList.empty()) {
		std::reverse(rNodeIdLinkList.begin(), rNodeIdLinkList.end());
	}
}

IPolylinePtr LinkTracer::Route::createPolyline() const
{
	if(! (m_pLink || m_pPrevRoute))
		return 0;

	IPointCollectionPtr ipPointCol;
	for(const Route* pRoute = this; pRoute; pRoute = pRoute->m_pPrevRoute) {
		long nNodeId = pRoute->m_nNodeId;
		const Link* pLink = pRoute->m_pLink;
		if(! (pLink && nNodeId))
			continue;

		IPointCollectionPtr ipLinkPointCol = pLink->getShape();
		if(ipLinkPointCol == 0)
			continue;

		long nPointCount;
		ipLinkPointCol->get_PointCount(&nPointCount);
		if(nPointCount == 0)
			continue;

		if(pLink->m_nToNodeId == nNodeId) {
			IClonePtr ipClone;
			IClonePtr(ipLinkPointCol)->Clone(&ipClone);
			ipLinkPointCol = ipClone;
			if(ipLinkPointCol == 0)
				continue;

			IPolylinePtr(ipLinkPointCol)->ReverseOrientation();
		}

		if(ipPointCol == 0) {
			IClonePtr ipClone;
			IClonePtr(ipLinkPointCol)->Clone(&ipClone);
			ipPointCol = ipClone;
		}
		else {
			if(nPointCount > 2) {
				ipPointCol->RemovePoints(0, 1);
			}
			ipPointCol->InsertPointCollection(0, ipLinkPointCol);
		}
	}

	return ipPointCol;
}

// LinkTracer::RouteMaster
//////////////////////////////////////////////////////////////////////

LinkTracer::RouteMaster::RouteMaster() :
m_nMaxRouteCount(1)
{
}

LinkTracer::RouteMaster::~RouteMaster()
{
	clear();
}

/**
 * @brief すべてのルートを削除する
 */
void LinkTracer::RouteMaster::clear()
{
	for(iterator it1 = m_cRouteMaster.begin(); it1 != m_cRouteMaster.end(); ++it1) {
		RouteMap& rMap = it1->second;
		for(RouteMap::iterator it2 = rMap.begin(); it2 != rMap.end(); ++it2) {
			delete it2->second;
			it2->second = 0;
		}
	}

	m_cRouteMaster.clear();
}

/**
 * @brief 指定したルート以外の、すべてのルートを削除する
 *
 * @param rRouteSet [in] 削除しないルート一覧
 */
void LinkTracer::RouteMaster::clearOtherRoute(const RouteSet& rRouteSet)
{
	RouteSet cRouteSet;
	for(RouteSet::const_iterator it = rRouteSet.begin(); it != rRouteSet.end(); ++it) {
		(*it)->getRouteSet(cRouteSet);
	}

	iterator it1_next;
	for(iterator it1 = m_cRouteMaster.begin(); it1 != m_cRouteMaster.end(); it1 = it1_next) {
		++(it1_next = it1);
		RouteMap& rMap = it1->second;

		RouteMap::iterator it2_next;
		for(RouteMap::iterator it2 = rMap.begin(); it2 != rMap.end(); it2 = it2_next) {
			++(it2_next = it2);
			if(cRouteSet.find(it2->second) == cRouteSet.end()) {
				delete it2->second;
				it2->second = 0;
				rMap.erase(it2);
			}
		}
		if(rMap.empty()) {
			m_cRouteMaster.erase(it1);
		}
	}
}

LinkTracer::Route* LinkTracer::RouteMaster::addRoute(const Route* pPrevRoute, long nNodeId, const Link* pNextLink)
{
	// 次のリンクか最大経路数が設定されていないかすれば何もしない
	if(! (pNextLink && m_nMaxRouteCount))
		return 0;

	// 前のルートの設定を行う(初回は0)
	const Link* pPrevLink = pPrevRoute ? pPrevRoute->m_pLink : 0;

	// 前のリンクと次のリンクを設定
	// イメージ
	//    <前のリンク>       <対象ノード>       <次のリンク>
	// ------------------------->●------------------------->
	// 
	long nLinkId1 = pPrevLink ? pPrevLink->m_nObjectId : 0;
	long nLinkId2 = pNextLink->m_nObjectId;

	// 前のリンクから対象ノードに出ていけない、かつ次のリンクは対象ノードから入れない場合は0を返す
	if(pPrevLink && ! (pPrevLink->isOutableTo(nNodeId) && pNextLink->isInableFrom(nNodeId)))
		return 0;

	// ルート用意
	Route* pRoute = new Route;

	// 追加
	if(! pRoute->setRoute(pPrevRoute, nNodeId, pNextLink)) {
		delete pRoute;
		return 0;
	}

	// 前のリンクと次のリンクを結ぶルート群を求める(つまり迂回路)
	RouteMap& rRouteMap = m_cRouteMaster[LinkIdPair(nLinkId1, nLinkId2)];

	// 右左禁なしルートの件数
	unsigned int nTurnregEmptyCount = 0;
	// ルートマップの逆イテレータの終わり、つまり最初を削除対象として定義
	RouteMap::reverse_iterator itErase = rRouteMap.rend();

	// 最初のリンクでなければ
	if(nLinkId1 != 0) {
		// 最大経路数が0でなければ
		if(m_nMaxRouteCount > 0) {
			// 迂回路を距離が長い順に巡回
			for(RouteMap::reverse_iterator it = rRouteMap.rbegin(); it != rRouteMap.rend(); ++it) {
				// 迂回路の中に右左禁の規制がなければ
				if(it->second->m_cTurnregMap.empty()) {
					// 右左禁なしの一番長い迂回路の方が今見ているルートよりも長いのであれば、この迂回路には意味がない(無限ループ？)
					if(! nTurnregEmptyCount) {
						if(pRoute->m_dDepth < it->first) {
							itErase = it;
						}
					}
					// 右左禁なし迂回路数をインクリメント
					nTurnregEmptyCount++;
				}
			}
		}

		// 既存の迂回路が意味をなさず、規定回数以上のループが生じているなら
		if(nTurnregEmptyCount >= m_nMaxRouteCount) {
			if(itErase == rRouteMap.rend()) {
				delete pRoute;
				return 0;
			}
			else {
#if(0)
				delete itErase->second;
				itErase->second = 0;
				rRouteMap.erase(itErase.base());
#endif
			}
		}
	}

	rRouteMap.insert(RouteMap::value_type(pRoute->m_dDepth, pRoute));

	return pRoute;	
}

void LinkTracer::RouteMaster::getRouteMapToLink(long nLinkId, RouteMap& rRouteMap) const
{
	for(const_iterator it1 = begin(); it1 != end(); ++it1) {
		const LinkIdPair& rLinkIdPair = it1->first;
		const RouteMap& rThisRouteMap = it1->second;
		if(rLinkIdPair.second != nLinkId)
			continue;

		rRouteMap.insert(rThisRouteMap.begin(), rThisRouteMap.end());
	}
}

// LinkTracer::Link
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LinkTracer::Link::Link(IFeature* ipLink)
{
	setLink(ipLink);
}

void LinkTracer::Link::clear()
{
	m_nObjectId = 0;
	m_nFromNodeId = 0;
	m_nToNodeId = 0;
	m_nOneway = 0;
	clearConnection();
	clearTurnreg();
}

void LinkTracer::Link::clearConnection()
{
	m_cInLinkSet1.clear();
	m_cInLinkSet2.clear();
	m_cOutLinkSet1.clear();
	m_cOutLinkSet2.clear();
}

void LinkTracer::Link::clearTurnreg()
{
	m_cTurnregSet.clear();
}

//////////////////////////////////////////////////////////////////////
// 設定/取得
//////////////////////////////////////////////////////////////////////

/**
 * @brief 属性を設定する
 *
 * @param ipLink [in] 道路リンク
 * @retval true 成功
 * @retval false 失敗
 */
bool LinkTracer::Link::setLink(IFeature* ipLink)
{
	clear();
	if(! ipLink)
		return false;

	// オブジェクトID取得
	ipLink->get_OID(&m_nObjectId);

	// 各種属性取得準備
	IFieldsPtr ipFields;
	if(ipLink->get_Fields(&ipFields) != S_OK || ipFields == 0)
		return false;

	long nIndex;
	CComVariant va;

	// 始点側ノードID取得
	if(ipFields->FindField(CComBSTR(kFromNodeID), &nIndex) == S_OK && nIndex >= 0) {
		ipLink->get_Value(nIndex, &va);
		va.ChangeType(VT_I4);
		m_nFromNodeId = va.lVal;
	}
	
	// 終点側ノードID取得
	if(ipFields->FindField(CComBSTR(kToNodeID), &nIndex) == S_OK && nIndex >= 0) {
		ipLink->get_Value(nIndex, &va);
		va.ChangeType(VT_I4);
		m_nToNodeId  = va.lVal;
	}
	
	// 一方通行コード取得
	if(ipFields->FindField(CComBSTR(kOneway), &nIndex) == S_OK && nIndex >= 0) {
		ipLink->get_Value(nIndex, &va);
		va.ChangeType(VT_I4);
		m_nOneway  = va.lVal;
	}

	return true;
}

/**
 * @brief 右左折禁止規制を追加する
 *
 * @note 右左折禁止と道路リンクの関連性の確認は行わない。
 *
 * @param pTurnreg 右左折禁止規制
 */
void LinkTracer::Link::addTurnreg(const Turnreg* pTurnreg)
{
	if(! pTurnreg)
		return;

	m_cTurnregSet.insert(pTurnreg);
}

/**
 * @brief 流出可能道路リンクを追加する
 *
 * @note 一方通行の方向が不当であったり、３点右左折禁止に引っかかっていたら追加しない。
 * @note それ以外の妥当性検証は行っていない。
 *
 * @param pNextLink [in] 流出可能道路リンク
 * @retval true 追加した
 * @retval false 追加しなかった
 */
bool LinkTracer::Link::addConnection(Link* pNextLink)
{
	if(! pNextLink)
		return false;

	bool bResult = true;

	long nOnewayToNodeId = getOnewayToNodeId();
	if(nOnewayToNodeId) {
		bResult &= addConnection(nOnewayToNodeId, pNextLink);
	}
	else {
		bResult &= addConnection(m_nFromNodeId, pNextLink);
		bResult &= addConnection(m_nToNodeId, pNextLink);
	}

	return bResult;
}

/**
 * @brief 流出可能道路リンクを追加する
 *
 * @note 一方通行の方向が不当であったり、３点右左折禁止に引っかかっていたら追加しない。
 * @note それ以外の妥当性検証は行っていない。
 *
 * @param nNodeId [in] 接続ノードID
 * @param pNextLink [in] 流出可能道路リンク
 * @retval true 追加した
 * @retval false 追加しなかった
 */
bool LinkTracer::Link::addConnection(long nNodeId, Link* pNextLink)
{
	if(! (nNodeId && pNextLink))
		return false;

	if(! (nNodeId == pNextLink->m_nFromNodeId || nNodeId == pNextLink->m_nToNodeId))
		return false;

	// nNodeIdが、pNextLinkの一方通行終点側ノードIDと一致したら追加しない
	if(nNodeId == pNextLink->getOnewayToNodeId())
		return false;

	// ３点右左折禁止規制に引っかかっていたら追加しない
	for(TurnregSet::const_iterator it = m_cTurnregSet.begin(); it != m_cTurnregSet.end(); ++it) {
		const Turnreg* pTurnreg = *it;
		if(! (pTurnreg && pTurnreg->m_cLinkIdList.size() == 2))
			continue;

		if(pTurnreg->m_cLinkIdList[0] == m_nObjectId && pTurnreg->m_cLinkIdList[1] == pNextLink->m_nObjectId)
			return false;
	}

	// 流出可能道路リンクとして追加する
	if(nNodeId == m_nFromNodeId) {
		m_cOutLinkSet1.insert(pNextLink);
	}
	else if(nNodeId == m_nToNodeId) {
		m_cOutLinkSet2.insert(pNextLink);
	}
	else
		return false;

	// 流入可能道路リンクとして追加する
	if(pNextLink->m_nFromNodeId == nNodeId) {
		pNextLink->m_cInLinkSet1.insert(this);
	}
	else if(pNextLink->m_nToNodeId == nNodeId) {
		pNextLink->m_cInLinkSet2.insert(this);
	}
	else
		return false;

	return true;
}

/**
 * @brief 一方通行始点側ノードIDを取得する
 *
 * @return 一方通行始点側ノードID。条件なし一方通行でない場合は0。
 */
long LinkTracer::Link::getOnewayFromNodeId() const
{
	using namespace sindy::schema::road_link::oneway;

	switch(m_nOneway) {
	case kFore:
		return m_nFromNodeId;
		break;
	case kReverse:
		return m_nToNodeId;
		break;
	default:
		return 0;
	}
}

/**
 * @brief 一方通行終点側ノードIDを取得する
 *
 * @return 一方通行終点側ノードID。条件なし一方通行でない場合は0。
 */
long LinkTracer::Link::getOnewayToNodeId() const
{
	using namespace sindy::schema::road_link::oneway;

	switch(m_nOneway) {
	case kFore:
		return m_nToNodeId;
		break;
	case kReverse:
		return m_nFromNodeId;
		break;
	default:
		return 0;
	}
}

long LinkTracer::Link::getOppositeNodeId(long nNodeId) const
{
	if(nNodeId == m_nFromNodeId)
		return m_nToNodeId;
	else if(nNodeId == m_nToNodeId)
		return m_nFromNodeId;
	return 0;
}

bool LinkTracer::Link::isInableFrom(long nNodeId) const
{
	return (nNodeId == m_nFromNodeId || nNodeId == m_nToNodeId) && getOnewayToNodeId() != nNodeId;
}

bool LinkTracer::Link::isOutableTo(long nNodeId) const
{
	return (nNodeId == m_nFromNodeId || nNodeId == m_nToNodeId) && getOnewayFromNodeId() != nNodeId;
}

IGeometryPtr LinkTracer::Link::getShapeCopy() const
{
	IGeometryPtr ipGeometry = getShape();
	if(ipGeometry == 0)
		return 0;

	IClonePtr ipClone;
	IClonePtr(ipGeometry)->Clone(&ipClone);
	return IGeometryPtr(ipClone);
}

void LinkTracer::Link::addSubFieldsTo(IQueryFilter* ipQueryFilter)
{
	ipQueryFilter->AddField(CComBSTR(_T("OBJECTID")));
	ipQueryFilter->AddField(CComBSTR(kFromNodeID));
	ipQueryFilter->AddField(CComBSTR(kToNodeID));
	ipQueryFilter->AddField(CComBSTR(kOneway));
}

// LinkTracer::Link2
//////////////////////////////////////////////////////////////////////

LinkTracer::Link2::Link2(IFeaturePtr ipLink) :
Link(ipLink),
m_ipGeometry(getShapeCopyOf(ipLink)),
m_dWeight(getWeightOf(ipLink))
{
}

LinkTracer::Link2::Link2(IFeaturePtr ipLink, double dWeight) :
Link(ipLink),
m_ipGeometry(getShapeCopyOf(ipLink)),
m_dWeight(dWeight)
{
}

void LinkTracer::Link2::clear()
{
	Link::clear();
	m_ipGeometry = 0;
	m_dWeight = 0.0;
}

IGeometryPtr LinkTracer::Link2::getShapeCopyOf(IFeature* ipLink)
{
	if(ipLink == 0)
		return 0;

	IGeometryPtr ipGeometry;
	ipLink->get_ShapeCopy(&ipGeometry);

	return ipGeometry;
}

double LinkTracer::Link2::getWeightOf(IFeature* ipLink)
{
	if(ipLink != 0) {
		IFieldsPtr ipFields;
		if(ipLink->get_Fields(&ipFields) == S_OK || ipFields != 0) {
			long nIndex;
			CComVariant va;

			if(ipFields->FindField(CComBSTR(_T("Shape.Len")), &nIndex) == S_OK && nIndex >= 0) {
				ipLink->get_Value(nIndex, &va);
				if(va.vt == VT_R8)
					return va.dblVal;
			}
		}
	}

	return 1.0;
}

void LinkTracer::Link2::addSubFieldsTo(IQueryFilter* ipQueryFilter)
{
	Link::addSubFieldsTo(ipQueryFilter);
	ipQueryFilter->AddField(CComBSTR(_T("Shape")));
	ipQueryFilter->AddField(CComBSTR(_T("Shape.Len")));
}

// LinkTracer::TraceSpace
//////////////////////////////////////////////////////////////////////

/**
 * @brief メンバ変数をクリアする
 *
 * @note - 道路リンクの登録解除
 * @note - ノードID別流出可能リンク一覧クリア
 * @note - ノードID別流入可能リンク一覧クリア
 * @note - 右左折禁止規制一覧クリア
 */
void LinkTracer::TraceSpace::clear()
{
	clearLink();
	m_cInableLinkMap.clear();
	m_cOutableLinkMap.clear();
	m_cTurnregMap.clear();
}

/**
 * @brief 道路リンクの配置、右左折禁止の設定、後処理を行う
 *
 * @note 以下の作業をひとまとめにしたメソッド
 * @note -# 全道路リンクに対するaddLink()
 * @note -# setTurnreg()
 * @note -# culminate()
 *
 * @param ipLinkCursor [in] 道路リンクカーソル
 * @param ipInfTurnregCursor [in] 右左折禁止Infカーソル
 * @param ipLqTurnregTable [in] 右左折禁止Lqテーブル
 * @retval true 配置成功
 * @retval false ipLinkCursorがNULLだった
 */
bool LinkTracer::TraceSpace::setLink(IFeatureCursor* ipLinkCursor, _ICursor* ipInfTurnregCursor, ITable* ipLqTurnregTable)
{
	clear();

	if(! ipLinkCursor)
		return false;

	IFeaturePtr ipLink;
	while(ipLinkCursor->NextFeature(&ipLink) == S_OK) {
		addLink(ipLink);
	}

	setTurnreg(ipInfTurnregCursor, ipLqTurnregTable);
	culminate();

	return true;
}

/**
 * @brief 空間に道路リンクを配置する
 *
 * @param ipLink [in] 配置する道路リンク
 */
void LinkTracer::TraceSpace::addLink(IFeaturePtr ipLink)
{
	if(ipLink == 0)
		return;

	// 道路リンクを登録する
	long nObjectId;
	ipLink->get_OID(&nObjectId);

	Link* pLink = addLinkImpl(nObjectId, ipLink);
	if(! pLink)
		return;

	// 道路の両端ノードに対して、流入可・流出可情報を追加する
	addInableLink(pLink->m_nFromNodeId, *pLink);
	addInableLink(pLink->m_nToNodeId, *pLink);
	addOutableLink(pLink->m_nFromNodeId, *pLink);
	addOutableLink(pLink->m_nToNodeId, *pLink);
}

/**
 * @brief 右左折禁止規制を登録する
 *
 * @param ipInfTurnregCursor [in] 右左折禁止規制Infカーソル
 * @param ipLqTurnregTable [in] 右左折禁止規制Lqテーブル
 */
void LinkTracer::TraceSpace::setTurnreg(_ICursor* ipInfTurnregCursor, ITable* ipLqTurnregTable)
{
	if(! (ipInfTurnregCursor && ipLqTurnregTable))
		return;

	m_cTurnregMap.setTurnreg(ipInfTurnregCursor, ipLqTurnregTable);
}

/**
 * @brief 後処理を行う
 *
 * @note 以下の２つのメソッドを実行する:
 * @note -# culminateTurnreg()
 * @note -# culminateConnection()
 */
void LinkTracer::TraceSpace::culminate()
{
	culminateTurnreg();
	culminateConnection();
}

/**
 * @brief 右左折禁止規制を、空間に配置されている道路リンクに適用する
 */
void LinkTracer::TraceSpace::culminateTurnreg()
{
	for(TurnregMap::const_iterator it1 = m_cTurnregMap.begin(); it1 != m_cTurnregMap.end(); ++it1) {
		const Turnreg& rTurnreg = it1->second;
		const Turnreg::LinkIdList& rLinkIdList = rTurnreg.m_cLinkIdList;
		for(Turnreg::LinkIdList::const_iterator it2 = rLinkIdList.begin(); it2 != rLinkIdList.end(); ++it2) {
			long nLinkId = *it2;
			Link* pLink = searchLink(nLinkId);
			if(! pLink)
				continue;

			pLink->addTurnreg(&rTurnreg);
		}
	}
}

/**
 * @brief 空間に配置されているリンク間の接続関係を設定する
 */
void LinkTracer::TraceSpace::culminateConnection()
{
	for(InableLinkMap::iterator it1 = m_cInableLinkMap.begin(); it1 != m_cInableLinkMap.end(); ++it1) {
		long nObjectId = it1->first;

		OutableLinkMap::iterator it2 = m_cOutableLinkMap.find(nObjectId);
		if(it2 == m_cOutableLinkMap.end())
			continue;

		addConnection(it1->second, it2->second);
	}
}

/**
 * @brief 指定したノードへ流入可能なリンクとして登録する
 *
 * @note 無条件一方通行・通行禁止によって、指定したノードへ流入できない場合は登録しない。
 *
 * @param nNodeId [in] ノードID
 * @param rLink [in] ノードへ流入可能なリンク
 */
void LinkTracer::TraceSpace::addInableLink(long nNodeId, Link& rLink)
{
	if(rLink.isOutableTo(nNodeId)) {
		m_cInableLinkMap[nNodeId].insert(&rLink);
	}
}

/**
 * @brief 指定したノードから流出可能なリンクとして登録する
 *
 * @note 無条件一方通行・通行禁止によって、指定したノードから流出できない場合は登録しない。
 *
 * @param nNodeId [in] ノードID
 * @param rLink [in] ノードから流出可能なリンク
 */
void LinkTracer::TraceSpace::addOutableLink(long nNodeId, Link& rLink)
{
	if(rLink.isInableFrom(nNodeId)) {
		m_cOutableLinkMap[nNodeId].insert(&rLink);
	}
}

/**
 * @brief 接続関係を設定する
 *
 * @param rInLinkSet [in] 基準リンク一覧
 * @param rOutLinkSet [in] 基準リンクから流出するリンク一覧
 */
void LinkTracer::TraceSpace::addConnection(LinkSet& rInLinkSet, LinkSet& rOutLinkSet)
{
	for(LinkSet::iterator it1 = rInLinkSet.begin(); it1 != rInLinkSet.end(); ++it1) {
		Link* pInLink = *it1;
		for(LinkSet::iterator it2 = rOutLinkSet.begin(); it2 != rOutLinkSet.end(); ++it2) {
			Link* pOutLink = *it2;
			if(pInLink == pOutLink)
				continue;

			pInLink->addConnection(pOutLink);
		}
	}
}

/**
 * @brief 指定したノードへ流入可能なリンク一覧を取得する
 *
 * @param nNodeId [in] ノードID
 * @return リンク一覧。流入可能なリンクが一つも存在しない場合はNULL。
 */
const LinkTracer::TraceSpace::LinkSet* LinkTracer::TraceSpace::searchInableLinkSetTo(long nNodeId) const
{
	return searchConnectLinkSet(nNodeId, inableLinkMap());
}

/**
 * @brief 指定したノードから流出可能なリンク一覧を取得する
 *
 * @param nNodeId [in] ノードID
 * @return リンク一覧。流出可能なリンクが一つも存在しない場合はNULL。
 */
const LinkTracer::TraceSpace::LinkSet* LinkTracer::TraceSpace::searchOutableLinkSetFrom(long nNodeId) const
{
	return searchConnectLinkSet(nNodeId, outableLinkMap());
}

/**
 * @brief 「ノードID別リンク一覧」から、指定したノードに接続するリンク一覧を取得する
 *
 * @param nNodeId [in] ノードID
 * @param rLinkMap [in] ノードID別リンク一覧
 * @return リンク一覧。指定したノードに接続する「リンク一覧」が存在しない場合はNULL。
 */
const LinkTracer::TraceSpace::LinkSet* LinkTracer::TraceSpace::searchConnectLinkSet(long nNodeId, const LinkMap& rLinkMap) const
{
	LinkMap::const_iterator it = rLinkMap.find(nNodeId);
	return (it != rLinkMap.end() ? &it->second : 0);
}

// LinkTracer::Tracer
//////////////////////////////////////////////////////////////////////

LinkTracer::Tracer::Tracer(const TraceSpace& rTraceSpace) :
m_rTraceSpace(rTraceSpace)
{
}

LinkTracer::Tracer::~Tracer()
{
	m_cRouteMaster.clear();
}

void LinkTracer::Tracer::clear()
{
	clearTracer();
}

void LinkTracer::Tracer::clearTracer()
{
	m_cRouteMaster.clear();
	m_cRouteMap.clear();
}

//////////////////////////////////////////////////////////////////////
// 設定/取得
//////////////////////////////////////////////////////////////////////

/**
 * @brief 追跡開始リンクを設定する
 *
 * @param nInNodeId [in] 追跡開始ノードID
 * @param nOutNodeId [in] 追跡開始リンク特定用、反対側ノードID。リンクの特定が不要な場合は0。
 * @retval true 設定成功
 * @retval fasle 設定失敗
 */
bool LinkTracer::Tracer::setStartNode(long nInNodeId, long nOutNodeId)
{
	clear();
	return addStartNode(nInNodeId, nOutNodeId);
}

/**
 * @brief 追跡開始リンクを追加する
 *
 * @param nInNodeId [in] 追跡開始ノードID
 * @param nOutNodeId [in] 追跡開始リンク特定用、反対側ノードID。リンクの特定が不要な場合は0。
 * @retval true 追加成功
 * @retval fasle 追加失敗
 */
bool LinkTracer::Tracer::addStartNode(long nInNodeId, long nOutNodeId)
{
	if(! nInNodeId)
		return false;

	// 追跡開始ノードから流出可能なリンクが存在しない場合、追加失敗
	const TraceSpace::OutableLinkMap& rLinkMap = m_rTraceSpace.outableLinkMap();
	TraceSpace::OutableLinkMap::const_iterator it1 = rLinkMap.find(nInNodeId);
	if(it1 == rLinkMap.end())
		return false;

	bool bResult = false;

	for(TraceSpace::LinkSet::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
		Link* pLink = *it2;
		// 反対側ノードIDが指定されている場合はここでリンクを特定
		if(nOutNodeId && pLink->getOppositeNodeId(nInNodeId) != nOutNodeId)
			continue;

		bResult |= addStartLink(nInNodeId, pLink);
	}

	return bResult;
}

bool LinkTracer::Tracer::setStartLink(long nLinkId)
{
	clear();
	return addStartLink(nLinkId);
}

bool LinkTracer::Tracer::addStartLink(long nLinkId)
{
	if(! nLinkId)
		return false;

	return addStartLink(m_rTraceSpace.searchLink(nLinkId));
}

bool LinkTracer::Tracer::addStartLink(const Link* pLink)
{
	if(! pLink)
		return false;

	bool bResult = false;

	bResult |= addStartLink(pLink->m_nFromNodeId, pLink);
	bResult |= addStartLink(pLink->m_nToNodeId, pLink);

	return bResult;
}

bool LinkTracer::Tracer::addStartLink(long nInNodeId, const Link* pLink)
{
	if(! (nInNodeId && pLink))
		return false;

	if(pLink->isInableFrom(nInNodeId)) {
		Route* pRoute = m_cRouteMaster.addRoute(0, nInNodeId, pLink);
		if(pRoute) {
			m_cRouteMap.insert(RouteMap::value_type(pRoute->m_dDepth, pRoute));
			return true;
		}
	}

	return false;
}

/**
 * @brief 現在、追跡対象となっているルートを取得する
 *
 * @warning ルートの管理権はインスタンスにある。
 * @warning インスタンスが破棄されるとルートも破棄される。
 * @warning それ以外の状況でも、
 */
const LinkTracer::Route* LinkTracer::Tracer::getCurrentRoute()
{
	RouteMap::iterator it = getCurrent();
	return it != m_cRouteMap.end() ? it->second : 0;
}

/**
 * @brief 現状で、指定したノードへ到達可能な経路一覧を取得する
 *
 * @param nNodeId [in] ノードID
 * @param rRouteMap [out] 経路一覧
 */
void LinkTracer::Tracer::getRouteMapToNode(long nNodeId, RouteMap& rRouteMap) const
{
	const TraceSpace::LinkSet* pInableLinkSet = m_rTraceSpace.searchInableLinkSetTo(nNodeId);
	const TraceSpace::LinkSet* pOutableLinkSet = m_rTraceSpace.searchOutableLinkSetFrom(nNodeId);
	if(! pInableLinkSet)
		return;

	// 全ルートで走査
	for(RouteMaster::const_iterator it1 = m_cRouteMaster.begin(); it1 != m_cRouteMaster.end(); ++it1) {
		const LinkIdPair& rPair = it1->first;
		const RouteMap& rMap = it1->second;

		// ルート終点リンクから指定ノードへ流出できなければならない
		for(TraceSpace::LinkSet::const_iterator it2 = pInableLinkSet->begin(); it2 != pInableLinkSet->end(); ++it2) {
			const Link* pLink1 = *it2;
			if(rPair.second != pLink1->m_nObjectId)
				continue;

			bool bRes = true;
			for(TraceSpace::LinkSet::const_iterator it3 = pOutableLinkSet->begin(); it3 != pOutableLinkSet->end(); ++it3) {
				const Link* pLink2 = *it3;
				if(rPair.first == pLink2->m_nObjectId) {
					bRes = false;
					break;
				}
			}
			if(! bRes)
				continue;

			for(RouteMap::const_iterator it3 = rMap.begin(); it3 != rMap.end(); ++it3) {
				rRouteMap.insert(*it3);
			}
		}
	}
}

/**
 * @brief 指定した経路の、次のリンク一覧を取得する
 *
 * @param rRoute [in] 経路
 * @return 次のリンク一覧。NULLが返ることは、設計仕様上あってはならない。
 */
const LinkTracer::Link::ConnectLinkSet* LinkTracer::Tracer::getNextLinkSet(const Route& rRoute) const
{
	long nNodeId = rRoute.m_nNodeId;
	const Link* pLink = rRoute.m_pLink;
	if(! (nNodeId && pLink))
		return 0;

	if(pLink->m_nFromNodeId == nNodeId)
		return &pLink->m_cOutLinkSet2;
	else if(pLink->m_nToNodeId == nNodeId)
		return &pLink->m_cOutLinkSet1;
	
	return 0;
}

//////////////////////////////////////////////////////////////////////
// 処理
//////////////////////////////////////////////////////////////////////

void LinkTracer::Tracer::skipTrace()
{
	m_cRouteMap.erase(getCurrent());
}

/**
 * @brief ２つのリンクを繋ぐルートを１つ見つけるまで追跡を行う
 * 
 * @param nStartLinkId [in] 始点リンクID
 * @param nEndLinkId [in] 終点リンクID
 */
const LinkTracer::Route* LinkTracer::Tracer::traceLinkToLink(long nStartLinkId, long nEndLinkId)
{
	if(! setStartLink(nStartLinkId))
		return 0;

	do {
		const Route* pRoute = getCurrentRoute();
		if(! pRoute)
			continue;

		if(pRoute->m_pLink->m_nObjectId == nEndLinkId)
			return pRoute;
	}
	while (stepTrace());

	return 0;
}

// LinkTracer::DijkstraTracer
//////////////////////////////////////////////////////////////////////

LinkTracer::DijkstraTracer::DijkstraTracer(const TraceSpace& rTraceSpace) :
LinkTracer::Tracer(rTraceSpace)
{
}

//////////////////////////////////////////////////////////////////////
// 設定/取得
//////////////////////////////////////////////////////////////////////

LinkTracer::RouteMap::iterator LinkTracer::DijkstraTracer::getCurrent()
{
	return m_cRouteMap.begin();
}

//////////////////////////////////////////////////////////////////////
// 処理
//////////////////////////////////////////////////////////////////////

/**
 * @brief 追跡を１ステップ進める
 *
 * @retval true 追跡を進めた
 * @retval false 追跡は終わっている
 */
bool LinkTracer::DijkstraTracer::stepTrace()
{
	// 待ち行列の先頭に位置する経路を取得
	// この経路は、言い換えれば起点から最も近い待ち行列
	RouteMap::iterator it = getCurrent();

	// 待ち行列がなくなったら終了
	if(it == m_cRouteMap.end())
		return false;

	// 今見ている経路の深さとルートを取得
	double dDepth = it->first;
	Route* pCurrentRoute = it->second;

	// 待ち行列から先頭の経路を除外
	m_cRouteMap.erase(it);

	// 経路を１ステップ進める
	stepTrace(dDepth, pCurrentRoute);

	return true;
}

/**
 * @brief 指定した深度の経路を１ステップ進める
 *
 * @param dDepth [in] 深度
 * @param pRoute [in] 経路
 */
void LinkTracer::DijkstraTracer::stepTrace(double, Route* pRoute)
{
	if(! pRoute)
		return;

	long nCurrentNodeId = pRoute->m_nNodeId;
	const Link* pCurrentLink = pRoute->m_pLink;
	if(! pCurrentLink)
		return;

	// 次のリンクへ…
	stepTrace(pRoute, pCurrentLink->getOppositeNodeId(nCurrentNodeId));
}

void LinkTracer::DijkstraTracer::stepTrace(Route* pRoute, long nNextNodeId)
{
	// 次のリンクがなければ打ち止め
	const Link::ConnectLinkSet* pSet = getNextLinkSet(*pRoute);
	if(! pSet)
		return;

	// 次につながるリンクの数だけ処理を流す。つまり、分岐に対応している
	for(Link::ConnectLinkSet::const_iterator it = pSet->begin(); it != pSet->end(); ++it) {
		stepTrace(pRoute, nNextNodeId, *it);
	}	
}

void LinkTracer::DijkstraTracer::stepTrace(Route* pRoute, long nNextNodeId, const Link* pNextLink)
{
	// 次のリンクを探索していく(分岐した数だけルートは増えていく)
	Route* pNextRoute = m_cRouteMaster.addRoute(pRoute, nNextNodeId, pNextLink);

	// 待ち行列に追加
	if(pNextRoute) {
		m_cRouteMap.insert(RouteMap::value_type(pNextRoute->m_dDepth, pNextRoute));
	}
}
