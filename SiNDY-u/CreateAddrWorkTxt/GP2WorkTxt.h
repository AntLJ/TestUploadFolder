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
#include <AddrLib/AddrCodeList.h>

using namespace addr;

/**
 * @class	CGP2WorkTxt
 * @brief	���|�C���g���ԃf�[�^�쐬�N���X
 */
class CGP2WorkTxt : public CWorkTxtImpl
{
public:
	CGP2WorkTxt( const CParameter& rParam, std::ofstream& ofs ) : CWorkTxtImpl(rParam, ofs){}
	~CGP2WorkTxt(void){}

	/**
	 * @brief	������
	 * @return	bool
	 */
	bool Init();

	/**
	 * @brief	�f�[�^�o��
	 * @return	bool
	 */
	bool WriteData();

private:

	CAddrCodeList	m_cAddrCodeList;	//!< �Z���R�[�h���X�g

};
