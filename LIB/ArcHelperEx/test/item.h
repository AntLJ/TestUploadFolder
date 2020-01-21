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

// moc class
class CItem
{
public:
	CItem() : m_vaFrom(-1L), m_vaTo(-1L), m_lOID(-1), m_bDirected(false)
	{
	}
	CItem( long from, long to, long oid, bool directed ) : m_vaFrom(from), m_vaTo(to), m_lOID(oid), m_bDirected(directed)
	{
	}
	bool operator == ( const CItem& item ) const
	{
		return ( GetOID() == item.GetOID() );
	}
	bool operator != ( const CItem& item ) const
	{
		return ( GetOID() != item.GetOID() );
	}
	CComVariant GetAttribute(LPCTSTR lpcsz) const
	{
		if( 0 == _tcsicmp(_T("FROM_NODE_ID"), lpcsz) )
			return m_vaFrom;
		else if( 0 == _tcsicmp(_T("TO_NODE_ID"), lpcsz) )
			return m_vaTo;
		return CComVariant();
	}
	long GetOID() const
	{
		return m_lOID;
	}
	CComVariant m_vaFrom, m_vaTo;
	long m_lOID;
	bool m_bDirected;
};

typedef std::list<CItem> CItems;
