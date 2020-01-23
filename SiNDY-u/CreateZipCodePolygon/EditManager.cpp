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

#include "stdafx.h"
#include "EditManager.h"

using namespace sindy;
using namespace sindy::errorcode;

EditManager::~EditManager(void)
{
	if (m_workspace.IsBeingEdited())
		StopEdit(false); // 編集は破棄
}

bool EditManager::StartEdit(bool undoRedo)
{
	if (m_workspace.GetNameString()->IsSDE())
	{
		IMultiuserWorkspaceEditPtr ipMultiWorkspace((IWorkspace*)m_workspace);
		if (ipMultiWorkspace)
		{
			VARIANT_BOOL isNonVersion = VARIANT_FALSE;
			ipMultiWorkspace->SupportsMultiuserEditSessionMode(esriMESMNonVersioned, &isNonVersion);
			if (isNonVersion)
			{
				// バージョン非対応なワークスペース
				if (FAILED(ipMultiWorkspace->StartMultiuserEditing(esriMESMNonVersioned)))
				{
					return false;
				}
			}
			else
			{
				// バージョン対応なワークスペース
				if (FAILED(ipMultiWorkspace->StartMultiuserEditing(esriMESMVersioned)))
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
	}
	else if (sindyErr_NoErr != m_workspace.StartEditing(undoRedo))
	{
		return false;
	}

	if (sindyErr_NoErr != m_workspace.StartEditOperation())
	{
		return false;
	}
	return true;
}

bool EditManager::StopEdit(bool saveEdit)
{
	if (!m_workspace.IsBeingEdited())
		return true;

	if (sindyErr_NoErr != m_workspace.StopEditOperation())
	{
		AbortEdit();
		return false;
	}

	if (sindyErr_NoErr != m_workspace.StopEditing(saveEdit))
	{
		return false;
	}
	return true;
}

bool EditManager::AbortEdit()
{
	if (sindyErr_NoErr != m_workspace.AbortEditOperation())
	{
		return false;
	}

	if (sindyErr_NoErr != m_workspace.StopEditing(false))
	{
		return false;
	}
	return true;
}
