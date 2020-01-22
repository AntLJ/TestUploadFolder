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

#include "Arguments.h"
#include "OutputLog.h"
#include "FeatureObjects.h"
#include <TDC/sindylib_core/FieldMap.h>

/**
 * @brief   ���C���N���X
 */
class CCheckRailwayNW
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CCheckRailwayNW( const Arguments& args ){
		m_strRailNwDB        = (args.m_strRailNwDB).c_str();
		m_strRailPoiDB       = (args.m_strRailPoiDB).c_str();
		m_strMeshDB          = (args.m_strMeshDB).c_str();
		m_strStationDB       = (args.m_strStationDB).c_str();
		//m_strMeshLog         = (args.m_strMeshLog).c_str();
		//m_strQueryLog        = (args.m_strQueryLog).c_str();
		m_strLog             = (args.m_strLog).c_str();
		m_strMeshList        = (args.m_strMeshList).c_str();
		m_dist_threshold     = _ttof( ( args.m_dist_threshold ).c_str() );
		m_neighbor_threshold = _ttof( ( args.m_neighbor_threshold ).c_str() );
		//m_meshMode           = args.m_meshMode;
		//m_queryMode          = args.m_queryMode;

		m_strDistThreshold.Format( _T("%s m"), (args.m_dist_threshold).c_str() );
		m_strNeighborThreshold.Format( _T("%s m"), (args.m_neighbor_threshold).c_str() );
	};

	/**
	 * @brief   �f�X�g���N�^
	 */
	CCheckRailwayNW(){};

	/**
	 * @brief   ���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute();

private:

	typedef std::map<long, std::set<long>> IDMAP;

	/**
	 * @brief   ������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool init();

	//bool init_mesh();
	//bool init_query();

	/**
	 * @brief   ���[�N�X�y�[�X�I�[�v��
	 * @param   name           [in]  �ڑ���
	 * @param   ipWorkspace    [in]  ���[�N�X�y�[�X
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool OpenWorkspace( const CString& name, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief   �t�B�[�`���N���X�I�[�v�����t�B�[���h�}�b�v�쐬
	 * @param   name           [in]  �t�B�[�`���N���X��
	 * @param   ipWorkspace    [in]  �I�[�v�����邽�߂̃��[�N�X�y�[�X
	 * @param   ipFeatureClass [out] �擾����t�B�[�`���N���X
	 * @param   fieldMap       [out] �擾����t�B�[���h�}�b�v
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool OpenFeatureClass( const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass, sindy::CFieldMap& fieldMap );

	/**
	 * @brief   �e�[�u���I�[�v�����t�B�[���h�}�b�v�쐬
	 * @param   name           [in]  �t�B�[�`���N���X��
	 * @param   ipWorkspace    [in]  �I�[�v�����邽�߂̃��[�N�X�y�[�X
	 * @param   ipTable        [out] �擾����e�[�u��
	 * @param   fieldMap       [out] �擾����t�B�[���h�}�b�v
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool OpenTable( const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable, sindy::CFieldMap& fieldMap );

	//�^�������b�V���א�9���b�V�����X�g���쐬����
	void Get9Mesh( long mesh, std::set<long>& meshlist );

	// ���b�V�����X�g����A���b�V���|���S�����擾
	bool GetMeshPolygons( long mesh, std::set<long>& meshlist, IGeometryPtr& ipMeshGeo, IGeometryPtr& ipUnionMeshGeo );

	std::vector<CString> CreateWhereByList( const std::set<long>& list, const CString& field );

	/**
	 * @brief   �^����ꂽ���b�V���`����̕��s�҃����N�Ɠ��H�m�[�h���擾
	 * @param   ipMeshGeo  [in]  ���b�V���|���S���̃W�I���g��
	 * @param   linkMap    [out] �擾�������H�����NMap(�L�[�FObjectID)
	 * @param   nodeMap    [out] �擾�������H�m�[�hMap(�L�[�FObjectID)
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool GetRailwayInMesh( 
		const IGeometryPtr& ipMeshGeo, 
		std::map<long, GeoClass>& railLinkMap,
		std::map<long, GeoClass>& railNodeMap,
		IDMAP& nodeNodeIDs,
		IDMAP& nodeLinkIDs
		);
	bool GetObjInMesh( 
		const IGeometryPtr& ipMeshGeo, 
		const IFeatureClassPtr& ipFeatureClass,
		const sindy::CFieldMap& fieldMap,
		std::map<long, GeoClass>& objMap
		);

	bool GetRelRecAll(
		const ITablePtr& ipTable,
		const sindy::CFieldMap& fieldMap,
		const CString& tableName,
		std::map<long, std::vector<recClass>>& recMap
		);
	bool GetOtherRecAll(
		const ITablePtr& ipTable,
		const sindy::CFieldMap& fieldMap,
		const CString& tableName,
		std::map<long, recClass>& recMap
		);

	/**
	 * @brief R-Tree�𐶐�����
	 * @param geoClassMap [in]  �Ώۃt�B�[�`���N���X
	 * @param rTree       [out] ���ʂ�R-Tree(�`����[r]�͎Q�Ƃ̈Ӗ��ł͂Ȃ��ł�)
	 */
	void createRTree( 
		const std::map<long, GeoClass>& geoMap,
		BGRTree& rTree
		);

	/**
	 * @brief   R-Tree����A�^�������͈̔͂Ɉ���������ID�Q���擾
	 * @param   box   [in]    �擾�������͈�(�G���x���[�v)
	 * @param   rTree [in]    �擾���Ă���Ώۂ�R-Tree
	 * @retval  set<long>     �Ώ۔͈͂�ID�Q
	 */
	std::set<long> getIntersectsIDs( const BGBox& box, const BGRTree& rTree );

	/**
	 *
	 */
	void MeshCheck( 
		long mesh,
		const IGeometryPtr& ipMeshGeo,
		const std::map<long, GeoClass>& railLinkMap,
		const std::map<long, GeoClass>& railNodeMap,
		const std::map<long, GeoClass>& staPointMap,
		const std::map<long, GeoClass>& baseStaMap,
		const BGRTree& railLinkRTree,
		const BGRTree& railNodeRTree,
		const BGRTree& staPointRTree,
		const BGRTree& baseStaRTree,
		const IDMAP& nodeNodeIDs,
		const IDMAP& nodeLinkIDs
		);

	void CheckRailLink(
		const IGeometryPtr& ipMeshGeo,
		const IGeometryPtr& ipBoundaryMesh, 
		const std::map<long, GeoClass>& railLinkMap,
		const std::map<long, GeoClass>& railNodeMap,
		const BGRTree& railLinkRTree,
		const BGRTree& railNodeRTree,
		const IDMAP& nodeLinkIDs
		);

	void CheckRailNode(
		const IGeometryPtr& ipMeshGeo, 
		const IGeometryPtr& ipBoundaryMesh, 
		const std::map<long, GeoClass>& railLinkMap,
		const std::map<long, GeoClass>& railNodeMap,
		const std::map<long, GeoClass>& staPointMap,
		const std::map<long, GeoClass>& baseStaMap,
		const BGRTree& railLinkRTree,
		const BGRTree& railNodeRTree,
		const BGRTree& staPointRTree,
		const BGRTree& baseStaRTree,
		const IDMAP& nodeLinkIDs
		);

	bool FindJoinLineInfo( long railNodeID, std::set<long>& stationIDs, std::set<long>& lineIDs );
	bool FindRailwayLine( long railLinkID, std::set<long>& railwayLineIDs );
	// ID�Z�b�g�Ǝ��g��ID�AObject�}�b�v��n���A���g�ȊO�̃I�u�W�F�N�g���擾
	std::list<GeoClass> GetObjects( 
		long oID,
		const std::set<long>& oIDs,
		const std::map<long, GeoClass>& geoMap
		);

	void QueryCheck();

	void CheckStaNumInLine();

	void CheckNonRel(const ITablePtr& ipTable, const CString& fcTableName, const CString& relTableName, const std::map<long, std::vector<recClass>>& relAll );

	bool GetOIDAll(
		const ITablePtr& ipTable,
		const CString& tableName,
		std::set<long>& oIDs
		);

	void SameLineSeqLinkChk();

	/**
	 * @brief   �w�肵���t�B�[�`���N���X�̊֘A�e�[�u���ƕR�t���S��POI�̃e�[�u���̃��R�[�h�����݂��邩�`�F�b�N����
	 * @param   featureClassName [in]  �t�B�[�`���N���X��
	 * @param   relRecMap        [in]  �֘A�e�[�u���̃��R�[�h�Q
	 * @param   relPoiMap        [in]  �S��POI�̃e�[�u���̃��R�[�h�Q
	 */
	void checkRelRecordExstence(const CString& featureClassName, const std::map<long, std::vector<recClass>>& relRecMap, const std::map<long, recClass>& poiRecMap);

	// ���g�ɂȂ���m�[�h����쐬����
	std::set<long> GetRelNodeIDs( long nodeID, const IDMAP& nodeNodeIDs );

	CString GetLineName( long lineID )
	{
		using namespace sindy::schema::railway_line;
		auto itr = m_railwayLine.find( lineID );
		if( itr == m_railwayLine.end() )
			return _T("");
		return itr->second.attr.at( kName );
	};

private:

	CString           m_strRailNwDB;        //!< �S��NW�ڑ���
	CString           m_strRailPoiDB;       //!< �S��POI�ڑ���
	CString           m_strMeshDB;          //!< ���b�V��DB�ڑ���
	CString           m_strStationDB;       //!< ���k�w�ɐڑ���
	CString           m_strLog;             //!< ���O�t�@�C��
	//CString           m_strMeshLog;         //!< ���b�V�����[�h���O�t�@�C��
	//CString           m_strQueryLog;        //!< �N�G�����[�h���O�t�@�C��
	CString           m_strMeshList;        //!< ���b�V�����X�g
	CString           m_strDistThreshold;   //!< ��������臒l(m)
	CString           m_strNeighborThreshold; //!< �ߖT����臒l(m)

	double            m_dist_threshold;     //!< ��������臒l(m)
	double            m_neighbor_threshold; //!< �ߖT����臒l(m)

	//bool              m_meshMode;           //!< ���b�V�����[�h
	//bool              m_queryMode;          //!< �N�G�����[�h

	IWorkspacePtr     m_ipRailNwWorkspace;  //!< �S��NW���[�N�X�y�[�X
	IWorkspacePtr     m_ipMeshWorkspace;    //!< ���b�V�����[�N�X�y�[�X
	IWorkspacePtr     m_ipRailPoiWorkspace; //!< �S��POI���[�N�X�y�[�X
	IWorkspacePtr     m_ipStationWorkspace; //!< ���k�w�Ƀ��[�N�X�y�[�X

	IFeatureClassPtr  m_ipRailLinkFC;       //!< �S�������N�t�B�[�`���N���X
	IFeatureClassPtr  m_ipRailNodeFC;       //!< �S���m�[�h�t�B�[�`���N���X
	ITablePtr         m_ipRelRailLinkTable; //!< �S�������N�R�t���e�[�u��
	ITablePtr         m_ipRelRailNodeTable; //!< �S���m�[�h�R�t���e�[�u��

	IFeatureClassPtr  m_ipStationPointFC;   //!< �w�|�C���g�t�B�[�`���N���X
	ITablePtr         m_ipRailwayLineTable; //!< �S���H���e�[�u��
	ITablePtr         m_ipJoinLineTable;    //!< ������H���e�[�u��

	IFeatureClassPtr  m_ipBaseStationFC;    //!< ���k�w�Ƀt�B�[�`���N���X

	IFeatureClassPtr  m_ipBaseMeshFC;       //!< �x�[�X���b�V���t�B�[�`���N���X

	sindy::CFieldMap  m_railLinkFM;         //!< �S�������N�t�B�[���h�}�b�v
	sindy::CFieldMap  m_railNodeFM;         //!< �S���m�[�h�t�B�[���h�}�b�v
	sindy::CFieldMap  m_relRailLinkFM;      //!< �S�������N�R�t���e�[�u���t�B�[���h�}�b�v
	sindy::CFieldMap  m_relRailNodeFM;      //!< �S���m�[�h�R�t���e�[�u���t�B�[���h�}�b�v
	sindy::CFieldMap  m_baseMeshFM;         //!< �x�[�X���b�V���t�B�[���h�}�b�v
	sindy::CFieldMap  m_stationPointFM;     //!< �w�|�C���g�t�B�[���h�}�b�v
	sindy::CFieldMap  m_railwayLineFM;      //!< �S���H���e�[�u���t�B�[���h�C���f�b�N�X
	sindy::CFieldMap  m_joinLineFM;         //!< ������H���e�[�u���t�B�[���h�C���f�b�N�X
	sindy::CFieldMap  m_baseStationFM;      //!< ���k�w�Ƀt�B�[���h�}�b�v

	COutPut           m_output;
	std::set<long>    m_meshList;

	std::map<long, CString> m_railLinkClassMap;
	std::map<long, CString> m_dispScaleClassMap;
	std::map<long, CString> m_railNodeClassMap;

	std::map<long, std::vector<recClass>> m_relRailwayLink;  //!< �S�������N�R�t���e�[�u���i�[�p�i�L�[�FRW_LINK_ID�j
	std::map<long, std::vector<recClass>> m_relRailwayNode;  //!< �S���m�[�h�R�t���e�[�u���i�[�p�i�L�[�FRW_NODE_ID�j
	std::map<long, recClass> m_joinLineInfo;    //!< ������H���e�[�u���i�[�p�i�L�[�FObjectID�j
	std::map<long, recClass> m_railwayLine;     //!< �S���H���e�[�u���i�[�p�i�L�[�FObjectID�j

//	std::map<CString, std::vector<CString>> m_execFields;
};

