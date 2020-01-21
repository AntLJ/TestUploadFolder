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
	 * @brief �Z��DB�w���p�[�N���X
	 * proteus�ւ̐ڑ���p�ł�(���[�J���l�[�~���O�̂�)
	 */
	class CJusyoHelper : boost::noncopyable
	{
	public:
		CJusyoHelper(void) : m_pDatabase("ADODB.Connection") {}

		/**
		 * @note �R���X�g���N�^�Őڑ�����ꍇ�́A�ďo����ɕK���ڑ����ۂ��m�F����(IsConnected)
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
		 * @brief  �ڑ�
		 * @param  lpcszConnectString [in] �ڑ�������(user/pass@service)
		 * @return bool
		 */
		bool Connect(const _TCHAR* lpcszConnectString);

		/**
		 * @brief  �ڑ�
		 * @param  lpcszUser [in] ���[�U��
		 * @param  lpcszPass [in] �p�X���[�h
		 * @param  lpcszService [in] �ڑ��L�q�q
		 * @return bool
		 */
		bool Connect(const _TCHAR* lpcszUser, const _TCHAR* lpcszPass, const _TCHAR* lpcszService);

		/**
		 * @brief  �ڑ������H
		 * @return bool
		 */
		bool IsConnected() { return m_pDatabase->State == adStateOpen; }

		/**
		 * @brief �R�l�N�V�����ؒf
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
		 * @brief  ����
		 * @param  lpcszSQL [in] SQL
		 * @param  rRecords [out] ���ʊi�[
		 * @return bool
		 */
		bool Select(const _TCHAR* lpcszSQL, CJusyoRecordSet& rRecords);

		/**
		 * @brief  ����
		 * @param  lpcszTable [in] �e�[�u����
		 * @param  lpcszWhere [in] Where��(NULL���ƁA�S��)
		 * @param  lpcszSubFields [in] �擾�t�B�[���h ���������́A�J���}��؂� ��NULL�̂Ƃ���*
		 * @param  rRecords [out] ���ʊi�[
		 * @return bool
		 */
		bool Select(
			const _TCHAR* lpcszTable,
			const _TCHAR* lpcszWhere,
			const _TCHAR* lpcszSubFields,
			CJusyoRecordSet& rRecords);

		/**
		 * @brief  �G���[���b�Z�[�W�擾
		 * @return �G���[���b�Z�[�W
		 */
		const _TCHAR* GetErrorMessage() const { return m_strError; }

		/**
		 * @brief  �ڑ�������
		 * @return �ڑ�������
		 */
		const _TCHAR* GetConnectString() const { return m_strConnectString; }

	private:

		/**
		 * @brief  m_pDatabase�̎��G���[�����擾����
		 * @note   Description����
		 * @return �G���[���̕�����
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
		_ConnectionPtr m_pDatabase;        //!< �f�[�^�x�[�X�I�u�W�F�N�g
		CString        m_strConnectString; //!< �ڑ�������
		CString        m_strError;         //!< �G���[���b�Z�[�W
	};

}	// namespace addr
