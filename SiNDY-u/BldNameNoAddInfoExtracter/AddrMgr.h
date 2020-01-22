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
#include <WinLib/ADOBase.h>

/// 住所情報保持用構造体
struct AddrInfo
{
	CString addr_code;  //!< 住所コード（11桁）
	CString ken_kanji;  //!< 都道府県
	CString ken_kana;   //!< 都道府県（ヨミ）
	CString shi_kanji;  //!< 市区町村
	CString shi_kana;   //!< 市区町村（ヨミ）
	CString oaza_kana;  //!< 大字
	CString oaza_kanji; //!< 大字（ヨミ）
	CString aza_kanji;  //!< 字
	CString aza_kana;   //!< 字（ヨミ）
};

class AddrMgr
{
public:

	AddrMgr(void){}
	virtual ~AddrMgr(void){}

	/**
	* @brief 初期化
	* @note 接続失敗した場合は例外を投げる
	* @param conStr [in] 接続文字（[ユーザ名@サーバ名]）
	*/
	void init( const CString& conStr );
	
	/**
	* @brief 市区町村レベルで住所文字列をキャッシュする
	* @note 接続失敗した場合は例外を投げる
	* @param cityCode [in] 住所コード（5桁）
	*/
	void cacheAddrInCity( const CString& cityCode );
	
	/**
	* @brief 11桁レベルの住所情報を取得する
	* @note 先に cacheAddrInCity()されている必要がある
	* @param cityCode [in] 住所コード（11桁）
	* @return 対応する住所情報
	*/
	AddrInfo& getAddrInfo( const CString& addrCode ); 

private:
	CADOBase m_ado;                      //!< 住所サーバ用
	std::map<CString, AddrInfo> m_addrs; //!< 住所情報キャッシュ用
};

