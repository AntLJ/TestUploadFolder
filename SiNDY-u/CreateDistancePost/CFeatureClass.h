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

#pragma once

#include "RowBase.h"
#include "FieldMap.h"
#include <boost/shared_ptr.hpp>

namespace sindy
{

/**
 * @class CFeatureClass
 * @brief フィーチャクラスクラス
 * @file  CFeatureClass.h
 * @author ビットエイジ　秦
 * $Id$
*/
class CFeatureClass
{
public:
	typedef boost::shared_ptr<CRowBase> CSPRowBase;
	typedef std::map<long, CSPRowBase> Container;
	typedef Container::const_iterator const_iterator;

	/**
	 * @brief コンストラクタ
	 */
	CFeatureClass(){};

	/**
	 * @brief デストラクタ
	 */
	~CFeatureClass()
	{
		m_ipFeatureClass = nullptr;
		m_mapContainer.clear();
	};

	/**
	 * @brief コンストラクタ
	 */
	CFeatureClass(const IFeatureClassPtr& ipFeatureClass)
	{
		m_ipFeatureClass = ipFeatureClass;
	}

	/**
	 * @brief コンストラクタ
	 */
	CFeatureClass( const CFeatureClass& obj )
	{
		m_ipFeatureClass = obj.m_ipFeatureClass;
	}

	/**
	 * @brief コピーコンストラクタ
	 */
	CFeatureClass& operator = (const IFeatureClassPtr& ipFeatureClass)
	{
		m_ipFeatureClass = ipFeatureClass;
		return *this;
	}

	operator IFeatureClassPtr() const { return m_ipFeatureClass; }

	/**
	 * @brief 空間参照取得
	 * @retval ISpatialReferencePtr
	*/
	ISpatialReferencePtr getSpRef();

	/**
	 * @brief 形状フィールド名取得
	 * @retval CString 形状フィールド名
	*/
	CString getShapeFieldName() const;

	/**
	 * @brief OBJECTIDフィールド名取得
	 * @retval CString OBJECTIDフィールド名
	*/
	CString getOIDFieldName() const;

	/**
	 * @brief フィーチャ検索
	 * @param ipFilter [in] 検索条件
	 * @retval Container 検索フィーチャ群
	*/
	Container search(const IQueryFilterPtr& ipFilter);

	/**
	 * @brief フィーチャ検索
	 * @param ipFilter [in] 検索条件
	 * @retval Container 検索フィーチャ群
	*/
	Container searchNoContain(const IQueryFilterPtr& ipFilter);

	/**
	 * @brief フィールド情報作成
	*/
	void CreateFieldMap();

	/**
	 * @brief フィールド情報取得
	 * @retval CFieldMap フィールド情報
	*/
	CSPFieldMap GetFieldMap() const;

	/**
	 * @brief フィーチャ取得
	 * @param nOID [in] OBJECTID
	 * @retval IFeaturePtr 検索フィーチャ
	*/
	CSPRowBase GetFeature(long nOID);

	/**
	 * @brief フィーチャ作成
	 * @retval IFeaturePtr 作成フィーチャ
	*/
	CSPRowBase Create();

	/**
	 * @brief コンテナクリア
	*/
	void Clear();

	/**
	 * @brief フィーチャ検索
	 * @retval Container 検索フィーチャ群
	*/
	const Container& getContainer() { return m_mapContainer; }

private:
	IFeatureClassPtr m_ipFeatureClass;	///< フィーチャクラス
	Container m_mapContainer;			///< 検索フィーチャ群
	CSPFieldMap m_spFieldMap;			///< フィールド情報群
};

}
