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
class WalkLinkCheckFunction
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param out               [in] ���O�o�͊Ǘ��N���X�̎Q��
	 * @param modufyFilter      [in] �ҏW�����t�B���^
	 * @param walkLinkClassMap  [in] ���s�Ҏ�ʂ̃R�[�h�l�h���C���}�b�v
	 * @param walkCode          [in] ���s�ҘH���R�[�h�̏��
	 * @param walkNoPassage     [in] ���s�Ғʍs�֎~�K���e�[�u���̏��
	 * @param walkOnewayMap     [in] ���s�҈���ʍs�K���e�[�u���̏��
	 * @param dist_threshold    [in] ��������臒l(m)
	 * @param angle_threshold   [in] �p�x臒l(�x)
	 * @param coordinateTolerance [in] ���W���e�l
	 * @param dataType          [in] �`�F�b�N�Ώۂ̃f�[�^�^�C�v
	 * @param microLogicalConnection  [in]  �������W�J���R�l�N�V����臒l(m)
	 */
	WalkLinkCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, const std::map<long, CString>& walkLinkClassMap
		, const RowsPack& walkCode
		, const MultiRowsPack& walkNoPassage
		, const MultiRowsPack& walkOneway
		, double dist_threshold
		, double angle_threshold
		, double coordinateTolerance
		, sindy::schema::walk_type::type_class::ECode dataType
		, double microLogicalConnection) :
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_walkLinkClassMap(walkLinkClassMap)
		, m_walkCode(walkCode)
		, m_walkNoPassage(walkNoPassage)
		, m_walkOneway(walkOneway)
		, m_dist_threshold(dist_threshold)
		, m_angle_threshold(angle_threshold)
		, m_coordinateTolerance(coordinateTolerance)
		, m_dataType(dataType)
		, m_microLogicalConnection(microLogicalConnection)
	{}

	/**
	 * @brief ���s�҃����N�̑S�Ẵ`�F�b�N�����s
	 * @param meshcode            [in] �������̓s�s�n�}���b�V���R�[�h
	 * @param ipMeshGeo           [in] �s�s�n�}���b�V���`��
	 * @param walkLink            [in] ���s�҃����N�Q�N���X
	 * @param walkNode            [in] ���s�҃m�[�h�Q�N���X
	 * @param roadLink            [in] ���H�����N�Q�N���X
	 * @param roadNode            [in] ���H�m�[�h�Q�N���X
	 * @param walkLinkRTree       [in] ���s�҃����NRTree
	 * @param roadLinkRTree       [in] ���H�����NRTree
	 * @param roadNodeRTree       [in] ���H�m�[�hRTree
	 * @param walkNodeLinkIDs     [in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 * @param bOther              [in] other���[�h�Ƃ��Ď��s���邩�ۂ�
	 * @param scrambleArea        [in] �X�N�����u���G���A�Q�N���X
	 * @param scrambleAreaRTree   [in] �X�N�����u���G���ARTree
	 * (ohter���[�h�Ŏ��s����ƁA����̃`�F�b�N�����s����Ȃ��Ȃ�B
	 * �ڍׂ͊J���������Q�Ƃ̂���)
	 */
	void CheckWalkLink(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const BGRTree& walkLinkRTree,
		const BGRTree& roadLinkRTree,
		const BGRTree& roadNodeRTree,
		const NODEIDMAP& walkNodeLinkIDs,
		bool bOther,
		const FeaturesPack& scrambleArea,
		const BGRTree& scrambleAreaRTree
	);

	/**
	 * @brief humancar���[�h�p�̃`�F�b�N�����s
	 * @detail �G���[�R�[�h 351�̂ݎ��s����
	 * @param meshcode    [in] �������̓s�s�n�}���b�V���R�[�h
	 * @param ipMeshGeo   [in] �s�s�n�}���b�V���`��
	 * @param walkLink    [in] ���s�҃����N�Q�N���X
	 * @param roadLink    [in] ���H�����N�Q�N���X
	 * @param roadLinkRTree [in] ���H�����NRTree
	 */
	void checkHumanCar(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& walkLink,
		const FeaturesPack& roadLink,
		const BGRTree& roadLinkRTree
	);

	// TODO: CheckWalkLink�Ƌ@�\���܂Ƃ߂���Ƃ���͂܂Ƃ߂�
	/**
	 * @brief release���[�h�p�̃`�F�b�N�����s
	 * @param meshcode        [in] �������̓s�s�n�}���b�V���R�[�h
	 * @param ipMeshGeo       [in] �s�s�n�}���b�V���`��
	 * @param walkLink        [in] ���s�҃����N�Q�N���X
	 * @param walkNode        [in] ���s�҃m�[�h�Q�N���X
	 * @param roadLink        [in] ���H�����N�Q�N���X
	 * @param roadNode        [in] ���H�m�[�h�Q�N���X
	 * @param walkLinkRTree   [in] ���s�҃����NRTree
	 * @param roadLinkRTree   [in] ���H�����NRTree
	 * @param roadNodeRTree   [in] ���H�m�[�hRTree
	 * @param walkNodeLinkIDs [in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 */
	void checkRelease(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const BGRTree& walkLinkRTree,
		const BGRTree& roadLinkRTree,
		const BGRTree& roadNodeRTree,
		const NODEIDMAP& walkNodeLinkIDs
	);

protected:
	/**
	 * @breif ���s�҃����N�����H�����N��ɂ��邩����
	 * @param findRoadLinkObj    [out] ���s�҃����N��ɂ��铹�H�����N���o��
	 * @param walkLinkObj        [in]  �`�F�b�N�Ώۂ̕��s�҃����N�̑����E�`��
	 * @param ipWalkLinkPointCol [in]  �`�F�b�N�Ώۂ̕��s�҃����N�̍\���_
	 * @param walkLinkPointCnt   [in]  �`�F�b�N�Ώۂ̕��s�҃����N�̍\���_��
	 * @param walkLinkClass      [in]  �`�F�b�N�Ώۂ̕��s�҃����N�̕��s�Ҏ��
	 * @param roadLinkSet        [in]  �`�F�b�N�Ώۂ̕��s�҃����N��Envelope�ɏd�Ȃ铹�H�����NID�Q
	 * @param roadLinkMap        [in]  ���H�����N�̑����E�`����i�[�����}�b�v
	 * @retval true  ���s�҂͓��H�����N��ɂ���
	 * @retval false ���s�҂͓��H�����N��ɂȂ�
	 */
	bool isWalkLinkOnRoadLink(
		GeoClass& findRoadLinkObj,
		const GeoClass& walkLinkObj,
		const IPointCollectionPtr& ipWalkLinkPointCol,
		long walkLinkPointCnt,
		long walkLinkClass,
		const std::set<long>& roadLinkSet,
		const std::map<long, GeoClass>& roadLinkMap
	);

	/**
	 * @brief ���s�҃����N�����s�҃����N�Ɋ܂܂��ꍇ��,�����l���܊֌W�����������`�F�b�N
	 * @param walkLinkObj        [in] �`�F�b�N�Ώۂ̕��s�҃����N�̑����E�`��
	 * @param ipWalkLinkPointCol [in] �`�F�b�N�Ώۂ̕��s�҃����N�̍\���_
	 * @param walkLinkPointCnt   [in] �`�F�b�N�Ώۂ̕��s�҃����N�̍\���_��
	 * @param walkLinkClass      [in] �`�F�b�N�Ώۂ̕��s�҃����N�̕��s�Ҏ��
	 * @param walkLink           [in] ���s�҃����N�Q�N���X
	 * @param roadLink           [in] ���H�����N�Q�N���X
	 * @param roadNode           [in] ���H�m�[�h�Q�N���X
	 * @param walkLinkRTree      [in] ���s�҃����NRTree
	 * @param roadLinkRTree      [in] ���H�����NRTree
	 * @param roadNodeRTree      [in] ���H�m�[�hRTree
	 * @param meshcode           [in] �������̓s�s�n�}���b�V���R�[�h
	 * @param bOther             [in] other���[�h�Ƃ��Ď��s���邩�ۂ�
	 */
	void checkMatching(
		const GeoClass& walkLinkObj,
		const IPointCollectionPtr& ipWalkLinkPointCol,
		long walkLinkPointCnt,
		long walkLinkClass,
		const FeaturesPack& walkLink,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const BGRTree& walkLinkRTree,
		const BGRTree& roadLinkRTree,
		const BGRTree& roadNodeRTree,
		const CString& meshcode,
		bool bOther
	);

	/**
	 * @brief	�����ӏ��̃|�C���g���擾����B
	 * @param	baseGeoClass	[in]	������N��GeoClass
	 * @param	linkIDs	[in]	�����`�F�b�N�Ώۃ����NID
	 * @param	link   	[in]	�`�F�b�N�ΏیQ�N���X
	 * @param	crossPoints	[out]	�����ӏ��̃��X�g
	 * @param	isWalkLink	[in]	�`�F�b�N�Ώۂ����s�҃����N���ۂ�
	 */
	void GetCrossPoint(
		const GeoClass& baseGeoClass,
		const std::set<long>& linkIDs,
		const FeaturesPack& link,
		std::vector<GeoClass>& crossPoints,
		bool isWalkLink /* = false*/
	);

	/**
	 * @brief	�w�肳�ꂽ�����N�Q���ɍL��t���OON�̃����N�����݂��邩�ǂ���
	 * @param	walkLinkList	[in]	�`�F�b�N�Ώۂ̃����N�Q
	 * @param	fieldMap        [in]	�`�F�b�N�Ώۂ̃t�B�[���h�}�b�v
	 * @return	true	���Ȃ��Ƃ�1�{�͑��݂���
	 * @return	false	���݂��Ȃ�
	 */
	bool ExistOpenAreaFlag(
		const std::list<GeoClass>& walkLinkList
		, const sindy::CFieldMap& fieldMap
	);

	/**
	 * @brief	�w�肳�ꂽ�����N�Q���ɐl�ԋ��p�����N�����݂��邩�ǂ���
	 * @param	walkLinkList	[in]	�`�F�b�N�Ώۂ̃����N�Q
	 * @param	fieldMap        [in]	�`�F�b�N�Ώۂ̃t�B�[���h�}�b�v
	 * @return	true	���Ȃ��Ƃ�1�{�͑��݂���
	 * @return	false	���݂��Ȃ�
	 */
	bool ExistHumanCarLink(
		const std::list<GeoClass>& walkLinkList
		, const sindy::CFieldMap& fieldMap
	);


	/**
	 * @brief	�����Ώۂ̃����N�Q���ŁA������N�A�m�[�h�ɐڑ����郊���N�Q���擾����B
	 * @param	baseLinkID	[in]	������NID
	 * @param	nodeID	[in]	��m�[�hID
	 * @param	searchAreaLinkSet	[in]	�����Ώۂ̃����N�Q
	 * @param	nodeLinkIdMap	[in]	���s�҃m�[�h�Ƃ��̃m�[�h�ɕR�Â����s�҃����N�Q��Map
	 * @param	walkLink   	[in]	���s�҃����N�Q�N���X
	 * @param	findLinkMap	[out]	�݂����������N�Q
	 */
	void SearchConnectLinks(
		long baseLinkID,
		long nodeID,
		const std::set<long>& searchAreaLinkSet,
		const NODEIDMAP& nodeLinkIdMap,
		const FeaturesPack& walkLink,
		std::map<long, long>& findLinkMap
	);

	/**
	 * @brief	�����Ώۂ̃����N�Q���ŁA������N�A�m�[�h����3�����N�ڂɓ��B���郊���N�Q���擾����B
	 * @param	baseLinkID	[in]	������NID
	 * @param	nodeID	[in]	��m�[�hID
	 * @param	searchAreaLinkSet	[in]	�����Ώۂ̃����N�Q
	 * @param	nodeLinkIdMap	[in]	���s�҃m�[�h�Ƃ��̃m�[�h�ɕR�Â����s�҃����N�Q��Map
	 * @param	walkLinkMap	[in]	���s�҃����N��MAP
	 * @param	findLinkMap	[out]	�݂����������N�Q
	 */
	void SearchThirdConnectLinks(
		long baseLinkID,
		long nodeID,
		const std::set<long>& searchAreaLinkSet,
		const NODEIDMAP& nodeLinkIdMap,
		const FeaturesPack& walkLink,
		std::map<long, long>& findLinkMap
	);

	/**
	 * @brief �n���ʘH�����N�֐ڑ�������s�҃����N�̊K�w�ړ���ʂ���������ł��邩�m�F���A
	 * ��肪����Ȃ�G���[���O���o�͂���
	 * @detail �n���ʘH�����N�͖{�G���[�`�F�b�N�ΏۂɂȂ�Ȃ��B<br>
	 * ���L�̂����ꂩ�̏ꍇ����������Ɣ��肳���B<br>
	 * �E�n���ʘH�����N-���s�҃����N�ڑ��_�̃m�[�h���G���x�[�^�m�[�h�ł���<br>
	 * �E�ڑ����Ă���n���ʘH�����N�̊K�w�ړ���ʂ��������ł���
	 * @param underLinkID    [in] �ڑ������n���ʘH�����N��ID
	 * @param connectLinkMap [in] �ڑ����郊���N�Q
	 * @param walkNodeObj    [in] �n���ʘH�����N-���s�҃����N�̐ڑ��_�ɂ���m�[�h�̏����i�[
	 * @param fieldMap       [in] ���s�҃m�[�h�̃t�B�[���h�}�b�v
	 * @param walkLink       [in] ���s�҃����N�̌Q�N���X
	 * @return �G���[�ƂȂ���s�҃����NID�̃��X�g
	 */
	std::set<long> checkFloorMoveConnectUnder(
		long underLinkID
		, const std::list<GeoClass>& connectLinkMap
		, const GeoClass& walkNodeObj
		, const sindy::CFieldMap& fieldMap
		, const FeaturesPack& walkLinkMap);

	/**
	 * @brief ���s�҃����N�̍\���_�����̕��s�҃����N�̍\���_��ɂ���ꍇ�ɃG���[���O���o�͂���
	 * @param checkedLinkId [in] �`�F�b�N�Ώۂ̕��s�҃����N��ID
	 * @param walkLink      [in] ���s�҃����N�Q�N���X
	 * @param meshcode      [in] �������̓s�s�n�}���b�V���R�[�h
	 */
	void checkPointOnOtherLink(
		const std::vector<long>& checkedLinkId
		, const FeaturesPack& walkLink
		, const CString& meshcode);

	/**
	 * @brief	���b�V���O�̐ڑ������N���擾
	 * @detail �`�F�b�N��͈͂�9�s�s�n�}���b�V���O�ւ̒T���ɔ����A<br>
	 * �`�F�b�N�͈͂͊g������A�Ή��̃����N���m�[�h�����X�V�����B<br>
	 * @param	nodeId	     [in/out] �Ώە��s�҃m�[�hID�i�T���J�n���͋N�_�m�[�h�A�I�����͏I�_�m�[�h�j
	 * @param   adjacentLink [in] �T���̋N�_�m�[�h�ɐڑ����郊���N
	 * @param	ipMeshGeo	 [in/out] �`�F�b�N�͈�
	 * @param   walkLink     [in/out] ���s�҃����N�Q�N���X
	 * @param   walkNode     [in/out] ���s�҃m�[�h�Q�N���X
	 * @param   nodeLinkIDs  [in/out] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 * @return	�擾�������R�[�h
	 * @retval	nullptr	�擾�Ɏ��s
	 */
	std::set<long> getAdjacentWalkLink(
		long& nodeId
		, const long linkId
		, const IGeometryPtr& ipMeshGeo
		, FeaturesPack& walkLink
		, FeaturesPack& walkNode
		, NODEIDMAP& nodeLinkIDs
	);

	/**
	 * @brief	�Ώۃm�[�h�̏��݂��郁�b�V���̏���ǉ�����
	 * @param	targetNodeId     [in] �Ώە��s�҃m�[�hID
	 * @param	ipMeshGeo	     [in] �`�F�b�N�͈�
	 * @param   walkLink         [in/out] ���s�҃����N�Q�N���X
	 * @param   walkNode         [in/out] ���s�ҌQ�N���X
	 * @param   walkNodeLinkIDs  [in/out] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	 */
	void addMeshInfo(
		const long targetNodeId,
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& walkLink,
		FeaturesPack& walkNode,
		NODEIDMAP& walkNodeLinkIDs);

	/**
	* @brief	�����`�F�b�N
	* @node		�G���[�R�[�h 301�`305,309,339
	* @param	walkLinkClass		[in] ���s�Ҏ��
	* @param	floorMoveClass		[in] �K�w�ړ����
	* @param	onewayCode			[in] ����ʍs��ʃR�[�h
	* @param	noPassCode			[in] �ʍs�֎~��ʃR�[�h
	* @param	roadSurfaceCode		[in] �H�ʑf�ރR�[�h
	* @param	walkCode			[in] ���s�ҘH���R�[�h
	* @param	isForRelease		[in] �����[�X�����`�F�b�N���H
	*/
	void attributesCheck(
		long walkLinkClass,
		long floorMoveClass,
		long onewayCode,
		long noPassCode,
		long roadSurfaceCode,
		long walkCode,
		bool isForRelease
	);

	/**
	* @brief	�t���O�����`�F�b�N
	* @node		�G���[�R�[�h 310
	* @param	autoWalkF		[in] �I�[�g�E�H�[�N�t���O
	* @param	stairF			[in] �K�i�t���O
	* @param	slopeF			[in] �X���[�v�t���O
	* @param	escalatorF		[in] �G�X�J���[�^�t���O
	* @param	arcadeF			[in] �����t���t���O
	* @param	openAreaF		[in] �L��t���O
	* @param	tunnelF			[in] �g���l���t���O
	* @param	scrambleF		[in] �X�N�����u���t���O
	* @param	onewayRF		[in] ����ʍs���ԋK���֘A�t���O
	* @param	noPassRF		[in] �ʍs�֎~���ԋK���֘A�t���O
	*/
	void flagsCheck(
		long autoWalkF,
		long stairF,
		long slopeF,
		long escalatorF,
		long arcadeF,
		long openAreaF,
		long tunnelF,
		long scrambleF,
		long onewayRF,
		long noPassRF
	);

	/**
	* @brief	From/To��ID�`�F�b�N
	* @node		�G���[�R�[�h 311,312,326
	* @param	walkLinkID		[in] ���s�҃����NOID
	* @param	fromWalkNodeID	[in] �n�_�m�[�h��OID
	* @param	toWalkNodeID	[in] �I�_�m�[�h��OID
	* @param	dupNodeMap		[out] �d���`�F�b�N�p�}�b�v
	*/
	void fromToIdCheck(
		long walkLinkID,
		long fromWalkNodeID,
		long toWalkNodeID,
		std::map<std::pair<long, long>, std::list<long>>& dupNodeMap
	);

	/**
	* @brief	�`����g�����`�F�b�N�P
	* @node		�G���[�R�[�h 313,316,317,326,350
	* @param	walkLinkObj			[in] ���s�҃����N�I�u�W�F�N�g
	* @param	walkNode			[in] ���s�҃m�[�h�Q�N���X
	* @param	walkLinkClass		[in] ���s�Ҏ��
	* @param	fromWalkNodeID		[in] �n�_�m�[�h��OID
	* @param	toWalkNodeID		[in] �I�_�m�[�h��OID
	* @param	walkNodeClassIndex	[in] ���s�҃m�[�h��ʂ̃C���f�b�N�X
	* @param	targetWalkLinkId	[out] �G���[�R�[�h360�̃`�F�b�N�pID���X�g
	* @param	fromWalkNodeObj		[out] �n�_�m�[�h�I�u�W�F�N�g
	* @param	toWalkNodeObj		[out] �I�_�m�[�h�I�u�W�F�N�g
	*/
	bool WalkLinkCheckFunction::shapeCheck(
		const GeoClass& walkLinkObj,
		const FeaturesPack& walkNode,
		long walkLinkClass,
		long fromWalkNodeID,
		long toWalkNodeID,
		long walkNodeClassIndex,
		std::vector<long>& targetWalkLinkId,
		GeoClass& fromWalkNodeObj,
		GeoClass& toWalkNodeObj
	);

	/**
	* @brief	�`����g�����`�F�b�N�Q
	* @node		�G���[�R�[�h 321�`325
	* @param	walkLinkObj			[in] ���s�҃����N�I�u�W�F�N�g
	* @param	fromNodeObj			[in] �n�_�m�[�h�I�u�W�F�N�g
	* @param	toNodeObj			[in] �I�_�m�[�h�I�u�W�F�N�g
	* @param	isForRelease		[in] �����[�X�����`�F�b�N���H
	*/
	void shapeCheck2(
		const GeoClass& walkLinkObj,
		const GeoClass& fromNodeObj,
		const GeoClass& toNodeObj,
		bool isForRelease
	);

	/**
	* @brief	�݂Ȃ����f�����`�F�b�N
	* @node		�G���[�R�[�h 402
	* @param	walkLinkObj			[in] ���s�҃����N�I�u�W�F�N�g
	* @param	walkLinkClass		[in] ���s�Ҏ��
	* @param	roadLink			[in] ���H�����N�Q�N���X
	* @param	roadLinkRTree		[in] ���H�����N��RTree
	*/
	void inferredCrossRoadCheck(
		const GeoClass& walkLinkObj,
		long walkLinkClass,
		const FeaturesPack& roadLink,
		const BGRTree& roadLinkRTree
	);

	/**
	* @brief	�����Ԃ̐����`�F�b�N�P
	* @node		�G���[�R�[�h 329,330,331,334,338,346,357,401
	* @param	floorMoveClass	[in] �K�w�ړ����
	* @param	stairF			[in] �K�i�t���O
	* @param	slopeF			[in] �X���[�v�t���O
	* @param	escalatorF		[in] �G�X�J���[�^�t���O
	* @param	onewayCode		[in] ����ʍs��ʃR�[�h
	* @param	autoWalkF		[in] �I�[�g�E�H�[�N�t���O
	* @param	arcadeF			[in] �����t���t���O
	* @param	openAreaF		[in] �L��t���O
	* @param	tunnelF			[in] �g���l���t���O
	* @param	scrambleF		[in] �X�N�����u���t���O
	* @param	walkLinkClass	[in] ���s�Ҏ��
	* @param	walkCode		[in] ���s�ҘH���R�[�h
	*/
	void attributeConformityCheck(
		long floorMoveClass,
		long stairF,
		long slopeF,
		long escalatorF,
		long onewayCode,
		long autoWalkF,
		long arcadeF,
		long openAreaF,
		long tunnelF,
		long scrambleF,
		long walkLinkClass,
		long walkCode
	);

	/**
	* @brief	�����Ԃ̐����`�F�b�N�Q
	* @node		�G���[�R�[�h 342,343,347,348,349
	* @param	onewayCode		[in] ����ʍs��ʃR�[�h
	* @param	noPassCode		[in] �ʍs�֎~��ʃR�[�h
	* @param	noPassRF		[in] �ʍs�֎~���ԋK���֘A�t���O
	* @param	onewayRF		[in] ����ʍs���ԋK���֘A�t���O
	* @param	walkLinkClass	[in] ���s�Ҏ��
	* @param	walkLinkID		[in] ���s�҃����NOID
	*/
	void attributeConformityCheck2(
		long onewayCode,
		long noPassCode,
		long noPassRF,
		long onewayRF,
		long walkLinkClass,
		long walkLinkID
	);

	/**
	* @brief	�R�[�h�̕ω��`�F�b�N
	* @node		�G���[�R�[�h 332,354
	* @param	fromLinks		[in] �n�_�����s�҃����N���X�g
	* @param	toLinks			[in] �I�_�����s�҃����N���X�g
	* @param	walkLinkObj		[in] ����s�҃����N�I�u�W�F�N�g
	* @param	walkLink		[in] ���s�҃����N�Q�N���X
	* @param	walkLinkClass	[in] ���s�Ҏ��
	* @param	floorMoveClass	[in] �K�w�ړ����
	* @param	floorLevelIndex	[in] �K�w���x���C���f�b�N�X
	*/
	void attributeChangeCheck(
		const std::list<GeoClass>& fromLinks,
		const std::list<GeoClass>& toLinks,
		const GeoClass& walkLinkObj,
		const FeaturesPack& walkLink,
		long walkLinkClass,
		long floorMoveClass,
		long floorLevelIndex
	);

	/**
	* @brief	�L��`�F�b�N
	* @node		�G���[�R�[�h 336
	* @param	openAreaF		[in] �n�_�����s�҃����N���X�g
	* @param	fromLinks		[in] �n�_�����s�҃����N���X�g
	* @param	toLinks			[in] �I�_�����s�҃����N���X�g
	* @param	walkLink		[in] ���s�҃����N�Q�N���X
	*/
	void openAreaCheck(
		long openAreaF,
		const std::list<GeoClass> & fromLinks,
		const std::list<GeoClass>& toLinks,
		const FeaturesPack& walkLink
	);

	/**
	* @brief	���s�҃����N�ڑ��`�F�b�N
	* @node		�G���[�R�[�h 337
	* @param	walkLinkClass		[in] ���s�Ҏ��
	* @param	fromWalkNodeObj		[in] �n�_���s�҃m�[�h�I�u�W�F�N�g
	* @param	toWalkNodeObj		[in] �I�_���s�҃m�[�h�I�u�W�F�N�g
	* @param	fromLinks			[in] �n�_�����s�҃����N���X�g
	* @param	toLinks				[in] �I�_�����s�҃����N���X�g
	* @param	walkLink			[in] ���s�҃����N�Q�N���X
	* @param	walkNodeClassIndex	[in] ���s�҃m�[�h��ʂ̃C���f�b�N�X
	*/
	void connectLinkCheck(
		long walkLinkClass,
		const GeoClass& fromWalkNodeObj,
		const GeoClass& toWalkNodeObj,
		const std::list<GeoClass> & fromLinks,
		const std::list<GeoClass>& toLinks,
		const FeaturesPack& walkLink,
		long walkNodeClassIndex
	);

	/**
	* @brief	���s����`��̏ꍇ�̑����`�F�b�N
	* @node		�G���[�R�[�h 355,356
	* @param	walkLinkClass	[in] ���s�Ҏ��
	* @param	floorMoveClass	[in] �K�w�ړ����
	* @param	escalatorF		[in] �G�X�J���[�^�t���O
	* @param	onewayCode		[in] ����ʍs��ʃR�[�h
	* @param	walkNodeObj		[in] ���s�҃m�[�h�I�u�W�F�N�g
	* @param	fromWalkNodeID	[in] �n�_�m�[�hOID
	* @param	toWalkNodeID	[in] �I�_�m�[�hOID
	* @param	walkNodeLinkIDs	[in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	* @param	walkLink		[in] ���s�҃����N�Q�N���X
	* @param	walkLinkRTree	[in] ���s�҃����N��RTree
	*/
	void parallelCheck(
		long walkLinkClass,
		long floorMoveClass,
		long escalatorF,
		long onewayCode,
		const GeoClass& walkLinkObj,
		long fromWalkNodeID,
		long toWalkNodeID,
		const NODEIDMAP& walkNodeLinkIDs,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree
	);

	/**
	* @brief	�n�������`�F�b�N
	* @node		�G���[�R�[�h 359
	* @param	walkLinkClass	[in] ���s�Ҏ��
	* @param	walkLinkID		[in] ���s�҃����NOID
	* @param	fromWalkNodeObj	[in] �n�_�m�[�h�I�u�W�F�N�g
	* @param	toWalkNodeObj	[in] �I�_�m�[�h�I�u�W�F�N�g
	* @param	fromLinks		[in] �n�_�����s�҃����N���X�g
	* @param	toLinks			[in] �I�_�����s�҃����N���X�g
	* @param	walkLink		[in] ���s�҃����N�Q�N���X
	* @param	walkNode		[in] ���s�҃m�[�h�Q�N���X
	* @param	meshcode		[in] ���b�V���R�[�h
	*/
	void underGroundCheck(
		long walkLinkClass,
		long walkLinkID,
		const GeoClass& fromWalkNodeObj,
		const GeoClass& toWalkNodeObj,
		const std::list<GeoClass> & fromLinks,
		const std::list<GeoClass>& toLinks,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const CString& meshcode
	);

	/**
	* @brief	�����I�ڑ����`�F�b�N
	* @node		�G���[�R�[�h 353
	* @param	ipMeshGeo			[in] �s�s�n�}���b�V���`��
	* @param	walkLink			[in] ���s�҃����N�Q�N���X
	* @param	walkNode			[in] ���s�҃m�[�h�Q�N���X
	* @param	walkNodeLinkIDs		[in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	* @param	walkLinkMapFor353	[in] �G���[�R�[�h353�p�̃����N���X�g
	*/
	void attributeConnectionCheck(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP walkNodeLinkIDs,
		const std::map<long, GeoClass>& walkLinkMapFor353
	);

	/**
	* @brief	����From/To�`�F�b�N
	* @node		�G���[�R�[�h 327
	* @param	dupNodeMap		[in] �d���`�F�b�N�p�}�b�v
	* @param	walkLink		[in] ���s�҃����N�Q�N���X
	* @param	meshcode		[in] ���b�V���R�[�h
	*/
	void sameFromToCheck(
		const std::map<std::pair<long, long>, std::list<long>>& dupNodeMap,
		const FeaturesPack& walkLink,
		const CString& meshcode
	);

	/**
	* @brief	�����N�ɕt���Ă���w��ID�Ɣ��Α���ID��Ԃ�
	* @node		�n�_�Ȃ�I�_�A�I�_�Ȃ�n�_
	* @param	dupNodeMap		[in] �d���`�F�b�N�p�}�b�v
	* @param	walkLink		[in] ���s�҃����N�Q�N���X
	* @param	meshcode		[in] ���b�V���R�[�h
	* @retval	���΂�OID
	*/
	long anotherNodeID(
		const GeoClass& linkObj,
		long nodeId,
		long fromIdIndex,
		long toIdIndex
	);

	/**
	* @brief	���f�����̏I�[���X�g��Ԃ�
	* @node		�m�[�h����A���̎�ʂ₻�̐�̃����N��ʂ����ɁA
	*           �I�[�Ƃ��Ĉ����m�[�h���X�g�����߂�
	*			�r���Ɂu�l�ԋ��p�v�Ɓu���f�����v�݂̂̃m�[�h����������A
	*			���̐������[bug 12633]�Ή�
	* @param	linkObj			[in] ���s�҃����N�I�u�W�F�N�g
	* @param	nodeObj			[in] ���s�҃m�[�h�I�u�W�F�N�g
	* @param	nodeClassIndex	[in] ���s�҃m�[�h��ʃC���f�b�N�X
	* @param	walkClassIndex	[in] ���s�Ҏ�ʃC���f�b�N�X
	* @param	fromIdIndex		[in] �n�_OID�C���f�b�N�X
	* @param	toIdIndex		[in] �I�_OID�C���f�b�N�X
	* @param	walkLink		[in] ���s�҃����N�Q�N���X
	* @param	walkNode		[in] ���s�҃m�[�h�Q�N���X
	* @param	walkNodeLinkIDs	[in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	* @param	inspectedIDs	[in/out] �����ς݂̃m�[�hID�Z�b�g
	* @retval	���f�����̏I�[�Ƃ��Ĉ����m�[�h��OID���X�g
	*/
	std::list<GeoClass> searchCrossRoadNode(
		const GeoClass& linkObj,
		const GeoClass& nodeObj,
		long nodeClassIndex,
		long walkClassIndex,
		long fromIdIndex,
		long toIdIndex,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs,
		std::set<long>& inspectedIDs
	);

	/**
	* @brief	���f�����̗��[�̊֘A���H�m�[�h�i�����_���́j�`�F�b�N
	* @node		[bug 12633]�Ή� �G���[�R�[�h 361
	* @param	nodeClassIndex	[in] ���s�҃m�[�h��ʃC���f�b�N�X
	* @param	walkClassIndex	[in] ���s�Ҏ�ʃC���f�b�N�X
	* @param	fromIdIndex		[in] �n�_OID�C���f�b�N�X
	* @param	toIdIndex		[in] �I�_OID�C���f�b�N�X
	* @param	linkObj			[in] ���s�҃����N�I�u�W�F�N�g
	* @param	nodeObj			[in] ���s�҃m�[�h�I�u�W�F�N�g
	* @param	walkLink		[in] ���s�҃����N�Q�N���X
	* @param	walkNode		[in] ���s�҃m�[�h�Q�N���X
	* @param	walkNodeLinkIDs	[in] ���s�҃m�[�h�ɐڑ�������s�҃����N��ID�Ή��\
	* @param	meshcode		[in] ���b�V���R�[�h
	*/
	void crossRoadNodeCheck(
		long nodeClassIndex,
		long walkClassIndex,
		long fromIdIndex,
		long toIdIndex,
		const GeoClass& linkObj,
		const GeoClass& fromWalkNodeObj,
		const GeoClass& toWalkNodeObj,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs,
		const CString& meshcode
	);

	/**
	* @brief	���D�m�[�h�̑O�㐮���`�F�b�N
	* @node		[bug 12687]�̃`�F�b�N�P�Ή�
	* @param	nodeClassIndex	[in] ���s�҃m�[�h��ʃC���f�b�N�X
	* @param	walkClassIndex	[in] ���s�Ҏ�ʃC���f�b�N�X
	* @param	nodeObj			[in] ���s�҃m�[�h�I�u�W�F�N�g
	* @param	links			[in] �m�[�h�ɐڑ�����Ă��郊���N���X�g
	*/
	bool ticketGateNodeCheck(
		long nodeClassIndex,
		long walkClassIndex,
		const GeoClass& nodeObj,
		const std::list<GeoClass> & links
	);

	/**
	* @brief	���D�������N�̗��[�m�[�h�̐����`�F�b�N
	* @node		[bug 12687]�̃`�F�b�N�P�Ή�
	* @param	nodeClassIndex	[in] ���s�҃m�[�h��ʃC���f�b�N�X
	* @param	walkClassIndex	[in] ���s�Ҏ�ʃC���f�b�N�X
	* @param	fromNodeObj		[in] �n�_�����s�҃m�[�h�I�u�W�F�N�g
	* @param	toNodeObj		[in] �I�_�����s�҃m�[�h�I�u�W�F�N�g
	* @param	fromLinks		[in] �n�_���m�[�h�ɐڑ�����Ă��郊���N���X�g
	* @param	toLinks			[in] �I�_���m�[�h�ɐڑ�����Ă��郊���N���X�g
	*/
	void internalTicketGateLinkCheck(
		long nodeClassIndex,
		long walkClassIndex,
		const GeoClass& fromWalkNodeObj,
		const GeoClass& toWalkNodeObj,
		const std::list<GeoClass> & fromLinks,
		const std::list<GeoClass>& toLinks
	);

	/**
	 * @brief �X�N�����u���t���O���t�^����Ă���̂ɃX�N�����u���G���A���ɖ������`�F�b�N
	 * @param [in] walkLinkObj        �`�F�b�N�Ώۂ̕��s�҃����N�̑����E�`��
	 * @param [in] scrambleArea       �X�N�����u���G���A�Q�N���X
	 * @param [in] scrambleAreaRTree  �X�N�����u���G���ARTree
	 * @param [in] scrambleFlag       �X�N�����u���t���O
	 */
	void walkLinkNotOnScrambleAreaCheck(
		const GeoClass& walkLinkObj,
		const FeaturesPack& scrambleArea,
		const BGRTree& scrambleAreaRTree,
		long scrambleFlag
	) const;

	/**
	 * @brief �X�N�����u���G���A���Ȃ̂ɃX�N�����u���t���O���t�^����Ă��Ȃ����`�F�b�N
	 * @param [in] walkLinkObj        �`�F�b�N�Ώۂ̕��s�҃����N�̑����E�`��
	 * @param [in] scrambleArea       �X�N�����u���G���A�Q�N���X
	 * @param [in] scrambleAreaRTree  �X�N�����u���G���ARTree
	 * @param [in] walkLinkClass      ���s�Ҏ��
	 * @param [in] scrambleFlag       �X�N�����u���t���O
	 */
	void walkLinkOnScrambleAreaCheck(
		const GeoClass& walkLinkObj,
		const FeaturesPack& scrambleArea,
		const BGRTree& scrambleAreaRTree,
		long walkLinkClass,
		long scrambleFlag
	) const;

	/**
	 * @brief �Ԉ֎q�ʍs�R�[�h���s���Ȓl���`�F�b�N
	 * @note �G���[�R�[�h:365
	 * @param [in] walkWheelChair     �Ԉ֎q�ʍs�R�[�h
	 */
	void checkWheelChair(
		long  walkWheelChair
	) const;

	/**
	 * @brief �̂�Δԍ����s���Ȓl���`�F�b�N
	 * @note �G���[�R�[�h:366
	 * @param [in] walkPlatformNumber     �̂�Δԍ�
	 */
	void checkPlatformNumber(
		long  walkPlatformNumber
	) const;

	/**
	 * @brief �������W�J���R�l�N�V�������`�F�b�N
	 * @note �G���[�R�[�h:367
	 * @param [in] walkLinkObj        �`�F�b�N�Ώۂ̕��s�҃����N�̑����E�`��
	 */
	void checkMicroLogicalConnection(
		const GeoClass& walkLinkObj
	) const;

	/**
	 * @brief ���W�J���R�l�N�V�����ƌ������Ă��邩�`�F�b�N
	 * @note �G���[�R�[�h:368
	 * @param [in] walkLinkObj        �`�F�b�N�Ώۂ̕��s�҃����N�̑����E�`��
	 * @param [in] walkLink           ���s�҃����N�Q�N���X
	 * @param [in] walkLinkRTree      ���s�҃����N��RTree
	 */
	void WalkLinkCheckFunction::checkCrossWalkLink(
		const GeoClass& walkLinkObj,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree
	) const;

protected:
	COutPut& m_output; //!< ���O�o�͗p�C���X�^���X�ւ̎Q��
	CComVariant m_modifyFilter; //!< �ҏW�����t�B���^
	const std::map<long, CString>& m_walkLinkClassMap; //!< ���s�Ҏ�ʂ̃R�[�h�l�h���C���}�b�v
	const RowsPack& m_walkCode; //!< ���s�ҘH���R�[�h�̏��
	const MultiRowsPack& m_walkNoPassage; //!< ���s�Ғʍs�֎~�K���e�[�u�����
	const MultiRowsPack& m_walkOneway; //!< ���s�҈���ʍs�K���e�[�u���̏��
	double m_dist_threshold; //!< ��������臒l(m)
	double m_angle_threshold;    //!< �p�x臒l(�x)
	double m_coordinateTolerance;//!< ���W���e�l
	sindy::schema::walk_type::type_class::ECode m_dataType;              //!< �f�[�^�^�C�v
	double	m_microLogicalConnection;                                    //!< �������W�J���R�l�N�V����臒l(m)
};
