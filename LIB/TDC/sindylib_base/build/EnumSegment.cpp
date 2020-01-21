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
 * @file EnumSegment.cpp
 * CEnumSegmentクラス実装ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "EnumSegment.h"
#include "ErrorCode.h"
#include "sindycomutil.h"
#include "sindymacroutil.h"
#include "math2.h"
#include "type_convert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;
bool CEnumSegment::operator==( const CEnumSegment& obj ) const
{
	// 同じジオメトリかどうかは調べようがないのでしらん
	if( ( ( ( ! p ) || ( -1 == m_lPart && -1 == m_lIndex ) ) && ( ( ! obj.p ) || ( -1 == obj.m_lPart && -1 == obj.m_lIndex ) ) ) ||	// 空のオブジェクトとラストは同じ物と解釈する（end()対策）
		( m_lPart == obj.m_lPart && m_lIndex == obj.m_lIndex ) )
		return true;
	return false;
}
/*
const CEnumSegment& CEnumSegment::operator++() const
{
	if( NULL == m_ipSeg )
		m_ipSeg = Next( dynamic_cast<long*>(&m_lPart), dynamic_cast<long*>(&m_lIndex) );
	else
		QueryNext( m_ipSeg, dynamic_cast<long*>(&m_lPart), dynamic_cast<long*>(&m_lIndex) );

	return *this;
}

const CEnumSegment& CEnumSegment::operator--() const
{
	if( NULL == m_ipSeg )
		m_ipSeg = Previous( dynamic_cast<long*>(&m_lPart), dynamic_cast<long*>(&m_lIndex) );
	else
		QueryPrevious( m_ipSeg, dynamic_cast<long*>(&m_lPart), dynamic_cast<long*>(&m_lIndex) );

	return *this;
}
*/
CEnumSegment& CEnumSegment::operator++()
{
	m_ipSeg = Next( &m_lPart, &m_lIndex );

	return *this;
}

CEnumSegment& CEnumSegment::operator--()
{
	m_ipSeg = Previous( &m_lPart, &m_lIndex );

	return *this;
}

/*const CEnumSegment& CEnumSegment::operator+( long n ) const
{
}

const CEnumSegment& CEnumSegment::operator-( long n ) const
{
}*/

bool CEnumSegment::IsLastInPart() const
{
	VARIANT_BOOL vb = VARIANT_FALSE;	// 返り値
	INVOKEMETHOD( IsLastInPart( &vb ) );
	return VB2bool(vb);
}
_ISegmentPtr CEnumSegment::Next( long* outPartIndex/* = NULL*/, long* SegmentIndex/* = NULL*/ ) const
{
	_ISegmentPtr ipSegment;	// 返り値
	if( p )
	{
		long lPart = -1, lIndex = -1;
		HRESULT hr = p->Next( &ipSegment, &lPart, &lIndex );
		if( S_OK == hr )
		{
			if( outPartIndex ) *outPartIndex = lPart;
			if( SegmentIndex  ) *SegmentIndex  = lIndex;
		}
		// 最後までくると S_FALSE が返るが、なぜかパート番号は0のまま
		// なので、-1にしておく（そうしないと最終判定ができない）
		else if( S_FALSE == hr )
		{
			if( outPartIndex ) *outPartIndex = -1;	// 
			if( SegmentIndex ) *SegmentIndex = -1;
		}
		else {
			LOGASSERTE_IF( SUCCEEDED(hr), sindyErr_GeometryFunctionFailed );
		}
	}
	return ipSegment;
}

_ISegmentPtr CEnumSegment::NextInPart( long* OutSegmentIndex/* = NULL*/ ) const
{
	_ISegmentPtr ipSegment;	// 返り値
	if( p )
	{
		long lIndex = -1;
		HRESULT hr = p->NextInPart( &ipSegment, &lIndex );
		if( S_OK == hr )
		{
			if( OutSegmentIndex ) *OutSegmentIndex  = lIndex;
		}
		else if( S_FALSE == hr )
		{
			if( OutSegmentIndex ) *OutSegmentIndex = -1;
			LOGASSERTE_IF( IsLastInPart(), sindyErr_AlgorithmFailed );	// パートの最後の確認
		}
		else {
			LOGASSERTE_IF( SUCCEEDED(hr), sindyErr_GeometryFunctionFailed );
		}
	}
	return ipSegment;
}
_ISegmentPtr CEnumSegment::Previous( long* outPartIndex/* = NULL*/, long* SegmentIndex/* = NULL*/ ) const
{
	_ISegmentPtr ipSegment;	// 返り値
	if( p )
	{
		long lPart = -1, lIndex = -1;
		HRESULT hr = p->Previous( &ipSegment, &lPart, &lIndex );
		if( S_OK == hr )
		{
			if( outPartIndex ) *outPartIndex = lPart;
			if( SegmentIndex  ) *SegmentIndex  = lIndex;
		}
		else if( S_FALSE == hr )
		{
			if( outPartIndex ) *outPartIndex = -1;	// 
			if( SegmentIndex ) *SegmentIndex = -1;
		}
		else {
			LOGASSERTE_IF( SUCCEEDED(hr), sindyErr_GeometryFunctionFailed );
		}
	}
	return ipSegment;
}

sindyErrCode CEnumSegment::Reset() const
{
	INVOKEMETHOD( Reset() );
	return (SUCCEEDED(hr)) ? sindyErr_NoErr : sindyErr_COMFunctionFailed;
}

sindyErrCode CEnumSegment::ResetToEnd() const
{
	INVOKEMETHOD( ResetToEnd() );
	return (SUCCEEDED(hr)) ? sindyErr_NoErr : sindyErr_COMFunctionFailed;
}

sindyErrCode CEnumSegment::SetAt( long iPart, long Segment ) const
{
	INVOKEMETHOD( SetAt( iPart, Segment ) );
	return (SUCCEEDED(hr)) ? sindyErr_NoErr : sindyErr_COMFunctionFailed;
}

sindyErrCode CEnumSegment::Skip( long numSegments ) const
{
	INVOKEMETHOD( Skip( numSegments ) );
	return (SUCCEEDED(hr)) ? sindyErr_NoErr : sindyErr_COMFunctionFailed;
}

double CEnumSegment::degree( ILine* ipLine ) const
{
	double deg = -1.0;	// 返り値

	LOGASSERTE_IF( ipLine, sindyErr_ArgIsNull )
	{
		IConstructAnglePtr ipAngle;
		LOGASSERTE_IF( SUCCEEDED( ipAngle.CreateInstance( CLSID_GeometryEnvironment ) ), sindyErr_COMCreateInstanceFailed )
		{
			double degThis = -1.0, degParam = -1.0;
			LOGASSERTE_IF( SUCCEEDED( ipAngle->ConstructLine( (ILinePtr)(_ISegment*)*this, &degThis ) ) &&
						   SUCCEEDED( ipAngle->ConstructLine( ipLine, &degParam ) ), sindyErr_GeometryFunctionFailed )
			{
				// ラジアン->度変換
				deg = rad2angle( M_PI - degThis + degParam );
				// 0 - 360 に収める
				if( 0.0 > deg ) deg = 360.0 - deg;
				while( 360.0 < deg ) deg -= 360.0;
			}
		}
	}

	return deg;
}

} // sindy
