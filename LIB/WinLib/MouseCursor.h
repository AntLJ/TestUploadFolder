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
 * @file MouseCursor.h
 * @brief <b>CMouseCursor定義ファイル
 */
#ifndef _MOUSECURSOR_H_
#define _MOUSECURSOR_H_

class CMouseCursor
{
public:
	CMouseCursor()
	{
		m_hOld = NULL;
	}
	CMouseCursor( HCURSOR cur )
	{
		m_hOld = ::SetCursor( cur );
	}
	CMouseCursor( long id )
	{
		SetCursor( id );
	}
	virtual ~CMouseCursor()
	{
		if( m_hOld )
			::SetCursor( m_hOld );
	}
	void SetCursor( long id )
	{
		LPCTSTR nID = NULL;
		switch( id )
		{
			case 0: nID = IDC_ARROW; break;
			case 1: nID = IDC_IBEAM; break;
			case 2: nID = IDC_WAIT; break;
			case 3: nID = IDC_CROSS; break;
			case 4: nID = IDC_UPARROW; break;
			case 5: nID = IDC_SIZEALL; break;
			case 6: nID = IDC_SIZENWSE; break;
			case 7: nID = IDC_SIZENESW; break;
			case 8: nID = IDC_SIZEWE; break;
			case 9: nID = IDC_SIZENS; break;
			case 10: nID = IDC_NO; break;
			default: nID = IDC_ARROW; break;
		}

		m_hOld = ::SetCursor( LoadCursor( NULL, nID ) );
	}
protected:
	HCURSOR m_hOld;
};

#endif // ifndef _MOUSECURSOR_H_
