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

#ifndef RNS_IO_LOCAL_TIME_H_
#define RNS_IO_LOCAL_TIME_H_

#include <time.h>
#include <iostream>

namespace rns {
namespace io {

/**
 * @brief ���ݎ��������n���ԑт̐ݒ�ɍ��킹�ďo�͂���B
 *
 * @param os [in] �o�̓X�g���[���B
 * @return �o�̓X�g���[���B
 *
 * @attention
 * ctime() �֐��̎d�l�ɑ���A����������̌��ɂ͉��s������t�^����B
 */
inline std::ostream& local_time(std::ostream& os)
{
	time_t ltime;
	::time(&ltime);
	return os << ::ctime(&ltime);
}

} // namespace io
} // namespace rns

#endif // RNS_IO_LOCAL_TIME_H_
