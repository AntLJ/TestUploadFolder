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

#include "StdAfx.h"
#include "PathUtility.h"
#include <directory_util.h>

namespace sys = std::tr2::sys;

// �t�@�C���̊g���q��������������ԋp
uh::tstring PathUtillity::getStem(uh::tstring path)
{
	sys::wpath p(path.c_str());
	return p.stem();
}

bool isDirectory( const uh::tstring& path )
{
	if( uh::isDirectory(path.c_str()) )
	{
		// FGDB�̓t�H���_�Ƃ݂Ȃ��Ȃ�
		if( !eq( path.substr( path.length() - 4 ), _T(".gdb") ) )
			return true;
	}
	return false;
}
