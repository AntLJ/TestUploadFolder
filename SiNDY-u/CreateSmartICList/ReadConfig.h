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

typedef std::set<long> lIDSET;

struct Parameters
{
	CString		m_strNwDB;
	lIDSET		m_lMeshSet;
	CString		m_strOut;
};

class CReadConfig
{
public:
	CReadConfig(void);
	~CReadConfig(void);

	/*
	 *	@brief	�ݒ�t�@�C�����當����̎擾
	 *	@param	 strConfigFile	[in]	�ݒ�t�@�C���̃p�X
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	Parameters getenv(const char* strConfigFile);
};
