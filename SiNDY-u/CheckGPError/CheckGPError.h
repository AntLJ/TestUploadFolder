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

// CheckGPError.h: CheckGPError �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKGPERROR_H__E00C35B4_4E7E_43DE_831F_D76F94982A69__INCLUDED_)
#define AFX_CHECKGPERROR_H__E00C35B4_4E7E_43DE_831F_D76F94982A69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SiNDYLib/Workspace.h>
#include <SiNDYLib/FeatureClass.h>
#include <AddrLib/AreacodeHelper.h>
#include "ListHandle.h"

using namespace addr;

/**
 * @class	CheckGPError
 */
class CheckGPError
{
public:

	CheckGPError();
	~CheckGPError();

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
	 * @brief	���ϐ�������擾
	 * @return	bool
	 */
	bool fnSetInfoFromEnv();

	/**
	 * @brief	�ҏW�J�n
	 * @return	bool
	 */
	bool fnEditStart();

	/**
	 * @brief	�ҏW�I��
	 * @return	bool
	 */
	bool fnEditStop();
	
	/** 
	 * @brief	�|�C���g�ǉ��ʒu�̃`�F�b�N
	 * @param	cRec	[in]	���R�[�h
	 * @return	EResultStat
	 */
	EResultStat checkInsertPoint( CLRec& cRec );

	/**
	 * @brief	�|�C���g�ǉ�
	 * @parma	cRec	[in]	���R�[�h
	 * @return	EResultStat(�`�F�b�N���ʃX�e�[�^�X)
	 */
	EResultStat insertPoint( CLRec &cRec );

	/**
	 * @brief	���O�o��
	 * @param	emStatus	[in]	�`�F�b�N���ʃX�e�[�^�X
	 * @param	cRec		[in]	���R�[�h
	 */
	void outputLog( EResultStat emStatus, CLRec &cRec );

private:

	CWorkspace			m_cWorkspace;			//!< ���[�N�X�y�[�X
	CWorkspace			m_cWorkspace_Ref;		//!< �X�V���[�h���Q�Ɨp���[�N�X�y�[�X(bug9300)

	bool	m_bUpdateMode;						//!< �X�V���[�h���ۂ�(���s�S�̂̃��[�h)
	bool	m_bAddMode;							//!< �ǉ����[�h���ۂ�(�e���R�[�h���Ƃ̃��[�h)
	bool	m_bCharAddrMode;					//!< �����t���Z���l�����[�h(Bug7212)

	std::ofstream		m_ofs;					//!< ���O�o�̓X�g���[��

	CFeatureClass		m_cCityAdminFC;			//!< �s�s�n�}�s���E�t�B�[�`���N���X
	CFeatureClass		m_cCityAdmin_Ref;		//!< �X�V���[�h���Q�Ɨp�s�s�n�}�s���E�t�B�[�`���N���X(bug9300)
	CFeatureClass		m_cGouPointFC;			//!< ���|�C���g�t�B�[�`���N���X
	CFeatureClass		m_cGouPointFC_Ref;		//!< �X�V���[�h���Q�Ɨp���|�C���g�t�B�[�`���N���X(bug9300)
	CFeatureClass		m_cCSPointFC;			//!< CS�|�C���g�t�B�[�`���N���X

	CListHandle			m_cListHandle;			//!< ���X�g�n���h���N���X

	CAreaCodeHelper		m_cAcHelper;			//!< �Z��DB�w���p�[�N���X	
};

#endif // !defined(AFX_CHECKGPERROR_H__E00C35B4_4E7E_43DE_831F_D76F94982A69__INCLUDED_)
