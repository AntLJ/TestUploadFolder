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

// ���̓����̃\�[�X�́A�؂蕨���قƂ�ǂł��E�E
#include "stdafx.h"
#include "poly_class.h"

//--------------------------------------------------------------------
//		output:	0:  �_�͑��p�`�̊O���ɂ���
//				1:  �_�͑��p�`�̓����ɂ���
//				-1: �_�͑��p�`�̋��E��ɂ���
//		function:
//				�^����ꂽ�_�����p�`�ɑ΂��Ăǂ��ɂ��邩�𒲂ׂ�
//--------------------------------------------------------------------
int		poly_cls::
innerpolygon	(	Dpos	c_Pt	)	//	���ׂ�_�̍��W
{
	int			i, ncross = 0;
//	int			maxy, miny;
	double		maxy, miny;
//	int			xint;
	double		xint;
	int			count	= e_Pts.size();

	for(i=0 ; i<count-1 ; ++i) {
		if(e_Pts[i].ypos == c_Pt.ypos) {
			if(e_Pts[i].xpos == c_Pt.xpos)
				return(-1);
			if(e_Pts[i].ypos == e_Pts[i+1].ypos) {
				if(e_Pts[i].xpos < e_Pts[i+1].xpos) {
					if((c_Pt.xpos>=e_Pts[i].xpos) && (c_Pt.xpos<=e_Pts[i+1].xpos)) return(-1);
				} else {
					if((c_Pt.xpos<=e_Pts[i].xpos) && (c_Pt.xpos>=e_Pts[i+1].xpos)) return(-1);
				}
			}
		}
	}

	for(i=0 ; i<count-1 ; ++i) {
		if(e_Pts[i].ypos < e_Pts[i+1].ypos) {
			maxy = e_Pts[i+1].ypos;
			miny = e_Pts[i].ypos;
		} else {
			maxy = e_Pts[i].ypos;
			miny = e_Pts[i+1].ypos;
		}
		if((e_Pts[i].ypos != e_Pts[i+1].ypos) &&
		  ((c_Pt.xpos<=e_Pts[i].xpos) || (c_Pt.xpos<=e_Pts[i+1].xpos)) &&
		  ((c_Pt.ypos<=maxy) && (c_Pt.ypos>=miny)) &&
		  (c_Pt.ypos != maxy)) {
			xint = e_Pts[i].xpos + (long) (c_Pt.ypos-e_Pts[i].ypos) * 
					(long) (e_Pts[i+1].xpos-e_Pts[i].xpos) / 
					(long) (e_Pts[i+1].ypos-e_Pts[i].ypos);
			if(c_Pt.xpos == xint) return(1);
			if(c_Pt.xpos < xint) ++ncross;
		}
	}

	return(ncross % 2);
}

//--------------------------------------------------------------------
// �|���S������`�F�b�N
//		output:	0:  �|���S���͓����Ȃ�
//				1:  �|���S���͓�����
//		function:
//				�|���S���P�̓����Ƀ|���S���Q�������邩�ǂ���
//--------------------------------------------------------------------
int		poly_cls::
innerpolycheck	(	poly_cls	c_Poly	)	// �Ώۓ_��
{
//	int	inner_cnt	= 0;
	int	count2		= c_Poly.e_Pts.size();

	// �Ƃ肠�����S�Ă̓_�������邩�̃`�F�b�N
	int	i;
	for(i = 0; i < count2; i++)
	{
		if(innerpolygon(c_Poly.e_Pts[i]) == 0)
			return(0);
	}
	// �_�񂪌������Ă���݂͂łĂ���
	if(crossline( c_Poly ) == 0) {
		return(1);	// �|���S���͓�����
	}else {
		return(0);
	}
}

//--------------------------------------------------------------------
//	int CrossLine(PointSet *line1, PointSet *line2)
//		output:	�Q�̐܂���̌�_�̌�
//		function:
//				�Q�̐܂���̌�_�̌������߂�
//		algorithm:
//				�Q�̐܂�����\������������ׂĂ̑g�ݍ��킹�ɂ������� CrossSegment
// 			�ɂ�����������s�Ȃ��������Ă���ꍇ�̐������߂�
//--------------------------------------------------------------------
int		poly_cls::
crossline(	poly_cls	c_Poly	)
{
	int			i, j;
	int			count;
	int			count1	= e_Pts.size();
	int			count2	= c_Poly.e_Pts.size();
	count = 0;
	for(i = 0 ; i < count1-1 ; i++)
	{
		for(j = 0 ; j < count2-1; j++)
		{
			Dpos		s1	= e_Pts[i];
			Dpos		e1	= e_Pts[i+1];
			Dpos		s2	= c_Poly.e_Pts[j];
			Dpos		e2	= c_Poly.e_Pts[j+1];
			line_seg	ls_1	(s1, e1);
			line_seg	ls_2	(s2, e2);

			count += ls_1.Cross_Seg(ls_2);
		}
	}

	return(count);
}

//-2000.5.30----------------------------------------------------------
// �|���S���̖ʐ�
//--------------------------------------------------------------------
/*
double get_real_area	(	Dpos	*e_Pts,		// �_��
							int		c_count,	// ��ԓ_��
							int		c_meshcode)	// ���b�V���R�[�h
{
	double		lon		= 0.0;
	double		lat		= 0.0;
	double		xratio	= 0.0;
	double		yratio	= 0.0;

	crd_cnv		crd;

	crd.MeshtoLL(c_meshcode, 0, 0, &lon, &lat);
	xratio	= RA * sin(M_PI / 2.0 - lat * M_PI / 180.0) * 2.0 * M_PI / 23040.0 / 1000000.0; // 23040 = 360*64
	yratio	= RA * 2.0 * M_PI / 34560.0 / 1000000.0;
	return(calc_area(e_Pts, c_count, xratio, yratio));
}
*/

//-2000.5.30----------------------------------------------------------
// �|���S���̖ʐ�
//--------------------------------------------------------------------
double		poly_cls::
calc_area	(	double	c_xratio,	// x�����䗦?
				double	c_yratio)	// y�����䗦?
{
	double	S		= 0.0;
	int		i;
	int		s		= 0;
	int		e		= 0;
	int		count	= e_Pts.size();
	for(i = 0; i < count - 1; i++)
	{
		s	= (i == 0) ? count-2: i-1;
		e	= (i == count-2) ? 0: i+1;
		S	+=((e_Pts[s].xpos - e_Pts[e].xpos) * c_xratio * e_Pts[i].ypos * c_yratio);
	}
	S /= (2.0);
	return(fabs(S));
}

//--------------------------------------------------------------------
// �|���S���̑�\�_��T��
//--------------------------------------------------------------------
int		poly_cls::
Find_Center	(	Dpos	*RprPt)	// ��\�_
{
    Crect	rect;
    int		iRet;
//  int		DivX, DivY;
	double	DivX, DivY;
//  int		BaseX, BaseY;
	double	BaseX, BaseY;
	int		i, j;
    int		DivCount	= 1;
//	int		count		= e_Pts.size();
    
    if((iRet = getouterrect( &rect ))){
        BaseX	= rect.xmin;
        BaseY	= rect.ymin;
        DivX	= rect.xmax - rect.xmin;
        DivY	= rect.ymax - rect.ymin;
        iRet = 0;

//      while(iRet != 1 && DivCount < 1024 && DivX != 0 && DivY != 0){
		while(iRet != 1 && DivCount < 1024 && DivX != 0.0 && DivY != 0.0){

            for(i = 0; iRet != 1 && i < DivCount; i++){
                rect.ymin	= BaseY + DivY * i;		// 1999.6.24 X->Y
                rect.ymax	= rect.ymin + DivY;
                for(j = 0; iRet != 1 && j < DivCount; j++){
                    rect.xmin	= BaseX + DivX * j;
                    rect.xmax	= rect.xmin + DivX;
                    iRet = iscenterin( &rect, RprPt );
                }
            }
            DivX /= 2;
            DivY /= 2;
            DivCount *= 2;	// 1999.6.24 4->2
        }
		if(iRet == 0){
			RprPt->xpos	= (double)e_Pts[0].xpos;
			RprPt->ypos	= (double)e_Pts[0].ypos;
		}
    }
    return iRet;
}

//--------------------------------------------------------------------
// ��`�̒��S���|���S�������ǂ����̃`�F�b�N
//--------------------------------------------------------------------
int		poly_cls::
iscenterin	(	Crect	*rect,	// �`�F�b�N�Ώۋ�`
				Dpos	*RprPt)	// ��\�_
{
    int iRet;
	Dpos tmp;
//	int	count	= e_Pts.size();

    RprPt->xpos	= (double)(rect->xmin + rect->xmax) / 2.0;
    RprPt->ypos	= (double)(rect->ymin + rect->ymax) / 2.0;
	tmp.xpos	= (int)RprPt->xpos;
	tmp.ypos	= (int)RprPt->ypos;
    if(innerpolygon( tmp ) == 1)
        iRet = 1;
    else
        iRet = 0;
    
    return iRet;
}

//--------------------------------------------------------------------
//	�|���S���_�񂩂�A�O�ڋ�`�̊l��
//--------------------------------------------------------------------
int		poly_cls::
getouterrect(	Crect	*rect)	//	�l����`
{
    int i;
    int iRet	= 0;
	int	count	= e_Pts.size(); 
    if(count > 2){
		rect->xmin	= e_Pts[0].xpos;
		rect->ymin	= e_Pts[0].ypos;
		rect->xmax	= e_Pts[0].xpos;
		rect->ymax	= e_Pts[0].ypos;
        for(i = 1; i < count; i++){
            if(e_Pts[i].xpos < rect->xmin)
                rect->xmin	= e_Pts[i].xpos;
            else if(e_Pts[i].xpos > rect->xmax)
                rect->xmax	= e_Pts[i].xpos;
            if(e_Pts[i].ypos < rect->ymin)
                rect->ymin	= e_Pts[i].ypos;
            else if(e_Pts[i].ypos > rect->ymax)
                rect->ymax	= e_Pts[i].ypos;
        }
        iRet = 1;
    }
    return iRet;
}

//--------------------------------------------------------------------
// 2001.10.03 PointsDiet �Ŏg���Ă����̂�����
// �_c_Pt�ƁADpos *e_Pts�ŕ\����郉�C���̋��������߂�
// �Ԃ�l	����(���K�����W�l)
//--------------------------------------------------------------------
double		poly_cls::
get_pt_arc_dist	(	Dpos		c_Pt	)	// ���������߂�_
{
	double	dist		= 0.0;
	double	cur_dist	= 0.0;
	int		i			= 0;
	int		count		= e_Pts.size();

	if(count == 1) {
		dist	= sqrt(pow(2.0, (e_Pts[0].xpos - c_Pt.xpos)) +
					pow(2.0, (e_Pts[0].ypos - c_Pt.ypos)));
		return	(dist);
	}else
	if(count >= 2) {
		for(i = 0; i < count-1; i++)
		{
			line_seg	cur_seg( e_Pts[i], e_Pts[i+1] );
			cur_dist	= cur_seg.get_pt_seg_dist(c_Pt);
			if(i == 0) {
				dist	= cur_dist;
			}else {
				if(cur_dist < dist) {
					dist	= cur_dist;
				}
			}
		}
	}else {		// �G���[�̏ꍇ
		return	(-1.0);
	}
	return	(dist);
}

/*
	�_��Ɠ_��̋��������߂�
	@return	�_�񓯎m�̋���
*/
double		poly_cls::
Get_Poly_Dist(	poly_cls	c_Poly	)	// �Ώۓ_��
{
	int	count			= e_Pts.size();
	double	a_dist		= 0.0;
	double	a_nearest	= 0.0;	// �ł��߂�����
	int		i			= 0;

	for( i = 0; i < count; i++)
	{
		a_dist	= 0.0;	// �O�̂��ߖ��񏉊���
		a_dist	= get_pt_arc_dist(c_Poly.e_Pts[i]);
		if(i == 0) {	
			a_nearest	= a_dist;
			continue;
		}
		if(a_dist < a_nearest) {
			a_nearest	= a_dist;
		}
	}
	return	( a_nearest );
}

/*
	�_��̉�]�i�J�n�ʒu��ς���j
*/
int		poly_cls::	
Rot_Pts(	int	c_Begin	)	// �J�n�ʒu�ԍ�[IN] 0origin
{
	std::vector<Dpos>	aTmpPts;	// �e���|����

	int	count	= e_Pts.size();
	int	i		= 0;
	if(c_Begin < 0 || c_Begin >= count) {	// �s���Ȓl�͉��������A-1��Ԃ�
		return	(-1);
	}
	// �J�n�ʒu����Ō�܂ł���荞��
	for(i = c_Begin; i < count; i++)
	{
		Dpos	cur	= e_Pts[i];
		aTmpPts.push_back( cur );
	}
	// �ŏ�����J�n�ʒu�O�܂ŕ��荞��
	for(i = 0; i < c_Begin; i++)
	{
		Dpos	cur = e_Pts[i];
		aTmpPts.push_back( cur);
	}
	// ���̃f�[�^���㏑��
	for(i = 0; i < count; i++)
	{
		e_Pts[i]	= aTmpPts[i];
	}
	return	(0);
}

/*
	�_��̔��]
	@return	0	����
	@return -1	���s
*/
int		poly_cls::
Reverse_Pts()
{
	std::vector<Dpos>	aTmpPts;	// �e���|����

	int	count	= e_Pts.size();
	int	i		= 0;
	if(count == 0) {				// �_�������Ȃ牽�������A-1��Ԃ�
		return	(-1);
	}
	// �Ōォ�珇�Ɏ��o��
	for(i = count-1; i >= 0; i--)
	{
		Dpos	cur	= e_Pts[i];
		aTmpPts.push_back( cur );
	}
	// ���̃f�[�^���㏑��
	for(i = 0; i < count; i++)
	{
		e_Pts[i]	= aTmpPts[i];
	}
	return	(0);
}

/*
	�_��̕���
*/
int		poly_cls::
Div_Pts(	int			c_Begin,	// �����n�_�ԍ�
			int			c_Num,		// �n�_����̕�ԓ_��
			poly_cls*	c_Poly )	// �����_��i�[
{
	if(c_Begin < 0) {
		return	(-1);
	}
	int	count	= e_Pts.size();		// ��ԓ_��
	int	max		= c_Begin + c_Num;
	if( max > count) {
		return	(-1);
	}
	int	i = 0;
	for(i = c_Begin; i < max; i++)
	{
		Dpos	cur	= e_Pts[i];
		c_Poly->e_Pts.push_back( cur );
	}
	return	(0);
}

/*
	�_�񂩂�A�A�[�N��֓����I�ɕϊ�
*/
int		poly_cls::
Make_Arcs(	void	)
{
	int	count	= e_Pts.size();

	if(count < 2) {
		return	(-1);
	}

	e_Arcs.clear();		// ����������

	int	i		= 0;
	for(i = 0; i < count-1; i++)
	{
		line_seg	cur(e_Pts[i], e_Pts[i+1]);
		e_Arcs.push_back(cur);
	}
	return	(0);
}

/*
	�A�[�N�񂩂�A�_��֓����I�ɕϊ�
	line_seg �́@e_Head��e_Tail����v���Ă��邱�Ƃ�
	�O��ƂȂ�
*/
void		poly_cls::
Make_Pts(	void	)
{
	e_Pts.clear();		// ���������Ă���

	std::vector<line_seg>::iterator		indx;
	for(indx = e_Arcs.begin(); indx != e_Arcs.end(); indx++)
	{
		line_seg	cur	= *indx;
		if(indx == e_Arcs.begin()) {	// �ŏ������n�_�ƏI�_������
			e_Pts.push_back( cur.e_Head );
			e_Pts.push_back( cur.e_Tail );
		}else {							// ���Ƃ͏I�_���������Ă���
			e_Pts.push_back( cur.e_Tail );
		}
	}
}

/*
	�A�[�N���A�[�N��ɑ}������
	@return	0	����
	@return -1	���s
*/
int		poly_cls::
Arc_Insert(	line_seg	c_Arc,	///< �}���A�[�N
			int			c_Num )	///< �A�[�N�}���ʒu
{
	if(c_Num < 0) {
		return	(-1);
	}

	//vector<line_seg>	a_TmpArcs;	// �A�[�N��

	int	count	= e_Arcs.size();		// �A�[�N��(��ԓ_��-1�j
	if( c_Num > (count-1)) {
		return	(-1);
	}

	e_Arcs.insert(e_Arcs.begin()+c_Num, c_Arc);

	/*
	// �}���ʒu�̑O�܂ł��e���|�����ɃR�s�[
	int	i	= 0;
	for(i = 0; i < c_Num; i++)
	{
		line_seg	cur	= e_Arcs[i];
		a_TmpArcs.push_back( cur );
	}
	// �Y���A�[�N���R�s�[
	a_TmpArcs.push_back( c_Arc );
	
	// �c����R�s�[
	for(i = c_Num; i < count; i++)
	{
		line_seg	cur	= e_Arcs[i];
		a_TmpArcs.push_back( cur );
	}

	// ���̃f�[�^���������ď㏑��
	e_Arcs.clear();
	vector<line_seg>::iterator	indx;
	for(indx = a_TmpArcs.begin(); indx != a_TmpArcs.end(); indx++)
	{
		e_Arcs.push_back( *indx );
	}*/
	return	(0);
}

/*
	�A�[�N����A�[�N��ɑ}������
	@return	0	����
	@return -1	���s
*/
int		poly_cls::
Arcs_Insert(	poly_cls&	c_Poly,		///< �}���A�[�N��
				int			c_Num )		///< �A�[�N�}���ʒu
{
	if(c_Num < 0) {
		return	(-1);
	}

	std::vector<line_seg>	a_TmpArcs;	// �A�[�N��

	int	count	= e_Arcs.size();		// �A�[�N��(��ԓ_��-1�j
	if( c_Num > (count-1)) {
		return	(-1);
	}
	e_Arcs.insert(e_Arcs.begin()+c_Num, c_Poly.e_Arcs.begin(), c_Poly.e_Arcs.end());

	return	(0);
}

/*
	�����_���ȃA�[�N���ڍ����ɕ��ׂ�i���̓_������������Ă��܂��j
	@return	0	����
	@return -1	���s
*/
int		poly_cls::
Arc_Connect(	int	c_Begin	)		///< �ڍ��J�n�A�[�N�ԍ�
{
	if(c_Begin < 0) {
		return	(-1);
	}

	int	count	= (int)e_Arcs.size();
	if( c_Begin > (count-1)) {
		return	(-1);
	}
	// 2003.7.29 �T�[�`�p�̃f�[�^
	line_seg	a_lBegin_Seg	= e_Arcs[c_Begin];
	
	// 2003.7.29 �ڍ��A�[�N�̍�����
	std::multimap < Dpos, line_seg, std::less <Dpos> >	a_SrcArcs;
	std::list<line_seg>									a_TmpArcs;
	std::vector<line_seg>::iterator	indx;
	
	// �e���|�����Ƀf�[�^���R�s�[
	int	a_iNo_Add_Count	= 0;	// �J�n�����Ɠ��������͈�͍폜���Ă���
	for(indx = e_Arcs.begin(); indx != e_Arcs.end(); indx++)
	{
		line_seg	a_lTmp	= *indx;
		Dpos		a_dKey	= a_lTmp.e_Head;	// �n�_�Ń\�[�g
		if( a_iNo_Add_Count == 0 && a_lTmp == a_lBegin_Seg ) {
			a_iNo_Add_Count++;
			continue;
		}
		a_SrcArcs.insert(std::multimap < Dpos, line_seg, std::less <Dpos> >::value_type( a_dKey, a_lTmp ));
	}

	std::list<line_seg>::iterator	indx_tmp;

	line_seg	ls_tmp	= a_lBegin_Seg;
	a_TmpArcs.push_back	( ls_tmp );	// �J�n�ʒu�̃f�[�^���e���|�����Ɋi�[

	std::multimap< Dpos, line_seg, std::less <Dpos> >::iterator	indx2;
	for(indx_tmp = a_TmpArcs.begin(); (int)a_TmpArcs.size() != count; indx_tmp++)	// �f�[�^���S���͂�����I���
	{
		line_seg	cur_arc		= (*indx_tmp);

		std::multimap< Dpos, line_seg, std::less <Dpos> >	a_ChkArcs;

		// �ڑ��A�[�N�̃`�F�b�N���[�v
		int		a_iSame_Count	= a_SrcArcs.count ( cur_arc.e_Tail );
		int i = 0;
	//	for( indx2 = a_SrcArcs.lower_bound ( cur_arc.e_Tail ); i < a_iSame_Count; i++ )	// ���݃A�[�N�̏I�_�ƁA�T�[�`���̃A�[�N�̎n�_����v		
		for( indx2 = a_SrcArcs.lower_bound ( cur_arc.e_Tail ); i < a_iSame_Count; i++ )	// ���݃A�[�N�̏I�_�ƁA�T�[�`���̃A�[�N�̎n�_����v
		{
			line_seg	srch_arc	= (*indx2).second;	// �T�[�`���̃A�[�N
			// �`�F�b�N�p�e���|�����ɕ��荞��
			a_ChkArcs.insert ( std::multimap < Dpos, line_seg, std::less <Dpos> >::value_type( srch_arc.e_Head, srch_arc ));
			a_SrcArcs.erase (indx2++);	// �f�[�^�����������
		}

		//�ڑ��A�[�N�ɂ��Ă̏���
		if(a_ChkArcs.size() == 1) {	// �ΏۃA�[�N���ЂƂ����Ȃ��ꍇ�́Aa_TmpArcs�փR�s�[����
			line_seg	tmp = (*a_ChkArcs.begin()).second;
			a_TmpArcs.push_back(tmp);
		}else
		if(a_ChkArcs.size() > 1) {	// �ΏۃA�[�N����������ꍇ
			int		i		= 0;
			double	degree	= 0.0;	// �p�x

			std::multimap < Dpos, line_seg, std::less <Dpos> >::iterator	chk_indx;
			std::multimap < Dpos, line_seg, std::less <Dpos> >::iterator	del_indx;
			for(chk_indx = a_ChkArcs.begin(); chk_indx != a_ChkArcs.end(); chk_indx++, i++)
			{
				// �Q�̃x�N�g���̂Ȃ��p���`�F�b�N����
				line_seg	chk_arc		= (*chk_indx).second;
				double		cur_deg	= 0.0;
				dvec		v1, v2;
				v1.get_vec	(cur_arc.e_Head, cur_arc.e_Tail);	// �����Ă���A�[�N
				v2.get_vec	(chk_arc.e_Head, chk_arc.e_Tail);	// �ڑ�����A�[�N
				cur_deg		= v1.get_deg( v2 );
				
				if(chk_indx == a_ChkArcs.begin()) {	// ���[�v�̍ŏ��͏���������
					degree		= cur_deg;
					del_indx	= chk_indx;
				}else {
					if(	!(cur_arc.e_Head == chk_arc.e_Tail &&
						  cur_arc.e_Tail == chk_arc.e_Head)) {	// ���S�t�����̃A�[�N�́A�Ώۂ���͂���
						if(cur_deg > degree ) {	// ���p�x�̑傫�������Ƃ�
							degree		= cur_deg;
							del_indx	= chk_indx;
						}
					}
				}
			}

			line_seg	a_lDel_Seg	= (*del_indx).second;
			a_TmpArcs.push_back	(a_lDel_Seg);	// a_TmpArcs�փR�s�[
			a_ChkArcs.erase		(del_indx);	// a_TmpArcs�֊i�[�����f�[�^�͏���
		//	a_SrcArcs.erase		(del_indx);		
			// �ΏۊO�������f�[�^���Aa_SrcArcs�ɖ߂��āA�Ăу`�F�b�N

			for(chk_indx = a_ChkArcs.begin(); chk_indx != a_ChkArcs.end(); chk_indx++)
			{
				line_seg	a_lReload	= (*chk_indx).second;
				a_SrcArcs.insert ( std::multimap < Dpos, line_seg, std::less <Dpos> >::value_type( a_lReload.e_Head, a_lReload ));
			}

		}else{	// �A�[�N���݂���Ȃ������ꍇ�́A������
			return	(-1);
		}
	}

	// �R�s�[����
	e_Arcs.clear();
	for(indx_tmp = a_TmpArcs.begin(); indx_tmp != a_TmpArcs.end(); indx_tmp++)
	{
	//	line_seg	a_lCopy	= (*indx_tmp);
	//	e_Arcs
		e_Arcs.push_back( *indx_tmp );
	}
	return	(0);
}
