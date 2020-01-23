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

/**
 * @file CacheGeometry.cpp
 * @brief <b>CCacheGeometryクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "CacheGeometry.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

CCacheGeometry::CCacheGeometry()
{
	m_pPoints = NULL;
	Init();
}

CCacheGeometry::~CCacheGeometry()
{
	Init();
}

CCacheGeometry::CCacheGeometry( IGeometry* pGeom )
{
	m_pPoints = NULL;
	CreateCache( pGeom );
}
	
CCacheGeometry::CCacheGeometry( const CCacheGeometry& lp )
{
	m_pPoints = NULL;
	*this = lp;
}

CCacheGeometry& CCacheGeometry::operator=( IGeometry* pGeom )
{
	CreateCache( pGeom );
	return *this;
}

CCacheGeometry& CCacheGeometry::operator=( const CCacheGeometry& lp )
{
	CopyCacheGeometry( *this, lp );
	return *this;
}

sindyErrCode CopyCacheGeometry( CCacheGeometry& dest, const CCacheGeometry& src )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	dest.DeleteCache();	// 既存のキャッシュは削除
	dest.m_lCount = src.m_lCount;
	if( src.m_pPoints )
	{
		dest.m_pPoints = new WKSPoint[dest.m_lCount];
		if( dest.m_pPoints )
			memcpy( dest.m_pPoints, src.m_pPoints, sizeof(WKSPoint) * dest.m_lCount );
		else {
			emErr = sindyErr_MemoryAllocationFailed;	// メモリアロケーションエラー
			ERRORLOG(emErr);
		}
	}

	return emErr;
}

/*CPoint* CCacheGeometry::operator[]( long index )
{
	CPoint* pPoint = NULL;	// 返り値

	if( m_pPoints )
	{
		if( index < m_lCount )
			pPoint = &m_pPoints[index];
		else
			_ASSERTE( index < m_lCount );
	}
	else
		_ASSERTE( m_pPoints != NULL );

	return pPoint;
}*/

const WKSPoint* CCacheGeometry::operator[]( long index ) const
{
	WKSPoint* pPoint = NULL;	// 返り値

	if( m_pPoints )
	{
		if( index < m_lCount )
			pPoint = &m_pPoints[index];
	}
	else
		_ASSERTE( m_pPoints != NULL );

	return pPoint;
}

void CCacheGeometry::Init()
{
	if( m_pPoints )
	{
		delete [] m_pPoints;
		m_pPoints = NULL;
	}
	m_lCount = 0;
}

sindyErrCode CCacheGeometry::CreateCache( IGeometry* pGeom )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if( pGeom )
	{
		esriGeometryType type;
		pGeom->get_GeometryType( &type );
		switch( type )
		{
			case esriGeometryPoint:
			{
				IPoint* pP = NULL;
				if( SUCCEEDED( pGeom->QueryInterface( IID_IPoint, (void**)&pP ) ) )
					emErr = CreateCache( pP );
				else {
					_ASSERT( false );
					emErr = sindyErr_COMQueryInterfaceFailed;
				}
				break;
			}
			default:
			{
				IPointCollection* pCol = NULL;
				if( SUCCEEDED( pGeom->QueryInterface( IID_IPointCollection, (void**)&pCol ) ) )
					emErr = CreateCache( pCol );
				else {
					_ASSERT( false );
					emErr = sindyErr_COMQueryInterfaceFailed;
				}
				break;
			}
		}
	}
	else
		emErr = sindyErr_COMInterfaceIsNull;

	return emErr;
}

sindyErrCode CCacheGeometry::CreateCache( IPoint* pGeom )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if( pGeom )
	{
		Init(); // 初期化

		m_pPoints = new WKSPoint[1];
		if( FAILED( pGeom->QueryCoords( &m_pPoints[0].X, &m_pPoints[0].Y ) ) )
		{
			_ASSERTE( false );
			emErr = sindyErr_COMQueryInterfaceFailed;
		}
		else
			m_lCount = 1;
	}
	else {
		_ASSERTE( pGeom );
		emErr = sindyErr_COMInterfaceIsNull;
	}

	return emErr;
}

sindyErrCode CCacheGeometry::CreateCache( IPointCollection* pGeom )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if( pGeom )
	{
		Init();	// 初期化

		// 点列コピー
		if( SUCCEEDED( pGeom->get_PointCount( &m_lCount ) ) )
		{
			m_pPoints = new WKSPoint[m_lCount];
			if( FAILED( pGeom->QueryWKSPoints( 0, m_lCount, m_pPoints ) ) )
			{
				_ASSERTE( false );
				emErr = sindyErr_COMFunctionFailed;
			}
		}
		else {
			_ASSERTE( false );
			emErr = sindyErr_COMFunctionFailed;
		}
	}
	else {
		_ASSERTE( pGeom != NULL );
		return sindyErr_COMInterfaceIsNull;
	}

	return emErr;
}

// キャッシュを削除する
void CCacheGeometry::DeleteCache()
{
	if( m_pPoints )
	{
		delete [] m_pPoints;
		m_pPoints = NULL;
	}
	m_lCount = 0;
}

} // sindy
