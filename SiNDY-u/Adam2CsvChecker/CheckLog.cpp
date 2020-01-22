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
#include "CheckLog.h"
#include "LogSystem.h"
#include <assert.h>

using namespace std;

const char* kDELIMITER = "\t";

CheckLog::CheckLog()
{
	m_cStream.open("checklist.txt");
	assert(m_cStream.is_open());
}

void CheckLog::printError(
				long           code,
				const string&  info,
				const string&  file,
				long           oid,
				const string&  field,
				const string&  msg
	)
{
	print(0, code, info, file, oid, field, msg);
}

void CheckLog::printWarning(
				long           code,
				const string&  info,
				const string&  file,
				long           oid,
				const string&  field,
				const string&  msg
	)
{
	print(1, code, info, file, oid, field, msg);
}

void CheckLog::printInfo(
				long           code,
				const string&  info,
				const string&  file,
				long           oid,
				const string&  field,
				const string&  msg
	)
{
	print(2, code, info, file, oid, field, msg);
}

void CheckLog::print(
				long           level,
				long           code,
				const string&  info,
				const string&  file,
				long           oid,
				const string&  field,
				const string&  msg
	)
{
	assert(m_cStream.is_open());
	if (! m_cStream.is_open()) { return; }

	static const char* kLevelStr[] = {
		"[ERR]",
		"[WAR]",
		"[INF]",
		"[DEV]",
	};

	m_cStream
		<< kLevelStr[level] << kDELIMITER   // �G���[���x��
		<< code             << kDELIMITER   // �G���[�R�[�h
		<< info.c_str()     << kDELIMITER   // �G���[���b�Z�[�W
		<< file.c_str()     << kDELIMITER   // �Ώۃt�@�C�� (�e�[�u����)
		<< oid              << kDELIMITER   // �I�u�W�F�N�gID
		<< field.c_str()    << kDELIMITER   // �G���[�t�B�[���h
		<< msg.c_str()      << kDELIMITER   // �G���[�l
		<< endl;
}




