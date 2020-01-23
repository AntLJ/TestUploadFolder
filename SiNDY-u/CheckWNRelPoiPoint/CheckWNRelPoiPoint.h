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

#include "stdafx.h"
#include "Log.h"
#include "RelPoiTbl.h"
#include "PoiFile.h"

class CCheckWNRelPoiPoint
{
public:
	/**
	 * @brief コンストラクタ
	 * @param cParam	[in] パラメータ
	 */
	CCheckWNRelPoiPoint(CParam& cParam);
	~CCheckWNRelPoiPoint();

	/**
	 * @brief	チェック実行
	 * @return			処理成否
	 */
	bool execute();

	/**
	 * @brief	エラーの存在有無
	 * @return			処理成否
	 */
	bool isError() {return m_isError;};

protected:
	/**
	 * @brief	初期処理
	 * @return			処理成否
	 */
	bool init();

	/**
	 * @brief	メッシュコードチェック
	 * @return			処理成否
	 */
	bool checkMeshCode();

	/**
	 * @brief	SHAPEデータのパス作成
	 * @param	strMesh		[in]	メッシュコード
	 * @return	std::string			SHAPEデータへのパス
	 */
	std::string makeShapeDirName(const std::string& strMesh);

	/**
	 * @brief	関係チェック
	 * @param	sShapeDir	[in]	SHAPEデータの基準パス
	 * @param	sMeshcode	[in]	チェック対象のメッシュコード
	 * @param	mapRelTbl	[in]	道路・歩行者ノード-POI関連テーブル
	 * @param	mapPoiData	[in]	POIデータ
	 * @param	bContinue	[out]	処理続行判定
	 * @return						処理成否
	 */	
	bool checkRelation(const std::string& sShapeDir,
					   const std::string& sMeshcode,
					   const MAP_REL_TBL& mapRelTbl,
					   const MAP_POI_DATA& mapPoiData,
					   bool& bContinue);

	/**
	 * @brief	関連テーブルチェック
	 * @return			処理成否
	 */
	bool checkRelTbl();

	/**
	 * @brief	ポイント取得
	 * @param	ipGeometry	[in]	取得対象のGeometry
	 * @param	rWKSPoint	[out]	座標がセットされたWKSPoint
	 * @return						処理成否
	 */	
	bool getPoint(const IGeometryPtr ipGeometry, WKSPoint& rWKSPoint );

	/**
	 * @brief	距離チェック
	 * @param	poiPoint	[in]	POIデータ座標
	 * @param	wnPoint		[in]	道路・歩行者ノード座標
	 * @param	dDistance	[out]	POIデータ座標⇔道路・歩行者ノード座標間距離
	 * @return						処理成否
	 */	
	bool checkDiscance(const WKSPoint& poiPoint, const WKSPoint& wnPoint, double& dDistance);

private:
	bool m_isError;							// エラー存在有無

	CParam m_CParam;						// パラメータ情報
	crd_cnv m_Ccrd_cnv;						// 地理座標変換クラス

	std::string m_sRelTblLayer;				// チェック対象関連テーブルレイヤ名
	MAP_REL_TBL m_mapRelTbl;				// チェック対象関連テーブルコンテナ
	MAP_POI_DATA m_mapPoiData;				// POIデータコンテナ
	MAP_REL_TBL m_mapMwnChk;				// 道路・歩行者データ用コンテナ
	std::vector<std::string> m_vecMeshList;	// メッシュリストコンテナ
};
