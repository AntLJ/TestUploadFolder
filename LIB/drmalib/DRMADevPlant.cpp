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

// DRMADevPlant.cpp: DRMADevPlant �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include <sstream>
#include "DRMADevPlant.h"

#ifdef _WIN32
const char drma::dev_plant::_pathChr = '\\';
#endif

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
const char* drma::dev_plant::fileNameOf(const char* cFullPath)
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
std::string drma::dev_plant::makeSourceLocation(const char* cFileName, int cLine)
{
	std::stringstream	aStream;
	aStream << "[" << fileNameOf(cFileName) << "(" << cLine << ")]";

	return aStream.str();
}

/**
 * @brief	�f�o�b�O�p���b�Z�[�W���o�͂���
 *
 * @note	������"[�t�@�C����(�s�ԍ�)] ���b�Z�[�W"
 *
 * @param	cFileName	[in]	�t�@�C����
 * @param	cLine		[in]	�s�ԍ�
 * @param	cMsg		[in]	���b�Z�[�W
 * @return	�\�[�X�ʒu���ʕ�����+���b�Z�[�W
 */
std::string drma::dev_plant::stringForDebugMessage(const char* cFileName, int cLine, const char* cMsg)
{
	std::string aResult(makeSourceLocation(cFileName, cLine));
	aResult += " ";
	aResult += cMsg;
	return aResult;
}
