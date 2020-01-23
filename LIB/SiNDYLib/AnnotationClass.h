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
 * @file AnnotationClass.h
 * @brief CAnnotationClass�N���X��`�t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _ANNOTATIONCLASS_H_
#define _ANNOTATIONCLASS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "AnnotationCommunity.h"
#include "RuleCommunity.h"

namespace sindy {
namespace annotation_class {
/**
 * @class CAnnotationClass
 * @brief <b>���L���[�����R�[�h���b�p�[�N���X</b>\n
 * ���̃N���X�͒��L���[�����R�[�h�iANNOTATION_CLASS�j�̃��b�p�[�N���X�ł��B
 * ���̃N���X��ʂ��āA�S�Ẵt�B�[���h�l�̎擾�E�ݒ肪�\�ł��B
 * �Ȃ��A���̃N���X�ɂ͂��̃N���X���L�̃��\�b�h������`����Ă��܂��񂪁A
 * �p�����̃N���X�ł���ȊO�̂��ׂẴ��\�b�h����`����Ă��܂��B
 */
class SINDYLIB_API CAnnotationClass : public CRow  
{
public:
	ROWCLASS_CONSTRUCTOR( CAnnotationClass, annotation_class )
	//@{ @name �����擾�֐�
	/**
	 * @brief SiNDY���L��ʃR�[�h���擾����
	 * @retval NULL�̎���-1
	 * @retval NULL�ȊO�̎���annotation_code::ECode
	 */
	 schema::annotation::annotation_code::ECode GetClassCode() const
	{
		return (schema::annotation::annotation_code::ECode)GetLongValueByFieldName( schema::annotation_class::kClass, -1 );
	}
	
	/**
	 * @brief ���e�ԍ����擾����
	 * @param nLen		[in]	lpszMsNo�̎��[�\������
	 * @param lpszMsNo	[out]	���e�ԍ��i�[�o�b�t�@
	 * @return ���e�ԍ�������
	 */
	LPCTSTR GetMSNo( int nLen, LPTSTR lpszMsNo ) const
	{
		return GetStringValueByFieldName( schema::annotation_class::kMSNo, nLen, lpszMsNo );
	}

	/**
	 * @brief IN���L��ʃR�[�h���擾����
	 * @param nLen		[in]	lpszClass�̎��[�\������
	 * @param lpszClass	[out]	IN���L��ʃR�[�h�i�[�o�b�t�@
	 * @return IN���L��ʃR�[�h������
	 */
	LPCTSTR GetINClass( int nLen, LPTSTR lpszClass ) const
	{
		return GetStringValueByFieldName( schema::annotation_class::kInClass, nLen, lpszClass );
	}

	/**
	 * @brief ��ʖ��̂��擾����
	 * @param nLen			[in]	lpszClassName�̎��[�\������
	 * @param lpszClassName	[out]	��ʖ��̊i�[�o�b�t�@
	 * @return ��ʖ��̕�����
	 */
	LPCTSTR GetClassName( int nLen, LPTSTR lpszClassName ) const
	{
		return GetStringValueByFieldName( schema::annotation_class::kClassName, nLen, lpszClassName );
	}

	/**
	 * @brief TOP_FLAG���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetTopFlag() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kTopFlag, false );
	}

	/**
	 * @brief MIDDLE_FLAG���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetMiddleFlag() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kMiddleFlag, false );
	}

	/**
	 * @brief BASE_FLAG���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetBaseFlag() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kBaseFlag, false );
	}

	/**
	 * @brief CITY_FLAG���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetCityFlag() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kCityFlag, false );
	}

	/**
	 * @brief �\���^�C�v�R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::disptype_rule::ECode GetDispType() const
	{
		return (schema::annotation_class::disptype_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kDispType, -1 );
	}

	/**
	 * @brief �g�b�v�}�b�vS1�\�����[���R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetTS1DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kTS1DispRule, -1 );
	}

	/**
	 * @brief �g�b�v�}�b�vS2�\�����[���R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetTS2DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kTS2DispRule, -1 );
	}

	/**
	 * @brief �g�b�v�}�b�vS3�\�����[���R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetTS3DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kTS3DispRule, -1 );
	}

	/**
	 * @brief �g�b�v�}�b�vS4�\�����[���R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetTS4DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kTS4DispRule, -1 );
	}

	/**
	 * @brief �g�b�v�}�b�v�^�ʒu�t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetTTruePosition() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kTTruePosition, false );
	}

	/**
	 * @brief �g�b�v�}�b�v�t�H���g�T�C�Y���擾����
	 * @retval -1 NULL�܂��͎擾���s
	 */
	int GetTFontSize() const
	{
		return GetLongValueByFieldName( schema::annotation_class::kTFontSize, -1 );
	}

	/**
	 * @brief �g�b�v�}�b�v�X�P�[���̒������t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetTExceptScale() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kTExceptScale, false );
	}

	/**
	 * @brief �g�b�v�}�b�v�Q�s���L�t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetTDoubleAnno() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kTDoubleAnno, false );
	}

	/**
	 * @brief �g�b�v�}�b�v�R�s�[�y�[�X�g�t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetTCopyPaste() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kTCopyPaste, false );
	}

	/**
	 * @brief �~�h���}�b�vS1�\�����[���R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetMS1DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kMS1DispRule, -1 );
	}

	/**
	 * @brief �~�h���}�b�vS2�\�����[���R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetMS2DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kMS2DispRule, -1 );
	}

	/**
	 * @brief �~�h���}�b�vS3�\�����[���R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetMS3DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kMS3DispRule, -1 );
	}

	/**
	 * @brief �~�h���}�b�vS4�\�����[���R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetMS4DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kMS4DispRule, -1 );
	}

	/**
	 * @brief �~�h���}�b�v�^�ʒu�t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetMTruePosition() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kMTruePosition, false );
	}

	/**
	 * @brief �~�h���}�b�v�t�H���g�T�C�Y���擾����
	 * @retval -1 NULL�܂��͎擾���s
	 */
	int GetMFontSize() const
	{
		return GetLongValueByFieldName( schema::annotation_class::kMFontSize, -1 );
	}

	/**
	 * @brief �~�h���}�b�v�X�P�[���̒������t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetMExceptScale() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kMExceptScale, false );
	}

	/**
	 * @brief �~�h���}�b�v�Q�s���L�t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetMDoubleAnno() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kMDoubleAnno, false );
	}

	/**
	 * @brief �~�h���}�b�v�R�s�[�y�[�X�g�t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetMCopyPaste() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kMCopyPaste, false );
	}

	/**
	 * @brief �~�h���}�b�v����f�[�^�̑��݃t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetMIDenticalData() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kMIDenticalData, false );
	}

	/**
	 * @brief �g�b�v�}�b�v����f�[�^�̑��݃t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetTIDenticalData() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kTIDenticalData, false );
	}

	/**
	 * @brief �x�[�X�}�b�vS1�\�����[���R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetBS1DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kBS1DispRule, -1 );
	}

	/**
	 * @brief �x�[�X�}�b�vS2�\�����[���R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetBS2DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kBS2DispRule, -1 );
	}

	/**
	 * @brief �x�[�X�}�b�vS3�\�����[���R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetBS3DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kBS3DispRule, -1 );
	}

	/**
	 * @brief �x�[�X�}�b�vS4�\�����[���R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetBS4DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kBS4DispRule, -1 );
	}

	/**
	 * @brief �x�[�X�}�b�v�^�ʒu�t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetBTruePosition() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kBTruePosition, false );
	}

	/**
	 * @brief �x�[�X�}�b�v�t�H���g�T�C�Y���擾����
	 * @retval -1 NULL�܂��͎擾���s
	 */
	int GetBFontSize() const
	{
		return GetLongValueByFieldName( schema::annotation_class::kBFontSize, -1 );
	}

	/**
	 * @brief �x�[�X�}�b�v�X�P�[���̒������t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetBExceptScale() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kBExceptScale, false );
	}

	/**
	 * @brief �x�[�X�}�b�v�Q�s���L�t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetBDoubleAnno() const
	{
		return GetBoolValueByFieldName(	schema::annotation_class::kBDoubleAnno, false );
	}

	/**
	 * @brief �x�[�X�}�b�v�R�s�[�y�[�X�g�t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetBCopyPaste() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kBCopyPaste, false );
	}

	/**
	 * @brief �x�[�X�}�b�v����f�[�^�̑��݃t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetBIDenticalData() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kBIDenticalData, false );
	}

	/**
	 * @brief �s�s�n�}�\�����[���R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetCDispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kCDispRule, -1 );
	}

	/**
	 * @brief �s�s�n�}�^�ʒu�t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetCTruePosition() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kCTruePosition, false );
	}

	/**
	 * @brief �s�s�n�}�t�H���g�T�C�Y���擾����
	 * @retval -1 NULL�܂��͎擾���s
	 */
	int GetCFontSize() const
	{
		return GetLongValueByFieldName( schema::annotation_class::kCFontSize, -1 );
	}

	/**
	 * @brief �s�s�n�}�X�P�[���̒������t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetCExceptScale() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kCExceptScale, false );
	}

	/**
	 * @brief �s�s�n�}�Q�s���L�t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetCDoubleAnno() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kCDoubleAnno, false );
	}

	/**
	 * @brief �s�s�n�}�R�s�[�y�[�X�g�t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetCCopyPaste() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kCCopyPaste, false );
	}

	/**
	 * @brief �s�s�n�}����f�[�^�̑��݃t���O���擾����
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetCIDenticalData() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kCIDenticalData, false );
	}

	/**
	 * @brief ��]���[����������擾����
	 * @param nLen			[in]	lpszRule�̎��[�\������
	 * @param lpszRule		[out]	��]���[��������i�[�o�b�t�@
	 * @return ��]���[��������
	 */
	LPCTSTR GetRotationRule( int nLen, LPTSTR lpszRule ) const
	{
		return GetStringValueByFieldName( schema::annotation_class::kRotationRule, nLen, lpszRule );
	}
	//@}

	/**
	 * @brief �\���X�P�[���ɂ������\�����[���R�[�h���擾����
	 * @return �G���[����NULL�̏ꍇ��-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetDispRule( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief �t�H���g�T�C�Y���擾����
	 * @param emTableType	[in]	�^�ʒu���͕\�������񃌃C���̃e�[�u���^�C�v
	 * @retval -1 NULL�܂��͎擾���s
	 */
	int GetFontSize( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>�\���X�P�[���ɍ������R�s�[�y�[�X�g�t���O���擾����</b>\n
	 * @param emTableType	[in]	�^�ʒu���͕\�������񃌃C���̃e�[�u���^�C�v
	 * @retval false �R�s�[�y�[�X�g�s�A���͒l��NULL�A�܂��͎擾���s
	 * @retval true �R�s�[�y�[�X�g��
	 */
	bool GetCopyPaste( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>�\���X�P�[���ɍ������������t���O���擾����</b>\n
	 * @param emTableType	[in]	�^�ʒu���͕\�������񃌃C���̃e�[�u���^�C�v
	 * @retval false �������s�A���͒l��NULL�A�܂��͎擾���s
	 * @retval true ��������
	 */
	bool GetExceptScale( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief ����f�[�^�̑��݃t���O���擾����
	 * @param emTableType	[in]	�^�ʒu���͕\�������񃌃C���̃e�[�u���^�C�v
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetIDenticalData( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief �^�ʒu�t���O���擾����
	 * @param emTableType	[in]	�^�ʒu���͕\�������񃌃C���̃e�[�u���^�C�v
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetTruePosition( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief �Q�s���L���t���O���擾����
	 * @param emTableType	[in]	�^�ʒu���͕\�������񃌃C���̃e�[�u���^�C�v
	 * @retval false 0����NULL�A�܂��͎擾���s
	 */
	bool GetDoubleAnno( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>�n���ꂽ�e�[�u���^�C�v�Ŏg�p�\�ȃ��[�����ǂ������`�F�b�N����</b>\n
	 * @retval true �g�p�\�ȃ��[��
	 * @retval false �g�p�s�\�ȃ��[��
	 */
	bool IsValidRule( sindyTableType::ECode emTableType ) const;

};

} // annotation_class

} // sindy

#endif // _ANNOTATIONCLASS_H_
