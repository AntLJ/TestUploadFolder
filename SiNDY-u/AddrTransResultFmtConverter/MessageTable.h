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
eCityFileStartConvert,
L"city.txt�t�@�C�������J�n\n")
MESSAGE_TABLE(
eCityFileEndConvert,
L"city.txt�t�@�C����������\n")
MESSAGE_TABLE(
eAddrFileStartConvert,
L"�A�h���X�t�@�C�������J�n\n")
MESSAGE_TABLE(
eAddrFileEndConvert,
L"�A�h���X�t�@�C����������\n")

// error
MESSAGE_TABLE(
eFailInitConvertControl,
L"#Error ����N���X�������Ɏ��s\n")
MESSAGE_TABLE(
eFailAnalyzeArgument,
L"#Error ������̓G���[\n")
MESSAGE_TABLE(
eFailAnalyzeConvMode,
L"--conv_mode�ւ̎w�肪�����ł���\n")
MESSAGE_TABLE(
eFailAnalyzeTransResult,
L"--trans_result�ւ̎w�肪�����ł���\n")
MESSAGE_TABLE(
eFailAnalyzeOut,
L"--out�ւ̎w�肪�����ł���\n")
MESSAGE_TABLE(
eFailAnalyzeRunLog,
L"--run_log�ւ̎w�肪�����ł���\n")
MESSAGE_TABLE(
eFailAnalyzeErrLog,
L"--err_log�ւ̎w�肪�����ł���\n")
MESSAGE_TABLE(
eFailRunLogInitialize,
L"#Error ���s�����O�̏������Ɏ��s���܂���\n")
MESSAGE_TABLE(
eFailErrLogInitialize,
L"#Error �G���[���O�̏������Ɏ��s���܂���\n")
MESSAGE_TABLE(
eFailReadFile,
L"�̓ǂݍ��݂����s���܂���\n")
MESSAGE_TABLE(
eFailCreateFile,
L"�̍쐬�����s���܂���\n")
MESSAGE_TABLE(
eFailGetFile,
L"�t�@�C��������܂���!\n")
MESSAGE_TABLE(
eFileColumnErr,
L"�v�̃t�B�[���h���S�đ��݂��Ă��܂�")
MESSAGE_TABLE(
eFindDifference,
L"����8���R�[�h���Ŗ|�󌋉ʂɍ��ق�����܂�")
MESSAGE_TABLE(
eFailOpenFile,
L"�t�@�C���I�[�v�������s���܂���\n")