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
 * @file	FileEUC.h
 */

/**
 * @class	CFileEUC
 * @brief	EUC�t�@�C���N���X
 */
class CFileEUC
{
public:
	CFileEUC(void);
	~CFileEUC(void);

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
	void close();

	/**
	* @brief	�t�@�C����������
	* @param	strMsg	[in]	�o�̓��b�Z�[�W
	*/
	void write(const std::string& strMsg);

private:
	/**
	* @brief	SJIS��EUC�ϊ�
	* @param	str	[in]	������
	* @return	�ϊ��㕶����
	*/
	std::string ConvSJIS2EUC(const std::string& str) const;

private:
	std::ofstream	m_ofsFile;
};
