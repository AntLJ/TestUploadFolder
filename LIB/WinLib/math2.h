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

#define _USE_MATH_DEFINES // M_PI���̒�`��L���ɂ���
#include <math.h>

/**
 * @brief <b>�p�x���烉�W�A���֕ϊ�����</b>\n
 * @param angle	[in]	�p�x�i0�`360�j
 * @return ���W�A��
 */
double angle2rad( const double& angle );
inline double angle2rad( const double& angle ){ return ( angle * (double)M_PI ) / (double)180.0; }

/**
 * @brief <b>���W�A������p�x�֕ϊ�����</b>\n
 * @param rad	[in]	���W�A��
 * @return �p�x�i0�`360�j
 */
double rad2angle( const double& rad );
inline double rad2angle( const double& rad ){ return ( rad * (double)180.0 ) / (double)M_PI; }

#endif // ifndef WINLIB_MATH2_H_
