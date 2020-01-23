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
#include <fstream>

enum ErrorLevel
{
	Info,
	Warning,
	Error,
	Fatal,
};

class LogManager
{
public:
	LogManager(void);
	~LogManager(void);

	/**
	 * @brief ���ݎ����̎擾
	 * @retval ���݂̃V�X�e������
	 */
	CString GetCurrentTimeString()
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		CString currentTime;
		currentTime.AppendFormat(
			_T("%04d/%02d/%02d %02d:%02d:%02d"),
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		return currentTime;
	}

	/**
	 * @brief ���s���O���J��
	 * @param path [in] ���s���O�̃p�X
	 * @retval true ����
	 * @retval false ���s
	 */
	bool OpenRunLog(const CString& path);

	/**
	 * @brief �G���[���O���J��
	 * @param path [in] �G���[���O�̃p�X
	 * @retval true ����
	 * @retval false ���s
	 */
	bool OpenErrLog(const CString& path);

	/**
	 * @brief ���s���O�̏�������
	 * @param path [in] �������ޕ�����
	 */
	void WriteRunLog(const CString& message);

	/**
	 * @brief �G���[���O�ɃR�����g���o��
	 * @note �^����������̐擪��#��t���ďo�͂���B
	 */
	void WriteErrComment(const CString& comment);

	/**
	 * @brief �G���[���O��������
	 * @param layer [in] ���C����
	 * @param objectId [in] �I�u�W�F�N�gID
	 * @param level [in] �G���[���x��
	 * @param message [in] �G���[���b�Z�[�W
	 * @param info1 [in] ���1
	 * @param info2 [in] ���2
	 */
	void WriteErrLog(
		const CString& layer, long objectId,
		ErrorLevel level, const CString& message,
		const CString& info1 = _T(""), const CString& info2 = _T(""));

private:
	/**
	 * @brief ���O���J��
	 * @param path [in] �G���[���O�̃p�X
	 * @param ofs [out] ���O�t�@�C���X�g���[��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool OpenLog(const CString& path, std::ofstream& ofs);

	/**
	 * @brief ���O��������
	 * @param message [in] �������ޕ�����
	 * @param ofs [in] ���O�t�@�C���X�g���[��
	 */
	void WriteLog(const CString& message, std::ofstream& ofs);

private:
	std::ofstream m_ofsRunLog; //!< ���s���O�t�@�C���X�g���[��
	std::ofstream m_ofsErrLog; //!< �G���[���O�t�@�C���X�g���[��
};

