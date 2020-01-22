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

<!--
/**
 * @file	specification.h
 * @brief	for Doxygen
 *
 * $Id: tool.h,v 1.1.1.1 2006/04/06 14:32:00 K.Omichi Exp $
 */
@page tool_specification ツール仕様詳細
-->

<ul>
	<h2>1.背景</h2>
	<p>　ArcMap上でフィーチャの属性を調べるまたは、変更するために必要なツールとして作成された。
	ArcMap標準のツールでも属性編集ツールはあるが、汎用的にすべての属性が表示されており、
	編集させたくない属性まで変更できてしまったり、最終更新者の履歴を自動で入れたいが、
	標準ツールであるためカスタマイズすることができなかったりする。そのため新規に属性表示編集ツールを作成した。</p>


	<h2>2.要求定義</h2>
		<h3>2.1.定義</h3>
			<p>　属性表示編集ツールは、すべてのフィーチャの属性を表示することができる。
			そして編集可能なフィーチャの編集可能な属性を編集することができる。<br>
			　またリンク列も表示しているものであれば、リンク列属性編集ツールを使用して、リンク列を選択することにより、
			 リンク列の属性を表示することができる。</p>
		<h3>2.2.目的</h3>
			<p>　ArcMap上に表示されているフィーチャであれば、このツールを使うことにより、属性を表示することができ、
			可能であれば属性編集を行うことができる。また、要望があってエラーチェックが行えるものは、
			属性を編集する際にエラーチェックを行い、エラーがある旨を伝えて、間違った属性編集を行えないようにする。</p>
		<h3>2.3.機能と概観</h3>
			<h4>2.3.1.機能</h4>
				<ul>
					<li>属性編集ツールを起動し、ArcMap上で属性を見たいフィーチャを左クリック、または矩形選択することにより、
					属性ダイアログが立ち上がり、属性を見ることができる
					<li>選択されたフィーチャのレイヤに対応したダイアログ画面があれば、そのレイヤのダイアログ画面が表示される
					<li>ダイアログ画面がない場合には、汎用のグリッド表示で、そのフィーチャの属性が表示される
					<li>属性編集ツール以外からでも、フィーチャを渡されることにより、属性ダイアログを表示することができる
				</ul>
			<h4>2.3.2.前提条件</h4>
				<ul>
					<li>レイヤ毎に子ダイアログを用意する
					<li>子ダイアログがないレイヤは編集できない
				</ul>
			<h4>2.3.3.概観</h4>
				<p>属性ダイアログ<br>
				<img border="0" src="attrdlg01.png"><br>
				</p>
				<p><b>①フィーチャ属性ツリー表示部</b><br>
				　この表示部分には、レイヤ名と選択されたオブジェクトIDをツリー表示する。</p>
				<p><b>②属性表示編集部</b><br>
				　この画面が属性表示編集ダイアログのメイン部分で、選択したフィーチャの属性を見たり、編集したりすることができる部分。この画面をレイヤ毎に作成する。</p
				<p><b>③属性表示変更ボタン</b><br>
				　このボタンを押すことにより、属性表示部の表示方法をグリッド表示とコモンコントロール表示に切り替えることができる。</p
				<p><b>④属性名称変更ボタン</b><br>
				　グリッド表示の際に属性名称の切り替えを行うボタン。</p
				<p><b>⑤編集警告時強制編集ボタン</b><br>
				　このボタンは属性編集が警告状態のときに強制的に編集にするボタン。</p
				<p><b>⑥フィーチャ編集状態表示部</b><br>
				　この表示部分は今選択してある属性が編集可能か編集不可能かを表示するものです。</p
				<p><b>⑦内容確認ボタン</b><br>
				　選択したフィーチャを確認したという情報をログに残す作業をするために使うボタン。</p
				<p><b>⑧OK ボタン</b><br>
				　今編集しているフィーチャの属性編集作業を確定するためのボタン。</p
				<p><b>⑨キャンセルボタン</b><br>
				　このボタンは、ダイアログを閉じるときに押すボタンです。編集を破棄するときにも使用します。</p

	<h2>3.基本設計</h2>
		<h3>3.1.基本方針</h3>
			<ul>
				<li>子ダイアログはSiNDY-eインストールディレクトリのChildDlgDLL内に置く
				<li>属性変更処理の時、エラーチェックをする
			</ul>
		<h4>3.1.1 フロー</h4>
			<table border="0">
				<tr>
					<td align="center"><img border="0" src="attrdlg02.png"></td>
				</tr>
				<tr>
					<td align="center">Fig.2　属性表示</td>
				</tr>
				<tr>
					<td>　</td>
				</tr>
				<tr>
					<td align="center"><img border="0" src="attrdlg03.png"></td>
				</tr>
				<tr>
					<td align="center">Fig.2　編集保存</td>
				</tr>
			</table>

	<h2>4.開発方法</h2>
		<p>C++</p>
		<h3>4.1.各種名称</h3>
		<h3>4.2.ビルド環境</h3>
		<ul>
			<li>Visual Studio 2005 SP1
			<li>ATL 3.0
			<li>STLPort 4.61
			<li>Platform SDK

			<p>以下 自社ライブラリ</p>
			<li>SindyLib
		</ul>

</ul>

