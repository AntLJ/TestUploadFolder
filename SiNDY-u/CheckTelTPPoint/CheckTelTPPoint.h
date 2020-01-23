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

//CheckTelTPPoint.h:	CCheckTelTPPoint�@�N���X�̃C���^�t�F�C�X


#if !defined(AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_)
#define AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>
#include "ErrorHandle.h"

class CCheckTelTPPoint
{
public:
	CCheckTelTPPoint( CEnviron& rEnv );
	virtual ~CCheckTelTPPoint();

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
	 * @brief	�������
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool checkArg( int argc, char* argv[] );

	/**
	 * @brief	���ϐ��擾���A�K�v���擾
	 * @return	bool
	 */
	bool setInfoFromSDE();

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
	 * @param	cTelTP	[in]	TelTPPoint
	 */
	void checkTelTPAndAdmin( const CTelTP& cTelTP );

	/**
	 * @brief	�w�i�Ƃ̐����`�F�b�N
	 * @param	cTelTP	[in]	TelTPPoint
	 */
	void checkTelTPAndSite( const CTelTP& cTelTP );

	/**
	 * @brief	����ܓx�o�x�̃|�C���g�̒������ʃR�[�h�`�F�b�N
	 * @brief	�d�b�ԍ��̃t���[�_�C�����`�F�b�N
	 * @param	cTelTP	[in]	TelTPPoint
	 */
	void checkTelTPPoints( const CTelTP& cTelTP );

	/**
	 * @brief	�|�C���g������Ă�|���S�����擾
	 * @note	���E��ɏ���Ă���Ƃ��́A�Ԃ��Ȃ�
	 * @param	ipGeo		[in]	�W�I���g��(�|�C���g)
	 * @param	lpCount		[out]	�擾�|���S����	
	 * @param	szSwitch	[in]	A �s���E C �s�s�n�}�w�i B  ���k�w�i
	 * @return	IFeaturePtr
	 */
	IFeaturePtr getFeatureWithinPoint( IGeometry* ipGeo, LONG* lpCount, char szSwitch );

	/**
	 * @brief	�w���ʂ��A�m�f�s�s�n�}�w�i��ʁH
	 * @param	lDomainCode	[in]	�h���C���l
	 * @return	bool
	 */
	bool isNGCitySite( LONG lDomainCode )
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
	bool isNGBaseSite( LONG lDomainCode )
	{
		if( m_BBGClass_NG.find(lDomainCode) != m_BBGClass_NG.end() )
			return true;

		return false;
	}

	/**
	 * @brief	TelTP�̎��͂�POI��IP�R�[�h&�d�b�ԍ��`�F�b�N(Bug 5701�Ή� 07/11/30�ǉ�)�@
	 * @param	cTelTP	[in]		TelTPPoint
	 */
	void CheckIPCODE( const CTelTP& cTelTP );

	/**
	 * @brief	����V�[�P���X�����|�C���g���������݂��Ȃ����`�F�b�N�iBug9295 �ǉ��j
	 * @note	�������ʃR�[�h���y�Y���{�݂���z���m�̂�
	 * @param	cTelTP	[in]	TelTPPoint
	 **/
	void CheckSameSequence( const CTelTP& cTelTP );


private:

	CErrorHandle	m_cError;				/// �G���[�n���h���N���X
	IWorkspacePtr	m_ipTPWorkspace;		/// TelTP���[�N�X�y�[�X
	
	IWorkspacePtr	m_ipPOIWorkspace;		/// POI���[�N�X�y�[�X(Bug 5701�Ή��@07/11/30�ǉ�)

	CEnviron&		m_rEnv;					/// ���ϐ�

	std::vector<long>	m_vecMesh;			/// ���b�V�����X�g

	std::vector<LONG>	m_vecTelTP;			/// TelTP�|�C���g�i�[�p�x�N�^(�X�i�b�v�V���b�g�΍�)


	IFeatureClassPtr m_ipTelTPFC;			/// �d�b�����|�C���g�t�B�[�`���N���X
	IFeatureClassPtr m_ipCityAdminFC;		/// �s�s�n�}�s���E�t�B�[�`���N���X
	IFeatureClassPtr m_ipCitySiteFC;		/// �s�s�n�}�w�i�t�B�[�`���N���X
	IFeatureClassPtr m_ipBaseSiteFC;		/// ���k�w�i�t�B�[�`���N���X
	IFeatureClassPtr m_ipMeshFC;			/// ���b�V���t�B�[�`���N���X
	IFeatureClassPtr m_ipCityMeshFC;		/// �s�s�n�}���b�V���t�B�[�`���N���X
	IFeatureClassPtr m_ipPOIFC;				/// POI�t�B�[�`���N���X(Bug 5701�Ή��@07/11/30�ǉ�)

	LONG	m_lCBGClassC_FID;				/// �s�s�n�}�w�i��ʃt�B�[���h�C���f�b�N�X
	LONG	m_lBSC4BGClassC_FID;			/// ���k�w�iS4��ʃt�B�[���h�C���f�b�N�X

	// �d�b�ԍ������|�C���g�����݂��Ă͂��߂ȓs�s�n�}�w�i���
	static const std::set<LONG> m_CBGClass_NG;

	// �d�b�ԍ������|�C���g�����݂��Ă͂��߂Ȓ��k�w�i���
	static const std::set<LONG> m_BBGClass_NG;

	double	m_dMeter;						/// 10�i�x�ɂ��T�����������a����������󂯎��(Bug 5701�Ή� 07/11/30�ǉ�)

};

#endif // !defined(AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_)




















