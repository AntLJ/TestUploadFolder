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
#include "Z2LinkLoad.h"
#include "Z2NodeLoad.h"

/**
 * @brief ���H�����N�G���[�f�[�^
 */
struct ErrorLink
{
	CString		eFrom;			///< �����N�n�_ID
	CString		eTo;			///< �����N�I�_ID
	int			eMesh;			///< �Q�����b�V��
	MError		eError;			///< �G���[���
};

typedef pair<int,int>			NodeXY;

/**
 * @brief ���H�����N���C���`�F�b�N�N���X
 */
class CCheckLinkLayer
{
public:
	CCheckLinkLayer(void);
	~CCheckLinkLayer(void);

	/**
	 * @brief IN�f�[�^�̓��H�����N���C�����`�F�b�N����B
	 * @note  �w�肵��2�����b�V���̓��H�����N���C�����`�F�b�N����B
	 *
	 * @param	rFilePath	[in]	���HIN�f�[�^�i�[�f�B���N�g��
	 * @param	rMeshCode	[in]	�w��2�����b�V���R�[�h
	 * @param	rLinkAttr	[in]	�����N��l
	 * @param	eCustomer	[in]	�d������
	 * @param	setMesh		[in]	VICS�`�F�b�N�p���b�V�����X�g
	 *
	 * @return	ture		����I��
	 * @return	false		�ُ�I��
	 */
	bool check( CString &rFilePath, int rMeshCode, LinkAttrList& rLinkAttr, Customer eCustomer, const std::set<int>& setMesh );

	bool IsError() { return m_bIsError; };
private:

	/**
	 * @brief �����N���C���̃G���[�����o�͂���B(�R�[�h�l:16�i��)
	 * @note  �w�肵��2�����b�V���̃����N�����`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�����N���
	 * @param	rCode		[in]	�G���[�����l
	 *
	 * @return	ture		����I��
	 * @return	false		�ُ�I��
	 */
	void PrintHexError( int rKey, ErrorLink& rError, int rCode); 

	/**
	 * @brief �����N���C���̃G���[�����o�͂���B(�R�[�h�l:16�i��)
	 * @note  �w�肵��2�����b�V���̃����N�����`�F�b�N����B
	 *
	 * @param	rKey		[in]	�G���[���L�[
	 * @param	rError		[in]	�G���[�����N���
	 * @param	rCode		[in]	�G���[�����l
	 *
	 * @return	ture		����I��
	 * @return	false		�ُ�I��
	 */
	void PrintError( int rKey, ErrorLink& rError, int rCode);

	/**
	* @brief	VICS�`�F�b�N
	* @param	setMesh				[in]	VICS�`�F�b�N�p���b�V�����X�g
	* @param	aError				[in]	�G���[�o�̓N���X
	* @param	pLink				[in]	���H�����N
	* @param	bVicsOrdRev			[in]	��or�t������VICS���t�^����Ă��邩
	*/
	void CheckVics( const std::set<int>& setMesh, ErrorLink& aError, IN2Link& pLink, bool bVicsOrdRev );

	/**
	* @brief	VICS�`�F�b�N�i�敪�ƃ��b�V���j
	* @param	setMesh				[in]	���b�V�����X�g
	* @param	aError				[in]	�G���[�o�̓N���X
	* @param	pLink				[in]	VICS�����N
	* @param	bVicsOrdRev			[in]	��or�t������VICS���t�^����Ă��邩
	*/
	void CheckVicsPartMesh( const std::set<int>& setMesh, ErrorLink& aError, IN2Link& pLink, bool bVicsOrdRev );

private:
	bool m_bIsError;
};
