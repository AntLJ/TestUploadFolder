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
#include "AddrToXyProcesser.h"
#include "globalfunc.h"

#include "Logger.h"

extern crd_cnv g_cnv;

void AddrToXyProcesser::init( const CString& svr, const CString& usr )
{
	connectAdo( svr, usr, m_ado );
}

bool AddrToXyProcesser::execFunc( const CString& addr )
{
	using namespace uh::str_util;

	CADOCursor cur = m_ado.search( formatA( 
		_T("select addr_to_xy('%s') from dual"), addr ) );

	if( FAILED( cur.err() ) )
	{
		// ここはFATAL
		throw std::runtime_error( formatA( _T("addr_to_xyに失敗： %s"), cur.GetErrorMessage() ));
	}

	for( auto& it : cur )
	{
		// 空列も含めて結果を取得
		CComVariant& val = (it)[0];
		m_result = split( ToString(val), _T(","), true );
	}

	// 住所レベルが帰ってきていなければXY解析失敗（xyaddr.exeと同等）
	if( m_result[ addr_to_xy::AddrLevel ].IsEmpty() )
		return false;

	return true;
}

CString AddrToXyProcesser::getAddrCode() const 
{
	using namespace addr_to_xy;

	// 県コード～号コードをくっつける
	const static std::vector<long > idxs = boost::assign::list_of
		(PrefCode)(CityCode)(OazaCode)(ChomeCode)(ChibanCode)(GouCode);

	CString code20;
	for( long idx : idxs )
		code20.Append( m_result[idx] );

	return code20;
}

WKSPoint AddrToXyProcesser::getLonLat() const
{
	using namespace addr_to_xy;

	WKSPoint lonLat = {};
	g_cnv.MeshtoLL( _ttol(m_result[Mesh]), _ttol(m_result[X]), _ttol(m_result[Y]),
		            &lonLat.X, &lonLat.Y );

	return lonLat;
}

long AddrToXyProcesser::getAddrLevel() const
{
	// XY付与結果の「精度」を返す
	return _ttol(m_result[ addr_to_xy::Precision ]);
}

bool AddrToXyProcesser::isPinpoint() const
{
	// ピンポイント判定は、
	// ・精度5
	//   または
	// ・号コード(20桁コード末尾4桁)が"0000"かつ、精度4かつ、地番止まりフラグが2か4

	int precision = _ttoi( m_result[addr_to_xy::Precision] );

	if( precision == 5 )
		return true;
	
	int chiban = _ttoi( m_result[addr_to_xy::ChibanF] );

	if( m_result[addr_to_xy::GouCode].Compare( _T("0000") ) == 0 &&
		precision == 4 &&
		( chiban == 2 || chiban == 4 ) )
		return true;

	return false;
}
