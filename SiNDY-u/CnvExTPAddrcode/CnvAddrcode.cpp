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
#include "CnvAddrcode.h"
#include <sindy/workspace.h>

/**
 * @brief	�Z��DB�ڑ�
 * @param	strDBProp	[in]	�ڑ��v���p�e�B
 * @param	cDatabase	[out]	�f�[�^�\�[�X
 * @return	bool
 */
inline bool getDataSource( const CString& strDBProp, ODatabase& cDatabase )
{
	CString str(strDBProp);

	int nPos = 0;
	CString strUser		= str.Tokenize( "/", nPos );
	CString strPass		= str.Tokenize( "@", nPos );
	CString strDBName	= str.Tokenize( "@", nPos );

	return OSUCCESS == cDatabase.Open( strDBName, strUser, strPass );
}


CCnvAddrcode::CCnvAddrcode(void) : 
	m_ipWorkspace(NULL),
	m_ipEditWorkspace(NULL),
	m_bEditMode(false),
	m_lAddrcode_FID(0),
	m_lAddrName_FID(0),
	m_lPointStatC_FID(0),
	m_lInfoSrcC_FID(0),
	m_lProgModifyDate_FID(0),
	m_lModifyProgName_FID(0)
{
}

CCnvAddrcode::~CCnvAddrcode(void)
{
}

// �g�p�@
void CCnvAddrcode::printUsage()
{
	std::cerr << "�y�g���d�b�ԍ������|�C���g�Z���R�[�h�u���c�[���z\n"
			  << "(Usage)CnvExTPAddrcode.exe [�I�v�V����] ( > ���O�t�@�C��)\n"
			  << "�E�K�{�I�v�V����\n"
			  << "  -c  [ref/update] ... ���샂�[�h�̎w��(ref : �X�V�ӏ����o  update : �X�V���[�h)\n"
			  << "�E�ǉ��I�v�V����\n"
			  << "  -l  [���R�[�h�Ή��\]  ... �w�莞�ɂ́A�Z���}�X�^�őΉ��ł��Ȃ����̂ɑ΂��āA���X�g�����ɉ\�Ȃ��̂͒u�����܂��B\n"
			  << "�E���ϐ��ɂ���\n"
			  << "  DB_ADDRESS       �Z���nSDE�ڑ��v���p�e�B\n"
			  << "  FC_EXTP_POINT    �g���d�b�ԍ������|�C���g�t�B�[�`���N���X\n"
			  << "  JDB_CONNECT_NEW  (�V)�Z���}�X�^\n"
			  << "  JDB_CONNECT_OLD  (��)�Z���}�X�^" << std::endl;
}

// ������
bool CCnvAddrcode::init(int argc, char *argv[])
{
	// �����̐��`�F�b�N
	if( argc < 3 )
	{
		std::cerr << "#ERROR �I�v�V�����̐����������Ȃ�" << std::endl;
		return false;
	}

	for( int i=1;i<argc-1;++i )
	{
		if( strcmp(argv[i], "-c") == 0 )
		{
			++i;
			if( strcmp(argv[i], "ref") == 0 )			m_bEditMode = false;
			else if( strcmp(argv[i], "update") == 0 )	m_bEditMode = true;

		}
		else if( strcmp(argv[i], "-l") == 0 )
		{
			// ���R�[�h�Ή��\�ǂݍ���
			if( !m_cTmpTable.load(argv[++i]) )
			{
				std::cerr << "#ERROR ���R�[�h�Ή��\�̓ǂݍ��݂Ɏ��s : " << argv[i] << std::endl;
				return false;
			}
		}
		else
		{
			std::cerr << "#ERROR �s���ȃI�v�V�����ł� : " << argv[i] << std::endl;
			return false;
		}
	}

	return setInfoFromEnv();	// ���ϐ�����A�K�v���̎擾
}

// ���s
bool CCnvAddrcode::execute()
{
	if( m_bEditMode )
	{
		// �ҏW���[�h�Ȃ�
		std::cerr << "���s���[�h : �ҏW" << std::endl;
		if( !editStart() )
			return false;

		IFeatureWorkspacePtr(m_ipEditWorkspace)->OpenTable( CComBSTR(_T(m_strExTPFName)), &m_ipExTPTable );
		if( NULL == m_ipExTPTable )
		{
			std::cerr << "#ERROR �ҏW���[�N�X�y�[�X����A�g���d�b�ԍ������|�C���g�e�[�u���̎擾�Ɏ��s : " << m_strExTPFName << std::endl;
			return false;
		}
	}
	else
		std::cerr << "���s���[�h : �Q��(�ҏW�͍s���܂���)" << std::endl;

	printHeader();

	// ���R�[�h�擾
	LONG lAllCount = 0;
	m_ipExTPTable->RowCount( NULL, &lAllCount );
	_ICursorPtr ipCursor;
	if( FAILED(m_ipExTPTable->Search(NULL, VARIANT_FALSE, &ipCursor)) )
	{
		std::cerr << "#ERROR ���R�[�h�̎擾�Ɏ��s" << std::endl;
		m_ipEditWorkspace->AbortEditOperation();
		return false;
	}
	
	_IRowPtr ipRow;
	LONG lCount = 1;
	while( ipCursor->NextRow(&ipRow) == S_OK )
	{
		std::cerr << lCount++ << " / " << lAllCount << " ������\r";

		editExTPPoint( ipRow );	// �ҏW
	}
	
	std::cerr << std::endl;

	if( m_bEditMode && !editStop() )
	{
		return false;
	}
	else
		std::cerr << "����" << std::endl;

	return true;
}

/**
 * �����֐�
 */

// ���ϐ�����K�v���̐ݒ�
bool CCnvAddrcode::setInfoFromEnv()
{
#ifdef _DEBUG
	_tputenv( _T("DB_ADDRESS=techmap/techmap/SDE.ExTP_Work/5151/spinel2") );
	_tputenv( _T("FC_EXTP_POINT=ExTP_POINT") );
	_tputenv( _T("JDB_CONNECT_NEW=k0703/k0703@delltcp") );
	_tputenv( _T("JDB_CONNECT_OLD=y0611e/y0611e@delltcp") );
#endif

	// ���ϐ��擾
	CString strDBProp, strJDBPropNew, strJDBPropOld;
	if( !strDBProp.GetEnvironmentVariable( _T("DB_ADDRESS") ) || 
		!strJDBPropNew.GetEnvironmentVariable( _T("JDB_CONNECT_NEW") ) ||
		!strJDBPropOld.GetEnvironmentVariable( _T("JDB_CONNECT_OLD") ) ||
		!m_strExTPFName.GetEnvironmentVariable( _T("FC_EXTP_POINT") ) )
	{
		std::cerr << "#ERROR ���ϐ����擾�ł��Ȃ����̂�����܂����B�m�F���Ă��������B" << std::endl;
		return false;
	}

	// �Z��DB�ڑ�
	if( !getDataSource(strJDBPropNew, m_cJDBNew) )
	{
		std::cerr << "#ERROR (�V)�Z��DB�ւ̐ڑ��Ɏ��s : " << strJDBPropNew << std::endl;
		return false;
	}
	std::cerr << "#(�V)�Z��DB�֐ڑ� : " << strJDBPropNew << std::endl;
	if( !getDataSource(strJDBPropOld, m_cJDBOld) )
	{
		std::cerr << "#ERROR (��)�Z��DB�ւ̐ڑ��Ɏ��s : " << strJDBPropOld << std::endl;
		return false;
	}
	std::cerr << "#(��)�Z��DB�֐ڑ� : " << strJDBPropOld << std::endl;

	// �Z���R�[�h�u���p�e�[�u���쐬
	std::cerr << "�Z���R�[�h�u���p�e�[�u���쐬���E�E�E";
	if( !makeAddrcodeMap() || !makeTmpcodeMap() )
	{
		std::cerr << "#ERROR �Z���R�[�h�u���p�e�[�u���̍쐬�Ɏ��s" << std::endl;
		return false;
	}
	std::cerr << "�쐬����\n" << std::endl;

	// SDE�ڑ�
	INamePtr ipName( sindy::create_workspace_name(strDBProp) );
	if( ipName )
	{
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		m_ipWorkspace = ipUnknown;
	}
	if( NULL == m_ipWorkspace )
	{
		std::cerr << "#Error SDE�ڑ��Ɏ��s : " << strDBProp << std::endl;
		return false;
	}
	std::cerr << "#SDE�ڑ� : " << strDBProp << std::endl;

	// �g���d�b�ԍ������|�C���g�e�[�u���擾
	IFeatureWorkspacePtr(m_ipWorkspace)->OpenTable( CComBSTR(m_strExTPFName), &m_ipExTPTable );
	if( NULL == m_ipExTPTable )
	{
		std::cerr << "#ERROR �g���d�b�ԍ������|�C���g�e�[�u���̎擾�Ɏ��s : " << m_strExTPFName << std::endl;
		return false;
	}

	// ���̂܂܏Z���R�[�h�t�B�[���h�C���f�b�N�X���擾
	m_ipExTPTable->FindField( CComBSTR(_T("ADDRCODE")), &m_lAddrcode_FID );
	m_ipExTPTable->FindField( CComBSTR(_T("ADDRNAME")), &m_lAddrName_FID );
	m_ipExTPTable->FindField( CComBSTR(_T("INFOSRC_C")), &m_lInfoSrcC_FID );
	m_ipExTPTable->FindField( CComBSTR(_T("POINTSTAT_C")), &m_lPointStatC_FID );
	m_ipExTPTable->FindField( CComBSTR(_T("PROGMODIFYDATE")), &m_lProgModifyDate_FID );
	m_ipExTPTable->FindField( CComBSTR(_T("MODIFYPROGNAME")), &m_lModifyProgName_FID );

	return true;
}

// 11���Z���R�[�h���L�[�ŁA�ŐV11���Z���R�[�h���l��map�R���e�i���쐬
bool CCnvAddrcode::makeAddrcodeMap()
{
	CString strSQL( "select distinct ken_code || shi_code || oaza_code || aza_code, "
					"getnewestcode(ken_code || shi_code || oaza_code || aza_code) from areacode_master" );

	ODynaset cDynaset( m_cJDBNew, strSQL );
	if( cDynaset.GetRecordCount() == 0 )
	{
		std::cerr << "#ERROR SQL�̎��s�Ɏ��s : " << strSQL << std::endl;
		return false;
	}

	OValue cVal1, cVal2;
	do
	{
		cDynaset.GetFieldValue( 0, &cVal1 );
		cDynaset.GetFieldValue( 1, &cVal2 );

		m_mapAddrcode.insert( std::pair<CString, CString>(CString(cVal1), CString(cVal2)) );	// map�Ɋi�[

		cDynaset.MoveNext();

	}while( !cDynaset.IsEOF() );

	return true;
}

// ���R�[�h���L�[�ŁA���R�[�h���l��map�R���e�i�쐬
bool CCnvAddrcode::makeTmpcodeMap()
{
	CString strSQL( "select distinct ken_code || shi_code || oaza_code || aza_code, "
					"getprevcode(ken_code || shi_code || oaza_code || aza_code) from areacode_master "
					"where shusei_code='+'" );

	ODynaset cDynaset( m_cJDBOld, strSQL );
	if( cDynaset.GetRecordCount() == 0 )
	{
		std::cerr << "#ERROR SQL�̎��s�Ɏ��s : " << strSQL << std::endl;
		return false;
	}

	OValue cVal1, cVal2;
	do
	{
		cDynaset.GetFieldValue( 0, &cVal1 );
		cDynaset.GetFieldValue( 1, &cVal2 );

		m_mapTmpcode.insert( std::pair<CString, CString>(CString(cVal1), CString(cVal2)) );	// map�Ɋi�[

		cDynaset.MoveNext();

	}while( !cDynaset.IsEOF() );
	
	return true;
}

// �ҏW�p�q�o�[�W�����쐬
bool CCnvAddrcode::makeChildVersion()
{
	// �q�o�[�W�������͗v����(�Ƃ肠�������Ԗ�)
	SYSTEMTIME st;
	::GetLocalTime( &st );
	CString strChildVersion;
	strChildVersion.Format( _T("CnvExTPAddrcode_%04d%02d%02d_%02d%02d%02d"), 
							st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	IVersionedWorkspacePtr ipVerWorkspace( m_ipWorkspace );
	if( NULL == ipVerWorkspace )
	{
		std::cerr << "#ERROR �o�[�W�����Ή��ł���܂���" << std::endl;
		return false;
	}

	// DEFAULT�o�[�W�����͂��߁I
	IVersionPtr ipCurVersion( m_ipWorkspace );
	CComBSTR bstr;
	ipCurVersion->get_VersionName(&bstr);

	CString strTmp( bstr );
	if( strTmp.Find(_T("DEFAULT")) != -1 )
	{
		std::cerr << "#ERROR DEFAULT�o�[�W�����ł��B�I�����܂� : " << strTmp << std::endl;
		return false;
	}

	// ���ɂ��̃o�[�W�����������݂��邩�`�F�b�N
	IVersionPtr ipChildVersion;
	ipVerWorkspace->FindVersion( CComBSTR(_T(strChildVersion)), &ipChildVersion );
	if( NULL == ipChildVersion )
	{
		ipCurVersion->CreateVersion( CComBSTR(_T(strChildVersion)), &ipChildVersion );
		if( NULL == ipChildVersion )
		{
			std::cerr << "#ERROR �q�o�[�W�����̍쐬�Ɏ��s���܂��� : " << strChildVersion << std::endl;
			return false;
		}
	}

	std::cerr << "�q�o�[�W���� : " << strChildVersion << " �̍쐬������\n" << std::endl;

	m_ipEditWorkspace = ipChildVersion;

	return true;
}

// �ҏW�J�n
bool CCnvAddrcode::editStart()
{
	// �ҏW�p�q�o�[�W�����쐬
	if( !makeChildVersion() )
		return false;

	if( SUCCEEDED(m_ipEditWorkspace->StartEditing(VARIANT_FALSE)) && SUCCEEDED(m_ipEditWorkspace->StartEditOperation()) )
	{
		std::cerr << "�ҏW���J�n���܂�" << std::endl;
		return true;
	}
	else
	{
		std::cerr << "#ERROR �ҏW���J�n�ł��܂���ł���" << std::endl;
		return false;
	}
}

// �ҏW�I��
bool CCnvAddrcode::editStop()
{
	// �X�V�̌㏈��(�ҏW��ۑ�����Ȃ� VARIANT_TRUE �ɂ��邱��)
	if( SUCCEEDED(m_ipEditWorkspace->StopEditOperation()) && SUCCEEDED(m_ipEditWorkspace->StopEditing(VARIANT_TRUE)) )
	{
		std::cerr << "�ҏW�ۑ����������܂���" << std::endl;
		return true;
	}
	else
	{
		std::cerr << "#ERROR �ҏW�̕ۑ��Ɏ��s���܂���" << std::endl;
		return false;
	}
}

// �g���d�b�ԍ������|�C���g�ҏW
void CCnvAddrcode::editExTPPoint( _IRow *ipRow )
{
	// �Z���R�[�h���擾����
	LONG lOID;
	ipRow->get_OID( &lOID );
	CComVariant vaVal;
	ipRow->get_Value( m_lAddrcode_FID, &vaVal );

	CString strAddrcode( vaVal.bstrVal );

	// ���R�[�h�Ȃ�΁A��ɋ��Z���R�[�h�ɖ߂��Ă���ύX�Ώۂ��`�F�b�N
	bool bIsTmp = false;	// ���R�[�h���ۂ�
	int nResult = 0;		// ���ʂ�1�̂Ƃ��ɍX�V
	CString strOldAddrcode;
	CString strNewAddrcode;
	if( isTmpCode(strAddrcode.Left(11), strOldAddrcode) )
	{
		bIsTmp = true;
		nResult = getNewAddrcode( strOldAddrcode, strNewAddrcode );

		// ���R�[�h���A�܂����R�[�h�̂܂܂Ȃ�X�V�Ȃ�(�蓮�X�V���Ȃ��Ƃ����Ȃ��\������̂Ń��O�͏o��)
		if( strAddrcode.Left(11) == strNewAddrcode )
		{
			printLog( ipRow, C_NOCHANGE_KARICODE, strAddrcode );
			return;
		}
	}
	else
	{
		// ���R�[�h�łȂ��Ƃ�
		nResult = getNewAddrcode( strAddrcode.Left(11), strNewAddrcode );
		
		// �V�R�[�h�Ƌ��R�[�h�����������`�F�b�N���A��������΍X�V�Ȃ�
		if( nResult != 2 )
			nResult = ( (strAddrcode.Left(11) != strNewAddrcode)? 1 : 0 );
	}


	if( 1 != nResult )
	{
		int nState = checkAddrCodeState( strAddrcode.Left(11) );
		if( HAISHI_CODE == nState || NG_CODE == nState )
		{
			if( m_cTmpTable.isSetting() )
			{
				// ���R�[�h�Ή��\�ł��u�����݂�Ȃ�`�F�b�N���Ă݂�
				if( m_cTmpTable.getTaiouCode(strAddrcode.Left(11), strNewAddrcode) )
					nResult = 1;	/// �X�V�ł���
			}

			if( 1 != nResult )
				printLog( ipRow, (HAISHI_CODE == nState? E_NOUPDATE_HAISHI_ADDRCODE : E_NOUPDATE_NG_ADDRCODE), "", strAddrcode );
		}
	}


	// �Z���R�[�h�ύX����Ȃ�A�R�[�h�u���{���ʑ����X�V
	if( 1 ==  nResult )
	{
		bool bEditError = false;

		// �Q�ƃ��[�h�Ȃ�A���O�o�͂��Ă����܂�
		if( !m_bEditMode )
		{
			if( checkAddrCodeState(strNewAddrcode) != HAISHI_CODE )
				printLog( ipRow, O_UPDATE_DATA, strAddrcode, strNewAddrcode + strAddrcode.Mid(11) );
			else
				printLog( ipRow, O_UPDATE_DATA_HAISHI_ADDRCODE, strAddrcode, strNewAddrcode + strAddrcode.Mid(11) );
			return;
		}

		// �ŏI�X�V�v���O������
		if( FAILED(ipRow->put_Value(m_lModifyProgName_FID, CComVariant(PROGNAME))) ){
			printLog( ipRow, E_UPDATE_PROGNAME );
			bEditError = true;
		}
			
		// �ŏI�v���O�����X�V����
		SYSTEMTIME st;
		::GetLocalTime( &st );
		CString strTime;
		strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
		if( FAILED(ipRow->put_Value(m_lProgModifyDate_FID, CComVariant(strTime))) )
		{
			printLog( ipRow, E_UPDATE_MODIFYDATE );
			bEditError = true;
		}

		// �Z���R�[�h�u��
		strNewAddrcode += strAddrcode.Mid(11);
		if( strNewAddrcode.GetLength() == 20 )
		{
			if( FAILED(ipRow->put_Value(m_lAddrcode_FID, CComVariant(strNewAddrcode))) )
			{
				printLog( ipRow, E_UPDATE_ADDRCODE, "", strNewAddrcode );
				bEditError = true;
			}
		}
		else
		{
			printLog( ipRow, E_UPDATE_ADDRCODE, "", strNewAddrcode );
			bEditError = true;
		}

		if( !bEditError )
		{
			if( SUCCEEDED(ipRow->Store()) )
			{
				int nErrorCode = 0;
				if( !bIsTmp )
					nErrorCode = (checkAddrCodeState(strNewAddrcode.Left(11)) == SIKOU_CODE)?  O_UPDATE : O_UPDATE_HAISHI_ADDRCODE;
				else
					nErrorCode = O_UPDATE_TMP;

				if( nErrorCode != 0 )
					printLog( ipRow, nErrorCode, strAddrcode, strNewAddrcode );
			}
			else
			{
				printLog( ipRow, E_STORE );
				m_ipEditWorkspace->AbortEditOperation();
			}
		}
		else
			m_ipEditWorkspace->AbortEditOperation();
	}
	
}

// 11���Z���R�[�h�̌��݂̏�Ԏ擾
int	CCnvAddrcode::checkAddrCodeState( const CString& strCode )
{
	// ���̏Z���R�[�h�����݂��邩�`�F�b�N
	CString strSQL;
	strSQL.Format( "select haishi_date from areacode_master where ken_code='%s' and shi_code='%s' and oaza_code='%s' and aza_code='%s'",
					strCode.Left(2), strCode.Mid(2, 3), strCode.Mid(5, 3), strCode.Mid(8, 3) );

	ODynaset cDynaset( m_cJDBNew, strSQL );
	if( cDynaset.GetRecordCount() < 1 )
		return NG_CODE;	// �R�[�h�Ȃ���Ζ����R�[�h
	
	OValue cVal;
	cDynaset.GetFieldValue( 0, &cVal );
	CString strVal(cVal);
	return (strVal == "000000")? SIKOU_CODE : HAISHI_CODE;
}

// ���O�o��
void CCnvAddrcode::printLog( _IRow* ipRow, int nErrorCode, const CString& strOldCode /* = "" */, const CString& strNewCode /* = "" */ )
{
	using namespace std;

	LONG lOID;
	ipRow->get_OID( &lOID );

	cout << "0\t" << m_strExTPFName << "\t" << lOID << "\t\t\t";

	switch( nErrorCode )
	{
	case O_UPDATE:		cout << "OK\t1\t�X�V����\t" << strOldCode << "\t" << strNewCode;			break;
	case O_UPDATE_TMP:	cout << "OK\t2\t�X�V����(���R�[�h)\t" << strOldCode << "\t" << strNewCode;	break;
	case E_UPDATE_PROGNAME:		cout << "ERROR\t3\t�ŏI�X�V�v���O�������̍X�V�Ɏ��s\t\t";	break;
	case E_UPDATE_MODIFYDATE:	cout << "ERROR\t4\t�ŏI�v���O�����X�V�����̍X�V�Ɏ��s\t\t";	break;
	case E_UPDATE_ADDRCODE:		cout << "ERROR\t5\t�Z���R�[�h�X�V�Ɏ��s\t\t" << strNewCode;	break;
	case E_STORE:				cout << "ERROR\t6\tStore�Ɏ��s\t\t";						break;
	case O_UPDATE_DATA:			cout << "OK\t7\t�X�V�Ώۃf�[�^\t" << strOldCode << "\t" << strNewCode;	break;
	case C_NOCHANGE_KARICODE:	cout << "CONFIRM\t8\t�V�Z���Ƃ��č쐬���ꂽ���R�[�h��������܂���B�m�F���Ă��������B\t" << strOldCode << "\t"; break;
	case O_UPDATE_HAISHI_ADDRCODE:	cout << "OK\t9\t�X�V����(�A���A�p�~�R�[�h)\t" << strOldCode << "\t" << strNewCode;	break;
	case O_UPDATE_DATA_HAISHI_ADDRCODE:	cout << "OK\t10\t�X�V�Ώۃf�[�^(�A���A�p�~�R�[�h)\t" << strOldCode << "\t" << strNewCode;	break;
	case E_NOUPDATE_HAISHI_ADDRCODE:	cout << "ERROR\t11\t�X�V�Ȃ�(�A���A�p�~�R�[�h)\t\t" << strNewCode;	break;
	case E_NOUPDATE_NG_ADDRCODE:	cout << "ERROR\t12\t�X�V�Ȃ�(�A���A�����R�[�h)\t\t" << strNewCode;	break;
	default:
		cout << "FATAL ERROR\t\t�c�[���쐬�҂ɖ⍇�����Ă�������\t\t";
		break;
	}
	cout << "\t";

	// �Z�����E�������W�񋟌��R�[�h�E�������ʃR�[�h�o��
	CComVariant vaValue;
	ipRow->get_Value( m_lAddrName_FID, &vaValue );
	if( vaValue.vt == VT_BSTR )
		cout << COLE2T(vaValue.bstrVal);
	cout << "\t";

	ipRow->get_Value( m_lInfoSrcC_FID, &vaValue );
	cout << vaValue.lVal << "\t";

	ipRow->get_Value( m_lPointStatC_FID, &vaValue );
	cout << vaValue.lVal << endl;
}
