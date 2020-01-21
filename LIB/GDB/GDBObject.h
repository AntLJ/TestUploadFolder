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
 *	GDBObject.h
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		êVãKçÏê¨
 */

#ifndef	___GDB_OBJECT_H___
#define	___GDB_OBJECT_H___

#include <string>

extern HRESULT GDBGetValue(_IRowPtr ipRow, const std::string &cColumn, VARIANT *vaVal);
extern HRESULT GDBGetValue(IFeaturePtr ipFeature, const std::string &cColumn, VARIANT *vaVal);

extern HRESULT GDBPutValue(_IRowPtr ipRow, const std::string &cColumn, VARIANT vaVal);
extern HRESULT GDBPutValue(IFeaturePtr ipFeature, const std::string &cColumn, VARIANT vaVal);

extern HRESULT GDBGetTableName(_IRowPtr ipRow, std::string *cName);
extern HRESULT GDBGetTableName(IFeaturePtr ipFeature, std::string *cName);

#endif	//___GDB_OBJECT_H___
