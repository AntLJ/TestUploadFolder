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
#include <limits>

namespace uh
{
	/**
	 * @brief ��(Not a number)��Ԃ�
	 * @return ��
	 */
	inline double NaN()
	{
		return std::numeric_limits<double>::quiet_NaN();
	}

	/**
	 * @brief �񐔂�
	 * @retval true  �񐔂ł���
	 * @retval false �񐔂łȂ�
	 */
	inline bool isNaN(double d)
	{
		return _isnan(d) ? true : false;
	}

	/**
	 * @brief ���������_�l���L�����ǂ���
	 * @note ����(1.#INF)�A�܂��́A��(-1#IND)�̏ꍇ��false��Ԃ�
	 * @retval true  ���������_�l���L���ł���
	 * @retval false ���������_�l���L���ł͂Ȃ�
	 */
	inline bool isFinite(double d)
	{
		return 0 != _finite(d);
	}

	/**
	 * @brief �قړ�������
	 * @param	d1 [in]			�l1
	 * @param	d2 [in]			�l2
	 * @param	threshold [in]	臒l
	 * @retval true  �قړ�����
	 * @retval false �������Ȃ�
	 */
	inline bool nearly_equal(double d1, double d2, double threshold)
	{
		if(d1 >= d2)
		{
			return (d1 - d2) <= threshold;
		}
		else
		{
			return (d2 - d1) <= threshold;
		}
	}

	/**
	 * @brief       ���������_�l���l�̌ܓ�����
	 * @param[in]   value   Z�l
	 * @param[in]   digit   �����_�ȉ��扽�ʂŎl�̌ܓ����s����
	 * @return      �l�̌ܓ��������������_�l
	 */
	inline double roundingOff(double value, long digit)
	{
		long operation = std::pow(10.0, digit - 1);

		value *= operation;
		value += 0.5;
		long lValue = static_cast<long>(value);
		double ret = static_cast<double>(lValue);
		ret /= operation;

		return ret;
	}
}
