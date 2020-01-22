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
		// ������FATAL
		throw std::runtime_error( formatA( _T("addr_to_xy�Ɏ��s�F %s"), cur.GetErrorMessage() ));
	}

	for( auto& it : cur )
	{
		// �����܂߂Č��ʂ��擾
		CComVariant& val = (it)[0];
		m_result = split( ToString(val), _T(","), true );
	}

	// �Z�����x�����A���Ă��Ă��Ȃ����XY��͎��s�ixyaddr.exe�Ɠ����j
	if( m_result[ addr_to_xy::AddrLevel ].IsEmpty() )
		return false;

	return true;
}

CString AddrToXyProcesser::getAddrCode() const 
{
	using namespace addr_to_xy;

	// ���R�[�h�`���R�[�h����������
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
	// XY�t�^���ʂ́u���x�v��Ԃ�
	return _ttol(m_result[ addr_to_xy::Precision ]);
}

bool AddrToXyProcesser::isPinpoint() const
{
	// �s���|�C���g����́A
	// �E���x5
	//   �܂���
	// �E���R�[�h(20���R�[�h����4��)��"0000"���A���x4���A�n�Ԏ~�܂�t���O��2��4

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
