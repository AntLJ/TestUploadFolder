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
 * @brief 関数の時間を計測したいけど、途中returnが沢山あって面倒、
 * っていう場合に使用します。
 * 関数の頭で 
 * <pre>
 * CFuncTraceWrapper ft( trace, _T("ほげ") );
 * </pre>
 * とすると、関数を抜けるときに CTimeTrace::Endしてくれます。
 * 当然ですが、あくまでも関数単位での計測しかしません。
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
