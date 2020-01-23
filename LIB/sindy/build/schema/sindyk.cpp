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
#include <sindy/schema/sindyk.h>

namespace sindy {

namespace schema {

namespace sindyk {

namespace master_history {
	const _TCHAR kAddMasterCode[] = _T("ADD_MASTER_CODE"); //!< 追加マスタコード
	const _TCHAR kDelMasterCode[] = _T("DEL_MASTER_CODE"); //!< 廃止マスタコード 
}

// データ分類マスタ
namespace group_master {
	const _TCHAR kTableName[] = _T("GROUP_MASTER"); //!< テーブル名

	const _TCHAR kGroupCode[] = _T("GRPCODE"); //!< データ分類コード
	const _TCHAR kName[] = _T("NAME"); //!< 名称
	const _TCHAR kMajorGroupCode[] = _T("MJRGRPCODE"); //!< データ大分類コード
}

// 店舗マスタ
namespace chain_master {
	const _TCHAR kTableName[] = _T("CHAIN_MASTER"); //!< テーブル名

	const _TCHAR kChainCode[] = _T("CHAINCODE"); //!< 店舗コード
	const _TCHAR kChainCode8[] = _T("CHAINCODE_8D"); //!< 店舗コード（8桁）
	const _TCHAR kChainGrpCode[] = _T("CHN_GRPCODE"); //!< 店舗分類コード
	const _TCHAR kName[] = _T("NAME"); //!< 名称
	const _TCHAR kYomi[] = _T("YOMI"); //!< 読み
	const _TCHAR kTwnSet[] = _T("TWN_SET_F"); //!< タウンページ設定対象フラグ
	const _TCHAR kGnrSet[] = _T("GNR_SET_F"); //!< 一般POI設定対象フラグ
}

// コンテンツマスタ
namespace contents_master {
	const _TCHAR kTableName[] = _T("CONTENTS_MASTER"); //!< テーブル名

	const _TCHAR kGroupCode[] = _T("GRPCODE"); //!< データ分類コード
	const _TCHAR kChainCode[] = _T("CHAINCODE"); //!< 店舗コード
	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< コンテンツコード
	const _TCHAR kContentsType[] = _T("CONTENTS_TYPE"); //!< コンテンツタイプ
	const _TCHAR kCATGroupCode[] = _T("CAT_GROUP_CODE"); //!< 大分類グループコード
	const _TCHAR kMNTGroupCode[] = _T("MNT_GROUP_CODE"); //!< 同時メンテグループコード
	const _TCHAR kName[] = _T("NAME"); //!< 読み
	const _TCHAR kInfoFilePath[] = _T("INFOFILE_PATH"); //!< 情報ファイル格納先
	const _TCHAR kInfoFileSeq[] = _T("INFOFILE_SEQ"); //!< 情報ファイルシーケンス
}

// 販社マスタ
namespace sls_master {
	const _TCHAR kTableName[] = _T("SALES_COMPANY_MASTER"); //!< テーブル名

	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< コンテンツコード
	const _TCHAR kSLSCode[] = _T("SLSCODE"); //!< 販社コード
	const _TCHAR kName[] = _T("NAME"); //!< 名称
	const _TCHAR kYomi[] = _T("YOMI"); //!< 読み
}

// コンテンツ機能
namespace contents_function {
	const _TCHAR kTableName[] = _T("CONTENTS_FUNCTION");		 //!< テーブル名

	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< コンテンツコード
	const _TCHAR kIDNCode[] = _T("IDNCODE"); //!< 機能コード
	const _TCHAR kLock[] = _T("LOCK_F"); //!< ロックフラグ
}

// コンテンツ紹介文
namespace contents_introduction {
	const _TCHAR kTableName[] = _T("CONTENTS_INTRODUCTION"); // !< テーブル名

	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< コンテンツコード
	const _TCHAR kFeatureItemSeq[] = _T("FEATURE_ITEM_SEQ"); //!< 紹介文項目シーケンス
	const _TCHAR kFeatureMarkCode[] = _T("FEATURE_MARK_CODE"); //!< 紹介文見出しコード
	const _TCHAR kFeatureMarkName[] = _T("FEATURE_MARK_NAME"); //!< 紹介文見出し名称
	const _TCHAR kUpdate[] = _T("UPDATE_C"); //!< 更新コード
}

// コンテンツ詳細情報
namespace contents_detailinfo {
	const _TCHAR kTableName[] = _T("CONTENTS_DETAILINFO"); // !< テーブル名

	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< コンテンツコード
	const _TCHAR kSortOrder[] = _T("SORT_ORDER"); //!< 並び順
	const _TCHAR kFeatureCode[] = _T("FEATURE_CODE"); //!< 詳細情報項目コード
	const _TCHAR kExist[] = _T("EXIST_F"); //!< ありなしフラグ
}

// 座標値種別マスタ
namespace xy_assortment_convert {
	extern const _TCHAR kTableName[] = _T("XY_ASSORTMENT_CONVERT"); //!< テーブル名

	extern const _TCHAR kPSTAttCode[] = _T("PST_ATTCODE"); //!< 座標値種別コード
	extern const _TCHAR kOrgPSTAttCode[] = _T("ORG_PST_ATTCODE"); //!< 座標値種別コード（派生元）
	extern const _TCHAR kTollRoad[] = _T("TOLLROAD_F"); //! 高速道路フラグ
}

// 座標値種別マスタ
namespace xy_assortment_master {
	extern const _TCHAR kTableName[] = _T("XY_ASSORTMENT_MASTER"); //!< テーブル名

	extern const _TCHAR kPSTAttCode[] = _T("PST_ATTCODE"); //!< 座標値種別コード
	extern const _TCHAR kName[] = _T("NAME"); //!< 名称
	extern const _TCHAR kPSAPriority[] = _T("PSA_PRIORITY"); //!< 座標値種別優先順位
	extern const _TCHAR kMNTPriority[] = _T("MNT_PRIORITY"); //!< 整備優先順位
}

// POI識別子紐付け種別マスタ
namespace poiid_connect_master {
	extern const _TCHAR kTableName[] = _T("POIID_CONNECT_MASTER"); //!< テーブル名
  
	extern _TCHAR kPCACode[] = _T("POI_CNC_ATTCODE"); //!< POI識別子紐付種別コード
	extern _TCHAR kName[] = _T("NAME"); //!< 名称
	extern _TCHAR kBankContentsCode[] = _T("BANK_CONTENTS_CODE"); //!< コンテンツコード（銀行）
	extern _TCHAR kATMContentsCode[] = _T("ATM_CONTENTS_CODE"); //!< コンテンツコード（ATM）
}

// 情報提供元マスタ
namespace infoprovide_master {
	extern const _TCHAR kTableName[] = _T("INFOPROVIDE_MASTER"); //!< テーブル名

	extern const _TCHAR kIPCode[] = _T("IPCODE"); //!< 情報提供元コード
	extern const _TCHAR kName[] = _T("NAME"); //!< 名称
	extern const _TCHAR kMNTPriority[] = _T("MNT_PRIORITY"); //!< 整備優先順位
}

// 精度マスタ
namespace accuracy_master {
	extern const _TCHAR kTableName[] = _T("ACCURACY_MASTER"); //!< テーブル名

	extern const _TCHAR kACCCode[] = _T("ACCCODE"); //!< 精度コード
	extern const _TCHAR kName[] = _T("NAME"); //!< 名称
	extern const _TCHAR kMNTPriority[] = _T("MNT_PRIORITY"); //!< 整備優先順位
}

// 参照バージョンマスタ
namespace refversion_master {
	extern const _TCHAR kTableName[] = _T("REFVERSION_MASTER"); //!< テーブル名

	extern const _TCHAR kMasterCode[] = _T("MASTER_CODE"); //!< マスタコード
	extern const _TCHAR kDataLot[] = _T("DATALOT"); //!< データロット
	extern const _TCHAR kWorkName[] = _T("WORK_NAME"); //!< 作業名
	extern const _TCHAR kAddr[] = _T("ADDR_VERSION"); //!< 住所リリースユーザ
	extern const _TCHAR kAdmin[] = _T("DB_CONNECT_INFO"); //!< SiNDYDB接続情報
	extern const _TCHAR kVersionCode[] = _T("VERSION_CODE"); //!< マスタバージョンコード
}

// 機能マスタ
namespace function_master {
	const _TCHAR kTableName[] = _T("FUNCTION_MASTER"); //!< テーブル名

	const _TCHAR kIDNCode[] = _T("IDNCODE"); //!< データ識別コード
	const _TCHAR kName[] = _T("NAME"); //!< 名称
	const _TCHAR kSiRiUSTableName[] = _T("SIRIUS_TABLE_NAME"); //!< SiRiUSテーブル名
	const _TCHAR kTableCode[] = _T("TABLE_CODE"); //!< 格納テーブルコード
}

// 機能詳細定義
namespace function_detail {
	const _TCHAR kTableName[] = _T("FUNCTION_DETAIL"); //!< テーブル名

	const _TCHAR kIDNCode[] = _T("IDNCODE"); //!< データ識別コード
	const _TCHAR kSortOrder[] = _T("SORT_ORDER"); //!< ソート順
	const _TCHAR kColumnName[] = _T("COLUMN_NAME"); //!< フィールド名
	const _TCHAR kSiRiUSColumnName[] = _T("SIRIUS_COLUMN_NAME"); //!< SiRiUSカラム名
}

// POIポイント
namespace _poi_point {
	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< コンテンツコード
	const _TCHAR kContentsSeq[] = _T("CONTENTS_SEQ"); //!< コンテンツ内シーケンス
	const _TCHAR kWorker[] = _T("WORKER"); //!< 編集者
	const _TCHAR kWorkDate[] = _T("WORK_DATE"); //!< 編集完了日
	const _TCHAR kVerifier[] = _T("VERIFIER"); //!< 検証者
	const _TCHAR kVerifyDate[] = _T("VERIFY_DATE"); //!< 検証完了日
	const _TCHAR kUPDAvailDate[] = _T("UPD_AVAIL_DATE"); //!< 更新情報有効日付
	const _TCHAR kDelete[] = _T("DELETE_C"); //!< 削除コード
	const _TCHAR kErrorFixed[] = _T("ERROR_F"); //!< エラー修正フラグ
	const _TCHAR kPSTIPCode[] = _T("PST_IP_C"); //!< 座標値提供元コード（情報提供元コード）
	const _TCHAR kACCCode[] = _T("ACC_C"); //!< 精度コード
	const _TCHAR kName[] = _T("NAME"); //!< 名称
	const _TCHAR kYomi[] = _T("YOMI_D"); //!< （区切り文字付き）読み
	const _TCHAR kCloseCode[] = _T("CLOSE_C"); //!< 閉鎖理由コード
	const _TCHAR kCloseMemo[] = _T("CLOSE_MEMO"); //!< 閉鎖理由メモ
	const _TCHAR kMNTMemoAdmin[] = _T("MNT_MEMO_ADMIN"); //!< 整備メモ（管理者）
	const _TCHAR kMNTMemoWork[] = _T("MNT_MEMO_WORK"); //!< 整備メモ（作業者）
	const _TCHAR kBGInfo1[] = _T("BG_INFO1"); //!< 参考資料１
	const _TCHAR kBGInfo2[] = _T("BG_INFO2"); //!< 参考資料２
	const _TCHAR kURL[] = _T("URL"); //!< URL
	const _TCHAR kIPUseID[] = _T("IP_USE_ID"); //!< 情報提供元利用ID
	const _TCHAR kFeature[] = _T("FEATURE"); //!< [紹介文]紹介文
	const _TCHAR kDetailInfo[] = _T("DETAILINFO"); //!< [詳細情報]詳細情報
	const _TCHAR kTel[] = _T("TEL"); //!< [電話番号]電話番号
	const _TCHAR kPrefCode[] = _T("PREFCODE"); //!< [表示用市区町村] 都道府県コード
	const _TCHAR kCityCode[] = _T("CITYCODE"); //!< [表示用市区町村] 市区町村コード
	const _TCHAR kAddress[] = _T("ADDRESS"); //!< [住所]町丁目以下住所
	const _TCHAR kLiquor[] = _T("LIQUOR_F"); //!< [酒取扱い]酒取扱いフラグ
	const _TCHAR kCigar[] = _T("CIGAR_F"); //!< [たばこ取扱い]たばこ取扱いフラグ
	const _TCHAR kCRSLM[] = _T("CRS_LM_F"); //!< [ランドマーク]交差点拡大時ランドマークフラグ
	const _TCHAR kHeightRST[] = _T("HEIGHT_RST"); //!< [駐車場制限]高さ制限
	const _TCHAR kWidthRST[] = _T("WIDTH_RST"); //!< [駐車場制限]幅制限
	const _TCHAR kLengthRST[] = _T("LENGTH_RST"); //!< [駐車場制限]長さ制限
	const _TCHAR kWeightRST[] = _T("WEIGHT_RST"); //!< [駐車場制限]重量制限
	const _TCHAR kParkingRSTBit[] = _T("PARKING_RST_B"); //!< [駐車場制限]駐車場制限ビット（3項目）
	const _TCHAR kInfoFileID[] = _T("INFOFILE_ID"); //!< [映像ファイル]情報ファイルID
	const _TCHAR kSLS[] = _T("SLS_C"); //!< [販社]販社コード
	const _TCHAR kCSID[] = _T("CS_ID"); //!< AnyPlace ID
	const _TCHAR kInfoID[] = _T("INFO_ID"); //!< SiNDY-i ID
	const _TCHAR kLMID[] = _T("LM_ID"); //!< ランドマークID
	const _TCHAR kTollRoad[] = _T("TOLLROAD_F"); //!< [有料道路]有料道路フラグ
	// 注記用
#ifdef _SINDYK_SUPPORT_COMMON_RELATION // 親子関係のサポート
	const _TCHAR kAnnoGUID[] = _T("ANNO_DISP_ID"); //!< 注記表示用GUID
#endif // ifdef _SINDYK_SUPPORT_COMMON_RELATION
	const _TCHAR kAnnoName1[] = _T("ANNO_DISP_NAME1"); //!< 注記表示用文字列１
	const _TCHAR kAnnoName2[] = _T("ANNO_DISP_NAME2"); //!< 注記表示用文字列２
	const _TCHAR kAnnoHyoukou[] = _T("ANNO_DISP_HYOUKOU"); //!< 注記表示用標高値
	const _TCHAR kAnnoMntMemo[] = _T("MNT_MEMO_ANNO"); //!< 注記整備メモ
	const _TCHAR kAnnoDispCode[] = _T("ANNO_DISP_C"); //!< 注記表示コード
	const _TCHAR kAnnoDispLevel[] = _T("ANNO_DISP_LEVEL"); //!< 注記表示レベル
	const _TCHAR kAnnoDispNL[] = _T("ANNO_DISP_NL"); //!< 注記改行位置
	const _TCHAR kAnnoDispYomi[] = _T("ANNO_DISP_YOMI"); //!< 注記ヨミ
	const _TCHAR kSerialID[] = _T("SERIAL_ID"); //!< シリアルID
	const _TCHAR kPartialNameD[] = _T("PARTIALNAME_D"); //!< [部分一致]部分一致名称（区切り文字付き）
	const _TCHAR kPartialYomiD[] = _T("PARTIALYOMI_D"); //!< [部分一致]部分一致よみ（区切り文字付き）
	const _TCHAR kTempleSeq[] = _T("TEMPLE_SEQ"); //!< [札所]札所番号
	const _TCHAR kEditStatusCode[] = _T("EDT_STATUS_C"); //! 編集状態コード
	const _TCHAR kEmergencyShelterC[] = _T("EMERGENCY_SHELTER_C"); //!< [緊急避難所]緊急避難所コード
}

namespace poi_point {
	const _TCHAR kTableName[] = _T("POI_POINT"); //!< テーブル名
}

namespace poi_point_org {
	const _TCHAR kTableName[] = _T("POI_POINT_ORG"); //!< テーブル名
}

// POIサブポイント
namespace _poi_sub_point {
	const _TCHAR kPOIPointID[] = _T("POI_POINT_ID"); //!< POIポイントID
	const _TCHAR kIDNCode[] = _T("IDNCODE"); //!< データ識別コード
	const _TCHAR kFuncSeq[] = _T("FUNCTION_SEQ"); //!< 機能内シーケンス
	const _TCHAR kDelete[] = _T("DELETE_C"); //!< 削除コード
	const _TCHAR kPSTAttCode[] = _T("PST_ATT_C"); //!< [駐車場/駐車場入口/到達]座標値種別コード
	const _TCHAR kPriority[] = _T("PRIORITY"); //!< [駐車場/駐車場入口/到達]座標値種別内優先順位
	const _TCHAR kPriorityAttCode[] = _T("PRIORITY_ATT_C"); //!< [駐車場/駐車場入口/到達]座標値種別内優先順位コード
	const _TCHAR kMNTInfo[] = _T("MNT_INFO"); //!< [駐車場作業/到達作業] 整備作業情報
	const _TCHAR kTollRoad[] = _T("TOLLROAD_F"); //!< [有料道路]有料道路フラグ
}

namespace poi_sub_point {
	const _TCHAR kTableName[] = _T("POI_SUB_POINT"); //!< テーブル名
}

namespace poi_sub_point_org {
	const _TCHAR kTableName[] = _T("POI_SUB_POINT_ORG"); //!< テーブル名
}

// POIサブ属性
namespace _poi_sub_attr {
	const _TCHAR kPOIPointID[] = _T("POI_POINT_ID");         //!< POIポイントID
	const _TCHAR kIDNCode[] = _T("IDNCODE");     //!< データ識別コード
	const _TCHAR kStartDate[] = _T("START_DATE"); //!< [複数営業時間]開始日
	const _TCHAR kEndDate[] = _T("END_DATE"); //!< [複数営業時間]終了日
	const _TCHAR kOpenTime[] = _T("OPEN_TIME"); //!< [複数営業時間]営業開始時刻
	const _TCHAR kCloseTime[] = _T("CLOSE_TIME"); //!< [複数営業時間]営業終了時刻
	const _TCHAR kDayCode[] = _T("DAY_C"); //!< [複数営業時間]曜日コード
	const _TCHAR kMultiChainCode[] = _T("MULTI_CHAIN_C"); //!< [複数店舗]複数店舗コード
}

namespace poi_sub_attr {
	const _TCHAR kTableName[] = _T("POI_SUB_ATTR"); //!< テーブル名
}

namespace poi_sub_attr_org {
	const _TCHAR kTableName[] = _T("POI_SUB_ATTR_ORG"); //!< テーブル名
}

// POI関連
namespace _poi_relation {
	const _TCHAR kTableName[] = _T("POI_RELATION"); //!< テーブル名

	const _TCHAR kRelatedKey[] = _T("RELATED_KEY"); //!< 関連キー
	const _TCHAR kPOIPointID[] = _T("POI_POINT_ID"); //!< POIポイントID
	const _TCHAR kFuncSeq[] = _T("FUNCTION_SEQ"); //!< 機能内シーケンス
	const _TCHAR kTmpParkingID[] = _T("TMP_PARKING_ID_D"); //!< [デパート提携駐車場] （区切り文字付き駐車場ID
	const _TCHAR kPOICNCATT[] = _T("POI_CNC_ATT_C"); //!< [POI識別子紐付] POI識別子紐付種別コード
	const _TCHAR kParkingID[] = _T("PARKING_ID"); //!< [提携独自駐車場] 独自駐車場ID
}

namespace poi_relation {
	const _TCHAR kTableName[] = _T("POI_RELATION"); //!< テーブル名
}

namespace poi_relation_org {
	const _TCHAR kTableName[] = _T("POI_RELATION_ORG"); //!< テーブル名
}

/// 店舗文字列照合辞書
namespace dicchn {
	const _TCHAR kTableName[] = _T("DICCHN"); //!< テーブル名

	const _TCHAR kInitDate[] = _T("INIT_DATE"); //!< 初回登録日
	const _TCHAR kDeleteDate[] = _T("DELETE_DATE"); //!< 削除日
	const _TCHAR kGroupCode[] = _T("GRPCODE"); //!< データ分類コード
	const _TCHAR kChainCode[] = _T("CHAINCODE"); //!< データ店舗コード
	const _TCHAR kChkDataSeq[] = _T("CHK_DATA_SEQ"); //!< チェックデータシーケンス
	const _TCHAR kFwdName[] = _T("FWD_NAME"); //!< 名称（前方一致）
	const _TCHAR kMidName[] = _T("MID_NAME"); //!< 名称（中間一致）
	const _TCHAR kBwdName[] = _T("BWD_NAME"); //!< 名称（後方一致）
	const _TCHAR kFwdYomi[] = _T("FWD_YOMI"); //!< 読み（前方一致）
	const _TCHAR kMidYomi[] = _T("MID_YOMI"); //!< 読み（中間一致）
	const _TCHAR kBwdYomi[] = _T("BWD_YOMI"); //!< 読み（後方一致）
} // dicchn

/// シリアルID作成テーブル
namespace create_serial_id {
	const _TCHAR kTableName[] = _T("CREATE_SERIALID");	//!< テーブル名
}

/// id_tableとid_addr_tableの共通部分
namespace id_table_base {
	const _TCHAR kAddIDType[] = _T("ADDIDTYPE_C");	//!< ID付与種別
} // namespace id_table_base

/// シリアルID対応テーブル
namespace id_table {
	using namespace id_table_base;

	const _TCHAR kTableName[] = _T("IDTABLE");		//!< テーブル名

	const _TCHAR kMeshCode[] = _T("MESHCODE");		//!< ２次メッシュコード
	const _TCHAR kX[] = _T("X");				//!< ２次メッシュ内正規化X座標
	const _TCHAR kY[] = _T("Y");				//!< ２次メッシュ内正規化Y座標
	const _TCHAR kCrossName[] = _T("CRS_NAME");		//!< 交差点名称
	const _TCHAR kRoadTypeCode[] = _T("ROADTYPECODE");	//!< 管理団体コード
	const _TCHAR kRoadTypeCodeSeq[] = _T("ROADTYPECODE_SEQ");	//!< 路線シーケンス番号
	const _TCHAR kRoadSeq[] = _T("ROAD_SEQ");			//!< 路線内シーケンス番号
	const _TCHAR kFacilName[] = _T("FCL_NAME");		//!< 道路施設名称
	const _TCHAR kFacilAttrCode[] = _T("FCL_ATTCODE");	//!< 道路施設種別コード
	const _TCHAR kDirectionCode[] = _T("DIRECTIONCODE");	//!< 方面コード
	const _TCHAR kDirectionName[] = _T("DIRECTIONNAME");	//!< 方面名称
	const _TCHAR kTelCode[] = _T("TEL_CODE");			//!< 局番
}

/// 住所コンテンツ内シーケンス対応テーブル
namespace id_addr_table
{
	using namespace id_table_base;

	const _TCHAR kTableName[] = _T("IDADDRTABLE");	//!< テーブル名

	const _TCHAR kZipCode[] = _T("ZIPCODE");		//!< 郵便番号
	const _TCHAR kName[] = _T("NAME");		//!< 名称
	const _TCHAR kPrefCode[] = _T("PREFCODE");	//!< 都道府県コード
	const _TCHAR kCityCode[] = _T("CITYCODE");	//!< 市区町村コード
	const _TCHAR kAdrCode1[] = _T("ADRCODE1");	//!< 大字・通称コード
	const _TCHAR kAdrCode2[] = _T("ADRCODE2");	//!< 字・丁目コード
	const _TCHAR kChibanCode[] = _T("CHIBANCODE");	//!< 地番コード
	const _TCHAR kJukyoCode[] = _T("JUKYOCODE");	//!< 住居番号コード
}

/// コンテンツ内シーケンス管理テーブル
namespace dtm_seq {
	const _TCHAR kTableName[] = _T("DTMSEQ");		//!< テーブル名

	const _TCHAR kContentsSeq[] = _T("CONTENTS_SEQ");		//!< コンテンツ内シーケンスの最大値
	const _TCHAR kLockFlag[] = _T("LOCK_F");		//!< ロックフラグ
}

/// コンテンツ識別子変換テーブル
namespace contents_convert {
	const _TCHAR kTableName[] = _T("CONTENTS_CONVERT"); //!< テーブル名

	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< コンテンツコード
	const _TCHAR kIPCode[] = _T("IPCODE"); //!< IPコード
	const _TCHAR kDataKubunCode[] = _T("DATA_KUBUN_CODE"); //!< データ区分コード
	const _TCHAR kAttCode[] = _T("ATTCODE"); //!< 種別コード
	const _TCHAR kChainCode8D[] = _T("CHAINCODE_8D"); //!< 店舗コード(8桁)
}

/// NTT業種マスタ
namespace ntt_master {
	const _TCHAR kTableName[] = _T("NTT_MASTER"); //!< テーブル名

	const _TCHAR kNTTCode[] = _T("NTTCODE"); //!< NTT業種コード
	const _TCHAR kName[] = _T("NAME"); //!< 名称
	const _TCHAR kNTTGroupCode[] = _T("NTTGRPCODE"); //!< NTT基本分類コード
	const _TCHAR kGroupCode[] = _T("GRPCODE"); //!< データ分類コード
}

/// データ取得区分マスタ
namespace data_kubun_master {
	const _TCHAR kTableName[] = _T("DATA_KUBUN_MASTER"); //!< テーブル名

	const _TCHAR kDataKubunCode[] = _T("DATA_KUBUN_CODE");; //!< データ取得区分コード
	const _TCHAR kName[] = _T("NAME"); //!< 名称
}

/// 種別マスタ
namespace assortment_master {
	const _TCHAR kTableName[] = _T("ASSORTMENT_MASTER"); //!< テーブル名

	const _TCHAR kATTCode[] = _T("ATTCODE"); //!< 種別コード
	const _TCHAR kName[] = _T("NAME"); //!< 名称
}

/// 局番変換テーブル
namespace tel_convert {
	const _TCHAR kTableName[] = _T("TEL_CONVERT"); //!< テーブル名

	const _TCHAR kTelInfOld[] = _T("TEL_INF_OLD"); //!< 番号領域
	const _TCHAR kTelCodeOld[] = _T("TELCODE_OLD"); //!< 変更前市外局番
	const _TCHAR kTelCode[] = _T("TELCODE");//!< 変更後市外局番
	const _TCHAR kTelAdd[] = _T("TEL_ADD"); //!< 変更後付加情報
	const _TCHAR kDigitOld[] = _T("DIGIT_OLD"); //!< 変更前番号桁数
	const _TCHAR kDigit[] = _T("DIGIT"); //!< 変更後番号桁数
	const _TCHAR kCngTelF[] = _T("CNG_TEL_F"); //!< 局番変更対象フラグ
	const _TCHAR kTgtArea[] = _T("TGT_AREA"); //!< 対象地域
	const _TCHAR kEnforcedDate[] = _T("ENFORCED_DATE"); //!< 実施日付
}

/// 曜日コード変換テーブル
namespace day_convert{
	const _TCHAR kTableName[] = _T("DAY_CONVERT"); //!< テーブル名
	
	const _TCHAR kDayCode[] = _T("DAYCODE"); //!< 曜日コード
	const _TCHAR kDayCodeBit[] = _T("DAYCODE_BIT"); //!< 曜日コード（ビット）
	const _TCHAR kTermAttCode[] = _T("TERM_ATTCODE"); //!< 期間種別コード
}

/// 鉄道分類変換テーブル
namespace traintype_convert{
	const _TCHAR kTableName[] = _T("TRAINTYPE_CONVERT"); //!< テーブル名
	
	const _TCHAR kGroupCode[] = _T("GRPCODE"); //!< データ分類コード
	const _TCHAR kContentsSeq[] = _T("CONTENTS_SEQ"); //!< コンテンツ内シーケンス
	const _TCHAR kTrainTypeCode[] = _T("TRAINTYPECODE"); //!< 鉄道分類コード
	const _TCHAR kTrainCorpCode[] = _T("TRAINCORPCODE"); //!< 鉄道会社コード
	const _TCHAR kTrainTypeCodeOld[] = _T("TRAINTYPECODE_OLD"); //!< 鉄道分類コード（旧）
	const _TCHAR kTrcDplF[] = _T("TRC_DPL_F"); //!< 鉄道分類重複フラグ
}

} // sindyk

} // schema

} // sindy
