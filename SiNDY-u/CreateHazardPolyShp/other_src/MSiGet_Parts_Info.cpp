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

#include "MSiGet_Parts_Info.h"


/**
	�G���[�v�����g
	@return	����
*/
void	msi_get_parts_info::
Error_Print	(	char*	c_cpErr_Code,		///< �G���[�R�[�h
				char*	c_cpErr_Str		)	///< �o�̓G���[������
{
	fprintf	( e_fpLog,	"0\t%s\t%d\t\t\tERROR\t%s\t%s\t%d\n",
					
						&e_sLayer_Name[0],	// ���C������
						e_lObj_ID,			// �I�u�W�F�N�gID
						c_cpErr_Code,		// �G���[�R�[�h
						c_cpErr_Str,
						e_iMeshCode );		// ���b�V���R�[�h 2003.6.23		// �o�̓G���[������						
}

/**
	������(���̂Ƃ��ɏ������o��)
	���W�I���g���̃N���[�����쐬���ĕ�������
	@return	�Ȃ�
*/
bool	msi_get_parts_info::
Init(	IGeometryPtr	c_ipGeo,		///< �W�I���g��
		long			c_lObj_ID,		///< �I�u�W�F�N�gID
		string			c_sLayer_Name,	///< ���C������
		int				c_iMeshCode,	///< ���b�V���R�[�h
		FILE*			c_fpLog,		///< ���O�t�@�C���p
		int				c_iMode	)		///< �č\�����[�h�̐ݒ�i�f�t�H���g�͍č\�����Ȃ��j
{
	//�@query�Ń����O���������́A���̍\��������悤�Ȃ̂ŁA�R�s�[���Ă����A����
	//�@�󂳂Ȃ��悤�ɂ���B
	IClonePtr	ipClone;
	((IClonePtr)c_ipGeo)->Clone( &ipClone );
	
	e_ipGeo			= (IGeometryPtr)ipClone;

	e_lObj_ID		= c_lObj_ID;
	e_sLayer_Name	= c_sLayer_Name;

	e_iMeshCode		= c_iMeshCode;
	e_fpLog			= c_fpLog;

	HRESULT			hr;

	if( FAILED(	c_ipGeo->get_GeometryType( &e_eGeoType )) ) {
		Error_Print ( "Init_01", "�W�I���g���^�C�v���Ƃ�܂���" );
		return		( false );
	}

	long			col_num	= 0;	// �R���N�V�����̐�

	e_ipGeoCol		= e_ipGeo;		// �W�I���g���R���N�V�����ɓ���Ă݂�
	
	if( FAILED( e_ipGeoCol->get_GeometryCount( &col_num )) ) {
		Error_Print ( "Init_02", "�W�I���g���J�E���g���Ƃ�܂���" );
		return		( false );
	}
#ifdef	_CHECK_
	if( col_num >= 2 ) {
		printf("col_num = %d\n", col_num );
	}
#endif
	if( e_eGeoType != esriGeometryPolygon ) {	// �|���S���ȊO�̂Ƃ�

		e_lPart_Num		= col_num;		// �p�[�g�̐��́A���̂܂܃R���N�V�����̐�
		e_lSubPart_Max	= 1;			// �T�u�p�[�g�̍ő�l��1�Ƃ���

	}else {		// �|���S���̏ꍇ

		// �����Ŋ���Init����Ă����Ƃ��́A������ĐV���Ƀ������m��
		Pre_Init_Chk ();
	
		e_ipPolygon		= e_ipGeo;

		// 2003.08.21�ǉ� ---------------------------------------------------------
		IPointCollectionPtr		a_ipTmpPts( e_ipPolygon );

		long	a_ipPtsCount = 0;	// ����ԓ_��

		hr	= a_ipTmpPts->get_PointCount ( &a_ipPtsCount );
		if ( FAILED( hr ) ) {
			Error_Print ( "Init_02", "get_PointCount�Ɏ��s" );
			return		( false );
		}

		//debug -----------------------
		double	tmpx, tmpy;
		IPointPtr	a_ipTmpPt;
		a_ipTmpPts->get_Point ( 0, &a_ipTmpPt );
		a_ipTmpPt->get_X( &tmpx );
		a_ipTmpPt->get_Y( &tmpy );		

		//--------------------------------
	
		e_wpPoints	= new WKSPoint[a_ipPtsCount];	// �_��̊m��
		hr	= a_ipTmpPts->QueryWKSPoints ( 0, a_ipPtsCount, e_wpPoints );
		if ( FAILED( hr ) ){
			Error_Print ( "Init_02", "WKSPoint�擾�Ɏ��s" );
			return		( false );
		}

		// ����������
		InitRing( a_ipPtsCount, e_wpPoints, e_lRings );
		// �_��̕�܊֌W���Ƃ��Ă���
		CheckExteriorInterior( e_wpPoints, e_lRings );
		if( c_iMode == GPI_RECONFIG_MODE ) {	// �|���S���̏�Ԃ����Ă����ōč\������
			if( Reconfig_Polygon() ) {		// �|���S���č\���@2003.10.25
				Error_Print ( "RECONFIG", "�|���S���̏�Ԃ�����łȂ��̂ōč\�����܂�" );
			}
		}
		// �A�E�^�[�����O���̎擾
		list<RING>::iterator	a_iIndx;
		for( a_iIndx = e_lRings.begin(); a_iIndx != e_lRings.end(); a_iIndx++ )
		{
			e_vChk_Rings.push_back(*a_iIndx);	//�`�F�b�N�p�ɕ��荞�ށi���X�g�̍ŏ����珇�ɕ��Ԃ��ƂɂȂ�j
		}
		int	a_iPoly_Number	= 0;
		for( a_iIndx = e_lRings.begin(); a_iIndx != e_lRings.end(); a_iIndx++, a_iPoly_Number++ )
		{
			RING	a_rOuter	= *a_iIndx;
			if( a_rOuter.IsClockwise == TRUE ) { // 2003.10.01
				e_vOuter.push_back( a_rOuter );	// 
				e_lPart_Num++;

				// �ő�T�u�p�[�g���𓾂�
				int a_iInner_Cnt = 0;
				a_iInner_Cnt	= Inner_Ring_Chk ( a_iPoly_Number );
				if( a_iInner_Cnt > e_lSubPart_Max ) {
					e_lSubPart_Max	= a_iInner_Cnt;
				}
			}
		}

#ifdef	_CHECK_
		if( e_lPart_Num > 2 ) {
			printf("pnum = %d\n", e_lPart_Num);
		}
#endif

		// <�����܂�>---------------------------------------------------------------
		e_lSubPart_Max++;	// �O���̕�1������
	}
	return	( true );
}


/**
	�����O������钆���������O�i�T�u�p�[�g�j�̃`�F�b�N�p
	����{�I��Init�������Ŏg�p
	���������E���|���S���ł��邱�Ƃ��O��
	@return	�����������O��
*/
int		msi_get_parts_info::
Inner_Ring_Chk(	int	c_iIndx	)	///< RING�|���S���̃C���f�b�N�X
{
	int	a_iInner_Ring_Num	= 0;	// ������O��

	list<SIMPLERING>::iterator	a_iIndx, a_iBegin, a_iEnd;
	a_iBegin	= e_vChk_Rings[c_iIndx].Contain.begin();
	a_iEnd		= e_vChk_Rings[c_iIndx].Contain.end();
	// ����������Ă���|���S���ɂ���
	for( a_iIndx = a_iBegin; a_iIndx != a_iEnd; a_iIndx++ )
	{
		SIMPLERING	a_sRing	= *a_iIndx;
		if( e_vChk_Rings[a_sRing.INDX].IsClockwise == FALSE ) {	// �����
			if( Is_Inner_Ring ( e_vChk_Rings[c_iIndx], e_vChk_Rings[a_sRing.INDX] ) ) {
				a_iInner_Ring_Num++;
			}
		}
	}
	return ( a_iInner_Ring_Num );
}

/** 2003.10.03
	�����O�ɑ΂��ē��������O�����̒��������ǂ������`�F�b�N����
	��Init��Ɏg�p
	@return	bool
*/
bool	msi_get_parts_info::
Is_Inner_Ring(		RING&	c_rOuter,	///< �O���|���S��
					RING&	c_rInner )	///< �����|���S��
{
	
	// c_rInner��Within�|���S�����Ac_rOuter��Within�|���S���{c_rOuter���܂߂Β��ڂ̒����������O
	int	a_iWithin_Diff	= c_rInner.Within.size() - c_rOuter.Within.size();
	if( a_iWithin_Diff != 1 ) {	// ���ł͂Ȃ��̂�
		return	( false );
	}
	if( c_rInner.IsClockwise == TRUE ) {	// �������E��肾������ΏۊO
		return	( false );
	}
	
	int	a_iFind_Mine		= 0;	// �����̃|���S���̊O���|���S���ɂ͎����������Ă��邱�Ƃ��O��
	int	a_iSame_Outer_Count	= 0;	// ���̒l��c_rOuter.Within.size()�Ɠ����ł���΂悢
	list<SIMPLERING>::iterator	a_iInner_Indx, a_iOuter_Indx;
	for( a_iInner_Indx = c_rInner.Within.begin(); a_iInner_Indx != c_rInner.Within.end(); a_iInner_Indx++ )
	{
		if( a_iInner_Indx->INDX == c_rOuter.NUM ) {	// �����������Ă���
			a_iFind_Mine	= 1;
		}
		for( a_iOuter_Indx = c_rOuter.Within.begin(); a_iOuter_Indx != c_rOuter.Within.end(); a_iOuter_Indx++ )
		{
			if( a_iInner_Indx->INDX == a_iOuter_Indx->INDX ) {
				a_iSame_Outer_Count++;
				break;
			}
		}
	}

	if( a_iFind_Mine == 1 && (unsigned)a_iSame_Outer_Count == c_rOuter.Within.size() ) {
		return	( true );
	}
	return ( false );
}

/**�@2003.10.25
	�|���S���̏�Ԃ����čč\������i���p�[�g�������̂P���x������|���S���̏ꍇ�ɍs���j
	@return	TRUE	�č\���������������΂���
	@return	FALSE	�č\�����������������ꍇ
*/
bool	msi_get_parts_info::
Reconfig_Polygon( void )
{
	//���̓����ł́A���Ɏ擾���ꂽ���
	//	list<RING>	e_lRings;		///< 2003.08.21
	//	WKSPoint*	e_wpPoints;		///< 2003.08.21
	// �̏�Ԃ��`�F�b�N���A��L��2�����o�ɂ��čX�V�������s�����̂Ƃ���
	// ��̓I�ɂ́AIsClockWise�̏���ύX���A�Y�����镔���̓_��𔽓]������

	bool	a_bRet	= FALSE;
	list<RING>::iterator	a_liIndx;
	int	i = 0;
	for( a_liIndx = e_lRings.begin(); a_liIndx != e_lRings.end(); a_liIndx++ )
	{
		RING	a_rCurRing	= *a_liIndx;
		// �������܂ރ|���S�����Ȃ��A�������܂ރ|���S����������Ȃ��A�������E���̏ꍇ�̂݁i�����������̂͂��̏�Ԃ����H�j
		if( a_rCurRing.IsClockwise == TRUE && 
			a_rCurRing.Contain.size() == 0 &&
			a_rCurRing.Within.size() == 1 )
		{
			a_liIndx->IsClockwise	= FALSE;	// �����ɂ���
			a_liIndx->XMIN			= a_liIndx->START + ( a_liIndx->END - a_liIndx->XMIN );

			int	a_iSize	= a_rCurRing.END - a_rCurRing.START + 1;

			WKSPoint*	a_wWorkPts	= new WKSPoint[a_iSize];
			// �e���|�����ɃR�s�[���Ă���
			
			int	j = 0;
			for( j = 0; j < a_iSize; j++ )
			{
				a_wWorkPts[j]	= e_wpPoints[a_rCurRing.START+j];	// ��������
			}
			for( j = 0; j < a_iSize; j++)
			{
				e_wpPoints[a_rCurRing.START+j]	= a_wWorkPts[(a_iSize - 1) - j];	// ��������
			}
			delete [] a_wWorkPts;
			a_bRet	= TRUE;	// �č\����������
		}
	}
	return	( a_bRet );
}

/**
	�p�[�g���̊l��
	@return	�p�[�g��
*/
long	msi_get_parts_info::
Get_Part_Num	( void )
{
	return	( e_lPart_Num );
}

/**
	�T�u�p�[�g���̊l��
	�������A�w��p�[�g�����݂��Ȃ��ꍇ�́A'0'��Ԃ�
	@return	�T�u�p�[�g��
*/
long	msi_get_parts_info::
Get_SubPart_Num	(	long	c_lPart_Num	)	// �w��p�[�g�ԍ�(�P�I���W��)
{
	long	a_lSubPart_Num	= 0;

	if(c_lPart_Num > e_lPart_Num || c_lPart_Num < 1) {
		Error_Print ( "Get_SubPart_Num_01", "�w��p�[�g�̃T�u�p�[�g���͂Ƃ肾���܂���" );
		return		( 0 );
	}

	if( e_eGeoType != esriGeometryPolygon ) {	// �|���S���ȊO�̂Ƃ�

		return	( 1 );				// �|���S���ȊO�́A�p�[�g==�T�u�p�[�g�̍l�������Ȃ̂ŁA'1'�Ƃ���

	}else {		// �|���S���̏ꍇ
	
		RING	a_rPart;
		a_rPart	= e_vOuter[c_lPart_Num-1];
		
		a_lSubPart_Num	= Inner_Ring_Chk( a_rPart.NUM );	// 2003.10.02
		a_lSubPart_Num++;
	}
	return	( a_lSubPart_Num );	
}

/**
	�T�u�p�[�g���̍ő吔
	�����܂�g�p�p�x�������Ȃ������Ȃ̂ŁA�������������ɂ��Ă���
	@return �T�u�p�[�g���̍ő�l
*/
long	msi_get_parts_info::
Get_SubPart_Max	( void )
{
	return	( e_lSubPart_Max );
}


/**
	�_��𓾂�
	�|���S���ɂ��Ă͉E���Ŏ擾�ł���΂悢�Ƃ����O��@\
	�����ŃN���[�����쐬���A���̃I�u�W�F�N�g�ɂ��ă|�C���g��Ԃ��@\
	���̂悤�ɂ��Ȃ��ƁA�N���X�ŕێ����Ă���}�`�ɂ��ď����������邨���ꂪ����
	@return IPointCollection�_��
	�����s�����ꍇ��NULL
*/
IPointCollectionPtr	msi_get_parts_info::
Get_Points	(	long		c_lPart_Num,		///< �p�[�g�ԍ�
				long		c_lSubPart_Num	)	///< �T�u�p�[�g�ԍ�
{
	HRESULT	hr;
	IGeometryPtr		a_ipSubPart_Geo;	// �T�u�p�[�g�̃W�I���g��

	if( c_lPart_Num > e_lPart_Num || c_lPart_Num < 1) {
		Error_Print ( "Get_Points_01", "�w��p�[�g�̓_��͂Ƃ肾���܂���" );
		return		( NULL );
	}

	// �����ŃT�u�p�[�g�ɊY������V�F�C�v���Ƃ��Ă���
	if( e_eGeoType != esriGeometryPolygon ) {	// �|���S���ȊO�������ꍇ

		if(FAILED(e_ipGeoCol->get_Geometry(c_lPart_Num-1, &a_ipSubPart_Geo)) ) {
			Error_Print ( "Get_Points_02", "�R���N�V��������W�I���g���̎��o���Ɏ��s" );
			return		( NULL );
		}

	}else {	// �|���S���̏ꍇ

		IPointCollectionPtr		a_ipSp_Pts(CLSID_Ring);

		if(c_lSubPart_Num == 1) {	// �T�u�p�[�g���P�̎��́A�O���̃����O��Ԃ�	
		
			// 2003.8.21 
			RING	a_rOuter		= e_vOuter[c_lPart_Num-1];
			if( FAILED ( a_ipSp_Pts->AddWKSPoints ( (a_rOuter.END - a_rOuter.START)+1, ( e_wpPoints + a_rOuter.START )) )) {
				Error_Print ( "Get_Points_02", "AddWKSPoints�Ɏ��s" );
				return		( NULL );		
			}
			a_ipSubPart_Geo			= a_ipSp_Pts;
		}else {

			// 2003.8.21 
			RING	a_rOuter		= e_vOuter[c_lPart_Num-1];
			int		a_iAddCount		= 0;

			// 2003.10.03 �w��T�u�p�[�g�ԍ��ɂ����钆�����|���S�����Ƃ��Ă���
			SIMPLERING	a_rInner;
			int		a_iSkip_Count	= 1;
			list<SIMPLERING>::iterator	a_ipInner_Indx;
			for( a_ipInner_Indx = a_rOuter.Contain.begin(); a_ipInner_Indx != a_rOuter.Contain.end(); a_ipInner_Indx++ )
			{
				// ���ڂ̒������ȊO�͔�΂�
				if( !Is_Inner_Ring ( a_rOuter, e_vChk_Rings[a_ipInner_Indx->INDX] ) ) {
					continue;
				}else {
					a_iSkip_Count++;
					if( a_iSkip_Count == c_lSubPart_Num ) {
						a_rInner	= *a_ipInner_Indx;
						break;
					}
				}
			}
			// -----------------------------------------

			// �����̌��𓾂�
			int			a_iRev_Size	= (a_rInner.END - a_rInner.START)+1;
			WKSPoint*	a_wRev_Pts	= new WKSPoint[a_iRev_Size];
			// �_��𔽎��v����
			int	i = 0, j = 0;
			for(i = 0, j = a_rInner.END; i < a_iRev_Size; i++, j-- )
			{
				a_wRev_Pts[i]	= e_wpPoints[j];
			}
			hr	= a_ipSp_Pts->AddWKSPoints ( a_iRev_Size, a_wRev_Pts );
			delete [] a_wRev_Pts;
			if( FAILED ( hr ) ) {
				Error_Print ( "Get_Points_02", "AddWKSPoints�Ɏ��s" );
				return		( NULL );		
			}
			a_ipSubPart_Geo			= a_ipSp_Pts;
		}
	}

	// �N���[�����쐬���āA���ꂩ����o�����_���Ԃ�
	IClonePtr	a_ipClone;
	if( FAILED ( ((IClonePtr)a_ipSubPart_Geo)->Clone( &a_ipClone ) ) ){
		Error_Print ( "Get_Points_03", "�W�I���g���̃N���[���Ɏ��s" );
		return		( NULL );
	}
	IPointCollectionPtr		a_ipPts_Tmp;	

	a_ipPts_Tmp	= (IGeometryPtr)a_ipClone;	// �l�������T�u�p�[�g�_����i�[
	return		( a_ipPts_Tmp );
}

/**
	�V�F�C�v�̎w��p�[�g�A�T�u�p�[�g����A�W�I���g����Ԃ�
	���Ԃ��W�I���g���̃^�C�v�́A���W�I���g���ɂ��B���̃����o�֐����A��͂� \
	�@�����ŃN���[�����쐬���Ă����Ԃ��`�ɂ��Ȃ��ƁA�O���ŏ�����������\���@\
	�@������B
	@return	true�@����
	@return false ���s
*/
IGeometryPtr	msi_get_parts_info::
Get_Geo_from_Part_Subpart	(	long	c_lPart_Num,		///< �p�[�g�ԍ�
								long	c_lSubPart_Num	)	///< �T�u�p�[�g�ԍ�
{
	HRESULT	hr;

	IGeometryPtr		a_ipSubPart_Geo;	// �T�u�p�[�g�̃W�I���g��

	if(c_lPart_Num > e_lPart_Num) {
		Error_Print ( "Get_Geo_from_Part_Subpart_01", "�w��p�[�g�̓_��͂Ƃ肾���܂���" );
		return		( NULL );
	}

	// �����ŃT�u�p�[�g�ɊY������V�F�C�v���Ƃ��Ă���
	if( e_eGeoType != esriGeometryPolygon ) {	// �|���S���ȊO�������ꍇ

		if( e_eGeoType != esriGeometryPolyline ) {
			Error_Print ( "Get_Geo_from_Part_Subpart_02", "���Ή��̃W�I���g���^�C�v" );
			return		( NULL );
		}

		IPolylinePtr		a_ipTmp_Line ( CLSID_Polyline );
		IPointCollectionPtr	a_ipLine_Pts_Tmp ( a_ipTmp_Line );
		hr	= a_ipLine_Pts_Tmp ->AddPointCollection ( Get_Points ( c_lPart_Num,	c_lSubPart_Num ) );	///< �T�u�p�[�g�ԍ�
		if( FAILED ( hr ) ) {
			Error_Print ( "Get_Geo_from_Part_Subpart_02", "�_��̒ǉ����ł��Ȃ�" );
			return		( NULL );			
		}
		
		if( a_ipLine_Pts_Tmp == NULL ) {
			Error_Print ( "Get_Geo_from_Part_Subpart_02", "�_��̎擾���ł��Ȃ�" );
			return		( NULL );
		}

		a_ipSubPart_Geo	= a_ipTmp_Line;

	}else
	if( e_eGeoType == esriGeometryPolygon ) {	// �|���S���̏ꍇ

		IPointCollectionPtr	a_ipLine_Pts_Tmp;
		a_ipLine_Pts_Tmp	= Get_Points ( c_lPart_Num,	c_lSubPart_Num );	///< �T�u�p�[�g�ԍ�
		if( a_ipLine_Pts_Tmp == NULL ) {
			Error_Print ( "Get_Geo_from_Part_Subpart_02", "�_��̎擾���ł��Ȃ�" );
			return		( NULL );
		}

		IPolygonPtr			a_ipTmp_Poly ( CLSID_Polygon );
		IPointCollectionPtr	a_tpTmpPts( a_ipTmp_Poly );

		// �_���ǉ�����
		if( FAILED (a_tpTmpPts->AddPointCollection ( a_ipLine_Pts_Tmp )) ) {
			Error_Print ( "Get_Geo_from_Part_Subpart_02", "�_��̒ǉ����ł��Ȃ�" );
			return		( NULL );
		}
		a_ipSubPart_Geo	= a_ipTmp_Poly;

	}

	// �N���[�����쐬���āA���ꂩ����o�����_���Ԃ�
	IClonePtr	a_ipClone;
	if( FAILED ( ((IClonePtr)a_ipSubPart_Geo)->Clone( &a_ipClone ) ) ){
		Error_Print ( "Get_Geo_from_Part_Subpart_03", "�R���N�V��������W�I���g���̎��o���Ɏ��s" );
		return		( NULL );
	}

	IGeometryPtr			a_ipRet_Geo;
	a_ipRet_Geo				= (IGeometryPtr)a_ipClone;
	
	return	( a_ipRet_Geo );	// �l�������T�u�p�[�g���i�[
}

/**
	�w��p�[�g�̃W�I���g�������o��
	���Ԃ��W�I���g���̃^�C�v�́A���W�I���g���ɂ��B�����ł́AGet_Geo_from_Part_Subpart \
	�@Get_Part_Polygon�ŃN���[�����쐬����̂ŁA���̂܂ܕԂ�
	@return �w��p�[�g�̃W�I���g��
*/
IGeometryPtr		msi_get_parts_info::
Get_Geo_from_Part	(	long	c_lPart_Num	)	///< �p�[�g�ԍ�
{

	IGeometryPtr		a_ipPart_Geo;			// �p�[�g�̃W�I���g��

	if( e_eGeoType != esriGeometryPolygon ) {	// �|���S���ȊO�������ꍇ
		a_ipPart_Geo	= Get_Geo_from_Part_Subpart	( c_lPart_Num, 1 );
		if( a_ipPart_Geo == NULL ){
			return	( NULL );
		}

	}else {	// �|���S���̏ꍇ

		IPolygonPtr		a_ipTmpPoly;
		a_ipTmpPoly		= Get_Part_Polygon ( c_lPart_Num );
		if( a_ipTmpPoly == NULL ) {
			return	( NULL );
		}
		a_ipPart_Geo	= a_ipTmpPoly;

	}
	return	( a_ipPart_Geo );
}


/**
	�w��p�[�g�̃|���S�������o���i�|���S���̏ꍇ�j
	���^����ꂽ�W�I���g�����|���S���ł���ꍇ�̂݁B�������C�����Ŏg�p�����ꍇ
	�@�ɂ́ANULL��Ԃ�
	�@�����ŃN���[�����쐬���Ă���|���S����Ԃ��B
	@return �w��p�[�g�̃|���S��
*/
IPolygonPtr		msi_get_parts_info::
Get_Part_Polygon	(	long	c_lPart_Num	)	///< �p�[�g�ԍ�
{

	HRESULT	hr;

	// �f�[�^�^�C�v�����������̔���
	if( e_eGeoType != esriGeometryPolygon ) {
		Error_Print ( "Get_Part_Polygon_01", "���̃��\�b�h�̓|���S���ȊO�ɂ͔�Ή��ł�" );
		return	( NULL );
	}
	// �p�[�g�ԍ������������̔���
	if(c_lPart_Num > Get_Part_Num()) {
		Error_Print ( "Get_Part_Polygon_02", "�w��p�[�g�ԍ����s���ł�" );
		return	( NULL );
	}


	IPolygonPtr				a_ipNewPolygon( CLSID_Polygon );
	IGeometryCollectionPtr	a_ipNewColl( a_ipNewPolygon );

	// �w��p�[�g�̊O�������O�����o�� 2003.08.25
	RING					a_irOuter_Ring	= e_vOuter[c_lPart_Num-1];
	
	IRingPtr				a_ipOuter_Ring( CLSID_Ring );
	IPointCollectionPtr		a_ipPoints( a_ipOuter_Ring );

	// �O���_���ǉ�
	hr	= a_ipPoints->AddWKSPoints ( (a_irOuter_Ring.END - a_irOuter_Ring.START)+1, ( e_wpPoints + a_irOuter_Ring.START )) ;
	if( FAILED ( hr ) ) {
		Error_Print ( "Get_Part_Polygon_04", "�w��p�[�g�̃����O���ǉ��ł��Ȃ�" );
		return		( NULL );
	}

	hr	= a_ipNewColl->AddGeometry ( (IGeometryPtr)a_ipOuter_Ring );
	if( FAILED ( hr ) ) {
		Error_Print ( "Get_Part_Polygon_04", "�w��p�[�g�̃����O���ǉ��ł��Ȃ�" );
		return		( NULL );
	}

	// �����_���ǉ��i�_��𔽎��v���ɂ���j
	list<SIMPLERING>::iterator	a_ipInner_Indx;
	for( a_ipInner_Indx = a_irOuter_Ring.Contain.begin(); a_ipInner_Indx != a_irOuter_Ring.Contain.end(); a_ipInner_Indx++ )
	{
		// ���ڂ̒������ȊO�͔�΂�
		if( !Is_Inner_Ring ( a_irOuter_Ring, e_vChk_Rings[a_ipInner_Indx->INDX] ) ) {
			continue;
		}

		SIMPLERING	a_sCur_Ring	= *a_ipInner_Indx;
		int			a_iRev_Size	= (a_sCur_Ring.END - a_sCur_Ring.START)+1;

		IRingPtr	a_ipInner_Ring ( CLSID_Ring );

		IPointCollectionPtr	a_ipInnerPts ( a_ipInner_Ring );

		hr	= a_ipInnerPts->AddWKSPoints ( (a_sCur_Ring.END - a_sCur_Ring.START)+1, ( e_wpPoints + a_sCur_Ring.START )) ;
		if( FAILED ( hr ) ) {
			Error_Print ( "Get_Part_Polygon_04", "�T�u�p�[�g�̃����O���ǉ��ł��Ȃ�" );
			continue;
		}

		hr	= a_ipNewColl->AddGeometry ( (IGeometryPtr)a_ipInner_Ring );
		if( FAILED ( hr ) ) {
			Error_Print ( "Get_Part_Polygon_04", "�T�u�p�[�g�̃����O���ǉ��ł��Ȃ�" );
			continue;
		}

	}
#ifdef	_DEBUG	// �����������H�H�H�͂������瓹�H�̒��������������������A�X��̒��������������������������E�E�E
//	ITopologicalOperator2Ptr	a_ipTopo;		// �_���I�y���[�^
//	a_ipTopo			= a_ipNewPolygon;
//	hr					= a_ipTopo->put_IsKnownSimple ( VARIANT_FALSE );
//	if( FAILED ( hr ) ) {
//		Error_Print ( "Get_Part_Polygon_07", "�|���S�����P�����ł��܂���ł���" );
//		return		( NULL );
//	}
//	hr					= a_ipNewPolygon->SimplifyPreserveFromTo();	// 2003.06.13 �C��
//	if( FAILED ( hr ) ) {
///		Error_Print ( "Get_Part_Polygon_07", "�|���S�����P�����ł��܂���ł���" );
//		return		( NULL );
//	}
#endif	
	return	( a_ipNewPolygon );
}


/**	
	�N���X�Ɋi�[����Ă���I�u�W�F�N�g�̏����o�͂���
	��Init������łȂ��Ƃ��߁I
	@return bool
*/
bool	msi_get_parts_info::
Print_Obj_Info		( void )
{
	// �p�[�g���o��
	fprintf ( e_fpLog, "�p�[�g�� = %d : �ő�T�u�p�[�g�� = %d\n", e_lPart_Num, e_lSubPart_Max );
	
	int	i = 0;
	for(i = 1; i <= e_lPart_Num; i++)
	{
		long			a_lSubNum	= 0;
		a_lSubNum		= Get_SubPart_Num ( i );
		fprintf			( e_fpLog, "�p�[�g�ԍ� = <%d> : �T�u�p�[�g�� = <%d>\n", i, a_lSubNum);
		IGeometryPtr	a_ipGeo;
		int j = 0;
		for( j = 1; j <= a_lSubNum; j++)
		{
			a_ipGeo			= Get_Geo_from_Part_Subpart ( i, j );
			
			IPointCollectionPtr		a_ipCur_Pts(a_ipGeo);
			
			long	a_lPointCount	= 0;
			if( FAILED(a_ipCur_Pts->get_PointCount ( &a_lPointCount )) ) {
				return ( false );
			}	
			fprintf ( e_fpLog ,"���|�C���g�� = %d\n", a_lPointCount);
			long	k = 0;
			for(k = 0; k < a_lPointCount; k++)
			{
				IPointPtr	a_ipPt;
				if ( FAILED( a_ipCur_Pts->get_Point ( k, &a_ipPt )) ) {
					return ( false );
				}
				double		x = 0.0, y = 0.0;
				a_ipPt->get_X ( &x );
				a_ipPt->get_Y ( &y );
				fprintf ( e_fpLog,	"�p�[�g = <%d> :�@�T�u�p�[�g = <%d> count = <%d> (%0.10f, %0.10f)\n",
									i, j, k, x, y);
			}
		}
	}
	return ( true );
}

/**	
	�N���X�Ɋi�[����Ă���I�u�W�F�N�g�̏����o�͂���
	�N���X�Ɋi�[����Ă���I�u�W�F�N�g�̏����o�͂���(���_��j
	��Init������łȂ��Ƃ��߁I
	@return bool
*/
bool	msi_get_parts_info::
Print_Obj_Info2		( void )
{
	// �p�[�g���o��
	fprintf ( e_fpLog, "�����O�� = %d : �ő�T�u�p�[�g�� = %d\n", e_vChk_Rings.size(), e_lSubPart_Max );
	
	int	i = 0;
	for(i = 0; i < e_vChk_Rings.size(); i++)
	{

		long			a_lSubNum	= 0;
	//	a_lSubNum		= Get_SubPart_Num ( i+1 );
		fprintf			( e_fpLog, "�����O�ԍ� = <%d> : NUM=<%d> Contain = <%d> rot= <%s>\n",
									i, e_vChk_Rings[i].NUM, e_vChk_Rings[i].Contain.size(), e_vChk_Rings[i].IsClockwise?"�E���":"�����");
			int	j = 0;
			list<SIMPLERING>::iterator	a_iIndx	= e_vChk_Rings[i].Contain.begin();
			for(j = 0; a_iIndx != e_vChk_Rings[i].Contain.end(); j++, a_iIndx++ )
			{
				SIMPLERING	tmp	= *a_iIndx;
				fprintf	( e_fpLog, "                   : �C���f�b�N�X= <%d> b= <%d> e=<%d> rot= <%s>\n", 
					tmp.INDX, tmp.START, tmp.END, e_vChk_Rings[tmp.INDX].IsClockwise?"�E���":"�����");
			}

		fprintf			( e_fpLog, "                   : Within  = <%d>\n", e_vChk_Rings[i].Within.size());
			a_iIndx	= e_vChk_Rings[i].Within.begin();
			for(j = 0; a_iIndx != e_vChk_Rings[i].Within.end(); j++, a_iIndx++ )
			{
				SIMPLERING	tmp	= *a_iIndx;
				fprintf	( e_fpLog, "                   : �C���f�b�N�X= <%d> b= <%d> e=<%d> rot= <%s>\n", 
					tmp.INDX, tmp.START, tmp.END, e_vChk_Rings[tmp.INDX].IsClockwise?"�E���":"�����");
			}
		fprintf			( e_fpLog, "                   : Cross   = <%d>\n", e_vChk_Rings[i].Cross.size());
			a_iIndx	= e_vChk_Rings[i].Cross.begin();
			for(j = 0; a_iIndx != e_vChk_Rings[i].Cross.end(); j++, a_iIndx++ )
			{
				SIMPLERING	tmp	= *a_iIndx;
				fprintf	( e_fpLog, "                   : �C���f�b�N�X= <%d> b= <%d> e=<%d> rot= <%s>\n", 
					tmp.INDX, tmp.START, tmp.END, e_vChk_Rings[tmp.INDX].IsClockwise?"�E���":"�����");
			}
		fprintf			( e_fpLog, "                   : Disjoint= <%d>\n", e_vChk_Rings[i].Disjoint.size());
			a_iIndx	= e_vChk_Rings[i].Disjoint.begin();
			for(j = 0; a_iIndx != e_vChk_Rings[i].Disjoint.end(); j++, a_iIndx++ )
			{
				SIMPLERING	tmp	= *a_iIndx;
				fprintf	( e_fpLog, "                   : �C���f�b�N�X= <%d> b= <%d> e=<%d> rot= <%s>\n", 
					tmp.INDX, tmp.START, tmp.END, e_vChk_Rings[tmp.INDX].IsClockwise?"�E���":"�����");
			}
	}
	return ( true );
}
