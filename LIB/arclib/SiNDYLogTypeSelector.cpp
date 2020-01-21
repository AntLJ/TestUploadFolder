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
#include "SiNDYStdLogHeader.h"
#include "SiNDYLogTypeSelector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

#if(0)
//////////////////////////////////////////////////////////////////////
// Reseeker
//////////////////////////////////////////////////////////////////////

/**
 * @brief �I�u�W�F�N�g�j�����ɃV�[�N�ʒu�����ɖ߂��N���X
 */
class Reseeker
{
public:
	Reseeker(std::istream& rSteram);
	~Reseeker();

private:
	std::istream& m_rStream;
	std::streampos m_SeekPos;
};

Reseeker::Reseeker(std::istream& rStream) :
m_rStream(rStream),
m_SeekPos(rStream.tellg())
{
}

Reseeker::~Reseeker()
{
	m_rStream.seekg(m_SeekPos, std::ios::beg);
}
#endif

//////////////////////////////////////////////////////////////////////
// SiNDYLogTypeSelector
//////////////////////////////////////////////////////////////////////

/**
 * @brief ���O�w�b�_�I�u�W�F�N�g�𐶐�����
 *
 * �X�g���[���̌��݈ʒu����ő�T�s��ǂݍ��݁A�w�b�_�^�C�v�̋L�q��T���B<br>
 * bRetainSeekPos��false�ɂ����ꍇ�A�������ꂽ���O�w�b�_�I�u�W�F�N�g�ɂ���āA�V�[�N�ʒu�͕ϓ�����B
 * - ���R�`�����O : �w�b�_�s�̎��̍s�̐擪�B
 * - �W���`�����O : �w�b�_�^�C�v���L�q���ꂽ�s�̎��̍s�̐擪�B
 * - �I�u�W�F�N�g����������Ȃ����� : �V�[�N�ʒu�͕ς��Ȃ��B
 * 
 * @param rSteram [in] ���O���̓X�g���[���B
 * @param bRetainSeekPos [in] �V�[�N�ʒu��ێ����邩�B
 * @return ���O�w�b�_�I�u�W�F�N�g�B�w�b�_�^�C�v���ǂݎ��Ȃ������ꍇ��NULL��Ԃ��B
 */
SiNDYLogHeader* LogTypeSelector::newLogHeader(std::istream& rStream, bool bRetainSeekPos)
{
	std::streampos aSeekPos = rStream.tellg();

	USES_CONVERSION;
	LPCSTR lpszFreeStyleLogHeaderId = T2CA(FreeStyleLogHeader::id()); //< ���R�`�����O�w�b�_ID�B
	LPCSTR lpszStdLogHeaderId = T2CA(StdLogHeader::id()); //< �W���`�����O�w�b�_ID�B

	SiNDYLogHeader* pResult = 0;

	char szLine[2048];
	for(int i = 0; i < 5; i++) {
		if(! rStream.getline(szLine, 2048))
			break;

		// �g���~���O�B
		char* psz = szLine + strspn(szLine, " \t\r\n");

		// ��s�ǂݔ�΂��B
		if(*psz == 0)
			continue;

		// �R�����g�s�ȊO�ɒH�蒅������A���͂⃍�O�`���͓���ł��Ȃ��B
		if(*psz != '#')
			break;

		if(strstr(psz, lpszFreeStyleLogHeaderId)) {
			// ���R�`�����O�w�b�_��Ԃ��B
			pResult = new FreeStyleLogHeader(rStream);
			break;
		}
		else if(strstr(psz, lpszStdLogHeaderId)) {
			// �W���`�����O�w�b�_��Ԃ��B
			pResult = new StdLogHeader();
			break;
		}
	}

	if(bRetainSeekPos || ! pResult) {
		rStream.seekg(aSeekPos, std::ios::beg);
	}

	return pResult;
}

SiNDYLogHeader* LogTypeSelector::newLogHeader(std::wistream& rStream, bool bRetainSeekPos)
{
	std::streampos aSeekPos = rStream.tellg();

	USES_CONVERSION;
	LPCWSTR lpszFreeStyleLogHeaderId = T2CW(FreeStyleLogHeader::id()); //< ���R�`�����O�w�b�_ID�B
	LPCWSTR lpszStdLogHeaderId = T2CW(StdLogHeader::id()); //< �W���`�����O�w�b�_ID�B

	SiNDYLogHeader* pResult = 0;

	wchar_t szLine[2048];
	for(int i = 0; i < 5; i++) {
		if(! rStream.getline(szLine, 2048))
			break;

		// �g���~���O�B
		wchar_t* psz = szLine + ::wcsspn(szLine, L" \t\r\n");

		// ��s�ǂݔ�΂��B
		if(*psz == 0)
			continue;

		// �R�����g�s�ȊO�ɒH�蒅������A���͂⃍�O�`���͓���ł��Ȃ��B
		if(*psz != '#')
			break;

		if(::wcsstr(psz, lpszFreeStyleLogHeaderId)) {
			// ���R�`�����O�w�b�_��Ԃ��B
			pResult = new FreeStyleLogHeader(rStream);
			break;
		}
		else if(::wcsstr(psz, lpszStdLogHeaderId)) {
			// �W���`�����O�w�b�_��Ԃ��B
			pResult = new StdLogHeader();
			break;
		}
	}

	if(bRetainSeekPos || ! pResult) {
		rStream.seekg(aSeekPos, std::ios::beg);
	}

	return pResult;
}
