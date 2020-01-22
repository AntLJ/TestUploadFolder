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

#ifndef RNS_URL_STRING_ALGORITHM_H_
#define RNS_URL_STRING_ALGORITHM_H_

#include <ctype.h>
#include <iterator>
#include <string>

namespace rns {
namespace algorithm {

/**
 * @brief URL��������f�R�[�h����B
 *
 * �o�͂���镶������ std::distance(first, last) �Ɠ������A����������Ȃ��B<br>
 * �s���ȃp�[�Z���g�G���R�[�f�B���O�� "?" �ɕϊ�����B
 *
 * @tparam InputIterator URL�����񔽕��q
 * @tparam OutputIterator �f�R�[�h�ςݕ�����o�͔����q
 * @param[in] first URL������擪�����q
 * @param[in] last URL������I�[�����q
 * @param[in] result �f�R�[�h�ςݕ�����o�͔����q
 * @return �f�R�[�h�ςݕ�����o�͔����q
 */
template <typename InputIterator, typename OutputIterator>
inline OutputIterator
decode_url_copy(InputIterator first, InputIterator last, OutputIterator result)
{
	typedef typename std::iterator_traits<InputIterator>::value_type char_type;

	for(; first != last; ++first)
	{
		char_type ch = *first;

		switch(ch)
		{
		case '%':
			ch = 0;
			for(int i = 0; i < 2; i++)
			{
				ch *= 16;

				if(++first == last)
				{
					*result = '?';
					return ++result;
				}

				const int x = ::tolower(std::char_traits<char_type>::to_int_type(*first));

				if(x >= '0' && x <= '9')
				{
					ch += x - '0';
					continue;
				}
				else if(x >= 'a' && x <= 'f')
				{
					ch += x - 'a' + 10;
					continue;
				}
				else
				{
					ch = '?';
					break;
				}
			}
			break;
		case '+':
			ch = ' ';
			break;
		}

		*result = ch;
		++result;
	}

	return result;
}

/**
 * @brief URL��������f�R�[�h����B
 *
 * ���̊֐��͈����𒼐ڏ���������B<br>
 * �f�R�[�h��̕����񒷂́A�f�R�[�h�O�Ɠ������A��������Z���Ȃ�B
 *
 * @tparam Sequence std::string �� std::vector<char>
 * @param[in,out] input �f�R�[�h���镶����
 *
 * @sa rns::algorithm::decode_url_copy()
 */
template <typename Sequence>
inline void decode_url(Sequence& input)
{
	input.erase(
		decode_url_copy(input.begin(), input.end(), input.begin()),
		input.end()
	);
}

} // namespace rns
} // namespace algorithm

#endif // RNS_URL_STRING_ALGORITHM_H_
