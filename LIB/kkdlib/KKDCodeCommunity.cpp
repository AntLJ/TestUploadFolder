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

// KKDCodeCommunity.cpp: KKDCodeCommunity �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "KKDCodeCommunity.h"

const int kkd::regulation_road_node::kRecordId = kkd::kRegulationRoadNodeId;

/// �����_�m�[�h��ʃR�[�h�h���C�����ꗗ
const char* kkd::codes::b::kDomains[] = {
	"",
	"�������_",
	"�_�~�[�����_",
	"�_�~�[�m�[�h"
};

/// �����_�ʍs�R�[�h�h���C�����ꗗ
const char* kkd::codes::c::kDomains[] = {
	"������",
	"�ʍs��",
	"�ʍs�s��",
	"���H�����ɂ��ʍs�s��",
	"",
	"",
	"",
	"�����t���ʍs�s��"
};

/// �Ԏ�R�[�h�h���C�����ꗗ
const char* kkd::codes::car_type::kDomains[] = {
	"",
	"�ԗ�",
	"�ԗ��i�^�N�V�[�������j",
	"�ԗ��i�^�N�V�[�E��ւ������j",
	"�ԗ��i�^�N�V�[�E�ݕ��������j",
	"�ԗ��i�o�X�������j",
	"�ԗ��i�o�X�E��ւ������j",
	"�ԗ��i�o�X�E�^�N�V�[�������j",
	"�ԗ��i�o�X�E�^�N�V�[�E��ւ������j",
	"�ԗ��i�o�X�E�^�N�V�[�E����ւ������j",
	"�ԗ��i�}�C�N���������j",
	"�ԗ��i�}�C�N���E�^�N�V�[�E��ւ������j",
	"�ԗ��i����ւ������j",
	"�ԗ��i�ό��o�X�������j",
	"�ԗ��i�������H�o���ԗ��������j",
	"�ԗ��i��݂������j",
	"�ԗ��i��^�E����������j",
	"�ԗ��i��^�E����E�^�N�V�[�������j",
	"�ԗ��i��^�������j",
	"�ԗ��i��^�o�X�E����ւ������j",
	"�ԗ��i��^�o�X�E�^�N�V�[�������j",
	"�ԗ��i��ւ������j",
	"�ԗ��i��ցE�^�N�V�[�������j",
	"�ԗ��i���ʂ������j",
	"�ԗ��i��^�o�X�E���ʂ������j",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"���",
	"��ցi�P�Q�T�����ȉ��������j",
	"��ցi�V�O�O�����ȉ��������j",
	"��ցi�V�O�O�����ȏ�������j",
	"�����",
	"����ցi�P�Q�T�����ȉ��������j",
	"����ցi�V�O�O�����ȉ��������j",
	"����ցi�V�O�O�����ȏ�������j",
	"",
	"",
	"��p",
	"",
	"",
	"����",
	"",
	"����",
	"���ʁi�^�N�V�[�������j",
	"�^�N�V�[",
	"����",
	"����i�^�N�V�[�������j",
	"�ݕ�",
	"�}�C�N��",
	"�o�X",
	"��^�o�X",
	"���",
	"",
	"��ݓ�",
	"",
	"���",
	"",
	"��^",
	"��^�i�}�C�N���������j",
	"��^�i�o�X�������j",
	"��^�i��^�o�X�������j",
	"",
	"",
	"",
	"�ő�ύڗʂQ���ȏ�̉ݕ�",
	"�ő�ύڗʂQ�D�T���ȏ�̉ݕ�",
	"�ő�ύڗʂR���ȏ�̉ݕ�",
	"�ő�ύڗʂQ�D�T���ȏ�̉ݕ�",
	"�ő�ύڗʂS���ȏ�̉ݕ�",
	"�ő�ύڗʂS�D�T���ȏ�̉ݕ�",
	"",
	"",
	"",
	"",
	"���d�ʂS���ȏ�",
	"���d�ʂT���ȏ�",
	"���d�ʂP�S���ȏ�",
	"���d�ʂP�U���ȏ�",
	"���d�ʂQ�O���ȏ�",
	"���d�ʂW���ȏ�",
	"���d�ʂW���ȏ�",
	"",
	"",
	"�댯���ύڎԗ�"
};

/// �Ԏ�R�[�h�h���C�����ꗗ
const char* kkd::codes::weekday::kDomains[] = {
	"",
	"���E�x��",
	"���E�x��������",
	"�x��",
	"�x��������",
	"�����i���`�y�j",
	"���j",
	"���j",
	"�Ηj",
	"���j",
	"�ؗj",
	"���j",
	"�y�j",
	"�y���E�x��",
	"�y���E�x��������",
	"�S�j��",
	"���j������",
	"���x�̑O��"
};

/// ��ʋK�����[���R�[�h�h���C�����ꗗ
const char* kkd::codes::regulation_lane::kDomains[] = {
	"������",
	"�Ԑ��P",
	"�Ԑ��Q",
	"�Ԑ��R",
	"�Ԑ��S",
	"�Ԑ��T",
	"�Ԑ��U",
	"�S�Ԑ�"
};
