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

#include "Arguments.h"
#include "LogManager.h"
#include "CommonAttribute.h"
#include <TDC/sindylib_core/FieldMap.h>

namespace
{
	// key:11桁住所コード val:郵便番号
	typedef std::map<CString,CString> AddrZipCodeMap;
}

/**
 * @brief   メインクラス
 */
class CCreateZipCodePolygon
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CCreateZipCodePolygon( const Arguments& args ) :
		m_idxCityCode(-1), m_idxAddrCode(-1), m_idxAddrCode2(-1)
	{
		m_args = args;
	}

	/**
	 * @brief   デストラクタ
	 */
	~CCreateZipCodePolygon(){}

	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();

	/**
	* @brief   処理実行(Shp出力)
	* @retval  true  成功
	* @retval  false 失敗
	*/
	bool executeShp();

	/**
	* @brief   処理実行(SDE出力)
	* @retval  true  成功
	* @retval  false 失敗
	*/
	bool executeSde();

private:

	/**
	 * @brief   初期化
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool init();

	/**
	 * @brief 住所DB関係の初期設定
	 * @note 住所マスタに接続して、郵便番号リストを作る
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool JusyoInit();

	/**
	 * @brief 都市地図行政界関係の初期設定
	 * @param ipCityAdminFc [out] 都市地図行政界フィーチャクラス
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool CityAdminInit(IFeatureClassPtr& ipCityAdminFc);

	/**
	 * @brief 出力先Shapeファイルの初期設定
	 * @param ipZipCodePolygonFc [out] 郵便番号ポリゴンデータ出力先フィーチャクラス
	 * @param ipZipCodeTable [out] 郵便番号テーブルデータ出力先テーブル
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool ShapeInit(IFeatureClassPtr& ipZipCodePolygonFc, ITablePtr& ipZipCodeTable);

	/**
	 * @brief 郵便番号取得
	 * @param cityCode [in] 都市地図行政界ポリゴンのCITYCODE
	 * @param addrCode [in] 都市地図行政界ポリゴンのADDRCODE、またはADDRCODE2
	 * @param oid [in] 都市地図行政界ポリゴンのOBJECTID
	 * @param zipCode [out] 郵便番号
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool GetZipCode(const CString& cityCode, const CString& addrCode, long oid, CString& zipCode);

	/**
	 * @brief 11桁住所コードから郵便番号を取得
	 * @param elevenCode [in] 11桁住所コード
	 * @retval 郵便番号
	 */
	CString TransAddrcode2ZipCode(const CString& elevenCode);

	/**
	 * @brief 郵便番号ポリゴンデータ出力
	 * @param zipCodePolygon [in] 郵便番号ポリゴン出力先フィーチャクラス
	 * @param polygonId [in] 郵便番号ポリゴンID
	 * @param ipGeometry [in] 郵便番号ポリゴンジオメトリ
	 * @param zipCode [in] 郵便番号
	 */
	void OutputFeatureClass(
		IFeatureClassPtr& zipCodePolygon,
		long polygonId,
		const IGeometryPtr& ipGeometry,
		const CString& zipCode);

	/**
	 * @note 郵便番号ポリゴンデータ出力(SDE)
	 * @param featureBuffer [in] フィーチャバッファ
	 * @param featureCursor [in] インサート用カーソル
	 * @param fieldMap [in] 郵便番号ポリゴンのフィールド対応表
	 * @param ipGeometry [in] ポリゴンジオメトリ形状
	 * @param zipCodePair [in] 属性としてセットする郵便番号
	 */
	void OutputFeatureClass(
		IFeatureBufferPtr& featureBuffer,
		IFeatureCursorPtr& featureCursor,
		const sindy::CFieldMap& fieldMap,
		const IGeometryPtr& ipGeometry,
		const std::pair<CString, CString>& zipCodePair);

	/**
	 * @brief 郵便番号テーブルデータ出力
	 * @param zipCodeTable [in] 郵便番号テーブル出力先テーブル
	 * @param polygonId [in] 郵便番号ポリゴンID
	 * @param addrCodes [in] 郵便番号ポリゴンに対応する住所コード
	 */
	void OutputTable(
		ITablePtr& zipCodeTable,
		long polygonId,
		const std::vector<CString>& addrCodes);

public:
	LogManager m_logManager; //!< ログ管理クラス

private:
	Arguments m_args; //!< 引数
	CommonAttribute m_commonAttr; //!< 共通の属性値(最終更新日時等)格納クラス
	AddrZipCodeMap m_addrZipCodeMap; //!< 住所コードと郵便番号の対応表
	ISpatialReferencePtr m_cityAdminSpatialReference; //!< 都市地図行政界の空間参照
	long m_idxCityCode, m_idxAddrCode, m_idxAddrCode2; //!< CITY_ADMINのフィールドインデックス
};

