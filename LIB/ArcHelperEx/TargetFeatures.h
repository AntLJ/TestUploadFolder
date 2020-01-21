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
* @file TargetFeatures.h
*
* @brief フィーチャをリスト管理するクラス群の定義ファイル
*/
#if	!defined( __TARGETFEATURES_H__)
#define	__TARGETFEATURES_H__	//!< 重複インクルード防止

#ifdef DEBUG
#include "AheDraw.h"
#endif // ifdef DEBUG

#include "GlobalFunctions.h"
#include "TargetFeature.h"
#include "AheMeshUtil.h"
#include "ErrObj.h"
#include "AheNetwork.h"
#include "ErrorMsg.h"

class CTargetItem;

/**
* @class CTargetItems
*
* @brief 操作対象フィーチャ管理コンテナ
*
* @note 操作対象をリスト管理し、総括的な操作を提供
*/
template <class T = CTargetItem>
class CTargetItemsT : public std::list<T>
{
public:
	CTargetItemsT<T>()
	{
	}
	virtual ~CTargetItemsT<T>(){};

	/**
	* @brief コピー演算子
	*
	* @param cTargetItems [in] コピー元
	*
	* @return CTargetItems& 自身への参照
	*/
	template <class TT>
	const CTargetItemsT<T>& operator=( const CTargetItemsT<TT>& cTargetItems )
	{
		clear();
		return CTargetItemsT<T>::operator +=( cTargetItems );
	}

	/**
	* @brief 追加演算子
	*
	* @param cTargetItems [in] 追加フィーチャ群
	*
	* @return CTargetItems& 自身への参照
	*/
	template <class TT>
	inline const CTargetItemsT<T>& operator+=( const CTargetItemsT<TT>& cTargetItems )
	{
		for( CTargetItemsT<TT>::const_iterator it = cTargetItems.begin(); it != cTargetItems.end(); ++it)
		{
			CTargetItemsT<T>::push_back( (*it));
		}

		return *this;
	}

	/**
	* @brief 削除演算子
	*
	* @param cTargetItems [in] 削除フィーチャ群
	*
	* @return CTargetItems& 自身への参照
	*/
	template <class TT>
	inline const CTargetItemsT<T>& operator-=( const CTargetItemsT<TT>& cTargetItems )
	{
		for( CTargetItemsT<TT>::const_iterator it = cTargetItems.begin(); it != cTargetItems.end(); ++it)
		{
			CTargetItemsT<T>::iterator it2 = begin();
			while( it2 != end() )
			{
				if( *it == *it2 )
					it2 = erase( it2 );
				else
					it2++;
			}
		}

		return *this;
	}

	template <class TT>
	inline const CTargetItemsT<T>& operator+( const CTargetItemsT<TT>& cTargetItems )
	{
		return CTargetItemsT<T>::operator +=( cTargetItems );
	}
	template <class TT>
	inline const CTargetItemsT<T>& operator-( const CTargetItemsT<TT>& cTargetItems )
	{
		return CTargetItemsT<T>::operator -=( cTargetItems );
	}
	/**
	* @brief 後尾への要素を追加
	*
	* @note 重複挿入を禁止している。
	*
	* @param cItem			[in]			追加する要素
	*/
	inline void push_back( const T& cItem )
	{
		if( cItem.GetFeature() == NULL )	// 空フィーチャ防止
		{
			_ASSERTE( cItem.GetFeature() != NULL );
			return;
		}

		if( IsAlreadyExist( cItem ) ) 
			return;
		
		list<T>::push_back( cItem);
	}

	/**
	* @brief 後尾への要素を追加
	*
	* @note 重複挿入を禁止している。
	*
	* @param ipFeature		[in]			追加する要素を構成するフィーチャ
	*/
	inline void push_back( IFeaturePtr ipFeature )
	{
		if( ipFeature == NULL )	// 空フィーチャ防止
		{
			_ASSERTE( ipFeature != NULL );
			return;
		}

		T cItem( ipFeature );
		CTargetItemsT<T>::push_back( cItem );
	}

	/**
	* @brief 先頭への要素を追加
	*
	* @note 重複挿入を禁止している。
	*
	* @param cItem			[in]			追加する要素
	*/
	inline void push_front( const T& cItem )
	{
		if( cItem.GetFeature() == NULL )	// 空フィーチャ防止
		{
			_ASSERTE( cItem.GetFeature() != NULL );
			return;
		}

		if( IsAlreadyExist( cItem ) )
			return;

		list<T>::push_front( cItem );
	}
	/**
	* @brief 先頭への要素を追加
	*
	* @note 重複挿入を禁止している。
	*
	* @param ipFeature		[in]			追加する要素を構成するフィーチャ
	*/
	void push_front( IFeaturePtr ipFeature )
	{
		if( ipFeature == NULL )	// 空フィーチャ防止
		{
			_ASSERTE( ipFeature != NULL );
			return;
		}

		T cItem( ipFeature );
		CTargetItems::push_front( cItem );
	}
	/**
	* @brief 全管理対象をバッファ化し、合成したポリゴンを作成
	*
	* @note ポイントをバッファしたものは、他の形状をバッファしたものと合成できないようなので、この関数ではポイントを対象から除外している。
	*
	* @param dbBufferSize [in] バッファ化するサイズ(map unit値
	* @param ppiPolygon [out] 合成したポリゴン
	*
	* @retval TRUE 正常終了
	* @retval FALSE 不正終了
	*/
	BOOL CreatePolygon( double dbBufferSize, IPolygon** ppiPolygon )
	{
		IPolygonPtr piMaster;
		IPolygonPtr piSlave;

		ITopologicalOperatorPtr piPolyTopo;
		IGeometryPtr piGeomMaster;
		IGeometryPtr piGeomSlave;
		IFeaturePtr piFeature;

		for( CTargetItemsT<T>::iterator it = begin(); it != end(); it++)
		{
			if( esriGeometryPoint == it->GetGeometryType())
			{
				// ポイントをバッファしたものは、他の形状をバッファしたものとユニオンできないようである。
				continue;
			}

			if( (*it).CreatePolygon( dbBufferSize, &piSlave))
			{
				if( NULL != piMaster)
				{
					piGeomMaster = piMaster;
					piPolyTopo = piGeomMaster;
					piGeomSlave = piSlave;
					HRESULT hr = piPolyTopo->Union( piGeomSlave, &piGeomMaster);
					if( SUCCEEDED( hr))
					{
						piMaster = piGeomMaster;
					}
				}
				else
				{
					piMaster = piSlave;
				}
			}
		}

		(*ppiPolygon) = piMaster;
		(*ppiPolygon)->AddRef();

		return ( NULL != piMaster);
	}

	/**
	* @brief 全管理対象を包含する矩形領域の取得
	*
	* @retval IEnvelopePtr 包含する矩形領域
	* @retval NULL 不正終了
	*/
	IEnvelopePtr GetEnvelope()
	{
		IGeometryPtr piGeom;
		IEnvelopePtr piEnvMaster;
		IEnvelopePtr piEnvSlave;
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it)
		{
			piGeom = it->GetShape();
			if( piGeom != NULL )
			{
				piGeom->get_Envelope( &piEnvSlave);
				if( NULL != piEnvMaster)
				{
					piEnvMaster->Union( piEnvSlave);
				}
				else
				{
					piEnvMaster = piEnvSlave;
				}
			}
		}

		return piEnvMaster;
	}
	/**
	 * @brief 全管理対象をポリラインにして Union を取る
	 *
	 * @note ポイントは対象になりません
	 *
	 * @retval IPolylinePtr
	 */
	IPolylinePtr GetUnion()
	{
		using namespace libErrMsg;
		IGeometryPtr ipUnionGeom;
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); it++ )
		{
			IGeometryPtr ipGeom;
			switch( it->GetGeometryType() )
			{
			case esriGeometryPolygon:
				ipGeom = AheConvertPolygon2Polyline( it->GetShape() );
				break;
			case esriGeometryPolyline:
				ipGeom = it->GetShapeCopy();
				break;
			default:
				break;
			}

			if( ipGeom != NULL ) 
			{
				if( ipUnionGeom == NULL )
					ipUnionGeom = ipGeom;
				else {
					// ユニオンするには空間参照があっている必要がある
					ISpatialReferencePtr ipSpRef;
					if( AheHasSameSpatialReference( ipGeom, ipUnionGeom ) == FALSE )
					{
						ISpatialReferencePtr ipSpRef1, ipSpRef2;
						ipGeom->get_SpatialReference( &ipSpRef1 );
						ipUnionGeom->get_SpatialReference( &ipSpRef2 );
						ATLTRACE(_T("CTargetItems::GetUnion() : ジオメトリ同士の空間参照が異なるので片方の空間参照を使用します。その場合、意図した結果にならない可能性があります。\n") );
						CString strMsg;
						strMsg.Format(GETUNION_LAYER_OID, it->GetFeatureClassName(), it->GetOID() );
						AheTraceSpatialReference( ipSpRef1, strMsg );
						AheTraceSpatialReference( ipSpRef2, _T("CTargetItems::GetUnion() : ユニオン") );
					}
					ipUnionGeom->get_SpatialReference( &ipSpRef );

					IGeometryPtr ipGeomTmp = AheUnion( ipGeom, ipUnionGeom, ipSpRef );
					if( ipGeomTmp == NULL )
					{
						ATLASSERT( ipGeomTmp == NULL );
						ATLTRACE(_T("CTargetItems::GetUnion() : ユニオンに失敗しました。\n") );
						AheTraceGeometry( ipGeom, _T("CTargetItems::GetUnion()") );
						AheTraceGeometry( ipUnionGeom, _T("CTargetItems::GetUnion()") );
					}
					else {
						//ITopologicalOperatorPtr ipTopo( ipGeomTmp );
						//if( ipTopo != NULL ) ipTopo->Simplify();	
						AheForceSimplify( ipGeomTmp ); // ユニオンを取ると KnownSimple が FALSE になる
					}

					ipUnionGeom = ipGeomTmp;
				}
			}
		}

		return ipUnionGeom;
	}

	/**
	 * @brief ネットワークリンクフィーチャクラスを取得する
	 * 管理一覧からネットワークモデルを判断し、ネットワークリンクフィーチャクラス
	 * をオープンして返します。
	 */
	IFeatureClassPtr GetNetworkLinkFeatureClass() const
	{
		for( const_iterator it = begin(); it != end(); ++it )
		{
			if( AheIsRelateWithNetwork( it->GetFeatureClassName() ) )
				return AheGetNetworkLinkFeatureClass( it->GetFeature() );
		}
		return NULL;
	}

	/**
	 * @brief ネットワークノードフィーチャクラスを取得する
	 * @note  管理一覧からネットワークモデルを判断し
	 * 初めに見つかったネットワークノードフィーチャクラスをオープンして返します。
	 */
	IFeatureClassPtr GetNetworkNodeFeatureClass() const
	{
		for( const_iterator it = begin(); it != end(); ++it )
		{
			if( AheIsRelateWithNetwork( it->GetFeatureClassName() ) )
				return AheGetNetworkNodeFeatureClass( it->GetFeature() );
		}
		return NULL;
	}

	/**
	* @brief 道路フィーチャ一覧取得
	*
	* @note 管理一覧より、Road_Linkテーブルのフィーチャのみ取得
	*
	* @param cTargetFeatures	[out]			道路一覧
	* @param bAdd				[in,optional]	追加するかどうか（デフォルト FALSE）
	*
	* @retval TRUE 正常終了
	* @retval FALSE 不正終了
	*/
	template <class TT>
	BOOL GetRoadFeatures( CTargetItemsT<TT>& cTargetFeatures, BOOL bAdd = FALSE)
	{	
		CString str;

		str.Format(_T("|%s|"), ROAD_TABLE_NAME );
		return GetFeatures( cTargetFeatures, str, bAdd );
	}

	/**
	* @brief ネットワークリンクフィーチャ一覧取得
	*
	* @note 管理一覧より、ネットワークリンクテーブルのフィーチャのみ取得
	*
	* @param cTargetFeatures	[out]			ネットワークリンク一覧
	* @param bAdd				[in,optional]	追加するかどうか（デフォルト FALSE）
	*
	* @retval TRUE 正常終了
	* @retval FALSE 不正終了
	*/
	template <class TT>
	BOOL GetNetworkLinkFeatures( CTargetItemsT<TT>& cTargetFeatures, BOOL bAdd = FALSE)
	{	
		return GetFeatures( cTargetFeatures, AheGetNetworkLinks(), bAdd );
	}

	/**
	* @brief ノードフィーチャ一覧取得
	*
	* @note 管理一覧より、Nodeテーブルのフィーチャのみ取得
	*
	* @param cTargetFeatures	[out]			ノード一覧
	* @param bAdd				[in,optional]	追加するかどうか（デフォルト FALSE）
	*
	* @retval TRUE 正常終了
	* @retval FALSE 不正終了
	*/
	template <class TT>
	BOOL GetNodeFeatures( CTargetItemsT<TT>& cTargetFeatures, BOOL bAdd = FALSE)
	{
		CString str;

		str.Format(_T("|%s|"), NODE_TABLE_NAME );
		return GetFeatures( cTargetFeatures, str, bAdd );
	}

	/**
	* @brief ネットワークノードフィーチャ一覧取得
	*
	* @note 管理一覧より、ネットワークノードテーブルのフィーチャのみ取得
	*
	* @param cTargetFeatures	[out]			ネットワークノード一覧
	* @param bAdd				[in,optional]	追加するかどうか（デフォルト FALSE）
	*
	* @retval TRUE 正常終了
	* @retval FALSE 不正終了
	*/
	template <class TT>
	BOOL GetNetworkNodeFeatures( CTargetItemsT<TT>& cTargetFeatures, BOOL bAdd = FALSE)
	{	
		return GetFeatures( cTargetFeatures, AheGetNetworkNodes(), bAdd );
	}

	/**
	* @brief 指定レイヤのフィーチャ一覧取得
	*
	* @note 管理一覧より、指定のテーブル名称のフィーチャのみ取得
	*
	* @param cTargetItems		[out]			指定レイヤのフィーチャ一覧
	* @param lpcszLayerNames    [in]			取得するレイヤ名群("|ROAD_LINK|ROAD_NODE|"のように | 区切りで単独あるいは複数指定する)
	* @param bAdd				[in,optional]	追加するかどうか（デフォルト FALSE）
	*
	* @retval TRUE 正常終了
	* @retval FALSE 不正終了
	*/
	template <class TT>
	BOOL GetFeatures( CTargetItemsT<TT>& cTargetItems, LPCTSTR lpcszLayerNames, BOOL bAdd = FALSE)
	{
		list<CString> listLayerNames;
		AheSepStr2listString( lpcszLayerNames, _T("|"), listLayerNames );

		if( bAdd == FALSE ) cTargetItems.clear();
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it )
		{
			CString strLayerName = it->GetFeatureClassName();
			for( list<CString>::iterator itNames = listLayerNames.begin(); itNames != listLayerNames.end(); itNames++ )
			{
				if( strLayerName.CompareNoCase( *itNames ) == 0 )
					cTargetItems.push_back( *it );
			}
		}
		return TRUE;
	}

	/**
	* @brief 指定レイヤのフィーチャ一覧取得
	*
	* @note 管理一覧より、指定のテーブル名称のフィーチャのみ取得
	*
	* @param cTargetItems		[out]			指定レイヤのフィーチャ一覧
	* @param lpcszLayerName     [in]			指定レイヤ名（ | は入れないこと）
	* @param bAdd				[in,optional]	追加するかどうか（デフォルト FALSE）
	*
	* @retval TRUE 正常終了
	* @retval FALSE 不正終了
	*/
	template <class TT>
	BOOL GetFeature( CTargetItemsT<TT>& cTargetItems, LPCTSTR lpcszLayerName, BOOL bAdd = FALSE)
	{
		CString layerName;
		layerName.Format(_T("|%s|"), lpcszLayerName);
		return GetFeatures( cTargetItems, layerName, bAdd );
	}

	/**
	* @brief 管理フィーチャ一覧に２次メッシュ境界ノードを含む？
	*
	* @retval TRUE 含む
	* @retval FALSE 含まない
	*/
	BOOL IsIncludedBorderNode( void)
	{
		CTargetItemsT<T>::iterator it = begin();
		while( it != end())
		{
			if( it->IsBorderNode() || it->IsBorderPECNode() ) 
				return TRUE;

			++it;
		}
		return FALSE;
	}

	/**
	* @brief 管理フィーチャ一覧にネットワーク関連のフィーチャが含まれるか確認
	*
	* @note ネットワーク関連のフィーチャか否かの判定にはAheIsRelateWithNetworkを使用
	*
	* @retval TRUE 含む
	* @retval FALSE 含まない
	*/
	BOOL IsIncludedNetworkItems( void)
	{
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it)
		{
			if( AheIsRelateWithNetwork( it->GetFeature() ) )
				return TRUE;
		}
		return FALSE;
	}
	
	/**
	* @brief ネットワークモデルのタイプを取得する
	*
	* @note 複数のタイプが混在していた場合は sindyeNotNetwork を返す
	*
	* @retrn TRUE ネットワークモデルタイプ
	*/
	sindyeNetworkModel GetNetworkType()
	{
		if( CTargetItems::empty() )
			return sindyeNotNetwork;

		// 複数の種類が混在していたら NotNetwork を返す
		sindyeNetworkModel type = AheGetNetWorkType( CTargetItems::begin()->GetFeature() );
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it)
		{
			if( type != AheGetNetWorkType( it->GetFeature() ) )
			{
				type = sindyeNotNetwork;
				break;
			}
		}

		return type;
	}

	/**
	* @brief 管理フィーチャ一覧に存在するかどうか
	*
	* @param item	[in]	比較アイテム
	*
	* @retval TRUE 存在する
	* @retval FALSE 存在しない
	*/
	template <class TT>
	BOOL IsAlreadyExist( const TT& item ) const
	{
		for( CTargetItemsT<T>::const_iterator it = begin(); it != end(); ++it )
		{
			if( *it == item ) return TRUE;
		}
		return FALSE;
	}

	/**
	* @brief 管理フィーチャ一覧に存在するかどうか
	*
	* @param ipFeature	[in]	IFeaturePtr
	*
	* @retval TRUE 存在する
	* @retval FALSE 存在しない
	*/
	BOOL IsAlreadyExist( IFeaturePtr ipFeature ) const
	{
		T item( ipFeature );
		return IsAlreadyExist( item );
	}

	/**
	* @brief 更新情報の設定
	*
	* @note 全管理対象の更新設定を呼び出して、更新情報を設定する。
	*
	* @param emUpdateType [in] 更新タイプ
	* @param lpModifyTime [in] 更新時間
	*
	* @retval TRUE 正常終了
	* @retval FALSE 不正終了
	*/
	BOOL SetModifyData( sindyUpdateType emUpdateType, LPSYSTEMTIME lpModifyTime = NULL)
	{
		// 変更時刻が指定されていない場合全体を同一時間で調整
		// マシン時間の整合を考えると、"SYSDATE"を指定するのが最善と思われるが、SDEがSYSDATEを正しく流さないようである。
		SYSTEMTIME stModifyTime;
		if( NULL == lpModifyTime)
		{
			::GetLocalTime( &stModifyTime);
		}
		else
		{
			CopyMemory( &stModifyTime, lpModifyTime, sizeof( SYSTEMTIME));
		}

		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it)
		{
			(*it).SetModifyData( emUpdateType, &stModifyTime);
		}
		return TRUE;
	}

	/**
	* @brief 管理対象一覧から、指定の位置に最も近いItemを取得
	*
	* @param piPoint チェック対象のポイント
	* @param dbTolerance 閾値
	* @param pcItem 最も近い要素
	* @param piHitPoint ヒット位置
	* @param plVertexIndex 
	*
	* @retval esriGeometryPartNone 指定したエリア内でヒットしない
	* @retval esriGeometryPartVertex 頂点にヒットした
	* @retval esriGeometryPartBoundary 辺上にヒットした
	*/
	esriGeometryHitPartType GetNearestItem( IPointPtr piPoint, double dbTolerance, CTargetItem* pcItem, IPoint** piHitPoint, long* plVertexIndex)
	{
		double dbHitDist;
		double dbMinDist = -1;

		// Results
		CTargetItemsT<T>::iterator itResultIt = end();
		esriGeometryHitPartType emResultHitPart = esriGeometryPartNone;
		IPointPtr piResultPoint;
		long lResultVertex;

		IPointPtr piRetPoint;
		long lRetVertex;
		esriGeometryHitPartType enRetHitPart;
		esriGeometryType geomType = esriGeometryNull;
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it )
		{
			esriGeometryType type = it->GetGeometryType();
			enRetHitPart = (*it).HitTest( piPoint, dbTolerance, &piRetPoint, &dbHitDist, NULL, &lRetVertex);
			if( esriGeometryPartNone != enRetHitPart)
			{
				DOUBLE dDelta = dbMinDist - dbHitDist;
				BOOL bIsNearest = FALSE;
				
				switch( type )
				{
					case esriGeometryPoint:
						switch( geomType )
						{
							// ポイントの場合で、且つ保持されているものもポイントの場合は距離で比較
							case esriGeometryPoint:
								if( dDelta < dbTolerance && dDelta > 0.0 )
									bIsNearest = TRUE;
								break;
							// 保持されているものがポイント以外のものなら、問答無用
							default:
								bIsNearest = TRUE;
								break;
						}
						break;
					case esriGeometryPolyline:
					case esriGeometryPolygon:
						switch( geomType )
						{
							// まだ何も保持されていない場合は問答無用
							case esriGeometryNull:
								bIsNearest = TRUE;
								break;
							// ポリライン・ポリゴンの場合で、且つ保持されているものがポリライン・ポリゴンの場合
							// ヒット位置が同じなら距離で比較
							// ヒット位置がちがうなら、常にVertex優先
							case esriGeometryPolyline:
							case esriGeometryPolygon:
								if( ( emResultHitPart == enRetHitPart && dDelta < dbTolerance && dDelta > 0.0 ) ||
									( emResultHitPart > enRetHitPart ) )
									bIsNearest = TRUE;
								break;
						}
						break;
					default:
						break;
				}

				if( bIsNearest == TRUE )
				{
					dbMinDist = dbHitDist;
					geomType = type;
					itResultIt = it;
					piResultPoint = piRetPoint;
					lResultVertex = lRetVertex;
					emResultHitPart = enRetHitPart;
				}
			}
		}

		if( end() != itResultIt)
		{
			if( NULL != pcItem)(*pcItem) = ( *itResultIt);
			if( NULL != piHitPoint)
			{
				(*piHitPoint) = piResultPoint;
				(*piHitPoint)->AddRef();
			}
			if( NULL != plVertexIndex)(*plVertexIndex) = lResultVertex;
		}
		return emResultHitPart;
	}

	/**
	* @brief 管理対象一覧から、指定の位置に最も近いItem群を取得
	*
	* @param piPoint チェック対象のポイント
	* @param dbTolerance 閾値
	* @param pcItems 最も近い要素群
	* @param piHitPoint ヒット位置
	* @param plVertexIndex 
	* @param bIgnoreGeometryType [in,optional] ジオメトリタイプの優先順位を無視するかどうか（デフォルト：false）
	*
	* @retval esriGeometryPartNone 指定したエリア内でヒットしない
	* @retval esriGeometryPartVertex 頂点にヒットした
	* @retval esriGeometryPartBoundary 辺上にヒットした
	*/
	template <class TT>
	esriGeometryHitPartType GetNearestItems( IPointPtr piPoint, double dbTolerance, CTargetItemsT<TT>* pcItems, IPoint** piHitPoint, long* plVertexIndex, bool bIgnoreGeometryType = false )
	{
		if ( ! ( piPoint && pcItems ) )
			return esriGeometryPartNone;

		IPointPtr			 	piResultPoint, piRetPoint;
		double 					dbHitDist = 0.0f, dbMinDist = -1.0f;
		long 					lResultVertex = -1, lRetVertex = -1;
		esriGeometryHitPartType emResultHitPart = esriGeometryPartNone, enRetHitPart = esriGeometryPartNone;
		esriGeometryType 		geomType = esriGeometryNull;

		// リストクリア
		pcItems->clear();
		for ( iterator it = begin(); it != end(); it++ ) {
			enRetHitPart = (*it).HitTest( piPoint, dbTolerance, &piRetPoint, &dbHitDist, NULL, &lRetVertex);
			ATLTRACE( _T("%s\n"), AheGetFeatureClassName( it->GetFeature() ) );
			if ( esriGeometryPartNone != enRetHitPart) {
				// 距離が同じなら追加
				if ( dbMinDist == dbHitDist && ( bIgnoreGeometryType || geomType == it->GetGeometryType() ) )
					pcItems->push_back( *it );
				// 距離が短い方を優先（ただし、ポイントと辺では常にポイントが優先）
				else if( ( ( ! bIgnoreGeometryType ) && geomType > it->GetGeometryType() ) || // TODO: こうしたら、テスト対象が2つあってどちらもラインもラインの場合、常に最初のほうしか採用されないのでは？？この関数本当に大丈夫？？
						 ( dbMinDist >= dbHitDist || -1 == dbMinDist ) &&
						 ( emResultHitPart == esriGeometryPartNone || enRetHitPart == esriGeometryPartVertex ) ) { // TODO: なんで、構成点のみに絞る必要がある？？これだと、初回だけしかBoundaryへのヒットが許されない。この関数本当に大丈夫？？
					geomType = it->GetGeometryType();
					pcItems->clear();
					pcItems->push_back( *it );
					dbMinDist = dbHitDist;
					piResultPoint = piRetPoint;
					lResultVertex = lRetVertex;
					emResultHitPart = enRetHitPart;
				}
			}
		}

		if ( piHitPoint && piResultPoint ) {
			(*piHitPoint) = piResultPoint;
			(*piHitPoint)->AddRef();
		}

		if ( plVertexIndex )
			(*plVertexIndex) = lResultVertex;

		return emResultHitPart;
	}

	/**
	* @brief 管理対象一覧から、編集対象一覧を作成
	*
	* @param piPoint チェック対象のポイント
	* @param dbTolerance 閾値
	* @param ppiHitPont ヒットした位置
	*
	* @return 選択された要素数
	*/
	int SelectByPoint( IPointPtr piPoint, double dbTolerance, IPoint** ppiHitPont)
	{
		IPointPtr piHitPoint;
		if( esriGeometryPartNone != GetNearestItem( piPoint, dbTolerance, NULL, &piHitPoint, NULL))
		{
			if( NULL != ppiHitPont)
			{
				(*ppiHitPont) = piHitPoint;
				(*ppiHitPont)->AddRef();
			}

			CTargetItemsT<T>::iterator it = begin();
			while( it != end())
			{
				IGeometryPtr ipGeom( it->GetShapeCopy() );
				// HitTestを実施する
				// 今まではIRelationalOperatorを使用していたが、誤差が大きすぎる。
				// しかも、ポイントの場合はAheIsDBEqualに飛ばされるため、ポイントとそれ以外で
				// さらに誤差が拡大する。
				// そもそもこのメソッドは閾値を与えられているので、本来HitTestで確認すべきもの
				IHitTestPtr ipHitTest( ipGeom );
				IPointPtr ipHit( AheCreateClone(piHitPoint) );
				double dHitDistance = 0.0;
				long lHitVertex = 0, lHitSegment = 0;
				VARIANT_BOOL vbRightSide = VARIANT_FALSE, vbHit = VARIANT_FALSE;
				// まずは構成点で確認
				ipHitTest->HitTest( piHitPoint, dbTolerance, esriGeometryPartVertex, ipHit, &dHitDistance, &lHitVertex, &lHitSegment, &vbRightSide, &vbHit );
				if( ! vbHit )
				{
					// 構成点でヒットしなかったらBoundaryで確認
					ipHitTest->HitTest( piHitPoint, dbTolerance, esriGeometryPartBoundary, ipHit, &dHitDistance, &lHitVertex, &lHitSegment, &vbRightSide, &vbHit );
					if( ! vbHit )
					{
						it = erase( it );
						continue;
					}
				}
				++it;
			}
		} // TODO: どこにもヒットしない場合はclearするべきでは？？この関数が本当に必要か、要確認
		return size();
	}

	/**
	* @brief 管理フィーチャ群の形状チェック
	*
	* @param strErrMsg					[out]			エラーメッセージ
	* @param bCheckSegmentOverlapOnly	[in,optional]	セグメント重複チェックのみの場合は TRUE
	* @param ipCheckEnv					[in,optional]	ポリゴンの重複チェック時の限定エリア（デフォルト：NULL）
	* @param ipRule						[in,optional]	ルールオブジェクト（デフォルト：NULL）
	*
	* @retval sindyeGeometryCheck
	*/
	sindyeGeometryCheck IsSafeGeometries( CString& strErrMsg, BOOL bCheckSegmentOverlapOnly = FALSE, IEnvelopePtr ipCheckEnv = NULL, ISiNDYRule3Ptr ipRule = NULL )
	{
		sindyeGeometryCheck emRet = sindyeGeometryNoCheck; // 戻り値

		CErrObjContainer errs;
		switch( IsSafeGeometries( errs, bCheckSegmentOverlapOnly, ipCheckEnv, ipRule ) )
		{
			case sindyeLogLevel_INFO: emRet = sindyeGeometryOK; break;
			case sindyeLogLevel_WARN: emRet = sindyeGeometryWarnning; break;
			case sindyeLogLevel_ERR:  emRet = sindyeGeometryError; break;
			default: break;
		}

		strErrMsg = errs.ToString();
		return emRet;
	}
	/**
	* @brief 管理フィーチャ群の形状チェック
	*
	* @param strErrMsg					[out]			エラーメッセージ
	* @param bCheckSegmentOverlapOnly	[in,optional]	セグメント重複チェックのみの場合は TRUE
	* @param ipCheckEnv					[in,optional]	ポリゴンの重複チェック時の限定エリア（デフォルト：NULL）
	* @param ipRule						[in,optional]	ルールオブジェクト（デフォルト：NULL）
	*
	* @retval sindyeLogLevel
	*/
	sindyeLogLevel IsSafeGeometries( CErrObjContainer& errs, BOOL bCheckSegmentOverlapOnly = FALSE, IEnvelopePtr ipCheckEnv = NULL, ISiNDYRule3Ptr ipRule = NULL )
	{
		using namespace sindy::schema;
		using namespace libErrMsg;
#ifdef DEBUG
		DWORD DWSTART = GetTickCount();
#endif // ifdef DEBUG

		CTargetItemsT<T>::iterator it, it2;
		CTargetItems cRoadItems;	// 接続角チェックを後でするのでこれに入れておく
		CTargetItems cNodeItems;	// 接続各チェック時にノードの種別をみるので必要
		BOOL bIsCheckAngle = ( bCheckSegmentOverlapOnly ) ? FALSE : TRUE;

		GetNetworkNodeFeatures( cNodeItems );	// ノード取得

		// ポリゴンの重複チェックは時間がかかるので、エリア指定がある場合は別バージョンでチェック
		// セグメントオーバーラップチェックのみの時はスルー
		if( ! bCheckSegmentOverlapOnly )
		{
			if( ipCheckEnv )
				CheckPolygonOverlap( ipCheckEnv, errs );
		}
		// 道路リンクがループしていないかどうか
		// セグメントがオーバーラップしていないかどうか確認（管理フィーチャ内のみ）
		for( it = begin(); it != end(); ++it )
		{
			CComVariant vaFromID, vaToID;
			CString strItFeatureClassName = it->GetFeatureClassName();
			CString strItOwnerDotClassName = it->GetOwnerDotClassName();

			// FROM、TO取得（道路のみ）
			if( AheIsNetworkLink( strItFeatureClassName ) )
			{
				cRoadItems.push_back( *it );	// 保持
				vaFromID = it->GetAttribute( FROM_FIELD );
				vaToID   = it->GetAttribute( TO_FIELD   );

				if( ( ! bCheckSegmentOverlapOnly ) && ( vaFromID.vt != VT_I4 || vaToID.vt != VT_I4 ) )
					errs.add( it->GetOwnerDotClassName(), it->GetOID(), it->GetShapeCopy(), -1, MakeWKSPoint( AheGetFromPoint( it->GetShape() ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_FROM_OR_TO_MISSING );
			}

			// 構成点数をみてチェックする・しないを判断（物凄く遅いため…）
			// セグメントオーバーラップチェックのみの時はスルーされる
			if ( bIsCheckAngle )
			{
				if( it->GetGeometryType() == esriGeometryPolygon && strItFeatureClassName.Compare( _T("PARKING_SHAPE") ) != 0 ) {
					LONG lPointCount = -1;
					IGeometryPtr ipGeom;
					it->GetFeature()->get_Shape( &ipGeom );
					((IPointCollectionPtr)ipGeom)->get_PointCount( &lPointCount );
					if( lPointCount > 10000 )	// [bug 4288]
					{
						CString strMsg;
						strMsg.Format(CONFIRM_CHECK_POLYGON_WITH_OVER_10000_POINT, strItFeatureClassName, it->GetOID(), lPointCount );
						if( AheMessageBox( ::GetActiveWindow(), strMsg, GEOMETRY_CHECK, MB_YESNO ) == IDNO )
							bIsCheckAngle = FALSE;
					}
				}
			}

			// 重複チェック（エリアなしの場合。ポリゴンのみ）
			// セグメントオーバーラップチェックのみの時はスルー
			if( ! bCheckSegmentOverlapOnly )
			{
				if( ! ipCheckEnv )
				{
					if( it->GetGeometryType() == esriGeometryPolygon &&
						strItFeatureClassName.Compare( _T("PARKING_SHAPE") ) != 0 &&
						!AheIsOverlapableLayer( it->GetFeatureClassName() ) )
					{
						IFeatureClassPtr ipFeatureClass( it->GetFeatureClass() );
						if( ipFeatureClass  )
						{
							CString strSubField;
							CComBSTR bstrOIDField, bstrShapeField;
							IGeometryPtr ipGeom;

							ipFeatureClass->get_OIDFieldName( &bstrOIDField );
							ipFeatureClass->get_ShapeFieldName( &bstrShapeField );
							strSubField.Format(_T("%s,%s"), COLE2CT(bstrOIDField), COLE2CT(bstrShapeField) );
							it->GetFeature()->get_Shape( &ipGeom );

							if( ! bIsCheckAngle )
								continue;

							IFeatureCursorPtr ipFeatCursor( AheSelectByShape( ipGeom, ipFeatureClass, esriSpatialRelIntersects, strSubField, _T(""), TRUE ) );
							if( ipFeatCursor )
							{
								IFeaturePtr ipFeat;
								while( SUCCEEDED( ipFeatCursor->NextFeature( &ipFeat ) ) )
								{
									if( ! ipFeat )
										break;

									CTargetItem item( ipFeat );
									LONG lOIDTmp = item.GetOID();

									if( it->GetOID() != lOIDTmp )
									{
										// 移動対象群に含まれている場合はその形状でチェック
										BOOL bIsOverlaps = FALSE;
										if( IsAlreadyExist( item ) )
										{
											// IsAlreadyExistは廃止して、find()を有効利用すべき！！
											iterator itItem = find( begin(), end(), item );
											_ASSERTE( itItem != end() );
											bIsOverlaps = AheIsOverlap2( ipGeom, itItem->GetShape() );
										}
										else
											bIsOverlaps = AheIsOverlap2( ipGeom, AheGetShape( ipFeat ) );

										if( bIsOverlaps )
											errs.add( it->GetOwnerDotClassName(), it->GetOID(), it->GetShapeCopy(), -1, MakeWKSPoint( AheGetAreaPoint( it->GetShape() ) ), -1, sindyeLogLevel_WARN, ERROR_DUPLICATE_OF_OID, lOIDTmp );
									}
								}
							}
						}
					}
				}
			}
			it2 = it;
			++it2;
			for( ; it2 != end(); it2++ )
			{
				if( strItFeatureClassName.Compare( it2->GetFeatureClassName() ) == 0 )
				{
					if( ( ! bCheckSegmentOverlapOnly ) && AheIsNetworkLink( strItFeatureClassName ) )
					{
						CComVariant vaFromID2 = it2->GetAttribute( FROM_FIELD );
						CComVariant vaToID2   = it2->GetAttribute( TO_FIELD );
						if( vaFromID2.vt != VT_I4 || vaFromID2.lVal < 1 || vaToID2.vt != VT_I4 || vaToID2.lVal < 1 )
						{
							errs.add( it2->GetOwnerDotClassName(), it2->GetOID(), it2->GetShapeCopy(), -1, MakeWKSPoint( AheGetFromPoint( it2->GetShape() ) ), -1, sindyeLogLevel_ERR, _T("%s"), ERROR_LINK_MUST_HAVE_FROM_OR_TO_ID );
						}

						// ループチェック
						if( ( vaFromID == vaFromID2 && vaToID == vaToID2 ) || ( vaFromID == vaToID2 && vaToID == vaFromID2 ) )
						{
							errs.add( it->GetOwnerDotClassName(), it->GetOID(), it->GetShapeCopy(), -1, MakeWKSPoint( AheGetFromPoint( it->GetShape() ) ), -1, sindyeLogLevel_ERR, ERROR_LINK_LOOPES, it->GetOID(), it2->GetOID() );
						}
					}
					// セグメントオーバーラップチェック
					if(( AheIsNetworkLink( strItFeatureClassName ) ||	// ネットワークリンク
						strItFeatureClassName.CompareNoCase( GRAD_TABLE_NAME ) == 0 ) &&	// 傾斜
						strItFeatureClassName.CompareNoCase( adam_v2::lane_link::kTableName ) != 0 )	// [ADAM] 車線リンクは除く
					{
						if( AheIsOverlap2( it->GetShape(), it2->GetShape() ) )
						{
							errs.add( it->GetOwnerDotClassName(), it->GetOID(), it->GetShapeCopy(), -1, MakeWKSPoint( AheGetFromPoint( it->GetShape() ) ), -1, sindyeLogLevel_ERR, ERROR_SEGMENT_OVERLAP, it->GetOID(), it2->GetOID() );
						}
					}
					else if( strItFeatureClassName.CompareNoCase( SEGMENTATTR_TABLE_NAME ) == 0 )	// リンク内属性
					{
						// 種別が一緒の場合のみエラーとする
						if( AheIsOverlap2( it->GetShape(), it2->GetShape() ) && 
							it->GetAttribute( _T("LINKATTR_C") ) == it2->GetAttribute( _T("LINKATTR_C") ) )
						{
							errs.add( it->GetOwnerDotClassName(), it->GetOID(), it->GetShapeCopy(), -1, MakeWKSPoint( AheGetFromPoint( it->GetShape() ) ), -1, sindyeLogLevel_ERR, ERROR_SAME_TYPE_SEGMENT_OVERLAP, it->GetOID(), it2->GetOID() );
						}
					}
					if( ! bCheckSegmentOverlapOnly )
					{
						// 自動運転支援マップは2次メッシュチェックしない
						if( AheIsNetworkLink(strItFeatureClassName) && !AheIsAdamNetworkLink(strItFeatureClassName) )
						{
							// 道路リンクの接続角チェック/2次メッシュ線での折り返しチェック
							// セグメントオーバーラップチェックのみの時はスルー
							IGeometryPtr ipBaseCopy(it->GetShapeCopy()), ipTestCopy(it2->GetShapeCopy());
							IPointPtr ipTouchPoint;
							if( AheIsTouchEndPoint( ipBaseCopy, ipTestCopy, NULL, &ipTouchPoint ) ) 
							{
								// 接点がメッシュに接しているかどうか
								INT ix = 0, iy = 0; 
								DOUBLE dX = 0.0, dY = 0.0;
								ipTouchPoint->QueryCoords( &dX, &dY );
								unsigned int iMeshCode = AheLL2MESH( dX, dY, 2, ix, iy, 1 ); // bug 8919
								if( ix == 0 || iy == 0 )
								{
									// [bug 1870] メッシュ線をはさんだリンクは135°以上の角度がついている必要がある
									DOUBLE dAngle = AheGetAngle( it->GetShape(), it2->GetShape() );
									if( -135.0 <= dAngle && dAngle <= 135.0 )
										errs.add( it->GetOwnerDotClassName(), it->GetOID(), ipBaseCopy, -1, MakeWKSPoint( dX, dY ), -1, sindyeLogLevel_ERR, ERROR_ANGLE_IS_135_DEGREE_OR_LESS, it->GetOID(), it2->GetOID(), (int)dAngle );
									// 折り返し
									IPointPtr ipCenterPoint1( CLSID_Point ), ipCenterPoint2( CLSID_Point );
									ipCenterPoint1->putref_SpatialReference( it->GetSpatialReference() ); // 空間参照付与
									ipCenterPoint2->putref_SpatialReference( it2->GetSpatialReference() ); // 空間参照付与
									DOUBLE dLen1 = 0, dLen2 = 0;
									DOUBLE dCenterX1 = 0, dCenterY1 = 0, dCenterX2 = 0, dCenterY2 = 0;

									((IPolylinePtr)ipBaseCopy)->get_Length( &dLen1 );
									((IPolylinePtr)ipTestCopy)->get_Length( &dLen2 );
									((IPolylinePtr)ipBaseCopy)->QueryPoint( esriNoExtension, dLen1/2.0, VARIANT_FALSE, ipCenterPoint1 );
									((IPolylinePtr)ipTestCopy)->QueryPoint( esriNoExtension, dLen2/2.0, VARIANT_FALSE, ipCenterPoint2 );
									ipCenterPoint1->QueryCoords( &dCenterX1, &dCenterY1 );
									ipCenterPoint2->QueryCoords( &dCenterX2, &dCenterY2 );

									int ixDummy = 0, iyDummy = 0;
									UINT iMeshCode1 = AheLL2MESH( dCenterX1, dCenterY1, 2, ixDummy, iyDummy );
									UINT iMeshCode2 = AheLL2MESH( dCenterX2, dCenterY2, 2, ixDummy, iyDummy );
									if( iMeshCode1 == iMeshCode2 )
										errs.add( it->GetOwnerDotClassName(), it->GetOID(), ipBaseCopy, -1, MakeWKSPoint( dX, dY ), -1, sindyeLogLevel_ERR, ERROR_ROAD_LINK_IS_FOLDED_MESH_BOUNDARY, it->GetOID(), it2->GetOID() );
								}
							}
							// [CHECK] [ROADCHECK 02320] 構成点同士が接触しているかどうか（bug 8463）
							IPointPtr ipTouch;
							if( AheIsTouchVertex( (IPointCollectionPtr)ipBaseCopy, (IPointCollectionPtr)ipTestCopy, ipTouch ) )
							{
								errs.add( it->GetOwnerDotClassName(), it->GetOID(), ipBaseCopy, -1, MakeWKSPoint( ipTouch ), -1, sindyeLogLevel_ERR, ERROR_POINTS_ARE_TOUCHING, it->GetOID(), it2->GetOID() );
							}
						}
					}
				}
			}
			// 形状チェック
			// セグメントオーバーラップチェックのみの時はスルー
			if( ! bCheckSegmentOverlapOnly )
				(*it).IsSafeGeometry( errs, FALSE, bIsCheckAngle, ipRule );
		}
		// [bug 1870] 道路リンクの接続角チェック（２差路で角度20°以下）
		// セグメントオーバーラップチェックのみの時はスルー
		if( ! bCheckSegmentOverlapOnly )
		{
			// 形状ではなくFROM、TOで接続状況を調べる(bug 5477)
			if( cRoadItems.size() > 1 ) // リンクは2本以上ないとチェックできない
			{
				// TODO 以下、ネットワークリンク別にチェックを分けるようにしないとメンテナンスが困難になるので分けること

				/// [ROADCHECK 02322] 同一ノードに接続する線分が近接しているをチェックするか否か
				// 歩道リンク以外ならチェックする(歩行者Phase2対応)
				bool bCheckCloseNode = (sindyeNetworkWalk!=AheGetNetWorkType(cRoadItems.begin()->GetFeature()));
				/// [鉄道全路線] 鉄道リンクでは二叉路の接続角度チェックは警告にする
				// [自動運転支援マップ]車線・上下線モデルでもチェックは警告にする
				bool bWarnAngleChk = (sindyeNetworkRailway==AheGetNetWorkType(cRoadItems.begin()->GetFeature()))
					|| (sindyeNetworkLane==AheGetNetWorkType(cRoadItems.begin()->GetFeature()))
					|| (sindyeNetworkUpDown==AheGetNetWorkType(cRoadItems.begin()->GetFeature()))
					|| (sindyeNetworkCompart==AheGetNetWorkType(cRoadItems.begin()->GetFeature()))
					|| (sindyeNetworkBorder==AheGetNetWorkType(cRoadItems.begin()->GetFeature()));
				// 接続状況を調べる
				std::map<long,CTargetItems> listGraph; // map<ノードID,list<リンクID>>
				for( CTargetItems::iterator itRoads1 = cRoadItems.begin(); itRoads1 != cRoadItems.end(); ++itRoads1 )
				{
					//if ( itRoads1->GetFeatureClassName().CompareNoCase( LANE_LINK_TABLE_NAME ) == 0 )
					if ( (sindyeNetworkLane==AheGetNetWorkType(cRoadItems.begin()->GetFeature()))
						|| (sindyeNetworkUpDown==AheGetNetWorkType(cRoadItems.begin()->GetFeature()))
						|| (sindyeNetworkCompart==AheGetNetWorkType(cRoadItems.begin()->GetFeature()))
						|| (sindyeNetworkBorder==AheGetNetWorkType(cRoadItems.begin()->GetFeature())) )
						continue;

					long lOID = itRoads1->GetOID();
					listGraph[itRoads1->GetAttribute( FROM_FIELD ).lVal].push_back(*itRoads1);
					listGraph[itRoads1->GetAttribute( TO_FIELD ).lVal  ].push_back(*itRoads1);
				}
				for( std::map<long,CTargetItems>::const_iterator itGraph = listGraph.begin(); itGraph != listGraph.end(); ++itGraph )
				{
					// 2叉路を見つけた
					if( 2 == itGraph->second.size() )
					{
						// 接続しているノードが2叉路ノード以外の場合は判定対象としない
						CTargetItems::iterator itNode = cNodeItems.begin();
						for( ; itNode != cNodeItems.end(); ++itNode )
						{
							if( itNode->GetOID() == itGraph->first )
								break;		
						}
						if( itNode != cNodeItems.end() )
						{
							CComVariant vaNodeClass = itNode->GetAttribute( NODECLASS_FIELD );
							if( vaNodeClass.vt == VT_I4 && 
								( vaNodeClass.lVal == sindyeNodeIntersection ) || 
								( vaNodeClass.lVal == sindyeNodeConnectPecLink ) ||
								( vaNodeClass.lVal == sindyePECNode ) ||
								( vaNodeClass.lVal == sindyeRelHWY ) )
								ATLTRACE(_T("CTargetItems::IsSafeGeometries() : 交差点ノード及びPEC・HWY関係ノードに接続しているため角度判定はしません：%d-%d\n"), itGraph->second.begin()->GetOID(), (++itGraph->second.begin())->GetOID() );
							else {
								// 接続角
								DOUBLE dAngle = AheGetAngle( itGraph->second.begin()->GetShape(), (++itGraph->second.begin())->GetShape() );
								if( -20.0 <= dAngle && dAngle <= 20.0 )
								{
									errs.add( itGraph->second.begin()->GetOwnerDotClassName(), itGraph->second.begin()->GetOID(), itGraph->second.begin()->GetShapeCopy(), -1, 
									MakeWKSPoint( AheGetConnectPoint( itGraph->second.begin()->GetShape(), (++itGraph->second.begin())->GetShape() ) ), -1,
									((bWarnAngleChk)? sindyeLogLevel_WARN:sindyeLogLevel_ERR), 
									ERROR_ANGLE_IS_20_DEGREE_OR_LESS, itGraph->second.begin()->GetOID(), (++itGraph->second.begin())->GetOID(), (int)dAngle );
								}
							}
						}
					}
					if( bCheckCloseNode && 2 <= itGraph->second.size() )
					{
						// [CHECK] [ROADCHECK 02322] 同一ノードに接続する線分が近接している（bug 8463）
						for( CTargetItems::const_iterator itConnectRoad = itGraph->second.begin(); itConnectRoad != itGraph->second.end(); ++itConnectRoad )
						{
							CTargetItems::const_iterator itConnectRoadNext = itConnectRoad;
							if( itGraph->second.end() == ++itConnectRoadNext )
								itConnectRoadNext = itGraph->second.begin();
							IPointCollectionPtr ipBaseGeom( itConnectRoad->GetShape() );
							IPointCollectionPtr ipTestGeom( itConnectRoadNext->GetShape() );
							double dLen = min( AheGetConnectSegmentCrack( ipBaseGeom, ipTestGeom ), AheGetConnectSegmentCrack( ipTestGeom, ipBaseGeom ) );
							if( 20.0 >= dLen )
								errs.add( itConnectRoad->GetOwnerDotClassName(), itConnectRoad->GetOID(), itConnectRoad->GetShapeCopy(), -1, 
								MakeWKSPoint( AheGetConnectPoint( itGraph->second.begin()->GetShape(), (++itGraph->second.begin())->GetShape() ) ), -1, sindyeLogLevel_WARN,
								ERROR_SEGMENT_CONNECT_SAME_NODE_IS_CLOSE, itConnectRoad->GetOID(), itConnectRoadNext->GetOID(), dLen );
						}
					}
				}
			}
		}
#ifdef DEBUG
		ATLTRACE(_T("●●● CTargetItems::IsSafeGeometries() : %lf ●●●\n"), (DOUBLE)(GetTickCount() - DWSTART ) / (DOUBLE)1000.0 );
#endif

		return errs.GetMaxLogLevel();
	}

	/**
	* @brief 管理対象一覧の編集完了
	*
	* @retval TRUE 正常終了
	* @retval FALSE 不正終了
	*/
	BOOL Store();

	/**
	* @brief 地理参照設定
	*
	* @note 地理参照を設定する。
	*
	* @param piSpatialReference [in] 設定する地理参照指定しない場合は、管理対象のレイヤから取得して設定
	*
	* @retval TRUE 正常終了
	* @retval FALSE 不正終了
	*/
	BOOL SnapToSpatialReference( ISpatialReferencePtr piSpatialReference = NULL);

	/**
	 * @brief エラーメッセージの取得
	 *
	 * @retval CString エラーメッセージ
	 */
	CString GetErrMessage()
	{
		CString strErrMessage;
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it )
			strErrMessage += it->GetErrMessage();

		return strErrMessage;
	}

	/**
	 * @brief エラーメッセージの削除
	 */
	void RemoveErrMessage()
	{
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it )	it->RemoveErrMessage();
	}

	/**
	* @brief ISetにアイテムを格納
	*
	* @param ipSet [out] ISetのインスタンス
	*/
	void SetISetFromItems(_ISet** ipSet);

	/**
	* @brief ISetにアイテムを格納
	*
	* @param ipSet [out] ISetのインスタンス
	*/
	_ISetPtr GetFeaturesSet();
	
	/**
	* @brief アイテムにISetを格納
	*
	* @param ipMap [in] IMapのインスタンス
	* @param ipSet [in] ISetのインスタンス
	*/
	void SetItemsFromISet(IMapPtr ipMap, _ISetPtr ipSet);

	/**
	* @brief アイテムにISetを格納(比較)
	*
	* @param ipMap [in] IMapのインスタンス
	* @param cCompItems [in] 比較アイテム
	* @param ipSet [in] ISetのインスタンス
	*/
	void SetItemsFromISetFromComp(IMapPtr ipMap, CTargetItemsT cCompItems, _ISetPtr ipSet);

	/**
	* @brief アイテムにISetを格納
	*
	* @param ipSet	[in]			_ISetPtr
	* @param bAdd	[in,optional]	追加格納の場合は TRUE（デフォルト：FALSE）
	*/
	void SetFeaturesSet( _ISetPtr ipSet, BOOL bAdd = FALSE );

	/**
	 * @brief 強制的に Simplify() をする
	 *
	 * @see AheForceSimplify
	 *
	 * @retval TRUE 成功
	 * @retval FALSE 失敗
	 */
	BOOL Simplify()
	{
		BOOL bRet = TRUE;
		for( CTargetItemsT<T>::iterator it = begin(); it != end(); ++it ) 
		{
			if( it->Simplify() == FALSE )
				bRet = FALSE;
		}

		return bRet;
	}
#ifdef _DEBUG
	void Trace( LPCTSTR lpcszHeader )
	{
		for( const_iterator it = begin(); it != end(); ++it )
		{
			ATLTRACE(_T("%s : TABLE[%s] OID[%d]\n"), lpcszHeader, AheGetFeatureClassName( it->GetFeature() ), it->GetOID() );
			AheTraceGeometry( it->GetShapeCopy(), lpcszHeader );
		}
	}
#else
	void Trace( LPCTSTR/* lpcszHeader*/ ){}
#endif // ifdef _DEBUG
private:
	BOOL CheckPolygonOverlap( IEnvelopePtr ipCheckEnv, CErrObjContainer& errs ) const
	{
#ifdef DEBUG
		DWORD DWSTART = GetTickCount();
#endif // ifdef DEBUG

		using namespace libErrMsg;

		// フィーチャクラス別に分ける
		std::map<LONG, CTargetItems> cFeaturesMap;
		for( CTargetItemsT<T>::const_iterator it = begin(); it != end(); ++it )
		{
			if( it->GetGeometryType() == esriGeometryPolygon &&
				it->GetFeatureClassName().Compare( _T("PARKING_SHAPE") ) != 0 &&
				!AheIsOverlapableLayer( it->GetFeatureClassName() ) )
			{
				IFeatureClassPtr ipFeatClass( it->GetFeatureClass() );
				if( ipFeatClass != NULL )
				{
					LONG lObjectClassID = -1;
					ipFeatClass->get_ObjectClassID( &lObjectClassID );
					cFeaturesMap[lObjectClassID].push_back( *it );
				}
			}
		}
		// フィーチャクラス別にチェック
		for( std::map<LONG, CTargetItems>::iterator itMap = cFeaturesMap.begin(); itMap != cFeaturesMap.end(); ++itMap )
		{
			IFeatureClassPtr ipFeatClass( itMap->second.begin()->GetFeatureClass() );
			CString strItFeatureClassName = AheGetFeatureClassName( ipFeatClass );

			// まずは編集フィーチャ内でのみチェック
			for( CTargetItems::iterator itFeatures = itMap->second.begin(); itFeatures != itMap->second.end(); ++itFeatures )
			{
				CTargetItems::iterator itFeatures2 = itFeatures;
				itFeatures2++;
				for( ; itFeatures2 != itMap->second.end(); ++itFeatures2 )
				{
					if( AheIsOverlap2( itFeatures->GetShape(), itFeatures2->GetShape() ) )  // ポリゴン重複は警告に（bug 5573）
					{
						errs.add( itFeatures->GetOwnerDotClassName(), itFeatures->GetOID(), itFeatures->GetShapeCopy(), -1, MakeWKSPoint( AheGetAreaPoint( itFeatures->GetShape() ) ), -1, sindyeLogLevel_WARN, ERROR_DUPLICATE_OF_OID, itFeatures2->GetOID() );
					}
				}
			}
			// 検索
			CString strSubField;
			CComBSTR bstrOIDField, bstrShapeField;

			ipFeatClass->get_OIDFieldName( &bstrOIDField );
			ipFeatClass->get_ShapeFieldName( &bstrShapeField );
			strSubField.Format(_T("%s,%s"), CString(bstrOIDField), CString(bstrShapeField) );
			IFeatureCursorPtr ipFeatCursor = AheSelectByShape( (IGeometryPtr)ipCheckEnv, ipFeatClass, esriSpatialRelIntersects, strSubField, _T(""), TRUE );
			// チェック
			if( ipFeatCursor )
			{
				IFeaturePtr ipFeat;
				while( SUCCEEDED( ipFeatCursor->NextFeature( &ipFeat ) ) )
				{
					if( ! ipFeat )
						break;

					CTargetItem cItemTmp( ipFeat );
					LONG lOIDTmp = cItemTmp.GetOID();

					for( CTargetItems::iterator itFeatures = itMap->second.begin(); itFeatures != itMap->second.end(); ++itFeatures )
					{
						if( itFeatures->GetOID() != lOIDTmp )
						{
							// 移動対象群に含まれている場合はその形状でチェック
							BOOL bIsOverlaps = FALSE;
							if( ! IsAlreadyExist( cItemTmp ) )
							{
								IGeometryPtr ipTestGeom;
								ipFeat->get_Shape( &ipTestGeom );
								bIsOverlaps = AheIsOverlap2( itFeatures->GetShape(), ipTestGeom );
							}

							if( bIsOverlaps )
							{
								errs.add( itFeatures->GetOwnerDotClassName(), itFeatures->GetOID(), itFeatures->GetShapeCopy(), -1, MakeWKSPoint( AheGetAreaPoint( itFeatures->GetShape() ) ), -1, sindyeLogLevel_WARN, ERROR_DUPLICATE_OF_OID, lOIDTmp );
							}
						}
					}
				}
			}
		}
#ifdef DEBUG
		ATLTRACE(_T("●●●    CTargetItems::CheckPolygonOverlap() : %lf ●●●\n"), ((DOUBLE)GetTickCount() - DWSTART )/(DOUBLE)1000.0 );
#endif
		return TRUE;
	}
};

typedef CTargetItemsT<CTargetItem>		CTargetItems;

// 管理対象一覧の編集完了
template <typename T>
inline BOOL CTargetItemsT<T>::Store()
{
	for( CTargetItemsT<T>::iterator it = begin(); it != end(); it++ ) {
		(*it).Store();
	}
	return TRUE;
}

// 地理参照設定
template <typename T>
inline BOOL CTargetItemsT<T>::SnapToSpatialReference( ISpatialReferencePtr piSpatialReference)
{
	for( CTargetItemsT<T>::iterator it = begin(); it != end(); it++)
	{
		( *it).SnapToSpatialReference( piSpatialReference);
	}
	return TRUE;
}

template <typename T>
inline void CTargetItemsT<T>::SetISetFromItems(_ISet** ipSet)
{
	_ISetPtr			ipSetTmp(CLSID_Set);

	for( CTargetItemsT<T>::iterator it = begin(); it != end(); it++) {
		ipSetTmp->Add( it->GetFeature() );
	}
	(*ipSet) = ipSetTmp;
	(*ipSet)->AddRef();
	return;
}

template <typename T>
inline _ISetPtr CTargetItemsT<T>::GetFeaturesSet()
{
	_ISetPtr ipSet( CLSID_Set );

	for( iterator it = begin(); it != end(); it++ ) 
	{
		ipSet->Add( it->GetFeature() );
	}

	return ipSet;
}

template <typename T>
inline void CTargetItemsT<T>::SetItemsFromISet(IMapPtr ipMap, _ISetPtr ipSet)
{
	USES_CONVERSION;

	LONG		lCount;

	ipSet->get_Count( &lCount );
	ipSet->Reset();

	clear();
	for ( LONG i = 0; i < lCount; i++ ) {
		ILayerPtr		ipLayer;
		IUnknownPtr		ipUnk;
		CComBSTR		bstrLayerName;

		ipSet->Next( &ipUnk );
		push_back( (IFeaturePtr)ipUnk );
	}
	return;
}

template <typename T>
inline void CTargetItemsT<T>::SetItemsFromISetFromComp(IMapPtr ipMap, CTargetItemsT cCompItems, _ISetPtr ipSet)
{
	LONG		lCount;

	ipSet->get_Count( &lCount );
	ipSet->Reset();

	clear();
	for ( LONG i = 0; i < lCount; i++ ) {
		IUnknownPtr 		ipUnk;
		CString 			strName;
		LONG 				lOID = 0;

		ipSet->Next( &ipUnk );

		IFeaturePtr 		ipFeature( ipUnk );

		if( ipFeature == NULL )
			continue;

		ipFeature->get_OID( &lOID );
		strName = AheGetFeatureClassName( ipFeature );

		for( CTargetItemsT<T>::iterator it = cCompItems.begin(); it != cCompItems.end(); it++ ) 
		{
			IFeaturePtr ipFeature2 = it->GetFeature();
			if ( lOID == (*it).GetOID() && strName.CompareNoCase( AheGetFeatureClassName( ipFeature2 ) ) == 0 )
				push_back( *it );
		}
	}
	return;
}

template <typename T>
inline void CTargetItemsT<T>::SetFeaturesSet( _ISetPtr ipSet, BOOL bAdd/* = FALSE*/ )
{
	if( ipSet == NULL ) return;

	if( bAdd == FALSE ) clear();

	ipSet->Reset();
	IUnknownPtr ipUnk;
	while( SUCCEEDED( ipSet->Next( &ipUnk ) ) )
	{
		if( ipUnk == NULL ) break;

		push_back( (IFeaturePtr)ipUnk );
	}
}

#endif	//__TARGETFEATURES_H__
