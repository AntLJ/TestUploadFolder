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

#include "FieldType.h"

namespace adam_v2
{
	/**
	 * 更新コード
	 */
	namespace update_code {
		enum ECode {
			kUnChange = 0,		//!< 変更なし
			kNew = 1,			//!< 新規追加
			kDelete = 2,		//!< 削除
			kUpdateAttr = 3,	//!< 属性変更
		};
	}

/**
 * 測量会社納入フォーマット
 *
 * @version	1.0.3
 * @see		\\win\dfs\部門横断PJ\ADAMデータ整備\40_整備ツール開発\01_オーサリングフォーマット
 */
class SurveyCSV
{
public:
	/**
	 * コンストラクタ
	 */
	SurveyCSV(void);
	/**
	 * デストラクタ
	 */
	virtual ~SurveyCSV(void);

	void Clear()
	{
		listLaneLink.clear();	
		listLanePoint.clear();
		listRelLaneLink.clear();
		listCompartLink.clear();
		listCompartPoint.clear();
		listRelCompartLink.clear();
		listBorderLink.clear();
		listBorderPoint.clear();
		listRelBorderLink.clear();
		listSignalPoint.clear();
		listRelSignalPoint.clear();
		listRoadSignPoint.clear();
		listRelRoadSignPoint.clear();
		listCrossWalkLine.clear();
		listRelCrossWalkLine.clear();
		listVirtualCompartLink.clear();
		listVirtualCompartPoint.clear();
		listRelVirtualCompartLink.clear();
		listCoveringArea.clear();
		listSpatialRef.clear();
		listVersion.clear();
	}

	/**
	 * オブジェクト基底クラス
	 */
	class BaseObject
	{
	public:
		FieldType<long long> objectid				;	///< 車線リンクID
	public:
		BaseObject() : 
			//						型										列番号	フィールド名		初期値				NULL許容	整数最大桁数			小数最大桁数						(option)(最小値		最大値) or 有効値
			objectid			(	FieldType<long long>					(	4,		_T("OBJECTID"),		1LL,			false,		10L,				0L,							1LL,		9999999999LL))
		{}
		virtual ~BaseObject(){}

		bool operator ==(const BaseObject& o)
		{
			return this->GetOID() == o.GetOID();
		}

		bool operator <(const BaseObject& o) const
		{
			if(this->GetOID() < o.GetOID())
			{
				return true;
			}

			return false;
		}
		static bool hasGeometry()
		{
			return true;
		}
	private:
		long long GetOID() const
		{
			return this->objectid.GetValue();
		}
	};

	/**
	 * オブジェクト基底クラス
	 */
	class BaseTable
	{
	public:
		FieldType<long long> objectid				;	///< 車線リンクID
	public:
		BaseTable() : 
			//						型											列番号	フィールド名		初期値				NULL許容	整数最大桁数			小数最大桁数						(option)(最小値		最大値) or 有効値
			objectid				(	FieldType<long long>					(	1,		_T("OBJECTID"),		1LL,			false,		10L,				0L,							1LL,		9999999999LL))
		{}
		virtual ~BaseTable(){}

		bool operator ==(const BaseTable& o)
		{
			return this->GetOID() == o.GetOID();
		}

		bool operator <(const BaseTable& o) const
		{
			if(this->GetOID() < o.GetOID())
			{
				return true;
			}

			return false;
		}
		static bool hasGeometry()
		{
			return false;
		}
	private:
		long long GetOID() const
		{
			return this->objectid.GetValue();
		}
	};

	/**
	 * 車線リンク CSV
	 * 
	 * lane_link.csv
	 */
	class LaneLink : public BaseObject
	{
	public:
		FieldType<Polyline> shape				;	///< 形状
		
		FieldType<long long> from_node_id		;	///< 始点ノードID
		FieldType<long long> to_node_id			;	///< 終点ノードID
		FieldType<long long> lane_group_id		;	///< 車線グループID
		FieldType<int> lane_group_c				;	///< 車線グループ種別コード
		FieldType<int> lane_no					;	///< 車線番号
		FieldType<int> lane_count				;	///< 車線数
		FieldType<int> max_legal_speed_c		;	///< 制限速度コード（最大）
		FieldType<int> min_legal_speed_c		;	///< 制限速度コード（最小）
		FieldType<int> travel_direction_c		;	///< 進行方向種別コード
		FieldType<int> road_structure_c			;	///< 道路構造種別コード
		FieldType<int> both_link_direction_f	;	///< 双方向通行可能フラグ
		FieldType<int> left_change_f			;	///< 左車線変更可能フラグ
		FieldType<int> right_change_f			;	///< 右車線変更可能フラグ
		FieldType<int> left_protrusion_f		;	///< 左はみ出し可能フラグ
		FieldType<int> right_protrusion_f		;	///< 右はみ出し可能フラグ
		FieldType<int> rubbing_f				;	///< 擦り付けフラグ
		FieldType<int> tunnel_f					;	///< トンネルフラグ
		FieldType<int> update_c					;	///< 更新コード

	public:
		LaneLink() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			shape					(	FieldType<Polyline>				(	1,		_T("LON,LAT,HEIGHT"),			Polyline(),	false,		0L,				0L														))
			,from_node_id			(	FieldType<long long>			(	5,		_T("FROM_NODE_ID"),				1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,to_node_id				(	FieldType<long long>			(	6,		_T("TO_NODE_ID"),				1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,lane_group_id			(	FieldType<long long>			(	7,		_T("LANE_GROUP_ID"),			1LL,		true,		10L,			0L,				1LL,		9999999999LL				))
			,lane_group_c			(	FieldType<int>					(	8,		_T("LANE_GROUP_C"),				1,			true,		5,				0,				DOMAIN_LANE_GROUP_C						))
			,lane_no				(	FieldType<int>					(	9,		_T("LANE_NO"),					0,			true,		2,				0,				0,		99								))
			,lane_count				(	FieldType<int>					(	10,		_T("LANE_COUNT"),				0,			true,		2,				0,				0,		99								))
			,max_legal_speed_c		(	FieldType<int>					(	11,		_T("MAX_LEGAL_SPEED_C"),		1001,		true,		5,				0,				DOMAIN_MAX_LEGAL_SPEED_C				))
			,min_legal_speed_c		(	FieldType<int>					(	12,		_T("MIN_LEGAL_SPEED_C"),		1001,		true,		5,				0,				DOMAIN_MIN_LEGAL_SPEED_C				))
			,travel_direction_c		(	FieldType<int>					(	13,		_T("TRAVEL_DIRECTION_C"),		1,			true,		5,				0,				DOMAIN_TRAVEL_DIRECTION_C				))
			,road_structure_c		(	FieldType<int>					(	14,		_T("ROAD_STRUCTURE_C"),			1,			true,		5,				0,				DOMAIN_ROAD_STRUCTURE_C					))
			,both_link_direction_f	(	FieldType<int>					(	15,		_T("BOTH_LINK_DIRECTION_F"),	0,			true,		1,				0,				DOMAIN_FLAG								))
			,left_change_f			(	FieldType<int>					(	16,		_T("LEFT_CHANGE_F"),			0,			true,		1,				0,				DOMAIN_FLAG								))
			,right_change_f			(	FieldType<int>					(	17,		_T("RIGHT_CHANGE_F"),			0,			true,		1,				0,				DOMAIN_FLAG								))
			,left_protrusion_f		(	FieldType<int>					(	18,		_T("LEFT_PROTRUSION_F"),		0,			true,		1,				0,				DOMAIN_FLAG								))
			,right_protrusion_f		(	FieldType<int>					(	19,		_T("RIGHT_PROTRUSION_F"),		0,			true,		1,				0,				DOMAIN_FLAG								))
			,rubbing_f				(	FieldType<int>					(	20,		_T("RUBBING_F"),				0,			true,		1,				0,				DOMAIN_FLAG								))
			,tunnel_f				(	FieldType<int>					(	21,		_T("TUNNEL_F"),					0,			true,		1,				0,				DOMAIN_FLAG								))
			,update_c				(	FieldType<int>					(	22,		_T("UPDATE_C"),					0,			false,		5,				0,				DOMAIN_UPDATE_C							))
		{}
		~LaneLink(){}
	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 車線ポイント CSV
	 * 
	 * lane_point.csv
	 */
	class LanePoint : public BaseObject
	{
	public:
		FieldType<Point> shape				;	///< 形状

		FieldType<long long>	node_id					;	///< 車線ノードID
		FieldType<int>			point_class_c			;	///< ポイント種別コード
		FieldType<long long>	lane_link_id			;	///< 車線リンクID
		FieldType<long long>	sequence				;	///< シーケンス番号
		FieldType<int>			lastnode_f				;	///< 終端ノードフラグ
		FieldType<int>			stop_point_f			;	///< 停止点フラグ
		FieldType<double>		width					;	///< 幅員
		FieldType<int>			linear_element_c		;	///< 線形要素属性コード
		FieldType<double>		curvature				;	///< 曲率
		FieldType<double>		curvature_radius		;	///< 曲率半径
		FieldType<double>		curvature_change_rate	;	///< 曲率変化率
		FieldType<double>		clothoid_parameter		;	///< クロソイドパラメータ
		FieldType<double>		longitudinal_slope		;	///< 縦断勾配
		FieldType<double>		cross_slope				;	///< 横断勾配
		FieldType<int>			cover_f					;	///< 覆い物フラグ
		FieldType<int>			update_c				;	///< 更新コード

	public:
		LanePoint() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			shape					(	FieldType<Point>				(	1,		_T("LON,LAT,HEIGHT"),			Point(),	false,		0L,				0L														))
			,node_id				(	FieldType<long long>			(	5,		_T("NODE_ID"),					1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,point_class_c			(	FieldType<int>					(	6,		_T("POINT_CLASS_C"),			1,			false,		5L,				0L,				DOMAIN_POINT_CLASS_C					))
			,lane_link_id			(	FieldType<long long>			(	7,		_T("LANE_LINK_ID"),				1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,sequence				(	FieldType<long long>			(	8,		_T("SEQUENCE"),					0LL,		false,		10L,			0L,				0LL,		9999999999LL				))
			,lastnode_f				(	FieldType<int>					(	9,		_T("LASTNODE_F"),				0,			false,		2L,				0L,				DOMAIN_FLAG								))
			,stop_point_f			(	FieldType<int>					(	10,		_T("STOP_POINT_F"),				0,			true,		2L,				0L,				DOMAIN_FLAG								))
			,width					(	FieldType<double>				(	11,		_T("WIDTH"),					0.0,		true,		5L,				3L,				0.0,		99999.999					))
			,linear_element_c		(	FieldType<int>					(	12,		_T("LINEAR_ELEMENT_C"),			1,			true,		5L,				0L,				DOMAIN_LINEAR_ELEMENT_C					))
			,curvature				(	FieldType<double>				(	13,		_T("CURVATURE"),				0.0,		true,		12L,			5L,				-1.0,		1.0							))
			,curvature_radius		(	FieldType<double>				(	14,		_T("CURVATURE_RADIUS"),			0.0,		true,		12L,			5L														))
			,curvature_change_rate	(	FieldType<double>				(	15,		_T("CURVATURE_CHANGE_RATE"),	0.0,		true,		12L,			5L														))
			,clothoid_parameter		(	FieldType<double>				(	16,		_T("CLOTHOID_PARAMETER"),		0.0,		true,		12L,			5L														))
			,longitudinal_slope		(	FieldType<double>				(	17,		_T("LONGITUDINAL_SLOPE"),		0.0,		true,		3L,				3L,				-999.999,		999.999					))
			,cross_slope			(	FieldType<double>				(	18,		_T("CROSS_SLOPE"),				0.0,		true,		3L,				3L,				-999.999,		999.999					))
			,cover_f				(	FieldType<int>					(	19,		_T("COVER_F"),					0,			true,		1L,				0L,				DOMAIN_FLAG								))
			,update_c				(	FieldType<int>					(	20,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
		{}
		~LanePoint(){}
	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 道路リンク－車線リンク 関連
	 * 
	 * rel_road_link_lane_link.csv
	 */
	class RelLaneLink : public BaseTable
	{
	public:
		FieldType<long long>	lane_link_id				;	///< 車線リンクID
		FieldType<long long>	road_link_id				;	///< 道路リンクID
		FieldType<int>			road_link_direction_c		;	///< 道路リンク方向コード
		FieldType<int>			update_c					;	///< 更新コード
	public:
		RelLaneLink() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			lane_link_id			(	FieldType<long long>			(	2,		_T("LANE_LINK_ID"),				0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,road_link_id			(	FieldType<long long>			(	3,		_T("ROAD_LINK_ID"),				0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,road_link_direction_c	(	FieldType<int>					(	4,		_T("ROAD_LINK_DIRECTION_C"),	1,			false,		5L,				0L,				DOMAIN_LINK_DIRECTION_C					))
			,update_c				(	FieldType<int>					(	5,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~RelLaneLink(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 区画線リンク CSV
	 * 
	 * compartment_link.csv
	 */
	class CompartLink : public BaseObject
	{
	public:
		FieldType<Polyline> shape				;	///< 形状

		FieldType<long long>	from_node_id		;	///< 始点ノードID
		FieldType<long long>	to_node_id			;	///< 終点ノードID
		FieldType<int>			compart_line_c		;	///< 線種別コード
		FieldType<int>			compart_style_c		;	///< 区画線設置様式コード
		FieldType<int>			slowdown_label_c	;	///< 減速表示フラグ
		FieldType<int>			installation_f		;	///< 設置物フラグ
		FieldType<int>			update_c			;	///< 更新コード

	public:
		CompartLink() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			shape					(	FieldType<Polyline>				(	1,		_T("LON,LAT,HEIGHT"),			Polyline(),	false,		0L,				0L														))
			,from_node_id			(	FieldType<long long>			(	5,		_T("FROM_NODE_ID"),				1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,to_node_id				(	FieldType<long long>			(	6,		_T("TO_NODE_ID"),				1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,compart_line_c			(	FieldType<int>					(	7,		_T("COMPART_LINE_C"),			1,			true,		5L,				0L,				DOMAIN_COMPART_LINE_C					))
			,compart_style_c		(	FieldType<int>					(	8,		_T("COMPART_STYLE_C"),			1,			true,		5L,				0L,				DOMAIN_COMPART_STYLE_C					))
			,slowdown_label_c		(	FieldType<int>					(	9,		_T("SLOWDOWN_LABEL_C"),			0,			true,		5L,				0L,				DOMAIN_COMPART_SLOW_C					))
			,installation_f			(	FieldType<int>					(	10,		_T("INSTALLATION_F"),			0,			true,		5L,				0L,				DOMAIN_FLAG								))
			,update_c				(	FieldType<int>					(	11,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~CompartLink(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 区画線ポイント CSV
	 * 
	 * compartment_point.csv
	 */
	class CompartPoint : public BaseObject
	{
	public:
		FieldType<Point> shape				;	///< 形状

		FieldType<long long>	node_id				;	///< 区画線ノードID
		FieldType<int>			point_class_c		;	///< ポイント種別コード
		FieldType<long long>	compart_link_id		;	///< 区画線リンクID
		FieldType<long long>	sequence			;	///< シーケンス番号
		FieldType<int>			lastnode_f			;	///< 終端ノードフラグ
		FieldType<double>		width				;	///< 幅員
		FieldType<int>			update_c			;	///< 更新コード

	public:
		CompartPoint() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			shape					(	FieldType<Point>				(	1,		_T("LON,LAT,HEIGHT"),			Point(),	false,		0L,				0L														))
			,node_id				(	FieldType<long long>			(	5,		_T("NODE_ID"),					1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,point_class_c			(	FieldType<int>					(	6,		_T("POINT_CLASS_C"),			1,			false,		5L,				0L,				DOMAIN_POINT_CLASS_C					))
			,compart_link_id		(	FieldType<long long>			(	7,		_T("COMPART_LINK_ID"),			1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,sequence				(	FieldType<long long>			(	8,		_T("SEQUENCE"),					0LL,		false,		10L,			0L,				0LL,		9999999999LL				))
			,lastnode_f				(	FieldType<int>					(	9,		_T("LASTNODE_F"),				0,			false,		2L,				0L,				DOMAIN_FLAG								))
			,width					(	FieldType<double>				(	10,		_T("WIDTH"),					0.0,		true,		5L,				3L,				0,			99999.999					))
			,update_c				(	FieldType<int>					(	11,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
		{}
		~CompartPoint(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 車線リンク－区画線リンク 関連
	 * 
	 * rel_lane_link_compartment_link.csv
	 */
	class RelCompartLink : public BaseTable
	{
	public:
		FieldType<long long>	lane_link_id				;	///< 車線リンクID
		FieldType<int>			lr_f						;	///< 左右判定フラグ
		FieldType<long long>	compart_link_id				;	///< 区画線リンクID
		FieldType<int>			compart_link_direction_c	;	///< 区画線リンク方向コード
		FieldType<long long>	sequence					;	///< シーケンス番号
		FieldType<int>			update_c					;	///< 更新コード

	public:
		RelCompartLink() : 
			//								型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			lane_link_id					(	FieldType<long long>			(	2,		_T("LANE_LINK_ID"),				1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,lr_f							(	FieldType<int>					(	3,		_T("LR_F"),						0,			true,		1L,				0L,				DOMAIN_FLAG								))
			,compart_link_id				(	FieldType<long long>			(	4,		_T("COMPART_LINK_ID"),			1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,compart_link_direction_c		(	FieldType<int>					(	5,		_T("COMPART_LINK_DIRECTION_C"),	1,			true,		5L,				0L,				DOMAIN_LINK_DIRECTION_C					))
			,sequence						(	FieldType<long long>			(	6,		_T("SEQUENCE"),					0LL,		true,		10L,			0L,				0LL,		9999999999LL				))
			,update_c						(	FieldType<int>					(	7,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
		{}
		~RelCompartLink(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 境界線リンク CSV
	 * 
	 * border_link.csv
	 */
	class BorderLink : public BaseObject
	{
	public:
		FieldType<Polyline> shape				;	///< 形状

		FieldType<long long>	from_node_id		;	///< 始点ノードID
		FieldType<long long>	to_node_id			;	///< 終点ノードID
		FieldType<int>			imaginary_boder_f	;	///< 仮想線フラグ
		FieldType<int>			update_c			;	///< 更新コード

	public:
		BorderLink() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			shape					(	FieldType<Polyline>				(	1,		_T("LON,LAT,HEIGHT"),			Polyline(),	false,		0L,				0L														))
			,from_node_id			(	FieldType<long long>			(	5,		_T("FROM_NODE_ID"),				0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,to_node_id				(	FieldType<long long>			(	6,		_T("TO_NODE_ID"),				0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,imaginary_boder_f		(	FieldType<int>					(	7,		_T("IMAGINARY_BODER_F"),		0,			true,		1L,				0L,				DOMAIN_FLAG								))
			,update_c				(	FieldType<int>					(	8,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
		{}
		~BorderLink(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 境界線ポイント
	 * 
	 * border_point.csv
	 */
	class BorderPoint : public BaseObject
	{
	public:
		FieldType<Point>		shape			;	///< 形状

		FieldType<long long>	node_id			;	///< 境界線ノードID
		FieldType<int>			point_class_c	;	///< ポイント種別コード
		FieldType<long long>	border_link_id	;	///< 境界線リンクID
		FieldType<long long>	sequence		;	///< シーケンス番号
		FieldType<int>			lastnode_f		;	///< 終端ノードフラグ
		FieldType<int>			update_c		;	///< 更新コード

	public:
		BorderPoint() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			shape					(	FieldType<Point>				(	1,		_T("LON,LAT,HEIGHT"),			Point(),	false,		0L,				0L														))
			,node_id				(	FieldType<long long>			(	5,		_T("NODE_ID"),					1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,point_class_c			(	FieldType<int>					(	6,		_T("POINT_CLASS_C"),			1,			false,		5L,				0L,				DOMAIN_POINT_CLASS_C					))
			,border_link_id			(	FieldType<long long>			(	7,		_T("COMPART_LINK_ID"),			1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,sequence				(	FieldType<long long>			(	8,		_T("SEQUENCE"),					0LL,		false,		10L,			0L,				0LL,		9999999999LL				))
			,lastnode_f				(	FieldType<int>					(	9,		_T("LASTNODE_F"),				0,			false,		2L,				0L,				DOMAIN_FLAG								))
			,update_c				(	FieldType<int>					(	10,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~BorderPoint(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 車線リンク－境界線リンク 関連
	 * 
	 * rel_border_link.csv
	 */
	class RelBorderLink : public BaseTable
	{
	public:
		FieldType<long long>	lane_link_id				;	///< 車線リンクID
		FieldType<int>			lr_f						;	///< 左右判定フラグ
		FieldType<long long>	border_link_id				;	///< 境界線リンクID
		FieldType<int>			border_link_direction_c		;	///< 境界線リンク方向コード
		FieldType<long long>	sequence					;	///< シーケンス番号
		FieldType<int>			update_c					;	///< 更新コード

	public:
		RelBorderLink() : 
			//								型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			lane_link_id					(	FieldType<long long>			(	2,		_T("LANE_LINK_ID"),				1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,lr_f							(	FieldType<int>					(	3,		_T("LR_F"),						0,			true,		1L,				0L,				DOMAIN_FLAG								))
			,border_link_id					(	FieldType<long long>			(	4,		_T("BORDER_LINK_ID"),			1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,border_link_direction_c		(	FieldType<int>					(	5,		_T("BORDER_LINK_DIRECTION_C"),	1,			true,		5L,				0L,				DOMAIN_LINK_DIRECTION_C					))
			,sequence						(	FieldType<long long>			(	6,		_T("SEQUENCE"),					0LL,		true,		10L,			0L,				0LL,		9999999999LL				))
			,update_c						(	FieldType<int>					(	7,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~RelBorderLink(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 信号機ポイント
	 * 
	 * signal_point.csv
	 */
	class SignalPoint : public BaseObject
	{
	public:
		FieldType<Point>	shape				;	///< 形状

		FieldType<double>	horizontal_angle	;	///< 水平角
		FieldType<int>		signal_mold_c		;	///< 信号機形状コード
		FieldType<int>		lamp_count			;	///< 信号灯数
		FieldType<int>		arrow_f				;	///< 矢印信号フラグ
		FieldType<int>		warning_f			;	///< 予告信号フラグ
		FieldType<int>		independent_f		;	///< 独立信号フラグ
		FieldType<int>		update_c			;	///< 更新コード

	public:
		SignalPoint() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			shape					(	FieldType<Point>				(	1,		_T("LON,LAT,HEIGHT"),			Point(),	false,		0L,				0L														))
			,horizontal_angle		(	FieldType<double>				(	5,		_T("HORIZONTAL_ANGLE"),			0.0,		true,		3L,				3L,				0.000,		359.999						))
			,signal_mold_c			(	FieldType<int>					(	6,		_T("SIGNAL_MOLD_C"),			1,			true,		5L,				0L,				DOMAIN_SIGNAL_MOLD_C					))
			,lamp_count				(	FieldType<int>					(	7,		_T("LAMP_COUNT"),				1,			true,		2L,				0L,				1LL,		99LL						))
			,arrow_f				(	FieldType<int>					(	8,		_T("ARROW_F"),					1,			true,		1L,				0L,				DOMAIN_FLAG								))
			,warning_f				(	FieldType<int>					(	9,		_T("WARNING_F"),				1,			true,		1L,				0L,				DOMAIN_FLAG								))
			,independent_f			(	FieldType<int>					(	10,		_T("INDEPENDENT_F"),			1,			true,		1L,				0L,				DOMAIN_FLAG								))
			,update_c				(	FieldType<int>					(	11,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~SignalPoint(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 車線ノード－信号機ポイント 関連
	 * 
	 * rel_lane_point_signal_point.csv
	 */
	class RelSignalPoint : public BaseTable
	{
	public:
		FieldType<long long>	lane_node_id		;	///< 車線ノードID
		FieldType<long long>	signal_point_id	;	///< 信号機ポイントID
		FieldType<int>			update_c		;	///< 更新コード

	public:
		RelSignalPoint() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			lane_node_id			(	FieldType<long long>			(	2,		_T("LANE_NODE_ID"),				0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,signal_point_id		(	FieldType<long long>			(	3,		_T("SIGNAL_POINT_ID"),			0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,update_c				(	FieldType<int>					(	4,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~RelSignalPoint(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 道路標識ポイント
	 * 
	 * roadsign_point.csv
	 */
	class RoadSignPoint : public BaseObject
	{
	public:
		FieldType<Point>		shape				;	///< 形状

		FieldType<double>		horizontal_angle	;	///< 水平角
		FieldType<uh::tstring>	roadsign_c			;	///< 道路標識コード
		FieldType<int>			variable_f			;	///< 可変フラグ
		FieldType<int>			update_c			;	///< 更新コード

	public:
		RoadSignPoint() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			shape					(	FieldType<Point>				(	1,		_T("LON,LAT,HEIGHT"),			Point(),	false,		0L,				0L														))
			,horizontal_angle		(	FieldType<double>				(	5,		_T("HORIZONTAL_ANGLE"),			0.0,		true,		3L,				3L,				0.000,		359.999						))
			,roadsign_c				(	FieldType<uh::tstring>			(	6,		_T("ROADSIGN_C"),				_T(""),		true,		32L,			0L,				DOMAIN_ROADSIGN_C						))
			,variable_f				(	FieldType<int>					(	7,		_T("VARIABLE_F"),				1,			true,		1L,				0L,				DOMAIN_FLAG								))
			,update_c				(	FieldType<int>					(	8,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~RoadSignPoint(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 車線ノード－道路標識ポイント 関連
	 * 
	 * rel_lane_point_roadsign_point.csv
	 */
	class RelRoadSignPoint : public BaseTable
	{
	public:
		FieldType<long long>	lane_node_id		;	///< 車線ノードID
		FieldType<long long>	roadsign_point_id	;	///< 道路標識ポイントID
		FieldType<int>			update_c			;	///< 更新コード

	public:
		RelRoadSignPoint() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			lane_node_id			(	FieldType<long long>			(	2,		_T("LANE_NODE_ID"),				0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,roadsign_point_id		(	FieldType<long long>			(	3,		_T("ROADSIGN_POINT_ID"),		0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,update_c				(	FieldType<int>					(	4,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~RelRoadSignPoint(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 横断歩道ライン
	 * 
	 * crosswalk_line.csv
	 */
	class CrossWalkLine : public BaseObject
	{
	public:
		FieldType<Polyline> shape				;	///< 形状

		FieldType<double> width				;	///< 幅
		FieldType<int>	update_c			;	///< 更新コード
	public:
		CrossWalkLine() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			shape					(	FieldType<Polyline>				(	1,		_T("LON,LAT,HEIGHT"),			Polyline(),	false,		0L,				0L														))
			,width					(	FieldType<double>				(	5,		_T("WIDTH"),					0.0,		true,		3L,				3L,				0.000,		999.999						))
			,update_c				(	FieldType<int>					(	6,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~CrossWalkLine(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 車線ノード－横断歩道ライン 関連
	 * 
	 * rel_lane_point_crosswalk_line.csv
	 */
	class RelCrossWalkLine : public BaseTable
	{
	public:
		FieldType<long long> lane_node_id		;	///< 車線ノードID
		FieldType<long long> crosswalk_line_id	;	///< 横断歩道ラインID
		FieldType<int>	update_c				;	///< 更新コード

	public:
		RelCrossWalkLine() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			lane_node_id		(	FieldType<long long>			(	2,		_T("LANE_NODE_ID"),					0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,crosswalk_line_id	(	FieldType<long long>			(	3,		_T("CROSSWALK_LINE_ID"),			0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,update_c			(	FieldType<int>					(	4,		_T("UPDATE_C"),						0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~RelCrossWalkLine(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 仮想線リンク CSV
	 * 
	 * virtual_compartment_link.csv
	 */
	class VirtualCompartLink : public BaseObject
	{
	public:
		FieldType<Polyline> shape				;	///< 形状

		FieldType<long long>	from_node_id		;	///< 始点ノードID
		FieldType<long long>	to_node_id			;	///< 終点ノードID
		FieldType<int>			compart_line_c		;	///< 線種別コード
		FieldType<int>			compart_style_c		;	///< 区画線設置様式コード
		FieldType<int>			slowdown_label_c	;	///< 減速表示フラグ
		FieldType<int>			installation_f		;	///< 設置物フラグ

	public:
		VirtualCompartLink() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			shape					(	FieldType<Polyline>				(	1,		_T("LON,LAT,HEIGHT"),			Polyline(),	false,		0L,				0L														))
			,from_node_id			(	FieldType<long long>			(	5,		_T("FROM_NODE_ID"),				1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,to_node_id				(	FieldType<long long>			(	6,		_T("TO_NODE_ID"),				1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,compart_line_c			(	FieldType<int>					(	7,		_T("COMPART_LINE_C"),			1,			true,		5L,				0L,				DOMAIN_COMPART_LINE_C					))
			,compart_style_c		(	FieldType<int>					(	8,		_T("COMPART_STYLE_C"),			1,			true,		5L,				0L,				DOMAIN_COMPART_STYLE_C					))
			,slowdown_label_c		(	FieldType<int>					(	9,		_T("SLOWDOWN_LABEL_C"),			0,			true,		5L,				0L,				DOMAIN_COMPART_SLOW_C					))
			,installation_f			(	FieldType<int>					(	10,		_T("INSTALLATION_F"),			0,			true,		5L,				0L,				DOMAIN_FLAG								))
			{}
		~VirtualCompartLink(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 仮想線ポイント CSV
	 * 
	 * virtual_compartment_point.csv
	 */
	class VirtualCompartPoint : public BaseObject
	{
	public:
		FieldType<Point> shape				;	///< 形状

		FieldType<long long>	node_id				;	///< 仮想線ノードID
		FieldType<int>			point_class_c		;	///< ポイント種別コード
		FieldType<long long>	v_compart_link_id		;	///< 仮想線リンクID
		FieldType<long long>	sequence			;	///< シーケンス番号
		FieldType<int>			lastnode_f			;	///< 終端ノードフラグ
		FieldType<double>		width				;	///< 幅員

	public:
		VirtualCompartPoint() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			shape					(	FieldType<Point>				(	1,		_T("LON,LAT,HEIGHT"),			Point(),	false,		0L,				0L														))
			,node_id				(	FieldType<long long>			(	5,		_T("NODE_ID"),					1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,point_class_c			(	FieldType<int>					(	6,		_T("POINT_CLASS_C"),			1,			false,		5L,				0L,				DOMAIN_POINT_CLASS_C					))
			,v_compart_link_id		(	FieldType<long long>			(	7,		_T("V_COMPART_LINK_ID"),		1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,sequence				(	FieldType<long long>			(	8,		_T("SEQUENCE"),					0LL,		false,		10L,			0L,				0LL,		9999999999LL				))
			,lastnode_f				(	FieldType<int>					(	9,		_T("LASTNODE_F"),				0,			false,		2L,				0L,				DOMAIN_FLAG								))
			,width					(	FieldType<double>				(	10,		_T("WIDTH"),					0.0,		true,		5L,				3L,				0,			99999.999					))
		{}
		~VirtualCompartPoint(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 車線リンク－仮想線リンク 関連
	 * 
	 * rel_lane_link_virtual_compartment_link.csv
	 */
	class RelVirtualCompartLink : public BaseTable
	{
	public:
		FieldType<long long>	lane_link_id				;	///< 車線リンクID
		FieldType<int>			lr_f						;	///< 左右判定フラグ
		FieldType<long long>	v_compart_link_id				;	///< 仮想線リンクID
		FieldType<int>			v_compart_link_direction_c	;	///< 仮想線リンク方向コード
		FieldType<long long>	sequence					;	///< シーケンス番号

	public:
		RelVirtualCompartLink() : 
			//								型									列番号		フィールド名						初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			lane_link_id					(	FieldType<long long>			(	2,		_T("LANE_LINK_ID"),					1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,lr_f							(	FieldType<int>					(	3,		_T("LR_F"),							0,			true,		1L,				0L,				DOMAIN_FLAG								))
			,v_compart_link_id				(	FieldType<long long>			(	4,		_T("V_COMPART_LINK_ID"),			1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,v_compart_link_direction_c		(	FieldType<int>					(	5,		_T("V_COMPART_LINK_DIRECTION_C"),	1,			true,		5L,				0L,				DOMAIN_LINK_DIRECTION_C					))
			,sequence						(	FieldType<long long>			(	6,		_T("SEQUENCE"),						0LL,		true,		10L,			0L,				0LL,		9999999999LL				))
		{}
		~RelVirtualCompartLink(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 覆い物エリア CSV
	 * 
	 * covering_area.csv
	 */
	class CoveringArea : public BaseObject
	{
	public:
		FieldType<Polygon> shape				;	///< 形状
		FieldType<int>	update_c				;	///< 更新コード

	public:
		CoveringArea() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			shape					(	FieldType<Polygon>				(	1,		_T("LON,LAT,HEIGHT"),			Polygon(),	false,		0L,				0L														))
			,update_c				(	FieldType<int>					(	5,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~CoveringArea(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * 空間参照
	 * 
	 * spatial_reference.csv
	 */
	class SpatialRef
	{
	public:
		FieldType<long long> srid		;	///< 空間参照ID
		FieldType<uh::tstring> name		;	///< 空間参照ID名

	public:
		SpatialRef() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			srid				(	FieldType<long long>			(	1,		_T("SRID"),							6668LL,		false,		10L,			0L,				SurveyCSV::DOMAIN_SRID				))
			,name				(	FieldType<uh::tstring>			(	2,		_T("NAME"),							_T(""),		false,		128L,			0L													))
			{}
		~SpatialRef(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	/**
	 * バージョン
	 * 
	 * version.csv
	 */
	class Version
	{
	public:
		FieldType<uh::tstring> data_def		;	///< データバージョン名
		FieldType<uh::tstring> format_def		;	///< フォーマットバージョン名

	public:
		Version() : 
			//						型									列番号		フィールド名					初期値		NULL許容	整数最大桁数	小数最大桁数	(option)(最小値		最大値) or 有効値
			data_def			(	FieldType<uh::tstring>				(	1,		_T("DATA_DEF"),					_T(""),		false,		128L,			0L													))
			,format_def			(	FieldType<uh::tstring>				(	2,		_T("FORMAT_DEF"),				_T(""),		false,		128L,			0L													))
			{}
		~Version(){}

	public:
		static const uh::tstring FILE_NAME		;	///< ファイル名
		static const int ROW_COUNT				;	///< 列数
	};

	

public:
	// フォーマット仕様書バージョン
	static const uh::tstring				CONST_FMT_VERSION				;	///< フォーマット仕様書バージョン
	static const std::vector<uh::tstring>	CONST_ALLOW_FMT_VERSION_LIST	;	///< 許容するフォーマット仕様書バージョンリスト

	static const uh::tstring				DELIMITER						;	///< デリミタ
	static const long						GEOMETRY_ROW_COUNT				;	///< ジオメトリ定義列数

	static const std::string				HEADER_PREFIX					;	///< ヘッダ開始文字
	static const long						HEADER_COUNT					;	///< ヘッダ行数

	//// 文字コード
	//static const char*						FILE_MODE						;	///< 文字コード、読み込みモード

	// コード値ドメイン
	static const std::vector<int>			DOMAIN_LANE_GROUP_C				;	///< 車線グループ種別コード
	static const std::vector<int>			DOMAIN_MAX_LEGAL_SPEED_C		;	///< 制限速度コード（最大）
	static const std::vector<int>			DOMAIN_MIN_LEGAL_SPEED_C		;	///< 制限速度コード（最小）
	static const std::vector<int>			DOMAIN_TRAVEL_DIRECTION_C		;	///< 進行方向種別コード
	static const std::vector<int>			DOMAIN_ROAD_STRUCTURE_C			;	///< 道路構造種別コード

	static const std::vector<int>			DOMAIN_POINT_CLASS_C			;	///< ポイント種別コード
	static const std::vector<int>			DOMAIN_LINEAR_ELEMENT_C			;	///< 線形要素属性コード
	
	static const std::vector<int>			DOMAIN_COMPART_LINE_C			;	///< 線種別コード
	static const std::vector<int>			DOMAIN_COMPART_STYLE_C			;	///< 区画線設置様式コード
	static const std::vector<int>			DOMAIN_COMPART_SLOW_C			;	///< 減速表示コード

	static const std::vector<int>			DOMAIN_LINK_DIRECTION_C			;	///< リンク方向コード
	
	static const std::vector<int>			DOMAIN_SIGNAL_MOLD_C			;	///< 信号機形状コード

	static const std::vector<uh::tstring>	DOMAIN_ROADSIGN_C				;	///< 道路標識コード

	static const std::vector<long long>		DOMAIN_SRID						;	///< 空間参照ID

	static const std::vector<int>			DOMAIN_UPDATE_C					;	///< 更新コード

	// フラグ
	static const std::vector<int>			DOMAIN_FLAG						;	///< フラグ値
	
	
	// 固定値
	static const double						CONST_CURV_STRAIGHT				;	///< 曲率:直線
	static const double						CONST_CURV_RAD_STRAIGHT			;	///< 曲率半径:直線
	static const double						CONST_CURV_CH_RA_STRT_PNT		;	///< 曲率変化率:起点
	static const double						CONST_CLOT_PARAM_STRAIGHT		;	///< クロソイドパラメータ:直線

	//データ実体
	//std::vectorの場合、大量のデータを持つとcapacity拡張時にメモリ不足を招くため、std::listで持つ
	std::list<LaneLink>					listLaneLink;				///< 車線リンク
	std::list<LanePoint>				listLanePoint;				///< 車線ポイント
	std::list<RelLaneLink>				listRelLaneLink;			///< 車線関連
	std::list<CompartLink>				listCompartLink;			///< 区画線リンク
	std::list<CompartPoint>				listCompartPoint;			///< 区画線ポイント
	std::list<RelCompartLink>			listRelCompartLink;			///< 区画線関連
	std::list<BorderLink>				listBorderLink;				///< 境界線リンク
	std::list<BorderPoint>				listBorderPoint;			///< 境界線ポイント
	std::list<RelBorderLink>			listRelBorderLink;			///< 境界線関連
	std::list<SignalPoint>				listSignalPoint;			///< 信号機ポイント
	std::list<RelSignalPoint>			listRelSignalPoint;			///< 信号機関連
	std::list<RoadSignPoint>			listRoadSignPoint;			///< 標識ポイント
	std::list<RelRoadSignPoint>			listRelRoadSignPoint;		///< 標識関連
	std::list<CrossWalkLine>			listCrossWalkLine;			///< 横断歩道ライン
	std::list<RelCrossWalkLine>			listRelCrossWalkLine;		///< 横断歩道関連
	std::list<VirtualCompartLink>		listVirtualCompartLink;		///< 仮想線リンク
	std::list<VirtualCompartPoint>		listVirtualCompartPoint;	///< 仮想線ポイント
	std::list<RelVirtualCompartLink>	listRelVirtualCompartLink;	///< 車線リンク－仮想線リンク 関連
	std::list<CoveringArea>				listCoveringArea;			///< 覆い物エリア
	std::list<SpatialRef>				listSpatialRef;				///< 空間参照
	std::list<Version>					listVersion;				///< バージョン
};

typedef std::shared_ptr<SurveyCSV> SurveyCSVPtr;

}; //adam_v2
