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
iStartInit,
_T("初期化開始\n"))
MESSAGE_TABLE(
iLogPathError,
_T("誤りのログパス\n"))
MESSAGE_TABLE(
iTxtAddressMasterPathError,
_T("誤りのテキスト住所マスタパス\n"))
MESSAGE_TABLE(
iLayerExcept,
_T("チェック対象:%s対応以外\n"))
MESSAGE_TABLE(
iFinishCreateInstance,
_T("初期化完了\n"))
MESSAGE_TABLE(
iCheckProgress,
_T("%u : チェック中\n"))
MESSAGE_TABLE(
iStartMesh,
_T("%u : チェック開始\n"))
MESSAGE_TABLE(
iStartCheck,
_T("%s : チェック開始\n"))
MESSAGE_TABLE(
iFinishCheck,
_T("%s : チェック完了: %.5lf sec\n"))
MESSAGE_TABLE(
iFinishMesh,
_T("%u : チェック完了\n"))
MESSAGE_TABLE(
iInterruptCheck,
_T("チェックを中断します。\n"))
MESSAGE_TABLE(
iProcessingTime,
_T("処理にかかった時間 : %f sec\n"))
MESSAGE_TABLE(
iCompleteCheck,
_T("チェックが終了しました。\n"))
MESSAGE_TABLE(
iAC_CheckInit,
_T("%s チェック初期化\n"))
MESSAGE_TABLE(
iAC_CheckOverlapStart,
_T("%s  重なるチェック開始\n"))
MESSAGE_TABLE(
iAC_CheckOverlapEnd,
_T("%s  重なるチェック完了\n"))
MESSAGE_TABLE(
iAC_CheckMissingStart,
_T("%s  抜けチェック開始\n"))
MESSAGE_TABLE(
iAC_CheckMissingEnd,
_T("%s  抜けチェック完了\n"))

// error
MESSAGE_TABLE(
eFailLoadSettingFile,
_T("#Error 設定ファイル読み込みに失敗。%s 以下の２つを確認して下さい。\n設定ファイルのパスは正しいですか？\n・設定ファイルが空ではないですか？\n"))
MESSAGE_TABLE(
eFailDoCheck,
_T("#Error 制御クラスチェックに失敗\n"))
MESSAGE_TABLE(
eFailInitCheckControl,
_T("#Error 制御クラス初期化に失敗\n"))
MESSAGE_TABLE(
eFailLoadMeshList,
_T("#Error メッシュリストの読み込みに失敗。%s 以下の２つを確認して下さい。\nメッシュリストのパスは正しいですか？\n・メッシュリストが空ではないですか？\n"))
MESSAGE_TABLE(
eFailLoadMasterList,
_T("#Error マスタリストの読み込みに失敗。%s 以下の２つを確認して下さい。\nマスタのパスは正しいですか？\n・マスタリストが空ではないですか？\n"))
MESSAGE_TABLE(
eFailCreateInstance,
_T("#Error %s : インスタンス生成に失敗した為、チェックを中止します。進捗ログを確認して下さい。\n"))
MESSAGE_TABLE(
eFailGetMeshFeature,
_T("#Error メッシュフィーチャが取得できません : %u\n"))
MESSAGE_TABLE(
eFailGetMeshGeometry,
_T("#Error メッシュジオメトリが取得できません. \n"))
MESSAGE_TABLE(
eFeailGetShape,
_T("#Error メッシュ内フィーチャのジオメトリが取得できません. %d\n"))
MESSAGE_TABLE(
eFailLoadLogFile,
_T("#Error ログファイルの読み込みに失敗\n"))
MESSAGE_TABLE(
eUnknownErrorCode,
_T("FATAL ERROR\t-1\t不明なエラーコード\n"))
MESSAGE_TABLE(
eFailAnalyzeArgument,
_T("#Error 引数解析エラー\n"))
MESSAGE_TABLE(
eFailRunLogInitialize,
_T("#Error 実行時エラーログの初期化に失敗しました.\n"))
MESSAGE_TABLE(
eFailOverlap,
_T("#Error 重なるチェック処理に失敗しました.\n"))
MESSAGE_TABLE(
eFailMissing,
_T("#Error 抜けチェック処理に失敗しました.\n"))
MESSAGE_TABLE(
eFailDifference,
_T("#Error Difference処理に失敗しました.\n"))
MESSAGE_TABLE(
eFailAddressPolyData,
_T("#Error 住所データは存在しない.\n"))
MESSAGE_TABLE(
eFailAddressDBOpen,
_T("#Error 住所ワークスペースオープンできません. パス:%s \n"))
MESSAGE_TABLE(
eFailMeshDBOpen,
_T("#Error メッシュワークスペースオープンできません. パス:%s \n"))
MESSAGE_TABLE(
eFailMapDBOpen,
_T("#Error 地図ワークスペースオープンできません. パス:%s \n"))
MESSAGE_TABLE(
eFailConstructUnion,
_T("#Error ConstructUnion処理に失敗しました.\n"))
MESSAGE_TABLE(
eFaiBaseMeshListSetting,
_T("#Error 引数layerの値がCITY_POLYGON、PLACENAME_REP_POINTのいずれかで指定して、base_mesh_listを指定しない.\n"))
MESSAGE_TABLE(
eFaiCityMeshListSetting,
_T("#Error 引数layerの値がADDR_POLYGONとADDR_POINTのいずれかで指定して、city_mesh_listを指定しない.\n"))
