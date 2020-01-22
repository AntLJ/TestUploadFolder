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

typedef CAttrBaseDlg ATTR_BASE_CLASS;
const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE")); //!< �ĕ`�施�߂��󂯂邽�߂̃��b�Z�[�W

// ���H�����N�������ʃO���b�h��̒�`
namespace search_road_link_grid
{
	enum grid_cols
	{
		kOID,	//!< ���H�����N��OID
		kDist	//!< �x���W�����瓹�H�����N�܂ł̋���(���[�g��)
	};
}

/////////////////////////////////////////////////////////////////////////////
// CAttrWarningSignDlg
class CAttrWarningSignDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg,
	public IDispEventImpl<IDC_GRID_ROAD_LINK, CAttrWarningSignDlg>
{
	// ���H�����N�������ʊi�[�p�}�b�v(key:����, value:���H�����N�̃t�B�[�`��)
	typedef std::multimap<double, CTargetItem> ResultMap;

public:
	/////////////////////////////////////////////////////////////////////////////
	//
	// CAttrWarningSignDlg ���b�Z�[�W�}�b�v
	//
	/////////////////////////////////////////////////////////////////////////////
	BEGIN_MSG_MAP(CAttrWarningSignDlg)
		MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
		MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
		COMMAND_HANDLER(IDC_COMBO_WARNING_SIGN, CBN_DROPDOWN, OnOpeningDropdownList)
		COMMAND_HANDLER(IDC_COMBO_WARNING_SIGN, CBN_CLOSEUP, OnClosedDropdownList)
		COMMAND_HANDLER(IDC_BUTTON_COPY_PICTURE_PATH, BN_CLICKED, OnClickedButtonCopyPicturePath)
		COMMAND_HANDLER(IDC_BUTTON_SEARCH_ROAD_LINK, BN_CLICKED, OnClickedSearchRoadLink)
		COMMAND_HANDLER(IDC_BUTTON_UPDATE_LINK_ID, BN_CLICKED, OnClickedButtonUpdateLinkID)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
		break;
	case IDC_COMBO_WARNING_SIGN:
	case IDC_EDIT_LINK_ID:
	case IDC_EDIT_PICTURE_PATH:
	case IDC_EDIT_SEARCH_METER:
	case IDC_EDIT_SOURCE:
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	END_MSG_MAP()

	BEGIN_SINK_MAP(CAttrWarningSignDlg)
		SINK_ENTRY(IDC_GRID_ROAD_LINK, DISPID_MOUSEUP, OnRoadLinkMouseUp)
	END_SINK_MAP()

		/////////////////////////////////////////////////////////////////////////////
		//
		// CAttrWarningSignDlg ���b�Z�[�W�n���h��
		//
		/////////////////////////////////////////////////////////////////////////////


		LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnClickedButtonCopyPicturePath(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnClickedSearchRoadLink(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnClickedButtonUpdateLinkID(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnOpeningDropdownList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnClosedDropdownList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		void __stdcall OnRoadLinkMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y);

private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWarningSignDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrWarningSignDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

private:
	void CreateControlRelation();
	void SettingWarningSignDropdownList();
	IFeatureClassPtr GetFeatureClass(const CString& strLayerName);
	CTargetItem GetTargetItem(const IFeatureClassPtr& ipFeatureClass, long lOID);
	CTargetItem GetRelatedLink(const IFeatureClassPtr& ipRoadLinkFC);
	void ExecuteSearchRoadLink();
	void InitSearchRoadLinkGrid();
	void UpdateSearchRoadLinkGrid(const ResultMap& resultMap);
	void SearchRoadLink();
	CString CheckSearchMeter();
	BOOL CheckWarningSign(CFeatureDef& cFeatureDef, CString& strErr);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWarningSignDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:

	std::map<int, CContainedWindow> m_mapSubClass;	//!< �e�R���g���[���E�B���h�E
	IVSFlexGridPtr m_spSearchRoadLinkGrid;			//!< ���H�����N�������ʃO���b�h�R���g���[��

	long m_lSearchDist;								//!< �Ή����H�����N�������̋���(�f�t�H���g:10m)

	IFeatureClassPtr m_ipRoadLinkFC;				//!< ���H�����N�̃t�B�[�`���N���X
	CTargetItem m_ipRoadLinkItem;					//!< �R�t�����H�����N�̃t�B�[�`��
	CTargetItem m_ipSelectedRoadLinkItem;			//!< �O���b�h�őI�����Ă��铹�H�����N�̃t�B�[�`��
};
