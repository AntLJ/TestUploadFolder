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

// main.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <fstream>
#include <boost/program_options.hpp>
#include <atl2/io.h>
#include <arctl/coinitializer.h>
#include <sindy/workspace.h>
#include "SharingData.h"
#include "RegionPyramidCreator.h"
#include "CheckNodeReachability.h"

namespace po = boost::program_options;

namespace {

struct RoadData
{
	IFeatureClassPtr m_ipRoadLinkClass;
	IFeatureClassPtr m_ipRoadNodeClass;
	SharingDataRef m_pSharingData;
};

bool check_required_option(const po::variables_map& vm, LPCSTR lpszOption, LPCSTR lpszDesc)
{
	if(vm.count(lpszOption) == 0) {
		std::cout << lpszDesc << "が未設定です。" << std::endl;
		return false;
	}
	return true;
}

boost::shared_ptr<RegionPropertyManagerPyramid>
open_region_database(LPCTSTR lpszRegionWorkspace, MSXML2::IXMLDOMNodeList* ipRegLayerElementList)
{
	std::cerr << "リージョンデータベースを開く .";

	const IFeatureWorkspacePtr ipRegionWorkspace(sindy::create_workspace(lpszRegionWorkspace));

	boost::shared_ptr<RegionPropertyManagerPyramid> pRegionPropertyManagerPyramid(new RegionPropertyManagerPyramid);
	for(long i = 0, n = ipRegLayerElementList->length; i < n; i++) {
		std::cerr << '.';

		const MSXML2::IXMLDOMElementPtr
			ipRegLayerElement(ipRegLayerElementList->item[i]),
			ipUnitElement(ipRegLayerElement->selectSingleNode("unit")),
			ipOverlapElement(ipRegLayerElement->selectSingleNode("overlap")),
			ipWhereClauseElement(ipRegLayerElement->selectSingleNode("whereclause"));

		pRegionPropertyManagerPyramid->push_back(
			new RegionPropertyManager(
				ipUnitElement != 0 ? ipRegionWorkspace->_OpenFeatureClass(ipUnitElement->text) : 0,
				ipOverlapElement != 0 ? ipRegionWorkspace->_OpenFeatureClass(ipOverlapElement->text) : 0,
				ipWhereClauseElement->text
			)
		);
	}

	std::cerr << " OK" << std::endl;

	return pRegionPropertyManagerPyramid;
}

std::auto_ptr<RoadData> open_road_database(LPCTSTR lpszRoadWorkspace)
{
	using namespace sindy;

	std::cerr << "SiNDY道路データベースを開く .";

	const IFeatureWorkspacePtr ipFeatureWorkspace(create_workspace(lpszRoadWorkspace));

	IWorkspacePtr(ipFeatureWorkspace)->ExecuteSQL(CComBSTR(_T("ALTER SESSION SET CURSOR_SHARING = SIMILAR")));

	std::auto_ptr<RoadData> pRoadData(new RoadData);
	pRoadData->m_ipRoadLinkClass = ipFeatureWorkspace->_OpenFeatureClass(schema::road_link::kTableName);
	pRoadData->m_ipRoadNodeClass = ipFeatureWorkspace->_OpenFeatureClass(schema::road_node::kTableName);
	pRoadData->m_pSharingData = create_sharing_data(link_queue_table(pRoadData->m_ipRoadLinkClass, L"TURNREG"));

	std::cerr << " OK" << std::endl;

	return pRoadData;
}

MSXML2::IXMLDOMDocumentPtr loadConfigFile(const LPCTSTR lpszConfigFile)
{
  	const MSXML2::IXMLDOMDocumentPtr ipDoc(__uuidof(MSXML2::DOMDocument));
	ipDoc->async = VARIANT_FALSE;

	if(! ipDoc->load(lpszConfigFile))
		throw std::runtime_error(static_cast<LPCSTR>(ipDoc->parseError->reason));

	return ipDoc;
}

void process(const po::variables_map& vm)
{
	using namespace sindy;

	const MSXML2::IXMLDOMDocumentPtr ipDoc(loadConfigFile(vm["config-file"].as<std::wstring>().c_str()));

	// リージョンデータベースを開く。
	const boost::shared_ptr<RegionPropertyManagerPyramid>
		pRgnPropMgrPyramid(
			open_region_database(
				vm["region-database"].as<std::wstring>().c_str(),
				ipDoc->selectNodes("CheckReachability/regionlayers/regionlayer")
			)
		);

	// SiNYD道路データベースを開く。
	const std::auto_ptr<RoadData> pRoadData(open_road_database(vm["road-database"].as<std::wstring>().c_str()));

	// リージョンマネージャのインスタンスを生成する。
	const RegionManagerRef
		pRgnMng(new RegionManager(pRoadData->m_ipRoadLinkClass, pRoadData->m_ipRoadNodeClass, pRoadData->m_pSharingData));

	// リージョン階層クリエータのインスタンスを生成する。
	const RegionPyramidCreatorRef pRegPyramidCreator(new RegionPyramidCreator(pRgnMng, pRgnPropMgrPyramid));

	std::ifstream ifs(vm["node-list"].as<std::wstring>().c_str());

	CheckNodeReachability aCheckNodeReachability(pRoadData->m_ipRoadNodeClass, pRegPyramidCreator);

	for(long nNodeID; ifs >> nNodeID; ) {
		std::cerr << nNodeID << std::endl;
		std::pair<bool, bool> aPair(aCheckNodeReachability.check(nNodeID));
		std::cout
			<< nNodeID << '\t'
			<< (aPair.first ? "○" : "×") << '\t'
			<< (aPair.second ? "○" : "×") << std::endl;
	}
}

} // anonymous namespace

int _tmain(int argc, _TCHAR* argv[])
{
	try {
		const arctl::coinitializer aCoInitializer;

		po::options_description desc("Allowed options");
		desc.add_options()
			("road-database", po::wvalue<std::wstring>(), "SiNDY道路データベース")
			("region-database", po::wvalue<std::wstring>(), "リージョンデータベース")
			("node-list", po::wvalue<std::wstring>(), "道路ノードIDリスト")
			("config-file", po::wvalue<std::wstring>(), "設定ファイル");

		if(argc == 1) {
			std::cout << desc << std::endl;
			return 1;
		}

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		bool bAllow = true;

		bAllow &= check_required_option(vm, "road-database", "SiNDY道路データベース");
		bAllow &= check_required_option(vm, "region-database", "リージョンデータベース");
		bAllow &= check_required_option(vm, "node-list", "道路ノードIDリスト");
		bAllow &= check_required_option(vm, "config-file", "設定ファイル");

		if(! bAllow)
			return 1;

		process(vm);
	}
	catch(const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	catch(const _com_error& e) {
		std::cout << e << std::endl;
	}

	return 0;
}
