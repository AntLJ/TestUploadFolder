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
 * @file
 *
 * @note
 * ヘッダファイルや関数の名前は割と適当。<br>
 * もっと良い名前が浮かんだら変更を検討する。
 */
#ifndef RNS_REINTERPRET_PUSH_BACK_H_
#define RNS_REINTERPRET_PUSH_BACK_H_

#include <vector>
#include <algorithm>
#include <boost/next_prior.hpp>
#include <boost/static_assert.hpp>

namespace rns {

/**
 * @brief std::vector を使ったバイナリデータバッファの末尾にPOD型の値を追加する。
 *
 * @param[in,out] buffer バイナリデータバッファ
 * @param[in] val 追加する値
 */
template <typename Byte, typename T>
void reinterpret_push_back(std::vector<Byte>& buffer, const T& val)
{
	BOOST_STATIC_ASSERT(sizeof(Byte) == 1);
	
	const typename std::vector<Byte>::size_type size = buffer.size();
	const Byte* p = reinterpret_cast<const Byte*>(&val);
	
	buffer.resize(size + sizeof(T));
	std::copy(p, p + sizeof(T), boost::next(buffer.begin(), size));
}

} // namespace rns

#endif // RNS_REINTERPRET_PUSH_BACK_H_
