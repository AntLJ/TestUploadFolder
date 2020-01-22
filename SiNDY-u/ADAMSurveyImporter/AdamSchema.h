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

#include <sindy/libschema.h>

#include "SchemaFiled.h"

namespace adam_v2
{

/**
 * ADAM スキーマ定義
 *
 * @version	2.1.5
 * @see		http://preon.mr.ipc.pioneer.co.jp/svn/release/trunk/public/SiNDY-b/Documents/data_model/
 */
class AdamSchema
{
public:
	/**
	 * コンストラクタ
	 */
	AdamSchema(void){}

	/**
	 * デストラクタ
	 */
	~AdamSchema(void){}

	/**
	 * データクリア
	 */
	void Clear();

public:
	/* ===================================================
	     インデックス番号とフィールド名定義 (フィーチャ)
	   =================================================== */
	/* 
	 * iPCFeature クラス
	 */
	class iPCFeature
	{
	public:
		SchemaFiled	objectid		;	///< OBJECTID
		SchemaFiled	shape			;	///< Shape
		SchemaFiled	operator_name	;	///< 最終更新者
		SchemaFiled	purpose_c		;	///< 作業目的
		SchemaFiled	modifydate		;	///< 最終更新日時
		SchemaFiled	updatetype_c	;	///< 最終更新内容
		SchemaFiled	progmodifydate	;	///< 最終プログラム更新日時
		SchemaFiled	modifyprogname	;	///< 最終更新プログラム名
		SchemaFiled	userclaim_f		;	///< ユーザクレームフラグ
		SchemaFiled	source			;	///< 情報ソース
	public:
		iPCFeature(const ITablePtr& ipTable) : 
			objectid		(ipTable, sindy::schema::adam_v2_adam_ipc_feature::kObjectID				),
			shape			(ipTable, sindy::schema::adam_v2_adam_ipc_feature::kShape				),
			operator_name	(ipTable, sindy::schema::adam_v2_adam_ipc_feature::kOperator				),
			purpose_c		(ipTable, sindy::schema::adam_v2_adam_ipc_feature::kPurpose				),
			modifydate		(ipTable, sindy::schema::adam_v2_adam_ipc_feature::kModifyDate			),
			updatetype_c	(ipTable, sindy::schema::adam_v2_adam_ipc_feature::kUpdateType			),
			progmodifydate	(ipTable, sindy::schema::adam_v2_adam_ipc_feature::kProgModifyDate		),
			modifyprogname	(ipTable, sindy::schema::adam_v2_adam_ipc_feature::kModifyProgName		),
			userclaim_f		(ipTable, sindy::schema::adam_v2_adam_ipc_feature::kUserClaim			),
			source			(ipTable, sindy::schema::adam_v2_adam_ipc_feature::kSource				)
		{}

		virtual ~iPCFeature(){}
	};

	/* 
	 * AdamiPCFeature クラス
	 */
	class AdamiPCFeature : public iPCFeature
	{
	public:
		SchemaFiled	speculation_c	;	///< 推測コード
		SchemaFiled	sourceid		;	///< 提供元ID
		SchemaFiled	source_update_c	;	///< 提供元更新コード
	public:
		AdamiPCFeature(const ITablePtr& ipTable) : 
			iPCFeature		(ipTable),
			speculation_c	(ipTable, sindy::schema::adam_v2_adam_ipc_feature::kSperculationC		),
			sourceid		(ipTable, sindy::schema::adam_v2_adam_ipc_feature::kSourceID				),
			source_update_c	(ipTable, sindy::schema::adam_v2_adam_ipc_feature::kSourceUpdateC )
		{}

		virtual ~AdamiPCFeature(){}
	};

	/* 
	 * GlobalAdamiPCFeature クラス
	 */
	class GlobalAdamiPCFeature : public AdamiPCFeature
	{
	public:
		SchemaFiled	globalid		;	///< GLOBALID
	public:
		GlobalAdamiPCFeature(const ITablePtr& ipTable) : 
			AdamiPCFeature	(ipTable),
			globalid		(ipTable, sindy::schema::adam_v2_adam_ipc_feature::kGlobalID				)
		{}

		virtual ~GlobalAdamiPCFeature(){}
	};

	/**
	 * 上下線リンク フィーチャ
	 */
	class UpdownLink : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名
		
		SchemaFiled	from_node_gid		;	///< 始点上下線ノードGID
		SchemaFiled	to_node_gid			;	///< 終点上下線ノードGID
		SchemaFiled	from_node_id		;	///< 始点上下線ノードOID
		SchemaFiled	to_node_id			;	///< 終点上下線ノードOID
		SchemaFiled	updown_group_id		;	///< 上下線グループOID
		SchemaFiled	lane_count			;	///< 車線数
		SchemaFiled	right_change		;	///< 右側増減車線数
		SchemaFiled	left_change			;	///< 左側増減車線数
		SchemaFiled	max_legal_speed_c	;	///< 制限速度コード（最大）
		SchemaFiled	min_legal_speed_c	;	///< 制限速度コード（最小）
		SchemaFiled	travel_direction_c	;	///< 進行方向種別コード
		SchemaFiled	road_structure_c	;	///< 道路構造種別コード
		SchemaFiled	oneway_c			;	///< 一方通行種別コード
		SchemaFiled	rubbing_f			;	///< 擦り付けフラグ
		SchemaFiled	tunnel_f			;	///< トンネルフラグ
	public:
		UpdownLink(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature		(ipTable),
			from_node_gid		(ipTable, sindy::schema::adam_v2::updown_link::kFromNodeGID		),
			to_node_gid			(ipTable, sindy::schema::adam_v2::updown_link::kToNodeGID		),
			from_node_id		(ipTable, sindy::schema::adam_v2::updown_link::kFromNodeID		),
			to_node_id			(ipTable, sindy::schema::adam_v2::updown_link::kToNodeID			),
			updown_group_id		(ipTable, sindy::schema::adam_v2::updown_link::kUpdoenGroupID	),
			lane_count			(ipTable, sindy::schema::adam_v2::updown_link::kLaneCount		),
			right_change		(ipTable, sindy::schema::adam_v2::updown_link::kRightChange		),
			left_change			(ipTable, sindy::schema::adam_v2::updown_link::kLeftChange		),
			max_legal_speed_c	(ipTable, sindy::schema::adam_v2::updown_link::kMaxLegalSpeedC	),
			min_legal_speed_c	(ipTable, sindy::schema::adam_v2::updown_link::kMinLegalSpeedC	),
			travel_direction_c	(ipTable, sindy::schema::adam_v2::updown_link::kTravelDirectionC	),
			road_structure_c	(ipTable, sindy::schema::adam_v2::updown_link::kRoadStructureC	),
			oneway_c			(ipTable, sindy::schema::adam_v2::updown_link::kOneWayC			),
			rubbing_f			(ipTable, sindy::schema::adam_v2::updown_link::kRubbingF			),
			tunnel_f			(ipTable, sindy::schema::adam_v2::updown_link::kTunnelF			)
		{}

		~UpdownLink(){}
	};

	/**
	 * 上下線ノード フィーチャ
	 */
	class UpdownNode : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled nodeclass_c		;	///< ノード種別コード
		SchemaFiled stop_point_f	;	///< 停止点フラグ
		SchemaFiled covering_f		;	///< 覆いものフラグ
	public:
		UpdownNode(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature	(ipTable),
			nodeclass_c		(ipTable, sindy::schema::adam_v2::updown_node::kNodeClassC		),
			stop_point_f	(ipTable, sindy::schema::adam_v2::updown_node::kStopPointF		),
			covering_f		(ipTable, sindy::schema::adam_v2::updown_node::kCoveringF		)
		{}

		~UpdownNode(){}
	};

	/**
	 * 車線リンク フィーチャ
	 */
	class LaneLink : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled from_node_gid		;	///< 始点車線ノードGID
		SchemaFiled to_node_gid			;	///< 終点車線ノードGID
		SchemaFiled from_node_id		;	///< 始点車線ノードOID
		SchemaFiled to_node_id			;	///< 終点車線ノードOID
		SchemaFiled lane_group_id		;	///< 車線グループOID
		SchemaFiled lane_number			;	///< 車線番号
		SchemaFiled lane_count			;	///< 車線数
		SchemaFiled max_legal_speed_c	;	///< 制限速度コード（最大）
		SchemaFiled min_legal_speed_c	;	///< 制限速度コード（最小）
		SchemaFiled travel_direction_c	;	///< 進行方向種別コード
		SchemaFiled road_structure_c	;	///< 道路構造種別コード
		SchemaFiled oneway_c			;	///< 一方通行種別コード
		SchemaFiled left_change_f		;	///< 左車線変更可能フラグ
		SchemaFiled right_change_f		;	///< 右車線変更可能フラグ
		SchemaFiled left_protrusion_f	;	///< 左はみ出し可能フラグ
		SchemaFiled right_protrusion_f	;	///< 右はみ出し可能フラグ
		SchemaFiled rubbing_f			;	///< 擦り付けフラグ
		SchemaFiled tunnel_f			;	///< トンネルフラグ
	public:
		LaneLink(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature		(ipTable),
			from_node_gid		(ipTable, sindy::schema::adam_v2::lane_link::kFromNodeGID		),
			to_node_gid			(ipTable, sindy::schema::adam_v2::lane_link::kToNodeGID			),
			from_node_id		(ipTable, sindy::schema::adam_v2::lane_link::kFromNodeID			),
			to_node_id			(ipTable, sindy::schema::adam_v2::lane_link::kToNodeID			),
			lane_group_id		(ipTable, sindy::schema::adam_v2::lane_link::kLaneGroupID		),
			lane_number			(ipTable, sindy::schema::adam_v2::lane_link::kLaneNumber			),
			lane_count			(ipTable, sindy::schema::adam_v2::lane_link::kLaneCount			),
			max_legal_speed_c	(ipTable, sindy::schema::adam_v2::lane_link::kMaxLegalSpeedC		),
			min_legal_speed_c	(ipTable, sindy::schema::adam_v2::lane_link::kMinLegalSpeedC		),
			travel_direction_c	(ipTable, sindy::schema::adam_v2::lane_link::kTravelDirectionC	),
			road_structure_c	(ipTable, sindy::schema::adam_v2::lane_link::kRoadStructureC		),
			oneway_c			(ipTable, sindy::schema::adam_v2::lane_link::kOneWayC			),
			left_change_f		(ipTable, sindy::schema::adam_v2::lane_link::kLeftChangeF		),
			right_change_f		(ipTable, sindy::schema::adam_v2::lane_link::kRightChangeF		),
			left_protrusion_f	(ipTable, sindy::schema::adam_v2::lane_link::kLeftProtrusionF	),
			right_protrusion_f	(ipTable, sindy::schema::adam_v2::lane_link::kRightProtrusionF	),
			rubbing_f			(ipTable, sindy::schema::adam_v2::lane_link::kRubbingF			),
			tunnel_f			(ipTable, sindy::schema::adam_v2::lane_link::kTunnelF			)
		{}

		~LaneLink(){}
	};

	/**
	 * 車線ノード フィーチャ
	 */
	class LaneNode : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled nodeclass_c				;	///< ノード種別コード
		SchemaFiled stop_point_f			;	///< 停止点フラグ
		SchemaFiled covering_f				;	///< 覆いものフラグ
	public:
		LaneNode(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature	(ipTable),
			nodeclass_c				(ipTable, sindy::schema::adam_v2::lane_node::kNodeClassC				),
			stop_point_f			(ipTable, sindy::schema::adam_v2::lane_node::kStopPointF				),
			covering_f				(ipTable, sindy::schema::adam_v2::lane_node::kCoveringF				)
		{}

		~LaneNode(){}
	};

	/**
	 * 区画線リンク フィーチャ
	 */
	class CompartLink : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled from_node_gid		;	///< 始点区画線ノードGID
		SchemaFiled to_node_gid			;	///< 終点区画線ノードGID
		SchemaFiled from_node_id		;	///< 始点区画線ノードOID
		SchemaFiled to_node_id			;	///< 終点区画線ノードOID
		SchemaFiled compart_line_c		;	///< 線種別コード
		SchemaFiled compart_style_c		;	///< 区画線設置様式コード
		SchemaFiled slowdown_label_c	;	///< 減速表示コード
		SchemaFiled installation_f		;	///< 設置物フラグ
	public:
		CompartLink(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature		(ipTable),
			from_node_gid		(ipTable, sindy::schema::adam_v2::compart_link::kFromNodeGID			),
			to_node_gid			(ipTable, sindy::schema::adam_v2::compart_link::kToNodeGID			),
			from_node_id		(ipTable, sindy::schema::adam_v2::compart_link::kFromNodeID			),
			to_node_id			(ipTable, sindy::schema::adam_v2::compart_link::kToNodeID			),
			compart_line_c		(ipTable, sindy::schema::adam_v2::compart_link::kCompartLineC		),
			compart_style_c		(ipTable, sindy::schema::adam_v2::compart_link::kCompartStyleC		),
			slowdown_label_c	(ipTable, sindy::schema::adam_v2::compart_link::kSlowdownLabelC		),
			installation_f		(ipTable, sindy::schema::adam_v2::compart_link::kInstallationF		)
		{}

		~CompartLink(){}
	};

	/**
	 * 区画線ノード フィーチャ
	 */
	class CompartNode : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled nodeclass_c	;	///< ノード種別コード
	public:
		CompartNode(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature	(ipTable),
			nodeclass_c				(ipTable, sindy::schema::adam_v2::compart_node::kNodeClassC		)
		{}

		~CompartNode(){}
	};

	/**
	 * 境界線リンク フィーチャ
	 */
	class BorderLink : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled from_node_gid		;	///< 始点区画線ノードGID
		SchemaFiled to_node_gid			;	///< 終点区画線ノードGID
		SchemaFiled from_node_id		;	///< 始点区画線ノードOID
		SchemaFiled to_node_id			;	///< 終点区画線ノードOID
		SchemaFiled imaginary_border_f	;	///< 仮想線フラグ
	public:
		BorderLink(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature		(ipTable),
			from_node_gid		(ipTable, sindy::schema::adam_v2::border_link::kFromNodeGID		),
			to_node_gid			(ipTable, sindy::schema::adam_v2::border_link::kToNodeGID		),
			from_node_id		(ipTable, sindy::schema::adam_v2::border_link::kFromNodeID		),
			to_node_id			(ipTable, sindy::schema::adam_v2::border_link::kToNodeID			),
			imaginary_border_f	(ipTable, sindy::schema::adam_v2::border_link::kImaginaryBorderF	)
		{}

		~BorderLink(){}
	};

	/**
	 * 境界線ノード フィーチャ
	 */
	class BorderNode : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled nodeclass_c	;	///< ノード種別コード
	public:
		BorderNode(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature	(ipTable),
			nodeclass_c				(ipTable, sindy::schema::adam_v2::border_node::kNodeClassC		)
		{}

		~BorderNode(){}
	};

	/**
	 * 信号機ポイント フィーチャ
	 */
	class SignalPoint : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled horizontal_angle	;	///< 水平角
		SchemaFiled signal_mold_c		;	///< 信号機形状コード
		SchemaFiled lamp_count			;	///< 信号灯数
		SchemaFiled arrow_f				;	///< 矢印信号フラグ
		SchemaFiled warning_f			;	///< 予告信号フラグ
		SchemaFiled independent_f		;	///< 独立信号フラグ
	public:
		SignalPoint(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature		(ipTable),
			horizontal_angle	(ipTable, sindy::schema::adam_v2::signal_point::kHorizontalAngle		),
			signal_mold_c		(ipTable, sindy::schema::adam_v2::signal_point::kSignalMoldC			),
			lamp_count			(ipTable, sindy::schema::adam_v2::signal_point::kLampCount			),
			arrow_f				(ipTable, sindy::schema::adam_v2::signal_point::kArrowF				),
			warning_f			(ipTable, sindy::schema::adam_v2::signal_point::kWarningF			),
			independent_f		(ipTable, sindy::schema::adam_v2::signal_point::kIndependentF		)
		{}

		~SignalPoint(){}
	};

	/**
	 * 横断歩道ライン フィーチャ
	 */
	class CrossWalkLine : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled width;		///< 幅
	public:
		CrossWalkLine(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature		(ipTable),
			width				(ipTable, sindy::schema::adam_v2::crosswalk_line::kWidth					)
		{}

		~CrossWalkLine(){}
	};

	/**
	 * 道路標識ポイント フィーチャ
	 */
	class RoadSignPoint : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled horizontal_angle	;	///< 水平角
		SchemaFiled roadsign_c			;	///< 道路標識コード
		SchemaFiled variable_f			;	///< 可変フラグ
	public:
		RoadSignPoint(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature		(ipTable),
			horizontal_angle	(ipTable, sindy::schema::adam_v2::roadsign_point::kHorizontalAngle	),
			roadsign_c			(ipTable, sindy::schema::adam_v2::roadsign_point::kRoadSignC			),
			variable_f			(ipTable, sindy::schema::adam_v2::roadsign_point::kVariableF			)
		{}

		~RoadSignPoint(){}
	};

	/**
	 * 整備済みエリア フィーチャ
	 */
	class MaintenanceArea : public iPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		MaintenanceArea(const ITablePtr& ipTable) : 
			iPCFeature		(ipTable)
		{}
		~MaintenanceArea(){}
	};

	/**
	 * 道路標示エリア フィーチャ
	 *
	 * @note	インポート対象外のため定義のみ
	 */
	class RoadMarkArea : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled roadmark_c	;	///< 道路標示コード
		SchemaFiled description ;	///< 標示内容（文字）
	public:
		RoadMarkArea(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature		(ipTable),
			roadmark_c	(ipTable, sindy::schema::adam_v2::roadmark_area::kRoadMarkC		),
			description	(ipTable, sindy::schema::adam_v2::roadmark_area::kDescription	)
		{}

		~RoadMarkArea(){}
	};

	/**
	 * 仮想区画線ライン フィーチャ
	 */
	class VirtualCompartLine : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名
	public:
		VirtualCompartLine(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature		(ipTable)
		{}

		~VirtualCompartLine(){}
	};

	/**
	 * 覆いものエリア フィーチャ
	 */
	class CoveringArea : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名
	public:
		CoveringArea(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature		(ipTable)
		{}

		~CoveringArea(){}
	};

	/* ===================================================
	     インデックス番号とフィールド名定義 (テーブル)
	   =================================================== */
	/* 
	 * Table_class クラス
	 */
	class TableClass
	{
	public:
		SchemaFiled	objectid		;	///< OBJECTID
		SchemaFiled	operator_name	;	///< 最終更新者
		SchemaFiled	purpose_c		;	///< 作業目的
		SchemaFiled	modifydate		;	///< 最終更新日時
		SchemaFiled	progmodifydate	;	///< 最終プログラム更新日時
		SchemaFiled	modifyprogname	;	///< 最終更新プログラム名
		SchemaFiled	updatetype_c	;	///< 最終更新内容
		SchemaFiled	userclaim_f		;	///< ユーザクレームフラグ
		SchemaFiled	source			;	///< 情報ソース
	public:
		TableClass(const ITablePtr& ipTable) : 
			objectid		(ipTable, sindy::schema::ipc_table::kObjectID			),
			operator_name	(ipTable, sindy::schema::ipc_table::kOperator			),
			purpose_c		(ipTable, sindy::schema::ipc_table::kPurpose				),
			modifydate		(ipTable, sindy::schema::ipc_table::kModifyDate			),
			progmodifydate	(ipTable, sindy::schema::ipc_table::kProgModifyDate		),
			modifyprogname	(ipTable, sindy::schema::ipc_table::kModifyProgName		),
			updatetype_c	(ipTable, sindy::schema::ipc_table::kUpdateType			),
			userclaim_f		(ipTable, sindy::schema::ipc_table::kUserClaim			),
			source			(ipTable, sindy::schema::ipc_table::kSource				)
		{}

		virtual ~TableClass(){}
	};

	/* 
	 * AdamTableClass クラス
	 */
	class AdamTableClass : public TableClass
	{
	public:
		SchemaFiled	sourceid		;	///< 提供元ID
		SchemaFiled	source_update_c	;	///< 提供元更新コード
	public:
		AdamTableClass(const ITablePtr& ipTable) : 
			TableClass	(ipTable),
			sourceid	(ipTable, sindy::schema::adam_v2_table::kSourceID		),
			source_update_c(ipTable, sindy::schema::adam_v2_table::kSourceUpdateC)
		{}

		virtual ~AdamTableClass(){}
	};

	/* 
	 * GlobalAdamTableClass クラス
	 */
	class GlobalAdamTableClass : public AdamTableClass
	{
	public:
		SchemaFiled	globalid		;	///< GLOBALID
	public:
		GlobalAdamTableClass(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			globalid	(ipTable, sindy::schema::adam_v2_global_table::kGlobalID		)
		{}

		virtual ~GlobalAdamTableClass(){}
	};

	/* 
	 * AdamNodeQueue クラス
	 */
	class AdamNodeQueue : public GlobalAdamTableClass
	{
	public:
		SchemaFiled	sequence		;	///< 順番
		SchemaFiled	lastnode_f		;	///< 終端ノードフラグ
	public:
		AdamNodeQueue(const ITablePtr& ipTable) : 
			GlobalAdamTableClass	(ipTable),
			sequence	(ipTable, sindy::schema::adam_v2_node_queue::kSequence		),
			lastnode_f	(ipTable, sindy::schema::adam_v2_node_queue::kLastNodeF		)
		{}

		virtual ~AdamNodeQueue(){}
	};

	/* 
	 * 道路リンク－上下線リンク関連
	 */
	class RelRoadLinkUpdownLink : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled road_link_id			;	///< 道路リンクOID
		SchemaFiled updown_link_gid			;	///< 上下線リンクGID
		SchemaFiled road_link_direction_c	;	///< 道路リンク方向コード
	public:
		RelRoadLinkUpdownLink(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			road_link_id			(ipTable, sindy::schema::adam_v2::rel_road_link_updown_link::kRoadLinkID			),
			updown_link_gid			(ipTable, sindy::schema::adam_v2::rel_road_link_updown_link::kUpdownLinkGID		),
			road_link_direction_c	(ipTable, sindy::schema::adam_v2::rel_road_link_updown_link::kRoadLinkDirectionC	)
		{}

		~RelRoadLinkUpdownLink(){}
	};

	/* 
	 * 上下線ペアリンク
	 */
	class PairUpdownLink : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled updown_link_gid1	;	///< 上下線リンクGID1
		SchemaFiled updown_link_gid2	;	///< 上下線リンクGID2
	public:
		PairUpdownLink(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			updown_link_gid1			(ipTable, sindy::schema::adam_v2::pair_updown_link::kUpdownLinkGID1		),
			updown_link_gid2			(ipTable, sindy::schema::adam_v2::pair_updown_link::kUpdownLinkGID2		)
		{}

		~PairUpdownLink(){}
	};

	/* 
	 * 上下線リンクグループ
	 */
	class UpdownLinkGroup : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled group_c	;	///< グループ種別コード
	public:
		UpdownLinkGroup(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			group_c			(ipTable, sindy::schema::adam_v2::updown_link_group::kGroupC			)
		{}

		~UpdownLinkGroup(){}
	};

	/* 
	 * 上下線リンク－車線リンク関連
	 */
	class RelUpdownLinkLaneLink : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled updown_link_gid	;	///< 上下線リンクGID
		SchemaFiled lane_link_gid	;	///< 車線リンクGID
	public:
		RelUpdownLinkLaneLink(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			updown_link_gid			(ipTable, sindy::schema::adam_v2::rel_updown_link_lane_link::kUpdownLinkGID		),
			lane_link_gid			(ipTable, sindy::schema::adam_v2::rel_updown_link_lane_link::kLaneLinkGID		)
		{}

		~RelUpdownLinkLaneLink(){}
	};

	/* 
	 * 道路リンク－車線リンク関連
	 */
	class RelRoadLinkLaneLink : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled road_link_id			;	///< 道路リンクOID
		SchemaFiled lane_link_gid			;	///< 車線リンクGID
		SchemaFiled road_link_direction_c	;	///< 道路リンク方向コード
	public:
		RelRoadLinkLaneLink(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			road_link_id			(ipTable, sindy::schema::adam_v2::rel_road_link_lane_link::kRoadLinkID			),
			lane_link_gid			(ipTable, sindy::schema::adam_v2::rel_road_link_lane_link::kLaneLinkGID			),
			road_link_direction_c	(ipTable, sindy::schema::adam_v2::rel_road_link_lane_link::kRoadLinkDirectionC	)
		{}

		~RelRoadLinkLaneLink(){}
	};

	/* 
	 * 車線リンクグループ
	 */
	class LaneLinkGroup : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled group_c					;	///< グループ種別コード
		SchemaFiled updown_link_group_id	;	///< 上下線リンクグループOID
	public:
		LaneLinkGroup(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			group_c					(ipTable, sindy::schema::adam_v2::lane_link_group::kGroupC				),
			updown_link_group_id	(ipTable, sindy::schema::adam_v2::lane_link_group::kUpDownLinkGroupID	)
		{}

		~LaneLinkGroup(){}
	};

	/* 
	 * 区画線リンク－車線リンク関連
	 */
	class RelCompartLinkLaneLink : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled compart_link_gid			;	///< 区画線リンクGID
		SchemaFiled lane_link_gid				;	///< 車線リンクGID
		SchemaFiled compart_link_direction_c	;	///< 区画線リンク方向コード
		SchemaFiled lr_c						;	///< 左右判定コード
		SchemaFiled sequence					;	///< 順番
	public:
		RelCompartLinkLaneLink(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			compart_link_gid			(ipTable, sindy::schema::adam_v2::rel_compart_link_lane_link::kCompartLinkGID		),
			lane_link_gid				(ipTable, sindy::schema::adam_v2::rel_compart_link_lane_link::kLaneLinkGID			),
			compart_link_direction_c	(ipTable, sindy::schema::adam_v2::rel_compart_link_lane_link::kCompartLinkDirectionC	),
			lr_c						(ipTable, sindy::schema::adam_v2::rel_compart_link_lane_link::kLRC					),
			sequence					(ipTable, sindy::schema::adam_v2::rel_compart_link_lane_link::kSequence				)
		{}

		~RelCompartLinkLaneLink(){}
	};
	
	/* 
	 * 道路境界線リンク－車線リンク関連
	 */
	class RelBorderLinkLaneLink : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled border_link_gid			;	///< 道路境界線リンクGID
		SchemaFiled lane_link_gid			;	///< 車線リンクGID
		SchemaFiled border_link_direction_c	;	///< 道路境界線リンク方向コード
		SchemaFiled lr_c					;	///< 左右判定コード
		SchemaFiled sequence				;	///< 順番
	public:
		RelBorderLinkLaneLink(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			border_link_gid				(ipTable, sindy::schema::adam_v2::rel_border_link_lane_link::kBorderLinkGID			),
			lane_link_gid				(ipTable, sindy::schema::adam_v2::rel_border_link_lane_link::kLaneLinkGID			),
			border_link_direction_c		(ipTable, sindy::schema::adam_v2::rel_border_link_lane_link::kBorderLinkDirectionC	),
			lr_c						(ipTable, sindy::schema::adam_v2::rel_border_link_lane_link::kLRC					),
			sequence					(ipTable, sindy::schema::adam_v2::rel_border_link_lane_link::kSequence				)
		{}

		~RelBorderLinkLaneLink(){}
	};
	
	/* 
	 * 上下線ノード－信号機ポイント関連
	 */
	class RelUpdoenNodeSignalPoint : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled updown_node_gid		;	///< 上下線ノードGID
		SchemaFiled signal_point_gid	;	///< 信号機ポイントGID
	public:
		RelUpdoenNodeSignalPoint(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			updown_node_gid			(ipTable, sindy::schema::adam_v2::rel_updown_node_signal_point::kUpdownNodeGID		),
			signal_point_gid		(ipTable, sindy::schema::adam_v2::rel_updown_node_signal_point::kSignalPointGID		)
		{}

		~RelUpdoenNodeSignalPoint(){}
	};
	
	/* 
	 * 車線ノード－信号機ポイント関連
	 */
	class RelLaneNodeSignalPoint : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled lane_node_gid		;	///< 車線ノードGID
		SchemaFiled signal_point_gid	;	///< 信号機ポイントGID
	public:
		RelLaneNodeSignalPoint(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			lane_node_gid			(ipTable, sindy::schema::adam_v2::rel_lane_node_signal_point::kLaneNodeGID		),
			signal_point_gid		(ipTable, sindy::schema::adam_v2::rel_lane_node_signal_point::kSignalPointGID	)
		{}

		~RelLaneNodeSignalPoint(){}
	};
	
	/* 
	 * 車線ノード－横断歩道ライン関連
	 */
	class RelLaneNodeCrossWalkLine : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled lane_node_gid		;	///< 車線ノードGID
		SchemaFiled crosswalk_line_gid	;	///< 横断歩道ラインGID
	public:
		RelLaneNodeCrossWalkLine(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			lane_node_gid			(ipTable, sindy::schema::adam_v2::rel_lane_node_crosswalk_line::kLaneNodeGID			),
			crosswalk_line_gid		(ipTable, sindy::schema::adam_v2::rel_lane_node_crosswalk_line::kCrosswalkLineGID	)
		{}

		~RelLaneNodeCrossWalkLine(){}
	};
	
	/* 
	 * 車線ノード－道路標識ポイント関連
	 */
	class RelLaneNodeRoadSignPoint : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled lane_node_gid		;	///< 車線ノードGID
		SchemaFiled roadsign_point_gid	;	///< 道路標識ポイントGID
	public:
		RelLaneNodeRoadSignPoint(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			lane_node_gid			(ipTable, sindy::schema::adam_v2::rel_lane_node_roadsign_point::kLaneNodeGID			),
			roadsign_point_gid		(ipTable, sindy::schema::adam_v2::rel_lane_node_roadsign_point::kRoadsignPointGID	)
		{}

		~RelLaneNodeRoadSignPoint(){}
	};

	/* 
	 * 道路標示エリア－車線リンク関連
	 */
	class RelRoadMarkAreaLaneLink : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled roadmark_area_gid		;	///< 道路標示エリアGID
		SchemaFiled lane_link_gid			;	///< 車線リンクGID
	public:
		RelRoadMarkAreaLaneLink(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			roadmark_area_gid		(ipTable, sindy::schema::adam_v2::rel_roadmark_area_lane_link::kRoadMarkAreaGID	),
			lane_link_gid			(ipTable, sindy::schema::adam_v2::rel_roadmark_area_lane_link::kLaneLinkGID		)
		{}

		~RelRoadMarkAreaLaneLink(){}
	};
	
	/* 
	 * 上下線ノード列
	 */
	class NQUpdownNode : public AdamNodeQueue
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled updown_link_gid	;	///< 上下線リンクGID
		SchemaFiled updown_node_gid	;	///< 上下線ノードGID
		SchemaFiled width			;	///< 幅員
	public:
		NQUpdownNode(const ITablePtr& ipTable) : 
			AdamNodeQueue	(ipTable),
			updown_link_gid		(ipTable, sindy::schema::adam_v2::nq_updown_node::kUpdownLinkGID	),
			updown_node_gid		(ipTable, sindy::schema::adam_v2::nq_updown_node::kUpdownNodeGID	),
			width				(ipTable, sindy::schema::adam_v2::nq_updown_node::kWidth			)
		{}

		~NQUpdownNode(){}
	};
	
	/* 
	 * 車線ノード列
	 */
	class NQLaneNode : public AdamNodeQueue
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled lane_link_gid			;	///< 車線リンクGID
		SchemaFiled lane_node_gid			;	///< 車線ノードGID
		SchemaFiled linear_element_c		;	///< 線形要素属性コード
		SchemaFiled width					;	///< 幅員
		SchemaFiled curvature				;	///< 曲率
		SchemaFiled curvature_radius		;	///< 曲率半径
		SchemaFiled curvature_change_rate	;	///< 曲率変化率
		SchemaFiled clothoid_parameter		;	///< クロソイドパラメータ
		SchemaFiled longitudinal_slope		;	///< 縦断勾配
		SchemaFiled cross_slope				;	///< 横断勾配
	public:
		NQLaneNode(const ITablePtr& ipTable) : 
			AdamNodeQueue			(ipTable),
			lane_link_gid			(ipTable, sindy::schema::adam_v2::nq_lane_node::kLaneLinkGID			),
			lane_node_gid			(ipTable, sindy::schema::adam_v2::nq_lane_node::kLaneNodeGID			),
			linear_element_c		(ipTable, sindy::schema::adam_v2::nq_lane_node::kLinearElement		),
			width					(ipTable, sindy::schema::adam_v2::nq_lane_node::kWidth				),
			curvature				(ipTable, sindy::schema::adam_v2::nq_lane_node::kCurvature			),
			curvature_radius		(ipTable, sindy::schema::adam_v2::nq_lane_node::kCurvatureRadius		),
			curvature_change_rate	(ipTable, sindy::schema::adam_v2::nq_lane_node::kCurvatureChangeRate	),
			clothoid_parameter		(ipTable, sindy::schema::adam_v2::nq_lane_node::kClothoidParameter	),
			longitudinal_slope		(ipTable, sindy::schema::adam_v2::nq_lane_node::kLongitudinalSlope	),
			cross_slope				(ipTable, sindy::schema::adam_v2::nq_lane_node::kCrossSlope			)
		{}

		~NQLaneNode(){}
	};
	
	/* 
	 * 区画線ノード列
	 */
	class NQCompartNode : public AdamNodeQueue
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled compart_link_gid	;	///< 区画線リンクGID
		SchemaFiled compart_node_gid	;	///< 区画線ノードGID
		SchemaFiled width				;	///< 線幅
	public:
		NQCompartNode(const ITablePtr& ipTable) : 
			AdamNodeQueue	(ipTable),
			compart_link_gid	(ipTable, sindy::schema::adam_v2::nq_compart_node::kCompartLinkGID	),
			compart_node_gid	(ipTable, sindy::schema::adam_v2::nq_compart_node::kCompartNodeGID	),
			width				(ipTable, sindy::schema::adam_v2::nq_compart_node::kWidth			)
		{}

		~NQCompartNode(){}
	};
	
	/* 
	 * 道路境界線ノード列
	 */
	class NQBorderNode : public AdamNodeQueue
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled border_link_gid	;	///< 道路境界線リンクGID
		SchemaFiled border_node_gid	;	///< 道路境界線ノードGID
	public:
		NQBorderNode(const ITablePtr& ipTable) : 
			AdamNodeQueue	(ipTable),
			border_link_gid		(ipTable, sindy::schema::adam_v2::nq_border_node::kBorderLinkGID	),
			border_node_gid		(ipTable, sindy::schema::adam_v2::nq_border_node::kBorderNodeGID	)
		{}

		~NQBorderNode(){}
	};

	/* 
	 * ADAMバージョン
	 * @note	インポート対象外のため定義のみ
	 */
	class AdamVersion
	{
	public:
		static const uh::tstring	TableName;	///< テーブル名

		SchemaFiled	objectid		;	///< OBJECTID
	public:
		AdamVersion(const ITablePtr& ipTable) : 
			objectid		(ipTable, sindy::schema::adam_v2::adam_version::kObjectID	)
		{}

		~AdamVersion(){}
	};

public:
	//データ実体
	//インポート対象外はコメントアウトしておく
	//IFeatureClassPtr	featureUpdownLink;				///< 上下線リンク フィーチャ
	//IFeatureClassPtr	featureUpdownNode;				///< 上下線ノード フィーチャ
	IFeatureClassPtr	featureLaneLink;				///< 車線リンク フィーチャ
	IFeatureClassPtr	featureLaneNode;				///< 車線ノード フィーチャ
	IFeatureClassPtr	featureCompartLink;				///< 区画線リンク フィーチャ
	IFeatureClassPtr	featureCompartNode;				///< 区画線ノード フィーチャ
	IFeatureClassPtr	featureBorderLink;				///< 境界線リンク フィーチャ
	IFeatureClassPtr	featureBorderNode;				///< 境界線ノード フィーチャ
	IFeatureClassPtr	featureSignalPoint;				///< 信号機ポイント フィーチャ
	IFeatureClassPtr	featureCrossWalkLine;			///< 横断歩道ポイント フィーチャ
	IFeatureClassPtr	featureRoadSignPoint;			///< 道路標識ポイント フィーチャ
	//IFeatureClassPtr	featureMaintenanceArea;			///< 整備済みエリア フィーチャ
	//IFeatureClassPtr	featureRoadMarkArea;			///< 道路標示エリア フィーチャ
	IFeatureClassPtr	featureVirtualCompartLine;		///< 仮想区画線ライン フィーチャ
	IFeatureClassPtr	featureCoveringArea;			///< 覆い物エリア フィーチャ

	//ITablePtr			tableRelRoadLinkUpdownLink;		///< 道路リンク－上下線リンク関連 テーブル
	//ITablePtr			tablePairUpdownLink;			///< 上下線ペアリンク テーブル
	//ITablePtr			tableUpdownLinkGroup;			///< 上下線リンクグループ テーブル
	//ITablePtr			tableRelUpdownLinkLaneLink;		///< 上下線リンク－車線リンク関連 テーブル
	ITablePtr			tableRelRoadLinkLaneLink;		///< 道路リンク－車線リンク関連 テーブル
	ITablePtr			tableLaneLinkGroup;				///< 車線リンクグループ テーブル
	ITablePtr			tableRelCompartLinkLaneLink;	///< 区画線リンク－車線リンク関連 テーブル
	ITablePtr			tableRelBorderLinkLaneLink;		///< 道路境界線リンク－車線リンク関連 テーブル
	//ITablePtr			tableRelUpdoenNodeSignalPoint;	///< 上下線ノード－信号機ポイント関連 テーブル
	ITablePtr			tableRelLaneNodeSignalPoint;	///< 車線ノード－信号機ポイント関連 テーブル
	ITablePtr			tableRelLaneNodeCrossWalkLine;	///< 車線ノード－横断歩道ライン関連 テーブル
	ITablePtr			tableRelLaneNodeRoadSignPoint;	///< 車線ノード－道路標識ポイント関連 テーブル
	//ITablePtr			tableRelRoadMarkAreaLaneLink;	///< 車線リンク－道路標示エリア関連 テーブル
	//ITablePtr			tableNQUpdownNode;				///< 上下線ノード列 テーブル
	ITablePtr			tableNQLaneNode;				///< 車線ノード列 テーブル
	ITablePtr			tableNQCompartNode;				///< 区画線ノード列 テーブル
	ITablePtr			tableNQBorderNode;				///< 道路境界線ノード列 テーブル
	ITablePtr			tableAdamVersion;				///< ADAMバージョン

};
typedef std::shared_ptr<AdamSchema> AdamSchemaPtr;

};	//namespace adam_v2