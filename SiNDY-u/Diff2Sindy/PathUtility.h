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

#pragma once

#include <iostream>
#include <algorithm>
#include <filesystem>
#include "tstring.h"

/**
 * @brief �p�X������Utillity�N���X
 */
class PathUtillity
{
public:
	PathUtillity(){};
	~PathUtillity(){};

	/**
	 * @brief  �t�@�C���̊g���q��������������ԋp
	 * @param  path  [in]	�t�@�C���p�X
	 * @retval
	 */
	static uh::tstring getStem(uh::tstring path);

private:

};

/**
 * @param �t�H���_���ǂ������肷��
 * @note FGDB�t�H���_�̓t�H���_�Ƃ݂Ȃ��܂���
 * @param path [in] ����p�X
 * @return �t�H���_�Ƃ݂Ȃ����Ȃ�true
 */
bool isDirectory( const uh::tstring& path );

/// ������̑啶���������𖳎����Ĕ�r����
bool eq( const uh::tstring& s1, const uh::tstring& s2 );

inline bool eq( const uh::tstring& s1, const uh::tstring& s2 )
{
	return 0 == _tcsicmp( s1.c_str(), s2.c_str() );
}
