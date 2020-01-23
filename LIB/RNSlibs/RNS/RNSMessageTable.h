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
 * @brief メッセージの種類（enum）、日本語メッセージ、英語メッセージの対応表
 * @note RNSMessage.hのgetMsg()を記述する際、日本語・英語の対応を分かりやすくする為に作成。
 *       用例はRNSMessage.h,RNSMessage.cppを参照の事。
 *       参考：trunk/SiNDY-e/LQEditTool/define_linkqueue.h
 */
//RNSSindy
MESSAGE_TABLE(
	iConfigFile,
	"設定ファイル",
	"Config File"
)
MESSAGE_TABLE(
	eConfigFileNotFound,
	"指定された設定ファイルの存在を確認できない",
	"Specified config file is not found."
)
MESSAGE_TABLE(
	eConfigFileLoadFailure,
	"設定ファイル読み込みに失敗",
	"Failed to load the Config File."
)
MESSAGE_TABLE(
	iConflictResolutionDeleteAfterUpdate,
	"コンフリクト解消(更新後削除)",
	"Conflict resolution (Delete after update)"
)
MESSAGE_TABLE(
	iConflictResolutionUpdateAfterDelete,
	"コンフリクト解消(削除後更新)",
	"Conflict resolution (Update after delete)"
)
MESSAGE_TABLE(
	iConflictResolutionDipliUpdate,
	"コンフリクト解消(更新の重複)",
	"Conflict resolution (Duplication of update)"
)
MESSAGE_TABLE(
	iConflictResolution,
	"コンフリクト解消",
	"Conflict resolution"
)
MESSAGE_TABLE(
	eStopEditOrAbortEditFailure,
	"StopEditOperation() または AbortEditoperation() に失敗",
	"Failed to StopEditOperation() or AbortEditoperation()"
)
MESSAGE_TABLE(
	eConflictDetected,
	"コンフリクトを検出",
	"Conflict is detected"
)
MESSAGE_TABLE(
	eReconcileFailure,
	"リコンサイルに失敗",
	"Failed to reconcile"
)
MESSAGE_TABLE(
	eStopEditFailure,
	"StopEditing() に失敗",
	"Failed to StopEditing()"
)
MESSAGE_TABLE(
	eStrangeReasonFailure,
	"未知の理由で失敗",
	"A strange reason goes wrong"
)
MESSAGE_TABLE(
	eNotConnected,
	"%sが%sに接続していない",
	"%s has not connected %s."
)
MESSAGE_TABLE(
	eColumnEmpty,
	"列情報が空",
	"Column information is empty"
)
MESSAGE_TABLE(
	eRegionEmpty,
	"リージョンが空",
	"Region is empty"
)
MESSAGE_TABLE(
	eCannotGetCursor,
	"カーソルを取得できない",
	"Cannot get the cursor"
)
MESSAGE_TABLE(
	eDoesNotSort,
	"ソートされていない",
	"It does not sort"
)
//RNSApp
MESSAGE_TABLE(
	iSuccess,
	" 成功",
	" success"
)
MESSAGE_TABLE(
	eFailure,
	" 失敗",
	" failure"
)
MESSAGE_TABLE(
	eOptionError1,
	"オプション -",
	"Option -"
)
MESSAGE_TABLE(
	eOptionError2,
	"は正しくありません",
	"is not right."
)
MESSAGE_TABLE(
	eUnreglatedSettingItemName,
	"規定外の設定項目名:",
	"The unregulated setting item name:"
)
