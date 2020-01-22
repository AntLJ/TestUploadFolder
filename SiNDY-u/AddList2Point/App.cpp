#include "StdAfx.h"
#include "App.h"

#include "ListHandle.h"
#include <WinLib/VersionInfo.h>
#include <crd_cnv/coord_converter.h>
#include <sindy/workspace.h>

// ���[�N�X�y�[�X�擾
inline IWorkspacePtr getWorkspace( LPCTSTR lpcszDBProp )
{
	INamePtr ipName( sindy::create_workspace_name(lpcszDBProp) );
	if( NULL == ipName )
		return NULL;
	IUnknownPtr ipUnknown;
	ipName->Open( &ipUnknown );
	return ipUnknown;
}

CApp::CApp(void) : m_nListMode(-1), m_lpszTargetFCName(NULL)
{
}

CApp::~CApp(void)
{
}

// �g�p�@
void CApp::printUsage()
{
	std::cerr << "�y���X�g����|�C���g�ǉ��c�[���z\n"
			  << "(Usage)AddList2Point.exe [�e��I�v�V����] ( > �i�����O�t�@�C��)\n"
			  << "�E�I�v�V�����ɂ���\n"
			  << "  -i <ID���X�g>     ... ID���X�g���w��(�t�H�[�}�b�g�̓w���v�Q��)\n"
			  << "  -m <MeshXY���X�g> ... MeshXY���X�g���w��(�t�H�[�}�b�g�̓w���v�Q��)\n"
			  << "�E���ϐ��ɂ���\n"
			  << "  DB_FROM  �ǉ�����DB�ڑ��v���p�e�B(-i �I�v�V�����w�莞�K�{)\n"
			  << "  FC_FROM  �ǉ����̃t�B�[�`���N���X��(�I�u�W�F�N�g����FC_TO�ƕK���ꏏ�łȂ��Ƃ����Ȃ�)\n"
			  << "  DB_TO    �ǉ����DB�ڑ��v���p�e�B\n"
			  << "  FC_TO    �ǉ���̃t�B�[�`���N���X��\n" << std::endl;
}

// ������
bool CApp::init(int argc, char *argv[])
{
	if( argc < 3 )
		return false;

	for( int i=1;i<argc-1;++i )
	{
		if( strcmp(argv[i], "-i") == 0 ){
			m_nListMode = IDLIST_MODE;
			strncpy( m_szListFile, argv[++i], sizeof(m_szListFile) );
		}
		else if( strcmp(argv[i], "-m") == 0 ){
			m_nListMode = MXYLIST_MODE;
			strncpy( m_szListFile, argv[++i], sizeof(m_szListFile) );
		}
		else{
			std::cerr << "#ERROR �s���ȃI�v�V���� : " << argv[i] << std::endl;
			return false;
		}
	}

	std::cout.precision(12);

	return setDBInfoFromEnv();
}

// ���s
bool CApp::execute()
{
	// �ҏW�J�n
	if( !editStart() )
		return false;

	// �ҏW�p���[�N�X�y�[�X�Œǉ���t�B�[�`���N���X���擾����
	((IFeatureWorkspacePtr)m_ipWorkspaceEdit)->OpenFeatureClass( CComBSTR(m_lpszTargetFCName), &m_ipTargetToFC );
	if( ! m_ipTargetToFC ){
		std::cerr << "#ERROR �ǉ���t�B�[�`���N���X�̎擾�Ɏ��s : " << m_lpszTargetFCName << std::endl;
		m_ipWorkspaceEdit->AbortEditOperation();
		return false;
	}

	// �ǉ���t�B�[�`���N���X�t�B�[���h�ꗗ(OID����)�쐬�{���O�w�b�_�[�o��
	CString strLogHeader( _T("#FREESTYLELOG\nLAYER\tOBJECTID\tMSG") );
	IFieldsPtr ipFields;
	m_ipTargetToFC->get_Fields( &ipFields );
	long lFieldCount = 0;
	ipFields->get_FieldCount( &lFieldCount );	// �t�B�[���h���擾
	for( long i = 0; i < lFieldCount; ++i )
	{
		IFieldPtr ipField;
		ipFields->get_Field( i, &ipField );
		CComBSTR bstr;
		ipField->get_Name( &bstr );
		CString strField( bstr );
		if( strField != _T("OBJECTID") && strField != _T("SHAPE") )
		{
			m_vecTargetFCFID.push_back( i );
			strLogHeader.AppendFormat( _T("\t%s"), strField );
		}
	}
	CVersion cVer;
	strLogHeader.AppendFormat( _T("\n#%s FILEVERSION %s PRODUCTVERSION %s"), cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
	std::cout << strLogHeader << std::endl;

	// �����J�n
	switch( m_nListMode )
	{
	case IDLIST_MODE:	addFromID();	break;
	case MXYLIST_MODE:	addFromMXY();	break;
	default:
		std::cerr << "#ERROR ���X�g�t�@�C�����w�肵�Ă��������I�I" << std::endl;
		break;
	}

	// �ҏW�����܂�
	if( !editStop() )
		return false;

	return true;
}

////////////////////////////////////////
//	�����֐�
////////////////////////////////////////

// ���ϐ�����c�a�֌W�̏��擾
bool CApp::setDBInfoFromEnv()
{
#ifdef _DEBUG
	_tputenv( _T("DB_FROM=FIX201101/FIX201101/SDE.DEFAULT/5151/topaz2") );
	_tputenv( _T("FC_FROM=Gou_Point") );
	_tputenv( _T("DB_TO=SiNDYTEST/SiNDYTEST/SiNDYTEST.AddList2Point/5151/coral2") );
	_tputenv( _T("FC_TO=Gou_Point") );
#endif

	LPTSTR lpszDBProp = NULL;

	// ID���X�g���[�h�Ȃ�A�ǉ����ɐڑ����t�B�[�`���N���X���擾
	if( IDLIST_MODE == m_nListMode )
	{
		if( !(lpszDBProp = _tgetenv(_T("DB_FROM"))) ){
			std::cerr << "#ERROR ���ϐ� DB_FROM �̎擾�Ɏ��s" << std::endl;
			return false;
		}
		LPTSTR lpszTargetFCName = NULL;
		if( !(lpszTargetFCName = _tgetenv(_T("FC_FROM"))) ){
			std::cerr << "#ERROR ���ϐ� FC_FROM �̎擾�Ɏ��s" << std::endl;
			return false;
		}

		m_ipFromWorkspace = getWorkspace(lpszDBProp);
		if( ! m_ipFromWorkspace ){
			std::cerr << "#ERROR �ǉ�����SDE�ڑ��Ɏ��s : " << lpszDBProp << std::endl;
			return false;
		}
		std::cerr << "�ǉ���DB�ڑ� : " << lpszDBProp << std::endl;
		((IFeatureWorkspacePtr)m_ipFromWorkspace)->OpenFeatureClass( _bstr_t(lpszTargetFCName), &m_ipTargetFromFC );
		if( ! m_ipTargetFromFC ){
			std::cerr << "#ERROR �ǉ����t�B�[�`���N���X�̎擾�Ɏ��s : " << lpszTargetFCName << std::endl;
			return false;
		}
	}

	lpszDBProp = NULL;
	if( !(lpszDBProp = _tgetenv(_T("DB_TO"))) ){
		std::cerr << "#ERROR ���ϐ� DB_TO �̎擾�Ɏ��s" << std::endl;
		return false;
	}
	if( !(m_lpszTargetFCName = _tgetenv(_T("FC_TO"))) ){
		std::cerr << "#ERROR ���ϐ� FC_TO �̎擾�Ɏ��s" << std::endl;
		return false;
	}

	m_ipToWorkspace = getWorkspace(lpszDBProp);
	if( ! m_ipToWorkspace ){
		std::cerr << "#ERROR �ǉ����SDE�ڑ��Ɏ��s : " << lpszDBProp << std::endl;
		return false;
	}
	std::cerr << "�ǉ���DB�ڑ� : " << lpszDBProp << std::endl;

	// �ǉ���t�B�[�`���N���X�́A�ҏW���[�N�X�y�[�X�Ŏ擾����

	return true;
}

// �ҏW�J�n
bool CApp::editStart()
{
	// ��ɕҏW�p�q�o�[�W�����쐬
	TCHAR szChildVerName[100] = _T("");
	SYSTEMTIME st;
	::GetLocalTime( &st );
	sprintf( szChildVerName, "AddList2Point_%04d%02d%02d%02d%02d%02d", 
			 st.wYear, st.wMonth, st.wDay,
			 st.wHour, st.wMinute, st.wSecond );
	IVersionedWorkspacePtr ipVerWorkspace( m_ipToWorkspace );
	if( NULL == ipVerWorkspace ){
		std::cerr << "#ERROR �o�[�W�����Ή��ł���܂���" << std::endl;
		return false;
	}

	// DEFAULT�o�[�W�����͂��߁I
	IVersionPtr ipCurVersion( m_ipToWorkspace );
	CComBSTR bstr;
	ipCurVersion->get_VersionName(&bstr);

	USES_CONVERSION;
	std::string strVer = OLE2A( bstr );
	if( strVer.find("DEFAULT") != -1 ){
		std::cerr << "#ERROR DEFAULT�o�[�W�����ł��B�I�����܂� : " << strVer << std::endl;
		return false;
	}

	// ���ɂ��̃o�[�W�����������݂��邩�`�F�b�N
	IVersionPtr ipChildVersion;
	ipVerWorkspace->FindVersion( CComBSTR(szChildVerName), &ipChildVersion );
	if( NULL == ipChildVersion ){
		ipCurVersion->CreateVersion( CComBSTR(szChildVerName), &ipChildVersion );
		if( NULL == ipChildVersion ){
			std::cerr << "#ERROR �q�o�[�W�����̍쐬�Ɏ��s : " << szChildVerName << std::endl;
			return false;
		}
	}
	std::cerr << "�q�o�[�W���� : " << szChildVerName << " �̍쐬������\n" << std::endl;

	m_ipWorkspaceEdit = ipChildVersion;
	if( SUCCEEDED(m_ipWorkspaceEdit->StartEditing(VARIANT_FALSE)) && SUCCEEDED(m_ipWorkspaceEdit->StartEditOperation()) ){
		std::cerr << "�ҏW���J�n���܂�" << std::endl;
		return true;
	}
	else{
		std::cerr << "#ERROR �ҏW���J�n�ł��܂���ł���" << std::endl;
		return false;
	}
}

// �ҏW�I��
bool CApp::editStop()
{
	// �X�V�̌㏈��(�ҏW��ۑ�����Ȃ� VARIANT_TRUE �ɂ��邱��)
	if( SUCCEEDED(m_ipWorkspaceEdit->StopEditOperation()) && SUCCEEDED(m_ipWorkspaceEdit->StopEditing(VARIANT_TRUE)) ){
		std::cerr << "�ҏW�ۑ�������" << std::endl;
		return true;
	}
	else{
		std::cerr << "#ERROR �ҏW�̕ۑ��Ɏ��s" << std::endl;
		m_ipWorkspaceEdit->AbortEditOperation();
		return false;
	}
}

// �h�c���X�g����ǉ�
void CApp::addFromID()
{
	std::vector<CIDPoint>	m_vecList;
	CListHandle cListHandle;
	if( !cListHandle.loadIDList(m_szListFile, m_vecList) ){
		std::cerr << "#ERROR ID���X�g�̓ǂݍ��݂Ɏ��s : " << m_szListFile << std::endl;
		return;
	}

	// �}���p�t�B�[�`���J�[�\��
	IFeatureCursorPtr ipInsFeatureCursor;
	if (FAILED(m_ipTargetToFC->Insert(VARIANT_TRUE, &ipInsFeatureCursor))) {
		std::cerr << "#ERROR �C���T�[�g�Ɏ��s : " << m_szListFile << std::endl;
		return;
	}

	int nAllCount = (signed)m_vecList.size();
	int nCount = 1;
	for( std::vector<CIDPoint>::const_iterator itr=m_vecList.begin();itr!=m_vecList.end();++itr )
	{
		std::cerr << nCount++ << " / " << nAllCount << " ������\r";

		// �ǉ����̃t�B�[�`�����擾
		IFeaturePtr ipFeature;
		m_ipTargetFromFC->GetFeature( itr->m_lOID, &ipFeature );
		if( ! ipFeature ){
			std::cout << "#ERROR �ǉ�������t�B�[�`�����擾�ł��Ȃ� " << itr->m_lOID << std::endl;
			continue;
		}

		// �ǉ����t�B�[�`���̑������Z�b�g
		std::string strError;
		IFeatureBufferPtr ipFeatureBuffer;
		m_ipTargetToFC->CreateFeatureBuffer(&ipFeatureBuffer);
		if (!setAttribute(ipFeature, ipFeatureBuffer, strError)) {
			std::cout << "#ERROR " << strError << " " << itr->m_lOID << std::endl;
			continue;
		}

		// ���X�g�̎w���ɏ]���āA�����l��ύX
		if( !setAttribute(itr->m_mapVal, ipFeatureBuffer, strError) ){
			std::cout << "#ERROR " << strError << " " << itr->m_lOID << std::endl;
			continue;
		}

		// ���ʑ����l�Z�b�g
		if( !setCommonAttribute(ipFeatureBuffer, strError) ){
			std::cout << "#ERROR " << strError << " " << itr->m_lOID << std::endl;
			continue;
		}

		// �ǉ�
		CComVariant vaOID;
		if( SUCCEEDED(ipInsFeatureCursor->InsertFeature(ipFeatureBuffer, &vaOID)) )
		{
			PrintLog( vaOID.lVal, ipFeatureBuffer );
			ipInsFeatureCursor->Flush();
		}
		else
			std::cout << "#ERROR �ǉ��Ɏ��s" << std::endl;
	}
}

// MXY���X�g����ǉ�
void CApp::addFromMXY()
{
	std::vector<CMXYPoint>	m_vecList;
	CListHandle cListHandle;
	if( !cListHandle.loadMXYList(m_szListFile, m_vecList) ){
		std::cerr << "#ERROR MXY���X�g�̓ǂݍ��݂Ɏ��s : " << m_szListFile << std::endl;
		return;
	}

	// �}���p�t�B�[�`���J�[�\��
	IFeatureCursorPtr ipInsFeatureCursor;
	m_ipTargetToFC->Insert( VARIANT_TRUE, &ipInsFeatureCursor );

	crd_cnv cCrdCnv;
	int nAllCount = (signed)m_vecList.size();
	int nCount = 1;
	for( std::vector<CMXYPoint>::const_iterator itr=m_vecList.begin();itr!=m_vecList.end();++itr )
	{
		std::cerr << nCount++ << " / " << nAllCount << " ������\r";

		d_lonlat cLonLat;
		int nResult = cCrdCnv.MeshtoLL( (*itr).m_nMeshCode, (*itr).m_nMeshX, (*itr).m_nMeshY, 
										&(cLonLat.lon), &(cLonLat.lat) );
		if( -1 == nResult ){
			std::cout << "#ERROR MESHXY����10�i�ܓx�o�x�ɕϊ����s " 
					  << (*itr).m_nMeshCode << "," << (*itr).m_nMeshX << "," << (*itr).m_nMeshY << std::endl;
			continue;
		}
		IPointPtr ipPoint( CLSID_Point );
		ipPoint->PutCoords( cLonLat.lon, cLonLat.lat );

		IFeatureBufferPtr ipFeatureBuffer;
		m_ipTargetToFC->CreateFeatureBuffer( &ipFeatureBuffer );

		// �f�t�H���g�l���i�[����
		LONG lFieldCount = 0;
		IFieldsPtr ipFields;
		ipFeatureBuffer->get_Fields( &ipFields );
		ipFields->get_FieldCount( &lFieldCount );
		for( LONG i = 0; i < lFieldCount; i++ )
		{
			IFieldPtr ipField;
			VARIANT_BOOL vaIsEditable = VARIANT_FALSE;

			ipFields->get_Field( i, &ipField );
			ipField->get_Editable( &vaIsEditable );
			// �ҏW�\�t�B�[���h�̂�
			if( VARIANT_TRUE == vaIsEditable )
			{
				CComVariant vaDefaultValue;
				ipField->get_DefaultValue( &vaDefaultValue );
				ipFeatureBuffer->put_Value( i, vaDefaultValue );
			}
		}
		ipFeatureBuffer->putref_Shape( IGeometryPtr(ipPoint) );

		// ���X�g�̎w���ɏ]���āA�����l��ύX
		std::string strError;
		if( !setAttribute((*itr).m_mapVal, ipFeatureBuffer, strError) ){
			std::cout << "#ERROR " << strError << " "
				      << (*itr).m_nMeshCode << "," << (*itr).m_nMeshX << "," << (*itr).m_nMeshY << std::endl;
			continue;
		}

		// ���ʑ����l�Z�b�g
		if( !setCommonAttribute(ipFeatureBuffer, strError) ){
			std::cout << "#ERROR " << strError << " " 
				      << (*itr).m_nMeshCode << "," << (*itr).m_nMeshX << "," << (*itr).m_nMeshY << std::endl;
			continue;
		}

		// �ǉ�
		CComVariant vaOID;
		if( SUCCEEDED(ipInsFeatureCursor->InsertFeature(ipFeatureBuffer, &vaOID)) )
		{
			PrintLog( vaOID.lVal, ipFeatureBuffer );
			ipInsFeatureCursor->Flush();
		}
		else
			std::cout << "#ERROR �ǉ��Ɏ��s" << std::endl;
	}
}

bool CApp::setAttribute(IFeaturePtr ipSrcFeature, IFeatureBufferPtr ipFeatureBuffer, std::string& strError)
{
	IFieldsPtr ipFields;
	ipFeatureBuffer->get_Fields( &ipFields );

	long count(0);
	ipFields->get_FieldCount(&count);
	for (long i = 0; i < count; ++i)
	{
		IFieldPtr ipField;
		ipFields->get_Field(i, &ipField);
		esriFieldType type;
		ipField->get_Type(&type);

		if (type == esriFieldTypeOID) {
			continue;
		}
		else if (type == esriFieldTypeGeometry) {
			IGeometryPtr geo;
			ipSrcFeature->get_ShapeCopy(&geo);
			ipFeatureBuffer->putref_Shape(geo);
			continue;
		}

		CComVariant val;
		ipSrcFeature->get_Value(i, &val);

		if (FAILED(ipFeatureBuffer->put_Value(i, val)))
		{
			CComBSTR bstr;
			ipField->get_AliasName(&bstr);
			strError = bstr + "�̃Z�b�g�Ɏ��s";
			return false;
		}
	}

	return true;
}

// �����l�Z�b�g
bool CApp::setAttribute( const std::map<CString, CString>& mapVal, IFeatureBuffer* ipFeatureBuffer, std::string& strError )
{
	IFieldsPtr ipFields;
	ipFeatureBuffer->get_Fields( &ipFields );
	USES_CONVERSION;
	for( std::map<CString, CString>::const_iterator itr=mapVal.begin();itr!=mapVal.end();++itr )
	{
		LONG lFID = 0;
		ipFields->FindField( CComBSTR(_T(itr->first)), &lFID );

		if( !itr->second.IsEmpty() ){
			
			// ��Ƀf�[�^�^���������ׂ�
			IFieldPtr ipField;
			ipFields->get_Field( lFID, &ipField );
			LONG varType;
			ipField->get_VarType( &varType );

			CComVariant vaValue;
			switch( varType )
			{
			case VT_BSTR:
				vaValue.vt = VT_BSTR;
				vaValue.bstrVal = T2BSTR(itr->second);
				break;
			case VT_I4:
				vaValue.vt = VT_I4;
				vaValue.lVal = atol(itr->second);
				break;
			case VT_DATE:
				vaValue.vt = VT_BSTR;
				vaValue.bstrVal = T2BSTR(itr->second);
				vaValue.ChangeType(VT_DATE);
				break;
			default:
				continue;
			}

			if( FAILED(ipFeatureBuffer->put_Value(lFID, vaValue)) ){
				strError = itr->first + "�̃Z�b�g�Ɏ��s";
				return false;
			}

		}
	}

	return true;
}

// ���ʑ����Z�b�g
bool CApp::setCommonAttribute( IFeatureBuffer* ipFeatureBuffer, std::string& strError )
{
	long lFID = 0;

	// �ŏI�X�V��(�l����Ȃ�A[sindy]�ɂ���)
	m_ipTargetToFC->FindField( CComBSTR(_T("OPERATOR")), &lFID );
	CComVariant vaValue;
	ipFeatureBuffer->get_Value( lFID, &vaValue );
	if( vaValue.vt == VT_EMPTY || vaValue.vt == VT_NULL
		|| (vaValue.vt == VT_BSTR && CString(vaValue.bstrVal) == _T(" ")) ){

		if( FAILED(ipFeatureBuffer->put_Value(lFID, CComVariant(OPERATOR))) ){
			strError = "OPERATOR�̃Z�b�g�Ɏ��s";
			return false;
		}
	}

	// �ŏI�X�V���e(�V�K�쐬�ɂ���)
	m_ipTargetToFC->FindField( CComBSTR(_T("UPDATETYPE_C")), &lFID );
	if( FAILED(ipFeatureBuffer->put_Value(lFID, CComVariant(1))) ){
		strError = "UPDATETYPE_C�̃Z�b�g�Ɏ��s";
		return false;
	}

	// �ŏI�X�V�v���O������
	m_ipTargetToFC->FindField( CComBSTR(_T("MODIFYPROGNAME")), &lFID );
	if( FAILED(ipFeatureBuffer->put_Value(lFID, CComVariant(PROGNAME))) ){
		strError = "MODIFYPROGNAME�̃Z�b�g�Ɏ��s";
		return false;
	}

	// �ŏI�v���O�����X�V����
	TCHAR szTime[100] = {0};
	SYSTEMTIME st;
	::GetLocalTime( &st );
	_stprintf( szTime, _T("%04d/%02d/%02d %02d:%02d:%02d"), 
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	m_ipTargetToFC->FindField( CComBSTR(_T("PROGMODIFYDATE")), &lFID );
	if( FAILED(ipFeatureBuffer->put_Value(lFID, CComVariant(szTime))) ){
		strError = "PROGMODIFYDATE�̃Z�b�g�Ɏ��s";
		return false;
	}

	return true;
}

// ���O�o��
void CApp::PrintLog( long lOID, IFeaturePtr ipFeature )
{
	std::cout << m_lpszTargetFCName << "\t" << lOID << "\t�ǉ�����";

	// Bug8605 �S�����o��
	for( std::vector<long>::const_iterator it = m_vecTargetFCFID.begin(); it != m_vecTargetFCFID.end(); ++it )
	{
		CComVariant vaValue;
		ipFeature->get_Value( *it, &vaValue );
		std::cout << "\t" << vaValue;
	}
	std::cout << std::endl;
}