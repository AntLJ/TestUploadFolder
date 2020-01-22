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
#include "AdamRelationChecker.h"

bool sindy::adam2::format::CLinkRelationChecker::run()
{
	assert(sCALLBACK && sALL_FEATURE && sALL_ROW && indexes.size() == 3);
	if (!(sCALLBACK && sALL_FEATURE && sALL_ROW && indexes.size() == 3)) { return false; }

	const auto& kLaneInfo = kFeatureFileList[indexes[0]];
	const auto& kLaneData = (*sALL_FEATURE)[indexes[0]];
	const auto& kOtherInfo = kFeatureFileList[indexes[1]];
	const auto& kOtherData = (*sALL_FEATURE)[indexes[1]];
	const auto& kRelInfo = kRelFileList[indexes[2]];
	const auto& kRelData = (*sALL_ROW)[indexes[2]];

	bool has_err = false;

	//// �֘A�e�[�u���`�F�b�N
	sCALLBACK->callback(_T("[INFO][%s]�����N�̊֌W��(�O���L�[)�`�F�b�N"), kRelInfo.alias_name);

	// LANE -> rel��T��
	sCALLBACK->callback(_T("[INFO][%s]�O���L�[���݃`�F�b�N (��������:Lane->rel)"), kRelInfo.alias_name);
	{
		long not_found = 0;

		std::map<long, std::vector<const Row*> > dic;
		fBuildDic(kRelData, CRelLinkChecker::LANE_ID, dic);
		for (const auto& itr : kLaneData)
		{
			long oid = GetLong<kFEATURE_OID>(itr);
			long structure_c = GetLong<CLaneLinkChecker::ROAD_STRUCTURE_C>(itr);

			auto f = dic.find(oid);
			if (f == dic.end())
			{
				sCALLBACK->callback(_T("[WARNING][%s]%s.%d�ɕR�t���֘A�f�[�^��rel_*.csv�ɒ�`����Ă��܂���."), 
									kRelInfo.alias_name, 
									kLaneInfo.alias_name,
									oid);
				not_found++;
				has_err = true;
				continue;
			}

			// ���Ԑ���Ԃ͑������N�ւ̕R�t���͎����Ȃ��̂ŃX�L�b�v
			if (structure_c == 6)
				continue;

			const auto& vec = f->second;

			long flag = 0;
			for (const auto* row : vec)
			{
				assert(row);
				long lr_f = GetLong<CRelLinkChecker::LR_F>(row->attr);
				if (lr_f == 0) { flag |= 0x01; }
				else { flag |= 0x02; }
			}
			if (flag != 0x03)
			{
				if (!(flag & 0x01)) { sCALLBACK->callback(_T("[WARNING][%s]%s.%d�̍�����%s�����݂��܂���"), kRelInfo.alias_name, kLaneInfo.alias_name, oid, kOtherInfo.alias_name); }
				if (!(flag & 0x02)) { sCALLBACK->callback(_T("[WARNING][%s]%s.%d�̉E����%s�����݂��܂���"), kRelInfo.alias_name, kLaneInfo.alias_name, oid, kOtherInfo.alias_name); }
				has_err = true;
			}
		}
	}

	// Other -> LANE ��T��
	sCALLBACK->callback(_T("[INFO][%s]�O���L�[���݃`�F�b�N (��������:Other->rel)"), kRelInfo.alias_name);
	{
		long not_found = 0;

		std::map<long, std::vector<const Row*>> dic;
		fBuildDic(kRelData, CRelLinkChecker::LINK_ID, dic);
		for (const auto& itr : kOtherData)
		{
			long oid = GetLong<kFEATURE_OID>(itr);
			auto f = dic.find(oid);
			if (f == dic.end())
			{
				sCALLBACK->callback(_T("[WARNING][%s]%s.%d�ɕR�t���֘A�f�[�^��rel_*.csv�ɒ�`����Ă��܂���."),
									kRelInfo.alias_name,
									kOtherInfo.alias_name,
									oid);
				not_found++;
				has_err = true;
				continue;
			}

			const auto& vec = f->second;

			// TODO : �ȉ��̃`�F�b�N������Ȃ�΁A�`�F�b�N�Ώۂ������̎Ԑ��敪���ɍi��ׂ��H

			//long flag = 0;
			//for (const auto* row : vec)
			//{
			//	assert(row);
			//	long lr_f = GetLong<CRelLinkChecker::LR_F>(row->attr);
			//	if (lr_f == 0) { flag |= 0x01; }
			//	else { flag |= 0x02; }
			//}
			//if (flag != 0x03)
			//{
			//	if (!(flag & 0x01)) { 
			//		sCALLBACK->callback(_T("[WARNING][%s]%s.%d�̉E����%s�����݂��܂���"), 
			//							kRelInfo.alias_name,
			//							kOtherInfo.alias_name,
			//							oid,
			//							kLaneInfo.alias_name);
			//	}
			//	if (!(flag & 0x02)) { 
			//		sCALLBACK->callback(_T("[WARNING][%s]%s.%d�̍�����%s�����݂��܂���"),
			//							kRelInfo.alias_name,
			//							kOtherInfo.alias_name,
			//							oid,
			//							kLaneInfo.alias_name);
			//	}
			//	has_err = true;
			//}
		}
	}

	sCALLBACK->callback(_T("[INFO][%s]�O���L�[�`�F�b�N (��������:rel->Lane)"), kRelInfo.alias_name);
	std::map<long, const Feature*> dicLane;  fBuildDic(kLaneData, kFEATURE_OID, dicLane);
	std::map<long, const Feature*> dicOther; fBuildDic(kOtherData, kFEATURE_OID, dicOther);

	std::set<long> rel_lane;
	std::set<long> rel_other;
	// LANE_ID, LR_F, SEQUENCE, OIDs
	std::map<long, std::map<long, std::map<long, std::vector<long>>>>  dicID_LR_SEQ;    
	// LANE_ID, OTHER_ID
	std::map<long, std::map<long, std::vector<long>>> dicID_OTHER;     

	long not_found_lane = 0;
	long not_found_other = 0;
	for (const auto& itr : kRelData)
	{
		const auto& row = itr.attr;
		long oid = GetLong<CRelLinkChecker::OBJECTID>(row);
		long lane_link = GetLong<CRelLinkChecker::LANE_ID>(row);
		long lr_f = GetLong<CRelLinkChecker::LR_F>(row);
		long other_link = GetLong<CRelLinkChecker::LINK_ID>(row);
		long direction = GetLong<CRelLinkChecker::LINK_DIRECTION_C>(row);
		long sequence = GetLong<CRelLinkChecker::SEQUENCE>(row);

		rel_lane.insert(lane_link);
		rel_other.insert(other_link);

		dicID_LR_SEQ[lane_link][lr_f][sequence].push_back(oid);
		dicID_OTHER[lane_link][other_link].push_back(oid);

		auto fLane = dicLane.find(lane_link);
		auto fOther = dicOther.find(other_link);

		bool err = false;
		if (fLane == dicLane.end())
		{
			not_found_lane++;
			sCALLBACK->callback(_T("[ERROR][%s.%d]�����N[%s.%d]�����f�[�^���Ɍ�����܂���."), 
								kRelInfo.alias_name,
								oid,
								kLaneInfo.alias_name,
								lane_link);
			err = true;
		}
		if (fOther == dicOther.end())
		{
			not_found_other++;
			sCALLBACK->callback(_T("[ERROR][%s.%d]�����N[%s.%d]�����f�[�^���Ɍ�����܂���."),
								kRelInfo.alias_name,
								oid,
								kOtherInfo.alias_name,
								other_link);
			err = true;
		}
		if (err) { 
			has_err = true;
			continue;
		}

		if (!checkAttr(itr, *fLane->second, *fOther->second))
			has_err = true;


		// �ʒu�֌W�̃`�F�b�N
		std::vector<Position> pos = fOther->second->shape;
		if (direction == 2) { std::reverse(pos.begin(), pos.end()); }

		auto nLane = fLane->second->shape.size();
		const Position& A = (sequence == 0) ? fLane->second->shape[0] : fLane->second->shape[nLane - 2];
		const Position& B = (sequence == 0) ? fLane->second->shape[1] : fLane->second->shape[nLane - 1];
		const Position& T = (sequence == 0) ? pos.front() : pos.back();

		auto judge
			= T.lon * (A.lat - B.lat)
			+ A.lon * (B.lat - T.lat)
			+ B.lon * (T.lat - A.lat)
			;
		long dir = (judge > 0) ? 0 : 1;     // <0: ��, >0: �E

		if (lr_f != dir)
		{
			sCALLBACK->callback(_T("[WARNING][%s.%d]���E�t���O�Ɍ�肪����܂�. [%s.%d][%s.%d]"),
								kRelInfo.alias_name,
								oid, kLaneInfo.alias_name,
								lane_link,
								kOtherInfo.alias_name,
								other_link);
			has_err = true;
		}
	}
	// �d�����R�[�h�̊m�F
	for (const auto& itID : dicID_LR_SEQ){
		for (const auto& itLR : itID.second){
			for (const auto& itSEQ : itLR.second)
			{
				if (itSEQ.second.size() > 1)
				{
					sCALLBACK->callback(_T("[WARNING][%s]��������̃f�[�^���������݂��܂�.[LINK=%d, LR_F=%d, SEQ=%d, Count=%d]"),
										kRelInfo.alias_name,
										itID.first,
										itLR.first,
										itSEQ.first,
										itSEQ.second.size());
					has_err = true;
				}
			}
		}
	}
	for (const auto& itID : dicID_OTHER){
		for (const auto& itOTHER : itID.second)
		{
			if (itOTHER.second.size() > 1)
			{
				sCALLBACK->callback(_T("[ERROR][%s]��������̃f�[�^���������݂��܂�.[LINK=%d, OTHER=%d, Count=%d]"),
									kRelInfo.alias_name,
									itID.first,
									itOTHER.first,
									itOTHER.second.size());
				has_err = true;
			}
		}
	}
	if (kLaneData.size() != rel_lane.size())
	{
		sCALLBACK->callback(_T("[INFO][%s]%s�����N��(%d)��%s�����N��(%d)����v���܂���."),
							kRelInfo.alias_name,
							kRelInfo.file_name,
							rel_lane.size(),
							kLaneInfo.file_name,
							kLaneData.size());
		has_err = true;
	}
	if (kOtherData.size() != rel_other.size())
	{
		sCALLBACK->callback(_T("[INFO][%s]%s�����N��(%d)��%s�����N��(%d)����v���܂���."),
							kRelInfo.alias_name,
							kRelInfo.file_name,
							rel_other.size(), 
							kOtherInfo.file_name,
							kOtherData.size());
		has_err = true;
	}

	return !has_err;
}



bool sindy::adam2::format::CLaneCompartLinkRelationChecker::checkAttr(const Row & rel, const Feature & lane_link, const Feature & compart_link)
{
	long lr_f = GetLong<CRelCompartLinkLaneLinkChecker::LR_F>(rel);
	long lane_no = GetLong<CLaneLinkChecker::LANE_NO>(lane_link);
	long lane_count = GetLong<CLaneLinkChecker::LANE_COUNT>(lane_link);
	long left_change = GetLong<CLaneLinkChecker::LEFT_CHANGE_F>(lane_link);
	long right_change = GetLong<CLaneLinkChecker::RIGHT_CHANGE_F>(lane_link);
	long compart_c = GetLong<CCompartLinkChecker::COMPART_LINE_C>(compart_link);

	// ���z���͑ΏۊO
	if (compart_c == 0)
		return true;

	// ��������ʃR�[�h�͔j����"1"�A��������"2"�A��������"3"�Ƃ������l��Ƃ��ĕ\�킳��� 
	// (e.g. 132 => �j���E�������E��������3�d��)
	// ���̂��߁A1���ځA�ő包�ڂ̒l��"1"�ł��邩���m�F����΁A���E����̎Ԑ��ύX���\���ǂ������킩��
	unsigned int digit = std::to_string(compart_c).length();
	bool movable_from_right = ((compart_c % 10) == 1);
	bool movable_from_left = ((compart_c / ((int)std::pow(10, digit - 1))) == 1);

	auto fInvalid = (lr_f == 0)                    // lane���猩�� (0: ���A1: �E)
		? ((left_change == 1)  != movable_from_right)     // ����
		: ((right_change == 1) != movable_from_left);     // �E��
	if (fInvalid)
	{
		const auto& kRelInfo = kRelFileList[index_rel_lane_link_compartment_link];
		long oid = GetLong<0>(rel);
		long change_f = (lr_f == 0) ? left_change : right_change;

		long lane_id = GetLong<kFEATURE_OID>(lane_link);
		long comp_id = GetLong<kFEATURE_OID>(compart_link);

		sCALLBACK->callback(
			_T("[ERROR][%s.%d]�Ԑ�.�Ԑ��ύX�\�t���O�Ƌ���.����ʃR�[�h���Ή����Ă��܂���.[LR=%d][LANE_ID=%d, CHANGE_F=%d][COMPART_ID=%d, COMPART_C=%d]"),
			kRelInfo.alias_name,
			oid,
			lr_f,
			lane_id,
			change_f,
			comp_id,
			compart_c
			);
	}

	return !fInvalid;
}


bool sindy::adam2::format::CNodeRelationChecker::run()
{
	assert(sCALLBACK && sALL_FEATURE && sALL_ROW && indexes.size() == 3);
	if (!(sCALLBACK && sALL_FEATURE && sALL_ROW && indexes.size() == 3)) { return false; }

	const auto& kLaneInfo = kFeatureFileList[indexes[0]];
	const auto& kLaneData = (*sALL_FEATURE)[indexes[0]];
	const auto& kOtherInfo = kFeatureFileList[indexes[1]];
	const auto& kOtherData = (*sALL_FEATURE)[indexes[1]];
	const auto& kRelInfo = kRelFileList[indexes[2]];
	const auto& kRelData = (*sALL_ROW)[indexes[2]];

	bool err = false;

	//// �֘A�e�[�u���`�F�b�N
	sCALLBACK->callback(_T("[INFO][%s]�m�[�h�̊֌W��(�O���L�[)�`�F�b�N"), kRelInfo.alias_name);

	// TODO: �P���ɋ����Ń`�F�b�N���������ł́A�s�v�ȃG���[���o�����邽�߁A�R�����g�A�E�g
	// Other -> LANE�Q�̕R�t�̕s��v���o
	//sCALLBACK->callback(_T("[INFO][%s]�O���L�[�R�t�ʒu�`�F�b�N"), kRelInfo.alias_name);
	//{
	//	// �������͖��ʃ@...
	//	std::map<long, std::vector<const Feature*>> dicLane;
	//	fBuildDic(kLaneData, CLaneNodeChecker::NODE_ID, dicLane);

	//	std::map<long, std::vector<const Row*>> arr;
	//	fBuildDic(kRelData, 2, arr);
	//	for (const auto& itr : arr)
	//	{
	//		if (itr.second.size() <= 1) { continue; }

	//		int n = int(itr.second.size());
	//		for (int i = 0; i<n; i++)
	//		{
	//			bool is_valid = false;
	//			for (int j = i + 1; j<n; j++)
	//			{
	//				const auto& pi = itr.second[i]->attr;
	//				const auto& pj = itr.second[j]->attr;

	//				const auto  fi = dicLane.find(GetLong<1>(pi));
	//				const auto  fj = dicLane.find(GetLong<1>(pj));

	//				if (fi == dicLane.end() || fj == dicLane.end()) { assert(false); continue; }

	//				// �ȈՃ`�F�b�N: �m�[�h�ԋ������Ԑ����������傫�����H
	//				const double di = GetDouble<10>(*fi->second[0]);
	//				const double dj = GetDouble<10>(*fj->second[0]);

	//				const auto& si = fi->second[0]->shape[0];
	//				const auto& sj = fj->second[0]->shape[0];
	//				auto dist = fCalcHubeny(si, sj);

	//				// ���ߖT��1�ł��������ok�Ƃ݂Ȃ�
	//				dist /= 1.2;
	//				if (dist < di && dist < dj) {
	//					is_valid = true;
	//					break;
	//				}
	//			}
	//			if (!is_valid)
	//			{
	//				sCALLBACK->callback(_T("[WARNING][%s]%s.%d�ɕR�Â��m�[�h������Ă��܂�."), 
	//									kRelInfo.alias_name,
	//									kOtherInfo.alias_name,
	//									itr.first);
	//				bool err = true;
	//				break;
	//			}
	//		}
	//	}
	//}

	// LANE -> rel��T��
	sCALLBACK->callback(_T("[INFO][%s]�O���L�[���݃`�F�b�N (��������:Lane->rel)"), kRelInfo.alias_name);
	{
		long not_found = 0;

		std::map<long, std::vector<const Row*> > dic;
		fBuildDic(kRelData, 1, dic);
		for (const auto& itr : kLaneData)
		{
			long nid = GetLong<4>(itr);
			auto f = dic.find(nid);
			if (f == dic.end())
			{
				//updateMsg(_T("[WARNING][%s]%s.%d�ɕR�t���֘A�f�[�^��rel_*.csv�ɒ�`����Ă��܂���."), kRelInfo.alias_name, kLaneInfo.alias_name, oid);
				not_found++;
			}
		}
	}
	// Other -> LANE��T��
	sCALLBACK->callback(_T("[INFO][%s]�O���L�[���݃`�F�b�N (��������:Other->Lane)"), kRelInfo.alias_name);
	{
		long not_found = 0;

		std::map<long, std::vector<const Row*> > dic;
		fBuildDic(kRelData, 2, dic);
		for (const auto& itr : kOtherData)
		{
			long oid = GetLong<kFEATURE_OID>(itr);
			auto f = dic.find(oid);
			if (f == dic.end())
			{
				sCALLBACK->callback(_T("[WARNING][%s]%s.%d�ɕR�t���֘A�f�[�^��rel_*.csv�ɒ�`����Ă��܂���."),
									kRelInfo.alias_name,
									kOtherInfo.alias_name,
									oid);
				not_found++;
				err = true;
				continue;
			}
		}
	}

	sCALLBACK->callback(_T("[INFO][%s]�O���L�[�`�F�b�N (��������:rel->Lane)"), kRelInfo.alias_name);

	std::map<long, std::vector<const Feature*> > dicLane;
	fBuildDic(kLaneData, CLaneNodeChecker::NODE_ID, dicLane);

	std::map<long, std::vector<const Feature*>> dicOther;
	fBuildDic(kOtherData, kFEATURE_OID, dicOther);

	std::set<long> rel_lane;
	std::set<long> rel_other;
	for (const auto& itr : kRelData)
	{
		const auto& row = itr.attr;
		long oid = GetLong<0>(row);
		long lane_point = GetLong<1>(row);
		long other_point = GetLong<2>(row);

		rel_lane.insert(lane_point);
		rel_other.insert(other_point);

		auto fLane = dicLane.find(lane_point);
		auto fOther = dicOther.find(other_point);

		bool notFound = false;
		if (fLane == dicLane.end())
		{
			sCALLBACK->callback(_T("[ERROR][%s.%d]�m�[�h[%s.%d]�����f�[�^���Ɍ�����܂���."), kRelInfo.alias_name, oid, kLaneInfo.alias_name, lane_point);
			err = true;
			notFound = true;
		}
		if (fOther == dicOther.end())
		{
			sCALLBACK->callback(_T("[ERROR][%s.%d]�m�[�h[%s.%d]�����f�[�^���Ɍ�����܂���."), kRelInfo.alias_name, oid, kOtherInfo.alias_name, other_point);
			err = true;
			notFound = true;
		}
		if (notFound) { continue; }

		if (!checkAttr(itr, *fLane->second[0], *fOther->second[0], kRelInfo))
			err = true;
	}
	if (kLaneData.size() != rel_lane.size())
	{
		sCALLBACK->callback(_T("[INFO][%s]%s�m�[�h��(%d)��%s�m�[�h��(%d)����v���܂���."), kRelInfo.alias_name, kRelInfo.file_name, rel_lane.size(), kLaneInfo.file_name, kLaneData.size());
		err = true;
	}
	if (kOtherData.size() != rel_other.size())
	{
		sCALLBACK->callback(_T("[INFO][%s]%s�m�[�h��(%d)��%s�m�[�h��(%d)����v���܂���."), kRelInfo.alias_name, kRelInfo.file_name, rel_other.size(), kOtherInfo.file_name, kOtherData.size());
		err = true;
	}

	return !err;
}

bool sindy::adam2::format::CLaneNodeSignalRelationChecker::checkAttr(
	const Row& row, 
	const Feature& lane_node,
	const Feature& object_point,
	const ImportFileList& filelist)
{
	// ��~�_�ȊO�͐M���@�ƕR�t���Ȃ�
	long stop_point_f = GetLong<CLaneNodeChecker::STOP_POINT_F>(lane_node);
	if (stop_point_f != 1)
	{
		long objectid  = GetLong<CLaneNodeChecker::OBJECTID>(lane_node);
		long nodeid = GetLong<CRelSignalPointLaneNodeChecker::LANE_NODE_ID>(row);
		long signalid = GetLong<CRelSignalPointLaneNodeChecker::SIGNAL_POINT_ID>(row);

		sCALLBACK->callback(_T("[WARNING][%s.%d]�M���@�ƕR�t���Ă��邪��~�_�ł͂Ȃ�[STOP_POINT_F=%d][NODE_ID=%ld][SIGNAL_POINT_ID=%ld]"),
							filelist.alias_name,
							objectid,
							stop_point_f,
							nodeid,
							signalid);

		return false;
	}

	return true;
}
