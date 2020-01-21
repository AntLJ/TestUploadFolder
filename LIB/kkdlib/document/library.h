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
 * @file	library.h
 * @brief	for Doxygen
 */
/**
@page library ライブラリ仕様 

@section library_caution 使用上の注意

このライブラリを使用するためには、DRMAデータハンドリングライブラリをリンクさせ、インクルードファイルへのパスを通す必要があります。\n

@section library_file ライブラリファイル

<table>
	<tr>
		<th>kkd.lib</th>
		<th>KKDデータハンドリングライブラリ・リリースバージョン</th>
	</tr>
	<tr>
		<th>kkdms.lib</th>
		<th>KKDデータハンドリングライブラリ・マルチスレッドリリースバージョン</th>
	</tr>
	<tr>
		<th>kkdmd.lib</th>
		<th>KKDデータハンドリングライブラリ・マルチスレッド(DLL)リリースバージョン</th>
	</tr>
	<tr>
		<th>kkdd.lib</th>
		<th>KKDデータハンドリングライブラリ・デバッグバージョン</th>
	</tr>
	<tr>
		<th>kkdmsd.lib</th>
		<th>KKDデータハンドリングライブラリ・マルチスレッドデバッグバージョン</th>
	</tr>
	<tr>
		<th>kkdmdd.lib</th>
		<th>KKDデータハンドリングライブラリ・マルチスレッド(DLL)デバッグバージョン</th>
	</tr>
	<tr>
		<th>kkdfd.lib</th>
		<th>KKDデータハンドリングライブラリ・BoundsChecker FinalCheckバージョン</th>
	</tr>
</table>

@section library_include インクルードファイル

<table>
	<tr>
		<th>KKDLoad.h</th>
		<th>KKDデータを丸ごとハンドリングするときに使用</th>
	</tr>
	<tr>
		<th>KKDLayerLoad.h</th>
		<th>KKDデータをレイヤごとにハンドリングするときに使用</th>
	</tr>
</table>

@section library_example ライブラリ使用例

@include example.cpp

*/
