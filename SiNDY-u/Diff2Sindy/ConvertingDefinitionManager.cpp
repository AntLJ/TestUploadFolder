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
#include "ConvertingDefinitionManager.h"
#include "Notifier.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <codecvt>
#include <memory>
#include <regex>
#include <iostream>

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

typedef std::basic_regex<TCHAR> tregex;
typedef std::match_results<const TCHAR *> tcmatch;
typedef std::match_results<uh::tstring::const_iterator> tsmatch;
typedef std::regex_token_iterator<uh::tstring::const_iterator> tregex_token_iterator;

namespace
{
	/**
	 * @brief ○○Conditionタグの子ノードから条件を読込む
	 * @param [in]  parentNode Conditionタグに対応するptreeのノード
	 * @param [out] condition  読込んだ条件の格納先
	 */
	inline void readConditionChild(const std::pair<std::wstring, basic_ptree<std::wstring, std::wstring>>& parentNode, Condition& condition)
	{
		for(const auto& child : parentNode.second.get_child(_T("")))
		{
			if( !condition.exps )
				condition.exps.reset( new ConditionExpressionContainer() );
			static_pointer_cast<ConditionExpressionContainer>(condition.exps)->vecExp.push_back( parse_condition(child) );
		}
	}

} // namespace

bool ConvertingDefinitionManager::parse(const uh::tstring& path)
{
	const uh::tstring kRootName(_T("root.Convert"));                       //!< ルートの階層のタグ定義名

	const uh::tstring kTargetAttributeDef(  _T("<xmlattr>.target"));
	const uh::tstring kNameAttributeDef(    _T("<xmlattr>.name"));
	const uh::tstring kValueAttributeDef(   _T("<xmlattr>.value"));
	const uh::tstring kCopyAllAttributeDef( _T("<xmlattr>.copyall"));
	const uh::tstring kBulkExeAttributeDef( _T("<xmlattr>.bulkexe"));

	const uh::tstring kLoopConditionDef(  _T("LoopCondition"));
	const uh::tstring kTargetConditionDef(_T("TargetCondition"));
	const uh::tstring kUpdateConditionDef(_T("UpdateCondition"));
	const uh::tstring kCreateConditionDef(_T("CreateCondition"));
	const uh::tstring kDeleteConditionDef(_T("DeleteCondition"));

	const uh::tstring kAdditionalUpdateDef(_T("AdditionalUpdate"));
	const uh::tstring kSiNDYHistoryDef(_T("SiNDYHistory"));
	const uh::tstring kConvertTableDef(_T("ConvertTable"));
	const uh::tstring kColumnDef(_T("Column"));
	const uh::tstring kTrue(_T("TRUE"));
	const uh::tstring kFalse(_T("FALSE"));

	const uh::tstring kSrc(_T("SRC"));
	const uh::tstring kDst(_T("DST"));

	// utf-8なxmlを読み込む準備
	wptree pt;
	using namespace boost::property_tree;
    std::basic_ifstream<typename wptree::key_type::value_type> ifs(uh::toStr(path).c_str());
	ifs.imbue(std::locale(std::locale(), new std::codecvt_utf8_utf16<wchar_t>));
	// xml読み込み
	read_xml( ifs, pt );
	for( auto& v : pt.get_child(kRootName) )
	{
		// LoopCondition
		if( eq( v.first, kLoopConditionDef ) )
		{
			boost::optional<uh::tstring> optLoopTarget = v.second.get_optional<uh::tstring>(kTargetAttributeDef);
			uh::tstring loopTarget;
			if( optLoopTarget )
				loopTarget = optLoopTarget.get();
			LoopType type = LoopType::LoopUnknown;
			if( loopTarget.empty() || // taregt指定なしの場合はSRCとみなす
				eq( kSrc, loopTarget ) )
				type = LoopType::LoopSRC;
			else if( eq( kDst, loopTarget ) )
				type = LoopType::LoopDST;

			if( LoopType::LoopUnknown == type )
			{
				std::wstringstream ss;
				ss << _T("未定義なループタイプ : ") << loopTarget;
				notifier::critical::versatile( ss.str() );
				return false; // 不明なループタイプが指定されている
			}
			m_listConditions.push_back( std::make_pair( type, Conditions() ) );
			Conditions& conds = m_listConditions.rbegin()->second;
			Condition& targetCondition = conds.first;
			std::list<SPCondition>& modifyCondition = conds.second;

			for(const auto& v2 : v.second.get_child(_T("")))
			{
				// TargetCondition
				if (v2.first == kTargetConditionDef)
				{
					// TargetCondition以下を取得
					readConditionChild(v2, targetCondition);
				}
				// UpdateCondition、CreateCondition、DeleteCondition
				else {
					SPCondition spcond;
					if( v2.first == kUpdateConditionDef )
					{
						spcond.reset( new Condition(Condition::Update) );
						readConditionChild( v2, *spcond );
					}
					else if( v2.first == kCreateConditionDef )
					{
						spcond.reset( new Condition(Condition::Create) );
						readConditionChild( v2, *spcond );
					}
					else if( v2.first == kDeleteConditionDef )
					{
						spcond.reset( new Condition(Condition::Delete) );
						readConditionChild( v2, *spcond );
					}
					if( spcond )
						modifyCondition.push_back(spcond);
				}
			}
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
						m_globalAdditionalUpdate.sindyHistory = true;
					else if( eq( kFalse, value ) )
						m_globalAdditionalUpdate.sindyHistory = false;
					else
						return false; // 無効な値
				}
			}
		}
		// 処理対象テーブル定義を取得
		else if( v.first == kConvertTableDef )
		{
			uh::tstring tableName = v.second.get<uh::tstring>(kNameAttributeDef);
			std::transform( tableName.begin(), tableName.end(), tableName.begin(), ::toupper ); // 大文字変換
			IndexList idxList;
			idxList.table = tableName;
			// 全属性コピーフラグ（デフォルトtrue）
			boost::optional<uh::tstring> copyall = v.second.get_optional<uh::tstring>(kCopyAllAttributeDef);
			idxList.copyall = ( copyall && eq(copyall.get(), kFalse) ) ? false : true; // デフォルトtrue
			// 全国一括で処理するフラグ（デフォルトfalse）
			boost::optional<uh::tstring> bulkexe = v.second.get_optional<uh::tstring>(kBulkExeAttributeDef);
			idxList.bulkexe = ( bulkexe && eq(bulkexe.get(), kTrue) ) ? true : false; // デフォルトfalse

			// カラム個別指定
			for(const auto& v2 : v.second.get_child(_T("")))
			{
				// Column
				if( eq( v2.first, kColumnDef ) )
				{
					if( !parse_column( v2, idxList ) )
						return false;
				}
				else if( !isIgnoreTag( v2.first ) )
				{
					std::wstringstream ss;
					ss << _T("未定義なタグ : ") << v2.first;
					notifier::critical::versatile( ss.str() );
					return false;
				}

			}
			m_listTarget.push_back(std::make_pair(tableName, idxList));
		}
		else if( !isIgnoreTag( v.first ) )
		{
			std::wstringstream ss;
			ss << _T("未定義なタグ : ") << v.first;
			notifier::critical::versatile( ss.str() );
			return false;
		}

	}

	return true;
}

bool hasDstCondition2( const SPConditionExpressions& exp )
{
	if( !exp )
		return false;

	if( exp->isContainer() )
	{
		for( const auto& exp2 : static_pointer_cast<ConditionExpressionContainer>(exp)->vecExp )
		{
			if( hasDstCondition2(exp2) )
				return true;
		}
	}
	else {
		const ConditionExpression& exp3 = *static_pointer_cast<ConditionExpression>(exp);
		return ( exp3.leftValue.type == ExpressionTypeDst || exp3.rightValue.type == ExpressionTypeDst );
	}
	return false;
}

// DST側の条件があるかどうか
bool ConvertingDefinitionManager::hasDstCondition() const
{
	for( const auto& condition : m_listConditions )
	{
		const auto& targetCondition = condition.second.first;
		if( hasDstCondition2( targetCondition.exps ) )
			return true;
		const auto& modifyCondition = condition.second.second;
		for( const auto& mcond : modifyCondition )
		{
			if( hasDstCondition2( mcond->exps ) )
				return true;
		}
	}
	return false;
}
