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
 * @file ErrorCodeBase.h
 * @brief <b>エラーコード表定義ファイル<\b>\n
 * このファイルではエラーコードを定義しています。
 * SiNDYで使用されるエラーコードは全てこのファイルで定義される必要があります。
 * 様々な場面で使用できるように、マクロ形式で定義しています。
 * 種別コードが必要なファイルでERROR_CODEマクロを定義し、
 * その後にこのファイルをインクルードしてください。
 * インクルード後はERROR_CODEマクロはundefして下さい。
 *
 * 定義されるエラーコードの種類は次の通り：
 *
 * ・引数チェック（プログラムロジックエラー）
 * ・単体チェック（オブジェクト単位チェックエラー）
 * ・複合チェック（オブジェクト間の論理チェックエラー）
 * ・その他のエラー
 *
 * 真偽判定をする関数はなるべくこのコードを返すようにして下さい。
 * <H4>例：enumとして使用する場合</H4>
 * @code
 * enum ECode {
 * #define ERROR_CODE(id,label) id,
 * #include "ErrorCodeBase.h"
 * #undef ERROR_CODE
 * };
 * @endcode
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * $Id$
 */
#ifndef SINDY_FOR_ENGLISH
ERROR_CODE(sindyErr_NoErr                               ,エラーなし)
ERROR_CODE(sindyErr_NoImplimentation                    ,未実装)
////////////////////////////////////////////////////////
// レジストリ・設定
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_RegNoSiNDY                          ,レジストリにSiNDY関連の設定がない)
ERROR_CODE(sindyErr_RegNoInstallRoot                    ,レジストリにインストールフォルダが格納されていない)
ERROR_CODE(sindyErr_RegNoInifileName                    ,レジストリにiniファイル名が格納されていない)
ERROR_CODE(sindyErr_IniNoServerName                     ,iniファイルにサーバ名が格納されていない)
ERROR_CODE(sindyErr_RegOpenFailed                       ,レジストリのオープンに失敗)
ERROR_CODE(sindyErr_RegWriteFailed                      ,レジストリの書き込みに失敗)
ERROR_CODE(sindyErr_RegReadFialed                       ,レジストリの読み込みに失敗)
////////////////////////////////////////////////////////
// 引数チェック（プログラムロジックエラー）
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_ArgIsNull                           ,引数がNULL)
ERROR_CODE(sindyErr_ArgLimitOver                        ,引数の範囲が想定外)
ERROR_CODE(sindyErr_ArgIsConsistency                    ,引数同士の整合が合わない)
////////////////////////////////////////////////////////
// メンバチェック（プログラムロジックエラー）
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_ClassMemberMissing                  ,クラスのメンバが足りない（初期化不足）)
////////////////////////////////////////////////////////
// プログラムロジックエラー
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_FunctionFailed                      ,関数呼び出しに失敗)
ERROR_CODE(sindyErr_MemoryAllocationFailed              ,メモリ確保に失敗)
ERROR_CODE(sindyErr_BufferIsNotEnough                   ,バッファ領域が足りない)
ERROR_CODE(sindyErr_ThrowException                      ,例外発生)
ERROR_CODE(sindyErr_InvalidTypeInfo                     ,型情報が一致しない)
ERROR_CODE(sindyErr_AlgorithmFailed                     ,アルゴリズムエラー)
ERROR_CODE(sindyErr_MethodLocked                        ,関数は使用禁止)
ERROR_CODE(sindyErr_ObjectLocked                        ,オブジェクトがロックされている)
////////////////////////////////////////////////////////
// COMエラー（プログラムロジックエラー）
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_COMInterfaceIsNull                  ,インターフェースポインタがNULL)
ERROR_CODE(sindyErr_COMCreateInstanceFailed             ,インスタンス作成に失敗)
ERROR_CODE(sindyErr_COMQueryInterfaceFailed             ,QueryInterfaceに失敗)
ERROR_CODE(sindyErr_COMFunctionFailed                   ,関数呼び出しに失敗)
ERROR_CODE(sindyErr_COMWrappedIntarfaceIsNull           ,ラップしているはずのCOMインターフェースがNULL)
////////////////////////////////////////////////////////
// ArcObjectsエラー（プログラムロジックエラー？）
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_AOConnectDBFailed                   ,データベースへの接続に失敗)
ERROR_CODE(sindyErr_AOStoreFailed                       ,Store()失敗)
ERROR_CODE(sindyErr_AODeleteFailed                      ,Delete()失敗)
ERROR_CODE(sindyErr_AOCreateRowFailed                   ,CreateRow()失敗)
ERROR_CODE(sindyErr_AOCreateFeatureFailed               ,CreateFeature()失敗)
ERROR_CODE(sindyErr_AOGetValueFailed                    ,get_Value()失敗)
ERROR_CODE(sindyErr_AOUnsupportValueType                ,取得したレコード値が、期待している型ではない)
ERROR_CODE(sindyErr_AOPutValueFailed                    ,put_Value()失敗)
ERROR_CODE(sindyErr_AOLayerNotStartEdit                 ,編集開始されたレイヤではありません)
ERROR_CODE(sindyErr_AOLayerNotAllowCreate               ,作成可能なレイヤではありません)
ERROR_CODE(sindyErr_AOLayerNotSelected                  ,レイヤの選択に失敗しました。TOC から１つのレイヤを選択してください。)
ERROR_CODE(sindyErr_AOGetWorkspaceFailed                ,Workspace が取得できませんでした)
ERROR_CODE(sindyErr_AOGetFeatureClassFailed             ,フィーチャクラスの取得に失敗しました。)
ERROR_CODE(sindyErr_AOLayerNotAllowSelect               ,選択できないレイヤです。)
ERROR_CODE(sindyErr_AOGetFieldIndexFailed               ,フィールドインデックスを取得できませんでした)
ERROR_CODE(sindyErr_AOStartEditOperationFailed          ,編集を開始することができませんでした。)
ERROR_CODE(sindyErr_AOFeatureSearchError                ,フィーチャクラスの検索でエラーが発生しました。\nエラーコードは %d です。)
ERROR_CODE(sindyErr_AOConnectEventsHasCookie            ,既にイベント接続済み)
ERROR_CODE(sindyErr_AOConnectEventsFailed               ,イベント接続に失敗)
ERROR_CODE(sindyErr_AODisconnectEventsFailed            ,イベント接続解除失敗)
ERROR_CODE(sindyErr_AOErr                               ,不明なArcObjects系エラー)
////////////////////////////////////////////////////////
// SDE 系エラー
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_SDEConnectFailed                    ,SDE C APIでのSDEへの接続が失敗)
ERROR_CODE(sindyErr_SDEDisconnectFailed                 ,SDE C APIでのSDEからの切断に失敗)
ERROR_CODE(sindyErr_SDEAPICallFailed                    ,SDE C APIのコールに失敗)
ERROR_CODE(sindyErr_SDEInvalidateFieldType              ,SDE C APIで検索できない列タイプ)
ERROR_CODE(sindyErr_SDEQueryFailed                      ,SDE C APIでのクエリーに失敗)
ERROR_CODE(sindyErr_SDEGetStateInfoFailed               ,SDE C APIでのステート情報の取得に失敗)
ERROR_CODE(sindyErr_SDEVersionChangeStateFailed         ,SDE C APIでのバージョンのステート変更に失敗)
////////////////////////////////////////////////////////
// 属性系エラー
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_FieldLengthLimitOver                ,フィールド長をオーバーしている)
ERROR_CODE(sindyErr_FieldRangeLimitOver                 ,フィールド許容範囲をオーバーしている)
ERROR_CODE(sindyErr_FieldDomainLimitOver                ,フィールドドメインに定義されていない値をセットしている)
ERROR_CODE(sindyErr_FieldPrecisionLimitOver             ,フィールド桁数をオーバーしている)
ERROR_CODE(sindyErr_SetNullValueToNotNullField          ,NOT NULLフィールドにNULL値をセットしている)
////////////////////////////////////////////////////////
// ジオメトリ系エラー
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_GeometryNotFound                    ,ジオメトリが存在しない)
ERROR_CODE(sindyErr_GeometryNotSupported                ,サポートされていないジオメトリ)
ERROR_CODE(sindyErr_GeometryPartTooMach                 ,ポイント・ラインジオメトリパート数が二つ以上存在する)
ERROR_CODE(sindyErr_GeometryExteriorTooMach             ,ポリゴンジオメトリのエクステリアが二つ以上存在する)
ERROR_CODE(sindyErr_GeometryNotCached                   ,ジオメトリキャッシュが必要な操作でキャッシュが作成されていない)
ERROR_CODE(sindyErr_GeometryPointNotFound               ,指定された構成点が存在しない)
ERROR_CODE(sindyErr_GeometryFunctionFailed              ,ジオメトリ演算に失敗)
ERROR_CODE(sindyErr_GeometryChangeToNull                ,ジオメトリが無くなる操作は不可)
ERROR_CODE(sindyErr_GeometrySegmentCountIsNotEnough     ,ジオメトリを構成するセグメントの数が足りない)
ERROR_CODE(sindyErr_GeometrySegmentTooShort             ,ジオメトリを構成するセグメントが短すぎる)
ERROR_CODE(sindyErr_GeometrySegmentsAngleOverFlow       ,ジオメトリを構成するセグメント間の角度が範囲外)
ERROR_CODE(sindyErr_GeometryOverlapped                  ,ジオメトリが他のジオメトリと重複している)
ERROR_CODE(sindyErr_GeometryOneselfTouches              ,ジオメトリが自分自身に接触している)
ERROR_CODE(sindyErr_GeometryRooped                      ,ジオメトリがループしている)
ERROR_CODE(sindyErr_GeometryNonSimple                   ,ジオメトリがシンプルではない)
ERROR_CODE(sindyErr_GeometryNonSimpleRingOrientation    ,A polygon is topologically simple、 but its rings may not be oriented correctly (outer rings - cw、 inner rings - ccw).)
ERROR_CODE(sindyErr_GeometryNonSimpleSegmentOrientation ,Individual segments are not consistantly oriented. The 'to' point of seg i should be incident on the 'from' point of seg i+1.)
ERROR_CODE(sindyErr_GeometryNonSimpleShortSegments      ,Some segments are shorter than allowed by the system units of the spatial reference associated with the geometry.)
ERROR_CODE(sindyErr_GeometryNonSimpleSelfIntersections  ,The interior of each part (rings、 planar paths) must not intersect itself or other parts.)
ERROR_CODE(sindyErr_GeometryNonSimpleUnclosedRing       ,The last segment in a ring must have its 'to' point incident on the 'from' point of the first segment.)
ERROR_CODE(sindyErr_GeometryNonSimpleEmptyPart          ,The geometry contains an empty part.)      
////////////////////////////////////////////////////////
// 編集系エラー
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_EditStartFailed                     ,StartEditingに失敗)
ERROR_CODE(sindyErr_EditStopFailed                      ,StopEditingに失敗)
ERROR_CODE(sindyErr_EditMustBeReconcile                 ,StopEditingに失敗（リコンサイルが必要）)
ERROR_CODE(sindyErr_EditOperationStartFailed            ,StartEditOperatonに失敗)
ERROR_CODE(sindyErr_EditOperationStopFailed             ,StopEditOperationに失敗)
ERROR_CODE(sindyErr_EditOperationAbortFailed            ,AbortEditOperationに失敗)
ERROR_CODE(sindyErr_EditReconcileFailed                 ,リコンサイルに失敗)
ERROR_CODE(sindyErr_EditPostFailed                      ,ポストに失敗)
ERROR_CODE(sindyErr_EditConflictDetected                ,編集がコンフリクトしている)
ERROR_CODE(sindyErr_EditCantRecognizedOriginalEdit      ,カスタムポストかローカルリコンサイルか判断できない)
ERROR_CODE(sindyErr_EditCustomPostNoEXTCLSID            ,EXTCLSIDが登録されていないためにカスタムポストできない)
ERROR_CODE(sindyErr_EditParentIsProtected               ,親バージョンがプロテクトされているためにローカルリコンサイルできない)
ERROR_CODE(sindyErr_EditParentIsNotProtected            ,親バージョンがプロテクトされていないためにカスタムポストできない)
ERROR_CODE(sindyErr_EditVersionIsNotChild               ,編集中のバージョン名が子バージョンではない)
ERROR_CODE(sindyErr_EditParentVersionIsChild            ,編集中のバージョンの親バージョン名が子バージョンである)
ERROR_CODE(sindyErr_EditCustomPostFailed                ,カスタムポストに失敗)
ERROR_CODE(sindyErr_EditLocalReconcileFailed            ,ローカルリコンサイルに失敗)
ERROR_CODE(sindyErr_VersionNotFound                     ,バージョンが見つからない)
ERROR_CODE(sindyErr_VersionNotCreated                   ,バージョン作成に失敗)
ERROR_CODE(sindyErr_VersionNotChanged                   ,バージョン変更に失敗)
ERROR_CODE(sindyErr_VersionNotDeleted                   ,バージョン削除に失敗)
ERROR_CODE(sindyErr_EditAlreadyStarted                  ,既に編集が開始されている)
ERROR_CODE(sindyErr_EditNotStarted                      ,編集が開始されていない)
ERROR_CODE(sindyErr_EditWorkspaceNotFound               ,編集中のワークスペースが見つからない)
ERROR_CODE(sindyErr_EditWorkspaceIsNotVersioned         ,編集中のワークスペースはバージョン対応ではない)
ERROR_CODE(sindyErr_FDOReconcileVersionNotAvailable     ,Unable to reconcile: the target version is currently being reconciled against[FDO_E_RECONICLE_VERSION_NOT_AVAILABLE = [-2147217137]].)
ERROR_CODE(sindyErr_FDOVersionBeingEdited               ,Operation not allowed while the version is being edited[FDO_E_VERSION_BEING_EDITED = [-2147217139]].)
ERROR_CODE(sindyErr_FDOVersionNotFound                  ,The version could not be located[FDO_E_VERSION_NOT_FOUND = [-2147217146]].)
ERROR_CODE(sindyErr_FDOVersionIsProtected               ,Operation not allowed because the version is protected[FDO_E_VERSION_IS_PROTECTED = [-2147217141]].)
ERROR_CODE(sindyErr_FDOVersionReconcileLost             ,Post not allowed after undoing a reconcile[FDO_E_VERSION_RECONCILE_LOST = [-2147217136].)
ERROR_CODE(sindyErr_FDOVersionRedefined                 ,The version has been redefined to reference a new database state[FDO_E_VERSION_REDEFINED = [-2147217147].)
ERROR_CODE(sindyErr_RowCreateFailed                     ,フィーチャ（レコード）の作成に失敗)
////////////////////////////////////////////////////////
// 単体チェック（オブジェクト単位チェックエラー）
////////////////////////////////////////////////////////
// 道路系
ERROR_CODE(sindyErr_RoadHasNoFromID                     ,道路リンクにFROMノードIDがない)
ERROR_CODE(sindyErr_RoadHasNoToID                       ,道路リンクにTOノードIDがない)
ERROR_CODE(sindyErr_RoadFromToPointIsEqualNodePoint     ,道路リンクのFROM、TOの座標と、関連しているノードの座標が一致しない)
ERROR_CODE(sindyErr_SegAttrGeometryNotWithinRoad        ,リンク内属性の形状が道路リンクの形状に含まれていない)
ERROR_CODE(sindyErr_GradGeometryNotWithinRoad           ,傾斜の形状が道路リンクの形状に含まれていない)
ERROR_CODE(sindyErr_RoadNodeClassInvalid                ,ノード種別コードがおかしい)
ERROR_CODE(sindyErr_InfTableIsNotMatchLQTable           ,INFのテーブルと対になるLQのテーブルではない)
ERROR_CODE(sindyErr_InfIDInvalid                        ,INFのOIDとLQのINF_IDが一致しない)
ERROR_CODE(sindyErr_NodeOfMeshEdgeNotExistMeshEdge      ,二次メッシュ区画線上（PEC）ノードなのにメッシュ区画上に存在しない)
ERROR_CODE(sindyErr_NormalNodeExistMeshEdge             ,二次メッシュ区画線上（PEC）ノード以外なのにメッシュ区画上に存在している)
ERROR_CODE(sindyErr_RoadMiddlePointExistMeshEdge        ,道路リンクの端点以外が二次メッシュ区画線上に存在している)
ERROR_CODE(sindyErr_RoadSegmentCrossMeshEdge            ,道路リンクが二次メッシュ区画を跨いでいる)
ERROR_CODE(sindyErr_NodeExist4MeshEdge                  ,ノードが二次メッシュ4隅に存在している)
ERROR_CODE(sindyErr_RoadEndPointExist4MeshEdge          ,道路リンクの端点が二次メッシュ4隅に存在している)
ERROR_CODE(sindyErr_NetworkLinkLoop                     ,道路リンクがループしています)

ERROR_CODE(sindyErr_RuleRelationshipContainGeometryNotWithinRoad  ,形状が道路リンクの形状に含まれていない)
ERROR_CODE(sindyErr_RuleRelationshipContainGeometryNotOnWalkNode  ,形状の始終点が歩行者ノード上にありません)

ERROR_CODE(sindyErr_LinkQueueNotRoad                    ,リンク列に道路がないものがあります)
ERROR_CODE(sindyErr_RoadBadLQRF                         ,リンク列存在フラグが正しくありません)
ERROR_CODE(sindyErr_LinkQueueBadDirection               ,リンク列の向きと道路の向きがあっていません)
ERROR_CODE(sindyErr_LinkQueueBadSequence                ,リンク列の順序が正しくありません)
ERROR_CODE(sindyErr_LinkQueueNotLastLinkF               ,リンク列の終端フラグがありません)
ERROR_CODE(sindyErr_LinkQueueLastLinkFs                 ,リンク列の終端フラグが複数あります)

// 注記系
ERROR_CODE(sindyErr_AnnoStrLenNotSame                   ,注記文字列の実際の長さと注記文字数が合わない)
ERROR_CODE(sindyErr_AnnoStrIsNotHalfNumber              ,注記文字列種別が国道番号・主要地方道番号・山岳なのに半角数字以外が使用されている)
ERROR_CODE(sindyErr_AnnoStrFistIsHalfZero               ,注記文字列種別が国道番号・主要地方道番号・山岳の時に半角数字の0が先頭にある)
ERROR_CODE(sindyErr_AnnoStrIsNotWide                    ,注記文字列種別が全角文字列のみの種別なのに指定された全角文字以外のものが使用されている)
ERROR_CODE(sindyErr_AnnoStrMountainInvalid              ,注記文字列種別が山岳の時の文字列がおかしい)
ERROR_CODE(sindyErr_AnnoStrICJCFTMustOver4              ,インターチェンジ、ジャンクション、フェリーターミナルなのに4文字以下になっている)
ERROR_CODE(sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT    ,インターチェンジ、ジャンクション、フェリーターミナルなのに最後の2文字が「ＩＣ」、「ＪＣ」、「ＦＴ」ではない)
ERROR_CODE(sindyErr_AnnoStrJCButLast3IsNotJCT           ,都市地図注記で且つジャンクションなのに最後の3文字が「ＪＣＴ」ではない)
ERROR_CODE(sindyErr_AnnoStrFirstIsWideZero              ,注記文字列種別が全角文字列のみの種別なのに先頭の文字が全角0になっている)
ERROR_CODE(sindyErr_AnnoStrRouteNumberOverFlow          ,注記文字列種別が主要地方道番号・国道番号のときに入力値が番号の範囲を越えている)
ERROR_CODE(sindyErr_AnnoStrInvalidChouon                ,全角長音記号が全角ひらがな・全角カタカナ以外の後にきている)
ERROR_CODE(sindyErr_AnnoStrLenInvalid                   ,注記文字列長が名称1<=名称2<=名称3になっていない)
ERROR_CODE(sindyErr_AnnoStrAngleInvalid                 ,注記文字列の隣り合う3点のなす角が不正)
ERROR_CODE(sindyErr_AnnoCodeInvalid                     ,注記種別がおかしい)
ERROR_CODE(sindyErr_AnnoDispTypeInvalid                 ,注記種別と表示タイプの整合が合わない)
ERROR_CODE(sindyErr_AnnoDispLineIllegalDouble           ,不正な二行注記)
ERROR_CODE(sindyErr_AnnoDispPositionInvalid             ,真位置と表示位置の種別別の位置関係がおかしい)
ERROR_CODE(sindyErr_AnnoDispTargetStrInvalid            ,表示位置のターゲット文字列コードがおかしい)
ERROR_CODE(sindyErr_AnnoDispFontSizeInvalid             ,表示位置のフォントサイズコードがおかしい)
ERROR_CODE(sindyErr_AnnoDispFontSizeInvalidWarn         ,表示位置のフォントサイズコードが10)
ERROR_CODE(sindyErr_AnnoDispFontSizeInvalidKanji        ,表示位置のフォントサイズコードが10の時に漢字を使用している)
ERROR_CODE(sindyErr_AnnoStringNumNotEqualVertexNum      ,表示位置の構成点数と文字数が一致しない)
ERROR_CODE(sindyErr_AnnoDispRelIDInvalid                ,表示位置の関連IDがおかしい)
ERROR_CODE(sindyErr_AnnoDispLineTypeInvalid             ,表示位置のラインタイプがルールに適合しない)
ERROR_CODE(sindyErr_AnnoDispExceptExist                 ,中抜けしてはいけない種別で中抜けしている)
ERROR_CODE(sindyErr_AnnoDispBoundsIsOverlaped           ,注記表示文字列同士が重なっている)
ERROR_CODE(sindyErr_AnnoMarkAndMarkIsOverlaped          ,注記記号同士が重なっている)
ERROR_CODE(sindyErr_AnnoMarkAndDispBoundsIsOverlaped    ,注記記号と、その記号に属さない他の注記文字列が重なっている)
ERROR_CODE(sindyErr_AnnoMarkAndOtherMeshIsOverlaped     ,注記記号と、他のメッシュの注記（記号or文字列）が重なっている)
ERROR_CODE(sindyErr_AnnoDispAndOtherMeshIsOverlaped     ,注記文字列と、他のメッシュの注記（記号or文字列）が重なっている)
ERROR_CODE(sindyErr_AnnoToStringBoundsLengthLimitOver   ,真位置から注記文字列までの距離が想定範囲外)
ERROR_CODE(sindyErr_AnnoNoMarkButStringBoundsOutside    ,文字列のみなのに文字列ポリゴン内に真位置がない)
ERROR_CODE(sindyErr_AnnoFlagButStringBoundsInside       ,旗揚げなのに文字列ポリゴン内に真位置がある)
ERROR_CODE(sindyErr_AnnoDispLineShouldNotExist          ,文字列表示無しなのに対応スケールの表示位置が存在する)
ERROR_CODE(sindyErr_AnnoDispLineShouldExist             ,文字列表示有りなのに対応スケールの表示位置が存在しない)
// ルール系
ERROR_CODE(sindyErr_NoLogin                             ,ログインしていない)
ERROR_CODE(sindyErr_LoginFailed                         ,ログインに失敗した)
ERROR_CODE(sindyErr_LoginCanceled                       ,ログインをキャンセルした)
ERROR_CODE(sindyErr_RuleNotFound                        ,適切な編集ルールが見つからない)
ERROR_CODE(sindyErr_RuleCantCached                      ,ルールのキャッシュを作ることができなかった)
ERROR_CODE(sindyErr_RuleNotAllowedMove                  ,編集ルールで移動が制限されている)
ERROR_CODE(sindyErr_RuleNotAllowedCreate                ,編集ルールで作成が制限されている)
ERROR_CODE(sindyErr_RuleNotAllowedDelete                ,編集ルールで削除が制限されている)
ERROR_CODE(sindyErr_RuleNotAllowedEditAttribute         ,編集ルールで属性編集が制限されている)
ERROR_CODE(sindyErr_RuleNotAllowedEditByLockArea        ,エリアがロックされている)
ERROR_CODE(sindyErr_RuleNotAllowedEditByEditArea        ,エリアが編集不可能である)
ERROR_CODE(sindyErr_AlreadyModified                     ,既に編集されている)
////////////////////////////////////////////////////////
// 関連性チェック（オブジェクト間の論理チェックエラー）
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_RelationIDInvalid                   ,関連IDとOBJECTIDが一致しない)
ERROR_CODE(sindyErr_RelFeatureNotFound                  ,関連フィーチャが見つからなかった)
ERROR_CODE(sindyErr_SearchFailed                        ,検索に失敗した)
ERROR_CODE(sindyErr_BuildingStepMustExistOnBuilding     ,階数は家形上に存在する必要があります。)
ERROR_CODE(sindyErr_NoFeaturesForFilling                ,範囲内に穴埋め対象のフィーチャを発見出来ませんでした)
ERROR_CODE(sindyErr_LinkCirculation                     ,リンクが循環しています！)
ERROR_CODE(sindyErr_LinkBranches                        ,リンクが分岐しています！)
ERROR_CODE(sindyErr_LinkNotContinuance                  ,リンク列が連続していません！)
ERROR_CODE(sindyErr_NoXorFeatures                       ,XOR に該当する図形を作成できませんでした。)
ERROR_CODE(sindyErr_NotAllowTouchMesh                   ,傾斜・リンク内属性は２次メッシュ境界に接することはできません)
ERROR_CODE(sindyErr_RoadNodeAlreadyExist                ,既にノードが存在しています。)
ERROR_CODE(sindyErr_NotAllowIndependentNode             ,単独ノードは作成できません。)
ERROR_CODE(sindyErr_CreateOnlyOnRoadNode                ,道路ノード上以外には作成できません。)
ERROR_CODE(sindyErr_BuildingStepMustExistOnOneBuilding  ,階数は１つの家形上に存在する必要があります。)
ERROR_CODE(sindyErr_PolygonAlreadyExist                 ,指定した場所には、既にポリゴンが存在しています！)
ERROR_CODE(sindyErr_RoadLinkNotSelected                 ,道路リンクが選択されませんでした。)
ERROR_CODE(sindyErr_LinkQueueNeedsTwoRoads              ,リンク列は２つ以上のリンクで構成する必要があります。)
ERROR_CODE(sindyErr_CreateInteriorPolygonFailed         ,ポリゴン作成中にエラーが発生しました。\nAddressBar で現在のアドレスを取得してバグ報告してください。)
ERROR_CODE(sindyErr_NodeClassInvalid                    ,ノード種別の設定に失敗しました。\n以下のことを確認してください。\n\n・２次メッシュ区画ノードに複数の道路を接続できない。\n・２次メッシュ上への単独道路は作成できない)
ERROR_CODE(sindyErr_BuildingOnly                        ,家形レイヤは単独では編集できません。建物階数レイヤがあることを確認してください。)
ERROR_CODE(sindyErr_NoSameNode							,一致するノードが見つからない)

ERROR_CODE(sindyErr_BadModelContain                     ,含む側と含まれる側の形状が一致しません)
ERROR_CODE(sindyErr_BadModelContainContain              ,含まれる側がありません)

////////////////////////////////////////////////////////
// 複合チェック
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_CheckReturnWarning                  ,オブジェクト論理チェックでワーニングを検出)
ERROR_CODE(sindyErr_CheckReturnErr                      ,オブジェクト論理チェックでエラーを検出)
////////////////////////////////////////////////////////
// その他のエラー
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_TableNotFound                       ,テーブルの読み込みに失敗した)
ERROR_CODE(sindyErr_FieldMapNotCreated                  ,フィールド対応表の作成に失敗した)
ERROR_CODE(sindyErr_FieldNotFound                       ,フィールドが存在しない)
ERROR_CODE(sindyErr_UnexpectedError                     ,予想外のエラーが発生しました。)
ERROR_CODE(sindyErr_UserCanceled                        ,キャンセルしました。)
ERROR_CODE(sindyErr_CreateAttributeDlgFailed            ,属性ダイアログの作成に失敗しました。)
ERROR_CODE(sindyErr_NoDataOnLQTable                     ,LQ テーブルにデータがありませんでした。)
ERROR_CODE(sindyErr_NoDataForOutput                     ,出力するデータがありません。)
ERROR_CODE(sindyErr_TooManyDatas                        ,出力するデータ量が多すぎます。\n一度に出力できるデータは1000行までです。)
ERROR_CODE(sindyErr_NoSearchResult                      ,検索結果はありませんでした。)
ERROR_CODE(sindyErr_UpdateLQTableFailed                 ,リンク列テーブル（LQ）の更新に失敗しました。)
ERROR_CODE(sindyErr_UpdateInfTableFailed                ,リンク列テーブル（INFO）の更新に失敗しました。)
ERROR_CODE(sindyErr_DeleteLQFailed                      ,リンク列の削除に失敗しました。)
ERROR_CODE(sindyErr_SameConditionExists                 ,既に追加されている条件です。)
ERROR_CODE(sindyErr_InvalidSearchCondition              ,検索に失敗しました。\n検索条件を確認してください。)
ERROR_CODE(sindyErr_CacheNotReflected                   ,キャッシュを反映することができない)
ERROR_CODE(sindyErr_UnknownTableType                    ,想定外のテーブルタイプ)
ERROR_CODE(sindyErr_FeatureCacheNotFound                ,フィーチャのキャッシュが作成されていない)
ERROR_CODE(sindyErr_CantElasticTableType                ,伸縮可能なテーブルタイプではない)
ERROR_CODE(sindyErr_ObjectAlreadyExist					,オブジェクトが既に存在している)
ERROR_CODE(sindyErr_ObjectNotExist						,オブジェクトが存在しない)
////////////////////////////////////////////////////////
// レイヤ系のエラー
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_NoFeatureLayer                      ,フィーチャレイヤではありません。)
ERROR_CODE(sindyErr_ChukiLayer                          ,注記レイヤです。)
ERROR_CODE(sindyErr_NoVisible                           ,非表示レイヤです。)
ERROR_CODE(sindyErr_NoMovable                           ,移動可能なレイヤではありません。)
ERROR_CODE(sindyErr_NoDeletable                         ,削除可能なレイヤではありません。)
ERROR_CODE(sindyErr_NoAttributeEditable                 ,属性編集可能なレイヤではありません。)
ERROR_CODE(sindyErr_SharedNoMovable                     ,共有移動対象レイヤが移動可能レイヤではありません。)
ERROR_CODE(sindyErr_SharedNoLayer                       ,共有編集対象レイヤ（%s）が読み込まれていません。)
ERROR_CODE(sindyErr_NoVisibleFeature                    ,レイヤ : %s OBJECTID : %d : 非表示レイヤに属しています。)
#else
ERROR_CODE(sindyErr_NoErr                               ,No error)
ERROR_CODE(sindyErr_NoImplimentation                    ,Not implemented)
////////////////////////////////////////////////////////
// レジストリ・設定
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_RegNoSiNDY                          ,There are no SiNDY related settings in the registry.)
ERROR_CODE(sindyErr_RegNoInstallRoot                    ,There is no installation folder in the registry.)
ERROR_CODE(sindyErr_RegNoInifileName                    ,There is no ini file name in the registry.)
ERROR_CODE(sindyErr_IniNoServerName                     ,There is no server name in the ini file.)
ERROR_CODE(sindyErr_RegOpenFailed                       ,Failed to open the registry.)
ERROR_CODE(sindyErr_RegWriteFailed                      ,Failed to write to the registry.)
ERROR_CODE(sindyErr_RegReadFialed                       ,Failed to read the registry.)
////////////////////////////////////////////////////////
// 引数チェック（プログラムロジックエラー）
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_ArgIsNull                           ,The argument is NULL.)
ERROR_CODE(sindyErr_ArgLimitOver                        ,The range of the argument was unexpected.)
ERROR_CODE(sindyErr_ArgIsConsistency                    ,The arguments are inconsistent.)
////////////////////////////////////////////////////////
// メンバチェック（プログラムロジックエラー）
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_ClassMemberMissing                  ,There is not enough class members (initialization insufficient).)
////////////////////////////////////////////////////////
// プログラムロジックエラー
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_FunctionFailed                      ,Failed to call a function.)
ERROR_CODE(sindyErr_MemoryAllocationFailed              ,Failed to acquire memory.)
ERROR_CODE(sindyErr_BufferIsNotEnough                   ,The buffer area is insufficient.)
ERROR_CODE(sindyErr_ThrowException                      ,An exception occurred.)
ERROR_CODE(sindyErr_InvalidTypeInfo                     ,The type information does not match.)
ERROR_CODE(sindyErr_AlgorithmFailed                     ,Algorithm error)
ERROR_CODE(sindyErr_MethodLocked                        ,Use of the function is prohibited.)
ERROR_CODE(sindyErr_ObjectLocked                        ,The object is locked.)
////////////////////////////////////////////////////////
// COMエラー（プログラムロジックエラー）
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_COMInterfaceIsNull                  ,The interface pointer is NULL.)
ERROR_CODE(sindyErr_COMCreateInstanceFailed             ,Failed to create an instance.)
ERROR_CODE(sindyErr_COMQueryInterfaceFailed             ,Failed to invoke QueryInterface.)
ERROR_CODE(sindyErr_COMFunctionFailed                   ,Failed to call a function.)
ERROR_CODE(sindyErr_COMWrappedIntarfaceIsNull           ,The COM interface that is supposed to be wrapped is NULL.)
////////////////////////////////////////////////////////
// ArcObjectsエラー（プログラムロジックエラー？）
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_AOConnectDBFailed                   ,Failed to connect to the database.)
ERROR_CODE(sindyErr_AOStoreFailed                       ,Failed to Store().)
ERROR_CODE(sindyErr_AODeleteFailed                      ,Failed to Delete().)
ERROR_CODE(sindyErr_AOCreateRowFailed                   ,Failed to CreateRow().)
ERROR_CODE(sindyErr_AOCreateFeatureFailed               ,Failed to CreateFeature().)
ERROR_CODE(sindyErr_AOGetValueFailed                    ,Failed to get_Value().)
ERROR_CODE(sindyErr_AOUnsupportValueType                ,The acquired record value is not of the expected type.)
ERROR_CODE(sindyErr_AOPutValueFailed                    ,Failed to put_Value().)
ERROR_CODE(sindyErr_AOLayerNotStartEdit                 ,This is not a layer for which editing has been started.)
ERROR_CODE(sindyErr_AOLayerNotAllowCreate               ,This is not a layer that can be created.)
ERROR_CODE(sindyErr_AOLayerNotSelected                  ,Failed to select the layer. Select one layer from the TOC.)
ERROR_CODE(sindyErr_AOGetWorkspaceFailed                ,The workspace could not be acquired.)
ERROR_CODE(sindyErr_AOGetFeatureClassFailed             ,Failed to acquire the feature class.)
ERROR_CODE(sindyErr_AOLayerNotAllowSelect               ,This layer cannot be selected.)
ERROR_CODE(sindyErr_AOGetFieldIndexFailed               ,The field index could not be acquired.)
ERROR_CODE(sindyErr_AOStartEditOperationFailed          ,Editing could not be started.)
ERROR_CODE(sindyErr_AOFeatureSearchError                ,An error occurred during the feature class search. \nThe error code is %d.)
ERROR_CODE(sindyErr_AOConnectEventsHasCookie            ,The event is already connected.)
ERROR_CODE(sindyErr_AOConnectEventsFailed               ,Failed to connect the event)
ERROR_CODE(sindyErr_AODisconnectEventsFailed            ,Failed to disconnect the event.)
ERROR_CODE(sindyErr_AOErr                               ,Unknown ArcObjects error.)
////////////////////////////////////////////////////////
// SDE 系エラー
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_SDEConnectFailed                    ,Failed to connect to SDE by SDE C API)
ERROR_CODE(sindyErr_SDEDisconnectFailed                 ,Failed to disconnect from SDE by SDE C API)
ERROR_CODE(sindyErr_SDEAPICallFailed                    ,Failed to call the SDE C API)
ERROR_CODE(sindyErr_SDEInvalidateFieldType              ,Column type that cannot be looked up by SDE C API)
ERROR_CODE(sindyErr_SDEQueryFailed                      ,Failed to query by SDE C API)
ERROR_CODE(sindyErr_SDEGetStateInfoFailed               ,Failed to get the state information by SDE C API)
ERROR_CODE(sindyErr_SDEVersionChangeStateFailed         ,Failed to change the state of version by SDE C API)
////////////////////////////////////////////////////////
// 属性系エラー
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_FieldLengthLimitOver                ,The field length has been exceeded.)
ERROR_CODE(sindyErr_FieldRangeLimitOver                 ,The allowable field range has been exceeded.)
ERROR_CODE(sindyErr_FieldDomainLimitOver                ,A value not defined for the field domain has been set.)
ERROR_CODE(sindyErr_FieldPrecisionLimitOver             ,The number of digits for the field has been exceeded.)
ERROR_CODE(sindyErr_SetNullValueToNotNullField          ,A NULL value has been set for a NOT NULL field.)
////////////////////////////////////////////////////////
// ジオメトリ系エラー
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_GeometryNotFound                    ,The geometry does not exist.)
ERROR_CODE(sindyErr_GeometryNotSupported                ,Unsupported geometry)
ERROR_CODE(sindyErr_GeometryPartTooMach                 ,Two or more point/line geometry parts exist.)
ERROR_CODE(sindyErr_GeometryExteriorTooMach             ,Two or more polygon geometry exteriors exist.)
ERROR_CODE(sindyErr_GeometryNotCached                   ,Cache was not created for an operation that required geometry cache.)
ERROR_CODE(sindyErr_GeometryPointNotFound               ,The specified composition point does not exist.)
ERROR_CODE(sindyErr_GeometryFunctionFailed              ,Failed to calculate the geometry.)
ERROR_CODE(sindyErr_GeometryChangeToNull                ,An operation that results in the loss of the geometry is not possible.)
ERROR_CODE(sindyErr_GeometrySegmentCountIsNotEnough     ,There is not enough segments to make up the geometry.)
ERROR_CODE(sindyErr_GeometrySegmentTooShort             ,The segments that make up the geometry are too short.)
ERROR_CODE(sindyErr_GeometrySegmentsAngleOverFlow       ,The angles between the segments that make up the geometry are outside of the allowable range.)
ERROR_CODE(sindyErr_GeometryOverlapped                  ,The geometry duplicates another geometry.)
ERROR_CODE(sindyErr_GeometryOneselfTouches              ,The geometry touches itself.)
ERROR_CODE(sindyErr_GeometryRooped                      ,The geometry is looped.)
ERROR_CODE(sindyErr_GeometryNonSimple                   ,The geometry is not simple.)
ERROR_CODE(sindyErr_GeometryNonSimpleRingOrientation    ,A polygon is topologically simple、 but its rings may not be oriented correctly (outer rings - cw、 inner rings - ccw).)
ERROR_CODE(sindyErr_GeometryNonSimpleSegmentOrientation ,Individual segments are not consistantly oriented. The 'to' point of seg i should be incident on the 'from' point of seg i+1.)
ERROR_CODE(sindyErr_GeometryNonSimpleShortSegments      ,Some segments are shorter than allowed by the system units of the spatial reference associated with the geometry.)
ERROR_CODE(sindyErr_GeometryNonSimpleSelfIntersections  ,The interior of each part (rings、 planar paths) must not intersect itself or other parts.)
ERROR_CODE(sindyErr_GeometryNonSimpleUnclosedRing       ,The last segment in a ring must have its 'to' point incident on the 'from' point of the first segment.)
ERROR_CODE(sindyErr_GeometryNonSimpleEmptyPart          ,The geometry contains an empty part.)      
////////////////////////////////////////////////////////
// 編集系エラー
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_EditStartFailed                     ,Failed to invoke StartEditing.)
ERROR_CODE(sindyErr_EditStopFailed                      ,Failed to invoke StopEditing.)
ERROR_CODE(sindyErr_EditMustBeReconcile                 ,Failed to invoke StopEditing (reconciliation is required).)
ERROR_CODE(sindyErr_EditOperationStartFailed            ,Failed to invoke StartEditOperaton.)
ERROR_CODE(sindyErr_EditOperationStopFailed             ,Failed to invoke StopEditOperation.)
ERROR_CODE(sindyErr_EditOperationAbortFailed            ,Failed to invoke AbortEditOperation.)
ERROR_CODE(sindyErr_EditReconcileFailed                 ,Failed to reconcile)
ERROR_CODE(sindyErr_EditPostFailed                      ,Failed to post)
ERROR_CODE(sindyErr_EditConflictDetected                ,Editing is conflicting)
ERROR_CODE(sindyErr_EditCantRecognizedOriginalEdit      ,It is not possible to determine whether local reconcile or custom post)
ERROR_CODE(sindyErr_EditCustomPostNoEXTCLSID            ,Cannot custom post because EXTCLSID is not registered)
ERROR_CODE(sindyErr_EditParentIsProtected               ,Local reconcile is not possible because parent version is protected)
ERROR_CODE(sindyErr_EditParentIsNotProtected            ,Custom post is not possible because parent version is not protected)
ERROR_CODE(sindyErr_EditVersionIsNotChild               ,The version name under editing is not the child version)
ERROR_CODE(sindyErr_EditParentVersionIsChild            ,The parent version name of the version under editing is child version)
ERROR_CODE(sindyErr_EditCustomPostFailed                ,Failed to custom post)
ERROR_CODE(sindyErr_EditLocalReconcileFailed            ,Failed to local reconcile)
ERROR_CODE(sindyErr_VersionNotFound                     ,Version cannot be found)
ERROR_CODE(sindyErr_VersionNotCreated                   ,Failure to create version)
ERROR_CODE(sindyErr_VersionNotChanged                   ,Failure to modify version)
ERROR_CODE(sindyErr_VersionNotDeleted                   ,Failure to delete version)
ERROR_CODE(sindyErr_EditAlreadyStarted                  ,Editing has already been started.)
ERROR_CODE(sindyErr_EditNotStarted                      ,Editing has not been started.)
ERROR_CODE(sindyErr_EditWorkspaceNotFound               ,The workspace being edited could not be found.)
ERROR_CODE(sindyErr_EditWorkspaceIsNotVersioned         ,The workspace being edited is not a supported version.)
ERROR_CODE(sindyErr_FDOReconcileVersionNotAvailable     ,Unable to reconcile: the target version is currently being reconciled against[FDO_E_RECONICLE_VERSION_NOT_AVAILABLE = [-2147217137]].)
ERROR_CODE(sindyErr_FDOVersionBeingEdited               ,Operation not allowed while the version is being edited[FDO_E_VERSION_BEING_EDITED = [-2147217139]].)
ERROR_CODE(sindyErr_FDOVersionNotFound                  ,The version could not be located[FDO_E_VERSION_NOT_FOUND = [-2147217146]].)
ERROR_CODE(sindyErr_FDOVersionIsProtected               ,Operation not allowed because the version is protected[FDO_E_VERSION_IS_PROTECTED = [-2147217141]].)
ERROR_CODE(sindyErr_FDOVersionReconcileLost             ,Post not allowed after undoing a reconcile[FDO_E_VERSION_RECONCILE_LOST = [-2147217136].)
ERROR_CODE(sindyErr_FDOVersionRedefined                 ,The version has been redefined to reference a new database state[FDO_E_VERSION_REDEFINED = [-2147217147].)
ERROR_CODE(sindyErr_RowCreateFailed                     ,Failed to create the feature (record).)
////////////////////////////////////////////////////////
// 単体チェック（オブジェクト単位チェックエラー）
////////////////////////////////////////////////////////
// 道路系
ERROR_CODE(sindyErr_RoadHasNoFromID                     ,Road link does not have FROM node ID)
ERROR_CODE(sindyErr_RoadHasNoToID                       ,Road link does not have TO node ID)
ERROR_CODE(sindyErr_RoadFromToPointIsEqualNodePoint     ,The coordinates of FROM and TO of road link and those of related nodes do not match)
ERROR_CODE(sindyErr_SegAttrGeometryNotWithinRoad        ,Shape of the road structure (Segment Attr) is not included in the shape of a road link)
ERROR_CODE(sindyErr_GradGeometryNotWithinRoad           ,Shape of the slope (gradient) is not included in the shape of a road link)
ERROR_CODE(sindyErr_RoadNodeClassInvalid                ,Node type code is invalid)
ERROR_CODE(sindyErr_InfTableIsNotMatchLQTable           ,It is not a LQ table paired with INF table)
ERROR_CODE(sindyErr_InfIDInvalid                        ,OID of INF and INF_ID of LQ do not match)
ERROR_CODE(sindyErr_NodeOfMeshEdgeNotExistMeshEdge      ,The node is not on the mesh boundary line even though it is a "node on the secondary mesh boundary line")
ERROR_CODE(sindyErr_NormalNodeExistMeshEdge             ,The node is on the mesh boundary line even though it is not a "node on the secondary mesh boundary line")
ERROR_CODE(sindyErr_RoadMiddlePointExistMeshEdge        ,A point other than the end point of a road link is present on the secondary mesh boundary line.)
ERROR_CODE(sindyErr_RoadSegmentCrossMeshEdge            ,Road link is across the secondary mesh boundary)
ERROR_CODE(sindyErr_NodeExist4MeshEdge                  ,Node is present at the four corners of the secondary mesh)
ERROR_CODE(sindyErr_RoadEndPointExist4MeshEdge          ,End points of a road link is present at the four corners of the secondary mesh)
ERROR_CODE(sindyErr_NetworkLinkLoop                     ,Road link is a loop)

ERROR_CODE(sindyErr_RuleRelationshipContainGeometryNotWithinRoad  ,Shape is not included in the shape of a road link)
ERROR_CODE(sindyErr_RuleRelationshipContainGeometryNotOnWalkNode  ,Start and end points of the shape are not on the pedestrian node)

ERROR_CODE(sindyErr_LinkQueueNotRoad                    ,There is a link queue without corresponding road)
ERROR_CODE(sindyErr_RoadBadLQRF                         ,Link queue presence flag is incorrect)
ERROR_CODE(sindyErr_LinkQueueBadDirection               ,Direction of road network and link queue does not match)
ERROR_CODE(sindyErr_LinkQueueBadSequence                ,Link queue sequence is incorrect)
ERROR_CODE(sindyErr_LinkQueueNotLastLinkF               ,Termination flag of a link queue does not exist)
ERROR_CODE(sindyErr_LinkQueueLastLinkFs                 ,There are several termination flags for the link queue)

// 注記系
ERROR_CODE(sindyErr_AnnoStrLenNotSame                   ,Number of characters does not match the actual length of the label string)
ERROR_CODE(sindyErr_AnnoStrIsNotHalfNumber              ,Although the label string class is "national highway number", "regional highway number", or "mountain", one-byte numeric number is not used)
ERROR_CODE(sindyErr_AnnoStrFistIsHalfZero               ,One-byte numeric number 0 is at the beginning when the label string class is "national highway number", "regional highway number", or "mountain")
ERROR_CODE(sindyErr_AnnoStrIsNotWide                    ,Although the label string class is only supposed to have two-byte character, those other than the specified two-byte characters are used)
ERROR_CODE(sindyErr_AnnoStrMountainInvalid              ,Character string is invalid when label string class is "mountain")
ERROR_CODE(sindyErr_AnnoStrICJCFTMustOver4              ,It is 4 characters or less even though it is an interchange, junction or ferry terminal)
ERROR_CODE(sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT    ,Although it is an interchange, junction or ferry terminal, the last 2 characters are not "IC", "JC", or "FT")
ERROR_CODE(sindyErr_AnnoStrJCButLast3IsNotJCT           ,Although it is a city map and a junction, the last 3 characters are not "JCT" )
ERROR_CODE(sindyErr_AnnoStrFirstIsWideZero              ,Although the label string class is only supposed to have two-byte character,  the first letter is a two-byte 0)
ERROR_CODE(sindyErr_AnnoStrRouteNumberOverFlow          ,Input value exceeds the number range when label string class is "national highway number" or "regional highway number")
ERROR_CODE(sindyErr_AnnoStrInvalidChouon                ,Two-byte macron is present behind the characters other than two-byte Hiragana and Katakana)
ERROR_CODE(sindyErr_AnnoStrLenInvalid                   ,Label string length is not Name1<=Name2<=Name3)
ERROR_CODE(sindyErr_AnnoStrAngleInvalid                 ,The angle formed by three adjacent points of a label string is invalid)
ERROR_CODE(sindyErr_AnnoCodeInvalid                     ,Label class is invalid)
ERROR_CODE(sindyErr_AnnoDispTypeInvalid                 ,Label class and display type do not match)
ERROR_CODE(sindyErr_AnnoDispLineIllegalDouble           ,Invalid two-line label)
ERROR_CODE(sindyErr_AnnoDispPositionInvalid             ,Positional relationship of each type of display position and true position is invalid)
ERROR_CODE(sindyErr_AnnoDispTargetStrInvalid            ,Target string code of the display position is invalid)
ERROR_CODE(sindyErr_AnnoDispFontSizeInvalid             ,Font size code of the display position is invalid)
ERROR_CODE(sindyErr_AnnoDispFontSizeInvalidWarn         ,Font size code of the display position is 10)
ERROR_CODE(sindyErr_AnnoDispFontSizeInvalidKanji        ,Kanji is used when font size code of the display position is 10)
ERROR_CODE(sindyErr_AnnoStringNumNotEqualVertexNum      ,Number of characters does not match the number of configuration points of the display position)
ERROR_CODE(sindyErr_AnnoDispRelIDInvalid                ,Relational ID of the display position is invalid)
ERROR_CODE(sindyErr_AnnoDispLineTypeInvalid             ,Line type of the display position does not match the rules)
ERROR_CODE(sindyErr_AnnoDispExceptExist                 ,A class that must be present in every scale is missing in some scale)
ERROR_CODE(sindyErr_AnnoDispBoundsIsOverlaped           ,Label display strings are overlapping)
ERROR_CODE(sindyErr_AnnoMarkAndMarkIsOverlaped          ,Label symbols are overlapping)
ERROR_CODE(sindyErr_AnnoMarkAndDispBoundsIsOverlaped    ,Label symbol and another label string that does not belong to the symbol are overlapping)
ERROR_CODE(sindyErr_AnnoMarkAndOtherMeshIsOverlaped     ,Label symbol and label (symbol or string) in another mesh are overlapping)
ERROR_CODE(sindyErr_AnnoDispAndOtherMeshIsOverlaped     ,Label string and label (symbol or string) in another mesh are overlapping)
ERROR_CODE(sindyErr_AnnoToStringBoundsLengthLimitOver   ,Distance from the true position to the label string is out of the assumed range)
ERROR_CODE(sindyErr_AnnoNoMarkButStringBoundsOutside    ,Although there is only character string, true position is not present inside of the character string polygon)
ERROR_CODE(sindyErr_AnnoFlagButStringBoundsInside       ,Although it is a balloon label, the true position is present inside of the character string polygon)
ERROR_CODE(sindyErr_AnnoDispLineShouldNotExist          ,Although it is "string no display", the display position of the corresponding scale is present)
ERROR_CODE(sindyErr_AnnoDispLineShouldExist             ,Although it is "string display", the display position of the corresponding scale is not present)
// ルール系
ERROR_CODE(sindyErr_NoLogin                             ,You are not logged in.)
ERROR_CODE(sindyErr_LoginFailed                         ,Failed to log in.)
ERROR_CODE(sindyErr_LoginCanceled                       ,Cancelled login.)
ERROR_CODE(sindyErr_RuleNotFound                        ,Appropriate editing rules could not be found.)
ERROR_CODE(sindyErr_RuleCantCached                      ,Cache for the rules could not be created.)
ERROR_CODE(sindyErr_RuleNotAllowedMove                  ,Movement is limited by the editing rules.)
ERROR_CODE(sindyErr_RuleNotAllowedCreate                ,Creation is limited by the editing rules.)
ERROR_CODE(sindyErr_RuleNotAllowedDelete                ,Deletion is limited by the editing rules.)
ERROR_CODE(sindyErr_RuleNotAllowedEditAttribute         ,Attribute editing is limited by the editing rules.)
ERROR_CODE(sindyErr_RuleNotAllowedEditByLockArea        ,The area is locked.)
ERROR_CODE(sindyErr_RuleNotAllowedEditByEditArea        ,The area cannot be edited.)
ERROR_CODE(sindyErr_AlreadyModified                     ,Already edited.)
////////////////////////////////////////////////////////
// 関連性チェック（オブジェクト間の論理チェックエラー）
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_RelationIDInvalid                   ,The associated ID and OBJECTID do not match.)
ERROR_CODE(sindyErr_RelFeatureNotFound                  ,The associated features could not be found.)
ERROR_CODE(sindyErr_SearchFailed                        ,Failed to search.)
ERROR_CODE(sindyErr_BuildingStepMustExistOnBuilding     ,Building step must exist on a building )
ERROR_CODE(sindyErr_NoFeaturesForFilling                ,A feature with holes target to be filled could not be found within the range.)
ERROR_CODE(sindyErr_LinkCirculation                     ,Link is circulating!)
ERROR_CODE(sindyErr_LinkBranches                        ,Link is branching!)
ERROR_CODE(sindyErr_LinkNotContinuance                  ,Link queue is not continuous!)
ERROR_CODE(sindyErr_NoXorFeatures                       ,The geometry corresponding to XOR could not be created.)
ERROR_CODE(sindyErr_NotAllowTouchMesh                   ,Slope/road structure cannot be in contact with the secondary mesh boundary)
ERROR_CODE(sindyErr_RoadNodeAlreadyExist                ,A node already exists)
ERROR_CODE(sindyErr_NotAllowIndependentNode             ,You cannot create an independent node)
ERROR_CODE(sindyErr_CreateOnlyOnRoadNode                ,You can create it only on the road node)
ERROR_CODE(sindyErr_BuildingStepMustExistOnOneBuilding  ,Building step must exist on a building )
ERROR_CODE(sindyErr_PolygonAlreadyExist                 ,A polygon already exists at the specified location!)
ERROR_CODE(sindyErr_RoadLinkNotSelected                 ,Road link was not selected)
ERROR_CODE(sindyErr_LinkQueueNeedsTwoRoads              ,Link queue must be composed of more than two links.)
ERROR_CODE(sindyErr_CreateInteriorPolygonFailed         ,An error has occurred during the polygon creation. \n Please get the current address and report it as a bug.)
ERROR_CODE(sindyErr_NodeClassInvalid                    ,Failed to set the node class \n Please check the following. \n\n You cannot connect multiple roads to the secondary mesh node. \n\n You cannot create a single road on the secondary mesh.)
ERROR_CODE(sindyErr_BuildingOnly                        ,Building layer cannot be edited independently. Please make sure that there is a building_step layer.)
ERROR_CODE(sindyErr_NoSameNode							,Matching node cannot be found)

ERROR_CODE(sindyErr_BadModelContain                     ,The shapes of the containing side and contained side do not match.)
ERROR_CODE(sindyErr_BadModelContainContain              ,There is no contained side.)

////////////////////////////////////////////////////////
// 複合チェック
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_CheckReturnWarning                  ,A warning was detected during the object logic check.)
ERROR_CODE(sindyErr_CheckReturnErr                      ,An error was detected during the object logic check.)
////////////////////////////////////////////////////////
// その他のエラー
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_TableNotFound                       ,Failed to read the table.)
ERROR_CODE(sindyErr_FieldMapNotCreated                  ,Failed to create the field correspondence table.)
ERROR_CODE(sindyErr_FieldNotFound                       ,The field does not exist.)
ERROR_CODE(sindyErr_UnexpectedError                     ,An unexpected error occurred.)
ERROR_CODE(sindyErr_UserCanceled                        ,The process was cancelled.)
ERROR_CODE(sindyErr_CreateAttributeDlgFailed            ,Failed to create the attribute dialog box.)
ERROR_CODE(sindyErr_NoDataOnLQTable                     ,There was no data in LQ table)
ERROR_CODE(sindyErr_NoDataForOutput                     ,There is no data to output.)
ERROR_CODE(sindyErr_TooManyDatas                        ,There is too much data to output. \nUp to 1000 lines of data can be output at one time.)
ERROR_CODE(sindyErr_NoSearchResult                      ,There were no search results.)
ERROR_CODE(sindyErr_UpdateLQTableFailed                 ,Failed to update the link queue table (LQ).)
ERROR_CODE(sindyErr_UpdateInfTableFailed                ,Failed to update the link queue table (INFO).)
ERROR_CODE(sindyErr_DeleteLQFailed                      ,Failed to delete the link queue)
ERROR_CODE(sindyErr_SameConditionExists                 ,This condition has already been added.)
ERROR_CODE(sindyErr_InvalidSearchCondition              ,Failed to perform search. \nCheck the search conditions.)
ERROR_CODE(sindyErr_CacheNotReflected                   ,Cannot apply the data in cache.)
ERROR_CODE(sindyErr_UnknownTableType                    ,Unexpected table type)
ERROR_CODE(sindyErr_FeatureCacheNotFound                ,The feature cache has not been created.)
ERROR_CODE(sindyErr_CantElasticTableType                ,Table type that cannot be expanded or contracted.)
ERROR_CODE(sindyErr_ObjectAlreadyExist					,The object already exists.)
ERROR_CODE(sindyErr_ObjectNotExist						,The object does not exist.)
////////////////////////////////////////////////////////
// レイヤ系のエラー
////////////////////////////////////////////////////////
ERROR_CODE(sindyErr_NoFeatureLayer                      ,This is not a feature layer.)
ERROR_CODE(sindyErr_ChukiLayer                          ,This is Annotation layer.)
ERROR_CODE(sindyErr_NoVisible                           ,This is an undisplayed layer.)
ERROR_CODE(sindyErr_NoMovable                           ,This is not a layer that can be moved.)
ERROR_CODE(sindyErr_NoDeletable                         ,This is not a layer that can be deleted.)
ERROR_CODE(sindyErr_NoAttributeEditable                 ,This is not a layer for which the attributes can be edited.)
ERROR_CODE(sindyErr_SharedNoMovable                     ,The shared movement target layer is not a layer that can be moved.)
ERROR_CODE(sindyErr_SharedNoLayer                       ,The shared edit target layer (%s) has not been read.)
ERROR_CODE(sindyErr_NoVisibleFeature                    ,Layer: %s OBJECTID: %d: Belongs to an undisplayed layer.)
#endif // ifndef SINDY_FOR_ENGLISH
