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
 * @brief 経路型特性テンプレート
 */
template <typename Path>
struct path_traits {};

namespace detail {

/**
 * @brief 経路列反復子
 *
 * @par テンプレート引数:
 * - @b Path  - 経路型
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
 * @brief 経路頂点列反復子
 *
 * 経路列反復子をベースに、頂点だけを取り出すようにしたもの。
 *
 * @par テンプレート引数:
 * - @b Path  - 経路型
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
 * @brief 経路クラス・基底
 *
 * @par テンプレート引数:
 * - @b Derived - 派生型
 * - @b VertexDescriptor - グラフの頂点記述子型
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
	 * @brief 始点経路を作成する。
	 *
	 * @param v [in] 経路始点頂点。
	 */
	explicit path_base(vertex_descriptor v) :
	m_v(v),
	m_pp(),
	m_reg()
	{
	}

	/**
	 * @brief 新経路を作成する。
	 *
	 * @param v [in] 到達頂点。
	 * @param pp [in] 先行経路。
	 * @param reg [in] 頂点列規制に絡んでいる先行頂点の数（到達頂点を含まない）。
	 */
	path_base(vertex_descriptor v, const Derived* pp, unsigned int reg) :
	m_v(v),
	m_reg(reg),
	m_pp(pp)
	{
	}
	
	/**
	 * @brief 経路頂点列を遡る反復子の始点（到達頂点を指す）を得る。
	 * @post vertices_begin(p) != vertices_end(p)
	 */
	const_vertices_iterator vertices_begin() const
	{
		return const_vertices_iterator(static_cast<const Derived*>(this));
	}

	/**
	 * @brief 経路頂点列の遡る反復子の終点（経路始点頂点の次を指す）を得る。
	 * @post vertices_begin(p) != vertices_end(p)
	 */
	const_vertices_iterator vertices_end() const
	{
		return const_vertices_iterator();
	}

	vertex_descriptor m_v; ///< 到達頂点。
	const Derived* m_pp; ///< 先行経路。存在しない場合はnull。
	unsigned int m_reg; ///< 頂点列規制に絡んでいる先行頂点数。[0,std::distance(vertices_begin(), vertices_end()))
};

/**
 * @brief 経路型特性・基底
 */
template <typename Path>
struct path_traits_base
{
	typedef Path path_type;
	typedef typename path_type::vertex_descriptor vertex_descriptor;
	typedef typename path_type::const_vertices_iterator const_vertices_iterator;
	typedef std::list<path_type> default_container_type;
	
	/**
	 * @brief 経路頂点列を遡る反復子の始点（到達頂点を指す）を得る。
	 *
	 * @param p [in] 経路。
	 * @post vertices_begin(p) != vertices_end(p)
	 */
	static const_vertices_iterator vertices_begin(const path_type& p)
	{
		return p.vertices_begin();
	}

	/**
	 * @brief 経路頂点列の遡る反復子の終点（経路始点頂点の次を指す）を得る。
	 *
	 * @param p [in] 経路。
	 * @post vertices_begin(p) != vertices_end(p)
	 */
	static const_vertices_iterator vertices_end(const path_type& p)
	{
		return p.vertices_end();
	}

	/**
	 * @brief 流入を一意に識別するために必要な最短点を得る。
	 *
	 * @param p [in] 経路。
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
	 * @brief 両経路の流入は同じか。
	 *
	 * @param p1 [in] 経路1。
	 * @param p2 [in] 経路2。
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
 * @brief 経路コンテナ特性・基底
 */
template <typename Paths>
struct paths_traits_base
{
	typedef Paths paths_type;
	typedef typename paths_type::value_type path_type;

	typedef path_traits<path_type> path_traits;

	/**
	 * @brief 同じ流入の経路をコンテナ中から探す。
	 *
	 * @retval 0 同じ流入の経路はコンテナ中に存在しない。
	 * @retval !0 同じ流入の経路。
	 */
	static path_type* typical_find(paths_type& ps, const path_type& p)
	{
		using namespace boost;

		const paths_type::iterator
			it(std::find_if(ps.begin(), ps.end(), bind2nd(ptr_fun(&path_traits::typicality_equal), p)));

		return it != ps.end() ? &*it : 0;
	}

	/**
	 * @brief 経路を追加する。
	 *
	 * @param ps [in] 追加先経路コンテナ。
	 * @param p [in] コンテナにディープコピーされる経路。
	 * @return コンテナに追加された経路。
	 */
	static const path_type* push(paths_type& ps, const path_type& p)
	{
		return &*ps.insert(ps.end(), p);
	}
};

} // namespace detail

/**
 * @brief 経路クラス
 *
 * @par テンプレート引数:
 * - @b VertexDescriptor - グラフの頂点記述子型
 */
template <typename VertexDescriptor>
class path : public detail::path_base<path<VertexDescriptor>, VertexDescriptor>
{
public:
	/**
	 * @brief 始点経路を作成する。
	 *
	 * @param v [in] 経路始点頂点。
	 */
	explicit path(vertex_descriptor v) :
	super_type(v)
	{
	}

	/**
	 * @brief 新経路を作成する。
	 *
	 * @param v [in] 到達頂点。
	 * @param pp [in] 先行経路。
	 * @param reg [in] 頂点列規制に絡んでいる先行頂点の数（到達頂点を含まない）。
	 */
	path(vertex_descriptor v, const path* pp, unsigned int reg) :
	super_type(v, pp, reg)
	{
	}
};

/**
 * @brief 経路型特性
 */
template <typename VertexDescriptor>
struct path_traits<path<VertexDescriptor> > :
	public detail::path_traits_base<path<VertexDescriptor> >
{
	/**
	 * @brief 新経路を作成する。
	 *
	 * @param v [in] 到達頂点。
	 * @param pp [in] 先行経路。
	 * @param reg [in] 頂点列規制に絡んでいる先行頂点の数（到達頂点を含まない）。
	 */
	static path_type make_path(vertex_descriptor v, const path_type* pp, unsigned int reg)
	{
		return path_type(v, pp, reg);
	}
};

/**
 * @brief 経路コンテナ特性テンプレート
 *
 * @par テンプレート引数:
 * - @b Paths - 経路コンテナ型
 */
template <typename Paths>
class paths_traits
{
};

/**
 * @brief 経路コンテナ特性
 */
template <typename Path, typename Allocator>
struct paths_traits<std::list<Path, Allocator> > :
	public detail::paths_traits_base<std::list<Path, Allocator> >
{
	/**
	 * @brief 同じ流入の経路が存在しない場合に限り、経路コンテナとキューに経路を追加する。
	 *
	 * @param Q [in,out] 経路ポインタキュー。
	 * @param ps [in,out] 経路コンテナ。
	 * @param p [in] 追加予定の経路。
	 * @retval true 経路を追加した。
	 * @retval false 経路は追加しなかった。
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
 * @brief 経路コンテナ特性に従い、経路コンテナとキューに経路を追加する。
 *
 * @param Q [in,out] 経路ポインタキュー。
 * @param ps [in,out] 経路コンテナ。
 * @param p [in] 追加予定の経路。
 * @retval true 経路を追加した。
 * @retval false 経路は追加しなかった。
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
