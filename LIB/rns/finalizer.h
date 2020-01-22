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

#ifndef RNS_FINALIZER_H_
#define RNS_FINALIZER_H_

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

namespace rns {

/**
 * @brief 指定した関数をデストラクタで呼び出すクラス
 *
 * @par テンプレート引数:
 * - <em>F</em> - 0引数関数型
 */
template <typename F>
class finalizer_base : boost::noncopyable
{
public:
	/**
	 * @brief デストラクタで呼び出す関数を指定する。
	 *
	 * @attention
	 * デストラクタから呼ばれるのだから、当然、例外を送出する関数を指定してはならない。
	 */
	finalizer_base(F f) : m_f(f) {}

	/**
	 * @brief 指定された関数を呼び出す。
	 */
	~finalizer_base() { m_f(); }

private:
	F m_f; ///< デストラクタから呼び出される関数。
};

typedef finalizer_base<boost::function<void ()> > finalizer;

} // namespace rns

#endif // RNS_FINALIZER_H_
