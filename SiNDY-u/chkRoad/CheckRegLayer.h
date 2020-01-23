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
#include "ZRegLoad.h"

/**
 * @brief リンク規制エラーデータ
 */
struct ErrorRegLink
{
	CString	eFrom;			///< リンク始点ID
	CString	eTo;			///< リンク終点ID
	int		eMesh;			///< 2次メッシュ
	MError	eError;			///< エラー情報
};

/**
 * @brief 3点ノード規制エラーデータ
 */
struct ErrorRegNode
{
	CString	eInID;			///< 流入ノードID
	CString	eID;			///< 対象ノードID
	CString	eOutID;			///< 流出ノードID
	int		eMesh;			///< 2次メッシュ
	MError	eError;			///< エラー情報
};

/**
 * @brief ノード列規制エラーデータ
 */
struct ErrorRegNodes
{
	vector<CString>	eID;	///< ノードID集合
	int		eMesh;			///< 2次メッシュ
	MError	eError;			///< エラー情報
};

/**
 * @brief エラーデータ集合体
 */
struct ErrorData
{
	ErrorRegLink	eLink;		///< リンク規制
	ErrorRegNode	eNode;		///< 3点ノード規制
	ErrorRegNodes	eNodes;		///< ノード列規制
};

/**
 * @brief 新交通規制レイヤチェッククラス
 */
class CCheckRegLayer
{
public:
	CCheckRegLayer(void);
	~CCheckRegLayer(void);

	/**
	 * @brief INデータの新交通規制レイヤをチェックする。
	 * @note  指定した2次メッシュのINデータ新交通規制情報をチェックする。
	 *
	 * @param	rFilePath	[in]	道路INデータ格納ディレクトリ
	 * @param	rMeshCode	[in]	指定2次メッシュコード
	 * @param	rRegAttr	[in]	新交通規制基準値
	 *
	 * @return	ture		正常終了
	 * @return	false		異常終了
	 */
	bool check( CString &rFilePath, int rMeshCode, RegAttrList& rRegAttr );

	bool IsError() { return m_bIsError; };
private:

	/**
	 * @brief 新交通規制レイヤのエラー情報を出力する。
	 * @note  指定した2次メッシュの 新交通規制情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラー新交通規制情報
	 * @param	rCode		[in]	エラー属性値
	 * @param	rType		[in]	規制種別
	 * @param	bError		[in]	エラーフラグ
	 * @param	bHex		[in]	16進数表示フラグ
	 *
	 * @return	なし
	 */
	void	PrintError( int rKey, ErrorData& rError, int rCode, int rType, bool bError, bool bHex);

	/**
	 * @brief 新交通規制レイヤのエラー情報を出力する。
	 * @note  指定した2次メッシュの 新交通規制情報をチェックする。
	 *
	 * @param	rKey		[in]	エラー情報キー
	 * @param	rError		[in]	エラー新交通規制情報
	 * @param	rType		[in]	規制種別
	 * @param	bError		[in]	エラーフラグ
	 *
	 * @return	なし
	 */
	void	PrintError( int rKey, ErrorData& rError, int rType, bool bError);

	/**
	 * @brief 新交通規制の規制種別を判定する。
	 * @note  指定した新交通規制情報の規制種別をチェックする。
	 *
	 * @param	rReg			[in]	新交通規制情報
	 * @param	rError			[in]	新交通規制情報エラー
	 * @param	rRegAttr		[in]	新交通規制基準値
	 * @param	rMeshCode		[in]	2次メッシュコード
	 *
	 * @return	規制種別
	 */
	int		CheckType( const INReg* rReg, ErrorData& rError, RegAttrList& rRegAttr, int rMeshCode );

	/**
	 * @brief 時間指定をチェックする。
	 * @note  指定した時間指定をチェックする。
	 *
	 * @param	rStart			[in]	開始時間
	 * @param	rEnd			[in]	終了時間
	 *
	 * @return	ture	正常
	 * @return	false	異常
	 */
	bool	CheckTimeCode( int rStart, int rEnd );

	/**
	 * @brief 月日指定をチェックする。
	 * @note  指定した月日指定をチェックする。
	 *
	 * @param	rStart			[in]	開始月日
	 * @param	rEnd			[in]	終了月日
	 *
	 * @return	ture	正常
	 * @return	false	異常
	 */
	bool	CheckDayCode( int rStart, int rEnd );

	/**
	 * @brief ノード列の重複をチェックする。
	 * @note  指定した新交通規制のノード列に重複が無いかをチェックする。
	 *
	 * @param	rReg				[in]	新交通規制
	 * @param	rMeshCode			[in]	2次メッシュコード
	 * @param	rType				[in]	規制種別
	 *
	 * @return	ture	正常
	 * @return	false	異常
	 */
	bool	CheckNode( const INReg* rReg, int rMeshCode, int rType);

private:
	bool m_bIsError;
};
