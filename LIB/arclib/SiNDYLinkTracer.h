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
	// 定義
		/**
		 * @brief 右左折禁止規制クラス
		 */
		class Turnreg
		{
		public:
			typedef std::vector<long> LinkIdList;

		// 構築/消滅
			Turnreg();
			Turnreg(_IRow* ipInfTurnreg);

			void clear();

		// 設定/取得
			bool setInfTurnreg(_IRow* ipInfTurnreg);

		// プロパティ
			LinkIdList m_cLinkIdList;
		};

		/**
		 * @brief オブジェクトID別右左折禁止規制集合
		 */
		class TurnregMap : public std::map<long, Turnreg>
		{
		public:
			bool setTurnreg(_ICursor* ipInfTurnregCursor, ITable* ipLqTurnregTable);
		};

		class Link;
		typedef std::pair<long, long> LinkIdPair;

		/**
		 * @brief ルート
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

		// 構築/消滅
			Route();

			void clear();

		// 設定/取得
			bool setRoute(const Route* pPrevRoute, long nNodeId, const Link* pLink);

			const Route* getFirstRoute() const;
			const Route* searchRoute(long nNodeId) const;

			void getRouteSet(std::set<const Route*>& rRouteSet) const;
			void getLinkIdList(LinkIdList& rLinkIdList) const;
			void getNodeIdList(NodeIdList& rNodeIdList) const;
			bool getNodeIdList(int nStartNodeId, int nEndNodeId, NodeIdList& rNodeIdList) const;
			void getNodeIdLinkList(NodeIdLinkList& rNodeIdLinkList) const;

			IPolylinePtr createPolyline() const;

		// プロパティ
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
		 * @brief 全ルート
		 */
		typedef std::map<LinkIdPair, RouteMap> _RouteMaster;
		class RouteMaster
		{
		public:
			typedef _RouteMaster::iterator iterator;
			typedef _RouteMaster::const_iterator const_iterator;
			typedef _RouteMaster::key_type key_type;

		// 構築/消滅
			RouteMaster();
			virtual ~RouteMaster();

			void clear();
			void clearOtherRoute(const RouteSet& rRouteSet);

		// 設定/取得
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
		 * @brief 道路リンククラス
		 */
		class Link
		{
		public:
		// 定義
			typedef std::set<const Link*> ConnectLinkSet;
			typedef std::set<const Turnreg*> TurnregSet;

		// 構築/消滅
			Link(IFeature* ipLink);

			virtual void clear();
			void clearConnection();
			void clearTurnreg();

		// 設定/取得
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

		// 静的関数
			static void addSubFieldsTo(IQueryFilter* ipQueryFilter);

		// プロパティ
			long m_nObjectId; ///< オブジェクトID
			long m_nFromNodeId; ///< 始点側ノードID
			long m_nToNodeId; ///< 終点側ノードID
			long m_nOneway; ///< 一方通行コード
			ConnectLinkSet m_cInLinkSet1; ///< 当該リンクにFromノード側から流入可能なリンク一覧
			ConnectLinkSet m_cInLinkSet2; ///< 当該リンクにToノード側から流入可能なリンク一覧
			ConnectLinkSet m_cOutLinkSet1; ///< 当該リンクのFromノード側から流出可能なリンク一覧
			ConnectLinkSet m_cOutLinkSet2; ///< 当該リンクのToノード側から流出可能なリンク一覧

			TurnregSet m_cTurnregSet; ///< 関係する右左折禁止規制

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

		// 静的関数
			static void addSubFieldsTo(IQueryFilter* ipQueryFilter);

		// 変数
			IGeometryPtr m_ipGeometry;
			double m_dWeight;

		protected:
			static IGeometryPtr getShapeCopyOf(IFeature* ipLink);
			static double getWeightOf(IFeature* ipLink);
		};

		/**
		 * @brief 追跡空間クラス
		 */
		class TraceSpace
		{
		public:
			typedef std::set<Link*> LinkSet;
			typedef std::map<long, LinkSet> LinkMap;
			typedef LinkMap InableLinkMap;
			typedef LinkMap OutableLinkMap;

		// 構築/消滅
			virtual void clear();

		// 設定/取得
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

			InableLinkMap m_cInableLinkMap; ///< 特定のノードへ流入可能なリンク
			OutableLinkMap m_cOutableLinkMap; ///< 特定のノードから流出可能なリンク
			TurnregMap m_cTurnregMap;
		};

		/**
		 * @brief 追跡を行うクラス
		 */
		class Tracer
		{
		public:
			typedef std::vector<Route> RouteList;

		// 構築/消滅
			Tracer(const TraceSpace& rTraceSpace);
			virtual ~Tracer();

			virtual void clear();
			void clearTracer();

		// 設定/取得
			bool setStartNode(long nInNodeId, long nOutNodeId = 0);
			bool addStartNode(long nInNodeId, long nOutNodeId = 0);

			bool setStartLink(long nLinkId);
			bool addStartLink(long nLinkId);

			const TraceSpace& getTraceSpace() const { return m_rTraceSpace; }
			const RouteMaster& getRouteMaster() const { return m_cRouteMaster; }
			const Route* getCurrentRoute();

			void getRouteMapToNode(long nNodeId, RouteMap& rRouteMap) const;

			virtual const Link::ConnectLinkSet* getNextLinkSet(const Route& rRoute) const;

		// 処理
			void skipTrace();

			const Route* traceLinkToLink(long nStartLinkId, long nEndLinkId);

		// 純粋仮想関数
			virtual bool stepTrace() = 0;

		protected:
			bool addStartLink(const Link* pLink);
			bool addStartLink(long nInNodeId, const Link* pLink);

		// 純粋仮想関数
			virtual RouteMap::iterator getCurrent() = 0;

		// プロパティ
			const TraceSpace& m_rTraceSpace; ///< 追跡空間
			RouteMaster m_cRouteMaster; ///< 全ルートの集合
			RouteMap m_cRouteMap; ///< 追跡を続けるルートの集合
		};

		/**
		 * @brief ダイクストラによる追跡を行うクラス
		 */
		class DijkstraTracer : public Tracer
		{
		public:
		// 構築/消滅
			DijkstraTracer(const TraceSpace& rTraceSpace);

		// 純粋仮想関数の解決
			virtual bool stepTrace();

		protected:
		// 処理
			virtual void stepTrace(double dDepth, Route* pRoute);
			virtual void stepTrace(Route* pRoute, long nNextNodeId);
			virtual void stepTrace(Route* pRoute, long nNextNodeId, const Link* pNextLink);

		// 純粋仮想関数の解決
			virtual RouteMap::iterator getCurrent();
		};

		/**
		 * @brief 追跡空間クラステンプレート
		 */
		template <typename T>
		class TraceSpaceT : public TraceSpace
		{
		public:
		// 定義
			typedef std::map<long, T> LinkMap;

		// 設定/取得
			const LinkMap& linkMap() const { return m_cLinkMap; }

			virtual const Link* searchLink(long nLinkId) const
			{
				LinkMap::const_iterator it = m_cLinkMap.find(nLinkId);
				return it != m_cLinkMap.end() ? &it->second : 0;
			}

		protected:
		// 構築/消滅
			virtual void clearLink()
			{
				m_cLinkMap.clear();
			}

		// 設定/取得
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
		// プロパティ
			LinkMap m_cLinkMap;
		};
	}
}

#define SiNDYLinkTracer sindy::LinkTracer

#endif // !defined(AFX_SINDYLINKTRACER_H__C8619591_C457_4528_B5D9_921387922F47__INCLUDED_)
