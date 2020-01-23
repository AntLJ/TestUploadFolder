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

// CheckTelTPError.h: CheckTelTPError �N���X�̃C���^�[�t�F�C�X
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
 * @class	CheckTelTPError
 */
class CheckTelTPError
{
public:

	CheckTelTPError();
	~CheckTelTPError();

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
	 * @brief	���O�o��
	 * @param	emStatus	[in]	�`�F�b�N���ʃX�e�[�^�X
	 * @param	cRec		[in]	���R�[�h
	 */
	void outputLog( EResultStat emStatus, CLRec &cRec );

private:

	CWorkspace			m_cWorkspace;			//!< ���[�N�X�y�[�X

	bool				m_bAddMode;				//!< �ǉ����[�h���ۂ�(�e���R�[�h���Ƃ̃��[�h)

	std::ofstream		m_ofs;					//!< ���O�o�̓X�g���[��

	CFeatureClass		m_cTelTPPointFC;		//_< TelTPPoint�t�B�[�`���N���X

	CListHandle			m_cListHandle;			//!< ���X�g�n���h���N���X

	CAreaCodeHelper		m_cAcHelper;			//!< �Z��DB�w���p�[�N���X	

};

#endif // !defined(AFX_CHECKGPERROR_H__E00C35B4_4E7E_43DE_831F_D76F94982A69__INCLUDED_)
