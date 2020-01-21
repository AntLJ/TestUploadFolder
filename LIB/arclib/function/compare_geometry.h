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
 * @brief ������W����֐��N���X
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
 * @brief �`���r�֐��N���X�e���v���[�g
 *
 * @par �e���v���[�g����:
 * - TFP : IRelationalOperator �̔�r�֐��B IRelationalOperator::Relation() �͈����̐����قȂ�̂ŁA���̃e���v���[�g�͎g���Ȃ��B
 *
 * @note
 * ��2������ const GeometryPtr_& �ɂ������̂́A�v��Ȃ��Ƃ����Ηv��Ȃ��B<br>
 * boost::bind(equals(), ipRelOp, boost::bind(identify<GeometryPtr>(), _1)) �Ƃł�����Ηǂ��킯�����B
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
 * @brief ����`�󔻒�֐��N���X
 *
 * compare_geometry �𓯈�`�󔻒�p�ɓ��ꉻ�����N���X�B<br>
 * ArcObjects 8.2 �� IRelationalOperator �͎��Ȍ����_��ɑ΂��ď�ɋU��Ԃ��̂ŁA
 * �_��`��̈�v��r�Ɋւ��Ă̂݁A�Ǝ��̃A���S���Y�����`���Ă���B
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

		// �`��^����v���Ȃ���Ύn�܂�Ȃ��B
		esriGeometryType eType = getData<esriGeometryType>(ipGeometry);
		if(eType != getData<esriGeometryType>(IGeometryPtr(ipRelOp)))
			return false;

		if(eType == esriGeometryPolyline) {
			// �_��`��̏ꍇ�͍\���_���W�����͂Ŕ�r����B
			typedef traits<IPointCollectionPtr>::Container Container;

			Container aContainer1(ipRelOp);
			Container aContainer2(ipGeometry);
			if(aContainer1.size() != aContainer2.size())
				return false;

			return std::equal(aContainer1.begin(), aContainer1.end(), aContainer2.begin(), point_equals());
		}
		else {
			// �_��`��ȊO�̏ꍇ�� IRelationalOperator::Equals() �Ɉς˂�B
			VARIANT_BOOL vb;
			return ipRelOp->Equals(ipGeometry, &vb) == S_OK && vb == VARIANT_TRUE;
		}
	}
};
#pragma warning( pop )
#endif // _USE_ARC9

class contains : public compare_geometry<&IRelationalOperator::Contains> {}; ///< �܂ށB
class crosses : public compare_geometry<&IRelationalOperator::Crosses> {}; ///< ��������B
class disjoint : public compare_geometry<&IRelationalOperator::Disjoint> {}; ///< �d�Ȃ�Ȃ��B
class equals : public compare_geometry<&IRelationalOperator::Equals> {}; ///< �������B
class overlaps : public compare_geometry<&IRelationalOperator::Overlaps> {}; ///< �d�Ȃ�B
class touches : public compare_geometry<&IRelationalOperator::Touches> {}; ///< �ڂ���B
class within : public compare_geometry<&IRelationalOperator::Within> {}; ///< �܂܂��B

} // namespace sindy

#endif // SINDY_FUNCTIONAL_COMPARE_GEOMETRY_H_
