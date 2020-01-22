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

// Anno.cpp: Anno �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <crd_cnv.h>
#include "Anno.h"

namespace {
	LPCSTR ANNO_ATTR_FIELD = "ANNOCLASS_C";
	LPCSTR ANNO_NAME1_FIELD = "NAME_STRING1";
	LPCSTR ANNO_DISP_FIELD = "DISPTYPE_C";
	LPCSTR ANNO_SCDISP1_FIELD = "SC1DISPTYPE_C";
	LPCSTR ANNO_SCDISP2_FIELD = "SC2DISPTYPE_C";
	LPCSTR ANNO_SCDISP3_FIELD = "SC3DISPTYPE_C";
	LPCSTR ANNO_SCDISP4_FIELD = "SC4DISPTYPE_C";

	LPCSTR ANNO_DISPTYPE[] = {
		ANNO_SCDISP1_FIELD,
		ANNO_SCDISP2_FIELD,
		ANNO_SCDISP3_FIELD,
		ANNO_SCDISP4_FIELD,
		NULL
	};
	LPCSTR ROAD_ATTR_FIELD = "FACILCLASS_C";
	LPCSTR KANJI_ATTR_FIELD = "NAME_KANJI";
	LPCSTR ROAD_CODE_FIELD = "ROAD_CODE";
	LPCSTR DUMMY_F_FIELD = "DUMMYFACIL_F";
	LPCSTR SITE_ATTR_FIELD = "BGCLASS_C";
	const LONG DEF_FUKANZEN = 60;

	LPCSTR COMP_FIELD = "CREATE_YEAR";	//2008/06/06 �G���A������p�Ɍ���t�B�[���h�ύX

	//#define KIGO_SIZE (0.000030517575) //�s�s�n�}��16�|�C���g�̔����̂���
	const double KIGO_SIZE = 0.00001; //�x�����[�g���ɕϊ�

	/*
	  ���[���e�[�u��
	  �v�f�O���珇��M1,M2�E�E�E�E,�s�s�n�}
	  �e�v�f�̂P�O�̈ʂ́u������̂݁v�u�L���̂݁v���̐���������
	  �iDISPTYPE�̃h���C���Ɠ����j
		0	�\���Ȃ�
		1	������̂�
		2	�L���{������
		4	�L���̂�

	  �e�v�f�̂P�̈ʂ́A�K���擾�A�擾���Ȃ��Ă��悢�Ȃǂ�����
		0	�擾���Ă͂����Ȃ�
		1	�擾���Ă��悢
		2	�K���擾
		3	�K���擾�i�y�A���l���F�ۗ��j
	*/
	//  IC                       M1  M2  M3  M4  S1  S2  S3  S4  T
	const LONG gRule1204100[] = {00, 00, 42, 12, 00, 00, 00, 00, 13};	//�h�b
	const LONG gRule1204101[] = {00, 00, 00, 00, 12, 12, 12, 13, 00};	//�h�b�i���������ԓ��j
	const LONG gRule1204102[] = {00, 00, 00, 00, 11, 12, 12, 12, 00};	//�h�b�i�L�������j
	const LONG gRule1204103[] = {00, 00, 00, 00, 11, 11, 11, 11, 00};	//�h�b�i�����j
	const LONG gRule1204104[] = {00, 00, 00, 00, 11, 12, 12, 12, 00};	//�h�b�i�L����v���j
	const LONG gRule1204105[] = {00, 00, 00, 00, 11, 11, 11, 11, 00};	//�h�b�i��v���j
	const LONG gRule1204106[] = {00, 00, 00, 00, 11, 12, 12, 12, 00};	//�h�b�i�L�������j
	const LONG gRule1204107[] = {00, 00, 00, 00, 11, 11, 11, 11, 00};	//�h�b�i�����j
	const LONG gRule1204108[] = {00, 00, 00, 00, 11, 12, 12, 12, 00};	//�h�b�i�L�����̑����j
	const LONG gRule1204109[] = {00, 00, 00, 00, 11, 11, 11, 11, 00};	//�h�b�i���̑����j
	const LONG gRule1204111[] = {00 ,00, 42, 12, 12, 12, 12, 13, 13};	//�h�b�i�X�}�[�g�h�b�j
	const LONG gRule1204112[] = {00 ,00, 00, 00, 12, 12, 12, 13, 00};	//�h�b�i�����������j

	//  �����v                   M1  M2  M3  M4  S1  S2  S3  S4  T
	const LONG gRule1204200[] = {00 ,00, 00, 11, 00, 00, 00, 00, 13};	//�����v
	const LONG gRule1204201[] = {00 ,00, 00, 00, 11, 11, 12, 13, 00};	//�����v�i�s�s�������j
	const LONG gRule1204202[] = {00 ,00, 00, 00, 11, 12, 12, 12, 00};	//�����v�i�L�������j
	const LONG gRule1204203[] = {00 ,00, 00, 00, 11, 11, 11, 11, 00};	//�����v�i�����j
	const LONG gRule1204204[] = {00 ,00, 00, 00, 11, 12, 12, 12, 00};	//�����v�i�L����v���j
	const LONG gRule1204205[] = {00 ,00, 00, 00, 11, 11, 11, 11, 00};	//�����v�i��v���j
	const LONG gRule1204206[] = {00 ,00, 00, 00, 11, 12, 12, 12, 00};	//�����v�i�L�������j
	const LONG gRule1204207[] = {00 ,00, 00, 00, 11, 11, 11, 11, 00};	//�����v�i�����j
	const LONG gRule1204208[] = {00 ,00, 00, 00, 11, 12, 12, 12, 00};	//�����v�i�L�����̑���
	const LONG gRule1204209[] = {00 ,00, 00, 00, 11, 11, 11, 11, 00};	//�����v�i���̑����j

	//  JC                       M1  M2  M3  M4  S1  S2  S3  S4  T
	const LONG gRule1204300[] = {00 ,00, 00, 00, 00, 00, 00, 00, 13};	//�i�b
	const LONG gRule1204301[] = {00 ,00, 00, 00, 12, 12, 12, 12, 00};	//�i�b�i���������ԓ��j
	const LONG gRule1204302[] = {00 ,00, 00, 00, 11, 12, 12, 12, 00};	//�i�b�i�s�s�������j
	const LONG gRule1204303[] = {00 ,00, 00, 00, 11, 12, 12, 12, 00};	//�i�b�i�L�������j
	const LONG gRule1204304[] = {00 ,00, 00, 00, 11, 11, 11, 11, 00};	//�i�b�i�����j
	const LONG gRule1204305[] = {00 ,00, 00, 00, 11, 12, 12, 12, 00};	//�i�b�i�L����v���j
	const LONG gRule1204306[] = {00 ,00, 00, 00, 11, 11, 11, 11, 00};	//�i�b�i��v���j
	const LONG gRule1204307[] = {00 ,00, 00, 00, 11, 12, 12, 12, 00};	//�i�b�i�L�������j
	const LONG gRule1204308[] = {00 ,00, 00, 00, 11, 11, 11, 11, 00};	//�i�b�i�����j
	const LONG gRule1204309[] = {00 ,00, 00, 00, 11, 12, 12, 12, 00};	//�i�b�i�L�����̑����j
	const LONG gRule1204310[] = {00 ,00, 00, 00, 11, 11, 11, 11, 00};	//�i�b�i���̑����j
	const LONG gRule1204311[] = {00 ,00, 00, 00, 12, 12, 12, 12, 00};	//�i�b�i�����������j

	//  SA                       M1  M2  M3  M4  S1  S2  S3  S4  T
	const LONG gRule1204400[] = {00 ,00, 00, 00, 00, 00, 00, 00, 23};	//�r�`
	const LONG gRule1204401[] = {00 ,00, 00, 00, 22, 22, 22, 23, 00};	//�r�`�i���������ԓ��j
	const LONG gRule1204402[] = {00 ,00, 00, 00, 21, 22, 22, 22, 00};	//�r�`�i�s�s�������j
	const LONG gRule1204403[] = {00 ,00, 00, 00, 21, 22, 22, 22, 00};	//�r�`�i�L�������j
	const LONG gRule1204404[] = {00 ,00, 00, 00, 21, 21, 21, 21, 00};	//�r�`�i�����j
	const LONG gRule1204405[] = {00 ,00, 00, 00, 21, 22, 22, 22, 00};	//�r�`�i�L����v���j
	const LONG gRule1204406[] = {00 ,00, 00, 00, 21, 21, 21, 21, 00};	//�r�`�i��v���j
	const LONG gRule1204407[] = {00 ,00, 00, 00, 21, 22, 22, 22, 00};	//�r�`�i�L�������j
	const LONG gRule1204408[] = {00 ,00, 00, 00, 21, 21, 21, 21, 00};	//�r�`�i�����j
	const LONG gRule1204409[] = {00 ,00, 00, 00, 21, 22, 22, 22, 00};	//�r�`�i�L�����̑����j
	const LONG gRule1204410[] = {00 ,00, 00, 00, 21, 21, 21, 21, 00};	//�r�`�i���̑����j
	const LONG gRule1204411[] = {00 ,00, 00, 00, 22, 22, 22, 23, 00};	//�r�`�i�����������j

	//  PA                       M1  M2  M3  M4  S1  S2  S3  S4  T
	const LONG gRule1204500[] = {00, 00, 00, 00, 00, 00, 00, 00, 23};	//�o�`	
	const LONG gRule1204501[] = {00, 00, 00, 00, 22, 22, 22, 23, 00};	//�o�`�i���������ԓ��j	
	const LONG gRule1204502[] = {00, 00, 00, 00, 21, 22, 22, 22, 00};	//�o�`�i�s�s�������j	
	const LONG gRule1204503[] = {00, 00, 00, 00, 21, 22, 22, 22, 00};	//�o�`�i�L�������j	
	const LONG gRule1204504[] = {00, 00, 00, 00, 21, 21, 21, 21, 00};	//�o�`�i�����j	
	const LONG gRule1204505[] = {00, 00, 00, 00, 21, 22, 22, 22, 00};	//�o�`�i�L����v���j	
	const LONG gRule1204506[] = {00, 00, 00, 00, 21, 21, 21, 21, 00};	//�o�`�i��v���j	
	const LONG gRule1204507[] = {00, 00, 00, 00, 21, 22, 22, 22, 00};	//�o�`�i�L�������j	
	const LONG gRule1204508[] = {00, 00, 00, 00, 21, 21, 21, 21, 00};	//�o�`�i�����j	
	const LONG gRule1204509[] = {00, 00, 00, 00, 21, 22, 22, 22, 00};	//�o�`�i�L�����̑����j	
	const LONG gRule1204510[] = {00, 00, 00, 00, 21, 21, 21, 21, 00};	//�o�`�i���̑����j	
	const LONG gRule1204511[] = {00, 00, 00, 00, 22, 22, 22, 23, 00};	//�o�`�i�����������j	

//	const LONG gRule1204600[] = {00, 00, 0,0,0,0,0,0,0};	//
}

KIND_TABLE Anno::m_TargetKind[] = {
	KIND_TABLE(7, 1204111, gRule1204111),
	KIND_TABLE(5, 1204100, gRule1204100),
	KIND_TABLE(5, 1204101, gRule1204101),
	KIND_TABLE(5, 1204102, gRule1204102),
	KIND_TABLE(5, 1204103, gRule1204103, true, false),
	KIND_TABLE(5, 1204104, gRule1204104),
	KIND_TABLE(5, 1204105, gRule1204105, true, false),
	KIND_TABLE(5, 1204106, gRule1204106),
	KIND_TABLE(5, 1204107, gRule1204107, true, false),
	KIND_TABLE(5, 1204108, gRule1204108),
	KIND_TABLE(5, 1204109, gRule1204109, true, false),
	KIND_TABLE(5, 1204112, gRule1204112, true),	//Add
	KIND_TABLE(4, 1204200, gRule1204200),
	KIND_TABLE(4, 1204201, gRule1204201),
	KIND_TABLE(4, 1204202, gRule1204202),
	KIND_TABLE(4, 1204203, gRule1204203, true, false),
	KIND_TABLE(4, 1204204, gRule1204204),
	KIND_TABLE(4, 1204205, gRule1204205, true, false),
	KIND_TABLE(4, 1204206, gRule1204206),
	KIND_TABLE(4, 1204207, gRule1204207, true, false),
	KIND_TABLE(4, 1204208, gRule1204208),
	KIND_TABLE(4, 1204209, gRule1204209, true, false),
	KIND_TABLE(3, 1204300, gRule1204300),
	KIND_TABLE(3, 1204301, gRule1204301),
	KIND_TABLE(3, 1204302, gRule1204302),
	KIND_TABLE(3, 1204303, gRule1204303),
	KIND_TABLE(3, 1204304, gRule1204304, true, false),	//Add
	KIND_TABLE(3, 1204305, gRule1204305),
	KIND_TABLE(3, 1204306, gRule1204306, true, false),	//Add
	KIND_TABLE(3, 1204307, gRule1204307),
	KIND_TABLE(3, 1204308, gRule1204308, true, false),	//Add
	KIND_TABLE(3, 1204309, gRule1204309),
	KIND_TABLE(3, 1204310, gRule1204310, true, false),	//Add
	KIND_TABLE(3, 1204311, gRule1204311, true),
	KIND_TABLE(2, 1204500, gRule1204500),	//Add
	KIND_TABLE(2, 1204501, gRule1204501),	//Add
	KIND_TABLE(2, 1204502, gRule1204502),	//Add
	KIND_TABLE(2, 1204503, gRule1204503),	//Add
	KIND_TABLE(2, 1204504, gRule1204504, true, false),	//Add
	KIND_TABLE(2, 1204505, gRule1204505),	//Add
	KIND_TABLE(2, 1204506, gRule1204506, true, false),	//Add
	KIND_TABLE(2, 1204507, gRule1204507),	//Add
	KIND_TABLE(2, 1204508, gRule1204508, true, false),	//Add
	KIND_TABLE(2, 1204509, gRule1204509),	//Add
	KIND_TABLE(2, 1204510, gRule1204510, true, false),	//Add
	KIND_TABLE(2, 1204511, gRule1204511, true),	//Add
	KIND_TABLE(1, 1204400, gRule1204400),	//Add
	KIND_TABLE(1, 1204401, gRule1204401),	//Add
	KIND_TABLE(1, 1204402, gRule1204402),	//Add
	KIND_TABLE(1, 1204403, gRule1204403),	//Add
	KIND_TABLE(1, 1204404, gRule1204404, true, false),	//Add
	KIND_TABLE(1, 1204405, gRule1204405),	//Add
	KIND_TABLE(1, 1204406, gRule1204406, true, false),	//Add
	KIND_TABLE(1, 1204407, gRule1204407),	//Add
	KIND_TABLE(1, 1204408, gRule1204408, true, false),	//Add
	KIND_TABLE(1, 1204409, gRule1204409),	//Add
	KIND_TABLE(1, 1204410, gRule1204410, true, false),	//Add
	KIND_TABLE(1, 1204411, gRule1204411, true),	//Add
//	KIND_TABLE(6, 1204600, 1204600),
	KIND_TABLE(-1, -1, NULL)
};

LPCSTR Anno::m_RoadKind[] = {
	"",
	"SA",
	"PA",
	"JCT",
	"�����v",
	"IC",
	"������",
	"�X�}�[�gIC",
	NULL
};

#define TEST

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

Anno::Anno()
{
	m_ipMesh = NULL;
}

Anno::Anno(IWorkspacePtr ipMapWorkspace, IWorkspacePtr ipAnnoWorkspace, IWorkspacePtr ipRodWorkSpace)
{
	m_ipMapWorkspace = ipMapWorkspace;
	m_ipAnnoWorkspace = ipAnnoWorkspace;
	m_ipRodWorkspace = ipRodWorkSpace;
	m_ipMesh = NULL;
}

Anno::~Anno()
{

}

bool Anno::init(FILE *pFile, LPCTSTR cAnnoClass, LPCTSTR cRoadClass, double cPower, int cMode)
{
	IFeatureWorkspacePtr ipMapFeatureWorkspace(m_ipMapWorkspace);
	ATLASSERT( ipMapFeatureWorkspace );
	IFeatureWorkspacePtr ipAnnoFeatureWorkspace(m_ipAnnoWorkspace);
	ATLASSERT( ipAnnoFeatureWorkspace );
	IFeatureWorkspacePtr ipRodFeatureWorkspace(m_ipRodWorkspace);
	ATLASSERT( ipRodFeatureWorkspace );
	m_pFile = pFile;
	m_Mode = cMode;

	if(FAILED(ipAnnoFeatureWorkspace->OpenFeatureClass(CComBSTR(_T(cAnnoClass)), &m_ipAnno))){
		fprintf(stderr, "#���L�^�ʒu�̃t�B�[�`���[���J���܂���B,%s\n", cAnnoClass);
		fprintf(m_pFile, "#���L�^�ʒu�̃t�B�[�`���[���J���܂���B,%s\n", cAnnoClass);
		return false;
	}else{
		CComBSTR aLayerName;
		((IDatasetPtr)m_ipAnno)->get_Name(&aLayerName);
		m_AnnoLayerName = aLayerName;
	}

	if(FAILED(ipRodFeatureWorkspace->OpenFeatureClass(CComBSTR(_T(cRoadClass)), &m_ipRoad))){
		fprintf(stderr, "#���H�{�݃t�B�[�`���[���J���܂���B,%s\n", cRoadClass);
		fprintf(m_pFile, "#���H�{�݃t�B�[�`���[���J���܂���B,%s\n", cRoadClass);
		return false;
	}else{
		CComBSTR aLayerName;
		((IDatasetPtr)m_ipRoad)->get_Name(&aLayerName);
		m_RoadLayerName = aLayerName;
	}

	m_ipAnno->FindField(CComBSTR(ANNO_NAME1_FIELD), &m_AnnoName1Index);
	if(m_AnnoName1Index < 0){
		fprintf(stderr, "#���L���̂P�̃t�B�[���h��������܂���B,%s\n", ANNO_NAME1_FIELD);
		fprintf(m_pFile, "#���L���̂P�̃t�B�[���h��������܂���B,%s\n", ANNO_NAME1_FIELD);
		return false;
	}
	m_ipAnno->FindField(CComBSTR(ANNO_ATTR_FIELD), &m_AnnoAttrIndex);
	if(m_AnnoAttrIndex < 0){
		fprintf(stderr, "#���L��ʂ̃t�B�[���h��������܂���B,%s\n", ANNO_ATTR_FIELD);
		fprintf(m_pFile, "#���L��ʂ̃t�B�[���h��������܂���B,%s\n", ANNO_ATTR_FIELD);
		return false;
	}
	if(m_Mode == 2){
		//�s�s�n�}
		m_ipAnno->FindField(CComBSTR(ANNO_DISP_FIELD), &m_AnnoDispTypeIndex[0]);
		if(m_AnnoDispTypeIndex[0] < 0){
			fprintf(stderr, "#���L�\���^�C�v�̃t�B�[���h��������܂���B,%s\n", ANNO_DISP_FIELD);
			fprintf(m_pFile, "#���L�\���^�C�v�̃t�B�[���h��������܂���B,%s\n", ANNO_DISP_FIELD);
			return false;
		}
		m_AnnoDispTypeIndex[1] = m_AnnoDispTypeIndex[2] = m_AnnoDispTypeIndex[3] = -1;
		m_RuleOffset = 8;
		m_RoadCodeIndex = -1;
	}else{
		//�x�[�X�E�~�h��
		for(int i = 0; ANNO_DISPTYPE[i] != NULL; i++){
			m_ipAnno->FindField(CComBSTR(ANNO_DISPTYPE[i]), &m_AnnoDispTypeIndex[i]);
			if(m_AnnoDispTypeIndex[i] < 0){
				fprintf(stderr, "#���L�\���^�C�v�̃t�B�[���h��������܂���B,%s\n", ANNO_DISPTYPE[i]);
				fprintf(m_pFile, "#���L�\���^�C�v�̃t�B�[���h��������܂���B,%s\n", ANNO_DISPTYPE[i]);
				return false;
			}
		}
		if(m_Mode ==1){
			//�x�[�X
			m_RuleOffset = 4;
			m_RoadCodeIndex = -1;
		}else{
			//�~�h��
			m_RuleOffset = 0;
			m_ipRoad->FindField(CComBSTR(ROAD_CODE_FIELD), &m_RoadCodeIndex);
			if(m_RoadCodeIndex < 0){
				fprintf(stderr, "#�H���R�[�h�̃t�B�[���h��������܂���B,%s\n", ROAD_CODE_FIELD);
				fprintf(m_pFile, "#�H���R�[�h�̃t�B�[���h��������܂���B,%s\n", ROAD_CODE_FIELD);
				return false;
			}
		}
	}
	m_ipRoad->FindField(CComBSTR(ROAD_ATTR_FIELD), &m_RoadAttrIndex);
	if(m_RoadAttrIndex < 0){
		fprintf(stderr, "#���H�{�ݑ����̃t�B�[���h��������܂���B,%s\n", ROAD_ATTR_FIELD);
		fprintf(m_pFile, "#���H�{�ݑ����̃t�B�[���h��������܂���B,%s\n", ROAD_ATTR_FIELD);
		return false;
	}
	m_ipRoad->FindField(CComBSTR(KANJI_ATTR_FIELD), &m_KanjiAttrIndex);
	if(m_KanjiAttrIndex < 0){
		fprintf(stderr, "#���H�{�݊������̃t�B�[���h��������܂���B,%s\n", KANJI_ATTR_FIELD);
		fprintf(m_pFile, "#���H�{�݊������̃t�B�[���h��������܂���B,%s\n", KANJI_ATTR_FIELD);
		return false;
	}
	m_ipRoad->FindField(CComBSTR(DUMMY_F_FIELD), &m_DummyIndex);
	if(m_DummyIndex < 0){
		fprintf(stderr, "#���H�{�݃_�~�[�t���O�t�B�[���h��������܂���B,%s\n", DUMMY_F_FIELD);
		fprintf(m_pFile, "#���H�{�݃_�~�[�t���O�t�B�[���h��������܂���B,%s\n", DUMMY_F_FIELD);
		return false;
	}

	fnSetNeedRank();
	m_BufDist = KIGO_SIZE * cPower;
	return m_AnnoConv.init(m_ipAnno);
}

bool Anno::execCheck()
{
	bool aResult = true;
	fprintf(m_pFile, "#臒l%.2f�Ŏ��s�B\n", m_BufDist / KIGO_SIZE);
	fprintf(stderr, "臒l%.2f�Ŏ��s���܂��B\n", m_BufDist / KIGO_SIZE);
	for(long i = 1; i < 8; ++i){
		if(i == 6){
			//������
			continue;
		}
		if(m_NeedRank[i] != 0){
			aResult &= fnPointCheck(i, m_pFile);
		}
		aResult &= fnPointCheck2(i, m_pFile);
		m_ID_Map.clear();
	}
	return aResult;
}

bool Anno::fnPointCheck(const LONG cRoadKind, FILE *cpFile)
{
	printf("���H�f�[�^��%s���������Ă��܂��B\n", m_RoadKind[cRoadKind]);
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString aWhere;
	aWhere.Format("%s = %d", ROAD_ATTR_FIELD, cRoadKind);
	ipFilter->put_WhereClause(CComBSTR(aWhere));
	long aTotal = 0;
	m_ipRoad->FeatureCount(ipFilter, &aTotal);
	IFeatureCursorPtr ipCursor;
	if(FAILED(m_ipRoad->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fprintf(stderr, "Error,%s�������s\n", m_RoadKind[cRoadKind]);
		fprintf(cpFile, "#Error,%s�������s\n", m_RoadKind[cRoadKind]);
		return false;
	}
	printf("�����I���B\n");
	bool aResult = true;
	IFeaturePtr ipFeature;
	long aCount = 0;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		if(fnIsTarget(ipFeature)){
			aResult &= fnPointComp(ipFeature, cRoadKind, cpFile);
		}
		aCount++;
		printf("%d/%d\r", aCount, aTotal);
	}
	printf("\n");
	return aResult;
}

bool Anno::fnPointComp(IFeaturePtr ipFeature, const LONG cRoadKind, FILE *cpFile)
{
	LONG aObjID;
	ipFeature->get_OID(&aObjID);
#ifdef TEST
	if( !cpFile && m_OK_ID.find(aObjID) != m_OK_ID.end()){
		return true;
	}
#endif
	CComVariant aDummy;
	ipFeature->get_Value(m_DummyIndex, &aDummy);
	if(aDummy.lVal == 1){
		return false;
	}
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	m_ipAnno->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	if(m_BufDist != 0.0){
		IGeometryPtr ipBuffer;
		((ITopologicalOperatorPtr)ipGeom)->Buffer(m_BufDist, &ipBuffer);
		ipFilter->putref_Geometry(ipBuffer);
	}else{
		ipFilter->putref_Geometry(ipGeom);
	}
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );

#ifdef _DEBUG
	if(aObjID == 6184){
		int a = 0;
	}
#endif
	IFeatureCursorPtr ipCursor;
	if(m_ipAnno->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
		fprintf(stderr, "Error,%s�߂��̒��L�������s�B,%d\n", m_RoadKind[cRoadKind], aObjID);
		fprintf(cpFile, "#Error,%s�߂��̒��L�������s�B,%d\n", m_RoadKind[cRoadKind], aObjID);
		return false;
	}
	crd_cnv cnv;
	double aLong, aLat;
	((IPointPtr)ipGeom)->QueryCoords(&aLong, &aLat);
	int aMesh, aX, aY;
	cnv.LLtoMesh(aLong, aLat, 64, &aMesh, &aX, &aY);

//	CComVariant aRoadKind;
//	ipFeature->get_Value(m_RoadAttrIndex, &aRoadKind);
	CComVariant aKanji;
	ipFeature->get_Value(m_KanjiAttrIndex, &aKanji);
	CString aRoadName(aKanji.bstrVal);
	CString aRoadCmpName = aRoadName;
	if(m_Mode == 0){
		aRoadCmpName.Replace("�h�b","");
	}
	aRoadCmpName.Replace("�r�`", "");
	aRoadCmpName.Replace("�o�`", "");
	aRoadCmpName.Replace("�����v", "");
//	aRoadCmpName.Replace("�i�b�s", "�i�b");
	int aCount = 0;
	IFeaturePtr ipTgtFeature;
	CString aAnnoNameList = "";
	bool aFind = false;
	long aLastKind = 0;
	ID_LIST aIDList;
	while(ipCursor->NextFeature(&ipTgtFeature) == S_OK){
		CComVariant aKind;
		ipTgtFeature->get_Value(m_AnnoAttrIndex, &aKind);
		int aTableIndex = fnGetTableIndex(aKind.lVal, cRoadKind);
		if(aTableIndex > -1){
//		if(fnIsTargetKind(aKind.lVal, cRoadKind)){
			aLastKind = aKind.lVal;
			CComVariant aName1;
			ipTgtFeature->get_Value(m_AnnoName1Index, &aName1);
			if(aName1.vt !=VT_NULL){
				CString aAnnoName(aName1.bstrVal);
				CString aAnnoCmpName = aAnnoName;
//				aAnnoCmpName.Replace("�h�b","");
				aAnnoCmpName.Replace("�r�`", "");
				aAnnoCmpName.Replace("�o�`", "");
				aAnnoCmpName.Replace("�����v", "");
				if(m_Mode == 1){
					aAnnoCmpName.Replace("�i�b", "�i�b�s");
				}
				if(aAnnoCmpName != aRoadCmpName){
					if( cpFile ){
						if(!fnPointComp2(ipTgtFeature, cRoadKind, NULL)){
							aCount++;
							if(aAnnoNameList != ""){
								aAnnoNameList += "�^";
							}
							aAnnoNameList += aAnnoName;
						}
					}
				}else{
					aFind = true;
					if( cpFile ){
//						if(aKind.lVal == 1204103 || aKind.lVal == 1204105 || aKind.lVal == 1204107 || aKind.lVal == 1204109){
						if(m_TargetKind[aTableIndex].m_IsFree){
							fprintf(cpFile, "0\t%s\t%d\t\t\tInfo\t0\t���̈�v�B�i�����j\t%d\t%s\t%s\t%s\t%s\n",
								static_cast<LPCSTR>(m_RoadLayerName), aObjID, aMesh, static_cast<LPCSTR>(aAnnoName), m_AnnoConv.getAttrName(aKind.lVal), static_cast<LPCSTR>(aRoadName), m_RoadKind[cRoadKind]);
						}else{
							fprintf(cpFile, "0\t%s\t%d\t\t\tInfo\t0\t���̈�v�B\t%d\t%s\t%s\t%s\t%s\n",
								static_cast<LPCSTR>(m_RoadLayerName), aObjID, aMesh, static_cast<LPCSTR>(aAnnoName), m_AnnoConv.getAttrName(aKind.lVal), static_cast<LPCSTR>(aRoadName), m_RoadKind[cRoadKind]);
						}
					}else{
						//�L���̃`�F�b�N�����ł���΂�������
						return true;
					}
					long aAnnoObjID;
					ipTgtFeature->get_OID(&aAnnoObjID);
					aIDList.push_back(aAnnoObjID);
				}
			}
		}
	}
	bool aResult = false;
	if(aFind){
		if(aIDList.size() > 1){
			for(ID_LIST::iterator it = aIDList.begin(); it != aIDList.end(); it++){
				m_ID_Map.insert(ID_MAP::value_type(*it, aIDList));
			}
		}
		aResult =  true;
	}
	if( cpFile ){
		if(!aFind && aCount == 0){
			if(m_NeedRank[cRoadKind] > 1){
				if(m_RoadCodeIndex > -1){
					//�~�h������
					CComVariant aRoadCode;
					ipFeature->get_Value(m_RoadCodeIndex, &aRoadCode);
					if(aRoadCode.lVal / 10000 == 20){
						fprintf(cpFile, "0\t%s\t%d\t\t\tError\t0\t%s�ɑΉ����钍�L��������Ȃ��B\t%d\t\t\t%s\t%s\n",
							static_cast<LPCSTR>(m_RoadLayerName), aObjID, m_RoadKind[cRoadKind], aMesh, static_cast<LPCSTR>(aRoadName), m_RoadKind[cRoadKind]);
					}else{
						fprintf(cpFile, "0\t%s\t%d\t\t\tWarning\t0\t%s�ɑΉ����钍�L��������Ȃ��B�i�K�{����Ȃ��j\t%d\t\t\t%s\t%s\n",
							static_cast<LPCSTR>(m_RoadLayerName), aObjID, m_RoadKind[cRoadKind], aMesh, static_cast<LPCSTR>(aRoadName), m_RoadKind[cRoadKind]);
					}
				}else{
					if(!fnCompSamePoint(ipFeature, aRoadName, cpFile)){
						fprintf(cpFile, "0\t%s\t%d\t\t\tError\t0\t%s�ɑΉ����钍�L��������Ȃ��B\t%d\t\t\t%s\t%s\n",
							static_cast<LPCSTR>(m_RoadLayerName), aObjID, m_RoadKind[cRoadKind], aMesh, static_cast<LPCSTR>(aRoadName), m_RoadKind[cRoadKind]);
					}
				}
			}else if(m_NeedRank[cRoadKind] == 1){
				fprintf(cpFile, "0\t%s\t%d\t\t\tWarning\t0\t%s�ɑΉ����钍�L��������Ȃ��B�i�K�{����Ȃ��j\t%d\t\t\t%s\t%s\n",
					static_cast<LPCSTR>(m_RoadLayerName), aObjID, m_RoadKind[cRoadKind], aMesh, static_cast<LPCSTR>(aRoadName), m_RoadKind[cRoadKind]);
			}
		}else if(aCount != 0){
			if(aFind){
				fprintf(cpFile, "0\t%s\t%d\t\t\tError\t0\t���̂��Ⴄ���̂��܂܂��B\t%d\t%s\t%s\t%s\t%s\n",
					static_cast<LPCSTR>(m_RoadLayerName), aObjID, aMesh, static_cast<LPCSTR>(aAnnoNameList), m_AnnoConv.getAttrName(aLastKind), static_cast<LPCSTR>(aRoadName), m_RoadKind[cRoadKind]);
			}else{
				fprintf(cpFile, "0\t%s\t%d\t\t\tError\t0\t���̂��Ⴄ�B\t%d\t%s\t%s\t%s\t%s\n",
					static_cast<LPCSTR>(m_RoadLayerName), aObjID, aMesh, static_cast<LPCSTR>(aAnnoNameList), m_AnnoConv.getAttrName(aLastKind), static_cast<LPCSTR>(aRoadName), m_RoadKind[cRoadKind]);
			}
		}
	}
	return aResult;
}

bool Anno::fnIsTargetKind(LONG cKind, const LONG cRoadKind)
{
	for(int i = 0; m_TargetKind[i].m_Road > 0; i++){
		if(cRoadKind == m_TargetKind[i].m_Road && cKind == m_TargetKind[i].m_Anno){
			return true;
		}
	}
	return false;
}

bool Anno::fnPointCheck2(const LONG cRoadKind, FILE *cpFile)
{
	printf("���L�f�[�^��%s���������Ă��܂��B\n", m_RoadKind[cRoadKind]);
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString aWhere;
	fnMakeSQL(cRoadKind, aWhere);
	ipFilter->put_WhereClause(CComBSTR(aWhere));
	long aTotal = 0;
	m_ipAnno->FeatureCount(ipFilter, &aTotal);
	IFeatureCursorPtr ipCursor;
	if(FAILED(m_ipAnno->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fprintf(stderr, "Error,���L��%s�������s\n", m_RoadKind[cRoadKind]);
		fprintf(cpFile, "#Error,���L��%s�������s\n", m_RoadKind[cRoadKind]);
		return false;
	}
	printf("�����I���B\n");
	bool aResult = true;
	IFeaturePtr ipFeature;
	LONG aCount = 0;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		aResult &= fnPointComp2(ipFeature, cRoadKind, cpFile);
		aCount++;
		printf("%d/%d\r", aCount, aTotal);
	}
	printf("\n");
	return aResult;
}

bool Anno::fnMakeSQL(const LONG cRoadKind, CString &cWhere)
{
	int i;
	for(i = 0; m_TargetKind[i].m_Road > 0; ++i){
		if(m_TargetKind[i].m_Road == cRoadKind){
			break;
		}
	}
	cWhere.Format("%s IN (%d",ANNO_ATTR_FIELD, m_TargetKind[i].m_Anno);
	for(i++; m_TargetKind[i].m_Road == cRoadKind; ++i){
		CString aKind;
		aKind.Format(",%d", m_TargetKind[i].m_Anno);
		cWhere += aKind;
	}
	cWhere += ")";
	return true;
}

bool Anno::fnPointComp2(IFeaturePtr ipFeature, const LONG cRoadKind, FILE *cpFile)
{
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	m_ipRoad->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	if(m_BufDist != 0.0){
		IGeometryPtr ipBuffer;
		((ITopologicalOperatorPtr)ipGeom)->Buffer(m_BufDist, &ipBuffer);
		ipFilter->putref_Geometry(ipBuffer);
	}else{
		ipFilter->putref_Geometry(ipGeom);
	}
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );

	long aObjID = 0;
	ipFeature->get_OID(&aObjID);
	IFeatureCursorPtr ipCursor;
	if(m_ipRoad->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
		fprintf(stderr, "Error,%s�߂��̎{�݌������s�B,%d\n", m_RoadKind[cRoadKind], aObjID);
		fprintf(cpFile, "#Error,%s�߂��̎{�݌������s�B,%d\n", m_RoadKind[cRoadKind], aObjID);
		return false;
	}
	crd_cnv cnv;
	double aLong, aLat;
	((IPointPtr)ipGeom)->QueryCoords(&aLong, &aLat);
	int aMesh, aX, aY;
	cnv.LLtoMesh(aLong, aLat, 64, &aMesh, &aX, &aY);

	CComVariant aAnnoKind;
	ipFeature->get_Value(m_AnnoAttrIndex, &aAnnoKind);
	CComVariant aName1;
	ipFeature->get_Value(m_AnnoName1Index, &aName1);
	CString aAnnoName(aName1.bstrVal);
	CString aAnnoCmpName = aAnnoName;
//	aAnnoCmpName.Replace("�h�b","");
	aAnnoCmpName.Replace("�r�`", "");
	aAnnoCmpName.Replace("�o�`", "");
	aAnnoCmpName.Replace("�����v", "");
	if(m_Mode == 1){
		aAnnoCmpName.Replace("�i�b", "�i�b�s");
	}	
	int aTableIndex = fnGetTableIndex(aAnnoKind.lVal, cRoadKind);
	fnDispScaleCheck(ipFeature, aTableIndex, aMesh, cpFile);
	int aCount = 0;
	IFeaturePtr ipTgtFeature;
	CString aRoadNameList = "";
	bool aFind = false;
	long aLastKind = 0;
	while(ipCursor->NextFeature(&ipTgtFeature) == S_OK){
		CComVariant aKind;
		ipTgtFeature->get_Value(m_RoadAttrIndex, &aKind);
//		if(aKind.lVal == 4 || aKind.lVal == 5){
		if(aKind.lVal == cRoadKind){
			CComVariant aDummy;
			ipTgtFeature->get_Value(m_DummyIndex, &aDummy);
			if(aDummy.lVal == 1){
				continue;
			}
			aLastKind = aKind.lVal;
			CComVariant aKanji;
			ipTgtFeature->get_Value(m_KanjiAttrIndex, &aKanji);
			if(aKanji.vt !=VT_NULL){
				CString aRoadName(aKanji.bstrVal);
				CString aRoadCmpName = aRoadName;
				if(m_Mode == 0){
					aRoadCmpName.Replace("�h�b","");
				}
				aRoadCmpName.Replace("�r�`", "");
				aRoadCmpName.Replace("�o�`", "");
				aRoadCmpName.Replace("�����v", "");
//				aRoadCmpName.Replace("�i�b�s", "�i�b");
				if(aAnnoCmpName != aRoadCmpName){
					if( cpFile ){
						if(!fnPointComp(ipTgtFeature, cRoadKind, NULL)){
							aCount++;
							if(aRoadNameList != ""){
								aRoadNameList += "�^";
							}
							aRoadNameList += aRoadName;
						}
					}
				}else{
					aFind = true;
					if( cpFile ){
//						if(aAnnoKind.lVal == 1204103 || aAnnoKind.lVal == 1204105 || aAnnoKind.lVal == 1204107 || aAnnoKind.lVal == 1204109){
						if(m_TargetKind[aTableIndex].m_IsFree){
//							aKind.lVal &= 0x07; //Warning�΍�e�X�g�p�i�o�O���ۂ��j
//							aKind.lVal &= 0x08; //Warning�΍�e�X�g�p Warning�����Ȃ�
//							if(aKind.lVal > 1833) return false; //Warning������
//							if(aKind.lVal > 1834) return false; //Warning�����Ȃ�
							fprintf(cpFile, "0\t%s\t%d\t\t\tInfo\t0\t���̈�v�B�i�����j\t%d\t%s\t%s\t%s\t%s\n",
								static_cast<LPCSTR>(m_AnnoLayerName), aObjID, aMesh, static_cast<LPCSTR>(aAnnoName), m_AnnoConv.getAttrName(aAnnoKind.lVal), static_cast<LPCSTR>(aRoadName), m_RoadKind[aKind.lVal]);
						}else{
							fprintf(cpFile, "0\t%s\t%d\t\t\tInfo\t0\t���̈�v�B\t%d\t%s\t%s\t%s\t%s\n",
								static_cast<LPCSTR>(m_AnnoLayerName), aObjID, aMesh, static_cast<LPCSTR>(aAnnoName), m_AnnoConv.getAttrName(aAnnoKind.lVal), static_cast<LPCSTR>(aRoadName), m_RoadKind[aKind.lVal]);
						}
					}
					return true;
				}
			}
		}
	}
	if( cpFile ){
		if(aCount == 0){
			if(m_TargetKind[aTableIndex].m_RoadReady){
				fprintf(cpFile, "0\t%s\t%d\t\t\tError\t0\t%s���L�ɑΉ�����{�݂�������Ȃ��B\t%d\t%s\t%s\t\t\n",
					static_cast<LPCSTR>(m_AnnoLayerName), aObjID, m_RoadKind[cRoadKind], aMesh, static_cast<LPCSTR>(aAnnoName), m_AnnoConv.getAttrName(aAnnoKind.lVal));
			}
		}else if(!aFind){
			fprintf(cpFile, "0\t%s\t%d\t\t\tError\t0\t���̂��Ⴄ�B\t%d\t%s\t%s\t%s\t%s\n",
				static_cast<LPCSTR>(m_AnnoLayerName), aObjID, aMesh, static_cast<LPCSTR>(aAnnoName), m_AnnoConv.getAttrName(aAnnoKind.lVal), static_cast<LPCSTR>(aRoadNameList), m_RoadKind[aLastKind]);
		}
	}
	return false;
}

bool Anno::setCityArea(LPCTSTR cMeshClass, LPCTSTR cSiteClass)
{
	IFeatureWorkspacePtr ipMapFeatureWorkspace(m_ipMapWorkspace);
	ATLASSERT( ipMapFeatureWorkspace );
	if(FAILED(ipMapFeatureWorkspace->OpenFeatureClass(CComBSTR(_T(cMeshClass)), &m_ipMesh))){
		fprintf(stderr, "#���b�V���|���S�����J���܂���B,%s\n", cMeshClass);
		fprintf(m_pFile, "#���b�V���|���S�����J���܂���B,%s\n", cMeshClass);
		return false;
	}
	m_ipMesh->FindField(CComBSTR(COMP_FIELD), &m_CompIndex);
	if(m_CompIndex < 0){
		fprintf(stderr, "#���S���t���O�̃t�B�[���h��������܂���B,%s\n", cMeshClass);
		fprintf(m_pFile, "#���S���t���O�̃t�B�[���h��������܂���B,%s\n", cMeshClass);
		return false;
	}

	if(FAILED(ipMapFeatureWorkspace->OpenFeatureClass(CComBSTR(_T(cSiteClass)), &m_ipSite))){
		fprintf(stderr, "#�s�s�n�}�w�i�t�B�[�`�����J���܂���B,%s\n", cSiteClass);
		fprintf(m_pFile, "#�s�s�n�}�w�i�t�B�[�`�����J���܂���B,%s\n", cSiteClass);
		return false;
	}
	m_ipSite->FindField(CComBSTR(SITE_ATTR_FIELD), &m_SiteIndex);
	if(m_SiteIndex < 0){
		fprintf(stderr, "#�w�i�����̃t�B�[���h��������܂���B,%s\n", cSiteClass);
		fprintf(m_pFile, "#�w�i�����̃t�B�[���h��������܂���B,%s\n", cSiteClass);
		return false;
	}
	return true;
}

bool Anno::fnIsTarget(IFeaturePtr ipFeature)
{
	if(m_ipMesh == NULL){
		//�S���Ώ�
		return true;
	}
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);

	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	m_ipMesh->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	ipFilter->putref_Geometry(ipGeom);
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );

	IFeatureCursorPtr ipCursor;
	if(m_ipMesh->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
		fprintf(stderr, "#Error,���H���܂܂�郁�b�V���|���S���������s�B\n");
		return false;
	}
	bool aFlag = false;
	IFeaturePtr ipTgtFeature;
	while(ipCursor->NextFeature(&ipTgtFeature) == S_OK){
		CComVariant aKind;
		ipTgtFeature->get_Value(m_CompIndex, &aKind);
		if(aKind.vt != VT_NULL){
			aFlag = true;
			break;
		}
	}

	if(aFlag){
		if(m_ipSite->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
			fprintf(stderr, "#Error,���H�ɂ������w�i�������s�B\n");
			return false;
		}
		while(ipCursor->NextFeature(&ipTgtFeature) == S_OK){
			CComVariant aKind;
			ipTgtFeature->get_Value(m_SiteIndex, &aKind);
			if(aKind.lVal == DEF_FUKANZEN){
				return false;
			}
		}
		return true;
	}		
	return false;
}

int Anno::fnGetTableIndex(long cAnnoAttr, long cRoadAttr)
{
	for(int i = 0; m_TargetKind[i].m_Road > 0; ++i){
		if(cRoadAttr == m_TargetKind[i].m_Road && cAnnoAttr == m_TargetKind[i].m_Anno){
			return i;
		}
	}
	return -1;
}

bool Anno::fnDispScaleCheck(IFeaturePtr ipFeature, int cTableIndex, long cMesh, FILE *cpFile)
{
	if( !cpFile ){
		return true;
	}
	for(int i = 0; i < 4 && m_AnnoDispTypeIndex[i] > -1; ++i){
		CComVariant aDispType;
		ipFeature->get_Value(m_AnnoDispTypeIndex[i], &aDispType);
		int aRule = m_TargetKind[cTableIndex].m_Rule[i+m_RuleOffset];
		bool aIsError = false;
		switch(aRule%10){
			case 0:	//�擾���Ă͂����Ȃ�
				if(aDispType.lVal != 0){
					aIsError = true;
				}
				break;
			case 1:	//�擾���Ă��悢
				if(aDispType.lVal != 0 && aDispType.lVal != aRule/10){
					aIsError = true;
				}
				break;
			case 2:	//�K���擾
			case 3:	//�K���擾�i�y�A���l���F�ۗ��j
				if(aDispType.lVal != aRule/10){
					aIsError = true;
					long aObjID;
					ipFeature->get_OID(&aObjID);
					ID_MAP::iterator aGroupe = m_ID_Map.find(aObjID);
					if(aGroupe != m_ID_Map.end()){
						ID_LIST::iterator aList = aGroupe->second.begin();
						for(; aList != aGroupe->second.end(); aList++){
							if(*aList != aObjID){
								IFeaturePtr ipGroupeFeature;
								m_ipAnno->GetFeature(*aList, &ipGroupeFeature);
								if(ipGroupeFeature != NULL){
									CComVariant aDispType2;
									ipGroupeFeature->get_Value(m_AnnoDispTypeIndex[i], &aDispType2);
									if(aDispType2.lVal == aRule/10){
										aIsError = false;
										break;
									}
								}
							}
						}
					}
				}
				break;
			default:
				break;
		}
		if(aIsError){
			CComVariant aName;
			ipFeature->get_Value(m_AnnoName1Index, &aName);
			CString aAnnoName = aName.bstrVal;
			long aObjID;
			ipFeature->get_OID(&aObjID);
			fprintf(cpFile, "0\t%s\t%d\t\t\tError\t0\t�X�P�[�����̕\���^�C�v���s��(�X�P�[��%d)�B\t%d\t%s\t%s\t\t\n",
				static_cast<LPCSTR>(m_AnnoLayerName), aObjID, i+1, cMesh, static_cast<LPCSTR>(aAnnoName), m_AnnoConv.getAttrName(m_TargetKind[cTableIndex].m_Anno));
			return false;
		}
	}
	return true;
}

bool Anno::fnSetNeedRank(void)
{
	for(int i = 0; i < 8; i++){
		m_NeedRank[i] = 0;
	}

	for(int i = 0; m_TargetKind[i].m_Road > 0; i++){
		for(int j = 0; j < 4 && m_AnnoDispTypeIndex[j] > -1; j++){
			int aRule = m_TargetKind[i].m_Rule[j+m_RuleOffset] % 10;
			if(aRule > m_NeedRank[m_TargetKind[i].m_Road]){
				m_NeedRank[m_TargetKind[i].m_Road] = aRule;
			}
		}
	}
	return true;
}

#define TEST
bool Anno::fnCompSamePoint(IFeaturePtr ipFeature, LPCTSTR cRoadName, FILE *cpFile)
{
	long aObjID = 0;
	ipFeature->get_OID(&aObjID);

#ifdef TEST
	if(m_OK_ID.find(aObjID) != m_OK_ID.end()){
		return true;
	}
#endif
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	m_ipRoad->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	IGeometryPtr ipBuffer;
	((ITopologicalOperatorPtr)ipGeom)->Buffer(KIGO_SIZE * 10.0, &ipBuffer);
	ipFilter->putref_Geometry(ipBuffer);
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );

	IFeatureCursorPtr ipCursor;
	if(m_ipRoad->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
		fprintf(stderr, "Error,����|�C���g�̎{�݌������s�B,%d\n", aObjID);
		fprintf(cpFile, "#Error,����|�C���g�̎{�݌������s�B,%d\n", aObjID);
		return false;
	}
#ifdef TEST
	ID_LIST aList;
#endif
	bool aResult = false;
	IFeaturePtr ipTgtFeature;
	while(ipCursor->NextFeature(&ipTgtFeature) == S_OK){
		long aTgtObjID = 0;
		ipTgtFeature->get_OID(&aTgtObjID);
#ifdef TEST
		aList.push_back(aTgtObjID);
#endif
		if(!aResult && aObjID != aTgtObjID){
			CComVariant aKanji;
			ipTgtFeature->get_Value(m_KanjiAttrIndex, &aKanji);
			if(CString(aKanji.bstrVal) == cRoadName){
				CComVariant aRoadKind;
				ipTgtFeature->get_Value(m_RoadAttrIndex, &aRoadKind);
				if(fnPointComp(ipTgtFeature, aRoadKind.lVal, NULL)){
					aResult = true;
#ifndef TEST
					break;
#endif
				}
			}
		}
	}
#ifdef TEST
	if(aResult){
		ID_LIST::iterator it;
		for(it = aList.begin(); it != aList.end(); it++){
			m_OK_ID.insert(*it);
		}
	}
#endif
	return aResult;
}
