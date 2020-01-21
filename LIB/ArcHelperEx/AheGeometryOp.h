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

#ifndef ARCHELPEREX_AHEGEOMETRYOP_H__
#define ARCHELPEREX_AHEGEOMETRYOP_H__

#include "define_enum.h"

typedef struct tagSEGMENTITEM
{
	esriGeometryType geometryType;
	LONG lPartID;
	LONG lSegmentID;
	BOOL bIsLastSegment;
	BOOL bIsFirstSegment;
	DOUBLE dFromX;
	DOUBLE dFromY;
	DOUBLE dToX;
	DOUBLE dToY;
	IPointPtr ipFromPoint;
	IPointPtr ipToPoint;
	_ISegmentPtr ipSegment;

} SEGMENTITEM;

/// �G���A�|�C���g���擾����
IPointPtr AheGetAreaPoint( IGeometry* ipGeom );

/**
 * @brief �n�_���擾����
 * @param ipGeom [in] �n�_���擾����`��
 * @retval NULL     ipGeom��ICurvePtr�ɕϊ��ł��Ȃ��`�󂩁A�n�_���Ȃ��`��
 * @retval NULL�ȊO �n�_�ʒu��ێ�����IPointPtr
 */
IPointPtr AheGetFromPoint( IGeometry* ipGeom );

/**
 * @brief �I�_���擾����
 * @param ipGeom [in] �I�_���擾����`��
 * @retval NULL     ipGeom��ICurvePtr�ɕϊ��ł��Ȃ��`�󂩁A�I�_���Ȃ��`��
 * @retval NULL�ȊO �I�_�ʒu��ێ�����IPointPtr
 */
IPointPtr AheGetToPoint( IGeometry* ipGeom );

/**
 * @brief �[�_���m�̐ڑ��_���擾����
 *
 * �[�_���m�Őڑ����Ă���ꍇ�̂݌���ł��B\n
 * �߂�l�́A���ipGeom1��̓_�ł��i�|�C���g�̏ꍇ�͎Q�Ƃł��邱�Ƃɒ��Ӂj�B\n
 * �ڑ����Ă��Ȃ��ꍇ��NULL���Ԃ�܂��B\n
 * IPointCollection����IPoint���T�|�[�g���Ă���^�C�v�ł����
 * ���삵�܂��B\n
 * �������݂���ꍇ�͍ŏ��Ɍ������ق���Ԃ��܂��B
 * @param ipGeom1 [in] ��r�`��P
 * @param ipGeom2 [in] ��r�`��Q
 * @return IPointPtr
 */
IPointPtr AheGetConnectPoint( IGeometry* ipGeom1, IGeometry* ipGeom2 );

/// WKSPoint���쐬����
WKSPoint MakeWKSPoint( IPoint* ipPoint );

/// WKSPoint���쐬����
WKSPoint MakeWKSPoint( const double& x, const double& y );
/////////////////////////////////////////////////////////////////////////////
// �W�I���g���ϊ���
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief IEnvelope -> IPolygon �ϊ�
 *
 * @note �ϊ����ꂽ�|���S���ɂ͋�ԎQ�Ƃ��ݒ肳��Ă��܂���
 *sAhe
 * @param ipEnv				[in]			IEnvelopePtr
 *
 * @return IPolygonPtr
 */
IPolygonPtr AheEnvelope2Polygon( IEnvelopePtr ipEnv );

/**
 * @brief �}���`�|���S������|���S���̃Z�b�g���擾����
 *
 * @note �Ⴆ�΁A���|���S���̓h�[�i�c�Ɠ���2���Ԃ�܂�
 * @note �}���`�|���S���ł͂Ȃ��ꍇ�́A1�����Ԃ�܂�
 *
 * @ipPolygon				[in]			�}���`�|���S��
 *
 * @retrn _ISetPtr
 */
_ISetPtr AheConvertMultiPolygon2Polygons( IPolygonPtr ipPolygon );

/**
 * @brief �|���S�����|�����C���ɕϊ�����
 * 
 * @note �|���S���̃��C����S�ă|�����C���ɕϊ������W�I���g����Ԃ��܂��B
 * @note ������ Interior/Exterior �Ή��ς݁B
 *
 * @param ipPolygon			[in]			�ϊ����|���S��
 *
 * @return IGeometyPtr
 */
IGeometryPtr AheConvertPolygon2Polyline( IPolygonPtr ipPolygon );

/**
 * @brief IGeometry -> list<SEGMENTITEM> �ϊ�
 *
 * @param ipGeometry		[in]			IGeometryPtr
 * @param listSegments		[out]			�Z�O�����g�R���N�V����
 *
 * @retval TRUE	����
 * @retcal FALSE ���s
 */
BOOL AheGeometry2SegmentItems( IGeometryPtr ipGeometry, std::list<SEGMENTITEM>& listSegments );

/////////////////////////////////////////////////////////////////////////////
// �����A�p�x�Ȃ�
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief 3�_�̂Ȃ��p�i0<=x<=180�j�����߂�
 *
 * @param ipPoint1			[in]			IPoint �̃C���X�^���X
 * @param ipPointCenter		[in]			IPoint �̃C���X�^���X
 * @param ipPoint2			[in]			IPoint �̃C���X�^���X
 *
 * @return 3�_�̂Ȃ��p�x
 */
DOUBLE AheGetAngle( IPointPtr ipPoint1, IPointPtr ipPointCenter, IPointPtr ipPoint2 );

/** 
 * @brief ���C���̐ڑ��p�����߂�
 *
 * @param ipCurve1			[in]			ICurvePtr
 * @param ipCurve2			[in]			ICurvePtr
 *
 * @return �ڑ��p�i���s�����ꍇ�� 0 ��������̂Œ��ӁI�I�j
 */
DOUBLE AheGetAngle( ICurvePtr ipCurve1, ICurvePtr ipCurve2 );

/**
 * @brief ���ς����߂�iipPoint1->ipPoint2 , ipPoint3->ipPoint4�j
 *
 * @param ipPoint1			[in]			IPoint �̃C���X�^���X
 * @param ipPoint2			[in]			IPoint �̃C���X�^���X
 * @param ipPoint3			[in]			IPoint �̃C���X�^���X
 * @param ipPoint4			[in]			IPoint �̃C���X�^���X
 *
 * @return ����
 */
DOUBLE AheGetInnerProduct( IPointPtr ipPoint1, IPointPtr ipPoint2, IPointPtr ipPoint3, IPointPtr ipPoint4 );

/**
 * @brief �x�N�g���������߂�iipPoint1->ipPoint2�j
 *
 * @param ipPoint1			[in]			IPoint �̃C���X�^���X
 * @param ipPoint2			[in]			IPoint �̃C���X�^���X
 *
 * @return �Q�_�̃x�N�g����
 */
DOUBLE AheGetLength( IPointPtr ipPoint1, IPointPtr ipPoint2 );

/**
 * @brief �����im�j�����߂�
 *
 * @param ipP1			[in]			IPoint �̃C���X�^���X
 * @param ipP2			[in]			IPoint �̃C���X�^���X
 *
 * @return �Q�_�̋����im�j
 */
double AheGetMeterLength( IPointPtr& ipP1, IPointPtr& ipP2 );

/**
 * @brief ���K�����W���������߂�
 *
 * @param ipP1			[in]			IPoint �̃C���X�^���X
 * @param ipP2			[in]			IPoint �̃C���X�^���X
 *
 * @return �Q�_�̐��K�����W����
 */
double AheGetMeshXYLength( IPointPtr& ipP1, IPointPtr& ipP2 );

/////////////////////////////////////////////////////////////////////////////
// RelationalOperator�n
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief �W�I���g���̊֘A���`�F�b�N����
 *
 * @note ��{�W�I���g���A��r�W�I���g�����ɃN���[�����쐬���Ĕ�r���܂�
 * @note ipSpRel �́A����t�B�[�`���N���X�̃W�I���g�����m�̏ꍇ�͂��̃t�B�[�`���N���X��SpatialReference��
 * @note �ǂ��ł����A�ʃt�B�[�`���N���X���m�̏ꍇ�͕Е���SpatialReference��n���Ɛ��������肪�A���Ă��܂���B
 * @note ���̏ꍇ�́AIMap::get_SpatialReference() �Ŏ擾�ł�����̂��g�p���邵���Ȃ��̂ł����A���̏ꍇ�r��
 * @note SpatialReference�������Ă��郌�C�����ǂݍ��܂�Ă���ƑS�Ă̍ő���񐔂�SpatialReference���g�p����
 * @note �邱�ƂɂȂ�܂��̂Œ��ӂ��K�v�ł��B
 * @note relOption �͕����w�肪�ł��܂�
 * @note see �W�I�f�[�^�x�[�X�݌v�K�C�h - Modiling Our World - p.110
 *
 * @param ipBaseGeom		[in]			��{�W�I���g��
 * @param ipTestGeom		[in]			��r�W�I���g��
 * @param relOption			[in,optional]	sindyeSpatialRel
 * @param ipSpRel			[in,optional]	�n���Q��
 *
 * @return sindyeSpatialRel
 */
sindyeSpatialRel AheTestGeometryRelationship( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, sindyeSpatialRel relOption = (sindyeSpatialRel)127, ISpatialReferencePtr ipSpRel = NULL );

/**
 * @brief �W�I���g�����m���ڐG���Ă��邩�ǂ���
 *
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			��{�W�I���g��
 * @param ipTestGeom		[in]			��r�W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval TRUE �ڐG���Ă���
 * @retval FALSE �ڐG���Ă��Ȃ�
 */
BOOL AheIsTouch( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsTouch( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, sindyeSpatialRelTouch, ipSpRel ) & sindyeSpatialRelTouch ) ? TRUE : FALSE;
}

/**
 * @brief �W�I���g�����m���d�����Ă��邩�ǂ���
 *
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			��{�W�I���g��
 * @param ipTestGeom		[in]			��r�W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval TRUE �d�����Ă���
 * @retval FALSE �d�����Ă��Ȃ�
 */
BOOL AheIsOverlap( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsOverlap( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, sindyeSpatialRelOverlap, ipSpRel ) & sindyeSpatialRelOverlap ) ? TRUE : FALSE;
}

/**
 * @brief �W�I���g�����m���d�����Ă��邩�ǂ���
 *
 * @note ����`��A�܂ށA�܂܂��̎��� TRUE ��Ԃ��܂�
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			��{�W�I���g��
 * @param ipTestGeom		[in]			��r�W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval TRUE �d�����Ă���
 * @retval FALSE �d�����Ă��Ȃ�
 */
BOOL AheIsOverlap2( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsOverlap2( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, (sindyeSpatialRel)(sindyeSpatialRelOverlap|sindyeSpatialRelEqual|sindyeSpatialRelWithin|sindyeSpatialRelContain), ipSpRel ) ) ? TRUE : FALSE;
}

/**
 * @brief �W�I���g�����m���N���X���Ă��邩�ǂ���
 *
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			��{�W�I���g��
 * @param ipTestGeom		[in]			��r�W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval TRUE �N���X���Ă���
 * @retval FALSE �N���X���Ă��Ȃ�
 */
BOOL AheIsCross( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsCross( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, sindyeSpatialRelCross, ipSpRel ) & sindyeSpatialRelCross ) ? TRUE : FALSE;
}

/**
 * @brief ��{�W�I���g������r�W�I���g���Ɋ܂܂�邩�ǂ���
 *
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			��{�W�I���g��
 * @param ipTestGeom		[in]			��r�W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval TRUE ��{�W�I���g������r�W�I���g���Ɋ܂܂��
 * @retval FALSE ��{�W�I���g������r�W�I���g���Ɋ܂܂�Ȃ�
 */

BOOL AheIsWithin( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsWithin( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, sindyeSpatialRelWithin, ipSpRel ) & sindyeSpatialRelWithin ) ? TRUE : FALSE;
}

/**
 * @brief ��{�W�I���g������r�W�I���g���Ɋ܂܂�邩�ǂ����i����`��܂ށj
 *
 * @note AheIsWithin �͓���`��̏ꍇ�� FALSE �ɂȂ�܂����AAheIsWithin2 �͓���`��̎��� TRUE ��Ԃ��܂�
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			��{�W�I���g��
 * @param ipTestGeom		[in]			��r�W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval TRUE ��{�W�I���g������r�W�I���g���Ɋ܂܂��
 * @retval FALSE ��{�W�I���g������r�W�I���g���Ɋ܂܂�Ȃ�
 */
BOOL AheIsWithin2( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsWithin2( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, (sindyeSpatialRel)(sindyeSpatialRelWithin|sindyeSpatialRelEqual), ipSpRel ) ) ? TRUE : FALSE;
}

/**
 * @brief ��{�W�I���g������r�W�I���g�����܂ނ�
 *
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			��{�W�I���g��
 * @param ipTestGeom		[in]			��r�W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval TRUE ��{�W�I���g������r�W�I���g�����܂�
 * @retval FALSE ��{�W�I���g������r�W�I���g�����܂܂Ȃ�
 */
BOOL AheIsContain( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsContain( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, sindyeSpatialRelContain, ipSpRel ) & sindyeSpatialRelContain ) ? TRUE : FALSE;
}

/**
 * @brief ��{�W�I���g������r�W�I���g�����܂ނ��i����`��܂ށj
 *
 * @note AheIsContain �͓���`��̏ꍇ�� FALSE �ɂȂ�܂����AAheIsContain2 �͓���`��̎��� TRUE ��Ԃ��܂�
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			��{�W�I���g��
 * @param ipTestGeom		[in]			��r�W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval TRUE ��{�W�I���g������r�W�I���g�����܂�
 * @retval FALSE ��{�W�I���g������r�W�I���g�����܂܂Ȃ�
 */
BOOL AheIsContain2( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsContain2( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, (sindyeSpatialRel)(sindyeSpatialRelContain|sindyeSpatialRelEqual), ipSpRel ) ) ? TRUE : FALSE;
}

/**
 * @brief ��{�W�I���g������r�W�I���g�����܂ނ��i����`��܂ށj
 *
 * @note ��{�@�\�� AheIsContain �Ɠ����ł����AAheTestGeometryRelationship �� sindyRelNone ���Ԃ��ė����ꍇ
 *		 �� PointCollection �ɕϊ����Ĕ�r���܂��B
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			��{�W�I���g��
 * @param ipTestGeom		[in]			��r�W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval TRUE ��{�W�I���g������r�W�I���g�����܂�
 * @retval FALSE ��{�W�I���g������r�W�I���g�����܂܂Ȃ�
 */
BOOL AheIsContain3( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );

/**
 * @brief ��{�W�I���g������r�W�I���g�����܂ނ��i����`��܂ށj
 *
 * @note ��{�@�\�� AheIsContain �Ɠ����ł����AipTestGeom �� �|�C���g �ȊO�̎��� �Z�O�����g ����
 *		 �������Ĕ�r���܂��B
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			��{�W�I���g��
 * @param ipTestGeom		[in]			��r�W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval TRUE ��{�W�I���g������r�W�I���g�����܂�
 * @retval FALSE ��{�W�I���g������r�W�I���g�����܂܂Ȃ�
 */
BOOL AheIsContain4( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );

/**
 * @brief �W�I���g�����m���������Ă��邩�ǂ���
 *
 * @see AheTestGeometryRalationship
 *
 * @param ipBaseGeom		[in]			��{�W�I���g��
 * @param ipTestGeom		[in]			��r�W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval TRUE �������Ă���
 * @retval FALSE �������Ă��Ȃ�
 */
BOOL AheIsDisjoint( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );
inline BOOL AheIsDisjoint( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ ){
	return ( AheTestGeometryRelationship( ipBaseGeom, ipTestGeom, sindyeSpatialRelDisjoint, ipSpRel ) & sindyeSpatialRelDisjoint ) ? TRUE : FALSE;
}

/**
 * @brief �W�I���g�����m�����������ǂ���
 * 
 * @note ���̊֐���IClone::Equals���g�p���܂��B
 * �܂��A�|�C���g���m�̔�r�̏ꍇ�ɂ� bug 5617 ����ׂ̈�
 * AheIsExactEqual�Ƃ̌��ʂƔ�r���A�قȂ�ꍇ�ɂ̂݋�ԎQ��
 * �ɂ��킹�čēx��r�������ʂ�Ԃ��܂��B
 * ��ԎQ�Ƃ��^�����Ă��Ȃ��ꍇ�ɂ͑o���̋�ԎQ�Ƃ̂��ׂ���
 * �����g�p���܂��B
 *
 * @param ipBaseGeom		[in]			��{�W�I���g��
 * @param ipTestGeom		[in]			��r�W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval TRUE ������
 * @retval FALSE �������Ȃ�
 */
BOOL AheIsEqual( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );

/**
 * @beif �f�[�^�x�[�X���Ɋi�[�����ꍇ�ɓ���_���ǂ��������؂���
 *
 * @note ���̊֐��́A��ԎQ�Ƃ��g�p����DB�i�[�l�i�����l�j�ɕϊ����A���̒l���������ǂ�����
 * ���ꔻ�肵�܂��B���݂̓|�C���g�����T�|�[�g���܂���B
 * @warning ��ԎQ�Ƃ����ꂩ�ǂ����͌��܂���B�ŏ��̈����̃W�I���g���̂��̂��g�p���܂��B
 * @see bug 5617
 */
BOOL AheIsDBEqual( IPointPtr ipP1, IPointPtr ipP2 );

/**
 * @brief �W�I���g�����m�����������ǂ���
 * 
 * @note ���̊֐��͓Ǝ���r�ɂ�茵���ɔ�r���܂��B
 * ��r�I�ɂ���r���s�������ꍇ��AheIsEqual���A
 * ����Ɋɂ���r���s�������ꍇ��IRelationalOperator::Equals��
 * �g�p���Ă��������B
 *
 * @param ipBaseGeom		[in]			��{�W�I���g��
 * @param ipTestGeom		[in]			��r�W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval TRUE ������
 * @retval FALSE �������Ȃ�
 */
BOOL AheIsExactEqual( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel = NULL );

/////////////////////////////////////////////////////////////////////////////
// TopologicalOperator�n�֐�
/////////////////////////////////////////////////////////////////////////////
/**
 * �W�I���g���� Union �����
 *
 * @note ipBaseGeom�AipAddGeom �̓R�s�[���쐬���Ă��牉�Z���܂�
 * @note �n���Q�Ƃ����킹�邱�Ƃ͔��Ɋ댯�Ȃ̂ŁA���̊֐��ɂ��Ă� ipSpRef �� NULL �̏ꍇ�͒n���Q�Ƃ͂��킹�܂���
 *
 * @param ipBaseGeom		[in]			Union �̃x�[�X�ƂȂ�W�I���g��
 * @param ipAddGeom			[in]			ipBaseGeom �� Union ����肽���W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval IGeometryPtr Union ��̃W�I���g��
 * @retval NULL Union ���s
 */
IGeometryPtr AheUnion( IGeometryPtr ipBaseGeom, IGeometryPtr ipAddGeom, ISpatialReferencePtr ipSpRef = NULL );

/**
 * �|�����C�� Union �����
 *
 * @note ipBaseGeom�AipAddGeom �̓R�s�[���쐬���Ă��牉�Z���܂�
 * @note AheUnion�͕K��Simplify�������邽�߁A���Ȍ����|�����C����FROM�ATO�����������Ȃ�܂��B
 * @note ���Ƀ����N��̕\���ł̓}���`�p�[�g�ɂȂ�Ɩ�󂪂������ȂƂ���ɂł܂��̂ŁA���̏ꍇ��
 * @note AheUnionNetwork���g�p���Ă��������B
 * @note ���̊֐��ł�SnapToSpatialReference�͍s���܂���B
 *
 * @param ipBaseGeom		[in]			Union �̃x�[�X�ƂȂ�W�I���g��
 * @param ipAddGeom			[in]			ipBaseGeom �� Union ����肽���W�I���g��
 * @param ipSpRel			[in]			�n���Q��
 * 
 * @retval IGeometryPtr Union ��̃W�I���g��
 * @retval NULL Union ���s
 */
IGeometryPtr AheUnionNetwork( IPolylinePtr ipBaseGeom, IPolylinePtr ipAddGeom, ISpatialReferencePtr ipSpRef );

/**
 * �W�I���g�����m�����̂���i�}���`�����_���p�j
 *
 * @note ipBaseGeom�AipAddGeom �̓R�s�[���쐬���Ă��牉�Z���܂�
 * @note �n���Q�Ƃ����킹�邱�Ƃ͔��Ɋ댯�Ȃ̂ŁA���̊֐��ɂ��Ă� ipSpRef �� NULL �̏ꍇ�͒n���Q�Ƃ͂��킹�܂���
 *
 * @param ipBaseGeom		[in]			Union �̃x�[�X�ƂȂ�W�I���g��
 * @param ipAddGeom			[in]			ipBaseGeom �� Union ����肽���W�I���g��
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval IGeometryPtr �Z�O�����g�ǉ���̃W�I���g��
 * @retval NULL  ���s
 */
IGeometryPtr AheUnionSegments( IGeometryPtr ipBaseGeom, IGeometryPtr ipAddGeom, ISpatialReferencePtr ipSpRef = NULL );

IGeometryPtr AheDifference( IGeometryPtr ipBaseGeom, IGeometryPtr ipAddGeom, ISpatialReferencePtr ipSpRef = NULL );

/**
 * �W�I���g���� Intersect �����
 *
 * @note ipBaseGeom�AipAddGeom �̓R�s�[���쐬���Ă��牉�Z���܂�
 *
 * @param ipBaseGeom		[in]			Intersect �̃x�[�X�ƂȂ�W�I���g���iipSpRef �� NULL �̏ꍇ�͂��ꂩ��n���Q�Ǝ擾�j
 * @param ipAddGeom			[in]			ipBaseGeom �� Intersect ����肽���W�I���g��
 * @param enumDimension		[in,optional]	Intersect ��̃W�I���g���̎����i�f�t�H���g�FesriGeometry1Dimension�j
 * @param ipSpRel			[in,optional]	�n���Q��
 * 
 * @retval IGeometryPtr Intersect ��̃W�I���g��
 * @retval NULL Intersect ���s
 */
IGeometryPtr AheIntersect( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, esriGeometryDimension enumDimension = esriGeometry1Dimension, ISpatialReferencePtr ipSpRef = NULL );

/////////////////////////////////////////////////////////////////////////////
// �������Z�֐�
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief �������邢�͐ڐG���Ă���|�C���g���擾����
 *
 * @note ITopologicalOperator::Intersects() �ł͎擾�ł��Ȃ��ꏊ�ł������ɔ��肵�܂�
 *
 * @param ipBaseGeometry	[in]			��r���W�I���g��
 * @param ipTestGeometry	[in]			��r��W�I���g��
 * @param iMode				[in,optional]	���胂�[�h�i0=Cross 1=Touch 2=Both, �f�t�H���g�F2�j
 * @param bIsDisjoint		[in,optional]	���胂�[�h�ɂ�炸�ADisjoint���ǂ�����Ԃ��i�f�t�H���g�FNULL�j
 */
IGeometryPtr AheGetCrossOrTouchPoints( IGeometryPtr ipBaseGeometry, IGeometryPtr ipTestGeometry, INT iMode = 2, BOOL* bIsDisjoint = NULL );

/**
 * @brief �|�C���g���|�����C���y�у|���S���t�B�[�`���� From �|�C���g�ł��邩�ǂ����̔���
 *
 * @see AheIsFromPoint
 *
 * @param ipPointGeom		[in]			�|�C���g�W�I���g��
 * @param ipCurveFeature	[in]			�|�����C���y�у|���S���t�B�[�`��
 * @param ipSpRef			[in,optional]	ISpatialReferencePtr
 *
 * @retval TRUE From �|�C���g�ł���
 * @retval FALSE From �|�C���g�ł͂Ȃ��A���̓|�����C���y�у|���S���t�B�[�`���ł͂Ȃ�
 */
BOOL AheIsFromPoint( IGeometryPtr ipPointGeom, IFeaturePtr ipCurveFeature, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief �|�C���g���|�����C���y�у|���S���� From �|�C���g�ł��邩�ǂ����̔���
 *
 * @see AheIsEqual
 *
 * @param ipPointGeom       [in]			�|�C���g�W�I���g��
 * @param ipCurve		    [in]			�|�����C���y�у|���S��
 * @param ipSpRef			[in,optional]	ISpatialReferencePtr
 *
 * @retval TRUE From �|�C���g�ł���
 * @retval FALSE From �|�C���g�ł͂Ȃ��A���̓|�����C���y�у|���S���ł͂Ȃ�
 */
BOOL AheIsFromPoint( IPointPtr ipPoint, ICurvePtr ipCurve, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief �|�C���g���|�����C���y�у|���S���t�B�[�`���� To �|�C���g�ł��邩�ǂ����̔���
 *
 * @see AheIsToPoint
 *
 * @param ipPointGeom		[in]			�|�C���g�W�I���g��
 * @param ipCurveFeature	[in]			�|�����C���y�у|���S���t�B�[�`��
 * @param ipSpRef			[in,optional]	ISpatialReferencePtr
 *
 * @retval TRUE To �|�C���g�ł���
 * @retval FALSE To �|�C���g�ł͂Ȃ��A���̓|�����C���y�у|���S���t�B�[�`���ł͂Ȃ�
 */
BOOL AheIsToPoint( IGeometryPtr ipPointGeom, IFeaturePtr ipCurveFeature, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief �|�C���g���|�����C���y�у|���S���� To �|�C���g�ł��邩�ǂ����̔���
 *
 * @see AheIsEqual
 *
 * @param ipPointGeom       [in]			�|�C���g�W�I���g��
 * @param ipCurve			[in]			�|�����C���y�у|���S��
 * @param ipSpRef			[in,optional]	ISpatialReferencePtr
 *
 * @retval TRUE To �|�C���g�ł���
 * @retval FALSE To �|�C���g�ł͂Ȃ��A���̓|�����C���y�у|���S���ł͂Ȃ�
 */
BOOL AheIsToPoint( IPointPtr ipPoint, ICurvePtr ipCurve, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief �|�C���g���|�����C���y�у|���S���t�B�[�`���̒[�_�ł��邩�ǂ����̔���
 *
 * @see AheIsEndPoint
 *
 * @param ipPointGeom       [in]			�|�C���g�W�I���g��
 * @param ipCurveFeature	[in]			�|�����C���y�у|���S���t�B�[�`��
 * @param ipSpRef			[in,optional]	ISpatialReferencePtr
 *
 * @retval TURE �[�_
 * @retval FALSE �[�_�ȊO
 */
BOOL AheIsEndPoint( IGeometryPtr ipPointGeom, IFeaturePtr ipCurveFeature, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief �|�C���g���|�����C���y�у|���S���̒[�_�ł��邩�ǂ����̔���
 *
 * @see AheIsFromPoint
 * @see AheIsToPoint
 *
 * @param ipPoint			[in]			�|�C���g
 * @param ipCurve			[in]			�|�����C���y�у|���S��
 * @param ipSpRef			[in,optional]	ISpatialReferencePtr
 *
 * @retval TURE �[�_
 * @retval FALSE �[�_�ȊO
 */
BOOL AheIsEndPoint( IPointPtr ipPoint, ICurvePtr ipCurve, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief �W�I���g���̒[�_���m���ڐG���Ă��邩�ǂ���
 *
 * @note �Ώۂ̓|�����C���̂�
 *
 * @param ipBaseCurve		[in]			��{�W�I���g��
 * @param ipTestCurve		[in]			��r�W�I���g��
 * @param ipSpRef			[in,optional]	�n���Q��
 * @param ipTouchPoint		[out,optional]	NULL����Ȃ���΁ATOUCH �����|�C���g��Ԃ��iFrom�D��j
 *
 * @retval TRUE �ڐG���Ă���
 * @retval FALSE �ڐG���Ă��Ȃ�
 */
BOOL AheIsTouchEndPoint( ICurvePtr ipBaseCurve, ICurvePtr ipTestCurve, ISpatialReferencePtr ipSpRef = NULL, IPoint** ipTouchPoint = NULL  );

/**
 * @brief �|�C���g���W�I���g����̍\���_���ǂ����̔���
 *
 * @param ipPoint			[in]			��r�|�C���g 
 * @param ipGeom			[in]			�W�I���g��
 * @param ipSpRef			[in,optional]	ISpatialReferencePtr
 * @param pIndex			[out,optional]	�q�b�g�����C���f�b�N�X�ԍ�
 *
 * @retval TRUE �\���_�ł���
 * @retval FALSE �\���_�ł͂Ȃ�
 */
BOOL AheIsVertexPoint( IPointPtr ipPoint, IGeometryPtr ipGeom, ISpatialReferencePtr ipSpRef = NULL, LONG* pIndex = NULL );

/**
 * @brief �W�I���g�������Ȍ������Ă��邩�ǂ����̔���
 *
 * @note �� 2 ������ TRUE ���w�肷��ƁA���S�Ɍ������Ă��镔���͎��Ȍ����Ƃ͔��肵�܂���
 *
 * @param ipGeometry		[in]			��r�W�I���g��
 * @param bIsNetworkFeature	[in,optional]	���H�n�̃W�I���g�����ǂ����i�f�t�H���g�FTRUE�j
 *
 * @retval TRUE ���Ȍ������Ă���
 * @retval FALSE ���Ȍ������Ă��Ȃ�
 */
BOOL AheIsOneselfCross( IGeometryPtr ipGeometry, BOOL bIsNetworkFeature = TRUE );

/**
 * @brief �W�I���g���ɔ����Z�O�����g�����݂��邩�ǂ����̔���
 *
 * @param ipGeometry		[in]			��r�W�I���g��
 * @param ipSpRef			[in,optional]	�ŏ��������v�Z���邽�߂̋�ԎQ�Ɓi�f�t�H���g�FNULL�j
 * @param bFixGeometry		[in,optional]	�����Z�O�����g�𔭌������ꍇ�ɏC�����邩�ǂ����i�f�t�H���g�FFALSE�j
 */
BOOL AheHasMicroSegment( IGeometryPtr ipGeometry, ISpatialReferencePtr ipSpRef = NULL, BOOL bFixGeometry = FALSE );

/**
 * @brief �|�C���g���Z�O�����g��ɏ���Ă��邩�ǂ���
 *
 * @note �n���ꂽ�����ɂ͈�ؕύX�������܂���
 * @note ���̊֐��͈ȉ��̃A���S���Y���Ŕ�����s���܂�
 * @note 
 * @note 1. �|�C���g�ƃZ�O�����g�̋������v�Z���āA��ԎQ�Ƃ̍ŏ��P�ʒ���蒷����� FALSE
 * @note                 ��
 * @note 2. ��r�|�C���g�����ԋ߂��Ƃ���Ƀ|�C���g��ǉ������� SnapToSpatialReference ��������B
 * @note    ���̌��ʌ��ʈړ������ǉ��_�ƁASnapToSpatialReference ������r�|�C���g���ׁA����ł�
 * @note    ����� FALSE�A����Ȃ� TRUE
 * @note
 * @note ���̊֐��́A�`��m���Ƀ|�C���g���ǂ��Ɉړ����邩���l���č쐬���Ă���܂��̂ŁA���S�ł��B
 * @note IRelationalOperator �� Touches �� Contains �ň���������Ȃ����Ƃ�����̂ŁA�������
 * @note �g�p���ĉ������B
 *
 * @param ipBaseSegment		[in]			�x�[�X�Z�O�����g
 * @param ipTestPoint		[in]			��r�|�C���g
 * @param bExeptEndPoint	[in,optional]	���[�_�������ꍇ��TRUE���w��i�f�t�H���g�FFALSE�j
 *
 * @retval TRUE ����Ă���
 * @retval FALSE ����Ă��Ȃ�
 */
BOOL AhePointIsOnSegment( _ISegmentPtr ipBaseSegment, IPointPtr ipTestPoint, ISpatialReferencePtr ipSpRef, BOOL bExeptEndPoint = FALSE );

/**
 * @brief �|�C���g���W�I���g����ɏ���Ă��邩�ǂ���
 *
 * @see AhePointIsOnSegment
 *
 * @note 
 * @param ipBaseGeometry	[in]			�x�[�X�W�I���g��
 * @param ipTestPoint		[in]			��r�|�C���g
 * @param ipSpRef			[in,optional]	��ԎQ�ƁiNULL�̏ꍇ�̓x�[�X�̂��̂��g�p�B�덷����o���ɂ����g�p���܂���B�j
 * @param bExeptVertex		[in,optional]	�\���_�������ꍇ��TRUE�����Ă��i�f�t�H���g�FFALSE�j
 *
 * @retval TRUE ����Ă���
 * @retval FALSE ����Ă��Ȃ�
 */
BOOL AhePointIsOnGeometry( IGeometryPtr ipBaseGeometry, IPointPtr ipTestPoint, ISpatialReferencePtr ipSpRef = NULL, BOOL bExeptVertex = FALSE );

/**
 * @brief �x�[�X�̃W�I���g����ɂ̂��Ă���e�X�g�̃W�I���g���̍\���_��Ԃ�
 *
 * @see AhePointIsOnGeometry
 *
 * @param ipBaseGeometry	[in]			�x�[�X�W�I���g��
 * @param ipTestGeometry	[in]			�e�X�g�W�I���g��
 * @param ipSpRef			[in,optional]	��ԎQ�ƁiNULL�̏ꍇ�̓x�[�X�̂��̂��g�p�B�덷����o���ɂ����g�p���܂���B�j
 *
 * @retval IPointCollectionPtr�iMultipoint�j
 * @retval NULL	�������Ă��Ȃ�
 */
IPointCollectionPtr AheGetOnGeometryPoints( IGeometryPtr ipBaseGeometry, IGeometryPtr ipTestGeometry, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief �x�[�X�̃W�I���g����ɒǉ��p�\���_������Ă���̂ł���΁A�\���_��ǉ�����
 *
 * @note �W�I���g����ɓ_�����݂��邩�ǂ����̔���́AAhePointIsOnGeometry ���g�p���Ă��܂�
 * @see AhePointIsOnGeometry
 *
 * @param ipBaseGeometry	[in]			�x�[�X�W�I���g��
 * @param ipAddVertexPoint	[in]			�ǉ��\���_�p�|�C���g
 * @param ipSpRef			[in,optional]	��ԎQ�ƁiNULL�̏ꍇ�̓x�[�X�̂��̂��g�p�B�덷����o���ɂ����g�p���܂���B�j
 *
 * @retval TRUE �ǉ�����
 * @retval FALSE �ǉ����Ȃ�����
 */
BOOL AheAddVertexIfOnGeometry( IGeometryPtr ipBaseGeometry, IPointPtr ipAddVertexPoint, ISpatialReferencePtr ipSpRef = NULL );

/**
 * @brief �x�[�X�̃W�I���g����ɒǉ��p�\���_�Q������Ă���̂ł���΁A�\���_��ǉ�����
 *
 * @see AheAddVertexIfOnGeometry
 *
 * @param ipBaseGeometry	[in]			�x�[�X�W�I���g��
 * @param ipAddVertexPoints	[in]			�ǉ��\���_�p�|�C���g�Q
 * @param ipSpRef			[in,optional]	��ԎQ�ƁiNULL�̏ꍇ�̓x�[�X�̂��̂��g�p�B�덷����o���ɂ����g�p���܂���B�j
 *
 * @retval TRUE �ǉ�����
 * @retval FALSE �ǉ����Ȃ�����
 */
BOOL AheAddVertexesIfOnGeometry( IGeometryPtr ipBaseGeometry, IGeometryPtr ipAddVertexPoints, ISpatialReferencePtr ipSpRef = NULL );

/// �|�C���g�ړ����Ƀ��o�[�o���h���ƂȂ肤��Z�O�����g��Ԃ�
ISegmentCollectionPtr AheGetRubberBandSegment( IGeometryPtr ipGeom, IPointPtr ipSplitPoint, ISpatialReferencePtr ipSpRef = NULL );

/**
 *
 * @brief IsKnownSimple��VARIANT_TRUE��ݒ�
 *
 * @param ipGeometry	[in]	�ݒ肷��`��
 */
IGeometryPtr AheSetKnownSimple(IGeometryPtr ipGeometry);
/**
 * @brief �ׂ荇���Ă���_�����������̂��폜(�S�p�[�g)
 *
 * @see AheRemoveAdjoiningPoint
 *
 * @param ipGeometry	[in]			�W�I���g��
 */
IGeometryPtr AheRemoveAdjoiningPoint(IGeometryPtr ipGeometry);
/**
 * @brief �ׂ荇���Ă���_�����������̂��폜
 *
 * @see AheRemoveAdjoiningPointProc
 *
 * @param ipGeometry	[in]			�W�I���g��
 */
IGeometryPtr AheRemoveAdjoiningPointProc(IGeometryPtr ipGeometry);

/////////////////////////////////////////////////////////////////////////////
// �l�b�g���[�N�n�`�F�b�N�Ŏg�p����֐�
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief �\���_���m���ڐG���Ă��Ȃ����ǂ������ׂ�
 *
 * �l�b�g���[�N�n�I�u�W�F�N�g���m�̍\���_���m�̐ڐG����Ɏg�p���܂��i���[�h�`�F�b�N 02320�j
 * �Ȃ̂ŁA�[�_���m�̐ڐG�͖������܂��B
 */
bool AheIsTouchVertex( IPointCollectionPtr& ipBaseGeom, IPointCollectionPtr& ipTestGeom, IPointPtr& ipTouch );

/**
 * @brief �[�_���ڐG���Ă���`��̑��Z�O�����g�Ƃ̋�����Ԃ��܂��B
 * 
 * �l�b�g���[�N�I�u�W�F�N�g�̐ڑ��Z�O�����g�̋�����r�Ɏg�p���܂��i���[�h�`�F�b�N 02322�j
 */
double AheGetConnectSegmentCrack( IPointCollectionPtr& ipBaseGeom, IPointCollectionPtr& ipTestGeom );

#endif // ifndef ARCHELPEREX_AHEGEOMETRYOP_H__
