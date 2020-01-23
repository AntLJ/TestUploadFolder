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
 * @file Operator.h
 * @brief <b>COperator�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _OPERATOR_H_
#define _OPERATOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace soperator {
/**
 * @class COperator
 * @brief <b>��ƎҊǗ����R�[�h�N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.OPERATOR�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API COperator : public CRow  
{
	ROWCLASS_CONSTRUCTOR( COperator, soperator )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>�I�y���[�^��ID���擾����</b>\n
	 * @retval 0�ȏ� �I�y���[�^��ID
	 * @retval -1 �G���[����NULL
	 */
	long GetOperatorID() const {
		return GetLongValueByFieldName( schema::soperator::kOperatorID, -1 );
	}

	/**
	 * @brief <b>�I�y���[�^�����擾����</b>\n
	 * @param nLen		[in]	�o�b�t�@���[�\������
	 * @param lpszName	[out]	�i�[�o�b�t�@
	 * @retval LPCTSTR �I�y���[�^��
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::soperator::kName, nLen, lpszName );
	}

	/**
	 * @brief <b>�I�y���[�^�̃G�C���A�X�����擾����</b>\n
	 * @param nLen		[in]	�o�b�t�@���[�\������
	 * @param lpszName	[out]	�i�[�o�b�t�@
	 * @retval LPCTSTR �G�C���A�X��
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetAliasName( int nLen, LPTSTR lpszName ) const{
		return GetStringValueByFieldName( schema::soperator::kAliasName, nLen, lpszName );
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>�I�y���[�^��ID��ݒ肷��</b>\n
	 * @param lOperatorID	[in]	�I�y���[�^��ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetOperatorID( long lOperatorID ) {
		return SetLongValueByFieldName( schema::soperator::kOperatorID, lOperatorID );
	}

	/**
	 * @brief <b>�I�y���[�^����ݒ肷��</b>\n
	 * @param lpcszName		[in]	�I�y���[�^��
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::soperator::kName, lpcszName );
	}

	/**
	 * @brief <b>�I�y���[�^�̃G�C���A�X����ݒ肷��</b>\n
	 * @param lpcszName		[in]	�G�C���A�X��
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetAliasName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::soperator::kAliasName, lpcszName );
	}
	//@}
};

} // soperator

} // sindy

#endif // _OPERATOR_H_
