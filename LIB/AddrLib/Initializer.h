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

#include "CodeConverter.h"
#include "YomiConverter.h"
#include "AreaCodeHelper.h"

namespace addr
{
	/**
	 * @brief	<b>�����t���Z���R�[�h�R���o�[�^�[�N���X�̏�����</b>
	 * @note	�����t���Z�����l�������Z���R�[�h���~�����ꍇ�́A�K���ŏ��ɌĂяo��
	 * @param	lpcszCodeConvDB	[in]	�R�[�h�ϊ�DB(AccessDB�p�X)
	 * @return	bool
	 */
	bool Init_CodeConverter( const _TCHAR* lpcszCodeConvDB )
	{
		return CCodeConverter::GetCodeConverter().Init(lpcszCodeConvDB);
	}

	/**
	 * @brief	<b>�����t���Z���p���~�t�^�N���X�̏�����</b>
	 * @note	�����t���Z���̃��~���~�����ꍇ�́A�K���ŏ��ɌĂяo��
	 * @param	lpcszYomiDB	[in]	���~�t�^DB(AccessDB�p�X)
	 * @return	bool
	 */
	bool Init_YomiConverter( const _TCHAR* lpcszYomiDB )
	{
		return CYomiConverter::GetYomiConverter().Init(lpcszYomiDB);
	}

	/**
	 * @brief	<b>�Z���}�X�^�w���p�[�N���X�������֐�</b>
	 * @note	�Z�����̗~�����ꍇ�́A�K���ŏ��ɌĂяo��
	 * @param	lpcszDBProp	[in]	�ڑ��v���p�e�B(�f�t�H���g�́ARelease���[�U)
	 * @return	bool
	 */
	bool Init_AreaCodeHelper( const _TCHAR* lpcszDBProp = _T("release/release@delltcp") )
	{		
		return g_cAreaCodeHelper.Connect(lpcszDBProp);
	}


}
