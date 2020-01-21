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

/**
 * @file グローバル変数実体ファイル
 * @brief DLL内でグローバルに使用する変数の実体を定義します。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "GlobalRules.h"
#include "GlobalFiles.h"
#include "GeometryRule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
	// ルール用
	CLogicRule	g_cLogicRule;									// 論理チェック用ルール管理クラス
	CSPRuleWrapper g_cRule;										// SiNDY編集ルール管理クラス

	bool g_bCheckUpdatedByArcObjects = true;					// CRow::Store()時にキャッシュ作成前を考慮して変更を確認するかどうか（SiNDY-eでは使用禁止！！）
	bool g_bSetHistory = true;									// CRow::Store()時に履歴をセットするかどうか
#ifdef _DEBUG
	bool g_bTraceUpdate = true;									// CRow::Store()時に保存状況のトレースを出すかどうか
#endif // ifdef _DEBUG
	bool g_bOutputTraceUpdate = false;							// CRow::Store()時の保存状況トレースをTRACEMESSAGEで出力するかどうか
	bool g_bCreateAllStoreInfo = true;							// CRow::Store()時に全てのエラーオブジェクトを生成するか
	bool g_bStoreContainer = true;								// CRow::Store()時にCContainerの中もStoreするかどうか
	bool g_bCalcStoreTime = false;								// CRow::Store()内のIRow::Store()時間を計測するかどうか
	DWORD g_dwTotalStoreTime = 0;								// CRow::Store()内のIRow::Store()時間の積算
	long g_lTotalStoreCount = 0;								// CRow::Store()内のIRow::Store()回数の積算
	double g_dTotalCacheTime = 0.0;								// CRow::CreateCacheにかかる時間の総計

	bool g_bBufferedSearch = true;								// CAnnotations::Select系で選択漏れがないように検索形状をバッファリングして検索するかどうか
	bool g_bDisableCheckToNoSubstance = false;					// 実体を持たないものをチェック対象から除外する
} // sindy
