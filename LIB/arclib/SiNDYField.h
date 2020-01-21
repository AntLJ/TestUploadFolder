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
 * @file SiNDYField.h
 * @brief 属性フィールド取得・保持クラス定義
 * @author 地図DB制作部道路制作グループ 菊地福太郎
 * $Id$
 */
#ifndef _SINDY_FIELD_H_
#define _SINDY_FIELD_H_

#include "SiNDYFieldIndex.h"

namespace sindy {

//////////////////////////////////////////////////////////////////////////////////////////
//
// Field
//
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief 属性フィールドとインデックスを取得・格納するクラス
 *
 * 既定クラスのコンストラクタは活用せず、インデックスも当クラスで設定する。
 */
template <LPCTSTR _FieldName>
class Field : public FieldIndex<_FieldName>
{
public:
// 構築
	/// IFieldsによる構築
	Field(IFields* ipFields)
	{
		init(ipFields);
	}

	/// IFieldsスマートポインタによる構築
	Field(const IFieldsPtr& ipFields)
	{
		init(ipFields);
	}

// 取得
	operator IField* () const
	{
		return m_ipField;
	}

private:
// 構築
	/**
	 * @brief 属性フィールド、およびそのインデックスを設定する
	 *
	 * @param ipFields [in] 属性フィールド一覧
	 */
	void init(IFields* ipFields)
	{
		ipFields->FindField(CComBSTR(_FieldName), &m_nFieldIndex);
		ipFields->get_Field(m_nFieldIndex, &m_ipField);
	}

// 変数
	IFieldPtr m_ipField; ///< 属性フィールド
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// 関数
//
//////////////////////////////////////////////////////////////////////////////////////////

// field_cast

template <LPCTSTR _Name>
inline const Field<_Name>& field_cast(const Field<_Name>& rField)
{
	return rField;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// SINDY_FIELD_CLASS
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "SiNDYInheritMacro.h"


#define SINDY_FIELD_CLASS(c, n, tuple) SINDY_MULTIPLE_INHERITANCE_I(c, n, _LIST_##n tuple, _SINDY_FIELD_INHERIT, _SINDY_FIELD_CONSTRUCT)

#define _SINDY_FIELD_INHERIT(name) public sindy::Field<name>
#define _SINDY_FIELD_CONSTRUCT(name) sindy::Field<name>(_Arg1)

} // namespace sindy

#endif // _SINDY_FIELD_H_
