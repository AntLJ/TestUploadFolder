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

// LinksCache.cpp: LinksCache �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <sindy/schema/road.h>
#include "LinksCache.h"

using namespace sindy::schema;
namespace sindy
{

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

LinksCache::LinksCache()
{

}

LinksCache::~LinksCache()
{

}

//////////////////////////////////////////////////////////////////////
// �N���A
//////////////////////////////////////////////////////////////////////

void LinksCache::clearCache()
{
	m_ipEnvelope = nullptr;
	m_ipCachedLinkClass = nullptr;
	m_cLinkMap.clear();
}

//////////////////////////////////////////////////////////////////////
// �ݒ�
//////////////////////////////////////////////////////////////////////

/**
 * @brief �w�肵�����H�����N�t�B�[�`���N���X���L���b�V���p�ɓo�^����
 *
 * @param ipClass [in] ���H�����N�t�B�[�`���N���X
 */
void LinksCache::setLinkClass(const IFeatureClassPtr& ipClass)
{
	if(m_ipCachedLinkClass != ipClass) {
		clearCache();
		m_ipLinkClass = ipClass;
	}
}

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief �w�肵����`���ɑ��݂��郊���N�A�y�у����N�Ɋ֘A����E���܋֎~���L���b�V������
 *
 * @note �w�肵����`���A�O��L���b�V��������`�ɓ�����ꍇ�A�����̃L���b�V���𗘗p����B
 * @note �ăL���b�V�������v�������ꍇ�AclearCache()�������Ƃ�cache()����Ηǂ��B
 *
 * @param ipEnvelope [in] �L���b�V���Ώۋ�`
 * @param lpLinkWhereClause [in] �����N�I��Where��
 * @retval true �L���b�V���ɐ��������B���邢�́A���ɃL���b�V������Ă���B
 * @retval false �L���b�V���Ɏ��s�����B
 */
bool LinksCache::init_cache()
{
	m_ipCachedLinkClass = m_ipLinkClass;

	return true;
}

bool LinksCache::add_cache(long nOID, const _IRowPtr& ipRow)
{
	m_cLinkMap[nOID] = Link(ipRow);

	return true;
}

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief �L���b�V���̒�����A�w�肵�����W�̋߂��ɑ��݂��郊���N���擾����
 *
 * @param ipPoint [in] �Ώۍ��W
 * @param dTolerance [in] 臒l
 * @return �����N�B�Ώۍ��W����臒l���Ƀ����N�����݂��Ȃ������ꍇ0�B
 */
const Link* LinksCache::findLink(const IPointPtr& ipPoint, double dTolerance) const
{
	if(! ipPoint)
		return nullptr;

	double dDistance = 0;
	const Link* pResult = nullptr;

	LinkMap::const_iterator itr = m_cLinkMap.begin();
	if(itr == m_cLinkMap.end())
		return pResult;
	
	IClonePtr ipClone;
	IClonePtr(ipPoint)->Clone(&ipClone);
	IPointPtr ipClonePoint(ipClone);

	ISpatialReferencePtr ipSpRef;
	itr->second.m_ipGeometry->get_SpatialReference(&ipSpRef);
	ipClonePoint->putref_SpatialReference(ipSpRef);

	IProximityOperatorPtr ipProOp(ipClonePoint);

	for(; itr != m_cLinkMap.end(); ++itr) {
		const Link& rLink = itr->second;

		double dDist;
		if(FAILED(ipProOp->ReturnDistance(rLink.m_ipGeometry, &dDist)))
			continue;

		if(dDist <= dTolerance && (! pResult || dDist < dDistance)) {
			pResult = &rLink;
			dDistance = dDist;
		}
	}

	return pResult;
}

/**
 * @brief �L���b�V���̒�����A�w�肵�����W�̋߂��ɑ��݂��郊���N��ID���擾����
 *
 * @param ipPoint [in] �Ώۍ��W
 * @param dTolerance [in] 臒l
 * @return �����NID�B�Ώۍ��W����臒l���Ƀ����N�����݂��Ȃ������ꍇ0�B
 */
int LinksCache::findLinkId(const IPointPtr& ipPoint, double dTolerance) const
{
	const Link* pLink = findLink(ipPoint, dTolerance);

	if(! pLink)
		return 0;

	return pLink->m_nObjectId;
}

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief �w�肵���`�󂪁A�L���b�V�������G���A���Ɏ��܂��Ă��邩
 *
 * @param ipGeometry [in] ���،`��
 * @retval true �G���A���ɑ��݂���
 * @retval false �G���A���ɑ��݂��Ȃ��B���邢�̓L���b�V���G���A�����݂��Ȃ��B
 */
bool LinksCache::cacheAreaIsContain(const IGeometryPtr& ipGeometry)
{
	if(! ipGeometry || ! m_ipEnvelope )
		return false;

	IRelationalOperatorPtr ipRelOp(m_ipEnvelope);
	VARIANT_BOOL vbContain;
	ipRelOp->Contains(ipGeometry, &vbContain);
	return vbContain == VARIANT_TRUE;
}

}
