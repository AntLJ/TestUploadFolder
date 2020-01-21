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

/*
**	MPQVer2.h
**
**		MPQ IO for format ver.2 class  header file
**
**		Programmed by H.Miyahara
**
**		EDITION:
**			2005/7/27	�쐬
*/

#ifndef	___MPQ_VER2___
#define	___MPQ_VER2___

#include "MPQVer1.h"

class MPQVer2 : public MPQVer1
{
public:
    struct Info
    {
		unsigned	eRes_		: 11;
		unsigned	eNotDraw	: 1;    //!< �`��s�t���O

		unsigned	eTunnel_Res	: 2;
		unsigned	eTunnel_End	: 1;    //!< �g���l����ԏI�[�t���O
		unsigned	eTunnel_Sct	: 1;    //!< �g���l����ԃt���O

		unsigned	eObject_Res	: 6;
		unsigned	eObject_End	: 1;    //!< ��p�I�u�W�F�N�g�I�[�t���O
		unsigned	eObject_Sct	: 1;    //!< ��p�I�u�W�F�N�g�t���O

		unsigned	eHeight_Res		: 4;
		unsigned	eHeight_GType	: 1;    //!< �n�Ս��擾�^�C�v
		unsigned	eHeight_GFlag	: 1;    //!< �n�Ս��擾�t���O
		unsigned	eHeight_End		: 1;    //!< ������ԏI�[�t���O
		unsigned	eHeight_Sct		: 1;    //!< �����t���O

		Info(void);
    };

public:
	MPQVer2(void) { }
	virtual ~MPQVer2() { }

	virtual int		getVer(void) const { return 2; }

	static Info	GetInfo(const MPQ::Attr& cAttr);
	static void	SetInfo(MPQ::Attr* cAttr, const Info& cInfo);
};

#endif	// ___MPQ_VER2___

