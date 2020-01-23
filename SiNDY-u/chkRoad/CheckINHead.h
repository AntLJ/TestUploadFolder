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
 * @brief INヘッダ情報エラーデータ
 */
struct ErrorHead
{
	CString		eFileName;		///< ファイル名称
	int			eMesh;			///< 2次メッシュコード
	MError		eError;			///< エラー情報
};

/**
 * @brief INヘッダチェッククラス
 */
class CCheckINHead
{
public:
	CCheckINHead(void);
	~CCheckINHead(void);

	/**
	 * @brief INデータのヘッダ情報をチェックする。
	 * @note  指定した2次メッシュのINデータヘッダ情報をチェックする。
	 *
	 * @param	rMntDir		[in]	道路INデータ格納ディレクトリ
	 * @param	iMeshCode	[in]	指定2次メッシュコード
	 * @param	rHeadAttr	[in]	INヘッダエラー情報
	 *
	 * @return	ture		正常終了
	 * @return	false		異常終了
	 */
	bool check( CString& rMntDir, int iMeshCode, HeadAttrList& rHeadAttr);

	bool IsError() { return m_bIsError; };
private:
	/**
	 * @brief 指定INデータファイルのヘッダ情報をチェックする。
	 * @note  指定したINデータヘッダ情報をチェックする。
	 *
	 * @param	rFilePath		[in]	指定INデータファイル
	 * @param	rHeadAttr		[in]	INヘッダエラー情報
	 * @param	rMeshCode		[in]	2次メッシュコード
	 *
	 * @return	ture		正常終了
	 * @return	false		異常終了
	 */
	bool checkFile( CString& rFilePath, HeadAttrList& rHeadAttr, int rMeshCode);

	/**
	 * @brief 指定INデータファイルのヘッダ情報をチェックする。
	 * @note  指定したINデータヘッダ情報をチェックする。
	 *
	 * @param	rWord			[in]	入力バイナリコード
	 * @param	bBcd			[in]	BCDコードにするかのフラグ
	 *
	 * @return	数字コード
	 */
	int ReverseWord(char* rWord, bool bBcd);

	/**
	 * @brief INヘッダのエラー情報を出力する。(コード値:10進数)
	 * @note  指定した2次メッシュのINヘッダ情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラー交差点名称情報
	 * @param	rCode		[in]	エラー値
	 *
	 * @return	なし
	 */
	void PrintError(int rKey, ErrorHead& rError, int rCode);

	/**
	 * @brief INヘッダのエラー情報を出力する。(コード値:文字列)
	 * @note  指定した2次メッシュのINヘッダ情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラー交差点名称情報
	 * @param	rWord		[in]	エラー値
	 *
	 * @return	なし
	 */
	void PrintStringError(int rKey, ErrorHead& rError, CString& rWord);

private:
	bool m_bIsError;		// エラーが存在したかどうか
};
