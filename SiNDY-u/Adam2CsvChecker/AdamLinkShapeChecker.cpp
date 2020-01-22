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
#include "AdamLinkShapeChecker.h"

bool sindy::adam2::format::CLinkShapeChecker::run()
{
	assert(sCALLBACK && sALL_FEATURE && indexes.size() == 1);
	if (!(sCALLBACK && sALL_FEATURE && indexes.size() == 1)) { return false; }

	const auto& kInfo = kFeatureFileList[indexes[0]];
	const auto& kData = (*sALL_FEATURE)[indexes[0]];

	sCALLBACK->callback(_T("[INFO][%s]��Ȍ`��`�F�b�N"), kInfo.alias_name);

	bool err = false;
	for (const auto& itr : kData)
	{
		const auto& shape = itr.shape;
		long objectid = GetLong<CLinkChecker::OBJECTID>(itr);
		long from_node = GetLong<CLinkChecker::FROM_NODE_ID>(itr);
		long to_node = GetLong<CLinkChecker::TO_NODE_ID>(itr);

		if (from_node == to_node){
			sCALLBACK->callback(_T("[ERROR][%s.%d]�n�I�_�m�[�hID����v���Ă��܂�.[NODE=%d]"), kInfo.alias_name, objectid, from_node);
			err = true;
		}

		{
			std::set<Position> dic;
			for (const auto& pos : shape)
			{
				if (!dic.insert(pos).second)
				{
					CString str(pos.toString().c_str());
					sCALLBACK->callback(_T("[ERROR][%s.%d]�P��`����ɓ������W�����݂��܂�.[POS=%s]"), kInfo.alias_name, objectid, str);
					err = true;
				}
			}
		}

		const auto n = shape.size();
		if (n < 2){
			sCALLBACK->callback(_T("[ERROR][%s.%d]�_�񐔂��s�����Ă��܂�.[PtNum=%d]"), kInfo.alias_name, objectid, n);
			err = true;
		}

		// ��Ȍ`��ɂ͂Ȃ�Ȃ��̂Ŏ���...
		if (n < 3) { continue; }

		for (auto i = 1; i<n - 1; i++)
		{
			const auto& prev = shape[i - 1];
			const auto& curr = shape[i];
			const auto& next = shape[i + 1];

			const Position A = { prev.lon - curr.lon, prev.lat - curr.lat, prev.alt - curr.alt };
			const Position B = { next.lon - curr.lon, next.lat - curr.lat, next.alt - curr.alt };

			const auto lAl = hypot(A.lon, A.lat);
			const auto lBl = hypot(B.lon, B.lat);

			const auto cosv = (A.lon * B.lon + A.lat * B.lat) / (lAl * lBl);
			const auto radian = acos(cosv);
			const auto degree = radian * 180. / 3.1415926535897932384626433832795;
			if (_finite(degree) && fabs(degree) < 30.)
			{
				sCALLBACK->callback(_T("[ERROR][%s.%d]��Ȍ`�󂪔������Ă��܂�.[SEQ=%d, DEG=%lf]"), kInfo.alias_name, objectid, i, degree);
				err = true;
			}
		}
	}

	// �����N�`��̏d���`�F�b�N
	for (auto it1 = kData.begin(); it1 != kData.end(); ++it1)
	{
		const auto first = it1->shape;

		auto it2 = it1;
		for (++it2; it2 != kData.end(); ++it2)
		{
			assert(it1 != it2);
			auto second = it2->shape;

			if (first == second) {
				sCALLBACK->callback(_T("[ERROR][%s.%d]�����N�̌`�󂪈�v���Ă��܂�.[LEFT_OID=%d, RIGHT_OID=%d][������]"), kInfo.alias_name, GetLong<kFEATURE_OID>(it1->attr), GetLong<kFEATURE_OID>(it1->attr), GetLong<kFEATURE_OID>(it2->attr));
				err = true;
			}
			reverse(second.begin(), second.end());
			if (first == second) {
				sCALLBACK->callback(_T("[ERROR][%s.%d]�����N�̌`�󂪈�v���Ă��܂�.[LEFT_OID=%d, RIGHT_OID=%d][�t����]"), kInfo.alias_name, GetLong<kFEATURE_OID>(it1->attr), GetLong<kFEATURE_OID>(it1->attr), GetLong<kFEATURE_OID>(it2->attr));
				err = true;
			}
		}
	}

	return !err;
}
