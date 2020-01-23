#pragma once

/**
 * @brief �f�[�^�x�[�X���[�e�B���e�B�N���X
 */
namespace DatabaseUtility
{
	/**
	 * @brief  �ҏW�J�n�����s
	 * @param  ipWorkspace  [in]  �ҏW�J�n�����{����WorkspaceEdit
	 * @param  strMsg       [out] �G���[���b�Z�[�W
	 * @retval true  �ҏW�J�n����������
	 * @retval false �ҏW�J�n�Ɏ��s����
	 */
	bool startEdit( IWorkspaceEditPtr& ipWorkspace, CString& strMsg );

	/**
	 * @brief  �ҏW�I�������s
	 * @param  ipWorkspaceEdit  [in]  StopEditing�����{����WorkspaceEdit
	 * @param  strMsg           [out] �G���[���b�Z�[�W
	 * @retval true  StopEditing����������
	 * @retval false StopEditing�����s����
	 */
	bool stopEdit( IWorkspaceEditPtr& ipWorkspace, CString& strMsg );

	/**
	 * @brief  �ҏW��j������
	 * @detail �ҏW���J�n����Ă��Ȃ��Ȃ牽�����Ȃ�
	 * @param  ipWorkspace [in]   �ҏW��j�����郏�[�N�X�y�[�X
	 * @param  strMsg    [out]  �G���[���b�Z�[�W
	 */
	void abortEdit( IWorkspaceEditPtr& ipWorkspace, CString& strMsg );

} // DatabaseUtility
