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

#include <boost/program_options.hpp>

typedef std::pair<long, long> POIMASTERDBINFO;

/**
 * @class	CMasterDBCache
 * @brief	DB内マスター系キャッシュクラス
 */
class CMasterDBCache
{
public:
	CMasterDBCache(void);
	~CMasterDBCache(void);

	/**
	 * @brief	キャッシュ作成
	 * @param	ipWorkspace	[in]	ワークスペース
	 * @return	bool
	 */
	bool Create( IWorkspacePtr ipWorkspace );

	/**
	 * @brief	データ分類・店舗コード情報取得
	 * @param	lContentsCode	[in]	コンテンツコード
	 * @return	データ分類・店舗コード情報
	 */
	const POIMASTERDBINFO* GetPOIMasterInfo( long lContentsCode )
	{
		std::map<long, POIMASTERDBINFO>::const_iterator it = m_mapInfo.find( lContentsCode );
		return it != m_mapInfo.end()? &(it->second) : NULL;
	}

	/**
	 * @brief	コンテンツコードリストから、WHERE句作成（Bug9215対応）
	 * @return	WHERE句のvector
	 */
	const std::vector<std::wstring> CreateWhereVecFromContentCode( const std::wstring& wsFieldName )
	{
		std::vector<std::wstring>	vecWhere;
		std::wostringstream	woStreamWhere;
		
		// カウント値
		int	iCnt = 1;
		for( std::set<long>::const_iterator itr = m_setContentsCode.begin() ; itr != m_setContentsCode.end() ; ++itr )
		{
			if( iCnt == 1 )
				woStreamWhere << wsFieldName << _T(" IN (") << *itr;
			else
				woStreamWhere << _T(",") << *itr;

			if( *itr == *(m_setContentsCode.rbegin()) || iCnt == 200 )
			{
				woStreamWhere << _T(")");
				vecWhere.push_back( woStreamWhere.str() );
				woStreamWhere.str(_T(""));
				iCnt = 0;
			}
			iCnt++;

		}
		return vecWhere;
	}

	CString	m_strContentsMaster;	//!< コンテンツマスタ取得用文字列 cApp::Init()で引数とってきて初期化
private:

	std::map<long, POIMASTERDBINFO>	m_mapInfo;	//!< キー : コンテンツコード 値 : (分類コード、店舗コード)
	std::set<long>					m_setContentsCode;

};
