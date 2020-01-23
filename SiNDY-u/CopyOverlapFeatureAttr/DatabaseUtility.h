#pragma once

/**
 * @brief データベースユーティリティクラス
 */
namespace DatabaseUtility
{
	/**
	 * @brief  編集開始を実行
	 * @param  ipWorkspace  [in]  編集開始を実施するWorkspaceEdit
	 * @param  strMsg       [out] エラーメッセージ
	 * @retval true  編集開始が成功した
	 * @retval false 編集開始に失敗した
	 */
	bool startEdit( IWorkspaceEditPtr& ipWorkspace, CString& strMsg );

	/**
	 * @brief  編集終了を実行
	 * @param  ipWorkspaceEdit  [in]  StopEditingを実施するWorkspaceEdit
	 * @param  strMsg           [out] エラーメッセージ
	 * @retval true  StopEditingが成功した
	 * @retval false StopEditingが失敗した
	 */
	bool stopEdit( IWorkspaceEditPtr& ipWorkspace, CString& strMsg );

	/**
	 * @brief  編集を破棄する
	 * @detail 編集が開始されていないなら何もしない
	 * @param  ipWorkspace [in]   編集を破棄するワークスペース
	 * @param  strMsg    [out]  エラーメッセージ
	 */
	void abortEdit( IWorkspaceEditPtr& ipWorkspace, CString& strMsg );

} // DatabaseUtility
