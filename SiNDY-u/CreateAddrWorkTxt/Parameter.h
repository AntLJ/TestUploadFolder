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

#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include <SiNDYLib/FeatureClass.h>

using namespace sindy;

/**
 * @struct	CParameter
 * @brief	���s���
 */
struct CParameter
{
	CFeatureClass	m_cPointFC;				//!< �Z���|�C���g�t�B�[�`���N���X
	CFeatureClass	m_cCityAdminFC;			//!< �s�s�n�}�s���E�t�B�[�`���N���X

	CString			m_strOutFile;			//!< �o�̓t�@�C��(CS�Ŏg�p)
	CString			m_strOutDir;			//!< �o�̓f�B���N�g��(KP,GP�Ŏg�p)

	std::pair<CString, CString> m_pairArea;	//!< ���s�G���A���(�I�v�V�����A���)

	CString			m_strJDBProp;			//!< �Z��DB�ڑ��v���p�e�B

	CParameter()
	{
		m_pairArea = std::make_pair<CString, CString>( _T("-ca"), NULL );	// �f�t�H���g�͑S��
	}

};

#endif __PARAMETER_H__
