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

// RNS.h
//
//////////////////////////////////////////////////////////////////////

#ifndef __RNS__
#define __RNS__

#ifdef _WIN32
#pragma warning(disable : 4290)
#endif // _WIN32 //

// ディレクトリ区切り文字
#if defined(macintosh)
#define	_pathChr ':'
#elif defined(_WIN32)
#define	_pathChr '\\'
#else
#define	_pathChr '/'
#endif

#define RNS_LOCATION "[" __FILE__ "]"

#include "RNSSindyCountry.h"
#include <crd_cnv/coord_converter.h>

extern crd_cnv g_cnv;

/**
 * @brief 方向列挙子
 *
 * 各々の値をビットOR結合することで
 */
typedef enum RNS_Direction {
	RNS_NO_DIR          =  0, ///< 方向無し
	RNS_UP              =  1, ///< 上
	RNS_DOWN            =  2, ///< 下
	RNS_UP_DOWN         =  3, ///< 上下
	RNS_LEFT            =  4, ///< 左
	RNS_UP_LEFT         =  5, ///< 左上
	RNS_DOWN_LEFT       =  6, ///< 左下
	RNS_UP_DOWN_LEFT    =  7, ///< 上下左
	RNS_RIGHT           =  8, ///< 右
	RNS_UP_RIGHT        =  9, ///< 右上
	RNS_DOWN_RIGHT      = 10, ///< 右下
	RNS_UP_DOWN_RIGTH   = 11, ///< 上下右
	RNS_LEFT_RIGHT      = 12, ///< 左右
	RNS_UP_LEFT_RIGHT   = 13, ///< 上左右
	RNS_DOWN_LEFT_RIGHT = 14, ///< 下左右
	RNS_FULL_DIR        = 15, ///< 全方向
};

/**
 * @brief ポイントの座標系
 */
enum CCoordinate{
	kMeshXY            = 1, //!< メッシュXY
	kLonLat            = 2, //!< 経緯度
	kRectangularCoords = 3, //!< 直交座標
	kVector            = 4, //!< RNSVector用
	kUnknownCoords     = -1 //!< 不明
};

/**
 * @brief ポイントの座標系、国情報
 */
class CInfo {
	CCoordinate m_Coordinate;
	country::country_type m_Country;
public:
	// デフォルトコンストラクタ
	CInfo(){ m_Coordinate = CCoordinate::kUnknownCoords; m_Country = country::country_type::japan; }

	// コンストラクタ
	CInfo(country::country_type countryType){ m_Coordinate = CCoordinate::kUnknownCoords; m_Country = countryType; }
	CInfo(CCoordinate coordinate, country::country_type countryType){ m_Coordinate = coordinate; m_Country = countryType; }

	CCoordinate coordinate(){ return m_Coordinate; }
	country::country_type country(){ return m_Country; }

	void operator=(CInfo &cInfo)
	{
		this->m_Coordinate = cInfo.m_Coordinate;
		this->m_Country = cInfo.m_Country;
	}

	bool operator==(CInfo &cInfo)
	{
		return (this->m_Coordinate == cInfo.m_Coordinate &&
			this->m_Country == cInfo.m_Country);
	}
};

#endif // __RNS__ //
