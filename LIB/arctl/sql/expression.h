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

/**
 * @file
 * @brief SQL�]�������C�u����
 *
 * -# SQL�]������ parse() �ŉ�͂���
 * -# parse() �ɂ���ē���ꂽ��@�� IRowBuffer �� evaluate() �ɓn���ĕ]������
 */
#ifndef ARCTL_SQL_EXPRESSION_H_
#define ARCTL_SQL_EXPRESSION_H_

#include <boost/version.hpp>
#if BOOST_VERSION <= 103400
#include <boost/spirit/core.hpp>
#else
#include <boost/spirit/include/classic_core.hpp>
#endif // BOOST_VERSION <= 103400
#include <sqltl/expression/grammar.h>
#include <sqltl/expression/com/lazy_evaluator.h>
#include <atl2/exception.h>
#include <CodeAnalysis/SourceAnnotations.h>

namespace arctl {

namespace sql {

namespace expression {

namespace detail {

typedef sqltl::expression::com_lazy_evaluator<IRowBuffer*> lazy_evaluator_base;

/**
 * @brief �]�����Ƀt�B�[���h���̂���C���f�b�N�X������o���đ����l�����߂�A�����t�B�[���h�l�]���N���X
 */
struct named_column_factor_evaluator : public lazy_evaluator_base::evaluator_type
{
public:
	/**
	 * @param[in] first �����t�B�[���h��������n�_�B
	 * @param[in] last �����t�B�[���h��������I�_�B
	 */
	template <typename InputIterator>
	named_column_factor_evaluator(
		InputIterator first,
		InputIterator last
	) :
	m_bstrColumn(std::basic_string<std::iterator_traits<TCHAR*>::value_type>(first, last).c_str())
	{
	}

	virtual _variant_t eval(argument_type& ipRowBuff) const
	{
		IFieldsPtr ipFields;
		atl2::valid(ipRowBuff->get_Fields(&ipFields));

		long nIndex;
		atl2::valid(ipFields->FindField(m_bstrColumn, &nIndex));
		
		_variant_t aVal;
		atl2::valid(ipRowBuff->get_Value(nIndex, &aVal));

		return aVal;
	}

private:
	const CComBSTR m_bstrColumn; ///< �����t�B�[���h���́B
};

/**
 * @brief �\�ߋ��߂Ă����������t�B�[���h�C���f�b�N�X���g���đ����l�����߂�A�����t�B�[���h�l�]���N���X
 */
struct indexed_factor_evaluator : public lazy_evaluator_base::evaluator_type
{
public:
	/**
	 * @param[in] nIndex �����t�B�[���h�C���f�b�N�X�B
	 */
	indexed_factor_evaluator(
		long nIndex
	) :
	m_nIndex(nIndex)
	{
	}

	virtual _variant_t eval(argument_type& ipRowBuff) const
	{
		_variant_t aVal;
		atl2::valid(ipRowBuff->get_Value(m_nIndex, &aVal));
		return aVal;
	}

private:
	const long m_nIndex; ///< �����t�B�[���h�C���f�b�N�X�B
};

} // namespace detail

/**
 * @brief IRowBuffer �����t�B�[���h�l��x���]������I�u�W�F�N�g�𐶐�����N���X
 *
 * ���ۂɕ]�������u�ԂɂȂ��ď��߂đ����t�B�[���h�l�� IRowBuffer ������o���]���I�u�W�F�N�g�𐶐�����B
 */
class row_lazy_evaluator : public detail::lazy_evaluator_base
{
public:
	/**
	 * @param[in] ipFields �����t�B�[���h�ꗗ�B�]��������͂���i�K�ő����t�B�[���h�C���f�b�N�X�����߂Ă����̂������ȏꍇ��null�ɂ���B
	 */
	explicit row_lazy_evaluator(
		IFields* ipFields = 0
	) :
	m_ipFields(ipFields)
	{
	}

	template <typename InputIterator>
	term_type column_factor(InputIterator first, InputIterator last) const
	{
		typedef typename std::iterator_traits<InputIterator>::value_type char_type;

		if(m_ipFields != 0) {
			long nField;
			m_ipFields->FindField(CComBSTR(std::basic_string<char_type>(first, last).c_str()), &nField);
			return term_type(new detail::indexed_factor_evaluator(nField));
		}
		else
			return term_type(new detail::named_column_factor_evaluator(first, last));
	}

private:
	IFieldsPtr m_ipFields; ///< �����t�B�[���h�ꗗ�B
};

/**
 * @brief ��@
 */
typedef detail::lazy_evaluator_base::term_type solution;

/**
 * @brief arctl::sql::expression::parse() �̖߂�l
 *
 * boost::spirit::parse_info �݊��ϐ��� arctl::sql::expression::evaluate() �p�ϐ������B
 */
template <typename IteratorT>
struct parse_info
{
	/**
	 * @param rOrginal [in] SQL���̉�͏��I���W�i���B
	 * @param rActor [in] SQL���̉�͂Ɏg�p������͎��A�N�V�����B
	 */
	template <typename Evaluator>
	parse_info(
#if BOOST_VERSION <= 103400
		const boost::spirit::parse_info<IteratorT>& rOrginal,
#else
		const boost::spirit::classic::parse_info<IteratorT>& rOrginal,
#endif
		const sqltl::expression::evaluation_actor<Evaluator>& rActor
	) :
	stop(rOrginal.stop),
	hit(rOrginal.hit),
	full(rOrginal.full),
	length(rOrginal.length)
	{
		if(full) {
			solution = rActor.answer();
		}
	}
/**
 * @name boost::spirit::parse_info �݊��ϐ�
 */
//@{
	IteratorT stop;
	bool hit;
	bool full;
	std::size_t length;
//@}

/**
 * @name arctl::sql::expression::evaluate() �p�ϐ�
 */
//@{
	solution solution;
//@}
};

/**
 * @brief SQL������͂���B
 *
 * @param[in] lpszExpression �]�����B
 * @param[in] ipFields �C���f�b�N�X��\�ߋ��߂Ă��������ꍇ�Ɏw�肷�� IFields �B
 * @return ��͌��ʁB
 *
 * ipFields ���w�肵������ evaluate() �̃R�X�g�͒Ⴍ�čςށB
 */
template <typename CharT>
inline parse_info<CharT const*>
parse([SA_Pre(Null=SA_No), SA_Pre(NullTerminated=SA_Yes)] CharT const* lpszExpression, IFields* ipFields = 0)
{
	namespace se = sqltl::expression;

	se::evaluation_actor<row_lazy_evaluator> aActor((row_lazy_evaluator(ipFields)));
	return parse_info<CharT const*>(se::parse(lpszExpression, aActor), aActor);
}

/**
 * @brief RowBuffer �I�u�W�F�N�g��]������B
 *
 * @param[in] ipRowBuff �]���ΏہB
 * @param[in] rSolution �]������ parse() ���ĕ҂ݏo���ꂽ��@�B
 * @throw std::runtime_error ��@���҂ݏo����Ă��Ȃ��B
 */
inline bool evaluate([SA_Pre(Null=SA_No)] IRowBuffer* ipRowBuff, const solution& rSolution)
{
	if(! rSolution)
		throw std::runtime_error("illegal parse_info.");

	return rSolution->eval(ipRowBuff);
}

} // namespace expression

} // namespace sql

} // namespace arctl

#endif // ARCTL_SQL_EXPRESSION_H_
