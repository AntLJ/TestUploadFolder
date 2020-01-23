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
 *	@file CmdLineArg.cpp
 *	@brief �R�}���h���C�������N���X ������
 *	@author	F.Adachi
 *	@date	2005/10/18		�V�K�쐬
 *	$ID$
 */

#include "StdAfx.h"
#include "CmdLineArg.h"
#include "ZCL/ZFileSwitch.h"
#include "ZCL/ZBoolSwitch.h"
#include "ZCL/ZCmdLine.h"
#include "ZCL/ZSepByTab.h"
#include "LogFile.h"


/// �R���X�g���N�^
CmdLineArg::CmdLineArg(void) :
	m_pDBInfo(NULL),
	m_EUC(false)
{
}

/// �f�X�g���N�^
CmdLineArg::~CmdLineArg()
{
}


/**
 *	@brief	���
 *	@note	�^����ꂽ�R�}���h���C����������͂��āA����ꂽ�l���i�[
 *	@param	cArgc	[in]	�R�}���h���C�������̐�
 *	@param	cArgv	[in]	�R�}���h���C�������̗�
 **/
void CmdLineArg::analyze( int cArgc, char** cArgv )
{
	std::string aFunc = "CmdLineArg::analyze()";

	ZCmdLine aCmdLine;
	aCmdLine.append( new ZFileSwitch( 'd', true, "DBSpecificationString", "DB�w�蕶����" ) );
	aCmdLine.append( new ZFileSwitch( 'l', true, "OutLogFile", "�o�̓��O�t�@�C��" ) );
	aCmdLine.append( new ZFileSwitch( 'r', true, "AppropriateAttributeFile", "�H���R�[�h�w��t�@�C��" ) );
	aCmdLine.append( new ZFileSwitch( 'p', true, "LinkPatchFile", "�����N�p�b�`�t�@�C��" ) );
	aCmdLine.append( new ZBoolSwitch( 'u', false, "UNIX", "���s�R�[�h��LF�ŏo��" ) );

	if (! aCmdLine.analyze( cArgc, cArgv ) ) {
		aCmdLine.errorHandle();
		throw std::runtime_error( "R1F:" + aFunc + " : �R�}���h���C���������s��" );
	}
	
	char* aDBStr = const_cast<char*>(static_cast<const char*>(aCmdLine.getSwitch("DBSpecificationString")->getObject()));
	const char* pLogFileName = static_cast<const char*>(aCmdLine.getSwitch("OutLogFile")->getObject());
	m_ApprAttrFilePath = static_cast<const char*>(aCmdLine.getSwitch("AppropriateAttributeFile")->getObject());
	m_LinkPatchFilePath = static_cast<const char*>(aCmdLine.getSwitch("LinkPatchFile")->getObject());
	
	// �I�v�V�����t���O�̐ݒ�
	m_EUC = (aCmdLine.getSwitch("UNIX")->getObject() == NULL) ? false : true;

	if (m_pDBInfo == NULL) { throw std::runtime_error( "R1F:" + aFunc + " : DBInfo�����Z�b�g" ); }
	m_pDBInfo->init(aDBStr);

	// �o�̓��O�t�@�C���̐ݒ�
	LogFile* pLog = LogFile::Instance();
	pLog->setStream( pLogFileName );
}
	
