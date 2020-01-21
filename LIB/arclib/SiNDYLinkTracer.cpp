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
 * @brief �I�u�W�F�N�gID�ƁA���ԋK����ݒ肷��
 *
 * @retval true ����
 * @retval false ������NULL������
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
 * @brief �E���܋֎~��o�^����
 *
 * @param ipInfTurnregCursor [in] �E���܋֎~Inf�J�[�\��
 * @param ipLqTurnregTable [in] �E���܋֎~Lq�e�[�u��
 * @retval true ����
 * @retval false ���s
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

	// �m�[�h�ƃ����N�������Ă��Ȃ����false
	if(! (nNodeId && pLink))
		return false;

	// ���܂Œʂ��Ă����o�H�̍Ō�̃����N���擾
	const Link* pPrevLink = pPrevRoute ? pPrevRoute->m_pLink : 0;

	// �C���[�W
	//    <�O�̃����N>       <�Ώۃm�[�h>       <���̃����N>
	// ------------------------->��------------------------->
	// 
	// �O�̃����N����Ώۃm�[�h�֍s���Ȃ�������A���̃����N���Ώۂ������Ȃ����false
	if(pPrevLink && ! (pPrevLink->isOutableTo(nNodeId) && pLink->isInableFrom(nNodeId)))
		return false;

	// �O�̃��[�g������Ȃ��
	if(pPrevRoute) {
		// �O�̃��[�g�̉E���ւ��擾
		const std::map<const Turnreg*, long>& rPrevTurnregMap = pPrevRoute->m_cTurnregMap;
		// �e�E���ւ�����
		for(std::map<const Turnreg*, long>::const_iterator it = rPrevTurnregMap.begin(); it != rPrevTurnregMap.end(); ++it) {
			const Turnreg* pTurnreg = it->first;
			long nNextCount = it->second + 1;

			// ���̃����N�����̉E���ւƖ��֌W�Ȃ牽�����Ȃ�
			Link::TurnregSet::const_iterator it2 = pLink->m_cTurnregSet.find(pTurnreg);
			if(it2 == pLink->m_cTurnregSet.end())
				continue;

			// �O�̃��[�g���ۗL����E���ւ̎��̃����N�����Y�����N�ł���ꍇ
			if(pTurnreg->m_cLinkIdList[nNextCount] == pLink->m_nObjectId) {
				// ���̃����N���E���ւ̍Ō�̃����N�łȂ���ΉE���ւɃ����N��ǉ����邾��
				if(nNextCount + 1 < static_cast<int>(pTurnreg->m_cLinkIdList.size())) {
					m_cTurnregMap[pTurnreg] = nNextCount;
				}
				else
				{
					// ���̃����N���E���ւ̍Ō�̃����N�������烋�[�g�ǉ����s
					return false;
				}
			}
		}
	}

	// ���Y�����N���ւ��E���ւ����񂷂�
	for(Link::TurnregSet::const_iterator it = pLink->m_cTurnregSet.begin(); it != pLink->m_cTurnregSet.end(); ++it) {
		// �E���ւ��Ȃ������胊���NID�̃��X�g���󂾂����肵���烋�[�g�ǉ����s(��O�������肵�������悢��������Ȃ�)
		const Turnreg* pTurnreg = *it;
		if(! pTurnreg || pTurnreg->m_cLinkIdList.empty())
			return false;

		// �E���ւ̃����N��1�{�ō\������Ă���_��
		if(pTurnreg->m_cLinkIdList.size() == 1) 
			return false;

		// �E���ւ̃����N�̍ŏ��̃����N�����̃����N��������
		if(pTurnreg->m_cLinkIdList[0] == pLink->m_nObjectId) {
			// �Ώۃm�[�h�����̃����N��From�m�[�h�ł���΁ATo���̃����N�Q���擾
			// To�m�[�h�ł����From���̐ڑ������N�Q���擾
			// �܂�A���̃����N�̒��ԃm�[�h����Ȃ����A���̎��̃����N�Q���擾
			const Link::ConnectLinkSet& rConnectLinkSet =
				(nNodeId == pLink->m_nFromNodeId) ? pLink->m_cOutLinkSet2 : pLink->m_cOutLinkSet1;

			// ���̎��̃����N�Q������
			for(Link::ConnectLinkSet::const_iterator it2 = rConnectLinkSet.begin(); it2 != rConnectLinkSet.end(); ++it2) {
				// ���̎��̃����N���A�E���ւ�2�Ԗڂ̃����N�������ꍇ�ɉE����map�ɓo�^
				const Link* pConnectLink = *it2;
				if(pConnectLink->m_nObjectId == pTurnreg->m_cLinkIdList[1]) {
					m_cTurnregMap.insert(std::map<const Turnreg*, long>::value_type(pTurnreg, 0));
					break;
				}
			}
		}
	}

	// ���[�g�̃m�[�hID�͑Ώۃm�[�h�A�����N�͎��̃����N�A�O�̃��[�g��o�^
	m_nNodeId = nNodeId;
	m_pLink = pLink;
	m_pPrevRoute = pPrevRoute;

	// �[���͑O�̃��[�g�Ƀ����N�̃E�F�C�g�����Z����
	m_dDepth = (m_pPrevRoute ? m_pPrevRoute->m_dDepth : 0.0) + m_pLink->getWeight(m_pPrevRoute);

	return true;
}

/**
 * @brief �����[�g���\�����邷�ׂẴ��[�g�̈ꗗ���擾����
 *
 * @note �������g���܂܂��
 *
 * @param rRouteSet [out] ���[�g�ꗗ
 */
void LinkTracer::Route::getRouteSet(std::set<const Route*>& rRouteSet) const
{
	for(const Route* pRoute = this; pRoute; pRoute = pRoute->m_pPrevRoute) {
		rRouteSet.insert(pRoute);
	}
}

/**
 * @brief �����[�g���\�����鏉���̃��[�g���擾����
 *
 * @return �����̃��[�g�B�����[�g���g�ł���ꍇ������B
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
 * @brief ���ׂẴ��[�g���폜����
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
 * @brief �w�肵�����[�g�ȊO�́A���ׂẴ��[�g���폜����
 *
 * @param rRouteSet [in] �폜���Ȃ����[�g�ꗗ
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
	// ���̃����N���ő�o�H�����ݒ肳��Ă��Ȃ�������Ή������Ȃ�
	if(! (pNextLink && m_nMaxRouteCount))
		return 0;

	// �O�̃��[�g�̐ݒ���s��(�����0)
	const Link* pPrevLink = pPrevRoute ? pPrevRoute->m_pLink : 0;

	// �O�̃����N�Ǝ��̃����N��ݒ�
	// �C���[�W
	//    <�O�̃����N>       <�Ώۃm�[�h>       <���̃����N>
	// ------------------------->��------------------------->
	// 
	long nLinkId1 = pPrevLink ? pPrevLink->m_nObjectId : 0;
	long nLinkId2 = pNextLink->m_nObjectId;

	// �O�̃����N����Ώۃm�[�h�ɏo�Ă����Ȃ��A�����̃����N�͑Ώۃm�[�h�������Ȃ��ꍇ��0��Ԃ�
	if(pPrevLink && ! (pPrevLink->isOutableTo(nNodeId) && pNextLink->isInableFrom(nNodeId)))
		return 0;

	// ���[�g�p��
	Route* pRoute = new Route;

	// �ǉ�
	if(! pRoute->setRoute(pPrevRoute, nNodeId, pNextLink)) {
		delete pRoute;
		return 0;
	}

	// �O�̃����N�Ǝ��̃����N�����ԃ��[�g�Q�����߂�(�܂�I��H)
	RouteMap& rRouteMap = m_cRouteMaster[LinkIdPair(nLinkId1, nLinkId2)];

	// �E���ւȂ����[�g�̌���
	unsigned int nTurnregEmptyCount = 0;
	// ���[�g�}�b�v�̋t�C�e���[�^�̏I���A�܂�ŏ����폜�ΏۂƂ��Ē�`
	RouteMap::reverse_iterator itErase = rRouteMap.rend();

	// �ŏ��̃����N�łȂ����
	if(nLinkId1 != 0) {
		// �ő�o�H����0�łȂ����
		if(m_nMaxRouteCount > 0) {
			// �I��H���������������ɏ���
			for(RouteMap::reverse_iterator it = rRouteMap.rbegin(); it != rRouteMap.rend(); ++it) {
				// �I��H�̒��ɉE���ւ̋K�����Ȃ����
				if(it->second->m_cTurnregMap.empty()) {
					// �E���ւȂ��̈�Ԓ����I��H�̕��������Ă��郋�[�g���������̂ł���΁A���̉I��H�ɂ͈Ӗ����Ȃ�(�������[�v�H)
					if(! nTurnregEmptyCount) {
						if(pRoute->m_dDepth < it->first) {
							itErase = it;
						}
					}
					// �E���ւȂ��I��H�����C���N�������g
					nTurnregEmptyCount++;
				}
			}
		}

		// �����̉I��H���Ӗ����Ȃ����A�K��񐔈ȏ�̃��[�v�������Ă���Ȃ�
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
// �\�z/����
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
// �ݒ�/�擾
//////////////////////////////////////////////////////////////////////

/**
 * @brief ������ݒ肷��
 *
 * @param ipLink [in] ���H�����N
 * @retval true ����
 * @retval false ���s
 */
bool LinkTracer::Link::setLink(IFeature* ipLink)
{
	clear();
	if(! ipLink)
		return false;

	// �I�u�W�F�N�gID�擾
	ipLink->get_OID(&m_nObjectId);

	// �e�푮���擾����
	IFieldsPtr ipFields;
	if(ipLink->get_Fields(&ipFields) != S_OK || ipFields == 0)
		return false;

	long nIndex;
	CComVariant va;

	// �n�_���m�[�hID�擾
	if(ipFields->FindField(CComBSTR(kFromNodeID), &nIndex) == S_OK && nIndex >= 0) {
		ipLink->get_Value(nIndex, &va);
		va.ChangeType(VT_I4);
		m_nFromNodeId = va.lVal;
	}
	
	// �I�_���m�[�hID�擾
	if(ipFields->FindField(CComBSTR(kToNodeID), &nIndex) == S_OK && nIndex >= 0) {
		ipLink->get_Value(nIndex, &va);
		va.ChangeType(VT_I4);
		m_nToNodeId  = va.lVal;
	}
	
	// ����ʍs�R�[�h�擾
	if(ipFields->FindField(CComBSTR(kOneway), &nIndex) == S_OK && nIndex >= 0) {
		ipLink->get_Value(nIndex, &va);
		va.ChangeType(VT_I4);
		m_nOneway  = va.lVal;
	}

	return true;
}

/**
 * @brief �E���܋֎~�K����ǉ�����
 *
 * @note �E���܋֎~�Ɠ��H�����N�̊֘A���̊m�F�͍s��Ȃ��B
 *
 * @param pTurnreg �E���܋֎~�K��
 */
void LinkTracer::Link::addTurnreg(const Turnreg* pTurnreg)
{
	if(! pTurnreg)
		return;

	m_cTurnregSet.insert(pTurnreg);
}

/**
 * @brief ���o�\���H�����N��ǉ�����
 *
 * @note ����ʍs�̕������s���ł�������A�R�_�E���܋֎~�Ɉ����������Ă�����ǉ����Ȃ��B
 * @note ����ȊO�̑Ó������؂͍s���Ă��Ȃ��B
 *
 * @param pNextLink [in] ���o�\���H�����N
 * @retval true �ǉ�����
 * @retval false �ǉ����Ȃ�����
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
 * @brief ���o�\���H�����N��ǉ�����
 *
 * @note ����ʍs�̕������s���ł�������A�R�_�E���܋֎~�Ɉ����������Ă�����ǉ����Ȃ��B
 * @note ����ȊO�̑Ó������؂͍s���Ă��Ȃ��B
 *
 * @param nNodeId [in] �ڑ��m�[�hID
 * @param pNextLink [in] ���o�\���H�����N
 * @retval true �ǉ�����
 * @retval false �ǉ����Ȃ�����
 */
bool LinkTracer::Link::addConnection(long nNodeId, Link* pNextLink)
{
	if(! (nNodeId && pNextLink))
		return false;

	if(! (nNodeId == pNextLink->m_nFromNodeId || nNodeId == pNextLink->m_nToNodeId))
		return false;

	// nNodeId���ApNextLink�̈���ʍs�I�_���m�[�hID�ƈ�v������ǉ����Ȃ�
	if(nNodeId == pNextLink->getOnewayToNodeId())
		return false;

	// �R�_�E���܋֎~�K���Ɉ����������Ă�����ǉ����Ȃ�
	for(TurnregSet::const_iterator it = m_cTurnregSet.begin(); it != m_cTurnregSet.end(); ++it) {
		const Turnreg* pTurnreg = *it;
		if(! (pTurnreg && pTurnreg->m_cLinkIdList.size() == 2))
			continue;

		if(pTurnreg->m_cLinkIdList[0] == m_nObjectId && pTurnreg->m_cLinkIdList[1] == pNextLink->m_nObjectId)
			return false;
	}

	// ���o�\���H�����N�Ƃ��Ēǉ�����
	if(nNodeId == m_nFromNodeId) {
		m_cOutLinkSet1.insert(pNextLink);
	}
	else if(nNodeId == m_nToNodeId) {
		m_cOutLinkSet2.insert(pNextLink);
	}
	else
		return false;

	// �����\���H�����N�Ƃ��Ēǉ�����
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
 * @brief ����ʍs�n�_���m�[�hID���擾����
 *
 * @return ����ʍs�n�_���m�[�hID�B�����Ȃ�����ʍs�łȂ��ꍇ��0�B
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
 * @brief ����ʍs�I�_���m�[�hID���擾����
 *
 * @return ����ʍs�I�_���m�[�hID�B�����Ȃ�����ʍs�łȂ��ꍇ��0�B
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
 * @brief �����o�ϐ����N���A����
 *
 * @note - ���H�����N�̓o�^����
 * @note - �m�[�hID�ʗ��o�\�����N�ꗗ�N���A
 * @note - �m�[�hID�ʗ����\�����N�ꗗ�N���A
 * @note - �E���܋֎~�K���ꗗ�N���A
 */
void LinkTracer::TraceSpace::clear()
{
	clearLink();
	m_cInableLinkMap.clear();
	m_cOutableLinkMap.clear();
	m_cTurnregMap.clear();
}

/**
 * @brief ���H�����N�̔z�u�A�E���܋֎~�̐ݒ�A�㏈�����s��
 *
 * @note �ȉ��̍�Ƃ��ЂƂ܂Ƃ߂ɂ������\�b�h
 * @note -# �S���H�����N�ɑ΂���addLink()
 * @note -# setTurnreg()
 * @note -# culminate()
 *
 * @param ipLinkCursor [in] ���H�����N�J�[�\��
 * @param ipInfTurnregCursor [in] �E���܋֎~Inf�J�[�\��
 * @param ipLqTurnregTable [in] �E���܋֎~Lq�e�[�u��
 * @retval true �z�u����
 * @retval false ipLinkCursor��NULL������
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
 * @brief ��Ԃɓ��H�����N��z�u����
 *
 * @param ipLink [in] �z�u���铹�H�����N
 */
void LinkTracer::TraceSpace::addLink(IFeaturePtr ipLink)
{
	if(ipLink == 0)
		return;

	// ���H�����N��o�^����
	long nObjectId;
	ipLink->get_OID(&nObjectId);

	Link* pLink = addLinkImpl(nObjectId, ipLink);
	if(! pLink)
		return;

	// ���H�̗��[�m�[�h�ɑ΂��āA�����E���o����ǉ�����
	addInableLink(pLink->m_nFromNodeId, *pLink);
	addInableLink(pLink->m_nToNodeId, *pLink);
	addOutableLink(pLink->m_nFromNodeId, *pLink);
	addOutableLink(pLink->m_nToNodeId, *pLink);
}

/**
 * @brief �E���܋֎~�K����o�^����
 *
 * @param ipInfTurnregCursor [in] �E���܋֎~�K��Inf�J�[�\��
 * @param ipLqTurnregTable [in] �E���܋֎~�K��Lq�e�[�u��
 */
void LinkTracer::TraceSpace::setTurnreg(_ICursor* ipInfTurnregCursor, ITable* ipLqTurnregTable)
{
	if(! (ipInfTurnregCursor && ipLqTurnregTable))
		return;

	m_cTurnregMap.setTurnreg(ipInfTurnregCursor, ipLqTurnregTable);
}

/**
 * @brief �㏈�����s��
 *
 * @note �ȉ��̂Q�̃��\�b�h�����s����:
 * @note -# culminateTurnreg()
 * @note -# culminateConnection()
 */
void LinkTracer::TraceSpace::culminate()
{
	culminateTurnreg();
	culminateConnection();
}

/**
 * @brief �E���܋֎~�K�����A��Ԃɔz�u����Ă��铹�H�����N�ɓK�p����
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
 * @brief ��Ԃɔz�u����Ă��郊���N�Ԃ̐ڑ��֌W��ݒ肷��
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
 * @brief �w�肵���m�[�h�֗����\�ȃ����N�Ƃ��ēo�^����
 *
 * @note ����������ʍs�E�ʍs�֎~�ɂ���āA�w�肵���m�[�h�֗����ł��Ȃ��ꍇ�͓o�^���Ȃ��B
 *
 * @param nNodeId [in] �m�[�hID
 * @param rLink [in] �m�[�h�֗����\�ȃ����N
 */
void LinkTracer::TraceSpace::addInableLink(long nNodeId, Link& rLink)
{
	if(rLink.isOutableTo(nNodeId)) {
		m_cInableLinkMap[nNodeId].insert(&rLink);
	}
}

/**
 * @brief �w�肵���m�[�h���痬�o�\�ȃ����N�Ƃ��ēo�^����
 *
 * @note ����������ʍs�E�ʍs�֎~�ɂ���āA�w�肵���m�[�h���痬�o�ł��Ȃ��ꍇ�͓o�^���Ȃ��B
 *
 * @param nNodeId [in] �m�[�hID
 * @param rLink [in] �m�[�h���痬�o�\�ȃ����N
 */
void LinkTracer::TraceSpace::addOutableLink(long nNodeId, Link& rLink)
{
	if(rLink.isInableFrom(nNodeId)) {
		m_cOutableLinkMap[nNodeId].insert(&rLink);
	}
}

/**
 * @brief �ڑ��֌W��ݒ肷��
 *
 * @param rInLinkSet [in] ������N�ꗗ
 * @param rOutLinkSet [in] ������N���痬�o���郊���N�ꗗ
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
 * @brief �w�肵���m�[�h�֗����\�ȃ����N�ꗗ���擾����
 *
 * @param nNodeId [in] �m�[�hID
 * @return �����N�ꗗ�B�����\�ȃ����N��������݂��Ȃ��ꍇ��NULL�B
 */
const LinkTracer::TraceSpace::LinkSet* LinkTracer::TraceSpace::searchInableLinkSetTo(long nNodeId) const
{
	return searchConnectLinkSet(nNodeId, inableLinkMap());
}

/**
 * @brief �w�肵���m�[�h���痬�o�\�ȃ����N�ꗗ���擾����
 *
 * @param nNodeId [in] �m�[�hID
 * @return �����N�ꗗ�B���o�\�ȃ����N��������݂��Ȃ��ꍇ��NULL�B
 */
const LinkTracer::TraceSpace::LinkSet* LinkTracer::TraceSpace::searchOutableLinkSetFrom(long nNodeId) const
{
	return searchConnectLinkSet(nNodeId, outableLinkMap());
}

/**
 * @brief �u�m�[�hID�ʃ����N�ꗗ�v����A�w�肵���m�[�h�ɐڑ����郊���N�ꗗ���擾����
 *
 * @param nNodeId [in] �m�[�hID
 * @param rLinkMap [in] �m�[�hID�ʃ����N�ꗗ
 * @return �����N�ꗗ�B�w�肵���m�[�h�ɐڑ�����u�����N�ꗗ�v�����݂��Ȃ��ꍇ��NULL�B
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
// �ݒ�/�擾
//////////////////////////////////////////////////////////////////////

/**
 * @brief �ǐՊJ�n�����N��ݒ肷��
 *
 * @param nInNodeId [in] �ǐՊJ�n�m�[�hID
 * @param nOutNodeId [in] �ǐՊJ�n�����N����p�A���Α��m�[�hID�B�����N�̓��肪�s�v�ȏꍇ��0�B
 * @retval true �ݒ萬��
 * @retval fasle �ݒ莸�s
 */
bool LinkTracer::Tracer::setStartNode(long nInNodeId, long nOutNodeId)
{
	clear();
	return addStartNode(nInNodeId, nOutNodeId);
}

/**
 * @brief �ǐՊJ�n�����N��ǉ�����
 *
 * @param nInNodeId [in] �ǐՊJ�n�m�[�hID
 * @param nOutNodeId [in] �ǐՊJ�n�����N����p�A���Α��m�[�hID�B�����N�̓��肪�s�v�ȏꍇ��0�B
 * @retval true �ǉ�����
 * @retval fasle �ǉ����s
 */
bool LinkTracer::Tracer::addStartNode(long nInNodeId, long nOutNodeId)
{
	if(! nInNodeId)
		return false;

	// �ǐՊJ�n�m�[�h���痬�o�\�ȃ����N�����݂��Ȃ��ꍇ�A�ǉ����s
	const TraceSpace::OutableLinkMap& rLinkMap = m_rTraceSpace.outableLinkMap();
	TraceSpace::OutableLinkMap::const_iterator it1 = rLinkMap.find(nInNodeId);
	if(it1 == rLinkMap.end())
		return false;

	bool bResult = false;

	for(TraceSpace::LinkSet::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
		Link* pLink = *it2;
		// ���Α��m�[�hID���w�肳��Ă���ꍇ�͂����Ń����N�����
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
 * @brief ���݁A�ǐՑΏۂƂȂ��Ă��郋�[�g���擾����
 *
 * @warning ���[�g�̊Ǘ����̓C���X�^���X�ɂ���B
 * @warning �C���X�^���X���j�������ƃ��[�g���j�������B
 * @warning ����ȊO�̏󋵂ł��A
 */
const LinkTracer::Route* LinkTracer::Tracer::getCurrentRoute()
{
	RouteMap::iterator it = getCurrent();
	return it != m_cRouteMap.end() ? it->second : 0;
}

/**
 * @brief ����ŁA�w�肵���m�[�h�֓��B�\�Ȍo�H�ꗗ���擾����
 *
 * @param nNodeId [in] �m�[�hID
 * @param rRouteMap [out] �o�H�ꗗ
 */
void LinkTracer::Tracer::getRouteMapToNode(long nNodeId, RouteMap& rRouteMap) const
{
	const TraceSpace::LinkSet* pInableLinkSet = m_rTraceSpace.searchInableLinkSetTo(nNodeId);
	const TraceSpace::LinkSet* pOutableLinkSet = m_rTraceSpace.searchOutableLinkSetFrom(nNodeId);
	if(! pInableLinkSet)
		return;

	// �S���[�g�ő���
	for(RouteMaster::const_iterator it1 = m_cRouteMaster.begin(); it1 != m_cRouteMaster.end(); ++it1) {
		const LinkIdPair& rPair = it1->first;
		const RouteMap& rMap = it1->second;

		// ���[�g�I�_�����N����w��m�[�h�֗��o�ł��Ȃ���΂Ȃ�Ȃ�
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
 * @brief �w�肵���o�H�́A���̃����N�ꗗ���擾����
 *
 * @param rRoute [in] �o�H
 * @return ���̃����N�ꗗ�BNULL���Ԃ邱�Ƃ́A�݌v�d�l�゠���Ă͂Ȃ�Ȃ��B
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
// ����
//////////////////////////////////////////////////////////////////////

void LinkTracer::Tracer::skipTrace()
{
	m_cRouteMap.erase(getCurrent());
}

/**
 * @brief �Q�̃����N���q�����[�g���P������܂ŒǐՂ��s��
 * 
 * @param nStartLinkId [in] �n�_�����NID
 * @param nEndLinkId [in] �I�_�����NID
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
// �ݒ�/�擾
//////////////////////////////////////////////////////////////////////

LinkTracer::RouteMap::iterator LinkTracer::DijkstraTracer::getCurrent()
{
	return m_cRouteMap.begin();
}

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief �ǐՂ��P�X�e�b�v�i�߂�
 *
 * @retval true �ǐՂ�i�߂�
 * @retval false �ǐՂ͏I����Ă���
 */
bool LinkTracer::DijkstraTracer::stepTrace()
{
	// �҂��s��̐擪�Ɉʒu����o�H���擾
	// ���̌o�H�́A����������΋N�_����ł��߂��҂��s��
	RouteMap::iterator it = getCurrent();

	// �҂��s�񂪂Ȃ��Ȃ�����I��
	if(it == m_cRouteMap.end())
		return false;

	// �����Ă���o�H�̐[���ƃ��[�g���擾
	double dDepth = it->first;
	Route* pCurrentRoute = it->second;

	// �҂��s�񂩂�擪�̌o�H�����O
	m_cRouteMap.erase(it);

	// �o�H���P�X�e�b�v�i�߂�
	stepTrace(dDepth, pCurrentRoute);

	return true;
}

/**
 * @brief �w�肵���[�x�̌o�H���P�X�e�b�v�i�߂�
 *
 * @param dDepth [in] �[�x
 * @param pRoute [in] �o�H
 */
void LinkTracer::DijkstraTracer::stepTrace(double, Route* pRoute)
{
	if(! pRoute)
		return;

	long nCurrentNodeId = pRoute->m_nNodeId;
	const Link* pCurrentLink = pRoute->m_pLink;
	if(! pCurrentLink)
		return;

	// ���̃����N�ցc
	stepTrace(pRoute, pCurrentLink->getOppositeNodeId(nCurrentNodeId));
}

void LinkTracer::DijkstraTracer::stepTrace(Route* pRoute, long nNextNodeId)
{
	// ���̃����N���Ȃ���Αł��~��
	const Link::ConnectLinkSet* pSet = getNextLinkSet(*pRoute);
	if(! pSet)
		return;

	// ���ɂȂ��郊���N�̐����������𗬂��B�܂�A����ɑΉ����Ă���
	for(Link::ConnectLinkSet::const_iterator it = pSet->begin(); it != pSet->end(); ++it) {
		stepTrace(pRoute, nNextNodeId, *it);
	}	
}

void LinkTracer::DijkstraTracer::stepTrace(Route* pRoute, long nNextNodeId, const Link* pNextLink)
{
	// ���̃����N��T�����Ă���(���򂵂����������[�g�͑����Ă���)
	Route* pNextRoute = m_cRouteMaster.addRoute(pRoute, nNextNodeId, pNextLink);

	// �҂��s��ɒǉ�
	if(pNextRoute) {
		m_cRouteMap.insert(RouteMap::value_type(pNextRoute->m_dDepth, pNextRoute));
	}
}
