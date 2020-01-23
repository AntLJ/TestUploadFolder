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
 * @brief データベースユーティリティクラス
 */
namespace databaseUtility
{
	/**
	 * @brief  編集開始を実行
	 * @param  ipWorkspace  [in] 編集開始を実施するWorkspaceEdit
	 * @retval true  編集開始が成功した
	 * @retval false 編集開始に失敗した
	 */
	bool startEdit( IWorkspaceEditPtr ipWorkspace );

	/**
	 * @brief  編集終了を実行
	 * @param  ipWorkspaceEdit  [in] StopEditingを実施するWorkspaceEdit
	 * @param  bSave            [in] 保存するか否か
	 * @retval true  StopEditingが成功した
	 * @retval false StopEditingが失敗した
	 */
	bool stopEdit( IWorkspaceEditPtr ipWorkspace, bool bSave );

	/**
	 * @brief 編集を破棄する
	 * @detail 編集が開始されていないなら何もしない
	 * @param[in]  workspace 編集を破棄するワークスペース
	 */
	void abortEdit( IWorkspaceEditPtr workspace );

} // databaseUtility
