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
 * @file WorkPurpose.h
 * @brief <b>CWorkPurpose�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _WORKPURPOSE_H_
#define _WORKPURPOSE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace workpurpose {
/**
 * @class CWorkPurpose
 * @brief <b>��ƖړI�Ǘ����R�[�h�N���X</b>\n
 * ���̃N���X�́AREFERENCE.WORKPURPOSE�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API CWorkPurpose : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CWorkPurpose, workpurpose )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>��ƖړI�����擾����</b>\n
	 * @param nLen		[in]	lpszName�̎��[�\������
	 * @param lpszPurpose	[out]	��ƖړI�����i�[����o�b�t�@
	 * @retval LPCTSTR ��ƖړI��
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetPurpose( int nLen, LPTSTR lpszPurpose ) const {
		return GetStringValueByFieldName( schema::workpurpose::kPurpose, nLen, lpszPurpose );
	}

	/**
	 * @brief <b>��ƖړIID���擾����</b>\n
	 * @retval 0�ȏ� ��ƖړIID
	 * @retval 0���� �G���[����NULL
	 */
	long GetPurposeID() const {
		return GetLongValueByFieldName( schema::workpurpose::kPurposeID, -1 );
	}

	/**
	 * @brief <b>MXD�t�@�C�������擾����</b>\n
	 * @param nLen			[in]			lpszMXD�̎��[�\������
	 * @param lpszMXD		[out]			�擾��������i�[����o�b�t�@
	 * @retval LPCTSTR MXD�t�@�C����
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetMXD( int nLen, LPTSTR lpszMXD ) const {
		return GetStringValueByFieldName( schema::workpurpose::kMXD, nLen, lpszMXD );
	}

	/**
	 * @brief <b>���������t�^�l���擾����</b>\n
	 * @param nLen					[in]			lpszAutoAttrGiving�̎��[�\������
	 * @param lpszAutoAttrGiving	[out]			�擾��������i�[����o�b�t�@
	 * @retval LPCTSTR ���������t�^�l
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetAutoAttrGiving( int nLen, LPTSTR lpszAutoAttrGiving ) const {
		return GetStringValueByFieldName( schema::workpurpose::kAutoAttrGiving, nLen, lpszAutoAttrGiving );
	}

	/**
	 * @brief <b>��ƖړI��ނ��擾����</b>\n
	 * @retval 0�ȏ� ��ƖړI���
	 * @retval -1 �G���[����NULL
	 */
	long GetWorkKind() const {
		return GetLongValueByFieldName( schema::workpurpose::kWorkKind, -1 );
	}

	/**
	 * @brief <b>���n�������f�p���ǂ����̃t���O���擾����</b>\n
	 * @note ���n�������f�p�̍�ƖړI�ŕҏW���s���ƁA���C�u�������Ŏ����I��
	 * ���n�����t���O�Ƀ`�F�b�N�����܂��B
	 * @retval true ���n�������f�p
	 * @retval false �G���[����NULL���͌��n�������f�p�ł͂Ȃ�
	 */
	bool GetGencho() const {
		return GetBoolValueByFieldName( schema::workpurpose::kGencho, false );
	}

	/**
	 * @brief <b>�N���[���C���p���ǂ����̃t���O���擾����</b>\n
	 * @note �N���[���C���p�̍�ƖړI�ŕҏW���s���ƁA���C�u�������Ŏ����I��
	 * �N���[���t���O�Ƀ`�F�b�N�����܂��B
	 * @retval true �N���[���C���p
	 * @retval false �G���[����NULL���̓N���[���C���p�ł͂Ȃ�
	 */
	bool GetClaim() const {
		return GetBoolValueByFieldName( schema::workpurpose::kClaim, false );
	}

	/**
	 * @brief <b>���L�p���ǂ����̃t���O���擾����</b>\n
	 * @note ���L�p�̍�Ƃł͎g�p���Ă͂����Ȃ��c�[���Ɋւ��ẮA�����̒l���`�F�b�N����
	 * �{�^����Enable/Disable�𑀍삷��K�v������܂��B
	 * @retval true ���L�p
	 * @retval false �G���[����NULL���͒��L�p�ł͂Ȃ�
	 */
	bool GetChuki() const {
		return GetBoolValueByFieldName( schema::workpurpose::kChuki, false );
	}

	/**
	 * @brief <b>�폜�ς݂��ǂ����̃t���O���擾����</b>\n
	 * @note ��ƖړI���R�[�h�͎��ۂɂ͍폜����邱�Ƃ͂Ȃ��A�ւ��ɂ��̃t���O�������܂��B
	 * @retval true �폜�ς�
	 * @retval false �G���[����NULL���͍폜����Ă��Ȃ�
	 */
	bool GetDelete() const {
		return GetBoolValueByFieldName( schema::workpurpose::kDelete, false );
	}

	/**
	 * @brief <b>�e�X�g�p���ǂ����̃t���O���擾����</b>\n
	 * @note ���̃t���O�͕��i�͕\�ɏo�܂���B
	 * @retval true �e�X�g�p
	 * @retval false �G���[����NULL���̓e�X�g�p�ł͂Ȃ�
	 */
	bool GetTest() const {
		return GetBoolValueByFieldName( schema::workpurpose::kTest, false );
	}

	/**
	 * @brief <b>�J�e�S�������擾����</b>\n
	 * @param nLen			[in]			lpszCategory�̎��[�\������
	 * @param lpszCategory	[out]			�J�e�S�������i�[����o�b�t�@
	 * @retval LPCTSTR �J�e�S����
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetCategory( int nLen, LPTSTR lpszCategory ) const {
		return GetStringValueByFieldName( schema::workpurpose::kCategory, nLen, lpszCategory );
	}

	/**
	 * @brief <b>���C���t�@�C���p�X���擾����</b>\n
	 * @note �ȑO�̓o�b�`�ŏ������Ă��܂������A���[���Ɏ�荞�݂܂����B
	 * @param nLen			[in]			lpszLayerFilePath�̎��[�\������
	 * @param lpszLayerFilePath	[out]			���C���t�@�C���p�X���i�[����o�b�t�@
	 * @retval LPCTSTR ���C���t�@�C���p�X
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetLayerFilePath( int nLen, LPTSTR lpszLayerFilePath ) const {
		return GetStringValueByFieldName( schema::workpurpose::kLayerFilePath, nLen, lpszLayerFilePath );
	}

	/**
	 * @brief <b>�ۑ��s�t���O���擾����</b>\n
	 * @note �ȑO�̓}�N���Ŏ������Ă��܂������A���[���Ɏ�荞�݂܂����B
	 * @retval true �ۑ��\�ȍ�ƖړI�i�f�t�H���g�j
	 * @retval false �ۑ��s�\�ȍ�ƖړI
	 */
	bool GetCanSave() const {
		return GetBoolValueByFieldName( schema::workpurpose::kCanSave, true );
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>��ƖړI����ݒ肷��</b>\n
	 * @param lpcszPurpose	[out]	��ƖړI��
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetPurpose( LPCTSTR lpcszPurpose ) {
		return SetStringValueByFieldName( schema::workpurpose::kPurpose, lpcszPurpose );
	}

	/**
	 * @brief <b>MXD�t�@�C������ݒ肷��</b>\n
	 * @param lpcszMXD		[in]			MXD�t�@�C����
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetMXD( LPCTSTR lpcszMXD ) {
		return SetStringValueByFieldName( schema::workpurpose::kMXD, lpcszMXD );
	}

	/**
	 * @brief <b>MXD�t�@�C������ݒ肷��</b>\n
	 * @param lpcszMXD		[in]			MXD�t�@�C����
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetAutoAttrGiving( LPCTSTR lpcszAutoAttrGiving ) {
		return SetStringValueByFieldName( schema::workpurpose::kAutoAttrGiving, lpcszAutoAttrGiving );
	}

	/**
	 * @brief <b>��ƖړI��ނ�ݒ肷��</b>\n
	 * @param lKind			[in]	��ƖړI���
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetWorkKind( long lKind ) {
		return SetLongValueByFieldName( schema::workpurpose::kWorkKind, lKind );
	}

	/**
	 * @brief <b>���n�������f�p���ǂ����̃t���O��ݒ肷��</b>\n
	 * @note ���n�������f�p�̍�ƖړI�ŕҏW���s���ƁA���C�u�������Ŏ����I��
	 * ���n�����t���O�Ƀ`�F�b�N�����܂��B
	 * @param bFlag		[in]	���n�������f�p���ǂ����̃t���O
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetGencho( bool bFlag ) {
		return SetBoolValueByFieldName( schema::workpurpose::kGencho, bFlag );
	}

	/**
	 * @brief <b>�N���[���C���p���ǂ����̃t���O��ݒ肷��</b>\n
	 * @note �N���[���C���p�̍�ƖړI�ŕҏW���s���ƁA���C�u�������Ŏ����I��
	 * �N���[���t���O�Ƀ`�F�b�N�����܂��B
	 * @param bFlag		[in]	�N���[���C���p���ǂ����̃t���O
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetClaim( bool bFlag ) {
		return SetBoolValueByFieldName( schema::workpurpose::kClaim, bFlag );
	}

	/**
	 * @brief <b>���L�p���ǂ����̃t���O��ݒ肷��</b>\n
	 * @note ���L�p�̍�Ƃł͎g�p���Ă͂����Ȃ��c�[���Ɋւ��ẮA�����̒l���`�F�b�N����
	 * �{�^����Enable/Disable�𑀍삷��K�v������܂��B
	 * @param bFlag		[in]	���L�p���ǂ����̃t���O
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetChuki( bool bFlag ) {
		return SetBoolValueByFieldName( schema::workpurpose::kChuki, bFlag );
	}

	/**
	 * @brief <b>��ƖړIID��ݒ肷��</b>\n
	 * @param lPurposeID	[in]	��ƖړIID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetPurposeID( long lPurposeID ) {
		return SetLongValueByFieldName( schema::workpurpose::kPurposeID, lPurposeID );
	}

	/**
	 * @brief <b>�폜�ς݂��ǂ����̃t���O��ݒ肷��</b>\n
	 * @note ��ƖړI���R�[�h�͎��ۂɂ͍폜����邱�Ƃ͂Ȃ��A�ւ��ɂ��̃t���O�������܂��B
	 * @param bFlag		[in]	�폜�ς݂��ǂ����̃t���O
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetDelete( bool bFlag ) {
		return SetBoolValueByFieldName( schema::workpurpose::kDelete, bFlag );
	}

	/**
	 * @brief <b>�e�X�g�p���ǂ����̃t���O��ݒ肷��</b>\n
	 * @note ���̃t���O�͕��i�͕\�ɏo�܂���B
	 * @param bFlag		[in]	�e�X�g�p���ǂ����̃t���O
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetTest( bool bFlag ) {
		return SetBoolValueByFieldName( schema::workpurpose::kTest, bFlag );
	}

	/**
	 * @brief <b>�J�e�S������ݒ肷��</b>\n
	 * @param lpcszCategory	[in]		�J�e�S����
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetCategory( LPCTSTR lpcszCategory ) {
		return SetStringValueByFieldName( schema::workpurpose::kCategory, lpcszCategory );
	}
	//@}
};

} // workpurpose

} // sindy

#endif // _WORKPURPOSE_H_
