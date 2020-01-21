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
/////////////////////////////////////////////////////
// メモリリーク検出用ヘッダ
// このコードはVC9のATL用です。
// MFCでは何もしなくても検出が可能です。
// VC8,VC6では下記コードだけでは検出できません
/////////////////////////////////////////////////////

#ifndef _AFXDLL
///////////////////////////////
// メモリリーク検出 malloc 用
///////////////////////////////
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
///////////////////////////////
// メモリリーク検出 new 用
///////////////////////////////
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#pragma message("メモリリークを検出するにはプログラム冒頭で下記コードを追加すること")
#pragma message("_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );")
#endif // ifdef _DEBUG
#endif // ifndef _AFXDLL
