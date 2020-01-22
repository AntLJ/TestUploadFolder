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


// �w�肵���S���H���e�[�u����OID�̃��R�[�h�����݂��邩
bool CRelTableData::isExist( long lId ) const
{
	return 
		boost::find_if( m_RailwayLineData, [&lId](const RailwayLineRecord& cRailwayLineRecord ){
				return ( cRailwayLineRecord.lRailwayLineOid == lId);
			}) != m_RailwayLineData.end();
}


// �w�肵���S���H���e�[�u����ID�Ɉ�v���郌�R�[�h�̃C�e���[�^��Ԃ� 
CRelTableData::Iter CRelTableData::find( long lId )
{
	return
		boost::find_if( m_RailwayLineData, [&lId](const RailwayLineRecord& cRailwayLineRecord ){
				return ( cRailwayLineRecord.lRailwayLineOid == lId);
			});
}


// ID���d�����郌�R�[�h�����擾
int CRelTableData::getDupliCount( const CRelTableData& cData ) const
{
	return
		boost::count_if( cData.m_RailwayLineData, [&](const RailwayLineRecord& cRailwayLineRecord ){
			return isExist( cRailwayLineRecord.lRailwayLineOid );
	});
}


// ���R�[�h�̍폜(�S���H���e�[�u����OID���w��) 
void CRelTableData::erase(long lid)
{
	CIter iter = find(lid);
	if( iter == m_RailwayLineData.end())
		return;

	erase(iter);
}


// �H�����X�g�f�[�^�̃R�s�[
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
