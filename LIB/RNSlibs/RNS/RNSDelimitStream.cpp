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

// RNSDelimitStream.cpp: RNSDelimitStream �N���X�̃C���v�������e�[�V����
//
// $Id: RNSDelimitStream.cpp,v 1.2 2003/05/16 09:36:19 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#include <string>
#include "RNSDelimitStream.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

/**
 * @brief	�R���X�g���N�^
 *
 * �o�͂���X�g���[���ƃf���~�^���w��B
 * 
 * @param	pStream		[in]	�o�̓X�g���[��
 * @param	cDelimiter	[in]	�f���~�^
 */
RNSDelimitStream::RNSDelimitStream(std::ostream* pStream, const char* cDelimiter) :
	mDelimNum(0),
	mStream(0)
{
	setStream(pStream);
	setDelimiter(cDelimiter);
}

//////////////////////////////////////////////////////////////////////
// �ݒ�
//////////////////////////////////////////////////////////////////////

/**
 * @brief	�o�̓X�g���[����ݒ�
 *
 * @param	pStream	[in]	�o�̓X�g���[��
 * @exception	std::invalid_arugment	pStream��NULL�ł���B
 */
void RNSDelimitStream::setStream(std::ostream* pStream)
{
	if(! pStream)
		throw std::invalid_argument(std::string("null stream pointer"));

	mStream = pStream;
}

/**
 * @brief	�f���~�^�ǉ��i�O�u�j
 *
 * ��l�^�̃I�y���[�^�͖ʓ|�Ȃ̂őΉ����Ă��Ȃ��B
 * 
 * @return	����
 */
RNSDelimitStream& RNSDelimitStream::operator ++ ()
{
	mDelimNum++;
	return *this;
}
