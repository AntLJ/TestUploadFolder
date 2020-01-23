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

// AttributeRistrict.cpp: CAttributeRistrict クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttributeRistrict.h"
#include "util.h"
#include "RuleCommunity.h"
#include "Row.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR OBJECT_NOT_EXIST       = _T("チェック対象フィールド（%s）がオブジェクトに存在しません。");
	LPCTSTR EDIT_TABLE_LIMIT       = _T("編集%s：テーブル[%s] 制限[%s]\n"); 
	LPCTSTR WARNING                = _T("警告");
	LPCTSTR PROHIBITION            = _T("禁止");
	LPCTSTR RULE_IGNORE_NON_EQUAL  = _T("%s : 属性制限ルール：\"%s\" : 「=」がありません。このルールは無視されます。\n");
#else
	LPCTSTR OBJECT_NOT_EXIST       = _T("The field (%s) to be checked does not exist in the object.");
	LPCTSTR EDIT_TABLE_LIMIT       = _T("Edit %s: Table [%s] Limit [%s]\n"); 
	LPCTSTR WARNING                = _T("Warning");
	LPCTSTR PROHIBITION            = _T("Prohibition");
	LPCTSTR RULE_IGNORE_NON_EQUAL  = _T("%s : Attribute limit rule:\"%s\" : [=] does not exist. This rule is ignored.\n");
#endif	// SINDY_FOR_ENGLISH
} // namespace

namespace sindy {
using namespace errorcode;

CAttributeRistrictBase::CAttributeRistrictBase()
{
}

CAttributeRistrictBase::~CAttributeRistrictBase()
{
}

bool CAttributeRistrictBase::SetRistrict( LPCTSTR lpcszRistrict )
{
	_ASSERTE(false);
	return false;
}

CAttributeRistrict::CAttributeRistrict()
{
}
CAttributeRistrict::~CAttributeRistrict()
{
}

// 属性制限文字列をパースする
bool CAttributeRistrict::SetRistrict( LPCTSTR lpcszRistrict )
{
	CString strRistrict( lpcszRistrict );
	m_strRistrict = strRistrict;	// 文字列全体を保持

	// フィールド名と制限値文字列の切り離し
	int nPos = strRistrict.Find( _T('='), 0 );
	if( 0 < nPos )
	{
		// 前の文字が「!」の場合は否定条件
		m_bNot = ( _T('!') == strRistrict[nPos-1] ) ? true : false;

		m_strFieldName = strRistrict.Left( ( m_bNot ) ? nPos - 1 : nPos );			// フィールド名
		m_strFieldName.MakeUpper();
		strRistrict = strRistrict.Right( strRistrict.GetLength() - ( nPos + 1 ) );	// 制限値切り離し
	}
	else {
		TRACEMESSAGE( RULE_IGNORE_NON_EQUAL, __func__, strRistrict );
		return false;	// フィールド名がパース出来ない場合はエラー
	}

	// 警告フラグの処理
	if( 0 == strRistrict.Right( 2 ).CompareNoCase( _T(":W") ) )
	{
		m_bWarning = true;
		strRistrict = strRistrict.Left( strRistrict.GetLength() - 2 );
	}
	else
		m_bWarning = false;

	return _SetRistrict( strRistrict );
}

// 与えられたオブジェクトが編集可能かどうかチェック
sindyeEditableType CAttributeRistrict::IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const
{
	sindyeEditableType iRet = sindyeEditableNoDefine;

	if( m_strFieldName.IsEmpty() )
	{
		_ASSERTE( ! m_strFieldName.IsEmpty() );
		return iRet;
	}

	CComVariant vaValue;	
	cRow.GetValueByFieldName( m_strFieldName, vaValue );

	if( vaValue.vt == VT_EMPTY )
	{
		_sntprintf( lpszMessage, nLen, OBJECT_NOT_EXIST, m_strFieldName );
		iRet = sindyeEditableNoDefine;
	}
	else {
		iRet = IsEditable( vaValue );
		if( sindyeEditableEditable != iRet )
		{
			_sntprintf( lpszMessage, nLen, EDIT_TABLE_LIMIT, 
				( sindyeEditableWarnning == iRet ) ? WARNING : PROHIBITION, cRow.GetTableName(), m_strRistrict );
		}
	}
	return iRet;
}

// 与えられた値が編集可能な値かどうかをチェック
sindyeEditableType CAttributeRistrict::IsEditable( const VARIANT& vaValue ) const
{
	sindyeEditableType emType = _IsEditable( vaValue );

	// 否定条件の時はひっくり返す
	if( m_bNot )
		emType = ( sindyeEditableEditable == emType ) ? sindyeEditableNoEditable : sindyeEditableEditable;

	// 警告処理の時は編集不可を編集警告に直す
	if( m_bWarning && sindyeEditableNoEditable == emType )
		emType = sindyeEditableWarnning;

	return emType;
}

bool CAttributeRistrictMinMax::_SetRistrict( LPCTSTR lpcszRistrict )
{
	bool bRet = false;	// 返り値

	CString strRistrict( lpcszRistrict );
	int nLen = strRistrict.GetLength();

	// 期待する文字列は「0-100」とか「0-100:W」とか
	// 数値であればOKだが、_ttof
	// マイナス値の場合は、-100から-1の場合「-100--1」、-11から30の場合「-11-30」

	// 「-」を探す
	int nPos = 0, nFindPos = -1;

	while( nPos + 1 < nLen && 0 <= ( nPos = strRistrict.Find( _T('-'), nPos + 1 ) ) )	// 初回nPosは0で、nPos+1から始めるので先頭の-は自動的に無視される
	{
		// 前の文字がe又はEの場合は無視
		if( _T('e') == strRistrict[nPos-1] ||
			_T('E') == strRistrict[nPos-1] )
			continue;

		nFindPos = nPos;

		break;
	}

	if( 0 < nFindPos )
	{
		CString strLeft = strRistrict.Left( nPos );
		CString strRight = strRistrict.Right( nLen - ( nPos + 1 ) );

		USES_CONVERSION;

		double dMin  = atof( T2CA(strLeft) );
		double dMax  = atof( T2CA(strRight) );

		// 最小値 < 最大値の場合のみセットする
		if( dMin < dMax )
		{
			first = dMin;
			second = dMax;
			bRet = true;
		}
	}
	return bRet;
}

sindyeEditableType CAttributeRistrictMinMax::_IsEditable( const VARIANT& vaValue ) const
{
	sindyeEditableType emType = sindyeEditableNoDefine;	// 返り値

	// 判定値は数値であることが条件
	// doubleに変換して比較する
	CComVariant vaValueTmp;
	if( SUCCEEDED( vaValueTmp.ChangeType( VT_R8, &vaValue ) ) )
		emType = ( first <= vaValueTmp.dblVal && vaValueTmp.dblVal <= second ) ? sindyeEditableNoEditable : sindyeEditableEditable;

	return emType;
}

bool CAttributeRistrictValues::_SetRistrict( LPCTSTR lpcszRistrict )
{
	bool bRet = true;	// 返り値

	CString strRistrict( lpcszRistrict );
	int nLen = strRistrict.GetLength();
	// 期待する文字列は「0」とか「1,2,3」とか

	// 「,」を探す
	int nPos = -1;
	while( ( nPos + 1 < nLen ) && 
		   ( 0 <= ( nPos = strRistrict.Find( _T(','), nPos + 1 ) ) ) )
	{
		if( 0 != nPos )
		{
			CString strValue = strRistrict.Left( nPos );

			AddValue( strValue );
		}
		strRistrict = strRistrict.Right( nLen - ( nPos + 1 ) );
		nLen = strRistrict.GetLength();
		nPos = -1;	// [bug 3711]
	}
	AddValue( strRistrict );

	return bRet;
}

sindyeEditableType CAttributeRistrictValues::_IsEditable( const VARIANT& vaValue ) const
{
	CComVariant vaValueTmp( vaValue );

	// 数値はdoubleに変換する
	switch( vaValueTmp.vt )
	{
		case VT_I2:
		case VT_I4:
		case VT_R4:
			vaValueTmp.ChangeType( VT_R8 );
			break;
	}

	for( const_iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
		if( vaValueTmp == *it )
			return sindyeEditableNoEditable;
	}
	return sindyeEditableEditable;
}

void CAttributeRistrictValues::AddValue( LPCTSTR lpcszValue )
{
	CString strValue( lpcszValue );

	// NULLかどうか判断
	if( 0 == strValue.CompareNoCase( _T("NULL") ) )
	{
		VARIANT vaValue;
		VariantInit( &vaValue );
		vaValue.vt = VT_NULL;
		push_back( vaValue );
	}

	// strValueが文字列か数値かを判断
	if( _T('\'') == strValue[0] &&
		_T('\'') == strValue[strValue.GetLength()-1] )
	{
		// 文字として処理
		strValue.Remove( _T('\'') );	// 「'」を消去
		push_back( (LPCTSTR)strValue );
	}
	else {
		// 数値として処理
		USES_CONVERSION;
		push_back( atof( T2CA(strValue) ) );
	}
}

CAttributeRistricts::CAttributeRistricts()
{
}

CAttributeRistricts::~CAttributeRistricts()
{
	clear();
}

void CAttributeRistricts::clear()
{
	m_listRistrict.clear();
}

bool CAttributeRistricts::AddRistrict( LPCTSTR lpcszRistrict )
{
	bool bRet = true; // 戻り値

	CString strRistrict( lpcszRistrict );
	strRistrict.Trim();

	// どのタイプかチェックする
	int nLen = strRistrict.GetLength();
	boost::shared_ptr<CAttributeRistrictBase> spRistrict;

	int nPosMinus = -1;
	while( nPosMinus + 1 < nLen && 0 < ( nPosMinus = strRistrict.Find( _T('-'), nPosMinus + 1 ) ) )
	{
		// 一つ前が「=」ならもう一度
		if( _T('=') == strRistrict[nPosMinus-1] ) continue;

		// 一つ後ろが数値又は「-」ならCCAttributeRistrictMinMax
		TCHAR strPosMinusNext = strRistrict[nPosMinus+1];
		if( _T('0') == strPosMinusNext ||
			_T('1') == strPosMinusNext ||
			_T('2') == strPosMinusNext ||
			_T('3') == strPosMinusNext ||
			_T('4') == strPosMinusNext ||
			_T('5') == strPosMinusNext ||
			_T('6') == strPosMinusNext ||
			_T('7') == strPosMinusNext ||
			_T('8') == strPosMinusNext ||
			_T('9') == strPosMinusNext ||
			_T('-') == strPosMinusNext )
		{
			spRistrict.reset( new CAttributeRistrictMinMax() );
			break;
		}
	}
	if( ! spRistrict )
		spRistrict.reset( new CAttributeRistrictValues() );

	// 属性制限文字列セット
	bRet = spRistrict->SetRistrict( strRistrict );

	if( bRet )
		m_listRistrict.push_back( spRistrict );
	
	return bRet;
}

bool CAttributeRistrictsAnd::SetRistrict( LPCTSTR lpcszRistrict )
{
	bool bRet = true; // 戻り値

	CString strRistrict( lpcszRistrict );
	// 警告の処理
	if( 0 == strRistrict.Right( 2 ).CompareNoCase( _T(":W") ) )
	{
		m_bWarning = true;
		strRistrict = strRistrict.Left( strRistrict.GetLength() - 2 );
	}
	else
		m_bWarning = false;

	// 分割
	int iStart = 0;
	CString strToken = strRistrict.Tokenize( _T("|"), iStart );
	while( ! strToken.IsEmpty() )
	{
		bRet = CAttributeRistricts::AddRistrict( strToken );
		strToken = strRistrict.Tokenize( _T("|"), iStart ); // 次
	}

	return bRet;
}

sindyeEditableType CAttributeRistrictsAnd::IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const
{
	/* 0:ERR 1:WARN 2:OK */

	sindyeEditableType iRet = sindyeEditableNoDefine;

	CString strErrMessage;
	for( const_iterator it = m_listRistrict.begin(), itEnd =  m_listRistrict.end(); it != itEnd; ++it )
	{
		CString strErrMsgTmp; // エラーメッセージ一時格納用

		sindyeEditableType iRetTmp = (*it)->IsEditable( cRow, 1023, strErrMsgTmp.GetBuffer( 1024 ) );
		strErrMsgTmp.ReleaseBuffer();

		if( ! strErrMsgTmp.IsEmpty() )
			strErrMessage += strErrMsgTmp;

		if( sindyeEditableNoDefine == iRet )
			iRet = iRetTmp;
		else if( iRet != iRetTmp )
		{
			iRet = max( iRet, iRetTmp );

			// もしsindyeEditableEditableなら、strErrMessageは空にする
			if( sindyeEditableEditable == iRet )
				strErrMessage.Empty();

			break;
		}
	}

	// エラーメッセージをコピー
	if( nLen >= strErrMessage.GetLength() )
		lstrcpyn( lpszMessage, strErrMessage, nLen );
	else {
		ERRORLOG(sindyErr_BufferIsNotEnough);
		SASSERT( nLen >= strErrMessage.GetLength() );
	}

	// iRetがエラーの場合、警告フラグがあるのなら
	// 警告に変更する
	if( sindyeEditableNoEditable == iRet && m_bWarning )
		iRet = sindyeEditableWarnning;

	return iRet;
}

CAttributeRistrictsOr::CAttributeRistrictsOr()
{
}

CAttributeRistrictsOr::~CAttributeRistrictsOr()
{
}

CAttributeRistrictsOr::CAttributeRistrictsOr( const CAttributeRistrictsOr& obj )
{
	*this = obj;
}

CAttributeRistrictsOr& CAttributeRistrictsOr::operator=( const CAttributeRistrictsOr& obj )
{
	m_listRistrict = obj.m_listRistrict;
	return *this;
}

bool CAttributeRistrictsOr::AddRistrict( LPCTSTR lpcszRistrict )
{
	bool bRet = false;	// 返り値

	CString strRistrict( lpcszRistrict );
	strRistrict.Trim();
	if( ! strRistrict.IsEmpty() )
	{
		// 縦棒「|」がある場合はCAttributeRistrictsAndに（bug 4521）
		if( 0 <= strRistrict.Find(_T('|'), 0 ) )
		{
			boost::shared_ptr<CAttributeRistrictBase> spRistrict( new CAttributeRistrictsAnd() );
			if( true == ( bRet = spRistrict->SetRistrict( strRistrict ) ) )
				m_listRistrict.push_back( spRistrict );
		}
		else
			bRet = CAttributeRistricts::AddRistrict( strRistrict );
	}

	return bRet;
}

sindyeEditableType CAttributeRistrictsOr::IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const
{
	/* 0:ERR 1:WARN 2:OK */

	sindyeEditableType iRet = sindyeEditableEditable;

	CString strErrMessage;
	for( const_iterator it = m_listRistrict.begin(), itEnd =  m_listRistrict.end(); it != itEnd; ++it )
	{
		CString strErrMsgTmp; // エラーメッセージ一時格納用

		sindyeEditableType iRetTmp = (*it)->IsEditable( cRow, 1023, strErrMsgTmp.GetBuffer( 1024 ) );
		strErrMsgTmp.ReleaseBuffer();

		if( ! strErrMsgTmp.IsEmpty() )
			strErrMessage += strErrMsgTmp;

		if( iRet >= sindyeEditableNoEditable && iRet > iRetTmp )
			iRet = iRetTmp;
		else if( iRet < sindyeEditableNoEditable )
			iRet = iRetTmp;
	}

	// エラーメッセージをコピー
	if( nLen >= strErrMessage.GetLength() )
		lstrcpyn( lpszMessage, strErrMessage, nLen );
	else {
		ERRORLOG(sindyErr_BufferIsNotEnough);
		SASSERT( nLen >= strErrMessage.GetLength() );
	}

	return iRet;
}

} // sindy
