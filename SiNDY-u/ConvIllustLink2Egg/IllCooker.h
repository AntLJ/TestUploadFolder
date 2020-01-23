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
#include "PgDataBaseEx.h"

#include <sindy/schema/illust.h>

/**
 *	@class IllCooker
 *	@brief ilustlinkレコード→inf_illust, lq_illustレコード変換クラス
 */
class IllCooker
{
public:
	IllCooker(void);
	~IllCooker(void);

	/**
	 *	@brief	イラストレコード変換関数
	 *	@param	orgRlDataBase [in] 変換元道路リンクPostgreSQLDBハンドリングクラス
	 *	@param	tgtRows [in] 変換対象イラストリンクレコード
	 *	@param	id2names [in] イラストリンクobjectidとイラスト名対応表
	 *	@param	roadLinkTable [in] road_linkテーブル名
	 *	@param	infIllInfos [out] inf_illustレコード情報
	 *	@param	lqIllInfos [out] lq_illustレコード情報
	 *	@return true 成功
	 *	@return false 失敗
	 */
	bool Cook(std::shared_ptr<PgDataBaseEx> orgRlDataBase,
			  const std::vector<ns::RowPtr>& tgtRows, 
		      const std::map<OID, IllFileNames>& id2names,
			  LPCTSTR roadLinkTable,
			  std::map<OID,InfIllInfo>& infIllInfos,
			  std::map<LqIllKey, LqIllInfo>& lqIllInfos);

private:
	/**
	 *	@brief	イラストリンクレコード情報取得関数
	 *	@param	tgtRows	[in] 変換対象イラストリンクレコード
	 *	@param	tgtIllLinkInfo [out] イラストリンクレコード情報
	 *	@return true 成功
	 *	@return false 失敗
	 */
	bool GetIllLinkInfo(const std::vector<ns::RowPtr>& tgtRows, 
						std::map<OID, IllLinkInfo>& tgtIllLinkInfo);
	/**
	 *	@brief	イラストリンク変換関数
	 *	@param	orgRlDataBase [in] 変換元道路リンクPostgreSQLDBハンドリングクラス
	 *	@param	procIllLinkInfo [in] 変換対象イラストリンク情報
	 *	@param	procNodes [in] 変換対象イラストリンク関連ノードID群
	 *	@param	id2names [in] イラストリンクobjectidとイラスト名対応表
	 *	@param	infIllInfos [out] inf_illustレコード情報
	 *	@param	lqIllInfos [out] lq_illustレコード情報
	 *	@return true 成功
	 *	@return false 失敗
	 */
	bool ConvertIllLink(std::shared_ptr<PgDataBaseEx> orgRlDataBase,
						LPCTSTR roadLinkTable,
						const std::vector<IllLinkInfo>& procIllLinkInfo, 
					    const std::vector<OID>&  procTgtNodes,
						const std::map<OID, IllFileNames>& id2names,
					    std::map<OID, InfIllInfo>& infIllInfos,
					    std::map<LqIllKey, LqIllInfo>& lqIllInfos);
	/**
	 *	@brief	イラストリンク関連道路リンク取得関数
	 *	@param	orgRlDataBase [in] 変換元道路リンクPostgreSQLDBハンドリングクラス
	 *	@param	procTgtNodes [in] 変換対象イラストリンク関連ノードID群
	 *	@param	modeDiff [in] 差分テーブルモードON/OFF (true:ON false:OFF)
	 *	@param	relRoadLinkInfos [out] イラストリンク関連道路リンク情報
	 *	@return true 成功
	 *	@return false 失敗
	 */
	bool GetRelRoadLink(std::shared_ptr<PgDataBaseEx> orgRlDataBase,
						LPCTSTR roadLinkTable,
						const std::vector<OID>& procTgtNodes,
						std::map<RoadLinkKey, RoadLinkInfo>& relRoadLinkInfos);
	/**
	 *	@brief	道路リンク情報取得関数
	 *	@param	rows [in] 対象道路リンクrowsポインタ
	 *	@param	relRoadLinkInfos [out] イラストリンク関連道路リンク情報
	 *	@return true 成功
	 *	@return false 失敗
	 */
	bool GetLinkInfo(const ns::RowsPtr& rows, std::map<RoadLinkKey, RoadLinkInfo>& relRoadLinkInfos);
	/**
	 *	@brief	ノードID列⇒リンクID列変換関数
	 *	@param	illLinkId [in]
	 *	@param	tgtNodeIds [in] 変換元ノードID列
	 *	@param	relRoadLinkInfos [in] 関連道路リンク情報
	 *	@param  lqInfos [out] リンクID列
	 *	@return true 成功
	 *	@return false 失敗
	 */
	bool nq2lq(OID illLinkId,
			   const std::vector<OID>& tgtNodeIds,
			   const std::map<RoadLinkKey, RoadLinkInfo>& relRoadLinkInfos,
			   std::vector<LinkQueueInfo>& lqInfos);
};

