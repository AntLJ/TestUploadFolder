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

#include <TDC/sindylib_core/FieldMap.h>
#include "Arguments.h"
#include "OutputLog.h"
#include "Properties.h"
#include "DatabaseControl.h"
#include "TimeReg.h"
#include "HeightInfoMgr.h"
#include "FieldLists.h"
/**
 * @class	CCheckWalk
 * @brief	���C���N���X
 */
class CCheckWalk
{
public:

	/**
	 * @brief �R���X�g���N�^
	 * @param args [in] �R�}���h���C������
	 * @param prop [in]	�ݒ���
	 */
	CCheckWalk(const Arguments& args, const CProperties& prop);

	/**
	 * @brief	�f�X�g���N�^
	 */
	virtual ~CCheckWalk() {}

	/**
	 * @brief	���s
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool execute();

private:
	typedef std::pair<IGeometryPtr, IGeometryPtr> GeometryPair;
	typedef std::pair<const long, std::pair<IGeometryPtr, IGeometryPtr>> CityMeshPair;
	typedef std::map<long, GeometryPair> CityMeshMap;
	typedef std::map<CString, IGeometryPtr> BaseMeshMap;
	/**
	* @brief �w�b�_��񓙏o��
	*/
	void OutputHeader();

	/**
	* @brief �Ώۃ��b�V���E���ӂ̃|���S���ǂݍ���
	*/
	void LoadMesh();


	/**
	* @brief �s�s�n�}���b�V���P�ʂł̏���
	*/
	void CheckByCityMesh();

	/**
	* @brief ���b�V���֌W�Ȃ��S�̂ł̏���
	*/
	void CheckByAllMesh();

	/**
	* @brief ���k���b�V���P�ʂł̏���
	*/
	void CheckByBaseMesh();

	/**
	* @brief �e���[�h�̓s�s�n�}�P�ʂł̏���
	* @param function [in] �e���[�h���̊֐��̃A�h���X
	*/
	void CheckFunctionByCityMesh(
		err_code::ECode(CCheckWalk::*function)(const CityMeshPair&));

	/**
	 * @brief all���[�h�p�̃`�F�b�N
	 * @param cityMeshPair [in]
	 * <�s�s�n�}���b�V���R�[�h, <�s�s�n�}���b�V���`��, �s�s�n�}���b�V������9���b�V���`��>>
	 * @return �G���[�R�[�h
	 */
	err_code::ECode allCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief humancar���[�h�p�̃`�F�b�N
	 * @param cityMeshPair [in]
	 * <�s�s�n�}���b�V���R�[�h, <�s�s�n�}���b�V���`��, �s�s�n�}���b�V������9���b�V���`��>>
	 * @return �G���[�R�[�h
	 */
	err_code::ECode humancarCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief walkable���[�h�p�̃`�F�b�N
	 * @param cityMeshPair [in]
	 * <�s�s�n�}���b�V���R�[�h, <�s�s�n�}���b�V���`��, �s�s�n�}���b�V������9���b�V���`��>>
	 * @return �G���[�R�[�h
	 */
	err_code::ECode walkableCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief height_node���[�h�p�̃`�F�b�N
	 * @param cityMeshPair [in]
	 * <�s�s�n�}���b�V���R�[�h, <�s�s�n�}���b�V���`��, �s�s�n�}���b�V������9���b�V���`��>>
	 * @return �G���[�R�[�h
	 */
	err_code::ECode heightNodeCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief link_relation���[�h�p�̃`�F�b�N
	 * @param cityMeshPair [in]
	 * <�s�s�n�}���b�V���R�[�h, <�s�s�n�}���b�V���`��, �s�s�n�}���b�V������9���b�V���`��>>
	 * @return �G���[�R�[�h
	 */
	err_code::ECode linkRelationCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief under���[�h�p�̃`�F�b�N
	 * @param cityMeshPair [in]
	 * <�s�s�n�}���b�V���R�[�h, <�s�s�n�}���b�V���`��, �s�s�n�}���b�V������9���b�V���`��>>
	 * @return �G���[�R�[�h
	 */
	err_code::ECode underGroundCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief other���[�h�p�̃`�F�b�N
	 * @param cityMeshPair [in]
	 * <�s�s�n�}���b�V���R�[�h, <�s�s�n�}���b�V���`��, �s�s�n�}���b�V������9���b�V���`��>>
	 * @return �G���[�R�[�h
	 */
	err_code::ECode otherCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief release���[�h�p�̃`�F�b�N
	 * @param cityMeshPair [in]
	 * <�s�s�n�}���b�V���R�[�h, <�s�s�n�}���b�V���`��, �s�s�n�}���b�V������9���b�V���`��>>
	 * @return �G���[�R�[�h
	 */
	err_code::ECode releaseCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief pedxing���[�h�p�̃`�F�b�N
	 * @param [in] cityMeshPair �s�s�n�}���b�V���R�[�h
	 * @return �G���[�R�[�h
	 */
	err_code::ECode pedxingCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief �x�[�X���b�V���P�ʂŃ`�F�b�N���K�v�Ȃ��̂̃`�F�b�N
	 */
	void checkBaseMesh();


	/**
	 * @brief	������
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool init();

	/**
	 * @brief ������񋓂����t�@�C����ǂݍ���
	 * @detail �x�[�X���b�V�����w�肳��Ă���ꍇ�A���̃x�[�X���b�V�����܂�ł���s�s�n�}���b�V�����w�肷��
	 * @note �T�^�I�ɂ̓��b�V�����X�g�t�@�C���̓ǂݍ��݂Ɏg��
	 * @param fileName      [in] ���X�g�t�@�C����
	 * @param integerList   [out] �o�̓��X�g��set
	 * @param cityMeshTable [in] �s�s�n�}���b�V���̃e�[�u��
	 * @retval true �ǂݍ��ݐ���
	 * @retval false �ǂݍ��ݎ��s
	 */
	bool loadMeshList(const CString& fileName, std::set<long>& integerList, const ITablePtr& cityMeshTable);

	/**
	 * @brief	�^�������b�V���א�9���b�V�����X�g���쐬����
	 */
	void Get9Mesh(long mesh, std::set<long>& meshlist);

	/**
	 * @brief	���b�V�����X�g����A���b�V���|���S�����擾
	 */
	bool GetMeshPolygons(long mesh, IGeometryPtr& ipMeshGeo, IGeometryPtr& ipUnionMeshGeo);

	/**
	 * @brief	�^����ꂽ���b�V���`����̓��H�����N�Ɠ��H�m�[�h���擾
	 * @param	ipMeshGeo	[in]	���b�V���|���S���̃W�I���g��
	 * @param	linkMap	[out] �擾�������H�����NMap(�L�[�FObjectID)
	 * @param	nodeMap	[out] �擾�������H�m�[�hMap(�L�[�FObjectID)
	 * @param	segmentAttrMap	[out] �擾���������N������Map(�L�[�FObjectID)
	 * @param	nodeLinkIDs	[out] FROM/TO�m�[�hID���L�[�Ƃ����L�[�l(�����N��OID)��map
	 * @retval	true	����
	 * @retval	false ���s
	 */
	bool GetRoadInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& roadLink,
		FeaturesPack& roadNode,
		FeaturesPack& segmentAttr,
		NODEIDMAP& nodeLinkIDs
	);

	/**
	 * @brief	�^����ꂽ���b�V���`����̕��s�҃����N�ƕ��s�҃m�[�h���擾
	 * @param	ipMeshGeo	[in]	���b�V���|���S���̃W�I���g��
	 * @param	walkLink	[in/out] �擾�������s�҃����N�Q�N���X
	 * @param	walkNode	[in/out] �擾�������s�҃m�[�h�Q�N���X
	 * @param	nodeNodeIDs	[out] FROM/TO�m�[�hID���L�[�Ƃ����m�[�hID��map
	 * @param	nodeLinkIDs	[out] FROM/TO�m�[�hID���L�[�Ƃ����L�[�l(�����N��OID)��map
	 * @retval	true	����
	 * @retval	false ���s
	 */
	bool GetWalkInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& walkLink,
		FeaturesPack& walkNode,
		NODEIDMAP& walkNodeNodeIDs,
		NODEIDMAP& walkNodeLinkIDs
	);
	/**
	* @brief	�^����ꂽ���b�V���`����̕��s�҃����N�ƕ��s�҃m�[�h���擾
	* @param	ipMeshGeo	[in]	���b�V���|���S���̃W�I���g��
	* @param	walkLink	[in/out] �擾�������s�҃����N�Q�N���X
	* @param	walkNode	[in/out] �擾�������s�҃m�[�h�Q�N���X
	* @param	nodeLinkIDs	[out] FROM/TO�m�[�hID���L�[�Ƃ����L�[�l(�����N��OID)��map
	* @retval	true	����
	* @retval	false ���s
	*/
	bool GetWalkInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& walkLink,
		FeaturesPack& walkNode,
		NODEIDMAP& walkNodeLinkIDs
	);

	/**
	 * @brief	�w�肳�ꂽ���b�V�����̉w�o����POI�f�[�^���擾
	 * @param	ipMeshGeo	[in]	�擾���郁�b�V���͈�
	 * @param	stationGate	[in/out]	�擾�����w�o����POI�Q�N���X
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool GetStationGateInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& stationGate
	);

	/**
	 * @brief	�w�肳�ꂽ���b�V������POI�|�C���g�f�[�^���擾
	 * @param	ipMeshGeo	[in]	�擾���郁�b�V���͈�
	 * @param	poiPoint	[in/out]	�擾����POI�|�C���g�Q�N���X
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool GetPoiPointInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& poiPoint
	);

	/**
	 * @brief	�w�肳�ꂽ���b�V�����̏d�Ȃ胊���N�����f�[�^���擾
	 * @param	ipMeshGeo	[in]	�擾���郁�b�V���͈�
	 * @param	heightNode	[in/out]	�擾�����d�Ȃ胊���N�����Q�N���X
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool GetHeightNodeInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& heightNode
	);
	/**
	 * @brief	�w�肳�ꂽ���b�V�����̒n���X�|���S�����擾
	 * @param	ipMeshGeo	[in]	�擾���郁�b�V���͈�
	 * @param	underArea	[in/out]	�擾�����n���X�|���S���Q�N���X
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool GetUndergroundInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& underArea
	);

	/**
	 * @brief	���s�Ҋ֘A�e�[�u������̃f�[�^�擾
	 */
	bool GetWalkRelationTables(void);

	/**
	 * @brief	�t�B�[�`���[�N���X�̃J�[�\���ƃt�B�[���h�}�b�v���擾����
	 * @param	tableValue	[in]	�擾�Ώۂ̃e�[�u��
	 * @param	fieldList	[in]	�擾�Ώۂ̃t�B�[���h�Q
	 * @param	ipMeshGeo	[in]	�擾���郁�b�V���͈�
	 * @param	strWhere	[in]	�擾������
	 * @param	ipCursor	[out]	�擾�����f�[�^�̃J�[�\��
	 * @param	fieldMap	[out]	�擾�����f�[�^�̃t�B�[���h�}�b�v
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool GetFeatureCursorAndFieldMap(
		const table tableValue,
		const FIELD_LIST& fieldList,
		const IGeometryPtr& ipMeshGeo,
		const CString& strWhere,
		IFeatureCursorPtr& ipCursor,
		sindy::CFieldMap& fieldMap
	);

	/**
	 * @brief	�e�[�u���̃J�[�\�����擾����
	 * @param	tableValue	[in]	�擾�Ώۂ̃e�[�u��
	 * @param	fieldList	[in]	�擾�Ώۂ̃t�B�[���h�Q
	 * @param	ipCursor	[out]	�擾�����f�[�^�̃J�[�\��
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool GetTableCursor(
		const table tableValue,
		const FIELD_LIST& fieldList,
		_ICursorPtr& ipCursor
	);

	/**
	 * @brief	�e�[�u���̃J�[�\���ƃt�B�[���h�}�b�v���擾����
	 * @param	tableValue	[in]	�擾�Ώۂ̃e�[�u��
	 * @param	fieldList	[in]	�擾�Ώۂ̃t�B�[���h�Q
	 * @param	ipCursor	[out]	�擾�����f�[�^�̃J�[�\��
	 * @param	fieldMap	[out]	�擾�����f�[�^�̃t�B�[���h�}�b�v
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool GetTableCursorAndFieldMap(
		const table tableValue,
		const FIELD_LIST& fieldList,
		_ICursorPtr& ipCursor,
		sindy::CFieldMap& fieldMap
	);

	/**
	 * @brief	�e�[�u���f�[�^���擾����
	 * @param	tableValue	[in]	�擾�Ώۂ̃e�[�u��
	 * @param	fieldList	[in]	�擾�Ώۂ̃t�B�[���h�Q
	 * @param	keyFieldName	[in]	�L�[�t�B�[���h��
	 * @param	dataMap	[out]	�擾�����f�[�^��map
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool GetRelationTable(
		const table tableValue,
		const FIELD_LIST& fieldList,
		const CString& keyFieldName,
		RowsPack& data
	);

	/**
	 * @brief	�e�[�u���f�[�^���擾����
	 * @param	tableValue	[in]	�擾�Ώۂ̃e�[�u��
	 * @param	fieldList	[in]	�擾�Ώۂ̃t�B�[���h�Q
	 * @param	keyFieldName	[in]	�L�[�t�B�[���h��
	 * @param	dataMap	[out]	�擾�����f�[�^��multimap
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool GetRelationTable(
		const table tableValue,
		const FIELD_LIST& fieldList,
		const CString& keyFieldName,
		MultiRowsPack& data
	);

	/**
	* @brief	�e�[�u���f�[�^���擾����
	* @note    FeaturesPack��m_ipTable��m_FieldMap���Z�b�g���ĕԂ�
	* @param	tableValue	[in]	�擾�Ώۂ̃e�[�u��
	* @return	FeaturesPack
	*/
	FeaturesPack CreateFeaturesPack(
		table tableValue
	);

	/**
	 * @brief	���H�m�[�h�̃`�F�b�N
	 * @param	meshcode	[in]	�`�F�b�N�Ώۃ��b�V���R�[�h
	 * @param	ipMeshGeo	[in]	�`�F�b�N�Ώۃ��b�V���̃W�I���g��
	 * @param	roadNode	[in]	�`�F�b�N�Ώۓ��H�m�[�h�Q�N���X
	 * @param	walkNode	[in]	�`�F�b�N�Q�Ɨp���s�҃m�[�h�Q�N���X
	 * @param	walkNodeRTree	[in]	�`�F�b�N�Q�Ɨp���s�҃m�[�hRTree
	 */
	void CheckRoadNode(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadNode,
		const FeaturesPack& walkNode,
		const BGRTree& walkNodeRTree
	);

	/**
	 * @brief	�Ȃ���`�F�b�N
	 */
	void CheckWalkRel(
		const IGeometryPtr& ipMeshGeo,
		const std::map<long, std::pair<IGeometryPtr, IGeometryPtr>>& cityMeshMap,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeNodeIDs,
		const NODEIDMAP& walkNodeLinkIDs
	);

	/**
	 * @brief	���s�Ғʍs�֎~�K���e�[�u���̃`�F�b�N(�S��)
	 * @param	walkLinkOIDSet	[in]	���s�҃����N��OID��Set
	 * @param	walkNoPassageMap	[in]	���s�Ғʍs�֎~�K���e�[�u��
	 */
	void CheckWalkNoPassageAll(
		const std::set<long>& walkLinkOIDSet,
		const MultiRowsPack& walkNoPassage
	);

	/**
	 * @brief	���s�҈���ʍs�K���e�[�u���̃`�F�b�N(�S��)
	 * @param	walkLinkOIDSet	[in]	���s�҃����N��OID��Set
	 * @param	walkNoPassageMap	[in]	���s�҈���ʍs�K���e�[�u��
	 */
	void CheckWalkOnewayAll(
		const std::set<long>& walkLinkOIDSet,
		const MultiRowsPack& walkOneway
	);

	/**
	 * @brief	���ԋK���֘A�e�[�u����LinkID���݃`�F�b�N
	 * @param	walkLinkOIDSet	[in]	���s�҃����N��OID��Set
	 * @param	timeRegulationMap	[in]	���ԋK���֘A�e�[�u����Map
	 * @param	errCode	[in]	�o�͂���G���[�R�[�h
	 */
	void CheckLinkIDExist(
		const std::set<long>& walkLinkOIDSet,
		const MultiRowsPack& timeRegulation,
		const err_code::ECode errCode
	);

	/**
	 * @brief	�����N�R�t�e�[�u���̃`�F�b�N(�S��)
	 * @param	linkRelOIDSet	[in]	���s�҃����N��OID��Set
	 * @param	linkRelationMap	[in]	�����N�R�t�e�[�u����Map
	 */
	void CheckLinkRelationAll(
		const std::set<long>& linkRelOIDSet,
		const MultiRowsPack& linkRelation
	);

	/**
	 * @brief	�w�o�����|�C���g�̃`�F�b�N
	 * @param	meshcode	[in]	�`�F�b�N�Ώۃ��b�V���R�[�h
	 * @param	ipMeshGeo	[in]	�`�F�b�N�Ώۂ̃W�I���g��
	 * @param	stationGatePointMap	[in]	�`�F�b�N�Ώۉw�o�����|�C���gMap
	 * @param	walkNodeMap	[in]	�`�F�b�N�Q�Ɨp���s�҃m�[�hMap
	 * @param	walkNodeRTree	[in]	�`�F�b�N�Q�Ɨp���s�҃m�[�hRTree
	 */
	void CheckStationGatePoint(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& stationGatePoint,
		const FeaturesPack& walkNode,
		const BGRTree& walkNodeRTree
	);

	/**
	 * @brief	�w�肳�ꂽNodeID�ƃ����N����Đڑ�����NodeID��Set���擾����B
	 * @param	nodeID	[in]	�m�[�hID
	 * @param	walkNodeNodeIDs	[in]	�m�[�hID��Map
	 */
	std::set<long> GetRelNodeIDs(
		long nodeID,
		const NODEIDMAP& walkNodeNodeIDs
	);

	/**
	 * @brief	���s�҃^�C�v�e�[�u������WALK_TYPE�̒l���擾����B
	 *
	 * @return	�擾����WALK_TYPE�̒l
	 */
	long getWalkTypeValue();

	/**
	 * @brief	�w�肳�ꂽ���b�V�����̃X�N�����u���G���A�f�[�^���擾
	 * @param	ipMeshGeo       [in]        �擾���郁�b�V���͈�
	 * @param	scrambleArea    [in/out]    �擾�����X�N�����u���G���A�Q�N���X
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool getScrambleAreaInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& scrambleArea
	);

	/**
	 * @brief	�w�肳�ꂽ���b�V�����̉��f�|�C���g�f�[�^���擾
	 * @param	ipMeshGeo   [in]        �擾���郁�b�V���͈�
	 * @param	pedxing     [in/out]    �擾�������f�|�C���g�Q�N���X
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool getPedxingInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& pedxing
	);

	/**
	 * @brief all��O���[�h�p�̃`�F�b�N
	 */
	void allCheckException();

	/**
	 * @brief other��O���[�h�p�̃`�F�b�N
	 */
	void otherCheckException();

	/**
	 * @brief  height_node��O���[�h�p�̃`�F�b�N
	 */
	void heightNodeCheckException();

	/**
	 * @brief release��O���[�h�p�̃`�F�b�N
	 */
	void releaseCheckException();

	/**
	 * @brief pedxing��O���[�h�p�̃`�F�b�N
	 */
	void pedxingCheckException();

private:

	CString	m_strStationDB; //!< �wPOI�i�[��DB�̐ڑ�������
	CString	m_strRoadDB;    //!< ���HNE�i�[��DB�̐ڑ�������
	CString	m_strWalkDB;    //!< ���s��NE�i�[��DB�̐ڑ�������
	CString	m_strMeshDB;    //!< ���b�V���i�[��DB�̐ڑ�������
	CString	m_strPoiDB;     //!< POI�i�[��DB�̐ڑ�������
	CString m_strUnderDB;   //!< �n���X�|���S���i�[��DB�̐ڑ�������
	CString	m_strMeshList;
	CString	m_strOutput;
	double	m_dist_threshold;     //!< ��������臒l(m)
	double	m_neighbor_threshold; //!< �ߖT����臒l(m)
	double	m_refer_id_threshold; //!< �֘AID����臒l(m)
	double	m_angle_threshold;    //!< �p�x臒l(�x)
	/**
	 * @brief	�����N�R�t�`�F�b�N�p����臒l
	 */
	double	m_buddy_dist_threshold;
	/**
	 * @brief	�����N�R�t�`�F�b�N�p�p�x臒l
	 */
	double	m_buddy_angle_threshold;

	double	m_sindyTolerance_threshold; //!< SiNDY���e�l(m)
	double  m_microLogicalConnection; //!< �������W�J���R�l�N�V����臒l(m)

	CString	m_strDistThreshold;
	CString	m_strNeighborThreshold;
	CString	m_strReferIDThreshold;
	CString	m_strAngleThreshold;
	CString m_filterDay;  //!< �ҏW�����t�B���^�̓��t�����̐ݒ�l���i�[
	CString m_filterTime; //!< �ҏW�����t�B���^�̎��ԕ����̐ݒ�l���i�[

	/**
	 * @brief	�����N�R�t�`�F�b�N�p����臒l(������)
	 */
	CString	m_strBuddyDistThreshold;
	/**
	 * @brief	�����N�R�t�`�F�b�N�p�p�x臒l(������)
	 */
	CString	m_strBuddyAngleThreshold;

	/**
	 * @brief	�d�Ȃ胊���N�����m�[�h�`�F�b�N�p�ݒ�t�@�C��
	 */
	uh::tstring	m_height_node_ini;
	CString	m_strHeightNodeIni;

	/**
	 * @brief	�f�[�^�x�[�X�֘A�Ǘ��N���X
	 */
	CDatabaseControl	m_database;
	COutPut	m_output;
	/**
	 * @brief	�d�Ȃ胊���N�����m�[�h�ݒ�Ǘ��N���X
	 */
	HeightInfoMgr	m_heightNodeMgr;

	std::set<long>	m_cityMeshList;
	std::set<long>	m_execGateIDs;

	std::map<long, CString> m_walkLinkClassMap;
	std::map<long, CString> m_floorMoveClassMap;
	std::map<long, CString> m_onewayClassMap;
	std::map<long, CString> m_roadSurfaceClassMap;
	std::map<long, CString> m_walkRoadClassMap;
	std::map<long, CString> m_walkNodeClassMap;
	std::map<long, CString> m_walkableMap;

	/**
	 * @brief	���s�ҘH���R�[�h��map
	 */
	RowsPack m_walkCodeMap;
	/**
	 * @brief	���s�Ғʍs�֎~�K���e�[�u����multimap
	 */
	MultiRowsPack m_walkNoPassageMap;
	/**
	 * @brief	���s�҈���ʍs�K���e�[�u����multimap
	 */
	MultiRowsPack m_walkOnewayMap;
	/**
	 * @brief	�����N�R�t�e�[�u����multimap
	 */
	MultiRowsPack m_linkRelationMap;
	/**
	 * @brief	�d�Ȃ胊���N�����m�[�h��multimap
	 */
	MultiRowsPack m_heightNodeMap;

	/**
	 * @brief	�`�F�b�N�Ώ�
	 */
	properties::check_target m_checkTarget;
	/**
	 * @brief	�`�F�b�N�Ώہi������j
	 */
	CString	m_strCheckTarget;

	double m_coordinateTolerance;//!< ���W���e�l
	CComVariant m_modifyFilter; //!< �ҏW�����t�B���^
	CityMeshMap m_cityMeshMap;	//!< <�s�s�n�}���b�V���R�[�h, <�s�s�n�}���b�V���`��, �s�s�n�}���b�V������9���b�V���`��>>
	BaseMeshMap m_baseMeshMap;	//!< <�x�[�X���b�V���R�[�h, �x�[�X���b�V���`��>
	sindy::schema::walk_type::type_class::ECode m_dataType;             //!< �f�[�^�^�C�v
};

