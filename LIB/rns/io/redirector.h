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

#ifndef RNS_IO_REDIRECTOR_H_
#define RNS_IO_REDIRECTOR_H_

#include <stdio.h>
#include <boost/utility.hpp>

namespace rns {
namespace io {

/**
 * @brief �o�͒u�������N���X
 */
class redirector : boost::noncopyable
{
public:
// �R���X�g���N�^�ƃf�X�g���N�^
	redirector(const char* lpszFilePath, const char* lpszMode, FILE* fp) :
	m_pFile(lpszFilePath ? ::freopen(lpszFilePath, lpszMode, fp) : 0)
	{
	}

	~redirector()
	{
		if(m_pFile) {
			::fclose(m_pFile);
		}
	}

private:
// �ϐ�
	FILE* m_pFile; ///< �o�̓t�@�C���ւ̃|�C���^�B
};

} // namespace io
} // namespace rns

#endif // RNS_IO_REDIRECTOR_H_
