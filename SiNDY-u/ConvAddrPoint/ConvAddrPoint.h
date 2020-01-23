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

/**
 * @file	ConvAddrPoint.h
 */

// 件数表ファイル名定義
const CString C_COUNT_AUTH_ADR_FILE = _T("count_auth_adr.txt");
const CString C_COUNT_AUTH_POS_FILE = _T("count_auth_pos.txt");

/**
 * @class	CConvAddrPoint
 * @brief	新住所用TSV変換メインクラス
 */
class CConvAddrPoint
{
public:
	CConvAddrPoint(CParam& cParam);
	~CConvAddrPoint(void);

	/**
	* @brief	初期処理
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool init(void);

	/**
	* @brief	処理実行
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool execute(void);

private:
	/**
	* @brief	adr_authdb処理実行
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool adr_authdb(void);

	/**
	* @brief	住所件数表作成
	* @param	adrp001	[in]	ADRP001変換クラス
	* @param	adrp002	[in]	ADRP002変換クラス
	* @param	adrp003	[in]	ADRP003変換クラス
	* @param	adrp004	[in]	ADRP004変換クラス
	* @param	adrp005	[in]	ADRP005変換クラス
	* @param	adrp006	[in]	ADRP006変換クラス
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool make_count_auth_adr(const CAdrp001& adrp001, const CAdrp002& adrp002, const CAdrp003& adrp003, const CAdrp004& adrp004, const CAdrp005& adrp005, const CAdrp006& adrp006);

	/**
	* @brief	pos_authdb処理実行
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool pos_authdb(void);

	/**
	* @brief	郵便番号件数表作成
	* @param	zipp001	[in]	Zipp001変換クラス
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool make_count_auth_pos(const CZipp001& zipp001);

	/**
	* @brief	adrmst_authdb処理実行
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool adrmst_authdb(void);

private:
	CParam&			m_cParam;
	IWorkspacePtr	m_ipWorkspaceAddr;
	IWorkspacePtr	m_ipWorkspaceMlang;
};

