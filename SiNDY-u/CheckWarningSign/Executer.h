#pragma once

/*
 *	@brief	実行ログ・エラーログ作成処理
 *	@param	TransParam [in] 各種パラメータ
 *	@return true 成功
 *	@return false 失敗
 */
bool initLog(const TransParam &transParam);

/*
 *	@brief	ツール情報出力処理
 *	@param	TransParam [in] 各種パラメータ
 *	@return true 成功
 *	@return false 失敗
 */
void procInfoWrite(TransParam &transParam);

/*
 *	@brief	ツール情報出力処理
 *	@param	TransParam [in] 各種パラメータ
 *	@param	ofsOutputLog [out] 実行ログ用ファイルストリーム
 *	@return true 成功
 *	@return false 失敗
 */
bool openOutputLog(const TransParam& transParam, std::ofstream	&ofsOutputLog);

/*
 *	@brief	コード値ドメインリスト作成処理
 *	@param	ipRoadField [in] 道路フィールド
 *	@param	mapDomain [out] コード値ドメインリスト
 *	@return true 成功
 *	@return false 失敗
 */
bool domainlist(const IFieldPtr& ipRoadField, std::map<long, CString>& mapDomain);

/*
 *	@brief	道路リンクデータの取得
 *	@param	transParam [in] 各種パラメータ
 *	@param	ipRoadWorkspace [in] 道路ワークスペース
 *	@param	ipRoadFC [out] 道路フィーチャクラス
 *	@param	ipRoadSPRef [out] 空間参照情報
 *	@param	mapDomain [out] コード値ドメインリスト
 *	@param	roadClassIndex [out] 道路表示名称フィールド番号
 *	@return true 成功
 *	@return false 失敗
 */
bool getRoadLink(const TransParam& transParam, const IWorkspacePtr& ipRoadWorkspace, IFeatureClassPtr& ipRoadFC, ISpatialReferencePtr& ipRoadSPRef, std::map<long, CString>& mapDomain, long& roadClassIndex);

/*
 *	@brief	警戒標識データの取得
 *	@param	transParam [in] 各種パラメータ
 *	@param	ipWSWorkspace [in] 警戒標識ワークスペース
 *	@param	ipWSFC [out] 警戒標識フィーチャクラス
 *	@param	mapGeo [out] 警戒標識データ情報
 *	@return true 成功
 *	@return false 失敗
 */
bool getWarningSign(const TransParam& transParam, const IWorkspacePtr& ipWSWorkspace, IFeatureClassPtr& ipWSFC, std::map<long, CAdapt<IPointPtr>>& mapGeo);

/*
 *	@brief	全体処理
 *	@param	transParam [in] 各種パラメータ
 *	@return true 成功
 *	@return false 失敗
 */
bool exec(const TransParam &transParam);