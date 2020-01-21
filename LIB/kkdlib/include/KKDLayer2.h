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

// KKDLayer2.h
//
//////////////////////////////////////////////////////////////////////

#ifndef __KKD_LAYER2_H__
#define __KKD_LAYER2_H__

#include <DRMALayer2.h>
#include "KKDLayer.h"

typedef DRMALayer2T<KKDRegulationRoadNodeLayer> _KKDRegulationRoadNodeLayer2;
class KKDRegulationRoadNodeLayer2 : public _KKDRegulationRoadNodeLayer2 
{
public:
	typedef std::map<Record::AppertainNode, Record*> IndexedAppertainRecordMap;

// オーバーライド
	virtual void clearIndex();
	virtual void setIndex();

// 設定/取得
	const Record* searchFromAppertain(const Record::AppertainNode& cAppretainNode) const;

private:
// 設定/取得
	void indexAppertainRecord(Record* cRecord);
	
// プロパティ
	IndexedAppertainRecordMap m_cIndexedAppertainRecordMap;
};

typedef DRMALayer2T<KKDRegulationRoadLinkLayer> _KKDRegulationRoadLinkLayer2;
class KKDRegulationRoadLinkLayer2 : public _KKDRegulationRoadLinkLayer2
{
public:
	typedef std::pair<int, int> NodeIdPair;
	typedef std::map<int, std::map<NodeIdPair, Record*> > IndexedNodeQueueRecordMap;

// オーバーライド
	virtual void clearIndex();
	virtual void setIndex();

// 設定/取得
	const Record* searchFromNodeQueue(int cMeshcode, int nNode1Id, int nNode2Id) const;

private:
// 設定/取得
	void indexNodeQueueRecord(Record* cRecord);

// プロパティ
	IndexedNodeQueueRecordMap m_cIndexedNodeQueueRecordMap;
};

typedef DRMALayer2T<KKDLaneRegulationRoadNodeLayer> _KKDLaneRegulationRoadNodeLayer2;
class KKDLaneRegulationRoadNodeLayer2 : public _KKDLaneRegulationRoadNodeLayer2
{
public:
	typedef std::map<KKDRegulationRoadNode::AppertainNode, Record*> IndexedAppertainRecordMap;

// オーバーライド
	virtual void clearIndex();
	virtual void setIndex();

// 設定/取得
	const Record* searchFromAppertain(const KKDRegulationRoadNode::AppertainNode& cAppretainNode) const;

private:
// 設定/取得
	void indexAppertainRecord(Record* cRecord);
	
// プロパティ
	IndexedAppertainRecordMap m_cIndexedAppertainRecordMap;
};

typedef DRMALayer2T<KKDLaneRegulationRoadLinkLayer> _KKDLaneRegulationRoadLinkLayer2;
class KKDLaneRegulationRoadLinkLayer2 : public _KKDLaneRegulationRoadLinkLayer2
{
public:
	typedef std::pair<int, int> NodeIdPair;
	typedef std::map<int, std::map<NodeIdPair, Record*> > IndexedNodeQueueRecordMap;

// オーバーライド
	virtual void clearIndex();
	virtual void setIndex();

// 設定/取得
	const Record* searchFromNodeQueue(int cMeshcode, int nNode1Id, int nNode2Id) const;

private:
// 設定/取得
	void indexNodeQueueRecord(Record* cRecord);

// プロパティ
	IndexedNodeQueueRecordMap m_cIndexedNodeQueueRecordMap;
};

typedef DRMALayer2T<KKDLaneRegulationIntersectionNodeLayer> KKDLaneRegulationIntersectionNodeLayer2;

#endif // __KKD_LAYER2_H__
