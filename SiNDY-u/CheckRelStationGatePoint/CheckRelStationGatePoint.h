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

#include <WinLib/VersionInfo.h>
#include <TDC\useful_headers\boost_program_options.h>
#include <shapelib/DBFHandle.h>

#include "Properties.h"
#include "StationGateFinder.h"

#define OPT_INPUT_DIR "input_dir,i"
#define OPT_MESH_LIST "mesh, m"
#define OPT_SGP_FILE "sgp_file, s"
#define OPT_JGD2000_PR_FILE "jgd2000_parameter_file, p"
#define OPT_LOG "log, x"

#define OPT_MAX 11

typedef std::multimap<long, long> REL_SGP;

class CCheckRelStationGatePoint
{
public:
	CCheckRelStationGatePoint();
	~CCheckRelStationGatePoint();

private:
	std::string m_shapeDir;
	std::string m_stationGatePoint;
	std::string m_jgd2000_pr;
	std::vector<std::string>	m_meshList;

	std::ofstream m_log;

	std::set<long> m_checkId;

	crd_cnv m_crd_cnv;

	bool m_isError;

public:
	/**
	 *@brief	引き数チェック
	 *@param	argc	[in]	引き数(件数)
	 *@param	argv	[in]	引き数(値)
	 *@return	bool
	 */
	bool arg_check(int argc, char* argv[]);

	/**
	 *@brief	チェック実行
	 *@return	bool
	 */
	bool execute();

	/**
	 *@brief	エラーの存在有無
	 *@return	bool
	 */
	bool isError();

protected:

	/**
	 *@brief	メッシュリストの読み込み
	 *@param	file	[in]	メッシュリストファイルパス
	 *@param	argv	[out]	メッシュコードのリスト
	 *@return	bool
	 */
	bool loadList(std::string file, std::vector<std::string> &VectorList);

	/**
	 *@brief	SHAPEデータのパス作成
	 *@param	strMesh	[in]	メッシュコード
	 *@param	strPath	[in]	SHAPEデータの基準パス
	 *@return	SHAPEデータへのパス
	 */
	std::string makeShapeDirName(std::string strMesh, std::string strPath);

	/**
	 *@brief	道路・歩行者ノード-駅出入口POI関連テーブルの読み込み
	 *@param	shapeDir	[in]	SHAPEデータの基準パス
	 *@param	relSgp	[out]	道路・歩行者ノード-駅出入口POI関連テーブル
	 *@return	bool
	 */
	bool readRelSgp(std::string shapeDir, REL_SGP &relSgp);

	/**
	 *@brief	関係チェック
	 *@param	relSgp	[in]	道路・歩行者ノード-駅出入口POI関連テーブル
	 *@param	shapeDir	[in]	SHAPEデータの基準パス
	 *@param	meshcode	[in]	チェック対象のメッシュコード
	 *@param	sgpFinder	[in]	駅出入口POIデータ検索クラス
	 *@return	bool
	 */	
	bool check_relation(REL_SGP relSgpMap,const std::string shapeDir, const std::string meshcode, StationGateFinder sgpFinder);

	/**
	 *@breif	フィールドインデックスの取得
	 *@param	ipTable	[in]	取得元テーブル
	 *@param	fieldName	[in]	フィールド名
	 *@param	index	[out]	インデックス
	 *@return	bool
	 */
	bool getFieldIndex(const ITablePtr ipTable, CString fieldName, long &index);

	/**
	 *@brief	ポイント取得
	 *@param	ipGeometry	[in]	取得対象のGeometry
	 *@param	rWKSPoint	[out]	座標がセットされたWKSPoint
	 *@return	bool
	 */	
	bool getPoint( const IGeometryPtr ipGeometry, WKSPoint& rWKSPoint );
	
	void printLog(std::string tag, std::string message1, std::string message2);
	void printLog(std::string tag, std::string message1, CString message2);
};
