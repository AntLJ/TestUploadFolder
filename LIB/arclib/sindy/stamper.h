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
 * @file stamper.h
 * @brief 情報刻印クラスライブラリ統合ヘッダ
 *
 * 各自必要なヘッダだけを直接読み込んでも良い。
 *
 * @section lib_stamper_date 日時の刻印
 * - sindy::time_stamper - インデックス指定による、現在日時の刻印。
 * - sindy::time_stamper_t - 属性フィールド名称指定による、現在日時の刻印。
 *   - sindy::operator_modify_date_stamper - 作業者による更新日時の刻印。
 *   - sindy::prog_modify_date_stamper - プログラム更新日時の刻印。
 *
 * @section lib_stamper_modify_date 最終更新履歴の刻印
 * - sindy::modify_stamper - 最終更新履歴の刻印。
 *   - sindy::prog_modify_stamper - プログラムによる最終更新履歴の刻印。
 *
 * @section lib_stamper_unary_modify 最終更新履歴の刻印（更新内容固定）
 * - sindy::unary_modify_stamper - @ref sindy::modify_stamper継承オブジェクトをカリー化する。
 */
#ifndef SINDY_STAMPER_H_
#define SINDY_STAMPER_H_

#include <sindy/stamper/time_stamper.h>
#include <sindy/stamper/modify_stamper.h>
#include <sindy/stamper/prog_modify_stamper.h>
#include <sindy/stamper/unary_modify_stamper.h>

#endif // SINDY_STAMPER_H_
