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

#include "WorkTxtImpl.h"

/**
 * @class	CCS2WorkTxt
 * @brief	CS�|�C���g���ԃf�[�^�쐬�N���X
 */
class CCS2WorkTxt : public CWorkTxtImpl
{
public:
	CCS2WorkTxt( const CParameter& rParam, std::ofstream& ofs ) : CWorkTxtImpl(rParam, ofs){}
	~CCS2WorkTxt(void){}

	/**
	 * @brief	������
	 * @return	bool
	 */
	bool Init(){ return true; }

	/**
	 * @brief	�f�[�^�o��
	 * @return	bool
	 */
	bool WriteData();
};
