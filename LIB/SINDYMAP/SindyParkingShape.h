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

// SindyParkingShape.h: SindyParkingShape クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYPARKINGSHAPE_H__E7AC0324_0FE8_4077_92E8_E3F8E901EE49__INCLUDED_)
#define AFX_SINDYPARKINGSHAPE_H__E7AC0324_0FE8_4077_92E8_E3F8E901EE49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyPolygonClass.h"

class SindyParkingShape : public SindyPolygonClass  
{
public:
	SindyParkingShape(SindyDBConnection *cDBConnection);
	virtual ~SindyParkingShape();

};

#endif // !defined(AFX_SINDYPARKINGSHAPE_H__E7AC0324_0FE8_4077_92E8_E3F8E901EE49__INCLUDED_)
