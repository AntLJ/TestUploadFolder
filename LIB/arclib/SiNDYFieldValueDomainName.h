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
 * @file SiNDYFieldValueDomainName.h
 * @brief �����l�h���C�����擾�֐��E�ێ��N���X��`
 * @author �n�}DB���암���H����O���[�v �e�n�����Y
 * $Id$
 */
#ifndef _SINDY_FIELDVALUE_DOMAINNAME_H_
#define _SINDY_FIELDVALUE_DOMAINNAME_H_

#include "SiNDYField.h"
#include "SiNDYFieldValue.h"

namespace sindy {

//////////////////////////////////////////////////////////////////////////////////////////
//
// getDomainName
//
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief �����l�h���C������BSTR�^�Ŏ擾����
 *
 * @param ipCodedValueDomain [in] ICodedValueDomain�B
 * @param rValue [in] �l�B
 * @param lpDomainName [out] �h���C�����iBSTR�j�B
 * @retval S_OK �擾�����B
 * @retval <>S_OK �擾���s�B
 */
inline HRESULT getDomainName(ICodedValueDomain* ipCodedValueDomain, const VARIANT& rValue, BSTR* lpDomainName)
{
	if(! (ipCodedValueDomain && lpDomainName))
		return E_POINTER;

	long nCodeCount = 0;
	ipCodedValueDomain->get_CodeCount(&nCodeCount);
	for(long i = 0; i < nCodeCount; i++) {
		_variant_t va;
		ipCodedValueDomain->get_Value(i, &va);
		if(va == rValue) {
			ipCodedValueDomain->get_Name(i, lpDomainName);
			return S_OK;
		}
	}

	return S_FALSE;
}

/**
 * @brief �����l�h���C������CString�^�Ŏ擾����
 *
 * @param ipCodedValueDomain [in] ICodedValueDomain�B
 * @param rValue [in] �l�B
 * @param rDomainName [out] �h���C�����iCString�j�B
 * @retval S_OK �擾�����B
 * @retval <>S_OK �擾���s�B
 */
inline HRESULT getDomainName(ICodedValueDomain* ipCodedValueDomain, const VARIANT& rValue, CString& rDomainName)
{
	CComBSTR bstrDomainName;
	HRESULT hr = getDomainName(ipCodedValueDomain, rValue, &bstrDomainName);
	rDomainName = bstrDomainName;
	return hr;
}

/**
 * @brief �����l�h���C�������擾����CString�^�Ŏ擾����
 *
 * @param ipField [in] �R�[�h�l�h���C�������t�B�[���h�B
 * @param rValue [in] �l�B
 * @param rDomainName [out] �h���C�����iCString�j�B
 * @retval S_OK �擾�����B
 * @retval <>S_OK �擾���s�B
 */
inline HRESULT getDomainName(IField* ipField, const VARIANT& rValue, CString& rDomainName)
{
	IDomainPtr ipDomain;
	ipField->get_Domain(&ipDomain);
	return getDomainName(ICodedValueDomainPtr(ipDomain), rValue, rDomainName);
}

/**
 * @brief �����l�h���C������CString�^�ŕԂ�
 *
 * @param ipCodedValueDomain [in] ICodedValueDomain�B
 * @param rValue [in] �l�B
 * @return �R�[�h�l�h���C�����B
 */
inline CString getDomainName(ICodedValueDomain* ipCodedValueDomain, const VARIANT& rValue)
{
	CComBSTR bstrDomainName;
	getDomainName(ipCodedValueDomain, rValue, &bstrDomainName);
	return bstrDomainName;
}

/**
 * @brief �����l�h���C������CString�^�ŕԂ�
 *
 * @param ipField [in] �R�[�h�l�h���C�������t�B�[���h�B
 * @param rValue [in] �l�B
 * @return �R�[�h�l�h���C�����B
 */
inline CString getDomainName(IField* ipField, const VARIANT& rValue)
{
	IDomainPtr ipDomain;
	ipField->get_Domain(&ipDomain);
	return getDomainName(ICodedValueDomainPtr(ipDomain), rValue);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// FieldValueDomainName
//
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief �����l�h���C�������擾�E�i�[����N���X
 */
template <LPCTSTR _FieldName>
class FieldValueDomainName
{
public:
	/**
	 * @brief _FieldName�Ɋւ���ICodedValueDomain�A����ђl�����I�u�W�F�N�g����h���C�������擾���A�C���X�^���X���\�z
	 *
	 * @param rRow [in] Field��FieldValue�����o�����Ƃ̂ł���I�u�W�F�N�g�B
	 */
	template<typename _Row>
	FieldValueDomainName(_Row const& rRow) :
	m_strDomainName(getDomainName(rRow, rRow))
	{
	}

// �擾���Z
	operator const CString& () const
	{
		return m_strDomainName;
	}

	operator const LPCTSTR () const
	{
		return static_cast<LPCTSTR>(m_strDomainName);
	}

private:
// �ÓI�擾
	/**
	 * @brief _Row����Field��FieldValue����ӂɎ��o�����ꍇ�̏���
	 *
	 * @param rField [in] �t�B�[���h�ێ��ҁB
	 * @param rValue [in] �l�ێ��ҁB
	 * @return �h���C�����B
	 */
	static CString getDomainName(const sindy::Field<_FieldName>& rField, const sindy::FieldValue<_FieldName>& rValue)
	{
		return sindy::getDomainName(static_cast<IField*>(rField), static_cast<const _variant_t&>(rValue));
	}

// �ϐ�
	CString m_strDomainName; ///< �h���C����
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// domainname_cast
//
//////////////////////////////////////////////////////////////////////////////////////////

template <LPCTSTR _FieldName>
const FieldValueDomainName<_FieldName>& domainname_cast(const FieldValueDomainName<_FieldName>& rDomainName)
{
	return rDomainName;
}

} // namespace sindy;

#endif // _SINDY_FIELDVALUE_DOMAINNAME_H_
