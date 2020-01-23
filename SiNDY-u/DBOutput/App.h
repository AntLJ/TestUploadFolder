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

#include "InitFile.h"

/**
 * @enum	emQueryMode
 * @brief	�ΏۃI�u�W�F�N�g�i���݃��[�h
 */
enum emQueryMode
{
	kNone,		//!< �i���݂Ȃ�
	kWhere,		//!< ����
	kOID,		//!< OID���X�g
	kMesh,		//!< ���b�V�����X�g
	kMeshWhere,	//!< ���b�V�����X�g+����
};

/**
 * @class	CApp
 */
class CApp
{
public:
	CApp(void) : m_emQueryMode(kNone), m_bLabelPoint(false), m_bSkip(false){}
	~CApp(void){}

	/**
	 * @brief	�g�p�@
	 */
	static void printUsage();

	/**
	 * @brief	������
	 * @param	argc	[in]	�R�}���h���C�������̐�
	 * @param	argv	[in]	�R�}���h���C������
	 * @retval	true	��������
	 * @retval	false	�������s
	 */
	bool init( int argc, TCHAR* argv[] );

	/**
	 * @brief	���s
	 * @retval	true	��������
	 * @retval	false	�������s
	 */
	bool execute();

private:

	/**
	 * @brief	OID�E���b�V�����X�g�ǂݍ���(���p)
	 * @param	lpcszFile	[in]		���X�g�t�@�C��
	 * @param	rList		[in,out]	���X�g
	 * @retval	true	��������
	 * @retval	false	�������s
	 */
	bool loadList( LPCTSTR lpcszFile, std::list<long>& rList );

	/**
	 * @brief	�w�胁�b�V���R�[�h�̃��b�V���`��擾
	 * @param	lMeshCode	[in]	���b�V���R�[�h
	 * @return	���b�V����`
	 */
	IGeometryPtr GetMeshShape( long lMeshCode );

	/**
	 * @brief	�o��
	 * @param	ipRow			[in]	IRow
	 * @param	strPostErrMsg	[in]	�G���[���b�Z�[�W�̍Ō�ɏo�͂��镶����
	 * @retval	true	��������
	 * @retval	false	�������s
	 */
	bool output( _IRow* ipRow, const CString& strPostErrMsg );

private:

	CInitFile				m_cInit;			//!< �ݒ���	

	IWorkspacePtr			m_ipWorkspace;		//!< ���[�N�X�y�[�X
	ITablePtr				m_ipTable;			//!< �o�͑Ώۃe�[�u��
	IFeatureClassPtr		m_ipMeshFC;			//!< ���b�V���t�B�[�`���N���X

	emQueryMode				m_emQueryMode;		//!< �I�u�W�F�N�g�i���݃��[�h
	CString					m_strWhereClause;	//!< Where��
	CString					m_strOIDList;		//!< OID���X�g
	CString					m_strMeshList;		//!< ���b�V�����X�g
	bool					m_bLabelPoint;		//!< �|���S���ɑ΂�����W�l���A�d�S�ł͂Ȃ����x���|�C���g�Ŏ擾���邩�ǂ���
	bool					m_bSkip;			//!< �w�肵�����ڂ��e�[�u���ɖ����ꍇ�G���[�Ƃ��Ȃ�(�X�L�b�v���ďo�͂���)
};
