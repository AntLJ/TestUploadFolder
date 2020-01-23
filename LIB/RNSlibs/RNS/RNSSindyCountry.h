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
 * @file
 * @brief �O���[�o��PJ�Œǉ��B���̏��������B
 */

#pragma once
#include <map>
#include <set>

namespace country
{
	/**
	 * @brief ���̎��
	 * @note �uMPA���^�C�v�Ƃ��ėǂ����͍���̓������悾���A�Ƃ肠���������i�K�ł͂����Ȃ̂ł������Ă����B
	 */
	enum class country_type
	{
		japan,     //!< ���{
		aseanL,    //!< �����ʍs��ASEAN����
		aseanR,    //!< �E���ʍs��ASEAN����
	};

	typedef std::set< country_type > CountryTypes;

	/**
	 * @brief country_type�S��ނ̏W����Ԃ�
	 * @return country_type�W��
	 */
	CountryTypes allCountryTypes();

} // namespace country
