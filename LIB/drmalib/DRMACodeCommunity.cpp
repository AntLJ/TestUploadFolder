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

// DRMACodeCommunity.cpp: DRMACodeCommunity �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "DRMACodeCommunity.h"

const int drma::admin1::kRecordId = drma::kAdmin1Id;
const int drma::admin2::kRecordId = drma::kAdmin2Id;
const int drma::base_road_node::kRecordId = drma::kBaseRoadNodeId;
const int drma::base_road_link::kRecordId = drma::kBaseRoadLinkId;
const int drma::base_road_link_attr::kRecordId = drma::kBaseRoadLinkAttrId;
const int drma::road_link_relation::kRecordId = drma::kRoadLinkRelationId;
const int drma::beacon_location::kRecordId = drma::kBeaconLocationId;
const int drma::all_road_node::kRecordId = drma::kAllRoadNodeId;
const int drma::all_road_link::kRecordId = drma::kAllRoadLinkId;
const int drma::site::kRecordId = drma::kSiteId;
const int drma::admin_location::kRecordId = drma::kAdminLocationId;
const int drma::railway_location::kRecordId = drma::kRailwayLocationId;
const int drma::facil_location::kRecordId = drma::kFacilLocationId;
const int drma::facil_shape::kRecordId = drma::kFacilShapeId;
const int drma::place_name_location::kRecordId = drma::kPlaceNameLocationId;
const int drma::all_road_link_attr::kRecordId = drma::kAllRoadLinkAttrId;

/// �m�[�h��ʃR�[�h�h���C�����ꗗ
const char* drma::codes::b::kDomains[] = {
	"",
	"�����_�m�[�h",
	"�s���~�܂�m�[�h",
	"�_�~�[�_�m�[�h",
	"���ӌ�_�m�[�h",
	"�����ω��_�m�[�h",
	"��ʊǐ���K�v�ȃm�[�h"
};

/// �����_�ʍs�R�[�h�h���C�����ꗗ
const char* drma::codes::c::kDomains[] = {
	"������",
	"�ʍs��",
	"�ʍs�s��",
	"�����t�i�Ԏ�j�ʍs��",
	"�����t�i�����A�j���j�ʍs��",
	"�����t�i�����A�j���A�Ԏ�j�ʍs��"
};

/// �Ǘ��҃R�[�h�h���C�����ꗗ
const char* drma::codes::d::kDomains[] = {
	"������",
	"���{���H���c",
	"���H�R���c",
	"���H����",
	"��",
	"�s���{��",
	"�w��s",
	"���̎s�������i�ܓ���23��j",
	"���̑��̊Ǘ���"
};

/// ���H��ʃR�[�h�h���C�����ꗗ
const char* drma::codes::e::kDomains[] = {
	"������",
	"���������ԓ��H",
	"�s�s�������H�i�܎w��s�s�������H�j",
	"��ʍ���",
	"��v�n�����i�s���{�����j",
	"��v�n�����i�w��s���j",
	"��ʓs���{����",
	"�w��s�̈�ʎs��",
	"",	// [8]�͔����Ă��邽��
	"���̑����H"
};

/// ��E�]���H�敪�R�[�h�h���C�����ꗗ
const char* drma::codes::f::kDomains[] = {
	"������",
	"�哹�H",
	"�]���H"
};

/// �����N��ʃR�[�h�h���C�����ꗗ
const char* drma::codes::h::kDomains[] = {
	"������",
	"�{���i�㉺���񕪗��j�����N",
	"�{���i�㉺�����j�����N",
	"�A���H�i�{���Ԃ̓n����j�����N",
	"�����_�������N",
	"�A���H�i�����v�j�����N",
	"�{���Ɠ���H���̑��������N",
	"�r�`�����������N",
	"���]�ԓ��������N",
	"�{�������ڑ������N"
};

/// �����N�ʍs�E�����ԕs�R�[�h�h���C�����ꗗ
const char* drma::codes::i::kDomains[] = {
	"������",
	"�����Ԓʍs��",
	"�����Ԓʍs�s��",
	"�����p�i�܍H�����j",
	"�v�挈��"
};

/// �ُ�C�ێ��ʍs�K����Ԏ�ʃR�[�h�h���C�����ꗗ
const char* drma::codes::j::kDomains[] = {
	"������",
	"�K����",
	"�J�ʋK���L",
	"��K���L",
	"���̑��K���L"
};

/// ���H�����敪�R�[�h�h���C�����ꗗ
const char* drma::codes::k::kDomains[] = {
	"������",
	"����13.0m�ȏ�",
	"����5.5m�ȏ�`13.0m����",
	"����3.0m�ȏ�`5.5m����",
	"����3.0m����"
};

/// �Ԑ����R�[�h�h���C�����ꗗ
const char* drma::codes::l::kDomains[] = {
	"������",
	"1�Ԑ�",
	"2�Ԑ�",
	"3�Ԑ�",
	"4�Ԑ�",
	"5�Ԑ�",
	"6�Ԑ��ُ�"
};

/// ��ʋK����ʃR�[�h�h���C�����ꗗ
const char* drma::codes::m::kDomains[] = {
	"������",
	"�K������",
	"�ʍs�֎~�i�������j",
	"�ʍs�֎~�i�����t�j",
	"����ʍs(�������A������)",
	"����ʍs(�t�����A������)",
	"����ʍs(�������A�����t)",
	"����ʍs(�t�����A�����t)",
	"����ʍs(���t�ؑւ��L��)"
};

/// ��ʋK��������ʃR�[�h�h���C�����ꗗ
const char* drma::codes::n::kDomains[] = {
	"������",
	"�Ԏ�̂�",
	"�����̂�",
	"�j���̂�",
	"�Ԏ�y�ю���",
	"�Ԏ�y�їj��",
	"�j���y�ю���",
	"�Ԏ�A�����y�їj��",
	"���̑��̏���"
};

/// �K�����x�R�[�h�h���C�����ꗗ
const char* drma::codes::o::kDomains[] = {
	"������",
	"30km/h����",
	"40km/h",
	"50km/h",
	"60km/h",
	"70km/h",
	"80km/h",
	"100km/h"
};

/// �`��f�[�^�擾�����R�[�h�h���C�����ꗗ
const char* drma::codes::o10::kDomains[] = {
	"������",
	"91�N�x�ȍ~���H�Ǘ��ғ�����",
	"�k��1/2.5���n�`�}",
	"90�N�x�ȑO���H�Ǘ��ғ�����",
	"�k��1/1���ȏ�̒n�`�}",
	"�y�n��搮�����Ǝғ�����",
	"�s�������������ƎҎ���",
	"�_���E�ѓ��E�`�p�������ƎҎ���",
	"���n����"
};

/// ������ʃR�[�h�h���C�����ꗗ
const char* drma::codes::p::kDomains[] = {
	"",
	"���E����",
	"�g���l��",
	"���哙",
	"����",
	"���̎{�݂��A���_�[�p�X",
	"������",
	"�������iETC�����j",
	"���H�ʏ̖�",
	"�������iETC�L��j",
	"�������iETC��p�j",
	"�o�C�p�X���H��",
};

/// ���n��ʃR�[�h�h���C�����ꗗ
const char* drma::codes::r::kDomains[] = {
	"",
	"�ʕ\�����n",
	"���\�����n"
};

/// ����R�[�h�h���C�����ꗗ
const char* drma::codes::s::kDomains[] = {
	"�Ӗ��Ȃ�",
	"�����E��",
	"���Ӑ�",
	"���z��"
};

/// �S����ʃR�[�h�h���C�����ꗗ
const char* drma::codes::u::kDomains[] = {
	"������",
	"�V����",
	"JR��",
	"JR���ȊO�i���ʓS���j",
	"JR���ȊO�i�H�ʓS��",
	"JR���ȊO�i���m���[���j",
	"JR���ȊO�i�V��ʁj",
	"���̑�"
};

/// �Y���L���R�[�h�h���C�����ꗗ
const char* drma::codes::y::kDomains[] = {
	"�������E�Ӗ��Ȃ�",
	"�Y������",
	"�Y������"
};
