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
 * @file QueryFunctions.h
 * @brief <b>ArcObjectsグローバルヘルパー関数定義ファイル</b>\n
 * このファイルには、主にクエリに関するグローバル関数が定義されています。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _ARCHELPEREX_GLOBALFUNCTIONS_H_
#define _ARCHELPEREX_GLOBALFUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief クエリフィルタ設定用関数
 * インスタンスが作成されていない場合はインスタンスをCLSID_QueryFilterで作成します。
 * インスタンスが作成されていれば、現在の設定を上書きします。
 * 
 * 空間クエリフィルタを作成する場合は、先にAheInitSpatialFilterを使用してください。
 *
 * @return IQueryFilterPtr
 */
IQueryFilterPtr AheInitQueryFilter( IQueryFilter* ipFilter							= NULL,
											LPCTSTR lpcszSubFields					= NULL,
											LPCTSTR lpcszWhereClause				= NULL,... );
/**
 * @brief 空間クエリフィルタ設定用関数
 * インスタンスが作成されていない場合はインスタンスをCLSID_SpatialFilterで作成します。
 * インスタンスが作成されていれば、現在の設定を上書きします。
 * 
 * サブフィールドやWHERE句の設定はAheInitQueryFilterを使用してください。
 *
 * @param ipFilter						[in,optional]	設定するフィルタ。NULLの場合は新規に作成します。（デフォルト：NULL）
 * @param pGeom							[in,optional]	検索エリア（デフォルト：NULL）
 * @param lpcszGeomField				[in,optional]	形状フィールド名（デフォルト：NULL）
 *                                                      必ずIFeatureClass::get_ShapeFieldName()の名称を使用してください
 * @param order							[in,optional]	検索優先順位（デフォルト：esriSearchOrderSpatial）
 * @param SpatialRel					[in,optional]	空間検索方法（デフォルト：esriSpatialRelIntersects）
 * @param lpcszSpatialRelDescription	[in,optional]	空間検索方法指定文字列（デフォルト：NULL）
 *
 * @return ISpatialFilterPtr
 */
ISpatialFilterPtr AheInitSpatialFilter( ISpatialFilter* ipFilter					= NULL,
												IGeometry* pGeom					= NULL,
												LPCTSTR lpcszGeomField				= _T("Shape"),
												esriSearchOrder order				= esriSearchOrderSpatial,
												esriSpatialRelEnum SpatialRel		= esriSpatialRelIntersects,
												LPCTSTR lpcszSpatialRelDescription	= NULL );

/// OBJECTIDを取得する
long AheGetOID( _IRow* ipRow );

/// フィーチャから形状を取得する
IGeometryPtr AheGetShape( IFeature* ipFeature );

/// フィーチャ形状コピーを取得する
IGeometryPtr AheGetShapeCopy( IFeature* ipFeature );

/**
 * @brief <b>矩形->ポリゴン変換</b>\n
 * @return ポリゴン
 */
//IPolygonPtr SINDYLIB_API AheEnvelope2Polygon( IEnvelope* ipEnv ); <- CGeometry::ToPolygon()

/**
 * @brief <b>COMオブジェクトのコピーを作成する</b>\n
 * ICloneインターフェースを持っている必要があります。
 * @param ipUnk	[in]	IUnknown*
 * @return IClonePtr
 */
IClonePtr AheCreateClone( IUnknown* ipUnk );

/**
 * @brief <b>レイヤファイルからレイヤを取得する</b>\n
 * @param lpcszLayerFile	[in]	レイヤファイル（フルパス）
 * @return ILayerPtr
 */
ILayerPtr AheOpenLayerFromFile( LPCTSTR lpcszLayerFile );

/**
 * @brief <b>プロパティセットを比較する</b>\n
 * 比較するプロパティは以下のとおり：\n
 * @li USER
 * @li SERVER
 * @li VERSION
 * @param ipProp1	[in]	プロパティセット１
 * @param ipProp2	[in]	プロパティセット２
 * @return bool
 */
bool AheIsSamePropertySet( IPropertySet* ipProp1, IPropertySet* ipProp2 );

/**
 * @brief <b>セパレータ付き文字列をリストに格納する</b>\n
 * 対応できる形式：str1;str2;str3など
 * @param lpcszTargetStr	[in]	分割したい文字列
 * @param lpcszSep			[in]	セパレータ
 * @param listStr			[out]	分割後のリスト
 */
void AheSepStr2listString( LPCTSTR lpcszTargetStr, LPCTSTR lpcszSep, std::list<CString>& listStr );

/**
 * @brief <b>セパレータ付き文字列をマップに格納する</b>\n
 * 対応できる形式：str1=hoge1;str2=hoge2;str3=hoge3など\n
 * 上記の場合は、lpcszSep1が「;」lpcszSep2が「=」になります。
 * @param lpcszTargetStr	[in]	分割したい文字列
 * @param lpcszSep1			[in]	セパレータ１
 * @param lpcszSep2			[in]	セパレータ２
 * @param mapStr			[out]	分割後のマップ
 */
void AheSepStr2mapString( LPCTSTR lpcszTargetStr, LPCTSTR lpcszSep1, LPCTSTR lpcszSep2, std::map<CString,CString>& mapStr );

/**
 * @brief <b>空間参照の最小単位距離を返す</b>\n
 * @param ipSpRef	[in]	空間参照
 * @retval 0以下	エラー
 * @retval 上記以外	最小単位距離(許容値)
 */
double AheGetXYDomain1UnitLength( ISpatialReference* ipSpRef );

/**
 * @brief PGDB編集モードかどうかを調べる
 * @return PGDB編集モードならtrue
 */
bool AheIsPGDBEditMode();

/**
 * @brief 重複を許可するレイヤかどうか
 * @return 重複許可ならtrue
 */
bool AheIsOverlapableLayer( const CString& layername );

/**
 * @brief SiNDY-cのテーブルに固有のフィールドかどうかを調べる
 * 以下のフィールド名をSiNDY-cフィールドとみなす\n
 * @li ORG_OBJID
 * @li ORG_MODIFIY_DATE
 * @li ORG_OPERATOR
 * @li CHECK_C
 * @param strField		[in]	フィールド名
 * @return 上記何れかならtrue
 */
bool AheIsSindycField( const CString& strField );

// トレース系関数
/**
 * @brief <b>ジオメトリをトレースする</b>\n
 * @param ipGeom		[in]	ジオメトリ
 * @param lpcszHeader	[in]	メッセージ
 */
#ifdef _DEBUG
void AheTraceGeometry( IGeometry* ipGeom, LPCTSTR lpcszHeader );
#else
#define AheTraceGeometry (void*)0;
#endif // ifdef _DEBUG

/**
 * @brief <b>空間参照をトレースする</b>\n
 * @param ipSpRef		[in]	空間参照
 * @param lpcszHeader	[in]	メッセージ
 */
#ifdef _DEBUG
void AheTraceSpatialReference( ISpatialReference* ipSpRef, LPCTSTR lpcszHeader );
void AheTraceSpatialReference( IGeometry* ipGeom, LPCTSTR lpcszHeader );
inline void AheTraceSpatialReference( IGeometry* ipGeom, LPCTSTR lpcszHeader )
{
	if( ipGeom )
	{
		ISpatialReferencePtr ipSpRef;
		ipGeom->get_SpatialReference( &ipSpRef );
		AheTraceSpatialReference( ipSpRef, lpcszHeader );
	}
}
#else
#define AheTraceSpatialReference (void*)0;
#endif // ifdef _DEBUG

/**
 * @brief 高精度の空間参照かどうか
 *
 * @param ipSpRef		[in]	空間参照
 *
 * @return bool
 */
bool AheIsHighPrecision(ISpatialReferencePtr ipSpRef);

/**
 * @brief ハイライト設定を取得する
 * @note　中でレジストリを見に行きます
 *
 * @param strXPath [in] 設定を取得するXMLノードを指すXPath
 * @param nRed [out] RGB()マクロ用の値(赤)
 * @param nGreen [out] RGB()マクロ用の値(緑)
 * @param nBlue [out] RGB()マクロ用の値(青)
 * 
 * @return 設定の取得に成功したらtrue、失敗したらfalse
 */
bool GetHighlightColor( const _bstr_t& strXPath, int& nRed, int& nGreen, int& nBlue );

#endif // _ARCHELPEREX_GLOBALFUNCTIONS_H_
