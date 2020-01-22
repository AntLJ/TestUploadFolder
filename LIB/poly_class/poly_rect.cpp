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
//	�O�ڒ����`�ŁA�����Ƀ`�F�b�N����
//	�Ԃ�l �_������ = 1 �_���O = 0
//--------------------------------------------------------------------
int		Crect::
Point_Inner_Chk(	Dpos		c_Pt	)	// �|�C���g
{
	if((xmin <= (c_Pt.xpos) && (c_Pt.xpos) <= xmax) &&
	   (ymin <= (c_Pt.ypos) && (c_Pt.ypos) <= ymax))
	{
		return(1);
	}
	return(0);
}

//--------------------------------------------------------------------
//	��`���ɋ�`�������邩�ǂ��� 2000.5.18
//	�O�ڒ����`�ŁA�����Ƀ`�F�b�N����
//	�Ԃ�l ��`2����`1�̓��� = 1 �O = 0
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
//	��`���m�̌����`�F�b�N
//	�O�ڒ����`�ŁA�����Ƀ`�F�b�N����
//	�Ԃ�l
//		��`1�Ƌ�`2������ = 1
//		���� = 1 �O = 0
//--------------------------------------------------------------------
int		Crect::
Rect_Cross_Chk	(	Crect	c_Rect	)	//	��`2
{
	// �O�������Ń`�F�b�N����
	if((xmax < c_Rect.xmin ||
		xmin > c_Rect.xmax ||
		ymax < c_Rect.ymin ||
		ymin > c_Rect.ymax))
	{
		return(0);
	}
	return(1);
}

