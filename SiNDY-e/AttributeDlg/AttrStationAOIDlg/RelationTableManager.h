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

#include <atlstr.h>
#include <FieldMap.h>
#include <list>
#include "CommonData.h"

/**
 * @brief あるテーブルとあるテーブルを関連付けるためだけに存在するテーブルを操作するためのクラス
 * @warning  データの追加・削除のどちらにおいても、使用するフィールドが同じ場合にしか扱えない
 */

class CRelationTableManager
{
public:
	/**
	* @brief 削除するデータのリストとなるデータ型
	* @note  例えば、あるフィールドの値が0,1,2のものを削除したいという場合、
	* @note  このリストの要素は頭から順に[0][1][2]となる
	*/
	typedef std::list<CString> DELETELIST;

	/**
	* @brief 削除するデータの組み合わせとなるリストを格納するデータ型
	* @note  例えば、フィールド1の値が"Hoge",フィールド2の値が0のものと
	* @note  例えば、フィールド1の値が"Pugya",フィールド2の値が1のものを
	* @note  削除したい場合、このリストの要素は頭から順に[["Hoge"][0]][["Pugya"][1]]となる
	*/
	typedef std::list<std::list<CString>> DELETELISTS;

	/**
	 * @brief 初期化処理
	 * @warning  このインスタンスを使用する前に、必ず一度だけ呼び出すこと
	 * @param[in] strTable  関連テーブル名
	 * @param[in] strKey    主キーとなるフィールド名
	 * @param[in] subFields 値の取得・挿入・削除の際に使用するフィールド名のリスト
	 * @param[in] ipRow     この引数と同じオーナーのテーブルを開く
	 */
	void Initialize( const CString& strTable, const CString& strKey, const FIELDNAMES& subFields, _IRowPtr& ipRow );

	/*
	* @brief 関連テーブルにデータを追加
	* @param[in] lOID    選択中のフィーチャーのOID
	* @param[in] lList   追加するデータのリスト(1要素=1行 1要素の中に各カラムのデータが入っている)
	* @retval true  データの追加に成功
	* @retval false データの追加に失敗
	*/
	bool AddData( long lOID, const AOI_LINE& lList ) const;

	/*
	* @brief 関連テーブルにデータを追加
	* @param[in] lOID    選択中のフィーチャーのOID
	* @param[in] lList   追加するデータのリスト(1要素=1行)
	* @retval true  データの追加に成功
	* @retval false データの追加に失敗
	*/
	bool AddData( long lOID, const AOI_MATRIX& lList ) const;

	/*
	* @brief 関連テーブルからデータを削除
	* @param[in] lOID           選択中のフィーチャーのOID
	* @param[in] lList          削除するデータのリスト(1要素=1行)
	* @retval true  データの削除に成功
	* @retval false データの削除に失敗
	*/
	bool DeleteData( long lOID, const DELETELIST& lList ) const;

	/*
	* @brief 関連テーブルからデータを削除
	* @param[in] lOID    選択中のフィーチャーのOID
	* @param[in] lList   削除するデータのリスト(1要素=1行 1要素の中にカラム)
	* @retval true  データの削除に成功
	* @retval false データの削除に失敗
	*/
	bool DeleteData( long lOID, const DELETELISTS& lList ) const;

	/*
	* @brief 関連テーブルからキーフィールドとOIDが一致する行の一要素を取得
	* @param[in] 選択中のフィーチャーのOID
	* @param[in] 取得する要素名
	* @return retList 取得したデータのリスト
	*/
	AOI_COLUMN GetRelationData( long lOID, const CString& strField ) const;

	/*
	* @brief 関連テーブルからキーフィールドとOIDが一致する行の特定の要素を取得
	* @param[in] 選択中のフィーチャーのOID
	* @param[in] 取得する要素名を格納したリスト
	* @return retList 取得したデータのリスト(1要素=1行 1要素中の各要素が各カラムの値)
	*/
	AOI_MATRIX GetRelationData( long lOID, const FIELDNAMES& strFields ) const;

private:
	/*
	* @brief IRowBufferPtrに値を入れる
	* @param[in] ipRowBuffer 値を入れるバッファ
	* @param[in] columns     入れる値
	* @retval true  正常終了
	* @retval false いずれかの値を入れることに失敗
	*/
	bool putValues( IRowBufferPtr& ipRowBuffer, const AOI_LINE& columns ) const;

	/*
	* @brief DB上のデータを削除する際の条件式を生成
	* @param[in] lOID  "m_KeyFieldName=%ld"に使用
	* @param[in] lList 削除するデータの各カラムの値を格納したリスト(1要素=1行 1要素中の各要素がカラムの値)
	* @return  strWhereClause 削除する行の条件式
	*/
	CString makeDeleteWhereClause(long lOID, const DELETELISTS& lList) const;

	ITablePtr         m_ipTable;      //!< 関連テーブル
	sindy::CFieldMap  m_RelTableFM;   //!< 関連テーブルのフィールドマップ
	CString           m_KeyFieldName; //!< 関連テーブル内を検索する際のキーとするフィールド
	FIELDNAMES        m_subFields;    //!< データを取得・削除する際の判定に使用するフィールド
};
