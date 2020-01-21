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
#include <locale>

namespace uh
{
	/**
	 * @brief ������̈���(�ƕ����̕��я��̐ݒ�)��������{�ꃍ�P�[���ɂ���
	 * @note	std::ifstream�œ��{����܂ރ}���`�o�C�g������̃t�@�C�������g���������ǁA
	 *			���l��3�����ɃJ���}�ŋ�؂��Ă͍���Ƃ����ꍇ�Ɏg�p�B
	 */
	inline void setJapaneseCharacterTypeFacet()
	{
		std::locale::global(std::locale("Japanese", std::locale::ctype));
	}
}
