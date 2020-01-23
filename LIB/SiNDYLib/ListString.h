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
 * @brief CListStringクラス定義ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
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
 * @brief <b>CStringのリストクラス</b>\n
 * このクラスは、DLL内外で安全に文字列配列の使用・受け渡しできるように、
 * メモリ確保・開放が絡む操作に関しては全てcppファイルに記述してあります。
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
