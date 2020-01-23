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
#include "DataDef.h"
/**
 * @brief 設定ファイルと入力DBを読込む
 */
class CConfigLoader
{
public:
	/// コンストラクタ
	CConfigLoader(){}
	/// デストラクタ
	virtual ~CConfigLoader(){}

	/**
	 * @brief 設定ファイルを読込む
	 * @detail filepathが空文字列の場合、inputDB内の全レイヤを生成対象とする
	 * @param filepath [in] 設定ファイルのパス
	 * @param inputStr [in] 入力DBの接続文字列
	 */
	bool loadConfig(const uh::tstring& filepath, const uh::tstring& inputStr);

	/**
	 * @brief 生成するメッシュレイヤの情報を取得
	 * @param meshDBStr [in] 生成するメッシュレイヤの接続文字列
	 * @param layerName [in] 生成するレイヤ名
	 */
	bool loadMesh(const uh::tstring& meshDBStr, const uh::tstring& layerName);

	/**
	 * @brief 読込んだ結果を返す
	 * @return レイヤ・データセット・ドメイン生成のために必要な情報群
	 */
	const create_info::CREATEINFOMAP& getResult() const { return m_createInfoMap; }

protected:
	create_info::CREATEINFOMAP m_createInfoMap; //!< レイヤ・データセット・ドメイン生成のために必要な情報群
};
