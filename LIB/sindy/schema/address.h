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

#ifndef SINDY_SCHEMA_ADDRESS_H_
#define SINDY_SCHEMA_ADDRESS_H_

#include <sindy/schema/common.h>
#include <sindy/schema/reference.h>

namespace sindy {

namespace schema {

// 不定地ポリゴン
namespace indef_polygon {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kCityCode[];			//!< 市区町村コード
	extern const _TCHAR kAddrCode[];			//!< 住所コード
} // indef_polygon

// 編集用 市区町村ポリゴン
namespace city_polygon
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kCityCode[];      //!< 市区町村コード
}

// 編集用 住所ポリゴン
namespace addr_polygon
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kCityCode[];      //!< 市区町村コード
	extern const _TCHAR kAddrCode1[];     //!< 住所コード（公称）
	extern const _TCHAR kExtName1[];      //!< 拡張名称（公称）
	extern const _TCHAR kExtYomi1[];      //!< 拡張読み（公称）
	extern const _TCHAR kAddrCode2[];     //!< 住所コード（通称）
	extern const _TCHAR kExtName2[];      //!< 拡張名称（通称）
	extern const _TCHAR kExtYomi2[];      //!< 拡張読み（通称）
	extern const _TCHAR kGaikufugo[];     //!< 街区符号
	extern const _TCHAR kAddrType[];      //!< 住所整備種別コード
	extern const _TCHAR kColorCode[];     //!< 街区色付与用色属性コード

	/// 住所整備種別コード
	namespace addr_type {
		enum ECode {
			kAreaUndefined           = 0, //!< 町丁目界未整備
			kDefinedWithDispAddr     = 1, //!< 住居表示整備済み
			kDefinedWithChiban       = 2, //!< 地番整備済み
			kUndefinedWithChiban     = 3, //!< 住居表示・地番未整備
		};
	} // addr_type
}

// 編集用 地名代表ポイント
namespace placename_rep_point
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kCityCode[];      //!< 市区町村コード
	extern const _TCHAR kAddrCode[];      //!< 住所コード
	extern const _TCHAR kName[];          //!< 代表点名称
	extern const _TCHAR kZipCode[];       //!< 郵便番号
}

// 整備用 住所ポイント
namespace addr_point
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kCityCode[];      //!< 市区町村コード
	extern const _TCHAR kAddrCode[];      //!< 住所コード
	extern const _TCHAR kGouNo[];         //!< 号番号
	extern const _TCHAR kYomi[];          //!< 読み
	extern const _TCHAR kPriority[];      //!< 優先フラグ
	extern const _TCHAR kIgnore[];        //!< 街区符号無効フラグ
	extern const _TCHAR kPointType[];     //!< ポイント整備種別コード
	extern const _TCHAR kConfirm[];       //!< 確認ステータスコード
	extern const _TCHAR kGeoBldID[];      //!< Geospace建物ID
	extern const _TCHAR kIPCBldID[];      //!< iPC建物ID

	// 住所ポイント整備種別コード
	namespace point_type {
		enum ECode {
			kAddrWithBldg         = 1, //!< 建物に紐づく住所
			kMultiAddrWithBldg    = 2, //!< 同一建物複数異住所
			kOutsideArea          = 3, //!< エリア外住所
			kWithinArea           = 4, //!< エリア内住所（建物紐付きなし）
			kAddrWithSubBldg      = 5, //!< 付随建物
		};
	} // pointy_type

	// 住所ポイント確認ステータスコード
	namespace confirm {
		enum ECode {
			kNone                = 0, //!< 未確認（エビデンスなし
			kInitGS              = 1, //!< 初期データ（Geospace）
			kInitIPC             = 2, //!< 初期データ（iPC）
			kPublicData          = 3, //!< 公的資料
			kResearched          = 4, //!< 調査資料
			kClaim               = 5, //!< クレーム
		};
	} // confirm
}

// 編集用 拡張コードリスト
namespace extcode_list
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kExtCode[];      //!< 拡張コード
	extern const _TCHAR kName[];         //!< 名称
	extern const _TCHAR kYomi[];         //!< 読み
	extern const _TCHAR kCreateDate[];   //!< 作成年月
}

// 編集用 文字付地番マスタ
namespace char_main_no_master
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kCityCode[];      //!< 市区町村コード
	extern const _TCHAR kAddrCode[];      //!< 住所コード
	extern const _TCHAR kChibanCode[];    //!< 街区符号・地番コード
	extern const _TCHAR kName[];          //!< 名称
	extern const _TCHAR kYomi[];          //!< 読み
}

// 編集用 文字付号マスタ
namespace char_sub_no_master
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kCityCode[];      //!< 市区町村コード
	extern const _TCHAR kAddrCode[];      //!< 住所コード
	extern const _TCHAR kChibanCode[];    //!< 街区符号・地番コード
	extern const _TCHAR kJukyoCode[];     //!< 住居番号・枝番コード
	extern const _TCHAR kName[];          //!< 名称
	extern const _TCHAR kYomi[];          //!< 読み
}

// 接頭辞接尾辞条件コード
namespace condition_code_table
{
	extern const _TCHAR kConditionCode[];   //!< 条件コード

	namespace condition_code {
		enum ECode {
			kNone = 0,                      //!< 該当無し
			kHead = 1,                      //!< 先頭
			kTail = 2,                      //!< 末尾
			kTailAfterNum = 3,              //!< 末尾+算用数字の直後
		};
	} // condition_code
}

// 編集用 接頭辞種別マスタ
namespace prefix_master
{
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::condition_code_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefix[];        //!< 接頭辞種別コード
	extern const _TCHAR kName[];          //!< 名称
	extern const _TCHAR kYomi[];          //!< 読み
}

// 編集用 接尾辞種別マスタ
namespace suffix_master
{
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::condition_code_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kSuffix[];          //!< 接尾辞種別コード
	extern const _TCHAR kName[];            //!< 名称
	extern const _TCHAR kYomi[];            //!< 読み
}

// 編集用 行政区域種別マスタ
namespace admin_att_master
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kAdminAtt[];      //!< 接尾辞種別コード
	extern const _TCHAR kName[];          //!< 名称
}

// 公称通称種別コード
namespace name_code_table {
	extern const _TCHAR kNameCode[];      //!< 公称通称種別コード

	namespace name_code {
		enum ECode {
			kOfficial      = 0, //!< 公称
			kLocal         = 1, //!< 通称
			kStreet        = 2, //!< 通り名
		};
	} // name_code
}

// 住居表示区域コード
namespace addr_disp_table {
	extern const _TCHAR kAddrDisp[];      //!< 住居表示区域コード

	namespace addr_disp {
		enum ECode {
			kOutsideArea   = 0, //!< 住居表示区域外
			kWithinArea    = 1, //!< 住居表示区域
			kMixed         = 2, //!< 混在
		};
	} // addr_disp
}

// 生成データ種別コード
namespace gnrt_type_table {
	extern const _TCHAR kGnrtType[];      //!< 生成データ種別コード

	namespace gnrt_type {
		enum ECode {
			kNormal          = 0, //!< 通常整備
			kStreet          = 1, //!< 通り名（機械生成）
		};
	} // gnrt_type
}

// リリース用 中縮行政界（市区町村）ポリゴン
namespace base_admin_polygon
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];     //!< 都道府県コード
	extern const _TCHAR kCityCode[];     //!< 市区町村コード
}

// リリース用 都市地図行政界（街区）ポリゴン
namespace city_admin_polygon
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];       //!< 都道府県コード
	extern const _TCHAR kCityCode[];       //!< 市区町村コード
	extern const _TCHAR kAddrCode1[];      //!< 町・大字コード
	extern const _TCHAR kAddrCode2[];      //!< 丁目・字コード
	extern const _TCHAR kChibanCode[];     //!< 街区符号・地番コード
}

// リリース用 都道府県ポイント
namespace pref_point
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];      //!< 都道府県コード
	extern const _TCHAR kName[];          //!< 名称
	extern const _TCHAR kYomi[];          //!< 読み
}

// リリース用 市区町村ポイント
namespace city_point
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];      //!< 都道府県コード
	extern const _TCHAR kCityCode[];      //!< 市区町村コード
	extern const _TCHAR kName[];          //!< 名称
	extern const _TCHAR kYomi[];          //!< 読み
	extern const _TCHAR kAdminAtt[];
}

// リリース用 町・大字ポイント
namespace oaza_point
{
	using namespace ::sindy::schema::ipc_feature;
	using namespace ::sindy::schema::name_code_table;
	using namespace ::sindy::schema::addr_disp_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];      //!< 都道府県コード
	extern const _TCHAR kCityCode[];      //!< 市区町村コード
	extern const _TCHAR kAddrCode1[];     //!< 町・大字コード
	extern const _TCHAR kName[];          //!< 名称
	extern const _TCHAR kYomi[];          //!< 読み
	extern const _TCHAR kPrefix[];        //!< 接頭辞種別コード
	extern const _TCHAR kSuffix[];        //!< 接尾辞種別コード
}

// リリース用 丁目・字ポイント
namespace aza_point
{
	using namespace ::sindy::schema::ipc_feature;
	using namespace ::sindy::schema::name_code_table;
	using namespace ::sindy::schema::addr_disp_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];      //!< 都道府県コード
	extern const _TCHAR kCityCode[];      //!< 市区町村コード
	extern const _TCHAR kAddrCode1[];     //!< 町・大字コード
	extern const _TCHAR kAddrCode2[];     //!< 丁目・字コード
	extern const _TCHAR kName[];          //!< 名称
	extern const _TCHAR kYomi[];          //!< 読み
	extern const _TCHAR kPrefix[];        //!< 接頭辞種別コード
	extern const _TCHAR kSuffix[];        //!< 接尾辞種別コード
}

// リリース用 街区・地番ポイント
namespace main_no_point
{
	using namespace ::sindy::schema::ipc_feature;
	using namespace ::sindy::schema::addr_disp_table;
	using namespace ::sindy::schema::gnrt_type_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];      //!< 都道府県コード
	extern const _TCHAR kCityCode[];      //!< 市区町村コード
	extern const _TCHAR kAddrCode1[];     //!< 町・大字コード
	extern const _TCHAR kAddrCode2[];     //!< 丁目・字コード
	extern const _TCHAR kChibanCode[];    //!< 街区符号・地番コード
	extern const _TCHAR kName[];          //!< 名称
	extern const _TCHAR kYomi[];          //!< 読み
	extern const _TCHAR kStatus[];        //!< 地番状態コード
	extern const _TCHAR kPntType[];       //!< 街区・地番識別コード
	extern const _TCHAR kRep[];           //!< 代表点フラグ
	extern const _TCHAR kBranch[];        //!< 支号フラグ
	extern const _TCHAR kMulti[];         //!< 複数住所フラグ
	extern const _TCHAR kBldOID[];        //!< 建物ID

	/// 地番状態コード
	namespace status {
		enum ECode {
			kNone           = 0, //!< 未設定
			kPolygon        = 1, //!< ポリゴンから生成
			kSingleChiban   = 2, //!< 地番止まりのポイントが1点、地番止まりではないポイントが0
			kSingleGou      = 3, //!< 地番止まりのポイントが0、地番止まりではないポイントが1点
			kMultiChiban    = 4, //!< 地番止まりのポイントが複数、地番止まりではないポイントが0
			kMultiGou       = 5, //!< 地番止まりのポイントが0、地番止まりではないポイントが複数
			kChibanGouMixed = 6, //!< 地番止まりのポイントが1以上、地番止まりではないポイントが1以上
		};
	} // status

	/// 街区・地番識別コード
	namespace pnt_type {
		enum ECode {
			kBlock       = 0, //!< 街区
			kChiban      = 1, //!< 地番
		};
	} // pnt_type
}

// リリース用 号・枝番ポイント
namespace sub_no_point
{
	using namespace ::sindy::schema::ipc_feature;
	using namespace ::sindy::schema::gnrt_type_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPrefCode[];      //!< 都道府県コード
	extern const _TCHAR kCityCode[];      //!< 市区町村コード
	extern const _TCHAR kAddrCode1[];     //!< 町・大字コード
	extern const _TCHAR kAddrCode2[];     //!< 丁目・字コード
	extern const _TCHAR kChibanCode[];    //!< 街区符号・地番コード
	extern const _TCHAR kJukyoCode[];     //!< 住居番号・枝番コード
	extern const _TCHAR kName[];          //!< 名称
	extern const _TCHAR kYomi[];          //!< 読み
	extern const _TCHAR kRep[];           //!< 代表点フラグ
	extern const _TCHAR kBranch[];        //!< 枝番フラグ
	extern const _TCHAR kMulti[];         //!< 複数住所フラグ
	extern const _TCHAR kBldOID[];        //!< 建物ID
}

// リリース用 郵便番号ポイント
namespace zip_point
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kZipCode[];       //!< 郵便番号
	extern const _TCHAR kZipAtt[];        //!< 郵便番号種別コード
	extern const _TCHAR kName[];          //!< 名称
	extern const _TCHAR kYomi[];          //!< 読み
	extern const _TCHAR kPrefCode[];      //!< 都道府県コード
	extern const _TCHAR kCityCode[];      //!< 市区町村コード
	extern const _TCHAR kAddrCode1[];     //!< 町・大字コード
	extern const _TCHAR kAddrCode2[];     //!< 丁目・字コード
	extern const _TCHAR kRep[];           //!< 代表点フラグ

	/// 郵便番号種別コード
	namespace zip_att {
		enum ECode {
			kAddress           = 1, //!< 住所
			kBuilding          = 2, //!< ビル
			kOffice            = 3, //!< 大口事業所
			kPostOfficeBox     = 4, //!< 私書箱
		};
	}
}

// リリース用 郵便番号ポリゴン
namespace zip_polygon
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kZipCode[];       //!< 郵便番号
	extern const _TCHAR kPriority[];      //!< 優先度
}

// リリース用 郵便番号ポリゴン重心ポイント
namespace zip_centroid
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kZipCode[];       //!< 郵便番号
}

// リリース用 郵便番号に紐付く市区町村代表ポイント
namespace zip_rep
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kZipCode[];       //!< 郵便番号
	extern const _TCHAR kPrefCode[];      //!< 都道府県コード
	extern const _TCHAR kCityCode[];      //!< 市区町村コード
}

// リリース用 郵便番号に紐付く住所コード 
namespace rel_zip_addr
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kZipCode[];       //!< 郵便番号
	extern const _TCHAR kPrefCode[];      //!< 都道府県コード
	extern const _TCHAR kCityCode[];      //!< 市区町村コード
	extern const _TCHAR kAddrCode1[];     //!< 町・大字コード
	extern const _TCHAR kAddrCode2[];     //!< 丁目・字コード
	extern const _TCHAR kChibanCode[];    //!< 街区符号・地番コード
	extern const _TCHAR kJukyoCode[];     //!< 住居番号・枝番コード
}

} // schema

} // sindy

#endif // ifndef SINDY_SCHEMA_ADDRESS_H_
