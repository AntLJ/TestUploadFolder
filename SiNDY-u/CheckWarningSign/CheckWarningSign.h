#pragma once

/*
 *	@brief	警戒標識⇔道路リンクの直線距離の算出処理
 *	@param	warningSignPoint [in] 警戒標識ポイント
 *	@param	nearestPoint [in] 道路リンク最近傍ポイント
 *	@param	distance [out]直線距離格納用変数
 *	@retval	true 成功
 *	@retval	false 失敗
 */
bool directDistance(const IPointPtr& warningSignPoint, const IPointPtr& nearestPoint, double& distance);

/*
 *	@brief	コード値ドメイン検索処理
 *	@param	mapDomain [in] コード値ドメインリスト
 *	@param	ClassCode [in] コード値
 *	@param	domainName [out] ドメイン名
 *	@retval	true 成功
 *	@retval	false 失敗
 */
bool getDomainName(const std::map<long, CString>& mapDomain, const long& ClassCode, CString& domainName);

/*
 *	@brief	最近傍道路リンク出力処理
 *	@param	transParam [in] 各種パラメータ
 *	@param	ipRoadFC [in] 道路フィーチャクラス
 *	@param	ipRoadSPRef [in] 空間参照情報
 *	@param	roadClassIndex [in] 道路表示名称フィールド番号
 *	@param	mapGeo [in] 警戒標識データ情報
 *	@param	mapDomain [in] コード値ドメインリスト
 *	@param	ofsOutputLog [out] 実行ログ用ファイルストリーム
 *	@retval	true 成功
 *	@retval	false 失敗
 */
bool checkWarningSign(const TransParam& transParam, const IFeatureClassPtr& ipRoadFC, const ISpatialReferencePtr& ipRoadSPRef, const long& roadClassIndex, const std::map<long, CAdapt<IPointPtr>>& mapGeo, const std::map<long, CString>& mapDomain, std::ofstream	&ofsOutputLog);