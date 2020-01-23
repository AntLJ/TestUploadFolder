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
#include <boost/weak_ptr.hpp>
#include <boost/variant.hpp>
#include <boost/thread.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <rns/algorithm.h>
#include "RegionManager.h"

namespace {

struct CheckExpired : boost::static_visitor<bool>
{
	template <typename T>
	bool operator()(const boost::shared_ptr<T>& p) const { return ! p; }

	template <typename T>
	bool operator()(const boost::weak_ptr<T>& p) const { return p.expired(); }
};

/**
 * @brief 特定のリージョンを読み込むクラス
 *
 * 一度読み込まれたリージョンは、クラスのインスタンスが破棄されるまでキャッシュとして残る。
 */
class RegionLoader
{
	typedef boost::weak_ptr<const Region> RegionWeakRef;
	typedef boost::variant<RegionRef, RegionWeakRef> Data;
public:
	RegionLoader(
		IFeatureClass* ipRoadLinkClass,
		IFeatureClass* ipRoadNodeClass,
		const SharingDataRef& pSharingData,
		const RegionPropertyRef& pRegionProp
	) :
	m_ipRoadLinkClass(ipRoadLinkClass),
	m_ipRoadNodeClass(ipRoadNodeClass),
	m_pSharingData(pSharingData),
	m_pRegionProp(pRegionProp)
	{
	}

	/**
	 * @brief リージョンを読み込む。
	 *
	 * 一度読み込んだリージョンは、クラスのインスタンスが破棄されるか、 unload() されて参照元が存在しなくなるまでキャッシュとして残る。
	 */
	RegionRef load()
	{
		using namespace boost;

		const mutex::scoped_lock lk(m_DataMutex);

		RegionRef pRegion;

		switch(m_Data.which()) {
		case 1:
			// unload() されたオブジェクト情報がまだ生き残っていたら再利用する。
			pRegion = get<RegionWeakRef>(m_Data).lock();
			if(pRegion) {
				m_Data = pRegion;
			}
			break;
		case 0:
			// まだ unload() されていない。
			pRegion = get<RegionRef>(m_Data);
			break;
		}

		if(! pRegion) {
			const RegionDataRef pRegData(create_region_data(m_ipRoadLinkClass, m_ipRoadNodeClass, *m_pRegionProp));
			const BasicGraphDataRef pBasicGraphData(create_basic_graph_data(*pRegData));
			const RegionGraphDataRef pRegGraphData(create_region_graph_data(pBasicGraphData, *pRegData, *m_pSharingData));

			shared_ptr<Region> pReg(new Region);
			pReg->m_pRegionData = pRegData;
			pReg->m_pRegionGraphData = pRegGraphData;

			m_Data = pRegion = pReg;
		}

		return pRegion;
	}

	/**
	 * @brief キャッシュしているリージョンを解放予定に入れる。
	 *
	 * 解放される前に load() が呼ばれたらキャッシュ状態に戻る。<br>
	 * キャッシュされていない場合は何もしない。
	 */
	void unload()
	{
		const boost::mutex::scoped_lock lk(m_DataMutex);

		if(m_Data.which() == 0) {
			m_Data = RegionWeakRef(get<RegionRef>(m_Data));
		}
	}

	/**
	 * @brief リージョンのキャッシュは解放されているか。
	 */
	bool expired()
	{
		const boost::mutex::scoped_lock lk(m_DataMutex);
		return boost::apply_visitor(CheckExpired(), m_Data);
	}

	RegionPropertyRef getRegionProperty() const
	{
		return m_pRegionProp;
	}
	
private:
	const IFeatureClassPtr m_ipRoadLinkClass;
	const IFeatureClassPtr m_ipRoadNodeClass;
	const SharingDataRef m_pSharingData;
	const RegionPropertyRef m_pRegionProp;

	Data m_Data;
	boost::mutex m_DataMutex;
};

struct LoaderIdentifier : public std::binary_function<const RegionLoader, const RegionPropertyRef, bool>
{
	bool operator()(const RegionLoader& rLoader, const RegionPropertyRef& pRegionProp) const
	{
		return rLoader.getRegionProperty() == pRegionProp;
	}
};

typedef boost::shared_ptr<RegionLoader> LoaderRef;
typedef std::list<LoaderRef> LoaderRefs;

/**
 * @brief キャッシュ解放済みで、どこからも参照されていない RegionLoader のインスタンスを解放する。
 */
void cleanup(LoaderRefs& rLoaderRefs)
{
	using namespace boost;

	for(LoaderRefs::iterator it(rLoaderRefs.begin()), it_next; it != rLoaderRefs.end(); it = it_next) {
		++(it_next = it);
		if((*it)->expired() && it->unique()) {
			rLoaderRefs.erase(it);
		}
	}
}

} // anonymous namespace

class RegionManager::Impl
{
public:
	Impl(IFeatureClass* ipRoadLinkClass, IFeatureClass* ipRoadNodeClass, SharingDataRef pSharingData) :
	m_ipRoadLinkClass(ipRoadLinkClass),
	m_ipRoadNodeClass(ipRoadNodeClass),
	m_pSharingData(pSharingData)
	{
	}

	LoaderRef getLoader(const RegionPropertyRef& pRegProp)
	{
		using namespace boost::lambda;

		const boost::mutex::scoped_lock lk(m_LoaderRefsMutex);

		LoaderRefs::const_iterator it(rns::mru_find_if(m_LoaderRefs, boost::lambda::bind(LoaderIdentifier(), *boost::lambda::_1, pRegProp)));
		if(it == m_LoaderRefs.end()) {
			const LoaderRef pLoader(new RegionLoader(m_ipRoadLinkClass, m_ipRoadNodeClass, m_pSharingData, pRegProp));
			it = m_LoaderRefs.insert(m_LoaderRefs.begin(), pLoader);
		}
		return *it;
	}

	bool releaseLRURegion()
	{
		const boost::mutex::scoped_lock lk(m_LoaderRefsMutex);

		for(LoaderRefs::reverse_iterator it(m_LoaderRefs.rbegin()), it_end(m_LoaderRefs.rend()); it != it_end; ++it) {
			RegionLoader& rLoader = **it;
			if(! rLoader.expired()) {
				rLoader.unload();
				cleanup(m_LoaderRefs);
				return true;
			}
		}

		return false;
	}
	
private:
	const IFeatureClassPtr m_ipRoadLinkClass;
	const IFeatureClassPtr m_ipRoadNodeClass;
	const SharingDataRef m_pSharingData;

	LoaderRefs m_LoaderRefs;
	boost::mutex m_LoaderRefsMutex;
};

RegionManager::RegionManager(IFeatureClass* ipRoadLinkClass, IFeatureClass* ipRoadNodeClass, SharingDataRef pSharingData) :
pimpl(new Impl(ipRoadLinkClass, ipRoadNodeClass, pSharingData))
{
}

RegionManager::~RegionManager()
{
}

/**
 * @brief リージョンを取得する。
 */ 
RegionRef RegionManager::getRegion(RegionPropertyRef pRegProp)
{
	return pimpl->getLoader(pRegProp)->load();
}

/**
 * @brief しばらく取り出されていないリージョンインスタンスを1つ解放する。
 *
 * 実際に解放されるのは、インスタンスの参照元が存在しなくなった瞬間。
 *
 * @retval true リージョンを1つ解放した。
 * @retval false リージョンが存在しない。
 */
bool RegionManager::releaseLRURegion()
{
	return pimpl->releaseLRURegion();
}
