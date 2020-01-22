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

#pragma once
#include "logger.h"
#include "TDC/sindylib_base/RowContainer.h"
#include "ErrLvDef.h"
#include "ErrTypeDef.h"

/**
 * @brief �G���[���O�Ǘ��N���X
 */
class CErrLogger :
	public CLogger
{
public:
	explicit CErrLogger(const uh::tstring& path) : CLogger(path){}

	/**
	 * @brief �w�b�_���o�͂���
	 * @note ���O�n���h���c�[���̎��R�`�����O�ŏo�͂���
	 */
	void header();
	
	/**
	 * @brief �w�肵���t�B�[�`���̏������O�ɏo�͂���
	 * @param row[in] �Ώۃt�B�[�`����Row�R���e�i
	 * @param errCode[in] �G���[�R�[�h
	 */
	void writeErrInfo( const sindy::CRowContainer& row, const err_type::error_type errCode );
};

