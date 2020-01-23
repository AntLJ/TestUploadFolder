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

class CDBManager
{
public:
	CDBManager(void);
	~CDBManager(void);

	/**
	 * @brief フィーチャカーソルの取得
	 * @param  ipFeatureClass		[in]	フィーチャクラスポインタ
	 * @param  bstrWhereClause		[in]	Where句文字列
	 * @retval	IFeatureCursorPtr	フィーチャカーソルポインタ
	 */
	IFeatureCursorPtr GetFeatureCursor(IFeatureClassPtr ipFeatureClass, CComBSTR bstrWhereClause);

	/**
	 *	@brief	指定した形状とesriSpatialRelEnumでした条件に一致するフィーチャカーソルの取得
	 *	@param	ipFeatureClass		[in]	フィーチャクラス
	 *	@param	ipGeom				[in]	比較元形状
	 *	@param	eStType				[in]	形状判定パラメータ
	 *	@param	lpcszSubFields		[in]	サブフィールド設定用文字列
	 *	@retval	IFeatureCursorPtr	フィーチャカーソルポインタ
	 */
	IFeatureCursorPtr GetFeatureCursor(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeom, esriSpatialRelEnum eStType, LPCTSTR lpcszSubFields=NULL);

	/**
	 * @brief クエリフィルタの作成
	 * @param bstrWhereClause	[in]		Where句文字列
	 * @retval	IQueryFilterPtr	クエリフィルタポインタ
	 */
	IQueryFilterPtr CreateQueryFilter(CComBSTR bstrWhereClause);

	/**
	 *	@brief	OBJECTIDの取得
	 *	@param	ipRow	[in]	一レコード
	 *	@retval	ObjectID
	 */
	long GetOID(const _IRowPtr& ipRow);

	/**
	 *	@brief	形状(ジオメトリ)の取得
	 *	@param	ipFeature	[in]	形状を持った一レコードの情報
	 *	@retval	形状
	 */
	IGeometryPtr CDBManager::GetGeom(const IFeaturePtr& ipFeature);

	/**
	 *	@brief	OIDとフィーチャクラスから形状(ジオメトリ)の取得
	 *	@param	lOID	[in]	形状を取得したいObjectid
	 *	@param	ipFC	[in]	形状を取得したいフィーチャクラス
	 *	@retval	形状
	 */
	IGeometryPtr CDBManager::GetGeomFromOID(long lOID, IFeatureClassPtr ipFC);

	/**
	 *	@brief	指定した形状に指定した条件でヒットした形状の特定の情報ををセットに格納して返す
	 *	@param	ipFeatureClass	[in]	フィーチャクラス
	 *	@param	ipGeom			[in]	ジオメトリ
	 *	@param	lFieldIndex		[in]	情報を取得したフィールドのインデックス
	 *	@param	eStType			[in]	esriSpatialRelEnum
	 *	@retval	std::set<long>			[out]	ヒットした形状のOIDを格納したset
	 */
	std::set<CAdapt< CComVariant>> CDBManager::GetHitValSet(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeom, long lFieldIndex, esriSpatialRelEnum eStType);

	/**
	 *	@brief	フィールドインデックスを指定しての値の取得
	 *	@param	ipRow		[in]	取得するオブジェクトの行
	 *	@param	lFieldIndex	[in]	取得した情報のインデックス
	 *	@return 指定されたフィールドのCComVariant値
	 */
	CComVariant		GetValue(_IRowPtr ipRow, long lFieldIndex);
};
