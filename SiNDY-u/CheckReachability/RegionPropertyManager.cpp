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
#include <map>
#include <string>
#include <boost/thread.hpp>
#include <arctl/geometry.h>
#include "RegionPropertyManager.h"

namespace {

} // anonymous namespace

class RegionPropertyManager::Impl
{
	typedef std::map<long, boost::shared_ptr<RegionProperty> > RegionPropertyMap;
public:
	Impl(IFeatureClass* ipInnerClass, IFeatureClass* ipOuterClass, LPCWSTR lpszRoadLinkWhereClause) :
	m_ipInnerClass(ipInnerClass),
	m_ipOuterClass(ipOuterClass),
	m_strRoadLinkWhereClause(lpszRoadLinkWhereClause)
	{
		_ASSERT(! ((m_ipInnerClass == 0) ^ (m_ipOuterClass == 0)));
	}

	boost::shared_ptr<RegionProperty> getRegionProperty(IPoint* ipPoint)
	{
		const boost::mutex::scoped_lock lk(m_Mutex);

		if(m_ipInnerClass != 0) {
			// ���W���������郊�[�W�����͈͂����߂�B
			// �����̃��[�W�����͈͂ɏd�Ȃ��Ă���ꍇ�A�쐼�̂��̂��D�悳���B
			const ISpatialFilterPtr ipSpFilter(__uuidof(SpatialFilter));
			ipSpFilter->SubFields = m_ipInnerClass->ShapeFieldName;
			ipSpFilter->_AddField(m_ipInnerClass->OIDFieldName);
			ipSpFilter->Geometry = ipPoint;
			ipSpFilter->SpatialRel = esriSpatialRelIntersects;

			typedef std::map<IPointPtr, IFeaturePtr, arctl::point_less> FeatureMap;

			const IFeatureCursorPtr ipInnerCursor(m_ipInnerClass->_Search(ipSpFilter, VARIANT_FALSE));

			FeatureMap aMap;
			for(IFeaturePtr ipFeature; ipInnerCursor->NextFeature(&ipFeature) == S_OK; ) {
				aMap.insert(FeatureMap::value_type(IAreaPtr(ipFeature->Shape->Envelope)->Centroid, ipFeature));
			}

			// ���[�W�����͈͂����܂�Ȃ�������NULL��Ԃ��B
			if(aMap.empty())
				return boost::shared_ptr<RegionProperty>();

			// ���ɓǂݍ��ݍς݂̃��[�W�����Ȃ�΁A�����Ԃ��B
			const IFeaturePtr ipInnerFeature(aMap.begin()->second);
			boost::shared_ptr<RegionProperty>& rpRegionProp = m_RegionPropMap[ipInnerFeature->OID];
			if(! rpRegionProp) {
				// ���[�W�����͈͂̃I�[�o�[���b�v�͈͂����߂�B
				const IGeometryPtr ipInnerGeom(ipInnerFeature->Shape);
				ipSpFilter->SubFields = m_ipOuterClass->ShapeFieldName;
				ipSpFilter->Geometry = ipInnerGeom;
				ipSpFilter->SpatialRel = esriSpatialRelWithin;

				const IGeometryPtr ipOuterGeom(m_ipOuterClass->_Search(ipSpFilter, VARIANT_TRUE)->_NextFeature()->ShapeCopy);

				const boost::shared_ptr<RegionProperty> pRegionProp(new RegionProperty);
				pRegionProp->m_ipInnerArea = ipInnerGeom;
				pRegionProp->m_ipOuterArea = ipOuterGeom;
				pRegionProp->m_strRoadLinkWhereClause = m_strRoadLinkWhereClause;
				rpRegionProp = pRegionProp;
			}
			return rpRegionProp;
		}
		else {
			boost::shared_ptr<RegionProperty>& rpRegionProp = m_RegionPropMap[0];
			if(! rpRegionProp) {
				const boost::shared_ptr<RegionProperty> pNewRegionProp(new RegionProperty);
				pNewRegionProp->m_strRoadLinkWhereClause = m_strRoadLinkWhereClause;
				rpRegionProp = pNewRegionProp;
			}
			return rpRegionProp;
		}
	}

private:
	const IFeatureClassPtr m_ipInnerClass;
	const IFeatureClassPtr m_ipOuterClass;
	const std::wstring m_strRoadLinkWhereClause;

	RegionPropertyMap m_RegionPropMap;
	boost::mutex m_Mutex;
};

RegionPropertyManager::RegionPropertyManager(
	IFeatureClass* ipInnerClass,
	IFeatureClass* ipOuterClass,
	LPCWSTR lpszRoadLinkWhereClause
) :
pimpl(new Impl(ipInnerClass, ipOuterClass, lpszRoadLinkWhereClause))
{
}

RegionPropertyManager::~RegionPropertyManager()
{
}

/**
 * @note
 * �����I�u�W�F�N�g����쐬���ꂽ RegionProperty �ł���΁A���[�W�����̓��ꐫ�̓|�C���^�Ō��؉\�ł���B
 */
boost::shared_ptr<RegionProperty> RegionPropertyManager::getRegionProperty(IPoint* ipPoint)
{
	return pimpl->getRegionProperty(ipPoint);
}
