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

#include <iostream>
#include <algorithm>
#include <filesystem>
#include "tstring.h"

/**
 * @brief パスを探すUtilityクラス
 */
class PathFinder
{
public:
	PathFinder(){};
	~PathFinder(){};

	/**
	 * @brief  ディレクトリパスからファイルパスの一覧を取得
	 * @param  directoryName  [in]  ディレクトリパス
	 * @retval ファイルパスの一覧
	 */
	std::vector<uh::tstring> getFilePath(const uh::tstring& directoryName);

	/**
	 * @brief  ディレクトリパスからファイルパスの一覧を取得
	 * @param  directoryName  [in]  ディレクトリパス
	 * @param  filters		  [in]  フィルタにする文字列のリスト(正規表現非対応)
	 * @retval ファイルパスの一覧
	 */
	std::vector<uh::tstring> getFilePath(const uh::tstring& directoryName, std::vector<uh::tstring> filters);

	/**
	 * @brief  ディレクトリパスからファイルパスの一覧を取得
	 * @param  directoryName  [in]  ディレクトリパス
	 * @param  filter		  [in] フィルタにする文字列(正規表現非対応)
	 * @retval ファイルパスの一覧
	 */
	std::vector<uh::tstring> getFilePath(const uh::tstring& directoryName, uh::tstring filter);

	/**
	 * @brief  ディレクトリパスから指定した拡張子をもつファイルパスの一覧を取得
	 * @param  directoryName  [in]  ディレクトリパス
	 * @param  filter		  [in] フィルタにする拡張子(正規表現非対応)
	 * @retval ファイルパスの一覧
	 */
	std::vector<uh::tstring> getFilePathByExtension(const uh::tstring& directoryName, uh::tstring filter);

	/**
	 * @brief  ディレクトリパスから指定した拡張子をもつファイルパスの一覧を取得
	 * @param  directoryName  [in]  ディレクトリパス
	 * @param  filters		  [in]  フィルタにする拡張子のリスト(正規表現非対応)
	 * @retval ファイルパスの一覧
	 */
	std::vector<uh::tstring> getFilePathByExtension(const uh::tstring& directoryName, std::vector<uh::tstring> filters);
};

