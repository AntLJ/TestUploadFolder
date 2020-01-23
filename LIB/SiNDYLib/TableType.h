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
 * @file TableType.h
 * @brief <b>�e�[�u���^�C�v��`�t�@�C��</b>\n
 * ���̃t�@�C���ł́A�e�[�u���^�C�v��enum�Œ�`���Ă��܂��B
 * ��`�̍ۂɁAFeatureTableType.h��ǂݍ���Ń}�N���o�R�Œ�`���܂��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _TABLETYPE_H_
#define _TABLETYPE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace sindy {
	namespace sindyTableType {
		enum ECode {
#define FEATURETABLE_TYPE(name_space,table_name,class_name) name_space,
#include "FeatureTableType.h"
#undef FEATURETABLE_TYPE
		};
	} // sindyTableType
} // sindy

#endif // _TABLETYPE_H_
