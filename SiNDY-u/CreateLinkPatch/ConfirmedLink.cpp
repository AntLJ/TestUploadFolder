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
 *	@file ConfirmedLink.cpp
 *	@brief �m�F�ς݃����N�W���N���X ������
 *	@author	F.Adachi
 *	@date	2005/10/21		�V�K�쐬
 *	$ID$
 */

#include "StdAfx.h"
#include "ConfirmedLink.h"


ConfirmedLink::ConfirmedLink(void)
{
}
ConfirmedLink::~ConfirmedLink()
{
}


/**
 *	@brief	���ݔ���
 *	@param	cObjectID	[in]	�ΏۃI�u�W�F�N�gID
 *	@return	���݂��邩�ۂ�
 **/
bool ConfirmedLink::exist( long cObjectID ) const
{
	return m_Data.find( cObjectID ) != m_Data.end();
}


/**
 *	@brief	�ǉ�
 *	@param	cObjectID	[in]	�ǉ�����I�u�W�F�N�gID
 **/
void ConfirmedLink::insert( long cObjectID )
{
	m_Data.insert( cObjectID ).second;	// �����̒l��ǉ����悤�Ƃ����false���Ԃ邪�A�����ł̓`�F�b�N���Ȃ�
}


/**
 *	@brief	�S�폜
 **/
void ConfirmedLink::clear(void)
{
	m_Data.clear();
}

