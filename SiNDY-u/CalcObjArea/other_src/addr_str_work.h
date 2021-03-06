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
// Z¶ñÌ¢ë¢ë×Hðs¤
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
		SI		= 0x01,	///< s
		KU		= 0x02,	///< æ
		TYOU	= 0x04,	///< ¬
		SON		= 0x08	///< º
	};
	addr_str_work() {
	}
	~addr_str_work() {
	}
	/**
		sæ¬ºÌ»è
	*/
	int	Get_Sikutyouson (
		char*	c_cpAddrCode	///< ZR[h
	);
	/**
		sæ¬ºÌ»è
	*/
	int	Get_Sikutyouson_byName (
		char*	c_cpAddrName	///< Z¼
	);
	/**
		sæ¬º¼Ìl¾
		131xxAxx100ÈOÍÅÌXy[XÅØé
	*/
	void	Get_Sikutyouson_Name (
		char*	c_cpAddrCode,	///< ZR[h
		char*	c_cpInputName,	///< üÍ
		char**	c_cpOututName	///< oÍ
	);

	/**
		xxð¶Ìt^
	*/
	void	Put_Yakusyo	(
		string&	c_sNoteStr
	);
};

#endif	// _ADDR_STR_WORK_H_

