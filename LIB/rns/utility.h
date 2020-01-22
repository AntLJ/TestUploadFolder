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

#ifndef RNS_UTILITY_H_
#define RNS_UTILITY_H_

#include <utility>

namespace rns {

/**
 * @brief std::pair �\�z�⏕�֐��N���X�B
 *
 * std::make_pair() ���N���X�ɂ����悤�Ȃ��́B
 *
 * @attention
 * std::pair �̊e�v�f�ɑ΂��� explicit �錾�͖��������B
 *
 * @par �e���v���[�g����:
 * - TPairType : �񍀃R���X�g���N�^�����݂��A first_type �� second_type ����`���ꂽ�^�B
 * - TFirstType : �f�t�H���g�� TPairType::first_type �BTPairType::first_type ��߂�l�Ƃ���P���֐��i�֐��I�u�W�F�N�g�s�j��ATPairType::first_type �ւ̈Öق̌^�ϊ����\�Ȍ^�ł��B
 * - TSecondType : �f�t�H���g�� TPairType::second_type �BTPairType::second_type ��߂�l�Ƃ���P���֐��i�֐��I�u�W�F�N�g�s�j��ATPairType::second_type �ւ̈Öق̌^�ϊ����\�Ȍ^�ł��B
 */
template <typename TPairType, typename TFirstType = TPairType::first_type, typename TSecondType = TPairType::second_type>
class pair_maker : public std::binary_function<TFirstType, TSecondType, TPairType>
{
public:
	template <typename T>
	TPairType operator()(T const& rT) const
	{
		return TPairType(TFirstType(rT), TSecondType(rT));
	}

	template <typename T1, typename T2>
	TPairType operator()(T1 const& first, T2 const& second) const
	{
		return TPairType(TFirstType(first), TSecondType(second));
	}
};

} // namespace rns

#endif // RNS_UTILITY_H_
