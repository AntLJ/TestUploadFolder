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
 *	@file ConfirmedLink.h
 *	@brief �m�F�ς݃����N�W���N���X ��`��
 *	@author	F.Adachi
 *	@date	2005/10/21		�V�K�쐬
 *	$ID$
 */

#ifndef ___CONFIRMEDLINK___
#define ___CONFIRMEDLINK___

#include <set>
#include <boost/utility.hpp>

/// <b>�m�F�ς݃����N�W���N���X</b>
class ConfirmedLink : boost::noncopyable
{
public:
	ConfirmedLink(void);
	~ConfirmedLink();

	bool	exist( long cObjectID ) const;	//< ���ݔ���
	void	insert( long cObjectID );		//< �ǉ�
	void	clear(void);					//< �S�폜

private:
	std::set<long> m_Data;		//< �m�F�ς݃����N�̃I�u�W�F�N�gID�W��

};

#endif
