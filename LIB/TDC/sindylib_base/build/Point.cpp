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
 * @file Point.cpp
 * @brief CPointクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "Point.h"
#include "ErrorCode.h"
#include "sindymacroutil.h"
#include "sindycomutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef SINDY_FOR_ENGLISH
const static _TCHAR MOVE_POINT_INFO[] = _T("%s : ポイント移動：X=%.20lf(差分%.20lf) Y=%.20lf(差分%.20lf)\n");
#else
const static _TCHAR MOVE_POINT_INFO[] = _T("%s : Move a point：X=%.20lf(diff%.20lf) Y=%.20lf(diff%.20lf)\n");
#endif	// SINDY_FOR_ENGLISH

namespace sindy {
using namespace errorcode;
double CPoint::GetX() const
{
	double dX = 0.0;
	INVOKEMETHOD( get_X( &dX ) );
	return dX;
}

double CPoint::GetY() const
{
	double dY = 0.0;
	INVOKEMETHOD( get_Y( &dY ) );
	return dY;
}

sindyErrCode CPoint::SetX( const double& X )
{
	INVOKEMETHOD( put_X( X ) );
	return (SUCCEEDED(hr)) ? sindyErr_NoErr : sindyErr_GeometryFunctionFailed;
}

sindyErrCode CPoint::SetY( const double& Y )
{
	INVOKEMETHOD( put_Y( Y ) );
	return (SUCCEEDED(hr)) ? sindyErr_NoErr : sindyErr_GeometryFunctionFailed;
}

void CPoint::QueryCoords( WKSPoint& point ) const
{
	INVOKEMETHOD( QueryCoords( &point.X, &point.Y ) );
}

std::pair<double,double> CPoint::QueryCoords() const
{
	std::pair<double,double> pairPoint;	// 返り値
	INVOKEMETHOD( QueryCoords( &pairPoint.first, &pairPoint.second ) );
	return pairPoint;
}

bool CPoint::Equals( const WKSPoint& point ) const
{
	bool bRet = false;	// 返り値

	if( p )
	{
		WKSPoint current = {0,0};
		QueryCoords( current );
		bRet = ( current.X == point.X && current.Y == point.Y );
	}
	return bRet;
}

bool CPoint::Equals( IPoint* lp ) const
{
	bool bRet = false;	// 返り値

	if( p && lp )
	{
		WKSPoint point = {0,0};
		QueryCoords( point );

		double dX = 0.0, dY = 0.0;
		LOGASSERTE_IF( SUCCEEDED( lp->QueryCoords( &dX, &dY ) ), sindyErr_GeometryFunctionFailed )
		{
			bRet = ( point.X == dX && point.Y == dY );
		}
	}
	return bRet;
}

// ポイントを移動する
sindyErrCode CPoint::Move( const std::pair<double,double>& pairMove )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	ITransform2DPtr ipTrans;
	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;

	LOGASSERTEERR_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ipGeom->QueryInterface( IID_ITransform2D, (void**)&ipTrans ) ), sindyErr_COMQueryInterfaceFailed )
		{
			LOGASSERTEERR_IF( SUCCEEDED( ipTrans->Move( pairMove.first, pairMove.second ) ), sindyErr_GeometryFunctionFailed )
			{
				TRACEMESSAGE( MOVE_POINT_INFO, __func__, GetX(), pairMove.first, GetY(), pairMove.second );
			}
		}
	}

	return emErr;
}

} // sindy
