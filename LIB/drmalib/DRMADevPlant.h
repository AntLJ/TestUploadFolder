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

// DRMADevPlant.h: DRMADevPlant �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

/**
 * @file DRMADevPlant.h
 *
 * @brief ���C�u�����쐬�T�|�[�g�p�֐���`
 */
#if !defined(AFX_DRMADEVPLANT_H__C07B5E4C_B3F7_407F_8565_368068E01286__INCLUDED_)
#define AFX_DRMADEVPLANT_H__C07B5E4C_B3F7_407F_8565_368068E01286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include <assert.h>
#include <string>
#include <stdexcept>

#ifdef _WIN32
#pragma warning(disable : 4290)
#ifndef for
#define for if(0);else for
#endif // for
#endif // _WIN32 //

namespace drma {
	namespace dev_plant {
		extern const char _pathChr;

		const char* fileNameOf(const char* cFullPath);
		std::string makeSourceLocation(const char* cFileName, int cLine);
		std::string stringForDebugMessage(const char* cFileName, int cLine, const char* cMsg);

		/**
		 * @brief �����l�ŗ�O�𔭐�������
		 *
		 * @param cArg [in] ���ؑΏ�
		 * @param cFileName [in] �t�@�C����
		 * @param cLine [in] �s�ԍ�
		 * @param cInvalidMessage [in] �����l�G���[���b�Z�[�W
		 */
		template <typename T>
		T valid(T cArg, const char* cFileName, int cLine, const char* cInvalidMessage = "invalid value")
		{
			if(! cArg)
				throw std::runtime_error(stringForDebugMessage(cFileName, cLine, cInvalidMessage));

			return cArg;
		}
	}
}

// �G���[�����p�}�N��
#define VALID(e) drma::dev_plant::valid(e, __FILE__, __LINE__)
#define LOCATION drma::dev_plant::makeSourceLocation(__FILE__, __LINE__);

#endif // !defined(AFX_DRMADEVPLANT_H__C07B5E4C_B3F7_407F_8565_368068E01286__INCLUDED_)
