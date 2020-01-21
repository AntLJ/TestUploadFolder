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
 *	GDBQueryDef.h
 *	@author	Fusatoshi Abe
 *	@date	2003/10/09		êVãKçÏê¨
 */

#ifndef	___GDB_QUERY_DEF_H___
#define	___GDB_QUERY_DEF_H___

#include <string>

extern HRESULT GDBQueryDef(IWorkspacePtr ipWorkspace, const std::string &cTablesName, const std::string &cWhereClause, const std::string &cSubFields, _ICursor **ipCursor);

#endif	//___GDB_QUERY_DEF_H___
