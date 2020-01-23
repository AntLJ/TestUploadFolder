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

#pragma once

#include <boost/utility.hpp>
#include <tuple>

class CTimeTrace : boost::noncopyable
{
public:
	typedef std::map<CString,std::map<DWORD,std::tr1::tuple<long,DWORD,DWORD>>> TRACEMAP;

	void Start( LPCTSTR lpcszKeyword );
	void End( LPCTSTR lpcszKeyword );
	void Report( FILE* out = NULL ) const;
	void Report( std::list<CString>& listReport ) const;
	void Trace() const;
	void Clear(){ m_mapTime.clear(); }
	const TRACEMAP& Get(){ return m_mapTime; }
private:
	TRACEMAP m_mapTime; // key:keyword value:times,sum,start time
};

/**
 * @class CFuncTraceWrapper
 * @brief �֐��̎��Ԃ��v�����������ǁA�r��return����R�����Ėʓ|�A
 * ���Ă����ꍇ�Ɏg�p���܂��B
 * �֐��̓��� 
 * <pre>
 * CFuncTraceWrapper ft( trace, _T("�ق�") );
 * </pre>
 * �Ƃ���ƁA�֐��𔲂���Ƃ��� CTimeTrace::End���Ă���܂��B
 * ���R�ł����A�����܂ł��֐��P�ʂł̌v���������܂���B
 */
class CFuncTraceWrapper
{
public:
	CFuncTraceWrapper( CTimeTrace& trace, LPCTSTR lpcszTag ) : m_t(&trace), m_tag(lpcszTag)
	{
		m_t->Start(m_tag);
	}
	~CFuncTraceWrapper()
	{
		m_t->End(m_tag);
	}
private:
	CTimeTrace* m_t;
	CString m_tag;
};
