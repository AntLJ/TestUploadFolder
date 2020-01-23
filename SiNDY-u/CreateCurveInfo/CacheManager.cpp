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

#include "StdAfx.h"
#include "CacheManager.h"
#include "RowContainer.h"
#include "CurveInfoUtility.h"

namespace curve_info
{
	CacheManager::CacheManager(
		IFeatureClassPtr cpRoadLinkClass,
		IFeatureClassPtr cpRoadNodeClass,
		IFeatureClassPtr cpBaseMeshClass
	) :
		m_pRoadLinkClass( cpRoadLinkClass ),
		m_pRoadNodeClass( cpRoadNodeClass ),
		m_pBaseMeshClass( cpBaseMeshClass )
	{
		// sindylib_baseを使用するための準備
		init_sindynlib();
		defineSindynModel();
	}

	void CacheManager::defineSindynModel()
	{
		using namespace sindy::schema;

		// リンクをトップとしたモデル
		m_Model.Add( road_link::kTableName, kObjectID, NULL, NULL );
		// リンク列のうち、必要なのは右左折禁止だけ
		m_Model.Add( lq_turnreg::kTableName,	link_queue::kLinkID,	road_link::kTableName,	kObjectID			);
		m_Model.Add( inf_turnreg::kTableName,	kObjectID,				lq_turnreg::kTableName,	link_queue::kInfID	);

		// 下記の理由により、ノードは上記モデルに含めない。
		//		リンクにはノードとの関連付けをするキーがFrom_Node_IDとTo_Node_IDの２種類あるが、
		//		現状のsindylib_baseのモデルの仕様はキーが複数ある事に対応していない。
		//		また、今回の用途では関連付いていなくても特に困らない。
		//		よって、ノードはリンクとは無関係のテーブルとして扱うものとし、モデル上では定義しない。
	}

	MeshCodes CacheManager::cachedMeshCodes() const
	{
		MeshCodes aMeshCodes;
		BOOST_FOREACH (const Cache::value_type& rElement, m_Cache)
		{
			aMeshCodes.insert( rElement.first );
		}
		return aMeshCodes;
	}

	bool CacheManager::isCached( long cMeshCode ) const
	{
		return m_Cache.find( cMeshCode ) != m_Cache.end();
	}

	/**
	 * @brief sindylib_baseのCSPContainerBaseのソート基準を定義するプレディケート
	 * @note	sindylib_baseから得られるリンク群、ノード群をID順に並べるために必要。
	 */
	struct lessCSPContainerBase
	{
		bool operator() ( const CSPContainerBase& lhs, const CSPContainerBase& rhs ) const
		{
			using namespace utility::sindylib_base;
			return irow( lhs )->OID < irow( rhs )->OID;
		}
	};

	const CSPTableFinder CacheManager::cache( long cMeshCode, CString cQueryWhereClause )
	{
		using namespace sindy;
		using namespace utility::sindylib_base;

		if (! isCached( cMeshCode ))
		{
			// メッシュのフィーチャを取得
			IFeaturePtr ipMesh( getMeshFeature( m_pBaseMeshClass, cMeshCode ) );
			// メッシュのフィーチャが見つからなければ失敗
			if (! ipMesh)
			{
				throw std::runtime_error("メッシュのフィーチャ取得に失敗");
			}

			// テーブルファインダを作成
			CSPTableFinder pTableFinder( new CTableFinder );
			m_Cache[ cMeshCode ] = pTableFinder;
			pTableFinder->InitCollection( AheGetTable( m_pRoadLinkClass), m_Model );

			// メッシュ内の全リンクを取得
			IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
			ipQueryFilter = AheGetSpatialFilter( ipMesh->GetShapeCopy(), m_pRoadLinkClass, esriSpatialRelContains );

			// コマンドライン引数で与えられたクエリを追加 (bug9617)
			if (! cQueryWhereClause.IsEmpty() )
			{
				ipQueryFilter->PutWhereClause( _bstr_t( cQueryWhereClause ) );
			}

			// メッシュ内のリンクと、モデル上でそれに関連付くオブジェクト群（リンク→LQ→Infまで）をキャッシュ
			using namespace schema;
			CSPTableContainer pLinkTable = pTableFinder->FindTable( road_link::kTableName );		///< リンクテーブル
			pLinkTable->Select(
				std::map< CString,IQueryFilterPtr >( 
					boost::assign::map_list_of(
						CString( road_link::kTableName ),
						ipQueryFilter
					)
				)
			);
			pLinkTable->sort( lessCSPContainerBase() );
			// 上記で読み込んだInf_Turnregに属するLQ群を読み込む
			road_link_table( pLinkTable )->SelectLQs( false, false, linkqueue_type::kLRTurn );

			// メッシュ内のノードをキャッシュ
			CSPTableContainer pNodeTable = pTableFinder->FindTable( road_node::kTableName );		///< ノードテーブル
			pNodeTable->Select(
				std::map< CString,IQueryFilterPtr >( 
					boost::assign::map_list_of(
						CString( road_node::kTableName ),
						AheGetSpatialFilter( ipMesh->GetShapeCopy(), m_pRoadNodeClass, esriSpatialRelIntersects )
					)
				)
			);
			pNodeTable->sort( lessCSPContainerBase() );
		}

		return m_Cache[ cMeshCode ];
	}

	void CacheManager::clear( long cMeshCode )
	{
		m_Cache.erase( cMeshCode );
	}

	void CacheManager::clearExcept( MeshCodes cMeshCodes )
	{
		// キャッシュを解放するメッシュのメッシュコード群を作成
		MeshCodes aClearMeshCodes;
		BOOST_FOREACH (Cache::value_type& rElement, m_Cache)
		{
			long aMeshCode = rElement.first;

			// 除外メッシュリストに載っているメッシュは初期化しない
			if (cMeshCodes.find( aMeshCode ) == cMeshCodes.end())
			{
				aClearMeshCodes.insert( aMeshCode );
			}
		}

		// 解放
		BOOST_FOREACH (long aMeshCode, aClearMeshCodes)
		{
			clear( aMeshCode );
		}
	}

	long CacheManager::existingMeshCode( long cLinkID ) const
	{
		using namespace sindy::schema;

		BOOST_FOREACH (const Cache::value_type& rElement, m_Cache)
		{
			const CSPTableFinder pTableFinder = rElement.second;
			const CSPTableContainer pLinkTable = pTableFinder->FindTable( road_link::kTableName );
			CContainerBase::iterator it = pLinkTable->find_by_key( kObjectID, cLinkID );
			if (it != pLinkTable->end())
			{
				return rElement.first;
			}
		}
		return 0;
	}

} // namespace curve_info

