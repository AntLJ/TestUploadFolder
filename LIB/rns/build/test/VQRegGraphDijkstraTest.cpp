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
#include <boost/assign/std/vector.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <rns/graph/vq_regulation/dijkstra_shortest_paths.h>

using namespace boost;
using namespace rns::graph::vq_regulation;
namespace vqreg_graph = rns::graph::vq_regulation;

namespace {

typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

typedef dijkstra_path<Vertex, int> Path;


/**
 * @brief 検証用のグラフAを作成する。
 *
 * @verbatim
 * 0 - 1 - 2 - 3
 * |   |       |
 * 4 - 5 - 6 - 7
 *     |
 *     8
 *
 * 各辺の重み
 * 1: 0-1 1-2 2-3 0-4 4-5 5-8
 * 2: 5-6
 * 3: 3-7 6-7
 * 5: 1-5
 * @endverbatim
 */
std::auto_ptr<Graph> create_graphA()
{
	typedef std::pair<int, int> Edge;

	const Edge aEdges[] = {
		Edge(0, 1), Edge(1, 2), Edge(2, 3),
		Edge(0, 4), Edge(1, 5), Edge(3, 7),
		Edge(4, 5), Edge(5, 6), Edge(6, 7),
		Edge(5, 8),
	};

	const long aWeights[] = {
		1, 1, 1,
		1, 5, 3,
		1, 2, 3,
		1,
	};

	return std::auto_ptr<Graph>(new Graph(aEdges, aEdges + sizeof(aEdges) / sizeof(aEdges[0]), aWeights, 9));
}

typedef std::list<Path> Paths;

} // anonymous namespace

class VQRegGraphDijkstraTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(VQRegGraphDijkstraTest);
	CPPUNIT_TEST(testNoReg);
	CPPUNIT_TEST(testReg);
	CPPUNIT_TEST_SUITE_END();

public:
	/**
	 * @brief グラフAにおいて、<b>頂点8</b>から<b>頂点3</b>に到達する最短経路を求める。
	 */
	void testNoReg()
	{
		const std::auto_ptr<Graph> pGraph(create_graphA());

		std::vector<std::vector<std::vector<Vertex> > > aVQRegs(9);
		std::vector<Paths> aPathsMap(9);

		dijkstra_queue<Path> aQueue;
		push_path(aQueue, aPathsMap[8], Path(8));

		dijkstra_shortest_paths(
			*pGraph,
			aQueue,
			&aVQRegs[0],
			&aPathsMap[0],
			get(edge_weight, *pGraph),
			vqreg_graph::dijkstra_visitor()
		);

		const Paths rPaths = aPathsMap[3];

		CPPUNIT_ASSERT_EQUAL(2U, rPaths.size());
		const Path& rPath1 = rPaths.front();
		const Path& rPath2 = *boost::next(rPaths.begin());

		CPPUNIT_ASSERT_EQUAL(6, path_traits<Path>::distance(rPath1));
		CPPUNIT_ASSERT_EQUAL(9, path_traits<Path>::distance(rPath2));

		std::ostringstream ossActual;
		std::copy(rPath1.vertices_begin(), rPath1.vertices_end(), std::ostream_iterator<Vertex>(ossActual));
		CPPUNIT_ASSERT_EQUAL(std::string("3210458"), ossActual.str());

		ossActual.str("");
		std::copy(rPath2.vertices_begin(), rPath2.vertices_end(), std::ostream_iterator<Vertex>(ossActual));
		CPPUNIT_ASSERT_EQUAL(std::string("37658"), ossActual.str());
	}

	/**
	 * @brief グラフAにおいて、5→4→0の頂点列規制を回避し、<b>頂点8</b>から<b>頂点3</b>に到達する最短経路を求める。
	 */
	void testReg()
	{
		const std::auto_ptr<Graph> pGraph(create_graphA());

		std::vector<std::vector<std::vector<Vertex> > > aVQRegs(9);
		std::vector<Paths> aPathsMap(9);

		using namespace boost::assign;

		std::vector<Vertex> aReg;
		aReg += 5,4,0;

		aVQRegs[5].push_back(aReg);
		aVQRegs[4].push_back(aReg);
		aVQRegs[0].push_back(aReg);

		dijkstra_queue<Path> aQueue;
		push_path(aQueue, aPathsMap[8], Path(8));

		dijkstra_shortest_paths(
			*pGraph,
			aQueue,
			&aVQRegs[0],
			&aPathsMap[0],
			get(edge_weight, *pGraph),
			vqreg_graph::dijkstra_visitor()
		);

		const Paths rPaths = aPathsMap[3];

		CPPUNIT_ASSERT_EQUAL(2U, rPaths.size());
		const Path& rPath1 = rPaths.front();
		const Path& rPath2 = *boost::next(rPaths.begin());

		CPPUNIT_ASSERT_EQUAL(9, path_traits<Path>::distance(rPath1));
		CPPUNIT_ASSERT_EQUAL(8, path_traits<Path>::distance(rPath2));

		std::ostringstream ossActual;
		std::copy(rPath1.vertices_begin(), rPath1.vertices_end(), std::ostream_iterator<Vertex>(ossActual));
		CPPUNIT_ASSERT_EQUAL(std::string("37658"), ossActual.str());

		ossActual.str("");
		std::copy(rPath2.vertices_begin(), rPath2.vertices_end(), std::ostream_iterator<Vertex>(ossActual));
		CPPUNIT_ASSERT_EQUAL(std::string("32158"), ossActual.str());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(VQRegGraphDijkstraTest);
