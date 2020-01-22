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

	// �����N-�|�C���g�̍��W�`�F�b�N
	sCALLBACK->callback(_T("[INFO][%s]�����N-�|�C���g�Ԃ̍��W��`�F�b�N (�T������:Point->Link)"), kLinkInfo.alias_name);

	std::map<long, const Feature*> dicLinkOID;
	std::map<long, const Feature*> dicNodeOID;
	fBuildDic(kLinkData, kFEATURE_OID, dicLinkOID);
	fBuildDic(kNodeData, kFEATURE_OID, dicNodeOID);

	std::map<long, std::vector<const Feature*>> dicNodeLink;
	fBuildDic(kNodeData, CNodeChecker::LINK_ID, dicNodeLink);

	bool err = false;

	// [LINKID][SEQUENCE][�m�[�h�t�B�[�`��] ��3�����}�b�v
	std::map<long, std::map<long, const Feature*>> dicNodeLinkSeq;
	for (const auto& itr : dicNodeLink)
	{
		// ToDo: �V�[�P���X�l����v����ꍇ�Ɍ��m�ł��Ȃ�
		for (const auto& f : itr.second){
			dicNodeLinkSeq[itr.first][GetLong<CNodeChecker::SEQUENCE>(*f)] = f;
		}

		long nq = 0;
		for (const auto& nod : dicNodeLinkSeq[itr.first])
		{
			if (nod.first != nq) {
				sCALLBACK->callback(_T("[ERROR][%s.%d]�V�[�P���X�ԍ��Ɍ��Ԃ����݂��܂�.[No=%d, SEQ=%d]"), kNodeInfo.alias_name, GetLong<3>(*nod.second), nq, nod.first);
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
			sCALLBACK->callback(_T("[ERROR][%s.%d]�I�[�|�C���g���m�[�h�ɂȂ��Ă��܂���.[CLASS=%d]"), kNodeInfo.alias_name, itr.first, class_c);
			err = true;
		}
		if (!is_last){
			sCALLBACK->callback(_T("[ERROR][%s.%d]�I�[�|�C���g�ɏI�[�t���O���t���Ă��܂���.[LAST_F=%d]"), kNodeInfo.alias_name, itr.first, is_last);
			err = true;
		}
		if (fIsEqualPosition(*first, *last, false)){
			sCALLBACK->callback(_T("[ERROR][%s.%d]�n�I�_�̍��W����v���Ă��܂�."), kLinkInfo.alias_name, itr.first);
			err = true;
		}
	}

	for (const auto& itr : kLinkData)
	{
		long objectid = GetLong<CLinkChecker::OBJECTID>(itr);

		if (itr.shape.size() != dicNodeLink[objectid].size()) {
			sCALLBACK->callback(_T("[ERROR][%s.%d]�����N�ƃm�[�h�̍\���_�����قȂ�܂�.[Link=%d, Node=%d]"), kLinkInfo.alias_name, objectid, itr.shape.size(), dicNodeLink[objectid].size());
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
			sCALLBACK->callback(_T("[ERROR][%s.%d]�����N[%s.%d]��������܂���."), kNodeInfo.alias_name, objectid, kLinkInfo.alias_name, link_id);
			err = true;
			continue;
		}
		if (f->second->shape.size() <= sequence) {
			not_found++;
			sCALLBACK->callback(_T("[ERROR][%s.%d]�V�[�P���X�������N[%s.%d]�̓_�񐔂𒴂��Ă��܂�.[SEQ=%d, PtNum=%d]"), kNodeInfo.alias_name, objectid, kLinkInfo.alias_name, link_id, sequence, f->second->shape.size());
			err = true;
			continue;
		}

		if (f->second->shape[sequence] != itr.shape[0]) {
			diff_pos++;
			auto first = CString(itr.shape[0].toString().c_str());
			auto last = CString(f->second->shape[sequence].toString().c_str());
			sCALLBACK->callback(_T("[ERROR][%s.%d][%s.%d]�̍��W�l����v���܂���.[SEQ=%d][Node=%s][Link=%s]"), 
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
	if (not_found > 0) { sCALLBACK->callback(_T("[INFO][%s]�f�[�^�s����: %d"), kLinkInfo.alias_name, not_found); }
	if (diff_pos  > 0) { sCALLBACK->callback(_T("[INFO][%s]���W�s��v��: %d"), kLinkInfo.alias_name, diff_pos); }


	//// �d�����R�[�h�̃`�F�b�N
	sCALLBACK->callback(_T("[INFO][%s]�m�[�hID�̓���`�F�b�N"), kNodeInfo.alias_name);

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
				sCALLBACK->callback(_T("[ERROR][%s.%d]���W�d���ł̃|�C���g��ʃR�[�h���s���ł�. [CLASS_C=%d]"), kNodeInfo.alias_name, objectid, class_c);
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
					sCALLBACK->callback(_T("[ERROR][%s.%d]�m�[�h�̑����l����v���܂���. [LEFT_OID=%d, RIGHT_OID=%d]"), 
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
					sCALLBACK->callback(_T("[ERROR][%s.%d]�m�[�h�̌`�󂪈�v���܂���. [LEFT_OID=%d, RIGHT_OID=%d][LEFT=%s, RIGHT=%s]"), 
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
	if (not_equal > 0) { sCALLBACK->callback(_T("[INFO][%s]����m�[�h�̑����s��v��: %d"), kNodeInfo.alias_name, not_equal); }
	if (invalid   > 0) { sCALLBACK->callback(_T("[INFO][%s]����m�[�h�̑����s���l��: %d"), kNodeInfo.alias_name, invalid); }

	// �ȉ��́A�Ԑ��݂̂�z��B
	// TODO: �ȉ��`�F�b�N�͎Ԑ��m�[�h�_���`�F�b�J�[�ֈڂ����ʃN���X������
	if (!(kLinkInfo.file_name == _T("lane_link.csv") && kNodeInfo.file_name == _T("lane_point.csv")))
		return true;

	// �m�[�h�ԋ����̃`�F�b�N
	sCALLBACK->callback(_T("[INFO][%s]�c�f���z�`�F�b�N"), kNodeInfo.alias_name);
	for (const auto& itr : dicNodeLink)
	{
		const auto& arr = itr.second;
		const auto  n = arr.size() - 1;

		for (auto i = 0; i<n; i++)
		{
			const auto& lhs = *arr[i];
			const auto& rhs = *arr[i + 1];

			// ���z�̋��ߕ��� 3�����x�N�g���̑傫���ɑ΂��āH
			// �[���t�H�[�}�b�g��Z�l�̐��x���e���H���ߐ������v�Z�ł��Ȃ�
			double z = rhs.shape[0].alt - lhs.shape[0].alt;
			const auto  len = fCalcHubeny(lhs.shape[0], rhs.shape[0]);
			//double calc_slope = z / hypot(len, z) * 100.;
			double calc_slope = z / len * 100.;
			double lhs_slope = GetDouble<CLaneNodeChecker::LONGITUDINAL_SLOPE>(lhs);
			if (fabs(calc_slope - lhs_slope) > 0.1)
			{
				sCALLBACK->callback(_T("[WARNING][%s.%d]�c�f���z���v�Z�l�ƈ�v���܂���. [LEFT_OID=%d, RIGHT_OID=%d][DATA=%.3f, CALC=%.3f][XY=%.3f, Z=%.3f]"),
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
