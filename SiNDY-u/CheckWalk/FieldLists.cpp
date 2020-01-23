#include "stdafx.h"
#include <TDC/useful_headers/tstring.h>
#include<TDC/useful_headers/str_util.h>
#include <sindy/schema/road.h>
#include "FieldLists.h"

namespace field_lists
{
	using namespace std;
	using namespace sindy::schema;
	// 道路リンク フィールド規定
	const FIELD_LIST roadLinkFieldList = boost::assign::list_of
	( CString( ipc_feature::kObjectID ) )
		(CString( ipc_feature::kShape ))
		(CString( ipc_feature::kOperator ))
		(CString( ipc_feature::kModifyDate ))
		(CString( ipc_feature::kModifyProgName ))
		(CString( ipc_feature::kProgModifyDate ))
		(CString( road_link::kFromNodeID ))
		(CString( road_link::kToNodeID ))
		(CString( road_link::kRoadClass ))
		(CString( road_link::kLaneCount ))
		(CString( road_link::kCarOnly ))
		(CString( road_link::kWalkable ))
		(CString( road_link::kNaviClass ))
		(CString( road_link::kNoPassageRF ))
		(CString( road_link::kMainLinkClass ))
		(CString( road_link::kPedestrianZone ))
		(CString( road_link::kAlleyMainte ));
	// 道路ノード フィールド規定
	const FIELD_LIST roadNodeFieldList = boost::assign::list_of
	( CString( ipc_feature::kObjectID ) )
		(CString( ipc_feature::kShape ))
		(CString( ipc_feature::kOperator ))
		(CString( ipc_feature::kModifyDate ))
		(CString( ipc_feature::kModifyProgName ))
		(CString( ipc_feature::kProgModifyDate ))
		(CString( road_node::kNodeClass ))
		(CString( road_node::kNameKanji ))
		(CString( road_node::kNameYomi ));

	// リンク内属性
	const FIELD_LIST segmentAttrFieldList = boost::assign::list_of
	( CString( ipc_feature::kObjectID ) )
		(CString( ipc_feature::kShape ))
		(CString( ipc_feature::kOperator ))
		(CString( ipc_feature::kModifyDate ))
		(CString( segment_attr::kLinkID ))
		(CString( segment_attr::kLinkAttr ));

	// 歩行者リンク フィールド規定(指定なし→全フィールド)
	const FIELD_LIST walkLinkFieldList = FIELD_LIST(0);

	// 歩行者ノード フィールド規定(指定なし→全フィールド)
	const FIELD_LIST walkNodeFieldList = FIELD_LIST( 0 );

	// 駅出入口ポイント フィールド規定
	const FIELD_LIST stationGatePointFieldList = boost::assign::list_of
	( CString( ipc_feature::kObjectID ) )
		(CString( ipc_feature::kShape ))
		(CString( ipc_feature::kOperator ))
		(CString( ipc_feature::kModifyDate ))
		(CString( ipc_feature::kModifyProgName ))
		(CString( ipc_feature::kProgModifyDate ))
		(CString( station_gate_point::kName ))
		(CString( station_gate_point::kYomi ));

	// POIポイント フィールド規定
	const FIELD_LIST poiPointFieldList = boost::assign::list_of
	( CString( ipc_feature::kObjectID ) )
		(CString( ipc_feature::kShape ))
		(CString( ipc_feature::kOperator ))
		(CString( ipc_feature::kModifyDate ))
		(CString( ipc_feature::kModifyProgName ))
		(CString( ipc_feature::kProgModifyDate ))
		(CString( sindyk::poi_point_org::kName ))
		(CString( sindyk::poi_point_org::kYomi ))
		(CString( sindyk::poi_point_org::kDelete ));

	// 地下街ポリゴン
	const FIELD_LIST underGroundAeraFiledList = boost::assign::list_of
	( CString( ipc_feature::kObjectID ) )
		(CString( ipc_feature::kShape ))
		(CString( ipc_feature::kOperator ))
		(CString( ipc_feature::kModifyDate ))
		(CString( ipc_feature::kModifyProgName ))
		(CString( ipc_feature::kProgModifyDate ));

	// 歩行者路線コード フィールド規定
	const FIELD_LIST walkCodeFieldList = boost::assign::list_of
	( CString( ipc_table::kObjectID ) )
		(CString( ipc_table::kOperator ))
		(CString( ipc_table::kModifyDate ))
		(CString( walk_code_list::kWalkCode ))
		(CString( walk_code_list::kNameKanji ))
		(CString( walk_code_list::kNameYomi ));

	// 歩行者通行禁止 フィールド規定
	const FIELD_LIST walkNoPassageFieldList = boost::assign::list_of
	( CString( ipc_table::kObjectID ) )
		(CString( ipc_table::kOperator ))
		(CString( ipc_table::kModifyDate ))
		(CString( ipc_feature::kModifyProgName ))
		(CString( ipc_feature::kProgModifyDate ))
		(CString( walk_nopassage::kLinkID ))
		(CString( walk_nopassage::kStartMonth ))
		(CString( walk_nopassage::kStartDay ))
		(CString( walk_nopassage::kStartHour ))
		(CString( walk_nopassage::kStartMin ))
		(CString( walk_nopassage::kEndMonth ))
		(CString( walk_nopassage::kEndDay ))
		(CString( walk_nopassage::kEndHour ))
		(CString( walk_nopassage::kEndMin ))
		(CString( walk_nopassage::kDayOfWeek ));

	// 歩行者一方通行 フィールド規定
	const FIELD_LIST walkOnewayFieldList = boost::assign::list_of
	( CString( ipc_table::kObjectID ) )
		(CString( ipc_table::kOperator ))
		(CString( ipc_table::kModifyDate ))
		(CString( ipc_feature::kModifyProgName ))
		(CString( ipc_feature::kProgModifyDate ))
		(CString( walk_oneway::kLinkID ))
		(CString( walk_oneway::kLinkDir ))
		(CString( walk_oneway::kStartMonth ))
		(CString( walk_oneway::kStartDay ))
		(CString( walk_oneway::kStartHour ))
		(CString( walk_oneway::kStartMin ))
		(CString( walk_oneway::kEndMonth ))
		(CString( walk_oneway::kEndDay ))
		(CString( walk_oneway::kEndHour ))
		(CString( walk_oneway::kEndMin ))
		(CString( walk_oneway::kDayOfWeek ));

	// リンク紐付テーブル フィールド規定
	const FIELD_LIST linkRelationFieldList = boost::assign::list_of
	( CString( ipc_table::kObjectID ) )
		(CString( ipc_table::kOperator ))
		(CString( ipc_table::kModifyDate ))
		(CString( ipc_feature::kModifyProgName ))
		(CString( ipc_feature::kProgModifyDate ))
		(CString( link_relation::kBaseLinkID ))
		(CString( link_relation::kRelatedLinkID ));

	// 重なりリンクの高さノード
	const FIELD_LIST heightNodeFieldList = boost::assign::list_of
	( CString( ipc_feature::kObjectID ) )
		(CString( ipc_feature::kShape ))
		(CString( ipc_feature::kOperator ))
		(CString( ipc_feature::kModifyDate ))
		(CString( ipc_feature::kModifyProgName ))
		(CString( ipc_feature::kProgModifyDate ))
		(CString( height_node::kLinkID1 ))
		(CString( height_node::kLinkID1Height ))
		(CString( height_node::kLinkID1Table ))
		(CString( height_node::kLinkID2 ))
		(CString( height_node::kLinkID2Height ))
		(CString( height_node::kLinkID2Table ));

	// スクランブルエリア フィールド規定(指定なし→全フィールド)
	const FIELD_LIST scrambleAreaList = FIELD_LIST( 0 );

	// 横断ポイント フィールド規定(指定なし→全フィールド)
	const FIELD_LIST pedxingList = FIELD_LIST( 0 );
}