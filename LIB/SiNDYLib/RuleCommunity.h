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
 * @file RuleCommunity.h
 * @brief <b>���[���n�e�[�u���t�B�[���h�E�h���C����`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _RULECOMMUNITY_H_
#define _RULECOMMUNITY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"


const int _AUTOVALUE_MAX_LEN		= 255;				//!< �����쐬�����l�̍ő啶����
const int _PURPOSE_MAX_LEN			= 64;				//!< ��ƖړI���t�B�[���h��
const int _MXD_MAX_LEN				= 255;				//!< MXD�t�@�C�����t�B�[���h���i_MAX_PATH�Ɠ����ɂ���ׂ��j
const int _CATEGORY_MAX_LEN			= 50;				//!< �J�e�S���t�B�[���h��
const int _LAYERFILEPATH_MAX_LEN	= 512;				//!< ���C���t�@�C���p�X�t�B�[���h���i_MAX_PATH�Ɠ����ɂ���ׂ��j

const int _TABLENAME_MAX_LEN		= 30;				//!< �e�[�u�����ő啶���񒷁iOracle�̐����j
const int _OWNERNAME_MAX_LEN		= 30;				//!< �I�[�i�[���ő啶���񒷁iOracle�̐����j
const int _OWNERTABLENAME_MAX_LEN	= 60;				//!< _TABLENAME_MAX_LEN + _OWNERNAME_MAX_LEN
const int _NAMESTRING_MAX_LEN		= 128;				//!< ���L�ő啶����

#endif // _RULECOMMUNITY_H_
