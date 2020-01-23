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

	std::wstring line; // 読込んだ行の格納先
	unsigned int lineNum = 0; // エラー文出力用に行数をカウント

	// 以下、エラー判定用
	unsigned int digit = 0; // 桁数が一致しているか判定用
	bool bSuccess = true;   // エラーなしで全行読込めたか否か

	while( std::getline(ifs, line) )
	{
		++lineNum;
		// 空の行は無視
		if( line.empty() ) continue;
		if( util::isNumber(uh::toTStr(line)) )
		{
			// 最初に読込まれた行は桁数を保持
			if(0==digit)
			{
				digit = line.size();
			}
			// 以降は桁数が同じか判定し、一つでも異なったらエラー
			else if( bSuccess && digit != line.size() )
			{
				tstringstream ss;
				ss << AheLoadString(IDS_DETECT_DIFF_DIGIT).GetString()
					<< _T(" : FILE[") << filepath << _T("]");
				RUNLOGGER.fatal(ss.str().c_str());
				bSuccess = false;
			}
			// 桁数が対応外なら、それはエラーとする
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
		else // 数値でないものが指定されていた場合、それはエラーとする
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
const uh::tstring kTrue(_T("TRUE"));   //!< 二値判定のtrueにあたる文字列
const uh::tstring kFalse(_T("FALSE")); //!< 二値判定のfalseにあたる文字列

/**
 * @brief タグがユニークか判定
 * @param keyOpt   [in] ユニーク判別に使用するフィールド値を取得した構造体
 * @param tag      [in] 処理中のタグ
 * @param loadList [in] メモリ上に読込まれているデータ
 * @retval true  タグはユニークである
 * @retval false タグがユニークではない、あるいは、
 * ユニーク判別に使用するフィールドの指定がない
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
 * @brief タグのユニークを判別に使用するフィールドが重複なく指定されているか確認
 * @param keyOpt   [in] ユニーク判別に使用するフィールドの値を取得した構造体
 * @param tag      [in] 処理中のタグ
 * @param fields   [in] 定義済みのフィールド群
 * @retval true  指定されている
 * @retval false 指定されていない
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
 * @brief 文字列をesriフィールドタイプに変換
 * @param str [in] 文字列
 * @return フィールドタイプ
 * @retval boost::none 変換に失敗
 */
boost::optional<esriFieldType> toFieldType(const uh::tstring& str)
{
	// ここの判定でしか使用しないので定数化はしないが、
	// TODO: ライブラリ等で定数化されたらそちらを使用する
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
 * @brief 文字列をBOOLに変換
 * @param str [in] 文字列
 * @return bool値
 * @retval boost::none 変換に失敗
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
 * @brief 文字列を数値に変換
 * @param str [in] 文字列
 * @return 数値
 * @retval boost::none 変換に失敗
 */
boost::optional<unsigned long> toULong(const uh::tstring& str)
{
	if( !std::all_of(str.cbegin(), str.cend(), _istdigit) )
		return boost::none;
	return std::stoul( str );
}

/**
 * @brief XMLのタイプ別必須属性チェック
 * @param type        [in] タイプ
 * @param optLength   [in] フィールド長
 * @param optNullable [in] NULL許可
 * @param optAlias    [in] エイリアス
 * @param optDefault  [in] デフォルト値
 * @retval true  必須属性が指定されている
 * @retval false 必須属性が指定されていない
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
	default: // 処理上ここにきてはいけない
		assert(false);
		return false;
		break;
	}
	return good;
}

/**
 * @brief CComVariant型をesriFieldTypeに合った型に変換する
 * @param vari [in,out] 変換するCComVariant型の変数
 * @param type [in] タイプ
 * @retval true  変換に成功
 * @retval false 変換に失敗
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
	default: // 処理上ここにきてはいけない
		assert(false);
		return false;
		break;
	}
	return SUCCEEDED(vari.ChangeType(dstType) );
}

/**
 * @brief デフォルト値の指定が正しいかチェック
 * @param type [in] タイプ
 * @param str  [in] デフォルト値を指定した文字列
 * @retval true  指定は誤っている
 * @retval false 指定は正しい
 */
inline bool checkDefaultValue( esriFieldType type, const uh::tstring& str)
{
	CComVariant checkCom(str.c_str());
	return changeVariantType(checkCom, type);
}

/**
 * @brief オプション属性を読込む
 * @param result   [out] 読込んだ結果の格納先
 * @param opt      [in]  属性の読込み対象
 * @param func     [in]  resultに変換するための関数
 * @return 読込みの成否
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
	const uh::tstring kRootName(_T("root.Create"));                 //!< ルートの階層のタグ定義名
	const uh::tstring kCreateTableNameDef(_T("CreateTable"));       //!< テーブルの階層のタグ定義名
	const uh::tstring kNameAttrDef(_T("<xmlattr>.name"));           //!< テーブル・カラム名称の属性名
	const uh::tstring kCreateColumnDef(_T("CreateColumn"));         //!< カラムの階層のタグ定義名
	const uh::tstring kLengthAttrDef(_T("<xmlattr>.length"));       //!< カラムのフィールド長の属性名
	const uh::tstring kNullableAttrDef(_T("<xmlattr>.isnullable")); //!< カラムのNULL許可の属性名
	const uh::tstring kTypeAttrDef(_T("<xmlattr>.type"));           //!< カラムのデータタイプの属性名
	const uh::tstring kAliasAttrDef(_T("<xmlattr>.alias"));         //!< カラムのエイリアスの属性名
	const uh::tstring kDefaultAttrDef(_T("<xmlattr>.default"));     //!< カラムのデフォルト値の属性名

	using namespace boost::property_tree;

	// utf-8なxmlを読み込む準備
	wptree pt;
	using namespace boost::property_tree;
    std::basic_ifstream<typename wptree::key_type::value_type> ifs(uh::toStr(filepath).c_str());
	ifs.imbue(std::locale(std::locale(), new std::codecvt_utf8_utf16<wchar_t>));

	// xml読み込み
	read_xml( ifs, pt );

	// エラーを一通りログに出力してから異常終了する
	bool good = true; // 読込み中にエラーがなかったか

	// <Create>
	for( auto& v : pt.get_child(kRootName) )
	{
		// <CreateTable>
		if( eq(v.first, kCreateTableNameDef) )
		{
			// テーブル名称を取得
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

			// CreateTable以下を取得
			for(const auto& v2 : v.second.get_child(_T("")))
			{
				// <CreateColumn>
				if( eq( v2.first, kCreateColumnDef ) )
				{
					// 属性はオプションとして取った後、必須の指定がなければエラーを吐く
					// 重複定義されているフィールドがないかチェック
					auto optColumnName = v2.second.get_optional<uh::tstring>(kNameAttrDef);
					if( !checkUniqueName( optColumnName, kCreateColumnDef, layerInfo.m_addFields) )
					{
						good = false;
						continue;
					}
					uh::tstring columnName = optColumnName.get();
					std::transform( columnName.begin(), columnName.end(), columnName.begin(), ::toupper ); // 大文字変換
					// 以降の必須属性はtypeの指定によって異なる
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

					// 以下は任意指定値。ただし、typeによっては必須もある
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

					// ここまでエラーがない(=最低限必要な属性が指定されている）なら、作成対象として登録
					IFieldEditPtr fieldEdit(CLSID_Field);
					fieldEdit->put_Name( CComBSTR(optColumnName.get().c_str()) );
					fieldEdit->put_Type( type.get() );
					// エイリアスは指定値をそのまま入れる
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

					// デフォルト値はデータ側に合わせてチェック
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
