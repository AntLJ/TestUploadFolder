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

// DRMALayerLoad.h
//
//////////////////////////////////////////////////////////////////////

/**
 * @file DRMALayerLoad.h
 * @brief DRMA�f�[�^���C���P�ʓǂݍ��݃N���X��`�w�b�_�t�@�C��
 */
#ifndef __DRMA_LAYER_LOAD_H__
#define __DRMA_LAYER_LOAD_H__

#include "DRMALoadProtocol.h"
#include "DRMALayer.h"

/**
 * @brief ���C���P�ʓǂݍ��݃e���v���[�g�N���X
 */
template<typename TLayer, typename TProtocol = DRMALoadProtocol>
class DRMALayerLoadTemplate : public TProtocol, public TLayer
{
protected:
// �v���g�R���̎���
	virtual bool append(int cRecordId, const char* cOriginalRecord)
	{
		if(cRecordId != Record::sRecordId)
			return true;

		TLayer::append(reinterpret_cast<const Record::Format&>(*cOriginalRecord));

		return true;
	}

	virtual void clear()
	{
		TLayer::clear();
	}
};

/**
 * @brief �Ǘ����C���p�ǂݍ��݃e���v���[�g�N���X
 *
 * �Ǘ��f�[�^�͂P���R�[�h�������݂��Ȃ����߁A�P���������炻���ŏI��
 */
template<typename TLayer, typename TProtocol = DRMALoadProtocol>
class DRMAAdminLayerLoadTemplate : public DRMALayerLoadTemplate<TLayer, TProtocol>
{
protected:
// �v���g�R���̎���
	virtual bool append(int cRecordId, const char* cOriginalRecord)
	{
		if(! DRMALayerLoadTemplate<TLayer, TProtocol>::append(cRecordId, cOriginalRecord))
			return false;

		setComplete();

		return true;
	}
};

// DRMA���C���ǂݍ��݃N���X��`

typedef DRMAAdminLayerLoadTemplate<DRMAAdmin1Layer> DRMAAdmin1Load;
typedef DRMAAdminLayerLoadTemplate<DRMAAdmin2Layer> DRMAAdmin2Load;
typedef DRMALayerLoadTemplate<DRMABaseRoadNodeLayer> DRMABaseRoadNodeLoad;
typedef DRMALayerLoadTemplate<DRMABaseRoadLinkLayer> DRMABaseRoadLinkLoad;
typedef DRMALayerLoadTemplate<DRMABaseRoadLinkAttrLayer> DRMABaseRoadLinkAttrLoad;
typedef DRMALayerLoadTemplate<DRMARoadLinkRelationLayer> DRMARoadLinkRelationLoad;
typedef DRMALayerLoadTemplate<DRMABeaconLocationLayer> DRMABeaconLocationLoad;
typedef DRMALayerLoadTemplate<DRMAAllRoadNodeLayer> DRMAAllRoadNodeLoad;
typedef DRMALayerLoadTemplate<DRMAAllRoadLinkLayer> DRMAAllRoadLinkLoad;
typedef DRMALayerLoadTemplate<DRMASiteLayer> DRMASiteLoad;
typedef DRMALayerLoadTemplate<DRMAAdminLocationLayer> DRMAAdminLocationLoad;
typedef DRMALayerLoadTemplate<DRMARailwayLocationLayer> DRMARailwayLocationLoad;
typedef DRMALayerLoadTemplate<DRMAFacilLocationLayer> DRMAFacilLocationLoad;
typedef DRMALayerLoadTemplate<DRMAFacilShapeLayer> DRMAFacilShapeLoad;
typedef DRMALayerLoadTemplate<DRMAPlaceNameLocationLayer> DRMAPlaceNameLocationLoad;
typedef DRMALayerLoadTemplate<DRMAAllRoadLinkAttrLayer> DRMAAllRoadLinkAttrLoad;

/**
 * @example example2.cpp
 *
 * D�h���C�u�̃��[�g�f�B���N�g�������ɑ��݂���uDRMA�v�f�B���N�g�����533945�̊�{���H�m�[�h�f�[�^��ǂݍ��݁A
 * �����_���̂����m�[�h�̏���W���o�͂ɏo�͂���B
 */

#endif
