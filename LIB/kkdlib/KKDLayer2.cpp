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

// KKDLayer2.cpp
//
//////////////////////////////////////////////////////////////////////

#include <DRMADevPlant.h>
#include "KKDLayer2.h"

// KKDRegulationRoadNodeLayer2
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// オーバーライド
//////////////////////////////////////////////////////////////////////

void KKDRegulationRoadNodeLayer2::clearIndex()
{
	_KKDRegulationRoadNodeLayer2::clearIndex();
	m_cIndexedAppertainRecordMap.clear();
}

void KKDRegulationRoadNodeLayer2::setIndex()
{
	_KKDRegulationRoadNodeLayer2::setIndex();
	for(iterator it = begin(); it != end(); ++it) {
		indexAppertainRecord(&*it);
	}
}

//////////////////////////////////////////////////////////////////////
// 設定/取得
//////////////////////////////////////////////////////////////////////

void KKDRegulationRoadNodeLayer2::indexAppertainRecord(Record* cRecord)
{
	for(int i = 0; i < cRecord->eAppertainNodeCount; i++) {	
		m_cIndexedAppertainRecordMap[cRecord->eAppertainNodes[i]] = cRecord;
	}
}

const KKDRegulationRoadNodeLayer2::Record* KKDRegulationRoadNodeLayer2::searchFromAppertain(const Record::AppertainNode& cAppretainNode) const
{
	IndexedAppertainRecordMap::const_iterator it = m_cIndexedAppertainRecordMap.find(cAppretainNode);
	return it != m_cIndexedAppertainRecordMap.end() ? it->second : 0;
}

// KKDRegulationRoadLinkLayer2
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// オーバーライド
//////////////////////////////////////////////////////////////////////

void KKDRegulationRoadLinkLayer2::clearIndex()
{
	_KKDRegulationRoadLinkLayer2::clearIndex();
	m_cIndexedNodeQueueRecordMap.clear();
}

void KKDRegulationRoadLinkLayer2::setIndex()
{
	_KKDRegulationRoadLinkLayer2::setIndex();
	for(iterator it = begin(); it != end(); ++it) {
		indexNodeQueueRecord(&*it);
	}
}

//////////////////////////////////////////////////////////////////////
// 設定/取得
//////////////////////////////////////////////////////////////////////

void KKDRegulationRoadLinkLayer2::indexNodeQueueRecord(Record* cRecord)
{
	for(int i = 0; i < cRecord->eMeshCount; i++) {
		const Record::NodesInMesh& rNodesInMesh = cRecord->eNodesInMeshs[i];
		std::map<NodeIdPair, Record*>& rMap = m_cIndexedNodeQueueRecordMap[rNodesInMesh.eMeshCode];
		for(int j = 0; j < rNodesInMesh.eNodeCount - 1; j++) {
			int aNodeId1 = rNodesInMesh.eNodeIds[j];
			int aNodeId2 = rNodesInMesh.eNodeIds[j + 1];

			if(aNodeId2 < aNodeId1) {
				int aTmp = aNodeId1;
				aNodeId1 = aNodeId2;
				aNodeId2 = aTmp;
			}

			rMap[NodeIdPair(aNodeId1, aNodeId2)] = cRecord;
		}
	}
}

const KKDRegulationRoadLinkLayer2::Record* KKDRegulationRoadLinkLayer2::searchFromNodeQueue(int cMeshcode, int nNodeId1, int nNodeId2) const
{
	IndexedNodeQueueRecordMap::const_iterator it1 = m_cIndexedNodeQueueRecordMap.find(cMeshcode);
	if(it1 == m_cIndexedNodeQueueRecordMap.end())
		return 0;

	std::map<NodeIdPair, Record*>::const_iterator it2 = it1->second.find(NodeIdPair(nNodeId1, nNodeId2));
	if(it2 != it1->second.end())
		return it2->second;

	it2 = it1->second.find(NodeIdPair(nNodeId2, nNodeId1));
	if(it2 != it1->second.end())
		return it2->second;

	return 0;
}

// KKDLaneRegulationRoadNodeLayer2
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// オーバーライド
//////////////////////////////////////////////////////////////////////

void KKDLaneRegulationRoadNodeLayer2::clearIndex()
{
	_KKDLaneRegulationRoadNodeLayer2::clearIndex();
	m_cIndexedAppertainRecordMap.clear();
}

void KKDLaneRegulationRoadNodeLayer2::setIndex()
{
	_KKDLaneRegulationRoadNodeLayer2::setIndex();
	for(iterator it = begin(); it != end(); ++it) {
		indexAppertainRecord(&*it);
	}
}

//////////////////////////////////////////////////////////////////////
// 設定/取得
//////////////////////////////////////////////////////////////////////

void KKDLaneRegulationRoadNodeLayer2::indexAppertainRecord(Record* cRecord)
{
	for(int i = 0; i < cRecord->eAppertainNodeCount; i++) {	
		const Record::AppertainNode& rAppertainNode = cRecord->eAppertainNodes[i];
		m_cIndexedAppertainRecordMap[KKDRegulationRoadNode::AppertainNode(rAppertainNode.eNodeMeshDirection, rAppertainNode.eNodeId)] = cRecord;
	}
}

const KKDLaneRegulationRoadNodeLayer2::Record* KKDLaneRegulationRoadNodeLayer2::searchFromAppertain(const KKDRegulationRoadNode::AppertainNode& cAppretainNode) const
{
	IndexedAppertainRecordMap::const_iterator it = m_cIndexedAppertainRecordMap.find(cAppretainNode);
	return it != m_cIndexedAppertainRecordMap.end() ? it->second : 0;
}

// KKDLaneRegulationRoadLinkLayer2
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// オーバーライド
//////////////////////////////////////////////////////////////////////

void KKDLaneRegulationRoadLinkLayer2::clearIndex()
{
	_KKDLaneRegulationRoadLinkLayer2::clearIndex();
	m_cIndexedNodeQueueRecordMap.clear();
}

void KKDLaneRegulationRoadLinkLayer2::setIndex()
{
	_KKDLaneRegulationRoadLinkLayer2::setIndex();
	for(iterator it = begin(); it != end(); ++it) {
		indexNodeQueueRecord(&*it);
	}
}

//////////////////////////////////////////////////////////////////////
// 設定/取得
//////////////////////////////////////////////////////////////////////

void KKDLaneRegulationRoadLinkLayer2::indexNodeQueueRecord(Record* cRecord)
{
	for(int i = 0; i < cRecord->eMeshCount; i++) {
		const Record::NodesInMesh& rNodesInMesh = cRecord->eNodesInMeshs[i];
		std::map<NodeIdPair, Record*>& rMap = m_cIndexedNodeQueueRecordMap[rNodesInMesh.eMeshCode];
		for(int j = 0; j < rNodesInMesh.eNodeCount - 1; j++) {
			int aNodeId1 = rNodesInMesh.eNodeIds[j];
			int aNodeId2 = rNodesInMesh.eNodeIds[j + 1];

			if(aNodeId2 < aNodeId1) {
				int aTmp = aNodeId1;
				aNodeId1 = aNodeId2;
				aNodeId2 = aTmp;
			}

			rMap[NodeIdPair(aNodeId1, aNodeId2)] = cRecord;
		}
	}
}

const KKDLaneRegulationRoadLinkLayer2::Record* KKDLaneRegulationRoadLinkLayer2::searchFromNodeQueue(int cMeshcode, int nNodeId1, int nNodeId2) const
{
	IndexedNodeQueueRecordMap::const_iterator it1 = m_cIndexedNodeQueueRecordMap.find(cMeshcode);
	if(it1 == m_cIndexedNodeQueueRecordMap.end())
		return 0;

	std::map<NodeIdPair, Record*>::const_iterator it2 = it1->second.find(NodeIdPair(nNodeId1, nNodeId2));
	if(it2 != it1->second.end())
		return it2->second;

	it2 = it1->second.find(NodeIdPair(nNodeId2, nNodeId1));
	if(it2 != it1->second.end())
		return it2->second;

	return 0;
}
