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

// CountGouPoint.cpp: CountGouPoint �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CountGouPoint.h"

char *strcpy(char *cDest, CComVariant *cSrc)
{

	USES_CONVERSION;
	return strcpy(cDest, OLE2T(cSrc->bstrVal));

}

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CountGouPoint::CountGouPoint()
{

	char	*aTmpEV;

	mArgv			= NULL;

	mCityCode		= NULL;
	mFNCityList		= NULL;
	mFNOutput		= NULL;

	mFPOutput		= NULL;

	mMode			= kNone;
	m_iExeMode		= 0; 

#ifdef _DEBUG
//	_putenv("DB_ADDRESS=sindy2009a/sindy2009a/SDE.Current09A/5151/topaz2");
//	_putenv("DB_ADDRESS=beta9/beta9/beta9.B9_200305/5151/geo-as");
	_putenv("DB_ADDRESS=ARC93/ARC93/SDE.SNP_ARC93_10M/5151/despina");
#endif

	mDBCon_Address	= getenv("DB_ADDRESS");

	strcpy(mFCGouPoint,		FC_GOU_POINT);
	strcpy(mFCCityAdmin,	FC_CITY_ADMIN);
	strcpy(mFCBaseAdmin,	FC_BASE_ADMIN);

	if((aTmpEV = getenv("FC_GOU_POINT"		)) != NULL)	strcpy(mFCGouPoint,		aTmpEV);
	if((aTmpEV = getenv("FC_CITY_ADMIN"		)) != NULL)	strcpy(mFCCityAdmin,	aTmpEV);
	if((aTmpEV = getenv("FC_BASE_ADMIN"		)) != NULL)	strcpy(mFCBaseAdmin,	aTmpEV);

}

CountGouPoint::~CountGouPoint()
{

}

bool CountGouPoint::
fnAnalyzeArg(void)
{

	int	i;

	if(mArgc < 2) {
		fprintf(stderr, "����������܂���\n");
		fnPrintUsage();
		return false;
	}

	for(i = 1; i < mArgc && mArgv[i][0] == '-'; i++) {

		switch(mArgv[i][1]) {

		case 'a':
			mMode = kAllCity;
			break;

		case 'c':
			i++;
			mMode = kSingleCity;
			if(i < mArgc) mCityCode = mArgv[i];
			else {
				fprintf(stderr, "����������܂���\n");
				return false;
			}
			break;

		case 'l':
			i++;
			mMode = kCitiesInList;
			if(i < mArgc) mFNCityList = mArgv[i];
			else {
				fprintf(stderr, "����������܂���\n");
				return false;
			}
			break;

		case 'o':
			i++;
			if(i < mArgc) mFNOutput = mArgv[i];
			else {
				fprintf(stderr, "����������܂���\n");
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
			fnPrintUsage();
			return false;
			break;

		default:
			fprintf(stderr, "�s���ȃI�v�V�����ł�: %s\n", mArgv[i]);
			return false;

		}

	}

	if(mFNOutput == NULL) {
		fprintf(stderr, "�o�̓t�@�C�������w�肳��Ă��܂���\n");
		return false;
	}
	if(mMode == kNone) {
		fprintf(stderr, "�I�v�V���� -a / -c / -l �̂����ꂩ���K�v�ł�\n");
		return false;
	} else if(mCityCode != NULL && strspn(mCityCode, "1234567890") != strlen(mCityCode)) {
		fprintf(stderr, "�Z���R�[�h���s���ł�\n");
		return false;
	}

	if( m_iExeMode == 0 )
	{
		fprintf(stderr, "���s���[�h���w�肳��Ă���܂���(-e or -f)\n");
		return false;
	}
	else if( m_iExeMode & ON_ElevenCode && m_iExeMode & ON_FiveCode )
	{
		fprintf(stderr, "���s���[�h�������w�肳��Ă���܂�(-e or -f)\n");
		return false;
	}

	if( m_iExeMode & ON_ElevenCode && mMode == kAllCity )
	{
		fprintf(stderr, "�S�������J�E���g����ۂ� -e ���w�肵�Ă�������\n");
		return false;
	}

	if( mCityCode != NULL && ((m_iExeMode & ON_ElevenCode && strlen(mCityCode) < 11) || (m_iExeMode & ON_FiveCode && strlen(mCityCode) < 5) ))
	{
		fprintf(stderr, "�Z���R�[�h�̌������s���ł�\n");
		return false;
	}

	return true;

}

void CountGouPoint::
fnPrintUsage(void)
{

	char	*aExecFN;
	aExecFN = strrchr(mArgv[0], '\\');
	if(aExecFN == NULL) aExecFN = mArgv[0];
	else aExecFN++;

	fprintf(stderr,
		"\n�y�R�}���h���́z\n\n"
		"\t%s\n\n"
		"�y�������e�z\n\n"
		"\t�E���|�C���g�̌������T���R�[�h���ƂɃJ�E���g����\n\n"
		"�y�g�p�`���z\n\n"
		"\t%s <�I�v�V����>\n\n"
		"�y�I�v�V�����z\n\n"
		"\t-a\n"
		"\t\t�S�����ɂ��ăJ�E���g����\n\n"
		"\t-c <�s�撬���R�[�h>\n"
		"\t\t�w�肵���R�[�h�̎s�撬���ɂ��ăJ�E���g����\n\n"
		"\t-l <�t�@�C����>\n"
		"\t\t�s�撬���R�[�h���X�g�̕��ɂ��ăJ�E���g����\n\n"
		"\t-o <�t�@�C����>\n"
		"\t\t�o�͐�t�@�C�������w��\n\n"
		"\t-?\n"
		"\t\t�g�p�@�i���̃e�L�X�g�j��\��\n\n"
		"�y���ϐ��z\n\n"
		"\tDB_ADDRESS\n"
		"\t\tGeoDB�ڑ�������i�Z���n�T�[�o�j���w��\n"
		"\t\t�`��: <user>/<password>/<version>/<instance>/<server>\n\n"
		"\tFC_GOU_POINT\n"
		"\t\t���|�C���g���C���̃t�B�[�`���N���X��\n"
		"\t\t�i�f�t�H���g: %s�j\n\n"
		"\tFC_CITY_ADMIN\n"
		"\t\t�s�s�n�}�s���E���C���̃t�B�[�`���N���X��\n"
		"\t\t�i�f�t�H���g: %s�j\n\n"
		"\tFC_BASE_ADMIN\n"
		"\t\t���k�s���E���C���̃t�B�[�`���N���X��\n"
		"\t\t�i�f�t�H���g: %s�j\n\n",
		aExecFN, aExecFN, FC_GOU_POINT, FC_CITY_ADMIN, FC_BASE_ADMIN);

}

bool CountGouPoint::
fnMakeCityList(void)
{

	switch(mMode) {

	case kAllCity:

		{

//			char			aWhere[128];
			char			aSubField[128];
			_ICursorPtr		ipCur_CityAdmin;
			IQueryDefPtr	ipQDef_CityAdmin;

			sprintf(aSubField, "distinct(%s)", FD_CITYCODE);
//			sprintf(aWhere, "%s > '00000'", FD_CITYCODE);
			((IFeatureWorkspacePtr)ipWorkspace[mDBID_Address])->CreateQueryDef(&ipQDef_CityAdmin);
			ipQDef_CityAdmin->put_Tables(CComBSTR(mFCCityAdmin));
			ipQDef_CityAdmin->put_SubFields(CComBSTR(aSubField));
//			ipQDef_CityAdmin->put_WhereClause(CComBSTR(aWhere));
			ipQDef_CityAdmin->Evaluate(&ipCur_CityAdmin);

			_IRowPtr		ipR_CityAdmin;
			while(ipCur_CityAdmin->NextRow(&ipR_CityAdmin) == S_OK) {
				CComVariant		aVar_CityCode;
				char			aCityCode[6];
				ipR_CityAdmin->get_Value(0, &aVar_CityCode);
				strcpy(aCityCode, &aVar_CityCode);
				if(!mCityList.Add(aCityCode)) return false;
			}

		}

		break;

	case kCitiesInList:
		if(!mCityList.Open(mFNCityList)) return false;
		break;

	case kSingleCity:
		if(!mCityList.Add(mCityCode)) return false;
		break;

	default:
		break;

	}

	return true;

}

#ifdef _USE_SPCACHE_

bool CountGouPoint::
fnCreateCache(const char *cCityCode)
{

	fprintf(stderr, "%s: �L���b�V���쐬��...", cCityCode);

	// ���k�s���E�|���S���̎擾
	char	aWhere[512];
	long	aBACount;
	IFeatureCursorPtr	ipFCur_BaseAdmin;
	sprintf(aWhere, "%s = '%s'", FD_CITYCODE, cCityCode);
	ipFCur_BaseAdmin = SearchByWhereClause(aWhere, ipFC_BaseAdmin, &aBACount);

	if(aBACount == 0) {

		fprintf(stderr, "�Ή����钆�k�s���E�|���S��������܂���\n");
		return false;

	} else {

		double			aCacheArea, aMaxArea = 0;
		IFeaturePtr		ipF_BaseAdmin;
		IEnvelopePtr	ipE_BaseAdmin, ipE_ForCache;
		while(ipFCur_BaseAdmin->NextFeature(&ipF_BaseAdmin) == S_OK) {
			ipF_BaseAdmin->get_Extent(&ipE_BaseAdmin);
			((IAreaPtr)ipE_BaseAdmin)->get_Area(&aCacheArea);
			if(aCacheArea > aMaxArea) {
				aMaxArea = aCacheArea;
				ipE_ForCache = ipE_BaseAdmin;
			}
		}

		if(aMaxArea > 1) {
			fprintf(stderr, "�G���A������Ȃ̂ŃL���b�V�����쐬���܂���\n");
			return false;
		} else {
			ipSpCacheMgr->FillCache(ipE_ForCache);
			fprintf(stderr, "����\n");
		}

	}

	return true;

}

bool CountGouPoint::
fnCreateCache(IGeometryPtr ipG_TgtArea)
{

	VARIANT_BOOL	aIsFull;
	ipSpCacheMgr->get_CacheIsFull(&aIsFull);
	if(aIsFull) return true;	// ���łɃL���b�V�����쐬����Ă���ꍇ�͔�����

	IEnvelopePtr	ipE_TgtArea;
	ipG_TgtArea->get_Envelope(&ipE_TgtArea);
	ipSpCacheMgr->FillCache(ipE_TgtArea);

	return true;

}

#endif

bool CountGouPoint::
Execute(int cArgc, char **cArgv)
{

	mArgc	= cArgc;
	mArgv	= cArgv;

	if(!fnAnalyzeArg()) return false;

	if(mDBCon_Address == NULL) {
		fprintf(stderr, "���ϐ� DB_ADDRESS ���ݒ肳��Ă��܂���\n");
		return false;
	}

	// �ڑ�
	mDBID_Address	= Connect(_T(mDBCon_Address));	if(mDBID_Address	< 0) return false;

	// �t�B�[�`���N���X�̎擾
	ipFC_GouPoint	= GetFeatureClass(mDBID_Address,	mFCGouPoint);
	if(ipFC_GouPoint	== NULL)	return false;
	ipFC_CityAdmin	= GetFeatureClass(mDBID_Address,	mFCCityAdmin);
	if(ipFC_CityAdmin	== NULL)	return false;
	ipFC_BaseAdmin	= GetFeatureClass(mDBID_Address,	mFCBaseAdmin);
	if(ipFC_BaseAdmin	== NULL)	return false;

	// �t�B�[���h�E�C���f�b�N�X�̎擾
	if(!GetFieldID(ipFC_GouPoint,	_T(FD_GOUTYPE),			&mFID_GouType))		return false;
	if(!GetFieldID(ipFC_CityAdmin,	_T(FD_CITYCODE),		&mFID_CityCode))	return false;
	if(!GetFieldID(ipFC_CityAdmin,	_T(FD_ADDRCODE),		&mFID_AddrCode))	return false;

	// 5�����X�gor11�����X�g���쐬
	if(!fnMakeCityList()) {
		fprintf(stderr, "�s�撬�����X�g�𐶐��ł��܂���\n");
		return false;
	}

	// �o�̓t�@�C���̃I�[�v��
	mFPOutput = fopen(mFNOutput, "w");
	if(mFPOutput == NULL) {
		fprintf(stderr, "�t�@�C�����J���܂���: %s\n", mFNOutput);
		return false;
	}

	ipSpCacheMgr	= ipWorkspace[mDBID_Address];

	int aTotalGouCount = 0, aTotalTpgCount = 0, aTotalTotalCount = 0, aTotalAdmCount = 0;

	CityListSet_i	aCLItr;
	
	// 5�����s���[�h
	if( m_iExeMode & ON_FiveCode )
	{
		// 5�����X�g�̃��[�v
		for(aCLItr = mCityList.begin(); aCLItr != mCityList.end(); aCLItr++) {

			GCSet	aGCSet;

			fprintf(stderr, "%s ������ ...\n", (*aCLItr).eCityCode);

#ifdef _USE_SPCACHE_
			// �L���b�V���쐬
			fnCreateCache((*aCLItr).eCityCode);
#endif

			char	aWhere2[512];
			long	aCACount;
			IFeatureCursorPtr	ipFCur_CityAdmin;
			sprintf(aWhere2, "%s = '%s'", FD_CITYCODE, (*aCLItr).eCityCode);
			ipFCur_CityAdmin = SearchByWhereClause(aWhere2, ipFC_CityAdmin, &aCACount);

			if(aCACount == 0) {
				fprintf(stderr, "%s: ���̎s�撬���R�[�h�����s�s�n�}�s���E�|���S��������܂���\n", (*aCLItr).eCityCode);
				continue;
			}

			long			i = 0;

			fprintf(stderr, "���|�C���g�J�E���g�� ...\n");

			// �s���E�|���S���̃��[�v
			IFeaturePtr	ipF_CityAdmin;
			while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK) {

				int		aGouCount	= 0;
				int		aTpgCount	= 0;

				i++;
				fprintf(stderr, "\r%8ld / %8ld", i, aCACount);

	//			long	aOID_CityAdmin;
	//			ipF_CityAdmin->get_OID(&aOID_CityAdmin);
				IGeometryPtr	ipG_CityAdmin;
				ipF_CityAdmin->get_Shape(&ipG_CityAdmin);

#ifdef _USE_SPCACHE_
				// �L���b�V���쐬�i���k�s���E�P�ʂ̂��̂��쐬����Ă��Ȃ��ꍇ�̂݁j
				fnCreateCache(ipG_CityAdmin);
#endif

				CComVariant		aVar_AddrCode;
				char			aAddrCode[12];
				sprintf(aAddrCode, "%s", (*aCLItr).eCityCode);
				ipF_CityAdmin->get_Value(mFID_AddrCode, &aVar_AddrCode);
				strcpy(&aAddrCode[5], &aVar_AddrCode);

				long	aGPCount;
				IFeatureCursorPtr	ipFCur_GouPoint;
				ipFCur_GouPoint = SearchByShape(ipG_CityAdmin, esriSpatialRelContains, ipFC_GouPoint, &aGPCount);

				if(aGPCount > 0) {
					// ���|�C���g�̃��[�v
					IFeaturePtr	ipF_GouPoint;
					while(ipFCur_GouPoint->NextFeature(&ipF_GouPoint) == S_OK) {
						CComVariant	aVar_GouType;
						ipF_GouPoint->get_Value(mFID_GouType, &aVar_GouType);
						if(aVar_GouType.lVal == 1)	aGouCount++;
						else						aTpgCount++;
					}
				}

				GouCount	aGC(aAddrCode, aGouCount, aTpgCount, 1);
				GCSet::iterator	aGCItr = aGCSet.FindCode(aAddrCode);
				if(aGCItr == aGCSet.end()) {	// ������Ȃ�
					aGCSet.insert(aGC);
				} else {						// ��������
					aGC.eGouCount += (*aGCItr).eGouCount;
					aGC.eTpgCount += (*aGCItr).eTpgCount;
					aGC.eAdmCount += (*aGCItr).eAdmCount;
					aGCSet.erase(aGCItr);
					aGCSet.insert(aGC);
				}

			}

#ifdef _USE_SPCACHE_
			ipSpCacheMgr->EmptyCache();
#endif

			fprintf(stderr, "\n����\n");

			// ���O�o�͂��^�u��؂�ɂ��A���v�̂܂��ɕ�����₷���L���������
			for(GCSet::iterator aItr = aGCSet.begin(); aItr != aGCSet.end(); aItr++) {
				int		aTotalCount	= (*aItr).eGouCount + (*aItr).eTpgCount;
				//fprintf(mFPOutput, "%s, %7ld, %7ld, %7ld, %4ld\n", (*aItr).eAddrCode, (*aItr).eGouCount, (*aItr).eTpgCount, aTotalCount, (*aItr).eAdmCount);
				fprintf(mFPOutput, "%s,\t%ld,\t%ld,\t%ld,\t%ld\n", (*aItr).eAddrCode, (*aItr).eGouCount, (*aItr).eTpgCount, aTotalCount, (*aItr).eAdmCount);
				aTotalGouCount += (*aItr).eGouCount;
				aTotalTpgCount += (*aItr).eTpgCount;
				aTotalAdmCount += (*aItr).eAdmCount;
			}
			fflush(mFPOutput);
		}
	}

	// 11�����s���[�h
	else if( m_iExeMode & ON_ElevenCode )
	{
		// 11�����X�g�̃��[�v
		for(aCLItr = mCityList.begin(); aCLItr != mCityList.end(); aCLItr++) {

			GCSet	aGCSet;

			fprintf(stderr, "%s%s ������ ...\n", (*aCLItr).eCityCode, (*aCLItr).eAddrCode);

			char	aWhere2[512];
			long	aCACount;
			IFeatureCursorPtr	ipFCur_CityAdmin;
			sprintf(aWhere2, "%s = '%s' AND %s = '%s'", FD_CITYCODE, (*aCLItr).eCityCode, FD_ADDRCODE, (*aCLItr).eAddrCode);
			ipFCur_CityAdmin = SearchByWhereClause(aWhere2, ipFC_CityAdmin, &aCACount);

			if(aCACount == 0) {
				fprintf(stderr, "%s%s: ����11���R�[�h�����s�s�n�}�s���E�|���S��������܂���\n", (*aCLItr).eCityCode, (*aCLItr).eAddrCode);
				continue;
			}

			long			i = 0;

			fprintf(stderr, "���|�C���g�J�E���g�� ...\n");

			// �s���E�|���S���̃��[�v
			IFeaturePtr	ipF_CityAdmin;
			while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK) {

				int		aGouCount	= 0;
				int		aTpgCount	= 0;

				i++;
				fprintf(stderr, "\r%8ld / %8ld", i, aCACount);


				IGeometryPtr	ipG_CityAdmin;
				ipF_CityAdmin->get_Shape(&ipG_CityAdmin);

#ifdef _USE_SPCACHE_
				// �L���b�V���쐬
				fnCreateCache(ipG_CityAdmin);
#endif

				CComVariant		aVar_AddrCode;
				char			aAddrCode[12];
				sprintf(aAddrCode, "%s", (*aCLItr).eCityCode);
				strcpy(&aAddrCode[5], (*aCLItr).eAddrCode);

				long	aGPCount;
				IFeatureCursorPtr	ipFCur_GouPoint;
				ipFCur_GouPoint = SearchByShape(ipG_CityAdmin, esriSpatialRelContains, ipFC_GouPoint, &aGPCount);

				if(aGPCount > 0) {
					// ���|�C���g�̃��[�v
					IFeaturePtr	ipF_GouPoint;
					while(ipFCur_GouPoint->NextFeature(&ipF_GouPoint) == S_OK) {
						CComVariant	aVar_GouType;
						ipF_GouPoint->get_Value(mFID_GouType, &aVar_GouType);
						if(aVar_GouType.lVal == 1)	aGouCount++;
						else						aTpgCount++;
					}
				}

				GouCount	aGC(aAddrCode, aGouCount, aTpgCount, 1);
				GCSet::iterator	aGCItr = aGCSet.FindCode(aAddrCode);
				if(aGCItr == aGCSet.end()) {	// ������Ȃ�
					aGCSet.insert(aGC);
				} else {						// ��������
					aGC.eGouCount += (*aGCItr).eGouCount;
					aGC.eTpgCount += (*aGCItr).eTpgCount;
					aGC.eAdmCount += (*aGCItr).eAdmCount;
					aGCSet.erase(aGCItr);
					aGCSet.insert(aGC);
				}

			}

#ifdef _USE_SPCACHE_
			ipSpCacheMgr->EmptyCache();
#endif

			fprintf(stderr, "\n����\n");
			
			// ���O�o�͂��^�u��؂�ɂ��A���v�̂܂��ɕ�����₷���L���������
			for(GCSet::iterator aItr = aGCSet.begin(); aItr != aGCSet.end(); aItr++) {
				int		aTotalCount	= (*aItr).eGouCount + (*aItr).eTpgCount;
				fprintf(mFPOutput, "%s,\t%ld,\t%ld,\t%ld,\t%ld\n", (*aItr).eAddrCode, (*aItr).eGouCount, (*aItr).eTpgCount, aTotalCount, (*aItr).eAdmCount);
				aTotalGouCount += (*aItr).eGouCount;
				aTotalTpgCount += (*aItr).eTpgCount;
				aTotalAdmCount += (*aItr).eAdmCount;
			}
			fflush(mFPOutput);
		}
	}

	// ���v�Z�o
	aTotalTotalCount = aTotalGouCount + aTotalTpgCount;
	fprintf(mFPOutput, "--------------------------------------------------\n");
	fprintf(mFPOutput, "Total Count,\t%ld,\t%ld,\t%ld,\t%ld\n", aTotalGouCount, aTotalTpgCount, aTotalTotalCount, aTotalAdmCount);

	fclose(mFPOutput);

	return true;

}
