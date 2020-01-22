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

// AttrSegmentAttrDlg.h : CAttrSegmentAttrDlg の宣言

#ifndef _ATTRSEGMENTATTRDLG_
#define _ATTRSEGMENTATTRDLG_

#include "resource.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrSegmentAttrDlg
class CAttrSegmentAttrDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSegmentAttrDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrSegmentAttrDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_SEGMENT_ATTR_CHECK1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_SEGMENT_ATTR_EDIT2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_SEGMENT_ATTR_EDIT3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_SEGMENT_ATTR_COMBO1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_SEGMENT_ATTR_EDIT4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_SEGMENT_ATTR_EDIT5)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSegmentAttrDlg メッセージハンドラ
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
		for( auto& subclass : m_mapSubClass)
		{
			subclass.second.SubclassWindow( GetDlgItem( subclass.first ) );
		}

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
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

		// コンボボックスの中身変更
		// TODO:各コントロールでイベント捕まえるようにする。
		if( nTargetControl == IDC_SEGMENT_ATTR_COMBO1 )
		{
			if( msg == CBN_SETFOCUS )
			{
				SetLinkAttrComboBox();
			}
			else if( msg == CBN_SELCHANGE )
			{
				SetNameFieldEditable();
			}
		}

		// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			// 2次メッシュ境界線を跨いでいる道路の場合は反対側の道路も同じ属性にしなくてはいけないため
			// 以下の処理を行う
			if( m_bIsRoadOnMesh )
			{
				// とりあえず、現在保持しているリストすべてにおいてループを行う
				for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
				{
					// 現在選択中の2次メッシュを跨いでいる道路リンクに関連しているリストとの照合
					CFeatureDef *pFeatureDef = NULL;
					for( std::list<CFeatureDef>::iterator iAttr = m_pAttrLinkOnMeshList.begin(); iAttr != m_pAttrLinkOnMeshList.end(); iAttr++ )
					{
						if( iAttr->GetFeature() == it->GetFeature() )
						{
							pFeatureDef = &(*it);
							break;
						}
					}

					if( pFeatureDef )
					{
						// すべてのフィールド値をチェック
						for( std::list<CFieldDef>::iterator itField = GetCurrentFeatureDef()->begin(); itField != GetCurrentFeatureDef()->end(); itField++ )
						{
							CString strFieldName = itField->m_strFieldName;

							// 以下内容をコピーする
							if( 0 == strFieldName.CompareNoCase( sindy::schema::segment_attr::kLinkAttr ) ||
								0 == strFieldName.CompareNoCase( sindy::schema::segment_attr::kUnderOver ) ||
								0 == strFieldName.CompareNoCase( sindy::schema::segment_attr::kName ) ||
								0 == strFieldName.CompareNoCase( sindy::schema::segment_attr::kSource ) )
							{
								// 上記のif文の条件以外はコピーする
								CFieldDef *pFieldDef;
								pFieldDef = pFeatureDef->GetFieldDef( SEGMENTATTR_TABLE_NAME, itField->m_strFieldName );
								VARIANT vaValue;
								pFeatureDef->GetFeature()->get_Value( itField->m_lFieldIndex, &vaValue );
								if( itField->m_vaValue != vaValue )
								{
									pFieldDef->m_vaValue = itField->m_vaValue;
									pFieldDef->m_bChanged = TRUE;
								}
								else{
									pFieldDef->m_vaValue = itField->m_vaValue;
									pFieldDef->m_bChanged = FALSE;
								}
							}
						}
					}
				}
			}

			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSegmentAttrDlg メンバ関数定義
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
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrSegmentAttrDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID);
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
	void SetLinkAttrComboBox();

	/**
	 * @brief 名称エディットボックスの有効無効切り替え
	 * @note  種別コンボボックスの値によって切り替える
	 */
	void SetNameFieldEditable();

	/**
	 * @brief 種別コンボボックスでNAMEフィールドの編集が可能な種別が選択されているかどうか
	 * @retval true  NAMEフィールド編集可の種別が選択されている
	 * @retval false NAMEフィールド編集不可の種別が選択されている
	 */
	bool IsNameFieldEditableAttr();

	/**
	 * @brief 名称フィールドがなければコントロールを非表示にする(ASEAN等の対応)
	 */
	void AdjustLayout();

////////////////////////////////////////////////////////////////////////////
//
// CAttrSegmentAttrDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< 各コントロールウィンドウ
	std::list<CFeatureDef>	m_pAttrLinkOnMeshList;		//!< メッシュを跨いでいるリンク内属性のフィーチャデフ
	BOOL				m_bIsRoadOnMesh;			//!< メッシュを跨いでいるかどうか
	HWND				m_hChildWnd;				//!< 子ダイアログハンドル
};

#endif // _ATTRSEGMENTATTRDLG_

