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

#pragma once

#include <set>
#include <vector>
#include <memory>
#include <CodeanAlysis/SourceAnnotations.h>
#include "RoadLink.h"
#include "BasicData.h"
#include "RegionProperty.h"

/**
 * @brief ���[�W�����̃I�u�W�F�N�g���
 */
struct RegionData
{
	std::auto_ptr<RoadLinks> m_pRoadLinks; ///< ���[�W�����I�[�o�[���b�v�͈͓����H�����N�ꗗ�B
	std::auto_ptr<RoadNodeIDSet> m_pInnerRoadNodeIDSet; ///< ���[�W���������H�m�[�hID�ꗗ�B
	std::auto_ptr<RoadNodeIDSet> m_pOuterRoadNodeIDSet; ///< ���[�W�����O�A�I�[�o�[���b�v�͈͓����H�m�[�hID�ꗗ�B
};

std::auto_ptr<RegionData>
create_region_data(
	[SA_Pre(Null=SA_No)] IFeatureClass* ipRoadLinkClass,
	[SA_Pre(Null=SA_No)] IFeatureClass* ipRoadNodeClass,
	const RegionProperty& rRegionProp
);
