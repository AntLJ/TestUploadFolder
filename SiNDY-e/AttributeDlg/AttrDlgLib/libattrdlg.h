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

#ifndef _LIBATTRDLG_H_
#define _LIBATTRDLG_H_

#pragma once

#if _MSC_VER == 1700
  #ifdef _DEBUG
    #pragma comment(lib,"libattrdlg_vc11-ugd.lib")
  #else
    #pragma comment(lib,"libattrdlg_vc11-u.lib")
  #endif // ifdef _DEBUG
#else
  #ifdef _DEBUG
    #pragma comment(lib,"libattrdlg-ugd.lib")
  #else
    #pragma comment(lib,"libattrdlg-u.lib")
  #endif // ifdef _DEBUG
#endif // if _MSC_VER == 1700

#endif // ifndef _LIBATTRDLG_H_
