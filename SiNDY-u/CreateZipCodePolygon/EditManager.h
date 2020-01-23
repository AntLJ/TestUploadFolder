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

class EditManager
{
public:
	EditManager(const sindy::CWorkspace& workspace)
	{
		m_workspace = (IWorkspace*)workspace;
	}
	virtual ~EditManager(void);

	/**
	 * @brief 編集開始
	 * @param undoRedo [in] Undo、Redoの有無
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool StartEdit(bool undoRedo);

	/**
	 * @brief 編集終了
	 * @param saveEdit [in] 編集保存の有無
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool StopEdit(bool saveEdit);

	/**
	 * @brief 編集破棄
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool AbortEdit();

private:
	sindy::CWorkspace m_workspace; //!< 編集対象ワークスペース
};
