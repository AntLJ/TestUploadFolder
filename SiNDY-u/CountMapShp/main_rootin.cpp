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
#include <atlstr.h>
#include <WinLib/VersionInfo.h>
#include <sindy/workspace.h>

#define	OPTION_MAX	13
#define	BMAX		512

bool	main_rootin::
arg_check	(	int		argc,		// ������
				char	**argv	)	// ������
{
	int		a_optcount[OPTION_MAX];	// �I�v�V�����̏d���̃J�E���g
	int		i;
	int		a_exeflag	= 0;
	int		digit		= 0;	// ���b�V���̌���

	//-----------------------------------------------------------------------
	// �c�[���o�[�W�����\�� 2008.11.12
	CVersion	  a_cvTool_Var;
	m_sVersion	  = a_cvTool_Var.GetFileVersion();
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
		return	( false );
	}

	char				*eMake_Type_Str		= NULL;

	// �I�v�V����������A���̂Ƃ���g�p���Ȃ�
	for(i = 1; i < argc;)
	{
		if(strcmp(argv[i], "-o") == 0) {
			if(argc < i+2) return( false );
			m_sOutInfoFile	= argv[i+1];
			a_exeflag		|= ON_OUTFILE;
			a_optcount[0]++;
			i+=2;
		} else if(strcmp(argv[i], "-l") == 0) {
			if(argc < i+2) return( false );
			m_sOutLogFile	= argv[i+1];
			a_exeflag		|= ON_LOGFILE;
			a_optcount[1]++;
			i+=2;
		}else if(strcmp(argv[i], "-m") == 0) {	// ���b�V�����X�g���w�肵���ꍇ�̓p�X�ƃ��b�V���Ńt�@�C����T��
			if(argc < i+2) return( false );
			m_sMeshList	= argv[i+1];
			a_exeflag		|= ON_MESHLIST;
			a_optcount[2]++;
			i+=2;
		}else if(strcmp(argv[i], "-M") == 0) {	// ���b�V�����X�g���w�肵���ꍇ�̓p�X�ƃ��b�V���Ńt�@�C����T��
			if(argc < i+2) return( false );
			m_lMeshCode	= atol(argv[i+1]);
			a_exeflag		|= ON_MONO_MESH;
			a_optcount[3]++;
			i+=2;
		}else if(strcmp(argv[i], "-s") == 0) {	// ���b�V�����X�g���w�肵���΂����ɁA�����ǉ��Ŏw�肷�邱�ƂŃ��b�V���̊K�w�������쐬���ăt�@�C����T��
			if(argc < i+2) return( false );
			m_iSubPathNum	= atoi(argv[i+1]);
			a_exeflag		|= ON_SUBDIR;
			a_optcount[4]++;
			i+=2;
		}else if(strcmp(argv[i], "-as") == 0) {	// �ǉ�������̎w��( xxxxxxxx<���ʎq>.xxx )�Ƃ���
			if(argc < i+2) return( false );
			m_sAddStr	= argv[i+1];
			a_exeflag		|= ON_ADDSTR;
			a_optcount[5]++;
			i+=2;
		}else if(strcmp(argv[i], "-ndpath") == 0) {	// ���L�\���f�[�^�̃p�X
			if(argc < i+2) return( false );
			m_sNoteDispFile	= argv[i+1];
			a_exeflag		|= ON_NOTEDISP;
			a_exeflag		|= ON_NOTECOUNT;
			a_optcount[6]++;
			i+=2;
		}else if(strcmp(argv[i], "-mdpath") == 0) {	// �L���\���f�[�^�̃p�X
			if(argc < i+2) return( false );
			m_sMarkDispFile	= argv[i+1];
			a_exeflag		|= ON_MARKDISP;
			a_exeflag		|= ON_NOTECOUNT;
			a_optcount[7]++;
			i+=2;
		}else if(strcmp(argv[i], "-mc") == 0) {		// ���b�V�����x��
			a_exeflag		|= ON_MESHCOUNT;
			a_optcount[8]++;
			i+=1;
		}else if(strcmp(argv[i], "-i") == 0) {
			if(argc < i+2) return( false );
			m_sSrcShpFile	= argv[i+1];
			a_exeflag		|= ON_INPUT_PATH;
			a_optcount[9]++;
			i+=2;
		} else if(strcmp(argv[i], "-allattr") == 0) {
			a_exeflag		|= ON_ALLATTR;
			a_optcount[10]++;
			i+=1;
		} else if(strcmp(argv[i], "-dtypeoff") == 0) {
			a_exeflag		|= ON_DATATYPEOFF;
			a_optcount[11]++;
			i+=1;
		}else if(strcmp(argv[i], "-v") == 0) {
			print_version();
			a_optcount[12]++;
			i+=1;
			return	( false );
		} else {
			fprintf	(stderr, "�I�v�V���� %s �͊Ԉ���Ă܂�!!\n\n", argv[i]);
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

	if( a_exeflag & ON_MONO_MESH && a_exeflag & ON_MESHLIST ) {
		fprintf	(stderr, "�I�v�V����'-m'��'-M'�͓����Ɏw��ł��܂���!!\n");
		return	( false );	
	}

	// ���L�\�����̓��̓p�X
	std::string	tmp = m_sAddStr;
	if( tmp == "_ANT" ) {
		if( !(a_exeflag & ON_NOTEDISP) ) {
			m_sNoteDispFile	= m_sSrcShpFile;
		}
		if( !(a_exeflag & ON_MARKDISP) ) {
			m_sMarkDispFile	= m_sSrcShpFile;
		}
		a_exeflag		|= ON_NOTECOUNT;
	}

	//-----------------------------------------------------------------------
	// DB�ڑ���
	if( getenv(DB_ANY) == NULL ) {
		fprintf	( m_fpOutLogFp, "���ϐ� DB_ANY ���ݒ肳��Ă��܂���\n" );
		return	( false );
	}else {
		m_DB_Init = getenv(DB_ANY);
	}

	string tmpstr = "FC_";
	if( a_exeflag & ON_NOTECOUNT ) {
		tmpstr += INI_ANNO_ATTR;
	}else {
		tmpstr += INI_BACK_ATTR;
	}

	if( getenv( tmpstr.c_str() ) == NULL ) {
		fprintf	( m_fpOutLogFp, "DB��̑����Ή��e�[�u���̊��ϐ����ݒ肳��Ă��܂���\n" );
		return	( false );
	}else {
		m_sAttrTableName	= getenv( tmpstr.c_str() );
	}

	if( !(a_exeflag & ON_INPUT_PATH) ) {
		fprintf	( m_fpOutLogFp, "�f�[�^���̓p�X��'-i'�Őݒ肵�Ă�������\n" );
		return	( false );
	}

	// �o�̓t�@�C�����I�[�v��
	if( a_exeflag & ON_OUTFILE ) {
		FILE*	a_fpOutFp	= fopen ( (char*)m_sOutInfoFile.c_str(), "w+" );
		if( a_fpOutFp == NULL ) {
			fprintf( m_fpOutLogFp, "�o�̓t�@�C�� <%s> ���I�[�v���ł��܂���\n", (char*)m_sOutInfoFile.c_str() );
			return	( false );
		}
		// ������stdout����o�͗p�ɐ؂�ւ���
		m_fpOutInfoFp	= a_fpOutFp;
	}

	// ���O�t�@�C�����I�[�v��	
	if( a_exeflag & ON_LOGFILE ) {
		FILE*	a_fpOutFp	= fopen ( (char*)m_sOutLogFile.c_str(), "w+" );
		if( a_fpOutFp == NULL ) {
			fprintf( m_fpOutLogFp, "���O�t�@�C�� <%s> ���I�[�v���ł��܂���\n", (char*)m_sOutLogFile.c_str() );
			return	( false );
		}
		// ������stdout����o�͗p�ɐ؂�ւ���
		m_fpOutLogFp	= a_fpOutFp;
	}

	fprintf	( m_fpOutLogFp,	"#SINDYSTDLOG\n" );
	fprintf	( m_fpOutLogFp,	"#�c�[���o�[�W����     : <%s>\n", m_sVersion.c_str() );
	fprintf	( m_fpOutLogFp,	"#DB�ڑ�               : <%s>\n", m_DB_Init.c_str() );
	fprintf	( m_fpOutLogFp,	"#�����e�[�u��         : <%s>\n", m_sAttrTableName.c_str() );
	fprintf	( m_fpOutLogFp,	"#���b�V�����X�g       : <%s>\n", m_sMeshList.c_str() );

	m_iExe_Flag	= a_exeflag;

	return		( true );

}

/**
	�c�[���̎g�p�@�̕\��
*/
void		main_rootin::
print_usage	(char **argv)
{
	fprintf(	stdout,
				"=== �y�V�F�C�v�t�@�C�������l�J�E���^�z ===\n"
				"<usage>\n"
				"%s [option..]\n"
				"\t<���ϐ�>\n"
				"\t      DB_ANY:              �����e�[�u��DB�ڑ����\n"
				"\t      FC_BACKGROUND_CLASS: �w�i�����e�[�u����(ex. REFERENCE.BACKGROUND_CLASS)\n"
				"\t      FC_ANNOTATION_CLASS: ���L�����e�[�u����(ex. REFERENCE.ANNOTATION_CLASS)\n"
				"\t<option>\n"
				"\t      -i                 : �Q�ƌ��V�F�C�v�t�@�C���p�X(ex. \"\\\\Hd-htgl156\\share\\���ؗp�f�[�^\\normal\\Citymap\n"
				"\t      -o                 : ���o�̓t�@�C���i�w�肪�����ꍇ�� stdout �֏o�́j\n"
				"\t      -l                 : ���O�o�̓t�@�C���i�w�肪�����ꍇ�� stderr �֏o�́j\n"
				"\t      -m                 : ���b�V�����X�g���w�肵���ꍇ�̓p�X�ƃ��b�V���Ńt�@�C����T��\n"
				"\t      -s                 : ���b�V���K�w�w��\n"
				"\t      -as                : �ǉ�������̎w��( �t�@�C������ xxxxxxxx<������>.xxx )�Ƃ���\n"
				"\t      -mc                : ���b�V���R�[�h�P�ʃJ�E���g\n"
				"\t      -ndpath            : ���L�\���f�[�^�̃p�X\n"
				"\t      -mdpath            : �L���\���f�[�^�̃p�X\n"
				"\t      -allattr           : �S�����\�����[�h\n"
				"\t      -dtypeoff          : �f�[�^�^�C�v�\��OFF\n"
				"\t      -v                 : �o�[�W�������\n"
				"\t                         �@��'-ndpath'��'-mdpath'���w�肳��Ȃ��ꍇ�́A'-i'�w��̏ꏊ���Q�Ƃ���\n"
			//	"\t<�o�[�W�������>\n"
			//	"\t      2008.07.04         1.0.0.1          : ��r��ɂ��������f�[�^��\������悤�ɕύX\n"
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
*/
void	main_rootin::
print_version ( void )
{
	fprintf(	stdout,
				"=== �y�V�F�C�v�t�@�C�������l�J�E���^�z ===\n"
				"\t<�o�[�W�������>\n"
				"\t2008/07/11    1.0.0.1        �E�V�K�쐬\n"
				"\t2010/08/25    1.1.0.2        �E[bug 8123]�ւ̑Ή�(���L�^�ʒu���̒ǉ�)\n"
				"\t                             �E�S�����\�����[�h('-allattr')�̒ǉ�\n"
				"\t2013/10/01    1.2.0.3        �Epostgres�Ή�\n"
				"\t                             �E[bug 8722][u][�v�]]�o�̓t�H�[�}�b�g��񋟎����ɍ��킹�ė~���� �ɑΉ�\n"
				);
}

/** 2006.01.09
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

	c_ipWorkspace =  sindy::create_workspace(LPCTSTR(m_DB_Init.c_str()));
	if(!c_ipWorkspace ){
		return false;
	}
	return ( true );
}

/**
	���b�V���P�ʃt�@�C�����̍쐬
*/
string	main_rootin::
make_mesh_fname ( int	c_iMeshCode,	///< ���b�V���R�[�h
				  char*	c_cpSrcShpPath,	///< �\�[�X�V�F�C�v�p�X
				  char*	c_cpAddStr )
{
	string	a_sRetStr	= "";
	string	a_sSubDir	= "";

	char	a_sTmp[4096];

	int	a_iDigit	= 0;
	a_iDigit	= m_cCrd.GetMeshDigit ( c_iMeshCode );

	if( m_iExe_Flag & ON_MONO_MESH || m_iExe_Flag & ON_MESHLIST ) {
		if( m_iExe_Flag & ON_SUBDIR ) {	// �K�w
			char	a_sSubStr[256];
			memset	( a_sSubStr, '\0', 256 );
			switch ( m_iSubPathNum )
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
		}
	}

	if( a_iDigit == 2 ) {
		sprintf	( a_sTmp, "%s%s\\%.2d%s", c_cpSrcShpPath, (char*)a_sSubDir.c_str(), c_iMeshCode, c_cpAddStr );
	}else {
		sprintf	( a_sTmp, "%s%s\\%d%s", c_cpSrcShpPath, (char*)a_sSubDir.c_str(), c_iMeshCode, c_cpAddStr );
	}

	a_sRetStr	= a_sTmp;

	return ( a_sRetStr );
}

/**
	�t�@�C���̃t���p�X������A�V�F�C�v�t�@�C�����𒊏o����֐�
	Function to extract the shape file name from the fullpath of the file
	@return	�Ȃ�
*/
bool main_rootin::
GetFeatureClass	(	const char*	sourceFile,		///< �\�[�X�t�@�C�����i�p�X���܂ށj
					string&		className	)	///< �t�B�[�`���N���X��
{
	char*	tempName	= NULL;
	char*	temp		= NULL;

	tempName = (char*)strrchr(sourceFile, '\\')+1;
	if(tempName == NULL) {
		temp		= new char[strlen(sourceFile)+1];
		if(temp == NULL) {
			return	( false );
		}
		strcpy		( temp, sourceFile );

		className	= temp;	// '\'�}�[�N���݂���Ȃ��̂Ńp�X�����܂܂Ȃ�
	}else {
		temp		= new char[strlen(tempName)+1];
		if( temp == NULL ) {
			return	( false );
		}
		strcpy		( temp, tempName );
		className	= temp;
	}
	delete [] temp;
	return ( false );
}

/**
	�t���p�X������A�t�@�C�����������āA�f�B���N�g������Ԃ��֐�
	// Function to remove the file name from the fullpath and return the directory
	@return	�Ȃ�
*/
bool	main_rootin::
GetFilePath	(	const char*		sourceFile,		///< �\�[�X�t�@�C����
				string&			filePath	)	///< �p�X��
{
	char*	temp	= NULL;

	temp	= new char[strlen(sourceFile)+1];
	if( temp == NULL ) {
		filePath	= "";
		return		( false );
	}

	temp		= strcpy(temp, sourceFile);

	char*		pathEnd;

	pathEnd		= strrchr(temp, '\\')+1;
	*pathEnd	= '\0';		// ��������؂�
	
	filePath	= temp;

	delete [] temp;

	return	( true );
}

/**
	����ǂݍ���
*/
bool	main_rootin::
read_info (	int						c_iMeshCode,
			char*					c_cpSrcShpPath,
			char*					c_cpSrcFClass,
			v_Feature_All_Attrs*	c_vpFAttr_List )
{
	HRESULT	hr;

	// Generate an envelope from the extents
	IWorkspaceFactoryPtr	a_ipWSFactory(CLSID_ShapefileWorkspaceFactory);
	if( !a_ipWSFactory )
		return false;

	// �f�B���N�g�������烏�[�N�X�y�[�X���I�[�v������
	// Open a workspace using the directory of the file
	IWorkspacePtr			a_ipSWSpace;
	hr	= a_ipWSFactory->OpenFromFile(CComBSTR(c_cpSrcShpPath), 0, &a_ipSWSpace);
	if (FAILED(hr)) {
		fprintf( m_fpOutLogFp, "<%.2d>\t���[�N�X�y�[�X�̃I�[�v���Ɏ��s<%s>\n", c_iMeshCode, (char*)m_sSrcShpPath.c_str() );
		return ( false );
	}

	IFeatureWorkspacePtr	a_ipFWS_Src(a_ipSWSpace);	// �t�B�[�`�����[�N�X�y�[�X���I�[�v��
	// �\�[�X�V�F�C�v�t�@�C��������t�B�[�`���N���X���I�[�v������
	// Open the feature class using the shape file name
	IFeatureClassPtr		a_ipSFClass;

	int retry_num = 0;
	for( retry_num = 0; retry_num < 5; retry_num++ )
	{
		hr = a_ipFWS_Src->OpenFeatureClass ( CComBSTR(c_cpSrcFClass), &a_ipSFClass );
		if (FAILED(hr)) {
			if( retry_num == 4 ) {
				fprintf( m_fpOutLogFp, "<%.2d>\t�t�B�[�`���N���X�̃I�[�v���Ɏ��s<%s> %d\n", c_iMeshCode, (char*)m_sSrcFClass.c_str(), hr );
				return ( false );
			}else {
				continue;
			}
		}else {	// ���������甲����
			break;
		}
	}
	
	char	a_tmpstr[1024];				//�@�����͂Ȃ�Ƃ��������C������
	sprintf	( a_tmpstr, "FID >= 0" );	// ���b�V���R�[�h�𕶎����
	_bstr_t	a_Where_Str ( a_tmpstr );

	IQueryFilterPtr		a_ipQ_Filter	( CLSID_QueryFilter );		// ��Ԍ����t�B���^	
	hr	= a_ipQ_Filter->put_WhereClause	( a_Where_Str );
	if ( FAILED ( hr ) )	{
		fprintf( m_fpOutLogFp, "<%.2d>\t�����pWHERE��̐ݒ肪�ł��Ȃ�\n", c_iMeshCode );
		return	( false );
	}

	// ��Ԍ����Ńt�B�[�`���J�[�\��
	IFeatureCursorPtr	ipFeatureCursor;
	hr = a_ipSFClass->Search( a_ipQ_Filter, VARIANT_FALSE, &ipFeatureCursor);
	if (FAILED(hr))	{
		fprintf( m_fpOutLogFp, "<%.2d>\t�����Ɏ��s\n", c_iMeshCode );
		return ( false );
	}

	// �f�[�^�����擾
	long	a_lDataNum	= 0;
	a_ipSFClass->FeatureCount ( a_ipQ_Filter, &a_lDataNum );

	// ���b�V�������̎擾
	long	a_lDigit	= 0;
	a_lDigit	= m_cCrd.GetMeshDigit ( c_iMeshCode );
	
	int	l = 0;
	int	i = 0;
	v_Feature_All_Attrs		a_vFAttr_List;
//	char	a_sMsgBuf[4096];
	for(i=0;;i++)
	{ 
		IFeaturePtr				ipFeature;
		if( ipFeatureCursor->NextFeature(&ipFeature) != S_OK) {
			break;
		}
		Feature_All_Attrs	a_fCurAttrs;

		a_fCurAttrs.Init ( ipFeature );	// �����Ă��鑮���݂�Ȏ��o��

		c_vpFAttr_List->push_back ( a_fCurAttrs );

	}

//	if( m_fpOutLogFp != stderr ) {
//		fprintf( m_fpOutLogFp,"%s �I��\n", c_cpSrcFClass);
//	}
	fflush ( m_fpOutLogFp );
	return	( true );
}

/**
	�f�[�^���`�F�b�N�p�̍\���ɂ���
	@return	bool
*/
bool	main_rootin::
mk_data_for_chk	( v_Feature_All_Attrs&	c_vpFSrc_Data,
				  mm_note_layer*		c_mpNote_Data )
{
	iv_Feature_All_Attrs	a_iIndx;
	for( a_iIndx = c_vpFSrc_Data.begin(); a_iIndx != c_vpFSrc_Data.end(); a_iIndx++ )
	{
		Feature_All_Attrs	a_fCurAttrs;
		string				a_sPrnMsg	= "";
		long				a_lAttrNum	= 0;

		a_fCurAttrs		= *a_iIndx;

		int j = 0;

		Feature_Attr	a_fOBJECTID, a_fANNOCLS_C, a_fNAMESTR1, a_fNAMESTR2, a_fNAMESTR3;

		a_fOBJECTID		= a_fCurAttrs.Get_Attr ( NF_OBJECTID );
		a_fANNOCLS_C	= a_fCurAttrs.Get_Attr ( NF_ANNOCLS_C );
		a_fNAMESTR1		= a_fCurAttrs.Get_Attr ( NF_NAMESTR1 );
		a_fNAMESTR2		= a_fCurAttrs.Get_Attr ( NF_NAMESTR2 );
		if( m_iDigit != 8 ) {
			a_fNAMESTR3		= a_fCurAttrs.Get_Attr ( NF_NAMESTR3 );
		}
		note_layer	a_nPutData;

		_bstr_t		a_bNS1;//	( a_fNAMESTR1.e_ccvAttr.bstrVal, false );
		_bstr_t		a_bNS2;//	( a_fNAMESTR2.e_ccvAttr.bstrVal, false );
		_bstr_t		a_bNS3;//	( a_fNAMESTR3.e_ccvAttr.bstrVal, false );

		a_bNS1		= a_fNAMESTR1.e_ccvAttr.bstrVal;
		a_bNS2		= a_fNAMESTR2.e_ccvAttr.bstrVal;
		if( m_iDigit != 8 ) {
			a_bNS3		= a_fNAMESTR3.e_ccvAttr.bstrVal;
		}

		a_nPutData.e_iFID		= a_fCurAttrs.Get_Obj_ID();		// OBJECTID�Ƃ͈Ⴄ���Ƃɒ��ӁI
		a_nPutData.e_iOBJECTID	= a_fOBJECTID.e_ccvAttr.intVal;
		a_nPutData.e_iANNOCLS_C	= a_fANNOCLS_C.e_ccvAttr.intVal;
		a_nPutData.e_sNAMESTR1	= (char *)a_bNS1;
		a_nPutData.e_sNAMESTR2	= (char *)a_bNS2;
		if( m_iDigit != 8 ) {
			a_nPutData.e_sNAMESTR3	= (char *)a_bNS3;
		}else {
			a_nPutData.e_sNAMESTR3	= SHP_NULL_STR;	// �����Ń_�~�[�����ċ󂾂Ƃ��Ă���		
		}
		c_mpNote_Data->insert ( mm_note_layer::value_type (a_nPutData.e_iOBJECTID, a_nPutData) );
	}
	return	( true );
}

/**
	�f�[�^���`�F�b�N�p�̍\���ɂ���
	@return	bool
*/
bool	main_rootin::
mk_data_for_chk	( v_Feature_All_Attrs&	c_vpFSrc_Data,
				  mm_note_disp_info*	c_mpNote_Disp_Data )
{
	iv_Feature_All_Attrs	a_iIndx;
	for( a_iIndx = c_vpFSrc_Data.begin(); a_iIndx != c_vpFSrc_Data.end(); a_iIndx++ )
	{
		Feature_All_Attrs	a_fCurAttrs;
		string				a_sPrnMsg	= "";
		long				a_lAttrNum	= 0;

		a_fCurAttrs		= *a_iIndx;

		int j = 0;

		Feature_Attr	a_fANNOID, a_fSCALE, a_fNAMEID, a_fLINEANNO_F, a_fFONTSIZE_C;

		a_fANNOID		= a_fCurAttrs.Get_Attr ( NF_ANNOID );
		a_fSCALE		= a_fCurAttrs.Get_Attr ( NF_SCALE );
		a_fNAMEID		= a_fCurAttrs.Get_Attr ( NF_NAMEID );
		a_fLINEANNO_F	= a_fCurAttrs.Get_Attr ( NF_LINEANNO_F );	// 2006.04.03
		a_fFONTSIZE_C	= a_fCurAttrs.Get_Attr ( NF_FONTSIZE_C );

		note_disp_info	a_nPutData;

		a_nPutData.e_iFID		= a_fCurAttrs.Get_Obj_ID ();
		a_nPutData.e_iANNOID	= a_fANNOID.e_ccvAttr.intVal;
		a_nPutData.e_iSCALE		= a_fSCALE.e_ccvAttr.intVal;
		a_nPutData.e_iNAMEID	= a_fNAMEID.e_ccvAttr.intVal;
		a_nPutData.e_iLINEANNO_F= a_fLINEANNO_F.e_ccvAttr.intVal;
		a_nPutData.e_iFONTSIZE_C= a_fFONTSIZE_C.e_ccvAttr.intVal;

		c_mpNote_Disp_Data->insert ( mm_note_disp_info::value_type (a_nPutData.e_iANNOID, a_nPutData) );
	}
	return	( true );
}

/**
	�f�[�^���`�F�b�N�p�̍\���ɂ���
	@return	bool
*/
bool	main_rootin::
mk_data_for_chk	( v_Feature_All_Attrs&	c_vpFSrc_Data,
				  mm_mark_disp_info*	c_mpMark_Disp_Data )
{
	iv_Feature_All_Attrs	a_iIndx;
	for( a_iIndx = c_vpFSrc_Data.begin(); a_iIndx != c_vpFSrc_Data.end(); a_iIndx++ )
	{
		Feature_All_Attrs	a_fCurAttrs;
		string				a_sPrnMsg	= "";
		long				a_lAttrNum	= 0;

		a_fCurAttrs		= *a_iIndx;

		int j = 0;

		Feature_Attr	a_fANNOID, a_fSCALE, a_fPOINT_F;

		a_fANNOID		= a_fCurAttrs.Get_Attr ( NF_ANNOID );
		a_fSCALE		= a_fCurAttrs.Get_Attr ( NF_SCALE );
		a_fPOINT_F		= a_fCurAttrs.Get_Attr ( NF_POINT_F );

		mark_disp_info	a_nPutData;

		a_nPutData.e_iFID		= a_fCurAttrs.Get_Obj_ID ();
		a_nPutData.e_iANNOID	= a_fANNOID.e_ccvAttr.intVal;
		a_nPutData.e_iSCALE		= a_fSCALE.e_ccvAttr.intVal;
		a_nPutData.e_iPOINT_F	= a_fPOINT_F.e_ccvAttr.intVal;

		c_mpMark_Disp_Data->insert ( mm_mark_disp_info::value_type (a_nPutData.e_iANNOID, a_nPutData) );
	}
	return	( true );
}

/**
	���L�ȊO�̃f�[�^�̃J�E���g���s��
*/
bool	main_rootin::
count_info (	int		c_iMeshCode,
				char*	c_cpSrcShpPath,
				char*	c_cpSrcFClass )
{
	HRESULT	hr;

	// Generate an envelope from the extents
	IWorkspaceFactoryPtr	a_ipWSFactory(CLSID_ShapefileWorkspaceFactory);
	if( !a_ipWSFactory )
		return false;

	// �f�B���N�g�������烏�[�N�X�y�[�X���I�[�v������
	// Open a workspace using the directory of the file
	IWorkspacePtr			a_ipSWSpace;
	hr	= a_ipWSFactory->OpenFromFile(CComBSTR(c_cpSrcShpPath), 0, &a_ipSWSpace);
	if (FAILED(hr)) {
		fprintf( m_fpOutLogFp, "<%.2d>\t���[�N�X�y�[�X�̃I�[�v���Ɏ��s<%s>\n", c_iMeshCode, c_cpSrcShpPath );
		return ( false );
	}

	IFeatureWorkspacePtr	a_ipFWS_Src(a_ipSWSpace);	// �t�B�[�`�����[�N�X�y�[�X���I�[�v��
	// �\�[�X�V�F�C�v�t�@�C��������t�B�[�`���N���X���I�[�v������
	// Open the feature class using the shape file name
	IFeatureClassPtr		a_ipSFClass;

	hr = a_ipFWS_Src->OpenFeatureClass ( CComBSTR(c_cpSrcFClass), &a_ipSFClass );
	if (FAILED(hr)) {
		fprintf( m_fpOutLogFp, "<%.2d>\t�t�B�[�`���N���X�̃I�[�v���Ɏ��s<%s> %d\n", c_iMeshCode, c_cpSrcFClass, hr );
		return ( false );
	}
	
	char	a_tmpstr[1024];				//�@�����͂Ȃ�Ƃ��������C������
	sprintf	( a_tmpstr, "FID >= 0" );	// ���b�V���R�[�h�𕶎����
	_bstr_t	a_Where_Str ( a_tmpstr );

	IQueryFilterPtr		a_ipQ_Filter	( CLSID_QueryFilter );		// ��Ԍ����t�B���^	
	hr	= a_ipQ_Filter->put_WhereClause	( a_Where_Str );
	if ( FAILED ( hr ) )	{
		fprintf( m_fpOutLogFp, "<%.2d>\t�����pWHERE��̐ݒ肪�ł��Ȃ�", c_iMeshCode );
		return	( false );
	}

	// ��Ԍ����Ńt�B�[�`���J�[�\��
	IFeatureCursorPtr	ipFeatureCursor;
	hr = a_ipSFClass->Search( a_ipQ_Filter, VARIANT_FALSE, &ipFeatureCursor);
	if (FAILED(hr))	{
		fprintf( m_fpOutLogFp, "<%.2d>\t�����Ɏ��s\n", c_iMeshCode );
		return ( false );
	}

	// �f�[�^�����擾
	long	a_lDataNum	= 0;
	a_ipSFClass->FeatureCount ( a_ipQ_Filter, &a_lDataNum );

	// ���b�V�������̎擾
	long	a_lDigit	= 0;
	a_lDigit	= m_cCrd.GetMeshDigit ( c_iMeshCode );
	
//	fprintf	( stdout, "<%.2d> -----[�`�F�b�N�J�n]----------\n", c_iMeshCode );
//	fprintf ( stdout, "<%.2d>\t%s %d���`�F�b�N��...\n", c_iMeshCode, c_cpSrcFClass, a_lDataNum );

	int	l = 0;
	int	i = 0;
	v_Feature_All_Attrs		a_vFAttr_List;
	char	a_sMsgBuf[4096];

	// �f�[�^�^�C�v���擾����
	string	a_sSiNDYFld;
	int		a_iDataType = 0;
	int		a_iShpType	= 0;
	if( !GetDBAttrFld ( &a_sSiNDYFld, &a_iDataType, &a_iShpType ) ) {
		return	( false );
	}
	
	for(i=0;;i++)
	{ 
		IFeaturePtr				ipFeature;
		if( ipFeatureCursor->NextFeature(&ipFeature) != S_OK) {
			break;
		}
		Feature_All_Attrs	a_fCurAttr;

		a_fCurAttr.Init ( ipFeature );	// �����Ă��鑮���݂�Ȏ��o��

		long		a_lOid		= 0;
		long		a_lSrc_Oid	= 0;	// DB��ł�OID
		string		a_sPrnMsg	= "";
		long		a_lAttrNum	= 0;

		a_lOid		= a_fCurAttr.Get_Obj_ID ();
		a_lAttrNum	= a_fCurAttr.Get_Num();

		int j = 0;

		sprintf( a_sMsgBuf, "%.2d", c_iMeshCode );
		a_sPrnMsg += a_sMsgBuf;

		int	a_iScaleChk_F	= 0;	// �X�P�[���Ó����`�F�b�N�����邩�̃t���O
		int	a_iScaleAttr[4]	= {0,0,0,0};

		data_info	a_dData, a_dSData[4];

		if( m_iExe_Flag & ON_MESHCOUNT ) {
			a_dData.e_iMeshCode		= c_iMeshCode;
		}

		Feature_Attr		a_fAttr, a_fUnder;
		Feature_Attr		a_fSAttr[4];	//�X�P�[���p
		switch ( a_iDataType )
		{
		case	BGP:
			switch (m_iInput_Lv )
			{
			case	SiINClassTable::top:
				a_fAttr	= a_fCurAttr.Get_Attr ( "BGCLS_C" );
				a_dData.e_iSindyAttr	= a_fAttr.e_ccvAttr.intVal;
				for ( j = 0; j < 4; j++ ) a_dData.e_iSCount[j]++;
				Put_Info	( a_dData );
				break;
			case	SiINClassTable::mid:
			case	SiINClassTable::base:
				a_fSAttr[0]	= a_fCurAttr.Get_Attr ( "SC1CLASS_C" );
				a_fSAttr[1]	= a_fCurAttr.Get_Attr ( "SC2CLASS_C" );
				a_fSAttr[2]	= a_fCurAttr.Get_Attr ( "SC3CLASS_C" );
				a_fSAttr[3]	= a_fCurAttr.Get_Attr ( "SC4CLASS_C" );
				for ( j = 0; j < 4; j++ )
				{
					if( m_iExe_Flag & ON_MESHCOUNT ) {
						a_dSData[j].e_iMeshCode	= c_iMeshCode;
					}
					a_dSData[j].e_iSindyAttr	= a_fSAttr[j].e_ccvAttr.intVal;
					a_dSData[j].e_iSCount[j]++;
					Put_Info	( a_dSData[j] );
				}
				break;
			case	SiINClassTable::city:
				a_fAttr	= a_fCurAttr.Get_Attr ( "BGCLS_C" );
				a_dData.e_iSindyAttr	= a_fAttr.e_ccvAttr.intVal;	
				a_dData.e_iSCount[3]	= 1;
				Put_Info	( a_dData );
				break;
			}
			break;
		case	BGL:
			switch (m_iInput_Lv )
			{
			case	SiINClassTable::top:
				a_fAttr	= a_fCurAttr.Get_Attr ( "BGCLS_C" );
				a_dData.e_iSindyAttr	= a_fAttr.e_ccvAttr.intVal;
				for ( j = 0; j < 4; j++ ) a_dData.e_iSCount[j] = 1;
				Put_Info	( a_dData );
				break;
			case	SiINClassTable::mid:
			case	SiINClassTable::base:
				a_fAttr		= a_fCurAttr.Get_Attr ( "BGCLS_C" );
				a_fUnder	= a_fCurAttr.Get_Attr ( "UNDER_F" );
				a_fSAttr[0]	= a_fCurAttr.Get_Attr ( "SC1DISP_F" );
				a_fSAttr[1]	= a_fCurAttr.Get_Attr ( "SC2DISP_F" );
				a_fSAttr[2]	= a_fCurAttr.Get_Attr ( "SC3DISP_F" );
				a_fSAttr[3]	= a_fCurAttr.Get_Attr ( "SC4DISP_F" );
				a_dData.e_iSindyAttr	= a_fAttr.e_ccvAttr.intVal;
				a_dData.e_iUnderF		= a_fUnder.e_ccvAttr.intVal;
				for ( j = 0; j < 4; j++ )
				{
					if( a_fSAttr[j].e_ccvAttr.intVal == 1 ) a_dData.e_iSCount[j] = 1; // �t���O�Ȃ̂�a_dData�Ɋi�[
				}
				Put_Info	( a_dData );
				break;
			case	SiINClassTable::city:
				a_fAttr		= a_fCurAttr.Get_Attr ( "BGCLS_C" );
				a_dData.e_iSindyAttr	= a_fAttr.e_ccvAttr.intVal;	
				a_dData.e_iSCount[3]	= 1;
				Put_Info	( a_dData );
				break;
			}
			break;
		case	BLD:
			a_fAttr		= a_fCurAttr.Get_Attr ( "BLDCLS_C" );
			a_dData.e_iSindyAttr	= a_fAttr.e_ccvAttr.intVal;			
			a_dData.e_iSCount[3]	= 1;
			Put_Info	( a_dData );
			break;
		case	RAL:
			switch (m_iInput_Lv )
			{
			case	SiINClassTable::base:
				a_fAttr		= a_fCurAttr.Get_Attr ( "RALCLS_C" );
				a_fUnder	= a_fCurAttr.Get_Attr ( "UNDER_F" );
				a_fSAttr[0]	= a_fCurAttr.Get_Attr ( "SC1DISP_F" );
				a_fSAttr[1]	= a_fCurAttr.Get_Attr ( "SC2DISP_F" );
				a_fSAttr[2]	= a_fCurAttr.Get_Attr ( "SC3DISP_F" );
				a_fSAttr[3]	= a_fCurAttr.Get_Attr ( "SC4DISP_F" );
				a_dData.e_iSindyAttr	= a_fAttr.e_ccvAttr.intVal;
				a_dData.e_iUnderF		= a_fUnder.e_ccvAttr.intVal;
				for ( j = 0; j < 4; j++ )
				{
					if( a_fSAttr[j].e_ccvAttr.intVal == 1 ) a_dData.e_iSCount[j] = 1;
				}
				Put_Info	( a_dData );
				break;
			case	SiINClassTable::city:
				a_fAttr		= a_fCurAttr.Get_Attr ( "RALCLS_C" );
				a_dData.e_iSindyAttr	= a_fAttr.e_ccvAttr.intVal;
				a_dData.e_iSCount[3]	= 1;	
				Put_Info	( a_dData );
				break;
			}
			break;
		case	USP:
			a_fAttr		= a_fCurAttr.Get_Attr ( "STACLS_C" );
			a_dData.e_iSindyAttr	= a_fAttr.e_ccvAttr.intVal;			
			a_dData.e_iUnderF		= 1; // �����I�ɒn��
			a_dData.e_iSCount[3]	= 1;
			Put_Info	( a_dData );
			break;
		case	STA:
			a_fAttr		= a_fCurAttr.Get_Attr ( "STACLS_C" );
			a_fUnder	= a_fCurAttr.Get_Attr ( "UNDER_F" );
			a_dData.e_iSindyAttr	= a_fAttr.e_ccvAttr.intVal;			
			a_dData.e_iUnderF		= a_fUnder.e_ccvAttr.intVal;
			a_dData.e_iSCount[3]	= 1;
			Put_Info	( a_dData );
			break;
		case	CNT:
			a_fAttr		= a_fCurAttr.Get_Attr ( "CNTCLS_C" );
			a_dData.e_iSindyAttr	= a_fAttr.e_ccvAttr.intVal;			
			a_dData.e_iSCount[3]	= 1;
			Put_Info	( a_dData );
			break;
		case	ADM:	// 2007.07.26
			a_dData.e_iSindyAttr	= 0;	// ADM�͎�ʃR�[�h�������Ȃ��̂�'0'�ɂ���
			a_dData.e_iSCount[3]	= 1;
			Put_Info	( a_dData );
			break;
		case	DCL:
			a_dData.e_iSindyAttr	= 0;	// ADM�͎�ʃR�[�h�������Ȃ��̂�'0'�ɂ���
			a_dData.e_iSCount[3]	= 1;
			Put_Info	( a_dData );
			break;
		}
	}

	fflush ( m_fpOutLogFp );
	fflush ( m_fpOutInfoFp );
	return	( true );
}

/**
	���L�̑����J�E���g���s��
*/
bool	main_rootin::
count_note_info (	int						c_iMeshCode,
					v_Feature_All_Attrs&	c_vpFSrc_Data,
					v_Feature_All_Attrs&	c_vpFNDF_Data,
					v_Feature_All_Attrs&	c_vpFMDF_Data	)
{
//	HRESULT	hr;

	bool bRet = true;

	mm_note_layer		a_mpNote_Data;		// ���L�f�[�^
	mm_note_disp_info	a_mpNote_Disp_Data;	// ���L�\���f�[�^
	mm_mark_disp_info	a_mpMark_Disp_Data;	// �L���\���f�[�^

	long	a_lDigit	= 0;
	a_lDigit	= m_cCrd.GetMeshDigit ( c_iMeshCode );
	m_iDigit	= a_lDigit;

	// �`�F�b�N�p�̃f�[�^�ɂ���
	mk_data_for_chk ( c_vpFSrc_Data, &a_mpNote_Data );
	mk_data_for_chk ( c_vpFNDF_Data, &a_mpNote_Disp_Data );
	mk_data_for_chk ( c_vpFMDF_Data, &a_mpMark_Disp_Data );

	imm_note_layer		a_iN_Indx;
	for ( a_iN_Indx = a_mpNote_Data.begin(); a_iN_Indx != a_mpNote_Data.end(); a_iN_Indx++ )
	{
		note_layer		a_nCur	= (*a_iN_Indx).second;

		int					a_iNOD_Count	= 0;	// �q�b�g�������L�\���f�[�^�̌�
		int					a_iMKD_Count	= 0;	// �q�b�g�����L���\���f�[�^�̌�

		pair< imm_note_disp_info, imm_note_disp_info >	a_piNOD_Indx_pair;
		pair< imm_mark_disp_info, imm_mark_disp_info >	a_piMKD_Indx_pair;

		// ���L�\���f�[�^����Y���f�[�^��T��
		a_iNOD_Count		= a_mpNote_Disp_Data.count ( a_nCur.e_iOBJECTID );	// OBJECTID���L�[�ɃT�[�`
		a_piNOD_Indx_pair	= a_mpNote_Disp_Data.equal_range ( a_nCur.e_iOBJECTID );
		// �L���\���f�[�^����Y���f�[�^��T��
		a_iMKD_Count		= a_mpMark_Disp_Data.count ( a_nCur.e_iOBJECTID );	// OBJECTID���L�[�ɃT�[�`
		a_piMKD_Indx_pair	= a_mpMark_Disp_Data.equal_range ( a_nCur.e_iOBJECTID );

		// �����ɑΉ����Ȃ���΃G���[
		if( a_iNOD_Count == 0 && a_iMKD_Count == 0 ) {
			fprintf( m_fpOutLogFp, "0\t%.2d\t%.2d_ANT\t%d\t%d\t%s\n",
					c_iMeshCode, c_iMeshCode, a_nCur.e_iFID, a_nCur.e_iOBJECTID, "�\���f�[�^��������Ȃ�");
			bRet = false;
			continue;
		}

	//	int	a_iNote_State[10] = {0,0,0,0,0,0,0,0,0,0}; // ���ꂼ��s�s�n�},S1,S2,S3,S4�ł̏�ԁi�T�܂ł����g��Ȃ����A�ꉞ�`�F�b�N�̂��߂P�O�Ƃ��Ă����j

		imm_note_disp_info	a_iNOD_Indx;
		imm_mark_disp_info	a_iMKD_Indx;

		data_info				a_dData;

		if( m_iExe_Flag & ON_MESHCOUNT ) {
			a_dData.e_iMeshCode		= c_iMeshCode;
		}
		a_dData.e_iSindyAttr	= a_nCur.e_iANNOCLS_C;

		// �^�ʒu�J�E���g���v���X 2010.08.17
		a_dData.e_iTrueCount++;

		// �^�ʒu�̏�񂩂�A�Y�����钍�L������\�����A���L�L���\�����̏������o��
		//-----------------------------------------------------------------------------------------
		// ������\��
		//-----------------------------------------------------------------------------------------
		for( a_iNOD_Indx = a_piNOD_Indx_pair.first; a_iNOD_Indx != a_piNOD_Indx_pair.second; a_iNOD_Indx++ )
		{
			note_disp_info	a_nC_NOD	= (*a_iNOD_Indx).second;

			// �s�s�n�}�̏ꍇ�A�f�[�^�̓X�P�[��4�̂Ƃ���ɏ�������
			if( a_nC_NOD.e_iSCALE == 0 ) {
				a_nC_NOD.e_iSCALE	= 4;
			}

			// ��Ԃ̊i�[
		//	a_iNote_State[a_nC_NOD.e_iSCALE]	|= EX_STR;	// �����񂠂�
			a_dData.e_iSCount[a_nC_NOD.e_iSCALE-1]++;	
		}

		//-----------------------------------------------------------------------------------------
		// �L���\��
		//-----------------------------------------------------------------------------------------
		for( a_iMKD_Indx = a_piMKD_Indx_pair.first; a_iMKD_Indx != a_piMKD_Indx_pair.second; a_iMKD_Indx++ )
		{
			mark_disp_info	a_nC_MKD	= (*a_iMKD_Indx).second;

			// �s�s�n�}�̏ꍇ�A�f�[�^�̓X�P�[��4�̂Ƃ���ɏ�������
			if( a_nC_MKD.e_iSCALE == 0 ) {
				a_nC_MKD.e_iSCALE	= 4;
			}

			// ��Ԃ̊i�[
		//	a_iNote_State[a_nC_MKD.e_iSCALE]	|= EX_MARK;	// �L������
			a_dData.e_iSCount_M[a_nC_MKD.e_iSCALE-1]++;	
		}
		Put_Info	( a_dData );
	}

	fflush ( m_fpOutLogFp );
	fflush ( m_fpOutInfoFp );
	return	bRet;
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
		if( c_sFieldName == "SC1CLASS_C" ) {
			*c_ipScale	= 1;
			return	( IS_SCCLS );
		}else
		if( c_sFieldName == "SC2CLASS_C" ) {
			*c_ipScale	= 2;
			return	( IS_SCCLS );
		}else
		if( c_sFieldName == "SC3CLASS_C" ) {
			*c_ipScale	= 3;
			return	( IS_SCCLS );
		}else
		if( c_sFieldName == "SC4CLASS_C" ) {
			*c_ipScale	= 4;
			return	( IS_SCCLS );
		}else
		if( c_sFieldName == "SC1DISP_F" ) {
			*c_ipScale	= 1;
			return	( IS_SCFLG );
		}else
		if( c_sFieldName == "SC2DISP_F" ) {
			*c_ipScale	= 2;
			return	( IS_SCFLG );
		}else
		if( c_sFieldName == "SC3DISP_F" ) {
			*c_ipScale	= 3;
			return	( IS_SCFLG );
		}else
		if( c_sFieldName == "SC4DISP_F" ) {
			*c_ipScale	= 4;
			return	( IS_SCFLG );
		}
	}
	return ( 0 );
}

/**
	�\�����[���̑Ó������m�F
*/
bool	main_rootin::
Chk_Disp_Rule	(	int		c_iDispState,	///< �\�����
					int		c_iRuleType )	///< �\�����[���̒l
{
	if( c_iDispState == NO_DISP ) {	// �\������
		if( c_iRuleType == 0 || c_iRuleType == 1 || c_iRuleType == 4) {
			return ( true );
		}
	}else
	if( c_iDispState == EX_STR ) {	// ������̂�
		if( c_iRuleType == 0 ) {
			return ( true );
		}
	}else
	if( c_iDispState == EX_MARK ) {	// �L���̂�
		if( c_iRuleType == 0 || c_iRuleType == 1 || c_iRuleType == 3) {
			return ( true );
		}
	}else
	if( c_iDispState == (EX_MARK|EX_STR) ) {	// �L���{������
		if( c_iRuleType == 0 || c_iRuleType == 1 || c_iRuleType == 2) {
			return ( true );
		}
	}else
	if( c_iDispState == (EX_POINT|EX_STR) ) {	// �w���_�{������
		if( c_iRuleType == 0 || c_iRuleType == 1 || c_iRuleType == 2 || c_iRuleType == 3) {
			return ( true );
		}
	}
	return ( false );
}

/**
	���L�\���̃X�P�[���Ԑ����`�F�b�N
*/
bool	main_rootin::
Chk_Scale_Disp_Rule ( int				c_iDigit,			///< �Y�����x����\�����b�V������
					  int*				c_ipNote_State,		///< ���L�̕\�����
					  note_disp_rule&	c_nFoundRule	)	///< ���L�̃��[��
{
	scale_disp_rule		a_sTgtRule;

	switch ( c_iDigit )
	{
	case	2:
		a_sTgtRule	= c_nFoundRule.e_lTopDispRule;
		break;
	case	4:
		a_sTgtRule	= c_nFoundRule.e_lMidDispRule;
		break;
	case	6:
		a_sTgtRule	= c_nFoundRule.e_lBaseDispRule;
		break;
	case	8:
		a_sTgtRule	= c_nFoundRule.e_lCityDispRule;
		break;
	}

	if( c_iDigit == 8 ) {
		if( Chk_Disp_Rule ( c_ipNote_State[0], a_sTgtRule.e_lDispRule[0] ) ) {
			return ( true );
		}
	}else {
		// �Ƃ肠�����S�X�P�[���̃��[���������Ă���ꍇ�̂�OK�Ƃ���
		if( Chk_Disp_Rule ( c_ipNote_State[1], a_sTgtRule.e_lDispRule[0] ) == true &&
			Chk_Disp_Rule ( c_ipNote_State[2], a_sTgtRule.e_lDispRule[1] ) == true &&
			Chk_Disp_Rule ( c_ipNote_State[3], a_sTgtRule.e_lDispRule[2] ) == true &&
			Chk_Disp_Rule ( c_ipNote_State[4], a_sTgtRule.e_lDispRule[3] ) == true ) {
			return ( true );
		}
	}
	return	( false );
}

/** ------------------------------------------------------------------------------
	���̒ǉ�
	@return	����
--------------------------------------------------------------------------------*/
void	main_rootin::
Put_Info	(	data_info		c_dInfo )		///< �������݃f�[�^[IN]
{
	ims_data_info	a_iIndx;

	int	roop	= 0;
	a_iIndx	= m_mData_Info.find( c_dInfo );
	if( a_iIndx == m_mData_Info.end() ) {	// �Y���f�[�^��������Ȃ��ꍇ�͒ǉ�
		m_mData_Info.insert ( c_dInfo );
	}else {
		data_info	a_dTmp_Dat	= *a_iIndx;
	//	a_dTmp_Dat.e_dLength	+= c_dInfo.e_dLength;	// ������ǉ�����
		for( roop = 0; roop < 4; roop++ )
		{
			a_dTmp_Dat.e_iSCount[roop]		+= c_dInfo.e_iSCount[roop];
			a_dTmp_Dat.e_iSCount_M[roop]	+= c_dInfo.e_iSCount_M[roop];
		}
		a_dTmp_Dat.e_iTrueCount	+= c_dInfo.e_iTrueCount;	// 2010.08.17
		// ��������f�[�^�Z�b�g����f�[�^���폜����
		m_mData_Info.erase	( a_iIndx );
		// �������C�������f�[�^��ǉ�����
		m_mData_Info.insert	( a_dTmp_Dat );
	}
}

/** 2007.01.10
	SiNDY��̑����t�B�[���h�����擾
*/
bool	main_rootin::
GetDBAttrFld	( string*	c_sSiNDYFld,	///< SiNDY��̃t�B�[���h��[OUT]
				  int*		c_ipDataType,	///< �f�[�^�^�C�v[OUT]
				  int*		c_ipShpType )	///< �V�F�C�v�f�[�^�̃^�C�v[OUT]
{

	if( strstr ( (char*)m_sAddStr.c_str(), "BGP" ) != NULL ) {
		*c_sSiNDYFld	= "BGCLASS_C";
		*c_ipDataType	= BGP;
		*c_ipShpType	= TYPE_POLY;
		return ( true );
	}else
	if( strstr ( (char*)m_sAddStr.c_str(), "BGL" ) != NULL ) {
		*c_sSiNDYFld	= "BGCLASS_C";
		*c_ipDataType	= BGL;
		*c_ipShpType	= TYPE_LINE;
		return ( true );
	}else
	if( strstr ( (char*)m_sAddStr.c_str(), "BLD" ) != NULL ) {
		*c_sSiNDYFld	= "BLDCLASS_C";
		*c_ipDataType	= BLD;
		*c_ipShpType	= TYPE_POLY;
		return ( true );
	}else
	if( strstr ( (char*)m_sAddStr.c_str(), "RAL" ) != NULL ) {
		*c_sSiNDYFld	= "RAILWAYCLASS_C";
		*c_ipDataType	= RAL;
		*c_ipShpType	= TYPE_LINE;
		return ( true );
	}else
	if( strstr ( (char*)m_sAddStr.c_str(), "USP" ) != NULL ) {
		*c_sSiNDYFld	= "STATIONCLASS_C";
		*c_ipDataType	= USP;
		*c_ipShpType	= TYPE_POLY;
		return ( true );
	}else
	if( strstr ( (char*)m_sAddStr.c_str(), "STA" ) != NULL ) {
		*c_sSiNDYFld	= "STATIONCLASS_C";
		*c_ipDataType	= STA;
		*c_ipShpType	= TYPE_POLY;
		return ( true );
	}else
	if( strstr ( (char*)m_sAddStr.c_str(), "CNT" ) != NULL ) {
		*c_sSiNDYFld	= "CNTCLASS_C";
		*c_ipDataType	= CNT;
		*c_ipShpType	= TYPE_POLY;
		return ( true );
	}else
	if( strstr ( (char*)m_sAddStr.c_str(), "ADM" ) != NULL ) {	// 2007.07.26 �ǉ�
	//	*c_sSiNDYFld	= "OBJECTID";
		*c_sSiNDYFld	= "D_ADMCLASS_C";
		*c_ipDataType	= ADM;
		*c_ipShpType	= TYPE_POLY;
		return ( true );
	}else
	if( strstr ( (char*)m_sAddStr.c_str(), "DCL" ) != NULL ) {
		*c_sSiNDYFld	= "D_DCLCLASS_C";
		*c_ipDataType	= DCL;
		*c_ipShpType	= TYPE_LINE;
		return ( true );
	}else
	if( strstr ( (char*)m_sAddStr.c_str(), "ANT" ) != NULL ) {	// 2008.04.07 �ǉ�
		*c_sSiNDYFld	= "ANNOCLASS_C";
		*c_ipDataType	= 0;
		*c_ipShpType	= TYPE_POINT;
		return ( true );
	}else{
		return ( false );
	}
}

/**
*/

/** ------------------------------------------------------------------------------
	���̏o��
	@return	����
--------------------------------------------------------------------------------*/
void	main_rootin::
Print_Info	(	ms_data_info*	c_mpmspCur_DSets,	///< ���ݒ��ڒ��̃f�[�^
				int				c_iData_Level,		///< �f�[�^���x��
				FILE*			c_fpOutFp	)		///< �o�̓t�@�C���|�C���^
{
	string	a_sLv;	// ���x��
	switch ( c_iData_Level )
	{
	case	SiINClassTable::top:	a_sLv	= "TOP";	break;
	case	SiINClassTable::mid:	a_sLv	= "MID";	break;
	case	SiINClassTable::base:	a_sLv	= "BASE";	break;
	case	SiINClassTable::city:	a_sLv	= "CITY";	break;
	default:
		break;
	}
	
	string	a_sData;
	if( m_sAddStr[0] == '_' ) {
		a_sData	= m_sAddStr.substr ( 1 );
	}else {
		a_sData	= m_sAddStr;
	}

	string	a_sDType;
	switch ( m_eGeometryType )
	{
	case esriGeometryPolygon:  a_sDType = "�|���S��"; break;
	case esriGeometryPolyline: a_sDType = "���C��";   break;
	default:                   a_sDType = "�|�C���g"; break;
	}

	string	a_sSiNDYFld;
	int		a_sDataType = 0;
	int		a_iShpType	= 0;
	GetDBAttrFld ( &a_sSiNDYFld, &a_sDataType, &a_iShpType );

	ims_data_info	a_iIndx;
	for( a_iIndx = c_mpmspCur_DSets->begin(); a_iIndx != c_mpmspCur_DSets->end(); a_iIndx++ )
	{
		data_info	a_dDat	= *a_iIndx;

		// �w�b�_�\��
		if( a_iIndx == c_mpmspCur_DSets->begin() ) {
			if( m_iExe_Flag & ON_MESHCOUNT ) {
				fprintf	( c_fpOutFp,"���b�V��\t" );
			}
			fprintf	( c_fpOutFp,"���x��\t�f�[�^\t��ʖ���\t" );
			if( !(m_iExe_Flag & ON_DATATYPEOFF) ) {
				fprintf	( c_fpOutFp,"�f�[�^�^�C�v\t");
			}
			fprintf	( c_fpOutFp,"��ʃR�[�h\t�n���t���O\t");

			if( m_iExe_Flag & ON_NOTECOUNT ) {
				fprintf	( c_fpOutFp,"�^�ʒu\t�L��(S1)\t���L(S1)\t�L��(S2)\t���L(S2)\t�L��(S3)\t���L(S3)\t�L��(S4)\t���L(S4)\n");
			}else {
				fprintf	( c_fpOutFp, "S1��\tS2��\tS3��\tS4��\n");
			}
		}

		SiINClassTable::Peace	tblinfo;
		tblinfo	= m_sAttrTable. GetSiToINInfo	(	a_sSiNDYFld,			// SiNDY�t�B�[���h����
													a_iShpType,				// �f�[�^�^�C�v
													a_dDat.e_iUnderF,		// �n��E�n���t���O(0:�n��A1:�n���j
													a_dDat.e_iSindyAttr );	// ��ʃR�[�h
		if( m_iExe_Flag & ON_MESHCOUNT ) {
			fprintf	( c_fpOutFp,"%02d\t", a_dDat.e_iMeshCode );
		}

		// �J�E���g�\��

		fprintf	( c_fpOutFp,	"%s\t%s\t%s\t",
								(char*)a_sLv.c_str(),
								(char*)a_sData.c_str(),
								&tblinfo.eName[0] );

		if( !(m_iExe_Flag & ON_DATATYPEOFF) ) {
			fprintf	( c_fpOutFp,"%s\t", (char*)a_sDType.c_str() );
		}

		if( m_iExe_Flag & ON_NOTECOUNT ) {
			fprintf	( c_fpOutFp,	"%d\t%d\t"
									"%d\t"		// �^�ʒu 2010.08.17
									"%d\t%d\t"	// s1
									"%d\t%d\t"	// s2
									"%d\t%d\t"	// s3
									"%d\t%d\n",	// s4
									 a_dDat.e_iSindyAttr, 0,// �_�~�[�̒n���t���O
									 a_dDat.e_iTrueCount, // 2010.08.17
									 a_dDat.e_iSCount_M[0], a_dDat.e_iSCount[0],
									 a_dDat.e_iSCount_M[1], a_dDat.e_iSCount[1],
									 a_dDat.e_iSCount_M[2], a_dDat.e_iSCount[2],
									 a_dDat.e_iSCount_M[3], a_dDat.e_iSCount[3]);
		}else {
			fprintf	( c_fpOutFp,	"%d\t%d\t"
									"%d\t%d\t%d\t%d\n",	// s1�`s4
									 a_dDat.e_iSindyAttr,
									 a_dDat.e_iUnderF,
									 a_dDat.e_iSCount[0], a_dDat.e_iSCount[1], a_dDat.e_iSCount[2], a_dDat.e_iSCount[3]);
		}
	}
}

/**
	�����s��
*/
bool main_rootin::
exec_command(	IWorkspacePtr	c_ipWSpace )	///< ���[�N�X�y�[�X
{
	bool bRet = true;

	//------------------------------------------------------------------------
	// �����Ή��e�[�u���̓ǂݍ��� 2006.01.10
	if( m_iExe_Flag & ON_MESHLIST ) {

		// ���b�V�����X�g�쐬
		if( !m_uUC.MakeMeshList( (char*)m_sMeshList.c_str(), &m_sMesh ) ) {
			fprintf( m_fpOutLogFp, "���b�V�����X�g <%s> ���I�[�v���ł��܂���\n", (char*)m_sMeshList.c_str() );
			return	( false );
		}

	}else {
		m_sMesh.insert ( m_lMeshCode );
	}

	char	a_cpMeshStr[10];
	memset	( a_cpMeshStr, '\0', 10 );
	sprintf	( a_cpMeshStr, "%.2d", *m_sMesh.begin() );
	
	switch ( m_cCrd.GetMeshDigit ( a_cpMeshStr ) )
	{
	case	2:	m_iInput_Lv	= SiINClassTable::top; break;
	case	4:	m_iInput_Lv	= SiINClassTable::mid; break;
	case	6:	m_iInput_Lv	= SiINClassTable::base; break;
	case	8:	m_iInput_Lv	= SiINClassTable::city;	break;
	default:	m_iInput_Lv	= SiINClassTable::top;	break;
	}
	//------------------------------------------------------------------------

	// �����e�[�u���̎擾
	if( m_sAttrTable.Read_DB_Table ( (char*)m_sAttrTableName.c_str(), c_ipWSpace, m_iInput_Lv ) == -1 ) {
		return	( false );
	}

	// 2010.08.19 ------------------------------------------------------------

	string	a_sSiNDYFld2;
	int		a_sDataType2 = 0;
	int		a_iShpType2	= 0;
	if( !GetDBAttrFld( &a_sSiNDYFld2, &a_sDataType2, &a_iShpType2 ) )
		return false;

	// �W�I���g���^�C�v�͕\���́u�|�C���g�v���̐؂蕪���̂ݎg�p�B
	switch ( a_iShpType2 )
	{
	case	TYPE_LINE:   m_eGeometryType = esriGeometryPolyline; break;
	case	TYPE_POLY:   m_eGeometryType = esriGeometryPolygon; break;
	default :            m_eGeometryType = esriGeometryPoint ; break;
	}

	if( m_iExe_Flag & ON_ALLATTR && !(m_iExe_Flag & ON_MESHCOUNT) ) {

		multiset<SiINClassTable::Peace, SiINClassTable::lessC>::iterator	a_iIndx2;
		for( a_iIndx2 = m_sAttrTable.mTable.begin(); a_iIndx2 != m_sAttrTable.mTable.end(); a_iIndx2++ )
		{
			SiINClassTable::Peace tmptbl	= *a_iIndx2;

			if( tmptbl.eSiFieldName != a_sSiNDYFld2 ||
				tmptbl.eData_Type != a_iShpType2 ) {
				continue;
			}

			switch ( m_iInput_Lv )
			{
			case	SiINClassTable::top:	if( tmptbl.m_Top_ExF == 0 ) {continue;}	break;
			case	SiINClassTable::mid:	if( tmptbl.m_Mid_ExF == 0 ) {continue;}	break;
			case	SiINClassTable::base:	if( tmptbl.m_Base_ExF == 0 ) {continue;}break;
			case	SiINClassTable::city:	if( tmptbl.m_City_ExF == 0 ) {continue;}break;
			default:
				break;
			}
			// �o�͂��Ă܂�����ʂ͔�΂�
			//BASE	BGL	�s���{���E	���C��	11
			//BASE	BGL	�s�撬���E	���C��	12
			if( m_iInput_Lv == SiINClassTable::base ) {
				if( tmptbl.eSiFieldName == "BGCLASS_C" &&
					(tmptbl.eSiClass == 11 || tmptbl.eSiClass == 12) ) {
					continue;
				}
			}else
			if( m_iInput_Lv == SiINClassTable::city ) {
				if( tmptbl.eSiFieldName == "BLDCLASS_C" ) {
					switch ( tmptbl.eSiClass )
					{
					case	3:	//CITY	BLD	�n���S�ȊO�w�z�[���i�n��j	�|���S��	3
					case	4:	//CITY	BLD	�n���S�w�z�[���i�n��j	�|���S��	4
					case	16:	//CITY	BLD	�s��	�|���S��	16
					case	17:	//CITY	BLD	�H��	�|���S��	17
					case	18:	//CITY	BLD	�q��	�|���S��	18
						continue;
					}
				}
			}

			// �����ŊY�����郌�x���̎�ʂ��J�E���g�p�̃R���e�i�Ɋi�[���Ă��܂��B
			data_info	a_dTmp_Dat;
			a_dTmp_Dat.e_iSindyAttr	= tmptbl.eSiClass;
			a_dTmp_Dat.e_iUnderF	= tmptbl.eUnder_Flag;
			a_dTmp_Dat.e_sFIELD		= tmptbl.eSiFieldName;
			
			m_mData_Info.insert	( a_dTmp_Dat );
		}
	}
	//------------------------------------------------------------------------

	// ���L�����J�E���g
	if( m_iExe_Flag & ON_NOTECOUNT ) {

		if( !(m_iExe_Flag & ON_MONO_MESH || m_iExe_Flag & ON_MESHLIST) ) {
			fprintf( m_fpOutLogFp, "���L�̑����̊֘A���`�F�b�N�ɂ̓��b�V�����X�g���K�v�ł�\n" );
			return	( false );
		}

		m_sSrcShpPath	= m_sSrcShpFile;	// �p�X�Ƃ��Ă���

		fprintf( m_fpOutLogFp, "_ANT ���̓p�X : %s\n", (char*)m_sSrcShpPath.c_str() );
		fprintf( m_fpOutLogFp, "_XDA ���̓p�X : %s\n", (char*)m_sNoteDispFile.c_str() );
		fprintf( m_fpOutLogFp, "_XDM ���̓p�X : %s\n", (char*)m_sMarkDispFile.c_str() );

		set<int>::iterator	a_iIndx;
		for( a_iIndx = m_sMesh.begin(); a_iIndx != m_sMesh.end(); a_iIndx++ )
		{
			int	a_iMesh	= *a_iIndx;
			string		a_sFName_ANT, a_sFName_XDA, a_sFName_XDM;
		//	a_sFName	= make_mesh_fname ( a_iMesh, e_cpNoteDispFile, "_ANT");

			a_sFName_ANT	= make_mesh_fname ( a_iMesh, (char*)m_sSrcShpPath.c_str(), "_ANT");
			a_sFName_XDA	= make_mesh_fname ( a_iMesh, (char*)m_sNoteDispFile.c_str(), "_XDA");
			a_sFName_XDM	= make_mesh_fname ( a_iMesh, (char*)m_sMarkDispFile.c_str(), "_XDM");

			string	a_sSrcShpPath, a_sNoteDispPath, a_sMarkDispFile;
			string	a_sSrcFClass, a_sNDFClass, a_sMDFClass;

			// �p�X�����o��
			GetFilePath		( (char*)a_sFName_ANT.c_str(), a_sSrcShpPath );
			GetFilePath		( (char*)a_sFName_XDA.c_str(), a_sNoteDispPath );
			GetFilePath		( (char*)a_sFName_XDM.c_str(), a_sMarkDispFile );
			// �t�@�C�����i�t�B�[�`���N���X���j�����o��
			GetFeatureClass	( (char*)a_sFName_ANT.c_str(), a_sSrcFClass );
			GetFeatureClass	( (char*)a_sFName_XDA.c_str(), a_sNDFClass );
			GetFeatureClass	( (char*)a_sFName_XDM.c_str(), a_sMDFClass );

			v_Feature_All_Attrs	a_vpSrcData, a_vpNDFData, a_vpMDFData; 

			if( !read_info ( a_iMesh, (char*)a_sSrcShpPath.c_str(), (char*)a_sSrcFClass.c_str(), &a_vpSrcData ) ) { bRet = false; continue; }
			if( !read_info ( a_iMesh, (char*)a_sNoteDispPath.c_str(), (char*)a_sNDFClass.c_str(), &a_vpNDFData ) ) { bRet = false; continue; }
			if( !read_info ( a_iMesh, (char*)a_sMarkDispFile.c_str(), (char*)a_sMDFClass.c_str(), &a_vpMDFData ) )  { bRet = false; continue; }

			fprintf(stdout, "<%.2d> %s �J�E���g��...", a_iMesh, (char*)m_sAddStr.c_str() );
			// ���L���̃J�E���g
			if( !count_note_info ( a_iMesh, a_vpSrcData, a_vpNDFData, a_vpMDFData ) ) {
				bRet = false;
			}
			fprintf(stdout, "�I��\n" );
		}
	}else {	// �ʏ�̑����`�F�b�N


		if( m_iExe_Flag & ON_MONO_MESH || m_iExe_Flag & ON_MESHLIST ) {
			m_sSrcShpPath	= m_sSrcShpFile;	// �p�X�Ƃ��Ă���

			fprintf( m_fpOutLogFp, "%s ���̓p�X : %s\n",(char*)m_sAddStr.c_str(), (char*)m_sSrcShpPath.c_str() );

			set<int>::iterator	a_iIndx;
			for( a_iIndx = m_sMesh.begin(); a_iIndx != m_sMesh.end(); a_iIndx++ )
			{
				int	a_iMesh	= *a_iIndx;
				string		a_sFName;
				a_sFName	= make_mesh_fname ( a_iMesh, (char*)m_sSrcShpPath.c_str(), (char*)m_sAddStr.c_str() );

				string	a_sSrcShpPath;
				string	a_sSrcFClass;

				// �p�X�����o��
				GetFilePath		( (char*)a_sFName.c_str(), a_sSrcShpPath );	
				// �t�@�C�����i�t�B�[�`���N���X���j�����o��
				GetFeatureClass	( (char*)a_sFName.c_str(), a_sSrcFClass );

				fprintf(stdout, "<%.2d> %s �J�E���g��...", a_iMesh, (char*)m_sAddStr.c_str() );
				if( !count_info ( a_iMesh, (char*)a_sSrcShpPath.c_str(), (char*)a_sSrcFClass.c_str() ) ) {
					fprintf( m_fpOutLogFp, "<%d>\t���s���s\n", a_iMesh );
					bRet = false;
				}
				fprintf(stdout, "�I��\n" );
			}

		}else {

			// �p�X�����o��
			GetFilePath		( (char*)m_sSrcShpFile.c_str(), m_sSrcShpPath );	
			// �t�@�C�����i�t�B�[�`���N���X���j�����o��
			GetFeatureClass	( (char*)m_sSrcShpFile.c_str(), m_sSrcFClass );

			fprintf(stdout, "%s �J�E���g��...", (char*)m_sAddStr.c_str() );
			if( !count_info ( 0, (char*)m_sSrcShpPath.c_str(), (char*)m_sSrcFClass.c_str() ) ) {
				fprintf( m_fpOutLogFp, "���s���s\n" );
				bRet = false;
			}
			fprintf(stdout, "�I��\n" );
		}
	}

	// �J�E���g�̏o��
	Print_Info	(&m_mData_Info, m_iInput_Lv, m_fpOutInfoFp );

	return ( bRet );
}
