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

#ifndef __SINDY_FUNCTIONAL_EQUAL_TO_H__
#define __SINDY_FUNCTIONAL_EQUAL_TO_H__

#include <functional>

namespace sindy {

/////////////////////////////////////////////////////////////////////////////
// ��`
/////////////////////////////////////////////////////////////////////////////

/// @brief SiNDY��equal_to
template <typename TInterface>
struct equal_to
{
};

/**
 * @brief �v���p�e�B�̈�v�������Đ^�Ɛ����֐��I�u�W�F�N�g
 *
 * TInterface �ɂ͈ȉ��̌^���g�p�\�B
 * - IUnknown ����т��̔h���N���X�A�܂��͂��̃|�C���^�B
 * - _com_ptr_t ����т��̔h���N���X�B
 */
template <typename TInterface>
struct data_equal
{
public:
	typedef interface_to_pointer<TInterface>::type first_argument_type;
	typedef bool result_type;

	/**
	 * @brief �v���p�e�B�̈�v������s��
	 *
	 * @warning TData �̓f�X�g���N�^�Ŏ��Ȃ�j���ł��Ȃ���΂Ȃ�Ȃ��B
	 */
	template <typename TData>
	bool operator()(first_argument_type pInterface, const TData& rData) const
	{
		if(pInterface == 0)
			return false;

		TData data;
		traits<TInterface>::accessor<TData>::get(pInterface, &data);
		return data == rData;
	}
};

// ��`
/////////////////////////////////////////////////////////////////////////////

/// @brief _variant_t�^equal_to���ꉻ
template <>
struct equal_to<_variant_t> : public std::binary_function<_variant_t, VARIANT, bool>
{
	bool operator()(const _variant_t& lhs, const VARIANT& rhs) const;
};

/////////////////////////////////////////////////////////////////////////////
// ����
/////////////////////////////////////////////////////////////////////////////

#pragma warning(push)
#pragma warning(disable : 4310)
/**
 * @brief �����l���r����
 *
 * ��{��_variant_t::operator==()�B<br>
 * ��������VARIANT�^��VT_UNKNOWN�̏ꍇ�Ɍ���AIRelationalOperator::Equals()�ɂ��`���r���s���B
 *
 * @param lhs [in] �����l1�B
 * @param rhs [in] �����l2�B
 * @retval true ��v�B
 * @retval false �s��v�B
 */
inline bool equal_to<_variant_t>::operator()(const _variant_t& lhs, const VARIANT& rhs) const
{
	if(lhs.vt == VT_UNKNOWN || rhs.vt == VT_UNKNOWN) {
		IRelationalOperatorPtr ipRelOp = lhs.punkVal;
		IGeometryPtr ipGeometry = rhs.punkVal;

		if(ipRelOp != 0 || ipGeometry != 0) {
			return equals()(ipRelOp, ipGeometry);
		}
	}

	return lhs == rhs;
}
#pragma warning(pop)

} // namespace sindy

#endif // __SINDY_FUNCTIONAL_EQUAL_TO_H__
