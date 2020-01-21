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

// �����������_���̂ɏC������
CString AheConvertToIntersectionName( LPCTSTR lpcszText, BOOL bIsKanji )
{
	INT nTextLen = lstrlen( lpcszText ) + 1;
	LPTSTR lpszText;

	lpszText = (LPTSTR)_malloca( nTextLen * sizeof(TCHAR) * 2 );	// 2�{�̗̈���m��
	// �S�đS�p�ɂ��Ă��܂�
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

		if( // TODO: ���G�����Ă��邽�߁AbIsKanji�ł܂Ƃ߂�ȂǏ���������@���C������
			( issjiswidekatakana( lpstr[0], lpstr[1] ) && !issjiswidesmallke( lpstr[0], lpstr[1] )) ||		// �u���v�ȊO�̑S�p�J�^�J�i
			( bIsKanji == TRUE && issjiskanji(        lpstr[0], lpstr[1] ) ) ||						// �S�p�����i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && issjiswidehiragana( lpstr[0], lpstr[1] ) ) ||						// �S�p�Ђ炪�ȁi�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && issjiswidenumber(   lpstr[0], lpstr[1] ) ) ||						// �u�O�v�`�u�X�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && issjiswidealphabet( lpstr[0], lpstr[1] ) ) ||						// �u���v�`�u�y�v�i�ǂ݂ł͎g�p�s�j
			// �ȉ��͈�ԏ��߂ɂ����Ă͂����Ȃ�
			( i != 0 && issjiswidechouon(          lpstr[0], lpstr[1] ) ) ||	// �u�[�v
			( bIsKanji == TRUE && i != 0 && issjiswidesmallkakko(      lpstr[0], lpstr[1] ) ) ||	// �u�i�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidesmalltojikakko(  lpstr[0], lpstr[1] ) ) ||	// �u�j�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidenakaguro(        lpstr[0], lpstr[1] ) ) ||	// �u�E�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidenun(             lpstr[0], lpstr[1] ) ) ||	// �u�X�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswideshime(           lpstr[0], lpstr[1] ) ) ||	// �u�Y�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidekanma(           lpstr[0], lpstr[1] ) ) ||	// �u�A�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswideminus(           lpstr[0], lpstr[1] ) ) ||	// �u�|�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidesmallke(         lpstr[0], lpstr[1] ) ) ||	// �u���v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswideperiod(          lpstr[0], lpstr[1] ) ) ||	// �u�D�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidenyoro(           lpstr[0], lpstr[1] ) ) ) 	// �u�`�v�i�ǂ݂ł͎g�p�s�j
		{
			// ���̂̏ꍇ
			if( bIsKanji ) 
			{
				// �����L���͑S�p�J�^�J�i�̌����
				if( issjiswidechouon( lpstr[0], lpstr[1] ) )
				{
					CString strPrev = CString_GetAt2( strText, i-1 );
					CT2CA lpstrPrev(strPrev);
					// �Ђ炪�Ȃ̂��Ƃɒ����L�������邱�Ƃ�����炵���i�����Y����k�j2003/05/15
					if( issjiswidekatakana( lpstrPrev[0], lpstrPrev[1] ) != TRUE && issjiswidehiragana(lpstrPrev[0], lpstrPrev[1] ) != TRUE )
						continue;
				}
				// [Bug3104]�ɔ����}�C�i�X�͐����̌��łȂ��Ă悢
				// �����A�L���̂��Ƃ̏ꍇ�͂�߂Ă���
				// �S�p�}�C�i�X�͐����̌����
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
			// �ǂ݂̏ꍇ
			else {
				// �����L���͑S�p�J�^�J�i�̌����
				if( issjiswidechouon( lpstr[0], lpstr[1] ) )
				{
					CString strPrev = CString_GetAt2( strText, i-1 );
					CT2CA lpstrPrev(strPrev);
					if( issjiswidekatakana( lpstrPrev[0], lpstrPrev[1] ) != TRUE )
						continue;
				}
			}
			// �����ǉ�
			strRet += CString( lpstr );
		}
	}
	_freea( lpszText );

	return strRet;
}
// ���������ʈē����̂ɏC������
CString AheConvertToDirguideName( LPCTSTR lpcszText, BOOL bIsKanji )
{
	INT nTextLen = lstrlen( lpcszText ) + 1;
	LPTSTR lpszFullWidthText = new TCHAR[ nTextLen * 2 ];	// 2�{�̗̈���m��
	// �S�đS�p�ɂ��Ă��܂�
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

		if( ( bIsKanji && issjiskanji(        lpstr[0], lpstr[1] ) ) ||				// �S�p�����i�ǂ݂ł͎g�p�s�j
			( bIsKanji && issjiswidehiragana( lpstr[0], lpstr[1] ) ) ||				// �S�p�Ђ炪�ȁi�ǂ݂ł͎g�p�s�j
			( issjiswidekatakana( lpstr[0], lpstr[1] ) && !issjiswidesmallke( lpstr[0], lpstr[1] ) ) ||	// �S�p�J�^�J�i
			( bIsKanji && issjiswidenumber(   lpstr[0], lpstr[1] ) ) ||				// �u�O�v�`�u�X�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji && issjiswidealphabet( lpstr[0], lpstr[1] ) ) ||				// �u���v�`�u�y�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji && issjiswideshime(  lpstr[0], lpstr[1] ) ) ||				// �u�Y�v�i�ǂ݂ł͎g�p�s�j�擪�����ł���(bug7282)
			// �ȉ��͈�ԏ��߂ɂ����Ă͂����Ȃ�
			( i != 0 && issjiswideperiod( lpstr[0], lpstr[1] ) ) ||					// �u�D�v
			( i != 0 && issjiswidenyoro(  lpstr[0], lpstr[1] ) ) ||					// �u�`�v
			( bIsKanji && i != 0 && issjiswidenun(    lpstr[0], lpstr[1] ) ) ||		// �u�X�v�i�ǂ݂ł͎g�p�s�j
			( i != 0 && issjiswidenakaguro(        lpstr[0], lpstr[1] ) ) ||		// �u�E�v
			( i != 0 && issjiswidesmallkakko(      lpstr[0], lpstr[1] ) ) ||		// �u�i�v
			( i != 0 && issjiswidesmalltojikakko(  lpstr[0], lpstr[1] ) ) ||		// �u�j�v
			( i != 0 && issjiswidechouon(          lpstr[0], lpstr[1] ) ) ||		// �u�[�v
			( i != 0 && issjiswidesmallke(          lpstr[0], lpstr[1] ) )||		// �u���v
			( i != 0 && issjiswidekanma(           lpstr[0], lpstr[1] ) ) ||		// �u�A�v
			( i != 0 && issjiswideminus(           lpstr[0], lpstr[1] ) ) )			// �u�|�v
		{
			// �����L���͑S�p�J�^�J�i�E�S�p�Ђ炪�Ȃ̌����
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
// �������A�N�Z�X�|�C���g�̕��ʖ��̂ɏC������
CString AheConvertToAccessPointName( LPCTSTR lpcszText, BOOL bIsKanji )
{
	INT nTextLen = lstrlen( lpcszText ) + 1;
	LPTSTR lpszFullWidthText = new TCHAR[ nTextLen * 2 ];	// 2�{�̗̈���m��
	// �S�đS�p�ɂ��Ă��܂�
	LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, lpcszText, nTextLen, lpszFullWidthText, nTextLen * 2 );

	CString strText( lpszFullWidthText );
	CString strRet;
	LONG lLen = CString_GetLength2( strText );

	USES_CONVERSION;

	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2( strText, i );
		CT2CA lpstr(str);

		if( ( bIsKanji == TRUE && issjiskanji(        lpstr[0], lpstr[1] ) ) ||			// �S�p�����i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && issjiswidehiragana( lpstr[0], lpstr[1] ) ) ||			// �S�p�Ђ炪�ȁi�ǂ݂ł͎g�p�s�j
			issjiswidekatakana( lpstr[0], lpstr[1] ) ||									// �S�p�J�^�J�i
			( bIsKanji == TRUE && issjiswidenumber(   lpstr[0], lpstr[1] ) ) ||			// �u�O�v�`�u�X�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && issjiswidealphabet( lpstr[0], lpstr[1] ) ) ||			// �u���v�`�u�y�v�i�ǂ݂ł͎g�p�s�j
			// �ȉ��͈�ԏ��߂ɂ����Ă͂����Ȃ�
			( bIsKanji == TRUE && i != 0 && issjiswideperiod( lpstr[0], lpstr[1] ) ) ||	// �u�D�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidenyoro(  lpstr[0], lpstr[1] ) ) ||	// �u�`�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidenun(    lpstr[0], lpstr[1] ) ) ||	// �u�X�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswideshime(  lpstr[0], lpstr[1] ) ) ||	// �u�Y�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidesmallkakko(      lpstr[0], lpstr[1] ) ) || // �u�i�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidesmalltojikakko(  lpstr[0], lpstr[1] ) ) || // �u�j�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidekanma(           lpstr[0], lpstr[1] ) ) || // �u�A�v�i�ǂ݂ł͎g�p�s�j
			( i != 0 && issjiswidechouon(          lpstr[0], lpstr[1] ) ) ||			// �u�[�v
			( i != 0 && issjiswidenakaguro(        lpstr[0], lpstr[1] ) ) )				// �u�E�v
		{
			// �����L���͑S�p�J�^�J�i�̌����
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
// �������{�ݏ��̕��ʖ��̂ɏC������
CString AheConvertToFacilInfoName( LPCTSTR lpcszText, BOOL bIsKanji )
{
	INT nTextLen = lstrlen( lpcszText ) + 1;
	LPTSTR lpszFullWidthText = new TCHAR[ nTextLen * 2 ];	// 2�{�̗̈���m��
	// �S�đS�p�ɂ��Ă��܂�
	LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, lpcszText, nTextLen, lpszFullWidthText, nTextLen * 2 );

	CString strText( lpszFullWidthText );
	CString strRet;
	LONG lLen = CString_GetLength2( strText );

	USES_CONVERSION;

	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2( strText, i );
		CT2CA lpstr(str);

		if( ( bIsKanji == TRUE && issjiskanji(        lpstr[0], lpstr[1] ) ) ||			// �S�p�����i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && issjiswidehiragana( lpstr[0], lpstr[1] ) ) ||			// �S�p�Ђ炪�ȁi�ǂ݂ł͎g�p�s�j
			issjiswidekatakana( lpstr[0], lpstr[1] ) ||									// �S�p�J�^�J�i
			( bIsKanji == TRUE && issjiswidenumber(   lpstr[0], lpstr[1] ) ) ||			// �u�O�v�`�u�X�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && issjiswidealphabet( lpstr[0], lpstr[1] ) ) ||			// �u���v�`�u�y�v�i�ǂ݂ł͎g�p�s�j
			// �ȉ��͈�ԏ��߂ɂ����Ă͂����Ȃ�
			( bIsKanji == TRUE && i != 0 && issjiswideperiod( lpstr[0], lpstr[1] ) ) ||	// �u�D�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidenyoro(  lpstr[0], lpstr[1] ) ) ||	// �u�`�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidenun(    lpstr[0], lpstr[1] ) ) ||	// �u�X�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswideshime(  lpstr[0], lpstr[1] ) ) ||	// �u�Y�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidesmallkakko(      lpstr[0], lpstr[1] ) ) || // �u�i�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidesmalltojikakko(  lpstr[0], lpstr[1] ) ) || // �u�j�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidekanma(           lpstr[0], lpstr[1] ) ) || // �u�A�v�i�ǂ݂ł͎g�p�s�j
			( bIsKanji == TRUE && i != 0 && issjiswidenakaguro(        lpstr[0], lpstr[1] ) ) || // �u�E�v�i�ǂ݂ł͎g�p�s�j
			( i != 0 && issjiswidechouon(          lpstr[0], lpstr[1] ) ) )				// �u�[�v
		{
			// �����L���͑S�p�J�^�J�i�̌����
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
		// bIsFull���݂đS�p�E���p�ɓ���
		INT nTextLen = lstrlen( lpcszText );
		DWORD dwFlag = ( bIsFull ) ? LCMAP_FULLWIDTH : LCMAP_HALFWIDTH;
		// �K�v���������擾
		int nNewLen = LCMapString( GetUserDefaultLCID(), dwFlag, lpcszText, nTextLen + 1, NULL, 0 );
		LPTSTR lpszText = new TCHAR[ nNewLen ];
		LCMapString( GetUserDefaultLCID(), dwFlag, lpcszText, nTextLen + 1, lpszText, nNewLen );
		strText = lpszText;
		delete [] lpszText;
	}

	CString strRet;
	LONG lLen = CString_GetLength2( strText );

	INT  nOcnt   = 0;		// �o�͕�����
	BOOL bMinus  = FALSE;
	BOOL bPeriod = FALSE;
	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2( strText, i );
		CT2A lpstr( str );

		if (bIsFull)	// �S�p�̏ꍇ
		{
			// �S�p�̎��A����������
			if (
			( issjiskanji(              lpstr[0], lpstr[1] ) ) ||	// �S�p����
			( issjiswidehiragana(       lpstr[0], lpstr[1] ) ) ||	// �S�p�Ђ炪��
			( issjiswidekatakana(       lpstr[0], lpstr[1] ) ) ||	// �S�p�J�^�J�i
			( issjiswidenumber(         lpstr[0], lpstr[1] ) ) ||	// �u�O�v�`�u�X�v
			( issjiswidealphabet(       lpstr[0], lpstr[1] ) ) ||	// �u���v�`�u�y�v
			( issjiswidegreek(          lpstr[0], lpstr[1] ) ) ||	// �S�p�M���V�A����
			( issjiswidekigou(          lpstr[0], lpstr[1] ) ) ||	// �S�p���p�\����L���i���̑��j
			// �ȉ��͈�ԏ��߂ɂ����Ă͂����Ȃ��H�i���L�̋K���́H�j
			( issjiswideperiod(         lpstr[0], lpstr[1] ) ) ||	// �u�D�v
			( issjiswidenyoro(          lpstr[0], lpstr[1] ) ) ||	// �u�`�v
			( issjiswidenun(            lpstr[0], lpstr[1] ) ) ||	// �u�X�v
			( issjiswideshime(          lpstr[0], lpstr[1] ) ) ||	// �u�Y�v
			( issjiswidenakaguro(       lpstr[0], lpstr[1] ) ) ||	// �u�E�v
			( issjiswidesmallkakko(     lpstr[0], lpstr[1] ) ) ||	// �u�i�v
			( issjiswidesmalltojikakko( lpstr[0], lpstr[1] ) ) ||	// �u�j�v
			( issjiswidechouon(         lpstr[0], lpstr[1] ) ) ||	// �u�[�v
			( issjiswidekanma(          lpstr[0], lpstr[1] ) ) ||	// �u�A�v
			( issjiswideminus(          lpstr[0], lpstr[1] ) ) 		// �u�|�v
			){
				// [bug 3151] ��O�����̊����͏���
				if( issjis3kanji( lpstr[0], lpstr[1] ) )
				{
					if( pstrErr ) *pstrErr += CString( lpstr );	// �G���[�����͕Ԃ�
					continue;
				}
				/*	���̐����͕K�v���H
				// �����L���͑S�p�J�^�J�i�̌����
				if( issjiswidechouon( lpstr[0], lpstr[1] ) )
				{
					CString strPrev = strText.GetAt2(i-1);
					LPSTR lpstrPrev = T2A( (LPTSTR)((LPCTSTR)strPrev) );
					if( issjiswidekatakana( lpstrPrev[0], lpstrPrev[1] ) != TRUE )
						continue;
				}
				// �S�p�}�C�i�X�͐����̌����
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
				if( pstrErr ) *pstrErr += CString( lpstr );	// �G���[�����͕Ԃ�
				continue;
			}
		}
		else					// ���p�̏ꍇ
		{
			// ���p�̎��A����������
			if ( isasciinumber( lpstr[0] ) )		// �u0�v�`�u9�v
			{
				if( nOcnt == 0 && isasciizero( lpstr[0] ) ) 	// �擪���u0�v�̓_��
					continue;
			}
			else if ( bUseMinusPeriod && isasciiminus( lpstr[0] ) && ( ! bMinus ) && nOcnt == 0)		// �u-�v������A�������擪�̂�
			{
				bMinus = TRUE;
				nOcnt  = -1;	// �u-�v�ȍ~����J�E���g���邽��
			}
			else if ( bUseMinusPeriod && isasciiperiod( lpstr[0] ) && ( ! bPeriod ) )	// �u.�v�����
			{
				bPeriod = TRUE;
				if (nOcnt == 0)	// �h�b�g�̑O�ɐ������Ȃ�������
				{
					// '0' = 0x30�������I�ɒǉ�
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

	// ���ʂ��u-�v�݂̂ɂȂ������ǂ����邩�i���p�̏ꍇ�݂̂̂͂��j
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

		lpszText = (LPTSTR)_malloca( nTextLen * sizeof(TCHAR) );	// �����̈���m��
		// �S�Ĕ��p�ɂ��Ă��܂�
		LCMapString( GetUserDefaultLCID(), LCMAP_HALFWIDTH, lpcszText, nTextLen, lpszText, nTextLen );
		strText = lpszText;
		_freea( lpszText );
	}

	CString strRet;
	LONG lLen = CString_GetLength2( strText );

	INT  nOcnt   = 0;		// �o�͕�����
	BOOL bMinus  = FALSE;
	BOOL bPeriod = FALSE;
	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2( strText, i );
		CT2A lpstr( str );

		if( lpstr )
		{
			// ���p�̎��A����������
			if ( isasciinumber( lpstr[0] ) )		// �u0�v�`�u9�v
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


// ���t�Ɏg���镶�����K���ȕ�����ɏC��
CString AheConvertToDateStr( LPCTSTR lpcszText )
{
	CString strText;
	{
		INT nTextLen = lstrlen( lpcszText ) + 1;
		LPTSTR lpszText = NULL;

		lpszText = (LPTSTR)_malloca( nTextLen * sizeof(TCHAR) );	// �����̈���m��
		// �S�Ĕ��p�ɂ��Ă��܂�
		LCMapString( GetUserDefaultLCID(), LCMAP_HALFWIDTH, lpcszText, nTextLen, lpszText, nTextLen );
		strText = lpszText;
		_freea( lpszText );
	}

	CString strRet;
	LONG lLen = CString_GetLength2( strText );

	INT  nOcnt   = 0;		// �o�͕�����
	// �ꕶ�����m�F���A�g�p�\�����̂ݕێ�����
	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2( strText, i );
		CT2A lpstr( str );

		if( lpstr )
		{
			// ���p�́u0�v�`�u9�v�Ɓu-�v�ȊO�͊i�[���Ȃ�
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
