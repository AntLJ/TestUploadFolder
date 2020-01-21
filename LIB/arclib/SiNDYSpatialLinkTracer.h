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

		// 構築/消滅
			SquareTraceSpace(typename SquareHash::HashFunc pfHash) :
			m_SquareHash(pfHash)
			{
			}

		// 設定/取得
			const SquareHash& getSquareHash() const { return m_SquareHash; }

			/**
			 * @brief 指定した座標に存在するノードへ流入可能なリンク一覧を取得する
			 *
			 * @param ipPoint [in] 座標。
			 * @return リンク一覧。指定した座標にノードが存在しないか、流入可能なリンクが一つも存在しない場合はNULL。
			 */
			const LinkSet* getInableLinkSetTo(IPoint* ipPoint) const
			{
				return searchConnectLinkSet(getNodeId(ipPoint), inableLinkMap());
			}

			/**
			 * @brief 指定した座標に存在するノードから流出可能なリンク一覧を取得する
			 *
			 * @param ipPoint [in] 座標。
			 * @return リンク一覧。指定した座標にノードが存在しないか、流出可能なリンクが一つも存在しない場合はNULL。
			 */
			const LinkSet* getOutableLinkSetFrom(IPoint* ipPoint) const
			{
				return searchConnectLinkSet(getNodeId(ipPoint), outableLinkMap());
			}

			/**
			 * @brief 指定した座標に存在するノードのIDを取得する
			 *
			 * @param ipPoint [in] 対象座標
			 * @return ノードID。指定した座標にノードが存在しなかった場合NULL。
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
			 * @brief 指定した座標の近くに存在するリンクを取得する
			 *
			 * @param ipPoint [in] 対象座標
			 * @param dTolerance [in] 閾値（度）
			 * @param rNearLinkMap [out] リンク一覧
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
		// 構築/消滅
			virtual void clearLink()
			{
				TraceSpaceT<T>::clearLink();
				m_SquareHash.clear();
			}

		// 設定/取得
			virtual Link* addLinkImpl(long nObjectId, IFeaturePtr ipLink)
			{
				// 基底クラスの実装を呼び出し
				T* pLink = static_cast<T*>(TraceSpaceT<T>::addLinkImpl(nObjectId, ipLink));
				if(! pLink)
					return 0;

				// 形状を取得し、ハッシュに配置する
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
		// プロパティ
			SquareHash m_SquareHash;
		};
	}
}

#endif // __SPATIAL_LINK_TRACER_H__ //
