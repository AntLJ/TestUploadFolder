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
* @file define.h
*
* @brief SiNDY�֘A�̌Œ�l��`�t�@�C��
*/
#ifndef __SINDY_ARCHELPER_DEFINE_ENUM
#define __SINDY_ARCHELPER_DEFINE_ENUM

#include "sindy/schema/road.h" 

/**
 * ���H�m�[�h��ʃR�[�h
 *
 * @note ���H�m�[�h�̎�ʂƈ�v������K�v������܂�
 * @see Documents/SiNDY-b/data_model/SiNDY���H�����p�����[�^.xls
 */
enum sindyeNodeClass
{
	sindyeNodeNoDef				= 0,	//!< �Ӗ��i�V�m�[�h
	sindyeNodeIntersection		= 1,	//!< �����_�m�[�h
	sindyeNodeEndPoint			= 2,	//!< �[�_�m�[�h
	sindyeNodeAttrChanged		= 3,	//!< �����ω��_�m�[�h
	sindyeNodeExistAttr			= 4,	//!< �����t���m�[�h
	sindyeNodeConnectPecLink	= 5,	//!< PEC�����N�ڑ��m�[�h
	sindyeNodeNQStartPoint		= 6,	//!< �m�[�h���_�m�[�h
	sindyeHWYChargePoint		= 7,	//!< �������m�[�h
	sindyeRelHWY				= 8,	//!< �n�C�E�F�C�֌W�m�[�h
	sindyeOnMesh				= 9,	//!< 2�����b�V�����ӏ�m�[�h
	sindyeCheckSameID			= 10,	//!< ���ꃊ���NID�h�~�m�[�h
	sindyePECNode				= 11,	//!< PEC�m�[�h
	sindyeOnMeshPEC				= 12,	//!< 2�����b�V�����ӏ�PEC�m�[�h
	sindyeDammyNode				= 13,	//!< �_�~�[�{�݃m�[�h�iTF�p�j
};

/**
 * ���s�҃m�[�h��ʃR�[�h
 *
 * @note ���s�҃m�[�h�̎�ʂƈ�v������K�v������܂�
 */
enum sindyeWalkNodeClass
{
	// ������enum�͎g�킸��schema���g������
	sindyeWalkNodeCommon			= sindy::schema::walk_node::node_class::kPublic,	        //!< ��ʃm�[�h
	sindyeWalkNodeEndPoint			= sindy::schema::walk_node::node_class::kTerminal,	        //!< �[�_�m�[�h
	sindyeWalkNodeAttrChanged		= sindy::schema::walk_node::node_class::kAlter,	            //!< �����J�ړ_�m�[�h
	sindyeWalkNodeOnRoadNode		= sindy::schema::walk_node::node_class::kRoadNW,	        //!< ���H�m�v�ڑ��m�[�h
	sindyeWalkNodeStation			= sindy::schema::walk_node::node_class::kGate,	            //!< �w�o�����m�[�h
	sindyeWalkNodeUndergroundGate	= sindy::schema::walk_node::node_class::kUndergroundGate,	//!< �n���X�o�����m�[�h
	sindyeWalkNodeElevetor			= sindy::schema::walk_node::node_class::kElevator,	        //!< �G���x�[�^�m�[�h
	sindyeWalkNodeOnMesh			= sindy::schema::walk_node::node_class::kOutline,	        //!< �}�s��m�[�h
};

/**
* @brief �ҏW���[�h
*/
enum sindyeEditMode
{
	sindyeNone        = 0,	//!< �ʏ탂�[�h
	sindyeViewOnly    = 1,	//!< �m�F���[�h
	sindyeViewSearch  = 2,	//!< �m�F�������[�h
	sindyeFullControl = 3	//!< �t���R���g���[�����[�h
};
/**
* @brief �t�B�[�`������p
*/
enum sindyeFeatureKinds
{
	sindyeUnknownFeature = 0,	//!< �s���ȃt�B�[�`��
	sindyeNodeFeature    = 1,	//!< �m�[�h�t�B�[�`��
	sindyeRoadFeature    = 2,	//!< ���H�t�B�[�`��
	sindyePointFeature   = 3,	//!< �|�C���g�t�B�[�`��
	sindyeLineFeature    = 4,	//!< ���C���t�B�[�`��
	sindyePolygonFeature = 5	//!< �|���S���t�B�[�`��
};
/**
* @brief �q�b�g�e�X�g�p
*/
enum sindyeFeatureHitType
{
	sindyeHitNone        = 0,	//!< �ǂ��ɂ��q�b�g���Ȃ�
	sindyeHitEndPoint    = 1,	//!< ���[�_�݂̂Ƀq�b�g����
	sindyeHitVertex      = 2,	//!< ���[�_�ȊO�̒��_�Ƀq�b�g����
	sindyeHitBoundary    = 3	//!< ���_���܂܂Ȃ��_�Ƀq�b�g����
};
/**
* @brief �X�i�b�v�p
*/
enum sindyeSnappingType
{
	sindyeSnapNone             = 0,	//!< �Ȃ�
	sindyeSnapNode             = 1,	//!< �m�[�h�ւ̃X�i�b�v
	sindyeSnapRoad             = 2,	//!< ���H�ւ̃X�i�b�v
	sindyeSnapRoadIntersection = 4,	//!< ���̌����_
	sindyeSnapAny              = 64	//!< ����ȊO�̉����B
};

/**
* @brief �X�V�^�C�v���
*
* @note �u�ύX�R�[�h�v�h���C���Ɠ����ɂ��邱��
*/
enum sindyUpdateType
{
	sindyUpdateTypeNoUpdate = 0,	//!< �ڍs������
	sindyUpdateTypeCreate   = 1,	//!< �V�K�쐬
	sindyUpdateTypeDelete   = 2,	//!< �폜
	sindyUpdateTypeShape    = 3,	//!< �`��ύX
	sindyUpdateTypeProperty = 4,	//!< �����ύX
};

// �`��`�F�b�N���ʃ��x���i�����͂̋������ɕ��ׂ邱�Ɓj
enum sindyeGeometryCheck
{
	sindyeGeometryNoCheck,			//!< �`�F�b�N�ł��Ȃ�
	sindyeGeometryOK,				//!< OK
	sindyeGeometryWarnning,			//!< ���[�j���O
	sindyeGeometryError,			//!< �G���[
};

// �W�I���g����r���Z�p
enum sindyeSpatialRel
{
	sindyeSpatialRelNone		= 0,	//!< �֘A���Ȃ�
	sindyeSpatialRelTouch		= 1,	//!< �ڐG
	sindyeSpatialRelOverlap		= 2,	//!< �I�[�o�[���b�v
	sindyeSpatialRelCross		= 4,	//!< ����
	sindyeSpatialRelWithin		= 8,	//!< �܂܂��
	sindyeSpatialRelContain		= 16,	//!< �܂�ł���
	sindyeSpatialRelDisjoint	= 32,	//!< ����
	sindyeSpatialRelEqual		= 64,	//!< ������
};

#endif
