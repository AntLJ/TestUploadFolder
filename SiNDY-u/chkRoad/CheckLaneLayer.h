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
#include "ZLaneLoad.h"

/**
 * @brief ���[�����G���[�f�[�^
 */
struct ErrorLane
{
	vector<CString>	eID;
	int				eMesh;
	MError			eError;
};

/**
 * @brief ���[����񃌃C���`�F�b�N�N���X
 */
class CCheckLaneLayer
{
public:
	CCheckLaneLayer(void);
	~CCheckLaneLayer(void);

	/**
	 * @brief IN�f�[�^�̃��[����񃌃C�����`�F�b�N����B
	 * @note  �w�肵��2�����b�V���̃��[�������`�F�b�N����B
	 *
	 * @param	rFilePath	[in]	���HIN�f�[�^�i�[�f�B���N�g��
	 * @param	rMeshCode	[in]	�w��2�����b�V���R�[�h
	 * @param	rLaneAttr	[in]	���[������l
	 *
	 * @return	ture		����I��
	 * @return	false		�ُ�I��
	 */
	bool check( CString &rFilePath, int rMeshCode, LaneAttrList& rLaneAttr );

	bool IsError() { return m_bIsError; };
private:
	/**
	 * @brief ���[����񃌃C���̃G���[�����o�͂���B(�R�[�h�l:16�i��)
	 * @note  �w�肵��2�����b�V���̃��[�������`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[���[�����
	 * @param	rCode		[in]	�G���[�����l
	 * @param	bError		[in]	�G���[�t���O
	 *
	 * @return	�Ȃ�
	 */
	void	PrintHexError( int rKey, ErrorLane& rError, int rCode, bool bError );

	/**
	 * @brief ���[����񃌃C���̃G���[�����o�͂���B(�R�[�h�l:10�i��)
	 * @note  �w�肵��2�����b�V���̃��[�������`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[���[�����
	 * @param	rCode		[in]	�G���[�����l
	 * @param	bError		[in]	�G���[�t���O
	 *
	 * @return	�Ȃ�
	 */
	void	PrintError( int rKey, ErrorLane& rError, int rCode, bool bError);

	/**
	 * @brief ���[����񃌃C���̃G���[�����o�͂���B
	 * @note  �w�肵��2�����b�V���̃��[�������`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[���[�����
	 * @param	bError		[in]	�G���[�t���O
	 *
	 * @return	�Ȃ�
	 */
	void	PrintError( int rKey, ErrorLane& rError, bool bError);

	/**
	 * @brief �Ώۃm�[�hID����G���[�f�[�^�Ɋi�[����B
	 * @note  ���[�������\������m�[�hID�Q�����[���G���[���Ɋi�[����B
	 *
	 * @param	pLane		[in]	���[�����
	 * @param	rError		[in]	�G���[���[�����
	 *
	 * @return	�Ȃ�
	 */
	void	SetID( INLane* pLane, ErrorLane& rError);

private:
	bool m_bIsError;
};
