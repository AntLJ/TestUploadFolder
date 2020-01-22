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

/*! \file FieldDef.cpp
	\brief CFieldDef クラスのインプリメンテーション
	\author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
	\date 2002-03-05
*/

#include "stdafx.h"
#include "../FieldDef.h"

namespace {
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR  ERROR_ENTER_NUMBER  = _T("Please enter a number");
	LPCTSTR  ERROR_INPUT_VALUE   = _T("Input value error");
	LPCTSTR  ERROR_OUT_OF_LIMITS = _T("The value is out of limits: %d Please enter a value between %g - %g ");
#else
	LPCTSTR  ERROR_ENTER_NUMBER  = _T("数値を入力してください");
	LPCTSTR  ERROR_INPUT_VALUE   = _T("入力値エラー");
	LPCTSTR  ERROR_OUT_OF_LIMITS = _T("制限範囲外の値です : %d   %g - %g の間で入力してください");
#endif

	/**
	 * @brief Z値付きのポイントかどうかを判定する
	 * @param [in] ipFeatureClass フィーチャクラス
	 * @retval true フィーチャクラスはZ値付きのポイント
	 * @retval false フィーチャクラスはZ値付きのポイントではない
	 */
	bool IsPointWithZ(IGeometryPtr ipGeometry)
	{
		// ジオメトリが有効かつZ値持ちでなければfalseを返す
		if(!(ipGeometry && AheGeoHasZ(ipGeometry)))
			return false;

		// フィーチャクラスのジオメトリタイプを取得する。POINT型かどうかを判断基準とする
		esriGeometryType type = esriGeometryNull;
		ipGeometry->get_GeometryType(&type);
		
		return type == esriGeometryPoint;
	}
}

namespace virtual_field {
	const _TCHAR kShapeZ[] = _T("SHAPE.Z");
}

/**
	コンストラクタ・デストラクタ
*/
/**
 * コンストラクタ
 */
CFieldDef::CFieldDef()
{
	Clear();
}
/**
 * デストラクタ
 */
CFieldDef::~CFieldDef()
{
	/**
	 * m_mapDomainを空にします
	 */
//	m_mapDomain.empty();
	m_mapDomain.clear();
}
/**
 * メンバ変数を全て初期状態に戻す
 */
void CFieldDef::Clear()
{
	m_ipRowBuffer= NULL;
	m_ipField = NULL;
	m_mapDomain.clear();

	m_vaInitValue = -2L;
	m_vaValue = -2L;

	m_lFieldIndex = -1;
	m_bChanged = FALSE;
	m_bIsInited = FALSE;
	m_bIsNullable = FALSE;
	m_bIsUnset = m_bIsUnsetOld = TRUE;
	m_bIsNotDomain = FALSE;

	// レンジドメイン用の構造体の初期化
	m_strctRangeDomain.dMax = 0.0;
	m_strctRangeDomain.dMin = 0.0;
	m_strctRangeDomain.bExist = TRUE;
	m_strctRangeDomain.bInited = FALSE;
}
/**
 * メンバ変数をフィールド値の変更がかかる前に戻す
 */
void CFieldDef::Reset()
{
	m_bChanged = FALSE;
	m_vaValue = m_vaInitValue;
	m_bIsUnset = m_bIsUnsetOld;	//!< セットされたかどうかのフラグも戻す必要あり（Storeの関係）
}
/**
 * フィールド値の変更をデータベースに書き込む
 *
 * @return 値が更新されたら TRUE 、更新されなかったら FALSE を返す
 *
 * フィールド値に変更が合った場合（m_bChanged が TRUE の場合）に IRowBuffer へ m_vaValue を適用します。
 * \note 履歴の書き込みはここでは行いません。CFeatureDef::Store() を参照してください。 
 */
BOOL CFieldDef::Store()
{
	IUnknownPtr ipUnk = NULL;
	BOOL bStored = FALSE;

	if( !m_bChanged )
		return bStored;

	/// 使ってないし、レコードもない場合は飛ばす
	if( m_bIsUnset && !m_ipRowBuffer )
		return bStored;

	// フィールドインデックスが0以上なら、値をセット
	if(m_ipField)
	{
		m_ipRowBuffer->put_Value( m_lFieldIndex, m_vaValue );
	}
	else if(m_strFieldName.CompareNoCase(virtual_field::kShapeZ) == 0)
	{
		// インデックスが負の値でフィールド名がZ値の仮想フィールドを指していた場合はZ値をセット
		IGeometryPtr ipGeometry;
		IFeaturePtr(m_ipRowBuffer)->get_Shape(&ipGeometry);
		if(!IsPointWithZ(ipGeometry))
			return bStored;

		IPointPtr(ipGeometry)->put_Z(m_vaValue.dblVal);
		IFeaturePtr(m_ipRowBuffer)->putref_Shape(ipGeometry);
	}
				
	ATLTRACE(_T("CFieldDef::Store() : 値を更新します：%s::%s\n"), m_strTableName, m_strFieldName );
	bStored = TRUE;	//!< 更新した

	m_bChanged = FALSE;	//!< 更新フラグを戻す

	return bStored;
}
/**
 * テーブル名とフィールド名をセットする
 *
 * @param lpszTableName	[in]	テーブル名
 * @param lpszFieldName	[in]	フィールド名
 *
 * \note メンバ変数の m_lpszTableName と m_lpszFieldName にコピーするだけです
 */
void CFieldDef::SetFieldDef( LPCTSTR lpszTableName,	LPCTSTR lpszFieldName )
{
	m_strFieldName = lpszFieldName;
	m_strTableName = lpszTableName;
}
/**
 * CFieldDef が実際に使用できるように設定する
 *
 * @param lFieldIndex	[in]	2番目の引数の IField インターフェースオブジェクトのインデックス番号
 * @param ipField		[in]	対応する IField インターフェースオブジェクト
 * @param ipRowBuffer	[in]	対応する IRowBuffer インターフェースオブジェクト（実際に値をやり取りするのはこっち）
 *
 * \note この関数でセットしないと実際に使用できるようにはならない（外部テーブルレコードの新規作成の場合を除く）
 */
void CFieldDef::SetRowBuffer( LONG lFieldIndex, IFieldPtr ipField, IRowBufferPtr ipRowBuffer )
{
	m_lFieldIndex = lFieldIndex;
	m_ipField = ipField;

	/// ipSet が NULL ならフィールドが存在していないのでフラグを立てる
	if( !ipRowBuffer ) {
		m_bIsUnset = m_bIsUnsetOld = TRUE;

		return;
	} else {
		m_ipRowBuffer = ipRowBuffer;
		m_bIsUnset = m_bIsUnsetOld = FALSE;
	}

	// フィールドの実体があるなら、値を取得して初期化
	if(m_ipField)
	{
		ipRowBuffer->get_Value( m_lFieldIndex, &m_vaInitValue );
		m_vaValue = m_vaInitValue;
	}
	else if(m_strFieldName.CompareNoCase(virtual_field::kShapeZ)==0)
	{
		// フィールドの実体がなく、名前がZ値の仮想フィールドを指していた場合はZ値を取得して値を設定
		IGeometryPtr ipGeometry;
		IFeaturePtr(ipRowBuffer)->get_Shape(&ipGeometry);
		double zVal = 0;
		if(IsPointWithZ(ipGeometry) && S_OK == IPointPtr(ipGeometry)->get_Z(&zVal))
		{
			m_vaValue.vt = VT_R8;
			m_vaValue.dblVal = zVal;
		}
		else
		{
			m_vaValue.vt = VT_NULL;
		}
		m_bIsInited = TRUE;
		return;
	}
			
	/// NULL OK なフィールドかどうか
	VARIANT_BOOL vaIsNullable = VARIANT_FALSE;
	m_ipField->get_IsNullable( &vaIsNullable );

	m_bIsNullable = ( vaIsNullable == VARIANT_TRUE ) ? TRUE : FALSE;	//!< NULL OK かどうかのフラグを立てる

	// フィールドのエイリアス名を取得
	CComBSTR bstrAliasName;
	m_ipField->get_AliasName( &bstrAliasName );
	m_strFieldAliasName = bstrAliasName;

	// DOMAINMAPを後で作ると作成側と開放側のスレッドが異なるために落ちるので
	// ここで作っちゃう
	GetDomain();

	m_bIsInited = TRUE;	//!< 初期化完了フラグ
}
/**
 * 対応するフィールドのドメインを取得する
 *
 *  データベースに問い合わせするのは一度限り（一度読み込んだら内部で保存）
 * 
 * @return 成功した場合は DOMAINMAP がかえり、失敗した場合、または一度検索した結果ドメインが存在しなかった場合は NULL が返る
 */
DOMAINMAP* CFieldDef::GetDomain()
{
	// フィールドが存在しない場合はNULL(Z値の仮想フィールドかもしれないので) 2017/2/17 ta_suga
	if (!m_ipField)
		return NULL;

	IDomainPtr ipDomain = NULL;
	ICodedValueDomainPtr ipCodedValueDomain = NULL;
	BSTR bstrName;
	CComVariant vaValue;
	LONG lCount = 0;

	USES_CONVERSION;
	
	/// 以前に既に調査済み且つドメインが存在しなかった場合
	if( m_bIsNotDomain ) 
		return NULL;

	else if( m_mapDomain.size() == 0  ) 
	{
		m_ipField->get_Domain( &ipDomain );
		if( !ipDomain )
			return NULL;
		ipCodedValueDomain = ipDomain;	//!< 固定値ドメイン
		if( !ipCodedValueDomain )
			return NULL;

		ipCodedValueDomain->get_CodeCount( &lCount );
		for( LONG i = 0; i < lCount; i++ ) {
			ipCodedValueDomain->get_Name( i, &bstrName );
			CString strName = bstrName;
			::SysFreeString( bstrName );
			ipCodedValueDomain->get_Value( i, &vaValue );

			CODEVALUE_DOMAIN domain;
			domain.idx = i;
			domain.code = vaValue;
			domain.code_discription = strName;
			m_mapDomain.insert(domain);
		}
	}

	return &m_mapDomain;
}
/**
 * 対応するフィールドのレンジドメインを取得する
 *
 *  データベースに問い合わせするのは一度限り（一度読み込んだら内部で保存）
 * 
 * @return 成功した場合は DOMAINMAP がかえり、失敗した場合、または一度検索した結果ドメインが存在しなかった場合は NULL が返る
 */
RANGEDOMAIN* CFieldDef::GetRangeDomain()
{
	// フィールドが存在しない場合はNULL(Z値の仮想フィールドかもしれないので) 2017/2/17 ta_suga
	if(!m_ipField)
		return NULL;

	IDomainPtr ipDomain = NULL;
	IRangeDomainPtr ipRangeDomain = NULL;
	CComVariant vaValue;
	LONG lCount = 0;

	USES_CONVERSION;
	
	/// 以前に既に調査済みの場合
	if( m_strctRangeDomain.bInited ) 
	{
		if( m_strctRangeDomain.bExist )	
			return &m_strctRangeDomain;	// レンジドメインが存在しているならメンバを返す
		else 
			return NULL;			// レンジドメインが存在していないなら NULL を返す
	}	

	// 2度検索させない
	m_strctRangeDomain.bInited = TRUE;

	m_ipField->get_Domain( &ipDomain );

	if( !ipDomain ) 
	{
		m_strctRangeDomain.bExist = FALSE;
		return NULL;
	}
	
	ipRangeDomain = ipDomain;	// レンジドメイン
	if( !ipRangeDomain )
	{
		m_strctRangeDomain.bExist = FALSE;
		return NULL;
	}

	// 最大値の取得
	ipRangeDomain->get_MaxValue( &vaValue );
	vaValue.ChangeType( VT_R8 );	// 問答無用で DOUBLE に変換（大丈夫？）
	m_strctRangeDomain.dMax = vaValue.dblVal;
	// 最小値の取得
	ipRangeDomain->get_MinValue( &vaValue );
	vaValue.ChangeType( VT_R8 );
	m_strctRangeDomain.dMin = vaValue.dblVal;

	m_strctRangeDomain.bExist = TRUE;	// レンジドメインがあった場合はフラグを立てる

	return &m_strctRangeDomain;
}

CComVariant CFieldDef::GetCodeDomainValue(long idx)
{
	auto iter = find_if(m_mapDomain.begin(), m_mapDomain.end(), 
		[&idx](const CODEVALUE_DOMAIN& domain) 
	{
		return domain.idx == idx;
	});

	if (iter == m_mapDomain.end())
		return CComVariant();

	return iter->code;
}

VARTYPE CFieldDef::GetCodedDomainType()
{
	return m_mapDomain.empty() ? VT_NULL : m_mapDomain.begin()->code.vt;
}

/**
 * 編集できるフィールドかどうかの判定を行う
 *
 * @return 編集できるフィールドなら TRUE、編集できないなら FALSE を返す
 *
 * \note この関数では本来編集できないフィールドのほかに、SiNDY-e 的にユーザに変更して欲しくないフィールドも含んでいます。
 * \note 今後も増えることが予想されるので、その場合は適宜追加する必要があります。
 */
BOOL CFieldDef::IsEditable()
{
	return AheIsEditableField( m_ipField );
}
/**
 * デバッグモード時、メンバ変数をアウトプットウィンドウにダンプする
 */
void CFieldDef::dump()
{
#ifdef _DEBUG	
	CComVariant vaValue, vaInitValue;
	vaValue.ChangeType( VT_BSTR, &m_vaValue );
	vaInitValue.ChangeType( VT_BSTR, &m_vaInitValue );

	ATLTRACE(_T("TABLE:%s FIELD:%s UNSET:%s UNSETOLD:%s NULLABLE:%s INITED:%s CHANGED:%s VALUE:%s INIVALUE:%s HASDOMAIN:%s RELTYPE:%d INDEX:%d\n"),
		m_strTableName, m_strFieldName, m_bIsUnset ? _T("TRUE") : _T("FALSE"), m_bIsUnsetOld ? _T("TRUE") : _T("FALSE"),
		m_bIsNullable ? _T("TRUE") : _T("FALSE"), m_bIsInited ? _T("TRUE") : _T("FALSE"), m_bChanged ? _T("TRUE") : _T("FALSE"),
		COLE2CT(vaValue.bstrVal), COLE2CT(vaInitValue.bstrVal), m_bIsNotDomain ? _T("FALSE") : _T("TRUE"), m_nRelType, m_lFieldIndex );
#endif
}
/**
 * m_vaValue又はm_vaInitValueの値を文字列に変換する
 *
 * @param bIsInitValue	[in]	TRUE の場合は m_vaInitValue、FALSE の場合は m_vaValue が変換対象になる
 *
 * @return 変換後の値が文字列として帰る
 *
 * \note NULL、や空などの特殊な場合を考慮してそれにあった文字列を返します。
 */
CString CFieldDef::value2string( BOOL bIsInitValue )
{
	CComVariant vaValue(_T("")), vaTarget;
	CString strRet;

	if( bIsInitValue ) 
		vaTarget = m_vaInitValue;
	else 
		vaTarget = m_vaValue;

	switch( vaTarget.vt )
	{
		case VT_NULL:
			strRet = _T("");
			break;
		case VT_EMPTY:
			strRet = EMPTY_VALUE;
			break;
		case VT_UNKNOWN:
			strRet = GEOMETRY_VALUE;
			break;
		default:
			vaValue.ChangeType( VT_BSTR, &vaTarget );
			strRet.Format( _T("%s"), COLE2T(vaValue.bstrVal));
			break;
	}
	
	return strRet;
}
/**
 * @brief 値が適用可能かどうかチェックする
 *
 * @note レンジドメインのみチェックしています
 *
 * @param vaValue		[in]	チェックする値
 *
 * @retval TRUE	適用可能
 * @retval FALSE 適用不可能
 */
BOOL CFieldDef::CheckValue( const CComVariant& vaValue, HWND hWnd )
{
	HWND hArcWnd = NULL;

	// NULL OK じゃないのに VT_NULL ならエラー
	if( vaValue.vt == VT_NULL )
	{
		if( ! m_bIsNullable )
			return FALSE;
	}
	else {
		// レンジドメインを考慮
		RANGEDOMAIN* pDomain = GetRangeDomain();
		if( pDomain && pDomain->bExist )
		{
			if( vaValue.vt != VT_I4 )
			{
				AheMessageBox( hWnd, ERROR_ENTER_NUMBER, ERROR_INPUT_VALUE, MB_OK|MB_ICONWARNING|MB_TASKMODAL, WARNLEVEL_HIGH );
				return FALSE;
			}
			else {
				if( pDomain->dMax < (DOUBLE)vaValue.lVal || pDomain->dMin > (DOUBLE)vaValue.lVal )
				{
					CString strMsg;
					strMsg.Format(ERROR_OUT_OF_LIMITS, vaValue.lVal, pDomain->dMin, pDomain->dMax );
					AheMessageBox( NULL, strMsg, ERROR_INPUT_VALUE, MB_OK|MB_ICONWARNING|MB_TASKMODAL );
		
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

BOOL CFieldDef::SetNewValue( CComVariant vaNewValue, HWND hWnd )
{
	if( ! CheckValue( vaNewValue, hWnd ) )
		return FALSE;
	else if( vaNewValue != m_vaValue )
	{
		m_vaValue = vaNewValue;

		// 初めてセットされた場合
		// こうしないと CFieldDef::Store() 時にはじかれる
		if( m_bIsUnset ) 
			m_bIsUnset = FALSE;	

		if( m_vaValue == m_vaInitValue )
		{
			m_bChanged = FALSE;
			ATLTRACE(_T("CFieldDef::SetNewValue() : 値が初期状態に戻りました %s\n"), m_strFieldName );
		} else {
			m_bChanged = TRUE;
			ATLTRACE(_T("CFieldDef::SetNewValue() : 値が変更されました %s\n"), m_strFieldName );
		}
	}

	return TRUE;
}
