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

// DirGuideSignDlg.h: CDirGuideSignDlg �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRGUIDESIGNDLG_H__62533650_12FE_4102_A674_DD25A5131675__INCLUDED_)
#define AFX_DIRGUIDESIGNDLG_H__62533650_12FE_4102_A674_DD25A5131675__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

#include <atlapp.h>                             // WTL�iatlgdi.h�Aatlcrack.h�j���g������
#include <atlgdi.h>                             // CDC                 �iWinMgr�Ŏg�p�j
#include <atlcrack.h>                           // ���b�Z�[�W�n
#include "../../LIB/SizingFramework/include/WinMgr.h"


class CDirGuideSignDlg : 
	public CAxDialogImpl<CDirGuideSignDlg>
{
public:
	CDirGuideSignDlg();
	virtual ~CDirGuideSignDlg();

	enum { IDD = IDD_DIRGUIDESIGNDLG };

BEGIN_MSG_MAP(CDirGuideSignDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DRAWITEM, OnCtlColorButton)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_CODE_HANDLER(BN_CLICKED, OnDirGuideButtonClick)
	COMMAND_CODE_HANDLER(BN_DBLCLK, OnDirGuideButtonDBLClick)
END_MSG_MAP()

	/**
	 * �_�C�A���O������������C�x���g�n���h��
	 */
	LRESULT CDirGuideSignDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CButton cButton;
		
		// TODO:���̕ӂ̓��\�[�X�ɖ��ߍ��߂Ȃ�������������(���r���[:4226�̎w�E�����Q��)
		// TODO:1000�Ƃ����ł��R�[�f�B���O�����邽�߁A�X�L�[�}�g���ȂǑΉ����l����(���r���[:4226�̎w�E�����Q��)
#ifndef FOR_ASEAN // Bug 11268�Ή�
		CRect rect( 0, 0, 160, 100 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1000 );

		rect.SetRect( 160, 0, 320, 100 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1001 );

		rect.SetRect( 320, 0, 480, 100 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1002 );

		rect.SetRect( 0, 100, 160, 200 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1003 );

		rect.SetRect( 160, 100, 320, 200 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1004 );

		rect.SetRect( 320, 100, 480, 200 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1005 );

		rect.SetRect( 0, 200, 160, 300 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1006 );

		rect.SetRect( 160, 200, 320, 300 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1007 );

		rect.SetRect( 320, 200, 480, 300 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1099 );
#else
		CRect rect( 0, 0, 160, 100 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1000 );

		rect.SetRect( 160, 0, 320, 100 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1051 );

		rect.SetRect( 320, 0, 480, 100 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1052 );

		rect.SetRect( 0, 100, 160, 200 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1053 );

		rect.SetRect( 160, 100, 320, 200 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1054 );

		rect.SetRect( 320, 100, 480, 200 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 1099 );
#endif // FOR_ASEAN


		return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
	}
	/**
	 * �{�^���̐F��ݒ�
	 *
	 * @note �G���[�l�F��
	 */
	LRESULT CDirGuideSignDlg::OnCtlColorButton(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		UINT nCtrlID = wParam;
		LPDRAWITEMSTRUCT lpDrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);

		RECT rect;
		::GetClientRect(GetDlgItem(nCtrlID), &rect);
		::Rectangle(lpDrawItem->hDC, rect.left, rect.top, rect.right, rect.bottom);

		HDC hmdc = CreateCompatibleDC(lpDrawItem->hDC);
		switch(nCtrlID)
		{
			case 1000:
				SelectObject(hmdc, m_hNoPatternBmp);
				break;
#ifndef FOR_ASEAN // Bug 11268�Ή�
			case 1001:
				SelectObject(hmdc, m_hTwoLevelCrossoverABmp);
				break;
			case 1002:
				SelectObject(hmdc, m_hTwoLevelCrossoverBBmp);
				break;
			case 1003:
				SelectObject(hmdc, m_hCrossRoadsBmp);
				break;
			case 1004:
				SelectObject(hmdc, m_hLeftForkedRoadBmp);
				break;
			case 1005:
				SelectObject(hmdc, m_hRightForkedRoadBmp);
				break;
			case 1006:
				SelectObject(hmdc, m_hYForkedRoadBmp);
				break;
			case 1007:
				SelectObject(hmdc, m_hTForkedRoadBmp);
				break;
#else
			case 1051:
				SelectObject(hmdc, m_hAseanGreenBmp);
				break;
			case 1052:
				SelectObject(hmdc, m_hAseanBlueBmp);
				break;
			case 1053:
				SelectObject(hmdc, m_hAseanWhiteBmp);
				break;
			case 1054:
				SelectObject(hmdc, m_hAseanOrangeBmp);
				break;
#endif // FOR_ASEAN
			case 1099:
				SelectObject(hmdc, m_hNullBmp);
				break;
			default:
				break;
		}

		BitBlt(lpDrawItem->hDC, 10, 5, rect.right+10, rect.bottom+5, hmdc, 0, 0, SRCCOPY);
		::DeleteDC(hmdc);
		return 0;
	}

	LRESULT OnDirGuideButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		ULONG ulNum = wID;
		m_lSelectedSign = ulNum;
		m_bOnece = TRUE;
		return 0;
	}

	LRESULT OnDirGuideButtonDBLClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		ULONG ulNum = wID;
		m_lSelectedSign = ulNum - 1000;

		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_lSelectedSign = -1;
		EndDialog(wID);
		return 0;
	}
public:

	inline LONG CDirGuideSignDlg::GetSelectedSign(){return m_lSelectedSign;}

private :
	LONG		m_lSelectedSign;
	BOOL		m_bOnece;
	// TODO:���̕ӂ�HBITMAP����CBitmap�ɕύX���邱�Ƃ���������(���r���[:4226�̎w�E�����Q��)
	HBITMAP		m_hNullBmp,m_hNoPatternBmp;
#ifndef FOR_ASEAN // Bug 11268�Ή�
	HBITMAP		m_hTwoLevelCrossoverABmp,m_hTwoLevelCrossoverBBmp,m_hCrossRoadsBmp,
				m_hLeftForkedRoadBmp,m_hRightForkedRoadBmp,m_hYForkedRoadBmp,m_hTForkedRoadBmp;
#else
	HBITMAP		m_hAseanGreenBmp, m_hAseanBlueBmp, m_hAseanWhiteBmp, m_hAseanOrangeBmp;
#endif // FOR_ASEAN
};

#endif // !defined(AFX_DIRGUIDESIGNDLG_H__62533650_12FE_4102_A674_DD25A5131675__INCLUDED_)
