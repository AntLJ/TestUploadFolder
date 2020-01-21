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

//#include "stdafx.h"
/*
**  MPQVer3
**	
**		MPQ IO for format ver.3 class
**
**		Programmed by Y.Chinen
*/

#include "stdafx.h"
#include "MPQVer3.h"
#include "ZEndian.h"


/*
**	for MPQVer3::Info
*/

//! ∫›Ωƒ◊∏¿

MPQVer3::Info::Info(void)
{
	*((int*)this) = 0;
	eRes_ = 1 << 10;
}

//!ëÆê´ílÇéÊÇËèoÇ∑
MPQVer3::Info MPQVer3::GetInfo(const MPQ::Attr &cAttr)
{
	MPQVer3::Info	aInfo;
	aInfo.eRes_			= (cAttr.eSurface&0xffc00000) >> 22;
	aInfo.eDeletion_End	= (cAttr.eSurface&0x00200000) >> 21;
	aInfo.eDeletion_Sct = (cAttr.eSurface&0x00100000) >> 20;
	
	aInfo.eTunnel_Res	= (cAttr.eSurface&0x000c0000) >> 18;
	aInfo.eTunnel_End	= (cAttr.eSurface&0x00020000) >> 17;
	aInfo.eTunnel_Sct	= (cAttr.eSurface&0x00010000) >> 16;

	aInfo.eComment_Res	= (cAttr.eSurface&0x0000f000) >> 12;
	aInfo.eComment_End	= (cAttr.eSurface&0x00000800) >> 11;
	
	aInfo.eObject_Res	= (cAttr.eSurface&0x00000400) >> 10;
	aInfo.eObject_End	= (cAttr.eSurface&0x00000200) >> 9;
	aInfo.eObject_Sct	= (cAttr.eSurface&0x00000100) >> 8;

	aInfo.eHeight_Res	= (cAttr.eSurface&0x000000c0) >> 6;
	aInfo.eHeight_MEnd  = (cAttr.eSurface&0x00000020) >> 5;
	aInfo.eHeight_MFlag = (cAttr.eSurface&0x00000010) >> 4;
	aInfo.eHeight_GType = (cAttr.eSurface&0x00000008) >> 3;
	aInfo.eHeight_GFlag = (cAttr.eSurface&0x00000004) >> 2;
	aInfo.eHeight_DEnd	= (cAttr.eSurface&0x00000002) >> 1;
	aInfo.eHeight_DSct	= (cAttr.eSurface&0x00000001);

	return aInfo;
}

//! ëÆê´ílÇê›íËÇ∑ÇÈ
void MPQVer3::SetInfo(MPQ::Attr* cAttr, const Info& cInfo)
{
	int aVal = (cInfo.eRes_ << 22) | (cInfo.eTunnel_Res << 18) | (cInfo.eComment_Res << 12) | (cInfo.eObject_Res << 10) | (cInfo.eHeight_Res << 6);
	
	if(0 != cInfo.eDeletion_End) aVal |= 0x00200000;
	if(0 != cInfo.eDeletion_Sct) aVal |= 0x00100000;

	if(0 != cInfo.eTunnel_End)	 aVal |= 0x00020000;
	if(0 != cInfo.eTunnel_Sct)   aVal |= 0x00010000;

	if(0 != cInfo.eComment_End)  aVal |= 0x00000800;

	if(0 != cInfo.eObject_End)   aVal |= 0x00000200;
	if(0 != cInfo.eObject_Sct)   aVal |= 0x00000100;

	if(0 != cInfo.eHeight_MEnd)  aVal |= 0x00000020;
	if(0 != cInfo.eHeight_MFlag) aVal |= 0x00000010;
	if(0 != cInfo.eHeight_GType) aVal |= 0x00000008;
	if(0 != cInfo.eHeight_GFlag) aVal |= 0x00000004;
	if(0 != cInfo.eHeight_DEnd)  aVal |= 0x00000002;
	if(0 != cInfo.eHeight_DSct)  aVal |= 0x00000001;

	cAttr->eSurface = aVal;
}








