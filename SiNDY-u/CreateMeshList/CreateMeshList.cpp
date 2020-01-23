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

// CreateMeshList.cpp: CreateMeshList �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CreateMeshList.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CreateMeshList::CreateMeshList()
{

	char	*aTmpEnv;

	mAllAdmin	= false;

	strcpy(mFCCityMesh,		FC_CITYMESH);
	strcpy(mFCBaseMesh,		FC_BASEMESH);
	strcpy(mFCCityAdmin,	FC_CITYADMIN);
	strcpy(mFDCityMeshCode,	FD_CITYMESHCODE);
	strcpy(mFDCompleteF,	FD_COMPLETE_F);

	if((aTmpEnv = getenv("FC_CITYMESH"))		!= NULL) strcpy(mFCCityMesh,		aTmpEnv);
	if((aTmpEnv = getenv("FC_BASEMESH"))		!= NULL) strcpy(mFCBaseMesh,		aTmpEnv);
	if((aTmpEnv = getenv("FC_CITYADMIN"))		!= NULL) strcpy(mFCCityAdmin,		aTmpEnv);

#ifdef _DEBUG
	_putenv("DB_ADDRESS=sindy/sindy/SDE.SiNDY200308/5151/topaz");
//	_putenv("DB_ADDRESS=sindy/sindy/SDE.DEFAULT/5151/topaz");
//	_putenv("DB_ADDRESS=trial/trial/SDE.DEFAULT/5151/topaz");
#endif

	mDBCon_Address = getenv("DB_ADDRESS");

}

CreateMeshList::~CreateMeshList()
{

}

bool CreateMeshList::
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

		case 'A':
			mAllAdmin = true;
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

	if(i < mArgc - 1) {
		fprintf(stderr, "����������܂���\n");
		fnPrintUsage();
		return false;
	}

	mOutFN = mArgv[i];

	if(mDBCon_Address == NULL) {
		fprintf(stderr, "���ϐ� DB_ADDRESS ���w�肳��Ă��܂���\n");
		return false;
	}

	if(mOutFN == NULL) {
		fprintf(stderr, "�o�̓t�@�C�������w�肳��Ă��܂���\n");
		return false;
	}

	return true;

}

void CreateMeshList::
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
		"\t���b�V�����X�g�i�s�s�n�}�j���쐬����\n\n"
		"�y�g�p�`���z\n\n"
		"\t%s [�I�v�V����] <�o�̓��b�V�����X�g�t�@�C����>\n\n"
		"�y�I�v�V�����z\n\n"
		"\t-A\n"
		"\t\t���b�V�����X�g�̍쐬�Ώۂ��s���E�|���S���̐����G���A�Ƃ���\n"
		"\t\t�i�s���E�|���S�����P�ł��܂ރ��b�V�������X�g�ɏo�͂���j\n\n"
		"\t-?\n"
		"\t\t�g�p�@�i���̃e�L�X�g�j��\��\n\n"
		"�y���ϐ��z\n\n"
		"\tDB_ADDRESS\n"
		"\t\tGeoDB�ڑ�������i�Z���n�T�[�o�j���w��\n"
		"\t\t�`��: <user>/<password>/<version>/<instance>/<server>\n\n"
		"\tFC_CITYMESH\n"
		"\t\t�s�s�n�}���b�V�����C���̃t�B�[�`���N���X��\n"
		"\t\t�i�f�t�H���g: %s�j\n\n"
		"\tFC_BASEMESH\n"
		"\t\t���k���b�V�����C���̃t�B�[�`���N���X��\n"
		"\t\t�i�f�t�H���g: %s�j\n\n"
		"\tFC_CITYADMIN\n"
		"\t\t�s�s�n�}�s���E���C���̃t�B�[�`���N���X��\n"
		"\t\t�i�f�t�H���g: %s�j\n\n",
		aExecFN, aExecFN, FC_CITYMESH, FC_BASEMESH, FC_CITYADMIN);

}

bool CreateMeshList::
Create(int cArgc, char **cArgv)
{

	mArgc	= cArgc;
	mArgv	= cArgv;

	if(!fnAnalyzeArg()) return false;

	// �ڑ�
	if(!Connect(mDBCon_Address)) return false;

	// �t�B�[�`���N���X�擾
	ipFC_CityMesh	= GetFeatureClass(mFCCityMesh);		if(ipFC_CityMesh	== NULL) return false;
	ipFC_BaseMesh	= GetFeatureClass(mFCBaseMesh);		if(ipFC_BaseMesh	== NULL) return false;
	if(mAllAdmin) {
		ipFC_CityAdmin	= GetFeatureClass(mFCCityAdmin);	if(ipFC_CityAdmin	== NULL) return false;
	}

	// �t�B�[���h�C���f�b�N�X�擾
	LONG	aFID_MeshCode;
	if(!GetFieldID(ipFC_CityMesh, mFDCityMeshCode, &aFID_MeshCode)) {
		printf("%s �̃t�B�[���h�C���f�b�N�X���擾�ł��܂���ł���\n", mFDCityMeshCode);
		return false;
	}

	// �t�B�[�`���J�[�\���擾
	LONG	aCount;
	IFeatureCursorPtr	ipFCur_CityMesh;
	if(mAllAdmin) {
		ipFCur_CityMesh = SearchByWhereClause(NULL, ipFC_CityMesh, &aCount);
	} else {
		char	aWhereClause[300];
		sprintf(aWhereClause, "%s is not null", mFDCompleteF);
		ipFCur_CityMesh = SearchByWhereClause(aWhereClause, ipFC_CityMesh, &aCount);
	}

	fprintf(stderr, "�S���b�V���擾��...");
	// �t�B�[�`���J�[�\���̃��[�v
	int		i = 0;
	IFeaturePtr	ipF_CityMesh;
	while(ipFCur_CityMesh->NextFeature(&ipF_CityMesh) == S_OK) {

		long	aOID_CityMesh;
		ipF_CityMesh->get_OID(&aOID_CityMesh);

		CComVariant	aVar_MeshCode;
		ipF_CityMesh->get_Value(aFID_MeshCode, &aVar_MeshCode);

		MeshListCont	aMLCont(aVar_MeshCode.lVal, aOID_CityMesh);
		mMeshList.insert(aMLCont);

	}
	fprintf(stderr, "����\n");

	if(mAllAdmin) {

		fprintf(stderr, "�s���E�|���S���̗L���m�F��...\n");

		ISpatialCacheManagerPtr		ipCacheMgr(ipWorkspace);
		char	aBaseMeshCode[7];
		memset(aBaseMeshCode, 0, 7);

		// ���b�V�����X�g�̃��[�v
		long	i = 0, aMLSize = mMeshList.size();
		MeshListSet_i	aMLItr;
		for(aMLItr = mMeshList.begin(); aMLItr != mMeshList.end();) {

			i++;
			fprintf(stderr, "%7ld/%7ld\r", i, aMLSize);

			if(strncmp(aBaseMeshCode, (*aMLItr).eMeshCode, 6) != 0) {
				// �L���b�V������ւ�
				IFeatureCursorPtr	ipFCur_BaseMesh;
				long	aBMCount;
				char	aWhere[64];
				sprintf(aBaseMeshCode, "%.6s", (*aMLItr).eMeshCode);
				sprintf(aWhere, "%s = %s", FD_BASEMESHCODE, aBaseMeshCode);
				ipFCur_BaseMesh = SearchByWhereClause(aWhere, ipFC_BaseMesh, &aBMCount);
				if(aBMCount != 0) {
					IFeaturePtr		ipF_BaseMesh;
					if(ipFCur_BaseMesh->NextFeature(&ipF_BaseMesh) == S_OK) {
						IEnvelopePtr	ipE_BaseMesh;
						ipF_BaseMesh->get_Extent(&ipE_BaseMesh);
						ipCacheMgr->FillCache(ipE_BaseMesh);
					} else {
						fprintf(stderr, "�L���b�V���̍쐬�Ɏ��s(�p�����܂�): %s\n", aBaseMeshCode);
					}
				} else {
					fprintf(stderr, "�L���b�V���̍쐬�Ɏ��s(�p�����܂�): %s\n", aBaseMeshCode);
				}
			}

			// OID����t�B�[�`�����擾
			IFeaturePtr		ipF_CityMesh;
			ipFC_CityMesh->GetFeature((*aMLItr).eObjectID, &ipF_CityMesh);

			// �G���x���[�v�擾
			IEnvelopePtr	ipE_CityMesh;
			ipF_CityMesh->get_Extent(&ipE_CityMesh);

			// ��Ԍ���
			long	aCACount=0;
			IFeatureCursorPtr	ipFCur_CityAdmin;
			ipFCur_CityAdmin = SearchByShapeAndDesc(ipE_CityMesh,"T********", ipFC_CityAdmin, &aCACount);

			// ����������s���E�|���S�����Ȃ��ꍇ �� ���b�V�����X�g���珜��
			if(aCACount == 0) {
				mMeshList.erase(aMLItr++);
			} else {
				aMLItr++;
			}

		}

		fprintf(stderr, "\n����\n");

	}

/*
	// �t�B�[�`���J�[�\���̃��[�v
	int		i = 0;
	IFeaturePtr	ipF_CityMesh;
	while(ipFCur_CityMesh->NextFeature(&ipF_CityMesh) == S_OK) {

		i++;
		fprintf(stderr, "%7ld/%7ld\r", i, aCount);

		CComVariant	aVar_MeshCode;
		ipF_CityMesh->get_Value(aFID_MeshCode, &aVar_MeshCode);

		if(mAllAdmin) {
			IEnvelopePtr	ipE_CityMesh;
			ipF_CityMesh->get_Extent(&ipE_CityMesh);
			IFeatureCursorPtr	ipFCur_CityAdmin;
			long	aCACount;
//			ipFCur_CityAdmin = SearchByShape(ipE_CityMesh, esriSpatialRelIntersects, ipFC_CityAdmin, &aCACount);
			ipFCur_CityAdmin = SearchByShape(ipE_CityMesh, "T********", ipFC_CityAdmin, &aCACount);
			if(aCACount == 0) continue;

			IFeaturePtr	ipF_CityAdmin;
			while(ipFCur_CityAdmin->NextFeature(&ipF_CityAdmin) == S_OK) {
				long	aOID_CityAdmin;
				ipF_CityAdmin->get_OID(&aOID_CityAdmin);
				IGeometryPtr	ipG_CityAdmin;
				ipF_CityAdmin->get_Shape(&ipG_CityAdmin);
				VARIANT_BOOL	aTouches;
				if(FAILED(((IRelationalOperatorPtr)ipE_CityMesh)->Touches(ipG_CityAdmin, &aTouches))) {
					fprintf(stdout, "%8ld, %9ld, IRelationalOperator::Touches ���s\n", aVar_MeshCode.lVal, aOID_CityAdmin);
					continue;
				}
				if(aTouches) aCACount--;
			}
			if(aCACount == 0) continue;

		}

		MeshListCont	aMLCont(aVar_MeshCode.lVal);
		mMeshList.insert(aMLCont);
		fprintf(stderr, "----- %8ld -----\n", aVar_MeshCode.lVal);

	}
*/
	// ���b�V���R�[�h�̏o��
	if(!mMeshList.Create(mOutFN)) {
		fprintf(stderr, "�o�̓t�@�C�����쐬�ł��܂���ł���: %s\n", mOutFN);
		return false;
	}

	fprintf(stderr, "�S�����I��\n");

	return true;

}

// SpatialRelDescription�𗘗p����shape�ɂ�錟��
IFeatureCursorPtr CreateMeshList::SearchByShapeAndDesc(IGeometryPtr		ipGeometry,
			  LPCTSTR			cSpatialRelDesc,
			  IFeatureClassPtr	ipFeatureClass,
			  LONG				*cCount)
{

	IFeatureCursorPtr	ipFeatureCursor;

	if(!ipFeatureClass) {
		std::cout << "[SearchByShapeAndDesc] �t�B�[�`���N���X�� NULL �ł�" << std::endl;
		return nullptr;
	}

	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	CComBSTR	bstrFieldName;

	/// �N�G���t�B���^�쐬
	ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
	ipFilter->put_GeometryField( bstrFieldName );
	ipFilter->putref_Geometry( ipGeometry );
	ipFilter->put_SpatialRel( esriSpatialRelRelation );
	ipFilter->put_SpatialRelDescription( CComBSTR(cSpatialRelDesc) );

	/// �������ʂ� 0 �Ȃ烊�^�[��
	ipFeatureClass->FeatureCount( ipFilter, cCount );
	if( *cCount == 0 ) {
		return nullptr;
	}

	/// ����
	if(FAILED(ipFeatureClass->Search( ipFilter, VARIANT_FALSE, &ipFeatureCursor ))) {
		std::cout << "[SearchByShapeAndDesc] Search �Ɏ��s���܂���" << std::endl;
		*cCount = 0;
		return nullptr;
	}

	return ipFeatureCursor;

}