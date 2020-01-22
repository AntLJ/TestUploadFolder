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

#include <map>
#include <string>

/**
 * @class	CTmpCodeCreator
 * @brief	文字付き住所用に一時的にコードを割当するクラス
 * @note	リリースデータの住所コードとは全く無関係。
 *			あくまでこのチェック内でのみ有効となる
 */
class CTmpCodeCreator
{
public:
	CTmpCodeCreator(void){}
	~CTmpCodeCreator(void){}

	/**
	 * @brief	仮コード取得
	 * @param	strAddrCode	[in]	11桁住所コード
	 * @param	strPntName	[in]	文字地番名
	 * @return	仮コード
	 */
	std::string GetTmpCode( const std::string& strAddrCode, const std::string& strPntName );

private:

	/**
	 * @brief	新コード申請
	 * @param	strAddrCode	[in]	11桁住所コード
	 * @param	strPntName	[in]	文字地番名
	 * @return	新仮コード
	 */
	std::string insertNewTmpCode( const std::string& strAddrCode, const std::string& strPntName );

	typedef std::map< std::string, std::string >	STRINGMAP;
	typedef std::map< std::string, STRINGMAP >		TMPCODEMAP;
	TMPCODEMAP	m_TmpCodeMap;	//!< 仮コードマップ	
};
