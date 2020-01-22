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
 * @brief 現在時刻を現地時間帯の設定に合わせて出力する。
 *
 * @param os [in] 出力ストリーム。
 * @return 出力ストリーム。
 *
 * @attention
 * ctime() 関数の仕様に則り、時刻文字列の後ろには改行文字を付与する。
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
