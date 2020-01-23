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
 * @file DispOtherRelation.h
 * @brief <b>CDispOtherRelation�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _DISPOTHERRELATION_H_
#define _DISPOTHERRELATION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace dispotherrelation {
/**
 * @class CDispOtherRelation
 * @brief <b>���H�����N�̕\����ʂƂ��̑��̑����̑Ή��Ǘ��p���R�[�h�N���X</b>\n
 * ���̃N���X�́AREFERENCE.DISPOTHERRELATION�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API CDispOtherRelation : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CDispOtherRelation, dispotherrelation )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>���H�����N�̓��H���ID���擾����</b>\n
	 * @retval 0�ȏ� ���H�����N�̓��H���ID
	 * @retval -1 �G���[����NULL
	 */
	long GetDispClassID() const {
		return GetLongValueByFieldName( schema::dispotherrelation::kDispClassID, -1 );
	}

	/**
	 * @brief <b>���H�����N�̂��̑��̑�����ID���擾����</b>\n
	 * @retval 0�ȏ� ���H�����N�̂��̑��̑�����ID
	 * @retval -1 �G���[����NULL
	 */
	long GetOtherAttrID() const {
		return GetLongValueByFieldName( schema::dispotherrelation::kOtherAttrID, -1 );
	}

	/**
	 * @brief <b>�֘A�l���擾����</b>\n
	 * @retval 0�ȏ� �֘A�l
	 * @retval -1 �G���[����NULL
	 */
	long GetRelValue() const {
		return GetLongValueByFieldName( schema::dispotherrelation::kRelValue, -1 );
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>���H�����N�̓��H���ID��ݒ肷��</b>\n
	 * @param nID	[in]	���H�����N�̓��H���ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetDispClassID( long nID ) {
		return SetLongValueByFieldName( schema::dispotherrelation::kDispClassID, nID );
	}

	/**
	 * @brief <b>���H�����N�̂��̑��̑�����ID��ݒ肷��</b>\n
	 * @param nID	[in]	���H�����N�̂��̑��̑�����ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetOtherAttrID( long nID ) {
		return SetLongValueByFieldName( schema::dispotherrelation::kOtherAttrID, nID );
	}

	/**
	 * @brief <b>�֘A�l��ݒ肷��</b>\n
	 * @param lVal	[in]	�֘A�l
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRelValue( long lVal ) {
		return SetLongValueByFieldName( schema::dispotherrelation::kRelValue, lVal );
	}
	//@}
};

} // dispotherrelation

} // sindy

#endif // _DISPOTHERRELATION_H_
