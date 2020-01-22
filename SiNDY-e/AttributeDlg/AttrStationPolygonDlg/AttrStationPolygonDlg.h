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

#include "StationInfoController.h"

// STL
#include <memory>

typedef CAttrBaseDlg ATTR_BASE_CLASS;


/**
 * @brief �w�|���S���p�����_�C�A���O
 * @note  �ҏW�Ώۂ̃t�B�[�`���N���X�͈ȉ��̒ʂ�
 * @note  STATION_DETAIL, MIDZOOM_PLATFORM, HIGHZOOM_STATION, MIDZOOM_STATION, LOWZOOM_PLATFORM
 */
class CAttrStationPolygonDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg2,
	public IDispEventImpl<IDC_GRID_STATION_INFO, CAttrStationPolygonDlg>,
	public IDispEventImpl<IDC_GRID_SEARCH_STATION, CAttrStationPolygonDlg>
{

public:

	/**
	 * @brief�w�|���S��5���C��������1���R�[�h����肷�邽�߂̃L�[
	 */
	struct StaObjKey
	{
		long m_LayerNo;		//!< ���C���ԍ�
		long m_LayerOid;	//!< ���C����OID

	private:
		StaObjKey() : m_LayerNo(0), m_LayerOid(0) {}
	public:
		StaObjKey(long lLayerNo, long lLayerOid): m_LayerNo(lLayerNo), m_LayerOid(lLayerOid) {}
		~StaObjKey(){}

		bool operator<(const StaObjKey& key) const {
			return m_LayerNo == key.m_LayerNo ? m_LayerOid < key.m_LayerOid : m_LayerNo < key.m_LayerNo;
		}
	};

public:

	/// ���b�Z�[�W�}�b�v
	BEGIN_MSG_MAP(CAttrStationPolygonDlg)
		MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
		MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
		COMMAND_HANDLER(IDC_BUTTON_APPLY, BN_CLICKED, OnClickedApplyButton)
		COMMAND_HANDLER(IDC_BUTTON_SEARCH_STATION, BN_CLICKED, OnClickStationSearch)
		COMMAND_ID_HANDLER(ID_MENUITEM_STATION_INFO_DELETE, OnClickDeleteMenu)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		CHAIN_MSG_MAP(ATTR_BASE_CLASS)

	ALT_MSG_MAP(IDC_EDIT_SOURCE)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	END_MSG_MAP()

	/// �C�x���g�n���h��	
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedApplyButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickStationSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	/// �V���N�}�b�v
	BEGIN_SINK_MAP(CAttrStationPolygonDlg)
		SINK_ENTRY(IDC_GRID_STATION_INFO, grid_util::disp_id::BeforeEdit, OnStationInfoBeforeEdit)
		SINK_ENTRY(IDC_GRID_STATION_INFO, DISPID_MOUSEUP, OnStationInfoMouseUp)
		SINK_ENTRY(IDC_GRID_SEARCH_STATION, grid_util::disp_id::BeforeEdit, OnStationSearchBeforeEdit)
	END_SINK_MAP()

	/// �O���b�h�̃C�x���g�n���h��
	void __stdcall OnStationInfoMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y);
	void __stdcall OnStationInfoBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel);
	void __stdcall OnStationSearchBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel);


public:

	CAttrStationPolygonDlg();
	virtual ~CAttrStationPolygonDlg(){};

	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){ m_strClassName = (CString)lpcszFeatureClassName; };
	virtual BOOL ErrorCheck() override { return TRUE; };
	virtual bool Update() override;

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL Changed();
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }


public:

	typedef std::map<StaObjKey, CStationInfoController>::iterator Iter;
	typedef std::shared_ptr<CStationInfoController> SPStationInfo;

	// "�w�|���S��1���R�[�h"��"�֘A�e�[�u������I�u�W�F�N�g"�̃}�b�v
	typedef std::pair<StaObjKey, SPStationInfo> StationInfoRecord;
	typedef std::map<StaObjKey, SPStationInfo> StationInfoMap;

private:

	void CreateControlRelation();

	/**
	 *	@brief  �{�^����Enable/Disable�̍X�V�A�֘A�e�[�u���̍X�V���̍X�V
	 *  @note   �O���b�h�ŊǗ����Ă���֘A�e�[�u���̏��́AControlRel�ōX�V�ł��Ȃ����߁A\n
	 *          �ύX���̃t���O�����X�V����B�������Ȃ���DB���X�V����Ȃ�
	 */
	void updateChanged();

	/**
	 *	@brief  ���ݕ\�����̃t�B�[�`���̉w��񃌃R�[�h��Ԃ�
	 */
	SPStationInfo getCurrentStationInfo();

	/**
	 *	@brief  �w��̃��C���ԍ�&���C����OID�̃��R�[�h�����ɂ����Ă��邩�ۂ�
	 *  @param [in] lLayerNo  ���C���ԍ�
	 *  @param [in] lLayerOid ���C����OID
	 *  @retval true  ���R�[�h�������Ă���
	 *  @retval false ���R�[�h�������Ă��Ȃ�
	 */
	bool hasStationInfo( long lLayerNo, long lLayerOid) const;


private:

	std::map<long, CContainedWindow> m_mapSubClass; //!< �e�R���g���[���E�B���h�E

	StationInfoMap m_StationInfoList;			//!< �֘A�e�[�u���̐���I�u�W�F�N�g�Q
	long    m_CurrentLayerNo;					//!< ���ݕ\�����̃t�B�[�`���̃��C���ԍ�
	long	m_CurrentOid;						//!< ���ݕ\�����̃t�B�[�`����OID

	bool	m_bEditable;						//!< �ҏW�J�n����Ă��邩�ۂ�

};
