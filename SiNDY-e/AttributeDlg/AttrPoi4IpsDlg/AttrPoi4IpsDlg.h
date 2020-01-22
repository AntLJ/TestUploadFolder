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

#include "TDC/sindylib_core/FieldMap.h"
#include "libschema.h"
#include "AccTable.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrPoi4IpsDlg
class CAttrPoi4IpsDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrPoi4IpsDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrPoi4IpsDlg)
	COMMAND_HANDLER(IDC_BUTTON_PP_FOUND, BN_CLICKED, onPressPpFound)
	COMMAND_HANDLER(IDC_BUTTON_DELETE,BN_CLICKED, onPressDelete)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEditEx)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEditEx)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEditEx)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

// メッセージマップ
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_CONTENTS_CODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_CONTENTS_NAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_NAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PP_FOUND)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	COMMAND_CODE_HANDLER(BN_CLICKED, onPressPpFound)
ALT_MSG_MAP(IDC_COMBO_PST_IP_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_AAC_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DELETE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	COMMAND_CODE_HANDLER(BN_CLICKED, onPressDelete)
ALT_MSG_MAP(IDC_EDIT_CLOSE_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_DELETE_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ANNO_DISP_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrPoi4IpsDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// コントロール
		CreateControlRelation();
		// サブクラス化
		for( auto& it : m_mapSubClass)
		{
			it.second.SubclassWindow( GetDlgItem( it.first ) );
		}

		// コンテンツマスタ読み込み
		readContentsMaster();
		// 精度マスタ取得
		m_AccCodeMaster = AccTable::codes;
		
		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}

	/**
	 * コモンコントロールの色変更用イベントハンドラ改良版
	 *
	 * フィールド値の変更が合った場合、ここでコモンコントロールの色が変わります
	 */
	LRESULT OnCtlColorEditEx(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{

		HDC hdc = (HDC) wParam;    // handle to display context 
		HWND hWnd = (HWND) lParam; // handle to static control 
		INT	nTargetControl = (INT)LOWORD(wParam);		// 操作対象コントロール

		// 親クラスのOnCtlColorEditを呼び出し
		LRESULT ret = OnCtlColorEdit(uMsg, wParam, lParam, bHandled);

		using namespace sindy::schema;

		// 情報提供元コード、精度のコンボボックス、閉鎖理由、削除コード、注記表示コードのエディットボックスは自前で変更管理
		CFieldDef* pFieldDef = nullptr;
		switch (::GetDlgCtrlID((HWND)lParam))
		{
			case IDC_COMBO_PST_IP_C:
				pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kPSTIPCode );
				break;
			case IDC_COMBO_AAC_C:
				pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kACCCode );
				break;
			case IDC_EDIT_CLOSE_C:
				pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kCloseCode );
				break;
			case IDC_EDIT_DELETE_C:
				pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kDelete );
				break;
			case IDC_EDIT_ANNO_DISP_C:
				pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kAnnoDispCode );
				break;
			default:
				break;
		}

		if ( pFieldDef == nullptr ||
			 ( pFieldDef != nullptr && !pFieldDef->m_bChanged )  )
			return ret;

		if( !m_hChangedBrush )
			m_hChangedBrush = CreateSolidBrush( RGB(255,168,126) );

		SetBkMode(hdc, TRANSPARENT );
		SetBkColor(hdc, RGB(255,168,126));

		// Return a handle to the brush
		HBRUSH far* hbr = (HBRUSH far*) m_hChangedBrush;
		
		return ((DWORD) hbr);
	}

	/**
	 * WM_COMMAND 用イベントハンドラ
	 *
	 * 各コモンコントロールで変更があった場合、ここで処理します
	 * 現在はエディットボックス、チェックボックス、コンボボックスの処理が行われます
	 */
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// SetCurrentFeatureDefIndex() の最中はキャンセル（タイミングによってはおかしくなるため）
		if( !m_bFinishInit ) return 0;

		INT msg = HIWORD(wParam);						// 操作メッセージ
		INT	nTargetControl = (INT)LOWORD(wParam);		// 操作対象コントロール

		// エディットボックスの時に全選択にする
		if( msg == EN_SETFOCUS )
			SelectDlgItem( nTargetControl );
		// コンボボックスのリストボックスの長さを調節
		if( msg == CBN_DROPDOWN )
			SetComboboxList( nTargetControl );

		// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) {

			// コンボボックスの値がセルチェンジしたときに値を変更する。
			if( ( nTargetControl == IDC_COMBO_PST_IP_C ) && ( msg == CBN_SELCHANGE) )
				setPstIPCode(uMsg, wParam, lParam, bHandled);
			else if( ( nTargetControl == IDC_COMBO_AAC_C ) && ( msg == CBN_SELCHANGE) )
				setAccCode(uMsg, wParam, lParam, bHandled);

			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			// ダイアログを更新領域に追加します
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}
		return 0;
	}

private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrPoi4IpsDlg メンバ関数定義
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
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrPoi4IpsDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

private:
	void CreateControlRelation();

	/**
	 * @brief 正位置判明ボタンが押下された場合の処理
	 */
	LRESULT onPressPpFound(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/**
	 * @brief  削除ボタンが押下された場合の処理
	 */
	LRESULT onPressDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/**
	 * @brief 情報提供元コードで値が選択された場合の処理
	 */
	LRESULT  setPstIPCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief 精度で値が選択された場合の処理
	 */
	LRESULT  setAccCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief  レジストリ値を取得
	 * @param  path  [in]  レジストリのパス
	 * @param  key  [in]  レジストリキー
	 * @param  out  [out]  値
	 * @return true:成功
	 */
	bool getRegKey(const CString& path , const CString& key, CString& out);

	/**
	 * @brief  選択されているフィーチャの情報を取得しメンバ変数にセット
	 * @return　なし
	 */
	void getCurrentFeatureInfo();
	
	/**
	 * @brief  コンボボックス作成(汎用)
	 * @param  id  [in]  コントロールID
	 * @param  tbl  [in]  設定元マスターテーブル
	 * @param  lpszTableName  [in]  テーブル名
	 * @param  lpszFieldName  [in]  カラム名
	 * @return
	 */
	void createComboBox(UINT id, const StringMasterTbl& tbl, LPCTSTR lpszTableName, LPCTSTR lpszFieldName);

	/**
	 * @brief  情報提供元コードコンボボックス作成
	 * @return　なし
	 */
	void createPstIpCombo();

	/**
	 * @brief  精度コンボボックス作成
	 * @return　なし
	 */
	void createAcc();

	/**
	 * @brief  コンボボックスのアイテムをFieldDefにセット
	 * @param  tbl  [in]  ドメインのテーブル
	 * @param  id  [in]  ドメインのテーブルのインデックス
	 * @param  lpszFieldName  [in]  設定するフィールド名称
	 * @return なし
	 */
	void setFieldDef4Combo(const StringMasterTbl& tbl, UINT id,  const LPCTSTR lpszFieldName);
	
	/**
	 * @brief  TSVファイルを2次元配列で返却する
	 * @param  file  [in]  ファイルパス
	 * @return 2次元配列
	 */
	std::vector<std::vector<CString>> parseTsvFile(const CString& file);
	
	/**
	 * @brief  ドメインからシーケンスを取得する
	 * @param  ipCode  [in]  IPコード
	 * @param  tbl  [in]  ドメインのテーブル
	 * @return シーケンス
	 */
	INT getComboIdx(const CString& ipCode, const StringMasterTbl& tbl);

	/**
	 * @brief  更新情報を付与する
	 * @param  featureDef  [in]  更新情報を付与するCFeatureDef
	 * @return true:成功
	 */
	bool UpdateHistory(CFeatureDef* featureDef);

	/**
	 * @brief  削除ボタンフラグを現在の状況から判断する
	 * @return 削除フラグに設定する値
	 */
	bool judgeDeleteFlg();

	/**
	 * @brief  削除ボタンフラグを現在の状況から判断する
	 * @param  pFieldDef  [in]  更新フラグを変更するCFieldDef
	 * @return なし
	 */
	void updateChange(CFieldDef* pFieldDef);

	/**
	 * @brief  キーが文字列のドメインを作成する
	 * @return なし
	 */
	void createStrKeyDomain();

	/**
	* @brief  キーが文字列のドメイン値を取得する
	 * @return　ドメインのレコード
	 */
	std::map< CString, CString> getStrKeyDomein(const LPCTSTR lpszFieldName);

	/**
	 * @brief  ドメインの値をエディットボックスに入れる
	 * @param  id  [in]  コントロールID
	 * @param  lpszFieldName  [in]  フィールド名称
	 * @return
	 */
	void setEditBoxByDomainVal(int id, LPCTSTR lpszFieldName);

	/**
	 * @brief  コンテンツマスタ読み込み
	 */
	bool readContentsMaster();

////////////////////////////////////////////////////////////////////////////
//
// CAttrPoi4IpsDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	/// 各コントロールウィンドウ
	std::map<int, CContainedWindow> m_mapSubClass; 
	/// ボタンが削除か
	bool m_isDelete;
	/// GRPCODE
	CString m_grpcode;
	/// CHAINCODE
	CString m_chainCode;
	/// ContentsSeq
	CString m_contentsSeq;
	/// コンテンツ名称
	CString m_contentsName;
	/// コンテンツタイプ
	LONG m_contentsType;
	///  新規POIか 
	bool m_isNewlyPoi;
	/// フィールドマップ
	sindy::CFieldMap m_fieldMap;
	/// 情報提供元コードマスタ値
	StringMasterTbl m_pstIpCodeMaster;
	/// 精度マスタ値
	StringMasterTbl m_AccCodeMaster;
	/// キーが文字列型であるドメインのマップ　std::map< フィールド名 , map<キー, 値> >
	std::map< CString, std::map<CString, CString> > m_strKeyDomainMap;
	/// コンテンツマスタ
	std::vector<std::vector<CString>> m_contentsMaster;
	/// 情報提供元コードマスタ読み込み
	std::vector<std::vector<CString>> m_infoProvideMaster;
};
