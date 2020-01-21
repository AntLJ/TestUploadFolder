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

#include "stdafx.h"
#include <exception>
#include <map>
#include <set>
#include <math.h>
#include <WaitCursor.h>
#include "LinkSnapTrace.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI
#include "GlobalFunctions.h"

//////////////////////////////////////////////////////////////////////
// anonymous namespace
//////////////////////////////////////////////////////////////////////

namespace {
/**
 * @brief IPointCollectionの最初または最後のポイントを取得する
 *
 * @param col           [in] IPointCollectionPtr
 * @param first_or_last [in] 最初と最後どちらのポイントを取得するか
 * @return 最初または最後のポイント
 */
IPointPtr GetGeomPoint(IPointCollectionPtr col, bool first_or_last)
{
	IPointPtr ret; // 戻り値

	if( ! col )
		return ret;

	if( first_or_last )
		col->get_Point( 0, &ret );
	else {
		long count = 0;
		col->get_PointCount( &count );
		col->get_Point( count-1, &ret );
	}

	return ret;
}
/**
 * @brief IPointCollectionの最初のポイントを取得する
 *
 * @param col [in] IPointCollectionPtr
 * @return 最初のポイント
 */
IPointPtr GetFirstPoint(IPointCollectionPtr col)
{
	return GetGeomPoint(col, true);
}
/**
 * @brief IPointColletionの最後のポイントを取得する
 *
 * @param col [in] IPointCollectionPtr
 * @return 最後のポイント
 */
IPointPtr GetLastPoint(IPointCollectionPtr col)
{
	return GetGeomPoint(col, false);
}
IPointPtr GetPoint( IFeature* ipFeature, long lIndex )
{
	IGeometryPtr ipGeom;

	ipFeature->get_ShapeCopy( &ipGeom );

	IPointCollectionPtr ipCol = ipGeom;
	IPointPtr ipPoint;

	if( ! ipCol )
		ipPoint = ipGeom;
	else
		ipCol->get_Point( lIndex, &ipPoint );

	return ipPoint;
}

/**
 * @brief 検索条件に一致するオブジェクトが存在するか
 * @bug [Bug 10374]トレース可能なラインデータを、平行移動後（[MANAGEMENT_ID]is not Null)のオブジェクトに限定する
 * @param ipClass [in] フィーチャクラス
 * @param nOID [in] OBJECTID
 * @param lpcszWhere [in] 検索条件
 * @return 一致するオブジェクト存在するかどうか true:存在、false:存在しない
 */
bool findObject(IFeatureClass* ipClass, long nOID, LPCTSTR lpcszWhere)
{
	if ( ! ipClass )
		return false;

	// OBJECTID名取得
	CComBSTR bstrOIDName;
	ipClass->get_OIDFieldName( &bstrOIDName );

	CString strTmp;
	strTmp.Format( _T("%s = %d AND %s"), (CString)bstrOIDName, nOID, lpcszWhere );
	long nCnt = 0;
	if( SUCCEEDED( ipClass->FeatureCount( AheInitQueryFilter(NULL, NULL, strTmp), &nCnt ) ) )
	{
		if ( nCnt > 0 )
			return true;
	}

	return false;
}

/**
 * @brief 指定座標に最も近いフィーチャを求める。
 *
 * @param ipFeatureCache [in] フィーチャキャッシュ。
 * @param ipPoint [in] 座標。
 * @param dTolerance [in] 閾値。
 * @param ipClass [in] フィーチャクラス [Bug 10374]
 * @param lpcszWhere [in] 検索条件 [Bug 10374]
 * @return 最近傍フィーチャ。座標との距離が閾値以下のフィーチャが存在しない場合null。
 */
IFeaturePtr search_nearest(
	IFeatureCache* ipFeatureCache,
	IPoint* ipPoint,
	double dTolerance,
	IFeatureClass* ipClass,
	LPCTSTR lpcszWhere=NULL)
{
	if(! (ipFeatureCache != NULL && ipPoint != NULL ))
		return 0;

	IFeaturePtr ipResultFeature;
	IProximityOperatorPtr ipProximityOperator(ipPoint);
	LONG lCount = 0;
	ipFeatureCache->get_Count( &lCount );

	for(long i = 0; i < lCount; i++) {
		//IFeaturePtr ipFeature(ipFeatureCache->GetFeature(i));
		IFeaturePtr ipFeature;
		ipFeatureCache->get_Feature( i, &ipFeature );

		if ( lpcszWhere && _tcslen(lpcszWhere) > 0 && ipClass )
		{
			// [Bug 10374]トレース可能なラインデータを、平行移動後（[MANAGEMENT_ID]is not Null)のオブジェクトに限定する
			// 検索条件が設定してある時は、OIDと条件で検索して結果がある時のみ処理をするようにする
			// フィーチャキャッシュは属性で絞り込めないため
			long nOID = 0;
			ipFeature->get_OID(&nOID);

			if ( ! findObject(ipClass, nOID, lpcszWhere) )
				continue;
		}

		IGeometryPtr ipGeom;
		ipFeature->get_ShapeCopy( &ipGeom );
		double dDistance = 0.0;
		ipProximityOperator->ReturnDistance(ipGeom, &dDistance);

		if(dDistance < dTolerance) {
			ipResultFeature = ipFeature;
			dTolerance = dDistance;
		}
	}

	return ipResultFeature;
}

/**
 * @brief 直進リンクを求める。
 *
 * @param ipFeatureCache [in] フィーチャキャッシュ。
 * @param ipLastPointCol [in] 最終点列。
 * @param ipClass [in] フィーチャクラス [Bug 10374]
 * @param lpcszWhere [in] 検索条件 [Bug 10374]
 * @return 直進リンク。存在しない場合null。
 */
IFeaturePtr search_straight(
	IFeatureCache* ipFeatureCache,
	IPointCollection* ipLastPointCol,
	IFeatureClass* ipClass,
	LPCTSTR lpcszWhere=NULL)
{
	// 状態検査。
	if(! (ipFeatureCache && ipLastPointCol))
		return 0;

	// 最終点列は２点以上必要。
	long nLastPointCount;
	if(ipLastPointCol->get_PointCount(&nLastPointCount) != S_OK || nLastPointCount < 2)
		return 0;

	// 最終点を求める。
	IPointPtr ipLastPoint;
	ipLastPointCol->get_Point( nLastPointCount - 1, &ipLastPoint );

	// 最終線分の角度を求める。
	ILinePtr ipLastLine(CLSID_Line);
	IPointPtr ipTmpPoint;
	ipLastPointCol->get_Point( nLastPointCount - 2, &ipTmpPoint );
	ipLastLine->PutCoords(ipTmpPoint, ipLastPoint);
	double dLastAngle = 0;
	ipLastLine->get_Angle( &dLastAngle );

	// 準備。
	IFeaturePtr ipResultFeature;
	double dResultAngle = M_PI;

	// 最直線リンクを求める。
	LONG lCount = 0;
	ipFeatureCache->get_Count( &lCount );
	for(long i = 0; i < lCount; i++) {
		IFeaturePtr ipFeature;
		ipFeatureCache->get_Feature(i, &ipFeature);
		if( ipFeature == NULL )
			continue;

		if ( lpcszWhere && _tcslen(lpcszWhere) > 0 && ipClass )
		{
			// [Bug 10374]トレース可能なラインデータを、平行移動後（[MANAGEMENT_ID]is not Null)のオブジェクトに限定する
			// 検索条件が設定してある時は、OIDと条件で検索して結果がある時のみ処理をするようにする
			// フィーチャキャッシュは属性で絞り込めないため
			long nOID = 0;
			ipFeature->get_OID(&nOID);

			if ( ! findObject(ipClass, nOID, lpcszWhere) )
				continue;
		}

		IGeometryPtr ipGeom;
		ipFeature->get_ShapeCopy( &ipGeom );
		IPointCollectionPtr ipPointCol( ipGeom );

		// 点列は２点以上必要。
		long nPointCount;
		if(ipPointCol->get_PointCount(&nPointCount) != S_OK || nPointCount < 2)
			continue;

		// 接続部の角度を求める。
		ILinePtr ipLine(CLSID_Line);
		int iResult1 = 0, iResult2 = 0;
		ipLastPoint->Compare(GetPoint(ipFeature,0), &iResult1);
		ipLastPoint->Compare(GetPoint(ipFeature,nPointCount-1), &iResult2);
		//ipLastPoint->Compare(GetPoint(ipPointCol,0), &iResult1);
		//ipLastPoint->Compare(GetPoint(ipPointCol,nPointCount-1), &iResult2);
		if(iResult1 == 0) {
			ipLine->PutCoords(GetPoint(ipFeature,0), GetPoint(ipFeature,1));
			//ipLine->PutCoords(GetPoint(ipPointCol,0), GetPoint(ipPointCol,1));
		}
		else if(iResult2 == 0) {
			ipLine->PutCoords(GetPoint(ipFeature, nPointCount - 1), GetPoint(ipFeature,nPointCount - 2));
			//ipLine->PutCoords(GetPoint(ipPointCol, nPointCount - 1), GetPoint(ipPointCol,nPointCount - 2));
		}
		else
			continue;

		double dAngle = 0;
		ipLine->get_Angle(&dAngle);
		dAngle = dAngle - dLastAngle;
		while(dAngle < -M_PI) { dAngle += M_PI * 2.0; }
		while(dAngle > M_PI) { dAngle -= M_PI * 2.0; }
		dAngle = ::fabs(dAngle);

		if(dAngle < dResultAngle) {
			dResultAngle = dAngle;
			ipResultFeature = ipFeature;
		}
	}

	return ipResultFeature;
}

/**
 * @brief 指定座標からの距離順にフィーチャを並べる。
 *
 * @param ipFeatureCache [in] フィーチャキャッシュ。
 * @param ipPoint [in] 座標。
 * @param dTolerance [in] 閾値。
 * @param ipConnectionPoint [in] 接続座標。
 * @param rFeatureMap [out] フィーチャ並べ先。
 * @param ipClass [in] フィーチャクラス [Bug 10374]
 * @param lpcszWhere [in] 検索条件 [Bug 10374]
 */
void order_by_distance(
	IFeatureCache* ipFeatureCache,
	IPoint* ipPoint,
	const double dTolerance,
	IPoint* ipConnectionPoint,
	std::multimap<double, IFeaturePtr>& rFeatureMap,
	IFeatureClass* ipClass,
	LPCTSTR lpcszWhere=NULL)
{
	if(! (ipFeatureCache != NULL && ipPoint != NULL && ipConnectionPoint != NULL))
		return;

	IProximityOperatorPtr ipProximityOperator(ipPoint);
	IRelationalOperatorPtr ipRelOp(ipConnectionPoint);
	LONG lCount = 0;
	ipFeatureCache->get_Count( &lCount );

	for(long i = 0; i < lCount; i++) {
		IFeaturePtr ipFeature;
		ipFeatureCache->get_Feature(i,&ipFeature);

		if ( lpcszWhere && _tcslen(lpcszWhere) > 0 && ipClass )
		{
			// [Bug 10374]トレース可能なラインデータを、平行移動後（[MANAGEMENT_ID]is not Null)のオブジェクトに限定する
			// 検索条件が設定してある時は、OIDと条件で検索して結果がある時のみ処理をするようにする
			// フィーチャキャッシュは属性で絞り込めないため
			long nOID = 0;
			ipFeature->get_OID(&nOID);

			if ( ! findObject(ipClass, nOID, lpcszWhere) )
				continue;
		}

		IGeometryPtr ipGeometry;
		ipFeature->get_ShapeCopy(&ipGeometry);

		VARIANT_BOOL vaIsTouches = VARIANT_FALSE;
		ipRelOp->Touches(ipGeometry, &vaIsTouches);
		if(vaIsTouches != VARIANT_TRUE)
			continue;

		double dDistance = 0;
		ipProximityOperator->ReturnDistance(ipGeometry,&dDistance);

		if(dDistance < dTolerance) {
			rFeatureMap.insert(std::multimap<double, IFeaturePtr>::value_type(dDistance, ipFeature));
		}
	}
}

/**
 * @brief リンクキャッシュクラス。
 */
class LinkCache
{
public:
// コンストラクタとデストラクタ
	LinkCache(IFeatureClass* ipLinkClass) :
	m_ipLinkClass(ipLinkClass)
	{
		if(m_ipLinkClass == 0)
			throw std::logic_error("リンクフィーチャクラスがnull.");
	}

	~LinkCache()
	{
	}

// 処理
	/**
	 * @brief キャッシュする。
	 *
	 * @param ipPoint [in] キャッシュ中心座標。
	 * @param dTolerance [in] スナップ閾値。
	 * @param dCacheRatio [in] スナップ閾値に対するキャッシュ範囲比。
	 * @return キャッシュ。失敗した場合はnull。
	 */
	IFeatureCachePtr cache(IPoint* ipPoint, double dTolerance, double dCacheRatio)
	{
		if( ipPoint == NULL )
			return 0;

		// キャッシュの確認。
		VARIANT_BOOL vaIsContains = VARIANT_FALSE;
		if(m_ipFeatureCache == 0) {
			m_ipFeatureCache.CreateInstance(CLSID_FeatureCache);
		}
		else{
			m_ipFeatureCache->Contains(ipPoint, &vaIsContains);
			if(vaIsContains == VARIANT_TRUE)
				return m_ipFeatureCache;
		}

		CWaitCursor aWaitCursor;

		// キャッシュ。
		m_ipFeatureCache->Initialize(ipPoint, dTolerance * dCacheRatio);

		double dCacheOverlap = dTolerance * (dCacheRatio + 1);

		IEnvelopePtr ipEnv;
		ipPoint->get_Envelope( &ipEnv );
		IEnvelopePtr ipEnvelope(ipEnv);
		ipEnvelope->Expand(dCacheOverlap, dCacheOverlap, VARIANT_FALSE);
		m_ipFeatureCache->AddFeatures(m_ipLinkClass, ipEnvelope);

		return m_ipFeatureCache;
	}

	/**
	 * @brief フィーチャクラス取得
	 * @bug   [Bug 10374]トレース可能なラインデータを、平行移動後（[MANAGEMENT_ID]is not Null)のオブジェクトに限定する
	 * @return フィーチャクラス
	*/
	IFeatureClassPtr getClass() { return m_ipLinkClass; }

private:
// 変数
	IFeatureClassPtr m_ipLinkClass; ///< リンクフィーチャクラス。
	IFeatureCache2Ptr m_ipFeatureCache; ///< フィーチャキャッシュ。
};

} // anonymous namespace 

//////////////////////////////////////////////////////////////////////
// LinkSnapTrace::impl
//////////////////////////////////////////////////////////////////////

class LinkSnapTrace::impl
{
public:
	typedef std::set<long> MarkingIndexSet;

// コンストラクタとデストラクタ
	impl(IFeatureClass* ipLinkClass) :
	m_dTolerance(0.0),
	m_dCacheRatio(2.0),
	m_bAhead(false),
	m_LinkCache(ipLinkClass)
	{
	}

	~impl()
	{
		stop();
	}

// 取得
	/**
	 * @brief 最後にマークされた位置を返す。
	 *
	 * @return 最後にマークされた位置。マークが存在しない場合null。
	 */
	long getLastMarkingIndex() const
	{
		return m_MarkingIndexSet.empty() ? -1 : *m_MarkingIndexSet.rbegin();
	}

// 処理
	/**
	 * @brief リンク追跡を開始する。
	 *
	 * @param ipPoint [in] カーソル座標。
	 * @retval true リンク追跡の開始に成功。
	 * @retval false 始点リンクを定められなかった。
	 */
	bool start(IPoint* ipPoint)
	{
		// 初期化。
		stop();

		// 状態確認。
		if(m_dTolerance <= 0)
			return false;

		// 検索。
		IFeaturePtr ipFeature(search_nearest(cache(ipPoint), ipPoint, m_dTolerance, m_LinkCache.getClass(), m_strWhere));
		if( ipFeature == NULL )
			return false;

		// 軌跡生成。
		m_ipTrace.CreateInstance(CLSID_Polyline);
		IGeometryPtr ipTmpGeom, ipTmpGeom2;
		ipFeature->get_ShapeCopy( &ipTmpGeom );
		((IGeometryCollectionPtr)ipTmpGeom)->get_Geometry( 0, &ipTmpGeom2 );
		m_ipTrace->AddGeometry(ipTmpGeom2);

		// リンク配列生成。
		m_ipLinkArray.CreateInstance(CLSID_Array);
		m_ipLinkArray->Add(ipFeature);

		// 向きの調整。
		adjustOrientation(ipPoint);

		return true;
	}

	/**
	 * @brief リンクを追跡する。
	 *
	 * @param ipPoint [in] カーソル座標。
	 * @retval true 軌跡に変化が生じた。
	 * @retval false 軌跡は以前のまま。
	 */
	bool trace(IPoint* ipPoint)
	{
		// 軌跡が存在しなければ追跡も不可能。
		if( m_ipTrace == NULL || m_ipLinkArray == NULL )
			return false;

		long nCount = getCount();
		if(nCount == 0)
			return false;

		// 向きの調整。
		bool bReversed = adjustOrientation(ipPoint);

		// リンクをキャッシュ。
		IFeatureCachePtr ipLinkCache(cache(ipPoint));

		// 分岐可能点を求める。
		IGeometryPtr ipTmpGeom;
		m_ipTrace->get_Geometry( nCount - 1, &ipTmpGeom );
		IPointCollectionPtr ipLastPath(ipTmpGeom);
		IPointPtr ipLastPoint(GetLastPoint(ipLastPath));
		IPointPtr ipSecondPoint((nCount > 1) ? IPointPtr(GetFirstPoint(ipLastPath)) : 0);

		// 最直線リンクを求める。
		// 分岐可能点がキャッシュ外に存在する場合、直進モードは先に進めない。
		VARIANT_BOOL vaIsContains = VARIANT_FALSE;
		ipLinkCache->Contains( ipLastPoint, &vaIsContains );
		IFeaturePtr ipStraightLink =
			(m_bAhead && vaIsContains == VARIANT_TRUE) ?
				search_straight(ipLinkCache, ipLastPath, m_LinkCache.getClass(), m_strWhere) : 0;

		// カーソル座標からの距離順に並べ替え。
		std::multimap<double, IFeaturePtr> aFeatureMap;
		order_by_distance(ipLinkCache, ipPoint, m_dTolerance, ipLastPoint, aFeatureMap, m_LinkCache.getClass(), m_strWhere);
		order_by_distance(ipLinkCache, ipPoint, m_dTolerance, ipSecondPoint, aFeatureMap, m_LinkCache.getClass(), m_strWhere);

		IFeaturePtr ipNextFeature;
		for(std::multimap<double, IFeaturePtr>::const_iterator it = aFeatureMap.begin(); it != aFeatureMap.end(); ++it) {
			long i = 0;
			for(i = 0; i < nCount; i++) {
				IUnknownPtr ipUnk;
				m_ipLinkArray->get_Element(i, &ipUnk);
				LONG lOID1 = 0, lOID2 = 0;
				((_IRowPtr)ipUnk)->get_OID(&lOID1);
				((_IRowPtr)it->second)->get_OID(&lOID2);
				if(lOID2 == lOID1)
					break;
			}

			if(i < nCount) {
				if(i == nCount - 2) {
					if(! back())
						continue;
					return true;
				}
				else if(i == nCount - 1)
					break;
				else
					continue;
			}

			if(m_bAhead) {
				// 直進モードでは、直進リンク以外にスナップすることはできない。
				LONG lOID1 = 0, lOID2 = 0;
				if( ipStraightLink != NULL )
				{
					((_IRowPtr)it->second)->get_OID(&lOID1);
					((_IRowPtr)ipStraightLink)->get_OID(&lOID2);
					if( lOID2 != lOID1 )
						continue;
				}
				else
					continue;
			}
			else {
				// 一手戻って別のリンクにスナップ。
				IGeometryPtr ipTmpGeom;
				it->second->get_ShapeCopy(&ipTmpGeom);
				VARIANT_BOOL vaIsTouches = VARIANT_FALSE;
				((IRelationalOperatorPtr)ipTmpGeom)->Touches(ipSecondPoint, &vaIsTouches);
				if(ipSecondPoint != NULL && vaIsTouches == VARIANT_TRUE) {
					if(! back())
						continue;
				}
			}

			ipNextFeature = it->second;
			break;
		}

		if(ipNextFeature != NULL) {
			step(ipNextFeature);
			return true;
		}

		return bReversed;
	}

	/**
	 * @brief マークする。
	 */
	void mark()
	{
		if(m_ipTrace == NULL)
			return;

		long nCount;
		if(m_ipTrace->get_GeometryCount(&nCount) != S_OK || nCount < 1)
			return;

		m_MarkingIndexSet.insert(nCount - 1);
	}

	/**
	 * @brief 最後のマークを外す。
	 */
	void demark()
	{
		if(m_MarkingIndexSet.empty())
			return;

		m_MarkingIndexSet.erase(--m_MarkingIndexSet.end());
	}

	/**
	 * @brief 最後のマークに戻る。
	 *
	 * 最後のマークが最後の要素に付けられている場合、最後から二つ目のマークに戻る。
	 */
	void undo()
	{
		long nLastMarkingIndex = getLastMarkingIndex();
		if(nLastMarkingIndex < 0)
			return;

		_ASSERT(m_ipTrace != NULL && m_ipLinkArray != NULL);

		long nCount = getCount();
		if(nLastMarkingIndex + 1 > nCount)
			return;

		if(nLastMarkingIndex + 1 == nCount) {
			// 最後のマークを外す。
			demark();

			// 最後から二つ目のマークに戻る。
			undo();
		}

		// 最後の要素を削除する。
		m_ipTrace->RemoveGeometries(nLastMarkingIndex + 1, nCount - nLastMarkingIndex - 1);
		for(long i = nCount - 1; i > nLastMarkingIndex; i--) {
			m_ipLinkArray->Remove(i);
		}
	}

	/**
	 * @brief リンク追跡を止める。
	 */
	void stop()
	{
		m_MarkingIndexSet.clear();
		m_ipTrace = 0;
		m_ipLinkArray = 0;
	}

	/**
	 * @brief 一手戻す。
	 *
	 * @retval true 成功。
	 * @retval false 失敗。あるいはマークされているため戻せない。
	 */
	bool back_public()
	{
		// 軌跡が存在しなかったら戻せない。
		if(m_ipTrace == NULL || m_ipLinkArray == NULL)
			return false;

		// 要素数取得。
		long nCount = getCount();

		// マークされていたら戻せない。
		if(getLastMarkingIndex() == nCount - 1)
			return false;

		// 最後の要素を削除する。
		m_ipTrace->RemoveGeometries(nCount - 1, 1);
		m_ipLinkArray->Remove(nCount - 1);

		return true;
	}

// 可変変数
	double m_dTolerance; ///< 選択閾値。
	double m_dCacheRatio; ///< キャッシュ比。
	bool m_bAhead; ///< 直進モードか。
	CString m_strWhere; ///!< 検索条件 [Bug 10374]

// 参照用変数
	IGeometryCollectionPtr m_ipTrace; ///< 軌跡。
	IArrayPtr m_ipLinkArray; ///< リンク配列。

private:
// 取得
	/**
	 * @brief 要素数を取得する。
	 *
	 * 要素数とは「Polylineを構成するPathの数」のこと。<br>
	 * これは「Polylilneを構成するFeatureの数」に等しい。
	 *
	 * @return 要素数。
	 */
	long getCount()
	{
		if(m_ipTrace == NULL || m_ipLinkArray == NULL)
			return 0;

		_ASSERT(m_ipTrace != NULL && m_ipLinkArray != NULL);

		long nCount = 0;
		m_ipTrace->get_GeometryCount(&nCount);
		LONG mm = 0;
		m_ipLinkArray->get_Count(&mm);
		//_ASSERT(nCount == 0;m_ipLinkArray->get_Count(&nCount));
		return nCount;
	}

// 処理
	IFeatureCachePtr cache(IPoint* ipPoint)
	{
		return m_LinkCache.cache(ipPoint, m_dTolerance, m_dCacheRatio);
	}

	/**
	 * @brief 軌跡の向きを調整する。
	 *
	 * @param ipPoint [in] カーソル座標。
	 * @retval true 向きを逆転させた。
	 * @retval false 軌跡に変更は加えていない。
	 */
	bool adjustOrientation(IPoint* ipPoint)
	{
		if( ipPoint == NULL )
			return false;

		// 構成パス数が1の場合に限る。
		LONG lCount = 0;
		m_ipTrace->get_GeometryCount(&lCount);
		if(m_ipTrace == NULL || lCount != 1)
			return false;

		// カーソルがポリラインの始点側にあるならば、ポリラインを反転させる。
		IPolylinePtr ipPolyline(m_ipTrace);

		double dAlong;
		ipPolyline->QueryPointAndDistance(esriNoExtension, ipPoint, VARIANT_TRUE, 0, &dAlong, 0, 0);

		if(dAlong < 0.5) {
			ipPolyline->ReverseOrientation();
			return true;
		}
		return false;
	}

	/**
	 * @brief 一手進める。
	 *
	 * @param ipFeature [in] 進めるフィーチャ。
	 */
	void step(IFeature* ipFeature)
	{
		if(m_ipTrace == NULL || m_ipLinkArray == NULL || ipFeature == NULL)
			return;

		IGeometryPtr ipTmpGeom;
		ipFeature->get_ShapeCopy(&ipTmpGeom);
		IPolylinePtr ipPolyine(ipTmpGeom);
		LONG lCount = 0;
		m_ipTrace->get_GeometryCount(&lCount);
		if(lCount > 0) {
			VARIANT_BOOL vaIsCompare = VARIANT_FALSE;
			IPointPtr ipPoint;
			((IPointCollectionPtr)ipPolyine)->get_Point(0,&ipPoint);
			int iResult = 0;
			(GetLastPoint(IPointCollectionPtr(m_ipTrace)))->Compare(ipPoint,&iResult);
			if(iResult != 0) {
				ipPolyine->ReverseOrientation();
			}
		}

		IGeometryPtr ipTmpGeom2;
		IGeometryCollectionPtr(ipPolyine)->get_Geometry(0, &ipTmpGeom2);
		m_ipTrace->AddGeometry(ipTmpGeom2);
		m_ipLinkArray->Add(ipFeature);
	}

	/**
	 * @brief 一手戻す。
	 *
	 * @retval true 成功。
	 * @retval false 失敗。あるいはマークされているため戻せない。
	 */
	bool back()
	{
		// 軌跡が存在しなかったら戻せない。
		if(m_ipTrace == NULL || m_ipLinkArray == NULL)
			return false;

		// 要素数取得。
		long nCount = getCount();

		// マークされていたら戻せない。
		if(getLastMarkingIndex() == nCount - 1)
			return false;

		// 最後の要素を削除する。
		m_ipTrace->RemoveGeometries(nCount - 1, 1);
		m_ipLinkArray->Remove(nCount - 1);

		return true;
	}

// 変数
	LinkCache m_LinkCache; ///< リンクキャッシュ。
	MarkingIndexSet m_MarkingIndexSet; ///< マーク一覧。
};

//////////////////////////////////////////////////////////////////////
// LinkSnapTrace
//////////////////////////////////////////////////////////////////////

// コンストラクタとデストラクタ
//////////////////////////////////////////////////////////////////////

LinkSnapTrace::LinkSnapTrace(IFeatureClass* ipLinkClass) :
pimpl(new impl(ipLinkClass))
{
}

LinkSnapTrace::~LinkSnapTrace()
{
}

// 設定
//////////////////////////////////////////////////////////////////////

/**
 * @brief 閾値を設定する。
 *
 * @param dTolerance [in] 閾値。
 * @param dCacheRatio [in] キャッシュ比。
 */
void LinkSnapTrace::setTolerance(double dTolerance, double dCacheRatio)
{
	pimpl->m_dTolerance = dTolerance;
	pimpl->m_dCacheRatio = dCacheRatio;
}

/**
 * @brief 直進モードか否かを設定する。
 *
 * @param bAhead [in] 直進モードか。
 */
void LinkSnapTrace::setAhead(bool bAhead)
{
	pimpl->m_bAhead = bAhead;
}

// [Bug 10374]トレース可能なラインデータを、平行移動後（[MANAGEMENT_ID]is not Null)のオブジェクトに限定する
// 検索条件設定
void LinkSnapTrace::setWhere(LPCTSTR lpcszWhere)
{
	pimpl->m_strWhere = lpcszWhere;
}


// 取得
//////////////////////////////////////////////////////////////////////

/**
 * @brief 軌跡を取得する。
 *
 * @return 軌跡。存在しない場合null。
 */
IPolylinePtr LinkSnapTrace::getTrace() const
{
	return pimpl->m_ipTrace;
}

/**
 * @brief リンクの並びを取得する。
 *
 * @return リンクの並び。存在しない場合null。
 */
IArrayPtr LinkSnapTrace::getLinkArray() const
{
	return pimpl->m_ipLinkArray;
}

/** @sa LinkSnapTrace::impl::getLastMarkingIndex() */
long LinkSnapTrace::getLastMarkingIndex() const
{
	return pimpl->getLastMarkingIndex();
}

// 処理
//////////////////////////////////////////////////////////////////////

/** @sa LinkSnapTrace::impl::start() */
bool LinkSnapTrace::start(IPoint* ipPoint)
{
	return pimpl->start(ipPoint);
}

/** @sa LinkSnapTrace::impl::trace() */
bool LinkSnapTrace::trace(IPoint* ipPoint)
{
	return pimpl->trace(ipPoint);
}

/** @sa LinkSnapTrace::impl::mark() */
void LinkSnapTrace::mark()
{
	pimpl->mark();
}

/** @sa LinkSnapTrace::impl::demark() */
void LinkSnapTrace::demark()
{
	pimpl->demark();
}

/** @sa LinkSnapTrace::impl::undo() */
void LinkSnapTrace::undo()
{
	pimpl->undo();
}
/** @sa LinkSnapTrace::impl::back() */
bool LinkSnapTrace::back_public()
{
	return pimpl->back_public();
}
