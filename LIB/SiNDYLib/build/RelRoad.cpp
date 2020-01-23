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
 * @file RelRoad.cpp
 * @brief <b>CRelRoadクラス実装ファイル</b>
 * @author 
 * @vertion $Id$
 */
#include "stdafx.h"
#include "RelRoad.h"
#include "ErrorObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

// コンフリクト回避のための既編集チェックを行う
sindyErrCode CRelRoad::CheckModify( CErrorObjects& cErrObjects ) const
{
	CErrorObject cErrObj( *this );
	// ネットワークリンク
	if( Changed() )
		CRoadLink::CheckModify( cErrObjects );

	// 関連オブジェクト
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			if( (*it2)->Changed() )
				(*it2)->CheckModify( cErrObjects );
		}
	}
	return cErrObjects.GetDefaultErrCode();
}

} // sindy
