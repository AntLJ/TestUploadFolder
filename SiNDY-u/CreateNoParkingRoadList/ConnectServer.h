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
 *  @brief �T�[�o�ڑ��ݒ�
 */
struct ConfData
{
	CString eSDEServer;			//!< �ڑ�����SDE�T�[�o���
	CString eNoParkingPath;		//!< ���֎���H�����̃p�X
	CString eOutput;			//!< �o�̓��X�g�p�X
	CString eOutputShape;		//!< �o��shape�p�X
	CString eOutputLink;		//!< ���ԋ֎~�����Ԃ̃��C���̏o��shape�p�X
};

class CConnectServer
{
public:
	CConnectServer(void);
	~CConnectServer(void);

	/**
	 * @brief �w��T�[�o�ւ̐ڑ����s���B
	 * @note  �w�肵���T�[�o�ւ̐ڑ����s���B
	 *
	 * @param	rConf		[in]		�ݒ�t�@�C���p�X	
	 *
	 * @return true		����I��
	 * @return false	�ُ�I��
	 */
	bool run(CString& rConf);

private:

	/**
	 * @brief �ݒ�t�@�C����ǂݍ��ށB
	 * @note  �w�肵���ݒ�t�@�C����ǂݍ��ށB
	 *
	 * @param	rConf		[in]		�ݒ�t�@�C���p�X
	 * @param	rConfData	[out]		�ڑ��ݒ�	
	 *
	 * @return �Ȃ�
	 */
	void ReadConf(CString& rConf, ConfData& rConfData);

	/**
	 * @brief	shape�̓s���{���f�B���N�g�����擾����
	 * @param	lpcszNoParkingPath		[in]		�ݒ�t�@�C����NOPARKINGDATA
	 * @param	setPerfecturePath		[out]		�s���{���f�B���N�g�����X�g
	 * @retval	true	�擾����
	 * @retval	false	�擾���s
	 */
	bool GetPerfecturePath( LPCTSTR lpcszNoParkingPath, std::set<CString>& setPerfecturePath );

	/**
	 * @brief	IWorkspace�擾
	 * @param	lpcszDBPath				[in]		�擾����DB�ւ̃p�X
	 * @param	lpcszMessage			[in]		�W���G���[�o�͂ւ̃��b�Z�[�W�iXX���擾��...��XX�����j
	 * @return	IWorkspacePtr
	 */
	IWorkspacePtr GetWorkspace( LPCTSTR lpcszDBPath, LPCTSTR lpcszMessage );

	/**
	 * @brief	IFeatureClass�擾
	 * @param	ipWorkspace				[in]		�擾�������t�B�[�`���N���X�̃��[�N�X�y�[�X
	 * @param	lpcszFeatureClassName	[in]		�擾�������t�B�[�`���N���X�̖��O
	 * @return	IFeatureClassPtr
	 */
	IFeatureClassPtr GetFeatureClass( IWorkspace* ipWorkspace, LPCTSTR lpcszFeatureClassName );
};
