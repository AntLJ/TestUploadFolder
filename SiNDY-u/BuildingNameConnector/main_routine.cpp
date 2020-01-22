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
#include <conio.h>

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

//--------------------------------------------------------------------------------
/**
* @brief �G���[���b�Z�[�W
*/
//--------------------------------------------------------------------------------
void	PrnMsg	( FILE* c_cpFp, FILE* c_cpFp2, const char* c_cpFormat, ... ) {
							
	va_list	a_vArgList = NULL;

	va_start(a_vArgList, c_cpFormat );	// ������

	vfprintf_s ( c_cpFp, c_cpFormat, a_vArgList );
	fflush	( c_cpFp );
	if( c_cpFp2 != NULL && c_cpFp != c_cpFp2 ) {
		vfprintf_s ( c_cpFp2, c_cpFormat, a_vArgList );
		fflush	( c_cpFp2 );					
	}
}

// DB�ւ̐ڑ��I�[�v��
bool	main_routine::
DB_Open ( void )
{
	// �����p���[�N�X�y�[�X
	m_ipWSpace_Building = sindy::create_workspace(CString(m_sDB_Building.c_str()));
	if(!m_ipWSpace_Building ){
		PrnMsg( m_fErr_Fp, stderr, "ERROR:DB���I�[�v���ł��Ȃ� : <%s>\n", m_sDB_Building.c_str() );
		return false;
	}
	// ���b�V���p���[�N�X�y�[�X
	m_ipWSpace_Mesh = sindy::create_workspace(CString(m_sDB_Mesh.c_str()));
	if(!m_ipWSpace_Mesh ){
		PrnMsg( m_fErr_Fp, stderr, "ERROR:DB���I�[�v���ł��Ȃ� : <%s>\n", m_sDB_Mesh.c_str() );
		return false;
	}
	// POI�p���[�N�X�y�[�X
	m_ipWSpace_POI = sindy::create_workspace(CString(m_sDB_POI.c_str()));
	if(!m_ipWSpace_POI ){
		PrnMsg( m_fErr_Fp, stderr, "ERROR:DB���I�[�v���ł��Ȃ� : <%s>\n", m_sDB_POI.c_str() );
		return false;
	}
	// �s���Y���p���[�N�X�y�[�X
	m_ipWSpace_RealEstate = sindy::create_workspace(CString(m_sDB_RealEstate.c_str()));
	if(!m_ipWSpace_RealEstate ){
		PrnMsg( m_fErr_Fp, stderr, "ERROR:DB���I�[�v���ł��Ȃ� : <%s>\n", m_sDB_RealEstate.c_str() );
		return false;
	}

	// �����p�t�B�[�`���N���X�̃I�[�v��
	if( FAILED (IFeatureWorkspacePtr(m_ipWSpace_Building)->OpenFeatureClass ( _bstr_t ( m_sFC_Building.c_str() ), &m_ipFC_Building ) ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:�t�B�[�`���N���X���I�[�v���ł��܂���ł��� : <%s>\n", m_sFC_Building.c_str() ); 
		return false;
	}
	// ���b�V���p�t�B�[�`���N���X�̃I�[�v��
	if( FAILED (IFeatureWorkspacePtr(m_ipWSpace_Mesh)->OpenFeatureClass ( _bstr_t ( m_sFC_Mesh.c_str() ), &m_ipFC_Mesh ) ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:�t�B�[�`���N���X���I�[�v���ł��܂���ł��� : <%s>\n", m_sFC_Mesh.c_str() ); 
		return false;
	}
	// POI�p�t�B�[�`���N���X�̃I�[�v��
	if( FAILED (IFeatureWorkspacePtr(m_ipWSpace_POI)->OpenFeatureClass ( _bstr_t ( m_sFC_POI.c_str() ), &m_ipFC_POI ) ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:�t�B�[�`���N���X���I�[�v���ł��܂���ł��� : <%s>\n", m_sFC_POI.c_str() ); 
		return false;
	}
	// �s���Y���p�t�B�[�`���N���X�̃I�[�v��
	if( FAILED (IFeatureWorkspacePtr(m_ipWSpace_RealEstate)->OpenFeatureClass ( _bstr_t ( m_sFC_RealEstate.c_str() ), &m_ipFC_RealEstate ) ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:�t�B�[�`���N���X���I�[�v���ł��܂���ł��� : <%s>\n", m_sFC_RealEstate.c_str() ); 
		return false;
	}
	return true;
}

//	���O�t�@�C�����I�[�v������
bool	main_routine::
Open_Log ( std::string& csFName,
           FILE**       cFp )
{
	if( m_sLog_File == "" ) {
		return	true;
	}else {
		*cFp = fopen ( csFName.c_str(), "w" );
		if( *cFp == NULL ) {
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
		"=== �y�������́E�����|���S���R�t���c�[���z(v%s) ===\n"
				"<usage>\n"
				"%s <�I�v�V����>... \n"
/*				"\t      '--db1' : ���폜�����Ώ�DB�ڑ�1(ex. --db1 SINDY2012B/SINDY2012B/SDE.SNP_delete_130220/5151/ruby2 )\n"
				"\t      '--fc1' : ���폜�����Ώۃt�B�[�`���N���X1(ex. --fc1 SINDY2012B.Building )\n"
				"\t      '--db2' : �@�폜�����Ώ�DB�ڑ�(ex. --db1 SINDY2012B/SINDY2012B/SDE.SNP_delete_130220/5151/ruby2 )\n"
				"\t      '--fc2' : �@�폜�����Ώۃt�B�[�`���N���X(ex. --fc1 SINDY2012B.Building_Step )\n"
				"\t      '--rel' : �@�t�B�[�`���N���X�P�ɑ΂���Q�̊֌W�w��(ex. --rel contains).\n"
				"\t                     <�w��\�Ȋ֌W>\n"
				"\t                        contains   fc1�̃f�[�^��fc2������i--rel���w�莞�̃f�t�H���g�j\n"
				"\t                        overlaps   fc1�̃f�[�^��fc2���d���ifc1��fc2���������j\n"
				"\t                        within     fc1�̃f�[�^��fc2������\n"
				"\t                        touches    fc1�̃f�[�^��fc2���ڐG\n"
				"\t                        equals     fc1�̃f�[�^��fc2�������iArc���Ɣ����Ȍ덷�����e��������ɂȂ�͂��j\n"
				"\t                        crosses    fc1�̃f�[�^��fc2�������ifc1��fc2���ʎ����j\n"
				"\t      '--oid' : ���I�u�W�F�N�gID���X�g���w��(ex. --oid .\\test\\objectid.txt)\n"
				"\t      '--l    : �@���O�t�@�C�������w��(ex. -l .\\test\\erace.log)\n"
				"\t      '--del' : �폜���[�h�w��i���w�莞�̓`�F�b�N���[�h�j\n"*/
				"\t      �����󂪂��Ă�����͕̂K�{�I�v�V�����B\n"
				"\t      \n", m_sVersion, argv[0] );
}


using boost::program_options::options_description;
using boost::program_options::value;
using boost::program_options::variables_map;
using boost::program_options::store;
using boost::program_options::parse_command_line;
using boost::program_options::notify;

// ���s���\��
void main_routine::
PrintExecInfo( void ) {
	PrnMsg( m_fErr_Fp, stderr, "#SINDYSTDLOG\n");
	PrnMsg( m_fErr_Fp, stderr, "#�c�[���o�[�W����               : <%s>\n", m_sVersion );
	PrnMsg( m_fErr_Fp, stderr, "#�����̐ڑ�DB                   : <%s>\n", m_sDB_Building.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#�����̃t�B�[�`���N���X��       : <%s>\n", m_sFC_Building.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#���b�V���̐ڑ�DB               : <%s>\n", m_sDB_Mesh.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#���b�V���̃t�B�[�`���N���X��   : <%s>\n", m_sFC_Mesh.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#POI�E���L�̐ڑ�DB              : <%s>\n", m_sDB_POI.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#POI�E���L�̃t�B�[�`���N���X��  : <%s>\n", m_sFC_POI.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#�s���Y�E�h�ΑΏە����̐ڑ�DB : <%s>\n", m_sDB_RealEstate.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#�s���Y�E�h�ΑΏە����̃t�B�[�`���N���X�� : <%s>\n", m_sFC_RealEstate.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#���b�V�����X�g                 : <%s>\n", m_sMeshList.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#POI�E���L�f�[�^�D��x�ݒ胊�X�g: <%s>\n", m_sPriorityList.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#�o��Shape�t�@�C����            : <%s>\n", m_sShapeFName.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#�G���[���O�t�@�C��             : <%s>\n", m_sLog_File.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#���s���O�t�@�C��               : <%s>\n", m_sExeLog_File.c_str() );

}

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
	// �y�K�{�̃I�v�V�����z
		("blddb", value<std::string>(&m_sDB_Building), "�����̐ڑ�DB")
		("bldfc", value<std::string>(&m_sFC_Building), "�����̃t�B�[�`���N���X��")
		("mlist", value<std::string>(&m_sMeshList), "���b�V�����X�g")
		("prlist", value<std::string>(&m_sPriorityList),"POI�E���L�f�[�^�D��x�ݒ胊�X�g")
		("outshp", value<std::string>(&m_sShapeFName),"�o��Shape�t�@�C����")
		("log", value<std::string>(&m_sLog_File),"�G���[���O�t�@�C��")
		("exelog", value<std::string>(&m_sExeLog_File),"���s���O�t�@�C��")
	// �y�C�ӂ̃I�v�V�����z
		("meshdb", value<std::string>(&m_sDB_Mesh), "���b�V���̐ڑ�DB")
		("meshfc", value<std::string>(&m_sFC_Mesh), "���b�V���̃t�B�[�`���N���X��")
		("poidb", value<std::string>(&m_sDB_POI), "POI�E���L�̐ڑ�DB")
		("poifc", value<std::string>(&m_sFC_POI), "POI�E���L�̃t�B�[�`���N���X��")
		("redb", value<std::string>(&m_sDB_RealEstate), "�s���Y���̐ڑ�DB")
		("refc", value<std::string>(&m_sFC_RealEstate), "�s���Y���̃t�B�[�`���N���X��")
		("fpdb", value<std::string>(&m_sDB_FireProof), "�h�ΑΏە����̐ڑ�DB")
		("fpfc", value<std::string>(&m_sFC_FireProof), "�h�ΑΏە����̃t�B�[�`���N���X��");

	variables_map vArgmap;

	try {
		store(parse_command_line(argc, argv, oOption), vArgmap);
		notify(vArgmap);

		// �ڑ�DB1�w�肪�����ꍇ�͏I��
		if ( !vArgmap.count("blddb") || !vArgmap.count("bldfc") || !vArgmap.count("mlist") ||
			 !vArgmap.count("prlist") || !vArgmap.count("outshp") || !vArgmap.count("log") || !vArgmap.count("exelog")) {
			PrnMsg ( m_fErr_Fp, stderr, "ERROR:�I�v�V�����w����m�F���Ă�������\n");
			return false;
		}

		// �f�t�H���g�ł͌���DB�̐ڑ����g�p����̂ł��̐ݒ���s��
		if( !vArgmap.count("meshdb") ) { m_sDB_Mesh = m_sDB_Building; }
		if( !vArgmap.count("meshfc") ) { m_sFC_Mesh = _T("REFERENCE.CITYMESH"); } // �f�t�H���g�͓s�s�n�}���b�V��
		if( !vArgmap.count("poidb") )  { m_sDB_POI = m_sDB_Building; }
		if( !vArgmap.count("poifc") )  { m_sFC_POI = m_sFC_Building; }
		if( !vArgmap.count("redb") ) { m_sDB_RealEstate = m_sDB_Building; }
		if( !vArgmap.count("refc") ) { m_sFC_RealEstate = m_sFC_Building; }
		if( !vArgmap.count("fpdb") ) { m_sDB_FireProof = m_sDB_Building; }
		if( !vArgmap.count("fpfc") ) { m_sFC_FireProof = m_sFC_Building; }

		// ���s�t���O����
	//	if( vArgmap.count("l") ) { m_lExe_Flag |= ON_LOGFILE; }
	}catch (...) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:�I�v�V�����̎w��ɊԈႢ������܂�\n");
		return false;
	}

	// ���s���O�t�@�C���̃I�[�v��
	if( !Open_Log (m_sExeLog_File, &m_fExe_Fp) ) {
		PrnMsg ( stderr, NULL, "ERROR:���s���O�t�@�C�����I�[�v���ł��Ȃ� : <%s>\n", m_sExeLog_File.c_str() );
		return	false;
	}

	// �G���[���O�t�@�C���̃I�[�v��
	if( !Open_Log (m_sLog_File, &m_fErr_Fp) ) {
		PrnMsg ( stderr, NULL, "ERROR:�G���[���O�t�@�C�����I�[�v���ł��Ȃ� : <%s>\n", m_sLog_File.c_str() );
		return	false;
	}
	// Shape�t�@�C���̃I�[�v��
	if( !m_mShpFile.Init ( m_sShapeFName, esriGeometryPoint ) ) {
		PrnMsg ( stderr, NULL, "ERROR:Shape�t�@�C�����I�[�v���ł��Ȃ� : <%s>\n", m_sShapeFName.c_str() );
		return	false;
	}

	//-----------------------------------------------------------------------------------------------
	// ���s�����o��
	PrintExecInfo();

	// �J�n���Ԃ̐ݒ�
	m_eExeTime.Set_Start();

	//--------------------------------------------------------------------------
	// ���b�V�����X�g�̓ǂݍ���
	//--------------------------------------------------------------------------
	if( !Read_List ( m_sMeshList, m_sMESH_List ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:���b�V�����X�g�̓ǂݍ��݂Ɏ��s : <%s>\n", m_sMeshList.c_str() );
		return false;
	}

	//--------------------------------------------------------------------------
	// �D��x���X�g�̓ǂݍ���
	//--------------------------------------------------------------------------
	if( !m_pPriorityMgr.Init ( m_sPriorityList ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:�D��x���X�g�̓ǂݍ��݂Ɏ��s : <%s>\n", m_sPriorityList.c_str() );
		return false;
	}

	//--------------------------------------------------------------------------
	// DB�ڑ��֘A�A�t�B�[�`���N���X�̃I�[�v������
	//--------------------------------------------------------------------------
	if( !DB_Open () ) {
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
	���b�V����`��GeoDB����擾���� \
	��GeoDB�ւ̐ڑ����m������Ă��邱�Ƃ��O��
--------------------------------------------------------------------------------*/
IGeometryPtr	main_routine::
GetMeshRect ( int c_iMeshCode ) // ���b�V���R�[�h
{
/*
	_bstr_t		a_bsWhere_Str;
	char		a_tmpstr[30];	//�@�����͂Ȃ�Ƃ��������C������
*/
	int			a_iDigit = 0;
	a_iDigit	= m_cCrd.GetMeshDigit( c_iMeshCode );

	if( a_iDigit != 2 && a_iDigit != 4 && a_iDigit != 6 && a_iDigit != 8 ) {
		return	( NULL );
	}
	CString csWhereStr; // ������

	csWhereStr.Format ("MESHCODE = %.2d", c_iMeshCode );

	// �����p�t�B���^�ݒ�
//	sprintf(a_tmpstr, "MESHCODE = %.2d", c_iMeshCode);		// ���b�V���R�[�h�𕶎����
//	a_bsWhere_Str	= a_tmpstr;		// string��BSTR�ɕϊ�
	
	IQueryFilterPtr		a_Sp_Filter	( CLSID_QueryFilter );	// ��Ԍ����t�B���^
	a_Sp_Filter->put_WhereClause	( _bstr_t(csWhereStr) );// Where��̐ݒ�
	
	// �T�[�`���Ă���
	IFeatureCursorPtr	a_ipFCursor;
	if( FAILED (m_ipFC_Mesh->Search ( a_Sp_Filter, VARIANT_FALSE, &a_ipFCursor )) ) {
		PrnMsg (m_fErr_Fp, NULL, "%d\t�f�[�^�̑��������Ɏ��s\n", c_iMeshCode);
		return NULL;
	}else {
		IFeaturePtr				a_ipFeature;
		while ( a_ipFCursor->NextFeature ( &a_ipFeature ) == S_OK )
		{
			IGeometryPtr		a_ipClip_Shp;

			if( FAILED(a_ipFeature->get_ShapeCopy ( &a_ipClip_Shp )) ) {	// �`��擾
				PrnMsg( m_fErr_Fp, NULL, "%d\t�V�F�C�v�̃R�s�[�Ɏ��s\n", c_iMeshCode);
				return NULL;
			}
			VARIANT_BOOL	a_bResult;	
			a_ipClip_Shp->get_IsEmpty( &a_bResult );
			if(a_bResult == VARIANT_TRUE) {	// �N���b�v���ʂ�NULL�Ȃ烁�b�V���O
				PrnMsg( m_fErr_Fp, NULL, "%d\t���b�V����`����\n", c_iMeshCode);
				return NULL;
			}
			return	( a_ipClip_Shp );
		}
	}
	return	NULL;
}

/** ------------------------------------------------------------------------------
	�t�B�[�`�����o�͗p�̃t�B�[�`���ɂ���
	@return	�f�[�^���i���s��-1�j
--------------------------------------------------------------------------------*/
int main_routine::
MakeDataContainer ( long                     cMeshCode,       //!< [in] �����b�V���R�[�h
                    const IFeatureCursorPtr& c_ipFCursor,     //!< [IN] �J�[�\��
                    mIFeatureContainer&      c_mWork_Data,    //!< [IN] �o�͗p�f�[�^
                    bool                     bLeftDownPtMode )//!< [in] ���XY�̏������|�C���g�����Y���b�V���Ɋ܂܂��ꍇ�̂݊i�[����ꍇ
{
	int	a_iCount	= 0;

	long lMeshLevel = 0;
	if( 1000 <= cMeshCode && cMeshCode < 10000 ) {
		lMeshLevel = 1;
	}else if( 100000 <= cMeshCode && cMeshCode < 1000000 ) {
		lMeshLevel = 2;
	}else if( 10000000 <= cMeshCode && cMeshCode < 100000000 ) {
		lMeshLevel = 64;
	}else {
		return -1; // �G���[����
	}

	// �f�[�^vector�R���e�i�ɗ��Ƃ�
	if( c_ipFCursor != NULL ) {

		IFeaturePtr		a_ipCur_Feat;
		while ( c_ipFCursor->NextFeature ( &a_ipCur_Feat ) == S_OK )
		{
			long	a_lOID = 0;
			if( FAILED ( a_ipCur_Feat->get_OID ( &a_lOID ) ) ) {return false;}

			IGeometryPtr	a_ipWorkGeo;	//!< ��Ɨp
			if( FAILED ( a_ipCur_Feat->get_ShapeCopy (&a_ipWorkGeo) )) {return false;}

			std::set<XYPt> sPts;

			if( bLeftDownPtMode == true ) {
				IPointCollectionPtr ipPts(a_ipWorkGeo);
				long lPtCount = 0;
				if( FAILED ( ipPts->get_PointCount(&lPtCount) ) ) {return false;}
				WKSPoint* wTmpPts = new WKSPoint[lPtCount];
				if( FAILED ( ipPts->QueryWKSPoints(0, lPtCount, wTmpPts) ) ) {
					delete wTmpPts;
					return false;
				}
				for( int i = 0; i < lPtCount; ++i )
				{
					XYPt xTmp;
					xTmp.mX = wTmpPts[i].X;
					xTmp.mY = wTmpPts[i].Y;
					sPts.insert ( xTmp );
				}
				delete wTmpPts;

				// ���XY���������_�����b�V�������̔���
				int iTmpMeshCode = 0, tX = 0, tY = 0;
				m_cCrd.LLtoMesh ( sPts.begin()->mX, sPts.begin()->mY, lMeshLevel, &iTmpMeshCode, &tX, &tY, 1 ); // 1000000�s����
				if( cMeshCode != iTmpMeshCode ) {
#ifdef _DEBUG
					PrnMsg( m_fErr_Fp, NULL, "0\t%s\t%d\t\t\tINFO\t\t���̃��b�V���ł͑ΏۊO\t��%d\n",
						m_sFC_Building.c_str(), a_lOID, iTmpMeshCode );
#endif
					continue; // ���Y���b�V���ł͔�΂�
				}
			}

			c_mWork_Data.insert ( mIFeatureContainer::value_type ( a_lOID, a_ipWorkGeo) );
			a_iCount++;
		}
	}else {
		a_iCount = -1;
	}
	return	a_iCount;
}

// �|�C���g���̃R���e�i���쐬����
int main_routine::
MakePointContainer ( const IFeatureCursorPtr& cipFCursor,    //!< [in] �t�B�[�`���J�[�\��
                     CString&                 cNameFidldName,//!< [in] ���̃t�B�[���h���w��
                     mPointInfo_XY&           cmWork_Data,   //!< [out] ��Ɨp�f�[�^
                     long                     clDataType )   //!< [in] �f�[�^�^�C�v(PriorityManager�Œ�`�̂���)
{
	int	a_iCount	= 0;

	IFieldsPtr ipFields;
	if( FAILED ( cipFCursor->get_Fields ( &ipFields ) ) ) {
		return -1;
	}
	long lFidldIndex = -1;
	// ���̂̃t�B�[���h�������Ă���
	if( FAILED ( ipFields->FindField ( _bstr_t(cNameFidldName), &lFidldIndex ) ) ) {
		return -1;
	}

	// POI���[�h�̏ꍇ�́A�R���e���c�R�[�h���擾����̂Ńt�B�[���h�擾�B
	long lContentsCodeIndex = -1;
	// �i����ȊO�͌Œ�ŃR���e���c�R�[�h��-1�ɐݒ�j
	if( clDataType == PriorityManager::TYPE_POI ) {
		if( FAILED ( ipFields->FindField ( _bstr_t("CONTENTS_CODE"), &lContentsCodeIndex ) ) ) {
			return -1;
		}
	}

	// POI���[�h�ȊO�̏ꍇ�́A�f�[�^�^�C�v�̔��ʃt�B�[���h���擾����
	long lSourceTypeIndex = -1;
	if( clDataType == -1 ) {
		if( FAILED ( ipFields->FindField ( _bstr_t("SOURCETYPE_C"), &lSourceTypeIndex ) ) ) {
			return -1;
		}
	}

	// �f�[�^vector�R���e�i�ɗ��Ƃ�
	if( cipFCursor != NULL ) {

		IFeaturePtr		a_ipCur_Feat;
		while ( cipFCursor->NextFeature ( &a_ipCur_Feat ) == S_OK )
		{
			PointInfo_XY pCurInfo; // �|�C���g���
			long	a_lOID = 0;
			if( FAILED (a_ipCur_Feat->get_OID ( &a_lOID ) ) ) {return false;}

			CComVariant cName;
			if( FAILED (a_ipCur_Feat->get_Value(lFidldIndex, &cName ) ) ) {return false;}

			IGeometryPtr	a_ipWorkGeo;	// ��Ɨp
			if( FAILED (a_ipCur_Feat->get_ShapeCopy (&a_ipWorkGeo) ) ) {return false;}

			// POI�̏ꍇ�̓R���e���c�R�[�h���擾����
			long lContents_Code = -1;
			if( clDataType == PriorityManager::TYPE_POI ) {
				CComVariant cContents_Code;
				if( FAILED (a_ipCur_Feat->get_Value ( lContentsCodeIndex, &cContents_Code ) ) ) {return false;}
				lContents_Code = cContents_Code.lVal;
			}
			// POI�ȊO�̏ꍇ�́A�\�[�X�^�C�v���擾����
			long lSourceType = PriorityManager::TYPE_POI;
			if( clDataType == -1 ) {
				CComVariant cSourceType;
				if( FAILED (a_ipCur_Feat->get_Value ( lSourceTypeIndex, &cSourceType ) ) ) {return false;}
				switch ( cSourceType.lVal )
				{
				case	1: lSourceType = PriorityManager::TYPE_REALESTATE; break; // �s���Y���
				case	2: lSourceType = PriorityManager::TYPE_FIREPROOF; break;  // �h�ΑΏە����
				default: 
#ifdef _DEBUG
					PrnMsg( m_fErr_Fp, stderr, "���Ή��̃^�C�v\t%d\t%d\n", a_lOID, cSourceType.lVal );
#endif
					continue;
				}
			}
			std::string sAttrName; // ��ʖ���
			long lPriority = m_pPriorityMgr.GetPriority ( lSourceType, lContents_Code, sAttrName );
			if( lPriority == -1 ) {
#ifdef _DEBUG
				PrnMsg( m_fErr_Fp, stderr, "�D��x���擾�ł��Ȃ�\t%d\t%d\n", clDataType, lContents_Code );
#endif
				continue;
			}
			// �\�[�g�p�R���e�i�Ɋi�[
			pCurInfo.mlDataType = lSourceType; // �f�[�^�^�C�v				
			pCurInfo.mlContentsCode = lContents_Code;
			pCurInfo.mlPriority = lPriority;  // �D��x
			pCurInfo.mlOID      = a_lOID;     // ID
			pCurInfo.mcsName    = cName.bstrVal;// ����
			pCurInfo.mipGeom    = a_ipWorkGeo;// �W�I���g��

			// POI�ŁA�����񂪁u�k���v�̏ꍇ�̓X�L�b�v���� 2014/03/07 �v�]
			if(  clDataType == PriorityManager::TYPE_POI && pCurInfo.mcsName == _T("�k��") ) {
				continue;
			}


			// XY��ǉ�
			if( FAILED ( IPointPtr(a_ipWorkGeo)->get_X(&pCurInfo.mX)) ){return false;}
			if( FAILED ( IPointPtr(a_ipWorkGeo)->get_Y(&pCurInfo.mY)) ){return false;}

			cmWork_Data.insert ( pCurInfo );
		//	cmWork_Data.insert ( mIFeatureContainer::value_type ( a_lOID, a_ipCur_Feat) );
			++a_iCount;
		}
	}else {
		a_iCount = -1;
	}
	return	a_iCount;
}

/**
* @brief XY�\�[�g�����|�C���g�񂩂�A�|���S���Ɋ܂܂��|�C���g�́A�D��x�\�[�g�̃R���e�i���쐬
* @return	����=true,���s=false
*/
bool   main_routine::
GetInnerPoint ( IGeometryPtr&   cipPolygon,   //!< [in] �|���S���W�I���g��
                mPointInfo_XY&  cmPoint_Data, //!< [in] �|�C���g���R���e�i(XY�\�[�g)
                mPointInfo&     cmOutput )    //!< [out] �|�C���g���R���e�i(�D��x�\�[�g)
{
	// �|���S���̊O�ڋ�`���擾
	IEnvelopePtr ipEnvelope;
	if( FAILED ( cipPolygon->get_Envelope ( &ipEnvelope ) ) ) { return false; }

	// �ŏ��E�ő�ʒu�����擾
	XYPt xyMin, xyMax;
	if( FAILED (ipEnvelope->get_XMin ( &xyMin.mX )) ) { return false; }
	if( FAILED (ipEnvelope->get_YMin ( &xyMin.mY )) ) { return false; }
	if( FAILED (ipEnvelope->get_XMax ( &xyMax.mX )) ) { return false; }
	if( FAILED (ipEnvelope->get_YMax ( &xyMax.mY )) ) { return false; }

	// �����L�[�쐬(Y�����́A�|�C���g�̍ŏ�����ő�܂ł������ΏۂƂ������̂łO�`�X�O�Łj
	PointInfo_XY pMinKey, pMaxKey;
	pMinKey.mX = xyMin.mX;
	pMinKey.mY = 0;
	pMaxKey.mX = xyMax.mX;
	pMaxKey.mY = 90;

	// ����
	mPointInfo_XY::const_iterator iIterXY_Begin, iIterXY_End; // XY�\�[�g�̍ŏ��ƍŌ�
	iIterXY_Begin = cmPoint_Data.lower_bound ( pMinKey );
	iIterXY_End   = cmPoint_Data.upper_bound ( pMaxKey );

	IRelationalOperatorPtr ipRel = cipPolygon;

	// Y�����Ŕ͈͂��i�����R���e�i���쐬����
	mPointInfo_XY::const_iterator iIterXY = iIterXY_Begin;
	for( iIterXY = iIterXY_Begin; iIterXY != cmPoint_Data.end(); ) // �R���e�i���̂̏I����������Ȃ��Ƃ���
	{
		// X�����͊��Ƀt�B���^�����O���Ă���̂�Y����������OK
		if( iIterXY->mY >= xyMin.mY && iIterXY->mY <= xyMin.mY ) {
			++iIterXY;
			continue;
		}

		PointInfo pAdd = *iIterXY;

		VARIANT_BOOL vRet1, vRet2;
		if( FAILED ( ipRel->Disjoint( pAdd.mipGeom, &vRet1 ) ) ) {
			return false;
		}

		if( vRet1 == VARIANT_FALSE ) {
			if( FAILED ( ipRel->Contains ( pAdd.mipGeom, &vRet2 ) ) ) {
				return false;
			}
			if( vRet2 == VARIANT_TRUE ) { // ���������L���܂ޏꍇ
				cmOutput.insert ( pAdd );
			}
		}
		
		if( iIterXY == iIterXY_End ) { // End�܂ŏ���������I��
			break;
		}
		++iIterXY;
	}
	return true;
}

/**
* @brief �f�[�^�o��
* @note  
* @return	����=true,���s=false
*/
bool   main_routine::
PrintOut( mIFeatureContainer& cmBuilding_Data, //!< [in] �����f�[�^�R���e�i
          mPointInfo_XY&      cmPoint_Data )   //!< [out] �|�C���g���R���e�i
{
	// �����̃��[�v
	mIFeatureContainer::const_iterator iBuidingIter;
	for( iBuidingIter = cmBuilding_Data.begin(); iBuidingIter != cmBuilding_Data.end(); ++iBuidingIter )
	{
		IGeometryPtr ipCurBuilding = (*iBuidingIter).second;

		// �������ł̗D��x����p�R���e�i
		mPointInfo mInnerPoints;

		// �|���S���ɓ�����_���擾
		if( !GetInnerPoint ( ipCurBuilding, cmPoint_Data, mInnerPoints ) ) {
			return false;
		}

		// �܂܂��_��������Ύ��̉ƌ`�ցI�I
		if( mInnerPoints.size() == 0 ) continue;

		// �����܂łɗD��x���ɂȂ�񂾃f�[�^���쐬�����̂ŁA�o�͂���
		mPointInfo::const_iterator iOutputIter;
		long lPtCount = 0;
		for( iOutputIter = mInnerPoints.begin(); iOutputIter != mInnerPoints.end(); ++iOutputIter, ++lPtCount )
		{
			if( lPtCount == 0 ) { // �ŗD��̃|�C���g�����o�͂���
				// �`��̏o��
				m_mShpFile.OuttoShapeFile ( IGeometryPtr((*iOutputIter).mipGeom), (*iOutputIter).mlOID, (*iBuidingIter).first, CString((*iOutputIter).mcsName) );
			}

			IPointPtr ipPt = (*iOutputIter).mipGeom;
			double dX = 0, dY = 0;
			if( FAILED ( ipPt->get_X ( &dX )) ) {return false;}
			if( FAILED ( ipPt->get_Y ( &dY )) ) {return false;}

			string sType;
			switch( (*iOutputIter).mlDataType )
			{
			case	PriorityManager::TYPE_POI:       sType = "POI"; break;
			case	PriorityManager::TYPE_REALESTATE:sType = "�s���Y"; break;
			case	PriorityManager::TYPE_FIREPROOF: sType = "�h�Ύ{��"; break;
			default: return false;
			}

			PrnMsg( m_fExe_Fp, NULL, "0\t%s\t%d\t%0.10f\t%0.10f\tINFO\t\t%s\t<%s>\t%d\t%d\t%s\n",
				m_sFC_Building.c_str(), (*iBuidingIter).first, dX, dY, lPtCount==0?"�ƌ`���̃|�C���g�i��\�j":"�ƌ`���̃|�C���g",
				sType.c_str(), (*iOutputIter).mlContentsCode, (*iOutputIter).mlOID, (*iOutputIter).mcsName );
		}
	}
	return true;
}

// �����`�󂩂�O�ڋ�`���쐬����
IGeometryPtr   main_routine::
MakeBuildingsEnvelope ( const mIFeatureContainer& cmWork_Data ) //!< [in] ��Ɨp�f�[�^
{
	ISpatialReferencePtr ipSpRef; // ��ԎQ�Ƃ��擾���Ă���

	double dXMin = 0, dYMin = 0, dXMax = 0, dYMax = 0;

	long lCount = 0;
	mIFeatureContainer::const_iterator iIter;
	for( iIter = cmWork_Data.begin(), lCount = 0; iIter != cmWork_Data.end(); ++iIter )
	{
		IGeometryPtr ipCurGeom = (*iIter).second;

		// �����A�E������o��
		IEnvelopePtr ipCurEnv;
		if( FAILED (ipCurGeom->get_Envelope ( &ipCurEnv ) ) ) {
			return NULL;
		}
		double dXMinTmp = 0, dYMinTmp = 0, dXMaxTmp = 0, dYMaxTmp = 0;
		if( FAILED (ipCurEnv->get_XMin ( &dXMinTmp ) ) ) {return NULL;}
		if( FAILED (ipCurEnv->get_YMin ( &dYMinTmp ) ) ) {return NULL;}
		if( FAILED (ipCurEnv->get_XMax ( &dXMaxTmp ) ) ) {return NULL;}
		if( FAILED (ipCurEnv->get_YMax ( &dYMaxTmp ) ) ) {return NULL;}
		if( lCount == 0 ) {
			dXMin = dXMinTmp;
			dYMin = dYMinTmp;
			dXMax = dXMaxTmp;
			dYMax = dYMaxTmp;
			if( FAILED(ipCurGeom->get_SpatialReference ( &ipSpRef )) ) { return NULL;}
		}else {
			// Min����i�[
			if( dXMin > dXMinTmp ) { dXMin = dXMinTmp; }
			if( dYMin > dYMinTmp ) { dYMin = dYMinTmp; }
			// ����Max
			if( dXMax < dXMaxTmp ) { dXMax = dXMaxTmp; }
			if( dYMax < dYMaxTmp ) { dYMax = dYMaxTmp; }
		}
		++lCount; // �f�[�^������ꍇ�����J�E���g����
	}

	// �|���S���쐬���ĕԋp


	IPolygonPtr          ipRectPoly(CLSID_Polygon); // ��`�|���S��
	IPointCollectionPtr	 pRecPolyPts(ipRectPoly);

	for(int i = 0; i < 5; ++i )
	{
		IPointPtr	ipPt(CLSID_Point);
		switch (i)
		{
		case	0:	ipPt->put_X ( dXMin ); ipPt->put_Y ( dYMin ); break;	// ����
		case	1:	ipPt->put_X ( dXMin ); ipPt->put_Y ( dYMax ); break;	// ����
		case	2:	ipPt->put_X ( dXMax ); ipPt->put_Y ( dYMax ); break;	// �E��
		case	3:	ipPt->put_X ( dXMax ); ipPt->put_Y ( dYMin ); break;	// �E��
		case	4:	ipPt->put_X ( dXMin ); ipPt->put_Y ( dYMin ); break;	// ����
		}
		pRecPolyPts->AddPoint(ipPt);
	}
	
	// ��ԎQ�Ƃ̕t�^�ƃX�i�b�v
	if( FAILED ( ipRectPoly->putref_SpatialReference ( ipSpRef ) ) ) { return NULL; }
	if( FAILED ( ipRectPoly->SnapToSpatialReference ( ) ) ) { return NULL; }

	return (IGeometryPtr(pRecPolyPts));
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

// �ϊ����s��
bool main_routine::
exec	( void )
{
	// �J�n���Ԃ̐ݒ�
	m_eExeTime.Set_Start();

	bool bIsOK = true;

	long a_lCount = 0;
	std::set<long>::const_iterator a_iIter;
	for( a_iIter = m_sMESH_List.begin(); a_iIter != m_sMESH_List.end(); ++a_iIter, ++a_lCount )
	{
		m_eExeTime.Set_Lap_Start();

		long lCurMesh = *a_iIter;

		PrnMsg( stdout, NULL, "#���b�V�� <%d> ������..", lCurMesh );

		// ���b�V����`�̎��o��
		IGeometryPtr	ipMeshRect;
		ipMeshRect = GetMeshRect( lCurMesh );
		if( ipMeshRect == NULL ) {	// ���b�V����`�������ꍇ
			PrnMsg( m_fErr_Fp, NULL, "%d\t�̃��b�V����`��������܂���ł���\n", lCurMesh );
			bIsOK = false;
			continue;
		}
		// ��ԎQ�Ƃ��擾����
		if( m_ipMeshSpRef == NULL ) {
			if( FAILED (ipMeshRect->get_SpatialReference ( &m_ipMeshSpRef )) ) {
				PrnMsg( m_fErr_Fp, stderr, "%d\t��ԎQ�Ƃ̎擾�Ɏ��s\n", lCurMesh );
				bIsOK = false;
				continue;
			}
		}

		// ���b�V����`�Ō����`����擾����
		IFeatureCursorPtr ipBuildingCursor;
		long lBuildingsCount = 0;
		ipBuildingCursor = Search_Feature ( m_ipFC_Building, ipMeshRect, esriSpatialRelIntersects, "", lBuildingsCount );
		if( ipBuildingCursor == NULL ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\t��Ԍ����Ɏ��s(����)\n", lCurMesh );
			bIsOK = false;
			continue;
		}
		// �������R���e�i�Ɋi�[
		mIFeatureContainer mWorkData;
		long lBldCount = MakeDataContainer ( lCurMesh, ipBuildingCursor, mWorkData, true );
		if( lBldCount < 0 ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\t�f�[�^�R���e�i�쐬�Ɏ��s\n", lCurMesh );
			bIsOK = false;
			continue;
		}

		// �����`�󂩂�O�ڋ�`���쐬����
		IGeometryPtr ipBuildingsEnvelope = MakeBuildingsEnvelope ( mWorkData );
		if( ipBuildingsEnvelope == NULL ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\t�����O�ڋ�`�쐬�Ɏ��s\n", lCurMesh );
			bIsOK = false;
			continue;
		}

		// �O�ڋ�`�Ń|�C���g����������
		// POI�擾
		IFeatureCursorPtr ipPOICursor;
		long lPOICount = 0;
		ipPOICursor = Search_Feature ( m_ipFC_POI, ipBuildingsEnvelope, esriSpatialRelIntersects, "", lPOICount );
		if( ipPOICursor == NULL ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\t��Ԍ����Ɏ��s(POI)\n", lCurMesh );
			bIsOK = false;
			continue;
		}
		IFeatureCursorPtr ipRealEstateCursor;
		long lRealEstateCount = 0;
		ipRealEstateCursor = Search_Feature ( m_ipFC_RealEstate, ipBuildingsEnvelope, esriSpatialRelIntersects, "", lRealEstateCount );
		if( ipRealEstateCursor == NULL ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\t��Ԍ����Ɏ��s(POI)\n", lCurMesh );
			bIsOK = false;
			continue;
		}

		// �����ƃ|�C���g�̓���֌W�𒲂ׂ�
		// �������R���e�i�Ɋi�[
		mPointInfo_XY mWorkPointsData;
		// POI�̃f�[�^���擾
		long lWorkPOICount = MakePointContainer ( ipPOICursor, CString(_T("NAME")), mWorkPointsData, PriorityManager::TYPE_POI );
		if( lWorkPOICount < 0 ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\t�f�[�^�R���e�i�쐬�Ɏ��s(POI)\n", lCurMesh );
			bIsOK = false;
			continue;
		}
		// �s���Y�����擾
		long lWorkRealEstateCount = MakePointContainer ( ipRealEstateCursor, CString(_T("NAME")), mWorkPointsData  );
		if( lWorkRealEstateCount < 0 ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\t�f�[�^�R���e�i�쐬�Ɏ��s(�s���Y���E�h�ΑΏە����)\n", lCurMesh );
			bIsOK = false;
			continue;
		}
		// ����֌W�𒲂ׁA�����o�͂���
		if( !PrintOut ( mWorkData, mWorkPointsData ) ) {
			PrnMsg( m_fErr_Fp, stderr, "%d\t���o�͂Ɏ��s\n", lCurMesh );
			bIsOK = false;
			continue;

		}
		DWORD          a_dMake_Finish	= 0;
		a_dMake_Finish = timeGetTime();

		PrnMsg(stdout, NULL, "�������� = %0.2f(s)\n", m_eExeTime.Get_Lap() / 1000.0 );

		// ��ƒ��f�p
		if(_kbhit()){
			int aKey = _getch();
			if(aKey == 'Q'){
				PrnMsg ( stdout, NULL, "#�X�V�����𒆒f���܂� %d �܂Ŋ���\n", lCurMesh );
				break;
			}
		}
	}
	PrnMsg( stdout, NULL, "\n" );			

	//�@���s���I��
	// �I�����Ԃ̎擾
	PrnMsg	( m_fErr_Fp, stderr, "#�f�[�^�ϊ������� = %0.2f(s)�ł���\n",
						(double)((m_eExeTime.Get_From_Start()) / 1000.0) );
	return	( true );
}
