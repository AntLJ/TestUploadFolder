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
 * @brief breadth_first_visit() �p�r�W�^
 */
struct bfs_visitor
{
	template <class Vertex, class Graph>
	void examine_vertex(Vertex, Graph&) {}

	template <class Vertex, class Graph>
	void finish_vertex(Vertex, Graph&) {}
};

/**
 * @brief ���_��K�����l���������D��o�H�T���A���S���Y���B
 *
 * �ʏ�̕��D��o�H�T���Ƃ͈قȂ�A�������_�����قȂ�΁A�������_����ȏ�ʉ߂��邱�Ƃ��ł���B
 *
 * @param g [in] �L���O���t�܂��͖����O���t�B�O���t�̌^�� Incidence Graph �̃��f���łȂ���΂Ȃ�Ȃ��B 
 * @param Q [in] ���_����������鏇�������肷�邽�߂Ɏg�p�����o�H�|�C���^�L���[�B
 * @param vqregs [in] �֘A���_��K���}�b�v�i���_�����j�B
 * @param paths [in] �o�H�R���e�i�}�b�v�i���_�����j�B�R���e�i�� std::list �Ɍ���B
 * @param vis [in] �r�W�^�B
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
		// �L���[���猻�s���_�������o���B
		const Buffer::value_type p = Q.front();
		Q.pop();

		const typename PathTraits::const_vertices_iterator
			pi_begin(PathTraits::vertices_begin(*p)), // ���s���_���w�������q
			pi_second(next(pi_begin)), // ��s���_���w�������q
			pi_end(PathTraits::vertices_end(*p)); // ��s���_��̏I�_���w�������q

		const Vertex u(*pi_begin);

		vis.examine_vertex(u, g);

		// ���s���_���痬�o����S�Ă̕ӂ𒲂ׂ�B
		typename GTraits::out_edge_iterator ei, ei_end;
		for(boost::tie(ei, ei_end) = out_edges(u, g); ei != ei_end; ++ei) {
			const Vertex v(target(*ei, g));

			// �t�^�[������悤�Ȍo�H�͋p���B
			if(pi_second != pi_end && v == *pi_second)
				continue;

			// ���_��K�����m�F����B
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
