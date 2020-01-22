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

///< �|���S���̌����� 2000.6.02
#ifndef	_POLY_CLASS_
#define	_POLY_CLASS_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <vector>
#include <map>
#include <set>

// �X�V����
// 2003.3.5	���Q�Ƃ̕ϐ����폜

///<#include <crd_cnv.h>								///< 2000.5.30 ���W�ϊ�
/**
	���̃N���X�ł́A�_�A���A�|���S���Ɋւ��鏈�����܂Ƃ߁A�ėp�I�Ɏg����
	���̂ɂ��邱�Ƃ��ړI�ł��邩������Ȃ��B
*/

/**
	���W�_
*/
struct	Dpos
{
	double	xpos;
	double	ypos;
	int		e_iAttr;
	Dpos	() {
		xpos	= 0.0;
		ypos	= 0.0;
		e_iAttr	= 0;
	}
	Dpos	(	double	c_xpos,
				double	c_ypos	) {
		xpos	= c_xpos;
		ypos	= c_ypos;
	}

	~Dpos	() {
	}

	/*
		�f�[�^�̃Z�b�g
		@return ����
	*/
	void	Data_Set(	double	c_xpos,		///< ������W�_X
						double	c_ypos)		///< ������W�_Y
	{
		xpos	= c_xpos;
		ypos	= c_ypos;
	}

	/**
		���W�_���m�̋������l��
	*/
	double	Get_Dist(
				Dpos	c_Pt		///< �Ώۓ_
			);
	/*
		���l���Z�q
	*/
	bool	operator==( const Dpos c_Pos ) const {	///< ��r�f�[�^
			if(	xpos == c_Pos.xpos &&
				ypos == c_Pos.ypos )
			{
				return true;
			}
			return false;
	}

	/**
		��r���Z�q
	*/
	bool	operator<( const Dpos c_Pos ) const {
		bool	ret	= false;
		if( xpos != c_Pos.xpos ) {
			if( xpos < c_Pos.xpos ) {
				ret	= true;
			}
		}else
		if( ypos != c_Pos.ypos ) {
			if( ypos < c_Pos.ypos ) {
				ret	= true;
			}
		}
		return ( ret );
	}

	/*
		������Z�q
	*/
	/*	void	operator=( Dpos	c_Pos)	///< ������W�_
			{
				xpos	= c_Pos.xpos;
				ypos	= c_Pos.ypos;
			};*/
};

struct	Dpos2	// 2003.07.29�@���z�������̍������p�Œǉ�
{
	double	xpos;
	double	ypos;
	int		e_indx;
	Dpos2	() {
		xpos	= 0.0;
		ypos	= 0.0;
		e_indx	= 0;
	}
	Dpos2	(	double	c_xpos,
				double	c_ypos,
				int		c_indx) {
		xpos	= c_xpos;
		ypos	= c_ypos;
		e_indx	= c_indx;
	}

	~Dpos2	() {
	}

	/*
		�f�[�^�̃Z�b�g
		@return ����
	*/
	void	Data_Set(	double	c_xpos,		///< ������W�_X
						double	c_ypos,		///< ������W�_Y
						int		c_indx	)
	{
		xpos	= c_xpos;
		ypos	= c_ypos;
		e_indx	= c_indx;
	}

	/**
		���W�_���m�̋������l��
	*/
	double	Get_Dist(
				Dpos2	c_Pt		///< �Ώۓ_
			);
	/*
		���l���Z�q
	*/
	bool	operator==( const Dpos2 c_Pos ) const {	///< ��r�f�[�^
			if(	xpos == c_Pos.xpos &&
				ypos == c_Pos.ypos )
			{
				return true;
			}
			return false;
	}

	/**
		��r���Z�q
	*/
	bool	operator<( const Dpos2 c_Pos ) const {
		bool	ret	= false;
		if( xpos != c_Pos.xpos ) {
			if( xpos < c_Pos.xpos ) {
				ret	= true;
			}
		}else
		if( ypos != c_Pos.ypos ) {
			if( ypos < c_Pos.ypos ) {
				ret	= true;
			}
		}else
		if( e_indx != c_Pos.e_indx ) {
			if( e_indx < c_Pos.e_indx ) {
				ret	= true;
			}
		}
		return ( ret );
	}
};

typedef	std::set<Dpos2, std::less <Dpos2> >				s_Dpos2;
typedef	std::set<Dpos2, std::less <Dpos2> >::iterator	is_Dpos2;

/*
	����
*/
struct	line_seg
{
	Dpos	e_Head;		// �����̎n�_
	Dpos	e_Tail;		// �����̏I�_

	/*
		�R���X�g���N�^�i�f�t�H���g�j
	*/
	line_seg	( ) {}

	/*
		�R���X�g���N�^
	*/
	line_seg	(	Dpos	c_Head,		///< ������W�_�i�n�_�j
					Dpos	c_Tail)		///< ������W�_�i�I�_�j
	{
		e_Head	= c_Head;
		e_Tail	= c_Tail;
	}

	/**
		���l���Z�q��`
	*/
	int		operator==( const line_seg c_Seg ) const {
		int	ret	= 0;
		if( e_Head == c_Seg.e_Head &&
			e_Tail == c_Seg.e_Tail ) {
			ret	= 1;
		}
		return ( ret );
	}

	/*
		�f�[�^�̃Z�b�g
		@return ����
	*/
	void	Data_Set(	Dpos	c_Head,		///< ������W�_�i�n�_�j
						Dpos	c_Tail)		///< ������W�_�i�I�_�j
	{
		e_Head	= c_Head;
		e_Tail	= c_Tail;
	}

	/*
		�����̌���������s�Ȃ�
		algorithm:
			�����P�A�Q�e�X�ɂ��Č݂��ɑ��̐����̂ǂ��瑤�ɂ��邩�𒲂�
			�����Ƃ��قȂ鑤�ɂ���Ό������Ă��邱�ƂɂȂ�
		@retval	0		�������������Ȃ�
		@retval	1		��������������				
	*/
	int		Cross_Seg(
					line_seg	c_Seg	///< �Ώې���
				);
	/*
		�Q�_ c_pt1,c_pt2�ō\�����������Ɠ_c_src_pt�̋��������߂�
		@return	����(���K�����W�l)
	*/
	double	get_pt_seg_dist(
					Dpos	c_Pt		///< ���������߂�_
				);
	/*
		PointsDiet �Ŏg���Ă����̂�����
		�Q�_ c_pt1,c_pt2��ʂ钼���Ɠ_c_src_pt�̋��������߂�
		@return	����(���K�����W�l)
	*/
	double	get_pt_line_dist(
					Dpos	c_Pt		///< �Ώۓ_
				);
};

struct	Crect
{
	double	xmin;
	double	ymin;
	double	xmax;
	double	ymax;
	
	/*
		�R���X�g���N�^
	*/
	Crect	(	void	)
	{
		xmin	= 0.0;
		ymin	= 0.0;
		xmax	= 0.0;
		ymax	= 0.0;
	}
	
	/*
		��`���m�̌����`�F�b�N
		�O�ڒ����`�ŁA�����Ƀ`�F�b�N����
		@return	1 = ��`1�Ƌ�`2������
		@return ���� = 1 �O = 0
	*/
	int		Rect_Cross_Chk	(
					Crect	c_Rect		///< �Ώۋ�`
				);

	/*
		��`���ɋ�`�������邩�ǂ��� 2000.5.18
		�O�ڒ����`�ŁA�����Ƀ`�F�b�N����
		@return ��`2����`1�̓��� = 1 �O = 0
	*/
	int		Rect_Inner_Chk	(
					Crect	c_Rect		///< �Ώۋ�`
				);
	/*
		�O�ڒ����`�ŁA�����Ƀ`�F�b�N����
		@return �_������ = 1 �_���O = 0
	*/
	int		Point_Inner_Chk	(
					Dpos		c_Pt	///< �|�C���g
				);

};

struct	dvec
{
	double	x;
	double	y;
	dvec	()	{
		x	= 0;
		y	= 0;
	}
	~dvec	() {}
	/*
		�x�N�g���̊O�ς̒l�����߂�(��������)
		@return �Q�̂Q�����x�N�g���̊O�ς̒l
	*/
	int		get_gaiseki(
					dvec		c_Vec		///< vector
				);
	/*
		�^����ꂽ�Q�_�����ԃx�N�g�����쐬����
		@return �^����ꂽ�n�_�ƏI�_�̍��W����쐬���ꂽ�x�N�g��
	*/				
	void	get_vec(
					Dpos		pt0,		///< �x�N�g���̎n�_�̍��W
					Dpos		pt1			///< �x�N�g���̏I�_�̍��W
				);
	/*
		�x�N�g���̓��ς̒l�����߂�(��������)
		@return 0 = �Ȃ��p��90�x
		@return 1 = �Ȃ��p���s�p
		@return	-1 = �Ȃ��p���݊p
	*/
	double	get_naiseki(
					dvec		c_Vec		///< �Ώۃx�N�g��
				);
	/*
		�Q�̃x�N�g���̂Ȃ��p�����߂�
		@return	�p�x
	*/
	double	get_deg(
					dvec		c_Vec
				);
};

/*
	�_��ɑ΂��ĉ��炩�̏������s���N���X
*/
struct	poly_cls
{
		std::vector<Dpos>		e_Pts;			///< �_��
		std::vector<line_seg>	e_Arcs;			///< �A�[�N��

		/*
			�����_��̃N���A
		*/
		void	Clear_Pts() {
				e_Pts.clear();
		}

		/*
			�����A�[�N��̃N���A
		*/
		void	Clear_Arcs() {
				e_Arcs.clear();
		}
		
		/*
			�_��̉�]�i�J�n�ʒu��ς���j
			@return	0	����
			@return -1	���s
		*/
		int		Rot_Pts(
						int			c_Begin		///< �J�n�ʒu�ԍ�
					);
		/*
			�_��̔��]
			@return	0	����
			@return -1	���s
		*/
		int		Reverse_Pts();

		/*
			�_��̕���
			@return	0	����
			@return -1	���s
		*/
		int		Div_Pts(
						int			c_Begin,	///< �����n�_�ԍ�
						int			c_Num,		///< �n�_����̕�ԓ_��
						poly_cls*	c_Poly		///< �����_��i�[
					);
		/*
			�_�񂩂�A�A�[�N��֓����I�ɕϊ�
			@return	0	����
			@return -1	���s
		*/
		int		Make_Arcs();

		/*
			�A�[�N�񂩂�A�_��֓����I�ɕϊ�
		*/
		void	Make_Pts();

		/*
			�A�[�N���ڍ����ɕ��ׂ�
			@return	0	����
			@return -1	���s
		*/
		int		Arc_Connect(
						int			c_Begin		///< �ڍ��J�n�A�[�N�ԍ�
					);

		/*
			�A�[�N���A�[�N��ɑ}������
			@return	0	����
			@return -1	���s
		*/
		int		Arc_Insert(
						line_seg	c_Arc,		///< �}���A�[�N
						int			c_Num		///< �A�[�N�}���ʒu
					);

		/*
			�A�[�N����A�[�N��ɑ}������
			@return	0	����
			@return -1	���s
		*/
		int		Arcs_Insert(
						poly_cls&	c_Poly,		///< �}���A�[�N��
						int			c_Num		///< �A�[�N�}���ʒu
					);


		/*
			�|���S���_�񂩂�A�O�ڋ�`�̊l��
		*/
		int		getouterrect(
						Crect	*rect		///< �l����`
					);

		/*
			�Q�̐܂���̌�_�̌������߂�
			algorithm:
				�Q�̐܂�����\������������ׂĂ̑g�ݍ��킹�ɂ������� CrossSegment
				�ɂ�����������s�Ȃ��������Ă���ꍇ�̐������߂�

			@return	�Q�̐܂���̌�_�̌�
		*/
		int		crossline(
						poly_cls	c_Poly	///< �`�F�b�N�Ώۓ_��
					);
		
		/*
			PointsDiet �Ŏg���Ă����̂�����
			�_c_src_pt�ƁADpos *c_pts�ŕ\����郉�C���̋��������߂�
			@return ����(���K�����W�l)
		*/
		double	get_pt_arc_dist(
						Dpos	c_src_pt	///< ���������߂�_
					);

		/*
			�_��Ɠ_��̋��������߂�
			@return	�_�񓯎m�̋���
		*/
		double	Get_Poly_Dist(
						poly_cls	c_Poly	///< �Ώۓ_��
					);

		/*
			��`�̒��S���|���S�������ǂ����̃`�F�b�N
		*/
		int		iscenterin(
						Crect	*rect,		///< �`�F�b�N�Ώۋ�`
						Dpos	*RprPt		///< ��\�_
					);

		/*
			�|���S������`�F�b�N
			�|���S���P�̓����Ƀ|���S���Q�������邩�ǂ���
			@return	0  �|���S���͓����Ȃ�
			@return 1  �|���S���͓�����
		*/
		int		innerpolycheck(
						poly_cls	c_Poly	///< �`�F�b�N�Ώۃ|���S���_��
					);

		/*
			�^����ꂽ�_�����p�`�ɑ΂��Ăǂ��ɂ��邩�𒲂ׂ�
			@return	0	�_�͑��p�`�̊O���ɂ���
			@return	1	�_�͑��p�`�̓����ɂ���
			@return -1	�_�͑��p�`�̋��E��ɂ���
		*/
		int		innerpolygon(
						Dpos	c_Pt		///< ���ׂ�_�̍��W
					);
		/*
			�|���S���̑�\�_��T��
			@retval	0	��������
			@retval	1	������Ȃ�
		*/
		int		Find_Center(
						Dpos	*RprPt		///< ��\�_
					);

		/*
			�|���S���̖ʐ�
		*/
		double	calc_area(
						double	c_xratio,	///< x�����䗦?
						double	c_yratio	///< y�����䗦?
					);

		/*
			�����A�[�N��̍��W�l�̕\���i����m�F�ړI�j
		*/
		void	Print_Arcs( void )	{
			
			int	i = 0;
			std::vector<line_seg>::iterator	indx;
			printf("[count] : (head(x,y) - tail(x,y)\n");
			for(indx = e_Arcs.begin(); indx != e_Arcs.end(); indx++, i++)
			{
				line_seg	cur	= *indx;
				printf	("[%d] : (%0.5f, %0.5f - %0.5f, %0.5f)\n",
							i,
							cur.e_Head.xpos,
							cur.e_Head.ypos,
							cur.e_Tail.xpos,
							cur.e_Tail.ypos );
			}
		}
};

#endif	///<_POLY_CLASS_
