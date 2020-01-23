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

class CSDEManager
{
public:
	/*
	 * @brief �R���X�g���N�^
	 */
	CSDEManager(void);
	/*
	 * @brief �f�X�g���N�^
	 */
	~CSDEManager(void);
	/**
	 * @brief SDE�ڑ�
	 * @param lpszUserName	[in] ���[�U
	 * @param lpszPassword	[in] �p�X���[�h
	 * @param lpszInstance	[in] �C���X�^���X
	 * @param lpszServer	[in] �T�[�o
	 * @param lpszVersion	[in] �o�[�W����
	 * @param ppWorkspace	[out] ���[�N�X�y�[�X�|�C���^
	 * @return bool
	 */
	bool CSDEManager::ConnectSDE(const LPCTSTR lpszUserName,
									const LPCTSTR lpszPassword,
									const LPCTSTR lpszInstance,
									const LPCTSTR lpszServer,
									const LPCTSTR lpszVersion,
									IWorkspacePtr& ppWorkspace);
	/**
	 * @brief �w�背�C���̃I�[�v��
	 * @param lpcszLayerName	[in]	���C����
	 * @param ipWorkspace		[in]	���[�N�X�y�[�X�|�C���^
	 * @param ppFeatureClass	[out]	�t�B�[�`���N���X�|�C���^
	 * @return bool
	 */
	bool OpenTargetLayer(const LPCTSTR lpcszLayerName, IWorkspacePtr ipWorkspace, IFeatureClass **ppFeatureClass);
	/**
	 * @brief �w��e�[�u���̃I�[�v��
	 * @param lpcszTableName	[in]	�e�[�u����
	 * @param ipWorkspace		[in]	���[�N�X�y�[�X�|�C���^
	 * @param ppTable			[out]	�e�[�u���|�C���^
	 * @return bool
	 */
	bool OpenTargetTable(const LPCTSTR lpcszTableName, IWorkspacePtr ipWorkspace, ITable **ppTable);
	/**
	 * @brief �J�[�\���̎擾(�t�B�[�`���Ȃ�)
	 * @param ipFeatureClass		[in]  �t�B�[�`���N���X�|�C���^
	 * @param lpszWhereClause		[in]  Where��
	 * @param ppCursor				[out] �J�[�\���|�C���^
	 * @return bool
	 */	
	bool GetCursor(ITablePtr ipTable, const LPCTSTR lpszWhereClause, _ICursor **ppCursor);
	/**
	 * @brief �t�B�[�`���[�J�[�\���̎擾
	 * @param ipFeatureClass		[in]  �t�B�[�`���N���X�|�C���^
	 * @param lpszWhereClause		[in]  Where��
	 * @param ppFeatureCursor	[out] �t�B�[�`���J�[�\���|�C���^
	 * @return bool
	 */
	bool GetFeatureCursor(IFeatureClassPtr ipFeatureClass, const LPCTSTR lpszWhereClause, IFeatureCursor **ppFeatureCursor);
	/**
	 * @brief �t�B�[���h�l�̎擾
	 * @note �^����ꂽRow�̎w�肳�ꂽ�t�B�[���h�̒l���擾
	 * @param ipRow [in] �擾����I�u�W�F�N�g�̍s
	 * @param lpcszFieldName [in] �t�B�[���h��
	 * @return �w�肳�ꂽ�t�B�[���h��CComVariant�l
	 */
	CComVariant	 GetValue(_IRowPtr ipRow, LPCTSTR lpcszFieldName );
};
