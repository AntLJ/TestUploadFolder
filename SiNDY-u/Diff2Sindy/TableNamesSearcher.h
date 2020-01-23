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
 * @brief テーブル名称取得クラス
 */
class TableNamesSearcher
{
public:
	TableNamesSearcher();
	~TableNamesSearcher();

	/**
	 * @brief  ワークスペース内にあるテーブル名称をすべて取得する
	 * @param  ipDatasetNames  [in]  元とするDatasetの名称
	 * @param  cTableNames    [out] テーブル名の一覧
	 * @retval
	 */
	void getWorkspaceTableNames( IEnumDatasetName* ipDatasetNames, std::vector<uh::tstring>& cTableNames );
private:
	/**
	 * @brief  データセットであるか判定
	 * @param  type  [out]  esriDatasetType
	 * @retval true:データセットである
	 */
	bool isDataset(esriDatasetType type);

};
