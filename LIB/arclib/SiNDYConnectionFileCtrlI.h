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

#ifndef __SINDY_CONNECTION_FILE_CTRL_I__H__
#define __SINDY_CONNECTION_FILE_CTRL_I__H__

#include "exportimport.h"
#include "SiNDYConnectionFile.h"

namespace sindy {

//////////////////////////////////////////////////////////////////////
// ��`
//////////////////////////////////////////////////////////////////////

#pragma warning(push)
#pragma warning(disable : 4251)
/**
 * @brief �ڑ��t�@�C���R���g���[���C���^�t�F�[�X�N���X
 */
class SINDYLIB_API ConnectionFileCtrlI
{
public:
// ��`
	enum EConnectionFileMenu
	{
		kConnectionFileAdd = 1,
		kConnectionFileEdit,
		kConnectionFileDelete,
		kConnectionFileCopy,
	};

// �\�z
	ConnectionFileCtrlI();

// ����
	virtual int PopupConnectionFileMenu(HWND hWnd, bool bSelected);
	virtual bool ConnectionFileFunction(HWND hWnd, EConnectionFileMenu eMenu, LPCTSTR lpszFilePath);

	const ConnectionFile::Folder& GetConnectionFileFolder() const { return m_ConnectionFileFolder; }

protected:
// ����
	bool RenameConnectionFile(LPCTSTR lpszOldName, LPCTSTR lpszNewName);

private:
// �ÓI����
	static void fileToClipboard(HWND hWnd, LPCTSTR lpszFilePath, int nFlag);

// �ϐ�
	ConnectionFile::Folder m_ConnectionFileFolder;
};
#pragma warning(pop)

} // namespace sindy

#endif // __SINDY_CONNECTION_FILE_CTRL_I__H__
