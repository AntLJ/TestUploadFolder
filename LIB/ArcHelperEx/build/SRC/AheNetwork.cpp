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
#include "AheNetwork.h"
#include "aheglobals.h"
#include "TargetFeatures.h"
#include "graph.h"
#include "RuleModel.h"
#include <iterator>

using namespace sindy::schema;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ネットワークモデルのラインフィーチャクラスかどうかを判定する
bool AheIsNetworkLink( LPCTSTR lpcszTableName )
{
	for( CRuleModelNetwork::const_iterator it = g_cRuleModelNetwork.begin(); it != g_cRuleModelNetwork.end(); ++it )
	{
		if( 0 == lstrcmpi( lpcszTableName, it->first ) ) // link
			return true;
	}
	return false;
}
bool AheIsNetworkLink( ILayer* ipLayer )
{
	return AheIsNetworkLink( AheGetFeatureClassName( ipLayer ) );
}
bool AheIsNetworkLink( IFeatureClass* ipFeatureClass )
{
	return AheIsNetworkLink( AheGetFeatureClassName( ipFeatureClass ) );
}
bool AheIsNetworkLink( IFeature* ipFeature )
{
	return AheIsNetworkLink( AheGetFeatureClassName( ipFeature ) );
}

// ADAMネットワークモデルのラインフィーチャクラスかどうかを判定する
bool AheIsAdamNetworkLink(LPCTSTR lpcszTableName)
{
	using namespace adam_v2;

	return
		0 == lstrcmpi(lpcszTableName, lane_link::kTableName) ||
		0 == lstrcmpi(lpcszTableName, updown_link::kTableName) ||
		0 == lstrcmpi(lpcszTableName, compart_link::kTableName) ||
		0 == lstrcmpi(lpcszTableName, border_link::kTableName);
}
bool AheIsAdamNetworkLink(ILayer* ipLayer)
{
	return AheIsAdamNetworkLink(AheGetFeatureClassName(ipLayer));
}
bool AheIsAdamNetworkLink(IFeatureClass* ipFeatureClass)
{
	return AheIsAdamNetworkLink(AheGetFeatureClassName(ipFeatureClass));
}
bool AheIsAdamNetworkLink(IFeature* ipFeature)
{
	return AheIsAdamNetworkLink(AheGetFeatureClassName(ipFeature));
}


// ネットワークモデルのポイントフィーチャクラスかどうかを判定する
bool AheIsNetworkNode( LPCTSTR lpcszTableName )
{
	for( CRuleModelNetwork::const_iterator it = g_cRuleModelNetwork.begin(); it != g_cRuleModelNetwork.end(); ++it )
	{
		if( 0 == lstrcmpi( lpcszTableName, it->second ) ) // point
			return true;
	}
	return false;
}
bool AheIsNetworkNode( ILayer* ipLayer )
{
	return AheIsNetworkNode( AheGetFeatureClassName( ipLayer ) );
}
bool AheIsNetworkNode( IFeatureClass* ipFeatureClass )
{
	return AheIsNetworkNode( AheGetFeatureClassName( ipFeatureClass ) );
}
bool AheIsNetworkNode( IFeature* ipFeature )
{
	return AheIsNetworkNode( AheGetFeatureClassName( ipFeature ) );
}

// ADAMネットワークモデルのポイントフィーチャクラスかどうかを判定する
bool AheIsAdamNetworkNode(LPCTSTR lpcszTableName)
{
	using namespace adam_v2;

	return
		0 == lstrcmpi(lpcszTableName, lane_node::kTableName) ||
		0 == lstrcmpi(lpcszTableName, updown_node::kTableName) ||
		0 == lstrcmpi(lpcszTableName, compart_node::kTableName) ||
		0 == lstrcmpi(lpcszTableName, border_node::kTableName);
}
bool AheIsAdamNetworkNode(ILayer* ipLayer)
{
	return AheIsAdamNetworkNode(AheGetFeatureClassName(ipLayer));
}
bool AheIsAdamNetworkNode(IFeatureClass* ipFeatureClass)
{
	return AheIsAdamNetworkNode(AheGetFeatureClassName(ipFeatureClass));
}
bool AheIsAdamNetworkNode(IFeature* ipFeature)
{
	return AheIsAdamNetworkNode(AheGetFeatureClassName(ipFeature));
}

// ネットワークモデルかどうか判定する
bool AheIsNetwork( LPCTSTR lpcszTableName )
{
	return ( AheIsNetworkLink( lpcszTableName ) || 
		     AheIsNetworkNode( lpcszTableName ) );
}
bool AheIsNetwork( ILayer* ipLayer )
{
	return AheIsNetwork( AheGetFeatureClassName( ipLayer ) );
}
bool AheIsNetwork( IFeatureClass* ipFeatureClass )
{
	return AheIsNetwork( AheGetFeatureClassName( ipFeatureClass ) );
}
bool AheIsNetwork( IFeature* ipFeature )
{
	return AheIsNetwork( AheGetFeatureClassName( ipFeature ) );
}

// ADAMネットワークモデルかどうか判定する
bool AheIsAdamNetwork(LPCTSTR lpcszTableName)
{
	return (AheIsAdamNetworkLink(lpcszTableName) ||
		AheIsAdamNetworkNode(lpcszTableName));
}
bool AheIsAdamNetwork(ILayer* ipLayer)
{
	return AheIsAdamNetwork(AheGetFeatureClassName(ipLayer));
}
bool AheIsAdamNetwork(IFeatureClass* ipFeatureClass)
{
	return AheIsAdamNetwork(AheGetFeatureClassName(ipFeatureClass));
}
bool AheIsAdamNetwork(IFeature* ipFeature)
{
	return AheIsAdamNetwork(AheGetFeatureClassName(ipFeature));
}

///ネットワークモデルタイプ判定
sindyeNetworkModel AheGetNetWorkType( LPCTSTR lpcszTableName )
{
	CString table = AheGetNetworkLinkFeatureClassName( lpcszTableName );
	if( 0 == table.CompareNoCase( road_link::kTableName ) )
		return sindyeNetworkRoad;
	
	if( 0 == table.CompareNoCase( walk_link::kTableName ) )
		return sindyeNetworkWalk;

	if( 0 == table.CompareNoCase( sj::subway_link::kTableName ) )
		return sindyeNetworkSubway;

	if( 0 == table.CompareNoCase( sj::railway_link::kTableName ) )
		return sindyeNetworkRailway;

	if( 0 == table.CompareNoCase( adam_v2::lane_link::kTableName ) )
		return sindyeNetworkLane;

	if( 0 == table.CompareNoCase( adam_v2::updown_link::kTableName ) )
		return sindyeNetworkUpDown;

	if( 0 == table.CompareNoCase( adam_v2::compart_link::kTableName ) )
		return sindyeNetworkCompart;

	if( 0 == table.CompareNoCase( adam_v2::border_link::kTableName ) )
		return sindyeNetworkBorder;

	return sindyeNotNetwork;
}
sindyeNetworkModel AheGetNetWorkType( ILayer* ipLayer )
{
	return AheGetNetWorkType( AheGetFeatureClassName( ipLayer ) );
}
sindyeNetworkModel AheGetNetWorkType( IFeatureClass* ipFeatureClass )
{
	return AheGetNetWorkType( AheGetFeatureClassName( ipFeatureClass ) );
}
sindyeNetworkModel AheGetNetWorkType( IFeature* ipFeature )
{
	return AheGetNetWorkType( AheGetFeatureClassName( ipFeature ) );
}

// ネットワークモデルに関連するかどうか判定する
bool AheIsRelateWithNetwork( LPCTSTR lpcszTableName )
{
	// ネットワークモデルそのものかどうか
	if( AheIsNetwork( lpcszTableName ) )
		return true;

	// ネットワークモデルに関連するものかどうか
	for( CRuleModelNetwork::const_iterator it = g_cRuleModelNetwork.begin(); it != g_cRuleModelNetwork.end(); ++it )
	{
		for( CRuleRelationshipContain::const_value_iterator it2 = g_cRuleRelationshipContain.begin( it->first ); it2 != g_cRuleRelationshipContain.end( it->first ); ++it2 )
		{
			if( 0 == lstrcmpi( lpcszTableName, it2->first ) )
				return true;
		}
	}
	// リンク列系かどうか
	for( CRuleLinkQueue::const_iterator it = g_cRuleLinkQueue.begin(); it != g_cRuleLinkQueue.end(); ++it )
	{
		for( CRuleLinkQueue::TRuleValue::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			if( 0 == it2->get<0>().CompareNoCase( lpcszTableName ) ||
				0 == it2->get<1>().CompareNoCase( lpcszTableName ) )
				return true;
		}
	}
	return false;
}
bool AheIsRelateWithNetwork( ILayer* ipLayer )
{
	return AheIsRelateWithNetwork( AheGetFeatureClassName( ipLayer ) );
}
bool AheIsRelateWithNetwork( IFeatureClass* ipFeatureClass )
{
	return AheIsRelateWithNetwork( AheGetFeatureClassName( ipFeatureClass ) );
}
bool AheIsRelateWithNetwork( IFeature* ipFeature )
{
	return AheIsRelateWithNetwork( AheGetFeatureClassName( ipFeature ) );
}

// ネットワークリンク・ノードフィーチャクラス名を返す
CString _AheGetNetworkFeatureClassName( LPCTSTR lpcszTableName, bool bIsLinkName )
{
	CString strRet; // 返り値

	if( ! lpcszTableName )
		return strRet;

	// 渡されたテーブル名がネットワークリンク名かどうか
	if( AheIsNetworkLink( lpcszTableName ) )
		strRet = ( bIsLinkName ) ? lpcszTableName : g_cRuleModelNetwork.GetPairName( lpcszTableName );
	// 渡されたテーブル名がネットワークノード名かどうか
	else if( AheIsNetworkNode( lpcszTableName ) )
		strRet = ( bIsLinkName ) ? g_cRuleModelNetwork.GetPairName( lpcszTableName ) : lpcszTableName;
	// 渡されたテーブル名がネットワークモデルに関連するテーブル名かどうか
	else if( AheIsRelateWithNetwork( lpcszTableName ) )
	{
		// 内包関係から探す
		for( CRuleRelationshipContain::const_iterator it = g_cRuleRelationshipContain.begin(); it != g_cRuleRelationshipContain.end(); ++it )
		{
			for( CRuleRelationshipContain::TRuleValue::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			{
				if( 0 == it2->first.Compare( lpcszTableName ) )
				{
					strRet = ( bIsLinkName ) ? it->first : g_cRuleModelNetwork.GetPairName( it->first );
					break;
				}
			}
			if( ! strRet.IsEmpty() )
				break;
		}
		if( strRet.IsEmpty() )
		{
			// リンク列から探す
			for( CRuleLinkQueue::const_iterator it = g_cRuleLinkQueue.begin(); it != g_cRuleLinkQueue.end(); ++it )
			{
				for( CRuleLinkQueue::TRuleValue::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
				{
					if( 0 == it2->get<0>().CompareNoCase( lpcszTableName ) ||
						0 == it2->get<1>().CompareNoCase( lpcszTableName ) )
					{
						strRet = ( bIsLinkName ) ? it->first : g_cRuleModelNetwork.GetPairName( it->first );
						break;
					}
				}
			}
		}
	}
	
	return strRet;
}

/// ネットワークリンクフィーチャクラス名を返す
CString AheGetNetworkLinkFeatureClassName( LPCTSTR lpcszTableName )
{
	return _AheGetNetworkFeatureClassName( lpcszTableName, true );
}

/// ネットワークノードフィーチャクラス名を返す
CString AheGetNetworkNodeFeatureClassName( LPCTSTR lpcszTableName )
{
	return _AheGetNetworkFeatureClassName( lpcszTableName, false );
}

// ネットワークモデルでの関連テーブル名一覧を取得する
CString AheGetNetworkTarget( LPCTSTR lpcszTableName )
{
	// 関連テーブル名はスプリット処理テーブル名一覧＋ネットワークノード
	CString strSplitTarget( AheGetNetworkSplitTarget( lpcszTableName ) );
	CString strRet;
	strRet.Format(_T("%s%s|"), strSplitTarget, AheGetNetworkNodeFeatureClassName( lpcszTableName ) );
	return strRet;
}

// ネットワークモデルでのスプリット処理に必要なライン系テーブル名一覧を取得する
CString AheGetNetworkSplitTarget( LPCTSTR lpcszTableName, bool bContainNW )
{
	// 渡されたテーブル名がネットワークモデルのものなら、そのモデルのラインと
	// そのラインと内包関係にあるラインを引っ張ってきて「|」で区切って返す
	CString strRet; // 返り値

	// まず、ネットワークモデルのラインを特定する
	CString strNetworkLink( AheGetNetworkLinkFeatureClassName( lpcszTableName ) );
	if( strNetworkLink.IsEmpty() )
		return strRet; // 見つからなかった

	if(bContainNW)
		strRet.Format(_T("|%s"), strNetworkLink ); // 追加

	// 内包するラインフィーチャクラスを追加する
	for( CRuleRelationshipContain::const_iterator it = g_cRuleRelationshipContain.begin(); it != g_cRuleRelationshipContain.end(); ++it )
	{
		if( 0 == it->first.Compare( strNetworkLink ) )
		{
			for( CRuleRelationshipContain::TRuleValue::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
				strRet.AppendFormat(_T("|%s"), *it2 ); // 追加
		}
	}

	// 空でないなら | を閉じる
	if( !strRet.IsEmpty() ) strRet.Append(_T("|"));
	return strRet;
}

// ネットワークリンクフィーチャクラス名の一覧を取得する
CString _AheGetNetworkNames( bool bIsLink )
{
	CString strRet; // 返り値
	for( CRuleModelNetwork::const_iterator it = g_cRuleModelNetwork.begin(); it != g_cRuleModelNetwork.end(); ++it )
	{
		if( strRet.IsEmpty() )
			strRet.Format(_T("|%s|"), ( bIsLink ) ? it->first : it->second );
		else {
			CString strRetTmp( strRet );
			strRet.Format(_T("%s%s|"), strRetTmp, ( bIsLink ) ? it->first : it->second );
		}
	}
	return strRet;
}

// ネットワークリンクフィーチャクラス名の一覧を取得する
CString AheGetNetworkLinks()
{
	return _AheGetNetworkNames( true );
}

// ネットワークノードフィーチャクラス名の一覧を取得する
CString AheGetNetworkNodes()
{
	return _AheGetNetworkNames( false );
}

// ネットワークリンクフィーチャクラスを返す
IFeatureClassPtr AheGetNetworkLinkFeatureClass( IFeature* ipFeature )
{
	return AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipFeature ), AheGetNetworkLinkFeatureClassName( AheGetFeatureClassName( ipFeature ) ) );
}

// ネットワークノードフィーチャクラスを返す
IFeatureClassPtr AheGetNetworkNodeFeatureClass( IFeature* ipFeature )
{
	return AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipFeature ), AheGetNetworkNodeFeatureClassName( AheGetFeatureClassName( ipFeature ) ) );
}

// 渡されたテーブル名に付随する関連フラグフィールド名のリストを取得する
std::list<CString> AheGetRelationFlagFieldName( LPCTSTR lpcszTableName )
{
	std::list<CString> listRet; // 返り値

	for( CRuleRelationTable::const_value_iterator it = g_cRuleRelationTable.begin( lpcszTableName ); it != g_cRuleRelationTable.end( lpcszTableName ); ++it )
		listRet.push_back( it->get<0>() );

	return listRet;
}

// 関連フラグフィールド名と対応する外部テーブルの外部キーフィールド名を取得する
CString AheGetRelationOuterKeyFieldName( LPCTSTR lpcszTableName, LPCTSTR lpcszRelFieldName )
{
	CString strRet; // 返り値
	for( CRuleRelationTable::const_value_iterator it = g_cRuleRelationTable.begin( lpcszTableName ); it != g_cRuleRelationTable.end( lpcszTableName ); ++it )
	{
		if( 0 == it->get<0>().CompareNoCase( lpcszRelFieldName ) )
		{
			strRet = it->get<2>();
			break;
		}
	}
	return strRet;
}

// 関連フラグフィールド名と対応する外部テーブル名を取得する
CString AheGetRelationTableName( LPCTSTR lpcszTableName, LPCTSTR lpcszRelFieldName )
{
	CString strRet; // 返り値
	for( CRuleRelationTable::const_value_iterator it = g_cRuleRelationTable.begin( lpcszTableName ); it != g_cRuleRelationTable.end( lpcszTableName ); ++it )
	{
		if( 0 == it->get<0>().CompareNoCase( lpcszRelFieldName ) )
		{
			strRet = it->get<1>();
			break;
		}
	}
	return strRet;
}

// フィーチャに関連する外部テーブルをオープンする
ITablePtr AheOpenRelationTable( IFeature* ipFeature, LPCTSTR lpcszRelFieldName )
{
	if( ! ipFeature )
		return NULL;

	return AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipFeature ), AheGetRelationTableName( AheGetFeatureClassName(ipFeature), lpcszRelFieldName ) );
}

// ネットワークノードに接続されているネットワークリンクを取得
LONG AheGetConnectedNetworkLink( IFeaturePtr ipFeature, CTargetItems& cTargetItems )
{
	// ネットワークノード限定
	if( ipFeature == NULL && AheGetFeatureKinds( ipFeature ) != sindyeNodeFeature )
	{
		_ASSERTE( FALSE );
		return -1;
	}

	ISpatialFilterPtr ipFilter;

	if( SUCCEEDED( ipFilter.CreateInstance( CLSID_SpatialFilter ) ) )
	{
		IQueryFilterPtr ipQueryFilter( ipFilter );
		if( SUCCEEDED( ipFilter->QueryInterface( &ipQueryFilter) ) )
		{
			// ネットワークリンククラスを渡されたフィーチャから検索する
			IFeatureClassPtr ipNWLinkClass( AheGetNetworkLinkFeatureClass(ipFeature) );
			if( ! ipNWLinkClass )
			{
				_ASSERTE( ipNWLinkClass );
				return NULL;
			}

			// ジオメトリの取得
			IGeometryPtr ipGeom;
			ipFeature->get_Shape( &ipGeom );
			if( ipGeom == NULL )
			{
				_ASSERTE( ipGeom != NULL );
				return NULL;
			}

			// クエリフィルタの作成
			ipFilter->putref_Geometry( ipGeom );
			CComBSTR cComBSTR;
			ipNWLinkClass->get_ShapeFieldName( &cComBSTR );
			ipFilter->put_GeometryField( cComBSTR );
			ipFilter->put_SpatialRel( esriSpatialRelIntersects );

			// 検索
			IFeatureCursorPtr ipFeatureCursor;
			if( SUCCEEDED( ipNWLinkClass->Search( ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor ) ) && NULL != ipFeatureCursor )
			{
				IFeaturePtr ipFeatureTmp = NULL;
				while( S_OK == ipFeatureCursor->NextFeature( &ipFeatureTmp ) )
					cTargetItems.push_back( ipFeatureTmp );
			}
		}
	}

	return cTargetItems.size();
}

// ネットワークリンクが接続しているノードフィーチャを取得
LONG AheGetConnectedNetworkNode( IFeaturePtr ipFeature, CTargetItems& cTargetItems )
{
	// ネットワークリンク限定
	if( ipFeature == NULL && AheGetFeatureKinds( ipFeature ) != sindyeRoadFeature )
	{
		_ASSERTE( FALSE );
		return -1;
	}

	ISpatialFilterPtr ipFilter;

	if( SUCCEEDED( ipFilter.CreateInstance( CLSID_SpatialFilter ) ) )
	{
		IQueryFilterPtr ipQueryFilter( ipFilter );
		if( SUCCEEDED( ipFilter->QueryInterface( &ipQueryFilter ) ) )
		{
			// 渡されたフィーチャと同じワークスペースからノードレイヤを見つける
			IFeatureClassPtr ipNWNodeClass( AheGetNetworkNodeFeatureClass(ipFeature) );
			if( ! ipNWNodeClass )
			{
				_ASSERTE( ipNWNodeClass );
				return NULL;
			}

			// ジオメトリの取得
			IGeometryPtr ipGeom;
			ipFeature->get_Shape( &ipGeom );
			if( ipGeom == NULL )
			{
				_ASSERTE( ipGeom != NULL );
				return NULL;
			}

			// クエリフィルタの作成
			ipFilter->putref_Geometry( ipGeom );
			CComBSTR cComBSTR;
			ipNWNodeClass->get_ShapeFieldName( &cComBSTR );
			ipFilter->put_GeometryField( cComBSTR );
			ipFilter->put_SpatialRel( esriSpatialRelTouches );

			// 検索
			IFeatureCursorPtr ipFeatureCursor;
			if( SUCCEEDED( ipNWNodeClass->Search( ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor ) ) && NULL != ipFeatureCursor )
			{
				IFeaturePtr ipFeatureTmp;
				while( S_OK == ipFeatureCursor->NextFeature( &ipFeatureTmp ) )
					cTargetItems.push_back( ipFeatureTmp );
			}
		}
	}

	return cTargetItems.size();
}

// ネットワークリンクが接続しているノードフィーチャを取得
bool AheGetConnectedNetworkLinkAndNode( IFeaturePtr ipSelectRoad, IFeaturePtr ipSelectNode, IFeaturePtr &ipConnectedNetworkLink, IFeaturePtr &ipOppNode )
{
	CTargetItems cNetworkLinkItems;
	// ノードについているすべてのネットワークリンクを取得
	AheGetConnectedNetworkLink( ipSelectNode, cNetworkLinkItems );
	
	for( CTargetItems::iterator iRoad = cNetworkLinkItems.begin(); iRoad != cNetworkLinkItems.end(); iRoad++ )
	{
		IFeaturePtr ipRoadFeature = iRoad->GetFeature();

		// いま選択中のネットワークリンクでなければ、欲しい道路
		LONG lSelectRoadOID, lRoadOID;
		ipSelectRoad->get_OID( &lSelectRoadOID );
		ipRoadFeature->get_OID( &lRoadOID );

		if( lSelectRoadOID != lRoadOID )
		{
			// 反対側のネットワークリンクを確保
			ipConnectedNetworkLink = ipRoadFeature;

			CTargetItems	cNodeItems;
			// ネットワークリンクについているノードを取得
			AheGetConnectedNetworkNode( ipRoadFeature, cNodeItems );

			for(CTargetItems::iterator iNode = cNodeItems.begin(); iNode != cNodeItems.end(); iNode++ )
			{
				IFeaturePtr ipNodeFeature = iNode->GetFeature();

				// 今選択中のノードでなければ、もう片方のノード
				LONG lSelectNodeOID, lNodeOID;
				ipSelectNode->get_OID( &lSelectNodeOID );
				ipNodeFeature->get_OID( &lNodeOID );

				if( lSelectNodeOID != lNodeOID )
				{
					// 反対側のノードを確保
					ipOppNode = ipNodeFeature;


					// ノード種別を取得
					IFieldsPtr		ipNodeFields;
					CComVariant		vNodeClass;
					long			lNodeClass = 0;
					sindyeNodeClass	sNodeClass; 

					ipNodeFeature->get_Fields(&ipNodeFields);
					ipNodeFields->FindField(CComBSTR(_T("NODECLASS_C")), &lNodeClass );
					ipNodeFeature->get_Value(lNodeClass, &vNodeClass);
					sNodeClass = sindyeNodeClass(vNodeClass.lVal);

					// 2次メッシュ上だったら、両端とも2次メッシュノード
					if( sNodeClass == sindyeOnMesh || sNodeClass == sindyeOnMeshPEC || sNodeClass == 99 )//WALK_NODEの区画辺上ノード=99
						return true;
				}
			}
		}
	}
	return false;
}

// 2次メッシュを跨いでいるネットワークリンクを取得する
bool AheIsNetworkLinkCrossMesh( IMapPtr ipMap, IFeaturePtr ipFeature, CTargetItems &cNetworkLinkCrossMesh )
{
	// PGDBであった場合には、最初でふるい落とす
	IDatasetPtr ipDataset( AheGetFeatureClass( ipFeature ) );
	if( ipDataset == NULL )
		return false;

	IFeatureClassPtr ipFeatureClass = NULL;

	// フィーチャクラス取得
	if( AheGetFeatureClass( ipMap, ipFeature, &ipFeatureClass ) )
	{
		// 道路リンクかどうか？
		if( AheIsNetworkLink( ipFeatureClass ) )
		{
			CTargetItems cNodeItems;

			// 道路リンクについているノードを取得
			LONG lNodeNum = AheGetConnectedNetworkNode( ipFeature, cNodeItems );

			// 接続しているノードの個数が2個でなかったら、おかしいので、ここで終了
			// [Bug2606]関連で、まだメッシュ境界にノードが2個重なっている場合があるので、それへの対応
			// 一本の道路リンクに接続しているノードは必ず2個である。
			if( lNodeNum != 2 )
				return false;

			for( CTargetItems::iterator iCnt = cNodeItems.begin(); iCnt != cNodeItems.end(); iCnt++ )
			{
				IFeaturePtr ipNodeFeature = iCnt->GetFeature();
				CTargetItems cRoadItems;

				// ノード種別を取得
				IFieldsPtr		ipNodeFields;
				CComVariant		vNodeClass;
				long			lNodeClass = 0;
				sindyeNodeClass	sNodeClass; 

				ipNodeFeature->get_Fields(&ipNodeFields);
				ipNodeFields->FindField(CComBSTR(_T("NODECLASS_C")), &lNodeClass );
				ipNodeFeature->get_Value(lNodeClass, &vNodeClass);
				sNodeClass = sindyeNodeClass(vNodeClass.lVal);


				// 2次メッシュ上だったら、反対側の道路を取得
				if( sNodeClass == sindyeOnMesh || sNodeClass == sindyeOnMeshPEC || sNodeClass == 99 )//WALK_NODEの区画辺上ノード=99
				{
					CTargetItems cConnectedRoadOnMesh;
					IFeaturePtr ipOppNode;
					IFeaturePtr ipRoadsOnMesh;
					cConnectedRoadOnMesh.clear();

					IFeaturePtr ipTempFeature = ipFeature;

					bool bIsBothNodeOnMesh = false;

					do{
						bIsBothNodeOnMesh = AheGetConnectedNetworkLinkAndNode( ipTempFeature, ipNodeFeature, ipRoadsOnMesh, ipOppNode );

						// 反対側の道路を確保しておく
						cConnectedRoadOnMesh.push_back( ipRoadsOnMesh );

						// 選択された反対側の道路にまた2次メッシュノードがついている場合、道路リンクとノードを
						// 置き換えて、続けて調べる
						if( bIsBothNodeOnMesh )
						{
							ipTempFeature = ipRoadsOnMesh;
							ipNodeFeature = ipOppNode; 
						}
					}while( bIsBothNodeOnMesh );

					// 選択された、2次メッシュ関連の道路を登録する
					for( CTargetItems::iterator it = cConnectedRoadOnMesh.begin(); it != cConnectedRoadOnMesh.end(); it++ )
					{
						IFeaturePtr ipFeatureOnMesh = it->GetFeature();
						cNetworkLinkCrossMesh.push_back( ipFeatureOnMesh );
					}
				}
			}
		}
	}
	if( cNetworkLinkCrossMesh.size() > 0 )
		return true;
	return false;
}

// 共通のノードIDリストを取得する（AheGetRelationshipContainFeatureで使用）
std::list<long> _AheGetCommonNodeID( const CTargetItems& cItems )
{
	std::list<long> listRet; // 戻り値

	// 共通のノードIDリストを取得する
	// ただし、cItemsが一つの場合は解決できないのでその場合に限り
	// 両端を返す
	long lNodeID = -1, lFromDummy = -1, lToDummy = -1;
	for( CTargetItems::const_iterator it = cItems.begin(); it != cItems.end(); ++it )
	{
		if( 0 > lFromDummy && 0 > lToDummy )
		{
			lFromDummy = it->GetAttribute( _T("FROM_NODE_ID") ).lVal;
			lToDummy   = it->GetAttribute( _T("TO_NODE_ID") ).lVal;
		}
		else {
			long lFromDummy2 = it->GetAttribute(_T("FROM_NODE_ID")).lVal;
			long lToDummy2   = it->GetAttribute(_T("TO_NODE_ID")).lVal;
			if( lFromDummy == lFromDummy2 || lFromDummy == lToDummy2 )
				lNodeID = lFromDummy;
			else if( lToDummy == lFromDummy2 || lToDummy == lToDummy2 )
				lNodeID = lToDummy;

			break; // 2回しか通さない（時間の無駄）
		}
	}
	if( 0 > lNodeID ) // セグメントにスナップさせて引いた場合
	{
		listRet.push_back( lFromDummy );
		listRet.push_back( lToDummy );
	}
	else
		listRet.push_back( lNodeID );

	return listRet;
}

// Containルール関係にあるもののうち関連させるべきもの（パス）を取得する
CTargetItems AheGetRelationshipContainFeature( IFeaturePtr ipWithinFeature, const CTargetItems& cContainFeatures )
{
	CTargetItems cRet; // 戻り値

	if( ! ipWithinFeature ) // 引数チェック
		return cRet;

	// ipWithinFeatureの両端点が接するものをcContainFeaturesから検索
	IGeometryPtr ipGeom;
	ipWithinFeature->get_Shape( &ipGeom );
	IPolylinePtr ipPolyline( ipGeom ); // 今のところラインしか想定していない
	if( ipPolyline )
	{
		IPointPtr ipFrom, ipTo;
		ipPolyline->get_FromPoint( &ipFrom );
		ipPolyline->get_ToPoint( &ipTo );

		CTargetItems cFromFeatures; // FROM側に接するフィーチャ群
		CTargetItems cToFeatures; // TO側に接するフィーチャ群

		// FROM、TOのどちらに接するか分ける
		for( CTargetItems::const_iterator it = cContainFeatures.begin(); it != cContainFeatures.end(); ++it )
		{
			IGeometryPtr ipContainGeom( it->GetShape() ); // 「含む」側フィーチャのジオメトリ
			if( AheTestGeometryRelationship( ipFrom, ipContainGeom, (sindyeSpatialRel)(sindyeSpatialRelTouch|sindyeSpatialRelWithin) ) )
				cFromFeatures.push_back( *it );
			if( AheTestGeometryRelationship( ipTo, ipContainGeom, (sindyeSpatialRel)(sindyeSpatialRelTouch|sindyeSpatialRelWithin) ) )
				cToFeatures.push_back( *it );
		}
		// 例外として、FROM、TOどちらにも含まれる場合はそれ以外にありえないので、そのケースをはじいておく
		cFromFeatures.sort();
		cToFeatures.sort();
		std::set_intersection( cFromFeatures.begin(), cFromFeatures.end(), cToFeatures.begin(), cToFeatures.end(), std::back_inserter(cRet) );

		_ASSERTE( cRet.empty() || 1 == cRet.size() ); // 空か、一つであるはず

		if( cRet.empty() )
		{
			using namespace path_analyzer;

			// FROM、TOを結べるパスをすべて調べる
			graph<CTargetItems,edge<CTargetItem> > cGraph( cContainFeatures );
			std::list<path<edge<CTargetItem> > > listPath;
			cGraph.create_path( _AheGetCommonNodeID( cFromFeatures ), _AheGetCommonNodeID( cToFeatures ), listPath );
#ifdef _DEBUG
			cGraph.trace_path(listPath);
#endif // ifdef _DEBUG
			// どのパスがふさわしいのか検証
			// 優先順位は以下のとおり：
			// 1. ipWithinFeatureのend_pointが接していること（=FROM、TOリストのうち一つを持っていること）
			// 2. ipWithinFeatureと2次元レベルで共有していること（一番長いもの）
			// 3. ipWithinFeatureと1次元レベルで共有していること（一番多いもの）
			if( 1 < listPath.size() )
			{
				// 1. の検証
				for( std::list<path<edge<CTargetItem> > >::iterator itPath = listPath.begin(); itPath != listPath.end(); )
				{
					bool bFindFrom = false, bFindTo = false;
					for( path<edge<CTargetItem> >::const_iterator it = itPath->begin(); it != itPath->end(); ++it )
					{
						if( ! bFindFrom ) bFindFrom = ( std::find( cFromFeatures.begin(), cFromFeatures.end(), (*it)->__edge_native ) == cFromFeatures.end() ) ? false : true;
						if( ! bFindTo   ) bFindTo   = ( std::find( cToFeatures.begin(),   cToFeatures.end(),   (*it)->__edge_native ) == cToFeatures.end()   ) ? false : true;

						if( bFindFrom && bFindTo )
							break;
					}

					if( ! ( bFindFrom && bFindTo ) )
						itPath = listPath.erase( itPath );
					else
						++itPath;
				}
			}
			if( 1 < listPath.size() )
			{
				// 2. の検証
				ITopologicalOperatorPtr ipTopo( ipGeom );
				std::map<double, path<edge<CTargetItem> > > mapPath; // 共通セグメント長でソート
				for( std::list<path<edge<CTargetItem> > >::const_iterator itPath = listPath.begin(); itPath != listPath.end(); ++itPath )
				{
					double dLen = 0.0f; // 共通セグメント長
					for( path<edge<CTargetItem> >::const_iterator it = itPath->begin(); it != itPath->end(); ++it )
					{
						IGeometryPtr ipResultGeom;
						if( SUCCEEDED( ipTopo->Intersect( (*it)->__edge_native.GetShape(), esriGeometry1Dimension, &ipResultGeom ) ) )
						{
							double dLenTmp = 0.0f;
							((ICurvePtr)ipResultGeom)->get_Length( &dLenTmp );
							dLen += dLenTmp;
						}
						else
							_ASSERTE( false ); // Intersectに失敗
					}
					if( 0 < dLen )
						mapPath[dLen] = *itPath;
				}
				if( ! mapPath.empty() )
				{
					listPath.clear();
					listPath.push_back( mapPath.rbegin()->second );
				}
			}
			if( 1 < listPath.size() )
			{
				// 3. の検証
				ITopologicalOperatorPtr ipTopo( ipGeom );
				std::map<int, path<edge<CTargetItem> > > mapPath; // 交差する点の数でソート
				for( std::list<path<edge<CTargetItem> > >::const_iterator itPath = listPath.begin(); itPath != listPath.end(); ++itPath )
				{
					int lCount = 0; // 交差する点の数
					for( path<edge<CTargetItem> >::const_iterator it = itPath->begin(); it != itPath->end(); ++it )
					{
						IGeometryPtr ipResultGeom;
						if( SUCCEEDED( ipTopo->Intersect( (*it)->__edge_native.GetShape(), esriGeometry0Dimension, &ipResultGeom ) ) )
						{
							long lCountTmp = 0;
							((IPointCollectionPtr)ipResultGeom)->get_PointCount( &lCountTmp );
							if( 0 < lCountTmp )
								lCount += lCountTmp;
						}
						else
							_ASSERTE( false ); // Intersectに失敗
					}
					if( 0 < lCount )
						mapPath[lCount] = *itPath;
				}
				if( ! mapPath.empty() )
				{
					listPath.clear();
					listPath.push_back( mapPath.rbegin()->second );
				}
			}
			if( ! listPath.empty() )
			{
				for( path<edge<CTargetItem> >::const_iterator it = listPath.begin()->begin(); it != listPath.begin()->end(); ++it )
					cRet.push_back( (*it)->__edge_native );
			}
		}
	}

	return cRet;
}

// 歩行者NWの汎用モデルかダウンコンバートモデルかを判定する関数
sindy::schema::walk_type::type_class::ECode AheIsDownconvert( IFeaturePtr ipFeature )
{
	using namespace sindy::schema::walk_type::type_class;
	// デフォルト返却値は歩道あり版（汎用版）
	ECode walktype = kGeneral;

	// 渡されたフィーチャのデータセットを取得し
	IFeatureClassPtr ipFeatureClass = AheGetFeatureClass( ipFeature );
	CString strFeatureClassName = AheGetFeatureClassName( ipFeatureClass );
	// 歩行者リンクか歩行者ノードじゃない関数から呼ばれたら、本来ここで落ちたほうがいい
	if( !(strFeatureClassName == walk_link::kTableName || strFeatureClassName == walk_node::kTableName) )
		_ASSERTE( false );
	// 取得したフィーチャクラスと同データセット内にWALK_TYPEがあるかどうかを調べる。
	ITablePtr ipWalkTypeTable = AheOpenSameOwnerTable( (ITablePtr)ipFeatureClass, walk_type::kTableName );
	// WALK_TYPEテーブルがない場合には歩道あり版ということになる
	if( !ipWalkTypeTable )
		return walktype;

	// 以下WALK_TYPEがある場合に歩道あり版なのか、ダウンコンバート版なのかの判定
	// 全件検索
	_ICursorPtr ipCursor;
	ipWalkTypeTable->Search( AheInitQueryFilter(), VARIANT_FALSE, &ipCursor );

	// 1件のみデータが入っている前提だが、もし複数入っていたら、最後のデータの値を取る
	_IRowPtr ipRow;
	int i = 0;
	while( ipCursor->NextRow( &ipRow ) == S_OK && ipRow )
	{
		CComVariant vaValue = AheGetAttribute( ipRow, walk_type::kWalkTypeClass );
		walktype = (ECode)vaValue.lVal;
		++i;
		// 1件以上格納するようなことがあったら、ここで落ちるべき
		if( i > 1 )
			_ASSERTE( false );
	}

	return walktype;
};