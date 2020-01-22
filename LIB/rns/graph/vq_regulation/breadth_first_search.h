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

#ifndef RNS_VQ_REGULATION_GRAPH_BREADTH_FIRST_SEARCH_H_
#define RNS_VQ_REGULATION_GRAPH_BREADTH_FIRST_SEARCH_H_

#include <boost/graph/graph_traits.hpp>
#include <boost/version.hpp>
#if BOOST_VERSION < 104000
# include <boost/property_map.hpp>
#else
# include <boost/property_map/property_map.hpp>
#endif // BOOST_VERSION < 104000
#include <rns/graph/vq_regulation/detail/path.h>
#include <rns/graph/vq_regulation/detail/overlap_length.h>

namespace rns {

namespace graph {

namespace vq_regulation {

/**
 * @brief breadth_first_visit() 用ビジタ
 */
struct bfs_visitor
{
	template <class Vertex, class Graph>
	void examine_vertex(Vertex, Graph&) {}

	template <class Vertex, class Graph>
	void finish_vertex(Vertex, Graph&) {}
};

/**
 * @brief 頂点列規制を考慮した幅優先経路探索アルゴリズム。
 *
 * 通常の幅優先経路探索とは異なり、流入頂点さえ異なれば、同じ頂点を二回以上通過することができる。
 *
 * @param g [in] 有向グラフまたは無向グラフ。グラフの型は Incidence Graph のモデルでなければならない。 
 * @param Q [in] 頂点が発見される順序を決定するために使用される経路ポインタキュー。
 * @param vqregs [in] 関連頂点列規制マップ（頂点属性）。
 * @param paths [in] 経路コンテナマップ（頂点属性）。コンテナは std::list に限定。
 * @param vis [in] ビジタ。
 */
template <class Graph, class Buffer, class VQRegulationsMap, class PathsMap, class BFSVisitor>
void breadth_first_visit(
	const Graph& g,
	Buffer& Q,
	VQRegulationsMap vqregs,
	PathsMap paths,
	BFSVisitor vis
)
{
	using namespace boost;

	typedef graph_traits<Graph> GTraits;
	typedef typename GTraits::vertex_descriptor Vertex;
	typedef typename GTraits::edge_descriptor Edge;

	typedef typename property_traits<VQRegulationsMap>::value_type VQRegulations;
	typedef typename property_traits<PathsMap>::value_type Paths;
	typedef paths_traits<Paths> PathsTraits;

	typedef typename PathsTraits::path_type Path;
	typedef typename PathsTraits::path_traits PathTraits;

	while(! Q.empty()) {
		// キューから現行頂点情報を取り出す。
		const Buffer::value_type p = Q.front();
		Q.pop();

		const typename PathTraits::const_vertices_iterator
			pi_begin(PathTraits::vertices_begin(*p)), // 現行頂点を指す反復子
			pi_second(next(pi_begin)), // 先行頂点を指す反復子
			pi_end(PathTraits::vertices_end(*p)); // 先行頂点列の終点を指す反復子

		const Vertex u(*pi_begin);

		vis.examine_vertex(u, g);

		// 現行頂点から流出する全ての辺を調べる。
		typename GTraits::out_edge_iterator ei, ei_end;
		for(boost::tie(ei, ei_end) = out_edges(u, g); ei != ei_end; ++ei) {
			const Vertex v(target(*ei, g));

			// Ｕターンするような経路は却下。
			if(pi_second != pi_end && v == *pi_second)
				continue;

			// 頂点列規制を確認する。
			const VQRegulations& rs(vqregs[v]);
			const unsigned int longest_regulation_overlap_length = typical_path_length(rs, v, pi_begin, pi_end);
			if(longest_regulation_overlap_length == 0)
				continue;

			push_path(
				Q,
				paths[v],
				PathTraits::make_path(v, p, longest_regulation_overlap_length - 1)
			);
		}

		vis.finish_vertex(u, g);
	}
}

} // namespace vq_regulation

} // namespace graph

} // namespace rns

#endif // RNS_VQ_REGULATION_GRAPH_BREADTH_FIRST_SEARCH_H_
