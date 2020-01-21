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

#include "enum_def.h"
#include "JusyoHelper.h"

namespace addr
{

/**
 * @class	CAreaCodeHelper
 * @brief	Areacode_{Master|Kana}�ւ̃A�N�Z�X�w���p�[�N���X
 */
class CAreaCodeHelper : public CJusyoHelper
{
public:
	CAreaCodeHelper(void){}
	~CAreaCodeHelper(void){}

	//@{ @name �Z���R�[�h�֘A
	/**
	 * @brief	<b>�ŐV�Z���R�[�h�擾</b>
	 * @note	�X�g�A�h�t�@���N�V����GetNewestCode�����b�v��������
 	 * @param	lpcszAddrCode	[in]	11���Z���R�[�h
	 * @return	�ŐV�Z���R�[�h(�擾���s���́ANULL)
	 */
	CString GetNewestCode( const _TCHAR* lpcszAddrCode );

	/**
	 * @brief	<b>���Z���R�[�h�擾(�����O����)</b>
	 * @note	�X�g�A�h�t�@���N�V����GetPrefCode�����b�v��������
	 * @param	lpcszAddrCode	[in]	11���Z���R�[�h
	 * @return	���Z���R�[�h(�擾���s���́ANULL)
	 */
	CString GetPrevCode( const _TCHAR* lpcszAddrCode );

	/**
	 * @brief	<b>�Z���R�[�h�X�e�[�^�X�Ԃ�(�{�s�A�p�~�A����)</b>
	 * @param	lpcszAddrCode	[in]	11���Z���R�[�h
	 * @return	�R�[�h�X�e�[�^�X
	 */
	addrCodeStatus GetCodeStatus( const _TCHAR* lpcszAddrCode );
	//@}

	//@{ @name �Z�����̊֘A
	/**
	 * @brief	�Z�����擾
	 * @noet	�Z���R�[�h�����ɉ����Ė��̂��擾�ł��܂�
	 * @param	lpcszAddrCode	[in]	�Z���R�[�h(2, 5, 8, 11���w��)
	 * @return	�Z������(�擾���s���́ANULL)
	 */
	CString GetAddrKanji( const _TCHAR* lpcszAddrCode );

	/**
	 * @brief	�Z���J�i�擾
	 * @noet	�Z���R�[�h�����ɉ����ăJ�i���擾�ł��܂�
	 * @param	lpcszAddrCode	[in]	�Z���R�[�h(2, 5, 8, 11���w��)
	 * @return	�Z���J�i(�擾���s���́ANULL)
	 */
	CString GetAddrKana( const _TCHAR* lpcszAddrCode );
	//@}

	//@{ @name ���̑����擾�֘A
	/**
	 * @brief	�X�֔ԍ��擾
	 * @param	lpcszAddrCode	[in]	�Z���R�[�h
	 * @return	�X�֔ԍ��R�[�h(�擾���s���́ANULL)
	 */
	CString GetZipCode( const _TCHAR* lpcszAddrCode );
	//@}
};

extern CAreaCodeHelper	g_cAreaCodeHelper;

}	// namespace addr
