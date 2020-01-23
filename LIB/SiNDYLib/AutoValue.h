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

#include "exportimport.h"

namespace sindy {

class SINDYLIB_API CAutoValue
{
public:
	CAutoValue(void);
	virtual ~CAutoValue(void);

	typedef std::list<std::pair<CString,CString> >::iterator iterator;
	typedef std::list<std::pair<CString,CString> >::const_iterator const_iterator;
	iterator begin() { return m_listValue.begin(); }
	iterator end() { return m_listValue.end(); }
	const_iterator begin() const { return m_listValue.begin(); }
	const_iterator end() const { return m_listValue.end(); }

	void Init( LPCTSTR lpcszAutoValue );
private:
#pragma warning(push)
#pragma warning(disable:4251)
	std::list<std::pair<CString,CString> > m_listValue; //!< フィールド名と値のペア
#pragma warning(pop)
};

} // sindy
