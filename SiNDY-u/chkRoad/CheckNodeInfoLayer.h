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
#include "ZNInfoLoad.h"

/**
 * @brief ノード情報エラーデータ
 */
struct ErrorNInfo
{
	vector<CString>	eID;			///< 対象ID群
	int				eMesh;			///< 2次メッシュコード
	MError			eError;			///< エラー情報
};

/**
 * @brief ノード情報レイヤチェッククラス
 */
class CCheckNodeInfoLayer
{
public:
	CCheckNodeInfoLayer(void);
	~CCheckNodeInfoLayer(void);

	/**
	 * @brief INデータのノード情報レイヤをチェックする。
	 * @note  指定した2次メッシュのノード情報をチェックする。
	 *
	 * @param	rFilePath	[in]	道路INデータ格納ディレクトリ
	 * @param	rMeshCode	[in]	指定2次メッシュコード
	 * @param	rNInfoAttr	[in]	ノード情報基準値
	 *
	 * @return	ture		正常終了
	 * @return	false		異常終了
	 */
	bool check( CString &rFilePath, int rMeshCode, NInfoAttrList& rNInfoAttr );

	bool IsError() { return m_bIsError; };
private:

	/**
	 * @brief 対象ノードID列をエラーデータに格納する。
	 * @note  ノード情報を構成するノードID群をノード情報エラーに格納する。
	 *
	 * @param	pNInfo		[in]	ノード情報
	 * @param	rError		[in]	ノード情報エラー
	 *
	 * @return	なし
	 */
	void	SetID( INNInfo* pNInfo, ErrorNInfo& rError);

	/**
	 * @brief ノード列に重複がないかをチェックする。
	 * @note  ノード情報を構成するノードID群に重複がないかをチェックする。
	 *
	 * @param	pNInfo		[in]	ノード情報
	 * @param	rError		[in]	ノード情報エラー
	 *
	 * @return	なし
	 */
	bool	CheckNode( INNInfo* pNInfo, ErrorNInfo& rError);

	/**
	 * @brief ノード情報レイヤのエラー情報を出力する。(コード値:文字列)
	 * @note  指定した2次メッシュのノード情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーリンク情報
	 * @param	rCode		[in]	エラー属性値
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void	PrintStringError( int rKey, ErrorNInfo& rError, CString& rCode, bool bError);

	/**
	 * @brief ノード情報レイヤのエラー情報を出力する。(コード値:16進数)
	 * @note  指定した2次メッシュのノード情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーノード情報
	 * @param	rCode		[in]	エラー属性値
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void	PrintHexError( int rKey, ErrorNInfo& rError, int rCode, bool bError);

	/**
	 * @brief ノード情報レイヤのエラー情報を出力する。(コード値:10進数)
	 * @note  指定した2次メッシュのノード情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーリンク情報
	 * @param	rCode		[in]	エラー属性値
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void	PrintError( int rKey, ErrorNInfo& rError, int rCode, bool bError);

	/**
	 * @brief ノード情報レイヤのエラー情報を出力する。
	 * @note  指定した2次メッシュのノード情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーリンク情報
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void	PrintError( int rKey, ErrorNInfo& rError, bool bError);

private:
	bool m_bIsError;
};
