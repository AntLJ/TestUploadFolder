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
#include "RND.h"
#include<map>
#include <atl2/io.h>

CRND::CRND(IFeatureClassPtr ipFeatureClass)
{
	m_ipFeatureClass = ipFeatureClass;
}

CRND::~CRND(void)
{
}


bool CRND::Point2Map(std::map<std::pair<long, long>, WKSPoint>& rSortSet, std::wstring rNodeFileName, bool bWalkOpt)
{
	HRESULT hRetObjct, hRetDiv;
	long lOBJECTIDIndex;
	//OBJECTID�̃t�B�[���h��̲��ޯ���擾
	hRetObjct = m_ipFeatureClass->FindField(_T("OBJECTID"),&lOBJECTIDIndex);
	if(FAILED(hRetObjct) || (-1 == lOBJECTIDIndex)){
		std::cerr << rNodeFileName << ":�t�B�[���h�C���f�b�N�X�̎擾�Ɏ��s���܂���"<<std::endl;
		return false;
	}

	long lDIVIDIndex;
	if(bWalkOpt){
		//DIVID�̃t�B�[���h��̲��ޯ���擾
		hRetDiv = m_ipFeatureClass->FindField(_T("DIVID"), &lDIVIDIndex);
		if(FAILED(hRetDiv) || (-1 == lDIVIDIndex)){
			std::cerr << rNodeFileName << ":�t�B�[���h�C���f�b�N�X�̎擾�Ɏ��s���܂���"<<std::endl;
			return false;
		}
	}

	IFeatureCursorPtr ipFeatureCursor;
	//̨�������َ擾
	if( FAILED( m_ipFeatureClass->Search(NULL, VARIANT_FALSE, & ipFeatureCursor) ) ){
		std::cerr << rNodeFileName << ":���ق̌����Ɏ��s���܂����B" << std::endl;
		return false;
		}
		
	IFeaturePtr ipFeature;						
	//̨�������ق��s����܂�̨���������
	while (ipFeatureCursor->NextFeature(&ipFeature)==S_OK && (ipFeature != NULL)) {

		long lOID = ipFeature->GetOID();

		CComVariant ObjectID;						//ObjectID
		if(FAILED(ipFeature->get_Value(lOBJECTIDIndex, &ObjectID))){
			std::cerr << rNodeFileName << "\tFID:" << lOID << "\tOBJECTID�̎擾�Ɏ��s" << std::endl;
			continue;
		}
				
		CComVariant DivID;							//DivID
		DivID.lVal = -1;							//�����l�ݒ�(RND�̏ꍇ��-1�Œ�Ƃ���)
		if(bWalkOpt){
			if(FAILED(ipFeature->get_Value(lDIVIDIndex, &DivID))){
				std::cerr << rNodeFileName << "\tFID:" << lOID << "\tDIVID�̎擾�Ɏ��s" << std::endl;
				continue;
			}
		}

		//�`����擾
		IGeometryPtr ipGeometry;
		if(FAILED(ipFeature->get_ShapeCopy(&ipGeometry)) ){
			std::cerr << rNodeFileName << "\tFID:" <<  lOID << "\t�`��̎擾�Ɏ��s" << std::endl;
			continue;
		}

		//�`�󁨓_				
		IPointPtr ipPoint(ipGeometry);
		
		//���W�擾
		WKSPoint Point;	
		if( FAILED(ipPoint->get_X(&(Point.X))) || FAILED(ipPoint->get_Y(&(Point.Y))) ){
			std::cerr << rNodeFileName << "\tFID:" <<  lOID << "\t���W�̎擾�Ɏ��s"<<std::endl;
			continue;
		}

		std::pair<long, long> pairOidDivid(ObjectID.lVal, DivID.lVal);
		rSortSet.insert(std::pair<std::pair<long, long>, WKSPoint>(pairOidDivid, Point));
	}
	return true;
}
