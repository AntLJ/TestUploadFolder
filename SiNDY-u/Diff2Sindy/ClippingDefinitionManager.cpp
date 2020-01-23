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
#include "ClippingDefinitionManager.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <codecvt>
#include "Conditions.h"
#include "Notifier.h"

using namespace std;
using namespace boost::property_tree;

namespace boost { namespace property_tree
{
    typedef string_path<uh::tstring, id_translator<uh::tstring> > tpath;
    typedef basic_ptree<uh::tstring, uh::tstring> tptree;
    typedef basic_ptree<uh::tstring, uh::tstring,
                        detail::less_nocase<uh::tstring> >
        tiptree;
} }

extern bool eq( const uh::tstring& s1, const uh::tstring& s2 ); // ConvertingDefinitionManager.cppで定義

// XML解析
bool ClippingDefinitionManager::parse(const uh::tstring& path)
{
	const uh::tstring kRootName(_T("root.Clip"));							//!< ルートの階層のタグ定義名
	const uh::tstring kClipTableNameDef(_T("ClipTable"));					//!< クリップするテーブルの階層のタグ定義名
	const uh::tstring kNameAttributeDef(_T("<xmlattr>.name"));		//!< クリップするテーブル名称の属性名
	const uh::tstring kHistoryAttributeNameDef(_T("<xmlattr>.sindyhistory"));	//!< クリップするか判定するフィールド名称の属性名
	const uh::tstring kTypeAttributeDef(_T("root.Clip.<xmlattr>.type"));
	const uh::tstring kAdditionalUpdateDef(_T("AdditionalUpdate"));
	const uh::tstring kSiNDYHistoryDef(_T("SiNDYHistory"));
	const uh::tstring kValueAttributeDef(_T("<xmlattr>.value"));
	const uh::tstring kTrue(_T("TRUE"));
	const uh::tstring kFalse(_T("FALSE"));
	const uh::tstring kLot(_T("LOT"));
	const uh::tstring kMesh(_T("MESH"));

	// utf-8なxmlを読み込む準備
	wptree pt;
	using namespace boost::property_tree;
    std::basic_ifstream<typename wptree::key_type::value_type> ifs(uh::toStr(path).c_str());
	ifs.imbue(std::locale(std::locale(), new std::codecvt_utf8_utf16<wchar_t>));

	// xml読み込み
	read_xml( ifs, pt );

	// タイプ
	auto clipType = pt.get_optional<uh::tstring>(kTypeAttributeDef);
	if( clipType )
	{
		if( 0 == _tcsicmp( clipType.get().c_str(), kLot.c_str() ) )
			m_emClipType = clipTypeLot;
		else if( 0 == _tcsicmp( clipType.get().c_str(), kMesh.c_str() ) )
			m_emClipType = clipTypeMesh;
		else
			m_emClipType = clipTypeMesh;
	}

	for( auto& v : pt.get_child(kRootName) )
	{
		if (v.first == kClipTableNameDef)
		{
			// クリップテーブル名称を取得
			ClippingDefinition m;
			m = v.second.get<uh::tstring>(kNameAttributeDef);
			m_ClippingDefinitions.push_back(m);
		}
		// AdditionalUpdate
		else if( v.first == kAdditionalUpdateDef )
		{
			for(const auto& v2 : v.second.get_child(_T("")))
			{
				if( eq( v2.first, kSiNDYHistoryDef ) )
				{
					uh::tstring value = v2.second.get<uh::tstring>( kValueAttributeDef );
					if( eq( kTrue, value ) )
						m_setHistory = true;
					else if( eq( kFalse, value ) )
						m_setHistory = false;
					else
						return false; // 無効な値
				}
			}
		}
		else if( !isIgnoreTag(v.first) ) {
			std::wstringstream ss;
			ss << _T("未定義のタグです : ") << v.first;
			notifier::critical::versatile( ss.str() );
		}

	}

	return true;
}

// コピーコンストラクタ
ClippingDefinitionManager::ClippingDefinitionManager(const ClippingDefinitionManager &o) : 
	m_ClippingDefinitions(o.m_ClippingDefinitions), 
	m_setHistory(o.m_setHistory), 
	m_emClipType(o.m_emClipType)
{
}
