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

// CheckJusyoDist.cpp: CheckJusyoDist �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "CheckJusyoDist.h"
#include <WinLib/VersionInfo.h>
#include <TDC/useful_headers/str_util.h>

DWORD	gTS, gTC, gTE, gTP[10], gTT;

using namespace uh;
using namespace sindy::schema;

/**
 * @brief	���p����������H
 * @param	strData	[in]	�f�[�^
 * @return	bool
 */
bool IsNumStr( const CString& strData )
{
	int iLen = strData.GetLength();
	for( int i = 0; i < iLen; ++i )
	{
		if( !_istdigit(strData.GetAt(i)) )
			return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CheckJusyoDist::CheckJusyoDist()
{
	mDBCon_Address	= NULL;
	mPGDB_Out	= NULL;

	mFC_CityAdmin	= NULL;
	mFC_GouPoint	= NULL;
	mFC_CSAddrPoint	= NULL;
	mFC_KlinkPoint	= NULL;

	mIgnoreTPG		= false;

	mFN_ChkLog		= NULL;

	mFN_CityList	= NULL;
	mCityCode		= NULL;
	mAddr8List      = NULL;

	mRangeMin		= -1;
	mRangeMax		= -1;

	mDistErr		= 800;		// �|�C���g�Q - �|�C���g�Q
	mDistNbr		= 100;		// �|�C���g�E�O���[�v��臒l

	mCLType			= kCLNone;

	m_iExeMode		= 0;

	// ���s���[�h���X�g�̏�����
	m_exeModeList.insert(ON_FiveCode);
	m_exeModeList.insert(ON_ElevenCode);
	m_exeModeList.insert(ON_Addrcode8);

}

CheckJusyoDist::~CheckJusyoDist()
{

}

// �g�p�@
void CheckJusyoDist::Usage()
{
	cerr << "[�V�����`�F�b�N�c�[��]\n"
		 << "(Usage) CheckJusyoDist.exe [���s�G���A�I�v�V����] [���̑��I�v�V����]\n"
		 << "�E���s�G���A�I�v�V�����ɂ���\n"
		 << "  -ca ... �S��\n"
		 << "  -cr [�J�n�s�撬���R�[�h-�I���s�撬���R�[�h] ... �G���A�w��\n"
		 << "  -cl [�s�撬�����X�g�t�@�C��] ... ���X�g�w��\n"
		 << "  -cs [�s�撬���R�[�h] ... �P��\n"
		 << "  -adr8list [�Z��8���R�[�h���X�g] ... ���X�g�w��\n"
		 << "�E���s���[�h�ɂ���\n"
		 << "  -f  ... 5���R�[�h���s���[�h(-ca, -cr���w�肵���Ƃ��͂�����)\n"
		 << "  -e  ... 11���R�[�h���s���[�h\n"
		 << "  -adr8  ... �Z��8���R�[�h�{�n�� ���s���[�h\n"
		 << "�E���̑��I�v�V�����ɂ���\n"
		 << "  -o  [���O�t�@�C��] ... �K�{�w��\n"
		 << "  -t                 ... TPG���O���[�h(�f�t�H���g�́AOff)\n"
		 << "  -dn [�O���[�v����] ... �O���[�v���̋���(�f�t�H���g�́A100m)\n"
		 << "  -de [�G���[�Ƃ���O���[�v�ԋ���] ... �e�O���[�v�Ԃ̃G���[�ƂȂ鋗��(�f�t�H���g�́A800m)\n"
		 << "�E���ϐ��ɂ���\n"
		 << "  DB_ADDRESS    ... �Z���nSDE�ڑ���\n"
		 << "  FC_GOU_POINT  ... ���|�C���g�t�B�[�`���N���X��\n"
		 << "  FC_CS_ADDR_POINT ... CS�|�C���g�t�B�[�`���N���X\n"
		 << "  FC_KLINK_POINT ... �ƌ`�����N�|�C���g�t�B�[�`���N���X\n"
		 << "  FC_CITY_ADMIN ... �s�s�n�}�s���E�t�B�[�`���N���X��\n"
		 << "  JDB_CONNECT ... �Z��DB�ڑ���\n"
		 << "  PGDB_OUT ... �o�͐��PGDB\n"
		 << "  FC_OUT   ... �o�͐�̃t�B�[�`���N���X��\n" << std::endl;
}

// ���ϐ��擾
bool CheckJusyoDist::fnGetEnvVar()
{
	mDBCon_Address	= getenv("DB_ADDRESS");
	if(!mDBCon_Address) {
		cerr << "���ϐ� DB_ADDRESS ���ݒ肳��Ă��܂���" << endl;
		return false;
	}

	mFC_GouPoint	= getenv("FC_GOU_POINT");
	if(!mFC_GouPoint) {
		cerr << "���ϐ� FC_GOU_POINT ���ݒ肳��Ă��܂���" << endl;
		return false;
	}

	mFC_CityAdmin	= getenv("FC_CITY_ADMIN");
	if(!mFC_CityAdmin) {
		cerr << "���ϐ� FC_CITY_ADMIN ���ݒ肳��Ă��܂���" << endl;
		return false;
	}

	mFC_CSAddrPoint	= getenv("FC_CS_ADDR_POINT");
	if(!mFC_CSAddrPoint) {
		cerr << "���ϐ� FC_CS_ADDR_POINT ���ݒ肳��Ă��܂���" << endl;
		return false;
	}

	mFC_KlinkPoint	= getenv("FC_KLINK_POINT");
	if(!mFC_KlinkPoint) {
		cerr << "���ϐ� FC_KLINK_POINT ���ݒ肳��Ă��܂���" << endl;
		return false;
	}

	mPGDB_Out	= getenv("PGDB_OUT");
	if(!mPGDB_Out) {
		cerr << "���ϐ� PGDB_OUT ���ݒ肳��Ă��܂���" << endl;
		return false;
	}

	mFC_Target = getenv("FC_OUT");
	if(!mFC_Target) {
		cerr << "���ϐ� FC_OUT ���ݒ肳��Ă��܂���" << endl;
		return false;
	}

	return true;
}

// �������
bool CheckJusyoDist::fnAnalyzeArg( int argc, char** argv )
{
	char	*pRange, *pHyphen;

	if( argc < 4 )
	{
		cerr << "����������܂���" << endl;
		return false;
	}

	for( int i = 1; i < argc && argv[i][0] == '-'; i++ )
	{
		uh::tstring arg(argv[i]);

		// ���s���[�h�i�Z��8���R�[�h�{�n�ԃR�[�h�j�̃I�v�V�����m�F
		if(arg == _T("-adr8"))
		{
			m_iExeMode |= ON_Addrcode8;
			continue;
		}
		
		// ���s�G���A�i�Z��8���R�[�h���X�g�j�̃I�v�V�����m�F
		if(arg == _T("-adr8list"))
		{
			mCLType = kAdr8List;
			mAddr8List = ( i+1 < argc ) ? argv[++i] : NULL;
			continue;
		}

		switch( argv[i][1] )
		{
		case 'c':
			switch( argv[i][2] )
			{
			case 'a':
				mCLType	= kCLAll;
				break;
			case 'l':
				mCLType	= kCLList;
				mFN_CityList	= argv[++i];
				break;
			case 's':
				mCLType	= kCLSingle;
				mCityCode	= argv[++i];
				break;
			case 'r':
				mCLType	= kCLRange;
				pRange	= argv[++i];
				mRangeMin	= atol(pRange);
				pHyphen	= strchr(pRange, '-');
				if(pHyphen != NULL) {
					mRangeMax	= atol(++pHyphen);
				}
				break;
			default:
				cerr << "�s���ȃI�v�V�����ł�: " << argv[i] << endl;
				return false;
			}
			break;
		case 'o':	// ���O�t�@�C��
			mFN_ChkLog	= argv[++i];
			break;
		case 't':
			mIgnoreTPG	= true;
			break;
		case 'd':
			switch( argv[i][2] )
			{
			case 'e':
				mDistErr	= atof(argv[++i]);
				break;
			case 'n':
				mDistNbr	= atof(argv[++i]);
				break;
			default:
				cerr << "�s���ȃI�v�V�����ł�: " << argv[i] << endl;
				return false;
			}
			break;
		case 'e':
			m_iExeMode |= ON_ElevenCode;
			break;
		case 'f':
			m_iExeMode |= ON_FiveCode;
			break;
		default:
			cerr << "�s���ȃI�v�V�����ł�: " << argv[i] << endl;
			return false;
		}
	}

	// ���s���[�h�����w��łȂ����`�F�b�N
	if( m_iExeMode == 0 )
	{
		cerr << "���s���[�h���w�肳��Ă���܂���(-e or -f or -adr8)" << endl;
		return false;
	}

	// ���s���[�h�������w�肳��Ă��Ȃ����`�F�b�N
	int count = 0;
	for(const auto& mode : m_exeModeList)
	{
		if(m_iExeMode & mode)
		{
			++count;
		}
	}

	if(count > 1)
	{
		cerr << "���s���[�h�������w�肳��Ă���܂�" << endl;
		return false;
	}

	// ���s���[�h�i�Z��8���R�[�h�{�n�ԃR�[�h�j�w�莞�̊֘A�I�v�V�����̃`�F�b�N
	if( m_iExeMode & ON_Addrcode8)
	{
		if(mCLType != kAdr8List)
		{
			cerr << "�I�v�V���� -adr8list ���w�肳��Ă��܂���" << endl;
			return false;
		}

		if(mAddr8List == NULL)
		{
			cerr << "�I�v�V���� -adr8list �ŏZ��8���R�[�h���X�g�t�@�C�����w�肳��Ă��܂���" << endl;
			return false;
		}
	}

	switch( mCLType )
	{
	case kCLNone:
		cerr << "�I�v�V���� -c* ���w�肳��Ă��܂���" << endl;
		return false;
	case kCLAll:
		if( (m_iExeMode & ON_ElevenCode) || (m_iExeMode & ON_Addrcode8) )
		{
			cerr << "�S���`�F�b�N�̍ۂ� -f(5�����[�h) ���w�肵�Ă�������" << endl;
			return false;
		}
		break;
	case kCLList:
		if( mFN_CityList == NULL )
		{
			cerr << "�I�v�V���� -cl �Ŏs�撬�����X�g�t�@�C�����w�肳��Ă��܂���" << endl;
			return false;
		}
		break;
	case kCLSingle:
		if( mCityCode == NULL )
		{
			cerr << "�I�v�V���� -cs �Ŏs�撬���R�[�h���w�肳��Ă��܂���" << endl;
			return false;
		}
		if( mCityCode != NULL && ((m_iExeMode & ON_ElevenCode && strlen(mCityCode) < 11) || (m_iExeMode & ON_FiveCode && strlen(mCityCode) < 5) ))
		{
			cerr << "�Z���R�[�h�̌������s���ł�" << endl;
			return false;
		}
		break;
	case kCLRange:
		if( mRangeMin < 0 || mRangeMax < 0 )
		{
			cerr << "�I�v�V���� -cr �Ŏs�撬���R�[�h�͈̔͂��������w�肳��Ă��܂���" << endl;
			return false;
		}
		if( (m_iExeMode & ON_ElevenCode) || (m_iExeMode & ON_Addrcode8) )
		{
			cerr << "�͈̓`�F�b�N�̍ۂ� -f(5�����[�h) ���w�肵�Ă�������" << endl;
			return false;
		}
		break;
	}

	if( mFN_ChkLog == NULL )
	{
		cerr << "�`�F�b�N���O�t�@�C�������w�肳��Ă��܂���" << endl;
		return false;
	}

	if( mDistErr == 0 )
	{
		cerr << "[�G���[�Ƃ���O���[�v�ԋ���]�𐳂����w�肵�Ă�������" << endl;
		return false;
	}
	if( mDistNbr == 0 )
	{
		cerr << "[�O���[�v������]�𐳂����w�肵�Ă�������" << endl;
		return false;
	}
	
	return true;

}


bool CheckJusyoDist::fnCreateTmpFC()
{
	// [Bug 6194]���PGDB�擾(�Ȃ���΍쐬)
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );

	IWorkspacePtr ipWorkspace;
	ipWorkspaceFactory->OpenFromFile( CComBSTR(mPGDB_Out), NULL, &ipWorkspace );
	if( NULL == ipWorkspace )
	{
		std::string strTmp = mPGDB_Out;

		// �f�B���N�g���ƃt�@�C���ɕ���
		int nPos = strTmp.rfind( "\\" );
		std::string strDir = (-1 != nPos)? strTmp.substr( 0, nPos ) : ".";
		std::string strFile = (-1 != nPos)? strTmp.substr( nPos+1 ) : strTmp;

		IPropertySetPtr ipPropertySet( CLSID_PropertySet );
		IWorkspaceNamePtr ipWorkspaceName;
		ipWorkspaceFactory->Create( CComBSTR(strDir.c_str()), CComBSTR(strFile.c_str()), ipPropertySet, NULL, &ipWorkspaceName );
		if( ipWorkspaceName )
		{
			IUnknownPtr ipUnknown;
			((INamePtr)ipWorkspaceName)->Open( &ipUnknown );
			ipWorkspace = ipUnknown;
		}
	}

	IFieldsPtr	ipFlds_TmpFC(CLSID_Fields);
	IFieldPtr	ipFld_Shape(CLSID_Field), ipFld_OID(CLSID_Field), ipFld_AddrCode(CLSID_Field);
	IGeometryDefPtr	ipGD_TmpFC(CLSID_GeometryDef);
	ISpatialReferencePtr	ipSR_TmpFC;
	IUIDPtr		ipUID_ClsID, ipUID_ExtClsID;

	ipFC_GouPoint->get_CLSID( &ipUID_ClsID );
	ipFC_GouPoint->get_EXTCLSID( &ipUID_ExtClsID );
	((IGeoDatasetPtr)ipFC_GouPoint)->get_SpatialReference( &ipSR_TmpFC );

	// GeometryDef �ݒ�
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_GeometryType(esriGeometryMultipoint);
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_GridCount(1);
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_GridSize(0, 0.05);
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_HasM(VARIANT_FALSE);
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_HasZ(VARIANT_FALSE);
	((IGeometryDefEditPtr)ipGD_TmpFC)->putref_SpatialReference(ipSR_TmpFC);

	// SHAPE �t�B�[���h
	((IFieldEditPtr)ipFld_Shape)->put_Name(CComBSTR("SHAPE"));
	((IFieldEditPtr)ipFld_Shape)->put_Type(esriFieldTypeGeometry);
	((IFieldEditPtr)ipFld_Shape)->putref_GeometryDef(ipGD_TmpFC);
	((IFieldsEditPtr)ipFlds_TmpFC)->AddField(ipFld_Shape);

	// ObjectID �t�B�[���h
	((IFieldEditPtr)ipFld_OID)->put_Name(CComBSTR("OBJECTID"));
	((IFieldEditPtr)ipFld_OID)->put_Type(esriFieldTypeOID);
	((IFieldsEditPtr)ipFlds_TmpFC)->AddField(ipFld_OID);

	// AddrCode �t�B�[���h
	((IFieldEditPtr)ipFld_AddrCode)->put_Name(CComBSTR("AddrCode"));
	((IFieldEditPtr)ipFld_AddrCode)->put_Type(esriFieldTypeString);
	((IFieldEditPtr)ipFld_AddrCode)->put_Length(16);
	((IFieldEditPtr)ipFld_AddrCode)->put_IsNullable(VARIANT_FALSE);
	((IFieldsEditPtr)ipFlds_TmpFC)->AddField(ipFld_AddrCode);

	// �t�B�[�`���N���X�쐬
	((IFeatureWorkspacePtr)ipWorkspace)->CreateFeatureClass( CComBSTR(mFC_Target), ipFlds_TmpFC, ipUID_ClsID, ipUID_ExtClsID, esriFTSimple, CComBSTR("SHAPE"), NULL, &ipFC_TmpPoints );
	if( NULL == ipFC_TmpPoints )
	{
		cerr << "�t�B�[�`���N���X " << mFC_Target << " �̍쐬�Ɏ��s���܂���" << endl;
		return false;
	}

	// �t�B�[���h�C���f�b�N�X�擾
	if(!GetFieldID(ipFC_TmpPoints,		FD_ADDR_CODE,			&mFID_AddrCode_TP))		return false;

	return true;
}

bool CheckJusyoDist::fnMakeCityList()
{
	IFeatureCursorPtr	ipFCur_CityAdmin;
	IFeaturePtr			ipF_CityAdmin;
	_ICursorPtr		ipCur_CityAdmin;
	IQueryDefPtr	ipQDef_CityAdmin;
	_IRowPtr			ipR_CityAdmin;
	char	aWhere[128], aRange[64], aSubField[64];
	string	aTmpStr;
	sprintf(aWhere, "%s > '00000'", city_admin::kCityCode);
	sprintf(aSubField, "distinct(%s)", city_admin::kCityCode);

	switch(mCLType) {
	case kCLRange:
		sprintf(aRange, " and %s between '%05d' and '%05d'", city_admin::kCityCode, mRangeMin, mRangeMax);
		strcat(aWhere, aRange);
	case kCLAll:
		((IFeatureWorkspacePtr)ipWorkspace)->CreateQueryDef(&ipQDef_CityAdmin);
		ipQDef_CityAdmin->put_Tables(CComBSTR(mFC_CityAdmin));
		ipQDef_CityAdmin->put_SubFields(CComBSTR(aSubField));
		ipQDef_CityAdmin->put_WhereClause(CComBSTR(aWhere));
		ipQDef_CityAdmin->Evaluate(&ipCur_CityAdmin);
		while(ipCur_CityAdmin->NextRow(&ipR_CityAdmin) == S_OK) {
			CComVariant		aVar_CityCode;
			ipR_CityAdmin->get_Value(0, &aVar_CityCode);
			aTmpStr	= ExtractStr(aVar_CityCode);
			if(!mCityList.Add(aTmpStr.c_str())) return false;
		}
		break;
	case kCLList:
		if(!mCityList.Open(mFN_CityList)) return false;
		break;
	case kCLSingle:
		if(!mCityList.Add(mCityCode)) return false;
		break;
	// �Z��8���R�[�h���X�g�w��
	case kAdr8List:
		// �Z��8���R�[�h���X�g�Ǎ���
		if(!mCityList.Open(mAddr8List))
		{
			return false;
		}
		break;
	}

	return true;
}

// �s���E���X�g����
bool CheckJusyoDist::fnMakeACList(const char *cCityCode)
{
	mACSet.clear();

	using namespace sindy::schema;

	CString whereCaluse;
	whereCaluse.Format(_T("%s = '%s' and %s = %d"), 
		city_admin::kCityCode, cCityCode,
		city_admin::kAddrClass, city_admin::addr_class::kResidenceIndicationAndChibanUndeveloped);

	long	aCount;
	IFeatureCursorPtr ipFCur_CityAdmin;
	if(!SearchByWhereClause(whereCaluse, ipFC_CityAdmin, aCount, ipFCur_CityAdmin))
	{
		return false;
	}

	if(aCount < 1)
	{
		cerr << cCityCode << ": �s���E�|���S��������܂���" << endl;
		return false;
	}

	IFeaturePtr	ipF_CityAdmin;
	while( S_OK == ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) && ipF_CityAdmin )
	{
		long lOID = 0;
		ipF_CityAdmin->get_OID( &lOID );

		// �W�E11���R�[�h�̎擾�E���X�g�ւ̒ǉ�
		CComVariant	vaValue;
		string	strAddrCode;
		ipF_CityAdmin->get_Value(mFID_CityCode, &vaValue);
		strAddrCode = (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "";
		vaValue.Clear();

		ipF_CityAdmin->get_Value(mFID_AddrCode, &vaValue);
		strAddrCode += (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "";
		vaValue.Clear();

		mACSet.Add( strAddrCode, lOID );

	}
	return true;
}

bool CheckJusyoDist::fnOpenChkLog()
{
	if( !mChkLog.Open(mFN_ChkLog, FREESTYLELOG) )
	{
		fprintf(stderr, "���O�t�@�C�����J���܂���\n");
		return false;
	}

	// �c�[�����E�o�[�W�����o��
	CVersion cVer;
	char szMsg[100] = {0};
	sprintf_s( szMsg, sizeof(szMsg), "%s FILEVERSION %s PRODUCTVERSION %s", cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
	mChkLog.OutputComment( szMsg );

	// �J�n���������O�֏o��
	mChkLog.OutputTime();

	// �w�b�_���o��
	mChkLog.OutputDirect("FLAG\tLAYER\tOBJECTID\t�|�C���g��\t���\t�Z���R�[�h\t�����n��\t�d�S�o�x\t�d�S�ܓx\t�s���{����\t�s�撬����\t�厚�E����\t���E����\tCOMMENT �n�j|�C��|�ۗ�\tCOMMENT");
	mChkLog.Flush();

	return true;
}

void CheckJusyoDist::getCityAdmin(const IFeaturePtr& ipCityAdmin, AdminAttr& adminAttr, IGeometryPtr& ipCityAdminGeo)
{
	CComVariant	vaValue;

	// CityCode�擾
	ipCityAdmin->get_Value( mFID_CityCode, &vaValue );
	adminAttr.eAddrCode = (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "00000";
	vaValue.Clear();

	// AddrCode �擾
	ipCityAdmin->get_Value(mFID_AddrCode, &vaValue);
	adminAttr.eAddrCode += (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "";
	vaValue.Clear();

	// AddrClass_c �擾
	ipCityAdmin->get_Value(mFID_AddrClassC, &vaValue);
	adminAttr.eAddrClassC = vaValue.lVal;

	// AddrCode2 �擾
	ipCityAdmin->get_Value( mFID_AddrCode2, &vaValue );
	adminAttr.eAddrCode2 = (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "";
	vaValue.Clear();

	// GaikuFugo �擾
	ipCityAdmin->get_Value(mFID_GaikuFugo, &vaValue);
	adminAttr.eGaikuFugo = (VT_I4 == vaValue.vt)? vaValue.lVal : 0;

	// �g���X�敄���擾
	ipCityAdmin->get_Value(mFID_ExtGaikuFugo, &vaValue);
	adminAttr.eExtGaiku = (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "";
	vaValue.Clear();

	// �W�I���g���擾
	ipCityAdmin->get_ShapeCopy( &ipCityAdminGeo );
}

// �w��s�����̍��|�C���g20����
bool CheckJusyoDist::fnAdminProc(const IFeaturePtr& ipFeature )
{
	// �s�s�n�}�s���E�|���S���̑����l���擾
	AdminAttr adminAttr;
	IGeometryPtr ipCityAdminGeo;
	getCityAdmin(ipFeature, adminAttr, ipCityAdminGeo);

	// �ʑ��n�Ȃǂ̓���R�[�h�͖���
	if( adminAttr.eAddrCode.substr(5, 6) == _T("999999") )
	{
		return true;
	}

	// �X�敄���Ȃ��Ő����ς� �� ����20�����͂��Ȃ�
	if( adminAttr.eExtGaiku.empty() && adminAttr.eGaikuFugo == 0 && 
		(adminAttr.eAddrClassC == 1 || adminAttr.eAddrClassC == 2) )
		return true;

	// �g���X�悠��Œ����ږ�����|�n�Ԑ����ς� �� ����20�������Ȃ�
	if( !adminAttr.eExtGaiku.empty() && 
		adminAttr.eAddrClassC == 0 || adminAttr.eAddrClassC == 2 )
		return true;	

	// �w��s�s�n�}�s���E�|���S����ɑ��݂��鍆�|�C���g���擾
	long gouPointCount = 0;
	IFeatureCursorPtr ipGouPointFeaCur;
	if(!SearchByShape(ipCityAdminGeo, esriSpatialRelContains, ipFC_GouPoint, gouPointCount, ipGouPointFeaCur))
	{
		return false;
	}

	if( gouPointCount != 0 )
	{
		IFeaturePtr	ipF_GouPoint;
		while(S_OK == ipGouPointFeaCur->NextFeature(&ipF_GouPoint) && ipF_GouPoint)
		{
			fnGetGPInfo(ipF_GouPoint, adminAttr, true);
		}
	}

	// KP��Ԍ���
	long aKPCount = 0;
	IFeatureCursorPtr	ipFCur_KlinkPoint;
	if(!SearchByShape(ipCityAdminGeo, esriSpatialRelContains, ipFC_KlinkPoint, aKPCount, ipFCur_KlinkPoint))
	{
		return false;
	}

	if( aKPCount == 0 )
	{
		return true;
	}

	IFeaturePtr	ipF_KlinkPoint;
	while(S_OK == ipFCur_KlinkPoint->NextFeature(&ipF_KlinkPoint) && ipF_KlinkPoint)
	{
		if(!fnGetKPInfo(ipF_KlinkPoint, adminAttr))
		{
			continue;
		}
	}

	return true;
}

bool CheckJusyoDist::getGouPoints(const IFeaturePtr& ipFeature )
{
	// �s�s�n�}�s���E�|���S���̑����l���擾
	AdminAttr adminAttr;
	IGeometryPtr ipCityAdminGeo;
	getCityAdmin(ipFeature, adminAttr, ipCityAdminGeo);

	// �ʑ��n�Ȃǂ̓���R�[�h�͖���
	if( adminAttr.eAddrCode.substr(5, 6) == _T("999999") )
	{
		return true;
	}

	// �w��s�s�n�}�s���E�|���S����ɑ��݂��鍆�|�C���g������
	long gouPointCount = 0;
	IFeatureCursorPtr ipGouPointFeaCur;
	if(!SearchByShape(ipCityAdminGeo, esriSpatialRelContains, ipFC_GouPoint, gouPointCount, ipGouPointFeaCur))
	{
		return false;
	}

	// �Y�����鍆�|�C���g��0���̏ꍇ�́A�ȍ~�̏����͍s��Ȃ�
	if( gouPointCount == 0)
	{
		return true;
	}

	// �Y�����鍆�|�C���g�̏����擾
	IFeaturePtr	ipGouPointFea;
	while(S_OK == ipGouPointFeaCur->NextFeature(&ipGouPointFea) && ipGouPointFea)
	{
		fnGetGPInfo(ipGouPointFea, adminAttr, false);
	}

	return true;
}

// ���|�C���g���擾(20����)
void CheckJusyoDist::fnGetGPInfo(const IFeaturePtr& ipFeature, const AdminAttr &cAdmAttr, bool isAddAdr2Point )
{
	CComVariant vaValue;
	AddrTmpRec		aATRec;

	// ���ԍ��̎擾
	ipFeature->get_Value(mFID_GouNo, &vaValue);
	string strGouNo = (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "";

	// ���ԍ���܂���[*]�Ȃ�`�F�b�N�ΏۊO
	if( strGouNo.empty() || strGouNo.find('*') == 0 )
		return;
	vaValue.Clear();

	// �g�����ԍ��t���O�̎擾
	ipFeature->get_Value(mFID_ExpGouNoF, &vaValue);
	bool bExGouF = (vaValue.vt != VT_NULL && vaValue.lVal == 1);

	// �Z��������ʃR�[�h�̎擾
	aATRec.eAddrClass	= cAdmAttr.eAddrClassC;

	// ObjectID �擾
	ipFeature->get_OID(&aATRec.ePointID);

	// [Bug6516]20������
	CreateAddrCode( cAdmAttr, strGouNo, bExGouF, aATRec );
	if( aATRec.eAddrCode.length() == 20 )
	{
		// ���^�C�v�̎擾
		ipFeature->get_Value(mFID_GouType, &vaValue);
		aATRec.eGouType = vaValue.lVal;

		// ���W�̎擾
		IGeometryPtr ipG_GouPoint;
		ipFeature->get_Shape(&ipG_GouPoint);
		((IPointPtr)ipG_GouPoint)->QueryCoords(&aATRec.eLongitude, &aATRec.eLatitude);

		// �ǉ�
		mAddrTmpSet.insert(aATRec);

		// CITY_ADMIN��ADDRCODE2�̒l��isAddAdr2Point�̒l���݂āA�ȍ~�̏����̎��{�L���𔻒f
		// �iisAddAdr2Point�l�͎��s���[�h�ɂ��قȂ�j
		if( cAdmAttr.eAddrCode2 != "000000" && isAddAdr2Point )
		{
			aATRec.eAddrCode = aATRec.eAddrCode.substr(0, 5) + cAdmAttr.eAddrCode2.c_str() + aATRec.eAddrCode.substr(11, 9);
			mAddrTmpSet.insert(aATRec);
		}
	}
}

// [Bug6516]20�������֐�
void CheckJusyoDist::CreateAddrCode( const AdminAttr& cAdmAttr, const string& strGouNo, bool bExGouF, AddrTmpRec& rRec )
{
	static CTmpCodeCreator cTmpCodeCreator;	//!< [Bug 6516]�ꎞ�R�[�h�쐬�e�[�u��

	// �ŏ��ɕ����t���Z���ɂȂ邩�Ȃ�Ȃ����ŏꍇ�킯(16�����ʏ�E�����t���ňقȂ�P�[�X�́A�����t����D�悷���)
	bool bCharChibanDomariF = false;	// �����t���n�ԂŒn�Ԏ~�܂�Z�����H
	std::string strCharPntName;	// �����t���Z���n�Ԗ��̊i�[�p

	// ���ԍ���[-]�ŕ������Ă���
	std::list<CString> listToken;
	CString strData( strGouNo.c_str() );
	strData.Replace( "#", "" );	// �擪#�͏���
	int iCurPos = 0;
	CString strToken( strData.Tokenize( "-", iCurPos ) );
	while( !strToken.IsEmpty() )
	{
		listToken.push_back( strToken );
		strToken = strData.Tokenize( "-", iCurPos );
	}

	// CITY_ADMIN�̊X�敄�������ݒ�inull�j�̏ꍇ
	if( cAdmAttr.eGaikuFugo == 0 )
	{

		// �g���X�敄�������ݒ�inull�j�ŁA���ԍ��̒l���u#�v�{���l�ȊO�̒l�ł���ꍇ
		if( cAdmAttr.eExtGaiku.empty() && strGouNo.find('#') == 0 && !IsNumStr(*(listToken.begin())) )	// �����n�� = ���ԍ�����
		{
			strCharPntName = *(listToken.begin());
			bCharChibanDomariF = (listToken.size() == 1);
		}
		else
		{
			if( !bExGouF )	// �g�����ԍ�OFF �����n�� = �X�敄��
			{
				if( !cAdmAttr.eExtGaiku.empty() )
				{
					// [bug11616]�Ή�
					// [Bug6516�ďC��]�ł́A�g���X�敄���ieExtGaiku�j�{���ԍ��i�̃n�C�t�����O�̒l�j�ŁA
					// �����t���n�Ԗ��̂𐶐����Ă������A�����t���Z���f�[�^�̐������W�b�N���݂邩����A
					// �����t���n�Ԗ��̂ɂ͊g���X�敄���̂ݎg�p��������K�؂Ȃ��߁A�C��
					strCharPntName = cAdmAttr.eExtGaiku;
					bCharChibanDomariF = false;
				}
			}
			else if( !IsNumStr(*(listToken.begin())) )	// �g�����ԍ�ON  �����n�� = ���ԍ�����
			{
				strCharPntName = *(listToken.begin());
				bCharChibanDomariF = (listToken.size() == 1);
			}
		}
	}
	else // CITY_ADMIN�̊X�敄���ɉ�������̒l������ꍇ
	{
		// �g�����ԍ��t���O��True�ŁA���ԍ��̒l���u#�v�{���l�ȊO�̒l�ł���ꍇ
		if(bExGouF && strGouNo.find('#') == 0 && !IsNumStr(*(listToken.begin())))
		{
			strCharPntName = *(listToken.begin());
			bCharChibanDomariF = (listToken.size() == 1);
		}
	}

	if( !strCharPntName.empty() )
	{
		// �����t���̍ہA�[���R�[�h�𔭍s����(�n�Ԏ~�܂�łȂ��Ƃ��́A��4����9999�ɂ���)
		std::string strPntCode = cTmpCodeCreator.GetTmpCode( cAdmAttr.eAddrCode, strCharPntName );
		std::string strGouCode = bCharChibanDomariF? "0000" : "9999";
		rRec.eCharPntName = strCharPntName;
		rRec.eAddrCode = (!strPntCode.empty())? cAdmAttr.eAddrCode + strPntCode + strGouCode : "";
	}
	else
	{
		long	aCode1 = 0, aCode2 = 0;
		char* p = strpbrk( (char*)strGouNo.c_str(), "0123456789-");				// �܂��A������ or �n�C�t����T��
		if( p )
		{								// �����i�Ȃ���ΏI���j
			if( *p == '-' )
				return;				// �n�C�t����������|�C���g�Ƃ��Ď擾���Ȃ�
			else
			{
				aCode1 = atoi(p);					// ���l���@
				p = strchr(p, '-');					// �n�C�t����T��
				if( p )
				{						// �����i�Ȃ���ΏI���j
					p = strpbrk(p, "0123456789");	// �������T��
					if( p )
						aCode2 = atoi(p);			// ���l���A
				}
			}
		}

		long	aChiban, aBango;
		if(!bExGouF && cAdmAttr.eGaikuFugo > 0) {	// �X�敄������
			aChiban	= cAdmAttr.eGaikuFugo;
			aBango	= aCode1;
			if( aBango > 9999 ) return;
		} else {						// �X�敄���Ȃ�
			aChiban	= aCode1;
			aBango	= aCode2;
			if(aChiban == 0 || aChiban > 99999 || aBango > 9999) return;
		}

		char szTmpCode[21] = "";
		sprintf_s( szTmpCode, sizeof(szTmpCode), "%s%05d%04d", cAdmAttr.eAddrCode.c_str(), aChiban, aBango );
		rRec.eAddrCode = szTmpCode;
	}
}

// �ƌ`�����N���擾
bool CheckJusyoDist::fnGetKPInfo(const IFeaturePtr& ipF_KlinkPoint, const AdminAttr& cAdmAttr)
{
	static string	aACOrg, aACNew;

	CComVariant		aTmpVar;
	AddrTmpRec		aATRec;
	
	// ���񋟌��R�[�h���u1�v�i�ƌ`�����N�f�[�^�j�ȊO�͖���
	ipF_KlinkPoint->get_Value(mFID_InfoSrcC, &aTmpVar);
	if(aTmpVar.lVal != 1) return false;

	// ���͗p���W���x��ʃR�[�h���u1�v�u2�v�u4�v�u5�v�u6�v�u7�v�ȊO�͖���
	ipF_KlinkPoint->get_Value(mFID_InputSeidoC, &aTmpVar);
	switch(aTmpVar.lVal) {
	case 1: case 2: case 4: case 5: case 6: case 7:
		break;
	default:
		return false;
	}

	// �Z��������ʃR�[�h�̎擾
	aATRec.eAddrClass	= cAdmAttr.eAddrClassC;

	// ObjectID �擾
	ipF_KlinkPoint->get_OID(&aATRec.ePointID);

	// �Z���R�[�h(20��)�̎擾
	char	aAddrCode[23];
	ipF_KlinkPoint->get_Value(mFID_AddrCode_KP, &aTmpVar);
	aATRec.eAddrCode = ExtractStr(aTmpVar);
	aTmpVar.Clear();

	aATRec.eAddrCode	= aATRec.eAddrCode.substr(0, 20);

	if(aATRec.eAddrCode.substr(11, 5) == "00000") return false;

	// 11���R�[�h���ŐV�R�[�h�ɕύX
	if(aATRec.eAddrCode.substr(0, 11) == aACOrg) {
		aATRec.eAddrCode	= aACNew + aATRec.eAddrCode.substr(11, 9);
	} else {
		aACOrg	= aATRec.eAddrCode.substr(0, 11);
		mJDBAccess.GetNewestCode(aATRec.eAddrCode.c_str(), aAddrCode);
		aATRec.eAddrCode	= aAddrCode;
		aACNew	= aATRec.eAddrCode.substr(0, 11);
	}

	if("900" < cAdmAttr.eAddrCode.substr(8, 3)) {
		// 11�����L���Ȃ�g�p
		bool	aValid;
		mJDBAccess.IsValid(aATRec.eAddrCode.substr(0, 11).c_str(), &aValid);
		if(!aValid) return false;
	} else {
		if("000" < cAdmAttr.eAddrCode.substr(8, 3) && cAdmAttr.eAddrCode.substr(8, 3) < "100") {
			// 11����v�Ȃ�g�p
			if(aATRec.eAddrCode.substr(0, 11) != cAdmAttr.eAddrCode) return false;
		} else {
			// �W����v�Ȃ�g�p
			if(aATRec.eAddrCode.substr(0, 8) != cAdmAttr.eAddrCode.substr(0, 8)) return false;
		}
	}

	// ���^�C�v
	aATRec.eGouType		= 3;

	// ���W�̎擾
	IGeometryPtr ipG_KlinkPoint;
	ipF_KlinkPoint->get_Shape(&ipG_KlinkPoint);
	((IPointPtr)ipG_KlinkPoint)->QueryCoords(&aATRec.eLongitude, &aATRec.eLatitude);

	// �ǉ�
	mAddrTmpSet.insert(aATRec);

	if( cAdmAttr.eAddrCode2 != "000000" ) {
		aATRec.eAddrCode = aATRec.eAddrCode.substr(0, 5) + cAdmAttr.eAddrCode2.c_str() + aATRec.eAddrCode.substr(11, 9);
		mAddrTmpSet.insert(aATRec);
	}

	return true;
}

// CS�|�C���g���擾
void CheckJusyoDist::fnCSAddrProc(const IFeaturePtr& ipFeature )
{
	AddrTmpRec		aATRec;

	// OID�̎擾
	ipFeature->get_OID(&aATRec.ePointID);

	aATRec.eGouType		= 0;

	// �Z���R�[�h(20��)�̎擾
	CComVariant vaValue;
	ipFeature->get_Value(mFID_AddrCode_CS, &vaValue);
	aATRec.eAddrCode = (VT_BSTR == vaValue.vt)? CW2A(vaValue.bstrVal) : "";
	vaValue.Clear();

	// ���W�̎擾
	IGeometryPtr	ipG_CSAddrPoint;
	ipFeature->get_Shape(&ipG_CSAddrPoint);
	((IPointPtr)ipG_CSAddrPoint)->QueryCoords(&aATRec.eLongitude, &aATRec.eLatitude);

	mAddrTmpSet.insert( aATRec );	// �ǉ�
}

void CheckJusyoDist::fnConvDataSet16()
{
	// ��������𖞂����Z���|�C���g�����O
	excludingPoints(true);

	// �Z��16���R�[�h�ŃO���[�v��
	makeAddrcodeGroupsByAdr16();

}

void CheckJusyoDist::fnConvDataSet8Chiban()
{
	// ��������𖞂����Z���|�C���g�����O
	excludingPoints(false);

	// �Z��8���R�[�h�{�n�ԃR�[�h�i�����t���Z���̏ꍇ�́A�Z��8���R�[�h�{�n�Ԗ��́j�ŃO���[�v��
	makeAddrcodeGroupsByAdr8Chiban();

}

void CheckJusyoDist::excludingPoints(bool isExcludeKakeiPoint)
{
	// CSP/GOU�ɓ���20�������݂���KLP/TPG(-t�w�莞)�����O
	AddrTmpSet::iterator aATItr;
	string	aACTmp;
	long	aGTTmp = 0;
	for( aATItr = mAddrTmpSet.begin(); aATItr != mAddrTmpSet.end(); aATItr++ )
	{
		if( aACTmp != (*aATItr).eAddrCode )
		{
			aACTmp	= (*aATItr).eAddrCode;
			aGTTmp	= (*aATItr).eGouType;
			
			continue;
		}

		if( aGTTmp < (*aATItr).eGouType )
		{
			// �ƌ`�����N�|�C���g�����O�ΏۂƂ��邩�ǂ����́AisExcludeKakeiPoint����
			if( (mIgnoreTPG && (*aATItr).eGouType == 2) || ( (*aATItr).eGouType == 3 && isExcludeKakeiPoint ) )	// TPG(-t�w�莞) or KLP
			{
				AddrTmpSet::iterator	aATTmp = aATItr;
				aATItr--;
				mAddrTmpSet.erase(aATTmp);
			}
		}
	}

	// CSP/GOU�ɓ���16�������݂���n�Ԏ~�܂�TPG�����O(-t�w�莞)
	if( !mIgnoreTPG )
		return;

	AddrTmpSet::iterator	aATItr1, aATItr2;
	for( aATItr1 = mAddrTmpSet.begin(); aATItr1 != mAddrTmpSet.end(); aATItr1++ )
	{
		if( (*aATItr1).eAddrCode.substr(16, 4) == "0000" && (*aATItr1).eGouType == 2 )	// �n�Ԏ~�܂聕TPG
		{
			for( aATItr2 = aATItr1, aATItr2++; aATItr2 != mAddrTmpSet.end() && (*aATItr1).eAddrCode.substr(0, 16) == (*aATItr2).eAddrCode.substr(0, 16); aATItr2++ )
			{
				if( (*aATItr2).eGouType < 2 )
				{
					AddrTmpSet::iterator	aATTmp = aATItr1;
					if( aATItr1 == mAddrTmpSet.begin() )
					{
						mAddrTmpSet.erase(aATTmp);
						aATItr1 = mAddrTmpSet.begin();
					}
					else
					{
						aATItr1--;
						mAddrTmpSet.erase(aATTmp);
					}
					break;
				}
			}
		}
	}
}

void CheckJusyoDist::makeAddrcodeGroupsByAdr16()
{
	// �O���[�v���O�Ƀ��X�g����ɂ���
	mAddrList.clear();

	AddrTmpSet::iterator aATItr = mAddrTmpSet.begin();
	while( aATItr != mAddrTmpSet.end() )
	{
		if( (*aATItr).eAddrClass != 3 )	// �����ς� �� ����
		{
			aATItr++;
			continue;
		}

		AddrRec	aAddrRec;
		aAddrRec.eAddrCode	= aATItr->eAddrCode.substr(0, 16);
		aAddrRec.eCharPntName = aATItr->eCharPntName;
		while( aATItr != mAddrTmpSet.end() && aAddrRec.eAddrCode == (*aATItr).eAddrCode.substr(0, 16) )
		{
			PointRec	aPtRec;
			aPtRec.ePointID		= (*aATItr).ePointID;
			aPtRec.eLongitude	= (*aATItr).eLongitude;
			aPtRec.eLatitude	= (*aATItr).eLatitude;
			aPtRec.eGouType		= (*aATItr).eGouType;
			aAddrRec.ePoints.push_back(aPtRec);
			aATItr++;
		}

		mAddrList.push_back(aAddrRec);
	}
}

void CheckJusyoDist::makeAddrcodeGroupsByAdr8Chiban()
{
	// �O���[�v���O�Ƀ��X�g����ɂ���
	mAddrList.clear();

	for(const auto& addrTmpRec : mAddrTmpSet)
	{
		// �u�Z��8���R�[�h�{�n�ԁi�ʏ�Z���F�n�ԃR�[�h�A�����t���n�ԁF�����t���n�Ԗ��́j�v�̒P�ʂŃO���[�v�����s�Ȃ��d�l�����A
		// ������́A�Z��16���R�[�h�{�n�Ԗ��̂̒P�ʂŃO���[�v������悤�ɂ��������߁A
		// �@�E�����R�[�h�i�Z���R�[�h��8�`11���ځj�͈ꗥ�u000�v
		// �@�E�����t���Z���̏ꍇ�A�n�ԃR�[�h�͈ꗥ�uA0000�v�A�����t���Z���łȂ��ꍇ�́A���X�̒n�ԃR�[�h�̂܂�
		// �Ƃ�����ŁA�O���[�v���̏������s�Ȃ�

		AddrRec tempAddrRec;

		// �Z��11���R�[�h���܂��i�[�i���̍ہA�����R�[�h�i�Z���R�[�h��8�`11���ځj�͈ꗥ�u000�v�ɂ���j
		tempAddrRec.eAddrCode = addrTmpRec.eAddrCode.substr(0, 8) + _T("000");

		// �n�ԃR�[�h�ɂ��āA�����t���Z���̏ꍇ�i�����t���n�Ԗ��̂���̏ꍇ�j�A
		// ���X�i�[����Ă���n�ԃR�[�h���A�����łȂ���΁uA0000�v���i�[
		tempAddrRec.eAddrCode += (addrTmpRec.eCharPntName.empty()) ? addrTmpRec.eAddrCode.substr(11, 5) : _T("A0000");

		// �����t���n�Ԗ��̂͌��X�̒l�����̂܂܊i�[
		tempAddrRec.eCharPntName = addrTmpRec.eCharPntName;

		PointRec pointRec;
		pointRec.ePointID = addrTmpRec.ePointID;
		pointRec.eLongitude = addrTmpRec.eLongitude;
		pointRec.eLatitude = addrTmpRec.eLatitude;
		pointRec.eGouType = addrTmpRec.eGouType;

		// �����Ώۂ̏Z���|�C���g�iaddrTmpRec�j���O���[�v�����X�g�imAddrList�j�ɑ��݂��邩����
		// �����Ƃ��ẮA����16���R�[�h�{�����t���n�Ԗ��̂̒l������AddrRec�����݂��邩��T���`�ƂȂ�
		auto& itr = find(mAddrList.begin(), mAddrList.end(), tempAddrRec);

		// ���݂��Ȃ���΁A�V�K�O���[�v�iAddrRec�j�Ƃ��ă��X�g�ɒǉ�
		if(itr == mAddrList.end())
		{
			tempAddrRec.ePoints.push_back(pointRec);
			mAddrList.push_back(tempAddrRec);

			continue;
		}

		// ���݂���΁A����mAddrList�ɒǉ��ς݂̃O���[�v�iAddrRec�j��ePoints�֒ǉ�
		itr->ePoints.push_back(pointRec);
	}
}

void CheckJusyoDist::fnCheckDataSet16()
{
	AddrList::iterator	aALItr;
	for( aALItr = mAddrList.begin(); aALItr != mAddrList.end(); aALItr++ )
	{
		// �O���[�v���X�g�̍쐬
		GroupList	aGList;
		for( PointList::iterator aPItr1 = (*aALItr).ePoints.begin(); aPItr1 != (*aALItr).ePoints.end(); aPItr1++ )
		{
			GroupList::iterator	aGItrTmp = aGList.end();
			for( GroupList::iterator	aGItr = aGList.begin(); aGItr != aGList.end(); aGItr++ )
			{
				for( PointList::iterator aPItr2 = (*aGItr).ePoints.begin(); aPItr2 != (*aGItr).ePoints.end(); aPItr2++ )
				{
					// �u�ߗׁv�|�C���g���ǂ����̃`�F�b�N
					double	aDist = mCConv.GetDist((*aPItr1).eLongitude, (*aPItr1).eLatitude, (*aPItr2).eLongitude, (*aPItr2).eLatitude);

					// �u�ߗׁv�ł���
					if(aDist < mDistNbr)
					{
						if( aGItrTmp != aGList.end() )	// ���ɑ��̃O���[�v�ɑ����Ă���
						{
							(*aGItrTmp).ePoints.splice((*aGItrTmp).ePoints.end(), (*aGItr).ePoints);
							aGItr = aGList.erase(aGItr);
							aGItr--;
						}
						else	// �܂����̃O���[�v�ɑ����Ă��Ȃ�
						{	
							(*aGItr).ePoints.push_back(*aPItr1);
							aGItrTmp = aGItr;
						}
						break;
					}
				}
			}

			// �u�ߗׁv���Ȃ�����
			if( aGItrTmp == aGList.end() )
			{
				GroupRec	aGRec;
				aGRec.ePoints.push_back(*aPItr1);
				aGList.push_back(aGRec);
			}
		}

		// �O���[�v����� �� �����`�F�b�N�̕K�v�Ȃ�
		if(aGList.size() == 1) continue;

		// �O���[�v���ő�\�_������
		for( GroupList::iterator	aGItr = aGList.begin(); aGItr != aGList.end(); aGItr++ )
		{
			double	aMinDist = DBL_MAX;
			PointList::iterator	aPItrTmp = (*aGItr).ePoints.begin();
			for( PointList::iterator aPItr1 = (*aGItr).ePoints.begin(); aPItr1 != (*aGItr).ePoints.end(); aPItr1++ )
			{
				double	aTotalDist = 0;
				for( PointList::iterator aPItr2 = (*aGItr).ePoints.begin(); aPItr2 != (*aGItr).ePoints.end(); aPItr2++ )
				{
					aTotalDist += mCConv.GetDist((*aPItr1).eLongitude, (*aPItr1).eLatitude, (*aPItr2).eLongitude, (*aPItr2).eLatitude);
				}
				if( aTotalDist < aMinDist )
				{
					aMinDist	= aTotalDist;
					aPItrTmp	= aPItr1;
				}
			}
			(*aGItr).eRepPoint	= *aPItrTmp;
		}

		// �O���[�v�Ԃŋ����`�F�b�N
		bool aErr = false;
		for( GroupList::iterator	aGItr1 = aGList.begin(); aGItr1 != aGList.end(); aGItr1++ )
		{
			GroupList::iterator	aGItr2;
			for( aGItr2 = aGItr1, aGItr2++; aGItr2 != aGList.end(); aGItr2++ )
			{
				double	aMinDist = DBL_MAX;
				for( PointList::iterator aPItr1 = (*aGItr1).ePoints.begin(); aPItr1 != (*aGItr1).ePoints.end(); aPItr1++ )
				{
					for( PointList::iterator aPItr2 = (*aGItr2).ePoints.begin(); aPItr2 != (*aGItr2).ePoints.end(); aPItr2++ )
					{
						double	aDist = mCConv.GetDist((*aPItr1).eLongitude, (*aPItr1).eLatitude, (*aPItr2).eLongitude, (*aPItr2).eLatitude);
						if(aDist < aMinDist)	aMinDist = aDist;
					}
				}
				if( aMinDist >= mDistErr )
				{
					aErr	= true;
					break;
				}
			}
			if(aErr) break;
		}

		if(aErr) {

			// �}���`�|�C���g�̍쐬
			int		aNum = 0;
			double	aSumX = 0, aSumY = 0;
			IMultipointPtr	ipMPt_TmpPoints(CLSID_Multipoint);
			for(PointList::iterator aPItr1 = (*aALItr).ePoints.begin(); aPItr1 != (*aALItr).ePoints.end(); aPItr1++) {
				IPointPtr	ipPt_Tmp(CLSID_Point);
				ipPt_Tmp->PutCoords((*aPItr1).eLongitude, (*aPItr1).eLatitude);
				aSumX	+= (*aPItr1).eLongitude;
				aSumY	+= (*aPItr1).eLatitude;
				aNum++;
				((IPointCollectionPtr)ipMPt_TmpPoints)->AddPoint(ipPt_Tmp);
			}

			// �d�S�̎Z�o
			double	aX = aSumX / aNum; 
			double	aY = aSumY / aNum;

			// �o�͗p�t�B�[�`���J�[�\���擾
			IFeatureCursorPtr	ipFCur_TmpPoints;
			ipFC_TmpPoints->Insert(VARIANT_TRUE, &ipFCur_TmpPoints);

			// �t�B�[�`������
			IFeatureBufferPtr	ipFBuf_TmpPoints;
			ipFC_TmpPoints->CreateFeatureBuffer(&ipFBuf_TmpPoints);

			ipFBuf_TmpPoints->putref_Shape(ipMPt_TmpPoints);

			std::string tmp((*aALItr).eAddrCode);
			CComVariant	aVar_AddrCode = (*aALItr).eAddrCode.c_str();
			ipFBuf_TmpPoints->put_Value(mFID_AddrCode_TP, aVar_AddrCode);

			// �Z���R�[�h����Z�����̎擾�iBug8609�Ή��j
			CString	KenKanji, ShiKanji, OazaKanji, AzaKanji;
			mJDBAccess.CodeToKanji( aALItr->eAddrCode.c_str(), KenKanji.GetBuffer( KEN_KANJI_MAX_LEN ), ShiKanji.GetBuffer( SHI_KANJI_MAX_LEN ), 
										OazaKanji.GetBuffer( OAZA_KANJI_MAX_LEN ), AzaKanji.GetBuffer( AZA_KANJI_MAX_LEN ) );

			// �t�B�[�`���ǉ�
			char szMessage[1024] = "";
			CComVariant	aOID_TP;
			if(FAILED(ipFCur_TmpPoints->InsertFeature(ipFBuf_TmpPoints, &aOID_TP)) || FAILED(ipFCur_TmpPoints->Flush())) {
				sprintf_s(szMessage, sizeof(szMessage), "0\t\t\t%d\t����16���|�C���g�Q�����U(���U�|�C���g�t�B�[�`���̍쐬�Ɏ��s)\t%s\t%s\t%16.12f\t%15.12f\t%s\t%s\t%s\t%s\t\t",
					aNum, 
					aALItr->eCharPntName.empty()? aALItr->eAddrCode.c_str() : aALItr->eAddrCode.substr(0, 11).c_str(),
					aALItr->eCharPntName.c_str(), aX, aY,
					KenKanji, ShiKanji, OazaKanji, AzaKanji);
			} else {
				sprintf_s(szMessage, sizeof(szMessage), "0\t%s\t%ld\t%d\t����16���|�C���g�Q�����U\t%s\t%s\t%16.12f\t%15.12f\t%s\t%s\t%s\t%s\t\t",
					mFC_Target, aOID_TP.lVal, aNum, 
					aALItr->eCharPntName.empty()? aALItr->eAddrCode.c_str() : aALItr->eAddrCode.substr(0, 11).c_str(),
					aALItr->eCharPntName.c_str(), aX, aY,
					KenKanji, ShiKanji, OazaKanji, AzaKanji);
			}
			mChkLog.OutputDirect(szMessage);
		}
	}
}

// 8���R�[�h�G���A�ŃL���b�V���쐬
bool CheckJusyoDist::fnCreateCache8(const string &cAddrCode)
{

	CString whereClause;
	whereClause.Format(_T("%s = '%s' and %s like '%s___'"),
								city_admin::kCityCode, cAddrCode.substr(0, 5).c_str(),
								city_admin::kAddrCode, cAddrCode.substr(5, 3).c_str());

	long	aCount;
	IFeatureCursorPtr ipFCur_CityAdmin;
	if(!SearchByWhereClause(whereClause, ipFC_CityAdmin, aCount, ipFCur_CityAdmin))
	{
		return false;
	}

	if(aCount < 1)
	{
		cerr << cAddrCode << ": �s���E�|���S��������܂���" << endl;
		return false;
	}

	double	aXMin = 180, aYMin = 180, aXMax = -180, aYMax = -180;

	IFeaturePtr		ipF_CityAdmin;
	while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK) {
		double	aX1, aY1, aX2, aY2;
		IEnvelopePtr	ipE_CityAdmin;
		ipF_CityAdmin->get_Extent(&ipE_CityAdmin);
		ipE_CityAdmin->QueryCoords(&aX1, &aY1, &aX2, &aY2);
		if(aX1 < aXMin) aXMin = aX1;
		if(aY1 < aYMin) aYMin = aY1;
		if(aX2 > aXMax) aXMax = aX2;
		if(aY2 > aYMax) aYMax = aY2;
	}

	IEnvelopePtr	ipE_CityAdmin8(CLSID_Envelope);
	ipE_CityAdmin8->PutCoords(aXMin, aYMin, aXMax, aYMax);
	ipE_CityAdmin8->Expand(0.001, 0.001, VARIANT_FALSE);
	ipSpCacheMgr->FillCache(ipE_CityAdmin8);

	return true;
}

bool CheckJusyoDist::Execute(int cArgc, char **cArgv)
{
	if( !fnAnalyzeArg(cArgc, cArgv) || !fnGetEnvVar() )
		return false;

	// GeoDB�֐ڑ�
	if(!Connect(mDBCon_Address))
	{
		return false;
	}

	// �t�B�[�`���N���X�擾
	if((ipFC_GouPoint		= GetFeatureClass(_T(mFC_GouPoint)))	== NULL) return false;
	if((ipFC_CityAdmin		= GetFeatureClass(_T(mFC_CityAdmin)))	== NULL) return false;
	if((ipFC_CSAddrPoint	= GetFeatureClass(_T(mFC_CSAddrPoint)))	== NULL) return false;
	if((ipFC_KlinkPoint		= GetFeatureClass(_T(mFC_KlinkPoint)))	== NULL) return false;

	// �t�B�[���h�C���f�b�N�X�擾
	if(!GetFieldID(ipFC_CityAdmin,		city_admin::kCityCode,			&mFID_CityCode))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		city_admin::kAddrCode,			&mFID_AddrCode))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		city_admin::kGaikuFugo,			&mFID_GaikuFugo))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		city_admin::kExtGaikuFugo,		&mFID_ExtGaikuFugo))	return false;
	if(!GetFieldID(ipFC_CityAdmin,		city_admin::kAreaClass,			&mFID_AreaClassC))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		city_admin::kAddrClass,			&mFID_AddrClassC))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		city_admin::kAddrCode2,			&mFID_AddrCode2))		return false;
	if(!GetFieldID(ipFC_GouPoint,		gou_point::kGouNo,				&mFID_GouNo))			return false;
	if(!GetFieldID(ipFC_GouPoint,		gou_point::kGouType,			&mFID_GouType))			return false;
	if(!GetFieldID(ipFC_GouPoint,		gou_point::kPriority,			&mFID_PriorityF))		return false;
	if(!GetFieldID(ipFC_GouPoint,		gou_point::kExpGouNo,			&mFID_ExpGouNoF))		return false;
	if(!GetFieldID(ipFC_KlinkPoint,		klink_point::kInfoSrc,			&mFID_InfoSrcC))		return false;
	if(!GetFieldID(ipFC_KlinkPoint,		klink_point::kInputSeido,		&mFID_InputSeidoC))		return false;
	if(!GetFieldID(ipFC_KlinkPoint,		klink_point::kAddrCodeOrg,		&mFID_AddrCode_KP))		return false;
	if(!GetFieldID(ipFC_CSAddrPoint,	cs_addr_point::kAddrCode,		&mFID_AddrCode_CS))		return false;

	// �Z��DB�ւ̐ڑ�
	if( !mJDBAccess.Connect() )
	{
		cerr << "�Z��DB�ւ̐ڑ��Ɏ��s" << endl;
		return false;
	}

	// �s�撬�����X�g�̐���
	cerr << "�s�撬�����X�g�쐬��...";
	if(!fnMakeCityList())
	{
		return false;
	}
	cerr << "����" << endl;

	// �`�F�b�N���ʊi�[��t�B�[�`���N���X�쐬
	if(!fnCreateTmpFC()) return false;

	// ���O�t�@�C���̃I�[�v��
	if(!fnOpenChkLog()) return false;

	ipSpCacheMgr = ipWorkspace;

	// ��O�擾����
	try
	{
		// 5�����s���[�h
		if( m_iExeMode & ON_FiveCode )
		{
			// �s�撬���R�[�h���X�g�̃��[�v
			for(const auto& city : mCityList)
			{
				cerr << city.eCityCode << ": �`�F�b�N��..." << endl;

				// �W�E11�����X�g�̍쐬�^�s���E�̃`�F�b�N
				if(!fnMakeACList(city.eCityCode))
				{
					continue;
				}
				mChkLog.Flush();

				// �W�����X�g�̃��[�v
				for(const auto& ac8Rec : mACSet)
				{
					cerr << ac8Rec.eAC8 << ": �`�F�b�N��..." << endl;

					// �L���b�V���쐬
					if(!fnCreateCache8(ac8Rec.eAC8))
					{
						continue;
					}

					// 11�����X�g�̃��[�v
					for(const auto& ac11Rec: ac8Rec.eAC11Set)
					{
						cerr << ac11Rec.eAC11 << ": �`�F�b�N��..." << endl;

						// �s���E�|���S���̃��[�v
						for(const auto& oid : ac11Rec.eOIDSet)
						{
							IFeaturePtr		ipF_CityAdmin;
							ipFC_CityAdmin->GetFeature(oid, &ipF_CityAdmin);
							fnAdminProc( ipF_CityAdmin );
						}

						// CS�|�C���g�̌���
						CString addrcode(ac11Rec.eAC11.c_str());
						if(!getCSAddrPoints(addrcode))
						{
							return false;
						}

						fnConvDataSet16();		// 16���P�ʂŃf�[�^�\����ϊ�
						fnCheckDataSet16();		// 16���P�ʂŃ|�C���g���m�̋����`�F�b�N

						mAddrTmpSet.clear();
					}
				}
			}
		}
		// [Bug 7210] ���X�g�w���11���Ή����ė~���� �ɑΉ� 
		else if ( m_iExeMode & ON_ElevenCode )
		{
			// 11���R�[�h���X�g�̃��[�v
			for(const auto& city : mCityList)
			{
				mAddrTmpSet.clear();

				cerr << city.eCityCode << city.eAddrCode << ": �`�F�b�N��..." << endl;

				using namespace sindy::schema;

				CString whereClause;
				whereClause.Format(_T("%s = '%s' and %s = '%s' and %s = %d"), 
					city_admin::kCityCode, city.eCityCode,
					city_admin::kAddrCode, city.eAddrCode,
					city_admin::kAddrClass, city_admin::addr_class::kResidenceIndicationAndChibanUndeveloped);

				IFeatureCursorPtr ipFCur_CityAdmin;
				long adminCount = 0;
				if(!SearchByWhereClause(whereClause, ipFC_CityAdmin, adminCount, ipFCur_CityAdmin))
				{
					continue;
				}

				if(adminCount < 1)
				{
					cerr << "�s���E�|���S��������܂���" << endl;
					continue;
				}

				// �擾�����s�s�n�}�s���E�|���S����ɑ��݂��鍆�|�C���g�f�[�^���擾
				IFeaturePtr	ipF_CityAdmin;
				while(S_OK == ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) && ipF_CityAdmin)
				{
					if(!fnAdminProc( ipF_CityAdmin ))
					{
						continue;
					}
				}

				CString addrcode;
				addrcode.Format(_T("%s%s"), city.eCityCode, city.eAddrCode);
				if(!getCSAddrPoints(addrcode))
				{
					continue;
				}

				fnConvDataSet16();		// 16���P�ʂŃf�[�^�\����ϊ�
				fnCheckDataSet16();		// 16���P�ʂŃ|�C���g���m�̋����`�F�b�N
			}
		}
		else if(m_iExeMode & ON_Addrcode8)
		{
			// 8���R�[�h���X�g�̃��[�v
			for(const auto city : mCityList)
			{
				mAddrTmpSet.clear();

				cerr << city.eCityCode << city.eAddrCode << ": �`�F�b�N��..." << endl;

				using namespace sindy::schema;

				// �����Ώۂ̏Z��8���R�[�h�ɍ��v����s�s�n�}�s���E�|���S�����擾
				// ���̍ہAADDRCODE�����łȂ��AADDRCODE2�J�����������̃L�[�Ƃ��Ďw�肷��
				CString whereClause;
				whereClause.Format(_T("%s = '%s' and (%s like '%s%%' or %s like '%s%%') and %s <> %d"), 
					city_admin::kCityCode, city.eCityCode,
					city_admin::kAddrCode, city.eAddrCode,
					city_admin::kAddrCode2, city.eAddrCode,
					city_admin::kAddrClass, city_admin::addr_class::kAzaUndeveloped);

				IFeatureCursorPtr ipCityAdminFeaCur;
				long adminCount = 0;
				if(!SearchByWhereClause(whereClause, ipFC_CityAdmin, adminCount, ipCityAdminFeaCur))
				{
					continue;
				}

				// �����ɍ��v����s�s�n�}�s���E�|���S�������݂��Ȃ��ꍇ�́A�ȍ~�̏������X�L�b�v
				if(adminCount < 1)
				{
					cerr << "���v����s�s�n�}�s���E�|���S��������܂���" << endl;
					continue;
				}

				// �擾�����s�s�n�}�s���E�|���S����ɑ��݂��鍆�|�C���g�f�[�^���擾
				IFeaturePtr	ipCityAdminFea;
				while(S_OK == ipCityAdminFeaCur->NextFeature(&ipCityAdminFea) && ipCityAdminFea)
				{
					if(!getGouPoints( ipCityAdminFea ))
					{
						continue;
					}
				}

				// �����Ώۂ̏Z��8���R�[�h�ɕR�t��CS�|�C���g�̎擾
				CString addrcode;
				addrcode.Format(_T("%s%s"), city.eCityCode, city.eAddrCode);
				if(!getCSAddrPoints(addrcode))
				{
					continue;
				}

				// �Z��8���R�[�h�{�n�ԁi����/�R�[�h�j�P�ʂɃf�[�^�\����ϊ�
				fnConvDataSet8Chiban();

				// �Z��8���R�[�h�{�n�ԁi����/�R�[�h�j�P�ʂŏZ���|�C���g�i��/CS�j���m�̋����`�F�b�N�����{
				fnCheckDataSet16();
			}
		}
		else
			return false;
	}
	catch(std::exception & e_std)
	{
		char aMessage[1024];
		std::cerr << "std::exception::what : " << e_std.what() << std::endl;
		sprintf(aMessage, "std::exception::what : ",e_std.what() );
		mChkLog.OutputDirect(aMessage);
		return false;
	}
	catch(_com_error &e_com)
	{
		char aMessage[1024];
		std::cerr << "com_error : " << e_com.ErrorMessage() << std::endl;
		sprintf(aMessage, "com_error : ",e_com.ErrorMessage() );
		mChkLog.OutputDirect(aMessage);
		return false;
	}
	
	// �I�����������O�֏o��
	mChkLog.OutputTime();

	return true;
}

bool CheckJusyoDist::getCSAddrPoints(const CString& addrcode)
{
	using namespace sindy::schema;

	// �w�肳�ꂽaddrcode�ɕR�t��CS�|�C���g�̎擾
	CString whereCaluse;
	whereCaluse.Format(_T("%s like '%s%%'"), cs_addr_point::kAddrCode, addrcode); 

	long pointCount = 0;
	IFeatureCursorPtr ipCsAddrPointFeaCur;
	if(!SearchByWhereClause(whereCaluse, ipFC_CSAddrPoint, pointCount, ipCsAddrPointFeaCur))
	{
		return false;
	}

	// �擾���ꂽ�|�C���g��0���̏ꍇ�́A���������I��
	if(pointCount == 0)
	{
		return true;
	}

	// CS�|�C���g�̃��[�v
	IFeaturePtr	ipCsAddrPointFea;
	while(S_OK == ipCsAddrPointFeaCur->NextFeature(&ipCsAddrPointFea) && ipCsAddrPointFea)
	{
		fnCSAddrProc( ipCsAddrPointFea );
	}

	return true;
}


