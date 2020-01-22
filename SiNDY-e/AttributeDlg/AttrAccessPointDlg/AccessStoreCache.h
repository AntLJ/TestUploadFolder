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

#include <sindy/schema.h>
#include <boost/operators.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

typedef std::basic_string<TCHAR> tstring;

namespace access_store_cache
{
	/**
	 * @brief フィールド
	 * @note	「Access_Storeテーブルの１フィールド」及び「Access_Store情報グリッドの１セル」に当たるデータ。
	 *			グリッドに格納される値は全て整数値またはbool値なので、long型で実装。
	 */
	struct CASField
		: private boost::operators<CASField>
	{
		CASField() : m_OrgValue( 0 ), m_NewValue( 0 ) {}
		CASField( long cOrgValue ) : m_OrgValue( cOrgValue ), m_NewValue( cOrgValue ) {}

		long m_OrgValue;		///< 元（読み込んだ時）の値
		long m_NewValue;		///< 更新後の値（初期値はm_OrgValueと同じ）

		/**
		 * @brief 大小比較演算子
		 * @note	更新後の値だけを比較基準とする。
		 * @param[in]	crField	比較対象フィールド
		 * @return	比較結果
		 */
		bool operator< ( const CASField& crField ) const
		{
			return m_NewValue < crField.m_NewValue;
		}

		/**
		 * @brief 等価比較演算子
		 * @note	更新後の値だけを比較基準とする。
		 * @param[in]	crField	比較対象フィールド
		 * @return	比較結果
		 */
		bool operator== ( const CASField& crField ) const
		{
			return m_NewValue == crField.m_NewValue;
		}

		/**
		 * @brief 元の値と更新後の値が異なるか否かを判定する
		 * @return	判定結果
		 */
		bool isUpdated() const
		{
			return m_OrgValue != m_NewValue;
		}
	};
	typedef tstring FieldName;
	typedef std::map<FieldName, CASField> CASFields;

	/**
	 * @brief 行
	 * @note	「Access_Storeテーブルの１行」及び「Access_Store情報グリッドの１行」に当たるデータ。
	 */
	struct CASRow
	{
		CASRow() : m_IsAdded(false), m_IsDeleted(false) {}

		CASFields m_Fields;		///< フィールド群
		bool m_IsAdded;										///< 追加された行であるか否か
		bool m_IsDeleted;									///< 削除された行であるか否か
		_IRowPtr m_pRow;									///< 情報の取得元_IRowPtr（追加された行ならNULL）

		/**
		 * @brief 指定されたフィールド名を持つフィールドへの参照を返す
		 * @note	存在しないフィールド名が指定された場合はstd::runtime_error例外を送出する。
		 *			基本的には、使い方に間違いがなければ起こらないはず。
		 *			データモデルのフィールド名が変わったり、フィールドが削除されたり、キャッシュに正常に格納できていない場合に起こる。
		 * @param[in]	cFieldName	フィールド名
		 * @return	フィールド
		 */
		const CASField& validField( tstring cFieldName ) const
		{
			CASFields::const_iterator itField = m_Fields.find( cFieldName );
			if (itField == m_Fields.end())
			{
				throw std::runtime_error("CASRow::validValue()失敗");
			}
			return itField->second;
		}
	};
	typedef long ObjectID;
	typedef std::map<ObjectID, CASRow> CASRows;

	typedef long MasterCode;	///< チェーン店マスターコード
	typedef long SundayFlag;	///< 日曜日フラグ
	typedef boost::tuple<MasterCode, SundayFlag, ObjectID> CSortedASRowsKey;	///< グリッド内でのソート基準となる値の集合
	typedef std::map<CSortedASRowsKey, CASRow> CSortedASRows;					///< グリッド用にソートされたCASRowの集合

	/**
	 * @brief Access_Storeテーブルのキャッシュ
	 * @note	グリッドに対する変更をすぐにDBに反映させなくてよいようにするためのデータ。
	 *			グリッド上で値が変更されたらまずキャッシュを変更し、「保存」の指示があったらまとめてDBに反映する。
	 */
	class CASCache
	{
	public:
		CASCache(void);
		~CASCache(void);

		/**
		 * @brief 全データ削除
		 */
		void clear()
		{
			m_Rows.clear();
			m_SortedRows.clear();
		}

		/**
		 * @brief 行データを追加する
		 * @note	「追加」ボタンが押された時用。アクセスポイントID以外の全フィールドが初期値の行を追加する。
		 * @param[in]	cAccessPointID	行データが関連付くアクセスポイントのID
		 */
		void addRow( long cAccessPointID );

		/**
		 * @brief IRowPtrから取得した情報を格納した行データを追加する
		 * @param[in]	追加する行データの元になるRowオブジェクト
		 */
		void addRow( _IRow* cpRow );

		/**
		 * @brief 指定されたオブジェクトIDを持つ行の指定されたフィールドを更新する
		 * @note	対象行またはフィールドが存在しなければ何もしない。
		 * @param[in]	cObjectID	対象オブジェクトID
		 * @param[in]	cFieldName	対象フィールド名
		 * @param[in]	cValue		フィールドに格納する値
		 */
		void updateField( long cObjectID, tstring cFieldName, long cValue );

		/**
		 * @brief 指定されたオブジェクトIDを持つ行の削除フラグを反転させる
		 * @note	指定されたオブジェクトIDを持つ行が存在しなければ何もしない。
		 * @param[in]	cObjectID	対象オブジェクトID
		 */
		void changeDeleteFlag( long cObjectID );

		/**
		 * @brief 
		 */
		const CASRows& rows() const { return m_Rows; }

		/**
		 * @brief ソート済み行群を求めて返す
		 * @note	m_Rowsの行群をグリッド用に並び替えて返す。ソート済み行群の有無に関わらずソートを行う。
		 * @return	ソート済み行群
		 * @retval	NULL	失敗（CASField::validField()失敗時）
		 */
		const CSortedASRows* sortRows();

		/**
		 * @brief ソート済み行群を返す
		 * @note	格納されているソート済み行群への参照を返す。ソート済み行群が作成済みかどうかはチェックしない。
		 * @return	ソート済み行群
		 */
		const CSortedASRows& sortedRows();

	private:
		/**
		 * @brief 指定されたオブジェクトIDを持つ行を返す
		 * @param[in]	cObjectID	対象オブジェクトID
		 * @return	行（見つからなければNULL）
		 */
		CASRow* getRow( long cObjectID );

		/**
		 * @brief 指定されたフィールド名を持つフィールドを返す
		 * @param[in]	cObjectID	対象オブジェクトID
		 * @param[in]	cFieldName	対象フィールド名
		 * @return	フィールド（見つからなければNULL）
		 */
		CASField* getField( long cObjectID, tstring cFieldName );

		/**
		 * @brief 仮オブジェクトIDを発行して返す
		 * @note	仮オブジェクトIDとは、「追加」ボタンで作成され、DBに未反映の行に与えられるオブジェクトID。
		 *			DBから取得した行と被らないようにするため、負の値とする。
		 *			仮オブジェクトIDを持つ行が既にあればその内最小のもの-1を返し、なければ-1を返す。
		 */
		long getTempObjectID() const;

		CASRows m_Rows;		///< 行群
		CSortedASRows m_SortedRows;	///< グリッド用にソートされた行群
	};

} // namespace access_store_cache
