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

#ifndef SINDY_FUNCTIONAL_COMPARE_GEOMETRY_H_
#define SINDY_FUNCTIONAL_COMPARE_GEOMETRY_H_

#include <functional>

namespace sindy {

/**
 * @brief 同一座標判定関数クラス
 */
class point_equals : public std::binary_function<IPoint*, IPoint*, bool>
{
public:
	bool operator()(IPoint* lhs, IPoint* rhs) const
	{
		int nResult;
		return lhs->Compare(rhs, &nResult) == S_OK && nResult == 0;
	}
};

/**
 * @brief 形状比較関数クラステンプレート
 *
 * @par テンプレート引数:
 * - TFP : IRelationalOperator の比較関数。 IRelationalOperator::Relation() は引数の数が異なるので、このテンプレートは使えない。
 *
 * @note
 * 第2引数を const GeometryPtr_& にしたものは、要らないといえば要らない。<br>
 * boost::bind(equals(), ipRelOp, boost::bind(identify<GeometryPtr>(), _1)) とでもすれば良いわけだし。
 */
template <HRESULT (__stdcall IRelationalOperator::*TFP)(IGeometry*, VARIANT_BOOL*)>
class compare_geometry : public std::binary_function<IRelationalOperator*, IGeometry*, bool>
{
public:
	bool operator()(IRelationalOperator* lhs, const GeometryPtr_& rhs) const { return operator()(lhs, rhs); }

	bool operator()(IRelationalOperator* ipRelOp, IGeometry* ipGeometry) const
	{
		if(! (ipRelOp && ipGeometry))
			return false;

		VARIANT_BOOL vb;
		return (ipRelOp->*TFP)(ipGeometry, &vb) == S_OK && vb == VARIANT_TRUE;
	}
};

#ifndef _USE_ARC9
#pragma warning( push )
#pragma warning( disable : 4310 )
/**
 * @brief 同一形状判定関数クラス
 *
 * compare_geometry を同一形状判定用に特殊化したクラス。<br>
 * ArcObjects 8.2 の IRelationalOperator は自己交差点列に対して常に偽を返すので、
 * 点列形状の一致比較に関してのみ、独自のアルゴリズムを定義している。
 */
template <>
class compare_geometry<&IRelationalOperator::Equals> : public std::binary_function<IRelationalOperator*, IGeometry*, bool>
{
public:
	bool operator()(IRelationalOperator* lhs, const GeometryPtr_& rhs) const { return operator()(lhs, rhs); }

	bool operator()(IRelationalOperator* ipRelOp, IGeometry* ipGeometry) const
	{
		if(! (ipRelOp && ipGeometry))
			return false;

		// 形状型が一致しなければ始まらない。
		esriGeometryType eType = getData<esriGeometryType>(ipGeometry);
		if(eType != getData<esriGeometryType>(IGeometryPtr(ipRelOp)))
			return false;

		if(eType == esriGeometryPolyline) {
			// 点列形状の場合は構成点座標を自力で比較する。
			typedef traits<IPointCollectionPtr>::Container Container;

			Container aContainer1(ipRelOp);
			Container aContainer2(ipGeometry);
			if(aContainer1.size() != aContainer2.size())
				return false;

			return std::equal(aContainer1.begin(), aContainer1.end(), aContainer2.begin(), point_equals());
		}
		else {
			// 点列形状以外の場合は IRelationalOperator::Equals() に委ねる。
			VARIANT_BOOL vb;
			return ipRelOp->Equals(ipGeometry, &vb) == S_OK && vb == VARIANT_TRUE;
		}
	}
};
#pragma warning( pop )
#endif // _USE_ARC9

class contains : public compare_geometry<&IRelationalOperator::Contains> {}; ///< 含む。
class crosses : public compare_geometry<&IRelationalOperator::Crosses> {}; ///< 交差する。
class disjoint : public compare_geometry<&IRelationalOperator::Disjoint> {}; ///< 重ならない。
class equals : public compare_geometry<&IRelationalOperator::Equals> {}; ///< 等しい。
class overlaps : public compare_geometry<&IRelationalOperator::Overlaps> {}; ///< 重なる。
class touches : public compare_geometry<&IRelationalOperator::Touches> {}; ///< 接する。
class within : public compare_geometry<&IRelationalOperator::Within> {}; ///< 含まれる。

} // namespace sindy

#endif // SINDY_FUNCTIONAL_COMPARE_GEOMETRY_H_
