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

// CreateExistFile.cpp: CreateExistFile �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CreateExistFile.h"
//#include "..\..\LIB\TDC\GeomtoShapeFile\GeomtoShapeFile.h" ����m�F�p1/15

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CreateExistFile::CreateExistFile()
{

	char	*aTmpStr;

	mArgc		= 0;
	mArgv		= NULL;

	if((aTmpStr = getenv("EXIST_DIR"))		!= NULL) mOutDir		= aTmpStr;

	mCreateMode	= kCreateNone;

	mDivNum		= 32;

	mFCCityMesh	= FC_CITYMESH;
	mFCBaseMesh	= FC_BASEMESH;
	mFCCitySite	= FC_CITYSITE;

	if((aTmpStr = getenv("DB_MAP"))			!= NULL) mDBMap			= aTmpStr;

	if((aTmpStr = getenv("FC_CITYMESH"))	!= NULL) mFCCityMesh	= aTmpStr;
	if((aTmpStr = getenv("FC_BASEMESH"))	!= NULL) mFCBaseMesh	= aTmpStr;
	if((aTmpStr = getenv("FC_CITYSITE"))	!= NULL) mFCCitySite	= aTmpStr;

	ExistError = false;

}

CreateExistFile::~CreateExistFile()
{

}

bool CreateExistFile::
fnAnalyzeArg()
{

	int		i;

	if(mArgc < ARG_MIN + 1) goto ARG_LACK;

	for(i = 1; i < mArgc && mArgv[i][0] == '-'; i++) {

		switch(mArgv[i][1]) {

		case 'a':
			mCreateMode = kCreateAllMesh;
			break;

		case 'l':
			i++;
			mCreateMode = kCreateMeshesInList;
			if(i < mArgc) mFNMeshList = mArgv[i];
			else goto ARG_LACK;
			break;

		case 'm':
			i++;
			mCreateMode = kCreateSingleMesh;
			if(i < mArgc) mMeshCode = mArgv[i];
			else goto ARG_LACK;
			break;

		case 'o':
			i++;
			if(i < mArgc) mOutDir = mArgv[i];
			else goto ARG_LACK;
			break;

		case 'd':
			i++;
			if(i < mArgc) mDivNum = atoi(mArgv[i]);
			else goto ARG_LACK;
			break;

		default:
			cout << "�s���ȃI�v�V�����ł�: " << mArgv[i] << endl;
			return false;

		}

	}

	if(mOutDir.size() == 0) {
		cout << "�o�͐�f�B���N�g�����w�肳��Ă��܂���" << endl;
		return false;
	}

	switch(mCreateMode) {

	case kCreateNone:
		cout << "�I�v�V���� -a / -l / -m ���K�v�ł�" << endl;
		return false;

	case kCreateAllMesh:
		break;

	case kCreateMeshesInList:
		if(mFNMeshList.size() == 0) {
			cout << "���b�V�����X�g���w�肳��Ă��܂���" << endl;
			return false;
		}
		break;

	case kCreateSingleMesh:
		if(mMeshCode.size() != 8 || mMeshCode.find_first_not_of("1234567890") != string::npos) {
			cout << "���b�V���R�[�h���s���ł�" << endl;
			return false;
		}
		break;

	}

	if(mDivNum < 2 || DIV_MAX < mDivNum) {
		cout << "���������s���ł��i�͈�: 2�`" << DIV_MAX << "�j: " << mDivNum << endl;
		return false;
	}

	return true;

ARG_LACK:

	cout << "����������܂���" << endl;
	fnPrintUsage();
	return false;

}

void CreateExistFile::
fnPrintUsage()
{

	string	aExecFN(mArgv[0]);
	string::size_type	p = aExecFN.find_last_of('\\') + 1;
	if(p != string::npos) aExecFN.erase(0, p);

	cout << endl;
	cout << "�y�R�}���h���́z" << endl << endl;
	cout << "\t" << aExecFN << endl << endl;
	cout << "�y�������e�z" << endl << endl;
	cout << "\t���ݔ���t�@�C���𐶐�����" << endl << endl;
	cout << "�y�g�p�`���z" << endl << endl;
	cout << "\t" << aExecFN << " <�I�v�V����>" << endl << endl;
	cout << "�y�I�v�V�����z" << endl << endl;
	cout << "\t-a" << endl;
	cout << "\t\t�S�s�s�n�}���b�V���ɂ��Đ���" << endl;
	cout << "\t\t�i���b�V�����X�g�͓s�s�n�}���b�V�����C�����玩�������j" << endl << endl;
	cout << "\t-l <���b�V�����X�g>" << endl;
	cout << "\t\t���b�V�����X�g�t�@�C���Ő����Ώۃ��b�V�����w��" << endl << endl;
	cout << "\t-m <���b�V���R�[�h>" << endl;
	cout << "\t\t�w��R�[�h�̓s�s�n�}���b�V���̂ݐ���" << endl << endl;
	cout << "\t-d <������>" << endl;
	cout << "\t\t���ݔ�����s���G���A�P�ʂ��C�s�s�n�}���b�V���̕������Ŏw��" << endl;
	cout << "\t\t�f�t�H���g��32" << endl << endl;
	cout << "\t-o <�f�B���N�g��>" << endl;
	cout << "\t\t���ݔ���t�@�C���̏o�͐�f�B���N�g���w��" << endl;
	cout << "\t\t�i���ϐ� EXIST_DIR �ł��w��j" << endl << endl;
	cout << "\t�� �I�v�V������ -a / -l / -m ���K�{" << endl << endl << endl;

}

bool CreateExistFile::
fnMakeMeshList()
{

	switch(mCreateMode) {

	case kCreateAllMesh:

		// SearchByWhereClause ���g�� "Complete_f is not null" �̏����ŃJ�[�\���擾 �� ���b�V�����X�g�𐶐�
		{
			long	aCount;
			string	aWhere;
			IFeatureCursorPtr	ipFCur_CityMesh;

			aWhere = FD_COMPLETE_F " is not null";
			ipFCur_CityMesh = SearchByWhereClause(_T(aWhere.c_str()), ipFC_CityMesh, &aCount);

			if(aCount == 0) {
				cout << "�s�s�n�}���b�V�����P������܂���" << endl;
				return false;
			}

			IFeaturePtr			ipF_CityMesh;
			while(ipFCur_CityMesh->NextFeature(&ipF_CityMesh) == S_OK) {
				CComVariant		aVar_MeshCode;
				ipF_CityMesh->get_Value(mFID_CityMeshCode, &aVar_MeshCode);
				mMeshList.Add(aVar_MeshCode.lVal);
			}
		}

		break;

	case kCreateMeshesInList:

		// ���b�V�����X�g�t�@�C�����J��
		if(!mMeshList.Open(mFNMeshList.c_str())) {
			cout << "���b�V�����X�g�t�@�C�����J���܂���: " << mFNMeshList << endl;
			return false;
		}
		break;

	case kCreateSingleMesh:
		// �w�胁�b�V���ЂƂ����X�g�֒ǉ�
		if(!mMeshList.Add(mMeshCode.c_str())) {
			cout << "���b�V���R�[�h��ǉ��ł��܂���: " << mMeshCode << endl;
			return false;
		}
		break;

	default:
		break;

	}

	return true;

}

bool CreateExistFile::
fnCreateCache(const string &cMeshCode)
{

	static string aBMCode;

	if(aBMCode == cMeshCode.substr(0, 6)) return true;

	cout << "�L���b�V���쐬��..." << flush;

	aBMCode	= cMeshCode.substr(0, 6);

	ISpatialCacheManagerPtr	ipCacheMgr(ipWorkspace);

	string	aWhere;
	long	aCount;
	IFeatureCursorPtr	ipFCur_BaseMesh;
	aWhere	= FD_BASEMESHCODE " = " + aBMCode;
	ipFCur_BaseMesh = SearchByWhereClause(aWhere.c_str(), ipFC_BaseMesh, &aCount);
	if(aCount == 0) {
		ipCacheMgr->EmptyCache();
		cout << "���s" << endl;
		return false;
	}

	IFeaturePtr	ipF_BaseMesh;
	if(ipFCur_BaseMesh->NextFeature(&ipF_BaseMesh) == S_OK) {
		IEnvelopePtr	ipE_BaseMesh;
		if (FAILED(ipF_BaseMesh->get_Extent(&ipE_BaseMesh))) {
			cout << "���s" << endl;
			return false;
		}
		if (FAILED(ipCacheMgr->FillCache(ipE_BaseMesh))) {
			cout << "���s" << endl;
			return false;
		}
	}

	cout << "����" << endl;
	return true;

}

IPolygonPtr CreateExistFile::
fnCreateIncompletePoly(IEnvelopePtr ipE_CityMesh, long *cCount)
{

	// ���b�V�����w�i�|���S���̎擾
	long	aCount;
	IFeatureCursorPtr	ipFCur_CitySite;
	ipFCur_CitySite = SearchByShape(ipE_CityMesh, esriSpatialRelIntersects, ipFC_CitySite, &aCount);
	if(aCount == 0) {
		cout << "�w�i�|���S�����P���Ȃ�" << endl;
		*cCount = -1;
		return NULL;
	}

	IPolygonPtr	ipPoly_Incomplete(CLSID_Polygon);
	IGeometryBagPtr	a_ipGeomBag(CLSID_GeometryBag);
	IGeometryCollectionPtr	ipGCol_Incomplete(a_ipGeomBag);

	ISpatialReferencePtr	a_ipSpRef;	// ��ԎQ��

	// �w�i�|���S���̃��[�v
	*cCount = 0;
	IFeaturePtr	ipF_CitySite;
	while(ipFCur_CitySite->NextFeature(&ipF_CitySite) == S_OK) {

		// �s���S�|���S���݂̂𒊏o
		CComVariant	aTmpVar;
		if (FAILED(ipF_CitySite->get_Value(mFID_BGClass_C, &aTmpVar)) ) {
			cout << "BGCLASS_C�̎擾�Ɏ��s" << endl;
			*cCount = -1;
			return NULL;
		}
		if(aTmpVar.lVal != 60) continue;

		IGeometryPtr	ipG_CitySite;
		ipF_CitySite->get_ShapeCopy(&ipG_CitySite);

		// ���b�V����`�Ƀ^�b�`���Ă��邾���̂��̂͏��O
		VARIANT_BOOL	aTouches;
		if (FAILED(((IRelationalOperatorPtr)ipE_CityMesh)->Touches(ipG_CitySite, &aTouches))) {
			cout << "�^�b�`�̔���Ɏ��s" << endl;
			*cCount = -1;
			return NULL;
		}
		if(aTouches) continue;

		if((*cCount) == 0 ) {
			if (FAILED(ipG_CitySite->get_SpatialReference ( &a_ipSpRef ))) {
				cout << "SpatialReference�擾�Ɏ��s" << endl;
				*cCount = -1;
				return NULL;
			}
		}

		(*cCount)++;
		if (FAILED(ipGCol_Incomplete->AddGeometry(ipG_CitySite))) {
			cout << "Geometry�ǉ��Ɏ��s" << endl;
			*cCount = -1;
			return NULL;
		}
	}

	IEnumGeometryPtr	ipEG_Incomplete(ipGCol_Incomplete);
	long	aIncompCount;
	if (FAILED(ipEG_Incomplete->get_Count(&aIncompCount))) {
		cout << "�|���S�����̎擾�Ɏ��s" << endl;
		*cCount = -1;
		return NULL;
	}
	if(aIncompCount == 0) {		// �s���S�|���S�����P���Ȃ�
		*cCount = 0;
		return NULL;
	}

	if (FAILED(a_ipGeomBag->putref_SpatialReference ( a_ipSpRef ))) {
		cout << "SpatialReference�̓K�p�Ɏ��s" << endl;
		*cCount = 0;
		return NULL;
	}

	// �s���S�|���S���𓝍��i�|���S�� or �}���`�|���S���j
	if (FAILED(((ITopologicalOperatorPtr)ipPoly_Incomplete)->ConstructUnion(ipEG_Incomplete))) {
		cout << "�s���S�|���S���̓����Ɏ��s" << endl;
		*cCount = 0;
		return NULL;
	}
	return ipPoly_Incomplete;

}

bool CreateExistFile::
Create(int cArgc, char **cArgv)
{

	mArgc	= cArgc;
	mArgv	= cArgv;

	if(mDBMap.size() == 0) {
		cout << "���ϐ� DB_MAP ���ݒ肳��Ă��܂���" << endl;
		return false;
	}

	if(!fnAnalyzeArg()) return false;

	string aExistLine(mDivNum, 'o');

	// �ڑ�
	if(!Connect(mDBMap.c_str())) {
		return false;
	}

	// �t�B�[�`���N���X�擾
	ipFC_CityMesh = GetFeatureClass(mFCCityMesh.c_str());	if(ipFC_CityMesh == NULL)	return false;
	ipFC_BaseMesh = GetFeatureClass(mFCBaseMesh.c_str());	if(ipFC_BaseMesh == NULL)	return false;
	ipFC_CitySite = GetFeatureClass(mFCCitySite.c_str());	if(ipFC_CitySite == NULL)	return false;

	// �t�B�[���h�C���f�b�N�X�擾...
	if(!GetFieldID(ipFC_CityMesh,	_T(FD_CITYMESHCODE),	&mFID_CityMeshCode))	return false;
	if(!GetFieldID(ipFC_BaseMesh,	_T(FD_BASEMESHCODE),	&mFID_BaseMeshCode))	return false;
	if(!GetFieldID(ipFC_CityMesh,	_T(FD_COMPLETE_F),		&mFID_CompleteFlag))	return false;
	if(!GetFieldID(ipFC_CitySite,	_T(FD_BGCLASS_C),		&mFID_BGClass_C))		return false;

	// ���b�V�����X�g�̎擾
	if(!fnMakeMeshList()) return false;

	// �f�o�b�O�p
	//string	a_sFName	= "C:\\kudo\\SVN\\SiNDY-u\\CreateExistFile\\test\\test.shp";
	//GeomtoShapeFile	a_gGeoToShp;
	//a_gGeoToShp.Init ( a_sFName, esriGeometryPolygon );

	// ���b�V�����X�g�̃��[�v
	MeshListSet_i	aMLItr;
	for(aMLItr = mMeshList.begin(); aMLItr != mMeshList.end(); aMLItr++) {

		// �L���b�V���ݒ�
//		if (! fnCreateCache((*aMLItr).eMeshCode.c_str()))
//			return false;

		// �s�s�n�}���b�V���̃J�[�\���擾
		IFeatureCursorPtr	ipFCur_CityMesh;
		long	aMeshCount;
		string	aWhere;

		aWhere	= FD_CITYMESHCODE " = " + (*aMLItr).eMeshCode;

		ipFCur_CityMesh	= SearchByWhereClause(aWhere.c_str(), ipFC_CityMesh, &aMeshCount);
		if(aMeshCount == 0) {
			cout << (*aMLItr).eMeshCode << ": �s�s�n�}���b�V�����擾�ł��܂���" << endl;
			ExistError = true;
			continue;
		}

		// �s�s�n�}���b�V���̃��[�v�i�P��̂݁j
		IFeaturePtr		ipF_CityMesh;
		while(ipFCur_CityMesh->NextFeature(&ipF_CityMesh) == S_OK) {

			int		i, j;
			FILE	*fp;
			char	aCurDirName[_MAX_DIR], aFileName[_MAX_FNAME];

			if(ipF_CityMesh == NULL) break;

			// �s�s�n�}���b�V���̃W�I���g�����G���x���[�v�擾
			IEnvelopePtr	ipE_CityMesh;
			if (FAILED(ipF_CityMesh->get_Extent(&ipE_CityMesh))) {
				cout << (*aMLItr).eMeshCode << ":�s�s�n�}���b�V���̃G���x���[�v�擾�Ɏ��s" << endl;
				return false;
			}

			// ���b�V���R�[�h�C���S�^�s���S�t���O�̎擾
			CComVariant	aVar_MeshCode, aVar_CompleteFlag;
			if (FAILED(ipF_CityMesh->get_Value(mFID_CityMeshCode, &aVar_MeshCode)) ||
				FAILED(ipF_CityMesh->get_Value(mFID_CompleteFlag, &aVar_CompleteFlag))) {
					cout << (*aMLItr).eMeshCode << ":���b�V���R�[�h or ���S/�s���S�t���O�̎擾�Ɏ��s" << endl;
					return false;
			}

			// �J�����g�f�B���N�g���ݒ�
			sprintf_s(aCurDirName, _MAX_DIR, "%s\\%4d", mOutDir.c_str(), aVar_MeshCode.lVal / 10000);
			if(_chdir(aCurDirName) == -1) {
				cout << "�f�B���N�g���V�K�쐬: " << aCurDirName << " ..." << endl;
				if(_mkdir(aCurDirName) == -1) {
					cout << "�쐬�ł��܂���" << endl;
					return false;
				}
				cout << "����" << endl;
				if(_chdir(aCurDirName) == -1) {
					cout << "�J�����g�f�B���N�g����ύX�ł��܂���" << endl;
					return false;
				}
			}

			// exist�t�@�C���I�[�v��
			sprintf_s(aFileName, _MAX_FNAME, "%8ld.exi", aVar_MeshCode.lVal);
			cout << aFileName << " �쐬��..." << flush;
			fp = fopen(aFileName, "wb");
			if(fp == NULL) {
				cout << "�I�[�v���ł��܂���" << endl;
				return false;
			}

			// �s���S�|���S���擾
			IPolygonPtr		ipPoly_Incomplete;
			long	aIncompCount;
			ipPoly_Incomplete = fnCreateIncompletePoly(ipE_CityMesh, &aIncompCount);
			if(aIncompCount < 0) {			// �w�i�|���S�����P������
				ExistError = true;
				continue;
			} else if(aIncompCount == 0) {	// �s���S�G���A������
				// �w�b�_�����o��
				fprintf(fp, "%8ld\tO\t%d\t%d\n", aVar_MeshCode.lVal, mDivNum, mDivNum);

				// �f�[�^�����o��
				for(i = 0; i < mDivNum; i++) {
					fprintf(fp, "%s\n", aExistLine.c_str());
				}
			} else {

				// �w�b�_�����o��
				fprintf(fp, "%8ld\tV\t%d\t%d\n", aVar_MeshCode.lVal, mDivNum, mDivNum);

				// �s�s�n�}���b�V���̎l�����W�擾
				double	aXMin, aXMax, aYMin, aYMax, aWidth, aHeight;
				if (FAILED(ipE_CityMesh->get_XMin(&aXMin)) ||
					FAILED(ipE_CityMesh->get_XMax(&aXMax)) ||
					FAILED(ipE_CityMesh->get_YMin(&aYMin)) ||
					FAILED(ipE_CityMesh->get_YMax(&aYMax)) ||
					FAILED(ipE_CityMesh->get_Width(&aWidth)) ||
					FAILED(ipE_CityMesh->get_Height(&aHeight))) {
						cout << (*aMLItr).eMeshCode << ":���b�V�����W�擾���s" << endl;
						fclose(fp);
						return false;
				}

				ISpatialReferencePtr	ipSpRef;
				ipE_CityMesh->get_SpatialReference ( &ipSpRef );

				// ���S�^�s���S����E�f�[�^�����o���i�Z���̃��[�v�j
				for(i = 0; i < mDivNum; i++) {

					for(j = 0; j < mDivNum; j++) {

						/**** �Z���̐ݒ�E�J�n ****/

						// �Z���̍��W�v�Z
						double	aX_Cell[2], aY_Cell[2];
						aX_Cell[0]	= aXMin + aWidth	* j			/ mDivNum;	// XMin
						aX_Cell[1]	= aXMin + aWidth	* (j + 1)	/ mDivNum;	// XMax
						aY_Cell[0]	= aYMax - aHeight	* (i + 1)	/ mDivNum;	// YMin
						aY_Cell[1]	= aYMax - aHeight	* i			/ mDivNum;	// YMax

						// �_�� �� Ring
						IRingPtr			ipR_Cell(CLSID_Ring);
						IPointCollectionPtr	ipPtCol_Cell(ipR_Cell);
						for(int n = 0; n < 5; n++) {
							IPointPtr	ipPoint(CLSID_Point);
							if (FAILED(ipPoint->put_X(aX_Cell[(int)((n + 1)/ 2) % 2])) ||
								FAILED(ipPoint->put_Y(aY_Cell[(int)((n + 2)/ 2) % 2]))) {
									cout << "�Z���̍��W�t�^�Ɏ��s" << endl;
									fclose(fp);
									return false;
							}
							if (FAILED(ipPtCol_Cell->AddPoint(ipPoint))) {
								cout << "�Z���̓_�ǉ��Ɏ��s" << endl;
								fclose(fp);
								return false;
							}
						}

						// Ring �� Polygon
						IPolygonPtr				ipPgn_Cell(CLSID_Polygon);
						IGeometryCollectionPtr	ipGCol_Cell(ipPgn_Cell);
						if (FAILED(ipGCol_Cell->AddGeometry(ipR_Cell))) {
							cout << "�Z���̒ǉ��Ɏ��s" << endl;
							fclose(fp);
							return false;
						}

						if (FAILED(ipPgn_Cell->putref_SpatialReference ( ipSpRef ))) {
							cout << "SpatialReference�K�p�Ɏ��s" << endl;
							fclose(fp);
							return false;
						}

						/**** �Z���̐ݒ�E�I�� ****/

						IRelationalOperatorPtr	ipROp_Cell(ipPgn_Cell);
						VARIANT_BOOL	aWithin, aOverlap;

						if(FAILED(ipROp_Cell->Within(ipPoly_Incomplete, &aWithin))) {
							cerr << "Within ���s: " << (*aMLItr).eMeshCode << " (" << setw(3) << j << ", " << setw(3) << i << ")" << endl;
							ExistError = true;
							aWithin = VARIANT_FALSE;
						}
						if(aWithin) {
							fprintf(fp, "x");
							continue;
						}

						if(FAILED(ipROp_Cell->Overlaps(ipPoly_Incomplete, &aOverlap))) {
							cerr << "Overlaps ���s: " << (*aMLItr).eMeshCode << " (" << setw(3) << j << ", " << setw(3) << i << ")" << endl;
							ExistError = true;
							aOverlap = VARIANT_FALSE;
						}
						if(aOverlap) {
							fprintf(fp, "v");
							continue;
						}

						fprintf(fp, "o");

					}

					fprintf(fp, "\n");

				}

			}
		//	a_gGeoToShp.OuttoShapeFile ( (IGeometryPtr)ipPoly_Incomplete );
			cout << "����" << endl;

			fclose(fp);

		}

	}
	if (ExistError)
		return false;
	else
		return true;

}
