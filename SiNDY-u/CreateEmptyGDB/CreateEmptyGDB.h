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
 * @brief 指定のパスに空のGDBを作成する
 */
class CCreateEmptyGDB
{
	/**
	 * @brief フォルダの階層構造定義
	 */
	struct FolderTree
	{
		uh::tstring m_folderName;                //!< フォルダ名
		std::vector<FolderTree> m_children;      //!< フォルダの子要素
		boost::optional<uh::tstring> m_fileName; //!< フォルダ直下に生成するファイル名
	};

public:
	/// コンストラクタ
	CCreateEmptyGDB(){}
	/// デストラクタ
	virtual ~CCreateEmptyGDB(){}

	/**
	 * @brief 生成するDBのフォーマットを設定
	 * @detail outputにフォルダを指定している場合、不要
	 * @param format [in] フォーマット
	 */
	void setFormat(const uh::tstring& format);

	/**
	 * @brief DBの生成先パスを設定
	 * @param path [in] 生成先のパス
	 * @param type [in] 生成するDBのタイプ
	 */
	void setOutput( const uh::tstring& path, OUTPUT_TYPE type);

	/**
	 * @brief メッシュリストを設定する
	 * @param mesh [in] メッシュリスト
	 */
	void setMeshList( const std::vector<uh::tstring>& meshList );

	/**
	 * @brief 生成先のフォルダ・ファイルを生成
	 * @retval true 致命的な問題が起こらずにファイルを作成できた
	 * @retval false 致命的な問題が起こった
	 */
	bool create();

	/**
	 * @brief 生成対象になったDBのパスリストの参照を取得
	 */
	const std::vector<uh::tstring>& getTargetPathList() const
	{
		return m_targetPathList;
	}

protected:
	/**
	 * @brief filepathの箇所の空のDBを作成する
	 * @param filename [in] 作成するファイルの名前(拡張子除く)
	 * @param ext      [in] 作成するファイルの拡張子(.付き)
	 * @param basePath [in] 作成先のフォルダパス
	 * @return 処理の成否
	 */
	bool createFile(const uh::tstring& filename, const uh::tstring& ext, const uh::tstring& basePath);

	/**
	 * @brief 階層的にフォルダ・ファイルを作成する
	 * @param folderPath [in] 作成先のフォルダパス
	 * @return 処理の成否
	 */
	bool createLevelFolder(const uh::tstring& folderPath);

	/**
	 * @brief FolderTreeの情報を元にフォルダ・ファイルを作成する
	 * @note ファイル・フォルダが存在しない場合のみ、作成する
	 * @param node     [in] 作成するフォルダ・ファイルの階層構造をもつ構造体
	 * @param rootPath [in] フォルダ作成のルートとなるフォルダ
	 * @retval true  ファイル・フォルダの生成に成功
	 * @retval false ファイル・フォルダの生成に失敗
	 */
	bool createFilesFromTree(const FolderTree& node, const boost::filesystem::path& rootPath);

private:
	uh::tstring m_format;     //!< 生成するDBのフォーマット
	uh::tstring m_output;     //!< 出力先パス
	OUTPUT_TYPE m_outputType; //!< 出力先タイプ
	std::vector<uh::tstring> m_meshList; //!< メッシュリスト

	/**
	 * @brief create()の対象となったDBのパス一覧
	 * @detail 作成されていなくても、対象になった時点でこのパスに追加される
	 */
	std::vector<uh::tstring> m_targetPathList;
};
