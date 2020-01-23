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
#include "WinLib\VersionInfo.h"

// フィーチャクラス共通属性更新用クラス
class CommonAttribute
{
public:
	CommonAttribute(void)
	{
		m_userName = _T("sindy");
		CVersion cVersion;
		m_toolName = cVersion.GetInternalName();

		// YYYY/MM/DD hh:mm:ss
		SYSTEMTIME stSystemTime = {};
		::GetLocalTime( &stSystemTime);
		m_modifyDate.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"),
			stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
			stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
	}
	virtual ~CommonAttribute(void);

	/*
	 * @brief 最終更新者用の文字列取得
	 * @retval 更新者名(sindy)
	 */
	CString GetLastUserName(){return m_userName;}

	/*
	 * @brief 最終更新プログラム名用の文字列取得
	 * @retval 更新プログラム名
	 */
	CString GetToolName(){return m_toolName;}

	// プログラム最終更新日時
	/*
	* @brief プログラム最終更新日時用の文字列取得
	* @retval 更新日(YYYY/MM/DD hh:mm:ss)
	*/
	CString GetModifyDate(){return m_modifyDate;}

private:
	CString m_userName; //!< 最終更新者名
	CString m_toolName; //!< 最終更新プログラム名
	CString m_modifyDate; //!< プログラム最終更新日時
};

