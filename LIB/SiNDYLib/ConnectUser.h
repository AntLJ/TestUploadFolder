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
 * @file ConnectUser.h
 * @brief <b>CConnectUser�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _CONNECTUSER_H_
#define _CONNECTUSER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace connectuser {

/**
 * @class CConnectUser
 * @brief <b>�ڑ����[�U���R�[�h�N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.CONNECTUSER�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API CConnectUser : public CRow  
{
public:
	ROWCLASS_CONSTRUCTOR( CConnectUser, connectuser )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>�ڑ����[�U��ID���擾����</b>\n
	 * @retval 0�ȏ� �ڑ����[�U��ID
	 * @retval -1 �G���[����NULL
	 */
	long GetConnectUserID() const {
		return GetLongValueByFieldName( schema::connectuser::kConnectUserID, -1 );
	}

	/**
	 * @brief <b>�ڑ����[�U�����擾����</b>\n
	 * @param nLen		[in]	�o�b�t�@�i�[�\������
	 * @param lpszName	[out]	�i�[�o�b�t�@
	 * @retval LPCTSTR �ڑ����[�U��
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::connectuser::kName, nLen, lpszName );
	}
	//@]
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>�ڑ����[�U��ID��ݒ肷��</b>\n
	 * @param nID		[in]	�ڑ����[�U��ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetConnectUserID( long nID ) {
		return SetLongValueByFieldName( schema::connectuser::kConnectUserID, nID );
	}

	/**
	 * @brief <b>�ڑ����[�U����ݒ肷��</b>\n
	 * @param lpcszName	[in]	�ڑ����[�U��
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::connectuser::kName, lpcszName );
	}
	//@}
};

} // connectuser

} // sindy

#endif // _CONNECTUSER_H_
