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

#ifndef SINDY_SCHEMA_ILLUST_H_
#define SINDY_SCHEMA_ILLUST_H_

#include <iostream>
#include <sindy/schema/common.h>
#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

/// イラストリンクフィーチャクラス
namespace illust_link {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kIllustName[];            //!< 画像ファイル名(ファイル名を分割してSiNDY上に入れるので、SiNDY上にはフィールドが存在しない)
	extern const _TCHAR kIllustClass[];           //!< イラスト種別
	extern const _TCHAR kCustomerCode[];          //!< 仕向け先コード
	extern const _TCHAR kRoadCode[];              //!< 路線番号
	extern const _TCHAR kRoadName[];              //!< 路線名称
	extern const _TCHAR kRoadSubName[];           //!< 路線副名称
	extern const _TCHAR kRoadSeq[];               //!< 路線内シーケンス
	extern const _TCHAR kFacilName[];             //!< 施設名称
	extern const _TCHAR kFacilSubName[];          //!< 施設副名称
	extern const _TCHAR kIdentificationName[];    //!< 識別用名称
	extern const _TCHAR kGuideRoute[];            //!< 方向
	extern const _TCHAR kUpDownC[];               //!< 上下線コード
	extern const _TCHAR kGuideSeq[];              //!< 案内シーケンス
	extern const _TCHAR kSuccessiveCode[];        //!< 連続分岐コード
	extern const _TCHAR kSingleBranchCode[];      //!< 単分岐コード
	extern const _TCHAR kIrregularCode[];         //!< イレギュラー分岐コード
	extern const _TCHAR kTunnelCode[];            //!< トンネルコード
	extern const _TCHAR kStraightCode[];          //!< 直進方向コード
	extern const _TCHAR kBoundaryCode[];          //!< 白線境界コード
	extern const _TCHAR kBranchDirectionCode[];   //!< 分岐方向コード
	extern const _TCHAR kLaneBranchPatternCode[]; //!< 車線分岐パターンコード
	extern const _TCHAR kDirguideExistCode[];     //!< 方面案内看板有無コード
	extern const _TCHAR kDirectionCode[];         //!< 案内方向コード

	extern const _TCHAR kBranch1PatternCode[];           //!< 分岐1_パターンコード
	extern const _TCHAR kBranch1LaneCountMainBefore[];   //!< 分岐1_本線変化前車線数
	extern const _TCHAR kBranch1LaneCountMainAfter[];    //!< 分岐1_本線変化後車線数
	extern const _TCHAR kBranch1LaneCountLeftBefore[];   //!< 分岐1_左分岐変化前車線数
	extern const _TCHAR kBranch1LaneCountLeftAfter[];    //!< 分岐1_左分岐変化後車線数
	extern const _TCHAR kBranch1LaneCountCenterBefore[]; //!< 分岐1_中央分岐変化前車線数
	extern const _TCHAR kBranch1LaneCountCenterAfter[];  //!< 分岐1_中央分岐変化後車線数
	extern const _TCHAR kBranch1LaneCountRightBefore[];  //!< 分岐1_右分岐変化前車線数
	extern const _TCHAR kBranch1LaneCountRightAfter[];   //!< 分岐1_右分岐変化後車線数
	extern const _TCHAR kBranch2PatternCode[];           //!< 分岐2_パターンコード
	extern const _TCHAR kBranch2LaneCountMainBefore[];   //!< 分岐2_本線変化前車線数
	extern const _TCHAR kBranch2LaneCountMainAfter[];    //!< 分岐2_本線変化後車線数
	extern const _TCHAR kBranch2LaneCountLeftBefore[];   //!< 分岐2_左分岐変化前車線数
	extern const _TCHAR kBranch2LaneCountLeftAfter[];    //!< 分岐2_左分岐変化後車線数
	extern const _TCHAR kBranch2LaneCountCenterBefore[]; //!< 分岐2_中央分岐変化前車線数
	extern const _TCHAR kBranch2LaneCountCenterAfter[];  //!< 分岐2_中央分岐変化後車線数
	extern const _TCHAR kBranch2LaneCountRightBefore[];  //!< 分岐2_右分岐変化前車線数
	extern const _TCHAR kBranch2LaneCountRightAfter[];   //!< 分岐2_右分岐変化後車線数
	extern const _TCHAR kBranch3PatternCode[];           //!< 分岐3_パターンコード
	extern const _TCHAR kBranch3LaneCountMainBefore[];   //!< 分岐3_本線変化前車線数
	extern const _TCHAR kBranch3LaneCountMainAfter[];    //!< 分岐3_本線変化後車線数
	extern const _TCHAR kBranch3LaneCountLeftBefore[];   //!< 分岐3_左分岐変化前車線数
	extern const _TCHAR kBranch3LaneCountLeftAfter[];    //!< 分岐3_左分岐変化後車線数
	extern const _TCHAR kBranch3LaneCountCenterBefore[]; //!< 分岐3_中央分岐変化前車線数
	extern const _TCHAR kBranch3LaneCountCenterAfter[];  //!< 分岐3_中央分岐変化後車線数
	extern const _TCHAR kBranch3LaneCountRightBefore[];  //!< 分岐3_右分岐変化前車線数
	extern const _TCHAR kBranch3LaneCountRightAfter[];   //!< 分岐3_右分岐変化後車線数
	extern const _TCHAR kBranch4PatternCode[];           //!< 分岐4_パターンコード
	extern const _TCHAR kBranch4LaneCountMainBefore[];   //!< 分岐4_本線変化前車線数
	extern const _TCHAR kBranch4LaneCountMainAfter[];    //!< 分岐4_本線変化後車線数
	extern const _TCHAR kBranch4LaneCountLeftBefore[];   //!< 分岐4_左分岐変化前車線数
	extern const _TCHAR kBranch4LaneCountLeftAfter[];    //!< 分岐4_左分岐変化後車線数
	extern const _TCHAR kBranch4LaneCountCenterBefore[]; //!< 分岐4_中央分岐変化前車線数
	extern const _TCHAR kBranch4LaneCountCenterAfter[];  //!< 分岐4_中央分岐変化後車線数
	extern const _TCHAR kBranch4LaneCountRightBefore[];  //!< 分岐4_右分岐変化前車線数
	extern const _TCHAR kBranch4LaneCountRightAfter[];   //!< 分岐4_右分岐変化後車線数

	extern const _TCHAR kCrossRoadFront_A[];             //!< 接続道路_手前_A
	extern const _TCHAR kCrossRoadFront_B[];             //!< 接続道路_手前_B
	extern const _TCHAR kCrossRoadFront_C[];             //!< 接続道路_手前_C
	extern const _TCHAR kCrossRoadFront_D[];             //!< 接続道路_手前_D
	extern const _TCHAR kCrossRoadFront_E[];             //!< 接続道路_手前_E
	extern const _TCHAR kCrossRoadFront_F[];             //!< 接続道路_手前_F
	extern const _TCHAR kCrossRoadMiddle_A[];            //!< 接続道路_中間_A
	extern const _TCHAR kCrossRoadMiddle_B[];            //!< 接続道路_中間_B
	extern const _TCHAR kCrossRoadMiddle_C[];            //!< 接続道路_中間_C
	extern const _TCHAR kCrossRoadMiddle_D[];            //!< 接続道路_中間_D
	extern const _TCHAR kCrossRoadMiddle_E[];            //!< 接続道路_中間_E
	extern const _TCHAR kCrossRoadMiddle_F[];            //!< 接続道路_中間_F
	extern const _TCHAR kCrossRoadAhead_A[];             //!< 接続道路_先_A
	extern const _TCHAR kCrossRoadAhead_B[];             //!< 接続道路_先_B
	extern const _TCHAR kCrossRoadAhead_C[];             //!< 接続道路_先_C
	extern const _TCHAR kCrossRoadAhead_D[];             //!< 接続道路_先_D
	extern const _TCHAR kCrossRoadAhead_E[];             //!< 接続道路_先_E
	extern const _TCHAR kCrossRoadAhead_F[];             //!< 接続道路_先_F
	extern const _TCHAR kCrossRoadAhead_G[];             //!< 接続道路_先_G

	extern const int    kMaxNodeNumber; // 最大ノード数
	extern const _TCHAR kNode0[];   ///< 道路ノード0(使わないが便宜上)
	extern const _TCHAR kNode1[];   ///< 道路ノード1
	extern const _TCHAR kNode2[];   ///< 道路ノード2
	extern const _TCHAR kNode3[];   ///< 道路ノード3
	extern const _TCHAR kNode4[];   ///< 道路ノード4
	extern const _TCHAR kNode5[];   ///< 道路ノード5
	extern const _TCHAR kNode6[];   ///< 道路ノード6
	extern const _TCHAR kNode7[];   ///< 道路ノード7
	extern const _TCHAR kNode8[];   ///< 道路ノード8
	extern const _TCHAR kNode9[];   ///< 道路ノード9
	extern const _TCHAR kNode10[];  ///< 道路ノード10
	extern const _TCHAR kNode11[];  ///< 道路ノード11
	extern const _TCHAR kNode12[];  ///< 道路ノード12
	extern const _TCHAR kNode13[];  ///< 道路ノード13
	extern const _TCHAR kNode14[];  ///< 道路ノード14
	extern const _TCHAR kNode15[];  ///< 道路ノード15
	extern const _TCHAR kNode16[];  ///< 道路ノード16
	extern const _TCHAR kNode17[];  ///< 道路ノード17
	extern const _TCHAR kNode18[];  ///< 道路ノード18
	extern const _TCHAR kNode19[];  ///< 道路ノード19
	extern const _TCHAR kNode20[];  ///< 道路ノード20

	extern const LPCTSTR kNode[21]; ///< 道路ノード一覧

	extern const _TCHAR kProvisional[];   //!< 暫定整備フラグ

	// イラスト仕向け先コード
	namespace customer {
		enum ECode {
			kUnset         = 0,   //!< 未設定
			kCommon        = 1,   //!< 共通
			kTF            = 2,   //!< TF
			kExternalSale  = 3,   //!< 外販
			kTR            = 4,   //!< TR
		};
	} // illust_customer

	// イラスト種別コード
	namespace illust_class {
		enum ECode {
			kSAPA          = 110, //!< SAPA
			kETC           = 120, //!< ETC（本線）
			kETC2          = 121, //!< ETC（出口）
			kProperJCT     = 130, //!< 固有JCT
			kBranch        = 140, //!< 高速分岐（都市）
			kBranch2       = 141, //!< 高速分岐（都市間）
			kEntrance      = 150, //!< 高速入口分岐
			kExit          = 151, //!< 高速出口分岐
			kInIC          = 210, //!< 高速入口
			kAirport       = 220, //!< 空港
			kRealCross     = 230, //!< 複雑交差点
			kGrade         = 240, //!< 側道分岐
		};
	} // illust_code
	
	// イラスト上下線コード
	namespace updown_code {
		enum ECode {
			kUnset         = 0,   //!< 未設定
			kUp            = 1,   //!< 上り
			kDown          = 2,   //!< 下り
			kUpDown        = 3,   //!< 上下線共有
		};
	} // updown_code

	// イラスト連続分岐コード
	namespace successive_code {
		enum ECode {
			kUnset              = 0,   //!< 未設定
			kThreeWayBranch     = 3,   //!< 3分岐
			kFourWayBranch      = 4,   //!< 4分岐
			kNotExistSuccessive = 9,   //!< 連続分岐なし
		};
	} // successive_code

	// イラスト単分岐コード
	namespace single_branch_code {
		enum ECode {
			kUnset                 = 0,   //!< 未設定
			kSingleBranchDesign    = 1,   //!< 単分岐デザイン
			kNonSingleBranchDesign = 2,   //!< 非単分岐デザイン
		};
	} // single_branch_code

	// イラストイレギュラー分岐コード
	namespace irregular_code {
		enum ECode {
			kUnset           = 0,   //!< 未設定
			kIrregularBranch = 1,   //!< イレギュラー分岐
			kCommonBranch    = 2,   //!< 通常分岐
		};
	} // irregular_code

	// イラストトンネルコード
	namespace tunnel_code {
		enum ECode {
			kUnset            = 0,   //!< 未設定
			kExistTunnel      = 1,   //!< トンネルあり
			kNotExistTunnel   = 2,   //!< トンネルなし
		};
	} // tunnel_code

	// イラスト直進方向コード
	namespace straight_code {
		enum ECode {
			kUnset            = 0,   //!< 未設定
			kStraight         = 1,   //!< 直進方向
			kBranch           = 2,   //!< 分岐方向
		};
	} // straight_code

	// イラスト白線境界なしコード
	namespace boundary_code {
		enum ECode {
			kUnset            = 0,   //!< 未設定
			kExistBoundary    = 1,   //!< 白線境界あり
			kNotExistBoundary = 2,   //!< 白線境界なし
		};
	} // boundary_code

	// イラスト分岐方向コード
	namespace branch_dir_code {
		enum ECode {
			kUnset         = 0,   //!< 未設定
			kLeft          = 1,   //!< 左
			kRight         = 2,   //!< 右
		};
	} // branch_dir_code

	// イラスト車線分岐パターンコード
	namespace lane_brn_pattern_code {
		enum ECode {
			kUnset             = 0,   //!< 未設定
			kExistLaneBranch   = 1,   //!< 車線分岐あり
			kExistFrontageJoin = 2,   //!< 側道合流あり
		};
	} // lane_brn_pattern_code

	// イラスト方面案内看板有無コード
	namespace dir_guide_exist_code {
		enum ECode {
			kUnset         = 0,   //!< 未設定
			kLX            = 1,   //!< 左
			kXR            = 2,   //!< 右
			kLR            = 3,   //!< 左右
			kXX            = 9,   //!< なし
		};
	} // dir_guide_exist_code

	// イラスト案内方向コード
	namespace direction_code {
		enum ECode {
			kUnset         = 0,   //!< 未設定
			kA             = 1,   //!< A
			kB             = 2,   //!< B
			kC             = 3,   //!< C
			kD             = 4,   //!< D
			kE             = 5,   //!< E
			kF             = 6,   //!< F
			kG             = 7,   //!< G
			kH             = 8,   //!< H
			kI             = 9,   //!< I
			kJ             = 10,  //!< J
			kK             = 11,  //!< K
			kL             = 12,  //!< L
			kM             = 13,  //!< M
			kN             = 14,  //!< N
			kO             = 15,  //!< O
		};
	} // direction_code

	// イラスト分岐パターンコード
	namespace branch_pattern_code {
		enum ECode {
			kUnset         = 0,   //!< 未設定
			kS             = 101, //!< 左右分岐
			kY             = 102, //!< Y字分岐
			kN             = 103, //!< 中央Y字分岐
			kC             = 104, //!< 連続分岐
			kO1            = 201, //!< 本線オーバーパス・側道水平
			kU1            = 202, //!< 本線アンダーパス・側道水平
			kO2            = 203, //!< 本線水平・側道アンダーパス
			kU2            = 204, //!< 本線水平・側道オーバーパス
			kF1            = 205, //!< 本線水平・側道水平
		};
	} // branch_pattern_code

	// イラスト接続道路有無コード
	namespace cross_road_code {
		enum ECode {
			kUnset             = 0,   //!< 未設定
			kExistCrossRoad    = 1,   //!< 接続道路あり
			kNotExistCrossRoad = 2,   //!< 接続道路なし
		};
	} // cross_road_code

} // illust_link

} // schema

} // sindy

#endif // SINDY_SCHEMA_ILLUST_H_
