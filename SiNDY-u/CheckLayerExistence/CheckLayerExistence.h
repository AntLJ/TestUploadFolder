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

#if !defined(AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_)
#define AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_

#include <sindy/workspace.h>
#include <vector>
#include "ErrorHandle.h"

//定義値
#define PROGNAME _T("CheckLayerExistence.exe")
#define TOOL_VERSION _T("Ver 1.0.0.0")

class CCheckLayerExistence
{
public:
	CCheckLayerExistence();

	virtual ~CCheckLayerExistence();

	/**
	 * @brief	使用法
	 */
	static void printUsage();

	/**
	 * @brief	初期化
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool init( int argc, char* argv[] );

	/**
	 * @brief	実行
	 */
	void execute();

	/**
	 * @brief	環境変数取得
	 * @return	bool
	 */
	bool getEnv();

private:
	
	/**
	 * @brief	引数解析
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool checkArg( int argc, char* argv[] );

	/**
	 * @brief	メッシュリスト読み込み
	 * @param	lpszFile	[in]	メッシュリスト
	 * @return	bool
	 */
	bool loadMeshlist( LPCTSTR lpszFile );

	/**
	 * @brief	レイヤリスト読み込み
	 * @param	lpszFile	[in]	レイヤリストファイル名
	 * @return	bool
	 */
	bool loadLayerlist( LPCTSTR lpszFile );

	/**
	 * @brief	PGDBファイルから必要情報取得
	 * @return	bool
	 */
	bool setInfoFromPGDB();

	/**
	 * @brief	実際にレイヤが存在するかチェックをおこなう
	 */
	void LayerCheck();

	void Mode_Meshcode();

	void Mode_Filename();

private:

	std::vector<std::string>	m_vecMesh;			// PGDBファイル
	std::vector<std::string>	m_vecLayerName;		// レイヤ名格納用

	CString						m_FileName;			// PGDBファイル名
	CString						m_FilePass;			// PGDBファイルフルパス格納用
	CString						m_FileProp;			// 環境変数からアドレス取得用		

	IWorkspacePtr				m_ipPGDBWorkspace;	// PGDBファイルオープン用ワークスペース
	IFeatureClassPtr			m_PGDBFC;			// レイヤの有無判別のためのレイヤオープン用のフィーチャクラス

	CString						m_MeshlistName;		// メッシュリスト名

	int							m_ModeFlag;			// モード指定用フラグ
													// 1：メッシュ毎の○×のみ出力　2：レイヤ毎に出力
	int							m_MeshSuccessFlag;	// メッシュ毎の○×判定用フラグ
	CErrorHandle				m_cError;			// エラーハンドルクラス

	int							m_ExeModeFlag;		// 2：メッシュリストからパスを自動生成(meshcode.mdb)　3：リスト内に直接ファイル名記述パスは起点パス以下
};



#endif // !defined(AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_)

