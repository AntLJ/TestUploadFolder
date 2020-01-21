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

#include "JusyoRecordSet.h"

namespace addr
{

	/**
	 * @class CJusyoHelper
	 * @brief 住所DBヘルパークラス
	 * proteusへの接続専用です(ローカルネーミングのみ)
	 */
	class CJusyoHelper : boost::noncopyable
	{
	public:
		CJusyoHelper(void) : m_pDatabase("ADODB.Connection") {}

		/**
		 * @note コンストラクタで接続する場合は、呼出し後に必ず接続成否を確認する(IsConnected)
		 */
		CJusyoHelper(const _TCHAR* lpcszConnectString) : m_pDatabase("ADODB.Connection")
		{
			Connect(lpcszConnectString);
		}

		CJusyoHelper(const _TCHAR* lpcszUser, const _TCHAR* lpcszPass, const _TCHAR* lpcszService)
			: m_pDatabase("ADODB.Connection")
		{
			Connect(lpcszUser, lpcszPass, lpcszService);
		}
		virtual ~CJusyoHelper(void) { Close(); }

		/**
		 * @brief  接続
		 * @param  lpcszConnectString [in] 接続文字列(user/pass@service)
		 * @return bool
		 */
		bool Connect(const _TCHAR* lpcszConnectString);

		/**
		 * @brief  接続
		 * @param  lpcszUser [in] ユーザ名
		 * @param  lpcszPass [in] パスワード
		 * @param  lpcszService [in] 接続記述子
		 * @return bool
		 */
		bool Connect(const _TCHAR* lpcszUser, const _TCHAR* lpcszPass, const _TCHAR* lpcszService);

		/**
		 * @brief  接続中か？
		 * @return bool
		 */
		bool IsConnected() { return m_pDatabase->State == adStateOpen; }

		/**
		 * @brief コネクション切断
		 */
		void Close()
		{
			if (m_pDatabase)
			{
				if (IsConnected())
				{
					m_pDatabase->Close();
					m_pDatabase = nullptr;
				}
			}
		}

		/**
		 * @brief  検索
		 * @param  lpcszSQL [in] SQL
		 * @param  rRecords [out] 結果格納
		 * @return bool
		 */
		bool Select(const _TCHAR* lpcszSQL, CJusyoRecordSet& rRecords);

		/**
		 * @brief  検索
		 * @param  lpcszTable [in] テーブル名
		 * @param  lpcszWhere [in] Where句(NULLだと、全件)
		 * @param  lpcszSubFields [in] 取得フィールド ※複数時は、カンマ区切り ※NULLのときは*
		 * @param  rRecords [out] 結果格納
		 * @return bool
		 */
		bool Select(
			const _TCHAR* lpcszTable,
			const _TCHAR* lpcszWhere,
			const _TCHAR* lpcszSubFields,
			CJusyoRecordSet& rRecords);

		/**
		 * @brief  エラーメッセージ取得
		 * @return エラーメッセージ
		 */
		const _TCHAR* GetErrorMessage() const { return m_strError; }

		/**
		 * @brief  接続文字列
		 * @return 接続文字列
		 */
		const _TCHAR* GetConnectString() const { return m_strConnectString; }

	private:

		/**
		 * @brief  m_pDatabaseの持つエラー情報を取得する
		 * @note   Descriptionだけ
		 * @return エラー情報の文字列
		 */
		CString getErrorText()
		{
			CString strError;
			long errCount = m_pDatabase->Errors->Count;
			for (long i = 0; i < errCount; ++i)
			{
				strError.AppendFormat(_T("%s\n"), m_pDatabase->Errors->GetItem(i)->Description);
			}
			m_pDatabase->Errors->Clear();
			return strError;
		}

	private:
		_ConnectionPtr m_pDatabase;        //!< データベースオブジェクト
		CString        m_strConnectString; //!< 接続文字列
		CString        m_strError;         //!< エラーメッセージ
	};

}	// namespace addr
