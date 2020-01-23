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
#include "ZLaneLoad.h"

/**
 * @brief レーン情報エラーデータ
 */
struct ErrorLane
{
	vector<CString>	eID;
	int				eMesh;
	MError			eError;
};

/**
 * @brief レーン情報レイヤチェッククラス
 */
class CCheckLaneLayer
{
public:
	CCheckLaneLayer(void);
	~CCheckLaneLayer(void);

	/**
	 * @brief INデータのレーン情報レイヤをチェックする。
	 * @note  指定した2次メッシュのレーン情報をチェックする。
	 *
	 * @param	rFilePath	[in]	道路INデータ格納ディレクトリ
	 * @param	rMeshCode	[in]	指定2次メッシュコード
	 * @param	rLaneAttr	[in]	レーン情報基準値
	 *
	 * @return	ture		正常終了
	 * @return	false		異常終了
	 */
	bool check( CString &rFilePath, int rMeshCode, LaneAttrList& rLaneAttr );

	bool IsError() { return m_bIsError; };
private:
	/**
	 * @brief レーン情報レイヤのエラー情報を出力する。(コード値:16進数)
	 * @note  指定した2次メッシュのレーン情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーレーン情報
	 * @param	rCode		[in]	エラー属性値
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void	PrintHexError( int rKey, ErrorLane& rError, int rCode, bool bError );

	/**
	 * @brief レーン情報レイヤのエラー情報を出力する。(コード値:10進数)
	 * @note  指定した2次メッシュのレーン情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーレーン情報
	 * @param	rCode		[in]	エラー属性値
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void	PrintError( int rKey, ErrorLane& rError, int rCode, bool bError);

	/**
	 * @brief レーン情報レイヤのエラー情報を出力する。
	 * @note  指定した2次メッシュのレーン情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーレーン情報
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void	PrintError( int rKey, ErrorLane& rError, bool bError);

	/**
	 * @brief 対象ノードID列をエラーデータに格納する。
	 * @note  レーン情報を構成するノードID群をレーンエラー情報に格納する。
	 *
	 * @param	pLane		[in]	レーン情報
	 * @param	rError		[in]	エラーレーン情報
	 *
	 * @return	なし
	 */
	void	SetID( INLane* pLane, ErrorLane& rError);

private:
	bool m_bIsError;
};
