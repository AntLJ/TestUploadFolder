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
 * @brief タグのユニークを判別に使用するフィールドが重複なく指定されているか確認
 * @param keyOpt       [in] ユニーク判別に使用するフィールドの値を取得した構造体
 * @param tag          [in] 処理中のタグ
 * @param indexInfoMap [in] メモリ上に読込まれているデータ
 * @retval true  指定されている
 * @retval false 指定されていない
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
 * @brief 文字列をindex_def::TYPEに変換
 * @param str [in] 文字列
 * @return index_def::TYPE
 * @retval boost::none 変換に失敗
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
* @brief 文字列が昇順を表すか否かを表すbool値に変換
* @param str [in] 文字列
* @return bool値
* @retval boost::none 変換に失敗
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
 * @brief 文字列をboolに変換
 * @param str [in] 文字列
 * @return bool値
 * @retval boost::none 変換に失敗
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

}

bool xmlToIndexDef(const uh::tstring& filepath, index_def::INDEXINFOMAP& result)
{
	const uh::tstring kRootName(_T("root.Index"));              //!< ルートの階層のタグ定義名
	const uh::tstring kTableDef(_T("Table"));                   //!< レイヤの階層のタグ定義名
	const uh::tstring kNameAttrDef(_T("<xmlattr>.name"));       //!< レイヤ・カラム名称の属性名
	const uh::tstring kColumnDef(_T("Column"));                 //!< カラムの階層のタグ定義名
	const uh::tstring kTypeAttrDef(_T("<xmlattr>.type"));       //!< インデックスの操作タイプの属性名
	const uh::tstring kOrderAttrDef(_T("<xmlattr>.order"));     //!< インデックスの並び順の属性名
	const uh::tstring kUniqueAttrDef(_T("<xmlattr>.unique")); //!< インデックスのユニークの属性名

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

	// <Index>
	for( auto& v : pt.get_child(kRootName) )
	{
		// <Table>
		if( eq(v.first, kTableDef) )
		{
			// レイヤ名称を取得
			auto nameOpt = v.second.get_optional<uh::tstring>(kNameAttrDef);
			if( !checkUniqueName( nameOpt, kTableDef, result) )
			{
				good = false;
				continue;
			}
			auto& indexList = result.getIndexInfo(nameOpt.get());

			// Table以下を取得
			for(const auto& v2 : v.second.get_child(_T("")))
			{
				// <Column>
				if( eq( v2.first, kColumnDef ) )
				{
					// 属性はオプションとして取った後、必須の指定がなければエラーを吐く
					// 重複定義されているフィールドがないかチェック
					auto optColumnName = v2.second.get_optional<uh::tstring>(kNameAttrDef);
					if( !optColumnName )
					{
						RUNLOGGER.fatalXML(IDS_ATTR_IS_NECESSARY, kColumnDef, _T(""), _T(""));
						continue;
					}
					uh::tstring columnName = optColumnName.get();
					std::transform( columnName.begin(), columnName.end(), columnName.begin(), ::toupper ); // 大文字変換

					auto optType = v2.second.get_optional<uh::tstring>(kTypeAttrDef);
					if(!optType)
					{
						RUNLOGGER.fatalXML(IDS_ATTR_IS_NECESSARY, kColumnDef, _T("NAME"), columnName.c_str());
						good = false;
						continue;
					}

					index_def::IndexInfo indexDef;
					indexDef.m_name = columnName;
					// ここまでエラーがない(=最低限必要な属性が指定されている）なら、作成対象として登録
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
