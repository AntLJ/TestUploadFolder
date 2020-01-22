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

// AttrRoadLinkDlg.h : CAttrRoadLinkDlg �̐錾

#ifndef __ATTRROADLINKDLG_H_
#define __ATTRROADLINKDLG_H_

#include "resource.h"       // ���C�� �V���{��

using namespace std;
typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrRoadLinkDlg
class CAttrRoadLinkDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg,
	public IDispEventImpl<IDC_GRID_NOPASSAGE, CAttrRoadLinkDlg>,	// ���ԋK���p�O���b�h�R���g���[��
	public IDispEventImpl<IDC_GRID_ONEWAY, CAttrRoadLinkDlg>		// ���ԋK���p�O���b�h�R���g���[��
{
public:

private:
	/**
	* �ҏW�r���[�̃��[�h
	*
	* ��ɓ��H������ҏW����ۂɂǂ̃R���g���[����\��������悢���̎w�j�Ƃ��Ďg�p���܂��B
	* CRowsControlRelation::SetControl() �̑�R�A��S�����Ƃ��Ďg�p���܂��B
	* @see CreateControlRelation()
	*/
	enum EditView {
		EDITVIEW_ROAD_KIHON,		//!< ���H�̊�{�����i��P�^�u�j
		EDITVIEW_ROAD_TUIKA,		//!< ���H�̒ǉ������i��Q�^�u�j
		EDITVIEW_ROAD_KOTUKISEI,	//!< ���H�̌�ʋK�������i��R�^�u�j
		EDITVIEW_ROAD_ROSEN,		//!< ���H�̘H�������i��S�^�u�j
		EDITVIEW_ROAD_DRMA,			//!< ���H��DRMA�����i��T�^�u�j
		EDITVIEW_ROAD_PARKING,		//!< ���H�̒��ԏꑮ���i��U�^�u�j
	};

	/**
	* �ꊇ�ύX�p�̑����ێ��^
	*
	*/
	struct BatchChange {
		CString NAME;			//!< ��`��
		CString DISPCLASS;		//!< ���H���
		CString NAVICLASS;		//!< �o�H���
		CString MANAGER;		//!< ���H�Ǘ���
		CString ROADWIDTH;		//!< ����
		CString LINKCLASS;		//!< �����N���
		CString LANE;			//!< �Ԑ���
		CString LEGALSPEED;		//!< �ō����x
		CString ALLEYMAINTE;	//!< �׊X�H��ƔN�x
		CString ROADCODE;		//!< �H���ԍ��R�[�h
		CString ROADNUMBER;		//!< �H���ԍ�
		CString KUBUNCODE;		//!< ��]�敪�R�[�h
		CString LIMITSPEED;		//!< �K�����x�R�[�h
		CString TRIPSPEED;		//!< ���s���x
	};

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadLinkDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
public:
BEGIN_MSG_MAP(CAttrRoadLinkDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_BUTTON_SEARCHPREFCODE, BN_CLICKED, OnClickedSearchPrefButton)           // [bug 12036]
	COMMAND_HANDLER(IDC_BUTTON_SEARCHROADNUMBER, BN_CLICKED, OnClickedSearchRoadNumberButton)   // [bug 12036]
	COMMAND_HANDLER(IDC_BUTTON_SEARCHROADCODE, BN_CLICKED, OnClickedSearchRoadCodeButton)       // [bug 12036]
	COMMAND_HANDLER(IDC_BUTTON_SEARCHSECTIONCODE, BN_CLICKED, OnClickedSearchSectionCodeButton) // [bug 12036]
	COMMAND_HANDLER(IDC_EDIT_RIGHTCHANGE, EN_UPDATE, OnUpdateEditValue)
	COMMAND_HANDLER(IDC_EDIT_LEFTCHANGE, EN_UPDATE, OnUpdateEditValue)
	COMMAND_HANDLER(IDC_EDIT_FLOORLEVEL, EN_UPDATE, OnUpdateEditValue)
	COMMAND_HANDLER(IDC_EDIT_MIDDLEFLOOR_LEVEL, EN_UPDATE, OnUpdateEditValue)
	COMMAND_HANDLER(IDC_CHECK_ISLANDF, BN_CLICKED, OnCheckWideAreaOrIsland)		// [bug 6712]
	COMMAND_HANDLER(IDC_CHECK_WIDEAREAF, BN_CLICKED, OnCheckWideAreaOrIsland)	// [bug 6712]
	COMMAND_HANDLER(IDC_COMBO_BATCHCHANGE, CBN_SELCHANGE, OnChangeBatchChangeButton)	// [bug 9397]

	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
	MESSAGE_HANDLER(WM_NOTIFY, OnNotify)

ALT_MSG_MAP(IDC_COMBO_BATCHCHANGE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_DISPCLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_NAVICLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_MANAGER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ROADWIDTH)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_LINKCLASS1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

ALT_MSG_MAP(IDC_CHECK_ISLANDF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_BYPASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_CARONLYF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_WIDEAREA)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_WIDEAREAF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_UPDOWNC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_LANE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LANE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_LEGALSPEED)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_RIGHTCHANGE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LEFTCHANGE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ALLEYMAINTE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_WORKYEAR)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_WALKABLE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

ALT_MSG_MAP(IDC_COMBO_ONEWAY)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_NOPASSAGE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

// [bug 12036]
ALT_MSG_MAP(IDC_EDIT_ROADCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCHROADCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_PREFCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCHPREFCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ROADNUMBER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCHROADNUMBER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HIGHWAYNUMBER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SECTIONCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCHSECTIONCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)


ALT_MSG_MAP(IDC_COMBO_KUBUNCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_LIMITSPEED)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TRIPSPEED)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)


ALT_MSG_MAP(IDC_COMBO_PARKINGCLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_PARKINGLINKCLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_FLOORLEVEL)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_MIDDLEFLOOR_LEVEL)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

ALT_MSG_MAP(IDC_TAB_ROADATTR)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

	END_MSG_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadLinkDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( !m_ipRule )
			m_ipRule = GetRule();
		// �R���g���[��
		CreateControlRelation();

		// ���ԋK���p�O���b�h�R���g���[��
		CAxWindow wndGrid = GetDlgItem(IDC_GRID_ONEWAY);
		wndGrid.QueryControl(&m_spOnewayGrid);

		wndGrid =GetDlgItem(IDC_GRID_NOPASSAGE);
		wndGrid.QueryControl(&m_spNopassageGrid);

		// �O���b�h�R���g���[������̃C�x���g���E���̂ɕK�v
		AtlAdviseSinkMap(this, TRUE);

		CString strTemp;  //!< pszText�ɓn����������ꎞ�I�Ɋi�[����ϐ�

		// ���H�����ҏW�p�^�u�R���g���[���ɃA�C�e����ǉ����܂�
		HWND hwndTab = GetDlgItem( IDC_TAB_ROADATTR );
		ATLASSERT( ::IsWindow( hwndTab ) );
		TC_ITEM pTabCtrlItem;
		pTabCtrlItem.mask = TCIF_TEXT;
		strTemp = AheLoadString(IDS_BASE);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_ROAD_KIHON, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_EXTENTION);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_ROAD_TUIKA, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_REGULATION);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_ROAD_KOTUKISEI, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_NUMBER);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_ROAD_ROSEN, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_DRMA);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_ROAD_DRMA, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_PARKING);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_ROAD_PARKING, (LPARAM)(&pTabCtrlItem));

		// �T�u�N���X��
		m_wnd0.SubclassWindow(GetDlgItem(IDC_COMBO_DISPCLASS));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_COMBO_NAVICLASS));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_COMBO_MANAGER));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_COMBO_ROADWIDTH));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_COMBO_LINKCLASS1));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_COMBO_BATCHCHANGE));	// [bug 9397] 
		m_wnd6.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));

		m_wndTuika0.SubclassWindow(GetDlgItem(IDC_CHECK_ISLANDF));
		m_wndTuika1.SubclassWindow(GetDlgItem(IDC_CHECK_BYPASS));
		m_wndTuika2.SubclassWindow(GetDlgItem(IDC_CHECK_CARONLYF));
		m_wndTuika3.SubclassWindow(GetDlgItem(IDC_COMBO_WIDEAREA));
		m_wndTuika4.SubclassWindow(GetDlgItem(IDC_COMBO_UPDOWNC));
		m_wndTuika5.SubclassWindow(GetDlgItem(IDC_CHECK_LANE));
		m_wndTuika6.SubclassWindow(GetDlgItem(IDC_EDIT_LANE));
		m_wndTuika7.SubclassWindow(GetDlgItem(IDC_EDIT_RIGHTCHANGE));
		m_wndTuika8.SubclassWindow(GetDlgItem(IDC_EDIT_LEFTCHANGE));
		m_wndTuika9.SubclassWindow(GetDlgItem(IDC_COMBO_ALLEYMAINTE));
		m_wndTuika10.SubclassWindow(GetDlgItem(IDC_EDIT_WORKYEAR));
		m_wndTuika11.SubclassWindow(GetDlgItem(IDC_CHECK_WIDEAREAF));
		m_wndTuika12.SubclassWindow(GetDlgItem(IDC_COMBO_LEGALSPEED));
		m_wndTuika13.SubclassWindow(GetDlgItem(IDC_COMBO_WALKABLE));

		m_wndKisei0.SubclassWindow(GetDlgItem(IDC_COMBO_ONEWAY));
		m_wndKisei1.SubclassWindow(GetDlgItem(IDC_COMBO_NOPASSAGE));

		m_wndRosen0.SubclassWindow(GetDlgItem(IDC_EDIT_ROADCODE));			  // [bug 12036]
		m_wndRosen1.SubclassWindow(GetDlgItem(IDC_BUTTON_SEARCHROADCODE));	  // [bug 12036]
		m_wndRosen2.SubclassWindow(GetDlgItem(IDC_EDIT_PREFCODE));			  // [bug 12036]
		m_wndRosen3.SubclassWindow(GetDlgItem(IDC_BUTTON_SEARCHPREFCODE));	  // [bug 12036]
		m_wndRosen4.SubclassWindow(GetDlgItem(IDC_EDIT_ROADNUMBER));		  // [bug 12036]
		m_wndRosen5.SubclassWindow(GetDlgItem(IDC_BUTTON_SEARCHROADNUMBER));  // [bug 12036]
		m_wndRosen6.SubclassWindow(GetDlgItem(IDC_EDIT_HIGHWAYNUMBER));		  // [bug 12036]
		m_wndRosen7.SubclassWindow(GetDlgItem(IDC_EDIT_SECTIONCODE));		  // [bug 12036]
		m_wndRosen8.SubclassWindow(GetDlgItem(IDC_BUTTON_SEARCHSECTIONCODE)); // [bug 12036]

		m_wndDRMA0.SubclassWindow(GetDlgItem(IDC_COMBO_KUBUNCODE));
		m_wndDRMA1.SubclassWindow(GetDlgItem(IDC_COMBO_LIMITSPEED));
		m_wndDRMA2.SubclassWindow(GetDlgItem(IDC_EDIT_TRIPSPEED));

		m_wndParking0.SubclassWindow(GetDlgItem(IDC_COMBO_PARKINGCLASS));
		m_wndParking1.SubclassWindow(GetDlgItem(IDC_COMBO_PARKINGLINKCLASS));
		m_wndParking2.SubclassWindow(GetDlgItem(IDC_EDIT_FLOORLEVEL));
		m_wndParking3.SubclassWindow(GetDlgItem(IDC_EDIT_MIDDLEFLOOR_LEVEL));

		m_wndTab.SubclassWindow(GetDlgItem(IDC_TAB_ROADATTR));
		// ���H�R�[�h�Ƃ����ɍs���̂��P��ɂ��邽�߂ɁA�e�t���O�̏�����
		m_bRoadCordFirstSearch = FALSE;

		::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0L);

		ChangeWindowMgrMap( m_iChangeType );

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		AtlAdviseSinkMap(this, FALSE);

		return 0;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
	}
	/**
	 * WM_NOTIFY �p�C�x���g�n���h��
	 *
	 * �ȉ��̏������s���܂�
	 * �E�^�u�̐؂�ւ�������������
	 */
	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( wParam == IDC_TAB_ROADATTR ) {
			// ���H�����N�����[�V�������`�F�b�N���^�u�̃n�C���C�g������
			CString strMessage;
			CheckRoadLinkRelation( &strMessage, GetCurrentFeatureDef());
			LPNMHDR pnmh = (LPNMHDR)lParam;
			if( pnmh->code == TCN_SELCHANGE ) {
				// �R���g���[���؂�ւ�
				m_iChangeType = TabCtrl_GetCurSel( pnmh->hwndFrom );
				
				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
				m_lTabNum = m_iChangeType;	// ���H�^�u�̏ꍇ�̂݃^�u��ID��ێ�
				ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_lTabNum, m_bForce, m_bEditable );

				// �e�^�u�ł̏���������
				switch( m_lTabNum )
				{
				case EDITVIEW_ROAD_ROSEN:
					if( !m_bRoadCordFirstSearch )
					{
						// ���H���̂��o�� [bug 12036]
						SetRoadName(IDC_EDIT_ROADCODE, IDC_EDIT_NAMEKANJI, IDC_EDIT_DISPLAYKANJI, IDC_EDIT_SHIELDNUMBER);
						::SetFocus( GetDlgItem(IDC_EDIT_ROADCODE) );
						m_bRoadCordFirstSearch = TRUE;

						// �s���{�������o�� [bug 12036]
						SetPrefName(IDC_EDIT_PREFCODE, IDC_EDIT_PREFNAME);

						// �������̂��o�� [bug 12036]
						SetPrefRoadName(IDC_EDIT_PREFCODE, IDC_EDIT_ROADNUMBER, IDC_EDIT_PREFNAME, IDC_EDIT_PREFROADNAME);

						// ��ԕʏ̂��o�� [bug 12036]
						SetSectionName(IDC_EDIT_SECTIONCODE,IDC_EDIT_SECTIONNAME);
					}
					break;
				case EDITVIEW_ROAD_KOTUKISEI:
					SetDlgStatus(IDC_COMBO_ONEWAY, FALSE );
					SetDlgStatus(IDC_COMBO_NOPASSAGE, FALSE );
					break;
				case EDITVIEW_ROAD_TUIKA:
					AdjustTuikaTabLayout();
					break;
				case EDITVIEW_ROAD_KIHON:
					// �ꊇ�ύX�p�̃R���{�ɒl���Z�b�g [bug 9397] 
					AddComboValue();

					// �ǂ����̒l���ύX����Ă���ꍇ�͈ꊇ�ύX�R���{���g�p�s�Ƃ��� [bug 9449]
					// �ҏW�J�n����Ă��Ȃ����C����������ύX���Ȃ��ibug 9719�j
					if( IsEditableFeature() && m_cArcHelper.IsStartEditing() )
						SetDlgStatus(IDC_COMBO_BATCHCHANGE, !Changed() );
					break;
				default:
					break;
				}

				bHandled = TRUE;

				if( IsEditableFeature() && m_cArcHelper.IsStartEditing() )
					SetForcusAfterTabChange();
				return 1;
			}
		}

		if( wParam == 0 ) {
			// ���H�����N�����[�V�������`�F�b�N���^�u�̃n�C���C�g������
			CString strMessage;
			CheckRoadLinkRelation( &strMessage, GetCurrentFeatureDef());
			HWND hwndTab = GetDlgItem( IDC_TAB_ROADATTR );
			if( lParam == -2 || lParam == -1)
			{
				// �R���g���[���؂�ւ�
				if( lParam == -2 )
				{
					if( m_iChangeType + 1 == 6 )
					{
						m_iChangeType = 0;
						::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
					}
					else
					{
						m_iChangeType = m_iChangeType + 1;
						::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
					}
				}
				else if( lParam == -1 )
				{
					if( m_iChangeType - 1 == -1 )
					{
						m_iChangeType = 5;
						::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
					}
					else
					{
						m_iChangeType = m_iChangeType - 1;
						::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
					}
				}
				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
				m_lTabNum = m_iChangeType;	// ���H�^�u�̏ꍇ�̂݃^�u��ID��ێ�
				ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_lTabNum, m_bForce, m_bEditable );

				if( m_lTabNum == 3 && m_bRoadCordFirstSearch == FALSE)
				{
					// ���H���̂��o��
					SetRoadName(IDC_EDIT_ROADCODE, IDC_EDIT_NAMEKANJI, IDC_EDIT_DISPLAYKANJI, IDC_EDIT_SHIELDNUMBER);
					::SetFocus( GetDlgItem(IDC_EDIT_ROADCODE) );
					m_bRoadCordFirstSearch = TRUE;

					// �s���{�������o�� [bug 12036]
					SetPrefName(IDC_EDIT_PREFCODE, IDC_EDIT_PREFNAME);

					// �������̂��o�� [bug 12036]
					SetPrefRoadName(IDC_EDIT_PREFCODE, IDC_EDIT_ROADNUMBER, IDC_EDIT_PREFNAME, IDC_EDIT_PREFROADNAME);

					// ��ԕʏ̂��o�� [bug 12036]
					SetSectionName(IDC_EDIT_SECTIONCODE, IDC_EDIT_SECTIONNAME);
				}
				bHandled = TRUE;
				SetForcusAfterTabChange();

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
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )) {
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			INT nID = (INT)LOWORD(wParam);
			// �l�ύX
			m_cControlRel.ControlChanged( nID );
			// ���H�����N�����[�V�������`�F�b�N���^�u�̃n�C���C�g������
			CString strMessage;
			CheckRoadLinkRelation( &strMessage, GetCurrentFeatureDef());
			// 2�����b�V�����E�����ׂ��ł��铹�H�̏ꍇ�͔��Α��̓��H�����������ɂ��Ȃ��Ă͂����Ȃ�����
			CopyMeshRoad();
			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
			// �ǂ����̒l���ύX����Ă���ꍇ�͈ꊇ�ύX�R���{���g�p�s�Ƃ��� [bug 9449]
			SetDlgStatus(IDC_COMBO_BATCHCHANGE, !Changed() );
		}

		return 0;
	}

	// �H�����̃R�[�h�ɑ΂���ucode�����v�{�^���������ꂽ���̏��� [bug 12036]
	LRESULT OnClickedSearchRoadCodeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// �s���{���R�[�h�ɑ΂���ucode�����v�{�^���������ꂽ���̏��� [bug 12036]
	LRESULT OnClickedSearchPrefButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// ���H�ԍ��ɑ΂���uNo.�����v�{�^���������ꂽ���̏��� [bug 12036]
	LRESULT OnClickedSearchRoadNumberButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// ��ԕʏ̃R�[�h�ɑ΂���ucode�����v�{�^���������ꂽ���̏��� [bug 12036]
	LRESULT OnClickedSearchSectionCodeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/**
	 * �E���������̓G�f�B�b�g�{�b�N�X�p�C�x���g�n���h��
	 * �����������̓G�f�B�b�g�{�b�N�X�p�C�x���g�n���h��
	 * ���ԏ�K�w���̓G�f�B�b�g�{�b�N�X�p�C�x���g�n���h��
	 * ���ԏ�K�w�ԃ��x�����̓G�f�B�b�g�{�b�N�X�p�C�x���g�n���h��
	 *
	 * �G�f�B�b�g�{�b�N�X�̕��������삳���ꍇ�A�����ŏ���
	 * ���͕������������ĕs�������͓��͂����Ȃ��悤�ɂ���
	 */
	LRESULT OnUpdateEditValue(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// ���͂̃`�F�b�N
		CheckInputValue( hWndCtl, wID );
		// ���H�����N�����[�V�������`�F�b�N���^�u�̃n�C���C�g������
		CString strMessage;
		CheckRoadLinkRelation( &strMessage, GetCurrentFeatureDef());
		// 2�����b�V�����E�����ׂ��ł��铹�H�̏ꍇ�͔��Α��̓��H�����������ɂ��Ȃ��Ă͂����Ȃ�����
		CopyMeshRoad();
		InvalidateRect( NULL, FALSE );
		// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
		SetButton( Changed() );

		return 0;
	}

	LRESULT OnCheckWideAreaOrIsland(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/**
	 * �ꊇ�ύX�R���{�{�b�N�X�p�C�x���g�n���h��
	 *
	 * �R���{�{�b�N�X�̒l��ύX�����ꍇ�A�����ŏ���
	 * �ύX�����l�ɍ��킹�Ĉꊇ�ύX�t�B�[���h��S�čX�V����
	 */
	LRESULT OnChangeBatchChangeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// �I�����ꂽ�ꊇ�ύX�̏����擾
		CComboBox cbBatchChange(GetDlgItem(IDC_COMBO_BATCHCHANGE));
		m_nIdx = cbBatchChange.GetCurSel();

		if( m_nIdx == CB_ERR )
			return 0;
		int key = cbBatchChange.GetItemData(m_nIdx);
		BatchChange bChange = m_mapBatchChange[key];

		// �ꊇ�ύX�̏�񂩂�A���ۂɑ������Z�b�g����
		SetBatchValue( IDC_COMBO_DISPCLASS, bChange.DISPCLASS );	// ���H���
		SetBatchValue( IDC_COMBO_NAVICLASS, bChange.NAVICLASS );	// �o�H���
		SetBatchValue( IDC_COMBO_MANAGER,	bChange.MANAGER );		// ���H�Ǘ���
		SetBatchValue( IDC_COMBO_ROADWIDTH, bChange.ROADWIDTH );	// ����
		SetBatchValue( IDC_COMBO_LINKCLASS1,bChange.LINKCLASS );	// �����N���
		SetBatchValue( IDC_EDIT_LANE,		bChange.LANE );			// �Ԑ���
		SetBatchValue( IDC_COMBO_LEGALSPEED,bChange.LEGALSPEED );	// �ō����x
		SetBatchValue( IDC_COMBO_ALLEYMAINTE,bChange.ALLEYMAINTE );	// �׊X�H��ƔN�x
		SetBatchValue( IDC_EDIT_ROADCODE,	bChange.ROADCODE );		// �H���ԍ��R�[�h
		SetBatchValue( IDC_EDIT_ROADNUMBER, bChange.ROADNUMBER );	// �H���ԍ�
		SetBatchValue( IDC_COMBO_KUBUNCODE, bChange.KUBUNCODE );	// ��]�敪�R�[�h
		SetBatchValue( IDC_COMBO_LIMITSPEED,bChange.LIMITSPEED );	// �K�����x�R�[�h
		SetBatchValue( IDC_EDIT_TRIPSPEED,	bChange.TRIPSPEED );	// ���s���x

		// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
		SetButton( Changed() );

		// �ꊇ�ύX�ŕύX�����ۂ́A�ꊇ�ύX�R���{���g�p�s�Ƃ��� [bug 9449]
		SetDlgStatus(IDC_COMBO_BATCHCHANGE, !Changed() );

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadLinkDlg �C�x���g�V���N�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_SINK_MAP(CAttrRoadLinkDlg)
	///Make sure the Event Handlers have __stdcall calling convention
	SINK_ENTRY(IDC_GRID_NOPASSAGE, DISPID_MOUSEDOWN, OnMouseDownNopassageGrid)
	SINK_ENTRY(IDC_GRID_ONEWAY, DISPID_MOUSEDOWN, OnMouseDownOnewayGrid)
	SINK_ENTRY(IDC_GRID_NOPASSAGE, 0x16, OnStartEditNoPassageGrid)
	SINK_ENTRY(IDC_GRID_ONEWAY, 0x16, OnStartEditOnewayGrid)
END_SINK_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// VS-FlexGrid �C�x���g�n���h��
//
/////////////////////////////////////////////////////////////////////////////
	/**
	 * ���ԋK���p�ŃZ�����N���b�N���ꂽ���ɔ�������C�x���g�n���h��
	 *
	 * 0�J�����ڂ̃Z���̐F�ύX�⃁�j���[�̕\�����s���܂�
	 */
	VOID __stdcall OnMouseDownNopassageGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
	{
		MouseDownTimeRegGrid( IDC_GRID_NOPASSAGE, m_spNopassageGrid, m_lCurrentNopassageIndex, TIME_NOPASSAGE_TABLE_NAME, Button, Shift, X, Y );
	}
	VOID __stdcall OnMouseDownOnewayGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
	{
		MouseDownTimeRegGrid( IDC_GRID_ONEWAY, m_spOnewayGrid, m_lCurrentOnewayIndex, TIME_ONEWAY_TABLE_NAME, Button, Shift, X, Y );
	}
	VOID __stdcall OnStartEditNoPassageGrid(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
	{
		*Cancel = true;
	}
	VOID __stdcall OnStartEditOnewayGrid(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
	{
		*Cancel = true;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadLinkDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum();
	virtual void SetTabNum(LONG lTabNum);
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrRoadLinkDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID);
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void ChangeWindowMgrMap( INT iIndex );
	void CreateControlRelation();

	/**
	* @brief �J�[�\�����擾���� [bug 12036]
	*
	* @param ipTable		[in]	�����Ώۃe�[�u����ITablePtr
	* @param ipCursor		[out]	�擾�����J�[�\�����i�[����_ICursorPtr
	* @param strWhereClause	[in]	�����ɗp����WHERE��
	* @param strSubFields	[in]	�����ɗp����t�B�[���h�w���
	*
	* @retval	true : �擾����
	* @retval	false: �擾���s
	*/
	bool GetCursor(const ITablePtr& ipTable, _ICursorPtr& ipCursor, const CString& strWhereClause, const CString& strSubFields);

	/**
	* @brief �G�f�B�b�g�{�b�N�X�ɓ��͂��ꂽ���l���擾���� [bug 12036]
	*
	* @param nCodeID		[in]	�l�����͂���Ă���G�f�B�b�g�R���g���[����ID
	*/
	long GetWindowNumberText(int nCodeID);

	/**
	* @brief �H���������s���A�H������\������ [bug 12036]
	*
	* �H�����̃e�[�u���ł��� ROAD_CODE_LIST ���疼�̂��������A��2�����ȉ��Ŏ������e�R���g���[���ɕ\�����܂�
	*
	* @param nCodeID		[in]	�H�����̃R�[�h�������Ă���G�f�B�b�g�R���g���[����ID
	* @param nNameLabelID	[in]	�H�����̂�\������X�^�e�B�b�N�R���g���[����ID
	* @param nDispLabelID	[in]	�H���ʏ̂�\������X�^�e�B�b�N�R���g���[����ID
	* @param nShieldNumberLabelID	[in]	�s�s�����ԍ���\������X�^�e�B�b�N�R���g���[����ID
	*/
	void SetRoadName(int nCodeID, int nNameLabelID, int nDispLabelID, int nShieldNumberLabelID);

	/**
	* @brief �s���{�����������s���A�s���{�����̂�\������ [bug 12036]
	*
	* �s���{�������X�g ���猟�����A��2�����Ŏ������R���g���[���ɕ\�����܂�
	*
	* @param nCodeID	[in]	�s���{���R�[�h�������Ă���G�f�B�b�g�R���g���[����ID
	* @param nLabelID	[in]	�s���{�����̂�\������X�^�e�B�b�N�R���g���[����ID
	*/
	void SetPrefName(int nCodeID, int nLabelID);

	/**
	* @brief CITY_ADMIN����PREF_CODE���擾���A�\������ [bug 12036]
	*
	* @param nCodeID			[in]	�s���{���R�[�h�������Ă���G�f�B�b�g�R���g���[����ID
	* @param nRoadNumberID		[in]	���H�ԍ��������Ă���G�f�B�b�g�R���g���[����ID
	*/
	void SetPrefCodeFromCityAdmin(int nCodeID);

	/**
	* @brief ���H�����N�����݂���CITY_ADMIN�|���S����CITYCODE�t�B�[���h����PREF_CODE���擾���� [bug 12036]
	*/
	long GetPrefCodeFromCityAdmin();

	/**
	* @brief �s���{�������̌������s���A�s���{�������̂�\������ [bug 12036]
	*
	* �s���{�������X�g ���疼�̂��������A��3�����Ŏ������R���g���[���ɕ\�����܂�
	* �s���{�������̃��X�g�ł��� PREF_ROAD_LIST ���疼�̂��������A��4�����Ŏ������R���g���[���ɕ\�����܂�
	*
	* @param nPrefCodeID		[in]	�s���{���R�[�h�������Ă���G�f�B�b�g�R���g���[����ID
	* @param nRoadNumberID		[in]	���H�ԍ��������Ă���G�f�B�b�g�R���g���[����ID
	* @param nPrefNameLabelID	[in]	�s���{�����̂�\������X�^�e�B�b�N�R���g���[����ID
	* @param nPrefRoadLabelID	[in]	�s���{�������̂�\������X�^�e�B�b�N�R���g���[����ID
	*/
	void SetPrefRoadName(int nPrefCodeID, int nRoadNumberID, int nPrefNameLabelID, int nPrefRoadLabelID);

	/**
	* @brief ��ԕʏ̃R�[�h�������s���A��ԕʏ̖���\������ [bug 12036]
	*
	* ��ԕʏ̃e�[�u���ł��� SECTION_CODE_LIST ���猟�����A��2�����Ŏ������R���g���[���ɕ\�����܂�
	*
	* @param nCodeID	[in]	��ԕʏ̃R�[�h�������Ă���G�f�B�b�g�R���g���[����ID
	* @param nLabelID	[in]	��ԕʏ̂�\������X�^�e�B�b�N�R���g���[����ID
	*/
	void SetSectionName(int nCodeID, int nLabelID);

	/**
	* @brief �G�f�B�b�g�{�b�N�X�ɓ��͂��ꂽ���l�̃`�F�b�N���s�� [bug 12036]
	*
	* @param nCodeID	[in]	�l�����͂��ꂽ�G�f�B�b�g�R���g���[����ID
	*
	* @retval	true : �ݒ�l��
	* @retval	false: �ݒ�l�O
	*/
	bool CheckNumVal(int nCodeID);

	/**
	* @brief �H�����̃R�[�h�܂��͋�ԕʏ̃R�[�h�̒l���`�F�b�N [bug 12036]
	*
	* @param	lNumVal	[in]	�H�����̃R�[�h�܂��͋�ԕʏ̃R�[�h�̒l
	*
	* @retval	true : �ݒ�l��
	* @retval	false: �ݒ�l�O
	*/
	bool CheckRoadCodeSectionCode(long lNumVal);

	/**
	* @brief �s���{���R�[�h�̒l���`�F�b�N [bug 12036]
	*
	* @param	lPrefCode	[in]	�H�����̃R�[�h�l
	*
	* @retval	true : �ݒ�l��
	* @retval	false: �ݒ�l�O
	*/
	bool CheckPrefCode(long lPrefCode);

	/**
	* @brief �s���{���R�[�h�A���H�ԍ��̒l���`�F�b�N [bug 12036]
	*
	* @param	lRoadNo		[in]	���H�ԍ�
	*
	* @retval	true : �ݒ�l��
	* @retval	false: �ݒ�l�O
	*/
	bool CheckRoadNo(long lRoadNo);

	void MouseDownTimeRegGrid( INT nID, IVSFlexGridPtr spGrid, LONG& lIndex, LPCTSTR lpcszTableName, SHORT Button, SHORT Shift, FLOAT X, FLOAT Y );
	BOOL CheckOneWayTimeReg();
	BOOL TimeRegErrorCheck(list<CRelationalTableDef>* pRelTableDef, INT iOneWay);

	/**
	* @brief ���H��ʂɉ����ēs���{���R�[�h�A���H�ԍ��̓��͂ɖ�肪�Ȃ������`�F�b�N���� [bug 12036]
	*
	*        ���H��ʂ�(2,4,103,104,106)�̂Ƃ��̂݁A�s���{���R�[�h���͉\
	*        ���H��ʂ�(1,2,4,103,104,106)�̂Ƃ��̂݁A���H�ԍ����͉\
	*         1 : ��ʍ���
	*         2 : ��v�n����(�s�{��)
	*         4 : ��ʓs���{����
	*        103: ��ʍ���(�L��)
	*        104: ��v�n����(�s���{��)(�L��)
	*        106: ��ʓs���{����(�L��)
	*
	* @param strMsg		[out]	���[�j���O���b�Z�[�W�̓��e
	*
	* @retval	true : �s���{���R�[�h�A���H�ԍ�����͉\
	* @retval	false: �s���{���R�[�h�A���H�ԍ�����͕s��
	*/
	bool CheckPrefCodeRoadNo(CString& strMsg, CFeatureDef& pFeatureDef);
	LONG CheckRoadLinkRelation(CString *strMessage, CFeatureDef *pFeatureDef);

	/**
	* @brief �ԍ��^�u�ɓ��͂��ꂽ�e�R�[�h�l�ɊY�����̂����݂��邩�`�F�b�N���� [bug 12036]
	*
	* @param strMsg		[out]	���[�j���O���b�Z�[�W�̓��e
	*
	* @retval	true : �Y�����̂���
	* @retval	false: �Y�����̂Ȃ�
	*/
	bool CheckNameExists(CString& strMsg, CFeatureDef& pFeatureDef);

	/**
	* @brief ���͂��ꂽ�H�����̃R�[�h�ɊY�����̂����݂��邩�`�F�b�N���� [bug 12036]
	*
	* @param strMsg		[out]	���[�j���O���b�Z�[�W�̓��e
	*/
	void CheckRoadCodeNameExists(CString& strMsg, CFeatureDef& pFeatureDef);

	/**
	* @brief ���͂��ꂽ�s���{���R�[�h�A���H�ԍ��ɊY�����̂����݂��邩�`�F�b�N���� [bug 12036]
	*
	* @param strMsg		[out]	���[�j���O���b�Z�[�W�̓��e
	*/
	void CheckPrefRoadNameExists(CString& strMsg, CFeatureDef& pFeatureDef);

	/**
	* @brief ���͂��ꂽ��ԕʏ̃R�[�h�ɊY�����̂����݂��邩�`�F�b�N���� [bug 12036]
	*
	* @param strMsg		[out]	���[�j���O���b�Z�[�W�̓��e
	*/
	void CheckSectionNameExists(CString& strMsg, CFeatureDef& pFeatureDef);
	void SetForcusAfterTabChange();
	void TabHighLight();
	void CopyMeshRoad();
	void AdjustTuikaTabLayout();
	HRESULT CreateActiveXControls(UINT nID);
	void LoadIniFile();
	void GetRegCode();
	void AddComboValue();
	void SetBatchValue(INT nID, CString& Value);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadLinkDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	LONG				m_lTabNum;					//!< ���H�����^�u�̕\���ʒu�ۑ��p
	INT					m_iChangeType;				//!< �R���g���[���؂�ւ��p

private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,m_wnd5,m_wnd6;  //!< �e�R���g���[���E�B���h�E
	CContainedWindow	m_wndTuika0,m_wndTuika1,m_wndTuika2,m_wndTuika3,
						m_wndTuika4,m_wndTuika5,m_wndTuika6,m_wndTuika7,
						m_wndTuika8,m_wndTuika9,m_wndTuika10,m_wndTuika11,
						m_wndTuika12,m_wndTuika13;                         //!< �e�R���g���[���E�B���h�E
	CContainedWindow	m_wndKisei0,m_wndKisei1;                           //!< �e�R���g���[���E�B���h�E
	CContainedWindow	m_wndRosen0,m_wndRosen1,m_wndRosen2,m_wndRosen3,
						m_wndRosen4,m_wndRosen5,m_wndRosen6,m_wndRosen7,
						m_wndRosen8;                                       //!< �e�R���g���[���E�B���h�E
	CContainedWindow	m_wndDRMA0,m_wndDRMA1,m_wndDRMA2;                  //!< �e�R���g���[���E�B���h�E
	CContainedWindow	m_wndParking0,m_wndParking1,m_wndParking2,m_wndParking3; //!< �e�R���g���[���E�B���h�E
	CContainedWindow	m_wndTab;                                          //!< �^�u�̃R���g���[���E�B���h�E

	LONG				m_lCurrentNopassageIndex;	//!< ���ԋK����0�J�����ڐF�ύX�߂��p�C���f�b�N�X�ԍ��ێ��p
	LONG				m_lCurrentOnewayIndex;		//!< ���ԋK����0�J�����ڐF�ύX�߂��p�C���f�b�N�X�ԍ��ێ��p
	IVSFlexGridPtr		m_spNopassageGrid;			//!< ���ԋK���p�̃O���b�h
	IVSFlexGridPtr		m_spOnewayGrid;				//!< ���ԋK���p�̃O���b�h

	BOOL	m_bRoadCordFirstSearch;					//!< ���H���̂�1�x�����������ǂ����̃t���O
	LONG	m_lFeatureIndex;						//!< ���H�����N�_�C�A���O���ł̃t�B�[�`���C���f�b�N�X�ێ��p
	LONG	m_lTableIndex;							//!< ���H�����N�_�C�A���O���ł̃e�[�u���C���f�b�N�X�ێ��p
	LONG	m_lRowIndex;							//!< ���H�����N�_�C�A���O���ł�Row�C���f�b�N�X�ێ��p
	BOOL	m_bForce;								//!< ���H�����N�_�C�A���O���ł̋����ҏW���[�h�ێ��p
	BOOL	m_bEditable;							//!< ���H�����N�_�C�A���O���ł̕ҏW�ł��邩�ǂ����ێ��p

	BOOL				m_bIsRoadOnMesh;			//!< ���H�����N�����b�V�����ׂ��ł��邩�ǂ����̃t���O
	CTargetItems		m_cRoadsOnMesh;				//!< ���b�V�����ׂ��ł��铹�H�����N�ێ��p
	list<CFeatureDef>	m_pRoadOnMeshList;			//!< ���b�V�����ׂ��ł��铹�H�����N��FeatureDef�ێ��p

	HWND				m_hChildWnd;				//!< �q�_�C�A���O�n���h��
	HWND				m_hAttributeDlgWnd;			//!< �e�E�B���h�n���h��

	CString				m_strIniPath;				//!< Ini�t�@�C���p�X [bug 9397] 
	int					m_nIdx;					//!< �ꊇ�ύX�p�̒�`���C���f�b�N�X�ێ��p [bug 9397] 

	map<int, BatchChange>	m_mapBatchChange;	//!< �ꊇ�ύX�v�f�擾�p [bug 9397]
};

#endif //__ATTRROADLINKDLG_H_
