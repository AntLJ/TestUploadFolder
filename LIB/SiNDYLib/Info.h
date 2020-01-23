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

// Info.h: CInfo クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(_INFO_H_)
#define _INFO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"

namespace sindy {
namespace link_queue {


class SINDYLIB_API CInfo : public CRow
{
	ROWCLASS_CONSTRUCTOR( CInfo, unknown )

	/**
	 * @brief <b>リンク列形状を付与する</b>\n
	 * @param ipGeom	[in]	リンク列形状
	 */
	void SetLQGeometry( IGeometry* ipGeom );
};

} // link_queue

} // sindy 

#endif // !defined(_INFO_H_)
