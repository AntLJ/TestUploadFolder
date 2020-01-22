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
#include "AdamSimpleChecker.h"

using namespace sindy::adam2::format;

bool CSimpleLaneLinkChecker::sub(const ImportFileList & kInfo, const std::vector<Feature>& kData)
{
	int err = 0;
	for (const auto& itr : kData)
	{
		long objectid = GetLong<CLaneLinkChecker::OBJECTID>(itr);
		long from_node = GetLong<CLaneLinkChecker::FROM_NODE_ID>(itr);
		long to_node = GetLong<CLaneLinkChecker::TO_NODE_ID>(itr);
		long group_id = GetLong<CLaneLinkChecker::LANE_GROUP_ID>(itr);
		long group_c = GetLong<CLaneLinkChecker::LANE_GROUP_C>(itr);
		long lane_no = GetLong<CLaneLinkChecker::LANE_NO>(itr);
		long lane_count = GetLong<CLaneLinkChecker::LANE_COUNT>(itr);
		long max_speed = GetLong<CLaneLinkChecker::MAX_LEGAL_SPEED_C>(itr);
		long min_speed = GetLong<CLaneLinkChecker::MIN_LEGAL_SPEED_C>(itr);
		long direction_c = GetLong<CLaneLinkChecker::TRAVEL_DIRECTION_C>(itr);
		long structure_c = GetLong<CLaneLinkChecker::ROAD_STRUCTURE_C>(itr);
		long left_change = GetLong<CLaneLinkChecker::LEFT_CHANGE_F>(itr);
		long right_change = GetLong<CLaneLinkChecker::RIGHT_CHANGE_F>(itr);
		long rubbing = GetLong<CLaneLinkChecker::RUBBING_F>(itr);

		bool fok = true;
		if (!nomentenance(itr, { 6, 7, 11, 12, 14, 17, 18 }))
		{
			sCALLBACK->callback(_T("[ERROR][%s.%d]�����ΏۊO�̃t�B�[���h�ɒl���i�[����Ă��܂�."), kInfo.alias_name, objectid);
			fok = false;
		}
		if (lane_no > lane_count)
		{
			sCALLBACK->callback(_T("[ERROR][%s.%d]�Ԑ��ԍ����Ԑ��������傫���l�ł�. [No=%d, Count=%d]"), kInfo.alias_name, objectid, lane_no, lane_count);
			fok = false;
		}
		if (min_speed < 999 && max_speed < 999)
		{
			if (min_speed > max_speed)
			{
				sCALLBACK->callback(_T("[ERROR][%s.%d]�Œᑬ�x���ō����x�������Ă��܂�. [MIN=%d, MAX=%d]"), kInfo.alias_name, objectid, min_speed, max_speed);
				fok = false;
			}
		}
		if (structure_c == 6)
		{
			if (!(lane_no == 0 && lane_count == 0))
			{
				sCALLBACK->callback(_T("[ERROR][%s.%d]���Ԑ���ԂɎԐ��ԍ��܂��͎Ԑ�����񂪊i�[����Ă��܂�. [No=%d, Count=%d]"), kInfo.alias_name, objectid, lane_no, lane_count);
				fok = false;
			}
		}
		if (left_change && lane_no == 1)
		{
			sCALLBACK->callback(_T("[ERROR][%s.%d]�ō����[���ł�. ����ȏ㍶�ɂ͎Ԑ��ύX�ł��܂���."), kInfo.alias_name, objectid);
			fok = false;
		}
		if (right_change && lane_no == lane_count)
		{
			sCALLBACK->callback(_T("[ERROR][%s.%d]�ŉE���[���ł�. ����ȏ㍶�ɂ͎Ԑ��ύX�ł��܂���."), kInfo.alias_name, objectid);
			fok = false;
		}
		if (rubbing && structure_c != 2 && structure_c != 5)
		{
			sCALLBACK->callback(_T("[WARNING][%s.%d]�C��t���t���O�͕��򍇗��Ԑ��݂̂ɕt�^�\�ł�.[STRUCT=%d]"), kInfo.alias_name, objectid, structure_c);
			fok = false;
		}
		if ((group_id != 0 && group_c == 0) || (group_id == 0 && group_c != 0))
		{
			sCALLBACK->callback(_T("[WARNING][%s.%d]�O���[�vID�Ǝ�ʐݒ�̐����������܂���. [GID=%d, GROUP_C=%d]"), kInfo.alias_name, objectid, group_id, group_c);
			fok = false;
		}

		if (!fok) { err++; }
	}

	// ����O���[�v���ł̐������m�F
	sCALLBACK->callback(_T("[INFO][%s]�O���[�v�������`�F�b�N"), kInfo.alias_name);
	std::map<long, std::vector<const Feature*> > dicGroup;
	fBuildDic(kData, CLaneLinkChecker::LANE_GROUP_ID, dicGroup);
	dicGroup.erase(0);

	for (const auto& itr : dicGroup)
	{
		if (itr.second.size() == 1) { continue; }

		std::set<long> STRUCT_C;
		std::set<long> LANE_NO;
		std::set<long> LANE_COUNT;
		bool fok = true;
		for (const auto& f : itr.second)
		{
			long objectid = GetLong<CLaneLinkChecker::OBJECTID>(*f);
			long structure_c = GetLong<CLaneLinkChecker::ROAD_STRUCTURE_C>(*f);
			long lane_no = GetLong<CLaneLinkChecker::LANE_NO>(*f);
			long lane_count = GetLong<CLaneLinkChecker::LANE_COUNT>(*f);

			auto fStr = STRUCT_C.insert(structure_c).second;
			auto fNo = LANE_NO.insert(lane_no).second;
			auto fCnt = LANE_COUNT.insert(lane_count).second;

			if (!fNo) {
				sCALLBACK->callback(_T("[ERROR][%s.%d]����O���[�v���ɎԐ��ԍ��̏d��������܂�.[GROUP=%d, LANE_NO=%d]"), kInfo.alias_name, objectid, itr.first, lane_no);
				fok = false;
			}
		if (LANE_COUNT.size() != 1) { 
		}
			sCALLBACK->callback(_T("[ERROR][%s]�O���[�v���ɕ����̎Ԑ��������݂��Ă��܂�. [GROUP_ID=%d, LANE_COUNT=%s]"), 
								kInfo.alias_name,
								itr.first,
								CString(concat(LANE_COUNT).c_str())); 
			fok = false;
		}
		if (STRUCT_C.size() != 1) {
			sCALLBACK->callback(_T("[WARNING][%s]�O���[�v���ɕ����̓��H�\����ʃR�[�h�����݂��Ă��܂�.[GROUP_ID=%d]"),
								kInfo.alias_name, 
								itr.first);
			fok = false;
		}
		if (!fok) { err++; }
	}

	return (err == 0);
}

bool CSimpleLanePointChecker::sub(const ImportFileList & kInfo, const std::vector<Feature>& kData)
{
	int err = 0;
	for (const auto& itr : kData)
	{
		long objectid = GetLong<CLaneNodeChecker::OBJECTID>(itr);
		long class_c = GetLong<CLaneNodeChecker::POINT_CLASS_C>(itr);
		long sequence = GetLong<CLaneNodeChecker::SEQUENCE>(itr);
		long lastnode_f = GetLong<CLaneNodeChecker::LAST_NODE_F>(itr);
		long stop_point_f = GetLong<CLaneNodeChecker::STOP_POINT_F>(itr);

		double width = GetDouble<CLaneNodeChecker::WIDTH>(itr);
		long   linear = GetLong  <CLaneNodeChecker::LINEAR_ELEMENT_C>(itr);
		double curve = GetDouble<CLaneNodeChecker::CURVATURE>(itr);
		double radius = GetDouble<CLaneNodeChecker::CURVATURE_RADIUS>(itr);
		double rate = GetDouble<CLaneNodeChecker::CURVATURE_CHANGE_RATE>(itr);
		double clothoid = GetDouble<CLaneNodeChecker::CLOTHOID_PARAMETER>(itr);

		bool fok = true;
		if (!nomentenance(itr, { 11, 12, 13, 14, 15 }))
		{
			sCALLBACK->callback(_T("[ERROR][%s.%d]�����ΏۊO�̃t�B�[���h�ɒl���i�[����Ă��܂�."), kInfo.alias_name, objectid);
			fok = false;
		}
		if (class_c == CNodeChecker::POINT_NODE)
		{
			if (!(sequence == 0 || lastnode_f))
			{
				sCALLBACK->callback(_T("[ERROR][%s.%d]�m�[�h�|�C���g����[/�I�[�ł͂���܂���."), kInfo.alias_name, objectid);
				fok = false;
			}
		}
		else
		{
			if (sequence == 0 || lastnode_f)
			{
				sCALLBACK->callback(_T("[ERROR][%s.%d]��ԓ_�|�C���g����[�܂��͏I�[�ł�."), kInfo.alias_name, objectid);
				fok = false;
			}
			if (stop_point_f)
			{
				sCALLBACK->callback(_T("[ERROR][%s.%d]��ԓ_�|�C���g�ɒ�~���t���O���t�^����Ă��܂�."), kInfo.alias_name, objectid);
				fok = false;
			}
			if (!(2.5 <= width && width < 10.0))
			{
				sCALLBACK->callback(_T("[WARNING][%s.%d]�����l���������Ȃ��\��������܂�. [WIDTH=%.3lf]"), kInfo.alias_name, objectid, width);
				fok = false;
			}
		}

		// Note: ���`�v�f�����A�ȗ��֘A�͐����ΏۊO�̂��߃`�F�b�N���珜�O

		if (!fok) { err++; }
	}

	return (err == 0);
}

bool sindy::adam2::format::CSimpleCompartLinkChecker::sub(const ImportFileList & kInfo, const std::vector<Feature>& kData)
{
	int err = 0;

	for (const auto& itr : kData)
	{
		long objectid = GetLong<CCompartLinkChecker::OBJECTID>(itr);
		long line_c = GetLong<CCompartLinkChecker::COMPART_LINE_C>(itr);
		long style_c = GetLong<CCompartLinkChecker::COMPART_STYLE_C>(itr);
		long slowdown_c = GetLong<CCompartLinkChecker::SLOWDOWN_LABEL_C>(itr);

		bool fok = true;
		if (!nomentenance(itr, { CCompartLinkChecker::INSTALLATION_F })){
			sCALLBACK->callback(_T("[ERROR][%s.%d]�����ΏۊO�̃t�B�[���h�ɒl���i�[����Ă��܂�."), kInfo.alias_name, objectid);
			fok = false;
		}

		if (line_c == 0) {
			if (!itr.attr[CCompartLinkChecker::COMPART_STYLE_C].empty()) {
				sCALLBACK->callback(_T("[ERROR][%s.%d]���z���̋����ݒu�l���R�[�h��<null>�ł͂���܂���."), kInfo.alias_name, objectid);
				fok = false;
			}
			if (!itr.attr[CCompartLinkChecker::SLOWDOWN_LABEL_C].empty()) {
				sCALLBACK->callback(_T("[ERROR][%s.%d]���z���̋��������\���R�[�h��<null>�ł͂���܂���."), kInfo.alias_name, objectid);
				fok = false;
			}
		}

		if (!fok) { err++; }
	}

	return (err == 0);
}

bool sindy::adam2::format::CSimpleCompartPointChecker::sub(const ImportFileList & kInfo, const std::vector<Feature>& kData)
{
	int err = 0;

	for (const auto& itr : kData)
	{
		long objectid = GetLong<CCompartNodeChecker::OBJECTID>(itr);
		long class_c = GetLong<CCompartNodeChecker::POINT_CLASS_C>(itr);
		long lastnode_f = GetLong<CCompartNodeChecker::LAST_NODE_F>(itr);

		double width = GetDouble<CCompartNodeChecker::WIDTH>(itr);

		bool fok = true;
		if (!nomentenance(itr, { CCompartNodeChecker::WIDTH }))
		{
			sCALLBACK->callback(_T("[ERROR][%s.%d]�����ΏۊO�̃t�B�[���h�ɒl���i�[����Ă��܂�."), kInfo.alias_name, objectid);
			fok = false;
		}

		if (lastnode_f && class_c != 1) {
			sCALLBACK->callback(_T("[ERROR][%s.%d]�\���_�|�C���g�ɏI�[�t���O���t�^����Ă��܂�."), kInfo.alias_name, objectid);
			fok = false;
		}
		if (!fok) { err++; }
	}

	return (err == 0);
}

bool sindy::adam2::format::CSimpleBorderLinkChecker::sub(const ImportFileList & kInfo, const std::vector<Feature>& kData)
{
	return true;
}

bool sindy::adam2::format::CSimpleBorderPointChecker::sub(const ImportFileList & kInfo, const std::vector<Feature>& kData)
{
	int err = 0;

	for (const auto& itr : kData)
	{
		long objectid = GetLong<CBorderNodeChecker::OBJECTID>(itr);
		long class_c = GetLong<CBorderNodeChecker::POINT_CLASS_C>(itr);
		long lastnode_f = GetLong<CBorderNodeChecker::LAST_NODE_F>(itr);

		bool fok = true;
		if (lastnode_f && class_c != 1) {
			sCALLBACK->callback(_T("[ERROR][%s.%d]�\���_�|�C���g�ɏI�[�t���O���t�^����Ă��܂�."), kInfo.alias_name, objectid);
			fok = false;
		}
		if (!fok) { err++; }
	}

	return (err == 0);
}

bool sindy::adam2::format::CSimpleSignalPointChecker::sub(const ImportFileList & kInfo, const std::vector<Feature>& kData)
{
	int err = 0;

	for (const auto& itr : kData)
	{
		long objectid = GetLong<CSignalPointChecker::OBJECTID>(itr);
		long lamp_num = GetLong<CSignalPointChecker::LAMP_COUNT>(itr);
		long arrow_f = GetLong<CSignalPointChecker::ARROW_F>(itr);

		bool fok = true;
		if (!nomentenance(itr, { CSignalPointChecker::LAMP_COUNT, CSignalPointChecker::INDEPENDENT_F })) {
			sCALLBACK->callback(_T("[ERROR][%s.%d]�����ΏۊO�̃t�B�[���h�ɒl���i�[����Ă��܂�."), kInfo.alias_name, objectid);
			fok = false;
		}

		if (arrow_f && lamp_num <= 3) {
			sCALLBACK->callback(_T("[WARNING][%s.%d]���t�M���̃����v�������Ȃ���������܂���.[LAMP=%d]"), kInfo.alias_name, objectid, lamp_num);
			fok = false;
		}

		if (!fok) { err++; }
	}

	return (err == 0);
}


bool sindy::adam2::format::CSimpleRoadsignPointChecker::sub(const ImportFileList & kInfo, const std::vector<Feature>& kData)
{
	return true;
}
