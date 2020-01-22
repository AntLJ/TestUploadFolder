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

#include "AddrToXyProcesser.h"
#include <WinLib/ADOBase.h>
#include <TDC/sindylib_base/TableFinder.h>

namespace sindy{
	class CRowContainer;
}

/**
* @brief 住所解析実行用クラス
* @note 旧フローにおける、BNAnalyzer.exe の処理を踏襲する
*       trunk\SiNDY-u\BNAnalyzer
*       怪しげな処理も存在するが、元々の処理に準じるものとする
*/
class AddrAnalyzer
{
public:
	AddrAnalyzer(void){}
	virtual ~AddrAnalyzer(void){}

	/**
	* @brief 初期化
	* @note 住所DBに各ユーザで接続する。失敗した場合は例外を投げる。
	* @param addrSvr [in] 住所サーバ接続文字列
	* @param xyUsr [in] XY付与ユーザ名 
	* @param workUsr [in] リリース作業ユーザ名
	* @param buildingDB [in] 家形DBの接続文字
	*/
	void init( const CString& addrSvr, const CString& xyUsr, const CString& workUsr, const CString& buildingDB );

	/**
	* @brief 住所解析結果をRowに反映する
	* @param row [in] BUILDINGNAME_SRC_POINT の Row
	* @return 住所解析が成功したか否か
	*/
	bool updateRow( sindy::CRowContainer& row );

private:
	/**
	* @brief 20桁コードに対応する住所ポイントの数を取得する
	* @note 前フローの count_point_per_addr.sql 実行に相当
	* @param addrCode [in] 対象の20桁コード
	* @return addrCode に対応する住所ポイント（号ポイントなど）の数
	*/
	long countSameAddr( const CString& addrCode );
	
	/**
	* @brief 同一の家形に乗っている住所ポイントの数を取得する
	* @note 前フローの extract_build_id_with_plural_points.sql 実行に相当
	* @param bldgSrcP [in] 家形を検索するポイント
	* @return bldgSrcP が乗っている家形と同じ家系上にある住所ポイント（号ポイントなど）の数
	*/
	long countBuildingAddrs( IPointPtr bldgSrcP );

	/**
	* @brief カウント用のSQLで得られる結果を取得する
	* @param query [in] SQLのクエリ
	* @return SQLのCOUNT結果
	*/
	long getCountResult( const CString& query );

	CString m_xyUser;              //!< XY付与で使用するユーザ名
	AddrToXyProcesser m_addr2XY;   //!< addr_to_xy実行用
	CADOBase m_adoWorkUser;        //!< 住所サーバ（リリース作業用ユーザ）操作用
	sindy::CTableFinder m_finder;  //!< 家形DBのファインダ
};

