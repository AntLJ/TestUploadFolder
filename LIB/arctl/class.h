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
 * @file class.h
 * @brief IClass 系テンプレートライブラリ。
 */
#ifndef ARCTL_CLASS_H_
#define ARCTL_CLASS_H_

#include <atl2/exception.h>
#include <atl2/type_traits.h>

namespace arctl {

namespace detail {

template <typename T> struct class_pointer_traits {};

template <>
struct class_pointer_traits<ITable*>
{
	typedef ITable interface_type;
	typedef ITablePtr class_type;
	typedef _ICursorPtr cursor_type;
	typedef _IRowPtr object_type;
	typedef IRowBufferPtr object_buffer_type;

	static _IRowPtr create_object(ITable* ipTable)
	{
		_IRowPtr ipRow;
		atl2::valid(ipTable->CreateRow(&ipRow));
		return ipRow;
	}

	static IRowBufferPtr create_object_buffer(ITable* ipTable)
	{
		IRowBufferPtr ipRowBuffer;
		atl2::valid(ipTable->CreateRowBuffer(&ipRowBuffer));
		return ipRowBuffer;
	}

	static ITablePtr open_class(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrName)
	{
		ITablePtr ipTable;
		atl2::valid(ipFeatureWorkspace->OpenTable(bstrName, &ipTable));
		return ipTable;
	}

	static HRESULT raw_open_class(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrName, ITable** ipTable)
	{
		return ipFeatureWorkspace->OpenTable(bstrName, ipTable);
	}
};

template <>
struct class_pointer_traits<IFeatureClass*>
{
	typedef IFeatureClass interface_type;
	typedef IFeatureClassPtr class_type;
	typedef IFeatureCursorPtr cursor_type;
	typedef IFeaturePtr object_type;
	typedef IFeatureBufferPtr object_buffer_type;

	static IFeaturePtr create_object(IFeatureClass* ipFeatureClass)
	{
		IFeaturePtr ipFeature;
		atl2::valid(ipFeatureClass->CreateFeature(&ipFeature));
		return ipFeature;
	}

	static IFeatureBufferPtr create_object_buffer(IFeatureClass* ipFeatureClass)
	{
		IFeatureBufferPtr ipFeatureBuffer;
		atl2::valid(ipFeatureClass->CreateFeatureBuffer(&ipFeatureBuffer));
		return ipFeatureBuffer;
	}

	static IFeatureClassPtr open_class(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrName)
	{
		IFeatureClassPtr ipFeatureClass;
		atl2::valid(ipFeatureWorkspace->OpenFeatureClass(bstrName, &ipFeatureClass));
		return ipFeatureClass;
	}

	static HRESULT raw_open_class(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrName, IFeatureClass** ipFeatureClass)
	{
		return ipFeatureWorkspace->OpenFeatureClass(bstrName, ipFeatureClass);
	}
};

} // namespace detail

/// IClass 継承クラス特性。
template <typename T>
class class_traits : public detail::class_pointer_traits<typename atl2::interface_pointer_of<T>::type>
{
	typedef detail::class_pointer_traits<typename atl2::interface_pointer_of<T>::type> super;
public:
	typedef typename super::interface_type interface_type;
	typedef typename super::class_type class_type;
};

} // namespace arctl

#endif // ARCTL_CLASS_H_
