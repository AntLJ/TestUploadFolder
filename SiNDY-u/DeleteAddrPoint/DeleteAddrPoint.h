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

// DeleteAddrPoint.h: DeleteAddrPoint クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKGPERROR_H__E00C35B4_4E7E_43DE_831F_D76F94982A69__INCLUDED_)
#define AFX_CHECKGPERROR_H__E00C35B4_4E7E_43DE_831F_D76F94982A69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SiNDYLib/Workspace.h>
#include <SiNDYLib/FeatureClass.h>
#include <SiNDYLib/Feature.h>
#include <ArcHelperEx/GlobalFunctions.h>

using namespace std;

/**
 * @class	DeleteAddrPoint
 */
class DeleteAddrPoint
{
public:

	DeleteAddrPoint();
	~DeleteAddrPoint();

	/**
	 * @brief	使用法
	 */
	static void PrintUsage();

	/**
	 * @brief	初期化
	 * @param	cArgc	[in]	引数の数
	 * @param	cArgv	[in]	引数
	 * @return	bool
	 */
	bool Init( int cArgc, _TCHAR* cArgv[] );

	/**
	 * @brief	実行
	 */
	bool Execute();

private:

	/**
	 * @brief	削除対象レイヤ列作成
	 */
	void fnSetDeleteLayer();

	/**
	 * @brief	環境変数から情報取得
	 * @return	bool
	 */
	bool fnSetInfoFromEnv();

	/**
	 * @brief	削除処理用子バージョンを作成
	 * @return	bool
	 */
	bool fnCreateChildVer();

	/**
	 * @brief	OIDリストを開き、vectorに格納
	 * @param	[in]	OIDリスト名
	 * @return	bool
	 */
	bool fnLoadOIDList( CString aOIDListName );

	/**
	 * @brief	ログにヘッダーを出力
	 */
	void fnPrintHeader();

	/**
	 * @brief	編集開始
	 * @return	bool
	 */
	bool fnEditStart();

	/**
	 * @brief	削除処理
	 * @return	bool
	 */
	bool fnDelete();

	/**
	 * @brief	編集終了
	 * @return	bool
	 */
	bool fnEditStop();

private:

	CWorkspace			m_cWorkspace;			// ワークスペース

	std::ofstream		m_ofs;					// ログ出力ストリーム
	CString				m_csLogFile;			// ログファイルパス

	list<long>			m_lOIDList;				// OIDリスト列

	CString				m_strFCName;			// 削除対象フィーチャクラス名
	CFeatureClass		m_cTargetPointFC;		// 削除対象ポイントフィーチャクラス

	set<CString>		m_csSetDeleteLayer;		// 削除対象レイヤ列格納用 

	CMeshXY				m_cMXY;					// メッシュXY取得用

	int					m_iGeometryFieldCount;	// ジオメトリのフィールド番号

};

#endif // !defined(AFX_CHECKGPERROR_H__E00C35B4_4E7E_43DE_831F_D76F94982A69__INCLUDED_)
