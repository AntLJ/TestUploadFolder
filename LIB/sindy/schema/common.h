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

#ifndef SINDY_SCHEMA_COMMON_H_
#define SINDY_SCHEMA_COMMON_H_

#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

namespace object {
	extern const _TCHAR kObjectID[]; ///< オブジェクトID
}

namespace feature {
	using namespace ::sindy::schema::object;

	extern const _TCHAR kShape[];
}

namespace ipc_table {
	using namespace ::sindy::schema::object;

	extern const _TCHAR kOperator[]; ///< 最終更新者
	extern const _TCHAR kPurpose[]; ///< 作業目的
	extern const _TCHAR kModifyDate[]; ///< 最終更新日時
	extern const _TCHAR kUpdateType[]; ///< 最終更新内容
	extern const _TCHAR kProgModifyDate[]; ///< 最終プログラム更新日時
	extern const _TCHAR kModifyProgName[]; ///< 最終更新プログラム名
	extern const _TCHAR kUserClaim[]; ///< ユーザクレームフラグ
	extern const _TCHAR kSource[]; ///< 情報ソース

	namespace update_type {
		enum ECode {
			kDefault,       ///< 移行時既存
			kCreated,       ///< 新規作成
			kDeleted,       ///< 削除
			kShapeUpdated,  ///< 形状変更
			kAttrUpdated,   ///< 属性変更
			kiPSImported,   ///< 上海インポート
			kImported,      ///< インポート
		};
	}
}

namespace ipc_feature {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::feature;
}

namespace tmp_info {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTmpMeshCode[]; ///< vdbメッシュコード
	extern const _TCHAR kTmpPrimID[]; ///< vdbプリミティブID
}

namespace fieldsurvey_table {
	using namespace ::sindy::schema::tmp_info;

	extern const _TCHAR kFieldSurvey[]; ///< 現地調査フラグ
	extern const _TCHAR kWorkYear[]; ///< 現地調査年度
}

namespace meshcode_class {
	using namespace ::sindy::schema::tmp_info;
	using namespace ::sindy::schema::category::mesh_code_table;
}

namespace ipcfeature {
	using namespace ::sindy::schema::tmp_info;
}

namespace link_queue {
	using namespace ::sindy::schema::meshcode_class;
	using namespace ::sindy::schema::category::directional_link_relational_table;

	extern const _TCHAR kInfID[]; ///< リンク列ID
	extern const _TCHAR kSequence[]; ///< リンク列順番
	extern const _TCHAR kLastLink[]; ///< 終端リンクフラグ
}

namespace changetypecode {

	enum Code {
		kInsert       = 1,  //!< INSERT
		kDelete       = 2,  //!< DELETE
		kAfterUpdate  = 3,  //!< UPDATE(AFTER)
		kBeforeUpdate = 4,  //!< UPDATE(BEFORE)
	};
}

namespace countrycode {

	enum Code {
		kUndefined = 0,
		kAustralia = 36,
		kBangladesh = 50,
		kBhutan = 64,
		kBrunei_Darussalam = 96,
		kMyanmar = 104,
		kCambodia = 116,
		kChina = 156,
		kTaiwan = 158,
		kHong_Kong = 344,
		kIndia = 356,
		kIndonesia = 360,
		kJapan = 392,
		kLaos = 418,
		kMacao = 446,
		kMalaysia = 458,
		kFederated_States_of_Micronesia = 583,
		kPalau = 585,
		kPapua_New_Guinea = 598,
		kPhilippines = 608,
		kTimor_Leste = 626,
		kSingapore = 702,
		kVietnam = 704,
		kThailand = 764,
	};
}

/* ============================================================================== */
/// ADAM v2用
namespace adam_v2_global_ipc_feature {
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kGlobalID[];		///< GLOBALID
}

namespace adam_v2_adam_ipc_feature {
	using namespace ::sindy::schema::adam_v2_global_ipc_feature;

	extern const _TCHAR kSourceID[];			///< 提供元ID
	extern const _TCHAR kSperculationC[];		///< 推測コード
	extern const _TCHAR kSourceLot[];	    	///< 提供元ロット
	extern const _TCHAR kSourceUpdateC[];		///< 提供元更新コード


	namespace speculation_cpde {
		enum ECode {
			kNotSperculation	= 1,			///< 推測値でない
			kSperculation		= 2,			///< 推測値である
		};
	}

	namespace source_update_code{
		enum ECode
		{
			kNotSet   = -1,			///< 未設定
			kNoModify = 0,			///< 変更なし
			kAdd      = 1,          ///< 追加
			kDelete   = 2,          ///< 削除
			kUpdate   = 3,          ///< 属性更新
		};

	}
}

namespace adam_v2_table {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kSourceID[];			///< 提供元ID
	extern const _TCHAR kSourceLot[];	    	///< 提供元ロット
	extern const _TCHAR kSourceUpdateC[];		///< 提供元更新コード

	namespace source_update_code{
		using namespace ::sindy::schema::adam_v2_adam_ipc_feature::source_update_code;
	}
}

namespace adam_v2_global_table {
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kGlobalID[];		///< GLOBALID
}

namespace adam_v2_node_queue {
	using namespace ::sindy::schema::adam_v2_global_table;

	extern const _TCHAR kSequence[];		///< 順番
	extern const _TCHAR kLastNodeF[];		///< 終端ノードフラグ
}
/* ============================================================================== */

} // namespace schema

} // namespace sindy

#endif // SINDY_SCHEMA_COMMON_H_
