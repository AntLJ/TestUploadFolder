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
#include "common.h"
#include "MatchingProcessor.h"
#include "SurveyPointManager.h"

class BldNameImporter
{
public:
	BldNameImporter(
		const std::vector<long>& srcPriListPrior,
		const std::vector<long>& srcPriListMatch,
		const std::vector<CString>& poiPriList,
		const std::vector<CString>& supPoiPriList,
		const std::vector<StrPair>& replaceList, 
		const std::set<long>& bldExcList,
		const std::map<long, long>& condList,
		const std::map<CString, long>& bldSrcIndeces,
		const std::map<CString, long>& bldAddInfoIndeces,
		const std::map<CString, long>& bldIndeces,
		const std::map<CString, long>& twnpageIndeces,
		const std::map<CString, long>& poiPtOrgIndeces,
		const std::map<CString, long>& bldNamePtIndeces,
		const std::map<CString, long>& relBldNameIndeces,
		const IFeatureClassPtr& ipBldSrcPt,
		const ITablePtr& ipBldAddInfo,
		const IFeatureClassPtr& ipBld,
		const IFeatureClassPtr& ipCityAdm,
		const IFeatureClassPtr& ipTwnpage,
		const IFeatureClassPtr& ipPoiPtOrg,
		const IFeatureClassPtr& ipBldNamePt,
		const ITablePtr& ipRelBldName,
		bool isUpdate,
		bool isSurvey,
		SurveyPointManager& surveyPtMgr);

	~BldNameImporter(void);

	/**
	 * @brief 初期化関数
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Init();

	/**
	 * @brief インポート関数
	 * @param addrCode [in] 処理対象行政界コード
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Import(const CString& addrCode);

private:
	/**
	 * @brief 行政界フィーチャ取得関数
	 * @note 与えられた行政界(5桁)コードを保持する行政界のマージを取得する。
	 * @param addrCode [in] 行政界コード
	 * @param tgtAdmGeometry [out] 5桁行政界
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetTgtCityAdms(const CString& addrCode, IGeometryPtr& tgtAdmGeometry);

	/**
	 * @brief 建物情報取得関数
	 * @note 与えられた行政界にintersectする建物フィーチャとoverlapする建物IDを取得する。
	 * @param admFeatre [in] 行政界フィーチャ
	 * @param intersectBldFeatures [out] intersectした建物フィーチャ
	 * @param overrapBldIDs [out] overrapBldIDs overlapした建物IDリスト
	 */
	bool GetTgtBldInfos(const IGeometryPtr& ipAdmGeometry, IDFeatureMap* intersectBldFeatures, std::set<long>* overlapBldIDs);

	/**
	 * @brief 建物ビル名称ポイント・建物ビル名称紐づきレコード取得関数
	 * @param bldID [in] 処理基準建物のOBJECTID
	 * @param ipBldFeature [in] 建物形状
	 * @param overlapBldIDs [in] 行政界とoverlapしている建物IDリスト
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetImportRecordInfos(long bldID, const IFeaturePtr& ipBldFeature, const std::set<long>& overlapBldIDs);

	/**
	 * @brief DB更新処理関数
	 * @param flushAll [in] 全件反映モード
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool DBUpdate(bool flushAll);

	/**
	 * @brief 既存建物ビル名称ポイント存在チェック
	 * @note  出力先建物ビル名称ポイントテーブルに、削除対象ポイントが存在しないかチェックする。
	 *        ⇒処理の基準となる建物に紐づき、尚且つ『信頼度コードが1(調査によって追加されたもの)以外の場合
	 * @param bldID					[in] 処理基準建物のOBJECTID
	 * @param eixstEditPoint		[out] 信頼度コードが1のポイントが存在するかどうか true:存在する false:存在しない 
	 */
	bool ChkExistBldNamePt(long bldID, bool* existEditPoint);

	/**
	 * @brief  穴埋め済ポリゴンの取得
	 * @note   対象ポリゴンのExteriorRingが一つであることが条件
	 * @param  ipTgtFeature		[in]  対象ポリゴンフィーチャ
	 * @param  ipFilledPolygon	[out] 穴埋め済ポリゴン
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetFilledPolygon(IFeaturePtr ipTgtFeature, IPolygonPtr& ipFilledPolygon);

	/**
     * @brief 建物上に存在する素材ポイント取得関数
	 * @param ipFilledBldPolygon	[in] 穴埋め済建物形状
	 * @param tgtSrcPtFeatures		[out] 建物上に存在する素材ポイント群
	 * @retval true 成功
	 * @retval false 失敗 
	 */
	bool GetTgtSrcPt(IPolygonPtr ipFilledBldPolygon, IDFeatureMap* tgtSrcPtFeatures);

	/**
     * @brief 建物上に存在する注記POIポイント取得関数
	 * @param ipFilledBldPolygon	[in] 穴埋め済建物形状
	 * @param tgtPoiPtFeatures		[out] 建物上に存在する注記POIポイント群
	 * @retval true 成功
	 * @retval false 失敗 
	 */
	bool GetTgtPoiPt(IPolygonPtr ipFilledBldPolygon, IDFeatureMap* tgtPoiPtFeatures);

	/**
	 *  @brief 穴埋め済建物形状上に存在するフィーチャ取得関数
	 *	@param bldID				[in] 処理基準建物のOBJECTID
	 *	@param ipInfillBldGeometry	[in] 処理基準建物形状(穴埋め済)
	 *	@param ipFeatureClass		[in] 取得対象フィーチャクラス
	 *	@param tableName			[in] テーブル名
	 *	@param tgtFeatures			[out] 建物上に存在するフィーチャ群
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool GetTgtAtInfillGeo(long bldID, const IGeometryPtr& ipInfillBldGeometry, const IFeatureClassPtr& ipFeatureClass, LPCTSTR tableName, IDFeatureMap* tgtFeatures);

	/**
	 * @brief　既存建物ビル名称ポイント削除関数
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool DeleteExistRecord();

	/**
	 * @brief 建物ビル名称ポイント登録関数
	 * @param bldNamePtInfo		[in] レコード登録情報
	 * @param bldNameAddInfo	[in] 付加情報レコード情報
	 * @param ipBldNameCursor	[in] 登録用フィーチャカーソル
	 * @param bldNamePtID		[out] 登録した際のobejctid
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool InsertBldNamePt(const BldNamePtInfo& bldNamePtInfo, const BldNameAddInfo& bldNameAddInfo, IFeatureCursorPtr& ipBldNamePtCursor, long* bldNamePtID);

	/**
	 * @brief 建物ビル名称紐づきレコード登録関数
	 * @param bldNamePtID			[in] 紐づく建物ビル名称ポイントobjectid
	 * @param bldSrcPtID			[in] 紐づく素材ID
	 * @param ipRelBldNameCursor	[in] 登録用フィーチャカーソル
	 * @retval ture  成功
	 * @retval false 失敗
	 */
	bool InsertRelBldName(long bldNamePtID, long bldSrcPtID, _ICursorPtr& ipRelBldNameCursor);

	/**
	 * @brief 建物ビル名称付加情報レコード取得関数
	 * @param bldNamePtInfo		[in] 建物ビル名称ポイント登録情報
	 * @param bldNameAddInfo	[out] 建物ビル名称付加情報レコード情報
	 * @retval ture  成功
	 * @retval false 失敗
	 */
	bool GetAddInfo(const BldNamePtInfo& bldNamePtInfo, BldNameAddInfo& bldNameAddInfo);

private:
	std::set<long> m_bldExcList;	//!< 処理対象外建物種別リスト

	const std::map<CString, long>	m_bldIndeces;			//!< buildingテーブルフィールドインデックス情報
	const std::map<CString, long>	m_bldSrcIndeces;		//!< buildingname_src_pointテーブルフィールドインデックス情報
	const std::map<CString, long>	m_bldAddInfoIndeces;	//!< buildingname_src_addinfoテーブルフィールドインデックス情報
	const std::map<CString, long>	m_twnpageIndeces;		//!< townpageテーブルインデックス情報
	const std::map<CString, long>	m_poiPtOrgIndeces;		//!< poi_point_orgテーブルインデックス情報
	const std::map<CString, long>	m_bldNamePtIndeces;		//!< buildingname_pointテーブルフィールドインデックス情報
	const std::map<CString, long>	m_relBldNameIndeces;	//!< rel_buildingnameテーブルフィールドインデックス情報

	const IFeatureClassPtr	m_ipBldSrcPt;		//!< buildingname_src_pointフィーチャクラス
	const ITablePtr			m_ipBldAddInfo;		//!< buildingname_src_addinfoテーブル
	const IFeatureClassPtr	m_ipBld;			//!< buildingフィーチャクラス
	const IFeatureClassPtr	m_ipCityAdm;		//!< city_adminフィーチャクラス
	const IFeatureClassPtr	m_ipTwnpage;		//!< townpageフィーチャクラス
	const IFeatureClassPtr	m_ipPoiPtOrg;		//!< poi_point_orgフィーチャクラス
	IFeatureClassPtr		m_ipBldNamePt;		//!< buildingname_pointフィーチャクラス
	ITablePtr				m_ipRelBldName;		//!< rel_buildingnameテーブル

	std::set<long>		m_cnvdOverlapBldIDs;	//!< 変換済建物ID格納コンテナ(行政界ポリゴンにoverlapするもののみ対象)

	std::set<long>		m_srcTypeCodedValues;	//!< 出典種別コード値リスト

	std::map<CString, long>	m_mapAddrCodeCounter;	//!< 「防火対象物情報」ポイント・20桁住所コードカウントリスト

	MatchingProcessor*	m_matchingProcessor;	//!< マッチング処理クラス

	const bool	m_isUpdate;		//!< DB更新モード
	const bool	m_isSuvey;		//!< 調査対象ポイントリスト作成モード

	SurveyPointManager&	m_surveyPtMgr;	//!< 調査対象ポイントリスト管理オブジェクト

	std::map<long, BldNamePtInfo>		m_bldNamePtInfos;		//!< 建物ビル名称ポイント登録リスト
	std::map<long, std::set<long>>		m_relBldNameInfos;		//!< 建物ビル名称紐付きレコード登録リスト
	std::set<long>						m_deleteBldNamePtIDs;	//!< 建物ビル名称ポイント削除対象IDリスト
};
