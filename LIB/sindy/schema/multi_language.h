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

#pragma once
#include <sindy/schema/category.h>

namespace sindy
{
namespace schema
{
/// 多言語DB
namespace multi_language
{
	using namespace category;

	/// 多言語関連テーブルの共通基底オブジェクトクラス
	namespace ipc_mlang_table
	{
		extern const _TCHAR kDeleteF[];    //!< 削除フラグ
		extern const _TCHAR kLangCode[];   //!< 言語コード

		/// SiNDY-k 言語コード
		namespace lang_code
		{
			enum ECode
			{
				romaji = 1,                 //!< ローマ字表記
				english = 2,                //!< 英語
				simplified_chinese = 3,     //!< 中国語簡体字
				traditional_chinese = 4,    //!< 中国語繁体字
				korean = 5,                 //!< 韓国語
				thai = 6,                   //!< タイ語
				indonesia = 7,              //!< インドネシア語
			};
		}
	}

	/// 部分一致名称を含むテーブル
	namespace partial_name_table
	{
		extern const _TCHAR kSortOrder[];   //!< 並び順
		extern const _TCHAR kPartialName[]; //!< 分割名称
	}

	/// 多言語注記
	namespace annotation
	{
		using namespace ipc_mlang_table;
		using namespace contents_seq_table;
		using namespace contents_code_table;

		extern const _TCHAR kTableName[]; //!< テーブル名

		extern const _TCHAR kAnnotationScale[]; //!< 注記表示スケール
		extern const _TCHAR kAnnotationID[];    //!< ANNOTATIONID
		extern const _TCHAR kWorkName[];        //!< 作業名
		extern const _TCHAR kOrgName[];         //!< 元名称
		extern const _TCHAR kOrgYomi[];         //!< 元読み
		extern const _TCHAR kName[];            //!< 名称

		/// 表示レイヤコード
		namespace display_layer
		{
			enum ECode
			{
				city   = 0,      //!< CITY
				base   = 1,      //!< BASE
				middle = 2,      //!< MIDDLE
				top    = 3,      //!< TOP
			};
		}
	}

	/// 多言語一般POI
	namespace poi
	{
		using namespace ipc_mlang_table;
		using namespace serial_id_table;
		using namespace contents_seq_table;
		using namespace group_code_table;
		using namespace chain_code_table;

		extern const _TCHAR kTableName[]; //!< テーブル名

		extern const _TCHAR kName[]; //!< 名称
	}

	/// 多言語一般POI部分一致
	namespace poi_partial
	{
		using namespace ipc_mlang_table;
		using namespace serial_id_table;
		using namespace contents_seq_table;
		using namespace group_code_table;
		using namespace chain_code_table;
		using namespace partial_name_table;

		extern const _TCHAR kTableName[]; //!< テーブル名

	}

	/// 多言語鉄道
	namespace railway
	{
		using namespace ipc_mlang_table;
		using namespace serial_id_table;
		using namespace contents_seq_table;
		using namespace group_code_table;

		extern const _TCHAR kTableName[]; //!< テーブル名

		extern const _TCHAR kTrainTypeCode[]; //!< 鉄道分類コード
		extern const _TCHAR kTrainCorpCode[]; //!< 鉄道会社コード
		extern const _TCHAR kTrainLineCode[]; //!< 鉄道路線コード
		extern const _TCHAR kLineSeq[];       //!< 路線内シーケンス
		extern const _TCHAR kStationSeq[];    //!< 鉄道駅内シーケンス
		extern const _TCHAR kName[];          //!< 名称
		extern const _TCHAR kSubName[];       //!< 略称・補足名称
	}

	/// 多言語鉄道部分一致
	namespace railway_partial
	{
		using namespace ipc_mlang_table;
		using namespace serial_id_table;
		using namespace contents_seq_table;
		using namespace group_code_table;
		using namespace partial_name_table;

		extern const _TCHAR kTableName[]; //!< テーブル名
	}

	/// 多言語住所
	namespace addr
	{
		using namespace ipc_mlang_table;
		using namespace contents_seq_table;
		using namespace group_code_table;

		extern const _TCHAR kTableName[]; //!< テーブル名

		extern const _TCHAR kAddrCode[];      //!< 20桁住所コード
		extern const _TCHAR kName[];          //!< 名称
		extern const _TCHAR kSubName[];       //!< 略称・補足名称
	}

	/// 多言語道路施設
	namespace road
	{
		using namespace ipc_mlang_table;
		using namespace serial_id_table;
		using namespace contents_seq_table;
		using namespace group_code_table;

		extern const _TCHAR kTableName[];    //!< テーブル名

		extern const _TCHAR kRoadTypeCode[]; //!< 道路分類コード
		extern const _TCHAR kRoadLineCode[]; //!< 道路路線コード
		extern const _TCHAR kRoadSeq[];      //!< 路線内シーケンス
		extern const _TCHAR kFclAttCode[];   //!< 道路施設種別コード
		extern const _TCHAR kDirectionCode[];//!< 方面コード
		extern const _TCHAR kName[];         //!< 名称
		extern const _TCHAR kSubName[];      //!< 補足名称
		extern const _TCHAR kShortName[];    //!< 略称
		extern const _TCHAR kDisplayName[];    //!< 表示用名称
	}

	/// 多言語道路施設部分一致
	namespace road_partial
	{
		using namespace ipc_mlang_table;
		using namespace serial_id_table;
		using namespace contents_seq_table;
		using namespace group_code_table;
		using namespace partial_name_table;

		extern const _TCHAR kTableName[];   //!< テーブル名
	}

	/// 多言語マスタ
	namespace master
	{
		using namespace ipc_mlang_table;

		namespace chain_master
		{
			extern const _TCHAR kTableName[]; //!< テーブル名

			extern const _TCHAR kChainCode[]; //!< 店舗コード
			extern const _TCHAR kName[]; //!< 名称
		}

		namespace group_master
		{
			extern const _TCHAR kTableName[]; //!< テーブル名

			extern const _TCHAR kGroupCode[]; //!< データ分類コード
			extern const _TCHAR kName[]; //!< 名称
		}

	}

	// リリース用 多言語都道府県ポイント
	namespace pref_point
	{
		using namespace ipc_mlang_table;

		extern const _TCHAR kTableName[];   //!< テーブル名

		extern const _TCHAR kPrefCode[];    //!< 都道府県コード
		extern const _TCHAR kName[];        //!< 名称
	}

	// リリース用 多言語市区町村ポイント
	namespace city_point
	{
		using namespace ipc_mlang_table;

		extern const _TCHAR kTableName[];   //!< テーブル名

		extern const _TCHAR kPrefCode[];    //!< 都道府県コード
		extern const _TCHAR kCityCode[];    //!< 市区町村コード
		extern const _TCHAR kName[];        //!< 名称
	}

	// リリース用 多言語町・大字ポイント
	namespace oaza_point
	{
		using namespace ipc_mlang_table;

		extern const _TCHAR kTableName[];   //!< テーブル名

		extern const _TCHAR kPrefCode[];    //!< 都道府県コード
		extern const _TCHAR kCityCode[];    //!< 市区町村コード
		extern const _TCHAR kAddrCode1[];   //!< 町・大字コード
		extern const _TCHAR kName[];        //!< 名称
	}

	// リリース用 多言語丁目・字ポイント
	namespace aza_point
	{
		using namespace ipc_mlang_table;

		extern const _TCHAR kTableName[];   //!< テーブル名

		extern const _TCHAR kPrefCode[];    //!< 都道府県コード
		extern const _TCHAR kCityCode[];    //!< 市区町村コード
		extern const _TCHAR kAddrCode1[];   //!< 町・大字コード
		extern const _TCHAR kAddrCode2[];   //!< 丁目・大コード
		extern const _TCHAR kName[];        //!< 名称
	}

	// リリース用 多言語街区・地番ポイント
	namespace main_no_point
	{
		using namespace ipc_mlang_table;

		extern const _TCHAR kTableName[];   //!< テーブル名

		extern const _TCHAR kPrefCode[];    //!< 都道府県コード
		extern const _TCHAR kCityCode[];    //!< 市区町村コード
		extern const _TCHAR kAddrCode1[];   //!< 町・大字コード
		extern const _TCHAR kAddrCode2[];   //!< 丁目・大コード
		extern const _TCHAR kChibanCode[];  //!< 街区符号・地番コード
		extern const _TCHAR kName[];        //!< 名称
	}

	// リリース用 多言語号・枝番ポイント
	namespace sub_no_point
	{
		using namespace ipc_mlang_table;

		extern const _TCHAR kTableName[];   //!< テーブル名

		extern const _TCHAR kPrefCode[];    //!< 都道府県コード
		extern const _TCHAR kCityCode[];    //!< 市区町村コード
		extern const _TCHAR kAddrCode1[];   //!< 町・大字コード
		extern const _TCHAR kAddrCode2[];   //!< 丁目・大コード
		extern const _TCHAR kChibanCode[];  //!< 街区符号・地番コード
		extern const _TCHAR kJukyoCode[];   //!< 住居番号・枝番コード
		extern const _TCHAR kName[];        //!< 名称
	}
} // namespace multi_language
} // namespace schema
} // namespace sindy
