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

#include "Objects.h"
#include <WinLib/VersionInfo.h>

using namespace std;

typedef std::multimap<CString, CGouPoint> CODEMAP;	// �L�[��20���Z���R�[�h �l�����I�u�W�F�N�g

/**
 * @class	CGouRec
 */

class CApp
{
public:
	CApp(void);
	~CApp(void);

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
	bool init( int argc, TCHAR* argv[] );

	/**
	 * @brief	���s
	 * @return	bool
	 */
	bool execute();

private:

	/**
	 * @brief	���ϐ��擾
	 * @return	bool
	 */
	bool getEnv();

	/**
	 * @brief	���b�V�����X�g�ǂݍ���
	 * @param	lpszFile	[in]	�t�@�C����
	 * @return	bool
	 */
	bool loadMeshList( LPCTSTR lpszFile );

	/**
	 * @brief	OK�Z���R�[�h���X�g
	 * @param	lpszFile	[in]	�t�@�C����
	 * @return	bool
	 */
	bool loadOKAddrCodeList( LPCTSTR lpszFile );

	/**
	 * @brief	DB������擾
	 * @param	lMeshcode	[in]	���b�V���R�[�h
	 * @return	bool
	 */
	bool setFromDB( LONG lMeshcode );

	/**
	 * @brief	�t�B�[���h�h�c�Z�b�g
	 * @note	�P�x�؂肵������Ȃ�
	 * @return	bool
	 */
	bool setFieldID();

	/**
	 * @brief	���b�V���t�B�[�`���擾
	 * @param	lMeshcode	[in]	���b�V���R�[�h
	 * @return	���b�V���t�B�[�`��
	 */
	IFeaturePtr getMeshFeature( LONG lMeshcode );

	/**
	 * @brief	20���Z���R�[�h���X�g�쐬
	 * @param	ipAdminFC	[in]		�s�s�n�}�s���E�t�B�[�`���N���X
	 * @param	ipGouFC		[in]		���|�C���g�t�B�[�`���N���X
	 * @param	ipMeshGeo	[in]		���b�V���W�I���g��
	 * @param	lMesh		[in]		���b�V���ԍ�
	 * @param	ipWorkspace	[in]		���[�N�X�y�[�X
	 * @param	codeMap		[in, out]	�R�[�h���X�g�i�[
	 */
	void makeCodeList( IFeatureClass* ipAdminFC, IFeatureClass* ipGouFC, IGeometry* ipMeshGeo, long lMesh, IWorkspace* ipWorkspace, CODEMAP& codeMap );

	/** 
	 * @brief	20���Z���R�[�h����
	 * @param	cCityAdmin	[in]	�s�s�n�}�s���E
	 * @param	cGouPoint	[in]	���|�C���g�t�B�[�`��
	 * @return	�Z���R�[�h
	 */
	CString getAddrCode( const CCityAdmin& cCityAdmin, const CGouPoint& cGouPoint );

	/**
	 * @brief	�폜�Z���R�[�h����
	 * @param	NewCodeList	[in]	�V�R�[�h���X�g
	 * @param	OldCodeList	[in]	���R�[�h���X�g
	 */
	void findDeleteCode( const CODEMAP& NewCodeList, const CODEMAP& OldCodeList );

	/**
	 * @brief	���ԍ�����A�s�v�ȕ�������
	 * @param	strGou	[in]	���ԍ�
	 */
	void filterGouNo( CString& strGou );

	/**
	 * @brief	�w�b�_�[�o��
	 */
	void printHeader()
	{
		if( !m_bIsAddMode )
		{
			CVersion cVer;
			CString strMsg;
			strMsg.Format( _T("#SINDYSTDLOG\n")
						   _T("#%s FILEVERSION %s PRODUCTVERSION %s"),
						   cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
			m_fout << strMsg << std::endl;
		}
	}

	/**
	 * @brief	�폜���ꂽ�|�C���g�o��
	 * @param	cGouPoint	[in]	�폜���ꂽ���|�C���g
	 * @param	strAddrCode	[in]	�Z���R�[�h
	 */
	void printDeleteGou( const CGouPoint& cGouPoint, const CString& strAddrCode );

////////////////////////////////////
//	�����o�ϐ�
////////////////////////////////////

	std::ofstream						m_fout;				/// �o�̓��O�X�g���[��
	bool								m_bIsAddMode;		/// �ǋL���[�h���ۂ�

	bool								m_bGroupMeshMode;	//!< ���b�V�����X�g���O���[�v�Ƃ��Ă݂郂�[�h[Bug 5449]

	IWorkspacePtr						m_ipNewWorkspace;	//!< ���[�N�X�y�[�X(�V)
	IWorkspacePtr						m_ipOldWorkspace;	//!< ���[�N�X�y�[�X(��)

	LPTSTR								m_lpszNewDBProp;	/// DB�ڑ��v���p�e�B(�V)
	LPTSTR								m_lpszOldDBProp;	/// DB�ڑ��v���p�e�B(��)
	LPTSTR								m_lpszNewCAFName;	/// �s�s�n�}�s���E�e�[�u����(�V)
	LPTSTR								m_lpszOldCAFName;	/// �s�s�n�}�s���E�e�[�u����(��)
	LPTSTR								m_lpszNewGouFName;	/// ���|�C���g�e�[�u����(�V)
	LPTSTR								m_lpszOldGouFName;	/// ���|�C���g�e�[�u����(��)
	LPTSTR								m_lpszMeshFName;	/// �s�s�n�}���b�V���e�[�u����

	std::vector<LONG>					m_vecMesh;			/// ���b�V�����X�g
	std::vector<LONG>::const_iterator	m_itrMesh;			/// ���݂̃��b�V���R�[�h�̈ʒu

	std::set<CString>					m_setOKCode;		/// �n�j�P�P���Z���R�[�h���X�g(�Z���ύX�ӏ���)

	IFeatureClassPtr					m_ipNewCAFC;		/// �s�s�n�}�s���E�t�B�[�`���N���X(�V)
	IFeatureClassPtr					m_ipNewGouFC;		/// ���|�C���g�t�B�[�`���N���X(�V)
	IFeatureClassPtr					m_ipOldCAFC;		/// �s�s�n�}�s���E�t�B�[�`���N���X(��)
	IFeatureClassPtr					m_ipOldGouFC;		/// ���|�C���g�t�B�[�`���N���X(��)
	IFeatureClassPtr					m_ipMeshFC;			/// �s�s�n�}���b�V���t�B�[�`���N���X

	CCityAdminFID						m_cCityAdminFID;	/// �s�s�n�}�s���E�t�B�[���h�C���f�b�N�X
	CGouPointFID						m_cGouPointFID;		/// ���|�C���g�t�B�[���h�C���f�b�N�X
};

/**
 * @brief	��Ԍ��� (+ ��������)
 * @param	ipGeo			[in]	�Ώۂ̃W�I���g��
 * @param	ipFeatureClass	[in]	�������̃t�B�[�`���N���X
 * @param	ppFeatureCursor	[out]	�������ʂ̃J�[�\��
 * @param	spatialRel		[in]	��������
 * @param	lpszWhereClause	[in]	��������������Ȃ�w�肷��(�f�t�H���g��NULL)
 * @param	lpszSubFields	[in]	�K�v�ȃt�B�[���h(�J���}��؂�)
 * @return	�����Ɉ�v��������
 */
LONG SelectByShapeWithCount( IGeometryPtr ipGeo,
							 IFeatureClassPtr ipFeatureClass,
							 IFeatureCursor** ppFeatureCursor,
				 			 esriSpatialRelEnum spatialRel,
							 LPCTSTR lpszWhereClause = NULL,
							 LPCTSTR lpszSubFields = NULL );
