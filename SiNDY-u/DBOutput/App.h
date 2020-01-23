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

#include "InitFile.h"

/**
 * @enum	emQueryMode
 * @brief	対象オブジェクト絞込みモード
 */
enum emQueryMode
{
	kNone,		//!< 絞込みなし
	kWhere,		//!< 属性
	kOID,		//!< OIDリスト
	kMesh,		//!< メッシュリスト
	kMeshWhere,	//!< メッシュリスト+属性
};

/**
 * @class	CApp
 */
class CApp
{
public:
	CApp(void) : m_emQueryMode(kNone), m_bLabelPoint(false), m_bSkip(false){}
	~CApp(void){}

	/**
	 * @brief	使用法
	 */
	static void printUsage();

	/**
	 * @brief	初期化
	 * @param	argc	[in]	コマンドライン引数の数
	 * @param	argv	[in]	コマンドライン引数
	 * @retval	true	処理成功
	 * @retval	false	処理失敗
	 */
	bool init( int argc, TCHAR* argv[] );

	/**
	 * @brief	実行
	 * @retval	true	処理成功
	 * @retval	false	処理失敗
	 */
	bool execute();

private:

	/**
	 * @brief	OID・メッシュリスト読み込み(兼用)
	 * @param	lpcszFile	[in]		リストファイル
	 * @param	rList		[in,out]	リスト
	 * @retval	true	処理成功
	 * @retval	false	処理失敗
	 */
	bool loadList( LPCTSTR lpcszFile, std::list<long>& rList );

	/**
	 * @brief	指定メッシュコードのメッシュ形状取得
	 * @param	lMeshCode	[in]	メッシュコード
	 * @return	メッシュ矩形
	 */
	IGeometryPtr GetMeshShape( long lMeshCode );

	/**
	 * @brief	出力
	 * @param	ipRow			[in]	IRow
	 * @param	strPostErrMsg	[in]	エラーメッセージの最後に出力する文字列
	 * @retval	true	処理成功
	 * @retval	false	処理失敗
	 */
	bool output( _IRow* ipRow, const CString& strPostErrMsg );

private:

	CInitFile				m_cInit;			//!< 設定情報	

	IWorkspacePtr			m_ipWorkspace;		//!< ワークスペース
	ITablePtr				m_ipTable;			//!< 出力対象テーブル
	IFeatureClassPtr		m_ipMeshFC;			//!< メッシュフィーチャクラス

	emQueryMode				m_emQueryMode;		//!< オブジェクト絞込みモード
	CString					m_strWhereClause;	//!< Where句
	CString					m_strOIDList;		//!< OIDリスト
	CString					m_strMeshList;		//!< メッシュリスト
	bool					m_bLabelPoint;		//!< ポリゴンに対する座標値を、重心ではなくラベルポイントで取得するかどうか
	bool					m_bSkip;			//!< 指定した項目がテーブルに無い場合エラーとしない(スキップして出力する)
};
