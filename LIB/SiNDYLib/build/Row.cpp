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
 * @file Row.cpp
 * @brief CRowクラス実装ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "Rule.h"
#include "Row.h"
#include "Table.h"
#include "sindycomutil.h"
#include "TableTypeUtil.h"
#include "GlobalRules.h"
#include "GlobalFunctions.h"
#include "ErrorObject.h"
#include "meshutil.h"
#include "GeometryRule.h"
#include "CacheAttribute.h"
#include "AheNetwork.h"
#include "RuleModel.h"
#include "AheLayerFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace winutil;
using namespace sindy::schema::category;
namespace sindy {
using namespace errorcode;

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR EXIST_WHEN_MIGRATING         = _T("移行時既存");
	LPCTSTR ATTRIBUTE_TOUNSUPPORTED_TYPE = _T("%s.%s : サポートされていない型に対して自動属性付与しようとしました。");
	LPCTSTR NONFEATURE_HAVE_SHAPE_CACHE  = _T("HRESULT[%d:%X] フィーチャではないものが形状キャッシュを持っています");
	LPCTSTR FAILED_TO_ADD_HISTORY        = _T("履歴付与に失敗しました");
	LPCTSTR CREATE_CACHE                 = _T("キャッシュを作成します\n");
	LPCTSTR THERE_MAY_BE_NULL_SHAPE      = _T("NULL Shape の可能性があります");
	LPCTSTR NOT_NULL_FIELD               = _T("[%s:NULL]:NOT NULLフィールド");
	LPCTSTR BYTES_OVER                   = _T("[%s:%s]:%dバイトオーバー");
	LPCTSTR ALLOWABLE_NUM_OF_DIGITS      = _T("[%s:%s]:許容桁数：%d");
	LPCTSTR ALLOWABLE_RANGE              = _T("[%s:%s]:許容範囲：%s～%s");
	LPCTSTR FIELDS_NOT_INITIALIZE        = _T("CRowがキャッシュされているのにm_spFieldsが初期化されていません。");
	LPCTSTR NAMESTRING_NOT_INITIALIZE    = _T("CRowがキャッシュされているのにm_spNameStringが初期化されていません。");
#else
	LPCTSTR EXIST_WHEN_MIGRATING         = _T("Exist when migrating");
	LPCTSTR ATTRIBUTE_TOUNSUPPORTED_TYPE = _T("%s.%s: An attempt was made to automatically add attributes to an unsupported type.");
	LPCTSTR NONFEATURE_HAVE_SHAPE_CACHE  = _T("HRESULT[%d:%X] Non-feature is having the shape cache");
	LPCTSTR FAILED_TO_ADD_HISTORY        = _T("Failed to add history.");
	LPCTSTR CREATE_CACHE                 = _T("Create cache.\n");
	LPCTSTR THERE_MAY_BE_NULL_SHAPE      = _T("There may be a NULL shape");
	LPCTSTR NOT_NULL_FIELD               = _T("[%s:NULL]: NOT NULL field");
	LPCTSTR BYTES_OVER                   = _T("[%s:%s]: %d bytes over");
	LPCTSTR ALLOWABLE_NUM_OF_DIGITS      = _T("[%s:%s]: Allowable number of digits: %d");
	LPCTSTR ALLOWABLE_RANGE              = _T("[%s:%s]: Allowable range: %s to %s");
	LPCTSTR FIELDS_NOT_INITIALIZE        = _T("CRow is having cache, but m_spFields is not initialized.");
	LPCTSTR NAMESTRING_NOT_INITIALIZE    = _T("CRow is having cache, but m_spNameString is not initialized.");
#endif // SINDY_FOR_ENGLISH
}

// 時間計測用
double g_dTotalCacheTime = 0.0;	//!< CRow::CreateCacheにかかる時間の総計

CRow::CRow() : 
  m_emTableType(sindyTableType::unknown),
  m_pCache(NULL),
  m_pGeom(NULL),
  m_lOID(-1),
  m_bCreated(false),
  m_bDeleted(false)

{
	m_spNameString.reset((CTableNameString*)NULL);
	m_spFields.reset((CFieldMap*)NULL);
}

CRow::CRow( const CRow& obj ) :
	m_emTableType(obj.m_emTableType),
	m_spNameString(obj.m_spNameString),
	m_spFields(obj.m_spFields),
	m_lOID(obj.m_lOID),
	m_bCreated(obj.m_bCreated),
	m_bDeleted(obj.m_bDeleted)
{
	m_pCache = ( obj.m_pCache ) ? new CCacheAttribute(*obj.m_pCache) : NULL;
	m_pGeom = ( obj.m_pGeom ) ? new CGeometry(*obj.m_pGeom) : NULL;
	AtlComPtrAssign( (IUnknown**)&p, (_IRow*)obj ); // Releaseを呼ぶとキャッシュとm_lOIDが初期化されるのでNG
}

CRow::CRow( _IRow* lp, sindyTableType::ECode emTableType/* = sindyTableType::unknown*/, bool bCreated/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ ) :
	m_emTableType(emTableType),
	m_pCache(NULL),
	m_pGeom(NULL),
	m_lOID(-1),
	m_bCreated(bCreated),
	m_bDeleted(false),
	m_spFields(spFields),
	m_spNameString(spNameString)
{
	SetObject( lp, emTableType, bCreated, spFields, spNameString ); \
}

CRow& CRow::operator=( const CRow& obj )
{
	AtlComPtrAssign( (IUnknown**)Release(), (_IRow*)obj ); // Releaseを呼ぶとキャッシュとm_lOIDが初期化されるので先にやる

	m_emTableType = obj.m_emTableType;
	m_spNameString = obj.m_spNameString;
	m_spFields = obj.m_spFields;
	if( m_pCache )
		delete m_pCache;
	m_pCache = ( obj.m_pCache ) ? new CCacheAttribute(*obj.m_pCache) : NULL;
	if( m_pGeom )
		delete m_pGeom;
	m_pGeom = ( obj.m_pGeom ) ? new CGeometry(*obj.m_pGeom) : NULL;
	m_lOID = obj.m_lOID;
	m_bCreated = obj.m_bCreated;
	m_bDeleted = obj.m_bDeleted;

	return *this;
}

CRow& CRow::operator=( _IRow* lp )
{
	m_emTableType = sindyTableType::unknown;
	m_spNameString.reset((CTableNameString*)NULL);
	m_spFields.reset((CFieldMap*)NULL);
	if( m_pCache )
		delete m_pCache;
	m_pCache = NULL;
	if( m_pGeom )
		delete m_pGeom;
	m_pGeom = NULL;
	m_lOID = -1;
	m_bCreated = false;
	m_bDeleted = false;
	AtlComPtrAssign( (IUnknown**)Release(), lp );

	return *this;
}

bool CRow::operator <( const CRow& obj )
{
	if( p && obj.p )
		return ( GetOID() < obj.GetOID() && *GetNameString() < *obj.GetNameString() );
	return false;
}

// フィーチャ・レコードをセットする
void CRow::SetObject( _IRow* lp, sindyTableType::ECode emTableType/* = sindyTableType::unknown*/, bool bCreated/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNames/* = CSPTableNameString()*/ )
{
	// 既存のものは捨てる
	DeleteCache();
	// メンバ変数初期化
	Init();

	// attach
	AtlComPtrAssign((IUnknown**)&p, lp);
	m_emTableType = emTableType;
	m_bCreated = bCreated;	// 作成フラグ
	m_spFields = spFields;
	m_spNameString = spNames;
}

// ダミーCRowを実体化させる
void CRow::Materialization( _IRow* lp )
{
	_ASSERTE( ! p );        // 実体があるものに対して処理しようとしている
	_ASSERTE( m_bCreated ); // 新規作成フラグがないものに対して処理しようとしている

	AtlComPtrAssign((IUnknown**)&p, lp);
}

errorcode::sindyErrCode CRow::Materialization( ITable* ipTable )
{
	errorcode::sindyErrCode emErr = errorcode::sindyErr_NoErr; // 戻り値

	if ( !m_bDeleted ) {
		if( ! p && ipTable )
		{
			// 実体化
			HRESULT hr = S_OK;
			if( SUCCEEDED( hr = ipTable->CreateRow( &p ) ) )
			{
			}
			else
				emErr = sindyErr_AOCreateRowFailed;
		}
		else
			emErr = sindyErr_AOCreateRowFailed;
	}
	else
		emErr = sindyErr_AOCreateRowFailed;

	return emErr;
}

errorcode::sindyErrCode CRow::Materialization( ITable* ipTable, ITable* ipSubTable )
{
	errorcode::sindyErrCode emErr = errorcode::sindyErr_NoErr; // 戻り値

	if ( errorcode::sindyErr_NoErr == (emErr = Materialization( ipTable )) ) {
		for ( CContainer::rows_iterator it = begin(CTable(ipSubTable).GetTableType()), itEnd = end(CTable(ipSubTable).GetTableType()); it != itEnd; ++it )
			emErr = (*it)->Materialization( ipSubTable );
	}

	return emErr;
}

// フィーチャのコピーを作成する
CRow CRow::Clone() const
{
	CRow cRow;	// 返り値

	LOGASSERTE_IF( p, sindyErr_ClassMemberMissing )
	{
		// 編集開始されているかどうか
		IWorkspaceEditPtr ipWork( GetWorkspace() );
		VARIANT_BOOL vbIsBeingEdited = VARIANT_FALSE;
		ipWork->IsBeingEdited( &vbIsBeingEdited );
		LOGASSERTE_IF( vbIsBeingEdited, sindyErr_EditNotStarted )
		{
			// 作成
			ITablePtr ipTable( GetTable() );
			LOGASSERTE_IF( SUCCEEDED( ipTable->CreateRow( cRow.Release() ) ), sindyErr_RowCreateFailed )
			{
				cRow.m_bCreated = true;	// 作成フラグを立てておく（履歴書き込みの為）

				// テーブル名キャッシュ、フィールド名対応表コピー
				cRow.m_emTableType = m_emTableType;
				cRow.m_spFields = m_spFields;
				cRow.m_spNameString = m_spNameString;
				// 属性・形状をコピー
				cRow.m_pCache = new CCacheAttribute( ( m_pCache ) ? *m_pCache : (_IRow*)*this );
				cRow.m_pGeom = new CGeometry( GetShapeCopy() );
			}
		}
	}
	return cRow;
}

// フィーチャの変更を元に戻す
void CRow::Reset( bool bRecursive/* = false*/ )
{
	// CContainer
	if( bRecursive )
		CContainer::Reset( bRecursive );

	// 削除フラグがクリアされない条件
	// ・pが削除済みの場合
	// ・削除フラグが立っており、pが存在しない
	//
	// 上記以外は最終的に作成・削除フラグを初期化する
	// 
	// DummyRowにしなければならない条件
	// ・削除フラグが立っておらず、pが存在しない

	// pが削除済みかどうかはOJBECTIDを取って、エラーが出るかどうかで確認
	bool bAlreadyDeleted = false;
	if( p )
	{
		CComVariant vaValue;
		if( FAILED( p->get_Value( 1, &vaValue ) ) )
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
	// CreateDummyRowでやっている属性セットを行う
	// pが存在しない場合は存在しなかったことにしなければ
	// ならないので、削除フラグを立てておく
	if( m_bCreated )
	{
		if( p )
		{
			// CTable::CreateDummyRow相当の
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

sindyTableType::ECode CRow::GetTableType() const
{
	if( sindyTableType::unknown == m_emTableType )
		m_emTableType = AheTableName2TableType( GetNameString()->GetTableName() );

	return m_emTableType;
}

void CRow::Delete( bool bRecursive/* = false*/ )
{
	m_bDeleted = true;	// 削除済みフラグ付与

	if( bRecursive )
	{
		// 内包ルールのものは削除する
		for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
			itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
			itContain != itContainEnd; ++itContain )
			CContainer::Delete( itContain->first, bRecursive );
		// テーブルタイプがotherのものは問答無用で削除（SiNDY-iでは全てotherで入っているので…）
		for( rows_iterator itRows = begin(sindyTableType::other), itRowsEnd = end(sindyTableType::other); itRows != itRowsEnd; ++itRows )
			(*itRows)->Delete( bRecursive );
	}
}

IFieldsPtr CRow::GetFields() const
{
	IFieldsPtr ipFields;
	INVOKEMETHOD( get_Fields( &ipFields ) );
	return ipFields;
}

bool CRow::HasOID() const
{
	// m_lOIDが-1の場合は未取得なので、実際に調べる
	if( -1 == m_lOID )
	{
		VARIANT_BOOL vbHasOID = VARIANT_FALSE;
		INVOKEMETHOD( get_HasOID( &vbHasOID ) );
		return VB2bool(vbHasOID);
	}
	else
		return ( m_lOID < 0 ) ? false : true;
}

long CRow::GetOID() const
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

void CRow::SetDefaultValue()
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
				else
				{
					// オペレータ名と作業目的IDがNULLのままだと、新規作成時のCheckLogic()のNOT NULLチェックでエラーになるので回避。
					CComVariant vaValue;

					GetValue( i, vaValue );

					if ( VT_NULL == vaValue.vt )
					{
						if ( i == cFields.HasField( schema::ipc_table::kOperator ) )
							SetValue( i, CComVariant(CComBSTR(EXIST_WHEN_MIGRATING)) );
						else if ( i == cFields.HasField( schema::ipc_table::kPurpose ) )
							SetValue( i, CComVariant(0) );
					}
				}
			}
		}
	}
}

// 新規作成時の自動付与属性を付与する
void CRow::SetCreateAutoValue()
{
	// ログインしている場合には新規作成時の自動付与属性
	if( g_cRule.IsLogined() )
	{
		const creatablelayers::CCreatableLayers* pCreLayers( g_cRule.GetCreatableLayersTable().Find( g_cRule.GetWorkPurposeID(), g_cRule.GetLayerNamesTable().Find( GetTableName() ) ) );
		if( pCreLayers )
		{
			const CAutoValue& cValue( pCreLayers->GetAutoValue() );
			CSPFieldMap cFields( GetFieldMap() );
			for( CAutoValue::const_iterator it = cValue.begin(); it != cValue.end(); ++it )
			{
				long lFieldIndex = cFields->FindField( it->first );
				if( 0 < lFieldIndex )
				{
					switch( cFields->GetFieldType( lFieldIndex ) )
					{
						case esriFieldTypeInteger:
						case esriFieldTypeSmallInteger: SetLongValue( lFieldIndex, _ttol(it->second) ); break;
						case esriFieldTypeString: SetStringValue( lFieldIndex, it->second ); break;
						default:
							_ERRORLOG( sindyErr_AlgorithmFailed, ATTRIBUTE_TOUNSUPPORTED_TYPE, GetTableName(), it->first );

							break;
					}
				}
			}
		}
	}
}

// キャッシュを反映させる
void CRow::UpdateCache( schema::ipc_table::update_type::ECode& emUpdate, CErrorObjects& cErrs, CErrorObject* pErr )
{
	if( ! p )
		return;

	HRESULT hr = S_OK;

	// 属性
	// キャッシュがある場合はキャッシュを先に反映させる
	if( m_pCache )
	{
		// キャッシュ反映
		CFieldMap& cFields( *GetFieldMap() );
		long lFieldCount = m_pCache->GetCount();
		for( long i = 0; i < lFieldCount; ++i )
		{
			// 編集できないフィールドは飛ばす
			if( cFields.IsEditable( i ) )
			{
				VARIANT& va = *(*m_pCache)[i];
				// EMPTYの場合は編集不可のフィールドか、又はクエリフィルタで省かれたもの
				if( va.vt != VT_EMPTY )
				{
					// 変更がある場合のみStoreする
					_variant_t vaValue;
					if( SUCCEEDED( hr = p->get_Value( i, &vaValue ) ) )
					{
						if( vaValue != va )
						{
							if( SUCCEEDED( hr = p->put_Value( i, va ) ) )
								emUpdate = schema::ipc_table::update_type::kAttrUpdated;
							else {
								CComVariant vaErr( va );
								vaErr.ChangeType(VT_BSTR);
								if( VT_NULL == vaErr.vt )
									vaErr = _T("NULL");
								if( ! pErr )
									pErr = &cErrs.push_back( CErrorObject( *this ) );
								pErr->push_back( CErrorInfo( sindyErr_AOPutValueFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X] FIELD[%s] VALUE[%s]"), hr, hr, cFields.GetName(i), (LPCTSTR)CString(vaErr.bstrVal) ) );
								Trace( true, false );
							}
						}
					}
					else {
						if( ! pErr )
							pErr = &cErrs.push_back( CErrorObject( *this ) );
						pErr->push_back( CErrorInfo( sindyErr_AOGetValueFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) );
					}
				}
			}
		}
	}
	// 形状
	if( m_pGeom )
	{
		IFeaturePtr ipFeat;
		if( SUCCEEDED( hr = p->QueryInterface( IID_IFeature, (void**)&ipFeat ) ) )
		{
			IGeometryPtr ipGeom;
			if( SUCCEEDED( hr = ipFeat->get_Shape( &ipGeom ) ) )
			{
				if( ! m_pGeom->EqualsVertex( ipGeom ) )
				{
					if( SUCCEEDED( hr = ipFeat->putref_Shape( (IGeometry*)*m_pGeom ) ) )
						emUpdate = schema::ipc_table::update_type::kShapeUpdated;
					else {
						if( ! pErr )
							pErr = &cErrs.push_back( CErrorObject( *this ) );
						pErr->push_back( CErrorInfo( sindyErr_AOPutValueFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) );
						AheTraceGeometry( (IGeometry*)*m_pGeom, _T("putref_Shape失敗") );
					}
				}
			}
			else {
				if( ! pErr )
					pErr = &cErrs.push_back( CErrorObject( *this ) );
				pErr->push_back( CErrorInfo( sindyErr_GeometryNotFound, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) );
			}
		}
		else {
			if( ! pErr )
				pErr = &cErrs.push_back( CErrorObject( *this ) );
			pErr->push_back( CErrorInfo( sindyErr_AlgorithmFailed, sindyErrLevel_ERR, NULL, 0, -1, NONFEATURE_HAVE_SHAPE_CACHE, hr, hr ) );
		}
	}
}

sindyErrCode CRow::Store( CErrorObjects& cErrs )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if ( ! p /*&& m_bDeleted*/ )	// 実体がなくて削除されるものはスルー。
		return cErrs.GetDefaultErrCode();

	CErrorObject* pErr = NULL;
	if( g_bCreateAllStoreInfo ) // インポート時には全てのレコード情報をエラーオブジェクトに登録する必要はない（かなり時間がかかる…）
	{
		// エラーオブジェクトに自分自身を登録
		if ( ! p ) {	// 実体がない場合
			CErrorObject cErrObj;

			cErrObj.SetErrorObject( boost::tuple<CString, long, CGeometry>(GetTableName(), -1, NULL) ); 
			pErr = &cErrs.push_back( cErrObj );
		} else
			pErr = &cErrs.push_back( CErrorObject( *this ) );
	}

	// 関連オブジェクトの保存
	if( g_bStoreContainer )
		CContainer::Store( cErrs );

	LOGASSERTEERR_IF( p, sindyErr_ClassMemberMissing )
	{
		// ネットワークリンクかつ削除じゃない時
		if ( AheIsNetworkLink( GetTableName() ) && ! m_bDeleted ) {
			bool bHasDeleted = false;

			// 関連オブジェクトに削除されるものがあるかチェック
			for( CContainer::const_iterator it = begin(); it != end(); ++it )
			{
				for( CContainer::const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
				{
					schema::ipc_table::update_type::ECode emUpdate = schema::ipc_table::update_type::kDefault;

					if ( (*it2)->Changed( &emUpdate ) && emUpdate == schema::ipc_table::update_type::kDeleted ) {
						bHasDeleted = true;
						break;
					}
				}

				if ( bHasDeleted )
					break;
			}

			// 削除されるものがあれば、履歴を付与(クレームフラグを付けるため)
			if ( bHasDeleted )
				SetHistory( schema::ipc_table::update_type::kAttrUpdated );
		}

		schema::ipc_table::update_type::ECode emUpdate = schema::ipc_table::update_type::kDefault;

		// 削除の場合
		HRESULT hr = S_OK;
		if( m_bDeleted )
		{
#ifdef _DEBUG
			long lOID = GetOID(); // 削除されるとできなくなるのでm_lOIDにキャッシュしておく（m_lOIDはDeleteCacheでは消えない）
#else
			GetOID(); // 削除されるとできなくなるのでm_lOIDにキャッシュしておく（m_lOIDはDeleteCacheでは消えない）
#endif // ifdef _DEBUG
			// 削除
			// 保存
			// 新規作成したものをStore()せずに、DeleteするとだめぽなのでStore()しとく
			// ただし、その場合は先にキャッシュを反映させておかないとNULL形状フィーチャ作成
			// レーダーに反応するので、キャッシュを反映（bug 9451）
			if ( m_bCreated )
			{
				UpdateCache( emUpdate, cErrs, pErr ); // 先にキャッシュ反映させてから
				if( SUCCEEDED( hr = p->Store() ), sindyErr_AOStoreFailed )
				{
#ifdef _DEBUG
					CString cStr;
					switch( emUpdate )
					{
						case schema::ipc_table::update_type::kShapeUpdated: cStr = _T("形状変更"); break;
						case schema::ipc_table::update_type::kAttrUpdated:  cStr = _T("属性変更"); break;
						case schema::ipc_table::update_type::kCreated:      cStr = _T("新規作成"); break;
						case schema::ipc_table::update_type::kDeleted:      cStr = _T("削除"); break;
						default: cStr = _T("移行時既存？"); break;
					}
					//TRACEMESSAGE( _T("%s : TABLE[%s] : OID[%d] : 保存完了（%s）\n"), CA2CT(__func__), GetTableName(), GetOID(), cStr );
					ATLTRACE( _T("%s : TABLE[%s] : OID[%d] : 保存完了（%s）\n"), __func__, GetTableName(), GetOID(), cStr );
#endif // ifdef _DEBUG
				}
				else {
					if( ! pErr )
						pErr = &cErrs.push_back( CErrorObject( *this ) );
					pErr->push_back( CErrorInfo( sindyErr_AOStoreFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) );
				}
			}

			// キャッシュ削除
			DeleteCache();

			// [FYI] IsKnownSimpleがFALSEの時は、自動的にSimplifyされるっぽい（同一構成点が無くなるみたい）あとで調べること（Storeしたら再度GetFeatureで取得し直さないとDB内にある形状にならないよ）
			if( SUCCEEDED( hr = p->Delete() ) )
			{
#ifdef _DEBUG
				ATLTRACE( _T("%s : TABLE[%s] : OID[%d] : 削除完了（%s）\n"), __func__, GetTableName(), m_lOID, _T("削除") );
#endif // ifdef _DEBUG
			}
			else {
				if( ! pErr )
					pErr = &cErrs.push_back( CErrorObject( *this ) );
				pErr->push_back( CErrorInfo( sindyErr_AODeleteFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) );
			}
		}
		// 削除以外の場合
		else if( m_pCache || m_pGeom )
			UpdateCache( emUpdate, cErrs, pErr );

		if( m_bCreated ) emUpdate = schema::ipc_table::update_type::kCreated;
		if( m_bDeleted ) emUpdate = schema::ipc_table::update_type::kDeleted;	// 削除を優先

		// キャッシュ削除
		DeleteCache();

		// 新規作成/削除以外はキャッシュ作成前の変更確認（キャッシュがあった場合は二度手間になるけどしょうがない…）
		if( ! ( schema::ipc_table::update_type::kCreated == emUpdate ||
			    schema::ipc_table::update_type::kDeleted == emUpdate ) )
		{
			// 結構時間がかかる処理のようなので、外部フラグで制御
			if( g_bCheckUpdatedByArcObjects ) // キャッシュ作成前の変更をチェックするかどうか
				Changed( &emUpdate );
			else
				emUpdate = schema::ipc_table::update_type::kAttrUpdated; // 強制的にStoreさせる（履歴には属性編集しかはいらないので、SiNDY-e系では高速処理用外部フラグは使用禁止！！）
		}

		if( sindyErr_NoErr == emErr )
		{
			if( schema::ipc_table::update_type::kDefault != emUpdate )
			{
				// 削除は除外しないとまずし。
				if( schema::ipc_table::update_type::kDeleted != emUpdate )
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
							if( sindyErr_NoErr == ( emErr = SetHistory( emUpdate ) ) )
								bStore = true;
							else 
							{
								if( ! pErr )
									pErr = &cErrs.push_back( CErrorObject( *this ) );
								pErr->push_back( CErrorInfo( emErr, sindyErrLevel_ERR, NULL, 0, -1, FAILED_TO_ADD_HISTORY ) );
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
							if( g_bTraceUpdate )
							{
								CString cStr;
								switch( emUpdate )
								{
									case schema::ipc_table::update_type::kShapeUpdated: cStr = _T("形状変更"); break;
									case schema::ipc_table::update_type::kAttrUpdated:  cStr = _T("属性変更"); break;
									case schema::ipc_table::update_type::kCreated:      cStr = _T("新規作成"); break;
									case schema::ipc_table::update_type::kDeleted:      cStr = _T("削除"); break;
									default: cStr = _T("移行時既存？"); break;
								}
								//TRACEMESSAGE( _T("%s : TABLE[%s] : OID[%d] : 保存完了（%s）\n"), CA2CT(__func__), GetTableName(), GetOID(), cStr );
								ATLTRACE( _T("%s : TABLE[%s] : OID[%d] : 保存完了（%s）\n"), __func__, GetTableName(), GetOID(), cStr );
							}
#endif // ifdef _DEBUG
						}
						else {
							if( ! pErr )
								pErr = &cErrs.push_back( CErrorObject( *this ) );
							pErr->push_back( CErrorInfo( sindyErr_AOStoreFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X] %s"), hr, hr, GETCOMERRORSTRING() ) );
							Trace();
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
					ATLTRACE( _T("%s : TABLE[%s] : OID[%d] : 削除\n"), __func__, GetTableName(), GetOID() );
#endif // ifdef _DEBUG
			}
#ifdef _DEBUG
			else if( g_bTraceUpdate )
				ATLTRACE( _T("%s : TABLE[%s] : OID[%d] : 未保存（変更無し）\n"), __func__, GetTableName(), GetOID() );
#endif // ifdef _DEBUG
		}
	} 

	// 最後にCerateフラグはfalseにしておく
	m_bCreated = false;

	return cErrs.GetDefaultErrCode();
}

// コンフリクト回避のための既編集チェックを行う
sindyErrCode CRow::CheckModify( CErrorObjects& cErrs ) const
{
	if ( (_IRow*)p ) {	// 実体があるもののみ
		if ( Changed() ) {
			// 自分自身
			TCHAR strMsg[2048];
			if( g_cRule.IsModified( *this, 2048, strMsg ) )
			{
				CErrorObject cErr( *this );
				cErr.push_back( CErrorInfo( sindyErr_AlreadyModified, sindyErrLevel_ERR, NULL, -1, -1, _T("%s"), strMsg ) );
				cErrs.push_back( cErr );
			}
		}
		// 関連オブジェクト
		CContainer::CheckModify( cErrs );
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

sindyErrCode CRow::SetHistory( schema::ipc_table::update_type::ECode emCode )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( p, sindyErr_ClassMemberMissing )
	{
		const CFieldMap& cFields( *GetFieldMap() );
		long lUpdateTypeIndex =cFields.HasField( schema::ipc_table::kUpdateType );
		if( 0 < lUpdateTypeIndex ) SetLongValue( lUpdateTypeIndex, emCode );					// 更新タイプ
		if( g_cRule.IsLogined() )
		{
			long lOperatorIndex = cFields.HasField( schema::ipc_table::kOperator );
			long lModifyDateIndex = cFields.HasField( schema::ipc_table::kModifyDate );
			long lPurposeIndex = cFields.HasField( schema::ipc_table::kPurpose );
			if( 0 < lOperatorIndex ) SetStringValue( lOperatorIndex, g_cRule.GetOperatorName() );	// オペレータ名
			if( 0 < lModifyDateIndex ) SetStringValue( lModifyDateIndex, GetTime() );				// 最終更新日時
			if( 0 < lPurposeIndex ) SetLongValue( lPurposeIndex, g_cRule.GetWorkPurposeID() );		// 作業目的ID
			// 自動付与属性
			const CAutoValue& cAutoValue( g_cRule.GetAutoValue() );
			for( CAutoValue::const_iterator it = cAutoValue.begin(); it != cAutoValue.end(); ++it )
			{
				long lFieldIndex = cFields.HasField( it->first );
				if( 0 < lFieldIndex ) 
				{
					switch( cFields.GetFieldType(lFieldIndex) )
					{
					case esriFieldTypeInteger:
					case esriFieldTypeSmallInteger: SetLongValue( lFieldIndex, _ttol(it->second) ); break;
					case esriFieldTypeString: SetStringValue( lFieldIndex, it->second ); break;
					default:
						_ERRORLOG( emErr = sindyErr_AlgorithmFailed, ATTRIBUTE_TOUNSUPPORTED_TYPE, GetTableName(), it->first );
						break;
					}
				}
			}
			// PGDB編集モードの場合の更新タイプ
//			if( AheIsPGDBEditMode() ) // モードによらずあれば更新（bug 5777）
			long lUpdateCIndex = cFields.HasField( sindyc_table::kUpdateC );
			if( 0 < lUpdateCIndex )
				SetLongValue( lUpdateCIndex, update_type2update_code( emCode ) | GetLongValue( lUpdateCIndex, 0 ) ); // 更新タイプ（PGDB編集モード用）（bug 5201）
		}
		else {
			long lModifyProgNameIndex = cFields.HasField( schema::ipc_table::kModifyProgName );
			long lProgModifyDateIndex = cFields.HasField( schema::ipc_table::kProgModifyDate );
			if( 0 < lModifyProgNameIndex )
			{
				TCHAR szBuffer[_MAX_PATH];
				::GetModuleFileName(NULL, szBuffer, _MAX_PATH);
				LPCTSTR pos = ::_tcsrchr(szBuffer, _T('\\'));
				SetStringValue( lModifyProgNameIndex,  pos ? pos + 1 : szBuffer );	// 更新プログラム名
			}
			if( 0 < lProgModifyDateIndex ) SetStringValue( lProgModifyDateIndex, GetTime() );				// バッチ処理での最終更新日時
		}
	}

	return emErr;
}

ITablePtr CRow::GetTable() const
{
	ITablePtr ipTable;
	INVOKEMETHOD( get_Table( &ipTable ) );
	return ipTable;
}

IWorkspacePtr CRow::GetWorkspace() const
{
	IWorkspacePtr ipRet;	// 返り値

	IDatasetPtr ipDataset( GetTable() );
	LOGASSERTE_IF( NULL != ipDataset, sindyErr_COMFunctionFailed )
	{
		LOGASSERTE_IF( SUCCEEDED( ipDataset->get_Workspace( &ipRet ) ), sindyErr_AOGetWorkspaceFailed );
	}

	return ipRet;
}

sindyErrCode CRow::GetValue( long lIndex, VARIANT& vaValue ) const
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
			LOGASSERTE_IF( p, sindyErr_ClassMemberMissing )
			{
				HRESULT hr;	// [bug 3182] たまにここでエラーが出るので、調査のために詳細ログを出す
				SUCCEEDED_IF( hr = p->get_Value( lIndex, &vaValue ), emErr = sindyErr_AOGetValueFailed, _T("HRESULT[%ld(0x%X)] TABLE[%s] OID[%ld] lIndex[%ld]"), hr, hr, GetTableName(), GetOID(), lIndex )
				{
				}
			}
//			INVOKEMETHOD( get_Value( lIndex, &vaValue ) );
//			emErr = (FAILED(hr)) ? sindyErr_AOGetValueFailed : sindyErr_NoErr;
		}
	}
	else
		emErr = sindyErr_ArgLimitOver;	// インデックスは0以上

	return emErr;
}

sindyErrCode CRow::SetValue( long lIndex, const VARIANT& vaValue )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if( lIndex >= 0 )
	{
		// キャッシュあり
		if( m_pCache )
		{
			if( FAILED( VariantCopy( (*m_pCache)[lIndex], (VARIANTARG*)&vaValue ) ) )	// constで受け取ってくれないので無理やりキャスト
				emErr = sindyErr_MemoryAllocationFailed;	// 適切な返り値ではないかも
		}
		// キャッシュなし
		else {
			INVOKEMETHOD( put_Value( lIndex, vaValue ) );
			emErr = (FAILED(hr)) ? sindyErr_AOPutValueFailed : sindyErr_NoErr;
		}
	}
	else
		emErr = sindyErr_ArgLimitOver;	// インデックスは0以上

	return emErr;
}

//フィールド対応表の参照を返す
CSPFieldMap CRow::GetFieldMap() const
{
	if( ! m_spFields )
	{
		// キャッシュされていないならメンバを自分で初期化する
		// これをすると、CRow毎にCTableNameStringの実体を持つことになるので
		// 本当は好ましくない。しかし、利便性を重視し、自分で作成できるように
		// しておく。ただし、デバッグトレースは出して開発者にはわかるように
		// しておくこと。
		LOGASSERTEM_IF( ! IsCached(), sindyErr_ClassMemberMissing, FIELDS_NOT_INITIALIZE )
		{
			IFieldsPtr ipFields;
			LOGASSERTE_IF( NULL != ( ipFields = GetFields() ), sindyErr_COMFunctionFailed )
			{
				m_spFields.reset( new CFieldMap( ipFields ) );

				ATLTRACE(_T("%s : CFieldMap cache created.\n"), __func__ );
			}
		}
	}

	return m_spFields;
}

// テーブル名などを管理するメンバの参照を返す
CSPTableNameString CRow::GetNameString() const
{
	if( ! m_spNameString )
	{
		// キャッシュされていないならメンバを自分で初期化する
		// これをすると、CRow毎にCTableNameStringの実体を持つことになるので
		// 本当は好ましくない。しかし、利便性を重視し、自分で作成できるように
		// しておく。ただし、デバッグトレースは出して開発者にはわかるように
		// しておくこと。
		LOGASSERTEM_IF( ! IsCached(), sindyErr_ClassMemberMissing, NAMESTRING_NOT_INITIALIZE )
		{
			ITablePtr ipTable;
			LOGASSERTE_IF( NULL != ( ipTable = GetTable() ), sindyErr_COMFunctionFailed )
			{
				m_spNameString.reset( new CTableNameString( ipTable ) );
#ifdef _DEBUG
				if( m_spNameString.get()->IsSDE() )
				{
					ATLTRACE(_T("%s : CTableNameString cache created : %s.%s[%s]%s@%s:%s\n"), 
						__func__,
						m_spNameString.get()->GetOwnerName(),
						m_spNameString.get()->GetTableName(),
						m_spNameString.get()->GetVersionName(),
						m_spNameString.get()->GetUserName(),
						m_spNameString.get()->GetServerName(),
						m_spNameString.get()->GetInstanceName()
					);
				}
				else {
					ATLTRACE(_T("%s : CTableNameString cache created : %s\\\\%s\n"),
						__func__,
						m_spNameString.get()->GetTableName(),
						m_spNameString.get()->GetPathName()
					);
				}
#endif // ifdef _DEBUG
			}
		}
	}

	return m_spNameString;
}

// テーブル名を返す
LPCTSTR CRow::GetTableName() const
{
	return GetNameString()->GetTableName();
}
/*
// テーブル名を返す
LPCTSTR CRow::GetTableName( LPTSTR lpszName, int nLen ) const
{
	// 引数チェック
	if( nLen > 0 && lpszName != NULL )
	{
		// 初期化
		lpszName[0] = _T('\0');

		CTable cTable( GetTableType(), GetTable() );
		LPCTSTR lpcszName = cTable.GetTableName();
		int nNameLen = lstrlen( lpcszName );
		
		if( nLen > nNameLen )
			lstrcpyn( lpszName, lpcszName, nLen );
		else
			ERRORLOG(sindyErr_BufferIsNotEnough);	// バッファ領域が足りない
	}
	else
		ERRORLOG(sindyErr_ArgLimitOver);

	return lpszName;
};
LPCTSTR CRow::GetTableName( _IRow* ipRow, LPTSTR lpszName, int nLen )
{
	CRow pRow;
	pRow.SetObject( ipRow );

	return pRow.GetTableName( lpszName, nLen );
}

// オーナー名を返す
LPCTSTR CRow::GetOwnerName( LPTSTR lpszName, int nLen ) const
{
	// 引数チェック
	if( nLen > 0 && lpszName != NULL )
	{
		// 初期化
		lpszName[0] = _T('\0');

		CTable cTable( GetTableType(), GetTable() );
		LPCTSTR lpcszName = cTable.GetOwnerName();
		int nNameLen = lstrlen( lpcszName );
		
		if( nLen > nNameLen )
			lstrcpyn( lpszName, lpcszName, nLen );
		else
			ERRORLOG(sindyErr_BufferIsNotEnough);	// バッファ領域が足りない
	}
	else
		ERRORLOG(sindyErr_ArgLimitOver);

	return lpszName;
}
LPCTSTR CRow::GetOwnerName( _IRow* ipRow, LPTSTR lpszName, int nLen )
{
	CRow pRow;
	pRow.SetObject( ipRow );

	return pRow.GetOwnerName( lpszName, nLen );
}


LPCTSTR CRow::GetServerName( LPTSTR lpszName, int nLen ) const
{
	USES_CONVERSION;
	// 引数チェック
	if( nLen > 0 && lpszName != NULL )
	{
		// 初期化
		lpszName[0] = _T('\0');

		CTable cTable( GetTableType(), GetTable() );
		IWorkspacePtr ipWorkspace;
		ipWorkspace = cTable.GetWorkspace();
		if( ipWorkspace != NULL )
		{
			IPropertySetPtr ipPropertySet;
			ipWorkspace->get_ConnectionProperties( &ipPropertySet );
			if( ipPropertySet != NULL )
			{
				CComVariant vaValue;
				ipPropertySet->GetProperty( CComBSTR(_T("SERVER")),&vaValue );
				LPCTSTR lpcszName = OLE2CT( vaValue.bstrVal );
				int nNameLen = lstrlen( lpcszName );
				
				if( nLen > nNameLen )
					lstrcpyn( lpszName, lpcszName, nLen );
				else
					ERRORLOG(sindyErr_BufferIsNotEnough);	// バッファ領域が足りない
			}
		}
	}
	else
		ERRORLOG(sindyErr_ArgLimitOver);

	return lpszName;
}
LPCTSTR CRow::GetServerName( _IRow* ipRow, LPTSTR lpszName, int nLen ) 
{
	CRow pRow;
	pRow.SetObject( ipRow );

	return pRow.GetServerName( lpszName, nLen );
}


LPCTSTR CRow::GetVersionName( LPTSTR lpszName, int nLen ) const
{
	USES_CONVERSION;
	// 引数チェック
	if( nLen > 0 && lpszName != NULL )
	{
		// 初期化
		lpszName[0] = _T('\0');

		CTable cTable( GetTableType(), GetTable() );
		IWorkspacePtr ipWorkspace;
		ipWorkspace = cTable.GetWorkspace();
		if( ipWorkspace != NULL )
		{
			IPropertySetPtr ipPropertySet;
			ipWorkspace->get_ConnectionProperties( &ipPropertySet );
			if( ipPropertySet != NULL )
			{
				CComVariant vaValue;
				ipPropertySet->GetProperty( CComBSTR(_T("VERSION")),&vaValue );
				LPCTSTR lpcszName = OLE2CT( vaValue.bstrVal );
				int nNameLen = lstrlen( lpcszName );
				
				if( nLen > nNameLen )
					lstrcpyn( lpszName, lpcszName, nLen );
				else
					ERRORLOG(sindyErr_BufferIsNotEnough);	// バッファ領域が足りない
			}
		}
	}
	else
		ERRORLOG(sindyErr_ArgLimitOver);

	return lpszName;
}
LPCTSTR CRow::GetVersionName( _IRow* ipRow, LPTSTR lpszName, int nLen ) 
{
	CRow pRow;
	pRow.SetObject( ipRow );

	return pRow.GetVersionName( lpszName, nLen );
}


void CRow::GetInfo( _IRow* ipRow, LPTSTR lpszLayerName, LPTSTR lpszOwnerName, LPTSTR lpszServerName, LPTSTR lpszVersionName)
{
	ITablePtr ipTable;
	IDatasetPtr ipDataset;
	if( ipRow != NULL )
	{
		// 初期化
		lpszLayerName[0] = _T('\0');
		lpszOwnerName[0] = _T('\0');
		lpszServerName[0] = _T('\0');
		lpszVersionName[0] = _T('\0');

		CRow::GetTableName( ipRow, lpszLayerName, 64 );
		CRow::GetOwnerName( ipRow, lpszOwnerName, 64 );
		CRow::GetServerName( ipRow, lpszServerName, 64 );
		CRow::GetVersionName( ipRow, lpszVersionName, 64 );
	}
}
*/
bool CRow::IsEqualObject(_IRow* ipRow ) const
{
	if( p )
	{
		// まずは、OIDのみで比較する（速度優先）
		if( AheGetOID( ipRow ) == GetOID() )
		{
			ITablePtr ipTable;
			ipRow->get_Table( &ipTable );
			CTableNameString cNames( ipTable );
			return ( cNames == *GetNameString() );
		}
	}
	return false;
}

bool CRow::IsEqualObject( const CRow& cRow ) const
{
	if( cRow.p && p ) // DummyRowの場合はpが存在しない
		return ( GetOID() == cRow.GetOID() && *GetNameString() == *cRow.GetNameString() );
	return false;
}

// キャッシュを作成する
sindyErrCode CRow::CreateCache( bool bDetatch/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ )
{
	double dStart = GetTickCount();	// 時間計測用

	if( spNameString )
		m_spNameString = spNameString;
	if( spFields )
		m_spFields     = spFields;

	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	// 既存のキャッシュがない場合にのみ作成
	// 属性のキャッシュ
	if( ! m_pCache )
	{
		// OBJECTIDだけはここで取得
		if( p )
			m_lOID = GetOID();

		// フィールド対応表が無ければ作成
		if( ! m_spFields )
		{
			if( p )
				m_spFields.reset( new CFieldMap( GetFields() ) );
			else
				emErr = sindyErr_ClassMemberMissing;
		}

		// テーブル対応表が無ければ作成
		if( ! m_spNameString )
		{
			if( p )
				m_spNameString.reset( new CTableNameString( GetTable() ) );
			else
				emErr = sindyErr_ClassMemberMissing;
		}

		// 対応表作成でエラーが無ければキャッシュを作成する
		if( m_spFields )
		{
			m_pCache = new CCacheAttribute();
			if( m_pCache )
				emErr = m_pCache->CreateCache( p, *m_spFields );
			else
				emErr = sindyErr_MemoryAllocationFailed;

		}
		else {
			emErr = sindyErr_FieldMapNotCreated;	// フィールド対応表がセットされていない
			ERRORLOG(emErr);
			SASSERT( m_spFields );
		}
	}
	// 形状のキャッシュ
	if( ! m_pGeom )
	{
		if( p ) // pがNULLの場合あり（bug 5421）が、エラーとはしない（CTable::CreateDummyRowのため）
		{
			// フィーチャのときのみの動作とするので、QUERYINVOKEMETHODは使用しない（エラーが出るため）
			IFeaturePtr ipFeature;
			if( SUCCEEDED( p->QueryInterface( IID_IFeature, (void**)&ipFeature ) ) )
			{
				IGeometryPtr ipGeom;
				if( SUCCEEDED( ipFeature->get_Shape( &ipGeom ) ) )
				{
					// フィーチャ作成中に呼ばれた場合は
					// NULL Shapeはありうるが、m_pGeomは作っておく
					// 必要がある
					m_pGeom = new CGeometry( ipGeom );
					m_pGeom->CreateCache( bDetatch );
				}
				else {
					emErr = sindyErr_GeometryNotFound;
					ERRORLOG(emErr);
					SASSERT( false );
				}
			}
		}
	}

	// 切り離し
	if( bDetatch )
	{
		if( p )
		{
			p->Release();
			p = NULL;
		}
	}

/*#ifdef _DEBUG
	if( m_pGeom )
		CGeometry::Trace( *m_pGeom, _T("hoge") );
#endif // ifdef _DEBUG*/

	// キャッシュ時間を計算
	g_dTotalCacheTime += (double)GetTickCount()-dStart;

	// 関連オブジェクトのキャッシュを作成
	CContainer::CreateCache( bDetatch );

	return emErr;
}

// キャッシュを削除する
void CRow::DeleteCache( bool bDeleteContainerCache/* = false*/ )
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
	m_lOID = -1;

	// コンテナのキャッシュを削除
	if( bDeleteContainerCache )
		CContainer::DeleteCache( bDeleteContainerCache );
}

// キャッシュされているかどうかをチェックする
bool CRow::IsCached() const
{
	bool bRet = false;	// 返り値

	// 属性がキャッシュされているかどうか
	if( m_pCache )
		bRet = true;

	// 形状がキャッシュされているかどうか
	if( ! ( m_pGeom && m_pGeom->IsCached() ) )
	{
		if( bRet )
			bRet = false;
	}

	return bRet;
}

// コピーする
sindyErrCode CRow::CopyTo( CRow& dest, bool bIsCopyCache/*=true*/, bool bIsCopyRow/*=true*/, bool bIsCopyFlag/*=true*/, bool bIsCopyContainer/*=true*/ ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if ( bIsCopyRow ) {
		AtlComPtrAssign( (IUnknown**)dest.Release(), (_IRow*)*this );	// IRow(QI)
		// OIDをコピーする
		dest.m_lOID = m_lOID;
	}

	if ( bIsCopyCache ) {
		// 属性をコピーする（既存のキャッシュはdest.Release()で削除されるのでいらない）
		if( m_pCache )
		{
			if ( dest.m_pCache )
				delete dest.m_pCache;

			dest.m_pCache = new CCacheAttribute( *m_pCache );
			if( ! dest.m_pCache ) 
				emErr = sindyErr_MemoryAllocationFailed;
		}
		// 形状をコピーする
		if( m_pGeom )
		{
			if ( dest.m_pGeom )
				delete dest.m_pGeom;

			dest.m_pGeom = new CGeometry( *m_pGeom );
			if( ! dest.m_pGeom )
				emErr = sindyErr_MemoryAllocationFailed;
		}

		if( emErr != sindyErr_NoErr )
		{
			ERRORLOG(emErr);
			SASSERT( emErr != sindyErr_NoErr );
		}
		// フィールド対応表をコピーする
		dest.m_spFields = m_spFields;

		// テーブル名等管理オブジェクトをコピーする
		dest.m_spNameString = m_spNameString;

		// テーブルタイプをコピーする
		dest.m_emTableType = m_emTableType;

		if ( bIsCopyContainer )
			((CContainer&)dest) = (CContainer&)(*this);
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

void CRow::swap( CRow& src, bool bIsCopyCache/*=true*/, bool bIsCopyRow/*=true*/, bool bIsCopyFlag/*=true*/, bool bIsCopyContainer/*=true*/ )
{
	CRow cSwapRow;

	CopyTo( cSwapRow, bIsCopyCache, bIsCopyRow, bIsCopyFlag, bIsCopyContainer );
	src.CopyTo( *this, bIsCopyCache, bIsCopyRow, bIsCopyFlag, bIsCopyContainer );
	cSwapRow.CopyTo( src, bIsCopyCache, bIsCopyRow, bIsCopyFlag, bIsCopyContainer );
}

// IRow*を新しいものに挿げ替える
sindyErrCode CRow::ReplaceNewRow( const CTable* pTargetTable/* = NULL*/, bool bRecursive/* = false*/ )
{
	if( !( p || pTargetTable ) )
		return sindyErr_ClassMemberMissing;

	CTable cTable( *pTargetTable );
	if( ! cTable )
		cTable.SetObject( GetTable(), GetFieldMap(), GetNameString() );

	_IRowPtr ipRow;
	if( FAILED( ((ITable*)cTable)->CreateRow( &ipRow ) ) )
		return sindyErr_AOCreateRowFailed;

	AtlComPtrAssign( (IUnknown**)&p, (IUnknown*)ipRow );
	m_lOID = AheGetOID( ipRow ); // m_lOIDだけは更新しておく

	m_bCreated = true; // 作成フラグを立てる

	if( pTargetTable )
	{
		// テーブルが渡されている場合はオーナー名やフィールド情報が
		// 異なる場合が考えられるため、ここでクリアする必要がある
		// でも、この後フィールド情報やテーブル情報が必要な場合には
		// 各フィーチャで再作成が走ってかなり非効率だよなぁ…。
		// やっぱり、各コンテナでテーブル情報を管理するか、SiNDY-iの
		// ようにテーブルファインダを継承するかしないとだめだよなぁ。。。
		// でもそうすると別ワークスペースを一緒に扱えないしなぁ。。。
		// 困ったなぁ。。。

		if( m_pCache )
		{
			// ここでキャッシュを新しいフィールド順番に並べ替える必要がある
			const CFieldMap& cNewFields( *cTable.GetFieldMap() );
			const CFieldMap& cOldFields( *GetFieldMap() );
			CCacheAttribute* pNewCache = new CCacheAttribute();
			pNewCache->CreateCache( p, cNewFields );
			int nCount = cNewFields.GetFieldCount();
			for( int i = 0; i < nCount; ++i )
			{
				long lIndex = cOldFields.FindField( cNewFields.GetName(i) );
				if( 0 <= lIndex )
					VariantCopy( (*pNewCache)[i], (*m_pCache)[lIndex] ); // VariantCopyはThreadSafeじゃない（MSDN ヘルプより）
			}
			// 属性キャッシュ挿げ替え
			delete m_pCache;
			m_pCache = pNewCache;
		}
		m_spNameString = cTable.GetNameString();
		m_spFields = cTable.GetFieldMap();
	}

	if( ! bRecursive )
		return sindyErr_NoErr;

	// 再帰的に処理する場合
	sindyErrCode emErr = sindyErr_NoErr; // 戻り値

	LPCTSTR lpcszTableName = GetTableName(); // このオブジェクトのテーブル名
	for( iterator it = begin(); it != end(); ++it )
	{
		// コンテナ内のテーブル名を取得しておく
		LPCTSTR lpcszRelationTableName = AheTableType2TableName( it->first );
		// ID関連があるものかどうか調べておく
		CString strOuterKeyFieldName; // 外部キーフィールド名
		for( CRuleRelationTable::const_value_iterator itRule = g_cRuleRelationTable.begin(lpcszTableName);
			itRule != g_cRuleRelationTable.end(lpcszTableName); ++itRule )
		{
			if( 0 == itRule->get<1>().CompareNoCase( lpcszRelationTableName ) )
			{
				strOuterKeyFieldName = itRule->get<2>();
				break;
			}
		}
		// ipTargetTableが指定されている場合は同じテーブル名を持つものを渡す
		ITablePtr ipContainerTargetTable;
		if( pTargetTable )
		{
			ipContainerTargetTable = cTable.OpenSameOwnerTable( lpcszRelationTableName );
			if( ! ipContainerTargetTable )
			{
				emErr = sindyErr_AOGetFeatureClassFailed; // テーブルがオープンできない場合は継続させない
				break;
			}
		}
		for( rows_iterator itRow = it->second.begin(); itRow != it->second.end(); ++itRow )
		{
			CTable cContainerTable( ipContainerTargetTable );
			if( sindyErr_NoErr != ( emErr = (*itRow)->ReplaceNewRow( &cContainerTable, bRecursive ) ) ) // 挿げ替え
				break;
			if( sindyErr_NoErr != ( emErr = (*itRow)->SetLongValueByFieldName(strOuterKeyFieldName, m_lOID) ) ) // 外部キーをセット
				break;
		}
		if( sindyErr_NoErr != emErr )
			break;
	}
	return emErr;
}

// 属性に変更があったかどうかをチェックする

bool CRow::Changed( schema::ipc_table::update_type::ECode* emUpdateType/* = NULL*/ ) const
{
	bool bChanged = false; // 返り値

	if( emUpdateType ) *emUpdateType = schema::ipc_table::update_type::kDefault;	// 引数初期化

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
			long lFieldCount = GetFieldMap()->GetFieldCount();

			// キャッシュがある場合
			if( m_pCache || m_pGeom )
			{
				if( m_pCache )
				{
					CComVariant vaCache, vaValue;

					for( long i = 0; i < lFieldCount; ++i )
					{
						if( GetFieldMap()->IsEditable( i ) )
						{
							// Shapeフィールドはキャッシュも取らないし、比較もできないので飛ばす
							if( GetFieldMap()->IsShapeField( i ) )
								continue;

							vaCache.Clear();
							vaValue.Clear();

							GetValue( i, vaCache );
							INVOKEMETHOD( get_Value(i, &vaValue) );

							// VT_DATEの比較は行えないため、VT_BSTRに変換して比較する
							if( vaCache.vt == VT_DATE )
								vaCache.ChangeType(VT_BSTR);
							if( vaValue.vt == VT_DATE )
								vaValue.ChangeType(VT_BSTR);

							if( vaCache != vaValue )
							{
								bChanged = true;
								if( emUpdateType )
									*emUpdateType = schema::ipc_table::update_type::kAttrUpdated;
								break;
							}
						}
					}
				}
				// 形状のチェック
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
								if( ! m_pGeom->EqualsVertex( ipOrg ) )
								{
									bChanged = true;
									if( emUpdateType )
										*emUpdateType = schema::ipc_table::update_type::kShapeUpdated;
								}
							}
						}
					}
				}
			}
			// キャッシュが無い場合
			else {
				// 属性のチェック
				IRowChangesPtr ipRowChanges( p );
				if( NULL != ipRowChanges )
				{
					VARIANT_BOOL vbChanged = VARIANT_FALSE;
					for( long i = 0; i < lFieldCount; ++i )
					{
						ipRowChanges->get_ValueChanged( i, &vbChanged );
						if( vbChanged )
						{
							bChanged = true;
							if( emUpdateType )
								*emUpdateType = schema::ipc_table::update_type::kAttrUpdated;
							break;	// 一つでもあるならもうチェック終わり
						}
					}
				}
				IFeatureChangesPtr ipFeatureChanges( (IFeaturePtr)p );
				if( NULL != ipFeatureChanges )
				{
					VARIANT_BOOL vbChanged = VARIANT_FALSE;
					ipFeatureChanges->get_ShapeChanged( &vbChanged );
					if( vbChanged )
					{
						bChanged = true;
						if( emUpdateType )
							*emUpdateType = schema::ipc_table::update_type::kShapeUpdated;
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
bool CRow::Changed( LPCTSTR lpcszFieldName ) const
{
	bool bChanged = false; // 返り値

	// キャッシュがある場合にはキャッシュで比較
	if( m_pCache )
	{
		long lIndex = GetFieldMap()->FindField(lpcszFieldName);
		if( GetFieldMap()->IsShapeField( lIndex ) )
			return bChanged;

		LOGASSERTE_IF( lIndex >= 0, sindyErr_AOGetFieldIndexFailed )
		{
			CComVariant vaCache, vaValue;
			GetValue( lIndex, vaCache );
			INVOKEMETHOD( get_Value( lIndex, &vaValue ) );

			// VT_DATEの比較は行えないため、VT_BSTRに変換して比較する
			if( vaCache.vt == VT_DATE )
				vaCache.ChangeType(VT_BSTR);
			if( vaValue.vt == VT_DATE )
				vaValue.ChangeType(VT_BSTR);

			if( vaCache != vaValue )
				bChanged = true;
		}
	}
	else if( p != NULL )
	{
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
			// 形状比較はできないので無視
			if( esriType != esriFieldTypeGeometry )
			{
				IRowChangesPtr ipRowChanges( p );
				CComVariant vaOrgVal, vaCurrentVal;
				ipRowChanges->get_OriginalValue( lIndex, &vaOrgVal );
				p->get_Value( lIndex, &vaCurrentVal );
				if( vaOrgVal != vaCurrentVal )
					bChanged = true;
			}
		}
	}
	return bChanged;
}

// フィールド名でフィールド値を取得する
sindyErrCode CRow::GetValueByFieldName( LPCTSTR lpcszFieldName, VARIANT& vaValue ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if( lpcszFieldName != NULL )
		emErr = GetValue( GetFieldMap()->FindField(lpcszFieldName), vaValue );	// 値取得
	else {
		emErr = sindyErr_ArgIsNull;	// フィールド名がNULL
		ERRORLOG(emErr);
	}

	return emErr;
}

// フィールド名でフィールド値をセットする
sindyErrCode CRow::SetValueByFieldName( LPCTSTR lpcszFieldName, const VARIANT& vaValue )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if( lpcszFieldName != NULL )
		emErr = SetValue( GetFieldMap()->FindField(lpcszFieldName), vaValue );
	else {
		emErr = sindyErr_ArgIsNull;
		ERRORLOG(emErr);
	}

	return emErr;
}

// インデックス番号でbool値を取得する
bool CRow::GetBoolValue( long index, bool bDefaultValue ) const
{
	VARIANT vaValue;
	VariantInit(&vaValue);

	// 引数のチェックはGetValueでするから良い
	if( sindyErr_NoErr == GetValue( index, vaValue ) )
	{
		switch( vaValue.vt )
		{
			case VT_I4: bDefaultValue = ( vaValue.lVal == 0 )? false : true;	break;
			case VT_NULL: break;
			default: SASSERT( vaValue.vt == VT_I4 || vaValue.vt == VT_NULL ); break;
		}
	}
	else
		SASSERT( false );	// 値取得に失敗した

	return bDefaultValue;
}

// インデックス番号でbool値をセットする
sindyErrCode CRow::SetBoolValue( long index, bool bNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_I4;
	vaValue.lVal = ( bNewValue ) ? 1 : 0;

	return SetValue( index, vaValue );
}

// インデックス番号でlong値を取得する
long CRow::GetLongValue( long index, long lDefaultValue ) const
{
	VARIANT vaValue;
	VariantInit(&vaValue);

	// 引数のチェックはGetValueでするから良い
	if( sindyErr_NoErr == GetValue( index, vaValue ) )
	{
		switch( vaValue.vt )
		{
			case VT_I4: break;
			case VT_NULL: vaValue.lVal = lDefaultValue; break;
			default: SASSERT( vaValue.vt == VT_I4 || vaValue.vt == VT_NULL ); vaValue.lVal = lDefaultValue; break;
		}
	}
	else
		SASSERT( false );	// 値取得に失敗した

	return vaValue.lVal;
}

// インデックス番号でlong値をセットする
sindyErrCode CRow::SetLongValue( long index, long lNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_I4;
	vaValue.lVal = lNewValue;

	return SetValue( index, vaValue );
}

// インデックス番号でdouble値を取得する
double CRow::GetDoubleValue( long index, const double& dDefaultValue ) const
{
	VARIANT vaValue;
	VariantInit(&vaValue);

	// 引数のチェックはGetValueでするから良い
	if( sindyErr_NoErr == GetValue( index, vaValue ) )
	{
		switch( vaValue.vt )
		{
			case VT_I8: break;
			case VT_NULL: vaValue.dblVal = dDefaultValue; break;
			default: SASSERT( vaValue.vt == VT_I8 || vaValue.vt == VT_NULL ); vaValue.dblVal = dDefaultValue; break;
		}
	}
	else
		SASSERT( false );	// 値取得に失敗した

	return vaValue.dblVal;
}

// インデックス番号でdouble値をセットする
sindyErrCode CRow::SetDoubleValue( long index, const double& dNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_I8;
	vaValue.dblVal = dNewValue;

	return SetValue( index, vaValue );
}

// インデックス番号でString値を取得する
LPCTSTR CRow::GetStringValue( long index, int nLen, LPTSTR lpszValue ) const
{
	LPCTSTR pRet = 0;	// 返り値

	LOGASSERTE_IF( lpszValue != NULL, sindyErr_ArgIsNull )
	{
		// 初期化
		lpszValue[0] = _T('\0');

		VARIANT vaValue;
		VariantInit(&vaValue);

		// 引数のチェックはSetValueByFieldNameでするから良い
		sindyErrCode emErr = sindyErr_NoErr;
		LOGASSERT_IF( sindyErr_NoErr == ( emErr = GetValue( index, vaValue ) ), emErr )
		{
			USES_CONVERSION;

			switch( vaValue.vt )
			{
				case VT_BSTR: lstrcpyn( lpszValue, OLE2CT(vaValue.bstrVal), nLen ); pRet = lpszValue; break;
				case VT_NULL: break;
				default: LOGASSERTE( vaValue.vt == VT_BSTR || vaValue.vt == VT_NULL, sindyErr_AOUnsupportValueType ); break;
			}
		}

		VariantClear(&vaValue);
	}

	return pRet;
}

// インデックス番号で文字列値をセットする
sindyErrCode CRow::SetStringValue( long index, LPCTSTR lpcszNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_BSTR;
	vaValue.bstrVal = T2BSTR(lpcszNewValue);

	sindyErrCode emErr = SetValue( index, vaValue );
	VariantClear(&vaValue);

	return emErr;
}

// フィールド名でbool値を取得する
bool CRow::GetBoolValueByFieldName( LPCTSTR lpcszFieldName, bool bDefaultValue ) const
{
	VARIANT vaValue;
	VariantInit(&vaValue);

	// 引数のチェックはSetValueByFieldNameでするから良い
	if( sindyErr_NoErr == GetValueByFieldName( lpcszFieldName, vaValue ) )
	{
		switch( vaValue.vt )
		{
			case VT_I4: bDefaultValue = ( vaValue.lVal == 0 ) ? false : true; break;
			case VT_NULL: break;
			default: SASSERT( vaValue.vt == VT_I4 || vaValue.vt == VT_NULL ); break;
		}
	}
	else
		SASSERT( false );	// 値取得に失敗した

	return bDefaultValue;
}

// フィールド名でbool値をセットする
sindyErrCode CRow::SetBoolValueByFieldName( LPCTSTR lpcszFieldName, bool bNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_I4;
	vaValue.lVal = (bNewValue) ? 1 : 0;

	return SetValueByFieldName( lpcszFieldName, vaValue );
}

// フィールド名でlong値を取得する
long CRow::GetLongValueByFieldName( LPCTSTR lpcszFieldName, long lDefaultValue ) const
{
	VARIANT vaValue;
	VariantInit(&vaValue);

	// 引数のチェックはSetValueByFieldNameでするから良い
	if( sindyErr_NoErr == GetValueByFieldName( lpcszFieldName, vaValue ) )
	{
		switch( vaValue.vt )
		{
			case VT_I4: break;
			case VT_I2: break;
			case VT_NULL: vaValue.lVal = lDefaultValue; break;
			default: //SASSERT( vaValue.vt == VT_I4 || vaValue.vt == VT_NULL );
				vaValue.lVal = lDefaultValue; break;
		}
	}
	else
		SASSERT( false );	// 値取得に失敗した

	return vaValue.lVal;
}

// フィールド名でlong値をセットする
errorcode::sindyErrCode CRow::SetLongValueByFieldName( LPCTSTR lpcszFieldName, long lNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_I4;
	vaValue.lVal = lNewValue;

	return SetValueByFieldName( lpcszFieldName, vaValue );
}

// フィールド名でdouble値を取得する
double CRow::GetDoubleValueByFieldName( LPCTSTR lpcszFieldName, const double& dDefaultValue ) const
{
	VARIANT vaValue;
	VariantInit(&vaValue);

	// 引数のチェックはSetValueByFieldNameでするから良い
	if( sindyErr_NoErr == GetValueByFieldName( lpcszFieldName, vaValue ) )
	{
		switch( vaValue.vt )
		{
			case VT_R8:
			case VT_DATE: break;
			case VT_NULL: vaValue.dblVal = dDefaultValue; break;
			default: SASSERT( vaValue.vt == VT_R8 || vaValue.vt == VT_DATE || vaValue.vt == VT_NULL ); vaValue.dblVal = dDefaultValue; break;
		}
	}
	else
		SASSERT( false );	// 値取得に失敗した

	return vaValue.dblVal;
}

// フィールド名でdouble値をセットする\n
errorcode::sindyErrCode CRow::SetDoubleValueByFieldName( LPCTSTR lpcszFieldName, const double& dNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_R8;
	vaValue.dblVal = dNewValue;

	return SetValueByFieldName( lpcszFieldName, vaValue );
}

// フィールド名でString値を取得する
LPCTSTR CRow::GetStringValueByFieldName( LPCTSTR lpcszFieldName, int nLen, LPTSTR lpszValue ) const
{
	LPCTSTR pRet = NULL;	// 返り値

	LOGASSERTE_IF( lpszValue != NULL, sindyErr_ArgIsNull )
	{
		// 初期化
		lpszValue[0] = _T('\0');

		VARIANT vaValue;
		VariantInit(&vaValue);

		// 引数のチェックはSetValueByFieldNameでするから良い
		sindyErrCode emErr = sindyErr_NoErr;
		LOGASSERT_IF( sindyErr_NoErr == ( emErr = GetValueByFieldName( lpcszFieldName, vaValue ) ), emErr )
		{
			USES_CONVERSION;
			switch( vaValue.vt )
			{
				case VT_BSTR: lstrcpyn( lpszValue, OLE2CT(vaValue.bstrVal), nLen ); pRet = lpszValue; break;
				case VT_NULL: break;
				default: LOGASSERTE( vaValue.vt == VT_BSTR || vaValue.vt == VT_NULL, sindyErr_AOUnsupportValueType ); break;
			}
		}
		VariantClear(&vaValue);
	}

	return pRet;
}

// フィールド名で文字列値をセットする
errorcode::sindyErrCode CRow::SetStringValueByFieldName( LPCTSTR lpcszFieldName, LPCTSTR lpcszNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_BSTR;
	vaValue.bstrVal = T2BSTR(lpcszNewValue);

	sindyErrCode emErr = SetValueByFieldName( lpcszFieldName, vaValue );
	VariantClear(&vaValue);
	return emErr;
}

// フィールド名でString値のサイズを取得する
int CRow::GetStringValueLengthByFieldName( LPCTSTR lpcszFieldName ) const
{
	int nSize = -1;	// 返り値

	VARIANT vaValue;
	VariantInit(&vaValue);

	sindyErrCode emErr = sindyErr_NoErr;
	LOGASSERT_IF( sindyErr_NoErr == ( emErr = GetValueByFieldName( lpcszFieldName, vaValue ) ), emErr )
	{
		USES_CONVERSION;

		switch( vaValue.vt )
		{
			case VT_BSTR: nSize = lstrlen( OLE2T(vaValue.bstrVal) ); break;
			case VT_NULL: nSize = 0; break;
			default: SASSERT( vaValue.vt == VT_BSTR || vaValue.vt == VT_NULL ); break;
		}
	}
	VariantClear(&vaValue);

	return nSize;
}

// フィールド名でString値の文字数を取得する
int CRow::GetStringValueNumByFieldName( LPCTSTR lpcszFieldName ) const
{
	int nSize = -1;	// 返り値

	VARIANT vaValue;
	VariantInit(&vaValue);

	sindyErrCode emErr = sindyErr_NoErr;
	LOGASSERT_IF( sindyErr_NoErr == ( emErr = GetValueByFieldName( lpcszFieldName, vaValue ) ), emErr )
	{
		switch( vaValue.vt )
		{
			case VT_BSTR: nSize = SysStringLen( vaValue.bstrVal ); break;
			case VT_NULL: nSize = 0; break;
			default: SASSERT( vaValue.vt == VT_BSTR || vaValue.vt == VT_NULL ); break;
		}
	}
	VariantClear(&vaValue);

	return nSize;
}

// 関連オブジェクトリストに格納する
/*void CRow::push_back( const CSPRow& cRow )
{
	LOGASSERTE_IF( *cRow, sindyErr_ArgIsNull )
		m_listRelRows[cRow->GetTableType()].push_back( cRow );
}*/

CGeometry* CRow::GetShape()
{
	// 形状がキャッシュされているかどうか
	if( ! ( m_pGeom && m_pGeom->IsCached() ) )
	{
		// 形状を持っているオブジェクトかどうか確認
		if( p && (IFeaturePtr)p )
		{
			// トレース出すこと！！
			TRACEMESSAGE(CREATE_CACHE);
			CreateCache();
		}
	}

	return m_pGeom;
}

const CGeometry* CRow::GetShape() const
{
	// 形状がキャッシュされているかどうか
	if( ! ( m_pGeom && m_pGeom->IsCached() ) )
	{
		// 形状を持っているオブジェクトかどうか確認
		if( p && (IFeaturePtr)p )
		{
			// トレース出すこと！！
			TRACEMESSAGE(CREATE_CACHE);
			const_cast<CRow*>(this)->CreateCache();	// キャッシュ作るだけなので、const 外す
		}
	}

	return m_pGeom;
}

sindyErrCode CRow::SetShape( IGeometry* ipGeom )
{
	if( ! ipGeom )
		return sindyErr_ArgIsNull;

	if( ! m_pGeom )
		m_pGeom = new CGeometry( (IGeometryPtr)AheCreateClone(ipGeom) );
	else
		(*m_pGeom) = ipGeom;

	return sindyErr_NoErr;
}

// 現在格納されているフィーチャの形状のコピーを返す
IGeometryPtr CRow::GetShapeCopy( bool bLatest/* = true*/, bool bOriginal/* = false*/, bool bFeatureChangedOriginal/* = false*/, bool bDiff/* = false*/, long/* lParam = -1*/  ) const
{
	IGeometryPtr ipGeom;	// 返り値

	if( bLatest )
	{
		if( m_pGeom )
			ipGeom = AheCreateClone( (IGeometry*)(*this->GetShape()) );
		else {
			// キャッシュしていない場合はキャッシュを作らないようにげっとする
			IFeaturePtr ipFeature;
			if( SUCCEEDED( QueryInterface( &ipFeature ) ) )
			{
				SUCCEEDED_IF( ipFeature->get_ShapeCopy( &ipGeom ), sindyErr_GeometryNotFound, THERE_MAY_BE_NULL_SHAPE )
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
			if( bOriginal || ( ipGeom == NULL && IsCached() ) )
			{
				IGeometryPtr ipGeomTmp;
				SUCCEEDED_IF( ipFeature->get_ShapeCopy( &ipGeomTmp ), sindyErr_GeometryNotFound, THERE_MAY_BE_NULL_SHAPE )
				{
					// [Bug 6065] ipGeomがNULL（bLatestがfalseでbOriginalがtrueの時に）でipGeomTmpもNULLの時にエラーログが出力されるので回避。
					if ( ipGeom ) {
						if ( ! bDiff )
							ipGeom = CGeometry::Union( ipGeom, ipGeomTmp );
						else
							ipGeom = CGeometry::SymmetricDifference( ipGeom, ipGeomTmp );
					} else
						ipGeom = ipGeomTmp;
				}
			}
			if( bFeatureChangedOriginal )
			{
				IGeometryPtr ipGeomTmp;
				IFeatureChangesPtr ipChanges( ipFeature );
				LOGASSERTE_IF( SUCCEEDED( ipChanges->get_OriginalShape( &ipGeomTmp ) ), sindyErr_GeometryFunctionFailed )
				{
					// [Bug 6065] ipGeomがNULL（bLatestがfalse時に）でipGeomTmpもNULLの時にエラーログが出力されるので回避。
					if ( ipGeom ) {
						if ( ! bDiff )
							ipGeom = CGeometry::Union( ipGeom, ipGeomTmp );
						else
							ipGeom = CGeometry::SymmetricDifference( ipGeom, ipGeomTmp );
					} else
						ipGeom = AheCreateClone( ipGeomTmp );
				}
			}
		}
	}
	return ipGeom;
}

// 整合性をチェックする
sindyErrCode CRow::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrs ) const
{
	if( g_bDisableCheckToNoSubstance )
	{
		// 実体がないもの（新規作成されたものを除く）は
		// チェック対象から外す
		if( ! ( p || m_bCreated ) )
			return sindyErr_NoErr;
	}

	// 自分自身のチェック
	// 形状
	schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

	if ( Changed( &emUpdateType ) )
	{
		if( cRule.GetUseEditableRule() ) // 編集ルールを使用する場合のみに限定
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
			{
				CErrorInfos infos;

				if( sindyErr_NoErr != CheckEditable( emEditType, infos ) )
				{
					CErrorObject cErr( *this );
					cErr.push_back( infos );
					cErrs.push_back( cErr );
				}
			}
		}

		// 編集したもの＆削除されていないもののみチェック
		if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
			const CGeometry* pGeom = GetShape();
			if( pGeom )
			{
				IEnvelopePtr ipChkEnv;

				// ポリゴンかつ変更があったオブジェクトで1000点以上構成点をもつもののみ変更領域取得
				if ( pGeom->GetGeometryType() == esriGeometryPolygon && emUpdateType != schema::ipc_table::update_type::kDefault ) {
					if ( pGeom->GetPointCount() > 1000 ) {
						// 変更領域取得
						IGeometryPtr ipEdit = GetShapeCopy(true, true, false, true);

						if ( ipEdit )
							ipEdit->get_Envelope( &ipChkEnv );
					}
				}

				CBasicLogicRule cBasicRule;
				// セグメントチェック
				CErrorInfos cErrInfos;
				if( sindyErr_NoErr != pGeom->CheckLogic( cRule.GetBasicRule( GetTableName(), GetNameString()->GetUserName(), cBasicRule ), cErrInfos, ipChkEnv ) )
				{
					CErrorObject cErr( *this );
					cErr.push_back( cErrInfos );
					cErrs.push_back( cErr );
				}
			}
			// 属性
			bool bIsSDE = GetNameString()->IsSDE(); // 文字列フィールドはPGDBとSDEで型が違うため、どっちなのかを判断する
			const CFieldMap& cFields( *GetFieldMap() );
			int nIndex = 0;
			for( CFieldMap::const_iterator it = cFields.begin(), itEnd = cFields.end(); it != itEnd; ++it, ++nIndex )
			{
				if( ! it->editable )
					continue;

				CComVariant vaValue;
				GetValue( nIndex, vaValue );
				if( VT_EMPTY == vaValue.vt )
					continue; // フィールドが取得制限されている場合

				// NOT NULLチェック（bug 6434）
				if( ! it->nullable )
				{
					if( VT_NULL == vaValue.vt || 
						VT_BSTR == vaValue.vt && 0 >= ::SysStringLen(vaValue.bstrVal) )
						cErrs.push_back( CErrorObject( *this, CErrorInfo( sindyErr_SetNullValueToNotNullField, sindyErrLevel_ERR, pGeom ? (IGeometry*)*pGeom : NULL, nIndex, -1, NOT_NULL_FIELD, it->name ) ) );
				}

				switch( it->type )
				{
					case esriFieldTypeString:
					{
						// 文字列フィールドあふれの検出
						if( VT_BSTR == vaValue.vt )
						{
							// 終端文字は入らない
							// [Bug 6129] City_Adminの構成点が削除できない。（PGDBのみ）
							// PGDBの時のit->lengthは文字数。
							size_t lByte = bIsSDE ? ( strlen(CW2A(vaValue.bstrVal)) ) : ( (wcslen( vaValue.bstrVal ) ) );
							if( VT_BSTR == vaValue.vt && lByte > it->length )
								cErrs.push_back( CErrorObject( *this, CErrorInfo( sindyErr_FieldLengthLimitOver, sindyErrLevel_ERR, pGeom ? (IGeometry*)*pGeom : NULL, nIndex, -1, BYTES_OVER, it->name, CString(vaValue.bstrVal), lByte - it->length ) ) );
						}
						break;
					}
					default:
						if( VT_NULL != vaValue.vt )
						{
							// 桁数あふれチェック
							if( 0 < it->precision )
							{
								// 整数桁数のみチェックする
								long num = 0;
								{
									CComVariant va( vaValue );
									va.ChangeType( VT_I4 );
									if( va.vt != VT_I4 ) // vaValue=""の場合にはChangeTypeでVT_I4にならない
										va = 0L;
									va.lVal = abs(va.lVal);
									while( 0 < va.lVal ){ va.lVal /= 10; ++num; }
								}
								if( it->precision - it->scale < num )
								{
									CComVariant vaValueStr( vaValue );
									vaValueStr.ChangeType( VT_BSTR );
									cErrs.push_back( CErrorObject( *this, CErrorInfo( sindyErr_FieldPrecisionLimitOver, sindyErrLevel_ERR, pGeom ? (IGeometry*)*pGeom : NULL, nIndex, -1, ALLOWABLE_NUM_OF_DIGITS, it->name, CString(vaValueStr.bstrVal), it->precision - it->scale ) ) );
								}
							}
							// レンジチェック
							if( VT_EMPTY != it->range.first.vt || VT_EMPTY != it->range.second.vt )
							{
								CComVariant va( vaValue ), vaMin( it->range.first ), vaMax( it->range.second );
								va.ChangeType( VT_R8 ); vaMin.ChangeType( VT_R8 ); vaMax.ChangeType( VT_R8 );
								if( ! ( ( VT_EMPTY == it->range.first.vt  || vaMin.dblVal <= va.dblVal ) &&   // MIN
									    ( VT_EMPTY == it->range.second.vt || vaMax.dblVal >= va.dblVal ) ) )  // MAX
								{
									CComVariant vaValueStr( vaValue ), vaMin( it->range.first ), vaMax( it->range.second );
									vaValueStr.ChangeType( VT_BSTR ); vaMin.ChangeType( VT_BSTR ); vaMax.ChangeType( VT_BSTR );
									cErrs.push_back( CErrorObject( *this, CErrorInfo( sindyErr_FieldRangeLimitOver, sindyErrLevel_ERR, pGeom ? (IGeometry*)*pGeom : NULL, nIndex, -1, ALLOWABLE_RANGE, it->name, CString(vaValueStr.bstrVal), CString(vaMin.bstrVal), CString(vaMax.bstrVal) ) ) );
								}
							}
						}
						break;
				}
			}
		}
	}

	// 関連オブジェクトのチェック
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			(*it2)->CheckLogic( cRule, cErrs );
	}
	return cErrs.GetDefaultErrCode();
}

// 編集可能かどうかをチェックする
sindyErrCode CRow::CheckEditable( sindyeEditType emType, CErrorInfos& cErrInfos ) const
{
	TCHAR strMsg[2048];

	sindyeEditableType emEditable = g_cRule.IsEditable( *this, emType, 2048, strMsg );
	if( sindyeEditableEditable != emEditable )
	{
		sindyErrCode eErrCode = sindyErr_RuleNotFound;
		switch( emType )
		{
		case sindyeEditCreate:		eErrCode = sindyErr_RuleNotAllowedCreate; break;
		case sindyeEditMove:		eErrCode = sindyErr_RuleNotAllowedMove; break;
		case sindyeEditDelete:		eErrCode = sindyErr_RuleNotAllowedDelete; break;
		case sindyeEditAttribute:	eErrCode = sindyErr_RuleNotAllowedEditAttribute; break;
		default:break;
		}
		cErrInfos.push_back( CErrorInfo( eErrCode, ( sindyeEditableWarnning == emEditable ) ? sindyErrLevel_WARN : sindyErrLevel_ERR, NULL, -1, -1, strMsg ) );
	}

	return cErrInfos.GetDefaultErrCode();
}

// メンバ変数を初期化する
void CRow::Init()
{
	m_emTableType = sindyTableType::unknown;
	m_pCache = NULL;
	m_pGeom  = NULL;
	m_lOID   = -1;
	m_bCreated = m_bDeleted = false;
	m_spNameString.reset( (CTableNameString*)NULL );
	m_spFields.reset( (CFieldMap*)NULL );
	CContainer::clear();
}

bool CRow::IsSameAttribute(const CRow& src, const CListString& cExclusions/*=CListString()*/) const
{
	// 接続プロパティ、オーナ名、テーブル名が同じか
	if ( !(src.GetNameString()->IsSameSource( *GetNameString() ) &&
           (0 == _tcscmp(GetTableName(), src.GetTableName())) ) )
		return false;

	CListString cDefaultExclusions;

	cDefaultExclusions.push_back( cExclusions );
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

		GetValueByFieldName( it->name, vaSource );
		src.GetValueByFieldName( it->name, vaDest );

		if ( vaSource != vaDest )
			return false;
	}

	return true;
}

void CRow::Trace( bool bAttribute/* = true*/, bool bGeometry/* = true*/ ) const
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
			IGeometryPtr ipGeom = (IGeometry*)(*GetShape());
			CGeometry::Trace( ipGeom, _T("形状") );
		}
	}
}

} // namespace sindy
