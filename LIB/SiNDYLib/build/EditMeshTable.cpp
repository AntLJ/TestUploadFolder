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
 * @file EditMeshTable.cpp
 * @breif <b>CEditMeshTableクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "EditMeshTable.h"
#include "meshutil.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace editmesh {

// テーブルを全てキャッシュする
long CEditMeshTable::CreateTableCache( bool bDetatch/* = false*/, long lPurposeID/* = -1*/ )
{
//	TRACEMESSAGE( _T("CEditMeshTable::CreateTableCache() : Started.\n") );

	CEditMesh cEditMesh;
	_ICursorPtr ipCursor;
	if( 0 > lPurposeID )
		ipCursor = Search( NULL, bDetatch );
	else
		ipCursor = Search( AheInitQueryFilter( NULL, _T("*"), _T("PURPOSE_ID=%d"), lPurposeID ), bDetatch );

	// 共有オブジェクト作成
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	while( SUCCEEDED( ipCursor->NextRow( cEditMesh.Release() ) ) )
	{
		if( cEditMesh == NULL ) break;

		// 作業目的IDとレイヤ名IDとオペレータ名IDをキーにして格納
		long lPurposeID = cEditMesh.GetPurposeID();
		long lLayerNameID = cEditMesh.GetLayerNameID();
		long lOperatorID = cEditMesh.GetOperatorID();

		// 既にあるかどうかチェック
		bool bFind = false;
		std::map<long, std::map<long, std::map<long, std::pair<CGeometry, std::list<CEditMesh> > > > >::iterator it1 = m_mapCache.find( lPurposeID );
		if( it1 != m_mapCache.end() )
		{
			std::map<long, std::map<long, std::pair<CGeometry, std::list<CEditMesh> > > >::iterator it2 = it1->second.find( lLayerNameID );
			if( it2 != it1->second.end() )
			{
				std::map<long, std::pair<CGeometry, std::list<CEditMesh> > >::iterator it3 = it2->second.find( lOperatorID );
				if( it3 != it2->second.end() )
				{
					it3->second.second.push_front( cEditMesh );
					it3->second.second.begin()->CreateCache( bDetatch, spFields, spNameString );
					it3->second.first.Union( MESH2POLY( cEditMesh.GetMeshCode(), NULL ) );

					bFind = true;
				}
			}
		}

		// なければ、新規登録
		if( ! bFind )
		{
			std::pair<CGeometry, std::list<CEditMesh> > pairEditMeshes;
			pairEditMeshes.first = MESH2POLY( cEditMesh.GetMeshCode(), NULL );
			m_mapCache[ lPurposeID ][ lLayerNameID ][ lOperatorID ] = pairEditMeshes;
			m_mapCache[ lPurposeID ][ lLayerNameID ][ lOperatorID ].second.push_front( cEditMesh );
			m_mapCache[ lPurposeID ][ lLayerNameID ][ lOperatorID ].second.begin()->CreateCache( bDetatch, spFields, spNameString );
		}
/*	
		m_mapCache[ lPurposeID ][ lLayerNameID ][ lOperatorID ].second.push_front( cEditMesh );
		m_mapCache[ lPurposeID ][ lLayerNameID ][ lOperatorID ].second.begin()->CreateCache( bDetatch, ipFields );
		// メッシュコードを矩形にしてマージ
		m_mapCache[ lPurposeID ][ lLayerNameID ][ lOperatorID ].first.Union( MESH2POLY( cEditMesh.GetMeshCode(), NULL ) );*/
	}

	m_bCached = true;

	// 切り離し
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CEditMeshTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();
}

// キャッシュをクリアする
void CEditMeshTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

// 編集可能なエリアかどうかをチェックする
bool CEditMeshTable::IsEditable( long lPurposeID, long lLayerNameID, long lOperatorID, IGeometry* ipGeom ) const
{
	// 作業エリアが登録されているなら
	// それを最優先で検証
	// 超特別対応、都市注記文字列は、PGDB範囲外にもおけるため下のチェックをしない
	// 26固定でいいの？ちと危ない、、
	if( lLayerNameID != 26 )
	{
		if( (IGeometry*)m_cGlobalWorkArea )
		{
			IGeometryPtr ipGlobalWorkAreaCopy( AheCreateClone( (IGeometry*)m_cGlobalWorkArea ) );
			if( ipGlobalWorkAreaCopy != NULL )
			{
				// GlobalWorkAreaは性質上空間参照が空で入っているので、
				// ここで空間参照をちゃんとあわせないと縁の部分で編集
				// できなくなる
				if( ipGeom )
				{
					ISpatialReferencePtr ipSpRef;
					ipGeom->get_SpatialReference( &ipSpRef );
					ipGlobalWorkAreaCopy->putref_SpatialReference( ipSpRef );
					ipGlobalWorkAreaCopy->SnapToSpatialReference();

					// [Bug 8164][e]SiNDY-c使用時のPGDB編集でメッシュ境界上のノードが移動できない
					// メッシュ境界上のポイントは、Containじゃ判定できないので、Disjointで
					esriGeometryType type = esriGeometryAny;
					ipGeom->get_GeometryType( &type );
					if( type == esriGeometryPoint )
					{
						CGeometry cGeom( ipGlobalWorkAreaCopy );
						if( cGeom.Disjoint( ipGeom ) )
							return false;
					}
					else
					{
						if( ! AheGeometryContains( ipGlobalWorkAreaCopy, ipGeom ) )
							return false;
					}
				}
			}
		}
	}

	// 引数チェック
	if( 0 > lOperatorID )	// [bug 3094] オペレータが登録されていなければ全て編集可能である
		return true;
	else
	{
		std::map<long, std::map<long, std::map<long, std::pair<CGeometry, std::list<CEditMesh> > > > >::const_iterator itByPurposeID = m_mapCache.find( lPurposeID );
		if( itByPurposeID != m_mapCache.end() )
		{
			std::map<long, std::map<long, std::pair<CGeometry, std::list<CEditMesh> > > >::const_iterator itByLayerNameID = itByPurposeID->second.find( lLayerNameID );
			if( itByLayerNameID != itByPurposeID->second.end() )
			{
				std::map<long, std::pair<CGeometry, std::list<CEditMesh> > >::const_iterator itByOperatorID = itByLayerNameID->second.find( lOperatorID );
				if( itByOperatorID != itByLayerNameID->second.end() )
					return itByOperatorID->second.first.Contains( ipGeom );
			}
		}
	}

	return false;
}

// 作業可能エリアを設定する
void CEditMeshTable::SetGlobalWorkArea( IGeometry* ipGeom )
{
	if( ! ipGeom )
		m_cGlobalWorkArea = NULL;
	else
		m_cGlobalWorkArea = (IGeometryPtr)AheCreateClone( ipGeom );
}

// 作業可能エリアを取得する
const CGeometry& CEditMeshTable::GetGlobalWorkArea() const
{
	return m_cGlobalWorkArea;
}

} // editmesh

} // sindy
