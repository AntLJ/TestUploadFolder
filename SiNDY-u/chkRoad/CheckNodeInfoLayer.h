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
#include "ZNInfoLoad.h"

/**
 * @brief �m�[�h���G���[�f�[�^
 */
struct ErrorNInfo
{
	vector<CString>	eID;			///< �Ώ�ID�Q
	int				eMesh;			///< 2�����b�V���R�[�h
	MError			eError;			///< �G���[���
};

/**
 * @brief �m�[�h��񃌃C���`�F�b�N�N���X
 */
class CCheckNodeInfoLayer
{
public:
	CCheckNodeInfoLayer(void);
	~CCheckNodeInfoLayer(void);

	/**
	 * @brief IN�f�[�^�̃m�[�h��񃌃C�����`�F�b�N����B
	 * @note  �w�肵��2�����b�V���̃m�[�h�����`�F�b�N����B
	 *
	 * @param	rFilePath	[in]	���HIN�f�[�^�i�[�f�B���N�g��
	 * @param	rMeshCode	[in]	�w��2�����b�V���R�[�h
	 * @param	rNInfoAttr	[in]	�m�[�h����l
	 *
	 * @return	ture		����I��
	 * @return	false		�ُ�I��
	 */
	bool check( CString &rFilePath, int rMeshCode, NInfoAttrList& rNInfoAttr );

	bool IsError() { return m_bIsError; };
private:

	/**
	 * @brief �Ώۃm�[�hID����G���[�f�[�^�Ɋi�[����B
	 * @note  �m�[�h�����\������m�[�hID�Q���m�[�h���G���[�Ɋi�[����B
	 *
	 * @param	pNInfo		[in]	�m�[�h���
	 * @param	rError		[in]	�m�[�h���G���[
	 *
	 * @return	�Ȃ�
	 */
	void	SetID( INNInfo* pNInfo, ErrorNInfo& rError);

	/**
	 * @brief �m�[�h��ɏd�����Ȃ������`�F�b�N����B
	 * @note  �m�[�h�����\������m�[�hID�Q�ɏd�����Ȃ������`�F�b�N����B
	 *
	 * @param	pNInfo		[in]	�m�[�h���
	 * @param	rError		[in]	�m�[�h���G���[
	 *
	 * @return	�Ȃ�
	 */
	bool	CheckNode( INNInfo* pNInfo, ErrorNInfo& rError);

	/**
	 * @brief �m�[�h��񃌃C���̃G���[�����o�͂���B(�R�[�h�l:������)
	 * @note  �w�肵��2�����b�V���̃m�[�h�����`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�����N���
	 * @param	rCode		[in]	�G���[�����l
	 * @param	bError		[in]	�G���[�t���O
	 *
	 * @return	�Ȃ�
	 */
	void	PrintStringError( int rKey, ErrorNInfo& rError, CString& rCode, bool bError);

	/**
	 * @brief �m�[�h��񃌃C���̃G���[�����o�͂���B(�R�[�h�l:16�i��)
	 * @note  �w�肵��2�����b�V���̃m�[�h�����`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�m�[�h���
	 * @param	rCode		[in]	�G���[�����l
	 * @param	bError		[in]	�G���[�t���O
	 *
	 * @return	�Ȃ�
	 */
	void	PrintHexError( int rKey, ErrorNInfo& rError, int rCode, bool bError);

	/**
	 * @brief �m�[�h��񃌃C���̃G���[�����o�͂���B(�R�[�h�l:10�i��)
	 * @note  �w�肵��2�����b�V���̃m�[�h�����`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�����N���
	 * @param	rCode		[in]	�G���[�����l
	 * @param	bError		[in]	�G���[�t���O
	 *
	 * @return	�Ȃ�
	 */
	void	PrintError( int rKey, ErrorNInfo& rError, int rCode, bool bError);

	/**
	 * @brief �m�[�h��񃌃C���̃G���[�����o�͂���B
	 * @note  �w�肵��2�����b�V���̃m�[�h�����`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�����N���
	 * @param	bError		[in]	�G���[�t���O
	 *
	 * @return	�Ȃ�
	 */
	void	PrintError( int rKey, ErrorNInfo& rError, bool bError);

private:
	bool m_bIsError;
};
