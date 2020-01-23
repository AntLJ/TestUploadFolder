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

/* ************************************************
	���e :
	���� :  
	�ǋL : 
 
	���� :  Fukuta.K
************************************************* */

/* ----- �C���N���[�h�t�@�C�� ----- */

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#include <imagehlp.h>
#else
#include <stdlib.h>
#endif



#include <iostream>

#include "RNSDirectory.h"

// ----- �R���X�g���N�^ ----- //

/// �f�t�H���g�R���X�g���N�^
RNSDirectory::RNSDirectory()
{
}

/// �R���X�g���N�^�i���������j
RNSDirectory::RNSDirectory(const char *iStr)
		: RNSString(iStr)
{
	dirComplete();
}

/// �R���X�g���N�^�i���������j
RNSDirectory::RNSDirectory(const std::string &iStr)
		: RNSString(iStr)
{
	dirComplete();
}

/// �f�X�g���N�^
RNSDirectory::~RNSDirectory()
{
}

// ----- ���� ----- //

/// ������ǉ�
RNSString& RNSDirectory::append(const RNSString &iStr)
{
	RNSString::append(iStr);
	dirComplete();
	return *this;
}

// ----- ���� ----- //

/**
 * @brief	�f�B���N�g���̑��݊m�F
 *
 * @retval	true	���݂���
 * @retval	false	���݂��Ȃ�
 */
bool RNSDirectory::exists() const
{
	if(empty())
		return false;

	std::string aString = substr(0, size()-1);
	struct stat	aStat;
	return (stat(aString.c_str(), &aStat) == 0 && (aStat.st_mode & _S_IFDIR));
}

/**
 * @brief	�f�B���N�g���쐬
 *
 * @retval	true	�쐬����
 * @retval	false	�쐬���s
 */
bool RNSDirectory::mkdir() const
{
#ifdef _WIN32
	return (MakeSureDirectoryPathExists(c_str()) == TRUE);
#else
	RNSString	aString("mkdir -p ");
	aString += c_str();
	return (system(aString.c_str()) == 0);
#endif
}

// ----- �I�y���[�^ ----- //

/// �������������
RNSString& RNSDirectory::operator = (const std::string &iStr)
{
	*static_cast<std::string*>(this) = iStr;
	dirComplete();
	return *this;
}

/// �������������
RNSString& RNSDirectory::operator = (const char *iStr)
{
	*static_cast<std::string*>(this) = iStr;
	dirComplete();
	return *this;
}

/**
 * �ǉ��A������s������ɍs������
 *
 * ��������f�B���N�g�����Ƃ��Ċ���������
 */
void RNSDirectory::appendEpiEffect()
{
	dirComplete();
}

///	�����Ƀf�B���N�g���}�[�N��t��
RNSDirectory& RNSDirectory::dirComplete(char iTrMark)
{
	if(empty())
		return *this;

	// �f�B���N�g����؂�}�[�N����Ȃ�
	if((*this)[(int)size()-1] != iTrMark){
		*this += iTrMark;//l1, iTrMark);
	}

	return *this;
}

