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

namespace sindy {

namespace schema {

namespace sindyi {

namespace object_locks {
	extern const _TCHAR kTableName[]; //!< テーブル名
};

namespace info_point {
	extern const _TCHAR kTableName[]; //!< テーブル名
};
namespace info_polyline {
	extern const _TCHAR kTableName[]; //!< テーブル名
};
namespace info_polygon {
	extern const _TCHAR kTableName[]; //!< テーブル名
};

namespace shape{
	namespace jump_type{
		enum ECode{
			eCenter = 0,
			eStartPoint = 1,
			eEndPoint = 2,
			eAllView = 3
		};
	}// jump_type
}// shape

namespace info_master {
	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kRegistrant[];		//!< 登録者
	extern const _TCHAR kRegistDate[];		//!< 登録日
	extern const _TCHAR kTitle[];			//!< タイトル
	extern const _TCHAR kInfoType[];		//!< 情報種別
	extern const _TCHAR kDetails[];			//!< 詳細情報
	extern const _TCHAR kMotoID[];			//!< 地図の素ID
	extern const _TCHAR kOldInfoID[];		//!< 旧情報DBID
	extern const _TCHAR kRoadManageID[];	//!< 道路管理ID
	extern const _TCHAR kMipss[];			//!< 旧MIPSSID

	//!< 情報種別コード
	namespace info_type {
		enum ECode {
			kDefault   = 0,		//!< 移行時既存
			kInquiry   = 1,		//!< SiNDY-i調査
			kAdjust    = 2,		//!< SiNDY-i整合
			kMotoiPC   = 7221,	//!< 地図の素（iPC）
			kMotoNEW   = 7309,	//!< 地図の素（新入社員）
			kMotoPEC   = 8102,	//!< 地図の素（PEC）
			kMotoMFW   = 9001,	//!< 地図の素（MFW）
			kMotoINV   = 1060,	//!< 地図の素（INV）
		};
	} // info_type
} // info_master

namespace info_relation {
	extern const _TCHAR kTableName[];	//!< テーブル名

	extern const _TCHAR kRelatedKey[];	//!< 関連キー
	extern const _TCHAR kMasterID[];	//!< マスターID
	extern const _TCHAR kConstSeq[];	//!< 制約順
	extern const _TCHAR kRelatedType[]; //!< 関連詳細コード

	//!< 関連詳細コード
	namespace related_type {
		enum ECode {
			kAdjust = 1,				//!< 編集作業整合
			kSameReference = 2,			//!< 同一問い合わせ先
			kComplexInstitution = 3,	//!< 複合施設
			kDuplication = 4,			//!< 重複
		};
	} // related_type

} // info_relation

namespace info_src {
	extern const _TCHAR kTableName[];	//!< テーブル名

	extern const _TCHAR kMasterID[];	//!< マスターID
	extern const _TCHAR kInfoName[];	//!< 情報源名称
	extern const _TCHAR kInfoSrc[];		//!< 情報源コード
	extern const _TCHAR kInfoURL[];		//!< 情報源URL
	extern const _TCHAR kRegistDate[];	//!< 登録日

	//!< 情報源コード
	namespace infosrc_code {
		enum ECode {
			kDefault = 0,                               //!< -
			kPublicInfomation = 100,                    //!< 公的情報
			kOfficialGazette = 101,                     //!< （公）官報
			kOfficialReport = 102,                      //!< （公）公報
			kPublicRelations = 103,                     //!< （公）広報
			kOfficialDietProceedings = 104,             //!< （公）議会議事録
			kBudget = 105,                              //!< （公）予算
			kAnnual = 106,                              //!< （公）年報
			kCS = 200,                                  //!< CS
			kCSCommonPress = 201,                       //!< （CS）一般紙
			kCSBuildingPaper = 202,                     //!< （CS）建設系新聞
			kCSCommercialFacilitiesPaler = 203,         //!< （CS）商業施設新聞
			kCSSpetializedMagazine = 204,               //!< （CS）専門誌
			kCSUrbanDevelopment = 205,                  //!< （CS）都市開発
			kLocalNews = 300,                           //!< 地域情報ニュース
			kLCommonPress = 301,                        //!< （地）一般紙
			kLSpetializedMagazine = 302,                //!< （地）専門誌
			kLPrefReport = 303,                         //!< （地）県報
			kLOther = 304,                              //!< （地）その他
			kWireReport = 400,                          //!< 配信ニュース
			kWRYahoo = 401,                             //!< （ニ）Yahoo!横断検索
			kWRGoogle = 402,                            //!< （ニ）Googleアラート
			kWebInformation = 500,                      //!< Web
			kWMLITHP = 501,                             //!< （Ｗ）省庁HP
			kWPrefHP = 502,                             //!< （Ｗ）都道府県HP
			kWCityHP = 503,                             //!< （Ｗ）市区町村HP
			kWCompanyHP = 504,                          //!< （Ｗ）企業団体HP
			kWWebNews = 505,                            //!< （Ｗ）ニュースサイト
			kWOtherHP = 506,                            //!< （Ｗ）その他HP
			kWebPatrol = 600,                           //!< Web巡回
			kWPWebNews = 601,                           //!< （Ｗ巡）ニュースサイト
			kWPJapanPost = 603,                         //!< （Ｗ巡）郵政公社
			kWPPrefHP = 604,                            //!< （Ｗ巡）都道府県HP
			kWPCityHP = 605,                            //!< （Ｗ巡）市区町村HP
			kWPCompanyHP = 606,                         //!< （Ｗ巡）企業団体HP
			kWPCommonHP = 607,                          //!< （Ｗ巡）一般HP
            kWPOther = 608,                             //!< （Ｗ巡）その他
			kHatena = 700,                              //!< はてなアンテナ
			kHMLITHP = 701,                             //!< （は）省庁HP
			kHPrefHP = 702,                             //!< （は）都道府県HP
			kHCityHP = 703,                             //!< （は）市区町村HP
			kHCompanyHP = 704,                          //!< （は）企業団体HP
			kHWebNews = 705,                            //!< （は）ニュースサイト
			kHOtherHP = 706,                            //!< （は）その他HP
			kOutHouseInformation = 800,                 //!< 外部提供情報
			kOShopList = 801,                           //!< （外）店舗リスト
			kODRMA = 802,                               //!< （外）DRMA
			kOPasco = 803,                              //!< （外）パスコ
			kOTellInterview = 804,                      //!< （外）取材記録（電話）
			kOVisitInterview = 805,                     //!< （外）取材記録（訪問）
			kODispatchInfomation = 806,                 //!< （外）発信情報
			kOGeographicDataCenter = 807,               //!< （外）国地協
			kOPostCode = 808,                           //!< （外）郵便番号
			kOKajo = 809,                               //!< （外）加除
			kContribution = 900,                        //!< 投稿情報
			kMIPSS = 901,                               //!< （投）MIPSS
			kMoto = 902,                                //!< （投）地図の素
			kInHouseInformation = 1000,                 //!< 社内情報
			kClaim = 1001,                              //!< （社）クレーム
			kFieldInvestigationPicture = 1002,          //!< （社）走行画像
			kNewRoadFieldInvestigationPicture = 1003,   //!< （社）新規道路現調画像
			kOrtho = 1004,                              //!< （社）オルソ
			kFieldInvestigation = 1005,                 //!< （社）現地調査
			kAddrRepresentivePointInvestigation = 1006, //!< （社）住所代表点調査
			kSendingUpInformation = 1100,               //!< 社内情報（打ち上げ）
			kSUMap = 1101,                              //!< （社打）地図打ち上げ
			kSURoad = 1102,                             //!< （社打）道路打ち上げ
			kSUAddr = 1103,                             //!< （社打）住所打ち上げ
			kSUPOI = 1104,                              //!< （社打）POI打ち上げ
			kSUOther = 1105,                            //!< （社打）その他打ち上げ
		};
	} // info_src
} // info_src

namespace info_submaster {
	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kMasterID[];		//!< マスターID
	extern const _TCHAR kEditionNo[];		//!< 版番号
	extern const _TCHAR kLock[];			//!< ロックフラグ
	extern const _TCHAR kValidDate[];		//!< 供用日
	extern const _TCHAR kValidDateCode[];	//!< 供用日コード
	extern const _TCHAR kOwner[];			//!< 調査責任者
	extern const _TCHAR kTimeLimit[];		//!< 調査期限
	extern const _TCHAR kComplete[];		//!< 調査完全F
	extern const _TCHAR kStatus[];			//!< ステータス
	extern const _TCHAR kRank[];			//!< ランク
	extern const _TCHAR kNoOperation[];		//!< 作業不要理由
	extern const _TCHAR kResult[];			//!< 調査結果
	extern const _TCHAR kSurvey[];			//!< 要現地調査
	extern const _TCHAR kPaper[];			//!< 紙資料F
	extern const _TCHAR kShereder[];		//!< 要廃棄資料

	//!< マスターステータスコード
	namespace master_status_type {
		enum ECode {
			kRegist		= 0,  //!< 登録
			kInquiry	= 10, //!< 調査中
			kEarly		= 11, //!< 時期尚早
			kConfirm	= 12, //!< 完了確認待ち
			kComplete	= 20, //!< 完了
			kReInquiry	= 30, //!< 再調査依頼
			kDuplicate	= 50, //!< 重複
		};
	}

	//!< 供用日コード
	namespace validate_type {
		enum ECode {
			kUnknown    = 0,	//!< 未確認
			kBackward	= 1,	//!< 過去
			kInterim	= 2,	//!< 暫定
			kFixed	    = 3,	//!< 確定
		};
	}

	//!< ランク
	namespace rank_type {
		enum ECode {
			kAAA    = 11,	//!< AAA
			kAA	    = 12,	//!< AA
			kA		= 13,	//!< A
			kB	    = 20,	//!< B
			kC	    = 30,	//!< C
			kD	    = 40,	//!< D
		};
	}

	//!< 作業不要理由
	namespace no_operation_type {
		enum ECode {
			kUnknown	= 0,			//!< -
			kReflected	= 1,			//!< 反映済み
			kCourtesyDenied	= 2,		//!< 資料提供拒否
			kCourtesyNone	= 3,		//!< 資料提供なし
			kContactNG	= 4,			//!< 問い合わせNG
			kExcludedFromWork	= 5,	//!< 作業対象外
			kNoSpaces	= 6,			//!< スペースなし
			kSourceUnknown	= 7,		//!< 情報源不詳
			kOnlyRegistered	= 8,		//!< 登録のみ
		};
	}

	//!< 要現地調査
	namespace survey_type {
		enum ECode {
			kUnknown		= 0,	//!< -
			kRequestPlan	= 1,	//!< 依頼予定
			kPreRequest		= 2,	//!< 依頼済み
			kDone			= 3,	//!< 調査済み
			kNoSurvey		= 4,	//!< 調査不可
		};
	}

	//!< 要廃棄資料
	namespace shereder_type {
		enum ECode {
			kNoMaterial   = 0,	//!< 要廃棄資料なし
			kAvailable    = 1,  //!< 要廃棄資料あり
			kPreDisposal  = 2,	//!< 廃棄済み
		};
	}

} // info_submaster

namespace feature_rel {
	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kInfoSubID[];		//!< サブマスターID
	extern const _TCHAR kGeometryID[];		//!< ジオメトリID
	extern const _TCHAR kGeometryType[];	//!< ジオメトリタイプ
	extern const _TCHAR kMeshCode[];		//!< メッシュコード
	extern const _TCHAR kInquiryClass[];	//!< 調査種別
	extern const _TCHAR kNameFixed[];		//!< 確定コード（名称）
	extern const _TCHAR kName[];			//!< 名称等
	extern const _TCHAR kAddressFixed[];	//!< 確定コード（住所）
	extern const _TCHAR kGeometryFixed[];	//!< 確定コード（形状・位置）
	extern const _TCHAR kAddress[];			//!< 住所
	extern const _TCHAR kAddressCode[];		//!< 20桁住所コード
	extern const _TCHAR kMaxFloor[];		//!< 階数情報
	extern const _TCHAR kWideSite[];		//!< 10000㎡フラグ
	extern const _TCHAR kRoadNo[];			//!< 道路番号
	extern const _TCHAR kOpen[];			//!< 開通内容
	extern const _TCHAR kDistance[];		//!< 距離[km]
	extern const _TCHAR kWidth[];			//!< 幅員
	extern const _TCHAR kRefShape[];		//!< 参照形状
	extern const _TCHAR kChangeAccuracy[];	//!< 変更精度
	extern const _TCHAR kDataAccuracy[];	//!< 資料精度
	extern const _TCHAR kDataObtain[];		//!< 取り寄せ元
	extern const _TCHAR kModifyLv[];		//!< 作業レベル
	extern const _TCHAR kFollowup[];		//!< 追跡F
	extern const _TCHAR kPOINotUse[];		//!< POI未採用フラグ
	extern const _TCHAR kPOISequences[];	//!< POIシーケンス
	extern const _TCHAR kMainF[];			//!< 代表フラグ
	extern const _TCHAR kSiteArea[];		//!< 敷地面積
	extern const _TCHAR kSiteAreaCode[];	//!< 敷地面積コード
	extern const _TCHAR kBuildArea[];		//!< 建築面積
	extern const _TCHAR kBuildAreaCode[];	//!< 建築面積コード

	//!< ジオメトリタイプ
	namespace geometry_type {
		enum ECode {
			kUnknown = 0,	//!< 不定
			kPoint = 1,		//!< ポイント
			kLine = 3,		//!< ライン
			kPolygon = 4,	//!< ポリゴン
		};
	}

	//!< 確定コード（名称）
	namespace name_fixed_type {
		enum ECode {
			kUnknown = 0,	//!< -
			kInterim = 1,	//!< 暫定
			kFixed = 2,		//!< 確定
			kDone = 4,		//!< 調査済
		};
	}

	//!< 確定コード（形状・位置）
	namespace geometry_fixed_type {
		enum ECode {
			kUnknown = 0,			//!< -
			kFixedPosition = 1,		//!< 位置確定
			kEndFixedPosition = 2,	//!< 位置確定終了
			kEndFixedShape = 3,		//!< 形状確定終了
			kDone = 4,				//!< 調査済
		};
	}

	//!< 確定コード（住所）
	namespace address_fixed {
		enum ECode {
			kDefault = 0, //初期値
			kInquiry = 1, //調査中
			kFixed   = 2, //確定
			kNoReflect = 3, // 反映不可
		};
	}

	//!< 開通内容
	namespace open_type {
		enum ECode {
			kUnknown = 0,			//!< -
			kOpenRoad = 1,			//!< 開通
			kExtension = 2,			//!< 延伸
			kWidening = 3,			//!< 拡幅
			kOpen = 4,				//!< 開設
			kChange = 5,			//!< 変更
			kLinearImprovement = 6, //!< 線形改良
			kAbolition	 = 7,		//!< 廃止
			kRegulation = 8,		//!< 規制
			kFreeOf = 9,			//!< 無料化
			kNameChange = 10,		//!< 名称変更
			kInduction = 11,		//!< 誘導
			kCorrection = 12,		//!< 修正
		};
	}

	//!< 参照形状
	namespace refshape_type {
		enum ECode {
			kUnknown = 0,				//!< －
			kTopographicMap = 1,		//!< 地形図
			kDRMA = 2,					//!< DRMA
			kCityMap = 3,				//!< 都市地図
			kTotalCapitalFigure = 4,	//!< 都計図
			kPreviousDrawing = 5,		//!< 先行図面
			kConstructionDrawings = 6,	//!< 工事図面
			kDrawingNotice = 7,			//!< 告示図面
			kLocationMap = 8,			//!< 位置図
			kLog = 9,					//!< 走行ログ
			kOrtho  = 10,				//!< オルソ画像
			kOther = 11,				//!< その他
			kUnnecessary = 12,			//!< 不要
			kAddedEdition = 13,			//!< 別版
		};
	}

	//!< 住所精度
	namespace accuracy_type {
		enum ECode {
			kUnknown = 0,				//!< －
			kGou = 1,					//!< 号・地番
			kBlock = 2,					//!< 街区
			kCityChome = 3,				//!< 町丁目
			kPlaceName = 4,				//!< 地名
			kRepresentativePoint = 5,	//!< 代表点
		};
	}

	//!< 取り寄せ元
	namespace obtain_type {
		enum ECode {
			kUnknown = 0,			//!< －
			kMoriokaCoverage = 1,	//!< 盛岡（取材）
			kMoriokaOther = 2,		//!< 盛岡（その他）
			kMeguro = 3,			//!< 目黒
			kKajo = 4,				//!< 加除
		};
	}

	//!< 敷地面積コード
	namespace sitearea_type {
		enum ECode {
			kUnknown = 0,	//!< 未確認
			kInterim = 1,	//!< 暫定
			kFixed = 2,		//!< 確定
		};
	}

	//!< 建築面積コード
	namespace buildarea_type {
		enum ECode {
			kUnknown = 0,	//!< 未確認
			kInterim = 1,	//!< 暫定
			kFixed = 2,		//!< 確定
		};
	}

} // feature_rel

namespace work_target {
	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kInfoSubID[];		//!< サブマスターID
	extern const _TCHAR kEditType[];		//!< 編集タイプ
	extern const _TCHAR kWorkName[];		//!< 作業名コード
	extern const _TCHAR kRegistrant[];		//!< 登録者
	extern const _TCHAR kRegistDate[];		//!< 登録日
	extern const _TCHAR kTimeLimit[];		//!< 対応期限
	extern const _TCHAR kDept[];			//!< 対応部門
	extern const _TCHAR kReflect[];			//!< 反映確認
	extern const _TCHAR kDifficultyLevel[]; //!< 作業難易度

	//!< 編集タイプ
	namespace edit_type {
		enum ECode {
			kMap = 1,	//!< 地図
			kRoad = 2,	//!< 道路
			kAddr = 3,	//!< 住所
			kPOI = 4,	//!< POI
		};
	} // edit_type

	//!< 対応部門
	namespace dept {
		enum ECode {
			kDefault = 0,				//!< -
			kMoto = 21,					//!< 地図の素
			kNewRoad = 22,				//!< 新規道路
			kAdjust = 23,				//!< 整合編集
			kDRMA = 24,					//!< DRMA差分
			kFieldInvestigation = 25,	//!< 新刊更新
			kComplain  = 26,			//!< コンプレイン対応
			kError = 27,				//!< エラー修正
			kSurvey = 28,				//!< 走行現調
			kSurveyNewRoad = 29,		//!< 新規道現調
			kRepairAddr = 31,			//!< 住所整備
		};
	} // dept

	//!< 反映確認
	namespace reflect {
		enum ECode {
			kCancel = 0,			//!< キャンセル
			kCanReflect = 1,		//!< 反映可
			kAlreadyReflected = 2,	//!< 反映済
			kCantReflect = 3,		//!< 反映不可
		};
	} // reflect

	//!< 作業難易度
	namespace difficulty_level {
		enum ECode {
			kLV0 = 0,
			kLV1 = 1,
			kLV2 = 2,
			kLV3 = 3,
			kLV4 = 4,
			kLV5 = 5,
		};
	} // difficulty_level
} // work_target

namespace work_progress {
	extern const _TCHAR kTableName[];	//!< テーブル名

	extern const _TCHAR kTargetID[];	//!< ターゲットID
	extern const _TCHAR kStatus[];		//!< ステータス
	extern const _TCHAR kWorker[];		//!< 作業者
	extern const _TCHAR kStartTime[];	//!< 開始日時
	extern const _TCHAR kEndTime[];		//!< 終了日時
	extern const _TCHAR kComment[];		//!< コメント

	//!< 作業ステータスコード
	namespace work_status {
		enum ECode {
			kOpen      = 0, //!< オープン
			kDraft     = 1, //!< 原稿
			kProof     = 2, //!< 校正
			kEdit      = 3, //!< 編集
			kCheck     = 4, //!< 検査
			kAdjust    = 5, //!< 整合確認
			kClose     = 6, //!< クローズ
			kReInquiry = 7, //!< 再調査確認
		};
	};

} // work_progress

namespace work_add_item {
	extern const _TCHAR kTableName[];	//!< テーブル名

	extern const _TCHAR kProgressID[];	//!< 進捗ID
	extern const _TCHAR kColID[];		//!< 追加項目列
	extern const _TCHAR kColValue[];	//!< 追加項目列値

	//!< SiNDY-i作業不要コード
	namespace no_work {
		enum ECode {
			kWorked = 1, //!< 反映済み
			kNoWork = 2, //!< 反映不可
		};
	} // no_work

	namespace adjust_status{
		enum ECode{
			kOK			= 1,	//!< OK
			kNGMap		= 11,	//!< 地図NG
			kNGRoad		= 12,	//!< 道路NG
			kNGPOI		= 13,	//!< POING
			kNGMapRoad	= 21,	//!< 地図・道路NG
			kNGRoadPOI	= 22,	//!< 道路・POING
			kNGPOIMap	= 23,	//!< POI・地図NG
		};
	}
} // work_add_item

namespace contact_master {
	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kIndustry_Name_C[]; //!< 業種名コード
	extern const _TCHAR kHead_Org[];		//!< 代表名称
	extern const _TCHAR kHead_Yomi[];		//!< 代表ヨミ
	extern const _TCHAR kMain_Num[];		//!< 代表電話番号
	extern const _TCHAR kPost_Code[];		//!< 郵便番号
	extern const _TCHAR kAddress[];			//!< 住所
	extern const _TCHAR kURL[];				//!< URL
	extern const _TCHAR kBiz_Brand[];		//!< 事業ブランド
	extern const _TCHAR kRemarks[];			//!< 備考
	extern const _TCHAR kPerson[];			//!< 社内担当者
	extern const _TCHAR kClose_F[];			//!< 閉鎖フラグ

	namespace industry_name_c {
		enum ECode {
			kUnknown = 0,			//!< -
			kPublicOffice = 1,		//!< 官公庁
			kHospital = 2,			//!< 病院
			kRailway = 3,			//!< 鉄道
			kAviation = 4,			//!< 航空
			kTransportation = 5,	//!< 運輸・旅行
			kAccommodation = 6,		//!< 宿泊施設
			kCommerce = 7,			//!< 商業
			kService = 8,			//!< サービス
			kEntertainment = 9,		//!< エンターテインメント
			kInformation = 10,		//!< 情報・通信
			kFinance = 11,			//!< 金融
			kBuilding = 12,			//!< ビル・住居
			kManufacture = 13,		//!< 製造
			kEnergy = 14,			//!< エネルギー
			kEducation = 15,		//!< 教育・研究機関
			kProduction = 16,		//!< 食料生産
		};
	}

} // contact_master

namespace contact_submaster {
	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kHeadId[];				//!< 代表ID
	extern const _TCHAR kBranch_Org[];			//!< 部署支店名称
	extern const _TCHAR kBranch_Yomi[];			//!< 部署支店ヨミ
	extern const _TCHAR kContact_Person[];		//!< 問合せ先担当者
	extern const _TCHAR kContact_Person_Yomi[];	//!< 問合せ先担当者ヨミ
	extern const _TCHAR kPost_Code[];			//!< 郵便番号
	extern const _TCHAR kAddress[];				//!< 住所
	extern const _TCHAR kPhone[];				//!< 直通電話番号
	extern const _TCHAR kExtension_Num[];		//!< 内線
	extern const _TCHAR kFax[];					//!< FAX番号
	extern const _TCHAR kRemarks[];				//!< 備考
	extern const _TCHAR kPermit_C[];			//!< 許可状況コード
	extern const _TCHAR kRegistrant[];			//!< 登録者
	extern const _TCHAR kRegist_Date[];			//!< 登録日

	//!< 許可状況コード
	namespace permit_c {
		enum ECode {
			kConfirming = 0,				//!< 調査可否確認中
			kInvestigable = 10,				//!< 調査可
			kHeadofficeInquiryContact = 11,	//!< 調査可（担当者固定）
			kUncertainMaterialOffer = 12,	//!< 調査可（資料提供まで可）
			kHeadofficeNG_OutletOK = 13,	//!< 調査可（本社NG/各店舗OK）
			kPoiLicensedCompany = 20,		//!< POI許諾
			kUnauthorized = 30,				//!< 調査不可
		};
	}
} // contact_submaster

namespace inquiry_history {
	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kInfoSubId[];		//!< サブマスターID
	extern const _TCHAR kContactSId[];		//!< 問い合わせ先ID
	extern const _TCHAR kUpdater[];			//!< 更新者
	extern const _TCHAR kUpdate_Date[];		//!< 更新日
	extern const _TCHAR kUpdate_Date_C[];	//!< 更新日コード
	extern const _TCHAR kNext_Date[];		//!< 次回調査日
	extern const _TCHAR kComments[];		//!< コメント

	//!< 更新日コード
	namespace update_date_c {
		enum ECode {
			kUnknown = 1,					//!< －
			kContact = 2,					//!< 問合せ
			kRequest = 3,					//!< 依頼
			kArrival = 4,					//!< 到着
			kChangestatus = 10,				//!< ステータス変更
			kChangeowner = 11,				//!< 調査責任者変更
			kAftercompletingUpdate = 20,	//!< 完了後更新
		};
	}

} // inquiry_history

namespace work_item_registry {
	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kEditType[];		//!< 編集タイプ
	extern const _TCHAR kStatus[];			//!< ステータス
	extern const _TCHAR kDispOrder[];		//!< 表示順
	extern const _TCHAR kColUse[];			//!< 列用途
	extern const _TCHAR kColName[];			//!< 列名称
	extern const _TCHAR kColType[];			//!< 列タイプ
	extern const _TCHAR kColDomain[];		//!< コード値ドメイン名

	namespace edit_type {
		enum ECode {
			kMap = 1,	//!< 地図
			kRoad = 2,	//!< 道路
			kAddr = 3,	//!< 住所
			kPOI = 4,	//!< POI
		};
	}
	namespace work_status {
		enum ECode {
			kOpen      = 0, //!< オープン
			kDraft     = 1, //!< 原稿
			kProof     = 2, //!< 校正
			kEdit      = 3, //!< 編集
			kCheck     = 4, //!< 検査
			kAdjust    = 5, //!< 整合確認
			kClose     = 6, //!< クローズ
			kReInquiry = 7, //!< 再調査確認
		};
	}
	namespace col_use {
		enum ECode {
			kProgress = 1, //!< 進捗入力
			kCheck	  = 2, //!< 検査項目（詳細）
			kNGRegist = 3, //!< NG登録用
		};
	}

	namespace col_type {
		enum ECode {
			kFlag	= 1, //!< フラグ
			kCode	= 2, //!< コード値
			kCount	= 3, //!< 件数 
		};
	}
} // work_item_registry


namespace work_target_master {
	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kWorkCode[]; 		//!< 作業コード
	extern const _TCHAR kWorkName[]; 		//!< 作業名
	extern const _TCHAR kBaseDate[]; 		//!< 基準日
	extern const _TCHAR kRelease_Codes[]; 	//!< リリース番号
	extern const _TCHAR kDispF[]; 			//!< 表示フラグ
	extern const _TCHAR kDataType_C[]; 		//!< データタイプ

	//!< データタイプ
	namespace data_type_c {
		enum ECode {
			kMainstream = 1,		//!< メジャー
			kMonth = 2,				//!< 月度
			kOpen = 3,				//!< 開通日開通
		};
	}

} // work_target_master

namespace work_indication {
	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kInfoSubID[]; 		//!< サブマスターID
	extern const _TCHAR kFromDept[]; 		//!< 送信部門
	extern const _TCHAR kToDept[]; 			//!< 受信部門
	extern const _TCHAR kRelated[]; 		//!< 関連情報
	extern const _TCHAR kIndication[]; 		//!< 作業指示
	extern const _TCHAR kRegistrant[]; 		//!< 登録者
	extern const _TCHAR kRegistDate[]; 		//!< 登録日

	//!< 送信部門
	namespace from_dept {
		enum ECode {
			kMap		= 1,	//!< 地図
			kRoad		= 2,	//!< 道路
			kAddr		= 3,	//!< 住所
			kPOI		= 4,	//!< POI
			kInfoMap	= 10,	//!< 調査(地図)
			kInfoRoad	= 20,	//!< 調査(道路)
			kInfoAddr	= 30,	//!< 調査(住所)
			kInfoPOI	= 40,	//!< 調査(POI)
		};
	}

	//!< 受信部門
	namespace to_dept {
		enum ECode {
			kMap		= 1,	//!< 地図
			kRoad		= 2,	//!< 道路
			kAddr		= 3,	//!< 住所
			kPOI		= 4,	//!< POI
			kInfoMap	= 10,	//!< 調査(地図)
			kInfoRoad	= 20,	//!< 調査(道路)
			kInfoAddr	= 30,	//!< 調査(住所)
			kInfoPOI	= 40,	//!< 調査(POI)
		};
	}

	//!< 関連情報
	namespace related_type {
		enum ECode {
			kUnknown			  = 0,	//!< -
			kCheckRequest		  = 1,	//!< 確認依頼
			kConfirmed_NoSupport  = 2,	//!< 確認済（対応不要）
			kConfirmed_Support    = 3,	//!< 確認済（要対応）
		};
	}

} // work_indication

namespace info_tag {
	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kInfoSubID[]; 		//!< サブマスターID
	extern const _TCHAR kTagName[]; 		//!< タグ

	//!< タグ
	namespace tag_type {
		enum ECode {
			kClaim      = 1,	//!< クレーム
			kWatch      = 2,	//!< WATCH
			kY          = 11,	//!< Y!
			kHwy		= 21,	//!< HWY
			kOpen		= 22,	//!< 開開
			kNewRoad	= 23,	//!< 新規道
		};
	}

} // info_tag

} // sindyi

} // schema

} // sindy
