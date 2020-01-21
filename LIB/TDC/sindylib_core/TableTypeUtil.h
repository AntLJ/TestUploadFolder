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
 * @file TableTypeUtil.h
 * @brief <b>�e�[�u���^�C�v�g�p���[�e�B���e�B��`�t�@�C��</b>\n
 * ��`�̍ۂɁAFeatureTableType.h��ǂݍ���Ń}�N���o�R�Œ�`���܂��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _TABLETYPEUTIL_H_
#define _TABLETYPEUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TableType.h"
#include "exportimport.h"

namespace sindy {

/**
 * @brief <b>�e�[�u���^�C�v����e�[�u�����ɕϊ�����</b>\n
 * @return �e�[�u����
 */
LPCTSTR SINDYLIB_API AheTableType2TableName( sindyTableType::ECode emCode );

/**
 * @brief <b>�e�[�u��������e�[�u���^�C�v�ɕϊ�����</b>\n
 * @return �e�[�u���^�C�v
 */
sindyTableType::ECode SINDYLIB_API AheTableName2TableType( LPCTSTR lpcszTableName );

/**
 * @brief <b>�e�[�u���^�C�v��LQ�n���ǂ���</b>\n
 * @return 
 */
bool SINDYLIB_API AheIsLQTableType( sindyTableType::ECode emCode );

} // sindy

#endif // _TABLETYPEUTIL_H_
