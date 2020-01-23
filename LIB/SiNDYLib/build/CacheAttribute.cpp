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
 * @file CacheAttribute.cpp
 * @brief <b>CCacheAttributeクラス実装ファイル</b>
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "CacheAttribute.h"
#include "FieldMap.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _ARC931_BUG_FIXED
#ifndef _ARC931_BUG_FIXED
void FixBSTR( VARIANT& va )
{
	if( va.vt != VT_BSTR )
		return;
	// 波ダッシュ -> チルダ変換
	UINT len = ::SysStringLen(va.bstrVal);
	for( UINT i = 0; i < len; ++i )
	{
		if( va.bstrVal[i] == 0x301C )
			va.bstrVal[i] = 0xFF5E;
	}
}
#else
#define FixBSTR (void*)0;
#endif // ifndef _ARC931_BUG_FIXED

HRESULT Fix_BSTR_get_Value( _IRow* p, long index, VARIANT* va )
{
	if( ! p )
	{
		_ASSERTE( p );
		return E_FAIL;
	}
	if( ! va )
	{
		_ASSERTE( va );
		return E_FAIL;
	}
	if( 0 > index )
	{
		_ASSERTE( 0 <= index );
		return E_FAIL;
	}
	HRESULT hr = p->get_Value( index, va );
	FixBSTR(*va);
	return hr;
}

namespace sindy {
using namespace errorcode;

CCacheAttribute::CCacheAttribute( const CCacheAttribute& lp )
{
	m_pVars = NULL;
	*this = lp;
}

CCacheAttribute& CCacheAttribute::operator=( const CCacheAttribute& lp )
{
	Init();
	CopyCacheAttribute( *this, lp );
	return *this;
}

// メンバ変数を初期化する
void CCacheAttribute::Init()
{
	if( m_pVars )
	{
		for( long i = 0; i < m_lCount; ++i )
			VariantClear(&m_pVars[i]);
		delete [] m_pVars;
		m_pVars = NULL;
	}
	m_lCount = 0;
}

// 属性のキャッシュを作成する
sindyErrCode CCacheAttribute::CreateCache( _IRow* lp, IFields* ipFields/* = NULL*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if( ! ipFields )
		lp->get_Fields( &ipFields );

	if( ipFields != NULL )
	{
		Init();

		ipFields->get_FieldCount( &m_lCount );

		// キャッシュ用バッファ作成
		m_pVars = new VARIANT[m_lCount];

		if( m_pVars )
		{
			for( long i = 0; i < m_lCount; ++i )
			{
				VariantInit(&m_pVars[i]);

				IFieldPtr ipField;
				ipFields->get_Field( i, &ipField );
				if( ipField != NULL )
				{
					long lVarType = 0;
					esriFieldType emType;
					ipField->get_VarType( &lVarType );
					ipField->get_Type( &emType );
					switch( emType )
					{
						// ジオメトリ、BLOBのフィールドは飛ばす
						case esriFieldTypeGeometry:
						case esriFieldTypeBlob:
							break;
						default:
							if( lp )
							{
								if( FAILED( Fix_BSTR_get_Value( lp, i, &m_pVars[i] ) ) )
								{
									emErr = sindyErr_AOGetValueFailed;	// エラーだが続行
									ERRORLOG( emErr );
								}
							}
							break;
					}
				}
			}
		}
		else {
			emErr = sindyErr_MemoryAllocationFailed;	// メモリ確保失敗
			ERRORLOG( emErr );
		}
	}
	else {
		emErr = sindyErr_COMFunctionFailed;
		ERRORLOG( emErr );
	}

	return emErr;
}

errorcode::sindyErrCode CCacheAttribute::CreateCache( _IRow* lp, const CFieldMap& cFields )
{
	sindyErrCode emErr = sindyErr_NoErr; // 戻り値

	Init();

	// キャッシュ用バッファ作成
	m_lCount = cFields.GetFieldCount();
	m_pVars = new VARIANT[m_lCount];

	if( m_pVars )
	{
		int i = 0;
		for( CFieldMap::const_iterator it = cFields.begin(); it != cFields.end(); ++it, ++i )
		{
			VariantInit(&m_pVars[i]);
			// ダミーRowの場合は全てNULLにしておかないと
			// NOT NULLチェックが機能しない
			// 実態がある場合は、下で取得しているのでやらない。
			// また、クエリでフィールド制限している場合には
			// Emptyである必要があるので、ダミーRowの場合に
			// のみ限定する。
			// ただし、SiNDY-iなんかでは実体化する前にCheckLogic
			// を行うため関連IDフィールドなんかはNULLのままになっ
			// てしまう。そこで、場当たり的ではあるが、*IDという
			// フィールドに関してはEmptyのままにする。
			if( ! ( lp || it->isid ) )
				m_pVars[i].vt = VT_NULL;

			switch( it->type )
			{
				// ジオメトリ、BLOBのフィールドは飛ばす
				case esriFieldTypeGeometry:
				case esriFieldTypeBlob:
					break;
				default:
					if( lp )
					{
						if( FAILED( Fix_BSTR_get_Value( lp, i, &m_pVars[i] ) ) )
						{
							emErr = sindyErr_AOGetValueFailed;	// エラーだが続行
							ERRORLOG( emErr );
						}
					}
					break;
			}
		}
	}
	else {
		emErr = sindyErr_MemoryAllocationFailed;	// メモリ確保失敗
		ERRORLOG( emErr );
	}

	return emErr;
}

// CCacheAttributeをコピーする
sindyErrCode CopyCacheAttribute( CCacheAttribute& dest, const CCacheAttribute& src )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	// 既存のものがあるなら削除
	dest.Init();
	
	// バッファ作成
	dest.m_pVars = new VARIANT[src.m_lCount];
	dest.m_lCount = src.m_lCount;

	for( long i = 0; i < src.m_lCount; ++i )
	{
		VariantInit(&dest.m_pVars[i]);
		if( FAILED( VariantCopy( &dest.m_pVars[i], (VARIANTARG*)&src.m_pVars[i] ) ) )
		{
			emErr = sindyErr_MemoryAllocationFailed;	// 返り値適切ではないかも
			ERRORLOG( emErr );
		}
	}

	return emErr;
}

} // sindy
