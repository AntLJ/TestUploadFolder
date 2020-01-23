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
 * @file SharedMoveLayers.h
 * @brief <b>CSharedMoveLayers�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _SHAREDMOVELAYERS_H_
#define _SHAREDMOVELAYERS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace sharedmovelayers {
/**
 * @class CSharedMoveLayers
 * @brief <b>���L�ړ��Ǘ����R�[�h�N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.SHAREDMOVELAYERS�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API CSharedMoveLayers : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CSharedMoveLayers, sharedmovelayers )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>���L�ړ��Z�b�gID���擾����</b>\n
	 * @retval 0�ȏ� ���L�ړ��Z�b�gID
	 * @retval -1 �G���[����NULL
	 */
	long GetSetID() const {
		return GetLongValueByFieldName( schema::sharedmovelayers::kSetID, -1 );
	}

	/**
	 * @brief <b>���C����ID���擾����</b>\n
	 * @retval 0�ȏ� ���C����ID
	 * @retval -1 �G���[����NULL
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::sharedmovelayers::kLayerNameID, -1 );
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>���L�ړ��Z�b�gID��ݒ肷��</b>\n
	 * @param lID	[in]	���L�ړ��Z�b�gID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetSetID( long lID ) {
		return SetLongValueByFieldName( schema::sharedmovelayers::kSetID, lID );
	}

	/**
	 * @brief <b>���C����ID��ݒ肷��</b>\n
	 * @param lID	[in]	���C����ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long lID ) {
		return SetLongValueByFieldName( schema::sharedmovelayers::kLayerNameID, lID );
	}
	//@}
};

} // shareddeletelayers

} // sindy

#endif // _SHAREDMOVELAYERS_H_
