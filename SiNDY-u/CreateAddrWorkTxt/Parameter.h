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

#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include <SiNDYLib/FeatureClass.h>

using namespace sindy;

/**
 * @struct	CParameter
 * @brief	実行情報
 */
struct CParameter
{
	CFeatureClass	m_cPointFC;				//!< 住所ポイントフィーチャクラス
	CFeatureClass	m_cCityAdminFC;			//!< 都市地図行政界フィーチャクラス

	CString			m_strOutFile;			//!< 出力ファイル(CSで使用)
	CString			m_strOutDir;			//!< 出力ディレクトリ(KP,GPで使用)

	std::pair<CString, CString> m_pairArea;	//!< 実行エリア情報(オプション、情報)

	CString			m_strJDBProp;			//!< 住所DB接続プロパティ

	CParameter()
	{
		m_pairArea = std::make_pair<CString, CString>( _T("-ca"), NULL );	// デフォルトは全国
	}

};

#endif __PARAMETER_H__
