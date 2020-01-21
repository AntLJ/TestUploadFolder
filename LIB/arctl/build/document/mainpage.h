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
@mainpage ArcObjectsテンプレートライブラリ

<center>
@image html ipc_logo.png
<br>
インクリメントP株式会社<br>
地図DB制作部
</center>

@date 2007/06/15
@author 菊地 福太郎

@section intro_prelibs 使用する外部ライブラリ
- <a href="http://www.boost.org/"><b>Boost</b></a>
- @b rns - 自作のテンプレートライブラリ
- @b atl2 - 自作のATL拡張ライブラリ

@section intro_preimports 動作保証環境

@subsection  intro_preimports_develop 開発環境
- Microsoft Visual Studio 8.0

@subsection  intro_preimports_arcgis ArcGIS環境
- ArcGIS Desktop 9.1
- ArcGIS Engine 9.1

@section intro_libs アルファベット順ライブラリリスト
- arctl/algorithm.h - 主に、既存のアルゴリズムをARCTL用に拡張したもの
- arctl/algorithm/table_name.h - テーブル名称文字列操作
- arctl/catalog.h - 「データベース コネクション」関係
- arctl/cursor.h - ITable::Update() によって生成された更新カーソルに関するアルゴリズム群
- arctl/field.h - 属性フィールドのインデックスや値の取得・保持・書き出しを行うクラステンプレート
- arctl/iterator.h - ICursor の走査、オブジェクトの削除など、ArcObjectsの操作を行う反復子群
- arctl/pgdb.h - PGDB関係のクラス・関数群
- arctl/sql/expression.h - SQL評価式
- arctl/topological.h - 幾何学演算群
- arctl/utility.h - その他雑用関数群
*/

/**
 * @defgroup unit_test ユニットテスト
 */
