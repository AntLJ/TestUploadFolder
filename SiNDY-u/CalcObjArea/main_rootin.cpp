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

//	2003.07.04	main_rootin.h�����`�𕪗�

/**
	DB�ւ̐ڑ��I�[�v��
	@return	true	����
	@return	false	���s
*/
bool	main_rootin::
DB_Open	(	IWorkspacePtr&			c_ipWorkspace	)
{
	USES_CONVERSION;	// OLE2T ���̈ꕔ�̃}�N�����g�p����ꍇ�ɂ͕K���K�v

	if( eExe_Flag & ON_PGDB_MODE ){	// 2007.03.02
		// �p�[�\�i���W�IDB�p�ł�
		IWorkspaceFactoryPtr	ipWorkspaceFactory	( CLSID_AccessWorkspaceFactory );
		IPropertySetPtr			ipPropertySet		( CLSID_PropertySet );					// �ڑ��v���p�e�B�[�i�[�p
		IWorkspacePtr			ipWorkspace;
		// �J�����g�����ɃI�[�v�����Ă݂�

		_bstr_t					a_bPGDG_FName;

		a_bPGDG_FName			= e_sConnect_Info.c_str();

		ipWorkspaceFactory->OpenFromFile( a_bPGDG_FName, 0, &ipWorkspace );
		if( ipWorkspace == NULL )	{
			fprintf ( stderr, "�p�[�\�i���W�IDB���I�[�v���ł��܂���ł���\n"); 
			return	( false );
		}else {
			printf(_T("�ڑ�����"));
			c_ipWorkspace			= ipWorkspace;
		}
		printf(_T("\n"));
		return	( true );
	}else {

		

		CString connectStr;
		connectStr.Format(_T("%s/%s/%s/%s/%s"),
			e_pTool_Init.get_UserName(), e_pTool_Init.get_Password(),
			e_pTool_Init.get_Version(), e_pTool_Init.get_Instance(), e_pTool_Init.get_Server());

		printf(_T(	"#�ڑ��v���p�e�B�F<%s>\n"),	connectStr);

		c_ipWorkspace = sindy::create_workspace(connectStr);

		// DB�֐ڑ�
		printf(_T("�ڑ���..."));
		if( !c_ipWorkspace) {
			return	( false );
		}else {
			return	( true );
		}
	}
}

/**
	�Z��DB�֌W�̃I�[�v��
	@return ����	true
	@return	���s	false
*/
bool	main_rootin::
Connect_to_Adm_DB	( void )	///< �ڑ�������[IN]
{
	fprintf( stdout,	"#�Z��DB�ڑ��v���p�e�B�F%s\n", e_sConnectAddrDB.c_str() );
	fprintf( stdout,	"#�ڑ���...");

	if( !e_aAddr_DB_Hndl.Init_DB ( (char*)e_sConnectAddrDB.c_str() ) ) {
		fprintf	( stderr, "#�T�[�r�X<%s>�ւ̐ڑ��Ɏ��s\n", e_sConnectAddrDB.c_str() );
		return	( false );
	}
	fprintf( stdout,	"�ڑ�����\n");
	return	( true );
}


/**
	�G���[���O�t�@�C�����I�[�v������
	@return	true	�I�[�v������
	@return false	�I�[�v�����s
*/
bool	main_rootin::
Open_Err_Log	( void )
{
	if( e_sLog_File == "" ) {
		return	( true );
	}else {
		e_fpErr_Fp	= fopen ( e_sLog_File.c_str(), "w" );
		if( e_fpErr_Fp == NULL ) {
			fprintf	( stderr, "�G���[���O�t�@�C��<%s>���I�[�v���ł��Ȃ�[main_rootin]\n", e_sLog_File.c_str() );
			return	( false );
		}
	}
	return	( true );
}

#define	OPTION_MAX	14
#define	BMAX		512

bool	main_rootin::
arg_check	(	int		argc,		// ������
				char	**argv	)	// ������
{
	int		a_optcount[OPTION_MAX];	// �I�v�V�����̏d���̃J�E���g
	int		i;
	int		a_exeflag	= 0;
//	int		digit		= 0;	// ���b�V���̌���

	//-----------------------------------------------------------------------
	// �c�[���o�[�W�����\��
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

	if(argc < 2) {
		return( false );
	}

//	char				*eMake_Type_Str		= NULL;

//	e_sInit_File		= argv[1];	///< �������t�@�C��
//	e_sCheck_Layer		= argv[2];	///< �`�F�b�N���C��

	// �I�v�V�������Ȃɂ������Ƃ�
	if( argc == 1 ) {
		return	( true );
	}

	// �I�v�V����������A���̂Ƃ���g�p���Ȃ�
	for(i = 1; i < argc;)
	{
		if(strcmp(argv[i], "-cnct") == 0) { // ���O�t�@�C���ݒ�i�ݒ肪�����ꍇ�́A�G���[��stderr�֐��ꗬ���ɂȂ�j
			if(argc < i+2) return( false );
			e_sConnect_Info		= argv[i+1];	///< �������t�@�C��
			a_exeflag			|= ON_DB_CNCT;
			a_optcount[0]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-fc") == 0) { // �`�F�b�N���C��
			if(argc < i+2) return( false );
			e_sCheck_Layer	= argv[i+1];	///< �������t�@�C��
			a_exeflag		|= ON_CHKLAYER;
			a_optcount[1]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-mf") == 0) { // ���b�V�����C��
			if(argc < i+2) return( false );
			e_sMesh_Layer	= argv[i+1];	///< �������t�@�C��
			a_exeflag		|= ON_MESHLAYER;
			a_optcount[2]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-l") == 0) { // ���O�t�@�C���ݒ�i�ݒ肪�����ꍇ�́A�G���[��stderr�֐��ꗬ���ɂȂ�j
			if(argc < i+2) return( false );
			e_sLog_File	= argv[i+1];		///< ���b�V���R�[�h���X�g�t�@�C��
			a_optcount[3]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-m") == 0) {	// 2002.9.24
			if(argc < i+2) return( false );
			e_sMeshCode_File	= argv[i+1];///< ���b�V���R�[�h���X�g�t�@�C��
			a_exeflag	|= ON_MESH;
			a_optcount[4]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-M") == 0) {	// 2003.4.1
			if(argc < i+2) return( false );
			a_exeflag	|= ON_MONO_MESH;
			e_iMeshCode	= atoi(argv[i+1]);	
			a_optcount[5]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-w") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_WHERE_STR;
			e_sWhere_Str	= argv[i+1];	// �G���A���i��������擾����
			a_optcount[6]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-pgdb") == 0) {
			a_exeflag		|= ON_PGDB_MODE;
			a_optcount[7]++;
			i+=1;
		} else
		if(strcmp(argv[i], "-v") == 0) {
			print_version();
			a_optcount[8]++;
			i+=1;
			return	( false );
		} else
		if(strcmp(argv[i], "-o") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_OUTFILE;
			e_sOut_File		= argv[i+1];	// �G���A���i��������擾����
			a_optcount[9]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-addrdb") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_ADDRDB;
			e_sConnectAddrDB= argv[i+1];	// �Z��DB�ւ̐ڑ�
			a_optcount[10]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-field") == 0) {
			a_exeflag		|= ON_FIELDS;
			e_sFields		= argv[i+1];
			a_optcount[11]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-ku") == 0) {
			a_exeflag		|= ON_KU_MODE;
			a_optcount[12]++;
			i+=1;
		} else
		if(strcmp(argv[i], "-gun") == 0) {
			a_exeflag		|= ON_GUN_MODE;
			a_optcount[13]++;
			i+=1;
		} else {
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

	// �������t�@�C���ݒ�
	if( !(a_exeflag & ON_DB_CNCT) ) {
		fprintf	(stderr, "DB�̐ڑ��ݒ��'-cnct'�ōs���Ă�������\n");
		return	( false );
	}else {
		if( !(a_exeflag & ON_PGDB_MODE ) ) {
			string	a_sTmpConnect_Info;	// �����ŃJ�b�g�����̂����Ȃ̂ŁE�E
			a_sTmpConnect_Info	= e_sConnect_Info;
			if( !e_pTool_Init.read_init_str ( (char*)a_sTmpConnect_Info.c_str() ) ) {
				return	( false );	// ���b�Z�[�W�͓����ŏo���Ă���
			}
		}
	}

	// ���b�V���w��
	if( a_exeflag & ON_MESH && a_exeflag & ON_MONO_MESH ) {
		fprintf	(stderr, "'-m','-M'�͓����w��ł��܂���\n");
		return	( false );
	}else {
		if( !(a_exeflag & ON_MESH || a_exeflag & ON_MONO_MESH) ) {
			fprintf	(stderr, "'-m','-M'�̂ǂ��炩���w�肵�ĉ�����\n");
			return	( false );
		}
	}

	// ���b�V�����C��
	if( !(a_exeflag & ON_MESHLAYER) ) {
		fprintf	(stderr, "���b�V���t�B�[�`���N���X��'-mf'�ōs���Ă�������\n");
		return	( false );
	}

	// �`�F�b�N���C��
	if( !(a_exeflag & ON_CHKLAYER) ) {
		fprintf	(stderr, "�|���S���t�B�[�`���N���X��'-fc'�ōs���Ă�������\n");
		return	( false );
	}

	// �o�̓t�@�C���I�[�v��
	if( a_exeflag & ON_OUTFILE ) {
		e_fpOut_Fp	= fopen ( e_sOut_File.c_str() , "w" );
		if ( e_fpOut_Fp == NULL ) {
			fprintf	( stderr, "�o�̓t�@�C��<%s>���I�[�v���ł��܂���\n", e_sOut_File.c_str() );
			return	( false );
		}
	}

	// �t�B�[���h����؂蕪����
	if( a_exeflag & ON_FIELDS && a_exeflag & ON_ADDRDB ) {
		fprintf	( stderr, "'-field'��'-addrdb'�̓����w��͏o���܂���\n" );
		return	( false );
	}else {
		if( a_exeflag & ON_FIELDS ) {
			// �t�B�[���h��','�ŕ�����
			size_t	a_sBegin	= 0;			// ������J�n�ʒu
			size_t	a_sPos		= 0;			// �f���~�^���o�ʒu
			string	a_sTmpStr	= e_sFields;	// ���������p
			for(;;)
			{
				a_sPos	= e_sFields.find ( ",", a_sBegin );
				if( a_sPos != e_sFields.npos ) {
					e_vsGetField.push_back ( e_sFields.substr ( a_sBegin, (a_sPos-a_sBegin) ) );
					a_sBegin	= a_sPos+1;
				}else {
					e_vsGetField.push_back ( e_sFields.substr ( a_sBegin, (e_sFields.size()-a_sBegin) ) );
					break;
				}
			}
		}else
		if( a_exeflag & ON_ADDRDB ) {
			if( !Connect_to_Adm_DB ( )  ) {
				fprintf	( stderr, "�Z��DB�̏������Ɏ��s���Ă��܂�\n" );
				return	( false );
			}	
		}else {
			fprintf	( stderr, "'-field'����'-addrdb'���w�肵�A�ǂ̃^�C�v�̏o�͂��s���������߂Ă�������\n" );
			return	( false );
		}
	}

	eExe_Flag	= a_exeflag;
	return		( true );
}

void		main_rootin::
print_usage	(char **argv)
{
	fprintf(	stdout,
			"=== �ySiNDY-DB�ʐϒl�Z�o�c�[���z(%s) ===\n"
				"<usage>\n"
				"\t%s [option]...\n"
				"\t<�I�v�V����>\n"
				"\t      '-cnct'            : DB�ڑ����(ex. -cnct ronly/ronly/SDE.DEFAULT/ruby2)\n"
				"\t      '-fc'              : �|���S���t�B�[�`���N���X(ex. -fc SINDY2009A.City_Admin)\n"
				"\t      '-mf'              : ���b�V���t�B�[�`���N���X(ex. -mf SINDY2009A.City_Admin)\n"
				"\t      '-l'               : ���O�f�[�^�o�̓p�X���w��\n"
				"\t      '-m'               : ���b�V���R�[�h���X�g�t�@�C�����w��(ex. -m .\\list\\mesh.txt�j\n"
				"\t      '-M'               : ���b�V���R�[�h���w��(ex. -M 59414130)\n"
				"\t      '-w'               : �����w���WHERE��w��(ex. OBJECTID > 0)\n"
				"\t      '-pgdb'            : PGDB�̃t�B�[�`���N���X��ǂݍ��ޏꍇ�Ɏw��\n"
				"\t      '-o'               : �o�̓t�@�C���w��(ex. -o out.txt)\n"
				"\t                           (�w�肵�Ȃ��ꍇ��stdout�֏o��)\n"
				"\t      '-addrdb'          : �Z��DB�ڑ��ݒ�(ex. y0901e/y0901e@delltcp)\n" 
				"\t                           ��'-field'�Ƃ͓����w��ł��Ȃ��B�s�撬���ʖʐϒl�����߂�ۂɎg�p\n"
				"\t      '-field'           : �o�̓t�B�[���h�ݒ�(ex -field CITYCODE,ADDRCODE)\n"
				"\t                           ��'-addrdb'�Ƃ͓����w��s��\n"
				"\t      '-ku'              : ���Z�߂Ďs�ɂ��ďo�͂���\n"
				"\t      '-gun'             : �S�����o�͂���(���w�莞�̓J�b�g����)\n"
				"\t      '-v'               : �o�[�W�������\��\n"
				"\t<�⑫>\n"
				"\t ��'-cnct','-fc','-mf','-m'����'-M','-addrdb'����'-field'�͕K�{�ł��B\n"
				"\t ���s�撬���ʖʐϒl�����߂�ꍇ��'-addrdb'���A���b�V���P�ʂŃ|���S���̖ʐρ{�t�B�[���h�̏��\n"
				"\t   ���o�͂���ꍇ��field���g�p���Ă��������B\n"

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
			, e_sVersion.c_str(), argv[0]);
}

/** ------------------------------------------------------------------------------
	�o�[�W�������̕\��
	@return		����
---------------------------------------------------------------------------------*/
void	main_rootin::
print_version	( void )
{
	fprintf(	stdout,
				"=== �ySiNDY-DB�ʐϒl�Z�o�c�[���z(%s) ===\n"
				"\t<�o�[�W�������>\n"
				"\t2009/02/23     1.0.0.1	: �V�K�쐬\n", 
				"\t2009/06/01     1.0.1.2	: [bug 7057]�ւ̑Ή�(���ߎs�̓s�s�R�[�h���K�؂ɏo�͂���Ȃ�)\n", 
				e_sVersion.c_str()
			);
}

/** ---------------------------------------------------------------------------
//	�G���[���O�̃w�b�_���o�͂���
//	@return	����
-----------------------------------------------------------------------------*/
void	main_rootin::
Output_Log_Header	( void )
{
	fprintf	( e_fpErr_Fp,		"#SINDYSTDLOG\n" );
	fprintf	( e_fpErr_Fp,		"#�c�[���o�[�W����          : <%s>\n", e_sVersion.c_str() );
	if( eExe_Flag & ON_PGDB_MODE ) {
		fprintf	( e_fpErr_Fp,	"#�ڑ����(PGDB)            : <%s>\n", e_sConnect_Info.c_str() );
	}else {
		fprintf	( e_fpErr_Fp,	"#�ڑ����(DB_ANY)          : <%s>\n", e_sConnect_Info.c_str() );
	}
	fprintf	( e_fpErr_Fp,		"#�o�̓t�@�C��              : <%s>\n", e_sOut_File.c_str() );
	fprintf	( e_fpErr_Fp,		"#���b�V���t�B�[�`���N���X  : <%s>\n", e_sMesh_Layer.c_str() );
	fprintf	( e_fpErr_Fp,		"#�|���S���t�B�[�`���N���X  : <%s>\n", e_sCheck_Layer.c_str() );
	if( eExe_Flag & ON_MESH ) {
		fprintf	( e_fpErr_Fp,	"#���b�V�����X�g            : <%s>\n", e_sMeshCode_File.c_str() );
	}else {
		fprintf	( e_fpErr_Fp,	"#���b�V���R�[�h            : <%d>\n", e_iMeshCode );
	}
	if( eExe_Flag & ON_WHERE_STR ) {
		fprintf	( e_fpErr_Fp,	"#���o����                  : <%s>\n", e_sWhere_Str.c_str() );
	}
	if( eExe_Flag & ON_ADDRDB ) {
		fprintf	( e_fpErr_Fp,	"#�Z��DB�ڑ����            : <%s>\n", e_sConnectAddrDB.c_str() );
		fprintf	( e_fpErr_Fp,	"#���ʋ�̏���              : <%s>\n", eExe_Flag&ON_KU_MODE?"�s�ɂ܂Ƃ߂�":"���̂܂܏o��" );
		fprintf	( e_fpErr_Fp,	"#�S���o��                  : <%s>\n", eExe_Flag&ON_GUN_MODE?"����":"�Ȃ�" );
	}else {
		fprintf	( e_fpErr_Fp,	"#�o�̓t�B�[���h            : <%s>\n", e_sFields.c_str() );
	}

//	fprintf	( e_fErr_Fp,		"#�����擾�t�B�[���h        : <%s>\n", e_sSet_Fields.c_str() );
	fflush	( e_fpErr_Fp );
}

/** ------------------------------------------------------------------------------
	���b�V����`��DB����l�����郁�\�b�h�̏�����
	��get_mesh_rect �����O�ɕK���Ă�
------------------------------------------------------------------------------==*/
bool	main_rootin::
get_mesh_init	(	mk_input_list&	c_mMList,			// ���̓��X�g�N���X
				//	string			c_sMeshLayer,		// ���b�V�����C����
					IWorkspacePtr	c_ipWorkSpace	)	// �w�肷�郏�[�N�X�y�[�X
{

	char*			a_cpList_File	= NULL;
	if( eExe_Flag & ON_MESH ) {
		a_cpList_File	= (char*)e_sMeshCode_File.c_str();
		if( a_cpList_File == NULL ) {
			fprintf	( e_fpErr_Fp, "���b�V���R�[�h���X�g�t�@�C�������Ƃ肾���܂���ł���[check_exe]\n");
			return	( false );
		}	
		if( c_mMList.read_mesh_list ( a_cpList_File ) != 0 ) {	// �����Ń��X�g��ǂݍ���
			fprintf	( e_fpErr_Fp, "���b�V���R�[�h���X�g�t�@�C��<%s>�̓ǂݍ��݂Ɏ��s[check_exe]\n", a_cpList_File );
			return	( false );
		}
	}else {
		if(c_mMList.read_mesh_list ( e_iMeshCode ) == -1) {
			fprintf	( e_fpErr_Fp, "���b�V��<%d>�̐ݒ�Ɏ��s���܂���\n", e_iMeshCode );
			return	( false );
		}
	}

	int			a_iMDigit		= 0;
	a_iMDigit	= c_mMList.get_mesh_digit ();
	
	switch	( a_iMDigit )
	{
	case	4:
	case	6:
	case	8:
		break;
	default:
		fprintf	( stderr, "���X�g�̃��b�V�������ɂ͑Ή����Ă��܂���[check_exe]\n");
		return	( false );
	}

	IFeatureWorkspacePtr	a_F_Workspace	( c_ipWorkSpace );	// �t�B�[�`�����[�N�X�y�[�X

	if( FAILED(a_F_Workspace->OpenFeatureClass(CComBSTR(e_sMesh_Layer.c_str()), &e_ipF_Class )) ) {
		fprintf	(stderr, "���C�� <%s> �̃I�[�v���Ɏ��s\n", e_sMesh_Layer.c_str());
		return	(false);
	}else {
		fprintf(stdout,	"<%s> ���C�����I�[�v��\n", e_sMesh_Layer.c_str());
	}

	// 2002.12.09 �f�[�^�����镔���̃��b�V�����X�g
	eDigit	= a_iMDigit;
	return	( true );
}

/** ------------------------------------------------------------------------------
	���b�V����`��GeoDB����擾���� \
	��GeoDB�ւ̐ڑ����m������Ă��邱�Ƃ��O��
--------------------------------------------------------------------------------*/
IPolygonPtr		main_rootin::
get_mesh_rect	( int	c_iMeshCode	)	///< ���b�V���R�[�h
{

	CComBSTR	a_SpatialCol;
	e_ipF_Class->get_ShapeFieldName	( &a_SpatialCol );
	BSTR		a_Where_Str;
	char		a_tmpstr[30];	//�@�����͂Ȃ�Ƃ��������C������

	crd_cnv		crd;
	int			a_iDigit	= 0;
	a_iDigit	= crd.GetMeshDigit( c_iMeshCode );
	if(a_iDigit == 4) {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// ���b�V���R�[�h�𕶎����
		eDigit	= 4;
	}else 
	if(a_iDigit == 6) {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// ���b�V���R�[�h�𕶎����
		eDigit	= 6;
	}else {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);		// ���b�V���R�[�h�𕶎����
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

			return	( a_Clip_Shp );
			
		/*	VARIANT_BOOL	a_bResult;
			a_Clip_Shp->get_IsEmpty( &a_bResult );
			if(a_bResult == VARIANT_TRUE) {	// �N���b�v���ʂ�NULL�Ȃ烁�b�V���O
				continue;
			}
			if( FAILED(a_Clip_Shp->get_Envelope( &a_ipMesh_Rect) )) {
				fprintf	( stderr, "�Y�����b�V��<%d>�̋�`���݂���܂���ł���\n", c_iMeshCode);
				return	( NULL );
			}else {
				return	( a_ipMesh_Rect );
			}*/
		}
	}
	return	( a_ipMesh_Rect );
}

/** ------------------------------------------------------------------------------
	��`�̃��C����
	@return	�|�����C��
--------------------------------------------------------------------------------*/
IPolylinePtr	main_rootin::
Get_Rect_Line	(	IEnvelopePtr	c_ipEnv	)		///< ���b�V����`
{
	//
	// 2002.07.01 Intersect�p�̃W�I���g�����쐬
	IPolylinePtr		a_ipRectLine(CLSID_Polyline);	// ��`�p�W�I���g��
	IPointCollectionPtr	a_ipRecPts	(a_ipRectLine);		// ��`�p�_��

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
	a_ipTopo		= a_ipRectLine;

	if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
		fprintf	( e_fpErr_Fp, "Simplify�Ɏ��s\n");
		return	( NULL );
	}
	if(FAILED ( a_ipRectLine->SimplifyNetwork() ) ) {		// 2003.06.13 ����������Ȃ��Ƃ��߂Ȃ��Ƃ�����
		fprintf	( e_fpErr_Fp,	"SimplifyNetwork�Ɏ��s" );	// �G���[���b�Z�[�W
		return	( NULL );
	}
	return ( a_ipRectLine );
}

/** ------------------------------------------------------------------------------
	��`�̃|���S����
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
		fprintf	( e_fpErr_Fp, "Simplify�Ɏ��s\n");
		return	( NULL );
	}
	if(FAILED ( a_ipRectPoly->SimplifyPreserveFromTo() ) ) {
		fprintf	( e_fpErr_Fp,	"SimplifyPreserveFromTo�Ɏ��s" );	// �G���[���b�Z�[�W
		return	( NULL );
	}
	return ( a_ipRectPoly );
}


/** ------------------------------------------------------------------------------
	�N���b�v�}�`��Ԃ�
--------------------------------------------------------------------------------*/
IGeometryPtr	main_rootin::
Get_Clip_Obj	(	IGeometryPtr	c_ipGeo,		///< �W�I���g��
					IGeometryPtr	c_ipClip	)	///< ����ŃN���b�v
{

	IGeometryPtr			a_ipClip_Shp;		// �����Ƀf�[�^���R�s�[���ăN���b�v����
	ITopologicalOperatorPtr	a_ipClip_Topo;
	a_ipClip_Topo				= c_ipGeo;
		
	if( FAILED( a_ipClip_Topo->Intersect (c_ipClip, esriGeometryNoDimension, &a_ipClip_Shp)) ) {
		fprintf	( e_fpErr_Fp,"�V�F�C�v�̃N���b�v�Ɏ��s" );	// �G���[���b�Z�[�W
		return	( NULL );
	}
	
	VARIANT_BOOL	a_bResult;
	a_ipClip_Shp->get_IsEmpty( &a_bResult );
	if(a_bResult == VARIANT_TRUE) {	// �N���b�v���ʂ�NULL�Ȃ烁�b�V���O
		return	( NULL );
	}

	return	( a_ipClip_Shp );

}

/** ------------------------------------------------------------------------------
	�����̌v�Z
	@return	���K�����W�l�ł̋���	
--------------------------------------------------------------------------------*/
double	main_rootin::
Get_Length	(	int			c_iPtCount,	// �|�C���g��
				Norm_Pt*	c_ipPts	)	// �f�[�^�J�E���g
{

	double	a_dLength	= 0.0;

	int		i = 0;
	for( i = 0; i < c_iPtCount-1; i++ )
	{
		double	a_iDiff_X	= c_ipPts[i+1].e_iX - c_ipPts[i].e_iX;
		double	a_iDiff_Y	= c_ipPts[i+1].e_iY - c_ipPts[i].e_iY;
		a_dLength	+= sqrt ( a_iDiff_X * a_iDiff_X + a_iDiff_Y * a_iDiff_Y );
	}
	return	( a_dLength );	
}

/** ------------------------------------------------------------------------------
	�|���S���̖ʐ�
--------------------------------------------------------------------------------*/
double main_rootin::
Get_Real_Area	(	int			c_count,	// ��ԓ_��
					Norm_Pt*	c_ipPts,	// IN�_��
					int			c_meshcode)	// ���b�V���R�[�h
{
	double		lon		= 0.0;
	double		lat		= 0.0;
	double		xratio	= 0.0;
	double		yratio	= 0.0;

	crd_cnv		crd;

	crd.MeshtoLL(c_meshcode, 0, 0, &lon, &lat);
	xratio	= RA * sin(M_PI / 2.0 - lat * M_PI / 180.0) * 2.0 * M_PI / 23040.0 / 1000000.0; // 23040 = 360*64
	yratio	= RA * 2.0 * M_PI / 34560.0 / 1000000.0;
	return(calc_area(c_count, c_ipPts, xratio, yratio));
}

/** ------------------------------------------------------------------------------
	�|���S���̖ʐ�
--------------------------------------------------------------------------------*/
double main_rootin::
calc_area	(	int			c_count,	// ��ԓ_��
				Norm_Pt*	c_ipPts,	// IN�_��
				double		c_xratio,	// x�����䗦?
				double		c_yratio)	// x�����䗦?
{
	double	S		= 0.0;
	int		i;
	int		s		= 0;
	int		e		= 0;
	for(i = 0; i < c_count - 1; i++)
	{
		s	= (i == 0) ? c_count-2: i-1;
		e	= (i == c_count-2) ? 0: i+1;
		S	+=((c_ipPts[s].e_iX - c_ipPts[e].e_iX) * c_xratio * c_ipPts[i].e_iY * c_yratio);
	}
	S /= (2.0);
	return(fabs(S));
}

/** ------------------------------------------------------------------------------
	���̒ǉ�
	@return	����
--------------------------------------------------------------------------------*/
void	main_rootin::
Put_Info	(	ms_data_info&	c_mInfos,
				data_info		c_dInfo	)
{
	ims_data_info	a_iIndx;

	a_iIndx	= c_mInfos.find( c_dInfo );
	if( a_iIndx == c_mInfos.end() ) {	// �Y���f�[�^��������Ȃ��ꍇ�͒ǉ�
		c_mInfos.insert ( c_dInfo );
	}else {
		data_info	a_dTmp_Dat	= *a_iIndx;
		a_dTmp_Dat.e_dSum	+= c_dInfo.e_dSum;	// ������ǉ�����
		// ��������f�[�^�Z�b�g����f�[�^���폜����
		c_mInfos.erase	( a_iIndx );
		// �������C�������f�[�^��ǉ�����
		c_mInfos.insert	( a_dTmp_Dat );
	}
}

/** ------------------------------------------------------------------------------
	���̏o��(�W�v)
	@return	����
--------------------------------------------------------------------------------*/
void	main_rootin::
Print_Info	(	ms_data_info&	c_mInfos,		///< �o�͏��
				FILE*			c_fpOutFp)		///< �o�̓t�@�C���|�C���^
{
	ims_data_info	a_iIndx;
	for( a_iIndx = c_mInfos.begin(); a_iIndx != c_mInfos.end(); a_iIndx++ )
	{
		data_info	a_dDat	= *a_iIndx;

		fprintf	( c_fpOutFp, "%s\t%s\t%0.10f\n",
							a_dDat.e_sAddrCode.c_str(),
							a_dDat.e_sChimei.c_str(),
							a_dDat.e_dSum );
	}
}

/** ------------------------------------------------------------------------------
	���̏o��(�P��)
	@return	����
--------------------------------------------------------------------------------*/
void	main_rootin::
Print_Info	(	int				c_iMeshCode,	///< ���b�V���R�[�h[IN]
				data_info&		c_mInfo,		///< �o�͏��[IN]
				FILE*			c_fpOutFp)		///< �o�̓t�@�C���|�C���^[IN]
{
	fprintf	( c_fpOutFp, "%d\t", c_iMeshCode );
	vector<CComVariant>::iterator	a_iIndx;
	for( a_iIndx = c_mInfo.e_vcAttrs.begin(); a_iIndx != c_mInfo.e_vcAttrs.end(); a_iIndx++ )
	{
		string	a_sTmpStr;
		switch ((*a_iIndx).vt)
		{
		case	VT_NULL:
			fprintf	( c_fpOutFp, "NULL" ); break;
		case	VT_I2:
		case	VT_I4:
		case	VT_INT:
		case	VT_UINT:
			fprintf	( c_fpOutFp, "%d", (*a_iIndx).intVal ); break;
		case	VT_R4:
		case	VT_R8:
			fprintf	( c_fpOutFp, "%0.15f", (*a_iIndx).dblVal ); break;
		case	VT_BSTR:
			a_sTmpStr	= e_cCU.Str_to_SJIS ((CComBSTR&)(*a_iIndx).bstrVal, common_use::UNICODE);
			fprintf	( c_fpOutFp, "%s", (char*)a_sTmpStr.c_str() );
			break;
		case	VT_DATE:
			{
			UDATE	a_uDate;
			VarUdateFromDate( (*a_iIndx).date, 0, &a_uDate );
			fprintf	( c_fpOutFp, "%d/%02d/%02d %02d:%02d:%02d",
				a_uDate.st.wYear, a_uDate.st.wMonth, a_uDate.st.wDay,
				a_uDate.st.wHour, a_uDate.st.wMinute, a_uDate.st.wSecond );
			}
			break;
		default:
			fprintf	( c_fpOutFp, "<���Ή��̌^>" );
			break;
		}
		fprintf	( c_fpOutFp, "\t");
	}
	fprintf	( c_fpOutFp, "%0.10f\n", c_mInfo.e_dSum );
	fflush	( c_fpOutFp );
}

/**
	�_��ɂ���
*/	/*
bool	main_rootin::
Get_Norm_Ps	(	IGeometryPtr		c_ipGeo,	///< �W�I���g��[IN]
			 	int					c_iMeshCode,///< ���b�V���R�[�h[IN]
				int					c_iMode,	///< 0=line,1=Polygon
				vector<Norm_Pt>&	c_vPts	)	///< �_��[OUT]
{
	HRESULT	hr;

	bool		a_iIsClockwise	= true;

	IPointCollectionPtr	a_ipPtCol(c_ipGeo);
	long	a_lCount	= 0;
	a_ipPtCol->get_PointCount ( &a_lCount );

	// WKSPoint�Ŏ擾����
	WKSPoint*	a_wpPoints	= NULL;

	a_wpPoints	= new WKSPoint[a_lCount];	// �_��̊m��
	hr	= a_ipPtCol->QueryWKSPoints ( 0, a_lCount, a_wpPoints );
	if ( FAILED( hr ) ){
		fprintf	( stderr, "WKSPoint�擾�Ɏ��s" );
		return	( true );
	}

	if( c_iMode == 1 ) {

		list<RING>				a_lRings;

		// ����������
		InitRing( a_lCount, a_wpPoints, a_lRings );
		// �_��̕�܊֌W���Ƃ��Ă���
		CheckExteriorInterior( a_wpPoints, a_lRings );

		// �����O�̏�Ԃ��m�F
		RING	a_rForChk	= *a_lRings.begin();
		if( a_rForChk.IsClockwise == TRUE ) {
			a_iIsClockwise	= true;
		}else {
			a_iIsClockwise	= false;
		}
	}

	crd_cnv	a_cCrd;
	
	int	i = 0;
	Norm_Pt	a_nPt;
	for( i = 0; i < a_lCount; i++ )
	{
		int	a_iX = 0, a_iY = 0;
		a_cCrd.LLtoMesh2( a_wpPoints[i].X, a_wpPoints[i].Y, c_iMeshCode, &a_nPt.e_iX, &a_nPt.e_iY);
		c_vPts.push_back( a_nPt );
	}
	delete [] a_wpPoints;
	return	( a_iIsClockwise );
}
*/
/**
	�I�u�W�F�N�g�̖ʐϋy�уA�[�N����Ԃ�
*//*
double	main_rootin::
Get_Obj_LA	(	IGeometryPtr	c_ipGeo,		///< �W�I���g��[IN]
				int				c_iMeshCode,	///< ���b�V���R�[�h[IN]
				int*			c_iType	)		///< �f�[�^�^�C�v 1=line, 2=polygon
{
	double	a_dRet_Value	= 0.0;	// �Z�o����

	// �W�I���g���^�C�Ղ����߂�
	esriGeometryType			a_eType;
	c_ipGeo->get_GeometryType ( &a_eType );

	//�@�����W�I���g���̐��𓾂�
	long	a_lGeoNum	= 0;
	IGeometryCollectionPtr	a_ipGeoCol ( c_ipGeo );

	a_ipGeoCol->get_GeometryCount ( &a_lGeoNum );
	int	i = 0;
	if( a_eType == esriGeometryPolyline ||
		a_eType == esriGeometryPolygon  )
	{
		bool	a_iIsClockwise	= 0;
		double	a_dCur_Val		= 0.0;
		vector<Norm_Pt>	a_vLinePts;
		for( i = 0; i < a_lGeoNum; i++ )
		{
			IGeometryPtr	a_ipCurGeo;
			a_ipGeoCol->get_Geometry( i, &a_ipCurGeo );
			// ���K���_��ɂ���
			
			if( a_eType == esriGeometryPolyline ){
				Get_Norm_Ps	(a_ipCurGeo, c_iMeshCode , 0, a_vLinePts);	///< �_��[OUT]
				a_dRet_Value += Get_Length		( a_vLinePts.size(), &a_vLinePts[0] );
			}else {
				a_iIsClockwise	= Get_Norm_Ps	(a_ipCurGeo, c_iMeshCode , 1, a_vLinePts);	///< �_��[OUT]
				a_dCur_Val	= Get_Real_Area	( a_vLinePts.size(), &a_vLinePts[0], c_iMeshCode );
				if( a_iIsClockwise != true ) {
					a_dCur_Val*=-1.0;
				}
				a_dRet_Value += a_dCur_Val;
			}
			a_vLinePts.clear();
		}
		if( a_eType == esriGeometryPolyline ){
			*c_iType	= 1;
		}else {
			*c_iType	= 2;
		}
	}else {
		*c_iType	= 0;
	}
	return	( a_dRet_Value );
}
*/

/**
	�ʐϒl�����߂�
*/
double main_rootin::
CalcArea(	IGeometryPtr	a_ipSrc_Geo,		///< �N���b�v�O�̃W�I���g��
			IGeometryPtr	a_ipCliped_Geo	)	///< �N���b�v��̃W�I���g��
{
	const double	EARTH_R	= 6370.0;

	IAreaPtr	a_ipSrcArea( a_ipSrc_Geo );
	IAreaPtr	a_ipCliped_Area( a_ipCliped_Geo );
	double a_dArea	= 0.0;
	a_ipCliped_Area->get_Area(&a_dArea);
	if(a_dArea > 0.0){

		// �N���b�v�O�̃f�[�^�����\�_�����߂�
		IPointPtr	ipPoint(CLSID_Point);
		a_ipSrcArea->QueryCentroid(ipPoint);

		double aY;
		ipPoint->get_Y(&aY);
		double aRatio = EARTH_R * sin(M_PI / 2.0 - aY * M_PI / 180.0) * 2.0 * M_PI/360.0;
		a_dArea *= (aRatio * EARTH_R * 2.0 * M_PI / 360.0);
	}else{
		a_dArea = 0.0;
	}
	return a_dArea;
}

/**
	�t�B�[�`�����瑮�������o��
*/
void	main_rootin::
GetFAttrs	(	Feature_All_Attrs&	c_fAttrs,	///< �����l[IN]
				data_info&			c_dInfo	)	///< ���o�������[OUT]
{
	vector<string>::iterator	a_iIndx;
	for( a_iIndx = e_vsGetField.begin(); a_iIndx != e_vsGetField.end(); a_iIndx++ )
	{
		// �������o��
		Feature_Attr	a_fAtt;
		a_fAtt	= c_fAttrs.Get_Attr ( (*a_iIndx).c_str() );
		c_dInfo.e_vcAttrs.push_back( a_fAtt.e_ccvAttr );
	}
}

/** ------------------------------------------------------------------------------
	�`�F�b�N���s��
	�������ŃG���[�t�@�C���̏��������s��
	@return	����	true
	@return ���s	false
--------------------------------------------------------------------------------*/
bool	main_rootin::
check_exe	(	IWorkspacePtr	c_ipWSpace )	///< ���[�N�X�y�[�X
{
	HRESULT		hr;
	crd_cnv		crd;
	// ���O�t�@�C���̃I�[�v��
	if( !Open_Err_Log () ) {
		return	( false );
	}

	// ���O�w�b�_�̏o��
	Output_Log_Header();

	// SiNDY�p�̃��O
	smp_sindy_err_info	a_sPrnInfo;
	a_sPrnInfo.Init		( e_fpErr_Fp );

	mk_input_list	a_mChk_List;

	// ���b�V�����X�g�֌W�̏�����
	if( !get_mesh_init( a_mChk_List , c_ipWSpace ) ) {
		fprintf	( stderr, "���b�V�����C���̊l���̏������Ɏ��s[check_exe]\n");
		return	( false );
	}
	
	// �`�F�b�N���郌�C�����̊l��
	char*			a_cpCheck_Layer	= NULL;
	a_cpCheck_Layer	= (char*)e_sCheck_Layer.c_str();		// �s�s�n�}���[�h�̏ꍇ

	if( a_cpCheck_Layer == NULL ) {
		fprintf	( stderr, "�`�F�b�N���C�������Ƃ肾���܂���ł���\n");
		return	( false );
	}

	// ���C���̓ǂݍ���
	msi_handle_ws	a_mHandle_WS;
	if( !a_mHandle_WS.Init ( c_ipWSpace, a_cpCheck_Layer) ) {
		fprintf	( stderr, "�`�F�b�N���C��<%s>�̓ǂݍ��݂Ɏ��s[check_exe]\n", a_cpCheck_Layer );
		return	( false );		
	}

	// �L���b�V����ɂƂ��Ă���
	// �I���������e�X�g 2002.6.24
	ISpatialCacheManagerPtr		a_ipSpCacheMgr( c_ipWSpace );
	char	a_cpPrnBuf[512];
	int		a_iCur_Mesh;
	ms_data_info	a_mData_Info;
	for( a_iCur_Mesh = a_mChk_List.get_mesh_first(); a_iCur_Mesh != -1; a_iCur_Mesh = a_mChk_List.get_mesh_next() )
	{
		/// �f�[�^�쐬�J�n����
		DWORD				a_dMesh_Begin_Time	= 0;
		a_dMesh_Begin_Time	= timeGetTime();

		// ���b�V�����ƂɃL���b�V���}�l�[�W�����N���A���Ă���
	/*	hr	= a_ipSpCacheMgr->EmptyCache( );
		if( FAILED ( hr ) ) {
			fprintf	( stderr, "�L���b�V���}�l�[�W�����N���A�ł��܂���ł���\n");
			continue;
		}*/

		fprintf	( stdout, "���b�V���R�[�h [%d] �`�F�b�N��\n", a_iCur_Mesh );
	
		// ���b�V����`�̎��o��
		IPolygonPtr		a_ipRect;
		a_ipRect		= get_mesh_rect( a_iCur_Mesh );
		if( a_ipRect == NULL ) {	// ���b�V����`�������ꍇ
			sprintf	( a_cpPrnBuf, "���b�V���R�[�h<%d>�̋�`��������܂���ł���\n", a_iCur_Mesh );
			a_sPrnInfo.Prn_Info	( a_cpPrnBuf, stderr );
			continue;
		}
	
		// ��ԃL���b�V���}�l�[�W���Ƀ��b�V�����̃f�[�^���Ƃ肾��
	//	a_ipSpCacheMgr->FillCache( a_ipRect );
	
		IPolygonPtr			a_ipRect_Poly;
	//	a_ipRect_Poly		= Get_Rect_Polygon	( a_ipRect );
		a_ipRect_Poly		= a_ipRect;
		if( a_ipRect_Poly == NULL ) {
			sprintf	( a_cpPrnBuf, "<%d>���b�V����`�|���S���쐬�Ɏ��s\n", a_iCur_Mesh );
			a_sPrnInfo.Prn_Info	( a_cpPrnBuf, stderr );
			continue;
		}
		IGeometryPtr		a_ipRect_Poly_Geo;
		a_ipRect_Poly_Geo	= a_ipRect_Poly;

		// ���b�V����`�̋��E���ŃT�[�`����
		IFeatureCursorPtr	a_ipDatas;
		a_ipDatas	= a_mHandle_WS.Search_Feature	( a_ipRect_Poly_Geo, esriSpatialRelIntersects, e_sWhere_Str, (RET_COUNT|RET_FCURSOR) );
		if (a_ipDatas == NULL ) {
			sprintf	( a_cpPrnBuf, "<%d>���b�V����`�Ńf�[�^�����܂���\n", a_iCur_Mesh );
			a_sPrnInfo.Prn_Info	( a_cpPrnBuf, stderr );
			continue;
		}

		long		a_lTCount	= 0;
		a_lTCount	= a_mHandle_WS.Get_Data_Count ();

		//-----------<�`�F�b�N�i�s�x�\��>--------------------
		long		i			= 0;
		fprintf	( stdout,	"���b�V���R�[�h [%d] ���f�[�^���� %d ��\n", a_iCur_Mesh, a_lTCount );
		//--------------------------------------------------

		IFeaturePtr	a_ipCurFeat;
		for ( ; ; i++)
		{
			//�@���b�V���ň����|���Ă����t�B�[�`��
			if( a_ipDatas->NextFeature ( &a_ipCurFeat ) == S_OK) {

				// �t�B�[�`���̑��������o��
				Feature_All_Attrs	a_fFeat_Attrs;
				a_fFeat_Attrs.Init	( a_ipCurFeat);
			
				// �W�I���g�����擾����
				IGeometryPtr			a_ipCurGeo;			// ���ݒ��ڒ��̃W�I���g��

				a_ipCurGeo	= a_fFeat_Attrs.Ret_Geo();
				if( a_ipCurGeo == NULL ) {
					sprintf	( a_cpPrnBuf, "�W�I���g���̃R�s�[���擾�ł��Ȃ�[check_exe]\n");
					a_sPrnInfo.Prn_Info	( a_cpPrnBuf, stderr );
					continue;
				}

				//---------------------------------------------------------------------------------------
				// �N���b�v����
				IGeometryPtr	a_ipCliped;
				a_ipCliped		= Get_Clip_Obj(	a_ipCurGeo,	a_ipRect_Poly_Geo );

				if( a_ipCliped	== NULL ) {
				//	sprintf	( a_cpPrnBuf, "�N���b�v���ʂ�NULL\n");
				//	a_sPrnInfo.Prn_Info	( a_cpPrnBuf, stderr );				
					continue;
				}

				// �ʐρE�A�[�N���v�Z����
				double	a_dArea			= CalcArea ( a_ipCurGeo, a_ipCliped );
				//---------------------------------------------------------------------------------------

				if( eExe_Flag & ON_FIELDS ) {
					data_info		a_dTmp;
					a_dTmp.e_dSum	= a_dArea;
					GetFAttrs		( a_fFeat_Attrs, a_dTmp );
					Print_Info		( a_iCur_Mesh, a_dTmp, e_fpOut_Fp );
				}else {
					// �������o��
					Feature_Attr	a_fAtt;
					a_fAtt	= a_fFeat_Attrs.Get_Attr ( "CITYCODE" );
					if( a_fAtt.e_ccvAttr.vt == VT_EMPTY ) {
						sprintf	( a_cpPrnBuf, "CITYCODE�̏�񂪎擾�ł��܂���\n");
						a_sPrnInfo.Prn_Info	( a_cpPrnBuf, stderr );	
						return	( false );
					}

					string	a_sTmpCode5;
					a_sTmpCode5	= e_cCU.Str_to_SJIS ((CComBSTR&)a_fAtt.e_ccvAttr.bstrVal, common_use::UNICODE);

					// �Z�b�g�ɒǉ�
					addr_dat				a_aOraInfo;
					e_aAddr_DB_Hndl.GetOraDBStr((char*)a_sTmpCode5.c_str(), a_aOraInfo);
				
					// ���ߎw��s�Ή�
					data_info	a_dObj_Info;
					addr_dat	a_aWardtoCity;
					a_dObj_Info.e_dSum		= a_dArea;
					if( eExe_Flag & ON_KU_MODE && e_aAddr_DB_Hndl.GetCitybyWard( a_aOraInfo, a_aWardtoCity ) ) {
						a_dObj_Info.e_sAddrCode	= a_aWardtoCity.e_sCode;
						a_dObj_Info.e_sChimei	= a_aWardtoCity.e_sShi_Kanji;
					}else {
						a_dObj_Info.e_sAddrCode	= a_sTmpCode5;
						a_dObj_Info.e_sChimei	= a_aOraInfo.e_sShi_Kanji;
					}
					// �S�����t���ꍇ�̓J�b�g����
					if( !(eExe_Flag & ON_GUN_MODE) ) {
						string	a_sCutStr;
						long	a_lPos	= 0;
						a_lPos	= a_dObj_Info.e_sChimei.find ( "�S�@" );
						if( a_lPos != a_dObj_Info.e_sChimei.npos ) {	// �S�p�X�y�[�X�Ō���
							long	a_lLen	= 0, a_lSubLen = 0;
							a_lLen	= a_dObj_Info.e_sChimei.length();
							a_lSubLen	= a_lLen - (a_lPos+4);
							if(a_lSubLen > 0) {
								a_sCutStr	= a_dObj_Info.e_sChimei.substr ( a_lPos+4, a_lSubLen);
								a_dObj_Info.e_sChimei	=a_sCutStr;
							}
						}
					}
					// �o�^�p�f�[�^
					Put_Info	( a_mData_Info,	a_dObj_Info	);
				}

				//--------------------------------------------<�����܂�>
			}else {
				break;
			}
		
			//----------------
			if( i > 0 && (i % 100) == 0) {	// 
				// �J�E���g100�����ƂɃ��b�Z�[�W��\��
				PrnMsg( stdout,	"���b�V���R�[�h [%d]  %d �����@%d �� �`�F�b�N�I�� \n", a_iCur_Mesh, a_lTCount, i );
			}
			//----------------
		}

		//--------------------------------------------<�����܂�>
		fflush ( e_fpErr_Fp );	// �f�[�^���t���b�V������
		//----------------
		PrnMsg	( stdout, "���b�V���R�[�h [%d] �`�F�b�N�I��\n", a_iCur_Mesh );
		
		DWORD			a_dMake_Finish	= 0;
		a_dMake_Finish	= timeGetTime();

		sprintf	( a_cpPrnBuf,	"<%d> : �`�F�b�N���� = %0.2f(s)�ł���\n",
							a_iCur_Mesh, (double)((a_dMake_Finish - a_dMesh_Begin_Time) / 1000.0) );

		a_sPrnInfo.Prn_Info_Std	( a_cpPrnBuf, stdout );
		//-----------------
	}
	// �s�撬���P�ʂŏo�͂���
	if( eExe_Flag & ON_ADDRDB ) {
		// �f�[�^�o��
		Print_Info	( a_mData_Info, e_fpOut_Fp );		///< �o�̓t�@�C���|�C���^
		a_mData_Info.clear();
	}
	return	( true );
}
