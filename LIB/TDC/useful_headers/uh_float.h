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
	 * @brief 非数(Not a number)を返す
	 * @return 非数
	 */
	inline double NaN()
	{
		return std::numeric_limits<double>::quiet_NaN();
	}

	/**
	 * @brief 非数か
	 * @retval true  非数である
	 * @retval false 非数でない
	 */
	inline bool isNaN(double d)
	{
		return _isnan(d) ? true : false;
	}

	/**
	 * @brief 浮動小数点値が有限かどうか
	 * @note 無限(1.#INF)、または、非数(-1#IND)の場合にfalseを返す
	 * @retval true  浮動小数点値が有限である
	 * @retval false 浮動小数点値が有限ではない
	 */
	inline bool isFinite(double d)
	{
		return 0 != _finite(d);
	}

	/**
	 * @brief ほぼ等しいか
	 * @param	d1 [in]			値1
	 * @param	d2 [in]			値2
	 * @param	threshold [in]	閾値
	 * @retval true  ほぼ等しい
	 * @retval false 等しくない
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
	 * @brief       浮動小数点値を四捨五入する
	 * @param[in]   value   Z値
	 * @param[in]   digit   小数点以下第何位で四捨五入を行うか
	 * @return      四捨五入した浮動小数点値
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
