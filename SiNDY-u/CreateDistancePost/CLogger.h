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
#include "COutput.h"

namespace sindy
{

/**
 * @class CLogger
 * @brief ログ管理クラス
 * @file  CLogger.h
 * @author ビットエイジ　秦
 * $Id$
*/
class CLogger :
	public COutput
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CLogger()
	{
		m_stream = nullptr;
	}

	/**
	 * @brief デストラクタ
	 */
	~CLogger()
	{
		EndLog();
	}

	/**
	 * @brief ログ初期設定
	 * @param lpszLogFile [in] ログファイル名
	 * @param pair [in] 路線番号、方向コード、距離標記号
	 * @param lpcszOpParam [in] パラメータ
	*/
	void initLog(LPCTSTR lpszLogFile, boost::tuple<long,long,CString> pair, LPCTSTR lpcszOpParam);

	/**
	 * @brief ログ書き込み
	 * @param nOID [in] オブジェクトID
	 * @param lpcszMark [in] 距離標記号
	 * @param dbVal [in] 距離標値
	*/
	void Log(long nOID, LPCTSTR lpcszMark, double dbVal);

	/**
	 * @brief ログ終了処理
	*/
	void EndLog();

private:
	FILE *m_stream;                         ///< ログファイルポインタ
	CString m_strLogFile;                   ///< ログファイル名
};

typedef boost::shared_ptr<CLogger> CSPLogger;

}
