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

#ifndef SINDY_SCHEMA_DRAFT_ROAD_H_
#define SINDY_SCHEMA_DRAFT_ROAD_H_

#include <iostream>
#include <boost/preprocessor.hpp>
#include <sindy/schema/common.h>   // 間借り
#include <sindy/schema/category.h> // 間借り

namespace sindy {

namespace schema {

namespace road_link {
	namespace walkable_class {
		enum ECode_draft {
			kBothSideWalk             = 3, //!< 両側歩道あり
			kRightSideWalkWalkable    = 4, //!< 右側歩道あり（人車共用）
			kRightSideWalkNonWalkable = 5, //!< 右側歩道あり（歩行禁止）
			kLeftSideWalkWalkable     = 6, //!< 左側歩道あり（人車共用）
			kLeftSideWalkNonWalkable  = 7, //!< 左側歩道あり（歩行禁止）
		};
	} // walkable_class
} // road_link

namespace walk_link {

	// 歩行者表示種別コード
	namespace walk_class { // 将来的になくなるコード値
		enum ECode_draft {
			kLogicalConnection          = 199,  //!< ロジカルコネクション
			kUnsurveyInternalTicketGate = 399,  //!< 改札内（仮）
		};
	} // walk_class
} // walk_link

namespace walk_node {

	namespace node_class {
		enum ECode_draft {
			kConnectedRoad = 15,       //!< 道路リンク接続ノード
			kUnsurveyTicketGate = 98,  //!< 改札ノード（仮）
		};
	} // node_class
} // walk_node

namespace pedxing {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[];     //!< テーブル名

	extern const _TCHAR kPedxingClass[];  //!< 歩行者ノード種別

	namespace pedxing_class {
		enum ECode_draft {
			kCrossRoad         = 1, //!< 横断歩道
			kInferredCrossRoad = 2, //!< みなし横断歩道
		};
	} // pedxing_class
} // pedxing

namespace scramble_area {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[];     //!< テーブル名

	extern const _TCHAR kScrambleClass[]; //!< スクランブル種別コード

	// スクランブル種別コード
	namespace scramble_class {
		enum ECode_draft {
			kManual      = 1,   //!< 手動
			kAuto        = 2,   //!< 機械
		};
	} // scramble_class
} // scramble_area
}
}
#endif // SINDY_SCHEMA_ROAD_H_
