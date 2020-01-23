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

#include "stdafx.h"
#include "RowClassFactory.h"
#include "RelRoad.h"
#include "RelNode.h"
#include "RelAnnotationParameter.h"
#include "FieldMap.h"
#include "NameString.h"
#include "Feature.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

CSPRow RowClassFactory( const type_info& info, _IRow* ipRow, sindyTableType::ECode emType, const CSPFieldMap& cFields, const CSPTableNameString& cNames )
{
	// TODO: ここは、VCがtypeofをサポートすればもっとスマートになる
	// BOOSTのtypeofを使用すればできないこともないが、やり方がわからないので誰かやって…
	CSPRow cRet; // 戻り値
	if( typeid(CRow&) == info )
		cRet.reset( new CRow( ipRow, emType, false, cFields, cNames ) );
	else if( typeid(CFeature&) == info )
		cRet.reset( new CFeature( ipRow, emType, false, cFields, cNames ) );
	else if( typeid(CRelRoad&) == info )
		cRet.reset( new CRelRoad( ipRow, emType, false, cFields, cNames ) );
	else if( typeid(CRelNode&) == info )
		cRet.reset( new CRelNode( ipRow, emType, false, cFields, cNames ) );
	else if( typeid(CRelAnnotationParameter&) == info )
		cRet.reset( new CRelAnnotationParameter( ipRow, emType, false, cFields, cNames ) );
	else if( typeid(CDispLine&) == info )
		cRet.reset( new CDispLine( ipRow, emType, false, cFields, cNames ) );
	else
		_ASSERTE(false);

	return cRet;
}

} // sindy
