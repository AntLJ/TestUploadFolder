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

// LQAttrDefaultDlg.cpp : CLQAttrDefaultDlg �̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LQAttrDefaultDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(DEFAULTMAP)
	BEGINROWS(WRCT_REST,0,0)
		RCREST(IDC_GRIDCTRL)
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CLQAttrDefaultDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrDefaultDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDefaultDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CLQAttrDefaultDlg::CLQAttrDefaultDlg()
 * @brief �R���X�g���N�^
 *
 * �Ȃɂ����܂���
 */
CLQAttrDefaultDlg::CLQAttrDefaultDlg() 
	: CLQAttrBaseDlg(DEFAULTMAP, IDD_LQATTRDEFAULTDLG)
{
}

/** 
 * @fn HWND CLQAttrDefaultDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * CLQAttrBaseDlg::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CLQAttrDefaultDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// �_�C�A���O�̍쐬��CLQAttrBaseDlg�ɔC���܂�
	m_hChildWnd = CLQAttrBaseDlg::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrDefaultDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CLQAttrDefaultDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}

/**
 * @fn BOOL CLQAttrDefaultDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CLQAttrDefaultDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = CLQAttrBaseDlg::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

    return bRet;
}

/**
 * @fn void CLQAttrDefaultDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CLQAttrDefaultDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// �W���O���b�h
		m_cControlRel.SetControl( IDC_GRIDCTRL,	TYPE_FEATUREGRID, 0, 0 );
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// �O���b�h�R���g���[���p
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @fn BOOL CLQAttrDefaultDlg::StartEditGridSel( INT nID, IVSFlexGridPtr spGrid, LONG Row, LONG Col )
 * �O���b�h�̃Z���ҏW�J�n���ɌĂ΂��
 *
 * @param nID		[in]	�O���b�h�̃R���g���[��ID
 * @param spGrid	[in]	�O���b�h�R���g���[��
 * @param Row		[in]	�ҏW�ΏۃZ���̍s
 * @param Col		[in]	�ҏW�ΏۃZ���̗�
 *
 * @return �ҏW�ł���Z���Ȃ� TRUE�A�o���Ȃ��Z���Ȃ� FALSE
 */
BOOL CLQAttrDefaultDlg::StartEditGridSel( INT nID, IVSFlexGridPtr spGrid, LONG Row, LONG Col )
{
	BOOL bRet = m_cControlRel.ClickOtherControl( nID, Row, Col, m_bIsGridEditMode );

	return bRet;
}
/**
 * @fn void CLQAttrDefaultDlg::AfterEditGridSel( INT nID, IVSFlexGridPtr spGrid, LONG Row, LONG Col )
 * �O���b�h�̃Z���ҏW�I�����ɌĂ΂��
 * 
 * @param nID		[in]	�O���b�h�̃R���g���[��ID
 * @param spGrid	[in]	�O���b�h�R���g���[��
 * @param Row		[in]	�ҏW�ΏۃZ���̍s
 * @param Col		[in]	�ҏW�ΏۃZ���̗�
 */
void CLQAttrDefaultDlg::AfterEditGridSel( INT nID, IVSFlexGridPtr spGrid, LONG Row, LONG Col )
{
	USES_CONVERSION;

	// �쐬�����R���{�{�b�N�X�͂��̓s�x�������Ȃ��ƃ_��
	spGrid->put_ComboList( CComBSTR(_T("")));

	m_cControlRel.EditedOtherControl( nID, Row, Col );	// �ҏW�� CFieldDef �ɓK�p
	m_cControlRel.ControlChanged( nID );					// �Ǐ]����R���g���[��������Ȃ炻����K�p
	// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
	SetButton( Changed() );
}

// FlexGrid�̃��C�Z���X�E�B���h�E���o���Ȃ��悤�ɂ��邽�߂̏���
HRESULT CLQAttrDefaultDlg::CreateActiveXControls(UINT nID)
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
