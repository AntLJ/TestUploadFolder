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
 *	@file DBHandleBase.cpp
 *	@brief DB�����A�v���P�[�V�������N���X ������
 *	@author	F.Adachi
 *	@date	2005/10/18		�V�K�쐬
 *	$ID$
 */

#include "StdAfx.h"
#include "DBHandleBase.h"
#include "DBAccess.h"
#include <fstream>
#include <set>
#include <vector>
#include <string>


/// �R���X�g���N�^
DBHandleBase::DBHandleBase(void) : 
	m_ipWorkspace(NULL)
{
}

/// �f�X�g���N�^
DBHandleBase::~DBHandleBase()
{
}


/**
 *	@brief	���C������
 **/
void DBHandleBase::run(void)
{
	std::string aFunc = "DBHandleBase::run()";
#ifdef	_DEBUG
	m_pDBInfo->disp( std::cout );
#endif
	DBAccess aDBAccess;

	m_ipWorkspace = aDBAccess.open( m_pDBInfo.get() );
	if (m_ipWorkspace == NULL) { throw std::runtime_error( "R1F:" + aFunc + " : ���[�N�X�y�[�X�擾���s" ); }

	// �����i�����̓T�u�N���X�Ɉˑ��j
	process();
}

