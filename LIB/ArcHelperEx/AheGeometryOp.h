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

#ifndef ARCHELPEREX_AHEGEOMETRYOP_H__
#define ARCHELPEREX_AHEGEOMETRYOP_H__

#include "define_enum.h"

typedef struct tagSEGMENTITEM
{
	esriGeometryType geometryType;
	LONG lPartID;
	LONG lSegmentID;
	BOOL bIsLastSegment;
	BOOL bIsFirstSegment;
	DOUBLE dFromX;
	DOUBLE dFromY;
	DOUBLE dToX;
	DOUBLE dToY;
	IPointPtr ipFromPoint;
	IPointPtr ipToPoint;
	_ISegmentPtr ipSegment;

} SEGMENTITEM;

/// エリアポイントを取得する
IPointPtr AheGetAreaPoint( IGeometry* ipGeom );

/**
 * @brief 始点を取得する
 * @param ipGeom [in] 始点を取得する形状
 * @retval NULL     ipGeomがICurvePtrに変換できない形状か、始点がない形状
 * @retval NULL以外 始点位置を保持するIPointPtr
 */
IPointPtr AheGetFromPoint( IGeometry* ipGeom );

/**
 * @brief 終点を取得する
 * @param ipGeom [in] 終点を取得する形状
 * @retval NULL     ipGeomがICurvePtrに変換できない形状か、終点がない形状
 * @retval NULL以外 終点位置を保持するIPointPtr
 */
IPointPtr AheGetToPoint( IGeometry* ipGeom );

/**
 * @brief 端点同士の接続点を取得する
 *
 * 端点同士で接続している場合のみ限定です。\n
 * 戻り値は、常にipGeom1上の点です（ポイントの場合は参照であることに注意）。\n
 * 接続していない場合はNULLが返ります。\n
 * IPointCollection又はIPointをサポートしているタイプであれば
 * 動作します。\n
 * 複数存在する場合は最初に見つけたほうを返します。
 * @param ipGeom1 [in] 比較形状１
 * @param ipGeom2 [in] 比較形状２
 * @return IPointPtr
 */
IPointPtr AheGetConnectPoint( IGeometry* ipGeom1, IGeometry* ipGeom2 );

/// WKSPointを作成する
WKSPoint MakeWKSPoint( IPoint* ipPoint );

/// WKSPointを作成する
WKSPoint MakeWKSPoint( const double& x, const double& y );
/////////////////////////////////////////////////////////////////////////////
// ジオメトリ変換等
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief IEnvelope -> IPolygon 変換
 *
 * @note 変換されたポリゴンには空間参照が設定されていません
 *sAhe
 * @param ipEnv				[in]			IEnvelopePtr
 *
 * @return IPolygonPtr
 */
IPolygonPtr AheEnvelope2Polygon( IEnvelopePtr ipEnv );

/**
 * @brief マルチポリゴンからポリゴンのセットを取得する
 *
 * @note 例えば、島ポリゴンはドーナツと島の2つが返ります
 * @note マルチポリゴンではない場合は、1つだけ返ります
 *
 * @ipPolygon				[in]			マルチポリゴン
 *
 * @retrn _ISetPtr
 */
_ISetPtr AheConvertMultiPolygon2Polygons( IPolygonPtr ipPolygon );

/**
 * @brief ポリゴンをポリラインに変換する
 * 
 * @note ポリゴンのラインを全てポリラインに変換したジオメトリを返します。
 * @note 複数の Interior/Exterior 対応済み。
 *
 * @param ipPolygon			[in]			変換元ポリゴン
 *
 * @return IGeometyPtr
 */
IGeometryPtr AheConvertPolygon2Polyline( IPolygonPtr ipPolygon );

/**
 * @brief IGeometry -> list<SEGMENTITEM> 変換
 *
 * @param ipGeometry		[in]			IGeometryPtr
 * @param listSegments		[out]			セグメントコレクション
 *
 * @retval TRUE	成功
 * @retcal FALSE 失敗
 */
BOOL AheGeometry2SegmentItems( IGeometryPtr ipGeometry, std::list<SEGMENTITEM>& listSegments );

/////////////////////////////////////////////////////////////////////////////
// 長さ、角度など
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief 3点のなす角（0<=x<=180）を求める
 *
 * @param ipPoint1			[in]			IPoint のインスタンス
 * @param ipPointCenter		[in]			IPoint のインスタンス
 * @param ipPoint2			[in]			IPoint のインスタンス
 *
 * @return 3点のなす角度
 */
DOUBLE AheGetAngle( IPointPtr ipPoint1, IPointPtr ipPointCenter, IPointPtr ipPoint2 );

/** 
 * @brief ラインの接続角を求める
 *
 * @param ipCurve1			[in]			ICurvePtr
 * @param ipCurve2			[in]			ICurvePtr
 *
 * @return 接続角（失敗した場合は 0 がかえるので注意！！）
 */
DOUBLE AheGetAngle( ICurvePtr ipCurve1, ICurvePtr ipCurve2 );

/**
 * @brief 内積を求める（ipPoint1->ipPoint2 , ipPoint3->ipPoint4）
 *
 * @param ipPoint1			[in]			IPoint のインスタンス
 * @param ipPoint2			[in]			IPoint のインスタンス
 * @param ipPoint3			[in]			IPoint のインスタンス
 * @param ipPoint4			[in]			IPoint のインスタンス
 *
 * @return 内積
 */
DOUBLE AheGetInnerProduct( IPointPtr ipPoint1, IPointPtr ipPoint2, IPointPtr ipPoint3, IPointPtr ipPoint4 );

/**
 * @brief ベクトル長を求める（ipPoint1->ipPoint2）
 *
 * @param ipPoint1			[in]			IPoint のインスタンス
 * @param ipPoint2			[in]			IPoint のインスタンス
 *
 * @return ２点のベクトル長
 */
DOUBLE AheGetLength( IPointPtr ipPoint1, IPointPtr ipPoint2 );

/**
 * @brief 距離（m）を求める
 *
 * @param ipP1			[in]			IPoint のインスタンス
 * @param ipP2			[in]			IPoint のインスタンス
 *
 * @return ２点の距離（m）
 */
double AheGetMeterLength( IPointPtr& ipP1, IPointPtr& ipP2 );

/**
 * @brief 正規化座標距離を求める
 *
 * @param ipP1			[in]			IPoint のインスタンス
 * @param ipP2			[in]			IPoint のインスタンス
 *
 * @return ２点の正規化座標距離
 */
double AheGetMeshXYLength( IPointPtr& ipP1, IPointPtr& ipP2 );

/////////////////////////////////////////////////////////////////////////////
// RelationalOperator系
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief ジオメトリの関連をチェックする
 *
 * @note 基本ジオメトリ、比較ジオメトリ共にクローンを作成して比較します
 * @note ipSpRel は、同一フィーチャクラスのジオメトリ同士の場合はそのフィーチャクラスのSpatialReferenceで
 * @note 良いですが、別フィーチャクラス同士の場合は片方のSpatialReferenceを渡すと正しい判定が帰ってきません。
 * @note その場合は、IMap::get_SpatialReference() で取得できるものを使用するしかないのですが、その場合荒い
 * @note SpatialReferenceを持っているレイヤが読み込まれていると全ての最大公約数のSpatialReferenceが使用され
 * @note ることになりますので注意が必要です。
 * @note relOption は複数指定ができます
 * @note see ジオデータベース設計ガイド - Modiling Our World - p.110
 *
 * @param ipBaseGeom		[in]			基本ジオメトリ
 * @param ipTestGeom		[in]			比較ジオメトリ
 * @param relOption			[in,optional]	sindyeSpatialRel
 * @param ipSpRel			[in,optional]	地理参照
 *
 * @return sindyeSpatialRel
 */
sindyeSpatialRel AheTestGeometryRelationship( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, sindyeSpatialRel relOption = (sindyeSpatialRel)127, ISpatialReferencePtr ipSpRel = NULL );

/**
 * @brief ジオメトリ同士が接触しているかどうか
 *
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			基本ジオメトリ
 * @param ipTestGeom		[in]			比較ジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval TRUE 接触している
 * @retval FALSE 接触していない
 */
BOOL AheIsTouch( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsTouch( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, sindyeSpatialRelTouch, ipSpRel ) & sindyeSpatialRelTouch ) ? TRUE : FALSE;
}

/**
 * @brief ジオメトリ同士が重複しているかどうか
 *
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			基本ジオメトリ
 * @param ipTestGeom		[in]			比較ジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval TRUE 重複している
 * @retval FALSE 重複していない
 */
BOOL AheIsOverlap( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsOverlap( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, sindyeSpatialRelOverlap, ipSpRel ) & sindyeSpatialRelOverlap ) ? TRUE : FALSE;
}

/**
 * @brief ジオメトリ同士が重複しているかどうか
 *
 * @note 同一形状、含む、含まれるの時も TRUE を返します
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			基本ジオメトリ
 * @param ipTestGeom		[in]			比較ジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval TRUE 重複している
 * @retval FALSE 重複していない
 */
BOOL AheIsOverlap2( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsOverlap2( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, (sindyeSpatialRel)(sindyeSpatialRelOverlap|sindyeSpatialRelEqual|sindyeSpatialRelWithin|sindyeSpatialRelContain), ipSpRel ) ) ? TRUE : FALSE;
}

/**
 * @brief ジオメトリ同士がクロスしているかどうか
 *
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			基本ジオメトリ
 * @param ipTestGeom		[in]			比較ジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval TRUE クロスしている
 * @retval FALSE クロスしていない
 */
BOOL AheIsCross( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsCross( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, sindyeSpatialRelCross, ipSpRel ) & sindyeSpatialRelCross ) ? TRUE : FALSE;
}

/**
 * @brief 基本ジオメトリが比較ジオメトリに含まれるかどうか
 *
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			基本ジオメトリ
 * @param ipTestGeom		[in]			比較ジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval TRUE 基本ジオメトリが比較ジオメトリに含まれる
 * @retval FALSE 基本ジオメトリが比較ジオメトリに含まれない
 */

BOOL AheIsWithin( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsWithin( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, sindyeSpatialRelWithin, ipSpRel ) & sindyeSpatialRelWithin ) ? TRUE : FALSE;
}

/**
 * @brief 基本ジオメトリが比較ジオメトリに含まれるかどうか（同一形状含む）
 *
 * @note AheIsWithin は同一形状の場合は FALSE になりますが、AheIsWithin2 は同一形状の時も TRUE を返します
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			基本ジオメトリ
 * @param ipTestGeom		[in]			比較ジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval TRUE 基本ジオメトリが比較ジオメトリに含まれる
 * @retval FALSE 基本ジオメトリが比較ジオメトリに含まれない
 */
BOOL AheIsWithin2( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsWithin2( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, (sindyeSpatialRel)(sindyeSpatialRelWithin|sindyeSpatialRelEqual), ipSpRel ) ) ? TRUE : FALSE;
}

/**
 * @brief 基本ジオメトリが比較ジオメトリを含むか
 *
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			基本ジオメトリ
 * @param ipTestGeom		[in]			比較ジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval TRUE 基本ジオメトリが比較ジオメトリを含む
 * @retval FALSE 基本ジオメトリが比較ジオメトリを含まない
 */
BOOL AheIsContain( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsContain( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, sindyeSpatialRelContain, ipSpRel ) & sindyeSpatialRelContain ) ? TRUE : FALSE;
}

/**
 * @brief 基本ジオメトリが比較ジオメトリを含むか（同一形状含む）
 *
 * @note AheIsContain は同一形状の場合は FALSE になりますが、AheIsContain2 は同一形状の時も TRUE を返します
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			基本ジオメトリ
 * @param ipTestGeom		[in]			比較ジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval TRUE 基本ジオメトリが比較ジオメトリを含む
 * @retval FALSE 基本ジオメトリが比較ジオメトリを含まない
 */
BOOL AheIsContain2( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsContain2( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, (sindyeSpatialRel)(sindyeSpatialRelContain|sindyeSpatialRelEqual), ipSpRel ) ) ? TRUE : FALSE;
}

/**
 * @brief 基本ジオメトリが比較ジオメトリを含むか（同一形状含む）
 *
 * @note 基本機能は AheIsContain と同じですが、AheTestGeometryRelationship で sindyRelNone が返って来た場合
 *		 に PointCollection に変換して比較します。
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			基本ジオメトリ
 * @param ipTestGeom		[in]			比較ジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval TRUE 基本ジオメトリが比較ジオメトリを含む
 * @retval FALSE 基本ジオメトリが比較ジオメトリを含まない
 */
BOOL AheIsContain3( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );

/**
 * @brief 基本ジオメトリが比較ジオメトリを含むか（同一形状含む）
 *
 * @note 基本機能は AheIsContain と同じですが、ipTestGeom が ポイント 以外の時は セグメント 毎に
 *		 分解して比較します。
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			基本ジオメトリ
 * @param ipTestGeom		[in]			比較ジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval TRUE 基本ジオメトリが比較ジオメトリを含む
 * @retval FALSE 基本ジオメトリが比較ジオメトリを含まない
 */
BOOL AheIsContain4( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );

/**
 * @brief ジオメトリ同士が分離しているかどうか
 *
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			基本ジオメトリ
 * @param ipTestGeom		[in]			比較ジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval TRUE 分離している
 * @retval FALSE 分離していない
 */
BOOL AheIsDisjoint( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsDisjoint( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, sindyeSpatialRelDisjoint, ipSpRel ) & sindyeSpatialRelDisjoint ) ? TRUE : FALSE;
}

/**
 * @brief ジオメトリ同士が等しいかどうか
 * 
 * @note この関数はIClone::Equalsを使用します。
 * また、ポイント同士の比較の場合には bug 5617 回避の為に
 * AheIsExactEqualとの結果と比較し、異なる場合にのみ空間参照
 * にあわせて再度比較した結果を返します。
 * 空間参照が与えられていない場合には双方の空間参照のより細かい
 * 方を使用します。
 *
 * @param ipBaseGeom		[in]			基本ジオメトリ
 * @param ipTestGeom		[in]			比較ジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval TRUE 等しい
 * @retval FALSE 等しくない
 */
BOOL AheIsEqual( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );

/**
 * @beif データベース内に格納した場合に同一点かどうかを検証する
 *
 * @note この関数は、空間参照を使用してDB格納値（整数値）に変換し、その値が同じかどうかで
 * 同一判定します。現在はポイントしかサポートしません。
 * @warning 空間参照が同一かどうかは見ません。最初の引数のジオメトリのものを使用します。
 * @see bug 5617
 */
BOOL AheIsDBEqual( IPointPtr ipP1, IPointPtr ipP2 );

/**
 * @brief ジオメトリ同士が等しいかどうか
 * 
 * @note この関数は独自比較により厳密に比較します。
 * 比較的緩い比較を行いたい場合はAheIsEqualを、
 * さらに緩い比較を行いたい場合はIRelationalOperator::Equalsを
 * 使用してください。
 *
 * @param ipBaseGeom		[in]			基本ジオメトリ
 * @param ipTestGeom		[in]			比較ジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval TRUE 等しい
 * @retval FALSE 等しくない
 */
BOOL AheIsExactEqual( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );

/////////////////////////////////////////////////////////////////////////////
// TopologicalOperator系関数
/////////////////////////////////////////////////////////////////////////////
/**
 * ジオメトリの Union を取る
 *
 * @note ipBaseGeom、ipAddGeom はコピーを作成してから演算します
 * @note 地理参照をあわせることは非常に危険なので、この関数については ipSpRef が NULL の場合は地理参照はあわせません
 *
 * @param ipBaseGeom		[in]			Union のベースとなるジオメトリ
 * @param ipAddGeom			[in]			ipBaseGeom と Union を取りたいジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval IGeometryPtr Union 後のジオメトリ
 * @retval NULL Union 失敗
 */
IGeometryPtr AheUnion( IGeometryPtr ipBaseGeom, IGeometryPtr ipAddGeom, ISpatialReferencePtr ipSpRef = NULL );

/**
 * ポリライン Union を取る
 *
 * @note ipBaseGeom、ipAddGeom はコピーを作成してから演算します
 * @note AheUnionは必ずSimplifyをかけるため、自己交差ポリラインはFROM、TOがおかしくなります。
 * @note 特にリンク列の表示ではマルチパートになると矢印がおかしなところにでますので、その場合は
 * @note AheUnionNetworkを使用してください。
 * @note この関数ではSnapToSpatialReferenceは行いません。
 *
 * @param ipBaseGeom		[in]			Union のベースとなるジオメトリ
 * @param ipAddGeom			[in]			ipBaseGeom と Union を取りたいジオメトリ
 * @param ipSpRel			[in]			地理参照
 * 
 * @retval IGeometryPtr Union 後のジオメトリ
 * @retval NULL Union 失敗
 */
IGeometryPtr AheUnionNetwork( IPolylinePtr ipBaseGeom, IPolylinePtr ipAddGeom, ISpatialReferencePtr ipSpRef );

/**
 * ジオメトリ同士を合体する（マルチレンダラ用）
 *
 * @note ipBaseGeom、ipAddGeom はコピーを作成してから演算します
 * @note 地理参照をあわせることは非常に危険なので、この関数については ipSpRef が NULL の場合は地理参照はあわせません
 *
 * @param ipBaseGeom		[in]			Union のベースとなるジオメトリ
 * @param ipAddGeom			[in]			ipBaseGeom と Union を取りたいジオメトリ
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval IGeometryPtr セグメント追加後のジオメトリ
 * @retval NULL  失敗
 */
IGeometryPtr AheUnionSegments( IGeometryPtr ipBaseGeom, IGeometryPtr ipAddGeom, ISpatialReferencePtr ipSpRef = NULL );

IGeometryPtr AheDifference( IGeometryPtr ipBaseGeom, IGeometryPtr ipAddGeom, ISpatialReferencePtr ipSpRef = NULL );

/**
 * ジオメトリの Intersect を取る
 *
 * @note ipBaseGeom、ipAddGeom はコピーを作成してから演算します
 *
 * @param ipBaseGeom		[in]			Intersect のベースとなるジオメトリ（ipSpRef が NULL の場合はこれから地理参照取得）
 * @param ipAddGeom			[in]			ipBaseGeom と Intersect を取りたいジオメトリ
 * @param enumDimension		[in,optional]	Intersect 後のジオメトリの次元（デフォルト：esriGeometry1Dimension）
 * @param ipSpRel			[in,optional]	地理参照
 * 
 * @retval IGeometryPtr Intersect 後のジオメトリ
 * @retval NULL Intersect 失敗
 */
IGeometryPtr AheIntersect( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, esriGeometryDimension enumDimension = esriGeometry1Dimension, ISpatialReferencePtr ipSpRef = NULL );

/////////////////////////////////////////////////////////////////////////////
// 複合演算関数
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief 交差あるいは接触しているポイントを取得する
 *
 * @note ITopologicalOperator::Intersects() では取得できない場所でも厳密に判定します
 *
 * @param ipBaseGeometry	[in]			比較元ジオメトリ
 * @param ipTestGeometry	[in]			比較先ジオメトリ
 * @param iMode				[in,optional]	判定モード（0=Cross 1=Touch 2=Both, デフォルト：2）
 * @param bIsDisjoint		[in,optional]	判定モードによらず、Disjointかどうかを返す（デフォルト：NULL）
 */
IGeometryPtr AheGetCrossOrTouchPoints( IGeometryPtr ipBaseGeometry, IGeometryPtr ipTestGeometry, INT iMode = 2, BOOL* bIsDisjoint = NULL );

/**
 * @brief ポイントがポリライン及びポリゴンフィーチャの From ポイントであるかどうかの判定
 *
 * @see AheIsFromPoint
 *
 * @param ipPointGeom		[in]			ポイントジオメトリ
 * @param ipCurveFeature	[in]			ポリライン及びポリゴンフィーチャ
 * @param ipSpRef			[in,optional]	ISpatialReferencePtr
 *
 * @retval TRUE From ポイントである
 * @retval FALSE From ポイントではない、又はポリライン及びポリゴンフィーチャではない
 */
BOOL AheIsFromPoint( IGeometryPtr ipPointGeom, IFeaturePtr ipCurveFeature, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief ポイントがポリライン及びポリゴンの From ポイントであるかどうかの判定
 *
 * @see AheIsEqual
 *
 * @param ipPointGeom       [in]			ポイントジオメトリ
 * @param ipCurve		    [in]			ポリライン及びポリゴン
 * @param ipSpRef			[in,optional]	ISpatialReferencePtr
 *
 * @retval TRUE From ポイントである
 * @retval FALSE From ポイントではない、又はポリライン及びポリゴンではない
 */
BOOL AheIsFromPoint( IPointPtr ipPoint, ICurvePtr ipCurve, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief ポイントがポリライン及びポリゴンフィーチャの To ポイントであるかどうかの判定
 *
 * @see AheIsToPoint
 *
 * @param ipPointGeom		[in]			ポイントジオメトリ
 * @param ipCurveFeature	[in]			ポリライン及びポリゴンフィーチャ
 * @param ipSpRef			[in,optional]	ISpatialReferencePtr
 *
 * @retval TRUE To ポイントである
 * @retval FALSE To ポイントではない、又はポリライン及びポリゴンフィーチャではない
 */
BOOL AheIsToPoint( IGeometryPtr ipPointGeom, IFeaturePtr ipCurveFeature, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief ポイントがポリライン及びポリゴンの To ポイントであるかどうかの判定
 *
 * @see AheIsEqual
 *
 * @param ipPointGeom       [in]			ポイントジオメトリ
 * @param ipCurve			[in]			ポリライン及びポリゴン
 * @param ipSpRef			[in,optional]	ISpatialReferencePtr
 *
 * @retval TRUE To ポイントである
 * @retval FALSE To ポイントではない、又はポリライン及びポリゴンではない
 */
BOOL AheIsToPoint( IPointPtr ipPoint, ICurvePtr ipCurve, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief ポイントがポリライン及びポリゴンフィーチャの端点であるかどうかの判定
 *
 * @see AheIsEndPoint
 *
 * @param ipPointGeom       [in]			ポイントジオメトリ
 * @param ipCurveFeature	[in]			ポリライン及びポリゴンフィーチャ
 * @param ipSpRef			[in,optional]	ISpatialReferencePtr
 *
 * @retval TURE 端点
 * @retval FALSE 端点以外
 */
BOOL AheIsEndPoint( IGeometryPtr ipPointGeom, IFeaturePtr ipCurveFeature, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief ポイントがポリライン及びポリゴンの端点であるかどうかの判定
 *
 * @see AheIsFromPoint
 * @see AheIsToPoint
 *
 * @param ipPoint			[in]			ポイント
 * @param ipCurve			[in]			ポリライン及びポリゴン
 * @param ipSpRef			[in,optional]	ISpatialReferencePtr
 *
 * @retval TURE 端点
 * @retval FALSE 端点以外
 */
BOOL AheIsEndPoint( IPointPtr ipPoint, ICurvePtr ipCurve, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief ジオメトリの端点同士が接触しているかどうか
 *
 * @note 対象はポリラインのみ
 *
 * @param ipBaseCurve		[in]			基本ジオメトリ
 * @param ipTestCurve		[in]			比較ジオメトリ
 * @param ipSpRef			[in,optional]	地理参照
 * @param ipTouchPoint		[out,optional]	NULLじゃなければ、TOUCH したポイントを返す（From優先）
 *
 * @retval TRUE 接触している
 * @retval FALSE 接触していない
 */
BOOL AheIsTouchEndPoint( ICurvePtr ipBaseCurve, ICurvePtr ipTestCurve, ISpatialReferencePtr ipSpRef = NULL, IPoint** ipTouchPoint = NULL  );

/**
 * @brief ポイントがジオメトリ上の構成点かどうかの判定
 *
 * @param ipPoint			[in]			比較ポイント 
 * @param ipGeom			[in]			ジオメトリ
 * @param ipSpRef			[in,optional]	ISpatialReferencePtr
 * @param pIndex			[out,optional]	ヒットしたインデックス番号
 *
 * @retval TRUE 構成点である
 * @retval FALSE 構成点ではない
 */
BOOL AheIsVertexPoint( IPointPtr ipPoint, IGeometryPtr ipGeom, ISpatialReferencePtr ipSpRef = NULL, LONG* pIndex = NULL );

/**
 * @brief ジオメトリが自己交差しているかどうかの判定
 *
 * @note 第 2 引数に TRUE を指定すると、完全に交差している部分は自己交差とは判定しません
 *
 * @param ipGeometry		[in]			比較ジオメトリ
 * @param bIsNetworkFeature	[in,optional]	道路系のジオメトリかどうか（デフォルト：TRUE）
 *
 * @retval TRUE 自己交差している
 * @retval FALSE 自己交差していない
 */
BOOL AheIsOneselfCross( IGeometryPtr ipGeometry, BOOL bIsNetworkFeature = TRUE );

/**
 * @brief ジオメトリに微小セグメントが存在するかどうかの判定
 *
 * @param ipGeometry		[in]			比較ジオメトリ
 * @param ipSpRef			[in,optional]	最小距離を計算するための空間参照（デフォルト：NULL）
 * @param bFixGeometry		[in,optional]	微小セグメントを発見した場合に修復するかどうか（デフォルト：FALSE）
 */
BOOL AheHasMicroSegment( IGeometryPtr ipGeometry, ISpatialReferencePtr ipSpRef = NULL, BOOL bFixGeometry = FALSE );

/**
 * @brief ポイントがセグメント上に乗っているかどうか
 *
 * @note 渡された引数には一切変更を加えません
 * @note この関数は以下のアルゴリズムで判定を行います
 * @note 
 * @note 1. ポイントとセグメントの距離を計算して、空間参照の最小単位長より長ければ FALSE
 * @note                 ↓
 * @note 2. 比較ポイントから一番近いところにポイントを追加した後 SnapToSpatialReference をかける。
 * @note    その結果結果移動した追加点と、SnapToSpatialReference した比較ポイントを比べ、同一でな
 * @note    ければ FALSE、同一なら TRUE
 * @note
 * @note この関数は、形状確定後にポイントがどこに移動するかを考えて作成してありますので、安全です。
 * @note IRelationalOperator の Touches や Contains で引っかからないことがあるので、こちらを
 * @note 使用して下さい。
 *
 * @param ipBaseSegment		[in]			ベースセグメント
 * @param ipTestPoint		[in]			比較ポイント
 * @param bExeptEndPoint	[in,optional]	両端点を除く場合はTRUEを指定（デフォルト：FALSE）
 *
 * @retval TRUE 乗っている
 * @retval FALSE 乗っていない
 */
BOOL AhePointIsOnSegment( _ISegmentPtr ipBaseSegment, IPointPtr ipTestPoint, ISpatialReferencePtr ipSpRef, BOOL bExeptEndPoint = FALSE );

/**
 * @brief ポイントがジオメトリ上に乗っているかどうか
 *
 * @see AhePointIsOnSegment
 *
 * @note 
 * @param ipBaseGeometry	[in]			ベースジオメトリ
 * @param ipTestPoint		[in]			比較ポイント
 * @param ipSpRef			[in,optional]	空間参照（NULLの場合はベースのものを使用。誤差割り出しにしか使用しません。）
 * @param bExeptVertex		[in,optional]	構成点を除く場合はTRUEをしてい（デフォルト：FALSE）
 *
 * @retval TRUE 乗っている
 * @retval FALSE 乗っていない
 */
BOOL AhePointIsOnGeometry( IGeometryPtr ipBaseGeometry, IPointPtr ipTestPoint, ISpatialReferencePtr ipSpRef = NULL, BOOL bExeptVertex = FALSE );

/**
 * @brief ベースのジオメトリ上にのっているテストのジオメトリの構成点を返す
 *
 * @see AhePointIsOnGeometry
 *
 * @param ipBaseGeometry	[in]			ベースジオメトリ
 * @param ipTestGeometry	[in]			テストジオメトリ
 * @param ipSpRef			[in,optional]	空間参照（NULLの場合はベースのものを使用。誤差割り出しにしか使用しません。）
 *
 * @retval IPointCollectionPtr（Multipoint）
 * @retval NULL	一つも乗っていない
 */
IPointCollectionPtr AheGetOnGeometryPoints( IGeometryPtr ipBaseGeometry, IGeometryPtr ipTestGeometry, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief ベースのジオメトリ上に追加用構成点が乗っているのであれば、構成点を追加する
 *
 * @note ジオメトリ上に点が存在するかどうかの判定は、AhePointIsOnGeometry を使用しています
 * @see AhePointIsOnGeometry
 *
 * @param ipBaseGeometry	[in]			ベースジオメトリ
 * @param ipAddVertexPoint	[in]			追加構成点用ポイント
 * @param ipSpRef			[in,optional]	空間参照（NULLの場合はベースのものを使用。誤差割り出しにしか使用しません。）
 *
 * @retval TRUE 追加した
 * @retval FALSE 追加しなかった
 */
BOOL AheAddVertexIfOnGeometry( IGeometryPtr ipBaseGeometry, IPointPtr ipAddVertexPoint, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief ベースのジオメトリ上に追加用構成点群が乗っているのであれば、構成点を追加する
 *
 * @see AheAddVertexIfOnGeometry
 *
 * @param ipBaseGeometry	[in]			ベースジオメトリ
 * @param ipAddVertexPoints	[in]			追加構成点用ポイント群
 * @param ipSpRef			[in,optional]	空間参照（NULLの場合はベースのものを使用。誤差割り出しにしか使用しません。）
 *
 * @retval TRUE 追加した
 * @retval FALSE 追加しなかった
 */
BOOL AheAddVertexesIfOnGeometry( IGeometryPtr ipBaseGeometry, IGeometryPtr ipAddVertexPoints, ISpatialReferencePtr ipSpRef = NULL );

/// ポイント移動時にラバーバンド候補となりうるセグメントを返す
ISegmentCollectionPtr AheGetRubberBandSegment( IGeometryPtr ipGeom, IPointPtr ipSplitPoint, ISpatialReferencePtr ipSpRef = NULL );

/**
 *
 * @brief IsKnownSimpleにVARIANT_TRUEを設定
 *
 * @param ipGeometry	[in]	設定する形状
 */
IGeometryPtr AheSetKnownSimple(IGeometryPtr ipGeometry);
/**
 * @brief 隣り合っている点が等しいものを削除(全パート)
 *
 * @see AheRemoveAdjoiningPoint
 *
 * @param ipGeometry	[in]			ジオメトリ
 */
IGeometryPtr AheRemoveAdjoiningPoint(IGeometryPtr ipGeometry);
/**
 * @brief 隣り合っている点が等しいものを削除
 *
 * @see AheRemoveAdjoiningPointProc
 *
 * @param ipGeometry	[in]			ジオメトリ
 */
IGeometryPtr AheRemoveAdjoiningPointProc(IGeometryPtr ipGeometry);

/////////////////////////////////////////////////////////////////////////////
// ネットワーク系チェックで使用する関数
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief 構成点同士が接触していないかどうか調べる
 *
 * ネットワーク系オブジェクト同士の構成点同士の接触判定に使用します（ロードチェック 02320）
 * なので、端点同士の接触は無視します。
 */
bool AheIsTouchVertex( IPointCollectionPtr& ipBaseGeom, IPointCollectionPtr& ipTestGeom, IPointPtr& ipTouch );

/**
 * @brief 端点が接触している形状の第一セグメントとの距離を返します。
 * 
 * ネットワークオブジェクトの接続セグメントの距離比較に使用します（ロードチェック 02322）
 */
double AheGetConnectSegmentCrack( IPointCollectionPtr& ipBaseGeom, IPointCollectionPtr& ipTestGeom );

#endif // ifndef ARCHELPEREX_AHEGEOMETRYOP_H__
