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
* @file
*
* @brief Arc Helper グローバル関数定義ファイル
*/
#if	!defined( __AHEGLOBALS_H__)
#define __AHEGLOBALS_H__	//!< 重複インクルード防止

#if	defined( _UNICODE)
typedef std::map< std::wstring, LONG>	FIELDMAP;
#else
typedef std::map< std::string, LONG>	FIELDMAP;
#endif

#include "AheGlobalsCoord.h"	// URI 解析用関数・座標変換など
#include "AheGlobalsMisc.h"		// その他
#include <ArcHelperEx/define.h>
#include <ArcHelperEx/define_enum.h>

/////////////////////////////////////////////////////////////////////////////
//
// 設定値操作
//
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief	設定iniファイルのパス取得
 *
 * @param lpszPath				[out]		設定INIファイルパス
 * @param nSize					[in]		取得バッファサイズ
 *
 * @return 設定INIファイルのパス文字列を保存するバッファサイズ
 */
int AheGetInifilePath( LPTSTR lpszPath, int nSize);

/**
 * @brief	距離（閾値）の設定を INI ファイルから取得する
 *
 * @param lpcszInifilePath		[in]		INI ファイルのパス
 *
 * @return 距離の値
 */
UINT AheGetSelectTolerance( LPCTSTR lpcszInifilePath = NULL);

//////////////////////////////////////////////////////////////////////
// ワークスペースに関する関数
//////////////////////////////////////////////////////////////////////

/**
 * @brief ワークスペースを取得する
 * @note IRow、IDataset、IWorkspaceのインターフェースを持っているオブジェクトに限ります。
 * @param ipUnk [in] IRow、IDataset、IWorkspaceのインターフェースを持ったオブジェクト
 * @return IWorkspace
 */
IWorkspacePtr AheGetWorkspace( IUnknown* ipUnk );

/**
 * @brief リモートワークスペースかどうかを調べる
 * @note IWorkspace::get_PathName()でパスが入っているかどうかで判定します。
 * @param ipWorkspace [in] ワークスペース
 * @retval true リモートワークスペース
 * @retval false ファイルベースのワークスペース
 */
bool AheIsRemoteWorkspace( IWorkspace* ipWorkspace );

//////////////////////////////////////////////////////////////////////
// 判定に関する関数
//////////////////////////////////////////////////////////////////////

/**
* @brief 道路レイヤ判定
*
* @note ROAD_TABLE_NAMEのテーブル名を持つレイヤかを判定する。
*
* @param piLayer [in] 判手するレイヤ
*
* @see ROAD_TABLE_NAME
*
* @retval TRUE 道路レイヤ
* @retval FALSE 道路レイヤではない
*/
BOOL AheIsRoadLayer( ILayerPtr piLayer);

/**
* @brief ノードレイヤ判定
*
* @note NODE_TABLE_NAMEのテーブル名を持つレイヤかを判定する。
*
* @param piLayer [in] 判手するレイヤ
*
* @see NODE_TABLE_NAME
*
* @retval TRUE ノードレイヤ
* @retval FALSE ノードレイヤではない
*/
BOOL AheIsNodeLayer( ILayerPtr piLayer);

/**
* @brief 道路クラス判定
*
* @note ROAD_TABLE_NAMEのテーブル名を持つフィーチャクラスかを判定する。
*
* @param piFeatureClass [in] フィーチャクラス
*
* @see ROAD_TABLE_NAME
*
* @retval TRUE 道路フィーチャクラス
* @retval FALSE 道路フィーチャクラスではない
*/
BOOL AheIsRoadClass( IFeatureClassPtr piFeatureClass);

/**
* @brief ノードクラス判定
*
* @note NODE_TABLE_NAMEのテーブル名を持つフィーチャクラスかを判定する。
*
* @param piFeatureClass [in] フィーチャクラス
*
* @see NODE_TABLE_NAME
*
* @retval TRUE ノードフィーチャクラス
* @retval FALSE ノードフィーチャクラスではない
*/
BOOL AheIsNodeClass( IFeatureClassPtr piFeatureClass);

/**
* @brief 道路フィーチャ判定
*
* @note ROAD_TABLE_NAMEのテーブル名を持つフィーチャかを判定する。
*
* @param piMap [in] マップオブジェクトのインスタンス
* @param piFeature [in] フィーチャ
*
* @see ROAD_TABLE_NAME
*
* @retval TRUE 道路フィーチャ
* @retval FALSE 道路フィーチャではない
*/
BOOL AheIsRoadFeature( IMapPtr piMap, IFeaturePtr piFeature);

/**
* @brief ノードフィーチャ判定
*
* @note NODE_TABLE_NAMEのテーブル名を持つフィーチャかを判定する。
*
* @param piMap [in] マップオブジェクトのインスタンス
* @param piFeature [in] フィーチャ
*
* @see NODE_TABLE_NAME
*
* @retval TRUE ノードフィーチャ
* @retval FALSE ノードフィーチャではない
*/
BOOL AheIsNodeFeature( IMapPtr piMap, IFeaturePtr piFeature);

/**
* @brief 注記レイヤ判定
* @param lpcszFeatureClassName [in] フィーチャクラス名
* @retval TRUE 注記レイヤ
* @retval FALSE 注記レイヤではない
*/
BOOL AheIsChukiLayer( LPCTSTR lpcszFeatureClassName );

/**
* @brief POIレイヤ判定
* @param layerName [in] レイヤ名
* @retval TRUE POIレイヤ
* @retval FALSE POIレイヤではない
*/
bool AheIsPoiLayer( const CString& layerName );

/**
* @brief ノードレイヤを取得
*
* @note TOCから、NODE_TABLE_NAMEのテーブル名を持つレイヤを取得
*
* @param piMap			[in]			マップオブジェクトのインスタンス
* @param bEditableOnly	[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
*
* @see NODE_TABLE_NAME
*
* @return ILayerPtr
*/
ILayerPtr AheGetNodeLayer( IMapPtr piMap, BOOL bEditableOnly = FALSE );

/**
* @brief 道路レイヤを取得
*
* @note TOCから、ROAD_TABLE_NAMEのテーブル名を持つレイヤを取得
*
* @param piMap			[in]			マップオブジェクトのインスタンス
* @param bEditableOnly	[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
*
* @see ROAD_TABLE_NAME
*
* @return ILayerPtr
*/
ILayerPtr AheGetRoadLayer( IMapPtr piMap, BOOL bEditableOnly = FALSE );

/**
* @brief ノードクラスを取得
*
* @note TOCから、NODE_TABLE_NAMEのテーブル名を持つフィーチャクラスを取得
*
* @param piMap			[in]			マップオブジェクトのインスタンス
* @param bEditableOnly	[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
*
* @see ROAD_TABLE_NAME
*
* @return IFeatureClassPtr
*/
IFeatureClassPtr AheGetNodeClass( IMapPtr piMap, BOOL bEditableOnly = FALSE );

/**
* @brief 道路クラスを取得
*
* @note TOCから、ROAD_TABLE_NAMEのテーブル名を持つフィーチャクラスを取得
*
* @param piMap			[in]			マップオブジェクトのインスタンス
* @param bEditableOnly	[in,optional]	編集可能なもののみを対象とする場合はTRUE（デフォルト：FALSE）
*
* @see ROAD_TABLE_NAME
*
* @return IFeatureClassPtr
*/
IFeatureClassPtr AheGetRoadClass( IMapPtr piMap, BOOL bEditableOnly = FALSE );

/**
 * @brief 渡されたフィーチャが可視レイヤに属するものかどうか調べる
 *
 * @param ipMap			[in]			Map
 * @param ipFeature		[in]			検査対象フィーチャ
 * @retval FALSE 不可視レイヤに属している、又はエラー
 * @retval TRUE 可視レイヤに属している
 */
BOOL AheIsVisibleFeature( IMap* ipMap, IFeature* ipFeature );

/// フィーチャ、テーブルが所属するレイヤをデータフレームから検索する
std::list<CAdapt<ILayerPtr> > AheFindLayers( IMap* ipMap, IUnknown* ipUnk, bool bVislbleOnly = false, bool bEditableOnly = false );

/// レイヤが表示されているかどうかをチェックする
BOOL AheIsVisibleLayer( IMapLayers* ipMap, ILayer* ipLayer );

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

/**
 * テーブル名を取得するための IDataset オブジェクトを取得する
 *
 * IFeatureClass->get_Dataset() で得られるオブジェクトとは違うことに注意！！
 *
 * @param ipUnk				[in]			IFeature、IFeatureClass、ILayerオブジェクトのうちのどれか
 * @param ipDataset			[out]			取得された IDataset オブジェクト
 *
 * @retval TURE IDataset を取得できた
 * @retval FALSE IDataset を取得できなかった
 */
BOOL AheGetDatasetForTableName( IMapPtr piMap, IUnknownPtr ipUnk, IDataset** ipDataset );

/**
 * @brief レイヤ名の比較
 *
 * @param piLayer			[in]			ILayer のインスタンス
 * @param lpcszLayerName	[in]			比較対象のレイヤ名
 *
 * @retval TURE 一致
 * @retval FALSE 不一致
 */
BOOL AheCompareLayerName( ILayerPtr piLayer, LPCTSTR lpcszLayerName);

/**
 * @brief フィーチャクラス名の比較
 *
 * @param piFeatureClass	[in]			IFeatureClass のインスタンス
 * @param lpcszLayerName	[in]			比較対象のレイヤ名
 *
 * @retval TURE 一致
 * @retval FALSE 不一致
 */
BOOL AheCompareFeatureName( IFeatureClassPtr piFeatureClass, LPCTSTR lpcszLayerName);

/**
 * @brief フィールド名から IField のインスタンス取得する
 *
 * @param piMap				[in]			IMap のインスタンス
 * @param lpszTableName		[in]			取得したい IField のテーブル名
 * @param lpszFieldName		[in]			取得したい IField の列名
 * @param ipRetField		[out]			取得された IField のインスタンス
 *
 * @retval TURE IFeild を取得できた
 * @retval FALSE IFeild を取得できなかった
 */
BOOL AheGetFieldByFieldName( IMapPtr piMap, LPCTSTR lpszTableName, LPCTSTR lpszFieldName, IField** ipField );

/**
 * @brief フィールド名から IField のインスタンス取得する
 *
 * @param ipTable			[in]			ITable のインスタンス
 * @param lpszFieldName		[in]			取得したい 列名
 * @param ipRetField		[out]			取得された IField のインスタンス
 *
 * @retval TURE IFeild を取得できた
 * @retval FALSE IFeild を取得できなかった
 */
BOOL AheGetFieldByFieldName( ITablePtr ipTable, LPCTSTR lpszFieldName, IField** ipRetField );

/**
 * @brief レコードが変更されているか
 *
 * @param ipRow			[in]			検査する IRow
 *
 * @retval TRUE 変更されている
 * @retval FALSE 変更されていない
 */
BOOL AheRowIsChanged( _IRowPtr ipRow );

/**
 * @brief 形状が変更されているか
 *
 * @param ipFeature		[in]			検査する IFeature
 *
 * @retval TRUE 変更されている
 * @retval FALSE 変更されていない
 */
BOOL AheShapeIsChanged( IFeaturePtr ipFeature );

/**
 * @brief 同じフィーチャかどうかのテスト
 *
 * @param ipFeature1	[in]			比較元の IFeature のインスタンス
 * @param ipFeature2	[in]			比較先の IFeature のインスタンス
 *
 * @retval TURE 一致
 * @retval FALSE 不一致
 */
BOOL AheIsSameFeature( IFeaturePtr ipFeature1, IFeaturePtr ipFeature2 );

/**
 * @brief 同じジオメトリかどうかのテスト
 *
 * @param ipGeom1		[in]			比較元の IGeometry のインスタンス
 * @param ipGeom2		[in]			比較先の IGeometry のインスタンス
 *
 * @retval TURE 一致
 * @retval FALSE 不一致
 */
BOOL AheIsSameGeometry( IGeometryPtr ipGeom1, IGeometryPtr ipGeom2 );

/**
 * @brief あるフィールドの値が同じかどうか
 *
 * @param ipFeature1	[in]			比較元の IFeature のインスタンス
 * @param ipFeature2	[in]			比較先の IFeature のインスタンス
 * @param bstrFieldName [in]			比較するフィールド名
 *
 * @retval TURE 一致
 * @retval FALSE 不一致
 */
BOOL AheIsSameField( IFeaturePtr ipFeature1, IFeaturePtr ipFeature2, BSTR bstrFieldName );

/**
 * @brief ある2つのレコードの属性値が同じかどうか
 *
 * @param ipRow1		[in]			比較するレコード（_IRowPtr or IFeaturePtr）
 * @param ipRow2		[in]			比較するレコード（_IRowPtr or IFeaturePtr）
 *
 * @note SiNDY-e 的に編集可能なフィールドのみチェックします
 *       両方のフィーチャに存在するフィールドのみチェックします
 * 
 * @retval TRUE  同じ属性値を持つ
 * @retval FALSE 異なる属性値を持つ
 */
BOOL AheIsSameAttributes( _IRowPtr ipRow1, _IRowPtr ipRow2 );

/**
 * 編集できるフィールドかどうかの判定を行う
 *
 * @param ipField		[in]			判定するフィールド
 * @param bForCopy		[in,optional]	コピー時かどうか\nコピーの時はいくつかのフィールドが編集可と判断されます。
 *
 * @return 編集できるフィールドなら TRUE、編集できないなら FALSE を返す
 *
 * @note 大文字小文字関係なく判定できます。
 *
 * @note この関数では本来編集できないフィールドのほかに、SiNDY-e 的にユーザに変更して欲しくないフィールドも含んでいます。
 * @note 今後も増えることが予想されるので、その場合は適宜追加する必要があります。
 */
BOOL AheIsEditableField( IFieldPtr ipField, bool bForCopy = FALSE );

/**
 * @brief 渡されたフィーチャの種類を返す
 *
 * @param ipFeature	[in]			調べる IFeature のインスタンス
 *
 * @return フィーチャの種類（sindyeFeatureKinds）
 */
sindyeFeatureKinds AheGetFeatureKinds( IFeaturePtr ipFeature );

/////////////////////////////////////////////////////////////////////////////
// フィールドに関する関数
/////////////////////////////////////////////////////////////////////////////
/*
 * @biref IFieldsを取得する
 * @param ipUnk [in] IFieldsを取り出せるオブジェクト
 * サポートするオブジェクトは以下のインターフェースを持つもの：
 * @li IFields
 * @li _IRow
 * @li ITable
 * @li _ICursor
 */
IFieldsPtr AheGetFields( IUnknown* ipUnk );

/*
 * @brief フィールド名でフィールドインデックスを取得する
 * @param ipFields [in] IFields
 * @param lpcszFieldName [in] フィールド名
 * フィールド名と検索文字列の大文字小文字が一致しない場合を考慮
 * し、ループを回して一つずつフィールド名を確認します。
 * 従って、IFields::FindFieldは使用しません。
 * @retval -1 フィールド名が見つからない、又は引数がNULL
 * @retval -1以外 フィールドインデックス
 */
long AheFindField( IFields* ipFields, LPCTSTR lpcszFieldName );

/**
 * @brief ShapeフィールドがZ値持ちかを判断する
 * @param ipField [in] 対象フィールド
 * @retval true Z値持ちである
 * @retval false Z値持ちではない
 */
bool AheHasZ(IFieldPtr ipField);

/**
 * @brief フィーチャクラスがZ値持ちかを判断する
 * @param ipFeatureClass [in] 対象フィーチャクラス
 * @retval true Z値持ちである
 * @retval false Z値持ちではない
 */
bool AheHasZ(IFeatureClassPtr ipFeatureClass);

/**
 * @brief ジオメトリがZ値持ちかを判断する
 * @param ipGeometry [in] 対象ジオメトリ
 * @retval true Z値持ちである
 * @retval false Z値持ちではない
 */
bool AheGeoHasZ(IGeometryPtr ipGeometry);

/**
 * @brief  ジオメトリがZ値を持っている場合、ジオメトリにZ値0を付与する
 * @detail Z値を持っていない場合、何もしない
 * @note これがないとZ値に非数が付与されて具合が悪くなるケースがある
         マルチポイントには対応していないのでご注意(すみません by ta_suga)
 * @param ipGeometry [in/out] 対象ジオメトリ
 * @retval true 付与成功
 * @retval false 付与失敗
 */
bool AheSetZeroToZ(IGeometryPtr ipGeometry);

//////////////////////////////////////////////////////////////////////
// フィーチャの属性に関する関数
//////////////////////////////////////////////////////////////////////

/**
 * フィーチャに属性をセットする
 * 
 * @note Store() はしません
 *
 * @param ipRow				[in]	セットするフィーチャ（レコード）
 * @param lpcszFieldName	[in]	セットするフィールド名
 * @param vaValue			[in]	セットする値
 *
 * @retval TRUE セットできた
 * @retval FALSE セットできなかった
 */
BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, const VARIANT& vaValue );
BOOL AheSetAttribute( IRowBufferPtr ipRow, long lIndex, const VARIANT& vaValue );
BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, long lValue );
BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, LPCTSTR lpcszValue );
BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, double dValue );
inline BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, long lValue ){
	return AheSetAttribute( ipRow, lpcszFieldName, CComVariant(lValue) );
}
inline BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, LPCTSTR lpcszValue ){
	return AheSetAttribute( ipRow, lpcszFieldName, CComVariant(lpcszValue) );
}
inline BOOL AheSetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName, const double dValue ){
	return AheSetAttribute( ipRow, lpcszFieldName, CComVariant(dValue) );
}

/**
 * フィーチャから属性を取得する
 *
 * @param ipRow				[in]	フィーチャ（レコード）
 * @param lpcszFieldName	[in]	取得するフィールド名
 *
 * @retval 取得できた値
 */
CComVariant AheGetAttribute( IRowBufferPtr ipRow, LPCTSTR lpcszFieldName );
CComVariant AheGetAttribute( IRowBufferPtr ipRow, long lIndex );

/**
 * @brief 属性をコピーする
 *
 * @param ipSourceRow       [in]			コピー元
 * @param ipDestRow         [in]			コピー先
 * @param bIsSplitMode		[in,optional]	フィーチャ分割時の属性コピーの場合はTRUEを指定する（デフォルト：FALSE）
 *
 * @return 正常終了したら TRUE
 */
BOOL AheCopyAttribute( IRowBufferPtr ipSourceRow, IRowBufferPtr ipDestRow, BOOL bIsSplitMode = FALSE );

#ifndef __BUILD_SINDYRULE__
/**
 * @brief フィーチャの修正情報を更新
 *
 * @param piRow					[in]			修正情報を更新するフィーチャ
 * @param emUpdateType			[in]			修正事由
 * @param lpSystemTime			[in]			システムの保持する時間
 *
 * @retval TURE 更新完了
 * @retval FALSE 更新異常
 */
BOOL AheSetModifyData( IRowBufferPtr piRow, sindyUpdateType emUpdateType, LPSYSTEMTIME lpSystemTime = NULL);
#endif //__BUILD_SINDYRULE__

/**
 * @brief フィーチャにデフォルト値を設定
 *
 * @note フィーチャを新規に作成した場合は必ず呼んでください
 *
 * @param ipRow					[in]			IRow、IFeature
 *
 * @retval TRUE 更新完了
 * @retval FALSE 渡されたフィーチャが NULL
 */
BOOL AheSetDefaultData( IRowBufferPtr ipRow );

/**
 * @brief 新規フィーチャ（レコード）を作成
 *
 * @param ipUnk					[in]			IFeature、IRow、ITable、IDataset、IFeatureClass など IDataset を取り出せるもの
 * @param ipGeom				[in]			ジオメトリ（フィーチャの場合のみ利用されるデフォルトは NULL）
 * @param bSetDefaultData		[in]			デフォルト値を入れるかどうか（デフォルトは TRUE）
 *
 * @note ArcGIS 8.2 から、ジオメトリの地理参照をちゃんとあわせないとずれるようになったため、作成
 * @note ipGeom は地理参照をあわせていなくても OK。必ず IGeoDataset::get_SpatialReference() で地理参照を取得してジオメトリに与えている。
 *
 * @return 作成された IRow または IFeature
 */
_IRowPtr AheCreateRow( IUnknownPtr ipUnk, IGeometryPtr ipGeom = NULL, BOOL bSetDefaultData = TRUE );

/**
 * @brief 新規フィーチャ（レコード）を作成
 *
 * @param ipUnk					[in]			IFeature、IRow、ITable、IDataset、IFeatureClass など IDataset を取り出せるもの
 * @param ipGeom				[in]			ジオメトリ（フィーチャの場合のみ利用されるデフォルトは NULL）
 * @param bSetDefaultData		[in]			デフォルト値を入れるかどうか（デフォルトは TRUE）
 *
 * @note ArcGIS 8.2 から、ジオメトリの地理参照をちゃんとあわせないとずれるようになったため、作成
 * @note ipGeom は地理参照をあわせていなくても OK。必ず IGeoDataset::get_SpatialReference() で地理参照を取得してジオメトリに与えている。
 *
 * @return 作成された IRowBuffer または IFeatureBuffer
 */
IRowBufferPtr AheCreateRowBuffer( IUnknownPtr ipUnk, IGeometryPtr ipGeom = NULL, BOOL bSetDefaultData = TRUE );

/**
 * @brief フィーチャに形状を反映させる
 *
 * @note IFeature::putref_Shape() で形状が変わる場合があったため、それの回避策
 *
 * @param ipFeature				[in]			IFeatureBufferPtr
 * @param ipGeometry			[in]			反映させたいジオメトリ
 *
 * @retval TRUE 反映成功
 * @retval FALSE 反映失敗
 */
BOOL AheSetShape( IFeatureBufferPtr ipFeature, IGeometryPtr ipGeometry );
//////////////////////////////////////////////////////////////////////
// デバッグ用関数
//////////////////////////////////////////////////////////////////////
/**
 * IRow のトレースをする（デバッグ版のみ）
 *
 * @param ipRow			[in]	IRowBufferPtr
 * @param lpcszMessage	[in]	メッセージ
 */
void AheTraceRow( IRowBufferPtr ipRow, LPCTSTR lpcszMessage );

//////////////////////////////////////////////////////////////////////
// 座標に関する関数
//////////////////////////////////////////////////////////////////////
/**
 * @brief ピクセル->LonLat変換
 *
 * @param pIDisplay	[in]			IDisplay のインスタンス
 * @param dPixelUnits		[in]			ピクセル値
 *
 * @return 座標上での距離
 */
DOUBLE AheConvertPixelsToMapUnits( IDisplayPtr pIDisplay, double dPixelUnits);

/**
 * @brief XY -> IPoint変換
 *
 * @param pIDisplay	[in]			IDisplay のインスタンス
 * @param X					[in]			X 座標値
 * @param Y					[in]			Y 座標値
 * @param ipPoint			[out]			変換後の IPoint のインスタンス
 */
void AheQueryMapPoint( IDisplayPtr pIDisplay, const LONG X, const LONG Y, IPoint** ipPoint);

//////////////////////////////////////////////////////////////////////
// ドメインに関する関数
//////////////////////////////////////////////////////////////////////
/**
 * @brief 指定された名称のコード値ドメインを取得する
 *
 * @param ipCodedValueDomain		[in]			ICodedValueDomain のインスタンス
 * @param lpszCodedValueDomainName	[in]			コード値ドメインの名称
 * @param va						[out]			取得した値
 *
 * @retval TURE コード値ドメインがあり
 * @retval FALSE コード値ドメインがなし
 */
BOOL AheCodedValueDomainName2VARIANT( ICodedValueDomainPtr ipCodedValueDomain, LPCTSTR lpszCodedValueDomainName, VARIANT& va );

/**
 * @brief 指定されたコード値ドメインの名称取得する
 *
 * @param ipCodedValueDomain		[in]			ICodedValueDomain のインスタンス
 * @param va						[in]			コード値ドメインの名称
 * @param lpszCodedValueDomainName	[out]			取得した値
 *
 * @retval TURE コード値ドメインがあり
 * @retval FALSE コード値ドメインがなし
 */
BOOL AheVARIANT2CodedValueDomainName( ICodedValueDomainPtr ipCodedValueDomain, VARIANT va, CString& lpszCodedValueDomainName );

/**
 * オブジェクトとフィールド名から相当するコード値ドメインの文字列と値を取得する
 *
 * @param ipUnk			[in]	IRowまたはIFeatureオブジェクト
 * @param lpszFieldName	[in]	対象フィールド名
 * @param strDomainName	[out]	取得されたコード値ドメインの文字列
 * @param vaValue		[out]	取得された値
 *
 * @retval TURE 正常終了
 * @retval FALSE 異常終了
 */
BOOL AheGetDomainNameAndValue( IUnknownPtr ipUnk, LPCTSTR lpszFieldName, CString& strDomainName, VARIANT& vaValue );

/**
 * @brief オブジェクトとフィールド名から相当するコード値ドメインの文字列を取得する
 *
 * @param ipUnk			[in]	IRowまたはIFeatureオブジェクト
 * @param lpszFieldName	[in]	対象フィールド名
 *
 * @return コード値ドメインの文字列を取得（コード値ドメインの文字列を取得できなかった場合は値をそのまま返す）
 */
CComVariant AheGetDomainName( IUnknownPtr ipUnk, LPCTSTR lpszFieldName );

/**
 * テーブル名とフィールド名からドメインを取得する
 *
 * @param lpszTableName			[in]	テーブル名
 * @param lpszFieldName			[in]	フィールド名
 * @param ipDomain				[out]	ドメイン
 *
 * @retval TURE ドメイン名を取得できた
 * @retval FALSE ドメイン名を取得できなかった
 */
BOOL AheGetDomainByFieldName( IMapPtr piMap, LPCTSTR lpszTableName, LPCTSTR lpszFieldName, IDomain** ipDomain );
//////////////////////////////////////////////////////////////////////
// 判定に関する関数
//////////////////////////////////////////////////////////////////////

/**
 * ノードフィーチャが終端かどうかのチェック
 *
 * @param ipNodeFeature [out] 調べるノードが含まれているフィーチャ
 *
 * @retval TURE 端点
 * @retval FALSE 端点以外
 */
BOOL AheIsEndNode( IFeaturePtr ipNodeFeature );

//////////////////////////////////////////////////////////////////////
// フィーチャ選択に関する関数
//////////////////////////////////////////////////////////////////////
/**
 * @brief ISpatialFilter を作成する
 *
 * @param ipGeom			[in]			選択範囲
 * @param ipFeatureClass	[in]			フィーチャクラス
 * @param spatialRel        [optional]		選択方法
 * @param lpcszSubFields	[in,optional]	取得するフィールド名
 * @param lpcszWhereClause	[in,optional]	WHERE 文
 *
 * @return IQueryFilterPtr
 */
IQueryFilterPtr AheGetSpatialFilter( IGeometryPtr ipGeom, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL/*, ISpatialReferencePtr ipSpRef = NULL*/ );
/**
 * 与えられたジオメトリの範囲内に存在するフィーチャを検索
 *
 * ipFeatureCursor はインスタンスを作成せずに渡してください
 *
 * @param ipGeom            [in]           選択範囲
 * @param ipFeatureClass    [in]     	   フィーチャクラス
 * @param ipFeatureCursor   [retval]       コレクション
 * @param spatialRel        [optional]	   選択方法
 * @param lpcszSubFields	[in,optional]  取得するフィールド名
 * @param lpcszWhereClause	[in,optional]  WHERE 文
 *
 * @return 検索できたフィーチャの数
 */
LONG AheSelectByShapeAndGetCount( IGeometryPtr ipGeom, IFeatureClassPtr ipFeatureClass, IFeatureCursor** ipFeatureCursor, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL, BOOL bRecycling = FALSE );

/**
 * 与えられたジオメトリの範囲内に存在するフィーチャを検索
 *
 * @param ipGeom            [in]           選択範囲
 * @param ipFeatureClass    [in]     	   フィーチャクラス
 * @param spatialRel        [optional]	   選択方法
 * @param lpcszSubFields	[in,optional]  取得するフィールド名
 * @param lpcszWhereClause	[in,optional]  WHERE 文
 * @param bRecycling		[in,optional]  デフォルト：FALSE
 *
 * @return IFeatureCursorPtr
 */
IFeatureCursorPtr AheSelectByShape( IGeometryPtr ipGeom, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL, BOOL bRecycling = FALSE );

/**
 * 与えられたジオメトリの範囲内に存在するフィーチャを検索
 *
 * @param ipGeom            [in]           選択範囲
 * @param ipFeatureClass    [in]     	   レイヤ
 * @param spatialRel        [optional]	   選択方法
 * @param lpcszSubFields	[in,optional]  取得するフィールド名
 * @param lpcszWhereClause	[in,optional]  WHERE 文
 * @param bRecycling		[in,optional]  デフォルト：FALSE
 *
 * @see AheSelectByShape
 *
 * @return IFeatureCursorPtr
 */
IFeatureCursorPtr AheSelectByShapeFromLayer( IGeometryPtr ipGeom, ILayerPtr ipLayer, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL, BOOL bRecycling = FALSE );
//////////////////////////////////////////////////////////////////////
// に関する関数
//////////////////////////////////////////////////////////////////////
/**
 * @brief フィーチャのポイント列を取得する
 *
 * @param ipFeature				[in]			対象となる IFeature のインスタンス
 * @param ipRetCol				[out]			フィーチャのポイントコレクションのインスタンス
 */
void AheGetFeatureVertex( IFeaturePtr ipFeature, IPointCollection** ipRetCol);

/**
 * @brief フィーチャのどの部分にヒットするか調べる
 *
 * @param piFeature				[in]			対象となる IFeature のインスタンス
 * @param piTestPoint			[in]			調査する IPoint のインスタンス			
 * @param dbTorelance			[in]			距離
 * @param piPoint				[out]			ヒットした IPoint のインスタンス
 * @param pdbHitDist			[out,optional]	ヒットした点とテスト点との距離
 * @param plPartIndex			[out,optional]	ヒットした辺のインデックス
 * @param plVertexIndex			[out,optional]	ヒットした頂点のインデックス
 * @param testType				[in,optional]	テストするタイプ（デフォルト：頂点、辺）
 *
 * @return ヒットタイプ
 */
esriGeometryHitPartType AheTestFeatureGeometryHit( IFeaturePtr piFeature, IPointPtr piTestPoint, double dbTorelance, IPoint** piPoint, double* pdbHitDist = NULL, LONG* lPartIndex = NULL, LONG* lVertexIndex = NULL, esriGeometryHitPartType testType = (esriGeometryHitPartType)(esriGeometryPartVertex|esriGeometryPartBoundary) );

/**
 * @brief ジオメトリのどの部分にヒットするか調べる
 *
 * @param ipGeom				[in]			対象となる IGeometry のインスタンス
 * @param piTestPoint			[in]			調査する IPoint のインスタンス			
 * @param dwTorelance			[in]			距離
 * @param piPoint				[out]			ヒットした IPoint のインスタンス
 * @param pdwHitDist			[out,optional]	ヒットした点とテスト点との距離
 * @param plHitPartIndex		[out,optional]	ヒットしたパートのインデックス
 * @param plHitSegmentIndex		[out,optional]	ヒットした辺のインデックス
 * @param testType				[in,optional]	テストするタイプ（デフォルト：頂点、辺）
 *
 * @return ヒットタイプ
 */
esriGeometryHitPartType AheTestGeometryHit( IGeometryPtr ipGeom, IPointPtr ipTestPoint, double dwTorelance, IPoint** ipPoint, double* pdwHitDist, LONG* plHitPartIndex, LONG* plHitSegmentIndex, esriGeometryHitPartType testType = (esriGeometryHitPartType)(esriGeometryPartVertex|esriGeometryPartBoundary) );

/**
 * @brief フィーチャの頂点にヒットした場合はそのインデックス番号を、ヒットしなかった場合は -1 を返す
 *
 * @note TestGeometryHit() の簡易版 (2 は、必ずlVertexIndexを返す。注記で使用)
 *
 * @param ipFeature				[in]			対象となる IFeature のインスタンス
 * @param ipPoint				[in]			テストする IPoint のインスタンス
 * @param dTorelance			[in]			距離
 *
 * @retval -1以外 ヒットしたインデックス番号
 * @retval -1 ヒットしなかった
 */
LONG AheHitTestVertexIndex( IFeaturePtr ipFeature, IPointPtr ipPoint, double dTorelance);
LONG AheHitTestVertexIndex2( IFeaturePtr ipFeature, IPointPtr ipPoint, double dTorelance);

/////////////////////////////////////////////////////////////////////////////
//
// フィーチャ編集に関する関数
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief フィーチャの形状からポイントを削除する
 *
 * @param ipFeature				[in]	フィーチャ
 * @param lIndex				[in]	構成点インデックス
 *
 * @return IPointCollectionPtr
 */
IPointCollectionPtr AheRemovePoint( IFeaturePtr ipFeature, LONG lIndex );

/**
 * ラインフィーチャをポイントジオメトリでスプリットする
 *
 *   主に道路をスプリットするのに使用します。
 *   SplitRoadByPoint() で使用されます。通常使用する場合は
 *   SplitRoadByPoint() を使用してください。
 *
 * @param ipPointGeom   [in]  スプリットポイントジオメトリ
 * @param ipLineFeature [in]  スプリット対象のラインフィーチャ
 *
 * @return スプリットした後のフィーチャセット
 */
//_ISetPtr AheSplitLineByPoint( IFeaturePtr ipLineFeature,  IGeometryPtr ipSplitPointGeom);

/**
 * @brief ラインフィーチャをポイントコレクションでスプリットする
 *
 * @param piLineFeature			[in]			スプリットしたいラインフィーチャ
 * @param piSplitPoipnts		[in]			スプリットする IPointCollection のインスタンス
 *
 * @return スプリットした結果のフィーチャ郡を格納した ISet のインスタンス
 */
_ISetPtr AheSplitLineByPoints( IFeaturePtr piLineFeature, IPointCollectionPtr piSplitPoipnts);

/**
 * @brief フィーチャからバッファ付きポリゴンを取得する
 *
 * @param ipFeature			[in]			ポリゴンを取得する対象フィーチャ
 * @param dbBufferSize		[in]			バッファするサイズ
 * @param ipPolygon			[out]			取得したポリゴン
 */
void AheFeature2Polygon( IFeaturePtr ipFeature, double dbBufferSize, IPolygon** ipPolygon);

/**
 * @brief ジオメトリからバッファ付きポリゴンを取得する
 *
 * @param ipGeom			[in]			ポリゴンを取得する対象ジオメトリ
 * @param dbBufferSize		[in]			バッファするサイズ
 * @param ipPolygon			[out/optional]	取得したポリゴン
 */
IPolygonPtr AheGeometry2Polygon( IGeometryPtr ipGeom, double dbBufferSize, IPolygon** ipPolygon=NULL);

/**
 * @brief フィールド名称とエイリアスのインデックスマップを取得します。
 *
 * @param piFeature			[in]			インデックスマップを取得する対象フィーチャ
 * @param cNameMap			[out]			フィールド名称とそのインデックスのマップコンテナ
 * @param cAliasMap			[out]			エイリアス名称とそのインデックスのマップコンテナ
 *
 * @return フィーチャのフィールド数
 */
LONG AheGetFieldMap( IFeaturePtr piFeature, FIELDMAP& cNameMap, FIELDMAP& cAliasMap);

/**
 * ポイントからバッファを持った矩形を返す
 *
 *   指定されたポイントと長さから矩形を作成します。
 *   フィーチャの選択などに多く使用されています。
 *
 * @param ipPoint [in]  ポイント
 * @param pixel   [in]  ピクセル
 * @param IEnvelopePtr
 */
BOOL AheGetEnvelope( IPointPtr ipPoint, IDisplayPtr piScrnDisp, LONG pixel, IEnvelope** ppiEnvelope);

/**
 * ポイントからバッファを持った矩形を返す
 *
 *   指定されたポイントと長さから矩形を作成します。
 *   フィーチャの選択などに多く使用されています。
 *
 * @param ipPoint      [in]  ポイント
 * @param dTolerance   [in]  幅
 * @return IEnvelopePtr
 */
BOOL AheGetEnvelope( IPointPtr ipPoint, DOUBLE dTolerance, IEnvelope** ppiEnvelope);

/**
 * @brief	指定の形状が、２点間を結ぶ直線と交差する点列を取得
 *
 * @param piBaseGeometry		[in]		チェック対象の形状
 * @param dbFromX				[in]		チェック対象線分の開始座標
 * @param dbFromY				[in]		チェック対象線分の開始座標
 * @param dbToX					[in]		チェック対象線分の終点座標
 * @param dbToY					[in]		チェック対象線分の終点座標
 * @param piPointCollection		[out]		交差点集合
 *
 * @return 常にTUREを返す
 */
//BOOL AheGetPointsCrossedLine( IGeometryPtr piBaseGeometry, double dbFromX, double dbFromY, double dbToX, double dbToY, IPointCollectionPtr piPointCollection);

/**
 * レベル付きメッセージ出力関数
 *
 *   指定されたレベルにしたがって、メッセージを出力する。
 *
 * @param lpcszMsg      [in]  メッセージ
 * @param lpcszCap   [in]  キャプション
 * @param uType   [in]  メッセージボックスの内容と動作
 * @param uLevel   [in]  出力レベル
 * @return select
 */
int AheMessageBox( LPCTSTR lpcszMsg, LPCTSTR lpcszCap, UINT uType, UINT uLevel = WARNLEVEL_MIDDLE);

/**
 * @brief	レベル付きメッセージ出力関数
 *
 * @param hWnd				[in]		メッセージボックスに渡すウィンドウハンドル
 * @param lpcszMsg			[in]		メッセージの内容
 * @param lpcszCap			[in]		メッセージボックスのキャプション
 * @param uType				[in]		メッセージボックスのボタンのタイプ
 * @param uLevel			[in]		メッセージのレベル
 *
 * @return ユーザーの選択値
 */
int AheMessageBox( HWND hWnd, LPCTSTR lpcszMsg, LPCTSTR lpcszCap, UINT uType, UINT uLevel = WARNLEVEL_MIDDLE);

/**
* @brief ZooMoveToolのキー移動拡大縮小処理
*
* @note ZooMoveToolで実装したキーボードでの処理部分を抽出
*
* @param piActView [in] ビュー
* @param lKeyCode [in] 仮想キーコード
* @param lShift [in] 修飾キー？ステータス
* @param pblHandled [out] 処理済みフラグ
*
* @retval TURE 正常終了
* @retval FALSE 異常終了
*/
BOOL AheZooMoveKeyDownProc( IActiveViewPtr piActView, LONG lKeyCode, LONG lShift, BOOL* pblHandled);

/**
* @brief カーソル位置に画面の中心を移動する
*
* @note 第2引数は[Bug 3772]拡大が効かなくなるバグへの対応をした際に、一旦Transformイベントをとめて、VisibleBoundsで領域を
*       設定を行ったところ、[Bug 5495]が発生してしまい、イベントをとめてしまうと描画の変更はあっても、内部的には前の座標位置で
*       あることがわかったため、MoveCenterToMousePointerの後にイベントを発行する処理がない場合には、イベントとめないことにする
*
* @param piActiveView [in] マップのビュー
* @param bSuppressEvents [in] Transformイベントをとめるか否か（デフォルト(true)はイベントをとめる）
*
* @retval TRUE 正常終了
* @retval FALSE 異常終了
*/
BOOL AheMoveCenterToMousePointer( IActiveViewPtr piActiveView, bool bSuppressEvents = true );

/**
* @brief カーソルをマップのセンターに移動
*
* @param piActiveView [in] マップのビュー
*
* @retval TRUE 正常終了
* @retval FALSE 異常終了
*/
BOOL AheMoveCursor2Center( IActiveViewPtr piActiveView);

/**
* @brief 指定ステップ数分の移動
*
* @param piActiveView [in] マップのビュー
* @param nXStep [in] 移動単位
* @param nYStep [in] 移動単位
* @retval TRUE 正常終了
* @retval FALSE 異常終了
*/
BOOL AheShiftMap( IActiveViewPtr piActView, int nXStep, int nYStep);

// MapFANのZM値に則した拡大率での縮小処理
void AheZoomUp( IActiveViewPtr piActiveView);
void AheZoomDown( IActiveViewPtr piActiveView);

// 再描画モードを取得
bool AheGetRefreshMode();

/**
* @brief ちらつき抑制再描画
*
* @param piActiveView [in] マップのビュー
* @param ipEnv [in] 再描画矩形
*/
void AheRefresh(IActiveViewPtr piActView, IEnvelopePtr ipEnv=NULL);

/////////////////////////////////////////////////////////////////////////////
//
// レジストリの基本アクセス関数
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief	レジストリのキーを作成する
 *
 * @param lpszKey				[in]		キー名
 * @param hKey					[in]		キーのハンドル
 *
 * @return 作成された REG KEY
 */
HKEY  EmxCreateRegKey( LPCTSTR lpszKey, HKEY hKey );

/**
 * @brief	指定されたレジストリのキーの値（DWORD）を書き込む
 *
 * @param lpszValueName			[in]		値
 * @param hKey					[in]		キーのハンドル
 *
 * @retval TURE 書き込めた
 * @retval FALSE 書き込み異常
 */BOOL  EmxSetRegDword( LPCTSTR lpszValueName, DWORD dwData, HKEY hKey );

/**
 * @brief	指定されたレジストリのキーの値（DWORD）を取得する
 *
 * @param lpszValueName			[in]		値の名前
 * @param lpdwBuff				[out]		取得した値
 * @param hKey					[in]		キーのハンドル
 *
 * @retval TURE 読み込めた
 * @retval FALSE 読み込み異常
 */
BOOL  EmxGetRegDword( LPCTSTR lpszValueName, DWORD* lpdwBuff, HKEY hKey );

/**
 * @brief	指定されたレジストリのキーの値（文字列）を書き込む
 *
 * @param lpszValueName			[in]		値の名前
 * @param lpdwBuff				[out]		取得した文字列
 * @param hKey					[in]		キーのハンドル
 *
 * @retval TURE 書き込めた
 * @retval FALSE 書き込み異常
 */
BOOL  EmxSetRegString( LPCTSTR lpszValueName, LPCTSTR lpcszString, HKEY hKey );

/**
 * @brief	指定されたレジストリのキーの値（文字列）を取得する
 *
 * @param lpszValueName			[in]		値の名前
 * @param lpdwBuff				[out]		取得した文字列
 * @param hKey					[in]		キーのハンドル
 *
 * @retval TURE 読み込めた
 * @retval FALSE 読み込み異常
 */
BOOL  EmxGetRegString( LPCTSTR lpszValueName, LPTSTR lpReturnedString, DWORD nSize, HKEY hKey);
BOOL  EmxGetRegString( LPCTSTR lpszValueName, LPTSTR lpReturnedString, HKEY hKey);

 /**
  * @brief 空間参照の最小距離を求める
  *
  * @param ipSpRef			[in]			空間参照
  *
  * @retval DOUBLE 最小距離（ipSpRef が NULL の場合は -1.0）
  */
DOUBLE AheGetXYDomain1UnitLength( ISpatialReferencePtr ipSpRef );

/**
 * @brief 空間参照の精度が細かい方を取得する
 *
 * @param ipSpRef1			[in]			空間参照
 * @param ipSpRef2			[in]			空間参照
 *
 * @return ISpatialReferencePtr
 */
ISpatialReferencePtr AheGetMinimumSpatialReference( ISpatialReferencePtr ipSpRef1, ISpatialReferencePtr ipSpRef2 );

#endif	//__AHEGLOBALS_H__
