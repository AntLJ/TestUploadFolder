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
 * @file LockMesh.h
 * @brief <b>CLockMesh�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _LOCKMESH_H_
#define _LOCKMESH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace lockmesh {
/**
 * @class CLockMesh
 * @brief <b>�ҏW���b�N���b�V���Ǘ����R�[�h�N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.LOCKMESH�e�[�u���̃��R�[�h�p���b�v�N���X�ł��B
 */
class SINDYLIB_API CLockMesh : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CLockMesh, lockmesh )
	//@{ @name �����擾�֐�
	/**
	 * @brief <b>���b�V���R�[�h���擾����</b>\n
	 * @retval 0�ȏ� ���b�V���R�[�h
	 * @retval -1 �G���[����NULL
	 */
	long GetMeshCode() const {
		return GetLongValueByFieldName( schema::lockmesh::kMeshCode, -1 );
	}

	/**
	 * @brief <b>���C����ID���擾����</b>\n
	 * @retval 0�ȏ� ���C����ID
	 * @retval -1 �G���[����NULL
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::lockmesh::kLayerNameID, -1 );
	}

	/**
	 * @brief <b>�ڑ����[�U��ID���擾����</b>\n
	 * @retval 0�ȏ� �ڑ����[�U��ID
	 * @retval -1 �G���[����NULL
	 */
	long GetConnectUserID() const {
		return GetLongValueByFieldName( schema::lockmesh::kConnectUserID, -1 );
	}
	//@}
	//@{ @name �����ݒ�֐�
	/**
	 * @brief <b>���b�V���R�[�h��ݒ肷��</b>\n
	 * @param lCode		[in]	���b�V���R�[�h
	 * @retval sindyErrCode
	 */
	errorcode::sindyErrCode SetMeshCode( long lCode ) {
		return SetLongValueByFieldName( schema::lockmesh::kMeshCode, lCode );
	}

	/**
	 * @brief <b>���C����ID��ݒ肷��</b>\n
	 * @param lID		[in]	���C����ID
	 * @retval sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long lID ) {
		return SetLongValueByFieldName( schema::lockmesh::kLayerNameID, lID );
	}

	/**
	 * @brief <b>�ڑ����[�U��ID��ݒ肷��</b>\n
	 * @param lID		[in]	�ڑ����[�U��ID
	 * @retval sindyErrCode
	 */
	errorcode::sindyErrCode SetConnectUserID( long lID ) {
		return SetLongValueByFieldName( schema::lockmesh::kConnectUserID, lID );
	}
	//@}
};

} // lockmesh

} // sindy

#endif // _LOCKMESH_H_
