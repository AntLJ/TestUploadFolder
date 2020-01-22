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

#include "boost/range/algorithm/find_if.hpp"
#include "boost/range/algorithm/count_if.hpp"
#include "RelTableData.h"


// 指定した乗り入れ路線IDのレコードが存在するか
bool CRelTableData::isExist( int nJoinLineId ) const
{
	return 
		boost::find_if( m_LineInfoData, [&nJoinLineId](const LineInfoRecord& cLineInfoRecord ){
				return (cLineInfoRecord.nJoinLineId == nJoinLineId);
			}) != m_LineInfoData.end();
}


// 指定した乗り入れ路線IDに一致するレコードのイテレータを返す 
CRelTableData::Iter CRelTableData::find( int nJoinLineId )
{
	return
		boost::find_if( m_LineInfoData, [&nJoinLineId](const LineInfoRecord& cLineInfoRecord ){
				return (cLineInfoRecord.nJoinLineId == nJoinLineId);
			});
}


// IDが重複するレコード数を取得
int CRelTableData::getDupliCount( const CRelTableData& cData ) const
{
	return
		boost::count_if( cData.m_LineInfoData, [&](const LineInfoRecord& cLineInfoRecord ){
				return isExist( cLineInfoRecord.nJoinLineId );
			});
}


// レコードの削除(JOIN_LINE_ID 指定) 
void CRelTableData::erase(long lid)
{
	CIter iter = find(lid);
	if( iter == m_LineInfoData.end())
		return;

	erase(iter);
}
