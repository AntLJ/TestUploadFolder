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
 * @file BackGroundClass.h
 * @brief <b>CBackGroundClass�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _BACKGROUNDCLASS_H_
#define _BACKGROUNDCLASS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace background_class {
/**
 * @class CBackGroundClass
 * @brief <b>�w�i�n���[�����R�[�h�N���X</b>\n
 */
class SINDYLIB_API CBackGroundClass : public CRow
{
	ROWCLASS_CONSTRUCTOR( CBackGroundClass, background_class )
	/**
	 * @brief <b>�e�[�u�������擾����</b>\n
	 * @return �e�[�u����
	 */
	 LPCTSTR GetTableName() const { return schema::background_class::kTableName; }

	//@{ @name �����擾�֐�
	/**
	 * @brief <b>��ʂ��擾����</b>\n
	 * @retval 0�ȏ� ���
	 * @retval -1 �G���[����NULL
	 */
	long GetClass() const {
		return GetLongValueByFieldName( schema::background_class::kClass, -1 );
	}

	/**
	 * @brief <b>��ʖ��̂��擾����</b>\n
	 * @param nLen		[in]	lpszName�̎��[�\������
	 * @param lpszName	[out]	��ʖ��̂��i�[����o�b�t�@
	 * @retval LPCTSTR ��ʖ���
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetClassName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::background_class::kClassName, nLen, lpszName );
	}

	/**
	 * @brief <b>���e�ԍ����擾����</b>\n
	 * @param nLen		[in]	lpszCode�̎��[�\������
	 * @param lpszCode	[out]	���e�ԍ����i�[����o�b�t�@
	 * @retval LPCTSTR ���e�ԍ�
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetMSNo( int nLen, LPTSTR lpszCode ) const {
		return GetStringValueByFieldName( schema::background_class::kMSNo, nLen, lpszCode );
	}

	/**
	 * @brief <b>���C����ID���擾����</b>\n
	 * @retval 0�ȏ� ���C����ID
	 * @retval -1 �G���[����NULL
	 */
	long GetLayerID() const {
		return GetLongValueByFieldName( schema::background_class::kLayerID, -1 );
	}

	/**
	 * @brief <b>�n���t���O���擾����</b>\n
	 * @return bool
	 */
	bool GetUnderGround() const {
		return GetBoolValueByFieldName( schema::background_class::kUnderGround, false );
	}

	/**
	 * @brief <b>IN��ʃR�[�h���擾����</b>\n
	 * @param nLen		[in]	lpszCode�̎��[�\������
	 * @param lpszCode	[out]	IN��ʃR�[�h���i�[����o�b�t�@
	 * @retval LPCTSTR ���e�ԍ�
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetINClass( int nLen, LPTSTR lpszCode ) const {
		return GetStringValueByFieldName( schema::background_class::kInClass, nLen, lpszCode );
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>��ʂ�ݒ肷��</b>\n
	 * @param lCode		[in]	��ʃR�[�h
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetClass( long lCode ) {
		return SetLongValueByFieldName( schema::background_class::kClass, lCode );
	}

	/**
	 * @brief <b>��ʖ��̂�ݒ肷��</b>\n
	 * @param lpcszName	[in]	��ʖ���
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetClassName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::background_class::kClassName, lpcszName );
	}

	/**
	 * @brief <b>���e�ԍ���ݒ肷��</b>\n
	 * @param lpcszCode	[in]	���e�ԍ�
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetMSNo( LPCTSTR lpcszCode ) {
		return SetStringValueByFieldName( schema::background_class::kMSNo, lpcszCode );
	}

	/**
	 * @brief <b>���C����ID��ݒ肷��</b>\n
	 * @param nID		[in]	���C����ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerID( long nID ) {
		return SetLongValueByFieldName( schema::background_class::kLayerID, nID );
	}

	/**
	 * @brief <b>�n���t���O��ݒ肷��</b>\n
	 * @param bFlag		[in]	�n���t���O
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetUnderGround( bool bFlag ) {
		return SetBoolValueByFieldName( schema::background_class::kUnderGround, bFlag );
	}

	/**
	 * @brief <b>IN��ʃR�[�h��ݒ肷��</b>\n
	 * @param lpcszCode	[out]	IN��ʃR�[�h
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetINClass( LPCTSTR lpcszCode ) {
		return SetStringValueByFieldName( schema::background_class::kInClass, lpcszCode );
	}
	//@}
};

} // background_class

} // sindy

#endif // _BACKGROUNDCLASS_H_
