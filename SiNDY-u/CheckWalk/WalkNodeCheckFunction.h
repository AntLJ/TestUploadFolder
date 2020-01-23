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
#include "Properties.h"

class WalkNodeCheckFunction
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param out                [in] ���O�o�͊Ǘ��N���X�̎Q��
	 * @param modufyFilter       [in] �ҏW�����t�B���^
	 * @param walkLinkClassMap   [in] ���s�Ҏ�ʂ̃R�[�h�l�h���C���}�b�v
	 * @param walkNodeClassMap   [in] �m�[�h��ʂ̃R�[�h�l�h���C���}�b�v
	 * @param dist_threshold     [in] ��������臒l(m)
	 * @param neighbor_threshold [in] �ߖT����臒l(m)
	 * @param refer_id_threshold [in] �֘AID����臒l(m)
	 * @param dataType           [in] �f�[�^�^�C�v
	 * @param coordinateTolerance [in] ���W���e�l
	 */
	WalkNodeCheckFunction(COutPut& out
		, CComVariant modifyFilter
		, const std::map<long, CString>& walkLinkClassMap
		, const std::map<long, CString>& walkNodeClassMap
		, double dist_threshold
		, double neighbor_threshold
		, double refer_id_threshold
		, sindy::schema::walk_type::type_class::ECode dataType
		, double coordinateTolerance
	) :
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_walkLinkClassMap(walkLinkClassMap)
		, m_walkNodeClassMap(walkNodeClassMap)
		, m_dist_threshold(dist_threshold)
		, m_neighbor_threshold(neighbor_threshold)
		, m_refer_id_threshold(refer_id_threshold)
		, m_dataType(dataType)
		, m_coordinateTolerance(coordinateTolerance)
	{}

	/**
	 * @brief ���s�҃m�[�h�p�̑S�`�F�b�N
	 * @param meshcode        [in] �������̓s�s�n�}���b�V���R�[�h
	 * @param ipMeshGeo       [in] �s�s�n�}���b�V���`��
	 * @param linkMap         [in] ���H�����N�̑����E�`����i�[�����}�b�v
	 * @param nodeMap         [in] ���H�m�[�h�̑����E�`����i�[�����}�b�v
	 * @param walkLinkMap     [in] ���s�҃����N�̑����E�`����i�[�����}�b�v
	 * @param walkNodeMap     [in] ���s�҃m�[�h�̑����E�`����i�[�����}�b�v
	 * @param gateMap         [in] �w�o�����̑����E�`����i�[�����}�b�v
	 * @param poiPointMap     [in] �n���X�o�����̑����E�`����i�[�����}�b�v
	 * @param scrambleArea    [in] �X�N�����u���G���A�̑����E�`����i�[�����}�b�v
	 * @param heightNode      [in] �d�Ȃ胊���N�̍����m�[�h�̑����E�`����i�[�����}�b�v
	 * @param linkRTree       [in] ���H�����N��RTree
	 * @param nodeRTree       [in] ���H�m�[�h��RTree
	 * @param walkLinkRTree   [in] ���s�҃����N��RTree
	 * @param walkNodeRTree   [in] ���s�҃m�[�h��RTree
	 * @param scrambleAreaRTree   [in] �X�N�����u���G���A��RTree
	 * @param heightNodeRTree [in] �d�Ȃ胊���N�̍����m�[�h��RTree
	 * @param walkNodeLinkIDs [in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 * @param baseMeshes      [in] �x�[�X���b�V��<���b�V���R�[�h,�`��>
	 * @param bOther          [in] other���[�h�Ƃ��Ď��s���邩�ۂ�
	 * (ohter���[�h�Ŏ��s����ƁA����̃`�F�b�N�����s����Ȃ��Ȃ�B
	 * �ڍׂ͊J���������Q�Ƃ̂���)
	 */
	void CheckWalkNode(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const FeaturesPack& stationGate,
		const FeaturesPack& poiPoint,
		const FeaturesPack& scrambleArea,
		const FeaturesPack& heightNode,
		const BGRTree& linkRTree,
		const BGRTree& nodeRTree,
		const BGRTree& walkLinkRTree,
		const BGRTree& walkNodeRTree,
		const BGRTree& scrambleAreaRTree,
		const BGRTree& heightNodeRTree,
		const NODEIDMAP& walkNodeLinkIDs,
		const std::map<CString, IGeometryPtr>& baseMeshes,
		bool bOther
	);

	/**
	 * @brief walkable���[�h�p�̃`�F�b�N
	 * @param ipMeshGeo       [in] �s�s�n�}���b�V���`��
	 * @param roadLink        [in] ���H�����N�Q�N���X
	 * @param roadNode        [in] ���H�m�[�h�Q�N���X
	 * @param walkNode        [in] ���s�҃m�[�h�Q�N���X
	 * @param linkRTree       [in] ���H�����N��RTree
	 * @param nodeRTree       [in] ���H�m�[�h��RTree
	 * @param walkNodeLinkIDs [in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 */
	void checkWalkable(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const FeaturesPack& walkNode,
		const BGRTree& linkRTree,
		const BGRTree& nodeRTree,
		const NODEIDMAP& walkNodeLinkIDs
	);

	/**
	 * @brief release���[�h�p�̑S�`�F�b�N
	 * @param meshcode        [in] �������̓s�s�n�}���b�V���R�[�h
	 * @param ipMeshGeo       [in] �s�s�n�}���b�V���`��
	 * @param roadLink        [in] ���H�����N�Q�N���X
	 * @param roadNode        [in] ���H�m�[�h�Q�N���X
	 * @param walkLink        [in] ���s�҃����N�Q�N���X
	 * @param walkNode        [in] ���s�҃m�[�h�Q�N���X
	 * @param stationGate     [in] �w�o�����Q�N���X
	 * @param poiPoint        [in] �n���X�o�����Q�N���X
	 * @param heightNode      [in] �d�Ȃ胊���N�̍����m�[�h�̌Q�N���X
	 * @param linkRTree       [in] ���H�����N��RTree
	 * @param nodeRTree       [in] ���H�m�[�h��RTree
	 * @param walkLinkRTree   [in] ���s�҃����N��RTree
	 * @param walkNodeRTree   [in] ���s�҃m�[�h��RTree
	 * @param heightNodeRTree [in] �d�Ȃ胊���N�̍����m�[�h��RTree
	 * @param walkNodeLinkIDs [in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 * @param baseMeshes      [in] �x�[�X���b�V��<���b�V���R�[�h,�`��>
	 */
	void checkRelease(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadlink,
		const FeaturesPack& roadNode,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const FeaturesPack& stationGate,
		const FeaturesPack& poiPoint,
		const FeaturesPack& heightNode,
		const BGRTree& linkRTree,
		const BGRTree& nodeRTree,
		const BGRTree& walkLinkRTree,
		const BGRTree& walkNodeRTree,
		const BGRTree& heightNodeRTree,
		const NODEIDMAP& walkNodeLinkIDs,
		const std::map<CString, IGeometryPtr>& baseMeshes
	);

protected:
	enum CheckType {
		All,
		Other,
		Release
	};
	/**
	 * @brief �m�[�h�`�󂪕s���ł��邩�ۂ�
	 * @param targetNode [in] �`�F�b�N�Ώۂ̃m�[�h�`��
	 * @param walkNodeX [out] �m�[�h�̌o�x
	 * @param walkNodeY [out] �m�[�h�̈ܓx
	 * @retval true  �`�󂪕s���ł���
	 * @retval false �`�󂪕s���ł͂Ȃ�
	 */
	bool isIllegalShape(
		const IGeometryPtr& targetNode,
		double& walkNodeX,
		double& walkNodeY
	);

	/**
	 * @brief �m�[�h�`�󂪕s���ł��邩�`�F�b�N
	 * @note �G���[�R�[�h:102
	 * @param walkNodeObj [in] �`�F�b�N�Ώۂ̃m�[�h�I�u�W�F�N�g
	 * @param walkNodeX   [out] �m�[�h�̌o�x
	 * @param walkNodeY   [out] �m�[�h�̈ܓx
	 */
	void checkNodeGeometry(
		const GeoClass& walkNodeObj,
		double& walkNodeX,
		double& walkNodeY);

	/**
	 * @brief �m�[�h�����b�V���̎l���ɂȂ����`�F�b�N
	 * @note �G���[�R�[�h:103
	 * @param walkNodeX   [in] �`�F�b�N�Ώۂ̃m�[�h�̌o�x
	 * @param walkNodeY   [in] �`�F�b�N�Ώۂ̃m�[�h�̈ܓx
	 */
	void checkBorderCorner(
		double walkNodeX,
		double walkNodeY);

	/**
	 * @brief ���b�V�����E��ɑ��݂���m�[�h�̎�ʂ����������`�F�b�N
	 * @note �G���[�R�[�h:104,105
	 * @param walkNodeObj   [in] �`�F�b�N�Ώۂ̃m�[�h�I�u�W�F�N�g
	 , @param walkNodeClass [in] �`�F�b�N�Ώۂ̃m�[�h���
	 * @param basePolyline  [in] �x�[�X���b�V���̘g��
	 */
	void checkBorderLine(
		const GeoClass& walkNodeObj,
		long walkNodeClass,
		const IPolylinePtr& basePolyline);

	/**
	 * @brief ���H�m�[�h��ɂ�����s�҃����N�̎�ʂ����HNW�ڑ��m�[�h�ɂȂ��Ă��邩�`�F�b�N
	 * @note �G���[�R�[�h:107
	 * @param walkNodeObj   [in] �`�F�b�N�Ώۂ̃m�[�h�I�u�W�F�N�g
	 , @param walkNodeClass [in] �`�F�b�N�Ώۂ̃m�[�h���
	 * @param roadNode      [in] ���H�m�[�h�Q�N���X
	 * @param roadNodeRTree [in] ���H�m�[�h��RTree
	 */
	void checkRoadNWConnectNode(
		const GeoClass& walkNodeObj,
		long walkNodeClass,
		const FeaturesPack& roadNode,
		const BGRTree& roadNodeRTree);

	/**
	 * @brief ���s�҃m�[�h�����s�҃����N�ɐڑ����Ă��邩�`�F�b�N
	 * @note �G���[�R�[�h:106
	 * @param walkNodeObj   [in] �`�F�b�N�Ώۂ̃m�[�h�I�u�W�F�N�g
	 * @param walkLink      [in] ���s�҃����N�Q�N���X
	 * @param walkLinkRTree [in] ���s�҃����N��RTree
	 */
	void checkWalkLinkConnectNode(
		const GeoClass& walkNodeObj,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree);

	/**
	 * @brief �����N�Ƃ̐ڑ���ԂɊւ��`�F�b�N�S��
	 * @param walkNodeObj	  [in] �`�F�b�N�Ώۂ̃m�[�h�I�u�W�F�N�g
	 * @param walkNode        [in] ���s�҃m�[�h�Q�N���X
	 * @param walkLink        [in] ���s�҃����N�Q�N���X
	 * @param walkNodeClass   [in] �`�F�b�N�Ώۂ̃m�[�h���
	 * @param roadNodeID      [in] �`�F�b�N�Ώۂ̓��H�m�[�h�Ή�ID
	 * @param walkClassIndex  [in] WalkClass�̃t�B�[���h�C���f�b�N�X
	 * @param walkNodeClassIndex [in] ���s�҃m�[�h��ʃC���f�b�N�X
	 * @param fromNodeIdIndex [in] �n�_�m�[�hOID�C���f�b�N�X
	 * @param toNodeIdIndex	  [in] �I�_�m�[�hOID�C���f�b�N�X
	 * @param walkNodeLinkIDs [in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 * @param roadLinkRTree   [in] ���H�����N��RTree
	 * @param roadNodeRTree   [in] ���H�m�[�h��RTree
	 * @param checkType       [in] �`�F�b�N���[�h
	 * (���[�h�ɂ���āA����̃`�F�b�N�����s����Ȃ��Ȃ�B
	 */
	void checkConenctLink(
		const GeoClass& walkNodeObj,
		const FeaturesPack& walkNode,
		const FeaturesPack& walkLink,
		long walkNodeClass,
		long roadNodeID,
		long walkClassIndex,
		long walkNodeClassIndex,
		long fromNodeIdIndex,
		long toNodeIdIndex,
		const NODEIDMAP& walkNodeLinkIDs,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const BGRTree& roadLinkRTree,
		const BGRTree& roadNodeRTree,
		CheckType checkType);


	/**
	 * @brief �[�_�m�[�h�����̃����N�ɐڐG���Ă��Ȃ����`�F�b�N
	 * @note �G���[�R�[�h:111
	 * @param walkNodeObj	  [in] �`�F�b�N�Ώۂ̃m�[�h�I�u�W�F�N�g
	 * @param walkLink        [in] ���s�҃����N�Q�N���X
	 * @param walkLinkRTree   [in] ���s�҃����N��RTree
	 * @param walkNodeClass   [in] �`�F�b�N�Ώۂ̃m�[�h���
	 * @param walkNodeLinkIDs [in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 */
	void checkTouchOtherLink(
		const GeoClass& walkNodeObj,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree,
		long walkNodeClass,
		const NODEIDMAP& walkNodeLinkIDs);

	/**
	 * @brief �m�[�h�����̃m�[�h�ɏd�Ȃ邩�`�F�b�N
	 * @note �G���[�R�[�h:112
	 * @param walkNodeObj	  [in] �`�F�b�N�Ώۂ̃m�[�h�I�u�W�F�N�g
	 * @param walkNode        [in] ���s�҃m�[�h�Q�N���X
	 * @param walkNodeRTree   [in] ���s�҃m�[�h��RTree
	 * @param walkNodeX   [in] �`�F�b�N�Ώۂ̃m�[�h�̌o�x
	 * @param walkNodeY   [in] �`�F�b�N�Ώۂ̃m�[�h�̈ܓx
	 */
	void checkTouchOtherNode(
		const GeoClass& walkNodeObj,
		const FeaturesPack& walkNode,
		const BGRTree& walkNodeRTree,
		double walkNodeX,
		double walkNodeY);

	/**
	 * @brief ���s�҃m�[�h�ɏd�Ȃ铹�H�m�[�h������
	 * @param roadNodeObj   [in] �`�F�b�N�Ώۂ̓��H�m�[�h�̏��
	 * @param walkNodeObj   [in] �`�F�b�N�Ώۂ̕��s�҃m�[�h�̏��
	 * @param roadNodeMap   [in] ���H�m�[�h�̏��
	 * @param roadNodeRTree [in] ���H�m�[�h��RTree
	 * @retval true  ���s�҃m�[�h�ɏd�Ȃ铹�H�m�[�h�����݂���
	 * @retval false ���s�҃m�[�h�ɏd�Ȃ铹�H�m�[�h�͑��݂��Ȃ�
	 */
	bool findOverlapRoadNode(
		GeoClass& roadNodeObj
		, const GeoClass& walkNodeObj
		, const std::map<long, GeoClass>& roadNodeMap
		, const BGRTree& roadNodeRTree
		, double range = 0.1
	) const;

	/**
	 * @brief ���s�\��ʂɊւ��`�F�b�N
	 * @param roadNodeObj        [in] �`�F�b�N�Ώۂ̓��H�m�[�h�̏��
	 * @param connectWalkLinkCnt [in] ���s�҃m�[�h�ɐڑ����Ă��郊���N�̐�
	 * @param roadLink           [in] ���H�����N�Q�N���X
	 * @param roadLinkRTree      [in] ���H�m�[�h��RTree
	 */
	bool checkWalkableConnect(
		const GeoClass& roadNodeObj
		, long connectWalkLinkCnt
		, const sindy::CFieldMap& fieldMap
		, const FeaturesPack& roadLink
		, const BGRTree& roadLinkRTree
	);

	/**
	 * @brief ���s�҃m�[�h�̊֘A�|�C���g�`�F�b�N
	 * @param walkNode   [in] �`�F�b�N�Ώۂ̕��s�҃m�[�h
	 * @param fieldMap�@ [in] ���s�҃m�[�h�̃t�B�[���h�}�b�v
	 * @param checkField [in] �`�F�b�N�Ώۂ̃t�B�[���h��
	 * @param point      [in] �`�F�b�N�ΏیQ�N���X
	 * @param bRelase    [in] relase���[�h�p�̃`�F�b�N�����邩�ۂ�
	 */
	void CheckWalkNodeWithPoint(
		const GeoClass& walkNode,
		const sindy::CFieldMap& fieldMap,
		const CString& checkField,
		const FeaturesPack& point,
		bool bRelease
	);

	/**
	* @brief ���H�m�[�h�Ƃ̐������`�F�b�N
	* @note	 �G���[�R�[�h:113,116,117,120,121
	* @param walkNodeClass		[in] ���s�҃m�[�h����
	* @param roadNodeID�@		[in] ���H�m�[�hOID
	* @param nodeNameIndex		[in] ���H�m�[�h�̌����_���̃C���f�b�N�X
	* @param walkNodeRoadIndex	[in] ���s�҃m�[�h�̓��H�m�[�h�Ή�ID�C���f�b�N�X
	* @param walkNodeObj		[in] ���s�҃m�[�h�I�u�W�F�N�g
	* @param roadNode			[in] ���H�m�[�h�Q�N���X
	* @param walkNode			[in] ���s�҃����N�Q�N���X
	* @param walkNodeRTree		[in] ���s�҃m�[�h��RTree
	* @param bRelase			[in] relase���[�h�p�̃`�F�b�N�����邩�ۂ�
	*/
	void compairRoadNodeCheck(
		long walkNodeClass,
		long roadNodeID,
		long nodeNameIndex,
		long walkNodeRoadIndex,
		const GeoClass& walkNodeObj,
		const FeaturesPack& roadNode,
		const FeaturesPack& walkNode,
		const BGRTree& walkNodeRTree,
		bool isForRelease
	);

	/**
	* @brief ���D���ɉ��D�O�����N���Ȃ����`�F�b�N
	* @note	 �G���[�R�[�h:132,133,134
	* @param walkLink			[in] ���s�҃����N�Q�N���X
	* @param walkNode			[in] ���s�҃m�[�h�Q�N���X
	* @param walkClassIndex		[in] ���s�ґ����̃C���f�b�N�X
	* @param walkNodeClassIndex	[in] ���s�҃m�[�h�����̃C���f�b�N�X
	* @param fromNodeIdIndex	[in] �n�_�m�[�hOID�C���f�b�N�X
	* @param toNodeIdIndex		[in] �I�_�m�[�hOID�C���f�b�N�X
	* @param links			    [in] �Ώۃm�[�h�ɐڑ����Ă��郊���N���X�g
	* @param walkNodeLinkIDs	[in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	*/
	void internalTicketGateConenctCheck(
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		long walkClassIndex,
		long walkNodeClassIndex,
		long fromNodeIdIndex,
		long toNodeIdIndex,
		const std::set<long>& links,
		const NODEIDMAP& walkNodeLinkIDs
	);

	/**
	* @brief �����N�����H�m�[�h�ɐڑ����Ă��邩�i�ċA�j
	* @param linkID			    [in] �`�F�b�N�Ώۂ̕��s�҃����N��OID
	* @param walkClassIndex		[in] ���s�ґ����̃C���f�b�N�X
	* @param walkNodeClassIndex	[in] ���s�҃m�[�h�����̃C���f�b�N�X
	* @param fromNodeIdIndex	[in] �n�_�m�[�hOID�C���f�b�N�X
	* @param toNodeIdIndex		[in] �I�_�m�[�hOID�C���f�b�N�X
	* @param walkLink			[in] ���s�҃����N�Q�N���X
	* @param walkNode			[in] ���s�҃m�[�h�Q�N���X
	* @param walkNodeLinkIDs	[in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	* @param inspectedIDs		[in] �����ςݕ��s�҃����N��OID���X�g
	* @return	true	���H�m�[�h�ɐڑ����Ă���
	* @return	false	���H�m�[�h�ɐڑ����Ă��Ȃ�
	*/
	bool isConnectedRoadNode(
		long linkID,
		long walkClassIndex,
		long walkNodeClassIndex,
		long fromNodeIdIndex,
		long toNodeIdIndex,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs,
		std::set<long>& inspectedIDs
	);

	/**
	* @brief �m�[�h�ɐڑ����Ă��郊���N�����H�m�[�h�ɐڑ����Ă��邩�i�ċA�j
	* @param walkNodeObj		[in] �`�F�b�N�Ώۂ̕��s�҃m�[�h�I�u�W�F�N�g
	* @param walkClassIndex		[in] ���s�ґ����̃C���f�b�N�X
	* @param walkNodeClassIndex	[in] ���s�҃m�[�h�����̃C���f�b�N�X
	* @param fromNodeIdIndex	[in] �n�_�m�[�hOID�C���f�b�N�X
	* @param toNodeIdIndex		[in] �I�_�m�[�hOID�C���f�b�N�X
	* @param walkLink			[in] ���s�҃����N�Q�N���X
	* @param walkNode			[in] ���s�҃m�[�h�Q�N���X
	* @param walkNodeLinkIDs	[in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	* @param inspectedIDs		[in] �����ςݕ��s�҃����N��OID���X�g
	* @return	true	���H�m�[�h�ɐڑ����Ă���
	* @return	false	���H�m�[�h�ɐڑ����Ă��Ȃ�
	*/
	bool isConnectedRoadNodeAtNode(
		const GeoClass& walkNodeObj,
		long walkClassIndex,
		long walkNodeClassIndex,
		long fromNodeIdIndex,
		long toNodeIdIndex,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs,
		std::set<long>& inspectedIDs
	);

	/**
	 * @brief ���H�����N��ɂ���̂ɓ��H�����N�ڑ��m�[�h�ɂȂ��Ă��邩�`�F�b�N
	 * @note �G���[�R�[�h:135
	 * @param [in] walkNodeObject �`�F�b�N�Ώۂ̃m�[�h�I�u�W�F�N�g
	 , @param [in] walkNodeClass  �`�F�b�N�Ώۂ̃m�[�h���
	 * @param [in] roadLink       ���H�����N�Q�N���X
	 * @param [in] roadLinkRTree  ���H�����N��RTree
	 */
	void checkRoadLinkConnectNode(
		const GeoClass& walkNodeObject,
		long walkNodeClass,
		const FeaturesPack& roadLink,
		const BGRTree& roadLinkRTree) const;

	/**
	 * @brief ���H�����N�ڑ��m�[�h�����H�����N�����ӏ��ɂ��邩�`�F�b�N
	 * @note �G���[�R�[�h:137
	 * @param [in] walkNodeObject �`�F�b�N�Ώۂ̕��s�҃m�[�h�I�u�W�F�N�g
	 , @param [in] walkNodeClass  �`�F�b�N�Ώۂ̕��s�҃m�[�h���
	 * @param [in] roadLink       ���H�����N�Q�N���X
	 * @param [in] roadLinkRTree  ���H�����N��RTree
	 * @param [in] roadNodeMap    ���H�m�[�h�̏��
	 * @param [in] roadNodeRTree  ���H�m�[�h��RTree
	 */
	void checkRoadLinkCrossNode(
		const GeoClass& walkNodeObject,
		long walkNodeClass,
		const FeaturesPack& roadLink,
		const BGRTree& roadLinkRTree,
		const std::map<long, GeoClass>& roadNodeMap,
		const BGRTree& roadNodeRTree) const;

	/**
	 * @brief �X�N�����u�������_�Ȃ̂ɃX�N�����u���G���A�|���S�����������`�F�b�N
	 * @param [in] walkNodeObject     �`�F�b�N�Ώۂ̕��s�҃m�[�h
	 * @param [in] walkLink           ���s�҃����N�Q�N���X
	 * @param [in] walkLinkRTree      ���s�҃����NRTree
	 * @param [in] scrambleArea       �X�N�����u���G���A�Q�N���X
	 * @param [in] scrambleAreaRTree  �X�N�����u���G���ARTree
	 */
	void checkWalkNodeCrossWalkLink(
		const GeoClass& walkNodeObject,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree,
		const FeaturesPack& scrambleArea,
		const BGRTree& scrambleAreaRTree
	) const;

	/**
	 * @brief �|�C���g���m�iWALK_NODE�AHEIGHT_NODE�j�̋������߂����`�F�b�N
	 * @param [in] walkNodeObject     �`�F�b�N�Ώۂ� ���f�|�C���g
	 * @param [in] walkNode           ���s�҃m�[�h�Q�N���X
	 * @param [in] heightNode         �d�Ȃ胊���N�̍����m�[�h�Q�N���X
	 * @param [in] walkNodeRTree      ���s�҃m�[�hRTree
	 * @param [in] heightNodeRTree    �d�Ȃ胊���N�̍����m�[�h��RTree
	 */
	void checkNearPoint(
		const GeoClass& walkNodeObject,
		const FeaturesPack& walkNode,
		const FeaturesPack& heightNode,
		const BGRTree& walkNodeRTree,
		const BGRTree& heightNodeRTree
	) const;

	/**
	 * @brief ���W�J���R�l�N�V�����������ڑ����Ă��邩�`�F�b�N
	 * @note �G���[�R�[�h:139
	 * @param [in] walkNodeObject �`�F�b�N�Ώۂ̃m�[�h�I�u�W�F�N�g
	 * @param [in] walkLink       ���s�҃����N�Q�N���X
	 * @param [in] walkLinkRTree  ���s�҃����N��RTree
	 */
	void checkConnectWalkLink(
		const GeoClass& walkNodeObject,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree) const;

	/**
	* @brie ���W�J���R�l�N�V�������s�p�ɐڑ����Ă��邩�`�F�b�N
	* @note �G���[�R�[�h:140
	* @param [in] walkNodeObject �`�F�b�N�Ώۂ̃m�[�h�I�u�W�F�N�g
	* @param [in] walkLink       ���s�҃����N�Q�N���X
	* @param [in] walkLinkRTree  ���s�҃����N��RTree
	*/
	void checkConnectWalkLinkAngle(
		const GeoClass& walkNodeObject,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree)const;

	/**
	 * @brief ���S�_�̗אړ_�̍��W���擾
	 * @param [in] ipGeometry           ���S�_�̌`��
	 * @param [in] ipGeometryCompare    �אړ_�̌`��
	 * @param [in] x                    X�̍��W
	 * @param [in] y                    Y�̍��W
	 */
	void getCoordinate(
		const IGeometryPtr& ipGeometry,
		const IGeometryPtr& ipGeometryCompare,
		double& x,
		double& y) const;

protected:
	COutPut& m_output; //!< ���O�o�͗p�C���X�^���X�ւ̎Q��
	CComVariant m_modifyFilter; //!< �ҏW�����t�B���^
	const std::map<long, CString>& m_walkLinkClassMap; //!< ���s�Ҏ�ʂ̃R�[�h�l�h���C���}�b�v
	const std::map<long, CString>& m_walkNodeClassMap; //!< �m�[�h��ʂ̃R�[�h�l�h���C���}�b�v
	double m_dist_threshold;     //!< ��������臒l(m)
	double m_neighbor_threshold; //!< �ߖT����臒l(m)
	double m_refer_id_threshold; //!< �֘AID����臒l(m)
	sindy::schema::walk_type::type_class::ECode m_dataType;             //!< �f�[�^�^�C�v
	double m_coordinateTolerance;//!< ���W���e�l
};
