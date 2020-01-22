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

#include <regex>
#include <boost/format.hpp>

namespace util
{
	/**
	 *  @brief �����񓯎m��������v���Ă��邩��Ԃ�
	 *         �w�肳��镶���̓��C�h�����ł��邱�Ƃ�O��Ƃ���
	 *  @param[in] target_str   ��������鑤�̕�����
	 *  @param[in] search_word  �������镶����
	 *  @retval true   ������v���Ă���
	 *  @retval false  ������v���Ă��Ȃ�
	 */
	bool partialMatch(const CString& target_str, const CString& search_word);
}

