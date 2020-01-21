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
 * @file topological.h
 * @brief 幾何学演算テンプレートライブラリ。
 */
#ifndef ARCTL_TOPOLOGICAL_H_
#define ARCTL_TOPOLOGICAL_H_

namespace arctl {
namespace topological {

/**
 * @brief accumulate 用、形状結合関数。
 *
 * @par 代替アルゴリズム:
 * @code
 * ITopologicalOperatorPtr ipTopoOp =
 *   std::accumulate(
 *     boost::next(first),
 *     last,
 *     ITopoloticalOperatorPtr(IClonePtr(*first)_Clone()),
 *     boost::bind(&ITopologicalOperator::_Union, _1, _2));
 * @endcode
 */
inline ITopologicalOperatorPtr union_geometry(ITopologicalOperator* ipTopoOp, IGeometry* ipGeometry)
{
	if(ipTopoOp != 0) {
		IGeometryPtr ipResultGeometry;
		ipTopoOp->Union(ipGeometry, &ipResultGeometry);
		return ipResultGeometry;
	}
	else {
		IClonePtr ipClone;
		IClonePtr(ipGeometry)->Clone(&ipClone);
		return ipClone;
	}
}

} // namespace topological
} // namespace arctl

#endif // ARCTL_TOPOLOGICAL_H_
