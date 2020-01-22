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

// プログラム引数
static const char* g_strShapeDir = "shp_dir";
static const char* g_strMeshlist = "meshlist";
static const char* g_strSpatialDB = "spatial_db";
static const char* g_strSpatialDBSchema = "spatial_db_schema";
static const char* g_strRunLog = "run_log";
static const char* g_strErrLog = "err_log";

/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	Arguments()
	{
	}

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

	CString GetOption();

private:
	CString GetOptionLine(const CString& strParam, const CString& strValue);

public:
	uh::tstring strShapeDir;        //!< Shapeのパス
	uh::tstring strMeshlist;        //!< メッシュリストのパス
	uh::tstring strSpatialDB;       //!< 空間参照取得用DBパス
	uh::tstring strSpatialDBSchema; //!< 空間参照取得用DBのスキーマ名
	uh::tstring strRunLog;          //!< 実行ログパス
	uh::tstring strErrLog;          //!< エラーログパス
};
