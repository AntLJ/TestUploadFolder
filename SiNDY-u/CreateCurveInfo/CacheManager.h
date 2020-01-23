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

#pragma once
#include "RoadLinkTable.h"
#include <boost/shared_ptr.hpp>

namespace curve_info
{
	using namespace adlib;
	using namespace sindy;

	typedef boost::shared_ptr< CTableFinder > CSPTableFinder;

	/**
	 * @brief DBから読み込んだデータのキャッシュを管理するクラス
	 */
	class CacheManager
	{
	private:
		typedef std::map< adlib::MeshCode, CSPTableFinder > Cache;
		Cache m_Cache;			///< メッシュ単位のキャッシュ

		IFeatureClassPtr		m_pRoadLinkClass;		///< 道路リンクフィーチャクラス
		IFeatureClassPtr		m_pRoadNodeClass;		///< 道路ノードフィーチャクラス
		IFeatureClassPtr		m_pBaseMeshClass;		///< ２次メッシュフィーチャクラス

		CModel m_Model;			///< sindylib_baseのモデル定義

	public:
		/// コンストラクタ
		CacheManager(
			IFeatureClassPtr cpRoadLinkClass,
			IFeatureClassPtr cpRoadNodeClass,
			IFeatureClassPtr cpBaseMeshClass
		);

		/// デストラクタ
		~CacheManager() {}

		/// sindylib_baseで使用するモデルを定義する
		void defineSindynModel();

		/// ２次メッシュフィーチャクラスを返す
		const IFeatureClassPtr meshClass() const { return m_pBaseMeshClass; }

		/// キャッシュが存在するメッシュのメッシュコード群を返す
		MeshCodes cachedMeshCodes() const;

		/**
		 * @brief 指定されたメッシュのキャッシュが既に存在するか否かを判定する
		 * @param[in]	cMeshCode	対象メッシュ
		 * @return	判定結果
		 */
		bool isCached( long cMeshCode ) const;

		/**
		 * @brief 指定されたメッシュのキャッシュを返す
		 * @note	キャッシュが存在しなければ作成する。std::map::operator[]と同様の考え方。
		 * @note	TODO: できれば、cQueryWhereClauseの指定により非常に重い検索が実行された場合、
		 *			強制中断してサーバに致命的な負荷を与えないようにする仕組みがあった方が良い。タイムアウトを設けるとか？
		 * @@param[in]	cMeshCode	対象メッシュ
		 * @@param[in]	cQueryWhereClause	対象リンク指定クエリ (bug9617)
		 * @return	キャッシュされたデータを取得するためのテーブルファインダ
		 */
		const CSPTableFinder cache( long cMeshCode, CString cQueryWhereClause );

		/**
		 * @brief 指定されたメッシュのキャッシュを解放する
		 * @param[in]	cMeshCode	対象メッシュ
		 */
		void clear( long cMeshCode );

		/**
		 * @brief 指定されたメッシュ群以外のキャッシュを全て解放する
		 * @param[in]	cMeshCode	対象メッシュ
		 */
		void clearExcept( MeshCodes cMeshCodes );

		/**
		 * @brief 読み込み済みメッシュの中から、指定されたリンクが存在するメッシュを探し、そのメッシュコードを返す
		 * @param[in]	cLinkID	対象リンクID
		 * @return	メッシュコード（見つからなかった場合は0）
		 */
		long existingMeshCode( long cLinkID ) const;
	};
	typedef std::tr1::shared_ptr< CacheManager > CacheManagerPtr;

} // namespace curve_info

