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

#include <atlstr.h>
#include "..\..\LIB\WinLib\VersionInfo.h"
#include "PointRotate.h"
//	2003.07.04	main_rootin.h�����`�𕪗�


std::string	main_rootin::
Str_to_SJIS		(	CComBSTR*	c_CStr	)	// ������(Unicode)
{	
	std::string		ret_str;

#ifndef	_ARC_9_1_
	USES_CONVERSION;
	if (*c_CStr != 0) {
		_bstr_t a_bTmpStr((*c_CStr));

		ret_str	= a_bTmpStr;
	}
#else
	long		length	= c_CStr->Length();

	char*		tmp_moji	= new char[length+1];

	memset		(tmp_moji, '\0', length+1);
	_bstr_t		a_bTmp ( *c_CStr, false );
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

/** ------------------------------------------------------------------------------
	DB�ւ̐ڑ��I�[�v��
	@return	���[�N�X�y�[�X�̃|�C���^
--------------------------------------------------------------------------------*/
IWorkspacePtr	main_rootin::
DB_Open	(	char*	c_cpTargetDB )		///< �Ώ�DB��
{
	IUnknownPtr ipUnknown = NULL;
	INamePtr ipName = INamePtr(sindy::create_workspace_name(CA2CT(c_cpTargetDB)));
	if( ! ipName ) {
		return NULL;
	}
	ipName->Open(&ipUnknown);
	if( ! ipUnknown ) {
		return NULL;
	}
	IWorkspacePtr ipWorkspace = ipUnknown;
	return ipWorkspace;
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
			PrnMsg	( stderr, "�G���[���O�t�@�C��<%s>���I�[�v���ł��Ȃ�[main_rootin]\n", e_cpLog_File );
			return	( false );
		}
	}
	return	( true );
}

#define	OPTION_MAX	15
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
	// �c�[���o�[�W�����\�� 2009.08.20
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
			PrnMsg	( stderr, "���������񂪒������܂�!!\n" );
			return	( false );
		}
	}

	if(argc < 2) {
		return( false );
	}

	// �I�v�V����������A���̂Ƃ���g�p���Ȃ�
	for(i = 1; i < argc;)
	{
		if(strcmp(argv[i], "-npi") == 0) {	// ���Ԉᔽ����G���A���������@�����g�p
			if(argc < i+2) return( false );
			e_cpInit_Str_NP_AREA	= argv[i+1];
			a_exeflag			|= ON_INIT_NP_AREA;
			a_optcount[0]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-npf") == 0) {	// ���Ԉᔽ����G���A�t�B�[�`���N���X�w��
			if(argc < i+2) return( false );
			e_cpNP_AREA_FC		= argv[i+1];
			a_exeflag			|= ON_FC_NP_AREA;
			a_optcount[1]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-mi") == 0) {	// ���Ԉᔽ����G���A���������
			if(argc < i+2) return( false );
			e_cpInit_Str_MESH	= argv[i+1];
			a_exeflag			|= ON_INIT_MESH;
			a_optcount[2]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-mf") == 0) {	// ���b�V���t�B�[�`���N���X�w��
			if(argc < i+2) return( false );
			e_cpMesh_FC			= argv[i+1];
			a_exeflag			|= ON_FC_MESH;
			a_optcount[4]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-l") == 0) { // ���O�t�@�C���ݒ�i�ݒ肪�����ꍇ�́A�G���[��stderr�֐��ꗬ���ɂȂ�j
			if(argc < i+2) return( false );
			e_cpLog_File	= argv[i+1];	///< ���b�V���R�[�h���X�g�t�@�C��
			a_exeflag		|= ON_LOGFILE;
			a_optcount[5]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-v") == 0) {
			//if(argc < i+1) return( false );
			print_version();
			a_optcount[6]++;
			i+=1;
			return	( false );	// ���ɂ�������H
		}else
		if(strcmp(argv[i], "-m") == 0) { // 2002.9.24
			if(argc < i+2) return( false );
			e_cpMeshCode_File	= argv[i+1];	///< ���b�V���R�[�h���X�g�t�@�C��
			a_exeflag		|= ON_MESH;
			a_optcount[7]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-M") == 0) { // 2003.4.1
			if(argc < i+2) return( false );
			a_exeflag		|= ON_MONO_MESH;
			e_iMeshCode		= atoi(argv[i+1]);	
			a_optcount[8]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-pgdb") == 0) {
			a_exeflag		|= ON_PGDB_MODE;
			e_cpPGDB_File	= argv[i+1];
			a_optcount[9]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-o") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_OUTPATH;
			e_cpOutPath		= argv[i+1];	
			a_optcount[10]++;
			i+=2;
		} else {
			PrnMsg	(stderr, "�I�v�V���� %s �͊Ԉ���Ă܂�!!\n", argv[i]);
			return	( false );
		}
	}

	// �I�v�V�����d���̃`�F�b�N
	for(i = 0; i < OPTION_MAX; i++)
	{
		if(a_optcount[i] > 1) {
			PrnMsg	(stderr, "�I�v�V�����̎w�肪�d�����Ă��܂�!!\n");
			return	( false );
		}
	}

	if( !(a_exeflag & ON_MESH) && !(a_exeflag & ON_MONO_MESH) ) {
		PrnMsg	(stderr, "'-m'��'-M'�Ń��b�V���̎w������Ă��������B\n");
		return	( false );
	}

	if( !(a_exeflag & ON_FC_NP_AREA) ) {		///< ���L�t�B�[�`���N���X
		PrnMsg	(stderr, "���Ԉᔽ����G���A�̃t�B�[�`���N���X���w�肳��Ă��܂���\n");
		return	( false );
	}
	if( !(a_exeflag & ON_INIT_MESH) ) {
		PrnMsg	(stderr, "���b�V���̐ڑ����w�肪����Ă��܂���\n");
		return	( false );
	}
	if( !(a_exeflag & ON_FC_MESH) ) {		///< ���b�V���t�B�[�`���N���X
		PrnMsg	(stderr, "���b�V���̃t�B�[�`���N���X���w�肳��Ă��܂���\n");
		return	( false );
	}
	if( !(a_exeflag & ON_OUTPATH) ) {		///< �o�̓p�X
		PrnMsg	(stderr, "�f�[�^�o�̓p�X���w�肳��Ă��܂���\n");
		return	( false );
	}
	if( !(a_exeflag & ON_PGDB_MODE) ) {		///< PGDB
		PrnMsg	(stderr, "���Ԉᔽ����G���A��PGDB�t�@�C�����w�肳��Ă��܂���\n");
		return	( false );
	}

	// �G���[���O�t�@�C���̃I�[�v��	2006.05.17 ���s�����o�͂������̂ł����ֈړ�
	if( !Open_Err_Log () ) {
		return	( false );
	}else {
		if( !e_sPrnInfo.Init(e_fErr_Fp) ) {
			return	( false );
		}
	}

	//--------------------------------------------------------------------------
	// ���s�����o�� 2009.09.30 �ǉ�
	PrnMsg( e_fErr_Fp, "#�c�[���o�[�W����                               : <%s>\n", m_sVersion.c_str() );
	PrnMsg( e_fErr_Fp, "#�����В��Ԉᔽ����G���A�t�B�[�`���N���X       : <%s>\n", e_cpNP_AREA_FC );
	PrnMsg( e_fErr_Fp, "#���b�V���t�B�[�`���N���X�pDB�ڑ����           : <%s>\n", e_cpInit_Str_MESH );
	PrnMsg( e_fErr_Fp, "#���b�V���t�B�[�`���N���X                       : <%s>\n", e_cpMesh_FC );
	if(a_exeflag & ON_PGDB_MODE) {
	PrnMsg( e_fErr_Fp, "#PGDB�t�@�C��                                   : <%s>\n", e_cpPGDB_File );
	}
	PrnMsg( e_fErr_Fp, "#�f�[�^�o�̓p�X                                 : <%s>\n", e_cpOutPath );
	if( a_exeflag & ON_MESH ) {
	PrnMsg( e_fErr_Fp, "#�`�F�b�N�Ώۃ��b�V�����X�g      : <%s>\n", e_cpMeshCode_File );
	}else {
	PrnMsg( e_fErr_Fp, "#�`�F�b�N�Ώۃ��b�V���R�[�h      : <%02d>\n", e_iMeshCode );
	}
	//--------------------------------------------------------------------------
	
	eExe_Flag	= a_exeflag;

	return		( true );
}

/** ------------------------------------------------------------------------------
// �g�p�@�\��
--------------------------------------------------------------------------------*/
void		main_rootin::
print_usage	(char **argv)
{
	PrnMsg(	stdout,
				"=== �y���Ԉᔽ����G���A�|���S��Shape�ϊ��c�[���z ===\n"
				"<usage>\n"
				"%s <�I�v�V����>... \n"
				"\t<����>\n"
			//	"\t      <arg1>         : ���k�ڒ��L�����ݒ�(ex. <user>/<passwd>/<instance>/<version>/<server>\n)"
			//	"\t      <arg2>         : ���k�ڒ��L�`�F�b�N���C����ex. SiNDY2006A.Base_Anno)\n"
			//	"\t      <arg3>         : �s�s�n�}���L�����ݒ�(<user>/<passwd>/<instance>/<version>/<server>\n)"
			//	"\t      <arg4>         : �s�s�n�}���L�`�F�b�N���C����(ex. SiNDY2006A.Base_Anno)\n"
			//	"\t      <arg5>         : �~�h�����L�����ݒ�(<user>/<passwd>/<instance>/<version>/<server>\n)"
			//	"\t      <arg6>         : �~�h�����L�`�F�b�N���C����(ex. SiNDY2006A.Base_Anno)\n"
			//	"\t      <arg7>         : ���b�V�����C����(ex. REFERENCE.BaseMesh)\n"
			//	"\t      <arg8>         : �����t�@�C��(ex. cross.dic)\n"
			//	"\t<�I�v�V����>\n"
				"\t      '-m'           : ���b�V�����X�g�t�@�C�������w��(ex. -m index.base)\n"
				"\t      '-M'           : �P���b�V�����w��(ex. -M 594141)\n"
				"\t      '-v'           : �o�[�W�������\��\n"
				"\t      '-npi'         : �����В��Ԉᔽ����G���A�|���S�����C���ڑ���� ex) -npi MQ200709/MQ200709/SDE.DEFAULT/ruby2\n"
				"\t      '-npf'         : �����В��Ԉᔽ����G���A�t�B�[�`���N���X�w�� ex) -npi MQ200709.City_Annotation\n"
				"\t      '-mi'          : ���b�V���t�B�[�`���N���X�ڑ���� ex) -mi ronly/ronly/SDE.DEFAULT/ruby2\n"
				"\t      '-mf'          : ���b�V���t�B�[�`���N���X�w��  ex) -mf REFERENCE.CityMesh\n"
				"\t      '-l'           : ���O�t�@�C�������w�� ex) -l .\\test\\make.log\n"
				"\t      '-pgdb'        : PGDB�t�@�C�������w�� ex) -pgdb .\\data\\ANNO.mdb\n"
				"\t      '-o'           : �f�[�^�o�̓p�X ex) -pgdb .\\data\\Citymap\n"
				"\t                       ���w�肵���f�[�^�p�X�ȉ��Ƀ��b�V���̃t�H���_�������쐬���o�͂���B\n"
				"\t<�⑫>\n"
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
	PrnMsg(	stdout,
				"=== �y���Ԉᔽ����G���A�|���S��Shape�ϊ��c�[���z ===\n"
				"\t<�o�[�W�������>\n"
			//	"\t2007/09/21        1.0.0.1            : �V�K�쐬\n"
				"\t2007/11/15        1.0.0.1            : �V�K�쐬\n"
				"\t2009/09/30        2.0.0.2            : �����Ђ̐V�t�H�[�}�b�g�ւ̑Ή�\n"
				"\t2014/11/27        2.0.0.3            : �c�[���̖߂�l�𓝈�,���v���J�Ή�\n"
			);
}

/** ------------------------------------------------------------------------------
	���b�V�����C���I�[�v��
------------------------------------------------------------------------------==*/

bool	main_rootin::
Open_Mesh_Layers	(	void	)
{
	IFeatureWorkspacePtr	a_F_Workspace	( e_ipWorkspace_Mesh );	// �t�B�[�`�����[�N�X�y�[�X

	if( FAILED(a_F_Workspace->OpenFeatureClass(CComBSTR(e_cpMesh_FC), &e_ipF_Class )) ) {
		PrnMsg( e_fErr_Fp, "���C�� <%s> �̃I�[�v���Ɏ��s\n", e_cpMesh_FC);
		return	(false);
	}else {
		PrnMsg( stdout,	"<%s> ���C�����I�[�v��\n", e_cpMesh_FC);
	}

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
	if( FAILED(e_ipF_Class->get_ShapeFieldName	( &a_SpatialCol )) ) {
		PrnMsg( e_fErr_Fp, "%d\t�V�F�C�v�̃t�B�[���h���擾�Ɏ��s\n", c_iMeshCode);
		return	( NULL );
	}

	_bstr_t		a_Where_Str;
	char		a_tmpstr[30];	//�@�����͂Ȃ�Ƃ��������C������

	crd_cnv		crd;
	int			a_iDigit	= 0;
	a_iDigit	= crd.GetMeshDigit( c_iMeshCode );
	switch (a_iDigit)
	{
	case	2:
	case	4:
	case	6:
	case	8:
		sprintf(a_tmpstr, "MESHCODE = %.2d", c_iMeshCode);		// ���b�V���R�[�h�𕶎����
		eDigit	= a_iDigit;
		break;
	default:
		PrnMsg( e_fErr_Fp, "%d\t���b�V���R�[�h�̌������s��\n", c_iMeshCode);
		return	( NULL );
		break;
	}

	a_Where_Str	= a_tmpstr;		// string��BSTR�ɕϊ�

	IQueryFilterPtr		a_Sp_Filter	( CLSID_QueryFilter );			// ��Ԍ����t�B���^
	if( FAILED	(a_Sp_Filter->put_WhereClause	( a_Where_Str)) ) {		// Where��̐ݒ�
		PrnMsg( e_fErr_Fp, "%d\tWhere��̐ݒ�Ɏ��s\n", c_iMeshCode);
		return	( NULL );
	}

	// �N���b�v��`�쐬
	IEnvelopePtr	a_ipMesh_Rect;	// ���b�V����`
	a_ipMesh_Rect	= NULL;

	// �T�[�`���Ă���
	IFeatureCursorPtr	a_F_Cursor;
	if( FAILED	(e_ipF_Class->Search	( a_Sp_Filter, VARIANT_FALSE, &a_F_Cursor )) ) {
		PrnMsg	(e_fErr_Fp, "%d\t�f�[�^�̑��������Ɏ��s\n", c_iMeshCode);
		return	(NULL);
	}else {
		IFeaturePtr				a_Feature;

		for(;;)
		{
			if( a_F_Cursor->NextFeature ( &a_Feature ) != S_OK) {	// �f�[�^���ЂƂ��o��
				break;
			}
				
			IGeometryPtr		a_Clip_Shp;		// �����Ƀf�[�^���R�s�[���ăN���b�v����

			if( FAILED(a_Feature->get_ShapeCopy	( &a_Clip_Shp )) ) {	// �t�B�[�`������
				PrnMsg( e_fErr_Fp, "%d\t�V�F�C�v�̃R�s�[�Ɏ��s\n", c_iMeshCode);
				return	( NULL );
			}
			
			VARIANT_BOOL	a_bResult;
			a_Clip_Shp->get_IsEmpty( &a_bResult );
			if(a_bResult == VARIANT_TRUE) {	// �N���b�v���ʂ�NULL�Ȃ烁�b�V���O
				continue;
			}
			if( FAILED(a_Clip_Shp->get_Envelope( &a_ipMesh_Rect) )) {
				PrnMsg( e_fErr_Fp, "�Y�����b�V��<%d>�̋�`���݂���܂���ł���\n", c_iMeshCode);
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
Open_Layers		( void )
{
	if( !e_mNP_AREA_WS.Init ( e_ipWorkspace_NP_AREA, e_cpNP_AREA_FC ) ) {
		PrnMsg( stderr, "���Ԉᔽ����G���A�t�B�[�`���N���X<%s>�̓ǂݍ��݂Ɏ��s\n", e_cpNP_AREA_FC );
		return	( false );
	}
	return	( true );
}

/** ------------------------------------------------------------------------------
	���b�V���P�ʃt�@�C�����̍쐬
--------------------------------------------------------------------------------*/
std::string	main_rootin::
make_mesh_fname ( int		c_iMeshCode,	///< ���b�V���R�[�h
				  int		e_iSubPathNum,	///< �T�u�p�X�̊K�w
				  char*		c_cpSrcShpPath,	///< �\�[�X�V�F�C�v�p�X
				  char*		c_cpAddStr,		///< �t�@�C�����ɒǉ����镶����
				  string&	c_sMkSubPath )	///< �������ꂽ�T�u�f�B���N�g��
{
	std::string	a_sRetStr	= "";
	std::string	a_sSubDir	= "";

	char	a_sTmp[4096];
	char	a_sTmp2[4096];

	int	a_iDigit	= 0;
	a_iDigit	= e_cCrd.GetMeshDigit ( c_iMeshCode );

	if( a_iDigit == 2 ) {
		sprintf	( a_sTmp, "%s\\%.2d\\%.2d%s", c_cpSrcShpPath, c_iMeshCode, c_iMeshCode, c_cpAddStr );
		sprintf	( a_sTmp2,"%s\\%.2d", c_cpSrcShpPath, c_iMeshCode );
		c_sMkSubPath= a_sTmp2;	// �p�X���Ԃ��悤��
	}else	{
		char	a_sSubStr[256];
		memset	( a_sSubStr, '\0', 256 );
		switch ( e_iSubPathNum )
		{
		case	0:
			if( c_iMeshCode >= 10000000 ) {
				sprintf ( a_sSubStr, "\\%d", c_iMeshCode/10000 );
			}else
			if( c_iMeshCode >= 100000 ) {
				sprintf ( a_sSubStr, "\\%d", c_iMeshCode/100);				
			}
			break;
		case	1:
			sprintf ( a_sSubStr, "\\%.2d", c_iMeshCode );
			break;
		case	2:
			sprintf ( a_sSubStr, "\\%d\\%d", c_iMeshCode / 100, c_iMeshCode );
			break;
		case	3:
			sprintf ( a_sSubStr, "\\%d\\%d\\%d", c_iMeshCode / 10000, c_iMeshCode / 100, c_iMeshCode );
			break;
		default:
			break;
		}
		a_sSubDir	= a_sSubStr;

		sprintf	( a_sTmp2,"%s%s", c_cpSrcShpPath, (char*)a_sSubDir.c_str() );
		c_sMkSubPath= a_sTmp2;	// �p�X���Ԃ��悤��

		sprintf	( a_sTmp, "%s%s\\%d%s", c_cpSrcShpPath, (char*)a_sSubDir.c_str(), c_iMeshCode, c_cpAddStr );
	}

	a_sRetStr	= a_sTmp;

	return ( a_sRetStr );
}

/** ------------------------------------------------------------------------------
	�t�@�C���̃t���p�X������A�V�F�C�v�t�@�C�����𒊏o����֐�
	Function to extract the shape file name from the fullpath of the file
	@return	�Ȃ�
--------------------------------------------------------------------------------*/
bool main_rootin::
GetFeatureClass	(	const std::string&	sourceFile,		///< �\�[�X�t�@�C����
					std::string&		className )		///< �p�X��
{
	std::string	tempName;
	std::string	temp;

	tempName	= sourceFile;

	char*	p1	= NULL;
	p1	= strrchr( (char*)tempName.c_str(), '\\');
	if( p1 == NULL ) {
		className	= tempName;	// '\'�}�[�N���݂���Ȃ��̂Ńp�X�����܂܂Ȃ�
	}else {
		className	= p1+1;
	}
	return	( true );
}

/** ------------------------------------------------------------------------------
	�t���p�X������A�t�@�C�����������āA�f�B���N�g������Ԃ��֐�
	// Function to remove the file name from the fullpath and return the directory
	@return	�Ȃ�
--------------------------------------------------------------------------------*/
bool	main_rootin::
GetFilePath	(	const std::string&	sourceFile,		///< �\�[�X�t�@�C����
				std::string&		filePath )		///< �p�X��
{

	std::string	temp;
	temp	= sourceFile;

	char*		pathEnd;
	pathEnd		= strrchr((char*)temp.c_str(), '\\');
	if( pathEnd == NULL ) {
		PrnMsg( stderr, "�f�B���N�g�����擾�Ɏ��s\t%s\n", (char*)temp.c_str() );
		return	( false );
	}
	*pathEnd	= '\0';		// ��������؂�
	
	filePath	= temp;
	return	( true );
}

/** ------------------------------------------------------------------------------
	�f�[�^��ǂݍ��݁AShape�t�@�C���ɕϊ�
	@return	����	true
	@return ���s	false
--------------------------------------------------------------------------------*/
bool	main_rootin::
Read_NP_AREA_to_Shape	( void )
{

	IFeatureCursorPtr	a_ipNP_Area_Cursor;

	// �L���b�V����ɂƂ��Ă���
	// �I���������e�X�g 2002.6.24
	int		a_iCur_Mesh;
	for( a_iCur_Mesh = e_sInput_List.get_mesh_first(); a_iCur_Mesh != -1; a_iCur_Mesh = e_sInput_List.get_mesh_next() )
	{
		PrnMsg( stdout, "���b�V��[%d] �f�[�^�擾\n", a_iCur_Mesh );

		e_iCurMeshCode	= a_iCur_Mesh;

		// ���b�V����`�̎��o��
		IGeometryPtr	a_ipMeshRect;
		a_ipMeshRect	= get_mesh_rect( a_iCur_Mesh );
		if( a_ipMeshRect == NULL ) {	// ���b�V����`�������ꍇ
			PrnMsg( e_fErr_Fp, "%d\t�̃��b�V����`��������܂���ł���\n", a_iCur_Mesh );
			return	( false );
		}
		
		IEnvelopePtr	a_ipRect;
		if( a_ipMeshRect->get_Envelope ( &a_ipRect ) ) {
			PrnMsg( e_fErr_Fp, "%d\t�̃��b�V����`�̎擾�Ɏ��s\n", a_iCur_Mesh );
			return	( false );
		}

		// ��ԎQ�Ƃ��擾���� 2007.08.22
		ISpatialReferencePtr	a_ipSpRef;
		if( FAILED (a_ipRect->get_SpatialReference ( &a_ipSpRef )) ) {
			PrnMsg( e_fErr_Fp, "%d\t��ԎQ�Ƃ̎擾�Ɏ��s\n", a_iCur_Mesh );
			return	( false );
		}

		a_ipNP_Area_Cursor	= e_mNP_AREA_WS.Search_Feature ( a_ipRect, esriSpatialRelIntersects, "", RET_FCURSOR );
		if ( a_ipNP_Area_Cursor == NULL ) {
			e_sPrnInfo.Prn_Info	( "���Ԉᔽ�|���S���f�[�^���擾�ł��܂���ł���\n", stderr );
			return	( false );
		}
		PrnMsg( stdout, "���b�V���R�[�h<%d> ���Ԉᔽ�|���S���f�[�^�ǂݍ��ݒ�\n", a_iCur_Mesh );
		if ( ! Set_NP_AREA_Data	( a_ipMeshRect, a_ipNP_Area_Cursor, e_mAll_Data, CITY_NOTE ) ) {
			return	( false );
		}

		// �o�̓t�@�C�����쐬
		std::string	a_sOutFName;
		int	a_iMkLv	= 0;
		switch (eDigit)
		{
		case	2:	a_iMkLv	= 1; break;
		case	4:	a_iMkLv	= 1; break;
		case	6:	a_iMkLv	= 2; break;
		case	8:	a_iMkLv	= 3; break;
			break;
		}
		std::string	a_sOutPath;
		a_sOutFName	= make_mesh_fname ( a_iCur_Mesh, a_iMkLv, e_cpOutPath, "_NPA.shp", a_sOutPath );
		// �f�[�^�o�̓p�X���쐬
		if( e_cUC.makedir ( (char*)a_sOutPath.c_str() ) == false ) {
			PrnMsg( e_fErr_Fp, "%d\t�f�B���N�g���쐬�Ɏ��s<%s>\n", a_iCur_Mesh, (char*)a_sOutPath.c_str() );
			return	( false );
		}

		if( ! NP_Area_to_Shape (	a_ipSpRef, e_mAll_Data, (char*)a_sOutFName.c_str() ) ) {
			return	( false );
		}

		e_mAll_Data.clear();
	}
	return	( true );
}

/**
	�`��N���b�v
*/
IGeometryPtr	main_rootin::
Mk_Clip_Data ( IGeometryPtr&		c_ipClipPoly,	///< �N���b�v�p�|���S��
			   IGeometryPtr&		c_ipTgtGeo, 	///< �N���b�v�����}�`
			   int					c_iClipType,	///< �N���b�v���� 0=Intersect 0�ȊO=Difference
			   int*					c_ipEmpty_F )	///< ��t���O
{

	IFeaturePtr				a_Feature;
	long					a_Shape_Number	= 0;	// �V�F�C�v�e�[�u���ɒǉ�����V�F�C�v��Info
	long					a_lSp_InitFlag	= 0;	// ��ԎQ�Ƃ��������������ǂ����̃t���O
	long					a_lFCount		= 0;	// �t�B�[�`���̃J�E���g

	IGeometryPtr		a_Src_Shp;	// �����g��Ȃ��H
	
	IGeometryPtr			a_ipCliped_Shp;		// �����Ƀf�[�^���R�s�[���ăN���b�v����
	ITopologicalOperatorPtr	a_ipClip_Topo;
	a_ipClip_Topo			= c_ipTgtGeo;
	
	if( c_iClipType == 0 ) {
		if( FAILED( a_ipClip_Topo->Intersect (c_ipClipPoly, esriGeometryNoDimension, &a_ipCliped_Shp)) ) {
			PrnMsg( e_fErr_Fp, "%d\tIntersect���s\n", e_iCurMeshCode);
			return	( NULL );
		}
	}else {
		if( FAILED( a_ipClip_Topo->Difference (c_ipClipPoly, &a_ipCliped_Shp)) ) {
			PrnMsg( e_fErr_Fp, "%d\tDifference���s\n", e_iCurMeshCode);
			return	( NULL );
		}
	}

	VARIANT_BOOL	a_bResult;
	a_ipCliped_Shp->get_IsEmpty( &a_bResult );
	if(a_bResult == VARIANT_TRUE) {	// �N���b�v���ʂ�NULL�Ȃ烁�b�V���O
		*c_ipEmpty_F	= 1;	
	//	fprintf( e_fErr_Fp,"�N���b�v���ʂ���\n");
		return	( NULL );
	}

	// ��ԎQ�Ƃ��擾���� 2007.08.22
	ISpatialReferencePtr	a_ipSpRef;
	if( FAILED (c_ipTgtGeo->get_SpatialReference ( &a_ipSpRef )) ) {
		PrnMsg( e_fErr_Fp, "%d\t��ԎQ�Ƃ̎擾�Ɏ��s\n", e_iCurMeshCode );
		return	( NULL );
	}

	// �V���v���t�@�C��������
	ITopologicalOperator2Ptr	a_ipTopo;

	a_ipTopo	= a_ipCliped_Shp;

	if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
		PrnMsg( e_fErr_Fp, "%d\tSimplify�Ɏ��s\n", e_iCurMeshCode );
		return	( NULL );
	}
	IPolygonPtr	a_ipTmpPoly;
	a_ipTmpPoly = a_ipCliped_Shp;

	if(FAILED ( a_ipTmpPoly->SimplifyPreserveFromTo() ) ) {
		PrnMsg( e_fErr_Fp, "%d\tSimplifyPreserveFromTo�Ɏ��s\n", e_iCurMeshCode );	// �G���[���b�Z�[�W
		return	( NULL );
	}
	// ��ԎQ�Ƃ̕t�^
	if( FAILED (a_ipCliped_Shp->putref_SpatialReference ( a_ipSpRef )) ) {
		PrnMsg( e_fErr_Fp, "%d\t��ԎQ�Ƃ̕t�^�Ɏ��s\n", e_iCurMeshCode );
		return	( NULL );
	}

	return ( a_ipCliped_Shp );
}

/** ------------------------------------------------------------------------------
	���Ԉᔽ����G���A�f�[�^���Z�b�g�ɗ��Ƃ�
	@return	���� true
	@return	���s false
--------------------------------------------------------------------------------*/
bool		main_rootin::
Set_NP_AREA_Data ( IGeometryPtr&		c_ipMeshRect,		///< ���b�V����`
				   IFeatureCursorPtr&	c_ipNoteCursor, 	///< ���L�J�[�\��
				   ms_data_info&		c_mNP_Area_Data,	///< ���Ԉᔽ�|���S���f�[�^
				   int					c_iData_Type	)	///< �f�[�^�^�C�v
{
	int	a_iCount	= 0;
	// �f�[�^���}���`�Z�b�g�̃f�[�^�ɗ��Ƃ�
	if( c_ipNoteCursor != NULL ) {
		IFeaturePtr	a_ipNote_Feat;
		// �������ꂽ�|���S���̃��[�v
		ms_data_info	a_mTmp_NP_AREA_Data;
		for(;;)
		{
			data_info	a_dNP_AREA_Data;
			if( c_ipNoteCursor->NextFeature ( &a_ipNote_Feat ) == S_OK ) {

				Feature_All_Attrs	a_fNote_Attrs;
				if( ! a_fNote_Attrs.Init	( a_ipNote_Feat ) ) {
					PrnMsg( e_fErr_Fp, "%d\t���Ԉᔽ�|���S���f�[�^�̂P�t�B�[�`���̑S�����̎擾�Ɏ��s\n", e_iCurMeshCode );
					return	( false );
				}

				Feature_Attr		a_fPolyID, a_fAttrCode, a_fGroupCode;
				// �I�u�W�F�N�gID
				a_fPolyID			= a_fNote_Attrs.Get_Attr ( "OBJECTID" );	// ���L��OID
			//	a_fAttrCode			= a_fNote_Attrs.Get_Attr ( "AttrCode" );	// �������ރR�[�h914@�`916@
			//	a_fGroupCode		= a_fNote_Attrs.Get_Attr ( "GroupCode" );	// ���ʔԍ�
				// �d�l�ύX�̈וύX	2009.05.18 		
				a_fAttrCode			= a_fNote_Attrs.Get_Attr ( "CODE" );		// �������ރR�[�h914@�`916@
	
				std::string		a_sTmpAttrCode;

				a_sTmpAttrCode	= Str_to_SJIS ((CComBSTR *)(&a_fAttrCode.e_ccvAttr.bstrVal));

				a_dNP_AREA_Data.e_iOID			= a_fPolyID.e_ccvAttr.intVal;
				a_dNP_AREA_Data.e_iGroupCode	= a_fPolyID.e_ccvAttr.intVal;
			//	�d�l�ύX�̈וύX	2009.05.18
			//	if( a_sTmpAttrCode == "914@" ) {	// �ŏd�_
				if( a_sTmpAttrCode == "P_avaa" ) {
					a_dNP_AREA_Data.e_iAttrCode	= 1;
				}else
			//	�d�l�ύX�̈וύX	2009.05.18
			//	if( a_sTmpAttrCode == "915@" ) {	// �d�_
				if( a_sTmpAttrCode == "P_avab" ) {
					a_dNP_AREA_Data.e_iAttrCode	= 2;				
				}else {	// �ŏd�_�A�d�_�ȊO�͓���Ȃ�
					continue;
				}
				
				// �����ŃN���b�v����@2007.10.11
				a_dNP_AREA_Data.e_ipGeo		= a_fNote_Attrs.Ret_Geo();			// �W�I���g��

				//---test
			//	if( a_dNP_AREA_Data.e_iOID == 1932 ) {
					ITopologicalOperator2Ptr	a_ipTopo(a_dNP_AREA_Data.e_ipGeo);
					if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
						PrnMsg( e_fErr_Fp, "%d\tSimplify�Ɏ��s\t%d\n", e_iCurMeshCode, a_dNP_AREA_Data.e_iOID );
						return	( false );
					}
					IPolygonPtr	a_iptmp(a_dNP_AREA_Data.e_ipGeo);
					if( FAILED(a_iptmp->SimplifyPreserveFromTo()) ){
						PrnMsg( e_fErr_Fp, "%d\tSimplifyPreserveFromTo�Ɏ��s\t%d\n", e_iCurMeshCode, a_dNP_AREA_Data.e_iOID );
						return	( false );
					}
			//		Out_to_Shape (	a_dNP_AREA_Data.e_ipGeo, ".\\testdata" );		///< �o�̓V�F�C�v�t�@�C����
			//		return	( false );
			//	}
				//---test kokomade

				// ���b�V���ŃN���b�v
				int				a_iEmpty_F	= 0;
				IGeometryPtr	a_ipCliped_Poly;
				a_ipCliped_Poly	= Mk_Clip_Data ( c_ipMeshRect, a_dNP_AREA_Data.e_ipGeo, 0, &a_iEmpty_F );
				if( a_iEmpty_F == 1 ) {
					PrnMsg( e_fErr_Fp, "%d\t��ɂȂ��Ă܂�\t%d\n", e_iCurMeshCode, a_dNP_AREA_Data.e_iOID);
					continue;
				}
				if( a_ipCliped_Poly == NULL ) {
					PrnMsg( e_fErr_Fp, "%d\tNULL�ɂȂ��Ă܂�\t%d\n", e_iCurMeshCode, a_dNP_AREA_Data.e_iOID);
					return( false );
				}
				
				// �N���b�v�f�[�^�ƍ����ւ�
				a_dNP_AREA_Data.e_ipGeo	= a_ipCliped_Poly;	

				a_mTmp_NP_AREA_Data.insert( a_dNP_AREA_Data );
				a_iCount++;
			}else {
				break;
			}
		}
		
		// �N���b�v�����͂����ł��
		ims_data_info	a_iCIndx1;
		int	a_iSkip_F	= 0;
		for( a_iCIndx1 = a_mTmp_NP_AREA_Data.begin(); a_iCIndx1 != a_mTmp_NP_AREA_Data.end(); a_iCIndx1++ )
		{
			data_info	a_dSrcDat;
			a_dSrcDat	= *a_iCIndx1;
			a_iSkip_F	= 0;

			ims_data_info	a_iCIndx2;
			for( a_iCIndx2 = a_iCIndx1; a_iCIndx2 != a_mTmp_NP_AREA_Data.end(); a_iCIndx2++ )
			{
				// �������m�̏ꍇ�̓X�L�b�v
				if( a_iCIndx1 == a_iCIndx2 ) {
					continue;
				}
				data_info	a_dTgtDat;
				a_dTgtDat	= *a_iCIndx2;

				// �C���^�[�Z�N�g���Ȃ��ꍇ�̓X�L�b�v
				//-------------------------------------------------------------------------------------
				VARIANT_BOOL	a_vRetBool;
				IRelationalOperatorPtr	a_ipRel;
				a_ipRel	= a_dSrcDat.e_ipGeo;
				if( FAILED(a_ipRel->Disjoint( a_dTgtDat.e_ipGeo, &a_vRetBool )) ){
					PrnMsg( e_fErr_Fp, "%d\tDisjoint()�֌W�̎擾�Ɏ��s\n", e_iCurMeshCode);
					return	( false );
				}
				if( a_vRetBool == VARIANT_TRUE ) {
					continue;
				}

				// �^�b�`���X�L�b�v
				if( FAILED(a_ipRel->Touches ( a_dTgtDat.e_ipGeo, &a_vRetBool )) ){
					PrnMsg( e_fErr_Fp, "%d\tTouches()�֌W�̎擾�Ɏ��s\n", e_iCurMeshCode);
					return	( false );
				}
				if( a_vRetBool == VARIANT_TRUE ) {
					continue;
				}

				//-------------------------------------------------------------------------------------
				// Tgt�� e_iAttrCode(�������ރR�[�h (914@(�ŏd�_)=2 915@(�d�_)=1)) ���������d�_�x����̏ꍇ�A
				// �d�_�x�������ꍇ�ł�����ID����̏ꍇ��Tgt��Src���N���b�v
				int	a_iClip_F	= 0;
				if( (a_dSrcDat.e_iAttrCode > a_dTgtDat.e_iAttrCode) ||
					((a_dSrcDat.e_iAttrCode == a_dTgtDat.e_iAttrCode) && (a_dSrcDat.e_iGroupCode < a_dTgtDat.e_iGroupCode)) )
				{
					int				a_iEmpty_F	= 0;
					IGeometryPtr	a_ipCliped_Poly;
					a_ipCliped_Poly	= Mk_Clip_Data ( a_dTgtDat.e_ipGeo, a_dSrcDat.e_ipGeo, 1, &a_iEmpty_F );
					if( a_iEmpty_F == 1 ) {
						PrnMsg( e_fErr_Fp, "%d\t��ɂȂ��Ă܂�\t%d\n", e_iCurMeshCode, a_dSrcDat.e_iOID);
						a_iSkip_F	= 1;
						break;
					}
					if( a_ipCliped_Poly == NULL ) {
						PrnMsg( e_fErr_Fp, "%d\tNULL�ɂȂ��Ă܂�\t%d\n", e_iCurMeshCode, a_dSrcDat.e_iOID);
						return	( false );
					}
					// �N���b�v�����f�[�^�Ɠ���ւ�
					a_dSrcDat.e_ipGeo	= a_ipCliped_Poly;
				}
			}
			// ��ɂȂ�ꍇ�̓X�L�b�v
			if( a_iSkip_F == 1 ) {
				continue;
			}

			IPolygonPtr		a_ipTmpPolygon(a_dSrcDat.e_ipGeo);
			if( FAILED(a_ipTmpPolygon->SimplifyPreserveFromTo()) ) {
				PrnMsg( e_fErr_Fp, "%d\tSimplifyPreserveFromTo�Ɏ��s\t%d\n", e_iCurMeshCode, a_dSrcDat.e_iOID );
				return	( false );
			}

			PointRotate		a_pRotate;
			IGeometryPtr	a_ipRotatedGeo;
			if( (a_ipRotatedGeo	= a_pRotate.ExecRotate ( a_dSrcDat.e_ipGeo )) == NULL ) {
				PrnMsg( e_fErr_Fp, "%d\t�_��̉�]�Ɏ��s\t%d\n", e_iCurMeshCode, a_dSrcDat.e_iOID );
				return	( false );
			}

			// �}���`�p�[�g�ɂȂ��Ă���|���S���𕪊�����
			msi_get_parts_info	a_mMP_Poly;
			if( ! a_mMP_Poly.Init ( a_ipRotatedGeo, a_dSrcDat.e_iOID, "NP_Polygon", 59414130, e_fErr_Fp ) ) {
				PrnMsg( e_fErr_Fp, "%d\t�}���`�p�[�g�|���S���̕����Ɏ��s\t%d\n", e_iCurMeshCode, a_dSrcDat.e_iOID );
				return	( false );
			}

			long	a_lPart_Num	= 0;
			a_lPart_Num	= a_mMP_Poly.Get_Part_Num();
			int i = 0;
			for( i = 0; i < a_lPart_Num; i++ )
			{
				IGeometryPtr	a_ipCurGeo;
				if( (a_ipCurGeo	= a_mMP_Poly.Get_Geo_from_Part ( i+1 )) == NULL ) {
					PrnMsg( e_fErr_Fp, "%d\t�V�F�C�v�̎w��p�[�g�A�T�u�p�[�g����A�W�I���g���擾�Ɏ��s\t%d\t%d\n", e_iCurMeshCode, a_dSrcDat.e_iOID, i );
					return	( false );
				}
				data_info	a_dTmpDat;
				a_dTmpDat	= a_dSrcDat;
				a_dTmpDat.e_ipGeo	= a_ipCurGeo;
				c_mNP_Area_Data.insert ( a_dTmpDat );
			}
		}
	}
	return	( true );
}

/** ------------------------------------------------------------------------------
	���Ԉᔽ����G���A�f�[�^���V�F�[�v�t�@�C���ɕϊ�
	@return	bool
--------------------------------------------------------------------------------*/
bool	main_rootin::
NP_Area_to_Shape (	ISpatialReferencePtr&	c_ipSpRef,			///< ��ԎQ�Ə��
					ms_data_info&			c_mNP_Area_Data,	///< ���Ԉᔽ����G���A�f�[�^
					char*					c_cpOutFName )		///< �o�̓V�F�C�v�t�@�C����
{
	std::string	a_sPathName	= "";	// �f�B���N�g����
	std::string	a_sShpName	= "";	// �p�X�𔲂����V�F�C�v�t�@�C����

	if(!GetFeatureClass ( c_cpOutFName, a_sShpName ) ) {
		return	( false );
	}

	if(!GetFilePath	( c_cpOutFName, a_sPathName ) ) {
		return	( false );
	}
		
	//-------------------------------------------------------------------------------
	// Shape�t�@�C���̃I�[�v������	
	//-------------------------------------------------------------------------------
	IWorkspaceFactoryPtr	a_ipWSFactory(CLSID_ShapefileWorkspaceFactory);
	IPropertySetPtr			a_ipPropertySet(CLSID_PropertySet);
	IWorkspacePtr			a_ipSWSpace;

	if (a_ipWSFactory == 0) {
	//	E_POINTER;
		PrnMsg( e_fErr_Fp, "%d\t���[�N�X�y�[�X�i�f�B���N�g���j�̐����Ɏ��s���܂���\n", e_iCurMeshCode);
		return	( false );
	}
	// ���[�N�X�y�[�X���I�[�v��
	CComVariant		aVar_Dir((char*)a_sPathName.c_str());
	_bstr_t			a_bTStr;
	a_bTStr			= "DATABASE";
	a_ipPropertySet->SetProperty(a_bTStr, aVar_Dir);
	// ���[�N�X�y�[�X�̃I�[�v��
	a_ipWSFactory->Open(a_ipPropertySet, 0, &a_ipSWSpace);
	if(a_ipSWSpace == NULL) {
		PrnMsg( e_fErr_Fp, "%d\t���[�N�X�y�[�X�i�f�B���N�g���j�̃I�[�v���Ɏ��s���܂���\n", e_iCurMeshCode);
		return	( false );
	}
	// �t�B�[�`���N���X�̃I�[�v��
	IFeatureWorkspacePtr	ipFWorkspace(a_ipSWSpace);
	IFeatureClassPtr		ipFClass;
	_bstr_t					a_bFCStr;
	a_bFCStr	= (char*)a_sShpName.c_str();
	ipFWorkspace->OpenFeatureClass(a_bFCStr, &ipFClass);
	if(ipFClass != NULL) {
		PrnMsg( e_fErr_Fp, "%d\t���łɓ�����shape�t�@�C�������݂��Ă��܂�\n", e_iCurMeshCode);
		a_ipSWSpace == NULL;
		return	( false );
	}

	IFieldsPtr				ipFields(CLSID_Fields);

	IGeometryDefPtr			ipGDef(CLSID_GeometryDef);
	IGeometryDefEditPtr		ipGDefEdit(ipGDef);

	// �i�[����Shape�̃W�I���g���^�C�v��ݒ�
	if( FAILED(ipGDefEdit->put_GeometryType(esriGeometryPolygon)) ) {
		PrnMsg( e_fErr_Fp, "%d\t�W�I���g���^�C�v�̐ݒ�Ɏ��s\n", e_iCurMeshCode);
		return	( false );
	}
	// ��ԎQ�Ƃ�ݒ肵�Ȃ���prj�t�@�C�����o�͂���Ȃ�
	if( FAILED (ipGDefEdit->putref_SpatialReference(c_ipSpRef)) ) {
		PrnMsg( e_fErr_Fp, "%d\t��ԎQ�Ƃ̕t�^�Ɏ��s\n", e_iCurMeshCode);
		return	( false );
	}

	// �W�I���g��(Shape)�E�t�B�[���h�ǉ�
	//-------------------------------------------------------------
	IFieldsEditPtr	ipFieldsEdit(ipFields);
	IFieldPtr		ipField(CLSID_Field);

	IFieldEditPtr	ipFieldEdit1(ipField);
	if( FAILED(ipFieldEdit1->put_Name(T2BSTR(_T("Shape")))) ) {
		PrnMsg( e_fErr_Fp, "%d\tShape�̖��O�̐ݒ�Ɏ��s\n", e_iCurMeshCode);
		return	( false );
	}
	if( FAILED(ipFieldEdit1->put_Type(esriFieldTypeGeometry)) ) {
		PrnMsg( e_fErr_Fp, "%d\tShape�̃^�C�v�̐ݒ�Ɏ��s\n", e_iCurMeshCode);
		return	( false );
	}
	if( FAILED( ipFieldEdit1->put_IsNullable(VARIANT_FALSE)) ) {
		PrnMsg( e_fErr_Fp, "%d\tShape��NULL�񋖗e�̐ݒ�Ɏ��s\n", e_iCurMeshCode);
		return	( false );
	}
	if( FAILED(ipFieldEdit1->putref_GeometryDef(ipGDefEdit)) ) {
		PrnMsg( e_fErr_Fp, "%d\tShape�̃W�I���g����`�̐ݒ�Ɏ��s\n", e_iCurMeshCode);
		return	( false );
	}

	if(ipFieldsEdit->AddField(ipFieldEdit1) != S_OK) {
		PrnMsg( e_fErr_Fp, "%d\tShape�̃t�B�[���h�ǉ��Ɏ��s\n", e_iCurMeshCode);
		return false;
	}

	IFieldPtr		ipField3(CLSID_Field);
	IFieldEditPtr	ipFieldEdit3(ipField3);
	if( FAILED(ipFieldEdit3->put_Name(T2BSTR(_T("NPINFO_C")))) ) {
		PrnMsg( e_fErr_Fp, "%d\tNPINFO_C�̖��O�̐ݒ�Ɏ��s\n", e_iCurMeshCode);
		return	( false );
	}
	if( FAILED(ipFieldEdit3->put_Type(esriFieldTypeInteger)) ) {
		PrnMsg( e_fErr_Fp, "%d\tNPINFO_C�̃^�C�v�̐ݒ�Ɏ��s\n", e_iCurMeshCode);
		return	( false );
	}
	if( FAILED(ipFieldEdit3->put_IsNullable(VARIANT_FALSE)) ) {
		PrnMsg( e_fErr_Fp, "%d\tNPINFO_C��NULL�񋖗e�̐ݒ�Ɏ��s\n", e_iCurMeshCode);
		return	( false );
	}
	// [Arc10.3.1�Ή�] Shape�t�B�[���h�����ّΉ�
	//   Arc10.1�܂Ńt�B�[���h���ݒ�͎��{���Ă��炸�AInteger�t�B�[���h�̓f�t�H���g����9�ɐݒ肳��Ă����B
	//   Arc10.3.1����d�l�ύX�ɂ��AInteger�t�B�[���h�̃f�t�H���g����9��10�ɕύX���ꂽ���߁A
	//   Integer�t�B�[���h���ݒ�ɗL����put_Precision()�ɂ�Arc10.1�܂łƓ����l�ɂȂ�t�B�[���h�����w�肵�Đݒ肷��B
	if (FAILED(ipFieldEdit3->put_Precision(9))) {
		PrnMsg(e_fErr_Fp, "%d\tNPINFO_C�̃t�B�[���h���̐ݒ�Ɏ��s\n", e_iCurMeshCode);
		return	(false);
	}

	if(ipFieldsEdit->AddField(ipFieldEdit3) != S_OK) {
		PrnMsg( e_fErr_Fp, "%d\tNPINFO_C�̃t�B�[���h�ǉ��Ɏ��s\n", e_iCurMeshCode);
		return	( false );
	}

	//-------------------------------------------------------------

	_bstr_t		a_bSName, a_bGeoFName;
	a_bSName	= (char*)a_sShpName.c_str();
	a_bGeoFName	= "Shape";

	if( FAILED(ipFWorkspace->CreateFeatureClass(a_bSName, ipFields,
				NULL, NULL, esriFTSimple, a_bGeoFName, CComBSTR(""), &ipFClass)) ) {
		PrnMsg( e_fErr_Fp, "%d\t�t�B�[�`���N���X�̐����Ɏ��s\n", e_iCurMeshCode );
		return	( false );
	}

	IFeatureCursorPtr	ipFeatureCursor;
	if( FAILED(ipFClass->Insert(VARIANT_TRUE, &ipFeatureCursor)) ) {
		PrnMsg( e_fErr_Fp, "%d\t�t�B�[�`���N���X��Insert�J�[�\�������Ɏ��s\n", e_iCurMeshCode );
		return	( false );
	}

	// �����t�B�[���h�̃C���f�b�N�X���擾
	long	a_lFldIndx[6] = {0,0,0,0,0,0};
	_bstr_t	a_bFldStr[6];
	a_bFldStr[0]	= "NPINFO_C";
	
	long	roop = 0;
	for( roop = 0; roop < 1; roop++ )
	{
		if (FAILED(ipFClass->FindField(a_bFldStr[roop], &a_lFldIndx[roop]))) {
			PrnMsg( e_fErr_Fp, "%d\t�����t�B�[���h�̃C���f�b�N�X�擾�Ɏ��s\n", e_iCurMeshCode );
			return	( false );
		}
	}

	ims_data_info	a_iIndx;
	for( a_iIndx = c_mNP_Area_Data.begin(); a_iIndx != c_mNP_Area_Data.end(); a_iIndx++ )
	{
		IFeatureBufferPtr	ipFeatureBuffer;
		if( FAILED(ipFClass->CreateFeatureBuffer(&ipFeatureBuffer)) ) {
			PrnMsg( e_fErr_Fp, "%d\t�t�B�[�`���o�b�t�@�����Ɏ��s\n", e_iCurMeshCode );
			return	( false );
		}

		data_info	a_dCurDat;

		a_dCurDat	= *a_iIndx;

		// �����t�^
		ipFeatureBuffer->putref_Shape(a_dCurDat.e_ipGeo);
		if (FAILED(ipFeatureBuffer->put_Value(a_lFldIndx[0], CComVariant(a_dCurDat.e_iAttrCode))) ) {
			PrnMsg( e_fErr_Fp, "%d\t�����ǉ��Ɏ��s\n", e_iCurMeshCode);
			return	( false );
		}
	
		_variant_t va;
		if( FAILED(ipFeatureCursor->InsertFeature(ipFeatureBuffer, &va)) ) {
			PrnMsg( e_fErr_Fp, "%d\t�t�B�[�`���ǉ��Ɏ��s\n", e_iCurMeshCode );
			return	( false );
		}
	}
	ipFeatureCursor->Flush();

	return	( true );
}

/** ------------------------------------------------------------------------------
	�ϊ����s��
	�������ŃG���[�t�@�C���̏��������s��
	@return	����	true
	@return ���s	false
--------------------------------------------------------------------------------*/
bool	main_rootin::
conv_exe	( void )
{
	// �J�n���Ԃ̐ݒ�
	e_eExeTime.Set_Start();

	if( eExe_Flag & ON_PGDB_MODE ){

		// �p�[�\�i���W�IDB
		PrnMsg( stdout, "���Ԉᔽ����G���APGDB<%s>���I�[�v���E�E\n", e_cpPGDB_File ); 
		e_ipWorkspace_NP_AREA = DB_Open( e_cpPGDB_File );
		if( ! e_ipWorkspace_NP_AREA )	{
			PrnMsg( e_fErr_Fp, "�p�[�\�i���W�IDB���I�[�v���ł��܂���ł���\n"); 
			return	( false );
		}else {
			PrnMsg( stdout, "�ڑ�����\n");
		}

	}else {

		// SDE�@�����g�p
		e_sPrnInfo.Prn_Info	( "���Ԉᔽ�|���S���f�[�^�I�[�v���E�E\n", stdout );
		e_ipWorkspace_NP_AREA = DB_Open( e_cpInit_Str_NP_AREA );
		if( ! e_ipWorkspace_NP_AREA ) {
			e_sPrnInfo.Prn_Info	( "�ڑ��o���Ȃ�����\n", stderr );
			return	( false );
		} else {
			PrnMsg	( stdout, "�ڑ�����\n" );
		}	

	}

	// ���b�V���p��SDE
	e_sPrnInfo.Prn_Info	( "���b�V��DB�I�[�v���E�E\n", stdout );
	e_ipWorkspace_Mesh = DB_Open( e_cpInit_Str_MESH );
	if( ! e_ipWorkspace_Mesh ) {
		e_sPrnInfo.Prn_Info	( "�ڑ��o���Ȃ�����\n", stderr );
		return	( false );
	} else {
		PrnMsg	( stdout, "�ڑ�����\n" );
	}	

	// DB�ڑ��ɂ����������Ԃ̕\��
	PrnMsg	( e_fErr_Fp,	"SiNDY-DB�ڑ��������� = %0.2f(s)�ł���\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );

	// ���b�V���ǂݍ���
	if( !Read_Mesh_List() ) {
		e_sPrnInfo.Prn_Info	( "���b�V���ǂݍ��ݎ��s\n");
		return	( false );
	}

	// ���C���I�[�v��
	if( !Open_Mesh_Layers() ) {
		e_sPrnInfo.Prn_Info	( "���b�V�����C���I�[�v���Ɏ��s���Ă��܂�\n", stderr );
		return	( false );
	}

	if( !Open_Layers ( ) ){	// �n�}�p���[�N�X�y�[�X
		e_sPrnInfo.Prn_Info	( "���Ԉᔽ����G���A���C���I�[�v���Ɏ��s���Ă��܂�\n", stderr );
		return	( false );
	}

	// �f�[�^�ǂݍ���
	if( !Read_NP_AREA_to_Shape ( ) ) {
		e_sPrnInfo.Prn_Info	( "�f�[�^�����Ɏ��s���Ă��܂�\n", stderr );
		return	( false );
	}

	//�@���s���I��
	// �I�����Ԃ̎擾
	PrnMsg	( e_fErr_Fp,	"�f�[�^�`�F�b�N������ = %0.2f(s)�ł���\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );
	return	( true );
}

/** ------------------------------------------------------------------------------
	���b�V�����X�g�̓ǂݍ��ݐݒ�
	@return	true	����
	@return	false	���s
--------------------------------------------------------------------------------*/
bool	main_rootin::
Read_Mesh_List ( void )
{
	char*			a_cpList_File	= NULL;		//	���X�g�t�@�C����
	if( eExe_Flag & ON_MESH || eExe_Flag & ON_MONO_MESH ) {		//	���b�V���R�[�h���X�g�w��̏ꍇ

		if( eExe_Flag & ON_MESH ) {
			a_cpList_File	= Ret_Mesh_File ( );
			if( a_cpList_File == NULL ) {
				PrnMsg	( stderr, "���b�V���R�[�h���X�g�t�@�C�������Ƃ肾���܂���ł���[check_exe]\n");
				return	( false );
			}	
			if( e_sInput_List.read_mesh_list ( a_cpList_File ) != 0 ) {	// �����Ń��X�g��ǂݍ���
				PrnMsg	( stderr, "���b�V���R�[�h���X�g�t�@�C��<%s>�̓ǂݍ��݂Ɏ��s[check_exe]\n", a_cpList_File );
				return	( false );
			}
		}else {
			if( e_sInput_List.read_mesh_list ( e_iMeshCode ) != 0 ) {	// �����Ń��b�V����ǂݍ���
				PrnMsg( e_fErr_Fp, "���b�V��<%d>�̐ݒ�Ɏ��s[check_exe]\n", e_iMeshCode );
				return	( false );
			}
		}

	}
	return	( true );
}
