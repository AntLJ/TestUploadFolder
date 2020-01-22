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
#include "AdamNwChecker.h"

bool sindy::adam2::format::CNwChecker::run()
{
	assert(sCALLBACK && sALL_FEATURE && indexes.size() == 2);
	if (!(sCALLBACK && sALL_FEATURE && indexes.size() == 2)) { return false; }

	const auto& kLinkInfo = kFeatureFileList[indexes[0]];
	const auto& kLinkData = (*sALL_FEATURE)[indexes[0]];

	const auto& kNodeInfo = kFeatureFileList[indexes[1]];
	const auto& kNodeData = (*sALL_FEATURE)[indexes[1]];

	// リンク-ポイントの座標チェック
	sCALLBACK->callback(_T("[INFO][%s]リンク-ポイント間の座標列チェック (探索方向:Point->Link)"), kLinkInfo.alias_name);

	std::map<long, const Feature*> dicLinkOID;
	std::map<long, const Feature*> dicNodeOID;
	fBuildDic(kLinkData, kFEATURE_OID, dicLinkOID);
	fBuildDic(kNodeData, kFEATURE_OID, dicNodeOID);

	std::map<long, std::vector<const Feature*>> dicNodeLink;
	fBuildDic(kNodeData, CNodeChecker::LINK_ID, dicNodeLink);

	bool err = false;

	// [LINKID][SEQUENCE][ノードフィーチャ] の3次元マップ
	std::map<long, std::map<long, const Feature*>> dicNodeLinkSeq;
	for (const auto& itr : dicNodeLink)
	{
		// ToDo: シーケンス値が一致する場合に検知できない
		for (const auto& f : itr.second){
			dicNodeLinkSeq[itr.first][GetLong<CNodeChecker::SEQUENCE>(*f)] = f;
		}

		long nq = 0;
		for (const auto& nod : dicNodeLinkSeq[itr.first])
		{
			if (nod.first != nq) {
				sCALLBACK->callback(_T("[ERROR][%s.%d]シーケンス番号に欠番が存在します.[No=%d, SEQ=%d]"), kNodeInfo.alias_name, GetLong<3>(*nod.second), nq, nod.first);
				err = true;
			}
			nq = nod.first + 1;
		}

		const auto* first = dicNodeLinkSeq[itr.first].begin()->second;
		const auto* last = dicNodeLinkSeq[itr.first].rbegin()->second;
		assert(first && last);

		long class_c = GetLong<CNodeChecker::POINT_CLASS_C>(*last);
		long is_last = GetLong<CNodeChecker::LAST_NODE_F>(*last);
		if (class_c != 1){
			sCALLBACK->callback(_T("[ERROR][%s.%d]終端ポイントがノードになっていません.[CLASS=%d]"), kNodeInfo.alias_name, itr.first, class_c);
			err = true;
		}
		if (!is_last){
			sCALLBACK->callback(_T("[ERROR][%s.%d]終端ポイントに終端フラグが付いていません.[LAST_F=%d]"), kNodeInfo.alias_name, itr.first, is_last);
			err = true;
		}
		if (fIsEqualPosition(*first, *last, false)){
			sCALLBACK->callback(_T("[ERROR][%s.%d]始終点の座標が一致しています."), kLinkInfo.alias_name, itr.first);
			err = true;
		}
	}

	for (const auto& itr : kLinkData)
	{
		long objectid = GetLong<CLinkChecker::OBJECTID>(itr);

		if (itr.shape.size() != dicNodeLink[objectid].size()) {
			sCALLBACK->callback(_T("[ERROR][%s.%d]リンクとノードの構成点数が異なります.[Link=%d, Node=%d]"), kLinkInfo.alias_name, objectid, itr.shape.size(), dicNodeLink[objectid].size());
			err = true;
		}
	}

	long not_found = 0;
	long diff_pos = 0;
	for (const auto& itr : kNodeData)
	{
		assert(itr.shape.size() == 1);

		long objectid = GetLong<CNodeChecker::OBJECTID>(itr);
		long class_c = GetLong<CNodeChecker::POINT_CLASS_C>(itr);
		long link_id = GetLong<CNodeChecker::LINK_ID>(itr);
		long sequence = GetLong<CNodeChecker::SEQUENCE>(itr);
		long is_last = GetLong<CNodeChecker::LAST_NODE_F>(itr);

		auto f = dicLinkOID.find(link_id);
		if (f == dicLinkOID.end()) {
			not_found++;
			sCALLBACK->callback(_T("[ERROR][%s.%d]リンク[%s.%d]が見つかりません."), kNodeInfo.alias_name, objectid, kLinkInfo.alias_name, link_id);
			err = true;
			continue;
		}
		if (f->second->shape.size() <= sequence) {
			not_found++;
			sCALLBACK->callback(_T("[ERROR][%s.%d]シーケンスがリンク[%s.%d]の点列数を超えています.[SEQ=%d, PtNum=%d]"), kNodeInfo.alias_name, objectid, kLinkInfo.alias_name, link_id, sequence, f->second->shape.size());
			err = true;
			continue;
		}

		if (f->second->shape[sequence] != itr.shape[0]) {
			diff_pos++;
			auto first = CString(itr.shape[0].toString().c_str());
			auto last = CString(f->second->shape[sequence].toString().c_str());
			sCALLBACK->callback(_T("[ERROR][%s.%d][%s.%d]の座標値が一致しません.[SEQ=%d][Node=%s][Link=%s]"), 
				                kNodeInfo.alias_name, 
				                objectid, 
				                kLinkInfo.alias_name,
				                link_id, 
				                sequence, 
				                first,
				                last);
			err = true;
			continue;
		}
	}
	if (not_found > 0) { sCALLBACK->callback(_T("[INFO][%s]データ不足数: %d"), kLinkInfo.alias_name, not_found); }
	if (diff_pos  > 0) { sCALLBACK->callback(_T("[INFO][%s]座標不一致数: %d"), kLinkInfo.alias_name, diff_pos); }


	//// 重複レコードのチェック
	sCALLBACK->callback(_T("[INFO][%s]ノードIDの同一チェック"), kNodeInfo.alias_name);

	std::map<long, std::vector<const Feature*> > overlap;
	fBuildDic(kNodeData, CNodeChecker::NODE_ID, overlap);

	long not_equal = 0;
	long invalid = 0;

	for (const auto& itr : overlap)
	{
		auto n = itr.second.size();
		if (n < 2) { continue; }
		std::string str;
		for (const auto& node : itr.second) { 
			str += form("%d,", GetLong<CNodeChecker::OBJECTID>(*node));
		}

		for (long left = 0; left < n; left++)
		{
			auto objectid = GetLong<CNodeChecker::OBJECTID>(*itr.second[left]);
			auto class_c = GetLong<CNodeChecker::POINT_CLASS_C>(*itr.second[left]);
			if (class_c != 1) {
				sCALLBACK->callback(_T("[ERROR][%s.%d]座標重複でのポイント種別コードが不正です. [CLASS_C=%d]"), kNodeInfo.alias_name, objectid, class_c);
				err = true;
				invalid++;
			}

			for (long right = left + 1; right<n; right++)
			{
				const auto& lhs = *itr.second[left];
				const auto& rhs = *itr.second[right];

				if (!fIsEqualAttr(lhs, rhs, m_chk_field_idxes))
				{
					not_equal++;
					sCALLBACK->callback(_T("[ERROR][%s.%d]ノードの属性値が一致しません. [LEFT_OID=%d, RIGHT_OID=%d]"), 
						                kNodeInfo.alias_name,
						                objectid, 
						                GetLong<kFEATURE_OID>(lhs),
						                GetLong<kFEATURE_OID>(rhs));
					err = true;
				}
				if (!fIsEqualPosition(lhs, rhs))
				{
					const CString lhs_shape(lhs.shape[0].toString().c_str());
					const CString rhs_shape(rhs.shape[0].toString().c_str());

					not_equal++;
					sCALLBACK->callback(_T("[ERROR][%s.%d]ノードの形状が一致しません. [LEFT_OID=%d, RIGHT_OID=%d][LEFT=%s, RIGHT=%s]"), 
										kNodeInfo.alias_name,
										objectid,
										GetLong<kFEATURE_OID>(lhs),
										GetLong<kFEATURE_OID>(rhs),
										lhs_shape,
										rhs_shape);
					err = true;
				}
			}
		}
	}
	if (not_equal > 0) { sCALLBACK->callback(_T("[INFO][%s]同一ノードの属性不一致数: %d"), kNodeInfo.alias_name, not_equal); }
	if (invalid   > 0) { sCALLBACK->callback(_T("[INFO][%s]同一ノードの属性不正値数: %d"), kNodeInfo.alias_name, invalid); }

	// 以下は、車線のみを想定。
	// TODO: 以下チェックは車線ノード論理チェッカーへ移すか別クラス化する
	if (!(kLinkInfo.file_name == _T("lane_link.csv") && kNodeInfo.file_name == _T("lane_point.csv")))
		return true;

	// ノード間距離のチェック
	sCALLBACK->callback(_T("[INFO][%s]縦断勾配チェック"), kNodeInfo.alias_name);
	for (const auto& itr : dicNodeLink)
	{
		const auto& arr = itr.second;
		const auto  n = arr.size() - 1;

		for (auto i = 0; i<n; i++)
		{
			const auto& lhs = *arr[i];
			const auto& rhs = *arr[i + 1];

			// 勾配の求め方が 3次元ベクトルの大きさに対して？
			// 納入フォーマットのZ値の精度が粗い？ため正しく計算できない
			double z = rhs.shape[0].alt - lhs.shape[0].alt;
			const auto  len = fCalcHubeny(lhs.shape[0], rhs.shape[0]);
			//double calc_slope = z / hypot(len, z) * 100.;
			double calc_slope = z / len * 100.;
			double lhs_slope = GetDouble<CLaneNodeChecker::LONGITUDINAL_SLOPE>(lhs);
			if (fabs(calc_slope - lhs_slope) > 0.1)
			{
				sCALLBACK->callback(_T("[WARNING][%s.%d]縦断勾配が計算値と一致しません. [LEFT_OID=%d, RIGHT_OID=%d][DATA=%.3f, CALC=%.3f][XY=%.3f, Z=%.3f]"),
									kLinkInfo.alias_name,
									itr.first,
									GetLong<kFEATURE_OID>(lhs),
									GetLong<kFEATURE_OID>(rhs),
									lhs_slope,
									calc_slope,
									len,
									z);
				err = true;
			}
		}
	}
	
	return !err;
}
