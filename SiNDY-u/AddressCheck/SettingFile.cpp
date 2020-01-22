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
#include "SettingFile.h"


// 設定ファイル読み込み
bool SettingFile::load(const CString& path, const std::vector<CString>& layer)
{
	boost::filesystem::path filePath = boost::filesystem::path( path );
	if(!boost::filesystem::exists( filePath )) return false;
	m_settingFilePath = path;

	for( const auto& layerName: layer )
	{
		CString logPath;

		if( !GetPrivateProfileString( layerName, LOG_NAME, _T(""), logPath.GetBuffer(MAX_PATH), MAX_PATH, path ) )
		{
			SYSTEMTIME st;
			::GetLocalTime( &st );
			logPath.Format( DEFAULT_LOGNAME, layerName, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
		}
				
		m_layerLogName[layerName] = logPath;
	}
	return true;

}

// ログファイル名の取得
const CString &SettingFile::getLayerLogName( const CString& layer ) const
{
	return m_layerLogName.find(layer)->second;
}

// 設定アイテムの取得
CString SettingFile::getLayerSettingItem(const CString& layer, const CString& itemName)const
{
	CString itemString;

	boost::filesystem::path filePath = boost::filesystem::path( m_settingFilePath );
	if(!boost::filesystem::exists( filePath )) return itemString;

	GetPrivateProfileString( layer, itemName, _T(""), itemString.GetBuffer(MAX_PATH), MAX_PATH, m_settingFilePath );
	
	return itemString;
}