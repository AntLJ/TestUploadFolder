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
#include "ZRegLoad.h"

/**
 * @brief �����N�K���G���[�f�[�^
 */
struct ErrorRegLink
{
	CString	eFrom;			///< �����N�n�_ID
	CString	eTo;			///< �����N�I�_ID
	int		eMesh;			///< 2�����b�V��
	MError	eError;			///< �G���[���
};

/**
 * @brief 3�_�m�[�h�K���G���[�f�[�^
 */
struct ErrorRegNode
{
	CString	eInID;			///< �����m�[�hID
	CString	eID;			///< �Ώۃm�[�hID
	CString	eOutID;			///< ���o�m�[�hID
	int		eMesh;			///< 2�����b�V��
	MError	eError;			///< �G���[���
};

/**
 * @brief �m�[�h��K���G���[�f�[�^
 */
struct ErrorRegNodes
{
	vector<CString>	eID;	///< �m�[�hID�W��
	int		eMesh;			///< 2�����b�V��
	MError	eError;			///< �G���[���
};

/**
 * @brief �G���[�f�[�^�W����
 */
struct ErrorData
{
	ErrorRegLink	eLink;		///< �����N�K��
	ErrorRegNode	eNode;		///< 3�_�m�[�h�K��
	ErrorRegNodes	eNodes;		///< �m�[�h��K��
};

/**
 * @brief �V��ʋK�����C���`�F�b�N�N���X
 */
class CCheckRegLayer
{
public:
	CCheckRegLayer(void);
	~CCheckRegLayer(void);

	/**
	 * @brief IN�f�[�^�̐V��ʋK�����C�����`�F�b�N����B
	 * @note  �w�肵��2�����b�V����IN�f�[�^�V��ʋK�������`�F�b�N����B
	 *
	 * @param	rFilePath	[in]	���HIN�f�[�^�i�[�f�B���N�g��
	 * @param	rMeshCode	[in]	�w��2�����b�V���R�[�h
	 * @param	rRegAttr	[in]	�V��ʋK����l
	 *
	 * @return	ture		����I��
	 * @return	false		�ُ�I��
	 */
	bool check( CString &rFilePath, int rMeshCode, RegAttrList& rRegAttr );

	bool IsError() { return m_bIsError; };
private:

	/**
	 * @brief �V��ʋK�����C���̃G���[�����o�͂���B
	 * @note  �w�肵��2�����b�V���� �V��ʋK�������`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�V��ʋK�����
	 * @param	rCode		[in]	�G���[�����l
	 * @param	rType		[in]	�K�����
	 * @param	bError		[in]	�G���[�t���O
	 * @param	bHex		[in]	16�i���\���t���O
	 *
	 * @return	�Ȃ�
	 */
	void	PrintError( int rKey, ErrorData& rError, int rCode, int rType, bool bError, bool bHex);

	/**
	 * @brief �V��ʋK�����C���̃G���[�����o�͂���B
	 * @note  �w�肵��2�����b�V���� �V��ʋK�������`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�V��ʋK�����
	 * @param	rType		[in]	�K�����
	 * @param	bError		[in]	�G���[�t���O
	 *
	 * @return	�Ȃ�
	 */
	void	PrintError( int rKey, ErrorData& rError, int rType, bool bError);

	/**
	 * @brief �V��ʋK���̋K����ʂ𔻒肷��B
	 * @note  �w�肵���V��ʋK�����̋K����ʂ��`�F�b�N����B
	 *
	 * @param	rReg			[in]	�V��ʋK�����
	 * @param	rError			[in]	�V��ʋK�����G���[
	 * @param	rRegAttr		[in]	�V��ʋK����l
	 * @param	rMeshCode		[in]	2�����b�V���R�[�h
	 *
	 * @return	�K�����
	 */
	int		CheckType( const INReg* rReg, ErrorData& rError, RegAttrList& rRegAttr, int rMeshCode );

	/**
	 * @brief ���Ԏw����`�F�b�N����B
	 * @note  �w�肵�����Ԏw����`�F�b�N����B
	 *
	 * @param	rStart			[in]	�J�n����
	 * @param	rEnd			[in]	�I������
	 *
	 * @return	ture	����
	 * @return	false	�ُ�
	 */
	bool	CheckTimeCode( int rStart, int rEnd );

	/**
	 * @brief �����w����`�F�b�N����B
	 * @note  �w�肵�������w����`�F�b�N����B
	 *
	 * @param	rStart			[in]	�J�n����
	 * @param	rEnd			[in]	�I������
	 *
	 * @return	ture	����
	 * @return	false	�ُ�
	 */
	bool	CheckDayCode( int rStart, int rEnd );

	/**
	 * @brief �m�[�h��̏d�����`�F�b�N����B
	 * @note  �w�肵���V��ʋK���̃m�[�h��ɏd�������������`�F�b�N����B
	 *
	 * @param	rReg				[in]	�V��ʋK��
	 * @param	rMeshCode			[in]	2�����b�V���R�[�h
	 * @param	rType				[in]	�K�����
	 *
	 * @return	ture	����
	 * @return	false	�ُ�
	 */
	bool	CheckNode( const INReg* rReg, int rMeshCode, int rType);

private:
	bool m_bIsError;
};
