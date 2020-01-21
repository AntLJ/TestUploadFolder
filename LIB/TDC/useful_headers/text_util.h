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

#include <list>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp> // is_any_of
#include <boost/algorithm/string/split.hpp>
#include "directory_util.h"

namespace uh{

namespace text_util{
	/**
	 * @brief テキスト(ファイル)指定文字列を解析してリストに格納する
	 * @param[in]	str	テキスト(ファイル)指定文字列 ex. hoge,fuga,... または C:\hoge.txt
	 * @retrn リスト(tstring)
	 */
	inline std::list<tstring> ToTStrList( const tstring& str )
	{
		std::list<tstring> lists;
		if( isFile(str.c_str()) )
		{
			// ファイルから1行ずつ読み込んで格納
			std::ifstream ifs(str);
			if( !(ifs && ifs.good()) )
				return lists;

			std::string buf;
			while(std::getline(ifs, buf))
			{
				lists.push_back( toTStr(buf) );
			}
		}
		else
		{
			// カンマ区切りをばらして格納
			std::vector<tstring> result;
			boost::algorithm::split(result, str, boost::is_any_of(","));
			for( auto& str : result )
			{
				lists.push_back( toTStr(str) );
			}
		}
		return lists;
	}
	
	/**
	 * @brief テキスト(ファイル)指定文字列を解析してリストに格納する
	 * @note long型にキャストする
	 * @param[in]	str	テキスト(ファイル)指定文字列 ex. hoge,fuga,... または C:\hoge.txt
	 * @retrn リスト(long)
	 */
	inline std::list<long> ToLongList( const tstring& str )
	{
		std::list<long> lists;
		std::list<tstring> strlists = ToTStrList( str );
		for( auto& str : strlists )
		{
			try{
				lists.push_back( boost::lexical_cast<long>( str ) );
			}catch( boost::bad_lexical_cast ){
				// キャストできなきゃ無視
			}
		}
		return lists;
	}

	/**
	 * @brief テキスト(ファイル)指定文字列を解析してリストに格納する
	 * @note unsigned long型にキャストする
	 * @param[in]	str	テキスト(ファイル)指定文字列 ex. hoge,fuga,... または C:\hoge.txt
	 * @retrn リスト(unsigned long)
	 */
	inline std::list<unsigned long> ToUnsignedLongList( const tstring& str )
	{
		std::list<unsigned long> lists;
		std::list<tstring> strlists = ToTStrList( str );
		for( auto& str : strlists )
		{
			try{
				lists.emplace_back( boost::lexical_cast<unsigned long>( str ) );
			}catch( boost::bad_lexical_cast ){
				// キャストできなきゃ無視
			}
		}
		return lists;
	}
} // text_util

} // uh
