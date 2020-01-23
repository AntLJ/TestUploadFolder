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
 * @file グローバルルール変数定義ファイル
 * @brief DLL内でグローバルに使用する変数を定義します。
 * データセグメントで定義しているわけではないのでここで
 * 定義された変数の内容は全てプロセス別になります。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _GLOBALARGUMENT_H_
#define _GLOBALARGUMENT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"
#include "TableType.h"

namespace sindy {
	class CRule;
	class CLogicRule;
	// ルール用
	extern SINDYLIB_API CRule			g_cRule;		//!< ルール管理クラス
	extern SINDYLIB_API CLogicRule		g_cLogicRule;	//!< 論理チェック用ルール管理クラス

	// CRow::Store制御用
	extern SINDYLIB_API bool	g_bCheckUpdatedByArcObjects;	//!< CRow::Store()時にキャッシュ作成前を考慮して変更を確認するかどうか（SiNDY-eでは使用禁止！！）
	extern SINDYLIB_API bool	g_bSetHistory;					//!< CRow::Store()時に履歴をセットするかどうか
#ifdef _DEBUG
	extern SINDYLIB_API bool	g_bTraceUpdate;					//!< CRow::Store()時に保存状況のトレースを出すかどうか
#endif // ifdef _DEBUG
	extern SINDYLIB_API bool	g_bOutputTraceUpdate;			//!< CRow::Store()時の保存状況トレースをTRACEMESSAGEで出力するかどうか
	extern SINDYLIB_API bool	g_bCreateAllStoreInfo;			//!< CRow::Store()時に全てのエラーオブジェクトを生成するか
	extern SINDYLIB_API bool	g_bStoreContainer;				//!< CRow::Store()時にCContainerの中もStoreするかどうか
	extern SINDYLIB_API bool	g_bCalcStoreTime;				//!< CRow::Store()内のIRow::Store()時間を計測するかどうか
	extern SINDYLIB_API DWORD	g_dwTotalStoreTime;				//!< CRow::Store()内のIRow::Store()時間の積算
	extern SINDYLIB_API long	g_lTotalStoreCount;				//!< CRow::Store()内のIRow::Store()回数の積算

	// CAnnotationsでの検索制御用
	extern SINDYLIB_API bool	g_bBufferedSearch;				//!< CAnnotations::Select系で選択漏れがないように検索形状をバッファリングして検索するかどうか
	extern SINDYLIB_API sindyTableType::ECode g_emViewTableType;//! CAnnotations::_SelectByShapeで不要なものをそぐ際に表示スケールを限定する際に使用する

	// チェック系制御用
	extern SINDYLIB_API bool	g_bDisableCheckToNoSubstance;	//!< 実体を持たないものをチェック対象から除外する
} // sindy

#endif // _GLOBALARGUMENT_H_
