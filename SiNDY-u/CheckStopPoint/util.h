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


namespace util {

	/**
	 * @brief 整数を列挙したファイルを読み込む
	 * @note 典型的にはメッシュリストファイルの読み込みに使う
	 * @param fileName [in] リストファイル名
	 * @param integerList [out] 出力リストのset
	 * @retval true 読み込み成功
	 * @retval false 読み込み失敗
	 */
	inline bool loadMeshList(const CString& fileName, std::set<long>& list)
	{
		std::ifstream ifs(fileName);
			
		// ストリームを取得できなければfalse
		if( !ifs ){
			return false;
		}
		std::copy(
			std::istream_iterator<long>(ifs),
			std::istream_iterator<long>(),
			inserter(list, list.end())
		);
		if(ifs.fail() && !ifs.eof()){
			return false;
		}
		return true;
	};
}
