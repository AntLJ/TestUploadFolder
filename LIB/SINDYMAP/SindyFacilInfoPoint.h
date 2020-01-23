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

// SindyFacilInfoPoint.h: SindyFacilInfoPoint クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYFACILINFOPOINT_H__9377DB9C_73BF_46E2_B80B_F31D53F443B0__INCLUDED_)
#define AFX_SINDYFACILINFOPOINT_H__9377DB9C_73BF_46E2_B80B_F31D53F443B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyHighwayPoint.h"

class SindyFacilInfoPoint : public SindyHighwayPoint  
{
public:
	SindyFacilInfoPoint(SindyDBConnection *cDBConnection);
	virtual ~SindyFacilInfoPoint();

	/**
	 * @brief 路線コードと路線内シーケンス番号から施設情報ポイントの_IRowPtrを取得
	 * @note	SindyHighwayPoint::SearchByFacilCode()ではGDBHWYFACILRANGEが得られるが、
	 *			ある路線コード・路線内シーケンス番号を持つ施設情報ポイントは１つしかないはず(HC3002)なので、
	 *			そのただ１つの施設情報ポイントだけを返す関数をオーバーロード。
	 * @param[in]	cRoadCode	路線コード
	 * @param[in]	cRoadSeq	路線内シーケンス番号
	 * @return	見つかった施設情報ポイント（見つからない場合はNULL）
	 */
	_IRow* SearchByFacilCode(long cRoadCode, long cRoadSeq);

	using SindyHighwayPoint::SearchByFacilCode;		// オーバーロードにより見えなくなるのを防ぐ
};

#endif // !defined(AFX_SINDYFACILINFOPOINT_H__9377DB9C_73BF_46E2_B80B_F31D53F443B0__INCLUDED_)
