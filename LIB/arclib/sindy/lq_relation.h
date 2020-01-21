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
 * @file lq_relation.h
 * @brief リンク列関係操作クラスライブラリ統合ヘッダ
 *
 * @section lib_lq_relation_interface インタフェース
 * - sindy::lq_relation_exist - リンクに関連付くリンク列の有無を確認する。
 * - sindy::lq_relation_edit - リンクにリンク列を関連付けたり、関連付けを解除する。
 * - sindy::lq_relation_mutable - sindy::lq_relation_exist と sindy::lq_relation_mutable を仮想継承したもの。
 * - sindy::directional_lq_relation_exist - リンクに関連付くリンク列の有無を方向別に確認する。
 * - sindy::directional_lq_relation_mutable - sindy::directional_lq_relation_exist と sindy::lq_relation_mutable を仮想継承したもの。
 *
 * @section lib_lq_relation_implement 実装
 * - sindy::bidirectional_lq_relation - VICSやインターナビVICSといった、リンクの方向に対してユニークに関連付けられるリンク列用の実装。
 *
 * @section lib_lq_relation_watcher 監視
 * - sindy::lq_relation_edit_watcher - リンク列の関連付けが更新されたリンクを保持する。
 * - sindy::lq_relation_watcher - 関連付けの更新によるフラグの必要性を厳密に監視する。
 * - sindy::directional_lq_relation_watcher - sindy::lq_relation_watcher に sindy::directional_lq_relation_exist を加えたもの。
 */

#ifndef SINDY_LQ_RELATION_H_
#define SINDY_LQ_RELATION_H_

#include <sindy/lq_relation/interface.h>
#include <sindy/lq_relation/directinal_interface.h>
#include <sindy/lq_relation/bidirectional.h>
#include <sindy/lq_relation/edit_watcher.h>
#include <sindy/lq_relation/watcher.h>
#include <sindy/lq_relation/directional_watcher.h>

#endif // SINDY_LQ_RELATION_H_
