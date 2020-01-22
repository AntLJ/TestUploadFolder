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

#include "Arguments.h"

/**
 * @brief   インポート用対応情報
 */
struct ImportInfo
{
	uh::tstring tableName;	//!< テーブル名（インポート先）
	uh::tstring fileName;	//!< CSVファイル名（インポート元）
	bool bKeepData;			//!< データ保持フラグ

	/**
	 * @brief   デフォルトコンストラクタ
	 */
	ImportInfo():bKeepData(false){}

	/**
	 * @brief   コンストラクタ
	 * @param[in]   table	テーブル名（インポート先）
	 * @param[in]   file	CSVファイル名（インポート元）
	 * @param[in]   flag	データ保持フラグ
	 */
	ImportInfo(const uh::tstring& table, const uh::tstring& file, bool flag)
		:tableName(table), fileName(file), bKeepData(flag){}
};

/**
 * @brief   メインクラス
 */
class ADAMSurveyImporter
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param[in]	args	引数
	 */
	ADAMSurveyImporter( const Arguments& args )
	{
		_args = args;
	}

	/**
	 * @brief   デストラクタ
	 */
	virtual ~ADAMSurveyImporter(){}

	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();

private:
	Arguments _args;      //!< 引数
};

