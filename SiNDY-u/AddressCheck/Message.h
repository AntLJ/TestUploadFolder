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

#include <tchar.h>

namespace message
{

/**
 * @brief ���b�Z�[�W�R�[�h
 */
enum emCheck_MessageCode
{
	// �}�N�����g����enum��`
#define MESSAGE_TABLE( code, japanese ) code,
#include "MessageTable.h"
#undef MESSAGE_TABLE
};

/**
 * @brief �w��R�[�h�ɑ΂��郁�b�Z�[�W���擾����
 * @param[in] code �R�[�h
 * @return ���b�Z�[�W
 */
const TCHAR* getMsg( emCheck_MessageCode code );
	
} // namespace message
