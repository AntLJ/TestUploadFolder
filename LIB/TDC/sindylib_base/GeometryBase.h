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
 * @file Geometry.h
 * CGeometryBaseクラス定義ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _GEOMETRYBASE_H
#define _GEOMETRYBASE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointIterator.h"
#include "GeometryIterator.h"
#include <utility>
#include "EditDef.h"

namespace sindy {
class CEnumSegment;
class CBasicLogicRule;
class CErrorInfosBase;
/**
 * @class CGeometryBase
 * @brief <b>ジオメトリ管理クラス</b>\n
 * 全てのジオメトリクラスはこのクラスが基底になります。\n\n
 * このクラスを継承したクラスを作成する場合、コピーコンストラクタ
 * は継承されないため、コピーコンストラクタに関しては実装しなおす
 * 必要があります。また、その際には必ずInit()を呼んで適切にメンバ
 * を初期化してください。
 * @see operator=
 */
#pragma warning(push)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

class CGeometryBase : public CComWrapper<IGeometry>  
{
public:
	explicit CGeometryBase()
	{
	}
	virtual ~CGeometryBase()
	{
	}

	explicit CGeometryBase( const CGeometryBase& obj )
	{
		*this = obj;
	}

	CGeometryBase( IGeometry* lp )
	{
		*this = lp;
	}
	explicit CGeometryBase( const GUID& guid )
	{
		IGeometryPtr lp;
		if( SUCCEEDED( lp.CreateInstance( guid ) ) )
			*this = lp;
	}
	CGeometryBase& operator=( const CGeometryBase& obj )
	{
		*this = obj.Clone();
		return *this;
	}
	CGeometryBase& operator=( IGeometry* lp )
	{
		AtlComPtrAssign((IUnknown**)&p, lp );
		return *this;
	}
	operator IGeometry*() const
	{
		return (IGeometry*)p;
	}
	
	operator IGeometry*()
	{
		return (IGeometry*)p;
	}
	operator IUnknown*() const { return p; }
	operator IUnknown*() { return p; }

	//@{ @name IGeometry+αインターフェース
	esriGeometryDimension GetDimension() const;
	IEnvelopePtr GetEnvelope() const;
	esriGeometryType GetGeometryType() const;
	bool IsEmpty() const;
	errorcode::sindyErrCode Project( ISpatialReference* newReferenceSystem );
	errorcode::sindyErrCode QueryEnvelope( IEnvelope* outEnvelope ) const;
	errorcode::sindyErrCode SetEmpty();
	errorcode::sindyErrCode SnapToSpatialReference();
	ISpatialReferencePtr GetSpatialReference() const;
	errorcode::sindyErrCode PutRefSpatialReference( ISpatialReference* spatialRef );
	double GetLength() const;
	//@}
	//@{ @name IPointCollectionインターフェース＋ポイントイテレータ
	/**
	 * @brief <b>ジオメトリの総構成点数を取得する</b>\n
	 * 複数のパートがある場合、それぞれの合計を返します。
	 * @return 構成点数
	 */
	long GetPointCount() const;

	/**
	 * @brief <b>ジオメトリの構成点を取得する</b>\n
	 * この関数が返すポイントは全てコピーです。そのため、この関数によって
	 * 得られたポイントに変更を加えても元のジオメトリには変更はありません。\n
	 * キャッシュがある場合とない場合に分ける必要があるため、内部でprivateな
	 * 方を呼んでいます。
	 * @see GetPoint( long, IGeometryPtr&, IPointCollectionPtr& )
	 * <h3>例：</h3>
	 * @code
	 * const CGeometryBase* pGeom = cFeat.GetShape();
	 * for( long i = 0; i < pGeom->GetPointCount(); ++i )
	 * {
	 *     printf( "X=%lf Y=%lf\n"), pGeom->GetPoint(i)->GetX(), pGeom->GetPoint(i)->GetY() );
	 * }
	 * @endcode
	 * @param index	[in]	構成点インデックス番号
	 * @return CPoint
	 */
	CPoint GetPoint( long index ) const;

	/**
	 * @brief <b>ジオメトリの構成点を取得する</b>\n
	 * パートを指定して構成点を取得する以外は GetPoint( long ) と一緒です。
	 * ただし、index はそのパート内でのインデックスになります。
	 * @param part	[in]	パートインデックス番号
	 * @param index	[in]	パート内構成点インデックス番号
	 * @return CPoint
	 */
	CPoint GetPoint( long part, long index ) const;

	/**
	 * @brief <b>構成点のイテレータを取得する</b>\n
	 * IPointCollection::get_EnumVerticesを使用します。
	 * IPointCollectionをサポートしていないジオメトリの場合は、NULLが返ります。その
	 * 場合、内部でエラーが発生しますので、IPointCollectionをサポートしていないジオメトリ
	 * では使用しないでください。
	 * @param queryGeometry	[in,optional]	クエリ形状（デフォルト：NULL）
	 * @return IEnumVertexPtr
	 */
	IEnumVertexPtr GetEnumVertex() const;

	errorcode::sindyErrCode UpdatePoint( long i, IPoint* p );

	typedef PointIterator point_iterator;
	typedef const PointIterator const_point_iterator;
	point_iterator point_begin(){ return PointIterator( (IGeometry*)*this ); }
	point_iterator point_end(){ return PointIterator(); }
	const_point_iterator point_begin() const { return PointIterator( (IGeometry*)*this ); }
	const_point_iterator point_end() const { return PointIterator(); }
	errorcode::sindyErrCode point_unique();
	//@}
	//@{ @name ISegmentCollectionインターフェース＋セグメントイテレータ
	/**
	 * @brief <b>セグメント数を返す</b>\n
	 * ポイント系のジオメトリの場合は、常に-1が返ります。
	 * また、その場合は内部でエラーが発生しますので、ポイント系
	 * のジオメトリについてはこのメソッドは使用しないでください。
	 * @retval 0未満	エラー
	 * @retval 0以上	セグメント数
	 */
	long GetSegmentCount() const;

	/**
	 * @brief <b>セグメントを取得する</b>\n
	 * ポイント系のジオメトリの場合は、常に-1が返ります。
	 * また、その場合は内部でエラーが発生しますので、ポイント系
	 * のジオメトリについてはこのメソッドは使用しないでください。
	 * @retval NULL		エラー
	 * @retval NULL以外	セグメント
	 */
	_ISegmentPtr GetSegment( long index ) const;

	/**
	 * @brief <b>セグメントを追加する</b>\n
	 * インデックス番号を-1にすると、bAddAfterに関係なく常に一番最後に挿入されます。\n
	 * bAddAfterをtrueにすると、指定したインデックス番号のセグメントは追加したセグメントになります。\n
	 * ポイント系のジオメトリについてはこのメソッドは使用しないでください。
	 * @param inSegment		[in]			追加するセグメント
	 * @param lInsertIndex	[in,optional]	挿入するインデックス番号（デフォルト：-1）
	 * @param bAddAfter		[in,optional]	インデックス番号の後に挿入するかどうか（デフォルト：true）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode AddSegment( _ISegment* inSegment, long lInsertIndex = -1, bool bAddAfter = true );

	typedef CEnumSegment const_segment_iterator;
	const_segment_iterator segment_begin( IEnvelope* ipQuery = NULL ) const;
	const_segment_iterator segment_end() const;
	/**
	 * @brief <b>重複セグメントを削除する</b>
	 * Simplify系は使用せずに重複セグメントを削除します。\n
	 * segment_iteratorを使用している場合、この関数によって
	 * イテレータがおかしくなることがあるのでこの関数使用前に
	 * 取得したイテレータはこの関数使用後に使用しないでください。\n
	 * また、セグメントを削除することによりセグメント同士のつながりが
	 * 分断されますが、内部ではその分断場所を修復しません。\n
	 * 分断をちゃんと修復するにはSimplify系を使用する必要がありますが、
	 * 場合によってはマルチパスや島ポリゴンができてしまう場合があるので
	 * 十分に注意してください。
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode segment_unique();
	//@}
	//@{ @name IGeometryCollectionインターフェース＋ジオメトリイテレータ（イテレータは全て参照を返すことに注意）
	/**
	 * @brief <b>パートジオメトリを返す</b>\n
	 * この関数で返るジオメトリはコピーではなく参照なので、
	 * この関数によって取得したジオメトリに対して変更を加えると
	 * 大元のジオメトリに対して変更を加えたことになりますので
	 * 注意してください。
	 * @param Index		[in]	パートインデックス番号
	 * @return IGeometryPtr	パートジオメトリ（参照であることに注意）
	 */
	IGeometryPtr GetPart( long Index ) const;

	long GetPartCount() const;
	typedef GeometryIterator part_iterator;
	typedef const GeometryIterator const_part_iterator;
	part_iterator part_begin(){ return GeometryIterator( (IGeometry*)*this ); }
	part_iterator part_end(){ return GeometryIterator(); }
	const_part_iterator part_begin() const { return GeometryIterator( (IGeometry*)*this ); }
	const_part_iterator part_end() const { return GeometryIterator(); }
	errorcode::sindyErrCode part_unique();
	//@}
	//@{ @name IHitTestインターフェース＋α
	bool HitTest( IPoint* QueryPoint, const double& searchRadius, esriGeometryHitPartType geometryPart, IPoint* hitPoint = NULL, double* hitDistance = NULL, long* hitPartIndex = NULL, long* hitSegmentIndex = NULL, bool* bRightSide = NULL ) const;
	bool HitTestSegment( IPoint* QueryPoint, const double& searchRadius, IPoint* hitPoint = NULL, double* hitDistance = NULL, long* hitPartIndex = NULL, long* hitSegmentIndex = NULL, bool* bRightSide = NULL ) const
	{ return HitTest( QueryPoint, searchRadius, esriGeometryPartBoundary, hitPoint, hitDistance, hitPartIndex, hitSegmentIndex, bRightSide ); }
	bool HitTestPoint( IPoint* QueryPoint, const double& searchRadius, IPoint* hitPoint = NULL, double* hitDistance = NULL, long* hitPartIndex = NULL, long* hitSegmentIndex = NULL, bool* bRightSide = NULL ) const
	{ return HitTest( QueryPoint, searchRadius, esriGeometryPartVertex, hitPoint, hitDistance, hitPartIndex, hitSegmentIndex, bRightSide ); }
	//@}
	//@{ @name IRelationalOperatorインターフェース
	bool Contains( IGeometry* ipGeom ) const;
	bool Crosses( IGeometry* ipGeom ) const;
	bool Disjoint( IGeometry* ipGeom ) const;
	bool Equals( IGeometry* ipGeom, bool bIgnoreOrient = false ) const;
	/**
	 * @brief 点列比較で同じかどうか判定する
	 * 
	 * 点列比較により同じ形状かどうかを判定します。\n
	 * 座標値で直に判定をするため、Store後に同一となる場合や
	 * 点列の向き（ライン）、始終点の違い（ポリゴン）も異なる
	 * 要因として判定されることに注意してください。\n
	 * また、空間参照は無視するため、異なる空間参照のジオメトリは
	 * ほぼ必ず異なる形状として判定されることに注意してください。\n
	 * @warning サポートする形状は IPoint、IPointCollectionのインターフェース
	 * を持つ形状のみであることに注意してください。
	 */
	bool EqualsVertex( IGeometry* ipGeom ) const;
	bool Overlaps( IGeometry* ipGeom ) const;
	bool Touches( IGeometry* ipGeom ) const;
	bool Within( IGeometry* ipGeom ) const;
	//@}
	//@{ @name IProximityOperatorインターフェース
	/**
	 * @brief <b>与えられたジオメトリとの距離を計算する</b>\n
	 * @retval 0未満 エラー
	 * @retval 0以上 距離
	 */
	double ReturnDistance( IGeometry* ipGeom ) const;

	/**
	 * @brief <b>与えられた点から一番近い点を返す</b>\n
	 * 構成点に限らず、辺上でもポイントとして返します。
	 * そのため、厳密にはそのジオメトリと極僅かにずれる場合がある
	 * 事を考慮してください。
	 * 
	 * ipRetPoint を使用する方は、既にあるインスタンスに入れて欲しい場合
	 * に有効です。また、高速化したい場合にも有効です。
	 * @param ipPoint		[in]	比較基点ポイント
	 * @param ipRetPoint	[out]	一番近いポイント（NOT NULL）
	 * @return 一番近いポイント
	 */
	IPointPtr ReturnNearestPoint( IPoint* ipPoint ) const;
	IPointPtr QueryNearestPoint( IPoint* ipPoint, IPoint* ipRetPoint ) const;

	//@{ @name ITopoligicalOpratorインターフェース
	IGeometryPtr Intersect( IGeometry* ipGeom, esriGeometryDimension emGeomDim ) const;
	IGeometryPtr Buffer( const double& Distance ) const;
	bool Simplify( bool force = false );

	//@}
	//@{ @name その他演算系
	/**
	 * @brief <b>ジオメトリの移動処理を行う</b>\n
	 * 与えるジオメトリによって結果が変わります：
	 * @li Polygon、Envelope、Ring : 条件付平行移動（与えられた形状の中のみ平行移動、境目はつなぐか境目のセグメントを移動）
	 * @li Point                   : 構成点の移動（MoveVertexが自動的に呼ばれる）
	 * @li NULL                    : 平行移動
	 * @param ipQueryGeom		[in]	クエリ形状
	 * @param pairMove			[in]	移動距離（マップ単位）
	 * @param emMode			[in]	移動モード
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Move( IGeometry* ipQueryGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode );

	/**
	 * @brief <b>構成点の移動処理を行う</b>\n
	 * クエリ形状内に存在する構成点を移動します。
	 * ポイントフィーチャの場合は単純に平行移動となります。
	 * @param ipQueryGeometry	[in]	クエリ形状
	 * @param pairMove			[in]	移動距離（マップ単位）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode MoveVertex( IGeometry* ipQueryGeometry, const std::pair<double,double>& pairMove );

	/**
	 * @brief <b>構成点を追加する</b>\n
	 * IPolycurve::SplitAtPointを使用して、構成点を追加します。\n
	 * 同一の構成点が存在する場合以外は、必ず構成点が追加されます。そのため、
	 * 渡されたポイントとは完全には一致しません。
	 * @param ipPoint			[in]			追加ポイント
	 * @param pAddPart			[out,optional]	追加されたパート（デフォルト：NULL）
	 * @param pAddIndex			[out,optional]	追加されたインデックス（デフォルト：NULL）
	 * @return true		追加された
	 * @return false	追加されなかった
	 */
	bool AddVertex( IPoint* ipPoint, long* pAddPart = NULL, long* pAddIndex = NULL );

	/**
	 * @brief <b>構成点を追加する</b>\n
	 * この関数は、IPolycurve2::SplitAtPointsを使用して、構成点を追加します。\n
	 * 同一の構成点が存在する場合以外は、必ず構成点が追加されます。そのため、
	 * 渡されたポイントとは完全には一致しません。
	 * @param splitPoints		[in]			追加ポイント列
	 * @param cutoffDistance	[in]			追加許容距離
	 * @param splitInfo			[out,optional]	追加点情報（デフォルト：NULL）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode AddVertex( IEnumVertex* splitPoints, const double& cutoffDistance, IEnumSplitPoint** splitInfo = NULL );

	/**
	 * @brief <b>構成点を追加する</b>\n
	 * IPointCollection::AddPointを使用して、構成点を追加します。\n
	 * 同一の構成点がある場合でも必ず追加されます。
	 * @param ipPoint			[in]			追加ポイント
	 * @param lPart				[in]			追加するパート
	 * @param lIndex			[in]			追加するインデックス
	 * @param bAddBefore		[in,optional]	trueの場合は指定したインデックスの前に追加（デフォルト：false）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode AddVertex( IPoint* ipPoint, long lPart, long lIndex, bool bAddBefore = false );

	/**
	 * @brief <b>ジオメトリの削除処理を行う</b>\n
	 * 与えるジオメトリによって結果が変わります：
	 * @li Polygon, Envelope, Ring : 与えられた形状に入る部分が消去される（全部の形状が消える場合はエラー）
	 * @li Point                   : 構成点の削除（EraseVertexが呼ばれる）
	 * @param ipQueryGeom		[in]	クエリ形状
	 * @retval sindyErr_NoErr					成功
	 * @retval sindyErr_GeometryChangeToNull	ジオメトリ形状が無くなる操作は不可（この場合はフィーチャ自体を消す必要がある）
	 * @retval 上記以外							エラー
	 */
	errorcode::sindyErrCode Erase( IGeometry* ipQueryGeom );

	/**
	 * @brief <b>構成点の削除処理を行う</b>\n
	 * 構成点が無ければ何もしません。\n
	 * ポイントフィーチャの場合はエラーが返ります。
	 * @param ipQueryPoint		[in]	クエリポイント
	 * @retval sindyErr_NoErr					成功
	 * @retval sindyErr_GeometryChangeToNull	ジオメトリ形状が無くなる操作は不可（この場合はフィーチャ自体を消す必要がある）
	 * @retval 上記以外							エラー
	 */
	errorcode::sindyErrCode EraseVertex( IPoint* ipQueryPoint );

	/**
	 * @brief <b>ジオメトリの分割処理を行う</b>\n
	 * @param ipQueryGeom		[in]			クエリ形状
	 * @param ipOtherGeometries	[out,optional]	NULLの場合は構成点のみ追加、そうでない場合は分割形状が入る（デフォルト：NULL）
	 * @param ipTakeOverPoint	[in,optional]	分割する場合に引き継ぐ形状を決めるためのポイント（デフォルト：NULL）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Split( IGeometry* ipQueryGeom, _ISet** ipOtherGeometries = NULL, IPoint* ipTakeOverPoint = NULL );

	/**
	 * @brief <b>ポリラインを点で分割する</b>\n
	 * この関数は、IPolycurve::SplitAtPointを使用します。\n
	 * 分割後にはマルチパートになりますが、SiNDYデータベース内では
	 * ポリラインのマルチパート形状はサポートしていませんので注意してください。
	 * サポートされるジオメトリは以下のとおり：\n
	 * @li Polyline
	 * @param splitPoint		[in]			分割する点
	 * @param newPartIndex		[out,optional]	分割されてできた新しいパートインデックス番号（エラーの場合は-1 デフォルト：NULL）
	 * @param hitIndex			[out,optional]	分割できなかった場合にヒットしたセグメント又は構成点番号を返す（何が返るのかいまいちわからん。エラーの場合は-1 デフォルト：NULL）
	 * @retval false	エラー、又は分割できなかった
	 * @retval true		分割できた
	 */
	bool CGeometryBase::Split( IPoint* splitPoint, long* newPartIndex = NULL, long* hitIndex = NULL );

	/**
	 * @brief <b>ポリラインを点列で分割する</b>\n
	 * この関数は、IPolycurve2::SplitAtPointsを使用します。\n
	 * 分割後にはマルチパートになりますが、SiNDYデータベース内では
	 * ポリラインのマルチパート形状はサポートしていませんので注意してください。
	 * サポートされるジオメトリは以下のとおり：\n
	 * @li Polyline
	 * @param splitPoints		[in]			分割する点列
	 * @param cutoffDistance	[in]			分割許容距離
	 * @param splitInfo			[out,optional]	分割点情報（デフォルト：NULL）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Split( IEnumVertex* splitPoints, const double& cutoffDistance, IEnumSplitPoint** splitInfo = NULL );

	/**
	 * @brief <b>ジオメトリのマージ処理を行う</b>\n
	 * 内部的にstatic関数の方を呼んで処理します。
	 * 基底のIGeometry*がNULLの場合は、渡された形状をコピーして
	 * 基底とします。
	 * @warning この関数を実行すると必ず基底ジオメトリが入れ替わりますので注意してください。
	 * @param ipGeom	[in]	UNION形状
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Union( IGeometry* ipGeom );

	/**
	 * @brief <b>ジオメトリのマージ処理を行う</b>\n
	 * この関数は、内部でITopologicalOperator::ConstructUnionを使用して
	 * マージ処理を行うため、ITopologicalOperator::Unionとは異なり、
	 * 二つの形状が物理的に離れていても大丈夫です。
	 * @param ipGeom1	[in]	UNION形状
	 * @param ipGeom2	[in]	UNION形状
	 */
	static IGeometryPtr Union( IGeometry* ipGeom1, IGeometry* ipGeom2, errorcode::sindyErrCode* pErr = NULL );

	/**
	 * @brief <b>ジオメトリの差分抽出処理を行う</b>\n
	 * 内部的にstatic関数の方を呼んで処理します。
	 * 基底のIGeometry*がNULLの場合は、渡された形状をコピーして
	 * 基底とします。
	 * @warning この関数を実行すると必ず基底ジオメトリが入れ替わりますので注意してください。
	 * @param ipGeom	[in]	SymmetricDifference形状
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SymmetricDifference( IGeometry* ipGeom );

	/**
	 * @brief <b>ジオメトリの差分抽出処理を行う</b>\n
	 * @param ipGeom1	[in]	Base形状
	 * @param ipGeom2	[in]	Difference形状
	 */
	static IGeometryPtr SymmetricDifference( IGeometry* ipGeom1, IGeometry* ipGeom2, errorcode::sindyErrCode* pErr = NULL );

	IGeometryPtr IntersectPoints( IGeometry* ipGeom ) const;

	//@}

	/**
	 * @brief <b>ジオメトリの矩形を取得する</b>\n
	 * @param dXMin	[out]	最小X
	 * @param dYMin	[out]	最小Y
	 * @param dXMax	[out]	最大X
	 * @param dYMax	[out]	最大Y
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetEnvelope( double& dXMin, double& dYMin, double& dXMax, double& dYMax ) const;

	IGeometryPtr Clone() const;

	//@{ @name ジオメトリ変換系
	/**
	 * @brief <b>ジオメトリから与えられた次元のジオメトリを生成する</b>\n
	 * 下記変換以外で次元が同じ場合はコピーを生成します。\n
	 * サポートしている変換は次の通り：\n
	 * @li Ring     -> Polygon
	 * @li Envelope -> Polygon
	 * @li Polygon  -> Polyline
	 * @li Ring     -> Polyline
	 * @li Envelope -> Polyline
	 * @li Path     -> Polyline
	 * @li Polygon  -> Multipoint
	 * @li Ring     -> Multipoint
	 * @li Envelope -> Multipoint
	 * @li Polyline -> Multipoint
	 * @li Path     -> Multipoint
	 * @li Point    -> Multipoint
	 * @param ipGeom	[in]	ジオメトリ
	 * @param emGeomDim	[in]	次元
	 * @retval NULL		エラー
	 * @retval NULL以外	変換後のジオメトリ
	 */
	static IGeometryPtr Convert( IGeometry* ipGeom, esriGeometryDimension emGeomDim );

	/**
	 * @brief <b>ジオメトリからマルチポイントを生成する</b>\n
	 * 与えられたジオメトリがマルチポイントの場合はコピーを生成します。\n
	 * サポートしているジオメトリタイプは以下のとおり：\n
	 * @li Polygon
	 * @li Ring
	 * @li Envelope
	 * @li Polyline
	 * @li Path
	 * @li Point
	 * @param ipGeom	[in]	ジオメトリ
	 * @retval NULL		エラー
	 * @retval NULL以外	変換後のジオメトリ
	 */
	static IGeometryPtr ToMultipoint( IGeometry* ipGeom );

	/**
	 * @brief <b>ジオメトリからポリラインを生成する</b>\n
	 * 与えられたジオメトリがポリラインの場合はコピーを生成します。\n
	 * サポートしているジオメトリタイプは以下のとおり：\n
	 * @li Polygon
	 * @li Ring
	 * @li Envelope
	 * @param ipGeom	[in]	ジオメトリ
	 * @retval NULL		エラー
	 * @retval NULL以外	変換後のジオメトリ
	 */
	static IGeometryPtr ToPolyline( IGeometry* ipGeom );

	/**
	 * @brief <b>ジオメトリからパスを生成する</b>\n
	 * 与えられたジオメトリがパスの場合はコピーを生成します。\n
	 * サポートしているジオメトリタイプは以下のとおり：\n
	 * @li Ring
	 * @li Envelope
	 * @param ipGeom	[in]	ジオメトリ
	 * @retval NULL		エラー
	 * @retval NULL以外	変換後のジオメトリ
	 */
	static IGeometryPtr ToPath( IGeometry* ipGeom );

	/**
	 * @brief <b>ジオメトリからポリゴンを生成する</b>\n
	 * 与えられたジオメトリがポリゴンの場合はコピーを生成します。\n
	 * サポートしているジオメトリタイプは以下のとおり：\n
	 * @li Ring
	 * @li Envelope
	 * @param ipGeom	[in]	ジオメトリ
	 * @retval NULL		エラー
	 * @retval NULL以外	変換後のジオメトリ
	 */
	static IGeometryPtr ToPolygon( IGeometry* ipGeom );

	/**
	 * @brief <b>ジオメトリからリングを生成する</b>\n
	 * 与えられたジオメトリがリングの場合はコピーを生成します。\n
	 * サポートしているジオメトリタイプは以下のとおり：\n
	 * @li Envelope
	 * @param ipGeom	[in]	ジオメトリ
	 * @retval NULL		エラー
	 * @retval NULL以外	変換後のジオメトリ
	 */
	static IGeometryPtr ToRing( IGeometry* ipGeom );
	//@}
	//@{ @name チェック関数
	/**
	 * @brief <b>全てのチェックを行う</b>
	 * @param cRule		[in]	ロジックチェック用ルール
	 * @param cErrInfos	[out]	エラー格納用コンテナ
	 * @param ipChkGeom	[in/optional]	チェック領域
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckLogic( const CBasicLogicRule& cRule, CErrorInfosBase& cErrInfos, IEnvelope* ipQuery = NULL ) const;

	/**
	 * @brief <b>セグメント系チェック</b>\n
	 * この関数は以下のチェックを実行します：
	 * @li 最低セグメント数チェック
	 * @li 微小セグメントチェック
	 * @li セグメントがなす角度チェック
	 * @li 開始・終了のなす角とセグメントの距離チェック
	 * @li 稲妻形状チェック
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckSegments( const CBasicLogicRule& cRule, CErrorInfosBase& cErrInfos, IEnvelope* ipQuery = NULL ) const;

	/**
	 * @brief <b>ポイント系チェック</b>\n
	 * この関数は以下のチェックを実行します：
	 * @li 最低ポイント数チェック（必須）
	 * @li 重複構成点チェック（必須）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckPoints( const CBasicLogicRule& cRule, CErrorInfosBase& cErrInfos, IEnvelope* ipQuery = NULL ) const;
	//@}
#ifdef _DEBUG
	/**
	 * @brief <b>点列をトレースする</b>\n
	 * トレース結果はデバッグウィンドウにのみ表示されます。
	 * リリース版では何もしません。
	 */
	static void Trace( IGeometry* ipGeom, LPCTSTR lpcszHeader = NULL );
	void Trace( LPCTSTR lpcszHeader = NULL ) const { Trace( (IGeometry*)(*this), lpcszHeader );	}
#else
	static void Trace( IGeometry* ){}
	static void Trace( IGeometry* /*ipGeom*/, LPCTSTR/* lpcszHeader = NULL*/ ){}
	void Trace( LPCTSTR/* lpcszHeader = NULL*/ ) const {}
	void Trace() const {}
#endif // ifdef _DEBUG
private:
};
#pragma warning(pop)

/// IRelationalOperator::Containsバグ回避版
bool AheGeometryContains( IGeometry* ipBase, IGeometry* ipTest );

} // sindy

#endif // _GEOMETRYBASE_H
