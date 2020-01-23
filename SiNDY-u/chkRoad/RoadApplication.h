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

/**
 * @brief IN�f�[�^�p�X
 */
struct INPath
{
	CString eMntDir;		///< .mnt�f�[�^�i�[�f�B���N�g���p�X
	CString ePolDir;		///< pol_code.txt�i�[�f�B���N�g���p�X
	CString eLatestDir;		///< latest��.mnt�f�[�^�i�[�f�B���N�g���p�X
};

/**
 * @brief �f�[�^�`�F�b�N���s�N���X
 */
class CRoadApplication
{
public:
	CRoadApplication(void);
	~CRoadApplication(void);

	/**
	 * @brief IN�f�[�^���`�F�b�N
	 * @note  �w��IN�f�[�^���`�F�b�N����B
	 *
	 * @param	rConfig			[in]	Config�t�@�C���ւ̃p�X
	 * @param	lpcszCustomer	[in]	�d������
	 * @param	bLatest			[in]	latest�̎w�肪���邩�iB�ňȍ~�̃`�F�b�N���ǂ����j
	 *
	 * @return	0:����I�� 1:�ُ�I�� 2:�ُ�I��(��O����) 3:�`�F�b�N�ɃG���[����
	 */
	long run( CString& rConfig, LPCTSTR lpcszCustomer, bool bLatest );

private:

	/**
	 * @brief Config�t�@�C���̐ݒ��ǂݍ���
	 * @note  �w�肵��Config�t�@�C���̐ݒ��ǂݍ��ށB
	 *
	 * @param	rConfig		[in]	Config�t�@�C���ւ̃p�X
	 *
	 * @return	�Ȃ�
	 */
	void GetConf( CString& rConfig );

	/**
	 * @brief	�d������w��
	 * @param	lpcszCustomer		[in]	�d������
	 * @return	Customer
	 */
	Customer CRoadApplication::GetCustomer( LPCTSTR lpcszCustomer );

	/**
	 * @brief .mnt�t�@�C�������݂��郁�b�V�����X�g���쐬����B
	 * @note  �w�肵��mnt�i�[Dir�ȉ��ɑ��݂���.mnt�t�@�C���̃��b�V�����X�g���쐬����B
	 * @param	lpcszMntDir	[in]	�����f�B���N�g��
	 *
	 * @return	mnt�t�@�C�������郁�b�V���̃��X�g
	 */
	std::set<int> CreateList( LPCTSTR lpcszMntDir );

	/**
	 * @brief .mnt�t�@�C�������݂��邩����������B
	 * @note  �w��K�w�ȉ���.mnt�t�@�C�������݂��邩����������B
	 *
	 * @param	hFind		[in]	�w��t�@�C���n���h��
	 * @param	fd			[in]	�����p�t�@�C���V�X�e��
	 * @param	lpPath		[in]	�w��K�w��΃p�X
	 * @param	setMesh		[in]	mnt�t�@�C�������郁�b�V���̃��X�g
	 *
	 * @return	�Ȃ�
	 */
	void FindFile(HANDLE hFind, WIN32_FIND_DATA &fd, LPCTSTR lpPath, std::set<int>& setMesh );

	INPath		m_INPathData;			///< �w��IN�f�[�^�i�[�f�B���N�g��
	set<int>	m_setDataMeshlist;		///< 2�����b�V�����X�g
	set<int>	m_setLatestMeshlist;	///< latest2�����b�V�����X�g
};
