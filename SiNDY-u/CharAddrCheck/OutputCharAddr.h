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

#include <oracl.h>

#include "CharAddrConvertFunc.h"

class COutputCharAddr
{
public:
	COutputCharAddr();
	~COutputCharAddr();

public:
	/**
	* @brief	設定初期化
	* @param	iGouPoint[in]				号ポイントクラス
	* @param	ipCityAdmin[in]				都市地図行政界クラス
	* @param	eOraDB[in]					Oracle接続先
	* @param	ipWorkPorposeTable[in]		作業目的テーブル
	* @param	lpcszOutput[in]				出力ディレクトリ
	*/
	void Init( IFeatureClass* ipGouPoint, IFeatureClass* ipCityAdmin, ODatabase& eOraDB, LPCTSTR lpcszOutput, ITable* ipWorkPorposeTable );

	/**
	* @brief	処理実行メソッド
	* @retval	true	:成功
	* @retval	false	:失敗
	*/
	bool run( LPCTSTR lpcszConvertTablePath, LPCTSTR lpcszGouPoint, LPCTSTR lpcszCityAdmin );

private:
	/**
	* @brief	文字付き住所の文字列を分割してリスト化
	* @param	ipCityAdmin[in]				都市地図行政界クラス
	* @param	mapList[in]					文字付き住所のフィーチャリスト
	* @param	lType[in]					処理タイプ（0:号ポイントリスト,1:都市地図行政界リスト）
	* @return	文字付き住所の文字列を分割した単語と行政界コードのリスト
	*/
	std::map<CString,std::map<CString,long>> getDataFromList( IFeatureClass* ipCityAdmin, const std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>& mapList, long lType );

	/**
	* @brief	リストを統合する
	* @param	mapGouCharList[in,out]		号ポイントリスト（こちらに統合される）
	* @param	mapCityAdminChar[in]		都市地図行政界リスト
	*/
	void CombineCharList( std::map<CString,std::map<CString,long>>& mapGouCharList, const std::map<CString,std::map<CString,long>>& mapCityAdminChar );

	/**
	* @brief	文字付き住所リスト出力
	* @note		単に文字付き住所のリストをOIDと文字列を出力するだけ
	* @param	mapItems[in]				文字付き住所リスト
	* @param	lpcszOutputFile[in]			出力ファイル名（拡張子を含まないこと）
	* @param	lpcszTableName[in]			テーブル名
	* @param	ipFeatureClass[in]			テーブル
	*/
	void WriteCharAddrList( const std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>& mapItems, LPCTSTR lpcszOutputFile, LPCTSTR lpcszTableName, IFeatureClass* ipFeatureClass );

	/**
	* @brief	文字付き住所を分割した文字列群を住所毎に出力
	* @param	mapCharAddr[in]				文字付き住所を分割した文字列群リスト
	* @param	lpcszOutputFile[in]			出力ファイル名（拡張子を含まないこと）
	*/
	void WriteCharAddrList2( const std::map<CString,std::map<CString,long>>& mapCharAddr, LPCTSTR lpcszOutputFile );

private:
	CCharAddrConvert m_cCharAddrConvert;		//!< 変換テーブルクラス
	IFeatureClassPtr m_ipGouPointClass;			//!< 号ポイントクラス
	IFeatureClassPtr m_ipCityAdminClass;		//!< 行政界クラス
	CString m_strOutput;						//!< 出力ディレクトリ
	ODatabase m_eOraDB;							//!< 接続DB
	ITablePtr m_ipWorkPorposeTable;				//!< 作業目的テーブル
};
