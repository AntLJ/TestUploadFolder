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
#include <sindy/workspace.h>

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
SiNDY_DB_Open	( IWorkspacePtr& c_ipWorkspace_Src )
{
	c_ipWorkspace_Src = sindy::create_workspace(m_sConnectStr);
	if(!c_ipWorkspace_Src ){
		return false;
	}
	return	true;
}

/** ------------------------------------------------------------------------------
	�G���[���O�t�@�C�����I�[�v������
	@return	true	�I�[�v������
	@return false	�I�[�v�����s
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Err_Log	( void )
{
	if( e_cpLog_File.size() == 0 ) {
		return	( true );
	}else {
		e_fErr_Fp	= fopen ( (char*)e_cpLog_File.c_str(), "w" );
		if( e_fErr_Fp == NULL ) {
			PrnMsg(stderr, NULL, "�G���[���O�t�@�C��<%s>���I�[�v���ł��Ȃ�\n", e_cpLog_File.c_str() );
			return	( false );
		}
	}
	return	( true );
}

/** ------------------------------------------------------------------------------
	�����e�[�u���t�@�C�����I�[�v������
	@return	true	�I�[�v������
	@return false	�I�[�v�����s
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Attr_Tbl	( void )
{
	if( e_cpAttr_Tbl_File.size() == 0 ) {
		return	( true );
	}else {
		e_fpAttr_Fp	= fopen ( (char*)e_cpAttr_Tbl_File.c_str(), "w" );
		if( e_fpAttr_Fp == NULL ) {
			PrnMsg(e_fErr_Fp, stderr, "�o�͑Ή��e�[�u���t�@�C��<%s>���I�[�v���ł��Ȃ�\n", e_cpAttr_Tbl_File.c_str() );
			return	( false );
		}
	}
	return	( true );
}


#define	OPTION_MAX	6
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

	for(i = 0; i < OPTION_MAX; i++)
	{
		a_optcount[i] = 0;
	}

	// ���������񐔂̃`�F�b�N
	for(i = 0; i < argc; i++)
	{
		if(strlen(argv[i]) > BMAX) {
			PrnMsg(stderr, NULL, "���������񂪒������܂�!!\n" );
			return	( false );
		}
		// �o�[�W�������\���`�F�b�N
		if(strcmp(argv[i], "-v") == 0) {
			print_version();
			return	( false );
		}
	}


	if(argc < 3) {
		return( false );
	}

	e_cpInit_File		= argv[1];	///< �������t�@�C��(�Z��DB�ڑ�)���܂�
	e_cpCheck_Src_Layer	= argv[2];	///< �`�F�b�N�����C��

	// �I�v�V����������A���̂Ƃ���g�p���Ȃ�
	for(i = 3; i < argc;)
	{
		if(strcmp(argv[i], "-l") == 0) { // ���O�t�@�C���ݒ�i�ݒ肪�����ꍇ�́A�G���[��stderr�֐��ꗬ���ɂȂ�j
			if(argc < i+2) return( false );
			e_cpLog_File	= argv[i+1];	///< ���b�V���R�[�h���X�g�t�@�C��
			a_optcount[0]++;
			i+=2;
		} else if(strcmp(argv[i], "-m") == 0) {
			if(argc < i+2) return( false );
			e_cpMeshCode_File	= argv[i+1];	///< ���b�V���R�[�h���X�g�t�@�C��
			eExe_Flag			|= ON_MESH;
			a_optcount[1]++;
			i+=2;
		} else if(strcmp(argv[i], "-M") == 0) {
			if(argc < i+2) return( false );
			eExe_Flag			|= ON_MONO_MESH;
			e_iMeshCode			= atoi(argv[i+1]);
			a_optcount[2]++;
			i+=2;
		} else if(strcmp(argv[i], "-t") == 0) {
			if(argc < i+2) return( false );
			e_cpAttr_Tbl_File	= argv[i+1];		///< �Ή����X�g�t�@�C��
			eExe_Flag			|= ON_ATTR_FILE;
			a_optcount[4]++;
			i+=2;
		} else if(strcmp(argv[i], "-v") == 0) {
			print_version();
			a_optcount[5]++;
			i+=1;
			return	( false );	// ���ɂ�������H
		} else {
			PrnMsg(stderr, NULL, "�I�v�V���� %s �͊Ԉ���Ă܂�!!\n", argv[i]);
			return	( false );
		}
	}

	// �I�v�V�����d���̃`�F�b�N
	for(i = 0; i < OPTION_MAX; i++)
	{
		if(a_optcount[i] > 1) {
			PrnMsg(stderr, NULL, "�I�v�V�����̎w�肪�d�����Ă��܂�!!\n");
			return	( false );
		}
	}

	// �������t�@�C���ݒ�
	if( e_pTool_Init.read_init_file ( (char*)e_cpInit_File.c_str() ) == -1 ) {
		return	( false );	// ���b�Z�[�W�͓����ŏo���Ă���
	}else {
		m_sConnectStr.Format(_T("%s/%s/%s/%s/%s"), e_pTool_Init.get_UserName(), e_pTool_Init.get_Password(),
			  e_pTool_Init.get_Version(), e_pTool_Init.get_Instance(), e_pTool_Init.get_Server() );
	}

	// �����e�[�u���t�@�C���I�[�v��
	if( eExe_Flag & ON_ATTR_FILE ) {
		if( !Open_Attr_Tbl () ) {
			return false;
		}
	}else {
		PrnMsg(stderr, NULL, "�o�͑����e�[�u���t�@�C����\'-t\'�Ŏw�肵�Ă�������\n");
		return false;
	}

	return		( true );

}

/** ------------------------------------------------------------------------------
// �g�p�@�\��
--------------------------------------------------------------------------------*/
void		main_rootin::
print_usage	(char **argv)
{
	PrnMsg(stdout, NULL,
				"=== �y�����������o�c�[���z ===\n"
				"<usage>\n"
				"%s <arg1> <arg2> <arg3> <arg4>\n"
				"\t<����>\n"
				"\t      <arg1>         : �`�F�b�N���p�������t�@�C��\n"
				"\t      <arg2>         : �`�F�b�N�����C����	(ex. TECHMAP.Building_mc)\n"
				"\t      <arg3>         : �`�F�b�N���p�������t�@�C��\n"
				"\t      <arg4>         : �`�F�b�N�惌�C����	(ex. SiNDY2004.Building)\n"
				"\t<�I�v�V����>\n"
				"\t      '-l'           : ���O�t�@�C�������w��\n"
				"\t      '-t'           : �o�͑������X�g�t�@�C�����w��\n"
				"\t      '-m'           : ���b�V���R�[�h���X�g�t�@�C�����w��\n"
				"\t      '-M'           : ���b�V���R�[�h���w��i1���A2���A8�����b�V���j\n"
				"\t      '-v'           : �o�[�W�������\��\n"
				"\t<�⑫>\n"
				"\t      �E'-m','-M',�͂ǂ��炩�K���w�肵�ĉ�����\n"
				"\t      �E'-m','-M'�͓����w��ł��܂���\n"
			, argv[0]);
}

/** ------------------------------------------------------------------------------
	�o�[�W�������̕\��
	@return		����
---------------------------------------------------------------------------------*/
void	main_rootin::
print_version	( void )
{
	PrnMsg(stdout, NULL,
				"=== �y�����Ή����X�g�쐬�c�[���z ===\n"
				"\t<�o�[�W�������>\n"
				"\t2004/05/14        1.00               : ���J�o�[�W����\n"
				"\t2014/02/04        2.1.0.3            : [bug 10159] [u][�v�]]14�H�d�l�ɑΉ����ė~����(Building_Diff_option) �ւ̑Ή�\n"
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
		if( e_cpMeshCode_File.size() == 0 ) {
			PrnMsg(e_fErr_Fp, stderr, "���b�V���R�[�h���X�g�t�@�C�������Ƃ肾���܂���ł���[check_exe]\n");
			return	( false );
		}	
		if( c_mMList.read_mesh_list ( (char*)e_cpMeshCode_File.c_str() ) != 0 ) {	// �����Ń��X�g��ǂݍ���
			PrnMsg(e_fErr_Fp, stderr, "���b�V���R�[�h���X�g�t�@�C��<%s>�̓ǂݍ��݂Ɏ��s[check_exe]\n", e_cpMeshCode_File.c_str() );
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
		PrnMsg(stdout, NULL, "<%s> ���C�����I�[�v��\n", a_cpMeshLayer);
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
				continue;
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
	���C���I�[�v��
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Layers		( 	IWorkspacePtr&	ipWorkspace_Src	)	// �`�F�b�N�����[�N�X�y�[�X[IN]
{

	if( !e_mChk_Src_WS.Init ( ipWorkspace_Src, (char*)e_cpCheck_Src_Layer.c_str() ) ) {
		PrnMsg(e_fErr_Fp, stderr, "�`�F�b�N�����C��<%s>�̓ǂݍ��݂Ɏ��s\n", e_cpCheck_Src_Layer );
		return	( false );
	}
	return	( true );
}

/**
	�ǂݍ��񂾃f�[�^���Z�b�g�ɗ��Ƃ�
	@return	�f�[�^��
*/
int		main_rootin::
Set_Data_Info (	IFeatureCursorPtr&	c_ipChk_Cursor,	///< �`�F�b�N���J�[�\��[IN]
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
			Feature_Attr   a_fObjID, a_fMeshCode, a_fPrim_ID, a_fAttr, a_fCORRIDOR_F, a_fNOWALL_F; // [Bug 10159]�Ή�
			a_fObjID       = a_fChk_Src_Attrs.Get_Attr ( "OBJECTID" );
			a_fMeshCode    = a_fChk_Src_Attrs.Get_Attr ( "TMP_MESHCODE" );
			a_fPrim_ID     = a_fChk_Src_Attrs.Get_Attr ( "TMP_PRIMID" );
			a_fAttr        = a_fChk_Src_Attrs.Get_Attr ( "BLDCLASS_C" );
			a_fCORRIDOR_F  = a_fChk_Src_Attrs.Get_Attr ( "CORRIDOR_F" );
			a_fNOWALL_F    = a_fChk_Src_Attrs.Get_Attr ( "NOWALL_F" );

			// [Bug 10159]�Ή�
			// ���O��ʂ̔���
			int ret = mAKAttrHandler.fnIsExcluded ( a_fAttr.e_ccvAttr.lVal, a_fNOWALL_F.e_ccvAttr.lVal, a_fCORRIDOR_F.e_ccvAttr.lVal );
			if( ret != 0 ) {
				continue;
			}
			// ���O�łȂ��ꍇ�́A�����l�̕ϊ�
			mAKAttrHandler.fnAttrConv ( a_fAttr.e_ccvAttr.lVal );

			a_dChk_Data.e_iOID		= a_fObjID.e_ccvAttr.intVal;
			a_dChk_Data.e_iMeshCode	= e_iMeshCode;			
			a_dChk_Data.e_iPrim_ID	= a_fPrim_ID.e_ccvAttr.intVal;
			a_dChk_Data.e_iAttr		= a_fAttr.e_ccvAttr.intVal;
			a_dChk_Data.e_ipGeo		= a_fChk_Src_Attrs.Ret_Geo();

			c_ipChk_Data.insert ( a_dChk_Data );
			a_iCount++;
		}
	}
	return	( a_iCount );
}

/** ------------------------------------------------------------------------------
	�Ή��e�[�u���̏o��
	@return	����
--------------------------------------------------------------------------------*/
void	main_rootin::
Print_Table		(	ms_data_info&		c_ipChk_Src_Data )		///< �`�F�b�N���f�[�^�̃Z�b�g[IN]
{
	FILE*	a_fpOutFp	= NULL;

	if( e_fpAttr_Fp == NULL ) {
		a_fpOutFp	= stdout;	// �w�肪�Ȃ��ꍇ�͕W���o�͂ɏo��
	}else {
		a_fpOutFp	= e_fpAttr_Fp;
	}

	// �f�[�^�̏o��
	ims_data_info	a_iSrc_Indx;
	for( a_iSrc_Indx = c_ipChk_Src_Data.begin(); a_iSrc_Indx != c_ipChk_Src_Data.end(); a_iSrc_Indx++ )
	{
		data_info	a_dCur_Dat	= *a_iSrc_Indx;
		fprintf	( a_fpOutFp, "%d\t%d\t%d\n", a_dCur_Dat.e_iMeshCode, a_dCur_Dat.e_iOID, a_dCur_Dat.e_iAttr );
	}
}

/** ------------------------------------------------------------------------------
	�������[�v
--------------------------------------------------------------------------------*/
void	main_rootin::
Exec_Roops ( IFeatureCursorPtr&	c_ipChk_Src_Cursor ) ///< �`�F�b�N���ƂȂ�f�[�^�̃J�[�\��[IN]
{
	// �s���E�̃��[�v
	int	i=0;
	IFeaturePtr		a_ipCity_Admin_Feat;
	int				a_iCityName_Count	= 0;
	int				a_iCityOffice_Count	= 0;

	ms_data_info	a_eChk_Src_Data;

	// �`�F�b�N���Ɛ�̃f�[�^���Z�b�g�ɂ���
	Set_Data_Info	( c_ipChk_Src_Cursor, a_eChk_Src_Data );
	// �Ή��e�[�u���̏o��
	Print_Table		( a_eChk_Src_Data );
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

	// �J�n���Ԃ̐ݒ�
	e_eExeTime.Set_Start();

	// �G���[���O�t�@�C���̃I�[�v��
	if( !Open_Err_Log () ) {
		return	( false );
	}

	// ���O�ɐݒ�����o��
	//--------------------------------------------------------------------------------------
	PrnMsg(e_fErr_Fp, stdout, "#�ڑ��v���p�e�B�F%s\n", m_sConnectStr );
	PrnMsg(e_fErr_Fp, stdout, "#�ڑ����C��    �F%s\n", e_cpCheck_Src_Layer.c_str() );
	if( eExe_Flag & ON_MESH ) {
		PrnMsg(e_fErr_Fp, stdout, "#���b�V�����X�g�F%s\n", e_cpMeshCode_File.c_str() );
	}else {
		PrnMsg(e_fErr_Fp, stdout, "#���b�V���R�[�h�F%d\n", e_iMeshCode );
	}
	PrnMsg(e_fErr_Fp, stdout, "#�o�̓t�@�C��  �F%s\n", e_cpAttr_Tbl_File.c_str() );
	//--------------------------------------------------------------------------------------

	// �ڑ�
	IWorkspacePtr	ipWorkspace_Src;

	if( !SiNDY_DB_Open	( ipWorkspace_Src )) {
		PrnMsg(e_fErr_Fp, stderr, "�ڑ��o���Ȃ�����\n");
		return	( false );
	}else {
		PrnMsg(stdout, NULL, "#�ڑ�����\n" );
	}

	// DB�ڑ��ɂ����������Ԃ̕\��
	PrnMsg(stdout, NULL, "SiNDY-DB�ڑ��������� = %0.2f(s)�ł���\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );

	// ���C���I�[�v��
	if( !Open_Layers ( ipWorkspace_Src ) ){	// �n�}�p���[�N�X�y�[�X
		PrnMsg(e_fErr_Fp, stderr, "���C���I�[�v���Ɏ��s���Ă��܂�\n");
		return	( false );
	}

	// ���b�V�����X�g�̓ǂݍ���
	mk_input_list		a_mMesh_List;
	if( !Read_Chk_List( ipWorkspace_Src, a_mMesh_List ) ) {
		PrnMsg(e_fErr_Fp, stderr, "���b�V�����X�g���擾�ł��܂���ł���\n");
		return	( false );	
	}

	ISpatialCacheManagerPtr		a_ipSpCacheMgr_Src( ipWorkspace_Src );

	// ���b�V���̃��[�v
	int					a_iCur_Mesh;
	bool bErrorStat = false;
	for( a_iCur_Mesh = a_mMesh_List.get_mesh_first(); a_iCur_Mesh != -1; a_iCur_Mesh = a_mMesh_List.get_mesh_next() )
	{
		PrnMsg(stdout, NULL, "#���b�V���R�[�h [%d] ������...", a_iCur_Mesh );
	
		e_eExeTime.Set_Lap_Start();

		IFeatureCursorPtr	a_ipChk_Src_Datas, a_ipChk_Tgt_Datas;
	
		// ���b�V����`�̎��o��
		IGeometryPtr	a_ipMeshRect;
		a_ipMeshRect	= get_mesh_rect( a_iCur_Mesh );
		if( a_ipMeshRect == NULL ) {	// ���b�V����`�������ꍇ
			PrnMsg(e_fErr_Fp, stderr, "���b�V���R�[�h<%d>�̋�`��������܂���ł���\n", a_iCur_Mesh );
			bErrorStat = true;
			continue;
		}
		// �N���X���Ŏg�p�ł���悤�ɂ��Ă���
		e_ipMeshRect	= a_ipMeshRect;
		e_iMeshCode		= a_iCur_Mesh;
		
		IEnvelopePtr	a_ipRect;
		a_ipMeshRect->get_Envelope ( &a_ipRect );

		// ���b�V����`�ŃT�[�`����
		a_ipChk_Src_Datas	= e_mChk_Src_WS.Search_Feature ( a_ipMeshRect, esriSpatialRelIntersects, "", RET_FCURSOR|RET_COUNT );
		if (a_ipChk_Src_Datas == NULL ) {
			PrnMsg(e_fErr_Fp, stderr, "<%d>���b�V����`�Ńf�[�^�����܂���\n", a_iCur_Mesh );
			bErrorStat = true;
			continue;
		}

		//-----------<�`�F�b�N�i�s�x�\��>--------------------
		long		a_lTCount	= 0;
		a_lTCount	= e_mChk_Src_WS.Get_Data_Count ();
		e_mChk_Src_WS.Clear_Data_Count();

		PrnMsg(stdout, NULL,	"%d �� ", a_lTCount);

		Exec_Roops ( a_ipChk_Src_Datas );

		DWORD			a_dMake_Finish	= 0;
		a_dMake_Finish	= timeGetTime();

		PrnMsg(stdout, NULL, "�������� = %0.2f(s)\n", e_eExeTime.Get_Lap() / 1000.0 );
	}

	//�@���s���I��
	// �I�����Ԃ̎擾
	PrnMsg(stdout, NULL, "�f�[�^�`�F�b�N������ = %0.2f(s)�ł���\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );

	if( bErrorStat == true ) {
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
		if( e_cpMeshCode_File.size() == 0 ) {
			PrnMsg(e_fErr_Fp, stderr, "���b�V���R�[�h���X�g�t�@�C�������Ƃ肾���܂���ł���\n");
			return	( false );
		}	
		if( c_aChk_List_Cls.read_mesh_list ( (char*)e_cpMeshCode_File.c_str() ) != 0 ) {	// �����Ń��X�g��ǂݍ���
			PrnMsg(e_fErr_Fp, stderr, "���b�V���R�[�h���X�g�t�@�C��<%s>�̓ǂݍ��݂Ɏ��s[check_exe]\n", e_cpMeshCode_File.c_str() );
			return	( false );
		}
	}else if( eExe_Flag & ON_MONO_MESH ) {	// �P���b�V���w��̏ꍇ
		if( c_aChk_List_Cls.read_mesh_list ( e_iMeshCode ) != 0 ) {	// �����Ń��X�g��ǂݍ���
			PrnMsg(e_fErr_Fp, stderr, "���b�V��<%d>�̐ݒ�Ɏ��s���܂���\n", e_iMeshCode );
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
