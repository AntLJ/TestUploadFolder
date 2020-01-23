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

// RNSPlant.h: RNSPlant �N���X�̃C���^�[�t�F�C�X
//
// $Id: RNSPlant.h,v 1.5 2003/10/09 06:56:47 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNSPLANT_H__FCED50F1_A521_455F_AA0D_50E565101206__INCLUDED_)
#define AFX_RNSPLANT_H__FCED50F1_A521_455F_AA0D_50E565101206__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RNS.h"

#include <iostream>
#include <string>

struct RNSPlant  
{
public:

	/// �f�B���N�g���ύX
	static bool changeDirectoryToFile(const char* cFileName);

	/// �t���p�X����t�@�C�����̊J�n�ʒu���擾����
	static const char* fileNameOf(const char* cFullPath);

	/// �t�@�C�����ƍs�ԍ����������āA�\�[�X�ʒu���莯�ʕ�������쐬����
	static std::string makeSourceLocation(const char* cFileName, int cLine);

	/// �f�o�b�O���b�Z�[�W�o�͗p��������쐬����
	static std::string stringForDebugMessage(const char* cFileName, int cLine, const char* cMsg);

	/// �W���G���[�o�͂Ƀ��b�Z�[�W���o�͂���
	static void trace(std::ostream& cStream, const char* cMsg);

	/**
	 * @brief	�����l�ŗ�O�𔭐�������
	 *
	 * @param	cArg			[in]	���ؑΏ�
	 * @param	cFileName		[in]	�t�@�C����
	 * @param	cLine			[in]	�s�ԍ�
	 * @param	cInvalidMessage	[in]	�����l�G���[���b�Z�[�W
	 */
	template <typename T>
	static T valid(T cArg, const char* cFileName, int cLine, const char* cInvalidMessage = "invalid value")
	{
		if(! cArg)
			throw std::runtime_error(RNSPlant::stringForDebugMessage(cFileName, cLine, cInvalidMessage));

		return cArg;
	}

	/**
	 * @brief	0�̏ꍇ��O�𔭐�������
	 *
	 * @param	cArg			[in]	���ؑΏ�
	 * @param	cFileName		[in]	�t�@�C����
	 * @param	cLine			[in]	�s�ԍ�
	 * @param	cInvalidMessage	[in]	�����l�G���[���b�Z�[�W
	 */
	template <typename T>
	static T nonZero(T cArg, const char* cFileName, int cLine, const char* cInvalidMessage = "invalid value")
	{
		if(cArg == 0)
			throw std::runtime_error(RNSPlant::stringForDebugMessage(cFileName, cLine, cInvalidMessage));

		return cArg;
	}

	/**
	 * @brief	NULL�|�C���^�ŗ�O�𔭐�������
	 *
	 * @param	cPointer		[in]	���ؑΏۃ|�C���^
	 * @param	cFileName		[in]	�t�@�C����
	 * @param	cLine			[in]	�s�ԍ�
	 * @param	cInvalidMessage	[in]	NULL�|�C���^�������ꍇ�̃G���[���b�Z�[�W
	 */
	template <typename T>
	static T* valid_pointer(T* cPointer, const char* cFileName, int cLine, const char* cMessage = "NULL pointer found")
	{
		return nonZero(cPointer, cFileName, cLine, cMessage);
	}
};

/// RNSPlant.h �� �d�l�ɒu�������邽�߁A��`����������
#ifdef RNS_LOCATION
#undef RNS_LOCATION
#endif

/**
 * @brief	RNSPlant::makeSourceLocation() ���g�p���ă\�[�X�ʒu���莯�ʕ�������쐬����
 *
 * @return	�\�[�X�ʒu���莯�ʕ�����
 */
#define RNS_LOCATION_STRING	RNSPlant::makeSourceLocation(__FILE__, __LINE__)

/**
 * @brief RNS_LOCATION_STRING ���g�p���ă\�[�X�ʒu���莯�ʕ�������쐬���A�����C������ɕϊ�����
 *
 * RNS.h �Œ�`����Ă��� RNS_LOCATION �ɔ�ׂ�ƁA�����Ԓx���B
 * �܂�������͑����Ƀf�X�g���N�g����Ă��܂����߁A�p���I�Ɏg�p����ꍇ�̓R�s�[����K�v������B
 *
 * @return �\�[�X�ʒu���莯�ʕ�����
 */
#define RNS_LOCATION		RNS_LOCATION_STRING.c_str()

#define RNSTRACE(msg)	RNSPlant::trace(std::cerr, msg)
#define RNSTRACEEX(msg)	RNSPlant::trace(std::cerr, (RNS_LOCAION_STRING += msg).c_str())

#define RNS_DEBUG_STRING(msg)	RNSPlant::stringForDebugMessage(__FILE__, __LINE__, msg)
#define RNS_DEBUG_CSTR(msg)		RNSPlant::stringForDebugMessage(__FILE__, __LINE__, msg).c_str()

#define NONZERO(e)			RNSPlant::nonZero(e,  __FILE__, __LINE__)
#define VALID(e)			RNSPlant::valid(e, __FILE__, __LINE__)
#define VALID_POINTER(e)	RNSPlant::valid_pointer(e, __FILE__, __LINE__)

#endif // !defined(AFX_RNSPLANT_H__FCED50F1_A521_455F_AA0D_50E565101206__INCLUDED_)
