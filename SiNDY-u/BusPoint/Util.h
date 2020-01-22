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
#include <TDC/useful_headers/tstring.h>

/**
 * @brief	引数格納用コンテナ
 */
struct Arguments
{
	uh::tstring in_list;         //!< 調査結果ファイルリスト
	uh::tstring import_log;      //!< インポートログファイル
	uh::tstring auto_rpoint_log; //!< ルート探索用ポイントログファイル
	uh::tstring db_check_log;    //!< DBチェックログファイル
	uh::tstring export_log;      //!< エクスポートログファイル
	uh::tstring bus_db;          //!< バスインポート用DB
	uh::tstring road_db;         //!< 道路NW用DB
	uh::tstring company_list;    //!< 出力事業者名リスト
	uh::tstring export_dir;      //!< 出力先フォルダ

	bool        bImport;          //!< インポートモード
	bool        bAutoRPoint;     //!< ルート探索用ポイントモード
	bool        bDbCheck;        //!< DBチェックモード
	bool        bExport;          //!< エクスポートモード
	bool        bAllCompany;     //!< 全事業者エクスポート

	Arguments(){
		bImport     = false;
		bAutoRPoint = false;
		bDbCheck    = false;
		bExport     = false;
		bAllCompany = false;
	}
};
