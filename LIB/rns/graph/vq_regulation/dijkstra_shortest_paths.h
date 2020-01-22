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

#ifndef RNS_VQ_REGULATION_GRAPH_DIJKSTRA_SHORTEST_PATHS_H_
#define RNS_VQ_REGULATION_GRAPH_DIJKSTRA_SHORTEST_PATHS_H_

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
 * @brief ダイクストラ最短経路探索アルゴリズム用経路クラス
 *
 * @par テンプレート引数:
 * - @b VertexDescriptor - グラフの頂点記述子型
 * - @b Distance - コストの型
 */
template <typename VertexDescriptor, typename Distance>
class dijkstra_path : public detail::path_base<dijkstra_path<VertexDescriptor, Distance>, VertexDescriptor>
{
public:
	typedef Distance distance_type;

	/**
	 * @brief 始点経路を作成する。
	 *
	 * @param v [in] 経路始点頂点。
	 * @param d [in] 開始コスト。
	 */
	explicit dijkstra_path(vertex_descriptor v, distance_type d = distance_type()) :
	super_type(v),
	m_dist(d)
	{
	}

	/**
	 * @brief 先行頂点情報を作成する。
	 *
	 * @param v [in] 到達頂点。
	 * @param d [in] 総コスト。
	 * @param pp [in] 先行経路。
	 * @param reg [in] 頂点列規制に絡んでいる先行頂点の数（到達頂点を含まない）。
	 */
	dijkstra_path(vertex_descriptor v, distance_type d, const dijkstra_path* pp, unsigned int reg) :
	super_type(v, pp, reg),
	m_dist(d)
	{
	}

	distance_type m_dist; ///< 総コスト。
};

/**
 * @brief ダイクストラ最短経路探索アルゴリズム用優先順位付きキュー
 *
 * フィボナッチヒープ等使った方が良いのだろうが、面倒なので二分木（ multimap ）で実装。
 */
template <typename Path>
class dijkstra_queue
{
public:
	typedef Path path_type;
	typedef typename path_traits<path_type>::distance_type distance_type;

	typedef std::multimap<distance_type, const Path*> container_type;
	typedef typename container_type::size_type size_type;
	typedef const Path* value_type;

	bool empty() const
	{
		return m_Container.empty();
	}

	size_type size() const
	{
		return m_Container.size();
	}

	void push(const Path* pp)
	{
		m_Container.insert(container_type::value_type(path_traits<path_type>::distance(*pp), pp));
	}

	void pop()
	{
		m_Container.erase(m_Container.begin());
	}

	const value_type& top() const
	{
		return m_Container.begin()->second;
	}

	void remove(const Path* pp)
	{
		for(container_type::iterator it(m_Container.begin()), it_next; it != m_Container.end(); it = it_next) {
			++(it_next = it);
			if(it->second == pp) {
				m_Container.erase(it);
			}
		}
	}

private:
	container_type m_Container;
};

/**
 * @brief ダイクストラ最短経路探索アルゴリズム用経路型特性
 */
template <typename VertexDescriptor, typename Distance>
struct path_traits<dijkstra_path<VertexDescriptor, Distance> > :
	public detail::path_traits_base<dijkstra_path<VertexDescriptor, Distance> >
{
	typedef typename path_type::distance_type distance_type;

	/**
	 * @brief 新経路を作成する。
	 *
	 * @param v [in] 到達頂点。
	 * @param weight [in] 重み。
	 * @param pp [in] 先行経路。
	 * @param reg [in] 頂点列規制に絡んでいる先行頂点の数（到達頂点を含まない）。
	 */
	static path_type make_path(vertex_descriptor v, Distance weight, const path_type* pp, unsigned int reg)
	{
		return path_type(v, pp ? distance(*pp) + weight : weight, pp, reg);
	}

	static distance_type distance(const path_type& p)
	{
		return p.m_dist;
	}
};

/**
 * @brief ダイクストラ最短経路探索アルゴリズム用経路コンテナ特性
 */
template <typename VertexDescriptor, typename Distance, typename Allocator>
struct paths_traits<std::list<dijkstra_path<VertexDescriptor, Distance>, Allocator > > :
	public detail::paths_traits_base<std::list<dijkstra_path<VertexDescriptor, Distance>, Allocator > >
{
	template <typename Queue>
	static bool push_path(Queue& Q, paths_type& ps, const path_type& p)
	{
		path_type* pp = typical_find(ps, p);
		if(pp) {
			if(path_traits::distance(p) >= path_traits::distance(*pp))
				return false;

			Q.remove(pp);
			Q.push(&(*pp = p));
		}
		else {
			Q.push(push(ps, p));
		}

		return true;
	}
};

/**
 * @brief dijkstra_shortest_paths() 用ビジタ
 */
struct dijkstra_visitor
{
	template <class Vertex, class Graph>
	void examine_vertex(Vertex, Graph&) {}

	template <class Vertex, class Graph>
	void finish_vertex(Vertex, Graph&) {}
};

/**
 * @brief 頂点列規制を考慮したダイクストラ最短経路探索アルゴリズム。
 *
 * 通常のダイクストラ最短経路探索とは異なり、流入頂点さえ異なれば、同じ頂点を二回以上通過することができる。
 *
 * @param g [in] 有向グラフまたは無向グラフ。グラフの型は Incidence Graph のモデルでなければならない。 
 * @param Q [in] 頂点が発見される順序を決定するために使用される dijkstra_queue 。
 * @param vqregs [in] 関連頂点列規制マップ（頂点属性）。
 * @param paths [in] 経路コンテナマップ（頂点属性）。コンテナは std::list に限定。
 * @param weight [in] コストマップ（辺属性）。
 * @param vis [in] ビジタ。
 */
template <
	typename Graph,
	typename Buffer,
	typename VQRegulationsMap,
	typename PathsMap,
	typename WeightMap,
	typename DijkstraVisitor
>
void dijkstra_shortest_paths(
	const Graph& g,
	Buffer& Q,
	VQRegulationsMap vqregs,
	PathsMap paths,
	WeightMap weight,
	DijkstraVisitor vis
)
{
	using namespace boost;

	typedef graph_traits<Graph> GTraits; // グラフ特性
	typedef typename GTraits::vertex_descriptor Vertex; // 頂点記述子型
	typedef typename GTraits::edge_descriptor Edge; // 辺記述子型

	typedef typename property_traits<VQRegulationsMap>::value_type VQRegulations; // 頂点列規制型
	typedef typename property_traits<PathsMap>::value_type Paths; // 経路コンテナ型
	typedef paths_traits<Paths> PathsTraits; // 経路コンテナ特性

	typedef typename PathsTraits::path_type Path; // 経路型
	typedef typename PathsTraits::path_traits PathTraits; // 経路型特性

	typedef typename property_traits<WeightMap>::value_type Weight; // 重み型

	while(! Q.empty()) {
		// キューから現行頂点情報を取り出す。
		const Buffer::value_type p = Q.top();
		Q.pop();

		const typename PathTraits::const_vertices_iterator
			pi_begin(PathTraits::vertices_begin(*p)), // 現行頂点を指す反復子
			pi_second(next(pi_begin)), // 先行頂点を指す反復子
			pi_end(PathTraits::vertices_end(*p)); // 先行頂点列の終点を指す反復子

		const Vertex u(*pi_begin);

		vis.examine_vertex(u, g);

		// 現行頂点から流出する全ての辺を調べる。
		typename GTraits::out_edge_iterator ei, ei_end;
		for(tie(ei, ei_end) = out_edges(u, g); ei != ei_end; ++ei) {
			const Edge e(*ei); // 現行頂点から流出する辺の一つ
			const Vertex v(target(e, g)); // 流出先頂点
			const Weight w(get(weight, e)); // 辺の重み

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
				PathTraits::make_path(v, w, p, longest_regulation_overlap_length - 1)
			);
		}

		vis.finish_vertex(u, g);
	}
}

} // namespace vq_regulation

} // namespace graph

} // namespace rns

#endif // RNS_VQ_REGULATION_GRAPH_DIJKSTRA_SHORTEST_PATHS_H_
