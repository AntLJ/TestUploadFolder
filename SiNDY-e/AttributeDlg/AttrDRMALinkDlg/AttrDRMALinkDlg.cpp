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

// AttrDRMALinkDlg.cpp: CAttrDRMALinkDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrDRMALinkDlg.h"

///  �E�B���h�E�}�l�[�W���z�u�}�b�v�i��{�����^�u�p�j
BEGIN_WINDOW_MAP(DRMAKIHONMAP)
	BEGINROWS(WRCT_REST,IDC_TAB_DRMAATTR,RCMARGINS(2,2))
		RCSPACE(22)	// �^�u�̂܂ݕ�
		RCSPACE(30)	
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_DISPCLASS)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_DISPCLASS)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_MANAGER)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_MANAGER)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ROADWIDTH)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_ROADWIDTH)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LINKCLASS)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LINKCLASS1)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�ǉ������^�u�p�j
BEGIN_WINDOW_MAP(DRMATUIKAMAP)
	BEGINROWS(WRCT_REST,IDC_TAB_DRMAATTR,RCMARGINS(2,2))
		RCSPACE(22)	// �^�u�̂܂ݕ�
		RCSPACE(30)	
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_BYPASS)
				RCTOFIT(IDC_CHECK_CARONLYF)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LANE)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LANE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�iP_LINK�p�j
BEGIN_WINDOW_MAP(DRMAKOTUKISEIMAP)
	BEGINROWS(WRCT_REST,IDC_TAB_DRMAATTR,RCMARGINS(2,2))
		RCSPACE(22)	// �^�u�̂܂ݕ�
		RCSPACE(30)	
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_REGULER)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_REGULER)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LINKOK)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LINKOK)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�H���ԍ��^�u�p�j
BEGIN_WINDOW_MAP(DRMAROSENMAP)
	BEGINROWS(WRCT_REST,IDC_TAB_DRMAATTR,RCMARGINS(2,2))
		RCSPACE(22)	// �^�u�̂܂ݕ�
		RCSPACE(30)	
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(5,5))
				RCTOFIT(IDC_STATIC_ROADNUMBER)
				RCTOFIT(-1)
				RCTOFIT(IDC_EDIT_ROADNUMBER)
				RCTOFIT(IDC_STATIC_ROADNUMBER2)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,50)
			BEGINROWS(WRCT_REST, 0, RCMARGINS(5,5))
				RCREST(IDC_GRID_DUPLILINK)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�iDRMA�^�u�p�j
BEGIN_WINDOW_MAP(DRMADRMAMAP)
	BEGINROWS(WRCT_REST,IDC_TAB_DRMAATTR,RCMARGINS(2,2))
		RCSPACE(22)	// �^�u�̂܂ݕ�
		RCSPACE(30)	
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_KUBUNCODE)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_KUBUNCODE)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LIMITSPEED)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_LIMITSPEED)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT,0,20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_TRIPSPEED)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_TRIPSPEED)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrDRMALinkDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrDRMALinkDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDRMALinkDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrDRMALinkDlg::CAttrDRMALinkDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrDRMALinkDlg::CAttrDRMALinkDlg() 
	: ATTR_BASE_CLASS(DRMAKIHONMAP, IDD_ATTRDRMALINKDLG)
{
	// �^�u�ʒu�̏�����
	m_iChangeType = 0;
	m_lDRMATabNum = 0;
}

/** 
 * @fn HWND CAttrDRMALinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrDRMALinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrDRMALinkDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrDRMALinkDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrDRMALinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrDRMALinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{
	CreateControlRelation();
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_lDRMATabNum, bForce, bEditable );

	return bRet;
}

/**
 * @fn LONG CAttrDRMALinkDlg::GetTabNum()
 * @brief ���݂̃^�u�ʒu��n���܂�
 *
 * @retval ���݂̃^�u�ʒu
 */
LONG CAttrDRMALinkDlg::GetTabNum()
{
	return m_lDRMATabNum;
}

/**
 * @fn void CAttrDRMALinkDlg::SetTabNum(LONG lTabNum)
 * @brief �^�u�ʒu���Z�b�g���܂�
 *
 * @param lTabNum	[in]	�^�u�ʒu
 */
void CAttrDRMALinkDlg::SetTabNum(LONG lTabNum)
{
	m_lDRMATabNum = lTabNum;
	m_iChangeType = lTabNum;
}

/**
 * @fn void CAttrDRMALinkDlg::ChangeWindowMgrMap( INT iIndex )
 * �E�B���h�E�}�l�[�W���̃}�b�v�̐؂�ւ����s��
 *
 * @param iIndex	[in]	�}�b�v�̃C���f�b�N�X�ԍ�
 */
void CAttrDRMALinkDlg::ChangeWindowMgrMap( INT iIndex )
{
	switch( iIndex ) {
	case EDITVIEW_DRMA_KIHON:		// ��{�����^�u
		m_winMgr.ChangeMgrMap(DRMAKIHONMAP);
		break;
	case EDITVIEW_DRMA_TUIKA:		// �ǉ������^�u
		m_winMgr.ChangeMgrMap(DRMATUIKAMAP);
		break;
	case EDITVIEW_DRMA_KOTUKISEI:	// ��ʋK���^�u
		m_winMgr.ChangeMgrMap(DRMAKOTUKISEIMAP);
		break;
	case EDITVIEW_DRMA_ROSEN:		// �H���ԍ��^�u 
		m_winMgr.ChangeMgrMap(DRMAROSENMAP);
		break;
	case EDITVIEW_DRMA_DRMA:		// DRMA�^�u 
		m_winMgr.ChangeMgrMap(DRMADRMAMAP);
		break;
	default:
		break;
	}
	// �Ĕz�u
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);	
}

/**
 * @fn void CAttrDRMALinkDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrDRMALinkDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		std::list<CString> array;
		
		// ���H�̊�{�����^�u
		m_cControlRel.SetControl( IDC_TAB_DRMAATTR,		TYPE_OTHER,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_DRMA );
		m_cControlRel.SetControl( IDC_STATIC_DISPCLASS,	TYPE_OTHER,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON );
		m_cControlRel.SetControl( IDC_COMBO_DISPCLASS,	TYPE_COMBO,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON, _T("DISPCODE"),			NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_MANAGER,	TYPE_OTHER,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON );
		m_cControlRel.SetControl( IDC_COMBO_MANAGER,	TYPE_COMBO,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON, _T("MANAGECODE"),		NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_COMBO_ROADWIDTH,	TYPE_COMBO,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON, _T("ROADWIDTHCODE"),	NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_ROADWIDTH,	TYPE_OTHER,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON );
		m_cControlRel.SetControl( IDC_COMBO_LINKCLASS1,	TYPE_COMBO,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON, _T("LINKKINDCODE"),		NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_LINKCLASS,	TYPE_OTHER,		EDITVIEW_DRMA_KIHON, EDITVIEW_DRMA_KIHON );
		// ���H�̒ǉ������^�u
		m_cControlRel.SetControl( IDC_CHECK_BYPASS,		TYPE_CHECKBOX,	EDITVIEW_DRMA_TUIKA, EDITVIEW_DRMA_TUIKA, _T("BYPASS_FLAG"),		NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_CHECK_CARONLYF,	TYPE_CHECKBOX,	EDITVIEW_DRMA_TUIKA, EDITVIEW_DRMA_TUIKA, _T("CARONLY_FLAG"),		NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_LANE,		TYPE_OTHER,		EDITVIEW_DRMA_TUIKA, EDITVIEW_DRMA_TUIKA );
		m_cControlRel.SetControl( IDC_COMBO_LANE,		TYPE_COMBO,		EDITVIEW_DRMA_TUIKA, EDITVIEW_DRMA_TUIKA, _T("ROADLANECODE"),		NULL, m_strFeatureClassName );
		// ���H�̌�ʋK���^�u
		m_cControlRel.SetControl( IDC_STATIC_REGULER,	TYPE_OTHER,		EDITVIEW_DRMA_KOTUKISEI, EDITVIEW_DRMA_KOTUKISEI );
		m_cControlRel.SetControl( IDC_COMBO_REGULER,	TYPE_COMBO,		EDITVIEW_DRMA_KOTUKISEI, EDITVIEW_DRMA_KOTUKISEI, _T("REGSCODE"),	NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_LINKOK,	TYPE_OTHER,		EDITVIEW_DRMA_KOTUKISEI, EDITVIEW_DRMA_KOTUKISEI );
		m_cControlRel.SetControl( IDC_COMBO_LINKOK,		TYPE_COMBO,		EDITVIEW_DRMA_KOTUKISEI, EDITVIEW_DRMA_KOTUKISEI, _T("LINKPASSCODE"),NULL, m_strFeatureClassName );
		// ���H�̘H���^�u
		m_cControlRel.SetControl( IDC_STATIC_ROADNUMBER,	TYPE_OTHER,		EDITVIEW_DRMA_ROSEN, EDITVIEW_DRMA_ROSEN );
		m_cControlRel.SetControl( IDC_EDIT_ROADNUMBER,		TYPE_EDIT,		EDITVIEW_DRMA_ROSEN, EDITVIEW_DRMA_ROSEN, _T("ROADNO"),			NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_ROADNUMBER2,	TYPE_OTHER,		EDITVIEW_DRMA_ROSEN, EDITVIEW_DRMA_ROSEN );
		array.clear();
		array.push_back( _T("ROAD_CODE") );
		array.push_back( _T("NAME_KANJI") );
		array.push_back( _T("NAME_YOMI") );
		m_cControlRel.SetControl( IDC_GRID_DUPLILINK, TYPE_OUTERROWS, EDITVIEW_DRMA_ROSEN, EDITVIEW_DRMA_ROSEN, array, NULL, DUPLILINK_TABLE_NAME, _T("LINK_ID"), IDC_CHECK_HASDUPLI, FOLLOW_VALUE );
		
		// ���H��DRMA�^�u
		m_cControlRel.SetControl( IDC_STATIC_KUBUNCODE,		TYPE_OTHER,		EDITVIEW_DRMA_DRMA, EDITVIEW_DRMA_DRMA );
		m_cControlRel.SetControl( IDC_COMBO_KUBUNCODE,		TYPE_COMBO,		EDITVIEW_DRMA_DRMA, EDITVIEW_DRMA_DRMA, _T("ROADSECCODE"),		NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_LIMITSPEED,	TYPE_OTHER,		EDITVIEW_DRMA_DRMA, EDITVIEW_DRMA_DRMA );
		m_cControlRel.SetControl( IDC_COMBO_LIMITSPEED,		TYPE_COMBO,		EDITVIEW_DRMA_DRMA, EDITVIEW_DRMA_DRMA, _T("REGSSPEEDCODE"),	NULL, m_strFeatureClassName );
		m_cControlRel.SetControl( IDC_STATIC_TRIPSPEED,		TYPE_OTHER,		EDITVIEW_DRMA_DRMA, EDITVIEW_DRMA_DRMA );
		m_cControlRel.SetControl( IDC_EDIT_TRIPSPEED,		TYPE_EDIT,		EDITVIEW_DRMA_DRMA, EDITVIEW_DRMA_DRMA, _T("TRAFFICSPEED"),		NULL, m_strFeatureClassName );
	}
}

HRESULT CAttrDRMALinkDlg::CreateActiveXControls(UINT nID)
{
	// Load dialog template and InitData
	HRSRC hDlgInit = ::FindResource(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(nID), (LPTSTR)_ATL_RT_DLGINIT);
	BYTE* pInitData = NULL;
	HGLOBAL hData = NULL;
	HRESULT hr = S_OK;
	if (hDlgInit != NULL)
	{
		hData = ::LoadResource(_AtlBaseModule.GetResourceInstance(), hDlgInit);
		if (hData != NULL)
			pInitData = (BYTE*) ::LockResource(hData);
	}

	HRSRC hDlg = ::FindResource(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(nID), (LPTSTR)RT_DIALOG);
	if (hDlg != NULL)
	{
		HGLOBAL hResource = ::LoadResource(_AtlBaseModule.GetResourceInstance(), hDlg);
		DLGTEMPLATE* pDlg = NULL;
		if (hResource != NULL)
		{
			pDlg = (DLGTEMPLATE*) ::LockResource(hResource);
			if (pDlg != NULL)
			{
				// Get first control on the template
				BOOL bDialogEx = _DialogSplitHelper::IsDialogEx(pDlg);
				WORD nItems = _DialogSplitHelper::DlgTemplateItemCount(pDlg);

				// Get first control on the dialog
				DLGITEMTEMPLATE* pItem = _DialogSplitHelper::FindFirstDlgItem(pDlg);
				HWND hWndPrev = GetWindow(GW_CHILD);

				// Create all ActiveX cotnrols in the dialog template and place them in the correct tab order (z-order)
				for (WORD nItem = 0; nItem < nItems; nItem++)
				{
					DWORD wID = bDialogEx ? ((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->id : pItem->id;
					if (_DialogSplitHelper::IsActiveXControl(pItem, bDialogEx))
					{
						BYTE* pData = NULL;
						DWORD dwLen = _DialogSplitHelper::FindCreateData(wID, pInitData, &pData);
						CComPtr<IStream> spStream;
						if (dwLen != 0)
						{
							HGLOBAL h = GlobalAlloc(GHND, dwLen);
							if (h != NULL)
							{
								BYTE* pBytes = (BYTE*) GlobalLock(h);
								BYTE* pSource = pData; 
								Checked::memcpy_s(pBytes, dwLen, pSource, dwLen);
								GlobalUnlock(h);
								CreateStreamOnHGlobal(h, TRUE, &spStream);
							}
							else
							{
								hr = E_OUTOFMEMORY;
								break;
							}
						}

						CComBSTR bstrLicKey;
						hr = _DialogSplitHelper::ParseInitData(spStream, &bstrLicKey.m_str);
						if (SUCCEEDED(hr))
						{
							CAxWindow2 wnd;
							// Get control caption.
							LPWSTR pszClassName = 
								bDialogEx ? 
									(LPWSTR)(((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem) + 1) :
									(LPWSTR)(pItem + 1);
							// Get control rect.
							RECT rect;
							rect.left = 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->x : 
									pItem->x;
							rect.top = 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->y : 
									pItem->y;
							rect.right = rect.left + 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->cx : 
									pItem->cx);
							rect.bottom = rect.top + 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->cy : 
									pItem->cy);

							// Convert from dialog units to screen units
							MapDialogRect(&rect);

							// Create AxWindow with a NULL caption.
							wnd.Create(m_hWnd, 
								&rect, 
								NULL, 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->style : 
									pItem->style) | WS_TABSTOP, 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->exStyle : 
									0,
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->id : 
									pItem->id,
								NULL);

							if (wnd != NULL)
							{
								// Set the Help ID
								if (bDialogEx && ((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->helpID != 0)
									wnd.SetWindowContextHelpId(((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->helpID);
								// Try to create the ActiveX control.
								static WCHAR pwchLicenseKey[] =
								{
									0x004F,	0x0030,	0x0045,	0x0054,	0x0054,	0x0043,	
									0x0037,	0x0034,	0x0037
								};
								hr = wnd.CreateControlLic(pszClassName, spStream, NULL, /*bstrLicKey*/CComBSTR(pwchLicenseKey));
								if (FAILED(hr))
									break;
								// Set the correct tab position.
								if (nItem == 0)
									hWndPrev = HWND_TOP;
								wnd.SetWindowPos(hWndPrev, 0,0,0,0,SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
								hWndPrev = wnd;
							}
							else
							{
								hr = AtlHresultFromLastError();
							}
						}
					}
					else
					{
						if (nItem != 0)
							hWndPrev = ::GetWindow(hWndPrev, GW_HWNDNEXT);
					}
					pItem = _DialogSplitHelper::FindNextDlgItem(pItem, bDialogEx);
				}
			}
			else
				hr = AtlHresultFromLastError();
		}
		else
			hr = AtlHresultFromLastError();
	}
	return hr;
}
