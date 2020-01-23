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
 * @file EditMesh.h
 * @brief <b>CEditMesh�N���X��`�t�@�C��</b>
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _EDITMESH_H_
#define _EDITMESH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace editmesh {
/**
 * @class CEditMesh
 * @brief <b>�ҏW�\���b�V���Ǘ����R�[�h�N���X�i���[���p�j</b>\n
 * ���̃N���X�́AREFERNCE.EDITMESH�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API CEditMesh : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CEditMesh, editmesh )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>���b�V���R�[�h���擾����</b>\n
	 * @retval 0�ȏ� ���b�V���R�[�h
	 * @retval -1 �G���[����NULL
	 */
	long GetMeshCode() const {
		return GetLongValueByFieldName( schema::editmesh::kMeshCode, -1 );
	}

	/**
	 * @brief <b>���C����ID���擾����</b>\n
	 * @retval 0�ȏ� ���C����ID
	 * @retval -1 �G���[����NULL
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::editmesh::kLayerNameID, -1 );
	}

	/**
	 * @brief <b>��ƖړIID���擾����</b>\n
	 * @retval 0�ȏ� ��ƖړIID
	 * @retval -1 �G���[����NULL
	 */
	long GetPurposeID() const {
		return GetLongValueByFieldName( schema::editmesh::kPurposeID, -1 );
	}

	/**
	 * @brief <b>�I�y���[�^��ID���擾����</b>\n
	 * @retval 0�ȏ� �I�y���[�^��ID
	 * @retval -1 �G���[����NULL
	 */
	long GetOperatorID() const {
		return GetLongValueByFieldName( schema::editmesh::kOperatorID, -1 );
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>���b�V���R�[�h��ݒ肷��</b>\n
	 * @param lCode		[in]	���b�V���R�[�h
	 * @retval sindyErrCode
	 */
	errorcode::sindyErrCode SetMeshCode( long lCode ) {
		return SetLongValueByFieldName( schema::editmesh::kMeshCode, lCode );
	}

	/**
	 * @brief <b>���C����ID��ݒ肷��</b>\n
	 * @param lID		[in]	���C����ID
	 * @retval sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long lID ) {
		return SetLongValueByFieldName( schema::editmesh::kLayerNameID, lID );
	}

	/**
	 * @brief <b>��ƖړIID��ݒ肷��</b>\n
	 * @param lPurposeID	[in]	��ƖړIID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetPurposeID( long lPurposeID ) {
		return SetLongValueByFieldName( schema::editmesh::kPurposeID, lPurposeID );
	}

	/**
	 * @brief <b>�I�y���[�^����ݒ肷��</b>\n
	 * @param lID			[in]	�I�y���[�^��ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetOperatorID( long lID ) {
		return SetLongValueByFieldName( schema::editmesh::kOperatorID, lID );
	}
	//@}
};

} // editmesh

} // sindy

#endif // _EDITMESH_H_
