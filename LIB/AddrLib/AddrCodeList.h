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

#include <set>
#include <SiNDYLib/FeatureClass.h>

namespace addr
{

using namespace sindy;

/**
 * @class	CAddrCodeList
 * @brief	住所コードリスト管理クラス
 * @note	都市地図行政界から住所コードを生成する形です。
 *			(住所コードを直接指定・コードリスト指定も可)
 */
class CAddrCodeList : public CFeatureClass
{
public:

	explicit CAddrCodeList(void){}
	virtual ~CAddrCodeList(void){}

	CAddrCodeList( sindyTableType::ECode emTableType, ITable* lp ){ SetObject( emTableType, lp ); }
	CAddrCodeList( sindyTableType::ECode emTableType, IFeatureClass* lp ){ SetObject( emTableType, lp ); }
	CAddrCodeList( ITable* lp ){ SetObject( lp ); }
	CAddrCodeList( IFeatureClass* lp ){	SetObject( lp ); }

	/**
	 * @brief	<b>全国リスト作成<b>
	 * @note	都市行政界からすべての住所コードリストを生成
	 * @param	lLevel	[in]	コードレベル[1-11]
	 * @return	bool
	 */
	bool MakeCodeListAll( long lLevel );

	/**
	 * @brief	<b>コード範囲指定</b>
	 * @note	指定した範囲で、都市地図行政界に存在する住所コードリストを生成
	 * @param	lpcszStartRange	[in]	開始コード
	 * @param	lpcszEndRange	[in]	終了コード
	 * @return	bool
	 */
	bool MakeCodeListFromRange( const _TCHAR* lpcszStartRange, const _TCHAR* lpcszEndRange );

	/**
	 * @brief	<b>コードリストファイルから追加</b>
	 * @param	lpcszFile	[in]	コードリスト
	 * @param	lLevel		[in]	何桁目まで読み込み？[1-11](デフォルト0 : 1行をそのまま読み込み)
	 * @return	bool
	 */
	bool AddFromFile( const _TCHAR* lpcszFile, long lLevel = 0 );

	/**
	 * @brief	<b>コード追加</b>
	 * @param	lpcszCode	[in]	住所コード
	 */
	void Add( const _TCHAR* lpcszCode ){ m_listCode.insert(lpcszCode); }

	typedef std::set<CString>	CodeList;
	typedef CodeList::iterator iterator;
	typedef CodeList::const_iterator const_iterator;

	iterator begin(){ return m_listCode.begin(); }
	iterator end(){ return m_listCode.end(); }
	size_t size(){ return m_listCode.size(); }
	void clear(){ return m_listCode.clear(); }

private:

	/**
	 * @brief	<b>クエリから追加</b>
	 * @param	lpcszWhere		[in]	Where句
	 * @param	lLevel			[in]	住所コード桁[1-11]
	 * @return	bool
	 */
	bool AddFromQuery( const _TCHAR* lpcszWhere, long lLevel );

private:

	CodeList	m_listCode;	//!< 住所コードリスト確認用

};

}	// namespace addr
