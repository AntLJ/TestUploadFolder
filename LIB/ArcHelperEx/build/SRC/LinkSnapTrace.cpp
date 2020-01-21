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
#include <exception>
#include <map>
#include <set>
#include <math.h>
#include <WaitCursor.h>
#include "LinkSnapTrace.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI
#include "GlobalFunctions.h"

//////////////////////////////////////////////////////////////////////
// anonymous namespace
//////////////////////////////////////////////////////////////////////

namespace {
/**
 * @brief IPointCollection�̍ŏ��܂��͍Ō�̃|�C���g���擾����
 *
 * @param col           [in] IPointCollectionPtr
 * @param first_or_last [in] �ŏ��ƍŌ�ǂ���̃|�C���g���擾���邩
 * @return �ŏ��܂��͍Ō�̃|�C���g
 */
IPointPtr GetGeomPoint(IPointCollectionPtr col, bool first_or_last)
{
	IPointPtr ret; // �߂�l

	if( ! col )
		return ret;

	if( first_or_last )
		col->get_Point( 0, &ret );
	else {
		long count = 0;
		col->get_PointCount( &count );
		col->get_Point( count-1, &ret );
	}

	return ret;
}
/**
 * @brief IPointCollection�̍ŏ��̃|�C���g���擾����
 *
 * @param col [in] IPointCollectionPtr
 * @return �ŏ��̃|�C���g
 */
IPointPtr GetFirstPoint(IPointCollectionPtr col)
{
	return GetGeomPoint(col, true);
}
/**
 * @brief IPointColletion�̍Ō�̃|�C���g���擾����
 *
 * @param col [in] IPointCollectionPtr
 * @return �Ō�̃|�C���g
 */
IPointPtr GetLastPoint(IPointCollectionPtr col)
{
	return GetGeomPoint(col, false);
}
IPointPtr GetPoint( IFeature* ipFeature, long lIndex )
{
	IGeometryPtr ipGeom;

	ipFeature->get_ShapeCopy( &ipGeom );

	IPointCollectionPtr ipCol = ipGeom;
	IPointPtr ipPoint;

	if( ! ipCol )
		ipPoint = ipGeom;
	else
		ipCol->get_Point( lIndex, &ipPoint );

	return ipPoint;
}

/**
 * @brief ���������Ɉ�v����I�u�W�F�N�g�����݂��邩
 * @bug [Bug 10374]�g���[�X�\�ȃ��C���f�[�^���A���s�ړ���i[MANAGEMENT_ID]is not Null)�̃I�u�W�F�N�g�Ɍ��肷��
 * @param ipClass [in] �t�B�[�`���N���X
 * @param nOID [in] OBJECTID
 * @param lpcszWhere [in] ��������
 * @return ��v����I�u�W�F�N�g���݂��邩�ǂ��� true:���݁Afalse:���݂��Ȃ�
 */
bool findObject(IFeatureClass* ipClass, long nOID, LPCTSTR lpcszWhere)
{
	if ( ! ipClass )
		return false;

	// OBJECTID���擾
	CComBSTR bstrOIDName;
	ipClass->get_OIDFieldName( &bstrOIDName );

	CString strTmp;
	strTmp.Format( _T("%s = %d AND %s"), (CString)bstrOIDName, nOID, lpcszWhere );
	long nCnt = 0;
	if( SUCCEEDED( ipClass->FeatureCount( AheInitQueryFilter(NULL, NULL, strTmp), &nCnt ) ) )
	{
		if ( nCnt > 0 )
			return true;
	}

	return false;
}

/**
 * @brief �w����W�ɍł��߂��t�B�[�`�������߂�B
 *
 * @param ipFeatureCache [in] �t�B�[�`���L���b�V���B
 * @param ipPoint [in] ���W�B
 * @param dTolerance [in] 臒l�B
 * @param ipClass [in] �t�B�[�`���N���X [Bug 10374]
 * @param lpcszWhere [in] �������� [Bug 10374]
 * @return �ŋߖT�t�B�[�`���B���W�Ƃ̋�����臒l�ȉ��̃t�B�[�`�������݂��Ȃ��ꍇnull�B
 */
IFeaturePtr search_nearest(
	IFeatureCache* ipFeatureCache,
	IPoint* ipPoint,
	double dTolerance,
	IFeatureClass* ipClass,
	LPCTSTR lpcszWhere=NULL)
{
	if(! (ipFeatureCache != NULL && ipPoint != NULL ))
		return 0;

	IFeaturePtr ipResultFeature;
	IProximityOperatorPtr ipProximityOperator(ipPoint);
	LONG lCount = 0;
	ipFeatureCache->get_Count( &lCount );

	for(long i = 0; i < lCount; i++) {
		//IFeaturePtr ipFeature(ipFeatureCache->GetFeature(i));
		IFeaturePtr ipFeature;
		ipFeatureCache->get_Feature( i, &ipFeature );

		if ( lpcszWhere && _tcslen(lpcszWhere) > 0 && ipClass )
		{
			// [Bug 10374]�g���[�X�\�ȃ��C���f�[�^���A���s�ړ���i[MANAGEMENT_ID]is not Null)�̃I�u�W�F�N�g�Ɍ��肷��
			// �����������ݒ肵�Ă��鎞�́AOID�Ə����Ō������Č��ʂ����鎞�̂ݏ���������悤�ɂ���
			// �t�B�[�`���L���b�V���͑����ōi�荞�߂Ȃ�����
			long nOID = 0;
			ipFeature->get_OID(&nOID);

			if ( ! findObject(ipClass, nOID, lpcszWhere) )
				continue;
		}

		IGeometryPtr ipGeom;
		ipFeature->get_ShapeCopy( &ipGeom );
		double dDistance = 0.0;
		ipProximityOperator->ReturnDistance(ipGeom, &dDistance);

		if(dDistance < dTolerance) {
			ipResultFeature = ipFeature;
			dTolerance = dDistance;
		}
	}

	return ipResultFeature;
}

/**
 * @brief ���i�����N�����߂�B
 *
 * @param ipFeatureCache [in] �t�B�[�`���L���b�V���B
 * @param ipLastPointCol [in] �ŏI�_��B
 * @param ipClass [in] �t�B�[�`���N���X [Bug 10374]
 * @param lpcszWhere [in] �������� [Bug 10374]
 * @return ���i�����N�B���݂��Ȃ��ꍇnull�B
 */
IFeaturePtr search_straight(
	IFeatureCache* ipFeatureCache,
	IPointCollection* ipLastPointCol,
	IFeatureClass* ipClass,
	LPCTSTR lpcszWhere=NULL)
{
	// ��Ԍ����B
	if(! (ipFeatureCache && ipLastPointCol))
		return 0;

	// �ŏI�_��͂Q�_�ȏ�K�v�B
	long nLastPointCount;
	if(ipLastPointCol->get_PointCount(&nLastPointCount) != S_OK || nLastPointCount < 2)
		return 0;

	// �ŏI�_�����߂�B
	IPointPtr ipLastPoint;
	ipLastPointCol->get_Point( nLastPointCount - 1, &ipLastPoint );

	// �ŏI�����̊p�x�����߂�B
	ILinePtr ipLastLine(CLSID_Line);
	IPointPtr ipTmpPoint;
	ipLastPointCol->get_Point( nLastPointCount - 2, &ipTmpPoint );
	ipLastLine->PutCoords(ipTmpPoint, ipLastPoint);
	double dLastAngle = 0;
	ipLastLine->get_Angle( &dLastAngle );

	// �����B
	IFeaturePtr ipResultFeature;
	double dResultAngle = M_PI;

	// �Œ��������N�����߂�B
	LONG lCount = 0;
	ipFeatureCache->get_Count( &lCount );
	for(long i = 0; i < lCount; i++) {
		IFeaturePtr ipFeature;
		ipFeatureCache->get_Feature(i, &ipFeature);
		if( ipFeature == NULL )
			continue;

		if ( lpcszWhere && _tcslen(lpcszWhere) > 0 && ipClass )
		{
			// [Bug 10374]�g���[�X�\�ȃ��C���f�[�^���A���s�ړ���i[MANAGEMENT_ID]is not Null)�̃I�u�W�F�N�g�Ɍ��肷��
			// �����������ݒ肵�Ă��鎞�́AOID�Ə����Ō������Č��ʂ����鎞�̂ݏ���������悤�ɂ���
			// �t�B�[�`���L���b�V���͑����ōi�荞�߂Ȃ�����
			long nOID = 0;
			ipFeature->get_OID(&nOID);

			if ( ! findObject(ipClass, nOID, lpcszWhere) )
				continue;
		}

		IGeometryPtr ipGeom;
		ipFeature->get_ShapeCopy( &ipGeom );
		IPointCollectionPtr ipPointCol( ipGeom );

		// �_��͂Q�_�ȏ�K�v�B
		long nPointCount;
		if(ipPointCol->get_PointCount(&nPointCount) != S_OK || nPointCount < 2)
			continue;

		// �ڑ����̊p�x�����߂�B
		ILinePtr ipLine(CLSID_Line);
		int iResult1 = 0, iResult2 = 0;
		ipLastPoint->Compare(GetPoint(ipFeature,0), &iResult1);
		ipLastPoint->Compare(GetPoint(ipFeature,nPointCount-1), &iResult2);
		//ipLastPoint->Compare(GetPoint(ipPointCol,0), &iResult1);
		//ipLastPoint->Compare(GetPoint(ipPointCol,nPointCount-1), &iResult2);
		if(iResult1 == 0) {
			ipLine->PutCoords(GetPoint(ipFeature,0), GetPoint(ipFeature,1));
			//ipLine->PutCoords(GetPoint(ipPointCol,0), GetPoint(ipPointCol,1));
		}
		else if(iResult2 == 0) {
			ipLine->PutCoords(GetPoint(ipFeature, nPointCount - 1), GetPoint(ipFeature,nPointCount - 2));
			//ipLine->PutCoords(GetPoint(ipPointCol, nPointCount - 1), GetPoint(ipPointCol,nPointCount - 2));
		}
		else
			continue;

		double dAngle = 0;
		ipLine->get_Angle(&dAngle);
		dAngle = dAngle - dLastAngle;
		while(dAngle < -M_PI) { dAngle += M_PI * 2.0; }
		while(dAngle > M_PI) { dAngle -= M_PI * 2.0; }
		dAngle = ::fabs(dAngle);

		if(dAngle < dResultAngle) {
			dResultAngle = dAngle;
			ipResultFeature = ipFeature;
		}
	}

	return ipResultFeature;
}

/**
 * @brief �w����W����̋������Ƀt�B�[�`������ׂ�B
 *
 * @param ipFeatureCache [in] �t�B�[�`���L���b�V���B
 * @param ipPoint [in] ���W�B
 * @param dTolerance [in] 臒l�B
 * @param ipConnectionPoint [in] �ڑ����W�B
 * @param rFeatureMap [out] �t�B�[�`�����א�B
 * @param ipClass [in] �t�B�[�`���N���X [Bug 10374]
 * @param lpcszWhere [in] �������� [Bug 10374]
 */
void order_by_distance(
	IFeatureCache* ipFeatureCache,
	IPoint* ipPoint,
	const double dTolerance,
	IPoint* ipConnectionPoint,
	std::multimap<double, IFeaturePtr>& rFeatureMap,
	IFeatureClass* ipClass,
	LPCTSTR lpcszWhere=NULL)
{
	if(! (ipFeatureCache != NULL && ipPoint != NULL && ipConnectionPoint != NULL))
		return;

	IProximityOperatorPtr ipProximityOperator(ipPoint);
	IRelationalOperatorPtr ipRelOp(ipConnectionPoint);
	LONG lCount = 0;
	ipFeatureCache->get_Count( &lCount );

	for(long i = 0; i < lCount; i++) {
		IFeaturePtr ipFeature;
		ipFeatureCache->get_Feature(i,&ipFeature);

		if ( lpcszWhere && _tcslen(lpcszWhere) > 0 && ipClass )
		{
			// [Bug 10374]�g���[�X�\�ȃ��C���f�[�^���A���s�ړ���i[MANAGEMENT_ID]is not Null)�̃I�u�W�F�N�g�Ɍ��肷��
			// �����������ݒ肵�Ă��鎞�́AOID�Ə����Ō������Č��ʂ����鎞�̂ݏ���������悤�ɂ���
			// �t�B�[�`���L���b�V���͑����ōi�荞�߂Ȃ�����
			long nOID = 0;
			ipFeature->get_OID(&nOID);

			if ( ! findObject(ipClass, nOID, lpcszWhere) )
				continue;
		}

		IGeometryPtr ipGeometry;
		ipFeature->get_ShapeCopy(&ipGeometry);

		VARIANT_BOOL vaIsTouches = VARIANT_FALSE;
		ipRelOp->Touches(ipGeometry, &vaIsTouches);
		if(vaIsTouches != VARIANT_TRUE)
			continue;

		double dDistance = 0;
		ipProximityOperator->ReturnDistance(ipGeometry,&dDistance);

		if(dDistance < dTolerance) {
			rFeatureMap.insert(std::multimap<double, IFeaturePtr>::value_type(dDistance, ipFeature));
		}
	}
}

/**
 * @brief �����N�L���b�V���N���X�B
 */
class LinkCache
{
public:
// �R���X�g���N�^�ƃf�X�g���N�^
	LinkCache(IFeatureClass* ipLinkClass) :
	m_ipLinkClass(ipLinkClass)
	{
		if(m_ipLinkClass == 0)
			throw std::logic_error("�����N�t�B�[�`���N���X��null.");
	}

	~LinkCache()
	{
	}

// ����
	/**
	 * @brief �L���b�V������B
	 *
	 * @param ipPoint [in] �L���b�V�����S���W�B
	 * @param dTolerance [in] �X�i�b�v臒l�B
	 * @param dCacheRatio [in] �X�i�b�v臒l�ɑ΂���L���b�V���͈͔�B
	 * @return �L���b�V���B���s�����ꍇ��null�B
	 */
	IFeatureCachePtr cache(IPoint* ipPoint, double dTolerance, double dCacheRatio)
	{
		if( ipPoint == NULL )
			return 0;

		// �L���b�V���̊m�F�B
		VARIANT_BOOL vaIsContains = VARIANT_FALSE;
		if(m_ipFeatureCache == 0) {
			m_ipFeatureCache.CreateInstance(CLSID_FeatureCache);
		}
		else{
			m_ipFeatureCache->Contains(ipPoint, &vaIsContains);
			if(vaIsContains == VARIANT_TRUE)
				return m_ipFeatureCache;
		}

		CWaitCursor aWaitCursor;

		// �L���b�V���B
		m_ipFeatureCache->Initialize(ipPoint, dTolerance * dCacheRatio);

		double dCacheOverlap = dTolerance * (dCacheRatio + 1);

		IEnvelopePtr ipEnv;
		ipPoint->get_Envelope( &ipEnv );
		IEnvelopePtr ipEnvelope(ipEnv);
		ipEnvelope->Expand(dCacheOverlap, dCacheOverlap, VARIANT_FALSE);
		m_ipFeatureCache->AddFeatures(m_ipLinkClass, ipEnvelope);

		return m_ipFeatureCache;
	}

	/**
	 * @brief �t�B�[�`���N���X�擾
	 * @bug   [Bug 10374]�g���[�X�\�ȃ��C���f�[�^���A���s�ړ���i[MANAGEMENT_ID]is not Null)�̃I�u�W�F�N�g�Ɍ��肷��
	 * @return �t�B�[�`���N���X
	*/
	IFeatureClassPtr getClass() { return m_ipLinkClass; }

private:
// �ϐ�
	IFeatureClassPtr m_ipLinkClass; ///< �����N�t�B�[�`���N���X�B
	IFeatureCache2Ptr m_ipFeatureCache; ///< �t�B�[�`���L���b�V���B
};

} // anonymous namespace 

//////////////////////////////////////////////////////////////////////
// LinkSnapTrace::impl
//////////////////////////////////////////////////////////////////////

class LinkSnapTrace::impl
{
public:
	typedef std::set<long> MarkingIndexSet;

// �R���X�g���N�^�ƃf�X�g���N�^
	impl(IFeatureClass* ipLinkClass) :
	m_dTolerance(0.0),
	m_dCacheRatio(2.0),
	m_bAhead(false),
	m_LinkCache(ipLinkClass)
	{
	}

	~impl()
	{
		stop();
	}

// �擾
	/**
	 * @brief �Ō�Ƀ}�[�N���ꂽ�ʒu��Ԃ��B
	 *
	 * @return �Ō�Ƀ}�[�N���ꂽ�ʒu�B�}�[�N�����݂��Ȃ��ꍇnull�B
	 */
	long getLastMarkingIndex() const
	{
		return m_MarkingIndexSet.empty() ? -1 : *m_MarkingIndexSet.rbegin();
	}

// ����
	/**
	 * @brief �����N�ǐՂ��J�n����B
	 *
	 * @param ipPoint [in] �J�[�\�����W�B
	 * @retval true �����N�ǐՂ̊J�n�ɐ����B
	 * @retval false �n�_�����N���߂��Ȃ������B
	 */
	bool start(IPoint* ipPoint)
	{
		// �������B
		stop();

		// ��Ԋm�F�B
		if(m_dTolerance <= 0)
			return false;

		// �����B
		IFeaturePtr ipFeature(search_nearest(cache(ipPoint), ipPoint, m_dTolerance, m_LinkCache.getClass(), m_strWhere));
		if( ipFeature == NULL )
			return false;

		// �O�Ր����B
		m_ipTrace.CreateInstance(CLSID_Polyline);
		IGeometryPtr ipTmpGeom, ipTmpGeom2;
		ipFeature->get_ShapeCopy( &ipTmpGeom );
		((IGeometryCollectionPtr)ipTmpGeom)->get_Geometry( 0, &ipTmpGeom2 );
		m_ipTrace->AddGeometry(ipTmpGeom2);

		// �����N�z�񐶐��B
		m_ipLinkArray.CreateInstance(CLSID_Array);
		m_ipLinkArray->Add(ipFeature);

		// �����̒����B
		adjustOrientation(ipPoint);

		return true;
	}

	/**
	 * @brief �����N��ǐՂ���B
	 *
	 * @param ipPoint [in] �J�[�\�����W�B
	 * @retval true �O�Ղɕω����������B
	 * @retval false �O�Ղ͈ȑO�̂܂܁B
	 */
	bool trace(IPoint* ipPoint)
	{
		// �O�Ղ����݂��Ȃ���ΒǐՂ��s�\�B
		if( m_ipTrace == NULL || m_ipLinkArray == NULL )
			return false;

		long nCount = getCount();
		if(nCount == 0)
			return false;

		// �����̒����B
		bool bReversed = adjustOrientation(ipPoint);

		// �����N���L���b�V���B
		IFeatureCachePtr ipLinkCache(cache(ipPoint));

		// ����\�_�����߂�B
		IGeometryPtr ipTmpGeom;
		m_ipTrace->get_Geometry( nCount - 1, &ipTmpGeom );
		IPointCollectionPtr ipLastPath(ipTmpGeom);
		IPointPtr ipLastPoint(GetLastPoint(ipLastPath));
		IPointPtr ipSecondPoint((nCount > 1) ? IPointPtr(GetFirstPoint(ipLastPath)) : 0);

		// �Œ��������N�����߂�B
		// ����\�_���L���b�V���O�ɑ��݂���ꍇ�A���i���[�h�͐�ɐi�߂Ȃ��B
		VARIANT_BOOL vaIsContains = VARIANT_FALSE;
		ipLinkCache->Contains( ipLastPoint, &vaIsContains );
		IFeaturePtr ipStraightLink =
			(m_bAhead && vaIsContains == VARIANT_TRUE) ?
				search_straight(ipLinkCache, ipLastPath, m_LinkCache.getClass(), m_strWhere) : 0;

		// �J�[�\�����W����̋������ɕ��בւ��B
		std::multimap<double, IFeaturePtr> aFeatureMap;
		order_by_distance(ipLinkCache, ipPoint, m_dTolerance, ipLastPoint, aFeatureMap, m_LinkCache.getClass(), m_strWhere);
		order_by_distance(ipLinkCache, ipPoint, m_dTolerance, ipSecondPoint, aFeatureMap, m_LinkCache.getClass(), m_strWhere);

		IFeaturePtr ipNextFeature;
		for(std::multimap<double, IFeaturePtr>::const_iterator it = aFeatureMap.begin(); it != aFeatureMap.end(); ++it) {
			long i = 0;
			for(i = 0; i < nCount; i++) {
				IUnknownPtr ipUnk;
				m_ipLinkArray->get_Element(i, &ipUnk);
				LONG lOID1 = 0, lOID2 = 0;
				((_IRowPtr)ipUnk)->get_OID(&lOID1);
				((_IRowPtr)it->second)->get_OID(&lOID2);
				if(lOID2 == lOID1)
					break;
			}

			if(i < nCount) {
				if(i == nCount - 2) {
					if(! back())
						continue;
					return true;
				}
				else if(i == nCount - 1)
					break;
				else
					continue;
			}

			if(m_bAhead) {
				// ���i���[�h�ł́A���i�����N�ȊO�ɃX�i�b�v���邱�Ƃ͂ł��Ȃ��B
				LONG lOID1 = 0, lOID2 = 0;
				if( ipStraightLink != NULL )
				{
					((_IRowPtr)it->second)->get_OID(&lOID1);
					((_IRowPtr)ipStraightLink)->get_OID(&lOID2);
					if( lOID2 != lOID1 )
						continue;
				}
				else
					continue;
			}
			else {
				// ���߂��ĕʂ̃����N�ɃX�i�b�v�B
				IGeometryPtr ipTmpGeom;
				it->second->get_ShapeCopy(&ipTmpGeom);
				VARIANT_BOOL vaIsTouches = VARIANT_FALSE;
				((IRelationalOperatorPtr)ipTmpGeom)->Touches(ipSecondPoint, &vaIsTouches);
				if(ipSecondPoint != NULL && vaIsTouches == VARIANT_TRUE) {
					if(! back())
						continue;
				}
			}

			ipNextFeature = it->second;
			break;
		}

		if(ipNextFeature != NULL) {
			step(ipNextFeature);
			return true;
		}

		return bReversed;
	}

	/**
	 * @brief �}�[�N����B
	 */
	void mark()
	{
		if(m_ipTrace == NULL)
			return;

		long nCount;
		if(m_ipTrace->get_GeometryCount(&nCount) != S_OK || nCount < 1)
			return;

		m_MarkingIndexSet.insert(nCount - 1);
	}

	/**
	 * @brief �Ō�̃}�[�N���O���B
	 */
	void demark()
	{
		if(m_MarkingIndexSet.empty())
			return;

		m_MarkingIndexSet.erase(--m_MarkingIndexSet.end());
	}

	/**
	 * @brief �Ō�̃}�[�N�ɖ߂�B
	 *
	 * �Ō�̃}�[�N���Ō�̗v�f�ɕt�����Ă���ꍇ�A�Ōォ���ڂ̃}�[�N�ɖ߂�B
	 */
	void undo()
	{
		long nLastMarkingIndex = getLastMarkingIndex();
		if(nLastMarkingIndex < 0)
			return;

		_ASSERT(m_ipTrace != NULL && m_ipLinkArray != NULL);

		long nCount = getCount();
		if(nLastMarkingIndex + 1 > nCount)
			return;

		if(nLastMarkingIndex + 1 == nCount) {
			// �Ō�̃}�[�N���O���B
			demark();

			// �Ōォ���ڂ̃}�[�N�ɖ߂�B
			undo();
		}

		// �Ō�̗v�f���폜����B
		m_ipTrace->RemoveGeometries(nLastMarkingIndex + 1, nCount - nLastMarkingIndex - 1);
		for(long i = nCount - 1; i > nLastMarkingIndex; i--) {
			m_ipLinkArray->Remove(i);
		}
	}

	/**
	 * @brief �����N�ǐՂ��~�߂�B
	 */
	void stop()
	{
		m_MarkingIndexSet.clear();
		m_ipTrace = 0;
		m_ipLinkArray = 0;
	}

	/**
	 * @brief ���߂��B
	 *
	 * @retval true �����B
	 * @retval false ���s�B���邢�̓}�[�N����Ă��邽�ߖ߂��Ȃ��B
	 */
	bool back_public()
	{
		// �O�Ղ����݂��Ȃ�������߂��Ȃ��B
		if(m_ipTrace == NULL || m_ipLinkArray == NULL)
			return false;

		// �v�f���擾�B
		long nCount = getCount();

		// �}�[�N����Ă�����߂��Ȃ��B
		if(getLastMarkingIndex() == nCount - 1)
			return false;

		// �Ō�̗v�f���폜����B
		m_ipTrace->RemoveGeometries(nCount - 1, 1);
		m_ipLinkArray->Remove(nCount - 1);

		return true;
	}

// �ϕϐ�
	double m_dTolerance; ///< �I��臒l�B
	double m_dCacheRatio; ///< �L���b�V����B
	bool m_bAhead; ///< ���i���[�h���B
	CString m_strWhere; ///!< �������� [Bug 10374]

// �Q�Ɨp�ϐ�
	IGeometryCollectionPtr m_ipTrace; ///< �O�ՁB
	IArrayPtr m_ipLinkArray; ///< �����N�z��B

private:
// �擾
	/**
	 * @brief �v�f�����擾����B
	 *
	 * �v�f���Ƃ́uPolyline���\������Path�̐��v�̂��ƁB<br>
	 * ����́uPolylilne���\������Feature�̐��v�ɓ������B
	 *
	 * @return �v�f���B
	 */
	long getCount()
	{
		if(m_ipTrace == NULL || m_ipLinkArray == NULL)
			return 0;

		_ASSERT(m_ipTrace != NULL && m_ipLinkArray != NULL);

		long nCount = 0;
		m_ipTrace->get_GeometryCount(&nCount);
		LONG mm = 0;
		m_ipLinkArray->get_Count(&mm);
		//_ASSERT(nCount == 0;m_ipLinkArray->get_Count(&nCount));
		return nCount;
	}

// ����
	IFeatureCachePtr cache(IPoint* ipPoint)
	{
		return m_LinkCache.cache(ipPoint, m_dTolerance, m_dCacheRatio);
	}

	/**
	 * @brief �O�Ղ̌����𒲐�����B
	 *
	 * @param ipPoint [in] �J�[�\�����W�B
	 * @retval true �������t�]�������B
	 * @retval false �O�ՂɕύX�͉����Ă��Ȃ��B
	 */
	bool adjustOrientation(IPoint* ipPoint)
	{
		if( ipPoint == NULL )
			return false;

		// �\���p�X����1�̏ꍇ�Ɍ���B
		LONG lCount = 0;
		m_ipTrace->get_GeometryCount(&lCount);
		if(m_ipTrace == NULL || lCount != 1)
			return false;

		// �J�[�\�����|�����C���̎n�_���ɂ���Ȃ�΁A�|�����C���𔽓]������B
		IPolylinePtr ipPolyline(m_ipTrace);

		double dAlong;
		ipPolyline->QueryPointAndDistance(esriNoExtension, ipPoint, VARIANT_TRUE, 0, &dAlong, 0, 0);

		if(dAlong < 0.5) {
			ipPolyline->ReverseOrientation();
			return true;
		}
		return false;
	}

	/**
	 * @brief ���i�߂�B
	 *
	 * @param ipFeature [in] �i�߂�t�B�[�`���B
	 */
	void step(IFeature* ipFeature)
	{
		if(m_ipTrace == NULL || m_ipLinkArray == NULL || ipFeature == NULL)
			return;

		IGeometryPtr ipTmpGeom;
		ipFeature->get_ShapeCopy(&ipTmpGeom);
		IPolylinePtr ipPolyine(ipTmpGeom);
		LONG lCount = 0;
		m_ipTrace->get_GeometryCount(&lCount);
		if(lCount > 0) {
			VARIANT_BOOL vaIsCompare = VARIANT_FALSE;
			IPointPtr ipPoint;
			((IPointCollectionPtr)ipPolyine)->get_Point(0,&ipPoint);
			int iResult = 0;
			(GetLastPoint(IPointCollectionPtr(m_ipTrace)))->Compare(ipPoint,&iResult);
			if(iResult != 0) {
				ipPolyine->ReverseOrientation();
			}
		}

		IGeometryPtr ipTmpGeom2;
		IGeometryCollectionPtr(ipPolyine)->get_Geometry(0, &ipTmpGeom2);
		m_ipTrace->AddGeometry(ipTmpGeom2);
		m_ipLinkArray->Add(ipFeature);
	}

	/**
	 * @brief ���߂��B
	 *
	 * @retval true �����B
	 * @retval false ���s�B���邢�̓}�[�N����Ă��邽�ߖ߂��Ȃ��B
	 */
	bool back()
	{
		// �O�Ղ����݂��Ȃ�������߂��Ȃ��B
		if(m_ipTrace == NULL || m_ipLinkArray == NULL)
			return false;

		// �v�f���擾�B
		long nCount = getCount();

		// �}�[�N����Ă�����߂��Ȃ��B
		if(getLastMarkingIndex() == nCount - 1)
			return false;

		// �Ō�̗v�f���폜����B
		m_ipTrace->RemoveGeometries(nCount - 1, 1);
		m_ipLinkArray->Remove(nCount - 1);

		return true;
	}

// �ϐ�
	LinkCache m_LinkCache; ///< �����N�L���b�V���B
	MarkingIndexSet m_MarkingIndexSet; ///< �}�[�N�ꗗ�B
};

//////////////////////////////////////////////////////////////////////
// LinkSnapTrace
//////////////////////////////////////////////////////////////////////

// �R���X�g���N�^�ƃf�X�g���N�^
//////////////////////////////////////////////////////////////////////

LinkSnapTrace::LinkSnapTrace(IFeatureClass* ipLinkClass) :
pimpl(new impl(ipLinkClass))
{
}

LinkSnapTrace::~LinkSnapTrace()
{
}

// �ݒ�
//////////////////////////////////////////////////////////////////////

/**
 * @brief 臒l��ݒ肷��B
 *
 * @param dTolerance [in] 臒l�B
 * @param dCacheRatio [in] �L���b�V����B
 */
void LinkSnapTrace::setTolerance(double dTolerance, double dCacheRatio)
{
	pimpl->m_dTolerance = dTolerance;
	pimpl->m_dCacheRatio = dCacheRatio;
}

/**
 * @brief ���i���[�h���ۂ���ݒ肷��B
 *
 * @param bAhead [in] ���i���[�h���B
 */
void LinkSnapTrace::setAhead(bool bAhead)
{
	pimpl->m_bAhead = bAhead;
}

// [Bug 10374]�g���[�X�\�ȃ��C���f�[�^���A���s�ړ���i[MANAGEMENT_ID]is not Null)�̃I�u�W�F�N�g�Ɍ��肷��
// ���������ݒ�
void LinkSnapTrace::setWhere(LPCTSTR lpcszWhere)
{
	pimpl->m_strWhere = lpcszWhere;
}


// �擾
//////////////////////////////////////////////////////////////////////

/**
 * @brief �O�Ղ��擾����B
 *
 * @return �O�ՁB���݂��Ȃ��ꍇnull�B
 */
IPolylinePtr LinkSnapTrace::getTrace() const
{
	return pimpl->m_ipTrace;
}

/**
 * @brief �����N�̕��т��擾����B
 *
 * @return �����N�̕��сB���݂��Ȃ��ꍇnull�B
 */
IArrayPtr LinkSnapTrace::getLinkArray() const
{
	return pimpl->m_ipLinkArray;
}

/** @sa LinkSnapTrace::impl::getLastMarkingIndex() */
long LinkSnapTrace::getLastMarkingIndex() const
{
	return pimpl->getLastMarkingIndex();
}

// ����
//////////////////////////////////////////////////////////////////////

/** @sa LinkSnapTrace::impl::start() */
bool LinkSnapTrace::start(IPoint* ipPoint)
{
	return pimpl->start(ipPoint);
}

/** @sa LinkSnapTrace::impl::trace() */
bool LinkSnapTrace::trace(IPoint* ipPoint)
{
	return pimpl->trace(ipPoint);
}

/** @sa LinkSnapTrace::impl::mark() */
void LinkSnapTrace::mark()
{
	pimpl->mark();
}

/** @sa LinkSnapTrace::impl::demark() */
void LinkSnapTrace::demark()
{
	pimpl->demark();
}

/** @sa LinkSnapTrace::impl::undo() */
void LinkSnapTrace::undo()
{
	pimpl->undo();
}
/** @sa LinkSnapTrace::impl::back() */
bool LinkSnapTrace::back_public()
{
	return pimpl->back_public();
}
