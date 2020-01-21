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

#include <functional>

namespace uh{
	/**
	* @brief �X�R�[�v�K�[�h�N���X
	* @note �X�R�[�v�𔲂���ۂɕK�����������s�����邱�Ƃ��ł��܂�
	*/
	class scope_guard
	{
		/**
		* @brief �R���X�g���N�^
		* @note �g�p���Ȃ�
		*/
		scope_guard();
	public:
		/**
		* @brief �֐�
		* @param f [in] ���s����֐��i�֐��|�C���^�A�����_�����j
		*/
		explicit scope_guard( std::function<void()> f ): func(f){}
		/**
		* @brief �f�X�g���N�^
		*/
		~scope_guard(){ func(); }

	private:
		std::function<void()> func; //!< ���s����֐�
	};
}
