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

#include "MasterDBCache.h"
#include "MasterListCache.h"
#include "Arguments.h"
#include "POIPointRecord.h"
#include "POISubPointRecord.h"
#include "Output.h"

typedef std::pair<CString, CString> CODEINFO;

class CApp
{
public:
	CApp(void) :
		m_lContentsCode_FID(-1), m_lSeq_FID(-1), m_lName_FID(-1), m_lTel_FID(-1), m_lDelete_FID(-1), m_lCityCode_FID(-1), m_lAddrCode_FID(-1), m_lAddrCode2_FID(-1),
		m_funcSeq_FID(-1), m_pstAttCode_FID(-1), m_priority_FID(-1), m_priorityAttCode_FID(-1), m_tollRoad_FID(-1), m_poiPointId_FID(-1), m_idnCode_FID(-1)
	{}
	~CApp(void){}

	/**
	 * @brief	使用法
	 */
	static void Usage();

	/**
	 * @brief	初期化
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool Init( int argc, _TCHAR* argv[] );

	/**
	 * @brief	実行
	 */
	void Run();

private:

	/**
	 * @brief	POI_POINTレコード格納
	 * @param	ipFeature	[in]	POIフィーチャ
	 * @param	pMaster		[in]	マスター情報
	 * @param	pUsable		[in]	利用情報
	 */
	void SetPOI( IFeaturePtr ipFeature, const POIMASTERDBINFO* pMaster, const POIUSABLEINFO* pUsable );

	/**
	 * @brief	POI_SUB_POINTレコード格納
	 * @param	poiPointId	[in]	POI_POINT_ID
	 */
	void SetPOISub( const long poiPointId );

	/**
	 * @brief	行政界情報取得
	 * @param	ipGeo		[in]		POI位置
	 * @param	rCodeInfo	[in, out]	住所コード情報
	 * @retval	true  : 成功
	 * @retval	false : 失敗
	 */
	bool GetCityAdminInfo( IGeometryPtr ipGeo, CODEINFO& rCodeInfo );

	/**
	 * @brief	家形情報取得
	 * @param	ipGeom	[in]	POI位置
	 * @param	buildingID [in,out]	家形ID
	 * @retval	true  : 取得成功
	 * @retval	false : 取得失敗
	 */
	bool GetBuildingInfo( const IGeometryPtr& ipGeom, long& buildingID );

	/**
	 * @brief  POI_SUB_POINTからデータを選別
	 * @note   DELETE_C=0(生存)のフィーチャをメンバ変数 m_mapPoiSub に格納
	 * @retval true  : 取得成功
	 * @retval false : 取得失敗
	 */
	bool CollectPoiSubPointInfo();

private:
	CArguments	m_cArgs;	//!< 実行時オプション

	IFeatureClassPtr		m_ipPOIPointFC;		//!< POI_POINT
	long	m_lContentsCode_FID, m_lSeq_FID, m_lName_FID, m_lTel_FID, m_lAcc_FID, m_lDelete_FID; //!< POI_POINTフィールドインデックス(コンテンツマスタID, シーケンス、名称、電話番号、精度コード、削除コード)

	IFeatureClassPtr m_ipPOISubPointFC; //!< POI_SUB_POINT
	long m_funcSeq_FID, m_pstAttCode_FID, m_priority_FID, m_priorityAttCode_FID, m_tollRoad_FID, m_poiPointId_FID, m_idnCode_FID; //!< POI_SUB_POINTフィールドインデックス

	IFeatureClassPtr		m_ipCityAdminFC;	//!< CITY_ADMIN
	long	m_lCityCode_FID, m_lAddrCode_FID, m_lAddrCode2_FID;	//!< CITY_ADMINフィールドインデックス(市区町村コード, 住所コード、住所コード2)
	IFeatureClassPtr		m_ipBuildingFC;		///!< BUILDING

	CMasterDBCache			m_cDBCache;		//!< マスター情報
	CMasterListCache		m_cListCache;	//!< 種別表リスト情報

	std::multiset<POIPointRecord> m_setPOI; //!< POIレコード情報(位置品質向上・ピンポイント率)
	std::multiset<POIPointRecord> m_setImpPOI; //!< POIレコード情報(重要施設)
	std::multiset<POISubPointRecord> m_setPOISub; //!< POI_SUB_POINTレコード情報

	std::map<long, std::list<IFeaturePtr>> m_mapPoiSub; //!< IDNCODEで絞りこんだPOI_SUB_POINT格納用 key:POI_POINT_ID, value:Feature

	CString m_idnCode; //!< オプションで指定されたIDNCODE(カンマがあればそのまま)
	std::map<long, long> m_idnCodeCounter; //!< キー:IDNCODE(文字列) 値:レコードとして出力された数

	Output m_outPOI; //!< POI_POINT出力
	Output m_outPOISub; //!< POI_SUB_POINT出力

};
