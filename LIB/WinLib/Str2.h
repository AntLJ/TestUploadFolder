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
 * @brief <b>CString拡張クラス</b>\n
 * このファイルには、CStringクラスの拡張が定義されています。
 * 通常MFC及びATLのCStringクラスは、MBCSな時に、GetLength、
 * GetAt関数で常に1バイト単位で結果を返します。
 * しかし、それでは都合が悪いことが多いので、外部関数として
 * 新たに定義しています。
 * Str.hから分離したのは、MFCのCStringクラスにも適用できる
 * ようにするためです。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _STR2_H_
#define _STR2_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief <b>CString::GetLength()の二バイト考慮版</b>\n
 * 通常MFC及びATLのCStringクラスは、MBCSな時に、GetLength
 * 関数で常に1バイト単位で結果を返します。
 * しかし、それでは都合が悪いことが多いのでその場合はこの
 * 関数を使用してください。
 * Str.hから分離したのは、MFCのCStringクラスにも適用できる
 * ようにするためです。
 * @param str	[in]	CString
 * @return 文字列長（二バイトコード考慮）
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
 * @brief <b>::CString::GetAt()の二バイト考慮版</b>\n
 * 通常MFC及びATLのCStringクラスは、MBCSな時に、GetAt
 * 関数で常に1バイト単位で結果を返します。
 * しかし、それでは都合が悪いことが多いのでその場合はこの
 * 関数を使用してください。
 * Str.hから分離したのは、MFCのCStringクラスにも適用できる
 * ようにするためです。
 * @param str		[in]	CString
 * @param nIndex	[in]	インデックス番号
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
 * @brief <b>文字列中の最後の「.」から後ろを返す</b>\n
 * たとえば、オーナー名付きのテーブル名からテーブル名のみ取り出す場合に使用してください。
 * 「.」が見つからない場合はそのままの文字列を返します。
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
 * @brief <b>文字列中の最後の「.」より前を返す</b>\n
 * たとえば、オーナー名付きのテーブル名からオーナー名のみ取り出す場合に使用してください。
 * 「.」が見つからない場合は空文字列を返します。
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
