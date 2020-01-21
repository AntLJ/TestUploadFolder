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

#include "stdafx.h"
#include "SiNDYFreeStyleLogHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

//////////////////////////////////////////////////////////////////////
// FreeStyleLogHeader
//////////////////////////////////////////////////////////////////////

FreeStyleLogHeader::FreeStyleLogHeader()
{
}

FreeStyleLogHeader::FreeStyleLogHeader(std::istream& rStream)
{
	Builder()(*this, rStream);
}

FreeStyleLogHeader::FreeStyleLogHeader(std::wistream& rStream)
{
	Builder()(*this, rStream);
}
FreeStyleLogHeader::FreeStyleLogHeader(LPCTSTR lpszHeaderLine)
{
	Builder()(*this, lpszHeaderLine);
}

//////////////////////////////////////////////////////////////////////
// FreeStyleLogHeader::Builder
//////////////////////////////////////////////////////////////////////

/**
 * @brief �X�g���[���𑖍����ăw�b�_�s��T���A���R�`�����O�w�b�_���\�z����
 *
 * @param rLogHeader [in] �\�z�Ώێ��R�`�����O�w�b�_�B
 * @param rStream [in] ���O���̓X�g���[���B
 * @retval true �\�z�����B�X�g���[���́A�w�b�_�s�̎��̍s�̐擪���w���B
 * @retval false �w�b�_�s�����݂��Ȃ��B�X�g���[����EOF���w���B
 */
bool FreeStyleLogHeader::Builder::operator()(SiNDYLogHeader& rLogHeader, std::istream& rStream)
{
	char szLine[2048];
	while(rStream.getline(szLine, 2048)) {
		if(operator()(rLogHeader, szLine))
			return true;
	}

	return false;
}

bool FreeStyleLogHeader::Builder::operator()(SiNDYLogHeader& rLogHeader, std::wistream& rStream)
{
	wchar_t szLine[2048];
	while(rStream.getline(szLine, 2048)) {
		if(operator()(rLogHeader, szLine))
			return true;
	}

	return false;
}

/**
 * @brief �w�b�_�s��������w�肵�A���R�`�����O�w�b�_���\�z����ichar�Łj
 *
 * @param rLogHeader [in] �\�z�Ώێ��R�`�����O�w�b�_�B
 * @param lpszHeaderLine [in] �w�b�_�s������B
 * @retval true �\�z�����B
 * @retval false �w�b�_�s�Ƃ��Ă͔F�߂��Ȃ��B
 */
bool FreeStyleLogHeader::Builder::operator()(SiNDYLogHeader& rLogHeader, LPCSTR lpszHeaderLine)
{
	// ��s�A�R�����g�s�̓w�b�_�ɐ��蓾�Ȃ�
	if(! lpszHeaderLine || lpszHeaderLine[0] == 0 || lpszHeaderLine[0] == '#')
		return false;

	// ����ȊO�̓w�b�_�Ƃ��ĔF�߂�
	USES_CONVERSION;
	rLogHeader.setHeader(A2CT(lpszHeaderLine), FreeStyleLogHeader::seps());

	return true;
}

/**
 * @brief �w�b�_�s��������w�肵�A���R�`�����O�w�b�_���\�z����iwhar_t�Łj
 *
 * @param rLogHeader [in] �\�z�Ώێ��R�`�����O�w�b�_�B
 * @param lpszHeaderLine [in] �w�b�_�s������B
 * @retval true �\�z�����B
 * @retval false �w�b�_�s�Ƃ��Ă͔F�߂��Ȃ��B
 */
bool FreeStyleLogHeader::Builder::operator()(SiNDYLogHeader& rLogHeader, LPCWSTR lpszHeaderLine)
{
	// ��s�A�R�����g�s�̓w�b�_�ɐ��蓾�Ȃ�
	if(! lpszHeaderLine || lpszHeaderLine[0] == 0 || lpszHeaderLine[0] == '#')
		return false;

	// ����ȊO�̓w�b�_�Ƃ��ĔF�߂�
	USES_CONVERSION;
	rLogHeader.setHeader(W2CT(lpszHeaderLine), FreeStyleLogHeader::seps());

	return true;
}
