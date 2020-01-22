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

#ifndef RNS_STRING_CONCATENATE_ITERATOR_H_
#define RNS_STRING_CONCATENATE_ITERATOR_H_

#include <boost/shared_ptr.hpp>

namespace rns {

/**
 * @brief ������A�������q
 *
 * std::ostream_iterator ���̃N���X�B<br>
 * ����_��
 * - ������^�̒l���󂯎���
 * - ������̎��͔̂����q���g�������A�����q�̕������ɃV�����[�R�s�[�����
 * - ��؂蕶���̕t����
 *   - std::ostream_iterator �͏o�͂��ƂɌ��ɋ�؂蕶����t��������i���s���̏o�͂ɍœK�j
 *   - ���̃N���X�͍ŏ��̏o�͎��͋�؂蕶���������A����ȍ~�͏o�͂̑O�ɋ�؂蕶����t��������i�J���}��؂�̏o�͂ɍœK�j
 *
 * @test StringConcatenateIteratorTest
 */
template <typename Char>
class basic_string_concatenate_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	typedef basic_string_concatenate_iterator self;
	typedef std::basic_ostringstream<Char> stream_type;
	typedef std::basic_string<Char> string_type;
public:
	class output_proxy
	{
		friend basic_string_concatenate_iterator;
	public:
		output_proxy(const Char* delim) :
		m_delim(delim),
		m_first(true)
		{
		}

		template <typename Value>
		output_proxy& operator=(const Value& rhs)
		{
			if(m_delim) {
				if(m_first) {
					m_first = false;
					m_oss << rhs;
				}
				else {
					m_oss << m_delim << rhs;
				}
			}
			else {
				m_oss << rhs;
			}

			return *this;
		}

	private:
		std::basic_ostringstream<Char> m_oss;
		const Char* m_delim;
		bool m_first;
	};

	/**
	 * @param[in] delim ��؂蕶����B�R�s�[�͂��Ȃ��̂Ŏ����ɒ��ӁB��؂�Ȃ��ꍇ��null�B
	 */
	explicit basic_string_concatenate_iterator(const Char* delim = 0) :
	m_proxy(new output_proxy(delim))
	{
	}

/// @name �����q�I�֐�
//@{
	output_proxy& operator*() { return *m_proxy; }
	self& operator++() { return *this; }
	self& operator++(int) { return *this; }
//@}

	/**
	 * @brief �A�����ꂽ�������Ԃ��B
	 */
	string_type str() const { return m_proxy->m_oss.str(); }

private:
	boost::shared_ptr<output_proxy> m_proxy;
};

typedef basic_string_concatenate_iterator<char> string_concatenate_iterator;
typedef basic_string_concatenate_iterator<wchar_t> wstring_concatenate_iterator;

#ifdef _TCHAR_DEFINED
typedef basic_string_concatenate_iterator<TCHAR> tstring_concatenate_iterator;
#endif // TCHAR_DEFINED

} // namespace rns

#endif // RNS_STRING_CONCATENATE_ITERATOR_H_
