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

#include <list>
#include <atlstr.h>             // CString

/**
* @brief 表中のある行の値を各要素に持つデータ型
*/
typedef std::list<CString>            AOI_LINE;

/**
* @brief 表中のある列の値を各要素に持つデータ型
*/
typedef std::list<CString>            AOI_COLUMN;

/**
* @brief 1つの表を表すデータ型
* @note  表中に行名やカラム名は存在しない
*/
typedef std::list<AOI_LINE> AOI_MATRIX;

/**
* @brief 処理対象とするフィールド名を各要素に格納するデータ型
*/
typedef std::list<CString> FIELDNAMES;
