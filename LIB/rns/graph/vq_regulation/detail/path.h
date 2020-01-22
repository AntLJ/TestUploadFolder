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

#ifndef RNS_VQ_REGULATION_GRAPH_PATH_H_
#define RNS_VQ_REGULATION_GRAPH_PATH_H_

#include <list>
#include <algorithm>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/functional.hpp>

namespace rns {

namespace graph {

namespace vq_regulation {

/**
 * @brief �o�H�^�����e���v���[�g
 */
template <typename Path>
struct path_traits {};

namespace detail {

/**
 * @brief �o�H�񔽕��q
 *
 * @par �e���v���[�g����:
 * - @b Path  - �o�H�^
 */
template <typename Path>
struct path_const_iterator :
	public boost::iterator_adaptor<
		path_const_iterator<Path>,
		const Path*,
		const Path,
		boost::forward_traversal_tag
	>
{
	friend class boost::iterator_core_access;
public:
	explicit path_const_iterator(const Path* first = 0) :
	iterator_adaptor_(first)
	{
	}

private:
	typename iterator_adaptor_::reference dereference() const
	{
		return *base();
	}

	void increment()
	{
		base_reference() = base()->m_pp;
	}
};

/**
 * @brief �o�H���_�񔽕��q
 *
 * �o�H�񔽕��q���x�[�X�ɁA���_���������o���悤�ɂ������́B
 *
 * @par �e���v���[�g����:
 * - @b Path  - �o�H�^
 */
template <typename Path>
struct path_const_vertices_iterator :
	public boost::iterator_adaptor<
		path_const_vertices_iterator<Path>,
		path_const_iterator<Path>,
		const typename path_traits<Path>::vertex_descriptor
	>
{
	friend class boost::iterator_core_access;
public:
	explicit path_const_vertices_iterator(const Path* first = 0) :
	iterator_adaptor_(base_type(first))
	{
	}

private:
	typename iterator_adaptor_::reference dereference() const
	{
		return base()->m_v;
	}
};

/**
 * @brief �o�H�N���X�E���
 *
 * @par �e���v���[�g����:
 * - @b Derived - �h���^
 * - @b VertexDescriptor - �O���t�̒��_�L�q�q�^
 */
template <typename Derived, typename VertexDescriptor>
class path_base
{
protected:
	typedef path_base<Derived, VertexDescriptor> super_type;
public:
	typedef VertexDescriptor vertex_descriptor;
	typedef detail::path_const_vertices_iterator<Derived> const_vertices_iterator;

	/**
	 * @brief �n�_�o�H���쐬����B
	 *
	 * @param v [in] �o�H�n�_���_�B
	 */
	explicit path_base(vertex_descriptor v) :
	m_v(v),
	m_pp(),
	m_reg()
	{
	}

	/**
	 * @brief �V�o�H���쐬����B
	 *
	 * @param v [in] ���B���_�B
	 * @param pp [in] ��s�o�H�B
	 * @param reg [in] ���_��K���ɗ���ł����s���_�̐��i���B���_���܂܂Ȃ��j�B
	 */
	path_base(vertex_descriptor v, const Derived* pp, unsigned int reg) :
	m_v(v),
	m_reg(reg),
	m_pp(pp)
	{
	}
	
	/**
	 * @brief �o�H���_���k�锽���q�̎n�_�i���B���_���w���j�𓾂�B
	 * @post vertices_begin(p) != vertices_end(p)
	 */
	const_vertices_iterator vertices_begin() const
	{
		return const_vertices_iterator(static_cast<const Derived*>(this));
	}

	/**
	 * @brief �o�H���_��̑k�锽���q�̏I�_�i�o�H�n�_���_�̎����w���j�𓾂�B
	 * @post vertices_begin(p) != vertices_end(p)
	 */
	const_vertices_iterator vertices_end() const
	{
		return const_vertices_iterator();
	}

	vertex_descriptor m_v; ///< ���B���_�B
	const Derived* m_pp; ///< ��s�o�H�B���݂��Ȃ��ꍇ��null�B
	unsigned int m_reg; ///< ���_��K���ɗ���ł����s���_���B[0,std::distance(vertices_begin(), vertices_end()))
};

/**
 * @brief �o�H�^�����E���
 */
template <typename Path>
struct path_traits_base
{
	typedef Path path_type;
	typedef typename path_type::vertex_descriptor vertex_descriptor;
	typedef typename path_type::const_vertices_iterator const_vertices_iterator;
	typedef std::list<path_type> default_container_type;
	
	/**
	 * @brief �o�H���_���k�锽���q�̎n�_�i���B���_���w���j�𓾂�B
	 *
	 * @param p [in] �o�H�B
	 * @post vertices_begin(p) != vertices_end(p)
	 */
	static const_vertices_iterator vertices_begin(const path_type& p)
	{
		return p.vertices_begin();
	}

	/**
	 * @brief �o�H���_��̑k�锽���q�̏I�_�i�o�H�n�_���_�̎����w���j�𓾂�B
	 *
	 * @param p [in] �o�H�B
	 * @post vertices_begin(p) != vertices_end(p)
	 */
	static const_vertices_iterator vertices_end(const path_type& p)
	{
		return p.vertices_end();
	}

	/**
	 * @brief ��������ӂɎ��ʂ��邽�߂ɕK�v�ȍŒZ�_�𓾂�B
	 *
	 * @param p [in] �o�H�B
	 * @post std::distance(typical_vertices_end(p), vertices_end(p)) >= 0
	 * @post vertices_begin(p) != typical_vertices_end(p)
	 */
	static const_vertices_iterator typical_vertices_end(const path_type& p)
	{
		const_vertices_iterator it(vertices_begin(p)), it_end(vertices_end(p));
		for(unsigned int n = p.m_reg + 2; n > 0 && it != it_end; n--) {
			++it;
		}
		return it;
	}

	/**
	 * @brief ���o�H�̗����͓������B
	 *
	 * @param p1 [in] �o�H1�B
	 * @param p2 [in] �o�H2�B
	 */
	static bool typicality_equal(const path_type& p1, const path_type& p2)
	{
		return
			typicality_equal_(
				vertices_begin(p1), typical_vertices_end(p1),
				vertices_begin(p2), typical_vertices_end(p2)
			);
	}

private:
	template <typename InputIterator>
	static bool typicality_equal_(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2)
	{
		for(; first1 != last1 && first2 != last2; ++first1, ++first2) {
			if(*first1 != *first2)
				return false;
		}

		return first1 == last1 && first2 == last2;
	}
};

/**
 * @brief �o�H�R���e�i�����E���
 */
template <typename Paths>
struct paths_traits_base
{
	typedef Paths paths_type;
	typedef typename paths_type::value_type path_type;

	typedef path_traits<path_type> path_traits;

	/**
	 * @brief ���������̌o�H���R���e�i������T���B
	 *
	 * @retval 0 ���������̌o�H�̓R���e�i���ɑ��݂��Ȃ��B
	 * @retval !0 ���������̌o�H�B
	 */
	static path_type* typical_find(paths_type& ps, const path_type& p)
	{
		using namespace boost;

		const paths_type::iterator
			it(std::find_if(ps.begin(), ps.end(), bind2nd(ptr_fun(&path_traits::typicality_equal), p)));

		return it != ps.end() ? &*it : 0;
	}

	/**
	 * @brief �o�H��ǉ�����B
	 *
	 * @param ps [in] �ǉ���o�H�R���e�i�B
	 * @param p [in] �R���e�i�Ƀf�B�[�v�R�s�[�����o�H�B
	 * @return �R���e�i�ɒǉ����ꂽ�o�H�B
	 */
	static const path_type* push(paths_type& ps, const path_type& p)
	{
		return &*ps.insert(ps.end(), p);
	}
};

} // namespace detail

/**
 * @brief �o�H�N���X
 *
 * @par �e���v���[�g����:
 * - @b VertexDescriptor - �O���t�̒��_�L�q�q�^
 */
template <typename VertexDescriptor>
class path : public detail::path_base<path<VertexDescriptor>, VertexDescriptor>
{
public:
	/**
	 * @brief �n�_�o�H���쐬����B
	 *
	 * @param v [in] �o�H�n�_���_�B
	 */
	explicit path(vertex_descriptor v) :
	super_type(v)
	{
	}

	/**
	 * @brief �V�o�H���쐬����B
	 *
	 * @param v [in] ���B���_�B
	 * @param pp [in] ��s�o�H�B
	 * @param reg [in] ���_��K���ɗ���ł����s���_�̐��i���B���_���܂܂Ȃ��j�B
	 */
	path(vertex_descriptor v, const path* pp, unsigned int reg) :
	super_type(v, pp, reg)
	{
	}
};

/**
 * @brief �o�H�^����
 */
template <typename VertexDescriptor>
struct path_traits<path<VertexDescriptor> > :
	public detail::path_traits_base<path<VertexDescriptor> >
{
	/**
	 * @brief �V�o�H���쐬����B
	 *
	 * @param v [in] ���B���_�B
	 * @param pp [in] ��s�o�H�B
	 * @param reg [in] ���_��K���ɗ���ł����s���_�̐��i���B���_���܂܂Ȃ��j�B
	 */
	static path_type make_path(vertex_descriptor v, const path_type* pp, unsigned int reg)
	{
		return path_type(v, pp, reg);
	}
};

/**
 * @brief �o�H�R���e�i�����e���v���[�g
 *
 * @par �e���v���[�g����:
 * - @b Paths - �o�H�R���e�i�^
 */
template <typename Paths>
class paths_traits
{
};

/**
 * @brief �o�H�R���e�i����
 */
template <typename Path, typename Allocator>
struct paths_traits<std::list<Path, Allocator> > :
	public detail::paths_traits_base<std::list<Path, Allocator> >
{
	/**
	 * @brief ���������̌o�H�����݂��Ȃ��ꍇ�Ɍ���A�o�H�R���e�i�ƃL���[�Ɍo�H��ǉ�����B
	 *
	 * @param Q [in,out] �o�H�|�C���^�L���[�B
	 * @param ps [in,out] �o�H�R���e�i�B
	 * @param p [in] �ǉ��\��̌o�H�B
	 * @retval true �o�H��ǉ������B
	 * @retval false �o�H�͒ǉ����Ȃ������B
	 */
	template <typename Queue>
	static bool push_path(Queue& Q, paths_type& ps, const path_type& p)
	{
		if(! typical_find(ps, p)) {
			Q.push(push(ps, p));
			return true;
		}

		return false;
	}
};

/**
 * @brief �o�H�R���e�i�����ɏ]���A�o�H�R���e�i�ƃL���[�Ɍo�H��ǉ�����B
 *
 * @param Q [in,out] �o�H�|�C���^�L���[�B
 * @param ps [in,out] �o�H�R���e�i�B
 * @param p [in] �ǉ��\��̌o�H�B
 * @retval true �o�H��ǉ������B
 * @retval false �o�H�͒ǉ����Ȃ������B
 */
template <typename Queue, typename Paths>
bool push_path(Queue& Q, Paths& ps, const typename paths_traits<Paths>::path_type& p)
{
	return paths_traits<Paths>::push_path(Q, ps, p);
}

} // namespace vq_regulation

} // namespace graph

} // namespace rns

#endif // RNS_VQ_REGULATION_GRAPH_PATH_H_
