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
// 定義
//////////////////////////////////////////////////////////////////////

#pragma warning(push)
#pragma warning(disable : 4251)
/**
 * @brief 接続ファイルコントロールインタフェースクラス
 */
class SINDYLIB_API ConnectionFileCtrlI
{
public:
// 定義
	enum EConnectionFileMenu
	{
		kConnectionFileAdd = 1,
		kConnectionFileEdit,
		kConnectionFileDelete,
		kConnectionFileCopy,
	};

// 構築
	ConnectionFileCtrlI();

// 処理
	virtual int PopupConnectionFileMenu(HWND hWnd, bool bSelected);
	virtual bool ConnectionFileFunction(HWND hWnd, EConnectionFileMenu eMenu, LPCTSTR lpszFilePath);

	const ConnectionFile::Folder& GetConnectionFileFolder() const { return m_ConnectionFileFolder; }

protected:
// 処理
	bool RenameConnectionFile(LPCTSTR lpszOldName, LPCTSTR lpszNewName);

private:
// 静的処理
	static void fileToClipboard(HWND hWnd, LPCTSTR lpszFilePath, int nFlag);

// 変数
	ConnectionFile::Folder m_ConnectionFileFolder;
};
#pragma warning(pop)

} // namespace sindy

#endif // __SINDY_CONNECTION_FILE_CTRL_I__H__
