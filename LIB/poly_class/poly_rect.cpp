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
//	外接長方形で、高速にチェックする
//	返り値 点が内部 = 1 点が外 = 0
//--------------------------------------------------------------------
int		Crect::
Point_Inner_Chk(	Dpos		c_Pt	)	// ポイント
{
	if((xmin <= (c_Pt.xpos) && (c_Pt.xpos) <= xmax) &&
	   (ymin <= (c_Pt.ypos) && (c_Pt.ypos) <= ymax))
	{
		return(1);
	}
	return(0);
}

//--------------------------------------------------------------------
//	矩形内に矩形が内包されるかどうか 2000.5.18
//	外接長方形で、高速にチェックする
//	返り値 矩形2が矩形1の内部 = 1 外 = 0
//--------------------------------------------------------------------
int		Crect::
Rect_Inner_Chk	(	Crect	c_Rect	)	// 
{
	if(xmin <= c_Rect.xmin && xmax >= c_Rect.xmin &&
	   xmin <= c_Rect.xmax && xmax >= c_Rect.xmax &&
	   ymin <= c_Rect.ymin && ymax >= c_Rect.ymin &&
	   ymin <= c_Rect.ymax && ymax >= c_Rect.ymax)
	{
		return(1);
	}
	return(0);
}

//--------------------------------------------------------------------
// 2001.10.05
//	矩形同士の交差チェック
//	外接長方形で、高速にチェックする
//	返り値
//		矩形1と矩形2が交差 = 1
//		内部 = 1 外 = 0
//--------------------------------------------------------------------
int		Crect::
Rect_Cross_Chk	(	Crect	c_Rect	)	//	矩形2
{
	// 外れる条件でチェックする
	if((xmax < c_Rect.xmin ||
		xmin > c_Rect.xmax ||
		ymax < c_Rect.ymin ||
		ymin > c_Rect.ymax))
	{
		return(0);
	}
	return(1);
}

