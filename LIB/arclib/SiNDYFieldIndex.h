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

#ifndef __SINDY_FIELD_INDEX_H__
#define __SINDY_FIELD_INDEX_H__

namespace sindy {

//////////////////////////////////////////////////////////////////////////////////////////
//
// FieldIndex
//
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief 属性フィールドインデックスを取得・格納するクラス
 */
template <LPCTSTR TFieldName>
class FieldIndex
{
public:
// 構築
	template <typename TInterface>
	FieldIndex(TInterface* ipInterface) { init(ipInterface); }

	template <typename TIIID>
	FieldIndex(const _com_ptr_t<TIIID>& rInterface) { init(static_cast<TIIID::Interface*>(rInterface)); }

	FieldIndex(_IRow* ipRow) { init(static_cast<IFields*>(getFieldsFrom(ipRow))); }
	FieldIndex(IFeature* ipFeature) { init(static_cast<IFields*>(getFieldsFrom(ipFeature))); }
	FieldIndex(const _IRowPtr& rRow) { init(static_cast<IFields*>(getFieldsFrom(static_cast<_IRow*>(rRow)))); }
	FieldIndex(const IFeaturePtr& rFeatire) { init(static_cast<IFields*>(getFieldsFrom(static_cast<IFeature*>(rFeatire)))); }
	
	FieldIndex(const FieldIndex& rFieldIndex) : m_nFieldIndex(rFieldIndex.m_nFieldIndex) { }

// 取得
	operator long () const
	{
		return m_nFieldIndex;
	}

	static LPCTSTR fieldName()
	{
		return TFieldName;
	}


// 静的関数
	template <typename TQueryFilter>
	static void addFieldsTo(TQueryFilter const& ipQueryFilter)
	{
		ipQueryFilter->AddField(CComBSTR(fieldName()));
	}

	template <typename TQueryFilter>
	static void putSubFields(TQueryFilter const& ipQueryFilter)
	{
		ipQueryFilter->put_SubFields(CComBSTR(fieldName()));
	}

	template <typename TStream>
	static void outputSubFields(TStream& oss)
	{
		std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
		oss.imbue(loc);
		oss << fieldName();
	}

protected:
// 構築
	FieldIndex() :
	m_nFieldIndex(-1)
	{
	}

// 静的関数
	template <typename TFieldsHolder>
	static IFieldsPtr getFieldsFrom(TFieldsHolder* ipFieldsHolder)
	{
		IFieldsPtr ipFields;
		ipFieldsHolder->get_Fields(&ipFields);
		return ipFields;
	}

// 変数
	long m_nFieldIndex;

private:
// 構築
	template <typename TFields>
	void init(TFields* ipFields)
	{
		ipFields->FindField(CComBSTR(TFieldName), &m_nFieldIndex);
	}
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// FieldIndexChain
//
//////////////////////////////////////////////////////////////////////////////////////////

template <LPCTSTR TFieldName, typename TFieldIndex>
class FieldIndexChain : public sindy::FieldIndex<TFieldName>, public TFieldIndex
{
public:
	template<typename TFields>
	FieldIndexChain(TFields const& rFields) :
	sindy::FieldIndex<TFieldName>(rFields),
	TFieldIndex(rFields)
	{
	}
};

class FieldIndexTerminater
{
public:
	template<typename TFields>
	FieldIndexTerminater(TFields const& rFields)
	{
	}
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// 関数
//
//////////////////////////////////////////////////////////////////////////////////////////

// fieldindex_cast

/**
 * @brief FieldIndexキャスト関数
 *
 * FieldIndex多重継承クラスから、しかるべきFieldIndexを抜き出す。
 *
 * @note FieldIndex<TName>()で複製するほうが実用的かもしれない。
 */
template <LPCTSTR TName>
inline const FieldIndex<TName>& fieldindex_cast(const FieldIndex<TName>& rFieldIndex)
{
	return rFieldIndex;
}

} // namespace sindy

#include "SiNDYFieldIndexMacro.h"

#endif // __SINDY_FIELD_INDEX_H__
