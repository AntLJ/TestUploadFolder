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
#include "DatabaseControl.h"

class CCheckWalk;
class RoadLinkCheckFunction
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param out               [in] ���O�o�͊Ǘ��N���X�̎Q��
	 * @param modufyFilter      [in] �ҏW�����t�B���^
	 * @param m_walkableMap     [in] ���s�҉\��ʂ̃R�[�h�l�h���C���}�b�v
	 * @param dataType           [in] �f�[�^�^�C�v
	 */
	RoadLinkCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, const std::map<long, CString>& walkableMap
		, sindy::schema::walk_type::type_class::ECode dataType
	) :
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_walkableMap(walkableMap)
		, m_dataType(dataType)
	{}

	/**
	 * @brief ���H�����N�̑S�`�F�b�N
	 * @param ipMeshGeo       [in] �s�s�n�}���b�V���`��
	 * @param roadLink        [in] ���H�����N�Q�N���X
	 * @param roadNode        [in] ���H�m�[�h�Q�N���X
	 * @param roadNodeLinkIDs [in] ���H�m�[�h�ɐڑ����铹�H�����N��ID�Ή��\
	 * @param walkLink        [in] ���s�҃����N�Q�N���X
	 * @param walkNode        [in] ���s�҃m�[�h�Q�N���X
	 * @param walkNodeLinkIDs [in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 * @param walkLinkRTree   [in] ���s�҃����N��RTree
	 * @param walkNodeRTree   [in] ���s�҃m�[�h��RTree
	 */
	void CheckRoadLink(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const NODEIDMAP& roadNodeLinkIDs,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs,
		const BGRTree& walkLinkRTree,
		const BGRTree& walkNodeRTree
	);

	/**
	 * @brief walkable���[�h�p�̃`�F�b�N
	 * @param ipMeshGeo       [in] �s�s�n�}���b�V���`��
	 * @param roadLink        [in] ���H�����N�Q�N���X
	 * @param roadNode        [in] ���H�m�[�h�Q�N���X
	 * @param roadNodeLinkIDs [in] ���H�m�[�h�ɐڑ����铹�H�����N��ID�Ή��\
	 * @param walkLink        [in] ���s�҃����N�Q�N���X
	 * @param walkNode        [in] ���s�҃m�[�h�Q�N���X
	 * @param walkNodeLinkIDs [in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 * @param walkLinkRTree   [in] ���s�҃����N��RTree
	 * @param walkNodeRTree   [in] ���s�҃m�[�h��RTree
	 */
	void checkWalkable(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const NODEIDMAP& roadNodeLinkIDs,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs,
		const BGRTree& walkLinkRTree,
		const BGRTree& walkNodeRTree
	);

	/**
	 * @brief other���[�h�p�̃`�F�b�N
	 * @param ipMeshGeo   [in] �s�s�n�}���b�V���`��
	 * @param roadLink    [in] ���H�����N�Q�N���X
	 * @param roadNodeLinkIDs [in] ���H�m�[�h�ɐڑ����铹�H�����N��ID�Ή��\
	 */
	void checkOther(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const NODEIDMAP roadNodeLinkIDs
	);


	/**
	 * @brief release���[�h�p�̃`�F�b�N
	 * @param ipMeshGeo   [in] �s�s�n�}���b�V���`��
	 * @param roadLink    [in] ���H�����N�Q�N���X
	 */
	void checkRelease(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink
	);

private:
	/**
	 * @brief walkable�̒l���Ó����`�F�b�N���A�G���[���O���o�͂���
	 * @param linkObj  [in] �`�F�b�N�Ώۂ̓��H�����N�̏��
	 * @param fieldMap [in] ���H�����N�̃t�B�[���h�}�b�v
	 */
	void checkWalkableField(
		const GeoClass& linkObj
		, const sindy::CFieldMap& fieldMap
	);

	/**
	 * @brief walkable�ȊO�̑����l���Ó����`�F�b�N
	 * @param linkObj [in] �`�F�b�N�Ώۂ̓��H�����N�̏��
	 * @param fieldMap [in] ���H�����N�̃t�B�[���h�}�b�v
	 */
	void checkOtherField(
		const GeoClass& linkObj
		, const sindy::CFieldMap& fieldMap);

	/**
	 * @brief �������Ȃ����Ă��Ȃ����Ó����`�F�b�N
	 * @node�@�G���[�R�[�h 511
	 * @param [in] linkId             �`�F�b�N�Ώۂ̓��H�����N��ID
	 * @param [in] linkObj            �`�F�b�N�Ώۂ̓��H�����N�̏��
	 * @param [in] connectFromLinkIds �`�F�b�N�Ώۂ̓��H�����N��from���ɐڑ����Ă��铹�H�����N��ID
	 * @param [in] connectToLinkIds   �`�F�b�N�Ώۂ̓��H�����N��to���ɐڑ����Ă��铹�H�����N��ID
	 * @param [in] roadLink           ���H�����N�Q�N���X
	 */
	void checkNotConnectedSideWalk(
		long linkId
		, const GeoClass& linkObj
		, const std::set<long>& connectFromLinkIds
		, const std::set<long>& connectToLinkIds
		, FeaturesPack& roadLink);

	/**
	 * @brief walkable�ȊO�̑����l���Ó����`�F�b�N
	 * @param linkId             [in] �`�F�b�N�Ώۂ̓��H�����N��ID
	 * @param linkObj            [in] �`�F�b�N�Ώۂ̓��H�����N�̏��
	 * @param connectFromLinkIds [in]
	 * �`�F�b�N�Ώۂ̓��H�����N��from���ɐڑ����Ă��铹�H�����N��ID
	 * @param connectToLinkIds   [in]
	 * �`�F�b�N�Ώۂ̓��H�����N��to���ɐڑ����Ă��铹�H�����N��ID
	 * @param roadLink           [in] ���H�����N�Q�N���X
	 * @param roadNode           [in] ���H�m�[�h�Q�N���X
	 * @param roadNodeLinkIDs    [in] ���H�m�[�h�ɐڑ����铹�H�����N��ID�Ή��\
	 * @param walkLink           [in] ���s�҃����N�Q�N���X
	 * @param walkNode           [in] ���s�҃m�[�h�Q�N���X
	 * @param walkNodeLinkIDs    [in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 * @param walkNodeRTree      [in] ���s�҃m�[�h��RTree
	 * @param meshcode           [in] �������̓s�s�n�}���b�V���R�[�h
	 * @param ipMeshGeo	         [in] �`�F�b�N�͈�
	 */
	void checkHumanCarSandwichedBetweenNotWalk(
		long linkId
		, const GeoClass& linkObj
		, const std::set<long>& connectFromLinkIds
		, const std::set<long>& connectToLinkIds
		, FeaturesPack& roadLink
		, FeaturesPack& roadNode
		, NODEIDMAP& roadNodeLinkIDs
		, FeaturesPack& walkLink
		, FeaturesPack& walkNode
		, NODEIDMAP& walkNodeLinkIDs
		, BGRTree& walkNodeRTree
		, const IGeometryPtr& ipMeshGeo);

	/**
	 * @brief walkable�ȊO�̑����l���Ó����`�F�b�N
	 * @param linkId             [in] �`�F�b�N�Ώۂ̓��H�����N��ID
	 * @param linkObj            [in] �`�F�b�N�Ώۂ̓��H�����N�̏��
	 * @param connectFromLinkIds [in]
	 * �`�F�b�N�Ώۂ̓��H�����N��from���ɐڑ����Ă��铹�H�����N��ID
	 * @param connectToLinkIds   [in]
	 * �`�F�b�N�Ώۂ̓��H�����N��to���ɐڑ����Ă��铹�H�����N��ID
	 * @param roadLink           [in] ���H�����N�Q�N���X
	 * @param roadNode           [in] ���H�m�[�h�Q�N���X
	 * @param roadNodeLinkIDs    [in] ���H�m�[�h�ɐڑ����铹�H�����N��ID�Ή��\
	 * @param walkLink           [in] ���s�҃����N�Q�N���X
	 * @param walkNode           [in] ���s�҃m�[�h�Q�N���X
	 * @param walkNodeLinkIDs    [in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 * @param walkLinkRTree      [in] ���s�҃����N��RTree
	 * @param walkNodeRTree      [in] ���s�҃m�[�h��RTree
	 * @param ipMeshGeo	         [in] �`�F�b�N�͈�
	 */
	void checkBeforeAfterLinkWalkable(
		long linkId
		, const GeoClass& linkObj
		, const std::set<long>& connectFromLinkIds
		, const std::set<long>& connectToLinkIds
		, FeaturesPack& roadLink
		, FeaturesPack& roadNode
		, NODEIDMAP& roadNodeLinkIDs
		, FeaturesPack& walkLink
		, FeaturesPack& walkNode
		, NODEIDMAP& walkNodeLinkIDs
		, BGRTree& walkLinkRTree
		, BGRTree& walkNodeRTree
		, const IGeometryPtr& ipMeshGeo);

	/**
	 * @brief	���b�V���O�̐ڑ����H�����N���擾
	 * @detail �`�F�b�N��͈͂�9�s�s�n�}���b�V���O�ւ̒T���ɔ����A<br>
	 * �`�F�b�N�͈͂͊g������A�Ή��̃����N���m�[�h�����X�V�����B<br>
	 * @param	nodeId	         [in/out] �Ώە��s�҃m�[�hID�i�T���J�n���͋N�_�m�[�h�A�I�����͏I�_�m�[�h�j
	 * @param   linkId           [in]     �Ώۃ����NID
	 * @param	ipMeshGeo	     [in/out] �`�F�b�N�͈�
	 * @param   roadLink         [in/out] ���H�����N�Q�N���X
	 * @param   roadNode         [in/out] ���H�m�[�h�Q�N���X
	 * @param   roadNodeLinkIDs  [in/out] ���H�m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 * @param   walkLink         [in/out] ���s�҃����N�Q�N���X
	 * @param   walkNode         [in/out] ���s�҃m�[�h�Q�N���X
	 * @param   walkNodeLinkIDs  [in/out] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 * @param	targetLinkIDs	 [in/out] �Ώۃ����NID�Q
	 * @return	�擾�������R�[�h
	 * @retval	nullptr	�擾�Ɏ��s
	 */
	std::set<long> getAdjacentRoadLink(
		long& nodeId,
		const long linkId,
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& roadLink,
		FeaturesPack& roadNode,
		NODEIDMAP& roadNodeLinkIDs,
		FeaturesPack& walkLink,
		FeaturesPack& walkNode,
		NODEIDMAP& walkNodeLinkIDs,
		std::set<long>& targetLinkIDs);

	/**
	 * @brief	�Ώۃm�[�h�̏��݂��郁�b�V���̏���ǉ�����
	 * @param	targetNodeId     [in] �Ώە��s�҃m�[�hID
	 * @param	ipMeshGeo	     [in] �`�F�b�N�͈�
	 * @param   roadLink         [in/out] ���H�����N�Q�N���X
	 * @param   roadNode         [in/out] ���H�m�[�h�Q�N���X
	 * @param   roadNodeLinkIDs  [in/out] ���H�m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 * @param   walkLink         [in/out] ���s�҃����N�Q�N���X
	 * @param   walkNode         [in/out] ���s�҃m�[�h�Q�N���X
	 * @param   walkNodeLinkIDs  [in/out] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 */
	void addMeshInfo(
		const long targetNodeId,
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& roadLink,
		FeaturesPack& roadNode,
		NODEIDMAP& roadNodeLinkIDs,
		FeaturesPack& walkLink,
		FeaturesPack& walkNode,
		NODEIDMAP& walkNodeLinkIDs);

	/**
	 * @brief	�������Ȃ����Ă��Ȃ��̔���
	 * @param	[in] isDirect     �@�@�@�@�@�@�ڑ������N�̌���
	 * @param	[in] linkWalkableCode	      �`�F�b�N�Ώۂ�WALKABLE_C�̒l
	 * @param   [in] connectedWalkableCode    �ڑ������N��WALKABLE_C�̒l
	 * @retval	true  �G���[
	 * @retval	false �G���[�ł͂Ȃ�
	 */
	bool isConnectSideWalkError(bool isDirect, long linkWalkableCode, long connectedWalkableCode);

	/**
	 * @brief	���s�\��ʃR�[�h���s���Ȓl���`�F�b�N
	 * @param	[in] walkableClass      ���s�\��ʃR�[�h
	 */
	void checkWalkableClass(
		long  walkableClass
	) const;

	/**
	 * @brief ���b�V�����E�ŕ��s�\��ʂ��ς���Ă��邩�`�F�b�N
	 * @param [in] connectFromLinkIds  �`�F�b�N�Ώۂ̓��H�����N�ɐڑ����Ă��铹�H�����N��ID
	 * @param [in] roadLink            ���H�����N�Q�N���X
	 * @param [in] nodeClass           �m�[�h��ʃR�[�h
	 * @param [in] walkableClass       ���s�\��ʃR�[�h
	 */
	void checkOverMeshWalkableClass(
		const std::set<long>& connectLinkIds,
		const FeaturesPack& roadLink,
		long nodeClass,
		long walkableClass
	) const;

protected:
	COutPut& m_output; //!< ���O�o�͗p�C���X�^���X�ւ̎Q��
	CComVariant m_modifyFilter; //!< �ҏW�����t�B���^
	const std::map<long, CString>& m_walkableMap; //!< ���s�\��ʂ̃R�[�h�l�h���C����
	sindy::schema::walk_type::type_class::ECode m_dataType;             //!< �f�[�^�^�C�v
};
