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

// PixTwip.h:  pixel’l ‚Æ twips’l ‚Ì•ÏŠ·

#ifndef __PIXTWIP_H__
#define __PIXTWIP_H__

static _inline int GetDeviceConstants(BOOL x)
{
	static short lpx = 0;
	static short lpy = 0;
	if (!lpx)
	{
		HDC hdc = GetDC(NULL);
		lpx = GetDeviceCaps(hdc, LOGPIXELSX); 
		lpy = GetDeviceCaps(hdc, LOGPIXELSY);
		ReleaseDC(NULL, hdc);
	}
	return (x)? lpx: lpy;
}       

int _inline XTwipsToPixels(double twips)
{
	return MulDiv(GetDeviceConstants(true), (int)twips, 1440);
}

int _inline YTwipsToPixels(double twips)
{
	return MulDiv(GetDeviceConstants(false), (int)twips, 1440);
}  

long _inline XPixelsToTwips(int pixels)
{
	return MulDiv(pixels, 1440, GetDeviceConstants(true));
}

long _inline YPixelsToTwips(int pixels)
{
 	return MulDiv(pixels, 1440, GetDeviceConstants(false));
}  

#endif // __PIXTWIP_H__
