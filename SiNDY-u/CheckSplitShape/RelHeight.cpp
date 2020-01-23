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
#include "RelHeight.h"

bool CRelHeightAll::Init(ITablePtr ipRelHeightTable, const std::set<long>& setUsedLinkID)
{
	m_ipTable = ipRelHeightTable;
	m_strTableName = shp05::schema::road::rel_height::kTableName;

	if(!SetFieldInfo(m_strTableName, ipRelHeightTable))
		return false;

	m_setLinkID = setUsedLinkID;

	return true;
}

bool CRelHeightAll::LoadData()
{
	bool bReturn = true;

	_ICursorPtr ipCursor;
	if(!GetCursor(m_strTableName, m_ipTable, NULL, ipCursor))
		return false;

#ifdef _DEBUG
	long lRowCount = 0;
	if(!GetRowCount(m_strTableName, m_ipTable, NULL, lRowCount))
		return false;
	long n = 0;
#endif
	_IRowPtr ipRow;
	while(S_OK == ipCursor->NextRow(&ipRow) && ipRow){
#ifdef _DEBUG
		n++;
		if(0 == n % 100)
			std::cout << "   RelHeight : " << n << " / " << lRowCount << "\r" << std::flush;
#endif
		// 相対高さ情報はOBJECTIDとDIVIDでユニークに
		CRelHeight cRelHeight;
		if(!cRelHeight.SetData(m_strTableName, ipRow, m_mapFieldIndex2Name, m_mapFieldName2Index, m_mapAttrName2Index, shp05::schema::road::rel_height::kLinkId, shp05::schema::road::rel_height::kLinkDivId))
			return false;

		// 高さノードに設定されていたリンクIDだけ確保
		if(m_setLinkID.cend() == m_setLinkID.find(cRelHeight.eObjectID.eID))
			continue;

		// 相対高さ情報はOBJECTIDとDIVID（LINKIDとそのDIVID）でユニークに
		std::pair<std::map<UNIQID,CRelHeight>::const_iterator,bool> itInsert = m_mapData.insert(std::make_pair(cRelHeight.eObjectID, cRelHeight));
		if(!itInsert.second){
			// データ重複エラー
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::fatal, err_code::NgHNDuplicateData, m_strTableName, cRelHeight.eObjectID.eID, cRelHeight.eObjectID.eDivID);
			bReturn = false;
		}
		// SDE側の高さノードスキーマに合わせた形でデータ確保
		m_mapHeight[cRelHeight.eObjectID.eID][cRelHeight.eObjectID.eDivID] = std::make_pair(cRelHeight.eVecAttr.at(m_mapAttrName2Index.at(shp05::schema::road::rel_height::kFromLevel)).lVal, cRelHeight.eVecAttr.at(m_mapAttrName2Index.at(shp05::schema::road::rel_height::kToLevel)).lVal);

	}
#ifdef _DEBUG
	std::cout << "   RelHeight : " << n << " / " << lRowCount << std::endl;
#endif

	return bReturn;
}
