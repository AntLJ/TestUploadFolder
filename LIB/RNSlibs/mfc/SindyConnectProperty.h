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

// SindyConnectProperty.h: CSindyConnectProperty クラスのインターフェイス
// $Id: SindyConnectProperty.h,v 1.1.1.1 2003/09/12 07:26:04 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYCONNECTPROPERTY_H__EF602B13_5B56_427D_B726_32D226E7EDB9__INCLUDED_)
#define AFX_SINDYCONNECTPROPERTY_H__EF602B13_5B56_427D_B726_32D226E7EDB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief SiNDY接続設定クラス
 */
class CSindyConnectProperty  
{
public:
// 設定処理
	void setPropertiesString(CString strProperties);

// 設定処理
	void clear();

// 取得処理
	CString getPropertiesString() const;
	IPropertySetPtr getPropertySet() const;

// プロパティ
	CString e_strServer; ///< サーバ
	CString e_strInstance; ///< インスタンス（サービス）
	CString e_strUser; ///< ユーザ
	CString e_strPassword; ///< パスワード（暗号化なし）
	CString e_strVersion; ///< バージョン

// 静的プロパティ
	static CString e_strServerName; ///< "SERVER"
	static CString e_strInstanceName; ///< "INSTANCE"
	static CString e_strUserName; ///< "USER"
	static CString e_strPasswordName; ///< "PASSWORD"
	static CString e_strVersionName; ///< "VERSION"
};

/**
 * @brief SiNDY接続設定クラス・iniファイル対応版
 */
class CSindyIniConnectProperty : public CSindyConnectProperty
{
public:
// 構築/消滅
	CSindyIniConnectProperty(LPCTSTR lpSectionName, LPCTSTR lpIniFilePath = 0);

// 入力
	void getFromPrivateIni();

// 出力
	void writeToPrivateIni();

// 判定
	BOOL isDefault() const;
	BOOL isDefaultServer() const { return e_strServer == m_cDefault.e_strServer; }
	BOOL isDefaultInstance() const { return e_strInstance == m_cDefault.e_strInstance; }
	BOOL isDefaultUser() const { return e_strUser == m_cDefault.e_strUser; }
	BOOL isDefaultPassword() const { return e_strPassword == m_cDefault.e_strPassword; }
	BOOL isDefaultVersion() const { return e_strVersion == m_cDefault.e_strVersion; }

protected:
// 入力
	void getFromPrivateIni(LPCTSTR lpSectionName);

// 出力
	void writeToPrivateIni(LPCTSTR lpSectionName);

// プロパティ
	CSindyConnectProperty  m_cDefault;

private:
	CString m_strIniPath; ///< 〈モジュール所在ディレクトリ〉\\connectproperty.ini
	CString m_strSection; ///< iniファイルセクション名
};

/**
 * @brief SiNDY接続設定クラス・共通
 *
 * コンストラクタでiniファイルよりセクション名"Common"に属する設定を読み込む。
 */
class CSindyCommonConnectProperty : public CSindyIniConnectProperty
{
public:
	CSindyCommonConnectProperty(LPCTSTR lpIniFilePath = 0) :
	CSindyIniConnectProperty(_T("Common"), lpIniFilePath) {}
};

/**
 * @brief SiNDY接続設定クラス・道路
 *
 * コンストラクタでiniファイルよりセクション名"Road"に属する設定を読み込む。
 */
class CSindyRoadConnectProperty : public CSindyIniConnectProperty
{
public:
	CSindyRoadConnectProperty(LPCTSTR lpIniFilePath = 0) :
	CSindyIniConnectProperty(_T("Road"), lpIniFilePath) {}
};

/**
 * @brief SiNDY接続設定クラス・中縮
 *
 * コンストラクタでiniファイルよりセクション名"Base"に属する設定を読み込む。
 */
class CSindyBaseConnectProperty : public CSindyIniConnectProperty
{
public:
	CSindyBaseConnectProperty(LPCTSTR lpIniFilePath = 0) :
	CSindyIniConnectProperty(_T("Base"), lpIniFilePath) {}
};

/**
 * @brief SiNDY接続設定クラス・都市地図
 *
 * コンストラクタでiniファイルよりセクション名"City"に属する設定を読み込む。
 */
class CSindyCityConnectProperty : public CSindyIniConnectProperty
{
public:
	CSindyCityConnectProperty(LPCTSTR lpIniFilePath = 0) :
	CSindyIniConnectProperty(_T("City"), lpIniFilePath) {}
};

/**
 * @brief SiNDY接続設定クラス・住所
 *
 * コンストラクタでiniファイルよりセクション名"Addr"に属する設定を読み込む。
 */
class CSindyAddrConnectProperty : public CSindyIniConnectProperty
{
public:
	CSindyAddrConnectProperty(LPCTSTR lpIniFilePath = 0) :
	CSindyIniConnectProperty(_T("Addr"), lpIniFilePath) {}
};

#endif // !defined(AFX_SINDYCONNECTPROPERTY_H__EF602B13_5B56_427D_B726_32D226E7EDB9__INCLUDED_)
