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
 * @file RelRoad.h
 * @brief <b>CRelRoadクラス定義ヘッダファイル</b>
 * @author 
 * @vertion $Id$
 */
#ifndef _RELROAD_H__
#define _RELROAD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RoadLink.h"

namespace sindy {
class CErrorObjects;
/**
 * @class CRelRoad
 * @brief 道路リンク管理クラス
 * このクラスでは、道路に関係のあるオブジェクトを保持
 * します
 */
class SINDYLIB_API CRelRoad : public road_link::CRoadLink
{
public:
	CRelRoad()
	{
	}
	virtual ~CRelRoad()
	{
	}
	CRelRoad( const CRelRoad& obj ) : CRoadLink( obj )
	{
	}
	CRelRoad( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::unknown, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() )
		: CRoadLink( lp, emTableType, bCreated, spFields, spNameString )
	{
	}
	CRelRoad& operator=( const CRelRoad& obj )
	{
		CRoadLink::operator==( obj );
		return *this;
	}
	CRelRoad& operator=( _IRow* lp )
	{
		CRoadLink::operator==( lp );
		return *this;
	}
	/**
	 * @brief <b>コンフリクト回避のための既編集チェックを行う</b>\n
	 * 変更があったものに対してのみチェックを行います。\n
	 * キャッシュを作成していないと変更のチェックに引っかからないので注意してください。\n
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckModify( CErrorObjects& cErrObjects ) const;
};

} // sindy

#endif // _RELROAD_H__
