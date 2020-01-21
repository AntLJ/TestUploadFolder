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

/*
**	MPQVer2
**
**		MPQ IO for format ver.2 class
**
**		Programmed by H.Miyahara
*/

#include "stdafx.h"
#include "MPQVer2.h"
#include "ZEndian.h"


/*
**	for MPQVer2::Info
*/

//! コンストラクタ
MPQVer2::Info::Info(void)
{
	*((int*)this) = 0;
	eRes_ = 1 << 10;
}

//! 属性値を取り出す
MPQVer2::Info MPQVer2::GetInfo(const MPQ::Attr& cAttr)
{
	MPQVer2::Info	aInfo;
	aInfo.eRes_    = (cAttr.eSurface&0xffe00000) >> 21;
	aInfo.eNotDraw = (cAttr.eSurface&0x00100000) >> 20;

	aInfo.eTunnel_Res = (cAttr.eSurface&0x000c0000) >> 18;
	aInfo.eTunnel_End = (cAttr.eSurface&0x00020000) >> 17;
	aInfo.eTunnel_Sct = (cAttr.eSurface&0x00010000) >> 16;

	aInfo.eObject_Res = (cAttr.eSurface&0x0000fc00) >> 10;
	aInfo.eObject_End = (cAttr.eSurface&0x00000200) >> 9;
	aInfo.eObject_Sct = (cAttr.eSurface&0x00000100) >> 8;

	aInfo.eHeight_Res   = (cAttr.eSurface&0x000000f0) >> 4;
	aInfo.eHeight_GType = (cAttr.eSurface&0x00000008) >> 3;
	aInfo.eHeight_GFlag = (cAttr.eSurface&0x00000004) >> 2;
	aInfo.eHeight_End   = (cAttr.eSurface&0x00000002) >> 1;
	aInfo.eHeight_Sct   = cAttr.eSurface & 0x00000001;

	return aInfo;
}

//! 属性値を設定する
void MPQVer2::SetInfo(MPQ::Attr* cAttr, const Info& cInfo)
{
	int	aVal = (cInfo.eRes_<<21) | (cInfo.eTunnel_Res<<18) | (cInfo.eObject_Res<<10) | (cInfo.eHeight_Res<<4);
	if(0 != cInfo.eNotDraw) aVal |= 0x00100000;

	if(0 != cInfo.eTunnel_End) aVal |= 0x00020000;
	if(0 != cInfo.eTunnel_Sct) aVal |= 0x00010000;

	if(0 != cInfo.eObject_End) aVal |= 0x00000200;
	if(0 != cInfo.eObject_Sct) aVal |= 0x00000100;

	if(0 != cInfo.eHeight_GType) aVal |= 0x00000008;
	if(0 != cInfo.eHeight_GFlag) aVal |= 0x00000004;
	if(0 != cInfo.eHeight_End)   aVal |= 0x00000002;
	if(0 != cInfo.eHeight_Sct)   aVal |= 0x00000001;

	cAttr->eSurface = aVal;
}

