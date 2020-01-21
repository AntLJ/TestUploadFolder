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
 * @file cursor.h
 * @brief _ICursor �n�e���v���[�g���C�u�����B
 */
#ifndef ARCTL_CURSOR_H_
#define ARCTL_CURSOR_H_

#include <atl2/exception.h>
#include <arctl/iterator/single_pass_iterators.h>

namespace arctl {

namespace detail {

template <bool TIsCursorIterator>
struct cursor_iterator_operator
{
	template <typename TIterator> static void update(TIterator it) { atl2::valid((*it)->Store()); }
	template <typename TIterator> static void del(TIterator it) { atl2::valid((*it)->Delete()); }
};

template <>
struct cursor_iterator_operator<true>
{
	static void update(_ICursor* ipCursor, _IRow* ipRow)
	{
		if(FAILED(ipCursor->UpdateRow(ipRow))) {
			atl2::valid(ipRow->Store());
		}
	}
	static void update(IFeatureCursor* ipFeatureCursor, IFeature* ipFeature)
	{
		if(FAILED(ipFeatureCursor->UpdateFeature(ipFeature))) {
			atl2::valid(ipFeature->Store());
		}
	}

	static void del(_ICursor* ipCursor, _IRow* ipRow)
	{
		if(FAILED(ipCursor->DeleteRow())) {
			atl2::valid(ipRow->Delete());
		}
	}
	static void del(IFeatureCursor* ipFeatureCursor, IFeature* ipFeature)
	{
		if(FAILED(ipFeatureCursor->DeleteFeature())) {
			atl2::valid(ipFeature->Delete());
		}
	}

	template <typename TIterator> static void update(TIterator it)
	{
		update(it.container(), *it);
	}
	template <typename TIterator> static void del(TIterator it)
	{
		del(it.container(), *it);
	}
};

template <typename T> struct cursor_pointer_traits {};

template <>
struct cursor_pointer_traits<_ICursor*>
{
	static _variant_t insert(_ICursor* ipCursor, IRowBuffer* ipRowBuffer)
	{
		_variant_t va;
		atl2::valid(ipCursor->InsertRow(ipRowBuffer, &va));
		return va;
	}
};

template <>
struct cursor_pointer_traits<IFeatureCursor*>
{
	static _variant_t insert(IFeatureCursor* ipCursor, IFeatureBuffer* ipFeatureBuffer)
	{
		_variant_t va;
		atl2::valid(ipCursor->InsertFeature(ipFeatureBuffer, &va));
		return va;
	}
};

} // namespace detail

/// _ICursor�n�����q���ۂ��𔻒肷��N���X�B
template <typename T> struct is_cursor_iterator { enum { value = false }; };
template <> struct is_cursor_iterator<atl2::container_traits<_ICursor>::iterator> { enum { value = true }; };
template <> struct is_cursor_iterator<atl2::container_traits<IFeatureCursor>::iterator> { enum { value = true }; };

/**
 * @brief �S�Ă̗v�f�� op(*it) ���{�s���A�^�l��Ԃ������̂� _IRow::Store() ����A���S���Y���B
 *
 * TInputIterator �� _ICursor �� IFeatureCursor �̔����q�ł���ꍇ�A _IRow::Store() �̑O�� _ICursor::UpdateRow() �����݁A
 * ���ꂪ�G���[��Ԃ������ɉ��߂� _IRow::Store() ���{�H����B<br>
 * ����͔����q���\������J�[�\�����uSearch�J�[�\���v�uUpdate�J�[�\���v�̂ǂ���ł����Ă��Ή��ł���悤�ɂ��邽�߂̎d�g�݂ł���B
 *
 * @note
 * �{���� FDO_E_CURSOR_WRONG_TYPE �Ƃ̔�r���ǂ����Ȃ̂��APGDB�����肾�� E_FAIL ���Ԃ��Ă���悤�Ȃ̂ŁA
 * �G���[���Ԃ��Ă����炷�ׂ� _IRow::Store() �։񂷂悤�ɂ��Ă���B
 *
 * @par �e���v���[�g����:
 * - @a TInputIterator : value_type �� _IRow* �� IFeature* �ł��锽���q�B
 * - @a TUnaryOperation : std::unary_function<_IRow*, bool> �B
 *
 * @param first [in] �n�_�����q�B
 * @param last [in] �I�_�����q�B
 * @param op [in] �X�V�֐��B
 * @return �X�V�֐��B
 *
 * @test @ref unit_test UpdateAlgorithmTest
 */
template <typename TInputIterator, typename TUnaryOperation>
inline TUnaryOperation update(TInputIterator first, TInputIterator last, TUnaryOperation op)
{
	for(; first != last; ++first) {
		if(op(*first)) {
			arctl::detail::cursor_iterator_operator<is_cursor_iterator<TInputIterator>::value>::update(first);
		}
	}

	return op;
}

/**
 * @brief �S�Ă̗v�f�� _IRow::Delete() ����A���S���Y���B
 *
 * @par �e���v���[�g����:
 * - TInputIterator : value_type �� _IRow* �� IFeature* �ł��锽���q�B
 *
 * @param first [in] �n�_�����q�B
 * @param last [in] �I�_�����q�B
 *
 * @note
 * TInputIterator �� _ICursor �� IFeatureCursor �̔����q�ł���ꍇ�A _IRow::Delete() �̑���� _ICursor::DeleteRow() ��p����B
 *
 * @test @ref unit_test UpdateAlgorithmTest
 */
template <typename TInputIterator>
inline void delete_all(TInputIterator first, TInputIterator last)
{
	for(; first != last; ++first) {
		arctl::detail::cursor_iterator_operator<is_cursor_iterator<TInputIterator>::value>::del(first);
	}
}

template <typename T>
class cursor_traits : public detail::cursor_pointer_traits<typename atl2::interface_pointer_of<T>::type>
{
	typedef detail::cursor_pointer_traits<typename atl2::interface_pointer_of<T>::type> super;
};

} // namespace arctl

#endif // ARCTL_CURSOR_H_
