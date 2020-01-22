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
#include "logger.h"
#include "TDC/sindylib_base/RowContainer.h"
#include "ErrLvDef.h"
#include "ErrTypeDef.h"

/**
 * @brief エラーログ管理クラス
 */
class CErrLogger :
	public CLogger
{
public:
	explicit CErrLogger(const uh::tstring& path) : CLogger(path){}

	/**
	 * @brief ヘッダを出力する
	 * @note ログハンドルツールの自由形式ログで出力する
	 */
	void header();
	
	/**
	 * @brief 指定したフィーチャの情報をログに出力する
	 * @param row[in] 対象フィーチャのRowコンテナ
	 * @param errCode[in] エラーコード
	 */
	void writeErrInfo( const sindy::CRowContainer& row, const err_type::error_type errCode );
};

