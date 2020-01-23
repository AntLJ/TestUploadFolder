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
 * @file winutil.h
 * @brief Windows ���[�e�B���e�B�[�֐��̒�`
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _WINUTIL_H_
#define _WINUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winutil {

bool BSTR2TCpy( LPTSTR lpszDst, BSTR bstrSrc, int nLen );

/// �E�B���h�E�̕�������擾����
CString GetWindowText( HWND hWnd );

/// ���Ԃ𕶎���Ŏ擾����
CString GetTime( LPSYSTEMTIME lpSystemTime = NULL );

/**
 * @brief <b>������Ń}�V�������擾����</b>\n
 * @return �}�V����
 */
CString GetMachineName();

/**
 * @brief <b>�������OS�����擾����</b>\n
 * @return OS��
 */
CString GetOperatingSystem();

/// �ʃE�B���h�E�Ńu���E�U��\������
void Navigate( LPCTSTR lpcszURL );

/// �N���b�v�{�[�h�ɕ�������R�s�[����֐�
bool SetToClipboard( const CString& strText );

/// �N���b�v�{�[�h�̕������Ԃ��֐�
CString GetFromClipboard();


} // winutil

#endif // _UTIL_H_
