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
 * @file shape.h
 * @brief for field.h
 */
#ifndef ARCTL_FIELD_SHAPE_H_
#define ARCTL_FIELD_SHAPE_H_

#include <atl2/exception.h>
#include <arctl/detail/multi_inherit.h>

namespace arctl {
namespace field {

/**
 * @ingroup field_index_group 属性値クラス
 * @brief 形状属性インデックスクラス。
 * @test @ref unit_test FieldShapeTest
 */
class shape_index_t : public arctl::detail::multi_inherit::base_t
{
public:
//@{ @name コンストラクタとデストラクタ
	shape_index_t() : m_n(-1) {}
	shape_index_t(const shape_index_t& rIndex) : m_n(rIndex.m_n) {}
	shape_index_t(IFeature* ipFeature) { operator=(ipFeature); }
	shape_index_t(IFeatureClass* ipFeatureClass) { operator=(ipFeatureClass); }
//@}

//@{ @name 演算子
	long index() const { return m_n; }
//@}

//@{ @name 演算子
	/// 属性フィールドインデックスを取得する。
	operator long() const { return index(); }
	bool operator!() const { return m_n == -1; }

	shape_index_t& operator=(IFeature* ipFeature)
	{
		IObjectClassPtr ipClass;
		atl2::valid(ipFeature->get_Class(&ipClass));
		return operator=(IFeatureClassPtr(ipClass));
	}

	shape_index_t& operator=(IFeatureClass* ipFeatureClass)
	{
		atl2::valid(ipFeatureClass->FindField(bname(ipFeatureClass), &m_n));
		return *this;
	}
//@}
	
//@{ @name 静的関数
	/// 属性フィールド名をBSTR型で取得する。
	static CComBSTR bname(IFeatureClass* ipFeatureClass)
	{
		CComBSTR bstrName;
		atl2::valid(ipFeatureClass->get_ShapeFieldName(&bstrName));
		return bstrName;
	}

	/// 属性フィールド名を出力する。
	template <typename TOutputIterator>
	static void copy_name(IFeatureClass* ipFeatureClass, TOutputIterator result)
	{
		*result = COLE2CT(bname(ipFeatureClass)).m_psz;
		++result;
	}
//@}

private:
// 変数
	long m_n; ///< 属性フィールドインデックス。
};

/** @addtogroup field_value_group */
/*@{*/

/**
 * @brief 形状クラステンプレート。
 *
 * 形状の保持、フィーチャへの形状の書き出しを行うクラス。
 *
 * @par 属性値の書き出し:
 * IFeature::putref_Shape()<br>
 * IFeatureBuffer::putref_Shape()
 *
 * @test @ref unit_test FieldShapeTest
 */
template <typename TIShape = IGeometry>
class shape_base_t : public arctl::detail::multi_inherit::base_t
{
public:
// 定義
	typedef shape_index_t index_type;
	typedef _com_ptr_t<_com_IIID<TIShape, &__uuidof(TIShape)> > shape_ref_type;
	typedef shape_ref_type value_type;

/// @name コンストラクタとデストラクタ
//@{
	shape_base_t() {}
	shape_base_t(TIShape* ipShape) { operator=(ipShape); }
//@}

/// @name 取得
//@{
	/// 保持している形状スマートポインタへの可変参照を返す。
	shape_ref_type& shape() { return m_ipShape; }

	/// 保持している形状スマートポインタへの不変参照を返す。
	const shape_ref_type& shape() const { return m_ipShape; }
//@}

/// @name 属性値クラス互換
//@{
	/// 保持している形状スマートポインタへの可変参照を返す。
	value_type& value() { return shape(); }

	/// 保持している形状スマートポインタへの不変参照を返す。
	const value_type& value() const { return shape(); }

	/**
	 * @brief @p ipFeatureに形状を書き出す。
	 *
	 * @param ipFeature [in] 形状の置き換えを行うフィーチャ。
	 * @retval true 形状の書き出しに成功した。
	 * @retval false 形状の書き出しに失敗した。
	 * @exception _com_error_t <b>IFeature::putref_Shape()</b>がエラーを発した。
	 */
	bool write(IFeature* ipFeature, const index_type& = index_type()) const
	{
		return atl2::valid(ipFeature->putref_Shape(m_ipShape)) == S_OK;
	}

	/**
	 * @brief @p ipFeatureBufferに形状を書き出す。
	 *
	 * @param ipFeatureBuffer [in] 形状の置き換えを行うフィーチャ。
	 * @retval true 形状の書き出しに成功した。
	 * @retval false 形状の書き出しに失敗した。
	 * @exception _com_error_t <b>IFeature::putref_Shape()</b>がエラーを発した。
	 */
	bool write(IFeatureBuffer* ipFeatureBuffer, const index_type& = index_type()) const
	{
		return atl2::valid(ipFeatureBuffer->putref_Shape(m_ipShape)) == S_OK;
	}

	/**
	 * @brief @a m_ipShapeと@p ipFeatureの形状を比較する。
	 *
	 * @param ipFeature [in] 比較対象。
	 */
	bool equal_value(IFeature* ipFeature, const index_type& = index_type()) const
	{
		return equal_shape(ipFeature);
	}

	/**
	 * @brief @a m_ipShapeと@p ipFeatureBufferの形状を比較する。
	 *
	 * @param ipFeatureBuffer [in] 比較対象。
	 */
	bool equal_value(IFeatureBuffer* ipFeatureBuffer, const index_type& = index_type()) const
	{
		return equal_shape(ipFeatureBuffer);
	}

	/**
	 * @brief 保持している形状を入れ替える。
	 *
	 * @param ipShape [in] 新しい形状。
	 */
	shape_base_t& operator=(TIShape* ipShape)
	{
		m_ipShape = ipShape;
		return *this;
	}
//@}

private:
// 判定
	/**
	 * @brief @a m_ipShapeと@p ipFeatureの形状を比較する。
	 *
	 * @param ipFeature [in] 比較対象。
	 * @retval true 形状は一致する。
	 * @retval false 形状は一致しない。
	 * @exception _com_error_t 形状比較時に予期しないエラーが発生した。
	 */
	template <typename TIFeature>
	bool equal_shape(TIFeature* ipFeature) const
	{
		IGeometryPtr ipGeometry;
		atl2::valid(ipFeature->get_Shape(&ipGeometry));

		VARIANT_BOOL vbEquals = VARIANT_FALSE;
		atl2::valid(IClonePtr(ipGeometry)->IsEqual(IClonePtr(m_ipShape), &vbEquals));
		return vbEquals != VARIANT_FALSE;
	}

// 変数
	value_type m_ipShape;
};

/**
 * @brief フィーチャ形状クラステンプレート。
 *
 * フィーチャの形状の取得・保持・書き出しを行うクラス。<br>
 * @ref shape_base_tに、フィーチャからの形状取得機構を備え付けたもの。
 *
 * @par 属性値取得可能オブジェクト:
 * IFeature
 * @par 属性値の取得:
 * (IFeature::*TGetShapeMethod)()
 *
 * @par テンプレート引数:
 * - TGetShapeMethod : フィーチャから形状を取得するメソッド。
 * - TIShape : IGeometry継承クラス。デフォルトは<b>IGeometry</b>。
 *
 * @sa shape_t, shape_copy_t
 *
 * @test @ref unit_test FieldShapeTest
 */
template <HRESULT (__stdcall IFeature::*TGetShapeMethod)(IGeometry**), typename TIShape = IGeometry>
class feature_shape_t : public shape_base_t<TIShape>
{
	typedef shape_base_t<TIShape> super;
public:
/// @name コンストラクタとデストラクタ
//@{ 
	feature_shape_t() {}
	feature_shape_t(IFeature* ipFeature) { operator=(ipFeature); }
	feature_shape_t(IFeature* ipFeature, const index_type&) { operator=(ipFeature); }
	feature_shape_t(TIShape* ipShape) : super(ipShape) {}
//@}

/// @name 演算子
//@{
	feature_shape_t& operator=(IFeature* ipFeature)
	{
		IGeometryPtr ipGeometry;
		atl2::valid((ipFeature->*TGetShapeMethod)(&ipGeometry));
		value() = ipGeometry;
		return *this;
	}
//@}
};

/**
 * @brief フィーチャからの形状取得に<b>IFeature::get_Shape()</b>を使う@ref feature_shape_t。
 *
 * @test @ref unit_test FieldShapeTest
 */
template <typename TIShape = IGeometry>
class shape_t : public feature_shape_t<&IFeature::get_Shape, TIShape>
{
	typedef feature_shape_t<&IFeature::get_Shape, TIShape> super;
public:
/// @name コンストラクタとデストラクタ
//@{
	shape_t() {}
	shape_t(IFeature* ipFeature) : super(ipFeature) {}
	shape_t(IFeature* ipFeature, const super::index_type&) : super(ipFeature) {}
	shape_t(IFeatureBuffer* ipFeatureBuffer) { operator=(ipFeatureBuffer); }
	shape_t(IFeatureBuffer* ipFeatureBuffer, const super::index_type&) { operator=(ipFeatureBuffer); }
	shape_t(TIShape* ipShape) : super(ipShape) {}
//@}

/// @name 演算子
//@{
	shape_t& operator=(IFeature* ipFeature)
	{
		super::operator=(ipFeature);
		return *this;
	}

	shape_t& operator=(IFeatureBuffer* ipFeatureBuffer)
	{
		IGeometryPtr ipGeometry;
		atl2::valid(ipFeatureBuffer->get_Shape(&ipGeometry));
		value() = ipGeometry;
		return *this;
	}
//@}
};

/**
 * @brief フィーチャからの形状取得に<b>IFeature::get_ShapeCopy()</b>を使う@ref feature_shape_t。
 *
 * @test @ref unit_test FieldShapeTest
 */
template <typename TIShape = IGeometry>
class shape_copy_t : public feature_shape_t<&IFeature::get_ShapeCopy, TIShape>
{
	typedef feature_shape_t<&IFeature::get_ShapeCopy, TIShape> super;
public:
/// @name コンストラクタとデストラクタ
//@{
	shape_copy_t() {}
	shape_copy_t(IFeature* ipFeature) : super(ipFeature) {}
	shape_copy_t(IFeature* ipFeature, const super::index_type&) : super(ipFeature) {}
//@}
};

/*@}*/

/**
 * @ingroup field_value_mutabl_get_group
 */ 
//@{
/// 形状を取り出す。
template <typename T>
inline T* get(const shape_base_t<T>& val) { return val.value(); }
//@}

} // namespace field
} // namespace arctl

#endif // ARCTL_FIELD_SHAPE_H_
