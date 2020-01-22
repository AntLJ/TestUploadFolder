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
 * @file ErrorCode.h
 * @brief �G���[�R�[�h��`�t�@�C��
 * ErrorLog.[cpp|h]�Ń}�N���W�J�����B
 * code   : enum�œW�J����G���[�R�[�h��
 * value  : �G���[�R�[�h
 * level  : ErrorDef.h �ɂ��� emErrLevel
 * msg_jp : �G���[���b�Z�[�W(���{��)
 */

ERROR_CODE(
kAC_AddrPolygonCheckOverLap,
0x01004001,
kError,
�Z���|���S�����s�撬���|���S���Əd�Ȃ��Ă��Ȃ�.)

ERROR_CODE(
kAC_AddrPolygonCheckMissing,
0x01004002,
kError,
�Z���|���S���ɔ���������.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError1,
0x01002001,
kError,
�����ڊE��������11���Z���R�[�h���u0�v�ł͂Ȃ�.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError2,
0x01002002,
kError,
�����ڊE�������ȊO��11���Z���R�[�h���u0�v.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError3,
0x01002003,
kError,
�����ڊE�������Ȃ̂ɊX�敄�����t���Ă���.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError4,
0x01002004,
kError,
�X�敄���E�g�����́i���́j�̗����������Ă���.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError5,
0x01002005,
kError,
�X�敄�����O.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError6,
0x01002006,
kError,
�X�敄�����R���ȏ�.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError7,
0x01002007,
kError,
�Z���\���E�n�Ԗ������Ȃ̂ɊX�敄���������Ă���.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError8,
0x01002008,
kError,
�Z���\��������/�n�Ԑ��������ςȂ̂ɊX�敄�����Ȃ�.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError9,
0x01002009,
kError,
�n�Ԑ��������ςȂ̂Ɋg�����̂�����.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError10,
0x01002010,
kError,
�g�����̂P�A2�̖��̂��قȂ�.)

ERROR_CODE(
kAC_AddrPolygonSameAddrCode,
0x01002011,
kError,
�Z���R�[�h(����)�ƏZ���R�[�h(�ʏ�)������.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError13,
0x01002012,
kError,
�Z���\��������/�n�Ԑ����ςȂ̂ɒʏ̊֘A�t�B�[���h�ɒl���i�[����Ă���.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError11,
0x01001002,
kError,
�Z��������ʃR�[�h�ɕs���Ȓl�������Ă���.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeError12,
0x01003001,
kError,
�Z��������ʃR�[�h���قȂ�|���S�������݂���.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeGeoError1,
0x01003002,
kError,
��ђn������.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrTypeGeoError2,
0x01003003,
kError,
���ꑮ���̃|���S�����אڂ��Ă���.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrCodeError1,
0x01004004,
kError,
�Z���}�X�^�ɑ��݂��Ȃ��Z���R�[�h�i���́j.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrCodeError2,
0x01004005,
kError,
�Z���}�X�^�ɑ��݂��Ȃ��Z���R�[�h�i�ʏ́j.)

ERROR_CODE(
kAC_AddrPolygonCheckAddrCodeError3,
0x01004006,
kError,
�p�~�ɂȂ����Z���R�[�h(����).)

ERROR_CODE(
kAC_AddrPolygonCheckAddrCodeError4,
0x01004007,
kError,
�p�~�ɂȂ����Z���R�[�h(�ʏ�).)

ERROR_CODE(
kAC_AddrPolygonNoPoint,
0x01004009,
kWarn,
����Z���R�[�h�����n����\�|�C���g�����݂��Ȃ�.)

ERROR_CODE(
kAC_AddrPolygonExtNameExistMaster,
0x01004011,
kError,
�g���X�敄��������11�����x���ő���.)

ERROR_CODE(
kAC_AddrPolygonNoPolygonInMesh,
0x01004003,
kError,
���b�V�����ɍs���E�|���S�����Ȃ�.)

ERROR_CODE(
kAC_AddrPolygonExistProhibitChar,
0x01001001,
kError,
�g�����̂ɋ֑�����.)

ERROR_CODE(
kAC_AddrPolygonAddrCode2Villa,
0x01001003,
kError,
�ʑ��n�̏Z���R�[�h���ʏ̂ɐݒ肳��Ă���.)

// city_polygon error message
ERROR_CODE(
kAC_CityPolygonSameCityAdjoin,
0x02003001,
kError,
���ꑮ���̃|���S�����אڂ��Ă���.)

ERROR_CODE(
kAC_CityPolygonAbolishedCode,
0x02004001,
kError,
�p�~�ɂȂ����R�[�h.)

ERROR_CODE(
kAC_CityPolygonCityNotInMaster,
0x02004002,
kError,
�s�撬���R�[�h���Z���}�X�^�ɂȂ�.)

ERROR_CODE(
kAC_CityPolygonCityMissing,
0x02004003,
kError,
�s�撬���|���S���ɔ���������.)

ERROR_CODE(
kAC_CityPolygonNoPolygonInMesh,
0x02004004,
kError,
���b�V�����Ɏs�撬���|���S�����Ȃ�.)
