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
// èZèäï∂éöóÒÇÃÇ¢ÇÎÇ¢ÇÎç◊çHÇçsÇ§
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
		SI		= 0x01,	///< és
		KU		= 0x02,	///< ãÊ
		TYOU	= 0x04,	///< í¨
		SON		= 0x08	///< ë∫
	};
	addr_str_work() {
	}
	~addr_str_work() {
	}
	/**
		ésãÊí¨ë∫ÇÃîªíË
	*/
	int	Get_Sikutyouson (
		char*	c_cpAddrCode	///< èZèäÉRÅ[Éh
	);
	/**
		ésãÊí¨ë∫ÇÃîªíË
	*/
	int	Get_Sikutyouson_byName (
		char*	c_cpAddrName	///< èZèäñº
	);
	/**
		ésãÊí¨ë∫ñºÇÃälìæ
		131xxÅAxx100à»äOÇÕç≈èâÇÃÉXÉyÅ[ÉXÇ≈êÿÇÈ
	*/
	void	Get_Sikutyouson_Name (
		char*	c_cpAddrCode,	///< èZèäÉRÅ[Éh
		char*	c_cpInputName,	///< ì¸óÕ
		char**	c_cpOututName	///< èoóÕ
	);

	/**
		xxñèäï∂éöÇÃïtó^
	*/
	void	Put_Yakusyo	(
		string&	c_sNoteStr
	);
};

#endif	// _ADDR_STR_WORK_H_

