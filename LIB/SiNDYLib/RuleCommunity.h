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
 * @file RuleCommunity.h
 * @brief <b>ルール系テーブルフィールド・ドメイン定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _RULECOMMUNITY_H_
#define _RULECOMMUNITY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"


const int _AUTOVALUE_MAX_LEN		= 255;				//!< 自動作成属性値の最大文字数
const int _PURPOSE_MAX_LEN			= 64;				//!< 作業目的名フィールド長
const int _MXD_MAX_LEN				= 255;				//!< MXDファイル名フィールド長（_MAX_PATHと同じにするべき）
const int _CATEGORY_MAX_LEN			= 50;				//!< カテゴリフィールド長
const int _LAYERFILEPATH_MAX_LEN	= 512;				//!< レイヤファイルパスフィールド長（_MAX_PATHと同じにするべき）

const int _TABLENAME_MAX_LEN		= 30;				//!< テーブル名最大文字列長（Oracleの制限）
const int _OWNERNAME_MAX_LEN		= 30;				//!< オーナー名最大文字列長（Oracleの制限）
const int _OWNERTABLENAME_MAX_LEN	= 60;				//!< _TABLENAME_MAX_LEN + _OWNERNAME_MAX_LEN
const int _NAMESTRING_MAX_LEN		= 128;				//!< 注記最大文字数

#endif // _RULECOMMUNITY_H_
