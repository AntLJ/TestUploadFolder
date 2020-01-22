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

#include "RelStationSiteData.h"

// boost
#include <boost/range/algorithm.hpp>


// 駅ポリゴン紐付きテーブルのレコードが存在するか
bool CRelStationSiteData::isExist( const RelStationSiteRecord& record ) const
{
	return 
		boost::find_if( m_RelStationData, [&record](const RelStationSiteRecord& src_record ){
				return ( 
					src_record.lGroupId  == record.lGroupId  && 
					src_record.lLayerNoC == record.lLayerNoC && 
					src_record.lLayerOid == record.lLayerOid
					);
			}) != m_RelStationData.end();
}


// 一致する駅ポリゴン紐付きテーブルのレコードのイテレータを返す
CRelStationSiteData::Iter CRelStationSiteData::find( const RelStationSiteRecord& record )
{
	return
		boost::find_if( m_RelStationData, [&record](const RelStationSiteRecord& src_record ){
				return ( 
					src_record.lGroupId  == record.lGroupId  && 
					src_record.lLayerNoC == record.lLayerNoC && 
					src_record.lLayerOid == record.lLayerOid 
					);
			});
}


// 一致する駅ポリゴン紐付きテーブルのレコードのイテレータを返す
CRelStationSiteData::Iter CRelStationSiteData::find( long lGroupId )
{
	return
		boost::find_if( m_RelStationData, [&lGroupId](const RelStationSiteRecord& src_record ){
				return ( 
					src_record.lGroupId  == lGroupId 
					);
			});
}


// 重複するレコード数を取得する
int CRelStationSiteData::getDupliCount( const CRelStationSiteData& cData ) const
{
	return
		boost::count_if( cData.m_RelStationData, [&](const RelStationSiteRecord& record ){
			return isExist( record );
	});
}


// レコードの削除 
void CRelStationSiteData::erase( long lGroupId )
{
	auto& iter = find( lGroupId );
	if( iter != m_RelStationData.end() )
		erase( iter );
}
