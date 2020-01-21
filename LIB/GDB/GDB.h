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
 *	GDB.h
 *	@author	Fusatoshi Abe
 *	@date	2003/08/04		新規作成
 */

#ifndef	___GDB_H___
#define	___GDB_H___

#include <atlbase.h>

#pragma warning(push)
#pragma warning(disable : 4146)

#include <../WinLib/arcobjects_import_highmethod.h>
#pragma warning(pop)

#include "GDBError.h"
#include "GDBWorkspace.h"
#include "GDBQuery.h"
#include "GDBSpatialSearch.h"
#include "GDBObject.h"
#include "GDBDomain.h"
#include "GDBGeometry.h"
#include "GDBQueryDef.h"

#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <boost/lexical_cast.hpp>

typedef std::map<long,_IRowPtr>				GDBOBJLIST;
typedef GDBOBJLIST::const_iterator			GDBOBJITER;
typedef std::pair<long,_IRowPtr>			GDBOBJSET;

typedef std::multimap<long,_IRowPtr>		GDB2OBJLIST;
typedef GDB2OBJLIST::const_iterator			GDB2OBJITER;
typedef std::pair<GDB2OBJITER,GDB2OBJITER>	GDBOBJRANGE;

struct lessVariant : public std::binary_function< const VARIANT&, const VARIANT&, bool >
{
	bool operator ()( const VARIANT& val1, const VARIANT& val2 ) const;
};

typedef std::set<VARIANT, lessVariant>		VARIANTSET;
typedef VARIANTSET::iterator				VARIANTITER;

// ローカルタイムの取得
HRESULT GDBGetLocalTime(std::string *cTime);

#endif	//___GDB_H___
