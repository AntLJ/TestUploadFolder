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
 * @file RoadAttribute.h
 * @brief <b>CRoadAttribute�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _ROADATTRIBUTE_H_
#define _ROADATTRIBUTE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace roadattribute {
/**
 * @class CRoadAttribute
 * @brief <b>���H�����N�������Ǘ����R�[�h�N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.ROADATTRIBUTE�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API CRoadAttribute : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CRoadAttribute, roadattribute )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>���H�����N�̑��������擾����</b>\n
	 * @param nLen		[in]	�o�b�t�@���[�\������
	 * @param lpszName	[out]	�i�[�o�b�t�@
	 * @retval LPCTSTR ���H�����N�̑�����
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::roadattribute::kName, nLen, lpszName );
	}

	/**
	 * @brief <b>���H�����N�̑������̃G�C���A�X�����擾����</b>\n
	 * @param nLen		[in]	�o�b�t�@���[�\������
	 * @param lpszName	[out]	�i�[�o�b�t�@
	 * @retval LPCTSTR ���H�����N�̑������̃G�C���A�X��
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetAliasName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::roadattribute::kAliasName, nLen, lpszName );
	}

	/**
	 * @brief <b>���H�����N�̑�����ID���擾����</b>\n
	 * @retval 0�ȏ� ���H�����N�̑�����ID
	 * @retval -1 �G���[����NULL
	 */
	long GetNameID() const {
		return GetLongValueByFieldName( schema::roadattribute::kNameID, -1 );
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>���H�����N�̑�������ݒ肷��</b>\n
	 * @param lpcszName		[in]	���H�����N�̑�����
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::roadattribute::kName, lpcszName );
	}

	/**
	 * @brief <b>���H�����N�̑������̃G�C���A�X����ݒ肷��</b>\n
	 * @param lpcszName		[in]	���H�����N�̑������̃G�C���A�X��
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetAliasName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::roadattribute::kAliasName, lpcszName );
	}

	/**
	 * @brief <b>���H�����N�̑�����ID��ݒ肷��</b>\n
	 * @param lID			[in]	���H�����N�̑�����ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetNameID( long lID ) {
		return SetLongValueByFieldName( schema::roadattribute::kNameID, lID );
	}
	//@}
};

} // roadattribute

} // sindy

#endif // _ROADATTRIBUTE_H_
