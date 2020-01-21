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

#include "StdAfx.h"
#include "JusyoHelper.h"
#include <string>
#include <regex>

namespace addr
{

	// 接続
	bool CJusyoHelper::Connect(const _TCHAR* lpcszConnectString)
	{
		// ユーザ・パスワードに分解
		typedef std::basic_string<_TCHAR> tstring;
		tstring strConectString(lpcszConnectString);

		// 構文チェック
		typedef std::basic_regex<_TCHAR> tregex;
		typedef std::match_results<tstring::const_iterator> tmatch;
		tregex re(_T("(.+)/(.+)@(.+)"));
		tmatch results;
		if (std::regex_match(strConectString, results, re))
		{
			int nSlaPos = (int)strConectString.find(_T("/"));
			tstring strUser = strConectString.substr(0, nSlaPos);

			strConectString = strConectString.substr(nSlaPos + 1);

			int nAtMark = (int)strConectString.find(_T("@"));
			tstring strPass = strConectString.substr(0, nAtMark);
			tstring strService = strConectString.substr(nAtMark + 1);

			return Connect(strUser.c_str(), strPass.c_str(), strService.c_str());
		}
		else
		{
			m_strError.Format(_T("接続文字列が正しくありません(user/pass@接続識別子), %s"), lpcszConnectString);
			return false;
		}
	}

	// 接続
	bool CJusyoHelper::Connect(
		const _TCHAR* lpcszUser, const _TCHAR* lpcszPass, const _TCHAR* lpcszService)
	{
		try
		{
			CString connectString;
			connectString.Format(_T("Provider=MSDAORA;Data Source=%s;"), lpcszService);
			HRESULT hr = m_pDatabase->Open(
				_bstr_t(connectString), _bstr_t(lpcszUser), _bstr_t(lpcszPass), adConnectUnspecified);
			if (FAILED(hr))
			{
				m_strError = getErrorText();
				return false;
			}

			m_strConnectString.Format(_T("%s/%s@%s"), lpcszUser, lpcszPass, lpcszService);
			return true;
		}
		catch (_com_error& e)
		{
			m_strError = (LPCSTR)e.Description();
			return false;
		}
	}

	// 検索
	bool CJusyoHelper::Select(const _TCHAR* lpcszSQL, CJusyoRecordSet& rRecords)
	{
		_RecordsetPtr recordset("ADODB.Recordset");
		try
		{
			// 後々RecordCountを正しく取得するにはCursorLocationをadUseClient
			// CursorTypeをadOpenStaticにする必要がある
			// →これはたぶんOracleがそうなのであって、AccessとかSQLServerとかだと条件が変わる
			recordset->PutRefActiveConnection(m_pDatabase);
			recordset->CursorLocation = adUseClient;
			HRESULT hr = recordset->Open(_bstr_t(lpcszSQL), vtMissing, adOpenStatic, adLockReadOnly, adCmdText);
			if (FAILED(hr))
			{
				m_strError = getErrorText();
				return false;
			}

			rRecords.SetRecordSet(recordset, lpcszSQL);
			return true;
		}
		catch (_com_error& e)
		{
			m_strError = (LPCSTR)e.Description();
			return false;
		}
	}

	// 検索
	bool CJusyoHelper::Select(
		const _TCHAR* lpcszTable,
		const _TCHAR* lpcszWhere,
		const _TCHAR* lpcszSubFields,
		CJusyoRecordSet& rRecords)
	{
		// SQL作成
		CString strSQL;
		strSQL.Format(_T("select %s from %s"), lpcszSubFields ? lpcszSubFields : _T("*"), lpcszTable);

		// Where 句あれば
		if (lpcszWhere)
			strSQL.AppendFormat(_T(" where %s"), lpcszWhere);

		return Select(strSQL, rRecords);
	}


} // namespace addr
