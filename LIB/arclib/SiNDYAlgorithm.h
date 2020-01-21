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
 * @brief SiNDYアルゴリズム定義
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
 * @brief 渡されたコンテナに対してfor_each()を試行するクラス
 *
 * やっていることはbind2ndと同じ。
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
	 * @note ポインタしか渡されないはず。
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
 * @brief 値に対応するコード値ドメインインデックスを検索する
 *
 * @param ipCodedValueDomain [in] コード値ドメイン。
 * @param rValue [in] 値。
 * @return 0基点のインデックス。対応するドメイン名が存在しなかった場合-1。
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
 * @note _Mainは「参照」か「インタフェースポインタ（スマートポインタ不可）」。
 */
template <typename _Main, typename _Sub>
class With
{
public:
public:
	typedef _Main referent_type;
	typedef _Sub support_type;

// 構築
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
 * @brief With作成関数群
 *
 * partial orderingできないvc用に作成。
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
	 * @name 擬似partial-ordering
	 *
	 * vcがpartial orderingに対応していれば、次のコードで用は済むのだが（というか_WithMaker構造体自体要らない）。
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
 * @brief _Mainと_SubをWithに入れて_Functionに渡す関数クラス
 */
template <typename _Function, typename _Sub>
class _Binder
{
public:
// 構築
	_Binder(_Function& rFunction, const _Sub& rSub) :
	m_rFunction(rFunction),
	m_rSub(rSub)
	{
	}
	
// 取得
	template <typename _Main>
	operator ()(_Main& rMain)
	{
		m_rFunction(_WithMaker<_Sub>::make(rMain, m_rSub));
	}

	_Function& function() { return m_rFunction; }

private:
	_Function& m_rFunction; ///< 関数オブジェクト
	const _Sub& m_rSub;
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// 関数
//
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief _Binder作成関数
 *
 * for_each支援クラスオブジェクト作成関数の１つ。
 *
 * @param rFunction [in] 関数オブジェクト
 * @param rSub [in] 関数オブジェクトに
 */
template <typename _Function, typename _Sub>
_Binder<_Function, _Sub> bind(_Function& rFunction, const _Sub& rSub)
{
	return _Binder<_Function, _Sub>(rFunction, rSub);
}

/**
 * @brief Withオブジェクトに対するfor_eachサポート関数構築マクロ
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
