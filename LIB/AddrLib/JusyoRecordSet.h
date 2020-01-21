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

#include <memory>
#include <boost/utility.hpp>

namespace addr
{

	/**
	 * @class CJysyoRecordSet
	 * @brief 住所レコードセット管理クラス
	 */
	class CJusyoRecordSet : boost::noncopyable
	{
	public:

		CJusyoRecordSet() : m_recordset("ADODB.Recordset") {}

		virtual ~CJusyoRecordSet() { Close(); }

		CJusyoRecordSet(_RecordsetPtr recordset, const _TCHAR* lpcszSQL) : m_recordset("ADODB.Recordset")
		{
			SetRecordSet(recordset, lpcszSQL);
		}

		/**
		 * @brief レコードセット
		 * @param pDynaset [in] ODynasetポインタ
		 * @param lpcszSQL [in] 実行SQL
		 */
		void SetRecordSet(const _RecordsetPtr& recordset, const _TCHAR* lpcszSQL);

		/**
		 * @brief 切断
		 */
		void Close()
		{
			if (m_recordset->State == adStateOpen)
			{
				m_recordset->Close();
				m_recordset = nullptr;
			}
		}

		//@{ @name レコードアクセス関連
		/**
		 * <h4>以下のような使い方を想定</h4>
		 * @code
		 * begin(); // スタートに戻して
		 * while( !end() )
		 * {
		 *   // 値を取得したりする
		 *   ....
		 *   next(); // 次のレコードへ
		 * }
		 * @endcode
		 */
		bool begin() { return SUCCEEDED(m_recordset->MoveFirst()); }
		bool next() { return SUCCEEDED(m_recordset->MoveNext()); }
		bool end() { return m_recordset->EndOfFile == VARIANT_FALSE ? false : true; }

		/**
		 * @brief  データ取得(_varian_t)
		 * @param  lIndex [in] フィールドインデックス
		 * @param  pVal [out] _varian_t
		 * @return bool
		 */
		bool GetValue(long lIndex, _variant_t& pVal) const;

		/**
		 * @brief  データ取得(_varian_t)
		 * @param  lpcszField [in] フィールド名
		 * @param  pVal [out] _variant_t
		 * @return bool
		 */
		bool GetValue(const _TCHAR* lpcszField, _variant_t& pVal) const;

		/**
		 * @brief  値取得(long)
		 * @param  lIndex [in] フィールドインデックス
		 * @return long
		 */
		long GetLongValue(long lIndex);

		/**
		 * @brief  値取得(long)
		 * @param  lcpszField [in] フィールド名
		 * @return long
		 */
		long GetLongValue(const _TCHAR* lpcszField);

		/**
		 * @brief  値取得(double)
		 * @param  lIndex [in] フィールドインデックス
		 * @return double
		 */
		double GetDoubleValue(long lIndex);

		/**
		 * @brief  値取得(double)
		 * @param  lcpszField [in] フィールド名
		 * @return double
		 */
		double GetDoubleValue(const _TCHAR* lpcszField);

		/**
		 * @brief  値取得(char)
		 * @param  lIndex [in] フィールドインデックス
		 * @return CString
		 */
		CString GetStringValue(long lIndex);

		/**
		 * @brief  値取得(char)
		 * @param  lcpszField [in] フィールド名
		 * @return CString
		 */
		CString GetStringValue(const _TCHAR* lpcszField);

		/**
		 * @brief  検索結果取得
		 * @return 件数(-1なら、クエリが失敗している)
		 */
		long GetRecordCount() const { return m_recordset->GetRecordCount(); }

		//@}

		/**
		 * @brief  エラーメッセージ取得
		 * @return エラーメッセージ
		 */
		const _TCHAR* GetErrorMessage() const { return m_strError; }

	private:
		_RecordsetPtr   m_recordset; //!< 結果格納用
		CString         m_strSQL;    //!< 現在実行中のSQL
		mutable CString	m_strError;  //!< エラーメッセージ
	};

}	// namespace addr
