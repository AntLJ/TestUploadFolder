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

// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__43A6B772_6307_44FF_9D17_8D69C912B0F6__INCLUDED_)
#define AFX_STDAFX_H__43A6B772_6307_44FF_9D17_8D69C912B0F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define _CRT_SECURE_NO_WARNINGS
#include "atlbase.h"

#pragma warning(push)
#pragma warning(disable : 4192)
#pragma warning(disable : 4146)

//#define INITGUID		// �����ADO���`���邽�߂̒萔(GUID)�̏�����
//#import "C:\Program Files\Common Files\System\ADO\msado15.dll" rename_namespace("ADOCG") rename("EOF", "EndOfFile")
//using namespace ADOCG;	// ���O���
//#include "icrsint.h"	// ADO���g���Ď擾�����t�B�[���h�̃f�[�^��ϊ�����}�N���Ȃǂ���`���ꂽ�w�b�_�[#pragma warning(pop)
#import "c:\Program Files\Common Files\system\ado\msadox.dll"  
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" rename("EOF", "EndOfFile")
//#import "c:\Program Files\Common Files\system\ado\msado15.dll"
#pragma warning(pop)

#define WIN32_LEAN_AND_MEAN // Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂�

#include <stdio.h>

// TODO: �v���O�����ŕK�v�ȃw�b�_�[�Q�Ƃ�ǉ����Ă��������B
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <io.h>
#include <atlstr.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__43A6B772_6307_44FF_9D17_8D69C912B0F6__INCLUDED_)
