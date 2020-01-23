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

// SindyTimeNoPassage.h: SindyTimeNoPassage �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYTIMENOPASSAGE_H__58071F55_4DFC_4845_8026_510B22601D64__INCLUDED_)
#define AFX_SINDYTIMENOPASSAGE_H__58071F55_4DFC_4845_8026_510B22601D64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyLinkTable.h"

class SindyTimeNoPassage : public SindyLinkTable  
{
public:
	SindyTimeNoPassage(SindyDBConnection *cDBConnection);
	virtual ~SindyTimeNoPassage();

};

#endif // !defined(AFX_SINDYTIMENOPASSAGE_H__58071F55_4DFC_4845_8026_510B22601D64__INCLUDED_)