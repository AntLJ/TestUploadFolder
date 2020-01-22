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

/** @file
 * CApp クラスのインプリメンテーション
 *
 * @author	shimano
 * @date	2003/04/09
 *
 * $Log: App.h,v $
 * Revision 1.3  2003/11/17 01:09:03  shimano
 * Ver.1.6（アクセス道路リスト出力フォーマット修正）対応
 *
 * Revision 1.2  2003/11/15 01:35:57  shimano
 * アクセス道路属性確認ファイル出力対応（Debugモード）
 *
 * Revision 1.1.1.2  2003/04/24 09:46:44  shimano
 * 新規作成
 *
 * Revision 1.0  2003/04/09 15:13:43  shimano
 * beta8対応。
 *
 */

#if !defined(AFX_APP_H__90BEEEFE_3B9D_486C_B608_D08B434A610E__INCLUDED_)
#define AFX_APP_H__90BEEEFE_3B9D_486C_B608_D08B434A610E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"doc.h"
#define	CONFIGFILE	"config.txt"		//このツールのＳＤＥ接続パラメタファイル

class CApp  
{
public:
	CApp();
	virtual ~CApp();
	bool	init(int, TCHAR* cArgv[]);
	bool	getenv(LPCTSTR);
	int	showMenu(void);
	void	getTime(CString &);
	int		run(void);
	int	selectjobnum(char);
	int		m_JobNo;			//リスト作成番号

// アトリビュート
protected:
	CDoc	   *m_pDoc;				//ドキュメント管理クラス
	CString		m_aMeshcodeFile;	//全国分メッシュコードリストファイル
};

#endif // !defined(AFX_APP_H__90BEEEFE_3B9D_486C_B608_D08B434A610E__INCLUDED_)
