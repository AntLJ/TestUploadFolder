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
#include "MergingDefinitionManager.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <codecvt>
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

extern bool eq( const uh::tstring& s1, const uh::tstring& s2 ); // ConvertingDefinitionManager.cpp�Œ�`

SPConditionExpressions parse_condition( const std::pair<std::wstring,boost::property_tree::wptree>& pt );

MergingDefinition::MergingDefinition()
{
}

// �R�s�[�R���X�g���N�^
MergingDefinition::MergingDefinition(const MergingDefinition &o) : 
	m_strTableName(o.m_strTableName),
	m_cCondition(o.m_cCondition),
	m_idxList(o.m_idxList)
{
}

MergingDefinition::~MergingDefinition()
{
}
// XML���
bool MergingDefinitionManager::parse(const uh::tstring& path)
{
	const uh::tstring kRootName(_T("root.Merge"));							//!< ���[�g�̊K�w�̃^�O��`��
	const uh::tstring kMegeTableNameDef(_T("MergeTable"));					//!< �}�[�W����e�[�u���̊K�w�̃^�O��`��
	const uh::tstring kMageTableNameAttributeDef(_T("<xmlattr>.name"));		//!< �}�[�W����e�[�u�����̂̑�����
	const uh::tstring kMergeAttributeFieldNameDef(_T("<xmlattr>.fieldName"));	//!< �}�[�W���邩���肷��t�B�[���h���̂̑�����
	const uh::tstring kValueAttributeDef(_T("<xmlattr>.value"));
	const uh::tstring kCopyAllAttributeDef(_T("<xmlattr>.copyall"));
	const uh::tstring kExpressionDef(_T("expression"));
	const uh::tstring kMergeConditionDef(_T("MergeCondition"));
	const uh::tstring kMergeValueName(_T("value"));							//!< �l�^�O�̖���
	const uh::tstring kTrue(_T("TRUE"));
	const uh::tstring kFalse(_T("FALSE"));
	const uh::tstring kColumnDef(_T("Column"));
	const uh::tstring kAdditionalUpdateDef(_T("AdditionalUpdate"));
	const uh::tstring kSiNDYHistoryDef(_T("SiNDYHistory"));
	const uh::tstring kConditionDef(_T("<xmlattr>.condition"));

	// utf-8��xml��ǂݍ��ޏ���
	wptree pt;
	using namespace boost::property_tree;
    std::basic_ifstream<typename wptree::key_type::value_type> ifs(uh::toStr(path).c_str());
	ifs.imbue(std::locale(std::locale(), new std::codecvt_utf8_utf16<wchar_t>));
	// xml�ǂݍ���
	read_xml( ifs, pt );
	for( auto& v : pt.get_child(kRootName) )
	{
		if ( eq( v.first, kMegeTableNameDef) )
		{
			// �}�[�W�e�[�u�����̂��擾
			MergingDefinition m;
			m.m_strTableName = v.second.get<uh::tstring>(kMageTableNameAttributeDef);

			// �S�����R�s�[�t���O�i�f�t�H���gtrue�j
			boost::optional<uh::tstring> copyall = v.second.get_optional<uh::tstring>(kCopyAllAttributeDef);
			m.m_idxList.copyall = ( copyall && eq(copyall.get(), kFalse) ) ? false : true; // �f�t�H���gtrue

			// MergeTable�ȉ����擾
			for(const auto& v2 : v.second.get_child(_T("")))
			{
				// MergeCondition
				if( eq( v2.first, kMergeConditionDef ) )
				{
					for(const auto& v3 : v2.second.get_child(_T("")))
					{
						m.m_cCondition.exps = parse_condition(v3);
					}
				}
				// Column
				else if( eq( v2.first, kColumnDef ) )
				{
					if( !parse_column( v2, m.m_idxList ) )
						return false;
				}
				else if( !isIgnoreTag(v2.first) ) {
					std::wstringstream ss;
					ss << _T("����`�̃^�O�ł� : ") << v2.first;
					notifier::critical::versatile( ss.str() );
				}
			}
			m_mergingDefinitions.push_back(m);
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
						return false; // �����Ȓl
				}
			}
		}
	}

	return true;
}

// �R�s�[�R���X�g���N�^
MergingDefinitionManager::MergingDefinitionManager(const MergingDefinitionManager &o) : m_setHistory(o.m_setHistory)
{
	m_mergingDefinitions = o.m_mergingDefinitions;
}
