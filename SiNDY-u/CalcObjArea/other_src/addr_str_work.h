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

//--------------------------------------------------------
// �Z��������̂��낢��׍H���s��
//--------------------------------------------------------

#include <string>

#ifndef	_ADDR_STR_WORK_H_
#define	_ADDR_STR_WORK_H_

//using namespace stlport;
using namespace std;

class	addr_str_work
{
public:
	enum	WHAT_IS_IT {
		SI		= 0x01,	///< �s
		KU		= 0x02,	///< ��
		TYOU	= 0x04,	///< ��
		SON		= 0x08	///< ��
	};
	addr_str_work() {
	}
	~addr_str_work() {
	}
	/**
		�s�撬���̔���
	*/
	int	Get_Sikutyouson (
		char*	c_cpAddrCode	///< �Z���R�[�h
	);
	/**
		�s�撬���̔���
	*/
	int	Get_Sikutyouson_byName (
		char*	c_cpAddrName	///< �Z����
	);
	/**
		�s�撬�����̊l��
		131xx�Axx100�ȊO�͍ŏ��̃X�y�[�X�Ő؂�
	*/
	void	Get_Sikutyouson_Name (
		char*	c_cpAddrCode,	///< �Z���R�[�h
		char*	c_cpInputName,	///< ����
		char**	c_cpOututName	///< �o��
	);

	/**
		xx���������̕t�^
	*/
	void	Put_Yakusyo	(
		string&	c_sNoteStr
	);
};

#endif	// _ADDR_STR_WORK_H_

