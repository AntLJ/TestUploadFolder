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

#ifndef __SINDY_GEOMETRY_H__
#define __SINDY_GEOMETRY_H__

#include "SiNDYTraits.h"
#include "SiNDYImplicitCast.h"
#include "SiNDYAlgorithm.h"
#include "SiNDYAttrCommunity.h"
#include "SiNDYFieldIndex.h"
#include "function/compare_geometry.h"

namespace sindy {

/**
 * @brief �`��擾�E�ێ��N���X
 *
 * SINDY_FIELDVALUE_CLASS�}�N���̈����Ƃ��Ďg����B
 */
class Geometry
{
public:
// ��`
	/**
	 * @note typdef�Œ�`����ƌp�����ɕs����N����̂ŁA�����Čp�����g�p�B
	 */
	class FieldIndex : public sindy::FieldIndex<kShape>
	{
	public:
		template <typename T>
		FieldIndex(T const& rArg) : sindy::FieldIndex<kShape>(rArg) { }
	};

// �\�z/����
	Geometry() {}
	explicit Geometry(IPointCollection* ipPointCol) : m_ipGeometry(ipPointCol) {}
	explicit Geometry(IGeometry* ipGeometry) : m_ipGeometry(ipGeometry) {}
	explicit Geometry(IFeature* ipFeature) { ipFeature->get_Shape(&m_ipGeometry); }

// �ݒ�/�擾
	IGeometry* getValue() const { return m_ipGeometry; }
	IGeometry* getGeometry() const { return m_ipGeometry; }

// �^�ϊ�
	operator IGeometry* () const { return getValue(); }
	Geometry& operator=(IFeature* ipFeature) { ipFeature->get_Shape(&m_ipGeometry); return *this; }
	IGeometry* operator->() const { return m_ipGeometry; }

private:
	IGeometryPtr m_ipGeometry;
};

class Envelope
{
public:
// �\�z
	Envelope() : m_dXMin(0), m_dYMin(0), m_dXMax(0), m_dYMax(0) {}
	template <typename T> Envelope(T const& rT) : m_dXMin(0), m_dYMin(0), m_dXMax(0), m_dYMax(0) { setEnvelope(rT); }

// �ݒ�
	void setEnvelope(const EnvelopePtr& rEnv)
	{
		setEnvelope(static_cast<IEnvelope*>(rEnv));
	}

	void setEnvelope(IEnvelope* ipEnv)
	{
		ipEnv->get_XMin(&m_dXMin);
		ipEnv->get_YMin(&m_dYMin);
		ipEnv->get_XMax(&m_dXMax);
		ipEnv->get_YMax(&m_dYMax);
	}

// �擾
	double getXMin() const { return m_dXMin; }
	double getYMin() const { return m_dYMin; }
	double getXMax() const { return m_dXMax; }
	double getYMax() const { return m_dYMax; }

// �ϐ�
	double m_dXMin, m_dYMin, m_dXMax, m_dYMax;
};

template <typename TInterface, HRESULT (__stdcall TInterface::*TFP)(VARIANT_BOOL*)>
struct get_variant_bool : public std::unary_function<TInterface*, bool>
{
	bool operator()(const _COM_SMARTPTR<_COM_SMARTPTR_LEVEL2<TInterface, &__uuidof(TInterface)> >& ip)
	{
		return operator()(static_cast<TInterface*>(ip));
	}

	bool operator()(TInterface* ip)
	{
		VARIANT_BOOL vb;
		(ip->*TFP)(&vb);
		return vb == VARIANT_TRUE;
	}
};

struct is_simple : public get_variant_bool<ITopologicalOperator, &ITopologicalOperator::get_IsSimple> {};


/**
 * @brief Arc�� IRelationalOperator �Ɉ˂炸�A�Ǝ��̃A���S���Y���ŋ�`�̏d��������s���B
 */
struct envelope_overlaps : public std::binary_function<IEnvelope*, IEnvelope*, bool>
{
	bool operator()(IEnvelope* ipEnv1, IEnvelope* ipEnv2)
	{
		if(! (ipEnv1 && ipEnv2))
			return false;

		double dXMin1, dYMin1, dXMax1, dYMax1;
		double dXMin2, dYMin2, dXMax2, dYMax2;

		ipEnv1->get_XMin(&dXMin1);
		ipEnv1->get_YMin(&dYMin1);
		ipEnv1->get_XMax(&dXMax1);
		ipEnv1->get_YMax(&dYMax1);

		ipEnv2->get_XMin(&dXMin2);
		ipEnv2->get_YMin(&dYMin2);
		ipEnv2->get_XMax(&dXMax2);
		ipEnv2->get_YMax(&dYMax2);

		return dXMin1 <= dXMax2 && dXMax1 >= dXMin2 && dYMin1 <= dYMax2 && dYMax1 >= dYMin2;
	}
};

/**
 * @brief Arc�� IRelationalOperator �Ɉ˂炸�A�Ǝ��̃A���S���Y���œ_��̌���������s���B
 */
struct polyline_crosses : public std::binary_function<IPointCollection*, IPointCollection*, bool>
{
	bool operator()(IPath* ipPath1, IPath* ipPath2)
	{
		return operator()(IPointCollectionPtr(ipPath1), IPointCollectionPtr(ipPath2));
	}

	bool operator()(IPolyline* ipPolyline1, IPolyline* ipPolyline2)
	{
		return operator()(IPointCollectionPtr(ipPolyline1), IPointCollectionPtr(ipPolyline2));
	}

	bool operator()(IPointCollection* ipPointCol1, IPointCollection* ipPointCol2)
	{
		if(! (ipPointCol1 && ipPointCol2))
			return false;

		// �\���_����2�����̓_�񓯎m�͌��������Ȃ��B
		long nCount1, nCount2;
		ipPointCol1->get_PointCount(&nCount1);
		ipPointCol2->get_PointCount(&nCount2);
		if(nCount1 < 2 || nCount2 < 2)
			return false;

		// �O�ڋ�`���d�Ȃ荇��Ȃ��_��͌��������Ȃ��B
		if(! envelope_overlaps()(
			getData<IEnvelopePtr>(IGeometryPtr(ipPointCol1)),
			getData<IEnvelopePtr>(IGeometryPtr(ipPointCol2))))
				return false;

		double a1x, a1y;
		std::pair<double, double> aPoint1_1;
		for(long i = 0; i < nCount1; i++) {
			double a2x, a2y;

			IPointPtr ipPoint1 = getData<IPointPtr>(ipPointCol1, i);
			ipPoint1->get_X(&a2x);
			ipPoint1->get_Y(&a2y);

			if(i > 0) {
				double aminx = min(a1x, a2x);
				double aminy = min(a1y, a2y);
				double amaxx = max(a1x, a2x);
				double amaxy = max(a1y, a2y);

				double b1x, b1y;
				for(long j = 0; j < nCount2; j++) {
					double b2x, b2y;
	
					IPointPtr ipPoint2 = getData<IPointPtr>(ipPointCol2, j);
					ipPoint2->get_X(&b2x);
					ipPoint2->get_Y(&b2y);

					if(j > 0) {
						// 2�_�ڂ�������W�̏ꍇ�B
						if(i + 1 < nCount1 && j + 1 < nCount2 && a2x == b2x && a2y == b2y) {
							double a3x, a3y, b3x, b3y;

							ipPoint1 = getData<IPointPtr>(ipPointCol1, i + 1);
							ipPoint2 = getData<IPointPtr>(ipPointCol2, j + 1);
							ipPoint1->get_X(&a3x);
							ipPoint1->get_Y(&a3y);
							ipPoint2->get_X(&b3x);
							ipPoint2->get_Y(&b3y);

							// -1:�E�� 0:���i 1:���܁B
							int n = side(b1x, b1y, b2x, b2y, b3x, b3y);

							int n12 = side(b1x, b1y, a1x, a1y, b2x, b2y);
							int n13 = side(b1x, b1y, a1x, a1y, b3x, b3y);
							int n32 = side(b1x, b1y, a3x, a3y, b2x, b2y);
							int n33 = side(b1x, b1y, a3x, a3y, b3x, b3y);

							if(n < 0) {
								if(((n12 + n13 == -2) && (n32 + n33 >= 0)) || ((n32 + n33 == -2) && (n12 + n13 >= 0)))
									return true;
							}
							else if(n > 0) {
								if(((n12 + n13 >= 0) && (n32 + n33 == -2)) || ((n32 + n33 >= 0) && (n12 + n13 == -2)))
									return true;
							}
							else {
								if(-4 == (n12 + n13) * (n32 + n33))
									return true;
							}
						}
						else {
							// ��`�ɂ��ȈՐڐG����B
							if(aminx < max(b1x, b2x) &&
							   amaxx > min(b1x, b2x) &&
							   aminy < max(b1y, b2y) &&
							   amaxy > min(b1y, b2y))
							{
								if(((a1x - a2x) * (b1y - a1y) + (a1y - a2y) * (a1x - b1x)) *
								   ((a1x - a2x) * (b2y - a1y) + (a1y - a2y) * (a1x - b2x)) < 0 &&
								   ((b1x - b2x) * (a1y - b1y) + (b1y - b2y) * (b1x - a1x)) *
								   ((b1x - b2x) * (a2y - b1y) + (b1y - b2y) * (b1x - a2x)) < 0)
									return true;
							}
						}
					}

					b1x = b2x;
					b1y = b2y;
				}
			}

			a1x = a2x;
			a1y = a2y;
		}

		return false;
	}

private:
	/**
	 * @brief �ϕ������߂�A�炵��
	 *
	 * @param x0 [in] �������n�_X���W�B
	 * @param y0 [in] �������n�_Y���W�B
	 * @param x1 [in] ����1�I�_X���W�B
	 * @param y1 [in] ����1�I�_Y���W�B
	 * @param x2 [in] ����2�I�_X���W�B
	 * @param y2 [in] ����2�I�_Y���W�B
	 * @retval -1 ����1�͐���2�̉E���ɑ��݂���A�Ǝv���B
	 * @retval 0 ����1�͐���2�ƌ����������ł���B
	 * @retval 1 ����1�͐���2�̍����ɑ��݂���A�Ǝv���B
	 */
	int side(double x0, double y0, double x1, double y1, double x2, double y2)
	{
		double d = x0 * (y2 - y1) + x1 * (y0 - y2) + x2 * (y1 - y0);
		return d < 0 ? -1 : d > 0 ? 1 : 0;
	}
};

/**
 * @brief �`���Union����֐��I�u�W�F�N�g
 */
struct UnionGeometry : public std::binary_function<ITopologicalOperator*, IGeometry*, ITopologicalOperatorPtr>
{
	ITopologicalOperatorPtr operator ()(ITopologicalOperator* ipTopoOp, const Geometry& rGeometry)
	{
		return operator()(ipTopoOp, static_cast<IGeometry*>(rGeometry));
	}

	/**
	 * @brief ��P�����ɑ�Q�����̌`���Union�����`����쐬����
	 *
	 * @param ipTopoOp [in] ��`��B
	 * @param ipGeometry [in] ��`���Union����`��B
	 * @return ��P�����ɑ�Q�����̌`���Union�����`��BipTopoOp��NULL�̏ꍇ��ipGeometry��Clone��Ԃ��B
	 */
	ITopologicalOperatorPtr operator ()(ITopologicalOperator* ipTopoOp, IGeometry* ipGeometry)
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
};

/**
 * @brief �_��`�����������֐��I�u�W�F�N�g
 */
struct JointPointCollection : public std::binary_function<IPointCollection*, IPointCollection*, IPointCollectionPtr>
{
	IPointCollectionPtr operator ()(IPointCollection* ipSrcPointCollection, IPointCollection* ipJointPointCollection)
	{
		using namespace sindy;

		if(! ipJointPointCollection)
			return ipSrcPointCollection;

		if(ipSrcPointCollection != 0) {
			long nPointCount2;
			ipJointPointCollection->get_PointCount(&nPointCount2);
			if(nPointCount2 == 0)
				return ipSrcPointCollection;

			long nPointCount1;
			ipSrcPointCollection->get_PointCount(&nPointCount1);
			if(nPointCount1 == 0) {
				// �_���ǉ�����B
				ipSrcPointCollection->AddPointCollection(ipJointPointCollection);

				// �������ɓ_�񂪑��݂��Ȃ��ꍇ�̏����͂���ŏI���B
				return ipSrcPointCollection;
			}

			IPointPtr ipPoint2_1, ipPoint2_2;
			ipJointPointCollection->get_Point(0, &ipPoint2_1);
			ipJointPointCollection->get_Point(nPointCount2 - 1, &ipPoint2_2);

			for(int i = 0; i < 2; i++) {
				IPointPtr ipPoint1;
				ipSrcPointCollection->get_Point(i == 0 ? (nPointCount1 - 1) : 0, &ipPoint1);

				int nCompare;
				IPointCollectionPtr ipAddPointCollection;
				
				if(ipPoint1->Compare(ipPoint2_1, &nCompare) == S_OK && nCompare == 0) {
					ipAddPointCollection = clone(ipJointPointCollection);
					ipAddPointCollection->RemovePoints(0, 1);
				}
				else if(ipPoint1->Compare(ipPoint2_2, &nCompare) == S_OK && nCompare == 0) {
					ICurvePtr ipCurve = clone(ipJointPointCollection);
					ipCurve->ReverseOrientation();
					ipAddPointCollection = ipCurve;
					ipAddPointCollection->RemovePoints(0, 1);
				}

				if(ipAddPointCollection != 0) {
					if(i == 1) {
						ICurvePtr(ipSrcPointCollection)->ReverseOrientation();
					}
					ipSrcPointCollection->AddPointCollection(ipAddPointCollection);
					break;
				}
			}

			return ipSrcPointCollection;
		}
		else {
			return clone(ipJointPointCollection);
		}
	}
};

/**
 * @brief �Q�_�Ԃ̋��������߂�iZCL�g�p�j
 */
struct ZCLDistance
{
	/**
	 * @brief ���������߂�
	 *
	 * _Point1��IPoint*��IPointPtr�̂����ꂩ�ł��邱�Ƃ�z��B
	 *
	 * @note �킴�킴�e���v���[�g���\�b�h�ɂ��Ă���̂́AZCL��g�p���ɂ��̃��\�b�h��ǂݔ�΂����߁B
	 *
	 * @param ipPoint1 [in] �_�P�B
	 * @param ipPoint2 [in] �_�Q�B
	 * @return �����im�j�B
	 */
	template <typename _Point1>
	double operator()(_Point1 const& ipPoint1, IPoint* ipPoint2)
	{
		double dX1, dY1, dX2, dY2;
		ipPoint1->get_X(&dX1);
		ipPoint1->get_Y(&dY1);
		ipPoint2->get_X(&dX2);
		ipPoint2->get_Y(&dY2);

		return ZLonLat(dX1, dY1).getDistance(ZLonLat(dX2, dY2));
	}
};

/**
 * @brief �_�񒷌v���֐��N���X
 */
template<typename _PointDistanceFunction>
class PolylineLengthFunctor
{
public:
	PolylineLengthFunctor(_PointDistanceFunction f) :
	m_Function(f),
	m_dLength(0.0)
	{
	}

	void reset()
	{
		m_ipPoint = 0;
		m_dLength = 0.0;
	}

	double length()
	{
		return m_dLength;
	}

	void operator ()(IPoint* ipPoint)
	{
		if(m_ipPoint != 0) {
			m_dLength += m_Function(m_ipPoint, ipPoint);
		}

		m_ipPoint = ipPoint;
	}

private:
	_PointDistanceFunction m_Function;
	IPointPtr m_ipPoint;
	double m_dLength;
};

/**
 * @brief �_�񒷌v���֐��N���X�����֐�
 *
 * @param f [in] �Q�_�Ԃ̋��������߂�֐��B
 */
template<typename _PointDistanceFunction>
inline PolylineLengthFunctor<_PointDistanceFunction> make_PolylineLengthFunctor(_PointDistanceFunction f)
{
	return PolylineLengthFunctor<_PointDistanceFunction>(f);
}

} // namespace sindy

#endif // __SINDY_GEOMETRY_H__
