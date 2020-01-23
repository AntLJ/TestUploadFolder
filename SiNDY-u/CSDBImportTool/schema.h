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

#include <vector>

typedef struct _tFieldSchema{
	CString strFieldName;
	CString strAlias;
	esriFieldType emType;
	long lLength;
	long lScale;
	VARIANT_BOOL vbNullable;
} tFieldSchema;

extern tFieldSchema g_CSDBFieldDef[], g_NAVIFieldDef[];
extern tFieldSchema g_FieldTest[];


#define CSDBFIELDS 32
#define NAVIFIELDS 32

#define ID_INDEX 0
