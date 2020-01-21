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

#pragma once

#pragma warning(push)
#pragma warning(disable: 4819)
#include <boost/program_options.hpp>
#pragma warning(pop)

namespace uh
{
	/**
	 * @brief boost::program_optionsをTCHARに対応させるテンプレート関数
	 * @return typed_value
	 */
	template<class T>
	boost::program_options::typed_value<T, TCHAR>*
	tvalue()
	{
		return tvalue<T>(0);
	}

	/**
	 * @brief boost::program_optionsをTCHARに対応させるテンプレート関数
	 * @param	v [in]	コマンドライン引数を受け取る変数のポインタ
	 * @return typed_value
	 */
	template<class T>
	boost::program_options::typed_value<T, TCHAR>*
	tvalue(T* v)
	{
		return new boost::program_options::typed_value<T, TCHAR>(v);
	}
}
