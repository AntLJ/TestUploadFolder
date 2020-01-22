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
#include "main_rootin.h"
#include <poly_class.h>
#include <atlstr.h>
#include <WinLib/VersionInfo.h>

using namespace sindy::schema;

/** ---------------------------------------------------------------------------
//	�w��̒������牽�x�ɓ����邩��Ԃ�
//	@return	�x�P�ʂł̋���
-----------------------------------------------------------------------------*/
double	main_rootin::
Get_Degree_Range (	int		c_iMeshCode,		///< ���b�V���R�[�h 
					double	c_dMeter_Range )	///< ���[�g���P�ʂ͈̔�
{
	crd_cnv		a_cCrd;
	double		a_dMeshWid	= 0.0;
	double		a_dMeshHei	= 0.0;
	double		a_dLd_Lon	= 0.0;	// �����ܓx
	double		a_dLd_Lat	= 0.0;	// �����o�x
	double		a_dRd_Lon	= 0.0;	// �����ܓx
	double		a_dRd_Lat	= 0.0;	// �����o�x

	a_dMeshWid	= a_cCrd.GetMeshDist ( c_iMeshCode, 0, 0, c_iMeshCode, 1000000, 0 );
	a_dMeshHei	= a_cCrd.GetMeshDist ( c_iMeshCode, 0, 0, c_iMeshCode, 0, 1000000 );

	a_cCrd.MeshtoLL ( c_iMeshCode, 0, 0, &a_dLd_Lon, &a_dLd_Lat );
	a_cCrd.MeshtoLL	( c_iMeshCode, 1000000, 0, &a_dRd_Lon, &a_dRd_Lat );

	return(	(a_dRd_Lon - a_dLd_Lon) * c_dMeter_Range / a_dMeshWid );

}

/** ---------------------------------------------------------------------------
//	�w��̒������牽�x�ɓ����邩��Ԃ�
//	@return	�x�P�ʂł̋���
-----------------------------------------------------------------------------*/
double	main_rootin::
Get_Degree_Range_Dev (	int		c_iMeshCode,		///< ���b�V���R�[�h
						int		c_iScale,			///< �X�P�[��
						int		c_iDev_Dist )		///< �f�o�C�X���W�_�l�ł̋���
{
	crd_cnv		a_cCrd;
	double		a_dMeshWid	= 0.0;
	double		a_dMeshHei	= 0.0;
	double		a_dLd_Lon	= 0.0;	// �����ܓx
	double		a_dLd_Lat	= 0.0;	// �����o�x
	double		a_dRd_Lon	= 0.0;	// �����ܓx
	double		a_dRd_Lat	= 0.0;	// �����o�x

	int	devrange_x[4];
	int	devrange_y[4];
	int i = 0, j = 0;
	for(i = 0, j = 1; i < 4; i++, j*=2)
	{
		devrange_x[i]	=	320 * j;
		devrange_y[i]	=	240 * j;
	}

	a_cCrd.MeshtoLL ( c_iMeshCode, 0, 0, &a_dLd_Lon, &a_dLd_Lat );
	if( a_cCrd.GetMeshDigit(c_iMeshCode) == 4 ) {
		a_cCrd.MeshtoLL	( c_iMeshCode, 2000000, 0, &a_dRd_Lon, &a_dRd_Lat );
	}else {
		a_cCrd.MeshtoLL	( c_iMeshCode, 1000000, 0, &a_dRd_Lon, &a_dRd_Lat );
	}
	// �o�x�����ł̋���
	return(	(a_dRd_Lon - a_dLd_Lon) * c_iDev_Dist / devrange_x[c_iScale-1] );
}

/** ---------------------------------------------------------------------------
//	�w��̒������牽�x�ɓ����邩��Ԃ�
-----------------------------------------------------------------------------*/
void	main_rootin::
Get_Degree_Range_Dev (	int		c_iMeshCode,		///< ���b�V���R�[�h[IN]
						int		c_iScale,			///< �X�P�[��[IN]
						int		c_iDev_Dist, 		///< �f�o�C�X���W�_�l�ł̋���[IN]
						double*	c_cpRetXDeg,		///< X�����̋���[OUT]
						double*	c_cpRetYDeg )		///< Y�����̋���[OUT]
{
	crd_cnv		a_cCrd;
	double		a_dMeshWid	= 0.0;
	double		a_dMeshHei	= 0.0;
	double		a_dLd_Lon	= 0.0;	// �����ܓx
	double		a_dLd_Lat	= 0.0;	// �����o�x
	double		a_dRu_Lon	= 0.0;	// �����ܓx
	double		a_dRu_Lat	= 0.0;	// �����o�x

	int	devrange_x[4];
	int	devrange_y[4];
	int i = 0, j = 0;
	for(i = 0, j = 1; i < 4; i++, j*=2)
	{
		devrange_x[i]	=	320 * j;
		devrange_y[i]	=	240 * j;
	}

	a_cCrd.MeshtoLL ( c_iMeshCode, 0, 0, &a_dLd_Lon, &a_dLd_Lat );
	if( a_cCrd.GetMeshDigit(c_iMeshCode) == 4 ) {
		a_cCrd.MeshtoLL	( c_iMeshCode, 2000000, 2000000, &a_dRu_Lon, &a_dRu_Lat );
	}else {
		a_cCrd.MeshtoLL	( c_iMeshCode, 1000000, 1000000, &a_dRu_Lon, &a_dRu_Lat );
	}
	// �o�x�����ł̋���
	*c_cpRetXDeg	= (a_dRu_Lon - a_dLd_Lon) * c_iDev_Dist / devrange_x[c_iScale-1] ;
	*c_cpRetYDeg	= (a_dRu_Lat - a_dLd_Lat) * c_iDev_Dist / devrange_y[c_iScale-1] ;
}

/** ---------------------------------------------------------------------------
//	��x�f�o�C�X���W�ɕϊ����A���̌�ܓx�o�x�ɖ߂�
//	@return	IPointPtr
-----------------------------------------------------------------------------*/
IPointPtr	main_rootin::
get_Device_Pt	(	int				c_iMeshCode,
					IPointPtr		c_ipPoint,
					int				c_iScale	)	///< �X�P�[��
{
	crd_cnv	crd;

	int	devrange_x[4];
	int	devrange_y[4];
	int i = 0, j = 0;
	for(i = 0, j = 1; i < 4; i++, j*=2)
	{
		devrange_x[i]	=	320 * j;
		devrange_y[i]	=	240 * j;
	}
	double		a_dTmp_X	= 0, a_dTmp_Y	= 0;
//	Device_Pt	a_mDev_Pt;		// ���f�o�C�X���W
	int			a_iNorm_X	= 0;
	int			a_iNorm_Y	= 0;

	double	a_dX = 0.0, a_dY = 0.0;
	c_ipPoint->get_X ( &a_dX );
	c_ipPoint->get_Y ( &a_dY );

	// ���K�����W�Ɉ�x�ϊ�����(���b�V���R�[�h�w��)
	crd.LLtoMesh2	( a_dX, a_dY, c_iMeshCode, &a_iNorm_X, &a_iNorm_Y);

	int	a_iMesh_Digit	= crd.GetMeshDigit ( c_iMeshCode );
	if( a_iMesh_Digit == 4 ) {
		a_iNorm_X /= 2;
		a_iNorm_Y /= 2;
	}

	// �f�o�C�X���W�̌v�Z�i�l�̌ܓ�����j
	a_dTmp_X		= (double)devrange_x[c_iScale-1] * a_iNorm_X / 1000000.0 + 0.5;
	a_dTmp_Y		= (double)devrange_y[c_iScale-1] * a_iNorm_Y / 1000000.0 + 0.5;
//	a_mDev_Pt.e_X	= (long)a_dTmp_X;
//	a_mDev_Pt.e_Y	= (long)a_dTmp_Y;
	// �v�Z�덷�̂��߂��AIN�f�[�^��+1pt�̍�������̂ŁA-1pt���Ă���
	int	a_iDev_X	= 0;
	int a_iDev_Y	= 0;

	a_iDev_X	= (long)a_dTmp_X-1;	// 2002.7.18
	a_iDev_Y	= (long)a_dTmp_Y-1;	// 2002.7.18

	// ���W���ܓx�o�x�ɖ߂�
	double	a_dAfter_X	= 0.0;
	double	a_dAfter_Y	= 0.0;
	double	a_dAfter_N_X= 0;
	double	a_dAfter_N_Y= 0;
	int		a_iTmp_X	= 0;
	int		a_iTmp_Y	= 0;

	a_dAfter_N_X	= (double)a_iDev_X * 1000000.0 / devrange_x[c_iScale-1] - 0.5;
	a_dAfter_N_Y	= (double)a_iDev_Y * 1000000.0 / devrange_y[c_iScale-1] - 0.5;
	a_iTmp_X	= (int)a_dAfter_N_X;
	a_iTmp_Y	= (int)a_dAfter_N_Y;
	if( a_iMesh_Digit == 4 ) {
		a_iTmp_X *= 2;
		a_iTmp_Y *= 2;
	}
	crd.MeshtoLL ( c_iMeshCode, a_iTmp_X, a_iTmp_Y, &a_dAfter_X, &a_dAfter_Y );

	IPointPtr	a_ipRet_Pt(CLSID_Point);
	a_ipRet_Pt->put_X ( a_dAfter_X );
	a_ipRet_Pt->put_Y ( a_dAfter_Y );

	return		( a_ipRet_Pt );
}

/** ---------------------------------------------------------------------------
//	�^�ʒu���܂߂��\���ʒu��S�Ď��o��
-----------------------------------------------------------------------------*/
bool	main_rootin::
Get_Disp_Pts	(	int						c_iMeshCode,		// ���b�V���R�[�h
				//	int						c_iAttr_Code,		// �����R�[�h
					Feature_All_Attrs&		c_fFAtt,			// ���L�^�ʒu�̏��
					msi_handle_ws*			c_mAny_Note_Line_WS,// �Y�����C���̌����p
					vector<disp_pt_info>&	c_vPoints )
{
	//--------------------------------------------------------------------
	// �^�ʒu�̏��
	disp_pt_info			a_dTruePtInfo;
	a_dTruePtInfo.e_iOid	= c_fFAtt.Get_Obj_ID();
	a_dTruePtInfo.e_ipPt	= (IPointPtr)c_fFAtt.Ret_Geo();
	if( m_iDigit == 8 ) {
		a_dTruePtInfo.e_iScale	= 3;	// �s�s�n�}�̓X�P�[���R
	}else {
		a_dTruePtInfo.e_iScale	= 4;	// ��
	}
	a_dTruePtInfo.e_iStat	= 1;		// �^�ʒu�ł�

	c_vPoints.push_back	( a_dTruePtInfo );	// �f�[�^�ǉ�
	//--------------------------------------------------------------------

	// ���L���C�����s�v�̏ꍇ�͔�����
	if( eExe_Flag & ON_DEL_DISP_LINE_MESSAGE ) {
		return	( true );
	}

	Feature_Attr	a_fAttr;
	long			a_lAttr_Code	= 0;
	a_fAttr			= c_fFAtt.Get_Attr ( annotation::kAnnoCode );
	a_lAttr_Code	= a_fAttr.e_ccvAttr.intVal;

	// �t�F���[�͕\���ʒu�͗v��Ȃ�
//	if( a_lAttr_Code == 203105 ||	// �t�F���[�q�H���̖��͓����n����
//		a_lAttr_Code == 203106 ) {	// ���̑����q�H���q�H���̖��͓����n����
//		return ( true );
//	}

	string	a_sWhereStr;
	char	a_cpStr[32];
	int		a_iDispFlag[4];
	sprintf	( a_cpStr, "%s = %d", disp_line::kAnnoID, a_dTruePtInfo.e_iOid );	// Where��쐬
	a_sWhereStr	= a_cpStr;
	int	a_iStart	= 0, a_iEnd	= 4;
	if( m_iDigit == 8 ) {
		a_iStart		= 2;
		a_iEnd			= 3;
		a_iDispFlag[2]	= 1;
	}else {
		Feature_Attr	a_fDispSc[4];
		int	i = 0;
		std::map<int,const _TCHAR*> SC_Disptypes;
		SC_Disptypes[0] = annotation::kSC1DispType;
		SC_Disptypes[1] = annotation::kSC2DispType;
		SC_Disptypes[2] = annotation::kSC3DispType;
		SC_Disptypes[3] = annotation::kSC4DispType;
		for( i = 0; i < 4; i++)
		{
			a_fDispSc[i]	= c_fFAtt.Get_Attr ( SC_Disptypes[i] );
			a_iDispFlag[i]	= a_fDispSc[i].e_ccvAttr.intVal;
		}
	}

	IFeatureCursorPtr	a_ipAny_Note_Line_Cursor;
	int	a_iRoop	= 0;
	for( a_iRoop = a_iStart; a_iRoop < a_iEnd; a_iRoop++ )
	{
		if( a_iDispFlag[a_iRoop] == 0 ) continue;

		a_ipAny_Note_Line_Cursor	= c_mAny_Note_Line_WS[a_iRoop].Search_Where ( a_sWhereStr, RET_FCURSOR );
		if ( a_ipAny_Note_Line_Cursor == NULL ) {
			e_sPrnInfo.Prn_Info_Std ( "���L���C���t�B�[�`���̎擾�Ɏ��s", stderr );
			continue;
		}else {
		// ���C���f�[�^�̃��[�v
			IFeaturePtr		a_ipAny_Note_Line_Feat;
			for (;;)
			{
				if( a_ipAny_Note_Line_Cursor->NextFeature ( &a_ipAny_Note_Line_Feat ) == S_OK ) {
					disp_pt_info	a_dCurPtInfo;
					a_dCurPtInfo.e_iScale	= a_iRoop + 1;	// �X�P�[��������
					// �I�u�W�F�N�gID�̎擾
					a_ipAny_Note_Line_Feat->get_OID ( (long*)&a_dCurPtInfo.e_iOid );
					//�@�W�I���g���̎擾
					IGeometryPtr	a_ipCurPtGeo;
					a_ipAny_Note_Line_Feat->get_Shape ( &a_ipCurPtGeo );
				//	a_dCurPtInfo.e_ipPt	= Get_Center_Pt( a_ipCurPtGeo );
					a_dCurPtInfo.e_ipPt	= Get_Center_Pt2( a_ipCurPtGeo, c_iMeshCode, a_dCurPtInfo.e_iScale );	// �L���̃T�C�Y�͂P�U�ŌŒ�
					c_vPoints.push_back ( a_dCurPtInfo );
				}else {
					break;
				}
			}
		}
	}
	
	return	( true );
}

/** ---------------------------------------------------------------------------
//	���C������L�����S�ʒu�ɂ�����_�����o��
-----------------------------------------------------------------------------*/
IPointPtr	main_rootin::
Get_Center_Pt	(	IGeometryPtr	c_ipGeo )	// �W�I���g��
{
	IPointPtr			a_ipRetPt(CLSID_Point);
	IPolylinePtr		a_ipPolyline(c_ipGeo);
	IPointCollectionPtr	a_ipPts(a_ipPolyline);

	IPointPtr	a_ipPt[2];
	double	a_dX[2] = {0.0, 0.0};
	double	a_dY[2] = {0.0, 0.0};
	int i = 0;
	for( i = 0; i < 2;i++ )
	{
		a_ipPts->get_Point ( i, &a_ipPt[i] );
		a_ipPt[i]->get_X ( &a_dX[i] );
		a_ipPt[i]->get_Y ( &a_dY[i] );
	}
	
	double	a_dXDiff	= (a_dX[1] - a_dX[0]) / 2.0;
	a_ipRetPt->put_X ( a_dX[0] + a_dXDiff );
	a_ipRetPt->put_Y ( a_dY[0] + a_dXDiff );
	return ( a_ipRetPt );
}

/** ---------------------------------------------------------------------------
//	���C������L�����S�ʒu�ɂ�����_�����o��
-----------------------------------------------------------------------------*/
IPointPtr	main_rootin::
Get_Center_Pt2	(	IGeometryPtr	c_ipGeo, 		///< �W�I���g��
					int				c_iMeshCode,	///< ���b�V���R�[�h
					int				c_iScale )		///< �X�P�[��
//					int				c_iFontSize )	///< �t�H���g�T�C�Y
{
	IPointPtr			a_ipRetPt(CLSID_Point);
	IPolylinePtr		a_ipPolyline(c_ipGeo);
	IPointCollectionPtr	a_ipPts(a_ipPolyline);

	double	a_dXDiff = 0.0, a_dYDiff = 0.0;

//	Get_Degree_Range_Dev ( c_iMeshCode, c_iScale, c_iFontSize, &a_dXDiff, &a_dYDiff );
///** 2006.04.18 �R�����g�A�E�g
	Get_Degree_Range_Dev ( c_iMeshCode, c_iScale, 16, &a_dXDiff, &a_dYDiff );
	// �v�Z�덷�̂��߂��AIN�f�[�^�Ƃ̊Ԃ�+1pt�̍����ł���̂ŁA-1.0pt���Ă��� 2006.08.10
	a_dXDiff	= a_dXDiff * (7.5-1.0) / 16.0;
	a_dYDiff	= a_dYDiff * (8.5-1.0) / 16.0;
//*/

/* 2006.08.09 �R�����g�A�E�g
	Get_Degree_Range_Dev ( c_iMeshCode, c_iScale, 1, &a_dXDiff, &a_dYDiff );
	a_dXDiff	*= 8.0;
	a_dYDiff	*= 8.0;
*/
	IPointPtr	a_ipPt;
	double	a_dX = 0.0;
	double	a_dY = 0.0;
	a_ipPts->get_Point ( 0, &a_ipPt ); //�@�n�_�����
	a_ipPt->get_X ( &a_dX );
	a_ipPt->get_Y ( &a_dY );
	
	a_ipRetPt->put_X ( a_dX + a_dXDiff );
	a_ipRetPt->put_Y ( a_dY + a_dYDiff );
	return ( a_ipRetPt );
}

/** ---------------------------------------------------------------------------
	�|�C���g�ƃA�[�N�̋��������߂�
	@return	double	����
-----------------------------------------------------------------------------*/
double	main_rootin::
Get_Dist	(	IPointPtr&				c_ipPt,			///< �|�C���g[IN]
				const IGeometryPtr&			c_ipLine	)	///< ���C��[IN]
{
	//-------------------------------------------------------------------------
	// �����p
	//-------------------------------------------------------------------------
	Dpos		a_dPt;	// �|�C���g�̍��W�l
	a_dPt.xpos	= 0.0, a_dPt.ypos	= 0.0;

	c_ipPt->get_X ( &a_dPt.xpos );
	c_ipPt->get_Y ( &a_dPt.ypos );
	//-------------------------------------------------------------------------
	// ���C���̃f�[�^���擾
	//-------------------------------------------------------------------------
	poly_cls			a_pLine;
	IPointCollectionPtr	a_ipLine_Pts;
	a_ipLine_Pts		= c_ipLine;
	
	// �S���̓_�̋��������߂āA���X�g�ɂ���
	long	a_lCount	= 0;
	a_ipLine_Pts->get_PointCount ( &a_lCount );

	int	roop_1	= 0;
	for(roop_1 = 0; roop_1 < a_lCount; roop_1++)
	{
		IPointPtr	a_ipCurPt;
		a_ipLine_Pts->get_Point	( roop_1, &a_ipCurPt );
	
		Dpos	a_dTmpPt;	// XY�����̂܂܎g��

		a_ipCurPt->get_X	( &a_dTmpPt.xpos );
		a_ipCurPt->get_Y	( &a_dTmpPt.ypos );
		a_pLine.e_Pts.push_back ( a_dTmpPt );
	}
	double	a_dDist	= 0.0;
	a_dDist	= a_pLine.get_pt_arc_dist( a_dPt );	// ���̋����̋��ߕ���OK�IGet_Poly_Dist�͖�肠��
	return	( a_dDist );
}

/** ---------------------------------------------------------------------------
//	SJIS�ւ̕ϊ�
//	@return	�ϊ��㕶����
-----------------------------------------------------------------------------*/
string	main_rootin::
Str_to_SJIS		(	CComBSTR*	c_CStr	)	// ������(Unicode)
{	
	string		ret_str;

#ifndef	_ARC_9_1_
	USES_CONVERSION;
	if (*c_CStr != 0) {
		ret_str = OLE2T(*c_CStr);
	}
#else
	long		length	= c_CStr->Length();

	char*		tmp_moji	= new char[length+1];

	memset		(tmp_moji, '\0', length+1);
	_bstr_t		a_bTmp ( *c_CStr, false );
//	wchar_t*	tmp_wide	= OLE2W(*c_CStr);
	wchar_t*	tmp_wide	= (wchar_t *)a_bTmp;

	if(tmp_wide == NULL) {
		ret_str	= "";
	}else {
		if(wcstombs( tmp_moji, tmp_wide, length+1 ) == -1){
			ret_str	= "";
		}else {
			ret_str	= tmp_moji;
		}
	}
	delete [] tmp_moji;
#endif	// endof _ARC_9_1_
	return	(ret_str);
}

/** ---------------------------------------------------------------------------
//	�G���[���O�t�@�C�����I�[�v������
//	@return	����	true
//	@return	���s	false
-----------------------------------------------------------------------------*/
bool	main_rootin::
Open_Err_Log	( void )
{
	if( e_cpLog_File == NULL ) {
		e_fErr_Fp	= stderr;
	}else {
		e_fErr_Fp	= fopen ( e_cpLog_File, "w" );
		if( e_fErr_Fp == NULL ) {
			fprintf	( stderr, "�G���[���O�t�@�C��<%s>���I�[�v���ł��Ȃ�[main_rootin]\n", e_cpLog_File );
			return	( false );
		}
	}

	return	( true );
}

/** ---------------------------------------------------------------------------
//	�G���[���O�̃w�b�_���o�͂���
//	@return	����
-----------------------------------------------------------------------------*/
void	main_rootin::
Output_Log_Header	( long		c_lDigit	)	///< ���b�V������[IN]
{
	fprintf	( e_fErr_Fp,		"#SINDYSTDLOG\n" );	// 2007.03
	fprintf	( e_fErr_Fp,		"#�c�[���o�[�W����          : <%s>\n", e_sVersion.c_str() );
	if( eExe_Flag & ON_PGDB_MODE ) {
		fprintf	( e_fErr_Fp,	"#�ڑ����(PGDB)            : <%s>\n",				e_cpPGDB_File );
	}else {
		fprintf	( e_fErr_Fp,	"#�ڑ����(DB_MAP)          : <%s/%s/%s/%s/%s>\n",	e_pTool_Init.get_UserName_Map(), e_pTool_Init.get_Password_Map(),
																					e_pTool_Init.get_Version_Map(),	e_pTool_Init.get_Instance_Map(),
																					e_pTool_Init.get_Server_Map() );
		fprintf	( e_fErr_Fp,	"#�ڑ����(DB_BASE)         : <%s/%s/%s/%s/%s>\n",	e_pTool_Init.get_UserName_Base(), e_pTool_Init.get_Password_Base(),
																					e_pTool_Init.get_Version_Base(), e_pTool_Init.get_Instance_Base(),
																					e_pTool_Init.get_Server_Base() );
	}
	fprintf	( e_fErr_Fp,		"#�S�����C��                : <%s>\n", m_sAny_Railway_Layer.c_str() );
	fprintf	( e_fErr_Fp,		"#���L�^�ʒu���C��          : <%s>\n", m_sAny_Note_Layer.c_str() );
	if( c_lDigit == 8 ) {
			fprintf	( e_fErr_Fp,"#���L�\�����C�����C��      : <%s>\n", m_sAny_Note_Line_Layer[2].c_str() );
	}else {
		int i = 0;
		for ( i = 0; i < 4; i++ )
		{
			fprintf	( e_fErr_Fp,"#���L�\�����C�����C��%d     : <%s>\n", i+1, m_sAny_Note_Line_Layer[i].c_str() );
		}
	}
	fprintf	( e_fErr_Fp,		"#�S�������͈�              : <%0.0fpt>\n", e_dDev_Dist );
}

#define	OPTION_MAX	14
#define	BMAX		512

/** ---------------------------------------------------------------------------
//	�����`�F�b�N
//	@return	����	true
//	@return	����	false
-----------------------------------------------------------------------------*/
bool	main_rootin::
arg_check	(	int		argc,		// ������
				char	**argv	)	// ������
{
	int		a_optcount[OPTION_MAX];	// �I�v�V�����̏d���̃J�E���g
	int		i;
	int		a_exeflag	= 0;

	//-----------------------------------------------------------------------
	// �c�[���o�[�W�����\�� 2008.09.05
	CVersion	a_cvTool_Var;
	CString		a_csVar;
	a_csVar		= a_cvTool_Var.GetFileVersion();
	e_sVersion	= (char*)(_bstr_t(a_csVar));
	//-----------------------------------------------------------------------

	for(i = 0; i < OPTION_MAX; i++)
	{
		a_optcount[i] = 0;
	}

	// ���������񐔂̃`�F�b�N
	for(i = 0; i < argc; i++)
	{
		if(strlen(argv[i]) > BMAX) {
			fprintf	( stderr, "���������񂪒������܂�!!\n" );
			return	( false );
		}
	}

	if(argc < 3) {
		return( false );
	}

//	e_cpLog_File		= argv[4];	///< ���O�f�[�^�o�̓p�X

	// �I�v�V�������Ȃɂ������Ƃ�
	if( argc == 1 ) {
		return	( true );
	}

	// �I�v�V����������A���̂Ƃ���g�p���Ȃ�
	for(i = 1; i < argc;)
	{
		if(strcmp(argv[i], "-l") == 0) {
			if(argc < i+2) return( false );
			e_cpLog_File	= argv[i+1];		///< ���O�t�@�C��
			a_exeflag		|= ON_LOGFILE;
			a_optcount[0]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-m") == 0) { // 2002.9.24
			if(argc < i+2) return( false );
			e_cpMeshCode_File	= argv[i+1];	///< ���b�V���R�[�h���X�g�t�@�C��
			a_exeflag		|= ON_MESH;
			a_optcount[1]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-v") == 0) { // 2002.9.24
			//if(argc < i+1) return( false );
			print_version();
			a_optcount[2]++;
			i+=1;
			return	( false );	// ���ɂ�������H
		} else
		if(strcmp(argv[i], "-w") == 0) {		//	���[�j���O�����w�� 2002.9.26
		//	if(argc < i+2) return( false );
		//	e_Warn_Dist		= atof(argv[i+1]);
		//	a_optcount[3]++;
		//	i+=2;
		} else
		if(strcmp(argv[i], "-e") == 0) {		//	�G���[�����w�� 2002.9.26
		//	if(argc < i+2) return( false );
		//	e_Err_Dist		= atof(argv[i+1]);
		//	a_optcount[4]++;
		//	i+=2;
		} else
		if(strcmp(argv[i], "-M") == 0) { // 2003.4.1
			if(argc < i+2) return( false );
			a_exeflag		|= ON_MONO_MESH;
			e_MeshCode		= atoi(argv[i+1]);	
			a_optcount[5]++;
			i+=2;
		} else 
		if(strcmp(argv[i], "-i") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_INIT_FILE;
			e_cpInit_File	= argv[i+1];	
			a_optcount[6]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-r") == 0) {		//	���[�j���O�����w�� 2002.9.26
			if(argc < i+2) return( false );
			e_dDev_Dist		= atof(argv[i+1]);
			a_optcount[7]++;
			i+=2;
		} else
	//	if(strcmp(argv[i], "-N") == 0) {		// ���H�ԍ������`�F�b�N�̏ꍇ		// 2004.2.3
	//		//if(argc < i+1) return( false );
	//		a_exeflag		|= ON_ROADNUM_ONLY;
	//		a_optcount[8]++;
	//		i+=1;
	//	} else 
	//	if(strcmp(argv[i], "-F") == 0) {		// �t�F���[�֌W�����`�F�b�N�̏ꍇ	// 2004.2.3
	//		//if(argc < i+1) return( false );
	//		a_exeflag		|= ON_FELLY_ONLY;
	//		a_optcount[9]++;
	//		i+=1;
	//	} else
		if(strcmp(argv[i], "-pgdb") == 0) {	/// 2007.02.23
			if(argc < i+2) return( false );
			e_cpPGDB_File	= argv[i+1];
			a_exeflag		|= ON_PGDB_MODE;
			a_optcount[10]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-addmsg") == 0) {		//	�ו����s�v�̂��̂ɂ��Ă����O���o��
			a_exeflag		|= ON_OUT_FUYOU_MESSAGE;
			a_optcount[11]++;
			i+=1;
		} else
		if(strcmp(argv[i], "-ddsplmsg") == 0) {	//	�\�����C���ɂ��Ẵ��b�Z�[�W���o�͂��Ȃ�
			a_exeflag		|= ON_DEL_DISP_LINE_MESSAGE;
			a_optcount[12]++;
			i+=1;
		} else
		if(strcmp(argv[i], "-nearest") == 0) {	//	��ԋ߂��H���Ƃ̂݃`�F�b�N���s��
			a_exeflag		|= ON_NEAREST_CHK;
			a_optcount[13]++;
			i+=1;
		} else {
			fprintf	(stderr, "�I�v�V���� %s �͊Ԉ���Ă܂�!!\n", argv[i]);
			return	( false );
		}
	}

	// �s���R�[�h���X�g�̎w��ƁA���b�V���R�[�h���X�g�̎w�肪�����ɏo���Ȃ��悤�Ƀ`�F�b�N
	if( (a_exeflag & ON_MESH) && (a_exeflag & ON_MONO_MESH) ) {
		fprintf	(stderr, "�I�v�V���� '-m','-M',�͓����Ɏw��ł��܂���!!\n");
		return	( false );
	}else {
		if( !(a_exeflag & ON_MESH || a_exeflag & ON_MONO_MESH) ) {	// ���b�V���R�[�h���X�g��ǂݍ���ł̃`�F�b�N�̏ꍇ
			fprintf	(stderr, "�I�v�V���� '-m','-M',�̂ǂ��炩���w�肵�Ă�������\n");
			return	( false );
		}
	}


	// �I�v�V�����d���̃`�F�b�N
	for(i = 0; i < OPTION_MAX; i++)
	{
		if(a_optcount[i] > 1) {
			fprintf	(stderr, "�I�v�V�����̎w�肪�d�����Ă��܂�!!\n");
			return	( false );
		}
	}
	eExe_Flag	= a_exeflag;

	int	a_iInit_Ret	= 0;			// 2007.11.07
	if( a_exeflag & ON_INIT_FILE ) {// 2007.11.07
		a_iInit_Ret	= e_pTool_Init.read_init_file ( Ret_Init_File() );
	}else {
		a_iInit_Ret	= e_pTool_Init.read_init_env ( );
	}
	if( a_iInit_Ret != 0 ) {
		fprintf ( stderr, "�������t�@�C��<%s>�̃I�[�v���Ɏ��s���܂���\n", Ret_Init_File() );
		return	( false );
	}

//	return		( a_exeflag );
	return		( true );

}

/** ---------------------------------------------------------------------------
//	�c�[���̎g�p�@�̕\��
-----------------------------------------------------------------------------*/
void		main_rootin::
print_usage	(char **argv)
{
	fprintf(	stdout,
			"=== �y�S���H�����L�ו����`�F�b�N�c�[���z(%s) ===\n"
				"<usage>\n"
				"%s [�I�v�V����...]\n"
				"\t[�I�v�V����...]\n"
				"\t      '-r'               : �f�o�C�X���W�ł͈̔͂��w��i�w��|�C���g�ȓ��ł����OK�Ƃ���j\n"
				"\t                         : ���f�t�H���g�l��16pt�Ƃ��Ă���\n"
				"\t      '-i'               : �ݒ�t�@�C����\n"
				"\t      '-i'               : �ݒ�t�@�C�����w��\n"
				"\t      '-l'               : ���O�t�@�C�������w��\n"
				"\t      '-m'               : ���b�V���R�[�h���X�g�t�@�C�����w��\n"
				"\t      '-M'               : ���b�V���R�[�h���w��i2���A8�����b�V���j\n"
				"\t      '-pgdb'            : PGDB�f�[�^�ɑ΂��Ẵ`�F�b�N\n"
				"\t                           ��SiNDY-C�p�̋@�\�łȂ����Ƃɒ��ӁB\n"
				"\t      '-addmsg'          : �ו����s�v�̂��̂ɂ��Ă����O���o��\n"
				"\t      '-ddsplmsg'        : �\�����C���ɂ��Ẵ��b�Z�[�W���o�͂��Ȃ�\n"
				"\t      '-nearest'         : ��ԋ߂��H���Ƃ̂݃`�F�b�N���s��\n"
				"\t      '-update'			: ��������\n"
				"\t      '-v'               : �o�[�W�������\��\n"
				"\t[���ϐ�]\n"
				"\t  <����>\n"
				"\t      DB_MAP                : <���[�U��>/<�p�X���[�h>/<�o�[�W����>/<�C���X�^���X>/<�T�[�o>\n"
				"\t      DB_BASE               : <���[�U��>/<�p�X���[�h>/<�o�[�W����>/<�C���X�^���X>/<�T�[�o>\n"
				"\t      FC_CITYMESH           : �s�s�n�}���b�V��FC\n"
				"\t      FC_BASEMESH           : ���k�ڃ��b�V��FC\n"
				"\t      FC_MIDDLEMESH         : �~�h�����b�V��FC\n"
				"\t  <8�����b�V���w�莞>\n"
				"\t      FC_CITY_ANNOTATION    : �s�s�n�}���L�^�ʒuFC\n"
				"\t      FC_CITY_DISP_LINE     : �s�s�n�}���L���C��FC\n"
				"\t      FC_CITY_RAILWAY       : �s�s�n�}�S�����C��FC\n"
				"\t  <2�����b�V���w�莞>\n"
				"\t      FC_BASE_ANNOTATION    : �x�[�X���L�^�ʒuFC\n"
				"\t      FC_B_SC1DISP_LINE     : �x�[�X���LS1�\�����C��FC\n"
				"\t      FC_B_SC2DISP_LINE     : �x�[�X���LS2�\�����C��FC\n"
				"\t      FC_B_SC3DISP_LINE     : �x�[�X���LS3�\�����C��FC\n"
				"\t      FC_B_SC4DISP_LINE     : �x�[�X���LS4�\�����C��FC\n"
				"\t      FC_BASE_RAILWAY       : �x�[�X�S�����C��FC\n"
				"\t  <middle���b�V���w�莞>\n"
				"\t      FC_MIDDLE_ANNOTATION  : �~�h�����L�^�ʒuFC\n"
				"\t      FC_M_SC1DISP_LINE     : �~�h�����LS1�\�����C��FC\n"
				"\t      FC_M_SC2DISP_LINE     : �~�h�����LS2�\�����C��FC\n"
				"\t      FC_M_SC3DISP_LINE     : �~�h�����LS3�\�����C��FC\n"
				"\t      FC_M_SC4DISP_LINE     : �~�h�����LS4�\�����C��FC\n"
				"\t  ����L�̊��ϐ��ł̐ݒ��'-i'�I�v�V�����ł����l�ɐݒ�ł��܂�\n"
			//	"\t      '-w'               : ���[�j���O�����w��(�P��:km)\n"
			//	"\t      '-e'               : �G���[�����w��(�P��:km)\n"
			, e_sVersion.c_str(), argv[0]);
}

/** ---------------------------------------------------------------------------
//	�o�[�W�������̕\��
//	@return		����
-----------------------------------------------------------------------------*/
void	main_rootin::
print_version	( void )
{
	fprintf(	stdout,
			"=== �y�S���H�����L�ו����`�F�b�N�c�[���z(%s) ===\n"
				"\t<�o�[�W�������>\n"
				"\t2008.09.16     0.9.0.1            �E�V�K�쐬�i���p�o�[�W�����j\n"
				"\t2008.09.16     0.9.1.2            �E[Bug 6409]�ւ̑Ή��B�i�^�ʒu�̏���ǉ��j\n"
				"\t2008.10.17     1.0.0.3            �E�������J�Ńc�[��\n"
			 	, e_sVersion.c_str());
}

/** ---------------------------------------------------------------------------
//	���b�V����`��DB����l�����郁�\�b�h�̏����� \
//	��get_mesh_rect �����O�ɕK���Ă�
-----------------------------------------------------------------------------*/
bool	main_rootin::
get_mesh_init	(	string			c_sMeshLayer,		// ���b�V�����C����
					IWorkspacePtr	c_ipWorkSpace	)	// �w�肷�郏�[�N�X�y�[�X
{
	IFeatureWorkspacePtr	a_F_Workspace	( c_ipWorkSpace );	// �t�B�[�`�����[�N�X�y�[�X

	// ���C���̃I�[�v��
	char*	a_Layer_Name		= &c_sMeshLayer[0];		// ����Ȃ�ł����̂��H�H2002/4/30

	if( FAILED(a_F_Workspace->OpenFeatureClass(CComBSTR(a_Layer_Name), &e_ipF_Class )) ) {
		fprintf	(stderr, "���C�� <%s> �̃I�[�v���Ɏ��s\n", a_Layer_Name);
		return	(false);
	}else {
		fprintf(stdout,	"<%s> ���C�����I�[�v��\n", a_Layer_Name);
	}
	
	return	( true );
}

/** ---------------------------------------------------------------------------
//	���b�V����`��GeoDB����擾���� \
//	��GeoDB�ւ̐ڑ����m������Ă��邱�Ƃ��O��
-----------------------------------------------------------------------------*/
IEnvelopePtr	main_rootin::
get_mesh_rect	( int	c_iMeshCode	)	///< ���b�V���R�[�h
{

	CComBSTR	a_SpatialCol;
	e_ipF_Class->get_ShapeFieldName	( &a_SpatialCol );

	char		a_tmpstr[30];	//�@�����͂Ȃ�Ƃ��������C������

	crd_cnv		crd;
	int			a_iDigit	= 0;
	a_iDigit	= crd.GetMeshDigit( c_iMeshCode );
	if(a_iDigit == 4) {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// ���b�V���R�[�h�𕶎���� 2007.09.03 �C��
		m_iDigit	= 4;
	}else
	if(a_iDigit == 6) {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// ���b�V���R�[�h�𕶎����
		m_iDigit	= 6;
	}else {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// ���b�V���R�[�h�𕶎���� 2007.09.03 �C��
		m_iDigit	= 8;
	}
	_bstr_t		a_Where_Str ( a_tmpstr );
//	a_Where_Str	= T2BSTR(_T(a_tmpstr));		// string��BSTR�ɕϊ�

	IQueryFilterPtr		a_Sp_Filter	( CLSID_QueryFilter );			// ��Ԍ����t�B���^
//	a_Sp_Filter->put_SearchOrder	( esriSearchOrderAttribute );	// ���������D��
	a_Sp_Filter->put_WhereClause	( a_Where_Str);					// Where��̐ݒ�
	
	// �N���b�v��`�쐬
	IEnvelopePtr	a_ipMesh_Rect;	// ���b�V����`
	a_ipMesh_Rect	= NULL;

	// �T�[�`���Ă���
	IFeatureCursorPtr	a_F_Cursor;
	if( FAILED	(e_ipF_Class->Search	( a_Sp_Filter, VARIANT_FALSE, &a_F_Cursor )) ) {
		fprintf	(stderr, "�f�[�^�̑��������Ɏ��s\n");
		return	(NULL);
	}else {
		IFeaturePtr				a_Feature;
		long					a_lSp_InitFlag	= 0;	// ��ԎQ�Ƃ��������������ǂ����̃t���O

		for(;;)
		{
			if( a_F_Cursor->NextFeature ( &a_Feature ) != S_OK) {	// �f�[�^���ЂƂ��o��
				break;
			}
				
			IGeometryPtr		a_Clip_Shp;		// �����Ƀf�[�^���R�s�[���ăN���b�v����

			if( FAILED(a_Feature->get_ShapeCopy	( &a_Clip_Shp )) ) {	// �t�B�[�`������
				fprintf	(stderr, "�V�F�C�v�̃R�s�[�Ɏ��s\n");
				continue;
			}
			
			VARIANT_BOOL	a_bResult;
			a_Clip_Shp->get_IsEmpty( &a_bResult );
			if(a_bResult == VARIANT_TRUE) {	// �N���b�v���ʂ�NULL�Ȃ烁�b�V���O
				continue;
			}
			if( FAILED(a_Clip_Shp->get_Envelope( &a_ipMesh_Rect) )) {
				fprintf	( stderr, "�Y�����b�V��<%d>�̋�`���݂���܂���ł���\n", c_iMeshCode);
				return	( NULL );
			}else {
				return	( a_ipMesh_Rect );
			}
		}
	}
	return	( a_ipMesh_Rect );
}

/** ---------------------------------------------------------------------------
//	�w��|�C���g�𒆐S�ɁA�w�肵��X,Y�����̕�����������`���쐬
-----------------------------------------------------------------------------*/
IEnvelopePtr	main_rootin::
make_rect	(	IPointPtr&		c_ipPt,				///< ���S�|�C���g
				double			c_dDegX,			///< X������
				double			c_dDegY	)			///< Y������
{
	IEnvelopePtr	a_ipRetRect (CLSID_Envelope);

	double	a_dCenter_X = 0.0, a_dCenter_Y = 0.0;

	c_ipPt->get_X ( &a_dCenter_X );
	c_ipPt->get_Y ( &a_dCenter_Y );

	ISpatialReferencePtr	a_ipSpRef;
	if( FAILED (c_ipPt->get_SpatialReference ( &a_ipSpRef ) )) {
		return	( NULL );
	}

	// ��ԎQ�ƍ��킹
	if( FAILED ( a_ipRetRect->putref_SpatialReference ( a_ipSpRef ) )) {
		return	( NULL );
	}

	// ���W�l�t�^
	a_ipRetRect->put_XMin ( a_dCenter_X - (c_dDegX) );
	a_ipRetRect->put_XMax ( a_dCenter_X + (c_dDegX) );
	a_ipRetRect->put_YMin ( a_dCenter_Y - (c_dDegY) );
	a_ipRetRect->put_YMax ( a_dCenter_Y + (c_dDegY) );

	return	( a_ipRetRect );

}

/** ---------------------------------------------------------------------------
//	DB�ւ̐ڑ��I�[�v��
//	@return	true	����
//	@return	false	���s
-----------------------------------------------------------------------------*/
bool	main_rootin::
DB_Open	(	IWorkspacePtr&			c_ipWorkspace_Map,			
			IWorkspacePtr&			c_ipWorkspace_Base )
{
	USES_CONVERSION;	// OLE2T ���̈ꕔ�̃}�N�����g�p����ꍇ�ɂ͕K���K�v

	fprintf	(	stdout,
				_T(	"DB_MAP �ڑ��v���p�e�B�F%s/%s/%s/%s/%s\n"),
				e_pTool_Init.get_UserName_Map(), 
				e_pTool_Init.get_Password_Map(),
				e_pTool_Init.get_Version_Map(),
				e_pTool_Init.get_Instance_Map(),
				e_pTool_Init.get_Server_Map() );

	fprintf	(	stdout,
				_T(	"DB_BASE �ڑ��v���p�e�B�F%s/%s/%s/%s/%s\n"),
				e_pTool_Init.get_UserName_Base(), 
				e_pTool_Init.get_Password_Base(),
				e_pTool_Init.get_Version_Base(),
				e_pTool_Init.get_Instance_Base(),
				e_pTool_Init.get_Server_Base() );

	CString connectStrMap, connectStrBase;
		connectStrMap.Format(_T("%s/%s/%s/%s/%s"),
		e_pTool_Init.get_UserName_Map(), e_pTool_Init.get_Password_Map(),
		e_pTool_Init.get_Version_Map(), e_pTool_Init.get_Instance_Map(), e_pTool_Init.get_Server_Map());

	connectStrBase.Format(_T("%s/%s/%s/%s/%s"),
		e_pTool_Init.get_UserName_Base(), e_pTool_Init.get_Password_Base(),
		e_pTool_Init.get_Version_Base(), e_pTool_Init.get_Instance_Base(), e_pTool_Init.get_Server_Base());

	c_ipWorkspace_Map = sindy::create_workspace(connectStrMap);
	c_ipWorkspace_Base = sindy::create_workspace(connectStrBase);

	// DB�֐ڑ�
	fprintf ( stdout, "�s�s�n�}�n�c�a�ڑ���..." );
	if( !c_ipWorkspace_Map ) {
		fprintf ( stderr, "�s�s�n�}�n�c�a�ɐڑ��o���Ȃ�����\n" );
		return	( false );
	}else {
		fprintf ( stdout, "�s�s�n�}�n�c�a�ڑ�����\n" );
	}
	
	fprintf ( stdout, "���k�ڌn�c�a�ڑ���..." );
	if( !c_ipWorkspace_Base ) {
		fprintf ( stderr, "���k�ڌn�c�a�ɐڑ��o���Ȃ�����\n", stderr );
		return	( false );
	}else {
		fprintf ( stdout, "�ڑ�����\n" );
	}
	return	( true );
}

/** ---------------------------------------------------------------------------
//	�`�F�b�N�̏���������
-----------------------------------------------------------------------------*/
bool	main_rootin::
check_init	( void )
{
	// �G���[���O�t�@�C���̃I�[�v��
	if( !Open_Err_Log () ) {
		return	( false );
	}else {
		if( !e_sPrnInfo.Init(e_fErr_Fp) ) {
			return	( false );
		}
	}

	if( eExe_Flag & ON_PGDB_MODE ) {
		// �p�[�\�i���W�IDB�p�ł�
		IWorkspaceFactoryPtr	ipWorkspaceFactory	( CLSID_AccessWorkspaceFactory );
		IPropertySetPtr			ipPropertySet		( CLSID_PropertySet );					// �ڑ��v���p�e�B�[�i�[�p
		IWorkspacePtr			ipWorkspace;

		// �J�����g�����ɃI�[�v�����Ă݂�
		_bstr_t	a_bsTmpFName	= e_cpPGDB_File;
		ipWorkspaceFactory->OpenFromFile( a_bsTmpFName, 0, &ipWorkspace );
		if( ipWorkspace == NULL )	{
		//	e_sPrnInfo.Prn_Info_Std ( "�p�[�\�i���W�IDB���I�[�v���ł��܂���ł���\n", stderr ); 
			fprintf	( stderr, "�p�[�\�i���W�IDB���I�[�v���ł��܂���ł���\n" );
			return	( false );
		}else {
			fprintf	( stdout, "�ڑ�����\n" );
		//	e_sPrnInfo.Prn_Info_Std ("�ڑ�����", stdout );
		}
		printf(_T("\n"));

		m_ipWSpace_Map	= ipWorkspace;
		m_ipWSpace_Base	= ipWorkspace;

	}else {
		// �s�s�n�}�n�p�ڑ�
		IWorkspacePtr			ipWorkspace_Map;
		// ���k�ڌn�p�ڑ�
		IWorkspacePtr			ipWorkspace_Base;

		if( !DB_Open (ipWorkspace_Map, ipWorkspace_Base) ) {
			return	( false );
		}

		ATLASSERT( ipWorkspace_Map != NULL );
		ATLASSERT( ipWorkspace_Base != NULL );

		m_ipWSpace_Map  = ipWorkspace_Map;
		m_ipWSpace_Base = ipWorkspace_Base;
	}

	// �G���[���b�Z�[�W�p
	char	a_cpErrMsgBuf[512];

	// �`�F�b�N���b�V�����X�g�̓ǂݍ���
	char*	a_cpList_File	= NULL;		//	���X�g�t�@�C����
	int		a_iMDigit	= 0;

	if( eExe_Flag & ON_MESH ) {
		a_cpList_File	= Ret_Mesh_File ( );
		if( a_cpList_File == NULL ) {
			e_sPrnInfo.Prn_Info_Std ( "���b�V���R�[�h���X�g�t�@�C�������Ƃ肾���܂���ł���\n" , stderr );
			return	( false );
		}	
		if( m_mChk_List.read_mesh_list ( a_cpList_File ) != 0 ) {	// �����Ń��X�g��ǂݍ���
			sprintf	( a_cpErrMsgBuf, "���b�V���R�[�h���X�g�t�@�C��<%s>�̓ǂݍ��݂Ɏ��s\n", a_cpList_File );
			e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
			return	( false );
		}
	}else {
		if( m_mChk_List.read_mesh_list ( e_MeshCode ) != 0 ) {	// �����Ń��X�g��ǂݍ���
			sprintf	( a_cpErrMsgBuf, "���b�V���R�[�h���X�g�t�@�C��<%s>�̓ǂݍ��݂Ɏ��s\n", a_cpList_File );
			e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
			return	( false );
		}
	}

	char*		a_cpMeshLayer	= NULL;

	a_iMDigit	= m_mChk_List.get_mesh_digit ();
	
	switch	( a_iMDigit )
	{
	case	8:	a_cpMeshLayer	= e_pTool_Init.get_City_Mesh ();	break;
	case	6:	a_cpMeshLayer	= e_pTool_Init.get_Base_Mesh();		break;
	case	4:	a_cpMeshLayer	= e_pTool_Init.get_Middle_Mesh();	break;
	default:
		fprintf	( stderr, "���X�g�̃��b�V�������ɂ͑Ή����Ă��܂���[check_exe]\n");
		return	( false );
	}

	if( !get_mesh_init( a_cpMeshLayer , m_ipWSpace_Map ) ) {	// ���b�V���̃f�[�^�͂ǂ���DB�ł��Q�Ƃł���͂��E�E�E
		e_sPrnInfo.Prn_Info_Std ( "���b�V�����C���̊l���̏������Ɏ��s[check_exe]\n" , stderr );
		return	( false );
	}
	
	switch	( a_iMDigit )
	{
	case	8:
		m_sAny_Railway_Layer		= e_pTool_Init.get_City_RailWay();
		m_sAny_Note_Layer			= e_pTool_Init.get_City_Note();
		m_sAny_Note_Line_Layer[2]	= e_pTool_Init.get_City_Note_Line();
		break;
	case	6:
		m_sAny_Railway_Layer		= e_pTool_Init.get_Base_Railway();
		m_sAny_Note_Layer			= e_pTool_Init.get_Base_Note();
		m_sAny_Note_Line_Layer[0]	= e_pTool_Init.get_Base_Note_Line1();
		m_sAny_Note_Line_Layer[1]	= e_pTool_Init.get_Base_Note_Line2();
		m_sAny_Note_Line_Layer[2]	= e_pTool_Init.get_Base_Note_Line3();
		m_sAny_Note_Line_Layer[3]	= e_pTool_Init.get_Base_Note_Line4();
		break;
	case	4:
		m_sAny_Railway_Layer		= e_pTool_Init.get_Base_Railway();
		m_sAny_Note_Layer			= e_pTool_Init.get_Middle_Note();
		m_sAny_Note_Line_Layer[0]	= e_pTool_Init.get_Middle_Note_Line1();
		m_sAny_Note_Line_Layer[1]	= e_pTool_Init.get_Middle_Note_Line2();
		m_sAny_Note_Line_Layer[2]	= e_pTool_Init.get_Middle_Note_Line3();
		m_sAny_Note_Line_Layer[3]	= e_pTool_Init.get_Middle_Note_Line4();
		break;
	default:
		e_sPrnInfo.Prn_Info_Std ( "�������ԈႢ", stderr );
		return	( false );
	}

	if( !m_mAny_Railway_WS.Init ( m_ipWSpace_Base, (char*)m_sAny_Railway_Layer.c_str() ) ) {
		sprintf	( a_cpErrMsgBuf, "�S�����C��<%s>�̓ǂݍ��݂Ɏ��s\n", m_sAny_Railway_Layer.c_str() );
		e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
		return	( false );
	}

	if( !m_mAny_Note_WS.Init ( m_ipWSpace_Map, (char*)m_sAny_Note_Layer.c_str() ) ) {
		sprintf	( a_cpErrMsgBuf, "���L���C��<%s>�̓ǂݍ��݂Ɏ��s\n", m_sAny_Note_Layer.c_str() );
		e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
		return	( false );	
	}
	
	if( a_iMDigit == 8 ) {
		if( !m_mAny_Note_Line_WS[2].Init ( m_ipWSpace_Map, (char*)m_sAny_Note_Line_Layer[2].c_str() ) ) {
			sprintf	( a_cpErrMsgBuf, "���L���C�����C��<%s>�̓ǂݍ��݂Ɏ��s\n", m_sAny_Note_Line_Layer[2] );
			e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
			return	( false );	
		}
	}else {
		for( int i = 0; i < 4; i++ )
		{
			if( !m_mAny_Note_Line_WS[i].Init ( m_ipWSpace_Map, (char*)m_sAny_Note_Line_Layer[i].c_str() ) ) {
				sprintf	( a_cpErrMsgBuf, "���L���C�����C��<%s>�̓ǂݍ��݂Ɏ��s\n", m_sAny_Note_Line_Layer[i].c_str() );
				e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
				return	( false );	
			}
		}
	}
	// ���O�̃w�b�_�����o��
	Output_Log_Header( a_iMDigit );
	return	( true );
}

/** ---------------------------------------------------------------------------
//	�ǂݍ��񂾃f�[�^���Z�b�g�ɗ��Ƃ�
//	@return	�f�[�^��
-----------------------------------------------------------------------------*/
int		main_rootin::
Get_Data_info (	IFeatureCursorPtr&	c_ipCursor,		///< �`�F�b�N���J�[�\��[IN]
				ms_data_info&		c_ipGetData )	///< �`�F�b�N���f�[�^�̃Z�b�g[OUT]
{
	int	a_iCount	= 0;
	// �f�[�^���}���`�Z�b�g�̃f�[�^�ɗ��Ƃ�
	if( c_ipCursor != NULL ) {
		IFeaturePtr	a_ipFeature;
		// �������ꂽ���L�̃��[�v
		for(;;)
		{
			data_info	a_dCur_Data;
			if( c_ipCursor->NextFeature ( &a_ipFeature ) == S_OK ) {

				Feature_All_Attrs	a_fAttrs;
				a_fAttrs.Init		( a_ipFeature );
				// �I�u�W�F�N�gID
			//	Feature_Attr			a_fObjID, a_fMeshCode, a_fPrim_ID, a_fAttr, a_fRP_F;
			//	a_fObjID				= a_fChk_Src_Attrs.Get_Attr ( "OBJECTID" );
			//	a_fAttr					= a_fChk_Src_Attrs.Get_Attr ( c_sAttrFName );
			//	a_dChk_Data.e_iAttr		= a_fAttr.e_ccvAttr.intVal;
			
				a_dCur_Data.m_iOID		= a_fAttrs.Get_Obj_ID();
				a_dCur_Data.m_ipGeo		= a_fAttrs.Ret_Geo();

				a_dCur_Data.m_faaAttr	= a_fAttrs;

				c_ipGetData.insert ( a_dCur_Data );
				a_iCount++;
			}else {
				break;
			}
		}
	}
	return	( a_iCount );
}

/*
 * @brief     �n��̓S�����ו����\���𔻒肵�A�\�ł���Ȃ�ABLE_SUBDIVIDE��Ԃ�
 * @note      Anno_Railway_Chk���Ŏg�p
 * @param[in] targetAttr �`�F�b�N�ΏۘH���̓S�����
 * @param[in] rightAttr  �H���Ɛ������Ƃ�Ă���ꍇ�̓S�����
 * @param[in] underFlag  �Ώۂ̓S�����n��ɂ��邩�A�n���ɂ��邩
 * @retval    ABLE_SUBDIVIDE     �ו����\
 * @retval    RAIL_INCONSISTENT  �H���ƕs����
 * @retval    NONE_SUBDIVIDE     �ו����̕K�v�Ȃ�
*/
main_rootin::ERROR_MESSAGE_CODE checkRailwayClass_Ground(long targetAttr, long rightAttr, long underFlag )
{
	if( targetAttr == rightAttr ) //��ʂ̐��������Ă���Ȃ�A�ו����ł��邩����
	{
		if( underFlag == 1 )
			return main_rootin::ERROR_MESSAGE_CODE::ABLE_SUBDIVIDE;  // �ו����\
	}
	else return main_rootin::ERROR_MESSAGE_CODE::RAIL_INCONSISTENT;  // �H���ƕs����

	return main_rootin::ERROR_MESSAGE_CODE::NONE_SUBDIVIDE;          //�ו����̕K�v�Ȃ�
};

/*
 * @brief     �n���t���O�̏�Ԃƒ��L��ʂ̐��������Ă��邩���肵�A�n��A�n���̏�Ԃ����΂ɂȂ��Ă���Ȃ�AGROUND_INCONSISTENT��Ԃ�
 * @note      Anno_Railway_Chk���Ŏg�p
 * @param[in] targetAttr �`�F�b�N�ΏۘH���̓S�����
 * @param[in] rightAttr  �H���Ɛ������Ƃ�Ă���ꍇ�̓S�����
 * @param[in] underFlag  �Ώۂ̓S�����n��ɂ��邩�A�n���ɂ��邩
 * @retval    GROUND_INCONSISTENT �n��n��������
 * @retval    RAIL_INCONSISTENT   �H���ƕs����
 * @retval    NONE_SUBDIVIDE      �����͎��Ă���
*/
main_rootin::ERROR_MESSAGE_CODE checkRailwayClass_Under(long targetAttr, long rightAttr, long underFlag )
{
	if( targetAttr == rightAttr)
	{
		if( underFlag == 0 )
			return main_rootin::ERROR_MESSAGE_CODE::GROUND_INCONSISTENT; // �n��n��������
	}
	else
		return main_rootin::ERROR_MESSAGE_CODE::RAIL_INCONSISTENT;       // �H���ƕs����

	return main_rootin::ERROR_MESSAGE_CODE::NONE_SUBDIVIDE;              // �����͎��Ă���
};


/** ---------------------------------------------------------------------------
//	���L�ƓS���̑��������ă`�F�b�N
//	@return	�f�[�^��
-----------------------------------------------------------------------------*/
void	main_rootin::
Anno_Railway_Chk (	data_info&				c_dAnnoData,		///< ���L�f�[�^[IN]
					disp_pt_info&			c_dCurDat,			///< �\���ʒu�f�[�^[IN]
					ms_data_info&			c_mRailway_Data	)	///< �����œ���ꂽ�S���f�[�^[IN]
{
	using namespace sindy::schema::city_railway;
	namespace anno_code = annotation::annotation_code;

	// ���L�����擾
	Feature_Attr	a_fAnno_Attr = c_dAnnoData.m_faaAttr.Get_Attr ( annotation::kAnnoCode );
	long			a_lAnno_Attr = a_fAnno_Attr.e_ccvAttr.intVal;
	Feature_Attr	a_fAnno_Str  = c_dAnnoData.m_faaAttr.Get_Attr ( annotation::kNameString1 );
	string			a_sAnno_Str  = Str_to_SJIS(	(CComBSTR*)&a_fAnno_Str.e_ccvAttr.bstrVal );

	char	a_cpErrMsgBuf[512];

	// �������Ƀ\�[�g
	map<double, data_info, less<double> >	a_dDist_Sorted_Railway;
	ims_data_info	a_iIndx;
	for( a_iIndx = c_mRailway_Data.begin(); a_iIndx != c_mRailway_Data.end(); a_iIndx++ )
	{
		// ���������߂�
		double	a_dDist	= 0.0;
		a_dDist	= Get_Dist ( c_dCurDat.e_ipPt, a_iIndx->m_ipGeo );
		a_dDist_Sorted_Railway.insert ( map<double, data_info, less<double> >::value_type ( a_dDist, *a_iIndx ) );
	}

	// �\�[�g�����f�[�^�Ń��[�v������
	map<double, data_info, less<double> >::iterator	a_iSIndx;
	for( a_iSIndx = a_dDist_Sorted_Railway.begin(); a_iSIndx != a_dDist_Sorted_Railway.end(); a_iSIndx++ )
	{
		data_info a_dRailway = (*a_iSIndx).second;

		// ���������߂�
		double a_dDist = 0.0;
		a_dDist	= Get_Dist ( c_dCurDat.e_ipPt, a_dRailway.m_ipGeo );

		// �S�������擾
		//�s�s�n�}���b�V�����Ώۂ��ۂ��ɂ���āA�t�B�[���h����ς���
		Feature_Attr	a_fRailway_Attr = a_dRailway.m_faaAttr.Get_Attr( m_iDigit==8 ?
			city_railway::kRailwayClass : base_railway::kRailwayClass );
		long			a_lRailway_Attr = a_fRailway_Attr.e_ccvAttr.intVal;

		//�s�s�n�}���b�V�����Ώۂ��ۂ��ɂ���āA�t�B�[���h����ς���
		Feature_Attr	a_fUnderGround	= a_dRailway.m_faaAttr.Get_Attr ( m_iDigit==8 ?
			city_railway::kUnderGround : base_railway::kUnderGround );
		///0:�n�� 1:�n��
		long			a_lUnderGround	= a_fUnderGround.e_ccvAttr.intVal;

		/// �G���[���
		ERROR_MESSAGE_CODE a_lErr_Stat = NONE_SUBDIVIDE;

		if( m_iDigit != 8 ) {//�x�[�X���L�A���邢�́A�~�h�����L�p�̃`�F�b�N
			// �����Ń`�F�b�N
			switch ( a_lAnno_Attr )
			{
			case anno_code::kShinkansenLineName:  // 1101101 �V�����H�����i�n��j
			case anno_code::kJRLineName:          // 1101102 �i�q�H�����i�n��j
			case anno_code::kQuasiPublicLineName: // 1101103 ��R�Z�N�^�[�H�����i�n��j
			case anno_code::kPrivateLineName:     // 1101104 ���S�H�����i�n��j
			case anno_code::kSubwayLineName:      // 1101106 �n���S�H�����i�n��j
			case anno_code::kMonorailLineName:    // 1101107 ���m���[���H�����i�n��j
			case anno_code::kShinkotsuLineName:   // 1101108 �V��ʘH�����i�n��j
				if( a_lRailway_Attr == (a_lAnno_Attr-1101100) ) {// �e�H����ʂƂ̔�r
					if( a_lUnderGround == 1 ) {
						a_lErr_Stat	= ABLE_SUBDIVIDE;	// �ו����\
					}
				}else {
					a_lErr_Stat	= RAIL_INCONSISTENT;		// �H���ƕs����
				}
				break;
			case anno_code::kShinkansenSubLineName:  // 1101201 �V�����H�����i�n���j
			case anno_code::kJRSubLineName:          // 1101202 �i�q�H�����i�n���j
			case anno_code::kQuasiPublicSubLineName: // 1101203 ��R�Z�N�^�[�H�����i�n���j
			case anno_code::kPrivateSubLineName:     // 1101204 ���S�H�����i�n���j
			case anno_code::kSubwaySubLineName:      // 1101206 �n���S�H�����i�n���j
			case anno_code::kMonorailSubLineName:    // 1101207 ���m���[���H�����i�n���j
			case anno_code::kShinkotsuSubLineName:   // 1101208 �V��ʘH�����i�n���j
				if( a_lRailway_Attr == (a_lAnno_Attr-1101200) ) {// �e�H����ʂƂ̔�r
					if( a_lUnderGround == 0 ) {
						a_lErr_Stat	= GROUND_INCONSISTENT;	// �n��n��������
					}
				}else {
					a_lErr_Stat	= RAIL_INCONSISTENT;		// �H���ƕs����
				}
				break;
			default:
				a_lErr_Stat	= NON_SUBJECT;	// �ΏۊO���
				break;
			}
		}else {	// �s�s�n�}
			// �����Ń`�F�b�N
			switch ( a_lAnno_Attr )
			{
			//�n��Ɋւ��锻��
			case anno_code::kShinkansenLineName:  // 1101101 �V�����H�����i�n��j
			case anno_code::kJRLineName:          // 1101102 �i�q�H�����i�n��j
			case anno_code::kQuasiPublicLineName: // 1101103 ��R�Z�N�^�[�H�����i�n��j
			case anno_code::kPrivateLineName:     // 1101104 ���S�H�����i�n��j
				//�S����ʂ����ʓS���Œn���ɂ���Ȃ�ו����\
				a_lErr_Stat = checkRailwayClass_Ground(a_lRailway_Attr, railway_class::kNormalRailway, a_lUnderGround);
				break;
			case anno_code::kSubwayLineName:      // 1101106 �n���S�H�����i�n��j
				//�S����ʂ��n���S�Œn���ɂ���Ȃ�ו����\
				a_lErr_Stat = checkRailwayClass_Ground(a_lRailway_Attr, railway_class::kSubway, a_lUnderGround);
				break;
			case anno_code::kMonorailLineName:    // 1101107 ���m���[���H�����i�n��j
				//�S����ʂ����m���[���Œn���ɂ���Ȃ�ו����\
				a_lErr_Stat = checkRailwayClass_Ground(a_lRailway_Attr, railway_class::kMonorail, a_lUnderGround);
				break;
			case anno_code::kShinkotsuLineName:   // 1101108 �V��ʘH�����i�n��j
				//�S����ʂ��V��ʃV�X�e���Œn���ɂ���Ȃ�ו����\
				a_lErr_Stat = checkRailwayClass_Ground(a_lRailway_Attr, railway_class::kUniversalTraffic, a_lUnderGround);
				break;
			case anno_code::kCableCarLineName:    // 1101110 �P�[�u���J�[�H����
				//�S����ʂ��V��ʃP�[�u���J�[�Œn���ɂ���Ȃ�ו����\
				a_lErr_Stat = checkRailwayClass_Ground(a_lRailway_Attr, railway_class::kCableCar, a_lUnderGround);
				break;
			//�ȉ��A�n���Ɋւ��锻��
			case anno_code::kShinkansenSubLineName:  // 1101201 �V�����H�����i�n���j
			case anno_code::kJRSubLineName:          // 1101202 �i�q�H�����i�n���j
			case anno_code::kQuasiPublicSubLineName: // 1101203 ��R�Z�N�^�[�H�����i�n���j
			case anno_code::kPrivateSubLineName:     // 1101204 ���S�H�����i�n���j
				//�S����ʂ����ʓS���Œn��ɂ���Ȃ�n��n�����t
				a_lErr_Stat	= checkRailwayClass_Under(a_lRailway_Attr, railway_class::kNormalRailway, a_lUnderGround);
				break;
			case anno_code::kSubwaySubLineName:      // 1101206 �n���S�H�����i�n���j
				//�S����ʂ��n���S�Œn��ɂ���Ȃ�n��n�����t
				a_lErr_Stat	= checkRailwayClass_Under(a_lRailway_Attr, railway_class::kSubway, a_lUnderGround);
				break;
			case anno_code::kMonorailSubLineName:    // 1101207 ���m���[���H�����i�n���j
				//�S����ʂ����m���[���Œn��ɂ���Ȃ�n��n�����t
				a_lErr_Stat	= checkRailwayClass_Under(a_lRailway_Attr, railway_class::kMonorail, a_lUnderGround);
				break;
			case anno_code::kShinkotsuSubLineName:   // 1101208 �V��ʘH�����i�n���j
				//�S����ʂ��V��ʃV�X�e���Œn��ɂ���Ȃ�n��n�����t
				a_lErr_Stat	= checkRailwayClass_Under(a_lRailway_Attr, railway_class::kUniversalTraffic, a_lUnderGround);
				break;
			default:
				a_lErr_Stat	= NON_SUBJECT; // �ΏۊO���
				break;
			}
		}

		//-----------------------------------------------------------------------
		// �G���[�o��
		// <���L�����l>\t<���L��������>\t<�S�������l>\t<�S����������>\t<�^�ʒu���H>\t<�X�P�[��>���b�Z�[�W

		e_sPrnInfo.Set_OID  ( c_dCurDat.e_iOid );
		if( c_dCurDat.e_iStat == 1 ) {
			e_sPrnInfo.Set_Layer_Name	( (char*)m_sAny_Note_Layer.c_str() );
		}else {
			e_sPrnInfo.Set_Layer_Name	( (char*)m_sAny_Note_Line_Layer[c_dCurDat.e_iScale-1].c_str() );
		}

		// �������̎擾(�s�s�n�}�͓S������*10+�n���t���O�Ō���)

		long a_lRailKey = 0;
		if( m_iDigit == 8 ) {
			a_lRailKey = a_lRailway_Attr*10+a_lUnderGround;
		}else {
			a_lRailKey = a_lRailway_Attr;
		}

		string a_sAnnoAttr, a_sRailAttr;
		map<long, string, less<long> >::iterator a_iNameIndx;
		a_iNameIndx	= m_mAttrStrTbl.find ( a_lAnno_Attr );		// ���L������
		if( a_iNameIndx !=  m_mAttrStrTbl.end() ) {
			a_sAnnoAttr	= (*a_iNameIndx).second;
		}
		a_iNameIndx	= m_mAttrStrTbl.find ( a_lRailKey );		// �S��������
		if( a_iNameIndx !=  m_mAttrStrTbl.end() ) {
			a_sRailAttr	= (*a_iNameIndx).second;
		}

		string a_sErrMsg;

		switch (a_lErr_Stat)
		{
		case NONE_SUBDIVIDE:      //!< �ו����̕K�v����
			a_sErrMsg = "�ו����̕K�v�Ȃ�";
			break;
		case ABLE_SUBDIVIDE:      //!< �ו����\
			a_sErrMsg = "�ו����\";
			break;
		case RAIL_INCONSISTENT:   //!< �H���ƕs����
			a_sErrMsg = "�H���ƕs����";
			break;
		case GROUND_INCONSISTENT: //!< �n��n��������
			a_sErrMsg = "�n��n��������";
			break;
		default:                  //!< �ΏۊO���
			a_sErrMsg = "�ΏۊO���";
			break;
		}

		string	a_sStat	= "�^�ʒu";
		if( c_dCurDat.e_iStat == 0 ) a_sStat = "�\���ʒu";

		sprintf	( a_cpErrMsgBuf, "%s\t%s\tS%d\t%d\t%s\t%d\t%s\t%d\t%s\t%d\t%0.10f",
			a_sErrMsg.c_str(), a_sStat.c_str(), c_dCurDat.e_iScale, c_dAnnoData.m_iOID, a_sAnno_Str.c_str(), a_lAnno_Attr, a_sAnnoAttr.c_str(), a_lRailway_Attr, a_sRailAttr.c_str(), a_dRailway.m_iOID, a_dDist ); 


		// �ו����̕K�v�Ȃ����\�����邩�̔���
		if( !(eExe_Flag & ON_OUT_FUYOU_MESSAGE) ) {
			if( a_lErr_Stat == NONE_SUBDIVIDE ) continue;
		}
		e_sPrnInfo.Set_Err_Msg ( a_cpErrMsgBuf );
		e_sPrnInfo.Prn_SiNDY_Err ( 0, NULL, NULL );

		// ��ԋ߂��Ƃ��낾���������I��
		if( eExe_Flag & ON_NEAREST_CHK ) {
			break;
		}
		//-----------------------------------------------------------------------
	}

}

/** ---------------------------------------------------------------------------
//	�`�F�b�N���s��
//	�������ŃG���[�t�@�C���̏��������s��
//	@return	����	true
//	@return ���s	false
-----------------------------------------------------------------------------*/
bool	main_rootin::
check_exe	( void )
{
	namespace anno_code = sindy::schema::annotation::annotation_code;

	//HRESULT				hr;

	// �`�F�b�N�̏�����
	if( !check_init() ) {
		return	( false );
	}

	// �G���[���O�p�̐ݒ�
	e_sPrnInfo.Set_Layer_Name ( (char*)m_sAny_Note_Layer.c_str() );

	//--------------------------------------------------------------------------------------

	// �L���b�V����ɂƂ��Ă���
	// �I���������e�X�g 2002.6.24
	ISpatialCacheManagerPtr		a_ipSpCacheMgr_Map	( m_ipWSpace_Map );
	ISpatialCacheManagerPtr		a_ipSpCacheMgr_Base	( m_ipWSpace_Base );

	//	1101101	�V�����H�����i�n��j
	//	1101102	�i�q�H�����i�n��j
	//	1101103	��R�Z�N�^�[�H�����i�n��j
	//	1101104	���S�H�����i�n��j
	//	1101106	�n���S�H�����i�n��j
	//	1101107	���m���[���H�����i�n��j
	//	1101108	�V��ʘH�����i�n��j
	//	1101201	�V�����H�����i�n���j
	//	1101202	�i�q�H�����i�n���j
	//	1101203	��R�Z�N�^�[�H�����i�n���j
	//	1101204	���S�H�����i�n���j
	//	1101206	�n���S�H�����i�n���j
	//	1101207	���m���[���H�����i�n���j
	//	1101208	�V��ʘH�����i�n���j
	// ���b�V����`�Œ��L�f�[�^���T�[�`����
	CString a_sNote_Where;	// �����Œ�l
//	if( m_iDigit == 8 ) {
	a_sNote_Where.Format(_T("((%s >= %ld AND %s <= %ld) OR (%s >= %ld AND %s <= %ld)) AND %s <> %ld AND %s <> %ld"),
		annotation::kAnnoCode, anno_code::kShinkansenLineName,
		annotation::kAnnoCode, anno_code::kShinkotsuLineName,
		annotation::kAnnoCode, anno_code::kShinkansenSubLineName,
		annotation::kAnnoCode, anno_code::kShinkotsuSubLineName,
		annotation::kAnnoCode, anno_code::kStreetCarLineName,
		annotation::kAnnoCode, 1101205); // �R�[�h�l1101205�̒��L��ʂ͑��݂��Ȃ� 2014/03/20����

	char	a_cpErrMsgBuf[512];

	int		a_iCur_Mesh	= 0;
	// ���b�V���P�ʂ̏���
	for( a_iCur_Mesh = m_mChk_List.get_mesh_first(); a_iCur_Mesh != -1; a_iCur_Mesh = m_mChk_List.get_mesh_next() )
	{
		e_sPrnInfo.Set_Mesh		( a_iCur_Mesh );
		e_sPrnInfo.Set_Err_Level( smp_sindy_err_info::EC_ERROR );

		// ���b�V�����ƂɃL���b�V���}�l�[�W�����N���A���Ă���
	//	hr	= a_ipSpCacheMgr_Map->EmptyCache( );
	//	if( FAILED ( hr ) ) {
	//		a_sPrnInfo.Prn_Light_Msg ( 2, "�s�s�n�}�n�f�[�^�p�L���b�V���}�l�[�W�����N���A�ł��܂���ł���", stderr );
	//		continue;
	//	}

		sprintf	( a_cpErrMsgBuf, "���b�V���R�[�h [%d] �`�F�b�N��\n", a_iCur_Mesh );
		e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stdout );

		// ���b�V����`�̎��o��
		IGeometryPtr	a_ipMeshRect;
		a_ipMeshRect	= get_mesh_rect( a_iCur_Mesh );
		if( a_ipMeshRect == NULL ) {	// ���b�V����`�������ꍇ
			sprintf	( a_cpErrMsgBuf, "���b�V���R�[�h<%d>�̋�`��������܂���ł���\n", a_iCur_Mesh );
			e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
			continue;
		}
		
		IEnvelopePtr	a_ipRect;
		a_ipMeshRect->get_Envelope ( &a_ipRect );

		// ��ԃL���b�V���}�l�[�W���Ƀ��b�V�����̃f�[�^���Ƃ肾��
	//	a_ipSpCacheMgr_Map->FillCache( a_ipRect );
	//	a_ipSpCacheMgr_Base->FillCache( a_ipRect );

		IFeatureCursorPtr	a_ipAny_Note_Cursor;
		a_ipAny_Note_Cursor	= m_mAny_Note_WS.Search_Feature( a_ipMeshRect, esriSpatialRelIntersects, (LPCTSTR)a_sNote_Where, RET_COUNT|RET_FCURSOR );
		if ( a_ipAny_Note_Cursor == NULL ) {
			sprintf	( a_cpErrMsgBuf, "<%d>���b�V����`�Ńf�[�^�����܂���\n", a_iCur_Mesh );
			e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
			continue;
		}

		//-----------<�`�F�b�N�i�s�x�\��>--------------------
		long		i			= 0;
		long		a_lTCount	= 0;
		a_lTCount	= m_mAny_Note_WS.Get_Data_Count ();
		sprintf	( a_cpErrMsgBuf, "���b�V���R�[�h [%d] ���f�[�^���� %d ��\n", a_iCur_Mesh, a_lTCount );
		e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stdout );
		//--------------------------------------------------

		ms_data_info	a_mAnnotation_Data;
		long			a_lCount	= 0;

		// �f�[�^���R���e�i�ɂ���
		a_lCount		= Get_Data_info ( a_ipAny_Note_Cursor, a_mAnnotation_Data );

		//--------------------------------------------------------------------
		e_sPrnInfo.Set_Mesh			( a_iCur_Mesh );
		e_sPrnInfo.Set_Err_Level	( smp_sindy_err_info::EC_ERROR );
		e_sPrnInfo.Set_Layer_Name	( (char*)m_sAny_Note_Layer.c_str() );
		//--------------------------------------------------------------------

		ims_data_info	a_iAnnoIndx;	// ���L�^�ʒu�̃C���f�b�N�X
		for( a_iAnnoIndx = a_mAnnotation_Data.begin(); a_iAnnoIndx != a_mAnnotation_Data.end(); a_iAnnoIndx++ )
		{
			data_info		a_dAnnoData	= *a_iAnnoIndx;

			Feature_Attr	a_fStr_1	= a_dAnnoData.m_faaAttr.Get_Attr ( annotation::kNameString1 );
			_bstr_t			a_bsAnno_Str= a_fStr_1.e_ccvAttr.bstrVal;
			Feature_Attr	a_fAttr_Code= a_dAnnoData.m_faaAttr.Get_Attr ( annotation::kAnnoCode );
			long			a_lAttr_Code= a_fAttr_Code.e_ccvAttr.intVal;

			// ���O�vOID�̃Z�b�g
			e_sPrnInfo.Set_OID		( a_dAnnoData.m_iOID );

			// �����Ő^�ʒu����f�o�C�X���W���v�Z���A�ܓx�o�x�ɖ߂��|�C���g���쐬���ă`�F�b�N��������
		//	IGeometryPtr	a_ipPtGeo;
		//	if( FAILED (a_ipAny_Note_Feat->get_Shape ( &a_ipPtGeo )) ) {
		//		e_sPrnInfo.Prn_Info_Std ( "�V�F�C�v�̎擾�Ɏ��s\n", stderr );
		//		continue;
		//	}
			
			// ���L�^�ʒu����A�\�����C�����܂߂��`�F�b�N�p�̐擪�|�C���g�����o��
			vector<disp_pt_info>	a_vChk_Pts;
			if( !Get_Disp_Pts	( a_iCur_Mesh, a_dAnnoData.m_faaAttr, m_mAny_Note_Line_WS, a_vChk_Pts ) ) {
				e_sPrnInfo.Prn_Info_Std ( "�\�����C���̏��擾�Ɏ��s\n", stderr );
				continue;
			}
			
			vector<disp_pt_info>::iterator	a_viIndx;
			for( a_viIndx = a_vChk_Pts.begin(); a_viIndx != a_vChk_Pts.end(); a_viIndx++ )
			{
				disp_pt_info	a_dCurDat	= *a_viIndx;

				// �f�o�C�X���W����ܓx�o�x�ɂ���
				// �T�[�`�p�̃|�C���g���쐬
				IPointPtr	a_ipPoint;
				a_ipPoint	= a_dCurDat.e_ipPt;	// �|�C���g���o��
			//	a_ipPoint	= 	get_Device_Pt	( a_iCur_Mesh, (IPointPtr)a_ipPtGeo, 4 );	// �X�P�[���S�Ń`�F�b�N
		
				e_sPrnInfo.Set_OID		( a_dCurDat.e_iOid );
				if( a_dCurDat.e_iStat == 1 ) {
					e_sPrnInfo.Set_Layer_Name	( (char*)m_sAny_Note_Layer.c_str() );
				}else {
					e_sPrnInfo.Set_Layer_Name	( (char*)m_sAny_Note_Line_Layer[a_dCurDat.e_iScale-1].c_str() );
				}

				//-------------------------------------------------------------------
				//�@���H�֌W�̃`�F�b�N
				//-------------------------------------------------------------------
				// �����Ő؂蕪����
				IFeatureCursorPtr			a_ipRailway_Intersect_Cursor;

				//----------------------------------------------------------------------------------
				// �����ŃW�I���g���o�b�t�@���쐬����
				//----------------------------------------------------------------------------------
				double		a_dDegree	= 0.0;
				a_dDegree	= Get_Degree_Range_Dev ( a_iCur_Mesh, a_dCurDat.e_iScale, (int)Get_Dev_Dist() );

				IGeometryPtr			a_ipBold_Search_Geo;
			//	ITopologicalOperatorPtr	a_ipBold_Line ( a_ipPoint );	// 2006.06.08
			//	if ( FAILED ( a_ipBold_Line->Buffer ( a_dDegree , &a_ipBold_Search_Geo ) ) ) {
			//		e_sPrnInfo.Set_Err_Msg("�W�I���g���o�b�t�@���쐬�ł��Ȃ�");
			//		e_sPrnInfo.Prn_SiNDY_Err( 2, NULL, stderr );
			//		continue;
			//	}

				// �^���W�b�N
				double	a_dDeg_X = 0.0, a_dDeg_Y = 0.0;

				Get_Degree_Range_Dev ( a_iCur_Mesh, a_dCurDat.e_iScale, (int)Get_Dev_Dist(), &a_dDeg_X, &a_dDeg_Y );

				IEnvelopePtr	a_ipTmpRect;
				a_ipTmpRect		= make_rect	(a_ipPoint,	a_dDeg_X, a_dDeg_Y );
				a_ipBold_Search_Geo	= a_ipTmpRect;

				//----------------------------------------------------------------------------------
				int	a_iInterSect_Count	= 0;
				a_ipRailway_Intersect_Cursor	= m_mAny_Railway_WS.Search_Feature ( a_ipBold_Search_Geo, esriSpatialRelIntersects, "", RET_COUNT|RET_FCURSOR );
				a_iInterSect_Count				= m_mAny_Railway_WS.Get_Data_Count();						

				char	a_cpTmpStr[512];
				if( a_ipRailway_Intersect_Cursor == NULL ) {
					e_sPrnInfo.Set_Err_Msg ( "�S���H���f�[�^�̌����Ɏ��s���Ă��܂�" );
					e_sPrnInfo.Prn_SiNDY_Err ( 0, NULL, NULL );
				}else {
					if( a_iInterSect_Count == 0 ) {
					//	sprintf( a_cpTmpStr, "%s\t�X�P�[��\t%d\t�Y������S���H����������܂���", (char*)a_sStat.c_str(), a_dCurDat.e_iScale );
						Feature_Attr	a_fAnno_Str  = a_dAnnoData.m_faaAttr.Get_Attr ( "NAME_STRING1" );
						string			a_sAnno_Str  = Str_to_SJIS(	(CComBSTR*)&a_fAnno_Str.e_ccvAttr.bstrVal );
						string			a_sStat	= "�^�ʒu";
						if( a_dCurDat.e_iStat == 0 ) a_sStat = "�\���ʒu";

						string	a_sAnnoAttr;
						map<long, string, less<long> >::iterator	a_iNameIndx;
						a_iNameIndx	= m_mAttrStrTbl.find ( a_lAttr_Code );		// ���L������
						if( a_iNameIndx !=  m_mAttrStrTbl.end() ) {
							a_sAnnoAttr	= (*a_iNameIndx).second;
						}

						sprintf	( a_cpTmpStr, "�Y������S���H����������܂���\t%s\tS%d\t%d\t%s\t%d\t%s\t\t\t\t%0.10f",
							a_sStat.c_str(), a_dCurDat.e_iScale, a_dAnnoData.m_iOID, a_sAnno_Str.c_str(), a_lAttr_Code, a_sAnnoAttr.c_str(), 0.0 ); 

						e_sPrnInfo.Set_Err_Msg ( a_cpTmpStr );							
						e_sPrnInfo.Prn_SiNDY_Err ( 0, NULL, NULL );
					}else {
						ms_data_info	a_mRailway_Data;	// �f�[�^
						long			a_lRail_Count	= 0;// �R���e�i
						// �f�[�^���R���e�i�ɂ���
						a_lRail_Count	= Get_Data_info ( a_ipRailway_Intersect_Cursor, a_mRailway_Data );
						// ���L�ƓS���̑��������ă`�F�b�N
						Anno_Railway_Chk ( a_dAnnoData, a_dCurDat, a_mRailway_Data );
					}		
				}
			}
			//-------------------------------------------------------------------
			if( i > 0 && (i % 100) == 0) {	// 
				// �J�E���g100�����ƂɃ��b�Z�[�W��\��
				fprintf( stdout, "���b�V���R�[�h [%d]  %d �����@%d �� �`�F�b�N�I�� \n", a_iCur_Mesh, a_lTCount, i );
			}
			//-------------------------------------------------------------------
		}
		fprintf	( stdout, "���b�V���R�[�h [%d] �`�F�b�N�I��\n", a_iCur_Mesh );
	}

	return	( true );
}
