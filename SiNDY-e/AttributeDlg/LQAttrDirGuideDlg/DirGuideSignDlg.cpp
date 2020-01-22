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

// DirGuideSignDlg.cpp: CDirGuideSignDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirGuideSignDlg.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CDirGuideSignDlg::CDirGuideSignDlg()
{
#ifndef FOR_ASEAN // Bug 11268対応
	m_hNullBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_NULL ));
	m_hNoPatternBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_NOPATTERN ));
	m_hTwoLevelCrossoverABmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_TWOLEVELCROSSOVERA ));
	m_hTwoLevelCrossoverBBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_TWOLEVELCROSSOVERB ));
	m_hCrossRoadsBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_CROSSROADS ));
	m_hLeftForkedRoadBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_LEFTFORKEDROAD ));
	m_hRightForkedRoadBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_RIGHTFORKEDROAD ));
	m_hYForkedRoadBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_YFORKEDROAD ));
	m_hTForkedRoadBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_TFORKEDROAD ));
#else
	// Bug 11268で画像を追加しているが、看板の絵に描いているフォント設定は下記のとおり
	// フォント名:Franklin Gothic スタイル:中太 サイズ:20
	m_hNullBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_ASEAN_NULL ));
	m_hNoPatternBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_ASEAN_OTHERCOLOR ));
	m_hAseanGreenBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_ASEAN_GREEN ));
	m_hAseanBlueBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_ASEAN_BLUE ));
	m_hAseanWhiteBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_ASEAN_WHITE ));
	m_hAseanOrangeBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_ASEAN_ORANGE ));
#endif // FOR_ASEAN
}

CDirGuideSignDlg::~CDirGuideSignDlg()
{
	::DeleteObject( m_hNullBmp );
	::DeleteObject( m_hNoPatternBmp );
#ifndef FOR_ASEAN // Bug 11268対応
	::DeleteObject( m_hTwoLevelCrossoverABmp );
	::DeleteObject( m_hTwoLevelCrossoverBBmp );
	::DeleteObject( m_hCrossRoadsBmp );
	::DeleteObject( m_hLeftForkedRoadBmp );
	::DeleteObject( m_hRightForkedRoadBmp );
	::DeleteObject( m_hYForkedRoadBmp );
	::DeleteObject( m_hTForkedRoadBmp );
#else
	::DeleteObject( m_hAseanGreenBmp );
	::DeleteObject( m_hAseanBlueBmp );
	::DeleteObject( m_hAseanWhiteBmp );
	::DeleteObject( m_hAseanOrangeBmp );
#endif // FOR_ASEAN
}

