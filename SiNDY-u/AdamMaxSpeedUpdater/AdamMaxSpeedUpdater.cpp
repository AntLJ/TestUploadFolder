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

#include "stdafx.h"

#include <atltime.h>
#include <VersionInfo.h>

// boost
#include <boost/assign/list_of.hpp>
// ArcHelperEx
#include <GlobalFunctions.h>
#include <AheLayerFunctions.h>
// useful_headders
#include <TDC/useful_headers/str_util.h>
// sindylib_base
#include <TDC/sindylib_base/TableContainer.h>

#include "Logger.h"
#include "FeatureReader.h"
#include "LinkagedLaneRoad.h"
#include "AdamMaxSpeedUpdater.h"

using namespace sindy::schema;
using namespace category;
using namespace adam_v2;


// 実行
bool CAdamMaxSpeedUpdater::execute()
{
	if (!initLog()) {
		finish(false);
		return false;
	}

	if (!openTable()) {
		finish(false);
		return false;
	}

	if (!run()) {
		finish(false);
		return false;
	}

	finish(true);

	return true;
}

bool CAdamMaxSpeedUpdater::initLog() const
{
	// 処理ログファイルオープン
	if (!RUNLOGGER.Initialize(m_args.m_logDir))
	{
		std::cerr << "処理ログファイルのオープンに失敗しました : " << uh::toStr(m_args.m_logDir.c_str()) << std::endl;
		return false;
	}

	// ツール実行情報出力
	RUNLOGGER.Log(_T("接続先DB(道路):\t%s\n"), m_args.m_roadDB.c_str());
	RUNLOGGER.Log(_T("接続先DB(ADAM):\t%s\n"), m_args.m_adamDB.c_str());
	RUNLOGGER.Log(_T("メッシュリスト:\t%s\n"), m_args.m_meshListStr.c_str());
	RUNLOGGER.Log(_T("ログ出力フォルダパス:\t%s\n"), m_args.m_logDir.c_str());

	// エラーログファイルオープン
	if (!ERRLOGGER.Initialize(m_args.m_logDir))
	{
		RUNLOGGER.Error(_T("エラーログファイルのオープンに失敗しました : ") + m_args.m_logDir + _T('\n'));
		return false;
	}

	RUNLOGGER.Info(_T("実行開始\n"));

	return true;
}

bool CAdamMaxSpeedUpdater::openTable()
{
	using namespace boost::assign;

	// 道路DB接続
	IWorkspacePtr road_ws = sindy::create_workspace(m_args.m_roadDB.c_str());
	if (!road_ws) {
		RUNLOGGER.Error(_T("道路DBへの接続に失敗しました : ") + m_args.m_roadDB + _T('\n'));
		return false;
	}
	// 道路関連テーブルオープン
	if (FAILED(IFeatureWorkspacePtr(road_ws)->OpenTable(CComBSTR(road_link::kTableName), &m_roadLinkFc))) {
		RUNLOGGER.Error(_T("道路リンクのオープンに失敗しました\n"));
		return false;
	}
	if (FAILED(IFeatureWorkspacePtr(road_ws)->OpenTable(CComBSTR(basemesh::kTableName), &m_basemeshTable)))
	{
		m_basemeshTable = AheOpenTableByTableName(road_ws, basemesh::kTableName, TRUE);
		if (!m_basemeshTable) {
			RUNLOGGER.Error(_T("ベースメッシュのオープンに失敗しました\n"));
			return false;
		}
	}

	// ADAMDB接続
	if (!m_workspace.open(m_args.m_adamDB.c_str())) {
		RUNLOGGER.Error(_T("ADAM DBへの接続に失敗しました : ") + m_args.m_adamDB + _T('\n'));
		return false;
	}

	// SDEならば子バージョンを作成して編集を行うモードにする
	if (m_workspace.isSDE())
	{
		const std::vector<CString> edit_tables = list_of(CString(rel_road_link_lane_link::kTableName));
		if (!m_workspace.childCreateModeOn(getChildVersionName(), edit_tables))
		{
			RUNLOGGER.Error(_T("道路リンク－車線リンク関連テーブルはバージョン対応ではありません\n"));
			return false;
		}
	}

	// NOTE: オープンするテーブルがこれ以上増えたら、ラムダにするなどを検討すること
	// ADAM関連テーブルオープン
	m_laneLinkFc = m_workspace.openTable(lane_link::kTableName);
	if (!m_laneLinkFc) {
		RUNLOGGER.Error(_T("車線リンクのオープンに失敗しました\n"));
		return false;
	}
	m_relRoadLinkLaneLinkT = m_workspace.openTable(rel_road_link_lane_link::kTableName);
	if (!m_relRoadLinkLaneLinkT) {
		RUNLOGGER.Error(_T("道路リンク－車線リンク関連テーブルのオープンに失敗しました\n"));
		return false;
	}
	m_laneLinkGroupT = m_workspace.openTable(lane_link_group::kTableName);
	if (!m_laneLinkGroupT) {
		RUNLOGGER.Error(_T("車線リンクグループテーブルのオープンに失敗しました\n"));
		return false;
	}

	// フィールドマップを取得
	m_laneLinkFM.CreateFieldMap(m_laneLinkFc->GetFields());
	m_roadLinkFM.CreateFieldMap(m_roadLinkFc->GetFields());
	m_laneLinkGroupFM.CreateFieldMap(m_laneLinkGroupT->GetFields());

	return true;
}

bool CAdamMaxSpeedUpdater::run()
{
	namespace grp_c = lane_link_group::group_code;

	// メッシュ単位で処理を行う
	// NOTE: これ以上やることが多くなったら、別クラス化を検討すること
	for (long meshcode : m_args.m_meshList)
	{
		// メッシュ跨ぎのリンクで重複して更新が入らないようにするため、メッシュ単位で更新する
		// 現仕様だと重複して更新が入っても問題にならないが、今後の不具合の元なのでこうしておく
		if (!startEdit())
			return false;

		// 1メッシュ分の車線リンク読み込み (最高速度が"可変式" or "○○km/h（可変）" or "標識/表示なし"のフィーチャのみ)
		static const CString whereClause = uh::str_util::format(
			_T("%s IN (%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld)"),
			lane_link::kMaxLegalSpeedC,
			adam_v2_link_code::speed_code::kVariable,
			adam_v2_link_code::speed_code::kNoLabel,
			adam_v2_link_code::speed_code::kVariable20km,
			adam_v2_link_code::speed_code::kVariable30km,
			adam_v2_link_code::speed_code::kVariable40km,
			adam_v2_link_code::speed_code::kVariable50km,
			adam_v2_link_code::speed_code::kVariable60km,
			adam_v2_link_code::speed_code::kVariable70km,
			adam_v2_link_code::speed_code::kVariable80km,
			adam_v2_link_code::speed_code::kVariable90km,
			adam_v2_link_code::speed_code::kVariable100km,
			adam_v2_link_code::speed_code::kVariable110km,
			adam_v2_link_code::speed_code::kVariable120km
			);

		LinkagedLaneRoad linkage(m_laneLinkFc, m_roadLinkFc, m_relRoadLinkLaneLinkT);
		if (!readLinkagedLaneRoad(meshcode, whereClause, 
								  boost::assign::list_of(grp_c::kCross)(grp_c::kToll)(grp_c::kSAPA)(grp_c::kSmartIC),
								  linkage))
		{
			abortEdit();
			return false;
		}

		// 更新処理
		if (!updateMaxSpeed(linkage, meshcode)) {
			abortEdit();
			return false;
		}

		stopEdit();


		// 更新後、車線リンク-道路リンク間の最高速度を比較し、一致していなければログ出力する
		// 目的は目視確認による手作業修正箇所の特定に使用するため

		LinkagedLaneRoad linkageAfterUpdate(m_laneLinkFc, m_roadLinkFc, m_relRoadLinkLaneLinkT);
		if (!readLinkagedLaneRoad(meshcode, CString(),
								  boost::assign::list_of(grp_c::kCross)(grp_c::kToll)(grp_c::kSmartIC),
								  linkageAfterUpdate))
		{
			return false;
		}

		// 比較結果を出力
		compareMaxSpeed(linkageAfterUpdate, meshcode);
	}

	return true;
}

bool CAdamMaxSpeedUpdater::updateMaxSpeed(LinkagedLaneRoad& linkagedLinks, long basemesh_no)
{
	using namespace lane_link::speed_code;

	for (const auto& record : linkagedLinks)
	{
		IFeaturePtr lane_link, road_link;
		LinkageAccuracy accuracy = linkagedLinks.getHigherPriorityPairLink(record, lane_link, road_link);

		// 車線リンクと道路リンクのペアがない場合
		if (accuracy == kNoPair) {
			ERRLOGGER.Warn(lane_link, nullptr, basemesh_no, _T("更新元の道路リンクが確定できなかったため、更新に失敗しました"));
			continue;
		}

		// 更新する"制限速度コード(最大)"の取得に失敗した場合
		long updateCode = getUpdateMaxSpeedCode(road_link);
		if (updateCode == kUnset) {
			ERRLOGGER.Warn(lane_link, road_link, basemesh_no, _T("更新元の道路リンクの最高速度コードが更新対象ではなかったため、更新を行いませんでした "));
			continue;
		}

		// 車線リンクの持つ"制限速度コード(最大)"と、更新する"制限速度コード(最大)"が等しければ、更新スキップ
		if(lane_link->GetValue(m_laneLinkFM.FindField(lane_link::kMaxLegalSpeedC)).lVal == updateCode)
			continue;

		// "制限速度コード(最大)"を更新
		if (FAILED(lane_link->put_Value(m_laneLinkFM.FindField(lane_link::kMaxLegalSpeedC), CComVariant(updateCode)))) {
			ERRLOGGER.Error(lane_link, road_link, basemesh_no, _T("データの更新に失敗しました"));
			return false;
		}
		if (FAILED(lane_link->Store())) {
			ERRLOGGER.Error(lane_link, road_link, basemesh_no, _T("データの更新に失敗しました"));
			return false;
		}

		// "制限速度コード(最大)"を更新の妥当性に応じてメッセージを出力
		if (accuracy == kAccurate)
			ERRLOGGER.Info(lane_link, road_link, basemesh_no, _T("制限速度コード（最大）を更新しました"), updateCode);
		else if(accuracy == kMostlyAccurate)
			ERRLOGGER.Info(lane_link, road_link, basemesh_no, _T("制限速度コード（最大）を更新しました (複数の道路リンクと紐付きあり) "), updateCode);
		else if (accuracy == kInaccurate)
			ERRLOGGER.Warn(lane_link, road_link, basemesh_no, _T("制限速度コード（最大）を更新しましたが不正確な可能性があります"), updateCode);
		else
			_ASSERTE(false);
	}

	return true;
}

void CAdamMaxSpeedUpdater::compareMaxSpeed(LinkagedLaneRoad & linkagedLinks, long basemesh_no)
{
	using namespace lane_link::speed_code;

	long idxLane = m_laneLinkFM.FindField(lane_link::kMaxLegalSpeedC);
	long idxRoad = m_roadLinkFM.FindField(road_link::kLegalSpeed);
	for (const auto& record : linkagedLinks)
	{
		long laneMaxSpeed = record.m_laneLink->GetValue(idxLane).lVal;
		for (const auto& road : record.m_roadLinks)
		{
			long roadMaxSpeed = road->GetValue(idxRoad).lVal;
			if(!isEquallMaxSpeed(laneMaxSpeed, roadMaxSpeed))
				ERRLOGGER.Info(record.m_laneLink, road, basemesh_no, _T("車線リンクと道路リンクの最高速度が一致していません"));
		}
	}
}

bool CAdamMaxSpeedUpdater::readLinkagedLaneRoad(
	long basemesh_no, const CString & lane_where_clause, const std::list<long>& exclude_group_code, 
	LinkagedLaneRoad & linkaged_links
	)
{
	FeatureReader laneLinkReader(m_laneLinkFc, m_basemeshTable);
	if (!laneLinkReader.read(basemesh_no, lane_where_clause)) {
		RUNLOGGER.Error(_T("車線リンクの読み込みに失敗しました : ") + basemesh_no + _T('\n'));
		return false;
	}

	auto laneLinks = laneLinkReader.getFeatures();
	if (!excludeByGroupCode(exclude_group_code, laneLinks)) {
		RUNLOGGER.Error(_T("車線リンクグループの読み込みに失敗しました : ") + basemesh_no + _T('\n'));
		return false;
	}

	if (!linkaged_links.read(laneLinks)) {
		RUNLOGGER.Error(_T("車線リンクに紐づく道路リンクの読み込みに失敗しました : ") + basemesh_no + _T('\n'));
		return false;
	}

	return true;
}

long CAdamMaxSpeedUpdater::getUpdateMaxSpeedCode(const IFeaturePtr road_link) const
{
	using namespace road_link;
	using namespace lane_link::speed_code;

	long limitSpeedC = road_link->GetValue(m_roadLinkFM.FindField(road_link::kLegalSpeed)).lVal;

	long updateCode(kUnset);
	switch (limitSpeedC)
	{
	case legal_speed::kUnInvestigated:
	case legal_speed::kZone30km:
		break;
	case legal_speed::k30kmOrLess:
		updateCode = kVariable30km;
		break;
	case legal_speed::k20km:
		updateCode = kVariable20km;
		break;
	case legal_speed::k30km:
		updateCode = kVariable30km;
		break;
	case legal_speed::k40km:
		updateCode = kVariable40km;
		break;
	case legal_speed::k50km:
		updateCode = kVariable50km;
		break;
	case legal_speed::k60km:
		updateCode = kVariable60km;
		break;
	case legal_speed::k70km:
		updateCode = kVariable70km;
		break;
	case legal_speed::k80km:
		updateCode = kVariable80km;
		break;
	case legal_speed::k90km:
		updateCode = kVariable90km;
		break;
	case legal_speed::k100km:
		updateCode = kVariable100km;
		break;
	case legal_speed::k110km:
		updateCode = kVariable110km;
		break;
	case legal_speed::k120km:
		updateCode = kVariable120km;
		break;
	default:
		break;
	}

	return updateCode;
}

 bool CAdamMaxSpeedUpdater::excludeByGroupCode(const std::list<long>& exclude_codes, std::vector<IFeaturePtr>& lane_links) const
{
	// 除外対象のグループIDを取得
	std::set<long> excludeGroupIds;
	{
		// 種別を1,000以上指定することは考えられなため、IN句制限は考慮しない
		std::list<CString> inClauses;
		sindy::CTableBase::IDs2Str(exclude_codes, inClauses);
		CString wc = uh::str_util::format(_T("%s IN (%s)"),
			lane_link_group::kGroupC, inClauses.front());

		_ICursorPtr cur;
		if (FAILED(m_laneLinkGroupT->Search(AheInitQueryFilter(nullptr, nullptr, wc), VARIANT_TRUE, &cur)))
			return false;

		_IRowPtr row;
		while (cur->NextRow(&row) == S_OK && row) {
			excludeGroupIds.insert(row->GetOID());
		}
	}

	// 除外対象のグループIDを持つ車線リンクを削除
	long idx = m_laneLinkFM.FindField(lane_link::kLaneGroupID);
	lane_links.erase(std::remove_if( std::begin(lane_links), std::end(lane_links),
		[&excludeGroupIds, &idx](IFeaturePtr f)
		{
			long groupId = f->GetValue(idx).lVal;
			return excludeGroupIds.count(groupId);
		}),
	std::end(lane_links));

	return true;
}

 bool CAdamMaxSpeedUpdater::isEquallMaxSpeed(long lane_speed_c, long road_speed_c) const
 {
	 using namespace road_link;
	 using namespace lane_link::speed_code;

	 switch (lane_speed_c)
	 {
	 case k20km:
	 case kVariable20km:
		 return road_speed_c == legal_speed::k20km;
	 case k30km:
	 case kVariable30km:
		 return
			 road_speed_c == legal_speed::k30kmOrLess ||
			 road_speed_c == legal_speed::k30km ||
			 road_speed_c == legal_speed::kZone30km;
	 case k40km:
	 case kVariable40km:
		 return road_speed_c == legal_speed::k40km;
	 case k50km:
	 case kVariable50km:
		 return road_speed_c == legal_speed::k50km;
	 case k60km:
	 case kVariable60km:
		 return road_speed_c == legal_speed::k60km;
	 case k70km:
	 case kVariable70km:
		 return road_speed_c == legal_speed::k70km;
	 case k80km:
	 case kVariable80km:
		 return road_speed_c == legal_speed::k80km;
	 case k90km:
	 case kVariable90km:
		 return road_speed_c == legal_speed::k90km;
	 case k100km:
	 case kVariable100km:
		 return road_speed_c == legal_speed::k100km;
	 case k110km:
	 case kVariable110km:
		 return road_speed_c == legal_speed::k110km;
	 case k120km:
	 case kVariable120km:
		 return road_speed_c == legal_speed::k120km;
	 default:
		 break;
	 }

	 // 上記以外の制限速度コード(可変式等)は比較出来ないので true
	 return true;
 }

 bool CAdamMaxSpeedUpdater::startEdit()
{
	if (!m_workspace.startEditing()) {
		RUNLOGGER.Error(_T("編集開始に失敗しました\n"));
		return false;
	}
	if (!m_workspace.startEditOperation()) {
		m_workspace.stopEditing(false);
		RUNLOGGER.Error(_T("編集開始に失敗しました\n"));
		return false;
	}
	return true;
}

void CAdamMaxSpeedUpdater::stopEdit()
{
	if (!m_workspace.stopEditOperation()) {
		m_workspace.stopEditing(false);
		RUNLOGGER.Error(_T("編集終了に失敗しました\n"));
		return;
	}
	if (!m_workspace.stopEditing(true)) {
		RUNLOGGER.Error(_T("編集終了に失敗しました\n"));
	}
}

void CAdamMaxSpeedUpdater::abortEdit()
{
	RUNLOGGER.Error(_T("編集に失敗しました\n"));
	if (!m_workspace.abortEditOperation()) {
		m_workspace.stopEditing(false);
		RUNLOGGER.Error(_T("編集終了に失敗しました\n"));
		return;
	}
	if (!m_workspace.stopEditing(false)) {
		RUNLOGGER.Error(_T("編集終了に失敗しました\n"));
	}
}

void CAdamMaxSpeedUpdater::finish(bool isSucceeded) const
{
	// 終了メッセージ出力
	uh::tstring result = isSucceeded ? _T("正常終了") : _T("異常終了");
	RUNLOGGER.Info(_T("処理結果: ") + result);

	// ファイルのクローズ
	RUNLOGGER.Finalize();
	ERRLOGGER.Finalize();
}

CString CAdamMaxSpeedUpdater::getChildVersionName() const
{
	static const CString prefix = _T("ETC_ADAMEDIT_");

	// 現在の日時を"YYYYMMDDHHDDSS" で取得
	CTime cTime = CTime::GetCurrentTime();
	const CString timeString = cTime.Format(_T("%Y%m%d%H%M%S"));

	return prefix + timeString;
}
