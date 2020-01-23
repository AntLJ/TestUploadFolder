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

#ifndef WINLIB_TYPE_CONVERT_H_
#define WINLIB_TYPE_CONVERT_H_

#pragma warning(push)
#pragma warning(disable: 4310)	// 「ｷｬｽﾄによって定数値が切り捨てられました。」の回避（レベル4）
inline bool VB2bool(VARIANT_BOOL vb) { return (vb) ? true : false; }
inline VARIANT_BOOL bool2VB(bool b) { return (b) ? VARIANT_TRUE : VARIANT_FALSE; }
inline BOOL VB2BOOL(VARIANT_BOOL vb) { return (vb) ? TRUE : FALSE; }
inline VARIANT_BOOL BOOL2VB(BOOL b) { return (b) ? VARIANT_TRUE : VARIANT_FALSE; }
inline BOOL bool2BOOL(bool b){ return (b) ? TRUE : FALSE; }
inline bool BOOL2bool(BOOL b){ return (b) ? true : false; }
#pragma warning(pop)

#endif // ifndef WINLIB_TYPE_CONVERT_H_ 
