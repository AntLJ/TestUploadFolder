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
#include "GlobalFunc.h"
#include <boost/filesystem.hpp>
#include <sindy/workspace.h>

namespace gf
{
	// ���[�N�X�y�[�X�擾
	IWorkspacePtr GetWorkspace( LPCTSTR lpcszDBProp )
	{
		IUnknownPtr ipUnknown = NULL;
		for( int i = 0; i < 5; ++i )
		{
			INamePtr ipName( sindy::create_workspace_name(lpcszDBProp) );
			if( NULL == ipName )
				return NULL;
			
			ipName->Open( &ipUnknown );
			if( ipUnknown )
				break;
		}
		return ipUnknown;
	}

	// ��ԁE��������
	IFeatureCursorPtr SelectByShape( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, long* pCount /* = NULL */,
									 esriSpatialRelEnum spatialRel /* = esriSpatialRelIntersects */,
									 LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
	{
		// �N�G���t�B���^�쐬
		ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
		CComBSTR bstrFieldName;	
		ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
		ipFilter->put_GeometryField( bstrFieldName );
		ipFilter->putref_Geometry( ipGeo );
		ipFilter->put_SpatialRel( spatialRel );
		
		// Where��w�肳��Ă�����A�Z�b�g����
		if( lpszWhereClause )
		{
			ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where��Z�b�g
			ipFilter->put_SearchOrder( esriSearchOrderSpatial );	// �����D�揇�ʂ��w�肷��
		}

		// SubFields�w�肳��Ă�����A�Z�b�g����
		if( lpszSubFields )
			ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

		// �������ʐ��擾
		if( pCount )
		{
			if( FAILED(ipFeatureClass->FeatureCount(ipFilter, pCount)) )
				return NULL;
		}

		// ����
		IFeatureCursorPtr ipFeatureCursor;
		ipFeatureClass->Search( ipFilter, VARIANT_FALSE, &ipFeatureCursor );
		return ipFeatureCursor;
	}

	// ��������
	_ICursorPtr SelectByAttr( ITablePtr ipTable, long* pCount /* = NULL */, LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
	{
		// �N�G���t�B���^�쐬
		IQueryFilterPtr ipFilter( CLSID_QueryFilter );
			
		// Where��w�肳��Ă�����A�Z�b�g����
		if( lpszWhereClause )
			ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where��Z�b�g

		// SubFields�w�肳��Ă�����A�Z�b�g����
		if( lpszSubFields )
			ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

		// �������ʐ��擾([Bug 6175]�C�ӂɕύX)
		if( pCount )
		{
			if( FAILED(ipTable->RowCount(ipFilter, pCount)) )
				return NULL;
		}

		// ����
		_ICursorPtr ipCursor;
		ipTable->Search( ipFilter, VARIANT_FALSE, &ipCursor );
		return ipCursor;
	}	
}

// addr_func
namespace addr_func
{
	// ���p�����H
	bool IsNumStr( const CString& strData )
	{
		int iLen = strData.GetLength();
		for( int i = 0; i < iLen; ++i )
		{
			if( !_istdigit(strData.GetAt(i)) )
				return false;
		}
		return true;
	}

	// CGouRec��r�֐�(��\�Z���I��p)
	bool CompareGouRec( const CGouRec& lhs, const CGouRec& rhs )
	{
		// �ȉ��̏����ŗD��t��(�D�捆�́A��΂Ȃ̂ł����ł͏Ȃ�)
		// (0) �厚�������łȂ�����[Bug7068]
		// (1) ���^�C�v = GOU
		// (2) �G���A��
		// (3) �ƌ`�̖ʐς��傫������
		// (4) OBJECTID�̏���������
		// (5) �G���A�O
		// (6) �ƌ`�̖ʐς��傫������
		// (7) OBJECTID�̏���������
		// (8) ���^�C�v = TPG
		// (9) (2) �` (7)�Ɠ��l

		// �ʏ̍s���E�ł̐���
		if( !lhs.m_bTsushoConv && rhs.m_bTsushoConv )	return true;
		else if( !(lhs.m_bTsushoConv ^ rhs.m_bTsushoConv) )
		{
			// ���^�C�v
			if( lhs.m_lGouType < rhs.m_lGouType )	return true;
			else if( lhs.m_lGouType == rhs.m_lGouType )
			{
				// 	�G���A���O
				if( lhs.m_bInArea && !rhs.m_bInArea )	return true;
				else if( !(lhs.m_bInArea ^ rhs.m_bInArea) )
				{
					// �ʐ�
					if( lhs.m_dKakeiArea > rhs.m_dKakeiArea )		return true;
					else if( lhs.m_dKakeiArea == rhs.m_dKakeiArea )	return lhs.m_lOID < rhs.m_lOID;	// �ʐς��������Ƃ��́AOID��������
					else											return false;
				}
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}

	// �����R�[�h�ʒu�I�ȗD��x����֐�
	int CompareGouRecPos( const CGouRec& lhs, const CGouRec& rhs )
	{		
		d_lonlat dLhs, dRhs;
		g_cCrdCnv.MeshtoLL( lhs.m_cMXY.m_nMeshCode, lhs.m_cMXY.m_nX, lhs.m_cMXY.m_nY, &dLhs.lon, &dLhs.lat );
		g_cCrdCnv.MeshtoLL( rhs.m_cMXY.m_nMeshCode, rhs.m_cMXY.m_nX, rhs.m_cMXY.m_nY, &dRhs.lon, &dRhs.lat );

		if( dLhs.lon != dRhs.lon )		return dLhs.lon > dRhs.lon? -1 : 1;
		else if( dLhs.lat != dRhs.lat )	return dLhs.lat > dRhs.lat? -1 : 1;
		else							return 0;
	}
}
