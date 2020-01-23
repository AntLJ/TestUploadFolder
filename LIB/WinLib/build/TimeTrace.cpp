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
#include "TimeTrace.h"
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

typedef std::pair<CString,std::map<DWORD,std::tr1::tuple<long,DWORD,DWORD>>> TRACEPAIR;
typedef std::map<DWORD,std::tr1::tuple<long,DWORD,DWORD>> TH_TRACEMAP;
typedef std::pair<DWORD,std::tr1::tuple<long,DWORD,DWORD>> TH_TRACEPAIR;

void CTimeTrace::Start( LPCTSTR lpcszKeyword )
{
	// �}���`�X���b�h�Ή��̂��߁A�X���b�hID���Z�b�g�ŋL������
	DWORD dwID = GetCurrentThreadId();
	TRACEMAP::iterator it = m_mapTime.find(lpcszKeyword);
	if( it == m_mapTime.end() )
		m_mapTime[lpcszKeyword][dwID] = std::tr1::make_tuple( 0, 0, GetTickCount() );
	else {
		std::tr1::tuple<long,DWORD,DWORD>& time = (it->second)[dwID];
		std::tr1::get<2>(time) = GetTickCount();
	}
}

void CTimeTrace::End( LPCTSTR lpcszKeyword )
{
	DWORD dwID = GetCurrentThreadId();
	TRACEMAP::iterator it = m_mapTime.find(lpcszKeyword);
	if( it == m_mapTime.end() )
		_ASSERTE( it != m_mapTime.end() ); // Start����Ă��Ȃ�
	else {
		TH_TRACEMAP::iterator it2 = it->second.find(dwID);
		if( it2 == it->second.end() )
			_ASSERTE( it2 != it->second.end() ); // Start����Ă��Ȃ��i�X���b�h���j
		else {
			std::tr1::tuple<long,DWORD,DWORD>& time = it2->second;
			if( 0 == std::tr1::get<2>(time) )
				_ASSERTE( 0 != std::tr1::get<2>(time) ); // Start����Ă��Ȃ�
			else {
				std::tr1::get<1>(time) += GetTickCount() - std::tr1::get<2>(time);
				++std::tr1::get<0>(time);
				std::tr1::get<2>(time) = 0;
			}
		}
	}
}

CString TIME2STR( DWORD time )
{
	if( 0 == time )
		return _T("0");

	CString strRet; // �߂�l

	DWORD dwHour = time/3600000;
	DWORD timeTmp = time%3600000;
	DWORD dwMin = timeTmp/60000;
	timeTmp = time%60000;
	DWORD dwSec = timeTmp;
	if( 0 != dwHour )
		strRet.Format(_T("%3d����"),dwHour);
	if( 0 != dwHour && 0 == dwMin )
		strRet += _T("00��");
	else if( 0 != dwMin )
	{
		CString strRetTmp( strRet );
		strRet.Format(_T("%s%2d��"), strRetTmp, dwMin );
	}
	if( 0 == dwSec )
		strRet += _T("00.000�b");
	else {
		CString strRetTmp( strRet );
		strRet.Format(_T("%s%2.3lf�b"), strRetTmp, (double)dwSec/1000.0 );
	}
	return strRet;
}

void CTimeTrace::Report( FILE* out/* = NULL*/ ) const
{
	if( ! out )
		out = stdout;

	fprintf_s( out, "%s\n", "----------���Ԍv���W�v-------------" );
	for( TRACEMAP::const_iterator it = m_mapTime.begin(); it != m_mapTime.end(); ++it )
	{
		for( TH_TRACEMAP::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			if( 0 == std::tr1::get<0>(it2->second) )
			{
				ATLTRACE(_T("CTimeTrace : [%s] ��End����Ă��܂���\n"), it->first);
				_ASSERTE( 0 != std::tr1::get<0>(it2->second) ); // End���Ȃ��H
			}
			else
				fprintf_s( out, "%-30s %15s�i���ρF%15s/%ld��)\n", CStringA(it->first), CStringA(TIME2STR(std::tr1::get<1>(it2->second))), CStringA(TIME2STR(std::tr1::get<1>(it2->second)/std::tr1::get<0>(it2->second))), std::tr1::get<0>(it2->second) );
		}
	}
}

void CTimeTrace::Report( std::list<CString>& listReport ) const
{
	listReport.push_back(CString());
	listReport.rbegin()->Format(_T("%s"), _T("----------���Ԍv���W�v-------------") );
	for( TRACEMAP::const_iterator it = m_mapTime.begin(); it != m_mapTime.end(); ++it )
	{
		for( TH_TRACEMAP::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			if( 0 == std::tr1::get<0>(it2->second) )
			{
				ATLTRACE(_T("CTimeTrace : [%s] ��End����Ă��܂���\n"), it->first);
				_ASSERTE( 0 != std::tr1::get<0>(it2->second) ); // End���Ȃ��H
			}
			else {
				CStringA tmp;
				CStringA label(it->first);
				if( 30 < label.GetLength() )
					tmp.Format("%s\n%46s�i���ρF%15s/%ld��)", label, CStringA(TIME2STR(std::tr1::get<1>(it2->second))), CStringA(TIME2STR(std::tr1::get<1>(it2->second)/std::tr1::get<0>(it2->second))), std::tr1::get<0>(it2->second) );
				else
					tmp.Format("%-30s %15s�i���ρF%15s/%ld��)", label, CStringA(TIME2STR(std::tr1::get<1>(it2->second))), CStringA(TIME2STR(std::tr1::get<1>(it2->second)/std::tr1::get<0>(it2->second))), std::tr1::get<0>(it2->second) );
				listReport.push_back(CString(tmp));
			}
		}
	}
}

void CTimeTrace::Trace() const
{
	// ���Ԍv�����|�[�g
	std::list<CString> report;
	Report(report);
	foreach( const CString& str, report )
		ATLTRACE(_T("%s\n"), str );
}
