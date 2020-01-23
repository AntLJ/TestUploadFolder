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
 * @file Feature.h
 * @brief CFeature�N���X��`�t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _FEATURE_H_
#define _FEATURE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "Geometry.h"

namespace sindy {
class CErrorInfos;
/**
 * @class CFeature
 * @brief <b>�t�B�[�`���N���X</b>\n
 * �S�Ẵt�B�[�`���n�̃N���X�͂��̃N���X�����ɂȂ�܂��B
 */
class SINDYLIB_API CFeature : public CRow
{
	ROWCLASS_CONSTRUCTOR( CFeature, unknown )
	operator IFeature*() const;
	operator IFeature**(){ Release(); return (IFeature**)&p; }
	//@{ @name IFeature�C���^�[�t�F�[�X
	IObjectClassPtr GetClass() const;
	IEnvelopePtr GetExtent() const;
	esriFeatureType GetFeatureType() const;
	//@}
	/**
	 * @brief <b>�^����ꂽ�`��͈̔͂ŕ��s�ړ��������s��</b>\n
	 * �ҏW���[�����l�����Ĉړ��������s���܂��B\n
	 * �ҏW�ς݃`�F�b�N�͍s���܂���B
	 * @param cGeom			[in]	�N�G���`��
	 * @param pairMove		[in]	�ړ�����
	 * @param emMode		[in]	�ړ����[�h
	 * @param cErrInfos		[out]	�G���[���i�[�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Move( const CGeometry& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>�^����ꂽ�`��Ƃ̃}�[�W���s��</b>\n
	 * �ҏW�ς݃`�F�b�N�͍s���܂���B
	 * @param cGeom			[in]	�}�[�W�`��
	 * @param cErrObjects	[out]	�G���[���i�[�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Merge( const CGeometry& cGeom, CErrorObjects& cErrObjects );
	/**
	 * @brief <b>�^����ꂽ�`��͈̔͂ɂ���_�̍폜���s��</b>\n
	 * �ҏW�ς݃`�F�b�N�͍s���܂���B
	 * @param cGeom			[in]	�폜�`��
	 * @param cErrObjects	[out]	�G���[���i�[�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Erase( const CGeometry& cGeom, CErrorObjects& cErrObjects );
	/**
	 * @brief <b>�^����ꂽ�`��ŕ������s��</b>\n
	 * �ҏW�ς݃`�F�b�N�͍s���܂���B
	 * �����`��Point/MultiPoint�̎��́A��_�Ƃ����߂Ȃ��Ŏg�p���Ă���̂Œ��ӁB
	 * ���C���ƃ|���S���̎�����AheGetCrossOrTouchPoints�Ō�_�����߂Ă�B
	 * @param cGeom				[in]	�����`��
	 * @param cSplittedGeoms	[out]	��������Ăł����`��Q
	 * @param cSplittedPoints	[out]	�����_
	 * @param cErrObjects		[out]	�G���[���i�[�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Split( const CGeometry& cGeom, CGeometry& cSplittedGeoms, CGeometry& cSplittedPoints, CErrorObjects& cErrObjects );
	/**
	 * @brief <b>�|�����C������_�Q�ŕ�������</b>\n
	 * @param cSplitterPoints	[in]	��_�Q
	 * @param cSplittedPoints	[out]	�����_
	 * @return ������̌`��Q
	 */
	IGeometryPtr _SplitPolyline( const CGeometry& cSplitterPoints, IPointCollection** ipSplittedPoints=NULL );
	/**
	 * @brief <b>�|���S�����|�����C���ŕ�������</b>\n
	 * @param cSplitterPoints	[in]	��_�Q
	 * @param cSplitterGeom		[in]	�������C��
	 * @return ������̌`��Q
	 */
	IGeometryPtr _SplitPolygon( const CGeometry& cSplitterPoints, const CGeometry& cSplitterGeom );

	/**
	 * @brief <b>���݊i�[����Ă���t�B�[�`���̑��݋�`��Ԃ�<b>\n
	 * @param bOriginal              [in,optional] �ύX�O�̌`���`���~�����ꍇ��true�i�f�t�H���g�Ftrue�j
	 * @param bChanged               [in,optional] �ύX��̌`���`���~�����ꍇ��true�i�f�t�H���g�Ftrue�j
	 * @param bOriginalIsArcOriginal [in,optional] IFeatureChanges::get_OriginalShape�̌`���`���~�����ꍇ�ɂ�true�i�f�t�H���g�Ftrue�j
	 * @note ���̊֐��́A�Ⴆ�ΕҏW��̍ĕ`�������ꍇ�ɕύX�O�E�ύX��̌`����܂񂾋�`���~�����ꍇ�ȂǂɎg�p���܂��B
	 * @warning Store()���Ă��܂��ƃL���b�V���͑S�Ĕj������܂��̂ŁA�ύX�O�̌`�󂪎擾�ł��Ȃ��Ȃ�܂��B
	 * �ύX�O�̌`����擾����K�v������ꍇ��Store()�O�ɂ��̊֐����ĂԕK�v������܂��B
	 */
	IEnvelopePtr GetEnvelope( bool bOriginal = true, bool bChanged = true, bool bFeatureChangedOriginal = true ) const;

	/// ��ԎQ�Ƃ��擾����
	ISpatialReferencePtr GetSpatialReference() const;
};

typedef boost::shared_ptr<CFeature> CSPFeature;

} // namespace sindy

#endif // _FEATURE_H_
