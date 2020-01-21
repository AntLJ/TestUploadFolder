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
#include "SiNDYRings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

// 構築/消滅
//////////////////////////////////////////////////////////////////////

Rings::Rings() :
m_nCount(0),
m_ppRings(0)
{
}

Rings::Rings(const Rings& rRings) :
m_nCount(0),
m_ppRings(0)
{
	operator=(rRings);
}

Rings::~Rings()
{
	clear();
}

// 設定/取得
//////////////////////////////////////////////////////////////////////

/**
 * @brief 保持している輪配列を解放する
 */
void Rings::clear()
{
	delete [] m_ppRings;
	m_ppRings = 0;
	m_nCount = 0;
}

/**
 * @brief 保持している輪の数を取得する
 */
unsigned int Rings::size()
{
	return m_nCount;
}

/**
 * @brief 輪配列を初期化する
 *
 * @note 先に clear() すること。
 *
 * @param nCount [in] 輪の数。
 * @retval 初期化された配列の先頭ポインタ。
 */
IRing** Rings::init(unsigned int nCount)
{
	if(nCount) {
		m_ppRings = new IRingPtr[nCount];
		m_nCount = nCount;
	}

	return operator IRing**();
}

// 演算子のオーバーライド
//////////////////////////////////////////////////////////////////////

Rings::operator IRing**()
{
	return reinterpret_cast<IRing**>(m_ppRings);
}

/**
 * @brief 配列演算
 *
 * @param n [in] 配列番号。
 */
IRing* Rings::operator[](unsigned int n)
{
	return m_ppRings[n];
}

/**
 * @brief 複製演算
 *
 * @param rRings [in] 複製元輪集。
 */
Rings& Rings::operator=(const Rings& rRings)
{
	clear();
	init(rRings.m_nCount);
	for(int i = 0; i < m_nCount; i++) {
		m_ppRings[i] = rRings.m_ppRings[i];
		m_ppRings[i]->AddRef();
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////
// ExteriorRings
//////////////////////////////////////////////////////////////////////

ExteriorRings::ExteriorRings()
{
}

ExteriorRings::ExteriorRings(IPolygon2* ipPolygon2)
{
	if(ipPolygon2) {
		setExteriorRings(ipPolygon2);
	}
}

void ExteriorRings::setExteriorRings(IPolygon2* ipPolygon2)
{
	long nCount;
	if(ipPolygon2->get_ExteriorRingCount(&nCount) == S_OK && nCount > 0) {
		ipPolygon2->QueryExteriorRingsEx(nCount, init(nCount));
	}
}

Rings& ExteriorRings::operator=(IPolygon2* ipPolygon2)
{
	clear();
	if(ipPolygon2) {
		setExteriorRings(ipPolygon2);
	}

	return *this;
}

Rings& ExteriorRings::operator=(const ExteriorRings& rRings)
{
	return Rings::operator=(rRings);
}
