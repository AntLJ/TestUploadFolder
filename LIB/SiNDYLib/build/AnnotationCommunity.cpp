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

/**
 * @file AnnotationCommunity.cpp
 * @brief 注記テーブルフィールド・ドメイン定義
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "exportimport.h"
#include "AnnotationCommunity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
	namespace annotation_code_label {
#ifndef _UNICODE
#define ANNOTATION_CODE(name,id,size,label) const TCHAR name[] = #label;
#else // ifndef _UNICODE
#define ANNOTATION_CODE(name,id,size,label) const TCHAR name[] = L#label;
#endif // ifndef _UNICODE
#include <sindy/schema/annotationcode.h>
#undef ANNOTATION_CODE
	} // annotation_code_label
} // sindy
