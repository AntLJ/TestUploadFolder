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

#ifndef ARCTL_GEOMETRY_H_
#define ARCTL_GEOMETRY_H_

#include <map>
#include <vector>
#include <functional>
#include <algorithm>
#include <rns/functional.h>
#include <atl2/exception.h>
#include <atl2/seal.h>
#include <arctl/iterator.h>

namespace arctl {

/**
 * @brief 点形状同士の大小比較演算を行う関数クラス。
 */
struct point_compare : public std::binary_function<IPoint*, IPoint*, bool>
{
	int operator()(IPoint* lhs, IPoint* rhs) const
	{
		int nCompare;
		atl2::valid(lhs->Compare(rhs, &nCompare));
		return nCompare;
	}
};

/**
 * @brief 点形状同士の小なり演算を行う関数クラス。
 */
struct point_less : public std::binary_function<IPoint*, IPoint*, bool>
{
	bool operator()(IPoint* lhs, IPoint* rhs) const
	{
		return point_compare()(lhs, rhs) < 0;
	}
};

/**
 * @brief 点形状同士の等号演算を行う関数クラス。
 */
struct point_equals : public std::binary_function<IPoint*, IPoint*, bool>
{
	bool operator()(IPoint* lhs, IPoint* rhs) const
	{
		return point_compare()(lhs, rhs) == 0;
	}
};

struct segment_equals : public std::binary_function<_ISegment*, _ISegment*, bool>
{
	bool operator()(_ISegment* lhs, _ISegment* rhs) const
	{
		IPointPtr ipFromPoint1, ipToPoint1;
		IPointPtr ipFromPoint2, ipToPoint2;

		lhs->get_FromPoint(&ipFromPoint1);
		lhs->get_ToPoint(&ipToPoint1);

		rhs->get_FromPoint(&ipFromPoint2);
		rhs->get_ToPoint(&ipToPoint2);

		int nComp;

		atl2::valid(ipFromPoint1->Compare(ipFromPoint2, &nComp));
		if(nComp == 0) {
			atl2::valid(ipToPoint1->Compare(ipToPoint2, &nComp));
		}

		return nComp == 0;
	}
};

/**
 * @brief 2つの線分の始終点が同じ（あるいは真逆）の関係にあるか判定する
 */
struct segment_reversible_equals : public std::binary_function<_ISegment*, _ISegment*, bool>
{
	bool operator()(_ISegment* lhs, _ISegment* rhs) const
	{
		IPointPtr ipFromPoint1, ipToPoint1;
		IPointPtr ipFromPoint2, ipToPoint2;

		lhs->get_FromPoint(&ipFromPoint1);
		lhs->get_ToPoint(&ipToPoint1);

		rhs->get_FromPoint(&ipFromPoint2);
		rhs->get_ToPoint(&ipToPoint2);

		int nComp;

		atl2::valid(ipFromPoint1->Compare(ipFromPoint2, &nComp));
		if(nComp == 0) {
			atl2::valid(ipToPoint1->Compare(ipToPoint2, &nComp));
		}
		else {
			atl2::valid(ipFromPoint1->Compare(ipToPoint2, &nComp));
			if(nComp == 0) {
				atl2::valid(ipToPoint1->Compare(ipFromPoint2, &nComp));
			}
		}

		return nComp == 0;
	}
};

/**
 * @brief 各点列の向きはそのまま、一列に繋がるよう順序を入れ替えて連結する。
 *
 * 入力反復子はIPointCollectionにQueryInterfaceできる要素を持つこと。
 *
 * @param first [in] 始点反復子。
 * @param last [in] 終点反復子。
 * @param ipPointCol [in,optional] 基準となる点列。nullを渡した場合、始点反復子が指す要素を複製して基準とする。
 * @return PointCollectionオブジェクト。一列につなげられなかった場合null。
 *
 * @note
 * 入力反復子をInputIteratorにしてソートするという手も考えたが、今以上に手間を要する上、あまり利点を感じない。<br>
 * とりあえずはこの accumulate() スタイルで十分だろう。
 */
template <typename ForwardIterator>
inline IPointCollectionPtr align_directed_pointcollection(
	ForwardIterator first,
	ForwardIterator last,
	IPointCollection* ipPointCol = 0)
{
	// 始終反復子が同じ位置を指している場合は何もしない。
	if(first == last)
		return ipPointCol;

	// 基準となる点列が渡されなかった場合、始点反復子が指す要素を基準とする。
	if(ipPointCol == 0) {
		IClonePtr ipClone;
		atl2::valid(IClonePtr(*first)->Clone(&ipClone));
		return align_directed_pointcollection(++first, last, IPointCollectionPtr(ipClone));
	}

	// とりあえず構成点を1個は発生させる。
	long nPointCount;
	for(; atl2::valid(ipPointCol->get_PointCount(&nPointCount)), nPointCount == 0; ++first) {
		// 構成点を1個も発生させられずに最後の要素に辿り着いた場合、基準となる点列をそのまま返す。
		if(first == last)
			return ipPointCol;

		atl2::valid(ipPointCol->AddPointCollection(IPointCollectionPtr(*first)));
	}

	typedef std::vector<atl2::seal<IPointCollectionPtr> > list_t;
	typedef std::map<IPointPtr, list_t::iterator, point_less> map_t;

	// 要素を全て展開。
	list_t aList;
	std::copy(first, last, std::back_inserter(aList));

	// 始終点座標を元に索引付け。
	map_t aForwardMap, aReverseMap;
	for(list_t::iterator it = aList.begin(); it != aList.end(); ++it) {
		// 座標がダブるようでは一列にすることなど不可能。
		if(! aForwardMap.insert(map_t::value_type(*atl2::begin(*it), it)).second)
			return 0;
		if(! aReverseMap.insert(map_t::value_type(*atl2::rbegin(*it), it)).second)
			return 0;
	}

	// 基準点列の後ろに点列を追加。
	map_t::iterator it;
	while((it = aForwardMap.find(*atl2::rbegin(ipPointCol))) != aForwardMap.end() && *it->second != 0) {
		atl2::valid(ipPointCol->AddPointCollection(*it->second));
		*it->second = 0;
		aForwardMap.erase(it);
	}

	// 基準点列の手前に点列を追加。
	while((it = aReverseMap.find(*atl2::begin(ipPointCol))) != aReverseMap.end() && *it->second != 0) {
		atl2::valid(ipPointCol->InsertPointCollection(0, *it->second));
		*it->second = 0;
		aReverseMap.erase(it);
	}

	// 繋がっていない要素が残っていたら失敗。
	return std::find_if(aList.begin(), aList.end(), rns::identity<bool>()) == aList.end() ? ipPointCol : 0;
}

} // namespace arctl

#endif // ARCTL_GEOMETRY_H_
