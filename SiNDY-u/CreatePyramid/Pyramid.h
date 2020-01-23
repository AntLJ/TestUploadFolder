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

class CPyramid
{
public:
	/**
     * @brief  �R���X�g���N�^
     */	
	CPyramid(void);

	/**
     * @brief  �f�X�g���N�^
     */	
	virtual ~CPyramid(void){}

	/**
     * @brief   ���b�V�����X�g�w��ŏ�������
     * @param   bOption [in]  �W�����͂Ŏw�肷�邩
     */	
	void TargetMeshList( bool bOption );

	/**
     * @brief   �f�B���N�g���w��ŏ�������
     * @param   bOption [in]  �W�����͂Ŏw�肷�邩
     */	
	void TargetPathList( bool bOption );

	/**
     * @brief   �s���~�b�h�t�@�C�����쐬����
	 * @note    �r���~�b�h�t�@�C���ɂ��Ă͉��L�̃y�[�W���Q�ƁB
	            http://desktop.arcgis.com/ja/arcmap/10.3/manage-data/raster-and-images/raster-pyramids.htm
				http://desktop.arcgis.com/ja/arcmap/10.3/manage-data/raster-and-images/ovr-pyramid-files.htm
     * @param   ipRasterWS [in]  ���X�^���[�N�X�y�[�X
     * @param   szFildFile [in]  �����Ώۃ��X�^�t�@�C����
     * @param   szMesh     [in]  ���b�V���R�[�h�i�\���p�j
     * @param   i          [in]  �����ԍ��i�\���p�j
     */	
	void CreatePyramidFile( IRasterWorkspacePtr ipRasterWS, LPCSTR szFildFile, LPCSTR szMesh, int i );

	/**
     * @brief   �J�n����(�������[�h�󂯕t��)
     * @param   bOption [in]  �W�����͂Ŏw�肷�邩
	 * @return  �������[�h
	 */	
	int StartFunction( bool bOption );

	/**
     * @brief   �I������
	 * @return  ���̏����t���O 
     */	
	int EndFunction();

	/**
     * @brief   �I�v�V��������͂��Đݒ肷��
     * @param   listOption [in]  �R�}���h���C���������X�g
	 * @retval  true   �W�����͂Ŏw�肷��
	 * @retval  false  �W�����͂Ŏw�肵�Ȃ��i�R�}���h���C�������Ŏw��j
	 */	
	bool InitOption( std::list<CString> listOption );

	CString m_strIniPath;		//!< ini�t�@�C���̃t���p�X
	CString	m_strCurrent;		//!< �J�����g�f�B���N�g���i���s�t�@�C���̃f�B���N�g���j

private:
	bool		m_bBatch;		//!< �W�����͂��ǂ���
	long		m_lDir;			//!< �Ǎ��݃f�B���N�g���ύX(0:./4��/8��or6��.jpg 1:./8��or6��.jpg 2:./6��/10��.jpg)
	long		m_lExtension;	//!< �g���q(0:jpg 1:png 2:bmp)
	long		m_lMode;		//!< �������[�h(1:���b�V�����X�g 2:�f�B���N�g��)
	CString		m_strMeshlist;	//!< ���b�V�����X�g�t�@�C���̃t���p�X
	CString		m_strRasterDir;	//!< ���X�^�t�@�C�����i�[����Ă���f�B���N�g��

};
