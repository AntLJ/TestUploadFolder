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

// LQAttrDirGuideDlg.cpp : CLQAttrDirGuideDlg �̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LQAttrDirGuideDlg.h"

namespace {
#ifndef FOR_ASEAN // Bug 11268�Ή�(���̐ݒ��ς���)
	int HOUMEN_WIDTH = 85;
	int KANJI_PERCENT = 25;
#else
	int HOUMEN_WIDTH = 115;
	int KANJI_PERCENT = 35;
#endif // FOR_ASEAN
}
/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i���ʈē��p�j
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
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CLQAttrDirGuideDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrDirGuideDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDirGuideDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CLQAttrDefaultDlg::CLQAttrDefaultDlg()
 * @brief �R���X�g���N�^
 *
 * �t�H���g�ݒ�Abitmap�̓ǂݍ��݂��s���܂��B
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
	// �t�H���g�쐬
	m_hFont = CreateFont(
		18, 0, 0, 0, 100,                           // �����C���C�w�p�x�C�x�p�x�C����
		FALSE, FALSE, FALSE,                        // �ΆC�����C�ŏ���
		SHIFTJIS_CHARSET,                           // �L�����N�^�ݒ�
		OUT_DEFAULT_PRECIS,                         // �o�͐��x
		CLIP_DEFAULT_PRECIS,                        // �N���b�v���x
		DEFAULT_QUALITY,                            // �i��
		FIXED_PITCH | FF_DONTCARE,                // �s�b�`
		AheLoadString(IDS_FONT));                       // �t�H���g���

	// �{�^���̃r�b�g�}�b�v�ǂݍ���
	m_hStraightBmp   = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_STRAIGHT));
	m_hStraightGBmp  = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_STRAIGHTG));
	m_hRightTurnBmp  = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_RIGHTTURN));
	m_hRightTurnGBmp = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_RIGHTTURNG));
	m_hLeftTurnBmp   = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_LEFTTURN));
	m_hLeftTurnGBmp  = ::LoadBitmap(_AtlBaseModule.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_LEFTTURNG));

#ifndef FOR_ASEAN // Bug 11268�Ή�
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
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * LQATTR_COMMON_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CLQAttrDirGuideDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// �_�C�A���O�̍쐬��LQATTR_COMMON_BASE_CLASS�ɔC���܂�
	m_hChildWnd = LQATTR_COMMON_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrDirGuideDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CLQAttrDirGuideDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}


/**
 * @fn BOOL CLQAttrDirGuideDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CLQAttrDirGuideDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	//�O��̏�Ԃɂ���āA�R���{�{�b�N�X�̑I�������O���[�A�E�g�������p�i�����������􂩂͕s���j
	if( bEditable )
	{
		for(INT i = 0; i < 10; ++i)
		{
			SetDlgStatus(m_vectorControlIDSet[i].nClassID, TRUE);
		}
	}
	//�O��̏�Ԃɂ���āA�R���{�{�b�N�X�̑I�������O���[�A�E�g�������p�i�����������􂩂͕s���j�@�����܂�
	m_bSetCurrentFeatureDefIndex = true;
    BOOL bRet = LQATTR_COMMON_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );
	m_bSetCurrentFeatureDefIndex = false;

	m_bEditable = bEditable;
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		::SetFocus( GetDlgItem(IDC_COMBO_HOUMENSHUBETU1) );

#ifndef FOR_ASEAN // Bug 11268�Ή�
		CLQRowDef *cLQRowDef = GetCurrentFeatureDef();
		CFieldDef* pSeqFieldDef = cLQRowDef->GetFieldDef( 	m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ), _T("SEQUENCE"));

		if( !pSeqFieldDef )
			SetDlgStatus( IDC_COMBO_SEQUENCE, FALSE );

		if( m_lLastLink == 1 )
			SetDlgStatus( IDC_COMBO_SEQUENCE, FALSE );
#else
		// ASEAN�̏ꍇ�͏�ɃV�[�P���X�̓O���[�A�E�g
		SetDlgStatus( IDC_COMBO_SEQUENCE, FALSE );
#endif // FOR_ASEAN
	}
	else
		::SendMessage( GetDlgItem(IDC_COMBO_HOUMENSHUBETU1), WM_KILLFOCUS, NULL, NULL );

	// ���ʈē��p�̃{�^���쐬
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
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CLQAttrDirGuideDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		CString strInfTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName );
		m_cControlRel.SetHWND( m_hWnd );
		// ���ʈē��p
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
/// ���݃Z�b�g����Ă�����ʈē���OID���擾����
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
/// ���݃Z�b�g����Ă�����ʈē��̗��������NID���擾����
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
/// �w�肳�ꂽ���[���̗��o�����NID���擾����
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

	// TOC�ɂ��邷�ׂẴW�I�t�B�[�`�����C�����擾���ă��[�v�񂵂��ړ��Ă̓��H�����N�擾����
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
				// ���H�����N���A�I�[�i���������łȂ��Ƃ����Ȃ�
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
		// LQ�Ɠ������[�N�X�y�[�X�̓��H�����N�t�B�[�`���N���X���擾
		IFeatureClassPtr ipRoadClass = GetRoadLinkWithLQ( pRowDef );
		// ���������N�t�B�[�`���擾
		// �ҏW�J�n�O�͕ҏW�L���b�V�����Ƃ�Ȃ��̂ŁA�S���������ق����͂₢�̂ŕҏW�J�n�O�J�n��ŏꍇ�킯
		if( IsStartEditing() )
		{
			if( ipRoadClass )
				ipRoadClass->GetFeature( lOID, &ipFeature );
		}
		else{
			// ���H�����N�e�[�u������N�G���������ĒT��
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
	LONG lCurrentInLinkID = GetDirGuideInLinkID( pFeatDef );			// ���݃Z�b�g����Ă�����ʈē��̗��������NID���擾����
	LONG lCurrentDirGuideOID  = GetDirGuideOID( pFeatDef );					// ���݃Z�b�g����Ă�����ʈē���OID���擾����

	if( lCurrentInLinkID < 0 )
	{
		// ���������NID��������Ȃ������ꍇ
		ATLASSERT( FALSE );
		// ���[�j���O���b�Z�[�W���o���Ă�������
		return;
	}

	// ���݃Z�b�g����Ă�����ʈē��̍ŏI�|�C���g���擾����
	IPointPtr ipCurrentFirstPoint;
	IPolylinePtr ipCurrentDirGuideGeom = pFeatDef->GetShape();
	if( ipCurrentDirGuideGeom )	
		ipCurrentDirGuideGeom->get_FromPoint( &ipCurrentFirstPoint );

	// ���݃Z�b�g����Ă�����ʈē��̗��������N��FROM�ATO�̂ǂ��炪 ipCurrentFirstPoint �Ȃ̂��𒲂ׂ�
	IFeaturePtr ipRoadFeature = GetRoadFeature( pFeatDef, lCurrentInLinkID ); // ���݃Z�b�g����Ă�����ʈē��̗��������N���擾����

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

	// �����m�[�hID���R���{�{�b�N�X�ɓ����p
	CComboBox comboNode( GetDlgItem(IDC_COMBO_INNODEID2) );			// �����m�[�hID�p�R���{�{�b�N�X

	comboNode.ResetContent();
	CComVariant vaFromID = AheGetAttribute( ipRoadFeature, FROM_FIELD );
	CComVariant vaToID   = AheGetAttribute( ipRoadFeature, TO_FIELD );
	vaFromID.ChangeType( VT_BSTR );
	vaToID.ChangeType( VT_BSTR );

	CString strInfoTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName );
	CString strLQTableName   = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoLQTableName );
	// vaLinkID �Ɠ��� ID �̓��H�����N�Ɋ֘A���Ă��āA�����̓��H�����N�����������N�ł�����ʈē���T��
	list<FEATUREDEF*> listSameInFeature;
	CComboBox combo(     GetDlgItem(IDC_COMBO_INLINKID) );			// ���������NID�p�R���{�{�b�N�X
	combo.ResetContent();
	m_listSameINFeature.clear();

	// ���ݕ\������Ă��郊���N��̃I�[�i�����擾
	CString strCurrentOwnerName = AheGetFeatureClassOwnerName( GetCurrentFeatureDef()->GetFeature() );

	list<FEATUREDEF>* pFeatDefList = m_cControlRel.GetFeatureDefList();
	for( list<FEATUREDEF>::iterator itFeatureDef = pFeatDefList->begin(); itFeatureDef != pFeatDefList->end(); itFeatureDef++ )
	{
		// ���ݕ\������Ă��郊���N��̃f�[�^�Z�b�g�łȂ�������A���������NID�ɃZ�b�g���Ȃ�
		CString strOwnerName = AheGetFeatureClassOwnerName( itFeatureDef->GetFeature() );
		if( strOwnerName != strCurrentOwnerName ) continue;

		if( lstrcmpi( itFeatureDef->GetTableName(), m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ) ) != 0 ) continue;	// ���ʈē��ȊO�͔�΂�

		CRelationalTableDef* pRelTableDef = itFeatureDef->GetRelationalTableDef( strLQTableName );
		if( pRelTableDef == NULL ) continue;

		for( CRelationalTableDef::iterator itLQRow = pRelTableDef->begin(); itLQRow != pRelTableDef->end(); itLQRow++ )
		{
			if( GetLongValue( &(*itLQRow), SEQUENCE_FIELD ) == 1 )
			{
				LONG lLinkID = GetLongValue( &(*itLQRow), LINK_ID_FIELD );
				CString strLinkID;

				// �R���{�{�b�N�X�ɗ��������NID��o�^
				strLinkID.Format(_T("%d"), lLinkID );
				INT nComboIndex = combo.FindString( 0, strLinkID );
				if( nComboIndex == CB_ERR )
					nComboIndex = combo.AddString( strLinkID );

				if( lLinkID == lCurrentInLinkID )
				{
					IPointPtr ipFirstPoint;
					IPolylinePtr ipDirGuideGeom = itFeatureDef->GetShape();
					if( ipCurrentDirGuideGeom != NULL )	ipDirGuideGeom->get_FromPoint( &ipFirstPoint );

					// �����m�[�hID���Z�b�g����
					LPCTSTR lpcszID = NULL;
					if( ( bIsEqualFrom == TRUE  && AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) ) ||
						( bIsEqualFrom == FALSE && AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) == FALSE ) )
						lpcszID = OLE2T(vaFromID.bstrVal);
					else
						lpcszID = OLE2T(vaToID.bstrVal);

					INT nPos = comboNode.FindString( 0, lpcszID );
					if( nPos == CB_ERR )
						nPos = comboNode.AddString( lpcszID );
					// �����n�_�������݂̂̂��O���[�s���O����
					if( AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) )
					{
						// ���݂̑I�����ڂƂ���
						combo.SetCurSel( nComboIndex );
						//
						comboNode.SetCurSel( nPos );
						// ���X�g�ɂԂ�����ł���
						listSameInFeature.push_back( &(*itFeatureDef) );
						break;
					}
				}
			}
		}
	}

	// �p�x�Ń\�[�g
	SortByAngle( GetCurrentFeatureDef(), lCurrentInLinkID, listSameInFeature, m_listSameINFeature );

	if( m_listSameINFeature.size() <= 0 ) 
	{
		ATLASSERT( m_listSameINFeature.size() > 0 );
		return;
	}

	// �����������ʈē��̐����{�^�����쐬����
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
		// �ǂ����̕��������f����
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


			// �ύX�O�̃V�[�P���X�擾
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

#ifndef FOR_ASEAN // Bug 11268�Ή�
			// �V�[�P���X�R���{�{�b�N�X�̒��g�̕ύX
			if( lGuidePattern == 0 || lGuidePattern == 99)
			{
				cSeqCombo.AddString( _T("<NULL>") );
				cSeqCombo.SetCurSel( 0 );

				// �R���{�{�b�N�X�̐F�𐧌�
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

				// �R���{�{�b�N�X�̐F�𐧌�
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

				// �R���{�{�b�N�X�̐F�𐧌�
				if( lSequence == lOriginalSequence )
					pSeqFieldDef->m_bChanged = FALSE;
				else
					pSeqFieldDef->m_bChanged = TRUE;

				cSeqCombo.SetCurSel( lSequence );
			}
#else
			// ASEAN���ł͗v��Ȃ��C�����邪SEQUENCE�̒l����NULL��NULL�ɕύX���ꂽ���Ƃ��l������(ta_suga)
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

	// lInLinkID �Ɠ��� ID �̓��H�����N�Ɋ֘A���Ă��āA�����̓��H�����N�����������N�ł�����ʂ�T��
	list<FEATUREDEF>* pFeatDefList = m_cControlRel.GetFeatureDefList();
	for( list<FEATUREDEF>::iterator itFeatureDef = pFeatDefList->begin(); itFeatureDef != pFeatDefList->end(); itFeatureDef++ )
	{
		if( lstrcmpi( itFeatureDef->GetTableName(), strInfoTableName ) != 0 ) continue;	// ���ʈȊO�͔�΂�

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
						// ���H�����N���擾
						IFeaturePtr ipRoadFeature = GetRoadFeature( GetCurrentFeatureDef(), lInLinkID );
						CComVariant vaFromID = AheGetAttribute( ipRoadFeature, FROM_FIELD );
						CComVariant vaToID   = AheGetAttribute( ipRoadFeature, TO_FIELD   );
						// �����m�[�hID��lInNodeID���������ǂ���
						if( ( ( itLQRow->GetFieldDef( LINKDIR_C_FIELD )->m_vaValue == CComVariant(1L)/* ������ */ ) && ( vaFromID.vt == VT_I4 && vaFromID.lVal == lInNodeID ) ) || 
							( ( itLQRow->GetFieldDef( LINKDIR_C_FIELD )->m_vaValue == CComVariant(2L)/* �t���� */ ) && ( vaToID.vt   == VT_I4 && vaToID.lVal   == lInNodeID ) ) )
						{
							// ���X�g�ɂԂ�����ł���
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

	// �p�x�Ń\�[�g
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

	// �r�b�g�}�b�v�����
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

	// ���������N�t�B�[�`���擾
	IFeaturePtr ipInRoadLink = GetRoadFeature( pRowDef, lStartLinkID );
	// [Bug2848]�Ή��̂��߂ɁAipInRoadLink��NULL���ǂ������ׂȂ��Ă͂Ȃ�Ȃ�
	// ���������N�ɂ́A�ʂ̃f�[�^�Z�b�g�̃����N�������Ă��Ă��܂����߁A
	if( !ipInRoadLink )
		return;

	IGeometryPtr ipInRoadLinkGeom;
	ipInRoadLink->get_ShapeCopy( &ipInRoadLinkGeom );
	
	// ���Ԗڂ̍\���_����͂��߂�΂悢�̂����ׂ�
	long lStart = -1;
	((IPointCollectionPtr)ipInRoadLinkGeom)->get_PointCount( &lStart );
	lStart -= 2;	// e.x. ���H�����N�̍\���_����3�Ȃ�1����i0�X�^�[�g�j
	_ASSERTE( lStart >= 0 );

	for( list<FEATUREDEF*>::iterator it = listOrg.begin(); it != listOrg.end(); ++it )
	{
		// �����N��`��ƍ\���_�����擾
		long lTotalPointCount = -1;
		IPointCollectionPtr ipLQGeometry = (*it)->GetShape();
		ipLQGeometry->get_PointCount( &lTotalPointCount );

		// �ŏI�����N�擾
		LONG lEndLinkID = GetDirGuideOutLinkID( *it );
		IFeaturePtr ipOutRoadLink = GetRoadFeature( pRowDef, lEndLinkID );
		if( !ipOutRoadLink )
			return;

		IGeometryPtr ipOutRoadLinkGeom;
		ipOutRoadLink->get_Shape( &ipOutRoadLinkGeom );

		// ���Ԗڂ̍\���_�����擾
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

	// ������������Ȃ�A����ɂ��̐�𒲂ׂ�
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
	// [lStart] - [lStart+1] �̃Z�O�����g��
	// [lEnd]   - [lEnd+1]   �̃Z�O�����g�̒[�_���������悤�ɕ��s�ړ����A�p�x�����߂�
	LONG lPointCount = -1;
	IPointCollectionPtr ipPoly( ipPolyline );
	if( !ipPoly )
		return -1;
	ipPoly->get_PointCount( &lPointCount );
	if( ! ( lPointCount > lStart + 1 && lPointCount > lEnd + 1 ) ) 
		return -1;

	// �x�N�g��AB�ƃx�N�g��CD�Ƃ���
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
	// ���s�ړ�����
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

			// ���ʈē��̂P�`�P�O�܂ł�����ׂ�
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
				// �Ԃɋ�̍s���Ȃ����ǂ�������ׂ�
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
#ifndef FOR_ASEAN // Bug 11268�Ή�
				// �������̂ƃJ�i���̂��o�������Ă��邩���͂ǂ���� NULL ���ǂ���
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
				// bug 11268�Ή� ASEAN�̓��͏���
				// �n���ʂ�NULL�Ȃ�Ί������ǂ݂�(��ꖼ�̂���񖼏̂�)NULL�łȂ���΂Ȃ�Ȃ�
				// �n���ʂ���NULL�Ȃ�΁A����(��ꖼ��)�ɒl�������Ă��Ȃ���΂Ȃ�Ȃ��B�ǂ�(��񖼏�)�͂ǂ����ł�����
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
				// ��̍s����������A�����Ńt���O�𗧂ĂĂ���
				if( pKanjiFieldDef->m_vaValue.vt == VT_NULL && pKanaFieldDef->m_vaValue.vt == VT_NULL && pGuideCFieldDef->m_vaValue.vt == VT_NULL)
					bIsBlank = TRUE;

				// ��ԏ�̍s����ɂȂ��Ă��邱�Ƃ͂��肦�Ȃ��B
				if( i == 1 && bIsBlank )
				{
#ifndef FOR_ASEAN // Bug 11440�Ή�(�����ł͊Ŕp�^�[�����u�p�^�[���Ȃ�:0�v��u���ݒ�:99�v�̂Ƃ��̂ݖ`���̋�������Ȃ��B)
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

			// ��������͓������������N����o�Ă�����ʈē��������Ŕ������Ă��邩�ǂ����𒲂ׂ�
			if( pGuidePatternFieldDef )
			{
				LONG lCurrentInLinkID = GetDirGuideInLinkID( &(*it) );			// ���݃Z�b�g����Ă�����ʈē��̗��������NID���擾����
				LONG lCurrentDirGuideOID  = GetDirGuideOID( &(*it) );					// ���݃Z�b�g����Ă�����ʈē���OID���擾����

				if( lCurrentInLinkID < 0 )
				{
					// ���������NID��������Ȃ������ꍇ
					ATLASSERT( FALSE );
					// ���[�j���O���b�Z�[�W���o���Ă�������
					return FALSE;
				}

				// ���݃Z�b�g����Ă�����ʈē��̍ŏI�|�C���g���擾����
				IPointPtr ipCurrentFirstPoint;
				IPolylinePtr ipCurrentDirGuideGeom = (*it).GetShape();
				if( ipCurrentDirGuideGeom != NULL )	ipCurrentDirGuideGeom->get_FromPoint( &ipCurrentFirstPoint );

				// ���݃Z�b�g����Ă�����ʈē��̗��������N��FROM�ATO�̂ǂ��炪 ipCurrentFirstPoint �Ȃ̂��𒲂ׂ�
				IFeaturePtr ipRoadFeature = GetRoadFeature( &(*it), lCurrentInLinkID ); // ���݃Z�b�g����Ă�����ʈē��̗��������N���擾����

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

				// �����m�[�hID���R���{�{�b�N�X�ɓ����p
				USES_CONVERSION;
				CComVariant vaFromID = AheGetAttribute( ipRoadFeature, FROM_FIELD );
				CComVariant vaToID   = AheGetAttribute( ipRoadFeature, TO_FIELD );
				vaFromID.ChangeType( VT_BSTR );
				vaToID.ChangeType( VT_BSTR );

				CString strInfoTableName = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName );
				CString strLQTableName   = m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoLQTableName );
				// vaLinkID �Ɠ��� ID �̓��H�����N�Ɋ֘A���Ă��āA�����̓��H�����N�����������N�ł�����ʈē���T��
				list<FEATUREDEF*> listSameInFeature;

				// ���ݕ\������Ă��郊���N��̃I�[�i�����擾
				CString strCurrentOwnerName = AheGetFeatureClassOwnerName( it->GetFeature() );

				list<FEATUREDEF>* pFeatDefList = m_cControlRel.GetFeatureDefList();
				for( list<FEATUREDEF>::iterator itFeatureDef = pFeatDefList->begin(); itFeatureDef != pFeatDefList->end(); itFeatureDef++ )
				{
					// ���ݕ\������Ă��郊���N��̃f�[�^�Z�b�g�łȂ�������A���������NID�ɃZ�b�g���Ȃ�
					CString strOwnerName = AheGetFeatureClassOwnerName( itFeatureDef->GetFeature() );
					if( strOwnerName != strCurrentOwnerName ) 
						continue;

					if( lstrcmpi( itFeatureDef->GetTableName(), m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ) ) != 0 ) 
						continue;	// ���ʈē��ȊO�͔�΂�

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

								// �����m�[�hID���Z�b�g����
								LPCTSTR lpcszID = NULL;
								if( ( bIsEqualFrom && AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) ) ||
									( !bIsEqualFrom && !AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) ) )
									lpcszID = OLE2T(vaFromID.bstrVal);
								else
									lpcszID = OLE2T(vaToID.bstrVal);

								// �����n�_�������݂̂̂��O���[�s���O����
								if( AheIsEqual( ipCurrentFirstPoint, ipFirstPoint ) )
								{
									CFieldDef* pOIDCheckFieldDef = (*itFeatureDef).GetFieldDef( strInfTableName, _T("OBJECTID"));
									LONG lCheckOID = pOIDCheckFieldDef->m_vaValue.lVal;
									// ���ƂŊp�x�̃`�F�b�N�����邽�߂ɁA�m�ۂ��Ă���
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
#ifndef FOR_ASEAN // Bug 11268�Ή� ASEAN�ȊO�̏ꍇ�̓V�[�P���X�`�F�b�N���s��
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

				// �V�[�P���X�̏��Ԃ��ŏI�`�F�b�N
				// 3��������ꍇ
				if( ( lGuidePattern == 1 || lGuidePattern == 2 || lGuidePattern == 3 ) && listSameInFeature.size() == 3 )
				{
					list<CLevelOID>			listCLevelOID;	//!< �������������N�������ʈē��i�[�p
					// �p�x�Ń\�[�g
					SortByAngle( &(*it), lCurrentInLinkID, listSameInFeature, listCLevelOID );
					list<double> listAngle;
					double dCheckAngle = 0;

					// �p�x�����X�g�ɂ���A���݃`�F�b�N���̊p�x���擾
					for( list<CLevelOID>::iterator itAngle = listCLevelOID.begin(); itAngle != listCLevelOID.end(); itAngle++ )
					{
						double dAngle = itAngle->m_lAngle;
						LONG lID = itAngle->m_lOID;
						if( lID == lOID )
							dCheckAngle = dAngle;

						listAngle.push_back(dAngle);
					}
					// �p�x�Ń\�[�g
					listAngle.sort();
					LONG lAngleNumber=0;
					// �����v���Ɍ��݃`�F�b�N���̊p�x�����Ԗڂ��𐔂���
					for( list<double>::iterator itAngle1 = listAngle.begin(); itAngle1 != listAngle.end(); itAngle1++ )
					{
						if( dCheckAngle == *itAngle1 )
							break;
						lAngleNumber++;
					}

					// �V�[�P���X�Ə��Ԃ������Ƃ����Ă��邩�ǂ������ׂ�
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
				// 2�����̏ꍇ
				else if( ( lGuidePattern == 4 || lGuidePattern == 5 || lGuidePattern == 6 || lGuidePattern == 7 ) && listSameInFeature.size() == 2 )
				{
					list<CLevelOID>			listCLevelOID;	//!< �������������N�������ʈē��i�[�p
					// �p�x�Ń\�[�g
					SortByAngle( &(*it), lCurrentInLinkID, listSameInFeature, listCLevelOID );
					list<double> listAngle;
					double dCheckAngle = 0;
					// �p�x�����X�g�ɂ���A���݃`�F�b�N���̊p�x���擾
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
					// �����v���Ɍ��݃`�F�b�N���̊p�x�����Ԗڂ��𐔂���
					for( list<double>::iterator itAngle1 = listAngle.begin(); itAngle1 != listAngle.end(); itAngle1++ )
					{
						if( dCheckAngle == *itAngle1 )
							break;
						lAngleNumber++;
					}

					// �ET���H�����͏��Ԃ��t�ɂȂ�̂ŏꍇ�킯
					if( lGuidePattern != 5 )
					{
						// �V�[�P���X�Ə��Ԃ������Ƃ����Ă��邩�ǂ������ׂ�
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
						// �V�[�P���X�Ə��Ԃ������Ƃ����Ă��邩�ǂ������ׂ�
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
	// ToolInfo�\���� 
	static TOOLINFO ti;
	if( !bIsChange )
	{
		// �G�f�B�b�g�{�b�N�X�ɂ�����Ă��镶������擾
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

		// �܂��c�[���`�b�v�쐬����Ă��Ȃ�������쐬����i��ԍŏ��̂ݓ����Ă���j
		HWND hToolTip;
		hToolTip =
			CreateWindowEx(
			0,				//�g���E�B���h�E�X�^�C��
			TOOLTIPS_CLASS,	//�N���X�l�[��
			NULL,			//�E�B���h�E�l�[��
			TTS_ALWAYSTIP,	// �E�B���h�E�X�^�C��
			CW_USEDEFAULT,	// �w���W
			CW_USEDEFAULT,	// �x���W
			CW_USEDEFAULT,	// ��
			CW_USEDEFAULT,	// ����
			GetDlgItem(nTargetItem),// �e�E�B���h�E�̃n���h��
			NULL,			// ���j���[�n���h��
			NULL,			// �C���X�^���X�n���h��
			NULL);			// WM_CREATE�f�[�^

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
		// �G�f�B�b�g�{�b�N�X�ɂ�����Ă��镶������擾
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
 * @brief �I������Ă���i���W�I�{�^����ON�ɂȂ��Ă���j�s�̃C���f�b�N�X��Ԃ��܂��B
 *
 * @retval 0�`9:�I������Ă���s�̃C���f�b�N�X
 * @retval -1�F�I������Ă��Ȃ�
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
 * @brief �L���ȁi���ʎ�ʂ�<null>�łȂ��j�s�̐���Ԃ��܂��B
 * @brief �A���A�r����<null>�̓J�E���g��͗L���Ƃ݂Ȃ��B
 *
 * @retval 0�`10	�L���ȍs�i�f�[�^�j��
 */
INT CLQAttrDirGuideDlg::GetValidGuideCount(void)
{
	ATLASSERT( m_vectorControlIDSet.size() == 10 );

	for(INT i = 9; i >= 0; --i)
	{
		//�ҏW�i�K�œr����<null>����͂��Ă��A���̎��_�ł͕ҏW�ł���悤��
		//��납�瑖�����āA�ŏ���<null>����Ȃ��Ȃ����Ƃ���܂ł�L���Ƃ���
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
 * @brief �w�肵���s����w�肵���s�֓��e���R�s�[���܂��B
 *
 * @param nTo	[in]	�R�s�[��̍s�ԍ��i0�`9�j
 * @param nFrom	[in]	�R�s�[���̍s�ԍ��i0�`9�j
 *
 * @retval TRUE		����
 * @retval FALSE	���s
 */
BOOL CLQAttrDirGuideDlg::CopyGuideItems(INT nTo, INT nFrom)
{
	ATLASSERT( m_vectorControlIDSet.size() == 10 );

	if(nTo == nFrom || nTo < 0 || m_vectorControlIDSet.size() < nTo || nFrom < 0 || m_vectorControlIDSet.size() < nFrom)
	{
		//�g�����Ԉ���Ă��Ȃ���΂����͒ʂ�Ȃ���
		return FALSE;	//�����ƌ����Ȃ����Ȃ����A�������̂��s���Ȃ������ƌ������Ƃ�
	}
	//���ʎ��
	CComboBox cCombo(GetDlgItem( m_vectorControlIDSet[nFrom].nClassID ));
	INT nSel = cCombo.GetCurSel();
	//���ʈē��i�����j
	CEdit editKanji( GetDlgItem( m_vectorControlIDSet[nFrom].nKanjiID) );
	INT nTextLen = editKanji.GetWindowTextLength() + 1;
	LPTSTR lpszKanjiText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	editKanji.GetWindowText( lpszKanjiText, nTextLen );
	//���ʈē��i�ǂ݁j
	CEdit editYomi( GetDlgItem( m_vectorControlIDSet[nFrom].nYomiID) );
	nTextLen = editYomi.GetWindowTextLength() + 1;
	LPTSTR lpszYomiText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	editYomi.GetWindowText( lpszYomiText, nTextLen );

	return SetGuidItems(nTo, nSel, lpszKanjiText, lpszYomiText);
}

/**
 * @fn BOOL CLQAttrDirGuideDlg::SetGuidItems(INT nIndex, INT nClass, LPCTSTR strKanji, LPCTSTR strYomi)
 * @brief �w�肵���s�̕��ʎ�ʁE�����E�ǂ݂�ݒ肵�܂��B
 *
 * @param nIndex	[in]	�ݒ肷��s�ԍ��i0�`9�j
 * @param nClass	[in]	���ʎ��
 * @param strKanji	[in]	���ʈē��i�����j
 * @param strYomi	[in]	���ʈē��i�ǂ݁j
 *
 * @retval TRUE		����
 * @retval FALSE	���s
 */
BOOL CLQAttrDirGuideDlg::SetGuidItems(INT nIndex, INT nClass, LPCTSTR strKanji, LPCTSTR strYomi)
{
	ATLASSERT( m_vectorControlIDSet.size() == 10 );

	CComboBox cCombo(GetDlgItem( m_vectorControlIDSet[nIndex].nClassID ));
	if(nClass < 0)
	{
		//�ꂵ�����ǁA�u�Ō�v��I��
		nClass = cCombo.GetCount()-1;
	}
#ifdef _TEST
	//�g�����������i�R�[�h�l�ŗ^���Ȃ��Ⴂ���Ȃ��H�j�̂����܂��s���Ȃ��̂ŕύX�ۗ�
	CControlDef* pControlDef = m_cControlRel.GetControlDef( GetDlgItem( m_vectorControlIDSet[nIndex].nClassID ));
	CComVariant vaValue = nClass;
	pControlDef->SetValue(vaValue);
	pControlDef->SetComboBox(vaValue.intVal);
#else
	cCombo.SetCurSel(nClass);
	//���ꂵ�Ȃ��ƁA�ύX�C�x���g�����M����Ȃ��B������WM_COMMAND�̎��͂�����Ɩ��
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
 * @brief ��̍s�̓��e�����ւ��܂��B
 *
 * @param nIndex1	[in]	����ւ���f�[�^�̍s�ԍ��i0�`9�j�P
 * @param nIndex2	[in]	����ւ���f�[�^�̍s�ԍ��i0�`9�j�Q
 *
 * @retval TRUE		����
 * @retval FALSE	���s
 */
BOOL CLQAttrDirGuideDlg::SwapGuideItems(INT nIndex1, INT nIndex2)
{
	ATLASSERT( m_vectorControlIDSet.size() == 10 );

	if(nIndex1 == nIndex2 || nIndex1 < 0 || m_vectorControlIDSet.size() < nIndex1 || nIndex2 < 0 || m_vectorControlIDSet.size() < nIndex2)
	{
		//�g�����Ԉ���Ă��Ȃ���΂����͒ʂ�Ȃ���
		return FALSE;	//�����ƌ����Ȃ����Ȃ����A�������̂��s���Ȃ������ƌ������Ƃ�
	}
	//���ʎ��
	CComboBox cCombo(GetDlgItem( m_vectorControlIDSet[nIndex1].nClassID ));
	INT nSel = cCombo.GetCurSel();
	//���ʈē��i�����j
	CEdit editKanji( GetDlgItem( m_vectorControlIDSet[nIndex1].nKanjiID) );
	INT nTextLen = editKanji.GetWindowTextLength() + 1;
	LPTSTR lpszKanjiText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	editKanji.GetWindowText( lpszKanjiText, nTextLen );
	//���ʈē��i�ǂ݁j
	CEdit editYomi( GetDlgItem( m_vectorControlIDSet[nIndex1].nYomiID) );
	nTextLen = editYomi.GetWindowTextLength() + 1;
	LPTSTR lpszYomiText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	editYomi.GetWindowText( lpszYomiText, nTextLen );

	CopyGuideItems(nIndex1, nIndex2);
	return SetGuidItems(nIndex2, nSel, lpszKanjiText, lpszYomiText);
}

/**
 * @fn void CLQAttrDirGuideDlg::UIUpdate( void )
 * @brief �{�^����\�����̗L���E�����𒲐����܂��i�G�f�B�b�g�{�b�N�X�͕ҏW�\���̂ݓƎ��Œ����j
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

		//�ҏW�J�n����Ă��鎞�́A�L���͈͂��f�[�^��+1�i�V�K�p�s�j�܂�
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
		//�ҏW�J�n����Ă��Ȃ����́A���W�I�{�^���̗L���͈͂��f�[�^���܂ŁB���̑��́A��ʂŐ���i�H�j
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
