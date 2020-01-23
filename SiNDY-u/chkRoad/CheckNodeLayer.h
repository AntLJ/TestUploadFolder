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
 * @brief ���H�m�[�h�G���[�f�[�^
 */
struct ErrorNode
{
	CString		eID;			///< �m�[�hID
	int			eMesh;			///< �Q�����b�V��
	MError		eError;			///< �G���[���
};

/**
 * @brief ���H�m�[�h���C���`�F�b�N�N���X
 */
class CCheckNodeLayer
{
public:
	CCheckNodeLayer(void);
	~CCheckNodeLayer(void);

	/**
	 * @brief IN�f�[�^�̓��H�m�[�h���C�����`�F�b�N����B
	 * @note  �w�肵��2�����b�V���̓��H�m�[�h�����`�F�b�N����B
	 *
	 * @param	rFilePath	[in]	���HIN�f�[�^�i�[�f�B���N�g��
	 * @param	rMeshCode	[in]	�w��2�����b�V���R�[�h
	 * @param	rNodeAttr	[in]	�m�[�h��l
	 *
	 * @return	ture		����I��
	 * @return	false		�ُ�I��
	 */
	bool check( CString &rFilePath, int rMeshCode, NodeAttrList& rNodeAttr );

	bool IsError() { return m_bIsError; };
private:

	/**
	 * @brief �m�[�h���C���̃G���[�����o�͂���B(�R�[�h�l:16�i��)
	 * @note  �w�肵��2�����b�V���̓��H�m�[�h�����`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�m�[�h���
	 * @param	rCode		[in]	�G���[�����l
	 * @param	bError		[in]	�G���[�t���O
	 *
	 * @return	�Ȃ�
	 */
	void PrintHexError( int rKey, ErrorNode& rError, int rCode, bool bError); 

	/**
	 * @brief �m�[�h���C���̃G���[�����o�͂���B(�R�[�h�l:10�i��)
	 * @note  �w�肵��2�����b�V���̓��H�m�[�h�����`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�m�[�h���
	 * @param	rCode		[in]	�G���[�����l
	 * @param	bError		[in]	�G���[�t���O
	 *
	 * @return	�Ȃ�
	 */
	void PrintError( int rKey, ErrorNode& rError, int rCode, bool bError); 

	/**
	 * @brief �m�[�h���C���̃G���[�����o�͂���B
	 * @note  �w�肵��2�����b�V���̓��H�m�[�h�����`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�m�[�h���
	 * @param	bError		[in]	�G���[�t���O
	 *
	 * @return	�Ȃ�
	 */
	void PrintError( int rKey, ErrorNode& rError, bool bError); 

private:
	bool m_bIsError;
};
