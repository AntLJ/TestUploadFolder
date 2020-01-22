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

#include <WinLib/ADOBase.h>

namespace addr_to_xy{
	enum result {
		Addr,                   //!< 住所文字列
		AddrLevel,				//!< 住所レベル
		PrefCode,				//!< 都道府県コード
		CityCode,				//!< 市区町村コード
		OazaCode,				//!< 大字・通称コード
		ChomeCode,				//!< 字・丁目コード
		ChibanCode,				//!< 地番コード
		GouCode,				//!< 号コード
		PrefName,				//!< 都道府県名
		CityName,				//!< 市区町村名
		OazaName,				//!< 大字・通称名
		ChomeName,				//!< 字・丁目名
		ChibanName,				//!< 地番
		GouName,				//!< 号
		Mesh,					//!< メッシュコード
		X,                      //!< X
		Y,                      //!< Y
		IPCode,					//!< ＩＰコード
		Precision,              //!< 精度
		ChibanF,                //!< 代表点・地番止まりフラグ

		items                   //!< 項目数
};

}

/**
* @brief XY付与（addr_to_xy）実行クラス
* @note oracleのみをサポート
        処理結果の各ゲッターは BNAnalyzer.exe の処理に準ずる
*/
class AddrToXyProcesser
{
public:
	AddrToXyProcesser(void){
		m_result.resize( addr_to_xy::result::items );
	}
	~AddrToXyProcesser(void){}

	/**
	* @brief 初期化
	* @note  住所サーバ接続に失敗した場合は例外を投げる
	* @param svr [in] サーバ名。tsnnames.ora で定義される接続名
	* @param usr [in] addr_to_xyの実行権限を持つユーザ名
	*/
	void init( const CString& svr, const CString& usr );

	/**
	* @brief addr_to_xy を実行する
	* @param addr [in] 住所文字列
	* @return 20桁コードが得られたか否か
	*/
	bool execFunc( const CString& addr );

	/**
	* @brief 20桁コードを取得する
	* @return 20桁コード
	*/
	CString getAddrCode() const;
	
	/**
	* @brief 経緯度を取得する
	* @return 経緯度
	*/
	WKSPoint getLonLat() const;
	
	/**
	* @brief 住所レベルを取得する
	* @return 住所レベル
	*/
	long getAddrLevel() const;
	
	/**
	* @brief ピンポイントフラグを取得する
	* @return ピンポイントフラグ
	*/
	bool isPinpoint() const;

private:
	CADOBase m_ado;                  //!< ADO管理用
	std::vector<CString> m_result;   //!< XY付与結果
};

