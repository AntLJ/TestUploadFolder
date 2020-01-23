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
 * @brief �ŏ�w�̃����N�ɓ��B�������Ƃ�����<strong>����n</strong>��O
 *
 * ����n�̗�O�͍D���ł͂Ȃ����ABGL���̌o�H�T���A���S���Y���𒆒f������@�͂��ꂭ�炢���������΂Ȃ��B
 */
struct Reachable {};

/**
 * @brief �w�肵�����_�͂P�_�̒ʍs�K���Ɉ��������邩�B
 *
 * @param v [in] ���_�B
 * @param rVertexBidirVQRegRefsMap [in] ���_�Ɋ֘A�t���Ă��钸�_��K���ꗗ�B
 * @param bReverse [in] �ʍs�K�����t�������猩�邩�B
 * @retval true �P�_�̒ʍs�K���Ɉ���������B
 * @retval false �P�_�̒ʍs�K���ɂ͈���������Ȃ��B
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
 * @brief �w�肵�����_���痬�o�\�ȕӂ͑��݂��邩�B
 *
 * @param rGraph [in] �O���t�B
 * @param u [in] ���_�B
 * @param rVertexBidirVQRegRefsMap [in] ���_�Ɋ֘A�t���Ă��钸�_��K���ꗗ�B
 * @param bReverse [in] �ʍs�K�����t�������猩�邩�B
 *
 * @note
 * ���߂�̂́u���o�\�ȕӁv�̗L���ł���A�u���o�\�Ȓ��_�v�ł͂Ȃ��B<br>
 * �O�҂��^�Ō�҂��U�ƂȂ�󋵂͗B��A�u���o�撸�_�ɂP�_�K���������Ă���v�ꍇ�̂݁B
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

typedef vqreg_graph::path<Graph::vertex_descriptor> GraphPath; ///< ��s���_
typedef std::list<GraphPath, fast_pool_allocator<GraphPath> > GraphPaths; ///< ��s���_��

typedef std::vector<GraphPaths, pool_allocator<GraphPaths> > GraphPathsMap; ///< ��s���_��v���p�e�B�}�b�v
typedef std::queue<const GraphPath*, std::deque<const GraphPath*, pool_allocator<const GraphPath*> > > Queue; ///< ��s���_��|�C���^�L���[

typedef std::vector<long, pool_allocator<long> > VertexStack;

/**
 * @brief ��w�̌o�H�T���L���[�ɒǉ�����o�H�T���J�n�_�X�^�b�N
 *
 * @note
 * ���̃N���X�̃����o�֐��͑S�ăX���b�h�Z�[�t�ł���B
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
	 * @brief �V�����o�H�T���J�n�_���ǉ������܂ő҂B
	 *
	 * @retval true �V�����o�H�T���J�n�_���ǉ����ꂽ�B
	 * @retval false ���w�̌o�H�T���͏I�������B�V�����o�H�T���J�n�_���ǉ�����邱�Ƃ͂Ȃ��B
	 * @post �o�H�T���J�n�_�ۗL�C�x���g�I�u�W�F�N�g����V�O�i����ԂɂȂ�B
	 */
	bool wait()
	{
		HANDLE hs[2] = { m_hHas.get(), m_hEnd.get() };
		return ::WaitForMultipleObjects(2, hs, FALSE, INFINITE) == WAIT_OBJECT_0;
	}

	/**
	 * @brief �V�����o�H�T���J�n�_��ǉ�����B
	 *
	 * @post �o�H�T���J�n�_�ۗL�C�x���g�I�u�W�F�N�g����V�O�i����ԂɂȂ�B
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
	 * @brief �V�����o�H�T���_�̈ꗗ�����o���B
	 * 
	 * @return �V�����o�H�T���_�̈ꗗ�B
	 */
	std::auto_ptr<VertexStack> release()
	{
		const boost::mutex::scoped_lock aLock(m_Mutex);
		::ResetEvent(m_hHas.get());
		return m_pStack;
	}

	/**
	 * @brief ���w�̌o�H�T�����I���������Ƃ�`����B
	 */
	void finish()
	{
		::SetEvent(m_hEnd.get());
	}

private:
	std::auto_ptr<VertexStack> m_pStack; ///< �o�H�T���J�n�_�ꗗ�B
	boost::shared_ptr<void> m_hEnd; ///< ���w�̌o�H�T�����I���������ɃV�O�i����Ԃɂ��Ă��炤�C�x���g�I�u�W�F�N�g�̃n���h���B
	boost::shared_ptr<void> m_hHas; ///< ���w�����w�ցA�V���Ȍo�H���������ꂽ���ɃV�O�i����Ԃɂ���C�x���g�I�u�W�F�N�g�̃n���h���B
	boost::mutex m_Mutex; ///< m_pStack �p�~���[�e�N�X�B
};

/**
 * @brief �o�H�T���p�r�W�^�p���E��{
 *
 * �o�H�T���A���S���Y���J�n�O�ɗ\�ߐݒ肵�Ă��������܂Ƃ߂��\���́B
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
	
	const Region& m_rRgn; ///< ���݂̃��[�W�����B
	const RegionPyramid::const_iterator m_it, m_itEnd; ///< ����ʂ̃��[�W�����ꗗ�B
	const bool m_bReverse; ///< �����K���t�]���[�h�B
	const HANDLE m_hReachableEvent; ///< �ŏ�w�ɓ��B�ł����Ƃ��ɃV�O�i����ԂƂȂ�C�x���g�I�u�W�F�N�g�̃n���h���B

	Queue m_Queue; ///< ��s���_��|�C���^�L���[�B
	GraphPathsMap m_PathMap; ///< ��s���_���z��B

	boost::shared_ptr<TraceBridge> m_pTraceBridge; ///< ���w���猻�݂̑w�֒H��t�����o�H���o�^�����o�H�T���J�n�_�X�^�b�N�B�ŉ��w�ł�NULL�B
};

/**
 * @brief �o�H�T���p�r�W�^�p���E�g��
 *
 * ��w�̌o�H�T�����J�n�����V�����X���b�h�Ƃ̂��Ƃ���s�����߂̍\���́B
 */
struct VisitorData
{
	VisitorData(const boost::shared_ptr<VisitorDataBase>& pBase) :
	m_pBase(pBase)
	{
	}

	const boost::shared_ptr<VisitorDataBase> m_pBase; ///< ��{���B

	boost::shared_ptr<TraceBridge> m_pUpTraceBridge; ///< ���݂̑w�����w�֒H��t���o�H��o�^����o�H�T���J�n�_�X�^�b�N�B��w�̌o�H�T�����J�n�����܂�NULL�B
	boost::shared_ptr<boost::thread> m_pUpThread; ///< ��w�̌o�H�T�����s���X���b�h�B��w�̌o�H�T�����J�n�����܂�NULL�B
};

void check(const boost::shared_ptr<VisitorDataBase>& pVisDataBase);

/**
 * @brief �o�H�T���p�r�W�^
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
		// �ǂ����̃X���b�h���ŏ�w�܂ŒH��t������A�o�H�T���͏I������B
		if(::WaitForSingleObject(m_rData.m_pBase->m_hReachableEvent, 0) == WAIT_OBJECT_0)
			throw Reachable();

		using namespace boost;
		using namespace sindy::schema::category::directional_link_relational_table::link_dir;

		const Region& rUpRgn = *m_rData.m_pBase->m_it;
		const RegionGraphData& rUpRgnGraphData = *rUpRgn.m_pRegionGraphData;
		const BasicGraphData& rUpBasicGraphData = *rUpRgnGraphData.m_pBasicGraphData;
		const NodeVertexMap& rUpNodeVertexMap = *rUpBasicGraphData.m_pNodeVertexMap;

		const long& nNodeID = get(vertex_index1, rGraph, v);

		// ��w�̃O���t�ɑ��݂��铹�H�m�[�h���B
		const Graph::vertex_descriptor* ps(rns::make_mapped_value_referer(rUpNodeVertexMap)(nNodeID));
		if(ps) {
			if(m_rData.m_pUpTraceBridge) {
				// ���ɏ�w�̌o�H�T�����J�n����Ă�����A�o�H�T���J�n�_����w�p�̃X�^�b�N�ɒǉ��B
				m_rData.m_pUpTraceBridge->add(nNodeID);
			}
			else {
				const RegionPyramid::const_iterator
					it(boost::next(m_rData.m_pBase->m_it)), it_end(m_rData.m_pBase->m_itEnd);

				if(it == it_end) {
					// 2��̑w�����݂����A1��̓��H�����N�ɓ��B�ł����Ȃ�A�����Ōo�H�T���I���B
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
						// ��w�o�H�T���J�n�_�X�^�b�N���쐬���A�r�W�^�p�g�����ɓo�^�B
						pUpDataBase->m_pTraceBridge.reset(new TraceBridge);
						m_rData.m_pUpTraceBridge = pUpDataBase->m_pTraceBridge;

						// ��w�o�H�T���X���b�h�𗧂��グ�A�r�W�^�p�g�����ɓo�^�B
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
			// �X�^�b�N��񓯊�����ł���悤�ɂ���B
			const std::auto_ptr<const VertexStack> pStack(m_rData.m_pBase->m_pTraceBridge->release());

			if(pStack.get()) {
				// �o�H�T���J�n�_��ǉ�����B
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
		// �o�H�T�������w����ɏI����Ă��܂����Ƃ��́A�V���Ȍo�H�T���J�n�_���ǉ�����邩�A���w�̌o�H�T�����I���܂ő҂B
	}

private:
	VisitorData& m_rData;
};

/**
 * @brief �o�H�T�����s���B
 *
 * @param pVisDataBase [in] �o�H�T���r�W�^�p��{���B
 */
void check(const boost::shared_ptr<VisitorDataBase>& pVisDataBase)
{
	const Region& rRgn = pVisDataBase->m_rRgn;
	const RegionGraphData& rRgnGraphData = *rRgn.m_pRegionGraphData;
	const BasicGraphData& rBasicGraphData = *rRgnGraphData.m_pBasicGraphData;

	_ASSERT(pVisDataBase->m_it != pVisDataBase->m_itEnd);

	VisitorData aVisData(pVisDataBase);

	try {
		// �o�H�T�����s���B
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

	// ��w�̌o�H�T���X���b�h�ɁA���w�̌o�H�T�����I���������Ƃ�`����B
	if(aVisData.m_pUpTraceBridge) {
		aVisData.m_pUpTraceBridge->finish();
	}

	// ��w�̌o�H�T�����I������̂�҂B
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
 * @brief �ŉ��w�̃��[�W�����ɂ����ē���̓��H�m�[�h�ɐڑ����铹�H�����N����A�ŏ�w�̓��H�����N�ɒH�蒅�����Ƃ��ł��邩�B
 *
 * @param nNodeID [in] ���H�m�[�hID�B
 * @retval true �ŏ�w�̓��H�����N�ɒH��t�����B���邢�́A�w��1�����݂��Ȃ��B
 * @retval false �ŏ�w�̓��H�����N�ɒH����Ȃ������B
 */
bool CheckUpperReachability::checkByNodeID(long nNodeID)
{
	// ���ɍŏ�w�ɒH����Ă�����I���B
	if(m_it == m_itEnd)
		return true;

	const Region& rRgn = *m_it;
	const RegionGraphData& rRgnGraphData = *rRgn.m_pRegionGraphData;
	const BasicGraphData& rBasicGraphData = *rRgnGraphData.m_pBasicGraphData;

	// �w�肵���m�[�h�����Y���[�W�����Ɋ܂܂�Ă��Ȃ�������A��ʃ��[�W�����ɓ��B�ł���͂��͂Ȃ��B
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
