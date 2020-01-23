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
#include "IllustSettings.h"
#include "LogMgr.h"
#include "common.h"
#include <sindy/schema/illust.h>

bool IllustSettingsRec::setNamingRule( const std::string& strNamingRule )
{
	std::vector<std::string> values;
	boost::split( values, strNamingRule, boost::is_any_of(",") );
	for( auto str : values )
	{
		m_vecNamingRule.push_back( CString( str.c_str() ) );
	}

	return true;
}

IllustSettings::IllustSettings()
{
}

IllustSettings::~IllustSettings()
{
}

bool IllustSettings::load( const CString& strFileName, const ITablePtr& ipIllustLinkTable )
{
	// ファイルをオープン
	std::ifstream ifs( strFileName );
	if( !ifs.is_open() ) return false;

	while( !ifs.eof() )
	{
		// 行を読み込み
		std::string strLine;
		std::getline( ifs, strLine );

		// 空行やコメント行はスキップ
		if( strLine.empty() ) continue;
		if( strLine.front() == '#' ) continue;

		// タブでカラム（トークン）に分解
		std::vector<std::string> vecValues;
		boost::split( vecValues, strLine, boost::is_any_of("\t") );

		if( vecValues.size() != 7 )
		{
			LogMgr::getInstance().writeRunLog( _T("処理情報設定ファイルのフィールド数が適切でありません") );
			return false;
		}

		IllustSettingsRec rec;
		rec.m_nIllustClass	= std::atol( vecValues[0].c_str() );	// イラスト種別コード
		rec.m_nCustomer		= std::atol( vecValues[1].c_str() );	// 仕向先コード
		rec.m_strPrefix		= CA2T( vecValues[2].c_str() );			// イラストファイルのプレフィックス
		rec.m_strExtension	= CA2T( vecValues[3].c_str() );			// イラストファイルの拡張子
		rec.setNamingRule( vecValues[4] );							// イラストファイルの命名規則
		rec.m_strIllustDir	= CA2T( vecValues[5].c_str() );			// イラストファイルの格納先ディレクトリ
		if( !m_cIllustOutputInfo.readExceptionConds( CString( vecValues[6].c_str() ), rec.m_cOutputInfo.mExceptionConds ) )
		{
			LogMgr::getInstance().writeRunLog( _T("出力除外条件が適切でありません") );
			return false;
		}

		// mapに挿入
		IllustSettings::_Pairib ret = insert( IllustSettings::value_type( IllustSettings::key_type( rec.m_nIllustClass, rec.m_nCustomer ), rec ) );

		// 挿入に失敗（キーの重複）
		if( !ret.second )
		{
			CString strTmp;
			strTmp.Format( _T("処理情報設定ファイルのキーが重複しています : %d, %d"), rec.m_nIllustClass, rec.m_nCustomer );
			LogMgr::getInstance().writeRunLog( strTmp );
			return false;
		}
	}

	ifs.close();

	if( !checkCodesRange( ipIllustLinkTable ) ) return false;
	if( !checkDirExistence() ) return false;

	return true;
}

bool IllustSettings::checkCodesRange( const ITablePtr& ipIllustLinkTable )
{
	// コード値ドメインの取得
	std::set< long, std::less< long > > setValuesIllustClass, setValuesCustomerC;
	if( !common_proc::getCodedValues( ipIllustLinkTable, sindy::schema::illust_link::kIllustClass, setValuesIllustClass ) )
	{
		LogMgr::getInstance().writeRunLog( _T("コード値ドメインを取得することができません : ") + CString( sindy::schema::illust_link::kIllustClass ) );
		return false;
	}
	if( !common_proc::getCodedValues( ipIllustLinkTable, sindy::schema::illust_link::kCustomerCode, setValuesCustomerC ) )
	{
		LogMgr::getInstance().writeRunLog( _T("コード値ドメインを取得することができません : ") + CString( sindy::schema::illust_link::kCustomerCode ) );
		return false;
	}

	// 各レコードをチェック
	bool bRet = true;
	for( const auto rec : *this )
	{
		// イラスト種別コードがコード値ドメインに含まれない
		if( setValuesIllustClass.find( rec.second.m_nIllustClass ) == setValuesIllustClass.end() )
		{
			CString strTmp;
			strTmp.Format( _T("コード値ドメインに含まれないイラスト種別コードです : %d"), rec.second.m_nIllustClass );
			LogMgr::getInstance().writeRunLog( strTmp );
			bRet = false;
		}
		// 仕向先コードがコード値ドメインに含まれない
		if( setValuesCustomerC.find( rec.second.m_nCustomer ) == setValuesCustomerC.end() )
		{
			CString strTmp;
			strTmp.Format( _T("コード値ドメインに含まれない仕向先コードです : %d"), rec.second.m_nCustomer );
			LogMgr::getInstance().writeRunLog( strTmp );
			bRet = false;
		}
	}

	return bRet;
}

bool IllustSettings::checkDirExistence()
{
	bool bRet = true;
	for( const auto rec : *this )
	{
		if( !::PathIsDirectory( rec.second.m_strIllustDir ) )
		{
			LogMgr::getInstance().writeRunLog( _T("指定されたイラストファイル格納ディレクトリが存在しません : ") + rec.second.m_strIllustDir );
			bRet = false;
		}
	}

	return bRet;
}
