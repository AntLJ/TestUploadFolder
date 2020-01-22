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
#include "MatchingProcessor.h"

using namespace sindy::schema::sj;
using namespace common_proc;

MatchingProcessor::MatchingProcessor(
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
	)
	: m_srcPriListPrior(srcPriListPrior), m_srcPriListMatch(srcPriListMatch), m_poiPriList(poiPriList), m_supPoiPriList(supPoiPriList),
	m_replaceList(replaceList), m_condList(condList), m_bldSrcIndeces(bldSrcIndeces), m_poiPtOrgIndeces(poiPtOrgIndeces),
	m_mapAddrCodeCounter(mapAddrCodeCounter), m_isSuvey(isSurvey), m_surveyPtMgr( surveyPtMgr )
{
}

MatchingProcessor::~MatchingProcessor(void)
{
}

bool MatchingProcessor::MatchingProc(
	IDFeatureMap& tgtSrcPtFeatures,
	const IDFeatureMap& tgtPoiPtFeatures,
	long bldID,
	IPolygonPtr ipFilledBldPolygon,
	BldNamePtInfo *bldNamePtInfo,
	std::set<long> *relIDs,
	bool *result,
	bool *excepted,
	bool *selected
	)
{
	*result = false;
	*excepted = false;
	*selected = false;

	// �Ώۑf�ރ|�C���g�����݂���ꍇ�ɑf�ރ|�C���g�𗘗p�������������{
	// �i�P���Ȃ��ꍇ�ł��⊮�p���LPOI�ɂ��⊮�͎��{����j
	if (tgtSrcPtFeatures.size() > 0)
	{
		// �u���O�����v�u�Ǝ������v�̑f�ރ|�C���g�����݂��邩�ǂ����`�F�b�N
		if (! CheckSpecialSrcPt(tgtSrcPtFeatures, bldID, bldNamePtInfo, result, excepted, selected))
			return false;
		if (*result || *excepted) {
			return true;
		}

		// �}�b�`���O�Ώۑf�ރ|�C���g�Q����u�Ǝ������v�̂��̂��폜
		DeleteOwnSurveyPts(tgtSrcPtFeatures);

		if (tgtSrcPtFeatures.size() > 0)
		{
			// ���LPOI�i�f�ރ|�C���g�j�����݂��邩�`�F�b�N���A���݂����炱���o�^
			if (! GetPoiSrcPtID(tgtSrcPtFeatures, bldID, bldNamePtInfo, result))
				return false;
			if (*result) {
				// POI�����݂������_�ő�\�_�Arel_buildingname�ɓo�^�����\�_ID�Q��o�^���ďI��
				long headID = bldNamePtInfo->e_srcOID;
				SetRelIDs(tgtSrcPtFeatures, headID, relIDs);
				return true;
			}

			// ���̑��̑f�ރ|�C���g�ɂ��}�b�`���O
			long headID = -1;
			if (1 == tgtSrcPtFeatures.size()) {
				// �ƌ`���1�|�C���g�������݂��Ȃ��ꍇ�̏���
				IDFeatureMap::const_iterator srcPtItr = tgtSrcPtFeatures.begin();
				if (! MatchingOnePoint(srcPtItr->first, srcPtItr->second.m_T, bldID, ipFilledBldPolygon, false, bldNamePtInfo, result))
					return false;
				// 1�|�C���g�������݂��Ȃ��ꍇ�́Arel_buildingname�ɂ͉����o�^���Ȃ�
			} else {
				// ������}�b�`���O���s��
				if (! MatchingPoints(tgtSrcPtFeatures, bldID, bldNamePtInfo, result))
					return false;
				if (*result) {
					headID = bldNamePtInfo->e_srcOID;
				} else {
					// ������ʂ̃|�C���g���������݂���͂��Ȃ̂ŁA�e�|�C���g�̐M�������`�F�b�N����B
					std::map<long, BldNamePtInfo> sameSrcBldNamPtInfos;
					for (auto& tgtSrcPt : tgtSrcPtFeatures) {
						BldNamePtInfo tmpInfo;
						bool priorityChk = false;
						if (! MatchingOnePoint(tgtSrcPt.first, tgtSrcPt.second, bldID, ipFilledBldPolygon, true, &tmpInfo, &priorityChk))
							return false;
						if (priorityChk)
							sameSrcBldNamPtInfos[tgtSrcPt.first] = tmpInfo;
					}
					if (sameSrcBldNamPtInfos.size() > 0) {
						headID = SelectSameSrcBldNamePtInfo(sameSrcBldNamPtInfos);
						*bldNamePtInfo = sameSrcBldNamPtInfos.find(headID)->second;
						*result = true;
					}
				}
			}

			if (*result) {
				// �̗p�����L�q���X�g�ɂ��t�B���^�����O
				// �̗p�����L�q���X�g�ɂȂ��M���x��ʃR�[�h�̏ꍇ�A�������̓}�b�`���O�����w�肳�ꂽ臒l�����̏ꍇ�́A�������̃|�C���g���R�[�h�������Z�b�g
				std::map<long, long>::const_iterator condItr = m_condList.find(bldNamePtInfo->e_priority);
				if (condItr == m_condList.end() || (bldNamePtInfo->e_matchPer != -1 && bldNamePtInfo->e_matchPer < condItr->second)) {
					bldNamePtInfo->Reset();
					*result = false;
				} else if (headID > 0) {
					SetRelIDs(tgtSrcPtFeatures, headID, relIDs);
				}
			}
		}
	}

	// �f�ރ|�C���g�ł͍̗p�|�C���g�����܂�Ȃ������ꍇ�A�⊮�p���LPOI�ɂ��⊮�����{
	if (! *result) {
		if (! SupplyWithPoiPtOrg(tgtPoiPtFeatures, bldID, bldNamePtInfo, result))
			return false;
	}

	// �����Ώۃ|�C���g���X�g�o�̓��[�h�ŏ�����\�_���擾�ł��Ȃ������ꍇ�́A�����Ώۃ|�C���g���o�^
	if (tgtSrcPtFeatures.size() > 0 && (! *result) && m_isSuvey && (! *selected))
	{
		long tgtSrcID = -1;
		IFeaturePtr ipTgtSrcFeature;
		if (! ::common_proc::SelectHeadNameFeature(tgtSrcPtFeatures, ::buildingname_src_point::kTableName, ::buildingname_src_point::kName, m_bldSrcIndeces, &tgtSrcID, ipTgtSrcFeature))
			return false;
		if(! m_surveyPtMgr.addFromBldgNameSrc( tgtSrcID, bldID, ipTgtSrcFeature ) )
			return false;
		*selected = true;
	}

	return true;
}

bool MatchingProcessor::CheckSpecialSrcPt(const IDFeatureMap& tgtSrcPtFeatures, long bldID, BldNamePtInfo *bldNamePtInfo, bool *result, bool *excepted, bool *selected)
{
	PriorFeatureMap priorFeatureMap;	// �D��x����}�b�v
	for (IDFeatureMap::const_iterator itrSrcPt = tgtSrcPtFeatures.begin(); itrSrcPt != tgtSrcPtFeatures.end(); ++itrSrcPt)
	{
		// �o�T��ʃR�[�h�擾
		CComVariant srcType;
		if (! ::GetValue((_IRowPtr)itrSrcPt->second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, &srcType))
			return false;
		// �o�T��ʃR�[�h���u���O�����v�̏ꍇ
		if (::buildingname_src_point::source_type::kExclusion == srcType.lVal)
		{
			LogSys::GetInstance().WriteFreeStyleLog(false, err_level::notice, _T("�u���O�����v�|�C���g�����݂��܂�"), sindy::schema::building::kTableName, bldID);
			*excepted = true;
			return true;	// ��������
		}
		// �̗p��ʃR�[�h�擾
		CComVariant adptCode;
		if (! ::GetValue((_IRowPtr)itrSrcPt->second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAdoption, m_bldSrcIndeces, &adptCode))
			return false;
		// �̗p��ʃR�[�h�̃`�F�b�N�i�u�Ǝ������v�|�C���g�̏ꍇ�j
		if (::buildingname_src_point::source_type::kOwnSurvey == srcType.lVal)
		{
			switch(adptCode.lVal)
			{
			case ::buildingname_src_point::adoption_code::kAdopt:			// �̗p
			case ::buildingname_src_point::adoption_code::kRejectClosed:	// �����ɂ��s�̗p�i���E���Ɓj
			case ::buildingname_src_point::adoption_code::kRejectUnkown:	// �����ɂ��s�̗p�i�s���j
				break;
			default:
				LogSys::GetInstance().WriteFreeStyleLog(false, err_level::err, _T("�s���ȁu�Ǝ������v�|�C���g�����݂��܂�"), ::buildingname_src_point::kTableName, itrSrcPt->first);
				continue;
			}
		}
		// ���̎擾
		CComVariant name;
		if (! ::GetValue((_IRowPtr)itrSrcPt->second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kName, m_bldSrcIndeces, &name))
			return false;
		// �u�D�旘�p�v��ʂ̏ꍇ�A�D��x����}�b�v�Ɋi�[
		if (std::find(m_srcPriListPrior.begin(), m_srcPriListPrior.end(), srcType.lVal) != m_srcPriListPrior.end())
		{
			SrcPriorKey srcPriorKey(m_srcPriListPrior, srcType.lVal, adptCode.lVal, CString(name.bstrVal).GetLength(), itrSrcPt->first);
			priorFeatureMap.insert(PriorFeatureMap::value_type(srcPriorKey, itrSrcPt->second.m_T));
		}
	}

	// �u�D�旘�p�v��ʂ̑f�ރ|�C���g���Ȃ��ꍇ�͔�����
	if (0 == priorFeatureMap.size()) return true;

	// �D��x1�ʂ̃��R�[�h�ɂ��ď���
	PriorFeatureMap::const_iterator itr = priorFeatureMap.begin();
	if (::buildingname_src_point::source_type::kOwnSurvey == itr->first.e_srcType && ::buildingname_src_point::adoption_code::kRejectClosed == itr->first.e_adptCode)
	{
		// �u�����ɂ��s�̗p�i���E���Ɓj�v�̏ꍇ
		// �u���O�����v�Ɠ����̈���
		LogSys::GetInstance().WriteFreeStyleLog(false, err_level::notice, _T("�u31: �����ɂ��s�̗p�i���E���Ɓj�v�ɂ�菜�O"), sindy::schema::building::kTableName, bldID);
		*excepted = true;
	}
	else if (::buildingname_src_point::source_type::kOwnSurvey == itr->first.e_srcType && ::buildingname_src_point::adoption_code::kRejectUnkown == itr->first.e_adptCode)
	{
		// �u�����ɂ��s�̗p�i�s���j�v
		// �����Ώۃ|�C���g������}��
		*selected = true;
	}
	else
	{
		// ���̑� �� �D��I�Ƀx�[�XDB�ɓo�^
		IFeaturePtr ipPrior = itr->second;
		// NAME�擾
		CComVariant name;
		if (! ::GetValue((_IRowPtr)ipPrior, ::buildingname_src_point::kTableName, ::buildingname_src_point::kName, m_bldSrcIndeces, &name))
			return false;
		// SOURCENAME�擾
		CComVariant srcName;
		if (! ::GetValue((_IRowPtr)ipPrior, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceName, m_bldSrcIndeces, &srcName))
			return false;
		// SOURCEID�擾
		CComVariant srcID;
		if (! ::GetValue((_IRowPtr)ipPrior, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceID, m_bldSrcIndeces, &srcID))
			return false;
		// ANYPLACE_ID�擾
		CComVariant anyplaceID;
		if (! ::GetValue((_IRowPtr)ipPrior, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAnyPlaceID, m_bldSrcIndeces, &anyplaceID))
			return false;
		// �`��擾
		IGeometryPtr ipGeometry;
		if (FAILED(ipPrior->get_ShapeCopy(&ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��擾�Ɏ��s���܂���"), ::buildingname_src_point::kTableName, itr->first.e_objectID);
			return false;
		}
		bldNamePtInfo->e_name		= (VT_BSTR == name.vt) ? (CString)name : _T("");
		bldNamePtInfo->e_bldOID		= bldID;
		bldNamePtInfo->e_srcType	= itr->first.e_srcType;
		bldNamePtInfo->e_srcName	= (VT_BSTR == srcName.vt) ? (CString)srcName : _T("");
		bldNamePtInfo->e_srcID		= (VT_BSTR == srcID.vt) ? (CString)srcID : _T("");
		bldNamePtInfo->e_srcOID		= itr->first.e_objectID;
		bldNamePtInfo->e_anyPlaceID	= anyplaceID.bstrVal;
		bldNamePtInfo->e_priority	= ::buildingname_point::priority_code::kEdit;
		bldNamePtInfo->e_matchPer	= -1;
		bldNamePtInfo->e_ipGeometry	= ipGeometry;
		*result = true;
	}

	// �c��̃��R�[�h�ɂ��č̗p����Ȃ��|���G���[���O�ɏo��
	++itr;
	for (; itr != priorFeatureMap.end(); ++itr)
	{
		CString errMsg;
		errMsg.Format(_T("�u�D�旘�p�v�̃|�C���g�����ꌚ�����ɕ������݂��邽�ߕs�̗p\t%d"),  itr->first.e_srcType);
		LogSys::GetInstance().WriteFreeStyleLog(false, err_level::notice, errMsg, ::buildingname_src_point::kTableName, itr->first.e_objectID);
	}

	return true;
}

void MatchingProcessor::DeleteOwnSurveyPts(IDFeatureMap& tgtSrcPtFeatures)
{
	// ��U�폜�Ώ�ID�i�L�[�j���擾����
	std::set<long> deleteIDs;
	for (auto rec : tgtSrcPtFeatures)
	{
		// �o�T��ʃR�[�h�擾
		CComVariant srcType;
		if (! ::GetValue((_IRowPtr)rec.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, &srcType))
			continue;
		// �u�Ǝ������v�̏ꍇ�͍폜�Ώ�ID�Ƃ��Ēǉ�
		if (buildingname_src_point::source_type::kOwnSurvey == srcType.lVal)
			deleteIDs.insert( rec.first );
	}
	// �폜�Ώ�ID�ɊY��������̂��폜
	for (auto delID : deleteIDs)
	{
		tgtSrcPtFeatures.erase(delID);
	}
}

bool MatchingProcessor::GetPoiSrcPtID(const IDFeatureMap& tgtSrcPtFeatures, long bldID, BldNamePtInfo* bldNamePtInfo, bool* existPOI)
{
	*existPOI = false;
	// �o�T�R�[�h��POI�ł���f�ނ�CONTENTS_CODE(BUILDING1_DIV1)�A����(NAME)�̒����Eobjectid���擾����B
	std::map<CString, std::set<POIINFO>> poiInfos; // �L�[:CONTENTS_CODE �v�f ���̂̒���&objectid&����(���̂̒������Z�����Ɋi�[�E���̂̒����������̏ꍇ��objectid��)
	for (const auto& tgtSrcPt : tgtSrcPtFeatures) {
		// �o�T�R�[�h�擾
		CComVariant srcType;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, &srcType))
			return false;
		if (::buildingname_src_point::source_type::kAnnoPOI != srcType.lVal)
			continue;		// ���LPOI�ȊO�͔�΂�
		// CONTENTS_CODE�擾
		CComVariant contentsCode;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kBuilding1Div1, m_bldSrcIndeces, &contentsCode))
			return false;
		// CONTENTS_CODE��NULL�̏ꍇ�A�G���[���o�͂���B
		if ( contentsCode.vt == VT_NULL || contentsCode.vt == VT_EMPTY) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("���LPOI�f�ނɃR���e���c�R�[�h�����݂��܂���"), ::buildingname_src_point::kTableName, tgtSrcPt.first);
			return false;
		}
		// NAME�擾
		CComVariant name;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kName, m_bldSrcIndeces, &name))
			return false;
		// SOURCEID�擾
		CComVariant srcID;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceID, m_bldSrcIndeces, &srcID))
			return false;
		// �`��擾
		IGeometryPtr ipGeometry;
		if (FAILED(tgtSrcPt.second.m_T->get_ShapeCopy(&ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��擾�Ɏ��s���܂���"), ::buildingname_src_point::kTableName, tgtSrcPt.first);
			return false;
		}
		// POI���̃L�[�Z�b�g
		POIINFO poiInfo;
		poiInfo.e_nameLength	= (VT_BSTR == name.vt) ? ((CString)name).GetLength() : 0;
		poiInfo.e_objectid		= tgtSrcPt.first;
		poiInfo.e_name			= (VT_BSTR == name.vt) ? (CString)name : _T("");
		poiInfo.e_srcID			= (VT_BSTR == srcID.vt) ? (CString)srcID : _T("");
		poiInfo.e_ipGeometry	= ipGeometry;
		CString strContentsCode	= (VT_BSTR == contentsCode.vt) ? (CString)contentsCode : _T("");
		poiInfos[strContentsCode].insert(poiInfo);
	}
	
	// �ΏۂƂȂ钍�LPOI�|�C���g�����݂��Ȃ��ꍇ�͏I��
	if (0 == poiInfos.size())
		return true;

	// �������݂���̂ŁA�D��x�`�F�b�N
	for (const auto& poiCode : m_poiPriList) {
		// m_poiPriList�͗D��x���Ɋi�[����Ă���̂ŁA�����莟��I��
		std::map<CString, std::set<POIINFO>>::const_iterator tgtSrcItr = poiInfos.find(poiCode);
		if (tgtSrcItr != poiInfos.end()) {
			// std::set<POIINFO>�͕�����̒Z�����i������̒������ꏏ�̏ꍇ��objectid���j�Ŋi�[����Ă���̂�1�ڂ̗v�f���Ώ�
			std::set<POIINFO>::const_iterator poiInfoItr = tgtSrcItr->second.begin();
			bldNamePtInfo->e_name		= poiInfoItr->e_name;
			bldNamePtInfo->e_bldOID		= bldID;
			bldNamePtInfo->e_srcType	= ::buildingname_point::source_type::kAnnoPOI;
			bldNamePtInfo->e_srcID		= poiInfoItr->e_srcID;
			bldNamePtInfo->e_srcOID		= poiInfoItr->e_objectid;
			bldNamePtInfo->e_priority	= ::buildingname_point::priority_code::kAnnoPoi;
			bldNamePtInfo->e_matchPer	= -1;
			bldNamePtInfo->e_ipGeometry	= poiInfoItr->e_ipGeometry.m_T;
			*existPOI = true;
			// �c���objectid�����O�ɏo�͂���B
			++poiInfoItr;
			for (; poiInfoItr != tgtSrcItr->second.end(); ++poiInfoItr) {
				LogSys::GetInstance().WriteFreeStyleLog(false, err_level::notice, _T("����POI�W���������������݂����ׁA������̒��� or objectid���őΏۊO�ɂȂ�܂���"), ::buildingname_src_point::kTableName, poiInfoItr->e_objectid);
			}
			break;
		}
	}
	if (! *existPOI) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("������ɒ��LPOI�D��x��ʃ��X�g�ɑ��݂��Ȃ����LPOI�f�ނ����݂��܂�"), sindy::schema::building::kTableName, bldID);
		return false;
	} 

	return true;
}

bool MatchingProcessor::MatchingOnePoint(
	long tgtSrcID,
	const IFeaturePtr& ipTgtSrcFeature,
	long bldID,
	IPolygonPtr ipFilledBldPolygon,
	bool isPart,
	BldNamePtInfo* bldNamePtInfo,
	bool* result
	)
{
	*result = false;
	// �h�ΑΏە��E�s���Y���EGS�̃`�F�b�N
	CComVariant sourceType;
	if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, &sourceType))
		return false;
	switch(sourceType.lVal) {
	case ::buildingname_src_point::source_type::kFirePreventive:	// �h�ΑΏە��̏ꍇ
		if (! AdoptFirePrevChk(ipTgtSrcFeature, result))
			return false;
		if (*result) {
			if (! isPart) {
				// �}�b�`���O�Ώی�����ɖ{�f�ނ������݂��Ȃ��ꍇ
				bldNamePtInfo->e_priority = ::buildingname_point::priority_code::kOneFirePreventive;
			} else {
				// �}�b�`���O�Ώی�����ɓ���ʑf�ނ���������ꍇ
				bldNamePtInfo->e_priority = ::buildingname_point::priority_code::kNPartFirePreventive;
			}
		}
		break;
	case ::buildingname_src_point::source_type::kImmovables:		// �s���Y���̏ꍇ
		if (! AdoptImmovablesChk(tgtSrcID, ipTgtSrcFeature, ipFilledBldPolygon, bldID, result))
			return false;
		if (*result) {
			if (! isPart) {
				// �}�b�`���O�Ώی�����ɖ{�f�ނ������݂��Ȃ��ꍇ
				bldNamePtInfo->e_priority = ::buildingname_point::priority_code::kOneImmovalbles;
			} else {
				// �}�b�`���O�Ώی�����ɓ���ʑf�ނ���������ꍇ
				bldNamePtInfo->e_priority = ::buildingname_point::priority_code::kNPartImmovalbles;
			}
		}
		break;
	case ::buildingname_src_point::source_type::kGeoSpace:			// GS�̏ꍇ
		break;	// GS�����̏ꍇ�͉������Ȃ�(�o�^���Ȃ�)
	default:
		CString errMsg;
		errMsg.Format(_T("%s���s���ł�"), ::buildingname_src_point::kSourceType);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::buildingname_src_point::kTableName, tgtSrcID);
		return false;
	}
	if (*result) {
		// ��\�_�ɑI�΂ꂽ�ꍇ�̂ݍX�V
		bldNamePtInfo->e_bldOID = bldID;
		bldNamePtInfo->e_matchPer = -1;
		bldNamePtInfo->e_srcOID = tgtSrcID;
		bldNamePtInfo->e_srcType = sourceType.lVal;
		// NAME�擾
		CComVariant name;
		if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kName, m_bldSrcIndeces, &name))
			return false;
		bldNamePtInfo->e_name = name.bstrVal;
		// SOURCENAME�擾
		CComVariant srcName;
		if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceName, m_bldSrcIndeces, &srcName))
			return false;
		bldNamePtInfo->e_srcName = srcName.bstrVal;
		// SOURCEID�擾
		CComVariant srcID;
		if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceID, m_bldSrcIndeces, &srcID))
			return false;
		bldNamePtInfo->e_srcID = srcID.bstrVal;
		// ANYPLACE_ID�擾
		CComVariant anyplaceID;
		if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAnyPlaceID, m_bldSrcIndeces, &anyplaceID))
			return false;
		bldNamePtInfo->e_anyPlaceID = anyplaceID.bstrVal;
		// �`��擾
		IGeometryPtr ipTempGeo;
		if (FAILED(ipTgtSrcFeature->get_ShapeCopy(&ipTempGeo))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��擾�Ɏ��s���܂���"), ::buildingname_src_point::kTableName, tgtSrcID);
			return false;
		}
		bldNamePtInfo->e_ipGeometry = ipTempGeo;
	}
	return true;
}

bool MatchingProcessor::MatchingPoints(const IDFeatureMap& tgtSrcPtFeatures, 
									   long bldID,
									   BldNamePtInfo *bldNamePtInfo,
									   bool *result)
{
	*result = false;
	std::map<long, MATCHINGINFO> resultMatch;	// �@ �L�[�F�f�ނ�objecttid �v�f�F�}�b�`���O����
	std::map<long, std::map<long, MATCHINGINFO*>> resultMatchSortPerfectMatch; // �L�[�F���S��v�� �v�f�F�@�̃}�b�v
	std::map<long, std::map<long, MATCHINGINFO*>> resultMatchSortReplacePerfectMatch; // �L�[�F���H�㊮�S��v�� �v�f�F�@�̃}�b�v
	std::map<double, std::map<long, MATCHINGINFO*>> resultMatchSortMaxMatchPer; // �L�[�F�ő�}�b�`���O�� �v�f�F�@�̃}�b�v
	// �e�f�ނ̃}�b�`���O���擾
	if (! GetEachMatchingInfo(tgtSrcPtFeatures,
							  &resultMatch,
							  &resultMatchSortPerfectMatch,
							  &resultMatchSortReplacePerfectMatch,
							  &resultMatchSortMaxMatchPer))
		return false;
	// �}�b�`���O��񂪖����ꍇ�͑S�ē�����ʂȂ̂ŁA�I��点��
	if (resultMatch.size() == 0) 
		return true;
	// ��\�_���菈��
	long headID = -1;
	long priority = -1;
	if (! GetHeadPt(resultMatchSortPerfectMatch, resultMatchSortReplacePerfectMatch, resultMatchSortMaxMatchPer, &headID, &priority)) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("��\�_���菈���Ɏ��s���܂���"), sindy::schema::building::kTableName, bldID);
		return false;
	}
	// �o�^���擾
	// �����o�^
	std::map<long, MATCHINGINFO>::iterator headItr = resultMatch.find(headID);
	bldNamePtInfo->e_name = headItr->second.e_name;
	bldNamePtInfo->e_bldOID = bldID;
	bldNamePtInfo->e_srcType = headItr->second.e_srcType;
	bldNamePtInfo->e_srcName = headItr->second.e_srcName;
	bldNamePtInfo->e_srcID = headItr->second.e_srcID;
	bldNamePtInfo->e_srcOID = headID;
	bldNamePtInfo->e_anyPlaceID = headItr->second.e_anyplaceID;
	bldNamePtInfo->e_priority = priority;
	bldNamePtInfo->e_matchPer = headItr->second.e_maxMatchPer;
	// �`��o�^
	IGeometryPtr ipTempGeo;
	IDFeatureMap::const_iterator featureItr = tgtSrcPtFeatures.find(headID);
	if (FAILED(featureItr->second->get_ShapeCopy(&ipTempGeo))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��擾�Ɏ��s���܂���"), ::buildingname_src_point::kTableName, headID);
		return false;
	}
	bldNamePtInfo->e_ipGeometry = ipTempGeo;
	*result = true;
	return true;
}

long MatchingProcessor::SelectSameSrcBldNamePtInfo(const std::map<long, BldNamePtInfo>& sameSrcBldNamePtInfo)
{
	// ������̒������r���āA�Z��
	std::map<long, BldNamePtInfo> shortStrBldNamePtInfos;
	long length = -1;
	for (const auto& bldNamePt :sameSrcBldNamePtInfo) {
		if (length == -1) {
			shortStrBldNamePtInfos[bldNamePt.first] = bldNamePt.second;
			length = bldNamePt.second.e_name.GetLength();
		} else {
			long tgtLength = bldNamePt.second.e_name.GetLength();
			if (length == tgtLength) {
				shortStrBldNamePtInfos[bldNamePt.first] = bldNamePt.second;
			} else if (length > tgtLength) {
				shortStrBldNamePtInfos.clear();
				shortStrBldNamePtInfos[bldNamePt.first] = bldNamePt.second;
			}
		}
	}
	// ��ԒZ��������̒��ŁA�f��ID����ԎႢ���̂���\
	return shortStrBldNamePtInfos.begin()->first;
}

bool MatchingProcessor::AdoptFirePrevChk(const IFeaturePtr& ipTgtSrcFeature, bool* result)
{
	// �ŏ���false��
	*result = false;
	// �������g�Ɠ���20���R�[�h�����h�ΑΏۑf�ނ����邩�`�F�b�N
	CComVariant addrCode;
	if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAddrCode, m_bldSrcIndeces, &addrCode))
		return false;
	const auto itr = m_mapAddrCodeCounter.find(CString(addrCode));
	if (m_mapAddrCodeCounter.end() != itr && 1 < (*itr).second) {
		// ����20���R�[�h�̖h�ΑΏە��f�ނ��������݂���̂ō̗p�O
		return true;
	}
	// �f�ނ��ێ�����20���R�[�h��iPC�Z���̍����x����1�������`�F�b�N
	CComVariant sameAddr, sameAddrCount, pinpoint;
	if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSameAddr, m_bldSrcIndeces, &sameAddr) ||
		! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSameAddrCount, m_bldSrcIndeces, &sameAddrCount) ||
		! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kPinPoint, m_bldSrcIndeces, &pinpoint))
		return false;
	if ((0 == sameAddr.lVal) && (1 == sameAddrCount.lVal) && (1 == pinpoint.lVal))
		*result = true;		// sameaddr_f = 0, sameaddr_count=1, pinpoin_f=1�ł����iPC�Z���̍����x���ň��
	return true;
}

bool MatchingProcessor::AdoptImmovablesChk(long tgtSrcID, const IFeaturePtr& ipTgtSrcFeature, IPolygonPtr ipFilledBldPolygon, long bldID, bool *result)
{
	*result = false;
	// iPC�Z���̈ܓx�o�x�擾
	CComVariant addrLon, addrLat;
	if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAddrLon, m_bldSrcIndeces, &addrLon) ||
		! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAddrLat, m_bldSrcIndeces, &addrLat))
		return false;
	// �擾�����ܓx�o�x����|�C���g�쐬
	IPointPtr ipPoint(CLSID_Point);
	if (FAILED(ipPoint->put_X(addrLon.dblVal)) || FAILED(ipPoint->put_Y(addrLat.dblVal))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("iPC�Z���ܓx�o�x����̃|�C���g�쐬���s���܂���"), ::buildingname_src_point::kTableName, tgtSrcID);
		return false;
	}
	// �쐬�����|�C���g�Ɍ����ߌ����`��̋�ԎQ�Ƃ�t�^
	ISpatialReferencePtr ipSpRef;
	if (FAILED(ipFilledBldPolygon->get_SpatialReference(&ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("��ԎQ�Ǝ擾�Ɏ��s���܂���"), sindy::schema::building::kTableName, bldID);
		return false;
	}
	if (FAILED(ipPoint->putref_SpatialReference(ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("iPC�Z���ܓx�o�x�|�C���g�ւ̋�ԎQ�ƕt�^�Ɏ��s���܂���"), ::buildingname_src_point::kTableName, tgtSrcID);
		return false;
	}

	// �쐬�����|�C���g�������ߌ����`����ɑ��݂��邩�`�F�b�N
	IRelationalOperatorPtr ipRel(ipFilledBldPolygon);
	VARIANT_BOOL containChk = VARIANT_FALSE;
	if (FAILED(ipRel->Contains((IGeometryPtr)ipPoint, &containChk))) {
		CString errMsg;
		errMsg.Format(_T("�����`��(objectid=%d)�Ƃ�contain�`�F�b�N�Ɏ��s���܂���"), ::buildingname_src_point::kTableName, tgtSrcID);
		return false;
	}
	if (VARIANT_TRUE == containChk) 
		*result = true;	// �����Ɋ܂܂�Ă���΍̗p
	return true;
}

bool MatchingProcessor::GetEachMatchingInfo(const IDFeatureMap& tgtSrcPtFeatures, 
											std::map<long, MATCHINGINFO>* resultMatch, 
											std::map<long, std::map<long, MATCHINGINFO*>>* resultMatchSortPerfectMatch,
											std::map<long, std::map<long, MATCHINGINFO*>>* resultMatchSortReplacePerfectMatch,
											std::map<double, std::map<long, MATCHINGINFO*>>* resultMatchSortMaxMatchPer)
{
	for (const auto& tgtSrcPt : tgtSrcPtFeatures) {
		MATCHINGINFO matchingInfo;
		// ��r���̑f�ގ�ގ擾
		CComVariant srcType1;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, &srcType1))
			return false;
		if (::buildingname_src_point::source_type::kGeoSpace == srcType1.lVal)
			continue;		// �f�ނ�GS�̏ꍇ�͑�\�_�ɂ͂Ȃ�Ȃ��̂Ŕ�΂�
		// �f�ގ�ޓo�^
		matchingInfo.e_srcType = srcType1.lVal;
		// ��r�����̎擾
		CComVariant name;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kName, m_bldSrcIndeces, &name))
			return false;
		CString orgName1 = name.bstrVal;
		// ��r�����̓o�^
		matchingInfo.e_name = orgName1;
		// ���H������擾
		CString replaceName1 = ReplaceStr(orgName1);
		// SOURCENAME�擾
		CComVariant srcName;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceName, m_bldSrcIndeces, &srcName))
			return false;
		matchingInfo.e_srcName = srcName.bstrVal;
		// SOURCEID�擾
		CComVariant srcID;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceID, m_bldSrcIndeces, &srcID))
			return false;
		matchingInfo.e_srcID = srcID.bstrVal;
		// AnyPlaceID�擾
		CComVariant anyplaceID;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAnyPlaceID, m_bldSrcIndeces, &anyplaceID))
			return false;
		matchingInfo.e_anyplaceID = anyplaceID.bstrVal;

		// �����Ɠ����f�ވȊO�Ƃ̃}�b�`���O����
		bool doMatching = false;
		for (const auto& tgtSrcPt2 : tgtSrcPtFeatures) {
			// ��r��̑f�ގ�ގ擾
			CComVariant srcType2;
			if (! ::GetValue((_IRowPtr)tgtSrcPt2.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, &srcType2))
				return false;
			// ��r����ʂƔ�r���ʂ���v�����ꍇ�́A�������΂�
			if (srcType1.lVal == srcType2.lVal)
				continue;
			doMatching = true;
			// ��r�於�̎擾
			CComVariant name2;
			if (! ::GetValue((_IRowPtr)tgtSrcPt2.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kName, m_bldSrcIndeces, &name2))
				return false;
			CString orgName2 = name2.bstrVal;
			// ��r����H�����擾
			CString replaceName2 = ReplaceStr(orgName2);
			// �}�b�`���O����
			// ���������v�`�F�b�N
			if (orgName1 == orgName2)
				++(matchingInfo.e_perfectMatchCount);
			// ���H�㕶����`�F�b�N
			if (replaceName1 == replaceName2)
				++(matchingInfo.e_replacePerfectMatchCount);
			// �}�b�`���O���`�F�b�N
			double matchPer = MatchingStr(replaceName1, replaceName2);
			if (matchPer > matchingInfo.e_maxMatchPer)
				matchingInfo.e_maxMatchPer = matchPer;		// �ő�}�b�`���O���o�^
			// �}�b�`���O���o�^
			matchingInfo.e_allMatchPer[tgtSrcPt2.first] = matchPer;
		}
		if (doMatching) {
			(*resultMatch)[tgtSrcPt.first] = matchingInfo;
			(*resultMatchSortPerfectMatch)[matchingInfo.e_perfectMatchCount].insert(std::pair<long,MATCHINGINFO*>(tgtSrcPt.first, &((*resultMatch)[tgtSrcPt.first])));
			(*resultMatchSortReplacePerfectMatch)[matchingInfo.e_replacePerfectMatchCount].insert(std::pair<long,MATCHINGINFO*>(tgtSrcPt.first, &((*resultMatch)[tgtSrcPt.first])));
			(*resultMatchSortMaxMatchPer)[matchingInfo.e_maxMatchPer].insert(std::pair<long,MATCHINGINFO*>(tgtSrcPt.first, &((*resultMatch)[tgtSrcPt.first])));
		}
	}
	return true;
}

CString MatchingProcessor::ReplaceStr(const CString& orgName)
{
	CString replaceName = orgName;
	for (const auto& strPair :m_replaceList) {
		replaceName.Replace(strPair.e_orgStr, strPair.e_replaceStr);
	}
	return replaceName;
}

double MatchingProcessor::MatchingStr(const CString& str1, const CString& str2)
{
	double matchPer = 0.0;
	CString baseStr, tgtStr;
	long length = 0;
	if (str1.GetLength() > str2.GetLength()) {
		baseStr = str1;
		tgtStr = str2;
		length = _tcsclen(str1);
	} else {
		baseStr = str2;
		tgtStr = str1;
		length = _tcsclen(str2);
	}
	// �ꕶ���Â���
	std::set<CString> baseCharSet = Str2Pieces(baseStr);
	std::set<CString> tgtCharSet = Str2Pieces(tgtStr);
	// ��v�������擾
	long sameCount = 0;
	for (const auto& str : baseCharSet) {
		if (tgtCharSet.find(str) != tgtCharSet.end())
			++sameCount;
	}
	matchPer = (sameCount*100.0)/length;
	return matchPer;
}

bool MatchingProcessor::GetHeadPt(const std::map<long, std::map<long, MATCHINGINFO*>>& resultMatchingSortPerfectMatch,
								  const std::map<long, std::map<long, MATCHINGINFO*>>& resultMatchingSortReplacePerfectMatch,
				                  const std::map<double, std::map<long, MATCHINGINFO*>>& resultMatchingSortMaxMatchPer,
								  long *headID,
								  long *priority)
{
	long maxPerfectMatchCount = resultMatchingSortPerfectMatch.rbegin()->first;
	long maxReplacePefectMatchCount = resultMatchingSortReplacePerfectMatch.rbegin()->first;
	std::map<long, MATCHINGINFO*> maxPerfectMatchInfo = resultMatchingSortPerfectMatch.rbegin()->second;
	std::map<long, MATCHINGINFO*> maxReplacePerfectMatchInfo = resultMatchingSortReplacePerfectMatch.rbegin()->second;
	//�@���H�㊮�S��v������ꍇ
	if (maxReplacePefectMatchCount > 0) {
		if (! GetHeadPtByPerfectMatch(maxPerfectMatchInfo, maxReplacePerfectMatchInfo, maxPerfectMatchCount, headID, priority))
			return false;
	} else {
		// ���H�㊮�S��v�������ꍇ
	std::map<long, MATCHINGINFO*> maxMatchPerInfo = resultMatchingSortMaxMatchPer.rbegin()->second;
	if (! GetHeadPtByMatchPer(maxMatchPerInfo, headID, priority))
		return false;
	}
	return true;
}

bool MatchingProcessor::GetHeadPtByPerfectMatch(const std::map<long, MATCHINGINFO*>& maxPerfectMatchInfo,
												const std::map<long, MATCHINGINFO*>& maxReplacePerfectMatchInfo,
												long maxPerfectMatchCount,
												long *headID,
												long *priority)
{
	bool existPerfectMatch = false;
	if (maxPerfectMatchCount > 0)
		existPerfectMatch = true;
	// �M���x�R�[�h3:���S��v������ԑ����f�ނ���̏ꍇ
	if (maxPerfectMatchInfo.size() == 1) {
		*headID = maxPerfectMatchInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri3;
		return true;
	}
	// �M���x�R�[�h4:���H�㊮�S��v������ԑ����f�ނ���̏ꍇ
	if (maxReplacePerfectMatchInfo.size() == 1) {
		*headID = maxReplacePerfectMatchInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri4;
		return true;
	}
	// �M���x�R�[�h5:���S��v������ԑ����f�ނ̒��ŗD��x����������
	std::map<long, MATCHINGINFO*> perfectMatchPriInfo = GetMatchInfoBySrcPri(maxPerfectMatchInfo);
	if (existPerfectMatch) {
		if (perfectMatchPriInfo.size() == 1) {
			*headID = perfectMatchPriInfo.begin()->first;
			*priority = ::buildingname_point::priority_code::kPri5;
			return true;
		}
		if (perfectMatchPriInfo.size() == 0) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�f�ޗD��x��ʃ��X�g�ɑ��݂��Ȃ��f�ނ�����܂�"), ::buildingname_src_point::kTableName);
			return false;
		}
	}
	// �M���x�R�[�h6:���H�㊮�S��v������ԑ������݂̒��ŗD��x����������
	std::map<long, MATCHINGINFO*> replacePerfectMatchPriInfo = GetMatchInfoBySrcPri(maxReplacePerfectMatchInfo);
	if (replacePerfectMatchPriInfo.size() == 1) {
		*headID = replacePerfectMatchPriInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri6;
		return true;
	}
	if (replacePerfectMatchPriInfo.size() == 0) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�f�ޗD��x��ʃ��X�g�ɑ��݂��Ȃ��f�ނ�����܂�"), ::buildingname_src_point::kTableName);
			return false;
	}
	// �M���x�R�[�h7:�M���x�R�[�h5�̃`�F�b�N�̍ۂɎ擾�����f�ނ̂��������񂪈�ԒZ������
	std::map<long, MATCHINGINFO*> perfectMatchLenInfo = GetMatchInfoByStrLen(perfectMatchPriInfo);
	if (existPerfectMatch) {
		if (perfectMatchLenInfo.size() == 1) {
			*headID = perfectMatchLenInfo.begin()->first;
			*priority = ::buildingname_point::priority_code::kPri7;
			return true;
		}
	}
	// �M���x�R�[�h8:�M���x�R�[�h6�̃`�F�b�N�̍ۂɎ擾�����f�ނ̂��������񂪈�ԒZ������
	std::map<long, MATCHINGINFO*> replacePerfectMatchLenInfo = GetMatchInfoByStrLen(replacePerfectMatchPriInfo);
	if (replacePerfectMatchLenInfo.size() == 1) {
		*headID = replacePerfectMatchLenInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri8;
		return true;
	}
	// �M���x�R�[�h9:�M���x�R�[�h7�̃`�F�b�N�̍ۂɎ擾�����f�ނ̂����Aobjectid����ԎႢ����
	if (existPerfectMatch) {
		if (perfectMatchLenInfo.size() >= 0) {
			*headID = perfectMatchLenInfo.begin()->first;
			*priority = ::buildingname_point::priority_code::kPri9;
			return true;
		}
	}
	// �M���x�R�[�h10:�M���x�R�[�h8�̃`�F�b�N�̍ۂɎ擾�����f�ނ̂����Aobjectid����ԎႢ����
	if (replacePerfectMatchLenInfo.size() >= 0) {
		*headID = replacePerfectMatchLenInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri10;
		return true;
	}
	// �����܂ŗ��邱�Ƃ͂Ȃ��͂��Ȃ̂ŁA�Ō�false�ɂ��Ă���
	LogSys::GetInstance().WriteFreeStyleLog(true, err_level::fatal, _T("�v���O�������s���ł�(��\�_���蕔)�B�J���҂ɂ��₢���킹���������B"));
	return false;
}

bool MatchingProcessor::GetHeadPtByMatchPer(const std::map<long, MATCHINGINFO*>& maxMatchPerInfo,
											long *headID,
											long *priority)
{
	// �M���x�R�[�h11:�}�b�`���O�����ő�̑f�ނ����
	if (maxMatchPerInfo.size() == 1) {
		*headID = maxMatchPerInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri11;
		return true;
	}
	// �M���x�R�[�h12:�M���x�R�[�h11�Ŏ擾�����f�ނ̂����A�D��x����ԍ�������
	std::map<long, MATCHINGINFO*> matchPerPriInfo = GetMatchInfoBySrcPri(maxMatchPerInfo);
	if (matchPerPriInfo.size() == 1) {
		*headID = matchPerPriInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri12;
		return true;
	}
	if (matchPerPriInfo.size() == 0) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�f�ޗD��x��ʃ��X�g�ɑ��݂��Ȃ��f�ނ�����܂�"), ::buildingname_src_point::kTableName);
			return false;
	}
	// �M���x�R�[�h13:�M���x�R�[�h12�Ŏ擾�����f�ނ̂����A������̒�������ԒZ������
	std::map<long, MATCHINGINFO*> matchPerLenInfo = GetMatchInfoByStrLen(matchPerPriInfo);
	if (matchPerLenInfo.size() == 1) {
		*headID = matchPerLenInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri13;
		return true;
	}
	// �M���x�R�[�h14:�M���x�R�[�h13�Ŏ擾�����f�ނ̂����AobjectID����ԎႢ����
	if (matchPerLenInfo.size() > 0) {
		*headID = matchPerLenInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri14;
		return true;
	}
	// �����܂ŗ��邱�Ƃ͂Ȃ��͂��Ȃ̂ŁA�Ō�false�ɂ��Ă���
	LogSys::GetInstance().WriteFreeStyleLog(true, err_level::fatal, _T("�v���O�������s���ł�(��\�_���蕔)�B�J���҂ɂ��₢���킹���������B"));
	return false;
}

std::map<long, MATCHINGINFO*> MatchingProcessor::GetMatchInfoBySrcPri(const std::map<long, MATCHINGINFO*>& matchingInfos)
{
	std::map<long, MATCHINGINFO*> retMatchingInfos;
	for (const auto & srcPri : m_srcPriListMatch) {
		for (const auto & matchingInfo: matchingInfos) {
			if (srcPri == matchingInfo.second->e_srcType)
				retMatchingInfos[matchingInfo.first] = matchingInfo.second;
		}
		// �D��x����for���͉񂵂Ă���̂ŁA�ŏ���retMatchingInfos�̗v�f�������ɂȂ������_�ŏI��
		if (0 < retMatchingInfos.size()) 
			break;
	}
	return retMatchingInfos;
}

std::map<long, MATCHINGINFO*> MatchingProcessor::GetMatchInfoByStrLen(const std::map<long, MATCHINGINFO*>& matchingInfos)
{
	std::map<long, MATCHINGINFO*> retMatchingInfos;
	long length = -1;
	for (const auto & matchingInfo : matchingInfos) {
		if (length == -1) {
			retMatchingInfos[matchingInfo.first] = matchingInfo.second;
			length = matchingInfo.second->e_name.GetLength();
		} else {
			long tgtLength = matchingInfo.second->e_name.GetLength();
			if (length == tgtLength) {
				retMatchingInfos[matchingInfo.first] = matchingInfo.second;
			} else if (length > tgtLength) {
				retMatchingInfos.clear();
				retMatchingInfos[matchingInfo.first] = matchingInfo.second;
			}
		}
	}
	return retMatchingInfos;
}

void MatchingProcessor::SetRelIDs(const IDFeatureMap& tgtSrcPtFeatures, long headID, std::set<long>* relIDs)
{
	for (const auto& tgtSrcPt : tgtSrcPtFeatures){
		if (tgtSrcPt.first != headID)
			relIDs->insert(tgtSrcPt.first);
	}
}

std::set<CString> MatchingProcessor::Str2Pieces(const CString& str)
{
	std::set<CString> strSet;
	long num = str.GetLength();
	for (long i = 0; i < num; ++i) {
		CString temp = str.Mid(i, 1);
		if (strSet.find(temp) == strSet.end())
			strSet.insert(temp);
	}
	return strSet;
}

bool MatchingProcessor::SupplyWithPoiPtOrg(const IDFeatureMap& tgtPoiPtFeatures, long bldID, BldNamePtInfo *bldNamePtInfo, bool *result)
{
	using namespace sindy::schema::sindyk;

	*result = false;

	// �}�b�`���O�Ώے��LPOI�|�C���g���Ȃ��ꍇ�͏I��
	if (0 == tgtPoiPtFeatures.size())
		return true;

	// ���LPOI�|�C���g�̃��X�g���A�R���e���c�R�[�h���L�[�Ƃ������X�g�ɕϊ�
	std::map<CString, std::set<POIINFO>> poiInfos;
	for (const auto& tgtPoiPt : tgtPoiPtFeatures) {
		// �R���e���c�R�[�h�擾
		CComVariant contentsCode;
		if (! ::common_proc::GetValue((_IRowPtr)tgtPoiPt.second.m_T, poi_point_org::kTableName, poi_point_org::kContentsCode, m_poiPtOrgIndeces, &contentsCode))
			return false;
		// �R���e���c���V�[�P���X�擾
		CComVariant contentsSeq;
		if (! ::common_proc::GetValue((_IRowPtr)tgtPoiPt.second.m_T, poi_point_org::kTableName, poi_point_org::kContentsSeq, m_poiPtOrgIndeces, &contentsSeq))
			return false;
		// ���̎擾
		CComVariant name;
		if (! ::common_proc::GetValue((_IRowPtr)tgtPoiPt.second.m_T, poi_point_org::kTableName, poi_point_org::kName, m_poiPtOrgIndeces, &name))
			return false;
		IGeometryPtr ipGeometry;
		if (FAILED((tgtPoiPt.second.m_T)->get_ShapeCopy(&ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��擾�Ɏ��s���܂���"), poi_point_org::kTableName, tgtPoiPt.first);
			return false;
		}
		// POI���}�b�v�ւ̃��R�[�h�ǉ�
		POIINFO poiInfo;
		poiInfo.e_objectid		= tgtPoiPt.first;
		poiInfo.e_name			= name.bstrVal;
		poiInfo.e_nameLength	= poiInfo.e_name.GetLength();
		poiInfo.e_srcID.Format(_T("%ld-%ld-%s"), (contentsCode.lVal / 100000), (contentsCode.lVal % 100000), contentsSeq.bstrVal);	// �R���e���c�R�[�h������5���Ə�ʌ��i4�`5���j�ɕ���
		poiInfo.e_ipGeometry	= ipGeometry;
		poiInfos[std::to_string(contentsCode.lVal).c_str()].insert(poiInfo);
	}

	// �ΏۂƂȂ钍�LPOI�|�C���g�����݂��Ȃ��ꍇ�͏I��
	if (0 == poiInfos.size())
		return true;

	// �D��x���X�g�����ƂɁA�P�̕⊮�p���LPOI�|�C���g��I��
	for (const auto& poiCode : m_supPoiPriList) {
		// m_poiPriList�͗D��x���Ɋi�[����Ă���̂ŁA�����莟��I��
		std::map<CString, std::set<POIINFO>>::const_iterator tgtSrcItr = poiInfos.find(poiCode);
		if (tgtSrcItr != poiInfos.end()) {
			// std::set<POIINFO>�͕�����̒Z�����i������̒������ꏏ�̏ꍇ��objectid���j�Ŋi�[����Ă���̂�1�ڂ̗v�f���Ώ�
			std::set<POIINFO>::const_iterator poiInfoItr = tgtSrcItr->second.begin();
			bldNamePtInfo->e_name		= poiInfoItr->e_name;
			bldNamePtInfo->e_bldOID		= bldID;
			bldNamePtInfo->e_srcType	= ::buildingname_point::source_type::kCompAnnoPOI;
			bldNamePtInfo->e_srcID		= poiInfoItr->e_srcID;
			bldNamePtInfo->e_srcOID		= poiInfoItr->e_objectid;
			bldNamePtInfo->e_priority	= ::buildingname_point::priority_code::kCompAnnoPoi;
			bldNamePtInfo->e_matchPer	= -1;
			bldNamePtInfo->e_ipGeometry	= poiInfoItr->e_ipGeometry.m_T;
			*result = true;
			// �c���objectid�����O�ɏo�͂���B
			++poiInfoItr;
			for (; poiInfoItr != tgtSrcItr->second.end(); ++poiInfoItr) {
				LogSys::GetInstance().WriteFreeStyleLog(false, err_level::notice, _T("����POI�W���������������݂����ׁA������̒��� or objectid���őΏۊO�ɂȂ�܂���"), poi_point_org::kTableName, poiInfoItr->e_objectid);
			}
			break;
		}
	}

	return true;
}
