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
#include "ParseFunction.h"
#include <codecvt>

using namespace util;

namespace parser
{

bool isIgnoreTag( const uh::tstring& tag )
{
	return ( eq( tag, _T("<xmlattr>") ) || eq( tag, _T("<xmlcomment>") ) );
}

namespace
{
/**
 * @brief �^�O�̃��j�[�N�𔻕ʂɎg�p����t�B�[���h���d���Ȃ��w�肳��Ă��邩�m�F
 * @param keyOpt       [in] ���j�[�N���ʂɎg�p����t�B�[���h�̒l���擾�����\����
 * @param tag          [in] �������̃^�O
 * @param indexInfoMap [in] ��������ɓǍ��܂�Ă���f�[�^
 * @retval true  �w�肳��Ă���
 * @retval false �w�肳��Ă��Ȃ�
 */
bool checkUniqueName(
	const boost::optional<uh::tstring>& keyOpt
	, const uh::tstring& tag
	, const index_def::INDEXINFOMAP& indexInfoMap
	)
{
	if( !keyOpt )
	{
		RUNLOGGER.fatalXML(IDS_ATTR_IS_NECESSARY, tag, _T(""), _T(""));
		return false;
	}

	if(indexInfoMap.isExist(keyOpt.get()))
	{
		RUNLOGGER.fatalXML(IDS_SAME_NAME_TAG, tag, _T("NAME"), keyOpt.get().c_str());
		return false;
	}
	return true;
}

/**
 * @brief �������index_def::TYPE�ɕϊ�
 * @param str [in] ������
 * @return index_def::TYPE
 * @retval boost::none �ϊ��Ɏ��s
 */
boost::optional<index_def::TYPE> toType(const uh::tstring& str)
{
	if(util::eq(str, _T("ADD")))
		return index_def::TYPE::ADD;
	if(util::eq(str, _T("DELETE")))
		return index_def::TYPE::DEL;
	return boost::none;
}

/**
* @brief �����񂪏�����\�����ۂ���\��bool�l�ɕϊ�
* @param str [in] ������
* @return bool�l
* @retval boost::none �ϊ��Ɏ��s
*/
boost::optional<bool> toOrderBool(const uh::tstring& str)
{
	if(util::eq(str, _T("ASC")))
		return true;
	if(util::eq(str, _T("DESC")))
		return false;
	return boost::none;
}

/**
 * @brief �������bool�ɕϊ�
 * @param str [in] ������
 * @return bool�l
 * @retval boost::none �ϊ��Ɏ��s
 */
boost::optional<bool> toBool(const uh::tstring& str)
{
	if( util::eq( str, _T("TRUE")) )
		return true;
	if( util::eq( str, _T("FALSE")) )
		return false;
	return boost::none;
}

/**
 * @brief �I�v�V����������Ǎ���
 * @param result   [out] �Ǎ��񂾌��ʂ̊i�[��
 * @param opt      [in]  �����̓Ǎ��ݑΏ�
 * @param func     [in]  result�ɕϊ����邽�߂̊֐�
 * @return �Ǎ��݂̐���
 */
template<typename _outType>
inline bool readOpt(
	_outType& result
	, const boost::optional<uh::tstring>& opt
	, std::function<boost::optional<_outType> (const uh::tstring&)>& func
	)
{
	if( !opt ) return true;
	auto cnvData = func( opt.get() );
	if( !cnvData ) return false;
	result = cnvData.get();
	return true;
}

}

bool xmlToIndexDef(const uh::tstring& filepath, index_def::INDEXINFOMAP& result)
{
	const uh::tstring kRootName(_T("root.Index"));              //!< ���[�g�̊K�w�̃^�O��`��
	const uh::tstring kTableDef(_T("Table"));                   //!< ���C���̊K�w�̃^�O��`��
	const uh::tstring kNameAttrDef(_T("<xmlattr>.name"));       //!< ���C���E�J�������̂̑�����
	const uh::tstring kColumnDef(_T("Column"));                 //!< �J�����̊K�w�̃^�O��`��
	const uh::tstring kTypeAttrDef(_T("<xmlattr>.type"));       //!< �C���f�b�N�X�̑���^�C�v�̑�����
	const uh::tstring kOrderAttrDef(_T("<xmlattr>.order"));     //!< �C���f�b�N�X�̕��я��̑�����
	const uh::tstring kUniqueAttrDef(_T("<xmlattr>.unique")); //!< �C���f�b�N�X�̃��j�[�N�̑�����

	using namespace boost::property_tree;

	// utf-8��xml��ǂݍ��ޏ���
	wptree pt;
	using namespace boost::property_tree;
    std::basic_ifstream<typename wptree::key_type::value_type> ifs(uh::toStr(filepath).c_str());
	ifs.imbue(std::locale(std::locale(), new std::codecvt_utf8_utf16<wchar_t>));

	// xml�ǂݍ���
	read_xml( ifs, pt );

	// �G���[����ʂ胍�O�ɏo�͂��Ă���ُ�I������
	bool good = true; // �Ǎ��ݒ��ɃG���[���Ȃ�������

	// <Index>
	for( auto& v : pt.get_child(kRootName) )
	{
		// <Table>
		if( eq(v.first, kTableDef) )
		{
			// ���C�����̂��擾
			auto nameOpt = v.second.get_optional<uh::tstring>(kNameAttrDef);
			if( !checkUniqueName( nameOpt, kTableDef, result) )
			{
				good = false;
				continue;
			}
			auto& indexList = result.getIndexInfo(nameOpt.get());

			// Table�ȉ����擾
			for(const auto& v2 : v.second.get_child(_T("")))
			{
				// <Column>
				if( eq( v2.first, kColumnDef ) )
				{
					// �����̓I�v�V�����Ƃ��Ď������A�K�{�̎w�肪�Ȃ���΃G���[��f��
					// �d����`����Ă���t�B�[���h���Ȃ����`�F�b�N
					auto optColumnName = v2.second.get_optional<uh::tstring>(kNameAttrDef);
					if( !optColumnName )
					{
						RUNLOGGER.fatalXML(IDS_ATTR_IS_NECESSARY, kColumnDef, _T(""), _T(""));
						continue;
					}
					uh::tstring columnName = optColumnName.get();
					std::transform( columnName.begin(), columnName.end(), columnName.begin(), ::toupper ); // �啶���ϊ�

					auto optType = v2.second.get_optional<uh::tstring>(kTypeAttrDef);
					if(!optType)
					{
						RUNLOGGER.fatalXML(IDS_ATTR_IS_NECESSARY, kColumnDef, _T("NAME"), columnName.c_str());
						good = false;
						continue;
					}

					index_def::IndexInfo indexDef;
					indexDef.m_name = columnName;
					// �����܂ŃG���[���Ȃ�(=�Œ���K�v�ȑ������w�肳��Ă���j�Ȃ�A�쐬�ΏۂƂ��ēo�^
					{
						std::function< boost::optional<index_def::TYPE> (const uh::tstring&) > func = toType;
						if( !readOpt(indexDef.m_type, optType, func ) )
						{
							RUNLOGGER.fatalXML(IDS_DETECT_UNKOWN_ATTR_VALUE, kColumnDef, _T("type"), optType.get().c_str());
							good = false;
						}
					}
					{
						std::function< boost::optional<bool> (const uh::tstring&) > func = toOrderBool;
						auto optOrder = v2.second.get_optional<uh::tstring>(kOrderAttrDef);
						if( !readOpt(indexDef.m_order_asc, optOrder, func ) )
						{
							RUNLOGGER.fatalXML(IDS_DETECT_UNKOWN_ATTR_VALUE, kColumnDef, _T("order"), optOrder.get().c_str());
							good = false;
						}
					}
					{
						std::function< boost::optional<bool> (const uh::tstring&) > func = toBool;
						auto optUnique = v2.second.get_optional<uh::tstring>(kUniqueAttrDef);
						if( !readOpt( indexDef.m_unique, optUnique, func ) )
						{
							RUNLOGGER.fatalXML(IDS_DETECT_UNKOWN_ATTR_VALUE, kColumnDef, _T("unique"), optUnique.get().c_str());
							good = false;
						}
					}
					indexList.push_back(indexDef);
				}
				else if( !isIgnoreTag(v2.first) )
				{
					RUNLOGGER.fatalXML(IDS_DETECT_UNKOWN_TAG, v2.first, _T(""), _T(""));
					good = false;
					continue;
				}
			}
		}
		else if( !isIgnoreTag(v.first) )
		{
			RUNLOGGER.fatalXML(IDS_DETECT_UNKOWN_TAG, v.first, _T(""), _T(""));
			good = false;
			continue;
		}

	}
	return good;
}

} // parser
