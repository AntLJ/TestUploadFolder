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

#include <boost/range/algorithm.hpp>
#include "RelTableData.h"


// 指定した鉄道路線テーブルのOIDのレコードが存在するか
bool CRelTableData::isExist( long lId ) const
{
	return 
		boost::find_if( m_RailwayLineData, [&lId](const RailwayLineRecord& cRailwayLineRecord ){
				return ( cRailwayLineRecord.lRailwayLineOid == lId);
			}) != m_RailwayLineData.end();
}


// 指定した鉄道路線テーブルのIDに一致するレコードのイテレータを返す 
CRelTableData::Iter CRelTableData::find( long lId )
{
	return
		boost::find_if( m_RailwayLineData, [&lId](const RailwayLineRecord& cRailwayLineRecord ){
				return ( cRailwayLineRecord.lRailwayLineOid == lId);
			});
}


// IDが重複するレコード数を取得
int CRelTableData::getDupliCount( const CRelTableData& cData ) const
{
	return
		boost::count_if( cData.m_RailwayLineData, [&](const RailwayLineRecord& cRailwayLineRecord ){
			return isExist( cRailwayLineRecord.lRailwayLineOid );
	});
}


// レコードの削除(鉄道路線テーブルのOIDを指定) 
void CRelTableData::erase(long lid)
{
	CIter iter = find(lid);
	if( iter == m_RailwayLineData.end())
		return;

	erase(iter);
}


// 路線リストデータのコピー
void CRelTableData::copyData(CRelTableData& cData) const
{
	cData.clear();
	for( const auto& rec : m_RailwayLineData )
	{
		RailwayLineRecord cRecord;

		cRecord.lRailwayLineOid = rec.lRailwayLineOid;
		cRecord.strRailwayName = rec.strRailwayName;
		cRecord.strCorpName = rec.strCorpName;
		cData.add(cRecord);
	}
}
