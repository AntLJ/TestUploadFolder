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
/////////////////////////////////////////////////////
// ���������[�N���o�p�w�b�_
// ���̃R�[�h��VC9��ATL�p�ł��B
// MFC�ł͉������Ȃ��Ă����o���\�ł��B
// VC8,VC6�ł͉��L�R�[�h�����ł͌��o�ł��܂���
/////////////////////////////////////////////////////

#ifndef _AFXDLL
///////////////////////////////
// ���������[�N���o malloc �p
///////////////////////////////
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
///////////////////////////////
// ���������[�N���o new �p
///////////////////////////////
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#pragma message("���������[�N�����o����ɂ̓v���O�����`���ŉ��L�R�[�h��ǉ����邱��")
#pragma message("_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );")
#endif // ifdef _DEBUG
#endif // ifndef _AFXDLL
