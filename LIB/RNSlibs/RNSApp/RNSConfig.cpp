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

// RNSConfig.cpp: RNSConfig クラスのインプリメンテーション
//
// $Log: RNSConfig.cpp,v $
// Revision 1.7  2003/08/18 08:36:00  f_kiku
// 設定の読み込みに使用可能なストリームを、ファイルストリームから入力ストリーム全般に変更。
//
// Revision 1.6  2003/07/16 04:12:19  f_kiku
// ダブルクォーテーションによる文字列結合に対応。
//
// Revision 1.5  2003/07/14 01:22:22  f_kiku
// 指定した項目に該当する値を、出現順に連結して取得。
//
// Revision 1.4  2003/05/16 09:39:05  f_kiku
// アルゴリズム変更。
//
// Revision 1.3  2003/05/06 06:45:32  f_kiku
// 確認関数追加。
//
// Revision 1.2  2003/04/16 01:46:22  f_kiku
// init()をvirtualに（Objective Cの流儀）。
//
// Revision 1.1  2003/01/10 06:21:28  f_kiku
// 設定ファイル読み込みクラス『RNSConfig』を追加。
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include <RNSDelimitStream.h>
#include "RNSMessage.h"
#include "RNSConfig.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RNSConfig::RNSConfig()
{

}

RNSConfig::~RNSConfig()
{

}

//////////////////////////////////////////////////////////////////////
// 初期化
//////////////////////////////////////////////////////////////////////

/**
 * @brief	getItemID()がオーバーライドされていたらItemIDMapを取得する。
 *
 * @note	オーバーライドする側はRNSConfig::init()を忘れずに。
 */
void RNSConfig::init()
{
	getItemID(mItemIDMap);
}

//////////////////////////////////////////////////////////////////////
// 読み込み
//////////////////////////////////////////////////////////////////////

/**
 * @brief	ファイル名を指定して読み込みを開始
 *
 * @param	cConfigFileName	[in]	ファイル名
 * @retval	true	読み込み成功
 * @retval	false	読み込み失敗
 */
bool RNSConfig::read(const char *cConfigFileName)
{
#if _MSC_VER > 1200
	USES_CONVERSION;
	std::ifstream	aStream(T2W(cConfigFileName));
#else
	std::ifstream	aStream(cConfigFileName);
#endif
	if(! aStream)
		return false;

	return read(aStream);
}

/**
 * @brief	入力ファイルストリームを指定して読み込みを開始
 *
 * @warning	256Byte以上の<項目名>（もしくは<値>）は256Byteで分割される。
 *
 * @param	cStream	[in]	入力ストリーム
 * @retval	true	読み込み成功
 * @retval	false	読み込み失敗
 */
bool RNSConfig::read(std::istream &cStream)
{
	if(! cStream)
		return false;

	char	aWord[258];
	bool	aFirst = true;

	int			aID = 0;
	std::string	aString;

	while( cStream >> std::setw(256) >> aWord ) {
		// 語頭に "#" がついた単語から後ろはコメントとして無視する
		if(aWord[0] == '#') {
			cStream.ignore(65536, '\n');
			aFirst = true;
			continue;
		}
		else if(! strcmp(aWord, ":")) {
			aFirst = false;
			continue;
		}
		else if(aFirst) {
			long aTmpID = 0;

			ItemIDMap::iterator	itr = mItemIDMap.find(aWord);

			if(itr != mItemIDMap.end()) {
				aTmpID = itr->second;
			}

			if(aTmpID) {
				aID = aTmpID;
				aString = aWord;
				itemFunc(aTmpID);
			}
			else {
				aID = 0;
				aString = aWord;
				if(! itemFunc(aWord)) {
					mItemMap[aString];
				}
			}


			aFirst = false;
		}
		else {
			std::string aValue;
			if(aWord[0] == '\"') {
				aValue = &(aWord[1]);
				std::string aAppendString;
				std::getline(cStream, aAppendString, '\"');
				aValue += aAppendString;
			}
			else {
				aValue = aWord;
			}


			if(aID) {
				if(! valueFunc(aID, aValue.c_str())) {
					mRegisteredItemMap[aID].push_back(aValue);
				}
			}
			else {
				if(! valueFunc(aString, aValue.c_str())) {
					mItemMap[aString].push_back(aValue);
				}
			}
		}

		// 単語の区切りが改行マークだったら行頭フラグを立てる
		aFirst = (cStream.peek() == '\n');
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// 取得
//////////////////////////////////////////////////////////////////////

/**
 * @brief	項目別値一覧を取得
 *
 * @return	項目別値一覧
 */
const RNSConfig::ItemMap& RNSConfig::itemMap() const
{
	return mItemMap;
}

/**
 * @brief	指定した項目名に該当する値一覧を取得
 *
 * @param	cItemName	項目名
 * @return	値一覧
 */
const RNSConfig::ValueList* RNSConfig::valueList(const char *cItemName) const
{
	ItemMap::const_iterator itr = itemMap().find(cItemName);

	if(itr != itemMap().end())
		return &(itr->second);

	return 0;
}

/**
 * @brief	指定した項目に該当する値を取得
 *
 * @note	値が複数存在する場合は結合して返す。
 *
 * @param	cItemName	項目
 * @param	cDelimiter	結合時に使用する区切り文字
 * @return	値。存在しない場合は空文字列。
 */
RNSConfig::Value RNSConfig::value(const char* cItemName, const char* cDelimiter) const
{
	std::stringstream	aStream;
	RNSDelimitStream	aDelimStream(&aStream, cDelimiter);

	const ValueList* pValue = valueList(cItemName);
	if(! pValue || pValue->empty())
		return std::string();

	for(int i = 0; i < (int)pValue->size(); i++) {
		aDelimStream << (*pValue)[i];
		++aDelimStream;
	}

	return aStream.str();
}


/**
 * @brief	登録された項目別値一覧を取得
 *
 * @return	項目別値一覧
 */
const RNSConfig::RegisteredItemMap& RNSConfig::registeredItemMap() const
{
	return mRegisteredItemMap;
}

/**
 * @brief	指定した項目IDに該当する値一覧を取得
 *
 * @param	cItemID	項目ID
 * @return	値一覧
 */
const RNSConfig::ValueList* RNSConfig::registeredValueList(ItemID cItemID) const
{
	RegisteredItemMap::const_iterator itr = registeredItemMap().find(cItemID);

	if(itr != registeredItemMap().end())
		return &(itr->second);

	return 0;
}

/**
 * @brief	指定した項目IDに該当する値を取得
 *
 * @note	値が複数存在する場合は結合して返す。
 *
 * @param	cItemID		項目ID
 * @param	cDelimiter	結合時に使用する区切り文字
 * @return	値。存在しない場合は空文字列。
 */
RNSConfig::Value RNSConfig::registeredValue(ItemID cItemID, const char* cDelimiter) const
{
	std::stringstream	aStream;
	RNSDelimitStream	aDelimStream(&aStream, cDelimiter);

	const ValueList* pValue = registeredValueList(cItemID);
	if(! pValue || pValue->empty())
		return std::string();

	for(int i = 0; i < (int)pValue->size(); i++) {
		aDelimStream << (*pValue)[i];
		++aDelimStream;
	}

	return aStream.str();
}

//////////////////////////////////////////////////////////////////////
// 確認
//////////////////////////////////////////////////////////////////////

/**
 * @brief 設定ファイルの妥当性確認
 *
 * @note	規定外の設定項目が存在しないことを確認する。
 * @note	必須の設定項目の存在は確認しない。
 *
 * @retval	true	規定外の設定項目は存在しない
 * @retval	false	規定外の設定項目が存在する
 */
bool RNSConfig::check(std::ostream& cStream) const
{
	using namespace rns_message;

	bool aResult = true;

	const ItemMap&	rItemMap = itemMap();

	for(ItemMap::const_iterator itr = rItemMap.begin(); itr != rItemMap.end(); ++itr) {
		const std::string&	rItemName = itr->first;
		//規定外の設定項目名:項目名
		cStream << getMsg(eUnreglatedSettingItemName) << rItemName.c_str() << std::endl;
		aResult = false;
	}

	return aResult;
}
