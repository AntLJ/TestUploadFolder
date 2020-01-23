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
#include "Properties.h"

class HeightNodeCheckFunction
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param out            [in] ���O�o�͊Ǘ��N���X�̎Q��
	 * @param dist_threshold [in] ��������臒l(m)
	 * @param coordinateTolerance [in] DB���e�l(�x)
	 * @param heightNodeMgr  [in] �d�Ȃ胊���N�����m�[�h�ݒ�Ǘ��N���X
	 * @param dataType       [in] �f�[�^�^�C�v
	 */
	HeightNodeCheckFunction(COutPut& out, double dist_threshold, double coordinateTolerance, const HeightInfoMgr& heightNodeMgr, sindy::schema::walk_type::type_class::ECode dataType) :
		m_output(out)
		, m_dist_threshold(dist_threshold)
		, m_coordinateTolerance(coordinateTolerance)
		, m_heightNodeMgr(heightNodeMgr)
		, m_dataType(dataType)
	{}

	/**
	 * @brief �d�Ȃ胊���N�����m�[�h�̑S�`�F�b�N
	 * @param ipMeshGeo       [in] �s�s�n�}���b�V���`��
	 * @param roadLink        [in] ���H�����N�Q�N���X
	 * @param segmentAttr     [in] �����N�������Q�N���X
	 * @param walkLink        [in] ���s�҃����N�Q�N���X
	 * @param heightNode      [in] �d�Ȃ胊���N�̍����m�[�h�Q�N���X
	 * @param nodeRTree       [in] ���H�m�[�h��RTree
	 * @param segmentAttrRTree[in] �����N��������RTree
	 * @param walkNodeRTree   [in] ���s�҃m�[�h��RTree
	 * @param heightNodeRTree [in] �d�Ȃ胊���N�̍����m�[�h��RTree
	 * @param checktarget     [in] �`�F�b�N�Ώ�
	 * @param isRelease		  [in] Release���[�h(�f�t�H���g:false)
	 */
	void checkHeightNode(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& segmentAttr,
		const FeaturesPack& walkLink,
		const FeaturesPack& heightNode,
		const BGRTree& nodeRTree,
		const BGRTree& segmentAttrRTree,
		const BGRTree& walkNodeRTree,
		const BGRTree& heightNodeRTree,
		properties::check_target checktarget,
		bool isRelease = false		
		);

	/**
	 * @brief release���[�h�p�̑S�`�F�b�N
	 * @param ipMeshGeo       [in] �s�s�n�}���b�V���`��
	 * @param roadLink        [in] ���H�����N�Q�N���X
	 * @param segmentAttr     [in] �����N�������Q�N���X
	 * @param walkLink        [in] ���s�҃����N�Q�N���X
	 * @param heightNode      [in] �d�Ȃ胊���N�̍����m�[�h�Q�N���X
	 * @param roadNodeRTree   [in] ���H�m�[�h��RTree
	 * @param segmentAttrRTree[in] �����N��������RTree
	 * @param walkNodeRTree   [in] ���s�҃m�[�h��RTree
	 * @param heightNodeRTree [in] �d�Ȃ胊���N�̍����m�[�h��RTree
	 * @param checktarget     [in] �`�F�b�N�Ώ�
	 */
	void checkRelease(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& segmentAttr,
		const FeaturesPack& walkLink,
		const FeaturesPack& heightNode,
		const BGRTree& roadNodeRTree,
		const BGRTree& segmentAttrRTree,
		const BGRTree& walkNodeRTree,
		const BGRTree& heightNodeRTree,
		properties::check_target checktarget
	)
	{
		checkHeightNode( ipMeshGeo, roadLink, segmentAttr, walkLink, heightNode,
			roadNodeRTree, segmentAttrRTree, walkNodeRTree, heightNodeRTree, checktarget, true );
	}

	/**
	 * @brief other���[�h�p�̑S�`�F�b�N
	 * @param [in] ipMeshGeo       �s�s�n�}���b�V���`��
	 * @param [in] heightNode �d�Ȃ胊���N�̍����m�[�h�Q�N���X
	 */
	void checkOther(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& heightNode
	) const;

	/**
	 * @brief ��O���[�h�p�̃`�F�b�N
	 * @param [in] heightNode �d�Ȃ胊���N�̍����m�[�h�Q�N���X
	 */
	void checkException(
		const FeaturesPack& heightNode
	);

protected:
	/**
	 * @brief	�d�Ȃ胊���N�����m�[�h�̏��
	 */
	typedef struct height_node_info {
		long linkID;
		long linkIDHeight;
		long linkIDTable;
		height_node_info() {
			linkID = 0;
			linkIDHeight = 0;
			linkIDTable = 0;
		};
		bool operator==(const height_node_info& cmp) {
			if (linkID == cmp.linkID && linkIDHeight == cmp.linkIDHeight && linkIDTable == cmp.linkIDTable)
				return true;
			return false;
		};
	}HEIGHT_NODE_INFO;

	/**
	 * @brief HEIGHT_NODE�̌`�󂪕s�����`�F�b�N
	 * @note �G���[�R�[�h:1017
	 * @param [in] ipTable  HEIGHT_NODE�e�[�u��
	 */
	void checkHeightNodeGeometry(const ITablePtr& ipTable) const;

	/**
	 * @brief �����NID���s���Ȓl���`�F�b�N
	 * @param [in] link1ID       �����N1��ID
	 * @param [in] link2ID       �����N2��ID
	 */
	void checkLinkID(
		long  link1ID,
		long  link2ID
	) const;

	/**
	 * @brief �|�C���g���mHEIGHT_NODE�̋������߂����`�F�b�N
	 * @param [in] heightNodeObject   �`�F�b�N�Ώۂ� ���f�|�C���g
	 * @param [in] heightNode         �d�Ȃ胊���N�̍����m�[�h�Q�N���X
	 * @param [in] heightNodeRTree    �d�Ȃ胊���N�̍����m�[�h��RTree
	 */
	void checkNearPoint(
		const GeoClass& heightNodeObject,
		const FeaturesPack& heightNode,
		const BGRTree& heightNodeRTree
	) const;

protected:
	COutPut& m_output; //!< ���O�o�͗p�C���X�^���X�ւ̎Q��
	double m_dist_threshold; //!< ��������臒l(m)
	double m_coordinateTolerance; //!< DB���e�l(�x)
	const HeightInfoMgr& m_heightNodeMgr; //!< �d�Ȃ胊���N�����m�[�h�ݒ�Ǘ��N���X
	sindy::schema::walk_type::type_class::ECode m_dataType;             //!< �f�[�^�^�C�v
};
