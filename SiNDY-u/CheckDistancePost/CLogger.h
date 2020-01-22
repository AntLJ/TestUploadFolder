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
#include "Output.h"

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
	public Output
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
	virtual ~CLogger()
	{
		EndLog();
	}

	/**
	 * @brief ログ初期設定
	 * @param lpszLogFile [in] ログファイル名
	 * @param nRoadCode [in] 路線番号
	 * @param nDirCode [in] 方向コード
	 * @param nDirCode [in] 距離標記号
	 * @param lpcszOpParam [in] パラメータ
	 * @param bSetHeader [in] ヘッダをセットするかどうか
	*/
	void initLog(LPCTSTR lpszLogFile, long nRoadCode, long nDirCode, LPCTSTR lpcszMark, LPCTSTR lpcszOpParam, bool bSetHeader);

	/**
	 * @brief ログ書き込み
	 * @param nOID [in] エラーオブジェクト
	 * @param dbVal [in] チェック先オブジェクト
	 * @param nCode [in] 
	 * @param lpcszMsg [in] 
	*/
	void AddLog(sindy::CSPRowBase pRow, long nCode, LPCTSTR lpcszMsg);

	/**
	 * @brief ログ書き込み
	 * @param nOID [in] エラーオブジェクト
	 * @param dbVal [in] チェック先オブジェクト
	 * @param nCode [in] 
	 * @param lpcszMsg [in] 
	*/
	void AddLog(long nOID, double dbVal, LPCTSTR lpcszMark, long nCode, LPCTSTR lpcszMsg);

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
