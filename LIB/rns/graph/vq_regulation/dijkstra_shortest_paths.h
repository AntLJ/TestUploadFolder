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
 * @brief �_�C�N�X�g���ŒZ�o�H�T���A���S���Y���p�o�H�N���X
 *
 * @par �e���v���[�g����:
 * - @b VertexDescriptor - �O���t�̒��_�L�q�q�^
 * - @b Distance - �R�X�g�̌^
 */
template <typename VertexDescriptor, typename Distance>
class dijkstra_path : public detail::path_base<dijkstra_path<VertexDescriptor, Distance>, VertexDescriptor>
{
public:
	typedef Distance distance_type;

	/**
	 * @brief �n�_�o�H���쐬����B
	 *
	 * @param v [in] �o�H�n�_���_�B
	 * @param d [in] �J�n�R�X�g�B
	 */
	explicit dijkstra_path(vertex_descriptor v, distance_type d = distance_type()) :
	super_type(v),
	m_dist(d)
	{
	}

	/**
	 * @brief ��s���_�����쐬����B
	 *
	 * @param v [in] ���B���_�B
	 * @param d [in] ���R�X�g�B
	 * @param pp [in] ��s�o�H�B
	 * @param reg [in] ���_��K���ɗ���ł����s���_�̐��i���B���_���܂܂Ȃ��j�B
	 */
	dijkstra_path(vertex_descriptor v, distance_type d, const dijkstra_path* pp, unsigned int reg) :
	super_type(v, pp, reg),
	m_dist(d)
	{
	}

	distance_type m_dist; ///< ���R�X�g�B
};

/**
 * @brief �_�C�N�X�g���ŒZ�o�H�T���A���S���Y���p�D�揇�ʕt���L���[
 *
 * �t�B�{�i�b�`�q�[�v���g���������ǂ��̂��낤���A�ʓ|�Ȃ̂œ񕪖؁i multimap �j�Ŏ����B
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
 * @brief �_�C�N�X�g���ŒZ�o�H�T���A���S���Y���p�o�H�^����
 */
template <typename VertexDescriptor, typename Distance>
struct path_traits<dijkstra_path<VertexDescriptor, Distance> > :
	public detail::path_traits_base<dijkstra_path<VertexDescriptor, Distance> >
{
	typedef typename path_type::distance_type distance_type;

	/**
	 * @brief �V�o�H���쐬����B
	 *
	 * @param v [in] ���B���_�B
	 * @param weight [in] �d�݁B
	 * @param pp [in] ��s�o�H�B
	 * @param reg [in] ���_��K���ɗ���ł����s���_�̐��i���B���_���܂܂Ȃ��j�B
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
 * @brief �_�C�N�X�g���ŒZ�o�H�T���A���S���Y���p�o�H�R���e�i����
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
 * @brief dijkstra_shortest_paths() �p�r�W�^
 */
struct dijkstra_visitor
{
	template <class Vertex, class Graph>
	void examine_vertex(Vertex, Graph&) {}

	template <class Vertex, class Graph>
	void finish_vertex(Vertex, Graph&) {}
};

/**
 * @brief ���_��K�����l�������_�C�N�X�g���ŒZ�o�H�T���A���S���Y���B
 *
 * �ʏ�̃_�C�N�X�g���ŒZ�o�H�T���Ƃ͈قȂ�A�������_�����قȂ�΁A�������_����ȏ�ʉ߂��邱�Ƃ��ł���B
 *
 * @param g [in] �L���O���t�܂��͖����O���t�B�O���t�̌^�� Incidence Graph �̃��f���łȂ���΂Ȃ�Ȃ��B 
 * @param Q [in] ���_����������鏇�������肷�邽�߂Ɏg�p����� dijkstra_queue �B
 * @param vqregs [in] �֘A���_��K���}�b�v�i���_�����j�B
 * @param paths [in] �o�H�R���e�i�}�b�v�i���_�����j�B�R���e�i�� std::list �Ɍ���B
 * @param weight [in] �R�X�g�}�b�v�i�ӑ����j�B
 * @param vis [in] �r�W�^�B
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

	typedef graph_traits<Graph> GTraits; // �O���t����
	typedef typename GTraits::vertex_descriptor Vertex; // ���_�L�q�q�^
	typedef typename GTraits::edge_descriptor Edge; // �ӋL�q�q�^

	typedef typename property_traits<VQRegulationsMap>::value_type VQRegulations; // ���_��K���^
	typedef typename property_traits<PathsMap>::value_type Paths; // �o�H�R���e�i�^
	typedef paths_traits<Paths> PathsTraits; // �o�H�R���e�i����

	typedef typename PathsTraits::path_type Path; // �o�H�^
	typedef typename PathsTraits::path_traits PathTraits; // �o�H�^����

	typedef typename property_traits<WeightMap>::value_type Weight; // �d�݌^

	while(! Q.empty()) {
		// �L���[���猻�s���_�������o���B
		const Buffer::value_type p = Q.top();
		Q.pop();

		const typename PathTraits::const_vertices_iterator
			pi_begin(PathTraits::vertices_begin(*p)), // ���s���_���w�������q
			pi_second(next(pi_begin)), // ��s���_���w�������q
			pi_end(PathTraits::vertices_end(*p)); // ��s���_��̏I�_���w�������q

		const Vertex u(*pi_begin);

		vis.examine_vertex(u, g);

		// ���s���_���痬�o����S�Ă̕ӂ𒲂ׂ�B
		typename GTraits::out_edge_iterator ei, ei_end;
		for(tie(ei, ei_end) = out_edges(u, g); ei != ei_end; ++ei) {
			const Edge e(*ei); // ���s���_���痬�o����ӂ̈��
			const Vertex v(target(e, g)); // ���o�撸�_
			const Weight w(get(weight, e)); // �ӂ̏d��

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
