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

namespace data_output
{
	/**
	 * @brief	ナビ向けPntレコード出力
	 * @param	stream		[in]	出力ストリーム
	 * @param	rPntList	[in]	Pntレコードリスト
	 */
	void OutPntFile( std::ofstream& stream, const PNTMAP& rPntList );

	/**
	 * @brief	外部提供向けPntレコード出力
	 * @note	Bug7599
	 * @param	stream		[in]	出力ストリーム
	 * @param	rPntList	[in]	Pntレコードリスト
	 */
	void OutPntFile_Outside( std::ofstream& stream, const PNTMAP& rPntList );

	/**
	 * @brief	ナビ向けGouレコード出力
	 * @param	stream		[in]	出力ストリーム
	 * @param	rGouList	[in]	Gouレコードリスト
	 */
	void OutGouFile( std::ofstream& stream, const GOUMAP& rGouList );

	/**
	 * @brief	外部提供向けGouレコード出力
	 * @note	Bug7599
	 * @param	stream		[in]	出力ストリーム
	 * @param	rGouList	[in]	Gouレコードリスト
	 */
	void OutGouFile_Outside( std::ofstream& stream, const GOUMAP& rGouList );
}
