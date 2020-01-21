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
 * @file arctl/field.h
 * @brief 属性フィールド系テンプレートライブラリ統合ヘッダ。
 *
 * @section lib_field_index 属性フィールドインデックス
 * - @ref arctl::field::index_t - 属性フィールドインデックスを取得・保持する。
 * - @ref ARCTL_FIELD_INDEX_CLASS - 複数の属性フィールドインデックスの取得・保持を行うクラスを定義するマクロ。
 *
 * @section lib_field_value 属性値
 * - @ref field_value_group - 属性値の取得・保持・書き出しを行う。
 * - @ref ARCTL_FIELD_VALUE_CLASS - @ref field_value_groupを多重継承したクラスを定義するマクロ。
 *
 * @section lib_field_modifier 属性値不一致時属性更新
 * - @ref arctl::field::modifier - 属性値が異なる場合にのみ、値を書き出す。
 */
#ifndef ARCTL_FIELD_H_
#define ARCTL_FIELD_H_

#include <arctl/field/index.h>
#include <arctl/field/value.h>
#include <arctl/field/oid.h>
#include <arctl/field/shape.h>
#include <arctl/field/modifier.h>
#include <arctl/field/dmodifier.h>

#endif // ARCTL_FIELD_H_
