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
#include "POICheckFunction.h"
#include "value.h"
#include "util.h"

using namespace sindy::schema;

void POICheckFunction::CheckPoiPoint(
	const CString& meshcode,
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& poiPointPack,
	const FeaturesPack& walkNodePack,
	const BGRTree& walkNodeRTree )
{
	using namespace sindyk::poi_point_org;
	using namespace ipc_feature;

	m_output.OutputStdErr( err_level::info, err_code::StrChecking, kTableName );

	// POI�|�C���g�̃`�F�b�N
	long deleteIndex = poiPointPack.m_FieldMap.FindField( kDelete );
	long nameIndex = poiPointPack.m_FieldMap.FindField( kName );
	long yomiIndex = poiPointPack.m_FieldMap.FindField( kYomi );
	long modOperatorIndex = poiPointPack.m_FieldMap.FindField( kOperator );
	long modDateIndex = poiPointPack.m_FieldMap.FindField( kModifyDate );
	long modProgramIndex = poiPointPack.m_FieldMap.FindField( kModifyProgName );
	long modProgramDateIndex = poiPointPack.m_FieldMap.FindField( kProgModifyDate );
	for ( const auto& poiPoint : poiPointPack.m_FeatureMap )
	{
		const auto& pointObj  = poiPoint.second;

		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if( pointObj.IsDisjoint( ipMeshGeo ) )
			continue;

		// [bug11619] �폜�R�[�h���u�����v����Ȃ����̂̓X�L�b�v
		if ( delete_code::kAlive != pointObj.GetLongValue( deleteIndex ) ) 
			continue;

		// ���g�ɖ��̂��Ȃ��ꍇ�X�L�b�v
		if ( pointObj.GetStringValue( nameIndex ).IsEmpty() )
			continue;

		if( ! util::existAroundWalkNodeSameName(pointObj, poiPointPack, walkNodePack, walkNodeRTree,
			kName, kYomi,walk_node::kUndergroundGatePointID, m_refer_id_threshold, walk_node::node_class::kUndergroundGate ) )
		{
			// �����[�j���O �t�߂ɓ���̒n���X�o�������������s�҃m�[�h�����݂��Ȃ�
			m_output.OutputLog( kTableName, poiPoint.first, err_level::warning, err_code::NotExistSameNamePOI,
				meshcode, pointObj.GetStringValue( modOperatorIndex ) , pointObj.GetStringValue( modDateIndex ),
				pointObj.GetStringValue( modProgramIndex ), pointObj.GetStringValue( modProgramDateIndex ), 
				pointObj.GetStringValue( nameIndex ), pointObj.GetStringValue( yomiIndex ) );
		}
	}
}
