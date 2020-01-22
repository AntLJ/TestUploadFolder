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
//	Vector GetVector(Point pt0, Point pt1)
//		output:	�^����ꂽ�n�_�ƏI�_�̍��W����쐬���ꂽ�x�N�g��
//
//		function:
//				�^����ꂽ�Q�_�����ԃx�N�g�����쐬����
//--------------------------------------------------------------------
void	dvec::
get_vec		(	Dpos		pt0,	//	�x�N�g���̎n�_�̍��W
				Dpos		pt1)	//	�x�N�g���̏I�_�̍��W
{
	double	x_diff	= (double)pt1.xpos - (double)pt0.xpos;
	double	y_diff	= (double)pt1.ypos - (double)pt0.ypos;

	// ��x�N�g���ւ̑Ή�
	if( x_diff == 0.0 ) {
		x	= x_diff;
	}
	if( y_diff == 0.0 ) {
		y	= y_diff;
	}
	// �P�ʃx�N�g���ɂ���
	x = x_diff / sqrt(x_diff*x_diff + y_diff*y_diff);
	y = y_diff / sqrt(x_diff*x_diff + y_diff*y_diff); 
}

//--------------------------------------------------------------------
// 2001.10.11
//	�x�N�g���̓��ς̒l�����߂�(��������)
// �Ȃ��p��90�x = 0
// �Ȃ��p���s�p = 0�ȏ�
// �Ȃ��p���݊p = ���l
//--------------------------------------------------------------------
double		dvec::
get_naiseki	(	dvec		c_Vec	)	//	�x�N�g�����̂Q
{
	//int		ret	= v0.x * c_Vec.x + v0.y * c_Vec.y;	// ���όv�Z
	double		ret	= (double)x * (double)c_Vec.x + (double)y * (double)c_Vec.y;	// ���όv�Z
	return	(ret);
    /*	2002.03.19�R�����g�A�E�g
	if(ret != 0.0)
        return ((ret) < 0.0)?-1:1;
    else	// ret == 0
        return 0;
	*/
}

//--------------------------------------------------------------------
//		output:	�Q�̂Q�����x�N�g���̊O�ς̒l
//
//		function:
//				�x�N�g���̊O�ς̒l�����߂�(��������)
//--------------------------------------------------------------------
int		dvec::
get_gaiseki	(	dvec	c_Vec)	//	vector
{
    double dRet = (double)x*(double)c_Vec.y - (double)y*(double)c_Vec.x;
    if(dRet != 0.0)
        return ((dRet) < 0.0)?-1:1;
    else
        return 0;
}

/*
	�Q�̃x�N�g���̂Ȃ��p�����߂�
	@return	�p�x
*/
double		dvec::
get_deg(	dvec	c_Vec	)
{
	double	n_atai	= 0.0;	// ���ς̒l
	int		g_atai	= 0;	// �O�ς̒l
	double	deg		= 0.0;	// �p�x
	double	v0		= 0.0;
	double	v1		= 0.0;
	v0		= fabs(sqrt(x*x+y*y));
	v1		= fabs(sqrt(c_Vec.x*c_Vec.x+c_Vec.y*c_Vec.y));
	n_atai	= get_naiseki	(c_Vec);
	g_atai	= get_gaiseki	(c_Vec);

	double	yogen	= 0.0;
	double	bunbo	= v0*v1;
	if(bunbo == 0.0 && n_atai == 0.0) {
		yogen	= 0.0;
	}else {
		yogen	= n_atai / bunbo;
	}

	double	ret_yogen	= 0.0;
	if( fabs(yogen - 1.0) < 1.0e-10 ) { // �_�u���ł̌v�Z���ƌ덷���ł�̂ł�������
		ret_yogen	= 0.0;
	}else {
		ret_yogen	= (double)acos(yogen) * 180.0;
	}

	deg		= ret_yogen / 3.14159265358979;
	
	if(g_atai > 0) {
		deg *= -1.0;
	}
	return	(deg);
}

//--------------------------------------------------------------------
//		output:	0:		�������������Ȃ�
//				1:		��������������
//		function:
//				�����̌���������s�Ȃ�
//		algorithm:
//				�����P�A�Q�e�X�ɂ��Č݂��ɑ��̐����̂ǂ��瑤�ɂ��邩�𒲂�
//				�����Ƃ��قȂ鑤�ɂ���Ό������Ă��邱�ƂɂȂ�
//--------------------------------------------------------------------
int		line_seg::
Cross_Seg	(	line_seg	c_Seg	)	//	�Ώې���
{
	dvec	 v0, v1, v2;

	v0.get_vec(e_Head, e_Tail);
	v1.get_vec(e_Head, c_Seg.e_Head);
	v2.get_vec(e_Head, c_Seg.e_Tail);
	if(v0.get_gaiseki(v1)*v0.get_gaiseki(v2) > 0) return(0);

	v0.get_vec(c_Seg.e_Head, c_Seg.e_Tail);
	v1.get_vec(c_Seg.e_Head, e_Head);
	v2.get_vec(c_Seg.e_Head, e_Tail);
	if(v0.get_gaiseki(v1)*v0.get_gaiseki(v2) > 0) return(0);

	if(((e_Head.xpos>e_Tail.xpos)?e_Head.xpos:e_Tail.xpos) <
		((c_Seg.e_Head.xpos<c_Seg.e_Tail.xpos)?c_Seg.e_Head.xpos:c_Seg.e_Tail.xpos)) return(0);
	if(((e_Head.ypos>e_Tail.ypos)?e_Head.ypos:e_Tail.ypos) <
		((c_Seg.e_Head.ypos<c_Seg.e_Tail.ypos)?c_Seg.e_Head.ypos:c_Seg.e_Tail.ypos)) return(0);
	if(((e_Head.xpos<e_Tail.xpos)?e_Head.xpos:e_Tail.xpos) >
		((c_Seg.e_Head.xpos>c_Seg.e_Tail.xpos)?c_Seg.e_Head.xpos:c_Seg.e_Tail.xpos)) return(0);
	if(((e_Head.ypos<e_Tail.ypos)?e_Head.ypos:e_Tail.ypos) >
		((c_Seg.e_Head.ypos>c_Seg.e_Tail.ypos)?c_Seg.e_Head.ypos:c_Seg.e_Tail.ypos)) return(0);

	return(1);
}

//--------------------------------------------------------------------
// 2001.10.03 PointsDiet �Ŏg���Ă����̂�����
// �Q�_ e_Head,e_Tail��ʂ钼���Ɠ_c_src_pt�̋��������߂�
// �Ԃ�l	����(���K�����W�l)
//--------------------------------------------------------------------
double		line_seg::
get_pt_line_dist	(	Dpos		c_Pt	)	// ���������߂�_
{
	double	dist	= 0.0;
	double	a		= 0.0;
	double	b		= 0.0;
	double	c		= 0.0;

	if(e_Head.xpos == e_Tail.xpos) {
		dist	= fabs(e_Head.xpos - c_Pt.xpos);
	} else {
		a		= ((double)e_Tail.ypos - (double)e_Head.ypos) / ((double)e_Tail.xpos - (double)e_Head.xpos);
		b		= -1.0;
		c		= 0.5 * (((double)e_Head.ypos + (double)e_Tail.ypos) - 
					((double)e_Tail.ypos - (double)e_Head.ypos) * 
					((double)e_Head.xpos + (double)e_Tail.xpos) / ((double)e_Tail.xpos - (double)e_Head.xpos));
		dist	= fabs(a * (double)c_Pt.xpos + b * (double)c_Pt.ypos + c) / sqrt(a * a + b * b);
	}
	return(dist);
}

//--------------------------------------------------------------------
// 2001.10.11
// �Q�_ e_Head,e_Tail�ō\�����������Ɠ_c_src_pt�̋��������߂�
// �Ԃ�l	����(���K�����W�l)
//--------------------------------------------------------------------
double		line_seg::
get_pt_seg_dist	(	Dpos		c_src_pt	)	// ���������߂�_
{
	double	dist	= 0.0;

	dvec	v1, v2;			// �x�N�^���̂P�A���̂Q
	dvec	vs1, vs2;		// �����\���_�ō\�������x�N�^
	double	nai_1	= 0;	// ���ς��̂P
	double	nai_2	= 0;	// ���ς��̂Q

	v1.get_vec	(e_Head, c_src_pt);	// ���W�_�P�ƒ��ړ_�̃x�N�g��
	v2.get_vec	(e_Tail, c_src_pt);	// ���W�_�Q�ƒ��ړ_�̃x�N�g��
	vs1.get_vec	(e_Head, e_Tail);		// ���W�_�P�ƍ��W�_�Q�̃x�N�g��
	vs2.get_vec	(e_Tail, e_Head);		// ���W�_�Q�ƍ��W�_�P�̃x�N�g��

	nai_1	= v1.get_naiseki	( vs1 );	// ���όv�Z
	nai_2	= v2.get_naiseki	( vs2 );	//  �V

	// 2008.09.12 ����~�X���Ă����̂ŏC��
	if(nai_1 >= 0 && nai_2 >= 0) {		// �Ȃ��p�������Ƃ��s�p(�����̊Ԃɒ���_������)
		line_seg	cur_seg(e_Head, e_Tail);
		dist		= cur_seg.get_pt_line_dist(c_src_pt);
	}else {	// �ǂ��������݊p�ɂȂ�(�����̊Ԃɒ���_�����Ȃ�)
		double	td1	= 0.0;
		double	td2	= 0.0;
		td1		= c_src_pt.Get_Dist( e_Head );
		td2		= c_src_pt.Get_Dist( e_Tail );
		if(td1 < td2) {
			dist	= td1;
		}else {
			dist	= td2;
		}
	}
	return(dist);
}
