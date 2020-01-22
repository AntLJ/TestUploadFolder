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


#ifndef __ATTRBUILDINGNAMEPOINTDLG_H_
#define __ATTRBUILDINGNAMEPOINTDLG_H_

#include "resource.h"
#include "WinLib/Str2.h"

// リストビューカラム名称、サイズ
// TODO:英語化対応
const LPTSTR BNP_SRC_COLUMN0 = _T("素材OID");
const int BNP_SRC_COLUMN0_SZ = 64;
const LPTSTR BNP_SRC_COLUMN1 = _T("名称");
const int BNP_SRC_COLUMN1_SZ = 200;
const LPTSTR BNP_SRC_COLUMN2 = _T("出典コード");
const int BNP_SRC_COLUMN2_SZ = 112;

namespace list_col
{
	namespace building_src
	{
		enum Ecode
		{
			SrcID,     // 素材OID
			Name,      // 名称
			SrcCode,   // 出典コード

			ColumnNum, // 行数
		};
	}
}

// リストビューカラム数
#define BNP_SRC_COLUMN_COUNT 3

typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrBuildingNamePointDlg
class CAttrBuildingNamePointDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CAttrBuildingNamePointDlg();
	/**
	 * @brief デストラクタ
	 */
	virtual ~CAttrBuildingNamePointDlg()
	{
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingNamePointDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrBuildingNamePointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_EDIT_NAME, EN_UPDATE, OnEnUpdateEditString)
	COMMAND_HANDLER(IDC_BUTTON_SEARCHOID, BN_CLICKED, OnBnClickedButtonSearchoid)
	COMMAND_HANDLER(IDC_EDIT_SURVEYDATE, EN_UPDATE, OnEnUpdateEditString)
	COMMAND_HANDLER(IDC_BUTTON_PRIORITYC, BN_CLICKED, OnBnClickedButtonPriorityc)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_EDIT_BUILDINGOID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCHOID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_NAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_NAMESEPARATED)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_YOMISEPARATED)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_BLDGCLASS1C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_BLDGCLASS2C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_BLDGCLASS3C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_FLOORS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_SOURCETYPEC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SRCOID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SURVEYDATE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ANYPLACEID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_PRIORITYC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PRIORITYC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_MATCHPER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_LIST_SRCPOINT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()


private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingNamePointDlg メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	/**
	 * @brief ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief WM_COMMAND 用イベントハンドラ
	 * @note  各コモンコントロールで変更があった場合、ここで処理します
	 * @note  現在はエディットボックス、チェックボックス、コンボボックスの処理が行われます
	 */
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief string型用イベントハンドラ
	 * @note  IDC_EDIT_NAME：入力時の形式に関わらず、全角に変換します。
	 * @note  IDC_EDIT_SURVEYDATE：入力時の形式に関わらず半角に変換し、不正な文字は入力できないようにします。
	 */
	LRESULT OnEnUpdateEditString(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/** 
	 * @brief 子ダイアログを作成します。
	 * @note  ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
	 * @param hWndParent	[in]	親ウィンドウハンドル
	 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
	 * @retval 作成された子ダイアログのハンドルを返します。
	 */
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);

	/**
	 * @brief 作成した子ダイアログを削除します。
	 * @note ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
	 */
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
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){m_strClassName = lpcszFeatureClassName;};

	/**
	* @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
	* さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
	* ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
	*
	* @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
	* @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
	* @param lRowIndex		[in]	レコード定義クラスのインデックス番号
	* @param bForce		[in]	強制的に編集するかどうか
	* @param bEditable		[in]	編集可能かどうか
	*
	* @retval 意味がない（TRUEしか返さない）
	*/
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

	LRESULT OnBnClickedButtonSearchoid(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedButtonPriorityc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


private:
	/**
	 * @brief 子ダイアログを実際に作成します。
	 * @note 実際の作成はControlRelに任せます。
	 */
	void CreateControlRelation();

	/**
	 * @brief ツールチップを作成します。
	 * @note 作成するツールチップは属性に変更がかからないため、作りっぱなしで問題なし
	 * @param nTargetItem	[in]	ツールチップを作成するコントロールアイテムのID
	*/
	void SetToolTip(INT nTargetItem);

	/**
	 * @brief 建物ビル名称ポイントに紐づく建物ビル名称素材ポイントの情報をリスト出力する
	 */
	void OutputBuildingSrcInfo();

	/**
	* @brief  全角パイプを除いたNAME_SEPARATEDがNAMEと同一であるか確認します。
	* @param  vaNameSep	[in]	NAME_SEPARATEDの値
	* @param  vaName	[in]	NAMEの値
	* @retval true		チェックOK
	* @retval false		チェックNG
	*/
	bool checkNameSeparated(const CComVariant& vaNameSep, const CComVariant& vaName);

	/**
	* @brief  YOMI_SEPARATEDが全角カタカナおよび全角パイプのみであるか確認します。
	* @param  vaVal	[in]	YOMI_SEPARATEDの値
	* @retval true	チェックOK
	* @retval false	チェックNG
	*/
	bool checkYomiSeparated(const CComVariant& vaVal);


/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingNamePointDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass;  //!< 各コントロールウィンドウ
	bool                            m_bShowSrcInfo; //!< 建物ビル名称素材ポイントの情報を出力したか

	static LVCOLUMN m_ColumnInfo[];  //!< 素材ポイント関連情報のリスト定義

};

#endif // __ATTRBUILDINGNAMEPOINTDLG_H_
