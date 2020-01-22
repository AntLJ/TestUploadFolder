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

// CheckExTPPoint.h: CCheckExTPPoint �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_)
#define AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "ErrorHandle.h"

/**
 * @enum	emCheckMode
 * @brief	�`�F�b�N���[�h
 */
enum emCheckMode
{
	kNormal,	//!< �ʏ�`�F�b�N���[�h
	kAlps,		//!< �A���v�X�`�F�b�N���[�h
	kAcc4,		//!< iPC(���x4)�`�F�b�N���[�h[Bug6554]
	kiPS,		//!< iPS�`�F�b�N���[�h[Bug6320]
	kHlp,		//!< HLP�`�F�b�N���[�h[Bug8624]
};

/**
 * @class	CCheckExTPPoint
 */
class CCheckExTPPoint
{
public:
	CCheckExTPPoint();
	~CCheckExTPPoint();

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

private:

	/**
	 * @brief	���ϐ��擾���A�K�v���擾
	 * @return	bool
	 */
	bool setInfoFromDB();

	/**
	 * @brief	���b�V�����X�g�ǂݍ���
	 * @param	lpszFile	[in]	���b�V�����X�g
	 * @return	bool
	 */
	bool loadMeshlist( LPCTSTR lpszFile );

	/**
	 * @brief	���b�V���t�B�[�`���擾
	 * @param	lMeshCode	[in]	���b�V���R�[�h
	 * @return	���b�V���t�B�[�`��
	 */
	IFeaturePtr getMeshFeature( long lMeshCode );

	/**
	 * @brief	�`�F�b�N���C��
	 */
	void checkMain();

	/**
	 * @brief	���b�V�����Ƃ̃`�F�b�N
	 */
	void checkMainByMesh();

	/**
	 * @brief	�s�s�n�}�s���E�Ƃ̐����`�F�b�N
	 * @param	cExTP	[in]	ExTPPoint
	 */
	void checkExTPAndAdmin( const CExTP& cExTP );

	/**
	 * @brief	�s�s�n�}�s���E�Ƃ̐����`�F�b�N(�A���v�X�p)
	 * @param	cExTP	[in]	ExTPPoint
	 */
	void checkAlpsAndAdmin( const CExTP& cExTP );

	/**
	 * @brief	�w�i�Ƃ̐����`�F�b�N
	 * @note	�w�i�Ƃ̐����̓A���v�X�`�F�b�N���[�h�����p
	 * @param	cExTP	[in]	EXTPPoint
	 */
	void checkExTPAndSite( const CExTP& cExTP );

	/**
	 * @brief	�|�C���g������Ă�|���S�����擾
	 * @note	���E��ɏ���Ă���Ƃ��́A�Ԃ��Ȃ�
	 * @param	ipGeo		[in]	�W�I���g��(�|�C���g)
	 * @param	lpCount		[out]	�擾�|���S����	
	 * @param	szSwitch	[in]	A �s���E C �s�s�n�}�w�i B  ���k�w�i M �s�s�n�}���b�V��
	 * @return	IFeaturePtr
	 */
	IFeaturePtr getFeatureIntersectsPoint( IGeometry* ipGeo, LONG* lpCount, char szSwitch );

	/**
	 * @brief	�w���ʂ��A�m�f�s�s�n�}�w�i��ʁH
	 * @param	lDomainCode	[in]	�h���C���l
	 * @return	bool
	 */
	bool isNGCitySite( long lDomainCode )
	{
		if( m_CBGClass_NG.find(lDomainCode) != m_CBGClass_NG.end() )
			return true;
		return false;
	}

	/**
	 * @brief	�w���ʂ��A�m�f���k�w�i��ʁH
	 * @param	lDomainCode	[in]	�h���C���l
	 * @return	bool
	 */
	bool isNGBaseSite( long lDomainCode )
	{
		if( m_BBGClass_NG.find(lDomainCode) != m_BBGClass_NG.end() )
			return true;
		return false;
	}

	/**
	 * @brief	����20���Z���R�[�h�����|�C���g���������݂��Ȃ����`�F�b�N�iBug9296 �ǉ��j
	 * @note	�������ʃR�[�h���y�������z�y�ʒu�s���z�͏��O
	 * @param	cExTP	[in]	EXTPPoint
	 **/
	void checkSame20Addrcode( const CExTP& cExTP );

private:

	emCheckMode		m_emCheckMode;			//!< �`�F�b�N���[�h(�ʏ� or �A���v�X or ��C)
	CString			m_strWhereClause;		//!< ���o����Where��(alps�����̃`�F�b�N��)

	CErrorHandle	m_cError;				//!< �G���[�n���h���N���X
	IWorkspacePtr	m_ipTPWorkspace;		//!< EXTP���[�N�X�y�[�X
	
	std::vector<long>	m_vecMesh;			//!< ���b�V�����X�g

	std::vector<CExTP>	m_vecExTP;			//!< ExTP�|�C���g�i�[�p�x�N�^(�X�i�b�v�V���b�g�΍�)

	CString			 m_strExTPFCName;		//!< �g���d�b�����|�C���g�t�B�[�`���N���X��

	IFeatureClassPtr m_ipExTPFC;			//!< �g���d�b�����|�C���g�t�B�[�`���N���X
	IFeatureClassPtr m_ipCityAdminFC;		//!< �s�s�n�}�s���E�t�B�[�`���N���X
	IFeatureClassPtr m_ipCitySiteFC;		//!< �s�s�n�}�w�i�t�B�[�`���N���X
	IFeatureClassPtr m_ipBaseSiteFC;		//!< ���k�w�i�t�B�[�`���N���X
	IFeatureClassPtr m_ipCityMeshFC;		//!< �s�s�n�}���b�V���t�B�[�`���N���X[Bug 5622]
	IFeatureClassPtr m_ipTargetMeshFC;		//!< ���b�V���t�B�[�`���N���X(�`�F�b�N�G���A�i�荞�ݗp)

	long	m_lCBGClassC_FID;				//!< �s�s�n�}�w�i��ʃt�B�[���h�C���f�b�N�X
	long	m_lBSC4BGClassC_FID;			//!< ���k�w�iS4��ʃt�B�[���h�C���f�b�N�X
	long	m_lCreateYear_FID;				//!< �s�s�n�}���b�V���쐬�N�x�t�B�[���h�C���f�b�N�X

	static const std::set<long> m_CBGClass_NG;		//!< �g���d�b�ԍ������|�C���g�����݂��Ă͂��߂ȓs�s�n�}�w�i���
	static const std::set<long> m_BBGClass_NG;		//!< �g���d�b�ԍ������|�C���g�����݂��Ă͂��߂Ȓ��k�w�i���
};

#endif // !defined(AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_)
