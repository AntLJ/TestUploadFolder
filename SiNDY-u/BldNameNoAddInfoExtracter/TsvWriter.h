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

#include <fstream>

struct AddrInfo;
namespace sindy{
	class CRowContainer;
	typedef boost::shared_ptr<CRowContainer> CSPRowContainer;
}

/// 列情報
namespace cols{
	enum index{
		SourceType,   //!< BUILDINGNAME_SRC_POINT.SOURCETYPE_C
		SourceName,   //!< BUILDINGNAME_SRC_POINT.SOURCENAME
		SourceId,     //!< BUILDINGNAME_SRC_POINT.SOURCE_ID
		SrcOid,       //!< BUILDINGNAME_POINT.SRC_OID
		Oid,          //!< BUILDINGNAME_POINT.OBJECTID
		Name,         //!< BUILDINGNAME_POINT.NAME
		Yomi,         //!< BUILDINGNAME_POINT.YOMI
		AddrCode,     //!< 住所コード
		Pref,         //!< 都道府県
		PrefYomi,     //!< 都道府県（ヨミ）
		City,         //!< 市区町村
		CityYomi,     //!< 市区町村（ヨミ）
		Oaza,         //!< 大字
		OazaYomi,     //!< 大字（ヨミ）
		Aza,          //!< 字
		AzaYomi,      //!< 字（ヨミ）

		num          //!< 列数
	};
};


class TsvWriter
{
public:
	TsvWriter(void){}
	virtual ~TsvWriter(void){}

	/**
	* @brief 初期化
	* @param path [in] 出力先ファイルのフルパス
	*/
	void init( const CString& path );
	
	/**
	* @brief 出力対象レコードを追加する
	* @note 内部でバッファし、flush()するまで実際の書き込みは行わない
	* @param bldName [in] 建物ビル名称
	* @param srcP [in] 素材ポイント
	* @param addrInfo [in] 住所関連情報
	* @param yomi [in] ヨミ
	*/
	void addRecord( const sindy::CRowContainer& bldName,
		            const sindy::CSPRowContainer& srcP,
					const AddrInfo& addrInfo,
					const CString& yomi );
	
	/**
	* @brief ファイル書き込み
	* @note addRecord()で貯めこんだレコードを実際にファイルに出力する
	*/
	void flush();

private:
	
	std::ofstream m_ofs;		                      //!< 出力ファイルストリーム
	typedef std::vector<CString> record;              //!< 1行分の出力データ
	typedef std::pair<CString, record> orderedRecord; //!< 住所コード順にするためのレコード情報
	std::set<orderedRecord> m_records;                //!< 出力行バッファ用
};

