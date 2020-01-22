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
#include "BaseDataCreator.h"

// 出典コードに対する確認ステータスの対応表
const std::map<CString, int> convertMap = boost::assign::map_list_of
	( _T("1"), 1 )
	( _T("2"), 2 )
	( _T("3"), 3 )
	( _T("4"), 3 )
	( _T("5"), 3 )
	( _T("6"), 3 )
	( _T("7"), 3 )
	( _T("8"), 3 )
	( _T("9"), 0 );

/**
 * @brief   住所ポリゴン・住所ポイントデータ生成クラス（派生クラス）
 */
class AddrDataCreator :	public BaseDataCreator
{

public:
	/**
	 * @brief   コンストラクタ
	 * @param   arg [in]  引数
	 */

	AddrDataCreator(const Arguments& arg);

	/**
	 * @brief   デストラクタ
	 */
	virtual ~AddrDataCreator() {}

	/**
	 * @brief   初期化
	 */
	bool init();

	/**
	 * @brief   データ生成
	 */
	bool create();

private:

	/**
	 * @brief   拡張名称・読み設定
	 * @param   kcode [in]  拡張コード
	 * @param   addrPolygonRec [in/out]  住所ポリゴンコンテナ
	 * @param   isKcode1 [in]  指定拡張コードは拡張コード1かどうか（true:拡張コード1、false：拡張コード2）
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool setExtcodeInfo(
		const CString& kcode,
		EdtAddrPolygonRec& addrPolygonRec,
		bool isKcode1
	);

	/**
	 * @brief   住所ポリゴン情報設定
	 * @param   feature [in]  住所ポリゴンフィーチャ
	 * @param   polyRec [out]  住所ポリゴンコンテナ
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool setEdtAddrPolygonRec(
		const IFeaturePtr& feature,
		EdtAddrPolygonRec& polyRec
	);

	/**
	 * @brief   住所ポイント情報設定
	 * @param   feature [in]  住所ポイントフィーチャ
	 * @param   addrPolygonRec [in]  住所ポリゴンコンテナ
	 * @param   addrPointRec [out]  住所ポイントコンテナ
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool setEdtAddrPointRec(
		const IFeaturePtr& feature,
		const EdtAddrPolygonRec& addrPolygonRec,
		EdtAddrPointRec& addrPointRec
	);

	/**
	 * @brief   拡張コードリスト情報MAP取得
	 * @param   extcodeListPath [out]  拡張コードリストファイルパス
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool getExtcodeList(
		const CString& extcodeListPath
	);

	/**
	 * @brief   住所ポリゴンデータ取得
	 * @param   addrPolygonRecs [out]  住所ポリゴンコンテナ群
	 * @param   citycode [in]  市区町村コード
	 * @param   countInfo [in/out]  件数カウンタ情報
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool getAddrPolygon(
		std::vector<EdtAddrPolygonRec>& addrPolygonRecs,
		const CString& citycode,
		CountInfo& countInfo
	);

	/**
	 * @brief   住所ポイントデータ取得
	 * @param   addrPolygonRecs [in]  住所ポリゴンコンテナ群
	 * @param   addrPointRecs [out]  住所ポイントコンテナ群
	 * @param   countInfo [in/out]  件数カウンタ情報
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool getAddrPoint(
		const std::vector<EdtAddrPolygonRec>& addrPolygonRecs,
		std::vector<EdtAddrPointRec>& addrPointRecs,
		CountInfo& countInfo
	);


	/**
	 * @brief   住所ポリゴンの境界上の住所ポリゴンのOIDを取得
	 * @param   addrPolygonRec [in]  住所ポリゴンコンテナ
	 * @param   touchOIds [out]  境界上の住所ポイントのOID群
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool getBoundaryTouchPoints(
		const EdtAddrPolygonRec& addrPolygonRec,
		std::vector<long>& touchOIds
	);

	/**
	 * @brief   住所ポリゴンデータインポート
	 * @param   addrPolygonRecs [in]  住所ポリゴンコンテナ群
	 * @param   countInfo [in/out]  件数カウンタ情報
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool insertAddrPolygon(
		const std::vector<EdtAddrPolygonRec>& addrPolygonRecs,
		CountInfo& countInfo
	);

	/**
	 * @brief   住所ポイントデータインポート
	 * @param   addrPointRecs [in]  住所ポイントコンテナ群
	 * @param   countInfo [in/out]  件数カウンタ情報
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool insertAddrPoint(
		const std::vector<EdtAddrPointRec>& addrPointRecs,
		CountInfo& countInfo
	);

private:

	/**
	 * @brief   号番号の半角化
	 * @param   str [in]  対象文字列
	 * @return  半角化した号番号
	 */
	CString toHalfGouNo(
		const CString& str
	);

	/**
	 * @brief   号番号設定
	 * @param   addrPointRec [in/out]  住所ポイントコンテナ
	 * @param   addressList [in]  住所文字列リスト
	 * @param   shinraiF [in]  信頼フラグ
	 * @param   hyouki [in]  表記
	 */
	void setGouNo(
		EdtAddrPointRec& addrPointRec,
		const std::vector<CString>& addressList,
		const CString& shinraiF,
		const CString& hyouki
	);

	/**
	 * @brief   確認ステータス設定
	 * @param   addrPointRec [in/out]  住所ポイントコンテナ
	 * @param   sourceCd [in]  出典コード
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool setConfirmC(
		EdtAddrPointRec& addrPointRec,
		const CString& sourceCd
	);

private:

	std::list<uh::tstring> m_citycodeList; //!< 市区町村コードリスト
	std::map<CString, ExtcodeListRec> m_extcodeListMap; //!< 拡張コードリストマップ（key:14桁コード、value:名称/読み
};

