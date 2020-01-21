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

#include <sstream>
#include <stdexcept>

namespace uh
{
	/**
	 * @brief ���W�b�N��O�̃��b�Z�[�W���쐬����
	 * @param file [in] �t�@�C����
	 * @param line [in] �s�ԍ�
	 * @param func [in] �֐���
	 */
	inline std::string makeLogicErrorMessage(const char* file, int line, const char* func)
	{
		std::ostringstream oss;
		oss << "THROW_LOGIC_ERROR @ " << func << " @ " << file << "(" << line << ")";
		return oss.str();
	}
}

/**
 * @brief �����I�Ƀ��W�b�N��O�𑗏o����
 * @note ���B�����Ȃ������������߂ɊJ������
 */
#define THROW_LOGIC_ERROR() throw std::runtime_error(uh::makeLogicErrorMessage(__FILE__, __LINE__, __FUNCTION__))

/**
 * @brief ����false�̂Ƃ��Ƀ��W�b�N��O�𑗏o����
 * @note Release�r���h�ł��@�\����assert�Ƃ��ĊJ������
 * @param expression [in] �]�����鎮
 */
#define ASSERT_LOGIC(expression) (void)( (!!(expression)) || ((THROW_LOGIC_ERROR()), 0) )
