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
 * @file	enum_def.h
 * @brief	AddrLib�Ŏg��enum��`�l���낢��
 */
#ifndef __ADDRLIB_ENUM_DEF__
#define __ADDRLIB_ENUM_DEF__

namespace addr
{

/**
 * @enum	addrPointClass
 * @brief	�Z���|�C���g���
 */
enum addrPointClass
{
	kGou,		//!< ���|�C���g(GOU)
	kTpg,		//!< ���|�C���g(TPG)
	kCS,		//!< CS�|�C���g
	kKlink,		//!< �ƌ`�����N�|�C���g
	kUnknown,	//!< �s��
};

/**
 * @enum	addrStatus
 * @brief	�Z���X�e�[�^�X
 */
enum addrStatus
{
	kNormal,			//!< �ʏ�Z��
	kExtGaiku,			//!< �g���X�敄������
	kMojiGou,			//!< ���ԍ��ɕ�������
	kMojiGou_ExtGaiku,	//!< ���ԍ��ɕ�������(�g���X�敄����)
	kSepOver,			//!< ���ԍ��̋�؂肪����
	kSepOver_ExtGaiku,	//!< ���ԍ��̋�؂肪����(�g���X�敄����)
	kNoOver,			//!< �n�ԁE���ŕ\��������Ȃ��ԍ�
	kNoOver_ExtGaiku,	//!< �n�ԁE���ŕ\��������Ȃ��ԍ�(�g���X�敄����)
};

/**
 * @enum	addrCodeStatus
 * @brief	�Z���R�[�h�X�e�[�^�X
 */
enum addrCodeStatus
{
	kShiko,		//!< �{�s�R�[�h
	kTmpShiko,	//!< ���{�s�R�[�h
	kHaishi,	//!< �p�~�R�[�h
	kTmpHaishi,	//!< ���p�~�R�[�h
	kInValid,	//!< �����R�[�h
};

}	// namespace addr

#endif	// __ADDRLIB_ENUM_DEF__
