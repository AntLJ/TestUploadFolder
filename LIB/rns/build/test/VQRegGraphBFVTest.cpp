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
#include <memory>
#include <queue>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <boost/assign/std/vector.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <rns/graph/vq_regulation/breadth_first_search.h>

using namespace boost;
using namespace rns::graph::vq_regulation;
namespace vqreg_graph = rns::graph::vq_regulation;

namespace {

typedef adjacency_list<vecS, vecS, undirectedS> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

typedef path<Vertex> Path;

/**
 * @brief 検証用のグラフAを作成する。
 *
 * @verbatim
 * 0 - 1
 * |   |
 * 2 - 3 - 4
 *     |
 *     5
 * @endverbatim
 */
std::auto_ptr<Graph> create_graphA()
{
	typedef std::pair<int, int> Edge;

	const Edge aEdges[] = {
		Edge(0, 1), Edge(0, 2),
		Edge(1, 3), Edge(2, 3),
		Edge(3, 4), Edge(3, 5),
	};

	return std::auto_ptr<Graph>(new Graph(aEdges, aEdges + sizeof(aEdges) / sizeof(aEdges[0]), 6));
}

/**
 * @brief 検証用のグラフBを作成する。
 *
 * @verbatim
 * 0 - 1 - 2 - 3
 * |   |       |
 * 4 - 5 - 6 - 7
 *     |
 *     8
 * @endverbatim
 */
std::auto_ptr<Graph> create_graphB()
{
	typedef std::pair<int, int> Edge;

	const Edge aEdges[] = {
		Edge(0, 1), Edge(1, 2), Edge(2, 3),
		Edge(0, 4), Edge(1, 5), Edge(3, 7),
		Edge(4, 5), Edge(5, 6), Edge(6, 7),
		Edge(5, 8),
	};

	return std::auto_ptr<Graph>(new Graph(aEdges, aEdges + sizeof(aEdges) / sizeof(aEdges[0]), 9));
}
typedef std::list<Path> Paths;

} // anonymous namespace

class VQRegGraphBFVTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(VQRegGraphBFVTest);
	CPPUNIT_TEST(testNoReg);
	CPPUNIT_TEST(testReg);
	CPPUNIT_TEST(testReg1);
	CPPUNIT_TEST_SUITE_END();

public:
	/**
	 * @brief <b>頂点5</b>から<b>頂点4</b>に到達する最短経路を求める。
	 */
	void testNoReg()
	{
		const std::auto_ptr<Graph> pGraph(create_graphA());

		std::vector<std::vector<std::vector<Vertex> > > aVQRegs(6);
		std::vector<Paths> aPathsMap(6);

		std::queue<const Path*> aQueue;
		push_path(aQueue, aPathsMap[5], Path(5));

		breadth_first_visit(*pGraph, aQueue, &aVQRegs[0], &aPathsMap[0], vqreg_graph::bfs_visitor());

		CPPUNIT_ASSERT_EQUAL(1U, aPathsMap[4].size());
		const Path& rPred = aPathsMap[4].front();

		std::ostringstream ossActual;
		std::copy(rPred.vertices_begin(), rPred.vertices_end(), std::ostream_iterator<Vertex>(ossActual));

		CPPUNIT_ASSERT_EQUAL(std::string("435"), ossActual.str());
	}

	/**
	 * @brief 5→3→4の頂点列規制を回避し、<b>頂点5</b>から<b>頂点4</b>に到達する最短経路を求める。
	 */
	void testReg()
	{
		const std::auto_ptr<Graph> pGraph(create_graphA());

		std::vector<std::vector<std::vector<Vertex> > > aVQRegs(6);
		std::vector<Paths> aPathsMap(6);

		using namespace boost::assign;

		std::vector<Vertex> aReg;
		aReg += 5,3,4;

		aVQRegs[5].push_back(aReg);
		aVQRegs[3].push_back(aReg);
		aVQRegs[4].push_back(aReg);

		std::queue<const Path*> aQueue;
		push_path(aQueue, aPathsMap[5], Path(5));

		breadth_first_visit(*pGraph, aQueue, &aVQRegs[0], &aPathsMap[0], vqreg_graph::bfs_visitor());

		CPPUNIT_ASSERT_EQUAL(1U, aPathsMap[4].size());
		const Path& rPred = aPathsMap[4].front();

		std::ostringstream ossActual;
		std::copy(rPred.vertices_begin(), rPred.vertices_end(), std::ostream_iterator<Vertex>(ossActual));

		CPPUNIT_ASSERT_EQUAL(std::string("4320135"), ossActual.str());
	}

	/**
	 * @brief 1点規制を検証する。
	 */
	void testReg1()
	{
		const std::auto_ptr<Graph> pGraph(create_graphB());

		std::vector<std::vector<std::vector<Vertex> > > aVQRegs(9);
		std::vector<Paths> aPathsMap(9);

		using namespace boost::assign;

		std::vector<Vertex> aReg(1, 6);
		aVQRegs[6].push_back(aReg);

		std::queue<const Path*> aQueue;
		push_path(aQueue, aPathsMap[8], Path(8));

		breadth_first_visit(*pGraph, aQueue, &aVQRegs[0], &aPathsMap[0], vqreg_graph::bfs_visitor());

		CPPUNIT_ASSERT_EQUAL(1U, aPathsMap[7].size());
		const Path& rPred = aPathsMap[7].front();

		std::ostringstream ossActual;
		std::copy(rPred.vertices_begin(), rPred.vertices_end(), std::ostream_iterator<Vertex>(ossActual));

		CPPUNIT_ASSERT_EQUAL(std::string("732158"), ossActual.str());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(VQRegGraphBFVTest);
