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

#ifndef __SINDY_RINGS_H__
#define __SINDY_RINGS_H__

#include "exportimport.h"

namespace sindy {

/**
 * @brief 輪集クラス。
 */
class SINDYLIB_API Rings
{
public:
// 構築/消滅
	Rings();
	Rings(const Rings& rRings);
	~Rings();

// 設定/取得
	void clear();

	unsigned int size();

// 演算子のオーバーライド
	operator IRing**();
	IRing* operator[](unsigned int n);
	Rings& operator=(const Rings& rRings);

protected:
// 設定/取得
	IRing** init(unsigned int nCount);

private:
// 変数
	long m_nCount; ///< 輪数。
	IRingPtr* m_ppRings; ///< 輪配列。
};

/**
 * @brief 外輪集クラス。
 */
class SINDYLIB_API ExteriorRings : public Rings
{
public:
// 構築/消滅
	ExteriorRings();
	ExteriorRings(IPolygon2* ipPolygon2);

// 演算子のオーバーライド
	Rings& operator=(IPolygon2* ipPolygon2);
	Rings& operator=(const ExteriorRings& rRings);

private:
	void setExteriorRings(IPolygon2* ipPolygon2);
};

} // namespace sindy

#endif // __SINDY_RINGS_H__  //
