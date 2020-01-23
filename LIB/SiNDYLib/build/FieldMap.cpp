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

/**
 * @file FieldMap.cpp
 * @brief CFieldMapクラス実装ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "FieldMap.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

// CFieldMapをコピーする
void CopyFieldMap( CFieldMap& dest, const CFieldMap& src )
{
	dest.m_mapFieldIndex = src.m_mapFieldIndex;
	dest.m_vecFieldName = src.m_vecFieldName;
	dest.m_lShapeIndex = src.m_lShapeIndex;
	dest.m_lOIDIndex = src.m_lOIDIndex;
}

sindyErrCode CFieldMap::CreateFieldMap( IFields* ipFields )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	clear();	// 初期化

	if( ipFields != NULL )
	{
		long lFieldCount = 0;
		IFieldPtr ipField;

		ipFields->get_FieldCount( &lFieldCount );
		for( long i = 0; i < lFieldCount; ++i )
		{
			_FIELD stField;
			VARIANT_BOOL vbIsEditable = VARIANT_FALSE;

			ipFields->get_Field( i, &ipField );
			if( ipField )
			{
				CComBSTR bstrName;
				// OBJECTID、SHPAEのフィールド番号を取得
				ipField->get_Type( &stField.type );
				if( esriFieldTypeOID == stField.type )
					m_lOIDIndex = i;
				else if( esriFieldTypeGeometry == stField.type )
					m_lShapeIndex = i;

				// 編集可不可情報を取得
				ipField->get_Editable( &vbIsEditable );
				stField.editable = VB2bool(vbIsEditable);

				// NULL OK情報を取得
				ipField->get_IsNullable( &vbIsEditable );
				stField.nullable = VB2bool(vbIsEditable);

				// フィールド名情報を取得
				ipField->get_Name( &bstrName );
				stField.name = bstrName;
				stField.name.MakeUpper();

				// フィールド名がIDで終わるかどうか
				// ダミーRowを作成する際にNULLセット
				// 対象外にするためにあらかじめ判別
				stField.isid = ( stField.name.Right(2) == _T("ID") );

				// エイリアス名情報を取得
				bstrName.Empty();
				ipField->get_AliasName( &bstrName );
				stField.alias = bstrName;

				// デフォルト値を取得
				ipField->get_DefaultValue( &stField.dvalue );
				// デフォルト値がフィールドタイプと異なる場合があるので、
				// 強制的に変更する
				if( VT_R8 == stField.dvalue.vt && esriFieldTypeInteger == stField.type )
					stField.dvalue.ChangeType(VT_I4);

				// フィールド長情報を取得
				long lLen = 0;
				ipField->get_Length( &lLen );
				stField.length = lLen;

				// 桁数情報を取得
				stField.precision = 0;
				ipField->get_Precision( &stField.precision );

				// 小数点以下桁数情報を取得
				stField.scale = 0;
				ipField->get_Scale( &stField.scale );

				// コード値・レンジドメイン情報を取得
				IDomainPtr ipDomain;
				ipField->get_Domain( &ipDomain );
				ICodedValueDomainPtr ipCDomain( ipDomain );
				IRangeDomainPtr ipRDomain( ipDomain );
				if( ipCDomain )
				{
					long lCodeCount = 0;
					ipCDomain->get_CodeCount( &lCodeCount );
					for( long i = 0; i < lCodeCount; ++i )
					{
						CComBSTR bstrName;
						CComVariant vaValue;
						ipCDomain->get_Name( i, &bstrName );
						ipCDomain->get_Value( i, &vaValue );
						vaValue.ChangeType(VT_I4);
						stField.domain[CString(bstrName)] = vaValue.lVal;
						stField.rdomain[vaValue.lVal] = CString(bstrName);
					}
				}
				else if( ipRDomain )
				{
					ipRDomain->get_MinValue( &stField.range.first );
					ipRDomain->get_MaxValue( &stField.range.second );
				}
			}
			m_mapFieldIndex[stField.name] = i;
			m_vecFieldName.push_back( stField );
		}
	}
	else
		emErr = sindyErr_COMInterfaceIsNull;

	return emErr;
}

// フィールドの付加属性を取得する
const CFieldMap::_FIELD& CFieldMap::GetFieldAttr( long Index ) const
{
	LOGASSERTE_IF( 0 <= Index && (ULONG)Index < GetFieldCount(), sindyErr_ArgLimitOver )
		return m_vecFieldName[Index];

	// 落ちるのを防ぐために、ない場合は一番最後を返す
	return *m_vecFieldName.rbegin();
}

// フィールドインデックスからフィールド名を取得する
LPCTSTR CFieldMap::GetName( long Index ) const
{
	LOGASSERTE_IF( 0 <= Index && (ULONG)Index < GetFieldCount(), sindyErr_ArgLimitOver )
		return m_vecFieldName[Index].name;
	return NULL;
}

// フィールドインデックスからフィールドエイリアス名を取得する
LPCTSTR CFieldMap::GetAliasName( long Index ) const
{
	LOGASSERTE_IF( 0 <= Index && (ULONG)Index < GetFieldCount(), sindyErr_ArgLimitOver )
		return m_vecFieldName[Index].alias;
	return NULL;
}

const VARIANT& CFieldMap::GetDefaultValue( long Index ) const
{
	LOGASSERTE_IF( 0 <= Index && (ULONG)Index < GetFieldCount(), sindyErr_ArgLimitOver )
		return m_vecFieldName[Index].dvalue;
	return vtMissing;
}

void CFieldMap::clear()
{
	m_mapFieldIndex.clear();
	m_vecFieldName.clear();
	m_lOIDIndex = -1;
	m_lShapeIndex = -1;
}

bool CFieldMap::IsEditable( long lIndex ) const
{
	LOGASSERTE_IF( 0 <= lIndex && (ULONG)lIndex < GetFieldCount(), sindyErr_ArgLimitOver )
		return m_vecFieldName[lIndex].editable;

	return false;
}

// フィールドがNULL OKかどうかをチェックする
bool CFieldMap::IsNullable( long lIndex ) const
{
	LOGASSERTE_IF( 0 <= lIndex && (ULONG)lIndex < GetFieldCount(), sindyErr_ArgLimitOver )
		return m_vecFieldName[lIndex].nullable;

	return false;
}

bool CFieldMap::IsSiNDYEditable(long lIndex, bool bForCopy/*=false*/) const
{
	LOGASSERTE_IF( 0 <= lIndex && (ULONG)lIndex < GetFieldCount(), sindyErr_ArgLimitOver )
	{
		if ( IsEditable( lIndex ) ) {
			CString strFieldName = m_vecFieldName[lIndex].name;

			INT iPos = strFieldName.ReverseFind('.');
			if( iPos > 0 ) strFieldName = strFieldName.Right( strFieldName.GetLength() - iPos - 1 );

			bool bRet = true;

			if ( IsShapeField(lIndex) ) bRet = false;
			else if ( IsOIDField(lIndex) ) bRet = false;
			//if(      lstrcmp( strFieldName, _T("FID")            ) == 0 ) bRet = false; // FID
			//else if( lstrcmp( strFieldName, _T("SHAPE")          ) == 0 ) bRet = false; // Shape
			//else if( lstrcmp( strFieldName, _T("AREA")           ) == 0 ) bRet = false; // AREA
			//else if( lstrcmp( strFieldName, _T("SHAPE_LENGTH")   ) == 0 ) bRet = false; // Shape_length
			//else if( lstrcmp( strFieldName, _T("OBJECTID")       ) == 0 ) bRet = false; // OBJECTID
			else if( lstrcmp( strFieldName, _T("ENABLED")        ) == 0 ) bRet = false; // Enabled
			else if( lstrcmp( strFieldName, schema::ipc_table::kOperator ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, schema::ipc_table::kPurpose ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, schema::ipc_table::kModifyDate ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, schema::ipc_table::kUpdateType ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, schema::ipc_table::kProgModifyDate ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, schema::ipc_table::kModifyProgName ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, schema::ipc_table::kUserClaim ) == 0 ) bRet = false;
			else if( ( ! bForCopy ) && lstrcmp( strFieldName, _T("FIELDSURVEY_F")  ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, _T("FROM_NODE_ID")   ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, _T("TO_NODE_ID")     ) == 0 ) bRet = false;
			else if( strFieldName.Right( 3 ).CompareNoCase( _T("_RF")   ) == 0 ) bRet = false;
			else if( strFieldName.Right( 5 ).CompareNoCase( _T("_LQRF") ) == 0 ) bRet = false;
			else if( strFieldName.Left(  3 ).CompareNoCase( _T("TMP")   ) == 0 ) bRet = false;
			else if( lstrcmp( strFieldName, _T("NODECLASS_C") ) == 0 ) bRet = false;

			return bRet;
		}
	}

	return false;
}

// フィールドタイプを取得する
esriFieldType CFieldMap::GetFieldType( long lIndex ) const
{
	LOGASSERTE_IF( 0 <= lIndex && (ULONG)lIndex < GetFieldCount(), sindyErr_ArgLimitOver )
		return m_vecFieldName[lIndex].type;

	return (esriFieldType)-1;
}

// コード値ドメイン対応表を取得する
const std::map<CString,long>& CFieldMap::GetDomain( LPCTSTR lpcszFieldName ) const
{
	const_iterator it = begin();
	std::advance( it, FindField( lpcszFieldName ) );
	return it->domain;
}

// フィールド名からフィールドインデックス番号を取得する
long CFieldMap::_FindField( LPCTSTR lpcszFieldName, bool bDebugTrace ) const
{
	CString strFieldName( lpcszFieldName );	// 大文字に変換する
	strFieldName.MakeUpper();

	std::map<CString,long>::const_iterator it = m_mapFieldIndex.find( strFieldName );
	if( it != m_mapFieldIndex.end() )
		return it->second;
	else if( bDebugTrace )
	{
#ifdef DEBUG
		_ASSERTE( it != m_mapFieldIndex.end() );
		// 対応表の中身を表示
		TRACEMESSAGE( _T("%s%s\n"), _T("DEBUG: CFieldMap インデックス番号取得エラー："), strFieldName );
		TRACEMESSAGE( _T("%s\n"), _T("DEBUG: ------------------------------------") );
		for( it = m_mapFieldIndex.begin(); it != m_mapFieldIndex.end(); ++it )
			TRACEMESSAGE( _T("DEBUG: [%s]=[%d]\n"), it->first, it->second );
#endif // DEBUG			
	}
	return -1;
}

} // sindy
