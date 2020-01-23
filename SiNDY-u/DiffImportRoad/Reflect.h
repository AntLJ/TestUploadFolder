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
#include "LayerManage.h"

namespace Reflect{
	extern const long NO_CHG_MODE;	///< テストモード（更新しない）ビットフラグ
	extern const long TDC_MODE;		///< TDC優先モード ビットフラグ
	extern bool g_IsContinue;		///< 処理継続フラグ（中断管理用）
	extern long g_NowProccess;		///< 現在処理中のプロセスナンバー（何番目）
	extern long g_Mode;				///< 動作モード
	extern long g_CurrentMesh;		///< 処理中のメッシュコード
	extern MESH_SET g_MeshSet;		///< 処理対象のメッシュリスト（セット）
	extern CString g_Operator;		///< 最終更新者セット用のユーザ名
	extern CString g_FixUser;		///< FIXユーザ名（削除系のログ出力用）
}

/**
 * @brief 編集差分反映を総合的に管理・実行するクラス
 */
class CReflect
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CReflect(void);

	/**
	 * @brief デストラクタ
	 */
	~CReflect(void);

	/**
	 * @brief 初期化・事前準備。最初に一回だけ呼べばいいもの
	 *
	 * @param cConnectStr	[in]	サーバ接続文字列
	 * @param cOldPath		[in]	編集前PGDBのベースパス
	 * @param cNewPath		[in]	編集後PGDBのベースパス
	 * @param cMeshList		[in]	メッシュリストファイル名
	 * @param cMode			[in]	動作モード
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool init_once(const CString cConnectStr[2], LPCTSTR cOldPath, LPCTSTR cNewPath, LPCTSTR cMeshList, long cMode);

	/**
	 * @brief 初期化・事前準備。中断した時に最初期化が必要なものを含む
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool CReflect::init(void);

	/**
	 * @brief 編集差分反映作業メイン関数
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool execReflect(void);

private:
	CLayerManage m_LayerManage;	///< SDEのワークスペース・フィーチャクラス等の管理クラス
	CString m_OldPath;		///< 編集前PGDBのベースパス
	CString m_NewPath;		///< 編集後PGDBのベースパス
	CString m_ConnectStr[2];	///< サーバ接続文字列

	/**
	 * @brief 初回のみ必要な初期化・事前準備。フォルダの有無等一部は外で実行
	 *
	 * @param cMeshList	[in]	メッシュリストファイル名
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnFirstOnlyInit(LPCTSTR cMeshList);

	/**
	 * @brief コマンド付きメッシュリストの読み込みとメッシュポリゴン取得
	 *
	 * @param cMeshList	[in]	メッシュリストファイル名
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnMeshLoad(LPCTSTR cMeshList);
};
