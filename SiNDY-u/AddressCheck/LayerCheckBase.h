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

#include "GlobalFunc.h"
#include "TableFinder.h"
#include "SettingFile.h"
#include "Logger.h"
#include "CommonCheck\RelationCheck.h"

/**
 * @class	LayerCheckBase
 * @brief	�`�F�b�N���N���X
 */
class LayerCheckBase
{
public:
	/**
	 * @brief  �R���X�g���N�^
	 */
	LayerCheckBase(void):m_curMeshCode(0L){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~LayerCheckBase(void){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	friend class RelationCheck;
	/**
	 * @brief	���C�����擾
	 * @return	���C����
	 */
	LPCTSTR getLayerCheckName(){ return getName(); }

	/**
	 * @brief	������
	 * @param	[in] settingFile     �ݒ�t�@�C��
	 * @param	[in] tableFinder     �e�[�u���Ǘ�
	 * @param	[in] errorLog        �G���[�t�@�C���Ǘ�
	 * @retval	true:����
	 * @retval	false:���s
	 */
	bool init( const SettingFile& settingFile, TableFinder& tableFinder, ErrLogger& errorLog, const CString& textAddressMaster ){ return initialize( settingFile, tableFinder, errorLog, textAddressMaster ); }

	/**
	 * @brief	�`�F�b�N
	 * @param	[in] ulMeshCode			���b�V���R�[�h
	 * @param	[in] ipMeshGeometry		���b�V���W�I���g��
	 */
	void checkLayer( unsigned long ulMeshCode, const IGeometryPtr& ipMeshGeometry, long meshID ){ check( ulMeshCode, ipMeshGeometry, meshID ); }

private:

	/**
	 * @brief	���C�����擾
	 * @return	���C����
	 */
	virtual LPCTSTR getName() = 0;

	/**
	 * @brief	������
	 * @param	[in] settingFile     �ݒ�t�@�C��
	 * @param	[in] tableFinder     �e�[�u���Ǘ�
	 * @param	[in] errorLog        �G���[�t�@�C���Ǘ�
	 * @param	[in] textAddressMaster  �Z���}�X�^�f�[�^�p�[�X
	 * @retval	true:����
	 * @retval	false:���s
	 */
	virtual bool initialize( const SettingFile& settingFile, TableFinder& tableFinder, ErrLogger& errorLog, const CString& textAddressMaster ) = 0;

	/**
	 * @brief	�`�F�b�N
	 * @param	[in] ulMeshCode			���b�V���R�[�h
	 * @param	[in] ipMeshGeometry		���b�V���W�I���g��
	 */
	virtual void check( unsigned long ulMeshCode, const IGeometryPtr& ipMeshGeometry, long meshID ) = 0;

public:

	ErrLogger		   *m_errorLog;		    //!< �G���[���O
	unsigned long	   m_curMeshCode;       //!< ���݃`�F�b�N���Ă��郁�b�V���R�[�h
	std::set<ErrorIdInfo>   m_errorIdSet;   //!< �G���[���Z�b�g
};
