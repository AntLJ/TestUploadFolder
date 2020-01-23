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
static const char* g_strBeforeShpPath = "before_shp";
static const char* g_strBeforeDBPath = "before_db";
static const char* g_strAfterShpPath = "after_shp";
static const char* g_strMeshDBPath = "mesh_db";
static const char* g_strMeshSchema = "mesh_schema";
static const char* g_strMeshlistPath = "meshlist_path";
static const char* g_strCheckMode = "check_mode";
static const char* g_strTky2JgdPath = "tky2jgd_path";
static const char* g_strMinSegment = "min_segment";
static const char* g_strRunlog = "runlog_path";
static const char* g_strErrlog = "errlog_path";

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

	CString Arguments::GetOption();

private:
	CString GetOptionLine(const CString& strParam, const CString& strValue);

public:
	uh::tstring BeforeShpPath;       //!< 変換前Shapeのパス
	uh::tstring BeforeDBPath;        //!< 変換前Shapeのパス
	uh::tstring AfterShpPath;        //!< 変換後Shapeのパス
	uh::tstring MeshDBPath;          //!< メッシュDB
	uh::tstring MeshSchema;          //!< メッシュDBのスキーマ
	uh::tstring MeshlistPath;        //!< メッシュリストのパス
	uh::tstring CheckMode;           //!< チェックモード（1=リンク内属性リンク化処理, 2=歩行者ノード分割・歩行者リンクマージ処理, 3=高さノード分割処理）
	uh::tstring Tky2JgdPath;         //!< 地域パラメータのパス
	uh::tstring MinSegment;          //!< 微小セグメント長[単位m]
	uh::tstring ErrLogPath;          //!< エラーログのパス
	uh::tstring RunLogPath;          //!< 実行ログのパス

};
