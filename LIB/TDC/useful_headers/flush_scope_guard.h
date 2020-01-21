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

namespace uh
{
	/**
	* @brief flush�p�X�R�[�v�K�[�h�N���X
	* @note Arc10.3.1�ŕW���i�G���[�j�o�͂����_�C���N�g����ƌ�����ꍇ������
	*       ��L�ւ̑Ή��Ƃ��ăX�R�[�v�𔲂���ۂɕK��std::flush�����s������
	*/
	class flush_scope_guard
	{
	public:
		/**
		* @brief �R���X�g���N�^
		*/
		flush_scope_guard(){}
		/**
		* @brief �f�X�g���N�^
		*/
		~flush_scope_guard(){
			std::cout<<std::flush;
			std::cerr<<std::flush;
		}

	private:
	};
}
