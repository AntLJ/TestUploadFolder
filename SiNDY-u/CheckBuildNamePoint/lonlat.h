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

#include "stdafx.h"

/**
 * @brief   緯度経度クラス
 */
class cLonLat
{
public:

	/**
	 * @brief   コンストラクタ
	 */
	cLonLat() : m_lon(0.0), m_lat(0.0) {};

	/**
	 * @brief   コンストラクタ
	 * @param   ipPoint [in]  ポイント
	 */
	cLonLat( const IPointPtr& ipPoint ){
		init( ipPoint );
	};

	/**
	 * @brief   コンストラクタ
	 * @param   ipGeom  [in]  ジオメトリ
	 */
	cLonLat( const IGeometryPtr& ipGeom ){
		SetLonLat( ipGeom );
	};

	/**
	 * @brief   デストラクタ
	 */
	~cLonLat(){};

	/**
	 * @brief   ジオメトリセット
	 * @note    ラインやポリゴンの場合は、厳密に求めていない
	 */
	void SetLonLat( const IGeometryPtr& ipGeom ){
		esriGeometryType eType =  ipGeom->GetGeometryType();
		switch( eType ){
		case esriGeometryPoint:
			init( (IPointPtr)ipGeom );
			break;
		case esriGeometryPolyline:
			init( ((IPointCollectionPtr)ipGeom)->GetPoint( ((IPointCollectionPtr)ipGeom)->GetPointCount() / 2 ) );
			break;
		case esriGeometryPolygon:
			init( (IPointPtr)(IAreaPtr(IGeometryPtr(ipGeom->GetEnvelope()))->GetCentroid()) );
			break;
		default:
			init();
			break;
		}
	};

	/**
	 * @brief   緯度取得
	 * @retval  double   緯度
	 */
	double GetLat() const { return m_lat; };

	/**
	 * @brief   経度取得
	 * @retval  double   経度
	 */
	double GetLon() const { return m_lon; };

private:

	/**
	 * @brief   初期化
	 */
	void init(){ m_lon = 0.0; m_lat = 0.0; };

	/**
	 * @brief   ポイントから座標値セット
	 * @param   ipPoint [in]  ポイント
	 */
	void init( const IPointPtr& ipPoint ){
		init();
		if( ipPoint )
			ipPoint->QueryCoords( &m_lon, &m_lat );
	};

private:

	double m_lon; //!< 経度
	double m_lat; //!< 緯度
};

