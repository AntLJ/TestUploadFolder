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
 * @file Feature.h
 * @brief CFeatureクラス定義ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _FEATURE_H_
#define _FEATURE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "Geometry.h"

namespace sindy {
class CErrorInfos;
/**
 * @class CFeature
 * @brief <b>フィーチャクラス</b>\n
 * 全てのフィーチャ系のクラスはこのクラスが基底になります。
 */
class SINDYLIB_API CFeature : public CRow
{
	ROWCLASS_CONSTRUCTOR( CFeature, unknown )
	operator IFeature*() const;
	operator IFeature**(){ Release(); return (IFeature**)&p; }
	//@{ @name IFeatureインターフェース
	IObjectClassPtr GetClass() const;
	IEnvelopePtr GetExtent() const;
	esriFeatureType GetFeatureType() const;
	//@}
	/**
	 * @brief <b>与えられた形状の範囲で平行移動処理を行う</b>\n
	 * 編集ルールを考慮して移動処理を行います。\n
	 * 編集済みチェックは行いません。
	 * @param cGeom			[in]	クエリ形状
	 * @param pairMove		[in]	移動差分
	 * @param emMode		[in]	移動モード
	 * @param cErrInfos		[out]	エラー情報格納コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Move( const CGeometry& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>与えられた形状とのマージを行う</b>\n
	 * 編集済みチェックは行いません。
	 * @param cGeom			[in]	マージ形状
	 * @param cErrObjects	[out]	エラー情報格納コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Merge( const CGeometry& cGeom, CErrorObjects& cErrObjects );
	/**
	 * @brief <b>与えられた形状の範囲にある点の削除を行う</b>\n
	 * 編集済みチェックは行いません。
	 * @param cGeom			[in]	削除形状
	 * @param cErrObjects	[out]	エラー情報格納コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Erase( const CGeometry& cGeom, CErrorObjects& cErrObjects );
	/**
	 * @brief <b>与えられた形状で分割を行う</b>\n
	 * 編集済みチェックは行いません。
	 * 分割形状がPoint/MultiPointの時は、交点とか求めないで使用しているので注意。
	 * ラインとポリゴンの時だけAheGetCrossOrTouchPointsで交点を求めてる。
	 * @param cGeom				[in]	分割形状
	 * @param cSplittedGeoms	[out]	分割されてできた形状群
	 * @param cSplittedPoints	[out]	分割点
	 * @param cErrObjects		[out]	エラー情報格納コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Split( const CGeometry& cGeom, CGeometry& cSplittedGeoms, CGeometry& cSplittedPoints, CErrorObjects& cErrObjects );
	/**
	 * @brief <b>ポリラインを交点群で分割する</b>\n
	 * @param cSplitterPoints	[in]	交点群
	 * @param cSplittedPoints	[out]	分割点
	 * @return 分割後の形状群
	 */
	IGeometryPtr _SplitPolyline( const CGeometry& cSplitterPoints, IPointCollection** ipSplittedPoints=NULL );
	/**
	 * @brief <b>ポリゴンをポリラインで分割する</b>\n
	 * @param cSplitterPoints	[in]	交点群
	 * @param cSplitterGeom		[in]	分割ライン
	 * @return 分割後の形状群
	 */
	IGeometryPtr _SplitPolygon( const CGeometry& cSplitterPoints, const CGeometry& cSplitterGeom );

	/**
	 * @brief <b>現在格納されているフィーチャの存在矩形を返す<b>\n
	 * @param bOriginal              [in,optional] 変更前の形状矩形が欲しい場合はtrue（デフォルト：true）
	 * @param bChanged               [in,optional] 変更後の形状矩形が欲しい場合はtrue（デフォルト：true）
	 * @param bOriginalIsArcOriginal [in,optional] IFeatureChanges::get_OriginalShapeの形状矩形が欲しい場合にはtrue（デフォルト：true）
	 * @note この関数は、例えば編集後の再描画をする場合に変更前・変更後の形状を含んだ矩形が欲しい場合などに使用します。
	 * @warning Store()してしまうとキャッシュは全て破棄されますので、変更前の形状が取得できなくなります。
	 * 変更前の形状を取得する必要がある場合はStore()前にこの関数を呼ぶ必要があります。
	 */
	IEnvelopePtr GetEnvelope( bool bOriginal = true, bool bChanged = true, bool bFeatureChangedOriginal = true ) const;

	/// 空間参照を取得する
	ISpatialReferencePtr GetSpatialReference() const;
};

typedef boost::shared_ptr<CFeature> CSPFeature;

} // namespace sindy

#endif // _FEATURE_H_
