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

#include "StdAfx.h"
#include "ListHandle.h"
#include <iterator>
#include <boost/tokenizer.hpp>


// デリミタの数カウント関数
inline int countDeli( const std::string& str, char szDeli )
{
	int nCount = 0;
	const int nLen = (signed)str.length();
	for( int i=0;i<nLen;++i )
	{
		if( str[i] == szDeli )
			++nCount;
	}

	return nCount;
}

CListHandle::CListHandle(void)
{
}

CListHandle::~CListHandle(void)
{
}

// IDリスト読み込み
bool CListHandle::loadIDList(LPCTSTR lpszFile, std::vector<CIDPoint> &rVecList)
{
	std::ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if( !ifs )
		return false;

	int nIndex = 0;
	while( !ifs.eof() )
	{
		++nIndex;
		std::string	str;
		std::getline( ifs, str, '\n' );
		if( str.empty() || str.find('#') == 0 )
			continue;

		int nDCount = countDeli(str, '\t');

		CIDPoint ob;
		ob.m_lOID = atol( str.substr(0, str.find("\t\n")).c_str() );

		if( nDCount > 0 && !setMapVal(str.substr(str.find('\t')+1), ob.m_mapVal) )
			continue;

		rVecList.push_back( ob );
	}

	return true;

}

// MXYリスト読み込み
bool CListHandle::loadMXYList(LPCTSTR lpszFile, std::vector<CMXYPoint> &rVecList)
{
	std::ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if( !ifs )
		return false;

	boost::char_separator<char> sep("\t\n", NULL, boost::keep_empty_tokens);	/// デリミタ
	int nIndex = 0;
	while( !ifs.eof() )
	{
		++nIndex;
		std::string str;
		std::getline( ifs, str, '\n' );
		if( str.empty() || str.find('#') == 0 )
			continue;

		int nDCount = countDeli(str, '\t');
		if( nDCount < 3 ){
			std::cout << "#ERROR フォーマットが正しくない " << nIndex << " 行目" << std::endl;
			continue;
		}

		boost::tokenizer< boost::char_separator<char> > tok(str, sep);
		boost::tokenizer< boost::char_separator<char> >::iterator itr = tok.begin();

		CMXYPoint ob;
		ob.m_nMeshCode = atoi( (*itr).c_str() );	++itr;
		ob.m_nMeshX = atoi( (*itr).c_str() );	++itr;
		ob.m_nMeshY = atoi( (*itr).c_str() );

		// 属性値定義のとこだけ取得
		std::string strTmp = str;
		for( int i=0;i<3;i++ )
			strTmp.replace( 0, strTmp.find('\t')+1, "" );

		if( setMapVal(strTmp, ob.m_mapVal) )
			rVecList.push_back( ob );
	}

	return true;
}

/////////////////////////////////////////
//	内部関数
/////////////////////////////////////////

// 属性地セット
bool CListHandle::setMapVal( const std::string& strData, std::map<CString, CString>& rMapVal )
{
	if( strData.empty() )
		return true;

	boost::char_separator<char> sep("\t\n", NULL, boost::keep_empty_tokens);	/// 属性値解析用デリミタ

	boost::tokenizer< boost::char_separator<char> > tok(strData, sep);
	boost::tokenizer< boost::char_separator<char> >::iterator itr = tok.begin();

	int nDCount = countDeli( strData, '\t' );
	for( int i=0;i<=nDCount;++i,++itr )
	{
		rMapVal.insert( std::pair<CString, CString>((*itr).substr(0, (*itr).find(':')).c_str(), (*itr).substr((*itr).find(':')+1).c_str()) );
	}

	return true;
}
