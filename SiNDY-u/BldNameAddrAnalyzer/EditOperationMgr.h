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

#include <TDC/sindylib_core/Workspace.h>

/// �ҏW�J�n�A�I���Ǘ��p�N���X
class EditOperationMgr
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	EditOperationMgr(void){};
	EditOperationMgr(const IWorkspacePtr& work){ Init(work); }
	
	/**
	* @brief �f�X�g���N�^
	* @note �ҏW���J�n����Ă��郏�[�N�X�y�[�X������Δj������
	*/
	~EditOperationMgr(void);

	/**
	* @brief �R���X�g���N�^
	* @param works [in] �Ǘ����郏�[�N�X�y�[�X
	* @param traceMsg [in] �g���[�X�p���b�Z�[�W��\�����邩
	*/
	void Init( const IWorkspacePtr& work, bool traceMsg = false ){ 
		m_work = work;
		m_work.m_bTrace = traceMsg;
	}

	/**
	* @brief �ҏW�A�ҏW�I�y���[�V�������J�n����
	* @return �Ȃ��i���s�����ꍇ�͗�O�����j
	*/
	void Start();
	
	/**
	* @brief �ҏW�A�ҏW�I�y���[�V�������I������
	* @return �Ȃ��i���s�����ꍇ�͗�O�����j
	*/
	void Stop();
	
	/**
	* @brief �ҏW��j������
	*/
	void Abort();

	bool IsBeingEdited();

private:
	sindy::CWorkspace m_work; //!< �Ǘ����郏�[�N�X�y�[�X
};

