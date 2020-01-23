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
 * @file ListLong.h
 * @brief CListLong�N���X��`�t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _LISTLONG_H_
#define _LISTLONG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"
#include <list>
#include <algorithm>

/**
 * @class CListLong
 * @brief <b>long�̃��X�g�N���X</b>\n
 * ���̃N���X�́ADLL���O�ň��S��long�z��̎g�p�E�󂯓n�����ł���悤�ɁA
 * �������m�ہE�J�������ޑ���Ɋւ��Ă͑S��cpp�t�@�C���ɋL�q���Ă���܂��B
 */
class SINDYLIB_API CListLong
{
public:
	CListLong();
	virtual ~CListLong();
	CListLong( const CListLong& longs ){ push_back( longs ); }
	CListLong( const std::list<long>& longs ){ push_back( longs ); }
	CListLong( LPCTSTR lpcszStr, LPCTSTR lpcszSep ){ push_back( lpcszStr, lpcszSep ); }
	CListLong& operator=( const CListLong& longs ){ clear(); push_back( longs ); return *this; }
	CListLong& operator=( const std::list<long>& longs ){ clear(); push_back( longs ); return *this; }
	CListLong& operator+=( const CListLong& longs ){ push_back( longs ); return *this; }
	CListLong& operator+=( const std::list<long>& longs ){ push_back( longs ); return *this; }
	void push_back( LPCTSTR lpcszStr, LPCTSTR lpcszSep );
	void push_back( long l );
	void push_back( const CListLong& ids );
	void push_back( const std::list<long>& ids );
	void clear();
	void sort(){ m_listLong.sort(); }
	void unique();
	typedef std::list<long>::iterator iterator;
	typedef std::list<long>::const_iterator const_iterator;
	iterator begin(){ return m_listLong.begin(); }
	iterator end(){ return m_listLong.end(); }
	const_iterator begin() const { return m_listLong.begin(); }
	const_iterator end() const { return m_listLong.end(); }
	unsigned int size() const { return m_listLong.size(); }
	bool empty() const { return m_listLong.empty(); }
	const_iterator find(long l) const { return std::find( begin(), end(), l ); }
private:
#pragma warning(push)
#pragma warning(disable:4251)
	std::list<long> m_listLong;
#pragma warning(pop)
};

#endif // ifndef _LISTLONG_H_
