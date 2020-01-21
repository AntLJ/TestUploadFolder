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
 * @file mainpage.h
 * @brief for Doxygen
 */
/**
@mainpage SiNDYテンプレートライブラリ

<center>
@image html ipc_logo.png
<br>
インクリメントP株式会社<br>
地図DB制作部
</center>

@version 1.1.0.0
@date 2005/04/28
@author 菊地 福太郎

@section intro_prelibs 使用するライブラリ
- <a href="http://www.boost.org/"><b>Boost</b></a>
- @b rns - 自作のテンプレートライブラリ
- @b sqltl - SQL構文テンプレートライブラリ
- @b atl2 - 自作のATL拡張ライブラリ
- @b arctl - ArcObjectsテンプレートライブラリ

@section intro_preimports 使用するタイプライブラリ
- @b msxml3.dll - esricore.olb の high_method が必要とする IXMLDOMElementPtr を定義するため。
  - named_guids
  - raw_interfaces_only
- @b esricore.olb
  - high_method_prefix("_")
  - raw_method_prefix("")
  - exclude("OLE_HANDLE", "OLE_COLOR")

@section intro_libs アルファベット順ライブラリリスト

- stamper.h - オブジェクトに様々な情報を刻むクラスライブラリ。
- @ref stamp_modifier.h "modifier/stamp_modifier.h" - iPCFeature継承オブジェクトの更新・生成時に、@ref term_sindy_historyを刻み込むクラスライブラリ。
- transform_to_nq.h - リンクの並びをノードの並びに変換するアルゴリズム。
- lq_relation.h - リンク視点によるリンク列情報管理ライブラリ。

@section intro_lastupdate 更新履歴

- <b>2005/04/28 VERSION 1.1.0.0</b>
  - lq_relation.h を追加。
  - @b sindy::modifier を sindy::stamp_modifier に改名。
- <b>2005/04/12 VERSION 1.0.0.1</b>
  - 既存の全てのライブラリをインライン化。
- <b>2005/03/31 VERSION 1.0.0.0</b>
  - 公開バージョン。
*/

/**
 * @defgroup unit_test ユニットテスト
 */
