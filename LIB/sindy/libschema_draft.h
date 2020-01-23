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

#ifndef SINDY_LIBSCHEMA_DRAFT_H_
#define SINDY_LIBSCHEMA_DRAFT_H_

#if _MSC_VER == 1700
#ifdef _UNICODE
#ifdef _DEBUG
#pragma comment(lib,"libsindy_schema_draft_vc11-ugd.lib")
#else
#pragma comment(lib,"libsindy_schema_draft_vc11-u.lib")
#endif // ifdef _DEBUG
#else
#ifdef _DEBUG
#pragma comment(lib,"libsindy_schema_draft_vc11-gd.lib")
#else
#pragma comment(lib,"libsindy_schema_draft_vc11.lib")
#endif // ifdef _DEBUG
#endif  // ifdef _UNICODE
#elif _MSC_VER == 1500
#ifdef _UNICODE
#ifdef _DEBUG
#pragma comment(lib,"libsindy_schema_draft_vc90-ugd.lib")
#else
#pragma comment(lib,"libsindy_schema_draft_vc90-u.lib")
#endif // ifdef _DEBUG
#else
#ifdef _DEBUG
#pragma comment(lib,"libsindy_schema_draft_vc90-gd.lib")
#else
#pragma comment(lib,"libsindy_schema_draft_vc90.lib")
#endif // ifdef _DEBUG
#endif  // ifdef _UNICODE
#else
#ifdef _UNICODE
#ifdef _DEBUG
#pragma comment(lib,"libsindy_schema_draft-ugd.lib")
#else
#pragma comment(lib,"libsindy_schema_draft-u.lib")
#endif // ifdef _DEBUG
#else
#ifdef _DEBUG
#pragma comment(lib,"libsindy_schema_draft-gd.lib")
#else
#pragma comment(lib,"libsindy_schema_draft.lib")
#endif // ifdef _DEBUG
#endif  // ifdef _UNICODE
#endif  // ifdef _MSC_VER

#include <schema_draft.h>

#endif // ifndef SINDY_LIBSCHEMA_DRAFT_H_
