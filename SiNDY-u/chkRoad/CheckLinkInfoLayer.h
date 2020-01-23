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

#include "SetRoadAttrList.h"

/**
 * @brief リンク情報エラーデータ
 */
struct ErrorLInfo
{
	CString			eFrom;			///< リンク始点ID
	CString			eTo;			///< リンク終点ID
	int				eMesh;			///< 2次メッシュコード
	MError			eError;			///< エラー情報
};

/**
 * @brief リンク情報レイヤチェッククラス
 */
class CCheckLinkInfoLayer
{
public:
	CCheckLinkInfoLayer(void);
	~CCheckLinkInfoLayer(void);

	/**
	 * @brief INデータのリンク情報レイヤをチェックする。
	 * @note  指定した2次メッシュのリンク情報をチェックする。
	 *
	 * @param	rFilePath	[in]	道路INデータ格納ディレクトリ
	 * @param	rMeshCode	[in]	指定2次メッシュコード
	 * @param	rLInfoAttr	[in]	リンク情報基準値
	 *
	 * @return	ture		正常終了
	 * @return	false		異常終了
	 */
	bool check( CString &rFilePath, int rMeshCode, LInfoAttrList& rLInfoAttr );

private:

	/**
	 * @brief リンク情報レイヤのエラー情報を出力する。(コード値:16進数)
	 * @note  指定した2次メッシュのリンク情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーリンク情報
	 * @param	rCode		[in]	エラー属性値
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void	PrintHexError( int rKey, ErrorLInfo& rError, int rCode, bool bError);

	/**
	 * @brief リンク情報レイヤのエラー情報を出力する。(コード値:10進数)
	 * @note  指定した2次メッシュのリンク情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーリンク情報
	 * @param	rCode		[in]	エラー属性値
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void	PrintError( int rKey, ErrorLInfo& rError, int rCode, bool bError);

	/**
	 * @brief リンク情報レイヤのエラー情報を出力する。
	 * @note  指定した2次メッシュのリンク情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーリンク情報
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void	PrintError( int rKey, ErrorLInfo& rError, bool bError);
};
