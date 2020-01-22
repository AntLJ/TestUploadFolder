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

#include "stdafx.h"
#include "TsvWriter.h"
#include "AddrMgr.h"
#include <TDC/sindylib_base/RowContainer.h>

using namespace sindy;
using namespace schema;
using namespace sj;

namespace init_func{
	void writeHeader( std::ofstream& ofs )
	{
		using namespace sindy::schema::sj;
		std::vector<CString> header = boost::assign::list_of
			( _T("出典コード") )
			( _T("出典元") )
			( _T("出典資料ID") )
			( _T("素材OID") )
			( _T("OID") )
			( _T("名称") )
			( _T("ヨミ") )
			( _T("住所コード") )
			( _T("都道府県") )
			( _T("都道府県（ヨミ）") )
			( _T("市区町村") )
			( _T("市区町村（ヨミ）") )
			( _T("大字") )
			( _T("大字（ヨミ）") )
			( _T("字") )
			( _T("字（ヨミ）") );

		assert( cols::index::num == header.size() );

		ofs << CT2A(uh::str_util::join( header, _T("\t") )) << std::endl;
	}
}

void TsvWriter::init( const CString& path )
{
	using namespace init_func;

	if( ! m_ofs || m_ofs.is_open() )
		return;

	m_ofs.open( path );
	if( ! m_ofs )
		throw std::runtime_error( uh::str_util::formatA(_T("Log file open failure. : %s" ), path) );

	writeHeader( m_ofs );
}

void TsvWriter::addRecord( const CRowContainer& bldName,
		                   const CSPRowContainer& srcP,
				           const AddrInfo& addrInfo,
				           const CString& yomi )
{
	using namespace uh::str_util;
	// フィールドと出力列の対応
	static const std::map<CString, cols::index> bldNameIdx = boost::assign::map_list_of
		( buildingname_point::kSourceType, cols::index::SourceType )
		( buildingname_point::kObjectID, cols::index::Oid )
		( buildingname_point::kSrcOID, cols::index::SrcOid )
		( buildingname_point::kName, cols::index::Name );
	
	static const std::map<CString, cols::index> srcPIdx = boost::assign::map_list_of
		( buildingname_src_point::kSourceName, cols::index::SourceName )
		( buildingname_src_point::kSourceID, cols::index::SourceId );

	// 列数分の領域を予め確保
	record line( cols::num );

	// BUILDNAME関連
	for( const auto& field : bldNameIdx )
	{
		line[ field.second ] = ToString( bldName.GetValue( field.first ) );
	}
	// 素材ポイントはない場合もある
	if( srcP )
	{
		for( const auto& field : srcPIdx )
		{
			line[ field.second ] = ToString( srcP->GetValue( field.first ) );
		}
	}

	// 住所関連
	line[ cols::index::AddrCode ] = addrInfo.addr_code;
	line[ cols::index::Pref ] = addrInfo.ken_kanji;
	line[ cols::index::PrefYomi ] = addrInfo.ken_kana;
	line[ cols::index::City ] = addrInfo.shi_kanji;
	line[ cols::index::CityYomi ] = addrInfo.shi_kana;
	line[ cols::index::Oaza ] = addrInfo.oaza_kanji;
	line[ cols::index::OazaYomi ] = addrInfo.oaza_kana;
	line[ cols::index::Aza ] = addrInfo.aza_kanji;
	line[ cols::index::AzaYomi ] = addrInfo.aza_kana;

	// ヨミ
	line[ cols::index::Yomi ] = yomi;

	// 住所コードでソートしつつ確保
	m_records.insert( std::make_pair( addrInfo.addr_code, line ) );
}

void TsvWriter::flush()
{
	// タブ区切りにして書き出し
	for( const auto& rec : m_records )
	{
		m_ofs << CT2CA(uh::str_util::join( rec.second, _T("\t") )) << std::endl;
	}
	m_records.clear();
}
