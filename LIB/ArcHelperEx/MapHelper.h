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
* @file MapHelper.h
*
* @brief IMapに関連する処理を行うCMapHelperの定義ファイル
*/
#if	!defined( __MAPHELPER_H__)
#define	__MAPHELPER_H__	//!< 重複インクルード防止

#include "AheGlobals.h"
#include "AheLayerFunctions.h"
#include "AheGlobalsX.h"
#include "AheGlobalsX2.h"

class CArcHelper;

/**
* @class CMapHelper
*
* @brief IMapに関連する処理をまとめたクラス。
*
* @note 基本的にはAheのグローバル関数を呼び出している
*/
class CMapHelper
{
public:
	CMapHelper( CArcHelper& cArcHelper);
	CMapHelper();
	~CMapHelper();

	/**
	* @brief 代入演算子
	*
	* @note コピー元の管理する IMap オブジェクトのインスタンスをコピーする
	*
	* @param cMapHelper [in] コピー元のクラスオブジェクト
	*
	* @return 自身への参照
	*/
	CMapHelper& operator=( CMapHelper& cMapHelper);

	/**
	* @brief 代入演算子
	*
	* @note コピー元のCArcHelper から IMap オブジェクトを取得し、自身を初期化する。
	*
	* @param cArcHelper [in] コピー元のクラスオブジェクト
	*
	* @return 自身への参照
	*/
	CMapHelper& operator=( CArcHelper& cArcHelper);

	//////////////////////////////////////////////////////////////////////
	// ワークスペースに関する関数
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief ワークスペースを取得
	*
	* @note ::AheGetWorkspace を内部で呼び出している。
	*
	* @param piUnk [in] オブジェクトのインスタンス
	* @param ppiWorkspace [out] 取得された IWorkspace のインスタンス
	*
	* @see AheGetWorkspace
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetWorkspace( IUnknownPtr piUnk, IWorkspace** ppiWorkspace);
	//////////////////////////////////////////////////////////////////////
	// テーブル検索に関する関数
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief ITableインスタンスを取得
	*
	* @note 指定されたテーブル名称で取得
	* @note ::AheGetTableByTableName を内部で呼び出している。
	*
	* @param lpszTableName [in] テーブル名称
	* @param ppiTable [out] 取得されたITableインスタンス
	*
	* @see AheGetTableByTableName
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetTableByTableName( LPCTSTR lpcszTableName, ITable** ppiTable );

	/**
	* @brief IField インスタンスを取得
	*
	* @note 指定されたテーブル名称とフィールド名称から取得する。
	* @note ::AheGetFieldByFieldName を内部で呼び出している
	*
	* @param lpszTableName [in] テーブル名称
	* @param lpszFieldName [in] フィールド名称
	* @param ppiField [out] 取得されたIField インスタンス
	*
	* @see AheGetFieldByFieldName
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetFieldByFieldName( LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName, IField** ppiField );

	/**
	* @brief IField インスタンスを取得
	*
	* @note 指定されたテーブルのインスタンスとフィールド名称から取得する。
	* @note ::AheGetFieldByFieldName を内部で呼び出している
	*
	* @param piTable [in] テーブルのインスタンス
	* @param lpszFieldName [in] フィールド名称
	* @param ppiField [out] 取得されたIField インスタンス
	*
	* @see AheGetFieldByFieldName
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetFieldByFieldName( ITablePtr piTable, LPCTSTR lpcszFieldName, IField** ppiRetField );

	/**
	* @brief IDomain インスタンスを取得
	*
	* @note 指定されたテーブル名称とフィールド名称から取得する。
	* @note ::AheGetDomainByFieldName を内部で呼び出している
	*
	* @param lpszTableName [in] テーブルの名称
	* @param lpszFieldName [in] フィールド名称
	* @param ppiField [out] 取得されたIDomain インスタンス
	*
	* @see AheGetDomainByFieldName
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetDomainByFieldName( LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName, IDomain** ppiDomain );

	//////////////////////////////////////////////////////////////////////
	// レイヤ取得に関する関数
	//////////////////////////////////////////////////////////////////////

	/**
	* @brief レイヤのテーブル名から ILayer を取得
	*
	* @param lpcszLayerName	[in]			テーブル名称
	* @param ppiRetLayer	[out]			取得されたレイヤオブジェクトのインスタンス
	* @param bEditableOnly	[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
	*
	* @see AheGetLayerByTableName
	* @see _FIND_TABLE
	* @see AheGetLayerByName
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetLayerByTableName( LPCTSTR lpcszLayerName, ILayer** ppiRetLayer, BOOL bEditableOnly = FALSE );

	/**
	* @brief レイヤのテーブル名から ILayer を取得
	*
	* @param lpcszLayerName [in] テーブル名称
	* @param bEditableOnly	[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
	*
	* @see AheGetLayerByTableName
	* @see _FIND_TABLE
	* @see AheGetLayerByName
	*
	* @return ILayerPtr
	*/
	ILayerPtr GetLayerByTableName( LPCTSTR lpcszLayerName, BOOL bEditableOnly = FALSE );

	/**
	* @brief レイヤのエイリアス名から ILayer を取得
	*
	* @param lpcszLayerName [in] エイリアス名称
	* @param ppiRetLayer [out] 取得されたレイヤオブジェクトのインスタンス
	*
	* @see AheGetLayerByAliasName
	* @see _FIND_ALIAS
	* @see AheGetLayerByName
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetLayerByAliasName( LPCTSTR lpcszLayerName, ILayer** ppiRetLayer );

	/**
	* @brief レイヤのエイリアス名から ILayer を取得
	*
	* @param lpcszLayerName [in] エイリアス名称
	*
	* @see AheGetLayerByAliasName
	* @see _FIND_ALIAS
	* @see AheGetLayerByName
	*
	* @return ILayerPtr
	*/
	ILayerPtr GetLayerByAliasName( LPCTSTR lpcszLayerName );

	/**
	* @brief *.mxdファイルに保存されているレイヤ名から ILayer を取得
	*
	* @param lpcszLayerName [in] *.mxdファイル内名称
	* @param ppiRetLayer [out] 取得されたレイヤオブジェクトのインスタンス
	*
	* @see AheGetLayerByMXDName
	* @see _FIND_MXDNAME
	* @see AheGetLayerByName
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetLayerByMXDName( LPCTSTR lpcszLayerName, ILayer** ppiRetLayer );

	/**
	* @brief *.mxdファイルに保存されているレイヤ名から ILayer を取得
	*
	* @param lpcszLayerName [in] *.mxdファイル内名称
	*
	* @see AheGetLayerByMXDName
	* @see _FIND_MXDNAME
	* @see AheGetLayerByName
	*
	* @return ILayerPtr
	*/
	ILayerPtr GetLayerByMXDName( LPCTSTR lpcszLayerName );

	/**
	* @brief レイヤ名から ILayer を取得
	*
	* @param lpcszLayerName [in] レイヤ名称
	* @param ppiRetLayer [out] 取得されたレイヤオブジェクトのインスタンス
	* @param type [in] 比較対照
	*
	* @see AheGetLayerByName
	* @see FINDTYPE
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetLayerByName( LPCTSTR lpcszLayerName, ILayer** ppiRetLayer, FINDTYPE type = _FIND_TABLE );

	/**
	* @brief レイヤ名から ILayer を取得
	*
	* @param lpcszLayerName [in] レイヤ名称
	* @param type [in] 比較対照
	*
	* @see AheGetLayerByName
	* @see FINDTYPE
	*
	* @return ILayerPtr
	*/
	ILayerPtr GetLayerByName( LPCTSTR lpcszLayerName, FINDTYPE type = _FIND_TABLE );

	/**
	* @brief *.lyr ファイルから ILayer を取得
	*
	* @param lpcszLayerFile [in] レイヤファイルパス
	* @param ppiRetLayer [out] 取得されたレイヤオブジェクトのインスタンス
	*
	* @see AheGetLayerFromFile
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetLayerFromFile( LPCTSTR lpcszLayerFile, ILayer** ppiRetLayer );

	/**
	* @brief *.lyr ファイルから ILayer を取得
	*
	* @param lpcszLayerFile [in] レイヤファイルパス
	*
	* @see AheGetLayerFromFile
	*
	* @return ILayerPtr
	*/
	ILayerPtr GetLayerFromFile( LPCTSTR lpcszLayerFile );
	//////////////////////////////////////////////////////////////////////
	// レイヤ名取得に関する関数
	//////////////////////////////////////////////////////////////////////

	/**
	* @brief レイヤのテーブル名を取得
	*
	* @param piUnk [in] 取得もとのインスタンス
	* @param pbstrLayerName [out] 取得されたレイヤ名称
	*
	* @see AheGetLayerTableName
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetLayerTableName( IUnknownPtr piUnk, BSTR* pbstrLayerName);

	/**
	* @brief レイヤのエイリアス名を取得
	*
	* @param piUnk [in] 取得もとのインスタンス
	* @param pbstrLayerName [out] 取得されたレイヤ名称
	*
	* @see AheGetLayerAliasName
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetLayerAliasName( IUnknownPtr piUnk, BSTR* pbstrLayerName);

	/**
	* @brief *.mxdファイルに保存されているレイヤ名を取得
	*
	* @param piUnk [in] 取得もとのインスタンス
	* @param pbstrLayerName [out] 取得されたレイヤ名称
	*
	* @see AheGetLayerMXDName
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetLayerMXDName( IUnknownPtr piUnk, BSTR* pbstrLayerName);

	/**
	* @brief レイヤ名を取得
	*
	* @param piUnk [in] 取得もとのインスタンス
	* @param pbstrName [out] 取得されたレイヤ名称
	* @param type [in] 取得種別
	*
	* @see AheGetLayerName
	* @see FINDTYPE
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetLayerName( IUnknownPtr piUnk, BSTR* pbstrName, FINDTYPE type = _FIND_TABLE );

	/**
	* @brief レイヤ名を取得
	*
	* @note TCHARへ変換して戻す
	*
	* @param piUnk [in] 取得もとのインスタンス
	* @param lpszName [out] 取得されたレイヤ名称
	* @param type [in] 取得種別
	*
	* @see AheGetLayerName
	* @see FINDTYPE
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetLayerName( IUnknownPtr piUnk, LPCTSTR& lpszName, FINDTYPE type = _FIND_TABLE );

	/**
	* @brief IDataset オブジェクトを取得
	*
	* @note テーブル名を取得するための IDataset オブジェクトを取得する
	*
	* @param piUnk [in] 取得もとのインスタンス
	* @param ppiDataset [out] 取得された IDataset のインスタンス
	*
	* @see AheGetDatasetForTableName
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetDatasetForTableName( IUnknownPtr piUnk, IDataset** ppiDataset );

	//////////////////////////////////////////////////////////////////////
	// フィーチャクラス取得に関する関数
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief フィーチャクラスを取得
	*
	* @param piUnk [in] フィーチャ
	* @param ppiFeatureClass [out] 取得された IFeatureClass のインスタンス
	*
	* @see AheGetFeatureClass
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetFeatureClass( IUnknownPtr piUnk, IFeatureClass** ppiFeatureClass );

	/**
	* @brief レイヤが表示可能なスケールかどうかの判定
	*
	* @param pGeoFeatureLayer [in] 判定対象
	*
	* @see AheIsVisibleScale
	*
	* @retval TURE 表示可
	* @retval FALSE 表示不可
	*/
	BOOL IsVisibleScale( IGeoFeatureLayerPtr pGeoFeatureLayer);

	//////////////////////////////////////////////////////////////////////
	// フィーチャ選択に関する関数
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief 指定のエリア内にあるフィーチャーを選択する
	*
	* @param piGeom				[in]			検索範囲形状
	* @param cTargetItems		[out]			検索結果格納コンポーネント
	* @param spatialRel			[in,optional]	選択方式
	* @param lpcszSubFields		[in,optional]	取得するフィールド名
	* @param lpcszWhereClause	[in,optional]	WHERE 文
	* @param bVisibleOnly		[in,optional]	表示されているレイヤのみの場合はTRUE
	* @param bEditableOnly		[in,optional]	編集可能レイヤのみの場合はTRUE
	*
	* @see AheSelectByShape
	*
	* @return 選択された対象の数
	*/
	LONG SelectByShape( IGeometryPtr piGeom, CTargetItems& cTargetItems, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL, BOOL bVisibleOnly = FALSE, BOOL bEditableOnly = FALSE );

	/**
	* @brief 指定のエリア内にあるフィーチャーを選択する
	*
	* @param ipGeometry			[in]			検索範囲形状
	* @param cTargetItems		[out]			検索結果格納コンポーネント
	* @param piLayer			[in]			検索対象レイヤ
	* @param spatialRel			[in,optional]	選択方式
	* @param lpcszSubFields		[in,optional]	取得するフィールド名
	* @param lpcszWhereClause	[in,optional]	WHERE 文
	*
	* @see AheSelectByShapeFromLayer
	*
	* @return 選択された対象の数
	*/
	LONG SelectByShapeFromLayer( IGeometryPtr ipGeometry, CTargetItems& cTargetItems, ILayerPtr ipLayer, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL );

	/**
	* @brief 指定のエリア内にあるフィーチャーを選択する
	*
	* @param ipGeometry			[in]			検索範囲形状
	* @param cTargetItems		[out]			検索結果格納コンポーネント
	* @param ipFeatureClass		[in]			検索対象フィーチャクラス
	* @param spatialRel			[in,optional]	選択方式
	* @param lpcszSubFields		[in,optional]	取得するフィールド名
	* @param lpcszWhereClause	[in,optional]	WHERE 文
	*
	* @see AheSelectByShapeFromFeatureClass
	*
	* @return 選択された対象の数
	*/
	LONG SelectByShapeFromFeatureClass( IGeometryPtr ipGeometry, CTargetItems& cTargetItems, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL );

	/**
	* @brief 指定のエリア内にあるフィーチャーを選択する
	*
	* @param ipGeom				[in]			検索範囲形状
	* @param cTargetItems		[out]			検索結果格納コンポーネント
	* @param lpcszLayersName	[in]			検索対象レイヤ名
	* @param spatialRel			[in,optional]	選択方式
	* @param lpcszSubFields		[in,optional]	取得するフィールド名
	* @param lpcszWhereClause	[in,optional]	WHERE 文
	* @param bEditableOnly		[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
	*
	* @see AheSelectByShapeFromLayers
	*
	* @return 選択された対象の数
	*/
	LONG SelectByShapeFromLayers( IGeometryPtr ipGeom, CTargetItems& cTargetItems, LPCTSTR lpcszLayersName, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL, BOOL bEditableOnly = FALSE );

	/**
	* @brief 渡されたフィーチャ（ノード限定）に接続されている道路をコレクションに入れて返す
	*
	* @param piFeature [in] 調査対象のノードフィーチャ
	* @param cTargetItems [out] 接続道路コレクション
	*
	* @see AheGetConnectedRoad
	*
	* @return 取得アイテム数
	*/
	LONG GetConnectedRoad( IFeaturePtr piFeature, CTargetItems& cTargetItems);
	
	/**
	* @brief フィーチャの構成点上に別のオブジェクトの構成点が存在するか調べる
	*
	* @param dTol [in] 検索範囲閾値
	* @param piFeature [in] 検査フィーチャ
	* @param piOwnFeature [in] 除外オブジェクト
	* @param bDeleteEndNode [in] 
	* @param ppiEndNode [out]
	* @param lpcszLayernames [in]	レイヤ名称[名称ごと、必ず後ろと前に|記載]
	*
	* @see AheCheckVertexPoint
	*/
	LONG CheckVertexPoint( double dTol, IFeaturePtr piFeature, IFeaturePtr piOwnFeature, BOOL bDeleteEndNode, IFeature** ppiEndNode, LPCTSTR lpcszLayerNames);


	/**
	* @brief ノードレイヤを取得する
	*
	* @note NODE_TABLE_NAME のレイヤを取得
	*
	* @param bEditableOnly	[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
	*
	* @see AheGetNodeLayer
	* @see NODE_TABLE_NAME
	*/
	ILayerPtr GetNodeLayer( BOOL bEditableOnly = FALSE );

	/**
	* @brief 道路レイヤを取得する
	*
	* @note ROAD_TABLE_NAME のレイヤを取得
	*
	* @param bEditableOnly	[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
	*
	* @see AheGetRoadLayer
	* @see ROAD_TABLE_NAME
	*/
	ILayerPtr GetRoadLayer( BOOL bEditableOnly = FALSE );

	/**
	* @brief ノードのフィーチャクラスを取得する
	*
	* @note NODE_TABLE_NAME のフィーチャクラスを取得
	*
	* @param bEditableOnly	[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
	*
	* @see AheGetNodeClass
	* @see NODE_TABLE_NAME
	*/
	IFeatureClassPtr GetNodeClass( BOOL bEditableOnly = FALSE );
	
	/**
	* @brief 道路のフィーチャクラスを取得する
	*
	* @note ROAD_TABLE_NAME のレイヤを取得
	*
	* @param bEditableOnly	[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
	*
	* @see AheGetRoadClass
	* @see ROAD_TABLE_NAME
	*/
	IFeatureClassPtr GetRoadClass( BOOL bEditableOnly = FALSE );
protected:
	IMapPtr m_piMap;	//!< 管理対象MAPの IMap インスタンス
};

#include "maphelper.inl"

#endif	//__MAPHELPER_H__
