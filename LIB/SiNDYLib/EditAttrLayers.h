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
 * @file EditAttrLayers.h
 * @brief <b>CEditAttrLayers�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _EDITATTRLAYERS_H_
#define _EDITATTRLAYERS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AbleLayers.h"
#include "RuleCommunity.h"

namespace sindy {
namespace editattrlayers {

/**
 * @class CEditAttrLayers
 * @brief <b>�����ҏW�\���C�����R�[�h�N���X�i���[���p�j</b>\n
 * ���̃N���X�́AREFERENCE.EDITATTRLAYERS�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API CEditAttrLayers : public CAbleLayers  
{
	ROWCLASS_CONSTRUCTOR( CEditAttrLayers, editattrlayers )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>��ƖړIID���擾����</b>\n
	 * @retval 0�ȏ� ��ƖړIID
	 * @retval -1 �G���[����NULL
	 */
	long GetPurposeID() const {
		return GetLongValueByFieldName( schema::editattrlayers::kPurposeID, -1 );
	}

	/**
	 * @brief <b>���C����ID���擾����</b>\n
	 * @retval 0�ȏ� ���C����ID
	 * @retval -1 �G���[����NULL
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::editattrlayers::kLayerNameID, -1 );
	}

	/**
	 * @brief <b>��������1���擾����</b>\n
	 * @param nLen			[in]	lpszRistrict�̎��[�\������
	 * @param lpszRistrict	[out]	�擾��������i�[����o�b�t�@
	 * @retval LPCTSTR ��������
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetRistrict1( int nLen, LPTSTR lpszRistrict ) const {
		return GetStringValueByFieldName( schema::editattrlayers::kRistrict1, nLen, lpszRistrict );
	}

	/**
	 * @brief <b>��������2���擾����</b>\n
	 * @param nLen			[in]	lpszRistrict�̎��[�\������
	 * @param lpszRistrict	[out]	�擾��������i�[����o�b�t�@
	 * @retval LPCTSTR ��������
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetRistrict2( int nLen, LPTSTR lpszRistrict ) const {
		return GetStringValueByFieldName( schema::editattrlayers::kRistrict2, nLen, lpszRistrict );
	}

	/**
	 * @brief <b>��������3���擾����</b>\n
	 * @param nLen			[in]	lpszRistrict�̎��[�\������
	 * @param lpszRistrict	[out]	�擾��������i�[����o�b�t�@
	 * @retval LPCTSTR ��������
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetRistrict3( int nLen, LPTSTR lpszRistrict ) const {
		return GetStringValueByFieldName( schema::editattrlayers::kRistrict3, nLen, lpszRistrict );
	}

	/**
	 * @brief <b>��������4���擾����</b>\n
	 * @param nLen			[in]	lpszRistrict�̎��[�\������
	 * @param lpszRistrict	[out]	�擾��������i�[����o�b�t�@
	 * @retval LPCTSTR ��������
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetRistrict4( int nLen, LPTSTR lpszRistrict ) const {
		return GetStringValueByFieldName( schema::editattrlayers::kRistrict4, nLen, lpszRistrict );
	}

	/**
	 * @brief <b>��������5���擾����</b>\n
	 * @param nLen			[in]	lpszRistrict�̎��[�\������
	 * @param lpszRistrict	[out]	�擾��������i�[����o�b�t�@
	 * @retval LPCTSTR ��������
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetRistrict5( int nLen, LPTSTR lpszRistrict ) const {
		return GetStringValueByFieldName( schema::editattrlayers::kRistrict5, nLen, lpszRistrict );
	}

	/**
	 * @brief <b>��������6���擾����</b>\n
	 * @param nLen			[in]	lpszRistrict�̎��[�\������
	 * @param lpszRistrict	[out]	�擾��������i�[����o�b�t�@
	 * @retval LPCTSTR ��������
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetRistrict6( int nLen, LPTSTR lpszRistrict ) const {
		return GetStringValueByFieldName( schema::editattrlayers::kRistrict6, nLen, lpszRistrict );
	}

	/**
	 * @brief <b>��������7���擾����</b>\n
	 * @param nLen			[in]	lpszRistrict�̎��[�\������
	 * @param lpszRistrict	[out]	�擾��������i�[����o�b�t�@
	 * @retval LPCTSTR ��������
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetRistrict7( int nLen, LPTSTR lpszRistrict ) const {
		return GetStringValueByFieldName( schema::editattrlayers::kRistrict7, nLen, lpszRistrict );
	}

	/**
	 * @brief <b>��������8���擾����</b>\n
	 * @param nLen			[in]	lpszRistrict�̎��[�\������
	 * @param lpszRistrict	[out]	�擾��������i�[����o�b�t�@
	 * @retval LPCTSTR ��������
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetRistrict8( int nLen, LPTSTR lpszRistrict ) const {
		return GetStringValueByFieldName( schema::editattrlayers::kRistrict8, nLen, lpszRistrict );
	}

	/**
	 * @brief <b>��������9���擾����</b>\n
	 * @param nLen			[in]	lpszRistrict�̎��[�\������
	 * @param lpszRistrict	[out]	�擾��������i�[����o�b�t�@
	 * @retval LPCTSTR ��������
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetRistrict9( int nLen, LPTSTR lpszRistrict ) const {
		return GetStringValueByFieldName( schema::editattrlayers::kRistrict9, nLen, lpszRistrict );
	}

	/**
	 * @brief <b>��������10���擾����</b>\n
	 * @param nLen			[in]	lpszRistrict�̎��[�\������
	 * @param lpszRistrict	[out]	�擾��������i�[����o�b�t�@
	 * @retval LPCTSTR ��������
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetRistrict10( int nLen, LPTSTR lpszRistrict ) const {
		return GetStringValueByFieldName( schema::editattrlayers::kRistrict10, nLen, lpszRistrict );
	}

	//@}

	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>��ƖړIID��ݒ肷��</b>\n
	 * @param lPurposeID	[in]	��ƖړIID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetPurposeID( long lPurposeID ) {
		return SetLongValueByFieldName( schema::editattrlayers::kPurposeID, lPurposeID );
	}

	/**
	 * @brief <b>���C����ID��ݒ肷��</b>\n
	 * @param lLayerNameID	[in]	���C����ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long lLayerNameID ) {
		return SetLongValueByFieldName( schema::editattrlayers::kLayerNameID, lLayerNameID );
	}

	/**
	 * @brief <b>��������1��ݒ肷��</b>\n
	 * @param lpcszRestrict	[in]	��������������
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict1( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::editattrlayers::kRistrict1, lpcszRestrict );
	}

	/**
	 * @brief <b>��������2��ݒ肷��</b>\n
	 * @param lpcszRestrict	[in]	��������������
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict2( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::editattrlayers::kRistrict2, lpcszRestrict );
	}

	/**
	 * @brief <b>��������3��ݒ肷��</b>\n
	 * @param lpcszRestrict	[in]	��������������
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict3( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::editattrlayers::kRistrict3, lpcszRestrict );
	}

	/**
	 * @brief <b>��������4��ݒ肷��</b>\n
	 * @param lpcszRestrict	[in]	��������������
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict4( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::editattrlayers::kRistrict4, lpcszRestrict );
	}

	/**
	 * @brief <b>��������5��ݒ肷��</b>\n
	 * @param lpcszRestrict	[in]	��������������
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict5( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::editattrlayers::kRistrict5, lpcszRestrict );
	}

	/**
	 * @brief <b>��������6��ݒ肷��</b>\n
	 * @param lpcszRestrict	[in]	��������������
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict6( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::editattrlayers::kRistrict6, lpcszRestrict );
	}

	/**
	 * @brief <b>��������7��ݒ肷��</b>\n
	 * @param lpcszRestrict	[in]	��������������
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict7( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::editattrlayers::kRistrict7, lpcszRestrict );
	}

	/**
	 * @brief <b>��������8��ݒ肷��</b>\n
	 * @param lpcszRestrict	[in]	��������������
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict8( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::editattrlayers::kRistrict8, lpcszRestrict );
	}

	/**
	 * @brief <b>��������9��ݒ肷��</b>\n
	 * @param lpcszRestrict	[in]	��������������
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict9( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::editattrlayers::kRistrict9, lpcszRestrict );
	}

	/**
	 * @brief <b>��������10��ݒ肷��</b>\n
	 * @param lpcszRestrict	[in]	��������������
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict10( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::editattrlayers::kRistrict10, lpcszRestrict );
	}
	//@}
};

} // editattrlayers

} // sindy

#endif // _EDITATTRLAYERS_H_