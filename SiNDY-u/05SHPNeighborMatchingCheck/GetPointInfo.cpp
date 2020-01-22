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
#include "GetPointInfo.h"


CGetPointInfo::CGetPointInfo(void) 
{
}

CGetPointInfo::~CGetPointInfo(void)
{
}

bool CGetPointInfo::GetPoint( IGeometryPtr ipGeometry,std::multimap<Key, Info>& rInfoMap,Key * pKey,Info * pInfo)
{
	//�`���_��
	IPointPtr ipPoint(ipGeometry);

	//���W�擾
	if(FAILED(ipPoint->get_X(&(pKey->e_X))) || FAILED(ipPoint->get_Y(&(pKey->e_Y)))){
		
		return false;
	}

	//�|�C���g�̏ꍇ��Ԕԍ������݂��Ȃ��̂ŁA0�����Ƃ�
	pInfo->e_PointNumber = 0;

	//�|�C���g����InfoMap�ɑ}��
	rInfoMap.insert(std::pair<Key,Info>(*pKey,*pInfo));

	return true;

}
