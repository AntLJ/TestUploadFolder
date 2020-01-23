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

#include <sindy/schema/map.h>

using namespace sindy::schema;

namespace{
	// �w�i�̃}�[�W�Ώێ�ʁi�}�[�W�p�̐ݒ�t�@�C�����]�L�j
	// GEOSPACE�Œǉ����ꂽ���̂őΏۂ̃R�[�h��ǉ��A���̂����g���Ă��Ȃ��R�[�h�͍폜
	static const long CS_TARGET_LIST[] = {
		city_site::bg_class::kBlock,                   //!< �X��
		city_site::bg_class::kSands,                   //!< ���n
		city_site::bg_class::kMarsh,                   //!< ���n
		city_site::bg_class::kLawnOther,               //!< �Ő������̑��Βn
		city_site::bg_class::kPark,                    //!< �A�͒n
		city_site::bg_class::kLevee,                   //!< ��O�n
		city_site::bg_class::kPool,                    //!< �v�[��
		city_site::bg_class::kSideway,                 //!< �����i���̑����j
		city_site::bg_class::kCenterDivider,           //!< ���������сi�W���j
		city_site::bg_class::kPublicRoadStairs,        //!< �����K�i
		city_site::bg_class::kHospital,                //!< �a�@
		city_site::bg_class::kSchool,                  //!< �w�Z
		city_site::bg_class::kPlant,                   //!< �H��
		city_site::bg_class::kDamGuideExist,           //!< �_���i�֗��L�ڂ���j
		city_site::bg_class::kDamGuideNotExist,        //!< �_���i�֗��L�ڂȂ��j
		city_site::bg_class::kStationSite,             //!< �S���~�n
		city_site::bg_class::kOtherSite,               //!< �ܑ��n���̑��~�n
		city_site::bg_class::kBreakwater,              //!< �h�g��
		city_site::bg_class::kForce,                   //!< ���q���E�ČR���K��i���Ԓn�ق��j
		city_site::bg_class::kForceManeuveringGround,  //!< ���q���E�ČR���K��i���K��j
		city_site::bg_class::kCenterDividerPark,       //!< ���������сi�A�͒n�j
	};

	// CityLine�̃}�[�W�Ώ�(bug 10519�Œǉ�)
	static const long CL_TARGET_LIST[] = {
		city_line::bg_class::kTollbooth,         //!< ������
		city_line::bg_class::kWatergate,         //!< ����
		city_line::bg_class::kErosionControlDam, //!< ���h�_��
		city_line::bg_class::kStairsDecoration,  //!< �K�i������
		city_line::bg_class::kParent,            //!< �e��
		city_line::bg_class::kStairsTread,       //!< �K�i���ʃ��C���i�����K�i�j
	};

	// �G���A�|���S���̃}�[�W�Ώێ�� (�������E��ƒ��ȊO)
	static const long AP_TARGET_LIST[] = {
		city_area::area_class::kPark,   //!< ����
	};
			
	// �s���E�̃}�[�W�Ώێ�� (�C�ȊO)
	static const long ADM_TARGET_LIST[] = {
		city_admin::area_class::kOutArea, //!< �G���A�O
		city_admin::area_class::kInArea,  //!< �G���A��
	};
}
