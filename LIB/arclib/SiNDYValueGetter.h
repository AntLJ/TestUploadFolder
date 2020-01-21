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

// SiNDYValueGetter.h: SiNDYValueGetter �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef __SINDY_FIELDVALUE_GETTER_H__
#define __SINDY_FIELDVALUE_GETTER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SiNDYFieldIndex.h"

namespace sindy {

/**
 * @defgroup getValue�l�X�ȃI�u�W�F�N�g���瑮���l���擾����֐��Q
 */
/*@{*/

/**
 * @defgroup getValue_common ��{�I�ȑ����擾�֐�
 *
 * @note �����͈ȉ��̂Ƃ���B
 * @note - ��P�����F�����擾�Ώ�
 * @note - ��Q�����F�����t�B�[���h�C���f�b�N�X
 * @note - ��R�����F�����l�擾��
 * @note - �߂�l�FHRESULT
 */
/*@{*/

inline HRESULT getValue(_IRow* ipRow, long nFieldIndex, VARIANT& rVariant)
{
	return ipRow->get_Value(nFieldIndex, &rVariant);
}

inline HRESULT getValue(IFeature* ipFeature, long nFieldIndex, VARIANT& rVariant)
{
	return ipFeature->get_Value(nFieldIndex, &rVariant);
}

inline HRESULT getValue(_IRow* ipRow, long nFieldIndex, _variant_t& rVariant)
{
	return ipRow->get_Value(nFieldIndex, &static_cast<VARIANT&>(rVariant));
}

inline HRESULT getValue(IFeature* ipFeature, long nFieldIndex, _variant_t& rVariant)
{
	return ipFeature->get_Value(nFieldIndex, &static_cast<VARIANT&>(rVariant));
}

inline HRESULT getValue(_IRow* ipRow, long nFieldIndex, CComVariant& rVariant)
{
	return ipRow->get_Value(nFieldIndex, &static_cast<VARIANT&>(rVariant));
}

inline HRESULT getValue(IFeature* ipFeature, long nFieldIndex, CComVariant& rVariant)
{
	return ipFeature->get_Value(nFieldIndex, &static_cast<VARIANT&>(rVariant));
}

template <typename _OutputType>
inline HRESULT getValue(IFeature* ipFeature, long nFieldIndex, _OutputType& rOutput)
{
	_variant_t va;
	HRESULT hr = getValue(ipFeature, nFieldIndex, va);
	rOutput = va;
	return hr;
}

template <typename _OutputType>
inline HRESULT getValue(_IRow* ipRow, long nFieldIndex, _OutputType& rOutput)
{
	_variant_t va;
	HRESULT hr = getValue(ipRow, nFieldIndex, va);
	rOutput = va;
	return hr;
}

/*@}*/

/**
 * @addtogroup getValue_custom 
 */
/*@{*/

template <LPCTSTR _FieldName, typename _Row, typename _OutputType>
inline HRESULT getValue(_Row const& rRow, _OutputType& rOutput, const FieldIndex<_FieldName>* const = 0)
{
	return getValue(rRow, fieldindex_cast<_FieldName>(rRow), rOutput);
}

/*@}*/

/**
 * @addtogroup getValue_array
 *
 * @
 */
/*@{*/

template <LPCTSTR _FieldNames, typename _Row, typename _OutputType>
HRESULT getValue(int n, _Row const& rRow, _OutputType& rOutput, const FieldIndex<_FieldNames>* const = 0)
{
	return _GetValueFunc<_FieldNames>::getValue(n, rRow, rOutput);
}

/*@}*/

/**
 * @defgroup returnValue_common �����l��_variant_t�^�̖߂�l�Ƃ��ĕԂ��֐�
 */
/*@{*/

template <typename _Row>
inline _variant_t getValue(_Row const& rRow, long nFieldIndex)
{
	_variant_t va;
	getValue(rRow, nFieldIndex, va);
	return va;
}

template <LPCTSTR _FieldName, typename _Row>
inline _variant_t getValue(_Row const& rRow, const FieldIndex<_FieldName>* const = 0)
{
	_variant_t va;
	getValue<_FieldName>(rRow, va);
	return va;
}

template <LPCTSTR _FieldNames, typename _Row>
inline _variant_t getValue(int n, _Row const& rRow, const FieldIndex<_FieldNames>* const = 0)
{
	_variant_t va;
	getValue<_FieldNames>(n, rRow, va);
	return va;
}

/*@}*/
/*@}*/

} // namespace sindy

// SiNDYValueGetter

class SiNDYValueGetter  
{
public:
// �\�z/����
	SiNDYValueGetter(_IRowPtr ipRow) :
	m_ipRow(ipRow),
	m_ipFields(getFields(ipRow))
	{
	}

// �ݒ�/�擾
	long getFieldIndex(BSTR bstrFieldName) const { return getFieldIndex(m_ipFields, bstrFieldName); }

	void getValue(long nFieldIndex, VARIANT* pValue) const { m_ipRow->get_Value(nFieldIndex, pValue); }
	void getValue(BSTR bstrFieldName, VARIANT* pValue) const { getValue(getFieldIndex(bstrFieldName), pValue); }

	_variant_t getValue(long nFieldIndex) const { return getValue(m_ipRow, nFieldIndex); }
	_variant_t getValue(BSTR bstrFieldName) const { return getValue(getFieldIndex(bstrFieldName)); }
	
	IDomainPtr getDomain(long nFieldIndex) const { return getDomain(m_ipFields, nFieldIndex); }

	bool getDomainName(BSTR bstrFieldName, BSTR* lpDomainName) const
	{
		return getDomainName(getFieldIndex(bstrFieldName), lpDomainName);
	}

	bool getDomainName(long nFieldIndex, BSTR* lpDomainName) const
	{
		if(! (nFieldIndex >= 0 && lpDomainName))
			return false;

		return getDomainName(getField(m_ipFields, nFieldIndex), getValue(nFieldIndex), lpDomainName);
	}

// �ÓI�֐�
	template <typename T>
	static IFieldsPtr getFields(T ipObject)
	{
		if(ipObject == 0)
			return 0;

		IFieldsPtr ipFields;
		ipObject->get_Fields(&ipFields);
		return ipFields;
	}

	static IFieldPtr getField(IFields* ipFields, long nFieldIndex)
	{
		if(! ipFields)
			return 0;

		IFieldPtr ipField;
		ipFields->get_Field(nFieldIndex, &ipField);
		return ipField;
	}

	static IDomainPtr getDomain(IFields* ipFields, long nFieldIndex)
	{
		return getDomain(getField(ipFields, nFieldIndex));
	}

	static IDomainPtr getDomain(IField* ipField)
	{
		if(! ipField)
			return 0;

		IDomainPtr ipDomain;
		ipField->get_Domain(&ipDomain);
		return ipDomain;
	}

	template <typename T>
	static long getFieldIndex(T ipFields, BSTR bstrFieldName)
	{
		if(ipFields == 0 || ! bstrFieldName)
			return 0;

		long nFieldIndex;
		ipFields->FindField(bstrFieldName, &nFieldIndex);
		return nFieldIndex;
	}

	template <typename T>
	static _variant_t getValue(T ipRow, BSTR bstrFieldName)
	{
		if(ipRow == 0)
			return _variant_t();

		return getValue(ipRow, getFieldIndex(getFields(ipRow), bstrFieldName));
	}

	template <typename T>
	static _variant_t getValue(T ipRow, long nFieldIndex)
	{
		if(ipRow == 0)
			return _variant_t();

		_variant_t va;
		ipRow->get_Value(nFieldIndex, &va);
		return va;
	}

	static bool getDomainName(IField* ipField, VARIANT vaValue, BSTR* lpDomainName)
	{
		return getDomainName(ICodedValueDomainPtr(getDomain(ipField)), vaValue, lpDomainName);
	}

	static bool getDomainName(ICodedValueDomain* ipCodedValueDomain, VARIANT vaValue, BSTR* lpDomainName)
	{
		if(! (ipCodedValueDomain && lpDomainName))
			return false;

		long nCodeCount = 0;
		ipCodedValueDomain->get_CodeCount(&nCodeCount);
		for(int i = 0; i < nCodeCount; i++) {
			_variant_t va;
			ipCodedValueDomain->get_Value(i, &va);
			if(va == vaValue) {
				ipCodedValueDomain->get_Name(i, lpDomainName);
				return true;
			}
		}

		return false;
	}

private:
// �ϐ�
	_IRowPtr m_ipRow;
	IFieldsPtr m_ipFields;
};

#include "detail/_GetValueFunc.h"

#endif // __SINDY_FIELDVALUE_GETTER_H__
