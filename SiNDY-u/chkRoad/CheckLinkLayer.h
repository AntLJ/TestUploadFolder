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
#include "Z2LinkLoad.h"
#include "Z2NodeLoad.h"

/**
 * @brief 道路リンクエラーデータ
 */
struct ErrorLink
{
	CString		eFrom;			///< リンク始点ID
	CString		eTo;			///< リンク終点ID
	int			eMesh;			///< ２次メッシュ
	MError		eError;			///< エラー情報
};

typedef pair<int,int>			NodeXY;

/**
 * @brief 道路リンクレイヤチェッククラス
 */
class CCheckLinkLayer
{
public:
	CCheckLinkLayer(void);
	~CCheckLinkLayer(void);

	/**
	 * @brief INデータの道路リンクレイヤをチェックする。
	 * @note  指定した2次メッシュの道路リンクレイヤをチェックする。
	 *
	 * @param	rFilePath	[in]	道路INデータ格納ディレクトリ
	 * @param	rMeshCode	[in]	指定2次メッシュコード
	 * @param	rLinkAttr	[in]	リンク基準値
	 * @param	eCustomer	[in]	仕向け先
	 * @param	setMesh		[in]	VICSチェック用メッシュリスト
	 *
	 * @return	ture		正常終了
	 * @return	false		異常終了
	 */
	bool check( CString &rFilePath, int rMeshCode, LinkAttrList& rLinkAttr, Customer eCustomer, const std::set<int>& setMesh );

	bool IsError() { return m_bIsError; };
private:

	/**
	 * @brief リンクレイヤのエラー情報を出力する。(コード値:16進数)
	 * @note  指定した2次メッシュのリンク情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーリンク情報
	 * @param	rCode		[in]	エラー属性値
	 *
	 * @return	ture		正常終了
	 * @return	false		異常終了
	 */
	void PrintHexError( int rKey, ErrorLink& rError, int rCode); 

	/**
	 * @brief リンクレイヤのエラー情報を出力する。(コード値:16進数)
	 * @note  指定した2次メッシュのリンク情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラーリンク情報
	 * @param	rCode		[in]	エラー属性値
	 *
	 * @return	ture		正常終了
	 * @return	false		異常終了
	 */
	void PrintError( int rKey, ErrorLink& rError, int rCode);

	/**
	* @brief	VICSチェック
	* @param	setMesh				[in]	VICSチェック用メッシュリスト
	* @param	aError				[in]	エラー出力クラス
	* @param	pLink				[in]	道路リンク
	* @param	bVicsOrdRev			[in]	順or逆方向にVICSが付与されているか
	*/
	void CheckVics( const std::set<int>& setMesh, ErrorLink& aError, IN2Link& pLink, bool bVicsOrdRev );

	/**
	* @brief	VICSチェック（区分とメッシュ）
	* @param	setMesh				[in]	メッシュリスト
	* @param	aError				[in]	エラー出力クラス
	* @param	pLink				[in]	VICSリンク
	* @param	bVicsOrdRev			[in]	順or逆方向にVICSが付与されているか
	*/
	void CheckVicsPartMesh( const std::set<int>& setMesh, ErrorLink& aError, IN2Link& pLink, bool bVicsOrdRev );

private:
	bool m_bIsError;
};
