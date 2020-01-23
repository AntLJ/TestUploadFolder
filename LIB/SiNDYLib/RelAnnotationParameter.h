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
 * @file RelAnnotationParameter.h
 * @brief <b>CRelAnnotationParameter�N���X��`�w�b�_�t�@�C��</b>
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @vertion $Id$
 */
#ifndef _RELANNOTATIONPARAMETER_H_
#define _RELANNOTATIONPARAMETER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RelAnnotation.h"

namespace sindy {

#ifdef SINDYLIB_CALC_TIME
extern SINDYLIB_API double g_dCRelAnnotationParameter_CopyTime;		//!< �I�u�W�F�N�g�R�s�[����
extern SINDYLIB_API double g_dCreateParameterCache_AllTime;			//!< CreateParameterCache�̑�����
extern SINDYLIB_API double g_dCreateParameterCache_RuleCacheTime;	//!< CreateParameterCache�̃��[���L���b�V������
extern SINDYLIB_API double g_dCreateParameterCache_AnnotationTime;	//!< CreateParameterCache�̒��L�^�ʒu�L����`�쐬����
extern SINDYLIB_API double g_dCreateParameterCache_DispLineTime;	//!< CreateParameterCache�̒��L�\���ʒu�o�E���f�B���O�{�b�N�X�쐬����
#endif // ifdef SINDYLIB_CALC_TIME

class CRelAnnotationParameter;
typedef boost::shared_ptr<CRelAnnotationParameter> CSPRelAnnotationParameter;

/**
 * @class CRelAnnotationParameter
 * @brief <b>�p�����[�^�L���b�V���@�\�t�����L�N���X</b>\n
 * ���̃N���X��CRelAnnotation�̋@�\�ɉ����A�e��p�����[�^���L���b�V������
 * �@�\��L���܂��B�`�F�b�J�[�Ȃǂō�������ʂɏ�������K�v������ꍇ��
 * �g�p���Ă��������B
 *
 * �L���b�V������p�����[�^�͈ȉ��̂Ƃ���ł��F
 * @li
 * @li
 * @li
 */
class SINDYLIB_API CRelAnnotationParameter : public CRelAnnotation
{
	ROWCLASS_CONSTRUCTOR( CRelAnnotationParameter, unknown );
	/**
	 * @brief <b>�R�s�[����</b>\n
	 * dest�ɃL���b�V��������ꍇ�́A�폜����܂��B
	 * �L���b�V��������ꍇ�́A�L���b�V�����ꏏ�ɃR�s�[����܂��B
	 * @see CRow::CopyTo
	 * @param dest				[in]	�y�[�X�g�ΏہiCRelAnnotationParameter����ł��j
	 * @param bIsCopyCache		[in]	�L���b�V�����R�s�[���邩�ǂ���
	 * @param bIsCopyRow		[in]	�I�u�W�F�N�g���R�s�[���邩�ǂ���
	 * @param bIsCopyFlag		[in]	�쐬�폜�t���O���R�s�[���邩�ǂ���
	 * @param bIsCopyContainer	[in]	�R���e�i���R�s�[���邩�ǂ���
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CopyTo( CRow& dest, bool bIsCopyCache=true, bool bIsCopyRow=true, bool bIsCopyFlag=true, bool bIsCopyContainer=true ) const;

	/**
	 * @brief <b>�p�����[�^�L���b�V�����폜����</b>\n
	 */
	void DeleteParameter();

	/**
	 * @brief <b>�p�����[�^�L���b�V�����쐬����</b>\n
	 * @param bForce	[in,optional]	�����I�ɍ�蒼���ꍇ��true�i�f�t�H���g�Ffalse�j
	 */
	errorcode::sindyErrCode UpdateParameter( bool bForce = false );

	/**
	 * @brief <b>�p�����[�^�L���b�V�����쐬����Ă��邩�ǂ������`�F�b�N����</b>\n
	 */
	bool IsCreatedParameterCache() { return m_bIsCached; }

	//@{ @name �`�F�b�N���[�`��
	/**
	 * @brief <b>�^�ʒu�ƕ\���ʒu�̃Z�b�g�ł̒P�́E�����`�F�b�N���s��</b>\n
	 * �s�s�n�}�ȊO�́AS1����S4�܂Ń`�F�b�N����܂��B\n\n
	 * �`�F�b�N����鍀�ڂ͈ȉ��̂Ƃ���F\n
	 * @li �P�̃`�F�b�N�ibDoUnitCheck��true�̎��̂݁j
	 *  @li �^�ʒu�P�̃`�F�b�N�iCAnnotaion::Check�Q�Ɓj
	 *  @li �\���ʒu�P�̃`�F�b�N�iCDispLine::Check�Q�Ɓj
	 * @li �����`�F�b�N�ibDoComplexCheck��true�̎��̂݁j
	 *  @li �^�ʒu����\��������|���S���܂ł̋����`�F�b�N
	 *  @li �^�ʒu�ƕ\��������|���S���̕�܊֌W�����������ǂ����̃`�F�b�N
	 *  @li �^�ʒu�ƕ\���ʒu�̃��C���^�C�v�̐������`�F�b�N
	 * @param cErrObjects		[out]			�G���[�I�u�W�F�N�g�̃��X�g
	 * @param bDoUnitCheck		[in,optional]	�P�̃`�F�b�N�����邩�ǂ���
	 * @param bDoComplexCheck	[in,optional]	�����`�F�b�N�����邩�ǂ���
	 * @param dThresholdMinPx	[in,optional]	�ŏ�臒l�i�P�ʁF�s�N�Z���A�}�C�i�X�A�f�t�H���g�F0.0�j
	 * @param dThresholdMaxPx	[in,optional]	�ő�臒l�i�P�ʁF�s�N�Z���A�f�t�H���g�F16.0�j
	 * @retval sindyErrCode �Ō�Ɍ��������G���[
	 */
	errorcode::sindyErrCode CheckLogic( CErrorObjects& cErrObjects, bool bDoUnitCheck = true, bool bDoComplexCheck = true, const double& dThresholdMinPx = 0.0, const double& dThresholdMaxPx = 16.0 ) const;
	//@}
	//@{ @name �C�e���[�^
	typedef std::list<WKSEnvelope>::iterator annorect_iterator;
	typedef std::list<WKSEnvelope>::const_iterator annorect_const_iterator;
	annorect_iterator annorect_begin() { return m_listAnnoRect.begin(); }
	annorect_iterator annorect_end() { return m_listAnnoRect.end(); }
	annorect_const_iterator annorect_begin() const { return m_listAnnoRect.begin(); }
	annorect_const_iterator annorect_end() const { return m_listAnnoRect.end(); }
	//@}

	/**
	 * @brief <b>���݊i�[����Ă���t�B�[�`���̌`��̃R�s�[��Ԃ�</b>\n
	 * @param bLatest               	[in,optional] �ŐV�̌`�󂪗~�����ꍇ��true�i�f�t�H���g�Ftrue�j
	 * @param bOriginal             	[in,optional] �ύX�O�̌`�󂪗~�����ꍇ��true�i�f�t�H���g�Ffalse�j
	 * @param bFeatureChangedOriginal	[in,optional] IFeatureChanges::get_OriginalShape�̌`�󂪗~�����ꍇ�ɂ�true�i�f�t�H���g�Ffalse�j
	 * @param bDiff				   		[in,optional] ������Ԃ��w�肵�����ɁAUnion�����`��ł͂Ȃ������`��ɂ��邩�ǂ���
	 * @param lParam					[in,optional] ���̃N���X�ł̓X�P�[���l�Ƃ��Ĉ����܂��B
	 * @note ���̊֐��́A�ύX�O�E�ύX����܂ޑS�Ă̌`��̃R�s�[���擾���邱�Ƃ��ł��܂��B
	 * ������Ԃ��w�肵���ꍇ��Union���ꂽ�`��ƂȂ�܂��B
	 * @note IGeometry::get_ShapeCopy�����̋@�\����������ɂ́A�����ɉ����w�肹���Ɏg�p���Ă��������B
	 * @note lParam�̓X�P�[���l�Ƃ��Ĉ����܂��B���̒l���Z�b�g����Ă���ꍇ�A���̃X�P�[���ł̋L����`
	 * �y�ѕ�����`�̃|���S�����Ԃ�܂��B
	 * ���̏ꍇ�A���̈����͖�������܂��B���̎d�l�́AGetShapeCopy�ł��̂悤�Ȋg���`����������߂�
	 * ����̍�ł��B
	 * @see CRow::GetShapeCopy
	 */
	IGeometryPtr GetShapeCopy( bool bLatest = true, bool bOriginal = false, bool bFeatureChangedOriginal = false, bool bDiff = false, long lParam = -1 ) const;

	/**
	 * @brief �L���b�V�����폜����
	 * @see CRow::DeleteCache()
	 */
	void DeleteCache( bool bRecursive = false );

	/**
	 * @brief <b>���[���L���b�V����Ԃ�</b>\n
	 * �n���S�w���n��ƒn���ƂňقȂ郋�[�����g�p���邽�߁A���[���̃L���b�V����
	 * 2���邱�Ƃւ̑Ή��ł��B
	 * @retval NULL		�G���[
	 * @retval NULL�ȊO	���[�����R�[�h�I�u�W�F�N�g�̃|�C���^
	 */
	const annotation_class::CAnnotationClass* GetValidRule() const;
#ifdef SINDYLIB_CALC_TIME
	static void TraceTime();
	static void ClearTime()
	{
		g_dCRelAnnotationParameter_CopyTime = 0.0;
		g_dCreateParameterCache_AllTime = 0.0;
		g_dCreateParameterCache_RuleCacheTime = 0.0;
		g_dCreateParameterCache_AnnotationTime = 0.0;
		g_dCreateParameterCache_DispLineTime = 0.0;
	}
#endif // ifdef SINDYLIB_CALC_TIME
protected:
	/**
	 * @brief �����o�ϐ�������������
	 * see CRow::Init()
	 */
	void Init();
private:
	bool										m_bIsCached;			//!< �p�����[�^�L���b�V�����쐬���ꂽ���ǂ����̔���p
#pragma warning(push)
#pragma warning(disable: 4251)
	// ���L�^�ʒu�L���b�V��
	std::list<WKSEnvelope>						m_listAnnoRect;			//!< ���L�^�ʒu�L����`
#pragma warning(pop)
};

} // sindy

#endif // ifndef _RELANNOTATIONPARAMETER_H_
