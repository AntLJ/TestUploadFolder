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
 * @brief �e�[�u�����̕����񂩂烆�[�U���̑O�u���̂ݎ��o������A�t�Ƀ��[�U���̑O�u���̂ݎ�菜������B
 *
 * - @ref algorithm_skip_owner_prefix
 * - @ref algorithm_copy_owner_prefix
 */
#ifndef ARCTL_TABLE_NAME_ALGORITHM_H_
#define ARCTL_TABLE_NAME_ALGORITHM_H_

#include <algorithm>
#include <boost/range.hpp>
#include <boost/concept_check.hpp>
#include <boost/tokenizer.hpp>
#include <../../TDC/useful_headers/text_util.h>

namespace arctl {

namespace algorithm {

namespace detail {

} // namespace detail

/**
 * @defgroup algorithm_skip_owner_prefix skip_owner_prefix
 */
//@{

/**
 * @brief ���[�U���̑O�u���i<em>user</em>.�j���������e�[�u�����̕���������߂�B
 *
 * @param[in] src ���[�U�[���̑O�u�����܂ނ�������Ȃ��e�[�u�����̕�����B
 * @return ���[�U���̑O�u�����������e�[�u�����̂̕�����B
 */
inline LPCTSTR skip_owner_prefix(const _bstr_t& src)
{
	std::wstring strret;
	std::wstring str(static_cast<LPCWSTR>(src));
	unsigned int pos = str.find( L".", 0 );
	if( pos != std::string::npos )
	{
		typedef boost::tokenizer<boost::char_separator<wchar_t>, std::wstring::const_iterator, std::wstring> token;
		boost::char_separator<wchar_t> sep(L".");
		token tok(str, sep);
		// �f�N�������g���ł��Ȃ����烋�[�v���čŌ�̂��Ԃ�悤�ɂ���
		for ( const auto& it : tok )
			strret = it;
	}
	else
		strret = str;
	static uh::tstring tStrret;
	tStrret = uh::toTStr(strret);
	return tStrret.c_str();
}

//@}

/**
 * @defgroup algorithm_copy_owner_prefix copy_owner_prefix
 */
//@{

/**
 * @brief ���[�U���̑O�u���i<em>user</em>.�j�𔲂��o���B
 *
 * @param[in] src ���[�U�[���̑O�u�����܂ނ�������Ȃ��e�[�u�����̕�����B
 * @return ���[�U���̑O�u��������B���������[�U���̕�������܂܂Ȃ��ꍇ�́uempty()�v�B
 *
 * @test
 * -# ���[�U���̑O�u�����܂� LPCTSTR ���n���ꂽ���ɐ��������삷��
 *   - <i>[ CopyOwnerPrefixTest::testConstCharArrayHasPrefix() ]</i>
 * -# ���[�U���̑O�u�����܂܂Ȃ� LPCTSTR ���n���ꂽ���ɐ��������삷��
 *   - <i>[ CopyOwnerPrefixTest::testConstCharArrayHasntPrefix() ]</i>
 */
template <typename Char>
inline std::basic_string<Char> copy_owner_prefix(const Char* src)
{
	std::basic_string<Char> srcStr(src, src+strlen(uh::toStr(src).c_str()));
	return	copy_owner_prefix(srcStr);
}

/**
 * @brief ���[�U���̑O�u���i<em>user</em>.�j�𔲂��o���B
 *
 * @param[in] src ���[�U�[���̑O�u�����܂ނ�������Ȃ��e�[�u�����̕�����B
 * @return ���[�U���̑O�u��������B���������[�U���̕�������܂܂Ȃ��ꍇ�́uempty()�v�B
 */
template <typename Char>
inline std::basic_string<Char> copy_owner_prefix(const std::basic_string<Char>& src)
{
	LPCTSTR skipOwnerStr = skip_owner_prefix(src.c_str());
	if(::_tcsicmp(src.c_str(),skipOwnerStr)==0)
		return std::basic_string<Char>();
	return std::basic_string<Char>(src.data(), src.find(skipOwnerStr));
}

/**
 * @brief ���[�U���̑O�u���i<em>user</em>.�j�𔲂��o���B
 *
 * @param[in] src ���[�U�[���̑O�u�����܂ނ�������Ȃ��e�[�u�����̕�����i_bstr_t�j�B
 * @return ���[�U���̑O�u��������i_bstr_t�j�B���������[�U���̕�������܂܂Ȃ��ꍇ�́ulength()==0�v�B
 *
 * @test
 * -# ���[�U���̑O�u�����܂ޕ����񂪓n���ꂽ���ɐ��������삷��
 *   - <i>[ CopyOwnerPrefixTest::testBStringHasPrefix() ]</i>
 * -# ���[�U���̑O�u�����܂܂Ȃ������񂪓n���ꂽ���ɐ��������삷��
 *   - <i>[ CopyOwnerPrefixTest::testBStringHasntPrefix() ]</i>
 */
inline _bstr_t copy_owner_prefix(const _bstr_t& src)
{
	std::wstring strret;
	std::wstring str(static_cast<LPCWSTR>(src));
	unsigned int pos = str.find( L".", 0 );
    if( pos != std::string::npos )
	{
		strret = str.substr(0, pos+1);
	}

	return strret.c_str();
}

//@}

} // namespace algorithm

using algorithm::skip_owner_prefix;
using algorithm::copy_owner_prefix;

} // namespace arctl

#endif // ARCTL_TABLE_NAME_ALGORITHM_H_
