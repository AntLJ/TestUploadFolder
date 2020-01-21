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

#ifndef ARCTL_OBJECT_MASSACRER_H_
#define ARCTL_OBJECT_MASSACRER_H_

#include <functional>
#include <boost/shared_ptr.hpp>
#include <atl2/exception.h>
#include <sqltl/inclause_portal.h>
#include <arctl/class.h>
#include <arctl/cursor.h>

namespace arctl {

/**
 * @brief テーブル内に存在する特定の属性値を持つオブジェクトをまとめて削除するクラス。
 *
 * オブジェクトの削除は以下のタイミングで行われる:
 * - 削除するオブジェクト数が1000に達したとき。
 * - flush() が呼ばれたとき。
 *
 * @attention flush() を呼ばないと、オブジェクトは削除されずに残る可能性がある。
 */
template <typename Class>
class object_massacrer
{
public:
	object_massacrer(Class* table, LPCTSTR field_name) :
	m_f(new delete_portal(massacre(table), field_name))
	{
		if(! table) throw std::invalid_argument("arctl::object_massacrer(): table is NULL.");
		if(! field_name) throw std::invalid_argument("arctl::object_massacrer(): field_name is NULL.");
	}

	~object_massacrer()
	{
	}

	template <typename T>
	void operator()(T const& t)
	{
		(*m_f)(t);
	}

	void flush()
	{
		m_f->flush();
	}

private:
// 定義
	/// 実際に削除を行うクラス。
	class massacre : public std::unary_function<LPCTSTR, void>
	{
	public:
		massacre(Class* table) :
		m_class(table),
		m_filter(CLSID_QueryFilter)
		{
			CComBSTR oid_field_name;
			m_class->get_OIDFieldName(&oid_field_name);
			m_filter->put_SubFields(oid_field_name);
		}

		void operator()(LPCTSTR lpszWhereClause)
		{
			using namespace atl2;

			m_filter->put_WhereClause(CComBSTR(lpszWhereClause));

			class_traits<Class>::cursor_type cursor;
			valid(m_class->Update(m_filter, VARIANT_FALSE, &cursor));

			delete_all(begin(cursor), end(cursor));
		}

	private:
		typename class_traits<Class>::class_type m_class;
		IQueryFilterPtr m_filter;
	};

	typedef sqltl::inclause_portal<massacre> delete_portal;

// 変数
	boost::shared_ptr<delete_portal> m_f;
};

} // namespace arctl

#endif // ARCTL_OBJECT_MASSACRER_H_
