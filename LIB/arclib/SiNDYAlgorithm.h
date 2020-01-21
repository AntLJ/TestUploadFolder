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
 * @file SiNDYAlgorith.h
 * @brief SiNDY�A���S���Y����`
 */
#ifndef __SINDY_ALGORITHM_H__
#define __SINDY_ALGORITHM_H__

#include "SiNDYTraits.h"
#include "SiNDYImplicitCast.h"
#include "function/compare_geometry.h"
#include "function/equal_to.h"
#include "detail/def_reference_to.h"

namespace sindy {

inline IClonePtr clone(IClonePtr ipOriginal)
{
	IClonePtr ipClone;
	ipOriginal->Clone(&ipClone);
	return ipClone;
}

/**
 * @brief �n���ꂽ�R���e�i�ɑ΂���for_each()�����s����N���X
 *
 * ����Ă��邱�Ƃ�bind2nd�Ɠ����B
 */
template<typename Function>
class ForEach
{
public:
	ForEach(Function cFunction) :
	m_rFunction(cFunction)
	{
	}

	/**
	 * @note �|�C���^�����n����Ȃ��͂��B
	 */
	template <typename TContainer>
	void operator()(TContainer* pContainer)
	{
		for_each<_REFERENCE_TO(Function)>(pContainer, m_rFunction);
	}

private:
	Function m_rFunction;
};

template<typename Function>
ForEach<Function> make_ForEach(Function cFunction)
{
	return ForEach<Function>(cFunction);
}

/**
 * @brief �l�ɑΉ�����R�[�h�l�h���C���C���f�b�N�X����������
 *
 * @param ipCodedValueDomain [in] �R�[�h�l�h���C���B
 * @param rValue [in] �l�B
 * @return 0��_�̃C���f�b�N�X�B�Ή�����h���C���������݂��Ȃ������ꍇ-1�B
 */
inline long find(ICodedValueDomain* ipCodedValueDomain, VARIANT& rValue)
{
	long nCodeCount = 0;
	ipCodedValueDomain->get_CodeCount(&nCodeCount);
	for(long i = 0; i < nCodeCount; i++) {
		_variant_t va;
		ipCodedValueDomain->get_Value(i, &va);
		if(va == rValue)
			return i;
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// With
//
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @note _Main�́u�Q�Ɓv���u�C���^�t�F�[�X�|�C���^�i�X�}�[�g�|�C���^�s�j�v�B
 */
template <typename _Main, typename _Sub>
class With
{
public:
public:
	typedef _Main referent_type;
	typedef _Sub support_type;

// �\�z
	With(_Main cMain, const _Sub& rSub) :
	m_Main(cMain),
	m_rSub(rSub)
	{
	}
	
	operator _Main () const { return m_Main; }
	operator const _Sub& () const { return m_rSub; }
	
private:
	_Main m_Main;
	const _Sub& m_rSub;
};

/**
 * @brief With�쐬�֐��Q
 *
 * partial ordering�ł��Ȃ�vc�p�ɍ쐬�B
 */
template <typename _Sub>
struct _WithMaker
{
	template <typename _Main>
	static With<_Main&, _Sub> make(_Main& rMain, const _Sub& rSub)
	{
		return With<_Main&, _Sub>(rMain, rSub);
	}

	/**
	 * @name �[��partial-ordering
	 *
	 * vc��partial ordering�ɑΉ����Ă���΁A���̃R�[�h�ŗp�͍ςނ̂����i�Ƃ�����_WithMaker�\���̎��̗v��Ȃ��j�B
	 * @code
	 * template <typename _Main>
	 * With<_Main*, _Sub> make(_Main* pMain, const _Sub& rSub)
	 * {
	 *     return With<_Main*, _Sub>(pMain, rSub);
	 * }
	 *
	 * template <typename _IIID>
	 * With<_IIID::Interface*, _Sub> make(const _com_ptr_t<_IIID>& rMain, const _Sub& rSub)
	 * {
	 *     return With<_IIID::Interface*, _Sub>(rMain, rSub);
	 * }
	 * @endcode
	 */
	//@{

	static With<_IRow*, _Sub> make(_IRow* ipRow, const _Sub& rSub)
	{
		return With<_IRow*, _Sub>(ipRow, rSub);
	}

	static With<IFeature*, _Sub> make(IFeature* ipFeature, const _Sub& rSub)
	{
		return With<IFeature*, _Sub>(ipFeature, rSub);
	}

	static With<ICursor*, _Sub> make(ICursor* ipCursor, const _Sub& rSub)
	{
		return With<ICursor*, _Sub>(ipCursor, rSub);
	}

	static With<IFeatureCursor*, _Sub> make(IFeatureCursor* ipFeatureCursor, const _Sub& rSub)
	{
		return With<IFeatureCursor*, _Sub>(ipFeatureCursor, rSub);
	}

	static With<_IRow*, _Sub> make(_IRowPtr ipRow, const _Sub& rSub)
	{
		return With<_IRow*, _Sub>(ipRow, rSub);
	}

	static With<IFeature*, _Sub> make(IFeaturePtr ipFeature, const _Sub& rSub)
	{
		return With<IFeature*, _Sub>(ipFeature, rSub);
	}

	static With<ICursor*, _Sub> make(ICursorPtr ipCursor, const _Sub& rSub)
	{
		return With<ICursor*, _Sub>(ipCursor, rSub);
	}

	static With<IFeatureCursor*, _Sub> make(IFeatureCursorPtr ipFeatureCursor, const _Sub& rSub)
	{
		return With<IFeatureCursor*, _Sub>(ipFeatureCursor, rSub);
	}
	//@}
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// _Binder
//
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief _Main��_Sub��With�ɓ����_Function�ɓn���֐��N���X
 */
template <typename _Function, typename _Sub>
class _Binder
{
public:
// �\�z
	_Binder(_Function& rFunction, const _Sub& rSub) :
	m_rFunction(rFunction),
	m_rSub(rSub)
	{
	}
	
// �擾
	template <typename _Main>
	operator ()(_Main& rMain)
	{
		m_rFunction(_WithMaker<_Sub>::make(rMain, m_rSub));
	}

	_Function& function() { return m_rFunction; }

private:
	_Function& m_rFunction; ///< �֐��I�u�W�F�N�g
	const _Sub& m_rSub;
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// �֐�
//
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief _Binder�쐬�֐�
 *
 * for_each�x���N���X�I�u�W�F�N�g�쐬�֐��̂P�B
 *
 * @param rFunction [in] �֐��I�u�W�F�N�g
 * @param rSub [in] �֐��I�u�W�F�N�g��
 */
template <typename _Function, typename _Sub>
_Binder<_Function, _Sub> bind(_Function& rFunction, const _Sub& rSub)
{
	return _Binder<_Function, _Sub>(rFunction, rSub);
}

/**
 * @brief With�I�u�W�F�N�g�ɑ΂���for_each�T�|�[�g�֐��\�z�}�N��
 */
#define _FOR_EACH_WITH_ALGORITHM(func_name) \
template <typename _Function, typename _Main, typename _Sub> \
inline _Function func_name(With<_Main, _Sub> cWith, _Function f) \
{ \
	_Main cMain = static_cast<_Main>(cWith); \
	return func_name(cMain, bind(f, static_cast<const _Sub&>(cWith))).function(); \
} \
template <typename _Function, typename _Main, typename _Sub> \
inline _Function func_name##_with(_Main cMain, const _Sub& rSub, _Function f) \
{ \
	return func_name(cMain, bind(f, rSub)).function(); \
}

_FOR_EACH_WITH_ALGORITHM(for_each);
_FOR_EACH_WITH_ALGORITHM(for_each_row);

#undef _FOR_EACH_NEXT
#undef _FIND_NEXT
#undef _RANGE_FOR_EACH_INDEX
#undef _FOR_EACH_INDEX
#undef _RANGE_FIND_INDEX
#undef _FIND_INDEX

} // namespace sindy

#include "algorithm/for_each.h"
#include "algorithm/find.h"
#include "detail/undef_reference_to.h"

#endif // __SINDY_ALGORITHM_H__
