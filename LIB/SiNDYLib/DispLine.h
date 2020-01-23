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
 * @file DispLine.h
 * @brief CDispLine�N���X��`�t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _DISPLINE_H_
#define _DISPLINE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Feature.h"
#include "AnnotationCommunity.h"
#include "AheWKSFunctions.h"

namespace sindy {

#ifdef SINDYLIB_CALC_TIME
extern SINDYLIB_API double g_dCalcFlagStringBoundsTime;	//!< CalcStringBounds������
extern SINDYLIB_API double g_dCalcPositionTime;			//!< CalcPosition������
extern SINDYLIB_API double g_dCalcParameterTime;		//!< CalcParameter������
extern SINDYLIB_API double g_dQueryPointsTime;			//!< CalcParameter�̓_��擾������
extern SINDYLIB_API double g_dCalcStringBoundsTime;		//!< CalcParameter��AheCalcStringBounds������
#endif // ifdef SINDYLIB_CALC_TIME

class CDispLine;
typedef boost::shared_ptr<CDispLine> CSPDispLine;

/**
 * @class CDispLine
 * @brief <b>���L�\���ʒu�t�B�[�`�����ۃN���X</b>\n
 * ���̃N���X�͒��L�\���ʒu�t�B�[�`���̒��ۃN���X�ł��B
 * ���L�\���ʒu�t�B�[�`���̋��ʕ����͑S�Ă��̃N���X�ɋL�q
 * ����Ă��܂��B
 */
class SINDYLIB_API CDispLine : public CFeature
{
	ROWCLASS_CONSTRUCTOR( CDispLine, unknown )

	friend class CRelAnnotationParameter; // �p�����[�^�L���b�V���𒼐ڎQ�Ƃ���֌W��friend�ݒ� TODO: ������������
	friend class CAnnotations; // ��ԃC���f�b�N�X���쐬����֌W��friend�ݒ� TODO: ������������

	//@{ @name �����擾�֐�
	/**
	 * @brief <b>�t�H���g�T�C�Y���擾����</b>\n
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::disp_line::font_size::ECode GetFontSize() const
	{
		return (schema::disp_line::font_size::ECode)GetLongValueByFieldName( schema::disp_line::kFontSize, -1 );
	}

	/**
	 * @brief <b>�֘A����s�s�n�}���LID���擾����</b>\n
	 * @return �G���[����NULL�̏ꍇ��-1�A����ȊO�͓s�s�n�}���LOBJECTID
	 */
	long GetAnnoID() const
	{
		return GetLongValueByFieldName( schema::disp_line::kAnnoID, -1 );
	}

	/**
	 * @brief <b>�Q�ƕ�����R�[�h��Ԃ�</b>\n
	 * @return �G���[����NULL�̏ꍇ��-1�A����ȊO��targetstr_code::ECode
	 */
	schema::disp_line::target_str::ECode GetTargetStr() const
	{
		return (schema::disp_line::target_str::ECode)GetLongValueByFieldName( schema::disp_line::kTargetStr, -1 );
	}
	
	/**
	 * @brief <b>��s���L�̓�s�ڊJ�n�C���f�b�N�X�ԍ����擾����</b>\n
	 * @return ��s���L�̓�s�ڊJ�n�C���f�b�N�X�ԍ�
	 */
	long GetSecondLineStartIndex() const
	{
		return m_lSecondLineStartIndex;
	}

	/**
	 * @brief <b>���L�����񃉃C���^�C�v���擾����</b>\n
	 * @return sindyAnnotationLineType
	 */
	sindyAnnotationLineType GetAnnotationLineType() const
	{
		return m_emLineType;
	}
	//@}

	//@{ @name �`�F�b�N�֐�
	/**
	 * @brief <b>�t�H���g�T�C�Y�l���`�F�b�N����</b>\n
	 * @note �t�H���g�T�C�Y��10�̎��́A�������g�p����Ă���΃G���[�A�g�p����Ă��Ȃ����
	 * �x���ƂȂ�܂��B
	 * @param lpcszNameString	[in]	�Ή�����\��������i�t�H���g�T�C�Y��10�̏ꍇ�̂ݎg�p�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckFontSize( LPCTSTR lpcszNameString ) const;

	/**
	 * @brief <b>�֘A���L�^�ʒuID���`�F�b�N����</b>\n
	 * 0�ȏ��K���Ƃ��܂��B���̏ꍇ�A�֘AID�����^�ʒu��
	 * �{���ɑ��݂��邩�ǂ����̃`�F�b�N�͂��܂���B
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckAnnoID() const;

	/**
	 * @brief <b>�Q�ƕ�����R�[�h���`�F�b�N����</b>\n
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckTargetStr() const;

	/**
	 * @brief <b>�\�����C���̊p�x���`�F�b�N����</b>\n
	 * @param emLineType	[in]			���C���^�C�v
	 * @param dErrAngle		[out,optional]	�G���[���̊p�x���Ԃ�i�f�t�H���g�FNULL�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckAngle( sindyAnnotationLineType emLineType, double* dErrAngle/* = NULL*/ ) const;

	/** �\�����C���`��̐��������`�F�b�N����
	 * @brief emDispTypeRule [in] ���C���\�����[��
	 * @retval sindyErr_AnnoDispLineTypeInvalid ���[�����u����v�Ȃ̂Ƀ��C���^�C�v���u����v�ł͂Ȃ��A���̓��[�����u����v����Ȃ��̂Ƀ��C���^�C�v���u����v
	 * @retval sindyErr_AnnoDispLineIllegalDouble �ꕶ���������݂��Ȃ��s������A����2�s�ڐ܂�Ԃ����������l
	 * @retval sindyErr_NoErr ������
	 */
	errorcode::sindyErrCode CheckLine( schema::annotation_class::disptype_rule::ECode emDispTypeRule ) const;

	//@}

	/**
	 * @brief <b>�\���ʒu�p�����[�^���v�Z����</b>\n
	 * �c���̔���́A��_�ڂƓ�_�ڂ�XY���W���݂Ĕ��肵�܂��B\n
	 * ��s��s�̔���͈�_�ڂƂ��̑��̓_���r���āA�ω������邲�Ƃ�
	 * �J�E���^�𑝉������Ĕ��肵�܂��B�J�E���^��2�ȏ�ɂȂ�ƁA����
	 * ���L�Ƃ݂Ȃ���܂��B\n\n
	 * @param bIsHalfString			[in]			���������K��̔����Ƃ��Čv�Z����\n�R�x�W���l�̏ꍇ�͂����true�ɂ���K�v������܂��B\n
	 *												�R�x�W���l���ǂ�����CDispLine::IsAltitudeValue�Œ��ׂ邱�Ƃ��ł��܂��B
	 * @param pSecondLineStartIndex	[out,optional]	��s���L�̏ꍇ�ɓ�s�ڊJ�n�C���f�b�N�X�ԍ�������i�f�t�H���g�FNULL�j
	 * @param pStringRects			[out,optional]	�X�̕\���������`������\n���ꂼ��͕��������O�ɂȂ�i�f�t�H���g�FNULL�j
	 * @param pRect					[out,optional]	�\���ʒu���C����`�i�ŏI�_�͊܂܂Ȃ��j�i�f�t�H���g�FNULL�j\n
	 *                                              ��s���L�̎��͋�`�ɂ͂Ȃ�܂���̂Œ��ӁI�I
	 * @param ipStringBounds		[out,optional]	������̃o�E���f�B���O�{�b�N�X������i�f�t�H���g�FNULL�j
	 * @retval -1 �`�󂪑��݂��Ȃ�
	 * @retval sindyAnnotationLineType
	 */
	sindyAnnotationLineType CalcParameter( bool bIsHalfString, long* pSecondLineStartIndex = NULL, WKSPoint* pStringRects = NULL, WKSEnvelope* pRect = NULL, IGeometry** ipStringBounds = NULL ) const;

	/**
	 * @brief <b>�n���ꂽ�|�C���g�ɑ΂��ĕ\���ʒu���c���E���̂ǂ̈ʒu�ɂ��邩�𔻒肷��</b>\n
	 * @param cPoint		[in]	����Ώۂ̃|�C���g
	 * @param emLineType	[in]	�c���E��s��s�iCalcParameter�Ŏ擾�\�j
	 * @param cRect			[in]	�\���ʒu���C����`�i�ŏI�_�͊܂܂Ȃ��j�iCalcParameter�Ŏ擾�\�j
	 * @return sindyAnnotationPosition
	 */
	sindyAnnotationPosition CalcPosition( const CPoint& cPoint, sindyAnnotationLineType emLineType, const WKSEnvelope& cRect ) const;

	/**
	 * @brief <b>�\�������O�`���v�Z����</b>\n
	 * �n���ꂽ�\�������O�`�Ɋ��g���̏ꍇ�̌`���t�����܂��B
	 * @param ipBound	[in,out]	�\��������O�`
	 * return sindyErrCode
	 */
	errorcode::sindyErrCode CalcStringBounds( IPoint* pAnnoPoint, IGeometry* ipBound ) const;

	IGeometryPtr GetStringBounds() const { return m_ipGeom; }

	// 
	/**
	 * @brief <b>1pt������̒������擾����</b>\n
	 * @param bCalcX	[in]	X���W�̒������擾����ꍇ��true�AY���W�̒������擾����ꍇ��false
	 * @return 1pt������̒����i�P�ʁF�x�i�ܓx�o�x�j�j
	 */
	double Calc1PTLen( bool bCalcX ) const;

	/**
	 * @brief <b>�e�[�u���^�C�v�ƃX�P�[�����r����</b>\n
	 * @param emScale	[in]	�X�P�[��
	 * @retval true �e�[�u���^�C�v�ɂӂ��킵���X�P�[��
	 * @retval false �e�[�u���^�C�v�ɍ���Ȃ��X�P�[��
	 */
	bool IsSameScale( sindyAnnotationScale emScale ) const { return ( emScale == GetMaxScale() ) ? true : false; }

	/**
	 * @brief <b>�R�x�W���l���ǂ���</b>\n
	 * @param emCode	[in]	��ʃR�[�h
	 * @retval true		�R�x�W���l�ł���
	 * @retval false	�R�x�W���l�ł͂Ȃ�
	 */
	bool IsAltitudeValue( schema::annotation::annotation_code::ECode emCode ) const { 
		return ( emCode == schema::annotation::annotation_code::kMountain && GetTargetStr() == schema::disp_line::target_str::kStr3 ) ? true : false;
	}

	/**
	 * @brief <b>�\���ł���X�P�[���̍ő�l���擾����</b>
	 * @retval -1		�G���[
	 * @retval -1�ȊO	�\���ő�X�P�[��
	 */
	sindyAnnotationScale CDispLine::GetMaxScale() const;


#ifdef SINDYLIB_CALC_TIME
	static void TraceTime();
	static void ClearTime()
	{
		g_dCalcStringBoundsTime = 0.0;
		g_dCalcPositionTime = 0.0;
		g_dCalcParameterTime = 0.0;
		g_dQueryPointsTime = 0.0;
	}
#endif // ifdef SINDYLIB_CALC_TIME

	/**
	 * @brief <b>�R�s�[����</b>\n
	 * dest�ɃL���b�V��������ꍇ�́A�폜����܂��B
	 * �L���b�V��������ꍇ�́A�L���b�V�����ꏏ�ɃR�s�[����܂��B
	 * @see CRow::CopyTo
	 * @param dest				[in]	�y�[�X�g�ΏہiCDispLine����ł��j
	 * @param bIsCopyCache		[in]	�L���b�V�����R�s�[���邩�ǂ���
	 * @param bIsCopyRow		[in]	�I�u�W�F�N�g���R�s�[���邩�ǂ���
	 * @param bIsCopyFlag		[in]	�쐬�폜�t���O���R�s�[���邩�ǂ���
	 * @param bIsCopyContainer	[in]	�R���e�i���R�s�[���邩�ǂ���
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CopyTo( CRow& dest, bool bIsCopyCache=true, bool bIsCopyRow=true, bool bIsCopyFlag=true, bool bIsCopyContainer=true ) const;

	/**
	 * @brief �L���b�V�����폜����
	 * @see CRow::DeleteCache()
	 */
	void DeleteCache( bool bDeleteContainerCache = false );

	/**
	 * @brief �p�����[�^�L���b�V�����폜����
	 */
	void ClearParameterCache();
protected:
	/**
	 * @brief �����o�ϐ�������������
	 * see CRow::Init()
	 */
	void Init();

	/**
	 * @brief �p�����[�^�L���b�V�����Z�b�g����
	 * 
	 * ���̊֐��́ACRelAnnotationParameter::CreateParameterCache�݂̂���Ă΂�܂��B
	 * TODO: �{���p�����[�^�L���b�V����CDispLine�ō쐬�����ׂ��ł����A�������ڂ��܂ł�
	 * �ԁA�b��I�ɂ��̊֐��ŃZ�b�g���܂��B
	 */
	void SetParameterCache( int nStringRectSize, long lSecondLineStartIndex, WKSPoint* pStringRects, const WKSEnvelope& cRect, IGeometry* ipGeom, sindyAnnotationLineType emLineType, bool bIsFlag, bool bCanCopy );
private:
#pragma warning(push)
#pragma warning(disable: 4251)
	int							m_nStringRectsSize;			//!< m_pStringRects�̃T�C�Y
	long						m_lSecondLineStartIndex;	//!< ��s���L�̓�s�ڊJ�n�C���f�b�N�X�ԍ�
	WKSPoint*					m_pStringRects;				//!< �X�̒��L������`
	WKSEnvelope					m_cRect;					//!< ���L������\����`
	IGeometryPtr				m_ipGeom;					//!< ���L������o�E���f�B���O�{�b�N�X�i���g���`��܂ށj
	sindyAnnotationLineType		m_emLineType;				//!< ���L�����񃉃C���^�C�v
	bool						m_bIsFlag;					//!< ���g�����ǂ���
	bool						m_bCanCopy;					//!< �R�s�[�\���ǂ���
#pragma warning(pop)
};

} // sindy

#endif // _DISPLINE_H_
