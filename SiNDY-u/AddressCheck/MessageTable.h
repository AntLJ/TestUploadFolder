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
 * @brief ���b�Z�[�W�R�[�h��`�t�@�C��
 * Message.[cpp|h]�Ń}�N���W�J�����B
 * code   : enum�œW�J����G���[�R�[�h��
 * msg_jp : ���b�Z�[�W(���{��)
 */

// TODO: �e�`�F�b�N�ŋ��ʂŎg�������ȃ��b�Z�[�W�͋��ʃ��b�Z�[�W�Ƃ��Ē�`���A������g���悤�ɂ���
// info
MESSAGE_TABLE(
iStartInit,
_T("�������J�n\n"))
MESSAGE_TABLE(
iLogPathError,
_T("���̃��O�p�X\n"))
MESSAGE_TABLE(
iTxtAddressMasterPathError,
_T("���̃e�L�X�g�Z���}�X�^�p�X\n"))
MESSAGE_TABLE(
iLayerExcept,
_T("�`�F�b�N�Ώ�:%s�Ή��ȊO\n"))
MESSAGE_TABLE(
iFinishCreateInstance,
_T("����������\n"))
MESSAGE_TABLE(
iCheckProgress,
_T("%u : �`�F�b�N��\n"))
MESSAGE_TABLE(
iStartMesh,
_T("%u : �`�F�b�N�J�n\n"))
MESSAGE_TABLE(
iStartCheck,
_T("%s : �`�F�b�N�J�n\n"))
MESSAGE_TABLE(
iFinishCheck,
_T("%s : �`�F�b�N����: %.5lf sec\n"))
MESSAGE_TABLE(
iFinishMesh,
_T("%u : �`�F�b�N����\n"))
MESSAGE_TABLE(
iInterruptCheck,
_T("�`�F�b�N�𒆒f���܂��B\n"))
MESSAGE_TABLE(
iProcessingTime,
_T("�����ɂ����������� : %f sec\n"))
MESSAGE_TABLE(
iCompleteCheck,
_T("�`�F�b�N���I�����܂����B\n"))
MESSAGE_TABLE(
iAC_CheckInit,
_T("%s �`�F�b�N������\n"))
MESSAGE_TABLE(
iAC_CheckOverlapStart,
_T("%s  �d�Ȃ�`�F�b�N�J�n\n"))
MESSAGE_TABLE(
iAC_CheckOverlapEnd,
_T("%s  �d�Ȃ�`�F�b�N����\n"))
MESSAGE_TABLE(
iAC_CheckMissingStart,
_T("%s  �����`�F�b�N�J�n\n"))
MESSAGE_TABLE(
iAC_CheckMissingEnd,
_T("%s  �����`�F�b�N����\n"))

// error
MESSAGE_TABLE(
eFailLoadSettingFile,
_T("#Error �ݒ�t�@�C���ǂݍ��݂Ɏ��s�B%s �ȉ��̂Q���m�F���ĉ������B\n�ݒ�t�@�C���̃p�X�͐������ł����H\n�E�ݒ�t�@�C������ł͂Ȃ��ł����H\n"))
MESSAGE_TABLE(
eFailDoCheck,
_T("#Error ����N���X�`�F�b�N�Ɏ��s\n"))
MESSAGE_TABLE(
eFailInitCheckControl,
_T("#Error ����N���X�������Ɏ��s\n"))
MESSAGE_TABLE(
eFailLoadMeshList,
_T("#Error ���b�V�����X�g�̓ǂݍ��݂Ɏ��s�B%s �ȉ��̂Q���m�F���ĉ������B\n���b�V�����X�g�̃p�X�͐������ł����H\n�E���b�V�����X�g����ł͂Ȃ��ł����H\n"))
MESSAGE_TABLE(
eFailLoadMasterList,
_T("#Error �}�X�^���X�g�̓ǂݍ��݂Ɏ��s�B%s �ȉ��̂Q���m�F���ĉ������B\n�}�X�^�̃p�X�͐������ł����H\n�E�}�X�^���X�g����ł͂Ȃ��ł����H\n"))
MESSAGE_TABLE(
eFailCreateInstance,
_T("#Error %s : �C���X�^���X�����Ɏ��s�����ׁA�`�F�b�N�𒆎~���܂��B�i�����O���m�F���ĉ������B\n"))
MESSAGE_TABLE(
eFailGetMeshFeature,
_T("#Error ���b�V���t�B�[�`�����擾�ł��܂��� : %u\n"))
MESSAGE_TABLE(
eFailGetMeshGeometry,
_T("#Error ���b�V���W�I���g�����擾�ł��܂���. \n"))
MESSAGE_TABLE(
eFeailGetShape,
_T("#Error ���b�V�����t�B�[�`���̃W�I���g�����擾�ł��܂���. %d\n"))
MESSAGE_TABLE(
eFailLoadLogFile,
_T("#Error ���O�t�@�C���̓ǂݍ��݂Ɏ��s\n"))
MESSAGE_TABLE(
eUnknownErrorCode,
_T("FATAL ERROR\t-1\t�s���ȃG���[�R�[�h\n"))
MESSAGE_TABLE(
eFailAnalyzeArgument,
_T("#Error ������̓G���[\n"))
MESSAGE_TABLE(
eFailRunLogInitialize,
_T("#Error ���s���G���[���O�̏������Ɏ��s���܂���.\n"))
MESSAGE_TABLE(
eFailOverlap,
_T("#Error �d�Ȃ�`�F�b�N�����Ɏ��s���܂���.\n"))
MESSAGE_TABLE(
eFailMissing,
_T("#Error �����`�F�b�N�����Ɏ��s���܂���.\n"))
MESSAGE_TABLE(
eFailDifference,
_T("#Error Difference�����Ɏ��s���܂���.\n"))
MESSAGE_TABLE(
eFailAddressPolyData,
_T("#Error �Z���f�[�^�͑��݂��Ȃ�.\n"))
MESSAGE_TABLE(
eFailAddressDBOpen,
_T("#Error �Z�����[�N�X�y�[�X�I�[�v���ł��܂���. �p�X:%s \n"))
MESSAGE_TABLE(
eFailMeshDBOpen,
_T("#Error ���b�V�����[�N�X�y�[�X�I�[�v���ł��܂���. �p�X:%s \n"))
MESSAGE_TABLE(
eFailMapDBOpen,
_T("#Error �n�}���[�N�X�y�[�X�I�[�v���ł��܂���. �p�X:%s \n"))
MESSAGE_TABLE(
eFailConstructUnion,
_T("#Error ConstructUnion�����Ɏ��s���܂���.\n"))
MESSAGE_TABLE(
eFaiBaseMeshListSetting,
_T("#Error ����layer�̒l��CITY_POLYGON�APLACENAME_REP_POINT�̂����ꂩ�Ŏw�肵�āAbase_mesh_list���w�肵�Ȃ�.\n"))
MESSAGE_TABLE(
eFaiCityMeshListSetting,
_T("#Error ����layer�̒l��ADDR_POLYGON��ADDR_POINT�̂����ꂩ�Ŏw�肵�āAcity_mesh_list���w�肵�Ȃ�.\n"))
