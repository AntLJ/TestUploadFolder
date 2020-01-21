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
#include <boost/foreach.hpp>
#include "ContainerBase.h"
#include "RowBase.h"
#include "GeometryBase.h"
#include "ErrorObject2.h"
#include "GlobalFunctions.h"
#include "SchemaSupport.h"

#define foreach BOOST_FOREACH
using namespace sindy;
using namespace sindy::errorcode;

#ifndef SINDY_FOR_ENGLISH
const static _TCHAR DUPLI_OID[] = _T("OID:%dと重複しています");
#else
const static _TCHAR DUPLI_OID[] = _T("Duplicate of OID:%d");
#endif	// SINDY_FOR_ENGLISH

bool CContainerBase::Equal::operator()( const CSPContainerBase& c ) const
{
/*typeof(
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(&*c);
	const CRowBase* pRow2 = dynamic_cast<const CRowBase*>(m_pContainer);
	if( pRow && pRow2 )
		return ( *pRow == *pRow2 );
	else if( pRow || pRow2 )
		return false;
*/
	return (void*)m_pContainer == (void*)&c; // コンテナ同士なら中身を比較してもしょうがないのでポインタ比較で
}

/*bool CContainerBase::EqualTableType::operator()( const CSPContainerBase& c ) const
{
	sindyTableType::ECode emType = m_emType;
	sindyTableType::ECode emType2 = c->_GetTableType();

	// どちらかがunknownの場合はfalseを返す
	if( sindyTableType::unknown == emType ||
		sindyTableType::unknown == emType2 )
		return false;

	return emType == emType2;
}

bool CContainerBase::EqualTableTypeSPRowBase::operator()( const CSPContainerBase& c ) const
{
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(&*c);
	if( pRow )
		return ( m_emType == pRow->GetTableType() );
	return false;
}

bool CContainerBase::EqualTableType_and_OIDSPRowBase::operator()( const CSPContainerBase& c ) const
{
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(&*c);
	if( pRow )
		return ( m_emType == pRow->GetTableType() && m_lOID == pRow->GetOID() );
	return false;
}

bool CContainerBase::EqualTableType_and_ValueSPRowBase::operator()( const CSPContainerBase& c ) const
{
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(&*c);
	if( pRow )
	{
		CComVariant vaValue;
		pRow->GetValue(m_lpcszFieldName,vaValue);
		return ( m_emType == pRow->GetTableType() && vaValue == *m_value );
	}
	return false;
}

bool CContainerBase::EqualRowBaseSPRowBase::operator()( const CSPContainerBase& c ) const
{
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(&*c);
	if( pRow )
		return ( *m_pRow == *pRow );
	return false;
}*/

CContainerBase::CContainerBase(void)
{
}

CContainerBase::~CContainerBase(void)
{
}

CContainerBase& CContainerBase::operator =( const CContainerBase& obj )
{
	GetContainer() = obj.GetContainer();
	return *this;
}

void CContainerBase::push_back( const CSPContainerBase& cCon, bool bSecureUnique/* = true*/ )
{
	if( ( ! bSecureUnique ) || end() == find( cCon ) )
		GetContainer().push_back( cCon );
}

void CContainerBase::push_front( const CSPContainerBase& cCon, bool bSecureUnique/* = true*/ )
{
	if( ( ! bSecureUnique ) || end() == find( cCon ) )
		GetContainer().push_front( cCon );
}

CContainerBase::iterator CContainerBase::insert( CContainerBase::iterator it, const CSPContainerBase& cContainer, bool bSecureUnique/* = true*/ )
{
	iterator ret = ( bSecureUnique ? find( cContainer ) : end() );
	if( ret == end() )
		ret = GetContainer().insert( it, cContainer );
	return ret;
}

/*CContainerBase::iterator CContainerBase::find( _IRow* ipRow )
{
	return find( CRowBase( ipRow ) );
}

CContainerBase::const_iterator CContainerBase::find( _IRow* ipRow ) const
{
	return find( CRowBase( ipRow ) );
}*/

void CContainerBase::clear()
{
	GetContainer().clear();
}

/*// コンテナ内オブジェクトを特定の条件で抽出する
CContainer CContainer::Query( sindyCompareOperator emOperator, sindyTableType::ECode emTableType ) const
{
	CContainer cContainer;
	Query( emOperator, emTableType, cContainer );
	return cContainer;
}
CContainer CContainer::Query( sindyCompareOperator emOperator, const CGeometry& cGeom, sindyeSpatialRel emSpRel ) const
{
	CContainer cContainer;
	Query( emOperator, cGeom, emSpRel, cContainer );
	return cContainer;
}
CContainer CContainer::Query( sindyCompareOperator emOperator, long lFieldIndex, const VARIANT& vaValue ) const
{
	CContainer cContainer;
	Query( emOperator, lFieldIndex, vaValue, cContainer );
	return cContainer;
}
CContainer CContainer::Query( sindyCompareOperator emOperator, LPCTSTR lpcszFieldName, const VARIANT& vaValue ) const
{
	CContainer cContainer;
	Query( emOperator, lpcszFieldName, vaValue, cContainer );
	return cContainer;
}*/
CSPContainerBase CContainerBase::Query( sindyCompareOperator2 emOperator, LPCTSTR lpcszTableName, bool bSecureUnique/* = true*/ ) const
{
	CSPContainerBase cContainer( new CContainerBase() );
	Query( emOperator, lpcszTableName, *cContainer, bSecureUnique );
	return cContainer;
}
/*void CContainer::Query( sindyCompareOperator emOperator, sindyTableType::ECode emTableType, CContainer& cContainer ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			sindyTableType::ECode emTargetTableType = (*it2)->GetTableType();
			switch( emOperator )
			{
				case sindyCompOpeEqual:	           if( emTargetTableType == emTableType ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanSmall:	       if( emTargetTableType < emTableType )  cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanBig:          if( emTargetTableType > emTableType )  cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanSmallOrEqual: if( emTargetTableType <= emTableType ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanBigOrEqual:   if( emTargetTableType >= emTableType ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeNotEqual:         if( emTargetTableType != emTableType ) cContainer.push_back( *it2 ); break;
				default: _ASSERTE( false ); break;
			}
			(*it2)->Query( emOperator, emTableType, cContainer );
		}
	}
}
void CContainer::Query( sindyCompareOperator emOperator, const CGeometry& cGeom, sindyeSpatialRel emSpRel, CContainer& cContainer ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			if( emSpRel & sindyeSpatialRelTouch ) {
				bool bRet = cGeom.Touches( (IGeometry*)(*(*it2)->GetShape()) );
				if( ( sindyCompOpeEqual == emOperator && bRet ) ||
					( sindyCompOpeNotEqual == emOperator && ( ! bRet ) ) )
					cContainer.push_back( *it2 );
			}
			if( emSpRel & sindyeSpatialRelOverlap ) {
				bool bRet = cGeom.Overlaps( (IGeometry*)(*(*it2)->GetShape()) );
				if( ( sindyCompOpeEqual == emOperator && bRet ) ||
					( sindyCompOpeNotEqual == emOperator && ( ! bRet ) ) )
					cContainer.push_back( *it2 );
			}
			if( emSpRel & sindyeSpatialRelCross ) {
				bool bRet = cGeom.Crosses( (IGeometry*)(*(*it2)->GetShape()) );
				if( ( sindyCompOpeEqual == emOperator && bRet ) ||
					( sindyCompOpeNotEqual == emOperator && ( ! bRet ) ) )
					cContainer.push_back( *it2 );
			}
			if( emSpRel & sindyeSpatialRelWithin ) {
				bool bRet = cGeom.Within( (IGeometry*)(*(*it2)->GetShape()) );
				if( ( sindyCompOpeEqual == emOperator && bRet ) ||
					( sindyCompOpeNotEqual == emOperator && ( ! bRet ) ) )
					cContainer.push_back( *it2 );
			}
			if( emSpRel & sindyeSpatialRelContain ) {
				bool bRet = cGeom.Contains( (IGeometry*)(*(*it2)->GetShape()) );
				if( ( sindyCompOpeEqual == emOperator && bRet ) ||
					( sindyCompOpeNotEqual == emOperator && ( ! bRet ) ) )
					cContainer.push_back( *it2 );
			}
			if( emSpRel & sindyeSpatialRelDisjoint ) {
				bool bRet = cGeom.Disjoint( (IGeometry*)(*(*it2)->GetShape()) );
				if( ( sindyCompOpeEqual == emOperator && bRet ) ||
					( sindyCompOpeNotEqual == emOperator && ( ! bRet ) ) )
					cContainer.push_back( *it2 );
			}
			if( emSpRel & sindyeSpatialRelEqual ) {
				bool bRet = cGeom.Equals( (IGeometry*)(*(*it2)->GetShape()) );
				if( ( sindyCompOpeEqual == emOperator && bRet ) ||
					( sindyCompOpeNotEqual == emOperator && ( ! bRet ) ) )
					cContainer.push_back( *it2 );
			}
			(*it2)->Query( emOperator, cGeom, emSpRel, cContainer );
		}
	}
}
void CContainer::Query( sindyCompareOperator emOperator, long lFieldIndex, const VARIANT& vaValue, CContainer& cContainer ) const
{
	if( 0 > lFieldIndex )
	{
		_ASSERTE( 0 <= lFieldIndex );
		return;
	}

	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			const CFieldMap& cFields( *(*it2)->GetFieldMap() );
			if( (size_t)lFieldIndex < cFields.GetFieldCount() )
			{
				CComVariant vaTargetValue;
				(*it2)->GetValue( lFieldIndex, vaTargetValue );
				if( vaTargetValue.vt == vaValue.vt ||
					vaTargetValue.vt == VT_NULL || 
					vaValue.vt == VT_NULL )
				{
					switch( emOperator )
					{
						case sindyCompOpeEqual:            if( vaTargetValue == vaValue ) cContainer.push_back( *it2 ); break;
						case sindyCompOpeThanSmall:        if( vaTargetValue <  vaValue ) cContainer.push_back( *it2 ); break;
						case sindyCompOpeThanBig:          if( vaTargetValue >  vaValue ) cContainer.push_back( *it2 ); break;
						case sindyCompOpeThanSmallOrEqual: if( vaTargetValue < vaValue || vaTargetValue == vaValue ) cContainer.push_back( *it2 ); break;
						case sindyCompOpeThanBigOrEqual:   if( vaTargetValue > vaValue || vaTargetValue == vaValue ) cContainer.push_back( *it2 ); break;
						case sindyCompOpeNotEqual:         if( vaTargetValue != vaValue ) cContainer.push_back( *it2 ); break;
						default: _ASSERTE( false ); break;
					}
				}
			}
			(*it2)->Query( emOperator, lFieldIndex, vaValue, cContainer );
		}
	}
}
void CContainer::Query( sindyCompareOperator emOperator, LPCTSTR lpcszFieldName, const VARIANT& vaValue, CContainer& cContainer ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			CComVariant vaTargetValue;
			(*it2)->GetValueByFieldName( lpcszFieldName, vaTargetValue );
			if( vaTargetValue.vt == vaValue.vt ||
				vaTargetValue.vt == VT_NULL || 
				vaValue.vt == VT_NULL )
			{
				switch( emOperator )
				{
					case sindyCompOpeEqual:            if( vaTargetValue == vaValue ) cContainer.push_back( *it2 ); break;
					case sindyCompOpeThanSmall:        if( vaTargetValue <  vaValue ) cContainer.push_back( *it2 ); break;
					case sindyCompOpeThanBig:          if( vaTargetValue >  vaValue ) cContainer.push_back( *it2 ); break;
					case sindyCompOpeThanSmallOrEqual: if( vaTargetValue < vaValue || vaTargetValue == vaValue ) cContainer.push_back( *it2 ); break;
					case sindyCompOpeThanBigOrEqual:   if( vaTargetValue > vaValue || vaTargetValue == vaValue ) cContainer.push_back( *it2 ); break;
					case sindyCompOpeNotEqual:         if( vaTargetValue != vaValue ) cContainer.push_back( *it2 ); break;
					default: _ASSERTE( false ); break;
				}
				(*it2)->Query( emOperator, lpcszFieldName, vaValue, cContainer );
			}
		}
	}
}*/
CContainerBase& CContainerBase::Query( sindyCompareOperator2 emOperator, LPCTSTR lpcszTableName, CContainerBase& cContainer, bool bSecureUnique/* = true*/ ) const
{
	// dynamic_castを使うので遅い
	// CRowContainerやCTableContainer等CContainerBaseを継承するクラスで
	// 実装したものが使用される必要がある
	// ここであえて実装しているのは純粋仮想関数にしたくないためだけ
	for( const_iterator it = begin(); it != end(); ++it )
	{
		CSPSchemaSupport cSchema = boost::dynamic_pointer_cast<CSchemaSupport>(*it); // 遅くなる
		if( cSchema )
		{
			CString strFieldName( cSchema->GetTableName() );
			switch( emOperator )
			{
			case sindyCompOpeEqual2:            if( strFieldName == lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeThanSmall2:        if( strFieldName <  lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeThanBig2:          if( strFieldName >  lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeThanSmallOrEqual2: if( strFieldName <= lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeThanBigOrEqual2:   if( strFieldName >= lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeNotEqual2:         if( strFieldName != lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			default: _ASSERTE( false ); break;
			}
		}
		(*it)->Query( emOperator, lpcszTableName, cContainer, bSecureUnique );
	}
	return cContainer;
}

// キャッシュを削除する
void CContainerBase::DeleteCache()
{
	CRowBase* pRow = dynamic_cast<CRowBase*>(this);
	if( pRow )
		pRow->DeleteCache();

	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		(*it)->DeleteCache();
}

// フィーチャの変更を元に戻す
void CContainerBase::Reset()
{
	foreach( CSPContainerBase& con, GetContainer() )
		con->Reset();
}

// 削除する
void CContainerBase::Delete( /*LPCTSTR lpcszTableName*//* = NULL*//*, bool bRecursive*//* = false*/ )
{
#if 0
	// 全削除かどうかの判定
	bool bAllDelete = ( lpcszTableName ) ? true : false;
	if( ! bAllDelete )
	{
		LPCTSTR lpcszThisTableName = _GetTableName();
		if( lpcszThisTableName && 0 == lstrcmpi( lpcszThisTableName, lpcszTableName ) )
			bAllDelete = true;
	}

	if( bAllDelete )
	{
		CRowBase* pRowBase = dynamic_cast<CRowBase*>(this);
		if( pRowBase )
			pRowBase->Delete();

		for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		{
			bool bDelete = ( bAllDelete ) ? true : false;
			(*it)->Delete( bAllDelete ? NULL : lpcszTableName );
		}
	}
#else
	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		(*it)->Delete();
#endif // if 0
}

void CContainerBase::UnDelete()
{
	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		(*it)->UnDelete();
}

// 与えられた形状の範囲で平行移動処理を行う
sindyErrCode CContainerBase::Move( const CGeometryBase& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMove, CErrorObjectsBase& cErrs )
{
	// CRowBase側の処理
	CRowBase* pRow = dynamic_cast<CRowBase*>(this);
	if( pRow )
		pRow->Move( cGeom, pairMove, emMove, cErrs );

	// コンテナリストの処理
	for( iterator it = begin(); it != end(); ++it )
		(*it)->Move( cGeom, pairMove, emMove, cErrs );

	return cErrs.GetDefaultErrCode();
}
// 編集内容を保存する
sindyErrCode CContainerBase::Store( CErrorObjectsBase& cErrs )
{
	// CRowBase側の処理
	CRowBase* pRow = dynamic_cast<CRowBase*>(this);
	if( pRow )
		pRow->Store( cErrs );

	// コンテナリストの処理
	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		(*it)->Store( cErrs );

	return cErrs.GetDefaultErrCode();
}

// 整合性をチェックする
sindyErrCode CContainerBase::CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrObjects ) const
{
	// TODO : チェッカーとして使うためには、チェック限定領域をCFeatures::CheckLogic()から渡せるようにしないとだめ。

	// CRowBase側の処理
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow )
		pRow->CheckLogic( cRule, cErrObjects );

	// コンテナリストの処理
	for( const_iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
		const CRowBase* pRow = dynamic_cast<const CRowBase*>(&**it);
		if( pRow )
		{
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			// 削除されていないもののみチェック
			if ( ! pRow->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted )
			{
				// 単体チェック
				pRow->CheckLogic( cRule, cErrObjects );

				// 複合チェック
				CGeometryBase cGeom( (IGeometry*)*pRow );
				if( (IGeometry*)cGeom )
				{
					IGeometryPtr ipEditArea;
					CSPErrorObjectBase cErrObj( new CErrorObjectBase( *pRow ) ); // エラー情報格納用

					// ポリゴンかつ変更があったオブジェクトで1000点以上構成点をもつもののみ変更領域取得
					if ( cGeom.GetGeometryType() == esriGeometryPolygon && emUpdateType != schema::ipc_table::update_type::kDefault && cGeom.GetPointCount() >= 1000 ) {
						// 変更領域取得
						ipEditArea = pRow->GetShapeCopy((ShapeMode)(enumAll|enumDiff));
					}

					// 変更があったオブジェクトのみ
					if ( emUpdateType != schema::ipc_table::update_type::kDefault )
					{
						// 重複許可レイヤは飛ばす
						if( AheIsOverlapableLayer( pRow->GetTableName() ) )
							continue;

						// 形状重複チェック（bug 4963）
						const_iterator it2 = it;
						++it2;
						for( ; it2 != itEnd; ++it2 )
						{
							const CRowBase* pRow2 = dynamic_cast<const CRowBase*>(&**it2);
							if( pRow2 )
							{
								emUpdateType = schema::ipc_table::update_type::kDefault;

								// 削除されていないものとチェック
								if ( ! pRow2->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted )
								{
									const CGeometryBase* pGeom2 = pRow2->GetShape();
									if( pGeom2 )
									{
										if ( pGeom2->GetGeometryType() == esriGeometryPolygon ) {
											bool bCheck2 = true;

											if ( ipEditArea )
											{
												// 変更領域にかかっているオブジェクトのみ重複チェック
												if ( ! pGeom2->Overlaps( ipEditArea ) )
													bCheck2 = false;
											}

											if ( bCheck2 )
											{
												// [Bug 5804]包含されているもの、しているもの両方でチェックしないといけない
												if( cGeom.Overlaps( (IGeometry*)(*pGeom2) ) || 
													cGeom.Contains( (IGeometry*)(*pGeom2) ) ||
													cGeom.Within( (IGeometry*)(*pGeom2) ) ||
													cGeom.Equals( (IGeometry*)(*pGeom2) ) )
												{
													cErrObj->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryOverlapped, sindyErrLevel_ERR, (IGeometry*)(*pRow->GetShape()), -1, -1, DUPLI_OID, pRow->GetOID() ) ) );
												}
											}
										}
									}
								}
							}
						}
					}
					if( sindyErr_NoErr != cErrObj->GetDefaultErrCode() )
						cErrObjects.push_back( cErrObj );
				}
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// コンフリクト回避のための既編集チェックを行う
sindyErrCode CContainerBase::CheckModify( CErrorObjectsBase& cErrObjects ) const
{
	// CRowBase側の処理
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow )
		pRow->CheckModify( cErrObjects );

	// コンテナリストの処理
	for( const_iterator it = begin(); it != end(); ++it )
		(*it)->CheckModify( cErrObjects );

	return cErrObjects.GetDefaultErrCode();
}

// 属性に変更があったかどうかをチェックする
bool CContainerBase::Changed( schema::ipc_table::update_type::ECode* emUpdateType/* = NULL*/ ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		if( (*it)->Changed( emUpdateType ) )
			return true;
	}
	return false;
}

// キャッシュされているかどうかをチェックする
bool CContainerBase::IsCached() const
{
	// CRowBase側の処理
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow )
	{
		if( ! pRow->IsCached() )
			return false;
	}

	// コンテナリストの処理
	for( const_iterator it = begin(); it != end(); ++it )
	{
		// 一つでもキャッシュされていないならfalse
		// とする。そうすることで、CreateCacheを呼ぶ
		// 為のトリガとなる
		if( ! (*it)->IsCached() )
			return false; 
	}

	return true;
}

CContainerBase::iterator CContainerBase::erase( iterator it )
{
	return GetContainer().erase( it );
}

CContainerBase::iterator CContainerBase::erase( iterator first, iterator last )
{
	return GetContainer().erase( first, last );
}

/*void CContainerBase::erase( const CSPRowBase& cRow )
{
	iterator it = find( *cRow );
	while( it != end() )
		it = erase( it );
}*/

// 現在格納されているフィーチャの存在矩形を返す
IEnvelopePtr CContainerBase::GetEnvelope( ShapeMode emMode/* = enumLatest*/, bool bChangedOnly/* = false*/, long lParam/* = -1*/ ) const
{
	IEnvelopePtr ipRet;

	// CRowBase側の処理
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow )
	{
		if( ( ! bChangedOnly ) || pRow->Changed() )
			ipRet = pRow->GetEnvelope( emMode );
	}

	// コンテナリストの処理
	for( const_iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
		IEnvelopePtr ipEnv( (*it)->GetEnvelope( emMode, bChangedOnly, lParam ) );
		if( ipRet )
		{
			if( ipEnv )
			{
				LOGASSERTE_IF( SUCCEEDED( ipRet->Union( ipEnv ) ), sindyErr_GeometryFunctionFailed )
				{
				}
			}
		}
		else
			ipRet = ipEnv;
	}
	return ipRet;
}

// 現在格納されているフィーチャの形状をマージしたものを返す
IGeometryPtr CContainerBase::GetShapeCopy( ShapeMode emMode/* = enumLatest*/, bool bChangedOnly/* = false */, long lParam/* = -1*/ ) const
{
	_ISetPtr ipSet( CLSID_Set );

	GetShapeCopy( ipSet, emMode, bChangedOnly, lParam );
	return AheConstructUnion( ipSet );
}

void CContainerBase::GetShapeCopy( _ISetPtr& ipCol, ShapeMode emMode/* = enumLatest*/, bool bChangedOnly/* = false */, long lParam/* = -1*/ ) const
{
	// CRowBase側の処理
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow && ( (! bChangedOnly) || pRow->Changed() ) )
		ipCol->Add( pRow->GetShapeCopy( emMode ) );

	// コンテナリストの処理
	for( const_iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		(*it)->GetShapeCopy( ipCol, emMode, bChangedOnly, lParam );
}

sindyErrCode CContainerBase::CreateCache( bool bDetach/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ )
{
	sindyErrCode emErr = sindyErr_NoErr; // 戻り値

	foreach( CSPContainerBase& con, GetContainer() )
	{
		sindyErrCode emErrTmp = con->CreateCache( bDetach, spFields, spNameString );

		if( sindyErr_NoErr == emErrTmp )
			emErr = emErrTmp;
	}
	return errorcode::sindyErr_NoErr;
}

// キャッシュ以外のパラメータを更新する
sindyErrCode CContainerBase::UpdateParameter( bool bForce/* = false*/ )
{
	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
		errorcode::sindyErrCode emErr = (*it)->UpdateParameter( bForce );
		if( errorcode::sindyErr_NoErr != emErr )
			return emErr; // 途中で終わる
	}
	return errorcode::sindyErr_NoErr;
}
void CContainerBase::DeleteParameter()
{
	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
			(*it)->DeleteParameter();
}

void CContainerBase::Trace( bool bAttribute/* = true*/, bool bGeometry/* = true*/ ) const
{
#ifdef _DEBUG
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow )
	{
		ATLTRACE( _T("container: Table[%s] OID[%d]\n"), pRow->GetTableName(), pRow->GetOID() );
	}
#endif // ifdef _DEBUG

	for( const_iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
#ifdef _DEBUG
		ATLTRACE(_T(" ") );
#endif // ifdef _DEBUG

		(*it)->Trace( bAttribute, bGeometry );
	}
}

/*// テーブルタイプを取得する
sindyTableType::ECode CContainerBase::_GetTableType() const
{
	// CRowBaseの場合
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow )
		return pRow->GetTableType();

	// CTableの場合
	const CTable* pTable = dynamic_cast<const CTable*>(this);
	if( pTable )
		return pTable->GetTableType();

	// ただのコンテナの場合
	return sindyTableType::unknown;
}

// テーブル名を取得する
LPCTSTR CContainerBase::_GetTableName() const
{
	// CRowBaseの場合
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow )
		return pRow->GetTableName();

	// CTableの場合
	const CTable* pTable = dynamic_cast<const CTable*>(this);
	if( pTable )
		return pTable->GetTableName();

	// ただのコンテナの場合
	return NULL;
}*/
