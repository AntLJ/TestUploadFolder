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
#include "GetPolygonInfo.h"


CGetPolygonInfo::CGetPolygonInfo(void) 
{
}

CGetPolygonInfo::~CGetPolygonInfo(void)
{
}

bool CGetPolygonInfo::GetPolygon( IGeometryPtr ipGeometry, std::multimap<Key,Info>& rInfoMap,Key * pKey, Info * pInfo)
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
		std::cerr << "�\���_��2����" << std::endl;
		return false;
	}

	//�\���_�̏����擾���A���ꂼ��rInfoMap�ɑ}��
	IPointPtr ipPoint;
	for (int i=0;i < lPointCount;i++)
	{
		//i�Ԗڂ̍��W���擾
		if(FAILED(ipPointCollection->get_Point(i,&ipPoint))
			||FAILED(ipPoint->get_X(&(pKey->e_X))) || FAILED(ipPoint->get_Y(&(pKey->e_Y) ) ) ){
				std::cerr << i <<"�_�ڂ̎擾�Ɏ��s���܂���" <<std::endl;
				return false;
		}
		//��ԓ_�ԍ��̎擾
		pInfo->e_PointNumber= i;
		//i�Ԗڂ̓_����rInfoMap�ɑ}��
		rInfoMap.insert(std::pair<Key,Info>(*pKey,*pInfo));
	}

	return true;
}



	
