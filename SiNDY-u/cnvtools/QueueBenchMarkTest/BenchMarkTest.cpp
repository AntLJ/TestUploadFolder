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

// QueueBenchMarkTest.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "shproaddata.h"
#include <time.h>

static const char *nqfilename = "D:\\temp\\data\\road\\5339\\533946\\533946_NqTurn";
static const char *inffilename = "D:\\temp\\data\\road\\5339\\533946\\533946_InfTurn";
static const char *lqfilename = "D:\\temp\\data\\road\\5339\\533946\\533946_LqTurn";
static const char *inffilename2 = "D:\\temp\\data\\road\\5339\\533946\\533946_InfTurn2";
static const char *linkfilename = "D:\\temp\\data\\road\\5339\\533946\\533946_RoadLink";
static const char *nodefilename = "D:\\temp\\data\\road\\5339\\533946\\533946_RoadNode";

static clock_t st;
static clock_t ed;

void start()
{
	st = clock();
}

void stop()
{
	ed = clock();
}

void print(const char *mess)
{
	if (mess)
		std::cout << mess << " : ";
	std::cout << (double)(ed - st) << std::endl;
}

bool find_crossing(Queue *nq, ShpNode *node)
{
	long nIndex = DBFGetFieldIndex(&nq->m_lqhdl, shape::road::nodequeue::kSecondNode);
	long nIndex2 = DBFGetFieldIndex(&nq->m_lqhdl, shape::road::nodequeue::kSequence);

	for (DBFAttributes::iterator it = nq->m_queue.begin(); it != nq->m_queue.end(); it++) {
		if (it->second[nIndex2].lVal != 1)
			continue;
		ShpEntity aNode;
		if (! node->find_node(it->second[nIndex].lVal, &aNode))
			std::cout << it->second[nIndex].lVal << ": node not found." << std::endl;
	}
	return true;
}

bool find_crossing(Queue *lq, ShpLink *link, ShpNode *node)
{
	long nIndex = DBFGetFieldIndex(&lq->m_lqhdl, shape::road::linkqueue::kLinkId);
	long nIndex2 = DBFGetFieldIndex(&lq->m_lqhdl, shape::road::linkqueue::kSequence);
	long nIndex3 = DBFGetFieldIndex(&lq->m_lqhdl, shape::road::linkqueue::kLinkDir);
	long nIndex4 = DBFGetFieldIndex(&link->m_dbfhdl, shape::road::road_link::kFromNodeId);
	long nIndex5 = DBFGetFieldIndex(&link->m_dbfhdl, shape::road::road_link::kToNodeId);

	for (DBFAttributes::iterator it = lq->m_queue.begin(); it != lq->m_queue.end(); it++) {
		// シーケンスが1のもののみ
		if (it->second[nIndex2].lVal != 1)
			continue;

		// リンクの検索
		ShpEntity aLink;
		if (! link->find_link(it->second[nIndex].lVal, &aLink)) {
			std::cout << it->second[nIndex].lVal << " : link not found." << std::endl;
			continue;
		}

		// リンクの向き
		long p = (it->second[nIndex3].lVal == 1) ? nIndex4 : nIndex5;

		// ノードの取得
		ShpEntity aNode;
		if (! node->find_node(aLink.m_vecAttributes[p].lVal, &aNode))
			std::cout << it->second[nIndex].lVal << " : node not found." << std::endl;
	}

	return true;
}

bool find_inner(Queue *nq, ShpLink *link)
{
	long nIndex = DBFGetFieldIndex(&nq->m_lqhdl, shape::road::nodequeue::kSecondNode);
	long nIndex2 = DBFGetFieldIndex(&nq->m_lqhdl, shape::road::nodequeue::kFirstNode);

	for (DBFAttributes::iterator it = nq->m_queue.begin(); it != nq->m_queue.end(); it++) {
		if (it->second[nIndex2].lVal != 1)
			continue;
		ShpEntity aEntity;
		if (! link->find_link2(it->second[nIndex2].lVal, it->second[nIndex].lVal, &aEntity))
			std::cout << it->second[nIndex2].lVal << "-" << it->second[nIndex].lVal << " : link not found." << std::endl;
	}
	return true;
}

bool find_inner2(Queue *nq, ShpLink *link)
{
	long nIndex = DBFGetFieldIndex(&nq->m_lqhdl, shape::road::nodequeue::kSecondNode);
	long nIndex2 = DBFGetFieldIndex(&nq->m_lqhdl, shape::road::nodequeue::kFirstNode);

	for (DBFAttributes::iterator it = nq->m_queue.begin(); it != nq->m_queue.end(); it++) {
		if (it->second[nIndex2].lVal != 1)
			continue;
		ShpEntity aEntity;
		if (! link->find_link(it->second[nIndex2].lVal, it->second[nIndex].lVal, &aEntity))
			std::cout << it->second[nIndex2].lVal << "-" << it->second[nIndex].lVal << " : link not found." << std::endl;
	}
	return true;
}

bool find_inner3(Queue *lq, ShpLink *link)
{
	long nIndex = DBFGetFieldIndex(&lq->m_lqhdl, shape::road::linkqueue::kLinkId);
	long nIndex2 = DBFGetFieldIndex(&lq->m_lqhdl, shape::road::linkqueue::kSequence);

	for (DBFAttributes::iterator it = lq->m_queue.begin(); it != lq->m_queue.end(); it++) {
		if (it->second[nIndex2].lVal != 1)
			continue;
		ShpEntity aLink;
		if (! link->find_link(it->second[nIndex].lVal, &aLink)) {
			std::cout << it->second[nIndex].lVal << " : link not found." << std::endl;
			continue;
		}
	}
	return true;
}

typedef std::pair<DBFAttributes::iterator,DBFAttributes::iterator> DBFRange;

bool find_nq(ShpLink *link, Queue *nq)
{
	long nIndex1 = DBFGetFieldIndex(&link->m_dbfhdl, shape::road::road_link::kFromNodeId);
	long nIndex2 = DBFGetFieldIndex(&link->m_dbfhdl, shape::road::road_link::kToNodeId);
	long nIndex3 = DBFGetFieldIndex(&nq->m_lqhdl, shape::road::nodequeue::kSequence);
	long nIndex4 = DBFGetFieldIndex(&nq->m_lqhdl, shape::road::nodequeue::kFirstNode);

	for (SHPEntities::iterator it = link->m_entities.begin(); it != link->m_entities.end(); it++) {
		long count = 0;
		DBFRange range = nq->m_map.equal_range(it->second.m_vecAttributes[nIndex1].lVal);
		{
			for (DBFAttributes::iterator p = range.first; p != range.second; p++) {
				if (p->second[nIndex3].lVal != 1)
					continue;
				if (p->second[nIndex4].lVal == it->second.m_vecAttributes[nIndex2].lVal) {
					count++;
				}
			}
		}
		range = nq->m_map.equal_range(it->second.m_vecAttributes[nIndex2].lVal);
		{
			for (DBFAttributes::iterator p = range.first; p != range.second; p++) {
				if (p->second[nIndex3].lVal != 1)
					continue;
				if (p->second[nIndex4].lVal == it->second.m_vecAttributes[nIndex1].lVal) {
					count++;
				}
			}
		}
	}
	return true;
}

bool find_lq(ShpLink *link, Queue *lq)
{
	long nIndex1 = DBFGetFieldIndex(&link->m_dbfhdl, shape::kObjectId);

	for (SHPEntities::iterator it = link->m_entities.begin(); it != link->m_entities.end(); it++) {
		long count = 0;
		DBFRange range = lq->m_map.equal_range(it->second.m_vecAttributes[nIndex1].lVal);
		for (DBFAttributes::iterator p = range.first; p != range.second; p++)
			count++;
	}
	return true;
}

long count(Queue *nq)
{
	long nIndex = DBFGetFieldIndex(&nq->m_lqhdl, shape::road::nodequeue::kSequence);

	long count = 0;
	for (DBFAttributes::iterator it = nq->m_queue.begin(); it != nq->m_queue.end(); it++) {
		if (it->second[nIndex].lVal != 1)
			continue;
		count++;
	}
	return count;
}

int main(int argc, char* argv[])
{
	Queue nq, lq;
	ShpLink link;
	ShpNode node;

	start();
	if (! nq.read(nqfilename, inffilename, shape::road::nodequeue::kSecondNode))
		return -1;
	stop();
	print("node-queue file read");

	start();
	if (! lq.read(lqfilename, inffilename2, shape::road::linkqueue::kLinkId))
		return -1;
	stop();
	print("link-queue file read");

	printf("ノード列総件数：%d\n", count(&nq));

	start();
	if (! link.read(linkfilename))
		return -1;
	stop();
	print("link file read");

	start();
	if (! node.read(nodefilename))
		return -1;
	stop();
	print("node file read");

	// リンクIDテーブルの生成
	start();
	if (! link.mk_linkid_table())
		return -1;
	stop();
	print("LinkId Table create");

	// 対象交差点ノードの取得
	start();
	if (! find_crossing(&nq, &node))
		return -1;
	stop();
	print("NQ Crossing find");

	start();
	if (! find_crossing(&lq, &link, &node))
		return -1;
	stop();
	print("LQ Crossing find");

	// 進入リンクの取得
	start();
	if (! find_inner(&nq, &link))
		return -1;
	stop();
	print("NQ Inner find");

	start();
	if (! find_inner2(&nq, &link))
		return -1;
	stop();
	print("NQ Inner find 2");

	start();
	if (! find_inner3(&lq, &link))
		return -1;
	stop();
	print("LQ Inner find");

	// リンクからの取得
	start();
	if (! find_nq(&link, &nq))
		return -1;
	stop();
	print("Link nq find");

	start();
	if (! find_lq(&link, &lq))
		return -1;
	stop();
	print("Link lq find");
/*
	// ノードからの取得
	start();
	if (! find_nq2(&node, &nq))
		return -1;
	stop();
	print("Node nq find");

	start();
	if (! find_lq2(&node, &link, &nq))
		return -1;
	stop();
	printf("Node lq find");
*/
	return 0;
}
