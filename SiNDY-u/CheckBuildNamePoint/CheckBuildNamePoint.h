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
#include "FeatureObjects.h"
#include "util.h"
#include "OutputLog.h"
#include "NameChk.h"
#include "FieldMap.h"
#include <sindy/workspace.h>

/**
 * @brief   ���C���N���X
 */
class CCheckBuildNamePoint
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CCheckBuildNamePoint( const Arguments& args ) :
		m_strBlNameDB(    (args.m_strBlNameDB).c_str() ), 
		m_strBuildingDB(  (args.m_strBuildingDB).c_str() ), 
		m_strAnnoDB(      (args.m_strAnnoDB).c_str() ), 
		m_strMeshDB(      (args.m_strMeshDB).c_str() ), 
		m_strOutput(      (args.m_strOutput).c_str() ), 
		m_strMeshList(    (args.m_strMeshList).c_str() ), 
		m_strNgBuild(     (args.m_strNgBuild).c_str() ), 
		m_strErrWord(     (args.m_strErrWord).c_str() ), 
		m_strWarnWord(    (args.m_strWarnWord).c_str() ), 
		m_strBldcOKWord(  (args.m_strBldcOKWord).c_str() ),
		m_strBldcNGWord(  (args.m_strBldcNGWord ).c_str() ),
		m_chkFixclass(     args.m_chkFixclass ),
		m_strReplaceWord( (args.m_strReplaceWord).c_str() ), 
		m_strJoinAnno(    (args.m_strJoinAnno).c_str() ),
		m_strExclude_mark((args.m_strExclude_mark).c_str() ),
		m_bufferRange(     args.m_bufferRange )
	{};

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~CCheckBuildNamePoint(){};

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
	 * @brief   �����ɕK�v�ȃ��C�����J��
	 * @param   blNameWsName   [in]  �r�����̃|�C���g���[�N�X�y�[�X��
	 * @param   buildingWsName [in]  �s�s�������[�N�X�y�[�X��
	 * @param   annoWsName     [in]  ���L���[�N�X�y�[�X��
	 * @param   meshWsName     [in]  ���b�V�����[�N�X�y�[�X��
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool OpenLayers( const CString& blNameWsName, const CString& buildingWsName, const CString& annoWsName, const CString& meshWsName )
	{
		using namespace sindy::schema;
		IWorkspacePtr ipBlNameWS, ipBuildingWS, ipAnnoWS, ipMeshWS;

		if( ! OpenWorkspace( blNameWsName, ipBlNameWS ) )
			return false;
		if( ! OpenFeatureClass( sj::buildingname_point::kTableName, ipBlNameWS, m_ipBlNameFC, m_blNameFM ) )
			return false;

		if( ! OpenWorkspace( buildingWsName, ipBuildingWS ) )
			return false;
		if( ! OpenFeatureClass( building::kTableName, ipBuildingWS, m_ipBuildingFC, m_buildingFM ) )
			return false;

		if( ! OpenWorkspace( annoWsName, ipAnnoWS ) )
			return false;
		if( ! OpenFeatureClass( city_annotation::kTableName, ipAnnoWS, m_ipCityAnnoFC, m_cityAnnoFM ) )
			return false;

		if( ! OpenWorkspace( meshWsName, ipMeshWS ) )
			return false;
		if( ! OpenFeatureClass( citymesh::kTableName, ipMeshWS, m_ipCityMeshFC, m_cityMeshFM ) )
			return false;

		return true;
	};

	/**
	 * @brief   ���[�N�X�y�[�X�I�[�v��
	 * @param   name        [in]  ���[�N�X�y�[�X��
	 * @param   ipWorkspace [out] ���[�N�X�y�[�X
	 * @retval  true  �I�[�v������
	 * @retval  false �I�[�v�����s
	 */
	bool OpenWorkspace( const CString& name, IWorkspacePtr& ipWorkspace )
	{
		ipWorkspace = sindy::create_workspace( name );
		if( !ipWorkspace ){
			std::cerr << "#ERROR Open Workspace Fail:" << CT2CA( name ) << std::endl;
			return false;
		}
		return true;
	};

	/**
	 * @brief   �t�B�[�`���N���X�I�[�v�����t�B�[���h�}�b�v�쐬
	 * @param   name           [in]  �t�B�[�`���N���X��
	 * @param   ipWorkspace    [in]  ���[�N�X�y�[�X
	 * @param   ipFeatureClass [out] �t�B�[�`���N���X
	 * @param   fieldMap       [out] �t�B�[���h�}�b�v
	 * @retval  true  �I�[�v������
	 * @retval  false �I�[�v�����s
	 */
	bool OpenFeatureClass(const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass, CFieldMap& fieldMap )
	{
		((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t( name ), &ipFeatureClass );
		if( !ipFeatureClass ){
			std::cerr << "#ERROR Open Feature Class Fail:" << CT2CA( name ) << std::endl;
			return false;
		}
		fieldMap.CreateFieldMap( ipFeatureClass->GetFields() );

		return true;
	};

	/**
	 * @brief   �e�[�u���I�[�v�����t�B�[���h�}�b�v�쐬
	 * @param   name           [in]  �e�[�u����
	 * @param   ipWorkspace    [in]  ���[�N�X�y�[�X
	 * @param   ipTable        [out] �e�[�u��
	 * @param   fieldMap       [out] �t�B�[���h�}�b�v
	 * @retval  true  �I�[�v������
	 * @retval  false �I�[�v�����s
	 */
	bool OpenTable(const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable, CFieldMap& fieldMap )
	{
		((IFeatureWorkspacePtr)ipWorkspace)->OpenTable( _bstr_t( name ), &ipTable );
		if( !ipTable ){
			std::cerr << "#ERROR Open Table Fail:" << CT2CA( name ) << std::endl;
			return false;
		}
		fieldMap.CreateFieldMap( ipTable->GetFields() );

		return true;
	};

	/**
	 * @brief   �^�������b�V���א�9���b�V�����X�g���쐬����
	 * @param   mesh     [in]  �擾���������S���b�V��
	 * @param   meshlist [out] �擾�������b�V�����X�g��set
	 */
	void Get9Mesh( long mesh, std::set<long>& meshlist );

	/**
	 * @brief   ���O�w�b�_�o��
	 */
	void OutputLogHeader();

	/**
	 * @brief   ���b�V�����X�g����A�����������b�V���|���S�����擾
	 * @param   mesh           [in]  ipMeshGeo�Ɏ�肽�����b�V���R�[�h
	 * @param   meshlist       [in]  ipUnionMeshGeo�Ɏ�肽�����b�V�����X�g
	 * @param   ipMeshGeo      [out] 1���b�V���̃��b�V���W�I���g��
	 * @param   ipUnionMeshGeo [out] �����������b�V���W�I���g��
	 * @retval  true  �擾����
	 * @retval  false �擾���s
	 */
	bool GetMeshPolygons( long mesh, const std::set<long>& meshlist, IGeometryPtr& ipMeshGeo, IGeometryPtr& ipUnionMeshGeo );

	/**
	 * @brief   �^����ꂽ���b�V���`����̃I�u�W�F�N�g���擾
	 * @param   ipMeshGeo      [in]  ���b�V���|���S���̃W�I���g��
	 * @param   fields         [in]  �l���擾�������t�B�[���h����Vec
	 * @param   ipFeatureClass [in]  �擾�������t�B�[�`���N���X
	 * @param   fieldMap       [in]  �t�B�[���h�}�b�v
	 * @param   geoMap         [out] �擾�����I�u�W�F�N�gMap(�L�[�FObjectID)
	 * @retval  true  �擾����
	 * @retval  false �擾���s
	 */
	bool GetObjInMesh( const IGeometryPtr& ipMeshGeo, const std::vector<CString>& fields, const IFeatureClassPtr& ipFeatureClass, const CFieldMap& fieldMap, std::map<long, GeoClass>& geoMap );

	/**
	 * @brief   �t�B�[���h�����̒l���擾���A�I�u�W�F�N�g�R���e�i�ɋl�߂�
	 * @param   ipFeature [in]  �擾�������t�B�[�`��
	 * @param   fields    [in]  �擾�������t�B�[���h����Vec
	 * @param   fieldMap  [in]  �t�B�[���h�}�b�v
	 * @param   obj       [out] �I�u�W�F�N�g�R���e�i
	 * @retval  true  �擾����
	 * @retval  false �擾���s
	 */
	bool GetFieldValues( const IFeaturePtr& ipFeature, const std::vector<CString>& fields, const CFieldMap& fieldMap, GeoClass& obj );

	/**
	 * @brief   R-Tree�𐶐�����
	 * @param   geoMap         [in]  �Ώۃt�B�[�`���N���X
	 * @param   rTree          [out] ���ʂ�R-Tree(�`����[r]�͎Q�Ƃ̈Ӗ��ł͂Ȃ�)
	 */
	void createRTree( const std::map<long, GeoClass>& geoMap, BGRTree& rTree );

	/**
	 * @brief   R-Tree����A�^�������͈̔͂Ɉ���������ID�Q���擾
	 * @param   box   [in]    �擾�������͈�(�G���x���[�v)
	 * @param   rTree [in]    �擾���Ă���Ώۂ�R-Tree
	 * @retval  set<long>     �Ώ۔͈͂�ID�Q
	 */
	std::set<long> getIntersectsIDs( const BGBox& box, const BGRTree& rTree );

	/**
	 * @brief   �`�F�b�N�̃��C���֐�
	 * @param   ipMeshGeo     [in]  �Y�����b�V���|���S��
	 * @param   blNameMap     [in]  �r�����̃|�C���g�̃I�u�W�F�N�g�}�b�v
	 * @param   buildingMap   [in]  �����̃I�u�W�F�N�g�}�b�v
	 * @param   cAnnoMap      [in]  �s�s���L�̃I�u�W�F�N�g�}�b�v
	 * @param   blNameRTree   [in]  �r�����̃|�C���g��RTree
	 * @param   buildingRTree [in]  ������RTree
	 * @param   cAnnoRTree    [in]  �s�s���L��RTree
	 */
	void CheckMain( const IGeometryPtr& ipMeshGeo, const std::map<long, GeoClass>& blNameMap, const std::map<long, GeoClass>& buildingMap, const std::map<long, GeoClass>& cAnnoMap, const BGRTree& blNameRTree, const BGRTree& buildingRTree, const BGRTree& cAnnoRTree );

	/**
	 * @brief   �r�����̃|�C���g�̌���ID�ƈʒu�̐����`�F�b�N
	 * @param   blNameOid     [in]  �r�����̃|�C���g��Oid
	 * @param   blNameObj     [in]  �r�����̃|�C���g�̃I�u�W�F�N�g�R���e�i
	 * @param   buildID       [in]  �r�����̃|�C���g�̌���ID
	 * @param   buildingMap   [in]  �����̃I�u�W�F�N�g�}�b�v
	 * @param   buildingRTree [in]  ������RTree
	 * @retval  true  �G���[�Ȃ�
	 * @retval  false �G���[����
	 */
	bool CheckBuildIdPos( long blNameOid, const GeoClass& blNameObj, long buildID, const std::map<long, GeoClass>& buildingMap, const BGRTree& buildingRTree );

	/**
	 * @brief   �r�����̃|�C���g�̖��̃`�F�b�N
	 * @param   blNameOid     [in]  �r�����̃|�C���g��Oid
	 * @param   blNameObj     [in]  �r�����̃|�C���g�̃I�u�W�F�N�g�R���e�i
	 */
	void CheckBlNameStr( long blNameOid, const GeoClass& blNameObj );

	/**
	* @brief   ������ʃR�[�h��OK/NG���[�h�ɂ�閼�̃`�F�b�N
	* @param   blNameObj     [in]  �r�����̃|�C���g�̃I�u�W�F�N�g�R���e�i
	* @param   blName        [in]  �r�����̃|�C���g�̖���
	* @param   judgeStr      [out] ����ɗp����������i�[�p
	* @retval  err_def::ECode �G���[���
	*/
	err_def::ECode CheckBlNameByBldcWord( const GeoClass& blNameObj, const std::string& blName, CString& judgeStr );

	/**
	* @brief  ������ʃR�[�h��NG���[�h�𖼏̂Ɏ����ǂ���
	* @param   blName        [in]  �r�����̃|�C���g�̖���
	* @param   bldClasSet    [in]  ������ʃR�[�h1~3�i�[�R���e�i
	* @param   judgeStr      [out] ����ɗp����������
	* @retval  true  ������ʃR�[�h��NG���[�h�𖼏̂Ɏ���
	* @retval  false ������ʃR�[�h��NG���[�h�𖼏̂Ɏ����Ȃ�
	*/
	bool HasBldcNGWord( const std::string& blName, const std::set<long>& bldClassSet, CString& judgeStr );

	/**
	* @brief   ������ʃR�[�h��OK���[�h�ɂ�閼�̃`�F�b�N
	* @param   blName        [in]  �r�����̃|�C���g�̖���
	* @param   bldClasSet    [in]  ������ʃR�[�h1~3�i�[�R���e�i
	* @param   judgeStr      [out] ����ɗp����������
	* @retval  err_def::ECode �G���[���
	*/
	err_def::ECode CheckBldNameByBldcOKWord( const std::string& blName, const std::set<long>& bldClassSet, CString& judgeStr );

	/**
	* @brief   ������ʃR�[�h�`�F�b�N
	           �`�F�b�N�Ώۂ̌�����ʃR�[�h�Q�ɁAOK�Ƃ��錚����ʃR�[�h���܂܂�Ă��邩�ǂ���
	* @param   chkBldcSet   [in]  �`�F�b�N�Ώۂ̌�����ʃR�[�h�Q
	* @param   okBldcVec    [in]  OK�Ƃ��錚����ʃR�[�h�Q
	* @retval  err_def::ECode �G���[���
	*/
	err_def::ECode CheckOKBldc( const std::set<long>& chkBldcSet, const std::vector<long>& okBldcVec );

	/**
	 * @brief   �r�����̃|�C���g�t�����l���`�F�b�N
	 * @param   blNameOid     [in]  �r�����̃|�C���g��Oid
	 * @param   blNameObj     [in]  �r�����̃|�C���g�̃I�u�W�F�N�g�R���e�i
	 */
	void CheckBlNameAddInfo( long blNameOid, const GeoClass& blNameObj );

	/**
	 * @brief   �r�����̃|�C���g�̏����ƌ`�̑����`�F�b�N
	 * @param   blNameOid     [in]  �r�����̃|�C���g��Oid
	 * @param   blNameObj     [in]  �r�����̃|�C���g�̃I�u�W�F�N�g�R���e�i
	 * @param   buildID       [in]  �r�����̃|�C���g�̌���ID
	 * @param   buildObj      [in]  �����̃I�u�W�F�N�g�R���e�i
	 */
	void CheckBuildClass( long blNameOid, const GeoClass& blNameObj, long buildID, const GeoClass& buildObj );

	/**
	 * @brief   ���ꌚ�����̃r�����̃|�C���g���`�F�b�N
	 * @param   blNameOid     [in]  �r�����̃|�C���g��Oid
	 * @param   blNameObj     [in]  �r�����̃|�C���g�̃I�u�W�F�N�g�R���e�i
	 * @param   buildObj      [in]  �����̃I�u�W�F�N�g�R���e�i
	 * @param   blNameMap     [in]  �r�����̃|�C���g�̃I�u�W�F�N�g�}�b�v
	 * @param   blNameRTree   [in]  �r�����̃|�C���g��RTree
	 */
	void CheckDupBlName( long blNameOid, const GeoClass& blNameObj, const GeoClass& buildObj, const std::map<long, GeoClass>& blNameMap, const BGRTree& blNameRTree );

	/**
	 * @brief   �o�b�t�@�͈͂̃r�����̃|�C���g���̃`�F�b�N
	 * @param   blNameOid     [in]  �r�����̃|�C���g��Oid
	 * @param   blNameObj     [in]  �r�����̃|�C���g�̃I�u�W�F�N�g�R���e�i
	 * @param   ipBufferGeom  [in]  �N�_�̃r�����̃|�C���g����w�苗���o�b�t�@���L�����W�I���g��
	 * @param   blNameMap     [in]  �r�����̃|�C���g�̃I�u�W�F�N�g�}�b�v
	 * @param   buildingMap   [in]  �����̃I�u�W�F�N�g�}�b�v
	 * @param   blNameRTree   [in]  �r�����̃|�C���g��RTree
	 * @param   buildingRTree [in]  ������RTree
	 */
	void CheckBufferRangeBlName( long blNameOid, const GeoClass& blNameObj, const IGeometryPtr& ipBufferGeom, const std::map<long, GeoClass>& blNameMap, const std::map<long, GeoClass>& buildingMap, const BGRTree& blNameRTree, const BGRTree& buildingRTree );

	/**
	 * @brief   ���̔�r�`�F�b�N
	 * @param   blNameOid     [in]  �r�����̃|�C���g��Oid
	 * @param   blName        [in]  �r�����̃|�C���g�̖���
	 * @param   blNameObj     [in]  �r�����̃|�C���g�̃I�u�W�F�N�g�R���e�i
	 * @param   buildObj      [in]  �����̃I�u�W�F�N�g�R���e�i
	 * @param   blNameMap     [in]  �r�����̃|�C���g�̃I�u�W�F�N�g�}�b�v
	 * @param   blNameRTree   [in]  �r�����̃|�C���g��RTree
	 */
	void CheckBufferRangeName( long blNameOid, const CString& blName, const GeoClass& blNameObj, const GeoClass& buildObj, const std::map<long, GeoClass>& blNameMap, const BGRTree& blNameRTree );

	/**
	 * @brief   ���̔�r�`�F�b�N
	 * @param   blNameOid     [in]  �r�����̃|�C���g��Oid
	 * @param   buildID       [in]  �r�����̃|�C���g�̌���ID
	 * @param   blNameObj     [in]  �r�����̃|�C���g�̃I�u�W�F�N�g�R���e�i
	 * @param   buildObj      [in]  �����̃I�u�W�F�N�g�R���e�i
	 * @param   ipBufferGeom  [in]  �N�_�̃r�����̃|�C���g����w�苗���o�b�t�@���L�����W�I���g��
	 * @param   cAnnoMap      [in]  �s�s���L�̃I�u�W�F�N�g�}�b�v
	 * @param   buildingMap   [in]  �����̃I�u�W�F�N�g�}�b�v
	 * @param   cAnnoRTree    [in]  �s�s���L��RTree
	 * @param   buildingRTree [in]  ������RTree
	 */
	void CheckBufferRangeAnno( long blNameOid, long buildID, const GeoClass& blNameObj, const GeoClass& buildObj, const IGeometryPtr& ipBufferGeom, const std::map<long, GeoClass>& cAnnoMap, const std::map<long, GeoClass>& buildingMap, const BGRTree& cAnnoRTree, const BGRTree& buildingRTree );

	/**
	 * @brief   �����r�����ƒ��L���̂���v���邩
	 * @note    �ǂ��炩��܂̏ꍇ����v�Ƃ݂Ȃ�
	 * @param   blName        [in]  �r�����̃|�C���g�̖���
	 * @param   annoObj       [in]  �s�s���L�̃I�u�W�F�N�g�R���e�i
	 * @param   bSame         [out] ���S��v������
	 * @retval  true  ��v
	 * @retval  false �s��v
	 */
	bool CompAnnoStr( const CString& blName, const GeoClass& annoObj, bool& bSame );

	/**
	 * @brief   ���g�̌������̒��L���Ɗ��S��v���ǂ�������A�G���[��ʂ��擾
	 * @param   sameBuildAnnoCnt [in]  ���g�̌������̒��L��
	 * @param   bSameName        [in]  ���S��v���ǂ���
	 * @retval  err_def::ECode �G���[���
	 */
	err_def::ECode GetExistAnnoErrDef( long sameBuildAnnoCnt, bool bSameName ){
		using namespace err_def;
		if( sameBuildAnnoCnt == 0 )
			return bSameName ? wSameAnnoOwnNo : wLikeAnnoOwnNo;
		else if( sameBuildAnnoCnt == 1 )
			return bSameName ? wSameAnnoOther : wLikeAnnoOther;
		else
			return bSameName ? wSameAnnoOthers : wLikeAnnoOthers;
	};

	/**
	 * @brief   ���g�̌������̒��L������A�G���[��ʂ��擾
	 * @param   sameBuildAnnoCnt [in]  ���g�̌������̒��L��
	 * @retval  err_def::ECode �G���[���
	 */
	err_def::ECode GetNoAnnoErrDef( long sameBuildAnnoCnt ){
		using namespace err_def;
		if( sameBuildAnnoCnt == 0 )
			return Ok; // OK�F���g�̉ƌ`�ɒ��L�Ȃ������ӂɈ�v����
		else if( sameBuildAnnoCnt == 1 )
			return wNotLikeAnnoOne;
		else
			return wNotLikeAnnoDup;
	};

	/**
	 * @brief   �w��͈͕��̃o�b�t�@���擾����
	 * @note    �o�b�t�@�͈̔͂�m_bufferRange�̒l(�P��:m)
	 * @param   ipGeometry [in] �ΏۃW�I���g��
	 * @retval  IGeometryPtr �o�b�t�@�`��
	 */
	IGeometryPtr getBufferGeometry( const IGeometryPtr& ipGeometry );

	/**
	 * @brief   �������|���S���𖄂߂�(Exterior�̂ݎ擾)
	 * @param   ipGeom [in]
	 * @retval  IGeometryPtr ���߂��`��
	 */
	IGeometryPtr getExterior( const IGeometryPtr& ipGeom );

	/**
	 * @brief   ��̕����񂪈�v�A�������͂ǂ��炩��܂��H
	 * @note    �ǂ��炩��܂̏ꍇ����v�Ƃ݂Ȃ�
	 * @param   str1  [in]  ��r������
	 * @param   str2  [in]  ��r������
	 * @param   bSame [out] ��v���ǂ���
	 * @retval  true  ��v
	 * @retval  false �s��v
	 */
	bool IsLikeName( const CString& str1, const CString& str2, bool& bSame );

	/**
	 * @brief   �u�����X�g�ɑ��݂��镶����u�����ĕԋp
	 * @param   cStr [in]  ������
	 * @retval  string �u����̕�����
	 */
	std::string ReplaceStr( const CString& cStr );

	/**
	* @brief   ���O���X�g�̋L�����폜������Ԃ̕����񃊃X�g��ԋp
	* @param   cStr [in]  �����񃊃X�g
	* @retval  CString �폜��̕����񃊃X�g
	*/
	std::vector<CString> ExcludeListedMark(const std::vector<CString>& cStr);

private:

	CString          m_strBlNameDB;          //!< �r������DB�ڑ���
	CString          m_strBuildingDB;        //!< �s�s����DB�ڑ���
	CString          m_strAnnoDB;            //!< ���LDB�ڑ���
	CString          m_strMeshDB;            //!< ���b�V��DB�ڑ���
	CString          m_strOutput;            //!< �A�E�g�v�b�g
	CString          m_strMeshList;          //!< ���b�V�����X�g
	CString          m_strNgBuild;           //!< NG������ʃ��X�g
	CString          m_strErrWord;           //!< NG�����񃊃X�g(�G���[)
	CString          m_strWarnWord;          //!< NG�����񃊃X�g(�x��)
	CString          m_strBldcOKWord;        //!< ������ʃR�[�h��OK���[�h���X�g
	CString          m_strBldcNGWord;        //!< ������ʃR�[�h��NG���[�h���X�g
	CString          m_strReplaceWord;       //!< �u�������񃊃X�g
	CString          m_strJoinAnno;          //!< �������r�p�u�����X�g
	CString          m_strExclude_mark;      //!< �������`�F�b�N�����O�L�����X�g
	double           m_bufferRange;          //!< �T���o�b�t�@(m)

	CString          m_strBlNameWorkspace;   //!< �r�����̃��[�N�X�y�[�X��
	CString          m_strBuildingWorkspace; //!< �������[�N�X�y�[�X��
	CString          m_strAnnoWorkspace;     //!< ���L���[�N�X�y�[�X��
	CString          m_strMeshWorkspace;     //!< ���b�V�����[�N�X�y�[�X��

	IFeatureClassPtr m_ipBlNameFC;           //!< �r�����̃t�B�[�`���N���X
	IFeatureClassPtr m_ipBuildingFC;         //!< �s�s�����t�B�[�`���N���X
	IFeatureClassPtr m_ipCityAnnoFC;         //!< �s�s���L�e�[�u��
	IFeatureClassPtr m_ipCityMeshFC;         //!< �s�s���b�V���t�B�[�`���N���X

	CFieldMap        m_blNameFM;             //!< �r�����̃t�B�[���h�}�b�v
	CFieldMap        m_buildingFM;           //!< �s�s�����t�B�[���h�}�b�v
	CFieldMap        m_cityAnnoFM;           //!< �s�s���L�t�B�[���h�}�b�v
	CFieldMap        m_cityMeshFM;           //!< �s�s���b�V���t�B�[���h�}�b�v

	COutPut          m_log;                  //!< ���O�Ǘ��N���X
	cNameChk         m_cNameChk;             //!< ���̃`�F�b�N�N���X
	
	std::set<long>   m_meshList;             //!< ���b�V�����X�g
	std::set<long>   m_ngBuildClass;         //!< NG������ʃR�[�h���X�g

	std::vector<std::string> m_errWordVec;     //!< NG���[�h���X�g�i�G���[�j
	std::vector<std::string> m_warnWordVec;    //!< NG���[�h���X�g�i�x���j
	std::set<std::string> m_ExcludeMarkVec; //!< �����`�F�b�N�����O�L�����X�g

	util::strKey_map         m_bldcOKWordMap;  //!< ������ʃR�[�h��OK���[�h���X�g
	util::vec_map            m_bldcNGWordMap;  //!< ������ʃR�[�h��NG���[�h���X�g
	bool                     m_chkFixclass;    //!< ������ʃR�[�h�ʃ��[�h�`�F�b�N����ʊm��R�[�h�����m�F�i0�j�̃f�[�^�ɑ΂��Ă̂ݍs�����ǂ���

	util::pair_vec           m_replaceWordVec; //!< �u���������X�g
	util::vec_map            m_joinAnnoMap;    //!< ���L��r�p�ڑ��������X�g

	IProjectedCoordinateSystemPtr m_ipPrjCoordinateSystem; //!< 19���W�n�̓��e���W�n(���b�V�����Ƃɒu��������)
};

