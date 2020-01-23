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

/**
 * @file RoadCommunity.h
 * @brief 道路テーブルフィールド・ドメイン定義
 * @author 
 * $Id$
 */
#ifndef _ROADCOMMUNITY_H_
#define _ROADCOMMUNITY_H_

namespace sindy {
	/**
	 * \namespace sindy::direction_code
	 * @brief ハイウェイ方向コード用ネームスペース
	 */	
	namespace direction_code {
		/**
		 * \enum sindy::direction_code::ECode
		 * @brief ハイウェイ方向コードドメイン
		 */
		enum ECode
		{
			kNoDirection,			//!< 方向なし
			kUpInnerWest,			//!< 上り・内回り・西行き
			kDownOuterEast,			//!< 下り・外回り・東行き
			kBoth					//!< 双方向共通
		};
	}

	/**
	 * \namespace sindy::inout_code
	 * @brief ハイウェイINOUTコード用ネームスペース
	 */	
	namespace inout_code {
		/**
		 * \enum sindy::inout_code::ECode
		 * @brief ハイウェイINOUTコードドメイン
		 */
		enum ECode
		{
			kNoDistinction,			//!< 区別なし
			kEntranceConfluence,	//!< 入口・合流
			kExitDivergence			//!< 出口・分岐
		};
	}

	/**
	 * \namespace sindy::gs_code
	 * @brief ガソリンスタンド識別コード用ネームスペース
	 */	
	namespace gs_code {
		/**
		 * \enum sindy::gs_code::ECode
		 * @brief ガソリンスタンド識別コードドメイン
		 */
		enum ECode
		{
			kNoGS,					//!< GS無し
			kOther,					//!< その他・未定
			kiEsso,					//!< エッソ
			kMobil,					//!< モービル
			kKygnus,				//!< キグナス
			kCosmo,					//!< コスモ
			kShell,					//!< シェル
			kJomo,					//!< ジョモ
			kGeneral,				//!< ゼネラル
			kStork,					//!< 九州
			kIdemitsu,				//!< 出光
			kTaiyo,					//!< 太陽
			kEneos					//!< ENEOS
		};
	}

	/**
	 * \namespace sindy::facilclass_code
	 * @brief Highway施設種別コード用ネームスペース
	 */	
	namespace facilclass_code {
		/**
		 * \enum sindy::facilclass_code::ECode
		 * @brief Highway施設種別コードドメイン
		 */
		enum ECode
		{
			kSA,					//!< SA
			kPA,					//!< PA
			kJCT,					//!< JCT
			kRamp,					//!< ランプ
			kIC,					//!< IC
			kTollgate				//!< 料金所
		};
	}

	/**
	 * \namespace sindy::tollclass_code
	 * @brief 料金所種別コード用ネームスペース
	 */	
	namespace tollclass_code {
		/**
		 * \enum sindy::tollclass_code::ECode
		 * @brief 料金所種別コードドメイン
		 */
		enum ECode
		{
			kOther,					//!< 料金所以外
			kMainTollgate,			//!< 本線上料金所
			kRampTollgate,			//!< ランプ上料金所
			kBarrier				//!< バリア
		};
	}

	/**
	 * \namespace sindy::tollfunc_code
	 * @brief 料金所機能コード2用ネームスペース
	 */	
	namespace tollfunc_code {
		/**
		 * \enum sindy::tollfunc_code::ECode
		 * @brief 料金所機能コード2ドメイン
		 */
		enum ECode
		{
			kNoFunction,							//!< 機能無し
			kTicketIssue,							//!< 発券機能
			kAdjustment,							//!< 精算機能
			kStandalone,							//!< 単独料金所
			kStandaloneWithTicketIssue,				//!< 単独料金所＋発券機能
			kStandaloneWithAdjustment,				//!< 単独料金所＋精算機能
			kUTurnTicketInspectionIssue,			//!< Uターン用検札機能
			kStandaloneChargeInvalidTicketIssue,	//!< 単独料金無効発券機能
		};
	}

	/**
	 * \namespace sindy::roadclass_code
	 * @brief 道路種別コード3用ネームスペース
	 */	
	namespace roadclass_code {
		/**
		 * \enum sindy::roadclass_code::ECode
		 * @brief 道路種別コード3ドメイン
		 */
		enum ECode
		{
			kNoDisplayKind = 0,				//!< 表示種別なし
			kExpressway = 101,				//!< 高速自動車道路
			kCityExpressway = 102,			//!< 都市高速道路
			kNational = 1,					//!< 一般国道
			kTurnpike = 103,				//!< 一般国道（有料）
			kMainLocalPrefRoad = 2,			//!< 主要地方道（都府県）
			kMainLocalPrefTollRoad = 104,	//!< 主要地方道（都府県）（有料）
			kMainLocalRoad = 3,				//!< 主要地方道（指定市）
			kMainLocalTollRoad = 105,		//!< 主要地方道（指定市）（有料）
			kPrefecturesRoad = 4,			//!< 一般都道府県道
			kPrefecturesTollRoad = 106,		//!< 一般都道府県道（有料）
			kCityRoad = 5,					//!< 指定市の一般市道
			kCityTollRoad = 107,			//!< 指定市の一般市道（有料）
			kOtherRoad = 6,					//!< その他道路
			kOtherTollRoad = 108,			//!< その他道路（有料）
			kOtherCullRoad = 7,				//!< その他道路（間引対象）
			kNarrowPathL1 = 8,				//!< 細街路L1
			kNarrowPathL2 = 9,				//!< 細街路L2
			kNarrowPathL3 = 10,				//!< 細街路L3
			kFerryLineS1 = 201,				//!< フェリー航路(非表示部)
			kFerryLineS2 = 202,				//!< フェリー航路(S2破線表示部)
			kFerryLineS3 = 203,				//!< フェリー航路(S3破線表示部)
			kFerryLineS4 = 204,				//!< フェリー航路(S4破線表示部)
			kVirtualLink = 301,				//!< 仮想リンク
			kGardenCourse = 302,			//!< 庭園路
			kBridgeLink = 303,				//!< ブリッジリンク
			kInstituteGateway = 304,		//!< 施設出入口リンク
			kParkingPremisesLink = 401,		//!< 駐車場構内道路リンク（PEC）
			kMatchingLink = 402,			//!< マッチングリンク（PEC）
			kInstituteLink = 305			//!< 施設内リンク
		};
	}

	/**
	 * \namespace sindy::naviclass_code
	 * @brief 道路経路用種別コード2用ネームスペース
	 */	
	namespace naviclass_code {
		/**
		 * \enum sindy::naviclass_code::ECode
		 * @brief 道路経路用種別コード2ドメイン
		 */
		enum ECode
		{
			kNotCourse = 0,					//!< 経路外道路
			kExpressway = 101,				//!< 高速自動車道路
			kCityExpressway = 102,			//!< 都市高速道路
			kNational = 1,					//!< 一般国道
			kTurnpike = 103,				//!< 一般国道（有料）
			kMainLocalPrefRoad = 2,			//!< 主要地方道（都府県）
			kMainLoaclPrefTollRoad = 104,	//!< 主要地方道（都府県）（有料）
			kMainLocalRoad = 3,				//!< 主要地方道（指定市）
			kMainLocalTollRoad = 105,		//!< 主要地方道（指定市）（有料）
			kPrefecturesRoad = 4,			//!< 一般都道府県道
			kPrefecturesTollRoad = 106,		//!< 一般都道府県道（有料）
			kCityRoad = 5,					//!< 指定市の一般市道
			kCityTollRoad = 107,			//!< 指定市の一般市道（有料）
			kOtherRoad = 6,					//!< その他道路
			kOtherTollRoad = 108,			//!< その他道路（有料）
			kConnectNational = 403,			//!< 接続国道
			kFerryLine = 201				//!< フェリー航路
		};
	}

	/**
	 * \namespace sindy::linkclass_code
	 * @brief リンク種別コード用ネームスペース
	 */	
	namespace linkclass_code {
		/**
		 * \enum sindy::linkclass_code::ECode
		 * @brief リンク種別コードドメイン
		 */
		enum ECode
		{
			kUnInvestigated = 0,			//!< 未調査・作業中
			kMainLine = 1,					//!< 本線(上下線非分離)リンク
			kMainLineWithMedian = 2,		//!< 本線(上下分離)リンク
			kConnection = 3,				//!< 連結路(本線間の渡り線)
			kIntersection = 4,				//!< 交差点内リンク
			kRamp = 5,						//!< 連結路(ランプ)リンク
			kSideLine = 6,					//!< 本線と同一路線の側道リンク
			kSA = 7,						//!< ＳＡなど側線リンク
			kCyclingRoad = 8,				//!< 自転車道等
			kConnectSideLine = 9,			//!< 本線側道接続リンク
		};
	}

	/**
	 * \namespace sindy::roadmanager_code
	 * @brief 道路管理者コード用ネームスペース
	 */	
	namespace roadmanager_code {
		/**
		 * \enum sindy::roadmanager_code::ECode
		 * @brief 道路管理者コードドメイン
		 */
		enum ECode
		{
			kUnInvestigated = 0,			//!< 未調査・作業中
			kJH = 1,						//!< 日本道路公団
			kRoadPublicCorp3 = 2,			//!< 道路３公団
			kRoadPuclicCorp = 3,			//!< 道路公社
			kNational = 4,					//!< 国
			kPrefectures = 5,				//!< 都道府県
			kCity = 6,						//!< 指定市
			kMunicipality = 7,				//!< 他の市町村
			kOthers = 8,					//!< その他の管理者
		};
	}
	
	/**
	 * \namespace sindy::roadwidth_code
	 * @brief 道路幅員区分コード2用ネームスペース
	 */	
	namespace roadwidth_code {
		/**
		 * \enum sindy::roadwidth_code::ECode
		 * @brief 道路幅員区分コード2ドメイン
		 */
		enum ECode 
		{
			kUnInvestigated = 0,			//!< 未調査
			kOver130 = 1,					//!< 幅員13.0m以上
			kFrom55To130 = 2,				//!< 幅員5.5m以上13.0m未満
			kFrom30To55 = 3,				//!< 幅員3.0m以上5.5m未満
			kLess30 = 4,					//!< 幅員3.0m未満
		};
	}

	/**
	 * \namespace sindy::widearea_code
	 * @brief 広域存在コード2用ネームスペース
	 */	
	namespace widearea_code {
		/**
		 * \enum sindy::widearea_code::ECode
		 * @brief 広域存在コード2ドメイン
		 */
		enum ECode 
		{
			kNoWideArea = 0,				//!< 広域なし
			kAvoidCharge = 1,				//!< 有料回避
			kBasic = 2,						//!< 標準
		};
	}

	/**
	 * \namespace sindy::updownclass_code
	 * @brief 上下線種別コード用ネームスペース
	 */	
	namespace updownclass_code {
		/**
		 * \enum sindy::updownclass_code::ECode
		 * @brief 上下線種別コードドメイン
		 */
		enum ECode 
		{
			kUnInvestigated = 0,			//!< 未調査
			kInner = 1,						//!< 上り・内回り・西行き
			kOuter = 2,						//!< 下り・外回り・東行き
			kCommon = 3,					//!< 上下線共通
		};
	}

	/**
	 * \namespace sindy::nopassage_code
	 * @brief 通行禁止種別コード用ネームスペース
	 */	
	namespace nopassage_code {
		/**
		 * \enum sindy::nopassage_code::ECode
		 * @brief 通行禁止種別コードドメイン
		 */
		enum ECode 
		{
			kNothing = 0,					//!< 通行禁止なし
			kNoPassage = 1,					//!< 条件なし通行禁止
			kNoPassageWithCondition = 2,	//!< 条件付き通行禁止
		};
	}

	/**
	 * \namespace sindy::oneway_code
	 * @brief 一方通行種別コード用ネームスペース
	 */	
	namespace oneway_code {
		/**
		 * \enum sindy::oneway_code::ECode
		 * @brief 一方通行種別コードドメイン
		 */
		enum ECode 
		{
			kNothing = 0,					//!< 一方通行なし
			kOneway = 1,					//!< 条件なし一方通行（正方向）
			kOnewayRev = 2,					//!< 条件なし一方通行（逆方向）
			kOnewayWithCond = 3,			//!< 条件付き一方通行（正方向）
			kOnewayRevWithCond = 4,			//!< 条件付き一方通行（逆方向）
			kOnwwayBothWithCond = 5,		//!< 条件付き一方通行（正逆あり）
		};
	}

	/**
	 * \namespace sindy::limitspeed_code
	 * @brief 規制速度コード2用ネームスペース
	 */	
	namespace limitspeed_code {
		/**
		 * \enum sindy::limitspeed_code::ECode
		 * @brief 規制速度コード2ドメイン
		 */
		enum ECode 
		{
			kUnInvestigated = 0,			//!< 未調査
			kLess30 = 1,					//!< ３０km/h未満
			k40 = 2,						//!< ４０km/h
			k50 = 3,						//!< ５０km/h
			k60 = 4,						//!< ６０km/h
			k70 = 5,						//!< ７０km/h
			k80 = 6,						//!< ８０km/h
			k100 = 7,						//!< １００km/h
		};
	}

	/**
	 * \namespace sindy::kubun_code
	 * @brief 主従道路区分コード用ネームスペース
	 */	
	namespace kubun_code {
		/**
		 * \enum sindy::kubun_code::ECode
		 * @brief 主従道路区分コードドメイン
		 */
		enum ECode 
		{
			kUnInvestigated = 0,			//!< 未調査
			kMainRoad = 1,					//!< 主道路
			kSubRoad = 2,					//!< 従道路
		};
	}

	/**
	 * \namespace sindy::alleymainte_code
	 * @brief 細街路作業年度コード用ネームスペース
	 */	
	namespace alleymainte_code {
		/**
		 * \enum sindy::alleymainte_code::ECode
		 * @brief 細街路作業年度コードドメイン
		 */
		enum ECode 
		{
			kNoData = 0,					//!< 該当なし
			k1998 = 1,						//!< 1998年度
			k1999 = 2,						//!< 1999年度
			k2000Spring = 3,				//!< 2000年度（春）
			k2000Autumn = 4,				//!< 2000年度（秋）
			k2001 = 5,						//!< 2001年度
			k2002 = 6,						//!< 2002年度
			k2003 = 7,						//!< 2003年度
		};
	}

	/**
	 * \namespace sindy::parkingclass_code
	 * @brief 駐車場種別コード用ネームスペース
	 */	
	namespace parkingclass_code {
		/**
		 * \enum sindy::parkingclass_code::ECode
		 * @brief 駐車場種別コードドメイン
		 */
		enum ECode 
		{
			kUnInvestigated = 0,				//!< 未調査
			kParkingStructure = 1,				//!< 立体駐車場
			kUndergroundParking = 2,			//!< 地下駐車場
			kUndergroundParkingStructure = 3,	//!< 立体かつ地下駐車場
			kTowerParking = 4,					//!< タワーパーキング
			kAutoRotateTowerParking = 5,		//!< 自動回転タワーパーキング
			kParkingLot = 6,					//!< 平面駐車場
		};
	}

	/**
	 * \namespace sindy::parkinglinkclass_code
	 * @brief 駐車場リンク種別コード用ネームスペース
	 */	
	namespace parkinglinkclass_code {
		/**
		 * \enum sindy::parkinglinkclass_code::ECode
		 * @brief 駐車場リンク種別コードドメイン
		 */
		enum ECode 
		{
			kUnInvestigated = 0,				//!< 未調査
			kEntrance = 1,						//!< 入口リンク
			kExit = 2,							//!< 出口リンク
			kGateway = 3,						//!< 出入口リンク
			kParkingPrecinct = 4,				//!< 駐車場構内リンク
			kLevelConnect = 5,					//!< レベル間の渡りリンク
			kParkingConnect = 6,				//!< 駐車場接続リンク
		};
	}

	/**
	 * \namespace sindy::nodeclass_code
	 * @brief ノード種別コード3用ネームスペース
	 */	
	namespace nodeclass_code {
		/**
		 * \enum sindy::nodeclass_code::ECode
		 * @brief ノード種別コード3ドメイン
		 */
		enum ECode 
		{
			kEmptyNode = 0,					//!< 機能なしノード
			kIntersectionNode = 1,			//!< 交差点ノード
			kEndNode = 2,					//!< 端点ノード
			kAttributeChangeNode = 3,		//!< 属性変化点ノード
			kAttributeNode = 4,				//!< 属性付きノード
			kPECLinkNode = 5,				//!< PECリンク接続ノード
			kLinkQueueNode = 6,				//!< リンク列関係ノード
			kTollGateNode = 7,				//!< 料金所ノード
			kHighwayNode = 8,				//!< ハイウェイ関係ノード
			kMeshNode = 9,					//!< ２次メッシュ区画辺上ノード
			kIdentityCheckNode = 10,		//!< 同一リンクＩＤ防止ノード
			kPECNode = 11,					//!< PECノード
			kMeshPECNode = 12,				//!< ２次メッシュ区画辺上PECノード
			kDummyNode = 13,				//!< ダミー施設ノード
		};
	}
	 
	/**
	 * \namespace sindy::height_code
	 * @brief ノード相対高さコード用ネームスペース
	 */	
	namespace height_code {
		/**
		 * \enum sindy::height_code::ECode
		 * @brief ノード相対高さコードドメイン
		 */
		enum ECode 
		{
			kDisabled = 0,					//!< 	相対高さなし
			kEnabled = 1,					//!< 	相対高さ有効
			kAuto = 2,						//!< 	相対高さ自動付与
		};
	}

	/**
	 * \namespace sindy::signal_code
	 * @brief 信号機種別コード用ネームスペース
	 */	
	namespace signal_code {
		/**
		 * \enum sindy::signal_code::ECode
		 * @brief 信号機種別コードドメイン
		 */
		enum ECode 
		{
			kNoSignal = 0,					//!< 		信号機なし
			kSignal = 1,					//!< 		信号機あり
			kUnInvestigated = 2,			//!< 		未調査
		};
	}

	/**
	 * \namespace sindy::linkattr_code
	 * @brief リンク内属性種別コード2用ネームスペース
	 */	
	namespace linkattr_code {
		/**
		 * \enum sindy::linkattr_code::ECode
		 * @brief リンク内属性種別コード2ドメイン
		 */
		enum ECode 
		{
			kBridge = 1,					//!< 橋・高架
			kTunnel = 2,					//!< トンネル
			kCave = 3,						//!< 洞門
			kRailroadCrossing = 4,			//!< 踏切
			kUnderPass = 5,					//!< アンダーパス
			kUpDownRelationship = 6,		//!< 上下関係
		};
	}

	/**
	 * \namespace sindy::signage_code
	 * @brief 地先種別コード用ネームスペース
	 */	
	namespace signage_code {
		/**
		 * \enum sindy::signage_code::ECode
		 * @brief 地先種別コードドメイン
		 */
		enum ECode 
		{
			kGeneral = 1,					//!< 地先（一般）
			kToll = 2,						//!< 地先（有料）
			kNationalWayNo = 3,				//!< 国道番号
			kPrefWayNo = 4,					//!< 県道番号
			kOther = 5,						//!< その他
			kTollString = 12,				//!< 有料（文字列）
			kTollNationalWayNo = 13,		//!< 有料（国道番号）
			kTollPrefWayNo = 14,			//!< 有料（県道番号）
		};
	}

	/**
	 * \namespace sindy::signage_code
	 * @brief 地先種別コード用ネームスペース
	 */	
	namespace signage_code {
		/**
		 * \enum sindy::signage_code::ECode
		 * @brief 地先種別コードドメイン
		 */
		enum ECode 
		{
			kAccessRoad = 1,				//!< アクセス道路
			kConnectRoad = 2,				//!< 乗り継ぎ道
			kInfomation = 3,				//!< 観光案内
		};
	}

	/**
	 * \namespace sindy::signage_code
	 * @brief 地先種別コード用ネームスペース
	 */	
	namespace signage_code {
		/**
		 * \enum sindy::signage_code::ECode
		 * @brief 地先種別コードドメイン
		 */
		enum ECode 
		{
			kHoliday = 1,					//!< 日・休日
			kExceptHolidayAndSunday = 2,	//!< 日・休日をのぞく
			kHolidayExceptSunday = 3,		//!< 休日(日曜はのぞく)
			kExceptHoliday = 4,				//!< 休日をのぞく
			kOrdinaryDays = 5,				//!< 平日
			kSunday = 6,					//!< 日曜
			kMonday = 7,					//!< 月
			kTuesday = 8.					//!< 火
			kWednesday = 9,					//!< 水
			kThursday = 10,					//!< 木
			kFriday = 11,					//!< 金
			kSaturday = 12,					//!< 土
			kWeekday = 13,					//!< 土日・休日
			kExceptWeekday = 14,			//!< 土日・休日をのぞく
			kAlldays = 15,					//!< 全曜日
			kExceptSunday = 16,				//!< 日曜日をのぞく
			kBeforeHoliday = 17,			//!< 日・休日の前日
		};
	}

	/**
	 * \namespace sindy::signage_code
	 * @brief 地先種別コード用ネームスペース
	 */	
	namespace signage_code {
		/**
		 * \enum sindy::signage_code::ECode
		 * @brief 地先種別コードドメイン
		 */
		enum ECode 
		{
			case kUnsurveyed:					///< 未設定
			case kNoGuide:						///< 道なり（案内なし）
			case k12:							///< 直進（１２時方向）
			case k1:							///< 右前方向（１時方向）
			case k2:							///< 右手前方向（２時方向）
			case k3:							///< 右折（３時方向）
			case k4:							///< 右斜め後ろ方向（４時方向）
			case k5:							///< 右戻る方向（５時方向）
			case k6:							///< 戻る方向（６時方向）
			case k7:							///< 左戻る方向（７時方向）
			case k8:							///< 左斜め後ろ方向（８時方向）
			case k9:							///< 左折（９時方向）
			case k10:							///< 左手前方向（１０時方向）
			case k11:							///< 左前方向（１１時方向）
};

#endif // _ROADCOMMUNITY_H_
