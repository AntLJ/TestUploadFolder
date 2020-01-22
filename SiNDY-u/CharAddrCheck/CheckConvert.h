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

#include "CharAddrConvertFunc.h"

/**
* @brief	実際の号ポイント、行政界が読み変換テーブルで変換できるかチェックするクラス
*/
class CCheckConvert
{
public:
	CCheckConvert();
	~CCheckConvert();

public:
	/**
	* @brief	設定初期化
	* @param	iGouPoint[in]				号ポイントクラス
	* @param	ipCityAdmin[in]				都市地図行政界クラス
	* @param	lpcszConvertTable[in]		変換テーブルのPGDBパス
	* @param	lpcszOutput[in]				出力ディレクトリ
	*/
	void Init( IFeatureClass* ipGouPoint, IFeatureClass* ipCityAdmin, LPCTSTR lpcszConvertTablePath, LPCTSTR lpcszOutput );

	/**
	* @brief	処理実行メソッド
	* @retval	true	:成功
	* @retval	false	:失敗
	*/
	bool run();

	/**
	* @brief	NGリストパス出力
	* @param	strGouPointText[out]		号ポイントNGリスト
	* @param	strCityAdminText[out]		都市地図行政界NGリスト
	*/
	bool get_GouCityAdminNG(CString& strGouPointText, CString& strCityAdminText);

private:
	/**
	* @brief	GetItems()で得られたリストを漢字読み変換リストに対応しているかチェック
	* @param	mapItem[in]					GetItems()で得られたリスト
	* @param	mapConvertedItems[in,out]	この処理で対応したもの（漢字読み変換で得られたリスト）
	*/
	void ConvertItemsFromKanjiYomiTable( const std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>& mapItems, std::map<long,std::pair<CString,long>>& mapConvertedItems );

	/**
	* @brief	GetItems()で得られたリストをオブジェクト読み変換リストに対応しているかチェック
	* @param	mapItem[in,out]				GetItems()で得られたリスト→対応したものは削除される
	* @param	mapConvertedItems[in,out]	この処理で対応したもの（オブジェクトID読み変換で得られたリスト）
	* @param	LayerType[in]				レイヤタイプ[1:都市地図行政界,2:号ポイント]
	*/
	void ConvertItemsFromObjectYomiTable( std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>>& mapItems, std::map<long,std::pair<CString,long>>& mapConvertedItems, GouOrCityAddrLayer LayerType );

	/**
	* @brief	変換結果の出力
	* @param	mapConvertedItems[in]		変換結果のアイテム群
	* @param	lpcszOutputFile[in]			出力ファイル名
	* @param	strNgFilePath[out]			NG出力ファイルパス
	* @retval	true	NG在り
	* @retval	false	NG無し
	*/
	bool WriteConvertResult( const std::map<long,std::pair<CString,long>>& mapConvertedItems, LPCTSTR lpcszOutputFile, CString& strNGFilePath );

private:
	CCharAddrConvert m_cCharAddrConvert;		//!< 変換テーブルクラス
	IFeatureClassPtr m_ipGouPointClass;			//!< 号ポイントクラス
	IFeatureClassPtr m_ipCityAdminClass;		//!< 行政界クラス
	CString m_strConvertTable;					//!< 変換テーブルPGDBパス
	CString m_strOutput;						//!< 出力ディレクトリ

	CString m_strNGGouFilePath;					//!< convertGouPoint_NG_*.txt
	CString m_strNGCityAdminFilePath;			//!< convertCityAdmin_NG_*.txt
	bool m_bGouNG;								//!< NGリストが空で無い場合はTrue
	bool m_bCityAdminNG;						//!< NGリストが空で無い場合はTrue
};
