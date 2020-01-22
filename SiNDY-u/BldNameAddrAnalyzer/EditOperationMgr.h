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

#include <TDC/sindylib_core/Workspace.h>

/// 編集開始、終了管理用クラス
class EditOperationMgr
{
public:
	/**
	* @brief コンストラクタ
	*/
	EditOperationMgr(void){};
	EditOperationMgr(const IWorkspacePtr& work){ Init(work); }
	
	/**
	* @brief デストラクタ
	* @note 編集が開始されているワークスペースがあれば破棄する
	*/
	~EditOperationMgr(void);

	/**
	* @brief コンストラクタ
	* @param works [in] 管理するワークスペース
	* @param traceMsg [in] トレース用メッセージを表示するか
	*/
	void Init( const IWorkspacePtr& work, bool traceMsg = false ){ 
		m_work = work;
		m_work.m_bTrace = traceMsg;
	}

	/**
	* @brief 編集、編集オペレーションを開始する
	* @return なし（失敗した場合は例外発生）
	*/
	void Start();
	
	/**
	* @brief 編集、編集オペレーションを終了する
	* @return なし（失敗した場合は例外発生）
	*/
	void Stop();
	
	/**
	* @brief 編集を破棄する
	*/
	void Abort();

	bool IsBeingEdited();

private:
	sindy::CWorkspace m_work; //!< 管理するワークスペース
};

