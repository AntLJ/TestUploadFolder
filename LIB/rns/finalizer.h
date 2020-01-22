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
 * @brief �w�肵���֐����f�X�g���N�^�ŌĂяo���N���X
 *
 * @par �e���v���[�g����:
 * - <em>F</em> - 0�����֐��^
 */
template <typename F>
class finalizer_base : boost::noncopyable
{
public:
	/**
	 * @brief �f�X�g���N�^�ŌĂяo���֐����w�肷��B
	 *
	 * @attention
	 * �f�X�g���N�^����Ă΂��̂�����A���R�A��O�𑗏o����֐����w�肵�Ă͂Ȃ�Ȃ��B
	 */
	finalizer_base(F f) : m_f(f) {}

	/**
	 * @brief �w�肳�ꂽ�֐����Ăяo���B
	 */
	~finalizer_base() { m_f(); }

private:
	F m_f; ///< �f�X�g���N�^����Ăяo�����֐��B
};

typedef finalizer_base<boost::function<void ()> > finalizer;

} // namespace rns

#endif // RNS_FINALIZER_H_
