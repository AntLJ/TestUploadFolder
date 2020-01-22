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
#include "LogSys.h"
#include "SurveyPointManager.h"

// 要素 名称の長さ&objectid&名称(名称の長さが短い順に格納・名称の長さが同じの場合はobjectid順)
/**
 * @brief POI情報レコード
 */
struct POIINFO
{
	long e_nameLength;		//!< 名称の長さ
	long e_objectid;		//!< objectid
	CString e_name;			//!< 名称
	CString e_srcID;		//!< 出典資料ID
	CAdapt<IGeometryPtr> e_ipGeometry; //!< 形状

	bool operator == (const POIINFO& poiInfo) const {
		return (e_nameLength == poiInfo.e_nameLength && e_objectid == poiInfo.e_objectid && e_name == poiInfo.e_name);
	}

	bool operator < (const POIINFO& poiInfo) const {
		if (e_nameLength != poiInfo.e_nameLength) return (e_nameLength < poiInfo.e_nameLength);
		return (e_objectid < poiInfo.e_objectid);
	}
};

/**
 * @brief マッチング情報レコード
 */
struct MATCHINGINFO
{
	long	e_perfectMatchCount;			//!< 完全一致個数
	long	e_replacePerfectMatchCount;		//!< 加工後完全一致個数
	double	e_maxMatchPer;					//!< 最大：マッチング率
	long	e_srcType;						//!< 出典コード
	CString	e_srcName;						//!< 出典元
	CString	e_srcID;						//!< 出典資料ID
	CString	e_anyplaceID;					//!< AnyPlaceID
	std::map<long, double> e_allMatchPer;	//!< 別素材とのマッチング率リスト（キー：比較先素材のobjectid 要素：マッチング率）
	CString	e_name;							//!< 名称

	MATCHINGINFO() : e_perfectMatchCount(0), e_replacePerfectMatchCount(0), e_maxMatchPer(0.0), e_srcType(0) {}
};

/**
 * @brief 優先度決定キー（優先利用種別用）
 */
struct SrcPriorKey
{
	long	e_srcType;	//!< 出典コード
	long	e_adptCode;	//!< 採用コード
	long	e_nameLen;	//!< 名称長さ
	long	e_objectID;	//!< オブジェクトID

	SrcPriorKey(const std::vector<long>& srcPriListPrior) :
		m_srcPriListPrior(srcPriListPrior) {}
	SrcPriorKey(const std::vector<long>& srcPriListPrior, long strType, long adptCode, long nameLen, long objectID) :
		e_srcType(strType), e_adptCode(adptCode), e_nameLen(nameLen), e_objectID(objectID), m_srcPriListPrior(srcPriListPrior) {}
	~SrcPriorKey() {}

	bool operator < (const SrcPriorKey& rec) const
	{
		// まず、それぞれの優先順位を取得し比較
		int idx1 = std::distance(m_srcPriListPrior.begin(), std::find(m_srcPriListPrior.begin(), m_srcPriListPrior.end(), e_srcType));
		int idx2 = std::distance(m_srcPriListPrior.begin(), std::find(m_srcPriListPrior.begin(), m_srcPriListPrior.end(), rec.e_srcType));
		if (idx1 != idx2) return (idx1 < idx2);
		// 独自調査の場合は採用コードを比較
		if (sindy::schema::sj::buildingname_src_point::source_type::kOwnSurvey == e_srcType && e_adptCode != rec.e_adptCode) return (e_adptCode < rec.e_adptCode);
		if (e_nameLen != rec.e_nameLen) return (e_nameLen < rec.e_nameLen);
		return (e_objectID < rec.e_objectID);
	}

private:
	const std::vector<long>& m_srcPriListPrior;	//!< 素材優先度種別リスト（優先利用）
};

/**
 * @brief 優先度決定マップ（優先利用種別用）
 */
typedef std::map<SrcPriorKey, CAdapt<IFeaturePtr>> PriorFeatureMap;

/**
 * @brief マッチング処理クラス
 */
class MatchingProcessor
{
public:
	MatchingProcessor(
		const std::vector<long>& srcPriListPrior,
		const std::vector<long>& srcPriListMatch,
		const std::vector<CString>& poiPriList,
		const std::vector<CString>& supPoiPriList,
		const std::vector<StrPair>& replaceList,
		const std::map<long, long>& condList,
		const std::map<CString, long>& bldSrcIndeces,
		const std::map<CString, long>& poiPtOrgIndeces,
		const std::map<CString, long>& mapAddrCodeCounter,
		bool isSurvey,
		SurveyPointManager& surveyPtMgr
		);

	~MatchingProcessor(void);

	/**
	 * @brief マッチング処理関数
	 * @param tgtSrcPtFeature		[in,out] マッチング対象素材ポイント群
	 * @param tgtPoiPtFeature		[in]  マッチング対象注記POIポイント群
	 * @param bldID					[in]  対象建物ID
	 * @param ipFilledBldPolygon	[in]  穴埋め済建物形状
	 * @param bldNamePtInfo			[out] 建物名称ポイントレコード登録情報
	 * @param relIDs				[out] rel_buildingnameに登録する代表点ID群
	 * @param result				[out] マッチング結果 true:成功(代表点決定) false:失敗(代表点となるポイント無し）
	 * @param excepted				[out] 「除外建物」による除外扱いかどうか
	 * @param selected				[out] 調査対象ポイントが設定済みかどうか
	 */
	bool MatchingProc(
		IDFeatureMap& tgtSrcPtFeatures,
		const IDFeatureMap& tgtPoiPtFeatures,
		long bldID,
		IPolygonPtr ipFilledBldPolygon,
		BldNamePtInfo *bldNamePtInfo,
		std::set<long> *relIDs,
		bool *result,
		bool *excepted,
		bool *selected
		);

private:
	/**
	 * @brief  特殊素材ポイント存在確認関数
	 * @note   「除外建物」「独自調査」の素材ポイントのチェック
	 * @param  tgtSrcPtFeature	[in]  マッチング対象素材ポイント群
	 * @param  bldID			[in]  対象建物ID
	 * @param  bldNamePtInfo	[out] 建物名称ポイントレコード登録情報
	 * @param  result			[out] マッチング結果
	 * @param  excepted			[out] 「除外建物」による除外扱いかどうか
	 * @param  selected			[out] 調査対象ポイントが設定済みかどうか（調査対象ポイント生成抑制のために使用）
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool CheckSpecialSrcPt(const IDFeatureMap& tgtSrcPtFeatures, long bldID, BldNamePtInfo *bldNamePtInfo, bool *result, bool *excepted, bool *selected);

	/**
	 * @brief  独自調査ポイントの削除
	 * @note   「独自調査」の素材ポイントをマッチング対象素材ポイント群から削除
	 * @param  tgtSrcPtFeature	[in,out]  マッチング対象素材ポイント群
	 */
	void DeleteOwnSurveyPts(IDFeatureMap& tgtSrcPtFeatures);

	/**
	 * @brief  注記POIポイント登録情報取得関数
	 * @note   注記POIポイントが存在しない場合はpoiSrcID=-1となる。
	 *         複数存在する場合は、注記POI優先度種別リスト(m_poiPriList)の中で優先度が高い種別を保持しているものとする。
	 *         ⇒同じ種別のポイントが複数存在した場合は、文字列の短い方を優先
	 *         ⇒文字列も同じの場合は、objectid順
	 * @param  tgtSrcPtFeature	[in]  マッチング対象素材ポイント群
	 * @param  bldID			[in]  対象建物ID
	 * @param  bldNamePtInfo	[out] 建物名称ポイントレコード登録情報 
	 * @param  existPOI			[out] 注記POIポイントの存在有無 true:存在する false:存在しない
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool GetPoiSrcPtID(const IDFeatureMap& tgtSrcPtFeatures, long bldID, BldNamePtInfo* bldNamePtInfo, bool* existPOI);
	
	/**
	 * @brief  単独ポイントマッチング処理
	 * @note   建物上に素材ポイントが一つしか無い場合のマッチング処理
	 * @param  tgtSrcID				[in]  素材ポイントID
	 * @param  ipTgtSrcFeature		[in]  素材ポイントフィーチャ
	 * @param  bldID				[in]  対象建物ID
	 * @param  ipFilledBldPolygon	[in]  穴埋め済建物形状
	 * @param  isPart				[in]  建物上に同種別の素材が存在し、その中の一つに対するマッチング処理か true: 建物上に同種別の素材が存在する。 flase:建物上には本処理で行っている素材しか存在しない。
	 * @param  bldNamePtInfo		[out] 建物名称ポイントレコード登録情報
	 * @param  result				[out] マッチング結果 true:成功(代表点決定) false:失敗(代表点となるポイント無し）
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool MatchingOnePoint(
		long tgtSrcID,
		const IFeaturePtr& ipTgtSrcFeature,
		long bldID, 
		IPolygonPtr ipFilledBldPolygon, 
		bool isPart,
		BldNamePtInfo* bldNamePtInfo, 
		bool* result
		);

	/**
	 *	@brief 複数ポイントマッチング処理
	 *	@note  文字列を使用したマッチング
	 *	@param tgtSrcPtFeatures [in] マッチング対象素材ポイント群
	 *	@param bldNamePtInfo [in] 素材ポイントID
	 *	@param relIDs [in] 対象建物ID
	 *	@param result [out] マッチング結果 true:成功(代表点決定) false:失敗(代表点となるポイント無し)
	 *	@return true  :成功
	 *	@return false :失敗
	 */
	bool MatchingPoints(
		const IDFeatureMap& tgtSrcPtFeatures, 
		long bldID,
		BldNamePtInfo *bldNamePtInfo,
		bool *result
		);

	/**
	 *	@brief 同一素材ポイントマッチング処理
	 *	@param sameSrcBldNamePtInfo [in] 同一素材ビル名称情報
	 *	@return 代表点ID
	 */
	long SelectSameSrcBldNamePtInfo(const std::map<long, BldNamePtInfo>& sameSrcBldNamePtInfo);

	/**
	 *	@brief 単独防火対象物採用チェック
	 *  @param ipTgtSrcFeature [in] 素材ポイントフィーチャ
	 *  @param result [out] 採用結果 true:採用 false:不採用
	 *  @return true　:成功
	 *  @return false :失敗
	 */
	bool AdoptFirePrevChk(const IFeaturePtr& ipTgtSrcFeature, bool* result);

	/**
	 *  @brief 不動産情報採用チェック
	 *  @param tgtSrcID [in] 素材ポイントID
	 *	@param ipTgtSrcFeature [in] 素材ポイントフィーチャ
	 *	@param ipInfillBldGeometry [in] 穴埋め済建物形状
	 *  @param bldID [in] 対象建物ID
	 *	@param result [out] 採用結果 true:採用 false:不採用
	 *  @return true  :成功
	 *  @return false :失敗
	 */
	bool AdoptImmovablesChk(long tgtSrcID, const IFeaturePtr& ipTgtSrcFeature, IPolygonPtr ipFilledBldPolygon, long bldID, bool *result);

	/**
	 *	@biref 各素材とのマッチング関数
	 *  @param tgtSrcPtFeatures [in] 素材ポイントフィーチャ群
	 *	@param resultMatch [out] マッチング結果
	 *	@param resultMatchSortPerfectMatch [out] 完全一致個数でソートされたマッチング結果
	 *  @param resultMatchSortReplacePerfectMatch [out] 加工後完全一致個数でソートされたマッチング結果
	 *  @param resultMatchSortMaxMatchPer [out] 最大マッチング率でソートされたマッチング結果
	 */
	bool GetEachMatchingInfo(
		const IDFeatureMap& tgtSrcPtFeatures, 
		std::map<long, MATCHINGINFO>* resultMatch, 
		std::map<long, std::map<long, MATCHINGINFO*>>* resultMatchSortPerfectMatch,
		std::map<long, std::map<long, MATCHINGINFO*>>* resultMatchSortReplacePerfectMatch,
		std::map<double, std::map<long, MATCHINGINFO*>>* resultMatchSortMaxMatchPer
		);

	/**
	 *	@brief 文字列置換関数
	 *	@param orgName [in]
	 *	@return 置換後文字列
	 */
	CString ReplaceStr(const CString& orgName);

	/**
	 *	@brief 文字列マッチング関数
	 *	@param str1 [in] 文字列1
	 *	@param str2 [in] 文字列2
	 *  @return マッチング率
	 */
	double MatchingStr(const CString& str1, const CString& str2);

	/**
	 *	@brief 代表点ID・信頼度取得関数
	 *	@param resultMatchingSortPerfectMatch [in] 完全一致個数でソートされたマッチング結果
	 *	@param resultMatchingSortReplacePerfectMatch [in] 加工後完全一致個数でソートされたマッチング結果
	 *  @param resultMatchingSortMaxMatchPer [in] マッチング率でソートされたマッチング結果
	 *  @param headID [out] 代表点ID
	 *  @param priority [out] 信頼度コード
	 *	@return true 成功
	 *	@return false 失敗
	 */
	bool GetHeadPt(
		const std::map<long, std::map<long, MATCHINGINFO*>>& resultMatchingSortPerfectMatch,
		const std::map<long, std::map<long, MATCHINGINFO*>>& resultMatchingSortReplacePerfectMatch,
		const std::map<double, std::map<long, MATCHINGINFO*>>& resultMatchingSortMaxMatchPer,
		long *headID,
		long *priority
		);
	
	/**
	 *	@brief 完全一致個数 or 加工後完全一致個数を用いた代表点ID・信頼度取得関数
	 *	@param maxPerfectMatchInfo [in] 完全一致個数が最大のマッチング結果
	 *  @param maxReplacePerfectMatchInfo [in] 加工後完全一致工数が最大のマッチング結果
	 *  @param maxPerfectMatchCount 完全一致個数
	 *  @param headID [out] 代表点ID
	 *  @param priority [out] 信頼度コード
	 *  @return true 成功
	 *  @return false 失敗
	 */
	bool GetHeadPtByPerfectMatch(
		const std::map<long, MATCHINGINFO*>& maxPerfectMatchInfo,
		const std::map<long, MATCHINGINFO*>& maxReplacePerfectMatchInfo,
		long maxPerfectMatchCount,
		long *headID,
		long *priority
		);

	/**
	 *	@brief 最大マッチング率を用いた代表点・信頼度取得関数
	 *  @param maxMatchPerInfo [in] マッチング率でソートされたマッチング結果
	 *  @param headID [out] 代表点ID
	 *	@param priority [out] 信頼度コード
	 *  @return true 成功
	 *  @return false 失敗
	 */
	bool GetHeadPtByMatchPer(const std::map<long, MATCHINGINFO*>& maxMatchPerInfo, long *headID, long *priority);

	/**
	 *	@brief 優先度が高いマッチング情報取得関数
	 *  @param matchingInfos [in] マッチング情報群
	 *  @return 優先度が最大のマッチング情報群
	 */
	std::map<long, MATCHINGINFO*> GetMatchInfoBySrcPri(const std::map<long, MATCHINGINFO*>& matchingInfos);

	/**
	 *  @brief 文字列が短いマッチング情報取得関数
	 *  @param matchingInfos [in] マッチング情報群
	 *  @return 文字列が一番短いマッチング情報群
	 */
	std::map<long, MATCHINGINFO*> GetMatchInfoByStrLen(const std::map<long, MATCHINGINFO*>& matchingInfos);

	/**
	 *	@brief rel_buildingname登録IDセット関数
	 *  @note headID(代表点ID)が定まっていることが条件
	 *  @param tgtSrcPtFeature [in] マッチング対象素材ポイント群
	 *	@param headID [in] 代表点ID
	 *	@param relIDs [out] rel_buildingnameに登録する代表点ID群
	 */
	void SetRelIDs(const IDFeatureMap& tgtSrcPtFeatures, long headID, std::set<long>* relIDs);

	/**
	 *  @brief 文字列を一文字づつ分解する関数
	 *  @param str [in] 文字列
	 *	@return 文字列を分解して格納したコンテナ
	 */
	std::set<CString> Str2Pieces(const CString& str);

	/**
	 * @brief  補完用注記POIポイントによる補完の実施
	 * @param  tgtPoiPtFeatures	[in]  マッチング対象注記POIポイント群
	 * @param  bldID			[in]  対象建物ID
	 * @param  bldNamePtInfo	[out] 建物名称ポイントレコード登録情報
	 * @param  result			[out] マッチング結果
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool SupplyWithPoiPtOrg(const IDFeatureMap& tgtPoiPtFeatures, long bldID, BldNamePtInfo *bldNamePtInfo, bool *result);

private:
	const std::vector<long>			m_srcPriListPrior;	//!< 素材優先度種別リスト（優先利用）
	const std::vector<long>			m_srcPriListMatch;	//!< 素材優先度種別リスト（マッチング）
	const std::vector<CString>		m_poiPriList;		//!< 注記POI優先度リスト
	const std::vector<CString>		m_supPoiPriList;	//!< 補完用注記POI優先度リスト
	const std::vector<StrPair>		m_replaceList;		//!< 置換文字列リスト
	const std::map<long, long>		m_condList;			//!< 採用条件記述リスト
	const std::map<CString, long>	m_bldSrcIndeces;	//!< 素材ポイントのフィールドインデックス
	const std::map<CString, long>	m_poiPtOrgIndeces;	//!< 注記POIポイントのフィールドインデックス

	const std::map<CString, long>&	m_mapAddrCodeCounter;	//!< 素材ポイント・20桁住所コードカウントリスト

	const bool	m_isSuvey;	//!< 調査対象ポイントリスト出力モードかどうか

	SurveyPointManager&	m_surveyPtMgr;	//!< 調査対象ポイントリスト管理オブジェクト
};
