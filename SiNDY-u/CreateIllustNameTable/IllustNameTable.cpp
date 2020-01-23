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
#include "IllustNameTable.h"
#include "LogMgr.h"

IllustNameTable::IllustNameTable()
{
}

IllustNameTable::~IllustNameTable()
{
	if( m_ofsIllustNameTable.is_open() ) m_ofsIllustNameTable.close();
}

bool IllustNameTable::open( const CString& strFileName, const boost::program_options::variables_map& varMap )
{
	m_ofsIllustNameTable.open( strFileName );
	if( !m_ofsIllustNameTable.is_open() ) return false;

	outputHeader( varMap );

	return true;
}

void IllustNameTable::write()
{
	IllustNameConverter cConverter( *this );
	for( const auto rec : *this )
	{
		CString strOut;
		strOut.Format( _T("%d\t%d\t%ld\t%s\t%s"), rec.m_nIllustClass, rec.m_nCustomer, rec.m_lOID, rec.m_strFileName, cConverter.getReleaseFileName( rec ) );
		m_ofsIllustNameTable << CT2A( strOut ) << std::endl;
	}
}

void IllustNameTable::outputHeader( const boost::program_options::variables_map& varMap )
{
	// バージョン情報出力
	m_ofsIllustNameTable << "# " << CT2A( LogMgr::getVersionInfo() ) << std::endl;

	// 実行オプション情報出力
	for( const auto rec : varMap )
		m_ofsIllustNameTable << "# --" << std::setw(20) << std::left << rec.first << rec.second.as<std::string>() << std::endl;
}

IllustNameConverter::IllustNameConverter()
{
}

IllustNameConverter::IllustNameConverter( const IllustNameTable& illustNameTable )
{
	int nSequence = 0;
	for( const auto rec : illustNameTable )
	{
		IllustNameConverterRec convRec( rec );
		// まずはレコードがすでに存在しているか確認
		const_iterator itr = find( convRec );
		// 存在しない場合はシーケンス番号をインクリメントしてレコードを追加
		if( itr == end() ) insert( IllustNameConverter::value_type( convRec, ++nSequence ) );
	}
}

IllustNameConverter::~IllustNameConverter()
{
}

CString IllustNameConverter::getReleaseFileName( const IllustNameRec& illustNameRec )
{
	CString strRet;
	strRet.Format( _T("%06d"), (*this)[ IllustNameConverterRec( illustNameRec ) ] );
	return strRet;
}
