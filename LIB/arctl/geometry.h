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
 * @brief �_�`�󓯎m�̑召��r���Z���s���֐��N���X�B
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
 * @brief �_�`�󓯎m�̏��Ȃ艉�Z���s���֐��N���X�B
 */
struct point_less : public std::binary_function<IPoint*, IPoint*, bool>
{
	bool operator()(IPoint* lhs, IPoint* rhs) const
	{
		return point_compare()(lhs, rhs) < 0;
	}
};

/**
 * @brief �_�`�󓯎m�̓������Z���s���֐��N���X�B
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
 * @brief 2�̐����̎n�I�_�������i���邢�͐^�t�j�̊֌W�ɂ��邩���肷��
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
 * @brief �e�_��̌����͂��̂܂܁A���Ɍq����悤���������ւ��ĘA������B
 *
 * ���͔����q��IPointCollection��QueryInterface�ł���v�f�������ƁB
 *
 * @param first [in] �n�_�����q�B
 * @param last [in] �I�_�����q�B
 * @param ipPointCol [in,optional] ��ƂȂ�_��Bnull��n�����ꍇ�A�n�_�����q���w���v�f�𕡐����Ċ�Ƃ���B
 * @return PointCollection�I�u�W�F�N�g�B���ɂȂ����Ȃ������ꍇnull�B
 *
 * @note
 * ���͔����q��InputIterator�ɂ��ă\�[�g����Ƃ�������l�������A���ȏ�Ɏ�Ԃ�v�����A���܂藘�_�������Ȃ��B<br>
 * �Ƃ肠�����͂��� accumulate() �X�^�C���ŏ\�����낤�B
 */
template <typename ForwardIterator>
inline IPointCollectionPtr align_directed_pointcollection(
	ForwardIterator first,
	ForwardIterator last,
	IPointCollection* ipPointCol = 0)
{
	// �n�I�����q�������ʒu���w���Ă���ꍇ�͉������Ȃ��B
	if(first == last)
		return ipPointCol;

	// ��ƂȂ�_�񂪓n����Ȃ������ꍇ�A�n�_�����q���w���v�f����Ƃ���B
	if(ipPointCol == 0) {
		IClonePtr ipClone;
		atl2::valid(IClonePtr(*first)->Clone(&ipClone));
		return align_directed_pointcollection(++first, last, IPointCollectionPtr(ipClone));
	}

	// �Ƃ肠�����\���_��1�͔���������B
	long nPointCount;
	for(; atl2::valid(ipPointCol->get_PointCount(&nPointCount)), nPointCount == 0; ++first) {
		// �\���_��1������������ꂸ�ɍŌ�̗v�f�ɒH�蒅�����ꍇ�A��ƂȂ�_������̂܂ܕԂ��B
		if(first == last)
			return ipPointCol;

		atl2::valid(ipPointCol->AddPointCollection(IPointCollectionPtr(*first)));
	}

	typedef std::vector<atl2::seal<IPointCollectionPtr> > list_t;
	typedef std::map<IPointPtr, list_t::iterator, point_less> map_t;

	// �v�f��S�ēW�J�B
	list_t aList;
	std::copy(first, last, std::back_inserter(aList));

	// �n�I�_���W�����ɍ����t���B
	map_t aForwardMap, aReverseMap;
	for(list_t::iterator it = aList.begin(); it != aList.end(); ++it) {
		// ���W���_�u��悤�ł͈��ɂ��邱�ƂȂǕs�\�B
		if(! aForwardMap.insert(map_t::value_type(*atl2::begin(*it), it)).second)
			return 0;
		if(! aReverseMap.insert(map_t::value_type(*atl2::rbegin(*it), it)).second)
			return 0;
	}

	// ��_��̌��ɓ_���ǉ��B
	map_t::iterator it;
	while((it = aForwardMap.find(*atl2::rbegin(ipPointCol))) != aForwardMap.end() && *it->second != 0) {
		atl2::valid(ipPointCol->AddPointCollection(*it->second));
		*it->second = 0;
		aForwardMap.erase(it);
	}

	// ��_��̎�O�ɓ_���ǉ��B
	while((it = aReverseMap.find(*atl2::begin(ipPointCol))) != aReverseMap.end() && *it->second != 0) {
		atl2::valid(ipPointCol->InsertPointCollection(0, *it->second));
		*it->second = 0;
		aReverseMap.erase(it);
	}

	// �q�����Ă��Ȃ��v�f���c���Ă����玸�s�B
	return std::find_if(aList.begin(), aList.end(), rns::identity<bool>()) == aList.end() ? ipPointCol : 0;
}

} // namespace arctl

#endif // ARCTL_GEOMETRY_H_
