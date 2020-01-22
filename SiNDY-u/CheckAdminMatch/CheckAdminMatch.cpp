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

// CheckAdminMatch.cpp: CheckAdminMatch �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CheckAdminMatch.h"
#include <atlstr.h>
#include <WinLib/VersionInfo.h>
char	gMessage[1024];

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

// �R���X�g���N�^
CheckAdminMatch::CheckAdminMatch()
{

	mDBCon_Address	= NULL;

	mFC_CityAdmin	= NULL;
	mFC_BaseAdmin	= NULL;

	mFN_ChkLog		= NULL;

	mFN_CityList	= NULL;
	mCityCode		= NULL;

	mRangeMin		= -1;
	mRangeMax		= -1;

	mDist1			= 0;		// Base_Admin - City_Admin
	mDist2			= 0;		// City_Admin - City_Admin �W��
	mDist3			= 0;		// City_Admin - City_Admin 11��
	mDist4			= 0;		// City_Admin - City_Admin �X�敄���E

	mOID_BaseAdmin	= 0;

	mCLType			= kCLNone;

	m_iExeMode		= 0;

}

// �f�X�g���N�^
CheckAdminMatch::~CheckAdminMatch()
{

}

// Usage���b�Z�[�W�\��
void	CheckAdminMatch::printUsage()
{
	cerr
		<< "�y�s���E�����`�F�b�N�c�[���z\n"
		<< "(Usage)CheckAdminMatch.exe [option]\n"
		<< "�E�`�F�b�N�ΏۃI�v�V����\n"
		<< " -ca                �S���`�F�b�N\n"
		<< " -cl [listfile]     ���X�g�w��i�s�撬�����X�g�j\n"
		<< " -cs [code]         �P��w��i�s�撬���R�[�h�j\n"
		<< " -cr [code-code]    �G���A�w��i�s�撬���R�[�h-�s�撬���R�[�h�j\n"
		<< " -o  [filename]     ���O�o�̓t�@�C�����w��\n"
		<< "�E���s���[�h�ɂ���\n"
		<< " -f                 5���R�[�h���s���[�h(-ca, -cr���w�肵���Ƃ��͂�����)\n"
		<< " -e                 11���R�[�h���s���[�h\n"
		<< "\n"
		<< "�E���ϐ��ɂ���\n"
		<< " DB_ADDRESS         �ڑ���SDE\n"
		<< " FC_CITY_ADMIN      �s�s�n�}�s���E�t�B�[�`���N���X\n"
		<< " FC_BASE_ADMIN      �x�[�X�s���E�t�B�[�`���N���X\n"
		<< " JDB_CONNECT		�Z��DB�ڑ���\n"
		<< " PGDB_OUT           ���U�|���S���쐬��PGDB�t�@�C����\n"
		<< " FC_OUT             ���U�|���S���쐬��t�B�[�`���N���X��\n"
		<< endl;
}

// ���ϐ��擾
bool CheckAdminMatch::
fnGetEnvVar()
{

	mDBCon_Address	= getenv("DB_ADDRESS");
	if( mDBCon_Address == NULL ) {
		cerr << "���ϐ� DB_ADDRESS ���ݒ肳��Ă��܂���" << endl;
		return false;
	}

	mPGDB_OUT		= getenv("PGDB_OUT");
	if( mPGDB_OUT == NULL )
	{
		cerr << "���ϐ� PGDB_OUT ���ݒ肳��Ă��܂���" << endl;
		return false;
	}

	mFC_OUT			= getenv("FC_OUT");
	if( mFC_OUT == NULL )
	{
		cerr << "���ϐ� FC_OUT ���ݒ肳��Ă��܂���" << endl;
		return false;
	}

	mFC_CityAdmin	= getenv("FC_CITY_ADMIN");
	if( mFC_CityAdmin == NULL ) {
		cerr << "���ϐ� FC_CITY_ADMIN ���ݒ肳��Ă��܂���" << endl;
		return false;
	}

	mFC_BaseAdmin	= getenv("FC_BASE_ADMIN");
	if( mFC_BaseAdmin == NULL ) {
		cerr << "���ϐ� FC_BASE_ADMIN ���ݒ肳��Ă��܂���" << endl;
		return false;
	}

	return true;

}

// �������
bool CheckAdminMatch::
fnAnalyzeArg()
{

	int		i;
	char	*pRange, *pHyphen;

	if(mArgc < 2) {
		cerr << "����������܂���" << endl;
		return false;
	}

	for(i = 1; i < mArgc && mArgv[i][0] == '-'; i++) {
		switch(mArgv[i][1]) {
		case 'c':
			switch(mArgv[i][2]) {
			case 'a':
				mCLType	= kCLAll;
				break;
			case 'l':
				mCLType	= kCLList;
				mFN_CityList	= mArgv[++i];
				break;
			case 's':
				mCLType	= kCLSingle;
				mCityCode	= mArgv[++i];
				break;
			case 'r':
				mCLType	= kCLRange;
				pRange	= mArgv[++i];
				mRangeMin	= atol(pRange);
				pHyphen	= strchr(pRange, '-');
				if(pHyphen != NULL) {
					mRangeMax	= atol(++pHyphen);
				}
				break;
			default:
				cerr << "�s���ȃI�v�V�����ł�: " << mArgv[i] << endl;
				return false;
			}
			break;
		case 'o':
			mFN_ChkLog	= mArgv[++i];
			break;
		case 'd':
			switch(mArgv[i][2]) {
			case '1':
				mDist1	= atof(mArgv[++i]);
				break;
			case '2':
				mDist2	= atof(mArgv[++i]);
				break;
			case '3':
				mDist3	= atof(mArgv[++i]);
				break;
			case '4':
				mDist4	= atof(mArgv[++i]);
				break;
			default:
				cerr << "�s���ȃI�v�V�����ł�: " << mArgv[i] << endl;
				return false;
			}
			break;
		case 'e':
			m_iExeMode |= ON_ElevenCode;
			break;
		case 'f':
			m_iExeMode |= ON_FiveCode;
			break;
		case '?':
			return false;
		default:
			cerr << "�s���ȃI�v�V�����ł�: " << mArgv[i] << endl;
			return false;
		}
	}

	if( m_iExeMode == 0 )
	{
		cerr << "���s���[�h���w�肳��Ă���܂���(-e or -f)" << endl;
		return false;
	}
	else if( m_iExeMode & ON_ElevenCode && m_iExeMode & ON_FiveCode )
	{
		cerr << "���s���[�h�������w�肳��Ă���܂�(-e or -f)" << endl;
		return false;
	}

	switch(mCLType) {
	case kCLNone:
		cerr << "�I�v�V���� -c* ���w�肳��Ă��܂���" << endl;
		return false;
	case kCLAll:
		if( m_iExeMode & ON_ElevenCode )
		{
			cerr << "�S���`�F�b�N�̍ۂ� -f(5�����[�h) ���w�肵�Ă�������" << endl;
			return false;
		}
		break;
	case kCLList:
		if(mFN_CityList == NULL) {
			cerr << "�I�v�V���� -cl �Ŏs�撬�����X�g�t�@�C�����w�肳��Ă��܂���" << endl;
			return false;
		}
		break;
	case kCLSingle:
		if(mCityCode == NULL) {
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
		if(mRangeMin < 0 || mRangeMax < 0) {
			cerr << "�I�v�V���� -cr �Ŏs�撬���R�[�h�͈̔͂��������w�肳��Ă��܂���" << endl;
			return false;
		}
		if( m_iExeMode & ON_ElevenCode )
		{
			cerr << "�͈̓`�F�b�N�̍ۂ� -f(5�����[�h) ���w�肵�Ă�������" << endl;
			return false;
		}
		break;
	}

	if(mFN_ChkLog == NULL) {
		cerr << "�`�F�b�N���O�t�@�C�������w�肳��Ă��܂���" << endl;
		return false;
	}

	return true;

}

// �o�͗pPGDB�쐬
bool CheckAdminMatch::
fnCreateTmpFC()
{
	// [Bug 6193]�Ή� PGDB�쐬 ���݂���ꍇ�͊����̂��̂��J��
	IWorkspaceFactoryPtr	ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
	IWorkspacePtr		ipWorkspace;
	ipWorkspaceFactory->OpenFromFile( CComBSTR(mPGDB_OUT), NULL, &ipWorkspace );
	if( ipWorkspace == NULL )
	{
		std::string		temp = mPGDB_OUT;

		int				iPos = temp.rfind( "\\" );
		std::string		temp_Dir, temp_File;
		if( iPos != -1 )
		{
			temp_Dir	= temp.substr( 0, iPos );
			temp_File	= temp.substr( iPos );
		}
		else
		{
			temp_Dir	= ".";
			temp_File	= temp;
		}
		IPropertySetPtr		ipPropertySet( CLSID_PropertySet );
		IWorkspaceNamePtr	ipWorkspaceName;
		if( ipWorkspaceFactory->Create( CComBSTR( temp_Dir.c_str() ), CComBSTR( temp_File.c_str() ), ipPropertySet, NULL, &ipWorkspaceName ) != S_OK )
		{
			cout << "#PGDB�t�@�C���̍쐬�Ɏ��s�I" << endl;
			return false;
		}
		if( ipWorkspaceName )
		{
			IUnknownPtr		ipUnknown;
			( (INamePtr)ipWorkspaceName )->Open( &ipUnknown );
			ipWorkspace = ipUnknown;
		}
		else
			return false;
	}


	IFieldsPtr	ipFlds_TmpFC(CLSID_Fields);
	IFieldPtr	ipFld_Shape(CLSID_Field), ipFld_OID(CLSID_Field), ipFld_CityCode(CLSID_Field), ipFld_AddrCode(CLSID_Field);
	IGeometryDefPtr	ipGD_TmpFC(CLSID_GeometryDef);
	ISpatialReferencePtr	ipSR_TmpFC;
	IUIDPtr		ipUID_ClsID, ipUID_ExtClsID;

	ipFC_CityAdmin->get_CLSID(&ipUID_ClsID);
	ipFC_CityAdmin->get_EXTCLSID(&ipUID_ExtClsID);
	((IGeoDatasetPtr)ipFC_CityAdmin)->get_SpatialReference(&ipSR_TmpFC);

	// GeometryDef �ݒ�
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_GeometryType(esriGeometryPolygon);
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_GridCount(1);
	((IGeometryDefEditPtr)ipGD_TmpFC)->put_GridSize(0, 0.1);
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
	((IFieldEditPtr)ipFld_AddrCode)->put_Length(11);
	((IFieldEditPtr)ipFld_AddrCode)->put_IsNullable(VARIANT_FALSE);
	((IFieldsEditPtr)ipFlds_TmpFC)->AddField(ipFld_AddrCode);

	mFC_TmpPoly = mFC_OUT;	// [bug 9651]�Ή��ŏC��

	// �t�B�[�`���N���X�쐬
	((IFeatureWorkspacePtr)ipWorkspace)->CreateFeatureClass( CComBSTR(mFC_OUT), ipFlds_TmpFC, ipUID_ClsID, ipUID_ExtClsID, esriFTSimple, CComBSTR("SHAPE"), NULL, &ipFC_TmpPoly );
	if( ipFC_TmpPoly == NULL )
	{
		cerr << "�t�B�[�`���N���X " << mFC_TmpPoly << " �̍쐬�Ɏ��s���܂���" << endl;
		return false;
	}

	// �t�B�[���h�C���f�b�N�X�擾
	if(!GetFieldID(ipFC_TmpPoly,		FD_ADDR_CODE,			&mFID_AddrCode_TP))		return false;

	return true;

}

// ���s5��or11�����X�g�쐬
bool CheckAdminMatch::
fnMakeCityList()
{

	IFeatureCursorPtr	ipFCur_CityAdmin;
	IFeaturePtr			ipF_CityAdmin;
	_ICursorPtr		ipCur_CityAdmin;
	IQueryDefPtr	ipQDef_CityAdmin;
	_IRowPtr			ipR_CityAdmin;
	char	aWhere[128], aRange[64], aSubField[64];
	string	aTmpStr;
	sprintf(aWhere, "%s > '00000'", FD_CITY_CODE);
	sprintf(aSubField, "distinct(%s)", FD_CITY_CODE);

	switch(mCLType) {
	case kCLRange:
		sprintf(aRange, " and %s between '%05d' and '%05d'", FD_CITY_CODE, mRangeMin, mRangeMax);
		strcat(aWhere, aRange);
	case kCLAll:
		((IFeatureWorkspacePtr)ipWorkspace[mDBID_Address])->CreateQueryDef(&ipQDef_CityAdmin);
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
	}

	return true;

}

// ���k�s���E�擾
void CheckAdminMatch::
fnSetBaseAdminPoly(const char *cCityCode)
{

	char	aWhere[128];
	sprintf(aWhere, "CityCode = '%s'", cCityCode);

	IPolygonPtr	ipP_Result(CLSID_Polygon);

	long	aCount;
	IFeatureCursorPtr	ipFCur_BaseAdmin;
	ipFCur_BaseAdmin = SearchByWhereClause(aWhere, ipFC_BaseAdmin, &aCount);

	if(aCount < 1) {

		ipP_BaseAdmin->SetEmpty();
		mOID_BaseAdmin	= 0;

		// �Z���R�[�h����Z�����̎擾�iBug8610�Ή��j
		CString	AddrKanji;
		mJDBAccess.CodeToKanji( cCityCode, AddrKanji.GetBuffer( KEN_KANJI_MAX_LEN + SHI_KANJI_MAX_LEN + OAZA_KANJI_MAX_LEN + AZA_KANJI_MAX_LEN) );

		cerr << cCityCode << ": ���k�s���E�|���S��(Base_Admin)������܂���" << endl;
		sprintf(gMessage, "0\t\t\t���k�s���E���Ȃ�\t%s\t\t\t%s\t\t", cCityCode,
			AddrKanji );
		mChkLog.OutputDirect(gMessage);

	} else {

		IFeaturePtr	ipF_BaseAdmin;
		double	aArea, aAreaMax = 0;
		while(ipFCur_BaseAdmin->NextFeature(&ipF_BaseAdmin) == S_OK) {

			long	aOID;
			ipF_BaseAdmin->get_OID(&aOID);

			IGeometryPtr	ipG_BaseAdmin;
			IGeometryPtr	ipG_Union;
			ipF_BaseAdmin->get_Shape(&ipG_BaseAdmin);

			((IAreaPtr)ipG_BaseAdmin)->get_Area(&aArea);
			if(aArea > aAreaMax) {
				aAreaMax		= aArea;
				mOID_BaseAdmin	= aOID;
			}

			((ITopologicalOperatorPtr)ipP_Result)->Union(ipG_BaseAdmin, &ipG_Union);
			ipP_Result	= ipG_Union;

		}
		ipP_BaseAdmin = ipP_Result;
	}

	return;

}

// ���s���X�g�쐬
bool CheckAdminMatch::
fnMakeACList(const char *cCityCode)
{

	mACSet.clear();

	_ICursorPtr		ipCur_CityAdmin;
	char	aWhere[128];
	string	aTmpStr;

	sprintf(aWhere, FD_CITY_CODE " = '%s'", cCityCode);
	IFeatureCursorPtr	ipFCur_CityAdmin;
	long	aCount;
	ipFCur_CityAdmin = SearchByWhereClause(aWhere, ipFC_CityAdmin, &aCount);

	if(aCount < 1) {
		cerr << cCityCode << ": �s���E�|���S��������܂���" << endl;
		return false;
	}

	IFeaturePtr	ipF_CityAdmin;
	while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK) {

		CityListCont Cont_temp;
		long	aOID_CityAdmin;
		ipF_CityAdmin->get_OID(&aOID_CityAdmin);

		// �W�E11���R�[�h�̎擾�E���X�g�ւ̒ǉ�
		CComVariant	aTmpVar;
		string	aACStr11;
		ipF_CityAdmin->get_Value(mFID_CityCode, &aTmpVar);	aTmpStr = ExtractStr(aTmpVar);	aTmpVar.Clear();	aACStr11 = aTmpStr;
		strcpy( Cont_temp.eCityCode, aTmpStr.c_str());
		ipF_CityAdmin->get_Value(mFID_AddrCode, &aTmpVar);	aTmpStr = ExtractStr(aTmpVar);	aTmpVar.Clear();	aACStr11 += aTmpStr;
		strcpy( Cont_temp.eAddrCode, aTmpStr.c_str());
		// 11�����[�h�̏ꍇ�́A���̓��X�g�ƏƂ炵���킹�A�������͎̂��s���X�g�ɒǉ����Ȃ�
		if( m_iExeMode & ON_ElevenCode && mCityList.find( Cont_temp ) == mCityList.end() )
			continue;
		mACSet.Add(aACStr11, aOID_CityAdmin);

		// �s�撬���s���E�Ƃ̐����`�F�b�N
		VARIANT_BOOL	aEmpty;
		ipP_BaseAdmin->get_IsEmpty(&aEmpty);
		if(!aEmpty) {
			IGeometryPtr	ipG_CityAdmin;
			ipF_CityAdmin->get_Shape(&ipG_CityAdmin);
			VARIANT_BOOL	aDisjoint;
			((IRelationalOperatorPtr)ipP_BaseAdmin)->Disjoint(ipG_CityAdmin, &aDisjoint);
			if(aDisjoint) {
				IPointPtr	ipPt_Centroid;
				double		aX, aY;
				((IAreaPtr)ipG_CityAdmin)->get_Centroid(&ipPt_Centroid);
				ipPt_Centroid->QueryCoords(&aX, &aY);

				// �Z���R�[�h����Z�����̎擾�iBug8610�Ή��j
				CString	AddrKanji;
				mJDBAccess.CodeToKanji( aACStr11.c_str(), AddrKanji.GetBuffer( KEN_KANJI_MAX_LEN + SHI_KANJI_MAX_LEN + OAZA_KANJI_MAX_LEN + AZA_KANJI_MAX_LEN) );

				sprintf(gMessage, "0\t%s\t%ld\t�K�؂Ȓ��k�s���E���ɂȂ�\t%s\t%16.12f\t%15.12f\t%s\t\t",
					mFC_CityAdmin, aOID_CityAdmin, aACStr11.c_str(), aX, aY,
					AddrKanji);
				mChkLog.OutputDirect(gMessage);
			}
		}

	}

	return true;

}

// ���O�t�@�C���I�[�v��&�K�v��񏑂�����
bool CheckAdminMatch::
fnOpenChkLog()
{

	if(!mChkLog.Open(mFN_ChkLog, FREESTYLELOG)) {
		fprintf(stderr, "���O�t�@�C�����J���܂���\n");
		return false;
	}

	// �c�[���������O�֏o��
	mChkLog.OutputComment("�s���E�����`�F�b�N�c�[��");
	CVersion cVer;
	char szMsg[100] = {0};
	sprintf_s( szMsg, sizeof(szMsg), "%s FILEVERSION %s PRODUCTVERSION %s",	cVer.GetOriginalFilename(), cVer.GetFileVersion(),cVer.GetProductVersion() );
	mChkLog.OutputComment(szMsg);

	// �J�n���������O�֏o��
	mChkLog.OutputTime();

	// �w�b�_���o��
	mChkLog.OutputDirect("FLAG\tLAYER\tOBJECTID\t���\t�Z���R�[�h\t�d�S�o�x\t�d�S�ܓx\t�Z������\tCOMMENT �n�j|�C��|�ۗ�\tCOMMENT");
	mChkLog.Flush();

	return true;

}

// �s�s�n�}�s���E�|���S���P�ʂ̃��X�g�쐬
bool CheckAdminMatch::
fnAdminProc(IFeaturePtr ipF_CityAdmin, const char *cCityCode)
{

	CComVariant	aTmpVar;
	AdminAttr	aAdmAttr;
	AddrPolyRec	aAPRec;
	long		aOID_CityAdmin;
	IGeometryPtr	ipG_CityAdmin;

	aAdmAttr.eAddrCode	= cCityCode;

	// ObjectID �擾
	ipF_CityAdmin->get_OID(&aOID_CityAdmin);

	// AddrCode �擾
	string	aTmpStr;
	ipF_CityAdmin->get_Value(mFID_AddrCode, &aTmpVar);
	aTmpStr = ExtractStr(aTmpVar);
	aAdmAttr.eAddrCode += aTmpStr;
	aTmpVar.Clear();

	// �ʑ��n�Ȃǂ̓���R�[�h�͖���
	if(aAdmAttr.eAddrCode.substr(5, 6) == "999999") {
		return true;
	}

	// AreaClass_c �擾
	ipF_CityAdmin->get_Value(mFID_AreaClassC, &aTmpVar);
	switch(aTmpVar.lVal) {
	case 0:		aAdmAttr.eAreaClassC = '0'; break;
	case 1:		aAdmAttr.eAreaClassC = '1'; break;
	case 2:		aAdmAttr.eAreaClassC = '2'; break;
	default:	return false;
	}

	// AddrClass_c �擾
	ipF_CityAdmin->get_Value(mFID_AddrClassC, &aTmpVar);
	switch(aTmpVar.lVal) {
	case 0:		aAdmAttr.eAddrClassC = '0'; break;
	case 1:		aAdmAttr.eAddrClassC = '1'; break;
	case 2:		aAdmAttr.eAddrClassC = '2'; break;
	case 3:		aAdmAttr.eAddrClassC = '3'; break;
	default:	return false;
	}

	// GaikuFugo �擾
	ipF_CityAdmin->get_Value(mFID_GaikuFugo, &aTmpVar);
	if(aTmpVar.vt == VT_NULL) {
		if(aAdmAttr.eAddrClassC == '1' || aAdmAttr.eAddrClassC == '2') return false;	// �����ς� �� ����20�����͂��Ȃ�
		aAdmAttr.eGaikuFugo = 0;
	} else {
		if(aTmpVar.lVal > 99999) return false;
		aAdmAttr.eGaikuFugo = aTmpVar.lVal;
	}

	// �W�I���g���擾
	if(FAILED(ipF_CityAdmin->get_ShapeCopy(&ipG_CityAdmin))) return false;

	// �|���S�����X�g�֒ǉ�
	aAPRec.eOID			= aOID_CityAdmin;
	aAPRec.ipPolygon	= ((IPolygonPtr)ipG_CityAdmin);
	
	// 11�����[�h�̏ꍇ�́A�ʓr8���|���S�����X�g���쐬���邽�߁A�����ł͒ǉ����Ȃ�
	if( m_iExeMode & ON_FiveCode )
		mAddrPoly8.push_back(aAPRec);
	mAddrPoly11.push_back(aAPRec);
	if(aAdmAttr.eAddrClassC == '1' || aAdmAttr.eAddrClassC == '2')	mAddrPolyB.push_back(aAPRec);	// �����ς� �� �X�惊�X�g�ɂ��ǉ�

	return true;
}

// �|���S����ԃ`�F�b�N
bool CheckAdminMatch::
fnCheckAPDist(const string &cAddrCode, const AddrPoly &cAddrPoly, double cDist)
{
	// ����11���|���S���Q�̗אڃO���[�v�쐬
	AddrGroup	aAGroup;
	for(AddrPoly::const_iterator aAPItr1 = cAddrPoly.begin(); aAPItr1 != cAddrPoly.end(); aAPItr1++) {
		AddrGroup::iterator aAGTmp = aAGroup.end();
		for(AddrGroup::iterator aAGItr = aAGroup.begin(); aAGItr != aAGroup.end(); aAGItr++) {
			for(AddrPoly::iterator aAPItr2 = (*aAGItr).begin(); aAPItr2 != (*aAGItr).end(); aAPItr2++) {
				// �אڃ|���S�����ǂ����`�F�b�N
				VARIANT_BOOL	aDisjoint;
				((IRelationalOperatorPtr)(*aAPItr1).ipPolygon)->Disjoint((*aAPItr2).ipPolygon, &aDisjoint);
				if(!aDisjoint) {	// �א�
					if(aAGTmp != aAGroup.end()) {	// ���ɑ��̃O���[�v�ɑ����Ă���
						(*aAGTmp).splice((*aAGTmp).end(), *aAGItr);
						aAGItr = aAGroup.erase(aAGItr);
						aAGItr--;
					} else {						// �܂����̃O���[�v�ɑ����Ă��Ȃ�
						(*aAGItr).push_back(*aAPItr1);
						aAGTmp = aAGItr;
					}
					break;
				}
			}
		}
		if(aAGTmp == aAGroup.end()) {	// �אڂ��Ȃ�����
			AddrPoly	aAPoly;
			aAPoly.push_back(*aAPItr1);
			aAGroup.push_back(aAPoly);
		}
	}

	// ����11���|���S���Q�̋����`�F�b�N
	if(aAGroup.size() > 1) {	// �O���[�v������
		IPolygonPtr	ipP_TmpPoly(CLSID_Polygon);

		for(AddrPoly::const_iterator aAPItr1 = cAddrPoly.begin(); aAPItr1 != cAddrPoly.end(); aAPItr1++) {
			IGeometryPtr	ipG_Result;

			// Arc931�Ή��Œǉ�(��ԎQ�Ƃ������ƃW�I���g�����Z�����������Ȃ�) 2010/06/11
			ISpatialReferencePtr	ipSpRef;
			(aAPItr1->ipPolygon)->get_SpatialReference( &ipSpRef );	// ��ԎQ�Ƃ��s���E�̃W�I���g������擾	
			ipP_TmpPoly->putref_SpatialReference( ipSpRef );

			((ITopologicalOperatorPtr)ipP_TmpPoly)->Union((*aAPItr1).ipPolygon, &ipG_Result);
			ipP_TmpPoly	= ipG_Result;
		}

		IPointPtr	ipPt_Centroid;
		double		aX, aY;
		if(FAILED(((IAreaPtr)ipP_TmpPoly)->get_Centroid(&ipPt_Centroid))) {
			aX	= 0;
			aY	= 0;
		} else {
			ipPt_Centroid->QueryCoords(&aX, &aY);
		}

		// �o�͗p�t�B�[�`���J�[�\���擾
		IFeatureCursorPtr	ipFCur_TmpPoly;
		ipFC_TmpPoly->Insert(VARIANT_TRUE, &ipFCur_TmpPoly);

		IFeatureBufferPtr	ipFBuf_TmpPoly;
		ipFC_TmpPoly->CreateFeatureBuffer(&ipFBuf_TmpPoly);

		ipFBuf_TmpPoly->putref_Shape(ipP_TmpPoly);

		CComVariant	aVar_AddrCode = cAddrCode.c_str();
		ipFBuf_TmpPoly->put_Value(mFID_AddrCode_TP, aVar_AddrCode);

		// �Z���R�[�h����Z�����̎擾�iBug8610�Ή��j
		CString	AddrKanji;
		mJDBAccess.CodeToKanji( cAddrCode.c_str(), AddrKanji.GetBuffer( KEN_KANJI_MAX_LEN + SHI_KANJI_MAX_LEN + OAZA_KANJI_MAX_LEN + AZA_KANJI_MAX_LEN) );

		CComVariant	aOID_TP;
		if(FAILED(ipFCur_TmpPoly->InsertFeature(ipFBuf_TmpPoly, &aOID_TP)) || FAILED(ipFCur_TmpPoly->Flush())) {
			sprintf(gMessage, "0\t\t\t�s���E�|���S�������U\t%s\t%16.12f\t%15.12f\t%s\t\t",
				cAddrCode.c_str(), aX, aY,
				AddrKanji);
		} else {
			sprintf(gMessage, "0\t%s\t%ld\t�s���E�|���S�������U\t%s\t%16.12f\t%15.12f\t%s\t\t",
				mFC_TmpPoly.c_str(), aOID_TP.lVal, cAddrCode.c_str(), aX, aY,
				AddrKanji);
		}
		mChkLog.OutputDirect(gMessage);
	}
	mChkLog.Flush();

	return true;
}

// 8���P�ʂ̍s���E�|���S���L���b�V���쐬
bool CheckAdminMatch::
fnCreateCache8(const string &cAddrCode)
{
	ipSpCacheMgr->EmptyCache();

	char	aWhere[1024];
	sprintf(aWhere, FD_CITY_CODE " = '%s' and " FD_ADDR_CODE " like '%s___'",
		cAddrCode.substr(0, 5).c_str(), cAddrCode.substr(5, 3).c_str());

	long	aCount;
	IFeatureCursorPtr	ipFCur_CityAdmin;
	ipFCur_CityAdmin = SearchByWhereClause(aWhere, ipFC_CityAdmin, &aCount);

	if(aCount < 1) {
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

// 8���P�ʂ̍s���E�|���S�����X�g�쐬(11�����[�h�p)
bool CheckAdminMatch::
fnCreatePolyList8(const string &cAddrCode)
{
	char	aWhere[1024];
	sprintf(aWhere, FD_CITY_CODE " = '%s' and " FD_ADDR_CODE " like '%s___'",
		cAddrCode.substr(0, 5).c_str(), cAddrCode.substr(5, 3).c_str());

	long	aCount;
	IFeatureCursorPtr	ipFCur_CityAdmin;
	ipFCur_CityAdmin = SearchByWhereClause(aWhere, ipFC_CityAdmin, &aCount);

	// �|���S���ԃ`�F�b�N�p�Ȃ̂ŁA1�ȉ��̏ꍇ�͍쐬���Ȃ�
	if(aCount < 2) {
		return false;
	}

	IFeaturePtr		ipF_CityAdmin;
	while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK) 
	{
		CComVariant	aTmpVar;
		AdminAttr	aAdmAttr;
		AddrPolyRec	aAPRec;
		long		aOID_CityAdmin;
		IGeometryPtr	ipG_CityAdmin;

		aAdmAttr.eAddrCode	= cAddrCode.substr(0, 5).c_str();

		// ObjectID �擾
		ipF_CityAdmin->get_OID(&aOID_CityAdmin);

		// AddrCode �擾
		string	aTmpStr;
		ipF_CityAdmin->get_Value(mFID_AddrCode, &aTmpVar);
		aTmpStr = ExtractStr(aTmpVar);
		aAdmAttr.eAddrCode += aTmpStr;
		aTmpVar.Clear();

		// �ʑ��n�Ȃǂ̓���R�[�h�͖���
		if(aAdmAttr.eAddrCode.substr(5, 6) == "999999") {
			continue;
		}

		// AreaClass_c �擾
		ipF_CityAdmin->get_Value(mFID_AreaClassC, &aTmpVar);
		switch(aTmpVar.lVal) {
		case 0:		aAdmAttr.eAreaClassC = '0'; break;
		case 1:		aAdmAttr.eAreaClassC = '1'; break;
		case 2:		aAdmAttr.eAreaClassC = '2'; break;
		default:	continue;
		}

		// AddrClass_c �擾
		ipF_CityAdmin->get_Value(mFID_AddrClassC, &aTmpVar);
		switch(aTmpVar.lVal) {
		case 0:		aAdmAttr.eAddrClassC = '0'; break;
		case 1:		aAdmAttr.eAddrClassC = '1'; break;
		case 2:		aAdmAttr.eAddrClassC = '2'; break;
		case 3:		aAdmAttr.eAddrClassC = '3'; break;
		default:	continue;
		}

		// GaikuFugo �擾
		ipF_CityAdmin->get_Value(mFID_GaikuFugo, &aTmpVar);
		if(aTmpVar.vt == VT_NULL) {
			if(aAdmAttr.eAddrClassC == '1' || aAdmAttr.eAddrClassC == '2') continue;	// �����ς� �� ����20�����͂��Ȃ�
			aAdmAttr.eGaikuFugo = 0;
		} else {
			if(aTmpVar.lVal > 99999) continue;
			aAdmAttr.eGaikuFugo = aTmpVar.lVal;
		}

		// �W�I���g���擾
		if(FAILED(ipF_CityAdmin->get_ShapeCopy(&ipG_CityAdmin))) continue;

		// �|���S�����X�g�֒ǉ�
		aAPRec.eOID			= aOID_CityAdmin;
		aAPRec.ipPolygon	= ((IPolygonPtr)ipG_CityAdmin);
		mAddrPoly8.push_back(aAPRec);
	}

	return true;
}

// ���C����
bool CheckAdminMatch::
Execute(int cArgc, char **cArgv)
{
	mArgc	= cArgc;
	mArgv	= cArgv;

	if(!fnGetEnvVar()) return false;
	if(!fnAnalyzeArg()) return false;

	// GeoDB�֐ڑ�
	mDBID_Address	= Connect(mDBCon_Address);	if(mDBID_Address	< 0) return false;

	// �t�B�[�`���N���X�擾
	if((ipFC_CityAdmin		= GetFeatureClass(mDBID_Address, _T(mFC_CityAdmin)))	== NULL) return false;
	if((ipFC_BaseAdmin		= GetFeatureClass(mDBID_Address, _T(mFC_BaseAdmin)))	== NULL) return false;

	// �t�B�[���h�C���f�b�N�X�擾
	if(!GetFieldID(ipFC_CityAdmin,		FD_CITY_CODE,			&mFID_CityCode))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		FD_ADDR_CODE,			&mFID_AddrCode))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		FD_GAIKU_FUGO,			&mFID_GaikuFugo))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		FD_EXT_GAIKU_FUGO,		&mFID_ExtGaikuFugo))	return false;
	if(!GetFieldID(ipFC_CityAdmin,		FD_AREA_CLASS_C,		&mFID_AreaClassC))		return false;
	if(!GetFieldID(ipFC_CityAdmin,		FD_ADDR_CLASS_C,		&mFID_AddrClassC))		return false;

	// �`�F�b�N���ʊi�[��t�B�[�`���N���X�쐬
	if(!fnCreateTmpFC()) return false;

	// �Z��DB�ւ̐ڑ�
	if( !mJDBAccess.Connect() )
	{
		cerr << "�Z��DB�ւ̐ڑ��Ɏ��s" << endl;
		return false;
	}

	// �s�撬�����X�g�̐���
	cerr << "�s�撬�����X�g�쐬��...";
	if(!fnMakeCityList()) return false;
	cerr << "����" << endl;

	// ���O�t�@�C���̃I�[�v��
	if(!fnOpenChkLog()) return false;

	ipSpCacheMgr = ipWorkspace[mDBID_Address];

	char	temp_addr[6];
	memset( temp_addr, '\0', 6 );
	try
	{
		// �s�撬���R�[�h���X�g�̃��[�v
		for(CityListSet_i aCLItr = mCityList.begin(); aCLItr != mCityList.end(); aCLItr++) {

			if( m_iExeMode & ON_ElevenCode && strcmp( temp_addr, (*aCLItr).eCityCode ) == 0 )
				continue;
			strcpy( temp_addr, (*aCLItr).eCityCode );
			cerr << (*aCLItr).eCityCode << ": �`�F�b�N��..." << endl;

			// �s�撬���|���S���̐ݒ�
			fnSetBaseAdminPoly((*aCLItr).eCityCode);

			// �W�E11�����X�g�̍쐬�^�s���E�̃`�F�b�N
			if(!fnMakeACList((*aCLItr).eCityCode)) continue;
			mChkLog.Flush();

			// �W�����X�g�̃��[�v
			ACSet::iterator	aACItr;
			for(aACItr = mACSet.begin(); aACItr != mACSet.end(); aACItr++) {

				cerr << (*aACItr).eAC8 << ": �`�F�b�N��..." << endl;

				// �L���b�V���쐬
				fnCreateCache8((*aACItr).eAC8);

				// 11�����X�g�̃��[�v
				AC11Set::iterator	aAC11Itr;
				for(aAC11Itr = (*aACItr).eAC11Set.begin(); aAC11Itr != (*aACItr).eAC11Set.end(); aAC11Itr++) {

					cerr << (*aAC11Itr).eAC11 << ": �`�F�b�N��..." << endl;

					// �s���E�|���S���̃��[�v
					set<long, less<long> >::iterator	aItr;
					for(aItr = (*aAC11Itr).eOIDSet.begin(); aItr != (*aAC11Itr).eOIDSet.end(); aItr++) {
						IFeaturePtr		ipF_CityAdmin;
						ipFC_CityAdmin->GetFeature(*aItr, &ipF_CityAdmin);
						if(!fnAdminProc(ipF_CityAdmin, (*aCLItr).eCityCode)) {
							mChkLog.Flush();
						}
					}
					fnCheckAPDist((*aAC11Itr).eAC11, mAddrPoly11, mDist3);
					fnCheckAPDist((*aAC11Itr).eAC11, mAddrPolyB, mDist4);

					mAddrPoly11.clear();
					mAddrPolyB.clear();
				}
				if( m_iExeMode & ON_FiveCode )
				{
					if((*aACItr).eAC11Set.size() > 1) {		// 11�����������݂���ꍇ�i���O�̏d����h���j
						fnCheckAPDist((*aACItr).eAC8, mAddrPoly8, mDist2);
					}
				}
				else
				{
					// 8���P�ʂ̃|���S�����X�g�쐬
					if(fnCreatePolyList8((*aACItr).eAC8) && (*aACItr).eAC11Set.size() > 1)
					{
						fnCheckAPDist((*aACItr).eAC8, mAddrPoly8, mDist2);
					}
				}
				mAddrPoly8.clear();
			}
		}
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
