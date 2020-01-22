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

#include <list>
#include <atlstr.h>             // CString

/**
* @brief �\���̂���s�̒l���e�v�f�Ɏ��f�[�^�^
*/
typedef std::list<CString>            AOI_LINE;

/**
* @brief �\���̂����̒l���e�v�f�Ɏ��f�[�^�^
*/
typedef std::list<CString>            AOI_COLUMN;

/**
* @brief 1�̕\��\���f�[�^�^
* @note  �\���ɍs����J�������͑��݂��Ȃ�
*/
typedef std::list<AOI_LINE> AOI_MATRIX;

/**
* @brief �����ΏۂƂ���t�B�[���h�����e�v�f�Ɋi�[����f�[�^�^
*/
typedef std::list<CString> FIELDNAMES;
