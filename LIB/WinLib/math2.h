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

#ifndef WINLIB_MATH2_H_
#define WINLIB_MATH2_H_

#define _USE_MATH_DEFINES // M_PI等の定義を有効にする
#include <math.h>

/**
 * @brief <b>角度からラジアンへ変換する</b>\n
 * @param angle	[in]	角度（0～360）
 * @return ラジアン
 */
double angle2rad( const double& angle );
inline double angle2rad( const double& angle ){ return ( angle * (double)M_PI ) / (double)180.0; }

/**
 * @brief <b>ラジアンから角度へ変換する</b>\n
 * @param rad	[in]	ラジアン
 * @return 角度（0～360）
 */
double rad2angle( const double& rad );
inline double rad2angle( const double& rad ){ return ( rad * (double)180.0 ) / (double)M_PI; }

#endif // ifndef WINLIB_MATH2_H_
