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
 * @brief �f�[�^�x�[�X���[�e�B���e�B�N���X
 */
namespace databaseUtility
{
	/**
	 * @brief  �ҏW�J�n�����s
	 * @param  ipWorkspace  [in] �ҏW�J�n�����{����WorkspaceEdit
	 * @retval true  �ҏW�J�n����������
	 * @retval false �ҏW�J�n�Ɏ��s����
	 */
	bool startEdit( IWorkspaceEditPtr ipWorkspace );

	/**
	 * @brief  �ҏW�I�������s
	 * @param  ipWorkspaceEdit  [in] StopEditing�����{����WorkspaceEdit
	 * @param  bSave            [in] �ۑ����邩�ۂ�
	 * @retval true  StopEditing����������
	 * @retval false StopEditing�����s����
	 */
	bool stopEdit( IWorkspaceEditPtr ipWorkspace, bool bSave );

	/**
	 * @brief �ҏW��j������
	 * @detail �ҏW���J�n����Ă��Ȃ��Ȃ牽�����Ȃ�
	 * @param[in]  workspace �ҏW��j�����郏�[�N�X�y�[�X
	 */
	void abortEdit( IWorkspaceEditPtr workspace );

} // databaseUtility
