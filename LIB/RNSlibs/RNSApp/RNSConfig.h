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

// RNSConfig.h: RNSConfig クラスのインターフェイス
//
// $Log: RNSConfig.h,v $
// Revision 1.7  2003/08/18 08:36:00  f_kiku
// 設定の読み込みに使用可能なストリームを、ファイルストリームから入力ストリーム全般に変更。
//
// Revision 1.6  2003/07/14 01:22:22  f_kiku
// 指定した項目に該当する値を、出現順に連結して取得。
//
// Revision 1.5  2003/05/16 09:39:05  f_kiku
// アルゴリズム変更。
//
// Revision 1.4  2003/05/06 06:45:32  f_kiku
// 確認関数追加。
//
// Revision 1.3  2003/04/16 01:46:22  f_kiku
// init()をvirtualに（Objective Cの流儀）。
//
// Revision 1.2  2003/01/22 11:00:22  f_kiku
// コメント修正。
//
// Revision 1.1  2003/01/10 06:21:28  f_kiku
// 設定ファイル読み込みクラス『RNSConfig』を追加。
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIG_H__3909720C_72F9_406F_B829_C6044BED81D8__INCLUDED_)
#define AFX_CONFIG_H__3909720C_72F9_406F_B829_C6044BED81D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <RNS.h>

#include <map>
#include <vector>
#include <string>

#include <atlconv.h>

/**
 * @brief	設定ファイル読み込みクラス
 *
 * @note	<項目名> <値> <値> ... という形式のファイルを解析する。
 * @note	"#"で始まる行はコメントとして読み飛ばす。
 * @warning	256Byte以上の<項目名>（もしくは<値>）は256Byteで分割される。
 */
class RNSConfig  
{
public:
	typedef	std::string					Item;				///< 項目名
	typedef unsigned int				ItemID;				///< 項目ID
	typedef std::string					Value;				///< 値

	typedef	std::vector<Value>			ValueList;			///< 値リスト
	typedef std::map<Item, ValueList>	ItemMap;			///< 項目別値リスト

	typedef	std::map<Item, ItemID>		ItemIDMap;			///< 項目別ID
	typedef std::map<ItemID, ValueList>	RegisteredItemMap;	///< 登録された項目別値リスト

	// --- 構築/消去 --- //

	RNSConfig();
	virtual ~RNSConfig();

	// --- 初期設定 --- //

	virtual void init();

	// --- 読み込み --- //

	/// ファイル名を指定して読み込み
	bool read(const char* cCongfiFileName);

		/// ストリームを指定して読み込み
	bool read(std::istream& cStream);

	// --- 取得 --- //

	/// 項目別値リストを取得する
	const ItemMap&				itemMap() const;

	/// 指定した項目の値リストを取得する
	const ValueList*			valueList(const char* cItemName) const;

	/// 指定した項目の値を取得する
	Value						value(const char* cItemName, const char* cDelimiter = "\t") const;

	/// 登録された項目別値リストを取得する
	const RegisteredItemMap&	registeredItemMap() const;

	/// 指定した項目IDの値リストを取得する
	const ValueList*			registeredValueList(ItemID cItemID) const;

	/// 指定した項目IDの値を取得する
	Value						registeredValue(ItemID cItemID, const char* cDelimiter = "\t") const;

	// --- 確認 --- //

	/// 登録されていない項目リストを出力する
	bool check(std::ostream& cStream) const;

protected:

	/**
	 * @brief	項目名にID付けをする
	 *
	 * @note	coItemIDMap[<項目名>] = <ID> として、項目名にIDを付与する。
	 * @note	IDを付けた項目は、mRegisteredItemMapに登録されるようになる。
	 * @warning	IDは正の整数にすること（0もだめ）。
	 *
	 * @param	coItemMap	[out]	項目別ID
	 */
	virtual void getItemID(ItemIDMap& coItemIDMap) const
	{
	}

	/**
	 * @brief	項目読み込み時拡張関数
	 *
	 * @note	<項目名>によって拡張したい処理をオーバーライドする。
	 *
	 * @param	cItem	[in]	項目名
	 * @retval	true	定義された項目名
	 * @retval	false	定義されていない項目名
	 */
	virtual bool itemFunc(const char* cItem)
	{
		return false;
	}

	/**
	 * @brief	項目読み込み時拡張関数
	 *
	 * @note	<項目ID>によって拡張したい処理をオーバーライドする。
	 *
	 * @param	cID		[in]	項目ID
	 */
	virtual void itemFunc(ItemID cID)
	{
	}

	/**
	 * @brief	値読み込み時拡張関数
	 *
	 * @note	<値>によって拡張したい処理をオーバーライドする。
	 *
	 * @param	cItem	[in]	カレント項目名
	 * @param	cValue	[in]	値文字列
	 * @retval	true	定義された項目名に対する値読み込み
	 * @retval	false	定義されていない項目名に対する値読み込み
	 */
	virtual bool valueFunc(const std::string& cItem, const char* cValue)
	{
		return false;
	}

	/**
	 * @brief	値読み込み時拡張関数
	 *
	 * @note	<値>によって拡張したい処理をオーバーライドする。
	 *
	 * @param	cItemID	[in]	カレント項目ID
	 * @param	cValue	[in]	値文字列
	 * @retval	true	定義された項目IDに対する値読み込み
	 * @retval	false	定義されていない項目IDに対する値読み込み
	 */
	virtual bool valueFunc(ItemID cItemID, const char* cValue)
	{
		return false;
	}

	// === 〈プロパティ〉 === //

	ItemIDMap			mItemIDMap;			///< 登録された項目別ID
	RegisteredItemMap	mRegisteredItemMap;	///< 登録された項目別値リスト
	ItemMap				mItemMap;			///< 項目別値リスト
};

#endif // !defined(AFX_CONFIG_H__3909720C_72F9_406F_B829_C6044BED81D8__INCLUDED_)
