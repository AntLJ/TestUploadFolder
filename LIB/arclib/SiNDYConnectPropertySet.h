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

// SiNDYConnectPropertySet.h: SiNDYConnectPropertySetSet クラスのインターフェイス
//
// TODO: RoadCheckのRoadCheckLogViewerプロジェクトにも含まれているが、含まれないようにすべき

#if !defined(AFX_SINDYCONNECTPROPERTYSET_H__EF602B13_5B56_427D_B726_32D226E7EDB9__INCLUDED_)
#define AFX_SINDYCONNECTPROPERTYSET_H__EF602B13_5B56_427D_B726_32D226E7EDB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief SiNDY接続設定クラス
 */
class SiNDYConnectPropertySet  
{
public:
// 構築/消滅
	SiNDYConnectPropertySet();
	SiNDYConnectPropertySet(LPCTSTR lpszProperties);
	SiNDYConnectPropertySet(const CString& rProperties);

// 設定/取得
	void clear();
	bool empty() const;

	void setPropertySet(const SiNDYConnectPropertySet& rPropertySet);
	bool setPropertiesString(const CString& rProperties);

	CString getPropertiesString(CString sep = m_szSep) const;
	IPropertySetPtr getPropertySet() const;

	bool operator == (const SiNDYConnectPropertySet& rPropertySet) const;
	bool operator != (const SiNDYConnectPropertySet& rPropertySet) const { return ! (*this == rPropertySet); }

// プロパティ
	CString m_strServer;                 //!< サーバ
	CString m_strInstance;               //!< インスタンス（サービス）
	CString m_strUser;                   //!< ユーザ
	CString m_strPassword;               //!< パスワード（暗号化なし）
	CString m_strVersion;                //!< バージョン
	CString m_strClient;                 //!< クライアント
	CString m_strAuthenticationMode;     //!< 認証モード
	CString m_strDBConnectionProperties; //!< 接続プロパティ
	CString m_strLocalDB;                //!< PGDBのパス
	bool m_bConnect_AS;                  //!< AS接続か

// 静的プロパティ
	static const TCHAR m_szSep[];
	static const CString m_strServerName;                 //!< "SERVER"
	static const CString m_strInstanceName;               //!< "INSTANCE"
	static const CString m_strUserName;                   //!< "USER"
	static const CString m_strPasswordName;               //!< "PASSWORD"
	static const CString m_strVersionName;                //!< "VERSION"
	static const CString m_strLocalDBName;                //!< "DATABASE"
	static const CString m_strClientName;                 //!< "DBCLIENT"
	static const CString m_strAuthenticationModeName;     //!< "AUTHENTICATION_MODE"
	static const CString m_strDBConnectionPropertiesName; //!< "DB_CONNECTION_PROPERTIES"

};

#endif // !defined(AFX_SINDYCONNECTPROPERTY_H__EF602B13_5B56_427D_B726_32D226E7EDB9__INCLUDED_)
