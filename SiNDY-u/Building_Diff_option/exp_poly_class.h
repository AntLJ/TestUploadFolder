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

#ifndef	_EXP_POLY_CLASS_H_
#define	_EXP_POLY_CLASS_H_

#include "StdAfx.h"
#include <poly_class.h>
/**
	poly_class�̊g���N���X
	IGeometryPtr�Ƃ̎󂯓n�������C���H
*/

struct	line_seg2 : public line_seg
{
	int		e_iIndx;
	
	/// �R���X�g���N�^
	line_seg2() {
		line_seg::line_seg();
		e_iIndx	= 0;
	}
	/// �f�X�g���N�^
	~line_seg2() {
	}

	/*
		�Q�_ c_pt1,c_pt2�ō\�����������Ɠ_c_src_pt�̋��������߂�
		@return	����(���K�����W�l)
	*/
	double		get_pt_seg_dist	(	Dpos2		c_src_pt	);	// ���������߂�_

	/*
		�f�[�^�̃Z�b�g(XY�̏��������ɂ���j
		@return ����
	*/
	void	Data_Set(	Dpos2	c_Head,		///< ������W�_�i�n�_�j
						Dpos2	c_Tail)		///< ������W�_�i�I�_�j
	{
		if( c_Head.xpos < c_Tail.xpos ) {
			e_Head.xpos	= c_Head.xpos;
			e_Tail.xpos	= c_Tail.xpos;
		}else {
			e_Head.xpos	= c_Tail.xpos;
			e_Tail.xpos	= c_Head.xpos;
		}
		if( c_Head.ypos < c_Tail.ypos ) {
			e_Head.ypos	= c_Head.ypos;
			e_Tail.ypos	= c_Tail.ypos;
		}else {
			e_Head.ypos	= c_Tail.ypos;
			e_Tail.ypos	= c_Head.ypos;
		}
	}

	/**
		��r���Z�q
	*/
	int		operator<( const line_seg2 c_Seg ) const {
		int	ret	= 0;
		if( line_seg::e_Head.xpos != c_Seg.e_Head.xpos ) {
			if( line_seg::e_Head.xpos < c_Seg.e_Head.xpos ) {
				ret	= 1;
			}
		}else
		if( line_seg::e_Head.ypos != c_Seg.e_Head.ypos ) {
			if( line_seg::e_Head.ypos < c_Seg.e_Head.ypos ) {
				ret	= 1;
			}
		}else
		if( line_seg::e_Tail.xpos != c_Seg.e_Tail.xpos ) {
			if( line_seg::e_Tail.xpos < c_Seg.e_Tail.xpos ) {
				ret	= 1;
			}
		}else
		if( line_seg::e_Tail.ypos != c_Seg.e_Tail.ypos ) {
			if( line_seg::e_Tail.ypos < c_Seg.e_Tail.ypos ) {
				ret	= 1;
			}
		}
		return ( ret );
	}

};

/*
	�_��ɑ΂��ĉ��炩�̏������s���N���X
*/
struct	exp_poly_class : public poly_cls
{
	set<Dpos2>			e_sExpPts;		///< XY�Ń\�[�g�����_��
//	multiset<Dpos2>		e_msErrPts;		///< �G���[�Ƃ��Č��o�����_�̃}���`�Z�b�g

	vector<line_seg2>	e_vExpArcs;		///< �A�[�N��
//	multiset<line_seg2>	e_msExpArcs;	///< �A�[�N��

private:
	/**
		�W�I���g���̓_����m�ۂ���
		@return	����	WKSPoint�_��ւ̃|�C���^
		@return	���s	NULL�|�C���^
	*/
	WKSPoint*	Get_GeoPts(
					IGeometryPtr	c_ipGeo,	///< �W�I���g��[IN]
					long&			c_lCount	///< ��ԓ_��[OUT]
				);
public:

	/// �R���X�g���N�^
	exp_poly_class	() {
	}
	/// �f�X�g���N�^
	~exp_poly_class	() {
	}
	
	/**
		�W�I���g�����΂炯���_��ɗ��Ƃ�����	
		@return	0	����
		@return -1	���s
	*/
	int		Set_GeotoPts(
				IGeometryPtr	c_ipGeo,	///< �W�I���g��[IN]
				int				c_iID 		///< ID[IN]
			);

	/**
		�W�I���g�����΂炯���A�[�N��i�x�N�^�j�ɗ��Ƃ�����	
		@return	0	����
		@return -1	���s
	*/
	int		Set_GeotoArcs(
				IGeometryPtr	c_ipGeo,	///< �W�I���g��[IN]
				int				c_iID		///< ID[IN]
			);

	/**
		�A�[�N�ɍł��߂��_�����o��
		@return	0	���o������
		@return	1	�Y���f�[�^����
	*/
	int		Get_NearestPt(
				line_seg2			c_lSeg,			///< �A�[�N�̐���[IN]
				double				c_lDeg,			///< 臒l�i�x�j[IN}
				vector<Dpos2>&		c_msErrPts		///< �G���[�ƂȂ�_�̃Z�b�g[OUT]
			);

	/**
		�_�ɍł��߂��A�[�N�����o��
		@return	0	���o������
		@return	1	�Y���f�[�^����
	*/
	int		Get_NearestArcs(
				Dpos2				c_lPt,			///< �_IN]
				double				c_lDeg,			///< 臒l�i�x�j[IN}
				vector<line_seg2>&	c_msErrPts		///< �G���[�ƂȂ�A�[�N�̃Z�b�g[OUT]
			);
};

#endif
