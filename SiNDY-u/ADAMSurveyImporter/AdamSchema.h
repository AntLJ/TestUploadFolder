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
 * ADAM �X�L�[�}��`
 *
 * @version	2.1.5
 * @see		http://preon.mr.ipc.pioneer.co.jp/svn/release/trunk/public/SiNDY-b/Documents/data_model/
 */
class AdamSchema
{
public:
	/**
	 * �R���X�g���N�^
	 */
	AdamSchema(void){}

	/**
	 * �f�X�g���N�^
	 */
	~AdamSchema(void){}

	/**
	 * �f�[�^�N���A
	 */
	void Clear();

public:
	/* ===================================================
	     �C���f�b�N�X�ԍ��ƃt�B�[���h����` (�t�B�[�`��)
	   =================================================== */
	/* 
	 * iPCFeature �N���X
	 */
	class iPCFeature
	{
	public:
		SchemaFiled	objectid		;	///< OBJECTID
		SchemaFiled	shape			;	///< Shape
		SchemaFiled	operator_name	;	///< �ŏI�X�V��
		SchemaFiled	purpose_c		;	///< ��ƖړI
		SchemaFiled	modifydate		;	///< �ŏI�X�V����
		SchemaFiled	updatetype_c	;	///< �ŏI�X�V���e
		SchemaFiled	progmodifydate	;	///< �ŏI�v���O�����X�V����
		SchemaFiled	modifyprogname	;	///< �ŏI�X�V�v���O������
		SchemaFiled	userclaim_f		;	///< ���[�U�N���[���t���O
		SchemaFiled	source			;	///< ���\�[�X
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
	 * AdamiPCFeature �N���X
	 */
	class AdamiPCFeature : public iPCFeature
	{
	public:
		SchemaFiled	speculation_c	;	///< �����R�[�h
		SchemaFiled	sourceid		;	///< �񋟌�ID
		SchemaFiled	source_update_c	;	///< �񋟌��X�V�R�[�h
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
	 * GlobalAdamiPCFeature �N���X
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
	 * �㉺�������N �t�B�[�`��
	 */
	class UpdownLink : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����
		
		SchemaFiled	from_node_gid		;	///< �n�_�㉺���m�[�hGID
		SchemaFiled	to_node_gid			;	///< �I�_�㉺���m�[�hGID
		SchemaFiled	from_node_id		;	///< �n�_�㉺���m�[�hOID
		SchemaFiled	to_node_id			;	///< �I�_�㉺���m�[�hOID
		SchemaFiled	updown_group_id		;	///< �㉺���O���[�vOID
		SchemaFiled	lane_count			;	///< �Ԑ���
		SchemaFiled	right_change		;	///< �E�������Ԑ���
		SchemaFiled	left_change			;	///< ���������Ԑ���
		SchemaFiled	max_legal_speed_c	;	///< �������x�R�[�h�i�ő�j
		SchemaFiled	min_legal_speed_c	;	///< �������x�R�[�h�i�ŏ��j
		SchemaFiled	travel_direction_c	;	///< �i�s������ʃR�[�h
		SchemaFiled	road_structure_c	;	///< ���H�\����ʃR�[�h
		SchemaFiled	oneway_c			;	///< ����ʍs��ʃR�[�h
		SchemaFiled	rubbing_f			;	///< �C��t���t���O
		SchemaFiled	tunnel_f			;	///< �g���l���t���O
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
	 * �㉺���m�[�h �t�B�[�`��
	 */
	class UpdownNode : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled nodeclass_c		;	///< �m�[�h��ʃR�[�h
		SchemaFiled stop_point_f	;	///< ��~�_�t���O
		SchemaFiled covering_f		;	///< �������̃t���O
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
	 * �Ԑ������N �t�B�[�`��
	 */
	class LaneLink : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled from_node_gid		;	///< �n�_�Ԑ��m�[�hGID
		SchemaFiled to_node_gid			;	///< �I�_�Ԑ��m�[�hGID
		SchemaFiled from_node_id		;	///< �n�_�Ԑ��m�[�hOID
		SchemaFiled to_node_id			;	///< �I�_�Ԑ��m�[�hOID
		SchemaFiled lane_group_id		;	///< �Ԑ��O���[�vOID
		SchemaFiled lane_number			;	///< �Ԑ��ԍ�
		SchemaFiled lane_count			;	///< �Ԑ���
		SchemaFiled max_legal_speed_c	;	///< �������x�R�[�h�i�ő�j
		SchemaFiled min_legal_speed_c	;	///< �������x�R�[�h�i�ŏ��j
		SchemaFiled travel_direction_c	;	///< �i�s������ʃR�[�h
		SchemaFiled road_structure_c	;	///< ���H�\����ʃR�[�h
		SchemaFiled oneway_c			;	///< ����ʍs��ʃR�[�h
		SchemaFiled left_change_f		;	///< ���Ԑ��ύX�\�t���O
		SchemaFiled right_change_f		;	///< �E�Ԑ��ύX�\�t���O
		SchemaFiled left_protrusion_f	;	///< ���͂ݏo���\�t���O
		SchemaFiled right_protrusion_f	;	///< �E�͂ݏo���\�t���O
		SchemaFiled rubbing_f			;	///< �C��t���t���O
		SchemaFiled tunnel_f			;	///< �g���l���t���O
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
	 * �Ԑ��m�[�h �t�B�[�`��
	 */
	class LaneNode : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled nodeclass_c				;	///< �m�[�h��ʃR�[�h
		SchemaFiled stop_point_f			;	///< ��~�_�t���O
		SchemaFiled covering_f				;	///< �������̃t���O
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
	 * ���������N �t�B�[�`��
	 */
	class CompartLink : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled from_node_gid		;	///< �n�_�����m�[�hGID
		SchemaFiled to_node_gid			;	///< �I�_�����m�[�hGID
		SchemaFiled from_node_id		;	///< �n�_�����m�[�hOID
		SchemaFiled to_node_id			;	///< �I�_�����m�[�hOID
		SchemaFiled compart_line_c		;	///< ����ʃR�[�h
		SchemaFiled compart_style_c		;	///< �����ݒu�l���R�[�h
		SchemaFiled slowdown_label_c	;	///< �����\���R�[�h
		SchemaFiled installation_f		;	///< �ݒu���t���O
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
	 * �����m�[�h �t�B�[�`��
	 */
	class CompartNode : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled nodeclass_c	;	///< �m�[�h��ʃR�[�h
	public:
		CompartNode(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature	(ipTable),
			nodeclass_c				(ipTable, sindy::schema::adam_v2::compart_node::kNodeClassC		)
		{}

		~CompartNode(){}
	};

	/**
	 * ���E�������N �t�B�[�`��
	 */
	class BorderLink : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled from_node_gid		;	///< �n�_�����m�[�hGID
		SchemaFiled to_node_gid			;	///< �I�_�����m�[�hGID
		SchemaFiled from_node_id		;	///< �n�_�����m�[�hOID
		SchemaFiled to_node_id			;	///< �I�_�����m�[�hOID
		SchemaFiled imaginary_border_f	;	///< ���z���t���O
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
	 * ���E���m�[�h �t�B�[�`��
	 */
	class BorderNode : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled nodeclass_c	;	///< �m�[�h��ʃR�[�h
	public:
		BorderNode(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature	(ipTable),
			nodeclass_c				(ipTable, sindy::schema::adam_v2::border_node::kNodeClassC		)
		{}

		~BorderNode(){}
	};

	/**
	 * �M���@�|�C���g �t�B�[�`��
	 */
	class SignalPoint : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled horizontal_angle	;	///< �����p
		SchemaFiled signal_mold_c		;	///< �M���@�`��R�[�h
		SchemaFiled lamp_count			;	///< �M������
		SchemaFiled arrow_f				;	///< ���M���t���O
		SchemaFiled warning_f			;	///< �\���M���t���O
		SchemaFiled independent_f		;	///< �Ɨ��M���t���O
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
	 * ���f�������C�� �t�B�[�`��
	 */
	class CrossWalkLine : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled width;		///< ��
	public:
		CrossWalkLine(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature		(ipTable),
			width				(ipTable, sindy::schema::adam_v2::crosswalk_line::kWidth					)
		{}

		~CrossWalkLine(){}
	};

	/**
	 * ���H�W���|�C���g �t�B�[�`��
	 */
	class RoadSignPoint : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled horizontal_angle	;	///< �����p
		SchemaFiled roadsign_c			;	///< ���H�W���R�[�h
		SchemaFiled variable_f			;	///< �σt���O
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
	 * �����ς݃G���A �t�B�[�`��
	 */
	class MaintenanceArea : public iPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		MaintenanceArea(const ITablePtr& ipTable) : 
			iPCFeature		(ipTable)
		{}
		~MaintenanceArea(){}
	};

	/**
	 * ���H�W���G���A �t�B�[�`��
	 *
	 * @note	�C���|�[�g�ΏۊO�̂��ߒ�`�̂�
	 */
	class RoadMarkArea : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled roadmark_c	;	///< ���H�W���R�[�h
		SchemaFiled description ;	///< �W�����e�i�����j
	public:
		RoadMarkArea(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature		(ipTable),
			roadmark_c	(ipTable, sindy::schema::adam_v2::roadmark_area::kRoadMarkC		),
			description	(ipTable, sindy::schema::adam_v2::roadmark_area::kDescription	)
		{}

		~RoadMarkArea(){}
	};

	/**
	 * ���z�������C�� �t�B�[�`��
	 */
	class VirtualCompartLine : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����
	public:
		VirtualCompartLine(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature		(ipTable)
		{}

		~VirtualCompartLine(){}
	};

	/**
	 * �������̃G���A �t�B�[�`��
	 */
	class CoveringArea : public GlobalAdamiPCFeature
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����
	public:
		CoveringArea(const ITablePtr& ipTable) : 
			GlobalAdamiPCFeature		(ipTable)
		{}

		~CoveringArea(){}
	};

	/* ===================================================
	     �C���f�b�N�X�ԍ��ƃt�B�[���h����` (�e�[�u��)
	   =================================================== */
	/* 
	 * Table_class �N���X
	 */
	class TableClass
	{
	public:
		SchemaFiled	objectid		;	///< OBJECTID
		SchemaFiled	operator_name	;	///< �ŏI�X�V��
		SchemaFiled	purpose_c		;	///< ��ƖړI
		SchemaFiled	modifydate		;	///< �ŏI�X�V����
		SchemaFiled	progmodifydate	;	///< �ŏI�v���O�����X�V����
		SchemaFiled	modifyprogname	;	///< �ŏI�X�V�v���O������
		SchemaFiled	updatetype_c	;	///< �ŏI�X�V���e
		SchemaFiled	userclaim_f		;	///< ���[�U�N���[���t���O
		SchemaFiled	source			;	///< ���\�[�X
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
	 * AdamTableClass �N���X
	 */
	class AdamTableClass : public TableClass
	{
	public:
		SchemaFiled	sourceid		;	///< �񋟌�ID
		SchemaFiled	source_update_c	;	///< �񋟌��X�V�R�[�h
	public:
		AdamTableClass(const ITablePtr& ipTable) : 
			TableClass	(ipTable),
			sourceid	(ipTable, sindy::schema::adam_v2_table::kSourceID		),
			source_update_c(ipTable, sindy::schema::adam_v2_table::kSourceUpdateC)
		{}

		virtual ~AdamTableClass(){}
	};

	/* 
	 * GlobalAdamTableClass �N���X
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
	 * AdamNodeQueue �N���X
	 */
	class AdamNodeQueue : public GlobalAdamTableClass
	{
	public:
		SchemaFiled	sequence		;	///< ����
		SchemaFiled	lastnode_f		;	///< �I�[�m�[�h�t���O
	public:
		AdamNodeQueue(const ITablePtr& ipTable) : 
			GlobalAdamTableClass	(ipTable),
			sequence	(ipTable, sindy::schema::adam_v2_node_queue::kSequence		),
			lastnode_f	(ipTable, sindy::schema::adam_v2_node_queue::kLastNodeF		)
		{}

		virtual ~AdamNodeQueue(){}
	};

	/* 
	 * ���H�����N�|�㉺�������N�֘A
	 */
	class RelRoadLinkUpdownLink : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled road_link_id			;	///< ���H�����NOID
		SchemaFiled updown_link_gid			;	///< �㉺�������NGID
		SchemaFiled road_link_direction_c	;	///< ���H�����N�����R�[�h
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
	 * �㉺���y�A�����N
	 */
	class PairUpdownLink : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled updown_link_gid1	;	///< �㉺�������NGID1
		SchemaFiled updown_link_gid2	;	///< �㉺�������NGID2
	public:
		PairUpdownLink(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			updown_link_gid1			(ipTable, sindy::schema::adam_v2::pair_updown_link::kUpdownLinkGID1		),
			updown_link_gid2			(ipTable, sindy::schema::adam_v2::pair_updown_link::kUpdownLinkGID2		)
		{}

		~PairUpdownLink(){}
	};

	/* 
	 * �㉺�������N�O���[�v
	 */
	class UpdownLinkGroup : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled group_c	;	///< �O���[�v��ʃR�[�h
	public:
		UpdownLinkGroup(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			group_c			(ipTable, sindy::schema::adam_v2::updown_link_group::kGroupC			)
		{}

		~UpdownLinkGroup(){}
	};

	/* 
	 * �㉺�������N�|�Ԑ������N�֘A
	 */
	class RelUpdownLinkLaneLink : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled updown_link_gid	;	///< �㉺�������NGID
		SchemaFiled lane_link_gid	;	///< �Ԑ������NGID
	public:
		RelUpdownLinkLaneLink(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			updown_link_gid			(ipTable, sindy::schema::adam_v2::rel_updown_link_lane_link::kUpdownLinkGID		),
			lane_link_gid			(ipTable, sindy::schema::adam_v2::rel_updown_link_lane_link::kLaneLinkGID		)
		{}

		~RelUpdownLinkLaneLink(){}
	};

	/* 
	 * ���H�����N�|�Ԑ������N�֘A
	 */
	class RelRoadLinkLaneLink : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled road_link_id			;	///< ���H�����NOID
		SchemaFiled lane_link_gid			;	///< �Ԑ������NGID
		SchemaFiled road_link_direction_c	;	///< ���H�����N�����R�[�h
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
	 * �Ԑ������N�O���[�v
	 */
	class LaneLinkGroup : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled group_c					;	///< �O���[�v��ʃR�[�h
		SchemaFiled updown_link_group_id	;	///< �㉺�������N�O���[�vOID
	public:
		LaneLinkGroup(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			group_c					(ipTable, sindy::schema::adam_v2::lane_link_group::kGroupC				),
			updown_link_group_id	(ipTable, sindy::schema::adam_v2::lane_link_group::kUpDownLinkGroupID	)
		{}

		~LaneLinkGroup(){}
	};

	/* 
	 * ���������N�|�Ԑ������N�֘A
	 */
	class RelCompartLinkLaneLink : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled compart_link_gid			;	///< ���������NGID
		SchemaFiled lane_link_gid				;	///< �Ԑ������NGID
		SchemaFiled compart_link_direction_c	;	///< ���������N�����R�[�h
		SchemaFiled lr_c						;	///< ���E����R�[�h
		SchemaFiled sequence					;	///< ����
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
	 * ���H���E�������N�|�Ԑ������N�֘A
	 */
	class RelBorderLinkLaneLink : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled border_link_gid			;	///< ���H���E�������NGID
		SchemaFiled lane_link_gid			;	///< �Ԑ������NGID
		SchemaFiled border_link_direction_c	;	///< ���H���E�������N�����R�[�h
		SchemaFiled lr_c					;	///< ���E����R�[�h
		SchemaFiled sequence				;	///< ����
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
	 * �㉺���m�[�h�|�M���@�|�C���g�֘A
	 */
	class RelUpdoenNodeSignalPoint : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled updown_node_gid		;	///< �㉺���m�[�hGID
		SchemaFiled signal_point_gid	;	///< �M���@�|�C���gGID
	public:
		RelUpdoenNodeSignalPoint(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			updown_node_gid			(ipTable, sindy::schema::adam_v2::rel_updown_node_signal_point::kUpdownNodeGID		),
			signal_point_gid		(ipTable, sindy::schema::adam_v2::rel_updown_node_signal_point::kSignalPointGID		)
		{}

		~RelUpdoenNodeSignalPoint(){}
	};
	
	/* 
	 * �Ԑ��m�[�h�|�M���@�|�C���g�֘A
	 */
	class RelLaneNodeSignalPoint : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled lane_node_gid		;	///< �Ԑ��m�[�hGID
		SchemaFiled signal_point_gid	;	///< �M���@�|�C���gGID
	public:
		RelLaneNodeSignalPoint(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			lane_node_gid			(ipTable, sindy::schema::adam_v2::rel_lane_node_signal_point::kLaneNodeGID		),
			signal_point_gid		(ipTable, sindy::schema::adam_v2::rel_lane_node_signal_point::kSignalPointGID	)
		{}

		~RelLaneNodeSignalPoint(){}
	};
	
	/* 
	 * �Ԑ��m�[�h�|���f�������C���֘A
	 */
	class RelLaneNodeCrossWalkLine : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled lane_node_gid		;	///< �Ԑ��m�[�hGID
		SchemaFiled crosswalk_line_gid	;	///< ���f�������C��GID
	public:
		RelLaneNodeCrossWalkLine(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			lane_node_gid			(ipTable, sindy::schema::adam_v2::rel_lane_node_crosswalk_line::kLaneNodeGID			),
			crosswalk_line_gid		(ipTable, sindy::schema::adam_v2::rel_lane_node_crosswalk_line::kCrosswalkLineGID	)
		{}

		~RelLaneNodeCrossWalkLine(){}
	};
	
	/* 
	 * �Ԑ��m�[�h�|���H�W���|�C���g�֘A
	 */
	class RelLaneNodeRoadSignPoint : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled lane_node_gid		;	///< �Ԑ��m�[�hGID
		SchemaFiled roadsign_point_gid	;	///< ���H�W���|�C���gGID
	public:
		RelLaneNodeRoadSignPoint(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			lane_node_gid			(ipTable, sindy::schema::adam_v2::rel_lane_node_roadsign_point::kLaneNodeGID			),
			roadsign_point_gid		(ipTable, sindy::schema::adam_v2::rel_lane_node_roadsign_point::kRoadsignPointGID	)
		{}

		~RelLaneNodeRoadSignPoint(){}
	};

	/* 
	 * ���H�W���G���A�|�Ԑ������N�֘A
	 */
	class RelRoadMarkAreaLaneLink : public AdamTableClass
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled roadmark_area_gid		;	///< ���H�W���G���AGID
		SchemaFiled lane_link_gid			;	///< �Ԑ������NGID
	public:
		RelRoadMarkAreaLaneLink(const ITablePtr& ipTable) : 
			AdamTableClass	(ipTable),
			roadmark_area_gid		(ipTable, sindy::schema::adam_v2::rel_roadmark_area_lane_link::kRoadMarkAreaGID	),
			lane_link_gid			(ipTable, sindy::schema::adam_v2::rel_roadmark_area_lane_link::kLaneLinkGID		)
		{}

		~RelRoadMarkAreaLaneLink(){}
	};
	
	/* 
	 * �㉺���m�[�h��
	 */
	class NQUpdownNode : public AdamNodeQueue
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled updown_link_gid	;	///< �㉺�������NGID
		SchemaFiled updown_node_gid	;	///< �㉺���m�[�hGID
		SchemaFiled width			;	///< ����
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
	 * �Ԑ��m�[�h��
	 */
	class NQLaneNode : public AdamNodeQueue
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled lane_link_gid			;	///< �Ԑ������NGID
		SchemaFiled lane_node_gid			;	///< �Ԑ��m�[�hGID
		SchemaFiled linear_element_c		;	///< ���`�v�f�����R�[�h
		SchemaFiled width					;	///< ����
		SchemaFiled curvature				;	///< �ȗ�
		SchemaFiled curvature_radius		;	///< �ȗ����a
		SchemaFiled curvature_change_rate	;	///< �ȗ��ω���
		SchemaFiled clothoid_parameter		;	///< �N���\�C�h�p�����[�^
		SchemaFiled longitudinal_slope		;	///< �c�f���z
		SchemaFiled cross_slope				;	///< ���f���z
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
	 * �����m�[�h��
	 */
	class NQCompartNode : public AdamNodeQueue
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled compart_link_gid	;	///< ���������NGID
		SchemaFiled compart_node_gid	;	///< �����m�[�hGID
		SchemaFiled width				;	///< ����
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
	 * ���H���E���m�[�h��
	 */
	class NQBorderNode : public AdamNodeQueue
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled border_link_gid	;	///< ���H���E�������NGID
		SchemaFiled border_node_gid	;	///< ���H���E���m�[�hGID
	public:
		NQBorderNode(const ITablePtr& ipTable) : 
			AdamNodeQueue	(ipTable),
			border_link_gid		(ipTable, sindy::schema::adam_v2::nq_border_node::kBorderLinkGID	),
			border_node_gid		(ipTable, sindy::schema::adam_v2::nq_border_node::kBorderNodeGID	)
		{}

		~NQBorderNode(){}
	};

	/* 
	 * ADAM�o�[�W����
	 * @note	�C���|�[�g�ΏۊO�̂��ߒ�`�̂�
	 */
	class AdamVersion
	{
	public:
		static const uh::tstring	TableName;	///< �e�[�u����

		SchemaFiled	objectid		;	///< OBJECTID
	public:
		AdamVersion(const ITablePtr& ipTable) : 
			objectid		(ipTable, sindy::schema::adam_v2::adam_version::kObjectID	)
		{}

		~AdamVersion(){}
	};

public:
	//�f�[�^����
	//�C���|�[�g�ΏۊO�̓R�����g�A�E�g���Ă���
	//IFeatureClassPtr	featureUpdownLink;				///< �㉺�������N �t�B�[�`��
	//IFeatureClassPtr	featureUpdownNode;				///< �㉺���m�[�h �t�B�[�`��
	IFeatureClassPtr	featureLaneLink;				///< �Ԑ������N �t�B�[�`��
	IFeatureClassPtr	featureLaneNode;				///< �Ԑ��m�[�h �t�B�[�`��
	IFeatureClassPtr	featureCompartLink;				///< ���������N �t�B�[�`��
	IFeatureClassPtr	featureCompartNode;				///< �����m�[�h �t�B�[�`��
	IFeatureClassPtr	featureBorderLink;				///< ���E�������N �t�B�[�`��
	IFeatureClassPtr	featureBorderNode;				///< ���E���m�[�h �t�B�[�`��
	IFeatureClassPtr	featureSignalPoint;				///< �M���@�|�C���g �t�B�[�`��
	IFeatureClassPtr	featureCrossWalkLine;			///< ���f�����|�C���g �t�B�[�`��
	IFeatureClassPtr	featureRoadSignPoint;			///< ���H�W���|�C���g �t�B�[�`��
	//IFeatureClassPtr	featureMaintenanceArea;			///< �����ς݃G���A �t�B�[�`��
	//IFeatureClassPtr	featureRoadMarkArea;			///< ���H�W���G���A �t�B�[�`��
	IFeatureClassPtr	featureVirtualCompartLine;		///< ���z�������C�� �t�B�[�`��
	IFeatureClassPtr	featureCoveringArea;			///< �������G���A �t�B�[�`��

	//ITablePtr			tableRelRoadLinkUpdownLink;		///< ���H�����N�|�㉺�������N�֘A �e�[�u��
	//ITablePtr			tablePairUpdownLink;			///< �㉺���y�A�����N �e�[�u��
	//ITablePtr			tableUpdownLinkGroup;			///< �㉺�������N�O���[�v �e�[�u��
	//ITablePtr			tableRelUpdownLinkLaneLink;		///< �㉺�������N�|�Ԑ������N�֘A �e�[�u��
	ITablePtr			tableRelRoadLinkLaneLink;		///< ���H�����N�|�Ԑ������N�֘A �e�[�u��
	ITablePtr			tableLaneLinkGroup;				///< �Ԑ������N�O���[�v �e�[�u��
	ITablePtr			tableRelCompartLinkLaneLink;	///< ���������N�|�Ԑ������N�֘A �e�[�u��
	ITablePtr			tableRelBorderLinkLaneLink;		///< ���H���E�������N�|�Ԑ������N�֘A �e�[�u��
	//ITablePtr			tableRelUpdoenNodeSignalPoint;	///< �㉺���m�[�h�|�M���@�|�C���g�֘A �e�[�u��
	ITablePtr			tableRelLaneNodeSignalPoint;	///< �Ԑ��m�[�h�|�M���@�|�C���g�֘A �e�[�u��
	ITablePtr			tableRelLaneNodeCrossWalkLine;	///< �Ԑ��m�[�h�|���f�������C���֘A �e�[�u��
	ITablePtr			tableRelLaneNodeRoadSignPoint;	///< �Ԑ��m�[�h�|���H�W���|�C���g�֘A �e�[�u��
	//ITablePtr			tableRelRoadMarkAreaLaneLink;	///< �Ԑ������N�|���H�W���G���A�֘A �e�[�u��
	//ITablePtr			tableNQUpdownNode;				///< �㉺���m�[�h�� �e�[�u��
	ITablePtr			tableNQLaneNode;				///< �Ԑ��m�[�h�� �e�[�u��
	ITablePtr			tableNQCompartNode;				///< �����m�[�h�� �e�[�u��
	ITablePtr			tableNQBorderNode;				///< ���H���E���m�[�h�� �e�[�u��
	ITablePtr			tableAdamVersion;				///< ADAM�o�[�W����

};
typedef std::shared_ptr<AdamSchema> AdamSchemaPtr;

};	//namespace adam_v2