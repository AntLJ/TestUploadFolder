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
 * @file RowBase.cpp
 * @brief CRowBaseクラス実装ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include <boost/foreach.hpp>
#include "type_convert.h"
#include "RowBase.h"
#include "GeometryBase.h"
#include "sindycomutil.h"
#include "TableTypeUtil.h"
#include "GlobalRules.h"
#include "GlobalFunctions.h"
#include "NameString.h"
#include "ErrorObject2.h"
#include "meshutil.h"
#include "GeometryRule.h"
#include "CacheAttribute.h"
#include "winutil.h"
#include "AheNetwork.h"
#include "RuleModel.h"
#include "AheLayerFunctions.h"
#include "sindylib_base_global.h"
#include "TimeTrace.h"
#include "RuleWrapper.h"
#include "GlobalRules.h"
#include <sindy/schema/category.h>

using namespace sindy::schema::category;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define foreach BOOST_FOREACH

#ifndef SINDY_FOR_ENGLISH
const static _TCHAR EXISTING_AND_SET_FLAG[]            = _T("実体があり削除フラグ及び作成フラグが立っている");
const static _TCHAR NONFEATURE_IS_HAVING_SHAPE_CACHE[] = _T("HRESULT[%d:%X] フィーチャではないものが形状キャッシュを持っています");
const static _TCHAR FAIL_ADD_HISTRY[]                  = _T("履歴付与に失敗しました");
const static _TCHAR SHAPE_CHANGE[]                     = _T("形状変更");
const static _TCHAR ATTR_MODIFY[]                      = _T("属性変更");
const static _TCHAR NEW_CREATE[]                       = _T("新規作成");
const static _TCHAR DELETE_STR[]                       = _T("削除");
const static _TCHAR EXISTED_SINDY_INTRODUCTION[]       = _T("移行時既存？");
const static _TCHAR FAIL_MOVE_FEATURE[]                = _T("フィーチャ移動に失敗");
const static _TCHAR NULL_CANNOT_SET[]                  = _T("NULLを設定することはできません[%s]");
const static _TCHAR OVER_CHARNUM_LIMIT[]               = _T("文字数制限を越えています[%s:%s...]:%dバイトオーバー");
const static _TCHAR OVER_CHARNUM_LIMIT2[]              = _T("文字数制限を越えています[%s:%s]:%dバイトオーバー");
const static _TCHAR OVER_DIGITNUM_LIMIT[]              = _T("許容桁数をオーバーしています[%s:%s]:許容桁数：%d");
const static _TCHAR OUT_OF_RANGEDOMAIN[]               = _T("レンジドメイン範囲外です[%s:%s]:許容範囲：%s～%s");
const static _TCHAR NON_EXIST_DOMAINCODE[]             = _T("コード値ドメインに存在しません[%s:%s]");
const static _TCHAR AUTO_ATTR_ASSIGNMENT_NOT_SUPPORT[] = _T("%s.%s : サポートされていない型に対して自動属性付与しようとしました。");
const static _TCHAR DELETE_INFO[]                      = _T("[削除] TABLE[%s] : OID[%d]\n");
const static _TCHAR SAVE_INFO[]                        = _T("[保存] TABLE[%s] : OID[%d] : 保存完了（%s）\n");
const static _TCHAR FIELDS_IS_NOT_INITIALIZED[]        = _T("CRowBaseがキャッシュされているのにm_spFieldsが初期化されていません。");
const static _TCHAR NAMESTR_IS_NOT_INITIALIZED[]       = _T("CRowBaseがキャッシュされているのにm_spNameStringが初期化されていません。");
const static _TCHAR FAIL_PUTREF_SHAPE[]                = _T("IFeature::putref_Shape()に失敗");
const static _TCHAR FAIL_GET_SHAPECOPY[]               = _T("IFeature::get_ShapeCopy() : 失敗しました");
const static _TCHAR MAY_BE_NULL_SHAPE[]                = _T("NULL Shape の可能性があります");
const static _TCHAR TIME_CREATE_CACHE_ATTR[]           = _T("CreateCache:属性");
const static _TCHAR TIME_CREATE_CACHE_SHAPE[]          = _T("CreateCache:形状");
const static _TCHAR TIME_CREATE_CACHE_RELEASE[]        = _T("CreateCache:切り離し");
#else
const static _TCHAR EXISTING_AND_SET_FLAG[]            = _T("It is existing, and create flag and delete flag is set");
const static _TCHAR NONFEATURE_IS_HAVING_SHAPE_CACHE[] = _T("HRESULT[%d:%X] Non-feature is having the shape cache");
const static _TCHAR FAIL_ADD_HISTRY[]                  = _T("Failed to add history");
const static _TCHAR SHAPE_CHANGE[]                     = _T("Shape change");
const static _TCHAR ATTR_MODIFY[]                      = _T("Attribute modification");
const static _TCHAR NEW_CREATE[]                       = _T("Create new");
const static _TCHAR DELETE_STR[]                       = _T("Delete");
const static _TCHAR EXISTED_SINDY_INTRODUCTION[]       = _T("Existed at the time of SiNDY introduction? ");
const static _TCHAR FAIL_MOVE_FEATURE[]                = _T("Failed to move feature");
const static _TCHAR NULL_CANNOT_SET[]                  = _T("Null cannot be set [%s]");
const static _TCHAR OVER_CHARNUM_LIMIT[]               = _T("Over the character number limit [%s:%s...]:%d bytes excess ");
const static _TCHAR OVER_CHARNUM_LIMIT2[]              = _T("Over the character number limit [%s:%s]:%d bytes excess ");
const static _TCHAR OVER_DIGITNUM_LIMIT[]              = _T("Number of digit is over the allowable number of digit s[%s:%s]:allowable number of digits:%d");
const static _TCHAR OUT_OF_RANGEDOMAIN[]               = _T("Out of range of the range domain[%s:%s]:allowable range:%s-%s");
const static _TCHAR NON_EXIST_DOMAINCODE[]             = _T("Does not exist in the code value domain[%s:%s]");
const static _TCHAR AUTO_ATTR_ASSIGNMENT_NOT_SUPPORT[] = _T("%s.%s : Automatic attribute assignment tried to adjust the model which is not supported.");
const static _TCHAR DELETE_INFO[]                      = _T("[Delete] TABLE[%s] : OID[%d]\n");
const static _TCHAR SAVE_INFO[]                        = _T("[Save] TABLE[%s] : OID[%d] : Save finished（%s）\n");
const static _TCHAR FIELDS_IS_NOT_INITIALIZED[]        = _T("CRowBase is having cache, but m_spFields is not initialized.");
const static _TCHAR NAMESTR_IS_NOT_INITIALIZED[]       = _T("CRowBase is having cache, but m_spNameString is not initialized.");
const static _TCHAR FAIL_PUTREF_SHAPE[]                = _T("IFeature::putref_Shape() failed.");
const static _TCHAR FAIL_GET_SHAPECOPY[]               = _T("IFeature::get_ShapeCopy() : failed");
const static _TCHAR MAY_BE_NULL_SHAPE[]                = _T("It may be NULL Shape.");
const static _TCHAR TIME_CREATE_CACHE_ATTR[]           = _T("CreateCache:Attribute");
const static _TCHAR TIME_CREATE_CACHE_SHAPE[]          = _T("CreateCache:Shape");
const static _TCHAR TIME_CREATE_CACHE_RELEASE[]        = _T("CreateCache:Release");
#endif	// SINDY_FOR_ENGLISH

using namespace winutil;
namespace sindy {
using namespace errorcode;

CRowBase::CRowBase() : 
  CSchemaSupport(),
  m_pCache(NULL),
  m_pGeom(NULL),
  m_lOID(-1),
  m_bCreated(false),
  m_bDeleted(false)

{
}

 CRowBase::CRowBase( const CRowBase& obj ) :
	CSchemaSupport(obj),
	m_lOID(obj.m_lOID),
	m_bCreated(obj.m_bCreated),
	m_bDeleted(obj.m_bDeleted)
{
	m_pCache = ( obj.m_pCache ) ? new CCacheAttribute(*obj.m_pCache) : NULL;
	m_pGeom = ( obj.m_pGeom ) ? new CGeometryBase(*obj.m_pGeom) : NULL;
	AtlComPtrAssign( (IUnknown**)&p, (_IRow*)obj ); // Releaseを呼ぶとキャッシュとm_lOIDが初期化されるのでNG
}

CRowBase::CRowBase( _IRow* lp, sindyTableType::ECode emTableType/* = sindyTableType::unknown*/, bool bCreated/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ ) :
	CSchemaSupport( emTableType, spNameString, spFields ),
	m_pCache(NULL),
	m_pGeom(NULL),
	m_lOID(-1),
	m_bCreated(bCreated),
	m_bDeleted(false)
{
	SetObject( lp, emTableType, bCreated, spFields, spNameString );
}

CRowBase& CRowBase::operator=( const CRowBase& obj )
{
	operator=( obj.p );

	// スキーマ部分コピー
	CSchemaSupport::operator=( obj );
	// キャッシュ部分をコピー
	m_pCache = ( obj.m_pCache ) ? new CCacheAttribute(*obj.m_pCache) : NULL;
	m_pGeom = ( obj.m_pGeom ) ? new CGeometryBase(*obj.m_pGeom) : NULL;
	m_lOID = obj.m_lOID;
	m_bCreated = obj.m_bCreated;
	m_bDeleted = obj.m_bDeleted;

	return *this;
}

CRowBase& CRowBase::operator=( _IRow* lp )
{
	AtlComPtrAssign( (IUnknown**)Release(), lp );

	return *this;
}

CRowBase::operator IGeometry*() const
{
	if( m_pGeom )
		return (IGeometry*)(*m_pGeom);
	else {
		IFeaturePtr ipFeat( p );
		if( ipFeat )
		{
			IGeometryPtr ipGeom;
			ipFeat->get_Shape( &ipGeom );
			return ipGeom;
		}
	}
	return (IGeometry*)NULL;
}

bool CRowBase::operator <( const CRowBase& obj )
{
	if( p && obj.p )
		return ( GetOID() < obj.GetOID() && *GetNameString() < *obj.GetNameString() );
	return false;
}

// フィーチャ・レコードをセットする
void CRowBase::SetObject( _IRow* lp, sindyTableType::ECode emTableType/* = sindyTableType::unknown*/, bool bCreated/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNames/* = CSPTableNameString()*/ )
{
	// 既存のものは捨てる
	DeleteCache();
	// メンバ変数初期化
	Init();

	// attach
	AtlComPtrAssign((IUnknown**)&p, lp);
	m_emTableType = emTableType;
	m_bCreated = bCreated;	// 作成フラグ
	if( spFields )
		SetFieldMap( spFields );
	if( spNames )
		SetNameString( spNames );
}

// ダミーCRowBaseを実体化させる
void CRowBase::Materialization( _IRow* lp )
{
	_ASSERTE( ! p );        // 実体があるものに対して処理しようとしている
	_ASSERTE( m_bCreated ); // 新規作成フラグがないものに対して処理しようとしている

	AtlComPtrAssign((IUnknown**)&p, lp);
}

// フィーチャのコピーを作成する
CSPRowBase CRowBase::Clone() const
{
	CSPRowBase cSPRowBase( new CRowBase() );	// 返り値
	CRowBase& cRowBase( *cSPRowBase );

	if( p )
	{
		// 編集開始されているかどうか
		IWorkspaceEditPtr ipWork( GetWorkspace() );
		VARIANT_BOOL vbIsBeingEdited = VARIANT_FALSE;
		ipWork->IsBeingEdited( &vbIsBeingEdited );
		LOGASSERTE_IF( vbIsBeingEdited, sindyErr_EditNotStarted )
		{
			// 作成
			ITablePtr ipTable( GetTable() );
			LOGASSERTE_IF( SUCCEEDED( ipTable->CreateRow( cRowBase.Release() ) ), sindyErr_RowCreateFailed )
			{
				cRowBase.m_bCreated = true;	// 作成フラグを立てておく（履歴書き込みの為）

				// テーブル名キャッシュ、フィールド名対応表コピー
				(CSchemaSupport&)cRowBase = (const CSchemaSupport&)*this;
				// 属性・形状をコピー
				cRowBase.m_pCache = new CCacheAttribute( ( m_pCache ) ? *m_pCache : (_IRow*)*this );
				cRowBase.m_pGeom = new CGeometryBase( GetShapeCopy() );
			}
		}
	}
	return cSPRowBase;
}

// フィーチャの変更を元に戻す
void CRowBase::Reset()
{
	// 削除フラグがクリアされない条件
	// ・pが削除済みの場合
	// ・削除フラグが立っており、pが存在しない
	//
	// 上記以外は最終的に作成・削除フラグを初期化する
	// 
	// DummyRowBaseにしなければならない条件
	// ・削除フラグが立っておらず、pが存在しない

	// pが削除済みかどうかはOJBECTIDを取って、エラーが出るかどうかで確認
	bool bAlreadyDeleted = false;
	if( p )
	{
		CComVariant vaValue;
		if( FAILED( Fix_BSTR_get_Value( p, 1, &vaValue ) ) )
			bAlreadyDeleted = true;
	}

	// 削除フラグがクリアされない条件の場合はそのままにする
	// ただし、bAlreadyDeletedのみ立っている可能性を考慮して
	// m_bDeletedに反映しておく
	if( bAlreadyDeleted || ( m_bDeleted && ( ! p ) ) )
	{
		m_bDeleted = true;
		return;
	}

	DeleteCache(); // キャッシュ削除

	// 作成フラグが立っており、pが存在する場合は
	// CreateDummyRowBaseでやっている属性セットを行う
	// pが存在しない場合は存在しなかったことにしなければ
	// ならないので、削除フラグを立てておく
	if( m_bCreated )
	{
		if( p )
		{
			// CTable::CreateDummyRowBase相当の
			// 処理を行う
			CreateCache(); // キャッシュ作成
			SetDefaultValue(); // デフォルト値セット
			SetCreateAutoValue(); // 自動付与属性セット
			m_bCreated = true;
		}
		else
		{
			m_bDeleted = true;
			return;
		}
	}
	else
		m_bCreated = false;

	m_bDeleted = false;
}

IFieldsPtr CRowBase::GetFields() const
{
	IFieldsPtr ipFields;
	if( p ) {
		INVOKEMETHOD( get_Fields( &ipFields ) );
	}
	return ipFields;
}

bool CRowBase::HasOID() const
{
	// m_lOIDが-1の場合は未取得なので、実際に調べる
	if( -1 == m_lOID )
	{
		VARIANT_BOOL vbHasOID = VARIANT_FALSE;
		if( p ) {
			INVOKEMETHOD( get_HasOID( &vbHasOID ) );
		}
		return VB2bool(vbHasOID);
	}
	else
		return ( m_lOID < 0 ) ? false : true;
}

long CRowBase::GetOID() const
{
	// 既に取得した形跡がある場合はキャッシュを使用
	if( p )
	{
		if( -1 == m_lOID )
		{
			if( ! HasOID() )
				m_lOID = -2; // OBJECTIDが無いテーブルをサポート
			else {
				INVOKEMETHOD( get_OID( const_cast<long*>(&m_lOID) ) );
			}
		}
	}
	return m_lOID;
}

void CRowBase::SetDefaultValue()
{
	CSPFieldMap cspFields( GetFieldMap() );
	if( cspFields )
	{
		CFieldMap& cFields( *cspFields.get() );
		long lFieldCount = cFields.GetFieldCount();
		for( long i = 0; i < lFieldCount; ++i )
		{
			if( cFields.IsEditable( i ) )
			{
				const VARIANT& va( cFields.GetDefaultValue(i) );
				if( ! ( VT_NULL == va.vt && ( ! cFields.IsNullable( i ) ) ) ) // NOT NULLなのにデフォルト値がNULLの場合はエラーになるので何もしない
					SetValue( i, cFields.GetDefaultValue( i ) );
			}
		}
	}
}

// 新規作成時の自動付与属性を付与する
void CRowBase::SetCreateAutoValue()
{
	// ログインしている場合には新規作成時の自動付与属性
	if( g_cRule && g_cRule->IsLogined() )
	{
		const CAutoValue& cValue( g_cRule->GetCreatedAutoValue(GetTableName()) );
		CSPFieldMap cFields( GetFieldMap() );
		for( CAutoValue::const_iterator it = cValue.begin(); it != cValue.end(); ++it )
		{
			long lFieldIndex = cFields->FindField( it->first );
			if( 0 < lFieldIndex )
			{
				switch( cFields->GetFieldType( lFieldIndex ) )
				{
				case esriFieldTypeInteger:
				case esriFieldTypeSmallInteger: SetValue( lFieldIndex, _ttol(it->second) ); break;
				case esriFieldTypeString: SetValue( lFieldIndex, it->second ); break;
				default:
					_ERRORLOG( sindyErr_AlgorithmFailed, AUTO_ATTR_ASSIGNMENT_NOT_SUPPORT, GetTableName(), it->first );
					break;
				}
			}
		}
	}
}

sindyErrCode CRowBase::Store( CErrorObjectsBase& cErrs )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if ( ! p )	// 実体がないものはスルー
		return cErrs.GetDefaultErrCode();

	CSPErrorObjectBase pErr;
	// エラーオブジェクトに自分自身を登録
	if( g_bCreateAllStoreInfo ) // インポート時には全てのレコード情報をエラーオブジェクトに登録する必要はない（かなり時間がかかる…）
		pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );

	// 実体があるのに作成フラグ及び削除フラグが立っていることはありえない
	if( IsCreated() && IsDeleted() )
	{
		if( ! pErr )
			pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
		pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AlgorithmFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("%s"), EXISTING_AND_SET_FLAG ) ) );
		return cErrs.GetDefaultErrCode();
	}

	schema::ipc_table::update_type::ECode emUpdate = schema::ipc_table::update_type::kDefault;

	// 削除の場合
	HRESULT hr = S_OK;
	if( IsDeleted() )
	{
		GetOID(); // 削除されるとできなくなるのでm_lOIDにキャッシュしておく（m_lOIDはDeleteCacheでは消えない）

		// キャッシュ削除
		DeleteCache();
		// 削除
		if( SUCCEEDED( hr = p->Delete() ) )
		{
#ifdef _DEBUG
			if( g_bOutputTraceUpdate || g_bTraceUpdate )
#else
			if( g_bOutputTraceUpdate )
#endif // ifdef _DEBUG
			{
				if( g_bOutputTraceUpdate )
					TRACEMESSAGE( DELETE_INFO, GetTableName(), m_lOID );
#ifdef _DEBUG
				else
					ATLTRACE( _T("[削除] TABLE[%s] : OID[%d]\n"), GetTableName(), m_lOID );
#endif // ifdef _DEBUG
			}
		}
		else {
			if( ! pErr )
				pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
			pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AODeleteFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X] MSG[%s]"), hr, hr, GETCOMERRORSTRING() ) ) );
		}
		return cErrs.GetDefaultErrCode();
	}

	bool bAttrUpdated = false; // UPDATE_C更新用
	// 削除以外の場合
	if( m_pCache || m_pGeom )
	{
		// 属性
		// キャッシュがある場合はキャッシュを先に反映させる
		if( m_pCache )
		{
			// キャッシュ反映
			const CFieldMap& cFields( *GetFieldMap() );
			long i = -1;
			foreach( const CFieldMap::_FIELD& field, std::make_pair( cFields.begin(), cFields.end() ) )
			{
				++i;
				// 編集できないフィールドは飛ばす
				if( field.editable && esriFieldTypeGeometry != field.type )
				{
					VARIANT& va = *(*m_pCache)[i];
					// EMPTYの場合は編集不可のフィールドか、又はクエリフィルタで省かれたもの
					if( va.vt != VT_EMPTY )
					{
						// 変更がある場合のみput_Valueする（既存変更のみ）
						bool update = ( m_bCreated )  ? true : false; // 新規作成の時は必ずセットするので比較しない
						if( ! update )
						{
							_variant_t vaValue;
							if( SUCCEEDED( hr = Fix_BSTR_get_Value( p, i, &vaValue ) ) )
							{
								if( ! IsEqualSDEValue( field.type, va, vaValue ) )
									update = true;
							}
							else {
								if( ! pErr )
									pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
								pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AOGetValueFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) ) );
							}
						}
						if( update )
						{
							if( SUCCEEDED( hr = p->put_Value( i, va ) ) )
							{
								emUpdate = schema::ipc_table::update_type::kAttrUpdated;
								bAttrUpdated = true; // 属性更新は形状更新で上書きされるため、UPDATE_Cを正しく更新するためには保持する必要あり
							}
							else {
								CComVariant vaErr( va );
								vaErr.ChangeType(VT_BSTR);
								if( VT_NULL == vaErr.vt )
									vaErr = _T("NULL");
								if( ! pErr )
									pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
								pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AOPutValueFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X] FIELD[%s] VALUE[%s] MSG[%s]"), hr, hr, cFields.GetName(i), (LPCTSTR)CString(vaErr.bstrVal), GETCOMERRORSTRING() ) ) );
								Trace( true, false );
							}
						}
					}
				}
			}
		}
		// 形状
		if( m_pGeom )
		{
			IFeaturePtr ipFeat(p);
			if( ipFeat )
			{
				// 変更がある場合のみputref_Shapeする（既存変更のみ）
				bool update = ( m_bCreated )  ? true : false; // 新規作成の時は必ずセットするので比較しない
				if( ! update )
				{
					IGeometryPtr ipGeom;
					if( SUCCEEDED( hr = ipFeat->get_Shape( &ipGeom ) ) )
					{
						// 判定ルーチンに IRelationalOperator::Equalsを使う
						if( g_bStoreShapeJudge_Native )
						{
							IRelationalOperatorPtr ipRel( ipGeom );
							if( ipRel )
							{
								VARIANT_BOOL vaEqual = VARIANT_FALSE;
								ipRel->Equals( (IGeometry*)*m_pGeom, &vaEqual );
								if( ! vaEqual )
									update = true;
							}
						}
						// 判定ルーチンに構成点比較（CGeometry::EqualsVertex）を使う
						else if( g_bStoreShapeJudge_Exact )
						{
							if( ! m_pGeom->EqualsVertex( ipGeom ) )
								update = true;
						}
						// 判定ルーチンに CGeoemtry::Equals（IClone::IsEqual）を使う＋α
						else {
							if( ! m_pGeom->Equals( ipGeom ) )
								update = true;
						}
					}
					else {
						if( ! pErr )
							pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
						pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryNotFound, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) ) );
					}
				}
				if( update )
				{
					if( SUCCEEDED( hr = ipFeat->putref_Shape( (IGeometry*)*m_pGeom ) ) )
						emUpdate = schema::ipc_table::update_type::kShapeUpdated;
					else {
						if( ! pErr )
							pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
						pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AOPutValueFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) ) );
						AheTraceGeometry( (IGeometry*)*m_pGeom, _T("putref_Shape失敗") );
					}
				}
			}
			else {
				if( ! pErr )
					pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
				pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AlgorithmFailed, sindyErrLevel_ERR, NULL, 0, -1, NONFEATURE_IS_HAVING_SHAPE_CACHE, hr, hr ) ) );
			}
		}
	}

	if( m_bCreated ) emUpdate = schema::ipc_table::update_type::kCreated;

	// キャッシュ削除
	DeleteCache();

	// 新規作成/削除以外はキャッシュ作成前の変更確認（キャッシュがあった場合は二度手間になるけどしょうがない…）
	// 結構時間がかかる処理のようなので、外部フラグで制御
	if( g_bCheckUpdatedByArcObjects ) // キャッシュ作成前の変更をチェックするかどうか
	{
		if( ! ( schema::ipc_table::update_type::kCreated == emUpdate ||
			schema::ipc_table::update_type::kDeleted == emUpdate ) )
			Changed( &emUpdate );
	}

	if( sindyErr_NoErr == emErr )
	{
		if( schema::ipc_table::update_type::kDefault != emUpdate )
		{
			bool bStore = false;

			// 履歴フィールドがないSiNDYオブジェクトも存在する（SiNDY-iとか）
			// それらにとっては無駄な処理にしかならないので、外部フラグで制御
			// できるようにする
			if( g_bSetHistory )
			{
				// LQも除外しないとまずし。
				if ( !AheIsLQTableType( GetTableType() ) ) {
					// 履歴を付与
					if( sindyErr_NoErr == ( emErr = SetHistory( emUpdate, bAttrUpdated ) ) )
						bStore = true;
					else 
					{
						if( ! pErr )
							pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
						pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( emErr, sindyErrLevel_ERR, NULL, 0, -1, FAIL_ADD_HISTRY ) ) );
						bStore = false;
					}
				}
				else
					bStore = true;
			}
			else
				bStore = true;

			if ( bStore ) {
				// 保存
				// [FYI] IsKnownSimpleがFALSEの時は、自動的にSimplifyされるっぽい（同一構成点が無くなるみたい）あとで調べること（Storeしたら再度GetFeatureで取得し直さないとDB内にある形状にならないよ）
				static DWORD dwStoreStart = 0;
				if( g_bCalcStoreTime )
					dwStoreStart = GetTickCount();
				if( SUCCEEDED( hr = p->Store() ) )
				{
#ifdef _DEBUG
					if( g_bTraceUpdate || g_bOutputTraceUpdate )
#else
					if( g_bOutputTraceUpdate )
#endif // ifdef _DEBUG
					{
						CString cStr;
						switch( emUpdate )
						{
						case schema::ipc_table::update_type::kShapeUpdated: cStr = SHAPE_CHANGE; break;
						case schema::ipc_table::update_type::kAttrUpdated:  cStr = ATTR_MODIFY; break;
						case schema::ipc_table::update_type::kCreated:      cStr = NEW_CREATE; break;
						case schema::ipc_table::update_type::kDeleted:      cStr = DELETE_STR; _ASSERTE( schema::ipc_table::update_type::kDeleted != emUpdate ); break; // 削除はもう来ないはず
						default: cStr = EXISTED_SINDY_INTRODUCTION; break;
						}
						if( g_bOutputTraceUpdate )
							TRACEMESSAGE( SAVE_INFO, GetTableName(), GetOID(), cStr );
#ifdef _DEBUG
						else
							ATLTRACE( _T("[保存] TABLE[%s] : OID[%d] : 保存完了（%s）\n"), GetTableName(), GetOID(), cStr );
#endif // ifdef _DEBUG
					}
				}
				else {
					if( ! pErr )
						pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
					pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AOStoreFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X] MSG[%s]"), hr, hr, GETCOMERRORSTRING() ) ) );
					Trace();
					// フィールドの桁あふれをチェックしておく
					CRowBase::CheckLogic( CLogicRule(), cErrs );
				}
				if( g_bCalcStoreTime )
				{
					g_dwTotalStoreTime += ( GetTickCount() - dwStoreStart );
					++g_lTotalStoreCount;
				}
			}
		}
#ifdef _DEBUG
		else if( g_bTraceUpdate )
			ATLTRACE( _T("%s : TABLE[%s] : OID[%d] : 未保存（変更無し）\n"), __func__, GetTableName(), GetOID() );
#endif // ifdef _DEBUG
	}

	// 最後にCerateフラグはfalseにしておく
	m_bCreated = false;

	return cErrs.GetDefaultErrCode();
}

// コンフリクト回避のための既編集チェックを行う
sindyErrCode CRowBase::CheckModify( CErrorObjectsBase& cErrs ) const
{
	if ( (_IRow*)p ) {	// 実体があるもののみ
		if ( Changed() ) {

			// 自分自身
			CString strMsg;
			if( g_cRule && g_cRule->IsModified( *this, strMsg ) )
			{
				CSPErrorObjectBase cErr( new CErrorObjectBase( *this ) );
				cErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AlreadyModified, sindyErrLevel_ERR, NULL, -1, -1, _T("%s"), strMsg ) ) );
				cErrs.push_back( cErr );
			}
		}
	}

	return cErrs.GetDefaultErrCode();
}

// SiNDY-c用更新タイプ変換関数（bug 5201）
long update_type2update_code( schema::ipc_table::update_type::ECode emCode )
{
	using namespace schema::ipc_table::update_type;

	long lRet = -1; // 返り値
	switch( emCode )
	{
		case kDefault:		lRet = 0; break; // 移行時既存 -> 修正なし/初期化
		case kCreated:		lRet = 3; break; // 新規作成   -> 新規入力/形状、属性を修正
		case kShapeUpdated:	lRet = 1; break; // 形状変更   -> 形状を修正
		case kAttrUpdated:	lRet = 2; break; // 属性変更   -> 属性を修正
		case kDeleted:       // 削除
		case kiPSImported:   // 上海インポート
		case kImported:      // インポート
		default:			lRet = -1; break; // 対応するものがないが、まあいいか…
	}
	return lRet;
}

sindyErrCode CRowBase::SetHistory( schema::ipc_table::update_type::ECode emCode, bool bAttrUpdated/* = false */ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	const CFieldMap& cFields( *GetFieldMap() );
	long lUpdateTypeIndex =cFields.HasField( schema::ipc_table::kUpdateType );
	if( 0 < lUpdateTypeIndex ) SetValue( lUpdateTypeIndex, emCode );					// 更新タイプ

	if( g_cRule && g_cRule->IsLogined() )
	{
		long lOperatorIndex = cFields.HasField( schema::ipc_table::kOperator );
		long lModifyDateIndex = cFields.HasField( schema::ipc_table::kModifyDate );
		long lPurposeIndex = cFields.HasField( schema::ipc_table::kPurpose );
		if( 0 < lOperatorIndex ) SetValue( lOperatorIndex, g_cRule->GetOperatorName() );	// オペレータ名
		if( 0 < lModifyDateIndex ) SetValue( lModifyDateIndex, GetTime() );				// 最終更新日時
		if( 0 < lPurposeIndex ) SetValue( lPurposeIndex, g_cRule->GetWorkPurposeID() );		// 作業目的ID
	}
	else {
		long lModifyProgNameIndex = cFields.HasField( schema::ipc_table::kModifyProgName );
		long lProgModifyDateIndex = cFields.HasField( schema::ipc_table::kProgModifyDate );
		if( 0 < lModifyProgNameIndex )
		{
			TCHAR szBuffer[_MAX_PATH] = {};
			::GetModuleFileName(NULL, szBuffer, _MAX_PATH);
			LPCTSTR pos = ::_tcsrchr(szBuffer, _T('\\'));
			SetValue( lModifyProgNameIndex,  pos ? pos + 1 : szBuffer );	// 更新プログラム名
		}
		if( 0 < lProgModifyDateIndex ) SetValue( lProgModifyDateIndex, GetTime() );				// バッチ処理での最終更新日時
	}
	// SiNDY-c更新タイプ
	// PGDB編集モードによらずあれば更新（bug 5777,10744）
	long lUpdateCIndex = cFields.HasField( sindyc_table::kUpdateC );
	if( 0 < lUpdateCIndex )
	{
		SetValue( lUpdateCIndex, update_type2update_code( emCode ) | GetValue( lUpdateCIndex, 0L ) ); // 更新タイプ（PGDB編集モード用）（bug 5201）
		// 形状更新の場合には、属性更新でもある場合あり
		// なので、形状更新の場合のみbAttrUpdatedフラグを見て追加対応する
		if( schema::ipc_table::update_type::kShapeUpdated == emCode && bAttrUpdated )
			SetValue( lUpdateCIndex, 2L | GetValue( lUpdateCIndex, 0L ) );
	}

	// 自動付与属性
	emErr = SetAutoValue();

	return emErr;
}

// 自動付与属性を付与する
sindyErrCode CRowBase::SetAutoValue()
{
	if( ! ( g_cRule && g_cRule->IsLogined() ) )
		return sindyErr_NoErr;

	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	const CFieldMap& cFields( *GetFieldMap() );
	const CAutoValue& cAutoValue( g_cRule->GetAutoValue() );
	for( CAutoValue::const_iterator it = cAutoValue.begin(); it != cAutoValue.end(); ++it )
	{
		long lFieldIndex = cFields.HasField( it->first );
		if( 0 < lFieldIndex ) 
		{
			switch( cFields.GetFieldType(lFieldIndex) )
			{
			case esriFieldTypeInteger:
			case esriFieldTypeSmallInteger: SetValue( lFieldIndex, _ttol(it->second) ); break;
			case esriFieldTypeString: SetValue( lFieldIndex, it->second ); break;
			default:
				_ERRORLOG( emErr = sindyErr_AlgorithmFailed, AUTO_ATTR_ASSIGNMENT_NOT_SUPPORT, GetTableName(), it->first );
				break;
			}
		}
	}

	return emErr;
}

ITablePtr CRowBase::GetTable() const
{
	ITablePtr ipTable;
	if( p ) {
		INVOKEMETHOD( get_Table( &ipTable ) );
	}
	return ipTable;
}

IWorkspacePtr CRowBase::GetWorkspace() const
{
	IWorkspacePtr ipRet;	// 返り値

	IDatasetPtr ipDataset( GetTable() );
	if( ipDataset )
	{
		LOGASSERTE_IF( SUCCEEDED( ipDataset->get_Workspace( &ipRet ) ), sindyErr_AOGetWorkspaceFailed );
	}

	return ipRet;
}

sindyErrCode CRowBase::GetValue( long lIndex, VARIANT& vaValue ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if( lIndex >= 0 )
	{
		// キャッシュあり
		if( m_pCache )
		{
			if( FAILED( VariantCopy( &vaValue, (*m_pCache)[lIndex] ) ) )
				emErr = sindyErr_MemoryAllocationFailed;	// 適切な返り値ではないかも
		}
		// キャッシュなし
		else {
			if( p )
			{
				HRESULT hr;	// [bug 3182] たまにここでエラーが出るので、調査のために詳細ログを出す
				SUCCEEDED_IF( hr = Fix_BSTR_get_Value( p, lIndex, &vaValue ), emErr = sindyErr_AOGetValueFailed, _T("HRESULT[%ld(0x%X)] TABLE[%s] OID[%ld] lIndex[%ld]"), hr, hr, GetTableName(), GetOID(), lIndex )
				{
				}
			}
			else
				emErr = sindyErr_ClassMemberMissing;
		}
	}
	else
		emErr = sindyErr_ArgLimitOver;	// インデックスは0以上

	return emErr;
}

sindyErrCode CRowBase::SetValue( long lIndex, const VARIANT& vaValue )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if( lIndex >= 0 )
	{
		// キャッシュあり
		if( m_pCache )
		{
			if( VT_BSTR == vaValue.vt && 0 == wcslen( vaValue.bstrVal ) )
			{
				CComVariant va;
				va.vt = VT_NULL;
				if( FAILED( VariantCopy( (*m_pCache)[lIndex], (VARIANTARG*)&va ) ) )	// constで受け取ってくれないので無理やりキャスト
					emErr = sindyErr_MemoryAllocationFailed;	// 適切な返り値ではないかも
			}
			else {
				if( FAILED( VariantCopy( (*m_pCache)[lIndex], (VARIANTARG*)&vaValue ) ) )	// constで受け取ってくれないので無理やりキャスト
					emErr = sindyErr_MemoryAllocationFailed;	// 適切な返り値ではないかも
			}
		}
		// キャッシュなし
		else {
			if( p ) {
				INVOKEMETHOD( put_Value( lIndex, vaValue ) );
				emErr = (FAILED(hr)) ? sindyErr_AOPutValueFailed : sindyErr_NoErr;
			}
			else
				emErr = sindyErr_ClassMemberMissing;
		}
	}
	else
		emErr = sindyErr_ArgLimitOver;	// インデックスは0以上

	return emErr;
}

// 値がNULLかどうか確認する
bool CRowBase::IsNull( long lIndex ) const
{
	CComVariant va;
	GetValue( lIndex, va );
	return ( VT_NULL == va.vt || VT_EMPTY == va.vt );
}

//フィールド対応表の参照を返す
CSPFieldMap CRowBase::GetFieldMap() const
{
	if( p && ( ! CSchemaSupport::GetFieldMap() ) )
	{
		// キャッシュされていないならメンバを自分で初期化する
		// これをすると、CRowBase毎にCTableNameStringの実体を持つことになるので
		// 本当は好ましくない。しかし、利便性を重視し、自分で作成できるように
		// しておく。ただし、デバッグトレースは出して開発者にはわかるように
		// しておくこと。
		LOGASSERTEM_IF( ! IsCached(), sindyErr_ClassMemberMissing, FIELDS_IS_NOT_INITIALIZED )
		{
			IFieldsPtr ipFields;
			LOGASSERTE_IF( NULL != ( ipFields = GetFields() ), sindyErr_COMFunctionFailed )
			{
				ATLTRACE(_T("%s : CFieldMap cache created.\n"), __func__ );
				return CSchemaSupport::GetFieldMap( ipFields );
			}
		}
	}

	return CSchemaSupport::GetFieldMap();
}

// テーブル名などを管理するメンバの参照を返す
CSPTableNameString CRowBase::GetNameString() const
{
	if( p && ( ! CSchemaSupport::GetNameString() ) )
	{
		// キャッシュされていないならメンバを自分で初期化する
		// これをすると、CRowBase毎にCTableNameStringの実体を持つことになるので
		// 本当は好ましくない。しかし、利便性を重視し、自分で作成できるように
		// しておく。ただし、デバッグトレースは出して開発者にはわかるように
		// しておくこと。
		LOGASSERTEM_IF( ! IsCached(), sindyErr_ClassMemberMissing, NAMESTR_IS_NOT_INITIALIZED )
		{
			ITablePtr ipTable;
			LOGASSERTE_IF( NULL != ( ipTable = GetTable() ), sindyErr_COMFunctionFailed )
			{
				return CSchemaSupport::GetNameString( ipTable );
			}
		}
	}

	return CSchemaSupport::GetNameString();
}

bool CRowBase::IsEqualObject(_IRow* ipRowBase ) const
{
	if( p )
	{
		// まずは、OIDのみで比較する（速度優先）
		if( AheGetOID( ipRowBase ) == GetOID() )
		{
			ITablePtr ipTable;
			ipRowBase->get_Table( &ipTable );
			CTableNameString cNames( ipTable );
			return ( cNames == *GetNameString() );
		}
	}
	return false;
}

bool CRowBase::IsEqualObject( const CRowBase& cRowBase ) const
{
	if( cRowBase.p && p ) // DummyRowBaseの場合はpが存在しない
		return ( GetOID() == cRowBase.GetOID() && *GetNameString() == *cRowBase.GetNameString() );
	return false;
}

// キャッシュを作成する
sindyErrCode CRowBase::CreateCache( bool bDetach/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CRowBase::CreateCache()"));

	double dStart = GetTickCount();	// 時間計測用

	if( spFields )
		SetFieldMap( spFields );
	if( spNameString )
		SetNameString( spNameString );

	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	g_trace.Start(TIME_CREATE_CACHE_ATTR);
	// 既存のキャッシュがない場合にのみ作成
	// 属性のキャッシュ
	if( ! m_pCache )
	{
		// OBJECTIDだけはここで取得
		if( p )
			m_lOID = GetOID();
		// 対応表作成でエラーが無ければキャッシュを作成する
		if( GetFieldMap() )
		{
			m_pCache = new CCacheAttribute();
			if( m_pCache )
				emErr = m_pCache->CreateCache( p, *GetFieldMap() );
			else
				emErr = sindyErr_MemoryAllocationFailed;

		}
		else {
			emErr = sindyErr_FieldMapNotCreated;	// フィールド対応表がセットされていない
			ERRORLOG(emErr);
			SASSERT( GetFieldMap() );
		}
	}
	g_trace.End(TIME_CREATE_CACHE_ATTR);
	g_trace.Start(TIME_CREATE_CACHE_SHAPE);
	// 形状のキャッシュ
	if( ! m_pGeom )
	{
		if( 0 < m_spFields->GetShapeIndex() ) // pがNULLの場合あり（bug 5421）が、エラーとはしない（CTable::CreateDummyRowBaseのため）
		{
			IGeometryPtr ipGeom;
			IFeaturePtr ipFeat( p );
			if( ipFeat )
			{
				LOGASSERTEM_IF( SUCCEEDED(ipFeat->get_ShapeCopy( &ipGeom )), sindyErr_GeometryNotFound, _T("%s"), FAIL_GET_SHAPECOPY )
				{
				}
			}
			m_pGeom = new CGeometryBase( ipGeom );
		}
	}
	g_trace.End(TIME_CREATE_CACHE_SHAPE);

	g_trace.Start(TIME_CREATE_CACHE_RELEASE);
	// 切り離し
	if( bDetach )
	{
		if( p )
		{
			p->Release();
			p = NULL;
		}
	}

	// キャッシュ時間を計算
	g_dTotalCacheTime += (double)GetTickCount()-dStart;
	g_trace.End(TIME_CREATE_CACHE_RELEASE);

	return emErr;
}

// キャッシュを削除する
void CRowBase::DeleteCache(/* bool bDeleteContainerCache*//* = false*/ )
{
	// 属性のキャッシュを削除
	if( m_pCache )
	{
		delete m_pCache;
		m_pCache = NULL;
	}
	// 形状のキャッシュを削除
	if( m_pGeom )
	{
		delete m_pGeom;
		m_pGeom = NULL;
	}
	// m_lOIDはDeleteCacheでは削除しない（仕様！！）
}

// キャッシュされているかどうかをチェックする
bool CRowBase::IsCached() const
{
	bool bRet = false;	// 返り値

	// 属性がキャッシュされているかどうか
	if( m_pCache )
		bRet = true;

	// 形状がキャッシュされているかどうか
	if( ! m_pGeom )
	{
		if( bRet )
			bRet = false;
	}

	return bRet;
}

// コピーする
sindyErrCode CRowBase::CopyTo( CRowBase& dest, bool bIsCopyCache/*=true*/, bool bIsCopyRowBase/*=true*/, bool bIsCopyFlag/*=true*/, bool bIsCopyHistory/* = true*/, bool bIsCopySchema/*=true*/ ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if ( bIsCopyRowBase ) {
		AtlComPtrAssign( (IUnknown**)dest.Release(), (_IRow*)*this );	// IRow(QI)
		// OIDをコピーする
		dest.m_lOID = m_lOID;
	}

	if ( bIsCopyCache ) {
		// 属性をコピーする（既存のキャッシュはdest.Release()で削除されるのでいらない）
		if( m_pCache )
		{
			static std::list<CString> g_listHistory;
			std::map<CString,CComVariant> mapHistory;
			if ( dest.m_pCache )
			{
				// 履歴コピーしない場合は元の履歴をバックアップ
				if( ! bIsCopyHistory )
				{
					if( g_listHistory.empty() )
					{
						g_listHistory.push_back( schema::ipc_table::kOperator );
						g_listHistory.push_back( schema::ipc_table::kModifyDate );
						g_listHistory.push_back( schema::ipc_table::kPurpose );
						g_listHistory.push_back( schema::ipc_table::kUpdateType );
						g_listHistory.push_back( schema::ipc_table::kProgModifyDate );
						g_listHistory.push_back( schema::ipc_table::kModifyProgName );
						g_listHistory.push_back( schema::ipc_table::kUserClaim ); // これも含めると、履歴じゃなくて ipc_table だな
					}
					const CFieldMap& fields = *dest.GetFieldMap();
					foreach( const CString& name, g_listHistory )
					{
						long index = fields.HasField(name);
						if( 0 <= index )
							mapHistory[name] = dest.GetValue(index);
					}
				}
				delete dest.m_pCache;
			}

			dest.m_pCache = new CCacheAttribute( *m_pCache );
			if( ! dest.m_pCache ) 
				emErr = sindyErr_MemoryAllocationFailed;
			else {
				// 履歴コピーしない場合は元の履歴を戻す
				if( ! bIsCopyHistory )
				{
					const CFieldMap& fields = *GetFieldMap(); // この後スキーマも変更される可能性があるためソース側のCFieldMapを使用する
					foreach( const CString& name, g_listHistory )
					{
						long index = fields.HasField(name);
						if( 0 <= index )
							dest.SetValue(index, mapHistory[name]);
					}
				}
			}
		}
		// 形状をコピーする
		if( m_pGeom )
		{
			if ( dest.m_pGeom )
				delete dest.m_pGeom;

			dest.m_pGeom = new CGeometryBase( *m_pGeom );
			if( ! dest.m_pGeom )
				emErr = sindyErr_MemoryAllocationFailed;
		}

		if( emErr != sindyErr_NoErr )
		{
			ERRORLOG(emErr);
			SASSERT( emErr != sindyErr_NoErr );
		}
		// スキーマ関係をコピーする
		if( bIsCopySchema )
			(CSchemaSupport&)dest = (const CSchemaSupport&)*this;
	}

	if ( bIsCopyFlag ) {
		dest.m_bCreated = m_bCreated;
		dest.m_bDeleted = m_bDeleted;
	}

	// srcがダミーかつdestのオブジェクトが存在するときは、destの作成フラグを戻す。
	if ( ! (_IRow*)(*this) && (_IRow*)dest && dest.m_bCreated )
		dest.m_bCreated = false;

	return emErr;
}

void CRowBase::swap( CRowBase& src, bool bIsCopyCache/*=true*/, bool bIsCopyRowBase/*=true*/, bool bIsCopyFlag/*=true*/ )
{
	CRowBase cSwapRowBase;

	CopyTo( cSwapRowBase, bIsCopyCache, bIsCopyRowBase, bIsCopyFlag );
	src.CopyTo( *this, bIsCopyCache, bIsCopyRowBase, bIsCopyFlag );
	cSwapRowBase.CopyTo( src, bIsCopyCache, bIsCopyRowBase, bIsCopyFlag );
}

// 与えられた形状の範囲で平行移動処理を行う
sindyErrCode CRowBase::Move( const CGeometryBase& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode, CErrorObjectsBase& cErrs )
{
	sindyErrCode emErr = sindyErr_NoErr;

	CGeometryBase* pGeom( GetShape() );
	if( pGeom && sindyErr_NoErr != ( emErr = pGeom->Move( cGeom, pairMove, emMode ) ) )
		cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this, CSPErrorInfoBase( new CErrorInfoBase( emErr, sindyErrLevel_ERR, NULL, -1, -1, _T("%s"), FAIL_MOVE_FEATURE ) ) ) ) );

	return cErrs.GetDefaultErrCode();
}

// 属性に変更があったかどうかをチェックする
bool CRowBase::Changed( schema::ipc_table::update_type::ECode* emUpdateType/* = NULL*/ ) const
{
	// 先に形状で比較する
	bool bChanged = ShapeChanged(); // 返り値

	if( emUpdateType ) 
		*emUpdateType = bChanged ? schema::ipc_table::update_type::kShapeUpdated : schema::ipc_table::update_type::kDefault;	// 引数初期化

	// 形状が変更されているなら属性の変更チェックはいらない
	if( bChanged )
		return true;

	// 属性のチェック
	if( p )
	{
		if ( m_bDeleted ) {
			bChanged = true;
			if( emUpdateType )
				*emUpdateType = schema::ipc_table::update_type::kDeleted;
		}
		else if( m_bCreated ) {
			bChanged = true;
			if( emUpdateType )
				*emUpdateType = schema::ipc_table::update_type::kCreated;
		} else {
			const CFieldMap& fields( *GetFieldMap() );

			// キャッシュがある場合
			if( m_pCache )
			{
				CComVariant vaCache, vaValue;
				long i = -1;
				foreach( const CFieldMap::_FIELD& field, std::make_pair( fields.begin(), fields.end() ) )
				{
					++i;
					if( field.editable )
					{
						// Shapeフィールドはキャッシュも取らないし、比較もできないので飛ばす
						if( esriFieldTypeGeometry == field.type )
							continue;

						vaCache.Clear();
						vaValue.Clear();

						GetValue( i, vaCache );
						Fix_BSTR_get_Value( p, i, &vaValue );
						if( ! IsEqualSDEValue( field.type, vaCache, vaValue ) )
						{
							bChanged = true;
							if( emUpdateType )
								*emUpdateType = schema::ipc_table::update_type::kAttrUpdated;
							break;
						}
					}
				}
			}
			// キャッシュが無い場合
			else {
				// 属性のチェック
				IRowChangesPtr ipRowBaseChanges( p );
				if( NULL != ipRowBaseChanges )
				{
					long lFieldCount = fields.GetFieldCount();
					VARIANT_BOOL vbChanged = VARIANT_FALSE;
					for( long i = 0; i < lFieldCount; ++i )
					{
						ipRowBaseChanges->get_ValueChanged( i, &vbChanged );
						if( vbChanged )
						{
							bChanged = true;
							if( emUpdateType )
								*emUpdateType = schema::ipc_table::update_type::kAttrUpdated;
							break;	// 一つでもあるならもうチェック終わり
						}
					}
				}
			}
		}
	}
	else if( m_bDeleted )
	{
		bChanged = true;
		if( emUpdateType )
			*emUpdateType = schema::ipc_table::update_type::kDeleted;
	}
	else if( m_bCreated )
	{
		bChanged = true;
		if( emUpdateType )
			*emUpdateType = schema::ipc_table::update_type::kCreated;
	}

	return bChanged;
}

// 属性に変更があったかどうかをチェックする
bool CRowBase::Changed( LPCTSTR lpcszFieldName ) const
{
	if( ! p )
		return IsCreated() ? true : false;

	bool bChanged = false; // 返り値

	// キャッシュがある場合にはキャッシュで比較
	if( m_pCache )
	{
		long lIndex = GetFieldMap()->FindField(lpcszFieldName);
		if( GetFieldMap()->IsShapeField( lIndex ) )
			return ShapeChanged();

		LOGASSERTE_IF( lIndex >= 0, sindyErr_AOGetFieldIndexFailed )
		{
			CComVariant vaCache, vaValue;
			GetValue( lIndex, vaCache );
			Fix_BSTR_get_Value( p, lIndex, &vaValue );

			// VT_DATEの比較は行えないため、VT_BSTRに変換して比較する
			if( vaCache.vt == VT_DATE )
				vaCache.ChangeType(VT_BSTR);
			if( vaValue.vt == VT_DATE )
				vaValue.ChangeType(VT_BSTR);

			if( vaCache != vaValue )
				bChanged = true;
		}
	}
	// キャッシュがない場合にはキャッシュを作らずに比較
	else {
		IFieldsPtr ipFields;
		p->get_Fields( &ipFields );
		long lIndex = -1;
		ipFields->FindField( _bstr_t(lpcszFieldName), &lIndex );
		if( lIndex > 0 )
		{
			IFieldPtr ipField;
			ipFields->get_Field( lIndex, &ipField );
			esriFieldType esriType;
			ipField->get_Type( &esriType );
			if( esriFieldTypeGeometry == esriType )
				return ShapeChanged();
			else {
				IRowChangesPtr ipRowBaseChanges( p );
				CComVariant vaOrgVal, vaCurrentVal;
				ipRowBaseChanges->get_OriginalValue( lIndex, &vaOrgVal );
				Fix_BSTR_get_Value( p, lIndex, &vaCurrentVal );
				if( vaOrgVal != vaCurrentVal )
					bChanged = true;
			}
		}
	}
	return bChanged;
}

// 形状に変更があるかどうかチェックする
bool CRowBase::ShapeChanged() const
{
	// 作成、削除は変更されたと見なす
	if( IsDeleted() || IsCreated() )
		return true;

	// 実体が無いものは変更されたと見なす
	if( ! p )
		return true;

	// キャッシュがある場合
	if( m_pGeom )
	{
		IFeaturePtr ipFeat;
		if( SUCCEEDED( p->QueryInterface( IID_IFeature, (void**)&ipFeat ) ) )
		{
			IGeometryPtr ipOrg;
			LOGASSERTE_IF( SUCCEEDED( ipFeat->get_Shape( &ipOrg ) ), sindyErr_GeometryNotFound )
			{
				// NULLが来る場合があるのでチェック
				if( (IGeometry*)*m_pGeom )
				{
					if( g_bShapeChanged_UseRelationalOperator )
					{
						IRelationalOperatorPtr ipRelOpe( ipOrg );
						VARIANT_BOOL vbEqual = VARIANT_FALSE;
						ipRelOpe->Equals( (IGeometry*)*m_pGeom, &vbEqual );
						return ! VB2bool(vbEqual);
					}
					else {
						if( ! m_pGeom->Equals( ipOrg ) )
							return true;
					}
				}
			}
		}
	}
	// キャッシュが無い場合
	else {
		IFeatureChangesPtr ipFeatureChanges( (IFeaturePtr)p );
		if( ipFeatureChanges )
		{
			VARIANT_BOOL vbChanged = VARIANT_FALSE;
			ipFeatureChanges->get_ShapeChanged( &vbChanged );
			if( vbChanged )
				return true;
		}
	}
	return false;
}

// キャッシュがある場合にそのキャッシュをコピーせずに取得する
const VARIANT* CRowBase::GetCacheValue( long index ) const { return ( m_pCache ? (*m_pCache)[index] : (const VARIANT*)NULL ); }
VARIANT* CRowBase::GetCacheValue( long index ){ return ( m_pCache ? (*m_pCache)[index] : (VARIANT*)NULL ); }
const VARIANT* CRowBase::GetValuePointer( long index, CComVariant& va ) const
{
	const VARIANT* ret = GetCacheValue(index);
	if( ! ret ) {
		if( sindyErr_NoErr != GetValue( index, va ) )
			SASSERT(false); // 値取得に失敗
		else
			ret = &va;
	}
	return ret;
}
VARIANT* CRowBase::GetValuePointer( long index, CComVariant& va )
{
	VARIANT* ret = GetCacheValue(index);
	if( ! ret ) {
		if( sindyErr_NoErr != GetValue( index, va ) )
			SASSERT(false); // 値取得に失敗
		else
			ret = &va;
	}
	return ret;
}

// CComVariantを返すGetValue
CComVariant CRowBase::GetValue( long lIndex ) const 
{
	CComVariant va;
	return *GetValuePointer( lIndex, va );
}


// インデックス番号でbool値を取得する
bool CRowBase::GetValue( long index, bool bDefaultValue ) const
{
	CComVariant va;
	const VARIANT* pVal = GetValuePointer( index, va );

	if( ! pVal )
		return bDefaultValue;

	switch( pVal->vt )
	{
	case VT_I4: bDefaultValue = ( pVal->lVal == 0 )? false : true;	break;
	case VT_NULL: break;
	default: SASSERT( pVal->vt == VT_I4 || pVal->vt == VT_NULL ); break;
	}

	return bDefaultValue;
}

// インデックス番号でbool値をセットする
sindyErrCode CRowBase::SetValue( long index, bool bNewValue )
{
	return SetValue( index, ( bNewValue ? 1L : 0L ) );
}

// インデックス番号でlong値を取得する
long CRowBase::GetValue( long index, long lDefaultValue ) const
{
	CComVariant va;
	const VARIANT* pVal = GetValuePointer( index, va );

	if( ! pVal )
		return lDefaultValue;

	switch( pVal->vt )
	{
	case VT_I2: return pVal->iVal; break;
	case VT_I4: return pVal->lVal; break;
	case VT_NULL: return lDefaultValue; break;
	default: SASSERT( pVal->vt == VT_I4 || pVal->vt == VT_NULL ); break;
	}

	return lDefaultValue;
}

// インデックス番号でlong値をセットする
sindyErrCode CRowBase::SetValue( long index, long lNewValue )
{
	VARIANT* pVal = GetCacheValue( index );
	if( pVal )
	{
		VariantClear( pVal );
		pVal->vt = VT_I4;
		pVal->lVal = lNewValue;

		return sindyErr_NoErr;
	}
	else
		return SetValue( index, CComVariant(lNewValue) );
}

sindyErrCode CRowBase::SetValue( long index, LPCTSTR lpcszNewValue )
{
	VARIANT* pVal = GetCacheValue( index );
	if( pVal )
	{
		VariantClear( pVal );
		pVal->vt = VT_BSTR;
		pVal->bstrVal = ::SysAllocString(lpcszNewValue);

		return sindyErr_NoErr;
	}
	else
		return SetValue( index, CComVariant(lpcszNewValue) );
}

// インデックス番号でdouble値を取得する
double CRowBase::GetValue( long index, const double& dDefaultValue ) const
{
	CComVariant va;
	const VARIANT* pVal = GetValuePointer( index, va );

	if( ! pVal )
		return dDefaultValue;

	switch( pVal->vt )
	{
	case VT_R4: return pVal->fltVal; break;
	case VT_R8: return pVal->dblVal; break;
	case VT_NULL: break;
	default: SASSERT( pVal->vt == VT_R8 || pVal->vt == VT_NULL ); break;
	}

	return dDefaultValue;
}

// インデックス番号でdouble値をセットする
sindyErrCode CRowBase::SetValue( long index, const double& dNewValue )
{
	VARIANT* pVal = GetCacheValue( index );
	if( pVal )
	{
		VariantClear( pVal );
		pVal->vt = VT_R8;
		pVal->dblVal = dNewValue;

		return sindyErr_NoErr;
	}
	else
		return SetValue( index, CComVariant(dNewValue) );
}

CString CRowBase::GetValue( long index, LPCTSTR lpcszDefaultValue ) const
{
	CComVariant va;
	const VARIANT* pVal = GetValuePointer( index, va );

	if( ! pVal )
		return lpcszDefaultValue;

	switch( pVal->vt )
	{
	case VT_NULL:
		return _T("");
		// fall through
	case VT_BSTR:
		return pVal->bstrVal;
		// fall through
	default:
		{
			CComVariant vaCpy;
			vaCpy.ChangeType( VT_BSTR, pVal );
			return vaCpy.bstrVal;
			// fall through
		}
	}
}

// 指定されたフィールドがデフォルト値かどうかをチェックする
bool CRowBase::IsDefaultValue( LPCTSTR lpcszFieldName ) const
{
	const CFieldMap& cFields( *GetFieldMap() );
	long lIndex = cFields.FindField( lpcszFieldName );
	if( 0 > lIndex )
	{
		_ASSERTE( 0 <= lIndex );
		return false;
	}
	CComVariant vaValue, vaDefault = cFields.GetDefaultValue(lIndex);
	GetValue( lIndex, vaValue );

	// 本来の型に戻してチェック
	VARENUM emType = VT_ERROR;
	switch( cFields.GetFieldType(lIndex) )
	{
	case esriFieldTypeSmallInteger:
	case esriFieldTypeInteger:
		emType = VT_I4;
		break;
	case esriFieldTypeSingle:
	case esriFieldTypeDouble:
		emType = VT_R8;
		break;
	case esriFieldTypeString:
		emType = VT_BSTR;
		// 空文字->NULLにする
		if( vaValue.vt == VT_BSTR && 0 == wcslen(vaValue.bstrVal) )
		{
			vaValue.Clear();
			vaValue.vt = VT_NULL;
		}
		break;
	case esriFieldTypeDate:
		emType = VT_DATE;
		break;
	default:
		break;
	}
	if( vaValue.vt != vaDefault.vt )
	{
		vaValue.ChangeType(emType);
		vaDefault.ChangeType(emType);
	}

	return ( vaValue == vaDefault );
}

// 変更前の値を取得する
CComVariant CRowBase::GetOriginalValue( LPCTSTR lpcszFieldName ) const
{
	return GetOriginalValue( GetFieldMap()->FindField( lpcszFieldName ) );
}

CComVariant CRowBase::GetOriginalValue( long lFieldIndex ) const
{
	CComVariant vaRet; // 戻り値
	const CFieldMap& cFields( *GetFieldMap() );

	// チェック
	if( ! ( 0 <= lFieldIndex && (ULONG)lFieldIndex < cFields.GetFieldCount() ) )
	{
		_ASSERTE( 0 <= lFieldIndex && (ULONG)lFieldIndex < cFields.GetFieldCount() ); // フィールドインデックス範囲外
		return vaRet;
	}
	if( ! p )
	{
		_ASSERTE( p ); // 実体が無ければならない
		return vaRet;
	}

	Fix_BSTR_get_Value( p, lFieldIndex, &vaRet );

	return vaRet;
}

CString CRowBase::GetDomainName( LPCTSTR lpszFiedlName ) const
{
	long index = this->FindField(lpszFiedlName);
	std::map<long, CString> domainMap = this->GetFieldMap()->GetFieldAttr(index).rdomain;
	CString value = this->GetValue(index, _T(""));
	// コード値ドメインでなければ値をそのまま返す
	// ドメイン定義外の値であればそのまま返す
	if(domainMap.empty() || domainMap.find(_ttol(value)) == domainMap.end())
		return value;

	return domainMap[_ttol(value)];
}

CGeometryBase* CRowBase::GetShape()
{
	return m_pGeom;
}

const CGeometryBase* CRowBase::GetShape() const
{
	return m_pGeom;
}

sindyErrCode CRowBase::SetShape( IGeometry* ipGeom )
{
	if( ! ipGeom )
		return sindyErr_ArgIsNull;

	if( m_pGeom )
		(*m_pGeom) = ipGeom;
	else {
		IFeaturePtr ipFeat( p );
		if( ipFeat )
		{
			LOGASSERTEM_IF( SUCCEEDED( ipFeat->putref_Shape( (IGeometryPtr)AheCreateClone(ipGeom) ) ), sindyErr_AOErr, _T("%s"), FAIL_PUTREF_SHAPE )
			{
			}
		}
		// INFやLQ等フィーチャじゃないものでも形状は必要なこともあるので
		else
			m_pGeom = new CGeometryBase( ipGeom );
	}

	return sindyErr_NoErr;
}

// 現在格納されているフィーチャの形状のコピーを返す
IGeometryPtr CRowBase::GetShapeCopy( ShapeMode emMode/* = enumLatest*/ ) const
{
	IGeometryPtr ipGeom;	// 返り値

	if( emMode & enumLatest )
	{
		if( m_pGeom )
			ipGeom = AheCreateClone( (IGeometry*)(*this->GetShape()) );
		else if( p ) {
			// キャッシュしていない場合はキャッシュを作らないようにげっとする
			IFeaturePtr ipFeature;
			if( SUCCEEDED( QueryInterface( &ipFeature ) ) )
			{
				SUCCEEDED_IF( ipFeature->get_ShapeCopy( &ipGeom ), sindyErr_GeometryNotFound, MAY_BE_NULL_SHAPE )
				{
				}
			}
		}
	}

	if( p )
	{
		IFeaturePtr ipFeature;
		if( SUCCEEDED( QueryInterface( &ipFeature ) ) )
		{
			if( (emMode & enumOriginal) || ( ipGeom == NULL && IsCached() ) )
			{
				IGeometryPtr ipGeomTmp;
				SUCCEEDED_IF( ipFeature->get_ShapeCopy( &ipGeomTmp ), sindyErr_GeometryNotFound, MAY_BE_NULL_SHAPE )
				{
					// [Bug 6065] ipGeomがNULL（bLatestがfalseでbOriginalがtrueの時に）でipGeomTmpもNULLの時にエラーログが出力されるので回避。
					if ( ipGeom ) {
						if ( ! (emMode & enumDiff) )
							ipGeom = CGeometryBase::Union( ipGeom, ipGeomTmp );
						else
							ipGeom = CGeometryBase::SymmetricDifference( ipGeom, ipGeomTmp );
					} else
						ipGeom = ipGeomTmp;
				}
			}
			if( emMode & enumFeatureChangedOriginal )
			{
				IGeometryPtr ipGeomTmp;
				IFeatureChangesPtr ipChanges( ipFeature );
				LOGASSERTE_IF( SUCCEEDED( ipChanges->get_OriginalShape( &ipGeomTmp ) ), sindyErr_GeometryFunctionFailed )
				{
					// [Bug 6065] ipGeomがNULL（bLatestがfalse時に）でipGeomTmpもNULLの時にエラーログが出力されるので回避。
					if ( ipGeom ) {
						if ( ! (emMode & enumDiff) )
							ipGeom = CGeometryBase::Union( ipGeom, ipGeomTmp );
						else
							ipGeom = CGeometryBase::SymmetricDifference( ipGeom, ipGeomTmp );
					} else
						ipGeom = AheCreateClone( ipGeomTmp );
				}
			}
		}
	}
	return ipGeom;
}

// 現在格納されているフィーチャの存在矩形を返す
IEnvelopePtr CRowBase::GetEnvelope( ShapeMode emMode/* = enumLatest*/ ) const
{
	IEnvelopePtr ipRet;

	// CRow内での変更形状矩形
	if( (emMode & enumLatest) && m_pGeom )
		ipRet = m_pGeom->GetEnvelope();

	// オリジナル
	if( emMode & enumOriginal )
	{
		IFeaturePtr ipFeat( p );
		if( ipFeat )
		{
			IEnvelopePtr ipEnv;
			IGeometryPtr ipGeom;
			ipFeat->get_Shape( &ipGeom );
			if( ipGeom )
			{
				ipGeom->get_Envelope( &ipEnv );
				if( ipEnv )
				{
					if( ipRet )
					{
						LOGASSERTE_IF( SUCCEEDED( ipRet->Union( ipEnv ) ), sindyErr_GeometryFunctionFailed );
					}
					else
						ipRet = ipEnv;
				}
			}
		}
	}

	// IFeatureChanged::OriginalShape
	if( emMode & enumFeatureChangedOriginal )
	{
		IFeatureChangesPtr ipChanged( p );
		if( ipChanged )
		{
			VARIANT_BOOL vbChanged = VARIANT_FALSE;
			ipChanged->get_ShapeChanged( &vbChanged );
			if( vbChanged )
			{
				IEnvelopePtr ipEnv;
				IGeometryPtr ipGeom;
				ipChanged->get_OriginalShape( &ipGeom );
				if( ipGeom )
				{
					ipGeom->get_Envelope( &ipEnv );
					if( ipEnv )
					{
						if( ipRet )
						{
							LOGASSERTE_IF( SUCCEEDED( ipRet->Union( ipEnv ) ), sindyErr_GeometryFunctionFailed );
						}
						else
							ipRet = ipEnv;
					}
				}
			}
		}
	}

	return ipRet;
}

// 整合性をチェックする
sindyErrCode CRowBase::CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrs ) const
{
	CErrorInfosBase infos;
	CheckLogic( cRule, infos );
	// エラーが存在するなら追加
	if( ! infos.empty() )
	{
		CSPErrorObjectBase cErr( new CErrorObjectBase( *this ) );
		cErr->push_back( infos );
		cErrs.push_back( cErr );
	} 
	return cErrs.GetDefaultErrCode();
}

void CRowBase::CheckLogic( const CLogicRule& cRule, CErrorInfosBase& infos ) const
{
	// CLogicRuleにサポート追加したので
	if( g_bDisableCheckToNoSubstance )
	{
		_ASSERTE( ! g_bDisableCheckToNoSubstance );
		const_cast<CLogicRule&>(cRule).SetDisableCheckToNoSubstance( g_bDisableCheckToNoSubstance );
	}

	if( cRule.GetDisableCheckToNoSubstance() )
	{
		// 実体がないもの（新規作成されたものを除く）は
		// チェック対象から外す
		if( ! ( p || m_bCreated ) )
			return;
	}

	// 自分自身のチェック
	bool bIsSDE = GetNameString()->IsSDE(); // 文字列フィールドはPGDBとSDEで型が違うため、どっちなのかを判断する
	const CFieldMap& cFields( *GetFieldMap() );

	bool bChangedAttr = false;  // 属性が変更されたかどうか
	bool bChangedShape = false; // 形状が変更されたかどうか

	// 削除されていない場合に限り属性・形状チェックを行う
	if( ! IsDeleted() )
	{
		// 属性チェック
		int nIndex = 0;
		for( CFieldMap::const_iterator it = cFields.begin(), itEnd = cFields.end(); it != itEnd; ++it, ++nIndex )
		{
			if( ! it->editable )
				continue;

			CComVariant vaValue, vaOrg;
			GetValue( nIndex, vaValue );
			if( VT_EMPTY == vaValue.vt )
				continue; // フィールドが取得制限されている場合

			bool bChangedField = ( IsCreated() || ( ! p ) ) ? true : false; // この時点でDetachされているものは変更前の値が取れないので新規と同じ扱いにする
			if( ! bChangedField )
			{
				 vaOrg = GetOriginalValue( nIndex );
				 bChangedField = !( vaValue == vaOrg );

				 if( bChangedField )
					 bChangedAttr = true;
			}
			CheckLogic_Field( cRule, infos, nIndex, *it, vaValue, vaOrg, bChangedField, bIsSDE );
		}

		// 形状チェック（TODO: ChangedShape関数として起こすべき）
		IGeometryPtr ipOrgShape;
		IFeatureChangesPtr ipFeat( p );
		if( ipFeat || GetShape() ) // フィーチャかどうか
		{
			if( IsCreated() || ( ! p ) ) // // この時点でDetachされているものは変更前の値が取れないので新規と同じ扱いにする
				bChangedShape = true;
			else {
				if( ipFeat )
					ipFeat->get_OriginalShape( &ipOrgShape );

				// 形状キャッシュがない場合
				if( ! GetShape() )
				{
					VARIANT_BOOL vbChanged = VARIANT_FALSE;
					if( ipFeat )
						ipFeat->get_ShapeChanged( &vbChanged );
					bChangedShape = VB2bool(vbChanged);
				}
				else {
					if( ipOrgShape )
						bChangedShape = ( ! m_pGeom->Equals( ipOrgShape ) );
				}
			}

			if( ( ! cRule.GetDisableCheckNoChange() ) || 
				( cRule.GetDisableCheckNoChange() && bChangedShape ) )
			{
				const CGeometryBase cGeom( (IGeometry*)*this );
				if( (IGeometry*)cGeom )
				{
					IEnvelopePtr ipChkEnv;

					// ポリゴンかつ1000点以上構成点をもつもののみ変更領域取得
					if ( cGeom.GetGeometryType() == esriGeometryPolygon  ) {
						if ( cGeom.GetPointCount() > 1000 ) {
							// 変更領域取得
							IGeometryPtr ipEdit = GetShapeCopy( (ShapeMode)(enumAll|enumDiff) );

							if ( ipEdit )
								ipEdit->get_Envelope( &ipChkEnv );
						}
					}

					// セグメントチェック
					cGeom.CheckLogic( cRule.GetBasicRule( GetTableName(), GetNameString()->GetUserName(), CBasicLogicRule() ), infos, ipChkEnv );
				}

			}
		}
	}

	// 変更タイプを特定
	schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;
	if( IsDeleted() )
		emUpdateType = schema::ipc_table::update_type::kDeleted;
	else if( IsCreated() )
		emUpdateType = schema::ipc_table::update_type::kCreated;
	else if( bChangedShape )
		emUpdateType = schema::ipc_table::update_type::kShapeUpdated;
	else if( bChangedAttr )
		emUpdateType = schema::ipc_table::update_type::kAttrUpdated;


	if( cRule.GetUseEditableRule() && // 編集ルールを使用する場合のみに限定
		schema::ipc_table::update_type::kDefault != emUpdateType )
	{
		// 編集可不可チェック
		sindyeEditType emEditType = (sindyeEditType)-1;

		switch( emUpdateType )
		{
		case schema::ipc_table::update_type::kCreated: emEditType = sindyeEditCreate; break;
		case schema::ipc_table::update_type::kDeleted: emEditType = sindyeEditDelete; break;
		case schema::ipc_table::update_type::kShapeUpdated: emEditType = sindyeEditMove; break;
		case schema::ipc_table::update_type::kAttrUpdated: emEditType = sindyeEditAttribute; break;
		default: break;
		}

		if( -1 < (int)emEditType )
			CheckEditable( emEditType, infos, cRule.GetDoCheckModify() );
	}
}

// フィールド単位のチェックをする
void CRowBase::CheckLogic_Field( const CLogicRule& cRule, CErrorInfosBase& cErrs, long lIndex, const CFieldMap::_FIELD& cFieldAttr, const CComVariant& vaValue, const CComVariant& vaOrg, bool bChanged, bool bIsSDE ) const
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 以下のチェックは全てデータベースに入れられるかどうかのチェックなので全て変更があるときのみでよい
	// 変更のある・なしに関わらず行う場合はこれより上に入れること！！！
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	if( ! bChanged )
		return;

	// [CHECK] NOT NULLチェック（bug 6434）
	if( ! cFieldAttr.nullable )
	{
		if( VT_NULL == vaValue.vt || 
			VT_BSTR == vaValue.vt && 0 >= ::SysStringLen(vaValue.bstrVal) )
			cErrs.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_SetNullValueToNotNullField, sindyErrLevel_ERR, GetShapeCopy(), lIndex, -1, NULL_CANNOT_SET, cFieldAttr.name ) ) );
	}

	switch( cFieldAttr.type )
	{
	case esriFieldTypeString:
		{
			// [CHECK] 文字列フィールドあふれの検出
			if( VT_BSTR == vaValue.vt )
			{
				// 終端文字は入らない
				// [Bug 6129] City_Adminの構成点が削除できない。（PGDBのみ）
				// PGDBの時のit->lengthは文字数。
				size_t lByte = bIsSDE ? ( strlen(CW2A(vaValue.bstrVal)) ) : ( (wcslen( vaValue.bstrVal ) ) );
				if( VT_BSTR == vaValue.vt && lByte > cFieldAttr.length )
				{
					CString strValue(vaValue.bstrVal);
					if( 1000 < strValue.GetLength() )
						cErrs.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_FieldLengthLimitOver, sindyErrLevel_ERR, (IGeometry*)*this, lIndex, -1, OVER_CHARNUM_LIMIT, cFieldAttr.name, strValue.Mid(0,1000), lByte - cFieldAttr.length ) ) );
					else
						cErrs.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_FieldLengthLimitOver, sindyErrLevel_ERR, (IGeometry*)*this, lIndex, -1, OVER_CHARNUM_LIMIT2, cFieldAttr.name, strValue, lByte - cFieldAttr.length ) ) );
				}
			}
			break;
		}
	default:
		if( VT_NULL != vaValue.vt )
		{
			// [CHECK] 桁数あふれチェック
			if( 0 < cFieldAttr.precision )
			{
				// 整数桁数のみチェックする
				long num = 0;
				{
					CComVariant va( vaValue );
					va.ChangeType( VT_I4 );
					va.lVal = abs(va.lVal);
					while( 0 < va.lVal ){ va.lVal /= 10; ++num; }
				}
				if( cFieldAttr.precision - cFieldAttr.scale < num )
				{
					CComVariant vaValueStr( vaValue );
					vaValueStr.ChangeType( VT_BSTR );
					cErrs.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_FieldPrecisionLimitOver, sindyErrLevel_ERR, (IGeometry*)*this, lIndex, -1, OVER_DIGITNUM_LIMIT, cFieldAttr.name, CString(vaValueStr.bstrVal), cFieldAttr.precision - cFieldAttr.scale ) ) );
				}
			}
			// [CHECK] レンジチェック
			if( VT_EMPTY != cFieldAttr.range.first.vt || VT_EMPTY != cFieldAttr.range.second.vt )
			{
				CComVariant va( vaValue ), vaMin( cFieldAttr.range.first ), vaMax( cFieldAttr.range.second );
				va.ChangeType( VT_R8 ); vaMin.ChangeType( VT_R8 ); vaMax.ChangeType( VT_R8 );
				if( ! ( ( VT_EMPTY == cFieldAttr.range.first.vt  || vaMin.dblVal <= va.dblVal ) &&   // MIN
					( VT_EMPTY == cFieldAttr.range.second.vt || vaMax.dblVal >= va.dblVal ) ) )  // MAX
				{
					CComVariant vaValueStr( vaValue ), vaMin( cFieldAttr.range.first ), vaMax( cFieldAttr.range.second );
					vaValueStr.ChangeType( VT_BSTR ); vaMin.ChangeType( VT_BSTR ); vaMax.ChangeType( VT_BSTR );
					cErrs.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_FieldRangeLimitOver, sindyErrLevel_ERR, (IGeometry*)*this, lIndex, -1, OUT_OF_RANGEDOMAIN, cFieldAttr.name, CString(vaValueStr.bstrVal), CString(vaMin.bstrVal), CString(vaMax.bstrVal) ) ) );
				}
			}
			// [CHECK] ドメインチェック
			if( ! cFieldAttr.rdomain.empty() )
			{
				if( cFieldAttr.rdomain.end() == cFieldAttr.rdomain.find(vaValue.lVal) )
				{
					CComVariant vaValueStr( vaValue );
					vaValueStr.ChangeType( VT_BSTR );
					cErrs.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_FieldDomainLimitOver, sindyErrLevel_ERR, (IGeometry*)*this, lIndex, -1, NON_EXIST_DOMAINCODE, cFieldAttr.name, CString(vaValueStr.bstrVal) ) ) );
				}
			}
		}
		break;
	}
}

// 編集可能かどうかをチェックする
sindyErrCode CRowBase::CheckEditable( sindyeEditType emType, CErrorInfosBase& cErrInfos, bool check_modify/* = true*/, bool err2warn_modify/* = false*/ ) const
{
	CString strMsg;
	sindyeEditableType emEditable = sindyeEditableEditable;
	if( g_cRule )
		emEditable = g_cRule->IsEditable( *this,  emType, strMsg, check_modify, err2warn_modify );

	// EditTypeによってErrCodeを変える（bug 9606）
	sindyErrCode errcode = sindyErr_NoErr;
	switch( emType )
	{
	case sindyeEditCreate: errcode = sindyErr_RuleNotAllowedCreate; break;
	case sindyeEditMove: errcode = sindyErr_RuleNotAllowedMove; break;
	case sindyeEditDelete: errcode = sindyErr_RuleNotAllowedDelete; break;
	case sindyeEditAttribute: errcode = sindyErr_RuleNotAllowedEditAttribute; break;
	default:
		break;
	}
	if( sindyeEditableEditable != emEditable )
		cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( errcode, ( sindyeEditableWarnning == emEditable ) ? sindyErrLevel_WARN : sindyErrLevel_ERR, NULL, -1, -1, strMsg ) ) );

	return cErrInfos.GetDefaultErrCode();
}

// メンバ変数を初期化する
void CRowBase::Init()
{
	m_emTableType = sindyTableType::unknown;
	m_pCache = NULL;
	m_pGeom  = NULL;
	m_lOID   = -1;
	m_bCreated = m_bDeleted = false;
	SetNameString( CSPTableNameString() );
	SetFieldMap( CSPFieldMap() );
}

bool CRowBase::IsSameAttribute(const CRowBase& src, const CSimpleArray<CString>& cExclusions) const
{
	// 接続プロパティ、オーナ名、テーブル名が同じか
	if ( !(src.GetNameString()->IsSameSource( *GetNameString() ) &&
           (0 == _tcscmp(GetTableName(), src.GetTableName())) ) )
		return false;

	std::list<CString> cDefaultExclusions;
	long count = cExclusions.GetSize();
	for( long i = 0; i < count; ++i )
		cDefaultExclusions.push_back( cExclusions[i] );

	// 除外フィールド
	cDefaultExclusions.push_back( schema::road_link::kSource );
	cDefaultExclusions.push_back( schema::road_link::kAdminCode );
	cDefaultExclusions.push_back( schema::inf_uturn::kSourceName );
	
	// 同じテーブルに所属するか
	ITablePtr ipTable = GetTable();
	ITablePtr ipTableSrc = src.GetTable();
	bool bIsSameTable = ipTable && ipTableSrc && ( (ITable*)ipTable == (ITable*)ipTableSrc );

	long i = 0;
	for ( CFieldMap::iterator it = GetFieldMap()->begin(), itEnd = GetFieldMap()->end(); it != itEnd; ++it,++i ) 
	{
		if( std::find( cDefaultExclusions.begin(), cDefaultExclusions.end(), it->name ) != cDefaultExclusions.end() ) // 比較除外フィールド？
			continue;
		
		const CFieldMap& fields = *GetFieldMap();
		if( !fields.IsSiNDYEditable( i ) ) // SiNDY的に編集可能なフィールド？
			continue;
		
		if( AheIsSindycField(fields.GetName( i )) ) // SiNDY-c固有のフィールド？（bug 9779）
			continue;
		
		if( !bIsSameTable )
		{
			if( src.GetFieldMap()->HasField( fields.GetName( i ) ) == -1 ) // 両方に存在する？
				continue;
		}

		CComVariant vaSource, vaDest;

		GetValue( it->name, vaSource );
		src.GetValue( it->name, vaDest );

		if ( vaSource != vaDest )
			return false;
	}

	return true;
}

void CRowBase::Trace( bool bAttribute/* = true*/, bool bGeometry/* = true*/ ) const
{
	CSPTableNameString sp = GetNameString();
	if( sp->GetPathName() )
		ATLTRACE(_T("CON[%s] TABLE[%s] OID[%d]\n"), sp->GetPathName(), sp->GetOwnerTableName(), GetOID() );
	else
		ATLTRACE(_T("CON[%s.%s@%s:%s] TABLE[%s] OID[%d]\n"), sp->GetUserName(), sp->GetVersionName(), sp->GetServerName(), sp->GetInstanceName(), sp->GetOwnerTableName(), GetOID() );

	if( bAttribute )
	{
		CFieldMap& cFields( *GetFieldMap() );
		long lFieldCount = cFields.GetFieldCount();
		for( long i = 0; i < lFieldCount; ++i )
		{
			switch( cFields.GetFieldType(i) )
			{
				case esriFieldTypeInteger:
				case esriFieldTypeSmallInteger:
				case esriFieldTypeSingle:
				case esriFieldTypeDouble:
				case esriFieldTypeString:
				case esriFieldTypeDate:
				{
					CComVariant vaValue;
					GetValue( i, vaValue );
					vaValue.ChangeType(VT_BSTR);
					ATLTRACE(_T("TABLE[%s] OID[%d] : %02d FIELD[%s] VALUE[%s]\n"), GetTableName(), GetOID(), i, cFields.GetName(i), ( VT_NULL == vaValue.vt ) ? _T("<NULL>") : CString(vaValue.bstrVal) );
				}
				default:
					break;
			}
		}
	}

	if( bGeometry )
	{
		if( (IFeaturePtr)p )
		{
			IGeometryPtr ipGeom( GetShapeCopy() );
			CGeometryBase::Trace( ipGeom, _T("形状") );
		}
	}
}

} // namespace sindy
