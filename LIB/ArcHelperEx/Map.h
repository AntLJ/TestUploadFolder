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
 * @brief <b>CMap定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏\n
 * @version $Id$
 */
#ifndef _MAP_H_
#define _MAP_H_

#include <atl2/seal.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AheBugEvasion.h"

/**
 * @brief レイヤの種類
 * LayerIteratorやCMapLayersで使用します
 */
enum iterator_type
{
	LayerTypeAny,             //!< 全てのレイヤ
	LayerTypeFeatureLayer,    //!< フィーチャレイヤ
	LayerTypeGraphicsLayer,   //!< グラフィックス（ラスタなど）レイヤ
	LayerTypeGroupLayer,      //!< グループレイヤ
	LayerTypeDataLayer,
};

/**
 * @class LayerIterator
 * @brief レイヤイテレータクラス
 * CMapLayersで使用します
 */
class LayerIterator
{
public:
	LayerIterator() : editableonly_(false), visibleonly_(false)
	{
	}
	LayerIterator( IMap* map__, iterator_type type__ = LayerTypeAny, bool editableonly__ = false, bool visibleonly__ = false )
	{
		if( ! map__ )
			return;

		maplayers_ = map__;
		editableonly_ = editableonly__;
		visibleonly_ = visibleonly__;

		IUIDPtr ipUID( CLSID_UID );
		switch( type__ )
		{
			case LayerTypeAny:
				ipUID->put_Value( CComVariant(L"{34c20002-4d3c-11d0-92d8-00805f7c28b0}") ); // ILayer
				break;
			case LayerTypeFeatureLayer:
				ipUID->put_Value( CComVariant(L"{E156D7E5-22AF-11D3-9F99-00C04F6BC78E}") ); // IGeoFeatureLayer
				break;
			case LayerTypeGraphicsLayer:
				ipUID->put_Value( CComVariant(L"{34B2EF81-F4AC-11D1-A245-080009B6F22B}") ); // IGraphicsLayer
				break;
			case LayerTypeGroupLayer:
				ipUID->put_Value( CComVariant(L"{EDAD6644-1810-11D1-86AE-0000F8751720}") ); // IGroupLayer
				break;
			case LayerTypeDataLayer:
				ipUID->put_Value( CComVariant(L"{6CA416B1-E160-11D2-9F4E-00C04F6BC78E}") ); // IDataLayer
			default:
				_ASSERTE( false ); // 他のを使用したい場合は追加してください
				ipUID->put_Value( CComVariant(L"{34c20002-4d3c-11d0-92d8-00805f7c28b0}") ); // ILayer
				break;
		}
		if( SUCCEEDED( map__->get_Layers( ipUID, VARIANT_TRUE, &layers_ ) ) && layers_ != NULL )
			layers_->Reset();

		operator++();
	}
	virtual ~LayerIterator()
	{
		layers_ = NULL;
	}
	const LayerIterator& operator++() const
	{
		if( ! is_end() )
		{
			layers_->Next( &current_layer_ );

			if( current_layer_ != NULL )
			{
				if( visibleonly_ && ( ! is_visible( maplayers_, current_layer_ ) ) )   // 表示されているかどうかの判定
					operator++();

				if( editableonly_ && ( ! is_editable( current_layer_ ) ) ) // 編集可能かどうかの判定
					operator++();
			}
		}

		if( current_layer_ == NULL )
			layers_ = NULL;

		return *this;
	}
	ILayerPtr operator->() const { return current_layer_; }
	ILayerPtr operator*() const { return current_layer_; }
	ITablePtr getTable() const
	{
		IFeatureLayerPtr flayer__( current_layer_ );
		if( flayer__ )
		{
			IFeatureClassPtr class__;
			flayer__->get_FeatureClass( &class__ );
			return (ITablePtr)class__;
		}
		return NULL;
	}
	bool operator==( const LayerIterator& iterator__ ) const
	{
		if( (IUnknown*)layers_ == (IUnknown*)iterator__.layers_ &&
			(IUnknown*)current_layer_ == (IUnknown*)iterator__.current_layer_ )
			return true;
		else
			return false;
	}
	bool operator!=( const LayerIterator& iterator__ ) const
	{
		return ! ( *this == iterator__ );
	}
	static bool is_visible( IMapLayers* maplayers__, ILayer* layer__ )
	{
		if( maplayers__ == NULL || ( ! layer__ ) )
			return false;

		VARIANT_BOOL vb = VARIANT_FALSE;
		maplayers__->IsLayerVisible( layer__, &vb );
		return ( vb ) ? true : false;
	}
	static bool is_editable( ILayer* layer__ )
	{
		if( ! layer__ )
			return false;

		IFeatureLayerPtr featlayer__( layer__ );
		if( featlayer__ != NULL )
		{
			IFeatureClassPtr featclass__;
			featlayer__->get_FeatureClass( &featclass__ );
			IDatasetPtr dataset__( featclass__ );
			if( dataset__ != NULL )
			{
				IWorkspacePtr workspace__;
				IDataset_getWorkspace( dataset__, &workspace__ ); // 赤びっくり回避ラッパーで取得
				IWorkspaceEditPtr editspace__( workspace__ );
				if( editspace__ != NULL )
				{
					VARIANT_BOOL vb = VARIANT_FALSE;
					editspace__->IsBeingEdited( &vb );
					return ( vb ) ? true : false;
				}
			}
		}
		return false;
	}
private:
	bool is_end() const
	{
		if( layers_ == NULL )
			return true;
		else
			return false;
	}
private:
	IMapLayersPtr maplayers_;
	mutable IEnumLayerPtr layers_;
	mutable ILayerPtr current_layer_;
	bool editableonly_;
	bool visibleonly_;
};

/**
 * @class CMap
 * @brief <b>IMapラップクラス</b>\n
 *
 * CMapLayers cLayers( ipMap );
 * for( CMapLayers::iterator it = cLayers.begin(); it != cLayers.end(); ++it )
 * {
 *     ...
 * }
 * みたいに使ってください。
 * begin()で引数を与えることで、例えばフィーチャレイヤのみとか、編集可能
 * のみとか表示されているもののみとかに絞り込んで取得することが可能です。
 * 引数を何も指定しなければ、全てのレイヤが順番に取得されます。
 * 多階層グループレイヤに対応しています。
 */
 class CMap : public atl2::seal<CComPtr<IMap> >
{
public:
	CMap()
	{
	}

	virtual ~CMap()
	{
	}

	CMap( IMap* lp )
	{
		*this = lp;
	}
	CMap& operator=( IMap* lp )
	{
        if(*this!=lp)
    		AtlComPtrAssign((IUnknown**)&p, lp);

        return *this;
	}

	//@{ @name IMap インターフェース（一部）
	ILayerPtr GetLayer( long index ) const;
	long GetLayerCount() const;
	IEnumLayerPtr GetLayers( IUID* UID, bool recursive ) const;
	double GetMapScale() const;
	//@}
	/**
	 * @brief <b>フィーチャレイヤのみのレイヤセットを返す</b>\n
	 * 内部でGetLayersを使用します。
	 * @param recursive	[in]	グループレイヤ内も対象に入れる場合はtrue
	 * @retval NULL	エラー又はフィーチャレイヤが存在しない
	 * @retval NULL以外 IEnumLayerPtr
	 */
	IEnumLayerPtr GetFeatureLayers( bool recursive ) const;

	/**
	 * @brief <b>TOC上のグループレイヤを全て取得する</b>\n
	 * @note 内部でGetLayersを使用します。
	 * @param recursive	[in]	グループレイヤ内も対象に入れる場合はtrue
	 * @retval NULL	エラー又はグループレイヤが存在しない
	 * @retval NULL以外 IEnumLayerPtr
	 */
	IEnumLayerPtr GetGroupLayers( bool recursive ) const;

	/**
	 * @brief <b>引数に渡したレイヤが所属するグループレイヤを取得する</b>\n
	 * @note 内部でGetGroupLayersを使用します。
	 * @param ipLayer [in] グループレイヤを取得するレイヤ
	 * @retval NULL	エラー又はフィーチャレイヤが存在しない
	 * @retval NULL以外 ICompositeLayerPtr
	 */
	ICompositeLayerPtr GetParentCompositeLayer( ILayerPtr ipLayer ) const;

#if 0
	/**
	 * @brief <b>レイヤを検索する</b>\n
	 * テーブル名以外をNULLにすると、その部分は一致判定に使用しません。
	 * lpcszServerName		[in]	サーバ名
	 * lpcszVersionName		[in]	バージョン名
	 * lpcszOwnerName		[in]	オーナー名
	 * lpcszTableName		[in]	テーブル名
	 * @retval NULL	エラー又は見つからない
	 * @retval NULL以外 ILayerPtr
	 */
	ILayerPtr FindLayer( LPCTSTR lpcszServerName, LPCTSTR lpcszVersionName, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>レイヤを検索する</b>\n
	 * テーブル名以外をNULLにすると、その部分は一致判定に使用しません。
	 * lpcszServerName		[in]	サーバ名
	 * lpcszVersionName		[in]	バージョン名
	 * lpcszOwnerTableName	[in]	オーナー.サーバ名
	 * @retval NULL	エラー又は見つからない
	 * @retval NULL以外 ILayerPtr
	 */
	ILayerPtr FindLayer( LPCTSTR lpcszServerName, LPCTSTR lpcszVersionName, LPCTSTR lpcszOwnerTableName ) const;
#endif // if 0
	/**
	 * @brief <b>レイヤを検索する</b>\n
	 * テーブル名以外をNULLにすると、その部分は一致判定に使用しません。
	 * @param ipWorkspace    [in] ワークスペース
	 * @param lpcszOwnerName [in] オーナー名
	 * @param lpcszTableName [in] テーブル名
	 * @param first_only     [in,optional] 最初の1つを見つけ次第returnする（デフォルト：false）
	 * @return 見つかったILayerPtrのリスト
	 */
	std::list<CAdapt<ILayerPtr>> FindLayers( const IWorkspacePtr& ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName, bool first_only = false ) const;

	/**
	 * @brief <b>レイヤを検索する</b>\n
	 * テーブル名以外をNULLにすると、その部分は一致判定に使用しません。
	 * 複数存在する場合は最初に見つけたレイヤを返します
	 * @param ipWorkspace			[in]	ワークスペース
	 * @param lpcszOwnerName		[in]	オーナー名
	 * @param lpcszTableName		[in]	テーブル名
	 * @retval NULL	エラー又は見つからない
	 * @retval NULL以外 ILayerPtr
	 */
	ILayerPtr FindLayer( IWorkspace* ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>レイヤを検索する</b>\n
	 * テーブル名以外をNULLにすると、その部分は一致判定に使用しません。
	 * 複数存在する場合は最初に見つけたレイヤを返します
	 * @param ipWorkspace			[in]	ワークスペース
	 * @param lpcszOwnerTableName	[in]	オーナー.テーブル名
	 * @retval NULL	エラー又は見つからない
	 * @retval NULL以外 ILayerPtr
	 */
	ILayerPtr FindLayer( IWorkspace* ipWorkspace, LPCTSTR lpcszOwnerTableName ) const;

	/**
	 * @brief <b>フィーチャクラスに対応するレイヤを検索する</b>\n
	 * @note フィーチャクラスに対応するレイヤが複数存在する場合は
	 * 一番始めに見つかったものを返す。
	 * @param ipFeatureClass [in] IFeatureClassインスタンス
	 * @retval NULL	エラー又は見つからない
	 * @retval NULL以外 指定したフィーチャクラスに対応するレイヤ
	 */
	ILayerPtr FindLayer( const IFeatureClassPtr ipFeatureClass ) const;

	/**
	 * @brief <b>指定レイヤと同じグループに所属するレイヤを全て検索する</b>\n
	 * @detail 所属グループがないもの同士は同じグループに所属するものとして扱う
	 * @param ipUnknown [in] IFeature、IFeatureClass、ILayerオブジェクトのうちのどれか
	 * @param lpcszOTableName [in] レイヤのテーブル名
	 * @return 見つかったILayerPtrのリスト
	 */
	std::list<CAdapt<ILayerPtr>> FindSameGroupLayers( const IUnknownPtr ipUnknown, LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>指定レイヤと同じグループに所属するレイヤを検索する</b>\n
	 * @detail 所属グループがないもの同士は同じグループに所属するものとして扱う。<br>
	 * 指定のテーブル名をもつレイヤが複数ある場合、一番初めに見つかったレイヤを返す。
	 * @param ipUnknown [in] IFeature、IFeatureClass、ILayerオブジェクトのうちのどれか
	 * @param lpcszOTableName [in] レイヤのテーブル名
	 * @retval NULL	エラー又は見つからない
	 * @retval NULL以外 ILayerPtr
	 */
	ILayerPtr FindSameGroupLayer( const IUnknownPtr ipUnknown, LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>グループレイヤからレイヤを全て検索する</b>\n
	 * @param ipTargetComposite  [in] 検索対象のグループレイヤ
	 * @param ipWorkspace        [in] ワークスペース
	 * @param lpcszOwnerName     [in] オーナー名
	 * @param lpcszOTableName    [in] 検索するテーブル名
	 * @param bOnlyDirectlyUnder [in,optional] グループレイヤの直下のみ検索するかどうか（デフォルト：false）
	 * @return 見つかったILayerPtrのリスト
	 */
	static std::list<CAdapt<ILayerPtr>> FindLayersFromComposite( const ICompositeLayerPtr ipTargetComposite, const IWorkspacePtr ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName, bool bOnlyDirectlyUnder = false );

	/**
	 * @brief <b>グループレイヤからレイヤを検索する</b>\n
	 * @note 同じグループに複数の同名フィーチャクラスがあった場合は
	 * 一番最初に見つかったものを返す
	 * @param ipTargetComposite  [in] 検索対象のグループレイヤ
	 * @param ipWorkspace        [in] ワークスペース
	 * @param lpcszOwnerName     [in] オーナー名
	 * @param lpcszOTableName    [in] 検索するテーブル名
	 * @param bOnlyDirectlyUnder [in,optional] グループレイヤの直下のみ検索するかどうか（デフォルト：false）
	 * @retval NULL	エラー又は見つからない
	 * @retval NULL以外 ILayerPtr
	 */
	static ILayerPtr FindLayerFromComposite( const ICompositeLayerPtr ipTargetComposite, const IWorkspacePtr ipWorkspace, LPCTSTR lpcszOwnerName, LPCTSTR lpcszTableName, bool bOnlyDirectlyUnder = false);

public:
	//@{ @name イテレータ
	typedef LayerIterator iterator;
	typedef const LayerIterator const_iterator;
	const_iterator begin( iterator_type type = LayerTypeAny, bool editableonly = false, bool visibleonly = false ) const { return LayerIterator( p, type, editableonly, visibleonly ); }
	iterator begin( iterator_type type = LayerTypeAny, bool editableonly = false, bool visibleonly = false ) { return LayerIterator( p, type, editableonly, visibleonly ); }
	const_iterator end() const { return LayerIterator(); }
	iterator end() { return LayerIterator(); }
	//@}
};

#endif // ifndef _MAP_H_
