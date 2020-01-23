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

// ODBConnect.h: CODBConnect クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODBCONNECT_H__DC075AE7_ADE6_42F6_9DA5_857C1821F544__INCLUDED_)
#define AFX_ODBCONNECT_H__DC075AE7_ADE6_42F6_9DA5_857C1821F544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Oracl.h"

/**
 * @class	CODBConnect
 * @brief	オラクル接続クラス
 */
class CODBConnect  
{
	
public:

	CODBConnect();

	/**
	 * @brief	コンストラクタ(先にプロパティ情報セット)
	 * @param	lpszProp	[in]	接続プロパティ(Ex. k0505/k0505@delltcp)
	 */
	CODBConnect( const char* lpszProp );

	~CODBConnect();

	/**
	 * @brief	オラクルＤＢに接続関数その１( プロパティ情報を既に保持しているとき )
	 * @return	bool
	 */
	bool Connect()
	{
		ShowProperty();
		return ( m_DBSource.Open(m_szService, m_szUser, m_szPass)==S_OK? true : false );
	}

	/**
	 * @brief	オラクルＤＢに接続関数その２(プロパティ情報直接指定)
	 * @note	デフォルトコンストラクタを呼び出しているときはこちらをどうぞ
	 * @param	lpszProp	[in]	接続プロパティ(Ex. k0505/k0505@delltcp)
	 * @return	bool
	 */
	bool Connect( const char* lpszProp );

	/**
	 * @brief	接続関数その3(セッション付き)
	 * @param	lpszProp	[in]	接続プロパティ
	 * @return	bool
	 */
	bool ConnectWithSess( const char* lpszProp );
	
	/**
	 * @brief	接続情報表示関数
	 */
	void ShowProperty()
	{
		std::cerr << "【住所ＤＢ接続プロパティ表示】" << std::endl;
		std::cerr << "USER     : " << m_szUser << std::endl;
		std::cerr << "PASSWORD : " << m_szPass << std::endl;
		std::cerr << "SERVICE  : " << m_szService << std::endl;
		std::cerr << std::endl;
	}

	/**
	 * @brief	DBソース取得
	 * @return	ODatabase
	 */
	const ODatabase& GetDbSource() const { return m_DBSource; }

	/**
	 * @brief	セッション取得
	 * @return	OSeesion
	 */
	const OSession& GetSession() const { return m_oSess; }

	/**
	 * @brief	SQL実行 
	 * @note	読み取り専用です
	 * @param	lpszSQL		[in]		SQL文
	 * @param	lpDynaset	[in, out]	ODynasetクラス
	 * @return	bool
	 */
	bool Execute( const char* lpszSQL, ODynaset* lpDynaset )
	{
		lpDynaset->Open( m_DBSource, lpszSQL, ODYNASET_READONLY );

		return ( lpDynaset->IsOpen()? true : false );
	}

//////////////////////////////////////////////////////////////
//	メンバ変数
////////////////////////////////////////////////////////////
private:

	char m_szService[50];	/// サービス名(Ex.delltcp)
	char m_szUser[50];		/// ユーザ名
	char m_szPass[50];		/// パスワード

	ODatabase m_DBSource;	/// DBソース

	OSession	m_oSess;	/// セッション

};

#endif // !defined(AFX_ODBCONNECT_H__DC075AE7_ADE6_42F6_9DA5_857C1821F544__INCLUDED_)
