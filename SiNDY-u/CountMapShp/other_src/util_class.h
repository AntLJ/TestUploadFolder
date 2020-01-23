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

#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <set>

#ifndef	_UTIL_CLASS_H_
#define	_UTIL_CLASS_H_

using	namespace	std;

class	util_class
{
public:
	enum	SRC_STR_MODE {
		UNICODE		= 0x01,	///< SiNDY��̃f�[�^�̏ꍇ
		SJIS		= 0x02	///< PGDB��̃f�[�^�̏ꍇ
	};

	string	Str_to_SJIS	(
		CComBSTR&	c_CStr,		///< ������(Unicode)
		int			c_iMode		///< �����ϊ����[�h
	);

	//-----------------------------------------------------------
	// ���b�V���R�[�h�̃��X�g���쐬����
	// @return	����	true
	// @return	���s	false
	//-----------------------------------------------------------
	bool	MakeMeshList(
		char*		c_cpRFName,	///< ���b�V�����X�g�t�@�C����[IN]
		set<int>*	c_spMList	///< ���b�V�����X�g�ւ̃|�C���^[OUT]
	);

	//------------------------------------------------------
	// ����ŕ�����𕪊�����i���̕�����𕪊����Ȃ��j
	// @return  ���o����������̌�
	//------------------------------------------------------
	int		DivbyToken(
		char		*c_cpString,	///< �����Ώە�����
		char		*c_cpToken,		///< ��؂蕶����
		int			c_iStrNum,		///< �擪���牽�Ԗڂ̕����񂩂���o����
		int			c_iN,			///< ���̕���������o����
		string*		c_cpStrPtr		///< ���o����������̃|�C���^
	);

};

#endif

