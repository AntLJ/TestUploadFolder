/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved. 
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF 
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
 * AND NONINFRINGEMENT. 
 * 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY 
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING 
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/


MESSAGE_TABLE(
	iStartTime,
	_T("開始時刻")
)

MESSAGE_TABLE(
	iEndTime,
	_T("終了時刻")
)

MESSAGE_TABLE(
	iExecTime,
	_T("処理時間\t%ld:%02ld:%02ld")
)

MESSAGE_TABLE(
	iWriteSettingInfo,
	_T("設定情報")
)

MESSAGE_TABLE(
	iWriteSource,
	_T("Source:\t\t\t%s")
)

MESSAGE_TABLE(
	iWriteSourceTable,
	_T("SourceTable:\t\t%s")
)

MESSAGE_TABLE(
	iWriteSourceField,
	_T("SourceField:\t\t%s")
)

MESSAGE_TABLE(
	iWriteSourceWhere,
	_T("SourceWhere:\t\t%s")
)

MESSAGE_TABLE(
	iWriteTarget,
	_T("Target:\t\t\t%s")
)

MESSAGE_TABLE(
	iWriteTargetTable,
	_T("TargetTable:\t\t%s")
)

MESSAGE_TABLE(
	iWriteTargetField,
	_T("TargetField:\t\t%s")
)

MESSAGE_TABLE(
	iWriteTargetWhere,
	_T("TargetWhere:\t\t%s")
)

MESSAGE_TABLE(
	iWriteMesh,
	_T("Mesh:\t\t\t%s")
)

MESSAGE_TABLE(
	iWriteMeshTable,
	_T("MeshTable:\t\t%s")
)

MESSAGE_TABLE(
	iWriteMeshListPath,
	_T("MeshListPath:\t\t%s")
)

MESSAGE_TABLE(
	iWriteExeMode,
	_T("ExeMode:\t\t%s")
)

MESSAGE_TABLE(
	iWriteAttrListPath,
	_T("AttrListPath:\t\t%s")
)

MESSAGE_TABLE(
	iWriteDatumFlag,
	_T("DatumFlag:\t\t%d")
)

MESSAGE_TABLE(
	iWriteFieldFlag,
	_T("FieldFlag:\t\t%d")
)

MESSAGE_TABLE(
	iWriteOverlay,
	_T("Overlay:\t\t%f")
)

MESSAGE_TABLE(
	iWriteMagnification,
	_T("Magnification:\t\t%f")
)

MESSAGE_TABLE(
	iWriteReducation,
	_T( "Reduction:\t\t%f" )
)

MESSAGE_TABLE(
	iWriteBuffer,
	_T("Buffer:\t\t\t%f")
)

MESSAGE_TABLE(
	iCannotOpen,
	_T("%sが開けません")
)

MESSAGE_TABLE(
	iReadFailed,
	_T("%sの読み込みが失敗")
)

MESSAGE_TABLE(
	iRequiredSetting,
	_T("設定ファイルの%sが未設定")
)

MESSAGE_TABLE(
	iAbnormalOverlay,
	_T("重畳率の設定値が0～100%%以外\t設定値:%lf")
)

MESSAGE_TABLE(
	iAbnormalMagnification,
	_T("拡大率の設定値が100%%未満\t設定値:%lf")
)

MESSAGE_TABLE(
	iAbnormalReduction,
	_T( "縮小率の設定値が0～100%%以外\t設定値:%lf" )
)

MESSAGE_TABLE(
	iAbnormalBuffer,
	_T("バッファーの設定値が0未満\t設定値:%lf")
)

MESSAGE_TABLE(
	iEmptyFile,
	_T("%sの内容が空白")
)

MESSAGE_TABLE(
	iNotConnectDB,
	_T("%sへ接続できません")
)

MESSAGE_TABLE(
	iErrorCreateVersion,
	_T("バージョン作成(%s)に失敗")
)

MESSAGE_TABLE(
	iSuccessCreateVersion,
	_T("バージョン作成成功")
)

MESSAGE_TABLE(
	iErrorEditWorkspace,
	_T("編集ワークスペースがありません")
)

MESSAGE_TABLE(
	iChildVerComment,
	_T("CopyOverlapFeatureAttr(重複フィーチャー更新ツール)により作成された子バージョン")
)

MESSAGE_TABLE(
	iNotOpenFC,
	_T("フィーチャクラス取得(%s)に失敗")
)

MESSAGE_TABLE(
	iNotGetIndex,
	_T("フィールドのインデックス取得(%s)に失敗")
)

MESSAGE_TABLE(
	iNotSameSpRef,
	_T("比較元と比較先の測地系が一致していない")
)

MESSAGE_TABLE(
	iNotSameField,
	_T("比較元と比較先のフィールド型が一致していない")
)

MESSAGE_TABLE(
	iErrorMeshSearch,
	_T("メッシュの検索に失敗")
)

MESSAGE_TABLE(
	iNotExistMeshCode,
	_T("存在しないメッシュコードが指定された\tメッシュコード：%u")
)

MESSAGE_TABLE(
	iEmptyMeshGeo,
	_T("メッシュリストのメッシュが存在しない")
)

MESSAGE_TABLE(
	iSuccessStartEditing,
	_T("編集開始成功")
)

MESSAGE_TABLE(
	iErrorStartEditing,
	_T("編集開始に失敗")
)

MESSAGE_TABLE(
	iErrorStartEditOperation,
	_T("オペレーション開始に失敗")
)

MESSAGE_TABLE(
	iSuccessStopEditing,
	_T("編集終了成功")
)

MESSAGE_TABLE(
	iErrorStopEditing,
	_T("編集終了に失敗\t%s")
)

MESSAGE_TABLE(
	iErrorStopEditOperation,
	_T("オペレーション終了に失敗")
)

MESSAGE_TABLE(
	iErrorConflicts,
	_T("コンフリクトしたため、編集をキャンセル")
)

MESSAGE_TABLE(
	iErrorReconcile,
	_T("リコンサイルに失敗\tエラーコード：%s")
)

MESSAGE_TABLE(
	iErrorAbortEditing,
	_T("編集破棄に失敗")
)

MESSAGE_TABLE(
	iStartMesh,
	_T("処理開始\tメッシュコード：%u")
)

MESSAGE_TABLE(
	iErrorSourceSearch,
	_T("比較元の検索に失敗")
)

MESSAGE_TABLE(
	iNotExistSource,
	_T("比較元が存在しない")
)

MESSAGE_TABLE(
	iErrorTargetSearch,
	_T("比較先の検索に失敗")
)

MESSAGE_TABLE(
	iNotExistTarget,
	_T("比較先が存在しない")
)

MESSAGE_TABLE(
	iLevelError,
	_T("Error")
)

MESSAGE_TABLE(
	iLevelWarning,
	_T("Warning")
)

MESSAGE_TABLE(
	iLevelInfo,
	_T("Info")
)

MESSAGE_TABLE(
	iErrOverBuffer,
	_T("比較先のオブジェクトが比較元の検索範囲をはみ出したため、スキップ")
)

MESSAGE_TABLE(
	iErrSrcNotExist,
	_T("重なっている比較元が存在しないため、スキップ")
)

MESSAGE_TABLE(
	iErrOverlaySrcNotExist,
	_T("重畳率の閾値を満たす比較元が存在しないため、スキップ")
)

MESSAGE_TABLE(
	iErrScaleSrcNotExist,
	_T("拡縮率の閾値を満たす比較元が存在しないため、スキップ")
)

MESSAGE_TABLE(
	iErrBothSrcNotExist,
	_T("重畳率と拡縮率を両方とも満たす比較元が存在しないため、スキップ")
)

MESSAGE_TABLE(
	iErrMulitSrcSkipMode,
	_T("最も重畳率が高いフィーチャが2つ以上あり、スキップモードのため、スキップ")
)

MESSAGE_TABLE(
	iErrMulitSrcAttrNoMatch,
	_T("最も重畳率が高いフィーチャが2つ以上あり、一致判定モードで比較元同士の属性が一致していないため、スキップ")
)

MESSAGE_TABLE(
	iErrAttrNotInAttrList,
	_T("フィーチャーの属性値が変換表に存在していないため、スキップ")
)

MESSAGE_TABLE(
	iErrNotSameField,
	_T("比較先と比較元のフィールドの型が一致していない")
)

MESSAGE_TABLE(
	iErrNotSameSpRef,
	_T("比較先と比較元の測地系が一致していない")
)

MESSAGE_TABLE(
	iErrPutValueFailed,
	_T("比較先のフィールド値への書き込みが失敗したため、スキップ")
)

MESSAGE_TABLE(
	iErrSuccessUpdate,
	_T("正常更新")
)

MESSAGE_TABLE(
	iErrUpdateFailed,
	_T("属性の更新に失敗")
)

MESSAGE_TABLE(
	iErrFailureOverlay,
	_T("重畳率が閾値を満たさないため、スキップ")
)

MESSAGE_TABLE(
	iErrFailureScale,
	_T("拡縮率が閾値を満たさないため、スキップ")
)

MESSAGE_TABLE(
	iErrUnkownError,
	_T("想定外のエラー")
)

MESSAGE_TABLE(
	iErrUnexpectedFieldType,
	_T("想定外のフィールド型")
)

MESSAGE_TABLE(
	iErrUnexpectedGeometryType,
	_T("想定外のジオメトリ型")
)
