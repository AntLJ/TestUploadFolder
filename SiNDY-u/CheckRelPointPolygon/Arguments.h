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
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

public:
	uh::tstring m_PointConnect; //!< ポイントの接続先名
	uh::tstring m_PolyConnect;  //!< ポリゴンの接続先名
	uh::tstring m_Point;        //!< ポイントのフィーチャ名
	uh::tstring m_Poly;         //!< ポリゴンのフィーチャ名
	uh::tstring m_PointWhere;   //!< ポイントのWHERE句
	uh::tstring m_PolyWhere;    //!< ポリゴンのWHERE句
	uh::tstring m_RunLog;       //!< 実行ログ
	uh::tstring m_ErrLog;       //!< エラーログ
};
