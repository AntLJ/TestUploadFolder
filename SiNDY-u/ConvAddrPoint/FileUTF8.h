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

/**
 * @file	FileUTF8.h
 */

/**
 * @class	CFileUTF8
 * @brief	UTF8�t�@�C���N���X
 */
class CFileUTF8
{
public:
	CFileUTF8(void);
	~CFileUTF8(void);

	/**
	* @brief	�t�@�C���I�[�v��
	* @param	strDir	[in]	�f�B���N�g����
	* @param	strFile	[in]	�t�@�C����
	* @retval	true	����
	* @retval	false	���s
	*/
	bool open(const CString& strDir, const CString& strFile);

	/**
	* @brief	�t�@�C���N���[�Y
	*/
	void close(void);

	/**
	* @brief	�t�@�C����������
	* @param	strMsg	[in]	�o�̓��b�Z�[�W
	*/
	void write(const std::wstring& strMsg);

private:
	/**
	* @brief	UNICODE��UTF8�ϊ�
	* @param	str	[in]	������
	* @return	�ϊ��㕶����
	*/
	std::wstring UNICODEToUTF8(const CString& src) const;

private:
	std::wofstream	m_ofsFile;
};

