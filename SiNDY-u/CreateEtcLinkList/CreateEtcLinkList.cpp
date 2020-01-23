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

// CreateEtcLinkList.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <arctl/coinitializer.h>
#include <sindy/workspace.h>
#include <sindy/schema.h>
#include <sstream>
#include <rns/format.h>
#include "crd_cnv.h"
#include "EtcLinkList.h"

namespace po = boost::program_options;
int iRET; //返り値用グローバル変数

/**
 *	@brief	指定されたRoad_Linkオブジェクトが存在する２次メッシュコードを返す
 *	@note	リンク上の中点が存在するメッシュを、リンクの存在するメッシュと見なす
 *	@param	cpFeature	[in]	対象Road_Linkオブジェクト
 *	@return	対象Road_Linkオブジェクトが存在する２次メッシュコード
 **/
long getMeshCode( IFeature* cpFeature )
{
	// ２点を結ぶリンク上の中心点のメッシュコードを調べる
	IPolylinePtr ipPolyline(cpFeature->GetShapeCopy());
	IPointPtr ipPoint(CLSID_Point);
	ipPolyline->QueryPoint( esriNoExtension, 0.5, VARIANT_TRUE, ipPoint );
	
	double aMidPointLon = ipPoint->GetX();
	double aMidPointLat = ipPoint->GetY();

	crd_cnv aCrd_cnv;
	int aMidX, aMidY;	// 正規化座標
	int aMeshCode;
	aCrd_cnv.LLtoMesh( aMidPointLon, aMidPointLat, 2, &aMeshCode, &aMidX, &aMidY, 0);

	return aMeshCode;
}

/**
 * @brief	出力
 * @param	os	[in]	出力ストリーム
 * @param	crEtcLinkList	[in]	出力対象リスト
 **/
void write(std::ostream& os, EtcLinkList& crEtcLinkList)
{
	using namespace std;
	std::string fd = "\t";

	BOOST_FOREACH (EtcLink aEtcLink, crEtcLinkList)
	{
		os << aEtcLink.eType << fd;
		os << aEtcLink.eMeshCode << fd;

		BOOST_FOREACH (long aNodeID, aEtcLink.eNodeList) {
			os << aNodeID << fd;
		}

		os << aEtcLink.eFromNodeID << fd;
		os << aEtcLink.eToNodeID << endl;
	}
}

/**
 * @brief	ETCリンクリストの作成
 * @param	cpFeatureWorkspace	[in]	対象ワークスペース
 * @param	crEtcLinkList	[out]	ETCリンクリスト格納バッファへの参照
 * @note	etc_link.txtのフォーマットでは、「中点ノード列」を列挙することにより、
 *			連続したリンクを１つのレコードにまとめることができるが、本プログラムでは未対応である。
 *			中点ノードを格納するような実装を行っていないため、始点ノードと終点ノードのみのレコードしか作成されない。
 *			これは、リンク群の始終点を列挙した後、どのように連続性をチェックして中点ノード列を作成しレコードを減らすか、
 *			というアルゴリズムが決められなかったためである。
 *			フォーマット上は中点ノード列が全くないデータでも問題はないため、保留としてある。
 **/
void makeLinkList(IFeatureWorkspace* cpFeatureWorkspace, EtcLinkList& crEtcLinkList)
{
	using namespace sindy::schema;
#ifdef	_DEBUG
	std::cout << "Road_Link読み込み" << std::endl;
#endif

	IFeatureClassPtr ipFeatureClass = cpFeatureWorkspace->_OpenFeatureClass(road_link::kTableName);
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	
	ipQueryFilter->WhereClause =
		(rns::tformat(_T("%1% IN(%2%,%3%)"))
			% road_link::kUpDownClass 
			% road_link::updown_class::kEtcRamp 
			% road_link::updown_class::kEtcAccess
		).str().c_str();

	ipQueryFilter->SubFields =
		(rns::tformat(_T("%1%,%2%,%3%,%4%"))
			% road_link::kUpDownClass
			% road_link::kFromNodeID
			% road_link::kToNodeID
			% ipFeatureClass->ShapeFieldName
		).str().c_str();

#ifdef	_DEBUG
	_bstr_t aWhereClause = ipQueryFilter->WhereClause;
	_bstr_t aSubFields = ipQueryFilter->SubFields;
#endif

	IFeatureCursorPtr ipCursor = ipFeatureClass->_Search(ipQueryFilter, FALSE);

#ifdef	_DEBUG
	std::cout << "値の取得" << std::endl;
#endif

	IFeaturePtr ipFeature;
	while (ipCursor->NextFeature(&ipFeature) == S_OK)
	{
		EtcLink aEtcLink;

		long aUpDownClass		= ipFeature->GetValue( ipFeature->Fields->_FindField(road_link::kUpDownClass));
		aEtcLink.eFromNodeID	= ipFeature->GetValue( ipFeature->Fields->_FindField(road_link::kFromNodeID));
		aEtcLink.eToNodeID		= ipFeature->GetValue( ipFeature->Fields->_FindField(road_link::kToNodeID));
		aEtcLink.eMeshCode		= getMeshCode(ipFeature);

		switch (aUpDownClass)
		{
		case road_link::updown_class::kEtcRamp:
			aEtcLink.eType = EtcLink::type_ramp;
			break;
		case road_link::updown_class::kEtcAccess:
			aEtcLink.eType = EtcLink::type_access;
			break;
		default:
			ATLASSERT(0);
		}

		crEtcLinkList.insert(aEtcLink);
	}
}


/**
 * @param	vm	[in]	コマンドライン引数管理オブジェクト
 **/
void process(po::variables_map& vm)
{
	// 出力ファイルストリームを準備
	std::string aOutputFilePath = vm["output_dir"].as<std::string>() + "/" + "etc_link.txt";
	std::ofstream aFout(aOutputFilePath.c_str(), std::ios::binary);		// 改行コードLFで出力するため、バイナリモードで開く
	if (! aFout)
	{
		throw std::runtime_error("出力ファイルオープンエラー");
	}

	IFeatureWorkspacePtr pFeatureWorkspace = INamePtr(sindy::create_workspace_name(
		CA2T(vm["road_database"].as<std::string>().c_str())))->_Open();

	EtcLinkList aEtcLinkList;

	makeLinkList(pFeatureWorkspace, aEtcLinkList);

	write(aFout, aEtcLinkList);
}

/**
 * @brief	エントリポイント
 **/
int _tmain(int argc, _TCHAR* argv[])
{
	iRET = 0;
	bool aIsSuccess = true;

	try
	{
		const arctl::coinitializer aCoInitializer;	// Arc9.3.1対応時にtryスコープ内に移動

		po::options_description desc("How to use");
		desc.add_options()
			("road_database",	po::value<std::string>(), "DB指定文字列")
			("output_dir",		po::value<std::string>(), "出力ディレクトリ");

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if (vm.count("road_database") == 0 ||
			vm.count("output_dir") == 0)
		{
			std::cerr << desc << std::endl;
			iRET=1;
			return iRET;
		}

		process(vm);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		aIsSuccess = false;
		iRET=2;
	}
	catch (_com_error& e)
	{
		std::cerr << e << std::endl;
		aIsSuccess = false;
		iRET=2;
	}

	if (! aIsSuccess) 
	{ 
		std::cerr << "異常終了" << std::endl;
	}

	
	return iRET;
}

