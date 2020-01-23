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
 * @brief 道路ノードエラーデータ
 */
struct ErrorNode
{
	CString		eID;			///< ノードID
	int			eMesh;			///< ２次メッシュ
	MError		eError;			///< エラー情報
};

/**
 * @brief 道路ノードレイヤチェッククラス
 */
class CCheckNodeLayer
{
public:
	CCheckNodeLayer(void);
	~CCheckNodeLayer(void);

	/**
	 * @brief INデータの道路ノードレイヤをチェックする。
	 * @note  指定した2次メッシュの道路ノード情報をチェックする。
	 *
	 * @param	rFilePath	[in]	道路INデータ格納ディレクトリ
	 * @param	rMeshCode	[in]	指定2次メッシュコード
	 * @param	rNodeAttr	[in]	ノード基準値
	 *
	 * @return	ture		正常終了
	 * @return	false		異常終了
	 */
	bool check( CString &rFilePath, int rMeshCode, NodeAttrList& rNodeAttr );

	bool IsError() { return m_bIsError; };
private:

	/**
	 * @brief ノードレイヤのエラー情報を出力する。(コード値:16進数)
	 * @note  指定した2次メッシュの道路ノード情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーノード情報
	 * @param	rCode		[in]	エラー属性値
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void PrintHexError( int rKey, ErrorNode& rError, int rCode, bool bError); 

	/**
	 * @brief ノードレイヤのエラー情報を出力する。(コード値:10進数)
	 * @note  指定した2次メッシュの道路ノード情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーノード情報
	 * @param	rCode		[in]	エラー属性値
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void PrintError( int rKey, ErrorNode& rError, int rCode, bool bError); 

	/**
	 * @brief ノードレイヤのエラー情報を出力する。
	 * @note  指定した2次メッシュの道路ノード情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーノード情報
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void PrintError( int rKey, ErrorNode& rError, bool bError); 

private:
	bool m_bIsError;
};
