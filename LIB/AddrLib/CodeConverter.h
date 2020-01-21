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

namespace addr
{
/**
 * @class	CCodeConverter
 * @brief	文字付き住所コードヘルパークラス
 * @note	プロセスで一つあれば良いので、シングルトン
 */
class CCodeConverter
{
private:
	CCodeConverter(void){}
	~CCodeConverter(void){}
	CCodeConverter( const CCodeConverter& ){}
	CCodeConverter& operator=( const CCodeConverter& ){}

public:

	/**
	 * @brief	インスタンス取得
	 * @return	CCodeHelper
	 */
	static CCodeConverter& GetCodeConverter()
	{
		static CCodeConverter cCodeConverter;
		return cCodeConverter;
	}

	/**
	 * @brief	初期化
	 * @param	lpcszCodeConvDB	[in]	コード変換DB(AccessDBパス)
	 * @return	bool
	 */
	bool Init( const _TCHAR* lpcszCodeConvDB );

	/**
	 * @brief	地番コード取得
	 * @param	lpcszAddrCode	[in]			11桁コード
	 * @param	lpcszPntName	[in]			地番名称
	 * @param	bSave			[in, optional]	取得したコードが新規ならテーブルに保存する？(デフォルト false)
	 * @return	新規地番コード(取得失敗時は、NULL)
	 */
	CString	GetPntCode( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszPntName, bool bSave = false );

	/**
	 * @brief	住居番号コード取得
	 * @param	lpcszAddrCode	[in]			11桁コード
	 * @param	lpcszGouName	[in]			住居番号名称
	 * @param	bSave			[in, optional]	取得したコードが新規ならテーブルに保存する？(デフォルト false)
	 * @return	新規住居番号コード(取得失敗時は、NULL)
	 */
	CString	GetGouCode( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszGouName, bool bSave = false );

	/**
	 * @brief	地番名称取得
	 * @param	lpcszAddrCode	[in]	11桁コード
	 * @param	lpcszPntCode	[in]	地番コード
	 * @return	地番名称(取得失敗時は、NULL)
	 */
	CString GetPntName( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszPntCode ) const;

	/**
	 * @brief	住居番号名称取得
	 * @param	lpcszAddrCode	[in]	11桁コード
	 * @param	lpcszGouCode	[in]	住居番号コード
	 * @return	住居番号名称(取得失敗時は、NULL)
	 */
	CString GetGouName( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszGouCode ) const;

	/**
	 * @brief	エラーメッセージ取得
	 * @return	エラーメッセージ
	 */
	CString GetErrorMessage() const{ return m_strError; }

private:

	/**
	 * @brief	新規コード登録
	 * @param	szClass		[in]	p: pnt g: gon
	 * @param	lpcszAddrCode	[in]	11桁住所コード
	 * @param	lpcszName		[in]	名称
	 * @param	lpcszNewCode	[in]	新規コード
	 * @return	bool
	 */
	bool InsertCode( TCHAR szClass, const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszName, const _TCHAR* lpcszNewCode );

private:

	CADOBase	m_cAdoHelper;	//!< ADOヘルパー
	mutable		CString		m_strError;		//!< 最後のエラーメッセージ

};

}	// namespace addr
