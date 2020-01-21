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
 * @file oid.h
 * @brief for field.h
 */
#ifndef ARCTL_FIELD_OID_H_
#define ARCTL_FIELD_OID_H_

#include <boost/operators.hpp>
#include <atl2/exception.h>
#include <arctl/detail/multi_inherit.h>

namespace arctl {
namespace field {

/**
 * @ingroup field_index_group �����l�N���X
 * @brief OID�����C���f�b�N�X�N���X�B
 */
class oid_index_t : public arctl::detail::multi_inherit::base_t
{
public:
//@{ @name �\�z/����
	oid_index_t() : m_n(-1) {}
	oid_index_t(const oid_index_t& rIndex) : m_n(rIndex.m_n) {}
	oid_index_t(IClass* ipClass) { operator=(ipClass); }
//@}

//@{ @name �擾
	long index() const { return m_n; }
//@}

//@{ @name ���Z
	operator long() const { return index(); }
	bool operator!() const { return m_n == -1; }

	oid_index_t& operator=(IClass* ipClass)
	{
		atl2::valid(ipClass->FindField(bname(ipClass), &m_n));
		return *this;
	}
//@}
	
//@{ @name �ÓI�֐�
	/// �����t�B�[���h����BSTR�^�Ŏ擾����B
	static CComBSTR bname(IClass* ipClass)
	{
		CComBSTR bstrName;
		atl2::valid(ipClass->get_OIDFieldName(&bstrName));
		return bstrName;
	}

	/// �����t�B�[���h�����o�͂���B
	template <typename TOutputIterator>
	static void copy_name(IClass* ipClass, TOutputIterator result)
	{
		*result = COLE2CT(bname(ipClass)).m_psz;
		++result;
	}
//@}

private:
// �ϐ�
	long m_n; ///< �����t�B�[���h�C���f�b�N�X�B
};

/**
 * @ingroup field_value_group
 * @brief �I�u�W�F�N�gID�N���X�B
 *
 * �I�u�W�F�N�gID���擾�E�ێ�����N���X�B
 *
 * @par �����l�擾�\�I�u�W�F�N�g:
 * _IRow
 * @par �����l�̎擾:
 * _IRow::get_OID()
 * @par �����l�̏����o��:
 * �s�B write() �� false ��Ԃ��B
 *
 * @test @ref unit_test FieldOIDTest
 */
class oid_t : public arctl::detail::multi_inherit::base_t, boost::totally_ordered<oid_t>
{
public:
// ��`
	typedef oid_index_t index_type;
	typedef long value_type;

///@name �\�z/����
//@{
	oid_t() : m_nOID(0) {}
	oid_t(long nOID) : m_nOID(nOID) {}
	oid_t(_IRow* ipRow) { atl2::valid((ipRow->get_OID)(&m_nOID)); }
	oid_t(_IRow* ipRow, const index_type&) { atl2::valid((ipRow->get_OID)(&m_nOID)); }
//@}

///@name �擾
//@{
	const long& oid() const { return m_nOID; }
//@}

///@name �����l�N���X�݊�
//@{
	long& value() { return m_nOID; }
	const long& value() const { return oid(); }
	bool write(IRowBuffer*, const index_type&) const { return false; }
//@}

///@name ���Z�q
//@{
	bool operator==(const oid_t& rhs) const { return m_nOID == rhs.m_nOID; }
	bool operator<(const oid_t& rhs) const { return m_nOID < rhs.m_nOID; }

	oid_t& operator=(long nOID)
	{
		m_nOID = nOID;
	}

	oid_t& operator=(_IRow* ipRow)
	{
		atl2::valid((ipRow->get_OID)(&m_nOID));
		return *this;
	}
//@}

private:
// �ϐ�
	long m_nOID;
};

} // namespace field
} // namespace arctl

#endif // ARCTL_FIELD_OID_H_
