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
 * @brief リンク拡張エラーデータ
 */
struct ErrorLExt
{
	CString			eFrom;			///< リンク始点ID
	CString			eTo;			///< リンク終点ID
	int				eMesh;			///< 2次メッシュコード
	MError			eError;			///< エラー情報
};

/**
 * @brief リンク拡張レイヤチェッククラス
 */
class CCheckLinkExLayer
{
public:
	CCheckLinkExLayer(void);
	~CCheckLinkExLayer(void);

	/**
	 * @brief INデータのリンク拡張レイヤをチェックする。
	 * @note  指定した2次メッシュのリンク拡張情報をチェックする。
	 *
	 * @param	rFilePath	[in]	道路INデータ格納ディレクトリ
	 * @param	rMeshCode	[in]	指定2次メッシュコード
	 * @param	rLInfoAttr	[in]	リンク拡張情報基準値
	 *
	 * @return	ture		正常終了
	 * @return	false		異常終了
	 */
	bool check( CString &rFilePath, int rMeshCode, LinkExAttrList& rLInfoAttr );

	bool IsError() { return m_bIsError; };
private:

	/**
	 * @brief リンク拡張レイヤのエラー情報を出力する。(コード値:16進数)
	 * @note  指定した2次メッシュの方面案内情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーリンク拡張情報
	 * @param	rCode		[in]	エラー属性値
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void	PrintHexError( int rKey, ErrorLExt& rError, int rCode, bool bError);

	/**
	 * @brief リンク拡張レイヤのエラー情報を出力する。(コード値:10進数)
	 * @note  指定した2次メッシュの方面案内情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーリンク拡張情報
	 * @param	rCode		[in]	エラー属性値
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void	PrintError( int rKey, ErrorLExt& rError, int rCode, bool bError);

	/**
	 * @brief リンク拡張レイヤのエラー情報を出力する。
	 * @note  指定した2次メッシュの方面案内情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーリンク拡張情報
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void	PrintError( int rKey, ErrorLExt& rError, bool bError);

	/**
	 * @brief 指定キーIDのリンク情報をチェックする。
	 * @note  指定したキーIDのリンク情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーリンク拡張情報
	 * @param	rLinkExAttr	[in]	リンク拡張基準値
	 * @param	rCode		[in]	エラー属性値
	 *
	 * @return	なし
	 */
	void	CheckLinkInfo( int rKey, ErrorLExt& rError, LinkExAttrList& rLinkExAttr, int rCode );

private:
	bool m_bIsError;
};
