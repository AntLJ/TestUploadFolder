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
 * @file time_stamper.h
 * @brief 日時刻印クラスの定義
 */
#ifndef SINDY_TIME_STAMPER_H_
#define SINDY_TIME_STAMPER_H_

#include <functional>
#include <atl2/exception.h>
#include <arctl/field.h>

namespace sindy {

/**
 * @brief 日時刻印クラス
 *
 * 渡された<b>IRowBuffer</b>に現在の日時を刻む単項関数クラス。<br>
 * 日時属性フィールド名が静的に定まっている場合は@ref time_stamper_tを使う。
 *
 * @test TimeStamperTest - @ref unit_test 
 */
class time_stamper : public std::unary_function<IRowBuffer*, bool>
{
public:
	/**
	 * @brief 日時属性フィールドインデックスを設定する。
	 * @param nIndex [in] 更新日時属性フィールドインデックス。
	 */
	time_stamper(long nIndex) :
	m_nIndex(nIndex)
	{
	}

	/**
	 * @brief 現在の日時を刻印する。
	 *
	 * @param ipRowBuffer [in] 刻印対象。
	 * @retval true 刻印に成功した。
	 * @exception _com_ptr_t <b>IRowBuffer::put_Value()</b>がエラーを返した。
	 */
	bool operator()(IRowBuffer* ipRowBuffer) const
	{
		// VT_DATE型のVARIANTを用意する。
		_variant_t va;
		va.vt = VT_DATE;

		// 現在の日時をVARIANTに格納する。
		SYSTEMTIME st;
		::GetLocalTime(&st);
		::SystemTimeToVariantTime(&st, &va.dblVal);

		// IRowBufferに値を格納する。
		atl2::valid(ipRowBuffer->put_Value(m_nIndex, va));

		return true;
	}

private:
// 変数
	const long m_nIndex; ///< 日時属性フィールドインデックス。
};

/**
 * @brief 日時属性フィールド名定義式・日時刻印クラス
 *
 * @ref time_stamperを特定の日時属性フィールド名に特殊化するテンプレートクラス。
 *
 * @par テンプレート引数:
 * - @a TFieldName : 日時属性フィールド名。
 *
 * @test TimeStamperTest - @ref unit_test 
 */
template <LPCTSTR TFieldName>
class time_stamper_t : public time_stamper
{
public:
// 定義
	typedef arctl::field::index_t<TFieldName> index_type; ///< 属性フィールドインデックス型。

//@{ @name 構築/消滅
	time_stamper_t(index_type const& rIndex) : time_stamper(rIndex) {}
//@}
};

} // namespace sindy

#endif // SINDY_TIME_STAMPER_H_
