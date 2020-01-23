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

#include "stdafx.h"
#include "Arguments.h"
#include "Logger.h"
#include <TDC/sindylib_core/FieldMap.h>



typedef std::map<CString, IFeaturePtr> FeatureMap;
typedef std::list<IFeaturePtr> FeatureList;
typedef std::list<CString> GidList;
typedef std::set<CString> GidSet;
typedef std::map<CString, GidSet> Node2LinkMap;
typedef std::multimap<long, CString> LRBorderMap;
typedef std::map<CString, LRBorderMap> LaneBorderRelMap;
typedef std::set<long> OidSet;
typedef std::map<CString, std::map<CString, long>> Link2NodeMap;

typedef std::list<long> MeshList;
typedef std::map<long, IGeometryPtr> MeshGeoMap;

/**
 * @brief   ���C���N���X
 */
class CCorrectZTool
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	explicit CCorrectZTool( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~CCorrectZTool(){}

	/**
	 * @brief   ���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute();

private:

	/**
	 * @brief   ������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool init();

	/**
	 * @brief	�Ԑ������N����
	 * @note	���b�V�����i�ׂ�ꍇ���܂ށj�̎Ԑ������N����������o��
	 * @param	mesh			[in] ���b�V��
	 * @param	ipcLaneLinkGids	[out] iPC�����N
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool searchLaneLink( const IGeometryPtr& mesh, GidList& ipcLaneLinkGids );

	/**
	 * @brief	�֘A�e�[�u������
	 * @note	�Ԑ������N�ɕR�t�����H���E�������N���֘A�e�[�u������T���o��
	 * @param	laneLinkGids	[in] �Ԑ������N
	 * @param	borderLinkGids	[out] ���H���E�������N
	 * @param	lane2border		[out] �Ԑ������N�Ɠ��H���E�������N�̑Ή��\
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool searchRelTable( const GidList& laneLinkGids, GidList& borderLinkGids, LaneBorderRelMap& lane2border );

	/**
	 * @brief	���H���E�������N����
	 * @note	���ۂɓ��H���E�������N����������o��
	 * @param	borderLinkGids	[in] ���H���E�������N�iGID���X�g�j
	 * @param	borderLinks		[out] ���H���E�������N�i�t�B�[�`���}�b�v�j
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool searchBorderLink( const GidList& borderLinkGids, FeatureMap& borderLinks );

	/**
	 * @brief	�Ԑ������N�X�V
	 * @note	���H���E�������N���g���ĎԐ������N��Z�l���X�V����
	 * @param	laneLinkGids	[in] �Ԑ������N
	 * @param	atLaneNodeGids	[in] AT�m�[�h
	 * @param	lane2border		[in] �Ԑ������N�Ɠ��H���E�������N�̑Ή��\
	 * @param	borderLinks		[in] ���H���E�������N�i�t�B�[�`���}�b�v�j
	 * @param	meshCode		[in] ���b�V���R�[�h
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool updateLaneLink( const GidList& laneLinkGids, const GidSet& atLaneNodeGids, const LaneBorderRelMap& lane2border, const FeatureMap& borderLinks, long meshCode );

	/**
	 * @brief	NQ�e�[�u������
	 * @note	�Ԑ������N�ɕR�t���Ԑ��m�[�h���m�[�h��e�[�u������T���o��
	 * @param	laneLinkGids	[in] �Ԑ������N
	 * @param	laneNodeGids	[out] �Ԑ��m�[�h
	 * @param	node2link		[out] �Ԑ��m�[�h�ƎԐ������N�̑Ή��\
	 * @param	link2node		[out] �Ԑ������N�ƎԐ��m�[�h�̑Ή��\�i�m�[�h��SEQUENCE�t���j
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool searchNqTable( const GidList& laneLinkGids, GidList& laneNodeGids, Node2LinkMap& node2link, Link2NodeMap& link2node );

	/**
	 * @brief	�Ԑ��m�[�h����
	 * @note	���ۂɎԐ��m�[�h����������o��
	 * @param	laneNodeGids	[out] �Ԑ��m�[�h
	 * @param	atLaneNodeGids	[out] AT�m�[�h
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool searchLaneNode( const GidList& laneNodeGids, GidSet& atLaneNodeGids );

	/**
	 * @brief	�Ԑ��m�[�h�X�V
	 * @note	���H���E�������N���g���ĎԐ��m�[�h��Z�l���X�V����
	 * @param	laneNodeGids	[in] �Ԑ������N
	 * @param	node2link		[in] �Ԑ��m�[�h�ƎԐ������N�̑Ή��\
	 * @param	lane2border		[in] �Ԑ������N�Ɠ��H���E�������N�̑Ή��\
	 * @param	borderLinks		[in] ���H���E�������N�i�t�B�[�`���}�b�v�j
	 * @param	meshCode		[in] ���b�V���R�[�h
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool updateLaneNode( const GidList& laneNodeGids, const Node2LinkMap& node2link, const LaneBorderRelMap& lane2border, const FeatureMap& borderLinks, long meshCode );

	/**
	 * @brief	Z�l�X�V����
	 * @note	�����N/�m�[�h�̋��ʏ����𖳗����֐���
	 * @param	ipFeature		[in/out] �t�B�[�`���[
	 * @param	ipUpdateCur		[in/out] �J�[�\��
	 * @param	linkGids		[in] �Ԑ������N��GID
	 * @param	lane2border		[in] �Ԑ������N�Ɠ��H���E�������N�̑Ή��\
	 * @param	borderLinks		[in] ���H���E�������N�i�t�B�[�`���}�b�v�j
	 * @param	skipPointNums	[in] Z�l�X�V���X�L�b�v�������\���_�ԍ��i�Ԑ������N�X�V���̂ݎg�p�j
	 * @param	layer			[in] ���C��
	 * @param	fieldMap		[in] �t�B�[���h�}�b�v
	 * @param	meshCode		[in] ���b�V���R�[�h
	 */
	void updateRun(
		IFeaturePtr& ipFeature,
		IFeatureCursorPtr& ipUpdateCur,
		const GidSet& linkGids,
		const LaneBorderRelMap& lane2border,
		const FeatureMap& borderLinks,
		const std::set<long>& skipPointNums,
		const CString layer,
		const sindy::CFieldMap& fieldMap,
		long meshCode
	);

	/**
	 * @brief	�X�V�����m�[�h�ɕR�Â������N���m��Z�l����v���Ă��邩�m�F���A��v���Ă��Ȃ������N�̍\���_�̓m�[�h��Z�l�ŕ␳����
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool doConsistentZ();

	/**
	 * @brief	�X�V�����m�[�h���g���āA�R�Â������N���擾����
	 * @param	field		[in] �t�B�[���h
	 * @param	ids			[in] IN��Ɏg��GID�̑�
	 * @param	node2Links	[out] �m�[�h�ƃ����N�̑Ή��\
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool getNode2Links( const CString& field, const CString& ids, std::map<CString, std::map<CString, std::vector<IFeaturePtr>>>& node2Links ) const;

	/**
	 * @brief	�R�Â������N���m��Z�l����v���Ă��邩�m�F���A�s��v�ȏꍇ�̓����N���m��ڑ�������
	 * @note	�C���������ʁA�}���z�ɂȂ��Ă��܂��������N���A��Ő��`��Ԃ��邽�߂Ɋm�ۂ���
	 * @param	ipFeature		[in] �m�[�h
	 * @param	field2Links		[in] �t�B�[���h�ƃ����N
	 * @param	linkInfo		[out] �X�V���K�v�ȃ����N���
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool connectLink( const IFeaturePtr& ipFeature, const std::map<CString, std::vector<IFeaturePtr>>& field2Links, std::map<CString, std::pair<IGeometryPtr, CString>>& linkInfo );

	/**
	 * @brief	�ڑ������������N���X�V����
	 * @param	ids				[in] IN��Ɏg��GID�̑�
	 * @param	linkInfo		[in] �X�V���K�v�ȃ����N���
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool reUpdateLinkZ( const CString& ids, std::map<CString, std::pair<IGeometryPtr, CString>> linkInfo );

	/**
	 * @brief	�}���z�̃����N�ɑ΂��Đ��`��Ԃ����݂�
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool doLinerInterpolation();

	/**
	 * @brief	���`��Ԃ��������N���X�V����
	 * @param	linkGids		[in] �Ԑ������N��GID
	 * @param	linkGeomMap		[in] ���`��Ԃ��������N���
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool reUpdateLinkZ( const GidList& linkGids, const std::map<CString, IGeometryPtr>& linkGeomMap );

	/**
	 * @brief	���`��Ԃ��������N�̍\���_�ɏ������m�[�h���X�V����
	 * @param	nodeGids		[in] �Ԑ��m�[�h��GID
	 * @param	node2link		[in] �Ԑ��m�[�h�ƎԐ������N�̑Ή��\
	 * @param	link2node		[in] �Ԑ������N�ƎԐ��m�[�h�̑Ή��\�i�m�[�h��SEQUENCE�t���j
	 * @param	linkGeomMap		[in] ���`��Ԃ��������N���
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool reUpdateNodeZ( const GidList& nodeGids, const Node2LinkMap& node2link, const Link2NodeMap& link2node, const std::map<CString, IGeometryPtr>& linkGeomMap );

	/**
	 * @brief	���H���E�������N�擾
	 * @param	linkGid			[in] �Ԑ������N��GID
	 * @param	lane2border		[in] �Ԑ������N�Ɠ��H���E�������N�̑Ή��\
	 * @param	borderLinks		[in] ���H���E�������N�i�t�B�[�`���}�b�v�j
	 * @param	lrCode			[in] ���E�R�[�h
	 * @param	borders			[out] ���E�R�[�h�ɉ��������H���E�������N
	 */
	void getBorderLink( const CString& linkGid, const LaneBorderRelMap& lane2border, const FeatureMap& borderLinks, long lrCode, FeatureList& borders );

	/**
	 * @brief	Z�l�X�V����
	 * @param	ipFeature		[in/out] �t�B�[�`���[
	 * @param	ipGeo			[in] �W�I���g��
	 * @param	fieldMap		[in] �t�B�[���h�}�b�v
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool putValue( IFeaturePtr& ipFeature, const IGeometryPtr& ipGeo, const sindy::CFieldMap& fieldMap );

	/**
	 * @brief	IN��Ŏg�p���镶������쐬����
	 * @param	target			[in] �v�f�i������j
	 * @param	sep				[in] �v�f���͂�����
	 * @return	������
	 */
	std::list<CString> getString( const std::list<CString>& target, const CString& sep = _T("'") );

	/**
	 * @brief	IN��Ŏg�p���镶������쐬����
	 * @param	target			[in] �v�f�i���l�j
	 * @return	������
	 */
	std::list<CString> getString( const std::list<long>& target );



	/**
	 * @brief	���[�N�X�y�[�X�擾
	 * @param	db			[in] �ڑ���DB
	 * @param	ipWorkspace	[in] ���[�N�X�y�[�X
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool openWorkspace( const CString& db, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	�f�t�H���g�o�[�W������
	 * @param	ipWorkspace	[in] ���[�N�X�y�[�X
	 * @retval	true  �f�t�H���g
	 * @retval	false ����ȊO
	 */
	bool isDefaultVersion( const IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	���b�V�����X�g�擾
	 * @param	fileName	[in] �t�@�C��
	 * @param	meshList	[out] ���b�V�����X�g
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool getMeshList( const CString& fileName, MeshList& meshList );

	/**
	 * @brief	���b�V���|���S���擾
	 * @param	meshList	[in] ���b�V�����X�g
	 * @param	meshGeoMap	[out] ���b�V���W�I���g���}�b�v
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool getMeshPolygon( const MeshList& meshList, MeshGeoMap& meshGeoMap );

	/**
	 * @brief	�e�[�u���擾
	 * @param	ipWorkspace	[in] ���[�N�X�y�[�X
	 * @param	tableName	[in] �e�[�u����
	 * @param	fieldMap	[out] �t�B�[���h�}�b�v
	 * @return	�e�[�u��
	 */
	ITablePtr openTable( const IWorkspacePtr& ipWorkspace, const CString& tableName, sindy::CFieldMap& fieldMap );

	/**
	 * @brief	�o�[�W�����Ή����C����
	 * @param	ipFeatureClass	[in] �t�B�[�`���N���X
	 * @retval	true  �o�[�W�����Ή�
	 * @retval	false �o�[�W������Ή�
	 */
	bool isVersioned( const IFeatureClassPtr& ipFeatureClass );

	/**
	 * @brief	�ҏW�J�n
	 * @param	ipWorkspace	[in/out] ���[�N�X�y�[�X
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool startEdit( IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	�ҏW�I��
	 * @param	ipWorkspace	[in/out] ���[�N�X�y�[�X
	 * @retval	true  ����
	 * @retval	false ���s
	 */
	bool stopEdit( IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	�ҏW�j��
	 * @param	ipWorkspace	[in/out] ���[�N�X�y�[�X
	 */
	static void abortEdit( IWorkspacePtr& ipWorkspace );

private:

	Arguments m_args;									//!< ����
	IWorkspacePtr m_ipWorkspace;						//!< ���[�N�X�y�[�X
	IWorkspacePtr m_ipMeshWorkspace;					//!< ���b�V�����[�N�X�y�[�X

	IFeatureClassPtr m_ipMesh;							//!< ���b�V��
	IFeatureClassPtr m_ipLaneLink;						//!< �Ԑ������N
	IFeatureClassPtr m_ipLaneNode;						//!< �Ԑ��m�[�h
	IFeatureClassPtr m_ipBorderLink;					//!< ���H���E�������N
	ITablePtr m_ipRelLaneLinkBorderLink;				//!< �Ԑ������N�|���H���E�������N�֘A
	ITablePtr m_ipNqLaneNode;							//!< �Ԑ��m�[�h��

	sindy::CFieldMap m_cMeshFM;							//!< �t�B�[���h�}�b�v�i���b�V���j
	sindy::CFieldMap m_cLaneLinkFM;						//!< �t�B�[���h�}�b�v�i�Ԑ������N�j
	sindy::CFieldMap m_cLaneNodeFM;						//!< �t�B�[���h�}�b�v�i�Ԑ��m�[�h�j
	sindy::CFieldMap m_cBorderLinkFM;					//!< �t�B�[���h�}�b�v�i���H���E�������N�j
	sindy::CFieldMap m_cRelLaneLinkBorderLinkFM;		//!< �t�B�[���h�}�b�v�i�Ԑ������N�|���H���E�������N�֘A�j
	sindy::CFieldMap m_cNqLaneNodeFM;					//!< �t�B�[���h�}�b�v�i�Ԑ��m�[�h��j

	MeshList m_meshList;								//!< ���b�V�����X�g
	MeshGeoMap m_meshGeoMap;							//!< ���b�V���W�I���g���}�b�v

	CString m_toolName;									//!< �c�[����

	GidSet m_updatedList;								//!< �X�V�����t�B�[�`���[
	FeatureMap m_updatedNode;							//!< �X�V�����Ԑ��m�[�h�̏��
	GidSet m_steepLink;									//!< �}���z�ɂȂ����Ԑ������N
	OidSet m_fixLink;									//!< ���`��ԏ����Ōo�H�T���ΏۊO�ɂ��郊���N
};

