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

#ifndef __SINDY_SPATIAL_LINK_TRACER_H__
#define __SINDY_SPATIAL_LINK_TRACER_H__

#include "SiNDYLinkTracer.h"
#include "SquareHash.h" //< GeoLib

namespace sindy
{
	namespace LinkTracer
	{
		template <typename T>
		class SquareTraceSpace : public TraceSpaceT<T>
		{
		public:
			typedef SquareHashT<const T*> SquareHash;
			typedef std::multimap<double, typename SquareHash::Member> NearLinkMap;

		// �\�z/����
			SquareTraceSpace(typename SquareHash::HashFunc pfHash) :
			m_SquareHash(pfHash)
			{
			}

		// �ݒ�/�擾
			const SquareHash& getSquareHash() const { return m_SquareHash; }

			/**
			 * @brief �w�肵�����W�ɑ��݂���m�[�h�֗����\�ȃ����N�ꗗ���擾����
			 *
			 * @param ipPoint [in] ���W�B
			 * @return �����N�ꗗ�B�w�肵�����W�Ƀm�[�h�����݂��Ȃ����A�����\�ȃ����N��������݂��Ȃ��ꍇ��NULL�B
			 */
			const LinkSet* getInableLinkSetTo(IPoint* ipPoint) const
			{
				return searchConnectLinkSet(getNodeId(ipPoint), inableLinkMap());
			}

			/**
			 * @brief �w�肵�����W�ɑ��݂���m�[�h���痬�o�\�ȃ����N�ꗗ���擾����
			 *
			 * @param ipPoint [in] ���W�B
			 * @return �����N�ꗗ�B�w�肵�����W�Ƀm�[�h�����݂��Ȃ����A���o�\�ȃ����N��������݂��Ȃ��ꍇ��NULL�B
			 */
			const LinkSet* getOutableLinkSetFrom(IPoint* ipPoint) const
			{
				return searchConnectLinkSet(getNodeId(ipPoint), outableLinkMap());
			}

			/**
			 * @brief �w�肵�����W�ɑ��݂���m�[�h��ID���擾����
			 *
			 * @param ipPoint [in] �Ώۍ��W
			 * @return �m�[�hID�B�w�肵�����W�Ƀm�[�h�����݂��Ȃ������ꍇNULL�B
			 */
			long getNodeId(IPoint* ipPoint) const
			{
				if(! ipPoint)
					return 0;

				double dX, dY;
				ipPoint->get_X(&dX);
				ipPoint->get_Y(&dY);

				const SquareHash::MemberSet* pMemberSet = getSquareHash().searchMemberSet(dX, dY);
				if(! pMemberSet)
					return 0;

				for(SquareHash::MemberSet::const_iterator it = pMemberSet->begin(); it != pMemberSet->end(); ++it) {
					const Link* pLink = *it;

					IPointCollectionPtr ipPointCol = pLink->getShape();
					if(ipPointCol == 0)
						continue;

					int nCompare;

					IPointPtr ipPoint1;
					ipPointCol->get_Point(0, &ipPoint1);
					ipPoint1->Compare(ipPoint, &nCompare);
					if(nCompare == 0)
						return pLink->m_nFromNodeId;

					long nPointCount;
					ipPointCol->get_PointCount(&nPointCount);
					if(nPointCount == 0)
						continue;

					IPointPtr ipPoint2;
					ipPointCol->get_Point(nPointCount - 1, &ipPoint2);
					ipPoint2->Compare(ipPoint, &nCompare);
					if(nCompare == 0)
						return pLink->m_nToNodeId;
				}

				return 0;
			}

			/**
			 * @brief �w�肵�����W�̋߂��ɑ��݂��郊���N���擾����
			 *
			 * @param ipPoint [in] �Ώۍ��W
			 * @param dTolerance [in] 臒l�i�x�j
			 * @param rNearLinkMap [out] �����N�ꗗ
			 */
			void getNearLinkMap(IPoint* ipPoint, double dTolerance, NearLinkMap& rNearLinkMap)
			{
				if(! ipPoint)
					return;

				double dX, dY;
				ipPoint->get_X(&dX);
				ipPoint->get_Y(&dY);

				SquareHash::MemberSet aMemberSet;
				getSquareHash().getMemberSet(dX - dTolerance, dY - dTolerance, dX + dTolerance, dY + dTolerance, aMemberSet);

				if(aMemberSet.empty())
					return;

				ISpatialReferencePtr ipSpRef;
				ipPoint->get_SpatialReference(&ipSpRef);

				IProximityOperatorPtr ipProOp(ipPoint);

				for(SquareHash::MemberSet::const_iterator it = aMemberSet.begin(); it != aMemberSet.end(); ++it) {
					const Link* pLink = *it;

					IGeometryPtr ipGeometry = pLink->getShapeCopy();
					if(ipGeometry == 0)
						continue;

					if(ipGeometry->putref_SpatialReference(ipSpRef) != S_OK || ipGeometry->SnapToSpatialReference() != S_OK)
						continue;

					double dDistance;
					ipProOp->ReturnDistance(ipGeometry, &dDistance);
					if(dDistance <= dTolerance) {
						rNearLinkMap.insert(NearLinkMap::value_type(dDistance, *it));
					}
				}
			}

		protected:
		// �\�z/����
			virtual void clearLink()
			{
				TraceSpaceT<T>::clearLink();
				m_SquareHash.clear();
			}

		// �ݒ�/�擾
			virtual Link* addLinkImpl(long nObjectId, IFeaturePtr ipLink)
			{
				// ���N���X�̎������Ăяo��
				T* pLink = static_cast<T*>(TraceSpaceT<T>::addLinkImpl(nObjectId, ipLink));
				if(! pLink)
					return 0;

				// �`����擾���A�n�b�V���ɔz�u����
				IGeometryPtr ipGeometry = pLink->getShape();
				if(ipGeometry != 0) {
					IEnvelopePtr ipEnv;
					if(ipGeometry->get_Envelope(&ipEnv) == S_OK && ipEnv != 0) {
						double dXMin, dYMin, dXMax, dYMax;
						ipEnv->get_XMin(&dXMin);
						ipEnv->get_YMin(&dYMin);
						ipEnv->get_XMax(&dXMax);
						ipEnv->get_YMax(&dYMax);

						const T* pConstLink = pLink;
						m_SquareHash.insert(pConstLink, dXMin, dYMin, dXMax, dYMax);
					}
				}

				return pLink;
			}

		private:
		// �v���p�e�B
			SquareHash m_SquareHash;
		};
	}
}

#endif // __SPATIAL_LINK_TRACER_H__ //
