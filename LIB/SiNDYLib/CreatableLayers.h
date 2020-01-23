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
 * @file CreatableLayers.h
 * @brief <b>CCreatableLayers�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _CREATABLELAYERS_H_
#define _CREATABLELAYERS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"
#include "AutoValue.h"

namespace sindy {
namespace creatablelayers {
/**
 * @class CCreatableLayers
 * @brief <b>�쐬�\���C���Ǘ��e�[�u���N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.MOVABLELAYERS�e�[�u���̃��b�v�N���X�ł��B
 */
class SINDYLIB_API CCreatableLayers : public CRow
{
public:
	ROWCLASS_CONSTRUCTOR( CCreatableLayers, creatablelayers )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>��ƖړIID���擾����</b>\n
	 * @retval 0�ȏ� ��ƖړIID
	 * @retval -1 �G���[����NULL
	 */
	long GetPurposeID() const {
		return GetLongValueByFieldName( schema::creatablelayers::kPurposeID, -1 );
	}

	/**
	 * @brief <b>���C����ID���擾����</b>\n
	 * @retval 0�ȏ� ���C����ID
	 * @retval -1 �G���[����NULL
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::creatablelayers::kLayerNameID, -1 );
	}

	/**
	 * @brief <b>�����쐬�t���O���擾����</b>\n
	 * @retval false NULL�A�G���[�A0
	 * @retval true 0�ȊO
	 */
	bool GetAutoFlag() const {
		return GetBoolValueByFieldName( schema::creatablelayers::kAutoFlag, false );
	}

	/**
	 * @brief <b>�����쐬�����l���擾����</b>\n
	 * @note �Ȃ�ׂ�CAutoValue��Ԃ��ق����g�p���Ă��������B
	 * @param nLen		[in]	�o�b�t�@�i�[�\�����񐔁i_AUTOVALUE_MAX_LEN���g�p���Ă��������j
	 * @param lpszValue	[out]	�i�[�o�b�t�@
	 * @retval LPCTSTR �����쐬�����l
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetAutoValue( int nLen, LPTSTR lpszValue ) const {
		return GetStringValueByFieldName( schema::creatablelayers::kAutoValue, nLen, lpszValue );
	}
	/**
	 * @brief <b>�����쐬�����l���擾����</b>\n
	 * @return CAutoValue
	 */
	const CAutoValue& GetAutoValue() const {
		return m_cAutoValue;
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>��ƖړIID��ݒ肷��</b>\n
	 * @param lPurposeID	[in]	��ƖړIID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetPurposeID( long lPurposeID ) {
		return SetLongValueByFieldName( schema::creatablelayers::kPurposeID, lPurposeID );
	}

	/**
	 * @brief <b>���C����ID��ݒ肷��</b>\n
	 * @param lLayerNameID	[in]	���C����ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long lLayerNameID ) {
		return SetLongValueByFieldName( schema::creatablelayers::kLayerNameID, lLayerNameID );
	}

	/**
	 * @brief <b>�����쐬�t���O��ݒ肷��</b>\n
	 * @param bAutoFlag		[in]	�����쐬�t���O
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetAutoFlag( bool bAutoFlag ) {
		return SetBoolValueByFieldName( schema::creatablelayers::kAutoFlag, bAutoFlag );
	}

	/**
	 * @brief <b>�����쐬�����l��ݒ肷��</b>\n
	 * @param lpcszValue	[in]	�����쐬�����l
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetAutoValue( LPCTSTR lpcszValue ) {
		m_cAutoValue.Init( lpcszValue );
		return SetStringValueByFieldName( schema::creatablelayers::kAutoValue, lpcszValue );
	}
	//@}
private:
	CAutoValue m_cAutoValue; //!< �����t�^����
};

} // creatablelayers

} // sindy

#endif // _CREATABLELAYERS_H_
