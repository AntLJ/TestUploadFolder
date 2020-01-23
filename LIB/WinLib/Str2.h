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
 * @file Str2.h
 * @brief <b>CString�g���N���X</b>\n
 * ���̃t�@�C���ɂ́ACString�N���X�̊g������`����Ă��܂��B
 * �ʏ�MFC�y��ATL��CString�N���X�́AMBCS�Ȏ��ɁAGetLength�A
 * GetAt�֐��ŏ��1�o�C�g�P�ʂŌ��ʂ�Ԃ��܂��B
 * �������A����ł͓s�����������Ƃ������̂ŁA�O���֐��Ƃ���
 * �V���ɒ�`���Ă��܂��B
 * Str.h���番�������̂́AMFC��CString�N���X�ɂ��K�p�ł���
 * �悤�ɂ��邽�߂ł��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _STR2_H_
#define _STR2_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief <b>CString::GetLength()�̓�o�C�g�l����</b>\n
 * �ʏ�MFC�y��ATL��CString�N���X�́AMBCS�Ȏ��ɁAGetLength
 * �֐��ŏ��1�o�C�g�P�ʂŌ��ʂ�Ԃ��܂��B
 * �������A����ł͓s�����������Ƃ������̂ł��̏ꍇ�͂���
 * �֐����g�p���Ă��������B
 * Str.h���番�������̂́AMFC��CString�N���X�ɂ��K�p�ł���
 * �悤�ɂ��邽�߂ł��B
 * @param str	[in]	CString
 * @return �����񒷁i��o�C�g�R�[�h�l���j
 */
inline int CString_GetLength2( const CString& str )
{
#ifdef _UNICODE
	return str.GetLength();
#else
	INT n = 0;
	for( int i = 0; i < str.GetLength(); i++ )
	{
		if( issjiswide( str.GetAt(i), str.GetAt(i+1) ) )
			i++;
		n++;
	}
	return n;
#endif	// ifdef _UNICODE
}

/**
 * @brief <b>::CString::GetAt()�̓�o�C�g�l����</b>\n
 * �ʏ�MFC�y��ATL��CString�N���X�́AMBCS�Ȏ��ɁAGetAt
 * �֐��ŏ��1�o�C�g�P�ʂŌ��ʂ�Ԃ��܂��B
 * �������A����ł͓s�����������Ƃ������̂ł��̏ꍇ�͂���
 * �֐����g�p���Ă��������B
 * Str.h���番�������̂́AMFC��CString�N���X�ɂ��K�p�ł���
 * �悤�ɂ��邽�߂ł��B
 * @param str		[in]	CString
 * @param nIndex	[in]	�C���f�b�N�X�ԍ�
 * @return CString
 */
inline CString CString_GetAt2( const CString& str, int nIndex )
{
#ifdef _UNICODE
	return (CString)str.GetAt(nIndex);
#else
	INT n = 0;
	CString strTmp;
		
	for( int i = 0; i < str.GetLength(); i++ )
	{
		bool flg = false;
		if( n == nIndex )
		{
			strTmp += str.GetAt(i);
			flg = true;
		}

		if( issjiswide( str.GetAt(i), str.GetAt(i+1) ) )
		{
			if( n == nIndex )
				strTmp += str.GetAt(i+1);

			i++;
		}

		if( flg == true ) 
			break;

		n++;
	}

	return strTmp;
#endif // ifdef _UNICODE
}

/**
 * @brief <b>�����񒆂̍Ō�́u.�v�������Ԃ�</b>\n
 * ���Ƃ��΁A�I�[�i�[���t���̃e�[�u��������e�[�u�����̂ݎ��o���ꍇ�Ɏg�p���Ă��������B
 * �u.�v��������Ȃ��ꍇ�͂��̂܂܂̕������Ԃ��܂��B
 * @return CString
 */
inline CString CString_LastPeriodRight( const CString& str )
{
	int nPos = str.ReverseFind( _T('.') );
	if( nPos >= 0 )
		return str.Right( str.GetLength() - nPos - 1 );
	else
		return str;
}

/**
 * @brief <b>�����񒆂̍Ō�́u.�v���O��Ԃ�</b>\n
 * ���Ƃ��΁A�I�[�i�[���t���̃e�[�u��������I�[�i�[���̂ݎ��o���ꍇ�Ɏg�p���Ă��������B
 * �u.�v��������Ȃ��ꍇ�͋󕶎����Ԃ��܂��B
 * @return CString
 */
inline CString CString_LastPeriodLeft( const CString& str )
{
	int nPos = str.Find( _T('.') );
	if( nPos >= 0 )
		return str.Left( nPos );
	else
		return CString();
}

#endif // _STR2_H_
