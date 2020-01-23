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
	 * @brief �ҏW�J�n
	 * @param undoRedo [in] Undo�ARedo�̗L��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool StartEdit(bool undoRedo);

	/**
	 * @brief �ҏW�I��
	 * @param saveEdit [in] �ҏW�ۑ��̗L��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool StopEdit(bool saveEdit);

	/**
	 * @brief �ҏW�j��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool AbortEdit();

private:
	sindy::CWorkspace m_workspace; //!< �ҏW�Ώۃ��[�N�X�y�[�X
};
