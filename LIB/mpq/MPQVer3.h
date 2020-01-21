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
** MPQVer3.h
**
**		MPQ IO for format ver.3 class header file
**
**		Programmed by Y.Chinen
**
**      EDITION:
**			2008/10/30	�쐬
*/

#ifndef ___MPQ_VER3___
#define ___MPQ_VER3___

#include "MPQVer1.h"


class MPQVer3 : public MPQVer1
{
public:
	struct Info
	{
		unsigned	eRes_			: 10;
		unsigned	eDeletion_End	: 1;	//!< �폜�Ώۋ�ԏI�[�t���O
		unsigned	eDeletion_Sct	: 1;	//!< �폜�Ώۃt���O

		unsigned	eTunnel_Res		: 2;
		unsigned	eTunnel_End		: 1;    //!< �g���l����ԏI�[�t���O
		unsigned	eTunnel_Sct		: 1;    //!< �g���l����ԃt���O

		unsigned	eComment_Res	: 4;
		unsigned	eComment_End	: 1;	//!< �R�����g��ԏI�[�t���O
		
		unsigned	eObject_Res		: 1;
		unsigned	eObject_End		: 1;    //!< ��p�I�u�W�F�N�g�I�[�t���O
		unsigned	eObject_Sct		: 1;    //!< ��p�I�u�W�F�N�g�t���O

		unsigned	eHeight_Res		: 2;
		unsigned	eHeight_MEnd	: 1;	//!< �}�b�`���O�p������ԏI�[�t���O
		unsigned	eHeight_MFlag	: 1;	//!< �}�b�`���O�p�����t���O
		unsigned	eHeight_GType	: 1;    //!< �n�Ս��擾�^�C�v
		unsigned	eHeight_GFlag	: 1;    //!< �n�Ս��擾�t���O
		unsigned	eHeight_DEnd	: 1;    //!< �`��p������ԏI�[�t���O
		unsigned	eHeight_DSct	: 1;    //!< �`��p�����t���O

		Info(void);
	};


public:
	MPQVer3(void) { }
	~MPQVer3(void) { }

	virtual int	getVer(void) const { return 3; }

	static Info GetInfo(const MPQ::Attr& cAttr);
	static void SetInfo(MPQ::Attr* cAttr, const Info& cInfo);
};

#endif	// ___MPQ_VER3___
