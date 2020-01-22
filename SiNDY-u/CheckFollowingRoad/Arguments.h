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

#include "MeshType.h"

/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	Arguments() : m_MeshType( meshtype::kUnknown ) {}

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);
	
	/**
	 * @brief メッシュリストの解析
	 * @note 処理メッシュ単位の取得、処理対象のメッシュコード一覧の取得、リスト内の不正な文字列の取得を行う
	 * @param	meshes [out] 処理対象のメッシュコードの一覧
	 * @param	mapInvalidRow [out]	メッシュリスト内の不正な文字列と行番号のペア
	 * @retval	true	メッシュリストの内容に問題なし
	 * @retval	false	不正な文字列が存在、または2種類以上のメッシュが混在している
	 */
	bool parseMeshes( std::vector<long>& meshes, std::map<int, CString>& mapInvalidRow );

public:
	uh::tstring	m_BaseDB;	//!< 比較元バージョンのDB
	uh::tstring m_CompDB;	//!< 比較先バージョンのDB
	uh::tstring m_MeshList;	//!< メッシュリスト
	CComVariant m_ModDate;	//!< 最終更新日
	uh::tstring m_Runlog;	//!< 実行ログ
	uh::tstring m_Errlog;	//!< エラーログ

	meshtype::MeshType m_MeshType;	//!< 処理するメッシュ単位
};
