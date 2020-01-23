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
 * @file RelNode.h
 * @brief <b>CRelNodeクラス定義ヘッダファイル</b>
 * @author 東北開発センターシステム開発グループ 古川 貴宏\n
 * @vertion $Id$
 */
#ifndef _RELNODE_H_
#define _RELNODE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RoadNode.h"

namespace sindy {

class SINDYLIB_API CRelNode : public road_node::CRoadNode
{
public:
	CRelNode()
	{
	}
	virtual ~CRelNode()
	{
	}
	CRelNode( const CRelNode& obj ) : CRoadNode( obj )
	{
	}
	CRelNode( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::unknown, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() )
		: CRoadNode( lp, emTableType, bCreated, spFields, spNameString )
	{
	}
	CRelNode& operator=( const CRelNode& obj )
	{
		CRoadNode::operator==( obj );
		return *this;
	}
	CRelNode& operator=( _IRow* lp )
	{
		CRoadNode::operator==( lp );
		return *this;
	}
public:
};

} // sindy

#endif // ifndef _RELNODE_H_
