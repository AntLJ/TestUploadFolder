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

#ifndef SINDY_SCHEMA_REFERENCE_H_
#define SINDY_SCHEMA_REFERENCE_H_

#include <sindy/schema/category.h>
#include <sindy/schema/common.h>

namespace sindy {

namespace schema {

/// 都市地図メッシュテーブル
namespace citymesh {
	using namespace ::sindy::schema::category::mesh_code_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kCityMeshCode[];		//!< メッシュコード(ライブラリ使用している他ツールで整合取るため、残しておく 2007/10/16 s_suzuki)
	extern const _TCHAR kCityName[];			//!< 市区町村名
	extern const _TCHAR kCreateYear[];			//!< 作成年度
	extern const _TCHAR kUpdateYear[];			//!< 作業年度
	extern const _TCHAR kComplete[];			//!< 完全化フラグ
	extern const _TCHAR kSea[];					//!< 海フラグ
	extern const _TCHAR kZuka[];				//!< 図化フラグ
	extern const _TCHAR kRaster[];				//!< ラスター
	extern const _TCHAR kCreateRasterYear[];	//!< ラスター作成年度
	extern const _TCHAR kCurrentAdjYear[];		//!< 現況修正年度
	extern const _TCHAR kPhotoShotDate[];		//!< 使用写真撮影年月日
	extern const _TCHAR kFieldResearchYear[];	//!< 現地調査実施年度（地図）
	extern const _TCHAR kTrSurveyYear[];		//!< 現地調査実施年度（規制）
	extern const _TCHAR kShaBgworkDate[];		//!< 上海作業年月（平面）
	extern const _TCHAR kShaAnnoworkDate[];		//!< 上海作業年月（注記）
	extern const _TCHAR kCreateAnnoMsDate[];	//!< 注記原稿作成年月
	extern const _TCHAR kAddrMainteYear[];		//!< 住所整備見直し年度
	extern const _TCHAR kJmapPublicationYear[];	//!< 参照j地図発行年度
	extern const _TCHAR kMapcubeWorkYear[];		//!< mapcube作業年度
	extern const _TCHAR kPdmUseYear[];			//!< pdm使用年度
} // citymesh

/// ベースメッシュフィーチャクラス
namespace basemesh {
	using namespace ::sindy::schema::category::mesh_code_table;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// ミドルメッシュテーブル
namespace middlemesh {
	using namespace ::sindy::schema::category::mesh_code_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kMiddleMeshCode[];		//!< メッシュコード(ライブラリ使用している他ツールで整合取るため、残しておく 2007/10/16 s_suzuki)
} // middlemesh

/// トップメッシュフィーチャクラス
namespace topmesh {
	using namespace ::sindy::schema::category::mesh_code_table;

	extern const _TCHAR kTableName[];			///< テーブル名
}

/// 参照用VICSフィーチャクラス
namespace org_vics {
	using namespace ::sindy::schema::category::original_vics_table;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// 参照用インターナビVICSフィーチャクラス
namespace ivics {
	using namespace ::sindy::schema::category::original_vics_table;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// 参照用走りやすさマップフィーチャクラス
namespace drive_map {
	using namespace ::sindy::schema::category::drive_map_table;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// TOWNPAGEフィーチャクラス
namespace townpage {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kIpCode[];		//!< IPコード
	extern const _TCHAR kDataKubun[];		//!< データ取得区分
	extern const _TCHAR kDataSeq[];		//!< データ取得シーケンス
	extern const _TCHAR kPstIpCode[];		//!< 座標提供会社コード
	extern const _TCHAR kAccCode[];		//!< 精度コード
	extern const _TCHAR kPinPointFlag[];	//!< ピンポイントフラグ
	extern const _TCHAR kPrefCode[];		//!< 都道府県コード
	extern const _TCHAR kCityCode[];		//!< 市区町村コード
	extern const _TCHAR kAdrCode1[];		//!< 大字・通称コード
	extern const _TCHAR kAdrCode2[];		//!< 字・丁目コード
	extern const _TCHAR kChibanCode[];	//!< 地番コード
	extern const _TCHAR kJukyoCode[];		//!< 住居番号コード
	extern const _TCHAR kNttGrpCode[];	//!< NTT分類コード
	extern const _TCHAR kNttCode[];		//!< NTT業種コード
	extern const _TCHAR kChainCode[];		//!< 店舗コード
	extern const _TCHAR kName[];			//!< 表示用名称
	extern const _TCHAR kYomi[];			//!< 表示用名称読み
	extern const _TCHAR kTelC[];			//!< 市街局番
	extern const _TCHAR kTel[];			//!< 電話番号
	extern const _TCHAR kNaCode[];		//!< 使用禁止理由コード
}

} // namespace schema

} // namespace sindy

#endif // SINDY_SCHEMA_REFERENCE_H_
