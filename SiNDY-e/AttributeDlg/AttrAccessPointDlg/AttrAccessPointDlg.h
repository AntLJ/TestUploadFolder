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

// AttrAccessPointDlg.h : CAttrAccessPointDlg �̐錾

#ifndef _ATTRACCESSPOINTDLG_
#define _ATTRACCESSPOINTDLG_

#include "resource.h"
#include "../../HwyEditTool/HwyHelper.h"
#include "WinLib/str2.h"
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include "AccessStoreCache.h"


///< �H����ʃR�[�h���R���e�i
typedef CHetCVDMap CCodeValueDomain;

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrAccessPointDlg
class CAttrAccessPointDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg,
	public IDispEventImpl<IDC_GRID_ACCESS_STORE,CAttrAccessPointDlg>
{
public:
	CAttrAccessPointDlg();
	~CAttrAccessPointDlg()
	{
		::DeleteObject( m_hFont );
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrAccessPointDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrAccessPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	COMMAND_HANDLER(IDC_COMBO_ROADCLASS_REF, CBN_SELCHANGE, OnSelchangeRsel_class)
	COMMAND_HANDLER(IDC_COMBO_ROADNAME_REF, CBN_SELCHANGE, OnSelchangeRsel_Name)
	COMMAND_HANDLER(IDC_COMBO_SEQUENCE, CBN_SELCHANGE, OnSelchangeRoadSequence)
	COMMAND_HANDLER(IDC_EDIT_HOUMENKANJI, EN_UPDATE, OnUpdateHoumenKanji)
	COMMAND_HANDLER(IDC_EDIT_HOUMENKANA, EN_UPDATE, OnUpdateHoumenYomi)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
	CHAIN_MSG_MAP(ATTR_BASE_CLASS)

ALT_MSG_MAP(IDC_CHECK_HWYMODEFLAG)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ROADCLASS_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ROADNAME_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ROSENCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_SEQUENCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SEQUENCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_DIRECTION)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_JUNCTIONCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENKANJI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENKANA)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	
ALT_MSG_MAP(IDC_CHECK_RESTAURANT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_RESTING)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_INFOMATION)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_SPA)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_CASHSERVICE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_TOILET)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_GS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_CAFETERIA)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_NAP)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_SHOWER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_FAX)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_HWYOASIS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_HANDYCAP)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_MULTIPLESTORE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_SOUBENIR)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_INFODESK)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_LAUNDRY)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_MAILBOX)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_DOGRUN)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

ALT_MSG_MAP(IDC_EDIT_MASTERCODE)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_START_HOUR)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_START_MIN)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_END_HOUR)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_END_MIN)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_MONDAY)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_TUESDAY)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_WEDNESDAY)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_THURSDAY)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_FRIDAY)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_SATURDAY)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_SUNDAY)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_HOLIDAY)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_GOLDENWEEK)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_BON)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_YEAREND)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_NEWYEAR)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_CHECK_ALL)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_UNCHECK_ALL)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_ADD_ACCESS_STORE)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_UPDATE_ACCESS_STORE)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DELETE_ACCESS_STORE)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SAVE_ACCESS_STORE)			// [Bug5556]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

ALT_MSG_MAP(IDC_TAB_ACCESSPOINT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

BEGIN_SINK_MAP(CAttrAccessPointDlg)					// [Bug5556]�Œǉ�
	SINK_ENTRY(IDC_GRID_ACCESS_STORE, 1, SelChangeGridAccessStore)
END_SINK_MAP()
void __stdcall SelChangeGridAccessStore();


/////////////////////////////////////////////////////////////////////////////
//
// CAttrAccessPointDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// �R���g���[���ƃt�B�[���h�̊֘A�t��
		CreateControlRelation();

		// �`�F�[���X�}�X�^�[�R�[�h�G�f�B�b�g�R���g���[���̍ő包���ݒ�i[Bug5556]�Œǉ��j
		CEdit( GetDlgItem( IDC_EDIT_MASTERCODE ) ).SetLimitText( m_MasterCodeMaxLength );
		// ���Ԍn�G�f�B�b�g�R���g���[���Q�̍ő包���ݒ�i[Bug5556]�Œǉ��j
		std::vector<long> aTimeEditControlIDs = boost::assign::list_of
			(IDC_EDIT_START_HOUR)(IDC_EDIT_START_MIN)(IDC_EDIT_END_HOUR)(IDC_EDIT_END_MIN);
		BOOST_FOREACH (long aIDC, aTimeEditControlIDs)
		{
			CEdit( GetDlgItem( aIDC ) ).SetLimitText( m_TimeStrMaxLength );
		}

		// ACCESSPOINT �p�^�u�R���g���[��
		HWND hwndTab = GetDlgItem( IDC_TAB_ACCESSPOINT );
		TC_ITEM pTabCtrlItem;

		ATLASSERT( ::IsWindow( hwndTab ) );

		CString strTemp;  //!< pszText�ɓn����������ꎞ�I�Ɋi�[����ϐ�
		pTabCtrlItem.mask = TCIF_TEXT;
		strTemp = AheLoadString( IDS_BASEINFO );
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, 0, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString( IDS_FACILITYINFO );
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, 1, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString( IDS_ACCESSSTORE );
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, 2, (LPARAM)(&pTabCtrlItem));
		::SetWindowPos( hwndTab, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED );

		// �T�u�N���X��
		m_wnd0.SubclassWindow(GetDlgItem(IDC_CHECK_HWYMODEFLAG));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_COMBO_ROADNAME_REF));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_EDIT_ROSENCODE));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_COMBO_SEQUENCE));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_EDIT_SEQUENCE));
		m_wnd6.SubclassWindow(GetDlgItem(IDC_COMBO_DIRECTION));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_COMBO_JUNCTIONCODE));
		m_wnd8.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENKANJI));
		m_wnd9.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENKANA));
		m_wnd10.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));

		m_wndInfo1.SubclassWindow(GetDlgItem(IDC_CHECK_RESTAURANT));
		m_wndInfo2.SubclassWindow(GetDlgItem(IDC_CHECK_RESTING));
		m_wndInfo3.SubclassWindow(GetDlgItem(IDC_CHECK_INFOMATION));
		m_wndInfo4.SubclassWindow(GetDlgItem(IDC_CHECK_SPA));
		m_wndInfo5.SubclassWindow(GetDlgItem(IDC_CHECK_CASHSERVICE));
		m_wndInfo6.SubclassWindow(GetDlgItem(IDC_CHECK_TOILET));
		m_wndInfo7.SubclassWindow(GetDlgItem(IDC_CHECK_CAFETERIA));
		m_wndInfo8.SubclassWindow(GetDlgItem(IDC_CHECK_NAP));
		m_wndInfo9.SubclassWindow(GetDlgItem(IDC_CHECK_SHOWER));
		m_wndInfo10.SubclassWindow(GetDlgItem(IDC_CHECK_FAX));
		m_wndInfo11.SubclassWindow(GetDlgItem(IDC_CHECK_HWYOASIS));
		m_wndInfo12.SubclassWindow(GetDlgItem(IDC_CHECK_HANDYCAP));
		m_wndInfo13.SubclassWindow(GetDlgItem(IDC_CHECK_SOUBENIR));
		m_wndInfo14.SubclassWindow(GetDlgItem(IDC_CHECK_INFODESK));
		m_wndInfo15.SubclassWindow(GetDlgItem(IDC_CHECK_LAUNDRY));
		m_wndInfo16.SubclassWindow(GetDlgItem(IDC_CHECK_MAILBOX));
		m_wndInfo19.SubclassWindow(GetDlgItem(IDC_CHECK_GS));
		m_wndInfo20.SubclassWindow(GetDlgItem(IDC_CHECK_MULTIPLESTORE));
		m_wndInfo21.SubclassWindow(GetDlgItem(IDC_CHECK_DOGRUN));

		m_wndAS1.SubclassWindow( GetDlgItem(IDC_EDIT_MASTERCODE) );					// [Bug5556]�Œǉ�
		m_wndAS2.SubclassWindow( GetDlgItem(IDC_EDIT_START_HOUR) );					// [Bug5556]�Œǉ�
		m_wndAS3.SubclassWindow( GetDlgItem(IDC_EDIT_START_MIN) );					// [Bug5556]�Œǉ�
		m_wndAS4.SubclassWindow( GetDlgItem(IDC_EDIT_END_HOUR) );					// [Bug5556]�Œǉ�
		m_wndAS5.SubclassWindow( GetDlgItem(IDC_EDIT_END_MIN) );					// [Bug5556]�Œǉ�
		m_wndAS6.SubclassWindow( GetDlgItem(IDC_CHECK_MONDAY) );					// [Bug5556]�Œǉ�
		m_wndAS7.SubclassWindow( GetDlgItem(IDC_CHECK_TUESDAY) );					// [Bug5556]�Œǉ�
		m_wndAS8.SubclassWindow( GetDlgItem(IDC_CHECK_WEDNESDAY) );					// [Bug5556]�Œǉ�
		m_wndAS9.SubclassWindow( GetDlgItem(IDC_CHECK_THURSDAY) );					// [Bug5556]�Œǉ�
		m_wndAS10.SubclassWindow( GetDlgItem(IDC_CHECK_FRIDAY) );					// [Bug5556]�Œǉ�
		m_wndAS11.SubclassWindow( GetDlgItem(IDC_CHECK_SATURDAY) );					// [Bug5556]�Œǉ�
		m_wndAS12.SubclassWindow( GetDlgItem(IDC_CHECK_SUNDAY) );					// [Bug5556]�Œǉ�
		m_wndAS13.SubclassWindow( GetDlgItem(IDC_CHECK_HOLIDAY) );					// [Bug5556]�Œǉ�
		m_wndAS14.SubclassWindow( GetDlgItem(IDC_CHECK_GOLDENWEEK) );				// [Bug5556]�Œǉ�
		m_wndAS15.SubclassWindow( GetDlgItem(IDC_CHECK_BON) );						// [Bug5556]�Œǉ�
		m_wndAS16.SubclassWindow( GetDlgItem(IDC_CHECK_YEAREND) );					// [Bug5556]�Œǉ�
		m_wndAS17.SubclassWindow( GetDlgItem(IDC_CHECK_NEWYEAR) );					// [Bug5556]�Œǉ�
		m_wndAS18.SubclassWindow( GetDlgItem(IDC_BUTTON_CHECK_ALL) );				// [Bug5556]�Œǉ�
		m_wndAS19.SubclassWindow( GetDlgItem(IDC_BUTTON_UNCHECK_ALL) );				// [Bug5556]�Œǉ�
		m_wndAS20.SubclassWindow( GetDlgItem(IDC_BUTTON_ADD_ACCESS_STORE) );		// [Bug5556]�Œǉ�
		m_wndAS21.SubclassWindow( GetDlgItem(IDC_BUTTON_UPDATE_ACCESS_STORE) );		// [Bug5556]�Œǉ�
		m_wndAS22.SubclassWindow( GetDlgItem(IDC_BUTTON_DELETE_ACCESS_STORE) );		// [Bug5556]�Œǉ�
		m_wndAS23.SubclassWindow( GetDlgItem(IDC_BUTTON_SAVE_ACCESS_STORE) );		// [Bug5556]�Œǉ�

		m_wndTab.SubclassWindow(GetDlgItem(IDC_TAB_ACCESSPOINT ) );

		// ���ʖ��̂ɕ����t�H���g��ݒ肷��
		m_wnd8.SetFont( m_hFont );

		// Access_Store�O���b�h�R���g���[����m_ipAccessStoreGrid�̊֘A�t���i[Bug5556]�Œǉ��j
		CAxWindow( GetDlgItem( IDC_GRID_ACCESS_STORE )).QueryControl( &m_ipAccessStoreGrid );

		// �O���b�h�R���g���[������̃C�x���g���E���i[Bug5556]�Œǉ��j
		AtlAdviseSinkMap(this, TRUE);

		// Debug�r���h�ł͑�0��i�I�u�W�F�N�gID�j��\���i[Bug5556]�Œǉ��j
#ifdef _DEBUG
		m_ipAccessStoreGrid->PutColHidden( 0, false );
#endif

		::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0L);
		ChangeWindowMgrMap( m_iChangeType );

		// �V�X�e���Ƀt�H�[�J�X��ݒ肳���邽��
		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}

	/**
	 * WM_NOTIFY �p�C�x���g�n���h��
	 *
	 * �ȉ��̏������s���܂�
	 * �E�^�u�̐؂�ւ�������������
	 */
	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( wParam == IDC_TAB_ACCESSPOINT ) {
			LPNMHDR pnmh = (LPNMHDR)lParam;
			if( pnmh->code == TCN_SELCHANGE ) {
				// �R���g���[���؂�ւ�
				m_iChangeType = TabCtrl_GetCurSel( pnmh->hwndFrom );
				
				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
				m_lAccessTabNum = m_iChangeType; // �^�u�̈ʒu�ۑ�
				//[Bug 5510]�H����ʂȂǂ̓Ǝ��ɃR���{�{�b�N�X�ݒ肵�Ă���悤�Ȃ�́A�l���ăZ�b�g�����Ƃ���
				//��x���Z�b�g����Ă��܂��B���̂��߁A�^�u�؂�ւ��̃^�C�~���O�ŉ�������Ȃ��Ȃ����B
				//�Ȃ̂ŁA�^�u�؂�ւ��^�C�~���O�ŃR���{�{�b�N�X���Đݒ肵�Ă�邽�߂�AttrBaseDlg::SetCurrentFeatureDefIndex�ł͂Ȃ�
				//AttrAccessPointDlg::SetCurrentFeatureDefIndex���Ă�ł��B
				SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_bForce, m_bEditable );

				bHandled = TRUE;
				return 1;
			}
		}

		if( wParam == 0 )
		{
			HWND hwndTab = GetDlgItem( IDC_TAB_ACCESSPOINT );
			if( lParam == -2 )
			{
				// �R���g���[���؂�ւ�
				if( m_iChangeType + 1 == 2 )
				{
					m_iChangeType = 0;
					::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
				}
				else
				{
					m_iChangeType = m_iChangeType + 1;
					::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
				}
				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
				m_lAccessTabNum = m_iChangeType;	// �^�u�̈ʒu�ۑ�
				SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_bForce, m_bEditable );

				bHandled = TRUE;

				if( IsEditableFeature() == TRUE && m_cArcHelper.IsStartEditing() == TRUE )
				{
					if( m_lAccessTabNum == 0 )
						::SetFocus( GetDlgItem(IDC_CHECK_HWYMODEFLAG) );
					else 
						::SetFocus( GetDlgItem(IDC_CHECK_RESTAURANT) );
				}
				return 1;
			}
			else if( lParam == -1 ) {
				// �R���g���[���؂�ւ�
				if( m_iChangeType - 1 == -1 )
				{
					m_iChangeType = 1;
					::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
				}
				else
				{
					m_iChangeType = m_iChangeType - 1;
					::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
				}
				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
				m_lAccessTabNum = m_iChangeType;	// �^�u�̈ʒu�ۑ�
				SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_bForce, m_bEditable );

				bHandled = TRUE;

				if( IsEditableFeature() == TRUE && m_cArcHelper.IsStartEditing() == TRUE )
				{
					if( m_lAccessTabNum == 0 )
						::SetFocus( GetDlgItem(IDC_CHECK_HWYMODEFLAG) );
					else 
						::SetFocus( GetDlgItem(IDC_CHECK_RESTAURANT) );
				}
				return 1;
			}
		}

		return 0;
	}

	/**
	 * WM_COMMAND �p�C�x���g�n���h��
	 *
	 * �e�R�����R���g���[���ŕύX���������ꍇ�A�����ŏ������܂�
	 * ���݂̓G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�A�R���{�{�b�N�X�̏������s���܂�
	 */
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// SetCurrentFeatureDefIndex() �̍Œ��̓L�����Z���i�^�C�~���O�ɂ���Ă͂��������Ȃ邽�߁j
		if( !m_bFinishInit ) return 0;

		INT msg = HIWORD(wParam);						// ���상�b�Z�[�W
		INT	nTargetControl = (INT)LOWORD(wParam);		// ����ΏۃR���g���[��

		// �G�f�B�b�g�{�b�N�X�̎��ɑS�I���ɂ���
		if( msg == EN_SETFOCUS )
			SelectDlgItem( nTargetControl );
		// �R���{�{�b�N�X�̃��X�g�{�b�N�X�̒����𒲐�
		if( msg == CBN_DROPDOWN )
			SetComboboxList( nTargetControl );

		// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
		if( ( msg == CBN_SELCHANGE ) || ( msg == EN_KILLFOCUS ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )) {
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}

		// �u�S�I���v�u�S�����v�{�^���������ꂽ���̏����i[Bug5556]�Œǉ��j
		if (nTargetControl == IDC_BUTTON_CHECK_ALL || nTargetControl == IDC_BUTTON_UNCHECK_ALL)
		{
			BOOST_FOREACH (long aID, GetASCheckBoxIDs())
			{
				CButton( GetDlgItem( aID ) ).SetCheck( nTargetControl == IDC_BUTTON_CHECK_ALL ? BST_CHECKED : BST_UNCHECKED );
			}
		}

		// �u�ǉ��v�u�X�V�v�u�폜�v�{�^���������ꂽ���̏����i[Bug5556]�Œǉ��j
		EditAccessStore( nTargetControl );

		// �u�ۑ��v�{�^���������ꂽ���̏����i[Bug5556]�Œǉ��j
		if (nTargetControl == IDC_BUTTON_SAVE_ACCESS_STORE)
		{
			SaveAccessStore();
		}

		return 0;
	}

	/**
	 * IDC_EDIT_HOUMENKANJI �p�C�x���g�n���h��
	 *
	 * �����ŁA�������͂̃`�F�b�N�������Ȃ��A�s�K�؂ȕ��������͂��ꂽ�ꍇ�ɂ�
	 * ���͂ł��Ȃ��悤�ɂ��܂��B
	 */
	LRESULT OnUpdateHoumenKanji(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( ::IsWindow( hWndCtl ) )
		{
			CEdit edit( hWndCtl );
			INT nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			edit.GetWindowText( lpszText, nTextLen );

			// <null> �̏ꍇ���l��
			if( lstrcmp( lpszText, NULL_VALUE ) != 0 )
			{
				// ���p�������S�p�����ϊ��A�g�p�֎~�������̃`�F�b�N
				CString strHoumenKanji = AheConvertToAccessPointName( lpszText, TRUE );
				// �A���t�@�x�b�g���������啶���ϊ�
				int nTextLen = lstrlen(strHoumenKanji) + 1;
				LPTSTR lpszChecked = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
				LCMapString( GetUserDefaultLCID(), LCMAP_UPPERCASE, strHoumenKanji, -1, lpszChecked, nTextLen );
				strHoumenKanji = lpszChecked;
				// �`�F�b�N�̌��ʏC�����s��ꂽ�ꍇ�A�\���ɔ��f����
				if( strHoumenKanji.Compare( lpszText ) != 0 )
				{
					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strHoumenKanji );
					// ���͈ʒu�̏C��
					INT nDiff = CString_GetLength2(CString( lpszText )) - CString_GetLength2(strHoumenKanji);
					edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
				}
			}
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
		}

		return 0;
	}

	/**
	 * IDC_EDIT_HOUMENKANA �p�C�x���g�n���h��
	 *
	 * �����ŁA�������͂̃`�F�b�N�������Ȃ��A�s�K�؂ȕ��������͂��ꂽ�ꍇ�ɂ�
	 * ���͂ł��Ȃ��悤�ɂ��܂��B
	 */
	LRESULT OnUpdateHoumenYomi(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( ::IsWindow( hWndCtl ) )
		{
			CEdit edit( hWndCtl );
			INT nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			edit.GetWindowText( lpszText, nTextLen );


			// <null> �̏ꍇ���l��
			if( lstrcmp( lpszText, NULL_VALUE ) != 0 )
			{
				CString strHoumenYomi = AheConvertToAccessPointName( lpszText, FALSE );
				if( strHoumenYomi.Compare( lpszText ) != 0)
				{
					INT nSelStart = 0, nSelEnd = 0;
					edit.GetSel( nSelStart, nSelEnd );
					edit.SetWindowText( strHoumenYomi );
					// ���͈ʒu�̏C��
					INT nDiff = CString_GetLength2(CString( lpszText )) - CString_GetLength2(strHoumenYomi);
					edit.SetSel( nSelStart - nDiff, nSelEnd - nDiff );
				}
			}
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
		}

		return 0;
	}

	/**
	 * �H����ʎQ�Ɨp���ύX���ꂽ�Ƃ��̃C�x���g�n���h��
	 *
	 * �H�����̎Q�Ɨp�ƁA�H���R�[�h�̒l��ύX���܂�
	 */
	LRESULT OnSelchangeRsel_class(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled)
	{
		// �I�����ꂽ�H����ʂ̎擾
		CComboBox cbLineClass = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
		int nIdx = cbLineClass.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_lLineClass = cbLineClass.GetItemData(nIdx);
		// �H���R�[�h�̍ĕ`��
		if(! RedrawRoadNameList(0))
			return 0;
		// �H���R�[�h�̐ݒ�
		if(! SetRoadCode(m_lRoadCode))
			return 0;
		// �H�����V�[�P���X�̒l��0�ɐݒ肷��
		CComVariant		vaRoadSequence;
		CFieldDef*		pFieldDef;
		pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_SEQ") );
		pFieldDef->m_vaValue = 0L;
		m_cRoadSeq.Format(_T("%d"), 0 );	

		ChangeRoadSeqCombobox();

		// �H���R�[�h�̒l�ɏ]���ăn�C�E�F�C���[�h�t���O�̃`�F�b�N�{�b�N�X���X�V
		ChangeHwyModeCheckBox(m_lRoadCode);

		return 0;
	}

	/**
	 * �H�����̂��ύX���ꂽ�Ƃ��̃C�x���g�n���h��
	 *
	 * �H���R�[�h�̒l��ύX���܂�
	 */
	LRESULT OnSelchangeRsel_Name(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// �I�����ꂽ�H���R�[�h�̎擾
		CComboBox cbRoadName = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_ROADNAME_REF));
		int nIdx = cbRoadName.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_lRoadCode = cbRoadName.GetItemData(nIdx);
		// �H���R�[�h�̐ݒ�
		if(! SetRoadCode(m_lRoadCode))
			return 0;
		// �H�����V�[�P���X�̒l��0�ɐݒ肷��
		CComVariant		vaRoadSequence;
		CFieldDef*		pFieldDef;
		pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_SEQ") );
		pFieldDef->m_vaValue = 0L;
		m_cRoadSeq.Format(_T("%d"), 0 );	

		ChangeRoadSeqCombobox();

		return 0;
	}

	/**
	 * �H�����V�[�P���X�Q�Ɨp���ύX���ꂽ�Ƃ��̃C�x���g�n���h��
	 *
	 * �H�����V�[�P���X�̒l��ύX���܂��B
	 */
	LRESULT OnSelchangeRoadSequence(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// �I�����ꂽ�H���R�[�h�̎擾
		CComboBox cbRoadSequence = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_SEQUENCE));
		int nIdx = cbRoadSequence.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		LONG lRoadSequence = cbRoadSequence.GetItemData(nIdx);
		m_cRoadSeq.Format(_T("%d"), lRoadSequence );	

		SetRoadSequence();

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrAccessPointDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp );
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum();
	virtual void SetTabNum(LONG lTabNum);
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

	LONG					m_lAccessTabNum;			//!< �A�N�Z�X�|�C���g�^�u�̕\���ʒu�ۑ��p
	INT						m_iChangeType;				//!< �R���g���[���؂�ւ��p

private:
	void CreateControlRelation();
	void ChangeWindowMgrMap( INT iIndex );
	void SetRoadSequence();
	void ChangeRoadSeqCombobox();
	bool SetLineClassList(LONG nLineClass);
	bool GetLineClass(int* pClass);
	bool SetLineClass(int nClass);
	bool RedrawRoadNameList(LONG nRoadCode);
	bool GetRoadCode(int* pRoadCode);
	bool SetRoadCode(int nRoadCode);

	ITablePtr GetRoadCodeTable();
	bool GetRoadCodeSet(CHetRoadCodeSet* pCodeSet);
	void ChangeHwyModeCheckBox(long cRoadCode);

	/**
	 * @brief Access_Store���O���b�h�̓��e���X�V����i[Bug5556]�Œǉ��j
	 * @note	�L���b�V������l���擾���ăO���b�h����S�ď���������B
	 */
	void UpdateASGrid();

	/**
	 * @brief Access_Store�^�u���̃R���g���[���Q��Access_Store���O���b�h�̏�Ԃɏ]���čX�V����i[Bug5556]�Œǉ��j
	 */
	void UpdateASControls();

	/**
	 * @brief Access_Store���O���b�h���̃Z���̐F��ݒ肷��i[Bug5556]�Œǉ��j
	 */
	void SetASGridColor();

	/**
	 * @brief Access_Store���O���b�h�̎w�肳�ꂽ�Z���̒l��cControlID�Ŏw�肳�ꂽ�G�f�B�b�g�R���g���[���ɔ��f����
	 * @param[in]	cRow		�Ώۍs�̍s�ԍ�
	 * @param[in]	cCol		�Ώۗ�̗�ԍ�
	 * @param[in]	cControlID	�ΏۃG�f�B�b�g�R���g���[���̃R���g���[��ID
	 */
	void UpdateASEditControl( long cRow, long cCol, long cControlID );

	/**
	 * @brief Access_Store���O���b�h�̎w�肳�ꂽ�Z���̒l��cControlID�Ŏw�肳�ꂽ�`�F�b�N�{�b�N�X�ɔ��f����
	 * @param[in]	cRow		�Ώۍs�̍s�ԍ�
	 * @param[in]	cCol		�Ώۗ�̗�ԍ�
	 * @param[in]	cControlID	�ΏۃG�f�B�b�g�R���g���[���̃R���g���[��ID
	 */
	void UpdateASCheckBox( long cRow, long cCol, long cControlID );

	/**
	 * @brief Access_Store�^�u�̕ҏW�\�ȃR���g���[���Q�̃R���g���[��ID�W����Ԃ��i[Bug5556]�Œǉ��j
	 * @note	�ҏW�ۂɉ�����Enabled/Disabled��؂�ւ���ׂ��ΏۃR���g���[���Q�𓾂�̂ɕK�v�B
	 * @return	�R���g���[��ID�W��
	 */
	std::vector<long> GetASEditableControlIDs();

	/**
	 * @brief Access_Store�^�u�̃`�F�b�N�{�b�N�X�Q�̃R���g���[��ID�W����Ԃ��i[Bug5556]�Œǉ��j
	 * @note	�u�S�I���v�u�S�����v�{�^���ɂ���ԕύX�ΏۃR���g���[���Q�𓾂�̂ɕK�v�B
	 * @return	�R���g���[��ID�W��
	 */
	std::vector<long> GetASCheckBoxIDs();

	/**
	 * @brief Access_Store�e�[�u����ǂݍ��ށi[Bug5556]�Œǉ��j
	 * @note	�_�C�A���O�ɕ\������Ă���A�N�Z�X�|�C���g�Q�Ɋ֘A�t����Access_Store�I�u�W�F�N�g��ǂݍ��݁A�L���b�V�����쐬����B
	 */
	void LoadAccessStore();

	/**
	 * @brief �u�ǉ��v�u�X�V�v�u�폜�v�{�^���������ꂽ���A���̓��e�ɉ����ăL���b�V�����X�V����i[Bug5556]�Œǉ��j
	 * @note	OnCommand()���ŌĂяo����A�����ꂽ�{�^�������ł��邩�͌Ăяo����ɔ��肷��B
	 * @param[in]	cControlID	�����ꂽ�{�^���̃R���g���[��ID
	 */
	void EditAccessStore( long cControlID );

	/**
	 * @brief Access_Store�O���b�h�̓��e��DB�ɔ��f����
	 */
	void SaveAccessStore();

	/**
	 * @brief Access_Store�L���b�V���̑Ώۍs�Ɋ܂܂��S�t�B�[���h�̒l�̑Ó������`�F�b�N����
	 * @param[in]	crRow	�Ώۍs
	 * @retval	true	�S�Ă̒l�͑Ó��ł���
	 * @retval	false	�Ó��łȂ��l�����݂���
	 */
	bool CheckASValuesValidity( const access_store_cache::CASRow& crRow );

		/**
		 * @brief �w�肳�ꂽ�t�B�[���h�̒l�����e�͈͓��ɂ��邩�ۂ��𔻒肵�A�͈͊O�ɂ���΃G���[�_�C�A���O��\������
		 * @param[in]	crRow		�Ώۍs
		 * @param[in]	cMin		���e�͈͂̍ŏ��l
		 * @param[in]	cMax		���e�͈͂̍ő�l
		 * @param[in]	cFieldName	�t�B�[���h��
		 * @param[in]	cFieldAlias	�t�B�[���h���̕ʖ��i�G���[���b�Z�[�W�p�j
		 * @return	���茋��
		 */
	bool IsValidASValue( const access_store_cache::CASRow& crRow, tstring cFieldName, const CString& cFieldAlias, long cMin, long cMax );

	/**
	 * @brief �G�f�B�b�g�R���g���[���Q����擾�������ɂ����Access_Store�L���b�V�����X�V����
	 * @param[in]	cControlID	�ΏۃG�f�B�b�g�R���g���[���̃R���g���[��ID
	 * @param[in]	cObjectID	�Ώ�Access_Store�I�u�W�F�N�g��ID
	 * @return	����
	 */
	bool UpdateASCacheByEditControls( long cControlID, long cObjectID );

		/**
		 * @brief �G�f�B�b�g�R���g���[������擾�������ɂ����Access_Store�L���b�V�����X�V����
		 * @param[in]	cControlID	�ΏۃG�f�B�b�g�R���g���[���̃R���g���[��ID
		 * @param[in]	cObjectID	�Ώ�Access_Store�I�u�W�F�N�g��ID
		 * @param[in]	cFieldName	�Ώۃt�B�[���h��
		 */
		void UpdateASCacheByEditControl( long cControlID, long cObjectID, tstring cFieldName );

		/**
		 * @brief �G�f�B�b�g�R���g���[�����琔�l���擾����
		 * @note	���l�𐳏�Ɏ擾�ł��Ȃ��ꍇ�́Aboost::lexical_cast�̎��s�ɂ���O�����o�����B
		 * @param[in]	cControlID	�ΏۃG�f�B�b�g�R���g���[���̃R���g���[��ID
		 * @return	�擾���ꂽ���l
		 */
		long GetNumFromEditControl( long cControlID );

	/**
	 * @brief �`�F�b�N�{�b�N�X����擾�������ɂ����Access_Store�L���b�V�����X�V����
	 * @param[in]	cControlID	�Ώۃ`�F�b�N�{�b�N�X�̃R���g���[��ID
	 * @param[in]	cObjectID	�Ώ�Access_Store�I�u�W�F�N�g��ID
	 * @param[in]	cFieldName	�Ώۃt�B�[���h��
	 */
	void UpdateASCacheByCheckBox( long cControlID, long cObjectID, tstring cFieldName );

////////////////////////////////////////////////////////////////////////////
//
// CAttrAccessPointDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,				//!< �u��{�����v�^�u�̊e�R���g���[���E�B���h�E
						m_wnd5,m_wnd6,m_wnd7,m_wnd8,m_wnd9,
						m_wnd10;
	CContainedWindow	m_wndInfo1,m_wndInfo2,m_wndInfo3,				//!< �u�{�ݏ��v�^�u�̊e�R���g���[���E�B���h�E
						m_wndInfo4,m_wndInfo5,m_wndInfo6,m_wndInfo7,
						m_wndInfo8,m_wndInfo9,m_wndInfo10,m_wndInfo11,
						m_wndInfo12,m_wndInfo13,m_wndInfo14,m_wndInfo15,
						m_wndInfo16,m_wndInfo17,m_wndInfo18,m_wndInfo19,
						m_wndInfo20,m_wndInfo21;
	CContainedWindow	m_wndAS1,m_wndAS2,m_wndAS3,m_wndAS4,m_wndAS5,	//!< �uAccess_Store�v�^�u�̊e�R���g���[���E�B���h�E�i[Bug5556]�Œǉ��j
						m_wndAS6,m_wndAS7,m_wndAS8,m_wndAS9,m_wndAS10,
						m_wndAS11,m_wndAS12,m_wndAS13,m_wndAS14,m_wndAS15,
						m_wndAS16,m_wndAS17,m_wndAS18,m_wndAS19,m_wndAS20,
						m_wndAS21,m_wndAS22,m_wndAS23;
	CContainedWindow	m_wndTab;										//!< �^�u�R���g���[��
	
	IVSFlexGridPtr		m_ipAccessStoreGrid;	//!< Access_Store���O���b�h�i[Bug5556]�Œǉ��j
	access_store_cache::CASCache	m_ASCache;	//!< Access_Store�e�[�u���̃L���b�V���i[Bug5556]�Œǉ��j

	CString		m_cRoadCode;				//!< ������ł̘H���R�[�h
	CString		m_cRoadSeq;					//!< ������ł̘H�����V�[�P���X
	// �_�C�A���O�̒l�ۑ��p�f�[�^
	LONG	m_lLineClass;					//!< ���ݑI�𒆂̘H����ʃR�[�h
	LONG	m_lRoadCode;					//!< ���ݑI�𒆂̘H���R�[�h
	// ���̒l
	LONG	m_lOrgRoadSequence;				//!< �ŏ��̘H�����V�[�P���X
	LONG	m_lOrgRoadCode;					//!< �ŏ��̘H���R�[�h

	LONG	m_lFeatureIndex;				//!< �A�N�Z�X�|�C���g�_�C�A���O���ł̃t�B�[�`���C���f�b�N�X�ێ��p
	LONG	m_lTableIndex;					//!< �A�N�Z�X�|�C���g�_�C�A���O���ł̃e�[�u���C���f�b�N�X�ێ��p
	LONG	m_lRowIndex;					//!< �A�N�Z�X�|�C���g�_�C�A���O���ł�Row�C���f�b�N�X�ێ��p
	BOOL	m_bForce;						//!< �A�N�Z�X�|�C���g�_�C�A���O���ł̋����ҏW���[�h�ێ��p
	BOOL	m_bEditable;					//!< �A�N�Z�X�|�C���g�_�C�A���O���ł̕ҏW�ł��邩�ǂ����ێ��p

	// �n�C�E�F�C�ҏW�p�e��f�[�^�̃R���e�i
	CHetRoadCodeSet m_cRoadCodeSet;			//!< �H���R�[�h���R���e�i
	CCodeValueDomain m_cLineClassDomain;	//!< �H����ʃR�[�h���R���e�i
	HFONT	m_hFont;						//!< �����t�H���g
	HWND	m_hChildWnd;					//!< �q�_�C�A���O�n���h��

	static const long m_MasterCodeMaxLength;	//!< �`�F�[���X�}�X�^�[�R�[�h�G�f�B�b�g�R���g���[�����̐��l�̍ő包���i[Bug5556]�Œǉ��j
	static const long m_TimeStrMaxLength;		//!< �����i���܂��͕��j��\���G�f�B�b�g�R���g���[�����̐��l�̍ő包���i[Bug5556]�Œǉ��j
	static const long m_ASEditBufferLength;		//!< Access_Store�^�u�̃G�f�B�b�g�R���g���[�����當������擾����ۂɎg�p����o�b�t�@�̒����i[Bug5556]�Œǉ��j
};

#endif // _ATTRACCESSPOINTDLG_

