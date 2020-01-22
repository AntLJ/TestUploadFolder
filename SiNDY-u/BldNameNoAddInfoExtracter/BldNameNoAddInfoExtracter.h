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
#include "TsvWriter.h"
#include "AddrMgr.h"
#include <TDC/sindylib_base/TableFinder.h>

namespace sindy{
	class CRowContainer;
	class CTableContainer;
}

/**
 * @brief   メインクラス
 */
class BldNameNoAddInfoExtracter
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	BldNameNoAddInfoExtracter( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   デストラクタ
	 */
	virtual ~BldNameNoAddInfoExtracter(){};

	/**
	 * @brief   実行
	 * @note 致命的なエラーがあった場合は例外を投げる
	 */
	void execute();

private:

	/**
	 * @brief   初期化
	 * @note 失敗した場合は例外を投げる
	 */
	void init();
	
	/**
	* @brief 紐付く素材ポイントを取得する
	* @note bldName が補完用POI由来の場合は、ダミーを作って返す
	* @param bldName [in] ビル名称
	* @parem poiT [in] POI_POINT_ORGテーブル
	* @return 素材ポイント
	*/
	sindy::CSPRowContainer getSrcPoint( const sindy::CRowContainer& bldName, sindy::CTableContainer& poiT );
	
	/**
	* @brief POI由来だったら読みを取得する
	* @param bldName [in] ビル名称
	* @parem poiT [in] POI_POINT_ORGテーブル
	* @return POIの読み
	*/
	CString getPoiYomi( const sindy::CRowContainer& bldName, sindy::CTableContainer& poiT );


private:
	Arguments m_args;                    //!< 引数
	sindy::CTableFinder m_bldNameFinder; //!< ビル名称ファインダ
	sindy::CTableFinder m_adminFinder;   //!< 行政界用ファインダ
	sindy::CTableFinder m_poiFinder;     //!< POI_ORG
	TsvWriter m_tsvWriter;               //!< 結果リスト出力用
	AddrMgr m_addrDb;                    //!< 住所DB操作用
	CString m_poiUser;                   //!< POIの接続ユーザ名
};

