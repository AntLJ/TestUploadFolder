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

#include "stdafx.h"
#include "RuleWrapper.h"
#include "type_convert.h"
#include "RowBase.h"
#include "GeometryBase.h"
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

using namespace sindy;

extern "C" const GUID __declspec(selectany) CLSID_SiNDYRuleObj =             {0x642db0f6,0x97cb,0x488d,{0xad,0x99,0xbb,0xd9,0xab,0xda,0xc2,0x47}};
extern "C" const GUID __declspec(selectany) CLSID_SiNDYModifyCheckObj =      {0x67dc3e12,0x061b,0x4ca8,{0xa8,0x29,0x40,0xc6,0x8d,0x7d,0x17,0x7a}};

#ifndef SINDY_FOR_ENGLISH
const static _TCHAR CANNOT_CREATE_LAYER[] = _T("作成不可能なレイヤです：%s");
const static _TCHAR THIS_IS_LOCKMESH[]    = _T("メッシュロックエリアです：%s");
const static _TCHAR NOT_EDITABLE_AREA[]   = _T("編集可能エリアではありません：%s");
#else
const static _TCHAR CANNOT_CREATE_LAYER[] = _T("Layer that cannot be created: %s");
const static _TCHAR THIS_IS_LOCKMESH[]    = _T("This area is locked.：%s");
const static _TCHAR NOT_EDITABLE_AREA[]   = _T("This is not an area that can be edited.：%s");
#endif	// SINDY_FOR_ENGLISH


// ルールオブジェクトにログイン情報がセットされているかどうか
bool CRuleWrapper::IsLogined() const
{
	VARIANT_BOOL is_logined = VARIANT_FALSE;
	ISiNDYRule4Ptr ipRule( GetRule() );
	if( ipRule )
		ipRule->IsLogined( &is_logined );

	return VB2bool(is_logined);
}

// 他に編集しているユーザがいないか確認する
bool CRuleWrapper::IsModified( const CRowBase& cRow, CString& strMsg ) const
{
	ISiNDYModifyCheckPtr ipMod( GetModify() );
	if( ! ipMod )
		return false;

	VARIANT_BOOL vbIsModified = VARIANT_FALSE;
	CComBSTR bstrModMsg;
	ipMod->IsModified( (_IRow*)cRow, &vbIsModified, &bstrModMsg );
	if( vbIsModified )
	{
		strMsg = bstrModMsg;
		return true;
	}
	return false;
}

// 編集可能かどうかを確認する
sindyeEditableType CRuleWrapper::IsEditable( const CRowBase& cRow, sindyeEditType emEditType, CString& strMsg, bool check_modify/* = true*/, bool err2warn_modify/* = false*/  ) const
{
	sindyeEditableType emRetType = sindyeEditableNoDefine;
	CComBSTR bstrMsg;

	// モディファイチェック
	if( check_modify )
	{
		if( IsModified( cRow, strMsg ) )
			return err2warn_modify ? sindyeEditableWarnning : sindyeEditableNoEditable;
	}

	// 編集ルール確認
	if( sindyeEditableNoDefine == emRetType )
	{
		ISiNDYRule4Ptr ipRule( GetRule() );
		if( ipRule && IsLogined() )
		{
			IGeometryPtr ipGeom( cRow.GetShapeCopy() );
			// 新規ものの場合は、バラでチェックする（bug 9745）
			if( cRow.IsCreated() )
			{
				emRetType = sindyeEditableEditable;

				// 作成可能かどうか
				VARIANT_BOOL vbIsCreatable = VARIANT_FALSE;
				ipRule->IsCreatableLayer( (CComBSTR)cRow.GetTableName(), &vbIsCreatable );
				if( ! vbIsCreatable )
				{
					emRetType = sindyeEditableNoEditable;
					strMsg.Format( CANNOT_CREATE_LAYER, cRow.GetTableName() );
				}

				// ロックエリアかどうか
				if( sindyeEditableEditable == emRetType && IsLockArea( cRow.GetOwnerName(), cRow.GetTableName(), ipGeom ) )
				{
					emRetType = sindyeEditableNoEditable;
					strMsg.Format( THIS_IS_LOCKMESH, cRow.GetTableName() );
				}

				// 編集エリアかどうか
				if( sindyeEditableEditable == emRetType && !IsEditableArea( cRow.GetTableName(), ipGeom ) )
				{
					emRetType = sindyeEditableNoEditable;
					strMsg.Format( NOT_EDITABLE_AREA, cRow.GetTableName() );
				}
			}
			else
			{
				// CRowBase::GetShape()はm_pGeomがNULLだった場合に(IGeometry*)にキャストできないのでGetShapeCopy()を使う（bug 9750）
				ipRule->IsEditableFeature( (_IRow*)cRow, ipGeom, emEditType, &emRetType, &bstrMsg );
				strMsg = bstrMsg;
			}
		}
	}

	return emRetType;
}

// ロックエリアかどうか確認する
bool CRuleWrapper::IsLockArea( const CString& strOwner, const CString& strTable, IGeometryPtr& ipGeom ) const
{
	ISiNDYRule4Ptr ipRule( GetRule() );
	if( ! ipRule )
		return false;

	VARIANT_BOOL vbIsLockArea = VARIANT_FALSE;
	ipRule->IsLockArea( CComBSTR(strOwner), CComBSTR(strTable), ipGeom, &vbIsLockArea );

	return VB2bool(vbIsLockArea);
}

// 編集可能エリアかどうか確認する
bool CRuleWrapper::IsEditableArea( const CString& strTable, IGeometryPtr& ipGeom ) const
{
	ISiNDYRule4Ptr ipRule( GetRule() );
	if( ! ipRule )
		return true;

	VARIANT_BOOL vbIsEditArea = VARIANT_TRUE;
	ipRule->IsEditArea( CComBSTR(strTable), ipGeom, &vbIsEditArea );

	return VB2bool(vbIsEditArea);
}

// 作業目的IDを取得する
long CRuleWrapper::GetWorkPurposeID() const
{
	long ret = -1; // 戻り値
	CComBSTR purpose;
	ISiNDYRule4Ptr ipRule( GetRule() );
	if( ipRule && IsLogined() )
		ipRule->GetOperatingPurpose( &ret, &purpose );

	return ret;
}

// ログイン名を取得する
CString CRuleWrapper::GetOperatorName() const
{
	CComBSTR bstrName;
	ISiNDYRule4Ptr ipRule( GetRule() );
	if( ipRule && IsLogined() )
		ipRule->get_OperatorName( &bstrName );

	return CString(bstrName);
}

// 新規作成時の自動付与属性を取得する
const CAutoValue& CRuleWrapper::GetCreatedAutoValue( const CString& strTable ) const
{
	// キャッシュがあるならキャッシュを探す
	if( ! m_mapAutoValue.empty() )
		return m_mapAutoValue[strTable];

	// キャッシュがないなら、キャッシュを作成
	static CAutoValue dummy;
	ISiNDYRule4Ptr ipRule( GetRule() );
	if( ! ( ipRule && IsLogined() ) )
		return dummy; // ログインするまでは、コンテナは空にしておく

	m_mapAutoValue[_T("dummy")] = CAutoValue(); // 二重初期化防止

	IEnumAutoValueLayerPtr ipEnum;
	ipRule->get_EnumAutoValueLayer( &ipEnum );
	if( ! ipEnum )
		return dummy;

	IEnumAutoValuePtr ipAutoValues;
	ipEnum->Reset();
	while( S_OK == ipEnum->Next( &ipAutoValues ) && ipAutoValues )
	{
		CComBSTR bstrName;
		ipAutoValues->get_Name( &bstrName );
		CAutoValue& listAutoValue = m_mapAutoValue[CString(bstrName)];
		ipAutoValues->Reset();
		BSTR bstrCol = NULL, bstrVal = NULL;
		while( S_OK == ipAutoValues->Next( &bstrCol, &bstrVal ) )
		{
			listAutoValue.push_back( std::make_pair(CString(bstrCol), CString(bstrVal)) );
			::SysFreeString(bstrCol); // CComBSTRの使い回しはリークのもとなので、生BSTRを使用する
			::SysFreeString(bstrVal);
		}
	}
	return m_mapAutoValue[strTable];
}

// 自動属性付与の値を取得する（bug 9759）
const CAutoValue& CRuleWrapper::GetAutoValue() const
{
	// キャッシュがあるならキャッシュを探す
	if( ! m_cAutoValue.empty() )
		return m_cAutoValue;

	// キャッシュがないなら、キャッシュを作成
	static CAutoValue dummy;
	ISiNDYRule4Ptr ipRule( GetRule() );
	if( ! ( ipRule && IsLogined() ) )
		return dummy; // ログインするまでは、コンテナは空にしておく

	m_cAutoValue.push_back( std::make_pair( _T(""), _T("") ) ); // 二重初期化防止

	// 自動属性付与を取得
	CComBSTR bstrAutoAttrGiving;
	ipRule->GetAutoAttrGiving( &bstrAutoAttrGiving );

	// tstringに空文字を突っ込むと落ちる（bug 9755）
	if( !bstrAutoAttrGiving )
		return m_cAutoValue;

	// field_name=value|… フォーマット
	uh::tstring av( bstrAutoAttrGiving );
	std::list<uh::tstring> key_vals;
	get_separated_values( av, uh::tstring(_T("|")), key_vals );
	BOOST_FOREACH( const uh::tstring& key_val, key_vals )
	{
		std::list<uh::tstring> vals;
		get_separated_values( key_val, uh::tstring(_T("=")), vals );
		if( 2 != vals.size() )
			continue;

		std::list<uh::tstring>::const_iterator it1 = vals.begin();
		std::list<uh::tstring>::const_iterator it2 = vals.begin(); ++it2;
		m_cAutoValue.push_back( std::make_pair( it1->c_str(), it2->c_str() ) );
	}

	// 値があればダミーを消しておく
	if( 1 < m_cAutoValue.size() )
		m_cAutoValue.pop_front();

	return m_cAutoValue;
}

// ルールオブジェクトを取得する
ISiNDYRule4Ptr CRuleWrapper::GetRule() const
{
	if( ! m_ipRule )
		m_ipRule.CreateInstance( CLSID_SiNDYRuleObj ); // シングルトンなので、ここで作って差支えない

	return m_ipRule;
}

// モディファイチェックオブジェクトを取得する
ISiNDYModifyCheckPtr CRuleWrapper::GetModify() const
{
	if( ! m_ipModify )
		m_ipModify.CreateInstance( CLSID_SiNDYModifyCheckObj ); // シングルトンなので、ここで作って差支えない

	return m_ipModify;
}
