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
#include "TextAddressMaster.h"
#include <sstream>

// テキスト住所読み込み
bool TextAddressMaster::load(const CString& path)
{
	CString searchPath = path + _T("\\*.*");
	WIN32_FIND_DATA findFileData;

	HANDLE hFind=::FindFirstFile(searchPath,&findFileData);
	if(INVALID_HANDLE_VALUE == hFind) return false;
	while(true)
	{
		if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(findFileData.cFileName[0]!='.')
			{
				CString filePath = path + _T("\\") + findFileData.cFileName;
				findFile(filePath, findFileData.cFileName);
			}
		}
		else
			return false;

		if(!FindNextFile(hFind,&findFileData))break;
	}
	FindClose(hFind);
	setDirPath( path );
	return true;
}

// テキスト住所ファイル探す
bool TextAddressMaster::findFile( const CString& filePath, const CString& folder)
{
	CString searchPath = filePath + _T("\\*.*");
	WIN32_FIND_DATA findFileData;

	HANDLE hFind=::FindFirstFile(searchPath,&findFileData);
	if(INVALID_HANDLE_VALUE == hFind) return false;
	while(true)
	{
		CString fileName(findFileData.cFileName);
		if(!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && fileName.Left(2) == folder )
		{
			CString path = filePath + _T("\\") + findFileData.cFileName;
			readData( path, folder );
		}

		if(!FindNextFile(hFind,&findFileData))break;
	}
	FindClose(hFind);

	return true;
}

// テキスト住所ファイル読み込み
bool TextAddressMaster::readData( const CString& filePath, const CString& folder)
{
	ifstream ifs( static_cast<CT2CW>(filePath) );
	if( !ifs ) return false;
	while( !ifs.eof() )
	{
		char	aBuf[1024]; 
		ifs.getline(aBuf,1024);
		if(strlen(aBuf) > 0)	 
		{
			CString str(aBuf);
			vector<CString> valueList =  uh::str_util::split(str, _T(","));
			if( valueList.size() != 3 ) return false;

			AddressMaster addressMaster(_ttol(valueList[1]), valueList[2], _T(""));
			m_textAddressMaster[valueList[0]] = addressMaster;

			AddressMaster addressMasterCode(_ttol(valueList[1]), _T(""), valueList[0]);
			m_textAddressMasterCode[valueList[2]] = addressMasterCode;

			CString cityCodeStr = valueList[0].Mid( 0, 5 );
			auto& itr = m_textAddressMasterCityMap.find( cityCodeStr );
			if( itr == m_textAddressMasterCityMap.end()	)
			{
				set<long> status;
				status.insert(_ttol(valueList[1]));
				m_textAddressMasterCityMap[cityCodeStr] = status;
			}else
				itr->second.insert(_ttol(valueList[1]));
		}
	}

	return true;
}

// 住所マスタの検索
AddressMaster TextAddressMaster::getTextAddressMaster( const CString& key ) 
{
	AddressMaster addressMaster;
	if ( !m_textAddressMaster.count(key) )
	{
		CString prefCode = key.Mid(0, 2);
		if( m_readedFolderSet.find(prefCode) != m_readedFolderSet.end() ) return addressMaster;

		findFile( m_textAddressMasterPath + _T("\\") + prefCode, prefCode );

		m_readedFolderSet.insert( prefCode );
	}

	const auto textAddressMasterItr = m_textAddressMaster.find(key);
	return ( textAddressMasterItr != m_textAddressMaster.end() )? textAddressMasterItr->second : addressMaster;
}

// 住所マスタコードの検索
const AddressMaster TextAddressMaster::getTextAddressMasterCode(  const CString& key ) const
{
	AddressMaster addressMasterCode( INVALID_VALUE, _T(""), _T("") );
	const auto addrMasterCodeItr = m_textAddressMasterCode.find(key);
	return ( addrMasterCodeItr != m_textAddressMasterCode.end() )? addrMasterCodeItr->second : addressMasterCode;
}

// 市区町村コードとステータスによると住所マスタの検索
bool TextAddressMaster::searchMaster(const CString &cityCode, long status)
{
	const auto& itr = m_textAddressMasterCityMap.find(cityCode);
	if( itr == m_textAddressMasterCityMap.end() )	return false;

	return (itr->second.find(status)==itr->second.end())?false:true;
}

// 市区町村コードによると住所マスタの検索
bool TextAddressMaster::searchMaster(const CString &cityCode)
{
	return (m_textAddressMasterCityMap.find(cityCode) == m_textAddressMasterCityMap.end())?false:true;
}
