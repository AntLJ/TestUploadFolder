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
 * @file FeatureContainer.cpp
 * @brief <b>CFeatureContainerクラス実装ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @vertion $Id$
 */
#include "stdafx.h"
#include "FeatureContainer.h"
#include "GlobalFunctions.h"
#include "ErrorObject.h"
#include "GeometryRule.h"
#include "Rule.h"
#include "GlobalRules.h"
#include "util.h"
#include "RowClassFactory.h"
#include "RuleModel.h"

namespace
{
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR NOT_EXIST_RELATIONS  = _T("[%s.%d] : [%s.%d] which should be related does not exist");
#else
	LPCTSTR NOT_EXIST_RELATIONS  = _T("[%s.%d] : 関連するはずの[%s.%d]が存在しません");
#endif // SINDY_FOR_ENGLISH
} // namespace


#if _MSC_VER >= 1700
	#include "TDC/useful_headers/text_util.h"
#else

#include<TDC/useful_headers/str_util.h>
namespace uh{
	namespace text_util{
		std::list<long> ToLongList( const CString& str )
		{
			std::list<long> lists;
			std::vector<CString> strlists = uh::str_util::split( str, _T(",") );
			for( std::vector<CString>::const_iterator it = strlists.begin(); it != strlists.end(); ++it )
			{
				lists.push_back( _ttol( *it ) );
			}
			return lists;
		}
	} // text_util
}// uh
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//

namespace sindy {
using namespace errorcode;

// 形状で検索する
sindyErrCode CFeatureContainer::Select( IGeometry* ipGeom, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit /*= true*/  )
{
	return Select( ipGeom, typeid(CFeature&), bRecycling, bCreateCache, pErrs, bInit );
}
sindyErrCode CFeatureContainer::Select( IGeometry* ipGeom, const type_info& info, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit /*= true*/  )
{
	RETURN_IF( ! p, sindyErr_ClassMemberMissing )

	// ジオメトリがNULLでも検索できるが、ここではあえて
	// エラーとすることで予期しない全件検索を回避する
	RETURN_IF( ! ipGeom, sindyErr_ArgIsNull )

	// 検索
	return Select( AheInitSpatialFilter( NULL, ipGeom ), info, bRecycling, bCreateCache, pErrs, bInit );
}
sindyErrCode CFeatureContainer::Select( IQueryFilter* ipFilter, const type_info& info, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit /*= true*/  )
{
	RETURN_IF(! ipFilter,sindyErr_ArgIsNull)

	// 検索
	return Add( Search( ipFilter, bRecycling ), info, GetTableType(), GetFieldMap(), GetNameString(), bRecycling, bCreateCache, pErrs, bInit );
}

// 検索
_ICursorPtr _Select( const CTable& table, const CString& strIDs, const CString& field, bool bRecycling = false )
{
	_ICursorPtr cur;
	
	if( field.IsEmpty() )
	{
		// フィールド指定がない場合はID直接指定で検索
		cur = table.GetRows( uh::text_util::ToLongList((LPCTSTR)strIDs), bRecycling );
	}
	else
	{
		// ある場合は文字列作成
		cur = table.Search( AheInitQueryFilter( NULL, NULL, _T("%s in (%s)"), field, strIDs  ), bRecycling );
	}
	return cur;
}

sindyErrCode CFeatureContainer::Select( LPCTSTR lpcszTableName, const std::list<long> listIDs, const type_info& info, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs/* = NULL*/, bool bInit/* = true*/, LPCTSTR lpcszWhereFieldName/* = NULL*/, LPCTSTR lpcszParentTableName/* = NULL*/ )
{
	if( listIDs.empty() )
		return sindyErr_NoErr;

	if( ! lpcszTableName )
		return sindyErr_ArgIsNull;

	if( 0 == lstrlen(lpcszTableName) )
		return sindyErr_ArgLimitOver;

	sindyErrCode emErr = sindyErr_NoErr; // 戻り値

	// 初期化
	if( bInit )
		clear();

	CTable cClass( OpenSameOwnerTable( lpcszTableName ) );
	sindyTableType::ECode emType = cClass.GetTableType();
	LOGASSERTEERR_IF( cClass != NULL, sindyErr_TableNotFound )
	{
		// 共有オブジェクトの取得
		CSPTableNameString spNameString( cClass.GetNameString() );
		CSPFieldMap spFieldMap( cClass.GetFieldMap() );

		
		std::list<CString> strIDs;
		IDs2Str( listIDs, strIDs );
		for( std::list<CString>::const_iterator itIDs = strIDs.begin(); itIDs != strIDs.end(); ++itIDs )
		{
			_ICursorPtr cur = _Select( cClass, *itIDs, lpcszWhereFieldName, bRecycling );
			LOGASSERTEERR_IF( cur, sindyErr_SearchFailed )
			{
				Add( cur, info, emType, spFieldMap, spNameString, bRecycling, bCreateCache, pErrs, bInit, lpcszWhereFieldName, lpcszParentTableName );
			}
		}

		ATLTRACE(_T("テーブル[%s] 検索：%d件\n"), lpcszTableName, size(emType) );
	}
	return emErr;
}
sindyErrCode CFeatureContainer::SelectByObject( _IRow* ipRow, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, bool bInit /*= true*/  )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	_ASSERTE(false); // TODO: この関数はおかしい！！

	// 初期化
	if( bInit )
		clear();

	CSPRow cFeat( new CFeature( ipRow ) ); // ipRowとthisのテーブルタイプが異なる場合を考慮

	if( cFeat == NULL )
		return emErr;

	// 同じフィーチャであったら登録はしない
	if( ! bInit )
	{
		for( const_iterator it = begin(); it != end(); ++it )
		{
			for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			{
				if( (*it2)->IsEqualObject((_IRow*)*cFeat) )
					return emErr;
			}
		}
	}

	push_back( cFeat ); // メンバに格納

	// リサイクルする場合はキャッシュを作る
	if( bRecycling || bCreateCache )
		cFeat->CreateCache( bRecycling ? true : false );

	return emErr;
}

// モデルで定義されているテーブルを紐付ける
void _Relate( std::map<long,CSPRow>& rows, const type_info& info, bool bRecycling = false, bool bCreateCache = false )
{
	if( rows.empty() )
		return;
	
	// 親のテーブルを確保
	CTable parentT( rows.begin()->second->GetTable() );

	// it <0>:親のテーブル名 <1>:削除対象のテーブル名 <2>:<1>のテーブルでキーとなるフィールド名
	for( CRuleRelationTable::const_value_iterator it = g_cRuleRelationTable.begin(parentT.GetTableName());
		it != g_cRuleRelationTable.end(parentT.GetTableName());
		++it )
	{
		CTable childT( parentT.OpenSameOwnerTable( it->get<1>() ) );
		CSPTableNameString spNameString( childT.GetNameString() );
		CSPFieldMap spFieldMap( childT.GetFieldMap() );

		// 1件ずつ検索するのは非効率なので、まとめてクエリを投げる
		std::list<long> ids;
		for( std::map<long,CSPRow>::const_iterator itRow = rows.begin();
			itRow != rows.end();
			++itRow )
		{
			ids.push_back( itRow->second->GetOID() );
		}

		if( ids.empty() )
			continue;

		// 自分の子供保持用
		std::map<long,CSPRow> childRows;

		std::list<CString> strIDs;
		CTable::IDs2Str( ids, strIDs );
		for( std::list<CString>::const_iterator itID = strIDs.begin();
			itID != strIDs.end();
			++itID
			)
		{
			_ICursorPtr cur = _Select( childT, *itID, it->get<2>(), bRecycling );

			long idx = spFieldMap->FindField( it->get<2>() );
			_IRowPtr row;
			int count = 0;
			while( S_OK == cur->NextRow( &row ) && row )
			{
				// 既にいたら無視
				CComVariant vaId = 0;
				row->get_Value( idx, &vaId );
				CRow& parent = *rows[vaId.lVal];
				if( parent.find( childT.GetTableType(), sindy::schema::kObjectID, CComVariant(AheGetOID(row)) ) !=
					parent.end( childT.GetTableType() ) )
					continue;

				CSPRow child( RowClassFactory( info, row, childT.GetTableType(), spFieldMap, spNameString ) );
				if( bRecycling || bCreateCache )
					child->CreateCache( bRecycling, spFieldMap, spNameString );

				// 自分の子どもとして登録
				parent.push_back( child, false );
				childRows[child->GetOID()] = child;
				++count;
			}

			ATLTRACE(_T("ADD : テーブル[%s] カーソル内件数：%d件\n"), parentT.GetTableName(), count );
		}

		// さらに関連づくものがあれば紐付ける
		_Relate( childRows, info, bRecycling, bCreateCache );
	}

}
sindyErrCode CFeatureContainer::Add( _ICursor* ipCursor, const type_info& info, sindyTableType::ECode emType, const CSPFieldMap& cFields, const CSPTableNameString& cNames, bool bRecycling/* = false*/, bool bCreateCache/* = false*/, CErrorObjects* pErrs /*= NULL*/, bool bInit/* = true*/, LPCTSTR lpcszOuterKeyFieldName/* = NULL*/, LPCTSTR lpcszParentTableName/* = NULL*/ )
{
	if( ! ipCursor )
		return sindyErr_ArgIsNull;

	// 初期化
	if( bInit )
		clear();

	sindyTableType::ECode emParentTableType = ( lpcszParentTableName ) ? AheTableName2TableType(lpcszParentTableName) : sindyTableType::other;
	long lOuterKeyFieldIndex = ( lpcszOuterKeyFieldName ) ? cFields->FindField( lpcszOuterKeyFieldName ) : -1;

	// bInitがfalseの場合にfind内でレコード単位で毎回ループ
	// まわすのは効率が悪すぎるので事前に対応表を作成しておく（bug 7301）
	std::map<long,CSPRow> mapRowIndex, mapParentRowIndex;
	std::set<long> setChildRowID;
	if( ! bInit )
	{
		for( const_rows_iterator it = begin(emType), itEnd = end(emType); it != itEnd; ++it )
		{
			mapRowIndex[(*it)->GetOID()] = *it;
		}

		if( sindyTableType::other != emParentTableType )
		{
			for( const_rows_iterator it = begin(emParentTableType), itEnd = end(emParentTableType); it != itEnd; ++it )
			{
				mapParentRowIndex[(*it)->GetOID()] = *it;

				// 内包オブジェクトの場合親オブジェクト単位でしか既存判定ができない
				// そのため、例えば道路リンクを分割処理してリンク内属性の親リンクが
				// 変更になった後に追加検索すると、既に検索済みにも関わらず同じリン
				// ク内属性が追加される事になる。
				// なので、事前に対応表を作成しておく
				for( const_rows_iterator it2 = (*it)->begin(emType); it2 != (*it)->end(emType); ++it2 )
					setChildRowID.insert((*it2)->GetOID());
			}
		}
	}

	long count = 0;
	_IRowPtr ipRow;
	while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow )
	{
		++count;

		long lOID = -1;
		ipRow->get_OID( &lOID );

		// 同じフィーチャであったら登録はしない
		if( ! bInit )
		{
			if( mapRowIndex.find(lOID) != mapRowIndex.end() ||    // ベースオブジェクトの場合
				setChildRowID.find(lOID) != setChildRowID.end() ) // ベースオブジェクトが保持する内包オブジェクトの場合
				continue;
		}

		CSPRow cRow( RowClassFactory( info, ipRow, emType, cFields, cNames ) );
		if( ! bInit )
			mapRowIndex[lOID] = cRow;

		// キャッシュ作成
		if( bRecycling || bCreateCache )
			cRow->CreateCache( bRecycling ? true : false, cFields, cNames );

		// 親テーブル名、外部キーフィールド名が指定されているなら関連付けて格納
		if( emParentTableType != sindyTableType::other && lOuterKeyFieldIndex > 0 )
		{
			//rows_iterator it = find( emParentTableType, cRow->GetLongValue( lOuterKeyFieldIndex, -1 ) );
			//LOGASSERTEM_IF( it != end( emParentTableType ), sindyErr_RelFeatureNotFound, NOT_EXIST_RELATIONS, cRow->GetTableName(), cRow->GetOID(), GetTableName(), cRow->GetLongValue( lOuterKeyFieldIndex, -1 ) )
			long keyId = cRow->GetLongValue( lOuterKeyFieldIndex, -1 );
			LOGASSERTEM_IF( mapParentRowIndex.find( keyId ) != mapParentRowIndex.end(), sindyErr_RelFeatureNotFound, NOT_EXIST_RELATIONS, cRow->GetTableName(), cRow->GetOID(), GetTableName(), cRow->GetLongValue( lOuterKeyFieldIndex, -1 ) )
				mapParentRowIndex[keyId]->push_back( cRow, false ); // 内包オブジェクトの重複は既に取り除いてあるのでここはユニークかどうか気にする必要はない

		}
		else
		{
			// そうでない場合は単純にコンテナに格納
			// bInitがtrueの時は、同じフィーチャがあるかどうかのチェックはいらない、
			// bInitがfalseの時は、既に上でチェックしているのでbSecureUniqueはfalseにしておく
			push_back( cRow, false );
		}
	}
	ATLTRACE(_T("ADD : テーブル[%s] カーソル内件数：%d件\n"), GetTableName(), count );

	// さらに関連する外部テーブルがあれば検索する
	std::map<long,CSPRow> mapOuter;
	for( std::map<long,CSPRow>::const_iterator itParent = mapParentRowIndex.begin();
		itParent != mapParentRowIndex.end();
		++itParent )
	{
		for( const_rows_iterator itChild = itParent->second->begin(emType);
			itChild != itParent->second->end(emType);
			++itChild )
		{
			mapOuter[(*itChild)->GetOID()] = *itChild;
		}
	}
	_Relate( mapOuter, info, bRecycling, bCreateCache );

	return sindyErr_NoErr;
}

} // sindy
