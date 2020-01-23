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

#include "stdafx.h"
#include "HeightInfoMgr.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <codecvt>

using namespace std;
using namespace boost::property_tree;

namespace boost {
	namespace property_tree
	{
		typedef string_path<uh::tstring, id_translator<uh::tstring> > tpath;
		typedef basic_ptree<uh::tstring, uh::tstring> tptree;
		typedef basic_ptree<uh::tstring, uh::tstring,
			detail::less_nocase<uh::tstring> >
			tiptree;
	}
}


// 高さ情報を読み込む
bool HeightInfoMgr::read(const uh::tstring& path)
{
	// 以下のような定義を読み込む
	// <?xml version="1.0" encoding="utf-8"?>
	// <root>
	//  <Feature type = "WALK_LINK">　・・・※level1
	//	  <HighAttribute>　		 ・・・※level2
	//		  <value>4</value>　	・・・※level3
	//		  <value>5</value>
	//	  </HighAttribute>
	//	  <LowAttribute>
	//		  <value>11</value>
	//		  <value>12</value>
	//	  </LowAttribute>
	//  </Feature>
	//  <Feature type = "ROAD_LINK">
	//	  <HighAttribute>
	//		  <value>1</value>
	//	  </HighAttribute>
	//	  <LowAttribute>
	//		  <value>5</value>
	//	  </LowAttribute>
	//  </Feature>
	// </root>

	const uh::tstring kRootName(_T("root"));		//!< ルートの階層のタグ定義名
	const uh::tstring kFeature(_T("Feature"));	  //!< Featureのタグ定義名
	const uh::tstring kType(_T("<xmlattr>.type"));		  //!< Featureのタイプを設定す要素
	const uh::tstring kHighAttribute(_T("HighAttribute"));  //!< 高い属性を設定するタグ
	const uh::tstring kLowAttribute(_T("LowAttribute"));	//!< 低い属性を設定するタグ
	const uh::tstring kMergeValueName(_T("value"));		 //!< 値タグの名称

	using namespace sindy::schema;

	// utf-8で定義されているxmlを読み込む準備
	wptree pt;
	using namespace boost::property_tree;
	std::basic_ifstream<typename wptree::key_type::value_type> ifs(uh::toStr(path).c_str());
	ifs.imbue(std::locale(std::locale(), new std::codecvt_utf8_utf16<wchar_t>));

	// xml読み込み
	read_xml(ifs, pt);
	for (auto& level1 : pt.get_child(_T("root")))
	{
		if (0 == _tcsicmp(level1.first.c_str(), kFeature.c_str()))
		{
			// 歩行者リンク、道路リンクかを判断
			uh::tstring type = level1.second.get<uh::tstring>(kType);
			std::set<long>* lowAttr(nullptr);
			std::set<long>* highAttr(nullptr);

			// 歩行者リンクの場合
			if (0 == _tcsicmp(type.c_str(), walk_link::kTableName))
			{
				// 設定する属性群をセット
				lowAttr = &m_walkLowAttr;
				highAttr = &m_walkHighAttr;
			}
			// 道路リンクの場合リンクの場合
			else if (0 == _tcsicmp(type.c_str(), road_link::kTableName))
			{
				// 設定する属性群をセット
				lowAttr = &m_roadLowAttr;
				highAttr = &m_roadHighAttr;
			}
			else
				// 歩行者リンク、道路リンク以外がtypeに設定されていたらエラー
				return false;

			for (const auto& level2 : level1.second.get_child(_T("")))
			{
				std::set<long>* attr(nullptr);
				// 高い属性の場合
				if (0 == _tcsicmp(level2.first.c_str(), kHighAttribute.c_str()))
					attr = highAttr;
				// 低い属性の場合
				else if (0 == _tcsicmp(level2.first.c_str(), kLowAttribute.c_str()))
					attr = lowAttr;
				else
					continue;

				// valueの値を取得
				for (const auto& level3 : level2.second.get_child(_T("")))
				{
					if (0 == _tcsicmp(level3.first.c_str(), kMergeValueName.c_str()))
						// 属性値をセット
						attr->insert(boost::lexical_cast<long>(level3.second.data()));
				}
			}
		}
	}

	return true;
}

namespace
{

	// 高さ情報を返す
	inline int getHeigthVal(long attr, const std::set<long>& lows, const std::set<long>& high)
	{
		if (lows.find(attr) != lows.end())
			return Height::low;
		if (high.find(attr) != high.end())
			return Height::high;
		return Height::ground;
	}

}

// 歩行者リンクの高さ情報を返す
int HeightInfoMgr::getWalkHeight(long attr) const
{
	return getHeigthVal(attr, m_walkLowAttr, m_walkHighAttr);
}

// 道路リンクの高さ情報を返す
int HeightInfoMgr::getRoadHeight(long attr) const
{
	return getHeigthVal(attr, m_roadLowAttr, m_roadHighAttr);
}
