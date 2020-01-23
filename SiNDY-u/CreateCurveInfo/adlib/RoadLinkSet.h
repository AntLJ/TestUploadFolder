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

#include <map>
#include <set>
#include "RowSet.h"
#include "ADLib.h"

namespace adlib
{
	namespace road_link_set
	{
		using namespace row_set;

		typedef IFeaturePtr RoadLink;

		// FromNodeIDまたはToNodeIDによる検索用コンテナの型の定義
		typedef std::multimap<NodeID, RoadLink> NodeIDTable;
		typedef std::pair<NodeIDTable::iterator, NodeIDTable::iterator> NodeIDTableRange;

		// From/ToノードIDの組による検索用コンテナの型の定義
		typedef NodeID FromNodeID;
		typedef NodeID ToNodeID;
		typedef std::pair<FromNodeID, ToNodeID> FromToNodeID;
		typedef std::map<FromToNodeID, RoadLink> FromToNodeIDTable;

		typedef std::pair<FromNodeID, ToNodeID> NodeIDPair;

		/**
		 * @brief RowSetをリンク用に特化したクラス
		 * @note	出典：SiNDYQToMPQ
		 */
		class RoadLinkSet : public RowSet
		{
		public:
			RoadLinkSet(void);
			~RoadLinkSet(void);

			/**
			 * @brief FromノードIDによる検索
			 * @param[in]	cNodeID	FromノードID
			 */
			NodeIDTableRange searchByFromNodeID(long cNodeID) const;

			/**
			 * @brief ToノードIDによる検索
			 * @param[in]	cNodeID	ToノードID
			 */
			NodeIDTableRange searchByToNodeID(long cNodeID) const;

			/**
			 * @brief From/ToノードIDの組による検索
			 * @note	与えられた２つのノードをFrom/Toノードとするリンクを返す
			 * @param[in]	cNodeID1	FromまたはToノードID
			 * @param[in]	cNodeID2	FromまたはToノードID
			 */
			IFeaturePtr searchByFromToNodeID(long cNodeID1, long cNodeID2) const;

			/**
			 * @brief FromまたはToノードIDによる検索
			 * @note	与えられたノードIDを持つノードをFromまたはToノードとするリンク群のIDを返す
			 * @param[in]	cNodeID	対象ノードID
			 * @return	リンクID群
			 */
			LinkIDs searchByNodeID( long cNodeID ) const;

			/**
			 * @brief 全コンテナの全要素を削除
			 */
			void clear(void);

			/**
			 * @brief リンクIDからFrom/ToノードIDを取得
			 * @param[in]	cOID	リンクのオブジェクトID
			 * @return	見つかったリンクのFrom/ToノードID（見つからない場合は(0,0)のペアを返す）
			 */
			NodeIDPair getFromToNodeID( long cOID ) const;

			/**
			 * テスト用 RoadLinkSetの中身を指定ストリームにダンプする
			 */
			void dump(std::ostream& co) const;

		private:
			mutable NodeIDTable m_FromNodeIDTable;			//!< FromノードIDによる検索用コンテナ
			mutable NodeIDTable m_ToNodeIDTable;			//!< ToノードIDによる検索用コンテナ
			mutable FromToNodeIDTable m_FromToNodeIDTable;	//!< From/ToノードIDの組による検索用コンテナ
		};
	} // namespace road_link_set
} // namespace adlib
