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
 * @file Table.cpp
 * @brief CTableクラス実装ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "Table.h"
#include "sindycomutil.h"
#include "util.h"
#include "TableTypeUtil.h"
#include "GlobalFunctions.h"
#include "GlobalRules.h"
#include "Rule.h"
#include "CreatableLayers.h"

#ifdef _DEBUG
#include "Geometry.h"
#endif // ifdef _DEBUG

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR GETROW_RETURN_FIRST_RECORD  = _T("%s : Warning : A number of record were found, but CTable::GetRow() returns only the record found first. \n");
#else
	LPCTSTR GETROW_RETURN_FIRST_RECORD  = _T("%s : 警告：複数のレコードが見つかりましたが、CTable::GetRow()は最初に見つかったレコードのみ返します。\n;");
#endif // SINDY_FOR_ENGLISH
} // namespace

namespace sindy {
using namespace errorcode;

// IClassインターフェース
IUIDPtr CTable::GetCLSID() const
{
	IUIDPtr ipUID;
	INVOKEMETHOD( get_CLSID( &ipUID ) );

	return ipUID;
}

long CTable::FindField( LPCTSTR Name ) const
{
	long lIndex = -1;
	if( ! Name ) return sindyErr_ArgIsNull;
	BSTR bstrName = T2BSTR(Name);
	INVOKEMETHOD( FindField( bstrName, &lIndex ) );
	SysFreeString(bstrName);

	return lIndex;
}

IFieldsPtr CTable::GetFields() const
{
	IFieldsPtr ipFields;
	INVOKEMETHOD( get_Fields( &ipFields ) );

	return ipFields;
}

bool CTable::HasOID() const
{
	VARIANT_BOOL vbHasOID = VARIANT_FALSE;
	INVOKEMETHOD( get_HasOID( &vbHasOID ) );

	return VB2bool(vbHasOID);
}

// ITableインターフェース
CRow& CTable::CreateRow( CRow& cRow )
{
	_IRowPtr ipRow;
	LOGASSERTE_IF( p, sindyErr_ClassMemberMissing )
	{
		SUCCEEDED_IF( p->CreateRow( &ipRow ), sindyErr_AOCreateRowFailed, _T("TABLE[%s]"), GetTableName() )
		{
			cRow.SetObject( ipRow, GetTableType(), true );

			// デフォルト値付与
			cRow.SetDefaultValue();

			// 新規作成自動属性付与
			cRow.SetCreateAutoValue();
		}
	}
	return cRow;
}

CRow& CTable::CreateDummyRow( CRow& cRow )
{
	CSPFieldMap cFields;
	CSPTableNameString cNames;
	if( (_IRow*)cRow )
	{
		cFields = cRow.GetFieldMap();
		cNames = cRow.GetNameString();
	}

	if ( ! cFields ) {	// ないときは自分のを使用
		cFields = GetFieldMap();
		cNames = GetNameString();
	}

	// 実体がないのでキャッシュだけ作っておく
	cRow.CreateCache( false, cFields, cNames );

	// デフォルト値付与
	cRow.SetDefaultValue();

	// 新規作成自動属性付与
	cRow.SetCreateAutoValue();

	return cRow;
}

sindyErrCode CTable::DeleteSearchedRows( IQueryFilter* QueryFilter )
{
	// レイアウトビューに切り替えるとGeometryBagでの検索に失敗する対応
//	WKSEnvelope wksenv={0};
//	ISpatialReferencePtr ipSpRef( resetSpRef( QueryFilter, wksenv ) );

	INVOKEMETHOD3( QueryFilter, DeleteSearchedRows );

	// 元に戻しておく
//	if ( ipSpRef )
//		ipSpRef->SetDomain( wksenv.XMin, wksenv.XMax, wksenv.YMin, wksenv.YMax );
}

_IRowPtr CTable::GetRow( long OID ) const
{
	_IRowPtr ipRow;
	INVOKEMETHOD( GetRow( OID, &ipRow ) );

	return ipRow;
}

_IRowPtr CTable::GetRow( IQueryFilter* QueryFilter ) const
{
	_IRowPtr ipRet;	// 返り値

	_ICursorPtr ipCursor = Search( QueryFilter, false );
	LOGASSERTE_IF( NULL != ipCursor, sindyErr_SearchFailed )
	{
		_IRowPtr ipRow;
		while( S_OK == ipCursor->NextRow( &ipRow ) )
		{
			if( NULL == ipRet )
				ipRet = ipRow;
			else {
				TRACEMESSAGE(GETROW_RETURN_FIRST_RECORD, __FILE__LINE__ );
				break;
			}
		}
	}

	return ipRet;
}

_ICursorPtr CTable::GetRows( const VARIANT& oids, bool Recycling ) const
{
	_ICursorPtr ipCursor;
	INVOKEMETHOD( GetRows( oids, bool2VB(Recycling), &ipCursor ) );

	return ipCursor;
}

long CTable::RowCount( IQueryFilter* QueryFilter ) const
{
	// レイアウトビューに切り替えるとGeometryBagでの検索に失敗する対応
//	WKSEnvelope wksenv={0};
//	ISpatialReferencePtr ipSpRef( resetSpRef( QueryFilter, wksenv ) );

	long lCount = -1;
	INVOKEMETHOD( RowCount( QueryFilter, &lCount ) );

	// 元に戻しておく
//	if ( ipSpRef )
//		ipSpRef->SetDomain( wksenv.XMin, wksenv.XMax, wksenv.YMin, wksenv.YMax );

	return lCount;
}

#ifdef _DEBUG
void _QueryFilter_Trace( LPCTSTR lpcszTableName, IQueryFilter* ipFilter, bool bRecycling )
{
	if( ipFilter )
	{
		// クエリフィルタのトレース
		CComBSTR bstrSubFields, bstrWhere;
		ipFilter->get_SubFields( &bstrSubFields );
		ipFilter->get_WhereClause( &bstrWhere );
		CString strWhere(bstrWhere);
		if( strWhere.GetLength() > 80 )
			strWhere = strWhere.Left(80) + _T("...");

		ISpatialFilterPtr ipSpFilter( ipFilter );
		if( NULL != ipSpFilter )
		{
			VARIANT_BOOL vbSpatialFilterOwnsGeometry = VARIANT_FALSE;
			IGeometryPtr ipGeom;
			CComBSTR bstrGeomField, bstrSpatialRelDesc;
			esriSearchOrder emOrder = esriSearchOrderSpatial;
			esriSpatialRelEnum emSpatialRel = esriSpatialRelIntersects;

			ipSpFilter->get_FilterOwnsGeometry( &vbSpatialFilterOwnsGeometry );
			ipSpFilter->get_Geometry( &ipGeom );
			ipSpFilter->get_GeometryField( &bstrGeomField );
			ipSpFilter->get_SearchOrder( &emOrder );
			ipSpFilter->get_SpatialRel( &emSpatialRel );
			ipSpFilter->get_SpatialRelDescription( &bstrSpatialRelDesc );
			ATLTRACE(_T("検索[%s]（リサイクル：%s）：クエリフィルタ：SubFields[%s] Where[%s] SFOG[%s] GEOMF[%s] DESC[%s] ORDER[%s] REL[%d]\n"), lpcszTableName, ( bRecycling ) ? _T("する") : _T("しない"), OLE2T(bstrSubFields), strWhere, ( vbSpatialFilterOwnsGeometry ) ? _T("TRUE") : _T("FALSE"), COLE2CT(bstrGeomField), COLE2CT(bstrSpatialRelDesc), ( emOrder == esriSearchOrderSpatial ) ? _T("SPATIAL") : _T("ATTRIBUTE"), emSpatialRel );
			CGeometry::Trace( ipGeom, _T("                      ：形状") );
			IAreaPtr ipArea( ipGeom );
			if( ipArea )
			{
				double dArea = 0;
				ipArea->get_Area( &dArea );
				ATLTRACE( _T("                      ：面積[%.15lf]\n"), dArea );
			}
		}
		else {
			ATLTRACE(_T("検索[%s]（リサイクル：%s）：クエリフィルタ：SubFields[%s] Where[%s]\n"), lpcszTableName, ( bRecycling ) ? _T("する") : _T("しない"), COLE2CT(bstrSubFields), strWhere );
		}
	}
}
#else
#define _QueryFilter_Trace ((void)0);
#endif // ifdef _DEBUG

_ICursorPtr CTable::Search( IQueryFilter* QueryFilter, bool Recycling ) const
{
	_ICursorPtr ipCursor;
	_QueryFilter_Trace( GetTableName(), QueryFilter, Recycling );

	// レイアウトビューに切り替えるとGeometryBagでの検索に失敗する対応
//	WKSEnvelope wksenv={0};
//	ISpatialReferencePtr ipSpRef( resetSpRef( QueryFilter, wksenv ) );

	SUCCEEDED_IF( p->Search( QueryFilter, bool2VB(Recycling), &ipCursor ), sindyErr_AOFeatureSearchError, NULL )
	{
	}

	// 元に戻しておく
//	if ( ipSpRef )
//		ipSpRef->SetDomain( wksenv.XMin, wksenv.XMax, wksenv.YMin, wksenv.YMax );

	return ipCursor;
}
void CTable::SetObject( sindyTableType::ECode emTableType, ITable *lp)
{
	SetObject( lp );
	m_emTableType = emTableType;	// SetObject( ITable* lp )内部でm_emTableTypeを初期化するので後でセットする必要がある
}

void CTable::SetObject( ITable* lp, const CSPFieldMap& cFields/* = CSPFieldMap()*/, const CSPTableNameString& cNames/* = CSPTableNameString()*/ )
{
	AtlComPtrAssign( (IUnknown**)&p, lp );
	
	m_emTableType = sindyTableType::unknown;
	m_spFields = cFields;
	m_spNameString = cNames;
}

// フィールド対応表の参照を返す
CSPFieldMap CTable::GetFieldMap() const
{
	if( ! m_spFields )
		m_spFields.reset( new CFieldMap( GetFields() ) );

	return m_spFields;
}

// テーブル名などを管理するメンバの共有コピーを返す
CSPTableNameString CTable::GetNameString() const
{
	if( ! m_spNameString )
		m_spNameString.reset( new CTableNameString( (ITable*)*this ) );

	return m_spNameString;
}

// 同じワークスペースに存在するテーブルをオープンする
ITablePtr CTable::OpenTable( LPCTSTR lpcszTableName ) const
{
	ITablePtr ipTable;
	IFeatureWorkspacePtr ipWorkspace( GetWorkspace() );
	if( NULL != ipWorkspace )
	{
		USES_CONVERSION;

		BSTR bstrTableName = T2BSTR(lpcszTableName);
		ipWorkspace->OpenTable( bstrTableName, &ipTable );
		::SysFreeString(bstrTableName);
	}
	return ipTable;
}

// 同じワークスペースに存在するテーブルをオープンする
ITablePtr CTable::OpenTable( LPCTSTR lpcszOwnerName, sindyTableType::ECode emTableType ) const
{
	CString strTableDotOwnerName;
	if( lpcszOwnerName && 0 < lstrlen(lpcszOwnerName) )
		strTableDotOwnerName.Format(_T("%s.%s"), lpcszOwnerName, AheTableType2TableName( emTableType ) );
	else
		strTableDotOwnerName = AheTableType2TableName( emTableType );

	return OpenTable( strTableDotOwnerName );
}

// 同じワークスペースに存在する同じオーナー名のテーブルをオープンする
ITablePtr CTable::OpenSameOwnerTable( LPCTSTR lpcszTableName ) const
{
	CString strTableName(lpcszTableName);

	// オーナー名付きのテーブル名の場合
	if( _tcschr( lpcszTableName, _T('.') ) )
	{
		// オーナー名を取る
		int nPeriodIndex = strTableName.Find( _T('.'), 0 );
		strTableName = strTableName.Right( strTableName.GetLength() - nPeriodIndex - 1 );
	}

	if( GetNameString()->HasOwnerName() )
		return OpenTable( CString(GetOwnerName()) + _T(".") + strTableName );
	else
		return OpenTable( strTableName );
}

// 同じワークスペースに存在する同じオーナー名のテーブルをオープンする
ITablePtr CTable::OpenSameOwnerTable( sindyTableType::ECode emTableType ) const
{
	return OpenTable( GetOwnerName(), emTableType );
}

// ワークスペースを取得する
IWorkspacePtr CTable::GetWorkspace() const
{
	IWorkspacePtr ipWorkspace;
	IDatasetPtr ipDataset( p );
	if( NULL != ipDataset )
		ipDataset->get_Workspace( &ipWorkspace );

	return ipWorkspace;
}

/*// OIDリストで検索する
_ICursorPtr CTable::GetRows( const std::list<long>& fids, bool Recycling ) const
{
	SAFEARRAYBOUND bound = { fids.size(), 0 };
	SAFEARRAY* pSa = SafeArrayCreate( VT_I4, 1, &bound );
	LONG* pArray;
	SafeArrayAccessData( pSa, (void**)&pArray );

	long i = 0;
	for( std::list<long>::const_iterator it = fids.begin(); it != fids.end(); ++it, ++i )
		pArray[i] = *it;

	SafeArrayUnaccessData( pSa );
	VARIANT vaArray;
	VariantInit( &vaArray );
	vaArray.vt = VT_ARRAY | VT_I4;
	vaArray.parray = pSa;

	_ICursorPtr ipCursor = GetRows( vaArray, Recycling );
	VariantClear( &vaArray );

	return ipCursor;
}*/

// OIDリストで検索する
_ICursorPtr CTable::GetRows( const std::list<long>& fids, bool Recycling, LPCTSTR lpcszRelIDFieldName ) const
{
	_ICursorPtr ipRet; // 返り値

	// 渡されたID列をSQL用の文字列に変換
	std::list<CString> strIDs;
	IDs2Str( fids, strIDs );

	// 関数上の制限により、複数文字列となる場合はエラーとする
	LOGASSERTE_IF( 1 >= strIDs.size(), sindyErr_ArgLimitOver )
		ipRet = Search( AheInitQueryFilter( NULL, NULL, _T("%s IN(%s)"), lpcszRelIDFieldName, *strIDs.begin() ), Recycling );

	return ipRet;
}

sindyTableType::ECode CTable::GetTableType() const
{
	LOGASSERTE_IF( p, sindyErr_ClassMemberMissing )
	{
		if( sindyTableType::unknown == m_emTableType )
			m_emTableType = AheTableName2TableType( GetNameString()->GetTableName() );
	}

	return m_emTableType;
}

// TODO:次回修正するときに、この関数削除しちゃってください
// Ver8.0.1では、まだとりあえず残しておきます。
// 空間参照再設定
ISpatialReferencePtr CTable::resetSpRef(IQueryFilter* QueryFilter, WKSEnvelope& wksenv) const
{
	// 
	ISpatialReferencePtr ipSpRef;
	ISpatialFilterPtr ipSpFilter( QueryFilter );

	if ( ipSpFilter )
	{
		IGeometryPtr ipGeom;
		ipSpFilter->get_Geometry( &ipGeom );

		if ( ipGeom )
		{
			esriGeometryType emtype = esriGeometryAny;
			ipGeom->get_GeometryType( &emtype );

			if ( esriGeometryBag == emtype )	// GeometryBagの時だけ
			{
				IGeometryPtr ipGeom_Shift( (IGeometryPtr)AheCreateClone( ipGeom ) );

				// 空間参照とって...(空間参照のコピーではだめなの)
				ipGeom_Shift->get_SpatialReference( &ipSpRef );
				if ( ipSpRef )
				{
					ipSpRef->GetDomain( &wksenv.XMin, &wksenv.XMax, &wksenv.YMin, &wksenv.YMax );

					// シフトして...
					double dbShift = 156.25f;
					ipSpRef->SetDomain( wksenv.XMin - dbShift, wksenv.XMax - dbShift, wksenv.YMin - dbShift, wksenv.YMax - dbShift );

					// セットしなおし
					CGeometry cGeom( ipGeom_Shift );
					for ( CGeometry::part_iterator itPart = cGeom.part_begin(); itPart != cGeom.part_end(); ++itPart )
					{
						if ( (*itPart) )
							(*itPart)->putref_SpatialReference( ipSpRef );
					}

					ipGeom_Shift->putref_SpatialReference( ipSpRef );
					ipSpFilter->putref_Geometry( ipGeom_Shift );
				}
			}
		}
	}

	return ipSpRef;
}

} // sindy
