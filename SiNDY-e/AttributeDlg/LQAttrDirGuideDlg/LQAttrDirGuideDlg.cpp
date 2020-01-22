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

// LQAttrDirGuideDlg.cpp : CLQAttrDirGuideDlg のインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LQAttrDirGuideDlg.h"

namespace {
#ifndef FOR_ASEAN // Bug 11268対応(幅の設定を変える)
	int HOUMEN_WIDTH = 85;
	int KANJI_PERCENT = 25;
#else
	int HOUMEN_WIDTH = 115;
	int KANJI_PERCENT = 35;
#endif // FOR_ASEAN
}
/// ウィンドウマネージャ配置マップ（方面案内用）
BEGIN_WINDOW_MAP(DIRGUIDE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(0,0))
		RCSPACE(1)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_INLINKID)
			RCTOFIT(IDC_COMBO_INLINKID)
			RCREST(-1)
			RCSPACE(4)
			RCTOFIT(IDC_STATIC_INNODEID)
			RCTOFIT(IDC_COMBO_INNODEID2)
		ENDGROUP()
		BEGINROWS(WRCT_REST, 0, RCMARGINS(3,0))
			BEGINROWS(WRCT_REST,IDC_STATIC_GROUPBOX,RCMARGINS(2,2))
				RCSPACE(45)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(3,0))
					RCSPACE(200)
					RCFIXED(IDC_STATIC_SEQENCE, 80)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_COMBO_SEQUENCE)
					ENDGROUP()
				ENDGROUP()
				RCSPACE(30)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(3,0))
					RCFIXED(IDC_STATIC_HOUMENSHUBETU, HOUMEN_WIDTH)
					RCPERCENT(IDC_STATIC_HOUMENKANJI, KANJI_PERCENT)
					RCPERCENT(IDC_STATIC_HOUMENYOMI, KANJI_PERCENT)
					RCREST(-1)
					BEGINROWS(WRCT_TOFIT,IDC_STATIC_GROUPBOX_BTN,RCMARGINS(0,0))
						RCSPACE(7)
						BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(3,3))
							RCFIXED(IDC_BUTTON_INS,30)
							RCFIXED(IDC_BUTTON_DEL,30)
							RCFIXED(IDC_BUTTON_UP, 30)
							RCFIXED(IDC_BUTTON_DN, 30)
						ENDGROUP()
						RCSPACE(2)
					ENDGROUP()
				ENDGROUP()
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(3,0))
					RCFIXED(IDC_COMBO_HOUMENSHUBETU1, HOUMEN_WIDTH)
					RCPERCENT(IDC_EDIT_HOUMENKANJI1, KANJI_PERCENT)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_EDIT_HOUMENYOMI1)
					ENDGROUP()
					RCFIXED(IDC_RADIO1,15)
				ENDGROUP()
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(3,0))
					RCFIXED(IDC_COMBO_HOUMENSHUBETU2, HOUMEN_WIDTH)
					RCPERCENT(IDC_EDIT_HOUMENKANJI2, KANJI_PERCENT)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_EDIT_HOUMENYOMI2)
					ENDGROUP()
					RCFIXED(IDC_RADIO2,15)
				ENDGROUP()
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(3,0))
					RCFIXED(IDC_COMBO_HOUMENSHUBETU3, HOUMEN_WIDTH)
					RCPERCENT(IDC_EDIT_HOUMENKANJI3, KANJI_PERCENT)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_EDIT_HOUMENYOMI3)
					ENDGROUP()
					RCFIXED(IDC_RADIO3,15)
				ENDGROUP()
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(3,0))
					RCFIXED(IDC_COMBO_HOUMENSHUBETU4, HOUMEN_WIDTH)
					RCPERCENT(IDC_EDIT_HOUMENKANJI4, KANJI_PERCENT)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_EDIT_HOUMENYOMI4)
					ENDGROUP()
					RCFIXED(IDC_RADIO4,15)
				ENDGROUP()
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(3,0))
					RCFIXED(IDC_COMBO_HOUMENSHUBETU5, HOUMEN_WIDTH)
					RCPERCENT(IDC_EDIT_HOUMENKANJI5, KANJI_PERCENT)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_EDIT_HOUMENYOMI5)
					ENDGROUP()
					RCFIXED(IDC_RADIO5,15)
				ENDGROUP()
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(3,0))
					RCFIXED(IDC_COMBO_HOUMENSHUBETU6, HOUMEN_WIDTH)
					RCPERCENT(IDC_EDIT_HOUMENKANJI6, KANJI_PERCENT)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_EDIT_HOUMENYOMI6)
					ENDGROUP()
					RCFIXED(IDC_RADIO6,15)
				ENDGROUP()
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(3,0))
					RCFIXED(IDC_COMBO_HOUMENSHUBETU7, HOUMEN_WIDTH)
					RCPERCENT(IDC_EDIT_HOUMENKANJI7, KANJI_PERCENT)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_EDIT_HOUMENYOMI7)
					ENDGROUP()
					RCFIXED(IDC_RADIO7,15)
				ENDGROUP()
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(3,0))
					RCFIXED(IDC_COMBO_HOUMENSHUBETU8, HOUMEN_WIDTH)
					RCPERCENT(IDC_EDIT_HOUMENKANJI8, KANJI_PERCENT)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_EDIT_HOUMENYOMI8)
					ENDGROUP()
					RCFIXED(IDC_RADIO8,15)
				ENDGROUP()
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(3,0))
					RCFIXED(IDC_COMBO_HOUMENSHUBETU9, HOUMEN_WIDTH)
					RCPERCENT(IDC_EDIT_HOUMENKANJI9, KANJI_PERCENT)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_EDIT_HOUMENYOMI9)
					ENDGROUP()
					RCFIXED(IDC_RADIO9,15)
				ENDGROUP()
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(3,0))
					RCFIXED(IDC_COMBO_HOUMENSHUBETU10, HOUMEN_WIDTH)
					RCPERCENT(IDC_EDIT_HOUMENKANJI10, KANJI_PERCENT)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_EDIT_HOUMENYOMI10)
					ENDGROUP()
					RCFIXED(IDC_RADIO10,15)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		RCSPACE(1)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_STATIC_SOURCE)
			RCREST(IDC_EDIT_SOURCE)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CLQAttrDirGuideDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrDirGuideDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDirGuideDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CLQAttrDefaultDlg::CLQAttrDefaultDlg()
 * @brief コンストラクタ
 *
 * フォント設定、bitmapの読み込みを行います。
 */
CLQAttrDirGuideDlg::CLQAttrDirGuideDlg() 
	: LQATTR_COMMON_BASE_CLASS(DIRGUIDE_MAP, IDD_LQATTRDIRGUIDEDLG),
	m_wndDirGuideInLink( _T("COMBOBOX"), this, IDC_COMBO_INLINKID ),
	m_wndDirGuideInNode( _T("COMBOBOX"), this, IDC_COMBO_INNODEID2 ),
	m_wndDirGuideClass1( _T("COMBOBOX"), this, IDC_COMBO_HOUMENSHUBETU1 ), 
	m_wndDirGuideClass2( _T("COMBOBOX"), this, IDC_COMBO_HOUMENSHUBETU2 ), 
	m_wndDirGuideClass3( _T("COMBOBOX"), this, IDC_COMBO_HOUMENSHUBETU3 ), 
	m_wndDirGuideClass4( _T("COMBOBOX"), this, IDC_COMBO_HOUMENSHUBETU4 ), 
	m_wndDirGuideClass5( _T("COMBOBOX"), this, IDC_COMBO_HOUMENSHUBETU5 ),
	m_wndDirGuideClass6( _T("COMBOBOX"), this, IDC_COMBO_HOUMENSHUBETU6 ), 
	m_wndDirGuideClass7( _T("COMBOBOX"), this, IDC_COMBO_HOUMENSHUBETU7 ), 
	m_wndDirGuideClass8( _T("COMBOBOX"), this, IDC_COMBO_HOUMENSHUBETU8 ), 
	m_wndDirGuideClass9( _T("COMBOBOX"), this, IDC_COMBO_HOUMENSHUBETU9 ), 
	m_wndDirGuideClass10( _T("COMBOBOX"), this, IDC_COMBO_HOUMENSHUBETU10 ),
	m_wndDirGuideKanji1( _T("EDIT"), this, IDC_EDIT_HOUMENKANJI1 ), 
	m_wndDirGuideKanji2( _T("EDIT"), this, IDC_EDIT_HOUMENKANJI2 ), 
	m_wndDirGuideKanji3( _T("EDIT"), this, IDC_EDIT_HOUMENKANJI3 ), 
	m_wndDirGuideKanji4( _T("EDIT"), this, IDC_EDIT_HOUMENKANJI4 ), 
	m_wndDirGuideKanji5( _T("EDIT"), this, IDC_EDIT_HOUMENKANJI5 ),
	m_wndDirGuideKanji6( _T("EDIT"), this, IDC_EDIT_HOUMENKANJI6 ), 
	m_wndDirGuideKanji7( _T("EDIT"), this, IDC_EDIT_HOUMENKANJI7 ), 
	m_wndDirGuideKanji8( _T("EDIT"), this, IDC_EDIT_HOUMENKANJI8 ), 
	m_wndDirGuideKanji9( _T("EDIT"), this, IDC_EDIT_HOUMENKANJI9 ), 
	m_wndDirGuideKanji10( _T("EDIT"), this, IDC_EDIT_HOUMENKANJI10 ),
	m_wndDirGuideYomi1( _T("EDIT"), this, IDC_EDIT_HOUMENYOMI1 ), 
	m_wndDirGuideYomi2( _T("EDIT"), this, IDC_EDIT_HOUMENYOMI2 ), 
	m_wndDirGuideYomi3( _T("EDIT"), this, IDC_EDIT_HOUMENYOMI3 ), 
	m_wndDirGuideYomi4( _T("EDIT"), this, IDC_EDIT_HOUMENYOMI4 ), 
	m_wndDirGuideYomi5( _T("EDIT"), this, IDC_EDIT_HOUMENYOMI5 ),
	m_wndDirGuideYomi6( _T("EDIT"), this, IDC_EDIT_HOUMENYOMI6 ), 
	m_wndDirGuideYomi7( _T("EDIT"), this, IDC_EDIT_HOUMENYOMI7 ), 
	m_wndDirGuideYomi8( _T("EDIT"), this, IDC_EDIT_HOUMENYOMI8 ), 
	m_wndDirGuideYomi9( _T("EDIT"), this, IDC_EDIT_HOUMENYOMI9 ), 
	m_wndDirGuideYomi10( _T("EDIT"), this, IDC_EDIT_HOUMENYOMI10 ),
	m_wndDirGuideGroup( _T("STATIC"), this, IDC_STATIC_GROUPBOX ),
	m_wndSource( _T("EDIT"), this, IDC_EDIT_SOURCE ),
	m_wndDirGuideGroupBtn( _T("STATIC"), this, IDC_STATIC_GROUPBOX_BTN ),
	m_wndDirGuideIns( _T("BUTTON"), this, IDC_BUTTON_INS ),
	m_wndDirGuideDel( _T("BUTTON"), this, IDC_BUTTON_DEL ),
	m_wndDirGuideUp( _T("BUTTON"), this, IDC_BUTTON_UP ),
	m_wndDirGuideDn( _T("BUTTON"), this, IDC_BUTTON_DN ),
	m_wndSequence( _T("COMBO"), this, IDC_COMBO_SEQUENCE )
{
	// フォント作成
	m_hFont = CreateFont(
		18, 0, 0, 0, 100,                           // 高さ，幅，Ｘ角度，Ｙ角度，太さ
		FALSE, FALSE, FALSE,                        // 斜体，下線，打消線
		SHIFTJIS_CHARSET,                           // キャラクタ設定
		OUT_DEFAULT_PRECIS,                         // 出力精度
		CLIP_DEFAULT_PRECIS,                        // クリップ精度
		DEFAULT_QUALITY,                            // 品質
		FIXED_PITCH | FF_DONTCARE,                // ピッチ
		AheLoadString(IDS_FONT));                       // フォント種類

	// ボタンのビットマップ読み込み
	m_hStraightBmp   = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_STRAIGHT));
	m_hStraightGBmp  = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_STRAIGHTG));
	m_hRightTurnBmp  = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_RIGHTTURN));
	m_hRightTurnGBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_RIGHTTURNG));
	m_hLeftTurnBmp   = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_LEFTTURN));
	m_hLeftTurnGBmp  = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_LEFTTURNG));

#ifndef FOR_ASEAN // Bug 11268対応
	m_hNoPatternBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_NOPATTERN ));
	m_hNullBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_NULL ));
	m_hNothingGuidePattern_c = ::LoadBitmap( _AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_NOTHINGGUIDEPATTERN_C ));
	m_hTwoLevelCrossoverABmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_TWOLEVELCROSSOVERA ));
	m_hTwoLevelCrossoverA0Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_TWOLEVELCROSSOVERA0 ));
	m_hTwoLevelCrossoverA1Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_TWOLEVELCROSSOVERA1 ));
	m_hTwoLevelCrossoverA2Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_TWOLEVELCROSSOVERA2 ));
	m_hTwoLevelCrossoverBBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_TWOLEVELCROSSOVERB ));
	m_hTwoLevelCrossoverB0Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_TWOLEVELCROSSOVERB0 ));
	m_hTwoLevelCrossoverB1Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_TWOLEVELCROSSOVERB1 ));
	m_hTwoLevelCrossoverB2Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_TWOLEVELCROSSOVERB2 ));
	m_hCrossRoadsBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_CROSSROADS ));
	m_hCrossRoads0Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_CROSSROADS0 ));
	m_hCrossRoads1Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_CROSSROADS1 ));
	m_hCrossRoads2Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_CROSSROADS2 ));
	m_hLeftForkedRoadBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_LEFTFORKEDROAD ));
	m_hLeftForkedRoad0Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_LEFTFORKEDROAD0 ));
	m_hLeftForkedRoad1Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_LEFTFORKEDROAD1 ));
	m_hRightForkedRoadBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_RIGHTFORKEDROAD ));
	m_hRightForkedRoad0Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_RIGHTFORKEDROAD0 ));
	m_hRightForkedRoad1Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_RIGHTFORKEDROAD1 ));
	m_hYForkedRoadBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_YFORKEDROAD ));
	m_hYForkedRoad0Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_YFORKEDROAD0 ));
	m_hYForkedRoad1Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_YFORKEDROAD1 ));
	m_hTForkedRoadBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_TFORKEDROAD ));
	m_hTForkedRoad0Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_TFORKEDROAD0 ));
	m_hTForkedRoad1Bmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_TFORKEDROAD1 ));
#else
	m_hNoPatternBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_ASEAN_OTHERCOLOR ));
	m_hNullBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_ASEAN_NULL ));
	m_hNothingGuidePattern_c = ::LoadBitmap( _AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_ASEAN_NOTHINGGUIDEPATTERN_C ));
	m_hAseanGreenBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_ASEAN_GREEN ));
	m_hAseanBlueBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_ASEAN_BLUE ));
	m_hAseanWhiteBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_ASEAN_WHITE ));
	m_hAseanOrangeBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_ASEAN_ORANGE ));
#endif // FOR_ASEAN
	m_hBrush1 = m_hBrush2 = m_hBrush3 = NULL;
	m_TwndKanji1 = m_TwndKanji2 = m_TwndKanji3 = m_TwndKanji4 = m_TwndKanji5 = NULL;
	m_TwndKanji6 = m_TwndKanji7 = m_TwndKanji8 = m_TwndKanji9 = m_TwndKanji10 = NULL;
	m_TwndKana1 = m_TwndKana2 = m_TwndKana3 = m_TwndKana4 = m_TwndKana5 = NULL;
	m_TwndKana6 = m_TwndKana7 = m_TwndKana8 = m_TwndKana9 = m_TwndKana10 = NULL;
	m_HWNDHoumen[0] = m_TwndKanji1;
	m_HWNDHoumen[1] = m_TwndKanji2;
	m_HWNDHoumen[2] = m_TwndKanji3;
	m_HWNDHoumen[3] = m_TwndKanji4;
	m_HWNDHoumen[4] = m_TwndKanji5;
	m_HWNDHoumen[5] = m_TwndKanji6;
	m_HWNDHoumen[6] = m_TwndKanji7;
	m_HWNDHoumen[7] = m_TwndKanji8;
	m_HWNDHoumen[8] = m_TwndKanji9;
	m_HWNDHoumen[9] = m_TwndKanji10;
	m_HWNDHoumen[10] = m_TwndKana1;
	m_HWNDHoumen[11] = m_TwndKana2;
	m_HWNDHoumen[12] = m_TwndKana3;
	m_HWNDHoumen[13] = m_TwndKana4;
	m_HWNDHoumen[14] = m_TwndKana5;
	m_HWNDHoumen[15] = m_TwndKana6;
	m_HWNDHoumen[16] = m_TwndKana7;
	m_HWNDHoumen[17] = m_TwndKana8;
	m_HWNDHoumen[18] = m_TwndKana9;
	m_HWNDHoumen[19] = m_TwndKana10;
	m_bSetCurrentFeatureDefIndex = false;

	m_vectorControlIDSet.reserve(10);
	m_vectorControlIDSet.push_back(CONTROL_ID_SET(IDC_RADIO1, IDC_COMBO_HOUMENSHUBETU1, IDC_EDIT_HOUMENKANJI1, IDC_EDIT_HOUMENYOMI1));
	m_vectorControlIDSet.push_back(CONTROL_ID_SET(IDC_RADIO2, IDC_COMBO_HOUMENSHUBETU2, IDC_EDIT_HOUMENKANJI2, IDC_EDIT_HOUMENYOMI2));
	m_vectorControlIDSet.push_back(CONTROL_ID_SET(IDC_RADIO3, IDC_COMBO_HOUMENSHUBETU3, IDC_EDIT_HOUMENKANJI3, IDC_EDIT_HOUMENYOMI3));
	m_vectorControlIDSet.push_back(CONTROL_ID_SET(IDC_RADIO4, IDC_COMBO_HOUMENSHUBETU4, IDC_EDIT_HOUMENKANJI4, IDC_EDIT_HOUMENYOMI4));
	m_vectorControlIDSet.push_back(CONTROL_ID_SET(IDC_RADIO5, IDC_COMBO_HOUMENSHUBETU5, IDC_EDIT_HOUMENKANJI5, IDC_EDIT_HOUMENYOMI5));
	m_vectorControlIDSet.push_back(CONTROL_ID_SET(IDC_RADIO6, IDC_COMBO_HOUMENSHUBETU6, IDC_EDIT_HOUMENKANJI6, IDC_EDIT_HOUMENYOMI6));
	m_vectorControlIDSet.push_back(CONTROL_ID_SET(IDC_RADIO7, IDC_COMBO_HOUMENSHUBETU7, IDC_EDIT_HOUMENKANJI7, IDC_EDIT_HOUMENYOMI7));
	m_vectorControlIDSet.push_back(CONTROL_ID_SET(IDC_RADIO8, IDC_COMBO_HOUMENSHUBETU8, IDC_EDIT_HOUMENKANJI8, IDC_EDIT_HOUMENYOMI8));
	m_vectorControlIDSet.push_back(CONTROL_ID_SET(IDC_RADIO9, IDC_COMBO_HOUMENSHUBETU9, IDC_EDIT_HOUMENKANJI9, IDC_EDIT_HOUMENYOMI9));
	m_vectorControlIDSet.push_back(CONTROL_ID_SET(IDC_RADIO10, IDC_COMBO_HOUMENSHUBETU10, IDC_EDIT_HOUMENKANJI10, IDC_EDIT_HOUMENYOMI10));
}

/** 
 * @fn HWND CLQAttrDirGuideDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * LQATTR_COMMON_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CLQAttrDirGuideDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// ダイアログの作成はLQATTR_COMMON_BASE_CLASSに任せます
	m_hChildWnd = LQATTR_COMMON_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrDirGuideDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CLQAttrDirGuideDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}


/**
 * @fn BOOL CLQAttrDirGuideDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CLQAttrDirGuideDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	//前回の状態によって、コンボボックスの選択肢がグレーアウトされる問題用（正しい解決策かは不明）
	if( bEditable )
	{
		for(INT i = 0; i < 10; ++i)
		{
			SetDlgStatus(m_vectorControlIDSet[i].nClassID, TRUE);
		}
	}
	//前回の状態によって、コンボボックスの選択肢がグレーアウトされる問題用（正しい解決策かは不明）　ここまで
	m_bSetCurrentFeatureDefIndex = true;
    BOOL bRet = LQATTR_COMMON_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );
	m_bSetCurrentFeatureDefIndex = false;

	m_bEditable = bEditable;
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		::SetFocus( GetDlgItem(IDC_COMBO_HOUMENSHUBETU1) );

#ifndef FOR_ASEAN // Bug 11268対応
		CLQRowDef *cLQRowDef = GetCurrentFeatureDef();
		CFieldDef* pSeqFieldDef = cLQRowDef->GetFieldDef( 	m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ), _T("SEQUENCE"));

		if( !pSeqFieldDef )
			SetDlgStatus( IDC_COMBO_SEQUENCE, FALSE );

		if( m_lLastLink == 1 )
			SetDlgStatus( IDC_COMBO_SEQUENCE, FALSE );
#else
		// ASEANの場合は常にシーケンスはグレーアウト
		SetDlgStatus( IDC_COMBO_SEQUENCE, FALSE );
#endif // FOR_ASEAN
	}
	else
		::SendMessage( GetDlgItem(IDC_COMBO_HOUMENSHUBETU1), WM_KILLFOCUS, NULL, NULL );

	// 方面案内用のボタン作成
	SetSequence();
	CreateDirGuideButton();

	for( INT i = 1; i <= 20; i++)
		SetToolTip( GetHoumenID(i) );

	INT nSelected = GetSelectedIndex();
	if(nSelected > 0)
	{
		CButton(GetDlgItem( m_vectorControlIDSet[nSelected].nRadioID )).SetCheck(FALSE);
	}
	CButton (GetDlgItem( m_vectorControlIDSet[0].nRadioID )).SetCheck(TRUE);
	UIUpdate();
	return bRet;
}

/**
 * @fn void CLQAttrDirGuideDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CLQAttrDirGuideDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		CString strInfTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName );
		m_cControlRel.SetHWND( m_hWnd );
		// 方面案内用
		m_cControlRel.SetControl( IDC_COMBO_SEQUENCE,		TYPE_COMBO,			0, 0,	_T("SEQUENCE"),			NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_COMBO_HOUMENSHUBETU1,	TYPE_COMBO,			0, 0,	_T("GUIDECLASS1_C"),	NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENKANJI1,	TYPE_EDITSTRING,	0, 0,	_T("NAME_KANJI1"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENYOMI1,		TYPE_EDITSTRING,	0, 0,	_T("NAME_YOMI1"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_COMBO_HOUMENSHUBETU2,	TYPE_COMBO,			0, 0,	_T("GUIDECLASS2_C"),	NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENKANJI2,	TYPE_EDITSTRING,	0, 0,	_T("NAME_KANJI2"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENYOMI2,		TYPE_EDITSTRING,	0, 0,	_T("NAME_YOMI2"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_COMBO_HOUMENSHUBETU3,	TYPE_COMBO,			0, 0,	_T("GUIDECLASS3_C"),	NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENKANJI3,	TYPE_EDITSTRING,	0, 0,	_T("NAME_KANJI3"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENYOMI3,		TYPE_EDITSTRING,	0, 0,	_T("NAME_YOMI3"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_COMBO_HOUMENSHUBETU4,	TYPE_COMBO,			0, 0,	_T("GUIDECLASS4_C"),	NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENKANJI4,	TYPE_EDITSTRING,	0, 0,	_T("NAME_KANJI4"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENYOMI4,		TYPE_EDITSTRING,	0, 0,	_T("NAME_YOMI4"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_COMBO_HOUMENSHUBETU5,	TYPE_COMBO,			0, 0,	_T("GUIDECLASS5_C"),	NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENKANJI5,	TYPE_EDITSTRING,	0, 0,	_T("NAME_KANJI5"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENYOMI5,		TYPE_EDITSTRING,	0, 0,	_T("NAME_YOMI5"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_COMBO_HOUMENSHUBETU6,	TYPE_COMBO,			0, 0,	_T("GUIDECLASS6_C"),	NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENKANJI6,	TYPE_EDITSTRING,	0, 0,	_T("NAME_KANJI6"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENYOMI6,		TYPE_EDITSTRING,	0, 0,	_T("NAME_YOMI6"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_COMBO_HOUMENSHUBETU7,	TYPE_COMBO,			0, 0,	_T("GUIDECLASS7_C"),	NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENKANJI7,	TYPE_EDITSTRING,	0, 0,	_T("NAME_KANJI7"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENYOMI7,		TYPE_EDITSTRING,	0, 0,	_T("NAME_YOMI7"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_COMBO_HOUMENSHUBETU8,	TYPE_COMBO,			0, 0,	_T("GUIDECLASS8_C"),	NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENKANJI8,	TYPE_EDITSTRING,	0, 0,	_T("NAME_KANJI8"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENYOMI8,		TYPE_EDITSTRING,	0, 0,	_T("NAME_YOMI8"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_COMBO_HOUMENSHUBETU9,	TYPE_COMBO,			0, 0,	_T("GUIDECLASS9_C"),	NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENKANJI9,	TYPE_EDITSTRING,	0, 0,	_T("NAME_KANJI9"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENYOMI9,		TYPE_EDITSTRING,	0, 0,	_T("NAME_YOMI9"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_COMBO_HOUMENSHUBETU10,TYPE_COMBO,			0, 0,	_T("GUIDECLASS10_C"),	NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENKANJI10,	TYPE_EDITSTRING,	0, 0,	_T("NAME_KANJI10"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENYOMI10,	TYPE_EDITSTRING,	0, 0,	_T("NAME_YOMI10"),		NULL, strInfTableName );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,	0, 0,	_T("SOURCE"),			NULL, strInfTableName );
	}
}
/// 現在セットされている方面案内のOIDを取得する
LONG CLQAttrDirGuideDlg::GetDirGuideOID( FEATUREDEF* pFeatDef )
{
	LONG lOID = -1;

	if( pFeatDef )
	{
		if( pFeatDef->GetFeature() )
			pFeatDef->GetFeature()->get_OID( &lOID );
	}
	return lOID;
}
/// 現在セットされている方面案内の流入リンクIDを取得する
LONG CLQAttrDirGuideDlg::GetDirGuideInLinkID( FEATUREDEF* pFeatDef )
{
	CRelationalTableDef* pRelTableDef = pFeatDef->GetRelationalTableDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoLQTableName ) );
	for( CRelationalTableDef::iterator itLQRow = pRelTableDef->begin(); itLQRow != pRelTableDef->end(); itLQRow++ )
	{
		if( GetLongValue( &(*itLQRow), SEQUENCE_FIELD ) == 1 )
			return GetLongValue( &(*itLQRow), LINK_ID_FIELD );
	}

	return -1;
}
/// 指定されたレーンの流出リンクIDを取得する
LONG CLQAttrDirGuideDlg::GetDirGuideOutLinkID( FEATUREDEF* pFeatDef )
{
	CRelationalTableDef* pRelTableDef = pFeatDef->GetRelationalTableDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoLQTableName ) );
	for( CRelationalTableDef::iterator itLQRow = pRelTableDef->begin(); itLQRow != pRelTableDef->end(); itLQRow++ )
	{
		if( GetLongValue( &(*itLQRow), LASTLINK_F_FIELD ) == 1 )
			return GetLongValue( &(*itLQRow), LINK_ID_FIELD );
	}

	return -1;
}

///
LONG CLQAttrDirGuideDlg::GetLongValue( CRowDef* pRowDef, LPCTSTR lpcszFieldName )
{
	LONG lVal = -1;

	if( pRowDef )
	{
		const CFieldDef* pFieldDef = pRowDef->GetFieldDef( lpcszFieldName );
		if( pFieldDef )
		{
			if( pFieldDef->m_vaValue.vt == VT_I4 )
				lVal = pFieldDef->m_vaValue.lVal;
		}
	}
	return lVal;
}
IFeatureClassPtr CLQAttrDirGuideDlg::GetRoadLinkWithLQ( CLQRowDef* pRowDef )
{
	IFeatureClassPtr ipRoadFeatureClass;
	if( !pRowDef )
		return NULL;

	_IRowPtr ipRow = pRowDef->GetFeature();
	ITablePtr ipTestTable;
	ipRow->get_Table( &ipTestTable );
	IDatasetPtr ipDataset = ipTestTable;
	CString strTestOwnerName = AheGetFeatureClassOwnerName( ipDataset );

	// TOCにいるすべてのジオフィーチャレイヤを取得してループ回しお目当ての道路リンク取得する
	IMapPtr ipMap( m_cArcHelper.GetMap() );
	IUIDPtr ipUID( CLSID_UID );
	ipUID->put_Value( CComVariant(L"{E156D7E5-22AF-11D3-9F99-00C04F6BC78E}") ); // IGeoFeatureLayer
	IEnumLayerPtr ipEnumLayer;
	if( SUCCEEDED( ipMap->get_Layers( ipUID, VARIANT_TRUE, &ipEnumLayer ) ) )
	{
		ipEnumLayer->Reset();
		ILayerPtr ipLayer;
		while( S_OK == ipEnumLayer->Next( &ipLayer ) )
		{
			if( !ipLayer )
				break;

			IFeatureClassPtr ipFeatClass;
			if( SUCCEEDED( ((IFeatureLayerPtr)ipLayer)->get_FeatureClass( &ipFeatClass ) ) )
			{
				// 道路リンクかつ、オーナ名が同じでないといけない
				if( AheGetFeatureClassName( ipLayer ).CompareNoCase( ROAD_TABLE_NAME ) == 0 && strTestOwnerName == AheGetFeatureClassOwnerName( ipFeatClass ))
				{
					ipRoadFeatureClass = ipFeatClass;
					break;
				}
			}
		}
	}
	return ipRoadFeatureClass;
}
IFeaturePtr CLQAttrDirGuideDlg::GetRoadFeature( CLQRowDef* pRowDef, LONG lOID )
{
	IFeaturePtr ipFeature;
	if( lOID > 0 )
	{
		// LQと同じワークスペースの道路リンクフィーチャクラスを取得
		IFeatureClassPtr ipRoadClass = GetRoadLinkWithLQ( pRowDef );
		// 流入リンクフィーチャ取得
		// 編集開始前は編集キャッシュがとれないので、全検索したほうがはやいので編集開始前開始後で場合わけ
		if( IsStartEditing() )
		{
			if( ipRoadClass )
				ipRoadClass->GetFeature( lOID, &ipFeature );
		}
		else{
			// 道路リンクテーブルからクエリ検索して探す
			CString		strWhereClause, strSubFields;
			strWhereClause.Format(_T("OBJECTID = %ld"), lOID );
			strSubFields.Format(_T("TO_NODE_ID,FROM_NODE_ID,SHAPE"));
			IQueryFilterPtr		ipQueryFilter( CLSID_QueryFilter );
			ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
			ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );

			IFeatureCursorPtr ipCursor;
			ipRoadClass->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
			ipCursor->NextFeature( &ipFeature );
		}
	}

	return ipFeature;	
}

void CLQAttrDirGuideDlg::CreateDirGuideButton()
{
	FEATUREDEF* pFeatDef = GetCurrentFeatureDef();	
	LONG lCurrentInLinkID = GetDirGuideInLinkID( pFeatDef );			// 現在セットされている方面案内の流入リンクIDを取得する
	LONG lCurrentDirGuideOID  = GetDirGuideOID( pFeatDef );					// 現在セットされている方面案内のOIDを取得する

	if( lCurrentInLinkID < 0 )
	{
		// 流入リンクIDが見つからなかった場合
		ATLASSERT( FALSE );
		// ワーニングメッセージを出してください
		return;
	}

	// 現在セットされている方面案内の最終ポイントを取得する
	IPointPtr ipCurrentFirstPoint;
	IPolylinePtr ipCurrentDirGuideGeom = pFeatDef->GetShape();
	if( ipCurrentDirGuideGeom )	
		ipCurrentDirGuideGeom->get_FromPoint( &ipCurrentFirstPoint );

	// 現在セットされている方面案内の流入リンクのFROM、TOのどちらが ipCurrentFirstPoint なのかを調べる
	IFeaturePtr ipRoadFeature = GetRoadFeature( pFeatDef, lCurrentInLinkID ); // 現在セットされている方面案内の流入リンクを取得する

	BOOL bIsEqualFrom = FALSE;
	if( ipRoadFeature )
	{
		IGeometryPtr ipGeom;
		IPointPtr ipFromPoint, ipToPoint;

		ipRoadFeature->get_Shape( &ipGeom );
		if( !ipGeom )
		{
			ATLASSERT( ipGeom != NULL );
			return;
		}
		((IPolylinePtr)ipGeom)->get_FromPoint( &ipFromPoint );
		((IPolylinePtr)ipGeom)->get_ToPoint( &ipToPoint );
		
		if( AheIsEqual( ipFromPoint, ipCurrentFirstPoint ) )
			bIsEqualFrom = TRUE;
		else if( AheIsEqual( ipToPoint, ipCurrentFirstPoint ) )
			bIsEqualFrom = FALSE;
		else
		{
			ATLASSERT(FALSE);
			return;
		}
	}

	// 流入ノードIDをコンボボックスに入れる用
	CComboBox comboNode( GetDlgItem(IDC_COMBO_INNODEID2) );			// 流入ノードID用コンボボックス

	comboNode.ResetContent();
	CComVariant vaFromID = AheGetAttribute( ipRoadFeature, FROM_FIELD );
	CComVariant vaToID   = AheGetAttribute( ipRoadFeature, TO_FIELD );
	vaFromID.ChangeType( VT_BSTR );
	vaToID.ChangeType( VT_BSTR );

	CString strInfoTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName );
	CString strLQTableName   = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoLQTableName );
	// vaLinkID と同じ ID の道路リンクに関連していて、且つその道路リンクが流入リンクである方面案内を探す
	list<FEATUREDEF*> listSameInFeature;
	CComboBox combo(     GetDlgItem(IDC_COMBO_INLINKID) );			// 流入リンクID用コンボボックス
	combo.ResetContent();
	m_listSameINFeature.clear();

	// 現在表示されているリンク列のオーナ名を取得
	CString strCurrentOwnerName = AheGetFeatureClassOwnerName( GetCurrentFeatureDef()->GetFeature() );

	list<FEATUREDEF>* pFeatDefList = m_cControlRel.GetFeatureDefList();
	for( list<FEATUREDEF>::iterator itFeatureDef = pFeatDefList->begin(); itFeatureDef != pFeatDefList->end(); itFeatureDef++ )
	{
		// 現在表示されているリンク列のデータセットでなかったら、流入リンクIDにセットしない
		CString strOwnerName = AheGetFeatureClassOwnerName( itFeatureDef->GetFeature() );
		if( strOwnerName != strCurrentOwnerName ) continue;

		if( lstrcmpi( itFeatureDef->GetTableName(), m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ) ) != 0 ) continue;	// 方面案内以外は飛ばす

		CRelationalTableDef* pRelTableDef = itFeatureDef->GetRelationalTableDef( strLQTableName );
		if( pRelTableDef == NULL ) continue;

		for( CRelationalTableDef::iterator itLQRow = pRelTableDef->begin(); itLQRow != pRelTableDef->end(); itLQRow++ )
		{
			if( GetLongValue( &(*itLQRow), SEQUENCE_FIELD ) == 1 )
			{
				LONG lLinkID = GetLongValue( &(*itLQRow), LINK_ID_FIELD );
				CString strLinkID;

				// コンボボックスに流入リンクIDを登録
				strLinkID.Format(_T("%d"), lLinkID );
				INT nComboIndex = combo.FindString( 0, strLinkID );
				if( nComboIndex == CB_ERR )
					nComboIndex = combo.AddString( strLinkID );

				if( lLinkID == lCurrentInLinkID )
				{
					IPointPtr ipFirstPoint;
					IPolylinePtr ipDirGuideGeom = itFeatureDef->GetShape();
					if( ipCurrentDirGuideGeom != NULL )	ipDirGuideGeom->get_FromPoint( &ipFirstPoint );

					// 流入ノードIDをセットする
					LPCTSTR lpcszID = NULL;
					if( ( bIsEqualFrom == TRUE  && AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) ) ||
						( bIsEqualFrom == FALSE && AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) == FALSE ) )
						lpcszID = OLE2T(vaFromID.bstrVal);
					else
						lpcszID = OLE2T(vaToID.bstrVal);

					INT nPos = comboNode.FindString( 0, lpcszID );
					if( nPos == CB_ERR )
						nPos = comboNode.AddString( lpcszID );
					// 同じ始点を持つもののみをグルーピングする
					if( AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) )
					{
						// 現在の選択項目とする
						combo.SetCurSel( nComboIndex );
						//
						comboNode.SetCurSel( nPos );
						// リストにぶち込んでおく
						listSameInFeature.push_back( &(*itFeatureDef) );
						break;
					}
				}
			}
		}
	}

	// 角度でソート
	SortByAngle( GetCurrentFeatureDef(), lCurrentInLinkID, listSameInFeature, m_listSameINFeature );

	if( m_listSameINFeature.size() <= 0 ) 
	{
		ATLASSERT( m_listSameINFeature.size() > 0 );
		return;
	}

	// 見つかった方面案内の数分ボタンを作成する
	LONG nCount = 0;
	for( list<CLevelOID>::iterator it = m_listSameINFeature.begin(); it != m_listSameINFeature.end(); it++, nCount++ )
	{
		INT nPrevRight;
		if( nCount > 0 )
		{
			list<CLevelOID>::iterator it = m_listSameINFeature.begin();
			advance( it, nCount - 1 );

			CRect cPrevRect;

			::GetClientRect( it->m_hWnd, cPrevRect );
			it->MapWindowPoints( GetDlgItem( IDC_STATIC_GROUPBOX ), cPrevRect );
			nPrevRight = cPrevRect.right;
		}
		else
			nPrevRight = 150;

		INT nWidth = 30, nHeight = 30;
		CRect rect( nPrevRight, 10, nPrevRight + nWidth, nHeight+10 );
		it->Create( GetDlgItem(IDC_STATIC_GROUPBOX), rect, _T("CLevelOID"), BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1000 + nCount );
		// どっちの方向か判断する
		DOUBLE dAngle = it->m_lAngle;
		LONG lOID = -1, lCurrentOID = -1;

		it->m_pLQRowDef->GetFeature()->get_OID( &lOID );
		pFeatDef->GetFeature()->get_OID( &lCurrentOID );

		if( 159 < dAngle && dAngle < 201 )
		{
			if( lOID == lCurrentOID )
			{
				it->SetBitmap( CopyBitmap( m_hStraightBmp ) );
				m_lDirection = 0;
			}
			else
				it->SetBitmap( CopyBitmap( m_hStraightGBmp ) );
		}
		else if( dAngle < 160 )
		{
			if( lOID == lCurrentOID )
			{
				it->SetBitmap( CopyBitmap( m_hRightTurnBmp ) );
				m_lDirection = 1;
			}
			else
				it->SetBitmap( CopyBitmap( m_hRightTurnGBmp ) );
		}
		else if( dAngle > 200 )
		{
			if( lOID == lCurrentOID )
			{
				it->SetBitmap( CopyBitmap( m_hLeftTurnBmp ) );
				m_lDirection = 2;
			}
			else
				it->SetBitmap( CopyBitmap( m_hLeftTurnGBmp ) );
		}
	}
}

void CLQAttrDirGuideDlg::SetSequence()
{
	CLQRowDef *cLQRowDef;
	cLQRowDef = GetCurrentFeatureDef();

	if( cLQRowDef )
	{
		CFieldDef* pGuideFieldDef = cLQRowDef->GetFieldDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ), _T("GUIDEPATTERN_C"));
		CFieldDef* pSeqFieldDef = cLQRowDef->GetFieldDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ), _T("SEQUENCE"));

		if( pGuideFieldDef && pSeqFieldDef )
		{
			CComboBox cSeqCombo(GetDlgItem( IDC_COMBO_SEQUENCE ));
			cSeqCombo.ResetContent();

			LONG lGuidePattern;
			lGuidePattern = pGuideFieldDef->m_vaValue.lVal;

			CComVariant vaValue = pSeqFieldDef->m_vaValue;
			LONG lSequence;
			if( vaValue.vt == VT_NULL )
				lSequence = -1;
			else
				lSequence = pSeqFieldDef->m_vaValue.lVal;


			// 変更前のシーケンス取得
			_IRowPtr ipRow = GetCurrentFeatureDef()->GetFeature();
			LONG lOriginalSequence;
			if( ipRow )
			{
				IFieldsPtr ipFields;
				LONG lIndex = 0;
				ipRow->get_Fields( &ipFields );
				ipFields->FindField( CComBSTR(_T("SEQUENCE")),&lIndex );
				CComVariant vaValue;
				ipRow->get_Value( lIndex, &vaValue );

				if( vaValue.vt == VT_NULL )
					lOriginalSequence = -1;
				else
					lOriginalSequence = vaValue.lVal;
			}

#ifndef FOR_ASEAN // Bug 11268対応
			// シーケンスコンボボックスの中身の変更
			if( lGuidePattern == 0 || lGuidePattern == 99)
			{
				cSeqCombo.AddString( _T("<NULL>") );
				cSeqCombo.SetCurSel( 0 );

				// コンボボックスの色を制御
				if( lSequence == lOriginalSequence )
					pSeqFieldDef->m_bChanged = FALSE;
				else
					pSeqFieldDef->m_bChanged = TRUE;
			}
			else if( lGuidePattern == 1 || lGuidePattern == 2 || lGuidePattern == 3 )
			{
				cSeqCombo.AddString( _T("0") );
				cSeqCombo.AddString( _T("1") );
				cSeqCombo.AddString( _T("2") );

				// コンボボックスの色を制御
				if( lSequence == lOriginalSequence )
					pSeqFieldDef->m_bChanged = FALSE;
				else
					pSeqFieldDef->m_bChanged = TRUE;

				cSeqCombo.SetCurSel( lSequence );
			}
			else if( lGuidePattern == 4 || lGuidePattern == 5 || lGuidePattern == 6 || lGuidePattern == 7 )
			{
				cSeqCombo.AddString( _T("0") );
				cSeqCombo.AddString( _T("1") );

				// コンボボックスの色を制御
				if( lSequence == lOriginalSequence )
					pSeqFieldDef->m_bChanged = FALSE;
				else
					pSeqFieldDef->m_bChanged = TRUE;

				cSeqCombo.SetCurSel( lSequence );
			}
#else
			// ASEAN環境では要らない気もするがSEQUENCEの値が非NULL→NULLに変更されたことを考慮する(ta_suga)
			cSeqCombo.AddString( _T("<NULL>") );
			pSeqFieldDef->m_bChanged = FALSE;
#endif // FOR_ASEAN
		}
	}
}

LONG CLQAttrDirGuideDlg::ChangeINLinkAndNodeID( LONG lInLinkID, LONG lInNodeID )
{
	list<FEATUREDEF*> listSameInFeature;
	list<CLevelOID>  listLevelOID;
	CString strInfoTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName );
	CString strLQTableName   = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoLQTableName );

	// lInLinkID と同じ ID の道路リンクに関連していて、且つその道路リンクが流入リンクである方面を探す
	list<FEATUREDEF>* pFeatDefList = m_cControlRel.GetFeatureDefList();
	for( list<FEATUREDEF>::iterator itFeatureDef = pFeatDefList->begin(); itFeatureDef != pFeatDefList->end(); itFeatureDef++ )
	{
		if( lstrcmpi( itFeatureDef->GetTableName(), strInfoTableName ) != 0 ) continue;	// 方面以外は飛ばす

		CRelationalTableDef* pRelTableDef = itFeatureDef->GetRelationalTableDef( strLQTableName );
		if( pRelTableDef == NULL ) continue;

		for( CRelationalTableDef::iterator itLQRow = pRelTableDef->begin(); itLQRow != pRelTableDef->end(); itLQRow++ )
		{
			if( itLQRow->GetFieldDef( SEQUENCE_FIELD )->m_vaValue == CComVariant(1L) )
			{
				if( itLQRow->GetFieldDef( LINK_ID_FIELD )->m_vaValue == CComVariant(lInLinkID) )
				{
					if( lInNodeID > 0 )
					{
						// 道路リンクを取得
						IFeaturePtr ipRoadFeature = GetRoadFeature( GetCurrentFeatureDef(), lInLinkID );
						CComVariant vaFromID = AheGetAttribute( ipRoadFeature, FROM_FIELD );
						CComVariant vaToID   = AheGetAttribute( ipRoadFeature, TO_FIELD   );
						// 流入ノードIDとlInNodeIDが同じかどうか
						if( ( ( itLQRow->GetFieldDef( LINKDIR_C_FIELD )->m_vaValue == CComVariant(1L)/* 順方向 */ ) && ( vaFromID.vt == VT_I4 && vaFromID.lVal == lInNodeID ) ) || 
							( ( itLQRow->GetFieldDef( LINKDIR_C_FIELD )->m_vaValue == CComVariant(2L)/* 逆方向 */ ) && ( vaToID.vt   == VT_I4 && vaToID.lVal   == lInNodeID ) ) )
						{
							// リストにぶち込んでおく
							listSameInFeature.push_back( &(*itFeatureDef) );
							break;
						}
					}
					else
					{
						listSameInFeature.push_back( &(*itFeatureDef) );
						break;
					}
				}
			}
		}
	}

	// 角度でソート
	SortByAngle( GetCurrentFeatureDef(), lInLinkID, listSameInFeature, listLevelOID );

	LONG lOID = -1;
	if( listLevelOID.size() > 0 )
		listLevelOID.begin()->m_pLQRowDef->GetFeature()->get_OID( &lOID );
	else
		ATLASSERT( listLevelOID.size() > 0 );

	return lOID;
}
HBITMAP CLQAttrDirGuideDlg::CopyBitmap ( HBITMAP hBitmapSrc, INT nTimes/* = 1*/ )
{
	if( nTimes <= 0 ) return NULL;

	BITMAP  bitmap;
	HBITMAP hBitmapDst;
	HDC     hdcSrc, hdcDst, hDC;

	GetObject( hBitmapSrc, sizeof(BITMAP), &bitmap );

	hDC = GetDC();

	hdcSrc = CreateCompatibleDC (hDC);
	hdcDst = CreateCompatibleDC (hDC);

	// ビットマップを作る
	hBitmapDst = CreateCompatibleBitmap( hDC, bitmap.bmWidth * nTimes, bitmap.bmHeight );

	SelectObject (hdcSrc, hBitmapSrc);
	SelectObject (hdcDst, hBitmapDst);

	for( INT i = 0; i < nTimes; i++ )
	{
		BitBlt( hdcDst, bitmap.bmWidth * i, 0, bitmap.bmWidth, bitmap.bmHeight, hdcSrc, 0, 0, SRCCOPY );
	}

	DeleteDC (hdcSrc);
	DeleteDC (hdcDst);
	ReleaseDC(hDC);

	return hBitmapDst;
}
void CLQAttrDirGuideDlg::SortByAngle( CLQRowDef* pRowDef, LONG lStartLinkID, list<FEATUREDEF*>& listOrg, list<CLevelOID>& listNew )
{
	if( lStartLinkID < 0 || listOrg.size() <= 0 ) 
		return;

	// 流入リンクフィーチャ取得
	IFeaturePtr ipInRoadLink = GetRoadFeature( pRowDef, lStartLinkID );
	// [Bug2848]対応のために、ipInRoadLinkがNULLかどうか調べなくてはならない
	// 流入リンクには、別のデータセットのリンクも入ってきてしまうため、
	if( !ipInRoadLink )
		return;

	IGeometryPtr ipInRoadLinkGeom;
	ipInRoadLink->get_ShapeCopy( &ipInRoadLinkGeom );
	
	// 何番目の構成点からはじめればよいのか調べる
	long lStart = -1;
	((IPointCollectionPtr)ipInRoadLinkGeom)->get_PointCount( &lStart );
	lStart -= 2;	// e.x. 道路リンクの構成点数が3なら1から（0スタート）
	_ASSERTE( lStart >= 0 );

	for( list<FEATUREDEF*>::iterator it = listOrg.begin(); it != listOrg.end(); ++it )
	{
		// リンク列形状と構成点数を取得
		long lTotalPointCount = -1;
		IPointCollectionPtr ipLQGeometry = (*it)->GetShape();
		ipLQGeometry->get_PointCount( &lTotalPointCount );

		// 最終リンク取得
		LONG lEndLinkID = GetDirGuideOutLinkID( *it );
		IFeaturePtr ipOutRoadLink = GetRoadFeature( pRowDef, lEndLinkID );
		if( !ipOutRoadLink )
			return;

		IGeometryPtr ipOutRoadLinkGeom;
		ipOutRoadLink->get_Shape( &ipOutRoadLinkGeom );

		// 何番目の構成点かを取得
		long lEnd = -1;
		((IPointCollectionPtr)ipOutRoadLinkGeom)->get_PointCount( &lEnd );
		lEnd = lTotalPointCount - lEnd;

		LONG lAngle = (LONG)GetAngle( (*it)->GetShape(), lStart );
		LONG lDrawAngle = (LONG)GetAngle2( (*it)->GetShape(), lStart, lEnd );
		LONG lOID = -1;
		(*it)->GetFeature()->get_OID( &lOID );
		
		CLevelOID cOID;
		cOID.m_lOID = lOID;
		cOID.m_mapLevelAngle[lStart] = lAngle;
		cOID.m_pLQRowDef = *it;
		cOID.m_lAngle = lDrawAngle;
		listNew.push_back( cOID );
	}

	// 同じ物があるなら、さらにその先を調べる
	for( list<CLevelOID>::iterator itLevelOID1 = listNew.begin(); itLevelOID1 != listNew.end(); ++itLevelOID1 )
	{
		IGeometryPtr ipLevelOID1Geom = itLevelOID1->m_pLQRowDef->GetShape();
		for( list<CLevelOID>::iterator itLevelOID2 = listNew.begin(); itLevelOID2 != listNew.end(); itLevelOID2++ )
		{
			if( itLevelOID1->m_lOID == itLevelOID2->m_lOID ) continue;

			if( *itLevelOID1 == *itLevelOID2 )
			{
				IGeometryPtr ipLevelOID2Geom = itLevelOID2->m_pLQRowDef->GetShape();
				IGeometryPtr ipIntersectGeom = AheIntersect( ipLevelOID1Geom, ipLevelOID2Geom );
				if( AheIsEqual( ipInRoadLinkGeom, ipIntersectGeom ) == FALSE )
				{
					IPointCollectionPtr ipPointCol( ipIntersectGeom );
					if( ipPointCol )
					{
						LONG lStart = -1;
						ipPointCol->get_PointCount( &lStart );
						lStart -= 2;

						itLevelOID1->m_mapLevelAngle[lStart] = (LONG)GetAngle( ipLevelOID1Geom, lStart );
						itLevelOID2->m_mapLevelAngle[lStart] = (LONG)GetAngle( ipLevelOID2Geom, lStart );
					}
				}
			}
		}
	}
	listNew.sort(greater<CLevelOID>());
}

DOUBLE CLQAttrDirGuideDlg::GetAngle( IPolylinePtr ipPolyline, LONG lStart/* = 0*/, BOOL bIsDrawAngle/* = FALSE*/ )
{
	IPointCollectionPtr ipPolyCopy( AheGetClone( ipPolyline ) );

	if( !ipPolyCopy )
		return -1;

	IPointPtr ipPointS, ipPointC, ipPointE;
	LONG lPointCount = -1;

	ipPolyCopy->get_PointCount( &lPointCount );

	if( lPointCount > lStart )   
		ipPolyCopy->get_Point( lStart, &ipPointS );
	if( lPointCount > lStart+1 ) 
		ipPolyCopy->get_Point( lStart+1, &ipPointC );
	if( !bIsDrawAngle )
	{
		if( lPointCount > lStart+2 ) 
			ipPolyCopy->get_Point( lStart+2, &ipPointE );
	}
	else 
		ipPolyCopy->get_Point( lPointCount - 1, &ipPointE );
	
	if( !ipPointS || !ipPointC || !ipPointE )
		return -1;

	DOUBLE dAngle = AheGetAngle( ipPointS, ipPointC, ipPointE );

	if( dAngle < 0.0 )
		dAngle = 360 + dAngle;

	return dAngle;
}
DOUBLE CLQAttrDirGuideDlg::GetAngle2( IPolylinePtr ipPolyline, LONG lStart, LONG lEnd )
{
	// [lStart] - [lStart+1] のセグメントと
	// [lEnd]   - [lEnd+1]   のセグメントの端点がくっつくように平行移動し、角度を求める
	LONG lPointCount = -1;
	IPointCollectionPtr ipPoly( ipPolyline );
	if( !ipPoly )
		return -1;
	ipPoly->get_PointCount( &lPointCount );
	if( ! ( lPointCount > lStart + 1 && lPointCount > lEnd + 1 ) ) 
		return -1;

	// ベクトルABとベクトルCDとする
	IPointPtr A, B, C, D;
	ipPoly->get_Point( lStart, &A );
	ipPoly->get_Point( lStart+1, &B );
	ipPoly->get_Point( lEnd, &C);
	ipPoly->get_Point( lEnd+1, &D );
	DOUBLE Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
	A->QueryCoords( &Ax, &Ay );
	B->QueryCoords( &Bx, &By );
	C->QueryCoords( &Cx, &Cy );
	D->QueryCoords( &Dx, &Dy );
	// 平行移動する
	DOUBLE Px = Cx - ( Cx - Bx );
	DOUBLE Py = Cy - ( Cy - By );
	DOUBLE D1x = Dx - ( Cx - Bx );
	DOUBLE D1y = Dy - ( Cy - By );

	IPointPtr D1( CLSID_Point );
	D1->PutCoords( D1x, D1y );
	DOUBLE dAngle = AheGetAngle( A, B, D1 );
	if( dAngle < 0.0 )
		dAngle = 360 + dAngle;

	return dAngle;
}
BOOL CLQAttrDirGuideDlg::ErrorCheck()
{
	m_bSequenceError = FALSE;
	BOOL bRet = TRUE;
	CString strInfTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName );
	CString strMsg;
	for( list<CLQRowDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		bool bCurrentRet = TRUE;
		if( strInfTableName.CompareNoCase( it->GetTableName() ) == 0 )
		{
			CFieldDef* pOIDFieldDef = it->GetFieldDef( strInfTableName, _T("OBJECTID"));
			long lOID = pOIDFieldDef->m_vaValue.lVal;

			CFieldDef* pGuidePatternFieldDef = it->GetFieldDef( strInfTableName, _T("GUIDEPATTERN_C") );
			long lGuidePattern = -1;
			if( pGuidePatternFieldDef != NULL )
				lGuidePattern = pGuidePatternFieldDef->m_vaValue.lVal;

			CFieldDef* pSequenceFieldDef = it->GetFieldDef( strInfTableName, _T("SEQUENCE") );
			CComVariant vaSequence;
			if( pSequenceFieldDef != NULL )
				vaSequence = pSequenceFieldDef->m_vaValue;

			// 方面案内の１～１０までをしらべる
			CString strNameKanji, strNameYomi,strGuideC;
			BOOL bIsBlank = FALSE;
			for( LONG i = 1; i <= 10; i++ )
			{
				strNameKanji.Format(_T("NAME_KANJI%d"), i );
				strNameYomi.Format(_T("NAME_YOMI%d"), i );
				strGuideC.Format(_T("GUIDECLASS%d_C"),i);
				CFieldDef* pKanjiFieldDef = it->GetFieldDef( strInfTableName, strNameKanji );
				CFieldDef* pKanaFieldDef  = it->GetFieldDef( strInfTableName, strNameYomi );
				CFieldDef* pGuideCFieldDef  = it->GetFieldDef( strInfTableName, strGuideC );

				if( !pKanjiFieldDef || !pKanaFieldDef || !pGuideCFieldDef )
				{
					_ASSERTE( pKanjiFieldDef != NULL && pKanaFieldDef != NULL );
					continue;
				}

				CString strMsg;
				// 間に空の行がないかどうかしらべる
				if( bIsBlank )
				{
					if( pKanjiFieldDef->m_vaValue.vt == VT_BSTR || pKanaFieldDef->m_vaValue.vt == VT_BSTR )
					{
						if( !m_bNoComment )
						{
							strMsg.Format( AheLoadString( IDS_BLANKROW ), strInfTableName, lOID );
							AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
						}
						bRet = FALSE;
						bCurrentRet = FALSE;
						break;
					}
				}
#ifndef FOR_ASEAN // Bug 11268対応
				// 漢字名称とカナ名称が双方入っているか又はどちらも NULL かどうか
				if( ! ( ( pKanjiFieldDef->m_vaValue.vt == VT_NULL && pKanaFieldDef->m_vaValue.vt == VT_NULL && pGuideCFieldDef->m_vaValue.vt == VT_NULL) ||
					    ( pKanjiFieldDef->m_vaValue.vt == VT_BSTR && pKanaFieldDef->m_vaValue.vt == VT_BSTR && pGuideCFieldDef->m_vaValue.vt != VT_NULL) ) )
				{
					if( !m_bNoComment )
					{
						strMsg.Format( AheLoadString( IDS_BLANKCELL ), strInfTableName, lOID, i );
						AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					}
					bRet = FALSE;
					bCurrentRet = FALSE;
					break;
				}
#else
				// bug 11268対応 ASEANの入力条件
				// 地先種別がNULLならば漢字も読みも(第一名称も第二名称も)NULLでなければならない
				// 地先種別が非NULLならば、漢字(第一名称)に値が入っていなければならない。読み(第二名称)はどっちでもいい
				if( ! ( ( pKanjiFieldDef->m_vaValue.vt == VT_NULL && pKanaFieldDef->m_vaValue.vt == VT_NULL && pGuideCFieldDef->m_vaValue.vt == VT_NULL) ||
					    ( pKanjiFieldDef->m_vaValue.vt == VT_BSTR && pGuideCFieldDef->m_vaValue.vt != VT_NULL) ) )
				{
					if( !m_bNoComment )
					{
						strMsg.Format( AheLoadString( IDS_BLANKKANJI ), strInfTableName, lOID, i );
						AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					}
					bRet = FALSE;
					bCurrentRet = FALSE;
					break;
				}
#endif // FOR_ASEAN
				// 空の行があったら、そこでフラグを立てておく
				if( pKanjiFieldDef->m_vaValue.vt == VT_NULL && pKanaFieldDef->m_vaValue.vt == VT_NULL && pGuideCFieldDef->m_vaValue.vt == VT_NULL)
					bIsBlank = TRUE;

				// 一番上の行が空になっていることはありえない。
				if( i == 1 && bIsBlank )
				{
#ifndef FOR_ASEAN // Bug 11440対応(国内版は看板パターンが「パターンなし:0」や「未設定:99」のときのみ冒頭の空を許さない。)
					if( !pGuidePatternFieldDef || lGuidePattern == 0 || lGuidePattern == 99)
#endif // FOR_ASEAN
					{
						if( !m_bNoComment )
						{
							strMsg.Format( AheLoadString( IDS_BLANKFIRSTROW ), strInfTableName, lOID );
							AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
						}
						bRet = FALSE;
						bCurrentRet = FALSE;
						break;					
					}
				}
			}

			// ここからは同じ流入リンクから出ている方面案内が同じ看板をもっているかどうかを調べる
			if( pGuidePatternFieldDef )
			{
				LONG lCurrentInLinkID = GetDirGuideInLinkID( &(*it) );			// 現在セットされている方面案内の流入リンクIDを取得する
				LONG lCurrentDirGuideOID  = GetDirGuideOID( &(*it) );					// 現在セットされている方面案内のOIDを取得する

				if( lCurrentInLinkID < 0 )
				{
					// 流入リンクIDが見つからなかった場合
					ATLASSERT( FALSE );
					// ワーニングメッセージを出してください
					return FALSE;
				}

				// 現在セットされている方面案内の最終ポイントを取得する
				IPointPtr ipCurrentFirstPoint;
				IPolylinePtr ipCurrentDirGuideGeom = (*it).GetShape();
				if( ipCurrentDirGuideGeom != NULL )	ipCurrentDirGuideGeom->get_FromPoint( &ipCurrentFirstPoint );

				// 現在セットされている方面案内の流入リンクのFROM、TOのどちらが ipCurrentFirstPoint なのかを調べる
				IFeaturePtr ipRoadFeature = GetRoadFeature( &(*it), lCurrentInLinkID ); // 現在セットされている方面案内の流入リンクを取得する

				BOOL bIsEqualFrom = FALSE;
				if( ipRoadFeature  )
				{
					IGeometryPtr ipGeom;
					IPointPtr ipFromPoint, ipToPoint;

					ipRoadFeature->get_Shape( &ipGeom );
					if( !ipGeom )
					{
						ATLASSERT( ipGeom != NULL );
						return FALSE;
					}
					((IPolylinePtr)ipGeom)->get_FromPoint( &ipFromPoint );
					((IPolylinePtr)ipGeom)->get_ToPoint( &ipToPoint );
					
					if( AheIsEqual( ipFromPoint, ipCurrentFirstPoint ) )
						bIsEqualFrom = TRUE;
					else if( AheIsEqual( ipToPoint, ipCurrentFirstPoint ) )
						bIsEqualFrom = FALSE;
					else
					{
						ATLASSERT(FALSE);
						return FALSE;
					}
				}

				// 流入ノードIDをコンボボックスに入れる用
				USES_CONVERSION;
				CComVariant vaFromID = AheGetAttribute( ipRoadFeature, FROM_FIELD );
				CComVariant vaToID   = AheGetAttribute( ipRoadFeature, TO_FIELD );
				vaFromID.ChangeType( VT_BSTR );
				vaToID.ChangeType( VT_BSTR );

				CString strInfoTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName );
				CString strLQTableName   = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoLQTableName );
				// vaLinkID と同じ ID の道路リンクに関連していて、且つその道路リンクが流入リンクである方面案内を探す
				list<FEATUREDEF*> listSameInFeature;

				// 現在表示されているリンク列のオーナ名を取得
				CString strCurrentOwnerName = AheGetFeatureClassOwnerName( it->GetFeature() );

				list<FEATUREDEF>* pFeatDefList = m_cControlRel.GetFeatureDefList();
				for( list<FEATUREDEF>::iterator itFeatureDef = pFeatDefList->begin(); itFeatureDef != pFeatDefList->end(); itFeatureDef++ )
				{
					// 現在表示されているリンク列のデータセットでなかったら、流入リンクIDにセットしない
					CString strOwnerName = AheGetFeatureClassOwnerName( itFeatureDef->GetFeature() );
					if( strOwnerName != strCurrentOwnerName ) 
						continue;

					if( lstrcmpi( itFeatureDef->GetTableName(), m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ) ) != 0 ) 
						continue;	// 方面案内以外は飛ばす

					CRelationalTableDef* pRelTableDef = itFeatureDef->GetRelationalTableDef( strLQTableName );
					if( !pRelTableDef ) 
						continue;

					for( CRelationalTableDef::iterator itLQRow = pRelTableDef->begin(); itLQRow != pRelTableDef->end(); itLQRow++ )
					{
						if( GetLongValue( &(*itLQRow), SEQUENCE_FIELD ) == 1 )
						{
							LONG lLinkID = GetLongValue( &(*itLQRow), LINK_ID_FIELD );
							if( lLinkID == lCurrentInLinkID )
							{
								IPointPtr ipFirstPoint;
								IPolylinePtr ipDirGuideGeom = itFeatureDef->GetShape();
								if( ipCurrentDirGuideGeom != NULL )	ipDirGuideGeom->get_FromPoint( &ipFirstPoint );

								// 流入ノードIDをセットする
								LPCTSTR lpcszID = NULL;
								if( ( bIsEqualFrom && AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) ) ||
									( !bIsEqualFrom && !AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) ) )
									lpcszID = OLE2T(vaFromID.bstrVal);
								else
									lpcszID = OLE2T(vaToID.bstrVal);

								// 同じ始点を持つもののみをグルーピングする
								if( AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) )
								{
									CFieldDef* pOIDCheckFieldDef = (*itFeatureDef).GetFieldDef( strInfTableName, _T("OBJECTID"));
									LONG lCheckOID = pOIDCheckFieldDef->m_vaValue.lVal;
									// あとで角度のチェックをするために、確保しておく
									listSameInFeature.push_back( &(*itFeatureDef) );

									if( lOID != lCheckOID )
									{

										CFieldDef* pCheckFieldDef = (*itFeatureDef).GetFieldDef(strInfTableName, _T("GUIDEPATTERN_C") );
										LONG lCheckGuidePattern = pCheckFieldDef->m_vaValue.lVal;
										if( lGuidePattern != lCheckGuidePattern )
										{
											if( !m_bNoComment )
											{
												strMsg.Format( AheLoadString( IDS_DIFFERENTDIRGUIDE ), strInfTableName, lOID, lCheckOID  );
												AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
												return FALSE;
											}
											bCurrentRet = FALSE;
										}
										CFieldDef* pCheckSeqFieldDef = (*itFeatureDef).GetFieldDef(strInfTableName, _T("SEQUENCE") );
										CComVariant vaCheckSeq = pCheckSeqFieldDef->m_vaValue;
#ifndef FOR_ASEAN // Bug 11268対応 ASEAN以外の場合はシーケンスチェックを行う
										if( !(lGuidePattern == 0 || lGuidePattern ==99) && vaCheckSeq == vaSequence )
										{
											m_bSequenceError = true;
											if( !m_bNoComment )
											{
												strMsg.Format( AheLoadString( IDS_SAMESEQUENCE ), strInfTableName, lOID, lCheckOID  );
												AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
												return FALSE;
											}
											bCurrentRet = FALSE;
										}
#endif // FOR_ASEAN
										break;
									}
								}
							}
						}
					}
				}

				// シーケンスの順番を最終チェック
				// 3方向ある場合
				if( ( lGuidePattern == 1 || lGuidePattern == 2 || lGuidePattern == 3 ) && listSameInFeature.size() == 3 )
				{
					list<CLevelOID>			listCLevelOID;	//!< 同じ流入リンクを持つ方面案内格納用
					// 角度でソート
					SortByAngle( &(*it), lCurrentInLinkID, listSameInFeature, listCLevelOID );
					list<double> listAngle;
					double dCheckAngle = 0;

					// 角度をリストにいれ、現在チェック中の角度を取得
					for( list<CLevelOID>::iterator itAngle = listCLevelOID.begin(); itAngle != listCLevelOID.end(); itAngle++ )
					{
						double dAngle = itAngle->m_lAngle;
						LONG lID = itAngle->m_lOID;
						if( lID == lOID )
							dCheckAngle = dAngle;

						listAngle.push_back(dAngle);
					}
					// 角度でソート
					listAngle.sort();
					LONG lAngleNumber=0;
					// 半時計回りに現在チェック中の角度が何番目かを数える
					for( list<double>::iterator itAngle1 = listAngle.begin(); itAngle1 != listAngle.end(); itAngle1++ )
					{
						if( dCheckAngle == *itAngle1 )
							break;
						lAngleNumber++;
					}

					// シーケンスと順番がちゃんとあっているかどうか調べる
					if( !(( vaSequence.lVal == 0 && lAngleNumber == 1 ) || ( vaSequence.lVal == 1 && lAngleNumber == 0 ) || ( vaSequence.lVal == 2 && lAngleNumber == 2 )) )
					{
						m_bSequenceError = true;
						if( !m_bNoComment )
						{
							strMsg.Format( AheLoadString( IDS_MISTAKESEQUENCE ), strInfTableName, lOID );
							int iRet = AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_YESNO, WARNLEVEL_HIGH );
							if( iRet == IDNO )
								return FALSE;
						}
						bCurrentRet = FALSE;
					}
				}
				// 2方向の場合
				else if( ( lGuidePattern == 4 || lGuidePattern == 5 || lGuidePattern == 6 || lGuidePattern == 7 ) && listSameInFeature.size() == 2 )
				{
					list<CLevelOID>			listCLevelOID;	//!< 同じ流入リンクを持つ方面案内格納用
					// 角度でソート
					SortByAngle( &(*it), lCurrentInLinkID, listSameInFeature, listCLevelOID );
					list<double> listAngle;
					double dCheckAngle = 0;
					// 角度をリストにいれ、現在チェック中の角度を取得
					for( list<CLevelOID>::iterator itAngle = listCLevelOID.begin(); itAngle != listCLevelOID.end(); itAngle++ )
					{
						double dAngle = itAngle->m_lAngle;
						LONG lID = itAngle->m_lOID;
						if( lID == lOID )
							dCheckAngle = dAngle;

						listAngle.push_back(dAngle);
					}
					listAngle.sort();
					LONG lAngleNumber=0;
					// 半時計回りに現在チェック中の角度が何番目かを数える
					for( list<double>::iterator itAngle1 = listAngle.begin(); itAngle1 != listAngle.end(); itAngle1++ )
					{
						if( dCheckAngle == *itAngle1 )
							break;
						lAngleNumber++;
					}

					// 右T字路だけは順番が逆になるので場合わけ
					if( lGuidePattern != 5 )
					{
						// シーケンスと順番がちゃんとあっているかどうか調べる
						if( !(( vaSequence.lVal == 0 && lAngleNumber == 0 ) || ( vaSequence.lVal == 1 && lAngleNumber == 1 ))  )
						{
							m_bSequenceError = true;
							if( !m_bNoComment )
							{
								strMsg.Format( AheLoadString( IDS_MISTAKESEQUENCE ), strInfTableName, lOID );
								int iRet = AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_YESNO, WARNLEVEL_HIGH );
								if( iRet == IDNO )
									return false;
							}
							bCurrentRet = FALSE;
						}
					}
					else
					{
						// シーケンスと順番がちゃんとあっているかどうか調べる
						if( !(( vaSequence.lVal == 0 && lAngleNumber == 1 ) || ( vaSequence.lVal == 1 && lAngleNumber == 0 ))  )
						{
							m_bSequenceError = true;
							if( !m_bNoComment )
							{
								strMsg.Format( AheLoadString( IDS_MISTAKESEQUENCE ), strInfTableName, lOID );
								int iRet = AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_YESNO, WARNLEVEL_HIGH );
								if( iRet == IDNO )
									return false;
							}
							bCurrentRet = FALSE;
						}
					}
				}
			}
		}
		if( bCurrentRet == TRUE )
			it->m_bError = false;
		else
			it->m_bError = true;
	}
	
	return bRet;			
}

BOOL CLQAttrDirGuideDlg::SetCtrlColorChange(HWND hTargetWND)
{
	foreach( const CONTROL_ID_SET& idset, m_vectorControlIDSet )
	{
		CButton cButton = GetDlgItem( idset.nRadioID );
		if(cButton.GetCheck())
		{
			if(GetDlgItem(idset.nClassID ) == hTargetWND || GetDlgItem( idset.nKanjiID ) == hTargetWND || GetDlgItem( idset.nYomiID ) == hTargetWND)
			{
				return TRUE;
			}
			return FALSE;
		}
	}
	return FALSE;
}

void CLQAttrDirGuideDlg::SetToolTip(INT nTargetItem, BOOL bIsChange)
{
	// ToolInfo構造体 
	static TOOLINFO ti;
	if( !bIsChange )
	{
		// エディットボックスにかかれている文字列を取得
		CEdit edit( GetDlgItem(nTargetItem) );
		INT nTextLen = edit.GetWindowTextLength() + 1;
		LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
		edit.GetWindowText( lpszText, nTextLen );

		for( INT i = 0; i < 20; i++ )
		{
			if( nTargetItem == GetHoumenID(i+1) )
			{
				if( m_HWNDHoumen[i] != NULL )
				{
					InitToolTip( nTargetItem, &ti );
					::SendMessage( m_HWNDHoumen[i], TTM_GETTOOLINFO, 0, (LPARAM)&ti );
					ti.lpszText = lpszText;

					::SendMessage( m_HWNDHoumen[i], TTM_SETTOOLINFO, 0, (LPARAM)&ti );
					return;
				}
			}
		}

		// まだツールチップ作成されていなかったら作成する（一番最初のみ入ってくる）
		HWND hToolTip;
		hToolTip =
			CreateWindowEx(
			0,				//拡張ウィンドウスタイル
			TOOLTIPS_CLASS,	//クラスネーム
			NULL,			//ウィンドウネーム
			TTS_ALWAYSTIP,	// ウィンドウスタイル
			CW_USEDEFAULT,	// Ｘ座標
			CW_USEDEFAULT,	// Ｙ座標
			CW_USEDEFAULT,	// 幅
			CW_USEDEFAULT,	// 高さ
			GetDlgItem(nTargetItem),// 親ウィンドウのハンドル
			NULL,			// メニューハンドル
			NULL,			// インスタンスハンドル
			NULL);			// WM_CREATEデータ

		InitToolTip( nTargetItem, &ti );

		ti.lpszText = lpszText;
		for( int i = 0; i < 20; i++ )
		{
			if( nTargetItem == GetHoumenID(i+1) )
			{
				m_HWNDHoumen[i] = hToolTip;
				::SendMessage( m_HWNDHoumen[i] , TTM_ADDTOOL , 0 , (LPARAM)&ti);
				::SendMessage( m_HWNDHoumen[i], TTM_SETDELAYTIME, TTDT_AUTOPOP, 32000);
				m_CwndKanji = m_HWNDHoumen[i];
				m_CwndKanji.SetFont( m_hFont );
				return;
			}
		}
	}
	else
	{
		// エディットボックスにかかれている文字列を取得
		CEdit edit( GetDlgItem(nTargetItem) );
		INT nTextLen = edit.GetWindowTextLength() + 1;
		LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
		edit.GetWindowText( lpszText, nTextLen );

		for( INT i = 0; i < 20; i++ )
		{
			if( nTargetItem == GetHoumenID(i+1) )
			{
				if( m_HWNDHoumen[i] )
				{
					InitToolTip( nTargetItem, &ti );
					::SendMessage( m_HWNDHoumen[i], TTM_GETTOOLINFO, 0, (LPARAM)&ti );
					ti.lpszText = lpszText;

					::SendMessage( m_HWNDHoumen[i], TTM_SETTOOLINFO, 0, (LPARAM)&ti );
					return;
				}
			}
		}
	}
}

void CLQAttrDirGuideDlg::InitToolTip(INT nTargetItem, TOOLINFO *ti)
{
	ZeroMemory(&(*ti) , sizeof(TOOLINFO)) ; 
	(*ti).cbSize = sizeof(TOOLINFO) ; 
	(*ti).uFlags = TTF_IDISHWND | TTF_SUBCLASS; 
	(*ti).hwnd = GetDlgItem(nTargetItem);
	(*ti).uId = (UINT_PTR)GetDlgItem(nTargetItem).m_hWnd;
}

/**
 * @fn INT CLQAttrDirGuideDlg:GetSelectedIndex(void)
 * @brief 選択されている（ラジオボタンがONになっている）行のインデックスを返します。
 *
 * @retval 0～9:選択されている行のインデックス
 * @retval -1：選択されていない
 */
INT CLQAttrDirGuideDlg::GetSelectedIndex(void)
{
	ATLASSERT( m_vectorControlIDSet.size() == 10 );

	for(INT i = 0; i < 10; ++i)
	{
		CButton cButton(GetDlgItem( m_vectorControlIDSet[i].nRadioID));
		if( cButton.GetCheck() )
		{
			return i;
		}
	}
	return -1;
}

/**
 * @fn INT CLQAttrDirGuideDlg::GetValidGuideCount(void)
 * @brief 有効な（方面種別が<null>でない）行の数を返します。
 * @brief 但し、途中の<null>はカウント上は有効とみなす。
 *
 * @retval 0～10	有効な行（データ）数
 */
INT CLQAttrDirGuideDlg::GetValidGuideCount(void)
{
	ATLASSERT( m_vectorControlIDSet.size() == 10 );

	for(INT i = 9; i >= 0; --i)
	{
		//編集段階で途中に<null>を入力しても、その時点では編集できるように
		//後ろから走査して、最初に<null>じゃなくなったところまでを有効とする
		CComboBox cCombo(GetDlgItem( m_vectorControlIDSet[i].nClassID ));
		INT nTextLen = cCombo.GetWindowTextLength()+1;
		CString strText;
		cCombo.GetWindowText( strText.GetBuffer( nTextLen ), nTextLen );
		if(strText != NULL_VALUE )
		{
			return i+1;
		}
	}
	return 0;
}

/**
 * @fn BOOL CLQAttrDirGuideDlg::CopyGuideItems(INT nTo, INT nFrom)
 * @brief 指定した行から指定した行へ内容をコピーします。
 *
 * @param nTo	[in]	コピー先の行番号（0～9）
 * @param nFrom	[in]	コピー元の行番号（0～9）
 *
 * @retval TRUE		成功
 * @retval FALSE	失敗
 */
BOOL CLQAttrDirGuideDlg::CopyGuideItems(INT nTo, INT nFrom)
{
	ATLASSERT( m_vectorControlIDSet.size() == 10 );

	if(nTo == nFrom || nTo < 0 || m_vectorControlIDSet.size() < nTo || nFrom < 0 || m_vectorControlIDSet.size() < nFrom)
	{
		//使い方間違っていなければここは通らないが
		return FALSE;	//成功と言えなくもないが、処理自体が行われなかったと言うことで
	}
	//方面種別
	CComboBox cCombo(GetDlgItem( m_vectorControlIDSet[nFrom].nClassID ));
	INT nSel = cCombo.GetCurSel();
	//方面案内（漢字）
	CEdit editKanji( GetDlgItem( m_vectorControlIDSet[nFrom].nKanjiID) );
	INT nTextLen = editKanji.GetWindowTextLength() + 1;
	LPTSTR lpszKanjiText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	editKanji.GetWindowText( lpszKanjiText, nTextLen );
	//方面案内（読み）
	CEdit editYomi( GetDlgItem( m_vectorControlIDSet[nFrom].nYomiID) );
	nTextLen = editYomi.GetWindowTextLength() + 1;
	LPTSTR lpszYomiText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	editYomi.GetWindowText( lpszYomiText, nTextLen );

	return SetGuidItems(nTo, nSel, lpszKanjiText, lpszYomiText);
}

/**
 * @fn BOOL CLQAttrDirGuideDlg::SetGuidItems(INT nIndex, INT nClass, LPCTSTR strKanji, LPCTSTR strYomi)
 * @brief 指定した行の方面種別・漢字・読みを設定します。
 *
 * @param nIndex	[in]	設定する行番号（0～9）
 * @param nClass	[in]	方面種別
 * @param strKanji	[in]	方面案内（漢字）
 * @param strYomi	[in]	方面案内（読み）
 *
 * @retval TRUE		成功
 * @retval FALSE	失敗
 */
BOOL CLQAttrDirGuideDlg::SetGuidItems(INT nIndex, INT nClass, LPCTSTR strKanji, LPCTSTR strYomi)
{
	ATLASSERT( m_vectorControlIDSet.size() == 10 );

	CComboBox cCombo(GetDlgItem( m_vectorControlIDSet[nIndex].nClassID ));
	if(nClass < 0)
	{
		//苦しいけど、「最後」を選択
		nClass = cCombo.GetCount()-1;
	}
#ifdef _TEST
	//使い方が悪い（コード値で与えなきゃいけない？）のかうまく行かないので変更保留
	CControlDef* pControlDef = m_cControlRel.GetControlDef( GetDlgItem( m_vectorControlIDSet[nIndex].nClassID ));
	CComVariant vaValue = nClass;
	pControlDef->SetValue(vaValue);
	pControlDef->SetComboBox(vaValue.intVal);
#else
	cCombo.SetCurSel(nClass);
	//これしないと、変更イベントが送信されない。しかもWM_COMMANDの時はちょっと厄介
	SendMessage(WM_COMMAND, (WPARAM)((CBN_SELCHANGE << 16) | (INT)m_vectorControlIDSet[nIndex].nClassID), 0);
#endif
	CEdit editKanji( GetDlgItem( m_vectorControlIDSet[nIndex].nKanjiID) );
	editKanji.SetWindowText(strKanji);
	CEdit editYomi( GetDlgItem( m_vectorControlIDSet[nIndex].nYomiID) );
	editYomi.SetWindowText(strYomi);

	return TRUE;
}

/**
 * @fn BOOL CLQAttrDirGuideDlg::SwapGuideItems( INT nIndex1, INT nIndex2 )
 * @brief 二つの行の内容を入れ替えます。
 *
 * @param nIndex1	[in]	入れ替えるデータの行番号（0～9）１
 * @param nIndex2	[in]	入れ替えるデータの行番号（0～9）２
 *
 * @retval TRUE		成功
 * @retval FALSE	失敗
 */
BOOL CLQAttrDirGuideDlg::SwapGuideItems(INT nIndex1, INT nIndex2)
{
	ATLASSERT( m_vectorControlIDSet.size() == 10 );

	if(nIndex1 == nIndex2 || nIndex1 < 0 || m_vectorControlIDSet.size() < nIndex1 || nIndex2 < 0 || m_vectorControlIDSet.size() < nIndex2)
	{
		//使い方間違っていなければここは通らないが
		return FALSE;	//成功と言えなくもないが、処理自体が行われなかったと言うことで
	}
	//方面種別
	CComboBox cCombo(GetDlgItem( m_vectorControlIDSet[nIndex1].nClassID ));
	INT nSel = cCombo.GetCurSel();
	//方面案内（漢字）
	CEdit editKanji( GetDlgItem( m_vectorControlIDSet[nIndex1].nKanjiID) );
	INT nTextLen = editKanji.GetWindowTextLength() + 1;
	LPTSTR lpszKanjiText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	editKanji.GetWindowText( lpszKanjiText, nTextLen );
	//方面案内（読み）
	CEdit editYomi( GetDlgItem( m_vectorControlIDSet[nIndex1].nYomiID) );
	nTextLen = editYomi.GetWindowTextLength() + 1;
	LPTSTR lpszYomiText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	editYomi.GetWindowText( lpszYomiText, nTextLen );

	CopyGuideItems(nIndex1, nIndex2);
	return SetGuidItems(nIndex2, nSel, lpszKanjiText, lpszYomiText);
}

/**
 * @fn void CLQAttrDirGuideDlg::UIUpdate( void )
 * @brief ボタンや表示欄の有効・無効を調整します（エディットボックスは編集可能時のみ独自で調整）
 */
void CLQAttrDirGuideDlg::UIUpdate(void)
{
	INT nValidCount = GetValidGuideCount();
	INT nSelected = GetSelectedIndex();
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && m_bEditable )
	{
		BOOL bEnable = (0 < nValidCount && nValidCount < 10 && 0 <= nSelected && nSelected < nValidCount) ? TRUE : FALSE;
		SetDlgStatus( IDC_BUTTON_INS, bEnable );

		bEnable = (nValidCount > 0 && 0 <= nSelected && nSelected < nValidCount) ? TRUE : FALSE;
		SetDlgStatus(IDC_BUTTON_DEL, bEnable);

		bEnable = (nValidCount > 1 && 0 < nSelected && nSelected < nValidCount) ? TRUE : FALSE;
		SetDlgStatus(IDC_BUTTON_UP, bEnable);

		bEnable = (nValidCount > 1 && 0 <= nSelected && nSelected < nValidCount-1) ? TRUE : FALSE;
		SetDlgStatus(IDC_BUTTON_DN, bEnable);

		//編集開始されている時は、有効範囲がデータ数+1（新規用行）まで
		INT i = 0;
		foreach( const CONTROL_ID_SET& idset, m_vectorControlIDSet )
		{
			BOOL bEnable = ( i <= nValidCount ) ? TRUE : FALSE;
			SetDlgStatus(idset.nRadioID, bEnable);
			SetDlgStatus(idset.nClassID, bEnable);
			SetDlgStatus(idset.nKanjiID, bEnable, TRUE);
			SetDlgStatus(idset.nYomiID, bEnable, TRUE);
			++i;
		}
	}
	else
	{
		//編集開始されていない時は、ラジオボタンの有効範囲がデータ数まで。その他は、上位で制御（？）
		INT i = 0;
		foreach( const CONTROL_ID_SET& idset, m_vectorControlIDSet )
		{
			SetDlgStatus(idset.nRadioID,  ( i < nValidCount ) ? TRUE : FALSE);
			++i;
		}

		SetDlgStatus(IDC_BUTTON_INS, FALSE);
		SetDlgStatus(IDC_BUTTON_DEL, FALSE);
		SetDlgStatus(IDC_BUTTON_UP, FALSE);
		SetDlgStatus(IDC_BUTTON_DN, FALSE);
	}
	InvalidateRect( NULL, FALSE );
	UpdateWindow();
}
