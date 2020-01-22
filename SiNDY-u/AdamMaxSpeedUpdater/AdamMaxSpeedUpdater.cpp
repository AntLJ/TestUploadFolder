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


// ���s
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
	// �������O�t�@�C���I�[�v��
	if (!RUNLOGGER.Initialize(m_args.m_logDir))
	{
		std::cerr << "�������O�t�@�C���̃I�[�v���Ɏ��s���܂��� : " << uh::toStr(m_args.m_logDir.c_str()) << std::endl;
		return false;
	}

	// �c�[�����s���o��
	RUNLOGGER.Log(_T("�ڑ���DB(���H):\t%s\n"), m_args.m_roadDB.c_str());
	RUNLOGGER.Log(_T("�ڑ���DB(ADAM):\t%s\n"), m_args.m_adamDB.c_str());
	RUNLOGGER.Log(_T("���b�V�����X�g:\t%s\n"), m_args.m_meshListStr.c_str());
	RUNLOGGER.Log(_T("���O�o�̓t�H���_�p�X:\t%s\n"), m_args.m_logDir.c_str());

	// �G���[���O�t�@�C���I�[�v��
	if (!ERRLOGGER.Initialize(m_args.m_logDir))
	{
		RUNLOGGER.Error(_T("�G���[���O�t�@�C���̃I�[�v���Ɏ��s���܂��� : ") + m_args.m_logDir + _T('\n'));
		return false;
	}

	RUNLOGGER.Info(_T("���s�J�n\n"));

	return true;
}

bool CAdamMaxSpeedUpdater::openTable()
{
	using namespace boost::assign;

	// ���HDB�ڑ�
	IWorkspacePtr road_ws = sindy::create_workspace(m_args.m_roadDB.c_str());
	if (!road_ws) {
		RUNLOGGER.Error(_T("���HDB�ւ̐ڑ��Ɏ��s���܂��� : ") + m_args.m_roadDB + _T('\n'));
		return false;
	}
	// ���H�֘A�e�[�u���I�[�v��
	if (FAILED(IFeatureWorkspacePtr(road_ws)->OpenTable(CComBSTR(road_link::kTableName), &m_roadLinkFc))) {
		RUNLOGGER.Error(_T("���H�����N�̃I�[�v���Ɏ��s���܂���\n"));
		return false;
	}
	if (FAILED(IFeatureWorkspacePtr(road_ws)->OpenTable(CComBSTR(basemesh::kTableName), &m_basemeshTable)))
	{
		m_basemeshTable = AheOpenTableByTableName(road_ws, basemesh::kTableName, TRUE);
		if (!m_basemeshTable) {
			RUNLOGGER.Error(_T("�x�[�X���b�V���̃I�[�v���Ɏ��s���܂���\n"));
			return false;
		}
	}

	// ADAMDB�ڑ�
	if (!m_workspace.open(m_args.m_adamDB.c_str())) {
		RUNLOGGER.Error(_T("ADAM DB�ւ̐ڑ��Ɏ��s���܂��� : ") + m_args.m_adamDB + _T('\n'));
		return false;
	}

	// SDE�Ȃ�Ύq�o�[�W�������쐬���ĕҏW���s�����[�h�ɂ���
	if (m_workspace.isSDE())
	{
		const std::vector<CString> edit_tables = list_of(CString(rel_road_link_lane_link::kTableName));
		if (!m_workspace.childCreateModeOn(getChildVersionName(), edit_tables))
		{
			RUNLOGGER.Error(_T("���H�����N�|�Ԑ������N�֘A�e�[�u���̓o�[�W�����Ή��ł͂���܂���\n"));
			return false;
		}
	}

	// NOTE: �I�[�v������e�[�u��������ȏ㑝������A�����_�ɂ���Ȃǂ��������邱��
	// ADAM�֘A�e�[�u���I�[�v��
	m_laneLinkFc = m_workspace.openTable(lane_link::kTableName);
	if (!m_laneLinkFc) {
		RUNLOGGER.Error(_T("�Ԑ������N�̃I�[�v���Ɏ��s���܂���\n"));
		return false;
	}
	m_relRoadLinkLaneLinkT = m_workspace.openTable(rel_road_link_lane_link::kTableName);
	if (!m_relRoadLinkLaneLinkT) {
		RUNLOGGER.Error(_T("���H�����N�|�Ԑ������N�֘A�e�[�u���̃I�[�v���Ɏ��s���܂���\n"));
		return false;
	}
	m_laneLinkGroupT = m_workspace.openTable(lane_link_group::kTableName);
	if (!m_laneLinkGroupT) {
		RUNLOGGER.Error(_T("�Ԑ������N�O���[�v�e�[�u���̃I�[�v���Ɏ��s���܂���\n"));
		return false;
	}

	// �t�B�[���h�}�b�v���擾
	m_laneLinkFM.CreateFieldMap(m_laneLinkFc->GetFields());
	m_roadLinkFM.CreateFieldMap(m_roadLinkFc->GetFields());
	m_laneLinkGroupFM.CreateFieldMap(m_laneLinkGroupT->GetFields());

	return true;
}

bool CAdamMaxSpeedUpdater::run()
{
	namespace grp_c = lane_link_group::group_code;

	// ���b�V���P�ʂŏ������s��
	// NOTE: ����ȏ��邱�Ƃ������Ȃ�����A�ʃN���X�����������邱��
	for (long meshcode : m_args.m_meshList)
	{
		// ���b�V���ׂ��̃����N�ŏd�����čX�V������Ȃ��悤�ɂ��邽�߁A���b�V���P�ʂōX�V����
		// ���d�l���Əd�����čX�V�������Ă����ɂȂ�Ȃ����A����̕s��̌��Ȃ̂ł������Ă���
		if (!startEdit())
			return false;

		// 1���b�V�����̎Ԑ������N�ǂݍ��� (�ō����x��"�ώ�" or "����km/h�i�ρj" or "�W��/�\���Ȃ�"�̃t�B�[�`���̂�)
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

		// �X�V����
		if (!updateMaxSpeed(linkage, meshcode)) {
			abortEdit();
			return false;
		}

		stopEdit();


		// �X�V��A�Ԑ������N-���H�����N�Ԃ̍ō����x���r���A��v���Ă��Ȃ���΃��O�o�͂���
		// �ړI�͖ڎ��m�F�ɂ����ƏC���ӏ��̓���Ɏg�p���邽��

		LinkagedLaneRoad linkageAfterUpdate(m_laneLinkFc, m_roadLinkFc, m_relRoadLinkLaneLinkT);
		if (!readLinkagedLaneRoad(meshcode, CString(),
								  boost::assign::list_of(grp_c::kCross)(grp_c::kToll)(grp_c::kSmartIC),
								  linkageAfterUpdate))
		{
			return false;
		}

		// ��r���ʂ��o��
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

		// �Ԑ������N�Ɠ��H�����N�̃y�A���Ȃ��ꍇ
		if (accuracy == kNoPair) {
			ERRLOGGER.Warn(lane_link, nullptr, basemesh_no, _T("�X�V���̓��H�����N���m��ł��Ȃ��������߁A�X�V�Ɏ��s���܂���"));
			continue;
		}

		// �X�V����"�������x�R�[�h(�ő�)"�̎擾�Ɏ��s�����ꍇ
		long updateCode = getUpdateMaxSpeedCode(road_link);
		if (updateCode == kUnset) {
			ERRLOGGER.Warn(lane_link, road_link, basemesh_no, _T("�X�V���̓��H�����N�̍ō����x�R�[�h���X�V�Ώۂł͂Ȃ��������߁A�X�V���s���܂���ł��� "));
			continue;
		}

		// �Ԑ������N�̎���"�������x�R�[�h(�ő�)"�ƁA�X�V����"�������x�R�[�h(�ő�)"����������΁A�X�V�X�L�b�v
		if(lane_link->GetValue(m_laneLinkFM.FindField(lane_link::kMaxLegalSpeedC)).lVal == updateCode)
			continue;

		// "�������x�R�[�h(�ő�)"���X�V
		if (FAILED(lane_link->put_Value(m_laneLinkFM.FindField(lane_link::kMaxLegalSpeedC), CComVariant(updateCode)))) {
			ERRLOGGER.Error(lane_link, road_link, basemesh_no, _T("�f�[�^�̍X�V�Ɏ��s���܂���"));
			return false;
		}
		if (FAILED(lane_link->Store())) {
			ERRLOGGER.Error(lane_link, road_link, basemesh_no, _T("�f�[�^�̍X�V�Ɏ��s���܂���"));
			return false;
		}

		// "�������x�R�[�h(�ő�)"���X�V�̑Ó����ɉ����ă��b�Z�[�W���o��
		if (accuracy == kAccurate)
			ERRLOGGER.Info(lane_link, road_link, basemesh_no, _T("�������x�R�[�h�i�ő�j���X�V���܂���"), updateCode);
		else if(accuracy == kMostlyAccurate)
			ERRLOGGER.Info(lane_link, road_link, basemesh_no, _T("�������x�R�[�h�i�ő�j���X�V���܂��� (�����̓��H�����N�ƕR�t������) "), updateCode);
		else if (accuracy == kInaccurate)
			ERRLOGGER.Warn(lane_link, road_link, basemesh_no, _T("�������x�R�[�h�i�ő�j���X�V���܂������s���m�ȉ\��������܂�"), updateCode);
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
				ERRLOGGER.Info(record.m_laneLink, road, basemesh_no, _T("�Ԑ������N�Ɠ��H�����N�̍ō����x����v���Ă��܂���"));
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
		RUNLOGGER.Error(_T("�Ԑ������N�̓ǂݍ��݂Ɏ��s���܂��� : ") + basemesh_no + _T('\n'));
		return false;
	}

	auto laneLinks = laneLinkReader.getFeatures();
	if (!excludeByGroupCode(exclude_group_code, laneLinks)) {
		RUNLOGGER.Error(_T("�Ԑ������N�O���[�v�̓ǂݍ��݂Ɏ��s���܂��� : ") + basemesh_no + _T('\n'));
		return false;
	}

	if (!linkaged_links.read(laneLinks)) {
		RUNLOGGER.Error(_T("�Ԑ������N�ɕR�Â����H�����N�̓ǂݍ��݂Ɏ��s���܂��� : ") + basemesh_no + _T('\n'));
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
	// ���O�Ώۂ̃O���[�vID���擾
	std::set<long> excludeGroupIds;
	{
		// ��ʂ�1,000�ȏ�w�肷�邱�Ƃ͍l�����Ȃ��߁AIN�吧���͍l�����Ȃ�
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

	// ���O�Ώۂ̃O���[�vID�����Ԑ������N���폜
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

	 // ��L�ȊO�̐������x�R�[�h(�ώ���)�͔�r�o���Ȃ��̂� true
	 return true;
 }

 bool CAdamMaxSpeedUpdater::startEdit()
{
	if (!m_workspace.startEditing()) {
		RUNLOGGER.Error(_T("�ҏW�J�n�Ɏ��s���܂���\n"));
		return false;
	}
	if (!m_workspace.startEditOperation()) {
		m_workspace.stopEditing(false);
		RUNLOGGER.Error(_T("�ҏW�J�n�Ɏ��s���܂���\n"));
		return false;
	}
	return true;
}

void CAdamMaxSpeedUpdater::stopEdit()
{
	if (!m_workspace.stopEditOperation()) {
		m_workspace.stopEditing(false);
		RUNLOGGER.Error(_T("�ҏW�I���Ɏ��s���܂���\n"));
		return;
	}
	if (!m_workspace.stopEditing(true)) {
		RUNLOGGER.Error(_T("�ҏW�I���Ɏ��s���܂���\n"));
	}
}

void CAdamMaxSpeedUpdater::abortEdit()
{
	RUNLOGGER.Error(_T("�ҏW�Ɏ��s���܂���\n"));
	if (!m_workspace.abortEditOperation()) {
		m_workspace.stopEditing(false);
		RUNLOGGER.Error(_T("�ҏW�I���Ɏ��s���܂���\n"));
		return;
	}
	if (!m_workspace.stopEditing(false)) {
		RUNLOGGER.Error(_T("�ҏW�I���Ɏ��s���܂���\n"));
	}
}

void CAdamMaxSpeedUpdater::finish(bool isSucceeded) const
{
	// �I�����b�Z�[�W�o��
	uh::tstring result = isSucceeded ? _T("����I��") : _T("�ُ�I��");
	RUNLOGGER.Info(_T("��������: ") + result);

	// �t�@�C���̃N���[�Y
	RUNLOGGER.Finalize();
	ERRLOGGER.Finalize();
}

CString CAdamMaxSpeedUpdater::getChildVersionName() const
{
	static const CString prefix = _T("ETC_ADAMEDIT_");

	// ���݂̓�����"YYYYMMDDHHDDSS" �Ŏ擾
	CTime cTime = CTime::GetCurrentTime();
	const CString timeString = cTime.Format(_T("%Y%m%d%H%M%S"));

	return prefix + timeString;
}
