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
#include "main_routine.h"

#include <atlstr.h>
#include "..\..\LIB\WinLib\VersionInfo.h"
#include <sindy/workspace.h>
#include <boost/program_options.hpp>

#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

namespace {
	typedef boost::char_separator<char> char_separator;
	typedef boost::tokenizer<char_separator> tokenizer;
}

// DB�ւ̐ڑ��I�[�v��
bool	main_routine::
DB_Open ( const CString&  c_csConnectStr, //!< [in] �ڑ�������
          IWorkspacePtr&  c_ipWorkspace ) //!< [in,out] Workspace�̎Q��
{
	IWorkspacePtr ipWorkspace =  sindy::create_workspace(c_csConnectStr);
	if(!ipWorkspace ){
		return false;
	}
	c_ipWorkspace	= ipWorkspace;
	return true;
}

//	���s���O�t�@�C�����I�[�v������
bool	main_routine::
Open_Exe_Log ( void )
{
	if( m_sExeLog_File == "" ) {
		return	true;
	}else {
		m_fExe_Fp	= fopen ( m_sExeLog_File.c_str(), "w" );
		if( m_fExe_Fp == NULL ) {
			return false;
		}
	}
	return true;
}


//	�G���[���O�t�@�C�����I�[�v������
bool	main_routine::
Open_Err_Log ( void )
{
	if( m_sLog_File == "" ) {
		return	true;
	}else {
		m_fErr_Fp	= fopen ( m_sLog_File.c_str(), "w" );
		if( m_fErr_Fp == NULL ) {
			return false;
		}
	}
	return true;
}

// �g�p�@�\��
void		main_routine::
print_usage	(char **argv)
{
	PrnMsg(	stdout, NULL,
		"=== �y���n�n�ϊ��c�[���z(v%s) ===\n"
				"<usage>\n"
				"%s <�I�v�V����>... \n"
				"\t      '-d'(db)        : �������Ώ�DB�ڑ�(ex. -d SINDY2012B/SINDY2012B/SDE.SNP_delete_130220/5151/ruby2 )\n"
				"\t      '-f'(fclass)    : �������Ώۃt�B�[�`���N���X(ex. -f SINDY2012B.Building )\n"
				"\t      '-l'(errlog)    : ���G���[���O�t�@�C�������w��(ex. -l .\\test\\ConvtoJGD.log)\n"
				"\t      '-x'(exelog)    : �����s���O�t�@�C�������w��(ex. -l .\\test\\ConvtoJGD_Exe.log)\n"
				"\t      '-p'(parameter) : ���n��p�����[�^�t�@�C�������w��(ex. -p TKY2JGD.par)\n"
				"\t      '-u'(update)    :   �X�V���[�h�w��i���w�莞�̓e�X�g���[�h�j\n"
				"\t      �����󂪂��Ă�����͕̂K�{�I�v�V�����B\n"
				"\t      \n", m_sVersion, argv[0] );
}

using boost::program_options::options_description;
using boost::program_options::value;
using boost::program_options::variables_map;
using boost::program_options::store;
using boost::program_options::parse_command_line;
using boost::program_options::notify;

// �����`�F�b�N
bool	main_routine::
arg_check ( int    argc,  //!< [IN] ������
            char** argv ) //!< [IN] ������
{
	//-----------------------------------------------------------------------
	// �c�[���o�[�W�����\��
	CVersion	a_cvTool_Var;
	m_sVersion	= a_cvTool_Var.GetFileVersion();
	//-----------------------------------------------------------------------

	options_description oOption("�I�v�V����");

	// �����̏������`
	oOption.add_options()
	//	("db1,d", value<string>(&m_sSrc_Path), "�폜�����Ώ�DB�ڑ�1")
		("db,d", value<std::string>(&m_sDB_Connect1), "�����Ώ�DB�ڑ�")
		("fclass,f", value<std::string>(&m_sFeatureClass1),"�����Ώۃt�B�[�`���N���X")
		("errlog,l", value<std::string>(&m_sLog_File), "�G���[���O�t�@�C����")
		("exelog,x", value<std::string>(&m_sExeLog_File), "���s���O�t�@�C����")
		("parameter,p", value<std::string>(&m_sParam_File), "�n��p�����[�^�t�@�C����")
		("update,u", "�X�V���[�h�w��");

	variables_map vArgmap;

	try {
		store(parse_command_line(argc, argv, oOption), vArgmap);
		notify(vArgmap);

		// �ڑ�DB�w�肪�����ꍇ�͏I��
		if ( !vArgmap.count("db") ||
			 !vArgmap.count("fclass") ||
			 !vArgmap.count("errlog") ||
			 !vArgmap.count("exelog") )
		{
			PrnMsg ( m_fErr_Fp, stderr, "ERROR:�K�{�I�v�V�����������Ă��܂�\n");
			return false;
		}

		if( vArgmap.count("update") ) { m_lExe_Flag |= ON_UPDATEMODE; }
		if( vArgmap.count("parameter") ) { m_lExe_Flag |= ON_RPARAM; }
	}catch (...) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:�I�v�V�����̎w��ɊԈႢ������܂�\n");
		return false;
	}

	// crd_cnv�̏�����
	if(  m_lExe_Flag & ON_RPARAM ) {
		if( !m_cCrd.JGD2000_RP_Init ( (char*)m_sParam_File.c_str() ) ) {
			PrnMsg ( stderr, NULL, "ERROR:�n��p�����[�^�t�@�C�����I�[�v���ł��Ȃ� : <%s>\n", m_sParam_File.c_str() );
			return	false;
		}
	}

	// ���s���O�t�@�C���̃I�[�v��
	if( !Open_Exe_Log () ) {
		PrnMsg ( stderr, NULL, "ERROR:���s���O�t�@�C�����I�[�v���ł��Ȃ� : <%s>\n", m_sExeLog_File.c_str() );
		return	false;
	}


	// �G���[���O�t�@�C���̃I�[�v��
	if( !Open_Err_Log () ) {
		PrnMsg ( stderr, NULL, "ERROR:���O�t�@�C�����I�[�v���ł��Ȃ� : <%s>\n", m_sLog_File.c_str() );
		return	false;
	}

	//-----------------------------------------------------------------------------------------------
	// ���s�����o��
	PrnMsg( m_fErr_Fp, stderr, "#SINDYSTDLOG\n");
	PrnMsg( m_fErr_Fp, stderr, "#�c�[���o�[�W����              : <%s>\n", m_sVersion );
	PrnMsg( m_fErr_Fp, stderr, "#�����Ώ�DB�ڑ�                : <%s>\n", m_sDB_Connect1.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#�����Ώۃt�B�[�`���N���X      : <%s>\n", m_sFeatureClass1.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#���s���[�h                    : <%s>\n", m_lExe_Flag&ON_UPDATEMODE?"�X�V���[�h":"�e�X�g���[�h" );
	//-----------------------------------------------------------------------------------------------
	
	if( m_lExe_Flag & ON_UPDATEMODE ) { // �폜���[�h�̏ꍇ
		if( m_sDB_Connect1.find ( ".gdb" ) == std::string::npos && 
			m_sDB_Connect1.find ( ".mdb" ) == std::string::npos ) {
			PrnMsg	( stdout, NULL, "#��LDB�ɑ΂��X�V���������s���܂����ԈႢ����܂��񂩁H<y/n> :" );
			int	a_cChkChar	= 0;
			// �R���\�[�����當�����擾	
			a_cChkChar	= toupper( getchar() );
			PrnMsg	( stdout, NULL, "\n");
			if( a_cChkChar != 'Y' ) {
				PrnMsg ( stdout, NULL, "#�X�V�����𒆒f���܂�\n");
				return false;
			}
		}
	}

	// �J�n���Ԃ̐ݒ�
	m_eExeTime.Set_Start();

	//--------------------------------------------------------------------------
	// DB�ڑ��֘A�A�t�B�[�`���N���X�̃I�[�v������
	//--------------------------------------------------------------------------
	CString	a_sDB_Connect1 = m_sDB_Connect1.c_str();

	if( !DB_Open ( a_sDB_Connect1, ipWorkspace_1 ) ) {
		PrnMsg( m_fErr_Fp, stderr, "ERROR:DB���I�[�v���ł��Ȃ� : <%s>\n", a_sDB_Connect1 );
		return false;
	}

	// FC�̃I�[�v������
	IFeatureWorkspacePtr	ipFeWspace_1 ( ipWorkspace_1 );

	if( FAILED (ipFeWspace_1->OpenFeatureClass ( _bstr_t ( m_sFeatureClass1.c_str() ), &m_ipFeatureClass_1 ) ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:�t�B�[�`���N���X���I�[�v���ł��܂���ł��� : <%s>\n", m_sFeatureClass1.c_str() ); 
		return false;
	}

	// DB�ڑ��ɂ����������Ԃ̕\��
	PrnMsg ( m_fErr_Fp, stderr, "#DB�ڑ��������� = %0.2f(s)�ł���\n", (double)((m_eExeTime.Get_From_Start()) / 1000.0) );

	return true;
}

// ���X�g�̓ǂݍ���
bool main_routine::
Read_List ( const std::string& c_sFileName, //!< [in] �t�@�C����
            std::set<long>&    c_sList )    //!< [in] ���X�g�iID���Ɏg�p�j
{
	const int bmax	= 512;
	FILE*     a_pFp;		
	char      a_cBuf[bmax];
	int       a_StrLen = 0;	

	a_pFp = fopen ( c_sFileName.c_str(), "r" );
	if(a_pFp == NULL) {
		return false;
	}

	while( fgets ( a_cBuf, bmax, a_pFp ) != NULL)
	{
		a_StrLen	= strlen ( a_cBuf );
		// �Ō��"\n"����菜��
		if(a_cBuf[a_StrLen - 1] == '\n') {
			a_cBuf[a_StrLen - 1] = '\0';
		}
		// �R�����g�����΂�
		if(a_cBuf[0] == '#' || a_cBuf[0] == '\0') {
			continue;
		}
		// ���X�g�ɒǉ�
		c_sList.insert ( atol ( a_cBuf ) );
	}
	fclose ( a_pFp );
	return true;
}

/** ------------------------------------------------------------------------------
	�t�B�[�`�����o�͗p�̃t�B�[�`���ɂ���
	@return	�f�[�^��
--------------------------------------------------------------------------------*/
int main_routine::
Make_Data_Container ( const IFeatureCursorPtr&                               c_ipFCursor,   //!< [IN] ���L�J�[�\��
                      std::map<long, CAdapt<IFeaturePtr>, std::less<long> >& c_mWork_Data ) //!< [IN] �o�͗p�f�[�^
{
	int	a_iCount	= 0;

	// �f�[�^vector�R���e�i�ɗ��Ƃ�
	if( c_ipFCursor != NULL ) {

		IFeaturePtr		a_ipCur_Feat;
		while ( c_ipFCursor->NextFeature ( &a_ipCur_Feat ) == S_OK )
		{
			long	a_lOID = 0;
			a_ipCur_Feat->get_OID ( &a_lOID );

			IGeometryPtr	a_ipWorkGeo;	//!< ��Ɨp
			a_ipCur_Feat->get_Shape (&a_ipWorkGeo);

			c_mWork_Data.insert ( std::map<long, CAdapt<IFeaturePtr>, std::less<long> >::value_type ( a_lOID, a_ipCur_Feat) );
			a_iCount++;
		}
	}
	return	( a_iCount );
}

//	�w��t�B�[�`���ɑ΂���esriSpatialRelEnum�Ŏw�肵���֌W�����f�[�^��
//	�Ƃ��Ă���
IFeatureCursorPtr   main_routine::
Search_Feature ( const IFeatureClassPtr& c_ipFeCls,     //!< [in] �t�B�[�`���N���X
                 const IGeometryPtr&     c_ipGeo,       //!< [in] �����p�W�I���g��
                 esriSpatialRelEnum      c_ipSpatialRel,//!< [in] �֌W�w��
                 std::string             c_sWhere,      //!< [in] Where��w��
                 long&                   c_iCount )     //!< [out] ������
{
	ISpatialFilterPtr	a_ipFilter(CLSID_SpatialFilter);
	CComBSTR			a_ccSpCol;

	if (FAILED(c_ipFeCls->get_ShapeFieldName ( &a_ccSpCol )) || // �V�F�C�v�t�B�[���h���擾
	    FAILED(a_ipFilter->putref_Geometry ( c_ipGeo )) ||      // �T�[�`�p�̃W�I���g��
	    FAILED(a_ipFilter->put_GeometryField ( a_ccSpCol )) ||  // �W�I���g���t�B�[���h
	    FAILED(a_ipFilter->put_SpatialRel ( c_ipSpatialRel )) ||// SpatialRel()
	    FAILED(a_ipFilter->put_SearchOrder ( esriSearchOrderSpatial )) ) { // �T�[�`�I�[�_�[
		PrnMsg( m_fErr_Fp, stderr, "ERROR:Filter�̐ݒ�Ɏ��s[Search_Feature]\n");
		return	( NULL );
	}

	if( c_sWhere.size() > 1 ) {
		if ( FAILED ( a_ipFilter->put_WhereClause ( _bstr_t ( c_sWhere.c_str() ) ) ) )	{
			PrnMsg( m_fErr_Fp, stderr, "ERROR:�����pWHERE��̐ݒ肪�ł��Ȃ�[Search_Feature]" );
			return	( NULL );
		}
	}
	// �f�[�^�J�E���g�����
	long		a_lData_Count	= 0;
	if ( FAILED ( c_ipFeCls->FeatureCount ( (IQueryFilterPtr)a_ipFilter, &a_lData_Count ) ) ) {
		PrnMsg( m_fErr_Fp, stderr, "ERROR:�����f�[�^�����擾�ł��Ȃ�[Search_Feature]\n");
		return	( NULL );
	}
	c_iCount	= a_lData_Count;

	IFeatureCursorPtr	a_ipFeCsr;	//	�ԋp�p�t�B�[�`���J�[�\��
	if (FAILED( c_ipFeCls->Search((IQueryFilterPtr)a_ipFilter, VARIANT_FALSE, &a_ipFeCsr)) ) {
		PrnMsg( m_fErr_Fp, stderr, "ERROR:�f�[�^�̃T�[�`���ł��Ȃ�[Search_Feature]\n");
		return	( NULL );
	}
	return	( a_ipFeCsr );
}

// �I�u�W�F�N�gID���X�g�̍쐬
bool main_routine::
MakeOBJECTIDList ( const IFeatureClassPtr& c_ipFeCls,         //!< [in] �t�B�[�`���N���X
                   std::set<long>&         c_sOBJECTID_List ) //!< [out] OBJECTID���X�g
{
	IQueryFilterPtr	a_ipFilter(CLSID_QueryFilter);
	CComBSTR		a_ccSpCol;

//		if ( FAILED ( a_ipFilter->put_WhereClause ( _bstr_t ( "OBJECTID  ) ) ) )	{
//			PrnMsg( m_fErr_Fp, stderr, "ERROR:�����pWHERE��̐ݒ肪�ł��Ȃ�[Search_Feature]" );
//			return	( NULL );
//		}
//	}
	// �f�[�^�J�E���g�����
	long		a_lData_Count	= 0;
	if ( FAILED ( c_ipFeCls->FeatureCount ( (IQueryFilterPtr)a_ipFilter, &a_lData_Count ) ) ) {
		PrnMsg( m_fErr_Fp, stderr, "ERROR:�����f�[�^�����擾�ł��Ȃ�[Search_Feature]\n");
		return false;
	}

	IFeatureCursorPtr	a_ipFeCsr;	//	�ԋp�p�t�B�[�`���J�[�\��
	if (FAILED( c_ipFeCls->Search((IQueryFilterPtr)a_ipFilter, VARIANT_FALSE, &a_ipFeCsr)) ) {
		PrnMsg( m_fErr_Fp, stderr, "ERROR:�f�[�^�̃T�[�`���ł��Ȃ�[Search_Feature]\n");
		return false;
	}

	long a_lCount = 0;
	IFeaturePtr	a_ipFeature;
	while ( a_ipFeCsr->NextFeature ( &a_ipFeature ) == S_OK )
	{
		if( a_lCount % 100 == 0 ) {
			PrnMsg( stdout, NULL, "%d/%d��������\r", a_lCount, a_lData_Count );			
		}
		long a_lOID = 0;
		a_ipFeature->get_OID ( &a_lOID );
		c_sOBJECTID_List.insert ( a_lOID );
		++a_lCount;
	}
	PrnMsg( stdout, NULL, "\n" );			
	return true;
}

// �G�f�B�b�g�̊J�n
bool main_routine::
StartEdit ( void )
{
	try {
		// ���L��Edit���[�h�ɕύX
		m_ipWorkspaceEdit_1	= ipWorkspace_1;

		// �ҏW�J�n�iArcMap �̕ҏW�J�n�Ɠ����j
		if( FAILED ( m_ipWorkspaceEdit_1->StartEditing( VARIANT_FALSE ) )) { // Undo Redo ������Ȃ��Ȃ� VARIANT_FALSE
			return false;
		}else {
			if( FAILED ( m_ipWorkspaceEdit_1->StartEditOperation() )) {
				return false;
			}
		}
	}catch ( ... ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:StartEditing���ɗ�O����\n" );
		return false;	
	}
	return true;
}

// �G�f�B�b�g�̏I��
bool main_routine::
StopEdit ( void )
{
	try {
		// �ҏW�̏I��
		if ( FAILED ( m_ipWorkspaceEdit_1->StopEditOperation() )) {
			return false;
		}else {
			if ( FAILED ( m_ipWorkspaceEdit_1->StopEditing( VARIANT_TRUE ) ) ) {
				//-----------------------------------------
				IErrorInfoPtr	ipErrorInfo;
				CString msg = "Unknown Error";
				::GetErrorInfo(0, &ipErrorInfo);
				if(ipErrorInfo != NULL){
					BSTR	hoge;
					ipErrorInfo->GetDescription(&hoge);
					msg	= hoge;
				}
				PrnMsg ( m_fErr_Fp, stderr, "ERROR:StopEditing�Ɏ��s���܂���\t[%s]", msg );
				//-----------------------------------------
				return ( false );
			}
		}
	}catch ( ... ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:StopEditing���ɗ�O����\n" );
		return false;
	}
	return true;
}

// ���̃I�u�W�F�N�g�ɑ΂���^�[�Q�b�g�̊֌W�`�F�b�N
bool main_routine::
RelCheck ( const IGeometryPtr& c_ipSrcGeom, //!< [in] ��r���W�I���g��
           const IGeometryPtr& c_ipTgtGeom, //!< [in] ��r��W�I���g��
           long                c_lRel_Type )//!< [in] �֌W�̃^�C�v
{
	VARIANT_BOOL a_bRet;
	IRelationalOperatorPtr	a_ipRelOpe ( c_ipSrcGeom );

	try {
		switch ( c_lRel_Type )
		{
		case contains: a_ipRelOpe->Contains ( c_ipTgtGeom, &a_bRet ); break;
		case overlaps: a_ipRelOpe->Overlaps ( c_ipTgtGeom, &a_bRet ); break;
		case within:   a_ipRelOpe->Within ( c_ipTgtGeom, &a_bRet ); break;
		case touches:  a_ipRelOpe->Touches ( c_ipTgtGeom, &a_bRet ); break;
		case equals:   a_ipRelOpe->Equals ( c_ipTgtGeom, &a_bRet ); break;
		case crosses:  a_ipRelOpe->Crosses ( c_ipTgtGeom, &a_bRet ); break;
		}
	}catch ( ... ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:RelCheck���ɗ�O����\n" );
		return false;
	}
	if( a_bRet == VARIANT_TRUE ) {
		return true;
	}
	return false;
}

/**
 * @brief ��\�_���擾
 * @return 
 * @return	����	true
 * @return	����	false
 */
bool main_routine::
GetPoint ( const IGeometryPtr& c_ipGeom, //!< [in] �W�I���g��
		   double&             c_dX,     //!< [out] �o�x
		   double&             c_dY )    //!< [out] �o�x
{
	// ������
	c_dX = 0.0;
	c_dY = 0.0;

	// �W�I���g���^�C�v
	esriGeometryType a_eGeomType;
	if( FAILED ( c_ipGeom->get_GeometryType ( &a_eGeomType ) ) ) {
		return false;
	}

	double a_dX = 0.0, a_dY = 0.0;

	switch ( a_eGeomType )
	{
	case esriGeometryPoint:
		{
			IPointPtr a_ipPoint ( c_ipGeom );
			a_ipPoint->get_X ( &a_dX );
			a_ipPoint->get_Y ( &a_dY );
		}
		break;
	case esriGeometryPolyline:
		{
			IPolylinePtr a_ipPolyline( c_ipGeom );
			double a_dLen = 0.0;
			a_ipPolyline->get_Length( &a_dLen );
			IPointPtr a_ipPt(CLSID_Point);
			a_ipPolyline->QueryPoint( esriNoExtension, a_dLen/2.0, VARIANT_FALSE, a_ipPt );
			a_ipPt->get_X ( &a_dX );
			a_ipPt->get_Y ( &a_dY );
		}
		break;
	case esriGeometryPolygon:
		{
			IAreaPtr a_ipArea ( (IPolygonPtr)c_ipGeom );
			IPointPtr a_ipPt;
			a_ipArea->get_LabelPoint ( &a_ipPt );
			a_ipPt->get_X ( &a_dX );
			a_ipPt->get_Y ( &a_dY );
		}
		break;
	}
	c_dX = a_dX;
	c_dY = a_dY;

	return true;
}

// JGD�ڍs�����W�I���g�����쐬����
IGeometryPtr main_routine::
Make_JGD_Geometry ( IGeometryPtr& c_ipGeom ) //!< [in] �W�I���g��
{
	// ��ԎQ�Ƃ��擾
	ISpatialReferencePtr a_ipSpRef;
	if( FAILED (c_ipGeom->get_SpatialReference ( &a_ipSpRef )) ) {
		return NULL;
	}

	if( m_ipGeom_Type == esriGeometryPoint ) {

		try {
	
			IPointPtr	a_ipPt(c_ipGeom);

			double	dBefore_X = 0, dBefore_Y = 0;
			a_ipPt->QueryCoords ( &dBefore_X, &dBefore_Y );

			double dAfter_He = 0, dAfter_X = 0, dAfter_Y = 0;
			if( m_lExe_Flag & ON_RPARAM ) {
				m_cCrd.TOKYOtoJGD2000_RP ( dBefore_X,  dBefore_Y, &dAfter_X, &dAfter_Y, true ); // IPC�p�ϊ�
			}else { // 3�p�����[�^
				m_cCrd.TOKYOtoJGD2000 ( dBefore_X,  dBefore_Y, 0, &dAfter_X, &dAfter_Y, &dAfter_He );
			}

			a_ipPt->put_X ( dAfter_X );
			a_ipPt->put_Y ( dAfter_Y );

		}catch (...) {
			return NULL;
		}
	}else {

	//	IGeometryBagPtr	a_ipNewGeom(CLSID_GeometryBag);
		try {
			// �������|���S��������̂ŁA�����O���l����������������

			IGeometryCollectionPtr	a_ipGeoColl(c_ipGeom);
		//	IGeometryCollectionPtr	a_ipNewGeoColl(a_ipNewGeom);
			long	a_ipGeomCount = 0;
			a_ipGeoColl->get_GeometryCount ( &a_ipGeomCount );
			for( long count = 0; count < a_ipGeomCount; ++count )
			{
				IGeometryPtr	a_ipCurGeom;
				a_ipGeoColl->get_Geometry ( count, &a_ipCurGeom ); // �W�I���g����

				IPointCollectionPtr	a_ipPts(a_ipCurGeom);

				long	a_lPtsCount = 0;
				a_ipPts->get_PointCount ( &a_lPtsCount );

				WKSPoint *pWKSPoint = new WKSPoint[a_lPtsCount];
				a_ipPts->QueryWKSPoints(0, a_lPtsCount, pWKSPoint);

				// ���n�n�ϊ�����
				for ( long i = 0; i < a_lPtsCount; ++i )
				{
					double dAfter_He = 0, dAfter_X = 0, dAfter_Y = 0;
					if( m_lExe_Flag & ON_RPARAM ) {
						m_cCrd.TOKYOtoJGD2000_RP ( pWKSPoint[i].X,  pWKSPoint[i].Y, &dAfter_X, &dAfter_Y, true ); // IPC�p�ϊ�
						pWKSPoint[i].X = dAfter_X;
						pWKSPoint[i].Y = dAfter_Y;
					}else { // 3�p�����[�^
						m_cCrd.TOKYOtoJGD2000 ( pWKSPoint[i].X,  pWKSPoint[i].Y, 0, &dAfter_X, &dAfter_Y, &dAfter_He );
						pWKSPoint[i].X = dAfter_X;
						pWKSPoint[i].Y = dAfter_Y;
					}
				}
				a_ipPts->SetWKSPoints( a_lPtsCount, pWKSPoint );

			//	a_ipNewGeoColl->AddGeometry ( a_ipCurGeom );
				delete [] pWKSPoint;
			}
		//	a_ipNewGeom->putref_SpatialReference ( a_ipSpRef );
		//	a_ipNewGeom->SnapToSpatialReference ();
		} catch (...) {
			return NULL;
		}
	}
	return c_ipGeom;
//	return a_ipNewGeom;
}

// �ϊ����s��
bool main_routine::
update_exec	( void )
{
	// �J�n���Ԃ̐ݒ�
	m_eExeTime.Set_Start();

	MakeOBJECTIDList ( m_ipFeatureClass_1, m_sOBJECTID_List );

	// �폜���[�h�̏ꍇ�̂�
	if( m_lExe_Flag & ON_UPDATEMODE ) {
		// �G�f�B�b�g�J�n
		if( !StartEdit () ) {
			PrnMsg ( m_fErr_Fp, stderr, "ERROR:StartEdit�����Ɏ��s���܂���\n" );
			return false;
		}
	}

	if( FAILED ( m_ipFeatureClass_1->get_ShapeType ( &m_ipGeom_Type ) ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:�W�I���g���^�C�v�̎擾�Ɏ��s���܂���\n" );
		return false;	
	}

	PrnMsg( m_fErr_Fp, stderr, "#�����I�u�W�F�N�g�� %d\n", m_sOBJECTID_List.size() );

	long a_lCount = 0;
	std::set<long>::const_iterator a_iIter;
	for( a_iIter = m_sOBJECTID_List.begin(); a_iIter != m_sOBJECTID_List.end(); ++a_iIter, ++a_lCount )
	{
		if( a_lCount % 100 == 0 ) {
			PrnMsg( stdout, NULL, "%d/%d��������\r", a_lCount, m_sOBJECTID_List.size() );			
		}
		long a_lCur_OBJECTID = *a_iIter;

		try {

			// OBJECTID�őΏۃI�u�W�F�N�g���擾
			IFeaturePtr a_ipSrcFeature;
			m_ipFeatureClass_1->GetFeature(a_lCur_OBJECTID, &a_ipSrcFeature);

			IGeometryPtr a_ipGeom;
			if( FAILED ( a_ipSrcFeature->get_Shape ( &a_ipGeom ) ) ) {
				PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\t�`��擾���s\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
				continue;
			}

			IGeometryPtr a_ipNewGeom = Make_JGD_Geometry ( a_ipGeom );
			if( a_ipNewGeom == NULL ) {
				PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\t�`��쐬���s\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
				continue;				
			}

			HRESULT hr;
			hr = a_ipSrcFeature->putref_Shape ( a_ipNewGeom );
			if( FAILED ( hr ) ) {
				PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\tputref_Shape()�Ɏ��s\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
			}

			// �f�[�^�X�V
			if( m_lExe_Flag & ON_UPDATEMODE ) {
				if( FAILED ( a_ipSrcFeature->Store () ) ) {
					PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\tStore()�Ɏ��s\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
				}
			}

			double a_dX = 0.0, a_dY = 0.0;
			GetPoint ( a_ipGeom, a_dX, a_dY );
		//	PrnMsg( m_fErr_Fp, NULL, "0\t%s\t%d\t%0.10f\t%0.10f\tNOTICE\t\t���W�ϊ�\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID, a_dX, a_dY );
		}catch (...) {
			PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\t��O����\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
			continue;
		}
	}
	PrnMsg( stdout, NULL, "\n" );			

	// �폜���[�h�̏ꍇ�̂�
	if( m_lExe_Flag & ON_UPDATEMODE ) {
		// �G�f�B�b�g�I��
		if( !StopEdit () ) {
			PrnMsg ( m_fErr_Fp, stderr, "ERROR:StopEdit�����Ɏ��s���܂���\n" );
			return false;
		}
	}

	//�@���s���I��
	// �I�����Ԃ̎擾
	PrnMsg	( m_fErr_Fp, stderr, "#�f�[�^�ϊ������� = %0.2f(s)�ł���\n",
						(double)((m_eExeTime.Get_From_Start()) / 1000.0) );
	return	( true );
}

// ��������w�肵�A�A�b�v�f�[�g�p�̃J�[�\��������ė���
bool	main_routine::
Update_Where ( const IFeatureClassPtr&  c_ipTable,    ///< [in] �������e�[�u��
               CString&                 c_sWhere,    ///< [in] �w��WHERE��
               int                      c_iRetType,  ///< [in] ����Ԃ���
               long&                    c_lCount,    ///< [out] �J�E���g
               IFeatureCursorPtr&       c_ipCursor ) ///< [out] �J�[�\��
{
	HRESULT			hr;

	// ������
	c_ipCursor = NULL;
	
	IQueryFilterPtr		ipQ_Filter ( CLSID_QueryFilter );
	
	hr	= ipQ_Filter->put_WhereClause ( CComBSTR(c_sWhere) );
	if ( FAILED ( hr ) ) {
		PrnMsg ( m_fErr_Fp, NULL, _T("%s�����pWHERE��̐ݒ肪�ł��Ȃ�\t<%s>\n"), ERR_STAT, c_sWhere );
		return false;
	}

	// �f�[�^�J�E���g�����
	if( c_iRetType & RET_COUNT ) {
		//---------------------------------------------------------
		long lData_Count = 0;
		hr	= c_ipTable->FeatureCount ( ipQ_Filter, &lData_Count );
		if ( FAILED ( hr ) )	{
			PrnMsg ( m_fErr_Fp, NULL, _T("%s�����f�[�^�����擾�ł��Ȃ�\t<%s>\n"), ERR_STAT, c_sWhere );
			return false;
		}
		c_lCount = lData_Count;
		//---------------------------------------------------------
	}

	// �T�[�`���Ă���
	IFeatureCursorPtr	ipCursor;
	if( c_iRetType & RET_CURSOR ) {
		if( FAILED(c_ipTable->Update( ipQ_Filter, VARIANT_FALSE, &ipCursor )) ) {
			PrnMsg ( m_fErr_Fp, NULL, _T("%s�f�[�^�����Ɏ��s\t<%s>\n"), ERR_STAT, c_sWhere );
			return false;
		}
		c_ipCursor = ipCursor;
	}
	return true;
}


// �ϊ����s��
bool main_routine::
update_exec_nolist ( void )
{
	// �J�n���Ԃ̐ݒ�
	m_eExeTime.Set_Start();

//	MakeOBJECTIDList ( m_ipFeatureClass_1, m_sOBJECTID_List );

	//--------------------------------------------------------------------------------
	long a_lData_Count	= 0;

	IFeatureCursorPtr	a_ipFeCsr;	//	�ԋp�p�t�B�[�`���J�[�\��
	if( !Update_Where( m_ipFeatureClass_1, CString(_T("")), RET_CURSOR|RET_COUNT, a_lData_Count, a_ipFeCsr ) )
		return false;

/*	long a_lCount = 0;
	IFeaturePtr	a_ipFeature;
	while ( a_ipFeCsr->NextFeature ( &a_ipFeature ) == S_OK )
	{
		if( a_lCount % 100 == 0 ) {
			PrnMsg( stdout, NULL, "%d/%d��������\r", a_lCount, a_lData_Count );			
		}
		long a_lOID = 0;
		a_ipFeature->get_OID ( &a_lOID );
		c_sOBJECTID_List.insert ( a_lOID );
		++a_lCount;
	}*/
	//--------------------------------------------------------------------------------

	// �폜���[�h�̏ꍇ�̂�
	if( m_lExe_Flag & ON_UPDATEMODE ) {
		// �G�f�B�b�g�J�n
		if( !StartEdit () ) {
			PrnMsg ( m_fErr_Fp, stderr, "ERROR:StartEdit�����Ɏ��s���܂���\n" );
			return false;
		}
	}

	if( FAILED ( m_ipFeatureClass_1->get_ShapeType ( &m_ipGeom_Type ) ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:�W�I���g���^�C�v�̎擾�Ɏ��s���܂���\n" );
		return false;	
	}

	PrnMsg( m_fErr_Fp, stderr, "#�����I�u�W�F�N�g�� %d\n", a_lData_Count );

	long a_lCount = 0;
//	std::set<long>::const_iterator a_iIter;
//	for( a_iIter = m_sOBJECTID_List.begin(); a_iIter != m_sOBJECTID_List.end(); ++a_iIter, ++a_lCount )
//	{
	IFeaturePtr a_ipSrcFeature;
	while ( a_ipFeCsr->NextFeature ( &a_ipSrcFeature ) == S_OK )
	{
		if( a_lCount % 100 == 0 ) {
			PrnMsg( stdout, NULL, "%d/%d��������\r", a_lCount, a_lData_Count );			
		}
		++a_lCount; // �����ł���Ă����ɖ��Ȃ�

		long a_lCur_OBJECTID = 0;
		if( FAILED (a_ipSrcFeature->get_OID ( &a_lCur_OBJECTID ) ) )
			return false;

		try {

			// OBJECTID�őΏۃI�u�W�F�N�g���擾
		//	m_ipFeatureClass_1->GetFeature(a_lCur_OBJECTID, &a_ipSrcFeature);

			IGeometryPtr a_ipGeom;
			if( FAILED ( a_ipSrcFeature->get_Shape ( &a_ipGeom ) ) ) {
				PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\t�`��擾���s\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
				continue;
			}

			IGeometryPtr a_ipNewGeom = Make_JGD_Geometry ( a_ipGeom );
			if( a_ipNewGeom == NULL ) {
				PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\t�`��쐬���s\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
				continue;				
			}

			HRESULT hr;
			hr = a_ipSrcFeature->putref_Shape ( a_ipNewGeom );
			if( FAILED ( hr ) ) {
				PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\tputref_Shape()�Ɏ��s\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
			}

			// �f�[�^�X�V
			if( m_lExe_Flag & ON_UPDATEMODE ) {
				if( FAILED ( a_ipSrcFeature->Store() ) ) {
					PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\tStore()�Ɏ��s\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
				}
			}

		//	double a_dX = 0.0, a_dY = 0.0;
		//	GetPoint ( a_ipGeom, a_dX, a_dY );
		//	PrnMsg( m_fErr_Fp, NULL, "0\t%s\t%d\t%0.10f\t%0.10f\tNOTICE\t\t���W�ϊ�\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID, a_dX, a_dY );
		}catch (...) {
			PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\t��O����\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
			continue;
		}
	}
	PrnMsg( stdout, NULL, "\n" );			

	// �폜���[�h�̏ꍇ�̂�
	if( m_lExe_Flag & ON_UPDATEMODE ) {
		// �G�f�B�b�g�I��
		if( !StopEdit () ) {
			PrnMsg ( m_fErr_Fp, stderr, "ERROR:StopEdit�����Ɏ��s���܂���\n" );
			return false;
		}
	}

	//�@���s���I��
	// �I�����Ԃ̎擾
	PrnMsg	( m_fErr_Fp, stderr, "#�f�[�^�ϊ������� = %0.2f(s)�ł���\n",
						(double)((m_eExeTime.Get_From_Start()) / 1000.0) );
	return	( true );
}
