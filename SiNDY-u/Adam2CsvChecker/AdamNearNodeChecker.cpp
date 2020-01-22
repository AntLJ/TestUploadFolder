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
#include "AdamNearNodeChecker.h"


namespace bgm = boost::geometry::model;
typedef bgm::point<double, 2, boost::geometry::cs::cartesian> BoostPoint;
typedef bgm::box<BoostPoint> BoostBox;
typedef std::pair<BoostPoint, const Feature*> BoostValue;

namespace bgi = boost::geometry::index;
typedef bgi::rtree<BoostValue, bgi::linear<32> > BoostRTree;

bool sindy::adam2::format::CNearNodeChecker::run()
{
	assert(sCALLBACK && sALL_FEATURE && indexes.size() == 1);
	if (!(sCALLBACK && sALL_FEATURE && indexes.size() == 1)) { return false; }

	const auto& kNodeInfo = kFeatureFileList[indexes[0]];
	const auto& kNodeData = (*sALL_FEATURE)[indexes[0]];

	// TODO: 近傍ノードチェックから別クラス化
	sCALLBACK->callback(_T("[INFO][%s]ノードの同一座標チェック"), kNodeInfo.alias_name);

	std::map<Position, std::vector<const Feature*> > dicNodeShape;
	for (const auto& itr : kNodeData) { dicNodeShape[itr.shape[0].XY()].push_back(&itr); }
	for (const auto& itr : dicNodeShape)
	{
		assert(!itr.second.empty());
		if (itr.second.size() == 1) { continue; }

		const auto* first = itr.second[0];
		long first_oid = GetLong<CNodeChecker::OBJECTID>(*first);
		long first_nid = GetLong<CNodeChecker::NODE_ID>(*first);
		for (const auto* ptr : itr.second)
		{
			assert(ptr);
			long objectid = GetLong<CNodeChecker::OBJECTID>(*ptr);
			long node_id = GetLong<CNodeChecker::NODE_ID>(*ptr);
			if (first_nid != node_id)
			{ 
				sCALLBACK->callback(
					_T("[ERROR][%s.%d]ノードIDが一致していません. [RIGHT_OID=%d][%s]"), 
					kNodeInfo.alias_name, 
					first_oid, objectid, 
					CString(first->shape[0].toString().c_str()));
			}
			if (first->shape[0].alt != ptr->shape[0].alt)
			{ 
				sCALLBACK->callback(
					_T("[ERROR][%s.%d]ノード高が一致していません. [RIGHT_OID=%d][%s]"),
					kNodeInfo.alias_name,
					first_oid, objectid,
					CString(first->shape[0].toString().c_str()));
			}
		}
	}

	sCALLBACK->callback(_T("[INFO][%s]ノード間の近傍チェック"), kNodeInfo.alias_name);

	BoostRTree dicRTree;
	for (const auto& itr : kNodeData)
	{
		if (GetLong<CNodeChecker::POINT_CLASS_C>(itr) != 1) { continue; }

		BoostPoint bp(itr.shape[0].lon, itr.shape[0].lat);
		dicRTree.insert(BoostValue(bp, &itr));
	}

	for (const auto& itr : kNodeData)
	{
		if (GetLong<CNodeChecker::POINT_CLASS_C>(itr) != 1)
			continue;

		static const double kOFFSET = 1.0e-6;

		const auto& c = itr.shape[0];
		BoostPoint pin(c.lon - kOFFSET, c.lat - kOFFSET);
		BoostPoint pax(c.lon + kOFFSET, c.lat + kOFFSET);
		BoostBox box(pin, pax);

		std::vector<BoostValue> result;
		dicRTree.query(bgi::intersects(box), back_inserter(result));

		assert(!result.empty());
		for (const auto& r : result)
		{
			const auto* ptr = r.second;
			assert(ptr);

			// XY平面で同一であれば同一座標チェックで確認できるのでパス
			if (c.equal2(ptr->shape[0])) { continue; }

			double dist = fCalcHubeny(c, ptr->shape[0]);
			if (dist < m_threshold)
			{
				long first_oid = GetLong<3>(itr);
				long right_oid = GetLong<3>(*ptr);
				sCALLBACK->callback(_T("[WARNING][%s.%d]近傍ノードが存在しています. [RIGHT_OID=%d][%.3lf][%s]"), 
									kNodeInfo.alias_name,
									first_oid,
									right_oid,
									dist,
									CString(c.toString().c_str()));
			}
		}
	}

	return true;
}
