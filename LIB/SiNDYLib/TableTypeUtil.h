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

/**
 * @file TableTypeUtil.h
 * @brief <b>テーブルタイプ使用ユーティリティ定義ファイル</b>\n
 * 定義の際に、FeatureTableType.hを読み込んでマクロ経由で定義します。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _TABLETYPEUTIL_H_
#define _TABLETYPEUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TableType.h"
#include "exportimport.h"

namespace sindy {

/**
 * @brief <b>テーブルタイプからテーブル名に変換する</b>\n
 * @return テーブル名
 */
LPCTSTR SINDYLIB_API AheTableType2TableName( sindyTableType::ECode emCode );

/**
 * @brief <b>テーブル名からテーブルタイプに変換する</b>\n
 * @return テーブルタイプ
 */
sindyTableType::ECode SINDYLIB_API AheTableName2TableType( LPCTSTR lpcszTableName );

/**
 * @brief <b>テーブルタイプがLQ系かどうか</b>\n
 * @return 
 */
bool SINDYLIB_API AheIsLQTableType( sindyTableType::ECode emCode );

} // sindy

#endif // _TABLETYPEUTIL_H_
