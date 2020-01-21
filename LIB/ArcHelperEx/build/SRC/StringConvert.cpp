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

#include "stdafx.h"
#include "StringConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 正しい交差点名称に修正する
CString AheConvertToIntersectionName( LPCTSTR lpcszText, BOOL bIsKanji )
{
	INT nTextLen = lstrlen( lpcszText ) + 1;
	LPTSTR lpszText;

	lpszText = (LPTSTR)_malloca( nTextLen * sizeof(TCHAR) * 2 );	// 2倍の領域を確保
	// 全て全角にしてしまう
	if( bIsKanji )
		LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, lpcszText, nTextLen, lpszText, nTextLen * 2 );
	else
		LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH|LCMAP_KATAKANA, lpcszText, nTextLen, lpszText, nTextLen * 2 );


	CString strText( lpszText );
	CString strRet;
	LONG lLen = CString_GetLength2( strText );
	
	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2( strText, i );
		CT2CA lpstr( str );

		if( // TODO: 複雑化しているため、bIsKanjiでまとめるなど条件分岐方法を修正する
			( issjiswidekatakana( lpstr[0], lpstr[1] ) && !issjiswidesmallke( lpstr[0], lpstr[1] )) ||		// 「ヶ」以外の全角カタカナ
			( bIsKanji == TRUE && issjiskanji(        lpstr[0], lpstr[1] ) ) ||						// 全角漢字（読みでは使用不可）
			( bIsKanji == TRUE && issjiswidehiragana( lpstr[0], lpstr[1] ) ) ||						// 全角ひらがな（読みでは使用不可）
			( bIsKanji == TRUE && issjiswidenumber(   lpstr[0], lpstr[1] ) ) ||						// 「０」～「９」（読みでは使用不可）
			( bIsKanji == TRUE && issjiswidealphabet( lpstr[0], lpstr[1] ) ) ||						// 「ａ」～「Ｚ」（読みでは使用不可）
			// 以下は一番初めにあってはいけない
			( i != 0 && issjiswidechouon(          lpstr[0], lpstr[1] ) ) ||	// 「ー」
			( bIsKanji == TRUE && i != 0 && issjiswidesmallkakko(      lpstr[0], lpstr[1] ) ) ||	// 「（」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidesmalltojikakko(  lpstr[0], lpstr[1] ) ) ||	// 「）」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidenakaguro(        lpstr[0], lpstr[1] ) ) ||	// 「・」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidenun(             lpstr[0], lpstr[1] ) ) ||	// 「々」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswideshime(           lpstr[0], lpstr[1] ) ) ||	// 「〆」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidekanma(           lpstr[0], lpstr[1] ) ) ||	// 「、」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswideminus(           lpstr[0], lpstr[1] ) ) ||	// 「－」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidesmallke(         lpstr[0], lpstr[1] ) ) ||	// 「ヶ」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswideperiod(          lpstr[0], lpstr[1] ) ) ||	// 「．」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidenyoro(           lpstr[0], lpstr[1] ) ) ) 	// 「～」（読みでは使用不可）
		{
			// 名称の場合
			if( bIsKanji ) 
			{
				// 長音記号は全角カタカナの後限定
				if( issjiswidechouon( lpstr[0], lpstr[1] ) )
				{
					CString strPrev = CString_GetAt2( strText, i-1 );
					CT2CA lpstrPrev(strPrev);
					// ひらがなのあとに長音記号がくることもあるらしい（福太郎さん談）2003/05/15
					if( issjiswidekatakana( lpstrPrev[0], lpstrPrev[1] ) != TRUE && issjiswidehiragana(lpstrPrev[0], lpstrPrev[1] ) != TRUE )
						continue;
				}
				// [Bug3104]に伴いマイナスは数字の後ろでなくてよい
				// ただ、記号のあとの場合はやめておく
				// 全角マイナスは数字の後限定
				else if( issjiswideminus( lpstr[0], lpstr[1] ) )
				{
					CString strPrev = CString_GetAt2( strText, i-1 );
					CT2CA lpstrPrev(strPrev);
					if( issjiswidekatakana( lpstrPrev[0], lpstrPrev[1] ) != TRUE && issjiskanji( lpstrPrev[0], lpstrPrev[1] ) != TRUE &&
						issjiswidehiragana( lpstrPrev[0], lpstrPrev[1] ) != TRUE && issjiswidenumber( lpstrPrev[0], lpstrPrev[1] ) != TRUE &&
						issjiswidealphabet( lpstrPrev[0], lpstrPrev[1] ) != TRUE)
						continue;
				}
			}
			// 読みの場合
			else {
				// 長音記号は全角カタカナの後限定
				if( issjiswidechouon( lpstr[0], lpstr[1] ) )
				{
					CString strPrev = CString_GetAt2( strText, i-1 );
					CT2CA lpstrPrev(strPrev);
					if( issjiswidekatakana( lpstrPrev[0], lpstrPrev[1] ) != TRUE )
						continue;
				}
			}
			// 文字追加
			strRet += CString( lpstr );
		}
	}
	_freea( lpszText );

	return strRet;
}
// 正しい方面案内名称に修正する
CString AheConvertToDirguideName( LPCTSTR lpcszText, BOOL bIsKanji )
{
	INT nTextLen = lstrlen( lpcszText ) + 1;
	LPTSTR lpszFullWidthText = new TCHAR[ nTextLen * 2 ];	// 2倍の領域を確保
	// 全て全角にしてしまう
	if( bIsKanji == TRUE )
		LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, lpcszText, nTextLen, lpszFullWidthText, nTextLen * 2 );
	else
		LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH|LCMAP_KATAKANA, lpcszText, nTextLen, lpszFullWidthText, nTextLen * 2 );

	CString strText( lpszFullWidthText );
	CString strRet;
	LONG lLen = CString_GetLength2( strText );

	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2( strText, i );
		CT2CA lpstr(str);

		if( ( bIsKanji && issjiskanji(        lpstr[0], lpstr[1] ) ) ||				// 全角漢字（読みでは使用不可）
			( bIsKanji && issjiswidehiragana( lpstr[0], lpstr[1] ) ) ||				// 全角ひらがな（読みでは使用不可）
			( issjiswidekatakana( lpstr[0], lpstr[1] ) && !issjiswidesmallke( lpstr[0], lpstr[1] ) ) ||	// 全角カタカナ
			( bIsKanji && issjiswidenumber(   lpstr[0], lpstr[1] ) ) ||				// 「０」～「９」（読みでは使用不可）
			( bIsKanji && issjiswidealphabet( lpstr[0], lpstr[1] ) ) ||				// 「ａ」～「Ｚ」（読みでは使用不可）
			( bIsKanji && issjiswideshime(  lpstr[0], lpstr[1] ) ) ||				// 「〆」（読みでは使用不可）先頭文字でも可(bug7282)
			// 以下は一番初めにあってはいけない
			( i != 0 && issjiswideperiod( lpstr[0], lpstr[1] ) ) ||					// 「．」
			( i != 0 && issjiswidenyoro(  lpstr[0], lpstr[1] ) ) ||					// 「～」
			( bIsKanji && i != 0 && issjiswidenun(    lpstr[0], lpstr[1] ) ) ||		// 「々」（読みでは使用不可）
			( i != 0 && issjiswidenakaguro(        lpstr[0], lpstr[1] ) ) ||		// 「・」
			( i != 0 && issjiswidesmallkakko(      lpstr[0], lpstr[1] ) ) ||		// 「（」
			( i != 0 && issjiswidesmalltojikakko(  lpstr[0], lpstr[1] ) ) ||		// 「）」
			( i != 0 && issjiswidechouon(          lpstr[0], lpstr[1] ) ) ||		// 「ー」
			( i != 0 && issjiswidesmallke(          lpstr[0], lpstr[1] ) )||		// 「ヶ」
			( i != 0 && issjiswidekanma(           lpstr[0], lpstr[1] ) ) ||		// 「、」
			( i != 0 && issjiswideminus(           lpstr[0], lpstr[1] ) ) )			// 「－」
		{
			// 長音記号は全角カタカナ・全角ひらがなの後限定
			if( issjiswidechouon( lpstr[0], lpstr[1] ) )
			{
				CString strPrev = CString_GetAt2( strText, i-1 );
				CT2CA lpstrPrev( strPrev );
				if( issjiswidekatakana( lpstrPrev[0], lpstrPrev[1] ) != TRUE && issjiswidehiragana( lpstrPrev[0], lpstrPrev[1] ) != TRUE )
					continue;
			}
			strRet += CString( lpstr );
		}
	}
	delete [] lpszFullWidthText;

	return strRet;
}
// 正しいアクセスポイントの方面名称に修正する
CString AheConvertToAccessPointName( LPCTSTR lpcszText, BOOL bIsKanji )
{
	INT nTextLen = lstrlen( lpcszText ) + 1;
	LPTSTR lpszFullWidthText = new TCHAR[ nTextLen * 2 ];	// 2倍の領域を確保
	// 全て全角にしてしまう
	LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, lpcszText, nTextLen, lpszFullWidthText, nTextLen * 2 );

	CString strText( lpszFullWidthText );
	CString strRet;
	LONG lLen = CString_GetLength2( strText );

	USES_CONVERSION;

	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2( strText, i );
		CT2CA lpstr(str);

		if( ( bIsKanji == TRUE && issjiskanji(        lpstr[0], lpstr[1] ) ) ||			// 全角漢字（読みでは使用不可）
			( bIsKanji == TRUE && issjiswidehiragana( lpstr[0], lpstr[1] ) ) ||			// 全角ひらがな（読みでは使用不可）
			issjiswidekatakana( lpstr[0], lpstr[1] ) ||									// 全角カタカナ
			( bIsKanji == TRUE && issjiswidenumber(   lpstr[0], lpstr[1] ) ) ||			// 「０」～「９」（読みでは使用不可）
			( bIsKanji == TRUE && issjiswidealphabet( lpstr[0], lpstr[1] ) ) ||			// 「ａ」～「Ｚ」（読みでは使用不可）
			// 以下は一番初めにあってはいけない
			( bIsKanji == TRUE && i != 0 && issjiswideperiod( lpstr[0], lpstr[1] ) ) ||	// 「．」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidenyoro(  lpstr[0], lpstr[1] ) ) ||	// 「～」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidenun(    lpstr[0], lpstr[1] ) ) ||	// 「々」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswideshime(  lpstr[0], lpstr[1] ) ) ||	// 「〆」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidesmallkakko(      lpstr[0], lpstr[1] ) ) || // 「（」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidesmalltojikakko(  lpstr[0], lpstr[1] ) ) || // 「）」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidekanma(           lpstr[0], lpstr[1] ) ) || // 「、」（読みでは使用不可）
			( i != 0 && issjiswidechouon(          lpstr[0], lpstr[1] ) ) ||			// 「ー」
			( i != 0 && issjiswidenakaguro(        lpstr[0], lpstr[1] ) ) )				// 「・」
		{
			// 長音記号は全角カタカナの後限定
			if( issjiswidechouon( lpstr[0], lpstr[1] ) )
			{
				CString strPrev = CString_GetAt2( strText, i-1 );
				CT2CA lpstrPrev( strPrev );
				if( issjiswidekatakana( lpstrPrev[0], lpstrPrev[1] ) != TRUE )
					continue;
			}
			strRet += CString( lpstr );
		}
	}

	delete [] lpszFullWidthText;

	return strRet;
}
// 正しい施設情報の方面名称に修正する
CString AheConvertToFacilInfoName( LPCTSTR lpcszText, BOOL bIsKanji )
{
	INT nTextLen = lstrlen( lpcszText ) + 1;
	LPTSTR lpszFullWidthText = new TCHAR[ nTextLen * 2 ];	// 2倍の領域を確保
	// 全て全角にしてしまう
	LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, lpcszText, nTextLen, lpszFullWidthText, nTextLen * 2 );

	CString strText( lpszFullWidthText );
	CString strRet;
	LONG lLen = CString_GetLength2( strText );

	USES_CONVERSION;

	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2( strText, i );
		CT2CA lpstr(str);

		if( ( bIsKanji == TRUE && issjiskanji(        lpstr[0], lpstr[1] ) ) ||			// 全角漢字（読みでは使用不可）
			( bIsKanji == TRUE && issjiswidehiragana( lpstr[0], lpstr[1] ) ) ||			// 全角ひらがな（読みでは使用不可）
			issjiswidekatakana( lpstr[0], lpstr[1] ) ||									// 全角カタカナ
			( bIsKanji == TRUE && issjiswidenumber(   lpstr[0], lpstr[1] ) ) ||			// 「０」～「９」（読みでは使用不可）
			( bIsKanji == TRUE && issjiswidealphabet( lpstr[0], lpstr[1] ) ) ||			// 「ａ」～「Ｚ」（読みでは使用不可）
			// 以下は一番初めにあってはいけない
			( bIsKanji == TRUE && i != 0 && issjiswideperiod( lpstr[0], lpstr[1] ) ) ||	// 「．」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidenyoro(  lpstr[0], lpstr[1] ) ) ||	// 「～」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidenun(    lpstr[0], lpstr[1] ) ) ||	// 「々」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswideshime(  lpstr[0], lpstr[1] ) ) ||	// 「〆」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidesmallkakko(      lpstr[0], lpstr[1] ) ) || // 「（」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidesmalltojikakko(  lpstr[0], lpstr[1] ) ) || // 「）」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidekanma(           lpstr[0], lpstr[1] ) ) || // 「、」（読みでは使用不可）
			( bIsKanji == TRUE && i != 0 && issjiswidenakaguro(        lpstr[0], lpstr[1] ) ) || // 「・」（読みでは使用不可）
			( i != 0 && issjiswidechouon(          lpstr[0], lpstr[1] ) ) )				// 「ー」
		{
			// 長音記号は全角カタカナの後限定
			if( issjiswidechouon( lpstr[0], lpstr[1] ) )
			{
				CString strPrev = CString_GetAt2( strText, i-1 );
				CT2CA lpstrPrev( strPrev );
				if( ! issjiswidekatakana( lpstrPrev[0], lpstrPrev[1] ) )
					continue;
			}
			strRet += CString( lpstr );
		}
	}

	delete [] lpszFullWidthText;

	return strRet;
}

//{ ADD 02.11.21 by hkakeno
CString AheConvertToAnnoJustStr( LPCTSTR lpcszText, BOOL bIsFull, BOOL bUseAlphabet, BOOL bUseMinusPeriod /* = FALSE */, CString* pstrErr/* = NULL*/ )
{
	CString strText;
	{
		// bIsFullをみて全角・半角に統一
		INT nTextLen = lstrlen( lpcszText );
		DWORD dwFlag = ( bIsFull ) ? LCMAP_FULLWIDTH : LCMAP_HALFWIDTH;
		// 必要文字数を取得
		int nNewLen = LCMapString( GetUserDefaultLCID(), dwFlag, lpcszText, nTextLen + 1, NULL, 0 );
		LPTSTR lpszText = new TCHAR[ nNewLen ];
		LCMapString( GetUserDefaultLCID(), dwFlag, lpcszText, nTextLen + 1, lpszText, nNewLen );
		strText = lpszText;
		delete [] lpszText;
	}

	CString strRet;
	LONG lLen = CString_GetLength2( strText );

	INT  nOcnt   = 0;		// 出力文字数
	BOOL bMinus  = FALSE;
	BOOL bPeriod = FALSE;
	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2( strText, i );
		CT2A lpstr( str );

		if (bIsFull)	// 全角の場合
		{
			// 全角の時、許されるもの
			if (
			( issjiskanji(              lpstr[0], lpstr[1] ) ) ||	// 全角漢字
			( issjiswidehiragana(       lpstr[0], lpstr[1] ) ) ||	// 全角ひらがな
			( issjiswidekatakana(       lpstr[0], lpstr[1] ) ) ||	// 全角カタカナ
			( issjiswidenumber(         lpstr[0], lpstr[1] ) ) ||	// 「０」～「９」
			( issjiswidealphabet(       lpstr[0], lpstr[1] ) ) ||	// 「ａ」～「Ｚ」
			( issjiswidegreek(          lpstr[0], lpstr[1] ) ) ||	// 全角ギリシア文字
			( issjiswidekigou(          lpstr[0], lpstr[1] ) ) ||	// 全角利用可能特殊記号（その他）
			// 以下は一番初めにあってはいけない？（注記の規則は？）
			( issjiswideperiod(         lpstr[0], lpstr[1] ) ) ||	// 「．」
			( issjiswidenyoro(          lpstr[0], lpstr[1] ) ) ||	// 「～」
			( issjiswidenun(            lpstr[0], lpstr[1] ) ) ||	// 「々」
			( issjiswideshime(          lpstr[0], lpstr[1] ) ) ||	// 「〆」
			( issjiswidenakaguro(       lpstr[0], lpstr[1] ) ) ||	// 「・」
			( issjiswidesmallkakko(     lpstr[0], lpstr[1] ) ) ||	// 「（」
			( issjiswidesmalltojikakko( lpstr[0], lpstr[1] ) ) ||	// 「）」
			( issjiswidechouon(         lpstr[0], lpstr[1] ) ) ||	// 「ー」
			( issjiswidekanma(          lpstr[0], lpstr[1] ) ) ||	// 「、」
			( issjiswideminus(          lpstr[0], lpstr[1] ) ) 		// 「－」
			){
				// [bug 3151] 第三水準の漢字は除く
				if( issjis3kanji( lpstr[0], lpstr[1] ) )
				{
					if( pstrErr ) *pstrErr += CString( lpstr );	// エラー文字は返す
					continue;
				}
				/*	この制限は必要か？
				// 長音記号は全角カタカナの後限定
				if( issjiswidechouon( lpstr[0], lpstr[1] ) )
				{
					CString strPrev = strText.GetAt2(i-1);
					LPSTR lpstrPrev = T2A( (LPTSTR)((LPCTSTR)strPrev) );
					if( issjiswidekatakana( lpstrPrev[0], lpstrPrev[1] ) != TRUE )
						continue;
				}
				// 全角マイナスは数字の後限定
				else if( issjiswideminus( lpstr[0], lpstr[1] ) )
				{
					CString strPrev = strText.GetAt2(i-1);
					LPSTR lpstrPrev = T2A( (LPTSTR)((LPCTSTR)strPrev) );
					if( issjiswidenumber( lpstrPrev[0], lpstrPrev[1] ) != TRUE )
						continue;
				}
				*/
			}
			else {
				if( pstrErr ) *pstrErr += CString( lpstr );	// エラー文字は返す
				continue;
			}
		}
		else					// 半角の場合
		{
			// 半角の時、許されるもの
			if ( isasciinumber( lpstr[0] ) )		// 「0」～「9」
			{
				if( nOcnt == 0 && isasciizero( lpstr[0] ) ) 	// 先頭が「0」はダメ
					continue;
			}
			else if ( bUseMinusPeriod && isasciiminus( lpstr[0] ) && ( ! bMinus ) && nOcnt == 0)		// 「-」一つだけ、しかも先頭のみ
			{
				bMinus = TRUE;
				nOcnt  = -1;	// 「-」以降からカウントするため
			}
			else if ( bUseMinusPeriod && isasciiperiod( lpstr[0] ) && ( ! bPeriod ) )	// 「.」一つだけ
			{
				bPeriod = TRUE;
				if (nOcnt == 0)	// ドットの前に数字がなかったら
				{
					// '0' = 0x30を強制的に追加
					strRet += _T("0");
				}
			}
			else if ( bUseAlphabet ) {
				if (isasciialphabet_large(lpstr[0])) {
					;
				}
				if (isasciialphabet_small(lpstr[0])) {
					lpstr[0] = lpstr[0] - 0x20;
				}
			}
			else
				continue;
		}

		strRet += CString( lpstr );
		nOcnt++;
	}

	// 結果が「-」のみになった時どうするか（半角の場合のみのはず）
	if (strRet.Compare(_T("-")) == 0)	strRet.Empty();

	return strRet;
}
//} ADD 02.11.21 by hkakeno


CString AheConvertToGenkouNoStr( LPCTSTR lpcszText )
{
	CString strText;
	{
		INT nTextLen = lstrlen( lpcszText ) + 1;
		LPTSTR lpszText = NULL;

		lpszText = (LPTSTR)_malloca( nTextLen * sizeof(TCHAR) );	// 同じ領域を確保
		// 全て半角にしてしまう
		LCMapString( GetUserDefaultLCID(), LCMAP_HALFWIDTH, lpcszText, nTextLen, lpszText, nTextLen );
		strText = lpszText;
		_freea( lpszText );
	}

	CString strRet;
	LONG lLen = CString_GetLength2( strText );

	INT  nOcnt   = 0;		// 出力文字数
	BOOL bMinus  = FALSE;
	BOOL bPeriod = FALSE;
	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2( strText, i );
		CT2A lpstr( str );

		if( lpstr )
		{
			// 半角の時、許されるもの
			if ( isasciinumber( lpstr[0] ) )		// 「0」～「9」
			{
				;
			}
			else if (isasciialphabet_large(lpstr[0]))
			{
				;
			}
			else if (isasciialphabet_small(lpstr[0]))
			{
				lpstr[0] = lpstr[0] - 0x20;
			}
			else
				continue;
		}
		else
			_ASSERTE( lpstr );

		strRet += CString( lpstr );
		nOcnt++;
	}

	return strRet;
}


// 日付に使われる文字列を適正な文字列に修正
CString AheConvertToDateStr( LPCTSTR lpcszText )
{
	CString strText;
	{
		INT nTextLen = lstrlen( lpcszText ) + 1;
		LPTSTR lpszText = NULL;

		lpszText = (LPTSTR)_malloca( nTextLen * sizeof(TCHAR) );	// 同じ領域を確保
		// 全て半角にしてしまう
		LCMapString( GetUserDefaultLCID(), LCMAP_HALFWIDTH, lpcszText, nTextLen, lpszText, nTextLen );
		strText = lpszText;
		_freea( lpszText );
	}

	CString strRet;
	LONG lLen = CString_GetLength2( strText );

	INT  nOcnt   = 0;		// 出力文字数
	// 一文字ずつ確認し、使用可能文字のみ保持する
	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2( strText, i );
		CT2A lpstr( str );

		if( lpstr )
		{
			// 半角の「0」～「9」と「-」以外は格納しない
			if ( !isasciinumber( lpstr[0] ) && !isasciislash(lpstr[0]) )
				continue;
		}
		else
			_ASSERTE( lpstr );

		strRet += CString( lpstr );
		nOcnt++;
	}

	return strRet;
}
