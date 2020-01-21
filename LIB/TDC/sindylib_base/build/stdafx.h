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

// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B
//

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �ꕔ�� CString �R���X�g���N�^�͖����I�ł��B

#if _MSC_VER < 1700
#include "memdbg.h" // ���������[�N���o
#endif // 


//#include "resource.h"
////////////////////////////
// ATL �T�|�[�g
////////////////////////////
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstr.h>

using namespace ATL;

#include "arcobjects.h" // ArcObjects Type Library �C���|�[�g

#if _MSC_VER >= 1700
#include "memdbg.h" // ���������[�N���o
#endif // 
////////////////////////////
////////////////////////////
////////////////////////////
#include <list>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <math.h>
#include "sindy/schema.h"
#include "CatIDs/ArcCATIDs.h"
#include "crd_cnv.h"
#include "macro_util.h"
#include "winutil.h"
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH
#include <TDC/useful_headers/tstring.h>

#pragma warning(disable: 4503)	// ���E���������Ă��������
#pragma warning(disable: 4786)	// �؂�̂Ă�ꂽ���Ă��������
#pragma warning(disable: 4290)	// ��O�w�肭�炢�A��������Ă��ǂ��c�񂶂�H


// VARIANT�^���m�̔�r�̍ہA�u�[(����)�v�Ɓu�\(�S�p�_�b�V��)�v�𓯂������ƌ��Ȃ��Ă��܂����߁A
// ����������ł���悤�㏑�����Ă���(bug5775, 11947)
// �K�v�ł���΁A�c�[���{�̂�stdafx.h�̍Ō�ɁA����operator�̐錾���R�s�y���Ă�������
bool operator==(const CComVariant& va1, const CComVariant& va2);
bool operator!=(const CComVariant& va1, const CComVariant& va2);
bool operator<(const CComVariant& va1, const CComVariant& va2);
bool operator>(const CComVariant& va1, const CComVariant& va2);

inline bool operator==(const CComVariant& va1, const CComVariant& va2)
{
	if(VT_BSTR == va1.vt && VT_BSTR == va2.vt)
		return (0 == wcscmp(va1.bstrVal, va2.bstrVal));
	return va1.operator==(va2);
}

inline bool operator!=(const CComVariant& va1, const CComVariant& va2)
{
	return !operator==(va1, va2);
}

inline bool operator<(const CComVariant& va1, const CComVariant& va2)
{
	if(VT_BSTR == va1.vt && VT_BSTR == va2.vt)
		return (0 > wcscmp(va1.bstrVal, va2.bstrVal));
	return va1.operator<(va2);
}

inline bool operator>(const CComVariant& va1, const CComVariant& va2)
{
	return (va2 < va1);
}
