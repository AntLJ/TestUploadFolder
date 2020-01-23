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
 * @file DispNaviRelation.h
 * @brief <b>CDispNaviRelation�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _DISPNAVIRELATION_H_
#define _DISPNAVIRELATION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace dispnavirelation {

/**
 * @class CDispNaviRelation
 * @brief <b>���H�����N�̕\����ʂƌo�H��ʂ̑Ή��Ǘ��\�p���R�[�h�N���X</b>\n
 * ���̃N���X�́AREFERENCE.DISPNAVIREALTION�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API CDispNaviRelation : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CDispNaviRelation, dispnavirelation )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>���H�����N�̓��H���ID���擾����</b>\n
	 * @retval 0�ȏ� ���H�����N�̓��H���ID
	 * @retval -1 �G���[����NULL
	 */
	long GetDispClassID() const {
		return GetLongValueByFieldName( schema::dispnavirelation::kDispClassID, -1 );
	}

	/**
	 * @brief <b>���H�����N�̌o�H���ID���擾����</b>\n
	 * @retval 0�ȏ� �H�����N�̌o�H���ID
	 * @retval -1 �G���[����NULL
	 */
	long GetNaviClassID() const {
		return GetLongValueByFieldName( schema::dispnavirelation::kNaviClassID, -1 );
	}

	/**
	 * @brief <b>�֘A�l���擾����</b>\n
	 * @retval 0�ȏ� �֘A�l
	 * @retval -1 �G���[����NULL
	 */
	long GetRelValue() const {
		return GetLongValueByFieldName( schema::dispnavirelation::kRelValue, -1 );
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>���H�����N�̓��H���ID��ݒ肷��</b>\n
	 * @param lID	[in]	���H�����N�̓��H���ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetDispClassID( long lID ) {
		return SetLongValueByFieldName( schema::dispnavirelation::kDispClassID, lID );
	}

	/**
	 * @brief <b>���H�����N�̌o�H���ID��ݒ肷��</b>\n
	 * @param lID	[in] �H�����N�̌o�H���ID
	 * @retval -1 �G���[����NULL
	 */
	errorcode::sindyErrCode SetNaviClassID( long lID ) {
		return SetLongValueByFieldName( schema::dispnavirelation::kNaviClassID, lID );
	}

	/**
	 * @brief <b>�֘A�l��ݒ肷��</b>\n
	 * @param lVal	[in]	�֘A�l
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRelValue( long lVal ) {
		return SetLongValueByFieldName( schema::dispnavirelation::kRelValue, lVal );
	}
	//@}
};

} // dispnavirelation

} // sindy

#endif // _DISPNAVIRELATION_H_
