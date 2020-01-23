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

// ODBConnect.cpp: CODBConnect クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ODBConnect.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

// デフォルトコンストラクタ
CODBConnect::CODBConnect()
{	
	memset( m_szService, '\0', sizeof(m_szService) );
	memset( m_szUser, '\0', sizeof(m_szUser) );
	memset( m_szPass, '\0', sizeof(m_szPass) );
}

// 引数有りコンストラクタ
CODBConnect::CODBConnect( const char* lpszProp ) 
{
	std::string strProp = lpszProp;

	strcpy( m_szUser, strProp.substr(0, strProp.find('/')).c_str() );
	strProp.replace(0, strProp.find('/')+1, "" );
	strcpy( m_szPass, strProp.substr(0, strProp.find('@')).c_str() );
	strProp.replace(0, strProp.find('@')+1, "" );
	strcpy( m_szService, strProp.c_str() );

}

// デコンストラクタ
CODBConnect::~CODBConnect()
{
	if( m_oSess.IsOpen() )
		m_oSess.Close();
}

// 接続関数その２
bool CODBConnect::Connect( const char* lpszProp )
{
	std::string strProp = lpszProp;

	strcpy( m_szUser, strProp.substr(0, strProp.find('/')).c_str() );
	strProp.replace(0, strProp.find('/')+1, "" );
	strcpy( m_szPass, strProp.substr(0, strProp.find('@')).c_str() );
	strProp.replace(0, strProp.find('@')+1, "" );
	strcpy( m_szService, strProp.c_str() );

	// オラクルに接続
	return ( Connect() );
}

// 接続関数その他3
bool CODBConnect::ConnectWithSess( const char* lpszProp )
{
	std::string strProp = lpszProp;

	strcpy( m_szUser, strProp.substr(0, strProp.find('/')).c_str() );
	strProp.replace( 0, strProp.find('/')+1, "" );
	strcpy( m_szPass, strProp.substr(0, strProp.find('@')).c_str() );
	strProp.replace( 0, strProp.find('@')+1, "" );
	strcpy( m_szService, strProp.c_str() );

	// セッションオープン
	m_oSess.Open();

	ShowProperty();

	// オラクルに接続
	return ( m_DBSource.Open(m_oSess, m_szService, m_szUser, m_szPass)==S_OK? true : false );
}
