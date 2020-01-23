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
 * @file Annotation.h
 * @brief<b>CAnnotation�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _ANNOTATION_H_
#define _ANNOTATION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Feature.h"
#include "AnnotationCommunity.h"
#include "RuleCommunity.h"
#include "TableType.h"
#include "AheWKSFunctions.h"

namespace sindy {

namespace annotation_class {
class CAnnotationClass;
};

/**
 * @class CAnnotation
 * @brief <b>���L�^�ʒu�t�B�[�`�����ۃN���X</b>\n
 * ���̃N���X�͒��L�^�ʒu�Ƃ��Ď��ׂ��C���^�[�t�F�[�X�̂�
 * ���������ۃN���X�ł��B
 */
class SINDYLIB_API CAnnotation : public CFeature
{
public:
	explicit CAnnotation()  : m_pAnnoRule(NULL), m_pAnnoRule2(NULL){}
	virtual ~CAnnotation(){}

	CAnnotation( sindyTableType::ECode emTableType, IFeature* lp ) : m_pAnnoRule(NULL), m_pAnnoRule2(NULL)
	{
		SetObject( lp, emTableType );
	}
	
	CAnnotation( sindyTableType::ECode emTableType, _IRow* lp ) : m_pAnnoRule(NULL), m_pAnnoRule2(NULL)
	{
		SetObject( lp, emTableType );
	}

	explicit CAnnotation( const CAnnotation& obj ) : m_pAnnoRule(NULL), m_pAnnoRule2(NULL)
	{
		*this = obj;
	}
	
	CAnnotation& operator=( const CAnnotation& obj )
	{
		obj.CopyTo( *this );
		m_pAnnoRule = obj.m_pAnnoRule;
		m_pAnnoRule2 = obj.m_pAnnoRule2;

		return *this;
	}

	//@{ @name �`�F�b�N�֐�
	/**
	 * @brief <b>���L������P���`�F�b�N����</b>\n
	 * ���[���ɉ�����������̃`�F�b�N�A�y��
	 * GetStringNum1�Ƃ̐��������`�F�b�N���܂��B
	 * @see sindy::CheckOrFixAnnotationString
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckString1() const;

	/**
	 * @brief <b>���L������Q���`�F�b�N����</b>\n
	 * ���[���ɉ�����������̃`�F�b�N�A�y��
	 * GetStringNum2�Ƃ̐��������`�F�b�N���܂��B
	 * @see sindy::CheckOrFixAnnotationString
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckString2() const;

	/**
	 * @brief <b>���L������R���`�F�b�N����</b>\n
	 * ���[���ɉ�����������̃`�F�b�N�A�y��
	 * GetStringNum3�Ƃ̐��������`�F�b�N���܂��B
	 * @see sindy::CheckOrFixAnnotationString
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckString3() const;

	/**
	 * @brief<b>���L������̒������`�F�b�N����</b>\n
	 * [rule_check_1_3_4_5E] ���L������͖���1>=����2�łȂ���΂Ȃ�Ȃ�\n
	 * [rule_check_1_3_4_4E] ����1�����݂��Ȃ��̂ɖ���2�����݂���
	 * �i���̃`�F�b�N�͏�̃`�F�b�N��CheckString1�ACheckString2�ł܂��Ȃ���̂ŕs�v�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckStringLen() const;

	/**
	 * @brief <b>���L��ʂ��`�F�b�N����</b>\n
	 * [rule_check_1_3_4_10E] ��`����Ă��Ȃ���ʃR�[�h�ł���΃G���[�i���[���e�[�u���ɓo�^����Ă���R�[�h�Ɣ�r����j\n
	 * ���L��ʂ��s�s�n�}�p�̎�ʂ��ǂ������`�F�b�N���܂��B
	 * �`�F�b�N���邽�߂ɒ��L�p���[���e�[�u�����Q�Ƃ��܂��B
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckAnnoCode() const;

	/**
	 * @brief <b>�\���^�C�v���`�F�b�N����</b>\n
	 * �\���^�C�v�Ǝ�ʂ̐��������Ă��邩�ǂ������`�F�b�N���܂��B
	 * �`�F�b�N���邽�߂ɒ��L�p���[���e�[�u�����Q�Ƃ��܂��B
	 * @param emTableType	[in]	���L�\���ʒu���C���e�[�u���^�C�v
	 * @retval sindyErr_NoErr	���������Ă���
	 * @retval sindyErr_AnnoDispTypeInvalid ���������Ă��Ȃ�
	 */
	errorcode::sindyErrCode CheckDispType( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>������|���S���܂ł̋������`�F�b�N����</b>\n
	 * @paarm pBounds			[in]			������|���S���i���g�������`��j
	 * @param emType			[in]			�\���ʒu���C���e�[�u���^�C�v
	 * @param dThresholdMinPx	[in,optional]	�ŏ�臒l�i�P�ʁF�s�N�Z���A�}�C�i�X�A�f�t�H���g�F0.0�j
	 * @param dThresholdMaxPx	[in,optional]	�ő�臒l�i�P�ʁF�s�N�Z���A�f�t�H���g�F16.0�j
	 * @param pdDistance		[out,optional]	���苗�����Ԃ�
	 * @retval sindyErr_NoErr								�G���[����
	 * @retval sindyErr_AnnoToStringBoundsLengthLimitOver	�������z��O
	 */
	errorcode::sindyErrCode CheckToStringBoundsLength( IGeometry* pBounds, sindyTableType::ECode emType, const double& dThresholdMinPx = 0.0, const double& dThresholdMaxPx = 16.0, double* pdDistance = NULL ) const;
	//@}

	//@{ @name �����擾�֐�
	/**
	 * @brief <b>���L������P��Ԃ�</b>\n
	 * @param nLen		[in]	lpszName�̎��[�\������
	 * @param lpszName	[out]	�̈���m�ۂ���Ă���o�b�t�@
	 * @return �G���[����NULL�̏ꍇ�͋󕶎���
	 */
	LPCTSTR GetNameString1( int nLen, LPTSTR lpszName ) const
	{
		return GetStringValueByFieldName( schema::annotation::kNameString1, nLen, lpszName );
	}

	/**
	 * @brief <b>���L������Q��Ԃ�</b>\n
	 * @param nLen		[in]	lpszName�̎��[�\������
	 * @param lpszName	[out]	�̈���m�ۂ���Ă���o�b�t�@
	 * @return �G���[����NULL�̏ꍇ�͋󕶎���
	 */
	LPCTSTR GetNameString2( int nLen, LPTSTR lpszName ) const
	{
		return GetStringValueByFieldName( schema::annotation::kNameString2, nLen, lpszName );
	}

	/**
	 * @brief <b>���L������R��Ԃ�</b>\n
	 * ���̊֐��͒��k�ȏ�̃��C���Ƃ̌݊�����ۂ��̂ł���A�s�s�n�}�̏ꍇ�͏�ɃG���[�ɂȂ�܂��B
	 * @param nLen		[in]	lpszName�̎��[�\������
	 * @param lpszName	[out]	�̈���m�ۂ���Ă���o�b�t�@
	 * @return �G���[����NULL�̏ꍇ�͋󕶎���
	 */
	LPCTSTR GetNameString3( int nLen, LPTSTR lpszName ) const
	{
		return GetStringValueByFieldName( schema::annotation::kNameString3, nLen, lpszName );
	}

	/**
	 * @brief <b>�Ώە�����R�[�h�ɏ]���Ē��L�������Ԃ�</b>\n
	 * @param emCode	[in]	�Ώە�����R�[�h
	 * @param nLen		[in]	lpszName�̎��[�\������
	 * @param lpszName	[out]	�̈���m�ۂ���Ă���o�b�t�@
	 * @return �G���[����NULL�̏ꍇ�͋󕶎���
	 */
	LPCTSTR GetNameString( schema::disp_line::target_str::ECode emCode, int nLen, LPTSTR lpszName ) const;

	/**
	 * @brief <b>���L�P��������Ԃ�</b>\n
	 * @retval -1 �G���[���͒l��NULL
	 * @retval -1�ȊO ���L�P������
	 */
	long GetStringNum1() const
	{
		return GetLongValueByFieldName( schema::annotation::kStringNum1, -1 );
	}

	/**
	 * @brief <b>���L�Q��������Ԃ�</b>\n
	 * @retval -1 �G���[���͒l��NULL
	 * @retval -1�ȊO ���L�Q������
	 */
	long GetStringNum2() const
	{
		return GetLongValueByFieldName( schema::annotation::kStringNum2, -1 );
	}

	/**
	 * @brief <b>���L�R��������Ԃ�</b>\n
	 * ���̊֐��͒��k�ȏ�̃��C���Ƃ̌݊�����ۂ��̂ł���A�s�s�n�}�̏ꍇ�͏��-1�ɂȂ�܂��B
	 * @retval -1 �G���[���͒l��NULL
	 * @retval -1�ȊO ���L�Q������
	 */
	long GetStringNum3() const
	{
		return GetLongValueByFieldName( schema::annotation::kStringNum3, -1 );
	}

	/**
	 * @brief <b>�Ώە�����R�[�h�ɏ]���ĕ�������Ԃ�</b>\n
	 * @param emCode	[in]	�Ώە�����R�[�h
	 * @retval -1 �G���[���͒l��NULL
	 * @retval -1�ȊO ���L�Q������
	 */
	long GetStringNum( schema::disp_line::target_str::ECode emCode ) const;

	/**
	 * @brief <b>���L��ʂ�Ԃ�</b>\n
	 * @retval -1 �G���[����NULL
	 */
	schema::annotation::annotation_code::ECode GetAnnoCode() const
	{
		return (schema::annotation::annotation_code::ECode)GetLongValueByFieldName( schema::annotation::kAnnoCode, -1 );
	}

	/**
	 * @brief <b>�\���^�C�v��Ԃ�</b>\n
	 * @param emTableType	[in]	�e�[�u���^�C�v
	 * @retval -1 �G���[����NULL
	 * @retval -1�ȊO disptype_code
	 */
	schema::annotation::disp_type::ECode GetDispType( sindyTableType::ECode emTableType ) const
	{
		switch( emTableType )
		{
			case sindyTableType::city_disp_line: return (schema::annotation::disp_type::ECode)GetLongValueByFieldName( schema::annotation::kDispType, -1 );
			case sindyTableType::c_sc1disp_line: return (schema::annotation::disp_type::ECode)GetLongValueByFieldName( schema::annotation::kDispType, -1 );
			case sindyTableType::b_sc4disp_line: 
			case sindyTableType::m_sc4disp_line: 
			case sindyTableType::t_sc4disp_line: return (schema::annotation::disp_type::ECode)GetLongValueByFieldName( schema::annotation::kSC4DispType, -1 );
			case sindyTableType::b_sc3disp_line: 
			case sindyTableType::m_sc3disp_line: 
			case sindyTableType::t_sc3disp_line: return (schema::annotation::disp_type::ECode)GetLongValueByFieldName( schema::annotation::kSC3DispType, -1 );
			case sindyTableType::b_sc2disp_line: 
			case sindyTableType::m_sc2disp_line: 
			case sindyTableType::t_sc2disp_line: return (schema::annotation::disp_type::ECode)GetLongValueByFieldName( schema::annotation::kSC2DispType, -1 );
			case sindyTableType::b_sc1disp_line: 
			case sindyTableType::m_sc1disp_line: 
			case sindyTableType::t_sc1disp_line: return (schema::annotation::disp_type::ECode)GetLongValueByFieldName( schema::annotation::kSC1DispType, -1 );
		}
		return (schema::annotation::disp_type::ECode)-1;
	}
	//@}

	/**
	 * @brief <b>�L����`���v�Z����</b>\n
	 * @param dMarkLen	[in]	�L����Ӓ��i�P�ʁF�ܓx�o�x�j
	 * @return ��`
	 */
	IEnvelopePtr CalcSymbolBounds( double dMarkLenX, double dMarkLenY ) const {
		WKSEnvelope cRect;
		CalcSymbolBounds( dMarkLenX, dMarkLenY, cRect );
		IEnvelopePtr ipEnv( CLSID_Envelope );
		ipEnv->PutCoords( cRect.XMin, cRect.YMin, cRect.XMax, cRect.YMax );
		return ipEnv;
	}
	/**
	 * @brief <b>�L����`���v�Z����</b>\n
	 * @param dMarkLenX	[in]	X�����L����Ӓ��i�P�ʁF�ܓx�o�x�j
	 * @param dMarkLenY	[in]	Y�����L����Ӓ��i�P�ʁF�ܓx�o�x�j
	 * @param cRect		[out]	�L����`
	 * return sindyErrCode
	 */
	errorcode::sindyErrCode CalcSymbolBounds( double dMarkLenX, double dMarkLenY, WKSEnvelope& cRect ) const;

	/**
	 * @brief <b>�\�����[�����擾����</b>\n
	 * ���L���[������K��������̂��������A�Ԃ��܂��B
	 * @param emTableType	[in]	���L�\���ʒu���C���e�[�u���^�C�v
	 * @return �\�����[��
	 */
	schema::annotation_class::scaledisp_rule::ECode CalcDispRule( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>���L�L���̃s�N�Z�������擾����</b>\n
	 * ��ʂ��Ƃ̋L���T�C�Y�͎�ʕ\�ɒ�`����Ă��܂��B
	 * @param emTableType	[in]	���L�\���ʒu���C���e�[�u���^�C�v
	 * @return �s�N�Z�����ie.x. 16 ���A���Ă����� 16x16�̋L����`�ł���Ƃ������Ɓj
	 */
	int CalcMarkSize( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>���L�L���̈�Ӓ����v�Z����</b>\n
	 * @see AheCalcDispLineType21PXLen
	 * @see CalcMarkSize
	 * @param emTableType	[in]	���L�\���ʒu���C���e�[�u���^�C�v
	 * @param bCalcX		[in]	X���W�l���擾����Ȃ�true�AY���W�l���擾����Ȃ�false
	 * @return �L����Ӓ��i�P�ʁF�x�i�ܓx�o�x�j�j
	 */
	double CalcMarkLength( sindyTableType::ECode emTableType, bool bCalcX ) const;

	/**
	 * @brief <b>�\���^�C�v���擾����</b>\n
	 * ���L���[������擾���܂��B
	 * @retval -1 �G���[�i��ʂ����݂��Ȃ��j
	 * @return disptypecode_code::ECode
	 */
	schema::annotation_class::disptype_rule::ECode CalcDispType() const;

	/**
	 * @brief <b>�d�����Ă��ǂ����ǂ���</b>\n
	 * @retval true �d���\
	 * @retval false �d���s�\
	 */
	bool CanOverlap() const;

	/**
	 * @brief <b>���L�L�����o�����Ƃ��ł��邩�ǂ���</b>\n
	 * ���݂̕\���^�C�v�̑����݂̂Ŕ��f���܂��B�\�����[���Ƃ̐�������
	 * �`�F�b�N���܂���B\n
	 * �\�����[���Ƃ̐�������CheckDispTypeCode�Ń`�F�b�N���邱�Ƃ��ł��܂��B
	 * @param emTableType	[in]	�\���ʒu���C���̃^�C�v
	 * @retval true �L���\���\
	 * @retval false �L���\���s��
	 */
	bool CanShowMark( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>���L������݂̂̕\�����ǂ���</b>\n
	 * ���݂̕\���^�C�v�̑����݂̂Ŕ��f���܂��B�\�����[���Ƃ̐�������
	 * �`�F�b�N���܂���B\n
	 * �\�����[���Ƃ̐�������CheckDispTypeCode�Ń`�F�b�N���邱�Ƃ��ł��܂��B
	 * @param emTableType	[in]	�\���ʒu���C���̃^�C�v
	 * @retval true ������݂̂̕\��
	 * @retval false ������݂̂̕\���ł͂Ȃ�
	 */
	bool CanShowStrOnly( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>�\������ׂ��X�P�[���l���擾����</b>\n
	 * @param nLevel	[in]	0:S4�i�s�s�n�}��0�̂݁j 1:S3 2:S2 3:S1
	 * @retval -1 �G���[
	 * @retval -1�ȊO �X�P�[��
	 */
	sindyAnnotationScale GetDispScale( int nLevel ) const;

	/**
	 * @brief <b>���[���L���b�V����Ԃ�</b>\n
	 * �n���S�w���n��ƒn���ƂňقȂ郋�[�����g�p���邽�߁A���[���̃L���b�V����
	 * 2���邱�Ƃւ̑Ή��ł��B
	 * @retval NULL		�G���[
	 * @retval NULL�ȊO	���[�����R�[�h�I�u�W�F�N�g�̃|�C���^
	 */
	const annotation_class::CAnnotationClass* GetRule() const;


protected:
	mutable annotation_class::CAnnotationClass*	m_pAnnoRule;			//!< ���L���[��
	mutable annotation_class::CAnnotationClass*	m_pAnnoRule2;			//!< ���L���[���i�n���S�w�i�n��j����ђn���S�w�i�n���j�̓X�P�[���ɂ���ă��[�����ς�邽�߁A���̂��߂̃��[���j <- �Ȃ�Ƃ����ā[
};

} // sindy

#endif // _ANNOTATION_H_
