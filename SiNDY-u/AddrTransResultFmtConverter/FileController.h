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
 * @brief  ���O�x�[�X�N���X
 */
class FileController
{
public:
	/**
	 * @brief  �R���X�g���N�^
	 */
	FileController(void):m_pFile(nullptr),m_bUTF8(false){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~FileController(void){ finalize(); }

	/**
	 * @brief  �t�@�C���I�쐬�A�w�b�_��������
	 */
	bool initialize( const std::wstring& strDirPath, const std::wstring& mode );

	/**
	 * @brief  �t�@�C���N���[�Y
	 */
	void finalize() 
	{ 
		if(!m_pFile)
			return;
		fclose(m_pFile);
		m_pFile = nullptr;
	}
	
	/**
	 * @brief  ���b�Z�[�W�o��
	 */
	void log( const std::wstring& strMessage );
	
	/**
	 * @brief  ���b�Z�[�W�̃R�[�h�̕ϊ�
	 */
	std::string wstringToString( const std::wstring& wstr );
	
	FILE*      m_pFile; //!< �t�@�C���X�g���[��
	bool       m_bUTF8; //!< �t�@�C����UTF-8�ŊJ���܂���
};
