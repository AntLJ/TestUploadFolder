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

// SiNDYConnectPropertySet.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CStringTokenizer.h"
#include "SiNDYConnectPropertySet.h"
#include <sindy/workspace.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const TCHAR SiNDYConnectPropertySet::m_szSep[] = _T(";");

const CString SiNDYConnectPropertySet::m_strServerName(_T("SERVER"));
const CString SiNDYConnectPropertySet::m_strInstanceName(_T("INSTANCE"));
const CString SiNDYConnectPropertySet::m_strUserName(_T("USER"));
const CString SiNDYConnectPropertySet::m_strPasswordName(_T("PASSWORD"));
const CString SiNDYConnectPropertySet::m_strVersionName(_T("VERSION"));
const CString SiNDYConnectPropertySet::m_strLocalDBName(_T("DATABASE"));
const CString SiNDYConnectPropertySet::m_strClientName(_T("DBCLIENT"));
const CString SiNDYConnectPropertySet::m_strAuthenticationModeName(_T("AUTHENTICATION_MODE"));
const CString SiNDYConnectPropertySet::m_strDBConnectionPropertiesName(_T("DB_CONNECTION_PROPERTIES"));

// SiNDYConnectPropertySet クラスのインプリメンテーション
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SiNDYConnectPropertySet::SiNDYConnectPropertySet() : m_bConnect_AS(false)
{

}

SiNDYConnectPropertySet::SiNDYConnectPropertySet(LPCTSTR lpszProperties) : m_bConnect_AS(false)
{
	setPropertiesString(lpszProperties);
}

SiNDYConnectPropertySet::SiNDYConnectPropertySet(const CString& rProperties) : m_bConnect_AS(false)
{
	setPropertiesString(rProperties);
}

//////////////////////////////////////////////////////////////////////
// 設定/取得
//////////////////////////////////////////////////////////////////////

void SiNDYConnectPropertySet::clear()
{
	m_strServer.Empty();
	m_strInstance.Empty();
	m_strUser.Empty();
	m_strPassword.Empty();
	m_strVersion.Empty();
	m_strLocalDB.Empty();
	m_strClient.Empty();
	m_strAuthenticationMode.Empty();
	m_strDBConnectionProperties.Empty();
	m_bConnect_AS = false;
}

bool SiNDYConnectPropertySet::empty() const
{
	return (
		m_strServer.IsEmpty() &&
		m_strInstance.IsEmpty() &&
		m_strUser.IsEmpty() &&
		m_strPassword.IsEmpty() &&
		m_strVersion.IsEmpty() &&
		m_strLocalDB.IsEmpty() &&
		m_strClient.IsEmpty() &&
		m_strAuthenticationMode.IsEmpty() &&
		m_strDBConnectionProperties.IsEmpty()
	);
}

void SiNDYConnectPropertySet::setPropertySet(const SiNDYConnectPropertySet& rConnectPropertySet)
{
	m_strServer = rConnectPropertySet.m_strServer;
	m_strInstance = rConnectPropertySet.m_strInstance;
	m_strUser = rConnectPropertySet.m_strUser;
	m_strPassword = rConnectPropertySet.m_strPassword;
	m_strVersion = rConnectPropertySet.m_strVersion;
	m_strLocalDB = rConnectPropertySet.m_strLocalDB;
	m_strClient = rConnectPropertySet.m_strClient;
	m_strAuthenticationMode = rConnectPropertySet.m_strAuthenticationMode;
	m_strDBConnectionProperties = rConnectPropertySet.m_strDBConnectionProperties;
	m_bConnect_AS = rConnectPropertySet.m_bConnect_AS;
}

/**
 * @brief "〈ユーザ〉/〈パスワード〉/〈バージョン〉/〈インスタンス〉/〈サーバ〉"という形式の文字列から値を設定する
 *
 * @param rProperties [in] 対象文字列
 */
bool SiNDYConnectPropertySet::setPropertiesString(const CString& rProperties)
{
	clear();
	CString strProperty;
	sindy::is_connect_string_as(rProperties, m_bConnect_AS, strProperty);

	CStringTokenizer tkz(strProperty, m_szSep, TOKEN_RET_EMPTY_ALL);
	// PGDBの場合は一つしかプロパティがない
	if( tkz.CountTokens() == 1 )
	{
		m_strLocalDB = tkz.GetNextToken();
		return ! tkz.HasMoreTokens();
	}

	// ここから下はSDE接続
	// DC接続の場合
	if(!m_bConnect_AS)
	{
		m_strClient = _T("oracle");
		m_strAuthenticationMode = _T("DBMS");
	}

	for(int i = 0; tkz.HasMoreTokens(); i++) {
		switch(i) {
		case 0:
			m_strUser = tkz.GetNextToken();
			break;
		case 1:
			m_strPassword = tkz.GetNextToken();
			break;
		case 2:
			m_strVersion = tkz.GetNextToken();
			break;
		case 3:
			if(m_bConnect_AS)
				m_strInstance = tkz.GetNextToken();
			else
				tkz.GetNextToken();  // 値はセットしない
			break;
		case 4:
			if(m_bConnect_AS)
				m_strServer = tkz.GetNextToken();
			else
				m_strDBConnectionProperties = tkz.GetNextToken();
			return ! tkz.HasMoreTokens();
		}
	}

	return false;
}

/**
 * @brief "〈ユーザ〉/〈パスワード〉/〈バージョン〉/〈インスタンス〉/〈サーバ〉"という形式の文字列を取得する
 *
 * @param sep [in] 区切り文字 (デフォルトはm_szSepの値(;)) 
 *
 * @return 接続プロパティ文字列
 */
CString SiNDYConnectPropertySet::getPropertiesString(CString sep) const
{
	CString strProperties;
	if(m_bConnect_AS)
	{
		// 接頭辞を追加"[AS]"
		strProperties.Format(
			_T("[AS]%s%s%s%s%s%s%s%s%s"),
			m_strUser, sep,
			m_strPassword, sep,
			m_strVersion, sep,
			m_strInstance, sep,
			m_strServer
			);
	}
	else
	{
		strProperties.Format(
			_T("%s%s%s%s%s%s%s%s%s"),
			m_strUser, sep,
			m_strPassword, sep,
			m_strVersion, sep,
			_T("5151"), sep,
			m_strDBConnectionProperties
			);
	}
	return strProperties;
}

IPropertySetPtr SiNDYConnectPropertySet::getPropertySet() const
{
	IPropertySetPtr ipPropertySet(CLSID_PropertySet);
	// PGDBにも対応する(bug9822)
	if( (m_strDBConnectionProperties.IsEmpty() && m_strUser.IsEmpty() && m_strPassword.IsEmpty() && m_strVersion.IsEmpty()) // DC接続
		|| (m_strServer.IsEmpty() && m_strInstance.IsEmpty() && m_strUser.IsEmpty() && m_strPassword.IsEmpty() && m_strVersion.IsEmpty())) // AS接続
		ipPropertySet->SetProperty(CComBSTR(_T("DATABASE")), CComVariant(m_strLocalDB));
	else
	{
		if(!m_bConnect_AS)
		{
			IWorkspaceName2Ptr ipWorkspaceName = sindy::create_workspace_name_oracle_dc(
				CComVariant(m_strDBConnectionProperties),
				CComVariant(m_strUser),
				CComVariant(m_strPassword),
				CComVariant(m_strVersion)			);
			ipWorkspaceName->get_ConnectionProperties(&ipPropertySet);
		}
		else
		{
			IWorkspaceName2Ptr ipWorkspaceName = sindy::create_workspace_name_oracle_as(
				CComVariant(m_strServer),
				CComVariant(m_strUser),
				CComVariant(m_strPassword),
				CComVariant(m_strVersion),
				CComVariant(m_strInstance));
			ipWorkspaceName->get_ConnectionProperties(&ipPropertySet);
		}

	}

	return ipPropertySet;
}

bool SiNDYConnectPropertySet::operator == (const SiNDYConnectPropertySet& rPropertySet) const
{
	return (
		// DC
		(m_strServer == rPropertySet.m_strServer &&
		m_strInstance == rPropertySet.m_strInstance &&
		m_strUser == rPropertySet.m_strUser &&
		m_strPassword == rPropertySet.m_strPassword &&
		m_strVersion == rPropertySet.m_strVersion) ||
		
		// ローカルDB(PGDBなど)
		m_strLocalDB == rPropertySet.m_strLocalDB ||
		
		// AS
		(m_strUser == rPropertySet.m_strUser &&
		m_strPassword == rPropertySet.m_strPassword &&
		m_strVersion == rPropertySet.m_strVersion &&
		m_strDBConnectionProperties == rPropertySet.m_strDBConnectionProperties)
	);
}
