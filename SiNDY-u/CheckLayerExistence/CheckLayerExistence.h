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

#if !defined(AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_)
#define AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_

#include <sindy/workspace.h>
#include <vector>
#include "ErrorHandle.h"

//��`�l
#define PROGNAME _T("CheckLayerExistence.exe")
#define TOOL_VERSION _T("Ver 1.0.0.0")

class CCheckLayerExistence
{
public:
	CCheckLayerExistence();

	virtual ~CCheckLayerExistence();

	/**
	 * @brief	�g�p�@
	 */
	static void printUsage();

	/**
	 * @brief	������
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool init( int argc, char* argv[] );

	/**
	 * @brief	���s
	 */
	void execute();

	/**
	 * @brief	���ϐ��擾
	 * @return	bool
	 */
	bool getEnv();

private:
	
	/**
	 * @brief	�������
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool checkArg( int argc, char* argv[] );

	/**
	 * @brief	���b�V�����X�g�ǂݍ���
	 * @param	lpszFile	[in]	���b�V�����X�g
	 * @return	bool
	 */
	bool loadMeshlist( LPCTSTR lpszFile );

	/**
	 * @brief	���C�����X�g�ǂݍ���
	 * @param	lpszFile	[in]	���C�����X�g�t�@�C����
	 * @return	bool
	 */
	bool loadLayerlist( LPCTSTR lpszFile );

	/**
	 * @brief	PGDB�t�@�C������K�v���擾
	 * @return	bool
	 */
	bool setInfoFromPGDB();

	/**
	 * @brief	���ۂɃ��C�������݂��邩�`�F�b�N�������Ȃ�
	 */
	void LayerCheck();

	void Mode_Meshcode();

	void Mode_Filename();

private:

	std::vector<std::string>	m_vecMesh;			// PGDB�t�@�C��
	std::vector<std::string>	m_vecLayerName;		// ���C�����i�[�p

	CString						m_FileName;			// PGDB�t�@�C����
	CString						m_FilePass;			// PGDB�t�@�C���t���p�X�i�[�p
	CString						m_FileProp;			// ���ϐ�����A�h���X�擾�p		

	IWorkspacePtr				m_ipPGDBWorkspace;	// PGDB�t�@�C���I�[�v���p���[�N�X�y�[�X
	IFeatureClassPtr			m_PGDBFC;			// ���C���̗L�����ʂ̂��߂̃��C���I�[�v���p�̃t�B�[�`���N���X

	CString						m_MeshlistName;		// ���b�V�����X�g��

	int							m_ModeFlag;			// ���[�h�w��p�t���O
													// 1�F���b�V�����́��~�̂ݏo�́@2�F���C�����ɏo��
	int							m_MeshSuccessFlag;	// ���b�V�����́��~����p�t���O
	CErrorHandle				m_cError;			// �G���[�n���h���N���X

	int							m_ExeModeFlag;		// 2�F���b�V�����X�g����p�X����������(meshcode.mdb)�@3�F���X�g���ɒ��ڃt�@�C�����L�q�p�X�͋N�_�p�X�ȉ�
};



#endif // !defined(AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_)

