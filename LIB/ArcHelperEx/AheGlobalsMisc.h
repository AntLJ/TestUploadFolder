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
 * @note AheGlobals.h/cppから主に「その他」に属する関数群を分離したもの。
 *       経緯は下記レビューを参照の事。
 *       ArcHelperExのリファクタリング（crd_cnv関係） http://orias.mr.ipc.pioneer.co.jp/reviewboard/r/2424/
 *       AheGlobals→AheGlobalsCoordへの分離だけで済むかと思われたが、
 *       ・既存コードに影響を与えないようにするため、AheGlobals.hでAheGlobalsCoord.hをincludeする必要がある。
 *       ・AheFact()はAheGlobalsCoord.cpp内で使用されている。
 *       という理由から、AheFact()がAheGlobals.hにあるとヘッダの循環参照が発生するため、
 *       AheGlobalsMisc.h/cppとして分離する必要があった。
 *       それ以外の関数も一緒に移動したのは、単にライブラリとしての体裁のためである。
 */
#pragma once

//////////////////////////////////////////////////////////////////////
// その他の関数
//////////////////////////////////////////////////////////////////////
/**
 * @brief フィーチャの構成点を取得する
 *
 * @param ipFeature			[in]			IFeature のインスタンス
 * @param lIndex			[in]			取得する構成点のインデックス
 *
 * @return IPoint のインスタンス
 */
IPointPtr AheGetPoint( IFeaturePtr ipFeature, LONG lIndex );

/**
 * @brief 階乗を求める処理
 *
 * @param lBase			[in]			階乗する元の数
 * @param lCnt			[in]			階乗する回数
 *
 * @return 計算結果
 */
long AheFact(long lBase, long lCnt);

/**
 * @brief 現在表示しているスケールで表示できるレイヤかどうかの判定
 *
 * @param piMap				[in]			IMap のインスタンス
 * @param pGeoFeatureLayer	[in]			IGeoFeatureLayer のインスタンス
 *
 * @retval TURE 現在表示しているスケールで表示可能
 * @retval FALSE 現在表示しているスケールで表示不可能
 */
BOOL AheIsVisibleScale( IMapPtr piMap, IGeoFeatureLayerPtr pGeoFeatureLayer );

/**
 * @brief プログラムIDからツールを取得する
 *
 * @note ツールバーに存在する場合のみ、取得できます。なお、取得時に新たにインスタンスは生成しません。
 *
 * @param ipDoc				[in]			IDocumentPtr
 * @param lpcszProgramID	[in]			プログラムID
 *
 * @return ICommandPtr
 */
_ICommandPtr AheGetCommandByProgramID( IDocumentPtr ipDoc, LPCTSTR lpcszProgramID );

/**
 * @brief プログラムIDからツールを取得する
 *
 * @note ツールバーに存在する場合のみ、取得できます。なお、取得時に新たにインスタンスは生成しません。
 *
 * @param ipDoc				[in]			IDocumentPtr
 * @param lpcszProgramID	[in]			プログラムID
 *
 * @return ICommandItemPtr
 */
ICommandItemPtr AheGetCommandItemByProgramID( IDocumentPtr ipDoc, LPCTSTR lpcszProgramID );

/**
 * @brief クローンを取得する
 *
 * @note クローンを作成できないものがあります
 *
 * @param ipUnk				[in]	クローン作成元
 *
 * @retval IClonePtr	クローン
 * @retval NULL			作成できなかった
 */
IClonePtr AheGetClone( IUnknownPtr ipUnk );

/**
 * @brief VARIANT 値の比較
 *
 * @param va1				[in]			比較元の VARIANT 値
 * @param va2				[in]			比較先の VARIANT 値
 *
 * @retval TURE 一致
 * @retval FALSE 不一致
 */
BOOL IsSameVariantObject( VARIANT va1, VARIANT va2 );

/**
 * @brief IField が NULL OK かどうかの判定
 *
 * @param ipField			[in]			判定したい IField のインスタンス
 *
 *
 * @retval TURE NULLを許す
 * @retval FALSE NULLを許さない
 */
BOOL AheIsNullableField( IFieldPtr ipField);

/**
 * @brief ジオメトリからポリライン取得
 *
 * @param ipGeometry	[in]	変換ジオメトリ
 *
 * @return IPolylinePtr
 */
BOOL AheGeometry2Polyline(IGeometryPtr ipGeometry, IPolyline** ppiPolyline);

/**
 * @brief ジオメトリからポイントコレクション取得
 *
 * @param ipGeometry	[in]	変換ジオメトリ
 *
 * @return IPointCollectionPtr
 */
BOOL AheGeometry2PointCollection(IGeometryPtr ipGeometry, IPointCollection** ppiPointCollection);

/**
 * @brief 左右どちら向き
 *
 * @param ipSPoint	[in]	スタートぽいんと
 * @param ipSPoint	[in]	ミドルぽいんと
 * @param ipSPoint	[in]	エンドぽいんと
 *
 * @return TRUE:左向き
 * @return FALSE:右向き
 */
BOOL AheIsRightAndLeft(IPointPtr ipSPoint, IPointPtr ipMPoint, IPointPtr ipEPoint);

/**
 * @brief 地理参照をあわせる
 *
 * @param ipGeom	[in]	IGeometryPtr
 * @param ipSpRef	[in]	ISpatialReferencePtr
 *
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL AheSnapToSpatialReference( IGeometryPtr ipGeom, ISpatialReferencePtr ipSpRef );

/**
 * @brief 強制的に Simplify をかける
 *
 * @note 形状によってかける Simplify を変更しています
 *
 * @param ipGeom	[in]	ジオメトリ
 *
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL AheForceSimplify( IGeometryPtr ipGeom );

/**
 * @brief 地理参照をあわせた後、強制的に Simplify をかける
 *
 * @see AheSnapToSpatialReference
 * @see AheForceSimplify
 *
 * @param ipGeom	[in]	IGeometryPtr
 * @param ipSpRef	[in]	ISpatialReferencePtr
 *
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL AheSnapToSpatialReferenceAndForceSimplify( IGeometryPtr ipGeom, ISpatialReferencePtr ipSpRef );

/**
 * @brief 文字列リソースから文字列を取得する
 *
 * @return CString
 */
inline CString AheLoadString( UINT nID ){ CString str; if( str.LoadString( nID ) ) return str; else _ASSERTE(false); return (LPCTSTR)NULL; }

/**
 * @brief 文字列リソースから文字列を取得する
 *
 * @return CStringA
 */
inline CStringA AheLoadStringA( UINT nID ){ CStringA str; if( str.LoadString( nID ) ) return str; else _ASSERTE(false); return (LPCSTR)NULL; }

/**
 * @brief 文字列リソースから文字列を取得する
 *
 * @return CStringW
 */
inline CStringW AheLoadStringW( UINT nID ){ CStringW str; if( str.LoadString( nID ) ) return str; else _ASSERTE(false); return (LPCWSTR)NULL; }

CString AheGetErrorMessage();

void AheTraceErrorMessage( LPCTSTR lpcszMessage );

BOOL AheHasSameSpatialReference( IGeometryPtr ipGeom1, IGeometryPtr ipGeom2 );

/**
 * @brief ウィンドウのテキストを取得する
 *
 * @param hWnd		[in]	ウィンドウハンドル
 *
 * @return CString
 */
CString AheGetWindowText( HWND hWnd );

/**
 * @brief 現在の日付と時刻を文字列で返す
 *
 * @param lpSystemTime	[in,optional]	指定するとその時間を文字列にして返す（デフォルト：NULL）
 *
 * @return CString
 */
CString AheGetNow( LPSYSTEMTIME lpSystemTime = NULL );

/**
 * @brief VARIANT -> CString 変換
 *
 * @param va			[in]	変換したい値
 *
 * @return CString
 */
CString AheVariant2Str( const CComVariant &va );

/**
 * 文字列の中からキーワードと文字列値を取得し、数値に変換して返す
 *
 * @param CString&		[in/out] 文字列  
 * @param CString		[in]	 文字列値
 * @param LPCTSTR		[in]	 キーワード
 * @param INT&			[out]	 数値の結果
 * @param INT			[in]	 0:整数 or 1:小数
 *
 * @retval TURE 正常な数値
 * @retval FALSE 異常な数値
 */
BOOL AheGetKeywordValue( CString&, CString, LPCTSTR, CString&, INT );
