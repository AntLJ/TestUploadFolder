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

#define TOOL_VERSION	_T("Ver 0.9,0,1")

class CApp
{
public:
	CApp(void);
	~CApp(void);

	/**
	 * @brief	�g�p�@
	 * @retval	false
	 */
	static bool printUsage();

	/**
	 * @brief	������
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool init( int argc, TCHAR* argv[] );

	/**
	 * @brief	���s
	 * @return	bool
	 */
	bool execute();

	/**
	 * @brief	�R���p�N�g��
	 * @param	lpszFile	[in]	PGDB�t�@�C��
	 * @return	bool
	 */
	bool compact( LPCTSTR lpszFile );

private:

	/**
	 * @brief	�����`�F�b�N
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool checkArg( int argc, TCHAR* argv[] );

	/**
	 * @brief	���b�V�����X�g�ǂݍ���
	 * @param	lpszFile	[in]	���b�V�����X�g
	 * @return	bool
	 */
	bool loadMeshList( LPCTSTR lpszFile );

///////////////////////////////////
//	�����o�ϐ�
///////////////////////////////////

	CString					m_strBaseDir;		/// �x�[�X�f�B���N�g��

	std::vector<CString>	m_vecFile;			/// PGDB�t�@�C�����X�g

	std::vector<long>		m_vecMesh;			/// ���b�V�����X�g

};
