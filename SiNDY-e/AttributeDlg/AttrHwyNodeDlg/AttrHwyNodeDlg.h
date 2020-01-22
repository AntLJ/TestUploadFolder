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

// AttrHwyNodeDlg.h : CAttrHwyNodeDlg �̐錾

#ifndef __ATTRHWYNODEDLG_H_
#define __ATTRHWYNODEDLG_H_

#include <atlframe.h>			// for COwnerDraw
#include "resource.h"
#include "RoadCodeListInfo.h"

const unsigned int WM_REFRESHFEATURE  = RegisterWindowMessage(_T("WM_REFRESHFEATURE"));       //!< �ĕ`��̈˗����󂯂邽�߂̃��b�Z�[�W
typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrHwyNodeDlg
class CAttrHwyNodeDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg,
	public COwnerDraw<CAttrHwyNodeDlg>	// ���������[�����C���[�W�̕`��̂��߁A�_�C�A���O���I�[�i�[�h���[�ɑΉ�������
{
public:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHwyNodeDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrHwyNodeDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
	COMMAND_HANDLER(IDC_COMBO_HWYNODE_ROADCLASS_REF, CBN_SELCHANGE, OnSelchangeRsel_class)
	COMMAND_HANDLER(IDC_COMBO_HWYNODE_ROADNAME_REF, CBN_SELCHANGE, OnSelchangeRsel_Name)
	COMMAND_HANDLER(IDC_COMBO_SEQUENCE, CBN_SELCHANGE, OnSelchangeRoadSequence)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
	CHAIN_MSG_MAP(COwnerDraw<CAttrHwyNodeDlg>)

ALT_MSG_MAP(IDC_CHECK_ISHWMODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HWYNODE_ROADCLASS_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HWYNODE_ROADNAME_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_SEQUENCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_DIRECTION)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_JUNCTION)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_DUMMY)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_GUIDE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_TOLLCLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_TOLLFUNC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_FROMNODE_ID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_TONODE_ID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_NOETC_CAR)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ETC_FACIL)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_TOLLFUNC_ETC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_NOGUIDETOLL_ETC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_TAB_HWYNODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TOLLGATELANE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HWYGUIDE_NUM)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ETC_TOLLGATE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHwyNodeDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_ipFacilInfoTable = NULL;
		m_ipRoadCodeListTable = NULL;
		m_ipRoadNodeTable = NULL;

		// �r�b�g�}�b�v�̓ǂݍ���
		m_ImpassableImg.LoadBitmap(IDB_BITMAP_IMPASSABLE);
		m_PublicImg.LoadBitmap(IDB_BITMAP_PUBLIC);
		m_EtcImg.LoadBitmap(IDB_BITMAP_ETC);
		m_MixedImg.LoadBitmap(IDB_BITMAP_MIXED);
		m_SwitchImg.LoadBitmap(IDB_BITMAP_SWITCH);
		m_UnknownImg.LoadBitmap(IDB_BITMAP_UNKNOWN);
		m_InvalidImg.LoadBitmap(IDB_BITMAP_INVALID);

		// �t�H���g�̍쐬
		m_TollGateLaneImgFont.CreatePointFont(75, _T("Arial"));

		// �R���g���[���ƃt�B�[���h�̊֘A�t��
		CreateControlRelation();

		// ���������[�����G�f�B�b�g�R���g���[���̌�������
		CEdit aTollGateLaneEdit = GetDlgItem(IDC_EDIT_TOLLGATELANE);
		aTollGateLaneEdit.SetLimitText(m_TollGateLaneStrMaxLength);

		// HWYNODE �p�^�u�R���g���[��
		CTabCtrl cTab( GetDlgItem( IDC_TAB_HWYNODE ) );
		cTab.AddItem( AheLoadString(IDS_BASEINFO) );
		cTab.AddItem( AheLoadString(IDS_TOLLGATELANEINFO) );
		cTab.AddItem( AheLoadString(IDS_ETC_TOLLGATEINFO) );

		// �T�u�N���X��
		m_wnd0.SubclassWindow(GetDlgItem(IDC_CHECK_ISHWMODE));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_COMBO_HWYNODE_ROADCLASS_REF));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_COMBO_HWYNODE_ROADNAME_REF));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_COMBO_SEQUENCE));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_COMBO_DIRECTION));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_COMBO_JUNCTION));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_CHECK_DUMMY));
		m_wnd8.SubclassWindow(GetDlgItem(IDC_CHECK_GUIDE));
		m_wnd9.SubclassWindow(GetDlgItem(IDC_COMBO_TOLLCLASS));
		m_wnd10.SubclassWindow(GetDlgItem(IDC_COMBO_TOLLFUNC));
		m_wnd11.SubclassWindow(GetDlgItem(IDC_COMBO_FROMNODE_ID));
		m_wnd12.SubclassWindow(GetDlgItem(IDC_COMBO_TONODE_ID));	
		m_wnd13.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wnd16.SubclassWindow(GetDlgItem(IDC_CHECK_NOETC_CAR));
		m_wnd17.SubclassWindow(GetDlgItem(IDC_COMBO_ETC_FACIL));
		m_wnd18.SubclassWindow(GetDlgItem(IDC_COMBO_TOLLFUNC_ETC));
		m_wnd19.SubclassWindow(GetDlgItem(IDC_CHECK_NOGUIDETOLL_ETC));
		m_wnd20.SubclassWindow(GetDlgItem(IDC_EDIT_TOLLGATELANE));
		m_wnd21.SubclassWindow(GetDlgItem(IDC_EDIT_HWYGUIDE_NUM));
		m_wnd22.SubclassWindow(GetDlgItem(IDC_EDIT_ETC_TOLLGATE));

		m_wndTab.SubclassWindow(GetDlgItem(IDC_TAB_HWYNODE) );

		cTab.SetCurSel( m_ChangeType );
		ChangeWindowMgrMap( m_ChangeType );

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}

	/**
	 * WM_REFRESHFEATURE �p�C�x���g�n���h��
	 *
	 * �ĕ`��̃C�x���g�������_�C�A���O���瑗���Ă��܂�
	 * Refresh()�ōĕ`�悵�܂�
	 */
	LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		if( wParam == 1 )
		{
			if( m_ipFromRoadGeom )
			{
				IEnvelopePtr ipFeatEnv;
				m_ipFromRoadGeom->get_Envelope( &ipFeatEnv );
				m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, ipFeatEnv );
			}
			m_ipFromRoadGeom = NULL;
			if( m_ipToRoadGeom )
			{
				IEnvelopePtr ipFeatEnv;
				m_ipToRoadGeom->get_Envelope( &ipFeatEnv );
				m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, ipFeatEnv );
			}
			m_ipToRoadGeom = NULL;
		}
		else
			Refresh();

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
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );

			switch (nTargetControl)
			{
				// �n�C�E�F�C���[�h�t���O���ύX���ꂽ�痬���E���o�m�[�h�̃R���{�{�b�N�X�̃I���I�t�؂�ւ�
				case IDC_CHECK_ISHWMODE:
					SetInOutNodeComboBox();
					break;
				// �����E���o�m�[�h�̃R���{�{�b�N�X�̒l���ύX���ꂽ��Ή���������ĕ`�悷��
				case IDC_COMBO_FROMNODE_ID:
				case IDC_COMBO_TONODE_ID:
					DrawInOutNode();
					break;
				// [Bug6217] ��ʎԗ��Ή��t���O��ETC�{�݃R�[�h�̏�Ԃɂ���āA���ꂼ��Ɉˑ�����R���g���[����Enable/Disable��؂�ւ���
				case IDC_CHECK_NOETC_CAR:
					ChangeDependentControlsOnNoEtcCarFlag();
					break;
				case IDC_COMBO_ETC_FACIL:
					ChangeDependentControlsOnEtcFacilCode();
					break;
				// ���������[�����G�f�B�b�g�R���g���[�����ύX���ꂽ��Ή�����C���[�W���ĕ`��
				case IDC_EDIT_TOLLGATELANE:
					DrawLaneImage();
					break;
			}

			// ��������ʁA�������@�\�ɃG���[���Ȃ������ׂ�
			CheckItems();
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}

		return 0;
	}

	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * �H����ʎQ�Ɨp���ύX���ꂽ�Ƃ��̃C�x���g�n���h��
	 *
	 * �H�����̎Q�Ɨp�ƁA�H���R�[�h�̒l��ύX���܂�
	 */
	LRESULT OnSelchangeRsel_class(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled)
	{
		// �I�����ꂽ�H����ʂ̎擾
		CComboBox cbLineClass = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_HWYNODE_ROADCLASS_REF));
		int nIdx = cbLineClass.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nLineClass = cbLineClass.GetItemData(nIdx);
		// �H���R�[�h�̍ĕ`��
		if(! RedrawRoadNameList(0))
			return 0;
		// �H���R�[�h�̐ݒ�
		SetRoadCode(m_nRoadCode);

		// �H�����V�[�P���X�̒l��0�ɐݒ肷��
		CComVariant		vaRoadSequence;
		CFieldDef*		pFieldDef;
		pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_SEQ") );
		pFieldDef->m_vaValue = 0L;
		m_cRoadSeq.Format(_T("%d"), 0 );	

		ChangeRoadSeqCombobox();

		// �H���R�[�h�̒l�ɏ]���ăn�C�E�F�C���[�h�t���O�̃`�F�b�N�{�b�N�X���X�V�i[Bug 6217]�Ή����ɒǉ��j
		ChangeHwyModeCheckBox(m_nRoadCode);

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
		CComboBox cbRoadName = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_HWYNODE_ROADNAME_REF));
		int nIdx = cbRoadName.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nRoadCode = cbRoadName.GetItemData(nIdx);
		// �H���R�[�h�̐ݒ�
		SetRoadCode(m_nRoadCode);

		// �H�����V�[�P���X�̒l��0�ɐݒ肷��
		CFieldDef*		pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_SEQ") );
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
		// �I�����ꂽ�H�����V�[�P���X�̎擾
		CComboBox cbRoadSequence = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_SEQUENCE));
		int nIdx = cbRoadSequence.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nRoadSequence = cbRoadSequence.GetItemData(nIdx);
		m_cRoadSeq.Format(_T("%d"), m_nRoadSequence );	

		SetRoadSequence();

		return 0;
	}

	void DrawItem(LPDRAWITEMSTRUCT lpdis);


/////////////////////////////////////////////////////////////////////////////
//
// CAttrHwyNodeDlg �����o�֐���`
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

	CAttrHwyNodeDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };
	void Refresh();

private:
	void CreateControlRelation();
	void ChangeWindowMgrMap(int cWindowMapIndex);
	void SetRoadSequence();
	void ChangeRoadSeqCombobox();
	bool SetLineClassList(LONG nLineClass);
	bool GetLineClass(int* pClass);
	bool SetLineClass(int nClass);
	bool RedrawRoadNameList(LONG nRoadCode);
	void SetRoadCode(int nRoadCode);
	void CheckItems();
	void SetInOutNodeComboBox();
	void CAttrHwyNodeDlg::DisableAllControls();
	IGeometryPtr GetConnectedRoad( LONG lNodeID, BOOL bIsIN, CFeatureDef* ipFeatureDef );
	void DrawInOutNode();
	void ChangeDependentControlsOnNoEtcCarFlag();
	void ChangeDependentControlsOnEtcFacilCode();
	void ChangeHwyModeCheckBox(long cRoadCode);
	bool EtcSpecificTollFieldsExist();
	bool TollGateLaneFieldExist();
	void SetAvailabilityOfEtcSpecificTollControls(void);
	void DrawLaneImage(void);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHwyNodeDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,
						m_wnd5,m_wnd7,m_wnd8,m_wnd9,
						m_wnd10,m_wnd11,m_wnd12,m_wnd13,
						m_wnd16,m_wnd17,m_wnd18,m_wnd19,m_wnd20,
						m_wnd21,m_wnd22;	//!< �e�R���g���[���E�B���h�E
	CContainedWindow	m_wndTab;

	CString		m_cRoadSeq;					//!< ������ł̘H�����V�[�P���X
	// �_�C�A���O�̒l�ۑ��p�f�[�^
	LONG	m_nLineClass;					//!< ���ݑI�𒆂̘H����ʃR�[�h
	LONG	m_nRoadCode;					//!< ���ݑI�𒆂̘H���R�[�h
	LONG	m_nRoadSequence;				//!< ���ݑI�𒆂̘H�����V�[�P���X
	// ���̒l
	LONG m_lOrgRoadSequence;				//!< �ŏ��̘H�����V�[�P���X
	LONG m_lOrgRoadCode;					//!< �ŏ��̘H���R�[�h

	LONG	m_lFeatureIndex;				//!< �q�_�C�A���O���ł̃t�B�[�`���C���f�b�N�X�ێ��p
	LONG	m_lTableIndex;					//!< �q�_�C�A���O���ł̃e�[�u���C���f�b�N�X�ێ��p
	LONG	m_lRowIndex;					//!< �q�_�C�A���O���ł�Row�C���f�b�N�X�ێ��p
	BOOL	m_bForce;						//!< �q�_�C�A���O���ł̋����ҏW���[�h�ێ��p
	BOOL	m_bEditable;					//!< �q�_�C�A���O���ł̕ҏW�ł��邩�ǂ����ێ��p

	HWND	m_hChildWnd;					//!< �q�_�C�A���O�n���h��

	IFeaturePtr	m_ipNodeUnderHwyNode;		//!< �n�C�E�F�C�m�[�h�̉��̃m�[�h
	IGeometryPtr m_ipFromRoadGeom;			//!< �������H�W�I���g��
	IGeometryPtr m_ipToRoadGeom;			//!< ���o���H�W�I���g��
	IGeometryPtr m_ipDummyFrom;				//!< �_�~�[�̗������H�W�I���g��
	IGeometryPtr m_ipDummyTo;				//!< �_�~�[�̗��o���H�W�I���g��

	ITablePtr	m_ipFacilInfoTable;			//!< �{�ݏ��e�[�u��
	ITablePtr	m_ipRoadCodeListTable;		//!< �H���R�[�h���X�g�e�[�u��
	ITablePtr	m_ipRoadNodeTable;			//!< ���H�m�[�h�e�[�u��

	long	m_CurrentTabIndex;				//!< �^�u�̕\���ʒu�ۑ��p
	int		m_ChangeType;					//!< �R���g���[���؂�ւ��p

	// ���������[�����̃C���[�W�p�r�b�g�}�b�v
	CBitmap m_ImpassableImg;
	CBitmap m_PublicImg;
	CBitmap m_EtcImg;
	CBitmap m_MixedImg;
	CBitmap m_SwitchImg;
	CBitmap m_UnknownImg;
	CBitmap m_InvalidImg;

	CFont	m_TollGateLaneImgFont;						//!< ���������[�����̃C���[�W�̃��[���ԍ��Ɏg�p����t�H���g
	static const long m_TollGateLaneStrMaxLength;		//!< ���������[�����G�f�B�b�g�R���g���[���̍ő包��
	static const long m_TollGateLaneImageColumnMaxNum;	//!< ���������[�����C���[�W�̂P�s�ɕ��ׂ�摜�̐�

	// �n�C�E�F�C�ҏW�p�e��f�[�^�̃R���e�i
	RoadCodeListArray	m_RoadCodeListArray;	//!< Road_Code_List����擾�������̃R���e�i

	std::auto_ptr<bool>	m_pEtcSpecificTollFieldsExist;	//!< ETC�����ʗ����Ή��̂��߂̃f�[�^���f���ύX[Bug6198]�Œǉ����ꂽ�t�B�[���h�����݂��邩�iNULL�Ȃ疢�`�F�b�N�j
	std::auto_ptr<bool>	m_pTollGateLaneFieldExist;		//!< ���������[���Ή��̂��߂̃f�[�^���f���ύX[Bug6557]�Œǉ����ꂽ�u���������[�����v�t�B�[���h�����݂��邩�iNULL�Ȃ疢�`�F�b�N�j
};

namespace hwynode_dialog_tab	//!< �^�u�̎��
{
	enum EHwyNodeDialogTab
	{
		base_attr,			//!< �u��{�����v�^�u
		tollgate_lane,		//!< �u���������[�����v�^�u
		etc_tollgate,       //!< �uETC���������v�^�u

		tab_end             //!<  �^�u�I�[����p
	};
}

namespace tollgate_lane_type	//!< �u���������[�����v�t�B�[���h�̐����ƈӖ��i���������[���̎�ށj�Ƃ̑Ή�
{
	enum ETollGateLaneType
	{
		impassable		= 0,	//!< �g�p�s��
		public_lane		= 1,	//!< ��ʃ��[��
		etc_lane		= 2,	//!< ETC���[��
		mixed_lane		= 3,	//!< ���݃��[��
		switch_lane		= 4,	//!< �؂�ւ����[��
		unknown_lane	= 5,	//!< �s�����[��
		invalid,				//!< ��L�ȊO�̕s���Ȓl�i�R�[�h�l�����������́Ainvalid�̑O�ɒǉ����邱�Ɓj
		tollgate_lane_type_num
	};
}

#endif //__ATTRHWYNODEDLG_H_
