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

#ifndef SINDY_SCHEMA_POI_H_
#define SINDY_SCHEMA_POI_H_

#include <sindy/schema/common.h>

namespace sindy {

namespace schema {

// POI�S����Ѓe�[�u��
namespace railway_corp {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kCorpCode[];		//!< ��ЃR�[�h
	extern const _TCHAR kName[];			//!< ��Ж�
	extern const _TCHAR kYomi[];			//!< ��Ж��ǂ�
	extern const _TCHAR kExName[];			//!< ��З���
	extern const _TCHAR kExYomi[];			//!< ��З��̓ǂ�

} // railway_corp

// POI�S���H���e�[�u��
namespace railway_line {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kCorpID[];			//!< �S����Ђh�c
	extern const _TCHAR kLineCode[];		//!< �H���R�[�h
	extern const _TCHAR kName[];			//!< �H����
	extern const _TCHAR kYomi[];			//!< �H�����ǂ�
	extern const _TCHAR kExName[];			//!< �H������
	extern const _TCHAR kExYomi[];			//!< �H�����̓ǂ�
	extern const _TCHAR kTypeID[];			//!< ��ʂh�c

	namespace type_code {
		enum ECode {
			kJR				= 1,		//!< �i�q
			kPrivate		= 2,		//!< ���S
			kThirdSector	= 3,		//!< ��R�Z�N�^�[
			kSubway			= 4,		//!< �n���S
			kNewrail		= 5,		//!< �V���
			kMonorail		= 6,		//!< ���m���[��
			kStreetrail		= 7,		//!< �H�ʓS��
			kRopeway		= 8,		//!< ���[�v�E�F�C
			kCableCar		= 9			//!< �P�[�u���J�[
		};
	} // typecode

} // railway_line

// ������H���e�[�u��
namespace join_line_info {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];	//!< �e�[�u����

	extern const _TCHAR kLineID[];		//!< �H��ID
	extern const _TCHAR kLineSeq[];		//!< �H�����V�[�P���X
	extern const _TCHAR kInfoID[];		//!< �w���ID
	extern const _TCHAR kSubName[];		//!< �⑫����
	extern const _TCHAR kSubYomi[];		//!< �⑫���̓ǂ�
	extern const _TCHAR kStationID[];	//!< �w�|�C���gID
	extern const _TCHAR kContentsSeq[];	//!< �R���e���c���V�[�P���X

} // join_line_info

// �w���}�X�^�e�[�u��
namespace station_info {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];	//!< �e�[�u����

	extern const _TCHAR kCorpID[];		//!< �S�����ID
	extern const _TCHAR kName[];		//!< �w��
	extern const _TCHAR kYomi[];		//!< �w���ǂ�
	extern const _TCHAR kAddrCode[];	//!< �Z���R�[�h
	extern const _TCHAR kTel[];			//!< �d�b�ԍ�
	extern const _TCHAR kAnAreaCode[];	//!< �s�O�ǔ�

} // station_info

// �w(������H��)�Əo�����̕R�t���e�[�u��
namespace join_station_gate {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];	//!< �e�[�u����

	extern const _TCHAR kJLineID[];		//!< ������H��ID
	extern const _TCHAR kGateID[];		//!< �w�o����ID
	extern const _TCHAR kContentsSeq[];	//!< �R���e���c���V�[�P���X

} // join_station_gate

// POI�S���w�|�C���g
namespace station_point {	
	extern const _TCHAR kTableName[];	//!< �e�[�u����

	using namespace ::sindy::schema::ipc_table;
	extern const _TCHAR kGuideID[];		//!< �U���|�C���gID

} // station_point

// POI�S���o�����|�C���g
namespace station_gate_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kName[];			//!< �o��������
	extern const _TCHAR kYomi[];			//!< �o�������̓ǂ�
	
	extern const _TCHAR kGuideID[];		//!< �U���|�C���gID

} // station_gate_point

// �U���|�C���g
namespace station_guide_point {	
	using namespace ::sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];	//!< �e�[�u����
	extern const _TCHAR kTollRoad[];	//!< �L�����H�t���O

} // station_guide_point

// �w(������H��)�ƕ��s�҂o�n�h�̕R�t���e�[�u��
namespace join_station_walk {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];	//!< �e�[�u����

	extern const _TCHAR kJLineID[];		//!< ������H��ID
	extern const _TCHAR kWalkPoiID[];	//!< POI�|�C���gID

} // join_station_gate

// ���s�҂o�n�h�|�C���g
namespace walk_poi_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kPOIClass[];		//!< ���s��POI���
	extern const _TCHAR kName[];			//!< ����
	extern const _TCHAR kYomi[];			//!< ���̓ǂ�
	extern const _TCHAR kFloorLevel[];		//!< �K�w�l
	extern const _TCHAR kIndoor[];			//!< �{�ݓ��t���O
	extern const _TCHAR kPicName[];			//!< �摜�t�@�C����

	namespace poi_class {
		enum ECode {
			kNone			=		 0,	//!< �������E��ƒ�
			kTaxi			=		 1, //!< �^�N�V�[����
			kMeetingSpot	=		 2, //!< �҂����킹�ꏊ
		};
	} // poi_class

} // walk_poi_point

// �`�F�[���X�}�X�^�[�R�[�h�e�[�u��
namespace poi_mstchn
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kChainCode[];		//!< �`�F�[���X�}�X�^�[�R�[�h�i8���j
	extern const _TCHAR kChainCodeOld[];	//!< �`�F�[���X�}�X�^�[�R�[�h�i6���j
	extern const _TCHAR kName[];			//!< �X�ܖ���
	extern const _TCHAR kYomi[];			//!< �X�ܖ��̓ǂ�
} // poi_mstchn

// �S�����ރR�[�h
namespace traintype {

	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kTypeCode[]; //!< �S�����ރR�[�h
	extern const _TCHAR kName[]; //!< ����
	extern const _TCHAR kYomi[]; //!< �ǂ�
} // traintype

} // schema

} // sindy

#endif // ifndef SINDY_SCHEMA_ANNOTATION_H_
