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
#include "util.h"
#include "OutputLog.h"
#include "HeightInfoMgr.h"

/**
 * @brief HEIGT_NODE���쐬����Ă���ׂ��ӏ��ɑ��݂��邩�ǂ����`�F�b�N����
 * @detail ���̃`�F�b�N�����ŁA1�s�s�n�}���b�V���̏������Ԃ�1�����قǂ�����B
 * �������Ղ��悤�Ɉ�N���X�Ƃ��Ē�`�B
 */
class HeightNodePositionCheckFunction
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param out           [in] ���O�o�͊Ǘ��N���X�̎Q��
	 * @param heightNodeMgr [in] �d�Ȃ胊���N�����m�[�h�ݒ�Ǘ��N���X
	 * @param coordinateTolerance [in] DB���e�l(�x)
	 */
	HeightNodePositionCheckFunction(
		COutPut& out,
		const HeightInfoMgr& heightNodeMgr,
		double coordinateTolerance ) :
		m_output( out ),
		m_heightNodeMgr( heightNodeMgr ),
		m_coordinateTolerance( coordinateTolerance )

	{}

	/**
	 * @brief	HEIGHT_NODE�̈ʒu���`�F�b�N
	 * @param	meshcode    [in] �`�F�b�N�Ώۃ��b�V���R�[�h
	 * @param	meshGeom    [in] �`�F�b�N�Ώۂ̃W�I���g��
	 * @param	roadLink    [in] �`�F�b�N�Q�Ɨp���H�����N�Q�N���X
	 * @param	walkLink    [in] �`�F�b�N�Q�Ɨp���s�҃����N�Q�N���X
	 * @param	walkNode    [in] �`�F�b�N�Q�Ɨp���s�҃m�[�h�Q�N���X
	 * @param	heightNode      [in] �`�F�b�N�Q�Ɨp�d�Ȃ胊���N�̍����m�[�h�Q�N���X
	 * @param	heightNodeRTree [in] �`�F�b�N�Q�Ɨp�d�Ȃ胊���N�̍����m�[�hRTree
	 */
	void checkPosition(
		const CString& meshcode,
		const IGeometryPtr& meshGeom,
		const FeaturesPack& roadLink,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const FeaturesPack& heightNode,
		const BGRTree& heightNodeRTree
		);

protected:
	/**
	 * @brief  �����̃^�C�v
	 */
	enum TYPE : unsigned
	{
		ROAD = 0,	//!< ���H
		WALK = 1,	//!< ���s��
	};

	/**
	 * @brief  �Ώۂ̃����N��HEIGHT_NODE�쐬�\�ȑ�����
	 * @param  geoClass [in] �����N�̑����E�`��
	 * @param  type     [in] ���s�� or ���H��
	 * @return �쐬�̉�
	 */
	bool isCreatableAttribute(
		const GeoClass& geoClass,
		const sindy::CFieldMap& fieldMap,
		TYPE type
	);

	/**
	 * @brief ��̌`��̌�_���Z�o���A
	 * ���̈ʒu��HEIGHT_NODE�A����сAWALK_NODE�����݂��Ȃ��Ȃ�G���[�Ƃ���
	 * @param base            [in] �`��1
	 * @param target          [in] �`��2
	 * @param walkNodeUnion   [in] WALK_NODE�̌`���Union�����`��i�}���`�|�C���g�j
	 * @param heightNodeUnion [in] HEIGHT_NODE�̌`���Union�����`��i�}���`�|�C���g�j
	 * @param heightNodeRTree [in] HEIGHT_NODE��RTree
	 * @param meshcode        [in] �`�F�b�N�Ώۃ��b�V���R�[�h
	 * @param meshGeom        [in] �`�F�b�N�Ώۂ̃W�I���g��
	 */
	void checkCrossPoint(
		const IGeometryPtr& base,
		const IGeometryPtr& target,
		const IGeometryPtr& walkNodeUnion,
		const IGeometryPtr& heightNodeUnion, 
		const BGRTree& heightNodeRTree,
		const CString& meshcode,
		const IGeometryPtr& meshGeom
		);

protected:
	COutPut& m_output;						//!< ���O�o�͗p�C���X�^���X�ւ̎Q��
	const HeightInfoMgr& m_heightNodeMgr;	//!< �d�Ȃ胊���N�����m�[�h�ݒ�Ǘ��N���X
	double m_coordinateTolerance;			//!< DB���e�l(�x)
	ISpatialReferencePtr m_ipSP;			//!< ��ԎQ��
};
