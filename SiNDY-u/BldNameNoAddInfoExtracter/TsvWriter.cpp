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
			( _T("�o�T�R�[�h") )
			( _T("�o�T��") )
			( _T("�o�T����ID") )
			( _T("�f��OID") )
			( _T("OID") )
			( _T("����") )
			( _T("���~") )
			( _T("�Z���R�[�h") )
			( _T("�s���{��") )
			( _T("�s���{���i���~�j") )
			( _T("�s�撬��") )
			( _T("�s�撬���i���~�j") )
			( _T("�厚") )
			( _T("�厚�i���~�j") )
			( _T("��") )
			( _T("���i���~�j") );

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
	// �t�B�[���h�Əo�͗�̑Ή�
	static const std::map<CString, cols::index> bldNameIdx = boost::assign::map_list_of
		( buildingname_point::kSourceType, cols::index::SourceType )
		( buildingname_point::kObjectID, cols::index::Oid )
		( buildingname_point::kSrcOID, cols::index::SrcOid )
		( buildingname_point::kName, cols::index::Name );
	
	static const std::map<CString, cols::index> srcPIdx = boost::assign::map_list_of
		( buildingname_src_point::kSourceName, cols::index::SourceName )
		( buildingname_src_point::kSourceID, cols::index::SourceId );

	// �񐔕��̗̈��\�ߊm��
	record line( cols::num );

	// BUILDNAME�֘A
	for( const auto& field : bldNameIdx )
	{
		line[ field.second ] = ToString( bldName.GetValue( field.first ) );
	}
	// �f�ރ|�C���g�͂Ȃ��ꍇ������
	if( srcP )
	{
		for( const auto& field : srcPIdx )
		{
			line[ field.second ] = ToString( srcP->GetValue( field.first ) );
		}
	}

	// �Z���֘A
	line[ cols::index::AddrCode ] = addrInfo.addr_code;
	line[ cols::index::Pref ] = addrInfo.ken_kanji;
	line[ cols::index::PrefYomi ] = addrInfo.ken_kana;
	line[ cols::index::City ] = addrInfo.shi_kanji;
	line[ cols::index::CityYomi ] = addrInfo.shi_kana;
	line[ cols::index::Oaza ] = addrInfo.oaza_kanji;
	line[ cols::index::OazaYomi ] = addrInfo.oaza_kana;
	line[ cols::index::Aza ] = addrInfo.aza_kanji;
	line[ cols::index::AzaYomi ] = addrInfo.aza_kana;

	// ���~
	line[ cols::index::Yomi ] = yomi;

	// �Z���R�[�h�Ń\�[�g���m��
	m_records.insert( std::make_pair( addrInfo.addr_code, line ) );
}

void TsvWriter::flush()
{
	// �^�u��؂�ɂ��ď����o��
	for( const auto& rec : m_records )
	{
		m_ofs << CT2CA(uh::str_util::join( rec.second, _T("\t") )) << std::endl;
	}
	m_records.clear();
}
