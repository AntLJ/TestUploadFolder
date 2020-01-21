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

#ifndef __SINDY_FUNCTIONAL_MODIFIED_H__
#define __SINDY_FUNCTIONAL_MODIFIED_H__

#include <functional>
#include "../SiNDYAttrCommunity.h"
#include "../SiNDYFieldValue.h"

namespace sindy {

/**
 * @brief 更新判定クラス
 */
template <typename T = IRow>
struct modified : public std::binary_function<_IRow*, _IRow*, bool>
{
public:
// 定義
	typedef FieldValue<kModifyDate> ModifyDate;
	typedef FieldValue<kProgModifyDate> ProgModifyDate;
	SINDY_FIELDVALUE_CLASS(FieldValue, 2, (_(ModifyDate), _(ProgModifyDate)));

	/**
	 * @brief 属性フィールドクラス拡張
	 */
	class FieldIndex : public FieldValue::FieldIndex
	{
	public:
		template <typename T>
		FieldIndex(T const& rFields) : FieldValue::FieldIndex(rFields)
		{
		}

		/**
		 * @brief 「プログラム更新日時」フィールドの有無を考慮した、サブフィールド追加関数
		 */
		template <typename T>
		static void addFieldsTo(IQueryFilter* ipQueryFilter, T const& rFields)
		{
			ipQueryFilter->AddField(CComBSTR(kModifyDate));
			if(ProgModifyDate::FieldIndex(rFields) >= 0) {
				ipQueryFilter->AddField(CComBSTR(kProgModifyDate));
			}
		}
	};

	template <typename TRow1, typename TRow2>
	bool operator()(TRow1 const& rRow1, TRow2 const& rRow2)
	{
		return operator()(rRow1, rRow2, rRow1, rRow2);
	};

	bool operator()(const ModifyDate& lsh1, const ModifyDate& rhs1, const ProgModifyDate& lhs2, const ProgModifyDate& rhs2)
	{
		return ! (equal_to<_variant_t>()(lsh1, rhs1) && equal_to<_variant_t>()(lhs2, rhs2));
	}
};

} // namespace sindy

#endif // __SINDY_FUNCTIONAL_EQUAL_TO_H__
