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
* @file MapHelper.h
*
* @brief IMap�Ɋ֘A���鏈�����s��CMapHelper�̒�`�t�@�C��
*/
#if	!defined( __MAPHELPER_H__)
#define	__MAPHELPER_H__	//!< �d���C���N���[�h�h�~

#include "AheGlobals.h"
#include "AheLayerFunctions.h"
#include "AheGlobalsX.h"
#include "AheGlobalsX2.h"

class CArcHelper;

/**
* @class CMapHelper
*
* @brief IMap�Ɋ֘A���鏈�����܂Ƃ߂��N���X�B
*
* @note ��{�I�ɂ�Ahe�̃O���[�o���֐����Ăяo���Ă���
*/
class CMapHelper
{
public:
	CMapHelper( CArcHelper& cArcHelper);
	CMapHelper();
	~CMapHelper();

	/**
	* @brief ������Z�q
	*
	* @note �R�s�[���̊Ǘ����� IMap �I�u�W�F�N�g�̃C���X�^���X���R�s�[����
	*
	* @param cMapHelper [in] �R�s�[���̃N���X�I�u�W�F�N�g
	*
	* @return ���g�ւ̎Q��
	*/
	CMapHelper& operator=( CMapHelper& cMapHelper);

	/**
	* @brief ������Z�q
	*
	* @note �R�s�[����CArcHelper ���� IMap �I�u�W�F�N�g���擾���A���g������������B
	*
	* @param cArcHelper [in] �R�s�[���̃N���X�I�u�W�F�N�g
	*
	* @return ���g�ւ̎Q��
	*/
	CMapHelper& operator=( CArcHelper& cArcHelper);

	//////////////////////////////////////////////////////////////////////
	// ���[�N�X�y�[�X�Ɋւ���֐�
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief ���[�N�X�y�[�X���擾
	*
	* @note ::AheGetWorkspace ������ŌĂяo���Ă���B
	*
	* @param piUnk [in] �I�u�W�F�N�g�̃C���X�^���X
	* @param ppiWorkspace [out] �擾���ꂽ IWorkspace �̃C���X�^���X
	*
	* @see AheGetWorkspace
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetWorkspace( IUnknownPtr piUnk, IWorkspace** ppiWorkspace);
	//////////////////////////////////////////////////////////////////////
	// �e�[�u�������Ɋւ���֐�
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief ITable�C���X�^���X���擾
	*
	* @note �w�肳�ꂽ�e�[�u�����̂Ŏ擾
	* @note ::AheGetTableByTableName ������ŌĂяo���Ă���B
	*
	* @param lpszTableName [in] �e�[�u������
	* @param ppiTable [out] �擾���ꂽITable�C���X�^���X
	*
	* @see AheGetTableByTableName
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetTableByTableName( LPCTSTR lpcszTableName, ITable** ppiTable );

	/**
	* @brief IField �C���X�^���X���擾
	*
	* @note �w�肳�ꂽ�e�[�u�����̂ƃt�B�[���h���̂���擾����B
	* @note ::AheGetFieldByFieldName ������ŌĂяo���Ă���
	*
	* @param lpszTableName [in] �e�[�u������
	* @param lpszFieldName [in] �t�B�[���h����
	* @param ppiField [out] �擾���ꂽIField �C���X�^���X
	*
	* @see AheGetFieldByFieldName
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetFieldByFieldName( LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName, IField** ppiField );

	/**
	* @brief IField �C���X�^���X���擾
	*
	* @note �w�肳�ꂽ�e�[�u���̃C���X�^���X�ƃt�B�[���h���̂���擾����B
	* @note ::AheGetFieldByFieldName ������ŌĂяo���Ă���
	*
	* @param piTable [in] �e�[�u���̃C���X�^���X
	* @param lpszFieldName [in] �t�B�[���h����
	* @param ppiField [out] �擾���ꂽIField �C���X�^���X
	*
	* @see AheGetFieldByFieldName
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetFieldByFieldName( ITablePtr piTable, LPCTSTR lpcszFieldName, IField** ppiRetField );

	/**
	* @brief IDomain �C���X�^���X���擾
	*
	* @note �w�肳�ꂽ�e�[�u�����̂ƃt�B�[���h���̂���擾����B
	* @note ::AheGetDomainByFieldName ������ŌĂяo���Ă���
	*
	* @param lpszTableName [in] �e�[�u���̖���
	* @param lpszFieldName [in] �t�B�[���h����
	* @param ppiField [out] �擾���ꂽIDomain �C���X�^���X
	*
	* @see AheGetDomainByFieldName
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetDomainByFieldName( LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName, IDomain** ppiDomain );

	//////////////////////////////////////////////////////////////////////
	// ���C���擾�Ɋւ���֐�
	//////////////////////////////////////////////////////////////////////

	/**
	* @brief ���C���̃e�[�u�������� ILayer ���擾
	*
	* @param lpcszLayerName	[in]			�e�[�u������
	* @param ppiRetLayer	[out]			�擾���ꂽ���C���I�u�W�F�N�g�̃C���X�^���X
	* @param bEditableOnly	[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
	*
	* @see AheGetLayerByTableName
	* @see _FIND_TABLE
	* @see AheGetLayerByName
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetLayerByTableName( LPCTSTR lpcszLayerName, ILayer** ppiRetLayer, BOOL bEditableOnly = FALSE );

	/**
	* @brief ���C���̃e�[�u�������� ILayer ���擾
	*
	* @param lpcszLayerName [in] �e�[�u������
	* @param bEditableOnly	[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
	*
	* @see AheGetLayerByTableName
	* @see _FIND_TABLE
	* @see AheGetLayerByName
	*
	* @return ILayerPtr
	*/
	ILayerPtr GetLayerByTableName( LPCTSTR lpcszLayerName, BOOL bEditableOnly = FALSE );

	/**
	* @brief ���C���̃G�C���A�X������ ILayer ���擾
	*
	* @param lpcszLayerName [in] �G�C���A�X����
	* @param ppiRetLayer [out] �擾���ꂽ���C���I�u�W�F�N�g�̃C���X�^���X
	*
	* @see AheGetLayerByAliasName
	* @see _FIND_ALIAS
	* @see AheGetLayerByName
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetLayerByAliasName( LPCTSTR lpcszLayerName, ILayer** ppiRetLayer );

	/**
	* @brief ���C���̃G�C���A�X������ ILayer ���擾
	*
	* @param lpcszLayerName [in] �G�C���A�X����
	*
	* @see AheGetLayerByAliasName
	* @see _FIND_ALIAS
	* @see AheGetLayerByName
	*
	* @return ILayerPtr
	*/
	ILayerPtr GetLayerByAliasName( LPCTSTR lpcszLayerName );

	/**
	* @brief *.mxd�t�@�C���ɕۑ�����Ă��郌�C�������� ILayer ���擾
	*
	* @param lpcszLayerName [in] *.mxd�t�@�C��������
	* @param ppiRetLayer [out] �擾���ꂽ���C���I�u�W�F�N�g�̃C���X�^���X
	*
	* @see AheGetLayerByMXDName
	* @see _FIND_MXDNAME
	* @see AheGetLayerByName
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetLayerByMXDName( LPCTSTR lpcszLayerName, ILayer** ppiRetLayer );

	/**
	* @brief *.mxd�t�@�C���ɕۑ�����Ă��郌�C�������� ILayer ���擾
	*
	* @param lpcszLayerName [in] *.mxd�t�@�C��������
	*
	* @see AheGetLayerByMXDName
	* @see _FIND_MXDNAME
	* @see AheGetLayerByName
	*
	* @return ILayerPtr
	*/
	ILayerPtr GetLayerByMXDName( LPCTSTR lpcszLayerName );

	/**
	* @brief ���C�������� ILayer ���擾
	*
	* @param lpcszLayerName [in] ���C������
	* @param ppiRetLayer [out] �擾���ꂽ���C���I�u�W�F�N�g�̃C���X�^���X
	* @param type [in] ��r�Ώ�
	*
	* @see AheGetLayerByName
	* @see FINDTYPE
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetLayerByName( LPCTSTR lpcszLayerName, ILayer** ppiRetLayer, FINDTYPE type = _FIND_TABLE );

	/**
	* @brief ���C�������� ILayer ���擾
	*
	* @param lpcszLayerName [in] ���C������
	* @param type [in] ��r�Ώ�
	*
	* @see AheGetLayerByName
	* @see FINDTYPE
	*
	* @return ILayerPtr
	*/
	ILayerPtr GetLayerByName( LPCTSTR lpcszLayerName, FINDTYPE type = _FIND_TABLE );

	/**
	* @brief *.lyr �t�@�C������ ILayer ���擾
	*
	* @param lpcszLayerFile [in] ���C���t�@�C���p�X
	* @param ppiRetLayer [out] �擾���ꂽ���C���I�u�W�F�N�g�̃C���X�^���X
	*
	* @see AheGetLayerFromFile
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetLayerFromFile( LPCTSTR lpcszLayerFile, ILayer** ppiRetLayer );

	/**
	* @brief *.lyr �t�@�C������ ILayer ���擾
	*
	* @param lpcszLayerFile [in] ���C���t�@�C���p�X
	*
	* @see AheGetLayerFromFile
	*
	* @return ILayerPtr
	*/
	ILayerPtr GetLayerFromFile( LPCTSTR lpcszLayerFile );
	//////////////////////////////////////////////////////////////////////
	// ���C�����擾�Ɋւ���֐�
	//////////////////////////////////////////////////////////////////////

	/**
	* @brief ���C���̃e�[�u�������擾
	*
	* @param piUnk [in] �擾���Ƃ̃C���X�^���X
	* @param pbstrLayerName [out] �擾���ꂽ���C������
	*
	* @see AheGetLayerTableName
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetLayerTableName( IUnknownPtr piUnk, BSTR* pbstrLayerName);

	/**
	* @brief ���C���̃G�C���A�X�����擾
	*
	* @param piUnk [in] �擾���Ƃ̃C���X�^���X
	* @param pbstrLayerName [out] �擾���ꂽ���C������
	*
	* @see AheGetLayerAliasName
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetLayerAliasName( IUnknownPtr piUnk, BSTR* pbstrLayerName);

	/**
	* @brief *.mxd�t�@�C���ɕۑ�����Ă��郌�C�������擾
	*
	* @param piUnk [in] �擾���Ƃ̃C���X�^���X
	* @param pbstrLayerName [out] �擾���ꂽ���C������
	*
	* @see AheGetLayerMXDName
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetLayerMXDName( IUnknownPtr piUnk, BSTR* pbstrLayerName);

	/**
	* @brief ���C�������擾
	*
	* @param piUnk [in] �擾���Ƃ̃C���X�^���X
	* @param pbstrName [out] �擾���ꂽ���C������
	* @param type [in] �擾���
	*
	* @see AheGetLayerName
	* @see FINDTYPE
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetLayerName( IUnknownPtr piUnk, BSTR* pbstrName, FINDTYPE type = _FIND_TABLE );

	/**
	* @brief ���C�������擾
	*
	* @note TCHAR�֕ϊ����Ė߂�
	*
	* @param piUnk [in] �擾���Ƃ̃C���X�^���X
	* @param lpszName [out] �擾���ꂽ���C������
	* @param type [in] �擾���
	*
	* @see AheGetLayerName
	* @see FINDTYPE
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetLayerName( IUnknownPtr piUnk, LPCTSTR& lpszName, FINDTYPE type = _FIND_TABLE );

	/**
	* @brief IDataset �I�u�W�F�N�g���擾
	*
	* @note �e�[�u�������擾���邽�߂� IDataset �I�u�W�F�N�g���擾����
	*
	* @param piUnk [in] �擾���Ƃ̃C���X�^���X
	* @param ppiDataset [out] �擾���ꂽ IDataset �̃C���X�^���X
	*
	* @see AheGetDatasetForTableName
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetDatasetForTableName( IUnknownPtr piUnk, IDataset** ppiDataset );

	//////////////////////////////////////////////////////////////////////
	// �t�B�[�`���N���X�擾�Ɋւ���֐�
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief �t�B�[�`���N���X���擾
	*
	* @param piUnk [in] �t�B�[�`��
	* @param ppiFeatureClass [out] �擾���ꂽ IFeatureClass �̃C���X�^���X
	*
	* @see AheGetFeatureClass
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetFeatureClass( IUnknownPtr piUnk, IFeatureClass** ppiFeatureClass );

	/**
	* @brief ���C�����\���\�ȃX�P�[�����ǂ����̔���
	*
	* @param pGeoFeatureLayer [in] ����Ώ�
	*
	* @see AheIsVisibleScale
	*
	* @retval TURE �\����
	* @retval FALSE �\���s��
	*/
	BOOL IsVisibleScale( IGeoFeatureLayerPtr pGeoFeatureLayer);

	//////////////////////////////////////////////////////////////////////
	// �t�B�[�`���I���Ɋւ���֐�
	//////////////////////////////////////////////////////////////////////
	/**
	* @brief �w��̃G���A���ɂ���t�B�[�`���[��I������
	*
	* @param piGeom				[in]			�����͈͌`��
	* @param cTargetItems		[out]			�������ʊi�[�R���|�[�l���g
	* @param spatialRel			[in,optional]	�I�����
	* @param lpcszSubFields		[in,optional]	�擾����t�B�[���h��
	* @param lpcszWhereClause	[in,optional]	WHERE ��
	* @param bVisibleOnly		[in,optional]	�\������Ă��郌�C���݂̂̏ꍇ��TRUE
	* @param bEditableOnly		[in,optional]	�ҏW�\���C���݂̂̏ꍇ��TRUE
	*
	* @see AheSelectByShape
	*
	* @return �I�����ꂽ�Ώۂ̐�
	*/
	LONG SelectByShape( IGeometryPtr piGeom, CTargetItems& cTargetItems, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL, BOOL bVisibleOnly = FALSE, BOOL bEditableOnly = FALSE );

	/**
	* @brief �w��̃G���A���ɂ���t�B�[�`���[��I������
	*
	* @param ipGeometry			[in]			�����͈͌`��
	* @param cTargetItems		[out]			�������ʊi�[�R���|�[�l���g
	* @param piLayer			[in]			�����Ώۃ��C��
	* @param spatialRel			[in,optional]	�I�����
	* @param lpcszSubFields		[in,optional]	�擾����t�B�[���h��
	* @param lpcszWhereClause	[in,optional]	WHERE ��
	*
	* @see AheSelectByShapeFromLayer
	*
	* @return �I�����ꂽ�Ώۂ̐�
	*/
	LONG SelectByShapeFromLayer( IGeometryPtr ipGeometry, CTargetItems& cTargetItems, ILayerPtr ipLayer, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL );

	/**
	* @brief �w��̃G���A���ɂ���t�B�[�`���[��I������
	*
	* @param ipGeometry			[in]			�����͈͌`��
	* @param cTargetItems		[out]			�������ʊi�[�R���|�[�l���g
	* @param ipFeatureClass		[in]			�����Ώۃt�B�[�`���N���X
	* @param spatialRel			[in,optional]	�I�����
	* @param lpcszSubFields		[in,optional]	�擾����t�B�[���h��
	* @param lpcszWhereClause	[in,optional]	WHERE ��
	*
	* @see AheSelectByShapeFromFeatureClass
	*
	* @return �I�����ꂽ�Ώۂ̐�
	*/
	LONG SelectByShapeFromFeatureClass( IGeometryPtr ipGeometry, CTargetItems& cTargetItems, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL );

	/**
	* @brief �w��̃G���A���ɂ���t�B�[�`���[��I������
	*
	* @param ipGeom				[in]			�����͈͌`��
	* @param cTargetItems		[out]			�������ʊi�[�R���|�[�l���g
	* @param lpcszLayersName	[in]			�����Ώۃ��C����
	* @param spatialRel			[in,optional]	�I�����
	* @param lpcszSubFields		[in,optional]	�擾����t�B�[���h��
	* @param lpcszWhereClause	[in,optional]	WHERE ��
	* @param bEditableOnly		[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
	*
	* @see AheSelectByShapeFromLayers
	*
	* @return �I�����ꂽ�Ώۂ̐�
	*/
	LONG SelectByShapeFromLayers( IGeometryPtr ipGeom, CTargetItems& cTargetItems, LPCTSTR lpcszLayersName, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL, BOOL bEditableOnly = FALSE );

	/**
	* @brief �n���ꂽ�t�B�[�`���i�m�[�h����j�ɐڑ�����Ă��铹�H���R���N�V�����ɓ���ĕԂ�
	*
	* @param piFeature [in] �����Ώۂ̃m�[�h�t�B�[�`��
	* @param cTargetItems [out] �ڑ����H�R���N�V����
	*
	* @see AheGetConnectedRoad
	*
	* @return �擾�A�C�e����
	*/
	LONG GetConnectedRoad( IFeaturePtr piFeature, CTargetItems& cTargetItems);
	
	/**
	* @brief �t�B�[�`���̍\���_��ɕʂ̃I�u�W�F�N�g�̍\���_�����݂��邩���ׂ�
	*
	* @param dTol [in] �����͈�臒l
	* @param piFeature [in] �����t�B�[�`��
	* @param piOwnFeature [in] ���O�I�u�W�F�N�g
	* @param bDeleteEndNode [in] 
	* @param ppiEndNode [out]
	* @param lpcszLayernames [in]	���C������[���̂��ƁA�K�����ƑO��|�L��]
	*
	* @see AheCheckVertexPoint
	*/
	LONG CheckVertexPoint( double dTol, IFeaturePtr piFeature, IFeaturePtr piOwnFeature, BOOL bDeleteEndNode, IFeature** ppiEndNode, LPCTSTR lpcszLayerNames);


	/**
	* @brief �m�[�h���C�����擾����
	*
	* @note NODE_TABLE_NAME �̃��C�����擾
	*
	* @param bEditableOnly	[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
	*
	* @see AheGetNodeLayer
	* @see NODE_TABLE_NAME
	*/
	ILayerPtr GetNodeLayer( BOOL bEditableOnly = FALSE );

	/**
	* @brief ���H���C�����擾����
	*
	* @note ROAD_TABLE_NAME �̃��C�����擾
	*
	* @param bEditableOnly	[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
	*
	* @see AheGetRoadLayer
	* @see ROAD_TABLE_NAME
	*/
	ILayerPtr GetRoadLayer( BOOL bEditableOnly = FALSE );

	/**
	* @brief �m�[�h�̃t�B�[�`���N���X���擾����
	*
	* @note NODE_TABLE_NAME �̃t�B�[�`���N���X���擾
	*
	* @param bEditableOnly	[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
	*
	* @see AheGetNodeClass
	* @see NODE_TABLE_NAME
	*/
	IFeatureClassPtr GetNodeClass( BOOL bEditableOnly = FALSE );
	
	/**
	* @brief ���H�̃t�B�[�`���N���X���擾����
	*
	* @note ROAD_TABLE_NAME �̃��C�����擾
	*
	* @param bEditableOnly	[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
	*
	* @see AheGetRoadClass
	* @see ROAD_TABLE_NAME
	*/
	IFeatureClassPtr GetRoadClass( BOOL bEditableOnly = FALSE );
protected:
	IMapPtr m_piMap;	//!< �Ǘ��Ώ�MAP�� IMap �C���X�^���X
};

#include "maphelper.inl"

#endif	//__MAPHELPER_H__
