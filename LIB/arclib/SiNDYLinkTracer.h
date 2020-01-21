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

// SiNDYLinkTracer.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYLINKTRACER_H__C8619591_C457_4528_B5D9_921387922F47__INCLUDED_)
#define AFX_SINDYLINKTRACER_H__C8619591_C457_4528_B5D9_921387922F47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include <map>
#include <hash_set>
#include <hash_map>

namespace sindy
{
	namespace LinkTracer  
	{
	// ��`
		/**
		 * @brief �E���܋֎~�K���N���X
		 */
		class Turnreg
		{
		public:
			typedef std::vector<long> LinkIdList;

		// �\�z/����
			Turnreg();
			Turnreg(_IRow* ipInfTurnreg);

			void clear();

		// �ݒ�/�擾
			bool setInfTurnreg(_IRow* ipInfTurnreg);

		// �v���p�e�B
			LinkIdList m_cLinkIdList;
		};

		/**
		 * @brief �I�u�W�F�N�gID�ʉE���܋֎~�K���W��
		 */
		class TurnregMap : public std::map<long, Turnreg>
		{
		public:
			bool setTurnreg(_ICursor* ipInfTurnregCursor, ITable* ipLqTurnregTable);
		};

		class Link;
		typedef std::pair<long, long> LinkIdPair;

		/**
		 * @brief ���[�g
		 */
		class Route
		{
		public:
			class NodeIdLink
			{
			public:
				NodeIdLink(long nNodeId, const Link* pLink) :
				m_nNodeId(nNodeId),
				m_pLink(pLink)
				{
				}

				long m_nNodeId;
				const Link* m_pLink;
			};

			typedef std::vector<NodeIdLink> NodeIdLinkList;
			typedef std::vector<long> LinkIdList;
			typedef std::vector<long> NodeIdList;

		// �\�z/����
			Route();

			void clear();

		// �ݒ�/�擾
			bool setRoute(const Route* pPrevRoute, long nNodeId, const Link* pLink);

			const Route* getFirstRoute() const;
			const Route* searchRoute(long nNodeId) const;

			void getRouteSet(std::set<const Route*>& rRouteSet) const;
			void getLinkIdList(LinkIdList& rLinkIdList) const;
			void getNodeIdList(NodeIdList& rNodeIdList) const;
			bool getNodeIdList(int nStartNodeId, int nEndNodeId, NodeIdList& rNodeIdList) const;
			void getNodeIdLinkList(NodeIdLinkList& rNodeIdLinkList) const;

			IPolylinePtr createPolyline() const;

		// �v���p�e�B
			long m_nNodeId;
			const Link* m_pLink;
			double m_dDepth;
			const Route* m_pPrevRoute;
			std::map<const Turnreg*, long> m_cTurnregMap;
		};

		typedef std::multimap<double, Route*> RouteMap;
		typedef std::set<const Route*> RouteSet;

		/**
		 * @class RouteMaster
		 * @brief �S���[�g
		 */
		typedef std::map<LinkIdPair, RouteMap> _RouteMaster;
		class RouteMaster
		{
		public:
			typedef _RouteMaster::iterator iterator;
			typedef _RouteMaster::const_iterator const_iterator;
			typedef _RouteMaster::key_type key_type;

		// �\�z/����
			RouteMaster();
			virtual ~RouteMaster();

			void clear();
			void clearOtherRoute(const RouteSet& rRouteSet);

		// �ݒ�/�擾
			void setMaxRouteCount(int nMaxRouteCount) { m_nMaxRouteCount = nMaxRouteCount; }
			unsigned int maxRouteCount() const { return m_nMaxRouteCount; }

			virtual Route* addRoute(const Route* pPrevRoute, long nNodeId, const Link* pNextLink);

			iterator begin() { return m_cRouteMaster.begin(); }
			const_iterator begin() const { return m_cRouteMaster.begin(); }

			iterator end() { return m_cRouteMaster.end(); }
			const_iterator end() const { return m_cRouteMaster.end(); }

			iterator find(const key_type& rKey) { return m_cRouteMaster.find(rKey); }
			const_iterator find(const key_type& rKey) const { return m_cRouteMaster.find(rKey); }

			void getRouteMapToLink(long nLinkId, RouteMap& rRouteMap) const;

		private:
			_RouteMaster m_cRouteMaster;
			unsigned int m_nMaxRouteCount;
		};

		/**
		 * @brief ���H�����N�N���X
		 */
		class Link
		{
		public:
		// ��`
			typedef std::set<const Link*> ConnectLinkSet;
			typedef std::set<const Turnreg*> TurnregSet;

		// �\�z/����
			Link(IFeature* ipLink);

			virtual void clear();
			void clearConnection();
			void clearTurnreg();

		// �ݒ�/�擾
			void addTurnreg(const Turnreg* pTurrneg);
			bool addConnection(Link* pNextLink);

			long getOnewayFromNodeId() const;
			long getOnewayToNodeId() const;
			long getOppositeNodeId(long nNodeId) const;

			bool isInableFrom(long nNodeId) const;
			bool isOutableTo(long nNodeId) const;

			virtual double getWeight(const Route*) const { return 1.0; }
			virtual IGeometryPtr getShape() const { return 0; }
			virtual IGeometryPtr getShapeCopy() const;

		// �ÓI�֐�
			static void addSubFieldsTo(IQueryFilter* ipQueryFilter);

		// �v���p�e�B
			long m_nObjectId; ///< �I�u�W�F�N�gID
			long m_nFromNodeId; ///< �n�_���m�[�hID
			long m_nToNodeId; ///< �I�_���m�[�hID
			long m_nOneway; ///< ����ʍs�R�[�h
			ConnectLinkSet m_cInLinkSet1; ///< ���Y�����N��From�m�[�h�����痬���\�ȃ����N�ꗗ
			ConnectLinkSet m_cInLinkSet2; ///< ���Y�����N��To�m�[�h�����痬���\�ȃ����N�ꗗ
			ConnectLinkSet m_cOutLinkSet1; ///< ���Y�����N��From�m�[�h�����痬�o�\�ȃ����N�ꗗ
			ConnectLinkSet m_cOutLinkSet2; ///< ���Y�����N��To�m�[�h�����痬�o�\�ȃ����N�ꗗ

			TurnregSet m_cTurnregSet; ///< �֌W����E���܋֎~�K��

		protected:
			bool setLink(IFeature* ipLink);
			bool addConnection(long nNodeId, Link* pNextLink);
		};

		class Link2 : public Link
		{
		public:
			Link2(IFeaturePtr ipLink);
			Link2(IFeaturePtr ipLink, double dWeight);

			virtual void clear();

			double getWeight() const { return m_dWeight; }

			virtual double getWeight(const Route*) const { return m_dWeight; }
			virtual IGeometryPtr getShape() const { return m_ipGeometry; }

		// �ÓI�֐�
			static void addSubFieldsTo(IQueryFilter* ipQueryFilter);

		// �ϐ�
			IGeometryPtr m_ipGeometry;
			double m_dWeight;

		protected:
			static IGeometryPtr getShapeCopyOf(IFeature* ipLink);
			static double getWeightOf(IFeature* ipLink);
		};

		/**
		 * @brief �ǐՋ�ԃN���X
		 */
		class TraceSpace
		{
		public:
			typedef std::set<Link*> LinkSet;
			typedef std::map<long, LinkSet> LinkMap;
			typedef LinkMap InableLinkMap;
			typedef LinkMap OutableLinkMap;

		// �\�z/����
			virtual void clear();

		// �ݒ�/�擾
			virtual bool setLink(IFeatureCursor* ipLinkCursor, _ICursor* ipInfTurnregCursor = 0, ITable* ipLqTurnregTable = 0);

			virtual void addLink(IFeaturePtr ipLink);
			virtual void setTurnreg(_ICursor* ipInfTurnregCursor, ITable* ipLqTurnregTable);
			virtual void culminate();

			virtual const InableLinkMap& inableLinkMap() const { return m_cInableLinkMap; }
			virtual const OutableLinkMap& outableLinkMap() const { return m_cOutableLinkMap; }

			virtual const LinkSet* searchInableLinkSetTo(long nNodeId) const;
			virtual const LinkSet* searchOutableLinkSetFrom(long nNodeId) const;

			virtual const Link* searchLink(long nLinkId) const = 0;
			virtual Link* searchLink(long nLinkId) = 0;

		protected:
			virtual void clearLink() = 0;
			virtual Link* addLinkImpl(long nObjectId, IFeaturePtr ipLink) = 0;

			virtual void culminateTurnreg();
			virtual void culminateConnection();

			void addInableLink(long nNodeId, Link& rLink);
			void addOutableLink(long nNodeId, Link& rLink);
			void addConnection(LinkSet& rInLinkSet, LinkSet& rOutLinkSet);

			const LinkSet* searchConnectLinkSet(long nNodeId, const LinkMap& rLinkMap) const;

			InableLinkMap m_cInableLinkMap; ///< ����̃m�[�h�֗����\�ȃ����N
			OutableLinkMap m_cOutableLinkMap; ///< ����̃m�[�h���痬�o�\�ȃ����N
			TurnregMap m_cTurnregMap;
		};

		/**
		 * @brief �ǐՂ��s���N���X
		 */
		class Tracer
		{
		public:
			typedef std::vector<Route> RouteList;

		// �\�z/����
			Tracer(const TraceSpace& rTraceSpace);
			virtual ~Tracer();

			virtual void clear();
			void clearTracer();

		// �ݒ�/�擾
			bool setStartNode(long nInNodeId, long nOutNodeId = 0);
			bool addStartNode(long nInNodeId, long nOutNodeId = 0);

			bool setStartLink(long nLinkId);
			bool addStartLink(long nLinkId);

			const TraceSpace& getTraceSpace() const { return m_rTraceSpace; }
			const RouteMaster& getRouteMaster() const { return m_cRouteMaster; }
			const Route* getCurrentRoute();

			void getRouteMapToNode(long nNodeId, RouteMap& rRouteMap) const;

			virtual const Link::ConnectLinkSet* getNextLinkSet(const Route& rRoute) const;

		// ����
			void skipTrace();

			const Route* traceLinkToLink(long nStartLinkId, long nEndLinkId);

		// �������z�֐�
			virtual bool stepTrace() = 0;

		protected:
			bool addStartLink(const Link* pLink);
			bool addStartLink(long nInNodeId, const Link* pLink);

		// �������z�֐�
			virtual RouteMap::iterator getCurrent() = 0;

		// �v���p�e�B
			const TraceSpace& m_rTraceSpace; ///< �ǐՋ��
			RouteMaster m_cRouteMaster; ///< �S���[�g�̏W��
			RouteMap m_cRouteMap; ///< �ǐՂ𑱂��郋�[�g�̏W��
		};

		/**
		 * @brief �_�C�N�X�g���ɂ��ǐՂ��s���N���X
		 */
		class DijkstraTracer : public Tracer
		{
		public:
		// �\�z/����
			DijkstraTracer(const TraceSpace& rTraceSpace);

		// �������z�֐��̉���
			virtual bool stepTrace();

		protected:
		// ����
			virtual void stepTrace(double dDepth, Route* pRoute);
			virtual void stepTrace(Route* pRoute, long nNextNodeId);
			virtual void stepTrace(Route* pRoute, long nNextNodeId, const Link* pNextLink);

		// �������z�֐��̉���
			virtual RouteMap::iterator getCurrent();
		};

		/**
		 * @brief �ǐՋ�ԃN���X�e���v���[�g
		 */
		template <typename T>
		class TraceSpaceT : public TraceSpace
		{
		public:
		// ��`
			typedef std::map<long, T> LinkMap;

		// �ݒ�/�擾
			const LinkMap& linkMap() const { return m_cLinkMap; }

			virtual const Link* searchLink(long nLinkId) const
			{
				LinkMap::const_iterator it = m_cLinkMap.find(nLinkId);
				return it != m_cLinkMap.end() ? &it->second : 0;
			}

		protected:
		// �\�z/����
			virtual void clearLink()
			{
				m_cLinkMap.clear();
			}

		// �ݒ�/�擾
			virtual Link* addLinkImpl(long nObjectId, IFeaturePtr ipLink)
			{
				std::pair<LinkMap::iterator, bool> cPair = m_cLinkMap.insert(LinkMap::value_type(nObjectId, T(ipLink)));
				return cPair.second ? &cPair.first->second : false;
			}

			virtual Link* searchLink(long nLinkId)
			{
				LinkMap::iterator it = m_cLinkMap.find(nLinkId);
				return it != m_cLinkMap.end() ? &it->second : 0;
			}

		private:
		// �v���p�e�B
			LinkMap m_cLinkMap;
		};
	}
}

#define SiNDYLinkTracer sindy::LinkTracer

#endif // !defined(AFX_SINDYLINKTRACER_H__C8619591_C457_4528_B5D9_921387922F47__INCLUDED_)
