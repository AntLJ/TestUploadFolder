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
 * @brief �����N���G���[�f�[�^
 */
struct ErrorLInfo
{
	CString			eFrom;			///< �����N�n�_ID
	CString			eTo;			///< �����N�I�_ID
	int				eMesh;			///< 2�����b�V���R�[�h
	MError			eError;			///< �G���[���
};

/**
 * @brief �����N��񃌃C���`�F�b�N�N���X
 */
class CCheckLinkInfoLayer
{
public:
	CCheckLinkInfoLayer(void);
	~CCheckLinkInfoLayer(void);

	/**
	 * @brief IN�f�[�^�̃����N��񃌃C�����`�F�b�N����B
	 * @note  �w�肵��2�����b�V���̃����N�����`�F�b�N����B
	 *
	 * @param	rFilePath	[in]	���HIN�f�[�^�i�[�f�B���N�g��
	 * @param	rMeshCode	[in]	�w��2�����b�V���R�[�h
	 * @param	rLInfoAttr	[in]	�����N����l
	 *
	 * @return	ture		����I��
	 * @return	false		�ُ�I��
	 */
	bool check( CString &rFilePath, int rMeshCode, LInfoAttrList& rLInfoAttr );

private:

	/**
	 * @brief �����N��񃌃C���̃G���[�����o�͂���B(�R�[�h�l:16�i��)
	 * @note  �w�肵��2�����b�V���̃����N�����`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�����N���
	 * @param	rCode		[in]	�G���[�����l
	 * @param	bError		[in]	�G���[�t���O
	 *
	 * @return	�Ȃ�
	 */
	void	PrintHexError( int rKey, ErrorLInfo& rError, int rCode, bool bError);

	/**
	 * @brief �����N��񃌃C���̃G���[�����o�͂���B(�R�[�h�l:10�i��)
	 * @note  �w�肵��2�����b�V���̃����N�����`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�����N���
	 * @param	rCode		[in]	�G���[�����l
	 * @param	bError		[in]	�G���[�t���O
	 *
	 * @return	�Ȃ�
	 */
	void	PrintError( int rKey, ErrorLInfo& rError, int rCode, bool bError);

	/**
	 * @brief �����N��񃌃C���̃G���[�����o�͂���B
	 * @note  �w�肵��2�����b�V���̃����N�����`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�����N���
	 * @param	bError		[in]	�G���[�t���O
	 *
	 * @return	�Ȃ�
	 */
	void	PrintError( int rKey, ErrorLInfo& rError, bool bError);
};
