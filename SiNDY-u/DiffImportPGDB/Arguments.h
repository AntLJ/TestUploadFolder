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
#include <TDC/useful_headers/tstring.h>
#include "LogFile.h"

using namespace uh;

class CArguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CArguments(void);

	/**
	 * @brief デストラクタ
	 */
	~CArguments(void);

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse( int argc, _TCHAR* argv[] );

public:
	tstring            m_oldPath;       //!< 編集前PGDBパス
	tstring            m_newPath;       //!< 編集後PGDBパス
	tstring            m_meshPath;      //!< メッシュリストパス
	tstring            m_layerListPath; //!< レイヤリストパス(Gモードのみ)
	std::list<CString> m_layerList;     //!< レイヤリスト
	tstring            m_availDate;     //!< 有効日付(poiインポート時のみ)
	tstring            m_suffix;        //!< 拡張子オプション格納
	tstring            m_logPath;       //!< ログ出力先
	
	bool               m_testMode;      //!< テストモード（更新しない）
	bool               m_forceMode;     //!< 強制（バージョン非対応でも実行）モード
	bool               m_tdcMode;       //!< TDC優先モード
	bool               m_exceptSdeMode; //!< SDE以外(PGDB、FGDB)にインポートするモード
	bool               m_geospaceMode;  //!< GEOSPACE家形インポートモード
	bool               m_onlyImport;    //!< マージなしモード
	bool               m_emptyDBMode;   //!< 空DBインポートモード

	// 実行時オプションの格納がメインなので、基本constで扱うことになるが
	// CLogFileは後でメッシュコード格納したりするのでmutableにしておく
	mutable CLogFile   m_logFile;       //!< ログファイル管理クラス
};

