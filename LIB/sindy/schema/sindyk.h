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

namespace sindy {

namespace schema {

namespace sindyk {

/// データ識別コード
namespace idncode {
	enum ECode {
#define IDN_CODE(code,name,id) name = id,
#include "idncode.h"
#undef IDN_CODE
	};
} // idncode

namespace master_history {
	extern const _TCHAR kAddMasterCode[]; //!< 追加マスタコード
	extern const _TCHAR kDelMasterCode[]; //!< 廃止マスタコード 
}

/// データ分類マスタ
namespace group_master {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kGroupCode[]; //!< データ分類コード
	extern const _TCHAR kName[]; //!< 名称
	extern const _TCHAR kMajorGroupCode[]; //!< データ大分類コード
	using namespace master_history;
}

/// 店舗マスタ
namespace chain_master {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kChainCode[]; //!< 店舗コード
	extern const _TCHAR kChainCode8[]; //!< 店舗コード（8桁）
	extern const _TCHAR kChainGrpCode[]; //!< 店舗分類コード
	extern const _TCHAR kName[]; //!< 名称
	extern const _TCHAR kYomi[]; //!< 読み
	extern const _TCHAR kTwnSet[]; //!< タウンページ設定対象フラグ
	extern const _TCHAR kGnrSet[]; //!< 一般POI設定対象フラグ
	using namespace master_history;
}

/// コンテンツマスタ
namespace contents_master {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kGroupCode[]; //!< データ分類コード
	extern const _TCHAR kChainCode[]; //!< 店舗コード
	extern const _TCHAR kContentsCode[]; //!< コンテンツコード
	extern const _TCHAR kContentsType[]; //!< コンテンツタイプ
	extern const _TCHAR kCATGroupCode[]; //!< 大分類グループコード
	extern const _TCHAR kMNTGroupCode[]; //!< 同時メンテグループコード
	extern const _TCHAR kName[]; //!< 読み
	extern const _TCHAR kInfoFilePath[]; //!< 情報ファイル格納先
	extern const _TCHAR kInfoFileSeq[]; //!< 情報ファイルシーケンス
	using namespace master_history;

	/// コンテンツタイプコード
	namespace contents_type_code {
		enum ECode {
			kPOI = 0, //!< POI専用
			kCommon = 1, //!< 注記POI兼用
			kAnno = 2, //!< 注記専用
		};
	}
}

/// 販社マスタ
namespace sls_master {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kContentsCode[]; //!< コンテンツコード
	extern const _TCHAR kSLSCode[]; //!< 販社コード
	extern const _TCHAR kName[]; //!< 名称
	extern const _TCHAR kYomi[]; //!< 読み
	using namespace master_history;
}

/// コンテンツ機能
namespace contents_function {
	extern const _TCHAR kTableName[];		 //!< テーブル名

	extern const _TCHAR kContentsCode[]; //!< コンテンツコード
	extern const _TCHAR kIDNCode[]; //!< 機能コード
	extern const _TCHAR kLock[]; //!< ロックフラグ
	using namespace master_history;

	/// 更新コード
	namespace update_code {
		enum ECode {
			kDelete = 1, //!< 削除
			kAdd = 2, //!< 追加
		};
	}
}

/// コンテンツ紹介文
namespace contents_introduction {
	extern const _TCHAR kTableName[]; // !< テーブル名

	extern const _TCHAR kContentsCode[]; //!< コンテンツコード
	extern const _TCHAR kFeatureItemSeq[]; //!< 紹介文項目シーケンス
	extern const _TCHAR kFeatureMarkCode[]; //!< 紹介文見出しコード
	extern const _TCHAR kFeatureMarkName[]; //!< 紹介文見出し名称
	using namespace master_history;
}

/// コンテンツ詳細情報
namespace contents_detailinfo {
	extern const _TCHAR kTableName[]; // !< テーブル名

	extern const _TCHAR kContentsCode[]; //!< コンテンツコード
	extern const _TCHAR kSortOrder[]; //!< 並び順
	extern const _TCHAR kFeatureCode[]; //!< 詳細情報項目コード
	extern const _TCHAR kExist[]; //!< ありなしフラグ
	using namespace master_history;

	/// 詳細情報項目コード
	namespace dinfo_code {
		enum ECode { // TODO: マスタ更新でインポートされるものなので、とりあえず必要なものだけ定義
			kOpenHours = 1, //!< 営業時間
			kHolidays = 2, //!< 休業日
			kRapid = 4, //!< 急速
			k100v = 5, //!< 100V
			k200v = 6 //!< 200V
		};
	}
}

/// 座標値種別マスタ
namespace xy_assortment_master {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kPSTAttCode[]; //!< 座標値種別コード
	extern const _TCHAR kName[]; //!< 名称
	extern const _TCHAR kPSAPriority[]; //!< 座標値種別優先順位
	extern const _TCHAR kMNTPriority[]; //!< 整備優先順位
}

/// 座標値種別変換マスタ
namespace xy_assortment_convert {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kPSTAttCode[]; //!< 座標値種別コード
	extern const _TCHAR kOrgPSTAttCode[]; //!< 座標値種別コード（派生元）
	extern const _TCHAR kTollRoad[]; //!< 高速道路フラグ
}

/// POI識別子紐付け種別マスタ
namespace poiid_connect_master {
	extern const _TCHAR kTableName[]; //!< テーブル名
  
	extern _TCHAR kPCACode[]; //!< POI識別子紐付種別コード
	extern _TCHAR kName[]; //!< 名称
	extern _TCHAR kBankContentsCode[]; //!< コンテンツコード（銀行）
	extern _TCHAR kATMContentsCode[]; //!< コンテンツコード（ATM）
}

/// 情報提供元マスタ
namespace infoprovide_master {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kIPCode[]; //!< 情報提供元コード
	extern const _TCHAR kName[]; //!< 名称
	extern const _TCHAR kMNTPriority[]; //!< 整備優先順位
}

/// 精度マスタ
namespace accuracy_master {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kACCCode[]; //!< 精度コード
	extern const _TCHAR kName[]; //!< 名称
	extern const _TCHAR kMNTPriority[]; //!< 整備優先順位
}

/// 参照バージョンマスタ
namespace refversion_master {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kMasterCode[]; //!< マスタコード
	extern const _TCHAR kDataLot[]; //!< データロット
	extern const _TCHAR kWorkName[]; //!< 作業名
	extern const _TCHAR kAddr[]; //!< 住所リリースユーザ
	extern const _TCHAR kAdmin[]; //!< SiNDYDB接続情報
	extern const _TCHAR kVersionCode[]; //!< マスタバージョンコード
}

/// 機能マスタ
namespace function_master {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kIDNCode[]; //!< データ識別コード
	extern const _TCHAR kName[]; //!< 名称
	extern const _TCHAR kSiRiUSTableName[]; //!< SiRiUSテーブル名
	extern const _TCHAR kTableCode[]; //!< 格納テーブルコード
}

/// 機能詳細
namespace function_detail {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kIDNCode[]; //!< データ識別コード
	extern const _TCHAR kSortOrder[]; //!< ソート順
	extern const _TCHAR kColumnName[]; //!< フィールド名
	extern const _TCHAR kSiRiUSColumnName[]; //!< SiRiUSカラム名
}

/// POIポイント
namespace _poi_point {
	extern const _TCHAR kContentsCode[]; //!< コンテンツコード
	extern const _TCHAR kContentsSeq[]; //!< コンテンツ内シーケンス
	extern const _TCHAR kWorker[]; //!< 編集者
	extern const _TCHAR kWorkDate[]; //!< 編集完了日
	extern const _TCHAR kVerifier[]; //!< 検証者
	extern const _TCHAR kVerifyDate[]; //!< 検証完了日
	extern const _TCHAR kUPDAvailDate[]; //!< 更新情報有効日付
	extern const _TCHAR kDelete[]; //!< 削除コード
	extern const _TCHAR kErrorFixed[]; //!< エラー修正フラグ
	extern const _TCHAR kPSTIPCode[]; //!< 座標値提供元コード（情報提供元コード）
	extern const _TCHAR kACCCode[]; //!< 精度コード
	extern const _TCHAR kName[]; //!< 名称
	extern const _TCHAR kYomi[]; //!< （区切り文字付き）読み
	extern const _TCHAR kCloseCode[]; //!< 閉鎖理由コード
	extern const _TCHAR kCloseMemo[]; //!< 閉鎖理由メモ
	extern const _TCHAR kMNTMemoAdmin[]; //!< 整備メモ（管理者）
	extern const _TCHAR kMNTMemoWork[]; //!< 整備メモ（作業者）
	extern const _TCHAR kBGInfo1[]; //!< 参考資料１
	extern const _TCHAR kBGInfo2[]; //!< 参考資料２
	extern const _TCHAR kURL[]; //!< URL
	extern const _TCHAR kIPUseID[]; //!< 情報提供元利用ID
	extern const _TCHAR kFeature[]; //!< [紹介文]紹介文
	extern const _TCHAR kDetailInfo[]; //!< [詳細情報]詳細情報
	extern const _TCHAR kTel[]; //!< [電話番号]電話番号
	extern const _TCHAR kPrefCode[]; //!< [表示用市区町村] 都道府県コード
	extern const _TCHAR kCityCode[]; //!< [表示用市区町村] 市区町村コード
	extern const _TCHAR kAddress[]; //!< [住所]町丁目以下住所
	extern const _TCHAR kLiquor[]; //!< [酒取扱い]酒取扱いフラグ
	extern const _TCHAR kCigar[]; //!< [たばこ取扱い]たばこ取扱いフラグ
	extern const _TCHAR kCRSLM[]; //!< [ランドマーク]交差点拡大時ランドマークフラグ
	extern const _TCHAR kHeightRST[]; //!< [駐車場制限]高さ制限
	extern const _TCHAR kWidthRST[]; //!< [駐車場制限]幅制限
	extern const _TCHAR kLengthRST[]; //!< [駐車場制限]長さ制限
	extern const _TCHAR kWeightRST[]; //!< [駐車場制限]重量制限
	extern const _TCHAR kParkingRSTBit[]; //!< [駐車場制限]駐車場制限ビット（3項目）
	extern const _TCHAR kInfoFileID[]; //!< [映像ファイル]情報ファイルID
	extern const _TCHAR kSLS[]; //!< [販社]販社コード
	extern const _TCHAR kCSID[]; //!< AnyPlace ID
	extern const _TCHAR kInfoID[]; //!< SiNDY-i ID
	extern const _TCHAR kLMID[]; //!< ランドマークID
	extern const _TCHAR kTollRoad[]; //!< [有料道路]有料道路フラグ
	extern const _TCHAR kSerialID[]; //!< シリアルID
	extern const _TCHAR kPartialNameD[]; //!< [部分一致]部分一致名称（区切り文字付き）
	extern const _TCHAR kPartialYomiD[]; //!< [部分一致]部分一致よみ（区切り文字付き）
	extern const _TCHAR kTempleSeq[]; //! [札所]札所番号
	extern const _TCHAR kEmergencyShelterC[]; //!< [緊急避難所]緊急避難所コード

	// 注記用
#ifdef _SINDYK_SUPPORT_COMMON_RELATION // 親子関係のサポート
	extern const _TCHAR kAnnoGUID[]; //!< 注記表示用GUID
#endif // ifdef _SINDYK_SUPPORT_COMMON_RELATION
	extern const _TCHAR kAnnoName1[]; //!< 注記表示用文字列１
	extern const _TCHAR kAnnoName2[]; //!< 注記表示用文字列２
	extern const _TCHAR kAnnoHyoukou[]; //!< 注記表示用標高値
	extern const _TCHAR kAnnoMntMemo[]; //!< 注記整備メモ
	extern const _TCHAR kAnnoDispCode[]; //!< 注記表示コード
	extern const _TCHAR kAnnoDispLevel[]; //!< 注記表示レベル
	extern const _TCHAR kAnnoDispNL[]; //!< 注記改行位置
	extern const _TCHAR kAnnoDispYomi[]; //!< 注記読み
	extern const _TCHAR kEditStatusCode[]; //! 編集状態コード

	/// 削除コード
	namespace delete_code {
		enum ECode {
			kAlive = 0, //!< 生存
			kDead = 1, //!< 削除
			kReserve = 2, //!< 削除保留
		};
	} // deletecode

	/// SiNDY-k機能格納先テーブルコード
	namespace sindyktable_code {
		enum ECode {
			kPoi_point =		1, //!< POI_POINT
			kPoi_sub_point =	2, //!< POI_SUB_POINT
			kPoi_sub_attr =		3, //!< POI_SUB_ATTR
			kPoi_reration =		4, //!< POI_RERATION
		};
	} // sindyktable_code
	
	/// 注記表示コード
	namespace annodisp_code {
		enum ECode {
			kNone = 0, //!< 非表示
			kNomal = 1, //!< 標準
			kForce = 2, //!< 強制表示
			kUnconfirm = 9 //!< 未確認
		};
	} // deletecode

	/// 編集ステータスコード
	namespace editstatus_code {
		enum ECode {
			kNone           = 0, //!< 未編集
			kWorking        = 1, //!< 編集中
			kWorkedNoverify = 2, //!< 編集済み（未検証）
			kVerified      = 3  //!< 検証済み
		};
	} // editstatus_code
}

namespace poi_point {
	extern const _TCHAR kTableName[]; //!< テーブル名
	using namespace _poi_point;
}

namespace poi_point_org {
	extern const _TCHAR kTableName[]; //!< テーブル名
	using namespace _poi_point;
}

/// POIサブポイント
namespace _poi_sub_point {
	extern const _TCHAR kPOIPointID[]; //!< POIポイントID
	extern const _TCHAR kIDNCode[]; //!< データ識別コード
	extern const _TCHAR kFuncSeq[]; //!< 機能内シーケンス
	extern const _TCHAR kDelete[]; //!< 削除コード
	extern const _TCHAR kPSTAttCode[]; //!< [駐車場/駐車場入口/到達]座標値種別コード
	extern const _TCHAR kPriority[]; //!< [駐車場/駐車場入口/到達]座標値種別内優先順位
	extern const _TCHAR kPriorityAttCode[]; //!< [駐車場/駐車場入口/到達]座標値種別内優先順位種別コード
	extern const _TCHAR kMNTInfo[]; //!< [駐車場作業/到達作業]紹介文
	extern const _TCHAR kTollRoad[]; //!< [有料道路]有料道路フラグ

	/// 削除コード
	namespace delete_code {
		enum ECode {
			kAlive = 0, //!< 生存
			kDead = 1, //!< 削除
			kReserve = 2, //!< 削除保留
		};
	} // deletecode
}

namespace poi_sub_point {
	extern const _TCHAR kTableName[]; //!< テーブル名
	using namespace _poi_sub_point;
}

namespace poi_sub_point_org {
	extern const _TCHAR kTableName[]; //!< テーブル名
	using namespace _poi_sub_point;
}

/// POIサブ属性
namespace _poi_sub_attr {
	extern const _TCHAR kPOIPointID[]; //!< POIポイントID
	extern const _TCHAR kIDNCode[]; //!< データ識別コード
	extern const _TCHAR kStartDate[]; //!< [複数営業時間]開始日
	extern const _TCHAR kEndDate[]; //!< [複数営業時間]終了日
	extern const _TCHAR kOpenTime[]; //!< [複数営業時間]営業開始時刻
	extern const _TCHAR kCloseTime[]; //!< [複数営業時間]営業終了時刻
	extern const _TCHAR kDayCode[]; //!< [複数営業時間]曜日コード
	extern const _TCHAR kMultiChainCode[]; //!< [複数店舗]複数店舗コード
}

namespace poi_sub_attr {
	extern const _TCHAR kTableName[]; //!< テーブル名
	using namespace _poi_sub_attr;
}

namespace poi_sub_attr_org {
	extern const _TCHAR kTableName[]; //!< テーブル名
	using namespace _poi_sub_attr;
}

/// POI関連
namespace _poi_relation {
	extern const _TCHAR kRelatedKey[]; //!< 関連キー
	extern const _TCHAR kPOIPointID[]; //!< POIポイントID
	extern const _TCHAR kFuncSeq[]; //!< 機能内シーケンス
	extern const _TCHAR kTmpParkingID[]; //!< [デパート提携駐車場] 提携駐車場・駐車場ID
	extern const _TCHAR kPOICNCATT[]; //!< [POI識別子紐付] POI識別子紐付種別コード
	extern const _TCHAR kParkingID[]; //!< [提携独自駐車場] 独自駐車場ID
}

namespace poi_relation {
	extern const _TCHAR kTableName[]; //!< テーブル名
	using namespace _poi_relation;
}

namespace poi_relation_org {
	extern const _TCHAR kTableName[]; //!< テーブル名
	using namespace _poi_relation;
}

/// 店舗文字列照合辞書
namespace dicchn {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kInitDate[]; //!< 初回登録日
	extern const _TCHAR kDeleteDate[]; //!< 削除日
	extern const _TCHAR kGroupCode[]; //!< データ分類コード
	extern const _TCHAR kChainCode[]; //!< データ店舗コード
	extern const _TCHAR kChkDataSeq[]; //!< チェックデータシーケンス
	extern const _TCHAR kFwdName[]; //!< 名称（前方一致）
	extern const _TCHAR kMidName[]; //!< 名称（中間一致）
	extern const _TCHAR kBwdName[]; //!< 名称（後方一致）
	extern const _TCHAR kFwdYomi[]; //!< 読み（前方一致）
	extern const _TCHAR kMidYomi[]; //!< 読み（中間一致）
	extern const _TCHAR kBwdYomi[]; //!< 読み（後方一致）
} // dicchn

/// シリアルID作成テーブル
namespace create_serial_id {
	extern const _TCHAR kTableName[];	//!< テーブル名
}

/// id_tableとid_addr_tableの共通部分
namespace id_table_base {
	using namespace category::contents_seq_table;
	using namespace category::group_code_table;
	using namespace category::chain_code_table;

	extern const _TCHAR kAddIDType[];	//!< ID付与種別

	/// ID付与種別コード
	namespace add_id_type
	{
		enum ECode
		{
			kCross						= 1,	//!< 交差点
			kRoadType					= 2,	//!< 道路分類
			kRoadLine					= 3,	//!< 道路路線
			kRoadFacilRepPoint			= 4,	//!< 道路施設代表点
			kRoadFacilPoint				= 5,	//!< 道路施設ポイント
			kRoadFacilBranchPoint		= 6,	//!< 道路施設分岐ポイント
			kRailwayType				= 7,	//!< 鉄道分類
			kRailwayCompany				= 8,	//!< 鉄道会社
			kRailwayLine				= 9,	//!< 鉄道路線
			kRailwayStation				= 10,	//!< 鉄道駅
			kRailwayInOut				= 11,	//!< 鉄道駅出入口
			kGeneralPOI					= 12,	//!< 一般POI
			kTownPage					= 14,	//!< タウンページ         // ssmaster.dllで使用するので番号変更しないこと！！
			kBellemax					= 15,	//!< Bellemax             // ssmaster.dllで使用するので番号変更しないこと！！
			kPhoneCodePointOut			= 16,	//!< 局番代表点（市外）   // ssmaster.dllで使用するので番号変更しないこと！！
			kPhoneCodePointCenter		= 17,	//!< 局番代表点（市内）   // ssmaster.dllで使用するので番号変更しないこと！！
			kZipCode					= 18,	//!< 郵便番号
			kPrefecture					= 19,	//!< 都道府県
			kMunicipality				= 20,	//!< 市町村
			kTown						= 21,	//!< 町丁目	// TODO: kTownPageと紛らわしいので変えたい…
			kChiban						= 22,	//!< 地番
			kChibanWithCharacter		= 23,	//!< 文字付地番
			kGou						= 24,	//!< 号
			kGouWithCharacter			= 25,	//!< 文字付号
		};
	}

	/**k
	 * @brief データ分類コード
	 * @note	コンテンツコードの構成要素の一部。「コンテンツコード = データ分類コード * 100000 + 店舗コード」である。
	 *			コード値ドメインではないが、それに類するものなので定義しておく。
	 * @note	分類はID付与種別とほぼ同じだが、下記の点が異なる。
	 *			・「郵便番号」が更に細分化されている。
	 *			・一般POIは可変なので定義なし。
	 *			・タウンページは実際の運用では可変だが、DTMSEQでの管理のため、代表となる値を決めている。
	 */
	namespace group_code
	{
		enum ECode
		{
			kCross						= 22,	//!< 交差点
			kRoadType					= 12,	//!< 道路分類
			kRoadLine					= 13,	//!< 道路路線
			kRoadFacilRepPoint			= 14,	//!< 道路施設代表点
			kRoadFacilPoint				= 15,	//!< 道路施設ポイント
			kRoadFacilBranchPoint		= 16,	//!< 道路施設分岐ポイント
			kRailwayType				= 17,	//!< 鉄道分類
			kRailwayCompany				= 18,	//!< 鉄道会社
			kRailwayLine				= 19,	//!< 鉄道路線
			kRailwayStation				= 20,	//!< 鉄道駅
			kRailwayInOut				= 21,	//!< 鉄道駅出入口
			kTownPage					= 2885,	//!< タウンページ
			kBellemax					= 1,	//!< Bellemax
			kPhoneCodePointOut			= 23,	//!< 局番代表点（市外）
			kPhoneCodePointCenter		= 24,	//!< 局番代表点（市内）
			kPostCode_Address			= 8,	//!< 郵便番号（住所）
			kPostCode_Building			= 9,	//!< 郵便番号（ビル）
			kPostCode_Office			= 10,	//!< 郵便番号（大口事業所）
			kPostCode_PostOfficeBox		= 11,	//!< 郵便番号（私書箱）
			kPrefecture					= 2,	//!< 都道府県
			kMunicipality				= 3,	//!< 市町村
			kTown						= 4,	//!< 町丁目	// TODO: kTownPageと紛らわしいので変えたい…
			kChiban						= 5,	//!< 地番
			kChibanWithCharacter		= 2453,	//!< 文字付地番
			kGou						= 6,	//!< 号
			kGouWithCharacter			= 2454,	//!< 文字付号
		};
	}

} // namespace id_table_base

/// シリアルID対応テーブル
namespace id_table {
	using namespace id_table_base;
	using namespace category::serial_id_table;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kMeshCode[];		//!< ２次メッシュコード
	extern const _TCHAR kX[];				//!< ２次メッシュ内正規化X座標
	extern const _TCHAR kY[];				//!< ２次メッシュ内正規化Y座標
	extern const _TCHAR kCrossName[];		//!< 交差点名称
	extern const _TCHAR kRoadTypeCode[];	//!< 管理団体コード
	extern const _TCHAR kRoadTypeCodeSeq[];	//!< 路線シーケンス番号
	extern const _TCHAR kRoadSeq[];			//!< 路線内シーケンス番号
	extern const _TCHAR kFacilName[];		//!< 道路施設名称
	extern const _TCHAR kFacilAttrCode[];	//!< 道路施設種別コード
	extern const _TCHAR kDirectionCode[];	//!< 方面コード
	extern const _TCHAR kDirectionName[];	//!< 方面名称
	extern const _TCHAR kTelCode[];			//!< 局番
}

/// 住所コンテンツ内シーケンス対応テーブル
namespace id_addr_table
{
	using namespace id_table_base;

	extern const _TCHAR kTableName[];	//!< テーブル名

	extern const _TCHAR kZipCode[];		//!< 郵便番号
	extern const _TCHAR kName[];		//!< 名称
	extern const _TCHAR kPrefCode[];	//!< 都道府県コード
	extern const _TCHAR kCityCode[];	//!< 市区町村コード
	extern const _TCHAR kAdrCode1[];	//!< 大字・通称コード
	extern const _TCHAR kAdrCode2[];	//!< 字・丁目コード
	extern const _TCHAR kChibanCode[];	//!< 地番コード
	extern const _TCHAR kJukyoCode[];	//!< 住居番号コード
}

/// コンテンツ内シーケンス管理テーブル
namespace dtm_seq {
	using namespace category::contents_code_table;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kContentsSeq[];		//!< コンテンツ内シーケンスの最大値
	extern const _TCHAR kLockFlag[];		//!< ロックフラグ
}

/// コンテンツ識別子変換テーブル
namespace contents_convert {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kContentsCode[]; //!< コンテンツコード
	extern const _TCHAR kIPCode[]; //!< IPコード
	extern const _TCHAR kDataKubunCode[]; //!< データ区分コード
	extern const _TCHAR kAttCode[]; //!< 種別コード
	extern const _TCHAR kChainCode8D[]; //!< 店舗コード(8桁)
	using namespace master_history;
}

/// NTT業種マスタ
namespace ntt_master {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kNTTCode[]; //!< NTT業種コード
	extern const _TCHAR kName[]; //!< 名称
	extern const _TCHAR kNTTGroupCode[]; //!< NTT基本分類コード
	extern const _TCHAR kGroupCode[]; //!< データ分類コード
	using namespace master_history;
}

/// データ取得区分マスタ
namespace data_kubun_master {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kDataKubunCode[]; //!< データ取得区分コード
	extern const _TCHAR kName[]; //!< 名称
	using namespace master_history;
}

/// 種別マスタ
namespace assortment_master {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kATTCode[]; //!< 種別コード
	extern const _TCHAR kName[]; //!< 名称
	using namespace master_history;
}

/// 局番変換テーブル
namespace tel_convert {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kTelInfOld[]; //!< 番号領域
	extern const _TCHAR kTelCodeOld[]; //!< 変更前市外局番
	extern const _TCHAR kTelCode[]; //!< 変更後市外局番
	extern const _TCHAR kTelAdd[]; //!< 変更後付加情報
	extern const _TCHAR kDigitOld[]; //!< 変更前番号桁数
	extern const _TCHAR kDigit[]; //!< 変更後番号桁数
	extern const _TCHAR kCngTelF[]; //!< 局番変更対象フラグ
	extern const _TCHAR kTgtArea[]; //!< 対象地域
	extern const _TCHAR kEnforcedDate[]; //!< 実施日付
}

/// 曜日コード変換テーブル
namespace day_convert{
	extern const _TCHAR kTableName[]; //!< テーブル名
	
	extern const _TCHAR kDayCode[]; //!< 曜日コード
	extern const _TCHAR kDayCodeBit[]; //!< 曜日コード（ビット）
	extern const _TCHAR kTermAttCode[]; //!< 期間種別コード
}

/// 鉄道分類変換テーブル
namespace traintype_convert {
	extern const _TCHAR kTableName[]; //!< テーブル名
	
	extern const _TCHAR kGroupCode[]; //!< データ分類コード
	extern const _TCHAR kContentsSeq[]; //!< コンテンツ内シーケンス
	extern const _TCHAR kTrainTypeCode[]; //!< 鉄道分類コード
	extern const _TCHAR kTrainCorpCode[]; //!< 鉄道会社コード
	extern const _TCHAR kTrainTypeCodeOld[]; //!< 鉄道分類コード（旧）
	extern const _TCHAR kTrcDplF[]; //!< 鉄道分類重複フラグ
}

} // sindyk

} // schema

} // sindy
