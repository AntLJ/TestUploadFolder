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
 * @file LayerNames.h
 * @brief <b>CLayerNames�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _LAYERNAMES_H_
#define _LAYERNAMES_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace layernames {
/**
 * @class CLayerNames
 * @brief <b>���C�����Ǘ����R�[�h�N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.LAYERNAMES�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API CLayerNames : public CRow
{
	ROWCLASS_CONSTRUCTOR( CLayerNames, layernames )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>���C�������擾����</b>\n
	 * @param nLen		[in]	lpszName�̎��[�\������
	 * @param lpszName	[out]	�̈���m�ۂ���Ă���o�b�t�@
	 * @return �G���[����NULL�̏ꍇ�͋󕶎���
	 */
	LPCTSTR GetLayerName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::layernames::kLayerName, nLen, lpszName );
	}
	/**
	 * @brief <b>���C����ID���擾����</b>\n
	 * @retval 0�ȏ� ���C����ID
	 * @retval -1 �G���[���͓o�^����Ă��Ȃ�
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::layernames::kLayerNameID, -1 );
	}
	/**
	 * @brief <b>���C���G�C���A�X�����擾����</b>\n
	 * @param nLen		[in]	lpszName�̎��[�\������
	 * @param lpszName	[out]	�̈���m�ۂ���Ă���o�b�t�@
	 * @return �G���[����NULL�̏ꍇ�͋󕶎���
	 */
	LPCTSTR GetLayerAliasName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::layernames::kLayerAliasName, nLen, lpszName );
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>���C������ݒ肷��</b>\n
	 * @param lpcszName	[in]	�ݒ肷�郌�C����
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::layernames::kLayerName, lpcszName );
	}
	/**
	 * @brief <b>���C����ID��ݒ肷��</b>\n
	 * @param lID		[in]	�ݒ肷�郌�C����ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long lID ) {
		return SetLongValueByFieldName( schema::layernames::kLayerNameID, lID );
	}
	/**
	 * @brief <b>���C���G�C���A�X����ݒ肷��</b>\n
	 * @param lpcszName	[in]	�ݒ肷�郌�C���G�C���A�X��
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerAliasName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::layernames::kLayerAliasName, lpcszName );
	}
	//@}
};

} // layernames 

} // sindy

#endif // _LAYERNAMES_H_
