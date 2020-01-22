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

// CheckGPError.h: CheckGPError クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKGPERROR_H__E00C35B4_4E7E_43DE_831F_D76F94982A69__INCLUDED_)
#define AFX_CHECKGPERROR_H__E00C35B4_4E7E_43DE_831F_D76F94982A69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SiNDYLib/Workspace.h>
#include <SiNDYLib/FeatureClass.h>
#include <AddrLib/AreacodeHelper.h>
#include "ListHandle.h"

using namespace addr;

/**
 * @class	CheckGPError
 */
class CheckGPError
{
public:

	CheckGPError();
	~CheckGPError();

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
	 * @brief	環境変数から情報取得
	 * @return	bool
	 */
	bool fnSetInfoFromEnv();

	/**
	 * @brief	編集開始
	 * @return	bool
	 */
	bool fnEditStart();

	/**
	 * @brief	編集終了
	 * @return	bool
	 */
	bool fnEditStop();
	
	/** 
	 * @brief	ポイント追加位置のチェック
	 * @param	cRec	[in]	レコード
	 * @return	EResultStat
	 */
	EResultStat checkInsertPoint( CLRec& cRec );

	/**
	 * @brief	ポイント追加
	 * @parma	cRec	[in]	レコード
	 * @return	EResultStat(チェック結果ステータス)
	 */
	EResultStat insertPoint( CLRec &cRec );

	/**
	 * @brief	ログ出力
	 * @param	emStatus	[in]	チェック結果ステータス
	 * @param	cRec		[in]	レコード
	 */
	void outputLog( EResultStat emStatus, CLRec &cRec );

private:

	CWorkspace			m_cWorkspace;			//!< ワークスペース
	CWorkspace			m_cWorkspace_Ref;		//!< 更新モード時参照用ワークスペース(bug9300)

	bool	m_bUpdateMode;						//!< 更新モードか否か(実行全体のモード)
	bool	m_bAddMode;							//!< 追加モードか否か(各レコードごとのモード)
	bool	m_bCharAddrMode;					//!< 文字付き住所考慮モード(Bug7212)

	std::ofstream		m_ofs;					//!< ログ出力ストリーム

	CFeatureClass		m_cCityAdminFC;			//!< 都市地図行政界フィーチャクラス
	CFeatureClass		m_cCityAdmin_Ref;		//!< 更新モード時参照用都市地図行政界フィーチャクラス(bug9300)
	CFeatureClass		m_cGouPointFC;			//!< 号ポイントフィーチャクラス
	CFeatureClass		m_cGouPointFC_Ref;		//!< 更新モード時参照用号ポイントフィーチャクラス(bug9300)
	CFeatureClass		m_cCSPointFC;			//!< CSポイントフィーチャクラス

	CListHandle			m_cListHandle;			//!< リストハンドルクラス

	CAreaCodeHelper		m_cAcHelper;			//!< 住所DBヘルパークラス	
};

#endif // !defined(AFX_CHECKGPERROR_H__E00C35B4_4E7E_43DE_831F_D76F94982A69__INCLUDED_)
