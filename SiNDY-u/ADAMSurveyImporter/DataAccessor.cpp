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

#include <TDC/useful_headers/directory_util.h>
#include "LogUtil.h"

#include "DataAccessor.h"

using namespace adam_v2;

bool DataAccessor::ReadCrdParam(const uh::tstring& fileName)
	{
		if( ! uh::isFile( fileName.c_str() ))
		{
			LogUtil::printError(50, "�t�@�C���I�[�v�����s\t" + uh::toStr( fileName ));
			return false;
		}

		// 1�b�Ԋu��5��ǂݍ��݂Ƀg���C
		for(int i=0; i<5; ++i)
		{
			if(_cnv.JGD2000_RP_Init(const_cast<char*>(uh::toStr(fileName).c_str())))
			{
				return true;
			}

			Sleep(1000);
		}
	
		LogUtil::printError(51, "���W�ϊ��p�p�����[�^�t�@�C���̓ǂݍ��݂Ɏ��s���܂���\t" + uh::toStr( fileName ));

		return false;
	}