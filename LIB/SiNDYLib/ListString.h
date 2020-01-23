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
 * @file ListString.h
 * @brief CListString�N���X��`�t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _LISTSTRING_H_
#define _LISTSTRING_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"
#include <list>
#include <algorithm>

/**
 * @class CListString
 * @brief <b>CString�̃��X�g�N���X</b>\n
 * ���̃N���X�́ADLL���O�ň��S�ɕ�����z��̎g�p�E�󂯓n���ł���悤�ɁA
 * �������m�ہE�J�������ޑ���Ɋւ��Ă͑S��cpp�t�@�C���ɋL�q���Ă���܂��B
 */
class SINDYLIB_API CListString
{
public:
	CListString();
	virtual ~CListString();
	CListString( const CListString& strs ){ push_back( strs ); }
	CListString( const std::list<CString>& strs ){ push_back( strs ); }
	CListString( LPCTSTR lpcszStr, LPCTSTR lpcszSep ){ push_back( lpcszStr, lpcszSep ); }
	CListString& operator=( const CListString& strs ){ clear(); push_back( strs ); return *this; }
	CListString& operator=( const std::list<CString>& strs ){ clear(); push_back( strs ); return *this; }
	CListString& operator+=( const CListString& strs ){ push_back( strs ); return *this; }
	CListString& operator+=( const std::list<CString>& strs ){ push_back( strs ); return *this; }
	void push_back( LPCTSTR lpcszStr, LPCTSTR lpcszSep );
	void push_back( LPCTSTR lpcszStr );
	void push_back( const CListString& strs );
	void push_back( const std::list<CString>& strs );
	void clear();
	void sort(){ m_listStr.sort(); }
	void unique();
	typedef std::list<CString>::iterator iterator;
	typedef std::list<CString>::const_iterator const_iterator;
	iterator begin(){ return m_listStr.begin(); }
	iterator end(){ return m_listStr.end(); }
	const_iterator begin() const { return m_listStr.begin(); }
	const_iterator end() const { return m_listStr.end(); }
	size_t size() const { return m_listStr.size(); }
	bool empty() const { return m_listStr.empty(); }
	const_iterator find(LPCTSTR lpcszStr) const { return std::find( begin(), end(), lpcszStr ); }
private:
#pragma warning(push)
#pragma warning(disable: 4251)
	std::list<CString> m_listStr;
#pragma warning(pop)
};

#endif // ifndef _LISTSTRING_H_
