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

#include "stdafx.h"
#include <sstream>
#include <rns/algorithm.h>
#include <atl2/io.h>
#include "CheckUpperReachability.h"
#include "CheckNodeReachability.h"

/**
 * @param ipRoadNodeClass [in] ���H�m�[�h�t�B�[�`���N���X�B
 * @param pRgnPyramidCreator [in] ���[�W�����K�w�쐬�I�u�W�F�N�g�B
 */
CheckNodeReachability::CheckNodeReachability(IFeatureClass* ipRoadNodeClass, RegionPyramidCreatorRef pRgnPyramidCreator) :
m_ipRoadNodeClass(ipRoadNodeClass),
m_pRgnPyramidCreator(pRgnPyramidCreator)
{
}

/**
 * @brief �w�肵���m�[�h���ŏ�ʂ̓��H�����N�Ɂ^����H��t���邩����������B
 *
 * @param nNodeID [in] �����Ώۃm�[�hID�B
 * @return �ŏ�ʂ̓��H�����N�Ɂ^���炽�ǂ蒅���邩�B
 */
std::pair<bool, bool> CheckNodeReachability::check(long nNodeID)
{
	// ���H�m�[�hID���L�[�Ƃ��ē��H�m�[�h�������B
	const IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));

	ipQueryFilter->SubFields = m_ipRoadNodeClass->ShapeFieldName;

	std::basic_ostringstream<TCHAR> oss;
	oss << m_ipRoadNodeClass->OIDFieldName << _T('=') << nNodeID;
	atl2::valid(ipQueryFilter->put_WhereClause(CComBSTR(oss.str().c_str())));

	const IFeatureCursorPtr ipFeatureCursor(m_ipRoadNodeClass->_Search(ipQueryFilter, VARIANT_TRUE));

	// �P��������Ȃ�������A�Q�ȏ㌩�������炨�������̂ŁA�Ƃ肠�����u�񓞒B�v�Ƃ���B
	IFeaturePtr ipRoadNode, ipRoadNodeDummy;
	if(ipFeatureCursor->NextFeature(&ipRoadNode) != S_OK || ipFeatureCursor->NextFeature(&ipRoadNodeDummy) == S_OK)
		return std::make_pair(false, false);

	// ���H�m�[�h�������郊�[�W�����s���~�b�h�����߂�B
	const std::auto_ptr<RegionPyramid> pRegPyramid(m_pRgnPyramidCreator->create(IPointPtr(ipRoadNode->Shape)));
	if(! pRegPyramid.get())
		return std::make_pair(false, false);
	
	// ���B������������B
	return 
		std::make_pair(
			CheckUpperReachability(pRegPyramid->begin(), pRegPyramid->end(), false).checkByNodeID(nNodeID),
			CheckUpperReachability(pRegPyramid->begin(), pRegPyramid->end(), true).checkByNodeID(nNodeID)
		);
}
