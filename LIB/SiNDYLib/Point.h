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
 * @file Point.h
 * @brief CPointクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _POINT_H_
#define _POINT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../LIB/WinLib/ComWrapper.h"
#include <utility>
#include "ErrorCode.h"

namespace sindy {
/**
 * @class CPoint
 * @brief <b>ポイントクラス</b>
 * このクラスはIPointのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4275)
class SINDYLIB_API CPoint : public CComWrapper<IPoint>
{
public:
	explicit CPoint(){}
	virtual ~CPoint(){}

	CPoint( const CPoint& obj )
	{
		*this = obj;
	}
	CPoint( IPoint* lp )
	{
		*this = lp;
	}
	CPoint& operator=( const CPoint& obj )
	{
		AtlComPtrAssign((IUnknown**)&p, obj.p);
		return *this;
	}
	CPoint& operator=( IPoint* lp )
	{
		AtlComPtrAssign( (IUnknown**)&p, (IUnknown*)lp );
		return *this;
	}
//	IPoint* operator ->(){ return p; }
	operator IPoint* () const { return p; }
	double GetX() const;
	double GetY() const;
	errorcode::sindyErrCode SetX( const double& X );
	errorcode::sindyErrCode SetY( const double& Y );
	void QueryCoords( WKSPoint& point ) const;
	std::pair<double,double> QueryCoords() const;

	bool Equals( const WKSPoint& point ) const;
	bool Equals( IPoint* lp ) const;

	/**
	 * @brief <b>ポイントを移動する</b>\n
	 * このポイントがポイントフィーチャ以外のジオメトリの一部の場合、このメソッドを呼んでも
	 * 意味がありません。CGeometry::Moveを使用してください。
	 * @param pairMove	[in]	移動差分
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Move( const std::pair<double,double>& pairMove );
};
#pragma warning(pop)

} // sindy

#endif // _POINT_H_
