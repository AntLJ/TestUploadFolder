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

// AttrBaseDlg.h : CAttrBaseDlg �̐錾

#ifndef __ATTRBASEDLG_H_
#define __ATTRBASEDLG_H_

#include "SizingFramework/include/WinMgr.h"
#include "ControlRel.h"

class CArcHelper;

#ifndef __DEFINE_WM_FEATURECHANGED__
#define __DEFINE_WM_FEATURECHANGED__
const unsigned int WM_FEATURECHANGED = RegisterWindowMessage(_T("WM_FEATURECHANGED"));	// �t�B�[�`���ɕύX���������ꍇ
#endif
/////////////////////////////////////////////////////////////////////////////
// CAttrBaseDlg
class CAttrBaseDlg : 
	public CAxDialogImpl<CAttrBaseDlg>
{
public:
	CAttrBaseDlg( WINRECT* pWinMap, WORD resourceID );

	virtual ~CAttrBaseDlg()
	{
		if( m_hChangedBrush != NULL )
			DeleteObject( m_hChangedBrush );
		if( m_hErrorBrush != NULL )
			DeleteObject( m_hErrorBrush );
		if( m_hBrushCombo != NULL )
			DeleteObject( m_hBrushCombo );
	}

	const int IDD;

BEGIN_MSG_MAP(CAttrBaseDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
END_MSG_MAP()
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnWinMgr(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	/**
	 * �R�����R���g���[���̐F�ύX�p�C�x���g�n���h��
	 *
	 * �t�B�[���h�l�̕ύX���������ꍇ�A�����ŃR�����R���g���[���̐F���ς��܂�
	 */
	LRESULT OnCtlColorEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HDC hdc = (HDC) wParam;    // handle to display context 
		HWND hWnd = (HWND) lParam; // handle to static control 

		CControlDef* def = m_cControlRel.GetControlDef( hWnd );

		if( def && def->Errored() )
		{
			if ( !m_hErrorBrush )
				m_hErrorBrush = CreateSolidBrush( RGB(255,0,0) );

			SetBkMode(hdc, TRANSPARENT );
			SetBkColor(hdc, RGB(255,0,0));

			// Return a handle to the brush
			HBRUSH far* hbr = (HBRUSH far*) m_hErrorBrush;

			return ((DWORD) hbr);
		}
		else if( def && def->Changed() )
		{
			if( !m_hChangedBrush )
				m_hChangedBrush = CreateSolidBrush( RGB(255,168,126) );

			SetBkMode(hdc, TRANSPARENT );
			SetBkColor(hdc, RGB(255,168,126));

			// Return a handle to the brush
			HBRUSH far* hbr = (HBRUSH far*) m_hChangedBrush;

			return ((DWORD) hbr);
		} 
		 CComboBox box( hWnd );
		if ( box.GetCount() > 0 && !box.IsWindowEnabled() ) {
			SetBkMode(hdc, TRANSPARENT );
			SetBkColor(hdc, RGB(m_lComboR,m_lComboG,m_lComboB));

			HBRUSH far* hbr = (HBRUSH far*) m_hBrushCombo;
			return ((DWORD)hbr );		
		}

		bHandled = TRUE;

		return 0;
	}

	/**
	 * @struct BACKGROUND_RULE
	 * @brief ���e�ԍ����[���̍\����
	 */
	typedef struct BackgroundRule{
		LONG	lClass;			// ��ʃR�[�h
		CString	strClass_Name;	// ��ʖ���
		CString	strMS_NO;		// ���e�ԍ�
		LONG	lLayerID;		// ���C��ID
	} BACKGROUND_RULE;

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetDlgCode(INT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL)
	{
		HWND hWnd = CAxDialogImpl::Create(hWndParent, dwInitParam);
		// �e�E�B���h�E�n���h�����m�ۂ��Ă���
		m_hAttributeDlgWnd = hWndParent;
		return hWnd;
	}
	void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList );
	virtual BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable ) = 0;
	virtual BOOL CheckReturnKeyItem(INT nClassID) = 0;
	virtual	BOOL CheckEscKeyItem(INT nClassID) = 0;
	virtual BOOL ErrorCheck() = 0;
	inline void SetArcHelper( IApplication* ipApp ){ m_cArcHelper.Init( (IApplicationPtr)ipApp ); m_cControlRel.SetArcHelper( ipApp ); };
	inline void SetAttributeDlgHWND( HWND hWnd ){ m_hAttributeDlgWnd = hWnd; };
	inline void ResetFeatures(){ m_cControlRel.ResetFeatures(); };
	inline void ClearFeatureDefs(){ m_cControlRel.ClearFeatureDefs(); };
	inline CString GetClassName(){ return m_strClassName; };
	BOOL IsMatchClass( CString strClassName ){ if( m_strClassName == strClassName ) return TRUE; else return FALSE; };
	inline void	SetAliasOrField( BOOL bAliasOrField ){ m_cControlRel.SetAliasOrField( bAliasOrField ); };
	BOOL CheckAllItemsError();

protected:
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, INT view, BOOL bForce, BOOL bEditable );
//	BOOL CheckReturnKeyItem(INT nClassID);
	BOOL IsStartEditing();
	BOOL IsEditableFeature();
	BOOL Changed();
	BOOL SetButton( BOOL bChanged );
	BOOL CheckInputValue( HWND hWndCtl, WORD wID );
	CFeatureDef* GetCurrentFeatureDef();
	VOID SelectDlgItem(INT nClassID);
	void SetIMEMode(BOOLEAN bIsOn);
	void SetComboboxList(INT nClassID);
	void SetDlgStatus(INT nID, BOOL bIsEnable, BOOL bIsEditBox = FALSE);
	int AttrMessageBox( LPCTSTR lpcszMsg, LPCTSTR lpcszCap, UINT uType, UINT uLevel );
	ISiNDYRulePtr GetRule(){ if( m_ipRule == NULL ) m_ipRule.CreateInstance( CLSID_SiNDYRuleObj ); return m_ipRule; };

	/**
	 * @brief BACKGROUND_CLASS���烋�[�����m�ۂ���
	 */
	void LoadBackgroundRule();

	/**
	 * @brief �R�[�h�l���猴�e�ԍ����擾����֐�
	 *
	 * @param	lClass	[in]  ���e�ԍ����ق����R�[�h�l
	 *
	 * @return  CString ���e�ԍ�������i������Ȃ������ꍇ�ɂ͋󕶎��j
	 */
	CString GetMSNOByClass(long lClass);

	/**
	 * @brief ���e�ԍ�����R�[�h�l���擾����֐�
	 *
	 * @param	strMSNO	[in]  ���e�ԍ�������
	 *
	 * @return  long �R�[�h�l�i������Ȃ������ꍇ�ɂ�-1�j
	 */
	long GetClassByMSNO(CString strMSNO);

protected:
	CWinMgr				m_winMgr;
	CArcHelper			m_cArcHelper;
	CString				m_strClassName;
	CWindow				m_wndTmp;
	CControlRel			m_cControlRel;
	std::list<CFeatureDef>*	m_pFeatureDefList;
	LONG				m_lFeatureDefIndex;
	BOOL				m_bFinishInit;
	HWND				m_hAttributeDlgWnd;
	ISiNDYRulePtr		m_ipRule;
	HBRUSH				m_hChangedBrush, m_hErrorBrush, m_hBrushCombo;	//!< �R�����R���g���[���̐F�ύX�p�u���V
	long				m_lComboR, m_lComboG, m_lComboB;
	std::vector<BACKGROUND_RULE>	m_BackgroundRule;	//!< �w�i���[��
};

#endif //__ATTRBASEDLG_H_
