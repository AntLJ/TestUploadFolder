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

#include "StdAfx.h"
#include "main_rootin.h"

// 2009.10.23 �ǉ�
#include <atlstr.h>
#include <sindy/workspace.h>
#include "..\..\LIB\WinLib\VersionInfo.h"

//	2003.07.04	main_rootin.h�����`�𕪗�

/**
* @brief ���b�Z�[�W�̏o��
*/
void PrnMsg	( FILE* c_cpFp, FILE* c_cpFp2, const char* c_cpFormat, ... )
{						
	va_list	a_vArgList = NULL;

	va_start(a_vArgList, c_cpFormat );	// ������

	vfprintf_s ( c_cpFp, c_cpFormat, a_vArgList );
	if( c_cpFp2 != NULL && c_cpFp != c_cpFp2) {
		vfprintf_s ( c_cpFp2, c_cpFormat, a_vArgList );
	}
	fflush	( c_cpFp );
}

/** ------------------------------------------------------------------------------
	SiNDYDB�ւ̐ڑ��I�[�v��
	@return	true	����
	@return	false	���s
--------------------------------------------------------------------------------*/
bool	main_rootin::
SiNDY_DB_Open ( IWorkspacePtr&	c_ipWorkspace_Src,
				IWorkspacePtr&	c_ipWorkspace_Tgt )
{
	CString csWorkStrSrc;
	csWorkStrSrc.Format(_T("%s/%s/%s/%s/%s"), e_pTool_Init.get_UserName(), e_pTool_Init.get_Password(),
					  e_pTool_Init.get_Version(), e_pTool_Init.get_Instance(), e_pTool_Init.get_Server() );

	PrnMsg(e_fErr_Fp, stdout, "#�ڑ��v���p�e�B(src)�F%s\n", csWorkStrSrc );

	c_ipWorkspace_Src = sindy::create_workspace(csWorkStrSrc);
	if(!c_ipWorkspace_Src ){
		PrnMsg(e_fErr_Fp, stderr, "�ڑ��o���Ȃ�����\n" );
		return false;
	}else {
		PrnMsg(e_fErr_Fp, stdout, "#�ڑ�����\n" );
	}

	CString csWorkStrTgt;
	csWorkStrTgt.Format(_T("%s/%s/%s/%s/%s"), e_pTool_Init_Tgt.get_UserName(), e_pTool_Init_Tgt.get_Password(),
					  e_pTool_Init_Tgt.get_Version(), e_pTool_Init_Tgt.get_Instance(), e_pTool_Init_Tgt.get_Server() );

	PrnMsg(e_fErr_Fp, stdout, "#�ڑ��v���p�e�B(tgt)�F%s\n", csWorkStrTgt );

	c_ipWorkspace_Tgt = sindy::create_workspace(csWorkStrTgt);
	if(!c_ipWorkspace_Tgt ){
		PrnMsg(e_fErr_Fp, stderr, "�ڑ��o���Ȃ�����\n" );
		return false;
	}else {
		PrnMsg(e_fErr_Fp, stdout, "#�ڑ�����\n" );
	}
	return	( true );
}

/** ------------------------------------------------------------------------------
	�G���[���O�t�@�C�����I�[�v������
	@return	true	�I�[�v������
	@return false	�I�[�v�����s
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Err_Log	( void )
{
	if( e_cpLog_File == NULL ) {
		return	( true );
	}else {
		e_fErr_Fp	= fopen ( e_cpLog_File, "w" );
		if( e_fErr_Fp == NULL ) {
			PrnMsg(stderr, NULL, "�G���[���O�t�@�C��<%s>���I�[�v���ł��Ȃ�\n", e_cpLog_File );
			return	( false );
		}
	}
	return	( true );
}

/** ------------------------------------------------------------------------------
	�o�͑Ή��e�[�u���t�@�C�����I�[�v������
	@return	true	�I�[�v������
	@return false	�I�[�v�����s
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Match_Tbl	( void )
{
	if( e_cpMatch_Tbl_File == NULL ) {
		return	( true );
	}else {
		e_fpMatch_Fp	= fopen ( e_cpMatch_Tbl_File, "w" );
		if( e_fpMatch_Fp == NULL ) {
			PrnMsg(e_fErr_Fp, stderr, "�o�͑Ή��e�[�u���t�@�C��<%s>���I�[�v���ł��Ȃ�\n", e_cpMatch_Tbl_File );
			return	( false );
		}
	}
	return	( true );
}


#define	OPTION_MAX	7
#define	BMAX		512

/** ------------------------------------------------------------------------------
// �����`�F�b�N
--------------------------------------------------------------------------------*/
bool	main_rootin::
arg_check	(	int		argc,		// ������
				char	**argv	)	// ������
{
	int		a_optcount[OPTION_MAX];	// �I�v�V�����̏d���̃J�E���g
	int		i;
	int		a_exeflag	= 0;

	//-----------------------------------------------------------------------
	// �c�[���o�[�W�����\��
	CVersion	a_cvTool_Var;
	CString		a_csVar;
	a_csVar		= a_cvTool_Var.GetFileVersion();
	m_sVersion	= (char*)(_bstr_t(a_csVar));
	//-----------------------------------------------------------------------

	for(i = 0; i < OPTION_MAX; i++)
	{
		a_optcount[i] = 0;
	}

	// ���������񐔂̃`�F�b�N
	for(i = 0; i < argc; i++)
	{
		if(strlen(argv[i]) > BMAX) {
			PrnMsg(e_fErr_Fp, stderr, "���������񂪒������܂�!!\n" );
			return	( false );
		}
		// �o�[�W�������\���`�F�b�N
		if(strcmp(argv[i], "-v") == 0) {
			print_version();
			return	( false );
		}
	}


	if(argc < 5) {
		return( false );
	}

	e_cpInit_File		= argv[1];	///< �������t�@�C��(�Z��DB�ڑ�)���܂�
	e_cpCheck_Src_Layer	= argv[2];	///< �`�F�b�N�����C��
	e_cpInit_File_Tgt	= argv[3];	///< �������t�@�C��(�Z��DB�ڑ�)���܂�
	e_cpCheck_Tgt_Layer	= argv[4];	///< �`�F�b�N�惌�C��

	// �I�v�V����������A���̂Ƃ���g�p���Ȃ�
	for(i = 5; i < argc;)
	{
		if(strcmp(argv[i], "-l") == 0) { // ���O�t�@�C���ݒ�i�ݒ肪�����ꍇ�́A�G���[��stderr�֐��ꗬ���ɂȂ�j
			if(argc < i+2) return( false );
			e_cpLog_File	= argv[i+1];	///< ���b�V���R�[�h���X�g�t�@�C��
			a_optcount[0]++;
			i+=2;
		} else if(strcmp(argv[i], "-m") == 0) {
			if(argc < i+2) return( false );
			e_cpMeshCode_File	= argv[i+1];	///< ���b�V���R�[�h���X�g�t�@�C��
			a_exeflag			|= ON_MESH;
			a_optcount[1]++;
			i+=2;
		} else if(strcmp(argv[i], "-M") == 0) {
			if(argc < i+2) return( false );
			a_exeflag			|= ON_MONO_MESH;
			e_iMeshCode			= atoi(argv[i+1]);
			a_optcount[2]++;
			i+=2;
		} else if(strcmp(argv[i], "-t") == 0) {
			if(argc < i+2) return( false );
			e_cpMatch_Tbl_File	= argv[i+1];		///< �Ή����X�g�t�@�C��
			a_exeflag			|= ON_MATCH_FILE;
			a_optcount[4]++;
			i+=2;
		}else if(strcmp(argv[i], "-lowacc") == 0) {		///< �ᐸ�x��v���[�h 2006.06.12
			a_exeflag			|= ON_LOWACC_MODE;
			a_optcount[5]++;
			i+=1;
		}else if(strcmp(argv[i], "-v") == 0) {
			print_version();
			a_optcount[6]++;
			i+=1;
			return	( false );	// ���ɂ�������H
		} else {
			PrnMsg(e_fErr_Fp, stderr, "�I�v�V���� %s �͊Ԉ���Ă܂�!!\n", argv[i]);
			return	( false );
		}
	}

	// �I�v�V�����d���̃`�F�b�N
	for(i = 0; i < OPTION_MAX; i++)
	{
		if(a_optcount[i] > 1) {
			PrnMsg(e_fErr_Fp, stderr, "�I�v�V�����̎w�肪�d�����Ă��܂�!!\n");
			return	( false );
		}
	}

	// �������t�@�C���ݒ�
	if( e_pTool_Init.read_init_file ( e_cpInit_File ) == -1 ) {
		return	( false );	// ���b�Z�[�W�͓����ŏo���Ă���
	}
	if( e_pTool_Init_Tgt.read_init_file ( e_cpInit_File_Tgt) != 0 ) {
		return ( false );
	}

	// �o�͑Ή��e�[�u���t�@�C���I�[�v��
	if( a_exeflag & ON_MATCH_FILE ) {
		Open_Match_Tbl ();
	}

	// �G���[���O�t�@�C���̃I�[�v��
	if( !Open_Err_Log () ) {
		return	( false );
	}else {
		if( !e_sPrnInfo.Init(e_fErr_Fp) ) {
			return	( false );
		}
	}
	eExe_Flag	= a_exeflag;

	//--------------------------------------------------------------------------
	// ���s�����o��
	PrnMsg( e_fErr_Fp, stdout, "#�c�[���o�[�W����                : <%s>\n", m_sVersion.c_str() );
	if( eExe_Flag & ON_MESH ) {
	PrnMsg( e_fErr_Fp, stdout,  "#�`�F�b�N�Ώۃ��b�V�����X�g      : <%s>\n", e_cpMeshCode_File );
	}else {
	PrnMsg( e_fErr_Fp, stdout,  "#�`�F�b�N�Ώۃ��b�V���R�[�h      : <%02d>\n", e_iMeshCode );
	}
	PrnMsg( e_fErr_Fp, stdout,  "#��r���p�ڑ��������t�@�C��      : <%s>\n", e_cpInit_File );

	PrnMsg( e_fErr_Fp, stdout,  "#��r��DB�ڑ����                : <%s/%s/%s/%s/%s>\n",
		e_pTool_Init.get_UserName(), e_pTool_Init.get_Password(), e_pTool_Init.get_Version(), e_pTool_Init.get_Instance(), e_pTool_Init.get_Server() );
	PrnMsg( e_fErr_Fp, stdout,  "#��r�����C��                    : <%s>\n", e_cpCheck_Src_Layer );
	PrnMsg( e_fErr_Fp, stdout,  "#��r��p�ڑ��������t�@�C��      : <%s>\n", e_cpInit_File_Tgt );
	PrnMsg( e_fErr_Fp, stdout,  "#��r��DB�ڑ����                : <%s/%s/%s/%s/%s>\n",
		e_pTool_Init_Tgt.get_UserName(), e_pTool_Init_Tgt.get_Password(), e_pTool_Init_Tgt.get_Version(), e_pTool_Init_Tgt.get_Instance(), e_pTool_Init_Tgt.get_Server() );
	PrnMsg( e_fErr_Fp, stdout,  "#��r�惌�C��                    : <%s>\n", e_cpCheck_Tgt_Layer );
	PrnMsg( e_fErr_Fp, stdout,  "#�o�̓t�@�C��                    : <%s>\n", e_cpMatch_Tbl_File );
	// ���[�h�\��
	if( eExe_Flag & ON_LOWACC_MODE ) {
		PrnMsg( e_fErr_Fp, stdout, "#�ᐸ�x��v���[�h\n" );
	}else {
		PrnMsg( e_fErr_Fp, stdout, "#�����x��v���[�h\n" );
	}
	//--------------------------------------------------------------------------


	return		( true );

}

/** ------------------------------------------------------------------------------
// �g�p�@�\��
--------------------------------------------------------------------------------*/
void		main_rootin::
print_usage	(char **argv)
{
	PrnMsg(stdout, NULL,
			"=== �y�����������o�c�[���z(%s) ===\n"
				"<usage>\n"
				"%s <arg1> <arg2> <arg3> <arg4>\n"
				"\t<����>\n"
				"\t      <arg1>         : �`�F�b�N���p�������t�@�C��\n"
				"\t      <arg2>         : �`�F�b�N�����C����	(ex. TECHMAP.Building_mc)\n"
				"\t      <arg3>         : �`�F�b�N���p�������t�@�C��\n"
				"\t      <arg4>         : �`�F�b�N�惌�C����	(ex. SiNDY2004.Building)\n"
				"\t<�I�v�V����>\n"
				"\t      '-l'           : ���O�t�@�C�������w��\n"
				"\t      '-t'           : �Ή����X�g�t�@�C�����w��\n"
				"\t      '-m'           : ���b�V���R�[�h���X�g�t�@�C�����w��\n"
				"\t      '-M'           : ���b�V���R�[�h���w��i1���A2���A8�����b�V���j\n"
				"\t      '-lowacc'      : �ᐸ�x��v���[�h\n"
				"\t      '-v'           : �o�[�W�������\��\n"
				"\t<�⑫>\n"
				"\t      �E'-m','-M',�͂ǂ��炩�K���w�肵�ĉ�����\n"
				"\t      �E'-m','-M'�͓����w��ł��܂���\n"
			, argv[0], m_sVersion.c_str());
}

/** ------------------------------------------------------------------------------
	�o�[�W�������̕\��
	@return		����
---------------------------------------------------------------------------------*/
void	main_rootin::
print_version	( void )
{
	PrnMsg(stdout, NULL,
			"=== �y�����������o�c�[���z(%s) ===\n"
				"\t<�o�[�W�������>\n"
				"\t2004/05/14        1.00               : ���J�o�[�W����\n"
				"\t2005/12/21        1.10               : �ᐸ�x���[�h�g�ݍ��݁B\n"
				"\t2006/06/12        1.20               : �ᐸ�x���[�h���I�v�V����'-lowacc'�Ő؂�ւ��\�ɏC���B\n"
				"\t2006/09/28        1.30               : �ᐸ�x��v���[�h�̈�v������������\n"
				"\t2008/05/27        2.0.0.1            : ���b�V���t�B�[���h�������C���������̂őΉ�\n"
				"\t                                     : VC8�Ή�\n"
				"\t2009/10/21        2.1.0.2            : TMP_PRIMID�����Ȃ��悤�ɏC��\n"
				"\t                                     : ���O�Ɏ��s���̏���ǉ�\n"
				"\t2014/02/10        2.2.0.4            : [but 10158][u][�v�]]14�H�d�l�ɑΉ����ė~����(Building_Diff) �ɑΉ�\n"
				"\t                                     : DB�ڑ���sindy/workspace.h���g�p\n"
				, m_sVersion.c_str()
//				"\t2004/03/25        1.00               : ���^�p�o�[�W����\n"
			);
}

/** ------------------------------------------------------------------------------
	���b�V����`��DB����l�����郁�\�b�h�̏�����
	��get_mesh_rect �����O�ɕK���Ă�
------------------------------------------------------------------------------==*/
bool	main_rootin::
get_mesh_init	(	mk_input_list&	c_mMList,			// ���̓��X�g�N���X
					IWorkspacePtr&	c_ipWorkSpace	)	// �w�肷�郏�[�N�X�y�[�X
{

	if( eExe_Flag & ON_MESH ) {
		if( e_cpMeshCode_File == NULL ) {
			PrnMsg(e_fErr_Fp, stderr, "���b�V���R�[�h���X�g�t�@�C�������Ƃ肾���܂���ł���[check_exe]\n");
			return	( false );
		}	
		if( c_mMList.read_mesh_list ( e_cpMeshCode_File ) != 0 ) {	// �����Ń��X�g��ǂݍ���
			PrnMsg(e_fErr_Fp, stderr, "���b�V���R�[�h���X�g�t�@�C��<%s>�̓ǂݍ��݂Ɏ��s[check_exe]\n", e_cpMeshCode_File );
			return	( false );
		}
	}else {
		if(c_mMList.read_mesh_list ( e_iMeshCode ) == -1) {
			PrnMsg(e_fErr_Fp, stderr, "���b�V��<%d>�̐ݒ�Ɏ��s���܂���\n", e_iMeshCode );
			return	( false );
		}
	}

	char*		a_cpMeshLayer	= NULL;
	int			a_iMDigit		= 0;
	a_iMDigit	= c_mMList.get_mesh_digit ();
	
	switch	( a_iMDigit )
	{
	case	8:
		a_cpMeshLayer	= e_pTool_Init.get_City_Mesh ();
		break;
	case	6:
		a_cpMeshLayer	= e_pTool_Init.get_Base_Mesh();
		break;
	case	4:
		a_cpMeshLayer	= e_pTool_Init.get_Middle_Mesh();
		break;
	default:
		PrnMsg(e_fErr_Fp, stderr, "���X�g�̃��b�V�������ɂ͑Ή����Ă��܂���[check_exe]\n");
		return	( false );
	}


	IFeatureWorkspacePtr	a_F_Workspace	( c_ipWorkSpace );	// �t�B�[�`�����[�N�X�y�[�X

	// ���C���̃I�[�v��
	if( FAILED(a_F_Workspace->OpenFeatureClass(CComBSTR(a_cpMeshLayer), &e_ipF_Class )) ) {
		PrnMsg(e_fErr_Fp, stderr, "���C�� <%s> �̃I�[�v���Ɏ��s\n", a_cpMeshLayer);
		return	(false);
	}else {
		PrnMsg(e_fErr_Fp, stderr,	"#<%s> ���C�����I�[�v��\n", a_cpMeshLayer);
	}

	// 2002.12.09 �f�[�^�����镔���̃��b�V�����X�g
	eDigit	= a_iMDigit;

	return	( true );
}

/** ------------------------------------------------------------------------------
	���b�V����`��GeoDB����擾���� \
	��GeoDB�ւ̐ڑ����m������Ă��邱�Ƃ��O��
--------------------------------------------------------------------------------*/
IEnvelopePtr	main_rootin::
get_mesh_rect	( int	c_iMeshCode	)	///< ���b�V���R�[�h
{

	CComBSTR	a_SpatialCol;
	e_ipF_Class->get_ShapeFieldName	( &a_SpatialCol );
	BSTR		a_Where_Str;
	char		a_tmpstr[30];	//�@�����͂Ȃ�Ƃ��������C������

	crd_cnv		crd;
	int			a_iDigit	= 0;
	a_iDigit	= crd.GetMeshDigit( c_iMeshCode );
	sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// ���b�V���R�[�h�𕶎����
	eDigit	= a_iDigit;

	a_Where_Str	= T2BSTR(_T(a_tmpstr));		// string��BSTR�ɕϊ�

	IQueryFilterPtr		a_Sp_Filter	( CLSID_QueryFilter );			// ��Ԍ����t�B���^
//	a_Sp_Filter->put_SearchOrder	( esriSearchOrderAttribute );	// ���������D��
	a_Sp_Filter->put_WhereClause	( a_Where_Str);					// Where��̐ݒ�
	
	// �N���b�v��`�쐬
	IEnvelopePtr	a_ipMesh_Rect;	// ���b�V����`
	a_ipMesh_Rect	= NULL;

	// �T�[�`���Ă���
	IFeatureCursorPtr	a_F_Cursor;
	if( FAILED	(e_ipF_Class->Search	( a_Sp_Filter, VARIANT_FALSE, &a_F_Cursor )) ) {
		PrnMsg(e_fErr_Fp, stderr, "�f�[�^�̑��������Ɏ��s\n");
		return	(NULL);
	}else {
		IFeaturePtr				a_Feature;

		while(a_F_Cursor->NextFeature ( &a_Feature ) == S_OK)
		{		
			IGeometryPtr		a_Clip_Shp;		// �����Ƀf�[�^���R�s�[���ăN���b�v����

			if( FAILED(a_Feature->get_ShapeCopy	( &a_Clip_Shp )) ) {	// �t�B�[�`������
				PrnMsg(e_fErr_Fp, stderr, "�V�F�C�v�̃R�s�[�Ɏ��s\n");
				return  ( NULL );
			}
			
			VARIANT_BOOL	a_bResult;
			a_Clip_Shp->get_IsEmpty( &a_bResult );
			if(a_bResult == VARIANT_TRUE) {	// �N���b�v���ʂ�NULL�Ȃ烁�b�V���O
				continue;
			}
			if( FAILED(a_Clip_Shp->get_Envelope( &a_ipMesh_Rect) )) {
				PrnMsg(e_fErr_Fp, stderr, "�Y�����b�V��<%d>�̋�`���݂���܂���ł���\n", c_iMeshCode);
				return	( NULL );
			}else {
				return	( a_ipMesh_Rect );
			}
		}
	}
	return	( a_ipMesh_Rect );
}

/** ------------------------------------------------------------------------------
	��`�̃|���S����Ԃ�
	@return	IPolygonPtr
--------------------------------------------------------------------------------*/
IPolygonPtr	main_rootin::
Get_Rect_Polygon	(	IEnvelopePtr&	c_ipEnv	)		///< ���b�V����`
{
	//
	// 2002.07.01 Intersect�p�̃W�I���g�����쐬
	IPolygonPtr		a_ipRectPoly(CLSID_Polygon);	// ��`�p�W�I���g��
	IPointCollectionPtr	a_ipRecPts	(a_ipRectPoly);		// ��`�p�_��

	IPointPtr			a_Pt[5];

	c_ipEnv->get_LowerLeft (&a_Pt[0]);
	c_ipEnv->get_UpperLeft (&a_Pt[1]);
	c_ipEnv->get_UpperRight(&a_Pt[2]);
	c_ipEnv->get_LowerRight(&a_Pt[3]);
	c_ipEnv->get_LowerLeft (&a_Pt[4]);

	int	roop = 0;
	for ( roop = 0; roop < 5; roop++ )
	{
		a_ipRecPts->AddPoint(a_Pt[roop]);
	}

	// ���Y����̂Ƃ��납�甲�� 2004.07.15
	double aMinX, aMinY, aMaxX, aMaxY;
	aMinX = aMinY = 1000000.0;
	aMaxX = aMaxY = -1000000.0;
	for( roop = 0; roop < 5; roop++ )
	{
		double	a_dTmpX	= 0.0, a_dTmpY = 0.0;
		a_Pt[roop]->get_X ( &a_dTmpX );
		a_Pt[roop]->get_Y ( &a_dTmpY );

		if(aMinX > a_dTmpX)
			aMinX = a_dTmpX;
		if(aMinY > a_dTmpY)
			aMinY = a_dTmpY;
		if(aMaxX < a_dTmpX)
			aMaxX = a_dTmpX;
		if(aMaxY < a_dTmpY)
			aMaxY = a_dTmpY;
	}

	m_CompMaxX = aMaxX;
	m_CompMaxY = aMaxY;
	m_CompMinX = aMinX;
	m_CompMinY = aMinY;
	//�@���������܂�

	// �V���v���t�@�C��������
	ITopologicalOperator2Ptr	a_ipTopo;

	a_ipTopo	= a_ipRectPoly;

	// �X�i�b�v����@2004.07.14
	ISpatialReferencePtr	a_ipSpRef;
	c_ipEnv->get_SpatialReference( &a_ipSpRef );
	a_ipRectPoly->putref_SpatialReference( a_ipSpRef );
	a_ipRectPoly->SnapToSpatialReference();

	if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
		PrnMsg(e_fErr_Fp, stderr, "Simplify�Ɏ��s\n");
		return	( NULL );
	}
	if(FAILED ( a_ipRectPoly->SimplifyPreserveFromTo() ) ) {
		PrnMsg(e_fErr_Fp, stderr, "SimplifyPreserveFromTo�Ɏ��s" );	// �G���[���b�Z�[�W
		return	( NULL );
	}
	return ( a_ipRectPoly );
}

/** ------------------------------------------------------------------------------
	���C���I�[�v��
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Layers		( 	IWorkspacePtr&	ipWorkspace_Src,	// �`�F�b�N�����[�N�X�y�[�X[IN]
					IWorkspacePtr&	ipWorkspace_Tgt )	// �`�F�b�N�惏�[�N�X�y�[�X[IN]
{

	if( !e_mChk_Src_WS.Init ( ipWorkspace_Src, e_cpCheck_Src_Layer ) ) {
		PrnMsg(e_fErr_Fp, stderr, "�`�F�b�N�����C��<%s>�̓ǂݍ��݂Ɏ��s\n", e_cpCheck_Src_Layer );
		return	( false );
	}
	if( !e_mChk_Tgt_WS.Init ( ipWorkspace_Tgt, e_cpCheck_Tgt_Layer ) ) {
		PrnMsg(e_fErr_Fp, stderr, "�`�F�b�N�惌�C��<%s>�̓ǂݍ��݂Ɏ��s\n", e_cpCheck_Tgt_Layer );
		return	( false );	
	}
	return	( true );
}

/**
	�ǂݍ��񂾃f�[�^���Z�b�g�ɗ��Ƃ�
	@return	�f�[�^��
*/
int		main_rootin::
Set_Data_Infos (	IFeatureCursorPtr&	c_ipChk_Cursor,	///< �`�F�b�N���J�[�\��[IN]
					ms_data_info&		c_ipChk_Data )	///< �`�F�b�N���f�[�^�̃Z�b�g[OUT]
{
	int	a_iCount	= 0;
	// �f�[�^���}���`�Z�b�g�̃f�[�^�ɗ��Ƃ�
	if( c_ipChk_Cursor != NULL ) {
		IFeaturePtr	a_ipNote_Feat;
		// �������ꂽ���L�̃��[�v
		while(c_ipChk_Cursor->NextFeature ( &a_ipNote_Feat ) == S_OK)
		{
			data_info	a_dChk_Data;

			Feature_All_Attrs		a_fChk_Src_Attrs;
			a_fChk_Src_Attrs.Init	( a_ipNote_Feat );
			// �I�u�W�F�N�gID
			Feature_Attr a_fObjID, /*a_fMeshCode, a_fPrim_ID,*/ a_fAttr, a_fCORRIDOR_F, a_fNOWALL_F;
			a_fObjID     = a_fChk_Src_Attrs.Get_Attr ( "OBJECTID" );
			a_fAttr      = a_fChk_Src_Attrs.Get_Attr ( "BLDCLASS_C" );
			a_fCORRIDOR_F= a_fChk_Src_Attrs.Get_Attr ( "CORRIDOR_F" );
			a_fNOWALL_F  = a_fChk_Src_Attrs.Get_Attr ( "NOWALL_F" );

			// ���O��ʂ̔���
			int ret = mAKAttrHandler.fnIsExcluded ( a_fAttr.e_ccvAttr.lVal, a_fNOWALL_F.e_ccvAttr.lVal, a_fCORRIDOR_F.e_ccvAttr.lVal );
			if( ret != 0 ) {
				// 14�H�ȑO�̃o�[�W�����ł��g�p����̂ƁA�����������v�f������̂ŏ��O�f�[�^�̏��͏o�͂��܂���B
				continue;
			}
			// ���O�łȂ��ꍇ�́A�����l�̕ϊ�
			mAKAttrHandler.fnAttrConv ( a_fAttr.e_ccvAttr.lVal );

			a_dChk_Data.e_iOID		= a_fObjID.e_ccvAttr.intVal;
			a_dChk_Data.e_iMeshCode	= e_iMeshCode;
			a_dChk_Data.e_iAttr		= a_fAttr.e_ccvAttr.intVal;
			a_dChk_Data.e_ipGeo		= a_fChk_Src_Attrs.Ret_Geo();

			c_ipChk_Data.insert ( a_dChk_Data );
			a_iCount++;
		}
	}
	return	( a_iCount );
}

/** ------------------------------------------------------------------------------
	�`��̃}�b�`���O�����擾����
	@return	STAT_EQUAL	��v
	@return	STAT_MODIFY	�`��ύX
	@return	STAT_LOWA_EQUAL	�`���v�i�ᐸ�x�j
	@return	STAT_LOWA_MODIFY	�`��ύX�i�ᐸ�x�j
	@return	0	�s��v
--------------------------------------------------------------------------------*/
int		main_rootin::
Get_Match_Info ( data_info&		c_dSrcDat,			///< �`�F�b�N���f�[�^[IN]
				 data_info&		c_dTgtDat,			///< �`�F�b�N��f�[�^[IN]
				 ms_data_info&	c_ipChk_Tgt_Data )	///< �`�F�b�N���f�[�^�̃Z�b�g[IN]
{
	// �`���v�̃`�F�b�N
	int	a_iRetStat = 0;
	a_iRetStat	= Shape_Chk_and_Ret_Stat ( c_dSrcDat, c_dTgtDat );
	c_dSrcDat.e_iStat	= a_iRetStat;
	if( a_iRetStat != 0 ) {
		if( a_iRetStat == STAT_MODIFY ) {	// �u�`��ύX�v�̏ꍇ
			ims_data_info	a_iIndx;
			a_iIndx	= c_ipChk_Tgt_Data.find ( c_dSrcDat );
			if( a_iIndx == c_ipChk_Tgt_Data.end() ) { //�@���b�V���ň����|�������R�[�h�ɑ��݂��Ȃ����̂ƈ�v���Ă���ꍇ
				c_dSrcDat.e_iStat	|=	STAT_MESHOUTER;	// �u���b�V���O�v��ǉ�
			}
		}
	}
	return	( a_iRetStat );
}

/** ------------------------------------------------------------------------------
	�`���v�̃`�F�b�N���s���A��Ԃ�Ԃ�
	@return	STAT_EQUAL	��v
	@return	STAT_MODIFY	�`��ύX
	@return	STAT_LOWA_EQUAL	�`���v�i�ᐸ�x�j
	@return	STAT_LOWA_MODIFY	�`��ύX�i�ᐸ�x�j
	@return	0	�s��v
--------------------------------------------------------------------------------*/
int		main_rootin::
Shape_Chk_and_Ret_Stat ( data_info&		c_dSrcDat,			///< �`�F�b�N���f�[�^[IN]
						 data_info&		c_dTgtDat )			///< �`�F�b�N��f�[�^[IN]
{
	int	a_iRetStat	= 0;
	if( Shape_Chk ( c_dSrcDat.e_ipGeo, c_dTgtDat.e_ipGeo ) ) {
		a_iRetStat	= STAT_EQUAL;	// �`���v
	}else {
		// �`���v�i�ᐸ�x�j,�`��ύX�i�ᐸ�x�j,�s��v�̔���
		a_iRetStat	= Shape_Chk_LowAccuracy ( c_dSrcDat.e_ipGeo, c_dTgtDat.e_ipGeo );
		if( a_iRetStat == 0 ) {	// �s��v�Ɣ��肳�ꂽ�̏ꍇ
			if( c_dSrcDat.e_iOID == c_dTgtDat.e_iOID ) {	// OBJECTID����v����ꍇ�́A�u�`��ύX�v������
				a_iRetStat	= STAT_MODIFY;
			}
		}
	}
	return	( a_iRetStat );
}

/** ------------------------------------------------------------------------------
	�`���v�̃`�F�b�N
	@return	true	��v
	@return	false	�s��v
--------------------------------------------------------------------------------*/
bool	main_rootin::
Shape_Chk ( IGeometryPtr&	c_dSrcGeo,		///< �`�F�b�N���f�[�^[IN]
		    IGeometryPtr&	c_dTgtGeo	)	///< �`�F�b�N��f�[�^[IN]
{
	IRelationalOperatorPtr	a_ipEqual_Chk;

	//---------------------------------------------------------------------------
	// 2006.06.17 ��r�̑O�ɁA�`�󓯎m���֌W���邩�𒲂ׂ�
	VARIANT_BOOL	a_vResult;
	IRelationalOperatorPtr	a_ipSrcRel;
	a_ipSrcRel	= c_dSrcGeo;
	if( FAILED(a_ipSrcRel->Disjoint ( c_dTgtGeo, &a_vResult ) )) {
		e_sPrnInfo.Prn_SiNDY_Err ( 0, "Disjoint�֌W�擾�Ɏ��s", NULL );
	}else {
		if( a_vResult == VARIANT_TRUE ) {
			return ( false );
		}
	}
	// 2006.06.17 ---------------------------------------------------------------

	// �e�X�g�p------------------------------------
	IPointCollectionPtr	a_ipPts_Src, a_ipPts_Tgt;
	a_ipPts_Src	= c_dSrcGeo;
	a_ipPts_Tgt	= c_dTgtGeo;

	long	a_lSrc_Count = 0, a_lTgt_Count = 0;
	a_ipPts_Src->get_PointCount ( &a_lSrc_Count );
	a_ipPts_Tgt->get_PointCount ( &a_lTgt_Count );

	// ��ԓ_���̍���
	if( a_lSrc_Count != a_lTgt_Count ) {
		return ( false );
	}

	IGeometryCollectionPtr	a_ipSrcGeoCol, a_ipTgtGeoCol;
	a_ipSrcGeoCol	= c_dSrcGeo;
	a_ipTgtGeoCol	= c_dTgtGeo;
	
	// �����炭�قƂ�ǈӖ��͂Ȃ��ł��낤���O�̂��߃`�F�b�N���A���o���ꂽ�ꍇ�̓G���[���O�ɏo�͂��Ă���
	long	a_lSrc_Col_Count = 0, a_lTgt_Col_Count = 0;
	if( a_lSrc_Col_Count != a_lTgt_Col_Count ) {
		return ( false );
	}

	WKSPoint*	a_wpSrcWKSPoints = NULL;
	WKSPoint*	a_wpTgtWKSPoints = NULL;
	a_wpSrcWKSPoints	= new WKSPoint[a_lSrc_Count];
	a_wpTgtWKSPoints	= new WKSPoint[a_lTgt_Count];

	a_ipPts_Src->QueryWKSPoints ( 0, a_lSrc_Count, a_wpSrcWKSPoints );
	a_ipPts_Tgt->QueryWKSPoints ( 0, a_lTgt_Count, a_wpTgtWKSPoints );
	
	bool	a_bRet = false;
	if( memcmp ( a_wpSrcWKSPoints, a_wpTgtWKSPoints, sizeof(WKSPoint) * a_lSrc_Count ) == 0 ) {
		a_bRet	= true;
	}

	delete [] a_wpSrcWKSPoints;
	delete [] a_wpTgtWKSPoints;

	return	( a_bRet );
	//--------------------------------------------
}

/** ------------------------------------------------------------------------------
	�W�I���g������_��̏����擾����
	@return		����
--------------------------------------------------------------------------------*/
void	main_rootin::
Get_PtsWithOrientation	( IGeometryPtr&	c_ipGeo,		///< �W�I���g��[IN]
						  set<Real_Pt2>& c_srPts	)	///< �_��[OUT]
{
	IGeometryCollectionPtr	a_ipGeoCol;

	a_ipGeoCol	= c_ipGeo;

	long	a_lGeoCount = 0;

	a_ipGeoCol->get_GeometryCount ( &a_lGeoCount );

	int	a_iGRoop = 0;
	for( a_iGRoop = 0; a_iGRoop < a_lGeoCount; a_iGRoop++ )
	{
		IGeometryPtr	a_iTmpGeo;
		a_ipGeoCol->get_Geometry (a_iGRoop, &a_iTmpGeo);

		IRingPtr		a_iTmpRing;
		a_iTmpRing	= a_iTmpGeo;
		
		// �����O���C���e���A���̔���
		int	a_iInterior_Flag = 0;
		VARIANT_BOOL	a_vIsExterior;
		a_iTmpRing->get_IsExterior( &a_vIsExterior );
		if( a_vIsExterior == VARIANT_FALSE ) {
			a_iInterior_Flag	= 1;
		}

		IPointCollectionPtr	a_ipTmpPts;
		a_ipTmpPts	= a_iTmpGeo;

		// ����Geometry�̕�ԓ_��
		long	a_lSrc_Count = 0;
		a_ipTmpPts->get_PointCount ( &a_lSrc_Count );

		// �����O�̓_����擾
		WKSPoint*	a_wpSrcWKSPoints = NULL;
		a_wpSrcWKSPoints	= new WKSPoint[a_lSrc_Count];

		a_ipTmpPts->QueryWKSPoints ( 0, a_lSrc_Count, a_wpSrcWKSPoints );

		// �_����Z�b�g�Ɋi�[����
		set<Real_Pt2>	a_sPts;
		int i = 0;
		for( i = 0; i < a_lSrc_Count; i++ ) {
			Real_Pt2	a_iPt;
			a_iPt		= a_wpSrcWKSPoints[i];
			a_iPt.e_iInteriorFlag	= a_iInterior_Flag;
			c_srPts.insert ( a_iPt );
		}
		delete [] a_wpSrcWKSPoints;
	}
}

/** ------------------------------------------------------------------------------
	�`���v�̃`�F�b�N�i�ᐸ�x�Łj 2006.09.25
	���I�u�W�F�N�g�̏d�����̖ʐς��Z�o���A�d���ʐς��`�F�b�N���f�[�^��1/10�ȉ��Ȃ�
	�@�s��v�Ƃ��Ĕ��肷��悤�ɂ����B
	�@�����������́A���̃|�C���g���O���������̂��̂��̏����݂Ĕ��肷��悤�ɂ����B
	@return	STAT_LOWA_EQUAL		�`���v�i�ᐸ�x�j
	@return	STAT_LOWA_MODIFY	�`��ύX�i�ᐸ�x�j
	@return	0	�s��v
--------------------------------------------------------------------------------*/
int		main_rootin::
Shape_Chk_LowAccuracy ( IGeometryPtr&	c_ipSrcGeo,		///< �`�F�b�N���f�[�^[IN]
						IGeometryPtr&	c_ipTgtGeo	)	///< �`�F�b�N��f�[�^[IN]
{
	// �I�u�W�F�N�g�̏d�����̖ʐς��Z�o���A�d���ʐς��`�F�b�N���f�[�^��1/10�ȉ��Ȃ�s��v�Ƃ��Ĕ���
	//--------------------------------------------------------------------------------------------
	ITopologicalOperatorPtr	a_ipTmpTopo(c_ipSrcGeo);
	IGeometryPtr	a_ipDupliPart;
	
	if( FAILED ( a_ipTmpTopo->Intersect ( c_ipTgtGeo, esriGeometryNoDimension, &a_ipDupliPart) )) {
		return ( 0 );
	}else {
		IAreaPtr			a_ipSrcPolyArea, a_ipDupliPolyArea;
		a_ipSrcPolyArea		= c_ipSrcGeo;
		a_ipDupliPolyArea	= a_ipDupliPart;

		double	a_dSrcArea = 0.0, a_dDupliArea = 0.0;

		if( FAILED ( a_ipSrcPolyArea->get_Area ( &a_dSrcArea ) )) {
			return ( 0 );
		}
		if( FAILED ( a_ipDupliPolyArea->get_Area ( &a_dDupliArea ) )) {
			return ( 0 );
		}

		if( a_dDupliArea < (a_dSrcArea / 10.0) ) {
			return ( 0 );
		}
	}	
	//--------------------------------------------------------------------------------------------

	// 臒l��`�i���K��XY��11pt�����i�����̂��߂�����ƍL�߂Ɂj�j
	const double a_cdTH_X = 0.000000171875;
	const double a_cdTH_Y = 0.000000114583336;

	// �_����Z�b�g�Ɋi�[����
	set<Real_Pt2>	a_sSrcPts, a_sTgtPts;

	// �����O�̉������t���œ_����擾
	Get_PtsWithOrientation ( c_ipSrcGeo, a_sSrcPts );
	Get_PtsWithOrientation ( c_ipTgtGeo, a_sTgtPts );

	// 80%�ȏ��v�̏ꍇ�͌`��ύX�Ƃ���(�����_�ȉ��͐؂�̂āj
	int	a_iBorderCount = 0;
	a_iBorderCount	= (double)a_sSrcPts.size() * 0.80;

	// ��r���_��Ƃ̃q�b�g���𒲂ׂ�
	int	a_iHit_Count	= 0;
	set<Real_Pt2>::iterator	a_iSrcIndx, a_iLowerIndx, a_iUpperIndx;
	for( a_iSrcIndx = a_sSrcPts.begin(); a_iSrcIndx != a_sSrcPts.end(); a_iSrcIndx++ )
	{
		Real_Pt2	a_rKey	= *a_iSrcIndx;

		// ������Ԃ̍쐬
		Real_Pt2			a_sLowerKey, a_sUpperKey;
		a_sLowerKey	= a_rKey;
		a_sUpperKey	= a_rKey;

		// X�����ɋ߂��I�u�W�F�N�g���Ђ������Ă���̂ŁAX���������ς����L�[�ł悢
		a_sLowerKey.e_iX	-= a_cdTH_X;
		a_sUpperKey.e_iX	+= a_cdTH_X;

		a_iLowerIndx	= a_sTgtPts.lower_bound ( a_sLowerKey );
		a_iUpperIndx	= a_sTgtPts.upper_bound ( a_sUpperKey );

		int	a_iEnd_Flag	= 0;	// ��ƏI���t���O

		set<Real_Pt2>::iterator	a_iChkIndx;
		for( a_iChkIndx = a_iLowerIndx; ; a_iChkIndx++ )
		{
			// a_iChkIndx ��
			if( a_iEnd_Flag == 1 || a_iChkIndx == a_sTgtPts.end() ) {
				break;
			}else if( a_iChkIndx == a_iUpperIndx ) {	// �����[�v�ŏI��
				a_iEnd_Flag	= 1;
			}
			Real_Pt2	a_rCur	= *a_iChkIndx;
			
			// ��r���Ɣ�r��̃f�[�^�̍��W�����l���AX����<=a_cdTH_X Y����<=a_cdTH_Y �Ȃ�OK�Ƃ���
			double		a_dDiff_X = 0.0, a_dDiff_Y = 0.0;
			a_dDiff_X = fabs ( a_rKey.e_iX - a_rCur.e_iX );
			a_dDiff_Y = fabs ( a_rKey.e_iY - a_rCur.e_iY );

			if( (a_dDiff_X <= a_cdTH_X) && (a_dDiff_Y <= a_cdTH_Y) && 
				(a_rKey.e_iInteriorFlag == a_rCur.e_iInteriorFlag) )
			{
				a_iHit_Count++;
				break;
			}
		}
	}

	int	a_iRet	= 0;
	// ��r���_��̃T�C�Y�Ɣ�r��_��̃T�C�Y�ƃq�b�g������v�����ꍇ�͈�v
	if( a_sSrcPts.size() == a_sTgtPts.size() && a_sSrcPts.size() == a_iHit_Count ) {
		a_iRet	= STAT_LOWA_EQUAL;	// �ᐸ�x��v
	}else {
		if( a_iHit_Count >= a_iBorderCount ) {
			a_iRet	= STAT_LOWA_MODIFY;
		}
	}
	return	( a_iRet );
}

#ifdef _DEBUG
void	main_rootin::
Print_Geo	( data_info&	c_dSrc,
			  data_info&	c_dTgt )
{
	IPointCollectionPtr	a_ipSrcPts( c_dSrc.e_ipGeo );
	IPointCollectionPtr	a_ipTgtPts( c_dTgt.e_ipGeo );

	long	a_lSrcCount	= 0, a_lTgtCount = 0;
	a_ipSrcPts->get_PointCount ( &a_lSrcCount );
	a_ipTgtPts->get_PointCount ( &a_lTgtCount );

	PrnMsg(e_fErr_Fp, stdout, "���_���i%d)\n", a_lSrcCount );
	int	i = 0, j = 0;
	for( i = 0; i < a_lSrcCount; i++ )
	{
		double		x=0.0, y=0.0;
		IPointPtr	a_iPt;
		a_ipSrcPts->get_Point ( i, &a_iPt );
		a_iPt->get_X ( &x );
		a_iPt->get_Y ( &y );
		PrnMsg(e_fErr_Fp, stdout, "pt[%d]\t%0.20f\t%0.20f\n", i, x, y );
	}
	PrnMsg(e_fErr_Fp, stdout, "��_���i%d)\n", a_lTgtCount );
	for( j = 0; j < a_lTgtCount; j++ )
	{
		double		x=0.0, y=0.0;
		IPointPtr	a_iPt;
		a_ipTgtPts->get_Point ( j, &a_iPt );
		a_iPt->get_X ( &x );
		a_iPt->get_Y ( &y );
		PrnMsg(e_fErr_Fp, stdout, "pt[%d]\t%0.20f\t%0.20f\n", j, x, y );
	}
}
#endif

/**
	���b�V�����E�ɑ��݂���f�[�^���擾���A�}�[�W�����}�`���쐬����
	���}�[�W��̃W�I���g���ƃ}�[�W�������ōł��Ⴂ���b�V���ԍ���PrimID���\�Ƃ��ĕԂ�
	�����b�V�������̃f�[�^�ɂ��Ă������Ń��b�V�����E�Ƃ̔�������A���b�V�����E�łȂ����
	�@false�̒l��Ԃ����̂Ƃ���
	@return	true	���b�V�����E�ŗאڂ���f�[�^���������ꍇ
	@return	false	�אڃf�[�^�������ꍇ
*/
bool	main_rootin::
Get_Neibour_Obj ( 	data_info&			c_ipChk_Src_Data,	///< �`�F�b�N���f�[�^[IN]
					bool				c_iMFlag,			///< ��x�}�[�W�����f�[�^���ǂ����̃t���O
					msi_handle_ws&		c_mChk_Src_WS,		///< �����̃��[�N�X�y�[�X�n���h��[IN]
					char*				c_cpCheck_Src_Layer,///< �`�F�b�N���C����[IN]
					data_info&			c_ipMerged_Data	)	///< �אڌ`����}�[�W��������[OUT]
{
	HRESULT	hr;
	
	// ������
	c_ipMerged_Data	= c_ipChk_Src_Data;

	// ��x���}�[�W���Ă��Ȃ��f�[�^�ɂ��Ă̓��b�V�����E�Ƃ̐ڐG������s��
	if( c_iMFlag == false ) {
		// ���b�V�����E�ƃI�u�W�F�N�g�̐ڐG�`�F�b�N
		VARIANT_BOOL			a_vBool;
		IRelationalOperatorPtr	a_ipRel;
		a_ipRel	= e_ipMeshRect_Line;
		hr	= a_ipRel->Touches( c_ipChk_Src_Data.e_ipGeo, &a_vBool );
		
		if( FAILED (hr) )				{ return ( false ); }
		if( a_vBool != VARIANT_TRUE )	{ return ( false ); }
	}
	
	// �����ɐڐG����I�u�W�F�N�g���������Ă���
	IFeatureCursorPtr	a_ipTouch_Datas;

	a_ipTouch_Datas	= c_mChk_Src_WS.Search_Feature ( c_ipChk_Src_Data.e_ipGeo, esriSpatialRelTouches, "", RET_FCURSOR);
	if (a_ipTouch_Datas == NULL ) { 
		return ( false );
	}

	// �f�[�^���Z�b�g�ɗ��Ƃ�
	ms_data_info		a_msTouch_Datas;
	ims_data_info		a_imsIndx;
	int					a_iTouches_Count	= 0;
	a_iTouches_Count	= Set_Data_Infos ( a_ipTouch_Datas, a_msTouch_Datas );
	if( a_iTouches_Count == 0 ) {
		return	( false );
	}

	IGeometryPtr	a_ipMrgDat;
	a_ipMrgDat		= c_ipChk_Src_Data.e_ipGeo;
	// ������
	data_info		a_ipTmpMerged_Data;
	a_ipTmpMerged_Data	= c_ipChk_Src_Data;
	// ���b�V�����E�Ɛڂ�����̂����}�[�W���Ă���

	// DEBUG------------------------------
	ISpatialReferencePtr	a_ipSpRef;
	c_ipChk_Src_Data.e_ipGeo->get_SpatialReference ( &a_ipSpRef );
	//------------------------------------

	int	a_iEqual_F	= 0;
	for( a_imsIndx = a_msTouch_Datas.begin(); a_imsIndx != a_msTouch_Datas.end(); a_imsIndx++ )
	{
		data_info				a_dTouchDat	= *a_imsIndx;
		// �}�[�W����
		if( fnIsNeedMergePolygon2(	c_ipChk_Src_Data.e_ipGeo, a_dTouchDat.e_ipGeo ) ) {
			IGeometryPtr	a_ipTmpGeo;
			ITopologicalOperatorPtr	a_ipMrgTopo;
			a_ipMrgTopo	= a_ipMrgDat;
			hr	= a_ipMrgTopo->Union ( a_dTouchDat.e_ipGeo, &a_ipTmpGeo );
			if( FAILED (hr) ) {
				continue;
			}
			a_ipMrgDat	= a_ipTmpGeo;
			// ��ԎႢ���b�V���ԍ��̈�ԎႢPrimID��U��
			if( a_ipTmpMerged_Data.e_iMeshCode != a_dTouchDat.e_iMeshCode ) {
				if( a_ipTmpMerged_Data.e_iMeshCode > a_dTouchDat.e_iMeshCode ) {
					a_ipTmpMerged_Data	= a_dTouchDat;
				}
			}else if( a_ipTmpMerged_Data.e_iOID != a_dTouchDat.e_iOID ) {
				if( a_ipTmpMerged_Data.e_iOID > a_dTouchDat.e_iOID ) {
					a_ipTmpMerged_Data	= a_dTouchDat;
				}		
			}
			// DEBUG------------------------------
			a_ipTmpMerged_Data.e_ipGeo->putref_SpatialReference ( a_ipSpRef );
			//------------------------------------
		}
	}
	a_ipTmpMerged_Data.e_ipGeo	= a_ipMrgDat;
	if( a_ipTmpMerged_Data.e_ipGeo == c_ipChk_Src_Data.e_ipGeo ) {
		return	( false );
	}
	// �����ōċA�����āA�}�[�W���ׂ��I�u�W�F�N�g��S�ă}�[�W������̌`���Ԃ�
	if( !Get_Neibour_Obj ( a_ipTmpMerged_Data, true, c_mChk_Src_WS, c_cpCheck_Src_Layer, c_ipMerged_Data )) {
		return	( true );	// �����ɓ����Ă���ꍇ�̓}�[�W���Ă���̂�true��ԋp
	}
	return ( true );	
}

/** ------------------------------------------------------------------------------
	�ᐸ�x�I�u�W�F�N�g�̍쐬�i�|���S���j
--------------------------------------------------------------------------------*/
IGeometryPtr	main_rootin::
Get_LowAccuracy_Obj ( IGeometryPtr&	c_ipSrcObj,	// �I�u�W�F�N�g
					  int			c_iValue )	// �����_�ȉ������̂P�܂Ō��邩
{
	//----------------------------------------------------------
	// �����O�����擾
	IGeometryCollectionPtr	a_ipGeoCol;
	a_ipGeoCol	= c_ipSrcObj;
	long	a_lGeoCount	= 0;	
	a_ipGeoCol->get_GeometryCount ( &a_lGeoCount );

	// �|���S�����쐬����
	IPolygonPtr				a_ipRetPoly (CLSID_Polygon);
	IGeometryPtr			a_ipRetGeo ( a_ipRetPoly );
	IGeometryCollectionPtr	a_ipRetCol ( a_ipRetGeo );

	int	a_iGRoop = 0;
	for( a_iGRoop = 0; a_iGRoop < a_lGeoCount; a_iGRoop++ )
	{
		IGeometryPtr	a_iTmpGeo;
		a_ipGeoCol->get_Geometry (a_iGRoop, &a_iTmpGeo);

		IPointCollectionPtr	a_ipTmpPts;
		a_ipTmpPts	= a_iTmpGeo;

		// ����Geometry�̕�ԓ_��
		long	a_lSrc_Count = 0;
		a_ipTmpPts->get_PointCount ( &a_lSrc_Count );

		// �����O�̓_����擾
		WKSPoint*	a_wpSrcWKSPoints = NULL;
		a_wpSrcWKSPoints	= new WKSPoint[a_lSrc_Count];

		a_ipTmpPts->QueryWKSPoints ( 0, a_lSrc_Count, a_wpSrcWKSPoints );

		// ���x�𗎂Ƃ�
		int	i = 0;
		_int64	a_iTmpX = 0, a_iTmpY = 0;
		for( i = 0; i < a_lSrc_Count; i++ )
		{
			a_iTmpX = a_wpSrcWKSPoints[i].X * c_iValue;
			a_iTmpY = a_wpSrcWKSPoints[i].Y * c_iValue;
			// ���x�𗎂Ƃ����̂Ō��̓_��ɒl��߂�
			a_wpSrcWKSPoints[i].X	= (double)a_iTmpX / c_iValue;
			a_wpSrcWKSPoints[i].Y	= (double)a_iTmpY / c_iValue;
		}

		// ���x���Ƃ��������O���쐬
		IRingPtr			a_ipNewRing (CLSID_Ring);
		IPointCollectionPtr	a_ipNewPolyPts ( a_ipNewRing );
		IGeometryPtr		a_ipNewGeo ( a_ipNewRing );

		a_ipNewPolyPts->SetWKSPoints ( a_lSrc_Count, a_wpSrcWKSPoints );
		// �����O��ǉ�
		a_ipRetCol->AddGeometry ( (IGeometryPtr)a_ipNewRing );

		delete [] a_wpSrcWKSPoints;
	
	}

	//----------------------------------------------------------

	// ��ԎQ�Ƃ�t�^����
	ISpatialReferencePtr	a_ipSpRef;
	c_ipSrcObj->get_SpatialReference( &a_ipSpRef );
	a_ipRetPoly->putref_SpatialReference( a_ipSpRef );
	// �X�i�b�v����@2004.07.14
	a_ipRetPoly->SnapToSpatialReference();

	// �V���v���t�@�C��������
	ITopologicalOperator2Ptr	a_ipTopo;
	a_ipTopo	= a_ipRetPoly;

	if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
		e_sPrnInfo.Prn_SiNDY_Err ( 0, "Simplify�Ɏ��s", NULL );
		return	( NULL );
	}
	if( FAILED ( a_ipTopo->Simplify () ) ) {
		e_sPrnInfo.Prn_SiNDY_Err ( 0, "Simplify�Ɏ��s", NULL );
		return	( NULL );
	}

	//----------------------------------------------------------
	IGeometryCollectionPtr	a_ipGeoCol2;
	a_ipGeoCol2	= a_ipRetGeo;
	long	a_lGeoCount2	= 0;	
	a_ipGeoCol2->get_GeometryCount ( &a_lGeoCount2 );

	if( a_lGeoCount > 1 ) {
		char	tmpstr[256];
		sprintf(tmpstr, "������ �� = %d : �쐬�� = %d", a_lGeoCount,  a_lGeoCount2 );
		e_sPrnInfo.Prn_SiNDY_Err ( 0, tmpstr, NULL );
	}

	//----------------------------------------------------------

	return	( a_ipRetGeo );
}

/** ------------------------------------------------------------------------------
	���v�����f�[�^���}�b�`�e�[�u���ɓo�^����
	@return	����
--------------------------------------------------------------------------------*/
void	main_rootin::
Entry_to_Match_Tbl(	data_info&	c_dData1,	// ��r��
					data_info&	c_dData2 )	// ��r��
{
	match_record		a_mEntry;
	a_mEntry.e_dData1	= c_dData1;
	a_mEntry.e_dData2	= c_dData2;
	e_msMatch_Tbl.insert ( a_mEntry );
}

/** ------------------------------------------------------------------------------
	�Ή��e�[�u���̏o��
	@return	����
--------------------------------------------------------------------------------*/
void	main_rootin::
Print_Table		(	ms_data_info&		c_ipChk_Src_Data,		///< �`�F�b�N���f�[�^�̃Z�b�g[IN]
					ms_data_info&		c_ipChk_Tgt_Data	)	///< �`�F�b�N���f�[�^�̃Z�b�g[IN]
{
	FILE*	a_fpOutFp	= NULL;

	if( e_fpMatch_Fp == NULL ) {
		a_fpOutFp	= stdout;	// �w�肪�Ȃ��ꍇ�͕W���o�͂ɏo��
	}else {
		a_fpOutFp	= e_fpMatch_Fp;
	}

	char	a_cpPrintBuf[512];

	ims_match_record	a_iIndx;
	for ( a_iIndx = e_msMatch_Tbl.begin(); a_iIndx != e_msMatch_Tbl.end(); a_iIndx++ )
	{
		match_record	a_mCur_Rec	= *a_iIndx;

		// ���b�V���R�[�h�������ꍇ�ɂ͌��ݒ��ڒ��̃��b�V����t�^����
		if( a_mCur_Rec.e_dData1.e_iMeshCode == 0) {
			a_mCur_Rec.e_dData1.e_iMeshCode	= e_iMeshCode;
		}

		// ���b�V���{PrimID�ƁA�^�[�Q�b�g�̃I�u�W�F�N�gID��\��
		sprintf	( a_cpPrintBuf, "%d\t%d\t%d\t%d\t%d", a_mCur_Rec.e_dData1.e_iMeshCode,
					a_mCur_Rec.e_dData1.e_iOID, a_mCur_Rec.e_dData2.e_iOID,
					a_mCur_Rec.e_dData1.e_iAttr, a_mCur_Rec.e_dData2.e_iAttr );
		//�`��̈�v�ɂ���
		if( a_mCur_Rec.e_dData1.e_iStat & STAT_EQUAL ) {
			strcat	( a_cpPrintBuf, "\t�`���v�P" );
		}else {
			if( a_mCur_Rec.e_dData1.e_iStat & STAT_LOWA_EQUAL ) {
				strcat	( a_cpPrintBuf, "\t�`���v�Q" );			
			}else if( a_mCur_Rec.e_dData1.e_iStat & STAT_LOWA_MODIFY ) { 
				strcat	( a_cpPrintBuf, "\t�`��ύX�Q" );
			}else {
				strcat	( a_cpPrintBuf, "\t�`��ύX�P" );
			}
		}

		// �אڏ��
		if( a_mCur_Rec.e_dData1.e_iStat & STAT_RINOBJ ) {
			strcat	( a_cpPrintBuf, "\t�אڕ���\n" );
		}else {
			strcat	( a_cpPrintBuf, "\n" );
		}
		// �o��
		fprintf ( a_fpOutFp, "%s", a_cpPrintBuf);
	}
	// �폜�f�[�^�̏o��
	ims_data_info	a_iSrc_Indx;
	for( a_iSrc_Indx = c_ipChk_Src_Data.begin(); a_iSrc_Indx != c_ipChk_Src_Data.end(); a_iSrc_Indx++ )
	{
		data_info	a_dCur_Dat	= *a_iSrc_Indx;
		if( a_dCur_Dat.e_iMeshCode == 0) {
			a_dCur_Dat.e_iMeshCode	= e_iMeshCode;
		}
		fprintf	( a_fpOutFp, "%d\t%d\t%d\t�폜�f�[�^\n", a_dCur_Dat.e_iMeshCode, 0, a_dCur_Dat.e_iOID );
	}

	e_sPrnInfo.Set_Layer_Name	( e_cpCheck_Tgt_Layer );

	// �ǉ��f�[�^�̏o��
	ims_data_info	a_iTgt_Indx;
	for( a_iTgt_Indx = c_ipChk_Tgt_Data.begin(); a_iTgt_Indx != c_ipChk_Tgt_Data.end(); a_iTgt_Indx++ )
	{
		data_info	a_dCur_Dat	= *a_iTgt_Indx;
		if( a_dCur_Dat.e_iMeshCode == 0) {
			a_dCur_Dat.e_iMeshCode	= e_iMeshCode;
		}
		fprintf	( a_fpOutFp, "%d\t%d\t%d\t�ǉ��f�[�^\n", a_dCur_Dat.e_iMeshCode, 0, a_dCur_Dat.e_iOID ); 
	
		e_sPrnInfo.Set_OID	( a_dCur_Dat.e_iOID );
	}

	// �\����Ƀe�[�u�����N���A
	e_msMatch_Tbl.clear();
}

/** ------------------------------------------------------------------------------
	�\�[�X�ƃ^�[�Q�b�g�̃`�F�b�N���s��
	���\�[�X�ƃ^�[�Q�b�g�����ւ��ă`�F�b�N���ł���悤�ɂ������i�t�����̂��߁j
--------------------------------------------------------------------------------*/
void	main_rootin::
Chk_Src_Tgt_Data ( 		ms_data_info&		c_ipChk_Src_Data,		///< �`�F�b�N���f�[�^�̃Z�b�g[IN]
						ms_data_info&		c_ipChk_Tgt_Data,		///< �`�F�b�N���f�[�^�̃Z�b�g[IN]
						msi_handle_ws&		c_mChk_Src_WS,			///< �^�[�Q�b�g�̃��[�N�X�y�[�X�n���h��[IN]
						msi_handle_ws&		c_mChk_Tgt_WS,			///< �^�[�Q�b�g�̃��[�N�X�y�[�X�n���h��[IN]
						char*				c_cpCheck_Src_Layer ) 	///< �`�F�b�N���C����
{
	char	a_cpMsg[512];	// ���b�Z�[�W�p

	e_sPrnInfo.Set_Layer_Name( c_cpCheck_Src_Layer );
	e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_ERROR );

	// �Ή����Ƃꂽ�f�[�^�̃C�e���[�^�̃e�[�u��
	m_ims_data_info	a_mMatch_Ite_Tbl_Src, a_mMatch_Ite_Tbl_Tgt;
	ims_data_info	a_iIndx;

	int	a_iCount	= 0;
	// 
	for( a_iIndx = c_ipChk_Src_Data.begin(); a_iIndx != c_ipChk_Src_Data.end(); a_iIndx++, a_iCount++ )
	{
		if( a_iCount % 500 == 0 ) {
			PrnMsg(stdout, NULL, "count = %d/%d\r", a_iCount, c_ipChk_Src_Data.size() );
		}
		data_info			a_dCurDat	= *a_iIndx;

		e_sPrnInfo.Set_OID	( a_dCurDat.e_iOID );

		// ���b�V�����E�ɐڂ���I�u�W�F�N�g�ɂ��Ă͗אڕ����擾���A�}�[�W�����`����쐬����
		data_info			a_dCurDat2;	// ��r�p�I�u�W�F�N�g
		bool				a_bMrgFlag	= false;
		a_bMrgFlag	= Get_Neibour_Obj	( a_dCurDat, false, c_mChk_Src_WS, c_cpCheck_Src_Layer, a_dCurDat2 );

		// OBJECTID�ŃT�[�`
		ims_data_info		a_iFind_Indx;
		int					a_iID_Search_Count	= 0;	// �������b�V���R�[�h��PrimID�̂��̂̐�
		a_iFind_Indx		= c_ipChk_Tgt_Data.find( a_dCurDat2 );
		a_iID_Search_Count	= c_ipChk_Tgt_Data.count( a_dCurDat2 );

		// �f�[�^�����������ꍇ�͌`��̈�v������
		if( a_iFind_Indx != c_ipChk_Tgt_Data.end() ) {
			int		a_iSC	= 0;
			ims_data_info	a_iFind_Cur_Indx	= a_iFind_Indx;
	
			for( a_iSC = 0; a_iSC < a_iID_Search_Count; a_iFind_Cur_Indx++, a_iSC++ )
			{
				data_info			a_lSearched	= *a_iFind_Cur_Indx;

				// �`��`�F�b�N���s��
				if( Get_Match_Info ( a_dCurDat,	a_lSearched, c_ipChk_Tgt_Data ) != 0 ) {
					Entry_to_Match_Tbl ( a_dCurDat, a_lSearched );	// �Ή�����ꂽ�f�[�^���R���e�i�ɓo�^
					a_mMatch_Ite_Tbl_Src.insert	( m_ims_data_info::value_type( (long)(&(*a_iIndx)), a_iIndx ) ); // �������f�[�^�̃C���f�b�N�X���R���e�i�ɓo�^
					a_mMatch_Ite_Tbl_Tgt.insert	( m_ims_data_info::value_type( (long)(&(*a_iFind_Cur_Indx)), a_iFind_Cur_Indx ) ); // ������f�[�^�̃C���f�b�N�X���R���e�i�ɓo�^
				}
			}
		}else {	// OBJECTID����v������̂�������Ȃ��ꍇ
		
			IFeatureCursorPtr	a_ipChk_Tgt_Datas;
			// ID����v������̂��Ȃ��ꍇ�͋�Ԍ����ŃT�[�`����
			a_ipChk_Tgt_Datas	= c_mChk_Tgt_WS.Search_Feature ( a_dCurDat2.e_ipGeo, esriSpatialRelIntersects, "", RET_FCURSOR);
			if (a_ipChk_Tgt_Datas != NULL ) {
				// �����ň������������f�[�^���Z�b�g�ɗ��Ƃ�
				ms_data_info		a_mIntersects_Datas;
				ims_data_info		a_mIs_Indx;
				int					a_iIntersects_Count	= 0;
				a_iIntersects_Count	= Set_Data_Infos ( a_ipChk_Tgt_Datas, a_mIntersects_Datas );
				
				for( a_mIs_Indx = a_mIntersects_Datas.begin(); a_mIs_Indx != a_mIntersects_Datas.end(); a_mIs_Indx++ )
				{
					data_info	a_dSearch_Tgt	= *a_mIs_Indx;
					// ��Ԍ����ň������������f�[�^�ƈ�v������
					// �`��`�F�b�N���s��
					if( Get_Match_Info ( a_dCurDat,	a_dSearch_Tgt, c_ipChk_Tgt_Data ) != 0 ) {
						Entry_to_Match_Tbl ( a_dCurDat, a_dSearch_Tgt );	// �Ή�����ꂽ�f�[�^���R���e�i�ɓo�^
						a_mMatch_Ite_Tbl_Src.insert	( m_ims_data_info::value_type( (long)(&(*a_iIndx)), a_iIndx ) ); // �������f�[�^�̃C���f�b�N�X���R���e�i�ɓo�^
						//--------------------------------------------------------------------------
						ims_data_info		a_iMatch_Indx;
						int	a_iMatch_Count	= c_ipChk_Tgt_Data.count ( a_dSearch_Tgt );
						a_iMatch_Indx	= c_ipChk_Tgt_Data.find( a_dSearch_Tgt  );
						if( a_iMatch_Indx != c_ipChk_Tgt_Data.end() ) {
							ims_data_info		a_iSame_Indx;
							int					a_iSame_Count	= 0;
							// ����ID�������̂�����̂ŁA�I�u�W�F�N�gID����v������̂�T���A���̃C���f�b�N�X���폜����悤�ɂ���
							for( a_iSame_Indx = a_iMatch_Indx; a_iSame_Count < a_iMatch_Count; a_iSame_Indx++, a_iSame_Count++)
							{
								data_info	a_iCurSame	= *a_iSame_Indx;
								if( a_iCurSame.e_iOID == a_dSearch_Tgt.e_iOID ) {
									// �f�[�^�e�[�u���ɊY���f�[�^���������Ƃ������ƂȂ̂ŁA�폜���X�g�ɓo�^����
									a_mMatch_Ite_Tbl_Tgt.insert ( m_ims_data_info::value_type( (long)(&(*a_iSame_Indx)), a_iSame_Indx ) );
								}
							}
						}else {
							// ������Ȃ��Ȃ烁�b�V���ň����������Ă��Ȃ����̂Ȃ̂ŁA�폜���X�g�ɂ͓o�^���Ȃ�
#ifdef _DEBUG
							e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_NOTICE );
							sprintf	( a_cpMsg,	"Tgt�̍폜�e�[�u���ɂȂ��f�[�^�ƈ�v\t%d\t%d", a_dCurDat.e_iOID, a_dSearch_Tgt.e_iOID );
							e_sPrnInfo.Prn_SiNDY_Err ( 0, a_cpMsg, NULL );
							e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_ERROR );
#endif
						}
						//--------------------------------------------------------------------------
					}
				}
			}
		}
	}
	PrnMsg(stdout, NULL, "\n");
	// �Ή����Ƃꂽ�f�[�^���\�[�X�ƃ^�[�Q�b�g�̃e�[�u����������Ă���
	im_ims_data_info	a_imMatch_Ite_Src, a_imMatch_Ite_Tgt;
	for( a_imMatch_Ite_Src = a_mMatch_Ite_Tbl_Src.begin(); a_imMatch_Ite_Src != a_mMatch_Ite_Tbl_Src.end(); a_imMatch_Ite_Src++ )
	{
		c_ipChk_Src_Data.erase ( (*a_imMatch_Ite_Src).second );
	}
	for( a_imMatch_Ite_Tgt = a_mMatch_Ite_Tbl_Tgt.begin(); a_imMatch_Ite_Tgt != a_mMatch_Ite_Tbl_Tgt.end(); a_imMatch_Ite_Tgt++ )
	{
		c_ipChk_Tgt_Data.erase ( (*a_imMatch_Ite_Tgt).second );
	}
	PrnMsg(stdout, NULL, "src = �c��%d�� tgt = �c��%d\n", c_ipChk_Src_Data.size(), c_ipChk_Tgt_Data.size() );
}

/**
	�폜�E�ǉ��Ɣ��肳�ꂽ���̂ɑ΂��Đ��x���኱���Ƃ����`�ōă`�F�b�N���s��
*/
void	main_rootin::
Chk_Retry ( 			ms_data_info&		c_ipChk_Src_Data,		///< �`�F�b�N���f�[�^�̃Z�b�g[IN]
						ms_data_info&		c_ipChk_Tgt_Data,		///< �`�F�b�N���f�[�^�̃Z�b�g[IN]
						msi_handle_ws&		c_mChk_Src_WS,			///< �^�[�Q�b�g�̃��[�N�X�y�[�X�n���h��[IN]
						msi_handle_ws&		c_mChk_Tgt_WS,			///< �^�[�Q�b�g�̃��[�N�X�y�[�X�n���h��[IN]
						char*				c_cpCheck_Src_Layer ) 	///< �`�F�b�N���C����
{
	char	a_cpMsg[512];	// ���b�Z�[�W�p

	const int	SEIDO	= 10000000;
//	const int	SEIDO	= 1000000;

	e_sPrnInfo.Set_Layer_Name	( c_cpCheck_Src_Layer );
	e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_ERROR );

	// �Ή����Ƃꂽ�f�[�^�̃C�e���[�^�̃e�[�u��
	m_ims_data_info	a_mMatch_Ite_Tbl_Src, a_mMatch_Ite_Tbl_Tgt;
	ims_data_info	a_iIndx;

	int	a_iCount	= 0;
	// 
	for( a_iIndx = c_ipChk_Src_Data.begin(); a_iIndx != c_ipChk_Src_Data.end(); a_iIndx++, a_iCount++ )
	{
		if( a_iCount % 500 == 0 ) {
			PrnMsg(stdout, NULL, "count = %d/%d\n", a_iCount, c_ipChk_Src_Data.size() );
		}
		data_info			a_dCurDat	= *a_iIndx;

		e_sPrnInfo.Set_OID	( a_dCurDat.e_iOID );
	
		// ���b�V�����E�ɐڂ���I�u�W�F�N�g�ɂ��Ă͗אڕ����擾���A�}�[�W�����`����쐬����
		data_info			a_dCurDat2;	// ��r�p�I�u�W�F�N�g
		bool				a_bMrgFlag	= false;
		a_bMrgFlag	= Get_Neibour_Obj	( a_dCurDat, false, c_mChk_Src_WS, c_cpCheck_Src_Layer, a_dCurDat2 );

		IFeatureCursorPtr	a_ipChk_Tgt_Datas;

		// ��Ԍ����ŃT�[�`����
		a_ipChk_Tgt_Datas	= c_mChk_Tgt_WS.Search_Feature ( a_dCurDat2.e_ipGeo, esriSpatialRelIntersects, "", RET_FCURSOR);
		if (a_ipChk_Tgt_Datas != NULL ) {
			// �����ň������������f�[�^���Z�b�g�ɗ��Ƃ�
			ms_data_info		a_mIntersects_Datas;
			ims_data_info		a_mIs_Indx;
			int					a_iIntersects_Count	= 0;
			a_iIntersects_Count	= Set_Data_Infos ( a_ipChk_Tgt_Datas, a_mIntersects_Datas );
	
			int	a_iEqual_F	= 0;
			// �������������f�[�^�̃��[�v
			for( a_mIs_Indx = a_mIntersects_Datas.begin(); a_mIs_Indx != a_mIntersects_Datas.end(); a_mIs_Indx++ )
			{
				data_info	a_dSearch_Tgt	= *a_mIs_Indx;
				data_info	a_dPutDat		= a_dCurDat;	///�@�o�^�p�f�[�^ 2005.12.21

				// ��Ԍ����ň������������f�[�^�ƈ�v������
				int	a_iChk_Result	= 0;
				a_iChk_Result	= Shape_Chk_LowAccuracy ( a_dCurDat2.e_ipGeo, a_dSearch_Tgt.e_ipGeo );	// 2006.09.27 �V�K���W�b�N
				if( a_iChk_Result == STAT_LOWA_EQUAL ||	
					a_iChk_Result == STAT_LOWA_MODIFY ) {

					if( a_iChk_Result == STAT_LOWA_EQUAL ) {
						a_dPutDat.e_iStat	|= STAT_LOWA_EQUAL;
					}else {
						a_dPutDat.e_iStat	|= STAT_LOWA_MODIFY;
					}

					// �^�[�Q�b�g�̃f�[�^���瓯�����̂�������I
					//----------------------------------------------------------
					ims_data_info	a_iTgtIndx;
					int	a_iFindTgt	= 0;
					for(a_iTgtIndx = c_ipChk_Tgt_Data.begin(); a_iTgtIndx != c_ipChk_Tgt_Data.end(); a_iTgtIndx++)
					{
						data_info	a_iCurDat	= *a_iTgtIndx;
						if( a_dSearch_Tgt.e_iOID == a_iCurDat.e_iOID ) {
							a_iFindTgt	= 1;
							break;
						}
					}
					// �^�[�Q�b�g�̎c��ɂ���ꍇ�ȊO�͒ǉ����Ȃ��i��v�������̂Ƃ��Ԃ�ꍇ�����邽�߁j
					if( a_iFindTgt == 1 ) {
						// �}�b�`�����f�[�^�̓e�[�u���ɓo�^����
						Entry_to_Match_Tbl ( a_dPutDat, a_dSearch_Tgt );
						a_mMatch_Ite_Tbl_Src.insert	( m_ims_data_info::value_type( (long)(&(*a_iIndx)), a_iIndx ) );
						// �e�[�u���ɓo�^
						a_mMatch_Ite_Tbl_Tgt.insert	( m_ims_data_info::value_type( (long)(&(*a_iTgtIndx)), a_iTgtIndx ) );
						// ���������ꍇ�̓��[�v�𔲂���
						break;
					}else {
#ifdef _DEBUG
						e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_NOTICE );
						sprintf	( a_cpMsg,	"Tgt�̍폜�e�[�u���ɂȂ��f�[�^�ƈ�v\t%d", a_dSearch_Tgt.e_iOID );
						e_sPrnInfo.Prn_SiNDY_Err ( 0, a_cpMsg, NULL );
						e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_ERROR );
#endif
					}
					//----------------------------------------------------------

				}else {
					continue;
				}
			}
		}
	}

	// �Ή����Ƃꂽ�f�[�^���\�[�X�ƃ^�[�Q�b�g�̃e�[�u����������Ă���
	im_ims_data_info	a_imMatch_Ite_Src, a_imMatch_Ite_Tgt;
	for( a_imMatch_Ite_Src = a_mMatch_Ite_Tbl_Src.begin(); a_imMatch_Ite_Src != a_mMatch_Ite_Tbl_Src.end(); a_imMatch_Ite_Src++ )
	{
		c_ipChk_Src_Data.erase ( (*a_imMatch_Ite_Src).second );
	}
	PrnMsg(stdout, NULL, "�ă`�F�b�N:�c��%d��\n", c_ipChk_Src_Data.size() );
	for( a_imMatch_Ite_Tgt = a_mMatch_Ite_Tbl_Tgt.begin(); a_imMatch_Ite_Tgt != a_mMatch_Ite_Tbl_Tgt.end(); a_imMatch_Ite_Tgt++ )
	{
		c_ipChk_Tgt_Data.erase ( (*a_imMatch_Ite_Tgt).second );
	}
	PrnMsg(stdout, NULL, "�ă`�F�b�N:�c��%d��\n", c_ipChk_Tgt_Data.size() );
}

/** ------------------------------------------------------------------------------
	�����`�F�b�N�̃��[�v
--------------------------------------------------------------------------------*/
void	main_rootin::
Diff_Check_Roops ( IFeatureCursorPtr&	c_ipChk_Src_Cursor,	 	///< �`�F�b�N���ƂȂ�f�[�^�̃J�[�\��[IN]
				   IFeatureCursorPtr&	c_ipChk_Tgt_Cursor ) 	///< �`�F�b�N��ƂȂ�f�[�^�̃J�[�\��[IN]
{
	// �s���E�̃��[�v
	int	i=0;
	IFeaturePtr		a_ipCity_Admin_Feat;
	int				a_iCityName_Count	= 0;
	int				a_iCityOffice_Count	= 0;

	ms_data_info	a_eChk_Src_Data, a_eChk_Tgt_Data;

	// �`�F�b�N���Ɛ�̃f�[�^���Z�b�g�ɂ���
	Set_Data_Infos	( c_ipChk_Src_Cursor, a_eChk_Src_Data );
	Set_Data_Infos	( c_ipChk_Tgt_Cursor, a_eChk_Tgt_Data );
	// �f�[�^�`�F�b�N
	Chk_Src_Tgt_Data ( a_eChk_Src_Data, a_eChk_Tgt_Data, e_mChk_Src_WS, e_mChk_Tgt_WS, e_cpCheck_Src_Layer );
	// �Ή��e�[�u���̏o��
	Print_Table		( a_eChk_Src_Data, a_eChk_Tgt_Data );
}


/** ------------------------------------------------------------------------------
	�`�F�b�N���s��
	�������ŃG���[�t�@�C���̏��������s��
	@return	����	true
	@return ���s	false
--------------------------------------------------------------------------------*/
bool	main_rootin::
check_exe	( void )
{
	char	a_cpErrMsg[512];

	// �J�n���Ԃ̐ݒ�
	e_eExeTime.Set_Start();

	// �ڑ�
	IWorkspacePtr			ipWorkspace_Src;
	IWorkspacePtr			ipWorkspace_Tgt;

	if( !SiNDY_DB_Open	( ipWorkspace_Src, ipWorkspace_Tgt )) {
		PrnMsg(e_fErr_Fp, stderr, "�ڑ��o���Ȃ�����\n" );
		return	( false );
	}

	// DB�ڑ��ɂ����������Ԃ̕\��
	PrnMsg(stdout, NULL, "SiNDY-DB�ڑ��������� = %0.2f(s)�ł���\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );


	// ���C���I�[�v��
	if( !Open_Layers ( ipWorkspace_Src, ipWorkspace_Tgt ) ){	// �n�}�p���[�N�X�y�[�X
		PrnMsg(e_fErr_Fp, stderr, "���C���I�[�v���Ɏ��s���Ă��܂�\n");
		return	( false );
	}

	// ���b�V�����X�g�̓ǂݍ���
	mk_input_list		a_mMesh_List;
	if( !Read_Chk_List( ipWorkspace_Src, a_mMesh_List ) ) {
		PrnMsg(e_fErr_Fp, stderr, "���b�V�����X�g���擾�ł��܂���ł���\n" );
		return	( false );	
	}

	// ���b�V���̃��[�v
	int					a_iCur_Mesh;
	int	a_iRoopCount	= 0;
	bool isErrorStat = false;
	for( a_iCur_Mesh = a_mMesh_List.get_mesh_first(); a_iCur_Mesh != -1; a_iCur_Mesh = a_mMesh_List.get_mesh_next(), a_iRoopCount++ )
	{
		PrnMsg(stdout, NULL, "#���b�V���R�[�h [%d] ������...", a_iCur_Mesh );
	
		e_eExeTime.Set_Lap_Start();

		IFeatureCursorPtr	a_ipChk_Src_Datas, a_ipChk_Tgt_Datas;
	
		// ���b�V����`�̎��o��
		IGeometryPtr	a_ipMeshRect;
		a_ipMeshRect	= get_mesh_rect( a_iCur_Mesh );
		if( a_ipMeshRect == NULL ) {	// ���b�V����`�������ꍇ
			PrnMsg(e_fErr_Fp, stderr, "���b�V���R�[�h<%d>�̋�`��������܂���ł���\n", a_iCur_Mesh );
			isErrorStat = true;
			continue;
		}
		// �N���X���Ŏg�p�ł���悤�ɂ��Ă���
		e_ipMeshRect	= a_ipMeshRect;
		e_iMeshCode		= a_iCur_Mesh;

		// ���b�V���̋��E�������o��
		ITopologicalOperatorPtr	a_ipMeshRectTopo;
		a_ipMeshRectTopo		= Get_Rect_Polygon( IEnvelopePtr(a_ipMeshRect) );
		if( FAILED (a_ipMeshRectTopo->get_Boundary ( &e_ipMeshRect_Line ) )) {
			PrnMsg(e_fErr_Fp, stderr, "���b�V���R�[�h<%d>�̋�`�̋��E�����擾�ł��Ȃ�\n", a_iCur_Mesh );
			isErrorStat = true;
			continue;
		}

		//-----------------------------------------------------------		
		IEnvelopePtr	a_ipRect, a_ipLargeRect;
		a_ipMeshRect->get_Envelope ( &a_ipRect );

		// ���b�V����`�ŃT�[�`����
		a_ipChk_Src_Datas	= e_mChk_Src_WS.Search_Feature ( a_ipMeshRect, esriSpatialRelIntersects, "", RET_FCURSOR|RET_COUNT );
		if (a_ipChk_Src_Datas == NULL ) {
			PrnMsg(e_fErr_Fp, stderr, "<%d>���b�V����`�Ńf�[�^�����܂���(��r��)\n", a_iCur_Mesh );
			isErrorStat = true;
			continue;
		}
		a_ipChk_Tgt_Datas	= e_mChk_Tgt_WS.Search_Feature ( a_ipMeshRect, esriSpatialRelIntersects, "", RET_FCURSOR|RET_COUNT );
		if ( a_ipChk_Tgt_Datas == NULL ) {
			PrnMsg(e_fErr_Fp, stderr, "<%d>���b�V����`�Ńf�[�^�����܂���(��r��)\n", a_iCur_Mesh );
			isErrorStat = true;
			continue;
		}

		//-----------<�`�F�b�N�i�s�x�\��>--------------------
		long		a_lTCount	= 0;
		a_lTCount	= e_mChk_Src_WS.Get_Data_Count ();
		e_mChk_Src_WS.Clear_Data_Count();

		PrnMsg(stdout, NULL, "���f�[�^���� src=%d ��, tgt=%d ��\n", a_lTCount, e_mChk_Tgt_WS.Get_Data_Count() );

		Diff_Check_Roops ( a_ipChk_Src_Datas, a_ipChk_Tgt_Datas );

		DWORD			a_dMake_Finish	= 0;
		a_dMake_Finish	= timeGetTime();

		PrnMsg(stdout, NULL, "#���b�V���R�[�h [%d] �������� = %0.2f(s)�ł���\n", a_iCur_Mesh, e_eExeTime.Get_Lap() / 1000.0 );
	}

	//�@���s���I��
	// �I�����Ԃ̎擾
	PrnMsg(stdout, NULL, "�f�[�^���������� = %0.2f(s)�ł���\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );
	if( isErrorStat == true ) {
		return false;
	}else {
		return true;
	}
}

/**
	�`�F�b�N�p�s���E���X�g�A���b�V���R�[�h���X�g�̓ǂݍ���
*/
bool	main_rootin::
Read_Chk_List	(	IWorkspacePtr&	c_ipWSpace,			///< ���[�N�X�y�[�X
					mk_input_list&	c_aChk_List_Cls	)	///< ���X�g�Ǘ��N���X�̎Q��
{

	if( eExe_Flag & ON_MESH ) {		//	���b�V���R�[�h���X�g�w��̏ꍇ
		if( e_cpMeshCode_File == NULL ) {
			PrnMsg(e_fErr_Fp, stderr, "���b�V���R�[�h���X�g�t�@�C�������Ƃ肾���܂���ł���\n");
			return	( false );
		}	
		if( c_aChk_List_Cls.read_mesh_list ( e_cpMeshCode_File ) != 0 ) {	// �����Ń��X�g��ǂݍ���
			PrnMsg(e_fErr_Fp, stderr, "���b�V���R�[�h���X�g�t�@�C��<%s>�̓ǂݍ��݂Ɏ��s[check_exe]\n", e_cpMeshCode_File );
			return	( false );
		}
	}else
	if( eExe_Flag & ON_MONO_MESH ) {	// �P���b�V���w��̏ꍇ
		if( c_aChk_List_Cls.read_mesh_list ( e_iMeshCode ) != 0 ) {	// �����Ń��X�g��ǂݍ���
			PrnMsg(e_fErr_Fp, stderr, "���b�V���R�[�h���X�g�t�@�C��<%s>�̓ǂݍ��݂Ɏ��s[check_exe]\n", e_cpMeshCode_File );
			return	( false );
		}
	}else {
		return ( false );
	}

	if( !get_mesh_init( c_aChk_List_Cls , c_ipWSpace ) ) {
		PrnMsg(e_fErr_Fp, stderr, "���b�V�����C���̊l���̏������Ɏ��s[check_exe]\n");
		return	( false );
	}
		
	switch	( eDigit )
	{
	case	8:
	case	6:
	case	4:
		break;
	default:
		PrnMsg(e_fErr_Fp, stderr, "���X�g�̃��b�V�������ɂ͑Ή����Ă��܂���[check_exe]\n");
		return	( false );
	}

	return	( true );
}

IEnvelopePtr	main_rootin::
Get_Expand_Rect	(	IEnvelopePtr&	c_iRect,	///< ���͋�`[IN]
					int				c_iMode,	///< �쐬���[�h�i0=����`�ƍ������_���ꏏ�ɍ쐬�A1=����`�𒆐S�ɍ쐬)[IN]
					double			c_dX,		///< X�����̔{��[IN]
					double			c_dY	)	///< Y�����̔{��[IN]
{
	IEnvelopePtr	a_ipExpand_Rect ( CLSID_Envelope );
	double			a_dMin_Lon	= 0.0,	a_dMin_Lat	= 0.0;
	double			a_dMax_Lon	= 0.0,	a_dMax_Lat	= 0.0;

	HRESULT			hr;

	hr	= c_iRect->get_XMin( &a_dMin_Lon );
	if( FAILED ( hr ) ) {
		return	( NULL );
	}
	hr	= c_iRect->get_YMin( &a_dMin_Lat );
	if( FAILED ( hr ) ) {
		return	( NULL );
	}
	hr	= c_iRect->get_XMax( &a_dMax_Lon );
	if( FAILED ( hr ) ) {
		return	( NULL );
	}
	hr	= c_iRect->get_YMax( &a_dMax_Lat );
	if( FAILED ( hr ) ) {
		return	( NULL );
	}

	double	a_dLon_Wid	= a_dMax_Lon - a_dMin_Lon;
	double	a_dLat_Wid	= a_dMax_Lat - a_dMin_Lat;

	if( c_iMode == 0 ) {
		// �����͕ς��Ȃ�
		a_dMax_Lon	= a_dMin_Lon + (a_dLon_Wid * c_dX);
		a_dMax_Lat	= a_dMin_Lat + (a_dLat_Wid * c_dY);
	}else {
		a_dMin_Lon	-=	(a_dLon_Wid * c_dX / 2.0);
		a_dMax_Lon	+=	(a_dLon_Wid * c_dX / 2.0);
		a_dMin_Lat	-=	(a_dLat_Wid * c_dY / 2.0);
		a_dMax_Lat	+=	(a_dLat_Wid * c_dY / 2.0);
	}

	hr	= a_ipExpand_Rect-> PutCoords ( a_dMin_Lon, a_dMin_Lat, a_dMax_Lon, a_dMax_Lat );
	if( FAILED ( hr ) ) {
		return	( NULL );
	}

	// ��ԎQ�Ƃ����킹��
	ISpatialReferencePtr	a_ipSpRef;
	hr	=c_iRect->get_SpatialReference ( &a_ipSpRef );
	if( FAILED ( hr ) ) {
		return ( NULL );
	}
	hr	= a_ipExpand_Rect->putref_SpatialReference	( a_ipSpRef );
	if( FAILED ( hr ) ) {
		return ( NULL );
	}
	return ( a_ipExpand_Rect );
}

bool main_rootin::
fnIsNeedMergePolygon2(	IGeometryPtr& ipSrcGeom,
						IGeometryPtr& ipTgtGeom)
{
	ITopologicalOperatorPtr  ipSrcLine(ipSrcGeom);
	ITopologicalOperatorPtr  ipTgtLine2(ipTgtGeom);
	IGeometryPtr ipTgtLine;

	IPointCollectionPtr ipPointCollectionS(ipSrcGeom);
	ATLASSERT( ipPointCollectionS != NULL );
	IPointCollectionPtr ipPointCollectionT(ipTgtGeom);
	ATLASSERT( ipPointCollectionT != NULL );
	LONG lPointCountS = 0;
	LONG lPointCountT = 0;
	IPointPtr ipPoint = NULL;
	ipPointCollectionS->get_PointCount( &lPointCountS );
	ipPointCollectionT->get_PointCount( &lPointCountT );

	if(lPointCountS > 3 && lPointCountT > 3){
		WKSPoint *aSrcPoints = new WKSPoint[lPointCountS];
		ipPointCollectionS->QueryWKSPoints(0, lPointCountS, aSrcPoints);
		WKSPoint *aTgtPoints = new WKSPoint[lPointCountT];
		ipPointCollectionT->QueryWKSPoints(0, lPointCountT, aTgtPoints);
		for(LONG i = 0; i < lPointCountS -1; i++){
			if(aSrcPoints[i].X == m_CompMinX && aSrcPoints[i+1].X == m_CompMinX){
				for(LONG j = 0; j < lPointCountT - 1; j++){
					if(aTgtPoints[j].X == m_CompMinX  && aTgtPoints[j+1].X == m_CompMinX){
						if(aSrcPoints[i].Y == aTgtPoints[j+1].Y && aSrcPoints[i+1].Y == aTgtPoints[j].Y){
							delete []aSrcPoints;
							delete []aTgtPoints;
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].X == m_CompMaxX && aSrcPoints[i+1].X == m_CompMaxX){
				for(LONG j = 0; j < lPointCountT - 1; j++){
					if(aTgtPoints[j].X == m_CompMaxX  && aTgtPoints[j+1].X == m_CompMaxX){
						if(aSrcPoints[i].Y == aTgtPoints[j+1].Y && aSrcPoints[i+1].Y == aTgtPoints[j].Y){
							delete []aSrcPoints;
							delete []aTgtPoints;
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].Y == m_CompMinY && aSrcPoints[i+1].Y == m_CompMinY){
				for(LONG j = 0; j < lPointCountT - 1; j++){
					if(aTgtPoints[j].Y == m_CompMinY  && aTgtPoints[j+1].Y == m_CompMinY){
						if(aSrcPoints[i].X == aTgtPoints[j+1].X && aSrcPoints[i+1].X == aTgtPoints[j].X){
							delete []aSrcPoints;
							delete []aTgtPoints;
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].Y == m_CompMaxY && aSrcPoints[i+1].Y == m_CompMaxY){
				for(LONG j = 0; j < lPointCountT - 1; j++){
					if(aTgtPoints[j].Y == m_CompMaxY  && aTgtPoints[j+1].Y == m_CompMaxY){
						if(aSrcPoints[i].X == aTgtPoints[j+1].X && aSrcPoints[i+1].X == aTgtPoints[j].X){
							delete []aSrcPoints;
							delete []aTgtPoints;
							return 1;
						}
					}
				}
			}
		}
		delete []aSrcPoints;
		delete []aTgtPoints;
	}
	return 0;
}
