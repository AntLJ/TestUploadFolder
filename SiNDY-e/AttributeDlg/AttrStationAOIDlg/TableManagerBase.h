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

#include <FieldMap.h>
#include "CommonData.h"

/**
* @brief あるテーブルとあるテーブルを関連付け、必要な情報を取得するためのクラス
* @warning  同じテーブルを二回見るような状況に対応できない
* @warning  :ex. あるSTATION_POIのGUIDEID→STATION_GUIDE_POINTのOBJECTID→STATION_GUIDE_POINTのOIDを持つ全てのSTATION_POINTのOID
* @warning  :この場合、インスタンスを二つ作成し、それぞれにSTATION_POI_→STATION_GUIDE_POINT、STATION_GUIDE_POINT→STATION_POINTを関連付ける
* @warning  実装上、○万件ごと、のような読み込みができない。そのようなものが欲しい場合は一から作り直すこと。
*/
class CTableManagerBase
{
	/**
	* @brief 読み取ったデータを多分木状に格納するためのクラス
	* @note  処理上、Headerとなるnodeのm_attrListは常に空であるはず
	*/
	class CSearchList
	{
	public:
		std::list<class CSearchList> m_childList; //!< 子となるノード
		AOI_LINE m_attrList;            //!< このノードがもつ属性
		/// コンストラクタ
		CSearchList(){}
		/// デストラクタ
		virtual ~CSearchList(){};
		/// コピーコンストラクタ
		CSearchList(const CSearchList& c)
		{
			m_childList = c.m_childList;
			m_attrList = c.m_attrList;
		}
		/**
		 * @brief 代入演算子
		 * @param[in] c 代入元CSearchList
		 * @return CSearchList
		 */
		CSearchList operator=(const CSearchList& c)
		{
			CSearchList temp;
			temp.m_childList = c.m_childList;
			temp.m_attrList = c.m_attrList;
			return temp;
		}
	};

public:

	/**
	* @brief このクラスで開かれるテーブルは、ここでセットしたテーブルのオーナーと同じになる
	* @warning 必ず最初に一度呼び出す
	*/
	void setTable(ITablePtr& ipTale)
	{
		m_ipTable = ipTale;
	}

	/**
	* @brief ニテーブル間での連結に必要な情報を設定する
	* @warning Main->Rel1->Rel2->Rel3->Mainのように、無限ループを作ってしまえるので注意
	* @warning Main->Rel1, Main->Rel2 のように、あるテーブルから二つ以上のテーブルに関連を持たせることはできない
	* @param[in] strMain    主体となるテーブル名
	* @param[in] strSub     主体に関連づくテーブル名
	* @param[in] strMainKey 主体でキーとなるフィールド名
	* @param[in] strSubKey  主体から関連が検索される際にキーとなるフィールド名
	*/
	void Join(const CString& strMain, const CString& strSub, const CString& strMainKey, const CString& strSubKey);

	/**
	* @brief 値を取得
	* @param[in] key       フィルタに与える値(フィルタに与える検索条件の"OBJECTID={ここ}")
	* @param[in] fieldName フィルタで対象とするフィールド名(フィルタに与える検索条件の"{ここ}=1")
	* @param[in] strTable  検索の開始点とするテーブル名
	* return retList 取得したデータのリスト
	*/
	AOI_MATRIX GetValue( const CString& key, const CString& filedName, const CString& strTable );

	/**
	* @brief DBから取得する値を設定
	* @param[in] dataList  取得する値のリスト(各要素が[テーブル名,値を取得するフィールド名]となっている)
	*/
	void SetReturnData( const std::list<std::pair<CString,CString>>& dataList );

private:

	/**
	* @brief 値を取得
	* @param[in] ipCursor strTableを検索した結果を保持したカーソル
	* @param[in] strTable 検索の対象となるテーブル名
	* return retList 取得したデータのリスト
	*/
	AOI_MATRIX getValue( _ICursorPtr& ipCursor, const CString& strTable );

	/**
	* @brief DBから値を読み出し、メモリ上に保持する処理(再帰)
	* @param[in]     ipCursor strTableを検索した結果を保持したカーソル
	* @param[in]     strTable 検索の対象となるテーブル名
	* @param[in,out] parent   読み込んだデータを格納する多分木
	*/
	void loadValue( _ICursorPtr& ipCursor, const CString& strTable, CSearchList& parent );

	/**
	*  @brief _IRowPtrから値を取得する
	*  @note  CSearchListに実際に値を入れているのはここ
	*  @param[in]     ipRow    対象の_IRowPtr
	*  @param[in]     strTable ipRowの対象であるテーブル名
	*  @param[in,out] addList  取得した値を格納するリスト
	*/
	void getRowValue( _IRowPtr& ipRow, const CString& strTable, AOI_LINE& addList );

	/**
	*  @brief strTableを検索する際のサブフィールドを生成
	*  @param[in,out] strSubField  サブフィールド文字列を格納する文字列
	*  @param[in]     strTable     サブフィールドを使用するテーブル名
	*  @param[in,out] keyField     strTableを検索する際のキーとなるフィールド名
	*/
	void setStrSubFields( CString& strSubField , const CString& strTable, const CString& keyField = CString() );

	/**
	*  @brief SearchListをAOI_MATRIXの形式に変換(再帰)
	*  @param[in]     ops          変換する元となる
	*  @param[in,out] lList        変換後の結果を格納するリスト
	*/
	void searchList2ListList( CSearchList& ops, AOI_MATRIX& lList ) const ;

	/**
	* @brief フィールドマップを作成
	* @param[in] フィールドマップを作成するテーブルの名称
	* @param[in,out] フィールドマップの作成先
	* @retval true  成功
	* @retval false 失敗
	*/
	bool createFieldMap( const CString& strTable, sindy::CFieldMap& fieldMap );

private:
	/**
	* @brief あるテーブルに対して、次に検索するテーブルと、次のテーブルを検索するのに必要なフィールド名を格納
	* @note  [あるテーブル名,[次に検索するテーブル名,次に検索するテーブルを検索するのに必要な、あるテーブルのフィールド名]]
	*/
	typedef std::map<CString,std::pair<CString,CString>> JOIN_MAP;


	JOIN_MAP m_joinMap;                               //!< あるテーブルに対して次の検索テーブルとなるものと、検索に必要なフィールド名を格納

	std::map<CString,sindy::CFieldMap> m_FieldMaps;   //!< 各テーブルのフィールドマップ
	std::map<CString,FIELDNAMES> m_subFields;         //!< テーブル名と読み込むフィールドの対応

	std::map<CString,CString> m_tableSearchedKey;     //!< 検索される側になったときに、キーとなるフィールドを格納

	/**
	* @brief 最終的に取得するデータの対応を格納
	* @note  各要素には[テーブル名,フィールド名]が入っている想定
	*/
	std::list<std::pair<CString,CString>> m_dataList;

	ITablePtr m_ipTable; //!< これと同じユーザーのテーブルを開くことになる
protected:

};
