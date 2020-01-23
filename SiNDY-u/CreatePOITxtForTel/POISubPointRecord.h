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

#pragma once
class POISubPointRecord
{
public:
	POISubPointRecord() : m_poiPointId(0), m_meshCode(0), m_x(0), m_y(0), m_priority(0), m_tollRoadFlag(0){}

	bool operator<( const POISubPointRecord& ob ) const
	{
		if( m_poiPointId == ob.m_poiPointId )
		{
			return _ttol(m_funcSeq) < _ttol(ob.m_funcSeq);
		}
		else
		{
			return m_poiPointId < ob.m_poiPointId;
		}
	}

	long m_poiPointId; //!< POI_POINT_ID
	CString m_funcSeq; //!< 機能内シーケンス
	int m_meshCode; //!< 2次メッシュコード
	int m_x; //!< MeshX
	int m_y; //!< MeshY
	CString m_pstAttCode; //!< 座標値種別コード
	int m_priority; //!< 座標値種別内優先順位コード
	CString m_priorityAttCode; //!< 座標値種別内優先順位種別コード
	int m_tollRoadFlag; //!< 有料道路フラグ
};

