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

// DRMAAdminLocation.h: DRMAAdminLocation クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAADMINLOCATION_H__070D0D74_0E74_4238_8B84_D12CDC9238F8__INCLUDED_)
#define AFX_DRMAADMINLOCATION_H__070D0D74_0E74_4238_8B84_D12CDC9238F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMABackGround.h"

/**
 * @brief DRMA行政界位置データフォーマットクラス
 */
typedef DRMABackGroundFormat DRMAAdminLocationFormat;

/**
 * @brief DRMA行政界位置データレコードクラス
 */
class DRMAAdminLocation : public DRMABackGround
{
public:
// 構築/消滅
	DRMAAdminLocation(const Format& cOriginalRecord) : DRMABackGround(cOriginalRecord)
	{
	}

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAADMINLOCATION_H__070D0D74_0E74_4238_8B84_D12CDC9238F8__INCLUDED_)
