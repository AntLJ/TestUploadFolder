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
#include "DataDef.h"

namespace parser
{

/**
 * @brief 無視して良いタグかどうか判定
 * @param tag [in] 判定するタグ
 * @retval true  無視してよい
 * @retval false 未定義のタグ
 */
bool isIgnoreTag( const uh::tstring& tag );

/**
 * @brief XMLファイルからインデックス操作定義を読込む
 * @param filepath [in]  XMLファイルのパス。
 * @param retult   [out] 読込んだ結果の格納先。中身がある場合、上書きされる
 */
bool xmlToIndexDef(const uh::tstring& filepath, index_def::INDEXINFOMAP& result);

} // parser
