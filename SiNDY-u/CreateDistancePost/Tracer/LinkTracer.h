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

// LinkTracer.h: LinkTracer �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "LinkTraceSpace.h"
#include "LinksCache.h"
#include "Dijkstra.h"

namespace sindy
{

class LinkTracer  
{
public:
	enum ESection {
		kSelectStartLink,
		kSelectEndLink,
		kStartTrace,
		kTracing,
	};

// �\�z/����
	LinkTracer();
	virtual ~LinkTracer();

	void clearCache();

// �ύX
	bool changeLinkWhereClause(LPCTSTR lpWhereClause);

// �n���h��
	void onActivate();
	void toStartSelectLinkSection();
	void onDeactivate();
	void onMouseDown(LONG Button, const IPointPtr& ipPoint);
	void onRefresh();

// �擾
	IFeaturePtr getLink(const IPointPtr& ipPoint);
	IPolylinePtr getRouteGeom() { return m_ipRoutePolyline; }
	std::list<long> getRouteIDs() { return m_listRouteIDs; };

// ����
	bool trace();
	bool traceWithDraw();
	bool traceToEnd();
	bool traceToLimit();

// ����
	bool init_cache(const IFeatureClassPtr& ipClass, double dbBuf);
	bool add_cache(long nOID, const _IRowPtr& ipRow);
	bool cache();

// �n���h��
	void onArriveMode(const IPointPtr& ipPoint);
	void onNormalMode(const IPointPtr& ipPoint);

private:
// �ݒ�
	void setEngine(Dijkstra* pNewEngine);
	void setRoute(const Route& rRoute);

// �擾
	double getMapSelectUnitTolerance();

// �ύX
	void changeSection(ESection section);

// ����
	
	bool startTrace(const IFeaturePtr& ipStartLink);
	bool startTrace(long nStartLinkId);

// �v���p�e�B
	CString m_strLinkWhereClause;
	ESection m_eSection;

	LinkTraceSpace m_cLinkTraceSpace; ///< �����N�ǐՋ��
	LinksCache m_cLinksCache; ///< �����N�L���b�V��
	IFeatureClassPtr m_ipCachedFeatureClass; ///< �L���b�V�����Ă���t�B�[�`���N���X

	Dijkstra* m_pDijkstra; ///< �����N�ǐՃG���W��

	IGeometryPtr m_ipBaseGeometry; ///< ��`��
	IPointPtr m_ipBasePoint; ///< ��`��
//	IFeaturePtr m_ipStartLink; ///< �J�n�����N
	IPolylinePtr m_ipRoutePolyline; ///< ���[�g�`��
	IPolygonPtr m_ipPolygon; ///< ������`

	std::list<long> m_listRouteIDs;
	double m_dbBuf;
};

}
