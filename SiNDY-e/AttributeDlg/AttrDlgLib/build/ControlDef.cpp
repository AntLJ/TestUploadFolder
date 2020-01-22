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

// ControlDef.cpp: CControlDef クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../ControlDef.h"

#include <TDC/useful_headers/str_util.h>

//#include <atlhost.h>

namespace {
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR ERROR_MORE_TAHN_32_DIGITS = _T("You can not enter more than 32 digits.");
	LPCTSTR ERROR_CAPTION             = _T("ERROR");
	LPCTSTR WARN_OVER_1024_BYTES      = _T("You can not enter more than 1024 bytes. Please note that the value will not be set as being entered now. ");
#else
	LPCTSTR ERROR_MORE_TAHN_32_DIGITS =	_T("32桁以上入力できません");
	LPCTSTR ERROR_CAPTION             = _T("エラー");
	LPCTSTR WARN_OVER_1024_BYTES      = _T("1024バイト以上入力できません。今入力されている通りに値はセットされません。ご注意ください。");
#endif


	/**
	 * @brief 指定VARTYPE が整数型か否か
	 * @param type VARTYPE
	 * @retval true  整数型である
	 * @retval false 整数型ではない
	 */
	bool isIntegerType(VARTYPE type)
	{
		return
			type == VT_I2 || type == VT_I4  || type == VT_UI2 || type == VT_UI4 ||
			type == VT_I8 || type == VT_UI8 || type == VT_INT || type == VT_UINT
			;
	}
}

/**
 * コンストラクタ
 */
CControlDef::CControlDef()
{
	m_ipRule.CreateInstance( CLSID_SiNDYRuleObj );
	Clear();
}
/**
 * デストラクタ
 */
CControlDef::~CControlDef()
{
	Clear();
}
void CControlDef::SetControlDef(IApplication* ipApp,    
								INT			nID, 
								HWND				hWnd,
								  CONTROLTYPE	type, 
								  INT			nGroupFrom,
								  INT			nGroupTo,
								  LPCTSTR		lpszFieldName/* = NULL*/, 
								  UINT			uiCheckType/* = NULL */,
								  LPCTSTR		lpszTableName/* = NULL */, 
								  LPCTSTR		lpszOuterKey/* = NULL */, 
								  INT			nFollowCtrlID/* = NULL */,
								  UINT			uiFollowBy/* = NULL */,
								  INT			nRelType)

{
	m_nID = nID;
	m_uiCheckType = uiCheckType;
	m_nGroupFrom = nGroupFrom;
	m_nGroupTo = nGroupTo;
	m_type = type;
	m_nFollowCtrlID = nFollowCtrlID;
	m_uiFollowBy = uiFollowBy;
	m_cArcHelper.Init( (IApplicationPtr)ipApp );

	m_hWnd = ::GetDlgItem( hWnd, m_nID );

	m_strTableName = lpszTableName;
	m_listFieldName.push_back( lpszFieldName );
}
void CControlDef::SetControlDef(IApplication* ipApp,  
								INT			nID, 
								HWND			hWnd,
								  CONTROLTYPE	type, 
								  INT			nGroupFrom,
								  INT			nGroupTo,
								  std::list<CString> arrayFieldNames, 
								  UINT			uiCheckType/* = NULL */,
								  LPCTSTR		lpszTableName/* = NULL */, 
								  LPCTSTR		lpszOuterKey/* = NULL */, 
								  INT			nFollowCtrlID/* = NULL */,
								  UINT			uiFollowBy/* = NULL */,
								  INT			nRelType/*,
								  IUnknownPtr	ipUnk*//* = NULL */)

{
	m_nID = nID;
	m_uiCheckType = uiCheckType;
	m_nGroupFrom = nGroupFrom;
	m_nGroupTo = nGroupTo;
	m_type = type;
	m_nFollowCtrlID = nFollowCtrlID;
	m_uiFollowBy = uiFollowBy;
	m_cArcHelper.Init( (IApplicationPtr)ipApp );

	m_hWnd = ::GetDlgItem( hWnd, m_nID );

	m_strTableName = lpszTableName;
	m_listFieldName = arrayFieldNames;
}
void CControlDef::SetFeatureDefList( std::list<FEATUREDEF>* pFeatureDefList )
{
	ATLASSERT( pFeatureDefList != NULL );
	ClearFeatureDef();

	m_pFeatureDefList = pFeatureDefList;
}
void CControlDef::SetCurrentFeatureDefIndex( LONG lIndex, LONG lTableIndex, LONG lRowIndex )
{
	/// 与えられたインデックスよりフィーチャの数が少ないなら戻る
	if( !m_pFeatureDefList || m_pFeatureDefList->size() <= (UINT)lIndex ) 
	{
		m_pFeatureDef = NULL;
		m_lTableIndex = m_lRowIndex = 0;

		return;
	}
	std::list<FEATUREDEF>::iterator it = m_pFeatureDefList->begin();
	advance( it, lIndex );

	m_pFeatureDef = &(*it);

	m_lTableIndex = lTableIndex;
	m_lRowIndex = lRowIndex;
}
CFieldDef* CControlDef::GetFieldDef( LPCTSTR lpcszFieldName )
{
	if( !lpcszFieldName ) {
		lpcszFieldName = *m_listFieldName.begin();

		/// この関数は対応するフィールドの数が一つの場合に限定なので
		if( m_listFieldName.size() > 1 ) {
			ATLASSERT(FALSE);
			return NULL;
		}
	}

	/// フィーチャが一つも登録されていない場合は NULL を返す
	if( !m_pFeatureDef ) return NULL;

	/// フィーチャフィールド
	// TYPE_FEATURESGRID の時はスルーするようにする（CTableEditDlg::SortByField() がうまくいかないため）
	if( m_type == TYPE_FEATURESGRID || lstrcmpi( m_pFeatureDef->GetTableName(), m_strTableName ) == 0 )
	{
		for( std::list<CFieldDef>::iterator itField = m_pFeatureDef->begin(); itField != m_pFeatureDef->end(); itField++ )
		{
			if( 0 == itField->m_strFieldName.CompareNoCase( lpcszFieldName ) )
				return &(*itField);
		}
	}
	/// 外部テーブルフィールド
	else
	{
		std::list<CRelationalTableDef>* pRelationalTableDef = m_pFeatureDef->GetRelationalTableDefList();
		for( std::list<CRelationalTableDef>::iterator it = pRelationalTableDef->begin(); it != pRelationalTableDef->end(); it++ ) 
		{
			if( lstrcmpi( it->GetTableName(), m_strTableName ) == 0 ) 
			{
				/// LINK_TIME_REG で固定値ドメインを取得する時にどうしても ASSERT になるので、ここは外す
				//ATLASSERT( it->size() == 1 );	//!< この関数は対応するフィールドの数が一つの場合に限定なので
				ATLTRACE(_T("CControlDef::GetFieldDef() : この関数は対応するフィールドが一つの場合に限り正常に動作します NUM=%d\n"), it->size() );
				for( std::list<CRowDef>::iterator itRow = it->begin(); itRow != it->end(); itRow++ )
				{
					for( std::list<CFieldDef>::iterator itField = itRow->begin(); itField != itRow->end(); itField++ )
					{
						if( 0 == itField->m_strFieldName.CompareNoCase( lpcszFieldName ) )
							return &(*itField);
					}
				}
			}
		}
	}

	return NULL;
}
void CControlDef::dump( LPCTSTR s )
{

}
/**
 * 対応するフィールドのドメインを取得する
 *
 *  データベースに問い合わせするのは一度限り（一度読み込んだら内部で保存）
 * 
 * @return 失敗した場合は NULL が返る
 */
DOMAINMAP* CControlDef::GetDomain( LPCTSTR lpcszFieldName )
{
	CFieldDef* pFieldDef = GetFieldDef( lpcszFieldName );

	if( !pFieldDef ) {
		ATLTRACE(_T("CControlDef::GetDomain(): 関連するフィールドがありません ID=%d\n"), m_nID );
		return FALSE;
	}

	return pFieldDef->GetDomain();
}
void CControlDef::Clear()
{
	m_nID = -1;
	m_type = 0;
	m_uiCheckType = 0x00;
	m_nGroupFrom = -1;
	m_nGroupTo = -1;
	m_nFollowCtrlID = -1;
	m_uiFollowBy = 0x00;
	m_cArcHelper.Release();
	m_hWnd = NULL;


	m_bErrored = FALSE;
	ClearFeatureDef();

	m_bAliasOrField = FALSE;

	m_listFieldName.clear();
}

/* つかわない？*/
void CControlDef::Reset()
{
	// エラーフラグはここでクリア
	m_bErrored = FALSE;
}
BOOL CControlDef::GetOldValue( CComVariant& vaValue )
{
	CFieldDef* pFieldDef = GetFieldDef();

	if( !pFieldDef ) {
		ATLTRACE(_T("CControlDef::GetOldValue(): 関連するフィールドがありません ID=%d\n"), m_nID );
		return FALSE;
	}

	vaValue = pFieldDef->m_vaValue;

	return TRUE;
}
void CControlDef::GetStringValue( CString& str )
{
	CFieldDef* pFieldDef = GetFieldDef();

	ATLASSERT( pFieldDef != NULL );
	if( !pFieldDef ) return ;
	
	str = pFieldDef->value2string();
}
/**
 * 与えられたLONG値を m_vaValue にセット
 *  その際、初期値から変更されていれば m_bChanged = TRUE に、変更されていなければ FALSE にセットする
 *  一番最初のフィールドしか見ないので、複数のフィールドを扱うものには適用できない
 * 
 * @param l				[in]	セットする LONG 値
 *
 */
void CControlDef::SetValue( CComVariant vaValue )
{
	CFieldDef* pFieldDef = GetFieldDef();

	if( !pFieldDef ) return ;

	// [Bug4731]値のチェックはするけれども、値が範囲外だったりしたとしても、現在の値を保持しておく。
	// 範囲外の値だったとしても、それは最終的にチェックされ確定できないため、問題なし。
	pFieldDef->CheckValue( vaValue, GetParent( m_hWnd ) );
	pFieldDef->m_vaValue = vaValue;

	/// 初めてセットされた場合
	/// こうしないと CFieldDef::Store() 時にはじかれる
	if( pFieldDef->m_bIsUnset )
		pFieldDef->m_bIsUnset = FALSE;

	// [Bug 5794] [Bug5775]の修正により属性値の変更が元に戻った場合の処理が消されてしまったのでちゃんとm_bChangedを戻せるようにした
	if( (pFieldDef->m_vaValue).vt == VT_BSTR )
	{
		// [bug 5775] VT_BSTR な VARIANT をオペレータで比較すると「―」と「ー」が区別されないようだ
		if( pFieldDef->m_vaInitValue.vt == VT_BSTR && 0 == wcscmp((pFieldDef->m_vaValue).bstrVal, (pFieldDef->m_vaInitValue).bstrVal))
			pFieldDef->m_bChanged = FALSE;
		// DATE型は文字列として処理するため、比較時はDATE型に戻す
		else if( pFieldDef->m_vaInitValue.vt == VT_DATE )
		{
			CComVariant vaDateValue = vaValue;
			vaDateValue.ChangeType( VT_DATE );

			if( vaDateValue.date == (pFieldDef->m_vaInitValue).date )
				pFieldDef->m_bChanged = FALSE;
			else
				pFieldDef->m_bChanged = TRUE;
		}
		else
			pFieldDef->m_bChanged = TRUE;
	}
	else
	{
		if( pFieldDef->m_vaValue != pFieldDef->m_vaInitValue )
			pFieldDef->m_bChanged = TRUE;
		else
			pFieldDef->m_bChanged = FALSE;
	}
	ATLTRACE( pFieldDef->m_bChanged ? _T("CControlDef::SetValue() : 値が変更されました %s\n") : _T("CControlDef::SetValue() : 値が初期状態に戻りました %s\n"), pFieldDef->m_strFieldName );


	// エディットコントロールの場合は _ttol で変換しているので再表示したほうが良い
	// 空白は全て <null> と表示したいので、ストリングタイプも追加
//	if( m_type == TYPE_EDIT || m_type == TYPE_EDITSTRING ) SetEdit();
}
/**
 * コントロールから値を取得し、変更されていれば m_vaValue に値をセットする
 *
 *  変更フラグは SetLongValue() でセットする
 *
 * @return m_vaValue の値が変更されたら TRUE、変更されなかったら FALSE
 */
BOOL CControlDef::SetValueFromControl()
{
	CComVariant vaCtrlVal;
	if( !GetControlValue( vaCtrlVal ) ) return FALSE;

	CComVariant vaOldVal;
	GetOldValue( vaOldVal );


	// [bug 5775] VT_BSTR な VARIANT をオペレータで比較すると「―」と「ー」が区別されないようだ
	if( vaCtrlVal.vt == VT_BSTR && vaOldVal.vt == VT_BSTR && (0 != wcscmp( vaCtrlVal.bstrVal, vaOldVal.bstrVal )) )
	{
		SetValue( vaCtrlVal );
		return TRUE;
	}

	// DATE型は文字列として処理するため、比較時はDATE型に戻す
	else if( vaCtrlVal.vt == VT_BSTR && vaOldVal.vt == VT_DATE )
	{
		CComVariant vaDateVal = vaCtrlVal;
		vaDateVal.ChangeType( VT_DATE );
		if( vaDateVal != vaOldVal )
		{
			SetValue( vaCtrlVal );
			return TRUE;
		}
	}

	/// 変更されている
	else if( vaCtrlVal != vaOldVal ) {
		SetValue( vaCtrlVal );
		return TRUE;
	}
	else {
		// NULL の場合は、<null> と空白の場合があるので、必ず <null> 表示にしなおす
		if( vaCtrlVal.vt == VT_NULL )
			SetValue( vaCtrlVal );
	}

	return FALSE;
}
/**
 * 現在コントロールに表示されている・あるいは選択されている値を取得
 *
 * LONG値限定
 * エディットコントロールの場合は無理やりLONG値に直すため、数値がおかしくなる場合あり
 *
 * @param def			[in]	コントロール
 * @param lVal			[out]	取得された値
 * 
 * @return 値が取得できれば TRUE、出来なければ FALSE
 */
BOOL CControlDef::GetControlValue( CComVariant& vaVal )
{
	TCHAR strVal[2000];
	INT nByte = -1;
	_ASSERTE( vaVal.vt == VT_NULL || vaVal.vt == VT_EMPTY );
	vaVal.Clear();

	switch( m_type ) {
	case TYPE_COMBO:
	{
		int nCurSel = (int)::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0L);
		if (0 <= nCurSel)
		{
			DWORD dwTmp = ::SendMessage(m_hWnd, CB_GETITEMDATA, nCurSel, 0L);
			// コンボボックスで且つ 0xFFFFFFF がセットされていたら NULL とする。 @see SetComboBox()
			if (dwTmp == 0xFFFFFFF)
			{
				vaVal.vt = VT_NULL;
			}
			else
			{
				// 先にOKボタンが押され初期化後にここに飛んで来ることがある。
				// そのため、ここでFieldDef が取れない場合は何もせず終了する。
				CFieldDef* fieldDef = GetFieldDef();
				if (!fieldDef)
					return FALSE;
				// 整数型のドメイン値ならば、コンボボックスにコード値が格納されているのでそのまま取得する
				// それ以外のドメイン型は、コンボボックスにインデックスが格納されているので、GetCodeDomainValue() で取り出す
				// ドメインでない値が格納されている場合はVT_NULLとなるので、それもそのまま取得する
				VARTYPE type = fieldDef->GetCodedDomainType();
				if(isIntegerType(type) || type == VT_NULL )
					vaVal = (long)dwTmp;
				else
					vaVal = GetFieldDef()->GetCodeDomainValue(dwTmp);
			}
		}
		break;
	}
	case TYPE_EDIT:
	case TYPE_EDITFLOAT:
	case TYPE_EDITDOUBLE:
		// [Bug4843]数値入力で32桁以上はないと勝手にしてしまう
		if( ::GetWindowTextLength( m_hWnd ) > 32 ) {
			AheMessageBox( NULL, ERROR_MORE_TAHN_32_DIGITS, ERROR_CAPTION, MB_OK|MB_TASKMODAL );
			return FALSE;
		}
		::GetWindowText( m_hWnd, strVal, 32 );	//!< 32バイト以上入力することは無いと仮定
		// <null> の場合を考慮
		if( lstrcmpi( strVal, NULL_VALUE ) == 0 || lstrcmpi( strVal, _T("") ) == 0 )
			vaVal.vt = VT_NULL;
		else
		{
			if( m_type == TYPE_EDIT )
			{
				vaVal.vt = VT_I4;
				vaVal.lVal = _ttol( strVal );
			}
			// [Bug 5333]歩行者NW(WALK_LINK)のFLOOR_LEVELがFLOATの値をとるため、特別対応しなくてはいけない
			else if( m_type == TYPE_EDITFLOAT )
			{
				vaVal.vt = VT_R4;
				vaVal.fltVal = _ttof(strVal);
			}
			else if( m_type == TYPE_EDITDOUBLE )
			{
				vaVal.vt = VT_R8;
				vaVal.dblVal = _ttof(strVal);
			}
		}
		break;
	case TYPE_CHECKBOX:
		vaVal = (long)::SendMessage(m_hWnd, BM_GETCHECK, 0, 0L);
		break;
	case TYPE_EDITSTRING:
		// [Bug 5418]文字数制限1024バイトまでOKになった。→	// [Bug 4891]文字数制限は512バイトまででOKとのこと
		// GetWindowTextLengthだと、文字数でのチェックになってしまうので、ANSIに変換しバイト数のチェックにする
		// 1024＋終端コード＋1バイト取得してチェック、::GetWindowTextの第3引数はUNICODE下では文字数となってしまう
		// のであまり意味はないかも、、
		::GetWindowText( m_hWnd, strVal, 1026 );	 
		nByte = ::WideCharToMultiByte(CP_ACP, 0, T2CW((LPCTSTR)(strVal)), -1, NULL, 0, NULL, NULL) - 1;
		if( nByte > 1024 )
			AheMessageBox( NULL, WARN_OVER_1024_BYTES, ERROR_CAPTION, MB_OK|MB_TASKMODAL );
		// <null> の場合を考慮
		if( lstrcmpi( strVal, NULL_VALUE ) == 0 || lstrcmpi( strVal, _T("") ) == 0 )
			vaVal.vt = VT_NULL;
		else
			vaVal = (LPCTSTR)strVal;
		break;
	default:
		return FALSE;
		break;
	}

	return TRUE;
}
/**
 * 格納されている値がエラーかどうかの判定
 *
 * @return エラーなら TRUE、そうでなければ FALSE
 */
BOOL CControlDef::Errored()
{
	return m_bErrored;
}
/**
 * コントロールに関連するフィールドに変更があるかどうかの判定
 *
 * @return 変更があれば TRUE、変更がなければ FALSE
 */
BOOL CControlDef::Changed()
{
	FEATUREDEF* pFeatureDef = NULL;
	BOOL bRet = FALSE;

	// コントロールがTYPE_FEATURESGRIDの場合、全てのフィーチャのフィールドを調べる必要がある
	if( m_type == TYPE_FEATURESGRID || m_type == TYPE_FEATUREGRID) {
		if( !m_pFeatureDefList )
			return FALSE;

		for( std::list<FEATUREDEF>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
		{
			bRet = _Changed( &(*it) );
			
			// 一つでも変更があったら終了
			if( bRet )
				break;
		}
	} 
	// それ以外のタイプの場合
	else {
		if( !m_pFeatureDef ) return FALSE;
	
		pFeatureDef = m_pFeatureDef;
		bRet = _Changed( pFeatureDef );
	}

	return bRet;
}
/**
 * Changed() から呼ばれる内部関数
 *
 * @param pFeatureDef フィーチャ定義クラス
 *
 * @return 変更があれば TRUE、なければ FALSE
 */
BOOL CControlDef::_Changed( FEATUREDEF* pFeatureDef )
{
	ATLASSERT( pFeatureDef != FALSE );

	CFieldDef* pFieldDef = NULL;
	BOOL bRet = FALSE;

	// フィーチャフィールド
	if( lstrcmpi( pFeatureDef->GetTableName(), m_strTableName ) == 0 ||
		m_type == TYPE_FEATUREGRID )	// TYPE_FEATUREGRID の場合はフィールド名もテーブル名も登録されない
	{
		for( std::list<CFieldDef>::iterator itField = pFeatureDef->begin(); itField != pFeatureDef->end(); ++itField )
		{
			// TYPE_FEATUREGRID の場合はフィールド名も登録されないので m_listFieldName で回すのは NG
			if( m_type == TYPE_FEATUREGRID )
			{
				if( itField->m_bChanged )
				{
					bRet = TRUE;
					pFieldDef = &(*itField);
					break;
				}
			} 
			// TYPE_FEATUREGRID 以外
			else {
				for( std::list<CString>::iterator itFieldName = m_listFieldName.begin(); itFieldName != m_listFieldName.end(); ++itFieldName )
				{
					if( 0 == itField->m_strFieldName.CompareNoCase( *itFieldName ) )
					{
						if( itField->m_bChanged )
						{
							bRet = TRUE;
							pFieldDef = &(*itField);
							break;
						}
					}
				}
			}
			if( bRet )
				break;
		}
	}
	// 外部テーブルフィールド
	else
	{
		std::list<CRelationalTableDef>* pRelationalTableDef = pFeatureDef->GetRelationalTableDefList();
		for( std::list<CRelationalTableDef>::iterator it = pRelationalTableDef->begin(); it != pRelationalTableDef->end(); it++ ) 
		{
			if( 0 == m_strTableName.CompareNoCase( it->GetTableName() ) ) 
			{
				// LINK_TIME_REG で固定値ドメインを取得する時にどうしても ASSERT になるので、ここは外す
				///ATLASSERT( it->size() == 1 );	// この関数は対応するフィールドの数が一つの場合に限定なので
				ATLTRACE(_T("CControlDef::Changed() : この関数は対応するフィールドが一つの場合に限り正常に動作します NUM=%d\n"), it->size() );
				for( std::list<CRowDef>::iterator itRow = it->begin(); itRow != it->end(); ++itRow )
				{
					// 外部テーブルのレコードに新規作成フラグ又は削除フラグが付いていたら変更ありとする
					if( itRow->GetDeleteFlag() || itRow->GetCreateFlag() )
					{
						// デバッグ
						ATLTRACE(_T("CControlDef::Changed()：外部テーブルのレコードに新規作成フラグ又は削除フラグが付いています\n"));

						return TRUE;
					}

					for( std::list<CFieldDef>::iterator itField = itRow->begin(); itField != itRow->end(); itField++ )
					{
						for( std::list<CString>::iterator itFieldName = m_listFieldName.begin(); itFieldName != m_listFieldName.end(); itFieldName++ )
						{
							if( 0 == itField->m_strFieldName.CompareNoCase( *itFieldName ) )
							{
								if( itField->m_bChanged )
								{
									bRet = TRUE;
									pFieldDef = &(*itField);
									break;
								}
							}
						}
						if( bRet )
							break;
					}
					if( bRet )
						break;
				}
			}
			if( bRet )
				break;
		}
	}
	// デバッグ
	if( bRet )
	{
		ATLTRACE(_T("CControlDef::Changed()：値が変更されています：%s value=%s and init value=%s\n"), 
			pFieldDef->m_strFieldName, pFieldDef->value2string(), pFieldDef->value2string(TRUE) );
	}

	return bRet;
}
/**
 * このコントロールが DISP_CLASS を扱うコントロールかどうかの判定
 *
 * @return DispClass を扱うコントロールなら TRUE、DispClass では無いまたは複数レコード、複数フィールドの場合は FALSE
 */
BOOL CControlDef::IsDispClass()
{
	if( m_listFieldName.size() != 1 ) return FALSE;

	if( lstrcmpi( *m_listFieldName.begin(), _T("ROADCLASS_C") ) == 0 ) return TRUE;

	return FALSE;
}
BOOL CControlDef::IsUnset()
{
	CFieldDef* pFieldDef = GetFieldDef();

	if( !pFieldDef ) {
		ATLTRACE(_T("CControlDef::IsUnset(): 関連するフィールドがありません ID=%d\n"), m_nID );
		return TRUE;
	}

	return pFieldDef->m_bIsUnset;
}
LPCTSTR CControlDef::GetTableName()
{	
	return m_strTableName;
}
/**
 * このコントロールが扱うフィールド名を取得
 *  複数フィールドを扱うコントロールの場合は使用しないこと（複数レコードの場合はOK）
 *
 * @return 複数フィールドの場合は NULL を返す
 */
LPCTSTR CControlDef::GetFieldName()
{	
	if( m_listFieldName.size() != 1 ) return NULL;

	return *m_listFieldName.begin();

}

/**
 * コントロールの入力状態を変更する
 *
 * @param bEnable	[in]	Enable にする場合は TRUE、Disable にする場合は FALSE
 * @param bForce	[in]	強制的に制御するかどうか（編集開始、終了のハンドリング時のみに使用）
 * @param bEditable	[in]	強制的に編集可・不可を設定する（編集開始、終了のハンドリング時のみに使用）
 */
void CControlDef::ChangeEnable( BOOL bEnable, BOOL bForce, BOOL bEditable )
{
	if ( m_type == TYPE_NONE )
		return;

	if( (m_type == TYPE_OTHER) || (m_type == TYPE_FEATUREGRID) || (m_type ==  TYPE_TIMEREG) || (m_type ==  TYPE_OWTIMEREG) ||
		(m_type == TYPE_OUTERROWS) || (m_type ==TYPE_FEATURESGRID) /*|| (m_type == TYPE_WALKONEWAY) || (m_type == TYPE_WALKNOPASSAGE)*/)
	{
		bEnable = TRUE;
	}
	else {

		// 編集不可の時には絶対編集させない
		if( m_pFeatureDef->m_sindyeEditableType == sindyeEditableNoEditable )
			bEnable = FALSE;

		// 編集警告の時には、編集警告解除ボタンを押したかどうかでチェック
		if( m_pFeatureDef->m_sindyeEditableType == sindyeEditableWarnning )
		{
			if( m_pFeatureDef->GetWarnDelete() )
				bEnable = TRUE;
			else 
				bEnable = FALSE;
		}

		// 強制的に編集不可の時は絶対に編集させないワーニングとか関係なし
		if( bForce && !bEditable ) 
			bEnable = FALSE;
	}


	DWORD dwRemove, dwAdd;

	if( !(m_type == TYPE_EDIT || m_type == TYPE_EDITFLOAT || m_type == TYPE_EDITDOUBLE || m_type == TYPE_EDITSTRING )) {
		if( bEnable ) {
				dwAdd = NULL;
				dwRemove = WS_DISABLED;
		} else {
			dwAdd = WS_DISABLED;
			dwRemove = NULL;
		}
		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if(dwStyle == dwNewStyle)
			return;
		::SetWindowLong(m_hWnd, GWL_STYLE, dwNewStyle);
	}
	/// エディットコントロールの場合だけ
	if( m_type == TYPE_EDIT || m_type == TYPE_EDITFLOAT || m_type == TYPE_EDITDOUBLE || m_type == TYPE_EDITSTRING ) {
		if( bEnable )
			::SendMessage( m_hWnd, EM_SETREADONLY, FALSE, 0L );
		else
			::SendMessage( m_hWnd, EM_SETREADONLY, TRUE, 0L );
	}

	return ;
}
/**
 * コンボボックスのリストの作成を行う
 *
 * DISPCLASS_C の値でチェックし、リストの内容を決定します。
 *
 * @param lDispClass	[in]	DISPCLASS_C の値
 */
void CControlDef::SetComboBox( LONG lDispClass )
{
	//IFieldsPtr           ipFields           = NULL;
	//IFieldPtr            ipField            = NULL;
	//IDomainPtr           ipDomain           = NULL; 
	//ICodedValueDomainPtr ipCodedValueDomain = NULL;


	USES_CONVERSION;

	// エラーフラグはここで一旦 FALSE にしておかないと、フィーチャを切り替えた後も有効になってしまう
	m_bErrored = FALSE;

	DOMAINMAP* mapDomain = GetDomain();

	// コンボボックス内のアイテムを全てクリア
	CComboBox cmb( m_hWnd );
	cmb.ResetContent();

	// CFieldDef::m_vaValue に登録されている値を取得
	CComVariant vaOldValue;
	GetOldValue( vaOldValue );

	// 値限定ドメイン
	if( mapDomain )
	{
		for( auto& domain : *mapDomain )
		{
			CString strDomainName	= domain.code_discription;
			cmb.InsertString( domain.idx, strDomainName );

			if(isIntegerType(domain.code.vt))
				cmb.SetItemData( domain.idx, domain.code.lVal );
			else {
				// 整数型のドメイン以外のときは暫定的にインデックスの値を入れておく
				cmb.SetItemData(domain.idx, domain.idx);
			}

			// 現在の値と同じ物が見つかったらそれを表示
			if( vaOldValue == domain.code ) {
				cmb.SetCurSel( domain.idx );
			}
		}
	}

	// Nullable なフィールドの場合はリストに追加
	if( IsNullable() ) 
	{
		BOOL bNullAdd = TRUE;
		// DISPOTHER で必須になっているものは NULL はコンボボックスに入れない
		if( m_uiCheckType & CHECK_DISPOTHER )
		{
			sindyeDispOtherRelation nDispOtherRel;
			m_ipRule->GetDispOtherRule( lDispClass, CComBSTR(GetFieldName()), &nDispOtherRel );
			if( ( nDispOtherRel == sindyeDispOtherOption ) || ( nDispOtherRel == sindyeDispOtherExceptNoRes ) )
				bNullAdd = FALSE;
		}

		if( bNullAdd )
		{
			long count = cmb.GetCount();
			cmb.InsertString(count, NULL_VALUE );
			cmb.SetItemData(count, 0xFFFFFFF );	// <null> の値は 0xFFFFFFF にしておく
			if( vaOldValue.vt == VT_NULL ) {
				cmb.SetCurSel(count + 1 );
			}
		}
	}
}
void CControlDef::SetEdit()
{
	CString strVal;

	CEdit edit( m_hWnd );
	INT nTextLen = edit.GetWindowTextLength() + 1;
	LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	edit.GetWindowText( lpszText, nTextLen );

	/// m_bIsUnset が TRUE の場合は空白をセット
	if( IsUnset() != TRUE ) {
		if( m_type == TYPE_EDITSTRING ) {
			GetStringValue( strVal );
		} else {
			CComVariant vaValue;
			GetOldValue( vaValue );
			// VT_NULL の場合は何も入れない
			if( vaValue.vt != VT_NULL )
			{
				// [Bug 5333]歩行者NMのFLOOR_LEVELで、ついにVT_R4が登場
				// 入力側（子ダイアログ側）で少数点1桁までしか入力できないようにしてしまっている
				if( vaValue.vt == VT_R4 )
					strVal.Format( _T("%lf"), vaValue.fltVal ); 
				else
				if( vaValue.vt == VT_R8 )
					strVal.Format( _T("%lf"), vaValue.dblVal ); 
				else
				{
					ATLASSERT( vaValue.vt == VT_I4 );
					strVal.Format(_T("%d"), vaValue.lVal );	//!< 面倒なのでLONG限定
				}
			}
		}
	}

	// 値が変わっていたらテキストを書き換える
	if( strVal.Compare( lpszText ) != 0 )
		::SetWindowText( m_hWnd, strVal );

}
void CControlDef::SetCheckBox()
{
	CComVariant vaValue;
	GetOldValue( vaValue );
	//[Bug 5531]ひまわりフラグなどの昔のデータではフィールドのない値の場合にはチェックをはずし、編集不可にしておく
	if( vaValue.vt == VT_EMPTY )
	{
		::SendMessage( m_hWnd, BM_SETCHECK, 0, 0L);
		::EnableWindow( m_hWnd, FALSE);
	}
	else
		::SendMessage( m_hWnd, BM_SETCHECK, vaValue.lVal, 0L);
}
void CControlDef::SetFeaturesGrid( BOOL bMulti )
{
	/// グリッド取得
	IVSFlexGridPtr spGrid;
	CAxWindow wndGrid( m_hWnd );
	wndGrid.QueryControl(&spGrid);

	if( !spGrid )
	{
		ATLASSERT( spGrid != NULL );
		return;
	}

	if( bMulti ) {
		LONG lCols = 1;
		if( m_pFeatureDefList->size() > 0 )
		{
			lCols = m_listFieldName.size() + 1;
		}
		spGrid->put_Cols( lCols );
		spGrid->put_Rows( 1 );
		spGrid->put_FixedCols( 1 );
		spGrid->put_FixedRows( 1 );
	} else {
		/// 初期化（1行2列。一行目は FixedRow）なんでここでやらなきゃならんの？わけわかんねー
		spGrid->put_Cols( 2 );
		spGrid->put_Rows( 1 );
		spGrid->put_FixedCols( 1 );
		spGrid->put_FixedRows( 1 );
	}
	LONG lCount = 0;
	
	/// 一つもフィーチャがセットされていないなら戻る
	if( !m_pFeatureDefList ) return;

	for( std::list<FEATUREDEF>::iterator i = m_pFeatureDefList->begin(); i != m_pFeatureDefList->end(); i++, lCount++ ) 
	{
		CString strItem, strItemTmp;
		std::list<LONG> listChanged;
		std::list<LONG> listUnEditable;

		LONG lFieldCount = 0;
		for( std::list<CString>::iterator itFieldNameList = m_listFieldName.begin(); itFieldNameList != m_listFieldName.end(); itFieldNameList++, lFieldCount++ )
		{
			std::list<CFieldDef>::iterator j;
			for( j = i->begin(); j != i->end(); j++ ) 
			{
				/// 同じフィールドを持つものを探す
				if( (*itFieldNameList).CompareNoCase( j->m_strFieldName ) == 0 ) 
					break;
			}

			// 関係ないグリッドの場合はこうなる
			if( j == i->end() )
			{
				return;
			}

			// bMulti == TRUE のときのみフィールドのエイリアス名を一行目に入れる
			if( lCount == 0 && bMulti )
			{
				CComBSTR bstrName;
				j->m_ipField->get_AliasName( &bstrName );

				spGrid->put_TextMatrix( 0, lFieldCount + 1, bstrName );
			}

			CString str;
			// NULL だった場合の処理			
			if( j->m_vaValue.vt == VT_NULL )
			{
				str = NULL_VALUE;
			} 
			// NULL じゃない場合の処理
			else 
			{
				DOMAINMAP* domain = j->GetDomain();
				if( !domain ) {
					va2str( j->m_vaValue, str );
				} else {
					for( DOMAINMAP::iterator l = domain->begin(); l != domain->end(); l++ ) {
						if( l->code == j->m_vaValue ) {
							str = l->code_discription;
							break;
						}
					}
				}
			}
			strItemTmp = strItem;
			if( strItemTmp.Compare( _T("") ) == 0 ) {
				strItem.Format(_T("%d\t%s"), lCount, str );
			} else {
				strItem.Format(_T("%s\t%s"), strItemTmp, str );
			}

			// 編集されているフィールドはインデックス番号保持
			if( j->m_bChanged )
				listChanged.push_back( lFieldCount );
			// 編集不可のフィールドはインデックス番号保持
			if( !j->IsEditable() )
				listUnEditable.push_back( lFieldCount );
		}
		spGrid->AddItem( _bstr_t( strItem ) );

		std::list<LONG>::iterator n;
		// 色を変えるフィールド行を追加した後で変更（編集されているフィールド）
		for( n = listChanged.begin(); n != listChanged.end(); n++ )
			SetGridColorToRed( spGrid, lCount + 1, (*n) + 1 );
		// 色を変えるフィールド行を追加した後で変更（編集不可のフィールド）
		for( n = listUnEditable.begin(); n != listUnEditable.end(); n++ )
			SetGridColorToUnEdit( spGrid, lCount + 1, (*n) + 1 );
		// Deleteフラグが立っていたらグレー表示
		if( i->GetDeleteFlag() )
		{
			for( ULONG lSel = 1; lSel <= m_listFieldName.size(); lSel++ )
				SetGridColor( spGrid, lCount + 1, lSel, 100, 100, 100 );
		}
	}
	// 位置調節
	if( lCount > 0 )
		spGrid->AutoSize( 0, CComVariant( (LONG)m_listFieldName.size() ) );
}
void CControlDef::SetOuterRowsGrid()
{
	/// グリッド取得
	IVSFlexGridPtr spGrid;
	CAxWindow wndGrid( m_hWnd );
	wndGrid.QueryControl(&spGrid);

	ATLASSERT( spGrid != NULL );

	/// グリッド初期化
	spGrid->put_Cols( m_listFieldName.size() );
	spGrid->put_Rows( 1 );
	spGrid->put_FixedCols( 1 );
	spGrid->put_FixedRows( 1 );

	LONG lCount = 0;
	
	/// 一つもフィーチャがセットされていないなら戻る
	if( !m_pFeatureDef ) return;

	CRelationalTableDef* pRelTable = m_pFeatureDef->GetRelationalTableDef( m_strTableName );

	if( !pRelTable ) return;

	for( CRelationalTableDef::iterator i = pRelTable->begin(); i != pRelTable->end(); i++, lCount++ ) {
		CString strItem, strItemTmp;
		std::list<LONG> listChanged;
		LONG lFieldCount = 0;
		for( std::list<CString>::iterator itFieldNameList = m_listFieldName.begin(); itFieldNameList != m_listFieldName.end(); itFieldNameList++, lFieldCount++ )
		{
			std::list<CFieldDef>::iterator j;
			for( j = i->begin(); j != i->end(); j++ ) 
			{
				/// 同じフィールドを持つものを探す
				if( 0 == (*itFieldNameList).CompareNoCase( j->m_strFieldName ) )
					break;
			}

			/// 絶対あるはず。なければフィールド名の登録が間違っている
			ATLASSERT( j != i->end() );

			CString str;
			// NULL だった場合の処理			
			if( j->m_vaValue.vt == VT_NULL )
			{
				str = NULL_VALUE;
			} 
			// NULL じゃない場合の処理
			else 
			{
				DOMAINMAP* domain = j->GetDomain();
				if( ! domain ) {
					va2str( j->m_vaValue, str );
				} else {
					for( DOMAINMAP::iterator l = domain->begin(); l != domain->end(); l++ ) {
						if( l->code == j->m_vaValue ) {
							str = l->code_discription;
							break;
						}
					}
				}
			}
			strItemTmp = strItem;
			if( strItemTmp.Compare( _T("") ) == 0 ) {
				strItem.Format(_T("%d\t%s"), lCount, str );
			} else {
				strItem.Format(_T("%s\t%s"), strItemTmp, str );
			}

			/// 編集されているかどうかチェックして、色を変える
			if( j->m_bChanged ) {
				listChanged.push_back( lFieldCount );
				
			}
		}
		spGrid->AddItem( _bstr_t(strItem) );
		/// 色を変えるフィールドを行を追加した後で変更
		for( std::list<LONG>::iterator n = listChanged.begin(); n != listChanged.end(); n++ )
			SetGridColorToRed( spGrid, lCount + 1, (*n) + 1 );
	}
}
void CControlDef::SetFeatureGrid( BOOL bMulti )
{
	/// グリッド取得
	IVSFlexGridPtr spGrid;
	CAxWindow wndGrid( m_hWnd );
	wndGrid.QueryControl(&spGrid);
	LONG lFieldCount = 0;

	// グリッドの行数を取得
	if( m_lTableIndex < 0 && m_lRowIndex < 0 )
	{
		// フィーチャの属性の場合
		lFieldCount = m_pFeatureDef->size();
	} else {
		// 外部テーブルの属性の場合
		std::list<CRelationalTableDef>* pRelTableDefList = m_pFeatureDef->GetRelationalTableDefList();
		std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin();
		lFieldCount = itRelTableDefList->size();
	}

	// スクロールバーの位置を保存するため
	VARIANT_BOOL	bVisible = VARIANT_FALSE;
	BOOL	bBorderFlag = FALSE;
	LONG		lTopRow=0;
	for(LONG i=1; i<lFieldCount+1; i++)
	{
		if( !bBorderFlag )
		{
			spGrid->get_RowIsVisible(i,&bVisible);
			if( bVisible == VARIANT_FALSE)
			{
				bBorderFlag = TRUE;
				continue;
			}
			break;
		}
		else
		{
			spGrid->get_RowIsVisible(i,&bVisible);
			if( bVisible == VARIANT_TRUE)
			{
				lTopRow = i;
				break;
			}
		}
	}
	
	ATLASSERT( spGrid != NULL );

	if( bMulti ) {
		spGrid->put_Cols( 5 );
		spGrid->put_Rows( 1 );
		spGrid->put_FixedCols( 1 );
		spGrid->put_FixedRows( 1 );
	} else {
		spGrid->put_Cols( 2 );
		spGrid->put_Rows( 1 );
		spGrid->put_FixedCols( 1 );
		spGrid->put_FixedRows( 1 );
#ifndef SINDY_FOR_ENGLISH
		spGrid->put_TextMatrix( 0, 0, CComBSTR(_T("属性名称")) );
		spGrid->put_TextMatrix( 0, 1, CComBSTR(_T("属性値")) );
#else
		spGrid->put_TextMatrix( 0, 0, CComBSTR(_T("Field Name")) );
		spGrid->put_TextMatrix( 0, 1, CComBSTR(_T("Value")) );
#endif	// SINDY_FOR_ENGLISH
		spGrid->PutColWidth( 0, 2000 );
	}

	if( !m_pFeatureDef ) return;

	lFieldCount = 0;
	if( m_lTableIndex < 0 && m_lRowIndex < 0 )
	{
		// フィーチャの属性の場合
		for( std::list<CFieldDef>::iterator j = m_pFeatureDef->begin(); j != m_pFeatureDef->end(); j++, lFieldCount++ ) 
		{
			_SetFeatureGrid( &(*j), spGrid, lFieldCount );
		}
	} else {
		// 外部テーブルの属性の場合
		std::list<CRelationalTableDef>* pRelTableDefList = m_pFeatureDef->GetRelationalTableDefList();
		std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin();
		advance( itRelTableDefList, m_lTableIndex );
		std::list<CRowDef>::iterator it = itRelTableDefList->begin();
		advance( it, m_lRowIndex );
		for( std::list<CFieldDef>::iterator itFieldDef = it->begin(); itFieldDef != it->end(); itFieldDef++, lFieldCount++ )
		{
			_SetFeatureGrid( &(*itFieldDef), spGrid, lFieldCount );
		}
	}

	spGrid->put_TopRow(lTopRow);
	// グリッド表示は属性編集できないようにする

	CString strRegOpenKey;
	strRegOpenKey.Format( _T("%s%s%sAttributeTool"), SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );

	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	BOOL bIsGridEditMode = FALSE;
	if ( hOpenKey )
	{
		DWORD dwResult = 0;
		if( EmxGetRegDword( _T("GridEditMode"), &dwResult, hOpenKey ) )
			bIsGridEditMode = (BOOL)dwResult;

		RegCloseKey( hOpenKey );
	}

	if( !bIsGridEditMode )
		spGrid->put_Editable(flexEDNone);

}
void CControlDef::_SetFeatureGrid( CFieldDef* pFieldDef, IVSFlexGridPtr spGrid, LONG lFieldCount )
{
	CString str, strItem;

	// NULL だった場合の処理			
	if( pFieldDef->m_vaValue.vt == VT_NULL )
	{
		str = NULL_VALUE;
	} 
	// NULL じゃない場合の処理
	else 
	{
		DOMAINMAP* domain = pFieldDef->GetDomain();
		if( ! domain )
		{
			// 作業目的名のみルールデータベースから読み込む
			if( 0 == pFieldDef->m_strFieldName.CompareNoCase( PURPOSE_C_FIELD ) )
			{
				CComBSTR bstrName;
				m_ipRule->GetOperatingPurposeFromID( pFieldDef->m_vaValue.lVal, &bstrName );
				str = bstrName;

				// 今なぜか入っている作業目的0において、0を出力するために必要
				if( str.IsEmpty() )
					str.Format(_T("%d"), pFieldDef->m_vaValue.lVal );
			}
			else
				va2str( pFieldDef->m_vaValue, str );
		} 
		else {
			for( DOMAINMAP::iterator l = domain->begin(); l != domain->end(); l++ )
			{
				if( l->code == pFieldDef->m_vaValue )
				{
					str = l->code_discription;
					break;
				}
			}
		}
	}

	if( ! m_bAliasOrField )
		strItem.Format(_T("%s\t%s"), pFieldDef->m_strFieldAliasName, str );
	else
		strItem.Format(_T("%s\t%s"), pFieldDef->m_strFieldName, str );

	spGrid->AddItem( _bstr_t(strItem) );

	/// 編集されているかどうかチェックして、色を変える
	if( pFieldDef->m_bChanged ) {
		SetGridColorToRed( spGrid, lFieldCount + 1, 1 );
				
	}
	/// 編集不可のフィールドなら色かえる
	if( !pFieldDef->IsEditable() ) {
		SetGridColorToUnEdit( spGrid, lFieldCount + 1, 1 );
	}
}
void CControlDef::SetOtherControl()
{

}
/**
 * データベース側で NULL OK なフィールドかどうかのチェック
 *
 * @return Nullable なら TRUE
 */
BOOL CControlDef::IsNullable( LPCTSTR lpcszFieldName )
{
	CFieldDef* pFieldDef = GetFieldDef( lpcszFieldName );

	if( !pFieldDef ) {
		ATLTRACE(_T("CControlDef::IsNullable(): 関連するフィールドがありません ID=%d\n"), m_nID );
		return FALSE;
	}

	return pFieldDef->m_bIsNullable;
}
/**
 * 
 * @return 編集できる場合は TRUE 、編集できないなら FALSE
 */
BOOL CControlDef::Click( LONG l1, LONG l2, BOOL bIsGridEditMode )
{
	/// グリッド取得
	IVSFlexGridPtr spGrid;
	CAxWindow wndGrid( m_hWnd );
	wndGrid.QueryControl(&spGrid);

	ATLASSERT( spGrid != NULL );

	LONG lFieldNum, lFeatNum, lCount;
	DOMAINMAP* domain;
	DOMAINMAP::iterator itDomain;
	std::list<FEATUREDEF>::iterator itFeatureDefList;
	std::list<CFieldDef>::iterator itFieldDefList;
	CString strList;
	
	switch( m_type ) {
	case TYPE_FEATUREGRID:
		lFieldNum = l1 - 1;
		// 現在選択されているフィールドまでシフト
		ATLASSERT( m_pFeatureDef != NULL );
		// 外部テーブルだった場合
		if( m_lTableIndex < 0 && m_lRowIndex < 0 )
		{
			itFieldDefList = m_pFeatureDef->begin();
			advance( itFieldDefList, lFieldNum );
		} else {
			std::list<CRelationalTableDef>* pRelTableDefList = m_pFeatureDef->GetRelationalTableDefList();
			std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin();
			advance( itRelTableDefList, m_lTableIndex );
			std::list<CRowDef>::iterator it = itRelTableDefList->begin();
			advance( it, m_lRowIndex );
			itFieldDefList = it->begin();
			advance( itFieldDefList, lFieldNum );
		}
		
		break;
	case TYPE_FEATURESGRID:
		lFieldNum = l2 - 1;
		lFeatNum = l1 - 1;
		// 現在選択されているレコードまでシフト
		itFeatureDefList = m_pFeatureDefList->begin();
		advance( itFeatureDefList, lFeatNum );
		// 現在選択されているフィールドまでシフト
		std::list<CString>::iterator itFieldName = m_listFieldName.begin();
		advance( itFieldName, lFieldNum );
		for( itFieldDefList = itFeatureDefList->begin(); itFieldDefList != itFeatureDefList->end(); ++itFieldDefList )
		{
			if( 0 == itFieldDefList->m_strFieldName.CompareNoCase( *itFieldName ) ) 
				break;
		}
		break;
	}

	/// 編集できないフィールドなら帰る
	if( !bIsGridEditMode )
	{
		if( !itFieldDefList->IsEditable() )
			return FALSE;
	}
	else
	{
		VARIANT_BOOL vaEditable = VARIANT_FALSE;
		itFieldDefList->m_ipField->get_Editable( &vaEditable );
		if( vaEditable == VARIANT_FALSE )
			return FALSE;
	}

	/// ドメイン取得
	domain = itFieldDefList->GetDomain();

	/// ドメインのない場合
	if( !domain ) 
	{
		CComBSTR bstrCel;

		USES_CONVERSION;

		spGrid->get_Text( &bstrCel );
		if( lstrcmpi( OLE2T(bstrCel), NULL_VALUE ) == 0 )
		{
			CComBSTR bstrText(_T(""));
			spGrid->put_TextMatrix( l1, l2, bstrText.Detach() );
		}

	}
	/// ドメインのある場合
	else 
	{
		for( lCount = 0, itDomain = domain->begin(); itDomain != domain->end(); itDomain++, lCount++ ) {
			if( strList.Compare( _T("") ) != 0 ) {
				strList += _T("|");
			}
			strList += itDomain->code_discription;
		}
		/// Nullable なフィールドの場合はリストに追加
		if( itFieldDefList->m_bIsNullable ) {
			if( strList.Compare( _T("") ) != 0 ) {
				strList += _T("|");
			}
			strList += NULL_VALUE;
		}
		spGrid->put_ComboList( CComBSTR(strList) );
	}
	return TRUE;
}
BOOL CControlDef::IsClickCelEditable( LONG l1, LONG l2 )
{
	/// グリッド取得
	IVSFlexGridPtr spGrid;
	CAxWindow wndGrid( m_hWnd );
	wndGrid.QueryControl(&spGrid);

	ATLASSERT( spGrid != NULL );

	LONG lFieldNum, lFeatNum;
	std::list<FEATUREDEF>::iterator itFeatureDefList;
	std::list<CFieldDef>::iterator itFieldDefList;
	CString strList;
	
	switch( m_type ) {
	case TYPE_FEATUREGRID:
		lFieldNum = l1 - 1;
		// 現在選択されているフィールドまでシフト
		ATLASSERT( m_pFeatureDef != NULL );
		// 外部テーブルだった場合
		if( m_lTableIndex < 0 && m_lRowIndex < 0 )
		{
			itFieldDefList = m_pFeatureDef->begin();
			advance( itFieldDefList, lFieldNum );
		} else {
			std::list<CRelationalTableDef>* pRelTableDefList = m_pFeatureDef->GetRelationalTableDefList();
			std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin();
			advance( itRelTableDefList, m_lTableIndex );
			std::list<CRowDef>::iterator it = itRelTableDefList->begin();
			advance( it, m_lRowIndex );
			itFieldDefList = it->begin();
			advance( itFieldDefList, lFieldNum );
		}
		
		break;
	case TYPE_FEATURESGRID:
		lFieldNum = l2 - 1;
		lFeatNum = l1 - 1;
		// 現在選択されているレコードまでシフト
		itFeatureDefList = m_pFeatureDefList->begin();
		advance( itFeatureDefList, lFeatNum );
		// 現在選択されているフィールドまでシフト
		std::list<CString>::iterator itFieldName = m_listFieldName.begin();
		advance( itFieldName, lFieldNum );
		for( itFieldDefList = itFeatureDefList->begin(); itFieldDefList != itFeatureDefList->end(); ++itFieldDefList )
		{
			if( 0 == itFieldDefList->m_strFieldName.CompareNoCase( *itFieldName ) ) 
				break;
		}
		break;
	}

	VARIANT_BOOL vaEditable = VARIANT_FALSE;
	itFieldDefList->m_ipField->get_Editable( &vaEditable );

	CComBSTR bstrFieldName;
	itFieldDefList->m_ipField->get_Name( &bstrFieldName );
	CString strFieldName( bstrFieldName );
	INT iPos = strFieldName.ReverseFind('.');
	if( iPos > 0 ) strFieldName = strFieldName.Right( strFieldName.GetLength() - iPos - 1 );
	strFieldName.MakeUpper();	// 全て大文字に置換

	// 形状は変更可能となっているので、get_Editableでは判断できないため
	if(      lstrcmp( strFieldName, _T("FID")            ) == 0 ) vaEditable = VARIANT_FALSE; // FID
	else if( lstrcmp( strFieldName, _T("SHAPE")          ) == 0 ) vaEditable = VARIANT_FALSE; // Shape
	else if( lstrcmp( strFieldName, _T("AREA")           ) == 0 ) vaEditable = VARIANT_FALSE; // AREA
	else if( lstrcmp( strFieldName, _T("SHAPE_LENGTH")   ) == 0 ) vaEditable = VARIANT_FALSE; // Shape_length

	return ( vaEditable ) ? TRUE : FALSE;
}

void CControlDef::Edited( LONG l1, LONG l2 )
{
	/// グリッド取得
	IVSFlexGridPtr spGrid;
	CAxWindow wndGrid( m_hWnd );
	wndGrid.QueryControl(&spGrid);

	ATLASSERT( spGrid != NULL );

	LONG lFieldNum, lFeatNum;
	DOMAINMAP* domain;
	DOMAINMAP::iterator itDomain;
	std::list<FEATUREDEF>::iterator itFeatureDefList;
	std::list<CFieldDef>::iterator itFieldDefList;
	CString strList;
	LPCTSTR lpszFieldName = NULL;
	CRelationalTableDef::iterator itRelTable;

	switch( m_type ) {
	case TYPE_FEATUREGRID:
		lFieldNum = l1 - 1;
		// 現在選択されているフィールドまでシフト
		ATLASSERT( m_pFeatureDef != NULL );
		// 外部テーブルだった場合
		if( m_lTableIndex < 0 && m_lRowIndex < 0 )
		{
			itFieldDefList = m_pFeatureDef->begin();
			advance( itFieldDefList, lFieldNum );
		} else {
			std::list<CRelationalTableDef>* pRelTableDefList = m_pFeatureDef->GetRelationalTableDefList();
			std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin();
			advance( itRelTableDefList, m_lTableIndex );
			std::list<CRowDef>::iterator it = itRelTableDefList->begin();
			advance( it, m_lRowIndex );
			itFieldDefList = it->begin();
			advance( itFieldDefList, lFieldNum );
		}
		break;
	case TYPE_FEATURESGRID:
		lFieldNum = l2 - 1;
		lFeatNum = l1 - 1;
		// 現在選択されているレコードまでシフト
		itFeatureDefList = m_pFeatureDefList->begin();
		advance( itFeatureDefList, lFeatNum );
		// 現在選択されているフィールドまでシフト
		std::list<CString>::iterator itFieldName = m_listFieldName.begin();
		advance( itFieldName, lFieldNum );
		for( itFieldDefList = itFeatureDefList->begin(); itFieldDefList != itFeatureDefList->end(); ++itFieldDefList )
		{
			if( 0 == itFieldDefList->m_strFieldName.CompareNoCase( *itFieldName ) )
				break;
		}
		break;
	}


	// ドメイン取得
	domain = itFieldDefList->GetDomain();

	USES_CONVERSION;

	// 選択されたテキストから値を取得
	CComBSTR bstrText;
	spGrid->get_TextMatrix( l1, l2, &bstrText );
	CComVariant vaValue;
	LONG lVarType = -1;
	itFieldDefList->m_ipField->get_VarType( &lVarType );

	vaValue.vt = VT_EMPTY;	// 初期化
	ATLASSERT( itFieldDefList->m_vaValue.vt != VT_EMPTY );	// ほんとはもっとあるけど

	// 値変換
	if( domain )
	{
		CString strText( bstrText );
		if( 0 == strText.CompareNoCase( NULL_VALUE ) )
			vaValue.vt = VT_NULL;
		else {
			for( itDomain = domain->begin(); itDomain != domain->end(); itDomain++ )
			{
				if( 0 == itDomain->code_discription.Compare( strText ) )
				{
					vaValue = itDomain->code;
					break;
				}
			}
			ATLASSERT( vaValue.vt != VT_EMPTY );
		}
	} else {
		CString strText( bstrText );
		if( strText.IsEmpty() )
			vaValue.vt = VT_NULL;
		else {
			vaValue = bstrText;
			vaValue.ChangeType( (USHORT)lVarType );
		}

		if( !itFieldDefList->CheckValue( vaValue, GetParent( m_hWnd ) ) )
		{
			// 表示を元に戻す
			CComVariant vaCelText;
			if( itFieldDefList->m_vaValue.vt == VT_NULL )
				spGrid->put_Text( CComBSTR(NULL_VALUE) );
			else
			{
				vaCelText.ChangeType( VT_BSTR, &(itFieldDefList->m_vaValue) );
				ATLASSERT( vaCelText.vt == VT_BSTR );
				spGrid->put_Text( vaCelText.bstrVal );
			}

			return;
		}

		USES_CONVERSION;

		CComVariant vaCelText;
		switch( lVarType )
		{
			case VT_BSTR:
				if( vaValue.vt == VT_NULL || lstrcmpi( OLE2T(vaValue.bstrVal), _T("") ) == 0 || lstrcmpi( OLE2T(vaValue.bstrVal), NULL_VALUE ) == 0 ) {
					spGrid->put_Text( _bstr_t(NULL_VALUE) );
					vaValue.ChangeType( VT_NULL );
				}
				break;
			case VT_I4:
			case VT_I2:
			case VT_R4:
			case VT_R8:
			{
				CString strText = OLE2T(bstrText);
				if( strText.CompareNoCase( NULL_VALUE ) == 0 ||	strText.CompareNoCase( _T("") ) == 0 ) {
					spGrid->put_Text( _bstr_t(NULL_VALUE) );
					vaValue.vt = VT_NULL;
				} else {
					vaCelText.ChangeType( VT_BSTR, &vaValue );
					spGrid->put_Text( vaCelText.bstrVal );
				}
				break;
			}
			default:
				break;
		}
	}

	/// m_vaValue にセット
	itFieldDefList->m_vaValue = vaValue;
	itFieldDefList->m_bIsUnset = FALSE;		//!< セットフラグ（これがないとStoreできない）
	/// m_vaInitValue と比べて変更されていたら色を変更
	if( itFieldDefList->m_vaValue != itFieldDefList->m_vaInitValue ) {
		SetGridColorToRed( spGrid, l1, l2 );
		/// m_bChanged フラグは？
		itFieldDefList->m_bChanged = TRUE;
	}
	/// 元に戻った場合を考慮して
	else {
		SetGridColorToDefault( spGrid, l1, l2 );
		itFieldDefList->m_bChanged = FALSE;
	}
}
/**
 * LinkTimeReg の内容をグリッドに反映させる
 *
 * @param nID				[in]	選択されているリンクの ObjectID
 */
void CControlDef::SetLinkTimeRegGrid()
{
	LONG lArrayCount = 0;
	IVSFlexGridPtr			spGrid = NULL;

	// グリッドコントロール取得
	CAxWindow wndGrid( m_hWnd );
	wndGrid.QueryControl(&spGrid);

	bool bIsWalkLink = false;
	CString strTblName = m_pFeatureDef->GetTableName();
	if( strTblName.CompareNoCase( _T("WALK_LINK") ) == 0 )
		bIsWalkLink = true;

	// １行目はリソース内でプロパティ設定（ | (Index) | 規制日時 | 曜日 | ）
	long lCols = 3;
	if( m_type == TYPE_OWTIMEREG )	// 一通グリッドだったら正逆カラム分追加
		lCols++;
	else if( bIsWalkLink )			// 歩行者通禁グリッドだったら規制対象カラム分追加
		lCols++;

	spGrid->put_Cols( lCols );

	// 一通グリッドだったら正逆の場合に方向カラムを表示
	if( m_type == TYPE_OWTIMEREG )
	{
		CFieldDef* pFieldDef = m_pFeatureDef->GetFieldDef( m_pFeatureDef->GetTableName(), _T("ONEWAY_C") );
		if( pFieldDef )
		{
			LONG lOneway_c = pFieldDef->m_vaValue.lVal;
			if (lOneway_c == 5){		
				spGrid->PutColWidth(1, 900);
			}else{
				spGrid->PutColWidth(1,   0);
			}
		}
	}

	spGrid->put_Rows( 1 );

	if( !m_pFeatureDef )
	{
		ATLASSERT( m_pFeatureDef != NULL );
		return ;
	}

	CRelationalTableDef* pRelTableDef = m_pFeatureDef->GetRelationalTableDef( m_strTableName );
	
	/// LINK_TIME_REG のテーブルが登録されていなければ戻る
	if( !pRelTableDef )
		return ;

	/// いくつレコードを表示すればいいか
	LONG lRowCount = 0;
	lRowCount = pRelTableDef->size();

	for( LONG i = 0; i < lRowCount; i++ ) {

		CString strDomainName1, strDomainName2, strNopassTarget;
		LONG lSMonth, lSDay, lEMonth, lEDay, lSHour, lSMin, lEHour, lEMin;
		BOOL bChanged1 = FALSE, bChanged2 = FALSE;

		CRelationalTableDef::iterator itRelTable = pRelTableDef->begin();

		advance( itRelTable, i );	//!< 表示するためのレコードまでシフト

		for( std::list<CFieldDef>::iterator itListFieldDef = itRelTable->begin(); itListFieldDef != itRelTable->end(); itListFieldDef++ )
		{
			// 各フィールドを取得し、時間規制を取得
			if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("START_MONTH") ) )
				lSMonth = itListFieldDef->m_vaValue.lVal;
			else if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("START_DAY") ) )
				lSDay   = itListFieldDef->m_vaValue.lVal;
			else if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("END_MONTH") ) )
				lEMonth   = itListFieldDef->m_vaValue.lVal;
			else if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("END_DAY") ) )
				lEDay   = itListFieldDef->m_vaValue.lVal;
			else if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("START_HOUR") ) )
				lSHour   = itListFieldDef->m_vaValue.lVal;
			else if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("START_MIN") ) )
				lSMin   = itListFieldDef->m_vaValue.lVal;
			else if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("END_HOUR") ) )
				lEHour   = itListFieldDef->m_vaValue.lVal;
			else if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("END_MIN") ) )
				lEMin   = itListFieldDef->m_vaValue.lVal;

			/// LINKDIR_C
			if ( (m_type == TYPE_OWTIMEREG) )
			{
				if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("LINKDIR_C") ) )
				{
					DOMAINMAP* domain = itListFieldDef->GetDomain();
					for( DOMAINMAP::iterator itDomain = domain->begin(); itDomain != domain->end(); ++itDomain )
					{
						if( itListFieldDef->m_vaValue.vt == VT_NULL )
						{
							strDomainName1 = NULL_VALUE;	// XXXXX TBD 暫定的にnull
							if( itListFieldDef->m_vaValue != itListFieldDef->m_vaInitValue ) 
								bChanged1 = TRUE;	//!< 変更フラグセット
							break;
						} 
						else if( itDomain->code == itListFieldDef->m_vaValue )
						{
							strDomainName1 = itDomain->code_discription;
							if( itListFieldDef->m_vaValue != itListFieldDef->m_vaInitValue ) 
								bChanged1 = TRUE;	//!< 変更フラグセット
							break;
						}				
					}
				}
			}

			/// DAYOFWEEK_CODE
			if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("DAYOFWEEK_C") ) )
			{
				DOMAINMAP* domain = itListFieldDef->GetDomain();
				for( DOMAINMAP::iterator itDomain = domain->begin(); itDomain != domain->end(); ++itDomain )
				{
					if( itListFieldDef->m_vaValue.vt == VT_NULL )
					{
						strDomainName2 = NULL_VALUE;
						if( itListFieldDef->m_vaValue != itListFieldDef->m_vaInitValue ) 
							bChanged2 = TRUE;	//!< 変更フラグセット
						break;
					} 
					else if( itDomain->code == itListFieldDef->m_vaValue )
					{
						strDomainName2 = itDomain->code_discription;
						if( itListFieldDef->m_vaValue != itListFieldDef->m_vaInitValue )
							bChanged2 = TRUE;	//!< 変更フラグセット
						break;
					}
				}
			}

			// NOPASS_TARGET（歩行者用の規制対象を判別するためのドメイン [bug 5472]）
			if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("NOPASS_TARGET") ) )
			{
				itListFieldDef->dump();
				DOMAINMAP* domain = itListFieldDef->GetDomain();
				for( DOMAINMAP::iterator itDomain = domain->begin(); itDomain != domain->end(); ++itDomain )
				{
					if( itListFieldDef->m_vaValue.vt == VT_NULL )
					{
						strNopassTarget = NULL_VALUE;
						break;
					} 
					else if( itDomain->code == itListFieldDef->m_vaValue )
					{
						strNopassTarget = itDomain->code_discription;
						break;
					}
				}
			}
		}
		CString strPeriod = GetPeriodString(lSMonth, lSDay, lEMonth, lEDay, lSHour, lSMin, lEHour, lEMin);


		CString strGrid;
		// 一通（道路・歩行者）表示用レコード
		if( m_type == TYPE_OWTIMEREG ){
			strGrid.Format(_T("%d\t%s\t%s\t%s"), i, strDomainName1, strPeriod, strDomainName2 );
		// 歩行者通禁表示用
		}else if( bIsWalkLink ){
			strGrid.Format(_T("%d\t%s\t%s\t%s"), i, strPeriod, strDomainName2, strNopassTarget );
		// その他（道路通禁）
		}else{
			strGrid.Format(_T("%d\t%s\t%s"), i, strPeriod, strDomainName2 );
		}

		spGrid->AddItem( _bstr_t(strGrid) );
	}

	// 整形 ＆ 編集不可
	spGrid->AutoSize( 0, spGrid->Cols-1 );
	spGrid->put_Editable( flexEDNone );
}

/**
 * 日にちの表示用文字列を返す
 *
 * 現在のメンバ変数から日にち表示用の文字列を作成し、返します
 *
 * @return 日にちの表示用文字列
 */
CString CControlDef::GetDateString(LONG lSMonth, LONG lSDay, LONG lEMonth, LONG lEDay)
{
	CString strDate;

	if( lSMonth == 1 && lSDay == 1 && lEMonth == 12 && lEDay == 31 ) {
#ifndef SINDY_FOR_ENGLISH
		strDate = _T("通年");	//!< 通年の場合
#else
		strDate = _T("All year");	//!< 通年の場合
#endif	// SINDY_FOR_ENGLISH
	} else if( lSMonth ==0 && lEMonth == 0 ){
#ifndef SINDY_FOR_ENGLISH
		strDate.Format( _T("毎月%s-%s"), l2str(lSDay), l2str(lEDay) ); // 0月 ⇒ 毎月表示( bug[6354] )
#else
		strDate.Format( _T("Every month%s-%s"), l2str(lSDay), l2str(lEDay) ); // 0月 ⇒ 毎月表示( bug[6354] )
#endif	// SINDY_FOR_ENGLISH
	}else{
		strDate.Format(_T("%s/%s-%s/%s"), l2str(lSMonth), l2str(lSDay), l2str(lEMonth), l2str(lEDay) );
	}

	return strDate;
}
/**
 * 時間の表示用文字列を返す
 *
 * 現在のメンバ変数から時間表示用の文字列を作成し、返します
 *
 * @return 時間の表示用文字列
 */
CString CControlDef::GetTimeString(LONG lSHour, LONG lSMin, LONG lEHour, LONG lEMin)
{
	CString strTime;

	strTime.Format(_T("%s:%s-%s:%s"), l2str(lSHour), l2str(lSMin), l2str(lEHour), l2str(lEMin) );

	return strTime;
}

/**
 * 時間規制文字列を作成し、返す
 *
 * グリッドで日時を表示するセルは一つなので、この関数でまとめて文字列を作成します
 *
 * @return 表示用日時文字列
 */
CString CControlDef::GetPeriodString(LONG lSMonth, LONG lSDay, LONG lEMonth, LONG lEDay, LONG lSHour, LONG lSMin, LONG lEHour, LONG lEMin)
{
	CString strPeriod;

	strPeriod.Format(_T("%s  %s"), GetDateString(lSMonth, lSDay, lEMonth, lEDay), GetTimeString(lSHour, lSMin, lEHour, lEMin) );
	return strPeriod;
}
/**
 * 与えられた数値を文字列に変換して返す
 *
 * 変換する際、一桁の数値に関しては二桁にして返します
 *
 * @return 変換後の文字列
 */
CString CControlDef::l2str( LONG l )
{
	CString str;

	if( l == 0 ) {
		str.Format(_T("00"));
	} else {
		str.Format(_T("%d"), l );
	}

	return str;
}

void CControlDef::SetGridColor( IVSFlexGridPtr spGrid, LONG lRow, LONG lCol, LONG r, LONG g, LONG b )
{ 
	spGrid->put_Cell( flexcpBackColor, _variant_t(lRow), _variant_t(lCol), _variant_t(lRow), _variant_t(lCol), _variant_t((long)RGB(r, g, b)) ); 
}
void CControlDef::SetGridColorToRed( IVSFlexGridPtr spGrid, LONG lRow, LONG lCol )
{ 
	SetGridColor( spGrid, lRow, lCol, 255, 168, 126 ); 
}
void CControlDef::SetGridColorToDefault( IVSFlexGridPtr spGrid, LONG lRow, LONG lCol )
{
	SetGridColor( spGrid, lRow, lCol, 0, 0, 0 );
}
void CControlDef::SetGridColorToUnEdit( IVSFlexGridPtr spGrid, LONG lRow, LONG lCol )
{
	SetGridColor( spGrid, lRow, lCol, 200, 250, 200 );
}

void CControlDef::va2str( VARIANT &va, CString& str )
{
	USES_CONVERSION;

	CComVariant vaStr(_T(""));

	switch ( va.vt ) {
		case VT_EMPTY:
			vaStr = EMPTY_VALUE;
			break;
		case VT_NULL:
			vaStr = NULL_VALUE;
			break;
		case VT_BOOL:
			if( va.boolVal == VARIANT_TRUE )
				vaStr = TRUE_VALUE;
			else if( va.boolVal == VARIANT_FALSE )
				vaStr = FALSE_VALUE;
			break;
			case VT_UNKNOWN:
				vaStr = GEOMETRY_VALUE;
			break;
		default:
			vaStr.ChangeType( VT_BSTR, &va );
			break;
	}
	
	str = OLE2T( (BSTR)vaStr.bstrVal );

}
