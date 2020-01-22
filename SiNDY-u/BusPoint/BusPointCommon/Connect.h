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
#include "stdafx.h"
#include "../FieldMap.h"
#include <sindy/workspace.h>

namespace DB
{
	/**
	 * @brief   ���[�N�X�y�[�X�I�[�v��
	 * @param   name           [in]  �ڑ���
	 * @param   ipWorkspace    [in]  ���[�N�X�y�[�X
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	inline bool OpenWorkspace( const CString& name, IWorkspacePtr& ipWorkspace )
	{
		ipWorkspace = sindy::create_workspace( name );
		if( ! ipWorkspace ){
			std::cerr << "#FATAL DB Connect Fail : " << CT2CA(name) << std::endl;
			return false;
		}
		return true;
	};

	/**
	 * @brief   ���[�N�X�y�[�X�I�[�v��
	 * @param   name            [in]  �ڑ���
	 * @param   ipEditWorkspace [in]  ���[�N�X�y�[�X
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	inline bool OpenWorkspace( const CString& name, IWorkspaceEditPtr& ipEditWorkspace )
	{
		IWorkspacePtr ipWorkspace;
		if( ! OpenWorkspace( name, ipWorkspace ) )
			return false;
		ipEditWorkspace = ipWorkspace;
		return true;
	};

	/**
	 * @brief   �t�B�[�`���N���X�I�[�v�����t�B�[���h�}�b�v�쐬
	 * @param   name           [in]  �t�B�[�`���N���X��
	 * @param   ipWorkspace    [in]  �I�[�v�����邽�߂̃��[�N�X�y�[�X
	 * @param   ipFeatureClass [out] �擾����t�B�[�`���N���X
	 * @param   fieldMap       [out] �擾����t�B�[���h�}�b�v
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	inline bool OpenFeatureClass( const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass, CFieldMap& fieldMap )
	{
		((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t( name ), &ipFeatureClass );
		if( ! ipFeatureClass ){
			std::cerr << "#FATAL FC Open Fail : " << CT2CA(name) << std::endl;
			return false;
		}
		// �t�B�[���h�}�b�v�쐬
		fieldMap.CreateFieldMap( ipFeatureClass->GetFields() );

		return true;
	};

	/**
	 * @brief   �e�[�u���I�[�v�����t�B�[���h�}�b�v�쐬
	 * @param   name           [in]  �t�B�[�`���N���X��
	 * @param   ipWorkspace    [in]  �I�[�v�����邽�߂̃��[�N�X�y�[�X
	 * @param   ipTable        [out] �擾����e�[�u��
	 * @param   fieldMap       [out] �擾����t�B�[���h�}�b�v
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	inline bool OpenTable( const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable, CFieldMap& fieldMap )
	{
		// �I�[�v��
		((IFeatureWorkspacePtr)ipWorkspace)->OpenTable( _bstr_t( name ), &ipTable );
		if( ! ipTable ){
			std::cerr << "#FATAL Table Open Fail : " << CT2CA(name) << std::endl;
			return false;
		}
		// �t�B�[���h�}�b�v�쐬
		fieldMap.CreateFieldMap( ipTable->GetFields() );
	
		return true;
	};

	/**
	 * @brief   �ҏW�J�n
	 * @param   ipEditWorkspace [in]  ���[�N�X�y�[�X
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	inline bool StartEdit( IWorkspaceEditPtr& ipEditWorkspace )
	{
		// �ҏW�J�n�iUndo Redo ������Ȃ����߁Afalse�j
		if( S_OK != ipEditWorkspace->StartEditing( VARIANT_FALSE ) || S_OK != ipEditWorkspace->StartEditOperation() ){
			std::cerr << "#ERROR Start Edit Fail" << std::endl;
			return false;
		}
		return true;
	};

	/**
	 * @brief   �ҏW�ۑ����I��
	 * @param   ipEditWorkspace [in]  ���[�N�X�y�[�X
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	inline bool StopEdit( IWorkspaceEditPtr& ipEditWorkspace )
	{
		// �ҏW�m��
		if( S_OK != ipEditWorkspace->StopEditOperation() ){
			std::cerr << "#ERROR Stop Edit Operation Fail" << std::endl;
			ipEditWorkspace->AbortEditOperation();
			return false;
		}
	
		// �ҏW�I���i�ۑ�����K�v�����邽�߁Atrue�j
		if( S_OK != ipEditWorkspace->StopEditing( VARIANT_TRUE ) ){
			std::cerr << "#ERROR Stop Edit Fail" << std::endl;
			return false;
		}
		return true;
	};



};
