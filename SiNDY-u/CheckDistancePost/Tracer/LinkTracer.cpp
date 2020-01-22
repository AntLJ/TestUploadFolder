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

// LinkTracer.cpp: LinkTracer �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <sindy/schema/road.h>
#include <WinLib/WaitCursor.h>
#include "LinkTracer.h"
#include <iostream>
#include <sindy/workspace.h>

using namespace sindy::schema;

namespace sindy
{


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

LinkTracer::LinkTracer() :
m_pDijkstra(nullptr),
m_eSection(kSelectStartLink),
m_dbBuf(8.0f)
{
	
}

LinkTracer::~LinkTracer()
{
	delete m_pDijkstra;
	m_pDijkstra = nullptr;
}

void LinkTracer::clearCache()
{
	toStartSelectLinkSection();
	m_cLinksCache.clearCache();
	m_ipCachedFeatureClass = nullptr;
	m_listRouteIDs.clear();
}

//////////////////////////////////////////////////////////////////////
// �ݒ�
//////////////////////////////////////////////////////////////////////

/**
 * @brief �o�H�T���G���W����ύX����
 *
 * @param pNewEngine [in] �V�����o�H�T���G���W��
 */
void LinkTracer::setEngine(Dijkstra* pNewEngine)
{
	delete m_pDijkstra;
	m_pDijkstra = pNewEngine;
}

/**
 * @brief �`�悷�郋�[�g��ݒ肷��
 *
 * @param rRoute [in] �`�悷�郋�[�g
 */
void LinkTracer::setRoute(const Route& rRoute)
{
	m_ipRoutePolyline = nullptr;

	if(! rRoute.isValid())
		return;

	IPointCollectionPtr ipPointCol;
	for(const Route* pRoute = &rRoute; pRoute; pRoute = pRoute->m_pPrevRoute) {
		const Link* pLink = pRoute->link();
		long nNodeId = pRoute->m_nNodeId;

#if _DEBUG
		CString strTmp;
		strTmp.Format(_T(" Route OID:%d\n"), pLink->m_nObjectId);
		OutputDebugString(strTmp);
#endif
		m_listRouteIDs.push_back( pLink->m_nObjectId );

		IClonePtr ipClone;
		IClonePtr(pLink->m_ipGeometry)->Clone(&ipClone);
		IPolylinePtr ipLinkPolyline(ipClone);

		// �Ō�Ɉꊇ���Ĕ��]����̂ŁA���ꂼ��̃����N�ł͔��]���ē���Ă���
		if(pLink->m_nFromNodeId == nNodeId) {
			ipLinkPolyline->ReverseOrientation();
		}

		if(! ipPointCol ) {
			ipPointCol = ipLinkPolyline;
		}
		else {
			IPointCollectionPtr ipPointCol2 = ipLinkPolyline;
			ipPointCol2->RemovePoints(0, 1);
			ipPointCol->AddPointCollection(ipPointCol2);
		}
	}

	if(ipPointCol ) {
		m_ipRoutePolyline = ipPointCol;
		m_ipRoutePolyline->ReverseOrientation();
	}

	m_listRouteIDs.reverse();
}

//////////////////////////////////////////////////////////////////////
// �ύX
//////////////////////////////////////////////////////////////////////

/**
 * @brief �����N�I��Where���ύX����
 *
 * @param lpWhereClause [in] �����N�I��Where��
 * @retval true ����
 * @retval false ���s
 */
bool LinkTracer::changeLinkWhereClause(LPCTSTR lpWhereClause)
{
	return true;
}

/**
 * @brief �Z�N�V������ύX����
 *
 * @param section [in] �Z�N�V����
 */
void LinkTracer::changeSection(ESection section)
{
	m_eSection = section;
}

//////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////

/**
 * @brief �c�[�����A�N�e�B�u�ɂȂ�������̏���
 */
void LinkTracer::onActivate()
{

}

/**
 * @brief ����������߁A�J�n�����N�I���Z�N�V�����ɖ߂�
 */
void LinkTracer::toStartSelectLinkSection()
{
	if(m_ipRoutePolyline )
		m_ipRoutePolyline = nullptr;
	m_ipBasePoint = nullptr;
	m_ipBaseGeometry = nullptr;
	if(m_pDijkstra)
		m_pDijkstra->clear();
	m_cLinkTraceSpace.clear();
	changeSection(kSelectStartLink);
}

/**
 * @brief �c�[������A�N�e�B�u�ɂȂ钼�O�̏���
 */
void LinkTracer::onDeactivate()
{
	clearCache();
}

/**
 * @brief ���B�\�����N���ʃ��[�h
 *
 * @param ipPoint [in] �N���b�N���ꂽ���W
 */
void LinkTracer::onArriveMode(const IPointPtr& ipPoint)
{
	long nId = 0;
	IGeometryPtr ipGeometry;

	const Link* pLink = m_cLinksCache.findLink(ipPoint, getMapSelectUnitTolerance());
	if(pLink) {
		nId = pLink->m_nObjectId;
		ipGeometry = pLink->m_ipGeometry;
	}

	if(nId && ipGeometry)
	{
		m_ipBaseGeometry = ipGeometry;
		m_ipBasePoint = ipPoint;

		if(startTrace(nId))
		{
			m_ipRoutePolyline = nullptr;
			changeSection(kStartTrace);
		}
		else
			m_ipBaseGeometry = nullptr;
	}
}

/**
 * @brief �ʏ�o�H�T�����[�h
 *
 * @param ipPoint [in] �N���b�N���ꂽ���W
 */
void LinkTracer::onNormalMode(const IPointPtr& ipPoint)
{
	switch(m_eSection) {
	case kSelectStartLink:
		onArriveMode(ipPoint);
		break;
	case kSelectEndLink:
	case kStartTrace:
	case kTracing:
		// �I�_�����N�I��
		int nLinkId = m_cLinksCache.findLinkId(ipPoint, getMapSelectUnitTolerance());
		if(nLinkId) {
			changeSection(kStartTrace);
			m_pDijkstra->setEndLinkId(nLinkId);
			traceToEnd();
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
/**
 * @brief �L���E���̃����N���L���b�V������
 *
 * @retval true ����
 * @retval false ���s
 */
bool LinkTracer::init_cache(const IFeatureClassPtr& ipClass, double dbBuf)
{
	try
	{
		ESection eSection = m_eSection;

		m_dbBuf = dbBuf;
		clearCache();
		m_ipCachedFeatureClass = ipClass;
		m_cLinksCache.setLinkClass(m_ipCachedFeatureClass);
		m_cLinksCache.init_cache();
	}
	catch(std::exception& e)
	{
		return false;
	}

	return true;
}

bool LinkTracer::add_cache(long nOID, const _IRowPtr& ipRow)
{
	return m_cLinksCache.add_cache(nOID, ipRow);
}

/**
 * @brief �L���E���̃����N���L���b�V������
 *
 * @retval true ����
 * @retval false ���s
 */
bool LinkTracer::cache()
{
	try
	{
		// �ǐՋ�Ԃ̐ݒ�
		if(m_cLinkTraceSpace.empty())
		{
			m_cLinkTraceSpace.setLinks(m_cLinksCache);
			changeSection(m_eSection);
		}
	}
	catch(std::exception& e)
	{
		return false;
	}

	return true;
}

/**
 * @brief �o�H�T���̎n�_�����N��ݒ肵�A�T���̊J�n��錾����
 *
 * @param ipStartLink [in] �n�_�����N
 * @retval true �o�H�T���̊J�n�ɐ���
 * @retval false �o�H�T���̊J�n�Ɏ��s
 */
bool LinkTracer::startTrace(const IFeaturePtr& ipStartLink)
{
	ATLASSERT(ipStartLink);

	long nOid;
	ipStartLink->get_OID(&nOid);

	return startTrace(nOid);
}

/**
 * @brief �o�H�T���̎n�_�����N��ݒ肵�A�T���̊J�n��錾����
 *
 * @param nStartLinkId [in] �n�_�����NID
 * @retval true �o�H�T���̊J�n�ɐ���
 * @retval false �o�H�T���̊J�n�Ɏ��s
 */
bool LinkTracer::startTrace(long nStartLinkId)
{
	setEngine(new Dijkstra);
	
	if(! m_pDijkstra->setLinks(m_cLinkTraceSpace, nStartLinkId))
		return false;

	return true;
}

/**
 * @brief �o�H�T�����P����i�߂�
 *
 * @retval true �o�H�T���͂܂�����
 * @retval false �o�H�T���͏I������
 */
bool LinkTracer::trace()
{
	if(! m_pDijkstra || ! (m_eSection == kStartTrace || m_eSection == kTracing))
		return false;

	CWaitCursor wait;

	bool bResult = m_pDijkstra->trace();
	if(bResult) {
		changeSection(kTracing);
	}
	else {
		changeSection(kSelectEndLink);
	}

	return true;
}

/**
 * @brief ���̌o�H�T���H��`�悵�A�T���𑱂���
 *
 * @retval true �o�H�T���͂܂�����
 * @retval false �o�H�T���͏I������
 */
bool LinkTracer::traceWithDraw()
{
	if(! m_pDijkstra || ! (m_eSection == kStartTrace || m_eSection == kTracing))
		return false;

	bool bResult = m_pDijkstra->trace();

	const Route* pRoute = m_pDijkstra->getRouteToEndLink();
	if(pRoute) {
		setRoute(*pRoute);
		return false;
	}

	const Route* pNextRoute = m_pDijkstra->getNextTraceRoute();
	if(pNextRoute) {
		setRoute(*pNextRoute);
	}

	return bResult;
}

/**
 * @brief �o�H�T�����I������܂ŒT���𑱂���
 *
 * @note �������ꂽ�o�H�͕`��p�ɓo�^�����
 * @retval true �T���͏I�����A�K�؂Ȍo�H���������ꂽ
 * @retval false �K�؂Ȍo�H�𔭌����邱�Ƃ��o���Ȃ�����
 */
bool LinkTracer::traceToEnd()
{
	if(! m_pDijkstra || ! (m_eSection == kStartTrace || m_eSection == kTracing))
		return false;

	CWaitCursor wait;

	changeSection(kTracing);

	while(m_pDijkstra->trace()) {
	}

	changeSection(kSelectEndLink);

	const Route* pRoute = m_pDijkstra->getRouteToEndLink();
	if(! pRoute)
		return false;

	setRoute(*pRoute);

	return true;
}

/**
 * @brief �o�H��H��邾���H��
 *
 * @note �H�����ꍇ�A�H�ꂽ�����N��`�悷��
 * @retval true �H����
 * @retval false �H��Ȃ�����
 */
bool LinkTracer::traceToLimit()
{
	if(! m_pDijkstra || ! (m_eSection == kSelectEndLink || m_eSection == kStartTrace))
		return false;

	CWaitCursor wait;

	changeSection(kTracing);

	m_pDijkstra->setEndLinkId(0);
	while(m_pDijkstra->trace()) {
	}

	changeSection(kSelectEndLink);

	return true;
}

//////////////////////////////////////////////////////////////////////
// �`��
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

/**
 * @brief �w�肵�����W�ɂ����Ƃ��߂������N���擾����
 *
 * @return �w�肵�����W�ɂ����Ƃ��߂������N�B�擾�ł��Ȃ������ꍇNULL�B
 */
IFeaturePtr LinkTracer::getLink(const IPointPtr& ipPoint)
{
	ATLASSERT(ipPoint);

	// �I���t�B�[�`���i�[�R���|�[�l�b�g�̏�����
	ITopologicalOperatorPtr ipTopoOp(ipPoint);

	// ���͂��ꂽ�|�C���g��I���G���A���a�Ńo�b�t�@���A�I���G���A���쐬
	IGeometryPtr ipRegion;
	ipTopoOp->Buffer(1, &ipRegion);

	// �����N���C���擾
	ILayerPtr ipLinkLayer = nullptr;
	if(! ipLinkLayer )
		return nullptr;

	// �����N���C������ߖT�����N���擾
	IFeatureCursorPtr ipFeatureCursor = nullptr;

	if(! ipFeatureCursor )
		return nullptr;

	ISpatialReferencePtr ipSpRef;
	ipPoint->get_SpatialReference(&ipSpRef);

	IProximityOperatorPtr ipProOp(ipPoint);

	IFeaturePtr ipResultLink;
	double dResultDistance = 0;

	IFeaturePtr ipLink;
	while(SUCCEEDED(ipFeatureCursor->NextFeature(&ipLink)) && ipLink ) {
		IGeometryPtr ipGeometry;
		ipLink->get_ShapeCopy(&ipGeometry);

		double dDistance;
		ipProOp->ReturnDistance(ipGeometry, &dDistance);
		if(ipResultLink == 0 || dDistance < dResultDistance) {
			dResultDistance = dDistance;
			ipResultLink = ipLink;
		}
	}

	return ipResultLink;
}

/**
 * @brief ���݂̊g�僂�[�h�őI��臒l���擾����
 *
 * @return �I��臒l
 */
double LinkTracer::getMapSelectUnitTolerance()
{
	IUnitConverterPtr ipUnit( CLSID_UnitConverter );
	double dbRet = 0.0f;

	atl2::valid(ipUnit->ConvertUnits( m_dbBuf, esriMeters, esriDecimalDegrees, &dbRet ));
	return dbRet;
}

}
