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

#include "stdafx.h"

#define LOG_ERROR	"ERROR"

#define MWN_LAYER	_T("MWN")
#define SQL_NODECLS _T("wkndcls_c = ")

typedef std::map<long, long> MAP_REL_TBL;			// 関連テーブルマップ（キー：NODE_ID、要素：POI_ID）
typedef std::map<long, WKSPoint> MAP_POI_DATA;		// POIデータマップ（キー：管理コード、要素：経度・緯度）

namespace nsCommon
{
	/**
	 * @brief	リスト読み込み(vector)
	 * @param	file		[in]	ファイルパス
	 * @param	vecList		[out]	コンテナ
	 * @param	sErrMsg		[out]	エラーメッセージ
	 * @return						処理成否
	 */
	bool loadList2vec(const std::string& file, std::vector<std::string>& vecList, std::string& sErrMsg);

	/**
	 * @brief	リスト読み込み(set)
	 * @param	file		[in]	ファイルパス
	 * @param	setList		[out]	コンテナ
	 * @param	sErrMsg		[out]	エラーメッセージ
	 * @return						処理成否
	 */
	bool loadList2set(const std::string& file, std::set<std::string>& setList, std::string& sErrMsg);

	/**
	 * @breif	フィールドインデックスの取得
	 * @param	ipTable		[in]	取得元テーブル
	 * @param	csField		[in]	フィールド名
	 * @param	lIndex		[out]	インデックス
	 * @param	sErrMsg		[out]	エラーメッセージ
	 * @return						処理成否
	 */
	bool getFieldIndex(const ITablePtr ipTable,
					   const CString& csField,
					   long &lIndex,
					   std::string& sErrMsg);

	/**
	 * @breif	レイヤ名取得
	 * @param	sFilePath	[in]	解析ファイル（フルパス）
	 * @return						レイヤ名
	 */
	std::string getLayerName(const std::string& sFilePath);

	/**
	 * @breif	ファイル名無しパス取得
	 * @param	sFilePath	[in]	解析ファイル（フルパス）
	 * @return						ファイル名無しパス
	 */
	std::string getPath(const std::string& sFilePath);
};

