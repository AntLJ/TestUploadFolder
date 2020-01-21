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

/**
 * @file lq_relation.h
 * @brief �����N��֌W����N���X���C�u���������w�b�_
 *
 * @section lib_lq_relation_interface �C���^�t�F�[�X
 * - sindy::lq_relation_exist - �����N�Ɋ֘A�t�������N��̗L�����m�F����B
 * - sindy::lq_relation_edit - �����N�Ƀ����N����֘A�t������A�֘A�t������������B
 * - sindy::lq_relation_mutable - sindy::lq_relation_exist �� sindy::lq_relation_mutable �����z�p���������́B
 * - sindy::directional_lq_relation_exist - �����N�Ɋ֘A�t�������N��̗L��������ʂɊm�F����B
 * - sindy::directional_lq_relation_mutable - sindy::directional_lq_relation_exist �� sindy::lq_relation_mutable �����z�p���������́B
 *
 * @section lib_lq_relation_implement ����
 * - sindy::bidirectional_lq_relation - VICS��C���^�[�i�rVICS�Ƃ������A�����N�̕����ɑ΂��ă��j�[�N�Ɋ֘A�t�����郊���N��p�̎����B
 *
 * @section lib_lq_relation_watcher �Ď�
 * - sindy::lq_relation_edit_watcher - �����N��̊֘A�t�����X�V���ꂽ�����N��ێ�����B
 * - sindy::lq_relation_watcher - �֘A�t���̍X�V�ɂ��t���O�̕K�v���������ɊĎ�����B
 * - sindy::directional_lq_relation_watcher - sindy::lq_relation_watcher �� sindy::directional_lq_relation_exist �����������́B
 */

#ifndef SINDY_LQ_RELATION_H_
#define SINDY_LQ_RELATION_H_

#include <sindy/lq_relation/interface.h>
#include <sindy/lq_relation/directinal_interface.h>
#include <sindy/lq_relation/bidirectional.h>
#include <sindy/lq_relation/edit_watcher.h>
#include <sindy/lq_relation/watcher.h>
#include <sindy/lq_relation/directional_watcher.h>

#endif // SINDY_LQ_RELATION_H_
