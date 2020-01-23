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
#include "ScrambleAreaCheckFunction.h"
#include "value.h"
#include "CheckWalk.h"

using namespace std;
using namespace sindy;
using namespace sindy::schema;

extern crd_cnv g_cnv;

void ScrambleAreaCheckFunction::check(
	const CString& meshcode,
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& scrambleArea,
	const FeaturesPack& walkLink,
	const BGRTree& walkLinkRTree,
	properties::check_target checktarget) const
{
	using namespace scramble_area;
	using namespace properties;

	if (m_dataType != walk_type::type_class::kDownConvert) return;

	m_output.SetLayer(kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, kTableName);

	long modOperatorIndex = scrambleArea.m_FieldMap.FindField(kOperator);
	long modDateIndex = scrambleArea.m_FieldMap.FindField(kModifyDate);
	long modProgramIndex = scrambleArea.m_FieldMap.FindField(kModifyProgName);
	long modProgramDateIndex = scrambleArea.m_FieldMap.FindField(kProgModifyDate);
	long scrambleAreaClassIndex = scrambleArea.m_FieldMap.FindField(kScrambleClass);

	for (const auto& scrambleAreaPair : scrambleArea.m_FeatureMap)
	{
		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if (scrambleAreaPair.second.IsDisjoint(ipMeshGeo))
			continue;

		auto  scrambleAreaID = scrambleAreaPair.first;
		auto& scrambleAreaObj = scrambleAreaPair.second;
		long  scrambleAreaClass = scrambleAreaObj.GetLongValue(scrambleAreaClassIndex);

		m_output.SetModifyInfo(scrambleAreaID, scrambleAreaObj.GetStringValue(modOperatorIndex),
			scrambleAreaObj.GetStringValue(modDateIndex), scrambleAreaObj.GetStringValue(modProgramIndex), scrambleAreaObj.GetStringValue(modProgramDateIndex));

		if (checktarget == check_target::all || checktarget == check_target::other ||
			checktarget == check_target::release)
		{
			// ���G���[ �X�N�����u���G���A���ɃX�N�����u���t���O���t�^���ꂽ���f������1������
			checkScrambleAreaCrossWalkLink(scrambleAreaObj, walkLink, walkLinkRTree);

			if (checktarget != check_target::release)
			{
				// ���G���[ �X�N�����u����ʃR�[�h���s���Ȓl
				checkScrambleAreaClass(scrambleAreaClass);
			}
		}

	}
}

// �X�N�����u���G���A�`�󂪕s���ł��邩�`�F�b�N
void ScrambleAreaCheckFunction::checkScrambleAreaGeometry(const ITablePtr& ipTable) const
{
	// �S���擾
	IFeatureCursorPtr ipCursor = ipTable->_Search(NULL, VARIANT_FALSE);
	if (!ipCursor) return;

	IFeaturePtr ipFeature;
	// �I�u�W�F�N�g���Ƃ̏���
	while (S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature) {
		IGeometryPtr ipGeometry = ipFeature->GetShape();
			
		if( !ipGeometry )
		{
			// ���G���[ �`�󂪑��݂��Ȃ�
			m_output.OutputRegLog( err_level::fatal, err_code::NgScrambleAreaGeometry);
			return;
		}

		esriGeometryType eType = ipGeometry->GetGeometryType();
		if( eType != esriGeometryPolygon )
		{
			// ���G���[ �|���S���^�ł͂Ȃ�
			m_output.OutputRegLog( err_level::fatal, err_code::NgScrambleAreaGeometry);
			return;
		}
	}
}

// �X�N�����u���G���A���ɃX�N�����u���t���O���t�^���ꂽ���f������1���������`�F�b�N
void ScrambleAreaCheckFunction::checkScrambleAreaCrossWalkLink(
	const GeoClass& scrambleAreaObject,
	const FeaturesPack& walkLink,
	const BGRTree& walkLinkRTree) const
{
	using namespace walk_link;
	using namespace uh::str_util;

	std::vector<GeoClass> crossObjects;
	if (util::findOverlapGeometry(scrambleAreaObject, walkLink, walkLinkRTree, crossObjects) <= 0)
	{
		m_output.OutputRegLog(err_level::error, err_code::NgScrambleAreaCrossWalkLink);
		return;
	}

	long  walkLinkClassIndex = walkLink.m_FieldMap.FindField(kWalkClass);
	long  walkLinkScrambleFlagIndex = walkLink.m_FieldMap.FindField(kScrambleFlag);

	for (const auto& crossObject : crossObjects)
	{
		auto walkLinkObj = walkLink.m_FeatureMap.find(crossObject.m_ObjectID);
		if (walkLinkObj == walkLink.m_FeatureMap.end()) continue;

		VARIANT_BOOL contains(VARIANT_FALSE);
		if (FAILED(((IRelationalOperatorPtr)scrambleAreaObject.m_ipGeometry)->Contains(crossObject.m_ipGeometry, &contains)) || contains == VARIANT_FALSE) continue;

		long  walkLinkClass = walkLinkObj->second.GetLongValue(walkLinkClassIndex);
		long  walkLinkScrambleFlag = walkLinkObj->second.GetLongValue(walkLinkScrambleFlagIndex);

		if ((walkLinkClass == walk_class::kCrossRoad && walkLinkScrambleFlag == 1))
			return;
	}

	m_output.OutputRegLog(err_level::error, err_code::NgScrambleAreaCrossWalkLink);
}

// �X�N�����u����ʃR�[�h���s���Ȓl���`�F�b�N
void ScrambleAreaCheckFunction::checkScrambleAreaClass(
	long  scrambleAreaClass
) const
{
	using namespace scramble_area;

	if (scrambleAreaClass != scramble_class::kAuto &&  scrambleAreaClass != scramble_class::kManual)
		m_output.OutputRegLog(err_level::fatal, err_code::NgScrambleAreaClass);
}

// ��O���[�h�p�̃`�F�b�N
void ScrambleAreaCheckFunction::checkException(
	const FeaturesPack& scrambleArea
)
{
	if (m_dataType != walk_type::type_class::kDownConvert) return;

	// ���G���[ �X�N�����u���G���A�|���S���̌`�󂪕s��
	checkScrambleAreaGeometry(scrambleArea.m_ipTable);
}