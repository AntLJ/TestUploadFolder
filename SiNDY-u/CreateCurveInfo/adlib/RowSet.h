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
#include "ADLib.h"

namespace adlib
{
	namespace row_set
	{
		typedef std::map< OID, _IRowPtr > OIDTable;	///< オブジェクトIDによる検索用コンテナ

		/**
		 * @brief オブジェクトIDで検索可能なRowの集合
		 * @note	出典：SiNDYQToMPQ
		 */
		class RowSet
		{
		public:
			RowSet(void) {}
			virtual ~RowSet(void) {}

			/**
			 * @brief 与えられたフィーチャカーソルからオブジェクト群を読み込む
			 * @param[in]	crRowCursor	対象フィーチャカーソル
			 */
			void load( _ICursor& crCursor );

			/**
			 * @brief オブジェクトIDによる検索
			 * @param[in]	cOID	検索するリンクのオブジェクトID
			 * @return	リンクのフィーチャ（見つからない場合はNULLを返す）
			 */
			_IRowPtr searchByOID( long cOID ) const;

			/**
			 * @brief 全コンテナの全要素を削除
			 * @note	派生クラスで個別にコンテナを持つならオーバーライドする必要あり
			 */
			virtual void clear(void);

			/**
			 * @brief OIDTableへの参照を読み取り専用で渡す
			 */
			const OIDTable& getOIDTable(void) const { return m_OIDTable; }

			/**
			 * @brief 読み込みが完了されているか否かを判定する
			 * @return	判定結果
			 */
			bool isLoaded() const { return m_OIDTable.size() != 0; }

		private:
			OIDTable m_OIDTable;					///< オブジェクトIDによる検索用コンテナ
		};
	} // namespace row_set
} // namespace adlib
