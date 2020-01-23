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

#include "StdAfx.h"
#include <queue>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <rns/functional.h>
#include <rns/graph/vq_regulation/breadth_first_search.h>
#include "VQRegulationsPropertyMap.h"
#include "UpperLevelFilter.h"
#include "CheckUpperReachability.h"

namespace vqreg_graph = rns::graph::vq_regulation;

namespace {

class VQRegsIterator :
	public boost::iterator_adaptor<VQRegsIterator, BidirVQRegRefs::const_iterator, const VertexQueue>
{
	friend class boost::iterator_core_access;
public:
	VQRegsIterator(base_type it, bool bReverse) :
	iterator_adaptor_(it),
	m_pFunc(bReverse ? &BidirectionalVQRegulation::reverse : &BidirectionalVQRegulation::fore)
	{
	}

private:
	const VertexQueue& dereference() const
	{
		return ((*base())->*m_pFunc)();
	}

	const VertexQueue& (BidirectionalVQRegulation::*m_pFunc)() const;
};

typedef boost::iterator_range<VQRegsIterator> VQRegsRange;

inline VQRegsRange make_range(const BidirVQRegRefs& rBidirVQRegRefs, bool bReverse)
{
	return
		VQRegsRange(
			VQRegsIterator(rBidirVQRegRefs.begin(), bReverse),
			VQRegsIterator(rBidirVQRegRefs.end(), bReverse)
		);
}

/**
 * @brief 最上層のリンクに到達したことを示す<strong>正常系</strong>例外
 *
 * 正常系の例外は好きではないが、BGL式の経路探索アルゴリズムを中断する方法はこれくらいしか浮かばない。
 */
struct Reachable {};

/**
 * @brief 指定した頂点は１点の通行規制に引っかかるか。
 *
 * @param v [in] 頂点。
 * @param rVertexBidirVQRegRefsMap [in] 頂点に関連付いている頂点列規制一覧。
 * @param bReverse [in] 通行規制を逆方向から見るか。
 * @retval true １点の通行規制に引っかかる。
 * @retval false １点の通行規制には引っかからない。
 */
bool is_regulated_vertex(
	Graph::vertex_descriptor v,
	const VertexBidirVQRegRefsMap& rVertexBidirVQRegRefsMap,
	bool bReverse
)
{
	const BidirVQRegRefs* pBidirVQRegRefs = rns::make_mapped_value_referer(rVertexBidirVQRegRefsMap)(v);
	return
		pBidirVQRegRefs &&
		vqreg_graph::typical_path_length(make_range(*pBidirVQRegRefs, bReverse), v, (int*)0, (int*)0) == 0;
}

/**
 * @brief 指定した頂点から流出可能な辺は存在するか。
 *
 * @param rGraph [in] グラフ。
 * @param u [in] 頂点。
 * @param rVertexBidirVQRegRefsMap [in] 頂点に関連付いている頂点列規制一覧。
 * @param bReverse [in] 通行規制を逆方向から見るか。
 *
 * @note
 * 求めるのは「流出可能な辺」の有無であり、「流出可能な頂点」ではない。<br>
 * 前者が真で後者が偽となる状況は唯一つ、「流出先頂点に１点規制が入っている」場合のみ。
 */
bool has_out_edge(
	const Graph& rGraph,
	const Graph::vertex_descriptor u,
	const VertexBidirVQRegRefsMap& rVertexBidirVQRegRefsMap,
	bool bReverse
)
{
	using namespace boost;

	const BidirVQRegRefs* pBidirVQRegRefs = rns::make_mapped_value_referer(rVertexBidirVQRegRefsMap)(u);

	Graph::out_edge_iterator ei, ei_end;
	boost::tie(ei, ei_end) = out_edges(u, rGraph);
	if(ei != ei_end && ! pBidirVQRegRefs)
		return true;

	for(; ei != ei_end; ei++) {
		_ASSERT(source(*ei, rGraph) == u);
		if(vqreg_graph::typical_path_length(make_range(*pBidirVQRegRefs, bReverse), target(*ei, rGraph), &u, &u + 1) > 0)
			return true;
	}

	return false;
}

using boost::pool_allocator;
using boost::fast_pool_allocator;

typedef vqreg_graph::path<Graph::vertex_descriptor> GraphPath; ///< 先行頂点
typedef std::list<GraphPath, fast_pool_allocator<GraphPath> > GraphPaths; ///< 先行頂点列

typedef std::vector<GraphPaths, pool_allocator<GraphPaths> > GraphPathsMap; ///< 先行頂点列プロパティマップ
typedef std::queue<const GraphPath*, std::deque<const GraphPath*, pool_allocator<const GraphPath*> > > Queue; ///< 先行頂点列ポインタキュー

typedef std::vector<long, pool_allocator<long> > VertexStack;

/**
 * @brief 上層の経路探索キューに追加する経路探索開始点スタック
 *
 * @note
 * このクラスのメンバ関数は全てスレッドセーフである。
 */
class TraceBridge : boost::noncopyable
{
public:
	TraceBridge() :
	m_hEnd(::CreateEvent(NULL, TRUE, FALSE, NULL), ::CloseHandle),
	m_hHas(::CreateEvent(NULL, TRUE, FALSE, NULL), ::CloseHandle)
	{
	}

	/**
	 * @brief 新しい経路探索開始点が追加されるまで待つ。
	 *
	 * @retval true 新しい経路探索開始点が追加された。
	 * @retval false 下層の経路探索は終了した。新しい経路探索開始点が追加されることはない。
	 * @post 経路探索開始点保有イベントオブジェクトが非シグナル状態になる。
	 */
	bool wait()
	{
		HANDLE hs[2] = { m_hHas.get(), m_hEnd.get() };
		return ::WaitForMultipleObjects(2, hs, FALSE, INFINITE) == WAIT_OBJECT_0;
	}

	/**
	 * @brief 新しい経路探索開始点を追加する。
	 *
	 * @post 経路探索開始点保有イベントオブジェクトが非シグナル状態になる。
	 */
	void add(long nNodeID)
	{
		const boost::mutex::scoped_lock aLock(m_Mutex);
		if(! m_pStack.get()) {
			m_pStack.reset(new VertexStack);
		}
		m_pStack->push_back(nNodeID);
		::SetEvent(m_hHas.get());
	}

	/**
	 * @brief 新しい経路探索点の一覧を取り出す。
	 * 
	 * @return 新しい経路探索点の一覧。
	 */
	std::auto_ptr<VertexStack> release()
	{
		const boost::mutex::scoped_lock aLock(m_Mutex);
		::ResetEvent(m_hHas.get());
		return m_pStack;
	}

	/**
	 * @brief 下層の経路探索が終了したことを伝える。
	 */
	void finish()
	{
		::SetEvent(m_hEnd.get());
	}

private:
	std::auto_ptr<VertexStack> m_pStack; ///< 経路探索開始点一覧。
	boost::shared_ptr<void> m_hEnd; ///< 下層の経路探索が終了した時にシグナル状態にしてもらうイベントオブジェクトのハンドル。
	boost::shared_ptr<void> m_hHas; ///< 下層から上層へ、新たな経路が発見された時にシグナル状態にするイベントオブジェクトのハンドル。
	boost::mutex m_Mutex; ///< m_pStack 用ミューテクス。
};

/**
 * @brief 経路探索用ビジタ用情報・基本
 *
 * 経路探索アルゴリズム開始前に予め設定しておく情報をまとめた構造体。
 */
struct VisitorDataBase
{
	VisitorDataBase(
		const Region& rRgn,
		RegionPyramid::const_iterator first,
		RegionPyramid::const_iterator last,
		bool bReverse,
		HANDLE hReachableEvent
	) :
	m_rRgn(rRgn),
	m_it(first),
	m_itEnd(last),
	m_bReverse(bReverse),
	m_hReachableEvent(hReachableEvent),
	m_PathMap(boost::num_vertices(*rRgn.m_pRegionGraphData->m_pBasicGraphData->m_pGraph))
	{
	}
	
	const Region& m_rRgn; ///< 現在のリージョン。
	const RegionPyramid::const_iterator m_it, m_itEnd; ///< より上位のリージョン一覧。
	const bool m_bReverse; ///< 方向規制逆転モード。
	const HANDLE m_hReachableEvent; ///< 最上層に到達できたときにシグナル状態となるイベントオブジェクトのハンドル。

	Queue m_Queue; ///< 先行頂点列ポインタキュー。
	GraphPathsMap m_PathMap; ///< 先行頂点情報配列。

	boost::shared_ptr<TraceBridge> m_pTraceBridge; ///< 下層から現在の層へ辿り付いた経路が登録される経路探索開始点スタック。最下層ではNULL。
};

/**
 * @brief 経路探索用ビジタ用情報・拡張
 *
 * 上層の経路探索を開始した新しいスレッドとのやりとりを行うための構造体。
 */
struct VisitorData
{
	VisitorData(const boost::shared_ptr<VisitorDataBase>& pBase) :
	m_pBase(pBase)
	{
	}

	const boost::shared_ptr<VisitorDataBase> m_pBase; ///< 基本情報。

	boost::shared_ptr<TraceBridge> m_pUpTraceBridge; ///< 現在の層から上層へ辿り付く経路を登録する経路探索開始点スタック。上層の経路探索が開始されるまでNULL。
	boost::shared_ptr<boost::thread> m_pUpThread; ///< 上層の経路探索を行うスレッド。上層の経路探索が開始されるまでNULL。
};

void check(const boost::shared_ptr<VisitorDataBase>& pVisDataBase);

/**
 * @brief 経路探索用ビジタ
 */
struct Visitor
{
	Visitor(VisitorData& rData) :
	m_rData(rData)
	{
	}

	template <typename Vertex, typename Graph>
	void examine_vertex(Vertex v, const Graph& rGraph)
	{
		// どこかのスレッドが最上層まで辿り付いたら、経路探索は終了する。
		if(::WaitForSingleObject(m_rData.m_pBase->m_hReachableEvent, 0) == WAIT_OBJECT_0)
			throw Reachable();

		using namespace boost;
		using namespace sindy::schema::category::directional_link_relational_table::link_dir;

		const Region& rUpRgn = *m_rData.m_pBase->m_it;
		const RegionGraphData& rUpRgnGraphData = *rUpRgn.m_pRegionGraphData;
		const BasicGraphData& rUpBasicGraphData = *rUpRgnGraphData.m_pBasicGraphData;
		const NodeVertexMap& rUpNodeVertexMap = *rUpBasicGraphData.m_pNodeVertexMap;

		const long& nNodeID = get(vertex_index1, rGraph, v);

		// 上層のグラフに存在する道路ノードか。
		const Graph::vertex_descriptor* ps(rns::make_mapped_value_referer(rUpNodeVertexMap)(nNodeID));
		if(ps) {
			if(m_rData.m_pUpTraceBridge) {
				// 既に上層の経路探索が開始されていたら、経路探索開始点を上層用のスタックに追加。
				m_rData.m_pUpTraceBridge->add(nNodeID);
			}
			else {
				const RegionPyramid::const_iterator
					it(boost::next(m_rData.m_pBase->m_it)), it_end(m_rData.m_pBase->m_itEnd);

				if(it == it_end) {
					// 2つ上の層が存在せず、1つ上の道路リンクに到達できたなら、ここで経路探索終了。
					if(
						has_out_edge(
							*rUpBasicGraphData.m_pGraph,
							*ps,
							*rUpRgnGraphData.m_pVertexBidirVQRegRefsMap,
							m_rData.m_pBase->m_bReverse
						)
					) {
						::SetEvent(m_rData.m_pBase->m_hReachableEvent);
						throw Reachable();
					}
				}
				else {
					const boost::shared_ptr<VisitorDataBase>
						pUpDataBase(
							new VisitorDataBase(
								rUpRgn,
								it,
								it_end,
								m_rData.m_pBase->m_bReverse,
								m_rData.m_pBase->m_hReachableEvent
							)
						);

					if(vqreg_graph::push_path(pUpDataBase->m_Queue, pUpDataBase->m_PathMap[*ps], GraphPath(*ps))) {
						// 上層経路探索開始点スタックを作成し、ビジタ用拡張情報に登録。
						pUpDataBase->m_pTraceBridge.reset(new TraceBridge);
						m_rData.m_pUpTraceBridge = pUpDataBase->m_pTraceBridge;

						// 上層経路探索スレッドを立ち上げ、ビジタ用拡張情報に登録。
						m_rData.m_pUpThread.reset(new boost::thread(boost::lambda::bind(&check, pUpDataBase)));
					}
				}
			}
		}
	}

	template <typename Vertex, typename Graph>
	void finish_vertex(Vertex, const Graph&)
	{
		if(! m_rData.m_pBase->m_pTraceBridge)
			return;

		const RegionGraphData& rRgnGraphData = *m_rData.m_pBase->m_rRgn.m_pRegionGraphData;
		const NodeVertexMap& rNodeVertexMap = *rRgnGraphData.m_pBasicGraphData->m_pNodeVertexMap;

		do {
			// スタックを非同期操作できるようにする。
			const std::auto_ptr<const VertexStack> pStack(m_rData.m_pBase->m_pTraceBridge->release());

			if(pStack.get()) {
				// 経路探索開始点を追加する。
				for(VertexStack::const_iterator it(pStack->begin()), it_end(pStack->end()); it != it_end; ++it) {
					const Graph::vertex_descriptor* pv(rns::make_mapped_value_referer(rNodeVertexMap)(*it));
					if(! pv)
						throw std::runtime_error("cant up trace.");
					
					if(! is_regulated_vertex(*pv, *rRgnGraphData.m_pVertexBidirVQRegRefsMap, m_rData.m_pBase->m_bReverse)) {
						vqreg_graph::push_path(m_rData.m_pBase->m_Queue, m_rData.m_pBase->m_PathMap[*pv], GraphPath(*pv));
					}
				}
			}
		} while(m_rData.m_pBase->m_Queue.empty() && m_rData.m_pBase->m_pTraceBridge->wait());
		// 経路探索が下層より先に終わってしまったときは、新たな経路探索開始点が追加されるか、下層の経路探索が終わるまで待つ。
	}

private:
	VisitorData& m_rData;
};

/**
 * @brief 経路探索を行う。
 *
 * @param pVisDataBase [in] 経路探索ビジタ用基本情報。
 */
void check(const boost::shared_ptr<VisitorDataBase>& pVisDataBase)
{
	const Region& rRgn = pVisDataBase->m_rRgn;
	const RegionGraphData& rRgnGraphData = *rRgn.m_pRegionGraphData;
	const BasicGraphData& rBasicGraphData = *rRgnGraphData.m_pBasicGraphData;

	_ASSERT(pVisDataBase->m_it != pVisDataBase->m_itEnd);

	VisitorData aVisData(pVisDataBase);

	try {
		// 経路探索を行う。
		vqreg_graph::breadth_first_visit(
			boost::make_filtered_graph(
				*rBasicGraphData.m_pGraph,
				UpperLevelFilter(
					*rBasicGraphData.m_pGraph,
					pVisDataBase->m_it->m_pRegionGraphData->m_pBasicGraphData->m_pLinkEdgeMap.get()
				)
			),
			pVisDataBase->m_Queue,
			VQRegulationsPropertyMap(
				*rRgnGraphData.m_pVertexBidirVQRegRefsMap,
				pVisDataBase->m_bReverse ? &BidirectionalVQRegulation::reverse : &BidirectionalVQRegulation::fore
			),
			&pVisDataBase->m_PathMap[0], 
			Visitor(aVisData)
		);
	}
	catch(const Reachable&) {}

	// 上層の経路探索スレッドに、下層の経路探索が終了したことを伝える。
	if(aVisData.m_pUpTraceBridge) {
		aVisData.m_pUpTraceBridge->finish();
	}

	// 上層の経路探索が終了するのを待つ。
	if(aVisData.m_pUpThread) {
		aVisData.m_pUpThread->join();
	}
}

} // anonymous namespace

CheckUpperReachability::CheckUpperReachability(
	RegionPyramid::const_iterator first,
	RegionPyramid::const_iterator last,
	bool bReverse
) :
m_it(first),
m_itEnd(last),
m_bReverse(bReverse)
{
}

/**
 * @brief 最下層のリージョンにおいて特定の道路ノードに接続する道路リンクから、最上層の道路リンクに辿り着くことができるか。
 *
 * @param nNodeID [in] 道路ノードID。
 * @retval true 最上層の道路リンクに辿り付けた。あるいは、層が1つも存在しない。
 * @retval false 最上層の道路リンクに辿りつけなかった。
 */
bool CheckUpperReachability::checkByNodeID(long nNodeID)
{
	// 既に最上層に辿りついていたら終了。
	if(m_it == m_itEnd)
		return true;

	const Region& rRgn = *m_it;
	const RegionGraphData& rRgnGraphData = *rRgn.m_pRegionGraphData;
	const BasicGraphData& rBasicGraphData = *rRgnGraphData.m_pBasicGraphData;

	// 指定したノードが当該リージョンに含まれていなかったら、上位リージョンに到達できるはずはない。
	const Graph::vertex_descriptor* pu = rns::make_mapped_value_referer(*rBasicGraphData.m_pNodeVertexMap)(nNodeID);
	if(! pu)
		return false;

	using namespace arctl::field;
	using namespace sindy::schema::road_link;

	boost::shared_ptr<void> hReachable(::CreateEvent(NULL, TRUE, FALSE, NULL), ::CloseHandle);
	boost::shared_ptr<VisitorDataBase>
		pVisDataBase(new VisitorDataBase(rRgn, boost::next(m_it), m_itEnd, m_bReverse, hReachable.get()));

	if(! is_regulated_vertex(*pu, *rRgnGraphData.m_pVertexBidirVQRegRefsMap, m_bReverse)) {
		vqreg_graph::push_path(pVisDataBase->m_Queue, pVisDataBase->m_PathMap[*pu], GraphPath(*pu));
	}

	check(pVisDataBase);

	return ::WaitForSingleObject(hReachable.get(), 0) == WAIT_OBJECT_0;
}
