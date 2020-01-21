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

/**
 * @class	CCodeConvTable
 * @brief	文字付き住所とコードの対応表
 */
class CCodeConvTable
{
public:
	CCodeConvTable(void){}
	~CCodeConvTable(void){ m_cAdoHelper.disconnect(); }

	/**
	 * @brief	データベースオープン
	 * @param	lpcszDBProp	[in]	
	 * @return	bool
	 */
	bool OpenDB( LPCTSTR lpcszDBProp );

	/**
	 * @brief	地番コード取得
	 * @param	strAddrCode	[in]	11桁コード
	 * @param	strPntName	[in]	地番名称
	 * @param	bSave		[in]	取得したコードが新規ならテーブルに保存する？(デフォルト true)
	 * @return	新規地番コード
	 */
	CString	GetPntCode( const CString& strAddrCode, const CString& strPntName, bool bSave = true );

	/**
	 * @brief	住居番号コード取得
	 * @param	strAddrCode	[in]	11桁コード
	 * @param	strGouName	[in]	住居番号名称
	 * @return	新規住居番号コード
	 */
	CString	GetGouCode( const CString& strAddrCode, const CString& strGouName );

private:

	/**
	 * @brief	新規コード登録
	 * @param	szClass		[in]	p: pnt g: gon
	 * @param	strAddrCode	[in]	11桁住所コード
	 * @param	strName		[in]	名称
	 * @param	strNewCode	[in]	新規コード
	 * @return	bool
	 */
	bool InsertCode( TCHAR szClass, const CString& strAddrCode, const CString& strName, const CString& strNewCode );

private:

	CADOBase		m_cAdoHelper;		//!< ADOヘルパー
};
