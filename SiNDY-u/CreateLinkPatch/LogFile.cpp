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
 *	@file LogFile.cpp
 *	@brief ���O�t�@�C���N���X ������
 *	@author	F.Adachi
 *	@date	2005/06/29		�V�K�쐬
 *	$ID$
 */

#include "StdAfx.h"
#include "LogFile.h"
#include <string>

LogFile* LogFile::mSelf = NULL;

/**
 *	@brief	�R���X�g���N�^
 **/
LogFile::LogFile(void) : mLogStream(NULL)
{

}

/**
 *	@brief	�f�X�g���N�^
 **/
LogFile::~LogFile()
{
	delete mLogStream;
}


/**
 *	@brief	�C���X�^���X�̎擾
 *	@note	LogFile�C���X�^���X�ւ̃|�C���^��Ԃ��B�C���X�^���X����������Ă��Ȃ���ΐ�������B
 *	@return	LogFile�C���X�^���X�ւ̃|�C���^
 **/
LogFile* LogFile::Instance(void)
{
  if( mSelf == NULL ) {
	mSelf = new LogFile;
  }
  
  return mSelf;
}


/**
 *	@brief	�X�g���[���̐���
 *	@note	�^����ꂽ�o�̓t�@�C�����I�[�v�����A�X�g���[���I�u�W�F�N�g�𐶐�����
 *	@param	cFilePath	[in]	�o�̓t�@�C���p�X
 **/
void LogFile::setStream( const char* cFilePath )
{
	std::string aFunc = "LogFile::setStream()";

	mLogStream = new std::ofstream( cFilePath );
	if( !*mLogStream ) { throw std::runtime_error( "R1F:" + aFunc + "���O�t�@�C���I�[�v���G���[" ); }
}
