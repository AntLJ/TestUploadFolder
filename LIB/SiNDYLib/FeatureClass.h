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
 * @file FeatureClass.h
 * @brief CFeatureClassクラス定義
 * $Id$
 */
#ifndef _FEATURECLASS_H_
#define _FEATURECLASS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"

namespace sindy {
/**
 * @class CFeatureClass
 * @brief IFeatureClassラッパークラス
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 */
class SINDYLIB_API CFeatureClass : public CTable
{
public:
	explicit CFeatureClass(){}
	virtual ~CFeatureClass(){}

	CFeatureClass( sindyTableType::ECode emTableType, ITable* lp ){ SetObject( emTableType, lp ); }
	CFeatureClass( sindyTableType::ECode emTableType, IFeatureClass* lp ){ SetObject( emTableType, lp ); }
	CFeatureClass( ITable* lp ){ SetObject( lp ); }
	CFeatureClass( IFeatureClass* lp ){	SetObject( lp ); }

	//@{ @name IObjectClassインターフェース
	LPCTSTR GetAliasName( LPTSTR lpszAliasName, int nLen ) const;
	long GetObjectClassID() const;
	//@}
	//@{ @name IFeatureClassインターフェース
	IFeaturePtr CreateFeature();
	long FeatureCount( IQueryFilter* Filter ) const;
	IFieldPtr GetAreaField() const;
	long GetFeatureClassID() const;
	IFeatureDatasetPtr GetFeatureDataset() const;
	esriFeatureType GetFeatureType() const;
	IFieldPtr GetLengthField() const;
	LPCTSTR GetShapeFieldName( LPTSTR lpszShapeFieldName, int nLen ) const;
	esriGeometryType GetShapeType() const;
	IFeaturePtr GetFeature( long OID ) const;
	IFeatureCursorPtr GetFeatures( const VARIANT& fids, bool Recycling ) const;
	//@}
	//@{ @name ヘルパー関数
	/**
	 * @brief フィーチャクラスで使用されている空間参照を取得する
	 * return ISpatialReferencePtr
	 */
	ISpatialReferencePtr GetSpatialReference() const;

	/**
	 * @brief <b>OIDリストで検索する</b>\n
	 * @param fids		[in]	OIDリスト
	 * @param Recycling	[in]	NextFeatureでのIFeatureをリサイクルする場合はtrue
	 * @return IFeatureCursorPtr
	 */
	IFeatureCursorPtr GetFeatures( const std::list<long>& fids, bool Recycling ) const;
	//@}
};

} // sindy

#endif // _FEATURECLASS_H_
