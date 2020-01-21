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
 * @file Map.h
 * @brief <b>CMap実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏\n
 * @version $Id$
 */
#include "stdafx.h"
#include "Map.h"
#include "type_convert.h"
#include "AheLayerFunctions.h"
#include "GlobalFunctions.h"
#include <TDC/useful_headers/str_util.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{

/**
 * @brief グループレイヤからレイヤを検索する機能を実装した関数
 * @note FindLayersFromComposite, FindLayerFromComposite で呼び出される。
 * bFirstOnlyオプションを外部公開しないために定義した関数となる。
 * @param ipTargetComposite  [in] 検索対象のグループレイヤ
 * @param ipWorkspace        [in] ワークスペース
 * @param lpcszOwnerName     [in] オーナー名
 * @param lpcszOTableName    [in] 検索するテーブル名
 * @param bOnlyDirectlyUnder [in] グループレイヤの直下のみ検索するかどうか
 * @param bFirstOnly         [in] 最初の1つを見つけ次第returnする（デフォルト：false）
 * @retval NULL	エラー又は見つからない
 * @retval NULL以外 ILayerPtr
 */
inline std::list<CAdapt<ILayerPtr>> FindFromComposite( const ICompositeLayerPtr ipTargetComposite, const IWorkspacePtr ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName, bool bOnlyDirectlyUnder, bool bFirstOnly = false )
{
	std::list<CAdapt<ILayerPtr>> layers;
	if(!ipTargetComposite) return layers;

	long layerNum = -1L;
	if(FAILED(ipTargetComposite->get_Count(&layerNum))) return layers;
	for( long i=0; i<layerNum; ++i )
	{
		ILayerPtr ipLayer;
		if( FAILED( ipTargetComposite->get_Layer(i, &ipLayer) ) || !ipLayer )
			continue;

		if( IFeatureClassPtr ipFeatureClass = AheGetFeatureClass(ipLayer) )
		{
			// テーブル名を比較
			if( lpcszTableName )
			{
				if( 0 != AheGetFeatureClassName( ipFeatureClass ).CompareNoCase( lpcszTableName ) )
					 continue;
			}
			// オーナー名が渡されている場合はオーナー名を比較する
			if( lpcszOwnerName )
			{
				if( 0 != AheGetFeatureClassOwnerName( ipFeatureClass ).CompareNoCase( lpcszOwnerName ) )
					continue;
			}
			// ワークスペースが渡されている場合はインスタンスを比較する
			if( ipWorkspace )
			{
				IDatasetPtr ipDataset( ipFeatureClass );
				if( !ipDataset ) continue;
				IWorkspacePtr ipDatasetWorkspace;
				if( FAILED(ipDataset->get_Workspace( &ipDatasetWorkspace )) )
					continue;
				if( ipDatasetWorkspace != ipWorkspace ) continue;
			}
			layers.push_back( ipLayer );
			if( bFirstOnly ) return layers;
		}

		// レイヤ直下しか検索しないなら以降の処理は実行しない
		if(bOnlyDirectlyUnder) continue;

		// グループレイヤなら内部を検索
		if( ICompositeLayerPtr ipCompositeLayer = ipLayer )
		{
			std::list<CAdapt<ILayerPtr>> childLayers = FindFromComposite( ipCompositeLayer, ipWorkspace, lpcszOwnerName, lpcszTableName, bOnlyDirectlyUnder, bFirstOnly );
			if(childLayers.empty()) continue;
			if( bFirstOnly ) return childLayers;
			// TODO c++11対応したならspliceにすること
			layers.insert( layers.end(), childLayers.begin(), childLayers.end() );
		}
	}
	return layers;
};

} // namespace


IEnumLayerPtr CMap::GetLayers( IUID* UID, bool recursive ) const
{
	IEnumLayerPtr ipLayers;	// 返り値

	if( ! UID )
		return NULL;

	if( FAILED( (*this)->get_Layers( UID, bool2VB(recursive), &ipLayers ) ) )
		_ASSERTE(false);

	return ipLayers;
}

// フィーチャレイヤのみのレイヤセットを返す
IEnumLayerPtr CMap::GetFeatureLayers( bool recursive ) const
{
	IEnumLayerPtr ipLayers;	// 返り値

	IUIDPtr ipUID( CLSID_UID );

	if( SUCCEEDED( ipUID->put_Value( _variant_t(L"{E156D7E5-22AF-11D3-9F99-00C04F6BC78E}") ) ) )
		ipLayers = GetLayers( ipUID, recursive );
	else
		_ASSERTE(false);

	return ipLayers;
}

IEnumLayerPtr CMap::GetGroupLayers( bool recursive ) const
{
	IEnumLayerPtr ipLayers;	// 返り値

	IUIDPtr ipUID( CLSID_UID );

	if( SUCCEEDED( ipUID->put_Value( _variant_t(L"{EDAD6644-1810-11D1-86AE-0000F8751720}") ) ) )
		ipLayers = GetLayers( ipUID, recursive );
	else
		_ASSERTE(false);

	return ipLayers;
}

ICompositeLayerPtr CMap::GetParentCompositeLayer( ILayerPtr ipLayer ) const
{
	// グループレイヤー群を取得する
	IEnumLayerPtr ipEnumLayer = GetGroupLayers( true );
	ipEnumLayer->Reset();

	ILayerPtr ipGroupLayer;
	while( ipEnumLayer->Next( &ipGroupLayer ) == S_OK && ipGroupLayer )
	{
		ICompositeLayerPtr ipCompositeLayer = ipGroupLayer;
		long lLayerCount = 0;
		ipCompositeLayer->get_Count( &lLayerCount );
		for( int i = 0; i < lLayerCount; ++i )
		{
			ILayerPtr ipCheckLayer;
			ipCompositeLayer->get_Layer( i, &ipCheckLayer );
			if( ipLayer == ipCheckLayer )
				return ipCompositeLayer;
		}
	}

	return NULL;
}

#if 0
// レイヤを検索する
ILayerPtr CMap::FindLayer( LPCTSTR/* lpcszServerName*/, LPCTSTR/* lpcszVersionName*/, LPCTSTR/* lpcszOwnerName*/, LPCTSTR/* lpcszTableName*/ ) const
{
	ILayerPtr ipLayer;	// 返り値

	LOGASSERTE(false,sindyErr_NoImplimentation );

	return ipLayer;
}

// レイヤを検索する
ILayerPtr CMap::FindLayer( LPCTSTR/* lpcszServerName*/, LPCTSTR/* lpcszVersionName*/, LPCTSTR/* lpcszOwnerTableName*/ ) const
{
	ILayerPtr ipLayer;	// 返り値

	LOGASSERTE(false,sindyErr_NoImplimentation );

	return ipLayer;
}
#endif // if 0

// レイヤを検索する
std::list<CAdapt<ILayerPtr>> CMap::FindLayers( const IWorkspacePtr& ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName, bool first_only /*= false*/ ) const
{
	std::list<CAdapt<ILayerPtr>> layers;

	if( !lpcszTableName )
		return layers;

	for( const_iterator it = begin(); it != end(); ++it )
	{
		ITablePtr ipTable( it.getTable() );
		if( !ipTable )
			continue;

		// テーブル名を比較
		if( 0 != AheGetFeatureClassName( ipTable ).CompareNoCase( lpcszTableName ) )
			continue;

		// オーナー名が渡されている場合はオーナー名を比較する
		if( lpcszOwnerName )
		{
			if( 0 != AheGetFeatureClassOwnerName( ipTable ).CompareNoCase( lpcszOwnerName ) )
				continue;
		}

		// ワークスペースが渡されている場合はプロパティセットを比較する
		if( ipWorkspace )
		{
			IDatasetPtr ipDataset( ipTable );
			if( ipDataset )
			{
				IWorkspacePtr ipDatasetWorkspace;
				IPropertySetPtr ipWSProp, ipDSProp;
				if( SUCCEEDED( ipDataset->get_Workspace( &ipDatasetWorkspace ) ) &&
					SUCCEEDED( ipDatasetWorkspace->get_ConnectionProperties( &ipDSProp ) ) &&
					SUCCEEDED( ipWorkspace->get_ConnectionProperties( &ipWSProp ) ) )
				{
					if( AheIsSamePropertySet( ipWSProp, ipDSProp ) ) 
						layers.push_back(*it);
				}
				else
					_ASSERTE(false);
			}
			else
				_ASSERTE(false);
		}
		else
			layers.push_back(*it);

		// 最初の1つを見つけたら終了
		if( first_only && !layers.empty() )
			return layers;
	}
	return layers;
}

// レイヤを検索する
ILayerPtr CMap::FindLayer( IWorkspace* ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName ) const
{
	std::list<CAdapt<ILayerPtr>> layers = FindLayers( ipWorkspace, lpcszOwnerName, lpcszTableName, true );
	return layers.empty() ? NULL : layers.begin()->m_T;
}

// レイヤを検索する
ILayerPtr CMap::FindLayer( IWorkspace* ipWorkspace, LPCTSTR lpcszOwnerTableName ) const
{
	ILayerPtr ipLayer;	// 返り値

	if( !lpcszOwnerTableName )
		return NULL;

	const std::vector<CString> owner_table = uh::str_util::split( lpcszOwnerTableName, _T(".") );

	if( 2 != owner_table.size() )
		return NULL;

	return FindLayer( ipWorkspace, owner_table[0], owner_table[1] );
}

// フィーチャクラスに対応するレイヤを検索する
ILayerPtr CMap::FindLayer( const IFeatureClassPtr ipFeatureClass ) const
{
	// フィーチャレイヤ群を取得する
	IEnumLayerPtr ipEnumLayer = GetFeatureLayers( true );
	ipEnumLayer->Reset();

	ILayerPtr ipLayer;
	while( ipEnumLayer->Next( &ipLayer ) == S_OK && ipLayer )
	{
		IFeatureLayerPtr ipFeatureLayer = ipLayer;
		IFeatureClassPtr ipCheckedFeatureClass;
		if( FAILED( ipFeatureLayer->get_FeatureClass(&ipCheckedFeatureClass) ) )
			continue;
		if( ipCheckedFeatureClass == ipFeatureClass )
			return ipLayer;
	}
	return NULL;
}

// レイヤと同じグループに所属する全てのレイヤを検索する
std::list<CAdapt<ILayerPtr>> CMap::FindSameGroupLayers( const IUnknownPtr ipUnknown, LPCTSTR lpcszTableName ) const
{
	ILayerPtr ipLayer = ipUnknown;
	// ipUnknown がレイヤでないなら、対応するレイヤを検索する
	if( !ipLayer )
	{
		ipLayer = FindLayer(ipUnknown);
		if( !ipLayer ) return std::list<CAdapt<ILayerPtr>>();
	}

	// グループレイヤに所属するなら、グループレイヤ内を検索
	if( ICompositeLayerPtr ipCompositeLayer = GetParentCompositeLayer( ipLayer ) )
		return FindLayersFromComposite( ipCompositeLayer, NULL, NULL, lpcszTableName, true );

	// グループレイヤに所属しないなら、TOCの直下にあるフィーチャクラスを検索
	std::list<CAdapt<ILayerPtr>> layers;
	for( const_iterator it = begin(); it != end(); ++it )
	{
		if( ICompositeLayerPtr(*it) ) continue;
		if( 0!=AheGetFeatureClassName( *it ).CompareNoCase( lpcszTableName ) )
			continue;

		// グループレイヤ内のレイヤなら対象外
		if( GetParentCompositeLayer( *it ) ) continue;
		layers.push_back(*it);
	}
	return layers;
}

// 指定レイヤと同じグループに所属するレイヤを検索する
ILayerPtr CMap::FindSameGroupLayer( const IUnknownPtr ipUnknown, LPCTSTR lpcszTableName ) const
{
	ILayerPtr ipLayer = ipUnknown;
	// ipUnknown がレイヤでないなら、対応するレイヤを検索する
	if( !ipLayer )
	{
		ipLayer = FindLayer(ipUnknown);
		if( !ipLayer ) return NULL;
	}

	// グループレイヤに所属するなら、グループレイヤ内を検索
	if( ICompositeLayerPtr ipCompositeLayer = GetParentCompositeLayer( ipLayer ) )
		return FindLayerFromComposite( ipCompositeLayer, NULL, NULL, lpcszTableName, true );

	// グループレイヤに所属しないなら、TOCの直下にあるフィーチャクラスを検索
	for( const_iterator it = begin(); it != end(); ++it )
	{
		if( ICompositeLayerPtr(*it) ) continue;
		if( 0!=AheGetFeatureClassName( *it ).CompareNoCase( lpcszTableName ) )
			continue;

		// グループレイヤ内のレイヤなら対象外
		if( GetParentCompositeLayer( *it ) ) continue;
		return *it;
	}
	return NULL;
}

// グループレイヤから条件を満たすレイヤを全て検索する
std::list<CAdapt<ILayerPtr>> CMap::FindLayersFromComposite( const ICompositeLayerPtr ipTargetComposite, const IWorkspacePtr ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName, bool bOnlyDirectlyUnder/*= false*/ )
{
	return FindFromComposite( ipTargetComposite, ipWorkspace, lpcszOwnerName, lpcszTableName, bOnlyDirectlyUnder );
}

// グループレイヤからレイヤを検索する
ILayerPtr CMap::FindLayerFromComposite( const ICompositeLayerPtr ipTargetComposite, const IWorkspacePtr ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName, bool bOnlyDirectlyUnder /*= false*/ )
{
	std::list<CAdapt<ILayerPtr>> layers = FindFromComposite( ipTargetComposite, ipWorkspace, lpcszOwnerName, lpcszTableName, bOnlyDirectlyUnder, true );
	return layers.empty() ? NULL : layers.begin()->m_T;
}
