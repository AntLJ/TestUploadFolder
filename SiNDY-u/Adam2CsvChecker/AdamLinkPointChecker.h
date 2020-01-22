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

#include "LogicalChecker.h"

namespace sindy  {
namespace adam2  {
namespace format {


/**
 * @brief 車線リンク-車線ポイントの関係性チェッカー
 */
class CSimpleLinkPointChecker : public CAdam2LoglcalChecker
{
public:
	CSimpleLinkPointChecker() {}
	virtual ~CSimpleLinkPointChecker() {}

private:
	virtual bool run() override;
};


} // format
} // adam2
} // sindy
