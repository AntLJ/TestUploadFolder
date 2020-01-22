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

#ifndef	__COMMON_USE_H__
#define	__COMMON_USE_H__

#include "StdAfx.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <set>

using namespace	std;

//-----------------------------------------
// ���ʂŎg�p�ł�����̂ɂ��Ă����ɂ܂Ƃ߂�
//-----------------------------------------


class	common_use
{

public:
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

	//-----------------------------------------------------------
	// ���b�V���R�[�h�̃��X�g���쐬����
	// @return	����	true
	// @return	���s	false
	//-----------------------------------------------------------
	bool	MakeMeshList(
		char*		c_cpRFName,	///< ���b�V�����X�g�t�@�C����[IN]
		set<int>*	c_spMList	///< ���b�V�����X�g�ւ̃|�C���^[OUT]	
	);

//	string	Str_to_SJIS	(
//		CComBSTR*	c_CStr		///< ������(Unicode)
//	);

	enum	SRC_STR_MODE {
		UNICODE		= 0x01,	///< SiNDY��̃f�[�^�̏ꍇ
		SJIS		= 0x02	///< PGDB��̃f�[�^�̏ꍇ
	};

	string	Str_to_SJIS	(
		CComBSTR&	c_CStr,	///< ������(Unicode)
		int			c_iMode	///< �����ϊ����[�h
	);

	unsigned	Rev_Int(void* c_vpBuffer)
	{
		char	a_cpBuffer[4];
		a_cpBuffer[0] = ((char*) c_vpBuffer)[3];
		a_cpBuffer[1] = ((char*) c_vpBuffer)[2];
		a_cpBuffer[2] = ((char*) c_vpBuffer)[1];
		a_cpBuffer[3] = ((char*) c_vpBuffer)[0];
		return *((unsigned*) a_cpBuffer);
	}

	/**
		���p�������S�p�ɒ����i�����A�A���t�@�x�b�g�̂݁j
	*/
	string	Hankaku_to_Zenkaku	( string	c_sStr	)	// ���p������
	{
		string		ret_str;

		long		length	= c_sStr.size();
		
		int	i = 0;
		for( i = 0; i < length; i++ )
		{
			// ASCII������̐��l
			if( (0x30 <= c_sStr[i] && c_sStr[i] <= 0x39) ||
				(0x41 <= c_sStr[i] && c_sStr[i] <= 0x5a) ||
				(0x61 <= c_sStr[i] && c_sStr[i] <= 0x7a) )
			{
				ret_str	+= 0x82;
				ret_str	+= c_sStr[i]+0x1f;
			}else {
				ret_str	+= c_sStr[i];	// ���̂܂ܕԂ��i�͈͊O�̕������͂���Ƃ��Ԃ񂨂������Ȃ�j
			}
		}
		return	(ret_str);
	}
};



#endif
