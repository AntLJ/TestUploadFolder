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
 * @file SharedDeleteLayers.h
 * @brief <b>CSharedDeleteLayers�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _SHAREDDELELAYERS_H_
#define _SHAREDDELELAYERS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace shareddeletelayers {
/**
 * @class CSharedDeleteLayers
 * @brief <b>���L�폜�Ǘ����R�[�h�N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.SHAREDDELETELAYERS�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API CSharedDeleteLayers : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CSharedDeleteLayers, shareddeletelayers )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>���L�ړ��Z�b�gID���擾����</b>\n
	 * @retval 0�ȏ� ���L�ړ��Z�b�gID
	 * @retval -1 �G���[����NULL
	 */
	long GetSetID() const {
		return GetLongValueByFieldName( schema::shareddeletelayers::kSetID, -1 );
	}

	/**
	 * @brief <b>���C����ID���擾����</b>\n
	 * @retval 0�ȏ� ���C����ID
	 * @retval -1 �G���[����NULL
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::shareddeletelayers::kLayerNameID, -1 );
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>���L�ړ��Z�b�gID��ݒ肷��</b>\n
	 * @param lID	[in]	���L�ړ��Z�b�gID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetSetID( long lID ) {
		return SetLongValueByFieldName( schema::shareddeletelayers::kSetID, lID );
	}

	/**
	 * @brief <b>���C����ID��ݒ肷��</b>\n
	 * @param lID	[in]	���C����ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long lID ) {
		return SetLongValueByFieldName( schema::shareddeletelayers::kLayerNameID, lID );
	}
	//@}
};

} // shareddeletelayers

} // sindy

#endif // _SHAREDDELELAYERS_H_
