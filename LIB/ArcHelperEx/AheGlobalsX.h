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
* @file AheGlobalsX.h
*
* @brief Arc Helper グローバル関数定義ファイル
*
* @note AheGlobalsの関数を利用し、処理を特化した、高機能型関数をまとめている
*/
#if	!defined( __AHEGLOBALSX_H__)
#define	__AHEGLOBALSX_H__	//!< 重複インクルード防止

#include "LoadQueueInfo.h"
#include "TargetFeatures.h"
#include "AheNetwork.h"
#include <sindy/schema.h>
#include <sindy/schema_draft/road_draft.h>

class CErrObjContainer;
/////////////////////////////////////////////////////////////////////////////
//
// 道路チェック関連（道路属性入力画面に出てくるもの含む）
//
/////////////////////////////////////////////////////////////////////////////
/**
 * ポイントがラインセレクションの端点の一つであるかどうかの判定
 *
 *   ArcObjects のリレーションシップコンポーネントを使用するので誤差による
 *   誤判定がありません
 *
 * @param ipPointGeom        [in]           ポイントジオメトリ
 * @param pRoadSelection     [in]           ラインフィーチャセレクション
 *
 * @retval TURE 端点
 * @retval FALSE 端点以外
 */
BOOL AheIsEndPoints( IGeometryPtr ipPointGeom, CTargetItems& cRoadSelection );
//////////////////////////////////////////////////////////////////////
// フィーチャ選択に関する関数
//////////////////////////////////////////////////////////////////////
/**
 * 指定のエリア内に存在するフィーチャをコレクションに格納
 *
 *   特定のフィーチャクラスのみ検索する場合には ipFeatureClass を指定します。
 *
 * @param ipGeom            [in]			選択範囲
 * @param cTargetItems		[out]			コレクション
 * @param spatialRel        [in,optional]	選択方法
 * @param lpcszSubFields	[in,optional]	取得するフィールド名
 * @param lpcszWhereClause	[in,optional]	WHERE 文
 * @param bVisibleOnly		[in,optional]	見えているレイヤのみにするかどうか（デフォルト：FALSE）
 * @param bEditableOnly		[in,optional]	編集可能レイヤのみにするかどうか（デフォルト：FALSE）
 *
 * @return 検索できたフィーチャの数
 */
LONG AheSelectByShape( IMapPtr piMap, IGeometryPtr ipGeom, CTargetItems& cTargetItems, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL, BOOL bVisibleOnly = FALSE, BOOL bEditableOnly = FALSE );

/**
 * 指定のエリア内に存在するフィーチャをコレクションに格納
 *
 *   特定のレイヤのみ検索する場合には レイヤ を指定します。
 *
 * @param ipGeometry        [in]           選択範囲
 * @param cTargetItems		[out]          コレクション
 * @param ipLayer			[in]           レイヤ
 * @param spatialRel        [in,optional]  選択方法
 * @param lpcszSubFields	[in,optional]  取得するフィールド名
 * @param lpcszWhereClause	[in,optional]  WHERE 文
 *
 * @return 選択されたアイテムの個数
 */
LONG AheSelectByShapeFromLayer( IGeometryPtr ipGeom, CTargetItems& cTargetItems, ILayerPtr ipLayer, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL );

/**
 * 指定のエリア内に存在するフィーチャをコレクションに格納
 *
 * @param ipGeometry        [in]           選択範囲
 * @param cTargetItems		[out]          コレクション
 * @param ipFeatureClass	[in]           フィーチャクラス
 * @param spatialRel        [in,optional]  選択方法
 * @param lpcszSubFields	[in,optional]  取得するフィールド名
 * @param lpcszWhereClause	[in,optional]  WHERE 文
 *
 * @return 選択されたアイテムの個数
 */
LONG AheSelectByShapeFromFeatureClass( IGeometryPtr ipGeometry, CTargetItems& cTargetItems, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL );

/**
 * 指定のエリア内に存在するフィーチャをコレクションに格納
 *
 * @param ipGeom            [in]			選択範囲
 * @param cTargetItems		[out]			コレクション
 * @param lpcszLayerNames   [in]			レイヤ名称[名称ごと、必ず後ろと前に|記載]
 * @param spatialRel		[in,optional]	選択方法
 * @param lpcszSubFields	[in,optional]	取得するフィールド名
 * @param lpcszWhereClause	[in,optional]	WHERE 文
 * @param bEditableOnly		[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
 *
 * @return 選択されたアイテムの個数
 */
LONG AheSelectByShapeFromLayers( IMapPtr piMap, IGeometryPtr ipGeom, CTargetItems& cTargetItems, LPCTSTR lpcszLayerNames, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL, BOOL bEditableOnly = FALSE );

//////////////////////////////////////////////////////////////////////
// 形状チェックに関する関数
//////////////////////////////////////////////////////////////////////
/**
 * @brief 保存してもよいフィーチャかどうかの判定
 *
 * @param ipFeature			[in]			チェック対象フィーチャ
 * @param errs				[out]			チェック結果
 * @param bCheckRoad		[in,optional]	道路リンクとノードの接続性をチェックするかどうか（デフォルト：TRUE）
 * @param bIsCheckAngle		[in,optional]	セグメント接続角度をチェックするかどうか（デフォルト：TRUE）
 * @param ipRule			[in,optional]	ルールオブジェクト（デフォルト：NULL）
 *
 * @retval TRUE		正常
 * @retval FALSE	異常
 */
BOOL AheIsSafeFeature( IFeaturePtr ipFeature, CErrObjContainer& errs, BOOL bCheckRoad = TRUE, BOOL bIsCheckAngle = TRUE, ISiNDYRule3Ptr ipRule = NULL );

/**
 * @brief 保存してもよい道路リンクフィーチャかどうかの判定
 *
 * @param ipRoadFeature		[in]			チェック対象道路フィーチャ
 * @param errs				[out]			チェック結果
 *
 * @retval TRUE		正常
 * @retval FALSE	異常
 */
BOOL AheIsSafeRoadForms( IFeaturePtr ipRoadFeature, CErrObjContainer& errs );

/**
 * @brief 同じノードを両端にもつリンクが複数存在チェック
 *
 * @param ipFeatureNode1	[in]			ノードフィーチャ
 * @param ipFeatureNode2	[in]			ノードフィーチャ
 * @param ipRoadFeature		[in]			道路リンクフィーチャ
 * @param errs				[out]			チェック結果
 *
 * @retval TRUE		正常
 * @retval FALSE	異常
 */
BOOL AheIsSafeRoadLoop( IFeaturePtr ipFeatureNode1, IFeaturePtr ipFeatureNode2, IFeaturePtr ipRoadFeature, CErrObjContainer& errs );

/**
 * @brief 正常な形状かどうかチェック
 *
 * @note 以下のチェックを行います。
 * @note 
 * @note 1. ポイントは常にスルー
 * @note 2. ポリゴン・ポリライン以外は NG
 * @note 3. ポリラインのループチェック
 * @note 4. ポリゴンは ExteriorRing が一つであることのチェック
 * @note 5. ねじれチェック（ここから各パートに分解してチェック）
 * @note 6. 構成点数のチェック
 * @note 7. パルス・鋭角ターンチェック（ポリゴンは開始・終了点も考慮）
 * @note 8. 稲妻形状チェック（ポリゴンは開始・終了点も考慮）
 *
 * @param ipGeom			[in]			チェック対象形状
 * @param lpcszOwnerName	[in]			フィーチャクラスオーナー名
 * @param lpcszClassName	[in]			フィーチャクラス名
 * @param lOID				[in]			OBJECTID
 * @param errs				[out]			チェック結果
 * @param bIsCheckAngle		[in,optional]	セグメント角度をチェックするかどうか（デフォルト・TRUE）
 * @param ipRule			[in,optional]	ルールオブジェクト
 * @param bDoSimplify		[in,optional]	終了時に形状にSimplifyをかけるかどうか（デフォルト：TRUE）
 * @param bCheckCross	[in,optional]	ポリラインの自己交差をチェックするかどうか（デフォルト：FALSE）
 *
 * @retval TRUE		正常
 * @retval FALSE	異常
 */
BOOL AheIsSafeGeometry( IGeometryPtr ipGeom, LPCTSTR lpcszOnwerName, LPCTSTR lpcszClassName, LONG lOID, sindyeGeometryCheck* check, CString& strErrMessage, BOOL bIsCheckAngle = TRUE, ISiNDYRule3Ptr ipRule = NULL, BOOL bDoSimplify = TRUE, BOOL bCheckCross=FALSE );
BOOL AheIsSafeGeometry( IGeometryPtr ipGeom, LPCTSTR lpcszOwnerName, LPCTSTR lpcszClassName, LONG lOID, CErrObjContainer& errs, BOOL bIsCheckAngle = TRUE, ISiNDYRule3Ptr ipRule = NULL, BOOL bDoSimplify = TRUE, BOOL bCheckCross=FALSE );

/**
 * @brief 道路フィーチャが接続しているノードフィーチャ一覧を取得
 *
 * @param ipFeature			[in]			道路フィーチャ
 * @param cTargetItems		[out,retval]	ノードフィーチャ一覧
 *
 * @retval LONG		取得できたノードの数
 */
LONG AheGetConnectedNode( IFeaturePtr ipFeature, CTargetItems& cTargetItems);

/**
 * @brief 渡されたフィーチャ（ノード限定）に接続されている道路をコレクションに入れて返す
 *
 * @param ipFeature				[in]			道路ノードの IFeature のインスタンス
 * @param cTargetItems			[in]			接続されている道路リンクのコレクション
 *
 * @return 見つかった道路リンクの数
 */
LONG AheGetConnectedRoad( IFeaturePtr ipFeature, CTargetItems& cTargetItems);

/**
 * @brief 選択した道路リンクの反対側のリンクを返し、反対側のノードが2次メッシュ区画辺上ノードかどうか
 *
 * @note AheIsRoadCrossMeshでしか使われない
 * @note 2次メッシュを跨いでいる道路リンクを取得する
 * @note ○-----------------○-----------------------------○
 * @note 　　　↑             ↑　　　　　　　↑              ↑
 * @note  　　inSelectRoad  ipSelectNode　　ipConnectedRoad　ipOppNode
 *
 * @param ipSelectRoad		[in]	選択した道路
 * @param ipSelectNode		[in]	選択したノード
 * @param ipConnectedRoad	[out]	接続している道路
 * @param ipOppNode			[out]	反対側のノード
 *
 * @retval TRUE 反対側のノードは2次メッシュ区画辺上ノード
 * @retval FALSE 反対側のノードは2次メッシュ区画辺上ノード以外
 */
BOOL AheGetConnectedRoadAndNode( IFeaturePtr ipSelectRoad, IFeaturePtr ipSelectNode, IFeaturePtr &ipConnectedRoad, IFeaturePtr &ipOppNode );

/**
 * @brief 選択されているフィーチャの中から、一番距離が近いフィーチャとHitPointとlPartIndexを返す
 *
 * @note HitTestを使用する関係上、閾値を設定してやる必要あり
 *		 TestGeometryHit()で頂点、辺に関わらず一番近いものが帰ってくるので
 *       ＊注意＊
 *       現在表示されているレイヤのみ対象です（SelectByShape()は非表示レイヤも対象）
 *
 * @param cTargetItems			[in]			選択されているフィーチャ郡
 * @param ipPoint				[in]			テストする IPoint のインスタンス
 * @param dTorelance			[in]			距離
 * @param ipRetPoint			[out]			見つかった IPoint のインスタンス
 * @param ipRetLayer			[out]			見つかった ILayer のインスタンス
 * @param ipRetFeature			[out]			見つかった IFeature のインスタンス
 * @param lPartIndex			[out]			ヒットした辺のインデックス
 * @param lVertexIndex			[out]			ヒットした頂点のインデックス
 * @param bIsHitVertex			[out]			頂点にヒットしたか？
 *
 * @retval TURE フィーチャが見つかった
 * @retval FALSE フィーチャが見つからなかった
 */
BOOL AheGetClosestVertexPointOrPartIndex( CTargetItems& cTargetItems, IPointPtr ipPoint, DOUBLE dTorelance, IPoint** ipRetPoint, ILayer** ipRetLayer, IFeature** ipRetFeature, LONG* lPartIndex, LONG*lVertexIndex, BOOL* bIsHitVertex);


/**
 * @brief 与えられたセレクションの中から一番近い頂点（IPoint）を返す
 *
 * @param cTargetItems			[in]			選択されているフィーチャ郡
 * @param ipPoint				[in]			テストする IPoint のインスタンス
 * @param dTorelance			[in]			距離
 * @param ipRetPoint			[out]			一番近い IPoint のインスタンス
 *
 * @retval TURE 頂点にヒット
 * @retval FALSE 頂点以外にヒット
 */
BOOL AheGetNearestVertexInSelection( CTargetItems& cTargetItems, IPointPtr ipPoint, IPoint** ipRetPoint);

/**
* @brief 渡されたフィーチャの構成点上に別のフィーチャがいくつあるか調べる
*
* @note 渡されたフィーチャの構成点上に別のフィーチャがいくつあるかを
* @note 調べます。このとき、渡されたフィーチャ自身とは別に一つだけカ
* @note ウントから除外するフィーチャを渡すことが出来ます。
* @note また、第５引数で TRUE を渡すと終端ノードがあった場合に第６引
* @note 数に終端ノードフィーチャが返ります。
*
* @param piMap			 [in]	IMap
* @param dTol			 [in]	フィーチャ選択幅閾値
* @param ipFeature		 [in]	構成点をチェックするフィーチャ
* @param ipOwnFeature	 [in]	除外フィーチャ
* @param bDeleteEndNode	 [in]	終端ノードをチェックするかどうか
* @param ipEndNode		 [out]	終端ノードがあった場合に返す
* @param lpcszLayerNames [in]   レイヤ名称[名称ごと、必ず後ろと前に|記載]
*
* @return 構成点上に存在するフィーチャの数
*/
LONG AheCheckVertexPoint( IMapPtr piMap, double dTol, IFeaturePtr ipFeature, IFeaturePtr ipOwnFeature, BOOL bDeleteEndNode, IFeature** ipEndNode, LPCTSTR lpcszLayerNames);

/**
 * @brief	指定のフィーチャがメッシュ境界と交差している点列取得
 *
 * @param piMap						[in]		IMap のインスタンス
 * @param piLineFeature				[in]		メッシュ境界線
 * @param ppiPointCollection		[out]		交差した点列を含むコレクション
 *
 * @retval TRUE 交差する点列あり
 * @retval FALSE 交差する点列なし
 */
BOOL AheGetPointsCrossedMesh( IMapPtr piMap, IFeaturePtr piLineFeature, IPointCollection** ppiPointCollection);


/////////////////////////////////////////////////////////////////////////////
// ノードに関する関数
/////////////////////////////////////////////////////////////////////////////
/**
 * ノードの種別を再設定する
 *
 * @param ipNodeFeature		[in]			ノードフィーチャ
 * @param cRoadItems		[in]			ノードフィーチャの種別設定に必要な道路リンクが含まれるフィーチャ群
 * @param bIsChanged			[out,optional]	変更されたかどうか
 * @param bSetModifyData	[in,optional]	属性変更履歴をつけるかどうか（デフォルト：FALSE）
 *
 * @retval TRUE ノード種別再設定成功
 * @retval FALSE 種別が決定できなかった
 */
BOOL AheResetNodeAttribute( IFeaturePtr ipNodeFeature, CTargetItems& cRoadItems, BOOL* bIsChanged = NULL, BOOL bSetModifyData = FALSE );

/**
 * 正しいと思われる道路ノード種別を返す
 *
 * @note cFeatureItems には、ipNodeFeature が接続するすべの道路リンクが含まれていることが前提です。
 * @note この前提条件を満たしていない場合、返り値の道路ノード種別が正しく設定できない場合があります。
 *
 * @param ipNodeFeature		[in]			道路ノード種別変更前のノード
 * @param cRoadItems		[in]			ipNodeFeature に接続する道路リンクを全て含むフィーチャのリスト
 * @param pbHasNodeAttr		[out]			道路ノードに接続する道路リンクが2本以下の場合にノードの交差点名称及び信号機フラグがついているかどうか
 *
 * @return sindyeNodeClass
 */
sindyeNodeClass AheGetModifyRoadNodeKind( IFeaturePtr ipNodeFeature, CTargetItems& cFeatureItems, BOOL* pbHasNodeAttr );

/**
 * 正しいと思われる歩行者ノード種別を返す
 *
 * @note cFeatureItems には、ipNodeFeature が接続するすべの歩行者リンクが含まれていることが前提です。
 * @note この前提条件を満たしていない場合、返り値の歩行者ノード種別が正しく設定できない場合があります。
 *
 * @param ipNodeFeature		[in]			歩行者ノード種別変更前のノード
 * @param cRoadItems		[in]			ipNodeFeature に接続する道路リンクを全て含むフィーチャのリスト
 *
 * @return schemaで定義されたコード値
 */
sindy::schema::walk_node::node_class::ECode AheGetModifyWalkNodeKind( IFeaturePtr ipNodeFeature, CTargetItems& cFeatureItems );

/**
 * 正しいと思われる地下鉄ノード種別を返す
 *
 * @note cFeatureItems には、ipNodeFeature が接続するすべての地下鉄リンクが含まれていることが前提です。
 * @note この前提条件を満たしていない場合、返り値の地下鉄ノード種別が正しく設定できない場合があります。
 *
 * @param ipNodeFeature		[in]			地下鉄ノード種別変更前のノード
 * @param cRoadItems		[in]			ipNodeFeature に接続する地下鉄リンクを全て含むフィーチャのリスト
 *
 * @return 地下鉄種別
 */
sindy::schema::sj::subway_node::node_class::ECode AheGetModifySubwayNodeKind( IFeaturePtr ipNodeFeature, CTargetItems& cFeatureItems );

/**
 * 正しいと思われる鉄道ノード種別を返す
 *
 * @note cFeatureItems には、ipNodeFeature が接続するすべての鉄道リンクが含まれていることが前提です。
 * @note この前提条件を満たしていない場合、返り値の鉄道ノード種別が正しく設定できない場合があります。
 *
 * @param ipNodeFeature		[in]			鉄道ノード種別変更前のノード
 * @param cRoadItems		[in]			ipNodeFeature に接続する鉄道リンクを全て含むフィーチャのリスト
 *
 * @return 鉄道ノード種別
 */
sindy::schema::sj::railway_node::node_class::ECode AheGetModifyRailwayNodeKind( IFeaturePtr ipNodeFeature, CTargetItems& cFeatureItems );

/**
 * ノードに属性が入っているかどうかをチェックする
 *
 * @param ipNodeFeature		[in]			ノード
 * @param bCheckAll			[in,optional]	FALSE の場合はチェックを交差点フラグ、交差点名称、交差点かなのみに限定
 *
 * @note 交差点フラグ、交差点名称、交差点かな、高さフラグ、高さデータをチェックします。
 *
 * @retval TRUE 属性あり
 * @retval FALSE 属性なし
 */
BOOL AheIsExistAttrNode( IFeaturePtr ipNodeFeature, BOOL bCheckAll = TRUE );

//////////////////////////////////////////////////////////////////////
// 道路リンクに関する関数
//////////////////////////////////////////////////////////////////////
/**
 * @brief 交差点を立体交差にする
 *
 * @param ipMap			[in]	IMap のインスタンス
 * @param ipScreen		[in]	IScreenDisplay のインスタンス
 * @param cNodeItem		[in]	ノードアイテム
 * @param ipPoint		[in]	ポイント
 * @param nMode			[in]	非交差にするかどうか
 * @param cRetDelItems	[out]	削除道路アイテムリスト
 *
 * @return TRUE  正常終了
 * @return FALSE 異常終了
 */
BOOL AheCrossingToTwoLevelCrossing(IMapPtr ipMap, IScreenDisplayPtr ipScreen, CTargetItem cNodeItem, IPointPtr ipPoint, INT nMode, CTargetItems& cRetDelItems);
BOOL AheCrossingToTwoLevelCrossing2(IMapPtr ipMap, IScreenDisplayPtr ipScreen, CTargetItem cNodeItem, IPointPtr ipPoint, INT nMode, CTargetItems& cRetDelItems);

/**
 * @brief 道路をマージ
 *
 * @param ipMap			[in]	IMap のインスタンス
 * @param ipScreen		[in]	IScreenDisplay のインスタンス
 * @param ipPoint		[in]	ポイント
 * @param cRoadItem1	[in]	道路アイテム１
 * @param cRoadItem2	[in]	道路アイテム２
 * @param bDel			[in]	構成点を削除するかどうか
 *
 * @return TRUE  正常終了
 * @return FALSE 異常終了
 */
BOOL AheSelectMeargeRoad(IMapPtr ipMap, IScreenDisplayPtr ipScreen, CTargetItems cRoadItems, IPointPtr ipPoint, CTargetItem cRoadItem1, CTargetItem cRoadItem2, BOOL bDel);

/**
 * @brief 道路をマージ
 *
 * @param ipMap			[in]	IMap のインスタンス
 * @param ipScreen		[in]	IScreenDisplay のインスタンス
 * @param ipPoint		[in]	ポイント
 * @param cRoadItem1	[in]	道路アイテム１
 * @param cRoadItem2	[in]	道路アイテム２
 * @param bDel			[in]	構成点を削除するかどうか
 *
 * @return TRUE  正常終了
 * @return FALSE 異常終了
 */
BOOL AheMeargeRoad(IMapPtr ipMap, IScreenDisplayPtr ipScreen, IPointPtr ipPoint, CTargetItem cRoadItem1, CTargetItem cRoadItem2, INT& nRetType, BOOL bDel);

/**
 * @brief 道路構成点削除
 *
 * @param ipScreen		[in]	IScreenDisplay のインスタンス
 * @param cSelectItem	[in]	道路アイテム
 * @param ipHitPoint	[in]	ポイント
 *
 * @return TRUE  正常終了
 * @return FALSE 異常終了
 */
BOOL AheDeleteRoadVertex( IScreenDisplayPtr ipScreen, CTargetItem& cSelectItem, IPointPtr ipHitPoint);

/**
 * @brief リンク内属性又は傾斜の検索
 *
 * @param ipPoint		[in]	ポイント
 * @param lpszLayer		[in]	検索レイヤ名
 * @param cRetItems		[out]	検索アイテム
 *
 * @return TRUE  正常終了
 * @return FALSE 異常終了
 */
BOOL AheSelectRoad_SegAttr_GradFromVertex(IMapPtr ipMap, IScreenDisplayPtr ipScreen, IPointPtr ipPoint, LPCTSTR lpszLayer, CTargetItems& cRetItems);

/**
 * @brief 道路にあるリンク内属性と傾斜の構成点を削除
 *
 * @param cRoadItem		[in]	ポイント
 * @param cSearchItems	[in]	検索レイヤ名
 *
 * @return TRUE  正常終了
 * @return FALSE 異常終了
 */
BOOL AheDeleteSegAttr_GradVertexWithRoad( IScreenDisplayPtr ipScreen, CTargetItems& cSearchItems, IPointPtr ipPoint);

/**
 * @brief ライン構成点削除
 *
 * @param cSelectItem		[in]	ラインアイテム
 * @param ipPoint			[in]	ポイント
 *
 * @return TRUE  正常終了
 * @return FALSE 異常終了
 */
BOOL AheDeletePolyLineVertex( IScreenDisplayPtr ipScreen, CTargetItem& cSelectItem, IPointPtr ipPoint);

/**
 * @brief ３点の角度を求める
 *
 * @param ipScreen		[in]	IScreenDisplay のインスタンス
 * @param cRoadItem1	[in]	道路アイテム１
 * @param cRoadItem2	[in]	道路アイテム２
 * @param ipPoint		[in]	ポイント
 *
 * @return DOUBLE
 */
DOUBLE AheGetRoadCrossAngle( IScreenDisplayPtr ipScreen, CTargetItem cRoadItem1, CTargetItem cRoadItem2, IPointPtr ipPoint);

/**
 * @brief 道路比較
 *
 * @param cRoadItem1	[in]	比較道路アイテム１
 * @param cRoadItem2	[in]	比較道路アイテム２
 *
 * @return TRUE  比較道路アイテム１のオブジェクトIDが大きい
 * @return FALSE 比較道路アイテム２のオブジェクトIDが大きい
 */
BOOL AheCompRoad(CTargetItem cRoadItem1, CTargetItem cRoadItem2);
//////////////////////////////////////////////////////////////////////
// リンク列に関する関数
//////////////////////////////////////////////////////////////////////
/**
 * 指定のエリア内に存在するロードキューのINFOレコードを取得
 *
 * @param ipRoadClass			[in]			道路リンククラス
 * @param ipInfoTable			[in]			検索するロードキューの INFO テーブル
 * @param ipLQTable				[in]			検索するロードキューの LQ テーブル
 * @param lpcszRoadRelFieldName	[in]			検索するロードキュー用のフラグ名称（道路リンクの属性）
 * @param ipGeom				[in]			選択範囲（道路リンクを選択）
 * @param lDrawMode				[in]			ロードキューの種類（IMultiSymbolRenderer で使用する描画モードを使用すること）
 * @param listInfoRows			[out]			取得できた INFO のレコード
 * @param listRoads				[out]			取得できた道路リンク
 * @param lSequence				[in,optional]	LQ_* を検索する際に指定するシーケンス番号（デフォルト：-1）
 *
 * @retval 取得できたINFOのレコード数
 */
LONG AheSelectInfoRowByShape( IFeatureClassPtr ipRoadClass, ITablePtr ipInfoTable, ITablePtr ipLQTable, LPCTSTR lpcszRoadRelFieldName, IGeometryPtr ipGeom, LONG lDrawMode, std::list<ROWDEF>& listInfoRows, std::list<ROWDEF>& listRoads, LONG lSequence = -1, bool bLastLink = false );

/**
 * 指定の道路リンクID列からロードキューのINFOレコードを取得
 *
 * @param listRoads				[in]		道路リンクID列
 * @param ipInfoTable			[in]		検索するロードキューの INFO テーブル
 * @param ipLQTable				[in]		検索するロードキューの LQ テーブル
 * @param lDrawMode				[in]		ロードキューの種類（IMultiSymbolRenderer で使用する描画モードを使用すること）
 * @param listInfoRows			[out]		取得できた INFO のレコード
 *
 * @retval 取得できたINFOのレコード数
 */
LONG AheSelectInfoRowByRoadIDs( const std::list<ROWDEF>& listRoads, ITablePtr ipInfoTable, ITablePtr ipLQTable, LONG lDrawMode, std::list<ROWDEF>& listInfoRows, bool bLastLink = false );

/**
 * 指定のINFOレコードからロードキューのLQレコードを取得
 *
 * @param listInfoRow			[in]		INFOレコード
 * @param ipLQTable				[in]		検索するロードキューの LQ テーブル
 * @param listLQRows			[out]		取得できた LQ のレコード
 *
 * @retval 取得できたLQのレコード数
 */
LONG AheSelectLQRowByInfoRow( const ROWDEF& listInfoRow, ITablePtr ipLQTable, std::list<ROWDEF>& listRQRows );

/**
 * 指定のINFOレコード列からロードキューのLQレコードを取得
 *
 * @param listInfoRows			[in]		INFOレコード列
 * @param ipLQTable				[in]		検索するロードキューの LQ テーブル
 * @param listLQRows			[out]		取得できた LQ のレコード
 *
 * @retval 取得できたLQのレコード数
 */
LONG AheSelectLQRowByInfoRows( const std::list<ROWDEF>& listInfoRows, ITablePtr ipLQTable, std::list<ROWDEF>& listLQRows );

/**
 * 指定の道路リンクID列からロードキューのLQレコードを取得
 *
 * @param listRoads				[in]		道路リンクID列
 * @param ipLQTable				[in]		検索するロードキューの LQ テーブル
 * @param lpcszRoadRelFieldName	[in]		検索するロードキュー用のフラグ名称（道路リンクの属性）
 * @param lDrawMode				[in]		ロードキューの種類（IMultiSymbolRenderer で使用する描画モードを使用すること）
 * @param listRQRows			[out]		取得できたLQのレコード
 * @param listInfoIDs			[out]		取得できたINFOのID列
 *
 * @retval 取得できたLQのレコード数
 */
LONG AheSelectLQRowByRoadIDs( const std::list<ROWDEF>& listRoads, ITablePtr ipLQTable, LPCTSTR lpcszRoadRelFieldName, LONG lDrawMode, std::list<ROWDEF>& listRQRows, std::list<LONG>& listInfoIDs );
LONG AheSelectLQRowByRoadIDs2( const std::list<ROWDEF>& listRoads, ITablePtr ipLQTable, LPCTSTR lpcszRoadRelFieldName, LONG lDrawMode, std::list<ROWDEF>& listRQRows, std::list<LONG>& listInfoIDs, CString& );

/**
 * 指定のLQレコード列から道路リンクを取得
 *
 * @param ipUnk					[in]			道路リンクのフィーチャクラスが取得できるもの
 * @param listLQRows			[in]			道路リンクを取得するのに必要なLQレコード列
 * @param listRoads				[in]			既に確保してある道路リンク（ない場合は空を渡してください）
 * @param listLQRoads			[out]			取得できた道路リンクのフィーチャ列
 * @param bComplete				[in,optional]	足りない道路リンクを検索するかどうか（デフォルトは検索する）	
 *
 * @retval 取得できた道路リンクの数
 */
LONG AheSelectLQRoad( IUnknownPtr ipUnk, const std::list<ROWDEF>& listLQRows, const std::list<ROWDEF>& listRoads, std::list<ROWDEF>& listLQRoads, BOOL bComplete = TRUE );

/**
 * リンク列描画用の形状を取得する
 *
 * @param lInfoID				[in]		描画するリンク列のレコードID
 * @param listLQRows			[in]		描画するのに必要なLQレコード列
 * @param listLQRoads			[in]		描画するのに必要な道路リンクフィーチャ列
 * @param bFixFlow				[out]		ジオメトリの方向が決定できたかどうか
 *
 * @return IGeometryPtr
 */
IGeometryPtr AheGetLQRoadGeometry( LONG lInfoID, const std::list<ROWDEF>& listLQRows, const std::list<ROWDEF>& listLQRoads, BOOL* bFixFlow );

/**
 * リンク列描画用の形状を取得する(マルチレンダラ用）
 *
 * @param lInfoID				[in]		描画するリンク列のレコードID
 * @param listLQRows			[in]		描画するのに必要なLQレコード列
 * @param listLQRoads			[in]		描画するのに必要な道路リンクフィーチャ列
 * @param bFixFlow				[out]		ジオメトリの方向が決定できたかどうか
 *
 * @return IGeometryPtr
 */
IGeometryPtr AheGetLQRoadGeometry2( LONG lInfoID, const std::list<ROWDEF>& listLQRows, const std::list<ROWDEF>& listLQRoads, BOOL* bFixFlow );

/**
 * @brief 削除するリンク列を選択する
 *
 * @param cSelectItems		[in]			リンク列を検索するためのリンクのコレクション
 *
 * @retval TURE 正常終了
 * @retval FALSE 異常終了
 */
BOOL AheSelectDeleteLQ( const CTargetItems& cSelectItem );

/**
 * @brief リンク列検索
 *
 * @param cSelectItem	[in]	道路アイテム
 * @param cCompItem		[in]	比較道路アイテム
 *
 * @return TRUE  正常終了
 * @return FALSE 異常終了
 */
BOOL AheSelectDeleteLQ( const CTargetItems& cRoadItems, CTargetItem& cSelectItem, CTargetItem& cCompItem, INT nType );

/**
 * @brief リンク列削除する
 *
 * @param cSelectItem			[in]	リンク列を検索するためのリンク
 * @param strInfoTableName		[in]	Infoテーブル名称
 * @param strLQTableName		[in]	LQテーブル名称
 * @param strRoadRelFieldName	[in]	リンクフラグ名称
 * @param lDrawMode				[in]	ロードキューの種類（IMultiSymbolRenderer で使用する描画モードを使用すること）
 *
 * @retval TURE 正常終了
 * @retval FALSE 異常終了
 */
BOOL AheDeleteLQ( CTargetItem& cSelectItem, LPCTSTR strInfoTableName, LPCTSTR strLQTableName, LPCTSTR strRoadRelFieldName, LONG lDrawMode );

/**
 * @brief リンク列削除
 *
 * @param cSelectItem			[in]	道路アイテム
 * @param cCompItem				[in]	比較道路アイテム
 * @param strInfoTableName		[in]	INFOテーブル名称
 * @param strLQTableName		[in]	LQテーブル名称
 * @param strRoadRelFieldName	[in]	検索するロードキュー用のフラグ名称（道路リンクの属性）
 * @param lDrawMode				[in]	ロードキューの種類（IMultiSymbolRenderer で使用する描画モードを使用すること）
 *
 * @return TRUE  正常終了
 * @return FALSE 異常終了
 */
BOOL AheDeleteLQ( const CTargetItems& cRoadItems, CTargetItem& cSelectItem, CTargetItem& cCompItem, LPCTSTR strInfoTableName, LPCTSTR strLQTableName, LPCTSTR strRoadRelFieldName, LONG lDrawMode, INT nType );

/**
 * @brief リンク列削除メッセージ
 *
 * @param cRoadItems	[in]	道路アイテムリスト
 *
 * @return TRUE  正常終了
 * @return FALSE 異常終了
 */
BOOL AheDeleteLQMsg( const CTargetItems& cRoadItems );

/**
 * @brief リンク列存在チェック
 *
 * @param ipMap					[in]	IMap のインスタンス
 * @param ipInfoTable			[in]	INFOテーブル
 * @param ipLQTable				[in]	LQテーブル
 * @param strRoadRelFieldName	[in]	検索するロードキュー用のフラグ名称（道路リンクの属性）
 * @param lDrawMode				[in]	ロードキューの種類（IMultiSymbolRenderer で使用する描画モードを使用すること）
 *
 * @return TRUE  正常終了
 * @return FALSE 異常終了
 */
BOOL AheCheckLQ(CTargetItem cSelectItem, ITablePtr ipInfoTable, ITablePtr ipLQTable, CString strRoadRelFieldName, LONG lDrawMode);

/**
 * @brief 道路リンク列に VICS を付与できるかどうか？（一方通行に違反していないか？）
 *
 * @param CtargetItems	[in]	対象道路リンク
 *
 * @return TRUE  付与可能
 * @return FALSE 付与不可能（既存の一方通行と逆）
 */
BOOL AheCheckOneWay(CTargetItems);

/**
 * @brief リンク列が作成可能か？
 *		  VICS、アクセス道のような重複を禁止しているもののチェック
 *
 * @param ipMap			[in]
 * @param CtargetItems	[in]	対象道路リンク
 * @param LQType		[in]	LQ の種類
 * @param nFromNodeID	[in]	
 *
 * @return TRUE  付与可能
 * @return FALSE 付与不可能（既存 LQ がある）
 */
BOOL AheCheckLQOverlap(IMapPtr ipMap, CTargetItems cItems, sindyeRoadQueueType LQType, INT nFromNodeID = 0 );

/**
 * @brief 道路リンク列に VICS を付与できるかどうか？
 *		  経路情報のチェック	
 *
 * @param CtargetItems	[in]	対象道路リンク
 *
 * @return TRUE  付与可能
 * @return FALSE 付与不可能（経路外の道路が含まれている）
 */
BOOL AheCheckVics(CTargetItems cItems);

//////////////////////////////////////////////////////////////////////
// ルールに関する関数
//////////////////////////////////////////////////////////////////////
/**
 * @brief フィーチャ選択閾値を返す
 *
 * @note ISiNDYRuleObj::GetSelectTolerance() のラッパー
 *
 * @param ipRule			[in,optional]	ISiNDYRulePtr
 *
 * @return INT
 */
INT AheGetTolerance( ISiNDYRulePtr ipRule = NULL );

/**
 * フィーチャが編集可能かどうかチェックする
 *
 * @note 可視レイヤに所属しているかどうかチェックして、属していなければFALSEを返します。
 * @note 可視レイヤに属しているのならAheIsEditableFeatureの結果を返します。
 *
 * @param ipMap						[in]			Map
 * @param ipFature					[in]			チェック対象フィーチャ
 * @param ipGeom					[in]			チェック対象形状（NULLの時はフィーチャの場合に限りipFeatureから取得）
 * @param type						[in]			sindyeEditType
 * @param ipRule					[in,optional]	ISiNDYRulePtr
 * @param bNotEditableIsNotShowDlg	[in,optional]	編集不可の時にダイアログを出さないかどうか（デフォルト：FALSE）
 * @param hWnd						[in,optional]	メッセージボックスの親ハンドル（デフォルト：NULL）
 *
 * @retval TRUE 編集可能
 * @retval FALSE 編集不可能
 */
BOOL AheIsEditableFeature2( IMap* ipMap, _IRow* ipFeature, IGeometry* ipGeom, sindyeEditType type, ISiNDYRule* ipRule = NULL, BOOL bNotEditableIsNotShowDlg = FALSE, HWND hWnd = NULL );

/**
 * フィーチャが編集可能かどうかチェックする
 *
 * @note ISiNDYRuleObj::IsEditableFeature() をラップして、TRUE か FALSE を返します。
 *
 * @param ipFature					[in]			チェック対象フィーチャ
 * @param ipGeom					[in]			チェック対象形状（NULLの時はフィーチャの場合に限りipFeatureから取得）
 * @param type						[in]			sindyeEditType
 * @param ipRule					[in,optional]	ISiNDYRulePtr
 * @param bNotEditableIsNotShowDlg	[in,optional]	編集不可の時にダイアログを出さないかどうか（デフォルト：FALSE）
 * @param hWnd						[in,optional]	メッセージボックスの親ハンドル（デフォルト：NULL）
 *
 * @retval TRUE 編集可能
 * @retval FALSE 編集不可能
 */
BOOL AheIsEditableFeature( _IRowPtr ipFeature, IGeometryPtr ipGeom, sindyeEditType type, ISiNDYRulePtr ipRule = NULL, BOOL bNotEditableIsNotShowDlg = FALSE, HWND hWnd = NULL );

/**
 * @brief 作成可能レイヤかどうか
 *
 * @note リンク列の場合はINFのテーブルを渡すこと
 *
 * @param ipTable					[in]			チェックするテーブル
 * @param ipCreateArea				[in]			作成する予定の形状
 * @param bNotEditableIsNotShowDlg	[in,optional]	編集不可の時にダイアログを出すかどうか（デフォルト：FALSE）
 * @param hWnd						[in,optional]	メッセージボックスの親ハンドル（デフォルト：NULL）
 *
 * @retval TRUE		作成可能
 * @retval FALSE	作成不可能
 */
BOOL AheIsCreatableLayer( ITablePtr ipTable, IGeometryPtr ipCreateArea, BOOL bNotEditableIsNotShowDlg = FALSE, HWND hWnd = NULL );

/**
 * @brief 削除可能レイヤかどうか
 *
 * @note ロックメッシュ、エディットメッシュを考慮に入れていないので注意！！
 *
 * @param lpcszLayerName	[in]			チェックするレイヤ名
 * @param ipRule			[in,optional]	ISiNDYRulePtr
 *
 * @retval TRUE 削除可能
 * @retval FALSE 削除不可能
 */
BOOL AheIsDeletableLayer( LPCTSTR lpcszLayerName, ISiNDYRulePtr ipRule = NULL );

/**
 * @brief 属性編集可能レイヤかどうか
 *
 * @note ロックメッシュ、エディットメッシュを考慮に入れていないので注意！！
 *
 * @param lpcszLayerName	[in]			チェックするレイヤ名
 * @param ipRule			[in,optional]	ISiNDYRulePtr
 *
 * @retval TRUE 属性編集可能
 * @retval FALSE 属性編集不可能
 */
BOOL AheIsAttrEditableLayer( LPCTSTR lpcszLayerName, ISiNDYRulePtr ipRule = NULL );

/**
 *
 * @brief 2次メッシュを跨いでいる道路リンクを取得する
 *
 * @param ipMap			[in]	IMap のインスタンス
 * @param ipFeature		[in]	選択した道路フィーチャ
 * @param cRoadCrossMesh[out]	2次メッシュを跨いでいる道路フィーチャ
 * 
 * retval TRUE 選択した道路フィーチャは2次メッシュと接している
 * retval FALSE 選択した道路フィーチャは2次メッシュと接していない
 */
BOOL AheIsRoadCrossMesh(IMapPtr ipMap, IFeaturePtr ipFeature, CTargetItems &cRoadCrossMesh);

/**
 *
 * @brief 2次メッシュを跨いでいるフィーチャを取得する
 *
 * 今のところ、傾斜とリンク内属性だけの判定
 *
 * @param ipFeature		[in]	選択したフィーチャ
 * @param cRoadCrossMesh[out]	2次メッシュを跨いでいるフィーチャ
 * 
 * retval TRUE 選択したフィーチャは2次メッシュと接している
 * retval FALSE 選択したフィーチャは2次メッシュと接していない
 */
BOOL AheIsFeatureCrossMesh(IMapPtr ipMap, IFeaturePtr ipFeature, CTargetItems &cFeatureCrossMesh);


#endif	//__AHEGLOBALSX_H__
