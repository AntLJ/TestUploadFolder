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

// LQAttrVICSDlg.cpp : CLQAttrVICSDlg のインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "LQAttrVICSDlg.h"

/// ウィンドウマネージャ配置マップ（VICS用）
BEGIN_WINDOW_MAP(VICS_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(0,0))
		RCSPACE(22)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_VICSMESHCODE)
			RCREST(IDC_EDIT_VICSMESHCODE)
		ENDGROUP()
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_VICSCLASS)
			RCREST(IDC_COMBO_VICSCLASS)
		ENDGROUP()
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_VICSID)
			RCREST(IDC_EDIT_VICSID)
		ENDGROUP()
		RCREST(-1)
		RCTOFIT(IDC_STATIC_SOURCESEP)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,10))
			RCTOFIT(IDC_STATIC_SOURCE2)
			RCREST(IDC_EDIT_SOURCE2)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CLQAttrVICSDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CLQAttrVICSDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrVICSDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CLQAttrInterNaviDlg::CLQAttrInterNaviDlg()
 * @brief コンストラクタ
 *
 * なにもしません
 */
CLQAttrVICSDlg::CLQAttrVICSDlg() 
	: LQATTR_COMMON_BASE_CLASS(VICS_MAP, IDD_LQATTRVICSDLG),
	m_wndVicsMeshCode( _T("EDIT"), this, IDC_EDIT_VICSMESHCODE ),
	m_wndVicsClass( _T("COMBOBOX"), this, IDC_COMBO_VICSCLASS ),
	m_wndVicsID( _T("EDIT"), this, IDC_EDIT_VICSID ),
	m_wndSource( _T("EDIT"), this, IDC_EDIT_SOURCE2 )
{
}

/** 
 * @fn HWND CLQAttrVICSDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * LQATTR_COMMON_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CLQAttrVICSDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	// ダイアログの作成はLQATTR_COMMON_BASE_CLASSに任せます
	m_hChildWnd = LQATTR_COMMON_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CLQAttrVICSDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CLQAttrVICSDlg::Delete()
{
	::DestroyWindow(m_hChildWnd);
    delete this;
}

/**
 * @fn BOOL CLQAttrVICSDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CLQAttrVICSDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = LQATTR_COMMON_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
		::SetFocus( GetDlgItem(IDC_EDIT_VICSMESHCODE) );
	else
		::SendMessage( GetDlgItem(IDC_EDIT_VICSMESHCODE), WM_KILLFOCUS, NULL, NULL );

	SetCurrentMeshCode();

    return bRet;
}

/**
 * @fn void CLQAttrVICSDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CLQAttrVICSDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// VICS用
		m_cControlRel.SetControl( IDC_EDIT_VICSMESHCODE,	TYPE_EDIT,			0, 0,	_T("VICS_MESHCODE"),	NULL, m_strFeatureClassName     );
		m_cControlRel.SetControl( IDC_COMBO_VICSCLASS,		TYPE_COMBO,			0, 0,	_T("VICSCLASS_C"),		NULL, m_strFeatureClassName     );
		m_cControlRel.SetControl( IDC_EDIT_VICSID,			TYPE_EDIT,			0, 0,	_T("VICS_LINK_ID"),		NULL, m_strFeatureClassName     );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE2,			TYPE_EDITSTRING,	0, 0,	_T("SOURCE"),			NULL, m_strFeatureClassName  );
	}
}
void CLQAttrVICSDlg::SetCurrentMeshCode()
{
	CEdit cEdit(GetDlgItem( IDC_EDIT_VICSMESHCODE ) );
    LPTSTR lpszMeshCode = (LPTSTR)_alloca((cEdit.GetWindowTextLength() + 1) * sizeof(TCHAR));        //!< 文字数分メモリ確保
    cEdit.GetWindowText( lpszMeshCode, cEdit.GetWindowTextLength() + 1 );

	CString strMeshCode;
	strMeshCode.Format( lpszMeshCode );
	// メッシュコードに0が入っていたら（つまり、初期化されていたら。新規作成の場合）
	if( strMeshCode.Compare( _T("0")) == 0 )
	{
		IPointPtr ipCurrentFirstPoint;
		IPolylinePtr ipCurrentDirGuideGeom = GetCurrentFeatureDef()->GetShape();
		if( ipCurrentDirGuideGeom )
		{
			ipCurrentDirGuideGeom->get_FromPoint( &ipCurrentFirstPoint );
			ISpatialReferencePtr ipSpRel;
			ipCurrentDirGuideGeom->get_SpatialReference( &ipSpRel );

			// 始点の道路リンクのメッシュコードを取得
			CMesh cMesh;			
			LONG lMeshCode;
			std::list<UINT> listMesh;
			BOOL bIsTouch = FALSE;
			bIsTouch = cMesh.IsTouches( ipCurrentFirstPoint, listMesh );

			// メッシュと接していたら、1個目と2個目の構成点の間の位置のメッシュコードをとる
			if( !bIsTouch )
				lMeshCode = cMesh.GetExistMeshCode(ipCurrentFirstPoint);
			else
			{
				IPointCollectionPtr ipPointCollection(CLSID_Polyline);
				ipPointCollection = ipCurrentDirGuideGeom;
				IPointPtr ipSecondPoint;
				ipPointCollection->get_Point( 1, &ipSecondPoint );
				double dFirstX,dFirstY,dSecondX,dSecondY;
				if( ipCurrentFirstPoint && ipSecondPoint )
				{
					ipCurrentFirstPoint->get_X( &dFirstX );
					ipCurrentFirstPoint->get_Y( &dFirstY );
					ipSecondPoint->get_X( &dSecondX );
					ipSecondPoint->get_Y( &dSecondY );
				}
				IPointPtr ipMiddlePoint( CLSID_Point );
				if( ipMiddlePoint != NULL )
				{
					ipMiddlePoint->put_X( ( dFirstX + dSecondX )/2 );
					ipMiddlePoint->put_Y( ( dFirstY + dSecondY )/2 );
				}
				// [Bug 6007]ッシュ境界でVICSを作成するとVICSメッシュコードが-1になるバグ対応
				ipMiddlePoint->putref_SpatialReference( ipSpRel );
				lMeshCode = cMesh.GetExistMeshCode(ipMiddlePoint);
			}
			CString strText;
			strText.Format(_T("%d"), lMeshCode);
			cEdit.SetWindowText( (LPCTSTR)strText );
		}
	}
}
LONG CLQAttrVICSDlg::GetLongValue( CRowDef* pRowDef, LPCTSTR lpcszFieldName )
{
	LONG lVal = -1;

	if( pRowDef )
	{
		const CFieldDef* pFieldDef = pRowDef->GetFieldDef( lpcszFieldName );
		if( pFieldDef )
		{
			if( pFieldDef->m_vaValue.vt == VT_I4 )
				lVal = pFieldDef->m_vaValue.lVal;
		}
	}
	return lVal;
}

BOOL CLQAttrVICSDlg::ErrorCheck()
{
	BOOL bRet = TRUE;
	CString strMsg;
	// 選択しているものの中で同じものを作っていないかの調査
	for( std::list<CLQRowDef>::iterator it1 = m_pFeatureDefList->begin(); it1 != m_pFeatureDefList->end(); it1++ )
	{
		CString strMsg;
		if( m_strFeatureClassName.CompareNoCase( it1->GetTableName() ) == 0 )
		{
			CFieldDef* pOIDFieldDef1 = it1->GetFieldDef( m_strFeatureClassName, _T("OBJECTID"));
			CFieldDef* pMeshCodeFieldDef1 = it1->GetFieldDef( m_strFeatureClassName, _T("VICS_MESHCODE") );
			CFieldDef* pVICSClassFieldDef1 = it1->GetFieldDef( m_strFeatureClassName, _T("VICSCLASS_C") );
			CFieldDef* pVICSLinkFieldDef1 = it1->GetFieldDef( m_strFeatureClassName, _T("VICS_LINK_ID") );
		
			LONG lOID1 = pOIDFieldDef1->m_vaValue.lVal;
			LONG lMeshCode1 = pMeshCodeFieldDef1->m_vaValue.lVal;
			LONG lVICSClass1 = pVICSClassFieldDef1->m_vaValue.lVal;
			LONG lVICSID1 = pVICSLinkFieldDef1->m_vaValue.lVal;

			for( std::list<CLQRowDef>::iterator it2 = m_pFeatureDefList->begin(); it2 != m_pFeatureDefList->end(); it2++ )
			{
				if( m_strFeatureClassName.CompareNoCase( it2->GetTableName() ) == 0 )
				{
					CFieldDef* pOIDFieldDef2 = it2->GetFieldDef( m_strFeatureClassName, _T("OBJECTID"));
					CFieldDef* pMeshCodeFieldDef2 = it2->GetFieldDef( m_strFeatureClassName, _T("VICS_MESHCODE") );
					CFieldDef* pVICSClassFieldDef2 = it2->GetFieldDef( m_strFeatureClassName, _T("VICSCLASS_C") );
					CFieldDef* pVICSLinkFieldDef2 = it2->GetFieldDef( m_strFeatureClassName, _T("VICS_LINK_ID") );
				
					LONG lOID2 = pOIDFieldDef2->m_vaValue.lVal;
					LONG lMeshCode2 = pMeshCodeFieldDef2->m_vaValue.lVal;
					LONG lVICSClass2 = pVICSClassFieldDef2->m_vaValue.lVal;
					LONG lVICSID2 = pVICSLinkFieldDef2->m_vaValue.lVal;

					if( lOID1 != lOID2 )
					{
						if( lMeshCode1 == lMeshCode2 && lVICSClass1 == lVICSClass2 && lVICSID1 == lVICSID2 )
						{
							strMsg.Format( AheLoadString( IDS_SAMEVICS ), m_strFeatureClassName, lOID1, lOID2 );
							AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

							bRet = FALSE;
							break;
						}
					}
				}
			}
		}
		if( !bRet )
			return FALSE;
	}

	// 現在存在しているVICSと同じVICSがないかどうか
	for( std::list<CLQRowDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		if( m_strFeatureClassName.CompareNoCase( it->GetTableName() ) == 0 )
		{
			CFieldDef* pOIDFieldDef = it->GetFieldDef( m_strFeatureClassName, _T("OBJECTID"));
			CFieldDef* pMeshCodeFieldDef = it->GetFieldDef( m_strFeatureClassName, _T("VICS_MESHCODE") );
			CFieldDef* pVICSClassFieldDef = it->GetFieldDef( m_strFeatureClassName, _T("VICSCLASS_C") );
			CFieldDef* pVICSLinkFieldDef = it->GetFieldDef( m_strFeatureClassName, _T("VICS_LINK_ID") );
		
			LONG lOID = pOIDFieldDef->m_vaValue.lVal;
			LONG lMeshCode = pMeshCodeFieldDef->m_vaValue.lVal;
			LONG lVICSClass = pVICSClassFieldDef->m_vaValue.lVal;
			LONG lVICSID = pVICSLinkFieldDef->m_vaValue.lVal;

			CString strMsg;

			IMapPtr			ipMap;
			ITablePtr		ipTable;
			_ICursorPtr		ipCursor;
			IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
			CString strWhereClause, strSubFields;
			ipMap = m_cArcHelper.GetMap();
        
			// InfVICSテーブル取得
			_IRowPtr ipCurrentRow;
			IWorkspacePtr ipWorkspace;
			ITablePtr ipTestTable;
			IDatasetPtr ipDataset;

			ipCurrentRow = it->GetFeature();
			ipCurrentRow->get_Table( &ipTestTable );
			ipDataset = ipTestTable;
			if( ipDataset )
				ipDataset->get_Workspace( & ipWorkspace );
			CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentRow );
			if( !strOwnerName.IsEmpty() )
				strOwnerName += _T(".") + m_strFeatureClassName;
			else
				strOwnerName = m_strFeatureClassName;

			ipTable = AheOpenTableByTableName( ipWorkspace, strOwnerName );

			// メッシュコードで検索
			strWhereClause.Format(_T("VICS_MESHCODE = %d AND VICSCLASS_C = %d AND VICS_LINK_ID = %d"), lMeshCode, lVICSClass, lVICSID );
			strSubFields.Format(_T("OBJECTID,VICS_LINK_ID,VICSCLASS_C"));
			ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
			ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
			ipTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

			_IRowPtr ipRow;
			if( ipCursor )
			{
				IFieldsPtr ipFields;
				IFieldPtr ipField;
				LONG lObjectIDIndex = -1;
				LONG lLinkIDIndex = -1;
				LONG lVICSClassIndex = -1;

				while( ipCursor->NextRow( &ipRow ) == S_OK )
				{
					if( lObjectIDIndex = -1 || lLinkIDIndex == -1 || lVICSClassIndex == -1 )
					{
						ipRow->get_Fields( &ipFields );
						ipFields->FindField( CComBSTR(_T("OBJECTID")), &lObjectIDIndex );
						ipFields->FindField( CComBSTR(_T("VICS_LINK_ID")), &lLinkIDIndex );
						ipFields->FindField( CComBSTR(_T("VICSCLASS_C")), &lVICSClassIndex );
					}

					VARIANT vaObjectID;
					ipRow->get_Value( lObjectIDIndex, &vaObjectID );

					if( lOID != vaObjectID.lVal )
					{
						VARIANT vaLinkIDValue;
						ipRow->get_Value( lLinkIDIndex, &vaLinkIDValue );
						VARIANT vaVICSClassValue;
						ipRow->get_Value( lVICSClassIndex, &vaVICSClassValue );
						strMsg.Format( AheLoadString( IDS_SAMEVICS ), m_strFeatureClassName, lOID, vaObjectID.lVal );
						AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

						bRet = FALSE;
						break;
					}
				}
			}			


			if( pMeshCodeFieldDef->m_vaValue.vt == VT_NULL || pMeshCodeFieldDef->m_vaValue.lVal == 0 )
			{
				strMsg.Format( AheLoadString( IDS_NOMESHCODE ), m_strFeatureClassName, lOID );
				AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

				bRet = FALSE;
				break;
			}
			if( pMeshCodeFieldDef->m_vaValue.lVal < 100000 || pMeshCodeFieldDef->m_vaValue.lVal > 999999 )
			{
				strMsg.Format( AheLoadString( IDS_NOTMESHCODE6 ), m_strFeatureClassName, lOID );
				AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

				bRet = FALSE;
				break;
			}
			if( pVICSClassFieldDef->m_vaValue.vt == VT_NULL || pVICSClassFieldDef->m_vaValue.lVal == 0  )
			{
				strMsg.Format( AheLoadString( IDS_NOVICSCLASS ), m_strFeatureClassName, lOID );
				AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

				bRet = FALSE;
				break;
			}
			if( pVICSLinkFieldDef->m_vaValue.vt == VT_NULL ||  pVICSLinkFieldDef->m_vaValue.lVal == 0 )
			{
				strMsg.Format( AheLoadString( IDS_NOLINKCODE ), m_strFeatureClassName, lOID );
				AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

				bRet = FALSE;
				break;
			}
		}
	}
	
	return bRet;
}
