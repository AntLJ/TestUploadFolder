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
#include "Output.h"

#include <strcode/INCLUDE/SJIS2EUC.H>

Output::~Output()
{
	if( m_ofs.is_open() )
		m_ofs.close();
}

void Output::Open( const uh::tstring& outputFilePath )
{
	m_ofs.imbue( std::locale("") );
	m_ofs.open(outputFilePath, std::ios::out | std::ios::binary);
}

bool Output::IsOpen()
{
	return m_ofs.is_open();
}

void Output::POIPointOutput( const POIPointRecordSet& recordSet )
{
	POIPointRecordSetItr& it = recordSet.begin();
	while( it != recordSet.end() )
	{
		// 電話番号と名称セットで見て、重複している場合とそうでない場合で場合分け
		CString strData;
		CString buildingID = _T("");
		int nCount = recordSet.count( *it );
		if( nCount == 1 )
		{
			if( it->m_buildingID > 0 )
				buildingID.Format( _T("%ld"), it->m_buildingID );

			strData.Format( _T("%ld\t%06d\t%d\t%d\t%ld\t%ld\t%s\t%d\t%s\t%s\t%s\t%s\t%d\t%s"),
				it->m_lOID,
				it->m_nMeshCode, it->m_nX, it->m_nY,								// 2次メッシュXY
				it->m_lGrpCode, it->m_lChainCode, it->m_strSeq, it->m_deleteCode,	// 分類・店舗・シーケンス・削除コード
				it->m_strTel, it->m_strName, it->m_strAddrCode1, it->m_strAddrCode2,// 電話番号・名称・住所コード
				it->m_nAddXYC, buildingID );										// 種別コード・家形ID
			++it;
		}
		else
		{
			std::pair<POIPointRecordSetItr, POIPointRecordSetItr>& itPair = recordSet.equal_range( *it );
			POIPointRecord cBasePOI( *it );

			do
			{
				// 種別による優先度(重要コンテンツ > 位置品質向上 > ピンポイント向上)
				// 精度による優先度( 1S > 3A > 1A > UK )
				int nBasePrior = cBasePOI.GetPriorValueFromAcc();
				int nCompPrior = itPair.first->GetPriorValueFromAcc();

				if( (cBasePOI.m_nAddXYC > itPair.first->m_nAddXYC)
					|| (cBasePOI.m_nAddXYC == itPair.first->m_nAddXYC && nBasePrior > nCompPrior)
					|| (cBasePOI.m_nAddXYC == itPair.first->m_nAddXYC && nBasePrior == nCompPrior && cBasePOI.m_lOID > itPair.first->m_lOID) )
				{
					cBasePOI = *itPair.first;
				}
				++itPair.first;
			}
			while( itPair.first != itPair.second );

			if( cBasePOI.m_buildingID > 0 )
				buildingID.Format( _T("%d"), cBasePOI.m_buildingID );

			strData.Format( _T("%ld\t%06d\t%d\t%d\t%ld\t%ld\t%s\t%d\t%s\t%s\t%s\t%s\t%d\t%s"),
				cBasePOI.m_lOID,
				cBasePOI.m_nMeshCode, cBasePOI.m_nX, cBasePOI.m_nY,										// 2次メッシュXY
				cBasePOI.m_lGrpCode, cBasePOI.m_lChainCode, cBasePOI.m_strSeq, cBasePOI.m_deleteCode,	// 分類・店舗・シーケンス・削除コード
				cBasePOI.m_strTel, cBasePOI.m_strName, cBasePOI.m_strAddrCode1, cBasePOI.m_strAddrCode2,// 電話番号・名称・住所コード
				cBasePOI.m_nAddXYC, buildingID );														// 種別コード・家形ID

			advance( it, nCount );
		}

		// EUCに変換
		char szConvData[1024] = "";
		SJIStoEUCString( CT2CA(strData), szConvData, sizeof(szConvData) );
		
		m_ofs << szConvData << "\n";
		m_ofs.flush();
	}
}

void Output::POISubPointOutput( const POISubPointRecordSet& recordSet )
{
	for( const auto& data : recordSet )
	{
		CString strData;
		strData.Format( _T("%ld\t%s\t%06d\t%ld\t%ld\t%s\t%d\t%s\t%d"), 
			data.m_poiPointId, // POI_POINT_ID
			data.m_funcSeq, // FuncSeq
			data.m_meshCode, // 2次メッシュコード
			data.m_x, // X
			data.m_y, // Y
			data.m_pstAttCode, // 座標値種別コード
			data.m_priority,// 座標値種別内優先順位
			data.m_priorityAttCode, // 座標値種別内優先順位種別コード
			data.m_tollRoadFlag ); // 有料道路フラグ

		// EUCに変換
		char szConvData[1024] = "";
		SJIStoEUCString( CT2CA(strData), szConvData, sizeof(szConvData) );

		m_ofs << szConvData << "\n";
		m_ofs.flush();
	}
}
