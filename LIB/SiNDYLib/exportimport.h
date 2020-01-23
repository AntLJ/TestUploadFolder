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

#ifndef _ANNOLIB_H_
#define _ANNOLIB_H_

#if(defined _LIB || defined _USE_SINDYLIB_STATIC)
#define SINDYLIB_API
#define SINDYLIB_EXPIMP
#else
#ifdef SINDYLIB_EXPORTS
#define SINDYLIB_API __declspec(dllexport)
#define SINDYLIB_EXPIMP
#else
#define SINDYLIB_API __declspec(dllimport)
#define SINDYLIB_EXPIMP extern
#endif // SINDYLIB_EXPORTS
#endif // defined _LIB || defined _USE_SINDYLIB_STATIC

/**
 * @brief <b>�C���X�^���X�n���h��</b>\n
 * DLL�łł́Ag_hInstance�͒�`���Ă���܂����A�X�^�e�B�b�N�����N
 * �łł͎����Œ�`����K�v������܂��B
 */
#ifndef _LIB
extern HINSTANCE g_hInstance;
#else
extern CComModule _Module;
#endif // _LIB

#endif // _ANNOLIB_H_
