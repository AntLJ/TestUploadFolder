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
	Arguments() : m_bufferRange(50.0), m_chkFixclass(true) {};

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  コマンドライン引数の確認
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @param  bFile    [in]  ファイルであるか（デフォルトfalse）
	 * @retval true    引数OK
	 * @retval false   引数NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue, bool bFile = false );
public:

	uh::tstring  m_strBlNameDB;    //!< ビル名称DB接続先
	uh::tstring  m_strBuildingDB;  //!< 都市建物DB接続先
	uh::tstring  m_strAnnoDB;      //!< 注記DB接続先
	uh::tstring  m_strMeshDB;      //!< メッシュDB接続先
	uh::tstring  m_strOutput;      //!< アウトプット
	uh::tstring  m_strMeshList;    //!< メッシュリスト
	uh::tstring  m_strNgBuild;     //!< NG建物種別リスト
	uh::tstring  m_strErrWord;     //!< NG文字列リスト(error)
	uh::tstring  m_strWarnWord;    //!< NG文字列リスト(warning)
	uh::tstring  m_strBldcOKWord;  //!< 建物種別コード別OKワードリスト
	uh::tstring  m_strBldcNGWord;  //!< 建物種別コード別NGワードリスト
	bool         m_chkFixclass;    //!< 建物種別コード別ワードチェックを種別確定コードが未確認（0）のデータに対してのみ行うかどうか
	uh::tstring  m_strReplaceWord; //!< 文字列比較用置換リスト
	uh::tstring  m_strJoinAnno;    //!< 注記比較用結合文字列リスト
	uh::tstring  m_strExclude_mark;//!< 分割数チェック時除外記号リスト
	double       m_bufferRange;    //!< 探索バッファ(m)
};
