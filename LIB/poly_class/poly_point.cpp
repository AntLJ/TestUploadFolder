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
#include "poly_class.h"

//--------------------------------------------------------------------
// 2001.10.11
// �_�Ɠ_�̐��K�����W�l�ł̋���
// �Ԃ�l
// 	���K�����W�l����
//--------------------------------------------------------------------
double	Dpos::
Get_Dist		(	Dpos		c_Pt	)	//	�Ώۓ_
{
	double	dist	= 0.0;

	double	x		= 0.0;
	double	y		= 0.0;

	x		= xpos - c_Pt.xpos;
	y		= ypos - c_Pt.ypos;

	dist	= sqrt(x*x + y*y);

	return	(dist);
}

double	Dpos2::
Get_Dist		(	Dpos2		c_Pt	)	//	�Ώۓ_
{
	double	dist	= 0.0;

	double	x		= 0.0;
	double	y		= 0.0;

	x		= xpos - c_Pt.xpos;
	y		= ypos - c_Pt.ypos;

	dist	= sqrt(x*x + y*y);

	return	(dist);
}
