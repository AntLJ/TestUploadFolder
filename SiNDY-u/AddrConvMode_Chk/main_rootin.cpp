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
#include <TDC/useful_headers/flush_scope_guard.h>

//	2003.07.04	main_rootin.h�����`�𕪗�

string	main_rootin::
Str_to_SJIS		(	CComBSTR&	c_CStr	)		// ������(Unicode)
{
	string		ret_str;

	if (*c_CStr != 0) {
		_bstr_t a_bTmpStr(c_CStr);
		ret_str = a_bTmpStr;
	}
	return	(ret_str);
}

/** ------------------------------------------------------------------------------
	SiNDYDB�ւ̐ڑ��I�[�v��
	@return	true	����
	@return	false	���s
--------------------------------------------------------------------------------*/
bool	main_rootin::
SiNDY_DB_Open	(IWorkspacePtr&			c_ipWorkspace_Addr	)
{
	// �Z��DB
	if( !DB_Open	( c_ipWorkspace_Addr,
					  e_pTool_Init.get_UserName_Address(), e_pTool_Init.get_Password_Address(),
					  e_pTool_Init.get_Instance_Address(), e_pTool_Init.get_Server_Address(),
					  e_pTool_Init.get_Version_Address() )) {
		fprintf	( stderr, "�ڑ��o���Ȃ�����\n");
		return	( false );
	}else {
		fprintf	( stdout, "�ڑ�����\n" );
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
			char*					c_cpInstance,		///< �C���X�^���X
			char*					c_cpServer,			///< �T�[�o��
			char*					c_cpVersion	)		///< �o�[�W����
{
	USES_CONVERSION;	// OLE2T ���̈ꕔ�̃}�N�����g�p����ꍇ�ɂ͕K���K�v

	

	CString connectStr;
	connectStr.Format(_T("%s/%s/%s/%s/%s"),	c_cpUserName, c_cpPassword,	c_cpVersion, c_cpInstance, c_cpServer );
	printf(_T(	"�ڑ��v���p�e�B�F<%s>\n"),	connectStr);
	
	c_ipWorkspace = sindy::create_workspace(connectStr);

	// DB�֐ڑ�
	printf(_T("�ڑ���..."));
	if( !c_ipWorkspace ) {
		return	( false );
	}else {
		return	( true );
	}
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
Output_Log_Header	( void )
{
	fprintf	( e_fErr_Fp,		"#SINDYSTDLOG\n" );
	fprintf	( e_fErr_Fp,		"#�c�[���o�[�W����          : <%s>\n", e_sVersion.c_str() );
	if( eExe_Flag & ON_PGDB_MODE ) {
		fprintf	( e_fErr_Fp,	"#�ڑ����(PGDB)            : <%s>\n",				e_cpPGDB_File );
	}else {
		fprintf	( e_fErr_Fp,	"#�ڑ����(DB_ADDRESS)      : <%s/%s/%s/%s/%s>\n",	e_pTool_Init.get_UserName_Address(), e_pTool_Init.get_Password_Address(),
																					e_pTool_Init.get_Version_Address(),	e_pTool_Init.get_Instance_Address(),
																					e_pTool_Init.get_Server_Address() );
	}
	fprintf	( e_fErr_Fp,		"#�s���E�|���S�����C��      : <%s>\n", e_sCheck_Layer );
	fprintf	( e_fErr_Fp,		"#���|�C���g���C��          : <%s>\n", e_sGou_Layer );
}

#define	OPTION_MAX	8
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
//	int		digit		= 0;	// ���b�V���̌���

	//-----------------------------------------------------------------------
	// �c�[���o�[�W�����\�� 2008.11.12
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

//	if(argc < 3) {
//		return( false );
//	}

//	e_cpInit_File		= argv[1];	///< �������t�@�C��(�Z��DB�ڑ�)���܂�
//	e_sCheck_Layer		= argv[2];	///< �`�F�b�N���C���i���L�j
//	e_sGou_Layer		= argv[3];	///< ���|�C���g���C��

	// �I�v�V�������Ȃɂ������Ƃ�
//	if( argc == 3 ) {
//		return	( true );
//	}

	// �I�v�V����������A���̂Ƃ���g�p���Ȃ�
	for(i = 1; i < argc;)
	{
		if(strcmp(argv[i], "-l") == 0) { // ���O�t�@�C���ݒ�i�ݒ肪�����ꍇ�́A�G���[��stderr�֐��ꗬ���ɂȂ�j
			if(argc < i+2) return( false );
			e_cpLog_File	= argv[i+1];	///< ���b�V���R�[�h���X�g�t�@�C��
			a_optcount[0]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-code") == 0) {
			if(argc < i+2) return( false );
			a_exeflag	|= ON_MONO_ADCODE;
			e_cpAddrCode	= argv[i+1];	
			a_optcount[1]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-t") == 0) {
			if( argc < i+2) return( false );
			e_cpCode_List_Txt	= argv[i+1];
			a_exeflag	|= ON_CODE_LIST_TXT;
			a_optcount[2]++;
			i+= 2;
		}else
		if(strcmp(argv[i], "-v") == 0) {
			//if(argc < i+1) return( false );
			print_version();
			a_optcount[3]++;
			i+=1;
			return	( false );	// ���ɂ�������H
		}else
		if(strcmp(argv[i], "-pgdb") == 0) {
			if(argc < i+2) return( false );
			e_cpPGDB_File	= argv[i+1];
			a_exeflag		|= ON_PGDB_MODE;
			WCARD1			= "\?";
			a_optcount[4]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-i") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_INIT_FILE;
			e_cpInit_File	= argv[i+1];	
			a_optcount[5]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-cadm") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_CITY_ADMIN;
			e_sCheck_Layer	= argv[i+1];	
			a_optcount[6]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-gou") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_GOU_POINT;
			e_sGou_Layer	= argv[i+1];	
			a_optcount[7]++;
			i+=2;
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

	// 2008.11.28 ���ϐ��ɑΉ�
	int	a_iInit_Ret	= 0;
	if( a_exeflag & ON_INIT_FILE ) {
		a_iInit_Ret	= e_pTool_Init.read_init_file ( Ret_Init_File() );
	}else {
		a_iInit_Ret	= e_pTool_Init.read_init_env ( );
	}
	if( a_iInit_Ret != 0 ) {
		fprintf ( stderr, "�������t�@�C��<%s>�̃I�[�v���Ɏ��s���܂���\n", Ret_Init_File() );
		return	( false );
	}

	// �I�v�V�����w�肪�����ꍇ�́A���ϐ�����擾���Ă݂�
	if( !(a_exeflag & ON_CITY_ADMIN) ) {
		e_sCheck_Layer	= e_pTool_Init.get_City_Admin();
	}
	if( !(a_exeflag & ON_GOU_POINT) ) {
		e_sGou_Layer	= e_pTool_Init.get_Gou_Point();
	}

	eExe_Flag	= a_exeflag;
//	return		( a_exeflag );
	return		( true );

}

/** ------------------------------------------------------------------------------
// �g�p�@�\��
--------------------------------------------------------------------------------*/
void		main_rootin::
print_usage	(char **argv)
{
	fprintf(	stdout,
				"=== �y�Z���ϊ����[�h�`�F�b�N�c�[���z(%s) ===\n"
				"<usage>\n"
				"%s [�I�v�V����...]\n"
				"\t<�I�v�V����>\n"
				"\t      '-i'              : �ݒ�t�@�C����\n"
				"\t      '-cadm'           : �s���E���C����(ex. SiNDY2008B.City_Admin)\n"
				"\t      '-gou'            : ���|�C���g���C����(ex. SiNDY2008B.Gou_Point)\n"
				"\t      '-l'              : ���O�t�@�C�������w��\n"
				"\t      '-code'           : �Z���R�[�h���w�肵�Ď��s�i8���A11���̂ݑΉ��j�iex 20209001101)\n"
				"\t      '-t'              : �o��11���R�[�h�e�L�X�g�t�@�C���iex .\\tmp\\�ϊ����[�h11��.txt�j\n"
				"\t                        �@���w�肵�Ȃ��ꍇ�͕W���o�͂ɏo�͂���\n"
				"\t      '-pgdb'           : �w��PGDB�̃f�[�^�ɑ΂��ă`�F�b�N�����s\n"
				"\t      '-v'              : �o�[�W�������\��\n"
				"\t[���ϐ�]\n"
				"\t  <����>\n"
				"\t      DB_ADDRESS        : <���[�U��>/<�p�X���[�h>/<�o�[�W����>/<�C���X�^���X>/<�T�[�o>\n"
				"\t      FC_CITY_ADMIN     : �s�s�n�}�s���EFC\n"
				"\t      FC_GOU_POINT      : ���|�C���gFC\n"
				"\t  ����L�̊��ϐ��ł̐ݒ��'-i'�I�v�V�����ł����l�ɐݒ�ł��܂�\n"
			//	"\t      '-m'           : ���b�V���R�[�h���X�g�t�@�C�����w��\n"
			//	"\t      '-M'           : ���b�V���R�[�h���w��i1���A2���A8�����b�V���j\n"
			//	"\t      '-w'           : Where����w��i��F\"OBJECTID = 1192\" �j\n"
			//	"\t<�⑫>\n"
			//	"\t      �E'-m','-M','-w'�͓����w��ł��܂���\n"
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
				"=== �y�Z���ϊ����[�h�`�F�b�N�c�[���z(%s) ===\n"
				"\t<�o�[�W�������>\n"
				"\t2004/05/20        1.00               �E ���J�o�[�W����\n"
				"\t2005/05/11        1.10               �E 8����v����Z���ɂ����ďZ���ϊ����[�h�̃t���O��\n"
				"\t                                        �����Ă�����̂�����ɂ�������炸�t���O��������\n"
				"\t                                        ���Ȃ�11���Z��������ꍇ���G���[�Ƃ��ă`�F�b�N��\n"
				"\t                                        ��悤�ɏC���B\n"
				"\t2006/08/01        1.20               �EArc9.1�ɑΉ�\n"
				"\t2008/12/03        2.0.0.1            �E VC8�ւ̈ڍs�����Ή��������̂őΉ�\n"
				"\t                                     �E �c�[���o�[�W�������ǉ�\n"
				"\t                                     �E ���O�w�b�_���ǉ�\n"
				"\t                                     �E PGDB�ɑΉ�('-pgdb'�I�v�V����)\n"
				"\t                                     �E ���ϐ��ł̐ݒ�ɑΉ�('-g','-ca'�ǉ�)\n"
				"\t                                     �E [Bug 6543]�ւ̑Ή��B(�Z���ϊ����[�h���u�厚�������v\n"
				"\t                                        �ɂ��Ă��Ȃ��s���E���G���[�ΏۂƂȂ�)\n"
				"\t                                     �E ���O�ɍ��^�C�v�̏���ǉ�\n"
				"\t2008/12/08        2.0.0.2            �E [Bug 6543]�ւ̍ďC��(9�`11���ڂ�'000'���`�F�b�N�Ώۂ�)\n"
				"\t2008/12/16        2.0.0.3            �E [Bug 6543]�ւ̍ďC��(�u�X�敄���v�u�g���X�敄���v��\��)\n"
				"\t2010/09/17        2.1.0.4            �E [bug 8229]�ւ̑Ή�(�����R�[�h�ڍs�ɑΉ����ė~����)\n"
				"\t                                        ���G���[���b�Z�[�W��ADDRCODE2�ւ̕ύX�ɂ��킹�ĕύX\n"
				"\t2010/10/29        2.1.1.5            �E [bug 8338]�ւ̑Ή�(Addrconvmode_chk�������d�l�ɂ��ė~����)\n"

				

//				"\t2004/03/25        1.00               : ���^�p�o�[�W����\n"
				, e_sVersion.c_str());
}

/** ------------------------------------------------------------------------------
	Where()����擾����
--------------------------------------------------------------------------------*/
string	main_rootin::
Get_Where_String(	char*	c_cpLayerName )		// ���C����
{
	string	a_sRet_Str	= "";
	if( c_cpLayerName == NULL ) {
		a_sRet_Str	= "";
		return	( a_sRet_Str );
	}

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

	//------------------------------------------------------
	// ���̑����l�ł̃`�F�b�N	2003.07.29
	//------------------------------------------------------
	// ���C�����Őڑ�WS�𔻒�
	string		a_sLayerName	= a_cpLayer;
	if( a_sLayerName == "City_Admin"	)	///< �s�s�n�}�s���E
	{
		a_sRet_Str	= "AREACLASS_C <> 2";
	}else {	// �s���E�ȊO
		a_sRet_Str	= "";
	}

	if( a_cpLayerTmp != NULL ) {
		delete [] a_cpLayerTmp;
	}
	return ( a_sRet_Str );
}

/** ------------------------------------------------------------------------------
	���C���I�[�v��
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Layers		( 	IWorkspacePtr	ipWorkspace_Addr )	// �Z���p���[�N�X�y�[�X
			//		IWorkspacePtr	ipWorkspace_Map )	// �n�}�p���[�N�X�y�[�X
{
	char*	a_cpCity_Admin_Layer	= NULL;	// �s�s�n�}�s���E���C����
	char*	a_cpGou_Point_Layer		= NULL;	// ���|�C���g���C����

	a_cpCity_Admin_Layer	= e_sCheck_Layer;
	a_cpGou_Point_Layer		= e_sGou_Layer;

	if( !e_mCity_Admin_WS.Init ( ipWorkspace_Addr, a_cpCity_Admin_Layer ) ) {
		fprintf	( stderr, "�s�s�n�}�s���E���C��<%s>�̓ǂݍ��݂Ɏ��s\n", a_cpCity_Admin_Layer );
		return	( false );
	}
	if( !e_mGou_Point_WS.Init ( ipWorkspace_Addr, a_cpGou_Point_Layer ) ) {
		fprintf	( stderr, "���|�C���g���C��<%s>�̓ǂݍ��݂Ɏ��s\n", a_cpGou_Point_Layer );
		return	( false );	
	}
	return	( true );
}

/**
	�s���R�[�h���X�g�����Z�b�g�ɗ��Ƃ�
	@return	�f�[�^��
*/
int		main_rootin::
Set_AdminCode_Data (	IFeatureCursorPtr&	c_ipAdminCursor )	///< �s���E�J�[�\��
{
	int	a_iCount	= 0;
	// �f�[�^���}���`�Z�b�g�̃f�[�^�ɗ��Ƃ�
	if( c_ipAdminCursor != NULL ) {
		IFeaturePtr	a_ipNote_Feat;
		// �������ꂽ���L�̃��[�v
		for(;;)
		{
			data_info	a_dAdmin_Data;
			if( c_ipAdminCursor->NextFeature ( &a_ipNote_Feat ) == S_OK ) {

				Feature_All_Attrs	a_fNote_Attrs;
				a_fNote_Attrs.Init	( a_ipNote_Feat );
				// �I�u�W�F�N�gID
				Feature_Attr		a_fObjID;
				a_fObjID			= a_fNote_Attrs.Get_Attr ( "OBJECTID" );
				// �s�撬���R�[�h
				Feature_Attr		a_fCityCode;
				a_fCityCode			= a_fNote_Attrs.Get_Attr ( "CITYCODE" );
				// �Z���R�[�h
				Feature_Attr		a_fAddrCode;
				a_fAddrCode			= a_fNote_Attrs.Get_Attr ( "ADDRCODE" );
				// �Z���R�[�h2
				Feature_Attr		a_fAddrCode2;
				a_fAddrCode2		= a_fNote_Attrs.Get_Attr ( "ADDRCODE2" );
				// �ϊ��^�C�v
			//	Feature_Attr		a_fConvCode;
			//	a_fConvCode			= a_fNote_Attrs.Get_Attr ( "CONVTYPE_C" );

				a_dAdmin_Data.e_iOID		= a_fObjID.e_ccvAttr.intVal;
				a_dAdmin_Data.e_sCityCode	= Str_to_SJIS ((CComBSTR&)a_fCityCode.e_ccvAttr.bstrVal);
				a_dAdmin_Data.e_sAddrCode	= Str_to_SJIS ((CComBSTR&)a_fAddrCode.e_ccvAttr.bstrVal);
				a_dAdmin_Data.e_sAddrCode2	= Str_to_SJIS ((CComBSTR&)a_fAddrCode2.e_ccvAttr.bstrVal);
			//	a_dAdmin_Data.e_iConvMode	= a_fConvCode.e_ccvAttr.intVal;

				e_mAdmin_Datas.insert( a_dAdmin_Data );		///< �ϊ����[�h�m�F
				e_mAdmin_Datas_Uniq.insert( a_dAdmin_Data );///< 11�����j�[�N

				string	a_sCode_8;	// 8���R�[�h
				a_sCode_8	= a_dAdmin_Data.e_sCityCode;
				char	a_cpCode_3[4];
				memset	( a_cpCode_3, '\0', 4 );
				strncpy ( a_cpCode_3, &a_dAdmin_Data.e_sAddrCode[0], 3 );
				a_sCode_8	+= a_cpCode_3;
				e_sCode_8.insert ( a_sCode_8 );

				a_iCount++;
			}else {
				break;
			}
		}
	}
	return	( a_iCount );
}

/** ------------------------------------------------------------------------------
	11���R�[�h���X�g�̏o��
--------------------------------------------------------------------------------*/
void	main_rootin::
Print_Code_List	( void )
{
	fprintf	( stdout, "11���R�[�h���X�g�쐬���E�E�E\n" );
	FILE*	a_fpOutput	= NULL;
	if( eExe_Flag & ON_CODE_LIST_TXT ) {
		a_fpOutput	= fopen ( e_cpCode_List_Txt, "w" );
		if( a_fpOutput == NULL ) {
			fprintf	(	stderr, "�R�[�h���X�g�o�̓t�@�C��<%s>���I�[�v���ł��܂���I�W���o�͂ɏo�͂��܂�\n",
						e_cpCode_List_Txt );
			a_fpOutput	= stdout;
		}
	}else {
		a_fpOutput	= stdout;
	}

	is_data_info	a_iIndx;
	for( a_iIndx = e_mAdmin_Datas_Uniq.begin(); a_iIndx != e_mAdmin_Datas_Uniq.end(); a_iIndx++ )
	{
		data_info	a_dCurDat	= *a_iIndx;
		fprintf	( a_fpOutput, "%s%s\n", &a_dCurDat.e_sCityCode[0], &a_dCurDat.e_sAddrCode[0] );  
	}

	if( a_fpOutput != stderr ) {
		if( e_cpCode_List_Txt == NULL ) {
			fprintf	( stdout, "11���R�[�h��stdout�ɏo�͂��܂���\n" );
		}else {
			fprintf	( stdout, "11���R�[�h���t�@�C��<%s>�ɏo�͂��܂���\n", e_cpCode_List_Txt );
		}
		fclose	( a_fpOutput );
	}
}

/** ------------------------------------------------------------------------------
	�ϊ����[�h�R�[�h�G���[�̃`�F�b�N
--------------------------------------------------------------------------------*/
void	main_rootin::
Chk_ConvMode	( void )
{
	char	a_cpMsg[256];	// ���b�Z�[�W�p

	e_sPrnInfo.Set_Layer_Name	( e_sCheck_Layer );

	ims_data_info	a_iIndx;
	for( a_iIndx = e_mAdmin_Datas.begin(); a_iIndx != e_mAdmin_Datas.end(); a_iIndx++ )
	{
		data_info	a_dDat	= *a_iIndx;

		e_sPrnInfo.Set_OID ( a_dDat.e_iOID );

		int		a_iCode_9_11	= 0;

		a_iCode_9_11	= atoi ( &a_dDat.e_sAddrCode[3] );	// 2004.05.14 CityCode�ɂȂ��Ă��̂ŏC��
		
		int		a_iEStat	= 0;

		// [bug 8338] �B�Ή��̂��ߒǉ�
		if( a_dDat.e_sAddrCode2 != "000000" &&
			a_dDat.e_sAddrCode == a_dDat.e_sAddrCode2 )
		{
			sprintf	( a_cpMsg, "ADDRCODE��ADDRCODE2�������R�[�h\t%s%s", a_dDat.e_sCityCode.c_str(), a_dDat.e_sAddrCode.c_str() );
			e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_ERROR );
			e_sPrnInfo.Set_Err_Msg		( a_cpMsg );
			e_sPrnInfo.Prn_SiNDY_Err	( 0, NULL, stderr );
		}

		// [bug 8338] �@�̑Ή��̂��߁A�R�����g�A�E�g
//	//	if( a_dDat.e_iConvMode == 1 &&
//		if( a_dDat.e_sAddrCode2 != "000000" &&
//	//		!(101 <= a_iCode_9_11 && a_iCode_9_11 <= 850) ) {
//			!(101 <= a_iCode_9_11 && a_iCode_9_11 <= 899) ) {
//	//		sprintf	( a_cpMsg, "�X�`�P�P���̃R�[�h����������\t%s%s\t�ϊ����[�h%d",
//	//				  &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0], a_dDat.e_iConvMode );
//			sprintf	( a_cpMsg, "�X�`�P�P���̃R�[�h����������\t%s%s\tADDRCODE2\t<%s>",
//					  &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0], a_dDat.e_sAddrCode2.c_str() );
//			e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_ERROR );
//			e_sPrnInfo.Set_Err_Msg		( a_cpMsg );
//			e_sPrnInfo.Prn_SiNDY_Err	( 0, NULL, stderr );
//
//		}
		// �ϊ����[�h2�͖����̂ŃR�����g�A�E�g 2010.09.15
	//	else
	//	if( a_dDat.e_iConvMode == 2 ) {
	//		sprintf	( a_cpMsg, "�ϊ����[�h��%d\t%s%s", a_dDat.e_iConvMode, &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0] );
	//		e_sPrnInfo.Set_Err_Level	( smp_sindy_err_info::EC_WARNING);
	//		e_sPrnInfo.Set_Err_Msg		( a_cpMsg );
	//		e_sPrnInfo.Prn_SiNDY_Err	( 0, NULL, stderr );
	//	}
	}

	// �G���[���x����߂�
	e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_ERROR );
}

/** ------------------------------------------------------------------------------
	11������v����s���E�̒��ŁA�ϊ����[�h�R�[�h���Ⴄ���̂��`�F�b�N����
--------------------------------------------------------------------------------*/
void	main_rootin::
Chk_SameCode11Admin ( void )
{
	char	a_cpMsg[256];	// ���b�Z�[�W�p

	e_sPrnInfo.Set_Layer_Name	( e_sCheck_Layer );

	is_data_info	a_iIndx;
	
	fprintf ( stdout, "11������v����s���E���`�F�b�N���܂�\n" );

	for( a_iIndx = e_mAdmin_Datas_Uniq.begin(); a_iIndx != e_mAdmin_Datas_Uniq.end(); a_iIndx++ )
	{
		data_info	a_dDat	= *a_iIndx;

		fprintf	( stdout, "�s���R�[�h<%s%s>�ϊ����[�h�`�F�b�N��\n", &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0] );

		// �Z���f�[�^�̒��ŁA�ϊ����[�h��1��2�̂��̂�����Ă���
		char	a_cpWhereStr[128];
//		sprintf	( a_cpWhereStr, "CITYCODE = '%s' AND ADDRCODE = '%s' AND CONVTYPE_C <> %d",
//				  &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0], a_dDat.e_iConvMode );
		sprintf	( a_cpWhereStr, "CITYCODE = '%s' AND ADDRCODE = '%s' AND ADDRCODE2 = '000000'",
				  &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0] );
		string	a_sWhere_Str= a_cpWhereStr; 
		IFeatureCursorPtr	a_ipAdminCursor;

		a_ipAdminCursor	= e_mCity_Admin_WS.Search_Where ( a_sWhere_Str, RET_FCURSOR );
		if ( a_ipAdminCursor == NULL ) {
			e_sPrnInfo.Prn_Info	( "�w��Z���R�[�h�̍s���E���擾�ł��܂���ł���\n", stderr );
			continue;
		}
		int	i = 0;
		IFeaturePtr		a_ipCity_Admin_Feat;
		for (;;i++)
		{
			if( a_ipAdminCursor->NextFeature ( &a_ipCity_Admin_Feat ) == S_OK ) {
				// �t�B�[�`���̑��������o��
				Feature_All_Attrs	a_fAdmin_Attrs;
				a_fAdmin_Attrs.Init( a_ipCity_Admin_Feat );
			
				// �I�u�W�F�N�gID
				Feature_Attr	a_fObjID;
				a_fObjID		= a_fAdmin_Attrs.Get_Attr ( "OBJECTID" );

			//	Feature_Attr	a_fConvCode;
			//	a_fConvCode		= a_fAdmin_Attrs.Get_Attr ( "CONVTYPE_C" );	// �ϊ��^�C�v

				Feature_Attr	a_fAddrCode2;
				a_fAddrCode2	= a_fAdmin_Attrs.Get_Attr ( "ADDRCODE2" );

				string	a_sAddrCode2;
				a_sAddrCode2	= Str_to_SJIS ((CComBSTR&)a_fAddrCode2.e_ccvAttr.bstrVal);

				e_sPrnInfo.Set_OID ( a_fObjID.e_ccvAttr.intVal );

			//	if( a_dDat.e_iConvMode != a_fConvCode.e_ccvAttr.intVal ) {
				if( a_sAddrCode2 == "000000" ) {
				//	sprintf	( a_cpMsg, "����11���R�[�h�Ȃ̂ɕϊ����[�h���Ⴄ\t���[�h%d\t%s%s",
				//			  a_fConvCode.e_ccvAttr.intVal, &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0] );
					sprintf	( a_cpMsg, "����11���R�[�h�Ȃ̂ɕϊ����[�h���Ⴄ\t%s%s", &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0] );
					e_sPrnInfo.Set_Err_Level	( smp_sindy_err_info::EC_ERROR);
					e_sPrnInfo.Set_Err_Msg		( a_cpMsg );
					e_sPrnInfo.Prn_SiNDY_Err	( 0, NULL, stderr );
				}
			}else {
				break;
			}
		}
	}
}

/**
	8������v����s���E�̒��ŁA�ϊ����[�h���Ⴄ���̂��`�F�b�N����
*/
void	main_rootin::
Chk_SameCode8Admin_Mode ( void )
{
	char	a_cpMsg[256];	// ���b�Z�[�W�p

	e_sPrnInfo.Set_Layer_Name	( e_sCheck_Layer );

	
	fprintf ( stdout, "8������v����s���E���`�F�b�N���܂�\n" );
	is_string	a_iIndx;
	for( a_iIndx = e_sCode_8.begin(); a_iIndx != e_sCode_8.end(); a_iIndx++ )
	{
		string	a_sDat	= *a_iIndx;

		fprintf	( stdout, "�s���R�[�h<%s>�ϊ����[�h�`�F�b�N��\n", &a_sDat[0]);

		char		a_cpCode_5[6], a_cpCode_3[4];
		memset	( a_cpCode_5, '\0', 6 );
		memset	( a_cpCode_3, '\0', 4 );

		strncpy	( a_cpCode_5, &a_sDat[0], 5 );
		strncpy	( a_cpCode_3, &a_sDat[5], 3 );

		// �Z���f�[�^�̒��ŁA�ϊ����[�h��1��2�̂��̂�����Ă���
		char	a_cpWhereStr[128];
		sprintf	( a_cpWhereStr, "CITYCODE = '%s' AND ADDRCODE LIKE '%s%s%s%s'", a_cpCode_5, a_cpCode_3, WCARD1, WCARD1, WCARD1 );
		string	a_sWhere_Str= a_cpWhereStr; 
		IFeatureCursorPtr	a_ipAdminCursor;

		a_ipAdminCursor	= e_mCity_Admin_WS.Search_Where ( a_sWhere_Str, RET_FCURSOR );
		if ( a_ipAdminCursor == NULL ) {
			e_sPrnInfo.Prn_Info	( "�w��Z���R�[�h�̍s���E���擾�ł��܂���ł���\n", stderr );
			continue;
		}

		int	i = 0;
		IFeaturePtr		a_ipCity_Admin_Feat;
		for (;;i++)
		{
			if( a_ipAdminCursor->NextFeature ( &a_ipCity_Admin_Feat ) == S_OK ) {
				// �t�B�[�`���̑��������o��
				Feature_All_Attrs	a_fAdmin_Attrs;
				a_fAdmin_Attrs.Init( a_ipCity_Admin_Feat );
			
				// �I�u�W�F�N�gID
				Feature_Attr	a_fObjID;
				a_fObjID		= a_fAdmin_Attrs.Get_Attr ( "OBJECTID" );

			//	Feature_Attr	a_fConvCode;
			//	a_fConvCode		= a_fAdmin_Attrs.Get_Attr ( "CONVTYPE_C" );	// �ϊ��^�C�v

				// �s�撬���R�[�h
				Feature_Attr	a_fCityCode;
				a_fCityCode		= a_fAdmin_Attrs.Get_Attr ( "CITYCODE" );
				// �Z���R�[�h
				Feature_Attr	a_fAddrCode;
				a_fAddrCode		= a_fAdmin_Attrs.Get_Attr ( "ADDRCODE" );

				Feature_Attr	a_fAddrCode2;
				a_fAddrCode2	= a_fAdmin_Attrs.Get_Attr ( "ADDRCODE2" );

				string	a_sCityCode, a_sAddrCode, a_sAddrCode2;
				a_sCityCode	= Str_to_SJIS ((CComBSTR&)a_fCityCode.e_ccvAttr.bstrVal);
				a_sAddrCode	= Str_to_SJIS ((CComBSTR&)a_fAddrCode.e_ccvAttr.bstrVal);
				a_sAddrCode2= Str_to_SJIS ((CComBSTR&)a_fAddrCode2.e_ccvAttr.bstrVal);

				e_sPrnInfo.Set_OID ( a_fObjID.e_ccvAttr.intVal );

				//���R���̒l�����o��
				int		a_iCode_9_11	= 0;
				a_iCode_9_11	= atoi ( &a_sAddrCode[3] );

				// �Z���ϊ����[�h��1����Ȃ��ĉ��R����'101'�`'850'�̏ꍇ
			//	if( a_fConvCode.e_ccvAttr.intVal != 1 &&
				if( (a_sAddrCode2 == "000000" ) &&
				//	(101 <= a_iCode_9_11 && a_iCode_9_11 <= 850) ) {
					(101 <= a_iCode_9_11 && a_iCode_9_11 <= 899) ) {
				//	sprintf	( a_cpMsg, "8����v����Z���ɂ����ďZ���ϊ����[�h�̃t���O�������Ă��Ȃ�\t���[�h%d\t%s%s",
				//		a_fConvCode.e_ccvAttr.intVal, &a_sCityCode[0], &a_sAddrCode[0] );
					sprintf	( a_cpMsg, "8����v����Z���ɂ�����ADDRCODE2�ɒl�������Ă��Ȃ�\t%s%s", &a_sCityCode[0], &a_sAddrCode[0] );
					e_sPrnInfo.Set_Err_Level	( smp_sindy_err_info::EC_ERROR);
					e_sPrnInfo.Set_Err_Msg		( a_cpMsg );
					e_sPrnInfo.Prn_SiNDY_Err	( 0, NULL, stderr );
				}
			}else {
				break;
			}
		}
	}
}

/** ------------------------------------------------------------------------------
// �s���E�̃��X�g���擾����i�W�I���g���܂ށj
--------------------------------------------------------------------------------*/
void	main_rootin::
Set_Admin_List	( IFeatureCursorPtr&	c_ipAdminCursor,
				  v_admin_info&			c_vAdmLists )
{
	IFeaturePtr		a_ipCity_Admin_Feat;

	int	i = 0;
	for (;;i++)
	{
		if( c_ipAdminCursor->NextFeature ( &a_ipCity_Admin_Feat ) == S_OK ) {

			// �t�B�[�`���̑��������o��
			Feature_All_Attrs	a_fAdm_Attrs;
			a_fAdm_Attrs.Init( a_ipCity_Admin_Feat );

			// �I�u�W�F�N�gID
			Feature_Attr		a_fAdminObjID;
			a_fAdminObjID		= a_fAdm_Attrs.Get_Attr ( "OBJECTID" );
			// �s�撬���R�[�h
			Feature_Attr		a_fCityCode;
			a_fCityCode			= a_fAdm_Attrs.Get_Attr ( "CITYCODE" );
			// �Z���R�[�h
			Feature_Attr		a_fAddrCode;
			a_fAddrCode			= a_fAdm_Attrs.Get_Attr ( "ADDRCODE" );
			// �Z���R�[�h2
			Feature_Attr		a_fAddrCode2;
			a_fAddrCode2		= a_fAdm_Attrs.Get_Attr ( "ADDRCODE2" );
			// �X�敄��
			Feature_Attr		a_fGaiku;
			a_fGaiku			= a_fAdm_Attrs.Get_Attr	( "GAIKUFUGO");		// 2008.12.16 [Bug 6543]�Ή�
			// �g���X�敄��
			Feature_Attr		a_fExtGaiku;
			a_fExtGaiku			= a_fAdm_Attrs.Get_Attr	( "EXTGAIKUFUGO");	// 2008.12.16 [Bug 6543]�Ή�

			string	a_sCityCode, a_sAddrCode, a_sAddrCode2;
			a_sCityCode	= Str_to_SJIS ((CComBSTR&)a_fCityCode.e_ccvAttr.bstrVal);
			a_sAddrCode	= Str_to_SJIS ((CComBSTR&)a_fAddrCode.e_ccvAttr.bstrVal);
			a_sAddrCode2= Str_to_SJIS ((CComBSTR&)a_fAddrCode2.e_ccvAttr.bstrVal);

			IGeometryPtr		a_ipAdminGeo;

			if( FAILED(a_ipCity_Admin_Feat->get_Shape ( &a_ipAdminGeo )) ) {	// �t�B�[�`������
				fprintf	(stderr, "�V�F�C�v�̎擾�Ɏ��s\n");
				continue;
			}

			admin_info		a_aDat;
			
			a_aDat.e_iOID	= a_fAdminObjID.e_ccvAttr.intVal;
			a_aDat.e_ipGeom	= a_ipAdminGeo;
			a_aDat.e_sCode1	= a_sCityCode + a_sAddrCode;
			a_aDat.e_sCode2	= a_sCityCode + a_sAddrCode2;

			//------------------------------------------------------------------------
			// 2008.12.16 [Bug 6543]�Ή�
			if( a_fGaiku.Is_Null() ) {
				a_aDat.e_lGaiku	= -1;
			}else {
				a_aDat.e_lGaiku	= a_fGaiku.e_ccvAttr.intVal;
			}
			if( a_fExtGaiku.Is_Null() ) {
				a_aDat.e_sExtGaiku	= "";
			}else {
				a_aDat.e_sExtGaiku	= Str_to_SJIS ((CComBSTR&)a_fExtGaiku.e_ccvAttr.bstrVal);
			}
			//------------------------------------------------------------------------

			c_vAdmLists.push_back ( a_aDat );
		
		}else {
			break;
		}
	}
}

/** ------------------------------------------------------------------------------
	���|�C���g�̃��X�g���쐬����
--------------------------------------------------------------------------------*/
int		main_rootin::
Set_Gou_Datas	( v_admin_info&			c_vAdmLists, 		///< �s���E
				  ms_gou_info&			c_mGou_Infos )		///< ���o�������f�[�^
{

	IFeaturePtr		a_ipCity_Admin_Feat;

	// ���ԍ���NULL�̂��̂͏���
	char	a_cpWhereStr[128];
	sprintf	( a_cpWhereStr,	"GOU_NO IS NOT NULL" );
	string	a_sWhere_Str= a_cpWhereStr; 

	iv_admin_info	a_iIndx;
	for( a_iIndx = c_vAdmLists.begin(); a_iIndx != c_vAdmLists.end(); a_iIndx++ )
	{
		admin_info	a_aAdmDat	= *a_iIndx;
	
			
		IFeatureCursorPtr	a_ipGouCursor;

		a_ipGouCursor	= e_mGou_Point_WS.Search_Feature ( a_aAdmDat.e_ipGeom, esriSpatialRelContains, a_sWhere_Str, RET_FCURSOR );
		if ( a_ipGouCursor == NULL ) {
			e_sPrnInfo.Prn_Info	( "�w��Z���R�[�h�̍s���E���擾�ł��܂���ł���\n", stderr );
			continue;
		}

		IFeaturePtr		a_ipGou_Feat;
		int	j = 0;
		for (;;j++)
		{
			if( a_ipGouCursor->NextFeature ( &a_ipGou_Feat ) == S_OK ) {
				
				// �t�B�[�`���̑��������o��
				Feature_All_Attrs	a_fGou_Attrs;
				a_fGou_Attrs.Init( a_ipGou_Feat );
			
				// �I�u�W�F�N�gID
				Feature_Attr	a_fObjID;
				a_fObjID		= a_fGou_Attrs.Get_Attr ( "OBJECTID" );
				// ���f�[�^
				Feature_Attr	a_fGou_NO;
				a_fGou_NO		= a_fGou_Attrs.Get_Attr ( "GOU_NO" );	// �ϊ��^�C�v
				// ���^�C�v 2008.12.03
				Feature_Attr	a_fGouType;
				a_fGouType		= a_fGou_Attrs.Get_Attr ( "GOUTYPE");
			
				e_sPrnInfo.Set_OID ( a_fObjID.e_ccvAttr.intVal );
				
				gou_info			a_gDat;
				a_gDat.e_iOID		= a_fObjID.e_ccvAttr.intVal;
				a_gDat.e_sCode11_1	= a_aAdmDat.e_sCode1;
				a_gDat.e_sCode11_2	= a_aAdmDat.e_sCode2;
				a_gDat.e_sChiban	= "";
				a_gDat.e_sChiban	= Str_to_SJIS ((CComBSTR&)a_fGou_NO.e_ccvAttr.bstrVal);
				a_gDat.e_iGouType	= a_fGouType.e_ccvAttr.intVal;
				//------------------------------------------------------------------------
				// 2008.12.16 [Bug 6543]�Ή�
				a_gDat.e_lGaiku		= a_aAdmDat.e_lGaiku;
				a_gDat.e_sExtGaiku	= a_aAdmDat.e_sExtGaiku;					
				//------------------------------------------------------------------------
				c_mGou_Infos.insert ( a_gDat );
			}else {
				break;
			}
		}
	}
	return	( 0 );
}

/** ------------------------------------------------------------------------------
	�Z�b�g�����č��̏d�����`�F�b�N����
--------------------------------------------------------------------------------*/
void	main_rootin::
Chk_Gou_Dupli	( ms_gou_info& c_mGou_Infos )
{

	char	a_cpMsg[256];	// ���b�Z�[�W�p

	e_sPrnInfo.Set_Layer_Name	( e_sGou_Layer );

	ims_gou_info	a_iInfo;
	// ��������̃Z�b�g��p�ӂ���
	set<string,less<string> >	a_sGouSet;
	for( a_iInfo = c_mGou_Infos.begin(); a_iInfo != c_mGou_Infos.end(); a_iInfo++ )
	{
		gou_info	a_gDat		= *a_iInfo;
		a_sGouSet.insert ( a_gDat.e_sChiban );
//		printf("%s\t%s\t%d\n", &a_gDat.e_sChiban[0], &a_gDat.e_sCode11[0], a_gDat.e_iOID );
	}

	e_sPrnInfo.Set_Err_Level	( smp_sindy_err_info::EC_ERROR );

	// ����������L�[�ɂ��ă`�F�b�N����
	set<string,less<string> >::iterator		a_iGIndx;
	for( a_iGIndx = a_sGouSet.begin(); a_iGIndx != a_sGouSet.end(); a_iGIndx++ )
	{
		string		a_sChiban	= *a_iGIndx;
		gou_info	a_gSKey;
		// �T�[�`�L�[
		a_gSKey.e_sChiban	= a_sChiban;	// ��������
		a_gSKey.e_sCode11_1	= "00000000000";
		a_gSKey.e_sCode11_2	= "00000000000";

		ims_gou_info	a_iBIndx, a_iEIndx;	// �J�n�E�I���C���f�b�N�X
		a_iBIndx	= c_mGou_Infos.lower_bound( a_gSKey );
		a_gSKey.e_sCode11_1	= "99999999999";
		a_gSKey.e_sCode11_2	= "99999999999";
		a_iEIndx	= c_mGou_Infos.upper_bound( a_gSKey );

		ims_gou_info	a_iGIIndx;
		int			roop = 0;
		gou_info	a_sCompare;
		bool		a_bErrStat	= false;
		for( a_iGIIndx = a_iBIndx; a_iGIIndx != a_iEIndx; a_iGIIndx++, roop++ )
		{
			gou_info	a_gGouInfo	= *a_iGIIndx;
			if( roop == 0 )	{
				a_sCompare	= a_gGouInfo;
			}else {
				// 11���R�[�h���Ⴄ�ꍇ�A�G���[�X�e�[�g�ɂ��ă��[�v�𔲂���
				// [bug 8338]�A�Ή��ŏC��
				if( a_sCompare.e_sCode11_1 != a_gGouInfo.e_sCode11_1 &&
					(a_sCompare.e_sCode11_2 == a_gGouInfo.e_sCode11_1 ||
					((a_sCompare.e_sCode11_2 == a_gGouInfo.e_sCode11_2) && a_sCompare.e_sCode11_2 != "000000")) ) {
					a_bErrStat	= true;
					break;
				}
			}
		}
		// �G���[���������ꍇ
		if( a_bErrStat == true ) {
			for( a_iGIIndx = a_iBIndx; a_iGIIndx != a_iEIndx; a_iGIIndx++ )
			{
				gou_info	a_gCurDat	= *a_iGIIndx;

				e_sPrnInfo.Set_OID ( a_gCurDat.e_iOID );

				//------------------------------------------------------------------------
				// 2008.12.16 [Bug 6543]�Ή� �X�敄���A�g���X�敄���̏o�͗p				
				char		a_cpGaikuStr[64];
				if( a_gCurDat.e_lGaiku >= 0 ) {
					sprintf	( a_cpGaikuStr, "%d", a_gCurDat.e_lGaiku );
				}else {
					// �ǂ������̏ꍇ�́A�g���X�敄����""��a_cpGaikuStr�ɓ���
					sprintf	( a_cpGaikuStr, "%s", a_gCurDat.e_sExtGaiku.c_str() );
				}
				//------------------------------------------------------------------------

				sprintf	( a_cpMsg, "8����v��11�����Ⴄ�s���E�ō����d��\t%s\t%d\t%s\t%s",
						  a_gCurDat.e_sChiban.c_str(), a_gCurDat.e_iGouType, a_gCurDat.e_sCode11_1.c_str(), a_cpGaikuStr );
				e_sPrnInfo.Set_Err_Msg		( a_cpMsg );
				e_sPrnInfo.Prn_SiNDY_Err	( 0, NULL, stderr );
			}
		}
	}
}

/** ------------------------------------------------------------------------------
	8������v����s���E�̒��ŁA�ʃ|���S���ɓ������|�C���g�����邩�̃`�F�b�N
--------------------------------------------------------------------------------*/
void	main_rootin::
Chk_SameCode8Admin ( void )
{
//	char	a_cpMsg[256];	// ���b�Z�[�W�p

	e_sPrnInfo.Set_Layer_Name	( e_sCheck_Layer );

	is_string	a_iIndx;
	for( a_iIndx = e_sCode_8.begin(); a_iIndx != e_sCode_8.end(); a_iIndx++ )
	{
		string	a_sDat	= *a_iIndx;

		char		a_cpCode_5[6], a_cpCode_3[4];
		memset	( a_cpCode_5, '\0', 6 );
		memset	( a_cpCode_3, '\0', 4 );

		strncpy	( a_cpCode_5, &a_sDat[0], 5 );
		strncpy	( a_cpCode_3, &a_sDat[5], 3 );

		fprintf	( stdout, "�s���R�[�h<%s>���|�C���g�`�F�b�N��\n", &a_sDat[0]);

		// 8������v����s���E������Ă���
		char	a_cpWhereStr[128];
		// 2008.11.12 [Bug 6543]�ւ̑Ή��B(�Z���ϊ����[�h���u�厚�������v�ɂ��Ă��Ȃ��s���E���G���[�ΏۂƂȂ�) 
	//	sprintf	( a_cpWhereStr, "CITYCODE = '%s' AND ADDRCODE LIKE '%s%s%s%s' AND CONVTYPE_C <> 0", a_cpCode_5, a_cpCode_3, WCARD1, WCARD1, WCARD1 );
	//	sprintf	( a_cpWhereStr, "(CITYCODE = '%s' AND ADDRCODE LIKE '%s%s%s%s' AND CONVTYPE_C <> 0) OR "
		sprintf	( a_cpWhereStr, "(CITYCODE = '%s' AND ADDRCODE LIKE '%s%s%s%s' AND ADDRCODE2 <> '000000') OR "			 
								"(CITYCODE = '%s' AND ADDRCODE = '%s000')", a_cpCode_5, a_cpCode_3, WCARD1, WCARD1, WCARD1, a_cpCode_5, a_cpCode_3 );
		string	a_sWhere_Str= a_cpWhereStr;
		IFeatureCursorPtr	a_ipAdminCursor;

		a_ipAdminCursor	= e_mCity_Admin_WS.Search_Where ( a_sWhere_Str, RET_FCURSOR );
		if ( a_ipAdminCursor == NULL ) {
			e_sPrnInfo.Prn_Info	( "�w��Z���R�[�h�̍s���E���擾�ł��܂���ł���\n", stderr );
			continue;
		}

		// ����������̃`�F�b�N ----------------------------------------------
		v_admin_info	a_vAdmLists;

		// �s���E�̃��X�g���擾����i�W�I���g���܂ށj
		Set_Admin_List	( a_ipAdminCursor, a_vAdmLists ); 

		ms_gou_info		a_mGou_Infos;

		// ���f�[�^������Ă���
		if( Set_Gou_Datas ( a_vAdmLists, a_mGou_Infos ) == 0 ) {
		}

		// ���`�F�b�N��
		Chk_Gou_Dupli	( a_mGou_Infos );
		// �����܂� ------------------------------------------------------------
	}
}



/** ------------------------------------------------------------------------------
	�s���E�̃��[�v
--------------------------------------------------------------------------------*/
void	main_rootin::
Admin_Check_Roops ( IFeatureCursorPtr&	c_ipAdminCursor	) 	///< �s���E�J�[�\��
{
//	char	a_cpMsg[256];	// ���b�Z�[�W�p

	// �s���E�̃��[�v
	int	i=0;
	IFeaturePtr		a_ipCity_Admin_Feat;
	int				a_iCityName_Count	= 0;
	int				a_iCityOffice_Count	= 0;

	// 11���R�[�h���̏����Z�b�g�ɂ���
	Set_AdminCode_Data ( c_ipAdminCursor );
	// 11���R�[�h���X�g�̏o��
	Print_Code_List ( );
	// �ϊ����[�h�R�[�h�G���[�̃`�F�b�N
	Chk_ConvMode ( );
	// 11������v����s���E�̒��ŁA�ϊ����[�h�R�[�h���Ⴄ���̂��`�F�b�N����
	Chk_SameCode11Admin ( );
	// 8������v����s���E�̒��ŁA�ϊ����[�h�R�[�h���Ⴄ���̂��`�F�b�N����
	Chk_SameCode8Admin_Mode ( );
	// 8������v����s���E�̒��ŁA���|�C���g���������̂����̍s���E�ɂ��邩�ǂ����̃`�F�b�N
	Chk_SameCode8Admin ( );
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
//	HRESULT				hr;

	uh::flush_scope_guard fsg;

	// �J�n���Ԃ̐ݒ�
	e_eExeTime.Set_Start();

	// COM ���g�p�ł���悤�ɂ��邽�߂̏�����
//	::CoInitialize(0);

	// SiNDY�̏������ݒ�
//	prog_init		a_pTool_Init;
//	if( a_pTool_Init.read_init_file ( Ret_Init_File() ) != 0 ) {
//		fprintf( stderr, "�������t�@�C��<%s>�̃I�[�v���Ɏ��s���܂���\n", Ret_Init_File());
//		return ( false );
//	}
	// COM ���g�p�ł���悤�ɂ��邽�߂̏�����
//	::CoInitialize(0);
	USES_CONVERSION;	// OLE2T ���̈ꕔ�̃}�N�����g�p����ꍇ�ɂ͕K���K�v

	IWorkspacePtr			ipWorkspace_Addr;
	if( eExe_Flag & ON_PGDB_MODE ) {
		// �p�[�\�i���W�IDB�p�ł�
		IWorkspaceFactoryPtr	ipWorkspaceFactory_Addr	( CLSID_AccessWorkspaceFactory );
		IPropertySetPtr			ipPropertySet_Addr		( CLSID_PropertySet );					// �ڑ��v���p�e�B�[�i�[�p
		_bstr_t					a_bsTmpFName;
		a_bsTmpFName			= e_cpPGDB_File;
		// �J�����g�����ɃI�[�v�����Ă݂�
		ipWorkspaceFactory_Addr->OpenFromFile( a_bsTmpFName, 0, &ipWorkspace_Addr );
		if( ipWorkspace_Addr == NULL )	{
			fprintf ( stderr, "�p�[�\�i���W�IDB���I�[�v���ł��܂���ł���\n"); 
	//		End	();
		}else {
			printf(_T("�ڑ�����"));
		}
		printf(_T("\n"));
	}else {
		// �s���E�p�ڑ�

		if( !SiNDY_DB_Open	( ipWorkspace_Addr )) {
			fprintf	( stderr, "�ڑ��o���Ȃ�����\n");
			return	( false );
		}else {
			fprintf	( stdout, "�ڑ�����\n" );
		}
	}

	// DB�ڑ��ɂ����������Ԃ̕\��
	fprintf	( stdout,	"SiNDY-DB�ڑ��������� = %0.2f(s)�ł���\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );

	// �G���[���O�t�@�C���̃I�[�v��
	if( !Open_Err_Log () ) {
		return	( false );
	}else {
		if( !e_sPrnInfo.Init(e_fErr_Fp) ) {
			return	( false );
		}
	}

	// ���O�̃w�b�_�����o��
	Output_Log_Header( );

	// ���C���I�[�v��
	if( !Open_Layers ( ipWorkspace_Addr ) ){	// �n�}�p���[�N�X�y�[�X
		e_sPrnInfo.Prn_Info	( "���C���I�[�v���Ɏ��s���Ă��܂�", stderr );
		return	( false );
	}


	char	a_cpWhereStr[128];

	// �����ň����|�������Z���R�[�h��Where��ɂ���@2004/05/14
	if( eExe_Flag & ON_MONO_ADCODE ) {
		char	a_cpStr_5[6];
		char	a_cpStr_6[7];
		memset	( a_cpStr_5, '\0', 6 );
		memset	( a_cpStr_6, '\0', 7 );
		int	a_iLeng	= 0;
		a_iLeng	= strlen ( e_cpAddrCode );
		if( a_iLeng != 8 && a_iLeng != 11 ) {
			e_sPrnInfo.Prn_Info	( "�w��Z���R�[�h�̌����͑Ή��O�ł�\n", stderr );
			return	( false );
		}
		strncpy	( a_cpStr_5, e_cpAddrCode, 5 );
		if( a_iLeng == 8 ) {
			strncpy	( a_cpStr_6, e_cpAddrCode+5, 3 );
		//	sprintf	( a_cpWhereStr, "CONVTYPE_C <> 0 AND CITYCODE = '%s' AND ADDRCODE LIKE '%s%s%s%s'", a_cpStr_5, a_cpStr_6, WCARD1, WCARD1, WCARD1 );
			sprintf	( a_cpWhereStr, "ADDRCODE2 <> '000000' AND CITYCODE = '%s' AND ADDRCODE LIKE '%s%s%s%s'", a_cpStr_5, a_cpStr_6, WCARD1, WCARD1, WCARD1 );
		}else {
			strncpy	( a_cpStr_6, e_cpAddrCode+5, 6 );
		//	sprintf	( a_cpWhereStr, "CONVTYPE_C <> 0 AND CITYCODE = '%s' AND ADDRCODE = '%s'", a_cpStr_5, a_cpStr_6 );
			sprintf	( a_cpWhereStr, "ADDRCODE2 <> '000000' AND CITYCODE = '%s' AND ADDRCODE = '%s'", a_cpStr_5, a_cpStr_6 );
		}
	}else {
	//	sprintf	( a_cpWhereStr, "CONVTYPE_C <> 0" );
		sprintf	( a_cpWhereStr, "ADDRCODE2 <> '000000'" );
	}

	// �Z���f�[�^�̒��ŁA�ϊ����[�h��1��2�̂��̂�����Ă���
	string	a_sWhere_Str= a_cpWhereStr; 
	IFeatureCursorPtr	a_ipAdminCursor;
	a_ipAdminCursor	= e_mCity_Admin_WS.Search_Where ( a_sWhere_Str, RET_FCURSOR|RET_COUNT );
	if ( a_ipAdminCursor == NULL ) {
		e_sPrnInfo.Prn_Info	( "�w��Z���R�[�h�̍s���E���擾�ł��܂���ł���\n", stderr );
		return	( false );
	}
	Admin_Check_Roops ( a_ipAdminCursor );
//	printf(" �Y���s���E�� = %d\n", e_mCity_Admin_WS.Get_Data_Count() );

	//�@���s���I��
	// �I�����Ԃ̎擾
	fprintf	( stdout,	"�f�[�^�`�F�b�N������ = %0.2f(s)�ł���\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );
	return	( true );
}
