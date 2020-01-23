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
#include <WinLib/ADOBase.h>

namespace sindy{
	class CTableContainer;
	class CFieldMap;
}

extern const CString LAT_FIELD;     //!< 経度フィールド
extern const CString LON_FIELD;     //!< 緯度フィールド

/// CSVフィールド情報管理用
struct FieldInfo
{
private:
	FieldInfo();
public:
	/// コンストラクタ
	FieldInfo( long csvIdx, long destIdx, const CString& field, bool opt );
	
	/// OBJECTIDフィールドか
	static bool isOidField( const CString& fieldName ){
		return (fieldName.CompareNoCase( sindy::schema::kObjectID ) == 0);
	}
	bool isOidField() const { return isOidField( fieldName ); }
	
	/// LATフィールドか
	static bool isLatField( const CString& fieldName ){
		return (fieldName.CompareNoCase( LAT_FIELD ) == 0); 
	}
	
	/// LONフィールドか
	static bool isLonField( const CString& fieldName ){
		return (fieldName.CompareNoCase( LON_FIELD ) == 0); 
	}

	/// LANまたは、LATフィールドか
	static bool isLatLonField( const CString& fieldName )	{
		return ( isLatField(fieldName) || isLonField(fieldName) );
	}
	bool isLatLonField() const { return isLatLonField( fieldName ); }


	bool operator == (const FieldInfo& fi ) const{
		return fi.fieldName == fieldName;
	}

	bool operator < (const FieldInfo& fi ) const{
		return csvIndex < fi.csvIndex;
	}

public:
	long csvIndex;      //!< CSV側インデックス
	long destIndex;     //!< テーブル側インデックス
	CString fieldName;  //!< フィールド名
	bool forceOutput;   //!< 強制出力フラグ
	bool isChanged;     //!< 変更フラグ
};

/// CSVレコード情報管理用
struct RecordInfo
{
public:
	// このクラスは、大量にコンテナに突っ込んだりそれをコピー（ではないが）したりされる。
	// 引数なし（デフォルト）コンストラクタだけを自前で用意するのであれば、コンパイラが気を利かせて
	// ムーブコンストラクタと代入演算子を用意してくれるが、良かれと思ってデストラクタやコピーコンストラクタを
	// 下手に自前で作ってしまうと、気を利かせてくれなくなり、パフォーマンスが著しく悪化する。
	// なので、そのようなコンストラクタを作らなければならない場合は、必ずムーブの面倒も見ること。
	/// コンストラクタ
	RecordInfo();

	/// オペレータ
	CComVariant& operator [](const FieldInfo& fi);

	bool setShape( const CString& fieldName, CComVariant& val );

	typedef std::pair<FieldInfo, CComVariant> fieldValue;       //!< １フィールド分の情報（key:フィールド情報 val:値）
	typedef std::list<fieldValue> fieldValues;                  //!< 全フィールド分の情報

	/// コンテナ操作用
	fieldValues::const_iterator begin() const { return m_fieldValues.cbegin();}
	fieldValues::const_iterator end() const { return m_fieldValues.cend();}
	fieldValues::iterator begin(){ return m_fieldValues.begin();}
	fieldValues::iterator end(){ return m_fieldValues.end();}

	bool isCreate;    //!< 新規作成するレコードか
	long oid;         //!< 対象のOID（新規は-1）
	bool hasShape;    //!< LAN,LOTの値が指定されているか
	WKSPoint latlon;  //!< LAN,LOT

private:
	fieldValues m_fieldValues; //!< 1レコードの全フィールド情報
};

class CsvLoader
{
public:
	/**
	* @brief csvのレコードを読み込む
	* @note  エラーが有った場合は例外を投げる
	* @param path  [in] csvファイルのフルパス
	* @param optFields [in] 強制的に出力するフィールド（","区切り）
	* @param table [in] 更新対象のテーブル 
	*/
	void Load( const CString& path, const CString& optFields, const sindy::CTableContainer& table );

	/**
	* @brief OIDで検索する
	* @note 見つからない場合は例外を投げる
	* @return 対象のレコード情報
	*/
	const RecordInfo& find_by_oid( long oid ) const;

	/**
	* @brief 更新対象のOIDをすべて取得する
	* @note 新規追加対象（OID：-1）は含まない
	* @return 更新対象のOBJECTID群
	*/
	std::vector< long > GetUpdateIDs() const;

	/**
	* @brief 新規追加対象のレコード群を取得する
	* @return 新規追加される予定のレコードたち
	*/
	typedef std::vector< RecordInfo > CsvRecords;
	std::vector< CsvRecords > GetNewRecordsList() const;
	
	/**
	* @brief 全フィールド情報を取得する
	* @return CSVのレコード情報群
	*/
	const std::vector<FieldInfo>& GetFieldInfos() const { return m_fieldInfos; }
	
	/// CSVレコード管理用（ first:行番号 second:レコード情報 ）
	typedef std::pair<long, RecordInfo> CsvRecord;


	/// コンテナ管理用
	std::vector< CsvRecord >::iterator begin() { return m_records.begin(); }
	std::vector< CsvRecord >::iterator end() { return m_records.end(); }
	std::vector< CsvRecord >::const_iterator cbegin() const { return m_records.cbegin(); }
	std::vector< CsvRecord >::const_iterator cend() const { return m_records.cend(); }
	size_t size() const { return m_records.size(); } 
	
	
private:
	/**
	* @brief csvを開き、全レコードを読み込む
	* @note エラーがあった場合は例外を投げる
	* @param path [in] csvファイルのフルパス
	*/
	void TryOpenCSV( const CString& path );
	
	/**
	* @brief CSVファイルを開き、全行をADOカーソルに取得する
	* @note エラーがあった場合は例外を投げる
	* @param path [in] csvファイルのフルパス
	* @param destField [in] 更新対象テーブルのフィールドマップ
	* @param optFields [in] 強制出力対象のフィールド名（","区切り）
	*/
	void init( const CString& path, const sindy::CFieldMap& destField, const CString& optFields );
	
	/**
	* @brief レコード情報を追加する
	* @param lineNum [in] 行番号
	* @param record [in] レコード情報
	*/
	void Add( long lineNum, const RecordInfo& record );

private:
	CADOBase m_ado;                          //!< ADO
	CADOCursor m_adoCur;                     //!< カーソル

	std::vector<FieldInfo> m_fieldInfos;     //!< フィールド情報
	std::vector< CsvRecord > m_records;      //!< レコード情報
};

