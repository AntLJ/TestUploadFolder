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
 * @brief ���b�Z�[�W�̎�ށienum�j�A���{�ꃁ�b�Z�[�W�A�p�ꃁ�b�Z�[�W�̑Ή��\
 * @note RNSMessage.h��getMsg()���L�q����ہA���{��E�p��̑Ή��𕪂���₷������ׂɍ쐬�B
 *       �p���RNSMessage.h,RNSMessage.cpp���Q�Ƃ̎��B
 *       �Q�l�Ftrunk/SiNDY-e/LQEditTool/define_linkqueue.h
 */
//RNSSindy
MESSAGE_TABLE(
	iConfigFile,
	"�ݒ�t�@�C��",
	"Config File"
)
MESSAGE_TABLE(
	eConfigFileNotFound,
	"�w�肳�ꂽ�ݒ�t�@�C���̑��݂��m�F�ł��Ȃ�",
	"Specified config file is not found."
)
MESSAGE_TABLE(
	eConfigFileLoadFailure,
	"�ݒ�t�@�C���ǂݍ��݂Ɏ��s",
	"Failed to load the Config File."
)
MESSAGE_TABLE(
	iConflictResolutionDeleteAfterUpdate,
	"�R���t���N�g����(�X�V��폜)",
	"Conflict resolution (Delete after update)"
)
MESSAGE_TABLE(
	iConflictResolutionUpdateAfterDelete,
	"�R���t���N�g����(�폜��X�V)",
	"Conflict resolution (Update after delete)"
)
MESSAGE_TABLE(
	iConflictResolutionDipliUpdate,
	"�R���t���N�g����(�X�V�̏d��)",
	"Conflict resolution (Duplication of update)"
)
MESSAGE_TABLE(
	iConflictResolution,
	"�R���t���N�g����",
	"Conflict resolution"
)
MESSAGE_TABLE(
	eStopEditOrAbortEditFailure,
	"StopEditOperation() �܂��� AbortEditoperation() �Ɏ��s",
	"Failed to StopEditOperation() or AbortEditoperation()"
)
MESSAGE_TABLE(
	eConflictDetected,
	"�R���t���N�g�����o",
	"Conflict is detected"
)
MESSAGE_TABLE(
	eReconcileFailure,
	"���R���T�C���Ɏ��s",
	"Failed to reconcile"
)
MESSAGE_TABLE(
	eStopEditFailure,
	"StopEditing() �Ɏ��s",
	"Failed to StopEditing()"
)
MESSAGE_TABLE(
	eStrangeReasonFailure,
	"���m�̗��R�Ŏ��s",
	"A strange reason goes wrong"
)
MESSAGE_TABLE(
	eNotConnected,
	"%s��%s�ɐڑ����Ă��Ȃ�",
	"%s has not connected %s."
)
MESSAGE_TABLE(
	eColumnEmpty,
	"���񂪋�",
	"Column information is empty"
)
MESSAGE_TABLE(
	eRegionEmpty,
	"���[�W��������",
	"Region is empty"
)
MESSAGE_TABLE(
	eCannotGetCursor,
	"�J�[�\�����擾�ł��Ȃ�",
	"Cannot get the cursor"
)
MESSAGE_TABLE(
	eDoesNotSort,
	"�\�[�g����Ă��Ȃ�",
	"It does not sort"
)
//RNSApp
MESSAGE_TABLE(
	iSuccess,
	" ����",
	" success"
)
MESSAGE_TABLE(
	eFailure,
	" ���s",
	" failure"
)
MESSAGE_TABLE(
	eOptionError1,
	"�I�v�V���� -",
	"Option -"
)
MESSAGE_TABLE(
	eOptionError2,
	"�͐���������܂���",
	"is not right."
)
MESSAGE_TABLE(
	eUnreglatedSettingItemName,
	"�K��O�̐ݒ荀�ږ�:",
	"The unregulated setting item name:"
)
