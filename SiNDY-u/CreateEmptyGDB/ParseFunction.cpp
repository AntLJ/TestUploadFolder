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
bool meshListToList(const uh::tstring& filepath, std::vector<uh::tstring>& result)
{
	std::wifstream ifs( uh::toWStr(filepath) );
	if( !ifs )
	{
		tstringstream ss;
		ss << AheLoadString(IDS_CANT_OPEN_MESHLIST).GetString()
			<< _T(" : FILE[") << filepath << _T("]");
		RUNLOGGER.fatal(ss.str().c_str());
		return false;
	}
	result.clear();

	std::wstring line; // �Ǎ��񂾍s�̊i�[��
	unsigned int lineNum = 0; // �G���[���o�͗p�ɍs�����J�E���g

	// �ȉ��A�G���[����p
	unsigned int digit = 0; // ��������v���Ă��邩����p
	bool bSuccess = true;   // �G���[�Ȃ��őS�s�Ǎ��߂����ۂ�

	while( std::getline(ifs, line) )
	{
		++lineNum;
		// ��̍s�͖���
		if( line.empty() ) continue;
		if( util::isNumber(uh::toTStr(line)) )
		{
			// �ŏ��ɓǍ��܂ꂽ�s�͌�����ێ�
			if(0==digit)
			{
				digit = line.size();
			}
			// �ȍ~�͌��������������肵�A��ł��قȂ�����G���[
			else if( bSuccess && digit != line.size() )
			{
				tstringstream ss;
				ss << AheLoadString(IDS_DETECT_DIFF_DIGIT).GetString()
					<< _T(" : FILE[") << filepath << _T("]");
				RUNLOGGER.fatal(ss.str().c_str());
				bSuccess = false;
			}
			// �������Ή��O�Ȃ�A����̓G���[�Ƃ���
			switch(line.size())
			{
			case 8:
			case 10:
				break;
			default:
				{
					tstringstream ss;
					ss << AheLoadString(IDS_FAILED_MESHCODE_DIGIT).GetString()
						<< _T(" : LINE[") << lineNum << _T("]")
						<< _T(" : STR[") << uh::toTStr(line) << _T("]")
						;
					RUNLOGGER.fatal(ss.str().c_str());
					bSuccess = false;
				}
				break;
			}
			result.emplace_back( line.c_str() );
		}
		else // ���l�łȂ����̂��w�肳��Ă����ꍇ�A����̓G���[�Ƃ���
		{
			tstringstream ss;
			ss << AheLoadString(IDS_DETECT_NON_NUMBER_ON_MESHLIST).GetString()
				<< _T(" : LINE[") << lineNum << _T("]")
				<< _T(" : STR[") << uh::toTStr(line) << _T("]")
				;
			RUNLOGGER.fatal(ss.str().c_str());
			bSuccess = false;
		}
	}
	if( !bSuccess )
	{
		result.clear();
		return false;
	}

	std::sort(result.begin(), result.end());
	result.erase(unique(result.begin(), result.end()), result.end());
	return true;
}

bool isIgnoreTag( const uh::tstring& tag )
{
	return ( eq( tag, _T("<xmlattr>") ) || eq( tag, _T("<xmlcomment>") ) );
}

namespace
{
const uh::tstring kTrue(_T("TRUE"));   //!< ��l�����true�ɂ����镶����
const uh::tstring kFalse(_T("FALSE")); //!< ��l�����false�ɂ����镶����

/**
 * @brief �^�O�����j�[�N������
 * @param keyOpt   [in] ���j�[�N���ʂɎg�p����t�B�[���h�l���擾�����\����
 * @param tag      [in] �������̃^�O
 * @param loadList [in] ��������ɓǍ��܂�Ă���f�[�^
 * @retval true  �^�O�̓��j�[�N�ł���
 * @retval false �^�O�����j�[�N�ł͂Ȃ��A���邢�́A
 * ���j�[�N���ʂɎg�p����t�B�[���h�̎w�肪�Ȃ�
 */
bool checkUniqueName(
	const boost::optional<uh::tstring>& keyOpt
	, const uh::tstring& tag
	, const std::vector<create_info::CreateLayerInfo>& loadList
	)
{
	if( !keyOpt )
	{
		RUNLOGGER.fatalXML(IDS_ATTR_IS_NECESSARY, tag, _T(""), _T(""));
		return false;
	}
	auto itTable = std::find_if(loadList.cbegin(), loadList.cend(),
		[&keyOpt](const create_info::CreateLayerInfo& elem)
		{ return eq( elem.m_name, keyOpt.get() ); });
	if(loadList.cend()!=itTable)
	{
		RUNLOGGER.fatalXML(IDS_SAME_NAME_TAG, tag, _T("NAME"), keyOpt.get().c_str());
		return false;
	}
	return true;
}

/**
 * @brief �^�O�̃��j�[�N�𔻕ʂɎg�p����t�B�[���h���d���Ȃ��w�肳��Ă��邩�m�F
 * @param keyOpt   [in] ���j�[�N���ʂɎg�p����t�B�[���h�̒l���擾�����\����
 * @param tag      [in] �������̃^�O
 * @param fields   [in] ��`�ς݂̃t�B�[���h�Q
 * @retval true  �w�肳��Ă���
 * @retval false �w�肳��Ă��Ȃ�
 */
bool checkUniqueName(
	const boost::optional<uh::tstring>& keyOpt
	, const uh::tstring& tag
	, const IFieldsPtr& fields
	)
{
	if( !keyOpt )
	{
		RUNLOGGER.fatalXML(IDS_ATTR_IS_NECESSARY, tag, _T(""), _T(""));
		return false;
	}
	long fieldCount = 0;
	fields->get_FieldCount(&fieldCount);
	for(long index =0; index<fieldCount; ++index)
	{
		IFieldPtr field;
		fields->get_Field(index, &field);
		CComBSTR name;
		field->get_Name(&name);

		if( util::eq(keyOpt.get(), name.m_str) )
		{
			RUNLOGGER.fatalXML(IDS_SAME_NAME_TAG, tag, _T("NAME"), keyOpt.get().c_str());
			return false;
		}
	}
	return true;
}

/**
 * @brief �������esri�t�B�[���h�^�C�v�ɕϊ�
 * @param str [in] ������
 * @return �t�B�[���h�^�C�v
 * @retval boost::none �ϊ��Ɏ��s
 */
boost::optional<esriFieldType> toFieldType(const uh::tstring& str)
{
	// �����̔���ł����g�p���Ȃ��̂Œ萔���͂��Ȃ����A
	// TODO: ���C�u�������Œ萔�����ꂽ�炻������g�p����
	if( util::eq( str, _T("DATE") ) )
		return esriFieldTypeDate;
	if( util::eq( str, _T("INTEGER") )
		|| util::eq( str, _T("LONGINTEGER") ))
		return esriFieldTypeInteger;
	if( util::eq( str, _T("SHORTINTEGER") ))
		return esriFieldTypeSmallInteger;
	if( util::eq( str, _T("DOUBLE") )
		|| util::eq( str, _T("FLOAT")) )
		return esriFieldTypeDouble;
	if( util::eq( str, _T("STRING") )
		|| util::eq( str, _T("TEXT") ) )
		return esriFieldTypeString;
	return boost::none;
}

/**
 * @brief �������BOOL�ɕϊ�
 * @param str [in] ������
 * @return bool�l
 * @retval boost::none �ϊ��Ɏ��s
 */
boost::optional<bool> toBool(const uh::tstring& str)
{
	if( util::eq( str, kTrue ) )
		return true;
	if( util::eq( str, kFalse ) )
		return false;
	return boost::none;
}

/**
 * @brief ������𐔒l�ɕϊ�
 * @param str [in] ������
 * @return ���l
 * @retval boost::none �ϊ��Ɏ��s
 */
boost::optional<unsigned long> toULong(const uh::tstring& str)
{
	if( !std::all_of(str.cbegin(), str.cend(), _istdigit) )
		return boost::none;
	return std::stoul( str );
}

/**
 * @brief XML�̃^�C�v�ʕK�{�����`�F�b�N
 * @param type        [in] �^�C�v
 * @param optLength   [in] �t�B�[���h��
 * @param optNullable [in] NULL����
 * @param optAlias    [in] �G�C���A�X
 * @param optDefault  [in] �f�t�H���g�l
 * @retval true  �K�{�������w�肳��Ă���
 * @retval false �K�{�������w�肳��Ă��Ȃ�
 */
inline bool checkNecessaryAttrForType(
	esriFieldType type
	, const boost::optional<uh::tstring>& optLength
	, const boost::optional<uh::tstring>& optNullable
	, const boost::optional<uh::tstring>& optAlias
	, const boost::optional<uh::tstring>& optDefault
	)
{
	bool good = true;
	switch(type)
	{
	case esriFieldTypeSmallInteger:
	case esriFieldTypeInteger:
	case esriFieldTypeString:
		good &= (optLength!=boost::none);
		break;
	case esriFieldTypeDouble:
	case esriFieldTypeDate:
		break;
	default: // �����ケ���ɂ��Ă͂����Ȃ�
		assert(false);
		return false;
		break;
	}
	return good;
}

/**
 * @brief CComVariant�^��esriFieldType�ɍ������^�ɕϊ�����
 * @param vari [in,out] �ϊ�����CComVariant�^�̕ϐ�
 * @param type [in] �^�C�v
 * @retval true  �ϊ��ɐ���
 * @retval false �ϊ��Ɏ��s
 */
inline bool changeVariantType(CComVariant& vari, esriFieldType type)
{
	VARTYPE dstType = VT_EMPTY;
	switch(type)
	{
	case esriFieldTypeSmallInteger:
		dstType = VT_I2;
		break;
	case esriFieldTypeInteger:
		dstType = VT_I4;
		break;
	case esriFieldTypeString:
		dstType = VT_BSTR;
		break;
	case esriFieldTypeDouble:
		dstType = VT_R8;
		break;
	case esriFieldTypeDate:
		dstType = VT_DATE;
		break;
	default: // �����ケ���ɂ��Ă͂����Ȃ�
		assert(false);
		return false;
		break;
	}
	return SUCCEEDED(vari.ChangeType(dstType) );
}

/**
 * @brief �f�t�H���g�l�̎w�肪���������`�F�b�N
 * @param type [in] �^�C�v
 * @param str  [in] �f�t�H���g�l���w�肵��������
 * @retval true  �w��͌���Ă���
 * @retval false �w��͐�����
 */
inline bool checkDefaultValue( esriFieldType type, const uh::tstring& str)
{
	CComVariant checkCom(str.c_str());
	return changeVariantType(checkCom, type);
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

} // namespace

bool xmlToCreateDef(const uh::tstring& filepath, create_info::CreateInfo& result)
{
	const uh::tstring kRootName(_T("root.Create"));                 //!< ���[�g�̊K�w�̃^�O��`��
	const uh::tstring kCreateTableNameDef(_T("CreateTable"));       //!< �e�[�u���̊K�w�̃^�O��`��
	const uh::tstring kNameAttrDef(_T("<xmlattr>.name"));           //!< �e�[�u���E�J�������̂̑�����
	const uh::tstring kCreateColumnDef(_T("CreateColumn"));         //!< �J�����̊K�w�̃^�O��`��
	const uh::tstring kLengthAttrDef(_T("<xmlattr>.length"));       //!< �J�����̃t�B�[���h���̑�����
	const uh::tstring kNullableAttrDef(_T("<xmlattr>.isnullable")); //!< �J������NULL���̑�����
	const uh::tstring kTypeAttrDef(_T("<xmlattr>.type"));           //!< �J�����̃f�[�^�^�C�v�̑�����
	const uh::tstring kAliasAttrDef(_T("<xmlattr>.alias"));         //!< �J�����̃G�C���A�X�̑�����
	const uh::tstring kDefaultAttrDef(_T("<xmlattr>.default"));     //!< �J�����̃f�t�H���g�l�̑�����

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

	// <Create>
	for( auto& v : pt.get_child(kRootName) )
	{
		// <CreateTable>
		if( eq(v.first, kCreateTableNameDef) )
		{
			// �e�[�u�����̂��擾
			auto nameOpt = v.second.get_optional<uh::tstring>(kNameAttrDef);
			if( !checkUniqueName( nameOpt, kCreateTableNameDef, result.m_layerInfo) )
			{
				good = false;
				continue;
			}
			create_info::CreateLayerInfo layerInfo;
			layerInfo.m_name = nameOpt.get();
			layerInfo.m_addFields = IFieldsEditPtr(CLSID_Fields);
			layerInfo.m_aliasName = v.second.get_optional<uh::tstring>(kAliasAttrDef);

			// CreateTable�ȉ����擾
			for(const auto& v2 : v.second.get_child(_T("")))
			{
				// <CreateColumn>
				if( eq( v2.first, kCreateColumnDef ) )
				{
					// �����̓I�v�V�����Ƃ��Ď������A�K�{�̎w�肪�Ȃ���΃G���[��f��
					// �d����`����Ă���t�B�[���h���Ȃ����`�F�b�N
					auto optColumnName = v2.second.get_optional<uh::tstring>(kNameAttrDef);
					if( !checkUniqueName( optColumnName, kCreateColumnDef, layerInfo.m_addFields) )
					{
						good = false;
						continue;
					}
					uh::tstring columnName = optColumnName.get();
					std::transform( columnName.begin(), columnName.end(), columnName.begin(), ::toupper ); // �啶���ϊ�
					// �ȍ~�̕K�{������type�̎w��ɂ���ĈقȂ�
					auto typeAttr = v2.second.get_optional<uh::tstring>(kTypeAttrDef);
					if(!typeAttr)
					{
						RUNLOGGER.fatalXML(IDS_ATTR_IS_NECESSARY, kCreateColumnDef, _T("NAME"), columnName.c_str());
						good = false;
						continue;
					}
					auto type = toFieldType( typeAttr.get() );
					if( !type )
					{
						RUNLOGGER.fatalXML(IDS_DETECT_UNKOWN_TYPE, kCreateColumnDef, _T("NAME"), columnName.c_str());
						good = false;
						continue;
					}

					// �ȉ��͔C�ӎw��l�B�������Atype�ɂ���Ă͕K�{������
					auto optLength = v2.second.get_optional<uh::tstring>(kLengthAttrDef);
					auto optNullable = v2.second.get_optional<uh::tstring>(kNullableAttrDef);
					auto optColumnAlias = v2.second.get_optional<uh::tstring>(kAliasAttrDef);
					auto optDefault = v2.second.get_optional<uh::tstring>(kDefaultAttrDef);
					if( !checkNecessaryAttrForType(type.get(), optLength, optNullable, optColumnAlias, optDefault) )
					{
						RUNLOGGER.fatalXML(IDS_ATTR_IS_NECESSARY, kCreateColumnDef, _T("NAME"), columnName.c_str());
						good = false;
						continue;
					}

					// �����܂ŃG���[���Ȃ�(=�Œ���K�v�ȑ������w�肳��Ă���j�Ȃ�A�쐬�ΏۂƂ��ēo�^
					IFieldEditPtr fieldEdit(CLSID_Field);
					fieldEdit->put_Name( CComBSTR(optColumnName.get().c_str()) );
					fieldEdit->put_Type( type.get() );
					// �G�C���A�X�͎w��l�����̂܂ܓ����
					if(optColumnAlias)
						fieldEdit->put_AliasName( CComBSTR(optColumnAlias.get().c_str()) );
					{
						std::function< boost::optional<unsigned long> (const uh::tstring&) > func = toULong;
						unsigned long length = 0;
						if( !readOpt( length, optLength, func ) )
						{
							RUNLOGGER.fatalXML(IDS_DETECT_INVALIED_LENGTH, kCreateColumnDef, _T("length"), optLength.get().c_str());
							good = false;
						}
						fieldEdit->put_Length(static_cast<long>(length));
						fieldEdit->put_Precision(static_cast<long>(length));

					}
					{
						std::function< boost::optional<bool> (const uh::tstring&) > func = toBool;
						bool nullable = true;
						if( !readOpt( nullable, optNullable, func ) )
						{
							RUNLOGGER.fatalXML(IDS_DETECT_UNKOWN_OPT, kCreateColumnDef, _T("isnullable"), optNullable.get().c_str());
							good = false;
						}
						fieldEdit->put_IsNullable( nullable? VARIANT_TRUE:VARIANT_FALSE );
					}

					// �f�t�H���g�l�̓f�[�^���ɍ��킹�ă`�F�b�N
					if( optDefault )
					{
						if( !checkDefaultValue(type.get(), optDefault.get()) )
						{
							RUNLOGGER.fatalXML(IDS_DEFAULT_INVALID, kCreateColumnDef, _T("default"), optDefault.get());
							good = false;
						}
						else
						{
							CComVariant vari(optDefault.get().c_str());
							changeVariantType(vari, type.get());
							fieldEdit->put_DefaultValue(vari);
						}
					}

					layerInfo.m_addFields->AddField( fieldEdit );
				}
				else if( !isIgnoreTag(v2.first) )
				{
					RUNLOGGER.fatalXML(IDS_DETECT_UNKOWN_TAG, v2.first, _T(""), _T(""));
					good = false;
					continue;
				}
			}
			result.m_layerInfo.push_back(layerInfo);
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
