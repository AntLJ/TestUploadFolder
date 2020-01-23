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

// DeleteAddrPoint.h: DeleteAddrPoint �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKGPERROR_H__E00C35B4_4E7E_43DE_831F_D76F94982A69__INCLUDED_)
#define AFX_CHECKGPERROR_H__E00C35B4_4E7E_43DE_831F_D76F94982A69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SiNDYLib/Workspace.h>
#include <SiNDYLib/FeatureClass.h>
#include <SiNDYLib/Feature.h>
#include <ArcHelperEx/GlobalFunctions.h>

using namespace std;

/**
 * @class	DeleteAddrPoint
 */
class DeleteAddrPoint
{
public:

	DeleteAddrPoint();
	~DeleteAddrPoint();

	/**
	 * @brief	�g�p�@
	 */
	static void PrintUsage();

	/**
	 * @brief	������
	 * @param	cArgc	[in]	�����̐�
	 * @param	cArgv	[in]	����
	 * @return	bool
	 */
	bool Init( int cArgc, _TCHAR* cArgv[] );

	/**
	 * @brief	���s
	 */
	bool Execute();

private:

	/**
	 * @brief	�폜�Ώۃ��C����쐬
	 */
	void fnSetDeleteLayer();

	/**
	 * @brief	���ϐ�������擾
	 * @return	bool
	 */
	bool fnSetInfoFromEnv();

	/**
	 * @brief	�폜�����p�q�o�[�W�������쐬
	 * @return	bool
	 */
	bool fnCreateChildVer();

	/**
	 * @brief	OID���X�g���J���Avector�Ɋi�[
	 * @param	[in]	OID���X�g��
	 * @return	bool
	 */
	bool fnLoadOIDList( CString aOIDListName );

	/**
	 * @brief	���O�Ƀw�b�_�[���o��
	 */
	void fnPrintHeader();

	/**
	 * @brief	�ҏW�J�n
	 * @return	bool
	 */
	bool fnEditStart();

	/**
	 * @brief	�폜����
	 * @return	bool
	 */
	bool fnDelete();

	/**
	 * @brief	�ҏW�I��
	 * @return	bool
	 */
	bool fnEditStop();

private:

	CWorkspace			m_cWorkspace;			// ���[�N�X�y�[�X

	std::ofstream		m_ofs;					// ���O�o�̓X�g���[��
	CString				m_csLogFile;			// ���O�t�@�C���p�X

	list<long>			m_lOIDList;				// OID���X�g��

	CString				m_strFCName;			// �폜�Ώۃt�B�[�`���N���X��
	CFeatureClass		m_cTargetPointFC;		// �폜�Ώۃ|�C���g�t�B�[�`���N���X

	set<CString>		m_csSetDeleteLayer;		// �폜�Ώۃ��C����i�[�p 

	CMeshXY				m_cMXY;					// ���b�V��XY�擾�p

	int					m_iGeometryFieldCount;	// �W�I���g���̃t�B�[���h�ԍ�

};

#endif // !defined(AFX_CHECKGPERROR_H__E00C35B4_4E7E_43DE_831F_D76F94982A69__INCLUDED_)
