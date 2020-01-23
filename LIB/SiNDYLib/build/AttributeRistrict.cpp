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

// AttributeRistrict.cpp: CAttributeRistrict �N���X�̃C���v�������e�[�V����
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
	LPCTSTR OBJECT_NOT_EXIST       = _T("�`�F�b�N�Ώۃt�B�[���h�i%s�j���I�u�W�F�N�g�ɑ��݂��܂���B");
	LPCTSTR EDIT_TABLE_LIMIT       = _T("�ҏW%s�F�e�[�u��[%s] ����[%s]\n"); 
	LPCTSTR WARNING                = _T("�x��");
	LPCTSTR PROHIBITION            = _T("�֎~");
	LPCTSTR RULE_IGNORE_NON_EQUAL  = _T("%s : �����������[���F\"%s\" : �u=�v������܂���B���̃��[���͖�������܂��B\n");
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

// ����������������p�[�X����
bool CAttributeRistrict::SetRistrict( LPCTSTR lpcszRistrict )
{
	CString strRistrict( lpcszRistrict );
	m_strRistrict = strRistrict;	// ������S�̂�ێ�

	// �t�B�[���h���Ɛ����l������̐؂藣��
	int nPos = strRistrict.Find( _T('='), 0 );
	if( 0 < nPos )
	{
		// �O�̕������u!�v�̏ꍇ�͔ے����
		m_bNot = ( _T('!') == strRistrict[nPos-1] ) ? true : false;

		m_strFieldName = strRistrict.Left( ( m_bNot ) ? nPos - 1 : nPos );			// �t�B�[���h��
		m_strFieldName.MakeUpper();
		strRistrict = strRistrict.Right( strRistrict.GetLength() - ( nPos + 1 ) );	// �����l�؂藣��
	}
	else {
		TRACEMESSAGE( RULE_IGNORE_NON_EQUAL, __func__, strRistrict );
		return false;	// �t�B�[���h�����p�[�X�o���Ȃ��ꍇ�̓G���[
	}

	// �x���t���O�̏���
	if( 0 == strRistrict.Right( 2 ).CompareNoCase( _T(":W") ) )
	{
		m_bWarning = true;
		strRistrict = strRistrict.Left( strRistrict.GetLength() - 2 );
	}
	else
		m_bWarning = false;

	return _SetRistrict( strRistrict );
}

// �^����ꂽ�I�u�W�F�N�g���ҏW�\���ǂ����`�F�b�N
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

// �^����ꂽ�l���ҏW�\�Ȓl���ǂ������`�F�b�N
sindyeEditableType CAttributeRistrict::IsEditable( const VARIANT& vaValue ) const
{
	sindyeEditableType emType = _IsEditable( vaValue );

	// �ے�����̎��͂Ђ�����Ԃ�
	if( m_bNot )
		emType = ( sindyeEditableEditable == emType ) ? sindyeEditableNoEditable : sindyeEditableEditable;

	// �x�������̎��͕ҏW�s��ҏW�x���ɒ���
	if( m_bWarning && sindyeEditableNoEditable == emType )
		emType = sindyeEditableWarnning;

	return emType;
}

bool CAttributeRistrictMinMax::_SetRistrict( LPCTSTR lpcszRistrict )
{
	bool bRet = false;	// �Ԃ�l

	CString strRistrict( lpcszRistrict );
	int nLen = strRistrict.GetLength();

	// ���҂��镶����́u0-100�v�Ƃ��u0-100:W�v�Ƃ�
	// ���l�ł����OK�����A_ttof
	// �}�C�i�X�l�̏ꍇ�́A-100����-1�̏ꍇ�u-100--1�v�A-11����30�̏ꍇ�u-11-30�v

	// �u-�v��T��
	int nPos = 0, nFindPos = -1;

	while( nPos + 1 < nLen && 0 <= ( nPos = strRistrict.Find( _T('-'), nPos + 1 ) ) )	// ����nPos��0�ŁAnPos+1����n�߂�̂Ő擪��-�͎����I�ɖ��������
	{
		// �O�̕�����e����E�̏ꍇ�͖���
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

		// �ŏ��l < �ő�l�̏ꍇ�̂݃Z�b�g����
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
	sindyeEditableType emType = sindyeEditableNoDefine;	// �Ԃ�l

	// ����l�͐��l�ł��邱�Ƃ�����
	// double�ɕϊ����Ĕ�r����
	CComVariant vaValueTmp;
	if( SUCCEEDED( vaValueTmp.ChangeType( VT_R8, &vaValue ) ) )
		emType = ( first <= vaValueTmp.dblVal && vaValueTmp.dblVal <= second ) ? sindyeEditableNoEditable : sindyeEditableEditable;

	return emType;
}

bool CAttributeRistrictValues::_SetRistrict( LPCTSTR lpcszRistrict )
{
	bool bRet = true;	// �Ԃ�l

	CString strRistrict( lpcszRistrict );
	int nLen = strRistrict.GetLength();
	// ���҂��镶����́u0�v�Ƃ��u1,2,3�v�Ƃ�

	// �u,�v��T��
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

	// ���l��double�ɕϊ�����
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

	// NULL���ǂ������f
	if( 0 == strValue.CompareNoCase( _T("NULL") ) )
	{
		VARIANT vaValue;
		VariantInit( &vaValue );
		vaValue.vt = VT_NULL;
		push_back( vaValue );
	}

	// strValue�������񂩐��l���𔻒f
	if( _T('\'') == strValue[0] &&
		_T('\'') == strValue[strValue.GetLength()-1] )
	{
		// �����Ƃ��ď���
		strValue.Remove( _T('\'') );	// �u'�v������
		push_back( (LPCTSTR)strValue );
	}
	else {
		// ���l�Ƃ��ď���
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
	bool bRet = true; // �߂�l

	CString strRistrict( lpcszRistrict );
	strRistrict.Trim();

	// �ǂ̃^�C�v���`�F�b�N����
	int nLen = strRistrict.GetLength();
	boost::shared_ptr<CAttributeRistrictBase> spRistrict;

	int nPosMinus = -1;
	while( nPosMinus + 1 < nLen && 0 < ( nPosMinus = strRistrict.Find( _T('-'), nPosMinus + 1 ) ) )
	{
		// ��O���u=�v�Ȃ������x
		if( _T('=') == strRistrict[nPosMinus-1] ) continue;

		// ���낪���l���́u-�v�Ȃ�CCAttributeRistrictMinMax
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

	// ��������������Z�b�g
	bRet = spRistrict->SetRistrict( strRistrict );

	if( bRet )
		m_listRistrict.push_back( spRistrict );
	
	return bRet;
}

bool CAttributeRistrictsAnd::SetRistrict( LPCTSTR lpcszRistrict )
{
	bool bRet = true; // �߂�l

	CString strRistrict( lpcszRistrict );
	// �x���̏���
	if( 0 == strRistrict.Right( 2 ).CompareNoCase( _T(":W") ) )
	{
		m_bWarning = true;
		strRistrict = strRistrict.Left( strRistrict.GetLength() - 2 );
	}
	else
		m_bWarning = false;

	// ����
	int iStart = 0;
	CString strToken = strRistrict.Tokenize( _T("|"), iStart );
	while( ! strToken.IsEmpty() )
	{
		bRet = CAttributeRistricts::AddRistrict( strToken );
		strToken = strRistrict.Tokenize( _T("|"), iStart ); // ��
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
		CString strErrMsgTmp; // �G���[���b�Z�[�W�ꎞ�i�[�p

		sindyeEditableType iRetTmp = (*it)->IsEditable( cRow, 1023, strErrMsgTmp.GetBuffer( 1024 ) );
		strErrMsgTmp.ReleaseBuffer();

		if( ! strErrMsgTmp.IsEmpty() )
			strErrMessage += strErrMsgTmp;

		if( sindyeEditableNoDefine == iRet )
			iRet = iRetTmp;
		else if( iRet != iRetTmp )
		{
			iRet = max( iRet, iRetTmp );

			// ����sindyeEditableEditable�Ȃ�AstrErrMessage�͋�ɂ���
			if( sindyeEditableEditable == iRet )
				strErrMessage.Empty();

			break;
		}
	}

	// �G���[���b�Z�[�W���R�s�[
	if( nLen >= strErrMessage.GetLength() )
		lstrcpyn( lpszMessage, strErrMessage, nLen );
	else {
		ERRORLOG(sindyErr_BufferIsNotEnough);
		SASSERT( nLen >= strErrMessage.GetLength() );
	}

	// iRet���G���[�̏ꍇ�A�x���t���O������̂Ȃ�
	// �x���ɕύX����
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
	bool bRet = false;	// �Ԃ�l

	CString strRistrict( lpcszRistrict );
	strRistrict.Trim();
	if( ! strRistrict.IsEmpty() )
	{
		// �c�_�u|�v������ꍇ��CAttributeRistrictsAnd�Ɂibug 4521�j
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
		CString strErrMsgTmp; // �G���[���b�Z�[�W�ꎞ�i�[�p

		sindyeEditableType iRetTmp = (*it)->IsEditable( cRow, 1023, strErrMsgTmp.GetBuffer( 1024 ) );
		strErrMsgTmp.ReleaseBuffer();

		if( ! strErrMsgTmp.IsEmpty() )
			strErrMessage += strErrMsgTmp;

		if( iRet >= sindyeEditableNoEditable && iRet > iRetTmp )
			iRet = iRetTmp;
		else if( iRet < sindyeEditableNoEditable )
			iRet = iRetTmp;
	}

	// �G���[���b�Z�[�W���R�s�[
	if( nLen >= strErrMessage.GetLength() )
		lstrcpyn( lpszMessage, strErrMessage, nLen );
	else {
		ERRORLOG(sindyErr_BufferIsNotEnough);
		SASSERT( nLen >= strErrMessage.GetLength() );
	}

	return iRet;
}

} // sindy
