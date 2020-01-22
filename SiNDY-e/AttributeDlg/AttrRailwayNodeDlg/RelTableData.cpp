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


// �w�肵��������H��ID�̃��R�[�h�����݂��邩
bool CRelTableData::isExist( int nJoinLineId ) const
{
	return 
		boost::find_if( m_LineInfoData, [&nJoinLineId](const LineInfoRecord& cLineInfoRecord ){
				return (cLineInfoRecord.nJoinLineId == nJoinLineId);
			}) != m_LineInfoData.end();
}


// �w�肵��������H��ID�Ɉ�v���郌�R�[�h�̃C�e���[�^��Ԃ� 
CRelTableData::Iter CRelTableData::find( int nJoinLineId )
{
	return
		boost::find_if( m_LineInfoData, [&nJoinLineId](const LineInfoRecord& cLineInfoRecord ){
				return (cLineInfoRecord.nJoinLineId == nJoinLineId);
			});
}


// ID���d�����郌�R�[�h�����擾
int CRelTableData::getDupliCount( const CRelTableData& cData ) const
{
	return
		boost::count_if( cData.m_LineInfoData, [&](const LineInfoRecord& cLineInfoRecord ){
				return isExist( cLineInfoRecord.nJoinLineId );
			});
}


// ���R�[�h�̍폜(JOIN_LINE_ID �w��) 
void CRelTableData::erase(long lid)
{
	CIter iter = find(lid);
	if( iter == m_LineInfoData.end())
		return;

	erase(iter);
}
