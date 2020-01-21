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
* @file AheGlobalsX.h
*
* @brief Arc Helper �O���[�o���֐���`�t�@�C��
*
* @note AheGlobals�̊֐��𗘗p���A��������������A���@�\�^�֐����܂Ƃ߂Ă���
*/
#if	!defined( __AHEGLOBALSX_H__)
#define	__AHEGLOBALSX_H__	//!< �d���C���N���[�h�h�~

#include "LoadQueueInfo.h"
#include "TargetFeatures.h"
#include "AheNetwork.h"
#include <sindy/schema.h>
#include <sindy/schema_draft/road_draft.h>

class CErrObjContainer;
/////////////////////////////////////////////////////////////////////////////
//
// ���H�`�F�b�N�֘A�i���H�������͉�ʂɏo�Ă�����̊܂ށj
//
/////////////////////////////////////////////////////////////////////////////
/**
 * �|�C���g�����C���Z���N�V�����̒[�_�̈�ł��邩�ǂ����̔���
 *
 *   ArcObjects �̃����[�V�����V�b�v�R���|�[�l���g���g�p����̂Ō덷�ɂ��
 *   �딻�肪����܂���
 *
 * @param ipPointGeom        [in]           �|�C���g�W�I���g��
 * @param pRoadSelection     [in]           ���C���t�B�[�`���Z���N�V����
 *
 * @retval TURE �[�_
 * @retval FALSE �[�_�ȊO
 */
BOOL AheIsEndPoints( IGeometryPtr ipPointGeom, CTargetItems& cRoadSelection );
//////////////////////////////////////////////////////////////////////
// �t�B�[�`���I���Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
/**
 * �w��̃G���A���ɑ��݂���t�B�[�`�����R���N�V�����Ɋi�[
 *
 *   ����̃t�B�[�`���N���X�̂݌�������ꍇ�ɂ� ipFeatureClass ���w�肵�܂��B
 *
 * @param ipGeom            [in]			�I��͈�
 * @param cTargetItems		[out]			�R���N�V����
 * @param spatialRel        [in,optional]	�I����@
 * @param lpcszSubFields	[in,optional]	�擾����t�B�[���h��
 * @param lpcszWhereClause	[in,optional]	WHERE ��
 * @param bVisibleOnly		[in,optional]	�����Ă��郌�C���݂̂ɂ��邩�ǂ����i�f�t�H���g�FFALSE�j
 * @param bEditableOnly		[in,optional]	�ҏW�\���C���݂̂ɂ��邩�ǂ����i�f�t�H���g�FFALSE�j
 *
 * @return �����ł����t�B�[�`���̐�
 */
LONG AheSelectByShape( IMapPtr piMap, IGeometryPtr ipGeom, CTargetItems& cTargetItems, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL, BOOL bVisibleOnly = FALSE, BOOL bEditableOnly = FALSE );

/**
 * �w��̃G���A���ɑ��݂���t�B�[�`�����R���N�V�����Ɋi�[
 *
 *   ����̃��C���̂݌�������ꍇ�ɂ� ���C�� ���w�肵�܂��B
 *
 * @param ipGeometry        [in]           �I��͈�
 * @param cTargetItems		[out]          �R���N�V����
 * @param ipLayer			[in]           ���C��
 * @param spatialRel        [in,optional]  �I����@
 * @param lpcszSubFields	[in,optional]  �擾����t�B�[���h��
 * @param lpcszWhereClause	[in,optional]  WHERE ��
 *
 * @return �I�����ꂽ�A�C�e���̌�
 */
LONG AheSelectByShapeFromLayer( IGeometryPtr ipGeom, CTargetItems& cTargetItems, ILayerPtr ipLayer, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL );

/**
 * �w��̃G���A���ɑ��݂���t�B�[�`�����R���N�V�����Ɋi�[
 *
 * @param ipGeometry        [in]           �I��͈�
 * @param cTargetItems		[out]          �R���N�V����
 * @param ipFeatureClass	[in]           �t�B�[�`���N���X
 * @param spatialRel        [in,optional]  �I����@
 * @param lpcszSubFields	[in,optional]  �擾����t�B�[���h��
 * @param lpcszWhereClause	[in,optional]  WHERE ��
 *
 * @return �I�����ꂽ�A�C�e���̌�
 */
LONG AheSelectByShapeFromFeatureClass( IGeometryPtr ipGeometry, CTargetItems& cTargetItems, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL );

/**
 * �w��̃G���A���ɑ��݂���t�B�[�`�����R���N�V�����Ɋi�[
 *
 * @param ipGeom            [in]			�I��͈�
 * @param cTargetItems		[out]			�R���N�V����
 * @param lpcszLayerNames   [in]			���C������[���̂��ƁA�K�����ƑO��|�L��]
 * @param spatialRel		[in,optional]	�I����@
 * @param lpcszSubFields	[in,optional]	�擾����t�B�[���h��
 * @param lpcszWhereClause	[in,optional]	WHERE ��
 * @param bEditableOnly		[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
 *
 * @return �I�����ꂽ�A�C�e���̌�
 */
LONG AheSelectByShapeFromLayers( IMapPtr piMap, IGeometryPtr ipGeom, CTargetItems& cTargetItems, LPCTSTR lpcszLayerNames, esriSpatialRelEnum spatialRel = esriSpatialRelIntersects, LPCTSTR lpcszSubFields = NULL, LPCTSTR lpcszWhereClause = NULL, BOOL bEditableOnly = FALSE );

//////////////////////////////////////////////////////////////////////
// �`��`�F�b�N�Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
/**
 * @brief �ۑ����Ă��悢�t�B�[�`�����ǂ����̔���
 *
 * @param ipFeature			[in]			�`�F�b�N�Ώۃt�B�[�`��
 * @param errs				[out]			�`�F�b�N����
 * @param bCheckRoad		[in,optional]	���H�����N�ƃm�[�h�̐ڑ������`�F�b�N���邩�ǂ����i�f�t�H���g�FTRUE�j
 * @param bIsCheckAngle		[in,optional]	�Z�O�����g�ڑ��p�x���`�F�b�N���邩�ǂ����i�f�t�H���g�FTRUE�j
 * @param ipRule			[in,optional]	���[���I�u�W�F�N�g�i�f�t�H���g�FNULL�j
 *
 * @retval TRUE		����
 * @retval FALSE	�ُ�
 */
BOOL AheIsSafeFeature( IFeaturePtr ipFeature, CErrObjContainer& errs, BOOL bCheckRoad = TRUE, BOOL bIsCheckAngle = TRUE, ISiNDYRule3Ptr ipRule = NULL );

/**
 * @brief �ۑ����Ă��悢���H�����N�t�B�[�`�����ǂ����̔���
 *
 * @param ipRoadFeature		[in]			�`�F�b�N�Ώۓ��H�t�B�[�`��
 * @param errs				[out]			�`�F�b�N����
 *
 * @retval TRUE		����
 * @retval FALSE	�ُ�
 */
BOOL AheIsSafeRoadForms( IFeaturePtr ipRoadFeature, CErrObjContainer& errs );

/**
 * @brief �����m�[�h�𗼒[�ɂ������N���������݃`�F�b�N
 *
 * @param ipFeatureNode1	[in]			�m�[�h�t�B�[�`��
 * @param ipFeatureNode2	[in]			�m�[�h�t�B�[�`��
 * @param ipRoadFeature		[in]			���H�����N�t�B�[�`��
 * @param errs				[out]			�`�F�b�N����
 *
 * @retval TRUE		����
 * @retval FALSE	�ُ�
 */
BOOL AheIsSafeRoadLoop( IFeaturePtr ipFeatureNode1, IFeaturePtr ipFeatureNode2, IFeaturePtr ipRoadFeature, CErrObjContainer& errs );

/**
 * @brief ����Ȍ`�󂩂ǂ����`�F�b�N
 *
 * @note �ȉ��̃`�F�b�N���s���܂��B
 * @note 
 * @note 1. �|�C���g�͏�ɃX���[
 * @note 2. �|���S���E�|�����C���ȊO�� NG
 * @note 3. �|�����C���̃��[�v�`�F�b�N
 * @note 4. �|���S���� ExteriorRing ����ł��邱�Ƃ̃`�F�b�N
 * @note 5. �˂���`�F�b�N�i��������e�p�[�g�ɕ������ă`�F�b�N�j
 * @note 6. �\���_���̃`�F�b�N
 * @note 7. �p���X�E�s�p�^�[���`�F�b�N�i�|���S���͊J�n�E�I���_���l���j
 * @note 8. ��Ȍ`��`�F�b�N�i�|���S���͊J�n�E�I���_���l���j
 *
 * @param ipGeom			[in]			�`�F�b�N�Ώی`��
 * @param lpcszOwnerName	[in]			�t�B�[�`���N���X�I�[�i�[��
 * @param lpcszClassName	[in]			�t�B�[�`���N���X��
 * @param lOID				[in]			OBJECTID
 * @param errs				[out]			�`�F�b�N����
 * @param bIsCheckAngle		[in,optional]	�Z�O�����g�p�x���`�F�b�N���邩�ǂ����i�f�t�H���g�ETRUE�j
 * @param ipRule			[in,optional]	���[���I�u�W�F�N�g
 * @param bDoSimplify		[in,optional]	�I�����Ɍ`���Simplify�������邩�ǂ����i�f�t�H���g�FTRUE�j
 * @param bCheckCross	[in,optional]	�|�����C���̎��Ȍ������`�F�b�N���邩�ǂ����i�f�t�H���g�FFALSE�j
 *
 * @retval TRUE		����
 * @retval FALSE	�ُ�
 */
BOOL AheIsSafeGeometry( IGeometryPtr ipGeom, LPCTSTR lpcszOnwerName, LPCTSTR lpcszClassName, LONG lOID, sindyeGeometryCheck* check, CString& strErrMessage, BOOL bIsCheckAngle = TRUE, ISiNDYRule3Ptr ipRule = NULL, BOOL bDoSimplify = TRUE, BOOL bCheckCross=FALSE );
BOOL AheIsSafeGeometry( IGeometryPtr ipGeom, LPCTSTR lpcszOwnerName, LPCTSTR lpcszClassName, LONG lOID, CErrObjContainer& errs, BOOL bIsCheckAngle = TRUE, ISiNDYRule3Ptr ipRule = NULL, BOOL bDoSimplify = TRUE, BOOL bCheckCross=FALSE );

/**
 * @brief ���H�t�B�[�`�����ڑ����Ă���m�[�h�t�B�[�`���ꗗ���擾
 *
 * @param ipFeature			[in]			���H�t�B�[�`��
 * @param cTargetItems		[out,retval]	�m�[�h�t�B�[�`���ꗗ
 *
 * @retval LONG		�擾�ł����m�[�h�̐�
 */
LONG AheGetConnectedNode( IFeaturePtr ipFeature, CTargetItems& cTargetItems);

/**
 * @brief �n���ꂽ�t�B�[�`���i�m�[�h����j�ɐڑ�����Ă��铹�H���R���N�V�����ɓ���ĕԂ�
 *
 * @param ipFeature				[in]			���H�m�[�h�� IFeature �̃C���X�^���X
 * @param cTargetItems			[in]			�ڑ�����Ă��铹�H�����N�̃R���N�V����
 *
 * @return �����������H�����N�̐�
 */
LONG AheGetConnectedRoad( IFeaturePtr ipFeature, CTargetItems& cTargetItems);

/**
 * @brief �I���������H�����N�̔��Α��̃����N��Ԃ��A���Α��̃m�[�h��2�����b�V�����ӏ�m�[�h���ǂ���
 *
 * @note AheIsRoadCrossMesh�ł����g���Ȃ�
 * @note 2�����b�V�����ׂ��ł��铹�H�����N���擾����
 * @note ��-----------------��-----------------------------��
 * @note �@�@�@��             ���@�@�@�@�@�@�@��              ��
 * @note  �@�@inSelectRoad  ipSelectNode�@�@ipConnectedRoad�@ipOppNode
 *
 * @param ipSelectRoad		[in]	�I���������H
 * @param ipSelectNode		[in]	�I�������m�[�h
 * @param ipConnectedRoad	[out]	�ڑ����Ă��铹�H
 * @param ipOppNode			[out]	���Α��̃m�[�h
 *
 * @retval TRUE ���Α��̃m�[�h��2�����b�V�����ӏ�m�[�h
 * @retval FALSE ���Α��̃m�[�h��2�����b�V�����ӏ�m�[�h�ȊO
 */
BOOL AheGetConnectedRoadAndNode( IFeaturePtr ipSelectRoad, IFeaturePtr ipSelectNode, IFeaturePtr &ipConnectedRoad, IFeaturePtr &ipOppNode );

/**
 * @brief �I������Ă���t�B�[�`���̒�����A��ԋ������߂��t�B�[�`����HitPoint��lPartIndex��Ԃ�
 *
 * @note HitTest���g�p����֌W��A臒l��ݒ肵�Ă��K�v����
 *		 TestGeometryHit()�Œ��_�A�ӂɊւ�炸��ԋ߂����̂��A���Ă���̂�
 *       �����Ӂ�
 *       ���ݕ\������Ă��郌�C���̂ݑΏۂł��iSelectByShape()�͔�\�����C�����Ώہj
 *
 * @param cTargetItems			[in]			�I������Ă���t�B�[�`���S
 * @param ipPoint				[in]			�e�X�g���� IPoint �̃C���X�^���X
 * @param dTorelance			[in]			����
 * @param ipRetPoint			[out]			�������� IPoint �̃C���X�^���X
 * @param ipRetLayer			[out]			�������� ILayer �̃C���X�^���X
 * @param ipRetFeature			[out]			�������� IFeature �̃C���X�^���X
 * @param lPartIndex			[out]			�q�b�g�����ӂ̃C���f�b�N�X
 * @param lVertexIndex			[out]			�q�b�g�������_�̃C���f�b�N�X
 * @param bIsHitVertex			[out]			���_�Ƀq�b�g�������H
 *
 * @retval TURE �t�B�[�`������������
 * @retval FALSE �t�B�[�`����������Ȃ�����
 */
BOOL AheGetClosestVertexPointOrPartIndex( CTargetItems& cTargetItems, IPointPtr ipPoint, DOUBLE dTorelance, IPoint** ipRetPoint, ILayer** ipRetLayer, IFeature** ipRetFeature, LONG* lPartIndex, LONG*lVertexIndex, BOOL* bIsHitVertex);


/**
 * @brief �^����ꂽ�Z���N�V�����̒������ԋ߂����_�iIPoint�j��Ԃ�
 *
 * @param cTargetItems			[in]			�I������Ă���t�B�[�`���S
 * @param ipPoint				[in]			�e�X�g���� IPoint �̃C���X�^���X
 * @param dTorelance			[in]			����
 * @param ipRetPoint			[out]			��ԋ߂� IPoint �̃C���X�^���X
 *
 * @retval TURE ���_�Ƀq�b�g
 * @retval FALSE ���_�ȊO�Ƀq�b�g
 */
BOOL AheGetNearestVertexInSelection( CTargetItems& cTargetItems, IPointPtr ipPoint, IPoint** ipRetPoint);

/**
* @brief �n���ꂽ�t�B�[�`���̍\���_��ɕʂ̃t�B�[�`�����������邩���ׂ�
*
* @note �n���ꂽ�t�B�[�`���̍\���_��ɕʂ̃t�B�[�`�����������邩��
* @note ���ׂ܂��B���̂Ƃ��A�n���ꂽ�t�B�[�`�����g�Ƃ͕ʂɈ�����J
* @note �E���g���珜�O����t�B�[�`����n�����Ƃ��o���܂��B
* @note �܂��A��T������ TRUE ��n���ƏI�[�m�[�h���������ꍇ�ɑ�U��
* @note ���ɏI�[�m�[�h�t�B�[�`�����Ԃ�܂��B
*
* @param piMap			 [in]	IMap
* @param dTol			 [in]	�t�B�[�`���I��臒l
* @param ipFeature		 [in]	�\���_���`�F�b�N����t�B�[�`��
* @param ipOwnFeature	 [in]	���O�t�B�[�`��
* @param bDeleteEndNode	 [in]	�I�[�m�[�h���`�F�b�N���邩�ǂ���
* @param ipEndNode		 [out]	�I�[�m�[�h���������ꍇ�ɕԂ�
* @param lpcszLayerNames [in]   ���C������[���̂��ƁA�K�����ƑO��|�L��]
*
* @return �\���_��ɑ��݂���t�B�[�`���̐�
*/
LONG AheCheckVertexPoint( IMapPtr piMap, double dTol, IFeaturePtr ipFeature, IFeaturePtr ipOwnFeature, BOOL bDeleteEndNode, IFeature** ipEndNode, LPCTSTR lpcszLayerNames);

/**
 * @brief	�w��̃t�B�[�`�������b�V�����E�ƌ������Ă���_��擾
 *
 * @param piMap						[in]		IMap �̃C���X�^���X
 * @param piLineFeature				[in]		���b�V�����E��
 * @param ppiPointCollection		[out]		���������_����܂ރR���N�V����
 *
 * @retval TRUE ��������_�񂠂�
 * @retval FALSE ��������_��Ȃ�
 */
BOOL AheGetPointsCrossedMesh( IMapPtr piMap, IFeaturePtr piLineFeature, IPointCollection** ppiPointCollection);


/////////////////////////////////////////////////////////////////////////////
// �m�[�h�Ɋւ���֐�
/////////////////////////////////////////////////////////////////////////////
/**
 * �m�[�h�̎�ʂ��Đݒ肷��
 *
 * @param ipNodeFeature		[in]			�m�[�h�t�B�[�`��
 * @param cRoadItems		[in]			�m�[�h�t�B�[�`���̎�ʐݒ�ɕK�v�ȓ��H�����N���܂܂��t�B�[�`���Q
 * @param bIsChanged			[out,optional]	�ύX���ꂽ���ǂ���
 * @param bSetModifyData	[in,optional]	�����ύX���������邩�ǂ����i�f�t�H���g�FFALSE�j
 *
 * @retval TRUE �m�[�h��ʍĐݒ萬��
 * @retval FALSE ��ʂ�����ł��Ȃ�����
 */
BOOL AheResetNodeAttribute( IFeaturePtr ipNodeFeature, CTargetItems& cRoadItems, BOOL* bIsChanged = NULL, BOOL bSetModifyData = FALSE );

/**
 * �������Ǝv���铹�H�m�[�h��ʂ�Ԃ�
 *
 * @note cFeatureItems �ɂ́AipNodeFeature ���ڑ����邷�ׂ̓��H�����N���܂܂�Ă��邱�Ƃ��O��ł��B
 * @note ���̑O������𖞂����Ă��Ȃ��ꍇ�A�Ԃ�l�̓��H�m�[�h��ʂ��������ݒ�ł��Ȃ��ꍇ������܂��B
 *
 * @param ipNodeFeature		[in]			���H�m�[�h��ʕύX�O�̃m�[�h
 * @param cRoadItems		[in]			ipNodeFeature �ɐڑ����铹�H�����N��S�Ċ܂ރt�B�[�`���̃��X�g
 * @param pbHasNodeAttr		[out]			���H�m�[�h�ɐڑ����铹�H�����N��2�{�ȉ��̏ꍇ�Ƀm�[�h�̌����_���̋y�ѐM���@�t���O�����Ă��邩�ǂ���
 *
 * @return sindyeNodeClass
 */
sindyeNodeClass AheGetModifyRoadNodeKind( IFeaturePtr ipNodeFeature, CTargetItems& cFeatureItems, BOOL* pbHasNodeAttr );

/**
 * �������Ǝv������s�҃m�[�h��ʂ�Ԃ�
 *
 * @note cFeatureItems �ɂ́AipNodeFeature ���ڑ����邷�ׂ̕��s�҃����N���܂܂�Ă��邱�Ƃ��O��ł��B
 * @note ���̑O������𖞂����Ă��Ȃ��ꍇ�A�Ԃ�l�̕��s�҃m�[�h��ʂ��������ݒ�ł��Ȃ��ꍇ������܂��B
 *
 * @param ipNodeFeature		[in]			���s�҃m�[�h��ʕύX�O�̃m�[�h
 * @param cRoadItems		[in]			ipNodeFeature �ɐڑ����铹�H�����N��S�Ċ܂ރt�B�[�`���̃��X�g
 *
 * @return schema�Œ�`���ꂽ�R�[�h�l
 */
sindy::schema::walk_node::node_class::ECode AheGetModifyWalkNodeKind( IFeaturePtr ipNodeFeature, CTargetItems& cFeatureItems );

/**
 * �������Ǝv����n���S�m�[�h��ʂ�Ԃ�
 *
 * @note cFeatureItems �ɂ́AipNodeFeature ���ڑ����邷�ׂĂ̒n���S�����N���܂܂�Ă��邱�Ƃ��O��ł��B
 * @note ���̑O������𖞂����Ă��Ȃ��ꍇ�A�Ԃ�l�̒n���S�m�[�h��ʂ��������ݒ�ł��Ȃ��ꍇ������܂��B
 *
 * @param ipNodeFeature		[in]			�n���S�m�[�h��ʕύX�O�̃m�[�h
 * @param cRoadItems		[in]			ipNodeFeature �ɐڑ�����n���S�����N��S�Ċ܂ރt�B�[�`���̃��X�g
 *
 * @return �n���S���
 */
sindy::schema::sj::subway_node::node_class::ECode AheGetModifySubwayNodeKind( IFeaturePtr ipNodeFeature, CTargetItems& cFeatureItems );

/**
 * �������Ǝv����S���m�[�h��ʂ�Ԃ�
 *
 * @note cFeatureItems �ɂ́AipNodeFeature ���ڑ����邷�ׂĂ̓S�������N���܂܂�Ă��邱�Ƃ��O��ł��B
 * @note ���̑O������𖞂����Ă��Ȃ��ꍇ�A�Ԃ�l�̓S���m�[�h��ʂ��������ݒ�ł��Ȃ��ꍇ������܂��B
 *
 * @param ipNodeFeature		[in]			�S���m�[�h��ʕύX�O�̃m�[�h
 * @param cRoadItems		[in]			ipNodeFeature �ɐڑ�����S�������N��S�Ċ܂ރt�B�[�`���̃��X�g
 *
 * @return �S���m�[�h���
 */
sindy::schema::sj::railway_node::node_class::ECode AheGetModifyRailwayNodeKind( IFeaturePtr ipNodeFeature, CTargetItems& cFeatureItems );

/**
 * �m�[�h�ɑ����������Ă��邩�ǂ������`�F�b�N����
 *
 * @param ipNodeFeature		[in]			�m�[�h
 * @param bCheckAll			[in,optional]	FALSE �̏ꍇ�̓`�F�b�N�������_�t���O�A�����_���́A�����_���Ȃ݂̂Ɍ���
 *
 * @note �����_�t���O�A�����_���́A�����_���ȁA�����t���O�A�����f�[�^���`�F�b�N���܂��B
 *
 * @retval TRUE ��������
 * @retval FALSE �����Ȃ�
 */
BOOL AheIsExistAttrNode( IFeaturePtr ipNodeFeature, BOOL bCheckAll = TRUE );

//////////////////////////////////////////////////////////////////////
// ���H�����N�Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
/**
 * @brief �����_�𗧑̌����ɂ���
 *
 * @param ipMap			[in]	IMap �̃C���X�^���X
 * @param ipScreen		[in]	IScreenDisplay �̃C���X�^���X
 * @param cNodeItem		[in]	�m�[�h�A�C�e��
 * @param ipPoint		[in]	�|�C���g
 * @param nMode			[in]	������ɂ��邩�ǂ���
 * @param cRetDelItems	[out]	�폜���H�A�C�e�����X�g
 *
 * @return TRUE  ����I��
 * @return FALSE �ُ�I��
 */
BOOL AheCrossingToTwoLevelCrossing(IMapPtr ipMap, IScreenDisplayPtr ipScreen, CTargetItem cNodeItem, IPointPtr ipPoint, INT nMode, CTargetItems& cRetDelItems);
BOOL AheCrossingToTwoLevelCrossing2(IMapPtr ipMap, IScreenDisplayPtr ipScreen, CTargetItem cNodeItem, IPointPtr ipPoint, INT nMode, CTargetItems& cRetDelItems);

/**
 * @brief ���H���}�[�W
 *
 * @param ipMap			[in]	IMap �̃C���X�^���X
 * @param ipScreen		[in]	IScreenDisplay �̃C���X�^���X
 * @param ipPoint		[in]	�|�C���g
 * @param cRoadItem1	[in]	���H�A�C�e���P
 * @param cRoadItem2	[in]	���H�A�C�e���Q
 * @param bDel			[in]	�\���_���폜���邩�ǂ���
 *
 * @return TRUE  ����I��
 * @return FALSE �ُ�I��
 */
BOOL AheSelectMeargeRoad(IMapPtr ipMap, IScreenDisplayPtr ipScreen, CTargetItems cRoadItems, IPointPtr ipPoint, CTargetItem cRoadItem1, CTargetItem cRoadItem2, BOOL bDel);

/**
 * @brief ���H���}�[�W
 *
 * @param ipMap			[in]	IMap �̃C���X�^���X
 * @param ipScreen		[in]	IScreenDisplay �̃C���X�^���X
 * @param ipPoint		[in]	�|�C���g
 * @param cRoadItem1	[in]	���H�A�C�e���P
 * @param cRoadItem2	[in]	���H�A�C�e���Q
 * @param bDel			[in]	�\���_���폜���邩�ǂ���
 *
 * @return TRUE  ����I��
 * @return FALSE �ُ�I��
 */
BOOL AheMeargeRoad(IMapPtr ipMap, IScreenDisplayPtr ipScreen, IPointPtr ipPoint, CTargetItem cRoadItem1, CTargetItem cRoadItem2, INT& nRetType, BOOL bDel);

/**
 * @brief ���H�\���_�폜
 *
 * @param ipScreen		[in]	IScreenDisplay �̃C���X�^���X
 * @param cSelectItem	[in]	���H�A�C�e��
 * @param ipHitPoint	[in]	�|�C���g
 *
 * @return TRUE  ����I��
 * @return FALSE �ُ�I��
 */
BOOL AheDeleteRoadVertex( IScreenDisplayPtr ipScreen, CTargetItem& cSelectItem, IPointPtr ipHitPoint);

/**
 * @brief �����N���������͌X�΂̌���
 *
 * @param ipPoint		[in]	�|�C���g
 * @param lpszLayer		[in]	�������C����
 * @param cRetItems		[out]	�����A�C�e��
 *
 * @return TRUE  ����I��
 * @return FALSE �ُ�I��
 */
BOOL AheSelectRoad_SegAttr_GradFromVertex(IMapPtr ipMap, IScreenDisplayPtr ipScreen, IPointPtr ipPoint, LPCTSTR lpszLayer, CTargetItems& cRetItems);

/**
 * @brief ���H�ɂ��郊���N�������ƌX�΂̍\���_���폜
 *
 * @param cRoadItem		[in]	�|�C���g
 * @param cSearchItems	[in]	�������C����
 *
 * @return TRUE  ����I��
 * @return FALSE �ُ�I��
 */
BOOL AheDeleteSegAttr_GradVertexWithRoad( IScreenDisplayPtr ipScreen, CTargetItems& cSearchItems, IPointPtr ipPoint);

/**
 * @brief ���C���\���_�폜
 *
 * @param cSelectItem		[in]	���C���A�C�e��
 * @param ipPoint			[in]	�|�C���g
 *
 * @return TRUE  ����I��
 * @return FALSE �ُ�I��
 */
BOOL AheDeletePolyLineVertex( IScreenDisplayPtr ipScreen, CTargetItem& cSelectItem, IPointPtr ipPoint);

/**
 * @brief �R�_�̊p�x�����߂�
 *
 * @param ipScreen		[in]	IScreenDisplay �̃C���X�^���X
 * @param cRoadItem1	[in]	���H�A�C�e���P
 * @param cRoadItem2	[in]	���H�A�C�e���Q
 * @param ipPoint		[in]	�|�C���g
 *
 * @return DOUBLE
 */
DOUBLE AheGetRoadCrossAngle( IScreenDisplayPtr ipScreen, CTargetItem cRoadItem1, CTargetItem cRoadItem2, IPointPtr ipPoint);

/**
 * @brief ���H��r
 *
 * @param cRoadItem1	[in]	��r���H�A�C�e���P
 * @param cRoadItem2	[in]	��r���H�A�C�e���Q
 *
 * @return TRUE  ��r���H�A�C�e���P�̃I�u�W�F�N�gID���傫��
 * @return FALSE ��r���H�A�C�e���Q�̃I�u�W�F�N�gID���傫��
 */
BOOL AheCompRoad(CTargetItem cRoadItem1, CTargetItem cRoadItem2);
//////////////////////////////////////////////////////////////////////
// �����N��Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
/**
 * �w��̃G���A���ɑ��݂��郍�[�h�L���[��INFO���R�[�h���擾
 *
 * @param ipRoadClass			[in]			���H�����N�N���X
 * @param ipInfoTable			[in]			�������郍�[�h�L���[�� INFO �e�[�u��
 * @param ipLQTable				[in]			�������郍�[�h�L���[�� LQ �e�[�u��
 * @param lpcszRoadRelFieldName	[in]			�������郍�[�h�L���[�p�̃t���O���́i���H�����N�̑����j
 * @param ipGeom				[in]			�I��͈́i���H�����N��I���j
 * @param lDrawMode				[in]			���[�h�L���[�̎�ށiIMultiSymbolRenderer �Ŏg�p����`�惂�[�h���g�p���邱�Ɓj
 * @param listInfoRows			[out]			�擾�ł��� INFO �̃��R�[�h
 * @param listRoads				[out]			�擾�ł������H�����N
 * @param lSequence				[in,optional]	LQ_* ����������ۂɎw�肷��V�[�P���X�ԍ��i�f�t�H���g�F-1�j
 *
 * @retval �擾�ł���INFO�̃��R�[�h��
 */
LONG AheSelectInfoRowByShape( IFeatureClassPtr ipRoadClass, ITablePtr ipInfoTable, ITablePtr ipLQTable, LPCTSTR lpcszRoadRelFieldName, IGeometryPtr ipGeom, LONG lDrawMode, std::list<ROWDEF>& listInfoRows, std::list<ROWDEF>& listRoads, LONG lSequence = -1, bool bLastLink = false );

/**
 * �w��̓��H�����NID�񂩂烍�[�h�L���[��INFO���R�[�h���擾
 *
 * @param listRoads				[in]		���H�����NID��
 * @param ipInfoTable			[in]		�������郍�[�h�L���[�� INFO �e�[�u��
 * @param ipLQTable				[in]		�������郍�[�h�L���[�� LQ �e�[�u��
 * @param lDrawMode				[in]		���[�h�L���[�̎�ށiIMultiSymbolRenderer �Ŏg�p����`�惂�[�h���g�p���邱�Ɓj
 * @param listInfoRows			[out]		�擾�ł��� INFO �̃��R�[�h
 *
 * @retval �擾�ł���INFO�̃��R�[�h��
 */
LONG AheSelectInfoRowByRoadIDs( const std::list<ROWDEF>& listRoads, ITablePtr ipInfoTable, ITablePtr ipLQTable, LONG lDrawMode, std::list<ROWDEF>& listInfoRows, bool bLastLink = false );

/**
 * �w���INFO���R�[�h���烍�[�h�L���[��LQ���R�[�h���擾
 *
 * @param listInfoRow			[in]		INFO���R�[�h
 * @param ipLQTable				[in]		�������郍�[�h�L���[�� LQ �e�[�u��
 * @param listLQRows			[out]		�擾�ł��� LQ �̃��R�[�h
 *
 * @retval �擾�ł���LQ�̃��R�[�h��
 */
LONG AheSelectLQRowByInfoRow( const ROWDEF& listInfoRow, ITablePtr ipLQTable, std::list<ROWDEF>& listRQRows );

/**
 * �w���INFO���R�[�h�񂩂烍�[�h�L���[��LQ���R�[�h���擾
 *
 * @param listInfoRows			[in]		INFO���R�[�h��
 * @param ipLQTable				[in]		�������郍�[�h�L���[�� LQ �e�[�u��
 * @param listLQRows			[out]		�擾�ł��� LQ �̃��R�[�h
 *
 * @retval �擾�ł���LQ�̃��R�[�h��
 */
LONG AheSelectLQRowByInfoRows( const std::list<ROWDEF>& listInfoRows, ITablePtr ipLQTable, std::list<ROWDEF>& listLQRows );

/**
 * �w��̓��H�����NID�񂩂烍�[�h�L���[��LQ���R�[�h���擾
 *
 * @param listRoads				[in]		���H�����NID��
 * @param ipLQTable				[in]		�������郍�[�h�L���[�� LQ �e�[�u��
 * @param lpcszRoadRelFieldName	[in]		�������郍�[�h�L���[�p�̃t���O���́i���H�����N�̑����j
 * @param lDrawMode				[in]		���[�h�L���[�̎�ށiIMultiSymbolRenderer �Ŏg�p����`�惂�[�h���g�p���邱�Ɓj
 * @param listRQRows			[out]		�擾�ł���LQ�̃��R�[�h
 * @param listInfoIDs			[out]		�擾�ł���INFO��ID��
 *
 * @retval �擾�ł���LQ�̃��R�[�h��
 */
LONG AheSelectLQRowByRoadIDs( const std::list<ROWDEF>& listRoads, ITablePtr ipLQTable, LPCTSTR lpcszRoadRelFieldName, LONG lDrawMode, std::list<ROWDEF>& listRQRows, std::list<LONG>& listInfoIDs );
LONG AheSelectLQRowByRoadIDs2( const std::list<ROWDEF>& listRoads, ITablePtr ipLQTable, LPCTSTR lpcszRoadRelFieldName, LONG lDrawMode, std::list<ROWDEF>& listRQRows, std::list<LONG>& listInfoIDs, CString& );

/**
 * �w���LQ���R�[�h�񂩂瓹�H�����N���擾
 *
 * @param ipUnk					[in]			���H�����N�̃t�B�[�`���N���X���擾�ł������
 * @param listLQRows			[in]			���H�����N���擾����̂ɕK�v��LQ���R�[�h��
 * @param listRoads				[in]			���Ɋm�ۂ��Ă��铹�H�����N�i�Ȃ��ꍇ�͋��n���Ă��������j
 * @param listLQRoads			[out]			�擾�ł������H�����N�̃t�B�[�`����
 * @param bComplete				[in,optional]	����Ȃ����H�����N���������邩�ǂ����i�f�t�H���g�͌�������j	
 *
 * @retval �擾�ł������H�����N�̐�
 */
LONG AheSelectLQRoad( IUnknownPtr ipUnk, const std::list<ROWDEF>& listLQRows, const std::list<ROWDEF>& listRoads, std::list<ROWDEF>& listLQRoads, BOOL bComplete = TRUE );

/**
 * �����N��`��p�̌`����擾����
 *
 * @param lInfoID				[in]		�`�悷�郊���N��̃��R�[�hID
 * @param listLQRows			[in]		�`�悷��̂ɕK�v��LQ���R�[�h��
 * @param listLQRoads			[in]		�`�悷��̂ɕK�v�ȓ��H�����N�t�B�[�`����
 * @param bFixFlow				[out]		�W�I���g���̕���������ł������ǂ���
 *
 * @return IGeometryPtr
 */
IGeometryPtr AheGetLQRoadGeometry( LONG lInfoID, const std::list<ROWDEF>& listLQRows, const std::list<ROWDEF>& listLQRoads, BOOL* bFixFlow );

/**
 * �����N��`��p�̌`����擾����(�}���`�����_���p�j
 *
 * @param lInfoID				[in]		�`�悷�郊���N��̃��R�[�hID
 * @param listLQRows			[in]		�`�悷��̂ɕK�v��LQ���R�[�h��
 * @param listLQRoads			[in]		�`�悷��̂ɕK�v�ȓ��H�����N�t�B�[�`����
 * @param bFixFlow				[out]		�W�I���g���̕���������ł������ǂ���
 *
 * @return IGeometryPtr
 */
IGeometryPtr AheGetLQRoadGeometry2( LONG lInfoID, const std::list<ROWDEF>& listLQRows, const std::list<ROWDEF>& listLQRoads, BOOL* bFixFlow );

/**
 * @brief �폜���郊���N���I������
 *
 * @param cSelectItems		[in]			�����N����������邽�߂̃����N�̃R���N�V����
 *
 * @retval TURE ����I��
 * @retval FALSE �ُ�I��
 */
BOOL AheSelectDeleteLQ( const CTargetItems& cSelectItem );

/**
 * @brief �����N�񌟍�
 *
 * @param cSelectItem	[in]	���H�A�C�e��
 * @param cCompItem		[in]	��r���H�A�C�e��
 *
 * @return TRUE  ����I��
 * @return FALSE �ُ�I��
 */
BOOL AheSelectDeleteLQ( const CTargetItems& cRoadItems, CTargetItem& cSelectItem, CTargetItem& cCompItem, INT nType );

/**
 * @brief �����N��폜����
 *
 * @param cSelectItem			[in]	�����N����������邽�߂̃����N
 * @param strInfoTableName		[in]	Info�e�[�u������
 * @param strLQTableName		[in]	LQ�e�[�u������
 * @param strRoadRelFieldName	[in]	�����N�t���O����
 * @param lDrawMode				[in]	���[�h�L���[�̎�ށiIMultiSymbolRenderer �Ŏg�p����`�惂�[�h���g�p���邱�Ɓj
 *
 * @retval TURE ����I��
 * @retval FALSE �ُ�I��
 */
BOOL AheDeleteLQ( CTargetItem& cSelectItem, LPCTSTR strInfoTableName, LPCTSTR strLQTableName, LPCTSTR strRoadRelFieldName, LONG lDrawMode );

/**
 * @brief �����N��폜
 *
 * @param cSelectItem			[in]	���H�A�C�e��
 * @param cCompItem				[in]	��r���H�A�C�e��
 * @param strInfoTableName		[in]	INFO�e�[�u������
 * @param strLQTableName		[in]	LQ�e�[�u������
 * @param strRoadRelFieldName	[in]	�������郍�[�h�L���[�p�̃t���O���́i���H�����N�̑����j
 * @param lDrawMode				[in]	���[�h�L���[�̎�ށiIMultiSymbolRenderer �Ŏg�p����`�惂�[�h���g�p���邱�Ɓj
 *
 * @return TRUE  ����I��
 * @return FALSE �ُ�I��
 */
BOOL AheDeleteLQ( const CTargetItems& cRoadItems, CTargetItem& cSelectItem, CTargetItem& cCompItem, LPCTSTR strInfoTableName, LPCTSTR strLQTableName, LPCTSTR strRoadRelFieldName, LONG lDrawMode, INT nType );

/**
 * @brief �����N��폜���b�Z�[�W
 *
 * @param cRoadItems	[in]	���H�A�C�e�����X�g
 *
 * @return TRUE  ����I��
 * @return FALSE �ُ�I��
 */
BOOL AheDeleteLQMsg( const CTargetItems& cRoadItems );

/**
 * @brief �����N�񑶍݃`�F�b�N
 *
 * @param ipMap					[in]	IMap �̃C���X�^���X
 * @param ipInfoTable			[in]	INFO�e�[�u��
 * @param ipLQTable				[in]	LQ�e�[�u��
 * @param strRoadRelFieldName	[in]	�������郍�[�h�L���[�p�̃t���O���́i���H�����N�̑����j
 * @param lDrawMode				[in]	���[�h�L���[�̎�ށiIMultiSymbolRenderer �Ŏg�p����`�惂�[�h���g�p���邱�Ɓj
 *
 * @return TRUE  ����I��
 * @return FALSE �ُ�I��
 */
BOOL AheCheckLQ(CTargetItem cSelectItem, ITablePtr ipInfoTable, ITablePtr ipLQTable, CString strRoadRelFieldName, LONG lDrawMode);

/**
 * @brief ���H�����N��� VICS ��t�^�ł��邩�ǂ����H�i����ʍs�Ɉᔽ���Ă��Ȃ����H�j
 *
 * @param CtargetItems	[in]	�Ώۓ��H�����N
 *
 * @return TRUE  �t�^�\
 * @return FALSE �t�^�s�\�i�����̈���ʍs�Ƌt�j
 */
BOOL AheCheckOneWay(CTargetItems);

/**
 * @brief �����N�񂪍쐬�\���H
 *		  VICS�A�A�N�Z�X���̂悤�ȏd�����֎~���Ă�����̂̃`�F�b�N
 *
 * @param ipMap			[in]
 * @param CtargetItems	[in]	�Ώۓ��H�����N
 * @param LQType		[in]	LQ �̎��
 * @param nFromNodeID	[in]	
 *
 * @return TRUE  �t�^�\
 * @return FALSE �t�^�s�\�i���� LQ ������j
 */
BOOL AheCheckLQOverlap(IMapPtr ipMap, CTargetItems cItems, sindyeRoadQueueType LQType, INT nFromNodeID = 0 );

/**
 * @brief ���H�����N��� VICS ��t�^�ł��邩�ǂ����H
 *		  �o�H���̃`�F�b�N	
 *
 * @param CtargetItems	[in]	�Ώۓ��H�����N
 *
 * @return TRUE  �t�^�\
 * @return FALSE �t�^�s�\�i�o�H�O�̓��H���܂܂�Ă���j
 */
BOOL AheCheckVics(CTargetItems cItems);

//////////////////////////////////////////////////////////////////////
// ���[���Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
/**
 * @brief �t�B�[�`���I��臒l��Ԃ�
 *
 * @note ISiNDYRuleObj::GetSelectTolerance() �̃��b�p�[
 *
 * @param ipRule			[in,optional]	ISiNDYRulePtr
 *
 * @return INT
 */
INT AheGetTolerance( ISiNDYRulePtr ipRule = NULL );

/**
 * �t�B�[�`�����ҏW�\���ǂ����`�F�b�N����
 *
 * @note �����C���ɏ������Ă��邩�ǂ����`�F�b�N���āA�����Ă��Ȃ����FALSE��Ԃ��܂��B
 * @note �����C���ɑ����Ă���̂Ȃ�AheIsEditableFeature�̌��ʂ�Ԃ��܂��B
 *
 * @param ipMap						[in]			Map
 * @param ipFature					[in]			�`�F�b�N�Ώۃt�B�[�`��
 * @param ipGeom					[in]			�`�F�b�N�Ώی`��iNULL�̎��̓t�B�[�`���̏ꍇ�Ɍ���ipFeature����擾�j
 * @param type						[in]			sindyeEditType
 * @param ipRule					[in,optional]	ISiNDYRulePtr
 * @param bNotEditableIsNotShowDlg	[in,optional]	�ҏW�s�̎��Ƀ_�C�A���O���o���Ȃ����ǂ����i�f�t�H���g�FFALSE�j
 * @param hWnd						[in,optional]	���b�Z�[�W�{�b�N�X�̐e�n���h���i�f�t�H���g�FNULL�j
 *
 * @retval TRUE �ҏW�\
 * @retval FALSE �ҏW�s�\
 */
BOOL AheIsEditableFeature2( IMap* ipMap, _IRow* ipFeature, IGeometry* ipGeom, sindyeEditType type, ISiNDYRule* ipRule = NULL, BOOL bNotEditableIsNotShowDlg = FALSE, HWND hWnd = NULL );

/**
 * �t�B�[�`�����ҏW�\���ǂ����`�F�b�N����
 *
 * @note ISiNDYRuleObj::IsEditableFeature() �����b�v���āATRUE �� FALSE ��Ԃ��܂��B
 *
 * @param ipFature					[in]			�`�F�b�N�Ώۃt�B�[�`��
 * @param ipGeom					[in]			�`�F�b�N�Ώی`��iNULL�̎��̓t�B�[�`���̏ꍇ�Ɍ���ipFeature����擾�j
 * @param type						[in]			sindyeEditType
 * @param ipRule					[in,optional]	ISiNDYRulePtr
 * @param bNotEditableIsNotShowDlg	[in,optional]	�ҏW�s�̎��Ƀ_�C�A���O���o���Ȃ����ǂ����i�f�t�H���g�FFALSE�j
 * @param hWnd						[in,optional]	���b�Z�[�W�{�b�N�X�̐e�n���h���i�f�t�H���g�FNULL�j
 *
 * @retval TRUE �ҏW�\
 * @retval FALSE �ҏW�s�\
 */
BOOL AheIsEditableFeature( _IRowPtr ipFeature, IGeometryPtr ipGeom, sindyeEditType type, ISiNDYRulePtr ipRule = NULL, BOOL bNotEditableIsNotShowDlg = FALSE, HWND hWnd = NULL );

/**
 * @brief �쐬�\���C�����ǂ���
 *
 * @note �����N��̏ꍇ��INF�̃e�[�u����n������
 *
 * @param ipTable					[in]			�`�F�b�N����e�[�u��
 * @param ipCreateArea				[in]			�쐬����\��̌`��
 * @param bNotEditableIsNotShowDlg	[in,optional]	�ҏW�s�̎��Ƀ_�C�A���O���o�����ǂ����i�f�t�H���g�FFALSE�j
 * @param hWnd						[in,optional]	���b�Z�[�W�{�b�N�X�̐e�n���h���i�f�t�H���g�FNULL�j
 *
 * @retval TRUE		�쐬�\
 * @retval FALSE	�쐬�s�\
 */
BOOL AheIsCreatableLayer( ITablePtr ipTable, IGeometryPtr ipCreateArea, BOOL bNotEditableIsNotShowDlg = FALSE, HWND hWnd = NULL );

/**
 * @brief �폜�\���C�����ǂ���
 *
 * @note ���b�N���b�V���A�G�f�B�b�g���b�V�����l���ɓ���Ă��Ȃ��̂Œ��ӁI�I
 *
 * @param lpcszLayerName	[in]			�`�F�b�N���郌�C����
 * @param ipRule			[in,optional]	ISiNDYRulePtr
 *
 * @retval TRUE �폜�\
 * @retval FALSE �폜�s�\
 */
BOOL AheIsDeletableLayer( LPCTSTR lpcszLayerName, ISiNDYRulePtr ipRule = NULL );

/**
 * @brief �����ҏW�\���C�����ǂ���
 *
 * @note ���b�N���b�V���A�G�f�B�b�g���b�V�����l���ɓ���Ă��Ȃ��̂Œ��ӁI�I
 *
 * @param lpcszLayerName	[in]			�`�F�b�N���郌�C����
 * @param ipRule			[in,optional]	ISiNDYRulePtr
 *
 * @retval TRUE �����ҏW�\
 * @retval FALSE �����ҏW�s�\
 */
BOOL AheIsAttrEditableLayer( LPCTSTR lpcszLayerName, ISiNDYRulePtr ipRule = NULL );

/**
 *
 * @brief 2�����b�V�����ׂ��ł��铹�H�����N���擾����
 *
 * @param ipMap			[in]	IMap �̃C���X�^���X
 * @param ipFeature		[in]	�I���������H�t�B�[�`��
 * @param cRoadCrossMesh[out]	2�����b�V�����ׂ��ł��铹�H�t�B�[�`��
 * 
 * retval TRUE �I���������H�t�B�[�`����2�����b�V���Ɛڂ��Ă���
 * retval FALSE �I���������H�t�B�[�`����2�����b�V���Ɛڂ��Ă��Ȃ�
 */
BOOL AheIsRoadCrossMesh(IMapPtr ipMap, IFeaturePtr ipFeature, CTargetItems &cRoadCrossMesh);

/**
 *
 * @brief 2�����b�V�����ׂ��ł���t�B�[�`�����擾����
 *
 * ���̂Ƃ���A�X�΂ƃ����N�����������̔���
 *
 * @param ipFeature		[in]	�I�������t�B�[�`��
 * @param cRoadCrossMesh[out]	2�����b�V�����ׂ��ł���t�B�[�`��
 * 
 * retval TRUE �I�������t�B�[�`����2�����b�V���Ɛڂ��Ă���
 * retval FALSE �I�������t�B�[�`����2�����b�V���Ɛڂ��Ă��Ȃ�
 */
BOOL AheIsFeatureCrossMesh(IMapPtr ipMap, IFeaturePtr ipFeature, CTargetItems &cFeatureCrossMesh);


#endif	//__AHEGLOBALSX_H__
