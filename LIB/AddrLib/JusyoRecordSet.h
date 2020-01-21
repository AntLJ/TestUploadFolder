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
	 * @brief �Z�����R�[�h�Z�b�g�Ǘ��N���X
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
		 * @brief ���R�[�h�Z�b�g
		 * @param pDynaset [in] ODynaset�|�C���^
		 * @param lpcszSQL [in] ���sSQL
		 */
		void SetRecordSet(const _RecordsetPtr& recordset, const _TCHAR* lpcszSQL);

		/**
		 * @brief �ؒf
		 */
		void Close()
		{
			if (m_recordset->State == adStateOpen)
			{
				m_recordset->Close();
				m_recordset = nullptr;
			}
		}

		//@{ @name ���R�[�h�A�N�Z�X�֘A
		/**
		 * <h4>�ȉ��̂悤�Ȏg������z��</h4>
		 * @code
		 * begin(); // �X�^�[�g�ɖ߂���
		 * while( !end() )
		 * {
		 *   // �l���擾�����肷��
		 *   ....
		 *   next(); // ���̃��R�[�h��
		 * }
		 * @endcode
		 */
		bool begin() { return SUCCEEDED(m_recordset->MoveFirst()); }
		bool next() { return SUCCEEDED(m_recordset->MoveNext()); }
		bool end() { return m_recordset->EndOfFile == VARIANT_FALSE ? false : true; }

		/**
		 * @brief  �f�[�^�擾(_varian_t)
		 * @param  lIndex [in] �t�B�[���h�C���f�b�N�X
		 * @param  pVal [out] _varian_t
		 * @return bool
		 */
		bool GetValue(long lIndex, _variant_t& pVal) const;

		/**
		 * @brief  �f�[�^�擾(_varian_t)
		 * @param  lpcszField [in] �t�B�[���h��
		 * @param  pVal [out] _variant_t
		 * @return bool
		 */
		bool GetValue(const _TCHAR* lpcszField, _variant_t& pVal) const;

		/**
		 * @brief  �l�擾(long)
		 * @param  lIndex [in] �t�B�[���h�C���f�b�N�X
		 * @return long
		 */
		long GetLongValue(long lIndex);

		/**
		 * @brief  �l�擾(long)
		 * @param  lcpszField [in] �t�B�[���h��
		 * @return long
		 */
		long GetLongValue(const _TCHAR* lpcszField);

		/**
		 * @brief  �l�擾(double)
		 * @param  lIndex [in] �t�B�[���h�C���f�b�N�X
		 * @return double
		 */
		double GetDoubleValue(long lIndex);

		/**
		 * @brief  �l�擾(double)
		 * @param  lcpszField [in] �t�B�[���h��
		 * @return double
		 */
		double GetDoubleValue(const _TCHAR* lpcszField);

		/**
		 * @brief  �l�擾(char)
		 * @param  lIndex [in] �t�B�[���h�C���f�b�N�X
		 * @return CString
		 */
		CString GetStringValue(long lIndex);

		/**
		 * @brief  �l�擾(char)
		 * @param  lcpszField [in] �t�B�[���h��
		 * @return CString
		 */
		CString GetStringValue(const _TCHAR* lpcszField);

		/**
		 * @brief  �������ʎ擾
		 * @return ����(-1�Ȃ�A�N�G�������s���Ă���)
		 */
		long GetRecordCount() const { return m_recordset->GetRecordCount(); }

		//@}

		/**
		 * @brief  �G���[���b�Z�[�W�擾
		 * @return �G���[���b�Z�[�W
		 */
		const _TCHAR* GetErrorMessage() const { return m_strError; }

	private:
		_RecordsetPtr   m_recordset; //!< ���ʊi�[�p
		CString         m_strSQL;    //!< ���ݎ��s����SQL
		mutable CString	m_strError;  //!< �G���[���b�Z�[�W
	};

}	// namespace addr
