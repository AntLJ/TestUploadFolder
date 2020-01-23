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
 * @file EditDef.h
 * @brief <b>�ҏW�n�p�萔��`�t�@�C��</b>\n
 * ODL�t�@�C���ƁAC/C++�̃w�b�_�ɓ�����܂��B
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _INC_SINDYEDIT_DEF_H_
#define _INC_SINDYEDIT_DEF_H_

#if defined(__MKTYPLIB__) || defined(__midl)
// MIDL�̏ꍇ
#define ODLCOMMENT(p) [helpstring(#p)]
#else
// C/C++�̏ꍇ
#define ODLCOMMENT(p)
#endif

/**
 * @enum sindyeMoveMode
 * @brief <b>�t�B�[�`���ړ����[�h��`</b>\n
 * ���̃��[�h�͎��CFeatures::Move�̃p�����[�^�Ƃ���
 * �g�p����܂��B
 */
typedef enum sindyeMoveMode
{
	ODLCOMMENT(�\���_�ǉ������ňړ�)					sindyeMove_SegmentIntersect					= 0,
	ODLCOMMENT(�\���_�ǉ������ňړ�)					sindyeMove_SegmentWithin					= 1,
	ODLCOMMENT(�ŋߖT�\���_�݈̂ړ�)					sindyeMove_Point							= 2,
	ODLCOMMENT(���E�ɍ\���_����ǉ�����Intersect�ړ�)	sindyeMove_SegmentAddPointBorderIntersect	= 3,
	ODLCOMMENT(���E�ɍ\���_����ǉ�����Within�ړ�)	sindyeMove_SegmentAddPointBorderWithin		= 4,
	ODLCOMMENT(���E�ɍ\���_���ǉ�����Within�ړ�)	sindyeMove_SegmentAddPointsBorder			= 5,
	ODLCOMMENT(Intersect�������̂����S���s�ړ�)         sindyeMove_WholeIntersect                   = 6,
	ODLCOMMENT(Within�������̂����S���s�ړ�)            sindyeMove_WholeWithin                      = 7,
} sindyeMoveMode;

#endif // _INC_SINDYEDIT_DEF_H_
