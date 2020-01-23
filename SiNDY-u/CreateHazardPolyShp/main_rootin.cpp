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

#include <atlstr.h>
#include "..\..\LIB\WinLib\VersionInfo.h"
#include "PointRotate.h"
#include <sindy/workspace.h>


/** ------------------------------------------------------------------------------
	SiNDYDB�ւ̐ڑ��I�[�v��
	@return	true	����
	@return	false	���s
--------------------------------------------------------------------------------*/
bool	main_rootin::
SiNDY_DB_Open	(	IWorkspacePtr&			c_ipWorkspace_Note	)
{

	// ���L�pDB
	m_sPrnInfo.Prn_Info			( "#�ЊQ�|���S���f�[�^�F\t", stdout );
	if( !DB_Open	( c_ipWorkspace_Note,
					  m_pTool_Init_NP.get_UserName(), m_pTool_Init_NP.get_Password(),
					  m_pTool_Init_NP.get_Version(), m_pTool_Init_NP.get_Instance(),
					  m_pTool_Init_NP.get_Server() )) {
		m_sPrnInfo.Prn_Info	( "�ڑ��o���Ȃ�����\n", stderr );
		return	( false );
	}else {
		PrnMsg	( stdout, "�ڑ�����\n" );
	}	
	return	( true );
}

/** ------------------------------------------------------------------------------
	DB�ւ̐ڑ��I�[�v��
	@return	true	����
	@return	false	���s
--------------------------------------------------------------------------------*/
bool	main_rootin::
DB_Open	(	IWorkspacePtr&			c_ipWorkspace,
			char*					c_cpUserName,		///< ���[�U��
			char*					c_cpPassword,		///< �p�X���[�h
			char*					c_cpVersion,		///< �o�[�W����
			char*					c_cpInstance,		///< �C���X�^���X
			char*					c_cpServer )		///< �T�[�o��
{
	CString csUserName = c_cpUserName;
	CString csPassword = c_cpPassword;
	CString csVersion  = c_cpVersion;
	CString csInstance = c_cpInstance;
	CString csServer   = c_cpServer;
	CString csConnStr;

	// �ڑ�������쐬
	csConnStr.Format(_T("%s/%s/%s/%s/%s"), csUserName, csPassword, csVersion, csInstance, csServer);

	char	a_cpPrnMsg[512];

	sprintf	( a_cpPrnMsg,	"#�ڑ��v���p�e�B�F%s/%s/%s/%s/%s\n",
							c_cpUserName, c_cpPassword, c_cpVersion, c_cpInstance, c_cpServer );

//	printf	( "%s", a_cpPrnMsg );

	m_sPrnInfo.Prn_Info			( a_cpPrnMsg, stdout );

	// DB�֐ڑ�
	printf("�ڑ���...");
	c_ipWorkspace = NULL;
	c_ipWorkspace = sindy::create_workspace(csConnStr);
	if (!c_ipWorkspace) {
		return	( false );
	}
	return	(true);
}

/** ------------------------------------------------------------------------------
	�G���[���O�t�@�C�����I�[�v������
	@return	true	�I�[�v������
	@return false	�I�[�v�����s
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Err_Log	( void )
{
	if( m_sLog_File == "" ) {
		return	( true );
	}else {
		m_fErr_Fp	= fopen ( (char*)m_sLog_File.c_str(), "w" );
		if( m_fErr_Fp == NULL ) {
			PrnMsg	( stderr, "�G���[���O�t�@�C��<%s>���I�[�v���ł��Ȃ�[main_rootin]\n", m_sLog_File.c_str() );
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
		if(strcmp(argv[i], "-hpi") == 0) {	// �ЊQ�G���A���������
			if(argc < i+2) return( false );
			m_sInit_Str_HZ_AREA	= argv[i+1];
			a_exeflag			|= ON_INIT_HZ_AREA;
			a_optcount[0]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-hpf") == 0) {	// �ЊQ�G���A�t�B�[�`���N���X�w��
			if(argc < i+2) return( false );
			m_sHZ_AREA_FC		= argv[i+1];
			a_exeflag			|= ON_FC_HZ_AREA;
			a_optcount[1]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-mi") == 0) {	// �ЊQ�G���A���������
			if(argc < i+2) return( false );
			m_sInit_Str_MESH	= argv[i+1];
			a_exeflag			|= ON_INIT_MESH;
			a_optcount[2]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-mf") == 0) {	// ���b�V���t�B�[�`���N���X�w��
			if(argc < i+2) return( false );
			m_sMesh_FC			= argv[i+1];
			a_exeflag			|= ON_FC_MESH;
			a_optcount[4]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-l") == 0) { // ���O�t�@�C���ݒ�i�ݒ肪�����ꍇ�́A�G���[��stderr�֐��ꗬ���ɂȂ�j
			if(argc < i+2) return( false );
			m_sLog_File	= argv[i+1];	///< ���b�V���R�[�h���X�g�t�@�C��
			a_exeflag		|= ON_LOGFILE;
			a_optcount[5]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-v") == 0) {
			print_version();
			a_optcount[6]++;
			i+=1;
			return	( false );	// ���ɂ�������H
		}else
		if(strcmp(argv[i], "-m") == 0) { // 2002.9.24
			if(argc < i+2) return( false );
			m_sMeshCode_File	= argv[i+1];	///< ���b�V���R�[�h���X�g�t�@�C��
			a_exeflag		|= ON_MESH;
			a_optcount[7]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-M") == 0) { // 2003.4.1
			if(argc < i+2) return( false );
			a_exeflag		|= ON_MONO_MESH;
			m_iMeshCode		= atoi(argv[i+1]);	
			a_optcount[8]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-pgdb") == 0) {
			a_exeflag		|= ON_PGDB_MODE;
			m_sPGDB_File	= argv[i+1];
			a_optcount[9]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-o") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_OUTPATH;
			m_sOutPath		= argv[i+1];	
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

	if( !(a_exeflag & ON_FC_HZ_AREA) ) {		///< ���L�t�B�[�`���N���X
		PrnMsg	(stderr, "�ЊQ�G���A�̃t�B�[�`���N���X���w�肳��Ă��܂���\n");
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
		PrnMsg	(stderr, "�ЊQ�G���A��PGDB�t�@�C�����w�肳��Ă��܂���\n");
		return	( false );
	}
	// �������ݒ�
//	if( !e_pTool_Init_Note.read_init_str ( e_cpInit_Str_HZ_AREA ) ) {
//		return	( false );	// ���b�Z�[�W�͓����ŏo���Ă���
//	}

	// �G���[���O�t�@�C���̃I�[�v��	2006.05.17 ���s�����o�͂������̂ł����ֈړ�
	if( !Open_Err_Log () ) {
		return	( false );
	}else {
		if( !m_sPrnInfo.Init(m_fErr_Fp) ) {
			return	( false );
		}
	}

	//--------------------------------------------------------------------------
	// ���s�����o�� 2009.09.30 �ǉ�
	PrnMsg( m_fErr_Fp, "#�c�[���o�[�W����                               : <%s>\n", m_sVersion.c_str() );
	if(a_exeflag & ON_PGDB_MODE) {
	PrnMsg( m_fErr_Fp, "#PGDB�t�@�C��                                   : <%s>\n", m_sPGDB_File.c_str() );
	}
	PrnMsg( m_fErr_Fp, "#�ЊQ�G���A�t�B�[�`���N���X                     : <%s>\n", m_sHZ_AREA_FC.c_str() );
	PrnMsg( m_fErr_Fp, "#���b�V���t�B�[�`���N���X�pDB�ڑ����           : <%s>\n", m_sInit_Str_MESH.c_str() );
	PrnMsg( m_fErr_Fp, "#���b�V���t�B�[�`���N���X                       : <%s>\n", m_sMesh_FC.c_str() );
	if( a_exeflag & ON_MESH ) {
	PrnMsg( m_fErr_Fp, "#�ϊ��Ώۃ��b�V�����X�g                         : <%s>\n", m_sMeshCode_File.c_str() );
	}else {
	PrnMsg( m_fErr_Fp, "#�ϊ��Ώۃ��b�V���R�[�h                         : <%02d>\n", m_iMeshCode );
	}
	PrnMsg( m_fErr_Fp, "#�f�[�^�o�̓p�X                                 : <%s>\n", m_sOutPath.c_str() );
	//--------------------------------------------------------------------------

	if( !m_pTool_Init_Mesh.read_init_str ( (char*)m_sInit_Str_MESH.c_str() ) ) {
		return	( false );	// ���b�Z�[�W�͓����ŏo���Ă���
	}

	m_iExe_Flag	= a_exeflag;

	return		( true );
}

/** ------------------------------------------------------------------------------
// �g�p�@�\��
--------------------------------------------------------------------------------*/
void		main_rootin::
print_usage	(char **argv)
{
	PrnMsg(	stdout,
				"=== �y�ЊQ�G���A�|���S��Shape�ϊ��c�[���z(%s) ===\n"
				"<usage>\n"
				"%s <�I�v�V����>... \n"
				"\t<����>\n"
				"\t      '-m'           : ���b�V�����X�g�t�@�C�������w��(ex. -m index.base)\n"
				"\t      '-M'           : �P���b�V�����w��(ex. -M 594141)\n"
				"\t      '-v'           : �o�[�W�������\��\n"
				"\t      '-hpi'         : �ЊQ�G���A�|���S�����C���ڑ���� ex) -hpi ronly/ronly/SDE.DEFAULT/ruby2\n"
				"\t      '-hpf'         : �ЊQ�G���A�t�B�[�`���N���X�w�� ex) -hpf HPAREA.City_Site\n"
				"\t      '-mi'          : ���b�V���t�B�[�`���N���X�ڑ���� ex) -mi ronly/ronly/SDE.DEFAULT/ruby2\n"
				"\t      '-mf'          : ���b�V���t�B�[�`���N���X�w��  ex) -mf REFERENCE.CityMesh\n"
				"\t      '-l'           : ���O�t�@�C�������w�� ex) -l .\\test\\make.log\n"
				"\t      '-pgdb'        : PGDB�t�@�C�������w�� ex) -pgdb .\\data\\test.mdb\n"
				"\t      '-o'           : �f�[�^�o�̓p�X ex) -pgdb .\\data\\Citymap\n"
				"\t                       ���w�肵���f�[�^�p�X�ȉ��Ƀ��b�V���̃t�H���_�������쐬���o�͂���B\n"
				"\t<�⑫>\n"
				"\t      �E'-m','-M'�͓����w��ł��܂���\n"
			, m_sVersion.c_str(), argv[0]);
}

/** ------------------------------------------------------------------------------
	�o�[�W�������̕\��
	@return		����
---------------------------------------------------------------------------------*/
void	main_rootin::
print_version	( void )
{
	PrnMsg(	stdout,
			"=== �y�ЊQ�G���A�|���S��Shape�ϊ��c�[���z(%s) ===\n"
				"\t<�o�[�W�������>\n"
				"\t2011/05/13        1.0.0.1            : �V�K�쐬\n", m_sVersion.c_str()
			);
}

/** ------------------------------------------------------------------------------
	���s�����o�͂��� 2006.05.17
	@return	char*
--------------------------------------------------------------------------------*/
void	main_rootin::
Print_Check_Info	( void )
{
//	sprintf	( a_cpPrnMsg,	"#�s�s�n�}���L�����͈�	%0.2f(m)\n", e_dRange_City );
//	m_sPrnInfo.Prn_Info	( a_cpPrnMsg, stdout );
//	m_sPrnInfo.Prn_Info			( a_cpPrnMsg, stdout );
}

/** ------------------------------------------------------------------------------
	���b�V����`��DB����l�����郁�\�b�h�̏�����
	��get_mesh_rect �����O�ɕK���Ă�
------------------------------------------------------------------------------==*/

bool	main_rootin::
get_mesh_init	(	mk_input_list&	c_mMList,			// ���̓��X�g�N���X
					IWorkspacePtr	c_ipWorkSpace	)	// �w�肷�郏�[�N�X�y�[�X
{

	if( m_iExe_Flag & ON_MESH ) {
		if( m_sMeshCode_File == "" ) {
			PrnMsg( m_fErr_Fp, "���b�V���R�[�h���X�g�t�@�C�������Ƃ肾���܂���ł���[check_exe]\n");
			return	( false );
		}	
		if( c_mMList.read_mesh_list ( (char*)m_sMeshCode_File.c_str() ) != 0 ) {	// �����Ń��X�g��ǂݍ���
			PrnMsg( m_fErr_Fp, "���b�V���R�[�h���X�g�t�@�C��<%s>�̓ǂݍ��݂Ɏ��s[check_exe]\n", m_sMeshCode_File.c_str() );
			return	( false );
		}
	}else {
		if(c_mMList.read_mesh_list ( m_iMeshCode ) == -1) {
			PrnMsg( m_fErr_Fp, "���b�V��<%d>�̐ݒ�Ɏ��s���܂���\n", m_iMeshCode );
			return	( false );
		}
	}

	int			a_iMDigit		= 0;
	a_iMDigit	= c_mMList.get_mesh_digit ();

	IFeatureWorkspacePtr	a_F_Workspace	( c_ipWorkSpace );	// �t�B�[�`�����[�N�X�y�[�X

	// ���C���̃I�[�v��
	if( FAILED(a_F_Workspace->OpenFeatureClass(CComBSTR((char*)m_sMesh_FC.c_str()), &m_ipF_Class )) ) {
		PrnMsg( m_fErr_Fp, "���C�� <%s> �̃I�[�v���Ɏ��s\n", m_sMesh_FC.c_str());
		return	(false);
	}else {
		PrnMsg( stdout,	"<%s> ���C�����I�[�v��\n", m_sMesh_FC.c_str());
	}

	// 2002.12.09 �f�[�^�����镔���̃��b�V�����X�g
	m_iDigit	= a_iMDigit;

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
	m_ipF_Class->get_ShapeFieldName	( &a_SpatialCol );
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
		m_iDigit	= a_iDigit;
		break;
	default:
		return	( NULL );
		break;
	}

	a_Where_Str	= a_tmpstr;		// string��BSTR�ɕϊ�

	IQueryFilterPtr		a_Sp_Filter	( CLSID_QueryFilter );			// ��Ԍ����t�B���^
//	a_Sp_Filter->put_SearchOrder	( esriSearchOrderAttribute );	// ���������D��
	a_Sp_Filter->put_WhereClause	( a_Where_Str);					// Where��̐ݒ�
	
	// �N���b�v��`�쐬
	IEnvelopePtr	a_ipMesh_Rect;	// ���b�V����`
	a_ipMesh_Rect	= NULL;

	// �T�[�`���Ă���
	IFeatureCursorPtr	a_F_Cursor;
	if( FAILED	(m_ipF_Class->Search	( a_Sp_Filter, VARIANT_FALSE, &a_F_Cursor )) ) {
		PrnMsg	(m_fErr_Fp, "%d\t�f�[�^�̑��������Ɏ��s\n", c_iMeshCode);
		return	(NULL);
	}else {
		IFeaturePtr				a_Feature;
		while(a_F_Cursor->NextFeature ( &a_Feature ) == S_OK)
		{				
			IGeometryPtr		a_Clip_Shp;		// �����Ƀf�[�^���R�s�[���ăN���b�v����

			if( FAILED(a_Feature->get_ShapeCopy	( &a_Clip_Shp )) ) {	// �t�B�[�`������
				PrnMsg( m_fErr_Fp, "%d\t�V�F�C�v�̃R�s�[�Ɏ��s\n", c_iMeshCode);
				continue;
			}
			
			VARIANT_BOOL	a_bResult;
			a_Clip_Shp->get_IsEmpty( &a_bResult );
			if(a_bResult == VARIANT_TRUE) {	// �N���b�v���ʂ�NULL�Ȃ烁�b�V���O
				continue;
			}
			if( FAILED(a_Clip_Shp->get_Envelope( &a_ipMesh_Rect) )) {
				PrnMsg( m_fErr_Fp, "�Y�����b�V��<%d>�̋�`���݂���܂���ł���\n", c_iMeshCode);
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
Get_Rect_Polygon	(	IEnvelopePtr	c_ipEnv	)		///< ���b�V����`
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

	// �V���v���t�@�C��������
	ITopologicalOperator2Ptr	a_ipTopo;

	a_ipTopo	= a_ipRectPoly;

	if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
		PrnMsg( m_fErr_Fp, "%d\tSimplify�Ɏ��s\n", m_iCurMeshCode);
		return	( NULL );
	}
	if(FAILED ( a_ipRectPoly->SimplifyPreserveFromTo() ) ) {
		PrnMsg( m_fErr_Fp,	"%d\tSimplifyPreserveFromTo�Ɏ��s", m_iCurMeshCode );	// �G���[���b�Z�[�W
		return	( NULL );
	}
	return ( a_ipRectPoly );
}

/** ------------------------------------------------------------------------------
	���C���I�[�v��
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Layers		( void )
{
	if( !m_mHZ_AREA_WS.Init ( m_ipWorkspace_HZ_AREA, (char*)m_sHZ_AREA_FC.c_str() ) ) {
		PrnMsg( stderr, "�ЊQ�G���A�t�B�[�`���N���X<%s>�̓ǂݍ��݂Ɏ��s\n", m_sHZ_AREA_FC.c_str() );
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
	a_iDigit	= m_cCrd.GetMeshDigit ( c_iMeshCode );

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
Read_HZ_AREA_to_Shape	( void )
{
	IFeatureCursorPtr	a_ipHZ_Area_Cursor;

	// �L���b�V����ɂƂ��Ă���
	// �I���������e�X�g 2002.6.24
	int		a_iCur_Mesh;
	for( a_iCur_Mesh = m_sInput_List.get_mesh_first(); a_iCur_Mesh != -1; a_iCur_Mesh = m_sInput_List.get_mesh_next() )
	{
		// ���b�V�����ƂɃL���b�V���}�l�[�W�����N���A���Ă���
/*	
		hr	= a_ipSpCacheMgr_CNote->EmptyCache( );
		if( FAILED ( hr ) ) {
			fprintf	( stderr, "�s�s�n�}���L�p�L���b�V���}�l�[�W�����N���A�ł��܂���ł���\n");
			continue;
		}
*/
		PrnMsg( stdout, "���b�V��[%d] �f�[�^�擾\n", a_iCur_Mesh );

		m_iCurMeshCode	= a_iCur_Mesh;

		// ���b�V����`�̎��o��
		IGeometryPtr	a_ipMeshRect;
		a_ipMeshRect	= get_mesh_rect( a_iCur_Mesh );
		if( a_ipMeshRect == NULL ) {	// ���b�V����`�������ꍇ
			PrnMsg( m_fErr_Fp, "%d\t�̃��b�V����`��������܂���ł���\n", a_iCur_Mesh );
			continue;
		}
		
		IEnvelopePtr	a_ipRect;
		a_ipMeshRect->get_Envelope ( &a_ipRect );

		// ��ԎQ�Ƃ��擾���� 2007.08.22
		ISpatialReferencePtr	a_ipSpRef;
		if( FAILED (a_ipRect->get_SpatialReference ( &a_ipSpRef )) ) {
			PrnMsg( m_fErr_Fp, "%d\t��ԎQ�Ƃ̎擾�Ɏ��s\n", a_iCur_Mesh );
			continue;
		}

		// ��ԃL���b�V���}�l�[�W���Ƀ��b�V�����̃f�[�^���Ƃ肾��
//		a_ipSpCacheMgr_CNote->FillCache( a_ipRect );
			
		a_ipHZ_Area_Cursor	= m_mHZ_AREA_WS.Search_Feature ( a_ipRect, esriSpatialRelIntersects, "", RET_FCURSOR );
		if ( a_ipHZ_Area_Cursor == NULL ) {
			m_sPrnInfo.Prn_Info	( "�ЊQ�|���S���f�[�^���擾�ł��܂���ł���\n", stderr );
			return	( false );
		}
		PrnMsg( stdout, "���b�V���R�[�h<%d> �ЊQ�|���S���f�[�^�ǂݍ��ݒ�\n", a_iCur_Mesh );
		Set_HZ_AREA_Data	( a_ipMeshRect, a_ipHZ_Area_Cursor, m_mAll_Data, CITY_NOTE );

		// �o�̓t�@�C�����쐬
		std::string	a_sOutFName;
		int	a_iMkLv	= 0;
		switch (m_iDigit)
		{
		case	2:	a_iMkLv	= 1; break;
		case	4:	a_iMkLv	= 1; break;
		case	6:	a_iMkLv	= 2; break;
		case	8:	a_iMkLv	= 3; break;
			break;
		}
		std::string	a_sOutPath;
		a_sOutFName	= make_mesh_fname ( a_iCur_Mesh, a_iMkLv, (char*)m_sOutPath.c_str(), "_DIS.shp", a_sOutPath );
		// �f�[�^�o�̓p�X���쐬
		if( m_cUC.makedir ( (char*)a_sOutPath.c_str() ) == false ) {
			PrnMsg( m_fErr_Fp, "%d\t�f�B���N�g���쐬�Ɏ��s<%s>\n", a_iCur_Mesh, (char*)a_sOutPath.c_str() );
			continue;
		}

		HZ_Area_to_Shape (	a_ipSpRef, m_mAll_Data, (char*)a_sOutFName.c_str() );

		m_mAll_Data.clear();
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
			PrnMsg( m_fErr_Fp, "%d\tIntersect���s\n", m_iCurMeshCode);
			return	( NULL );
		}
	}else {
		if( FAILED( a_ipClip_Topo->Difference (c_ipClipPoly, &a_ipCliped_Shp)) ) {
			PrnMsg( m_fErr_Fp, "%d\tDifference���s\n", m_iCurMeshCode);
			return	( NULL );
		}
	}

	VARIANT_BOOL	a_bResult;
	a_ipCliped_Shp->get_IsEmpty( &a_bResult );
	if(a_bResult == VARIANT_TRUE) {	// �N���b�v���ʂ�NULL�Ȃ烁�b�V���O
		*c_ipEmpty_F	= 1;	
	//	fprintf( m_fErr_Fp,"�N���b�v���ʂ���\n");
		return	( NULL );
	}

	// ��ԎQ�Ƃ��擾���� 2007.08.22
	ISpatialReferencePtr	a_ipSpRef;
	if( FAILED (c_ipTgtGeo->get_SpatialReference ( &a_ipSpRef )) ) {
		PrnMsg( m_fErr_Fp, "%d\t��ԎQ�Ƃ̎擾�Ɏ��s\n", m_iCurMeshCode );
		return	( NULL );
	}

	// �V���v���t�@�C��������
	ITopologicalOperator2Ptr	a_ipTopo;

	a_ipTopo	= a_ipCliped_Shp;

//	IGeometryEnvironment2Ptr	a_ipGeoEnv(CLSID_GeometryEnvironment);
//	if( FAILED ( a_ipGeoEnv->put_Pre81Compatibility( VARIANT_TRUE ) ) ) {// add
//		PrnMsg( m_fErr_Fp, "put_Pre81Compatibility\n" );
//	
//	}
	if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
		PrnMsg( m_fErr_Fp, "%d\tSimplify�Ɏ��s\n", m_iCurMeshCode );
		return	( NULL );
	}

	IPolygonPtr	a_ipTmpPoly;
	a_ipTmpPoly = a_ipCliped_Shp;

	if(FAILED ( a_ipTmpPoly->SimplifyPreserveFromTo() ) ) {
		PrnMsg( m_fErr_Fp, "%d\tSimplify�Ɏ��s\n", m_iCurMeshCode );	// �G���[���b�Z�[�W
		return	( NULL );
	}
	// ��ԎQ�Ƃ̕t�^
	if( FAILED (a_ipCliped_Shp->putref_SpatialReference ( a_ipSpRef )) ) {
		PrnMsg( m_fErr_Fp, "%d\t��ԎQ�Ƃ̕t�^�Ɏ��s\n", m_iCurMeshCode );
		return	( NULL );
	}

	return ( a_ipCliped_Shp );
}

/** ------------------------------------------------------------------------------
	�ЊQ�G���A�f�[�^���Z�b�g�ɗ��Ƃ�
	@return	�f�[�^��
--------------------------------------------------------------------------------*/
int		main_rootin::
Set_HZ_AREA_Data ( IGeometryPtr&		c_ipMeshRect,		///< ���b�V����`
				   IFeatureCursorPtr&	c_ipNoteCursor, 	///< ���L�J�[�\��
				   ms_data_info&		c_mHZ_Area_Data,	///< �ЊQ�|���S���f�[�^
				   int					c_iData_Type	)	///< �f�[�^�^�C�v
{
	int	a_iCount	= 0;
	// �f�[�^���}���`�Z�b�g�̃f�[�^�ɗ��Ƃ�
	if( c_ipNoteCursor != NULL ) {
		IFeaturePtr	a_ipNote_Feat;
		// �������ꂽ�|���S���̃��[�v
		ms_data_info	a_mTmp_HZ_AREA_Data;
		while(c_ipNoteCursor->NextFeature ( &a_ipNote_Feat ) == S_OK)
		{
			data_info	a_dHZ_AREA_Data;
			Feature_All_Attrs	a_fNote_Attrs;
			a_fNote_Attrs.Init	( a_ipNote_Feat );

			Feature_Attr		a_fPolyID, a_fAttrCode;
			// �I�u�W�F�N�gID
			a_fPolyID			= a_fNote_Attrs.Get_Attr ( "OBJECTID" );	// OID
			a_fAttrCode			= a_fNote_Attrs.Get_Attr ( "BGCLASS_C" );	// ����

			a_dHZ_AREA_Data.e_iOID		= a_fPolyID.e_ccvAttr.intVal;
			a_dHZ_AREA_Data.e_iAttrCode	= a_fAttrCode.e_ccvAttr.intVal;

			// �������X��ȊO�̏ꍇ�̓G���[���o�͂��A�X�L�b�v����
			if( a_dHZ_AREA_Data.e_iAttrCode == 1 ) {
				a_dHZ_AREA_Data.e_iAttrCode	= 100;	// �Z���G���A�����ɂ���
			}else {
				PrnMsg( m_fErr_Fp, "%d\t%d\t�������͈͊O\t%d\n", m_iCurMeshCode, a_dHZ_AREA_Data.e_iOID, a_dHZ_AREA_Data.e_iAttrCode);
				continue;
			}
			
			// �����ŃN���b�v����@2007.10.11
			a_dHZ_AREA_Data.e_ipGeo		= a_fNote_Attrs.Ret_Geo();			// �W�I���g��

			//---test
		//	if( a_dHZ_AREA_Data.e_iOID == 1932 ) {
		//		ITopologicalOperator2Ptr	a_ipTopo(a_dHZ_AREA_Data.e_ipGeo);
		//		if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
		//			return	( NULL );
		//		}
		//		IPolygonPtr	a_iptmp(a_dHZ_AREA_Data.e_ipGeo);
		//		if( FAILED(a_iptmp->SimplifyPreserveFromTo()) ){
		//			return	( false);
		//		}
		//		Out_to_Shape (	a_dHZ_AREA_Data.e_ipGeo, ".\\testdata" );		///< �o�̓V�F�C�v�t�@�C����
		//		return	( false );
		//	}
			//---test kokomade

			// ���b�V���ŃN���b�v
			int				a_iEmpty_F	= 0;
			IGeometryPtr	a_ipCliped_Poly;
			a_ipCliped_Poly	= Mk_Clip_Data ( c_ipMeshRect, a_dHZ_AREA_Data.e_ipGeo, 0, &a_iEmpty_F );
			if( a_iEmpty_F == 1 ) {
				PrnMsg( m_fErr_Fp, "%d\t%d\t��ɂȂ��Ă܂�\n", m_iCurMeshCode, a_dHZ_AREA_Data.e_iOID);
				continue;
			}
			if( a_ipCliped_Poly == NULL ) {
				PrnMsg( m_fErr_Fp, "%d\t%d\tNULL�ɂȂ��Ă܂�\n", m_iCurMeshCode, a_dHZ_AREA_Data.e_iOID);
				continue;
			}
			
			// �N���b�v�f�[�^�ƍ����ւ�
			a_dHZ_AREA_Data.e_ipGeo	= a_ipCliped_Poly;	

			a_mTmp_HZ_AREA_Data.insert( a_dHZ_AREA_Data );
			a_iCount++;
		}
		
		// �N���b�v�����͂����ł��
		ims_data_info	a_iCIndx1;
		int	a_iSkip_F	= 0;
		for( a_iCIndx1 = a_mTmp_HZ_AREA_Data.begin(); a_iCIndx1 != a_mTmp_HZ_AREA_Data.end(); a_iCIndx1++ )
		{
			data_info	a_dSrcDat;
			a_dSrcDat	= *a_iCIndx1;
			a_iSkip_F	= 0;

			ims_data_info	a_iCIndx2;
			for( a_iCIndx2 = a_iCIndx1; a_iCIndx2 != a_mTmp_HZ_AREA_Data.end(); a_iCIndx2++ )
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
					PrnMsg( m_fErr_Fp, "%d\t%d\tDisjoint()�֌W�̎擾�Ɏ��s\n", m_iCurMeshCode, a_dSrcDat.e_iOID);
					continue;
				}else {
					if( a_vRetBool == VARIANT_TRUE ) {
						continue;
					}else {
						// �^�b�`���X�L�b�v
						if( FAILED(a_ipRel->Touches ( a_dTgtDat.e_ipGeo, &a_vRetBool )) ){
							PrnMsg( m_fErr_Fp, "%d\t%d\tTouches()�֌W�̎擾�Ɏ��s\n", m_iCurMeshCode, a_dSrcDat.e_iOID);
							continue;
						}else {
							if( a_vRetBool == VARIANT_TRUE ) {
								continue;
							}
						}
					}
				}
				//-------------------------------------------------------------------------------------
			//	int			e_iAttrCode;	///< �������ރR�[�h (914@(�ŏd�_)=2 915@(�d�_)=1)
			//	int			e_iGroupCode;
				// Tgt�� e_iAttrCode ���������d�_�x����̏ꍇ�A�d�_�x�������ꍇ�ł�����ID����̏ꍇ��Tgt��Src���N���b�v
				int	a_iClip_F	= 0;
				if( (a_dSrcDat.e_iAttrCode > a_dTgtDat.e_iAttrCode) ||
					((a_dSrcDat.e_iAttrCode == a_dTgtDat.e_iAttrCode) && (a_dSrcDat.e_iGroupCode < a_dTgtDat.e_iGroupCode)) )
				{
					int				a_iEmpty_F	= 0;
					IGeometryPtr	a_ipCliped_Poly;
					a_ipCliped_Poly	= Mk_Clip_Data ( a_dTgtDat.e_ipGeo, a_dSrcDat.e_ipGeo, 1, &a_iEmpty_F );
					if( a_iEmpty_F == 1 ) {
						PrnMsg( m_fErr_Fp, "%d\t%d\t��ɂȂ��Ă܂�\n", m_iCurMeshCode, a_dSrcDat.e_iOID);
						a_iSkip_F	= 1;
						break;
					}
					if( a_ipCliped_Poly == NULL ) {
						PrnMsg( m_fErr_Fp, "%d\t%d\tNULL�ɂȂ��Ă܂�\n", m_iCurMeshCode, a_dSrcDat.e_iOID);
						a_iSkip_F	= 1;
						break;
					}
					// �N���b�v�����f�[�^�Ɠ���ւ�
					a_dSrcDat.e_ipGeo	= a_ipCliped_Poly;
				}
			}
			// ��ɂȂ�ꍇ�̓X�L�b�v
			if( a_iSkip_F == 1 ) {
				continue;
			}

			// �f�[�^����ւ�
			IPolygonPtr		a_ipTmpPolygon(a_dSrcDat.e_ipGeo);
			a_ipTmpPolygon->SimplifyPreserveFromTo();

			PointRotate		a_pRotate;
			IGeometryPtr	a_ipRotatedGeo;
			a_ipRotatedGeo	= a_pRotate.ExecRotate ( a_dSrcDat.e_ipGeo );

			// �}���`�p�[�g�ɂȂ��Ă���|���S���𕪊�����
			msi_get_parts_info	a_mMP_Poly;
			a_mMP_Poly.Init ( a_ipRotatedGeo, a_dSrcDat.e_iOID, "DIS", m_iCurMeshCode, m_fErr_Fp );
			long	a_lPart_Num	= 0;
			a_lPart_Num	= a_mMP_Poly.Get_Part_Num();
			int i = 0;
			for( i = 0; i < a_lPart_Num; i++ )
			{
				IGeometryPtr	a_ipCurGeo;
				a_ipCurGeo	= a_mMP_Poly.Get_Geo_from_Part ( i+1 );
				data_info	a_dTmpDat;
				a_dTmpDat	= a_dSrcDat;
				a_dTmpDat.e_ipGeo	= a_ipCurGeo;
				c_mHZ_Area_Data.insert ( a_dTmpDat );
			}
		}
	}
	return	( a_iCount );
}

/** ------------------------------------------------------------------------------
	�ЊQ�G���A�f�[�^���V�F�[�v�t�@�C���ɕϊ�
	@return	bool
--------------------------------------------------------------------------------*/
bool	main_rootin::
HZ_Area_to_Shape (	ISpatialReferencePtr&	c_ipSpRef,			///< ��ԎQ�Ə��
					ms_data_info&			c_mHZ_Area_Data,	///< �ЊQ�G���A�f�[�^
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
		PrnMsg( m_fErr_Fp, "%d\t���[�N�X�y�[�X�i�f�B���N�g���j�̃I�[�v���Ɏ��s���܂���\n", m_iCurMeshCode);
		return	( false );
	}
	// �t�B�[�`���N���X�̃I�[�v��
	IFeatureWorkspacePtr	ipFWorkspace(a_ipSWSpace);
	IFeatureClassPtr		ipFClass;
	_bstr_t					a_bFCStr;
	a_bFCStr	= (char*)a_sShpName.c_str();
	ipFWorkspace->OpenFeatureClass(a_bFCStr, &ipFClass);
	if(ipFClass != NULL) {
		PrnMsg( m_fErr_Fp, "%d\t���łɓ�����shape�t�@�C�������݂��Ă��܂�\n", m_iCurMeshCode);
		a_ipSWSpace == NULL;
		return	( false );
	}

	IFieldsPtr				ipFields(CLSID_Fields);

	IGeometryDefPtr			ipGDef(CLSID_GeometryDef);
	IGeometryDefEditPtr		ipGDefEdit(ipGDef);
//	ISpatialReferencePtr	ipSpRef(CLSID_UnknownCoordinateSystem);

	// �i�[����Shape�̃W�I���g���^�C�v��ݒ�
	ipGDefEdit->put_GeometryType(esriGeometryPolygon);
	// ��ԎQ�Ƃ�ݒ肵�Ȃ���prj�t�@�C�����o�͂���Ȃ�
//	if( FAILED (ipGDefEdit->putref_SpatialReference(c_ipSpRef)) ) {
//		PrnMsg( m_fErr_Fp, "%d\t��ԎQ�Ƃ̕t�^�Ɏ��s\n", m_iCurMeshCode);
//		return	( false );
//	}

	// �W�I���g��(Shape)�E�t�B�[���h�ǉ�
	//-------------------------------------------------------------
	IFieldsEditPtr	ipFieldsEdit(ipFields);
	IFieldPtr		ipField(CLSID_Field);

	IFieldEditPtr	ipFieldEdit1(ipField);
	ipFieldEdit1->put_Name(T2BSTR(_T("Shape")));
	ipFieldEdit1->put_Type(esriFieldTypeGeometry);
	ipFieldEdit1->put_IsNullable(VARIANT_FALSE);
	ipFieldEdit1->putref_GeometryDef(ipGDefEdit);

	if(ipFieldsEdit->AddField(ipFieldEdit1) != S_OK) return false;

	IFieldPtr		ipField2(CLSID_Field);
	IFieldEditPtr	ipFieldEdit2(ipField2);
	ipFieldEdit2->put_Name(T2BSTR(_T("BGCLS_C")));
	ipFieldEdit2->put_Type(esriFieldTypeInteger);
	ipFieldEdit2->put_IsNullable(VARIANT_FALSE);

	// [Arc10.3.1�Ή�] Shape�t�B�[���h�����ّΉ�
	//   Arc10.1�܂Ńt�B�[���h���ݒ�͎��{���Ă��炸�AInteger�t�B�[���h�̓f�t�H���g����9�ɐݒ肳��Ă����B
	//   Arc10.3.1����d�l�ύX�ɂ��AInteger�t�B�[���h�̃f�t�H���g����9��10�ɕύX���ꂽ���߁A
	//   Integer�t�B�[���h���ݒ�ɗL����put_Precision()�ɂ�Arc10.1�܂łƓ����l�ɂȂ�t�B�[���h�����w�肵�Đݒ肷��B
	ipFieldEdit2->put_Precision(9);

	if(ipFieldsEdit->AddField(ipFieldEdit2) != S_OK) return false;
	//-------------------------------------------------------------

	_bstr_t		a_bSName, a_bGeoFName;
	a_bSName	= (char*)a_sShpName.c_str();
	a_bGeoFName	= "Shape";

	ipFWorkspace->CreateFeatureClass(a_bSName, ipFields,
		NULL, NULL, esriFTSimple, a_bGeoFName, CComBSTR(""), &ipFClass);

	IFeatureCursorPtr	ipFeatureCursor;
	ipFClass->Insert(VARIANT_TRUE, &ipFeatureCursor);

	// �����t�B�[���h�̃C���f�b�N�X���擾
	long	a_lFldIndx[6] = {0,0,0,0,0,0};
	_bstr_t	a_bFldStr[6];
	a_bFldStr[0]	= "BGCLS_C";
	
	long	roop = 0;
	for( roop = 0; roop < 1; roop++ )
	{
		if (FAILED(ipFClass->FindField(a_bFldStr[roop], &a_lFldIndx[roop]))) {
			PrnMsg( m_fErr_Fp, "%d\t�����t�B�[���h�̃C���f�b�N�X�擾�Ɏ��s\n", m_iCurMeshCode );
			return	( false );
		}
	}

	ims_data_info	a_iIndx;
	for( a_iIndx = c_mHZ_Area_Data.begin(); a_iIndx != c_mHZ_Area_Data.end(); a_iIndx++ )
	{
		IFeatureBufferPtr	ipFeatureBuffer;
		ipFClass->CreateFeatureBuffer(&ipFeatureBuffer);

		data_info	a_dCurDat;

		a_dCurDat	= *a_iIndx;

		// �����t�^
		ipFeatureBuffer->putref_Shape(a_dCurDat.e_ipGeo);
		if (FAILED(ipFeatureBuffer->put_Value(a_lFldIndx[0], CComVariant(a_dCurDat.e_iAttrCode))) ) {
			PrnMsg( m_fErr_Fp, "%d\t�����ǉ��Ɏ��s\n", m_iCurMeshCode);
		}
	
		_variant_t va;
		ipFeatureCursor->InsertFeature(ipFeatureBuffer, &va);
		//ipFeatureCursor->Flush();
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
	m_eExeTime.Set_Start();

	USES_CONVERSION;	// OLE2T ���̈ꕔ�̃}�N�����g�p����ꍇ�ɂ͕K���K�v

	if( m_iExe_Flag & ON_PGDB_MODE ){

		// �p�[�\�i���W�IDB�p�ł�
		IWorkspaceFactoryPtr	ipWorkspaceFactory	( CLSID_AccessWorkspaceFactory );
		IPropertySetPtr			ipPropertySet		( CLSID_PropertySet );					// �ڑ��v���p�e�B�[�i�[�p
		USES_CONVERSION;	// OLE2T ���̈ꕔ�̃}�N�����g�p����ꍇ�ɂ͕K���K�v
		// �J�����g�����ɃI�[�v�����Ă݂�

		_bstr_t			a_bOpenFName;	// T2BSTR����_bstr_t�ɕύX 2007.06.18
		a_bOpenFName	= m_sPGDB_File.c_str();
		PrnMsg( stdout, "#�ЊQ�G���APGDB<%s>���I�[�v���E�E\n", m_sPGDB_File.c_str() ); 
		ipWorkspaceFactory->OpenFromFile( a_bOpenFName, 0, &m_ipWorkspace_HZ_AREA );
		if( m_ipWorkspace_HZ_AREA == NULL )	{
			PrnMsg( m_fErr_Fp, "�p�[�\�i���W�IDB���I�[�v���ł��܂���ł���\n"); 
			return	( false );
		}else {
			PrnMsg( stdout, "�ڑ�����\n");
		}

	}else {

		if( !SiNDY_DB_Open	( m_ipWorkspace_HZ_AREA )) {
			m_sPrnInfo.Prn_Info("DB�֐ڑ��o���Ȃ�����\n", stderr );
			return	( false );
		}else {
			PrnMsg	( stdout, "�ڑ�����\n" );
		}
	}

	// ���b�V���p��
	if( !DB_Open	( m_ipWorkspace_Mesh,
					  m_pTool_Init_Mesh.get_UserName(), m_pTool_Init_Mesh.get_Password(),
					  m_pTool_Init_Mesh.get_Version(), m_pTool_Init_Mesh.get_Instance(),
					  m_pTool_Init_Mesh.get_Server() )) {
		m_sPrnInfo.Prn_Info	( "�ڑ��o���Ȃ�����\n", stderr );
		return	( false );
	}else {
		PrnMsg	( stdout, "�ڑ�����\n" );
	}	

	Print_Check_Info();	// 2006.05.17 �ǉ�

	// DB�ڑ��ɂ����������Ԃ̕\��
	PrnMsg	( m_fErr_Fp,	"#SiNDY-DB�ڑ��������� = %0.2f(s)�ł���\n",
						(double)((m_eExeTime.Get_From_Start()) / 1000.0) );

	if( !Read_Mesh_List() ) {
		m_sPrnInfo.Prn_Info	( "���b�V���ǂݍ��ݎ��s");
		return	( false );
	}

	// ���C���I�[�v��
	if( !Open_Layers ( ) ){	// �n�}�p���[�N�X�y�[�X
		m_sPrnInfo.Prn_Info	( "���C���I�[�v���Ɏ��s���Ă��܂�", stderr );
		return	( false );
	}

	// �f�[�^�ǂݍ���
	if( !Read_HZ_AREA_to_Shape ( ) ) {
		m_sPrnInfo.Prn_Info	( "�f�[�^�����Ɏ��s���Ă��܂�", stderr );
		return	( false );
	}

	//�@���s���I��
	// �I�����Ԃ̎擾
	PrnMsg	( m_fErr_Fp,	"#�f�[�^�`�F�b�N������ = %0.2f(s)�ł���\n",
						(double)((m_eExeTime.Get_From_Start()) / 1000.0) );
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
	if( m_iExe_Flag & ON_MESH || m_iExe_Flag & ON_MONO_MESH ) {		//	���b�V���R�[�h���X�g�w��̏ꍇ

		if( m_iExe_Flag & ON_MESH ) {
			if( m_sMeshCode_File == "" ) {
				PrnMsg	( stderr, "���b�V���R�[�h���X�g�t�@�C�������Ƃ肾���܂���ł���[check_exe]\n");
				return	( false );
			}	
			if( m_sInput_List.read_mesh_list ( (char*)m_sMeshCode_File.c_str() ) != 0 ) {	// �����Ń��X�g��ǂݍ���
				PrnMsg	( stderr, "���b�V���R�[�h���X�g�t�@�C��<%s>�̓ǂݍ��݂Ɏ��s[check_exe]\n", m_sMeshCode_File.c_str() );
				return	( false );
			}
		}else {
			if( m_sInput_List.read_mesh_list ( m_iMeshCode ) != 0 ) {	// �����Ń��X�g��ǂݍ���
				PrnMsg	( stderr, "���b�V���R�[�h���X�g�t�@�C��<%s>�̓ǂݍ��݂Ɏ��s[check_exe]\n", m_sMeshCode_File.c_str() );
				return	( false );
			}
		}
		if( !get_mesh_init( m_sInput_List , m_ipWorkspace_Mesh ) ) {	// ���b�V���̃f�[�^�͂ǂ���DB�ł��Q�Ƃł���͂��E�E�E
			PrnMsg	( stderr, "���b�V�����C���̊l���̏������Ɏ��s[check_exe]\n");
			return	( false );
		}
	}
	return	( true );
}
