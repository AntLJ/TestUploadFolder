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
#include "GetPolyLineInfo.h"

CGetPolyLineInfo::CGetPolyLineInfo(void)
{
}

CGetPolyLineInfo::~CGetPolyLineInfo(void)
{
}

bool CGetPolyLineInfo::GetPolyline(IGeometryPtr ipGeometry,std::multimap<Key, Info>& rInfoMap, Key * pKey, Info * pInfo)
{
	//�`���_��̏W���ɕϊ�
	IPointCollectionPtr ipPointCollection(ipGeometry);
	long lPointCount;
	if(FAILED(ipPointCollection->get_PointCount(&lPointCount) ) ){
		std::cerr << "�\���_�̎擾�Ɏ��s" << std::endl;
		return false;
	}
	//�\���_��2�����ł����
	if(lPointCount < 2 ){
		std::cerr << "�\���_����2�_����" << std::endl;
		return false;
	}

	//�n�_���擾
	IPointPtr ipPoint;
	if(FAILED(ipPointCollection->get_Point(0,&ipPoint))
		||FAILED(ipPoint->get_X(&(pKey->e_X))) || FAILED(ipPoint->get_Y(&(pKey->e_Y) ) ) ){
			std::cerr << "�n�_�̎擾�Ɏ��s���܂���" <<std::endl;
			return false;
	}
	//�n�_�̕�ԓ_�ԍ��擾
	pInfo->e_PointNumber = 0;
	//�n�_�̏���InfoMap�ɑ}��
	rInfoMap.insert(std::pair<Key,Info>(*pKey,*pInfo));


	//�I�_���擾
	if(FAILED(ipPointCollection->get_Point(lPointCount-1,&ipPoint))
		||FAILED(ipPoint->get_X(&(pKey->e_X))) || FAILED(ipPoint->get_Y(&(pKey->e_Y) ) ) ){
			std::cerr << "�I�_�̎擾�Ɏ��s���܂���" <<std::endl;
			return false;
	}
	//�I�_�̕�ԓ_�ԍ����擾
	pInfo->e_PointNumber = lPointCount-1;
	//�I�_�̏���rInfoMap�ɑ}��
	rInfoMap.insert(std::pair<Key,Info>(*pKey,*pInfo));

	return true;

}

	
	




