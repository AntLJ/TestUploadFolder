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


// ��������ǂݍ���
bool HeightInfoMgr::read(const uh::tstring& path)
{
	// �ȉ��̂悤�Ȓ�`��ǂݍ���
	// <?xml version="1.0" encoding="utf-8"?>
	// <root>
	//  <Feature type = "WALK_LINK">�@�E�E�E��level1
	//	  <HighAttribute>�@		 �E�E�E��level2
	//		  <value>4</value>�@	�E�E�E��level3
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

	const uh::tstring kRootName(_T("root"));		//!< ���[�g�̊K�w�̃^�O��`��
	const uh::tstring kFeature(_T("Feature"));	  //!< Feature�̃^�O��`��
	const uh::tstring kType(_T("<xmlattr>.type"));		  //!< Feature�̃^�C�v��ݒ肷�v�f
	const uh::tstring kHighAttribute(_T("HighAttribute"));  //!< ����������ݒ肷��^�O
	const uh::tstring kLowAttribute(_T("LowAttribute"));	//!< �Ⴂ������ݒ肷��^�O
	const uh::tstring kMergeValueName(_T("value"));		 //!< �l�^�O�̖���

	using namespace sindy::schema;

	// utf-8�Œ�`����Ă���xml��ǂݍ��ޏ���
	wptree pt;
	using namespace boost::property_tree;
	std::basic_ifstream<typename wptree::key_type::value_type> ifs(uh::toStr(path).c_str());
	ifs.imbue(std::locale(std::locale(), new std::codecvt_utf8_utf16<wchar_t>));

	// xml�ǂݍ���
	read_xml(ifs, pt);
	for (auto& level1 : pt.get_child(_T("root")))
	{
		if (0 == _tcsicmp(level1.first.c_str(), kFeature.c_str()))
		{
			// ���s�҃����N�A���H�����N���𔻒f
			uh::tstring type = level1.second.get<uh::tstring>(kType);
			std::set<long>* lowAttr(nullptr);
			std::set<long>* highAttr(nullptr);

			// ���s�҃����N�̏ꍇ
			if (0 == _tcsicmp(type.c_str(), walk_link::kTableName))
			{
				// �ݒ肷�鑮���Q���Z�b�g
				lowAttr = &m_walkLowAttr;
				highAttr = &m_walkHighAttr;
			}
			// ���H�����N�̏ꍇ�����N�̏ꍇ
			else if (0 == _tcsicmp(type.c_str(), road_link::kTableName))
			{
				// �ݒ肷�鑮���Q���Z�b�g
				lowAttr = &m_roadLowAttr;
				highAttr = &m_roadHighAttr;
			}
			else
				// ���s�҃����N�A���H�����N�ȊO��type�ɐݒ肳��Ă�����G���[
				return false;

			for (const auto& level2 : level1.second.get_child(_T("")))
			{
				std::set<long>* attr(nullptr);
				// ���������̏ꍇ
				if (0 == _tcsicmp(level2.first.c_str(), kHighAttribute.c_str()))
					attr = highAttr;
				// �Ⴂ�����̏ꍇ
				else if (0 == _tcsicmp(level2.first.c_str(), kLowAttribute.c_str()))
					attr = lowAttr;
				else
					continue;

				// value�̒l���擾
				for (const auto& level3 : level2.second.get_child(_T("")))
				{
					if (0 == _tcsicmp(level3.first.c_str(), kMergeValueName.c_str()))
						// �����l���Z�b�g
						attr->insert(boost::lexical_cast<long>(level3.second.data()));
				}
			}
		}
	}

	return true;
}

namespace
{

	// ��������Ԃ�
	inline int getHeigthVal(long attr, const std::set<long>& lows, const std::set<long>& high)
	{
		if (lows.find(attr) != lows.end())
			return Height::low;
		if (high.find(attr) != high.end())
			return Height::high;
		return Height::ground;
	}

}

// ���s�҃����N�̍�������Ԃ�
int HeightInfoMgr::getWalkHeight(long attr) const
{
	return getHeigthVal(attr, m_walkLowAttr, m_walkHighAttr);
}

// ���H�����N�̍�������Ԃ�
int HeightInfoMgr::getRoadHeight(long attr) const
{
	return getHeigthVal(attr, m_roadLowAttr, m_roadHighAttr);
}
