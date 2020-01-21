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

// EditHelper.h: CEditHelper �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITHELPER_H__6456CB1F_0C72_4A48_95B7_12F5D5AEE546__INCLUDED_)
#define AFX_EDITHELPER_H__6456CB1F_0C72_4A48_95B7_12F5D5AEE546__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _USE_SETLQATTRIBUTE

#include "MapHelper.h"
#include "ArcHelper.h"

//class CTargetItems;
class CErrObjContainer;

struct LQDEF {
	_IRowPtr obj;
	long oid;
	long linkid;
	long seq;
	bool last;
	long dir;	// 1:������ 2:�t����
	sindyeRoadQueueType kind;
	long infid;
};

struct ROADDEF {
	long oid;
	long rf;
	IGeometryPtr shape;
};

class CEditHelper : public CMapHelper, public CArcHelper
{
public:
	struct LQRFINFO
	{
		long oid;
		std::map<sindyeRoadQueueType,bool> LQRF;
	};

public:
	CEditHelper();
	virtual ~CEditHelper();

	struct LongPair { LONG lID1; LONG lID2; };

	CEditHelper( CArcHelper& cHelper ){ *this = cHelper; m_piMap = cHelper.GetMap(); };

	/**
	* @brief ������Z�q
	*
	* @note �R�s�[����CArcHelper ���� IMap �I�u�W�F�N�g���擾���A���g������������B
	*
	* @param cArcHelper [in] �R�s�[���̃N���X�I�u�W�F�N�g
	*
	* @return ���g�ւ̎Q��
	*/
	CEditHelper& operator=( CArcHelper& cHelper ){ ((CArcHelper&)*this) = cHelper; m_piMap = cHelper.GetMap(); return *this; };

	/**
	* @brief �Ǘ��t�B�[�`�������b�V�����E�Ɠ��H�ŃX�v���b�g����
	*
	* @warning �֐�����߂�����A �ҏW��j������E���Ȃ��Ɋւ�炸cTargetFeatures �y�� cNewFeatures �͕K�� Store() ����K�v������܂��B
	*
	* @param cTartgetItems		[in,out]		�X�v���b�g�Ώۂ��܂ލX�V�t�B�[�`���Q
	* @param cNewFeatures		[out]			�V�K�ɍ쐬���ꂽ�t�B�[�`���Q
	* @param bSplitAll			[in]			������������ꍇ�� TRUE
	* @param pReferenceNodes	[out,optional]	�擾�����m�[�h�Q
	* @param ipSplitPoints		[in,optional]   �����_�Q
	* 
	* @note ipSplitPoints���w�肵���ꍇ�A���b�V�����E�������Ă���ȊO�̏ꏊ�ŕ�������Ȃ��Ȃ�܂��B
	* ���R�AbSplitAll�̃t���O�����Ӗ��Ȃ��̂ɂȂ�܂��ibug 4882�j�B
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL SplitRoadByRoadAndMesh( CTargetItems& cTargetFeatures, CTargetItems& cNewFeatures, BOOL bSplitAll, CErrObjContainer& errs, IPointPtr ipTakeOverPoint = NULL, CTargetItems* pReferenceNodes = NULL, IPointCollection* ipSplitPoints = NULL );
	BOOL SplitRoadByRoadAndMesh( CTargetItems& cTargetFeatures, CTargetItems& cNewFeatures, BOOL bSplitAll, IPointPtr ipTakeOverPoint = NULL, CTargetItems* pReferenceNodes = NULL, IPointCollection* ipSplitPoints = NULL );

	/**
	* @brief �Ǘ��t�B�[�`�����W�I���g���ŃX�v���b�g����
	* 
	* @warning �֐�����߂�����A�ҏW��j������E���Ȃ��Ɋւ�炸 cTargetFeatures �y�� cNewFeatures �͕K�� Store() ����K�v������܂��B
	*
	* @param cTartgetItems				[in,out]		�X�v���b�g�Ώۂ��܂ލX�V�t�B�[�`���Q
	* @param lpcszSplitLayers			[in]			�X�v���b�g�Ώۃ��C���i�u|�v�Z�p���[�g�j
	* @param ipGeom						[in]			�X�v���b�g���邽�߂̃W�I���g��
	* @param cNoSetModifyDataFeatures	[in,out]		�ҏW�������������Ȃ��t�B�[�`���Q
	* @param cNewFeatures				[out]			�V�K�쐬���ꂽ�t�B�[�`���Q
	* @param ipTakeOverPoint			[in,optional]	�X�v���b�g��Ɍ���OID�������p���t�B�[�`�������߂邽�߂̃|�C���g�i���̃|�C���g�ɋ߂��t�B�[�`���������p���j
	* @param iSplitMode					[in,optional]	�X�v���b�g���[�h�i0=Cross 1=Touch 2=Both, �f�t�H���g�F2�j
	* @param pReferenceNodes			[out,optional]	�擾�����m�[�h�Q
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/ 
	BOOL SplitFeaturesByGeometry( CTargetItems& cTargetFeatures, LPCTSTR lpcszSplitLayers, IGeometryPtr ipGeom, CTargetItems& cNoSetModifyDataFeatures, CTargetItems& cNewFeatures, IPointPtr ipTakeOverPoint = NULL, INT iSplitMode = 2, CTargetItems* pReferenceNodes = NULL );

	/**
	* @brief �W�I���g�����W�I���g���ŃX�v���b�g����
	*
	* @param ipBaseGeometry				[in]			�X�v���b�g����W�I���g��
	* @param ipSplitterGeometry			[in]			�X�v���b�g���邽�߂̃W�I���g��
	* @param iSplitMode					[in,optional]	�X�v���b�g���[�h�i0=Cross 1=Touch 2=Both, �f�t�H���g�F2�j
	* @param bIsRoadFeature				[in,optional]	���H�t�B�[�`�����ǂ����i�f�t�H���g�FFALSE�j
	* @param ipSplittedPoints			[in,optional]	bIsRoadFeature �� TRUE �̎��ɂ͂���ɃX�v���b�g�|�C���g������i���H�p�j
	* @param bTest						[in,optional]	�e�X�g�p�B�P���ɐ؂�邩�ǂ����m�F���邾���̂Ƃ��̓��b�Z�[�W�{�b�N�X��}�����邽�߂�TRUE��ݒ肷�邱�Ɓi�f�t�H���g:FALSE�j
	*
	* @retval _ISetPtr	�X�v���b�g��̌`��iIGeometryPtr�j
	*/
	_ISetPtr SplitGeometryByGeometry( IGeometryPtr ipBaseGeometry, IGeometryPtr ipSplitterGeometry, INT iSplitMode = 2, BOOL bIsRoadFeature = FALSE, IGeometryPtr ipSplittedPoints = NULL, BOOL bTest = FALSE );

	/**
	 * @brief �|�����C�����|�C���g�Q�ŃX�v���b�g����
	 *
	 * @param ipPolyline				[in]			�X�v���b�g����|�����C��
	 * @param ipSplitterPoints			[in]			�X�v���b�g���邽�߂̃|�C���g�Q
	 * @param ipSplittedPoints			[in,optional]	���ۂɃX�v���b�g���ꂽ�|�C���g���i�[�����
	 *
	 * @retval _ISetPtr	�X�v���b�g��̌`��iIGeometryPtr�j
	 */
	_ISetPtr SplitPolylineByPoints( IPolylinePtr ipPolyline, IGeometryPtr ipSplitterPoints, IPointCollectionPtr ipSplittedPoints = NULL );

	/**
	 * @brief �|���S�����|�����C���ŃX�v���b�g����
	 *
	 * @param ipPolygon					[in]			�X�v���b�g����|���S��
	 * @param ipSplitterPolyline		[in]			�X�v���b�g���邽�߂̃|�����C��
	 * @param bTest						[in,optional]	�e�X�g�p�B�P���ɐ؂�邩�ǂ����m�F���邾���̂Ƃ��̓��b�Z�[�W�{�b�N�X��}�����邽�߂�TRUE��ݒ肷�邱�Ɓi�f�t�H���g:FALSE�j
	 *
	 * @retval _ISetPtr
	 */
	_ISetPtr SplitPolygonByPolyline( IPolygonPtr ipPolygon, IPolylinePtr ipSplitterPolyline, BOOL bTest = FALSE );

#ifdef _USE_SETLQATTRIBUTE
	/**
	 * @brief	�������ꂽ���H�ɑΉ�����悤�� LQ �̏����C������
	 *
	 * @param lFeatureID		[in]	���̓��H�����N ID
	 * @param ipBeforeSplitGeom	[in]	���̓��H�����N�̈ړ���A�X�v���b�g�O�̌`��
	 * @param cNewRoads			[in]	������̓��H�����N�t�B�[�`��
	 *
	 * @return ����I�������� TRUE
	 */
	BOOL SetLQAttribute( LONG lFeatureID, IGeometryPtr ipBeforeSplitGeom, CTargetItems& cNewRoads, std::list<LQRFINFO>& listLQRFInfo, std::list<ROWDEF>& listLQRows );
#else
	/**
	 * @breif <b>�������ꂽ���H�����N�̂��߂̐V����LQ���쐬����</b>\n
	 *�@��̃����N��ɑ΂��Ĉ�x�ɕ����񕪊����s����ꍇ���l�����āA
	 * �V�[�P���X�A�ŏI�t���O�A�����R�[�h�͈�ؐ��������܂���B
	 * �������킹�͌Ăяo�����ōs���K�v������܂��B
	 * �������킹�̕��@�Ɋւ��ẮASplitFeaturesByGeometry�̍Ō�̕����Q�l��
	 * ���Ă��������B
	 * cRoad					[in]	ID�������p�������H�����N
	 * cNewRoads				[in]	�V�����쐬���ꂽ���H�����N
	 * listLQRows				[out]	�V�����쐬���ꂽLQ�̃��X�g
	 * @retval TRUE ����I��
	 * @retval FALSE ���s
	 */
	BOOL CEditHelper::CreateLQForSplittedRoad( CTargetItem& cRoad, CTargetItems& cNewRoads, std::list<ROWDEF>& listLQRows );
#endif // ifdef _USE_SETLQATTRIBUTE

	/**
	 * @brief <b>�V�[�P���X�̐��������C������</b>\n
	 * SetLQAttribute���o�O�o�O�Ȃ̂ŁA���̊֐���INF���̃V�[�P���X��
	 * �C�����܂��B
	 * @param listLQ			[in]	�C���Ώ�LQ�̃��X�g
	 * @param mapRoad			[in]	�֘A���铹�H�����N�̃��X�g�i�L�[�FOBJECTID�j
	 * @retval TRUE �C������
	 * @retval FALSE �C�����s
	 */
	static BOOL FixLQSequence( std::list<LQDEF>& listLQ, std::map<long, ROADDEF>& mapRoad );

	/**
	 * @brief �������ꂽNW�t�B�[�`���̊֘A�e�[�u�����폜
	 * @note  CRuleRelationTable�Ŋ֘A�t���O�t�B�[���h����łȂ����̂ɂ��ẮA�֘A�t���O�������Ă����ꍇ�̂ݍ폜����
	 *
	 * @param ipFeature				[in]	�폜����NW�t�B�[�`��
	 *
	 * @retval TRUE  �֘A�e�[�u���̍폜�ɐ���
	 * @retval FALSE �֘A�e�[�u���̍폜�Ɏ��s
	 */
	BOOL DeleteRelationTables( IFeaturePtr ipFeature ) const;

	/**
	 * @brief �������ꂽNW�t�B�[�`���̊֘A�e�[�u���̓��e���R�s�[
	 * @note  CRuleRelationTable�Ŋ֘A�t���O�t�B�[���h����łȂ����̂ɂ��ẮA�֘A�t���O�������Ă����ꍇ�̂݃R�s�[����
	 *
	 * @param ipFeature				[in]	�R�s�[����NW�����N
	 * @param cSplittedRoadFeatures	[in]	�������ꂽNW�����N
	 *
	 * @retval TRUE  �֘A�e�[�u���̃R�s�[�ɐ���
	 * @retval FALSE �֘A�e�[�u���̃R�s�[�Ɏ��s
	 */
	BOOL CopyRelationAttribute( IFeaturePtr ipFeature, CTargetItems& cSplittedRoadFeatures );

	/**
	 * @brief �������邢�͐ڐG���Ă���|�C���g���擾����
	 *
	 * @param cDstItems			[in]			��r��t�B�[�`���Q
	 * @param cSrcItems			[in]			��r���t�B�[�`���Q
	 * @param iMode				[in,optional]	���胂�[�h�i0=Cross 1=Touch 2=Both, �f�t�H���g�F2�j
	 * @param pNoRelItems		[out,optional]	��r��t�B�[�`���Q�̒��Ō�_�E�ړ_��������Ȃ��������̂�����
	 *
	 * @return IPointCollectionPtr
	 */
	IPointCollectionPtr GetCrossOrTouchPoints( CTargetItems& cDstItems, CTargetItems& cSrcItems, INT iMode = 2, CTargetItems* pNoRelItems = NULL );

	/**
	* @brief �؂蔲���� XOR ���s�������ƂŁA�}���`�|���S���������I�u�W�F�N�g�𕪊�����
	*
	* @param ILayerPtr			[in]	�Ώۂ̃��C��
	* @param CTargetItems&		[in]	���̃}���`�|���S��
	* @param CTargetItems&		[in]	������̃|���S���R���N�V����
	*
	* @retval TRUE		�����͐���
	* @retval FALSE		�������ɃG���[������
	**/
	BOOL SplitMultiPolygonToSimplePolygons(ILayerPtr pILayer, CTargetItems& cSource, CTargetItems& cDest);
	
	/**
	* @brief Ring �� IPolygonPtr2 �̓����ɂ���ꍇ�́AInterior �Ƃ��Ēǉ�����
	*
	* @param IPolygon2Ptr				[in/out]	�`�F�b�N�Ώۂ̃|���S��
	* @param IRingPtr					[in/out]	�`�F�b�N���� Ring
	* @param IGeometryCollectionPtr		[out]		�ǉ����� Ring �̃W�I���g��
	**/
	void AddInterior(IPolygon2Ptr, IRingPtr, IGeometryCollectionPtr);

	/**
	* @brief �|���S���̏d�Ȃ���`�F�b�N����i�d�Ȃ��Ă����ꍇ�͐؂蔲���j
	*
	* @param cTargetItems [in]	�`�F�b�N����Ώ�
	* @param cModifyItems [out]	�؂蔲���ꂽ�I�u�W�F�N�g
	* @param bAskOverlap  [in]	�d�Ȃ�̉����q�˂邩�ǂ���
	*
	* @retval TRUE	�����͐���
	* @retval FALSE �G���[������
	**/
	BOOL CutOutPolygonByPolygon( CTargetItems& cTargetItems, CTargetItems& cModifyItems, bool bAskOverlap=true );

	/**
	* @brief �N���b�N���ꂽ�_���܂ޓ��ڃ|���S�����쐬����
	*
	* @param ILayerPtr		[in]	�쐬�Ώۂ̃��C��
	* @param IPointPtr		[in]	�N���b�N���ꂽ�_
	* @param IGeometry**	[out]	�쐬���ꂽ�W�I���g��
	*
	* @note	�Q�ƃJ�E���g��ύX����K�v�Ȃ�
	*
	* @retval > 0		���ڃ|���S������i�쐬�j
	* @retval = 0		���ڃ|���S���Ȃ�
	* @retval < 0		�G���[����
	*/
	int CreateInteriorPolygon(ILayer*, IPoint*, IGeometry**);

	// GetCrossOrTouchPoints�Ɠ����������ʂȏ������Ȃ���������
	IPointCollectionPtr GetCrossOrTouchPoints2( CTargetItems& cDstItems, CTargetItems& cSrcItems, INT iMode = 2, CTargetItems* pNoRelItems = NULL );

	// GetCrossOrTouchPoints�Ɠ����������ʂȏ������Ȃ���������
	IGeometryPtr GetCrossOrTouchPoints2( IGeometryPtr ipBaseGeometry, IGeometryPtr ipTestGeometry, INT iMode = 2 );

	// AhePointIsOnGeometry�Ɠ������������͍����Ȃ͂�
	BOOL PointIsOnGeometry( IGeometryPtr ipBaseGeometry, IPointPtr ipTestPoint, ISpatialReferencePtr ipSpRef = NULL, BOOL bExeptVertex = FALSE );

private:
	/**
	 * @brief �Ώۃt�B�[�`���Q���X�v���b�g�������ʂ𓾂�
	 * @note  �\���̃`�F�b�N�Ȃǂ͍s���Ă��Ȃ�
	 * @param lpcszSplitLayers         [in]			 �X�v���b�g�Ώۃ��C���i�u|�v�Z�p���[�g�j
	 * @param ipGeom                   [in]           �X�v���b�g���邽�߂̃W�I���g��
	 * @param cNoSetModifyDataFeatures [in]           �ҏW�������������Ȃ��t�B�[�`���Q
	 * @param cModifyNodeKindFeatures  [in,out]       �m�[�h��ʂ��Đݒ肵�����̂ŁA���� cNewFeatures �ɂ� cTargetFeatures �ɂ����݂��Ȃ�����
	 * @param ipTakeOverPoint          [in,optional]  �X�v���b�g��Ɍ���OID�������p���t�B�[�`�������߂邽�߂̃|�C���g�i���̃|�C���g�ɋ߂��t�B�[�`���������p���j
	 * @param iSplitMode               [in,optional]  �X�v���b�g���[�h�i0=Cross 1=Touch 2=Both, �f�t�H���g�F2�j
	 * @param cTakeOveredFeatures      [in,out]       �������ꂽ���ʌ���ID�������p�����t�B�[�`���Q
	 * @param pReferenceNodes          [out,optional] �擾�����m�[�h�Q
	 * @param cNewFeatures             [in,out]       �V�K�쐬���ꂽ�t�B�[�`���Q
	 * @param cTargetFeatures          [in,out]       �X�v���b�g�Ώۂ��܂ލX�V�t�B�[�`���Q
	 * @pram  listLQRows               [out]          �X�v���b�g�̍ۂɌ�������LQ���R�[�h
	 * @retval TRUE  �����ɐ���
	 * @retval FALSE �����Ɏ��s
	 */
	BOOL GetSplitResult(
		LPCTSTR lpcszSplitLayers,
		const IGeometryPtr& ipGeom,
		CTargetItems& cNoSetModifyDataFeatures,
		CTargetItems& cModifyNodeKindFeatures,
		const IPointPtr& ipTakeOverPoint,
		INT iSplitMode,
		CTargetItems& cTakeOveredFeatures,
		CTargetItems* pReferenceNodes,
		CTargetItems& cNewFeatures,
		CTargetItems& cTargetFeatures,
		std::list<ROWDEF>& listLQRows);

	/**
	* @brief �X�v���b�g�Ώۃ��C���ɏ�������t�B�[�`���Q�����ꂼ��X�v���b�g�`��ŕ�������
	* @note  �\���_�̒ǉ��ʒu���Z�o
	* @note  �֘A�e�[�u���̃R�s�[���쐬
	* @param lpcszSplitLayers         [in]			 �X�v���b�g�Ώۃ��C���i�u|�v�Z�p���[�g�j
	* @param strOthersName            [in]           NW���f���ȊO�̃t�B�[�`����\�����f����
	* @param ipGeom                   [in]           �X�v���b�g�`��
	* @param cNoSetModifyDataFeatures [in]           �ҏW�������������Ȃ��t�B�[�`���Q
	* @param ipTakeOverPoint          [in,optional]  �X�v���b�g��Ɍ���OID�������p���t�B�[�`�������߂邽�߂̃|�C���g�i���̃|�C���g�ɋ߂��t�B�[�`���������p���j
	* @param iSplitMode               [in,optional]  �X�v���b�g���[�h�i0=Cross 1=Touch 2=Both, �f�t�H���g�F2�j
	* @param listLQRFInfo             [out]          �t�^����LQRF�Q
	* @pram  listLQRows               [out]          �X�v���b�g�̍ۂɌ�������LQ���R�[�h
	* @param cTakeOveredFeatures      [in,out]       �������ꂽ���ʌ���ID�������p�����t�B�[�`���Q
	* @param cNewFeatures             [in,out]       �V�K�쐬���ꂽ�t�B�[�`���Q
	* @param cTargetFeatures          [in,out]       �X�v���b�g�Ώۂ��܂ލX�V�t�B�[�`���Q
	* @param createNodePoints         [in]           NW���f�����Ƃ́A�\���_�i�m�[�h�j�����ׂ��|�C���g�̏W��
	* @retval TRUE  �����ɐ���
	* @retval FALSE �����Ɏ��s
	*/
	BOOL SplitFeatures(
		LPCTSTR lpcszSplitLayers,
		LPCTSTR strOthersName,
		const IGeometryPtr& ipGeom,
		const CTargetItems& cNoSetModifyDataFeatures,
		const IPointPtr& ipTakeOverPoint,
		INT iSplitMode,
		std::list<LQRFINFO>& listLQRFInfo,
		std::list<ROWDEF>& listLQRows,
		CTargetItems& cTakeOveredFeatures,
		CTargetItems& cNewFeatures,
		const CTargetItems& cTargetFeatures,
		std::map<const CString, IPointCollectionPtr>& createNodePoints);

	/**
	* @brief �l�b�g���[�N�����N�ł���t�B�[�`���݂̂�ΏۂƂ��ăX�v���b�g����
	* @note  ��̓I�ɂ�RuleModel�ɂ�NW���f���Ƃ��Ē�`����Ă���t�B�[�`���݂̂�ΏۂƂ���
	* @note  �m�[�h�𐶐����鏈���⃊���N��̕����������s��
	* @param ipGeom                   [in]           �X�v���b�g���邽�߂̃W�I���g��
	* @param cNewFeatures             [in,out]       �V�K�쐬���ꂽ�t�B�[�`���Q
	* @param cTargetFeatures          [in,out]       �X�v���b�g�Ώۂ��܂ލX�V�t�B�[�`���Q
	* @param cNoSetModifyDataFeatures [in,out]       �ҏW�������������Ȃ��t�B�[�`���Q
	* @param cModifyNodeKindFeatures  [in,out]       �m�[�h��ʂ��Đݒ肵�����̂ŁA���� cNewFeatures �ɂ� cTargetFeatures �ɂ����݂��Ȃ�����
	* @param listLQRFInfo             [in]           �t�^����LQRF�Q
	* @param createNodePoints         [in]           NW���f�����Ƃ́A�\���_�i�m�[�h�j�����ׂ��|�C���g�̏W��
	* @param cTakeOveredFeatures      [in]           �������ꂽ���ʌ���ID�������p�����t�B�[�`���Q
	* @param pReferenceNodes          [out,optional] �擾�����m�[�h�Q
	* @retval TRUE  �����ɐ����������AcTargetFeatures��NW���f���łȂ�
	* @retval FALSE �����Ɏ��s
	*/
	BOOL SplitNetworkLink(
		const IGeometryPtr& ipGeom,
		CTargetItems& cNewFeatures,
		CTargetItems& cTargetFeatures,
		CTargetItems& cNoSetModifyDataFeatures,
		CTargetItems& cModifyNodeKindFeatures,
		const std::list<LQRFINFO>& listLQRFInfo,
		const std::map<const CString, IPointCollectionPtr>& createNodePoints,
		CTargetItems& cTakeOveredFeatures,
		CTargetItems* pReferenceNodes) const;

	/**
	* @brief �l�b�g���[�N�����N�ɓ�����t�B�[�`���Q�̍\���_��֘AID�ɓK�؂�ID��ݒ�
	* @note  ���H�����N�ɑ΂��郊���N�������y�ьX�΂ȂǁB
	* @param cNewFeatures            [in,out] �V�K�쐬���ꂽ�t�B�[�`���Q
	* @param cTargetFeatures         [in,out] �X�v���b�g�Ώۂ��܂ލX�V�t�B�[�`���Q
	* @param cTakeOveredFeatures     [in]     �������ꂽ���ʌ���ID�������p�����t�B�[�`���Q
	* @param cModifyNodeKindFeatures [in]     �m�[�h��ʂ��Đݒ肵�����̂ŁA���� cNewFeatures �ɂ� cTargetFeatures �ɂ����݂��Ȃ�����
	* @retval TRUE  �֘AID�t�B�[���h�̐ݒ�ɐ���
	* @retval FALSE �֘AID�t�B�[���h�̐ݒ�Ɏ��s
	*/
	BOOL ResetCommonFeatures(
		CTargetItems& cNewFeatures,
		CTargetItems& cTargetFeatures,
		const CTargetItems& cNoSetModifyDataFeatures,
		const CTargetItems& cTakeOveredFeatures);
	/**
	* @brief �S�Ẵ����N�������y�ьX�΂� EndPoint ��ڂ���l�b�g���[�N�����N�ɒǉ�����
	* @param cNewFeatures            [in]     �V�K�쐬���ꂽ�t�B�[�`���Q
	* @param cSegAttrAndGradFeatures [in,out] �V�K�쐬�E�X�V���ꂽ�t�B�[�`�����烊���N�������E�X�΂��������o��������
	* @param cAllRoadFeatures        [in]     �V�K�쐬�E�X�V���ꂽ�t�B�[�`������l�b�g���[�N�����N���������o��������
	* @param ipRoadSpRef             [in]     �����ΏۂƂȂ����l�b�g���[�N�����N�t�B�[�`���N���X�̋�ԎQ��
	* @retval TRUE  EndPoint�̒ǉ��ɐ���
	* @retval FALSE EndPoint�̒ǉ��Ɏ��s
	*/
	BOOL AddEndPointToNetworkLink(
		const CTargetItems& cNewFeatures,
		CTargetItems& cSegAttrAndGradFeatures,
		const CTargetItems& cAllRoadFeatures,
		const ISpatialReferencePtr& ipRoadSpRef) const;

	/**
	* @brief �S�Ẵ����N�������y�ьX�΂̍\���_�č\�z�y�ъ֘A�����NID�̍Đݒ�
	* @param cNewFeatures             [in,out] �V�K�쐬���ꂽ�t�B�[�`���Q
	* @param cTargetFeatures          [in,out] �X�v���b�g�Ώۂ��܂ލX�V�t�B�[�`���Q
	* @param cNoSetModifyDataFeatures [in]     �ҏW�������������Ȃ��t�B�[�`���Q
	* @param cTakeOveredFeatures      [in]     �������ꂽ���ʌ���ID�������p�����t�B�[�`���Q
	* @param cSegAttrAndGradFeatures  [in,out] �V�K�쐬�E�X�V���ꂽ�t�B�[�`�����烊���N�������E�X�΂��������o��������
	* @param cAllRoadFeatures         [in]     �V�K�쐬�E�X�V���ꂽ�t�B�[�`������l�b�g���[�N�����N���������o��������
	* @param ipRoadClass              [in]     �����ΏۂƂȂ����l�b�g���[�N�H�����N�t�B�[�`���N���X
	* @param ipRoadSpRef              [in]     �����ΏۂƂȂ����l�b�g���[�N�����N�t�B�[�`���N���X�̋�ԎQ��
	* @retval TRUE  �֘A�l�b�g���[�N�����NID�̍Đݒ�ɐ���
	* @retval FALSE �֘A�l�b�g���[�N�����NID�̍Đݒ�Ɏ��s
	*/
	BOOL ReconstructCommonVertexAndResetRelField(
		CTargetItems& cNewFeatures,
		CTargetItems& cTargetFeatures,
		const CTargetItems& cNoSetModifyDataFeatures,
		const CTargetItems& cTakeOveredFeatures,
		CTargetItems& cSegAttrAndGradFeatures,
		const CTargetItems& cAllRoadFeatures,
		const IFeatureClassPtr& ipRoadClass,
		const ISpatialReferencePtr& ipRoadSpRef);

	/**
	* @brief �������NW�t�B�[�`���Ƃ��̊֘A�t�B�[�`���̑Ή���SiNDY�̃f�[�^�Ƃ��Č����Ƃ��ɖ��Ȃ����m�F
	* @note  cNewFeatures�͕ύX����Ȃ����AGetFeature�֐���const�ŌĂяo���Ȃ����߁Aconst�ɂ��Ă��Ȃ�
	* @param cNewFeatures    [in]     �V�K�쐬���ꂽ�t�B�[�`���Q
	* @param cTargetFeatures [in,out] �X�v���b�g�Ώۂ��܂ލX�V�t�B�[�`���Q
	* @pram  listLQRows      [in]     �X�v���b�g�̍ۂɌ�������LQ���R�[�h
	* @retval TRUE  �\���ɖ��͂Ȃ�����
	* @retval FALSE �\���ɖ�肪����
	*/
	BOOL CheckNWStructure(CTargetItems& cNewFeatures, CTargetItems& cTargetFeatures,  const std::list<ROWDEF>& listLQRows) const;

	/**
	* @brief NW�̃m�[�h�ƃ����N���������Ă��邩�m�F
	* @note CheckNWStructure���ł̂ݎg�p�����
	* @param cLastLinks [in] �X�v���b�g�Ώۂ��܂ލX�V�����NW�t�B�[�`���Q
	* @param cLastNodes [in] �X�v���b�g�Ώۂ��܂ލX�V�����NW�m�[�h�t�B�[�`���Q
	* @retval TRUE  �����N�������E�X�΂̕���������ɍs��ꂽ
	* @retval FALSE �����N�������E�X�΂̕���������ɍs���Ȃ�����
	*/
	BOOL CheckNWConnect(const CTargetItems& cLastLinks, const CTargetItems& cLastNodes) const;

	/**
	* @brief �����N�������E�X�΂̕���������ɍs��ꂽ���m�F
	* @note CheckNWStructure���ł̂ݎg�p�����
	* @param cLastRoads   [in] �X�v���b�g�Ώۂ��܂ލX�V����铹�H�����N�t�B�[�`���Q
	* @param cLastSegGrad [in] �X�v���b�g�Ώۂ��܂ލX�V����郊���N�������E�X�΃t�B�[�`���Q
	* @retval TRUE  �����N�������E�X�΂̕���������ɍs��ꂽ
	* @retval FALSE �����N�������E�X�΂̕���������ɍs���Ȃ�����
	*/
	BOOL CheckCommonFeaturesConnect(const CTargetItems& cLastRoads, const CTargetItems& cLastSegGrad) const;

	/**
	* @brief �����N��̕���������ɍs��ꂽ���m�F
	* @note CheckNWStructure���ł̂ݎg�p�����
	* @note bug 3521�Ή����Ƀt�B�[�`���̑����ύX���s���悤�ɂȂ��Ă���̂ŁAcLastRoads��out�ƂȂ��Ă���
	* @param cNewFeatures [in]     �V�K�쐬���ꂽ���H�����N�t�B�[�`���Q
	* @param cLastRoads   [in,out] �X�v���b�g�Ώۂ��܂ލX�V����铹�H�����N�t�B�[�`���Q
	* @pram  listLQRows   [in]     �X�v���b�g�̍ۂɌ�������LQ���R�[�h
	* @retval TRUE  �����N��̕���������ɍs��ꂽ
	* @retval FALSE �����N��̕���������ɍs���Ȃ�����
	*/
	BOOL CheckLQConnect(const CTargetItems& cNewFeatures, CTargetItems& cLastRoads, const std::list<ROWDEF>& listLQRows) const;

private:
/*	void SetDomain();
	void ResetDomain();*/
private:
	CLoadQueueInfo		m_cLoadQueueInfo;

//	DOUBLE m_dXMin, m_dXMax, m_dYMin, m_dYMax;
};

#endif // !defined(AFX_EDITHELPER_H__6456CB1F_0C72_4A48_95B7_12F5D5AEE546__INCLUDED_)
