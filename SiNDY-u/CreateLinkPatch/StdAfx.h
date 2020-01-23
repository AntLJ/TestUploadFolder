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
 *	@file StdAfx.h
 *	@brief �W���V�X�e���C���N���[�h�t�@�C�� ��`��
 *	@author	F.Adachi
 *	@date	2005/10/18		�V�K�쐬
 *	$ID$
 */

// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__6780A134_E1B0_4A91_B314_862E128BDD51__INCLUDED_)
#define AFX_STDAFX_H__6780A134_E1B0_4A91_B314_862E128BDD51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlbase.h>			// CComBSTR, _T

#pragma warning(push)
#pragma warning(disable : 4146)		// VC++�̃o�O�΍�
#pragma warning(disable : 4192)		// exclude�������w�肷�ׂ��H

#include "WinLib\arcobjects_import_highmethod.h"
#include "WinLib\arcobjects_import_rawmethod.h"

#pragma warning(pop)

#include <atlstr.h>

// TODO: �v���O�����ŕK�v�ȃw�b�_�[�Q�Ƃ�ǉ����Ă��������B
#include <ostream>
#include <iostream>
#include <boost/utility.hpp>		// boost::noncopyable

#include "ZCL/libzcl.h"
#include "crd_cnv/coord_converter.h"
#include "WKH/libwkh.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__6780A134_E1B0_4A91_B314_862E128BDD51__INCLUDED_)
