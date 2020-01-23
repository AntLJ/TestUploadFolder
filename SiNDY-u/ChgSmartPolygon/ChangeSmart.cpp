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
#include "ChangeSmart.h"
#include "globals.h"

CChangeSmart::CChangeSmart(void) : m_dAllowOffset( 0.001 )
{
}

CChangeSmart::~CChangeSmart(void)
{
}

bool CChangeSmart::ExecSmart(IFeatureClassPtr ipFeatureClass)
{
	if( !ipFeatureClass )
	{
		std::cerr << "[ERROR] no FeatureClass" << std::endl;
		return false;
	}

	// �W�I���g���^�C�v�`�F�b�N
	esriGeometryType esriType = esriGeometryLine;
	ipFeatureClass->get_ShapeType( &esriType );
	if( esriGeometryPolygon != esriType )
	{
		std::cerr << "[ERROR] Only Polygon. GeometryType = " << esriType << std::endl;
		return false;
	}

	// �`��̌���
	IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL );
	if( !ipFilter )
	{
		std::cerr << "[ERROR] no QueryFilter" << std::endl;
		return false;
	}
	IFeatureCursorPtr ipCursor;
	ipFeatureClass->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	if( !ipCursor )
	{
		std::cerr << "[ERROR] fail feature search" << std::endl;
		return false;
	}

	IFeaturePtr ipFeature;
	std::map<long, bool> mapFeatureChk;
	while( ipCursor->NextFeature( &ipFeature ) == S_OK && ipFeature )
	{
		long lOID = 0;
		ipFeature->get_OID( &lOID );
		mapFeatureChk[ lOID ] = false;
	}

	if( !ToSmart( ipFeatureClass, mapFeatureChk ) )
		return false;

	return true;
}

bool CChangeSmart::ToSmart(IFeatureClassPtr ipFeatureClass, std::map<long, bool>& mapFeatureChk)
{
	long lTotalCnt = mapFeatureChk.size();
	long lCnt = 1;
	long lFinishCnt = 0;
	long lDelCnt = 0;

	OutPutTime( "Polygon �Ԉ����X�^�[�g" );

	std::list<std::pair<long, CSmartPath>> listRecursiveCheck;

	bool no_error = true;

	for( std::map<long, bool>::iterator itOID = mapFeatureChk.begin(); itOID != mapFeatureChk.end(); ++itOID, ++lCnt )
	{
		long lBaseOID = itOID->first;
		printf( "%07d / %07d\r", lCnt, lTotalCnt );

		IFeaturePtr ipFeature;
		ipFeatureClass->GetFeature( itOID->first, &ipFeature );
		if( !ipFeature )
		{
			std::cerr << "[ERROR] fail GetFeature ( " << itOID->first << " )" << std::endl;
			no_error = false;
			continue;
		}
		IGeometryPtr ipGeom;
		ipFeature->get_ShapeCopy( &ipGeom );

		std::map<long, CSmartShape>::iterator itSmart = m_mapSmartShape.find( itOID->first );
		if( itSmart == m_mapSmartShape.end() )
		{
			CSmartShape cSmart;
			cSmart.SetOID( itOID->first );
			cSmart.SetAllowOffset( m_dAllowOffset );
			if( !cSmart.SetGeometry( ipGeom ) )
				return false;
			m_mapSmartShape[ itOID->first ] = cSmart;
		}

		ISpatialFilterPtr ipSpFilter = AheGetSpatialFilter( ipGeom, ipFeatureClass, esriSpatialRelIntersects, 0, 0 );

		IFeatureCursorPtr ipCursor;
		ipFeatureClass->Search( ipSpFilter, VARIANT_FALSE, &ipCursor );
		if( !ipCursor )
		{
			std::cerr << "[ERROR] fail search polygon ( " << itOID->first << " )" << std::endl;
			no_error = false;
			continue;
		}

		// �����ΏۂƐڐG����|���S����OID
		std::list<long> listIntersectOID;

		IFeaturePtr ipTouchFeature;
		while( S_OK == ipCursor->NextFeature( &ipTouchFeature ) && ipTouchFeature )
		{
			long lOID = 0;
			ipTouchFeature->get_OID( &lOID );
			if( lOID == itOID->first )
				continue;
			// ���Ƀ`�F�b�N�ς݂ł���΁A�X�L�b�v
			if( mapFeatureChk[ lOID ] )
				continue;

			listIntersectOID.push_back( lOID );

			IGeometryPtr ipTargetGeom;
			ipTouchFeature->get_ShapeCopy( &ipTargetGeom );

			std::map<long, CSmartShape>::iterator itSmart = m_mapSmartShape.find( lOID );
			if( itSmart == m_mapSmartShape.end() )
			{
				CSmartShape cSmart;
				cSmart.SetOID( lOID );
				cSmart.SetAllowOffset( m_dAllowOffset );
				if( !cSmart.SetGeometry( ipTargetGeom ) )
					return false;
				m_mapSmartShape[ lOID ] = cSmart;
			}

			// ��|���S���ƐڐG�|���S���̃C���^�[�Z�N�g(���L����)���擾����
			CreateShareSegment( itOID->first, lOID );

			// �`�F�b�N�ς�
			mapFeatureChk[ itOID->first ] = 1;
		}

		// STORE�܂ōs��
		std::map<long, CSmartShape>::iterator it = m_mapSmartShape.find( itOID->first );
		if( it != m_mapSmartShape.end() )
		{
			if( !it->second.ChangeAllSegment( ipFeatureClass ))
				return false;

			IPolygonPtr ipPolygon = it->second.GetPolygon();
			if( ipPolygon )
			{
				// �o�����|���S�����ʐ�0�ł���΁A�t�B�[�`���폜
				IAreaPtr ipArea( ipPolygon );
				if( ipArea )
				{
					double dArea = 0;
					ipArea->get_Area( &dArea );
					if( dArea <= 0 )
					{
						ipFeature->Delete();
						++lDelCnt;
					}
					else
					{
						ipFeature->putref_Shape( ipPolygon );
						ipFeature->Store();
						++lFinishCnt;
					}
				}
				m_mapSmartShape.erase( it );
				// �`�F�b�N�ς�
				mapFeatureChk[ itOID->first ] = 1;
			}
		}
	}
	CString cStr;
	cStr.Format( _T("Polygon �����I�� : %d( DEL=%d ) / %d"), lFinishCnt, lDelCnt, lTotalCnt );
	std::string str = CT2CA( cStr );
	OutPutTime( str );

	for( std::map<long, CSmartShape>::iterator it = m_mapSmartShape.begin(); it != m_mapSmartShape.end(); ++it ) {
		if(! it->second.ChangeAllSegment( ipFeatureClass ))
			return false;
	}

	StorePolygon( ipFeatureClass );

	OutPutTime( "Polygon �Ԉ����I��" );

	return no_error;
}

// ���L�������擾����
void CChangeSmart::CreateShareSegment(long lBaseOID, long lTargetOID)
{
	std::map<long, CSmartShape>::iterator itBase = m_mapSmartShape.find( lBaseOID );
	std::map<long, CSmartShape>::iterator itTarget = m_mapSmartShape.find( lTargetOID );

	if( itBase->second.ExecShareLine( itTarget->second ) )
	{
		m_mapSmartShape[ lBaseOID ] = itBase->second;
		m_mapSmartShape[ lTargetOID ] = itTarget->second;
	}
}

// �|���S�����쐬���A�ۑ�����
void CChangeSmart::StorePolygon(IFeatureClassPtr ipFeatureClass)
{
	OutPutTime( "Polygon �ۑ��X�^�[�g" );

	long lCnt = 0;
	for( std::map<long, CSmartShape>::iterator it = m_mapSmartShape.begin(); it != m_mapSmartShape.end(); ++it, ++lCnt )
	{
		long lOID = it->first;
#if _DEBUG
		std::cerr << lOID << std::endl;
#endif
		IFeaturePtr ipFeature;
		ipFeatureClass->GetFeature( it->first, &ipFeature );
		if( !ipFeature )
			continue;

		IPolygonPtr ipPolygon = it->second.GetPolygon();
		if( !ipPolygon )
			continue;

		ipFeature->putref_Shape( ipPolygon );
		ipFeature->Store();
	}

	CString cStr;
	cStr.Format( _T("Polygon �ۑ��I�� : %d"), lCnt );
	std::string str = CT2CA( cStr );
	OutPutTime( str );
}
