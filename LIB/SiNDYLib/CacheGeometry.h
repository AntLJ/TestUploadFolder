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
 * @file CacheGeometry.h
 * @brief <b>CCacheGeometryクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _CACHEGEOMETRY_H_
#define _CACHEGEOMETRY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"

namespace sindy {

/**
 * @class CCacheGeometry
 * @brief <b>WKSPointベースの形状クラス</b>\n
 */
class SINDYLIB_API CCacheGeometry
{
public:
	CCacheGeometry();
	virtual ~CCacheGeometry();

	CCacheGeometry( IGeometry* pGeom );
	CCacheGeometry( const CCacheGeometry& lp );
	CCacheGeometry& operator=( IGeometry* pGeom );
	CCacheGeometry& operator=( const CCacheGeometry& lp );

//	CPoint* operator[]( long index );
	const WKSPoint* operator[]( long index ) const;
	long GetCount() const { return m_lCount; }
	friend errorcode::sindyErrCode CopyCacheGeometry( CCacheGeometry& dest, const CCacheGeometry& src );
private:
	void Init();
	errorcode::sindyErrCode CreateCache( IPointCollection* pGeom );
	errorcode::sindyErrCode CreateCache( IGeometry* pGeom );
	errorcode::sindyErrCode CreateCache( IPoint* pGeom );
	void DeleteCache();
private:
	WKSPoint* m_pPoints;
	long m_lCount;
};

} // sindy

#endif // _CACHEGEOMETRY_H_
