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

// KKDLayerLoad.h
//
//////////////////////////////////////////////////////////////////////

/**
 * @file KKDLayerLoad.h
 * @brief KKD�f�[�^���C���P�ʓǂݍ��݃N���X��`�w�b�_�t�@�C��
 */
#ifndef __KKD_LAYER_LOAD_H__
#define __KKD_LAYER_LOAD_H__

#include <DRMALayerLoad.h>
#include "KKDLoadProtocol.h"
#include "KKDLayer.h"

/**
 * @brief KKD���C���P�ʓǂݍ��݃e���v���[�g�N���X
 */
template <typename TLayer>
class KKDLayerLoadTemplate : public DRMALayerLoadTemplate<TLayer, KKDLoadProtocol>
{
};

/**
 * @brief KKD�Ǘ����C���p�ǂݍ��݃e���v���[�g�N���X
 */
template <typename TLayer>
class KKDAdminLayerLoadTemplate : public DRMAAdminLayerLoadTemplate<TLayer, KKDLoadProtocol>
{
};

// KKD���C���ǂݍ��݃N���X��`

typedef KKDAdminLayerLoadTemplate<KKDAdminLayer> KKDAdminLoad;
typedef KKDLayerLoadTemplate<KKDRegulationRoadNodeLayer> KKDRegulationRoadNodeLoad;
typedef KKDLayerLoadTemplate<KKDRegulationRoadLinkLayer> KKDRegulationRoadLinkLoad;
typedef KKDLayerLoadTemplate<KKDLaneRegulationRoadNodeLayer> KKDLaneRegulationRoadNodeLoad;
typedef KKDLayerLoadTemplate<KKDLaneRegulationRoadLinkLayer> KKDLaneRegulationRoadLinkLoad;
typedef KKDLayerLoadTemplate<KKDLaneRegulationIntersectionNodeLayer> KKDLaneRegulationIntersectionNodeLoad;

/**
 * @example example2.cpp
 *
 * D�h���C�u�̃��[�g�f�B���N�g�������ɑ��݂���ukkd�v�f�B���N�g�����533945��KKD��ʋK�����H�m�[�h�f�[�^��ǂݍ��݁A
 * �e������o�͂���B
 */

#endif
