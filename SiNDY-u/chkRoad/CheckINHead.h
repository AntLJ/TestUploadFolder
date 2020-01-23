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

#include "SetRoadAttrList.h"

/**
 * @brief IN�w�b�_���G���[�f�[�^
 */
struct ErrorHead
{
	CString		eFileName;		///< �t�@�C������
	int			eMesh;			///< 2�����b�V���R�[�h
	MError		eError;			///< �G���[���
};

/**
 * @brief IN�w�b�_�`�F�b�N�N���X
 */
class CCheckINHead
{
public:
	CCheckINHead(void);
	~CCheckINHead(void);

	/**
	 * @brief IN�f�[�^�̃w�b�_�����`�F�b�N����B
	 * @note  �w�肵��2�����b�V����IN�f�[�^�w�b�_�����`�F�b�N����B
	 *
	 * @param	rMntDir		[in]	���HIN�f�[�^�i�[�f�B���N�g��
	 * @param	iMeshCode	[in]	�w��2�����b�V���R�[�h
	 * @param	rHeadAttr	[in]	IN�w�b�_�G���[���
	 *
	 * @return	ture		����I��
	 * @return	false		�ُ�I��
	 */
	bool check( CString& rMntDir, int iMeshCode, HeadAttrList& rHeadAttr);

	bool IsError() { return m_bIsError; };
private:
	/**
	 * @brief �w��IN�f�[�^�t�@�C���̃w�b�_�����`�F�b�N����B
	 * @note  �w�肵��IN�f�[�^�w�b�_�����`�F�b�N����B
	 *
	 * @param	rFilePath		[in]	�w��IN�f�[�^�t�@�C��
	 * @param	rHeadAttr		[in]	IN�w�b�_�G���[���
	 * @param	rMeshCode		[in]	2�����b�V���R�[�h
	 *
	 * @return	ture		����I��
	 * @return	false		�ُ�I��
	 */
	bool checkFile( CString& rFilePath, HeadAttrList& rHeadAttr, int rMeshCode);

	/**
	 * @brief �w��IN�f�[�^�t�@�C���̃w�b�_�����`�F�b�N����B
	 * @note  �w�肵��IN�f�[�^�w�b�_�����`�F�b�N����B
	 *
	 * @param	rWord			[in]	���̓o�C�i���R�[�h
	 * @param	bBcd			[in]	BCD�R�[�h�ɂ��邩�̃t���O
	 *
	 * @return	�����R�[�h
	 */
	int ReverseWord(char* rWord, bool bBcd);

	/**
	 * @brief IN�w�b�_�̃G���[�����o�͂���B(�R�[�h�l:10�i��)
	 * @note  �w�肵��2�����b�V����IN�w�b�_�����`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�����_���̏��
	 * @param	rCode		[in]	�G���[�l
	 *
	 * @return	�Ȃ�
	 */
	void PrintError(int rKey, ErrorHead& rError, int rCode);

	/**
	 * @brief IN�w�b�_�̃G���[�����o�͂���B(�R�[�h�l:������)
	 * @note  �w�肵��2�����b�V����IN�w�b�_�����`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�����_���̏��
	 * @param	rWord		[in]	�G���[�l
	 *
	 * @return	�Ȃ�
	 */
	void PrintStringError(int rKey, ErrorHead& rError, CString& rWord);

private:
	bool m_bIsError;		// �G���[�����݂������ǂ���
};
