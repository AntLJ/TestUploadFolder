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

// VerifyLog.h: CVerifyLog クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERIFYLOG_H__5535F33B_DE87_4F11_A0FA_1EDC46CF699E__INCLUDED_)
#define AFX_VERIFYLOG_H__5535F33B_DE87_4F11_A0FA_1EDC46CF699E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "atlwfile.h"

#define	DELIMITA	_T(",")

/**
	内容確認用ログファイルハンドリングクラス

　　出力先：%SiNDY-e%\log
	ログ命名規則:[ホスト名]_[ユーザ名]_[年月日].log
	ログ出力フォーマット:ログハンドルツール標準準拠(カンマ区切りテキスト)
		- 1)チェック済フラグ
		- 2)レイヤ名
		- 3)オブジェクトＩＤ
		- 4)2次メッシュコード
		- 5)都市地図メッシュコード
		- 6)確認作業時のユーザ名 
		- 7)確認作業時のＰＣ名
		- 8)確認作業時のボタンを押した日時(2003/06/26 12:00:00)
		- 9)確認作業時のMXDファイル名
		- 10)確認作業時のツールのバージョン(episode???)
		- 11)最終変更時のユーザ名
		- 12)最終変更時の作業目的
		- 13)最終変更日時
		- 14)最終変更時の変更内容
		- 15)プログラム最終変更日時
		- 16)プログラム最終変更時のプログラム名
		- 17)その他コメント行
*/
class CVerifyLog  
{
public:
	CVerifyLog();
	virtual ~CVerifyLog();

	BOOL AppendVerifyLog( IMap* ipMap, _IRowPtr ipRow );

protected:
	BOOL Initialize();

protected:
	CFile				m_VerifyLogFile;	//!< 内容確認済ログファイル
	ISiNDYRulePtr	m_pISiNDYRuleObj;	//!< SiNDYルール
	CString				m_strInstallDir;	//!< SiNDY-eインストールディレクトリ名
	CString				m_strHostName;		//!< ホスト名
	CString				m_strLogDirName;	//!< ログ格納ディレクトリ名
	CString				m_strLogFileName;	//!< ログファイル名
	CString				m_strOperator;		//!< オペレーター名
	CString				m_strToolVer;		//!< ツールバージョン
	SYSTEMTIME			m_SystemTime;		//!< 内容確認時の時間

	CString				m_strPrevLayerName;	//!< 前回内容確認したレイヤ
	long				m_nPrevOID;			//!< 前回内容確認したオブジェクトID
	CFieldDef			m_UpdateFiled;
	CFieldDef			m_ProgUpdateFiled;
	DOMAINMAP*			m_UpdateFiledDomain;//!< 更新日フィールドドメイン
};

#endif // !defined(AFX_VERIFYLOG_H__5535F33B_DE87_4F11_A0FA_1EDC46CF699E__INCLUDED_)
