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
 * @file RoadDispClass.h
 * @brief <b>CRoadDispClass�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _ROADDISPCLASS_H_
#define _ROADDISPCLASS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace roaddispclass {
/**
 * @class CRoadDispClass
 * @brief <b>���H�����N�̕\����ʊǗ����R�[�h�N���X�i���[���p�j</b>\n
 * ���̃N���X�́AREFERENCE.ROADDISPCLASS�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API CRoadDispClass : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CRoadDispClass, roaddispclass )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>���H��ʖ����擾����</b>\n
	 * @param nLen		[in]	�i�[�o�b�t�@������
	 * @param lpszName	[out]	�i�[�o�b�t�@
	 * @retval LPCTSTR ���H��ʖ�
	 * @retval NULL �G���[����NULL
	 */
	LPCTSTR GetName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::roaddispclass::kName, nLen, lpszName );
	}

	/**
	 * @brief <b>���H��ʖ�ID���擾����</b>\n
	 * @retval 0�ȏ� ���H��ʖ�ID
	 * @retval -1 �G���[����NULL
	 */
	long GetNameID() const {
		return GetLongValueByFieldName( schema::roaddispclass::kNameID, -1 );
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>���H��ʖ���ݒ肷��</b>\n
	 * @param lpcszName	[in]	���H��ʖ�
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::roaddispclass::kName, lpcszName );
	}

	/**
	 * @brief <b>���H��ʖ�ID��ݒ肷��</b>\n
	 * @param lID	[in]	���H��ʖ�ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetNameID( long lID ) {
		return SetLongValueByFieldName( schema::roaddispclass::kNameID, lID );
	}
	//@}
};

} // roaddispclass

} // sindy

#endif // _ROADDISPCLASS_H_
