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

#include <string>
#include <vector>
#include <locale>
#include <stdexcept>
#include <cassert>

namespace uh
{
	/**
	 * @brief マルチバイト文字列をワイド文字列に変換する
	 * @node C++標準ライブラリstd::codecvtを使っている
	 * @param str [in] マルチバイト文字列
	 * @param loc [in] ロケール
	 * @return ワイド文字列
	 */
	inline std::wstring Str2WStr(const std::string& str, const std::locale& loc)
	{
		typedef std::codecvt<wchar_t, char, std::mbstate_t> CodeCvt;
		const CodeCvt& converter = std::use_facet<CodeCvt>(loc);
		std::mbstate_t state = {0};

		const char* src     = str.c_str();
		const char* src_end = str.c_str() + str.length();
		int max = str.length();
		int wlen = converter.length(state, src, src_end, max);
		// nullターミネート用に1文字多くバッファを確保する
		std::vector<wchar_t> wchar_buf(wlen + 1);
		wchar_t* dst       = &wchar_buf.at(0);
		wchar_t* dst_limit = &wchar_buf.at(wlen);
		const char* src_next = 0;
		wchar_t* dst_next = 0;
		std::codecvt_base::result result = converter.in(
			state,		// 状態保持用の変数
			src,		// 変換元文字列の先頭アドレス
			src_end,	// 変換元文字列の最後の次のアドレス
			src_next,	// 最後に変換に成功した変換元文字列の要素の次を指すポインタ
			dst,		// 変換先文字列の先頭アドレス
			dst_limit,	// 変換先文字列の最後の次のアドレス
			dst_next);	// 最後に変換に成功した変換先文字列の要素の次を指すポインタ
		if(result != std::codecvt_base::ok)
		{
			throw std::runtime_error("Fail Str2WStr()");
		}
		// 全ての文字を変換できたときは src_end == src_next になる
		assert(src_end == src_next);
		// nullターミネートしてくれないので、ここで行う
		*dst_next = L'\0';
		return std::wstring(dst);
	}

	/**
	 * @brief ワイド文字列をマルチバイト文字列に変換する
	 * @node C++標準ライブラリstd::codecvtを使っている
	 * @param wstr [in] ワイド文字列
	 * @param loc [in] ロケール
	 * @return マルチバイト文字列
	 */
	inline std::string WStr2Str(const std::wstring& wstr, const std::locale& loc)
	{
		typedef std::codecvt<wchar_t, char, std::mbstate_t> CodeCvt;
		const CodeCvt& converter = std::use_facet<CodeCvt>(loc);
		std::mbstate_t state = {0};

		const wchar_t* src = wstr.c_str();
		const wchar_t* src_end  = wstr.c_str() + wstr.length();
		static const int DST_BUF_SIZE = 8;
		// ＃VC11にはDST_BUF_SIZEが奇数のとき、
		// ＃(dst + DST_BUF_SIZE)を超えたアドレスにアクセスしてしまう問題があることが確認できている。
		// ＃同様な現象が起こると困るので、バッファを多めに確保する。
		std::vector<char> dst_buf(DST_BUF_SIZE + 5, '\0');
		char* dst = &dst_buf.at(0);
		std::string ret_str;
		while(src != src_end)
		{
			const wchar_t* src_next = 0;
			char* dst_next = 0;
			std::codecvt_base::result result = converter.out(
				state,
				src, src_end, src_next,
				dst, dst + DST_BUF_SIZE, dst_next);
//			assert(dst_next <= dst + DST_BUF_SIZE);	// 上述の問題の確認用
			switch(result)
			{
			case std::codecvt_base::partial:
			case std::codecvt_base::ok:
				if (dst < dst_next)
				{
					// ＃文字数を指定してappendするので、
					// ＃nullターミネートのことは気にする必要がない
					ret_str.append(dst, (size_t)(dst_next - dst));
				}
				else
				{
					throw std::runtime_error("Fail WStr2Str()");
				}
				break;
			default:
				throw std::runtime_error("Fail WStr2Str()");
			}
			src = src_next;
		}
		return ret_str;
	}
}
