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

#include "MakeAddrRec.h"

/**
 * @enum	EListMode
 * @brief	���X�g���[�h(�ǂ��������`�Ŏs�撬���R�[�h���X�g����邩)
 */
enum EListMode
{
	kAll,		//!< �S��
	kRange,		//!< �͈͎w��
	kList,		//!< ���X�g�w��
	kSingle,	//!< �P��
};

/**
 * @class	CApp
 * @brief	�A�v���P�[�V�����N���X
 */
class CApp
{
public:
	CApp(void);
	~CApp(void);

	/**
	 * @brief	�g�p�@
	 */
	static void PrintUsage();

	/**
	 * @brief	������
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool Init( int argc, _TCHAR* argv[] );

	/**
	 * @brief	���s
	 */
	void Run();

private:

	/**
	 * @brief	�ݒ�t�@�C���ǂݍ���
	 * @param	lpcszFile	[in]	�ݒ�t�@�C��
	 * @return	bool
	 */
	bool LoadInitFile( LPCTSTR lpcszFile );

	/**
	 * @brief	�s�撬���R�[�h���X�g�쐬
	 * @return	bool
	 */
	bool MakeCityCodeList();

	/**
	 * @brief	�f�[�^�����o��
	 * @param	lpcszCityCode	[in]	�s�撬���R�[�h
	 * @param	rPntList		[in]	PNT���R�[�h���X�g
	 * @param	rGouList		[in]	GOU���R�[�h���X�g
	 */
	bool WriteData( LPCTSTR lpcszCityCode, const PNTMAP& rPntList, const GOUMAP& rGouList );

private:

	EListMode			m_emListMode;		//!< ���X�g���[�h
	CString				m_strListInfo;		//!< ���X�g���[�h�ɉ������⑫���
	std::set<CString>	m_setCode;			//!< �s�撬���R�[�h���X�g

	CParameter			m_cParam;			//!< �ݒ���

	IWorkspacePtr		m_ipAddrWorkspace;	//!< �Z���n���[�N�X�y�[�X
	IWorkspacePtr		m_ipMapWorkspace;	//!< �n�}�n���[�N�X�y�[�X

	IFeatureClassPtr	m_ipCityAdminFC;	//!< �s�s�n�}�s���E�t�B�[�`���N���X
	IFeatureClassPtr	m_ipGouPointFC;		//!< ���|�C���g�t�B�[�`���N���X

	CFieldMap			m_cAdminFID;		//!< �s�s�n�}�s���E�t�B�[���h�C���f�b�N�X���X�g
	CFieldMap			m_cGouFID;			//!< ���|�C���g�t�B�[���h�C���f�b�N�X���X�g

	CMakeAddrRec		m_cMakeAddrRec;		//!< �Z�����R�[�h�����N���X
};
