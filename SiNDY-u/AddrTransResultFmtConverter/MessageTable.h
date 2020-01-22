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
 * @brief メッセージコード定義ファイル
 * Message.[cpp|h]でマクロ展開される。
 * code   : enumで展開するエラーコード名
 * msg_jp : メッセージ(日本語)
 */

// TODO: 各チェックで共通で使えそうなメッセージは共通メッセージとして定義し、それを使うようにする
// info
MESSAGE_TABLE(
eCityFileStartConvert,
L"city.txtファイル処理開始\n")
MESSAGE_TABLE(
eCityFileEndConvert,
L"city.txtファイル処理完了\n")
MESSAGE_TABLE(
eAddrFileStartConvert,
L"アドレスファイル処理開始\n")
MESSAGE_TABLE(
eAddrFileEndConvert,
L"アドレスファイル処理完了\n")

// error
MESSAGE_TABLE(
eFailInitConvertControl,
L"#Error 制御クラス初期化に失敗\n")
MESSAGE_TABLE(
eFailAnalyzeArgument,
L"#Error 引数解析エラー\n")
MESSAGE_TABLE(
eFailAnalyzeConvMode,
L"--conv_modeへの指定が無効である\n")
MESSAGE_TABLE(
eFailAnalyzeTransResult,
L"--trans_resultへの指定が無効である\n")
MESSAGE_TABLE(
eFailAnalyzeOut,
L"--outへの指定が無効である\n")
MESSAGE_TABLE(
eFailAnalyzeRunLog,
L"--run_logへの指定が無効である\n")
MESSAGE_TABLE(
eFailAnalyzeErrLog,
L"--err_logへの指定が無効である\n")
MESSAGE_TABLE(
eFailRunLogInitialize,
L"#Error 実行時ログの初期化に失敗しました\n")
MESSAGE_TABLE(
eFailErrLogInitialize,
L"#Error エラーログの初期化に失敗しました\n")
MESSAGE_TABLE(
eFailReadFile,
L"の読み込みが失敗しました\n")
MESSAGE_TABLE(
eFailCreateFile,
L"の作成が失敗しました\n")
MESSAGE_TABLE(
eFailGetFile,
L"ファイルがありません!\n")
MESSAGE_TABLE(
eFileColumnErr,
L"」のフィールドが全て存在しています")
MESSAGE_TABLE(
eFindDifference,
L"同一8桁コード内で翻訳結果に差異があります")
MESSAGE_TABLE(
eFailOpenFile,
L"ファイルオープンが失敗しました\n")