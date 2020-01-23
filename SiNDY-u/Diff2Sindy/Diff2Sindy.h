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

#include "Arguments.h"
#include "Workspace.h"
#include <WinLib/VersionInfo.h>

using namespace sindy;


/**
 * @brief   メインクラス
 */
class CDiff2Sindy
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CDiff2Sindy( Arguments args )
	:m_args(args)
	{
		// CVersionより本ツールで使用する情報を取得
		CVersion version;
		m_InternalName = version.GetInternalName();		//!< ツール名称
		m_FileVersion = version.GetFileVersion();		//!< ファイルバージョン
		m_ProductVersion = version.GetProductVersion();	//!< プロダクトバージョン
	};

	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();

private:

	/**
	 * @brief   初期化
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool init();

private:

	/**
	 * @brief  処理ログを表示
	 */
	void outputExecInfo() const;

	/**
	 * @brief  複数のDBを一つのDBにする
	 * @param meshes [in] 処理対象メッシュリスト
	 * @retval true 処理成功
	 */
	bool convertDB( const std::vector<uh::tstring>& meshes );

	/**
	 * @brief  隣接マージを実施する
	 * @param meshes [in] 処理対象メッシュリスト
	 * @retval true:処理成功
	 */
	bool mergeDB( const std::vector<uh::tstring>& meshes );

	/**
 	 * @brief クリッピング処理を実施する
	 * @param meshes [in] 処理対象メッシュリスト
	 * @retval true 処理成功
	*/
	bool clipDB( const std::vector<uh::tstring>& meshes );

	mutable Arguments m_args;				//!< 引数
	CString m_InternalName;					//!< ツール名称
	CString m_FileVersion;					//!< ファイルバージョン
	CString m_ProductVersion;				//!< プロダクトバージョン
};

