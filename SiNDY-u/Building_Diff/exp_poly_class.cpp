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

#include "exp_poly_class.h"

/**
------------------------------------------------
	�W�I���g���̓_����m�ۂ���
	@return	����	WKSPoint�_��ւ̃|�C���^
	@return	���s	NULL�|�C���^
------------------------------------------------
*/
WKSPoint*	exp_poly_class::
Get_GeoPts( IGeometryPtr	c_ipGeo, 		///< �W�I���g��[IN]
		    long&			c_lCount )		///< ��ԓ_��[OUT]
{
	HRESULT					hr;
	IPointCollectionPtr		a_ipPtsArray( c_ipGeo );

	long					a_lCount	= 0;
	a_ipPtsArray->get_PointCount( &a_lCount );
	if( a_lCount == 0 ) {
		c_lCount	= 0;
		return	( NULL );
	}

	WKSPoint*	a_wpPoints;
	a_wpPoints	= new WKSPoint[a_lCount];	// �_��̊m��
	hr	= a_ipPtsArray->QueryWKSPoints ( 0, a_lCount, a_wpPoints );
	if ( FAILED( hr ) ){
		c_lCount	= 0;
		return		( NULL );
	}
	c_lCount	= a_lCount;
	return	( a_wpPoints );
}

/**
------------------------------------------------
	�W�I���g�����΂炯���_��ɗ��Ƃ�����	
	@return	0	����
	@return -1	���s
------------------------------------------------
*/
int		exp_poly_class::
Set_GeotoPts( IGeometryPtr	c_ipGeo, 	///< �W�I���g��[IN]
			  int			c_iID )		///< ID[IN]
{

	long		a_lCount	= 0;
	WKSPoint*	a_wpPoints	= NULL;
	a_wpPoints	= Get_GeoPts ( c_ipGeo, a_lCount );
	if( a_wpPoints == NULL ) {
		return	( -1 );
	}

	//�@�_��̃R�s�[
	int i = 0;
	for(i = 0; i < a_lCount; i++)
	{
		Dpos2		a_dPt;

		a_dPt.xpos	= a_wpPoints[i].X;
		a_dPt.ypos	= a_wpPoints[i].Y;
		a_dPt.e_indx= c_iID;
	
		// �A�[�N���ǉ�
		e_sExpPts.insert ( a_dPt );
	}
	delete [] a_wpPoints;
	return	( 0 );
}

/**
------------------------------------------------
	�W�I���g�����΂炯���A�[�N��i�x�N�^�j�ɗ��Ƃ�����	
	@return	0	����
	@return -1	���s
------------------------------------------------
*/
int		exp_poly_class::
Set_GeotoArcs( IGeometryPtr	c_ipGeo, 	///< �W�I���g��[IN]
			   int			c_iID )		///< ID[IN]
{
	long		a_lCount	= 0;
	WKSPoint*	a_wpPoints	= NULL;
	a_wpPoints	= Get_GeoPts ( c_ipGeo, a_lCount );
	if( a_wpPoints == NULL ) {
		return	( -1 );
	}

	//�@�_��̃R�s�[
	int i = 0;
	for(i = 0; i < a_lCount-1; i++)
	{
		Dpos		a_dBegin;
		Dpos		a_dEnd;

		a_dBegin.xpos	= a_wpPoints[i].X;
		a_dBegin.ypos	= a_wpPoints[i].Y;
		a_dEnd.xpos		= a_wpPoints[i+1].X;
		a_dEnd.ypos		= a_wpPoints[i+1].Y;

		line_seg2		cur_arc;
		cur_arc.e_Head	= a_dBegin;
		cur_arc.e_Tail	= a_dEnd;
		cur_arc.e_iIndx	= c_iID;
		// �A�[�N���ǉ�
		e_vExpArcs.push_back(cur_arc);
	//	e_msExpArcs.insert(cur_arc);
	}
	delete [] a_wpPoints;
	return	( 0 );
}

/**
------------------------------------------------
	�A�[�N�ɍł��߂��_�����o��
	@return	0	���o������
	@return	1	�Y���f�[�^����
------------------------------------------------
*/
int		exp_poly_class::
Get_NearestPt( line_seg2		c_lSeg,			///< �A�[�N�̐���[IN]
			   double			c_lDeg, 		///< 臒l�i�x�j[IN}
			   vector<Dpos2>&	c_msErrPts )	///< �G���[�ƂȂ�_�̃R���e�i[OUT]
{
	Dpos2	a_dMinPt, a_dMaxPt;
	// �ő�l�A�ŏ��l�̎��o��
	if( c_lSeg.e_Head.xpos < c_lSeg.e_Tail.xpos ) {
		a_dMinPt.xpos	= c_lSeg.e_Head.xpos;
		a_dMaxPt.xpos	= c_lSeg.e_Tail.xpos;
	}else {
		a_dMinPt.xpos	= c_lSeg.e_Tail.xpos;
		a_dMaxPt.xpos	= c_lSeg.e_Head.xpos;
	}
	if( c_lSeg.e_Head.ypos < c_lSeg.e_Tail.ypos ) {
		a_dMinPt.ypos	= c_lSeg.e_Head.ypos;
		a_dMaxPt.ypos	= c_lSeg.e_Tail.ypos;
	}else {
		a_dMinPt.ypos	= c_lSeg.e_Tail.ypos;
		a_dMaxPt.ypos	= c_lSeg.e_Head.ypos;
	}
	a_dMinPt.e_indx	= 0;
	a_dMaxPt.e_indx	= 1000000000;

	// 臒l���ŏ��A�ő�l�ɉ�������
	a_dMinPt.xpos -= c_lDeg;
	a_dMinPt.ypos -= c_lDeg;
	a_dMaxPt.xpos += c_lDeg;
	a_dMaxPt.ypos += c_lDeg;

	multiset<Dpos2>::iterator	a_iBegin_Indx, a_iEnd_Indx, a_iIndx, a_iChk;

	a_iBegin_Indx	= e_sExpPts.lower_bound ( a_dMinPt );
	a_iEnd_Indx		= e_sExpPts.upper_bound ( a_dMaxPt );
	// �G���[�|�C���g�̃N���A
	c_msErrPts.clear();
int cnt=0;
	for( a_iIndx = a_iBegin_Indx; ; a_iIndx++,cnt++ )
	{
		Dpos2	a_dCurPt	= *a_iIndx;
		// �_�ƃA�[�N�̋����v�Z�̌��ʁA臒l�ȉ��Ȃ�G���[�|�C���g�Ƃ��Ēǉ�����
		double	a_dDist		= 0;
		Dpos tmp; tmp.xpos = a_dCurPt.xpos; tmp.ypos = a_dCurPt.ypos;
		a_dDist	= c_lSeg.get_pt_seg_dist ( a_dCurPt );
	//	a_dDist	= c_lSeg.get_pt_line_dist ( tmp );
	//	if( a_dDist != 0.0 && a_dDist <= c_lDeg ) {
		if( a_dDist <= c_lDeg ) {	
			c_msErrPts.push_back ( a_dCurPt );
		}
		if( a_iIndx == a_iEnd_Indx ) break;
	}
printf("match = %d\n", cnt );
	if( c_msErrPts.size() > 0 ) {
		return ( 0 );
	}
	return ( -1 );
}

/**
	�_�ɍł��߂��A�[�N�����o��
	@return	0	���o������
	@return	1	�Y���f�[�^����
*/
int		exp_poly_class::
Get_NearestArcs(	Dpos2				c_lPt,			///< �_[IN]
					double				c_lDeg,			///< 臒l�i�x�j[IN}
					vector<line_seg2>&	c_msErrPts	)	///< �G���[�ƂȂ�A�[�N�̃Z�b�g[OUT]
{
	line_seg2	a_dBegin, a_dEnd;
	// �ő�l�A�ŏ��l�̎��o��
	Dpos2	a_dBPt, a_dEPt;
	a_dBPt	= c_lPt;
	a_dEPt	= c_lPt;
	a_dBPt.xpos	-= c_lDeg;
	a_dBPt.ypos	-= c_lDeg;
	a_dEPt.xpos	+= c_lDeg;
	a_dEPt.ypos	+= c_lDeg;

	a_dBegin.Data_Set	( a_dBPt, a_dBPt );
	a_dEnd.Data_Set		( a_dEPt, a_dEPt );
	a_dBegin.e_iIndx	= 1000000000;
	a_dEnd.e_iIndx		= 0;

	// 臒l���ŏ��A�ő�l�ɉ�������

/*	multiset<line_seg2>::iterator	a_iBegin_Indx, a_iEnd_Indx, a_iIndx;
//	vector<line_seg2>::iterator	a_iIndx;
	a_iBegin_Indx	= e_msExpArcs.lower_bound ( a_dBegin );
	a_iEnd_Indx		= e_msExpArcs.upper_bound ( a_dEnd );

	// �G���[�|�C���g�̃N���A
	c_msErrPts.clear();

//	for( a_iIndx = a_iBegin_Indx; a_iIndx != a_iEnd_Indx; a_iIndx++ )
	for( a_iIndx = a_iBegin_Indx; ; a_iIndx++ )
//	for( a_iIndx = e_vExpArcs.begin();a_iIndx != e_vExpArcs.end() ; a_iIndx++ )
	{
		line_seg2	a_dCurSeg	= *a_iIndx;
		// �_�ƃA�[�N�̋����v�Z�̌��ʁA臒l�ȉ��Ȃ�G���[�|�C���g�Ƃ��Ēǉ�����
		double	a_dDist		= 0;
		a_dDist	= a_dCurSeg.get_pt_seg_dist ( c_lPt );
		if( a_dDist != 0.0 && a_dDist <= c_lDeg ) {
			if( a_dDist == 0.0 ) {
			printf( "%0.10f\n", a_dDist );
			}
			c_msErrPts.push_back ( a_dCurSeg );
			break;
		}
		if( a_iIndx == a_iEnd_Indx ) {
			break;
		}
	}
	if( c_msErrPts.size() > 0 ) {
		return ( 0 );
	}*/
	return ( -1 );
}

double		line_seg2::
get_pt_seg_dist	(	Dpos2		c_src_pt	)	// ���������߂�_
{
	double	dist	= 0.0;

	dvec	v1, v2;			// �x�N�^���̂P�A���̂Q
	dvec	vs1, vs2;		// �����\���_�ō\�������x�N�^
	double	nai_1	= 0;	// ���ς��̂P
	double	nai_2	= 0;	// ���ς��̂Q

	Dpos	a_dPos;
	a_dPos.xpos	= c_src_pt.xpos;
	a_dPos.ypos	= c_src_pt.ypos;

	v1.get_vec	(e_Head, a_dPos);	// ���W�_�P�ƒ��ړ_�̃x�N�g��
	v2.get_vec	(e_Tail, a_dPos);	// ���W�_�Q�ƒ��ړ_�̃x�N�g��
	vs1.get_vec	(e_Head, e_Tail);		// ���W�_�P�ƍ��W�_�Q�̃x�N�g��
	vs2.get_vec	(e_Tail, e_Head);		// ���W�_�Q�ƍ��W�_�P�̃x�N�g��

	nai_1	= v1.get_naiseki	( vs1 );	// ���όv�Z
	nai_2	= v2.get_naiseki	( vs2 );	//  �V

	if(nai_1 > 0.0 && nai_2 > 0.0) {		// �Ȃ��p�������Ƃ��s�p(�����̊Ԃɒ���_������)
		line_seg	cur_seg(e_Head, e_Tail);
		dist		= cur_seg.get_pt_line_dist(a_dPos);
	}else {	// �ǂ��������݊p�ɂȂ�(�����̊Ԃɒ���_�����Ȃ�)
		double	td1	= 0.0;
		double	td2	= 0.0;
		td1		= a_dPos.Get_Dist( e_Head );
		td2		= a_dPos.Get_Dist( e_Tail );
		if(td1 < td2) {
			dist	= td1;
		}else {
			dist	= td2;
		}
	}
	return(dist);
}
