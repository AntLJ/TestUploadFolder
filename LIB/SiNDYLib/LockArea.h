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
 * @file LockArea.h
 * @brief <b>CLockArea�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _LOCKAREA_H_
#define _LOCKAREA_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Feature.h"
#include "RuleCommunity.h"

namespace sindy {
namespace lockarea {
/**
 * @class CLockArea
 * @brief <b>�ҏW���b�N�G���A�Ǘ����R�[�h�N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.LOCKAREA�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API CLockArea : public CFeature  
{
	ROWCLASS_CONSTRUCTOR( CLockArea, lockarea )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>���C����ID���擾����</b>\n
	 * @retval 0�ȏ� ���C����ID
	 * @retval -1 �G���[����NULL
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::lockarea::kLayerNameID, -1 );
	}

	/**
	 * @brief <b>�ڑ����[�U��ID���擾����</b>\n
	 * @retval 0�ȏ� �ڑ����[�U��ID
	 * @retval -1 �G���[����NULL
	 */
	long GetConnectUserID() const {
		return GetLongValueByFieldName( schema::lockarea::kConnectUserID, -1 );
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>���C����ID��ݒ肷��</b>\n
	 * @param nID		[in]	���C����ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long nID ) {
		return SetLongValueByFieldName( schema::lockarea::kLayerNameID, nID );
	}

	/**
	 * @brief <b>�ڑ����[�U��ID���擾����</b>\n
	 * @param nID		[in]	�ڑ����[�U��ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetConnectUserID( long nID ) {
		return SetLongValueByFieldName( schema::lockarea::kConnectUserID, nID );
	}
	//@}
};

} // lockarea

} // sindy

#endif // _LOCKAREA_H_
