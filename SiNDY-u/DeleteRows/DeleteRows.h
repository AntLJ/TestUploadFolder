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
#include "stdafx.h"
#include "Arguments.h"

#define FORTIME	0
#define FORVER	1

/**
 * @brief	現在時刻を文字列で取得
 * @return	文字列(yyyy/mm/dd hh/mm/ss)
 */
inline CString	GetCurDateStr( int mode = FORTIME )
{
	SYSTEMTIME	st;
	::GetLocalTime( &st );

	CString strTime;

	// 時間用にはyyyy/mm/dd hh:mm:ss
	if( mode == FORTIME )
		strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	// バージョン用にはyyyymmddhhmmss
	else
		strTime.Format( _T("%04d%02d%02d%02d%02d%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

	return strTime;
}

/**
 * @class DeleteRows
 */
class DeleteRows
{
public:

	/**
	 * @brief	コンストラクタ
	 */
	DeleteRows( uh::tstring	input, uh::tstring file, uh::tstring fcname, uh::tstring log ){
		strInput  = input.c_str();	
		strFile   = file.c_str();
		strFcname = fcname.c_str();
		strLog    = log.c_str();
	};

	~DeleteRows(){};

	/**
	 * @brief	初期化
	 * @param	cArgc	[in]	引数の数
	 * @param	cArgv	[in]	引数
	 * @return	bool
	 */
	bool Init();

	/**
	 * @brief	実行
	 */
	bool Execute();

private:

	/**
	 * @brief	ログにヘッダーを出力
	 */
	void fnPrintHeader();

	/**
	 * @brief	編集開始
	 * @return	bool
	 */
	bool fnEditStart();

	/**
	 * @brief	編集終了
	 * @return	bool
	 */
	bool fnEditStop();

	/**
	 * @brief	削除処理
	 * @return	bool
	 */
	void fnDelete();



private:

	CString	strInput;	
	CString strFile;
	CString strFcname;
	CString strLog;

	IWorkspaceEditPtr m_ipWorkspace;

	ITablePtr         m_ipTable;

	std::ofstream     m_ofs;					// ログ出力ストリーム

	std::list<long>   m_lOIDList;				// OIDリスト列
};
