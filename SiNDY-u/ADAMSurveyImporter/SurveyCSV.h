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
	 * �X�V�R�[�h
	 */
	namespace update_code {
		enum ECode {
			kUnChange = 0,		//!< �ύX�Ȃ�
			kNew = 1,			//!< �V�K�ǉ�
			kDelete = 2,		//!< �폜
			kUpdateAttr = 3,	//!< �����ύX
		};
	}

/**
 * ���ʉ�Д[���t�H�[�}�b�g
 *
 * @version	1.0.3
 * @see		\\win\dfs\���剡�fPJ\ADAM�f�[�^����\40_�����c�[���J��\01_�I�[�T�����O�t�H�[�}�b�g
 */
class SurveyCSV
{
public:
	/**
	 * �R���X�g���N�^
	 */
	SurveyCSV(void);
	/**
	 * �f�X�g���N�^
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
	 * �I�u�W�F�N�g���N���X
	 */
	class BaseObject
	{
	public:
		FieldType<long long> objectid				;	///< �Ԑ������NID
	public:
		BaseObject() : 
			//						�^										��ԍ�	�t�B�[���h��		�����l				NULL���e	�����ő包��			�����ő包��						(option)(�ŏ��l		�ő�l) or �L���l
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
	 * �I�u�W�F�N�g���N���X
	 */
	class BaseTable
	{
	public:
		FieldType<long long> objectid				;	///< �Ԑ������NID
	public:
		BaseTable() : 
			//						�^											��ԍ�	�t�B�[���h��		�����l				NULL���e	�����ő包��			�����ő包��						(option)(�ŏ��l		�ő�l) or �L���l
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
	 * �Ԑ������N CSV
	 * 
	 * lane_link.csv
	 */
	class LaneLink : public BaseObject
	{
	public:
		FieldType<Polyline> shape				;	///< �`��
		
		FieldType<long long> from_node_id		;	///< �n�_�m�[�hID
		FieldType<long long> to_node_id			;	///< �I�_�m�[�hID
		FieldType<long long> lane_group_id		;	///< �Ԑ��O���[�vID
		FieldType<int> lane_group_c				;	///< �Ԑ��O���[�v��ʃR�[�h
		FieldType<int> lane_no					;	///< �Ԑ��ԍ�
		FieldType<int> lane_count				;	///< �Ԑ���
		FieldType<int> max_legal_speed_c		;	///< �������x�R�[�h�i�ő�j
		FieldType<int> min_legal_speed_c		;	///< �������x�R�[�h�i�ŏ��j
		FieldType<int> travel_direction_c		;	///< �i�s������ʃR�[�h
		FieldType<int> road_structure_c			;	///< ���H�\����ʃR�[�h
		FieldType<int> both_link_direction_f	;	///< �o�����ʍs�\�t���O
		FieldType<int> left_change_f			;	///< ���Ԑ��ύX�\�t���O
		FieldType<int> right_change_f			;	///< �E�Ԑ��ύX�\�t���O
		FieldType<int> left_protrusion_f		;	///< ���͂ݏo���\�t���O
		FieldType<int> right_protrusion_f		;	///< �E�͂ݏo���\�t���O
		FieldType<int> rubbing_f				;	///< �C��t���t���O
		FieldType<int> tunnel_f					;	///< �g���l���t���O
		FieldType<int> update_c					;	///< �X�V�R�[�h

	public:
		LaneLink() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
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
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * �Ԑ��|�C���g CSV
	 * 
	 * lane_point.csv
	 */
	class LanePoint : public BaseObject
	{
	public:
		FieldType<Point> shape				;	///< �`��

		FieldType<long long>	node_id					;	///< �Ԑ��m�[�hID
		FieldType<int>			point_class_c			;	///< �|�C���g��ʃR�[�h
		FieldType<long long>	lane_link_id			;	///< �Ԑ������NID
		FieldType<long long>	sequence				;	///< �V�[�P���X�ԍ�
		FieldType<int>			lastnode_f				;	///< �I�[�m�[�h�t���O
		FieldType<int>			stop_point_f			;	///< ��~�_�t���O
		FieldType<double>		width					;	///< ����
		FieldType<int>			linear_element_c		;	///< ���`�v�f�����R�[�h
		FieldType<double>		curvature				;	///< �ȗ�
		FieldType<double>		curvature_radius		;	///< �ȗ����a
		FieldType<double>		curvature_change_rate	;	///< �ȗ��ω���
		FieldType<double>		clothoid_parameter		;	///< �N���\�C�h�p�����[�^
		FieldType<double>		longitudinal_slope		;	///< �c�f���z
		FieldType<double>		cross_slope				;	///< ���f���z
		FieldType<int>			cover_f					;	///< �������t���O
		FieldType<int>			update_c				;	///< �X�V�R�[�h

	public:
		LanePoint() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
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
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * ���H�����N�|�Ԑ������N �֘A
	 * 
	 * rel_road_link_lane_link.csv
	 */
	class RelLaneLink : public BaseTable
	{
	public:
		FieldType<long long>	lane_link_id				;	///< �Ԑ������NID
		FieldType<long long>	road_link_id				;	///< ���H�����NID
		FieldType<int>			road_link_direction_c		;	///< ���H�����N�����R�[�h
		FieldType<int>			update_c					;	///< �X�V�R�[�h
	public:
		RelLaneLink() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
			lane_link_id			(	FieldType<long long>			(	2,		_T("LANE_LINK_ID"),				0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,road_link_id			(	FieldType<long long>			(	3,		_T("ROAD_LINK_ID"),				0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,road_link_direction_c	(	FieldType<int>					(	4,		_T("ROAD_LINK_DIRECTION_C"),	1,			false,		5L,				0L,				DOMAIN_LINK_DIRECTION_C					))
			,update_c				(	FieldType<int>					(	5,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~RelLaneLink(){}

	public:
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * ���������N CSV
	 * 
	 * compartment_link.csv
	 */
	class CompartLink : public BaseObject
	{
	public:
		FieldType<Polyline> shape				;	///< �`��

		FieldType<long long>	from_node_id		;	///< �n�_�m�[�hID
		FieldType<long long>	to_node_id			;	///< �I�_�m�[�hID
		FieldType<int>			compart_line_c		;	///< ����ʃR�[�h
		FieldType<int>			compart_style_c		;	///< �����ݒu�l���R�[�h
		FieldType<int>			slowdown_label_c	;	///< �����\���t���O
		FieldType<int>			installation_f		;	///< �ݒu���t���O
		FieldType<int>			update_c			;	///< �X�V�R�[�h

	public:
		CompartLink() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
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
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * �����|�C���g CSV
	 * 
	 * compartment_point.csv
	 */
	class CompartPoint : public BaseObject
	{
	public:
		FieldType<Point> shape				;	///< �`��

		FieldType<long long>	node_id				;	///< �����m�[�hID
		FieldType<int>			point_class_c		;	///< �|�C���g��ʃR�[�h
		FieldType<long long>	compart_link_id		;	///< ���������NID
		FieldType<long long>	sequence			;	///< �V�[�P���X�ԍ�
		FieldType<int>			lastnode_f			;	///< �I�[�m�[�h�t���O
		FieldType<double>		width				;	///< ����
		FieldType<int>			update_c			;	///< �X�V�R�[�h

	public:
		CompartPoint() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
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
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * �Ԑ������N�|���������N �֘A
	 * 
	 * rel_lane_link_compartment_link.csv
	 */
	class RelCompartLink : public BaseTable
	{
	public:
		FieldType<long long>	lane_link_id				;	///< �Ԑ������NID
		FieldType<int>			lr_f						;	///< ���E����t���O
		FieldType<long long>	compart_link_id				;	///< ���������NID
		FieldType<int>			compart_link_direction_c	;	///< ���������N�����R�[�h
		FieldType<long long>	sequence					;	///< �V�[�P���X�ԍ�
		FieldType<int>			update_c					;	///< �X�V�R�[�h

	public:
		RelCompartLink() : 
			//								�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
			lane_link_id					(	FieldType<long long>			(	2,		_T("LANE_LINK_ID"),				1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,lr_f							(	FieldType<int>					(	3,		_T("LR_F"),						0,			true,		1L,				0L,				DOMAIN_FLAG								))
			,compart_link_id				(	FieldType<long long>			(	4,		_T("COMPART_LINK_ID"),			1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,compart_link_direction_c		(	FieldType<int>					(	5,		_T("COMPART_LINK_DIRECTION_C"),	1,			true,		5L,				0L,				DOMAIN_LINK_DIRECTION_C					))
			,sequence						(	FieldType<long long>			(	6,		_T("SEQUENCE"),					0LL,		true,		10L,			0L,				0LL,		9999999999LL				))
			,update_c						(	FieldType<int>					(	7,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
		{}
		~RelCompartLink(){}

	public:
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * ���E�������N CSV
	 * 
	 * border_link.csv
	 */
	class BorderLink : public BaseObject
	{
	public:
		FieldType<Polyline> shape				;	///< �`��

		FieldType<long long>	from_node_id		;	///< �n�_�m�[�hID
		FieldType<long long>	to_node_id			;	///< �I�_�m�[�hID
		FieldType<int>			imaginary_boder_f	;	///< ���z���t���O
		FieldType<int>			update_c			;	///< �X�V�R�[�h

	public:
		BorderLink() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
			shape					(	FieldType<Polyline>				(	1,		_T("LON,LAT,HEIGHT"),			Polyline(),	false,		0L,				0L														))
			,from_node_id			(	FieldType<long long>			(	5,		_T("FROM_NODE_ID"),				0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,to_node_id				(	FieldType<long long>			(	6,		_T("TO_NODE_ID"),				0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,imaginary_boder_f		(	FieldType<int>					(	7,		_T("IMAGINARY_BODER_F"),		0,			true,		1L,				0L,				DOMAIN_FLAG								))
			,update_c				(	FieldType<int>					(	8,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
		{}
		~BorderLink(){}

	public:
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * ���E���|�C���g
	 * 
	 * border_point.csv
	 */
	class BorderPoint : public BaseObject
	{
	public:
		FieldType<Point>		shape			;	///< �`��

		FieldType<long long>	node_id			;	///< ���E���m�[�hID
		FieldType<int>			point_class_c	;	///< �|�C���g��ʃR�[�h
		FieldType<long long>	border_link_id	;	///< ���E�������NID
		FieldType<long long>	sequence		;	///< �V�[�P���X�ԍ�
		FieldType<int>			lastnode_f		;	///< �I�[�m�[�h�t���O
		FieldType<int>			update_c		;	///< �X�V�R�[�h

	public:
		BorderPoint() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
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
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * �Ԑ������N�|���E�������N �֘A
	 * 
	 * rel_border_link.csv
	 */
	class RelBorderLink : public BaseTable
	{
	public:
		FieldType<long long>	lane_link_id				;	///< �Ԑ������NID
		FieldType<int>			lr_f						;	///< ���E����t���O
		FieldType<long long>	border_link_id				;	///< ���E�������NID
		FieldType<int>			border_link_direction_c		;	///< ���E�������N�����R�[�h
		FieldType<long long>	sequence					;	///< �V�[�P���X�ԍ�
		FieldType<int>			update_c					;	///< �X�V�R�[�h

	public:
		RelBorderLink() : 
			//								�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
			lane_link_id					(	FieldType<long long>			(	2,		_T("LANE_LINK_ID"),				1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,lr_f							(	FieldType<int>					(	3,		_T("LR_F"),						0,			true,		1L,				0L,				DOMAIN_FLAG								))
			,border_link_id					(	FieldType<long long>			(	4,		_T("BORDER_LINK_ID"),			1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,border_link_direction_c		(	FieldType<int>					(	5,		_T("BORDER_LINK_DIRECTION_C"),	1,			true,		5L,				0L,				DOMAIN_LINK_DIRECTION_C					))
			,sequence						(	FieldType<long long>			(	6,		_T("SEQUENCE"),					0LL,		true,		10L,			0L,				0LL,		9999999999LL				))
			,update_c						(	FieldType<int>					(	7,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~RelBorderLink(){}

	public:
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * �M���@�|�C���g
	 * 
	 * signal_point.csv
	 */
	class SignalPoint : public BaseObject
	{
	public:
		FieldType<Point>	shape				;	///< �`��

		FieldType<double>	horizontal_angle	;	///< �����p
		FieldType<int>		signal_mold_c		;	///< �M���@�`��R�[�h
		FieldType<int>		lamp_count			;	///< �M������
		FieldType<int>		arrow_f				;	///< ���M���t���O
		FieldType<int>		warning_f			;	///< �\���M���t���O
		FieldType<int>		independent_f		;	///< �Ɨ��M���t���O
		FieldType<int>		update_c			;	///< �X�V�R�[�h

	public:
		SignalPoint() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
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
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * �Ԑ��m�[�h�|�M���@�|�C���g �֘A
	 * 
	 * rel_lane_point_signal_point.csv
	 */
	class RelSignalPoint : public BaseTable
	{
	public:
		FieldType<long long>	lane_node_id		;	///< �Ԑ��m�[�hID
		FieldType<long long>	signal_point_id	;	///< �M���@�|�C���gID
		FieldType<int>			update_c		;	///< �X�V�R�[�h

	public:
		RelSignalPoint() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
			lane_node_id			(	FieldType<long long>			(	2,		_T("LANE_NODE_ID"),				0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,signal_point_id		(	FieldType<long long>			(	3,		_T("SIGNAL_POINT_ID"),			0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,update_c				(	FieldType<int>					(	4,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~RelSignalPoint(){}

	public:
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * ���H�W���|�C���g
	 * 
	 * roadsign_point.csv
	 */
	class RoadSignPoint : public BaseObject
	{
	public:
		FieldType<Point>		shape				;	///< �`��

		FieldType<double>		horizontal_angle	;	///< �����p
		FieldType<uh::tstring>	roadsign_c			;	///< ���H�W���R�[�h
		FieldType<int>			variable_f			;	///< �σt���O
		FieldType<int>			update_c			;	///< �X�V�R�[�h

	public:
		RoadSignPoint() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
			shape					(	FieldType<Point>				(	1,		_T("LON,LAT,HEIGHT"),			Point(),	false,		0L,				0L														))
			,horizontal_angle		(	FieldType<double>				(	5,		_T("HORIZONTAL_ANGLE"),			0.0,		true,		3L,				3L,				0.000,		359.999						))
			,roadsign_c				(	FieldType<uh::tstring>			(	6,		_T("ROADSIGN_C"),				_T(""),		true,		32L,			0L,				DOMAIN_ROADSIGN_C						))
			,variable_f				(	FieldType<int>					(	7,		_T("VARIABLE_F"),				1,			true,		1L,				0L,				DOMAIN_FLAG								))
			,update_c				(	FieldType<int>					(	8,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~RoadSignPoint(){}

	public:
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * �Ԑ��m�[�h�|���H�W���|�C���g �֘A
	 * 
	 * rel_lane_point_roadsign_point.csv
	 */
	class RelRoadSignPoint : public BaseTable
	{
	public:
		FieldType<long long>	lane_node_id		;	///< �Ԑ��m�[�hID
		FieldType<long long>	roadsign_point_id	;	///< ���H�W���|�C���gID
		FieldType<int>			update_c			;	///< �X�V�R�[�h

	public:
		RelRoadSignPoint() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
			lane_node_id			(	FieldType<long long>			(	2,		_T("LANE_NODE_ID"),				0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,roadsign_point_id		(	FieldType<long long>			(	3,		_T("ROADSIGN_POINT_ID"),		0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,update_c				(	FieldType<int>					(	4,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~RelRoadSignPoint(){}

	public:
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * ���f�������C��
	 * 
	 * crosswalk_line.csv
	 */
	class CrossWalkLine : public BaseObject
	{
	public:
		FieldType<Polyline> shape				;	///< �`��

		FieldType<double> width				;	///< ��
		FieldType<int>	update_c			;	///< �X�V�R�[�h
	public:
		CrossWalkLine() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
			shape					(	FieldType<Polyline>				(	1,		_T("LON,LAT,HEIGHT"),			Polyline(),	false,		0L,				0L														))
			,width					(	FieldType<double>				(	5,		_T("WIDTH"),					0.0,		true,		3L,				3L,				0.000,		999.999						))
			,update_c				(	FieldType<int>					(	6,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~CrossWalkLine(){}

	public:
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * �Ԑ��m�[�h�|���f�������C�� �֘A
	 * 
	 * rel_lane_point_crosswalk_line.csv
	 */
	class RelCrossWalkLine : public BaseTable
	{
	public:
		FieldType<long long> lane_node_id		;	///< �Ԑ��m�[�hID
		FieldType<long long> crosswalk_line_id	;	///< ���f�������C��ID
		FieldType<int>	update_c				;	///< �X�V�R�[�h

	public:
		RelCrossWalkLine() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
			lane_node_id		(	FieldType<long long>			(	2,		_T("LANE_NODE_ID"),					0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,crosswalk_line_id	(	FieldType<long long>			(	3,		_T("CROSSWALK_LINE_ID"),			0LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,update_c			(	FieldType<int>					(	4,		_T("UPDATE_C"),						0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~RelCrossWalkLine(){}

	public:
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * ���z�������N CSV
	 * 
	 * virtual_compartment_link.csv
	 */
	class VirtualCompartLink : public BaseObject
	{
	public:
		FieldType<Polyline> shape				;	///< �`��

		FieldType<long long>	from_node_id		;	///< �n�_�m�[�hID
		FieldType<long long>	to_node_id			;	///< �I�_�m�[�hID
		FieldType<int>			compart_line_c		;	///< ����ʃR�[�h
		FieldType<int>			compart_style_c		;	///< �����ݒu�l���R�[�h
		FieldType<int>			slowdown_label_c	;	///< �����\���t���O
		FieldType<int>			installation_f		;	///< �ݒu���t���O

	public:
		VirtualCompartLink() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
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
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * ���z���|�C���g CSV
	 * 
	 * virtual_compartment_point.csv
	 */
	class VirtualCompartPoint : public BaseObject
	{
	public:
		FieldType<Point> shape				;	///< �`��

		FieldType<long long>	node_id				;	///< ���z���m�[�hID
		FieldType<int>			point_class_c		;	///< �|�C���g��ʃR�[�h
		FieldType<long long>	v_compart_link_id		;	///< ���z�������NID
		FieldType<long long>	sequence			;	///< �V�[�P���X�ԍ�
		FieldType<int>			lastnode_f			;	///< �I�[�m�[�h�t���O
		FieldType<double>		width				;	///< ����

	public:
		VirtualCompartPoint() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
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
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * �Ԑ������N�|���z�������N �֘A
	 * 
	 * rel_lane_link_virtual_compartment_link.csv
	 */
	class RelVirtualCompartLink : public BaseTable
	{
	public:
		FieldType<long long>	lane_link_id				;	///< �Ԑ������NID
		FieldType<int>			lr_f						;	///< ���E����t���O
		FieldType<long long>	v_compart_link_id				;	///< ���z�������NID
		FieldType<int>			v_compart_link_direction_c	;	///< ���z�������N�����R�[�h
		FieldType<long long>	sequence					;	///< �V�[�P���X�ԍ�

	public:
		RelVirtualCompartLink() : 
			//								�^									��ԍ�		�t�B�[���h��						�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
			lane_link_id					(	FieldType<long long>			(	2,		_T("LANE_LINK_ID"),					1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,lr_f							(	FieldType<int>					(	3,		_T("LR_F"),							0,			true,		1L,				0L,				DOMAIN_FLAG								))
			,v_compart_link_id				(	FieldType<long long>			(	4,		_T("V_COMPART_LINK_ID"),			1LL,		false,		10L,			0L,				1LL,		9999999999LL				))
			,v_compart_link_direction_c		(	FieldType<int>					(	5,		_T("V_COMPART_LINK_DIRECTION_C"),	1,			true,		5L,				0L,				DOMAIN_LINK_DIRECTION_C					))
			,sequence						(	FieldType<long long>			(	6,		_T("SEQUENCE"),						0LL,		true,		10L,			0L,				0LL,		9999999999LL				))
		{}
		~RelVirtualCompartLink(){}

	public:
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * �������G���A CSV
	 * 
	 * covering_area.csv
	 */
	class CoveringArea : public BaseObject
	{
	public:
		FieldType<Polygon> shape				;	///< �`��
		FieldType<int>	update_c				;	///< �X�V�R�[�h

	public:
		CoveringArea() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
			shape					(	FieldType<Polygon>				(	1,		_T("LON,LAT,HEIGHT"),			Polygon(),	false,		0L,				0L														))
			,update_c				(	FieldType<int>					(	5,		_T("UPDATE_C"),					0,			false,		5L,				0L,				DOMAIN_UPDATE_C							))
			{}
		~CoveringArea(){}

	public:
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * ��ԎQ��
	 * 
	 * spatial_reference.csv
	 */
	class SpatialRef
	{
	public:
		FieldType<long long> srid		;	///< ��ԎQ��ID
		FieldType<uh::tstring> name		;	///< ��ԎQ��ID��

	public:
		SpatialRef() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
			srid				(	FieldType<long long>			(	1,		_T("SRID"),							6668LL,		false,		10L,			0L,				SurveyCSV::DOMAIN_SRID				))
			,name				(	FieldType<uh::tstring>			(	2,		_T("NAME"),							_T(""),		false,		128L,			0L													))
			{}
		~SpatialRef(){}

	public:
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	/**
	 * �o�[�W����
	 * 
	 * version.csv
	 */
	class Version
	{
	public:
		FieldType<uh::tstring> data_def		;	///< �f�[�^�o�[�W������
		FieldType<uh::tstring> format_def		;	///< �t�H�[�}�b�g�o�[�W������

	public:
		Version() : 
			//						�^									��ԍ�		�t�B�[���h��					�����l		NULL���e	�����ő包��	�����ő包��	(option)(�ŏ��l		�ő�l) or �L���l
			data_def			(	FieldType<uh::tstring>				(	1,		_T("DATA_DEF"),					_T(""),		false,		128L,			0L													))
			,format_def			(	FieldType<uh::tstring>				(	2,		_T("FORMAT_DEF"),				_T(""),		false,		128L,			0L													))
			{}
		~Version(){}

	public:
		static const uh::tstring FILE_NAME		;	///< �t�@�C����
		static const int ROW_COUNT				;	///< ��
	};

	

public:
	// �t�H�[�}�b�g�d�l���o�[�W����
	static const uh::tstring				CONST_FMT_VERSION				;	///< �t�H�[�}�b�g�d�l���o�[�W����
	static const std::vector<uh::tstring>	CONST_ALLOW_FMT_VERSION_LIST	;	///< ���e����t�H�[�}�b�g�d�l���o�[�W�������X�g

	static const uh::tstring				DELIMITER						;	///< �f���~�^
	static const long						GEOMETRY_ROW_COUNT				;	///< �W�I���g����`��

	static const std::string				HEADER_PREFIX					;	///< �w�b�_�J�n����
	static const long						HEADER_COUNT					;	///< �w�b�_�s��

	//// �����R�[�h
	//static const char*						FILE_MODE						;	///< �����R�[�h�A�ǂݍ��݃��[�h

	// �R�[�h�l�h���C��
	static const std::vector<int>			DOMAIN_LANE_GROUP_C				;	///< �Ԑ��O���[�v��ʃR�[�h
	static const std::vector<int>			DOMAIN_MAX_LEGAL_SPEED_C		;	///< �������x�R�[�h�i�ő�j
	static const std::vector<int>			DOMAIN_MIN_LEGAL_SPEED_C		;	///< �������x�R�[�h�i�ŏ��j
	static const std::vector<int>			DOMAIN_TRAVEL_DIRECTION_C		;	///< �i�s������ʃR�[�h
	static const std::vector<int>			DOMAIN_ROAD_STRUCTURE_C			;	///< ���H�\����ʃR�[�h

	static const std::vector<int>			DOMAIN_POINT_CLASS_C			;	///< �|�C���g��ʃR�[�h
	static const std::vector<int>			DOMAIN_LINEAR_ELEMENT_C			;	///< ���`�v�f�����R�[�h
	
	static const std::vector<int>			DOMAIN_COMPART_LINE_C			;	///< ����ʃR�[�h
	static const std::vector<int>			DOMAIN_COMPART_STYLE_C			;	///< �����ݒu�l���R�[�h
	static const std::vector<int>			DOMAIN_COMPART_SLOW_C			;	///< �����\���R�[�h

	static const std::vector<int>			DOMAIN_LINK_DIRECTION_C			;	///< �����N�����R�[�h
	
	static const std::vector<int>			DOMAIN_SIGNAL_MOLD_C			;	///< �M���@�`��R�[�h

	static const std::vector<uh::tstring>	DOMAIN_ROADSIGN_C				;	///< ���H�W���R�[�h

	static const std::vector<long long>		DOMAIN_SRID						;	///< ��ԎQ��ID

	static const std::vector<int>			DOMAIN_UPDATE_C					;	///< �X�V�R�[�h

	// �t���O
	static const std::vector<int>			DOMAIN_FLAG						;	///< �t���O�l
	
	
	// �Œ�l
	static const double						CONST_CURV_STRAIGHT				;	///< �ȗ�:����
	static const double						CONST_CURV_RAD_STRAIGHT			;	///< �ȗ����a:����
	static const double						CONST_CURV_CH_RA_STRT_PNT		;	///< �ȗ��ω���:�N�_
	static const double						CONST_CLOT_PARAM_STRAIGHT		;	///< �N���\�C�h�p�����[�^:����

	//�f�[�^����
	//std::vector�̏ꍇ�A��ʂ̃f�[�^������capacity�g�����Ƀ������s�����������߁Astd::list�Ŏ���
	std::list<LaneLink>					listLaneLink;				///< �Ԑ������N
	std::list<LanePoint>				listLanePoint;				///< �Ԑ��|�C���g
	std::list<RelLaneLink>				listRelLaneLink;			///< �Ԑ��֘A
	std::list<CompartLink>				listCompartLink;			///< ���������N
	std::list<CompartPoint>				listCompartPoint;			///< �����|�C���g
	std::list<RelCompartLink>			listRelCompartLink;			///< �����֘A
	std::list<BorderLink>				listBorderLink;				///< ���E�������N
	std::list<BorderPoint>				listBorderPoint;			///< ���E���|�C���g
	std::list<RelBorderLink>			listRelBorderLink;			///< ���E���֘A
	std::list<SignalPoint>				listSignalPoint;			///< �M���@�|�C���g
	std::list<RelSignalPoint>			listRelSignalPoint;			///< �M���@�֘A
	std::list<RoadSignPoint>			listRoadSignPoint;			///< �W���|�C���g
	std::list<RelRoadSignPoint>			listRelRoadSignPoint;		///< �W���֘A
	std::list<CrossWalkLine>			listCrossWalkLine;			///< ���f�������C��
	std::list<RelCrossWalkLine>			listRelCrossWalkLine;		///< ���f�����֘A
	std::list<VirtualCompartLink>		listVirtualCompartLink;		///< ���z�������N
	std::list<VirtualCompartPoint>		listVirtualCompartPoint;	///< ���z���|�C���g
	std::list<RelVirtualCompartLink>	listRelVirtualCompartLink;	///< �Ԑ������N�|���z�������N �֘A
	std::list<CoveringArea>				listCoveringArea;			///< �������G���A
	std::list<SpatialRef>				listSpatialRef;				///< ��ԎQ��
	std::list<Version>					listVersion;				///< �o�[�W����
};

typedef std::shared_ptr<SurveyCSV> SurveyCSVPtr;

}; //adam_v2
