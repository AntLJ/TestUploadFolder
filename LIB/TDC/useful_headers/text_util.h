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
	 * @brief �e�L�X�g(�t�@�C��)�w�蕶�������͂��ă��X�g�Ɋi�[����
	 * @param[in]	str	�e�L�X�g(�t�@�C��)�w�蕶���� ex. hoge,fuga,... �܂��� C:\hoge.txt
	 * @retrn ���X�g(tstring)
	 */
	inline std::list<tstring> ToTStrList( const tstring& str )
	{
		std::list<tstring> lists;
		if( isFile(str.c_str()) )
		{
			// �t�@�C������1�s���ǂݍ���Ŋi�[
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
			// �J���}��؂���΂炵�Ċi�[
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
	 * @brief �e�L�X�g(�t�@�C��)�w�蕶�������͂��ă��X�g�Ɋi�[����
	 * @note long�^�ɃL���X�g����
	 * @param[in]	str	�e�L�X�g(�t�@�C��)�w�蕶���� ex. hoge,fuga,... �܂��� C:\hoge.txt
	 * @retrn ���X�g(long)
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
				// �L���X�g�ł��Ȃ��ᖳ��
			}
		}
		return lists;
	}

	/**
	 * @brief �e�L�X�g(�t�@�C��)�w�蕶�������͂��ă��X�g�Ɋi�[����
	 * @note unsigned long�^�ɃL���X�g����
	 * @param[in]	str	�e�L�X�g(�t�@�C��)�w�蕶���� ex. hoge,fuga,... �܂��� C:\hoge.txt
	 * @retrn ���X�g(unsigned long)
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
				// �L���X�g�ł��Ȃ��ᖳ��
			}
		}
		return lists;
	}
} // text_util

} // uh
