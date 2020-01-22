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
 * @brief 駅ポリゴン用属性ダイアログ
 * @note  編集対象のフィーチャクラスは以下の通り
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
	 * @brief駅ポリゴン5レイヤ内から1レコードを特定するためのキー
	 */
	struct StaObjKey
	{
		long m_LayerNo;		//!< レイヤ番号
		long m_LayerOid;	//!< レイヤ内OID

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

	/// メッセージマップ
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

	/// イベントハンドラ	
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedApplyButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickStationSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	/// シンクマップ
	BEGIN_SINK_MAP(CAttrStationPolygonDlg)
		SINK_ENTRY(IDC_GRID_STATION_INFO, grid_util::disp_id::BeforeEdit, OnStationInfoBeforeEdit)
		SINK_ENTRY(IDC_GRID_STATION_INFO, DISPID_MOUSEUP, OnStationInfoMouseUp)
		SINK_ENTRY(IDC_GRID_SEARCH_STATION, grid_util::disp_id::BeforeEdit, OnStationSearchBeforeEdit)
	END_SINK_MAP()

	/// グリッドのイベントハンドラ
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

	// "駅ポリゴン1レコード"と"関連テーブル制御オブジェクト"のマップ
	typedef std::pair<StaObjKey, SPStationInfo> StationInfoRecord;
	typedef std::map<StaObjKey, SPStationInfo> StationInfoMap;

private:

	void CreateControlRelation();

	/**
	 *	@brief  ボタンのEnable/Disableの更新、関連テーブルの更新情報の更新
	 *  @note   グリッドで管理している関連テーブルの情報は、ControlRelで更新できないため、\n
	 *          変更情報のフラグだけ更新する。そうしないとDBが更新されない
	 */
	void updateChanged();

	/**
	 *	@brief  現在表示中のフィーチャの駅情報レコードを返す
	 */
	SPStationInfo getCurrentStationInfo();

	/**
	 *	@brief  指定のレイヤ番号&レイヤ内OIDのレコードを既にもっているか否か
	 *  @param [in] lLayerNo  レイヤ番号
	 *  @param [in] lLayerOid レイヤ内OID
	 *  @retval true  レコードを持っている
	 *  @retval false レコードを持っていない
	 */
	bool hasStationInfo( long lLayerNo, long lLayerOid) const;


private:

	std::map<long, CContainedWindow> m_mapSubClass; //!< 各コントロールウィンドウ

	StationInfoMap m_StationInfoList;			//!< 関連テーブルの制御オブジェクト群
	long    m_CurrentLayerNo;					//!< 現在表示中のフィーチャのレイヤ番号
	long	m_CurrentOid;						//!< 現在表示中のフィーチャのOID

	bool	m_bEditable;						//!< 編集開始されているか否か

};
