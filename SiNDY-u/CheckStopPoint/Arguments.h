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


/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	Arguments(){
		m_extract = false;
		m_check   = false;
	};

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

public:
	uh::tstring	m_db_road;      //!< 道路DB接続用(チェックモードでは経年変化後の道路リンクとして使用)
	uh::tstring m_db_road_past; //!< 経年変化前の道路DB接続用
	uh::tstring m_db_stop;      //!< 一時停止DB接続用
	uh::tstring m_db_mesh;      //!< メッシュDB接続用
	uh::tstring m_output;       //!< ログ出力用
	uh::tstring m_meshlist;     //!< メッシュリスト用
	bool        m_extract;      //!< 抽出モード
	bool        m_check;        //!< チェックモード
};
