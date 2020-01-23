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
#include "Container.h"
#include "Feature.h"
#include "ErrorObject.h"
#include "GlobalFunctions.h"

using namespace sindy;
using namespace sindy::errorcode;

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR DUPLICATE_OF_OID = _T("OID:%dと重複しています");
#else
	LPCTSTR DUPLICATE_OF_OID = _T("Duplicate of OID: %d");
#endif // SINDY_FOR_ENGLISH
}

CContainer::CContainer(void)
{
}

CContainer::~CContainer(void)
{
}

errorcode::sindyErrCode CContainer::Materialization( ITable* ipTable )
{
	errorcode::sindyErrCode emErr = errorcode::sindyErr_NoErr; // 戻り値

	for( iterator it = begin(); it != end(); ++it )
	{
		// 関連レコードを実体化
		for( rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			emErr = (*it2)->Materialization( ipTable );
	}

	// find用インデックスを更新
	UpdateIndex();

	return emErr;
}

CContainer& CContainer::operator =( const CContainer& obj )
{
	m_mapRowsContainer = obj.m_mapRowsContainer;
#ifdef _USE_FIND_INDEX
	m_mapRowsContainerOIDIndex = obj.m_mapRowsContainerOIDIndex;
#endif //_USE_FIND_INDEX
	return *this;
}

void CContainer::push_back( const CSPRow& cRow, bool bSecureUnique/* = true*/ )
{
	if( ! ( bSecureUnique && find( *cRow ) ) )
	{
		sindyTableType::ECode emType = cRow->GetTableType();
		m_mapRowsContainer[emType].push_back( cRow );
#ifdef _USE_FIND_INDEX
		long lOID = cRow->GetOID();
		if( 0 < lOID )
			m_mapRowsContainerOIDIndex[emType][lOID] = m_mapRowsContainer[emType].size() - 1;
#endif //_USE_FIND_INDEX
	}
}

void CContainer::push_front( const CSPRow& cRow, bool bSecureUnique/* = true*/ )
{
	if( ! ( bSecureUnique && find( *cRow ) ) )
	{
		sindyTableType::ECode emType = cRow->GetTableType();
		m_mapRowsContainer[emType].push_front( cRow );
#ifdef _USE_FIND_INDEX
		long lOID = cRow->GetOID();
		if( 0 < lOID )
		{
			// 既存のものは一つずつずらす必要があるので、このメソッドはpush_backじゃだめな
			// 場合に限定すること！！
			for( std::map<long,long>::iterator it = m_mapRowsContainerOIDIndex[emType].begin(); it != m_mapRowsContainerOIDIndex[emType].end(); ++it )
				++it->second;
			m_mapRowsContainerOIDIndex[emType][lOID] = 0;
		}
#endif //_USE_FIND_INDEX
	}
}

void CContainer::clear()
{
	m_mapRowsContainer.clear();
#ifdef _USE_FIND_INDEX
	m_mapRowsContainerOIDIndex.clear();
#endif //_USE_FIND_INDEX
}

bool CContainer::QueryDoughnutInteriorContainFeatures(sindyTableType::ECode emTableType, const CGeometry& cGeom, CContainer& cContains) const
{
	for ( const_rows_iterator it2 = begin(emTableType), it2End = end(emTableType); it2 != it2End; ++it2 ) {
		bool bHasContain = false;

		// Interior
		CGeometry::const_part_iterator itPart = (*it2)->GetShape()->part_begin(); ++itPart;

		for ( ; itPart != (*it2)->GetShape()->part_end(); ++itPart ) {
			CGeometry cInteriorGeom = CGeometry::ToPolygon( (*itPart) );

			if ( cInteriorGeom.Contains( cGeom ) || cInteriorGeom.Equals( cGeom ) ) {
				bHasContain = true;
				break;
			}
		}

		if ( bHasContain ) {
			cContains.push_back( (*it2) );
			break;
		}
	}

	return !cContains.empty();
}

// コンテナ内オブジェクトを特定の条件で抽出する
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
}
CContainer CContainer::Query( sindyCompareOperator emOperator, LPCTSTR lpcszTableName ) const
{
	CContainer cContainer;
	Query( emOperator, lpcszTableName, cContainer );
	return cContainer;
}
void CContainer::Query( sindyCompareOperator emOperator, sindyTableType::ECode emTableType, CContainer& cContainer ) const
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
			if( ! (IGeometry*)cGeom )
				cContainer.push_back(*it2);
			else {
				CGeometry* pGeom = (*it2)->GetShape();
				if( ! pGeom )
					continue;

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
}
void CContainer::Query( sindyCompareOperator emOperator, LPCTSTR lpcszTableName, CContainer& cContainer ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			CString strFieldName( (*it2)->GetTableName() );
			switch( emOperator )
			{
				case sindyCompOpeEqual:            if( strFieldName == lpcszTableName ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanSmall:        if( strFieldName <  lpcszTableName ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanBig:          if( strFieldName >  lpcszTableName ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanSmallOrEqual: if( strFieldName <= lpcszTableName ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanBigOrEqual:   if( strFieldName >= lpcszTableName ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeNotEqual:         if( strFieldName != lpcszTableName ) cContainer.push_back( *it2 ); break;
				default: _ASSERTE( false ); break;
			}
			(*it2)->Query( emOperator, lpcszTableName, cContainer );
		}
	}
}

// キャッシュを削除する
void CContainer::DeleteCache( bool bDeleteContainerCache/* = false*/ )
{
	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
		for( rows_iterator itRows = it->second.begin(), itRowsEnd = it->second.end(); itRows != itRowsEnd; ++itRows )
			itRows->get()->DeleteCache( bDeleteContainerCache );
	}
}

// フィーチャの変更を元に戻す
void CContainer::Reset( bool bRecursive/* = false*/ )
{
	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
		for( rows_iterator itRows = it->second.begin(); itRows != it->second.end(); )
		{
			itRows->get()->Reset( bRecursive );
			// まだ削除フラグが残っている場合はコンテナから除去
			if( itRows->get()->IsDeleted() )
				itRows = it->second.erase( itRows );
			else
				++itRows;
		}
	}
}

// 削除する
void CContainer::Delete( LPCTSTR lpcszTableName/* = NULL*/, bool bRecursive/* = false*/ )
{
	if( lpcszTableName )
	{
		bool AllScan = false; // テーブル名を全てスキャンするかどうか

		sindyTableType::ECode emType = AheTableName2TableType( lpcszTableName );
		if( sindyTableType::other == emType || 
			sindyTableType::unknown == emType )
			AllScan = true;

		if( AllScan )
		{
			// テーブル名を比較しながら削除
			for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
			{
				bool bCanDeleteContainer = false; // コンテナ単位で消しても良いかどうか
				// テーブルタイプが分かっている場合はここで判定することで少しでもスピード稼ぎ
				if( sindyTableType::other != it->first && 
					0 == _tcsicmp( AheTableType2TableName(it->first), lpcszTableName ) )
					bCanDeleteContainer = true;

				for( rows_iterator itRows = it->second.begin(), itRowsEnd = it->second.end(); itRows != itRowsEnd; ++itRows )
				{
					if( bCanDeleteContainer ||                                         // コンテナ単位で消しても良い場合
						0 == _tcsicmp( (*itRows)->GetTableName(), lpcszTableName ) )   // テーブル名が一致する場合
						itRows->get()->Delete( bRecursive );
				}
			}
		}
		else {
			// 特定のテーブルタイプのみ削除
			iterator it = find( emType );
			if( it != end() )
			{
				for( rows_iterator itRows = it->second.begin(), itRowsEnd = it->second.end(); itRows != itRowsEnd; ++itRows )
					(*itRows)->Delete( bRecursive );
			}
		}

	}
	else {
		// 全て削除
		for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		{
			for( rows_iterator itRows = it->second.begin(), itRowsEnd = it->second.end(); itRows != itRowsEnd; ++itRows )
				itRows->get()->Delete( bRecursive );
		}
	}
}


// 与えられた形状との交差部分で分割処理を行う
/*errorcode::sindyErrCode CContainer::Split( const CGeometry& cGeom, const CSplitTarget& cSplitTarget, CErrorObjects& cErrObjects )
{
	for( iterator it = begin(); it != end(); ++it )
	{
		CErrorObject cErrObj( *it ); // エラー情報格納用

		if( sindyErr_NoErr != it->Split( cGeom, cSplitTarget, cErrObj.push_back( CErrorInfos() ) ) )
			cErrObjects.push_back( cErrObj );
	}

	return cErrObjects.GetDefaultErrCode();
}*/

// 与えられた形状の範囲で平行移動処理を行う
sindyErrCode CContainer::Move( const CGeometry& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMove, CErrorObjects& cErrObjects )
{
	for( iterator it = begin(); it != end(); ++it )
	{
		for( rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			CFeature* pFeat = NULL;
			if( NULL != ( pFeat = dynamic_cast<CFeature*>( it2->get() ) ) )
				pFeat->Move( cGeom, pairMove, emMove, cErrObjects );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// 編集内容を保存する
sindyErrCode CContainer::Store( CErrorObjects& cErrObjects )
{
	for( iterator it = begin(); it != end(); ++it )
	{
		for( rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			// 変更があったかどうかはCRow::Store()内で行うため、ここでは
			// 単純にCRow::Store()を呼ぶだけでよい
			// 関連オブジェクトの保存はCRow::Store()で行う
			(*it2)->Store( cErrObjects );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// 整合性をチェックする
sindyErrCode CContainer::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const
{
	// TODO : チェッカーとして使うためには、チェック限定領域をCFeatures::CheckLogic()から渡せるようにしないとだめ。

	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			// 単体チェック
			//
			// 変更あるなしに関わらずやらないと内部コンテナまでチェックがいかない
			// 且つ、形状によらずにチェックが必要（bug 6410）
			(*it2)->CheckLogic(  /*g_cRule.CreateLogicRule( *it, */cRule/* )*/, cErrObjects );

			// 複合チェック
			//
			// 変更があり、且つ削除されていないもののみチェック
			// TODO：チェッカーとして使用するにはCLogicRuleにフラグを持たせる必要あり（このままでは非常にまずい）
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;
			(*it2)->Changed( &emUpdateType );
			if( emUpdateType == schema::ipc_table::update_type::kDeleted ||
				emUpdateType == schema::ipc_table::update_type::kDefault )
				continue;

			// 複合チェック
			CGeometry* pGeom = (*it2)->GetShape();
			if( pGeom )
			{
				IGeometryPtr ipEditArea;
				CErrorObject cErrObj( *it2->get() ); // エラー情報格納用

				// ポリゴンかつ変更があったオブジェクトで1000点以上構成点をもつもののみ変更領域取得
				if ( pGeom->GetGeometryType() == esriGeometryPolygon && emUpdateType != schema::ipc_table::update_type::kDefault && pGeom->GetPointCount() >= 1000 ) {
					// 変更領域取得
					ipEditArea = (*it2)->GetShapeCopy(true, true, false, true);
				}

				// 変更があったオブジェクトのみ
				if ( emUpdateType != schema::ipc_table::update_type::kDefault )
				{
					// 重複許可レイヤだったら飛ばす
					if( AheIsOverlapableLayer( AheTableType2TableName(it->first) ) )
						continue;

					// 形状重複チェック（bug 4963）
					for( const_rows_iterator it3 = it->second.begin(); it3 != it->second.end(); ++it3 )
					{
						if ( it2 != it3 ) {
							emUpdateType = schema::ipc_table::update_type::kDefault;

							// 削除されていないものとチェック
							if ( ! (*it3)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted )
							{
								CGeometry* pGeom2 = (*it3)->GetShape();
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
											if( pGeom->Overlaps( (IGeometry*)(*pGeom2) ) || 
												pGeom->Contains( (IGeometry*)(*pGeom2) ) ||
												pGeom->Within( (IGeometry*)(*pGeom2) ) ||
												pGeom->Equals( (IGeometry*)(*pGeom2) ) )
											{
												cErrObj.push_back( CErrorInfo( sindyErr_GeometryOverlapped, sindyErrLevel_ERR, (IGeometry*)(*(*it3)->GetShape()), -1, -1, DUPLICATE_OF_OID, (*it3)->GetOID() ) );
											}
										}
									}
								}
							}
						}
					}
				}
				if( sindyErr_NoErr != cErrObj.GetDefaultErrCode() )
					cErrObjects.push_back( cErrObj );
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// コンフリクト回避のための既編集チェックを行う
sindyErrCode CContainer::CheckModify( CErrorObjects& cErrObjects ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			(*it2)->CheckModify( cErrObjects );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// キャッシュされているかどうかをチェックする
bool CContainer::IsCached() const
{
	bool bRet = false;	// 返り値

	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			bool bCached = (*it2)->IsCached();
			if( ! bRet )
				bRet = bCached;
		}
	}

	return bRet;
}

CSPRow CContainer::find( _IRow* ipRow ) const
{
	return find( CRow(ipRow) );
}

CSPRow CContainer::find( const CRow& cRow ) const
{
	for( const_iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
		for( const_rows_iterator itRows = it->second.begin(), itRowsEnd = it->second.end(); itRows != itRowsEnd; ++itRows )
		{
			if( cRow == **itRows )
				return *itRows;

			CSPRow cRet( (*itRows)->find(cRow) );
			if( cRet.get() )
				return cRet;
		}
	}
	return CSPRow();
}

CContainer::rows_iterator CContainer::find( sindyTableType::ECode emType, long lOID )
{
	rows_iterator itEnd = end(emType);
#ifndef _USE_FIND_INDEX
	for( rows_iterator it = begin(emType); it != itEnd; ++it )
	{
		if( (*it)->GetOID() == lOID )
			return it;
	}
#else
	std::map<long,long>::const_iterator it = m_mapRowsContainerOIDIndex[emType].find(lOID);
	if( it != m_mapRowsContainerOIDIndex[emType].end() )
	{
		rows_iterator itRows = m_mapRowsContainer[emType].begin();
		std::advance( itRows, it->second );
		return itRows;
	}
#endif  //_USE_FIND_INDEX
	return itEnd;
}

CContainer::const_rows_iterator CContainer::find( sindyTableType::ECode emType, long lOID ) const
{
	const_rows_iterator itEnd = end(emType);
#ifndef _USE_FIND_INDEX
	for( const_rows_iterator it = begin(emType); it != itEnd; ++it )
	{
		if( (*it)->GetOID() == lOID )
			return it;
	}
#else
	TRowsContainerOIDIndex::const_iterator it = m_mapRowsContainerOIDIndex.find(emType);
	if( it != m_mapRowsContainerOIDIndex.end() )
	{
		std::map<long,long>::const_iterator itIndex = it->second.find(lOID);
		if( itIndex != it->second.end() )
		{
			const_rows_iterator itRows = m_mapRowsContainer.find(emType)->second.begin();
			std::advance( itRows, itIndex->second );
			return itRows;
		}
	}
#endif  //_USE_FIND_INDEX
	return itEnd;
}

CContainer::rows_iterator CContainer::find( sindyTableType::ECode emType, LPCTSTR lpcszFieldName, const VARIANT& vaValue )
{
	rows_iterator itEnd = end(emType);
	for( rows_iterator it = begin(emType); it != itEnd; ++it )
	{
		CComVariant vaTestValue;
		(*it)->GetValueByFieldName(lpcszFieldName, vaTestValue);
		if( vaTestValue == vaValue )
			return it;
	}
	return itEnd;
}
CContainer::const_rows_iterator CContainer::find( sindyTableType::ECode emType, LPCTSTR lpcszFieldName, const VARIANT& vaValue ) const
{
	const_rows_iterator itEnd = end(emType);
	for( const_rows_iterator it = begin(emType); it != itEnd; ++it )
	{
		CComVariant vaTestValue;
		(*it)->GetValueByFieldName(lpcszFieldName, vaTestValue);
		if( vaTestValue == vaValue ) 
			return it;
	}
	return itEnd;
}

void CContainer::erase( const CSPRow& cRow )
{
	sindyTableType::ECode emType = cRow->GetTableType();
	const_rows_iterator it = find( emType, cRow->GetOID() );

	if ( it != end(emType) )
		m_mapRowsContainer[emType].erase( std::remove( begin(emType), end(emType), cRow ), end(emType) );
	UpdateIndex();
}

CContainer::rows_iterator CContainer::erase( sindyTableType::ECode emType, rows_iterator position )
{
	return m_mapRowsContainer[emType].erase( position );
}

CContainer::rows_iterator CContainer::erase( sindyTableType::ECode emType, rows_iterator first, rows_iterator last )
{
	return m_mapRowsContainer[emType].erase( first, last );
}

// 現在格納されているフィーチャの存在矩形を返す
IEnvelopePtr CContainer::GetEnvelope( bool bOriginal/* = true*/, bool bChanged/* = true*/, bool bFeatureChangedOriginal/* = true*/ ) const
{
	IEnvelopePtr ipRet;
	for( const_iterator it = begin(); it != end(); it++ )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			IEnvelopePtr ipEnv( (*it2)->GetEnvelope( bOriginal, bChanged, bFeatureChangedOriginal ) );
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
	}
	return ipRet;
}

// 現在格納されているフィーチャの形状をマージしたものを返す
IGeometryPtr CContainer::GetShape( bool bLatest/* = true*/, bool bOriginal/* = false*/, bool bFeatureChangedOriginal/* = false*/, long lParam/* = -1*/ ) const
{
	// TODO: 一階層しか対応していない
	// ちゃんとやるなら、ジオメトリタイプごとに形状収集
	// -> ジオメトリタイプごとにマージ
	// -> GeometryBagに格納
	// とやったほうが効率が良い？
	IGeometryPtr ipRet;

	IGeometryCollectionPtr ipPointsCol, ipPolylinesCol, ipPolygonsCol;
	for( const_iterator itType = begin(); itType != end(); ++itType )
	{
		for( const_rows_iterator it = itType->second.begin(); it != itType->second.end(); ++it )
		{
			IGeometryPtr ipGeom = (*it)->GetShapeCopy( bLatest, bOriginal, bFeatureChangedOriginal, false, lParam );
			if( ipGeom != NULL )
			{
				esriGeometryType emType = esriGeometryAny;
				ipGeom->get_GeometryType( &emType );
				switch( emType )
				{
					case esriGeometryBag:
					{
						IEnumGeometryPtr ipEnumGeom( ipGeom );
						ipEnumGeom->Reset();
						IGeometryPtr ipGeomPart;
						while( S_OK == ipEnumGeom->Next( &ipGeomPart ) )
						{
							esriGeometryType emTypePart = esriGeometryAny;
							ipGeomPart->get_GeometryType( &emTypePart );
							switch( emTypePart )
							{
								case esriGeometryMultipoint:
								case esriGeometryPoint:
									if( ipPointsCol == NULL )
										ipPointsCol.CreateInstance( CLSID_GeometryBag );
									ipPointsCol->AddGeometry( ipGeomPart );
									break;
								case esriGeometryPolyline:
									if( ipPolylinesCol == NULL )
										ipPolylinesCol.CreateInstance( CLSID_GeometryBag );
									ipPolylinesCol->AddGeometry( ipGeomPart );
									break;
								case esriGeometryPolygon:
									if( ipPolygonsCol == NULL )
										ipPolygonsCol.CreateInstance( CLSID_GeometryBag );
									ipPolygonsCol->AddGeometry( ipGeomPart );
									break;
								default:
									_ASSERTE(false);
									break;
							}
						}
						break;
					}
					case esriGeometryMultipoint:
					case esriGeometryPoint:
						if( ipPointsCol == NULL )
							ipPointsCol.CreateInstance( CLSID_GeometryBag );
						ipPointsCol->AddGeometry( ipGeom );
						break;
					case esriGeometryPolyline:
						if( ipPolylinesCol == NULL )
							ipPolylinesCol.CreateInstance( CLSID_GeometryBag );
						ipPolylinesCol->AddGeometry( ipGeom );
						break;
					case esriGeometryPolygon:
						if( ipPolygonsCol == NULL )
							ipPolygonsCol.CreateInstance( CLSID_GeometryBag );
						ipPolygonsCol->AddGeometry( ipGeom );
						break;
					default:
						_ASSERTE(false);
						break;
				}
			}
		}
	}
	// それぞれをUnionする
	ITopologicalOperatorPtr ipPointsTopo, ipPolylinesTopo, ipPolygonsTopo;
	if( ipPointsCol != NULL )
	{
		ipPointsTopo.CreateInstance( CLSID_Multipoint );
		ipPointsTopo->ConstructUnion( (IEnumGeometryPtr)ipPointsCol );
	}
	if( ipPolylinesCol != NULL )
	{
		ipPolylinesTopo.CreateInstance( CLSID_Polyline );
		ipPolylinesTopo->ConstructUnion( (IEnumGeometryPtr)ipPolylinesCol );
	}
	if( ipPolygonsCol != NULL )
	{
		ipPolygonsTopo.CreateInstance( CLSID_Polygon );
		ipPolygonsTopo->ConstructUnion( (IEnumGeometryPtr)ipPolygonsCol );
	}
	// どれか一つしかなければそれを返り値とする
	bool bPoints    = ( ipPointsTopo != NULL && ipPolylinesTopo == NULL && ipPolygonsTopo == NULL );
	bool bPolylines = ( ipPointsTopo == NULL && ipPolylinesTopo != NULL && ipPolygonsTopo == NULL );
	bool bPolygons  = ( ipPointsTopo == NULL && ipPolylinesTopo == NULL && ipPolygonsTopo != NULL );
	if( bPoints || bPolylines || bPolygons )
	{
		if( bPoints )
			ipRet = ipPointsTopo;
		else if( bPolylines )
			ipRet = ipPolylinesTopo;
		else if( bPolygons )
			ipRet = ipPolygonsTopo;
	}
	else if( ipPointsTopo != NULL || ipPolylinesTopo != NULL || ipPolygonsTopo != NULL )
	{
		IGeometryCollectionPtr ipGeomCol( CLSID_GeometryBag );
		if( ipPointsTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPointsTopo );
		if( ipPolylinesTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPolylinesTopo );
		if( ipPolygonsTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPolygonsTopo );
		ipRet = ipGeomCol;
	}
	return ipRet;
}

sindyErrCode CContainer::CreateCache( bool bDetatch/* = false*/ )
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			sindyErrCode emErrTmp = (*it2)->CreateCache( bDetatch );
			if( sindyErr_NoErr != emErrTmp )
				return emErrTmp;
		}
	}
	return errorcode::sindyErr_NoErr;
}

// キャッシュ以外のパラメータを更新する
sindyErrCode CContainer::UpdateParameter( bool bForce/* = false*/ )
{
	for( iterator it = begin(); it != end(); ++it )
	{
		for( rows_iterator itRow = it->second.begin(); itRow != it->second.end(); ++itRow )
		{
			errorcode::sindyErrCode emErr = (*itRow)->UpdateParameter( bForce );
			if( errorcode::sindyErr_NoErr != emErr )
				return emErr;
		}
	}
	return errorcode::sindyErr_NoErr;
}
void CContainer::DeleteParameter()
{
	for( iterator it = begin(); it != end(); ++it )
	{
		for( rows_iterator itRow = it->second.begin(); itRow != it->second.end(); ++itRow )
			(*itRow)->DeleteParameter();
	}
}

void CContainer::Trace( bool bAttribute/* = true*/, bool bGeometry/* = true*/ ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			(*it2)->Trace( bAttribute, bGeometry );
	}
}

IGeometryPtr CContainer::GetEditArea() const
{
	IGeometryCollectionPtr ipGeomBag;

	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			// 形状変更と新規作成の変更があったオブジェクト
			if ( (*it2)->Changed( &emUpdateType ) && 
				( emUpdateType == schema::ipc_table::update_type::kShapeUpdated || emUpdateType == schema::ipc_table::update_type::kCreated ) ) {
				CGeometry* pGeom = (*it2)->GetShape();

				if( pGeom )
				{
					IGeometryPtr ipEditArea = (*it2)->GetShapeCopy(true, true, false, true);

					if ( ipEditArea )
					{
						if ( ! ipGeomBag ) {
							ipGeomBag.CreateInstance( CLSID_GeometryBag );
							((IGeometryPtr)ipGeomBag)->putref_SpatialReference( (*it2)->GetShape()->GetSpatialReference() );
						}

						ipGeomBag->AddGeometry( (IGeometryPtr)AheCreateClone( ipEditArea ) );
					}
				}
			}
		}
	}

	return ipGeomBag;
}

// find用のOIDインデックスを再構築する
void CContainer::UpdateIndex()
{
#ifdef _USE_FIND_INDEX
	m_mapRowsContainerOIDIndex.clear();
	for( const_iterator it = begin(); it != end(); ++it )
	{
		std::map<long,long>& mapOIDIndex( m_mapRowsContainerOIDIndex[it->first] );
		long i = 0;
		for( const_rows_iterator itRows = it->second.begin(); itRows != it->second.end(); ++itRows )
			mapOIDIndex[itRows->get()->GetOID()] = i++;
	}
#endif //_USE_FIND_INDEX
}
