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
 * @file EditHistory.h
 * @brief <b>CEditHistory�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _EDITHISTORY_H_
#define _EDITHISTORY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace edithistory {
/**
 * @class CEditHistory
 * @brief <b>���[���̕ҏW����ێ����R�[�h�N���X�i���[���p�j</b>\n
 * ���̃N���X�́AREFERENCE.EDITHISTORY�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API CEditHistory : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CEditHistory, edithistory )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>�ҏW�������擾����</b>\n
	 * @param nLen		[in]	�o�b�t�@�i�[�\������
	 * @param lpszDate	[out]	�擾������i�[�p�o�b�t�@
	 * @retval LPCTSTR �ҏW����
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetModifyDate( int nLen, LPTSTR lpszDate ) const {
		return GetStringValueByFieldName( schema::edithistory::kModifyDate, nLen, lpszDate );
	}

	/**
	 * @brief <b>�ҏW�e�[�u�������擾����</b>\n
	 * @param nLen		[in]	�o�b�t�@�i�[�\������
	 * @param lpszName	[out]	�擾������i�[�p�o�b�t�@
	 * @retval LPCTSTR �ҏW�e�[�u����
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetModifyTable( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::edithistory::kModifyTable, nLen, lpszName );
	}

	/**
	 * @brief <b>�ҏW���R�[�hOBJECTID���擾����</b>\n
	 * @retval 0�ȏ� �ҏW���R�[�hOBJECTID
	 * @retval -1 �G���[����NULL
	 */
	long GetModifyOID() const {
		return GetLongValueByFieldName( schema::edithistory::kModifyOID, -1 );
	}

	/**
	 * @brief <b>�ҏW�R�[�h���擾����</b>\n
	 * @retval 0�ȏ� �ҏW�R�[�h
	 * @retval -1 �G���[����NULL
	 */
	long GetUpdateType() const {
		return GetLongValueByFieldName( schema::edithistory::kUpdateType, -1 );
	}

	/**
	 * @brief <b>�ҏW�Җ����擾����</b>\n
	 * @param nLen		[in]	�o�b�t�@�i�[�\������
	 * @param lpszName	[out]	�擾������i�[�p�o�b�t�@
	 * @retval LPCTSTR �ҏW�Җ�
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetOperator( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::edithistory::kOperator, nLen, lpszName );
	}

	/**
	 * @brief <b>���b�Z�[�W���擾����</b>\n
	 * @param nLen			[in]	�o�b�t�@�i�[�\������
	 * @param lpszMessage	[out]	�擾������i�[�p�o�b�t�@
	 * @retval LPCTSTR ���b�Z�[�W
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetMessage( int nLen, LPTSTR lpszMessage ) const {
		return GetStringValueByFieldName( schema::edithistory::kMessage, nLen, lpszMessage );
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>�ҏW������ݒ肷��</b>\n
	 * @param lpcszDate	[in]	�ҏW����
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetModifyDate( LPCTSTR lpcszDate ) {
		return SetStringValueByFieldName( schema::edithistory::kModifyDate, lpcszDate );
	}

	/**
	 * @brief <b>�ҏW�e�[�u������ݒ肷��</b>\n
	 * @param lpcszName	[in]	�ҏW�e�[�u����
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetModifyTable( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::edithistory::kModifyTable, lpcszName );
	}

	/**
	 * @brief <b>�ҏW���R�[�hOBJECTID��ݒ肷��</b>\n
	 * @param lOID		[in]	�ҏW���R�[�hOBJECTID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetModifyOID( long lOID ) {
		return SetLongValueByFieldName( schema::edithistory::kModifyOID, lOID );
	}

	/**
	 * @brief <b>�ҏW�R�[�h��ݒ肷��</b>\n
	 * @param lCode		[in]	�ҏW�R�[�h
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetUpdateType( long lCode ) {
		return SetLongValueByFieldName( schema::edithistory::kUpdateType, lCode );
	}

	/**
	 * @brief <b>�ҏW�Җ���ݒ肷��</b>\n
	 * @param lpcszName	[in]	�ҏW�Җ�
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetOperator( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::edithistory::kOperator, lpcszName );
	}

	/**
	 * @brief <b>���b�Z�[�W��ݒ肷��</b>\n
	 * @param lpcszMessage	[in]	���b�Z�[�W
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetMessage( LPCTSTR lpcszMessage ) {
		return SetStringValueByFieldName( schema::edithistory::kMessage, lpcszMessage );
	}
	//@}
};

} // edithistory

} // sindy

#endif // _EDITHISTORY_H_
