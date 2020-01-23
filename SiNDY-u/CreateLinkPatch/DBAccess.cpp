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

/**
 *	@file	DBAccess.cpp
 *	@brief	DB�ڑ��Ǘ��N���X ������
 *	@author	F.Adachi
 *	@date	2005/08/05		�V�K�쐬
 *	$ID$
 */

#include "StdAfx.h"
#include "DBAccess.h"
#include "sindy/workspace.h"


/// �R���X�g���N�^
DBAccess::DBAccess(void)
{
}

/// �f�X�g���N�^
DBAccess::~DBAccess()
{
}


/**
 *	@brief	�Ώ�DB�ɐڑ�
 *	@return	���[�N�X�y�[�X�ւ̃|�C���^
 **/
IWorkspacePtr DBAccess::open( const DBInfo* cpDBInfo ) const
{
	if (!cpDBInfo) { return NULL; }

	// �ڑ��p�v���p�e�B�ݒ�

	// SDE�ڑ�
	IWorkspacePtr ipWorkspace = sindy::create_workspace(cpDBInfo->m_DataBase);
	if(! ipWorkspace)
		return NULL;

	return ipWorkspace;
}

