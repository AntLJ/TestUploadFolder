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

// RNSPlant.cpp: RNSPlant �N���X�̃C���v�������e�[�V����
//
// $Id: RNSPlant.cpp,v 1.4 2003/05/16 09:37:38 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#include "RNS.h"
#include <direct.h>
#include <sstream>
#include "RNSPlant.h"

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief	�J�����g�f�B���N�g����ύX����
 *
 * @param	cFileName	[in]	�t�@�C����
 * @retval	true	����
 * @retval	false	���s
 */
bool RNSPlant::changeDirectoryToFile(const char* cFileName)
{
	char* pPathEnd = 0;

	if((pPathEnd = strrchr((char*)cFileName, _pathChr)) != 0) {
		char	aPath[256];
		strncpy(aPath, cFileName, pPathEnd - cFileName);
		aPath[pPathEnd - cFileName] = 0;
		if(::chdir(aPath))
			return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief	�t���p�X����t�@�C�����̊J�n�ʒu���擾����
 *
 * @param	cFullPath	[in]	�t���p�X
 * @return	�t���p�X�ɑ��݂���Ō���̃Z�p���[�^����1byte���̕����ւ̃|�C���^��Ԃ��B\n
 *			�Z�p���[�^�����݂��Ȃ��ꍇ�́AcFullPath�����̂܂ܕԂ��B
 */
const char* RNSPlant::fileNameOf(const char* cFullPath)
{
	return strrchr(cFullPath, _pathChr) ? (strrchr(cFullPath, _pathChr) + 1) : cFullPath;
}

/**
 * @brief	�t�@�C�����ƍs�ԍ����������āA�\�[�X�ʒu���莯�ʕ�������쐬����
 *
 * @note	������"[�t�@�C����(�s�ԍ�)]"
 *
 * @param	cFileName	[in]	�t�@�C����
 * @param	cLine		[in]	�s�ԍ�
 * @return	�\�[�X�ʒu���ʕ�����
 */
std::string RNSPlant::makeSourceLocation(const char* cFileName, int cLine)
{
	std::stringstream	aStream;
	aStream << "[" << fileNameOf(cFileName) << "(" << cLine << ")]";

	return aStream.str();
}

/**
 * @brief	�f�o�b�O�p���b�Z�[�W���쐬���ĕԂ�
 *
 * @note	������"[<DM>�t�@�C����(�s�ԍ�)] ���b�Z�[�W"
 *
 * @param	cFileName	[in]	�t�@�C����
 * @param	cLine		[in]	�s�ԍ�
 * @param	cMsg		[in]	���b�Z�[�W
 * @return	�\�[�X�ʒu���ʕ�����+���b�Z�[�W
 */
std::string RNSPlant::stringForDebugMessage(const char* cFileName, int cLine, const char* cMsg)
{
	std::string aResult(makeSourceLocation(cFileName, cLine));
	aResult += " ";
	aResult += cMsg;
	return aResult;
}

void RNSPlant::trace(std::ostream& cStream, const char* cMsg)
{
	cStream << cMsg << std::endl;
}
