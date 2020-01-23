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
#include <sindy/schema/address.h>

namespace sindy {

namespace schema {

namespace indef_polygon {
	const _TCHAR kTableName[]			= _T("INDEF_POLYGON");			//!< テーブル名
	const _TCHAR kCityCode[]			= _T("CITYCODE");				//!< 市区町村コード
	const _TCHAR kAddrCode[]			= _T("ADDRCODE");				//!< 住所コード
}

// 編集用 市区町村ポリゴン
namespace city_polygon
{

	const _TCHAR kTableName[] = _T("CITY_POLYGON");

	const _TCHAR kCityCode[] = _T("CITYCODE");;
}

// 編集用 住所ポリゴン
namespace addr_polygon
{
	const _TCHAR kTableName[] = _T("ADDR_POLYGON");

	const _TCHAR kCityCode[] = _T("CITYCODE");
	const _TCHAR kAddrCode1[] = _T("ADDRCODE1");
	const _TCHAR kExtName1[] = _T("EXT_NAME1");
	const _TCHAR kExtYomi1[] = _T("EXT_YOMI1");
	const _TCHAR kAddrCode2[] = _T("ADDRCODE2");
	const _TCHAR kExtName2[] = _T("EXT_NAME2");
	const _TCHAR kExtYomi2[] = _T("EXT_YOMI2");
	const _TCHAR kGaikufugo[] = _T("GAIKUFUGO");
	const _TCHAR kAddrType[] = _T("ADDRTYPE_C");
	const _TCHAR kColorCode[] = _T("COLORCODE");
}

// 編集用 地名代表ポイント
namespace placename_rep_point
{
	const _TCHAR kTableName[] = _T("PLACENAME_REP_POINT");

	const _TCHAR kCityCode[] = _T("CITYCODE");
	const _TCHAR kAddrCode[] = _T("ADDRCODE");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kZipCode[] = _T("ZIPCODE");
}

// 整備用 住所ポイント
namespace addr_point
{
	const _TCHAR kTableName[] = _T("ADDR_POINT");

	const _TCHAR kCityCode[] = _T("CITYCODE");
	const _TCHAR kAddrCode[] = _T("ADDRCODE");
	const _TCHAR kGouNo[] = _T("GOU_NO");
	const _TCHAR kYomi[] = _T("YOMI");
	const _TCHAR kPriority[] = _T("PRIORITY_F");
	const _TCHAR kIgnore[] = _T("IGNORE_F");
	const _TCHAR kPointType[] = _T("POINTTYPE_C");
	const _TCHAR kConfirm[] = _T("CONFIRM_C");
	const _TCHAR kGeoBldID[] = _T("GEO_BLD_ID");
	const _TCHAR kIPCBldID[] = _T("IPC_BLD_ID");
}

// 編集用 拡張コードリスト
namespace extcode_list
{
	const _TCHAR kTableName[] = _T("EXTCODE_LIST");

	const _TCHAR kExtCode[] = _T("EXTCODE");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kYomi[] = _T("YOMI");
	const _TCHAR kCreateDate[] = _T("CREATE_DATE");
}

// 編集用 文字付地番マスタ
namespace char_main_no_master
{
	const _TCHAR kTableName[] = _T("CHAR_MAIN_NO_MASTER");

	const _TCHAR kCityCode[] = _T("CITYCODE");
	const _TCHAR kAddrCode[] = _T("ADDRCODE");
	const _TCHAR kChibanCode[] = _T("CHIBANCODE");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kYomi[] = _T("YOMI");
}

// 編集用 文字付号マスタ
namespace char_sub_no_master
{
	const _TCHAR kTableName[] = _T("CHAR_SUB_NO_MASTER");

	const _TCHAR kCityCode[] = _T("CITYCODE");
	const _TCHAR kAddrCode[] = _T("ADDRCODE");
	const _TCHAR kChibanCode[] = _T("CHIBANCODE");
	const _TCHAR kJukyoCode[] = _T("JUKYOCODE");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kYomi[] = _T("YOMI");
}

// 接頭辞接尾辞条件コード
namespace condition_code_table {
	const _TCHAR kConditionCode[] = _T("COND_C");
}


// 編集用 接頭辞種別マスタ
namespace prefix_master
{
	const _TCHAR kTableName[] = _T("PREFIX_MASTER");

	const _TCHAR kPrefix[] = _T("PREFIX_C");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kYomi[] = _T("YOMI");
}

// 編集用 接尾辞種別マスタ
namespace suffix_master
{
	const _TCHAR kTableName[] = _T("SUFFIX_MASTER");
	
	const _TCHAR kSuffix[] = _T("SUFFIX_C");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kYomi[] = _T("YOMI");
}

// 編集用 行政区域種別マスタ
namespace admin_att_master
{
	const _TCHAR kTableName[] = _T("ADMIN_ATT_MASTER");
	
	const _TCHAR kAdminAtt[] = _T("ADMINATT_C");
	const _TCHAR kName[] = _T("NAME");
}

// 公称通称種別コード
namespace name_code_table {
	const _TCHAR kNameCode[] = _T("NAME_C");
}

// 住居表示区域コード
namespace addr_disp_table {
	const _TCHAR kAddrDisp[] = _T("ADDRDISP_C");
}

// 生成データ種別コード
namespace gnrt_type_table {
	const _TCHAR kGnrtType[] = _T("GNRT_TYPE");
}

// リリース用 中縮行政界（市区町村）ポリゴン
namespace base_admin_polygon
{
	const _TCHAR kTableName[] = _T("BASE_ADMIN_POLYGON");

	const _TCHAR kPrefCode[] = _T("PREFCODE");
	const _TCHAR kCityCode[] = _T("CITYCODE");
}

// リリース用 都市地図行政界（街区）ポリゴン
namespace city_admin_polygon
{
	const _TCHAR kTableName[] = _T("CITY_ADMIN_POLYGON");

	const _TCHAR kPrefCode[] = _T("PREFCODE");
	const _TCHAR kCityCode[] = _T("CITYCODE");
	const _TCHAR kAddrCode1[] = _T("ADDRCODE1");
	const _TCHAR kAddrCode2[] = _T("ADDRCODE2");
	const _TCHAR kChibanCode[] = _T("CHIBANCODE");
}

// リリース用 都道府県ポイント
namespace pref_point
{
	const _TCHAR kTableName[] = _T("PREF_POINT");

	const _TCHAR kPrefCode[] = _T("PREFCODE");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kYomi[] = _T("YOMI");
}

// リリース用 市区町村ポイント
namespace city_point
{
	const _TCHAR kTableName[] = _T("CITY_POINT");

	const _TCHAR kPrefCode[] = _T("PREFCODE");
	const _TCHAR kCityCode[] = _T("CITYCODE");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kYomi[] = _T("YOMI");
	const _TCHAR kAdminAtt[] = _T("ADMINATT_C");
}

// リリース用 町・大字ポイント
namespace oaza_point
{
	const _TCHAR kTableName[] = _T("OAZA_POINT");

	const _TCHAR kPrefCode[] = _T("PREFCODE");
	const _TCHAR kCityCode[] = _T("CITYCODE");
	const _TCHAR kAddrCode1[] = _T("ADDRCODE1");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kYomi[] = _T("YOMI");
	const _TCHAR kPrefix[] = _T("PREFIX_C");
	const _TCHAR kSuffix[] = _T("SUFFIX_C");
}

// リリース用 丁目・字ポイント
namespace aza_point
{
	const _TCHAR kTableName[] = _T("AZA_POINT");

	const _TCHAR kPrefCode[] = _T("PREFCODE");
	const _TCHAR kCityCode[] = _T("CITYCODE");
	const _TCHAR kAddrCode1[] = _T("ADDRCODE1");
	const _TCHAR kAddrCode2[] = _T("ADDRCODE2");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kYomi[] = _T("YOMI");
	const _TCHAR kPrefix[] = _T("PREFIX_C");
	const _TCHAR kSuffix[] = _T("SUFFIX_C");
}

// リリース用 街区・地番ポイント
namespace main_no_point
{
	const _TCHAR kTableName[] = _T("MAIN_NO_POINT");

	const _TCHAR kPrefCode[] = _T("PREFCODE");
	const _TCHAR kCityCode[] = _T("CITYCODE");
	const _TCHAR kAddrCode1[] = _T("ADDRCODE1");
	const _TCHAR kAddrCode2[] = _T("ADDRCODE2");
	const _TCHAR kChibanCode[] = _T("CHIBANCODE");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kYomi[] = _T("YOMI");
	const _TCHAR kStatus[] = _T("STATUS_C");
	const _TCHAR kPntType[] = _T("PNTTYPE_C");
	const _TCHAR kRep[] = _T("REP_F");
	const _TCHAR kBranch[] = _T("BRANCH_F");
	const _TCHAR kMulti[] = _T("MULTI_F");
	const _TCHAR kBldOID[] = _T("BLD_OID");
}

// リリース用 号・枝番ポイント
namespace sub_no_point
{
	const _TCHAR kTableName[] = _T("SUB_NO_POINT");

	const _TCHAR kPrefCode[] = _T("PREFCODE");
	const _TCHAR kCityCode[] = _T("CITYCODE");
	const _TCHAR kAddrCode1[] = _T("ADDRCODE1");
	const _TCHAR kAddrCode2[] = _T("ADDRCODE2");
	const _TCHAR kChibanCode[] = _T("CHIBANCODE");
	const _TCHAR kJukyoCode[] = _T("JUKYOCODE");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kYomi[] = _T("YOMI");
	const _TCHAR kRep[] = _T("REP_F");
	const _TCHAR kBranch[] = _T("BRANCH_F");
	const _TCHAR kMulti[] = _T("MULTI_F");
	const _TCHAR kBldOID[] = _T("BLD_OID");
}

// リリース用 郵便番号ポイント
namespace zip_point
{
	const _TCHAR kTableName[] = _T("ZIP_POINT");

	const _TCHAR kZipCode[] = _T("ZIPCODE");
	const _TCHAR kZipAtt[] = _T("ZIP_ATT_C");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kYomi[] = _T("YOMI");
	const _TCHAR kPrefCode[] = _T("PREFCODE");
	const _TCHAR kCityCode[] = _T("CITYCODE");
	const _TCHAR kAddrCode1[] = _T("ADDRCODE1");
	const _TCHAR kAddrCode2[] = _T("ADDRCODE2");
	const _TCHAR kRep[] = _T("REP_F");
}

// リリース用 郵便番号ポリゴン
namespace zip_polygon
{
	const _TCHAR kTableName[] = _T("ZIP_POLYGON");

	const _TCHAR kZipCode[] = _T("ZIPCODE");
	const _TCHAR kPriority[] = _T("PRIORITY");
}

// リリース用 郵便番号ポリゴン重心ポイント
namespace zip_centroid
{
	const _TCHAR kTableName[] = _T("ZIP_CENTROID");

	const _TCHAR kZipCode[] = _T("ZIPCODE");
}

// 郵便番号に紐付く市区町村代表ポイント
namespace zip_rep
{
	const _TCHAR kTableName[] = _T("ZIP_REP");

	const _TCHAR kZipCode[] = _T("ZIPCODE");
	const _TCHAR kPrefCode[] = _T("PREFCODE");
	const _TCHAR kCityCode[] = _T("CITYCODE");
}

// リリース用 郵便番号に紐付く住所コード 
namespace rel_zip_addr
{
	const _TCHAR kTableName[] = _T("REL_ZIP_ADDR");

	const _TCHAR kZipCode[] = _T("ZIPCODE");
	const _TCHAR kPrefCode[] = _T("PREFCODE");
	const _TCHAR kCityCode[] = _T("CITYCODE");
	const _TCHAR kAddrCode1[] = _T("ADDRCODE1");
	const _TCHAR kAddrCode2[] = _T("ADDRCODE2");
	const _TCHAR kChibanCode[] = _T("CHIBANCODE");
	const _TCHAR kJukyoCode[] = _T("JUKYOCODE");
}

} // schema

} // sindy
