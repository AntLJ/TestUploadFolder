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

#include "main_rootin.h"

/**
	�G���[���O�t�@�C�����I�[�v������
	@return	true	�I�[�v������
	@return false	�I�[�v�����s
*/
bool	main_rootin::
Open_Err_Log	( void )
{
	if( eExe_Flag & ON_LOG_FILE ) {
	
		e_fErr_Fp	= fopen ( e_cpLog_File, "w" );
		if( e_fErr_Fp == NULL ) {
			fprintf	( stderr, "�G���[���O�t�@�C��<%s>���I�[�v���ł��Ȃ�[main_rootin]\n", e_cpLog_File );
			return	( false );
		}
		
	}else {

		return	( true );

	}
	return	( true );
}

#define	OPTION_MAX	6
#define	BMAX		512

bool	main_rootin::
arg_check	(	int		argc,		// ������
				char	**argv	)	// ������
{
	int		a_optcount[OPTION_MAX];	// �I�v�V�����̏d���̃J�E���g
	int		i;
	int		a_exeflag	= 0;
//	int		digit		= 0;	// ���b�V���̌���

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

//	char				*eMake_Type_Str		= NULL;

	e_sCheck_Layer		= argv[1];	///< �`�F�b�N���C��

	// �I�v�V�������Ȃɂ������Ƃ�
	if( argc == 2 ) {
		return	( true );
	}

	// �I�v�V����������A���̂Ƃ���g�p���Ȃ�
	for(i = 2; i < argc;)
	{
		if(strcmp(argv[i], "-l") == 0) { // ���O�t�@�C���ݒ�i�ݒ肪�����ꍇ�́A�G���[��stderr�֐��ꗬ���ɂȂ�j
			if(argc < i+2) return( false );
			e_cpLog_File	= argv[i+1];	///< ���b�V���R�[�h���X�g�t�@�C��
			a_exeflag		|= ON_LOG_FILE;
			a_optcount[0]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-m") == 0) {
			if(argc < i+2) return( false );
			e_cpMeshCode_File	= argv[i+1];	///< ���b�V���R�[�h���X�g�t�@�C��
			a_exeflag		|= ON_MESH;
			a_optcount[1]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-v") == 0) {
			//if(argc < i+1) return( false );
			print_version();
			a_optcount[2]++;
			i+=1;
			return	( false );	// ���ɂ�������H
		} else
		if(strcmp(argv[i], "-M") == 0) {
			if(argc < i+2) return( false );
			e_iMeshCode		= atoi(argv[i+1]);	///< ���b�V���R�[�h
			a_exeflag		|= ON_MONO_MESH;
			a_optcount[3]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-pgdb") == 0) {	/// 2007.05.21
			if(argc < i+2) return( false );
			e_cpPGDB_File	= argv[i+1];
			a_exeflag		|= ON_PGDB_MODE;
			a_optcount[4]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-i") == 0) {	///< 2007.10.25
			if(argc < i+2) return( false );
			e_cpInit_File	= argv[i+1];	///< �������t�@�C��
			a_exeflag		|= ON_INIT_FILE;
			a_optcount[5]++;
			i+=2;
		} else{
			fprintf	(stderr, "�I�v�V���� %s �͊Ԉ���Ă܂�!!\n", argv[i]);
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
	if( a_exeflag & ON_MONO_MESH && a_exeflag & ON_MESH ) {
		fprintf	(stderr, "�I�v�V����'-m'��'-M'�͓����Ɏw��ł��܂���!!\n");
		return	( false );	
	}
	eExe_Flag	= a_exeflag;
//	return		( a_exeflag );
	return		( true );

}

void		main_rootin::
print_usage	(char **argv)
{
	fprintf(	stdout,
				"=== �y�w�i�X�P�[���Ԑ����`�F�b�N�z===\n"
				"<usage>\n"
				"%s <arg1>\n"
				"\t<����>\n"
				"\t      <arg1>             : �`�F�b�N���C����(ex. SiNDY.City_Site)\n"
				"\t<�I�v�V����>\n"
				"\t      '-i'               : �ݒ�t�@�C����\n"
				"\t      '-l'               : ���O�t�@�C�������w��\n"
				"\t      '-m'               : ���b�V���R�[�h���X�g�t�@�C�����w��\n"
				"\t      '-M'               : ���b�V���R�[�h\n"
				"\t      '-pgdb'            : PGDB�t�@�C����(ex, -pgdb C:\\test\\�e�X�g.mdb)\n"
				"\t      '-v'               : �o�[�W�������\��\n"

			//	"\t<�f�[�^�쐬�^�C�v>\n"
			//	"\t      'note'             : ���L�f�[�^���쐬\n"
			//	"\t   ���f�[�^�쐬�^�C�v�ƃ��b�V���R�[�h���X�g���̃��b�V���R�[�h\n"
			//	"\t     �̌�������A�쐬����f�[�^�����肵�Ă���B\n"
			//	"\t     �g���q���ɂ��ẮA�����Ŏ������ʂ��Ă���B\n"
			//	"\t<���ϐ�>\n"
			//	"\t      DBMS_ATTR_PATH		: ������`�t�@�C���̂���f�B���N�g����\n"
			//	"\t      DBMS_ATTR	    	: ������`�t�@�C����\n"
			//	"\t<����>\n"
			//	"\t      �n�ʃ|���S���ƁA�s���E�f�[�^�̘_���ς��Ƃ�A�n�ʌ`�̍s���E�|���S�����쐬����\n"
			, argv[0]);
}

/**
	�o�[�W�������̕\��
	@return		����
*/
void	main_rootin::
print_version	( void )
{
	fprintf(	stdout,
				"=== �y�w�i�X�P�[���Ԑ����`�F�b�N�z ===\n"
				"\t<�o�[�W�������>\n"
				"\t2006/09/08	1.0.0.1		�E�V�K�쐬\n"
				"\t2007/05/21	1.0.1.2		�E�X�P�[����ʂɖ����������������Ă���ꍇ�A�S�ẴX�P�[���̎�ʂ��u1�v�̏ꍇ��\n"
				"\t							�@�G���[�Ƃ��Č��o����Ă��Ȃ����������C��\n"
				"\t							�EPGDB�̃`�F�b�N�ɑΉ�('-pgdb'�I�v�V�����̒ǉ�)\n"
				"\t2007/09/27   1.1.0.3		�EVC8�Ή�\n"
				"\t                    		�E���b�V���R�[�h�t�B�[���h���ύX�ւ̑Ή�\n"
				"\t2007/10/26   1.2.0.5		�E[Bug 5633]�ւ̑Ή��i���ϐ�����̐ݒ�ɑΉ��j\n"
				"\t                    		�E�ݒ�t�@�C���ł̐ݒ���I�v�V�����ł̐ݒ�ɕύX('-i'�I�v�V����)\n"
				"\t                         �E���ϐ����̓��ꉻ\n"

			);
}

/**
	���b�V����`��DB����l�����郁�\�b�h�̏�����
	��get_mesh_rect �����O�ɕK���Ă�
*/
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

/**
	���b�V����`��GeoDB����擾���� \
	��GeoDB�ւ̐ڑ����m������Ă��邱�Ƃ��O��
*/
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
	if(a_iDigit == 2) {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// ���b�V���R�[�h�𕶎����
		eDigit	= 2;
	}else
	if(a_iDigit == 4) {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// ���b�V���R�[�h�𕶎���� 2007.09.03�C��
		eDigit	= 4;
	}else
	if(a_iDigit == 6) {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// ���b�V���R�[�h�𕶎����
		eDigit	= 6;
	}else {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);		// ���b�V���R�[�h�𕶎���� 2007.09.03�C��
		eDigit	= 8;
	}

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
		fprintf	(stderr, "�f�[�^�̑��������Ɏ��s\n");
		return	(NULL);
	}else {
		IFeaturePtr				a_Feature;
//		long					a_lSp_InitFlag	= 0;	// ��ԎQ�Ƃ��������������ǂ����̃t���O

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


/**
	���b�V�����X�g��GeoDB�ォ��擾����
	@return	bool
*/
bool	main_rootin::
get_mesh_list	( long	c_lDigit )	// ���b�V���R�[�h�̌���
{

	CComBSTR	a_SpatialCol;
	e_ipF_Class->get_ShapeFieldName	( &a_SpatialCol );
	BSTR		a_Where_Str;
	char		a_tmpstr[60];	//�@�����͂Ȃ�Ƃ��������C������
	BSTR		a_bMesh_Str;	// �T�[�`�p���b�V��������

//	crd_cnv		crd;
	int			a_iDigit	= 0;
	a_iDigit	= c_lDigit;
	// ���k�ڂ͂������b�V���R�[�h�����邾���Ȃ̂ŁA���݂��ǂ��Ƃ��͂��܂�Ӗ����Ȃ��H
	if(a_iDigit == 2) {
		a_bMesh_Str	= T2BSTR(_T("MESHCODE"));
		sprintf		(a_tmpstr, "MESHCODE <> 0");		// ���b�V���R�[�h�𕶎����
		eDigit	= 2;
	}else
	if(a_iDigit == 4) {
		a_bMesh_Str	= T2BSTR(_T("MESHCODE"));
		sprintf		(a_tmpstr, "MESHCODE <> 0");		// ���b�V���R�[�h�𕶎����
		eDigit		= 4;
	}else
	if(a_iDigit == 6) {
		a_bMesh_Str	= T2BSTR(_T("MESHCODE"));
		sprintf		(a_tmpstr, "MESHCODE <> 0");		// ���b�V���R�[�h�𕶎����
		eDigit		= 6;
	}else {		// �s�s�n�}���b�V���̏ꍇ
		// ���b�V���R�[�h�𕶎����
		a_bMesh_Str	= T2BSTR(_T("MESHCODE"));
		sprintf		(a_tmpstr, "COMPLETE_F = 1 or COMPLETE_F = 0" );
		eDigit		= 8;
	}

	a_Where_Str	= T2BSTR(_T(a_tmpstr));		// string��BSTR�ɕϊ�

	IQueryFilterPtr		a_Sp_Filter	( CLSID_QueryFilter );			// ��Ԍ����t�B���^
//	a_Sp_Filter->put_SearchOrder	( esriSearchOrderAttribute );	// ���������D��
	a_Sp_Filter->put_WhereClause	( a_Where_Str);					// Where��̐ݒ�
	
	// �N���b�v��`�쐬
	IEnvelopePtr	a_ipMesh_Rect;	// ���b�V����`
	a_ipMesh_Rect	= NULL;

	// ���b�V���R�[�h�̃t�B�[���h���擾����
	IFieldsPtr	a_ipFields;
	LONG		a_lFieldCount;

	// �����R�s�[
	if ( FAILED (e_ipF_Class->FindField ( a_bMesh_Str, &a_lFieldCount ) ) ) {
		fprintf	(stderr, "�f�[�^�̑��������Ɏ��s\n");
		return	(false);
	}

	// �T�[�`���Ă���
	IFeatureCursorPtr	a_F_Cursor;
	if( FAILED	(e_ipF_Class->Search	( a_Sp_Filter, VARIANT_FALSE, &a_F_Cursor )) ) {
		fprintf	(stderr, "�f�[�^�̑��������Ɏ��s\n");
		return	(false);
	}else {
		IFeaturePtr				a_Feature;

		for(;;)
		{
			if( a_F_Cursor->NextFeature ( &a_Feature ) != S_OK) {	// �f�[�^���ЂƂ��o��
				break;
			}
			
			CComVariant	a_cCur_Value;		// �擾���鑮���l
			
			a_Feature->get_Value ( a_lFieldCount, &a_cCur_Value);

			int	a_iCur_Mesh	= 0;
			a_iCur_Mesh		= a_cCur_Value.intVal;

			e_sExi_Mesh_List.insert ( a_iCur_Mesh );	
		}
	}
	return	( true );

}

/**
	�w�胁�b�V�������݂��邩�̃`�F�b�N
	@return	bool
*/
bool	main_rootin::
chk_mesh_exist	( int	c_iMeshCode	)		///< ���b�V���R�[�h
{
	set<int, less<int> >::iterator	a_isIndx;

	a_isIndx	= e_sExi_Mesh_List.find( c_iMeshCode );
	if( a_isIndx != e_sExi_Mesh_List.end() ) {
		return	( true );
	}

	return	( false );
}

/**
	�X�P�[����ʂ̔���
	@return	0		�X�P�[����ʂłȂ�
	@return	IS_SCCLS(1)	�X�P�[�����
	@return	IS_SCFLG(2)	�X�P�[���t���O
*/
int	main_rootin::
Scale_Attr_Chk	( string&	c_sFieldName,	///< �t�B�[���h��
				  int*		c_ipScale )		///< �Y���X�P�[��
{
// �X�P�[���ʂ̒l�̑Ó����̂��߂ɃX�P�[���ʑ�����ǉ�
	if( c_sFieldName.find ( "SC" ) == -1 ) {
		return	( 0 );
	}else {
		if( c_sFieldName == "SC1BGCLASS_C" ) {
			*c_ipScale	= 1;
			return	( IS_SCCLS );
		}else
		if( c_sFieldName == "SC2BGCLASS_C" ) {
			*c_ipScale	= 2;
			return	( IS_SCCLS );
		}else
		if( c_sFieldName == "SC3BGCLASS_C" ) {
			*c_ipScale	= 3;
			return	( IS_SCCLS );
		}else
		if( c_sFieldName == "SC4BGCLASS_C" ) {
			*c_ipScale	= 4;
			return	( IS_SCCLS );
		}else
		if( c_sFieldName == "DISPSCALE1_F" ) {
			*c_ipScale	= 1;
			return	( IS_SCFLG );
		}else
		if( c_sFieldName == "DISPSCALE2_F" ) {
			*c_ipScale	= 2;
			return	( IS_SCFLG );
		}else
		if( c_sFieldName == "DISPSCALE3_F" ) {
			*c_ipScale	= 3;
			return	( IS_SCFLG );
		}else
		if( c_sFieldName == "DISPSCALE4_F" ) {
			*c_ipScale	= 4;
			return	( IS_SCFLG );
		}
	}
	return ( 0 );
}

/**
	�X�P�[����ʂ̐������̔���
*/
int	main_rootin::
Chk_Scale_Class_Adjust ( int*	c_ipScaleAttr )
{
	// �X�P�[���P�A�X�P�[���Q�A�X�P�[���R�A�X�P�[���S�����L�̊֌W�ȊO�͂���
	// 1 1 1 A	��\�� ��\�� ��\�� ����A
	if( c_ipScaleAttr[0] == 1 &&
		c_ipScaleAttr[1] == 1 &&
		c_ipScaleAttr[2] == 1 &&
		c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// 1 1 A A	��\�� ��\�� ����A ����A
	if( c_ipScaleAttr[0] == 1 &&
		c_ipScaleAttr[1] == 1 &&
		(c_ipScaleAttr[2] == c_ipScaleAttr[3]) &&
		c_ipScaleAttr[2] > 1 ) {
		return	( true );
	}else
	// 1 1 B A	��\�� ��\�� ����B ����A
	if( c_ipScaleAttr[0] == 1 &&
		c_ipScaleAttr[1] == 1 &&
		(c_ipScaleAttr[2] != c_ipScaleAttr[3]) &&
		c_ipScaleAttr[2] > 1 && c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// 1 A A A	��\�� ����A ����A ����A
	if( c_ipScaleAttr[0] == 1 &&
		(c_ipScaleAttr[1] == c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] == c_ipScaleAttr[3]) &&
		c_ipScaleAttr[1] > 1 ) {
		return	( true );
	}else
	// 1 B A A	��\�� ����B ����A ����A
	if( c_ipScaleAttr[0] == 1 &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] == c_ipScaleAttr[3]) &&
		c_ipScaleAttr[1] > 1 && c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// 1 B B A	��\�� ����B ����B ����A
	if( c_ipScaleAttr[0] == 1 &&
		(c_ipScaleAttr[1] == c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] != c_ipScaleAttr[3]) &&
		c_ipScaleAttr[1] > 1 && c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// 1 C B A	��\�� ����C ����B ����A
	if( c_ipScaleAttr[0] == 1 &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] != c_ipScaleAttr[3]) &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[3]) &&
		c_ipScaleAttr[1] > 1 && c_ipScaleAttr[2] > 1 && c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// A A A A	����A ����A ����A ����A
	if( (c_ipScaleAttr[0] == c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[1] == c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] == c_ipScaleAttr[3]) && c_ipScaleAttr[0] > 1 ) {
		return	( true );
	}else
	// B A A A	����B ����A ����A ����A
	if( (c_ipScaleAttr[0] != c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[1] == c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] == c_ipScaleAttr[3]) &&
		c_ipScaleAttr[0] > 1 && c_ipScaleAttr[1] > 1 ) {
		return	( true );
	}else
	// B B A A	����B ����B ����A ����A
	if( (c_ipScaleAttr[0] == c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] == c_ipScaleAttr[3]) &&
		c_ipScaleAttr[0] > 1 && c_ipScaleAttr[2] > 1 ) {
		return	( true );
	}else
	// B B B A	����B ����B ����B ����A
	if( (c_ipScaleAttr[0] == c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[1] == c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] != c_ipScaleAttr[3]) &&
		c_ipScaleAttr[0] > 1 && c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// C B A A	����C ����B ����A ����A
	if( (c_ipScaleAttr[0] != c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[0] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] == c_ipScaleAttr[3]) &&
		c_ipScaleAttr[0] > 1 && c_ipScaleAttr[1] > 1 &&c_ipScaleAttr[2] > 1 ) {
		return	( true );
	}else
	// C B B A	����C ����B ����B ����A
	if( (c_ipScaleAttr[0] != c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[1] == c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] != c_ipScaleAttr[3]) &&
		(c_ipScaleAttr[0] != c_ipScaleAttr[3]) &&
		c_ipScaleAttr[0] > 1 && c_ipScaleAttr[1] > 1 &&c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// C C B A	����C ����C ����B ����A
	if( (c_ipScaleAttr[0] == c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] != c_ipScaleAttr[3]) &&
		(c_ipScaleAttr[0] != c_ipScaleAttr[3]) &&
		c_ipScaleAttr[0] > 1 && c_ipScaleAttr[2] > 1 &&c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// D C B A	����D ����C ����B ����A
	if( (c_ipScaleAttr[0] != c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[0] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[0] != c_ipScaleAttr[3]) &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[3]) &&
		(c_ipScaleAttr[2] != c_ipScaleAttr[3]) &&
		c_ipScaleAttr[0] > 1 && c_ipScaleAttr[1] > 1 && c_ipScaleAttr[2] > 1 && c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else{
		return ( false );
	}
}

/**
	�X�P�[���t���O�̐������̔���
*/
int	main_rootin::
Chk_Scale_Flag_Adjust ( int*	c_ipScaleAttr )
{
	// �X�P�[���P�A�X�P�[���Q�A�X�P�[���R�A�X�P�[���S�����L�̊֌W�ȊO�͂���
	// 0 0 0 1	��\�� ��\�� ��\�� �\��
	// 0 0 1 1	��\�� ��\�� �\�� �\��
	// 0 1 1 1	��\�� �\�� �\�� �\��
	// 1 1 1 1	�\�� �\�� �\�� �\��
	if( c_ipScaleAttr[0] == 0 && c_ipScaleAttr[1] == 0 && c_ipScaleAttr[2] == 0 && c_ipScaleAttr[3] == 1 ) {
		return	( true );
	}else
	if( c_ipScaleAttr[0] == 0 && c_ipScaleAttr[1] == 0 && c_ipScaleAttr[2] == 1 && c_ipScaleAttr[3] == 1 ) {
		return	( true );
	}else
	if( c_ipScaleAttr[0] == 0 && c_ipScaleAttr[1] == 1 && c_ipScaleAttr[2] == 1 && c_ipScaleAttr[3] == 1 ) {
		return	( true );
	}else
	if( c_ipScaleAttr[0] == 1 && c_ipScaleAttr[1] == 1 && c_ipScaleAttr[2] == 1 && c_ipScaleAttr[3] == 1 ) {
		return	( true );
	}
	return ( false );
}

/** 2007.06.19 �ǉ�
	DB�ւ̐ڑ��I�[�v��
	@return	true	����
	@return	false	���s
*/
bool	main_rootin::
DB_Open	(	IWorkspaceFactoryPtr&	c_ipWorkspaceFactory,
			IPropertySetPtr&		c_ipPropertySet,
			IWorkspacePtr&			c_ipWorkspace	)
{
	USES_CONVERSION;	// OLE2T ���̈ꕔ�̃}�N�����g�p����ꍇ�ɂ͕K���K�v

	/// �ڑ��p�v���p�e�B�ݒ�
	CComVariant		vaUser		(_T(e_pTool_Init.get_UserName()) );
	CComVariant		vaPass		(_T(e_pTool_Init.get_Password()) );
	CComVariant		vaInstance	(_T(e_pTool_Init.get_Instance()) );
	CComVariant		vaServer	(_T(e_pTool_Init.get_Server()) );
	CComVariant		vaVersion	(_T(e_pTool_Init.get_Version()) );

	_bstr_t	a_bUSER, a_bINSTANCE, a_bSERVER, a_bPASSWORD, a_bVERSION;	// T2BSTR����_bstr_t�ɕύX 2007.06.18

	a_bUSER		= "USER";
	a_bINSTANCE	= "INSTANCE";
	a_bSERVER	= "SERVER";
	a_bPASSWORD	= "PASSWORD";
	a_bVERSION	= "VERSION";

	c_ipPropertySet->SetProperty( a_bUSER,		vaUser );
	c_ipPropertySet->SetProperty( a_bINSTANCE,	vaInstance );
	c_ipPropertySet->SetProperty( a_bSERVER,	vaServer );
	c_ipPropertySet->SetProperty( a_bPASSWORD,	vaPass );
	c_ipPropertySet->SetProperty( a_bVERSION,	vaVersion );

	printf(_T(	"�ڑ��v���p�e�B�F%s/%s/%s/%s/%s\n"),
				OLE2T( vaUser.bstrVal ), 
				OLE2T( vaPass.bstrVal ),
				OLE2T( vaVersion.bstrVal ),
				OLE2T( vaInstance.bstrVal ),
				OLE2T( vaServer.bstrVal ) );

	// DB�֐ڑ�
	printf(_T("�c�a�ڑ���..."));
	if( c_ipWorkspaceFactory->Open( c_ipPropertySet, NULL, &c_ipWorkspace ) != S_OK ) {
		return	( false );
	}else {
		return	( true );
	}
}

/**
	�`�F�b�N���s��
	�������ŃG���[�t�@�C���̏��������s��
	@return	����	true
	@return ���s	false
*/
bool	main_rootin::
check_exe	( void )
{
//#ifdef	_DEBUG
	// �J�n���Ԃ̎擾
	DWORD			a_dBegin_Time	= 0;
	a_dBegin_Time	= timeGetTime();
//#endif

	HRESULT		hr;

	// �������t�@�C���Ǎ���
	if( eExe_Flag & ON_INIT_FILE ) {
		if( e_pTool_Init.read_init_file ( Ret_Init_File() ) != 0 ) {
			return	( false );
		}
	} else {
		// ���ϐ�����ǂݍ���
		if( e_pTool_Init.read_init_env () != 0 ) {
			return	( false );		
		}
	}

	IWorkspacePtr			ipWorkspace_Any;

	// DB�I�[�v������
	if( eExe_Flag & ON_PGDB_MODE ){
		// �p�[�\�i���W�IDB�p�ł�
		IWorkspaceFactoryPtr	ipWorkspaceFactory_Any	( CLSID_AccessWorkspaceFactory );
		IPropertySetPtr			ipPropertySet_Any		( CLSID_PropertySet );					// �ڑ��v���p�e�B�[�i�[�p
		USES_CONVERSION;	// OLE2T ���̈ꕔ�̃}�N�����g�p����ꍇ�ɂ͕K���K�v
		// �J�����g�����ɃI�[�v�����Ă݂�
		ipWorkspaceFactory_Any->OpenFromFile( T2BSTR(_T(e_cpPGDB_File) ), 0, &ipWorkspace_Any );
		if( ipWorkspace_Any == NULL )	{
			fprintf ( stderr, "�p�[�\�i���W�IDB���I�[�v���ł��܂���ł���\n"); 
			return	( false );
		}else {
			printf(_T("�ڑ�����"));
		}
		printf(_T("\n"));
	}else {

		// �s�s�n�}�n�p�ڑ�
		IWorkspaceFactoryPtr	ipWorkspaceFactory_Any( CLSID_SdeWorkspaceFactory );	// 
		IPropertySetPtr			ipPropertySet_Any( CLSID_PropertySet );					// �ڑ��v���p�e�B�[�i�[�p

		// DB�ւ̐ڑ�
		if( !DB_Open	(	ipWorkspaceFactory_Any, ipPropertySet_Any, ipWorkspace_Any	) ) {
			fprintf	(stderr, "�c�a�ɐڑ��o���Ȃ�����\n");
			return	( false );
		}else {
			printf(_T("�c�a�ڑ�����"));
		}
		ATLASSERT( ipWorkspace_Any != NULL );
	}

//#ifdef	_DEBUG
	// DB�ڑ��ɂ����������Ԃ̕\��
	DWORD				a_dConnect_Time		= 0;
	a_dConnect_Time		= timeGetTime();

	fprintf	( stdout,	"SiNDY-DB�ڑ��������� = %0.2f(s)�ł���\n",
						(double)((a_dConnect_Time - a_dBegin_Time) / 1000.0) );
//#endif

	// �G���[���O�t�@�C���̃I�[�v��
	if( !Open_Err_Log () ) {
		return	( false );
	}else {
		if( !e_sErr_Log.Init ( Log_Fp() ) ) {
			return ( false );
		}
	}

	mk_input_list	a_mChk_List;
	char*			a_cpList_File	= NULL;
	int				a_iMDigit		= 0;

	if( eExe_Flag & ON_MESH ||		//	���b�V���R�[�h���X�g�w��̏ꍇ
		eExe_Flag & ON_MONO_MESH ) {	//	���b�V���R�[�h�w��̏ꍇ

		if( eExe_Flag & ON_MESH ) {
			a_cpList_File	= Ret_Mesh_File ( );
			if( a_cpList_File == NULL ) {
				fprintf	( stderr, "���b�V���R�[�h���X�g�t�@�C�������Ƃ肾���܂���ł���[check_exe]\n");
				return	( false );
			}	
			if( a_mChk_List.read_mesh_list ( a_cpList_File ) != 0 ) {	// �����Ń��X�g��ǂݍ���
				fprintf	( stderr, "���b�V���R�[�h���X�g�t�@�C��<%s>�̓ǂݍ��݂Ɏ��s[check_exe]\n", a_cpList_File );
				return	( false );
			}
		}else {
			if( a_mChk_List.read_mesh_list ( e_iMeshCode ) != 0 ) {	// �����Ń��X�g��ǂݍ���
				fprintf	( stderr, "���b�V���R�[�h<%s>�̓ǂݍ��݂Ɏ��s[check_exe]\n", a_cpList_File );
				return	( false );
			}
		}
	
		char*		a_cpMeshLayer	= NULL;
		a_iMDigit	= a_mChk_List.get_mesh_digit ();
		
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
		case	2:	// 2004.02.24
			a_cpMeshLayer	= e_pTool_Init.get_Top_Mesh ();
			break;
		default:
			fprintf	( stderr, "���X�g�̃��b�V�������ɂ͑Ή����Ă��܂���[check_exe]\n");
			return	( false );
		}

		if( !get_mesh_init( a_cpMeshLayer , ipWorkspace_Any ) ) {
			fprintf	( stderr, "���b�V�����C���̊l���̏������Ɏ��s[check_exe]\n");
			return	( false );
		}

		if( !get_mesh_list ( a_iMDigit ) ) {
			fprintf	( stderr, "���b�V�����X�g���쐬�ł��܂���ł���[check_exe]\n");
			return	( false );
		}
	}else {
		fprintf	( stderr, "���b�V�����X�g'-m'�܂��̓��b�V���R�[�h'-M'���w�肵�Ă�������\n");
		return	( false );
	}

	// �`�F�b�N���郌�C�����̊l��
	char*			a_cpCheck_Layer	= NULL;
	a_cpCheck_Layer	= Ret_Check_Layer();
	if( a_cpCheck_Layer == NULL ) {
		fprintf	( stderr, "�`�F�b�N�Ώۂ̃|�C���g�f�[�^���C�������Ƃ肾���܂���ł���\n");
		return	( false );
	}

	// �`�F�b�N���C���̓ǂݍ���
	msi_handle_ws	a_mChk_WS;
	fprintf	( stderr, "<%s>���C���̃`�F�b�N���s���܂�\n", a_cpCheck_Layer);
	if( !a_mChk_WS.Init ( ipWorkspace_Any, a_cpCheck_Layer) ) {
		fprintf	( stderr, "�`�F�b�N���C��<%s>�̓ǂݍ��݂Ɏ��s[check_exe]\n", a_cpCheck_Layer );
		return	( false );		
	}

	// �L���b�V����ɂƂ��Ă���
	// �I���������e�X�g 2002.6.24
	ISpatialCacheManagerPtr		a_ipSpCacheMgr_Any( ipWorkspace_Any );

	set<checked_id>				a_sChecked_OID_Set;
	set<checked_id>::iterator	a_sChecked_OID_Indx;

	if( eExe_Flag & ON_MESH	||
		eExe_Flag & ON_MONO_MESH	) {	// ���b�V���R�[�h���X�g��ǂݍ���ł̃`�F�b�N�̏ꍇ

		int		a_iCur_Mesh;
		for( a_iCur_Mesh = a_mChk_List.get_mesh_first(); a_iCur_Mesh != -1; a_iCur_Mesh = a_mChk_List.get_mesh_next() )
		{

			/// �f�[�^�쐬�J�n����
			DWORD				a_dMesh_Begin_Time	= 0;
			a_dMesh_Begin_Time	= timeGetTime();

			e_sErr_Log.Set_Mesh			( a_iCur_Mesh );	// ���b�V���R�[�h�̐ݒ�
			e_sErr_Log.Set_Layer_Name	( a_cpCheck_Layer ); // ���C�����̂̐ݒ�
			e_sErr_Log.Set_Err_Level	( 0 );				// �G���[���x���̐ݒ�

			// ���b�V�����ƂɃL���b�V���}�l�[�W�����N���A���Ă���
			hr	= a_ipSpCacheMgr_Any->EmptyCache( );
			if( FAILED ( hr ) ) {
				fprintf	( stderr, "�L���b�V���}�l�[�W�����N���A�ł��܂���ł���\n");
				continue;
			}

			fprintf	( stdout, "���b�V���R�[�h [%d] �`�F�b�N��\n", a_iCur_Mesh );

			IFeatureCursorPtr	a_ipDatas;
		
			// ���b�V����`�̎��o��
			IEnvelopePtr	a_ipRect;
			a_ipRect		= get_mesh_rect( a_iCur_Mesh );
			if( a_ipRect == NULL ) {	// ���b�V����`�������ꍇ
				fprintf	( stderr, "���b�V���R�[�h<%d>�̋�`��������܂���ł���\n", a_iCur_Mesh );
				continue;
			}
		
			// ��ԃL���b�V���}�l�[�W���Ƀ��b�V�����̃f�[�^���Ƃ肾��
//			a_ipSpCacheMgr_Any->FillCache( a_ipRect );

			// ���b�V����`�̋��E���ŃT�[�`����
			a_ipDatas	= a_mChk_WS.Search_Feature	( a_ipRect, esriSpatialRelIntersects, "", RET_COUNT|RET_FCURSOR );	// ���b�V����`���C���ƃ|�C���g�̌���������

			if (a_ipDatas == NULL ) {
				fprintf	( stderr, "<%d>���b�V����`�Ńf�[�^�����܂���\n", a_iCur_Mesh );
				continue;
			}

			//-----------<�`�F�b�N�i�s�x�\��>--------------------
			long		i			= 0;
			long		a_lTCount	= 0;

			a_lTCount	= a_mChk_WS.Get_Data_Count ();

			fprintf	( stdout,	"���b�V���R�[�h [%d] ���f�[�^���� %d ��\n", a_iCur_Mesh, a_lTCount );

			//--------------------------------------------------

			IFeaturePtr		a_ipCurFeat;
			ms_geometry_inc	a_msGeometry_MSet;
			char	a_cMsgBuf[512];
			// �f�[�^���x�N�^�ɂ����Ă���
			for ( ; ; i++)
			{
				geometry_inc	a_gGeoInc;
				//�@���b�V���ň����|���Ă����t�B�[�`��
				if( a_ipDatas->NextFeature ( &a_ipCurFeat ) == S_OK) {

					// �I�u�W�F�N�gID�̏d���`�F�b�N�̃`�F�b�N
			/*		a_sOID_Indx	= a_sOID_Set.find ( (int)a_lOid );
					if( a_sOID_Indx != a_sOID_Set.end() ) {
						continue;	// ��x�`�F�b�N�����z�͂��Ȃ�
					}else {
						// �`�F�b�N����ID��o�^
						a_sOID_Set.insert ( (int)a_lOid );
					}*/

					Feature_All_Attrs	a_fCurAttrs;

					a_fCurAttrs.Init ( a_ipCurFeat );	// �����Ă��鑮���݂�Ȏ��o��

					long		a_lOid		= 0;
					long		a_lSrc_Oid	= 0;	// DB��ł�OID
					string		a_sPrnMsg	= "";
					long		a_lAttrNum	= 0;

					a_lOid		= a_fCurAttrs.Get_Obj_ID ();
					a_lAttrNum	= a_fCurAttrs.Get_Num();
					e_sErr_Log.Set_OID	( (int)a_lOid );

					int	a_iScaleChk_F	= 0;	// �X�P�[���Ó����`�F�b�N�����邩�̃t���O
					int	a_iRet	= 0;
					int	a_iScaleAttr[4]	= {0,0,0,0};

					int j = 0;
					for( j = 0; j < a_lAttrNum; j++ )
					{

						Feature_Attr	a_fCAtt;
						// �L�[�t�B�[���h�̏������o��
						a_fCAtt			= a_fCurAttrs.Get_Attr ( j );

						string	a_sFld_Name	= a_fCAtt.e_bFld_Name;
						// �啶����
						int	c = 0;
						while(a_sFld_Name[c] != '\0' ){
							a_sFld_Name[c]	= (char)toupper(a_sFld_Name[c]);
							c++;
						}

						if( a_sFld_Name == "FID" ||
							a_sFld_Name == "OBJECTID" ||
							a_sFld_Name == "SHAPE" ) {
							if( a_sFld_Name == "OBJECTID" ) {
								a_lSrc_Oid	= a_fCAtt.e_ccvAttr.intVal;
							}
							continue;
						}
						int	a_iAttrVal	= 0;
						
						int	a_iScale= 0;
						// �X�P�[���֌W��ʂ̎��o��
						a_iRet	= Scale_Attr_Chk ( a_fCAtt.e_bFld_Name, &a_iScale );
						if( a_iRet != 0 ) {
							if( a_iScaleChk_F == 0 ) {
								a_iScaleChk_F				= a_iRet;
							}
							a_iAttrVal					= a_fCAtt.e_ccvAttr.intVal;
							a_iScaleAttr[a_iScale-1]	= a_iAttrVal;	// �X�P�[���ʂ̑���������Ă���
						}else {
							continue;
						}
					}

					if ( a_iScaleChk_F == IS_SCFLG ) {	// �\���t���O�`���̏ꍇ
						if( !Chk_Scale_Flag_Adjust ( a_iScaleAttr ) ) {
							sprintf( a_cMsgBuf, "�X�P�[���Ԃ̐�������������\t�y%d %d %d %d�z",
									a_iScaleAttr[0], a_iScaleAttr[1], a_iScaleAttr[2], a_iScaleAttr[3]);
							e_sErr_Log.Set_Err_Msg	( a_cMsgBuf );	// �G���[���b�Z�[�W�̐ݒ�
							e_sErr_Log.Prn_SiNDY_Err( 0, NULL, stderr  );
						}
					}else {	// �X�P�[���ʎ�ʂ̏ꍇ
						if( !Chk_Scale_Class_Adjust ( a_iScaleAttr ) ) {
							sprintf( a_cMsgBuf, "�X�P�[���Ԃ̐�������������\t�y%d %d %d %d�z",
									a_iScaleAttr[0], a_iScaleAttr[1], a_iScaleAttr[2], a_iScaleAttr[3]);
							e_sErr_Log.Set_Err_Msg	( a_cMsgBuf );	// �G���[���b�Z�[�W�̐ݒ�
							e_sErr_Log.Prn_SiNDY_Err( 0, NULL, stderr  );
						}
					}
		/*			//----------------------------------------------------------
					//	if( a_sChecked_OID_Set.size() > 10000000 ) {
						if( a_sChecked_OID_Set.size() > 1000000 ) {
							a_sChecked_OID_Set.clear();
						}
						a_sChecked_OID_Indx	= a_sChecked_OID_Set.find( a_cChk_Test );
						if( a_sChecked_OID_Indx != a_sChecked_OID_Set.end() ){
							continue;	// �`�F�b�N�ς݂Ȃ̂łƂ΂�
						}else {
							a_sChecked_OID_Indx	= a_sChecked_OID_Set.find( a_cChk_Test_Rev );
							if( a_sChecked_OID_Indx != a_sChecked_OID_Set.end() ){
								continue;	// �`�F�b�N�ς݂Ȃ̂łƂ΂�
							}
							// �݂���Ȃ������ꍇ�͐V�K�o�^
							a_sChecked_OID_Set.insert ( a_cChk_Test );
						}
					}*/
					//----------------
					if( i > 0 && (i % 100) == 0) {	// 
						// �J�E���g100�����ƂɃ��b�Z�[�W��\��
						fprintf( stdout,	"���b�V���R�[�h [%d]  %d �����@%d �� �`�F�b�N�I�� \n",
											a_iCur_Mesh,
											a_lTCount,
											i	);
					}
					//----------------

				}else {
					break;
				}
			}

			DWORD			a_dMake_Finish	= 0;
			a_dMake_Finish	= timeGetTime();

			fprintf	( stdout,	"<%d> :  �f�[�^�`�F�b�N���� = %0.2f(s)�ł���\n",
								a_iCur_Mesh,
								(double)((a_dMake_Finish - a_dMesh_Begin_Time) / 1000.0) );
			//	e_sErr_Log.Set_OID	( a_lCur_Oid );
			//----------------
			fprintf	( stdout, "���b�V���R�[�h [%d] �`�F�b�N�I��\n", a_iCur_Mesh );
			//-----------------
		}
	
	}
	return	( true );
}

/**
	���C�����̂���K�v�ȃ��[�N�X�y�[�X�𔻒肷��
*/
IWorkspacePtr	main_rootin::
Get_Ref_Workspace	(	IWorkspacePtr	c_ipWSpace_Map,		///< ���[�N�X�y�[�X
						IWorkspacePtr	c_ipWSpace_Base,	///< ���[�N�X�y�[�X
						IWorkspacePtr	c_ipWSpace_Addr,	///< ���[�N�X�y�[�X
						char*			c_cpLayerName )
{
	if( c_cpLayerName == NULL ) {
		return	( NULL );
	}
	IWorkspacePtr	a_ipRet_WSpace	= NULL;
	char*			a_cpLayerTmp	= NULL;
	a_cpLayerTmp	= new char[strlen(c_cpLayerName) + 1];
	char*			a_cpLayer	= NULL;

	// ���C�������R�s�[
	strcpy ( a_cpLayerTmp, c_cpLayerName );
	a_cpLayer	= strtok( a_cpLayerTmp, "." );	// �s���I�h�Ő؂�
	a_cpLayer	= strtok( NULL, "." );
	if( a_cpLayer == NULL ) {
		delete [] a_cpLayerTmp;
		return	( NULL );
	}

	// ���C�����Őڑ�WS�𔻒�
	string		a_sLayerName	= a_cpLayer;
	if( a_sLayerName == "CityMesh" ||		///< �s�s�n�}���b�V�����C��	
		a_sLayerName == "BaseMesh" ||		///< ���k�ڃ��b�V�����C��	
		a_sLayerName == "MiddleMesh" )		///< �~�h�����b�V�����C��
	{
		a_ipRet_WSpace	= c_ipWSpace_Map;
	}else
	if( a_sLayerName == "City_Annotation" ||///< �s�s�n�}���L�|�C���g���C��
		a_sLayerName == "City_Disp_Line" ||	///< �s�s�n�}���L���C�����C��
		a_sLayerName == "City_Line"	||		///< �s�s�n�}�w�i�A�[�N���C��
		a_sLayerName == "City_Railway" ||	///< �s�s�n�}�S�����C��
		a_sLayerName == "City_Site" ||		///< �s�s�n�}�X�惌�C��
		a_sLayerName == "City_Station" ||	///< �s�s�n�}�w�Ƀ��C��	
		a_sLayerName == "Building" ||		///< �s�s�n�}�������C��	
		a_sLayerName == "City_Line" ||		///< �s�s�n�}�����A�[�N���C��
		a_sLayerName == "Building_Step" )	///< �s�s�n�}kpl���C��
	{
		a_ipRet_WSpace	= c_ipWSpace_Map;
	}else
	if( a_sLayerName ==	"Sc1Disp_Line"	||	///< ���k�ڒ��L���C�����C���P
		a_sLayerName ==	"Sc2Disp_Line"	||	///< ���k�ڒ��L���C�����C���Q
		a_sLayerName ==	"Sc3Disp_Line"	||	///< ���k�ڒ��L���C�����C���R
		a_sLayerName ==	"Sc4Disp_Line"	||	///< ���k�ڒ��L���C�����C���S
		a_sLayerName ==	"Base_Annotation"	||	///< ���k�ڒ��L
		a_sLayerName ==	"Base_Contour"	||	///< �i�ʃ��C��
		a_sLayerName ==	"Base_Line"	||		///< ���k�ڔw�i�A�[�N���C��
		a_sLayerName ==	"Base_Railway"	||	///< ���k�ړS�����C��
		a_sLayerName ==	"Base_Site"	||		///< ���k�ڔw�i�A�{�݃��C��
		a_sLayerName ==	"Base_Station"	||	///< ���k�ډw�Ƀ��C��
		a_sLayerName ==	"Sc1Disp_Line"	||	///< �~�h�����L���C�����C���P
		a_sLayerName ==	"Sc2Disp_Line"	||	///< �~�h�����L���C�����C���Q
		a_sLayerName ==	"Sc3Disp_Line"	||	///< �~�h�����L���C�����C���R
		a_sLayerName ==	"Sc4Disp_Line"	||	///< �~�h�����L���C�����C���S
		a_sLayerName ==	"Middle_Annotation"	||	///< �~�h�����L
		a_sLayerName ==	"Middle_Contour"	||	///< �i�ʃ��C��
		a_sLayerName ==	"Middle_Line"	||	///< �~�h���w�i�A�[�N���C��
		a_sLayerName ==	"Middle_Site"	)	///< �~�h���w�i�A�{�݃��C��
	{
		a_ipRet_WSpace	= c_ipWSpace_Base;
	}else 
	if( a_sLayerName == "Base_Admin"	||	///< ���k�ڍs���E
		a_sLayerName == "City_Admin"	||	///< �s�s�n�}�s���E
		a_sLayerName == "Base_Rep_Point"||	///< ���k��Addr�|�C���g
		a_sLayerName == "City_Rep_Point" ||	///< �n�Ԓ��L���C��
		a_sLayerName == "Gou_Point" ||		///< ���|�C���g
		a_sLayerName == "KLink_Point"	)	///< ���|�C���g
	{
		a_ipRet_WSpace	= c_ipWSpace_Addr;		
	}

	if( a_cpLayerTmp != NULL ) {
		delete [] a_cpLayerTmp;
	}
	return ( a_ipRet_WSpace );
}
