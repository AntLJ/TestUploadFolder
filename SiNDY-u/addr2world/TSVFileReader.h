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

#include "typedef.h"
#include <fstream>
#include <CStringTokenizer.h>


class TSVFileReader : public CStringArrayArray
{
	CString m_fileName;

public:

	/**
	 *	@brief	�R���X�g���N�^
	 */
	TSVFileReader(void) : m_fileName(_T("")) {}

	/**
	 *	@brief	�f�X�g���N�^
	 */
	virtual ~TSVFileReader(void) {}

	/**
	 *	@brief	�t�@�C���ǂݍ���
	 *	@param	fileName [in] �t�@�C���̃p�X
	 *	@retval	�ǂݍ��񂾃��R�[�h��(�s��)�A�G���[�̏ꍇ��-1�����^�[������
	 */
	long read(LPCTSTR fileName)
	{
		m_fileName = fileName ? fileName : _T("");
		if (! m_fileName.GetLength())
			return -1;
		this->clear();
		std::ifstream file(m_fileName);
		if (! file.is_open())
			return -1;
		char buf[BUFSIZ+1];
		while (file.getline(buf, BUFSIZ)) {
			CString tmp = CA2CT(buf);
			CStringTokenizer tokenizer(tmp, _T("\t"), TOKEN_RET_EMPTY_ALL);
			CStringArray record;
			int count = tokenizer.CountTokens();
			for (int i = 0; i < count; i++) {
				record.push_back(tokenizer.GetNextToken());
			}
			this->push_back(record);
		}
		return (long)this->size();
	}
};

