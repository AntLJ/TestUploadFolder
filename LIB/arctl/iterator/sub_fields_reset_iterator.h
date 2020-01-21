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

#ifndef ARCTL_SUB_FIELDS_RESET_ITERATOR_H_
#define ARCTL_SUB_FIELDS_RESET_ITERATOR_H_

#include <iterator>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/or.hpp>
#include <atl2/io.h>
#include <atl2/exception.h>

namespace arctl {

/**
 * @brief �T�u�t�B�[���h�����������q
 *
 * �n���ꂽ�v�f�ŃT�u�t�B�[���h�����������锽���q�B<br>
 * �ŏ��̗v�f�� put_SubFields ���A���̌�̗v�f�� AddField ����B<br>
 * �����q���A�������܂߂đS�ď��������_�ŏ������͏I������B
 *
 * �n����l�͈ȉ���2��B
 * - BSTR�̃X�}�[�g�|�C���^
 *   - CComBSTR
 *   - _bstr_t
 * - �o�̓X�g���[���ɏo�͉\�Ȓl
 *
 * �v�f��1���n����Ȃ������ꍇ�A �T�u�t�B�[���h�͕ω����Ȃ��B
 *
 * @note
 * BSTR�^�̒l��LPWSTR�^�̒l�Ƌ�ʂ��t���Ȃ��̂ŁA�O�̂��� CComBSTR ��ʂ��B
 * 
 * @note
 * rns::string_concatenate_iterator ���g�����Ǝv���Ă����̂����A�����Ă݂�Ƃ��܂胁���b�g���Ȃ��B
 */
template <typename InterfacePtr>
class sub_fields_reset_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	typedef sub_fields_reset_iterator self;
public:
	class output_proxy
	{
	public:
		explicit output_proxy(const InterfacePtr& filter) :
		m_filter(filter),
		m_first(true)
		{
		}

		template <typename T>
		output_proxy& operator=(const T& rhs)
		{
			if(m_first) {
				atl2::valid(m_filter->put_SubFields(to_bstr(rhs)));
				m_first = false;
			}
			else {
				atl2::valid(m_filter->AddField(to_bstr(rhs)));
			}

			return *this;
		}

	private:
		template <typename T>
		struct is_smart_bstr : 
			public boost::mpl::or_<
				boost::is_same<T, CComBSTR>,
				boost::is_same<T, _bstr_t>
			>
		{};

		/**
		 * @brief BSTR�̃X�}�[�g�|�C���^�Ȃ�l�����̂܂܎Q�Ƃ���B
		 *
		 * @param[in] BSTR�̃X�}�[�g�|�C���^�B
		 *
		 * @note
		 * �uBSTR�Ɍ^�ϊ��\�Ȓl�v�Ƃ����������l�������A���̏ꍇLPWSTR���܂܂��̂Œf�O�B
		 */
		template <typename T>
		static typename boost::enable_if<is_smart_bstr<T>, BSTR>::type to_bstr(const T& rhs)
		{
			return rhs;
		}

		template <typename T>
		static typename boost::disable_if<is_smart_bstr<T>, CComBSTR>::type to_bstr(const T& rhs)
		{
			return boost::lexical_cast<CComBSTR>(rhs);
		}

		InterfacePtr m_filter;
		bool m_first;
	};

	explicit sub_fields_reset_iterator(const InterfacePtr& filter) :
	m_proxy(new output_proxy(filter))
	{
	}

/// @name �����q�I�֐�
//@{
	output_proxy& operator*() { return *m_proxy; }
	self& operator++() { return *this; }
	self& operator++(int) { return *this; }
//@}

private:
	boost::shared_ptr<output_proxy> m_proxy;
};

template <typename InterfacePtr>
inline sub_fields_reset_iterator<InterfacePtr> sub_fields_resetter(const InterfacePtr& ipFilter)
{
	return sub_fields_reset_iterator<InterfacePtr>(ipFilter);
}

} // namespace arctl

#endif // ARCTL_SUB_FIELDS_RESET_ITERATOR_H_
