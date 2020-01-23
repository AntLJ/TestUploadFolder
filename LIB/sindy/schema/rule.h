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

#ifndef SINDY_SCHEMA_RULE_H_
#define SINDY_SCHEMA_RULE_H_

namespace sindy {

namespace schema {

/// ���L���[���e�[�u��
namespace annotation_class {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kClass[];				//!< SiNDY���L��ʃR�[�h
	extern const _TCHAR kMSNo[];				//!< ���e�ԍ�
	extern const _TCHAR kInClass[];				//!< IN���L��ʃR�[�h
	extern const _TCHAR kClassName[];			//!< ��ʖ���
	extern const _TCHAR kTopFlag[];				//!< TOP_FLAG
	extern const _TCHAR kMiddleFlag[];			//!< MIDDLE_FLAG
	extern const _TCHAR kBaseFlag[];			//!< BASE_FLAG
	extern const _TCHAR kCityFlag[];			//!< CITY_FLAG
	extern const _TCHAR kDispType[];			//!< �\���^�C�v
	extern const _TCHAR kTS1DispRule[];			//!< �g�b�v�}�b�vS1�\�����[��
	extern const _TCHAR kTS2DispRule[];			//!< �g�b�v�}�b�vS2�\�����[��
	extern const _TCHAR kTS3DispRule[];			//!< �g�b�v�}�b�vS3�\�����[��
	extern const _TCHAR kTS4DispRule[];			//!< �g�b�v�}�b�vS4�\�����[��
	extern const _TCHAR kTTruePosition[];		//!< �g�b�v�}�b�v�^�ʒu�t���O
	extern const _TCHAR kTFontSize[];			//!< �g�b�v�}�b�v�t�H���g�T�C�Y
	extern const _TCHAR kTExceptScale[];		//!< �g�b�v�}�b�v�X�P�[���̒������t���O
	extern const _TCHAR kTDoubleAnno[];			//!< �g�b�v�}�b�v�Q�s���L�t���O
	extern const _TCHAR kTCopyPaste[];			//!< �g�b�v�}�b�v�R�s�[�y�[�X�g�t���O
	extern const _TCHAR kTIDenticalData[];		//!< �g�b�v�}�b�v����f�[�^�̑��݃t���O
	extern const _TCHAR kMS1DispRule[];			//!< �~�h���}�b�vS1�\�����[��
	extern const _TCHAR kMS2DispRule[];			//!< �~�h���}�b�vS2�\�����[��
	extern const _TCHAR kMS3DispRule[];			//!< �~�h���}�b�vS3�\�����[��
	extern const _TCHAR kMS4DispRule[];			//!< �~�h���}�b�vS4�\�����[��
	extern const _TCHAR kMTruePosition[];		//!< �~�h���}�b�v�^�ʒu�t���O
	extern const _TCHAR kMFontSize[];			//!< �~�h���}�b�v�t�H���g�T�C�Y
	extern const _TCHAR kMExceptScale[];		//!< �~�h���}�b�v�X�P�[���̒������t���O
	extern const _TCHAR kMDoubleAnno[];			//!< �~�h���}�b�v�Q�s���L�t���O
	extern const _TCHAR kMCopyPaste[];			//!< �~�h���}�b�v�R�s�[�y�[�X�g�t���O
	extern const _TCHAR kMIDenticalData[];		//!< �~�h���}�b�v����f�[�^�̑��݃t���O
	extern const _TCHAR kBS1DispRule[];			//!< �x�[�X�}�b�vS1�\�����[��
	extern const _TCHAR kBS2DispRule[];			//!< �x�[�X�}�b�vS2�\�����[��
	extern const _TCHAR kBS3DispRule[];			//!< �x�[�X�}�b�vS3�\�����[��
	extern const _TCHAR kBS4DispRule[];			//!< �x�[�X�}�b�vS4�\�����[��
	extern const _TCHAR kBTruePosition[];		//!< �x�[�X�}�b�v�^�ʒu�t���O
	extern const _TCHAR kBFontSize[];			//!< �x�[�X�}�b�v�t�H���g�T�C�Y
	extern const _TCHAR kBExceptScale[];		//!< �x�[�X�}�b�v�X�P�[���̒������t���O
	extern const _TCHAR kBDoubleAnno[];			//!< �x�[�X�}�b�v�Q�s���L�t���O
	extern const _TCHAR kBCopyPaste[];			//!< �x�[�X�}�b�v�R�s�[�y�[�X�g�t���O
	extern const _TCHAR kBIDenticalData[];		//!< �x�[�X�}�b�v����f�[�^�̑��݃t���O
	extern const _TCHAR kCDispRule[];			//!< �s�s�n�}S1�\�����[��
	extern const _TCHAR kCTruePosition[];		//!< �s�s�n�}�^�ʒu�t���O
	extern const _TCHAR kCFontSize[];			//!< �s�s�n�}�t�H���g�T�C�Y
	extern const _TCHAR kCExceptScale[];		//!< �s�s�n�}�X�P�[���̒������t���O
	extern const _TCHAR kCDoubleAnno[];			//!< �s�s�n�}�Q�s���L�t���O
	extern const _TCHAR kCCopyPaste[];			//!< �s�s�n�}�R�s�[�y�[�X�g�t���O
	extern const _TCHAR kCIDenticalData[];		//!< �s�s�n�}����f�[�^�̑��݃t���O
	extern const _TCHAR kRotationRule[];		//!< ��]�Ɋ֘A���郋�[��

	/// �X�P�[���\�����[��
	namespace scaledisp_rule {
		enum ECode {
			kOOOOO						= 0,	//!< �L��+������ ... �w���_���\						-> kAny (10)
			kOXOXO						= 1, //!< �L��+������(�w���_�s��) ... �w���_���E�L���̂ݕs��	-> kNorDirPoint (50)
			kOOXXX						= 2, //!< �L���̂�												-> kMarkOnly (90)
			kOXOXX						= 3, //!< ������̂�											-> kStrOnly (100)
			kXXXXX						= 4, //!< �\������												-> kNone (999)
			kAny						= 10,	//!< �w+��|�L+��|�L�̂�|���̂�|�\����	 �c ���ł��L��
			kAnyMust					= 11,	//!< �w+��|�L+��|�L�̂�|���̂�|�~		 �c ���ł��L��i�\���K�{�j
			kMustMark					= 20,	//!< �w+��|�L+��|�L�̂�|    �~|�\����	 �c �L���K�{
			kMustMarkMust				= 21,	//!< �w+��|�L+��|�L�̂�|    �~|�~		 �c �L���K�{�i�\���K�{�j
			kMustStr					= 30,	//!< �w+��|�L+��|    �~|���̂�|�\����	 �c ������K�{
			kMustStrMust				= 31,	//!< �w+��|�L+��|    �~|���̂�|�~		 �c ������K�{�i�\���K�{�j
			kMustPair					= 40,	//!< �w+��|�L+��|    �~|    �~|�\����	 �c �L���ƕ����K���y�A
			kMustPairMust				= 41,	//!< �w+��|�L+��|    �~|    �~|�~		 �c �L���ƕ����K���y�A�i�\���K�{�j
			kNotDirPoint				= 50,	//!< �~   |�L+��|�L�̂�|���̂�|�\����	 �c �w���_�s��
			kNotDirPointMust			= 51,	//!< �~   |�L+��|�L�̂�|���̂�|�~		 �c �w���_�s�i�\���K�{�j
			kNotDirPointMultMark		= 60,	//!< �~   |�L+��|�L�̂�|    �~|�\����	 �c �w���_�s�L���K�{
			kNotDirPointMultMarkMust	= 61,	//!< �~   |�L+��|�L�̂�|    �~|�~		 �c �w���_�s�L���K�{�i�\���K�{�j
			kNotDirPointMustStr			= 70,	//!< �~   |�L+��|    �~|���̂�|�\����	 �c �w���_�s������K�{
			kNotDirPointMustStrMust		= 71,	//!< �~   |�L+��|    �~|���̂�|�~		 �c �w���_�s������K�{�i�\���K�{�j
			kMarkStr					= 80,	//!< �~   |�L+��|    �~|    �~|�\����	 �c �L���i�w���_�s�j+������̂�
			kMarkStrMust				= 81,	//!< �~   |�L+��|    �~|    �~|�~		 �c �L���i�w���_�s�j+������̂݁i�\���K�{�j
			kMarkOnly					= 90,	//!< �~   |   �~|�L�̂�|    �~|�\����	 �c �L���i�w���_�s�j�̂�
			kMarkOnlyMust				= 91,	//!< �~   |   �~|�L�̂�|    �~|�~		 �c �L���i�w���_�s�j�̂݁i�\���K�{�j
			kStrOnly					= 100,	//!< �~   |   �~|    �~|���̂�|�\����	 �c ������̂�
			kStrOnlyMust				= 101,	//!< �~   |   �~|    �~|���̂�|�~		 �c ������̂݁i�\���K�{�j
			kNone						= 999,	//!< �~   |   �~|    �~|    �~|�\����	 �c �\���Ȃ�
		};
	} // scaledisp_rule

	/// ���L�\��������\���^�C�v�R�[�h
	namespace disptype_rule {
		enum ECode {
			kCommon			= 0, //!< ���
			kLinear			= 1, //!< ����
			kFlag			= 2, //!< ���g��
			kRouteNumber	= 3, //!< ���H�ԍ�
			kSpecialLinear	= 4, //!< �������
		};
	} // disp_type
} // annotation_class

/// �w�i���[���e�[�u��
namespace background_class {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kClass[];				//!< ��ʃR�[�h
	extern const _TCHAR kClassName[];			//!< ��ʖ���
	extern const _TCHAR kMSNo[];				//!< ���e�ԍ�
	extern const _TCHAR kLayerID[];				//!< �w�i���C����ID
	extern const _TCHAR kUnderGround[];			//!< �n���t���O
	extern const _TCHAR kInClass[];				//!< IN��ʃR�[�h

	/// �w�i���C���R�[�h
	namespace background_layer_id {
		enum ECode {
			kCommon				= 0,	//!< ����
			kTopLine			= 1,	//!< �g�b�v���C��
			kTopSite			= 4,	//!< �g�b�v�w�i
			kTopContour			= 5,	//!< �g�b�v�i��
			kMiddleLine			= 11,	//!< �~�h�����C��
			kMiddleRail			= 12,	//!< �~�h���S��
			kMiddleSta			= 13,	//!< �~�h���S���w
			kMiddleSite			= 14,	//!< �~�h���w�i
			kMiddleContour		= 15,	//!< �~�h���i��
			kBaseLine			= 21,	//!< �x�[�X���C��
			kBaseRail			= 22,	//!< �x�[�X�S��
			kBaseSta			= 23,	//!< �x�[�X�S���w
			kBaseSite			= 24,	//!< �x�[�X�w�i
			kBaseContour		= 25,	//!< �x�[�X�i��
			kBaseGround			= 26,	//!< �x�[�X���n
			kCityLine			= 31,	//!< �s�s���C��
			kCityRail			= 32,	//!< �s�s�S��
			kCitySta			= 33,	//!< �s�s�S���w
			kCitySite			= 34,	//!< �s�s�w�i
			kCityBuilding		= 35,	//!< �s�s����
			kCityBuildingLine	= 36,	//!< �s�s�������C��
		};
	} // background_layer_id
} // background_class

/// ��ƔN�x�Ǘ��e�[�u��
namespace alley_mainte_list {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kAlleyMainteList[];		//!< �׊X�H��Ə��R�[�h
	extern const _TCHAR kWorkYear[];			//!< ��ƔN�x
	extern const _TCHAR Regulation[];			//!< �K�������t���O
} // allay_mainte_list

/// �ڑ����[�U���Ǘ��e�[�u��
namespace connectuser {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kConnectUserID[];		//!< �ڑ����[�UID
	extern const _TCHAR kName[];				//!< �ڑ����[�U��
} // connectuser

/// �쐬�\���C���Ǘ��e�[�u��
namespace creatablelayers {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kPurposeID[];			//!< ��ƖړIID
	extern const _TCHAR kLayerNameID[];			//!< ���C����ID
	extern const _TCHAR kAutoFlag[];			//!< �����쐬�t���O
	extern const _TCHAR kAutoValue[];			//!< �����쐬�����l
} // creatablelayers

/// �폜�\���C���Ǘ��e�[�u��
namespace deletablelayers {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kPurposeID[];			//!< ��ƖړIID
	extern const _TCHAR kLayerNameID[];			//!< ���C����ID
	extern const _TCHAR kRistrict1[];			//!< �����������[���P
	extern const _TCHAR kRistrict2[];			//!< �����������[���Q
	extern const _TCHAR kRistrict3[];			//!< �����������[���R
	extern const _TCHAR kRistrict4[];			//!< �����������[���S
	extern const _TCHAR kRistrict5[];			//!< �����������[���T
	extern const _TCHAR kRistrict6[];			//!< �����������[���U
	extern const _TCHAR kRistrict7[];			//!< �����������[���V
	extern const _TCHAR kRistrict8[];			//!< �����������[���W
	extern const _TCHAR kRistrict9[];			//!< �����������[���X
	extern const _TCHAR kRistrict10[];			//!< �����������[���P�O
} // deletablelayers

/// ���H�����N�̕\����ʂƌo�H��ʂ̊֘A��`�p�e�[�u��
namespace dispnavirelation {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kDispClassID[];			//!< ���H�����N�\�����ID
	extern const _TCHAR kNaviClassID[];			//!< ���H�����N�o�H���ID
	extern const _TCHAR kRelValue[];			//!< ���[���l
} // dispnavirelation

/// �H�����N�̕\����ʂƂ���ȊO�̑����̊֘A��`�p�e�[�u��
namespace dispotherrelation {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kDispClassID[];			//!< ���H�����N�\�����ID
	extern const _TCHAR kOtherAttrID[];			//!< ���H�����N����ID
	extern const _TCHAR kRelValue[];			//!< ���[���l
} // dispotherrelation

/// �����ҏW�\���C���Ǘ��e�[�u��
namespace editattrlayers {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kPurposeID[];			//!< ��ƖړIID
	extern const _TCHAR kLayerNameID[];			//!< ���C����ID
	extern const _TCHAR kRistrict1[];			//!< �����������[���P
	extern const _TCHAR kRistrict2[];			//!< �����������[���Q
	extern const _TCHAR kRistrict3[];			//!< �����������[���R
	extern const _TCHAR kRistrict4[];			//!< �����������[���S
	extern const _TCHAR kRistrict5[];			//!< �����������[���T
	extern const _TCHAR kRistrict6[];			//!< �����������[���U
	extern const _TCHAR kRistrict7[];			//!< �����������[���V
	extern const _TCHAR kRistrict8[];			//!< �����������[���W
	extern const _TCHAR kRistrict9[];			//!< �����������[���X
	extern const _TCHAR kRistrict10[];			//!< �����������[���P�O
} // editattrlayers

/// ���[���ҏW�����e�[�u��
namespace edithistory {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kModifyDate[];			//!< �ύX����
	extern const _TCHAR kModifyTable[];			//!< �ύX�������[���e�[�u����
	extern const _TCHAR kModifyOID[];			//!< �ύX����OBJECTID�ԍ�
	extern const _TCHAR kUpdateType[];			//!< �ύX���@
	extern const _TCHAR kOperator[];			//!< �ύX�Җ�
	extern const _TCHAR kMessage[];				//!< ���b�Z�[�W
} // edithistory

/// �ҏW�\���b�V���Ǘ��e�[�u��
namespace editmesh {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kMeshCode[];			//!< ���b�V���R�[�h
	extern const _TCHAR kLayerNameID[];			//!< ���C����ID
	extern const _TCHAR kPurposeID[];			//!< ��ƖړIID
	extern const _TCHAR kOperatorID[];			//!< �I�y���[�^ID
} // editmesh

/// ���C�����Ǘ��e�[�u��
namespace layernames {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kLayerName[];			//!< ���C����
	extern const _TCHAR kLayerNameID[];			//!< ���C����ID
	extern const _TCHAR kLayerAliasName[];		//!< ���C���G�C���A�X��
} // layernames

/// ���b�N�G���A�Ǘ��e�[�u��
namespace lockarea {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kLayerNameID[];			//!< ���C����ID
	extern const _TCHAR kConnectUserID[];		//!< �ڑ����[�U��ID
} // lockarea

/// ���b�N���b�V���Ǘ��e�[�u��
namespace lockmesh {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kMeshCode[];			//!< ���b�V���R�[�h
	extern const _TCHAR kLayerNameID[];			//!< ���C����ID
	extern const _TCHAR kConnectUserID[];		//!< �ڑ����[�U��ID
} // lockmesh

/// �ړ��\���C���Ǘ��e�[�u��
namespace movablelayers {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kPurposeID[];			//!< ��ƖړIID
	extern const _TCHAR kLayerNameID[];			//!< ���C����ID
	extern const _TCHAR kRistrict1[];			//!< �����������[���P
	extern const _TCHAR kRistrict2[];			//!< �����������[���Q
	extern const _TCHAR kRistrict3[];			//!< �����������[���R
	extern const _TCHAR kRistrict4[];			//!< �����������[���S
	extern const _TCHAR kRistrict5[];			//!< �����������[���T
	extern const _TCHAR kRistrict6[];			//!< �����������[���U
	extern const _TCHAR kRistrict7[];			//!< �����������[���V
	extern const _TCHAR kRistrict8[];			//!< �����������[���W
	extern const _TCHAR kRistrict9[];			//!< �����������[���X
	extern const _TCHAR kRistrict10[];			//!< �����������[���P�O
} // movablelayers

/// ��ƎҊǗ��e�[�u��
namespace soperator {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kOperatorID[];			//!< �I�y���[�^��ID
	extern const _TCHAR kName[];				//!< �I�y���[�^��
	extern const _TCHAR kAliasName[];			//!< �I�y���[�^���G�C���A�X
} // operator

/// ���H�������̊Ǘ��e�[�u��
namespace roadattribute {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kName[];				//!< ���H�����N������
	extern const _TCHAR kAliasName[];			//!< ���H�����N�������G�C���A�X
	extern const _TCHAR kNameID[];				//!< ���H�����N������ID
} // roadattribute

/// ���H�����N�\����ʖ��Ǘ��e�[�u��
namespace roaddispclass {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kName[];				//!< ���H�����N�\����ʖ�
	extern const _TCHAR kNameID[];				//!< ���H�����N�\����ʖ�ID
} // roaddispclass

/// ���H�����N�o�H��ʖ��Ǘ��e�[�u��
namespace roadnaviclass {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kName[];				//!< ���H�����N�o�H��ʖ�
	extern const _TCHAR kNameID[];				//!< ���H�����N�o�H��ʖ�ID
} // roadnaviclass

/// ���L�폜��`�Ǘ��e�[�u��
namespace shareddeletelayers {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kSetID[];				//!< ���L�폜���C���Z�b�g�ԍ�
	extern const _TCHAR kLayerNameID[];			//!< ���C����ID
} // shareddeletelayers

/// ���L�ړ���`�Ǘ��e�[�u��
namespace sharedmovelayers {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kSetID[];				//!< ���L�ړ����C���Z�b�g�ԍ�
	extern const _TCHAR kLayerNameID[];			//!< ���C����ID
} // sharedmovelayers

/// ��ƖړI�Ǘ��e�[�u��
namespace workpurpose {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kPurpose[];				//!< ��ƖړI��
	extern const _TCHAR kMXD[];					//!< MXD�t�@�C����
	extern const _TCHAR kAutoAttrGiving[];		//!< ���������t�^�l
	extern const _TCHAR kWorkKind[];			//!< ��Ǝ��
	extern const _TCHAR kGencho[];				//!< �������f��Ɨp�t���O
	extern const _TCHAR kClaim[];				//!< �N���[���C����Ɨp�t���O
	extern const _TCHAR kChuki[];				//!< ���L�ҏW��Ɨp�t���O
	extern const _TCHAR kPurposeID[];			//!< ��ƖړI��ID
	extern const _TCHAR kDelete[];				//!< �폜�ς݃t���O
	extern const _TCHAR kTest[];				//!< �e�X�g�p�t���O
	extern const _TCHAR kCategory[];			//!< ��ƃJ�e�S��
	extern const _TCHAR kLayerFilePath[];		//!< ���C���t�@�C���p�X
	extern const _TCHAR kCanSave[];				//!< �ۑ��s�t���O
} // workpurpose

// PGDB�o�͏��e�[�u��
namespace history_table {

	extern const _TCHAR kTableName[];    //!< �e�[�u����

	extern const _TCHAR kObjectID[];     //!< �I�u�W�F�N�gID
	extern const _TCHAR kName[];         //!< ����
	extern const _TCHAR kFeatureClass[]; //!< �t�B�[�`���N���X��
	extern const _TCHAR kConnectInfo[];  //!< �ڑ����
	extern const _TCHAR kUser[];         //!< ���[�U��
	extern const _TCHAR kMachine[];      //!< �}�V����
	extern const _TCHAR kDate[];         //!< ���t
	extern const _TCHAR kOperation[];    //!< ��Ɩ�
} // history_table

} // schema

} // sindy

#endif // ifndef SINDY_SCHEMA_RULE_H_
