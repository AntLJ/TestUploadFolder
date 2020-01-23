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
 *	@file DBInfo.cpp
 *	@brief DB���N���X ������
 *	@author	F.Adachi
 *	@date	2005/08/05		�V�K�쐬
 *	$ID$
 */

#include "StdAfx.h"
#include "DBInfo.h"
#include <string>

const CString DBInfo::m_Instance = "5151";

/// �R���X�g���N�^
DBInfo::DBInfo(void)
{
}

/// �f�X�g���N�^
DBInfo::~DBInfo()
{
}

/**
 *	@brief	������
 *	@param	cpDBStr	[in]	DB�w�蕶���� (��F"TECHROAD\@coral.mr.ipc.pioneer.co.jp(SNAP_TEST)" )
 **/
void DBInfo::init( char* cpDBStr )
{
	std::string aFunc = "DBInfo::init()";
	
	m_DataBase = cpDBStr;
}


/**
 *	@brief	�e�t�B�[���h�̒l��\��
 *	@param	crStream	[in]	�o�̓X�g���[��
 **/
void DBInfo::disp( std::ostream& crStream ) const
{
	crStream << std::endl;
	crStream << "�f�[�^�x�[�X���F["	<< static_cast<const char*>(m_DataBase) << "]" << std::endl;				
	crStream << std::endl;
}

