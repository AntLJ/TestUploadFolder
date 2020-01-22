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

#pragma once

#include "RelNodeInfos.h"
#include "RelNodeGridController.h"

// �ĕ`��̈˗����󂯂邽�߂̃��b�Z�[�W
const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE")); 


typedef CAttrBaseDlg ATTR_BASE_CLASS;

// CAttrRoadSignPointDlg
class CAttrRoadSignPointDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg2,
	public IDispEventImpl<IDC_VSFLEXGRID_NODE, CAttrRoadSignPointDlg>
{
public:
	/**
	* @brief �ҏW�r���[�̃��[�h
	* @detail CRowsControlRelation::SetControl() �̑�R�A��S�����Ƃ��Ďg�p���܂��B
	* @see CreateControlRelation()
	*/
	enum EditView {
		EDITVIEW_ROADSIGN_KIHON,		//!< ���H�W���|�C���g�̊�{�����i��1�^�u�j
		EDITVIEW_ROADSIGN_CLASS,		//!< ���H�W���|�C���g�̓��H�W����ʑ����i��2�^�u�j
	};

public:
	CAttrRoadSignPointDlg();
	~CAttrRoadSignPointDlg() {};

// ���b�Z�[�W�}�b�v
public:
	BEGIN_MSG_MAP(CAttrRoadSignPointDlg)
		MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
		MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
		COMMAND_HANDLER(IDC_BUTTON_SEARCH, BN_CLICKED, OnClickedSearchButton)

		COMMAND_ID_HANDLER(ID_MENUITEM_ADD, OnClickAddMenu)
		COMMAND_ID_HANDLER(ID_MENUITEM_DELETE, OnClickDeleteMenu)

		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		//MESSAGE_HANDLER(WM_NOTIFY, OnNotify)

		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
		CHAIN_MSG_MAP(ATTR_BASE_CLASS)

	ALT_MSG_MAP(IDC_EDIT_ROADSIGN_C)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_BUTTON_SEARCH)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	// ��{�^�u��
	ALT_MSG_MAP(IDC_EDIT_HORIZONTAL_ANGLE)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_CHECK_VARIABLE_F)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

	ALT_MSG_MAP(IDC_EDIT_SOURCE)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	END_MSG_MAP()

	BEGIN_SINK_MAP(CAttrRoadSignPointDlg)
		SINK_ENTRY(IDC_VSFLEXGRID_NODE, DISPID_MOUSEUP, OnMouseUpGrid)
		SINK_ENTRY(IDC_VSFLEXGRID_NODE, grid_util::disp_id::BeforeEdit, OnBeforeEdit)
		SINK_ENTRY(IDC_VSFLEXGRID_NODE, grid_util::disp_id::AfterEdit, OnAfterEdit)
		SINK_ENTRY(IDC_VSFLEXGRID_NODE, 0x01, OnCellChangedGrid)
	END_SINK_MAP()


public:
	// ���b�Z�[�W�C�x���g�n���h��
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedSearchButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

	// ���j���[�́u�ǉ��v�u�폜�v�{�^���N���b�N
	LRESULT OnClickAddMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	// �ĕ`�施�߂��󂯎�����Ƃ��̏���
	LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual bool Update() override;
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hWnd; };
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return m_TabNum; };
	virtual void SetTabNum(LONG lTabNum){ m_TabNum = lTabNum;}
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){
		return ::SendMessage( m_hWnd, message, wParam, lParam );
	}; 

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID);
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }
	BOOL Changed();

private:
	void CreateControlRelation();

	/**
	*  @brief ���H�W���|�C���g���̕ύX��ԂƁA�{�^���� Enable/Disable �؂�ւ�
	*/
	void UpdateChanged();

	/**
	*  @brief ���H�W���|�C���g�ƕR�Â��m�[�h�Ƃ����ԃ��C���̕`��
	*/
	void DrawLinkageLine();

	/**
	*  @brief �|�����C���̕`��
	*  @param[in] polyline  �`�悷��|�����C��
	*  @param[in] color     �`��F
	*/
	void DrawPolyLine(const IPolylinePtr& polyline, const DWORD& color);

	/**
	* @brief EditControl �ɓ��͂��ꂽ���H�W���R�[�h���������AComboBox �փZ�b�g����
	* @param[in]  editControlId   ���H�W���R�[�h����͂���EditControl ��ID
	* @param[in]  comboBoxId      ���H�W���R�[�h��ݒ肷��ComboBox ��ID
	* @retval     true            �Z�b�g����
	* @retval     false           �Z�b�g���s
	*/
	bool SetRoadsignCodeToComboBox(INT editControlId, INT comboBoxId);

	/**
	 * @brief	�E�C���h�E�}�b�v�ύX
	 * @param	nIndex	[in]	�^�u�C���f�b�N�X
	 */
	//void ChangeWindowMgrMap( INT nIndex );

	///**
	// * @brief	<��{>�^�u�ɐ؂�ւ����Ƃ��̃R���g���[������
	// */
	//void ChangeCtrlEnableToKihonTab();

	///**
	// * @brief	<�K��>�^�u�ɐ؂�ւ����Ƃ��̃R���g���[������
	// */
	//void ChangeCtrlEnableToKiseiTab();

	// VS-FlexGrid�C�x���g�n���h��
	void __stdcall OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel);
	void __stdcall OnAfterEdit(LONG Row, LONG Col);
	void __stdcall OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y);
	void __stdcall OnCellChangedGrid(/*LONG Row, LONG Col*/) {};

private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< �e�R���g���[���E�B���h�E

	LONG		m_TabNum;					//!< �^�u�R���g���[���ʒu
	INT			m_ChangeType;				//!< �R���g���[���؂�ւ��p

	LONG		m_FeatureIndex;				//!< �t�B�[�`���C���f�b�N�X�ێ��p
	LONG		m_TableIndex;				//!< �e�[�u���C���f�b�N�X�ێ��p
	LONG		m_RowIndex;					//!< Row�C���f�b�N�X�ێ��p
	BOOL		m_IsForce;					//!< �����ҏW���[�h�ێ��p
	BOOL		m_Editable;					//!< �ҏW�ł��邩�ǂ����ێ��p

	long m_CurrentOid;			//!< ���݃_�C�A���O�ɕ\�����Ă��铹�H�W���|�C���g��OID
	std::unordered_map<long, RelNodeInfos> m_RelNodes;	//!< �֘A�e�[�u����� 

	RelNodeGridController m_Grid;	//!< �֘A�m�[�h�O���b�h�̐���
};
